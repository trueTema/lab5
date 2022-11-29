#pragma once
#include "RBTree.h"

template<typename _Key, class _cmp = MyComparator<_Key>>
class MultiSet : public RBTree<_Key, _Key, false, true, _cmp> {
private:
	size_t size_s;
	using rbt = RBTree<_Key, _Key, false, true, _cmp>;
	const _Key& get_median(typename rbt::node* cur, size_t greater, size_t lower, size_t count) const {
		if (cur == nullptr) throw SetException(IncorrectComparator);
		if (count / 2 <= (count - greater) && count / 2 >= lower) return cur->data.second[0];
		if (count / 2 > (count - greater)) {
			return get_median(cur->right, rbt::get_height(cur->right->right), lower + rbt::get_height(cur->right->left) + 1, count);
		}
		else {
			return get_median(cur->left, greater + rbt::get_height(cur->left->right) + 1, rbt::get_height(cur->left->left), count);
		}
	}
public:
	using iterator = typename RBTree<_Key, _Key, false, true, _cmp>::iterator;
	MultiSet() : RBTree<_Key, _Key, false, true, _cmp>() { size_s = 0; }
	MultiSet(const MultiSet<_Key, _cmp>& other) : RBTree<_Key, _Key, false, true, _cmp>(other) {
		size_s = other.size_s;
	}
	void insert(const _Key& key) {
		RBTree<_Key, _Key, false, true, _cmp>::insert(key, key);
		size_s++;
	}
	void remove(const _Key& key) {
		rbt::remove(key, key);
		size_s--;
	}
	const _Key& get_max() const {
		typename rbt::node* cur = rbt::root;
		if (cur == nullptr) {
			throw SetException(EmptySequence);
		}
		while (cur->right != nullptr) {
			cur = cur->right;
		}
		if (cur == nullptr) throw SetException(IncorrectRange);
		return cur->data.second[0];
	}
	const _Key& get_min() const {
		typename rbt::node* cur = rbt::root;
		if (cur == nullptr) {
			throw SetException(EmptySequence);
		}
		while (cur->left != nullptr) {
			cur = cur->left;
		}
		if (cur == nullptr) throw SetException(IncorrectRange);
		return cur->data.second[0];
	}
	const _Key& get_median() const { // 1 2 3 4 6 7 8 9
		if (rbt::root == nullptr) {
			throw SetException(EmptySequence);
		}
		return get_median(rbt::root, rbt::get_height(rbt::root->right), rbt::get_height(rbt::root->left), count());
	}
	size_t count() const {
		return size_s;
	}
	~MultiSet() = default;
};