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
	HashTable<int, int, true, my_hash<int>, comparator<int>> ht;
	ht.insert(1, 1);
	ht.insert(2, 4);
	ht.insert(3, 9);
	ht.insert(13, 10);
	ht.insert(23, 11);
	ht.insert(33, 12);
	ht.insert(43, 13);
	ht.insert(53, 14);
	for (HashTable<int, int, true, my_hash<int>, comparator<int>>::iterator it = ht.begin(); it != ht.end(); it++) {
		cout << (*it).first << " " << (*it).second << endl;
	}
	cout << endl << endl;
	cout << ht.find(3) << endl;
	cout << ht.find(13) << endl;
	cout << ht.find(23) << endl;
	cout << ht.find(33) << endl;
	cout << ht.find(43) << endl;
	cout << ht.find(53) << endl;
	return 0;
}