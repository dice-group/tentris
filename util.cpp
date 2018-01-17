//
// Created by me on 17.01.18.
//

#include "util.hpp"

using std::string;
using std::vector;
using std::ostream;
using std::cout;
using std::endl;

template<typename TElem>
ostream &operator<<(ostream &os, const vector<TElem> &vec) {
    typename vector<TElem>::const_iterator iter_begin = vec.begin();
    typename vector<TElem>::const_iterator iter_end = vec.end();
    os << "(";
    for (auto iter = iter_begin; iter != iter_end; ++iter) {
        cout << ((iter != iter_begin) ? ", " : "") << *iter;
    }
    os << ")";
    return os;
}