#ifndef HYPERTRIE_BOOLHYPERTRIE_HASH_DIAGONAL_HPP
#define HYPERTRIE_BOOLHYPERTRIE_HASH_DIAGONAL_HPP

#include <functional>
#include <exception>

#include "hypertrie/internal/util/CONSTANTS.hpp"

#include "hypertrie/internal/container/TslMap.hpp"
#include "hypertrie/internal/container/BoostFlatSet.hpp"
#include "hypertrie/internal/BoolHypertrie_impl.hpp"

namespace hypertrie::internal {


	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class HashDiagonal {

		using const_BoolHypertrie_t = const_BoolHypertrie<key_part_type, map_type, set_type>;
		template<pos_type depth>
		using RawBoolHypertrie = typename const_BoolHypertrie_t::template RawBoolHypertrie<depth>;
		using Key = typename const_BoolHypertrie_t::Key;
		using SliceKey = typename const_BoolHypertrie_t::SliceKey;
		template<pos_type depth, pos_type diag_depth>
		using RawHashDiagonal =  typename hypertrie::internal::interface::rawboolhypertrie<key_part_type, map_type, set_type>::template RawHashDiagonal<diag_depth, depth>;

		struct RawDiagFunctions {
			void (*init)(void *);

			key_part_type (*currentKeyPart)(void const *);

			const_BoolHypertrie_t (*currentValue)(void const *);

			std::shared_ptr<void const> (*getValueByKeyPart)(void const *, key_part_type);

			bool (*contains)(void const *, key_part_type);

			void (*inc)(void *);

			bool (*empty)(void const *);

			size_t (*size)(void const *);
		};

		template<pos_type diag_depth_, pos_type depth>
		static auto call_currentValue([[maybe_unused]]void const *diag_ptr) -> const_BoolHypertrie_t {
			if constexpr (depth > diag_depth_) {
				return const_BoolHypertrie_t(RawHashDiagonal<diag_depth_, depth>::currentValue(diag_ptr));
			} else {
				throw std::invalid_argument{"currentValue is only implemented for depth > diag_depth"};
			}
		}

		template<pos_type diag_depth_, pos_type depth>
		static auto call_getValueByKeyPart([[maybe_unused]]void const *diag_ptr,
		                                   [[maybe_unused]]key_part_type key_part) -> std::shared_ptr<void const> {
			if constexpr (depth > diag_depth_) {
				return RawHashDiagonal<diag_depth_, depth>::getValueByKeyPart(diag_ptr, key_part);
			} else {
				throw std::invalid_argument{"currentValue is only implemented for depth > diag_depth"};
			}
		}

		template<pos_type diag_depth_, pos_type depth>
		static auto
		call_contains([[maybe_unused]]void const *diag_ptr, [[maybe_unused]]key_part_type key_part) -> bool {
			if constexpr (depth > diag_depth_) {
				throw std::invalid_argument{"currentValue is only implemented for depth > diag_depth"};
			} else {
				return bool(RawHashDiagonal<diag_depth_, depth>::getValueByKeyPart(diag_ptr, key_part));
			}
		}

		template<pos_type depth, pos_type diag_depth_>
		inline static RawDiagFunctions getRawDiagFunctions() {
			return RawDiagFunctions{
					&RawHashDiagonal<diag_depth_, depth>::init,
					&RawHashDiagonal<diag_depth_, depth>::currentKeyPart,
					&call_currentValue<diag_depth_, depth>,
					&call_getValueByKeyPart<diag_depth_, depth>,
					&call_contains<diag_depth_, depth>,
					&RawHashDiagonal<diag_depth_, depth>::inc,
					&RawHashDiagonal<diag_depth_, depth>::empty,
					&RawHashDiagonal<diag_depth_, depth>::size
			};
		}

		inline static std::vector<std::vector<RawDiagFunctions>> functions{
				{
						getRawDiagFunctions<1, 1>()
				},
				{
						getRawDiagFunctions<2, 1>(),
						getRawDiagFunctions<2, 2>()
				},
				{
						getRawDiagFunctions<3, 1>(),
						getRawDiagFunctions<3, 2>(),
						getRawDiagFunctions<3, 3>()
				},
				{
						getRawDiagFunctions<4, 1>(),
						getRawDiagFunctions<4, 2>(),
						getRawDiagFunctions<4, 3>(),
						getRawDiagFunctions<4, 4>(),
				},
				{
						getRawDiagFunctions<5, 1>(),
						getRawDiagFunctions<5, 2>(),
						getRawDiagFunctions<5, 3>(),
						getRawDiagFunctions<5, 4>(),
						getRawDiagFunctions<5, 5>(),
				}
		};

	public:
		using poss_type = std::vector<pos_type>;
	private:

		std::shared_ptr<void> raw_diag;
		RawDiagFunctions *raw_diag_funcs;

		template<pos_type diag_depth_, pos_type depth>
		static inline std::shared_ptr<void>
		getRawDiagonal(const const_BoolHypertrie_t &boolhypertrie, [[maybe_unused]]const poss_type &positions) {
			if constexpr (depth == diag_depth_) {
				const auto &raw_boolhypertrie = *(static_cast<RawBoolHypertrie<depth> const *>(boolhypertrie.hypertrie.get()));
				return std::make_shared<RawHashDiagonal<diag_depth_, depth>>(raw_boolhypertrie);
			} else {
				const auto &raw_boolhypertrie = *(static_cast<RawBoolHypertrie<depth> const *>(boolhypertrie.hypertrie.get()));
				return std::make_shared<RawHashDiagonal<diag_depth_, depth>>(raw_boolhypertrie, positions);
			}
		}

		static inline std::shared_ptr<void>
		getRawDiagonal(const const_BoolHypertrie_t &boolhypertrie, const poss_type &positions) {
			switch (boolhypertrie.depth()) {
				case 1: {
					return getRawDiagonal<1, 1>(boolhypertrie, positions);
				}
				case 2: {
					switch (positions.size()) {
						case 1: {
							return getRawDiagonal<1, 2>(boolhypertrie, positions);
						}
						case 2: {
							return getRawDiagonal<2, 2>(boolhypertrie, positions);
						}
						default:
							break;
					}
					break;
				}
				case 3: {
					switch (positions.size()) {
						case 1: {
							return getRawDiagonal<1, 3>(boolhypertrie, positions);
						}
						case 2: {
							return getRawDiagonal<2, 3>(boolhypertrie, positions);
						}
						case 3: {
							return getRawDiagonal<3, 3>(boolhypertrie, positions);
						}
						default:
							break;
					}
					break;
				}
				case 4: {
					switch (positions.size()) {
						case 1: {
							return getRawDiagonal<1, 4>(boolhypertrie, positions);
						}
						case 2: {
							return getRawDiagonal<2, 4>(boolhypertrie, positions);
						}
						case 3: {
							return getRawDiagonal<3, 4>(boolhypertrie, positions);
						}
						case 4: {
							return getRawDiagonal<4, 4>(boolhypertrie, positions);
						}
						default:
							break;
					}
					break;
				}
				case 5: {
					switch (positions.size()) {
						case 1: {
							return getRawDiagonal<1, 5>(boolhypertrie, positions);
						}
						case 2: {
							return getRawDiagonal<2, 5>(boolhypertrie, positions);
						}
						case 3: {
							return getRawDiagonal<3, 5>(boolhypertrie, positions);
						}
						case 4: {
							return getRawDiagonal<4, 5>(boolhypertrie, positions);
						}
						case 5: {
							return getRawDiagonal<5, 5>(boolhypertrie, positions);
						}
						default:
							break;
					}
					break;
				}
				default:
					break;
			}
			throw std::logic_error{"not implemented."};
		}

	public:

		HashDiagonal() = default;

		HashDiagonal(HashDiagonal &) = default;

		HashDiagonal(const HashDiagonal &) = default;

		HashDiagonal(HashDiagonal &&) noexcept = default;

		HashDiagonal &operator=(HashDiagonal &&) = default;

		HashDiagonal &operator=(const HashDiagonal &) = default;


		HashDiagonal(const_BoolHypertrie_t const *const boolhypertrie, const poss_type &positions) :
				raw_diag(getRawDiagonal(*boolhypertrie, positions)),
				raw_diag_funcs(&functions[boolhypertrie->depth() - 1][positions.size() - 1]) {}

		HashDiagonal(const const_BoolHypertrie_t &boolhypertrie, const poss_type &positions) :
				HashDiagonal(&boolhypertrie, positions) {}


		/*
		* Potentially forwards the Diagonal and leafs it in a safe state. <br/>
		* It checks if the current key_part is valid and increments it until it is valid.
		*/
		void init() const { // #
			raw_diag_funcs->init(raw_diag.get());
		}

		/*
		* Must only be called in a safe state. <br/>
		* Returns the current value.
		*/
		[[nodiscard]]
		key_part_type currentKeyPart() const { // #
			return raw_diag_funcs->currentKeyPart(raw_diag.get());
		}

		[[nodiscard]]
		const_BoolHypertrie_t currentValue() const {
			return raw_diag_funcs->currentValue(raw_diag.get());
		}

		/**
		 * use only if the diagonal is not calculated over all dimensions.
		 * @param key_part
		 * @return
		 */
		[[nodiscard]]
		std::shared_ptr<void const> operator[](key_part_type key_part) {
			return raw_diag_funcs->getValueByKeyPart(raw_diag.get(), key_part);
		}

		/**
		 * use only if the diagonal is calculated over all dimensions.
		 * @param key_part
		 * @return
		 */
		[[nodiscard]]
		bool contains(key_part_type key_part) {
			return raw_diag_funcs->contains(raw_diag.get(), key_part);
		}

		/*
		* Forwards the Diagonal and leafs it in a safe state. <br/>
		* Increments the diagonal to the next valid key_part.
		*/
		void operator++() { // #
			return raw_diag_funcs->inc(raw_diag.get());
		}

		/*
		* If it returns true there are no key_parts left for sure.
		* Otherwise there are potential key_parts left and therefore, there may also be valid key_parts left.
		* @return
		*/
		[[nodiscard]]
		bool empty() const { // #
			return raw_diag_funcs->empty(raw_diag.get());
		}

		/*
		* Always safe. <br/>
		* Get the number of potential key_parts. This is a upper bound to the valid key_parts.
		* @return number of potential key_parts
		*/
		[[nodiscard]]
		size_t size() const {
			return raw_diag_funcs->size(raw_diag.get());
		}

		bool operator<(const HashDiagonal &other) const {
			return this->size() < other.size();
		}
	};
}

#endif //HYPERTRIE_BOOLHYPERTRIE_HASH_DIAGONAL_HPP
