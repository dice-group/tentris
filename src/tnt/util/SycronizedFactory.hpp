#ifndef TNT_CACHE_HPP
#define TNT_CACHE_HPP

#include <unordered_map>
#include <memory>
#include <list>
#include <mutex>

template<typename K, typename V>
class SycronizedFactory {
    struct Entry {
        Entry(const K &key, const V &value)
                : key{key}, value{value} {}

        Entry(const K &key, const V &&value)
                : key{key}, value{value} {}

        K key;
        V value;
    };

    using Entry_p =  std::shared_ptr<Entry>;
    using LruList_pos = typename std::list<Entry_p>::iterator;

    /**
     * Mutex to protect access to _lru_list and _cache
     */
    std::mutex _cache_mutex;
    /**
     * Capacity of this Factory.
     */
    size_t _capacity;
    /**
     * entries (key and value) ordered by least recently used.
     */
    std::list<Entry_p> _lru_list;
    /**
     * mapping from key to the position in _lru_list
     */
    std::unordered_map<K, LruList_pos> _key_to_pos;

    /**
     * Checks weather a key is contained and returns a iterator pointing to the corresponding entry.
     * <br/>
     * The call of this function must be guarded by _cache_mutex.
     * @param key key to a entry
     * @return tuple (is in cache)
     */
    std::tuple<bool, LruList_pos> unsynced_contains(const K &key) const {
        if (_key_to_pos.count(key)) {
            LruList_pos pos = _key_to_pos.at(key);
            return {true, pos};
        } else {
            return {false, {}};
        }
    }

    /**
     * Erases the element that wasn't use for the longest time.
     * <br/>
     * The call of this function must be guarded by _cache_mutex.
     */
    void unsynced_popLeastRecentlyUsed() {
        _key_to_pos.erase(_lru_list.back()->key);
        _lru_list.pop_back();
    }

    /**
     * Makes the entry that entry_pos points to the latest. entry_pos is not checked and must be valid.
     * <br/>
     * The call of this function must be guarded by _cache_mutex.
     * @param entry_pos pointer to an entry. It is updated in the process and stays valid.
     */
    void unsafe_makeLatest(LruList_pos &entry_pos) {
        // move it to the front
        _lru_list.splice(_lru_list.begin(), _lru_list, entry_pos);
        // update the mapper
        entry_pos = _lru_list.begin();
        _key_to_pos[(*entry_pos)->key] = entry_pos;
    }


public:
    /**
     * This method constructs a value from a given key. This must be implemented manually for every
     * specialization of this template. <n />
     * When called internally by the SycronizedFactory the return value is wrapped into a std::shared_pointer. Thus
     * the user is not responsable for destructing the return value manually.
     * @param key key to construct value for
     * @return pointer to newly constructed value. The caller is in care of destructing the value.
     */
    static V *construct(const K &key);

    SycronizedFactory(uint capacity = 500) : _capacity{capacity} {}

    size_t size() const {
        std::lock_guard{_cache_mutex};
        return _key_to_pos.size();
    }

    /**
     * Get a std::shared_pointer to a cached resource. The resource is not guaranteed to be in the cache at any time
     * after calling this method. So keep its std::shared_pointer or it may be destructed.
     * @param key key to the resource
     * @return the resource for the key
     */
    std::shared_ptr<V> get(const K &key) {
        std::lock_guard{_cache_mutex};

        if (const auto &[contains, element_p] = unsynced_contains(key); contains) {
            unsafe_makeLatest(element_p);
            return (*element_p)->value;
        } else {
            if (_lru_list.size() == _capacity)
                unsynced_popLeastRecentlyUsed();
            _lru_list.push_front({key, std::make_shared(construct(key))});
            const Entry_p entry_pos = _lru_list.begin();
            _key_to_pos[entry_pos->key] = entry_pos;
            return entry_pos->value;
        }
    }
};

#endif //TNT_CACHE_HPP
