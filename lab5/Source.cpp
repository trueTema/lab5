#include <iostream>
#include "RBTree.h"
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include "Dictionary.h"
#include "HashTable.h"
#include <unordered_map>

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

int main() {
	srand(time(0));
	RBTree<int, int> rbt;
	rbt.insert(1, 1);
	rbt.insert(2, 4);
	rbt.insert(3, 9);
	rbt.insert(4, 16);
	for (RBTree<int, int>::iterator it = rbt.begin(); it != rbt.end(); it++) {
		pair<int,int> p = (*it);
		p.second = 107;
	}
	for (RBTree<int, int>::iterator it = rbt.begin(); it != rbt.end(); it++) {
		cout << (*it).first << " " << (*it).second << endl;
	}
	return 0;
}