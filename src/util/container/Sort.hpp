#ifndef SPARSETENSOR_UTIL_SORT
#define SPARSETENSOR_UTIL_SORT

#include <vector>
#include <numeric>
#include <algorithm>

namespace tnt::util::container {
/**
 * Calculates the permutation that sorts the given vector.
 * @tparam T type of the values the vector holds
 * @tparam Compare Type of the compare function.
 * @param vec the vector
 * @param compare the compare function
 * @return a vector that maps the the positions in the vector to the sorted positions.
 */
    template<typename T, typename Compare>
    std::vector<std::size_t> sortPermutation(const std::vector<T> &vec, const Compare &compare) {
        std::vector<std::size_t> permutation(vec.size());
        std::iota(permutation.begin(), permutation.end(), 0);
        std::sort(permutation.begin(), permutation.end(),
                  [&](size_t i, size_t j) { return compare(vec[i], vec[j]); });
        return permutation;
    }

/**
 * Calculates the inverse permutation of a given sort permutation. With the inverse permutation a sorted vector can
 * be restored to the order it had before it was sorted.
 * @tparam T type of the values the vector holds
 * @param permutation a sort permutation
 * @return the inverse of the given sort permutation
 */
    template<typename T>
    inline std::vector<size_t> invPermutation(const std::vector<T> &permutation) {
        std::vector<size_t> inv_permutation(permutation.size());
        for (size_t i = 0; i < permutation.size(); i++)
            inv_permutation[permutation[i] - 1] = i + 1;
        return inv_permutation;
    }


/**
 * Applies a permutation inplace.
 * @tparam T type of the vector
 * @param vec the vector to apply the permutation on
 * @param permutation the permutation.
 */
    template<typename T>
    void applyPermutation(std::vector<T> &vec, const std::vector<std::size_t> &permutation) {
        std::vector<bool> done(vec.size());
        for (std::size_t i = 0; i < vec.size(); ++i) {
            if (done[i]) {
                continue;
            }
            done[i] = true;
            std::size_t prev_j = i;
            std::size_t j = permutation[i];
            while (i != j) {
                std::swap(vec[prev_j], vec[j]);
                done[j] = true;
                prev_j = j;
                j = permutation[j];
            }
        }
    }
};


#endif //SPARSETENSOR_UTIL_SORT
