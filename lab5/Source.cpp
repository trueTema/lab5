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

template<typename T>
void time() {
	DynamicArray<double> time_d;
	DynamicArray<double> time_d_del;
	DynamicArray<double> time_hd;
	DynamicArray<double> time_hd_del;
	for (int cnt = 1; cnt <= 10; cnt++) {
		double average_insert_d = 0;
		double average_insert_d_del = 0;
		double average_insert_hd = 0;
		double average_insert_hd_del = 0;
		for (int k = 0; k < 5; k++) {
			Dictionary<T, int> dict;
			HashDictionary<T, int> hdict;
			DynamicArray<T> da;
			for (int i = 0; i < cnt * 500; i++) {
				da.Append(random_unit<T>::generate());
			}
			auto start = std::chrono::system_clock::now();
			for (int i = 0; i < cnt*500; i++) {
				dict.insert(da[i], 1);
			}
			auto end = std::chrono::system_clock::now();
			average_insert_d += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			start = std::chrono::system_clock::now();
			for (int i = 0; i < cnt * 500; i++) {
				hdict.insert(da[i], 1);
			}
			end = std::chrono::system_clock::now();
			average_insert_hd += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			start = std::chrono::system_clock::now();
			for (int i = 0; i < cnt * 500; i++) {
				if (dict.find(da[i]))
					dict.remove(da[i]);
			}
			end = std::chrono::system_clock::now();
			average_insert_d_del += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			start = std::chrono::system_clock::now();
			for (int i = 0; i < cnt * 500; i++) {
				if (hdict.find(da[i]) != hdict.end())
					hdict.remove(da[i]);
			}
			end = std::chrono::system_clock::now();
			average_insert_hd_del += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		}
		time_d.Append(average_insert_d / 5);
		time_hd.Append(average_insert_hd / 5);
		time_d_del.Append(average_insert_d_del / 5);
		time_hd_del.Append(average_insert_hd_del / 5);
	}
	cout << "\nRBT Dictionary:\n\t";
	for (int i = 0; i < 10; i++) {
		cout << "Count: " << (i + 1) * 500 << " Insert:\t" << time_d[i] << "\tDeleting:\t" << time_d_del[i];
		cout << "\n\n\t";
	}
	cout << "\Hash Dictionary:\n\t";
	for (int i = 0; i < 10; i++) {
		cout << "Count: " << (i + 1) * 500 << " Insert:\t" << time_hd[i] << "\tDeleting:\t" << time_hd_del[i];
		cout << "\n\n\t";
	}
}

void help() {
	cout << "\n";
	cout << "\tСписок команд: \n\n\t=======================================\n\n\thist - обратиться к меню Гистограммы\n\tmatrix - к меню разреженной матрицы\n\tapointer - к меню алфавитного указателя\n\ttime {-t тип} - сравнительный замер времени работы Hash - словаря и RBT - словаря в реальном времени\n\n\t=======================================\n\n";
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
			else if (cmd_t == "time") {
				DynamicArray<Argument<string>>* da = _MakeArgumentList<string>(s, false);
				if (da->GetSize() > 1) throw SetException(UnknownFlag);
				int type = 0;
				if (da->GetSize() == 1) {
					if (da->Get(0).GetFlag() == 't') {
						if (da->Get(0).Getvalue() == "int") {
							type = 0;
						}
						else if (da->Get(0).Getvalue() == "float") {
							type = 1;
						}
						else if (da->Get(0).Getvalue() == "string") {
							type = 2;
						}
						else throw SetException(IncorrectValue);
					}
					else throw SetException(UnknownFlag);
				}
				if (type == 0) {
					time<int>();
				}
				else if (type == 1) {
					time<double>();
				}
				else {
					time<string>();
				}
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