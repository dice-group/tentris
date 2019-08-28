#ifndef HYPERTRIE_BOOLHYPERTRIE_IMPL_H
#define HYPERTRIE_BOOLHYPERTRIE_IMPL_H

#include <tuple>

#include <utility>
#include <tsl/hopscotch_map.h>
#include <tsl/hopscotch_set.h>
#include <memory>
#include <variant>
#include <vector>
#include <cassert>

#include <itertools.hpp>
#include "hypertrie/internal/util/CONSTANTS.hpp"
#include "hypertrie/internal/util/NTuple.hpp"
#include "hypertrie/internal/util/CountDownNTuple.hpp"
#include "hypertrie/internal/util/FrontSkipIterator.hpp"
#include "hypertrie/internal/util/PosCalc.hpp"
#include "hypertrie/internal/container/AllContainer.hpp"


// what the DUMMY* template parameters do: https://stackoverflow.com/questions/10178598/specializing-a-templated-member-of-a-template-class

namespace hypertrie::internal {
	namespace {
		using namespace iter;
	}

	constexpr int factorial(int n) {
		return n <= 1 ? 1 : (n * factorial(n - 1));
	}

	template<pos_type diag_depth, pos_type depth, typename key_part_type, template<typename, typename> typename map_type,
			template<typename> typename set_type, typename = typename std::enable_if_t<(diag_depth <= depth)>>
	class RawHashDiagonal;

	template<pos_type diag_depth, pos_type depth, typename key_part_type, template<typename, typename> typename map_type,
			template<typename> typename set_type, typename = typename std::enable_if_t<(diag_depth <= depth)>>
	class RawOrderedDiagonal;

	using pos_type = uint8_t;

	template<pos_type depth, typename key_part_type>
	using RawKey = std::array<key_part_type, depth>;

	template<pos_type depth, typename key_part_type>
	using RawSliceKey = std::array<std::optional<key_part_type>, depth>;

	using size_t = std::size_t;

	template<pos_type depth, typename key_part_type, template<typename, typename> typename map_type,
			template<typename> typename set_type, typename  = typename std::enable_if_t<(
					depth >= 1)>>
	class RawBoolHypertrie;


	template<typename key_part_type_t, template<typename, typename> typename map_type_t,
			template<typename> typename set_type_t> //
	class RawBoolHypertrie<1, key_part_type_t, map_type_t, set_type_t> {
		// make the bool hypertries friends to each other
		template<pos_type, typename, template<typename, typename> typename,
				template<typename> typename, typename>
		friend
		class RawBoolHypertrie;

	protected:
		template<pos_type depth_t>
		using boolhypertrie_c = RawBoolHypertrie<depth_t, key_part_type_t, map_type_t, set_type_t, void>;


	public:
		typedef RawKey<1, key_part_type_t> Key;
		typedef RawSliceKey<1, key_part_type_t> SliceKey;
		typedef bool child_type;
	protected:
		typedef set_type_t<key_part_type_t> children_type;
		typedef children_type edges_type;
	public:
		typedef key_part_type_t key_part_type;

		template<typename key, typename value>
		using map_type = map_type_t<key, value>;
		template<typename key>
		using set_type = set_type_t<key>;
		constexpr static const pos_type depth = 1;

		static auto &const_emtpy_instance() {
			static thread_local std::shared_ptr<boolhypertrie_c<depth>> inst{};
			return inst;
		}

	protected:
		edges_type edges{};
	public:
		RawBoolHypertrie() = default;

		RawBoolHypertrie(RawBoolHypertrie &) = delete;

		RawBoolHypertrie(RawBoolHypertrie &&) noexcept = default;

		[[nodiscard]]
		bool operator[](key_part_type_t key_part) const {
			return edges.count(key_part);
		}

		[[nodiscard]]
		bool operator[](Key key) const {
			return edges.count(key[0]);
		}

	protected:
		template<pos_type slice_count, typename  = typename std::enable_if_t<(slice_count == 1)>>
		[[nodiscard]]
		bool
		diagonal_rek([[maybe_unused]]const std::vector<pos_type> &positions, [[maybe_unused]]std::vector<bool> &done,
		             const key_part_type_t &key_part) const {
			return edges.count(key_part);
		}

	public:

		template<pos_type slice_count, typename  = typename std::enable_if_t<(slice_count == 0)>>
		auto operator[](const SliceKey &key) const {
			assert(key[0].has_values());
			return this->operator[](key[0]).value();
		}

		[[nodiscard]]
		child_type get([[maybe_unused]]pos_type pos, key_part_type_t key_part) const {
			return edges.count(key_part);
		}

		void set(const SliceKey &key, bool value) {
			if (value)
				edges.insert(key[0]);
			else
				edges.erase(key[0]);
		}

		void set(key_part_type_t key, bool value) {
			if (value)
				edges.insert(key);
			else
				edges.erase(key);
		}

		[[nodiscard]]
		child_type diagonal(const key_part_type_t &key_part) const {
			return edges.count(key_part);
		}

		[[nodiscard]]
		std::vector<size_t> getCards(const std::vector<pos_type> &positions) const {
			assert((positions.size() == 1) ? positions[0] == 0 : positions.empty());
			std::vector<size_t> cards(positions.size());
			for (auto i : range(positions.size()))
				cards[i] = edges.size();
			return cards;
		}

		[[nodiscard]]
		size_t size() const {
			return edges.size();
		}

		[[nodiscard]]
		bool empty() const {
			return edges.empty();
		}

		template<pos_type, pos_type, typename, template<typename, typename> typename,
				template<typename> typename, typename>
		friend
		class RawHashDiagonal;

		template<pos_type, pos_type, typename, template<typename, typename> typename,
				template<typename> typename, typename>
		friend
		class RawOrderedDiagonal;

		class iterator {
		public:
			using self_type =  iterator;
			using value_type = std::vector<key_part_type>;
		protected:
			typename edges_type::const_iterator iter;
			typename edges_type::const_iterator end;
			mutable value_type value_;
		public:

			iterator(boolhypertrie_c<depth> const *const raw_boolhypertrie) : iter(raw_boolhypertrie->edges.cbegin()),
			                                                                  end(raw_boolhypertrie->edges.cend()),
			                                                                  value_(1) {
				if (iter != end)
					value_[0] = *iter;
			}

			iterator(boolhypertrie_c<depth> const &raw_boolhypertrie) : iterator(&raw_boolhypertrie) {}

			inline self_type &operator++() {
				++iter;
				return *this;
			}

			static void inc(void *it_ptr) {
				auto &it = *static_cast<iterator *>(it_ptr);
				++it;
			}

			[[nodiscard]]
			inline const value_type &operator*() const {
				value_[0] = *iter;
				return value_;
			}

			[[nodiscard]]
			static const value_type &value(void const *it_ptr) {
				auto &it = *static_cast<iterator const *>(it_ptr);
				return *it;
			}

			[[nodiscard]]
			inline operator bool() const { return iter != end; }

			[[nodiscard]]
			static bool ended(void const *it_ptr) {
				auto &it = *static_cast<iterator const *>(it_ptr);
				return not it;
			}

		};

		using const_iterator = iterator;

		[[nodiscard]]
		iterator begin() const noexcept { return {this}; }

		[[nodiscard]]
		const_iterator cbegin() const noexcept { return {this}; }


		[[nodiscard]]
		bool end() const noexcept { return false; }

		[[nodiscard]]
		bool cend() const noexcept { return false; }
	};


	template<pos_type depth_t, typename key_part_type_t, template<typename, typename> typename map_type_t,
			template<typename> typename set_type_t> //
	class RawBoolHypertrie<depth_t, key_part_type_t, map_type_t, set_type_t, typename std::enable_if_t<(depth_t >
	                                                                                                    1)>> {
	protected:
		// make the bool hypertries friends to each other
		template<pos_type, typename, template<typename, typename> typename,
				template<typename> typename, typename>
		friend
		class RawBoolHypertrie;


		template<pos_type depth_>
		using boolhypertrie_c = RawBoolHypertrie<depth_, key_part_type_t, map_type_t, set_type_t>;

		typedef util::PosCalc PosCalc;
	public:
		typedef RawKey<depth_t, key_part_type_t> Key;
		typedef RawSliceKey<depth_t, key_part_type_t> SliceKey;
	protected:
		typedef PosCalc::subkey_mask_t subkey_mask_t;
	public:
		typedef RawBoolHypertrie<depth_t - 1, key_part_type_t, map_type_t, set_type_t> child_type;
	protected:
		typedef map_type_t<key_part_type_t, std::shared_ptr<child_type>> children_type;

		typedef std::array<children_type, depth_t> edges_type;
	public:
		typedef key_part_type_t key_part_type;
		template<typename key, typename value>
		using map_type = map_type_t<key, value>;
		template<typename key>
		using set_type = set_type_t<key>;
		constexpr static const pos_type depth = depth_t;
		static constexpr bool is_tsl_map = std::is_same_v<map_type<int, int>, container::tsl_sparse_map<int, int>>;
		static constexpr bool is_tsl_set = std::is_same_v<set_type<int>, container::tsl_sparse_set<int>>;

		static auto &const_emtpy_instance() {
			static thread_local std::shared_ptr<boolhypertrie_c<depth>> inst{};
			return inst;
		}

	protected:
		mutable edges_type edges;
		size_t _size = 0;
	public:
		RawBoolHypertrie() = default;

		RawBoolHypertrie(RawBoolHypertrie &) = delete;

		RawBoolHypertrie(RawBoolHypertrie &&) noexcept = default;

		[[nodiscard]]
		inline bool operator[](const Key &key) const {
			auto pos = minCardPos();
			auto child = get(pos, key[pos]);
			if (child) {
				static typename child_type::Key next_key;
				for (auto i = 0, j = 0; i < depth_t;)
					if (i == pos) ++i;
					else next_key[j++] = key[i++];
				return (*child)[next_key];
			} else {
				return false;
			}
		}

	protected:
		[[nodiscard]]
		static auto extractPossAndKeyParts(const SliceKey &key) {
			std::vector<pos_type> positions;
			std::vector<key_part_type> key_parts;
			positions.reserve(key.size());
			for (const auto[position, key_part] : enumerate(key))
				if (key_part.has_value()) {
					positions.push_back(position);
					key_parts.push_back(key_part.value());
				}
			return std::pair{positions, key_parts};
		}

	public:
		template<pos_type diag_depth, typename  = typename std::enable_if_t<((diag_depth > 0) and
		                                                                     (diag_depth <= depth))>>
		[[nodiscard]]
		auto diagonal(const std::vector<pos_type> &positions, const key_part_type_t &key_part) const {
			assert(positions.size() == diag_depth);
			std::vector<bool> done(positions.size(), false);
			return this->template diagonal_rek<diag_depth>(positions, done, key_part);
		}

		[[nodiscard]]
		bool diagonal(const key_part_type_t &key_part) const {
			pos_type min_pos = minCardPos();
			auto found = edges[min_pos].find(key_part);
			if (found != edges[min_pos].end())
				return found->second->diagonal(key_part);
			else
				return false;
		}


	protected:
		template<pos_type diag_depth, typename  = typename std::enable_if_t<((diag_depth > 0) and
		                                                                     (diag_depth <= depth))>>
		[[nodiscard]]
		auto diagonal_rek(const std::vector<pos_type> &positions, std::vector<bool> &done,
		                  const key_part_type_t &key_part) const
		-> std::conditional_t<(depth != diag_depth), std::shared_ptr<boolhypertrie_c<
				depth - diag_depth>> *, bool> {
			std::size_t min_i = 0;
			auto min_size = std::numeric_limits<std::size_t>::max();
			std::size_t delta = 0;
			std::size_t delta_i = 0;
			for (auto i : range(positions.size()))
				if (not done[i]) {
					if (auto current_size = edges[positions[i] - delta].size(); current_size < min_size) {
						min_i = i;
						min_size = current_size;
						delta_i = delta;
					}
				} else {
					delta += 1;
					continue;
				}

			auto min_pos = positions[min_i] - delta_i;
			done[min_i] = true;
			auto found = edges[min_pos].find(key_part);
			if (found != edges[min_pos].end()) {
				if constexpr (diag_depth == 1){
					if constexpr (is_tsl_map)
					return &found.value();
					else
						return &found->second;

				}
				else
					return found->second->template diagonal_rek<diag_depth - 1>(positions, done, key_part);
			}
			if constexpr (depth != diag_depth) return &boolhypertrie_c<depth - diag_depth>::const_emtpy_instance();
			else return false;
		}


	public:
		template<pos_type slice_count, typename  = typename std::enable_if_t<((slice_count >= 0) and
		                                                                      (slice_count < depth))>>
		[[nodiscard]]
		auto operator[](const SliceKey &key) const {
			if constexpr (slice_count > 0) {
				auto[positions, key_parts] = extractPossAndKeyParts(key);
				const PosCalc *posCalc = PosCalc::getInstance(depth);
				return resolve<slice_count>(std::move(positions), std::move(key_parts), posCalc);
			} else {
				Key full_key;
				for (auto[slice, full] : zip(key, full_key))
					full = *slice;
				return this->operator[](full_key);
			}
		}


	protected:
		template<pos_type slice_count>
		[[nodiscard]] auto
		resolve(std::vector<pos_type> &&positions, std::vector<key_part_type> &&key_parts, const PosCalc *posCalc) const
		-> std::shared_ptr<boolhypertrie_c<slice_count>> {
			auto pos_it = minCardPos(positions, posCalc);
			auto key_part_it = key_parts.begin() + std::distance(positions.begin(), pos_it);
			auto child = get(posCalc->key_to_subkey_pos(*pos_it), *key_part_it);

			if (child) {
				if constexpr (depth - 1 == slice_count) {
					return child;
				} else {
					auto nextPosCalc = posCalc->use(*pos_it);
					positions.erase(pos_it);
					key_parts.erase(key_part_it);
					return child->template resolve<slice_count>(std::move(positions), std::move(key_parts),
					                                            nextPosCalc);
				}
			} else {
				return {};
			}
		}

	public:
		[[nodiscard]]
		std::shared_ptr<child_type> get(pos_type position, key_part_type_t key_part) const {
			auto found = edges[position].find(key_part);
			if (found != edges[position].end()) {
				return found->second;
			} else {
				return {};
			}
		}

	protected:
		/**
		 * This must only be used interally for setting key_parts
		 * @param position
		 * @param key_part
		 * @return
		 */
		[[nodiscard]]
		std::shared_ptr<child_type> get_unsafe(pos_type position, key_part_type_t key_part) {
			auto found = edges[position].find(key_part);
			if (found != edges[position].end()) {
				return found->second;
			} else {
				return {};
			}
		}

	public:


		[[nodiscard]]
		std::vector<size_t> getCards(const std::vector<pos_type> &positions) const {
			std::vector<size_t> cards(positions.size());
			for (auto i : range(positions.size()))
				cards[i] = edges[i].size();
			return cards;
		}

	protected:
		[[nodiscard]]
		pos_type minCardPos() const {
			if constexpr (depth == 2) {
				if (edges[0].size() <= edges[1].size()) {
					return 0;
				} else {
					return 1;
				}
			} else {
				auto min_pos = 0;
				auto min_card = std::numeric_limits<typename children_type::size_type>::max();
				for (const auto[i, children] : enumerate(edges)) {
					if (children.size() < min_card) {
						min_card = children.size();
						min_pos = i;
					}
				}
				return min_pos;
			}
		}

		[[nodiscard]]
		std::vector<pos_type>::iterator minCardPos(std::vector<pos_type> &positions, const PosCalc *posCalc) const {
			auto pos_it = positions.begin();
			auto min_pos = pos_it;
			auto min_card = std::numeric_limits<typename children_type::size_type>::max();
			for (; pos_it != positions.end(); ++pos_it) {
				const auto &children = edges[posCalc->key_to_subkey_pos(*pos_it)];
				if (children.size() < min_card) {
					min_card = children.size();
					min_pos = pos_it;
				}
			}
			return min_pos;
		}

		[[nodiscard]]
		std::vector<pos_type>::iterator minCardPos(std::vector<pos_type> &positions) const {
			auto pos_it = positions.begin();
			auto min_pos = pos_it;
			auto min_card = std::numeric_limits<typename children_type::size_type>::max();
			for (; pos_it != positions.end(); ++pos_it) {
				const auto &children = edges[*pos_it];
				if (children.size() < min_card) {
					min_card = children.size();
					min_pos = pos_it;
				}
			}
			return min_pos;
		}

	public:

		void set(const Key &key, bool value) {
			// check if the entry for this value is true.
			if (operator[](key)) {
				if (not value) {
					static tsl::hopscotch_set<subkey_mask_t> finished_subtries(factorial(depth_t));
					static tsl::hopscotch_set<subkey_mask_t> deleted_subtries(factorial(depth_t));

					subkey_mask_t subkey_mask(depth_t);
					PosCalc const *pos_calc = PosCalc::getInstance(subkey_mask);
					_<depth_t>::delRek(*this, key, finished_subtries, deleted_subtries, pos_calc);
					finished_subtries.clear();
					deleted_subtries.clear();
				}
			} else {
				if (value) {
					// cache for already created sub HyperTries.
					static tsl::hopscotch_map<subkey_mask_t, std::shared_ptr<void>> finished_subtries(
							factorial(depth_t));

					// get pos_calc for this.
					subkey_mask_t subkey_mask(depth_t);
					PosCalc const *pos_calc = PosCalc::getInstance(subkey_mask);

					// store subkey recursively
					_<depth_t>::setRek(*this, key, finished_subtries, pos_calc);
					finished_subtries.clear();
				}
			}
		}

	protected:

		template<pos_type current_depth,
				typename  = typename std::enable_if_t<(depth_t >= 1 and current_depth <= depth_t)>,
				typename DUMMY = void>
		struct _;

		template<pos_type current_depth, typename DUMMY>
		struct _<current_depth, typename std::enable_if_t<(current_depth > 1 and depth_t == current_depth)>, DUMMY> {

			using child_ = typename boolhypertrie_c<depth_t>::template _<current_depth - 1>;

			inline static void
			setRek(boolhypertrie_c<current_depth> &current, const Key &key,
			       tsl::hopscotch_map<subkey_mask_t, std::shared_ptr<void>> &finished_nodes,
			       PosCalc const *pos_calc) {

				// update this node
				current._size += 1;
				// a child must be set or updated for every subkey_pos available.
				for (const auto key_pos : pos_calc->getKeyPoss()) {
					key_part_type_t key_part = key[key_pos];

					// get pos_calc for next child and check if it was already updated earlier.
					PosCalc const *next_pos_calc = pos_calc->use(key_pos);
					const auto &finished_child = finished_nodes.find(next_pos_calc->getSubKeyMask());


					auto subkey_pos = pos_calc->key_to_subkey_pos(key_pos);

					// get the child at the current position.
					if (auto child_ptr = current.get_unsafe(subkey_pos, key_part); child_ptr) {
						// the child exists ...
						// ... and the subtrie starting with the child was not already finished:
						if (finished_child == finished_nodes.end()) {
							// call this function for the child
							child_::setRek(child_ptr, key, finished_nodes, next_pos_calc);
						}
					} else {
						// the child does not exist ...
						// ... and the subtrie starting with the child was already finished:
						if (finished_child != finished_nodes.end()) {
							// set the child at this node
							auto child = std::static_pointer_cast<boolhypertrie_c<current_depth - 1>>
									(
											finished_child->second);
							current.edges[subkey_pos].insert(std::make_pair(key_part, child));

						} else { // ... and the subtrie starting with the child was not already finished:
							// set a new child and call this function for the child
							auto child = std::make_shared<boolhypertrie_c<current_depth - 1>>
									();
							current.edges[subkey_pos].insert(std::make_pair(key_part, child));
							child_::setRek(child, key, finished_nodes, next_pos_calc);
						}
					}
				}
			}

			inline static void
			delRek(boolhypertrie_c<current_depth> &current, const Key &key,
			       tsl::hopscotch_set<subkey_mask_t> &finished_nodes,
			       tsl::hopscotch_set<subkey_mask_t> &deleted_nodes, PosCalc const *pos_calc) {

				// update this node
				current._size -= 1;

				// add it to the finished ( means updated ) nodes.
				finished_nodes.insert(pos_calc->getSubKeyMask());


				// a child must be set or updated for every subkey_pos available.
				for (const auto key_pos : pos_calc->getKeyPoss()) {
					key_part_type_t key_part = key[key_pos];

					// get pos_calc for next child and check if it was already updated earlier.
					auto subkey_pos = pos_calc->key_to_subkey_pos(key_pos);
					auto found = current.edges[subkey_pos].find(key_part);

					PosCalc const *next_pos_calc = pos_calc->use(key_pos);

					if (not bool(finished_nodes.count(next_pos_calc->getSubKeyMask()))) {
						// call this function for the child
						auto child = (*found).second;

						child_::delRek(child, key, finished_nodes, deleted_nodes, next_pos_calc);
					}
					if (deleted_nodes.count(next_pos_calc->getSubKeyMask())) {
						current.edges[subkey_pos].erase(found);
					}
				}
			}
		};


		template<pos_type current_depth, typename DUMMY>
		struct _<current_depth, typename std::enable_if_t<(current_depth < depth_t and current_depth > 1)>, DUMMY> {

			using child_ = typename boolhypertrie_c<depth_t>::template _<current_depth - 1>;

			inline static void
			setRek(std::shared_ptr<boolhypertrie_c<current_depth>> current,
			       const Key &key,
			       tsl::hopscotch_map<subkey_mask_t, std::shared_ptr<void>>
			       &finished_nodes,
			       PosCalc const *pos_calc
			) {

				// update this node
				current->_size += 1;

				// add it to the finished ( means updated ) nodes.
				finished_nodes[pos_calc->getSubKeyMask()] = current;


				// a child must be set or updated for every subkey_pos available.
				for (const auto key_pos : pos_calc->getKeyPoss()) {
					key_part_type_t key_part = key[key_pos];

					// get pos_calc for next child and check if it was already updated earlier.
					PosCalc const *next_pos_calc = pos_calc->use(key_pos);
					const auto &finished_child = finished_nodes.find(next_pos_calc->getSubKeyMask());


					auto subkey_pos = pos_calc->key_to_subkey_pos(key_pos);

					// get the child at the current position.
					if (auto child_ptr = current->get_unsafe(subkey_pos, key_part); child_ptr) {
						// the child exists ...
						// ... and the subtrie starting with the child was not already finished:
						if (finished_child == finished_nodes.end()) {
							// call this function for the child
							_<current_depth - 1>::setRek(child_ptr, key, finished_nodes, next_pos_calc);
						}
					} else {
						// the child does not exist ...
						// ... and the subtrie starting with the child was already finished:
						if (finished_child != finished_nodes.end()) {
							// set the child at this node
							auto child = std::static_pointer_cast<boolhypertrie_c<current_depth - 1>>
									(
											finished_child->second);
							current->edges[subkey_pos].insert(std::make_pair(key_part, child));

						} else { // ... and the subtrie starting with the child was not already finished:
							// set a new child and call this function for the child
							auto child = std::make_shared<boolhypertrie_c<current_depth - 1>>
									();
							current->edges[subkey_pos].insert(std::make_pair(key_part, child));
							_<current_depth - 1>::setRek(child, key, finished_nodes, next_pos_calc);
						}
					}
				}
			}

			inline static void
			delRek(std::shared_ptr<boolhypertrie_c<current_depth>> current,
			       const Key &key,
			       tsl::hopscotch_set<subkey_mask_t> &finished_nodes,
			       tsl::hopscotch_set<subkey_mask_t> &deleted_nodes, PosCalc
			       const *pos_calc) {

				// update this node
				current->_size -= 1;

				// add it to the finished ( means updated ) nodes.
				finished_nodes.insert(pos_calc->getSubKeyMask());


				// a child must be set or updated for every subkey_pos available.
				for (const auto key_pos : pos_calc->getKeyPoss()) {
					key_part_type_t key_part = key[key_pos];

					// get pos_calc for next child and check if it was already updated earlier.
					auto subkey_pos = pos_calc->key_to_subkey_pos(key_pos);
					auto found = current->edges[subkey_pos].find(key_part);

					PosCalc const *next_pos_calc = pos_calc->use(key_pos);

					if (not bool(finished_nodes.count(next_pos_calc->getSubKeyMask()))) {
						// call this function for the child
						auto child = (*found).second;
						child_::delRek(child, key, finished_nodes, deleted_nodes, next_pos_calc);
					}
					if (deleted_nodes.count(next_pos_calc->getSubKeyMask())) {
						current->edges[subkey_pos].erase(found);
					}
				}

				if (current->empty()) {
					deleted_nodes.insert(pos_calc->getSubKeyMask());
				}
			}
		};


		template<typename DUMMY>
		struct _<1, typename std::enable_if_t<(true)>, DUMMY> {
			template<pos_type depth_>
			using boolhypertrie_c = RawBoolHypertrie<depth_, key_part_type_t, map_type_t, set_type_t, void>;

			inline static void setRek(
					std::shared_ptr<boolhypertrie_c<1>> current, const Key &key,
					tsl::hopscotch_map<subkey_mask_t, std::shared_ptr<void>> &finished_nodes,
					PosCalc const *pos_calc) {
				// add it to the finished ( means updated ) nodes.
				finished_nodes[pos_calc->getSubKeyMask()] = current;
				// set the entry in the set
				key_part_type_t key_part = key[pos_calc->subkey_to_key_pos(0)];

				current->set(key_part, true);
			}

			inline static void
			delRek(std::shared_ptr<boolhypertrie_c<1>> current, const Key &key,
			       tsl::hopscotch_set<subkey_mask_t> &finished_nodes,
			       tsl::hopscotch_set<subkey_mask_t> &deleted_nodes, PosCalc const *pos_calc) {

				// update this node

				finished_nodes.insert(pos_calc->getSubKeyMask());

				key_part_type_t key_part = key[pos_calc->subkey_to_key_pos(0)];

				current->set(key_part, false);

				if (current->empty()) {
					deleted_nodes.insert(pos_calc->getSubKeyMask());
				}
			}
		};

	public:

		[[nodiscard]]
		size_t size() const {
			return _size;
		}

		[[nodiscard]]
		bool empty() const {
			return _size == 0;
		}

		template<pos_type, pos_type, typename, template<typename, typename> typename,
				template<typename> typename, typename>
		friend
		class RawHashDiagonal;

		template<pos_type, pos_type, typename, template<typename, typename> typename,
				template<typename> typename, typename>
		friend
		class RawOrderedDiagonal;

		class iterator {
			template<pos_type depth_>
			using childen_t  = typename boolhypertrie_c<depth_>::children_type::const_iterator;

			util::CountDownNTuple<childen_t, depth_t> iters;
			util::CountDownNTuple<childen_t, depth_t> ends;
		public:
			using self_type =  iterator;
			using value_type = std::vector<key_part_type>;
		protected:
			boolhypertrie_c<depth> const *const raw_boolhypertrie;

			std::vector<key_part_type> key;
			bool ended_;
		public:

			iterator(boolhypertrie_c<depth> const *const raw_boolhypertrie)
					: raw_boolhypertrie(raw_boolhypertrie),
					  key(depth),
					  ended_{raw_boolhypertrie->empty()} {
				if (not ended_)
					init_rek();
			}

			iterator(boolhypertrie_c<depth> const &raw_boolhypertrie) : iterator(&raw_boolhypertrie) {}

			inline self_type &operator++() {
				inc_rek();
				return *this;
			}

			static void inc(void *it_ptr) {
				auto &it = *static_cast<iterator *>(it_ptr);
				++it;
			}

			inline const value_type &operator*() const { return key; }

			static const value_type &value(void const *it_ptr) {
				auto &it = *static_cast<iterator const *>(it_ptr);
				return *it;
			}

			inline operator bool() const { return not ended_; }

			static bool ended(void const *it_ptr) {
				auto &it = *static_cast<iterator const *>(it_ptr);
				return it.ended_;
			}

		protected:

			inline void init_rek() {
				// get the iterator
				auto &iter = std::get<depth - 1>(iters);
				iter = raw_boolhypertrie->edges[0].cbegin();
				auto &end = std::get<depth - 1>(ends);
				end = raw_boolhypertrie->edges[0].cend();

				// set the key_part in the key
				key[0] = std::get<depth - 1>(iters)->first;
				init_rek < depth - 1 > ();
			}

			template<pos_type current_depth,
					typename =std::enable_if_t<(current_depth < depth and current_depth >= 1)> >
			inline void init_rek() {
				// get parent iterator
				auto &iter = std::get<current_depth - 1>(iters);
				auto &end = std::get<current_depth - 1>(ends);
				childen_t<current_depth + 1> &parent_it = std::get<current_depth>(iters);

				// resolve the current depth raw_boolhypertrie
				std::shared_ptr<boolhypertrie_c<current_depth>> current_boolhypertrie = parent_it->second;
				// set the current depth raw_boolhypertrie iterator

				if constexpr (current_depth > 1) {
					iter = current_boolhypertrie->edges[0].cbegin();
					end = current_boolhypertrie->edges[0].cend();
					key[depth - current_depth] = iter->first;
					// set the key_part in the key
					init_rek<current_depth - 1>();
				} else {
					iter = current_boolhypertrie->edges.cbegin();
					end = current_boolhypertrie->edges.cend();
					key[depth - 1] = *iter;
				}

			}

			inline void inc_rek() {
				bool inc_done = inc_rek < depth - 1 > ();
				if (not inc_done) {
					auto &iter = std::get<depth - 1>(iters);
					auto &end = std::get<depth - 1>(ends);
					++iter;
					if (iter != end) {
						key[0] = iter->first;
						init_rek<depth - 1>();
					} else {
						ended_ = true;
					}
				}
			}

			template<pos_type current_depth,
					typename =std::enable_if_t<(current_depth < depth and current_depth >= 1)> >
			inline bool inc_rek() {
				if constexpr (current_depth > 1) {
					bool inc_done = inc_rek<current_depth - 1>();
					if (inc_done) {
						return true;
					} else {
						auto &iter = std::get<current_depth - 1>(iters);
						auto &end = std::get<current_depth - 1>(ends);
						++iter;
						if (iter != end) {
							init_rek<current_depth - 1>();
							key[depth - current_depth] = iter->first;
							return true;
						} else {
							return false;
						}
					}
				} else {
					// get the iterator
					auto &iter = std::get<0>(iters);
					auto &end = std::get<0>(ends);
					// increment it
					++iter;
					// check if it is still valid
					if (iter != end) {
						key[depth - 1] = *iter;
						return true;
					} else {
						return false;
					}
				}
			}
		};

		using const_iterator = iterator;

		iterator begin() const noexcept { return {this}; }

		const_iterator cbegin() const noexcept { return {this}; }


		bool end() const noexcept { return false; }

		bool cend() const noexcept { return false; }
	};
}

#endif //HYPERTRIE_BOOLHYPERTRIE_IMPL_H

