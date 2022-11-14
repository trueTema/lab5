#pragma once
#include "DynamicArray.h"
#include "LinkedList.h"
template<typename V>
struct ListIterators {
	using iterator = typename LinkedList<V>::iterator;
	using const_iterator = typename LinkedList<V>::const_iterator;
};

template<typename V>
struct ArrayIterators {
	using iterator = typename DynamicArray<V>::iterator;
	using const_iterator = typename DynamicArray<V>::const_iterator;
};
