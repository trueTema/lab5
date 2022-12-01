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
	setlocale(LC_ALL, "Russian");
	srand(time(0));
	APointer a("привет, меня зовут артём. давай знакомиться?", 2);
	a.add("\n- А меня гена!");
	a.remove("меня");
	a.print();
	return 0;
}