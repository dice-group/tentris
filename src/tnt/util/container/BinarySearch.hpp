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
    inline size_t insert_pos(const std::vector<T> &sorted_vector, const T &value) noexcept {
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
    inline size_t insert_pos(const std::vector<T> &sorted_vector, const size_t low, const T &value) noexcept {
        assert(low <= sorted_vector.size());
        if (size_t size = sorted_vector.size(); low >= size)
            return size;
        auto low_iterator = sorted_vector.cbegin();
        std::advance(low_iterator, low);
        auto iterator = std::lower_bound(low_iterator, sorted_vector.cend(), value);
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
    inline size_t insert_pos(const std::vector<T> &sorted_vector, const size_t low, const size_t high, const T &value) noexcept {
        assert(high <= sorted_vector.size());
        assert(low <= sorted_vector.size());
        if (low >= high)
            return high;
        else {
            auto low_iterator = sorted_vector.cbegin();
            std::advance(low_iterator, low);
            auto high_iterator = sorted_vector.cbegin();
            std::advance(high_iterator, high);
            auto iterator = std::lower_bound(low_iterator, high_iterator, value);
            return static_cast<size_t>(std::distance(sorted_vector.cbegin(), iterator));
        }
    }


    /**
     * Searches for the position of a value in a sorted vector (with no duplicate values).
     * @tparam T Type of the values stored by the vector.
     * @param sorted_vector A sorted vector.
     * @param value The value to search for.
     * @return the position of value in the vector or ::sparsetensor::container::NOT_FOUND if its not in there.
     */
    template<typename T>
    inline size_t search(const std::vector<T> &sorted_vector, const T &value) noexcept {
        auto iterator = std::lower_bound(sorted_vector.cbegin(), sorted_vector.cend(), value);
        if (iterator == sorted_vector.end() or *iterator != value)
            return NOT_FOUND;
        else
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
    inline size_t search(const std::vector<T> &sorted_vector, const size_t low, const T &value) noexcept {
        if (low >= sorted_vector.size())
            return NOT_FOUND;
        else {
            auto low_iterator = sorted_vector.cbegin();
            std::advance(low_iterator, low);
            auto iterator = std::lower_bound(low_iterator, sorted_vector.cend(), value);
            if (iterator == sorted_vector.end() or *iterator != value)
                return NOT_FOUND;
            else
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
    inline size_t search(const std::vector<T> &sorted_vector, const size_t low, const size_t high, const T &value) noexcept {
        assert(high <= sorted_vector.size());
        assert(low <= sorted_vector.size());
        if (low >= high)
            return NOT_FOUND;
        auto low_iterator = sorted_vector.cbegin();
        std::advance(low_iterator, low);
        auto high_iterator = sorted_vector.cbegin();
        std::advance(high_iterator, high);
        auto iterator = std::lower_bound(low_iterator, high_iterator, value);
        if (iterator == high_iterator or *iterator != value)
            return NOT_FOUND;
        else
            return static_cast<size_t>(std::distance(sorted_vector.cbegin(), iterator));
    }
}

#endif //SPARSETENSOR_CONTAINER_BINARYSEARCH

