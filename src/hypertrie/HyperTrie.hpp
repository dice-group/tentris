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

class HyperTrie::MatchKeyPosIterator;

template<typename T>
class HyperTrie {
public:
    friend class HyperTrie::MatchKeyPosIterator;

    typedef uint64_t key_part_t;
    typedef PosCalc::key_pos_t key_pos_t;
    typedef PosCalc::subkey_mask_t subkey_mask_t;

    HyperTrie(key_pos_t depth) : depth(depth) {}

    /**
     * Depth is the length of the keys.
     */
    key_pos_t depth;

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
    vector<map<key_part_t, variant<HyperTrie *, T>> *> edges_by_pos =
            vector<map<key_part_t, variant<HyperTrie *, T>> *>(depth);

    /**
     * Get a pointer to the edges map by subkey_pos. If the map doesn't exist so far it is created.
     * @param subkey_pos position of the next coordinate. MUST be in range [0,self->depth).
     * @return a pointer to a map that maps the used key_parts at subkey_pos of a subkey to sub-hypertries or T entries.
     */
    map<key_part_t, variant<HyperTrie *, T>> *getOrCreateEdges(key_pos_t subkey_pos) noexcept {
        auto edges = this->edges_by_pos[subkey_pos];
        // if the map for subkey_pos doesn't exist create it.
        if (edges == nullptr) {
            edges = new map<uint64_t, variant<HyperTrie *, T>>();
            this->edges_by_pos[subkey_pos] = edges;
        }
        return edges;
    }

    /**
     * Get pointer to edges map by subkey_pos.
     * @param subkey_pos position of the next coordinate. MUST be in range [0,self->depth).
     * @return optional if it exists: a map that maps the used key_parts at pos of a key to sub-hypertries or T entries
     */
    optional<map<key_part_t, variant<HyperTrie *, T>> *> getEdges(key_pos_t subkey_pos) noexcept {
        map<key_part_t, variant<HyperTrie *, T>> *edges = this->edges_by_pos[subkey_pos];
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
    variant<HyperTrie *, T> setChild(key_pos_t pos, key_part_t key_part, optional<variant<HyperTrie *, T>> &value) {
        auto edge = getOrCreateEdges(pos);

        // depth > 1 means it is an inner node. So a sub-HyperTrie is added not a T value.
        if (this->depth > 1) {
            // no value was passed. So create a new sub HyperTrie of depth this->depth -1 and add it.
            if (not value) {
                variant<HyperTrie *, T> child{new HyperTrie(this->depth - uint8_t(1))};
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
    optional<variant<HyperTrie *, T>> getChild(key_pos_t subkey_pos, key_part_t key_part) {
        optional<map<key_part_t, variant<HyperTrie *, T>> *> edges_ = getEdges(subkey_pos);
        // check if HyperTrie is emtpy.
        if (edges_) {
            map<uint64_t, variant<HyperTrie *, T>> *edges = *edges_;
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
        optional<map<key_part_t, variant<HyperTrie *, T>> *> edges_ = getEdges(pos);
        throw "Not yet implemented.";
    }

public:

    /**
     * Get an value or SubTrie by a key.
     * @param key Vector of uint64 coordinates.
     * @return a SubTrie or a value depending on the length of the key.
     */
    [[deprecated]]
    optional<variant<HyperTrie *, T>> get(vector<key_part_t> &key) {
        // TODO: 0
        // implement optional<key_part_t> for range queries
        // TODO: 1
        // TODO: optimize access order
        HyperTrie *current_trie = this;
        for (key_pos_t pos = 0; pos < key.size(); pos++) {
            const optional<variant<HyperTrie *, T>> &child_ = current_trie->getChild(0, key[pos]);
            // child exists
            if (child_) {
                if (pos != key.size() - 1) { // leaf
                    current_trie = std::get<HyperTrie *>(*child_);
                } else { // inner node
                    return {*child_};
                }
            } else
                return std::nullopt;
        }
        // if subkey is empty return this
        return {variant<HyperTrie *, T>{current_trie}};
    }

    /**
     * Get an value or SubTrie by a key.
     * @param key Vector of optional uint64 key_parts. If a key_pos is an std::nullopt the key_part for that position is not set resulting in a slice.
     * @return a SubTrie or a value depending if the key contains slices..
     */
    optional<variant<HyperTrie *, T>> get(vector<std::optional<key_part_t>> &key) {
        // extract non_slice_key_parts
        map<key_pos_t, key_part_t> non_slice_key_parts{};
        for (key_pos_t key_pos = 0; key_pos < size(key); ++key_pos) {
            if (key[key_pos]) {
                non_slice_key_parts[key_pos] = *key[key_pos];
            }
        }

        if (non_slice_key_parts.empty()) {
            return {{this}};
        } else {
            // get child while there are non slice parts in the subkey.
            HyperTrie *result = this;
            while (not non_slice_key_parts.empty()) {

                // find key_pos with minimal cardinality position
                // todo: this can be precomputed and cached for every possible cardinality order.
                key_pos_t min_card_key_pos = getMinCardKeyPos(non_slice_key_parts, result);

                // get the child at the key_pos with minimal cardinality
                const optional<variant<HyperTrie *, T>> &child =
                        result->getChild(min_card_key_pos, non_slice_key_parts[min_card_key_pos]);

                // check if there is actually a child
                if (child) {
                    // depth = 1 -> return value
                    if (result->depth == 1) {
                        return {{*child}};
                    } else { // else it is a HyperTrie
                        result = std::get<HyperTrie *>(*child);
                    }
                }
            }
            return {{result}};
        }
    }

    key_pos_t getMinCardKeyPos(const map<key_pos_t, key_part_t> &non_slice_key_parts, const HyperTrie *result) const {
        size_t min_card = ::std::numeric_limits<unsigned long>::max();
        key_pos_t min_card_key_pos = 0;
        for (const auto &key_pos_and_part_ : non_slice_key_parts) {
            const key_pos_t key_pos = key_pos_and_part_.first;
            size_t card = result->edges_by_pos.at(key_pos)->size();
            if (card < min_card) {
                min_card = card;
                min_card_key_pos = key_pos;
            }
        }
        return min_card_key_pos;
    }

    /**
     * TODO: unsafe!
     * @return
     */
    key_pos_t getMinCardKeyPos() const {

        size_t min_card = ::std::numeric_limits<unsigned long>::max();
        key_pos_t min_card_key_pos = 0;

        for (size_t key_pos = 0; key_pos < edges_by_pos.size(); ++key_pos) {
            map<key_part_t, variant<HyperTrie *, T>> *edge = edges_by_pos.at(key_pos);
            size_t card = edge->size();
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
                 std::unordered_map<subkey_mask_t, HyperTrie *> &finished_subtries, PosCalc *pos_calc) {
        // update this node
        this->leafsum += value_diff;
        this->leafcount += not has_old_value;

        // add it to the finished ( means updated ) nodes.
        finished_subtries[pos_calc->removed_positions] = this;

        // subtrie has only one position left: insert value
        if (pos_calc->subkey_length == 1) {
            key_part_t key_part = key[pos_calc->subkey_to_key_pos(0)];

            optional<variant<HyperTrie *, T>> value_ = optional < variant<HyperTrie *, T>>
            { variant<HyperTrie *, T>{new_value}};

            this->setChild(0, key_part, value_);
        } else { // depth > 1 -> inner node
            // a child must be set or updated for every subkey_pos available.
            for (key_pos_t key_pos : pos_calc->subkey_to_key) {
                key_part_t key_part = key[key_pos];

                // get pos_calc for next child and check if it was already updated earlier.
                PosCalc *const next_pos_calc = pos_calc->use(key_pos);
                auto finished_child = finished_subtries.find(next_pos_calc->removed_positions);

                // get the child at the current position.
                optional<variant<HyperTrie *, T>> child_ = getChild(pos_calc->key_to_subkey_pos(key_pos), key_part);

                // the child exists ...
                if (child_) {
                    // ... and the subtrie starting with the child was not already finished:
                    if (finished_child == finished_subtries.end()) {
                        // call this function for the child
                        HyperTrie *child = std::get<HyperTrie *>(*child_);
                        child->set_rek(key, new_value, has_old_value, value_diff, finished_subtries, next_pos_calc);

                    }

                } else { // the child does not exist ...
                    // ... and the subtrie starting with the child was already finished:
                    if (finished_child != finished_subtries.end()) {
                        // set the child at this node
                        variant<HyperTrie *, T> value{finished_child->second};
                        optional<variant<HyperTrie *, T>> value_{value};
                        this->setChild(pos_calc->key_to_subkey_pos(key_pos), key_part, value_);

                    } else { // ... and the subtrie starting with the child was not already finished:
                        // set a new child and call this function for the child
                        optional<variant<HyperTrie *, T>> value_ = std::nullopt;
                        variant<HyperTrie *, T> new_child = this->setChild(pos_calc->key_to_subkey_pos(key_pos),
                                                                           key_part, value_);
                        HyperTrie *child = std::get<HyperTrie *>(new_child);
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
        optional<variant<HyperTrie *, T>> oldValue_ = get(key);
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
        std::unordered_map<subkey_mask_t, HyperTrie *> finished_subtries{};

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

namespace HyperTrie {
    using std::tuple;

    template<typename T>
    class MatchKeyPosIterator : public std::iterator<std::input_iterator_tag,
            std::tuple<key_part_t, vector<variant<HyperTrie *, T>>>> {
    public:

        MatchKeyPosIterator(const HyperTrie *&hyperTrie, const vector<key_pos_t> &key_poss) {

            if (hyperTrie->empty()) {
                end = true;
            } else {
                const key_pos_t min_card_key_pos = hyperTrie->getMinCardKeyPos();

                // init key_poss
                bool seen_key_pos = false;
                for (const key_pos_t &key_pos : key_poss)
                    if (key_pos != min_card_key_pos)
                        if (not seen_key_pos)
                            this->key_poss.push_back(key_pos);
                        else
                            this->key_poss.push_back(key_pos - 1);
                    else
                        seen_key_pos = true;

                // init subkey
                for (int i = 0; i < this->key_poss.size(); ++i) {
                    this->subkey.push_back({});
                }

                // get iterator to iterate the sub-HyperTries or values by the min-card position.
                map<key_part_t, variant<HyperTrie *, T>> *edges = hyperTrie->edges_by_pos.at(min_card_key_pos);
                this->child_it = edges->begin();
                this->child_it_end = edges->end();
                (*this)++;
            }
        }

    private:
        map<key_part_t, variant<HyperTrie *, T>>::iterator child_it;
        map<key_part_t, variant<HyperTrie *, T>>::iterator child_it_end;
        key_part_t key_part{};
        bool end{};
        vector<key_pos_t> key_poss{};
        vector<std::optional<key_part_t>> subkey{};

    public:
        key_part_t *first = &key_part;
        variant<HyperTrie *, T> *second;


    public:

        MatchKeyPosIterator &operator++() {
            if (child_it == child_it_end) { // end reached.
                end = true;
            } else { // end not yet at end
                do {
                    // get next key_part
                    key_part = child_it->first;
                    // get next result for min-card position
                    variant<HyperTrie *, T> &child = child_it->second;
                    if (subkey.size() == 0) {
                        second = &child;
                        break;
                    } else {
                        // write the key_part to the relevant positions of the subkey
                        for (const key_pos_t &key_pos : key_poss) {
                            subkey[key_pos] = key_part;
                        }

                        // check if the same key_part exists also for the other relevant key_pos
                        optional<variant<HyperTrie *, T>> &result = std::get<HyperTrie *>(child)->get(subkey);
                        if (result) {
                            second = &(*result);
                            break;
                        } else {
                            continue;
                        }
                    }
                    child_it++;
                } while (child_it != child_it_end);
            }
            child_it++;
            return *this;
        }

        MatchKeyPosIterator operator++(int) {
            MatchKeyPosIterator it_copy{*this};
            operator++();
            return it_copy;
        }

        bool operator==(const MatchKeyPosIterator &rhs) const {
            // TODO: does this make sense?
            if (rhs.end && end)
                return true;
            else
                return key_part == rhs.key_part;
        }

        bool operator!=(const MatchKeyPosIterator &rhs) const {
            // TODO: does this make sense?
            if (rhs.end != end)
                return true;
            else
                return key_part != rhs.key_part;
        }
    };
};


#endif //LIBSPARSETENSOR_HYPERTRIE_HPP
