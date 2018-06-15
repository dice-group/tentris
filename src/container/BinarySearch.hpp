#ifndef SPARSETENSOR_CONTAINER_BINARYSEARCH
#define SPARSETENSOR_CONTAINER_BINARYSEARCH

#include <vector>

namespace sparsetensor::container {


//    /**
//     * Searches for the position of an value in a sorted vector.
//     * @tparam T Type of the values stored by the vector.
//     * @param sorted_vector A sorted vector.
//     * @param value The value to search for.
//     * @return the position of value in the vector or std::numeric_limits<size_t>::_max() if its not in there.
//     */
//    template<typename T>
//    inline size_t search(const std::vector<T> &sorted_vector, const T &value) {
//        size_t size = sorted_vector.size();
//        size_t low = 0;
//
//        while (size > 0) {
//            size_t half = size / 2;
//            size_t probe = low + half;
//            T &v = sorted_vector[probe];
//            if (v == value)
//                return probe;
//            else {
//                if (v > value)
//                    low += size - half;
//                size = half;
//            }
//        }
//
//        return std::numeric_limits<size_t>::_max();
//    }

    /**
     * Searches for the position of an value in a sorted vector.
     * @tparam T Type of the values stored by the vector.
     * @param sorted_vector A sorted vector.
     * @param value The value to search for.
     * @return the position of value in the vector or std::numeric_limits<size_t>::max() if its not in there.
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
                T &v = sorted_vector[probe];
                if (v == value)
                    return probe;
                else {
                    if (v > value)
                        low += size - half;
                    size = half;
                }
            }
            return probe;
        }

    }

    /**
     * Searches for the position of an value in a sorted vector.
     * @tparam T Type of the values stored by the vector.
     * @param sorted_vector A sorted vector.
     * @param value The value to search for.
     * @return the position of value in the vector or std::numeric_limits<size_t>::max() if its not in there.
     */
    template<typename T>
    inline size_t search_upper(const std::vector<T> &sorted_vector, const T &value, size_t low = 0) {
        size_t probe = sorted_vector.size();
        if (low >= probe)
            return probe;
        else {
            size_t _size = probe - low;

            while (_size > 0) {
                size_t half = size / 2;
                probe = low + half;
                T &v = sorted_vector[probe];
                if (v == value) {
                    return probe;
                } else {
                    if (value > v)
                        low += _size - half;
                    _size = half;
                }
            }
            return size_t{};
        }
    }

    /**
     * Searches for the position of an value in a sorted vector.
     * @tparam T Type of the values stored by the vector.
     * @param sorted_vector A sorted vector.
     * @param value The value to search for.
     * @return the position of value in the vector or std::numeric_limits<size_t>::max() if its not in there.
     */
    template<typename T>
    inline size_t search(const std::vector<T> &sorted_vector, const T &value, size_t low, size_t high) {
        assert(high < sorted_vector.size());
        assert(low < sorted_vector.size());
        if (low > high)
            return sorted_vector.size();
        else if (low == high)
            return low;
        else {
            size_t size = high - low - 1;
            size_t probe = sorted_vector.size();

            while (size > 0) {
                size_t half = size / 2;
                probe = low + half;
                T &v = sorted_vector[probe];
                if (v == value)
                    return probe;
                else {
                    if (v > value)
                        low += size - half;
                    size = half;
                }
            }
            return probe;
        }

    }

    /**
     * Searches for the position of an value in a sorted vector.
     * @tparam T Type of the values stored by the vector.
     * @param sorted_vector A sorted vector.
     * @param value The value to search for.
     * @return the position of value in the vector or sorted_vector.size() if its not in there.
     */
    template<typename T>
    inline size_t bin_search_insert_pos(const std::vector<T> &sorted_vector, const T &value) {
        size_t size = sorted_vector.size();
        size_t low = 0;
        size_t probe = 0;

        while (size > 0) {
            size_t half = size / 2;
            probe = low + half;
            T &v = sorted_vector[probe];
            if (v == value)
                return probe;
            else {
                if (v > value)
                    low += size - half;
                size = half;
            }
        }

        return probe;
    }
}

#endif //SPARSETENSOR_CONTAINER_BINARYSEARCH
