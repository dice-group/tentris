#include "einsum/operator/Einsum.hpp"

#include <tuple>
#include <type_traits>

using std::tuple;

template<class SubContainer>
class Container;


template<class ContImpl>
class Iterator {
    static_assert(std::is_base_of<Container<ContImpl>, ContImpl>::value, "ContImpl not derived from Container");
public:

    tuple<int, int> &operator*();

    Iterator<ContImpl> &operator++();

    bool operator!=(const Iterator<ContImpl> &rhs) const;
};



template<class SubContainer>
class Container {
public:
    Iterator<SubContainer> begin();

    Iterator<SubContainer> end();

};

class ContImpl;

template<>
class Iterator<ContImpl> {
public:
    ContImpl &container;

    Iterator(ContImpl &container, bool is_end = false) : container(container) { init(); }

    void init() {
    }

    tuple<int, int> &operator*() {
    }

    Iterator<ContImpl> &operator++() {
    }

    bool operator!=(const Iterator<ContImpl> &rhs) const {
    }
};

class ContImpl : public Container<ContImpl> {
public:
    Iterator<ContImpl> begin() {
        return Iterator<ContImpl>(*this, false);
    }

    Iterator<ContImpl> end() {
        return Iterator<ContImpl>(*this, true);
    }
};


int main() {
    ContImpl cont{};
    for (auto &entry : cont) {
        //
    }
}