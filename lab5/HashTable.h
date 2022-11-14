#pragma once
#include "Sequence.h" 

template<typename _Key, size_t Hash(_Key)>
class HashTable {
private:
	size_t capacity;
	size_t size;
	
public:

};