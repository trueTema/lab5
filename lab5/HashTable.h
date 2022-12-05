#pragma once
#include <type_traits>
#include <iostream>
#include "LinkedList.h"
#include "DynamicArray.h"

template<typename _Key, typename _Value, bool CanChangeValue = true, class _Hash = std::hash<_Key>, class _cmp = std::equal_to<_Key>>
class HashTable {
	template<typename Key, typename Value, bool CanChangeValue>
	friend class HashTableIterator;
	_cmp comparator;
	_Hash hasher;
private:
	size_t hash_capacity;
	size_t size;
	const double max_load_factor = 0.75;
	struct Elem {
		size_t tmp_hash;
		size_t real_hash;
		std::pair<const _Key, _Value> data;
		Elem() = default;
		Elem(const _Key& key, const _Value& value, const size_t hash, const size_t tmp) : data(key, value) {
			this->real_hash = hash;
			this->tmp_hash = tmp;
		}
		bool operator==(const Elem& other) {
			return other.real_hash == real_hash && comparator(other.data.first, data.first) && other.data.second == data.second;
		}
		bool operator!=(const Elem& other) {
			return !(other.real_hash == real_hash && comparator(other.data.first, data.first) && other.data.second == data.second);
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
			if ((HashVector[(*it).real_hash % hash_capacity] == HashList.end()) || (((*HashVector[(*it).real_hash % hash_capacity]).real_hash % hash_capacity) != ((*it).real_hash % hash_capacity))) {
				(*it).tmp_hash = (*it).real_hash % hash_capacity;
				HashList.Prepend(*it);
				HashVector[(*it).real_hash % hash_capacity] = HashList.begin();
				if (it == HashVector[(*it).real_hash % prev_capacity]) {
					HashVector[(*it).real_hash % prev_capacity] = HashList.end();
				}
				HashList.del_item(it);
			}
			else {
				(*it).tmp_hash = (*it).real_hash % hash_capacity;
				HashList.InsertAt(*it, HashVector[(*it).real_hash % hash_capacity]);
				HashVector[(*it).real_hash % hash_capacity]--;
				if (it == HashVector[(*it).real_hash % prev_capacity]) {
					HashVector[(*it).real_hash % prev_capacity] = HashList.end();
				}
				HashList.del_item(it);
			}
		}
	}
public:

	using iterator = HashTableIterator<_Key, _Value, CanChangeValue>;

	HashTable() {
		hash_capacity = 1;
		size = 0;
		HashVector.Append(HashList.end());
	}

	iterator begin() {
		typename LinkedList<Elem>::iterator* it = new typename LinkedList<Elem>::iterator(HashList.begin());
		iterator* it_res = reinterpret_cast<iterator*>(it);
		return *it_res;
	}

	iterator end() {
		typename LinkedList<Elem>::iterator* it = new typename LinkedList<Elem>::iterator(HashList.end());
		iterator* it_res = reinterpret_cast<iterator*>(it);
		return *it_res;
	}


	HashTable(const HashTable<_Key, _Value, CanChangeValue, _Hash, _cmp>& other) : HashList(other.HashList), HashVector(other.HashVector) {
		this->size = other.size;
		this->hash_capacity = other.hash_capacity;
	}

	void reserve(size_t n) {
		size_t required_size = size_t((double)n / max_load_factor) + 1;
		this->rehash(required_size);
		return;
	}

	void insert(const _Key& key, const _Value& value) {
		size_t hash = hasher(key);
		if (HashVector[hash % hash_capacity] == HashList.end())
		{
			HashList.Append(Elem(key, value, hash, hash % hash_capacity));
			HashVector[hash % hash_capacity] = HashList.end() - 1;
		}
		else {
			HashList.InsertAt(Elem(key, value, hash, hash % hash_capacity), HashVector[hash % hash_capacity]);
		}
		size++;
		if ((double)size / (double)hash_capacity > max_load_factor) {
			rehash((hash_capacity + 1) * 2);
		}
	}

	void remove(const _Key& key) {
		size_t hash = hasher(key);
		for (typename LinkedList<Elem>::iterator it = HashVector[hash % hash_capacity]; it != HashList.end() && (*it).tmp_hash == (hash % hash_capacity); it++) {
			if (comparator((*it).data.first, key)) {
				if (it == HashVector[hash % hash_capacity] && ((it + 1) == HashList.end() || (*(it + 1)).real_hash != (*it).real_hash)) {
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

	iterator find(const _Key& key) {
		size_t hash = hasher(key);
		for (typename LinkedList<Elem>::iterator it = HashVector[hash % hash_capacity]; it != HashList.end() && (*it).tmp_hash == (hash % hash_capacity); it++) {
			if (comparator((*it).data.first, key)) {
				typename LinkedList<Elem>::iterator* it_x = new typename LinkedList<Elem>::iterator(it);
				iterator* it_res = reinterpret_cast<iterator*>(it_x);
				return *it_res;
			}
		}
		return this->end();
	}

	size_t capacity() const noexcept {
		return size_t(hash_capacity * max_load_factor);
	}

	bool Equals(const HashTable<_Key, _Value, CanChangeValue, _Hash, _cmp>& other) {
		return this->HashList.IsSubList(other.HashList) && this->HashList.size == other.HashList.size;
	}

	bool operator==(const HashTable<_Key, _Value, CanChangeValue, _Hash, _cmp>& other) const noexcept {
		return this->Equals(other);
	}
	bool operator!=(const HashTable<_Key, _Value, CanChangeValue, _Hash, _cmp>& other) const noexcept {
		return !this->Equals(other);
	}

	std::conditional_t<CanChangeValue, _Value&, const _Value&> operator[](const _Key& key) {
		iterator it = find(key);
		if (it != this->end()) {
			return (*it).second;
		}
		else {
			this->insert(key, _Value());
			return (*find(key)).second;
		}
	}
};

template<typename _Key, typename _Value, bool CanChangeValue = true>
class HashTableIterator : public BidirectionalIterator<typename HashTable<_Key, _Value>::Elem, true> {
private:
	using thisiterator = BidirectionalIterator<typename HashTable<_Key, _Value>::Elem, true>;
public:
	HashTableIterator(const HashTableIterator<_Key, _Value>& other) : thisiterator(other) {}
	std::conditional_t<CanChangeValue, std::pair <const _Key, _Value>&, const std::pair <const _Key, _Value>&> operator*() {
		return this->item->data.data;
	}
};