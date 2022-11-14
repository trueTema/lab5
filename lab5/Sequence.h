#pragma once
#include "Iterators.h"

enum SequenceType { ArrayType, ListType, ZeroType };

template<typename T, class _Iterators>
class Sequence
{
public:
	using iterator = typename _Iterators::iterator;
	using const_iterator = typename _Iterators::const_iterator;
	virtual iterator begin() = 0;
	virtual iterator end() = 0;
	virtual const_iterator cbegin() = 0;
	virtual const_iterator cend() = 0;
	virtual T& GetFirst() = 0;
	virtual T& GetLast() = 0;
	virtual T& Get(int index) = 0;
	virtual Sequence<T, _Iterators>* GetSubSequence(iterator, iterator) = 0;
	virtual int GetLength() = 0;
	virtual void Append(T item) = 0;
	virtual void Prepend(T item) = 0;
	virtual void Set(int index, T item) = 0;
	virtual iterator find(iterator, iterator, T item) = 0;
	virtual void del_item(iterator) = 0;
	virtual void InsertAt(T item, iterator) = 0;
	virtual bool IsSubSeq(Sequence<T, _Iterators>* seq) = 0;
	virtual bool Equals(Sequence<T, _Iterators>* seq) = 0;
	virtual Sequence<T, _Iterators>* SplitSeq(bool(T)) = 0;
	virtual Sequence<T, _Iterators>* Concat(Sequence<T, _Iterators>* seq) = 0;
	virtual SequenceType GetType() = 0;
};