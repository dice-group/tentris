#ifndef HYPERTRIE_PERMUTATIONSORT_HPP
#define HYPERTRIE_PERMUTATIONSORT_HPP

#include <numeric>
#include <vector>
#include "itertools.hpp"


namespace hypertrie::internal::util::sort_permutation {
	namespace {
		using namespace iter;
	}

	using SortPermutation = std::vector<std::size_t>;

	SortPermutation getDefault(size_t n) {
		SortPermutation permutation(n);
		std::iota(permutation.begin(), permutation.end(), 0);
		return permutation;
	}

	// inspired by and taken from https://stackoverflow.com/questions/17074324/how-can-i-sort-two-vectors-in-the-same-way-with-criteria-that-uses-only-one-of
	template<typename T>
	SortPermutation get(const std::vector<T> &vec) {
		SortPermutation p(vec.size());
		std::iota(p.begin(), p.end(), 0);
		std::sort(p.begin(), p.end(), [&](std::size_t i, std::size_t j) { return vec[i] < vec[j]; });
		return p;
	}

	template<typename T>
	void apply(std::vector<T> &vec, const SortPermutation p) {
		std::vector<bool> done(vec.size());
		for (std::size_t i = 0; i < vec.size(); ++i) {
			if (done[i]) {
				continue;
			}
			done[i] = true;
			std::size_t prev_j = i;
			std::size_t j = p[i];
			while (i != j) {
				std::swap(vec[prev_j], vec[j]);
				done[j] = true;
				prev_j = j;
				j = p[j];
			}
		}
	}
}


#endif //HYPERTRIE_PERMUTATIONSORT_HPP
