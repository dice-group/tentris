#ifndef LIBSPARSETENSOR_HYPERTRIE_HPP
#define LIBSPARSETENSOR_HYPERTRIE_HPP


#include <cstdint>
#include <map>
#include <vector>
#include <variant>
#include <optional>
#include <unordered_map>
#include "boost/variant.hpp"
#include "PosCalc.hpp"
#include <limits>


using std::vector;
using std::variant;
using std::map;
using std::optional;
using std::tuple;


typedef uint64_t key_part_t;
using PosCalc::key_pos_t;
using PosCalc::subkey_mask_t;

template<typename T>
class HyperTrieMatchKeyPosIterator;

template<typename T>
class HyperTrie {
public:
    friend class HyperTrieMatchKeyPosIterator<T>;


    HyperTrie(key_pos_t depth) : depth(depth) {}

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
    map<key_part_t, variant<HyperTrie<T> *, T>> *getOrCreateEdges(key_pos_t subkey_pos) noexcept {
        auto edges = this->edges_by_pos[subkey_pos];
        // if the map for subkey_pos doesn't exist create it.
        if (edges == nullptr) {
            edges = new map<uint64_t, variant<HyperTrie<T> *, T>>();
            this->edges_by_pos[subkey_pos] = edges;
        }
        return edges;
    }

/**
 * Get pointer to edges map by subkey_pos.
 * @param subkey_pos position of the next coordinate. MUST be in range [0,self->depth).
 * @return optional if it exists: a map that maps the used key_parts at pos of a key to sub-hypertries or T entries
 */
    optional<map<key_part_t, variant<HyperTrie<T> *, T>> *> getEdges(key_pos_t subkey_pos) {
        map<key_part_t, variant<HyperTrie<T> *, T>> *edges = this->edges_by_pos.at(subkey_pos);
        // if the map for subkey_pos doesn't exist create it.
        if (edges == nullptr)
            return std::nullopt;
        else
            return {edges};
    }

/**
 * Add a child or a value to the hypertrie.
 * @param pos The key position where the child should be added.
 * @param key_part The key_part to be used at the key position pos.
 * @param value The sub-hypertrie or T to be set.
 * @return Optional Difference between old and new value if this-> depth is 1. TODO!
 */
    variant<HyperTrie<T> *, T>
    setChild(key_pos_t pos, key_part_t key_part, optional<variant<HyperTrie<T> *, T>> &value) {
        auto edge = getOrCreateEdges(pos);

        // depth > 1 means it is an inner node. So a sub-HyperTrie is added not a T value.
        if (this->depth > 1) {
            // no value was passed. So create a new sub HyperTrie of depth this->depth -1 and add it.
            if (not value) {
                variant<HyperTrie<T> *, T> child{new HyperTrie<T>(this->depth - uint8_t(1))};
                (*edge)[key_part] = child;
                return child;
            } // a value was passed. Add it.
            else {
                (*edge)[key_part] = *value;
                return *value;
            }
        } // depth == 1 means it is a leaf node. So Values are directly assigned to key_parts
        else {
            // look current entry up.
            auto old_value_ = (*edge).find(key_part);
            // write new value
            (*edge)[key_part] = *value;

            // if old value didn't exist return new value
            if (old_value_ == (*edge).end()) {
                return *value;
            } // if old value existed return difference.
            else {
                T new_value = std::get<T>(*value);
                T old_value = std::get<T>(old_value_->second);
                T value_diff = new_value - old_value;
                return {value_diff};
            }
        }
    }

/**
 * Get a child by key_part position and key_part value.
 * @param subkey_pos The key position where to look for the child.
 * @param key_part The key_part identifing the child.
 * @return Optional the child if it exists. If this->depth is 1 the child is of value T otherwise it is a HyperTrie.
 */
    optional<variant<HyperTrie<T> *, T>> getChild(key_pos_t subkey_pos, key_part_t key_part) {
        optional<map<key_part_t, variant<HyperTrie<T> *, T>> *> edges_ = getEdges(subkey_pos);
        // check if HyperTrie is emtpy.
        if (edges_) {
            map<uint64_t, variant<HyperTrie<T> *, T>> *edges = *edges_;
            // find child
            auto child_ = edges->find(key_part);
            // return it if it exist
            if (child_ != edges->end()) {
                return {child_->second};
            }
        }
        // in any other case return that no child exists
        return std::nullopt;
    }

    void delChild(key_pos_t &pos, key_part_t key_part) {
        optional<map<key_part_t, variant<HyperTrie<T> *, T>> *> edges_ = getEdges(pos);
        throw "Not yet implemented.";
    }

public:

/**
 * Get an value or SubTrie by a key.
 * @param key Vector of uint64 coordinates.
 * @return a SubTrie or a value depending on the length of the key.
 */
//[[deprecated]]
    optional<variant<HyperTrie<T> *, T>> get(const vector<key_part_t> &key) {
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
    optional<variant<HyperTrie<T> *, T>> get(vector<std::optional<key_part_t>> &key) {
        // extract non_slice_key_parts
        map<key_pos_t, key_part_t> non_slice_key_parts{};
        for (key_pos_t key_pos = 0; key_pos < size(key); ++key_pos) {
            if (key[key_pos]) {
                non_slice_key_parts[key_pos] = *key[key_pos];
            }
        }
        if (this->empty()) {
            return std::nullopt;
        }

        if (non_slice_key_parts.empty()) {
            return {variant<HyperTrie<T> *, T>{this}};
        } else {
            // get child while there are non slice parts in the subkey.
            HyperTrie<T> *result = this;
            PosCalc *posCalc = PosCalc::getInstance(this->depth);
            while (not non_slice_key_parts.empty()) {

                // find key_pos with minimal cardinality position
                // todo: this can be precomputed and cached for every possible cardinality order.
                key_pos_t min_card_key_pos = getMinCardKeyPos(non_slice_key_parts, result, posCalc);
                key_pos_t min_card_subkey_pos = posCalc->key_to_subkey_pos(min_card_key_pos);

                // get the child at the key_pos with minimal cardinality
                const optional<variant<HyperTrie<T> *, T>> &child =
                        result->getChild(min_card_subkey_pos, non_slice_key_parts[min_card_key_pos]);

                // check if there is actually a child
                if (child) {
                    // depth = 1 -> return value
                    if (result->depth == 1) {

                        return {variant<HyperTrie<T> *, T>{*child}};
                    } else { // else it is a HyperTrie
                        HyperTrie<T> *another = std::get<HyperTrie<T> *>(*child);
                        result = std::get<HyperTrie<T> *>(*child);
                        non_slice_key_parts.erase(min_card_key_pos);
                        posCalc = posCalc->use(min_card_key_pos);
                    }
                } else {
                    return std::nullopt;
                }
            }
            return {variant<HyperTrie<T> *, T>{result}};
        }
    }

    /**
     * Returns the smallest key that is used at the given position.
     * @param key_pos key position.
     * @return samllest key at given position or the maximum key_part_t value if no entry is in this HyperTrie.
     */
    inline key_part_t getMinKeyPart(key_pos_t key_pos) {
        const std::optional<map<key_part_t, variant<HyperTrie<T> *, T>> *> &edges_ = getEdges(key_pos);
        if (edges_) {
            map<key_part_t, variant<HyperTrie<T> *, T>> *edges = (*edges_);
            if (not edges->empty()) {
                return edges->begin()->first;
            }
        }
        return std::numeric_limits<key_part_t>::max();
    }

    /**
     * Returns the largest key that is used at the given position.
     * @param key_pos key position.
     * @return largest key at given position or the minimum key_part_t value if no entry is in this HyperTrie.
     */
    inline key_part_t getMaxKeyPart(key_pos_t key_pos) {
        const std::optional<map<key_part_t, variant<HyperTrie<T> *, T>> *> &edges_ = getEdges(key_pos);
        if (edges_) {
            map<key_part_t, variant<HyperTrie<T> *, T>> *edges = (*edges_);
            if (not edges->empty()) {
                return edges->rbegin()->first;
            }
        }
        return std::numeric_limits<key_part_t>::min();
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

    key_pos_t
    getMinCardKeyPos(const map<key_pos_t, key_part_t> &non_slice_key_parts, const HyperTrie<T> *result,
                     const PosCalc *posCalc) const {
        size_t min_card = ::std::numeric_limits<size_t>::max();
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
 * TODO: unsafe! Check first if hypertrie is not empty!
 * @return
 */
    key_pos_t getMinCardKeyPos() {

        size_t min_card = ::std::numeric_limits<unsigned long>::max();
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
    void set_rek(vector<key_part_t> &key, T &new_value, bool has_old_value, T &value_diff,
                 std::unordered_map<subkey_mask_t, HyperTrie<T> *> &finished_subtries, PosCalc *pos_calc) {
        // update this node
        this->leafsum += value_diff;
        this->leafcount += not has_old_value;

        // add it to the finished ( means updated ) nodes.
        finished_subtries[pos_calc->removed_positions] = this;

        // subtrie has only one position left: insert value
        if (pos_calc->subkey_length == 1) {
            key_part_t key_part = key[pos_calc->subkey_to_key_pos(0)];

            optional<variant<HyperTrie<T> *, T>> value_ = optional < variant<HyperTrie<T> *, T>>
            { variant<HyperTrie<T> *, T>{new_value}};

            this->setChild(0, key_part, value_);
        } else { // depth > 1 -> inner node
            // a child must be set or updated for every subkey_pos available.
            for (key_pos_t key_pos : pos_calc->subkey_to_key) {
                key_part_t key_part = key[key_pos];

                // get pos_calc for next child and check if it was already updated earlier.
                PosCalc *const next_pos_calc = pos_calc->use(key_pos);
                auto finished_child = finished_subtries.find(next_pos_calc->removed_positions);

                // get the child at the current position.
                optional<variant<HyperTrie<T> *, T>> child_ = getChild(pos_calc->key_to_subkey_pos(key_pos), key_part);

                // the child exists ...
                if (child_) {
                    // ... and the subtrie starting with the child was not already finished:
                    if (finished_child == finished_subtries.end()) {
                        // call this function for the child
                        HyperTrie<T> *child = std::get<HyperTrie<T> *>(*child_);
                        child->set_rek(key, new_value, has_old_value, value_diff, finished_subtries, next_pos_calc);

                    }

                } else { // the child does not exist ...
                    // ... and the subtrie starting with the child was already finished:
                    if (finished_child != finished_subtries.end()) {
                        // set the child at this node
                        variant<HyperTrie<T> *, T> value{finished_child->second};
                        optional<variant<HyperTrie<T> *, T>> value_{value};
                        this->setChild(pos_calc->key_to_subkey_pos(key_pos), key_part, value_);

                    } else { // ... and the subtrie starting with the child was not already finished:
                        // set a new child and call this function for the child
                        optional<variant<HyperTrie<T> *, T>> value_ = std::nullopt;
                        variant<HyperTrie<T> *, T> new_child = this->setChild(pos_calc->key_to_subkey_pos(key_pos),
                                                                              key_part, value_);
                        HyperTrie<T> *child = std::get<HyperTrie<T> *>(new_child);
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
    void set(vector<key_part_t> &key, T &value) {
        if (key.size() != this->depth) {
            throw "Key length must match HyperTrie->depth";
        }
        // check if there is already another value for this subkey.
        optional<variant<HyperTrie<T> *, T>> oldValue_ = get(key);
        bool has_old_value = false;
        T oldValue;
        if (oldValue_) {
            has_old_value = true;
            oldValue = std::get<T>(*oldValue_);
            // when the value doesn't we don't need to do anything.
            if (oldValue == value) {
                return;
            }
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

    void del(vector<key_part_t> &coords) {
        throw "Not yet implemented.";
    }

    inline bool empty() const noexcept {
        return leafcount == 0;
    }

};


#endif //LIBSPARSETENSOR_HYPERTRIE_HPP
