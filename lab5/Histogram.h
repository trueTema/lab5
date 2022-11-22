#pragma once
#include "HashDictionary.h"

template<typename T>
struct MyComparator {
	short int operator()(const T& first, const T& second) const noexcept {
		if (first < second) return -1;
		if (first > second) return 1;
		return 0;
	}
};

template<typename T>
struct MyKeyGetter {
	T operator()(const T& elem) const noexcept {
		return elem;
	}
};

template<typename T>
struct statistics {
public:
	const T max;
	const T min;
	const T up_quantile;
	const T mean;
	const T down_quantile;
	const int count;
};
template<>
struct statistics<int> {
public:
	const int max;
	const int min;
	const int up_quantile;
	const int mean;
	const int down_quantile;
	const int count;
	const double average;
};
template<>
struct statistics<double> {
public:
	const double max;
	const double min;
	const double up_quantile;
	const double mean;
	const double down_quantile;
	const int count;
	const double average;
};



template<class _Obj, typename _KeyType = _Obj, class _Key = MyKeyGetter<_Obj>, class _cmp = MyComparator<_Obj>>
class Histogram {
private:
	const _Key _key_getter;
	const _cmp comparator;
	HashDictionary<_KeyType, LinkedList<_Obj>> hd;
	DynamicArray<statistics<_Obj>> stats;
	DynamicArray<_KeyType> bins;
	_KeyType find_place(const _Obj& obj) {
		size_t left = 0;
		size_t right = bins.GetSize();
		size_t ans = (left + right) / 2;
		while (left < right) {
			size_t mid = (left + right) / 2;
			if (comparator(_key_getter(obj), bins[mid]) == 0) {
				return _key_getter(bins[mid]);
			}
			else if (comparator(_key_getter(obj), bins[mid]) < 0) {
				right = mid - 1;
			}
			else {
				ans = mid;
				left = mid + 1;
			}
		}
		return _key_getter(bins[ans]);
	}
public:
	Histogram() = delete;
	Histogram(const std::initializer_list<_KeyType>& bins) {
		hd.reserve(bins.size());
		this->bins = bins;
		for (auto i : bins) {
			hd[i] = LinkedList<_Obj>();
		}
	}
	Histogram(const DynamicArray<_Obj>& vector, const DynamicArray<_KeyType>& bins) {
		hd.reserve(bins.size());
		this->bins = bins;
		for (typename DynamicArray<_Obj>::const_iterator it = vector.cbegin(); it != vector.cend(); it++) {
			hd[find_place(*it)].Append(*it);
		}
	}
	~Histogram() = default;
	//void plot(); //??????????????????????????????????????????

	void print() {
		std::cout << "\tRange\tObjects\n\n\n";
		for (typename DynamicArray<_KeyType>::const_iterator it = bins.cbegin(); it != bins.cend(); it++) {
			std::cout << "\t" << *it << "\t";
			for (typename LinkedList<_Obj>::const_iterator it_l = hd[*it].cbegin(); it_l != hd[*it].cend(); it_l++) {
				
				std::cout << *it_l;
				if (it_l != (hd[*it].cend() - 1)) std::cout << ",";
				std::cout << " ";
			}
			std::cout << "\n\n";
		}
	}

	void add(const _Obj& object) {
		hd[find_place(object)].Append(object);
	}
};