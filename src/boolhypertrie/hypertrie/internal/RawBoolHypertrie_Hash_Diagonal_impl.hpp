#ifndef HYPERTRIE_RAWBOOLHYPERTRIE_HASH_DIAGONAL_IMPL_HPP
#define HYPERTRIE_RAWBOOLHYPERTRIE_HASH_DIAGONAL_IMPL_HPP

#include "hypertrie/internal/util/CONSTANTS.hpp"

#include "hypertrie/internal/RawBoolHypertrie_impl.hpp"
#include "hypertrie/internal/container/TslMap.hpp"
#include "hypertrie/internal/container/BoostFlatSet.hpp"
#include <numeric>

namespace hypertrie::internal {

	template<pos_type diag_depth, pos_type depth, typename key_part_type, template<typename, typename> typename map_type,
			template<typename> typename set_type>
	class RawHashDiagonal<diag_depth, depth, key_part_type, map_type, set_type, std::enable_if_t<(
			depth == diag_depth and depth >= 1)>> {

		template<pos_type depth_>
		using RawBoolHypertrie_t = RawBoolHypertrie<depth_, key_part_type, map_type, set_type>;
		using children_type = typename RawBoolHypertrie_t<depth>::children_type;
		using child_type = typename RawBoolHypertrie_t<depth>::child_type;
	private:
		RawBoolHypertrie_t<depth> const &rawboolhypertrie;
		typename children_type::const_iterator iter;
		typename children_type::const_iterator end;

	public:
		explicit RawHashDiagonal(const RawBoolHypertrie_t<depth> &boolhypertrie) :
				rawboolhypertrie{boolhypertrie} {}

		static void init(void *diag_ptr) {
			auto &diag = *static_cast<RawHashDiagonal *>(diag_ptr);
			if constexpr (depth > 1) {
				const auto min_card_pos = diag.rawboolhypertrie.minCardPos();
				const auto &min_dim_edges = diag.rawboolhypertrie.edges[min_card_pos];
				diag.iter = min_dim_edges.begin();
				diag.end = min_dim_edges.end();
				if (not empty(diag_ptr)) {
					if (not diag.iter->second->diagonal(diag.iter->first)) {
						inc(diag_ptr);
					}
				}
			} else {
				diag.iter = diag.rawboolhypertrie.edges.begin();
				diag.end = diag.rawboolhypertrie.edges.end();
			}
		}

		static key_part_type currentKeyPart(void const *diag_ptr) {
			auto &diag = *static_cast<RawHashDiagonal const *>(diag_ptr);
			if constexpr (depth > 1)
				return diag.iter->first;
			else
				return *diag.iter;
		}

		static bool getValueByKeyPart(void const *diag_ptr, key_part_type key_part) {
			auto &diag = *static_cast<RawHashDiagonal const *>(diag_ptr);
			return diag.rawboolhypertrie.diagonal(key_part);
		}

		static void inc(void *diag_ptr) {
			auto &diag = *static_cast<RawHashDiagonal *>(diag_ptr);
			if constexpr  (depth > 1) {
				assert(not empty(diag_ptr));
				do {
					++diag.iter;
				} while (not empty(diag_ptr) and not diag.iter->second->diagonal(diag.iter->first));
			} else {
				++diag.iter;
			}
		}

		static bool empty(void const *diag_ptr) {
			auto &diag = *static_cast<RawHashDiagonal const *>(diag_ptr);
			return diag.iter == diag.end;
		}

		static size_t size(void const *diag_ptr) {
			auto &diag = *static_cast<RawHashDiagonal const *>(diag_ptr);
			if constexpr (depth > 1) {
				const auto min_card_pos = diag.rawboolhypertrie.minCardPos();
				return diag.rawboolhypertrie.edges[min_card_pos].size();
			} else {
				return diag.rawboolhypertrie.size();
			}
		}
	};

	template<pos_type diag_depth, pos_type depth, typename key_part_type, template<typename, typename> typename map_type,
			template<typename> typename set_type>
	class RawHashDiagonal<diag_depth, depth, key_part_type, map_type, set_type, std::enable_if_t<(
			depth > diag_depth and depth > 1)>> {

		template<pos_type depth_>
		using RawBoolHypertrie_t = RawBoolHypertrie<depth_, key_part_type, map_type, set_type>;
		using children_type = typename RawBoolHypertrie_t<depth>::children_type;
		using child_type = typename RawBoolHypertrie_t<depth>::child_type;
	public:
		using value_type = std::shared_ptr<RawBoolHypertrie_t<depth - diag_depth> const>;
	private:
		mutable RawBoolHypertrie_t<depth> const *rawboolhypertrie;
		std::vector<pos_type> diag_poss;
		typename children_type::const_iterator iter;
		typename children_type::const_iterator end;
		value_type value;

	public:
		RawHashDiagonal(RawBoolHypertrie_t<depth> const *const boolhypertrie, std::vector<pos_type> positions)
				: rawboolhypertrie{boolhypertrie}, diag_poss{std::move(positions)} {}

		RawHashDiagonal(RawBoolHypertrie_t<depth> const &boolhypertrie, std::vector<pos_type> positions)
				: RawHashDiagonal(&boolhypertrie, positions) {}

		RawHashDiagonal(std::shared_ptr<RawBoolHypertrie_t<depth> const> const &boolhypertrie,
		            std::vector<pos_type> positions) : RawHashDiagonal(boolhypertrie.get(), positions) {}

		static void init(void *diag_ptr) {
			auto &diag = *static_cast<RawHashDiagonal *>(diag_ptr);
			auto min_card_pos_it = diag.rawboolhypertrie->minCardPos(diag.diag_poss);
			const auto &min_dim_edges = diag.rawboolhypertrie->edges[*min_card_pos_it];
			if constexpr (diag_depth > 1) {
				auto min_card_pos = *min_card_pos_it;
				diag.diag_poss.erase(min_card_pos_it);
				auto posCalc = util::PosCalc::getInstance(depth)->use(min_card_pos);
				for (auto &pos : diag.diag_poss)
					pos = posCalc->key_to_subkey_pos(pos);
			}
			diag.iter = min_dim_edges.begin();
			diag.end = min_dim_edges.end();
			if (not empty(diag_ptr)) {
				if constexpr (diag_depth > 1) {
					typename child_type::SliceKey slice_key{};
					for (const auto pos : diag.diag_poss) {
						slice_key[pos] = diag.iter->first;
					}
					diag.value = diag.iter->second->template operator[]<(depth  - diag_depth )>(slice_key);
					if (not bool(diag.value))
						inc(diag_ptr);
				}
			}
		}

		static key_part_type currentKeyPart(void const *diag_ptr) {
			auto &diag = *static_cast<RawHashDiagonal const *>(diag_ptr);
			return diag.iter->first;
		}

		static value_type currentValue(void const *diag_ptr) {
			auto &diag = *static_cast<RawHashDiagonal const *>(diag_ptr);
			if constexpr (diag_depth == 1)
				return diag.iter->second;
			else
				return diag.value;
		}

		static value_type getValueByKeyPart(void const *diag_ptr, key_part_type key_part) {
			auto &diag = *static_cast<RawHashDiagonal const *>(diag_ptr);
			if constexpr (diag_depth == 1) {
				return diag.rawboolhypertrie->get(diag.diag_poss[0], key_part);
			} else {

				typename RawBoolHypertrie_t<depth>::SliceKey slice_key{};
				for (const auto pos : diag.diag_poss) {
					slice_key[pos] = key_part;
				}
				return diag.rawboolhypertrie->template operator[]<(depth - diag_depth)>(slice_key);
			}
		}

		static void inc(void *diag_ptr) {
			auto &diag = *static_cast<RawHashDiagonal *>(diag_ptr);
			if constexpr  (diag_depth == 1) {
				++diag.iter;
			} else {
				assert(not empty(diag_ptr));
				typename child_type::SliceKey slice_key{};
				do {
					++diag.iter;
					if (empty(diag_ptr))
						return;
					for (const auto pos : diag.diag_poss) {
						slice_key[pos] = diag.iter->first;
					}
					diag.value = diag.iter->second->template operator[]<(depth - diag_depth)>(slice_key);
				} while (not bool(diag.value));
			}
		}

		static bool empty(void const *diag_ptr) {
			auto &diag = *static_cast<RawHashDiagonal const *>(diag_ptr);
			return diag.iter == diag.end;
		}

		static size_t size(void const *diag_ptr) {
			auto &diag = *static_cast<RawHashDiagonal const *>(diag_ptr);
			if constexpr (depth > 1) {
				const auto min_card_pos = diag.rawboolhypertrie->minCardPos();
				return diag.rawboolhypertrie->edges[min_card_pos].size();
			} else {
				return diag.boolhypertrie.size();
			}
		}
	};


}

#endif //HYPERTRIE_RAWBOOLHYPERTRIE_HASH_DIAGONAL_IMPL_HPP

