#include <iostream>
#include "RBTree.h"
#include <string>
#include <vector>
#include <chrono>

using namespace std;


int f(const int a, const int b) {
	return a + b;
}

int main() {
	srand(time(0));
	RBTree<int, int> rbt;

	vector<std::pair<int, int>> vec, vec2;
	for (int i = 0; i < 10000; i++) {
		int key = rand() % 20000 - 10000;
		int value = rand() % 20000 - 10000;
		if (!rbt.find(key)) {
			vec.push_back(make_pair(key, value));
			rbt.insert(key, value);
		}
	}
	for (int i = 0; i < vec.size(); i++) {
		int key = vec[i].first;
		int value = vec[i].second;
		//cout << "test: " << i + 1 << " " << key << " " << value << endl;
		try {
			if (rbt.get(key) != value) {
				cout << "Lox";
				return 0;
			}
			rbt.remove(key);
			rbt.insert(key, value);
		}
		catch (...) {
			cout << "failed test: " << i + 1 << " " << key << " " << value << endl;
			return 0;
		}
	}
	for (int i = 0; i < vec.size(); i++) {
		int key = vec[i].first;
		int value = vec[i].second;
		try {
			rbt.remove(key);
		}
		catch (...) {
			cout << "failed test: " << i + 1 << " " << key << " " << value << endl;
			return 0;
		}
	}

	cout << endl << endl << rbt.get_height() << endl << endl << rbt.check();
	return 0;
}