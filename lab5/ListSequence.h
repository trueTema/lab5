#include "Sequence.h"

template<typename T>
class ListSequence : public Sequence<T, ListIterators<T>>
{
protected:
	LinkedList<T>* list;
public:
	using iterator = typename ListIterators<T>::iterator;
	using const_iterator = typename ListIterators<T>::const_iterator;
	iterator begin() override {
		return list->begin();
	}
	const_iterator cbegin() override {
		return list->cbegin();
	}
	iterator end() override {
		return list->end();
	}
	const_iterator cend() override {
		return list->cend();
	}

	ListSequence();
	ListSequence(size_t, T);
	ListSequence(T*, int);
	ListSequence(Sequence<T, ListIterators<T>>*);
	ListSequence(std::initializer_list<T>);

	~ListSequence();

	T& GetFirst() override;
	T& GetLast() override;
	T& Get(int) override;
	Sequence<T, ListIterators<T>>* GetSubSequence(iterator, iterator) override;
	int GetLength() override;
	void Set(int, T) override;
	void Append(T) override;
	void Prepend(T) override;
	void InsertAt(T, iterator) override;
	void del_item(iterator) override;
	iterator find(iterator, iterator, T) override;
	Sequence<T, ListIterators<T>>* SplitSeq(bool(T)) override;
	bool IsSubSeq(Sequence<T, ListIterators<T>>*) override;
	bool Equals(Sequence<T, ListIterators<T>>*) override;
	Sequence<T, ListIterators<T>>* Concat(Sequence<T, ListIterators<T>>*) override;
	SequenceType GetType() override;


	T& operator[](int);
	bool operator==(Sequence<T, ListIterators<T>>*);
	bool operator!=(Sequence<T, ListIterators<T>>*);
};

template<typename T>
ListSequence<T>::ListSequence() {
	list = new LinkedList<T>;
}

template<typename T>
ListSequence<T>::ListSequence(size_t size, T value) {
	list = new LinkedList<T>(size, value);
}

template<typename T>
ListSequence<T>::ListSequence(T* items, int count) {
	list = new LinkedList<T>(items, count);
}

template<typename T>
ListSequence<T>::ListSequence(Sequence<T, ListIterators<T>>* list) {
	this->list = new LinkedList<T>(((ListSequence<T>*)list)->list);
}

template<typename T>
ListSequence<T>::ListSequence(std::initializer_list<T> list) {
	this->list = new LinkedList<T>(list);
}


template<typename T>
ListSequence<T>::~ListSequence() {
	delete list;
}

template<typename T>
T& ListSequence<T>::GetFirst() {
	return this->list->GetFirst();
}
template<typename T>
T& ListSequence<T>::GetLast() {
	return this->list->GetLast();
}
template<typename T>
T& ListSequence<T>::Get(int index) {
	return this->list->Get(index);
}

template<typename T>
Sequence<T, ListIterators<T>>* ListSequence<T>::GetSubSequence(iterator start, iterator end) {
	LinkedList<T>* x = this->list->GetSubList(start, end);
	ListSequence<T>* resseq = new ListSequence<T>;
	delete resseq->list;
	resseq->list = x;
	return resseq;
}

template<typename T>
int ListSequence<T>::GetLength() {
	return this->list->GetLength();
}

template<typename T>
void ListSequence<T>::Set(int index, T item) {
	this->list->Set(index, item);
}

template<typename T>
void ListSequence<T>::Append(T item) {
	this->list->Append(item);
}
template<typename T>
void ListSequence<T>::Prepend(T item) {
	this->list->Prepend(item);
}
template<typename T>
void ListSequence<T>::InsertAt(T item, iterator it) {
	this->list->InsertAt(item, it);
}

template<typename T>
void ListSequence<T>::del_item(iterator it) {
	this->list->del_item(it);
}

template<typename T>
typename ListSequence<T>::iterator ListSequence<T>::find(iterator start, iterator end, T item) {
	return this->list->find(start, end, item);
}

template<typename T>
Sequence<T, ListIterators<T>>* ListSequence<T>::SplitSeq(bool (cmp)(T)) {
	LinkedList<T>* x = this->list->SplitList(cmp);
	ListSequence<T>* resseq = new ListSequence<T>;
	delete resseq->list;
	resseq->list = x;
	return resseq;
}

template<typename T>
bool ListSequence<T>::IsSubSeq(Sequence<T, ListIterators<T>>* seq) {
	return this->list->IsSubList(((ListSequence<T>*)seq)->list);
}

template<typename T>
bool ListSequence<T>::Equals(Sequence<T, ListIterators<T>>* seq) {
	return this->list->Equals(((ListSequence<T>*)seq)->list);
}

template<typename T>
Sequence<T, ListIterators<T>>* ListSequence<T>::Concat(Sequence<T, ListIterators<T>>* seq) {
	LinkedList<T>* x = this->list->Concat(((ListSequence<T>*)seq)->list);
	ListSequence<T>* resseq = new ListSequence<T>;
	delete resseq->list;
	resseq->list = x;
	return resseq;
}

template<typename T>
SequenceType ListSequence<T>::GetType() {
	return ListType;
}

template<typename T>
bool ListSequence<T>::operator==(Sequence<T, ListIterators<T>>* seq) {
	return this->Equals(seq);
}
template<typename T>
bool ListSequence<T>::operator!=(Sequence<T, ListIterators<T>>* seq) {
	return !this->Equals(seq);
}

template <typename T>
T& ListSequence<T>:: operator[] (int index) {
	return this->Get(index);
}