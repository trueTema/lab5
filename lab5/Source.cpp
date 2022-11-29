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

using namespace std;


size_t __hash(int x) {
	return x % 1;
}

struct x {
	int a;
	x(int a) {
		this->a = a;
	}
	x() = default;
	friend ostream& operator<<(ostream& os, const x& a);
};

ostream& operator<<(ostream& os, const x& a) {
	os << a.a;
	return os;
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
	srand(time(0));
	Histogram<string, char, MyKey> hist({ 'a','b','c' });
	hist.add("aa");
	hist.add("abd");
	hist.add("asd");
	hist.add("bdsa");
	hist.add("ff");
	hist.add("cc");
	hist.print();
	hist.describe();
	return 0;
}