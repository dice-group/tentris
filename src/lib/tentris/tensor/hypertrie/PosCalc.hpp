#ifndef SPARSETENSOR_HYPERTRIE_POSCALC_HPP
#define SPARSETENSOR_HYPERTRIE_POSCALC_HPP



#include <cstdint>
#include <boost/container/flat_map.hpp>
#include <vector>
#include <memory>
#include <itertools.hpp>
#include "tentris/util/All.hpp"
namespace {
    using namespace tentris::util::types;
}

namespace tentris::tensor::hypertrie {
	/**
		* Provides translation between position in superkeys with subkeys. (1,2,4) for example is a subkey of (1,2,3,4).
		* And the other way around.
		*/
	// TODO: make threadsafe
	class PosCalc {
		template<typename key, typename value>
		using flat_map = ::boost::container::flat_map<key, value>;
	public:
		using key_pos_t = uint8_t;
		using subkey_mask_t = ::std::vector<bool>;
	private:
		/**
		 * Stores for superkey positions to which subkey positions they map.
		 */
		const std::vector<key_pos_t> key_to_subkey;

		/**
		 * Stores for subkey positions to which superkey positions they map. So this is also a list of all currently relevant superkey pos.
		 */
		const std::vector<key_pos_t> subkey_to_key;

		/**
		 * removed_positions bit vector of positions removed from superkey to subkey.
		 */
		const subkey_mask_t removed_positions;

		/**
		 * Cache for PosCalcs of subkeys that are 1 shorter.
		 */
		mutable std::vector<PosCalc const *> next_pos_calcs;

		PosCalc(std::vector<key_pos_t> &&keyToSubkey,
				std::vector<key_pos_t> &&subkeyToKey,
				const subkey_mask_t &removedPositions) :
				key_to_subkey{keyToSubkey},
				subkey_to_key{subkeyToKey},
				removed_positions{removedPositions},
				next_pos_calcs(this->keyLength()) {}

	public:

		key_pos_t keyLength() const {
			return key_to_subkey.size();
		}

		key_pos_t subkeyLength() const {
			return subkey_to_key.size();
		}

		/**
		 * Convert a superkey position to a subkey key position.
		 * @param key_pos superkey position
		 * @return subkey position
		 */
		key_pos_t key_to_subkey_pos(key_pos_t key_pos) const {
			return key_to_subkey[key_pos];
		}

		/**
		 * Convert a subkey position to a superkey position.
		 * @param subkey_pos subkey position
		 * @return superkey position
		 */
		key_pos_t subkey_to_key_pos(key_pos_t subkey_pos) const {
			return subkey_to_key[subkey_pos];
		}

		/**
		 * Get all keypositions that this subkey contains.
		 * @return vector of key positions.
		 */
		const std::vector<key_pos_t> &getKeyPoss() const {
			return subkey_to_key;
		}

		/**
		 * bit vector of positions removed from superkey to subkey.
		 * @return bit vector
		 */
		const subkey_mask_t &getSubKeyMask() const {
			return removed_positions;
		}

		/**
		 * PosCalcs of subkeys that has one position less.
		 * @param key_pos position of superkey to be removed.
		 * @return PosCalc like this but without position key_pos.
		 */
		PosCalc const *use(const key_pos_t &key_pos) const {
			PosCalc const *child = next_pos_calcs.at(key_pos);
			if (child == nullptr) {
				std::vector<bool> used_pos_mask(this->keyLength(), true);
				for (auto subkey_pos : this->subkey_to_key)
					used_pos_mask[subkey_pos] = false;
				used_pos_mask[key_pos] = true;

				child = getInstance(used_pos_mask);
				next_pos_calcs[key_pos] = child;
			}
			return child;
		}

		/**
		 * Holds all instances.
		 */
		inline static flat_map<std::vector<bool>, std::unique_ptr<PosCalc const>> instances{};

		/**
		 * Get an instance for a vector of used positions.
		 * @param removed_positions bit vector of positions removed from superkey to subkey.
		 * @return an instance
		 */
		static PosCalc const *getInstance(const std::vector<bool> &removed_positions) {
			auto instance_ = instances.find(removed_positions);
			if (instance_ != instances.end()) {
				// if an instance already exists return it
				return instance_->second.get();
			} else {
				// else create it and store it for reuse
				auto key_length = key_pos_t(removed_positions.size());

				key_pos_t subkey_length = std::count(removed_positions.begin(), removed_positions.end(), false);

				subkey_mask_t next_removed_positions(key_length);
				std::vector<key_pos_t> next_key_to_subkey(key_length);
				std::vector<key_pos_t> next_subkey_to_key(subkey_length);

				key_pos_t offset = 0;

				for (auto key_pos : iter::range(key_length)) {

					if (removed_positions[key_pos]) {
						offset++;
					} else {
						const key_pos_t subkey_pos = key_pos - offset;

						next_key_to_subkey[key_pos] = subkey_pos;
						next_subkey_to_key[subkey_pos] = key_pos;
					}
				}

				instances[removed_positions] = std::unique_ptr<PosCalc const>{new PosCalc{
						std::move(next_key_to_subkey), std::move(next_subkey_to_key), removed_positions}};
				return instances[removed_positions].get();
			}
		}

		/**
		 * Get an instance for a empty vector of used positions of given length.
		 * @param removed_positions length of the empty vector.
		 * @return an instance
		 */
		static PosCalc const *getInstance(const size_t &length) {
			subkey_mask_t key_mask(length);
			return getInstance(key_mask);
		}

	};


}


#endif //SPARSETENSOR_HYPERTRIE_POSCALC_HPP
