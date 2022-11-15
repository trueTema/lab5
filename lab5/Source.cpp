#include <iostream>
#include "RBTree.h"
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include "Dictionary.h"
#include "HashTable.h"

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

int main() {
	srand(time(0));
	
	return 0;
}