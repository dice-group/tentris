#ifndef SPARSETENSOR_CONTAINER_BINARYSEARCH
#define SPARSETENSOR_CONTAINER_BINARYSEARCH

#include <vector>
#include <algorithm>

namespace sparsetensor::container {
    /**
     * NOT_FOUND return value for function search
     */
    constexpr const size_t NOT_FOUND = std::numeric_limits<size_t>::max();
    /**
     * Maximum value of std::size_t.
     */
    constexpr const size_t MAX_SIZE_T = std::numeric_limits<size_t>::max();
    /**
     * Minimum value of std::size_t.
     */
    constexpr const size_t MIN_SIZE_T = std::numeric_limits<size_t>::min();

    /**
     * Searches for the position to insert an given value into a sorted vector (with no duplicate values). If it exists already its position is
     * returned.
     * @tparam T Type of the values stored by the vector.
     * @param sorted_vector A sorted vector.
     * @param value The value to search for.
     * @param low index of the lower end (included) of the search range, default: 0
     * @return insert position
     */
    template<typename T>
    inline size_t insert_pos(const std::vector<T> &sorted_vector, const T &value, size_t low = 0) {
        size_t probe = sorted_vector.size();
        if (low >= probe)
            return probe;
        else {
            size_t size = probe - low;
            while (size > 0) {
                size_t half = size / 2;
                probe = low + half;
                const T &v = sorted_vector[probe];
                if (v == value)
                    return probe;
                else {
                    if (v < value)
                        low += size - half;
                    size = half;
                }
            }
            return low;
        }
    }

    /**
     * Searches for the position of a value in a sorted vector (with no duplicate values).
     * @tparam T Type of the values stored by the vector.
     * @param sorted_vector A sorted vector.
     * @param value The value to search for.
     * @param low index of the lower end (included) of the search range, default: 0
     * @return the position of value in the vector or ::sparsetensor::container::NOT_FOUND if its not in there.
     */
    template<typename T>
    inline size_t search(const std::vector<T> &sorted_vector, const T &value, size_t low = 0) {
        size_t probe = sorted_vector.size();
        if (low >= probe)
            return probe;
        else {
            size_t size = probe - low;
            while (size > 0) {
                size_t half = size / 2;
                probe = low + half;
                const T &v = sorted_vector[probe];
                if (v == value)
                    return probe;
                else {
                    if (v < value)
                        low += size - half;
                    size = half;
                }
            }
            return NOT_FOUND;
        }
    }

    /**
     * Searches for the position to insert an given value into a sorted vector (with no duplicate values). If it exists already its position is
     * returned.
     * @tparam T Type of the values stored by the vector.
     * @param sorted_vector A sorted vector.
     * @param value The value to search for.
     * @param low index of the lower end (included) of the search range, default: 0
     * @param high index of the higher end (included) of the search range
     * @return insert position
     */
    template<typename T>
    inline size_t insert_pos(const std::vector<T> &sorted_vector, const T &value, size_t low, size_t high) {
        // TODO: siehe Klebezettel am Monitor
        if (low > high)
            return sorted_vector.size();
        else if (low == high)
            return low;
        else {
            size_t size = high - low + 1;
            size_t probe;

            while (size > 0) {
                size_t half = size / 2;
                probe = low + half;
                const T &v = sorted_vector[probe];
                if (v == value)
                    return probe;
                else {
                    if (v < value)
                        low += size - half;
                    size = half;
                }
            }
            return low;
        }

    }

    /**
     * Searches for the position of a value in a sorted vector (with no duplicate values).
     * @tparam T Type of the values stored by the vector.
     * @param sorted_vector A sorted vector.
     * @param value The value to search for.
     * @param low index of the lower end (included) of the search range, default: 0
     * @param high index of the higher end (included) of the search range
     * @return the position of value in the vector or ::sparsetensor::container::NOT_FOUND if its not in there.
     */
    template<typename T>
    inline size_t search(const std::vector<T> &sorted_vector, const T &value, size_t low, size_t high) {
        if (low > high)
            return sorted_vector.size();
        else if (low == high)
            return low;
        else {
            size_t size = high - low + 1;
            size_t probe = sorted_vector.size();

            while (size > 0) {
                size_t half = size / 2;
                probe = low + half;
                const T &v = sorted_vector[probe];
                if (v == value)
                    return probe;
                else {
                    if (v < value)
                        low += size - half;
                    size = half;
                }
            }
            return NOT_FOUND;
        }
    }
}

#endif //SPARSETENSOR_CONTAINER_BINARYSEARCH
