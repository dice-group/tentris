#ifndef HYPERTRIE_BOOLHYPERTRIE_ORDERED_DIAGONAL_IMPL_HPP
#define HYPERTRIE_BOOLHYPERTRIE_ORDERED_DIAGONAL_IMPL_HPP

#include "hypertrie/internal/util/CONSTANTS.hpp"

#include "hypertrie/internal/BoolHypertrie_impl.hpp"
#include "hypertrie/internal/container/BoostFlatMap.hpp"
#include "hypertrie/internal/container/BoostFlatSet.hpp"

namespace hypertrie::internal {

	template<typename key_part_type>
	class BoolHypertrie<key_part_type, container::boost_flat_map, container::boost_flat_set>
			: public BoolHypertrieImpl<key_part_type, container::boost_flat_map, container::boost_flat_set> {
		typedef BoolHypertrieImpl <key_part_type, container::boost_flat_map, container::boost_flat_set> super_t;
		template<pos_type depth>
		using RawBoolHypertrie = typename super_t::template RawBoolHypertrie<depth>;

	public:
		explicit BoolHypertrie(pos_type depth)
				: BoolHypertrieImpl<key_part_type, container::boost_flat_map, container::boost_flat_set>(depth) {}

		class Diagonal {
			template<class key, class value>
			using map_type = container::boost_flat_map<key, value>;
			template<class key>
			using set_type = container::boost_flat_set<key>;



			template<pos_type depth, pos_type diag_depth>
			using RawDiagonal = typename hypertrie::internal::RawBoolHypertrie<depth, key_part_type, map_type, set_type>::template Diagonal<diag_depth>;

			struct RawDiagFunctions {
				RawDiagFunctions(
						key_part_type(

						*getKeyPart)(void const *),

						key_part_type (*setLower)(void *, key_part_type),

						bool (*containsAndIncrement)(void *, key_part_type),

						void (*inc)(void *),

						void (*init)(void *),

						bool (*empty)(void const *),
						size_t(

						*size)(void const *)) :

						getKeyPart(getKeyPart),
						setLower(setLower),
						containsAndIncrement(containsAndIncrement),
						inc(inc),
						init(init),
						empty(empty),
						size(size) {}

				key_part_type (*getKeyPart)(void const *);

				key_part_type (*setLower)(void *, key_part_type);

				bool (*containsAndIncrement)(void *, key_part_type);

				void (*inc)(void *);

				void (*init)(void *);

				bool (*empty)(void const *);

				size_t (*size)(void const *);
			};

			inline static std::vector<std::vector<RawDiagFunctions>> functions
					{
							{
									RawDiagFunctions{
											&RawDiagonal<1, 1>::getKeyPart,
											&RawDiagonal<1, 1>::setLower,
											&RawDiagonal<1, 1>::containsAndIncrement,
											&RawDiagonal<1, 1>::inc,
											&RawDiagonal<1, 1>::init,
											&RawDiagonal<1, 1>::empty,
											&RawDiagonal<1, 1>::size
									}
							}
					};

		public:
			using poss_type = std::vector<pos_type>;
		private:

			std::shared_ptr<void> raw_diag;
			void *raw_diag_ptr;
			const RawDiagFunctions &raw_diag_funcs;

			static inline std::shared_ptr<void>
			getRawDiagonal(const BoolHypertrie &boolhypertrie, const pos_type &diag_depth) {
				switch (boolhypertrie.depth()) {
					case 1: {
						const auto &raw_boolhypertrie = *(static_cast<RawBoolHypertrie<1> const *>(boolhypertrie.hypertrie.get()));
						return std::make_shared<RawDiagonal<1, 1>>(raw_boolhypertrie);
					}
					default:
						throw std::logic_error{"not implemented."};
				}
			}

		public:

			Diagonal(const BoolHypertrie &boolhypertrie, const poss_type &positions) :
					raw_diag(getRawDiagonal(boolhypertrie, positions.size())),
					raw_diag_ptr{raw_diag.get()},
					raw_diag_funcs(functions[boolhypertrie.depth() - 1][positions.size() - 1]) {}


/**
 * Potentially forwards the Diagonal and leafs it in a safe state. <br/>
 * It checks if the current key_part is valid and increments it until it is valid.
 */
			void init() const {
				raw_diag_funcs.init(raw_diag_ptr);
			}

/**
 * Must only be called in a safe state. <br/>
 * Returns the current value.
 */
			key_part_type getKeyPart() const {
				return std::invoke(raw_diag_funcs.getKeyPart(raw_diag_ptr);
			}

/**
 * Forwards the Diagonal and leafs it in a safe state. <br/>
 * getKeyPart() may be called afterwards if it didn't return the maximum value of key_part_type.
 * @param key_part a key_part that must be larger then the value currently returned by getKeyPart()
 * @return the current key_part. A subsequent call to getKeyPart() returns the same value.
 */
			key_part_type setLower(key_part_type key_part) {
				return raw_diag_funcs.setLower(raw_diag_ptr, key_part);
			}

/**
 * Forwards the Diagonal and leafs it in an unsafe state.  <br/>
 * It checks if the key_part is valid key_part. If so it returns true, otherwise false.
 * After that check the Diagonal is incremented to the next possible key_part.
 * @param key_part a key_part that must be larger or equal to the value currently returned
 * @return if key_part is valid for this Diagonal.
 */
			bool containsAndIncrement(key_part_type key_part) {
				return raw_diag_funcs.containsAndIncrement(raw_diag_ptr, key_part);
			}

/**
 * Forwards the Diagonal and leafs it in a safe state. <br/>
 * Increments the diagonal to the next valid key_part.
 */
			void operator++() {
				return raw_diag_funcs.inc(raw_diag_ptr);
			}

/**
 * If it returns true there are no key_parts left for sure.
 * Otherwise there are potential key_parts left and therefore, there may also be valid key_parts left.
 * @return
 */
			bool empty() const {
				return raw_diag_funcs.empty(raw_diag_ptr);
			}

/**
 * Always safe. <br/>
 * Get the number of potential key_parts left. This is a upper bound to the valid key_parts.
 * @return number of potential key_parts left
 */
			size_t size() const {
				return raw_diag_funcs.size(raw_diag_ptr);
			}

/**
 * Reduces the search range of key_parts to [min, max].
 * If the range is smaller than the input [min, max], min and max are being updated.
 * @param min minimum key_part
 * @param max maximum key_part
 */
			void narrowRange(key_part_type &min, key_part_type &max) {
				// TODO: implement
			}

		};
	};

}
#endif //HYPERTRIE_BOOLHYPERTRIE_ORDERED_DIAGONAL_IMPL_HPP
