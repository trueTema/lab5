#include <iostream>
#include "RBTree.h"
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include "Dictionary.h"
#include "HashDictionary.h"
#include <unordered_map>
#include "Histogram.h"
#include "Set.h"
#include "alphabet_pointer.h"
#include "Sparse_matrix.h"

using namespace std;


size_t __hash(int x) {
	return x % 1;
}

template<typename T>
struct comparator {
	bool operator()(const T& first, const T& second) {
		return first == second;
	}
};

template<typename T>
struct my_hash {
	size_t operator()(const T& first) {
		return first % 10;
	}
};

struct MyKey {
public:
	const char& operator ()(const string& x) const noexcept {
		return x[0];
	}
};

struct _key {
	int operator()(std::string x) const {
		return x.size();
	}
};


int main() {
	setlocale(LC_ALL, "Russian");
	srand(time(0));
	
	sparse_vector<int> sv({ 1,2,3,4 });
	sparse_vector<int> sv2 = sv;
	sv2.append(5);
	cout << (sv2.get(5));
	return 0;
}