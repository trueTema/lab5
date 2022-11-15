#pragma once
#include "Sequence.h" 
#include "ArraySequence.h"
#include <type_traits>

template<typename _Key, typename _Value, class _Hash = std::hash<_Key>, class _cmp = std::equal_to<_Key>>
class HashTable {
private:
	size_t hash_capacity;
	size_t size;
	const double max_load_factor = 0.75;
	struct Elem {
		size_t hash;
		_Key key;
		_Value value;
		Elem() = default;
		Elem(const _Key& key, const _Value& value, const size_t hash) {
			this->key = key;
			this->hash = hash;
			this->value = value;
		}
	};
	LinkedList<Elem> HashList;
	DynamicArray<typename LinkedList<Elem>::iterator> HashVector;
public:
	HashTable() {
		hash_capacity = 1;
		size = 0;
		HashVector.Append(HashList.end());
	}

	void insert(const _Key& key, const _Value& value) {
		size_t hash = _Hash::_Do_hash(key);
		if (HashVector[hash % hash_capacity] == HashList.end())
		{
			HashList.push_back(Elem(_Key(), _Value(), hash));
			HashVector[hash % hash_capacity] = HashList.end() - 1;
		}
		HashList.InsertAt(Elem(key, value, hash), HashVector[hash % hash_capacity] + 1);
		size++;
		if ((double)size / (double)hash_capacity > max_load_factor) {
			rehash();
		}
	}

	void rehash() {
		size_t prev_capacity = hash_capacity;
		this->hash_capacity = (hash_capacity + 1) * 2;
		HashVector.Resize(hash_capacity);
		for (int i = prev_capacity; i < hash_capacity; i++) {
			HashVector[i] = HashList.end();
		}
		for (typename LinkedList<Elem>::iterator it = HashList.begin(); it != HashList.end(); it++) {
			if ((*HashVector[(*it).hash % hash_capacity]).hash != (*it).hash) {
				HashVector[(*it).hash % hash_capacity] = it;
				HashVector[(*it).hash % prev_capacity] = HashList.end();
			}
		}
	}

	const _Value& find(const _Key& key) {
		size_t hash = _Hash::_Do_hash(key);
		for (typename LinkedList<Elem>::iterator it = HashVector[hash % hash_capacity] + 1; it != HashList.end() && (*it).hash == hash; it++) {
			if ((*it).key == key) {
				return (*it).value;
			}
		}
		throw SetException(NoSuchElement);
	}

	size_t capacity() const noexcept {
		return size_t(hash_capacity * max_load_factor);
	}
};