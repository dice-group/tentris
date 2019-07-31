#ifndef HYPERTRIE_LARGER_EQUAL_HPP
#define HYPERTRIE_LARGER_EQUAL_HPP

namespace hypertrie::internal::util {
// TODO: use instead of std::lower_bound
	template<class ForwardIt, class T>
	ForwardIt spin_on(ForwardIt first, const ForwardIt &last, const T &value) {
		ForwardIt it;
		typename std::iterator_traits<ForwardIt>::difference_type count, step;
		count = std::distance(first, last);

		while (count > 0) {
			it = first;
			step = count / 2;
			std::advance(it, step);
			if (value == *it) {
				return it;
			} else if (*it < value) {
				first = ++it;
				count -= step + 1;
			} else
				count = step;
		}
		if (first == last - 1 && *first != value)
			return last;
		else
			return first;
	}
}

#endif //HYPERTRIE_LARGER_EQUAL_HPP
