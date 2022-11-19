#pragma once
#include "RBTree.h"

template<typename _Key, typename _Value>
class Dictionary : public RBTree<_Key, _Value, true> {
public:
	using iterator = typename RBTree<_Key, _Value, true>::iterator;
	Dictionary() : RBTree<_Key, _Value, true>() {}
	Dictionary(const Dictionary<_Key, _Value>& other) : RBTree<_Key, _Value, true>(other) {
	}
	~Dictionary() = default;
};