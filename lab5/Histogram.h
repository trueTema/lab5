#pragma once
#include "HashDictionary.h"
//#define DynamicArray<statistics<_Obj, _KeyType, _Key, _cmp>> std::vector<statistics<_Obj, _KeyType, _Key, _cmp>>
#include "Set.h"

//template<typename T>
//struct MyComparator {
//	short int operator()(const T& first, const T& second) const noexcept {
//		if (first < second) return -1;
//		if (first > second) return 1;
//		return 0;
//	}
//};

template<typename T>
struct MyKeyGetter {
	const T& operator()(const T& elem) const noexcept {
		return elem;
	}
};

template<class _Obj, typename _KeyType, class _Key, class _cmp>
struct statistics {
private:
	const _Key _key_getter;
	MultiSet<_KeyType, _cmp> ms;
	void update() {
		max = ms.get_max();
		min = ms.get_min();
		median = ms.get_median();
		count = ms.count();
	}
public:
	statistics() = default;
	~statistics() = default;

	_KeyType max;
	_KeyType min;
	//const _Obj& up_quantile;
	_KeyType median;
	//const _Obj& down_quantile;
	int count;
	void add(const _Obj& obj) {
		ms.insert(_key_getter(obj));
	}
	void remove(const _Obj& obj) {
		ms.remove(_key_getter(obj));
	}
	void calculate() {
		try {
			update();
		}
		catch (SetException e) {
			if (e.id != EmptySequence) {
				throw e;
			}
			else {
				max = _KeyType();
				min = _KeyType();
				median = _KeyType();
			}
		}
	}
	statistics(const LinkedList<_Obj>& list) {
		for (typename LinkedList<_Obj>::const_iterator it = list.begin(); it != list.end(); it++) {
			ms.insert(_key_getter(*it));
		}
		update();
	}
	statistics<_Obj, double, _Key, _cmp>& operator=(const statistics<_Obj, _KeyType, _Key, _cmp>& other) {
		this->max = other.max;
		this->min = other.min;
		this->ms = other.ms;
		this->median = other.median;
		return *this;
	}
	template<class _O, typename _K, class _Ky, class _C>
	friend std::ostream& operator <<(std::ostream& os, const statistics<_O, _K, _Ky, _C>);
};
template<class _Obj, class _Key, class _cmp>
struct statistics<_Obj, int, _Key, _cmp> {
private:
	using _KeyType = int;
	const _Key _key_getter;
	MultiSet<_KeyType, _cmp> ms;
	void update() {
		max = ms.get_max();
		min = ms.get_min();
		median = ms.get_median();
		count = ms.count();
	}
public:
	statistics() = default;
	~statistics() = default;

	int max;
	int min;
	//const int up_quantile;
	int median;
	//const int down_quantile;
	int count;
	double average;
	void add(const _Obj& obj) {
		ms.insert(_key_getter(obj));
		average = (average * count + _key_getter(obj)) / (count + 1);
		count++;
	}
	void remove(const _Obj& obj) {
		ms.remove(_key_getter(obj));
		average = (average * count - _key_getter(obj)) / (count - 1);
		count--;
	}
	void calculate() {
		try {
			update();
		}
		catch (SetException e) {
			if (e.id != EmptySequence) {
				throw e;
			}
			else {
				max = 0;
				min = 0;
				average = 0;
				median = 0;
			}
		}
	}
	statistics(const LinkedList<_Obj>& list) {
		for (typename LinkedList<_Obj>::const_iterator it = list.begin(); it != list.end(); it++) {
			ms.insert(_key_getter(*it));
			average += _key_getter(*it);
		}
		average /= list.GetLength();
		update();
	}
	statistics<_Obj, int, _Key, _cmp>& operator=(const statistics<_Obj, int, _Key, _cmp>& other) {
		this->max = other.max;
		this->min = other.min;
		this->ms = other.ms;
		this->average = other.average;
		this->median = other.median;
		return *this;
	}
};
template<class _Obj, class _Key, class _cmp>
struct statistics<_Obj, double, _Key, _cmp> {
private:
	using _KeyType = double;
	const _Key _key_getter;
	MultiSet<_KeyType, _cmp> ms;
	void update() {
		max = ms.get_max();
		min = ms.get_min();
		median = ms.get_median();
		count = ms.count();
	}
public:
	double max;
	double min;
	//double up_quantile;
	double median;
	//double down_quantile;
	int count;
	double average;
	statistics() = default;
	~statistics() = default;
	void add(const _Obj& obj) {
		ms.insert(_key_getter(obj));
		average = (average * count + _key_getter(obj)) / (count + 1);
		count++;
	}
	void remove(const _Obj& obj) {
		ms.remove(_key_getter(obj));
		average = (average * count - _key_getter(obj)) / (count - 1);
		count--;
	}
	void calculate() {
		try {
			update();
		}
		catch (SetException e) {
			if (e.id != EmptySequence) {
				throw e;
			}
			else {
				max = 0;
				min = 0;
				average = 0;
				median = 0;
			}
		}
	}
	statistics(const LinkedList<_Obj>& list) {
		for (typename LinkedList<_Obj>::const_iterator it = list.begin(); it != list.end(); it++) {
			ms.insert(_key_getter(*it));
			average += _key_getter(*it);
		}
		average /= list.GetLength();
		update();
	}
	statistics<_Obj, double, _Key, _cmp>& operator=(const statistics<_Obj, double, _Key, _cmp>& other) {
		this->max = other.max;
		this->min = other.min;
		this->average = other.average;
		this->ms = other.ms;
		this->median = other.median;
		return *this;
	}
	void print() {
		try {
			calculate();
		}
		catch (SetException e) {
			if (e.id != EmptySequence) {
				throw e;
			}
		}
		std::cout << "Min: " << min << " Max: " << max << " Median: " << median << " Count: " << count << " Average: " << average << "\n";
	}
};

template<class _O, typename _K, class _Ky, class _C>
std::ostream& operator <<(std::ostream& os, statistics<_O, _K, _Ky, _C>& st) {
	st.calculate();

	os << "Min: " << st.min << " Max: " << st.max << " Median: " << st.median << " Count: " << st.count << "\n";
	return os;
}

template<class _O, class _Ky, class _C>
std::ostream& operator <<(std::ostream& os, statistics<_O, int, _Ky, _C>& st) {
	st.calculate();
	os << "Min: " << st.min << " Max: " << st.max << " Median: " << st.median << " Count: " << st.count << " Average: " << st.average << "\n";
	return os;
}

template<class _O, class _Ky, class _C>
std::ostream& operator <<(std::ostream& os, statistics<_O, double, _Ky, _C>& st) {
	st.calculate();

	os << "Min: " << st.min << " Max: " << st.max << " Median: " << st.median << " Count: " << st.count << " Average: " << st.average << "\n";
	return os;
}

template<class _Obj, typename _KeyType = _Obj, class _Key = MyKeyGetter<_Obj>, class _cmp = MyComparator<_KeyType>>
class Histogram {
private:
	const _Key _key_getter;
	const _cmp comparator;
	HashDictionary<_KeyType, LinkedList<_Obj>> hd;
	std::vector<statistics<_Obj, _KeyType, _Key, _cmp>> stats;
	DynamicArray<_KeyType> bins;
	int find_place(const _Obj& obj) {
		const _KeyType& key = _key_getter(obj);
		if (key > bins[bins.GetSize() - 1]) return -1;
		if (key < bins[0]) return -1;
		int left = -1;
		int right = bins.GetSize();
		int ans = (left + right) / 2;
		while (left <= right) {
			int mid = (left + right) / 2;
			if (comparator(key, bins[mid]) == 0) {
				if (mid == bins.GetSize() - 1) mid--;
				return mid;
			}
			else if (comparator(key, bins[mid]) < 0) {
				right = mid - 1;
			}
			else {
				ans = mid;
				left = mid + 1;
			}
		}
		if (ans == bins.GetSize() - 1) ans--;
		return ans;
	}
public:
	Histogram() = delete;
	Histogram(const std::initializer_list<_KeyType>& bins) {
		if (bins.size() < 2) throw SetException(IncorrectRange);
		hd.reserve(bins.size());
		this->bins = bins;
		stats.resize(bins.size() - 1);
		for (auto i : bins) {
			hd[i] = LinkedList<_Obj>();
		}
	}
	Histogram(const DynamicArray<_Obj>& vector, const DynamicArray<_KeyType>& bins) {
		if (bins.size() < 2) throw SetException(IncorrectRange);
		hd.reserve(bins.size());
		this->bins = bins;
		stats.resize(bins.size() - 1);
		for (typename DynamicArray<_Obj>::const_iterator it = vector.cbegin(); it != vector.cend(); it++) {
			int pos = find_place(*it);
			if (pos == -1) continue;
			hd[bins[pos]].Append(*it);
		}
		for (int i = 0; i < bins.GetSize(); i++) {
			stats[i] = hd[bins[i]];
		}
	}
	~Histogram() = default;


	template<class _O, typename _K, class _Ky, class _C>
	friend std::ostream& operator<<(std::ostream&, Histogram<_O, _K, _Ky, _C>&);

	void describe() {
		std::cout << "\tRange\tStatistics\n\n\n";
		for (int it = 0; it < (bins).GetSize() - 1; it++) {
			std::cout << "\t" << (bins)[it] << " - " << (bins)[it + 1] << "\t";
			std::cout << (stats)[it];
			std::cout << "\n\n";
		}
	}

	const std::vector<statistics<_Obj, _KeyType, _Key, _cmp>>& get_stats() const noexcept {
		return stats;
	}

	void add(const _Obj& object) {
		int pos = find_place(object);
		if (pos == -1) return;
		hd[bins[pos]].Append(object);
		stats[pos].add(object);
	}
	void remove(const _Obj& object) {
		int pos = find_place(object);
		if (pos == -1) return;
		typename LinkedList<_Obj>::iterator it = hd[bins[pos]].find(hd[bins[pos]].begin(), hd[bins[pos]].end(), object);
		hd[bins[pos]].del_item(it);
		stats[pos].remove(object);
	}
};

template<class _O, typename _K, class _Ky, class _C>
std::ostream& operator<<(std::ostream& os, Histogram<_O, _K, _Ky, _C>& hist) {
	os << "\tRange\tObjects\n\n\n";
	for (typename DynamicArray<_K>::const_iterator it = hist.bins.cbegin(); it != (hist.bins.cend() - 1); it++) {
		os << "\t" << *it << " - " << *(it + 1) << "\t";
		for (typename LinkedList<_O>::const_iterator it_l = hist.hd[*it].cbegin(); it_l != hist.hd[*it].cend(); it_l++) {
			os << *it_l;
			if (it_l != (hist.hd[*it].cend() - 1)) os << ",";
			os << " ";
		}
		os << "\n\n";
	}
	return os;
}