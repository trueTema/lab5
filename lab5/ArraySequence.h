#include "Sequence.h"

template<typename T>
class ArraySequence : public Sequence<T, ArrayIterators<T>>
{
	template<typename V> friend class DynamicArray;
private:

protected:
	DynamicArray<T>* arr = nullptr;
public:
	using iterator = typename ArrayIterators<T>::iterator;
	using const_iterator = typename ArrayIterators<T>::const_iterator;
	iterator begin() override {
		return arr->begin();
	}
	const_iterator cbegin() override {
		return arr->cbegin();
	}
	iterator end() override {
		return arr->end();
	}
	const_iterator cend() override {
		return arr->cend();
	}

	ArraySequence();
	ArraySequence(size_t, T);
	ArraySequence(T*, int);
	ArraySequence(Sequence<T, ArrayIterators<T>>*);
	ArraySequence(std::initializer_list<T>);
	~ArraySequence();

	T& GetFirst() override;
	T& GetLast() override;
	T& Get(int) override;


	Sequence<T, ArrayIterators<T>>* GetSubSequence(iterator, iterator) override;
	int GetLength() override;
	void Append(T) override;
	void Prepend(T) override;
	void Set(int, T) override;
	void InsertAt(T, iterator) override;
	void del_item(iterator) override;
	Sequence<T, ArrayIterators<T>>* SplitSeq(bool(T)) override;
	Sequence<T, ArrayIterators<T>>* Concat(Sequence<T, ArrayIterators<T>>*) override;
	iterator find(iterator, iterator, T) override;
	bool IsSubSeq(Sequence<T, ArrayIterators<T>>*) override;
	bool Equals(Sequence<T, ArrayIterators<T>>*) override;
	SequenceType GetType() override;

	T& operator[] (int index);
	bool operator==(ArraySequence<T>&);
	bool operator!=(ArraySequence<T>&);
};

template<typename T>
ArraySequence<T>::ArraySequence() {
	arr = new DynamicArray<T>;
}

template<typename T>
ArraySequence<T>::ArraySequence(size_t size, T value) {
	arr = new DynamicArray<T>(size, value);
}

template<typename T>
ArraySequence<T>::ArraySequence(T* items, int count) {
	arr = new DynamicArray<T>(items, count);
}

template<typename T>
ArraySequence<T>::ArraySequence(Sequence<T, ArrayIterators<T>>* arr) {
	this->arr = new DynamicArray<T>(((ArraySequence<T>*)arr)->arr);
}

template<typename T>
ArraySequence<T>::ArraySequence(std::initializer_list<T> list) {
	this->arr = new DynamicArray<T>(list);
}


template<typename T>
ArraySequence<T>::~ArraySequence() {
	delete arr;
}

template<typename T>
T& ArraySequence<T>::GetFirst() {
	return this->arr->Get(0);
}

template<typename T>
T& ArraySequence<T>::GetLast() {
	return this->arr->Get(this->arr->GetSize() - 1);
}

template<typename T>
T& ArraySequence<T>::Get(int index) {
	return this->arr->Get(index);
}

template<typename T>
Sequence<T, ArrayIterators<T>>* ArraySequence<T>::GetSubSequence(iterator start, iterator end) {
	DynamicArray<T>* x = this->arr->GetSubArray(start, end);
	ArraySequence<T>* resseq = new ArraySequence<T>;
	delete resseq->arr;
	resseq->arr = x;
	return resseq;
}

template<typename T>
int ArraySequence<T>::GetLength() {
	return this->arr->GetSize();
}
template<typename T>
void ArraySequence<T>::Append(T item) {
	this->arr->Append(item);
}
template<typename T>
void ArraySequence<T>::Prepend(T item) {
	this->arr->Prepend(item);
}

template<typename T>
void ArraySequence<T>::Set(int index, T item) {
	this->arr->Set(index, item);
}

template<typename T>
void ArraySequence<T>::InsertAt(T item, iterator it) {
	this->arr->InsertAt(item, it);
}

template<typename T>
void ArraySequence<T>::del_item(iterator it) {
	this->arr->del_item(it);
}

template<typename T>
Sequence<T, ArrayIterators<T>>* ArraySequence<T>::SplitSeq(bool cmp(T)) {
	DynamicArray<T>* x = this->arr->SplitArray(cmp);
	ArraySequence<T>* resseq = new ArraySequence<T>;
	delete resseq->arr;
	resseq->arr = x;
	return resseq;
}

template<typename T>
Sequence<T, ArrayIterators<T>>* ArraySequence<T>::Concat(Sequence<T, ArrayIterators<T>>* seq) {
	DynamicArray<T>* x = this->arr->Concat(((ArraySequence<T>*)seq)->arr);
	ArraySequence<T>* resseq = new ArraySequence<T>;
	delete resseq->arr;
	resseq->arr = x;
	return resseq;
}

template<typename T>
typename ArraySequence<T>::iterator ArraySequence<T>::find(iterator start, iterator end, T item) {
	return this->arr->find(start, end, item);
}

template<typename T>
bool ArraySequence<T>::IsSubSeq(Sequence<T, ArrayIterators<T>>* seq) {
	return this->arr->IsSubArr(((ArraySequence<T>*)seq)->arr);
}

template<typename T>
bool ArraySequence<T>::Equals(Sequence<T, ArrayIterators<T>>* seq) {
	return this->arr->Equals(((ArraySequence<T>*)seq)->arr);
}

template<typename T>
SequenceType ArraySequence<T>::GetType() {
	return ArrayType;
}

template<typename T>
bool ArraySequence<T>:: operator==(ArraySequence<T>& seq) {
	return this->Equals(&seq);
}

template<typename T>
bool ArraySequence<T>:: operator!=(ArraySequence<T>& seq) {
	return !this->Equals(&seq);
}

template<typename T>
T& ArraySequence<T>:: operator[] (int index) {
	return this->Get(index);
}