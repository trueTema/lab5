#pragma once
#include "exception.h"

template<typename T, bool IsConst>
class BidirectionalIterator;

template<typename T>
class LinkedList;

template<typename T>
size_t distance(typename LinkedList<T>::iterator begin, typename LinkedList<T>::iterator end) {
	size_t res = 0;
	while (begin != end) {
		begin++;
		res++;
	}
	return res;
}



template<typename V>
struct ListIterators;

template <class T>
class LinkedList
{
	template<typename V, bool X>
	friend class BidirectionalIterator;
private:
	int size = 0;
	struct Item {
		Item* next = nullptr;
		Item* prev = nullptr;
		T data;
		Item() : data() {
			next = nullptr;
			prev = nullptr;
		}
		Item(const T& newdata) : data(newdata) {}
	};
	struct List {
		Item* head = nullptr;
		Item* tail = nullptr;
		List() {
			this->head = nullptr;
			this->tail = new Item();
		}
		~List() {
		}
	};

	List* items = nullptr;
	List* CreateList() {
		List* list;
		try {
			list = new List();
		}
		catch (std::bad_alloc) {
			throw SetException(MemoryAllocateError);
		}
		return list;
	}



	void push_back(T data) {
		Item* cur;
		try {
			cur = new Item(data);
		}
		catch (std::bad_alloc) {
			throw SetException(MemoryAllocateError);
		}
		if (size != 0) {
			cur->prev = items->tail->prev;
			items->tail->prev->next = cur;
			cur->next = items->tail;
			items->tail->prev = cur;
		}
		else {
			if (items->tail == nullptr)
				items->tail = new Item();
			cur->next = items->tail;
			items->tail->prev = cur;
			cur->prev = items->head;
			items->head = cur;
		}
		size++;
	}

	void push(T data, Item* item) {

		if (size == 0) {
			push_back(data);
			return;
		}
		Item* newitem;
		try {
			newitem = new Item(data);
		}
		catch (std::bad_alloc) {
			throw SetException(MemoryAllocateError);
		}
		newitem->next = item;
		newitem->prev = item->prev;

		if (item->prev != nullptr) {
			item->prev->next = newitem;
		}
		else {
			items->head = newitem;
		}
		item->prev = newitem;
		size++;
	}



	void delete_item(Item* item) {
		if (item == nullptr) {
			throw SetException(IndexOutOfRange);
		}

		if (item->prev != nullptr && item->next != items->tail) {
			item->prev->next = item->next;
			item->next->prev = item->prev;
		}
		//else if (cur->next == nullptr && cur->prev != nullptr) {
		//	items->tail = cur->prev;
		//	items->tail->next = nullptr;
		//}
		else if (item->prev == nullptr) {
			items->head = item->next;
			items->head->prev = nullptr;
		}
		else {
			items->tail->prev = item->prev;
			item->prev->next = items->tail;
		}

		delete item;
		size--;
	}

	void SetItem(int index, T item) {
		Item* cur = items->head;
		if (index < 0) {
			throw SetException(IndexOutOfRange);
		}
		for (int i = 0; cur != nullptr && i < index; i++) {
			cur = cur->next;
		}
		if (cur == nullptr) {
			throw SetException(IndexOutOfRange);
		}
		cur->data = item;
	}

	T& GetItem(int index) {
		Item* cur = items->head;
		if (index < 0) {
			throw SetException(IndexOutOfRange);
		}
		for (int i = 0; cur != nullptr && i < index; i++) {
			cur = cur->next;
		}
		if (cur == nullptr) {
			throw SetException(IndexOutOfRange);
		}
		return cur->data;
	}

	bool IsEqual(LinkedList<T>* other) {
		Item* cur = this->items->head;
		Item* cur2 = other->items->head;
		while (cur != nullptr && cur2 != nullptr) {
			if (cur->data != cur2->data) return false;
			cur = cur->next;
			cur2 = cur2->next;
		}
		if (cur != nullptr || cur2 != nullptr) {
			return false;
		}
		return true;
	}

	void push_tobegin(T data) {
		if (size == 0) {
			push_back(data);
			return;
		}
		Item* cur = nullptr;
		try {
			cur = new Item(data);
		}
		catch (std::bad_alloc) {
			throw SetException(MemoryAllocateError);
		}
		items->head->prev = cur;
		cur->next = items->head;
		cur->prev = nullptr;
		items->head = cur;
		size++;
	}

public:
	//���������
	using iterator = BidirectionalIterator<T, false>;
	using const_iterator = BidirectionalIterator<T, true>;
	iterator begin() {
		if (items->head == nullptr) {
			iterator iter(this->items->tail, this);
			return iter;
		}
		iterator iter(this->items->head, this);
		return iter;
	}
	const_iterator cbegin() const {
		if (items->head == nullptr) {
			const_iterator iter(this->items->tail, this);
			return iter;
		}
		const_iterator iter(this->items->head, this);
		return iter;
	}
	iterator end() {
		iterator iter(this->items->tail, this);
		return iter;
	}
	const_iterator cend() const {
		const_iterator iter(this->items->tail, this);
		return iter;
	}

	//������������
	LinkedList();
	LinkedList(size_t, T);
	LinkedList(T*, int);
	LinkedList(const LinkedList<T>&);
	LinkedList(std::initializer_list<T>);
	LinkedList(LinkedList<T>&&);

	//�������������
	~LinkedList();

	//�������
	T& GetFirst();
	T& GetLast();
	T& Get(int);
	int GetLength() const;
	void Append(T);
	void Prepend(T);
	void Set(int, T);
	void InsertAt(T, iterator);
	LinkedList<T>* GetSubList(iterator, iterator);
	LinkedList<T>* Concat(LinkedList<T>*);
	void del_item(iterator&);
	iterator find(iterator, iterator, T);
	LinkedList<T>* SplitList(bool(T));
	bool Equals(const LinkedList<T>&) const;
	bool IsSubList(LinkedList<T>*);
	void clear() {
		if (items == nullptr) return;
		Item* cur = items->head;
		while (cur != nullptr) {
			Item* cur2 = cur->next;
			if (cur2 != nullptr)
				cur2->prev = nullptr;
			delete cur;
			cur = cur2;
		}
		items->head = nullptr;
		items->tail = nullptr;
		size = 0;
	}
	//���������
	T& operator[](int);
	bool operator==(const LinkedList<T>&) const;
	bool operator!=(const LinkedList<T>&) const;

	LinkedList<T>& operator=(const LinkedList<T>& other) {
		if (this != &other) {
			clear();
			if (!items)
				delete items;
			size = 0;
			items = CreateList();
			if (other.GetLength() == 0) return *this;
			for (Item* it = other.items->head; it != other.items->tail; it = it->next) {
				this->push_back(it->data);
			}
			size = other.size;
		}
		return *this;
	}

	LinkedList<T>& operator=(LinkedList<T>&& other) {
		if (this != &other) {
			clear();
			if (!items)
				delete items;
			this->items = other.items;
			size = other.size;
			other.items = nullptr;
			other.size = 0;
		}
		
		return *this;
	}
};


template<typename T, bool IsConst>
class BidirectionalIterator {
	template<typename T> friend class LinkedList;
	template<typename T, class _Iterators> friend class Merge_Sort;
protected:
	using type = std::conditional_t<IsConst, const T&, T&>;
	typename LinkedList<T>::Item* item = nullptr;
	const LinkedList<T>* arr = nullptr;
public:
	BidirectionalIterator() = default;
	BidirectionalIterator(typename LinkedList<T>::Item* item, const LinkedList<T>* arr) : arr(arr) {
		this->item = &(*item);
	}
	BidirectionalIterator(const BidirectionalIterator<T, IsConst>& other) : arr(other.arr) {
		this->item = &(*other.item);
	}
	type operator *() {
		return item->data;
	}
	type operator *() const {
		return item->data;
	}
	BidirectionalIterator<T, IsConst>& operator++() {
		item = item->next;
		return *this;
	}
	BidirectionalIterator<T, IsConst>& operator--() {
		item = item->prev;
		return *this;
	}
	BidirectionalIterator<T, IsConst>& operator++(int) {
		item = item->next;
		return *this;
	}
	BidirectionalIterator<T, IsConst>& operator--(int) {
		item = item->prev;
		return *this;
	}

	BidirectionalIterator<T, IsConst>& operator+=(int t) {
		for (int i = 0; i < t; i++) {
			if (item->next == nullptr) {
				throw SetException(IncorrectRange);
			}
			item = item->next;
		}
		return *this;
	}
	BidirectionalIterator<T, IsConst>& operator-=(int t) {
		for (int i = 0; i < t; i++) {
			if (item->prev == nullptr) {
				throw SetException(IncorrectRange);
			}
			item = item->prev;
		}
		return *this;
	}
	BidirectionalIterator<T, IsConst> operator+(int t) {
		BidirectionalIterator<T, IsConst> newIterator(this->item, this->arr);
		for (int i = 0; i < t; i++) {
			newIterator.item = newIterator.item->next;
		}
		return newIterator;
	}
	BidirectionalIterator<T, IsConst> operator-(int t) {
		BidirectionalIterator<T, IsConst> newIterator(this->item, this->arr);
		for (int i = 0; i < t; i++) {
			newIterator.item = newIterator.item->prev;
		}
		return newIterator;
	}

	bool operator!=(const BidirectionalIterator<T, IsConst>& other) {
		if (this->arr != other.arr) return true;
		if (this->item != other.item) return true;
		return false;
	}
	bool operator==(const BidirectionalIterator<T, IsConst>& other) {
		if (this->arr != other.arr) return false;
		if (this->item != other.item) return false;
		return true;
	}
};


//������������
template<class T>
LinkedList<T>::LinkedList() {
	items = CreateList();
	//items->tail = new Item();
}

template<class T>
LinkedList<T>::LinkedList(size_t size, T value) {
	items = CreateList();
	for (int i = 0; i < size; i++) {
		push_back(value);
	}
	this->size = size;
}
template<class T>
LinkedList<T>::LinkedList(T* items, int count) {
	this->items = CreateList();

	for (int i = 0; i < count; i++) {
		push_back(items[i]);
	}
	size = count;
}
template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) {
	items = CreateList();
	
	for (Item* it = other.items->head; it != nullptr &&  it != other.items->tail; it = it->next) {
		this->push_back(it->data);
	}
	size = other.size;
}

template<class T>
LinkedList<T>::LinkedList(LinkedList<T>&& other) : items(nullptr), size(0) {
	items = other.items;
	size = other.size;
	other.items = nullptr;
	other.size = 0;
}

template<class T>
LinkedList<T>::LinkedList(std::initializer_list<T> list) {
	items = CreateList();
	for (auto i : list) {
		push_back(i);
	}
	size = int(list.size());
}


//�������������
template<class T>
LinkedList<T>::~LinkedList() {
	clear();
	delete items;
	items = nullptr;
}

//�������
template<class T>
T& LinkedList<T>::GetFirst() {
	return this->GetItem(0);
}

template<class T>
T& LinkedList<T>::GetLast() {
	return this->GetItem(this->size - 1);
}

template<class T>
T& LinkedList<T>::Get(int index) {
	return GetItem(index);
}

template<class T>
int LinkedList<T>::GetLength() const {
	return size;
}

template<class T>
void LinkedList<T>::Append(T item) {
	push_back(item);
}

template<class T>
void LinkedList<T>::Prepend(T item) {
	push_tobegin(item);
}

template<class T>
void LinkedList<T>::Set(int index, T item) {
	SetItem(index, item);
}

template<class T>
void LinkedList<T>::InsertAt(T item, iterator it) {
	Item* cur = it.item;

	push(item, cur);
}

template<class T>
LinkedList<T>* LinkedList<T>::GetSubList(iterator start, iterator end) {
	if (distance<T>(this->begin(), start) > distance<T>(this->begin(), end)) throw SetException(NegativeRange);
	LinkedList<T>* list = new LinkedList;
	for (iterator i = start; i != end; i++) {
		list->Append(*i);
	}
	return list;
}

template<class T>
LinkedList<T>* LinkedList<T>::Concat(LinkedList<T>* list) {
	LinkedList<T>* list_res = new LinkedList;
	for (int i = 0; i < size; i++) {
		list_res->Append(this->Get(i));
	}
	for (int i = 0; i < list->size; i++) {
		list_res->Append(list->Get(i));
	}
	return list_res;
}

template<class T>
void LinkedList<T>::del_item(iterator& it) {
	Item* cur = it.item;
	it.item = cur->next;
	delete_item(cur);
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::find(iterator start, iterator end, T item) {
	if (distance<T>(begin(), start) >= distance<T>(begin(), end)) throw SetException(NegativeRange);
	for (iterator i = start; i != end; i++) {
		if (*i == item) return i;
	}
	return this->end();
}

template<class T>
LinkedList<T>* LinkedList<T>::SplitList(bool cmp(T)) {
	LinkedList<T>* res = new LinkedList<T>;
	for (iterator i = this->begin(); i != this->end(); i++) {
		if (cmp(*i)) {
			res->Append(*i);
			this->del_item(i);
			i--;
		}
	}
	return res;
}

template<class T>
bool LinkedList<T>::Equals(const LinkedList<T>& seq) const {
	if (this->GetLength() != seq.GetLength()) return false;
	Item* i = this->items->head;
	Item* j = seq.items->head;
	for (; i != this->items->tail && j != seq.items->tail; i = i->next, j = j->next) {
		if (i->data != j->data) return false;
	}
	return true;
}

template<class T>
bool LinkedList<T>::IsSubList(LinkedList<T>* seq) {
	iterator pos = this->begin();
	for (iterator i = seq->begin(); i != seq->end() && pos != this->end(); i++) {
		pos = this->find(pos, this->end(), *i);
	}
	return pos != this->end();
}

//���������
template<class T>
T& LinkedList<T>:: operator[](int index) {
	return GetItem(index);
}

template<class T>
bool LinkedList<T>:: operator==(const LinkedList<T>& seq) const {
	return this->Equals(seq);
}

template<class T>
bool LinkedList<T>:: operator!=(const LinkedList<T>& seq) const {
	return !this->Equals(seq);
}