#pragma once
#include "DynamicArray.h"
#include <string>
#include "Histogram.h"

using namespace Traits;

namespace histo_menu {

	template<typename T>
	struct mod_getter {
		int operator()(T x) const {
			return int(x) % 10;
		}
	};

	struct len_getter {
		int operator()(const std::string& x) const {
			return x.size();
		}
	};

	struct age_getter {
		int operator()(const Person& x) const {
			return x.age;
		}
	};

	struct salary_getter {
		int operator()(const Person& x) const {
			return x.salary;
		}
	};


	void help() {
		cout << "\n";
		cout << "\tСписок команд: \n\n\t[] - обязательные аргументы, {} - опциональные аргументы\n\n\t=======================================\n\n\tcreate [-o тип_объекта] {-k тип_ключа} {-g ключ} {-e (создать пустую гистограмму)} - создать гистограму\n\tprint - вывести гистограмму\n\tdescribe - вывести статистику по каждому столбцу\n\tadd [-i элемент] - добавить элемент в гистограмму\n\tremove [-i элемент] - удалить элемент из гистограммы\n\n\t=======================================\n\n";
	}

	template<typename _Obj, typename _KeyType = _Obj, typename _Key = MyKeyGetter<_Obj>, typename _Cmp = MyComparator<_KeyType>>
	class HistoMenu {
	private:
		Histogram<_Obj, _KeyType, _Key, _Cmp>* hist = nullptr;
		template<typename fillT>
		void fill_bins(DynamicArray<fillT>& da, std::string str) {
			std::string cur;
			for (int i = 0; i < str.size(); i++) {
				if (str[i] != ' ') cur += str[i];
				else {
					da.Append(_TypeCast<fillT>::cast(cur));
					cur = "";
				}
			}
			if (cur != "")
				da.Append(_TypeCast<fillT>::cast(cur));
		}
	public:
		HistoMenu(bool f) {
			DynamicArray<_KeyType> da;
			DynamicArray<_Obj> obj;
			std::string str;
			std::cout << "Enter partition:\n>> ";
			std::getline(std::cin, str);
			fill_bins<_KeyType>(da, str);
			if (f) {
				std::cout << "Enter items:\n>> ";
				std::getline(std::cin, str);
				fill_bins<_Obj>(obj, str);
			}
			hist = new Histogram<_Obj, _KeyType, _Key, _Cmp>(obj, da);
		}
		void Process(std::string cmd, DynamicArray<Argument<string>>* arr) {
			if (cmd == "describe") {
				if (arr->GetSize() != 0) throw SetException(UnknownFlag);
				hist->describe();
			}
			else if (cmd == "print") {
				if (arr->GetSize() != 0) throw SetException(UnknownFlag);
				std::cout << *hist;
			}
			else if (cmd == "add") {
				if (arr->GetSize() > 1) throw SetException(UnknownFlag);
				if (arr->Get(0).GetFlag() != 'i') throw SetException(UnknownFlag);
				_Obj obj = _TypeCast<_Obj>::cast(arr->Get(0).Getvalue());
				hist->add(obj);
			}
			else if (cmd == "remove") {
				if (arr->GetSize() > 1) throw SetException(UnknownFlag);
				if (arr->Get(0).GetFlag() != 'i') throw SetException(UnknownFlag);
				_Obj obj = _TypeCast<_Obj>::cast(arr->Get(0).Getvalue());
				hist->remove(obj);
			}
			else if (cmd == "help") {
				help();
			}
			else {
				throw SetException(UnknownCommand);
			}
		}

		~HistoMenu() = default;
	};

	template<typename _Obj, typename _KeyType = _Obj, typename _Key = MyKeyGetter<_Obj>, typename _Cmp = MyComparator<_KeyType>>
	std::string main_listener(bool f) {
		HistoMenu<_Obj, _KeyType, _Key, _Cmp> hm(f);
		std::string cmd;
		std::cout << "[Histogram menu]:\n>> ";
		getline(std::cin, cmd);
		std::string cmd_t = _GetCommand(cmd);
		while (cmd_t != "back" && cmd_t != "create" && cmd_t != "exit") {
			DynamicArray<Argument<std::string>>* arr = _MakeArgumentList<std::string>(cmd, false);
			try {
				hm.Process(cmd_t, arr);
			}
			catch (SetException e) {
				cout << e.message();
			}
			std::cout << "[Histogram menu]:\n>> ";
			getline(std::cin, cmd);
			cmd_t = _GetCommand(cmd);
		}
		return cmd;
	}

	std::string receive(std::string cmd) {
		while (_GetCommand(cmd) != "back" && _GetCommand(cmd) != "exit") {
			DynamicArray<Argument<std::string>>* arr = _MakeArgumentList<std::string>(cmd, false);
			if (_GetCommand(cmd) == "create") {
				enum objType { Int, Float, String, Person, NAO };
				enum getter { Age, Salary, Obj, Mod, Length, NAG };
				objType obj = NAO;
				objType keytype = obj;
				getter keygetter = Obj;
				bool f = true;
				for (int i = 0; i < arr->GetSize(); i++) {
					if (arr->Get(i).GetFlag() == 'o') {
						std::string x = arr->Get(i).Getvalue();
						if (x == "int") {
							obj = Int;
						}
						else if (x == "float") {
							obj = Float;
						}
						else if (x == "string") {
							obj = String;
						}
						else if (x == "person") {
							obj = Person;
						}
						else throw SetException(IncorrectValue);
					}
					else if (arr->Get(i).GetFlag() == 'k') {
						std::string x = arr->Get(i).Getvalue();
						if (x == "int") {
							keytype = Int;
						}
						else if (x == "float") {
							keytype = Float;
						}
						else if (x == "string") {
							keytype = String;
						}
						else throw SetException(IncorrectValue);
					}
					else if (arr->Get(i).GetFlag() == 'g') {
						std::string x = arr->Get(i).Getvalue();
						if (x == "age") {
							keygetter = Age;
						}
						else if (x == "salary") {
							keygetter = Salary;
						}
						else if (x == "obj") {
							keygetter = Obj;
						}
						else if (x == "mod") {
							keygetter = Mod;
						}
						else if (x == "len") {
							keygetter = Length;
						}
						else throw SetException(IncorrectValue);
					}
					else if (arr->Get(i).GetFlag() == 'e') {
						if (arr->Get(i).Getvalue() != "") throw SetException(IncorrectValue);
						f = false;
					}
					else {
						throw SetException(UnknownFlag);
					}
				}
				if (obj == NAO) {
					throw SetException(NoRequiredArgument);
				}
				if (keytype == NAO) keytype = obj;
				cmd = "";
				try {
					if (obj == Int) {
						if (keytype == Int) {
							if (keygetter == Obj) {
								cmd = main_listener<int, int>(f);
							}
							else if (keygetter == Mod) {
								cmd = main_listener<int, int, mod_getter<int>>(f);
							}
							else throw SetException(IncorrectValue);
						}
						else {
							throw SetException(IncorrectValue);
						}
					}
					else if (obj == Float) {
						if (keytype == Float) {
							if (keygetter == Obj) {
								cmd = main_listener<double, double>(f);
							}
							else if (keygetter == Mod) {
								cmd = main_listener<double, double, mod_getter<double>>(f);
							}
							else throw SetException(IncorrectValue);
						}
						else {
							throw SetException(IncorrectValue);
						}
					}
					else if (obj == String) {
						if (keytype == Int) {
							if (keygetter == Length) {
								cmd = main_listener<std::string, int, len_getter>(f);
							}
							else throw SetException(IncorrectValue);
						}
						else if (keytype == String) {
							if (keygetter == Obj) {
								cmd = main_listener<std::string, std::string>(f);
							}
							else throw SetException(IncorrectValue);
						}
						else {
							throw SetException(IncorrectValue);
						}
					}
					else if (obj == Person) {
						if (keytype == Int) {
							if (keygetter == Age) {
								cmd = main_listener<struct Person, int, age_getter>(f);
							}
							else if (keygetter == Salary) {
								cmd = main_listener<struct Person, int, salary_getter>(f);
							}
							else throw SetException(IncorrectValue);
						}
						else {
							throw SetException(IncorrectValue);
						}
					}
				}
				catch (SetException e) {
					std::cout << e.message();
					std::cout << "[Histogram menu]:\n>> ";
					getline(std::cin, cmd);
				}
			}
			else if (cmd == "help") {
				help();
				std::cout << "[Histogram menu]:\n>> ";
				getline(std::cin, cmd);
			}
			else if (cmd == "") {
				std::cout << "[Histogram menu]:\n>> ";
				getline(std::cin, cmd);
			}
			else {
				throw SetException(EmptySequence);
			}
		}
		return cmd;
	}

};

namespace APointer_menu {
	void help() {
		cout << "\n";
		cout << "\tСписок команд: \n\n\t[] - обязательные аргументы, {} - опциональные аргументы\n\n\t=======================================\n\n\tcreate {-s (размер страницы исчисляется в символах)} {-e (создать пустой указатель)} - создать указатель\n\tprint - вывести указатель\n\tadd [-i слово] - добавить слово в указатель\n\tremove [-i слово] - удалить слово из указателя\n\tadd_text - добавить текст в указател\n\tremove_text - удалить текст из указателя\n\n\t=======================================\n\n";
	}

	template<bool IsWords>
	class APMenu {
	private:
		APointer<IsWords>* ap = nullptr;
	public:
		APMenu(bool f) {
			std::string str;
			std::cout << "Enter page size:\n>> ";
			int page_size = 0;
			getline(std::cin, str);
			page_size = _TypeCast<int>::cast(str);
			if (page_size < 1) throw SetException(IncorrectValue);
			str = "";
			if (f) {
				std::cout << "Enter text:\n>> ";
				std::getline(std::cin, str);
			}
			ap = new APointer<IsWords>(str, page_size);
		}
		void Process(std::string cmd, DynamicArray<Argument<string>>* arr) {
			if (cmd == "print") {
				if (arr->GetSize() != 0) throw SetException(UnknownFlag);
				ap->print();
			}
			else if (cmd == "add") {
				if (arr->GetSize() > 1) throw SetException(UnknownFlag);
				if (arr->Get(0).GetFlag() != 'i') throw SetException(UnknownFlag);
				ap->add(arr->Get(0).Getvalue());
			}
			else if (cmd == "add_text") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				std::string str;
				getline(std::cin, str);
				ap->add(str);
			}
			else if (cmd == "remove") {
				if (arr->GetSize() > 1) throw SetException(UnknownFlag);
				if (arr->Get(0).GetFlag() != 'i') throw SetException(UnknownFlag);
				ap->remove(arr->Get(0).Getvalue());
			}
			else if (cmd == "help") {
				help();
			}
			else if (cmd == "remove_text") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				std::string str;
				getline(std::cin, str);
				ap->remove(str);
			}
			else if (cmd == "get_pages") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				std::string str;
				getline(std::cin, str);
				LinkedList<size_t> ls = ap->get_pages(str);
				cout << "\n\t<";
				for (LinkedList<size_t>::const_iterator it = ls.cbegin(); it != ls.cend(); it++) {
					std::cout << *it;
					if (it != ls.cend() - 1) std::cout << ", ";
				}
				cout << ">\n\n";
			}
			else {
				throw SetException(UnknownCommand);
			}
		}

		~APMenu() = default;
	};

	template<bool IsWords>
	std::string main_listener(bool f) {
		APMenu<IsWords> apm(f);
		std::string cmd;
		std::cout << "[APointer menu]:\n>> ";
		getline(std::cin, cmd);
		std::string cmd_t = _GetCommand(cmd);
		while (cmd_t != "back" && cmd_t != "create" && cmd_t != "exit") {
			DynamicArray<Argument<std::string>>* arr = _MakeArgumentList<std::string>(cmd, false);
			try {
				apm.Process(cmd_t, arr);
			}
			catch (SetException e) {
				cout << e.message();
			}
			std::cout << "[APointer menu]:\n>> "; 
			getline(std::cin, cmd);
			cmd_t = _GetCommand(cmd);
		}
		return cmd;
	}

	std::string receive(std::string cmd) {
		while (_GetCommand(cmd) != "back" && _GetCommand(cmd) != "exit") {
			DynamicArray<Argument<std::string>>* arr = _MakeArgumentList<std::string>(cmd, false);
			if (_GetCommand(cmd) == "create") {
				enum getter { Sym, Words };
				getter get = Words;
				bool f = true;
				for (int i = 0; i < arr->GetSize(); i++) {
					if (arr->Get(i).GetFlag() == 's') {
						if (arr->Get(i).Getvalue() != "") throw SetException(IncorrectValue);
						get = Sym;
					}
					else if (arr->Get(i).GetFlag() == 'e') {
						if (arr->Get(i).Getvalue() != "") throw SetException(IncorrectValue);
						f = false;
					}
					else {
						throw SetException(UnknownFlag);
					}
				}
				cmd = "";
				while (_GetCommand(cmd) != "back" && _GetCommand(cmd) != "create"&& _GetCommand(cmd) != "exit") {
					try {
						if (get == Words) {
							cmd = main_listener<true>(f);
						}
						else if (get == Sym) {
							cmd = main_listener<false>(f);
						}
					}
					catch (SetException e) {
						std::cout << e.message();
					}
				}
			}
			else if (cmd == "") {
				std::cout << "[APointer menu]:\n>> ";
				getline(std::cin, cmd);
			}
			else if (cmd == "help") {
				help();
				std::cout << "[APointer menu]:\n>> ";
				getline(std::cin, cmd);
			}
			else {
				throw SetException(EmptyAP);
			}
		}
		return cmd;
	}
}

namespace SparseMatrix_menu {

	void help() {
		cout << "\n";
		cout << "\tСписок команд: \n\n\t[] - обязательные аргументы, {} - опциональные аргументы\n\n\t=======================================\n\n\tcreate {-t тип} {-e (создать пустую матрицу)} - создать матрицу\n\tprint - вывести матрицу\n\tadd_scal, add_matr, multiply_scal, multiply_left, multiply_right - арифметические операции над матрицей\n\tadd_col - добавить столбец в матрицу\n\tremove_col [-n номер] - удалить столбец из матрицы\n\tadd_row - добавить строку в матрицу\n\tremove_row [-n номер] - удалить строку из матрицы\n\n\t=======================================\n\n";
	}

	template<typename fillT>
	void fill_bins(DynamicArray<fillT>& da, std::string str) {
		std::string cur;
		for (int i = 0; i < str.size(); i++) {
			if (str[i] != ' ') cur += str[i];
			else {
				da.Append(_TypeCast<fillT>::cast(cur));
				cur = "";
			}
		}
		if (cur != "")
			da.Append(_TypeCast<fillT>::cast(cur));
	}

	template<typename T>
	sparse_mmatrix<T>* make_matrix(bool f, int hor, int ver) {
		if (hor < 0) throw SetException(IncorrectValue);
		if (ver < 0) throw SetException(IncorrectValue);
		sparse_mmatrix<T>* sm = new sparse_mmatrix<T>(hor, ver);
		std::string str = "";
		if (f) {
			for (int i = 0; i < hor; i++) {
				DynamicArray<T> row;
				std::cout << "Enter row:\n>> ";
				std::getline(std::cin, str);
				fill_bins<T>(row, str);
				if (row.GetSize() != ver) throw SetException(IncorrectRange);
				for (int j = 0; j < ver; j++) {
					sm->set(i + 1, j + 1, row[j]);
				}
			}
		}
		return sm;
	}
	

	template<typename T>
	class SMMenu {
	private:
		sparse_mmatrix<T>* sm = nullptr;
		
	public:
		SMMenu(bool f) {
			std::cout << "\nEnter size:\n>> ";
			DynamicArray<T> arr;
			std::string str;
			getline(std::cin, str);
			fill_bins(arr, str);
			if (arr.GetSize() != 2) throw SetException(IncorrectValue);
			int hor = arr[0];
			int ver = arr[1];
			sm = make_matrix<T>(f, hor, ver);
		}
		void Process(std::string cmd, DynamicArray<Argument<string>>* arr) {
			if (cmd == "print") {
				if (arr->GetSize() != 0) throw SetException(UnknownFlag);
				std::cout << *sm;
			}
			else if (cmd == "set") {
				if (arr->GetSize() > 1) throw SetException(UnknownFlag);
				if (arr->GetSize() == 0) throw SetException(NoRequiredArgument);
				if (arr->Get(0).GetFlag() != 'i') throw SetException(UnknownFlag);
				std::cout << "\nEnter coordinates:\n>> ";
				DynamicArray<T> coords;
				std::string str;
				getline(std::cin, str);
				fill_bins(coords, str);
				if (coords.GetSize() != 2) throw SetException(IncorrectValue);
				sm->set(coords[0], coords[1], _TypeCast<T>::cast(arr->Get(0).Getvalue()));
			}
			else if (cmd == "add_row") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				std::string str;
				std::cout << "Enter row:\n>> ";
				std::getline(std::cin, str);
				DynamicArray<T> row;
				fill_bins<T>(row, str);
				sm->add_row(row);
			}
			else if (cmd == "add_col") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				std::string str;
				std::cout << "Enter column:\n>> ";
				std::getline(std::cin, str);
				DynamicArray<T> row;
				fill_bins<T>(row, str);
				sm->add_column(row);
			}
			else if (cmd == "remove_col") {
				if (arr->GetSize() != 1) throw SetException(UnknownFlag);
				if (arr->Get(0).GetFlag() != 'n') throw SetException(UnknownFlag);
				sm->remove_column(_TypeCast<int>::cast(arr->Get(0).Getvalue()));
			}
			else if (cmd == "remove_row") {
				if (arr->GetSize() != 1) throw SetException(UnknownFlag);
				if (arr->Get(0).GetFlag() != 'n') throw SetException(UnknownFlag);
				sm->remove_row(_TypeCast<int>::cast(arr->Get(0).Getvalue()));
			}
			else if (cmd == "add_scal") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				std::string str;
				std::cout << "Enter value:\n>>";
				getline(std::cin, str);
				T val = _TypeCast<T>::cast(str);
				*sm += val;
			}
			else if (cmd == "add_matr") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				sparse_mmatrix<T>* other = make_matrix<T>(true, sm->get_ver(), sm->get_hor());
				*sm += *other;
			}
			else if (cmd == "multiply_scal") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				std::string str;
				std::cout << "Enter value:\n>>";
				getline(std::cin, str);
				T val = _TypeCast<T>::cast(str);
				*sm *= val;
			}
			else if (cmd == "multiply_left") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				sparse_mmatrix<T>* other = make_matrix<T>(true, sm->get_ver(), sm->get_hor());
				*sm = (*other) * (*sm);
			}
			else if (cmd == "multiply_right") {
				if (arr->GetSize() > 0) throw SetException(UnknownFlag);
				sparse_mmatrix<T>* other = make_matrix<T>(true, sm->get_ver(), sm->get_hor());
				*sm = (*sm) * (*other);
			}
			else if (cmd == "help") {
				help();
			}
			else {
				throw SetException(UnknownCommand);
			}
		}
		~SMMenu() = default;
	};

	template<typename T>
	std::string main_listener(bool f) {
		SMMenu<T> apm(f);
		std::string cmd;
		std::cout << "[Sparse matrix menu]:\n>> ";
		getline(std::cin, cmd);
		std::string cmd_t = _GetCommand(cmd);
		while (cmd_t != "back" && cmd_t != "create" && cmd_t != "exit") {
			DynamicArray<Argument<std::string>>* arr = _MakeArgumentList<std::string>(cmd, false);
			try {
				apm.Process(cmd_t, arr);
			}
			catch (SetException e) {
				cout << e.message();
			}
			std::cout << "[Sparse matrix menu]:\n>> ";
			getline(std::cin, cmd);
			cmd_t = _GetCommand(cmd);
		}
		return cmd;
	}

	std::string receive(std::string cmd) {
		while (_GetCommand(cmd) != "back" && _GetCommand(cmd) != "exit") {
			DynamicArray<Argument<std::string>>* arr = _MakeArgumentList<std::string>(cmd, false);
			if (_GetCommand(cmd) == "create") {
				enum type { Int, Float };
				type T = Int;
				bool f = true;
				for (int i = 0; i < arr->GetSize(); i++) {
					if (arr->Get(i).GetFlag() == 't') {
						if (arr->Get(i).Getvalue() == "int") {
							T = Int;
						}
						else if (arr->Get(i).Getvalue() == "double") {
							T = Float;
						}
						else {
							throw SetException(IncorrectValue);
						}
					}
					else if (arr->Get(i).GetFlag() == 'e') {
						if (arr->Get(i).Getvalue() != "") throw SetException(IncorrectValue);
						f = false;
					}
					else {
						throw SetException(UnknownFlag);
					}
				}
				cmd = "";
					try {
						if (T == Int) {
							cmd = main_listener<int>(f);
						}
						else if (T == Float) {
							cmd = main_listener<double>(f);
						}
					}
					catch (SetException e) {
						std::cout << e.message();
					}
			}
			else if (cmd == "") {
				std::cout << "[Sparse matrix menu]:\n>> ";
				getline(std::cin, cmd);
			}
			else if (cmd == "help") {
				help();
				std::cout << "[Sparse matrix menu]:\n>> ";
				getline(std::cin, cmd);
			}
			else {
				throw SetException(EmptyAP);
			}
		}
		return cmd;
	}
}