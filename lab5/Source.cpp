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
#include "menu.h"

using namespace std;

struct x {
	string name;
	int age;
	int salary;
	x() = default;
	x(string n, int a, int s) {
		name = n;
		age = a;
		salary = s;
	}
	int& operator[](int a) {
		return age;
	}
	const int& operator[](int a) const {
		return age;
	}
	friend std::ostream& operator<<(std::ostream&, const x&);
};

ostream& operator<<(std::ostream& os, const x& o) {
	os << "< " << o.name << " " << o.age << " " << o.salary << " >";
	return os;
}

struct key {
	int operator()(const x& o) const {
		return o.salary;
	}
};

void change(x& a) {
	a[0] = 21;
}

void change(const x& a) {
	cout << a[0];
}

int main() {
	setlocale(LC_ALL, "Russian");
	srand(time(0));
	Histogram<x, int, key> hist({ 1,1000,10000,100000 });
	hist.add(x("aptem", 19, 19000));
	cout << hist;
	return 0;
}