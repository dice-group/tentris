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
#include <type_traits>

#include <robin_hood.h>
#include <spdlog/spdlog.h>

#include <dice/hash/DiceHash.hpp>

namespace dice::endpoint {
	template<class Key, class Value>
	class SyncedLRUCache {

		struct node_type {
			Key key;
			std::shared_ptr<Value const> value;

			node_type(Key key) : key(std::move(key)), value(std::make_shared<Value>(this->key)) {}
		};

		using list_type = std::list<node_type>;
		using map_type = robin_hood::unordered_map<Key, typename list_type::iterator, dice::hash::DiceHashMartinus<Key>>;
		using value_ptr = std::shared_ptr<Value>;

	private:
		mutable std::mutex lock_;
		map_type cache_;
		list_type lru_list_;
		size_t const max_size_;
		size_t const elasticity_;

	public:
		/**
		 * the maxSize is the soft limit of keys and (maxSize + elasticity) is the
		 * hard limit
		 * the cache is allowed to grow till (max_size + elasticity) and is pruned back
		 * to maxSize keys
		 * set maxSize = 0 for an unbounded cache (but in that case, you're better off
		 * using a std::unordered_map
		 * directly anyway! :)
		 */
		explicit SyncedLRUCache(size_t max_size = 1000, size_t elasticity = 100) noexcept
			: max_size_(max_size), elasticity_(elasticity) {}

		// Disallow copying.
		SyncedLRUCache(const SyncedLRUCache &) = delete;

		SyncedLRUCache &operator=(const SyncedLRUCache &) = delete;

		[[nodiscard]] size_t size() const noexcept {
			std::lock_guard<std::mutex> g(lock_);
			return cache_.size();
		}

		[[nodiscard]] bool empty() const noexcept {
			std::lock_guard<std::mutex> g(lock_);
			return cache_.empty();
		}

		void clear() noexcept {
			std::lock_guard<std::mutex> g(lock_);
			cache_.clear();
			lru_list_.clear();
		}

		[[nodiscard]] std::shared_ptr<Value const> operator[](Key const &key) noexcept(std::is_nothrow_constructible_v<Value, Key>) {
			std::lock_guard<std::mutex> g(lock_);
			spdlog::trace("Query cache entries: {}/{} (elastic: {})", cache_.size(), max_size(), max_allowed_size());
			const auto iter = cache_.find(key);
			if (iter == cache_.end()) {
				auto &key_value = lru_list_.emplace_front(key);
				cache_[key] = lru_list_.begin();
				prune();
				return key_value.value;
			} else {
				lru_list_.splice(lru_list_.begin(), lru_list_, iter->second);
				return iter->second->value;
			}
		}

		[[nodiscard]] size_t max_size() const noexcept { return max_size_; }

		[[nodiscard]] size_t elasticity() const noexcept { return elasticity_; }

		[[nodiscard]] size_t max_allowed_size() const noexcept { return max_size_ + elasticity_; }

		bool is_compiled();

	private:
		size_t prune() noexcept {
			if (max_size_ == 0 || cache_.size() < max_allowed_size()) {
				return 0;
			}
			size_t count = 0;
			while (cache_.size() > max_size_) {
				cache_.erase(lru_list_.back().key);
				lru_list_.pop_back();
				++count;
			}
			return count;
		}
	};

}// namespace dice::endpoint


#endif//TENTRIS_SYNCEDLRUCACHE_HPP
