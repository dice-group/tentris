#ifndef HYPERTRIE_BOOLHYPERTRIE_IMPL_HPP
#define HYPERTRIE_BOOLHYPERTRIE_IMPL_HPP


#include "hypertrie/internal/util/CONSTANTS.hpp"

#include "hypertrie/internal/RawBoolHypertrie.hpp"
#include "hypertrie/internal/util/CONSTANTS.hpp"
#include <functional>
#include <memory>

namespace hypertrie::internal {
	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class HashDiagonal;

	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class OrderedDiagonal;

	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class BoolHypertrie;


	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class const_BoolHypertrie {
	protected:
		template<pos_type depth>
		using RawBoolHypertrie = typename hypertrie::internal::interface::rawboolhypertrie<key_part_type, map_type, set_type>::template RawBoolHypertrie<depth>;
		template<pos_type depth, pos_type diag_depth>
		using RawHashDiagonal = typename hypertrie::internal::interface::rawboolhypertrie<key_part_type, map_type, set_type>::template RawHashDiagonal<diag_depth, depth>;
		template<pos_type depth, pos_type diag_depth>
		using RawOrderedDiagonal = typename hypertrie::internal::interface::rawboolhypertrie<key_part_type, map_type, set_type>::template RawOrderedDiagonal<diag_depth, depth>;
	public:
		typedef std::vector<std::optional<key_part_type>> SliceKey;
		typedef std::vector<key_part_type> Key;
	protected:

		pos_type depth_ = 0;

		std::shared_ptr<void> hypertrie;

		static std::shared_ptr<void> get_hypertrie(pos_type depth) {
			switch (depth) {
				case 1:
					return std::make_shared<RawBoolHypertrie<1>>();
				case 2:
					return std::make_shared<RawBoolHypertrie<2>>();
				case 3:
					return std::make_shared<RawBoolHypertrie<3>>();
				case 4:
					return std::make_shared<RawBoolHypertrie<4>>();
				case 5:
					return std::make_shared<RawBoolHypertrie<5>>();
				default:
					throw std::logic_error{"not implemented."};
			}
		}

	public:
		const_BoolHypertrie() = default;

		const_BoolHypertrie(const_BoolHypertrie &) = default;

		const_BoolHypertrie(const const_BoolHypertrie &) = default;

		const_BoolHypertrie(const_BoolHypertrie &&) noexcept = default;

		const_BoolHypertrie &operator=(const BoolHypertrie<key_part_type, map_type, set_type> &other) {
			this->depth_ = other.depth_;
			this->hypertrie = other.hypertrie;
			return *this;
		}

		const_BoolHypertrie &operator=(const const_BoolHypertrie &other) = default;

		const_BoolHypertrie &operator=(BoolHypertrie<key_part_type, map_type, set_type> other) {
			this->depth_ = other.depth_;
			this->hypertrie = other.hypertrie;
			return *this;
		}

		const_BoolHypertrie &operator=(const_BoolHypertrie &&other) = default;

		const_BoolHypertrie &operator=(BoolHypertrie<key_part_type, map_type, set_type> &&other) {
			this->depth_ = std::move(other.depth_);
			this->hypertrie = std::move(other.hypertrie);
			return *this;
		}

		explicit const_BoolHypertrie(pos_type depth) : depth_(depth), hypertrie(get_hypertrie(depth)) {}

	protected:
		template<pos_type depth>
		explicit const_BoolHypertrie(const std::shared_ptr<RawBoolHypertrie<depth> const> &boolhypertrie)
				: depth_(depth), hypertrie(std::const_pointer_cast<RawBoolHypertrie<depth>>(
				const_cast<std::shared_ptr<RawBoolHypertrie<depth> const> &>(boolhypertrie))) {}

		template<pos_type depth>
		inline static const_BoolHypertrie
		instance(const std::shared_ptr<RawBoolHypertrie<depth> const> &boolhypertrie) {
			return const_BoolHypertrie(boolhypertrie);
		}


	protected:
		template<pos_type depth>
		inline static std::vector<size_t>
		rawGetCards(const std::shared_ptr<void> &hypertrie, const std::vector<pos_type> &positions) {
			auto raw_hypertrie = std::static_pointer_cast<RawBoolHypertrie<depth> const>(hypertrie);
			return raw_hypertrie->getCards(positions);
		}

	public:
		[[nodiscard]]
		std::vector<size_t> getCards(const std::vector<pos_type> &positions) const {
			assert(positions.size() <= depth());
			switch (depth_) {
				case 1: {
					return rawGetCards<1>(hypertrie, positions);
				}
				case 2: {
					return rawGetCards<2>(hypertrie, positions);
				}
				case 3: {
					return rawGetCards<3>(hypertrie, positions);
				}
				case 4: {
					return rawGetCards<4>(hypertrie, positions);
				}
				case 5: {
					return rawGetCards<5>(hypertrie, positions);
				}
				default:
					throw std::logic_error{"not implemented."};
			}
		}

		[[nodiscard]]
		size_t size() const {
			switch (depth_) {
				case 1: {
					return std::static_pointer_cast<RawBoolHypertrie<1> const>(hypertrie)->size();
				}
				case 2: {
					return std::static_pointer_cast<RawBoolHypertrie<2> const>(hypertrie)->size();
				}
				case 3: {
					return std::static_pointer_cast<RawBoolHypertrie<3> const>(hypertrie)->size();
				}
				case 4: {
					return std::static_pointer_cast<RawBoolHypertrie<4> const>(hypertrie)->size();
				}
				case 5: {
					return std::static_pointer_cast<RawBoolHypertrie<5> const>(hypertrie)->size();
				}
				default:
					throw std::logic_error{"not implemented."};
			}
		}

		[[nodiscard]]
		bool operator[](const Key &key) const {
			switch (depth_) {
				case 1:
					return std::static_pointer_cast<RawBoolHypertrie<1> const>(hypertrie)->operator[](key[0]);
				case 2: {
					typename RawBoolHypertrie<2>::Key raw_key;
					std::copy_n(key.begin(), 2, raw_key.begin());
					return std::static_pointer_cast<RawBoolHypertrie<2> const>(hypertrie)->operator[](raw_key);
				}
				case 3: {
					typename RawBoolHypertrie<3>::Key raw_key;
					std::copy_n(key.begin(), 3, raw_key.begin());
					return std::static_pointer_cast<RawBoolHypertrie<3> const>(hypertrie)->operator[](raw_key);
				}
				case 4: {
					typename RawBoolHypertrie<4>::Key raw_key;
					std::copy_n(key.begin(), 4, raw_key.begin());
					return std::static_pointer_cast<RawBoolHypertrie<4> const>(hypertrie)->operator[](raw_key);
				}
				case 5: {
					typename RawBoolHypertrie<5>::Key raw_key;
					std::copy_n(key.begin(), 5, raw_key.begin());
					return std::static_pointer_cast<RawBoolHypertrie<5> const>(hypertrie)->operator[](raw_key);
				}
				default:
					throw std::logic_error{"not implemented."};
			}
		}

	protected:
		template<pos_type depth>
		inline static std::tuple<typename RawBoolHypertrie<depth>::SliceKey, pos_type>
		extractRawSliceKey(const SliceKey &slice_key) {
			typename RawBoolHypertrie<depth>::SliceKey raw_slice_key;
			std::copy_n(slice_key.begin(), depth, raw_slice_key.begin());
			return {raw_slice_key, std::count(slice_key.begin(), slice_key.end(), std::nullopt)};
		}

		template<pos_type depth, pos_type result_depth>
		inline static auto
		executeRawSlice(const std::shared_ptr<void> &hypertrie,
		                typename RawBoolHypertrie<depth>::SliceKey raw_slice_key)
		-> std::conditional_t<(result_depth > 0), std::optional<const_BoolHypertrie const>, bool> {
			auto raw_hypertrie = std::static_pointer_cast<RawBoolHypertrie<depth> const>(hypertrie);
			if constexpr (result_depth > 0) {
				auto result = raw_hypertrie->template operator[]<result_depth>(raw_slice_key);
				if (result)
					return instance(raw_hypertrie->template operator[]<result_depth>(raw_slice_key));
				else
					return std::nullopt;
			} else
				return raw_hypertrie->template operator[]<0>(raw_slice_key);
		}

	public:
		[[nodiscard]]
		std::variant<std::optional<const_BoolHypertrie>, bool> operator[](const SliceKey &slice_key) const {
			switch (depth_) {
				case 1: {
					if (slice_key[0]) {
						return std::static_pointer_cast<RawBoolHypertrie<1> const>(hypertrie)->operator[](
								*(slice_key[0]));
					} else {
						return {*this};
					}
					[[fallthrough]];
				}

				case 2: {
					auto[raw_slice_key, count] = extractRawSliceKey<2>(slice_key);
					switch (count) {
						case 2:
							return {*this};
						case 1:
							return executeRawSlice<2, 1>(hypertrie, std::move(raw_slice_key));
						case 0:
							return executeRawSlice<2, 0>(hypertrie, std::move(raw_slice_key));
					}
					[[fallthrough]];
				}

				case 3: {
					auto[raw_slice_key, count] = extractRawSliceKey<3>(slice_key);
					switch (count) {
						case 3:
							return {*this};
						case 2:
							return executeRawSlice<3, 2>(hypertrie, std::move(raw_slice_key));
						case 1:
							return executeRawSlice<3, 1>(hypertrie, std::move(raw_slice_key));
						case 0:
							return executeRawSlice<3, 0>(hypertrie, std::move(raw_slice_key));
					}
					[[fallthrough]];
				}

				case 4: {
					auto[raw_slice_key, count] = extractRawSliceKey<4>(slice_key);
					switch (count) {
						case 4:
							return {*this};
						case 3:
							return executeRawSlice<4, 3>(hypertrie, std::move(raw_slice_key));
						case 2:
							return executeRawSlice<4, 2>(hypertrie, std::move(raw_slice_key));
						case 1:
							return executeRawSlice<4, 1>(hypertrie, std::move(raw_slice_key));
						case 0:
							return executeRawSlice<4, 0>(hypertrie, std::move(raw_slice_key));
					}
					[[fallthrough]];
				}

				case 5: {
					auto[raw_slice_key, count] = extractRawSliceKey<5>(slice_key);
					switch (count) {
						case 5:
							return {*this};
						case 4:
							return executeRawSlice<5, 4>(hypertrie, std::move(raw_slice_key));
						case 3:
							return executeRawSlice<5, 3>(hypertrie, std::move(raw_slice_key));
						case 2:
							return executeRawSlice<5, 2>(hypertrie, std::move(raw_slice_key));
						case 1:
							return executeRawSlice<5, 1>(hypertrie, std::move(raw_slice_key));
						case 0:
							return executeRawSlice<5, 0>(hypertrie, std::move(raw_slice_key));
					}
					[[fallthrough]];
				}
				default:
					throw std::logic_error{"not implemented."};
			}
		}

		[[nodiscard]]
		pos_type depth() const { return depth_; }

		template<typename, template<typename, typename> class,
				template<typename> class>
		friend
		class HashDiagonal;

		template<typename, template<typename, typename> class,
				template<typename> class>
		friend
		class OrderedDiagonal;

		class iterator {
		protected:
			struct RawMethods {
				std::shared_ptr<void> (*begin)(const const_BoolHypertrie &boolHypertrie);

				Key (*value)(void const *);

				void (*inc)(void *);

				bool (*ended)(void const *);

			};

			template<pos_type depth>
			inline static RawMethods generateRawMethods() {
				return RawMethods{
						[](const const_BoolHypertrie &boolHypertrie) -> std::shared_ptr<void> {
							return std::make_shared<typename RawBoolHypertrie<depth>::iterator>(
									*static_cast<RawBoolHypertrie<depth> const *>(boolHypertrie.hypertrie.get()));
						},
						&RawBoolHypertrie<depth>::iterator::value,
						&RawBoolHypertrie<depth>::iterator::inc,
						&RawBoolHypertrie<depth>::iterator::ended};
			}

			inline static const std::vector<RawMethods> RawMethodsCache{
					generateRawMethods<1>(),
					generateRawMethods<2>(),
					generateRawMethods<3>(),
					generateRawMethods<4>(),
					generateRawMethods<5>()
			};

			static RawMethods const &getRawMethods(pos_type depth) {
				return RawMethodsCache[depth - 1];
			};


		protected:
			RawMethods const *raw_methods;
			std::shared_ptr<void> raw_iterator;

		public:
			using self_type =  iterator;
			using value_type = Key;

			iterator() = default;

			iterator(const_BoolHypertrie const *const boolHypertrie) :
					raw_methods(&getRawMethods(boolHypertrie->depth())),
					raw_iterator(std::invoke(raw_methods->begin, *boolHypertrie)) {}

			iterator(const_BoolHypertrie &boolHypertrie) : iterator(&boolHypertrie) {}

			self_type &operator++() {
				std::invoke(raw_methods->inc, raw_iterator.get());
				return *this;
			}

			value_type operator*() const { return std::invoke(raw_methods->value, raw_iterator.get()); }

			operator bool() const { return not std::invoke(raw_methods->ended, raw_iterator.get()); }

		};

		using const_iterator = iterator;

		[[nodiscard]]
		iterator begin() const { return iterator{this}; }

		[[nodiscard]]
		const_iterator cbegin() const { return iterator{this}; }

		[[nodiscard]]
		bool end() const { return false; }

		[[nodiscard]]
		bool cend() const { return false; }

	};


	template<typename key_part_type, template<typename, typename> class map_type,
			template<typename> class set_type>
	class BoolHypertrie : public const_BoolHypertrie<key_part_type, map_type, set_type> {
	protected:
		using base = const_BoolHypertrie<key_part_type, map_type, set_type>;
		template<pos_type depth>
		using RawBoolHypertrie = typename base::template RawBoolHypertrie<depth>;
		template<pos_type depth, pos_type diag_depth>
		using RawHashDiagonal =  typename base::template RawHashDiagonal<diag_depth, depth>;
		template<pos_type depth, pos_type diag_depth>
		using RawOrderedDiagonal =  typename base::template RawOrderedDiagonal<diag_depth, depth>;
		// proxy for fields:
		using base::depth_;
		using base::hypertrie;
	public:
		using SliceKey =  typename base::SliceKey;
		using Key =  typename base::Key;

		BoolHypertrie() : base{} {}

		BoolHypertrie(BoolHypertrie &boolhypertrie) : base{boolhypertrie} {}

		BoolHypertrie(const BoolHypertrie &) = default;

		BoolHypertrie(BoolHypertrie &&boolhypertrie) noexcept : base{boolhypertrie} {}

		explicit BoolHypertrie(pos_type depth) : base{depth} {}


	protected:
		template<pos_type depth>
		inline static void rawSet(const std::shared_ptr<void> hypertrie, const Key &key, bool value) {
			auto raw_hypertrie = std::static_pointer_cast<RawBoolHypertrie<depth>>(hypertrie);
			if constexpr (depth > 1) {
				typename RawBoolHypertrie<depth>::Key raw_key;
				std::copy_n(key.begin(), depth, raw_key.begin());
				raw_hypertrie->set(raw_key, value);
			} else {
				raw_hypertrie->set(key[0], value);
			}
		}

	public:
		void set(const Key &key, bool value) {
			assert(key.size() == depth_);
			switch (this->depth()) {
				case 1: {
					rawSet<1>(hypertrie, key, value);
					break;
				}
				case 2: {
					rawSet<2>(hypertrie, key, value);
					break;
				}
				case 3: {
					rawSet<3>(hypertrie, key, value);
					break;
				}
				case 4: {
					rawSet<4>(hypertrie, key, value);
					break;
				}
				case 5: {
					rawSet<5>(hypertrie, key, value);
					break;
				}
				default:
					throw std::logic_error{"not implemented."};
			}
		}
	};
}


#endif //HYPERTRIE_BOOLHYPERTRIE_IMPL_HPP

