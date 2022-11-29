#pragma once
#include "RBTree.h"

template<typename _Key, typename _Value, class _cmp = MyComparator<_Key>>
class Dictionary : public RBTree<_Key, _Value, true, false, _cmp> {
public:
	using iterator = typename RBTree<_Key, _Value, true, false, _cmp>::iterator;
	Dictionary() : RBTree<_Key, _Value, true, false, _cmp>() {}
	Dictionary(const Dictionary<_Key, _Value, _cmp>& other) : RBTree<_Key, _Value, true, false, _cmp>(other) {
	}
	_Value& get(const _Key& key) {
		return RBTree<_Key, _Value, true, false, _cmp>::get(key)[0];
	}
	_Value& operator[](const _Key& key) {
		try {
			return this->get(key);
		}
		catch (SetException e) {
			if (e.id == NoSuchElement) {
				RBTree<_Key, _Value, true, false, _cmp>::insert(key, _Value());
				return this->get(key);
			}
			else {
				throw e;
			}
		}
	}
	~Dictionary() = default;
};