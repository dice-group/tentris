//
// Created by me on 29.01.18.
//

#ifndef TEST_HYPERTRIEMATCHKEYPOSITERATOR_HPP
#define TEST_HYPERTRIEMATCHKEYPOSITERATOR_HPP

#import <cstdint>
#import "HyperTrie.hpp"
#import "../einsum/Subscript.hpp"
#import <variant>
#import <tuple>

using std::variant;
using std::tuple;
using Subscript::label_pos_t;
using Subscript::label_t;


template<typename T>
class HyperTrieMatchKeyPosIterator;

template<typename T>
class HyperTrieDiagonal {

public:
    HyperTrieDiagonal(HyperTrie<T> *hyperTrie, vector<label_pos_t> &label_poss) : hyperTrie(hyperTrie),
                                                                                  label_poss(label_poss) {
        min_card_key_pos = hyperTrie->getMinCardKeyPos();
    }

    ~HyperTrieDiagonal() = default {
        delete iter;
    }

    HyperTrieMatchKeyPosIterator *iter;
    HyperTrie<T> *hyperTrie;
    key_pos_t min_card_key_pos{};
    const vector<label_pos_t> label_poss;

    size_t estimCard() {
        return hyperTrie->getCard(min_card_key_pos);
    }

    key_part_t min() {
        return hyperTrie->getMinKeyPart(min_card_key_pos);
    }

    key_part_t max() {
        return hyperTrie->getMaxKeyPart(min_card_key_pos);;
    }

    optional<variant<HyperTrie<T> *, T>> find(const key_part_t &key_part) {
        // TODO: make real key
        const vector<key_part_t> &key = vector<key_part_t>{key_part};

        return hyperTrie->get(key);;

    }


    HyperTrieMatchKeyPosIterator lower_bound(key_part_t min_key) {
        return HyperTrieMatchKeyPosIterator(<#initializer#>, std::vector());
    }

    HyperTrieMatchKeyPosIterator upper_bound(key_part_t max_key) {
        return HyperTrieMatchKeyPosIterator(<#initializer#>, std::vector());
    }
};

template<typename T>
class HyperTrieMatchKeyPosIterator : public std::iterator<std::input_iterator_tag,
        tuple<uint64_t, vector<variant<HyperTrie<T> *, T>>>> {


public:

    HyperTrieMatchKeyPosIterator(const HyperTrie<T> *&hyperTrie, const vector<key_pos_t> &key_poss) {

        if (hyperTrie->empty()) {
            end = true;
        } else {
            key_pos_t min_card_key_pos = hyperTrie->getMinCardKeyPos();


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
                this->subkey.push_back(std::nullopt);
            }

            // get iterator to iterate the sub-HyperTries or values by the min-card position.
            map<key_part_t, variant<HyperTrie<T> *, T>> *edges = hyperTrie->edges_by_pos.at(min_card_key_pos);
            this->child_it = edges->begin();
            edges->lower_bound()
            this->child_it_end = edges->end();
            this->size = edges->size();
            (*this)++;
        }
    }

private:
    typename map<key_part_t, variant<HyperTrie<T> *, T>>::iterator child_it;
    typename map<key_part_t, variant<HyperTrie<T> *, T>>::iterator child_it_end;
    key_part_t key_part{};
    bool end{};
    size_t size{};
    vector<key_pos_t> key_poss{};
    vector<std::optional<key_part_t>> subkey{};

public:
    key_part_t *first = &key_part;
    variant<HyperTrie<T> *, T> *second;


public:

    size_t getSize() const {
        return size;
    }

    HyperTrieMatchKeyPosIterator &operator++() {
        if (child_it == child_it_end) { // end reached.
            end = true;
        } else { // end not yet at end
            do {
                // get next key_part
                key_part = child_it->first;
                // get next result for min-card position
                variant<HyperTrie<T> *, T> &child = child_it->second;
                if (subkey.size() == 0) {
                    second = &child;
                    break;
                } else {
                    // write the key_part to the relevant positions of the subkey
                    for (const key_pos_t &key_pos : key_poss) {
                        subkey[key_pos] = key_part;
                    }

                    // check if the same key_part exists also for the other relevant key_pos
                    optional<variant<HyperTrie<T> *, T>> &result = std::get<HyperTrie<T> *>(child)->get(subkey);
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

    void lower_bound(T low) {
        child_it.lower
    }

    HyperTrieMatchKeyPosIterator operator++(int) {
        HyperTrieMatchKeyPosIterator it_copy{*this};
        operator++();
        return it_copy;
    }

    bool operator==(const HyperTrieMatchKeyPosIterator &rhs) const {
        // TODO: does this make sense?
        if (rhs.end && end)
            return true;
        else
            return key_part == rhs.key_part;
    }

    bool operator!=(const HyperTrieMatchKeyPosIterator &rhs) const {
        // TODO: does this make sense?
        if (rhs.end != end)
            return true;
        else
            return key_part != rhs.key_part;
    }
};


#endif //TEST_HYPERTRIEMATCHKEYPOSITERATOR_HPP
