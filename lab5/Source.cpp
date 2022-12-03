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
	sparse_matrix<int> mat = { {1,2,3},{1,2,3} };
	mat.add_column({ 1,2 });
	mat.remove_column(2);
	cout << mat;
	return 0;
}