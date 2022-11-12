#include <iostream>
#include "RBTree.h"
#include <string>
#include <vector>
#include <chrono>

using namespace std;

int main() {
	srand(time(0));
	RBTree<int, int> rbt;
	rbt.insert(0, 0);
	rbt.insert(1, 1);
	rbt.insert(2, 2);
	rbt.insert(3, 3);
	rbt.insert(4, 4);
	cout << rbt.check();
	/*for (int i = 0; i < 100; i++) {
		rbt.insert(i, i);
	}
	for (int i = 30; i >= 0; i--) {
		cout << rbt.get(i) << endl;
		rbt.remove(i);
	}
	for (int i = 31; i < 70; i++) {
		cout << rbt.get(i) << endl;
		rbt.remove(i);
	}
	for (int i = 99; i >= 70; i--) {
		cout << rbt.get(i) << endl;
		rbt.remove(i);
	}*/
	vector<std::pair<int, int>> vec;
	for (int i = 0; i < 10000; i++) {
		vec.push_back(make_pair(rand() % 20000 - 10000, rand() % 20000 - 10000));
		rbt.insert(vec[i].first, vec[i].second);
	}
	cout << endl << rbt.check();
	//for (int i = 0; i < 10000; i++) {
	//	int key = vec[i].first;
	//	int value = vec[i].second;
	//	cout << "test: " << i + 1 << " " << key << " " << value << endl;
	//	//try {
	//		rbt.remove(key);
	//		//if (rbt.get(key) != value) {
	//		//	cout << "Lox";
	//		//	return 0;
	//		//}
	//	/* }
	//	catch (...) {
	//		cout << "failed test: " << i + 1 << " " << key << " " << value << endl;
	//		return 0;
	//	}*/
	//}
	//cout << endl << endl << rbt.get_height() << endl;
	return 0;
}