#pragma once
#include "HashTable.h"

template<typename _Key, typename _Value>
class HashDictionary : public HashTable<_Key, _Value, true> {
public:
	using iterator = typename HashTable<_Key, _Value, true>::iterator;
	HashDictionary() : HashTable<_Key, _Value, true>() {}
	HashDictionary(const HashDictionary<_Key, _Value>& other) : HashTable<_Key, _Value, true>(other) {}
	~HashDictionary() = default;
};