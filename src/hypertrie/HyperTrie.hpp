#ifndef SPARSETENSOR_HYPERTRIE_HYPERTRIE_HPP
#define SPARSETENSOR_HYPERTRIE_HYPERTRIE_HPP


#include "PosCalc.hpp"
#include "Types.hpp"
#include "../tensor/Types.hpp"
#include <cstdint>
#include <map>
#include <vector>
#include <variant>
#include <optional>
#include <unordered_map>
#include <limits>
#include <ostream>

using std::ostream;


using std::vector;
using std::unordered_map;
using std::variant;
using std::map;
using std::optional;
using std::tuple;
using sparsetensor::tensor::key_pos_t;
using sparsetensor::tensor::key_part_t;
using sparsetensor::tensor::Key_t;


namespace sparsetensor::hypertrie {


    template<typename T>
    class Diagonal;

    template<typename T>
    class HyperTrie {
        friend class Diagonal<T>;

    public:

        explicit HyperTrie(const key_pos_t depth) : depth(depth) {}

        /**
         * Depth is the length of the keys.
         */
        key_pos_t depth{};

        /**
         * Sum of all entries.
         */
        T leafsum{};

        /**
         * Number of entries.
         */
        uint64_t leafcount{};

    private:
        /**
         * edges_by_pos: subkey_pos -> edges;<br/>
         * edges: key_part -> subtrie OR value
         */
        vector<map<key_part_t, variant<HyperTrie<T> *, T>> *> edges_by_pos =
                vector<map<key_part_t, variant<HyperTrie<T> *, T>> *>(depth);

        /**
         * Get a pointer to the edges map by subkey_pos. If the map doesn't exist so far it is created.
         * @param subkey_pos position of the next coordinate. MUST be in range [0,self->depth).
         * @return a pointer to a map that maps the used key_parts at subkey_pos of a subkey to sub-hypertries or T entries.
         */
        map<key_part_t, variant<HyperTrie<T> *, T>> *getOrCreateEdges(const key_pos_t subkey_pos) noexcept {
            auto &edges = this->edges_by_pos[subkey_pos];
            // if the map for subkey_pos doesn't exist create it.
            if (edges == nullptr) {
                edges = new map<uint64_t, variant<HyperTrie<T> *, T>>();
            }
            return edges;
        }

        /**
         * Get pointer to edges map by subkey_pos.
         * @param subkey_pos position of the next coordinate. MUST be in range [0,self->depth).
         * @return optional if it exists: a map that maps the used key_parts at pos of a key to sub-hypertries or T entries
         */
        map<key_part_t, variant<HyperTrie<T> *, T>> *getEdges(const key_pos_t subkey_pos) const {
            map<key_part_t, variant<HyperTrie<T> *, T>> *edges = this->edges_by_pos.at(subkey_pos);
            // if the map for subkey_pos doesn't exist create it.
            if (edges == nullptr)
                throw "edge does not exist.";
            else
                return edges;
        }

        /**
         * Add a child or a value to the hypertrie.
         * @param pos The key position where the child should be added.
         * @param key_part The key_part to be used at the key position pos.
         * @param value The sub-hypertrie or T to be set.
         * @return Optional Difference between old and new value if this-> depth is 1. TODO!
         */
        variant<HyperTrie<T> *, T> &
        setChild(const key_pos_t pos, const key_part_t key_part,
                 const optional<variant<HyperTrie<T> *, T>> &value = std::nullopt) {
            map<key_part_t, variant<HyperTrie<T> *, T>> * edge = getOrCreateEdges(pos);

            // depth > 1 means it is an inner node. So a sub-HyperTrie is added not a T value.
            if (this->depth > 1 && not value) {
                // no value was passed. So create a new sub HyperTrie of depth this->depth -1 and add it.
                variant<HyperTrie<T> *, T> child{new HyperTrie<T>(this->depth - uint8_t(1))};
                (*edge)[key_part] = child;
            } else {
                (*edge)[key_part] = *value;
            }
            // return new value
            return edge->at(key_part);
        }

        /**
         * Get a child by key_part position and key_part value.
         * @param subkey_pos The key position where to look for the child.
         * @param key_part The key_part identifing the child.
         * @return Optional the child if it exists. If this->depth is 1 the child is of value T otherwise it is a HyperTrie.
         */
        variant<HyperTrie<T> *, T> &getChild(const key_pos_t subkey_pos, const key_part_t key_part) {
            map<key_part_t, variant<HyperTrie<T> *, T>> *edges = getEdges(subkey_pos);
            // find child
            const auto &child_ = edges->find(key_part);
            // return it if it exist
            if (child_ != edges->end()) {
                return child_->second;
            }
            // in any other case return that no child exists
            throw "The requested child doesn't exist.";
        }

        void delChild(const key_pos_t pos, const key_part_t key_part) {
            map<key_part_t, variant<HyperTrie<T> *, T>> *edges = getEdges(pos);
            throw "Not yet implemented.";
        }

    public:

        /**
         * Get an value or SubTrie by a key.
         * @param key Vector of uint64 coordinates.
         * @return a SubTrie or a value depending on the length of the key.
         */
        variant<HyperTrie<T> *, T> &get(const Key_t &key) {
            vector<std::optional<key_part_t>> intern_key(this->depth);
            for (key_pos_t key_pos = 0; key_pos < key.size(); ++key_pos) {
                intern_key[key_pos] = {key[key_pos]};
            }
            return get(intern_key);
        }


        /**
         * Get an value or SubTrie by a key.
         * @param key Vector of optional uint64 key_parts. If a key_pos is an std::nullopt the key_part for that position is not set resulting in a slice.
         * @return a SubTrie or a value depending if the key contains slices..
         */
        variant<HyperTrie<T> *, T> &get(const vector<std::optional<key_part_t>> key) {
            if (this->empty()) {
                throw "hypertrie is emtpy.";
            }

            // extract non_slice_key_parts
            map<key_pos_t, key_part_t> non_slice_key_parts{};
            for (key_pos_t key_pos = 0; key_pos < size(key); ++key_pos) {
                if (key[key_pos]) {
                    non_slice_key_parts[key_pos] = *key[key_pos];
                }
            }

            if (non_slice_key_parts.empty()) {
                throw "empty slices are currently not supported.";
                //return {variant<HyperTrie<T> *, T>{this}};
            } else {
                // get child while there are non slice parts in the subkey.
                HyperTrie<T> *current_subtrie = this;
                PosCalc *posCalc = PosCalc::getInstance(this->depth);
                while (not non_slice_key_parts.empty()) {

                    // find key_pos with minimal cardinality position
                    // todo: this can be precomputed and cached for every possible cardinality order.
                    key_pos_t min_card_key_pos = getMinCardKeyPos(non_slice_key_parts, current_subtrie, posCalc);
                    key_pos_t min_card_subkey_pos = posCalc->key_to_subkey_pos(min_card_key_pos);

                    // get the child at the key_pos with minimal cardinality
                    variant<HyperTrie<T> *, T> &child =
                            current_subtrie->getChild(min_card_subkey_pos, non_slice_key_parts[min_card_key_pos]);

                    // HyperTrie<T> *another = std::get<HyperTrie<T> *>(child);
                    non_slice_key_parts.erase(min_card_key_pos);
                    if (non_slice_key_parts.empty())
                        return child;
                    current_subtrie = std::get<HyperTrie<T> *>(child);
                    posCalc = posCalc->use(min_card_key_pos);
                }
                throw "something is fishy";
            }
        }

        /**
         * Returns the smallest key that is used at the given position.
         * @param key_pos key position.
         * @return samllest key at given position or the maximum key_part_t value if no entry is in this HyperTrie.
         */
        inline key_part_t getMinKeyPart(const key_pos_t key_pos) {
            try {
                map<key_part_t, variant<HyperTrie<T> *, T>> *edges = getEdges(key_pos);
                if (not edges->empty()) {
                    return edges->begin()->first;
                }
            } catch (...) {
                return KEY_PART_MAX;
            }
        }

        /**
         * Returns the largest key that is used at the given position.
         * @param key_pos key position.
         * @return largest key at given position or the minimum key_part_t value if no entry is in this HyperTrie.
         */
        inline key_part_t getMaxKeyPart(const key_pos_t key_pos) const {
            try {
                map<key_part_t, variant<HyperTrie<T> *, T>> *edges = getEdges(key_pos);
                if (not edges->empty()) {
                    return edges->rbegin()->first;
                }
            } catch (...) {
                return KEY_PART_MIN;
            }
        }

        /**
         * Unsafe! Check first that hyperTrie is not empty.<br/>
         * Looksup the number of children at the given Position.
         * @param key_pos position to check amount of children.
         * @return number of children for given position.
         */
        inline size_t getCard(key_pos_t key_pos) const {
            return edges_by_pos.at(key_pos)->size();
        }

        inline vector<size_t> getCards(const vector<key_pos_t> &key_poss) const {
            vector<size_t> cards(key_poss.size());
            for (int i = 0; i < key_poss.size(); ++i) {
                cards[i] = getCard(key_poss.at(i));
            }
            return cards;
        }

        key_pos_t getMinCardKeyPos(const map<key_pos_t, key_part_t> &non_slice_key_parts,
                                   const HyperTrie<T> *result,
                                   const PosCalc *posCalc) const {
            size_t min_card = SIZE_MAX;
            key_pos_t min_card_key_pos = 0;
            for (const auto &
            [key_pos, key_part] : non_slice_key_parts) {
                size_t card = getCard(posCalc->key_to_subkey_pos(key_pos));
                if (card < min_card) {
                    min_card = card;
                    min_card_key_pos = key_pos;
                }
            }
            return min_card_key_pos;
        }


        /**
         * unsafe! Check first if hypertrie is not empty!
         * @return
         */
        key_pos_t getMinCardKeyPos() const {

            size_t min_card = SIZE_MAX;
            key_pos_t min_card_key_pos = 0;

            for (key_pos_t key_pos = 0; key_pos < edges_by_pos.size(); ++key_pos) {
                size_t card = getCard(key_pos);
                if (card < min_card) {
                    min_card = card;
                    min_card_key_pos = key_pos;
                }
            }
            return min_card_key_pos;
        }


    private:

        /**
         * Inserts and links the HyperTrie nodes recursively.
         * @param key key to save the value for
         * @param new_value the new value to be safed.
         * @param has_old_value if there was an value for this key before.
         * @param value_diff difference beween former value and current value
         * @param finished_subtries map of finished sub HyperTries
         * @param pos_calc PosCalc object for the current sub HyperTrie
         */
        void set_rek(const Key_t &key, const T &new_value, const bool &has_old_value, const T &value_diff,
                     unordered_map<subkey_mask_t, HyperTrie<T> *> &finished_subtries, PosCalc *pos_calc) {
            // update this node
            this->leafsum += value_diff;
            this->leafcount += not has_old_value;

            // add it to the finished ( means updated ) nodes.
            finished_subtries[pos_calc->getSubKeyMask()] = this;

            // subtrie has only one position left: insert value
            if (pos_calc->subkey_length == 1) {
                key_part_t key_part = key[pos_calc->subkey_to_key_pos(0)];

                std::optional<variant<HyperTrie<T> *, T>> value_{new_value};

                this->setChild(0, key_part, value_);
            } else { // depth > 1 -> inner node
                // a child must be set or updated for every subkey_pos available.
                for (const key_pos_t key_pos : pos_calc->getKeyPoss()) {
                    key_part_t key_part = key[key_pos];

                    // get pos_calc for next child and check if it was already updated earlier.
                    PosCalc *next_pos_calc = pos_calc->use(key_pos);
                    const auto &finished_child = finished_subtries.find(next_pos_calc->getSubKeyMask());


                    try {
                        // get the child at the current position.
                        variant<HyperTrie<T> *, T> &child_ = getChild(pos_calc->key_to_subkey_pos(key_pos), key_part);
                        // the child exists ...
                        // ... and the subtrie starting with the child was not already finished:
                        if (finished_child == finished_subtries.end()) {
                            // call this function for the child
                            HyperTrie<T> *child = std::get<HyperTrie<T> *>(child_);
                            child->set_rek(key, new_value, has_old_value, value_diff, finished_subtries, next_pos_calc);

                        }
                    } catch (...) {
                        // the child does not exist ...
                        // ... and the subtrie starting with the child was already finished:
                        if (finished_child != finished_subtries.end()) {
                            // set the child at this node
                            // TODO: could use references here as well
                            optional<variant<HyperTrie<T> *, T>> value_{{finished_child->second}};
                            this->setChild(pos_calc->key_to_subkey_pos(key_pos), key_part, value_);

                        } else { // ... and the subtrie starting with the child was not already finished:
                            // set a new child and call this function for the child
                            variant<HyperTrie<T> *, T> &new_child = this->setChild(pos_calc->key_to_subkey_pos(key_pos),
                                                                                   key_part);
                            HyperTrie<T> *&child = std::get<HyperTrie<T> *>(new_child);
                            child->set_rek(key, new_value, has_old_value, value_diff, finished_subtries, next_pos_calc);
                        }
                    }
                }
            }
        }

    public:

        /**
         * Set a value for a key.
         * @param key key to the value
         * @param value value to be set
         */
        void set(const Key_t key, const T value) {
            if (key.size() != this->depth) {
                throw "Key length must match HyperTrie->depth";
            }
            // check if there is already another value for this subkey.
            bool has_old_value = false;
            T oldValue{};
            try {
                variant<HyperTrie<T> *, T> &oldValue_ = get(key);
                has_old_value = true;
                oldValue = std::get<T>(oldValue_);
                if (oldValue == value) {
                    return;
                }
            } catch (...) {

            }

            // calculate value difference.
            T value_diff = has_old_value ? (value - oldValue) : value;

            // cache for already created sub HyperTries.
            std::unordered_map<subkey_mask_t, HyperTrie<T> *> finished_subtries{};

            // get pos_calc for this.
            subkey_mask_t subkey_mask(key.size());
            PosCalc *pos_calc = PosCalc::getInstance(subkey_mask);

            // store subkey recursively
            set_rek(key, value, has_old_value, value_diff, finished_subtries, pos_calc);
        }

        void del(const Key_t coords) {
            throw "Not yet implemented.";
        }

        inline bool empty() const noexcept {
            return leafcount == 0;
        }

        typename map<key_part_t, variant<HyperTrie<T> *, T>>::iterator begin(key_pos_t key_pos = 0) {
            return this->edges_by_pos.at(key_pos)->begin();
        }

        typename map<key_part_t, variant<HyperTrie<T> *, T>>::const_iterator cbegin(key_pos_t key_pos = 0) {
            return this->edges_by_pos.at(key_pos)->cbegin();
        }

        typename map<key_part_t, variant<HyperTrie<T> *, T>>::iterator
        lower_bound(key_part_t min_key_part = KEY_PART_MIN, key_pos_t key_pos = 0) {
            return this->edges_by_pos.at(key_pos)->lower_bound(min_key_part);

        }

        typename map<key_part_t, variant<HyperTrie<T> *, T>>::iterator end(key_pos_t key_pos = 0) {
            return this->edges_by_pos.at(key_pos)->end();
        }

        typename map<key_part_t, variant<HyperTrie<T> *, T>>::const_iterator cend(key_pos_t key_pos = 0) {
            return this->edges_by_pos.at(key_pos)->cend();
        }

        typename map<key_part_t, variant<HyperTrie<T> *, T>>::iterator
        upper_bound(key_part_t max_key_part = KEY_PART_MAX, key_pos_t key_pos = 0) {
            return this->edges_by_pos.at(key_pos)->upper_bound(max_key_part);
        }

        friend ostream &operator<<(ostream &out, HyperTrie<T> &trie) {
            out << "<HyperTrie: depth=" << int(trie.depth) << ", leafcount=" << int(trie.leafcount) << ", leafsum="
                << int(trie.leafsum) << ">";
            return out;
        }

    };
}

#endif //SPARSETENSOR_HYPERTRIE_HYPERTRIE_HPP
