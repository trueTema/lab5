#pragma once
#include "RBTree.h"

template<typename _Key, typename _Value>
class Dictionary {
private:
	RBTree<my_pair<_Key, _Value>> rbt;
public:
	Dictionary() = default;
	Dictionary(const Dictionary<_Key, _Value>& other) : rbt(other.rbt) {
	}
	void emplace(const _Key& key, const _Value& value) {
		rbt.insert(my_pair<_Key, _Value>(key, value));
	}
	void remove(const _Key& key) {
		rbt.remove(my_pair<_Key, _Value>(key, key));
		size = rbt.size;
	}
	_Value& get(const _Key& key) {
		return rbt.get(my_pair<_Key, _Value>(key, key)).second;
	}
	size_t size() const noexcept {
		return rbt.size();
	}
	~Dictionary() = default;

	bool operator==(const Dictionary<_Key, _Value>& other) const noexcept {
		return this->rbt.Equals(other.rbt);
	}
	bool operator!=(const Dictionary<_Key, _Value>& other) const noexcept {
		return !this->rbt.Equals(other.rbt);
	}
	_Value& operator[](const _Key& key) {
		try {
			return this->get(key);
		}
		catch (SetException e) {
			if (e.id == NoSuchElement) {
				this->emplace(key, _Value());
				return this->get(key);
			}
			else {
				throw e;
			}
		}
	}
};