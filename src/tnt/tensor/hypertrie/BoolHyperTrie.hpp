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

namespace {
    using namespace tnt::util::types;
}

namespace tnt::tensor::hypertrie {
    class BoolHyperTrie;

    using Operands =  typename std::vector<BoolHyperTrie *>;

    union Children {
    private:
        using inner_edges = tnt::util::container::VecMap<key_part_t, BoolHyperTrie *>;
        using leaf_edges = tnt::util::container::VecSet<key_part_t>;
    public:
        std::vector<inner_edges> _inner_edges;
        leaf_edges _leaf_edges;

        ~Children() {};
    };

    class BoolHyperTrie {
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
        Children children;


    public:
        /**
         * The default constructor creates a empty BoolHyperTrie with depth = 1.
         */
        BoolHyperTrie() : _depth(1), children{._leaf_edges = leaf_edges{}} {}

        /**
         * Creates a empty BoolHyperTrie with given depth.
         * @param depth depth of the BoolHyperTrie
         */
        explicit BoolHyperTrie(key_pos_t depth) : _depth(depth),
                                                  children{(depth > 1)
                                                           ? Children{._inner_edges =std::vector<inner_edges>(depth)}
                                                           : Children{._leaf_edges =leaf_edges{}}} {}

        BoolHyperTrie(std::initializer_list<Key_t> keys)
                : BoolHyperTrie(key_pos_t((keys.size() > 0) ? keys.begin()->size() : 0)) {
            for (const auto &key : keys) {
                assert(key.size() == _depth);
                set(key, true);
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
         * @throws std::out_of_range if not 1 <= subkey_pos <= this->depth()
         */
        inline inner_edges &getInnerEdges(const key_pos_t &subkey_pos) {
            return children._inner_edges.at(subkey_pos);
        }

    public:
        inline const inner_edges &getInnerEdges(const key_pos_t &subkey_pos) const {
            return children._inner_edges.at(subkey_pos);
        }

    private:
        /**
         * Returns leaf edges.
         * @throws std::exception::bad_variant_access if the depth of this BoolHyperTrie is not 1
         * @return a set of leaf edges.
         */
        inline leaf_edges &getLeafEdges() {
            return children._leaf_edges;
        }

    public:
        inline const leaf_edges &getLeafEdges() const {
            return children._leaf_edges;
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
         * @throws std::out_of_range if not 1 <= subkey_pos <= this->depth()
         */
        inline BoolHyperTrie *createNewChild(const key_pos_t &key_pos, const key_part_t &key_part) {
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
         * @return the child or an nullpoint if it doesn't exist
         * @throws std::out_of_range if not 1 <= subkey_pos <= this->depth() or the key doesn't exist
         */
        BoolHyperTrie *getInnerChild(const key_pos_t &key_pos, const key_part_t &key_part) {
            inner_edges &edges = getInnerEdges(key_pos);
            // find child
            try {
                return edges.at(key_part);

            } catch (std::out_of_range ex) {
                return nullptr;
            }
        }

        /**
         * Checks weather a value for key is set. It removes thereby entries from the key.
         * @param key
         * @return
         */
        inline bool get_internal(Key_t key) const {
            // copy the key to a set
            assert(not key.empty());
            BoolHyperTrie *current_subtrie = const_cast<BoolHyperTrie *>(this);
            while (not key.empty()) {
                if (current_subtrie->_depth > 1) {
                    key_pos_t min_card_key_pos = current_subtrie->getMinCardKeyPos();
                    current_subtrie = current_subtrie->getInnerChild(min_card_key_pos, key.at(min_card_key_pos));
                    if (current_subtrie == nullptr)
                        return false;
                    key.erase(key.begin() + min_card_key_pos);
                } else {
                    return current_subtrie->getLeafEdges().contains(key.at(0));
                }
            }
        }

    public:

        /**
         * Get an value or sub BoolHyperTrie by a key.
         * @param key key to the requested part
         * @return a sub BoolHyperTrie or a value depending on the length of the key.
         * @throws the entry doesn't exist
         */
        inline bool get(const Key_t &key) const {
            assert(key.size() == this->depth());
            // copy the key to a set
            std::vector<key_part_t> intern_key{key};
            return get_internal(intern_key);
        }

        /**
         * Retrieves a entry or sub-hypertrie by setting all key positions provided in key_positions to the value
         * key_part. All other key_parts are variable.
         * @param key_positions positions that have a key part
         * @param key_part the key part
         * @return the subtrie for the given key or a nullptr
         */
        inline BoolHyperTrie *
        getDiagonal(const std::vector<key_pos_t> &key_positions, const key_part_t &key_part) const {
            assert(key_positions.size() < this->depth());
            BoolHyperTrie *current_subtrie = const_cast<BoolHyperTrie *>(this);

            std::set<key_pos_t> key_poss(key_positions.begin(), key_positions.end());
            PosCalc *pos_calc = PosCalc::getInstance(_depth);
            while (not key_poss.empty()) {
                key_pos_t key_pos = extractPosOfMinCardKeyPos(key_poss, pos_calc);
                current_subtrie = current_subtrie->getInnerEdges(pos_calc->key_to_subkey_pos(key_pos)).at(key_part);
                if (current_subtrie == nullptr)
                    return nullptr;

                pos_calc = pos_calc->use(key_pos);
            }
            return current_subtrie;
        }

        bool getFullDiagonal(const key_part_t &key_part) const {
            // copy the key to a vector
            std::vector<key_part_t> intern_key(this->depth(), key_part);
            return get_internal(intern_key);
        }

    private:
        /**
         * Retrieves for a set of key parts at their positions (given in a map: position -> key_part) the stored
         * sub-hypertrie.
         * @param non_slice_key_parts
         * @return pointer to the subhypertrie or nullptr if it doesn't exist
         */
        inline BoolHyperTrie *getSlice_internal(std::map<key_pos_t, key_part_t> &non_slice_key_parts) const {
            assert(non_slice_key_parts.size() < this->size());
            // get child while there are non slice parts in the subkey.
            BoolHyperTrie *current_subtrie = const_cast<BoolHyperTrie *>(this);
            PosCalc *pos_calc = PosCalc::getInstance(this->_depth);

            while (not non_slice_key_parts.empty()) {
                key_pos_t min_card_key_pos = current_subtrie->getMinCardKeyPos(non_slice_key_parts, pos_calc);
                key_pos_t min_card_subkey_pos = pos_calc->key_to_subkey_pos(min_card_key_pos);

                current_subtrie = current_subtrie->getInnerChild(min_card_subkey_pos,
                                                                 non_slice_key_parts.at(min_card_key_pos));
                if (current_subtrie == nullptr)
                    return nullptr;

                non_slice_key_parts.erase(min_card_key_pos);
                pos_calc = pos_calc->use(min_card_key_pos);
            }
            return current_subtrie;
        }

    public:
        inline BoolHyperTrie *getSlice(const SliceKey_t &key) const {
            assert(std::count(key.begin(), key.end(), std::nullopt) != 0);

            // extract non_slice_key_parts
            std::map<key_pos_t, key_part_t> non_slice_key_parts{};
            for (key_pos_t key_pos : range(key_pos_t(key.size())))
                if (key.at(key_pos).has_value())
                    non_slice_key_parts[key_pos] = *key.at(key_pos);

            // get child while there are non slice parts in the subkey.
            BoolHyperTrie *current_subtrie = const_cast<BoolHyperTrie *>(this);
            PosCalc *pos_calc = PosCalc::getInstance(this->_depth);

            while (not non_slice_key_parts.empty()) {
                key_pos_t min_card_key_pos = current_subtrie->getMinCardKeyPos(non_slice_key_parts, pos_calc);
                key_pos_t min_card_subkey_pos = pos_calc->key_to_subkey_pos(min_card_key_pos);

                current_subtrie = current_subtrie->getInnerChild(min_card_subkey_pos,
                                                                 non_slice_key_parts.at(min_card_key_pos));
                if (current_subtrie == nullptr)
                    return nullptr;

                non_slice_key_parts.erase(min_card_key_pos);
                pos_calc = pos_calc->use(min_card_key_pos);
            }
            return current_subtrie;
        }

        /**
         * Get an value or sub BoolHyperTrie by a key.
         * @param key Vector of optional key_parts. If a key_pos is an std::nullopt the key_part for that position is
         * not set resulting in a slice.
         * @return a sub BoolHyperTrie or a value depending if the key contains slices.
         */
        inline std::variant<BoolHyperTrie *, bool> get(const SliceKey_t &key) const {
            const auto slice_count = std::count(key.begin(), key.end(), std::nullopt);

            if (slice_count == 0) {
                Key_t key_internal(this->depth());
                for (const auto &[pos, key_part] : enumerate(key))
                    key_internal[pos] = *key.at(pos);
                return {get_internal(key_internal)};
            } else {
                return {getSlice(key)};
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
                    return children._leaf_edges.min();
                } else {
                    return children._inner_edges.at(key_pos).min();
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
                    return children._leaf_edges.max();
                } else {
                    return children._inner_edges.at(key_pos).min();
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
                return children._leaf_edges.size();
            } else {
                return children._inner_edges.at(key_pos).size();
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

        template<typename T>
        key_pos_t extractPosOfMinCardKeyPos(T &key_poss, const PosCalc *posCalc) const {
            size_t min_card = SIZE_MAX;
            auto min_key_pos_ = key_poss.begin();
            auto min_key_pos = *min_key_pos_;
            for (auto key_pos_ = min_key_pos_; key_pos_ != key_poss.end(); ++key_pos_) {
                size_t card = getCard(posCalc->key_to_subkey_pos(*key_pos_));
                if (card < min_card) {
                    min_card = card;
                    min_key_pos_ = key_pos_;
                    min_key_pos = *key_pos_;
                }
            }
            key_poss.erase(min_key_pos_);
            return min_key_pos;
        }


        template<typename T>
        key_pos_t getMinCardKeyPos(const T &map_from_key_pos,
                                   const PosCalc *posCalc) const {
            size_t min_card = SIZE_MAX;
            key_pos_t min_card_key_pos = 0;
            for (const key_pos_t &key_pos : map_from_key_pos) {
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
        template<typename V>
        key_pos_t getMinCardKeyPos(const std::map<key_pos_t, V> &map_from_key_pos,
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


                    // get the child at the current position.

                    if (BoolHyperTrie *child_ = getInnerChild(pos_calc->key_to_subkey_pos(key_pos), key_part);
                            child_ != nullptr) {
                        // the child exists ...
                        // ... and the subtrie starting with the child was not already finished:
                        if (finished_child == finished_subtries.end()) {
                            // call this function for the child
                            child_->set_rek(key, finished_subtries, next_pos_calc);
                        }
                    } else {
                        // the child does not exist ...
                        // ... and the subtrie starting with the child was already finished:
                        if (finished_child != finished_subtries.end()) {
                            // set the child at this node
                            BoolHyperTrie *child = finished_child->second;
                            this->addChildAsPointer(pos_calc->key_to_subkey_pos(key_pos), key_part, child);

                        } else { // ... and the subtrie starting with the child was not already finished:
                            // set a new child and call this function for the child
                            BoolHyperTrie *new_child = this->createNewChild(pos_calc->key_to_subkey_pos(key_pos),
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
            if (get(key)) {
                if (not value) {
                    // cache for already created sub HyperTries.
                    std::unordered_map<subkey_mask_t, BoolHyperTrie *> finished_subtries{};

                    // get pos_calc for this.
                    subkey_mask_t subkey_mask(key.size());
                    PosCalc *pos_calc = PosCalc::getInstance(subkey_mask);

                    // remove subkey recursively
                    del_rek(key, finished_subtries, pos_calc);
                    return;
                }
            } else {
                if (value) {
                    // cache for already created sub HyperTries.
                    std::unordered_map<subkey_mask_t, BoolHyperTrie *> finished_subtries{};

                    // get pos_calc for this.
                    subkey_mask_t subkey_mask(key.size());
                    PosCalc *pos_calc = PosCalc::getInstance(subkey_mask);

                    // store subkey recursively
                    set_rek(key, finished_subtries, pos_calc);
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

        // TODO: do not test as it is currently not used
        std::variant<leaf_edges::iterator, inner_edges::KeyView::iterator> begin_(const key_pos_t &key_pos) {
            if (this->_depth == 1) {
                return children._leaf_edges.begin();
            } else {
                return children._inner_edges.at(key_pos).begin();
            }
        }

        // TODO: do not test as it is currently not used
        std::variant<leaf_edges::iterator, inner_edges::KeyView::iterator> end_(const key_pos_t &key_pos) {
            if (this->_depth == 1) {
                return children._leaf_edges.end();
            } else {
                return children._inner_edges.at(key_pos).end();
            }
        }

        // TODO: do not test as it is currently not used
        std::variant<leaf_edges::iterator, inner_edges::KeyView::iterator>
        lower_bound(key_part_t min_key_part = KEY_PART_MIN, key_pos_t key_pos = 0) {
            if (this->_depth == 1) {
                return children._leaf_edges.lower_bound(min_key_part);
            } else {
                return children._inner_edges.at(key_pos).lower_bound(min_key_part);
            }
        }

        // TODO: do not test as it is currently not used
        std::variant<leaf_edges::iterator, inner_edges::KeyView::iterator>
        upper_bound(key_part_t max_key_part = KEY_PART_MAX, key_pos_t key_pos = 0) {
            if (this->_depth == 1) {
                return children._leaf_edges.upper_bound(max_key_part);
            } else {
                return children._inner_edges.at(key_pos).upper_bound(max_key_part);
            }
        }

        friend std::ostream &operator<<(std::ostream &out, BoolHyperTrie &trie) {
            out << "<BoolHyperTrie: _depth=" << int(trie._depth) << ", leafcount=" << int(trie._leafcount) << ">";
            return out;
        }

    public:
        class DiagonalView {

            std::vector<key_pos_t> _positions;

            key_part_t _min = 0;
            key_part_t _max = KEY_PART_MAX;
            size_t _min_ind;
            size_t _max_ind;
            size_t _size;

            union {
                inner_edges *_inner_edges;
                leaf_edges *_leaf_edges;
            };

            BoolHyperTrie *_result;

        public:
            friend std::ostream &operator<<(std::ostream &os, const DiagonalView &view) {
                os << "_positions: " << view._positions << " _min: " << view._min << " _max: " << view._max
                   << " _min_ind: " << view._min_ind << " _max_ind: " << view._max_ind << " _size: " << view._size;
                return os;
            }

        public:
            /**
             * Constructor without restricting the range.
             * @param map the VecMap to be viewed.
             */
            DiagonalView(const BoolHyperTrie *trie, const std::vector<key_pos_t> &positions) :
                    _positions{positions}, _min_ind{0},
                    _max_ind{SIZE_MAX}, _size{trie->size()} {
                if (trie->depth() == 1) {
                    // trie has depth 1, so positions is assumed to be < 0 >.
                    // handle VecSet instead of VecMap
                    // result is bool
                    _leaf_edges = const_cast<leaf_edges *>(&trie->getLeafEdges());

                    _min = _leaf_edges->min();
                    _max = _leaf_edges->max();
                    _max_ind = _leaf_edges->size() - 1;

                    setMinMax_p = &setMinMax_I;
                    first_lower_p = &first_from_lower_bound_I;
                    increment_current_until_hit_p = &increment_current_until_hit_I;
                    containsAndUpdateLower_p = &containsAndUpdateLower_I;


                } else if (trie->depth() > positions.size()) {
                    // trie has depth greater than 1, but not all positions are used.
                    // So the result is a trie again.
                    size_t min_size = SIZE_MAX;

                    key_pos_t min_key_pos = 0;

                    // iterate all key_positions
                    for (const key_pos_t &key_pos : _positions) {

                        const inner_edges &children = trie->getInnerEdges(key_pos);
                        // find greatest min
                        if (const key_part_t &current_min = children.min(); _min < current_min)
                            _min = current_min;
                        // find smallest max
                        if (const key_part_t &current_max = children.max();  current_max < _max)
                            _max = current_max;
                        // find smallest size
                        // and set the _min_key_pos to its key_pos
                        if (const size_t &child_size = children.size(); child_size < min_size) {
                            min_key_pos = key_pos_t(key_pos);
                            min_size = child_size;
                        }
                    }
                    _inner_edges = const_cast<inner_edges *>(&trie->getInnerEdges(min_key_pos));
                    _max_ind = _inner_edges->size() - 1;

                    // calculate the positions of the key to use
                    PosCalc *pos_calc = PosCalc::getInstance(trie->depth())->use(min_key_pos);
                    std::vector<key_pos_t> sub_key_poss{};
                    for (const key_pos_t &pos : positions) {
                        if (pos != min_key_pos) {
                            sub_key_poss.emplace_back(pos_calc->key_to_subkey_pos(pos));
                        }
                    }
                    _positions = sub_key_poss;

                    setMinMax_p = &setMinMax_III;
                    first_lower_p = &first_from_lower_bound_II;
                    increment_current_until_hit_p = &increment_current_until_hit_II;
                    containsAndUpdateLower_p = &containsAndUpdateLower_II;

                } else {
                    // trie has depth greater than 1 and uses all positions.
                    // So the result is bool
                    size_t min_size = SIZE_MAX;

                    key_pos_t min_key_pos = 0;

                    // iterate all key_positions
                    for (const key_pos_t &key_pos : range(trie->depth())) {

                        const inner_edges &children = trie->getInnerEdges(key_pos);
                        // find greatest min
                        if (const key_part_t &current_min = children.min(); _min < current_min)
                            _min = current_min;
                        // find smallest max
                        if (const key_part_t &current_max = children.max();  current_max < _max)
                            _max = current_max;
                        // find smallest size
                        // and set the _min_key_pos to its key_pos
                        if (const size_t &child_size = children.size(); child_size < min_size) {
                            min_key_pos = key_pos;
                            min_size = child_size;
                        }
                    }
                    _inner_edges = const_cast<inner_edges *>(&trie->getInnerEdges(min_key_pos));
                    _max_ind = _inner_edges->size() - 1;

                    setMinMax_p = &setMinMax_III;
                    first_lower_p = &first_from_lower_bound_III;
                    increment_current_until_hit_p = &increment_current_until_hit_III;
                    containsAndUpdateLower_p = &containsAndUpdateLower_III;
                }

                if (not positions.size()) {
                    throw "DiagonalView must have at least one position.";
                }


            }

            /**
             * Minimal key in range.
             * @return
             */
            inline key_part_t lower() const {
                return _min;
            }

            /**
             * Maximum key in range.
             * @return
             */
            inline key_part_t upper() const {
                return _max;
            }

            /**
             * Size of the range.
             * @return
             */
            inline const size_t &size() const {
                return _size;
            }

            void updateSize() {
                if(_min_ind <= _max_ind)
                    _size = _max_ind - _min_ind + 1;
                else
                    _size = 0;
            }

            /*
             */
        private:
            static key_part_t first_from_lower_bound_I(DiagonalView &view, const key_part_t &lower) {
                if (lower != view._leaf_edges->keys().at(view._min_ind)) {
                    view._min_ind = tnt::util::container::insert_pos(view._leaf_edges->keys(), view._min_ind,
                                                                     view._max_ind + 1, lower);
                    return increment_current_until_hit_I(view);

                } else {
                    view._min = lower;
                    return lower;
                }
            }

            static key_part_t increment_current_until_hit_I(DiagonalView &view) {
                if (view._min_ind != view._max_ind + 1) {
                    view._min = view._leaf_edges->keyByInd(view._min_ind);
                    view.updateSize();
                    return view._min;
                } else {
                    view._size = 0;
                    return KEY_PART_MAX;
                }
            }

            static key_part_t first_from_lower_bound_II(DiagonalView &view, const key_part_t &lower) {
                const std::vector<key_part_t> &children_keys = view._inner_edges->keys();

                if (lower != children_keys.at(view._min_ind)) {
                    view._min_ind = tnt::util::container::insert_pos(children_keys, view._min_ind,
                                                                     view._max_ind + 1, lower);
                }
                return increment_current_until_hit_II(view);
            }

            static key_part_t
            increment_current_until_hit_II(DiagonalView &view) {
                const std::vector<key_part_t> &children_keys = view._inner_edges->keys();
                const std::vector<BoolHyperTrie *> &children_values = view._inner_edges->values();
                key_part_t current_key_part;
                while (view._min_ind != view._max_ind + 1) {

                    current_key_part = children_keys.at(view._min_ind);
                    const BoolHyperTrie *child = children_values.at(view._min_ind);
                    view._result = child->getDiagonal(view._positions, current_key_part);
                    if (view._result != nullptr) {
                        view._min_ind = view._min_ind;
                        view._size = view._max_ind - view._min_ind + 1;
                        view._min = current_key_part;
                        return current_key_part;
                    } else
                        ++view._min_ind;
                }
                view._size = 0;
                return KEY_PART_MAX;
            }

            static key_part_t first_from_lower_bound_III(DiagonalView &view, const key_part_t &lower) {
                const std::vector<key_part_t> &childrens_keys = view._inner_edges->keys();

                if (lower != childrens_keys.at(view._min_ind)) {
                    view._min_ind = tnt::util::container::insert_pos(childrens_keys, view._min_ind,
                                                                     view._max_ind + 1, lower);
                }
                return increment_current_until_hit_III(view);
            }

            static key_part_t increment_current_until_hit_III(DiagonalView &view) {
                const std::vector<key_part_t> &childrens_keys = view._inner_edges->keys();
                const std::vector<BoolHyperTrie *> &childrens_values = view._inner_edges->values();

                key_part_t current_key_part;
                while (view._min_ind != view._max_ind + 1) {

                    current_key_part = childrens_keys.at(view._min_ind);
                    const BoolHyperTrie *child = childrens_values.at(view._min_ind);
                    if (child->getFullDiagonal(current_key_part)) {
                        view._size = view._max_ind - view._min_ind + 1;
                        view._min = current_key_part;
                        return current_key_part;
                    } else
                        ++view._min_ind;
                }
                view._size = 0;
                return KEY_PART_MAX;
            }

            key_part_t (*first_lower_p)(DiagonalView &view, const key_part_t &);

            key_part_t (*increment_current_until_hit_p)(DiagonalView &view);

        public:
            /**
             * Finds the next valid key_part that is greater or equal to the given min_. this->min() is updated alongside..
             * @param min_
             * @return the new minimal key_part
             */
            inline key_part_t first(const key_part_t &min_) {
                return (this->first_lower_p)(*this, min_);
            }

            /*
            */
        public:
            /**
             * Finds the next valid key_part that is greater or equal to this->min(). this->min() is updated alongside..
             * @return the new minimal key_part
             */
            inline key_part_t first() {
                return (this->first_lower_p)(*this, _min);
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
                return _result;
            }

            /*
             */
        private:
            static bool containsAndUpdateLower_I(DiagonalView &view, const key_part_t &key_part) {
                size_t ind = tnt::util::container::insert_pos(view._leaf_edges->keys(), view._min_ind,
                                                              view._max_ind + 1, key_part);
                if (ind != view._max_ind + 1) {
                    view._min = view._leaf_edges->keyByInd(ind);

                    view._min_ind = ind;
                    view.updateSize();
                    return (view._min == key_part);
                } else {
                    view._size = 0;
                    view._min = KEY_PART_MAX;
                    return false;
                }
            }

            static bool containsAndUpdateLower_II(DiagonalView &view, const key_part_t &key_part) {
                const std::vector<key_part_t> &childrens_keys = view._inner_edges->keys();
                const std::vector<BoolHyperTrie *> &childrens_values = view._inner_edges->values();

                key_part_t current_key_part = key_part;

                size_t ind = (current_key_part == childrens_keys.at(view._min_ind))
                             ? view._min_ind
                             : tnt::util::container::insert_pos(childrens_keys, view._min_ind, view._max_ind + 1,
                                                                key_part);


                if (ind != view._max_ind + 1) {
                    current_key_part = childrens_keys.at(ind);
                    if (current_key_part == key_part) {
                        const BoolHyperTrie *child = childrens_values.at(ind);
                        view._result = child->getDiagonal(view._positions, current_key_part);
                        if (view._result != nullptr) {
                            view._min_ind = ind;
                            view.updateSize();
                            view._min = current_key_part;
                            return true;
                        } else{
                            view._min_ind = ind + 1;
                            view.updateSize();
                            return false;
                        }
                    } else{
                        view._min_ind = ind;
                        view._min = current_key_part;
                        view.updateSize();
                        return false;
                    }
                } else {
                    view._size = 0;
                    view._min = KEY_PART_MAX;
                    return false;
                }
            }

            static bool containsAndUpdateLower_III(DiagonalView &view, const key_part_t &key_part) {
                const std::vector<key_part_t> &childrens_keys = view._inner_edges->keys();
                const std::vector<BoolHyperTrie *> &childrens_values = view._inner_edges->values();

                key_part_t current_key_part = key_part;

                size_t ind = (current_key_part == childrens_keys.at(view._min_ind))
                             ? view._min_ind
                             : tnt::util::container::insert_pos(childrens_keys, view._min_ind, view._max_ind + 1,
                                                                key_part);


                if (ind != view._max_ind + 1) {
                    current_key_part = childrens_keys.at(ind);
                    if (current_key_part == key_part) {
                        const BoolHyperTrie *child = childrens_values.at(ind);
                        if (child->getFullDiagonal(current_key_part)) {
                            view._min_ind = ind;
                            view.updateSize();
                            view._min = current_key_part;
                            return true;
                        } else{
                            view._min_ind = ind + 1;
                            view._min = childrens_keys.at(view._min_ind);
                            view.updateSize();
                            return false;
                        }
                    } else{
                        view._min_ind = ind;
                        view._min = current_key_part;
                        view.updateSize();
                        return false;
                    }
                } else {
                    view._size = 0;
                    view._min = KEY_PART_MAX;
                    return false;
                }
            }


            bool (*containsAndUpdateLower_p)(DiagonalView &view, const key_part_t &);

        public:
            inline bool containsAndUpdateLower(const key_part_t &key_part) {
                return (this->containsAndUpdateLower_p)(*this, key_part);
            }



            /*
             */

        public:
            inline key_part_t incrementMin() {
                ++_min_ind;
                return (this->increment_current_until_hit_p)(*this);
            }

        private:

            static void setMinMax_I(DiagonalView &view, key_part_t &min, key_part_t &max) {
                // calc min
                if (view._leaf_edges->keyByInd(view._min_ind) != min) {
                    view._min_ind = std::get<1>(
                            view._leaf_edges->containsAndInd(min, view._min_ind, view._max_ind + 1));;
                    if (view._min_ind > view._max_ind){
                        view._size = 0;
                        return;
                    }
                    view._min = view._leaf_edges->keyByInd(view._min_ind);
                    min = view._min;
                }
                // calc max
                if (view._leaf_edges->keyByInd(view._max_ind) != max) {
                    view._max_ind = std::get<1>(
                            view._leaf_edges->containsAndIndLower(max, view._min_ind, view._max_ind + 1));
                    if (view._min_ind > view._max_ind){
                        view._size = 0;
                        return;
                    }
                    view._max = view._leaf_edges->keyByInd(view._max_ind);
                    max = view._max;
                }
                if (min <= max) {
                    view.updateSize();
                } else {
                    view._size = 0;
                }
            }

            static void setMinMax_III(DiagonalView &view, key_part_t &min, key_part_t &max) {
                // calc min
                if (view._inner_edges->keyByInd(view._min_ind) != min) {
                    view._min_ind = std::get<1>(view._inner_edges->containsAndInd(min, view._min_ind, view._max_ind + 1));
                    if (view._min_ind > view._max_ind){
                        view._size = 0;
                        return;
                    }
                    view._min = view._inner_edges->keyByInd(view._min_ind);
                    min = view._min;
                }
                // calc max
                if (view._inner_edges->keyByInd(view._max_ind) != max) {
                    view._max_ind = std::get<1>(
                            view._inner_edges->containsAndIndLower(max, view._min_ind, view._max_ind + 1));
                    if (view._min_ind > view._max_ind) {
                        view._size = 0;
                        return;
                    }
                    view._max = view._inner_edges->keyByInd(view._max_ind);
                    max = view._max;
                }
                if (min <= max) {
                    view.updateSize();
                } else {
                    view._size = 0;
                }
            }


            void (*setMinMax_p)(DiagonalView &, key_part_t &, key_part_t &);

        protected:
            inline void setMinMax(key_part_t &min, key_part_t &max) {
                (this->setMinMax_p)(*this, min, max);
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
                for (const DiagonalView &diag :diagonals) {
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
                    for (DiagonalView &diag : diagonals) {
                        if (diag._size == 0)
                            return std::make_tuple(min_, max_);
                        diag.setMinMax(min_, max_);
                        if (diag._size == 0)
                            return std::make_tuple(min_, max_);
                    }

                    if ((temp_min == min_) and (temp_max == max_))
                        break;
                    temp_min = min_;
                    temp_max = max_;
                }
                return std::make_tuple(min_, max_);
            }

            class iterator {
                DiagonalView &_view;
                bool _ended;

            public:
                iterator(DiagonalView &view, bool ended = false) : _view{view}, _ended(ended) {}

                iterator &operator++() {
                    if (not _ended) {
                        key_part_t next = _view.first();
                        if (next > _view._max)
                            _ended = true;
                    }
                    return *this;
                }

                BoolHyperTrie *operator*() {
                    // build the result
                    return _view._result;
                }

                inline bool operator==(const iterator &rhs) const {
                    // careful, it doesn't check if it is tested against another iterator for the same Join.
                    return ((rhs._ended and _ended));
                }

                inline bool operator!=(const iterator &rhs) const {
                    return not this->operator==(rhs);
                }
            };

            iterator begin() {
                return iterator{*this};
            }

            iterator end() {
                return iterator{*this, true};
            }
        };
    };
};

#endif //SPARSETENSOR_HYPERTRIE_BOOLHYPERTRIE_HPP


