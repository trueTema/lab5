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




int main() {
	setlocale(LC_ALL, "Russian");
	srand(time(0));
	DynamicArray<DynamicArray<int>> d = { {1,2,3}, {0,2,0}, {1,2,3} };
	sparse_matrix<int> mat = { { -5 , 7 , 29 }, { -65 , -97 , -83 } };
	sparse_matrix<int> mat2 = { {-68, 69 }, { 95, 29 }, { 56, -13 } };
	mat *= mat2;
	cout << mat;
	return 0;
}