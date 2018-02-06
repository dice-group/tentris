#ifndef LIBSPARSETENSOR_POSCALC_HPP
#define LIBSPARSETENSOR_POSCALC_HPP


#include "Types.hpp"
#include <cstdint>
#include <map>
#include <vector>
#include "../tensor/Types.hpp"

using std::vector;
using std::map;
using sparsetensor::tensor::key_pos_t;

namespace sparsetensor::hypertrie {


    /**
     * Provides translation between position in superkeys with subkeys. (1,2,4) for example is a subkey of (1,2,3,4).
     * And the other way around.
     */
    class PosCalc {
    private:
        /**
         * Holds all instances.
         */
        inline static map<vector<bool>, PosCalc *> instances{};

        /**
         * Private construcor.
         * @param key_length  length of superkeys.
         * @param subkey_length length of subkeys.
         */
        explicit PosCalc(const key_pos_t key_length, uint8_t subkey_length) : key_length(key_length),
                                                                              subkey_length(subkey_length) {}

    public:
        /**
         * Length of superkeys.
         */
        key_pos_t key_length;

        /**
         * Length of subkeys.
         */
        key_pos_t subkey_length;

    private:
        /**
         * Stores for superkey positions to which subkey positions they map.
         */
        vector<key_pos_t> key_to_subkey = vector<key_pos_t>(key_length);

    public:

        /**
         * Stores for subkey positions to which superkey positions they map. So this is also a list of all currently relevant superkey pos.
         */
        vector<key_pos_t> subkey_to_key = vector<key_pos_t>(subkey_length);

        /**
         * removed_positions bit vector of positions removed from superkey to subkey.
         */
        subkey_mask_t removed_positions = subkey_mask_t(key_length);

    private:
        /**
         * Cache for PosCalcs of subkeys that are 1 shorter.
         */
        vector<PosCalc *> next_pos_calcs = vector<PosCalc *>(key_length);

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
         * PosCalcs of subkeys that has one position less.
         * @param key_pos position of superkey to be removed.
         * @return PosCalc like this but without position key_pos.
         */
        inline PosCalc *use(const key_pos_t &key_pos) {
            PosCalc *child = next_pos_calcs.at(key_pos);
            if (child == nullptr) {
                vector<bool> used_pos_mask(this->key_length, true);
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
        static PosCalc *getInstance(const vector<bool> &removed_positions) {
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


#endif //LIBSPARSETENSOR_POSCALC_HPP
