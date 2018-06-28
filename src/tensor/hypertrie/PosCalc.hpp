#ifndef SPARSETENSOR_HYPERTRIE_POSCALC_HPP
#define SPARSETENSOR_HYPERTRIE_POSCALC_HPP


#include <cstdint>
#include <map>
#include <vector>

#include "../../util/All.hpp"


namespace tnt::hypertrie {


    /**
     * Provides translation between position in superkeys with subkeys. (1,2,4) for example is a subkey of (1,2,3,4).
     * And the other way around.
     */
    class PosCalc {
        using key_pos_t = tnt::util::types::key_pos_t;
        using subkey_mask_t = tnt::util::types::subkey_mask_t;
        /**
         * Holds all instances.
         */
        inline static std::map<std::vector<bool>, PosCalc *> instances{};
    public:
        /**
         * Length of superkeys.
         */
        const key_pos_t key_length;

        /**
         * Length of subkeys.
         */
        const key_pos_t subkey_length;
    private:
        /**
         * Stores for superkey positions to which subkey positions they map.
         */
        std::vector<key_pos_t> key_to_subkey;

        /**
         * Stores for subkey positions to which superkey positions they map. So this is also a list of all currently relevant superkey pos.
         */
        std::vector<key_pos_t> subkey_to_key;

        /**
         * removed_positions bit vector of positions removed from superkey to subkey.
         */
        subkey_mask_t removed_positions;

        /**
         * Cache for PosCalcs of subkeys that are 1 shorter.
         */
        std::vector<PosCalc *> next_pos_calcs;

        /**
             * Private construcor.
             * @param key_length  length of superkeys.
             * @param subkey_length length of subkeys.
             */
        PosCalc(key_pos_t key_length, key_pos_t subkey_length) : key_length(key_length),
                                                                 subkey_length(subkey_length),
                                                                 key_to_subkey(std::vector<key_pos_t>(key_length)),
                                                                 subkey_to_key(std::vector<key_pos_t>(subkey_length)),
                                                                 removed_positions(subkey_mask_t(key_length)),
                                                                 next_pos_calcs(std::vector<PosCalc *>(key_length)) {}

    public:
        /**
         * Convert a superkey position to a subkey key position.
         * @param key_pos superkey position
         * @return subkey position
         */
        inline key_pos_t key_to_subkey_pos(key_pos_t key_pos) const {
            return key_to_subkey[key_pos];
        }

        /**
         * Convert a subkey position to a superkey position.
         * @param subkey_pos subkey position
         * @return superkey position
         */
        inline key_pos_t subkey_to_key_pos(key_pos_t subkey_pos) const {
            return subkey_to_key[subkey_pos];
        }

        /**
         * Get all keypositions that this subkey contains.
         * @return vector of key positions.
         */
        inline const std::vector<key_pos_t> &getKeyPoss() const {
            return subkey_to_key;
        }

        /**
         * bit vector of positions removed from superkey to subkey.
         * @return bit vector
         */
        inline const subkey_mask_t &getSubKeyMask() const {
            return removed_positions;
        }

        /**
         * PosCalcs of subkeys that has one position less.
         * @param key_pos position of superkey to be removed.
         * @return PosCalc like this but without position key_pos.
         */
        inline PosCalc *use(const key_pos_t &key_pos) {
            PosCalc *child = next_pos_calcs.at(key_pos);
            if (child == nullptr) {
                std::vector<bool> used_pos_mask(this->key_length, true);
                for (uint8_t subkey_pos : this->subkey_to_key)
                    used_pos_mask[subkey_pos] = false;
                used_pos_mask[key_pos] = true;

                child = getInstance(used_pos_mask);
                next_pos_calcs[key_pos] = child;
            }
            return child;
        }

        /**
         * Get an instance for a vector of used positions.
         * @param removed_positions bit vector of positions removed from superkey to subkey.
         * @return an instance
         */
        static PosCalc *getInstance(const std::vector<bool> &removed_positions) {
            auto instance_ = instances.find(removed_positions);
            if (instance_ != instances.end()) {
                // if an instance already exists return it
                return instance_->second;
            } else {
                // else create it and store it for reuse
                uint8_t key_length = uint8_t(removed_positions.size());

                uint8_t subkey_length = 0;
                for (bool removed_pos : removed_positions) {
                    subkey_length += not removed_pos;
                }

                PosCalc *instance = new PosCalc(key_length, subkey_length);

                uint8_t offset = 0;

                for (uint8_t key_pos = 0; key_pos < key_length; key_pos++) {
                    instance->removed_positions[key_pos] = removed_positions[key_pos];

                    if (removed_positions[key_pos]) {
                        offset++;
                    } else {
                        const uint8_t subkey_pos = key_pos - offset;

                        instance->key_to_subkey[key_pos] = subkey_pos;
                        instance->subkey_to_key[subkey_pos] = key_pos;

                    }
                }

                instances.insert_or_assign(removed_positions, instance);
                return instance;
            }
        }

        /**
         * Get an instance for a empty vector of used positions of given length.
         * @param removed_positions length of the empty vector.
         * @return an instance
         */
        static PosCalc *getInstance(const size_t &length) {
            subkey_mask_t keymask(length);
            return getInstance(keymask);
        }

    };
}


#endif //SPARSETENSOR_HYPERTRIE_POSCALC_HPP
