#pragma once
#include "DynamicArray.h"
#include "Auxillary.h"
#include <string>
#include "Histogram.h"

using namespace Traits;


template<typename _Obj, typename _KeyType = _Obj, typename _Key = MyKeyGetter<_Obj>, typename _Cmp = MyComparator<_Obj>>
class HistoMenu {
private:
	Histogram<_Obj, _KeyType, _Key, _Cmp>* hist = nullptr;
public:
	HistoMenu() = default;
	HistoMenu(const std::initializer_list<_KeyType>& list) {
		hist = new Histogram<_Obj, _KeyType, _Key, _Cmp>(list);
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
		else {
			throw SetException(UnknownCommand);
		}
	}
	std::string listener() {
		std::string cmd;
		getline(std::cin, cmd);
		while (_GetCommand(cmd) != "back" && _GetCommand(cmd) != "create") {
			std::string cmdt = _GetCommand(cmd);
			DynamicArray<Argument<std::string>>* arr = _MakeArgumentList<std::string>(cmd, false);
			Process(cmdt, arr);
			getline(std::cin, cmd);
		}
		return cmd;
	}
	~HistoMenu() = default;
};

class HistoListener {
public:
	static void receive() {
		std::string cmd;
		return;
	}
};