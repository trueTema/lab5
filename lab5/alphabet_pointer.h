#pragma once
#include "LinkedList.h"
#include <string>
#include "Dictionary.h"

class APointer {
private:
	Dictionary<std::string, LinkedList<size_t>> dict;
	std::string text;
	const std::string restricted = "!\"#$%&’()*+,-./:;<=>?@[]^_`{|}~. \n\t";
	const size_t page_size;
	size_t page_num = 1;
	LinkedList<std::string>* split(const std::string::const_iterator& begin, const std::string::const_iterator& end) const noexcept {
		LinkedList<std::string>* list = new LinkedList<std::string>();
		for (std::string::const_iterator it = begin; it != end; it++) {
			if (restricted.find(*it) != std::string::npos) {
				while (it != end && restricted.find(*it) != std::string::npos) it++;
				if (it == end) return list;
				list->Append(std::string());
			}
			if (list->GetLength() == 0) {
				list->Append(std::string());
			}
			(*(list->end()-1)).push_back(*it);
		}
		return list;
	}
	void place_to_dict(LinkedList<std::string>* w_list) {
		size_t cur_size = 0;
		for (LinkedList<std::string>::const_iterator it = w_list->cbegin(); it != w_list->cend(); it++) {
			dict[*it].Append(page_num);
			cur_size++;
			if (cur_size >= page_size) {
				page_num++;
				cur_size = 0;
			}
		}
	}
public:
	APointer() = delete;
	APointer(const std::string& str, size_t page_size) : page_size(page_size) {
		text = str;
		if (page_size < 1) throw SetException(IncorrectValue);
		place_to_dict(split(str.cbegin(), str.cend()));
	}
	LinkedList<size_t> get_pages(const std::string& word) noexcept {
		LinkedList<size_t> res(dict[word]);
		return res;
	}
	void print() noexcept {
		for (Dictionary<std::string, LinkedList<size_t>>::iterator it = dict.begin(); it != dict.end(); it++) {
			std::cout << "\t" << (*it).first << "\n\t<";
			for (LinkedList<size_t>::const_iterator itl = (*it).second[0].cbegin(); itl != (*it).second[0].cend(); itl++) {
				std::cout << *itl;
				if (itl != (*it).second[0].cend() - 1) std::cout << ", ";
			}
			std::cout << ">\n";
			std::cout << std::endl;
		}
	}
	void add(const std::string& text) {
		this->text += text;
		place_to_dict(split(text.cbegin(), text.cend()));
	}
	void remove(const std::string& text) {
		size_t check = this->text.find(text);
		while (check != std::string::npos) {
			this->text.erase(check, text.size());
			check = this->text.find(text);
		}
		dict.clear();
		page_num = 1;
		place_to_dict(split(this->text.cbegin(), this->text.cend()));
	}
};