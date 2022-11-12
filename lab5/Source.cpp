#include <iostream>
#include "RBTree.h"
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include "Dictionary.h"

using namespace std;


int f(const int a, const int b) {
	return a + b;
}

struct x {
	int a;
	x(int a) {
		this->a = a;
	}
};

int main() {
	srand(time(0));
	RBTree<int> rbt1;
	rbt1.insert(1);
	rbt1.insert(2);
	rbt1.insert(3);
	rbt1.insert(4);
	rbt1.insert(5);
	RBTree<int> rbt2(rbt1);
	cout << rbt2.get(3);

	/*Dictionary<int, int> mp;
	mp.emplace(1, 5);
	mp.emplace(2, 6);
	mp.emplace(3, 7);
	mp.emplace(4, 8);
	cout << mp.get(3);*/
	return 0;
}