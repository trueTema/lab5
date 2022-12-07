#pragma once
#include "LinkedList.h"
#include <string>
#include "Dictionary.h"

template<bool IsWords = true>
class APointer {
private:
	Dictionary<std::string, LinkedList<size_t>> dict;
	std::string text;
	const std::string restricted = "!\"#$%&’()*+,-./:;<=>?@[]^_`{|}~.";
	const size_t page_size;
	size_t page_num = 1;
	size_t cur_size = 0;
	LinkedList<std::string>* split(const std::string::const_iterator& begin, const std::string::const_iterator& end) const noexcept {
		LinkedList<std::string>* list = new LinkedList<std::string>();
		for (std::string::const_iterator it = begin; it != end; it++) {
			if (*it == ' ' || *it == '\n' || *it == '\t') {
				while (it != end && (*it == ' ' || *it == '\n' || *it == '\t')) it++;
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

	void delete_restricted(std::string& str) {
		for (std::string::iterator it = str.begin(); it != str.end();) {
			if (restricted.find(*it) != std::string::npos) {
				str.erase(it);
			}
			else it++;
		}
	}

	void place_to_dict(LinkedList<std::string>* w_list) {
		for (LinkedList<std::string>::iterator it = w_list->begin(); it != w_list->end(); it++) {
			delete_restricted(*it);
			if ((*it).size() == 0) continue;
			if (!IsWords) {
				if ((*it).size() > (page_num == 1 ? page_size / 2 : page_num % 10 == 0 ? page_size*3/4:page_size) && cur_size == 0) page_num--;
				cur_size += (*it).size();
				if (cur_size > (page_num == 1 ? page_size / 2 : page_num % 10 == 0 ? page_size * 3 / 4 : page_size)) {
					page_num++;
					cur_size = (*it).size();
				}
				else cur_size++;
			}
			else {
				cur_size += 1;
				if (cur_size > (page_num == 1 ? page_size / 2 : page_num % 10 == 0 ? page_size * 3 / 4 : page_size)) {
					page_num++;
					cur_size = 1;
				}
			}
			dict[*it].Append(page_num);
		}
	}
public:
	APointer() = delete;
	APointer(size_t page_size) :page_size(page_size) {}
	APointer(const std::string& str, size_t page_size) : page_size(page_size) {
		if (str.size() == 0) return;
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
		if (text.size() == 0) return;
		this->text += " " + text;
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
		cur_size = 0;
		place_to_dict(split(this->text.cbegin(), this->text.cend()));
	}
};