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
#include <Windows.h>
#include "Sparse_matrix.h"
#include "menu.h"

using namespace std;

void print(const LinkedList<int>& list) {
	for (LinkedList<int>::const_iterator it = list.cbegin(); it != list.cend(); it++) {
		cout << *it << " ";
	}
}

void help() {
	cout << "\n";
	cout << "\tСписок команд: \n\n\t=======================================\n\n\thist - обратиться к меню Гистограммы\n\tmatrix - к меню разреженной матрицы\n\tapointer - к меню алфавитного указателя\n\n\t=======================================\n\n";
}


int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(0));
	string s;
	cout << "[Main menu]:\n>> ";
	getline(cin, s);
	while (s != "exit") {
		try {
			string cmd_t = _GetCommand(s);
			if (cmd_t == "hist") {
				s = histo_menu::receive(s.find(' ') != string::npos ? s.substr(s.find(' ') + 1, s.size() - cmd_t.size() - 1) : "");
			}
			else if (cmd_t == "apointer") {
				s = APointer_menu::receive(s.find(' ') != string::npos ? s.substr(s.find(' ') + 1, s.size() - cmd_t.size() - 1) : "");
			}
			else if (cmd_t == "matrix") {
				s = SparseMatrix_menu::receive(s.find(' ') != string::npos ? s.substr(s.find(' ') + 1, s.size() - cmd_t.size() - 1) : "");
			}
			else if (cmd_t == "help") {
				help();
			}
			else {
				throw SetException(UnknownCommand);
			}
		}
		catch (SetException e) {
			cout << e.message();
		}
		if (s == "exit") return 0;
		cout << "[Main menu]:\n>> ";
		getline(cin, s);
	}
	return 0;
}