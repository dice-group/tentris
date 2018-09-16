#ifndef SPARSETENSOR_CONTAINER_BINARYSEARCH
#define SPARSETENSOR_CONTAINER_BINARYSEARCH

#include <vector>
#include <algorithm>
#include <iterator>

namespace tnt::util::container {
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
     * @return insert position
     */
    template<typename T>
    inline size_t insert_pos(const std::vector<T> &sorted_vector, const T &value) {
        auto iterator = std::lower_bound(sorted_vector.cbegin(), sorted_vector.cend(), value);
        return static_cast<size_t>(std::distance(sorted_vector.cbegin(), iterator));
    }

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
    inline size_t insert_pos(const std::vector<T> &sorted_vector, const T &value, size_t low) {
        size_t probe = sorted_vector.size();
        if (low >= probe)
            return probe;
        auto lowIterator = sorted_vector.cbegin();
        std::advance(lowIterator, low);
        auto iterator = std::lower_bound(lowIterator, sorted_vector.cend(), value);
        return static_cast<size_t>(std::distance(sorted_vector.cbegin(), iterator));

    }

    /**
     * Searches for the position to insert an given value into a sorted vector (with no duplicate values). If it exists already its position is
     * returned.
     * @tparam T Type of the values stored by the vector.
     * @param sorted_vector A sorted vector.
     * @param value The value to search for.
     * @param low index of the lower end (included) of the search range, default: 0
     * @param high index of the higher end (excluded) of the search range todo take care of it usages
     * @return insert position
     */
    template<typename T>
    inline size_t insert_pos(const std::vector<T> &sorted_vector, const T &value, size_t low, size_t high) {
        if (high > sorted_vector.size()) //todo throw exception or just replace high or make sure not have such a case and remove the line
            high = sorted_vector.size();
        if (low > high) //todo maybe illegal usages
            return high + 1;
        if (low + 1 == high)
            return sorted_vector.at(low) <= value ? low : high;

        auto lowIterator = sorted_vector.cbegin();
        std::advance(lowIterator, low);
        auto highIterator = sorted_vector.cbegin();
        std::advance(highIterator, high);

        auto iterator = std::lower_bound(lowIterator, highIterator, value);
        return static_cast<size_t>(std::distance(sorted_vector.cbegin(), iterator));
    }


    /**
     * Searches for the position of a value in a sorted vector (with no duplicate values).
     * @tparam T Type of the values stored by the vector.
     * @param sorted_vector A sorted vector.
     * @param value The value to search for.
     * @return the position of value in the vector or ::sparsetensor::container::NOT_FOUND if its not in there.
     */
    template<typename T>
    inline size_t search(const std::vector<T> &sorted_vector, const T &value) {
        auto iterator = std::lower_bound(sorted_vector.cbegin(), sorted_vector.cend(), value);
        if (iterator == sorted_vector.end() or *iterator != value) return NOT_FOUND;
        return static_cast<size_t>(std::distance(sorted_vector.cbegin(), iterator));
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
    inline size_t search(const std::vector<T> &sorted_vector, const T &value, const size_t &low) {
        size_t probe = sorted_vector.size();
        if (low >= probe)
            return NOT_FOUND;
        else {
            auto lowIterator = sorted_vector.cbegin();
            std::advance(lowIterator, low);
            auto iterator = std::lower_bound(lowIterator, sorted_vector.cend(), value);
            if (iterator == sorted_vector.end() or *iterator != value) return NOT_FOUND;
            return static_cast<size_t>(std::distance(sorted_vector.cbegin(), iterator));
        }
    }

    /**
     * Searches for the position of a value in a sorted vector (with no duplicate values).
     * @tparam T Type of the values stored by the vector.
     * @param sorted_vector A sorted vector.
     * @param value The value to search for.
     * @param low index of the lower end (included) of the search range, default: 0
     * @param high index of the higher end (excluded) of the search range todo take care of it usages
     * @return the position of value in the vector or ::sparsetensor::container::NOT_FOUND if its not in there.
     */
    template<typename T>
    inline size_t search(const std::vector<T> &sorted_vector, const T &value, const size_t low, const size_t high) {
        if (low > high)
            return NOT_FOUND;
//        else if (low + 1 == high)
//            return sorted_vector.at(low) == value ? low : NOT_FOUND;

        auto lowIterator = sorted_vector.cbegin();
        std::advance(lowIterator, low);
        auto highIterator = sorted_vector.cbegin();
        std::advance(highIterator, high);
        auto iterator = std::lower_bound(lowIterator, highIterator, value);

        if (iterator == highIterator or *iterator != value) return NOT_FOUND;

        return static_cast<size_t>(std::distance(sorted_vector.cbegin(), iterator));
    }
}

#endif //SPARSETENSOR_CONTAINER_BINARYSEARCH

