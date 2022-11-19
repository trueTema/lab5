#pragma once
#include <type_traits>
#include "LinkedList.h"
#include "DynamicArray.h"

template<typename _Key, typename _Value, class _Hash = std::hash<_Key>, class _cmp = std::equal_to<_Key>, bool CanChangeValue = true>
class HashTable {
	template<typename Key, typename Value>
	friend class HashTableIterator;
	_cmp comparator;
	_Hash hasher;
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
		bool operator==(const Elem& other) {
			return other.hash == hash && comparator(other.key, key) && other.value == value;
		}
		bool operator!=(const Elem& other) {
			return !(other.hash == hash && comparator(other.key, key) && other.value == value);
		}
	};
	LinkedList<Elem> HashList;
	DynamicArray<typename LinkedList<Elem>::iterator> HashVector;

	void rehash(size_t newsize) {
		size_t prev_capacity = hash_capacity;
		this->hash_capacity = newsize;
		HashVector.Resize(hash_capacity);
		for (int i = prev_capacity; i < hash_capacity; i++) {
			HashVector[i] = HashList.end();
		}
		for (typename LinkedList<Elem>::iterator it = HashList.begin(); it != HashList.end(); ) {
			if ((HashVector[(*it).hash % hash_capacity] == HashList.end()) || ((*HashVector[(*it).hash % hash_capacity]).hash != (*it).hash)) {
				HashList.push_tobegin(*it);
				HashVector[(*it).hash % hash_capacity] = HashList.begin();
				if (it == HashVector[(*it).hash % prev_capacity]) {
					HashVector[(*it).hash % prev_capacity] = HashList.end();
				}
				HashList.del_item(it);
			}
			else it++;
		}
	}
public:

	using iterator = HashTableIterator<_Key, _Value>;

	HashTable() {
		hash_capacity = 1;
		size = 0;
		HashVector.Append(HashList.end());
	}

	iterator begin() {
		typename LinkedList<Elem>::const_iterator it = HashList.cbegin();
		iterator res = HashList.cbegin();
		return res;
	}

	iterator end() {
		typename LinkedList<Elem>::const_iterator it = HashList.cend();
		iterator res = HashList.cend();
		return res;
	}

	void insert(const _Key& key, const _Value& value) {
		size_t hash = hasher(key);
		if (HashVector[hash % hash_capacity] == HashList.end())
		{
			HashList.push_back(Elem(key, value, hash));
			HashVector[hash % hash_capacity] = HashList.end() - 1;
		}
		else {
			HashList.InsertAt(Elem(key, value, hash), HashVector[hash % hash_capacity]);
		}
		size++;
		if ((double)size / (double)hash_capacity > max_load_factor) {
			rehash((hash_capacity + 1) * 2);
		}
	}

	void remove(const _Key& key) {
		size_t hash = hasher(key);
		for (typename LinkedList<Elem>::iterator it = HashVector[hash % hash_capacity]; it != HashList.end() && (*it).hash == hash; it++) {
			if (comparator((*it).key, key)) {
				if (it == HashVector[hash % hash_capacity] && ((it + 1) == HashList.end() || (*(it + 1)).hash != (*it).hash)) {
					HashVector[hash % hash_capacity] = HashList.end();
				}
				HashList.del_item(it);
				this->size--;
				if ((double)size / (double)(hash_capacity / 2 - 1) <= max_load_factor / 2) {
					rehash(hash_capacity / 2 - 1);
				}
				return;
			}
		}
		throw SetException(NoSuchElement);
	}

	std::conditional_t<CanChangeValue, _Value&, const _Value&> find(const _Key& key) {
		size_t hash = hasher(key);
		for (typename LinkedList<Elem>::iterator it = HashVector[hash % hash_capacity]; it != HashList.end() && (*it).hash == hash; it++) {
			if (comparator((*it).key, key)) {
				return (*it).value;
			}
		}
		throw SetException(NoSuchElement);
	}

	size_t capacity() const noexcept {
		return size_t(hash_capacity * max_load_factor);
	}
};

template<typename _Key, typename _Value>
class HashTableIterator {
	using thisiterator = typename LinkedList<typename HashTable<_Key, _Value>::Elem>::const_iterator;
private:
	thisiterator it;
public:
	HashTableIterator(const HashTableIterator<_Key, _Value>& other) : it(other.it) {}
	HashTableIterator(const thisiterator& other) : it(other) {}
	const std::pair<_Key, _Value>& operator*() {
		return std::make_pair((*it).key, (*it).value);
	}
	HashTableIterator<_Key, _Value>& operator++() {
		it++;
		return *this;
	}
	HashTableIterator<_Key, _Value>& operator--() {
		it--;
		return *this;
	}
	HashTableIterator<_Key, _Value>& operator++(int) {
		it++;
		return *this;
	}
	HashTableIterator<_Key, _Value>& operator--(int) {
		it--;
		return *this;
	}
	bool operator==(const HashTableIterator<_Key, _Value>& other) {
		return (this->it) == (other.it);
	}
	bool operator!=(const HashTableIterator<_Key, _Value>& other) {
		return (this->it) != (other.it);
	}
};