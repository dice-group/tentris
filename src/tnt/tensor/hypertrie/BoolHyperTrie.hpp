#ifndef SPARSETENSOR_HYPERTRIE_BOOLHYPERTRIE_HPP
#define SPARSETENSOR_HYPERTRIE_BOOLHYPERTRIE_HPP

#include <cstdint>
#include <map>
#include <set>
#include <vector>
#include <variant>
#include <optional>
#include <unordered_map>
#include <limits>
#include <ostream>
#include <tuple>
#include <functional>

#include "tnt/tensor/hypertrie/PosCalc.hpp"
#include "tnt/util/container/VecMap.hpp"
#include "tnt/util/container/VecSet.hpp"
#include "tnt/util/All.hpp"

namespace tnt::tensor::hypertrie {

    class BoolHyperTrie {
        using Key_t = tnt::util::types::Key_t;
        using key_pos_t = tnt::util::types::key_pos_t;
        using key_part_t = tnt::util::types::key_part_t;
        using subkey_mask_t = tnt::util::types::subkey_mask_t;
    public:
        /**
         * Inner edges are encoded by mapping a key_part to an subhypertrie. Only key_parts that map to an non-zero
         * subhypertrie are stored. A inner_edges is used for every position of the stored keys.
         */
        using inner_edges = tnt::util::container::VecMap<key_part_t, BoolHyperTrie *>;
        /**
         * Leaf edges are encoded by storing only the key_parts that map to a true. All other key parts map to zero.
         */
        using leaf_edges = tnt::util::container::VecSet<key_part_t>;
    private:

        // TODO: write destructor
        // TODO: implement del
        key_pos_t _depth{}; ///< The depth defines the length of keys.

        size_t _leafcount{}; ///< Number of entries.

        /**
         * Stores the subtries. For depth > 1 a vector of length equal to depth stores for every position in the keys to
         * the non-zero elements a map that holds the subtries for that position. The map stores then for every key_part
         * used at any key to a non-zero element at that position the proper subBoolHyperTrie.
         */
        std::variant<std::vector<inner_edges>, leaf_edges> _subtries{};



    public:
        /**
         * The default constructor creates a empty BoolHyperTrie with depth = 1.
         */
        BoolHyperTrie() : _depth(1), _subtries{leaf_edges{}} {}

        /**
         * Creates a empty BoolHyperTrie with given depth.
         * @param depth depth of the BoolHyperTrie
         */
        explicit BoolHyperTrie(key_pos_t depth) : _depth(depth), _subtries{} {
            if (depth > 1) {
                _subtries = std::vector<inner_edges>(depth);
            } else {
                _subtries = leaf_edges{};
            }
        }

        /**
         * Get the depth of the BoolHyperTrie.
         * @return depth of this
         */
        inline const key_pos_t &depth() const noexcept {
            return _depth;
        }

        /**
         * Get the number of none-zero elements stored.
         * @return number of stored none-zero elements
         */
        inline const size_t &size() const noexcept {
            return _leafcount;
        }

    private:


        /**
         * Returns the inner edges for a given position of this BoolHyperTrie.
         * @param subkey_pos key_position in this hypertrie. May differ from the position in an parent BoolHyperTrie.
         * @return a map of inner_edges.
         * @throws std::exception::bad_variant_access if the depth of this BoolHyperTrie is 1
         * @throws std::out_of_range if not 1 <= subkey_pos <= this->depth() 
         */
        inner_edges &getInnerEdges(const key_pos_t &subkey_pos) {
            return std::get<std::vector<inner_edges>>(this->_subtries).at(subkey_pos);
        }

        /**
         * Returns leaf edges.
         * @throws std::exception::bad_variant_access if the depth of this BoolHyperTrie is not 1
         * @return a set of leaf edges.
         */
        inline leaf_edges &getLeafEdges() {
            return std::get<leaf_edges>(this->_subtries);
        }

        /**
         * Adds an Child BoolHyperTrie at the given position and for the given key_part.
         * @param pos position of the key_part
         * @param key_part a key_part that must not be in use at this position and BoolHyperTrie.
         * @param subhypertrie A pointer to a BoolHyperTrie. For consistency it must have a depth = --this->depth(). The
         * argument is consumed and thus must not be used again after calling this function.
         * @throws std::exception::bad_variant_access if the depth of this BoolHyperTrie is 1
         * @throws std::out_of_range if not 1 <= subkey_pos <= this->depth()
         */
        inline void addChildAsPointer(const key_pos_t &pos, key_part_t key_part, BoolHyperTrie *&subhypertrie) {
            inner_edges &edges = getInnerEdges(pos);
            edges.setItem(key_part, subhypertrie);
        }

        /**
         * Creates a new Child BoolHyperTrie at the given position and for the given key_part.
         * @param key_pos position of the key_part
         * @param key_part a key_part that must not be in use at this position and BoolHyperTrie.
         * @return a pointer to the created child BoolHyperTrie
         * @throws std::exception::bad_variant_access if the depth of this BoolHyperTrie is 1
         * @throws std::out_of_range if not 1 <= subkey_pos <= this->depth()
         */
        inline BoolHyperTrie *insertNewChild(const key_pos_t &key_pos, const key_part_t &key_part) {
            inner_edges &edges = getInnerEdges(key_pos);
            // no value was passed. So create a new sub HyperTrie of _depth this->_depth -1 and add it.
            BoolHyperTrie *child_ = new BoolHyperTrie(this->_depth - (key_pos_t(1)));
            edges.setItem(key_part, child_);
            // return new value
            return child_;
        }

        /**
         * Get an child of an inner node at the given position and for the given key_part.
         * @param key_pos position of the key_part
         * @param key_part key_part where to look for the child
         * @return the child
         * @throws std::exception::bad_variant_access if the depth of this BoolHyperTrie is 1
         * @throws std::out_of_range if not 1 <= subkey_pos <= this->depth() or the key doesn't exist
         */
        BoolHyperTrie *getInnerChild(const key_pos_t &key_pos, const key_part_t &key_part) {
            inner_edges &edges = getInnerEdges(key_pos);
            // find child
            return edges.at(key_part);
        }

    public:

        /**
         * Get an value or sub BoolHyperTrie by a key.
         * @param key key to the requested part
         * @return a sub BoolHyperTrie or a value depending on the length of the key.
         * @throws the entry doesn't exist
         */
        std::variant<BoolHyperTrie *, bool> get(const Key_t &key) const {
            std::vector<std::optional<key_part_t>> intern_key(this->_depth);
            for (key_pos_t key_pos = 0; key_pos < key.size(); ++key_pos) {
                intern_key[key_pos] = {key[key_pos]};
            }
            return get(intern_key);
        }


        /**
         * Get an value or sub BoolHyperTrie by a key.
         * @param key Vector of optional key_parts. If a key_pos is an std::nullopt the key_part for that position is
         * not set resulting in a slice.
         * @return a sub BoolHyperTrie or a value depending if the key contains slices.
         */
        std::variant<BoolHyperTrie *, bool> get(const std::vector<std::optional<key_part_t>> &key) const {
            if (this->empty()) {
                throw "hypertrie is emtpy.";
            }

            // extract non_slice_key_parts
            std::map<key_pos_t, key_part_t> non_slice_key_parts{};
            for (key_pos_t key_pos = 0; key_pos < key.size(); ++key_pos) {
                if (key[key_pos]) {
                    non_slice_key_parts[key_pos] = *key[key_pos];
                }
            }

            if (non_slice_key_parts.empty()) {
                throw "empty slices are currently not supported.";
                //return {variant<HyperTrie<bool> *, T>{this}}; // TODO: is there a problem with this?
            } else {
                // get child while there are non slice parts in the subkey.
                BoolHyperTrie *current_subtrie = const_cast<BoolHyperTrie *>(this);
                PosCalc *posCalc = PosCalc::getInstance(this->_depth);
                while (not non_slice_key_parts.empty()) {
                    if (non_slice_key_parts.size() == 1) {
                        const auto&[key_pos, key_part] = *non_slice_key_parts.cbegin();

                        if (current_subtrie->_depth == 1) {
                            leaf_edges &entries = current_subtrie->getLeafEdges();
                            if (entries.contains(key_part))
                                return true;
                            else
                                throw "No value stored for given Key.";
                        } else {
                            key_pos_t subkey_pos = posCalc->key_to_subkey_pos(key_pos);

                            return current_subtrie->getInnerChild(subkey_pos, non_slice_key_parts[key_pos]);
                        }
                    } else {
                        key_pos_t min_card_key_pos = current_subtrie->getMinCardKeyPos(non_slice_key_parts, posCalc);
                        key_pos_t min_card_subkey_pos = posCalc->key_to_subkey_pos(min_card_key_pos);

                        current_subtrie = current_subtrie->getInnerChild(min_card_subkey_pos,
                                                                         non_slice_key_parts[min_card_key_pos]);

                        non_slice_key_parts.erase(min_card_key_pos);
                        posCalc = posCalc->use(min_card_key_pos);
                    }
                }
                throw "Is never reached.";
            }
        }

        /**
         * Returns the smallest key that is used at the given position.
         * @param key_pos position of the key_part
         * @return smallest key_part at given position or the maximum key_part_t value if no entry is in this BoolHyperTrie.
         */
        inline key_part_t getMinKeyPart(const key_pos_t &key_pos = 0) {
            try {
                if (this->_depth == 1) {
                    return std::get<leaf_edges>(this->_subtries).min();
                } else {
                    return std::get<std::vector<inner_edges>>(this->_subtries).at(key_pos).min();
                }
            } catch (...) {}
            return KEY_PART_MAX;
        }

        /**
         * Returns the largest key that is used at the given position.
         * @param key_pos position of the key_part
         * @return largest key_part at given position or the minimum key_part_t value if no entry is in this BoolHyperTrie.
         */
        inline key_part_t getMaxKeyPart(const key_pos_t &key_pos = 0) const {
            try {
                if (this->_depth == 1) {
                    return std::get<leaf_edges>(this->_subtries).max();
                } else {
                    return std::get<std::vector<inner_edges>>(this->_subtries).at(key_pos).min();
                }
            } catch (...) {}
            return KEY_PART_MIN;
        }

        /**
         * Lookup the number of children at the given position.
         * @param key_pos position to check amount of children.
         * @return number of children for given position.
         */
        inline size_t getCard(const key_pos_t &key_pos) const {
            if (this->_depth == 1) {
                return std::get<leaf_edges>(this->_subtries).size();
            } else {
                return std::get<std::vector<inner_edges>>(this->_subtries).at(key_pos).size();
            }
        }

        /**
         * Get the cardinalities (numbers of direct children) for the given key positions.
         * @param key_poss vector of key positions
         * @return cardinalities at the requested positions
         */
        inline std::vector<size_t> getCards(const std::vector<key_pos_t> &key_poss) const {
            std::vector<size_t> cards(key_poss.size());
            for (size_t i = 0; i < key_poss.size(); ++i) {
                cards[i] = getCard(key_poss.at(i));
            }
            return cards;
        }

        /**
         * Given a map that has key_pos keys, return the key_pos out of them with the minimum cardinality.
         * @param map_from_key_pos a map that has key_pos as keys
         * @param posCalc a position calculator
         * @return the minimum key position.
         */
        key_pos_t getMinCardKeyPos(const std::map<key_pos_t, key_part_t> &map_from_key_pos,
                                   const PosCalc *posCalc) const {
            size_t min_card = SIZE_MAX;
            key_pos_t min_card_key_pos = 0;
            for (const key_pos_t &key_pos : keys(map_from_key_pos)) {
                size_t card = getCard(posCalc->key_to_subkey_pos(key_pos));
                if (card < min_card) {
                    min_card = card;
                    min_card_key_pos = key_pos;
                }
            }
            return min_card_key_pos;
        }

        /**
         * Given a map that has key_pos keys, return the key_pos out of them with the minimum cardinality.
         * @param map_from_key_pos a map that has key_pos as keys
         * @param posCalc a position calculator
         * @return the minimum key position.
         */
        key_pos_t getMinCardKeyPos() const {
            size_t min_card = SIZE_MAX;
            key_pos_t min_card_key_pos = 0;
            for (key_pos_t key_pos = 0; key_pos < _depth; ++key_pos) {
                size_t card = getCard(key_pos);
                if (card < min_card) {
                    min_card = card;
                    min_card_key_pos = key_pos;
                }
            }
            return min_card_key_pos;
        }

    private:
        void del_rek(const Key_t &key, std::unordered_map<subkey_mask_t, BoolHyperTrie *> &finished_subtries,
                     PosCalc *pos_calc) {
            throw "del_rek not yet implemented.";
        }

        /**
         * Inserts and links the HyperTrie nodes recursively.
         * @param key key to save the value for
         * @param finished_subtries map of finished sub HyperTries
         * @param pos_calc PosCalc object for the current sub HyperTrie
         */
        void set_rek(const Key_t &key, std::unordered_map<subkey_mask_t, BoolHyperTrie *> &finished_subtries,
                     PosCalc *pos_calc) {
            // update this node
            this->_leafcount += 1;

            // add it to the finished ( means updated ) nodes.
            finished_subtries[pos_calc->getSubKeyMask()] = this;

            // subtrie has only one position left: insert value
            if (pos_calc->subkey_length == 1) {
                key_part_t key_part = key.at(pos_calc->subkey_to_key_pos(0));

                leaf_edges &leafs = this->getLeafEdges();

                leafs.add(key_part);

            } else { // _depth > 1 -> inner node
                // a child must be set or updated for every subkey_pos available.
                for (const key_pos_t key_pos : pos_calc->getKeyPoss()) {
                    key_part_t key_part = key.at(key_pos);

                    // get pos_calc for next child and check if it was already updated earlier.
                    PosCalc *next_pos_calc = pos_calc->use(key_pos);
                    const auto &finished_child = finished_subtries.find(next_pos_calc->getSubKeyMask());

                    try {
                        // get the child at the current position.
                        BoolHyperTrie *child_ = getInnerChild(pos_calc->key_to_subkey_pos(key_pos), key_part);
                        // the child exists ...
                        // ... and the subtrie starting with the child was not already finished:
                        if (finished_child == finished_subtries.end()) {
                            // call this function for the child
                            child_->set_rek(key, finished_subtries, next_pos_calc);
                        }
                    } catch (...) {
                        // the child does not exist ...
                        // ... and the subtrie starting with the child was already finished:
                        if (finished_child != finished_subtries.end()) {
                            // set the child at this node
                            BoolHyperTrie *child = finished_child->second;
                            this->addChildAsPointer(pos_calc->key_to_subkey_pos(key_pos), key_part, child);

                        } else { // ... and the subtrie starting with the child was not already finished:
                            // set a new child and call this function for the child
                            BoolHyperTrie *new_child = this->insertNewChild(pos_calc->key_to_subkey_pos(key_pos),
                                                                            key_part);
                            new_child->set_rek(key, finished_subtries, next_pos_calc);
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
         * @throws "Key length must match HyperTrie->_depth";
         * @throws "Delete not supported yet."
         */
        void set(const Key_t &key, const bool &value) {
            if (key.size() != this->_depth) {
                throw "Key length must match HyperTrie->_depth";
            }
            // check if there is already another value for this subkey.
            try {
                get(key); // fails if key is not stored
                if (value) {
                    // cache for already created sub HyperTries.
                    std::unordered_map<subkey_mask_t, BoolHyperTrie *> finished_subtries{};

                    // get pos_calc for this.
                    subkey_mask_t subkey_mask(key.size());
                    PosCalc *pos_calc = PosCalc::getInstance(subkey_mask);

                    // remove subkey recursively
                    del_rek(key, finished_subtries, pos_calc);
                    return;
                } else {
                    throw "Delete not supported yet.";
                }
            } catch (...) {
                if (value) {
                    // cache for already created sub HyperTries.
                    std::unordered_map<subkey_mask_t, BoolHyperTrie *> finished_subtries{};

                    // get pos_calc for this.
                    subkey_mask_t subkey_mask(key.size());
                    PosCalc *pos_calc = PosCalc::getInstance(subkey_mask);

                    // store subkey recursively
                    set_rek(key, finished_subtries, pos_calc);
                } else {
                    return;
                }
            }


        }

        void del(const Key_t &coords) {
            throw "Not yet implemented.";
        }

        /**
         * Returns if there are no nonzero elements in the trie.
         * @return true if it is empty
         */
        inline bool empty() const noexcept {
            return _leafcount == 0;
        }

        std::variant<leaf_edges::iterator, inner_edges::KeyView::iterator> begin_(const key_pos_t &key_pos) {
            if (this->_depth == 1) {
                return std::get<leaf_edges>(this->_subtries).begin();
            } else {
                return std::get<std::vector<inner_edges>>(this->_subtries).at(key_pos).begin();
            }
        }

        std::variant<leaf_edges::iterator, inner_edges::KeyView::iterator> end_(const key_pos_t &key_pos) {
            if (this->_depth == 1) {
                return std::get<leaf_edges>(this->_subtries).end();
            } else {
                return std::get<std::vector<inner_edges>>(this->_subtries).at(key_pos).end();
            }
        }

        std::variant<leaf_edges::iterator, inner_edges::KeyView::iterator>
        lower_bound(key_part_t min_key_part = KEY_PART_MIN, key_pos_t key_pos = 0) {
            if (this->_depth == 1) {
                return std::get<leaf_edges>(this->_subtries).lower_bound(min_key_part);
            } else {
                return std::get<std::vector<inner_edges>>(this->_subtries).at(key_pos).lower_bound(min_key_part);
            }
        }

        std::variant<leaf_edges::iterator, inner_edges::KeyView::iterator>
        upper_bound(key_part_t max_key_part = KEY_PART_MAX, key_pos_t key_pos = 0) {
            if (this->_depth == 1) {
                return std::get<leaf_edges>(this->_subtries).upper_bound(max_key_part);
            } else {
                return std::get<std::vector<inner_edges>>(this->_subtries).at(key_pos).upper_bound(max_key_part);
            }
        }

        friend std::ostream &operator<<(std::ostream &out, BoolHyperTrie &trie) {
            out << "<BoolHyperTrie: _depth=" << int(trie._depth) << ", leafcount=" << int(trie._leafcount) << ">";
            return out;
        }

    public:
        class DiagonalView {
        protected:
            const BoolHyperTrie *_trie;

            std::vector<key_pos_t> _positions;
            union {
                leaf_edges *_leafs;
                std::vector<inner_edges> *_edges;
            };
            key_part_t _min;
            key_part_t _max;
            size_t _min_ind;
            size_t _max_ind;
            size_t _size;

            key_pos_t _min_key_pos;

            BoolHyperTrie *_result;


        public:
            DiagonalView(const DiagonalView &diag) :
                    _trie{diag._trie}, _positions{diag._positions}, _min_ind{diag._min_ind},
                    _max_ind{diag._max_ind}, _size{diag._size} {}

            DiagonalView(DiagonalView &diag) :
                    _trie{diag._trie}, _positions{diag._positions}, _min_ind{diag._min_ind},
                    _max_ind{diag._max_ind}, _size{diag._size} {}

            /**
             * Constructor without restricting the range.
             * @param map the VecMap to be viewed.
             */
            DiagonalView(const BoolHyperTrie *trie, const std::vector<key_pos_t> positions) :
                    _trie{trie}, _positions{positions}, _min_ind{0},
                    _max_ind{SIZE_MAX}, _size{_trie->size()} {
                if (trie->depth() == 1) {
                    // trie has depth 1, so positions is assumed to be < 0 >.
                    // handle VecSet instead of VecMap
                    // result is bool

                    _leafs = const_cast<leaf_edges *>(&std::get<leaf_edges>(_trie->_subtries));
                    _min = _leafs->min();
                    _max = _leafs->max();
                    _max_ind = _leafs->size() - 1;

                    first_lower_p = &DiagonalView::first_lower_I;
                    containsAndUpdateLower_p = &DiagonalView::containsAndUpdateLower_I;


                } else if (trie->depth() > positions.size()) {
                    // trie has depth greater than 1, but not all positions are used.
                    // So the result is a trie again.
                    _edges = const_cast<std::vector<inner_edges> *>(&std::get<std::vector<inner_edges>>(
                            _trie->_subtries));
                    size_t min_size = SIZE_MAX;

                    // iterate all key_positions
                    for (size_t i = 0; i < _positions.size(); ++i) {

                        const key_pos_t &key_pos = _positions[i];

                        const inner_edges &children = _edges->at(key_pos);
                        // find greatest min
                        if (const key_part_t &current_min = children.min(); _min < current_min)
                            _min = current_min;
                        // find smallest max
                        if (const key_part_t &current_max = children.max();  current_max < _max)
                            _max = current_max;
                        // find smallest size
                        // and set the _min_key_pos to its key_pos
                        if (const size_t &child_size = children.size(); child_size < min_size) {
                            _min_key_pos = key_pos_t(key_pos);
                            min_size = child_size;
                        }
                    }
                    // calculate the positions of the key to use
                    PosCalc *pos_calc = PosCalc::getInstance(trie->depth())->use(_min_key_pos);
                    std::vector<key_pos_t> _sub_key_poss{};
                    _sub_key_poss.resize(pos_calc->subkey_length);
                    for (const key_pos_t &pos : positions) {
                        if (pos != _min_key_pos) {
                            _sub_key_poss.emplace_back(pos_calc->key_to_subkey_pos(pos));
                        }
                    }
                    _positions = _sub_key_poss;

                    first_lower_p = &DiagonalView::first_lower_II;
                    containsAndUpdateLower_p = &DiagonalView::containsAndUpdateLower_II;

                } else {
                    // trie has depth greater than 1 and uses all positions.
                    // So the result is bool
                    _edges = const_cast<std::vector<inner_edges> *>(&std::get<std::vector<inner_edges>>(
                            _trie->_subtries));
                    size_t min_size = SIZE_MAX;

                    // iterate all key_positions
                    for (size_t key_pos = 0; key_pos < _edges->size(); ++key_pos) {

                        const inner_edges &children = _edges->at(key_pos);
                        // find greatest min
                        if (const key_part_t &current_min = children.min(); _min < current_min)
                            _min = current_min;
                        // find smallest max
                        if (const key_part_t &current_max = children.max();  current_max < _max)
                            _max = current_max;
                        // find smallest size
                        // and set the _min_key_pos to its key_pos
                        if (const size_t &child_size = children.size(); child_size < min_size) {
                            _min_key_pos = key_pos_t(key_pos);
                            min_size = child_size;
                        }
                    }

                    first_lower_p = &DiagonalView::first_lower_III;
                    containsAndUpdateLower_p = &DiagonalView::containsAndUpdateLower_III;
                }

                if (not positions.size()) {
                    throw "DiagonalView must have at least one position.";
                }


            }

            inline key_part_t lower() const {
                return _min;
            }

            inline key_part_t upper() const {
                return _max;
            }

            inline const size_t &size() const {
                return _size;
            }

            /*
             */
        private:
            key_part_t first_lower_I(const key_part_t &key_part) {
                size_t ind = (key_part == _leafs->_keys.at(_min_ind))
                             ? _min_ind
                             : tnt::util::container::search(_leafs->_keys, key_part, _min_ind, _max_ind);
                _min_ind = ind;
                if (ind != tnt::util::container::NOT_FOUND) {
                    _size = _max_ind - ind + 1;
                    _min = _leafs->byInd(ind);
                    return _min;
                } else {
                    _size = 0;
                    return KEY_PART_MAX;
                }
            }

            key_part_t first_lower_II(const key_part_t &key_part) {
                inner_edges &children = _edges->at(_min_key_pos);
                const std::vector<key_part_t> &childrens_keys = children.keys();
                const std::vector<BoolHyperTrie *> &childrens_values = children.values();

                key_part_t current_key_part = key_part;

                size_t ind = (current_key_part == childrens_keys.at(_min_ind))
                             ? _min_ind
                             : tnt::util::container::insert_pos(childrens_keys, key_part, _min_ind, _max_ind);
                while (ind != _max_ind + 1) {

                    current_key_part = childrens_keys.at(ind);
                    const BoolHyperTrie *child = childrens_values.at(ind);
                    try {
                        // TODO: implement diagonal key retrieval in HyperTrie directly.
                        std::vector<std::optional<key_part_t>> key(_positions.size() - 1);
                        for (size_t pos: _positions) {
                            key[pos] = current_key_part;
                        }
                        _result = std::get<BoolHyperTrie *>(child->get(key));
                        _min_ind = ind;
                        _size = _max_ind - ind + 1;
                        _min = current_key_part;
                        return current_key_part;
                    } catch (...) {}
                    ++ind;
                }
                _size = 0;
                return KEY_PART_MAX;


            }

            key_part_t first_lower_III(const key_part_t &key_part) {
                inner_edges &children = _edges->at(_min_key_pos);
                const std::vector<key_part_t> &childrens_keys = children.keys();
                const std::vector<BoolHyperTrie *> &childrens_values = children.values();

                key_part_t current_key_part = key_part;

                size_t ind = (current_key_part == childrens_keys.at(_min_ind))
                             ? _min_ind
                             : tnt::util::container::insert_pos(childrens_keys, key_part, _min_ind, _max_ind);

                while (ind != _max_ind + 1) {

                    current_key_part = childrens_keys.at(ind);
                    const BoolHyperTrie *child = childrens_values.at(ind);
                    try {
                        // TODO: implement diagonal key retrieval in HyperTrie directly.
                        child->get(Key_t(_positions.size() - 1, current_key_part));
                        _min_ind = ind;
                        _size = _max_ind - ind + 1;
                        _min = current_key_part;
                        return current_key_part;
                    } catch (...) {}

                    ++ind;
                }
                _size = 0;
                return KEY_PART_MAX;
            }

            key_part_t (DiagonalView::*first_lower_p)(const key_part_t &);

        public:
            /**
             * Finds the next valid key_part that is greater or equal to the given min_. this->min() is updated alongside..
             * @param min_
             * @return the new minimal key_part
             */
            inline key_part_t first(const key_part_t &min_) {
                return (this->*first_lower_p)(min_);
            }

            /*
    */
        public:
            /**
             * Finds the next valid key_part that is greater or equal to this->min(). this->min() is updated alongside..
             * @return the new minimal key_part
             */
            inline key_part_t first() {
                return (this->*first_lower_p)(_min_ind);
            }

            /*
             */

        public:
            /**
             * Returns the HyperTrie that corresponds to the last call of first() retrieved. This function
             * must only be called if before either first(...) was called returning a key_part in range or
             * if  containsAndUpdateLower() was called returning true.
             * @return the child HyperTrie
             */
            inline BoolHyperTrie *minValue() {
                // todo: think about if it is sufficient to return BoolHyperTries.
                return _result;
            }

            /*
             */
        private:
            bool containsAndUpdateLower_I(const key_part_t &key_part) {
                size_t ind = tnt::util::container::insert_pos(_leafs->_keys, key_part, _min_ind, _max_ind);
                if (ind != _max_ind + 1) {
                    _min = _leafs->byInd(ind);
                    _min_ind = ind;
                    _size = _max_ind - ind + 1;
                    return _min == key_part;
                } else {
                    _min = KEY_PART_MAX;
                    _size = 0;
                    return false;
                }
            }

            bool containsAndUpdateLower_II(const key_part_t &key_part) {
                inner_edges &children = _edges->at(_min_key_pos);
                const std::vector<key_part_t> &childrens_keys = children.keys();
                const std::vector<BoolHyperTrie *> &childrens_values = children.values();

                key_part_t current_key_part = key_part;

                size_t ind = (current_key_part == childrens_keys.at(_min_ind))
                             ? _min_ind
                             : tnt::util::container::insert_pos(childrens_keys, key_part, _min_ind, _max_ind);


                if (ind != _max_ind + 1) {
                    current_key_part = childrens_keys.at(ind);
                    if (current_key_part == key_part) {
                        const BoolHyperTrie *child = childrens_values.at(ind);
                        try {
                            // TODO: implement diagonal key retrieval in HyperTrie directly.
                            std::vector<std::optional<key_part_t>> key(_positions.size() - 1);
                            for (size_t pos: _positions) {
                                key[pos] = current_key_part;
                            }
                            _result = std::get<BoolHyperTrie *>(child->get(key));
                            _min_ind = ind;
                            _size = _max_ind - ind + 1;
                            _min = current_key_part;
                            return true;
                        } catch (...) {}
                    }
                }
                if (ind != _max_ind + 1) {

                    _min_ind = ind + 1;
                    _size = _max_ind - _min_ind + 1;
                    _min = childrens_keys.at(ind);
                } else {
                    _size = 0;
                    return KEY_PART_MAX;
                }
                return false;
            }

            bool containsAndUpdateLower_III(const key_part_t &key_part) {
                inner_edges &children = _edges->at(_min_key_pos);
                const std::vector<key_part_t> &childrens_keys = children.keys();
                const std::vector<BoolHyperTrie *> &childrens_values = children.values();

                key_part_t current_key_part = key_part;

                size_t ind = (current_key_part == childrens_keys.at(_min_ind))
                             ? _min_ind
                             : tnt::util::container::insert_pos(childrens_keys, key_part, _min_ind, _max_ind);


                if (ind != _max_ind + 1) {
                    current_key_part = childrens_keys.at(ind);
                    if (current_key_part == key_part) {
                        const BoolHyperTrie *child = childrens_values.at(ind);
                        try {
                            // TODO: implement diagonal key retrieval in HyperTrie directly.
                            child->get(Key_t(_positions.size() - 1, current_key_part));
                            _min_ind = ind;
                            _size = _max_ind - ind + 1;
                            _min = current_key_part;
                            return true;
                        } catch (...) {}
                    }
                }
                if (ind != _max_ind + 1) {

                    _min_ind = ind + 1;
                    _size = _max_ind - _min_ind + 1;
                    _min = childrens_keys.at(ind);
                } else {
                    _size = 0;
                    _min = _max;
                }
                return false;
            }


            bool (DiagonalView::*containsAndUpdateLower_p)(const key_part_t &);

        public:
            inline bool containsAndUpdateLower(const key_part_t &key_part) {
                return (this->*containsAndUpdateLower_p)(key_part);
            }



            /*
             */

        public:
            inline key_part_t incrementMin() {
                ++_min_ind;
                if (_min_ind <= _max_ind) {
                    return (this->*first_lower_p)(_min_ind);
                } else {
                    return _min = KEY_PART_MAX;
                }
            }

        private:
            void setMinMax_III(key_part_t &min, key_part_t &max) {
                const inner_edges &children = _edges->at(_min_key_pos);
                // calc min
                if (children.keyByInd(_min_ind) != min) {
                    _min_ind = std::get<1>(children.containsAndInd(min, _min_ind, _max_ind));;
                    _min = children.keyByInd(_min_ind);
                    min = _min;
                }
                // calc max
                if (children.keyByInd(_max_ind) != max) {
                    _max_ind = std::get<1>(children.containsAndIndLower(max, _min_ind, _max_ind));
                    _max = children.keyByInd(_max_ind);
                    max = _max;
                }
                if (min <= max) {
                    _size = max - min;
                } else {
                    _size = 0;
                }
            }


            void (DiagonalView::*setMinMax_p)(key_part_t &, key_part_t &);

        protected:
            inline void setMinMax(key_part_t &min, key_part_t &max) {
                (this->*setMinMax_p)(min, max);
            }

            /*
             *
             */
        public:
            /**
             * reduces the lower and upper bounds of the DiagonalViews a bit by talking the largest min and smallest max of all.
             * It is not safe that afterwards all mins/max' are equal.
             * @param diagonals the diagonals where the range should be minimized to a common range.
             * @return tuple of the largest min and smallest max.
             */
            static std::tuple<size_t, size_t> minimizeRange(std::vector<DiagonalView> diagonals) {
                key_part_t min_ = 0;
                key_part_t max_ = SIZE_MAX;
                // get min and max
                for (size_t i = 0; i < diagonals.size(); ++i) {
                    const DiagonalView &diag = diagonals[i];
                    if (const key_part_t &current_min = diag.lower(); min_ < current_min)
                        min_ = current_min;
                    if (const key_part_t &current_max = diag.upper();  current_max < max_)
                        max_ = current_max;
                }
                // return if min > max, i.e. there are no key_part candidates left no more.
                if (min_ > max_) {
                    return std::make_tuple(min_, max_);
                }
                // narrow down min and max
                auto temp_min = min_;
                auto temp_max = max_;
                for (int times = 0; times < 2; ++times) { // do it twice
                    for (size_t i = 0; i < diagonals.size(); ++i) {
                        DiagonalView &diag = diagonals[i];
                        diag.setMinMax(min_, max_);
                        if (diag._size == 0)
                            return std::make_tuple(min_, max_);
                    }
                    if ((temp_min == min_) and (temp_max == max_))
                        break;
                }
                return std::make_tuple(min_, max_);
            }
        };
    };
};

#endif //SPARSETENSOR_HYPERTRIE_BOOLHYPERTRIE_HPP


