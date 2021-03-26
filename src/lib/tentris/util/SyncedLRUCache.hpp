#ifndef TENTRIS_SYNCEDLRUCACHE_HPP
#define TENTRIS_SYNCEDLRUCACHE_HPP

/*
 * LRUCache11 - a templated C++11 based LRU cache class that allows
 * specification of
 * key, value and optionally the map container type (defaults to
 * std::unordered_map)
 * By using the std::unordered_map and a linked list of keys it allows O(1) insert, delete
 * and
 * refresh operations.
 *
 * This is a header-only library and all you need is the LRUCache11.hpp file
 *
 * Github: https://github.com/mohaps/lrucache11
 *
 * This is a follow-up to the LRUCache project -
 * https://github.com/mohaps/lrucache
 *
 * Copyright (c) 2012-22 SAURAV MOHAPATRA <mohaps@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <algorithm>
#include <cstdint>
#include <list>
#include <mutex>
#include <stdexcept>
#include <thread>

#include <Dice/hash/DiceHash.hpp>

namespace tentris::util::sync {

	template<typename K, typename V>
	struct KeyValuePair {
		K key;
		std::shared_ptr<V> value;

		KeyValuePair(K key) : key(std::move(key)), value(std::make_shared<V>(this->key)) {}
	};

	template<class Key, class Value>
	class SyncedLRUCache {
	public:
		using map_type = tsl::hopscotch_map<Key, typename std::list<KeyValuePair<Key, Value>>::iterator, Dice::hash::DiceHash<Key>>;
		using node_type =  KeyValuePair<Key, Value>;
		using list_type = std::list<KeyValuePair<Key, Value>>;
		using Lock  = std::mutex;
		using Guard = std::lock_guard<Lock>;
		using value_ptr = std::shared_ptr<Value>;

		// Disallow copying.
		SyncedLRUCache(const SyncedLRUCache &) = delete;

		SyncedLRUCache &operator=(const SyncedLRUCache &) = delete;

	private:
		mutable Lock lock_;
		map_type cache_;
		list_type keys_;
		size_t maxSize_;
		size_t elasticity_;
	public:

		/**
		 * the maxSize is the soft limit of keys and (maxSize + elasticity) is the
		 * hard limit
		 * the cache is allowed to grow till (maxSize + elasticity) and is pruned back
		 * to maxSize keys
		 * set maxSize = 0 for an unbounded cache (but in that case, you're better off
		 * using a std::unordered_map
		 * directly anyway! :)
		 */
		explicit SyncedLRUCache(size_t maxSize = 1000, size_t elasticity = 100)
				: maxSize_(maxSize), elasticity_(elasticity) {}

		[[nodiscard]] size_t size() const {
			Guard g(lock_);
			return cache_.size();
		}

		[[nodiscard]] bool empty() const {
			Guard g(lock_);
			return cache_.empty();
		}

		void clear() {
			Guard g(lock_);
			cache_.clear();
			keys_.clear();
		}

		[[nodiscard]] value_ptr &operator[](const Key &key) {
			Guard g(lock_);
			const auto iter = cache_.find(key);
			logging::logTrace(fmt::format("cache size: {} lru size: {}", cache_.size(), keys_.size()));
			if (iter == cache_.end()) {
				auto &key_value = keys_.emplace_front(key);
				cache_[key] = keys_.begin();
				prune();
				return key_value.value;
			} else {
				keys_.splice(keys_.begin(), keys_, iter->second);
				return iter->second->value;
			}
		}

		size_t getMaxSize() const { return maxSize_; }

		size_t getElasticity() const { return elasticity_; }

		size_t getMaxAllowedSize() const { return maxSize_ + elasticity_; }

	private:

		size_t prune() {
			size_t maxAllowed = maxSize_ + elasticity_;
			if (maxSize_ == 0 || cache_.size() < maxAllowed) {
				return 0;
			}
			size_t count = 0;
			while (cache_.size() > maxSize_) {
				cache_.erase(keys_.back().key);
				keys_.pop_back();
				++count;
			}
			return count;
		}


	};

}


#endif //TENTRIS_SYNCEDLRUCACHE_HPP
