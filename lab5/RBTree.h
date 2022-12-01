#pragma once
#include "exception.h"
#include "LinkedList.h"

template<typename T>
struct MyComparator {
	short int operator()(const T& first, const T& second) const noexcept {
		if (first < second) return -1;
		if (first > second) return 1;
		return 0;
	}
};

template<typename _Key, typename _Value, bool CanChangeValue = true, bool IsMulti = false, class _cmp = MyComparator<_Key>>
class RBTree {
protected:
	template<typename _Key, typename _Value, bool CanChangeValue, bool IsMulti, class _cmp, bool IsConst>
	friend class RBTIterator;
	_cmp comparator;
	enum _color { BLACK, RED, None };

	struct node {
		node* right;
		node* left;
		node* parent;
		std::pair<const _Key, LinkedList<_Value>> data;
		_color color;
		node() : std::pair<const _Key, LinkedList<_Value>>(_Key(), LinkedList<_Value>()) {
			right = nullptr;
			left = nullptr;
			parent = nullptr;
		}
		node(const node& other) : data(other.data) {
			this->color = other.color;
			left = other.left;
			right = other.right;
			parent = other.parent;
		}
		node(const node* other) : data(other->data) {}
		node(const _Key& key, const _Value& value, const _color& color) : data(key, LinkedList<_Value>()) {
			this->color = color;
			data.second.Append(value);
			left = nullptr;
			right = nullptr;
			parent = nullptr;
		}
		~node() = default;
	};
	
	LinkedList<node*> traversal_order;

	void clear(node* cur) noexcept {
		if (cur == nullptr) return;
		node* right = cur->right;
		node* left = cur->left;
		
		delete cur;
		clear(right);
		clear(left);
	}

	node* root = nullptr;
	size_t _size;

	node* get_uncle(const node* cur) noexcept {
		if (cur->parent->parent == nullptr) {
			return nullptr;
		}
		if (cur->parent == cur->parent->parent->left)
			return cur->parent->parent->right;
		return cur->parent->parent->left;
	}

	bool isRight(const node* cur) noexcept {
		if (cur->parent == nullptr) {
			return false;
		}
		return cur->parent->right == cur;
	}

	node* getbrother(const node* p) noexcept {
		return isRight(p) ? p->parent->left : p->parent->right;
	}

	void rotateRight(node* cur) noexcept {
			node* p = cur->left;
			if (!p) return;
			if (cur == root) root = p;
			cur->left = p->right;
			if (p->right)
				p->right->parent = cur;
			if (isRight(cur) && cur->parent != nullptr) {
				cur->parent->right = p;
			}
			else if (cur->parent != nullptr) {
				cur->parent->left = p;
			}
			p->right = cur;
			p->parent = cur->parent;
			cur->parent = p;
	}

	void rotateLeft(node* cur) noexcept {
			node* p = cur->right;
			if (!p) return;
			if (cur == root) root = p;
			cur->right = p->left;
			if (p->left)
				p->left->parent = cur;
			if (isRight(cur) && cur->parent != nullptr) {
				cur->parent->right = p;
			}
			else if (cur->parent != nullptr) {
				cur->parent->left = p;
			}
			p->left = cur;
			p->parent = cur->parent;
			cur->parent = p;
	}

	void fixnode(node* cur) noexcept {
		while (cur->parent != nullptr && cur->parent->color == RED) {
			node* uncle = get_uncle(cur);
			if (!isRight(cur->parent)) {
				if (uncle != nullptr && uncle->color == RED) {
					cur->parent->color = BLACK;
					uncle->color = BLACK;
					cur->parent->parent->color = RED;
					cur = cur->parent->parent;
				}
				else {
					if (isRight(cur)) {
						cur = cur->parent;
						rotateLeft(cur);
					}
					cur->parent->color = BLACK;
					cur->parent->parent->color = RED;
					rotateRight(cur->parent->parent);
				}
			}
			else {
				if (uncle != nullptr && uncle->color == RED) {
					cur->parent->color = BLACK;
					uncle->color = BLACK;
					cur->parent->parent->color = RED;
					cur = cur->parent->parent;
				}
				else {
					if (!isRight(cur)) {
						cur = cur->parent;
						rotateRight(cur);
					}
					cur->parent->color = BLACK;
					cur->parent->parent->color = RED;
					rotateLeft(cur->parent->parent);
				}
			}
		}
		this->root->color = BLACK;
	}

	node* nearest_key(const node* cur) noexcept {
		node* res = cur->right;
		while (res->left != nullptr) {
			res = res->left;
		}
		return res;
	}

	_color GetChildColors(const node* cur) noexcept {
		_color res = BLACK;
		if (cur->right != nullptr) {
			if (cur->right->color == RED) {
				res = RED;
			}
			else {
				res = BLACK;
			}
		}
		if (cur->left != nullptr) {
			if (cur->left->color != res) {
				res = None;
			}
		}
		return res;
	}

	void fixdelete(node* p) noexcept {
		if (p == root) return;
		node* bro = getbrother(p);
		if (bro->color == BLACK) { //black bro
			if (GetChildColors(bro) == BLACK) { //all children are black
				bro->color = RED;
				if (p->parent->color == RED) {
					p->parent->color = BLACK;
					return;
				}
				else {
					fixdelete(p->parent);
					return;
				}
			}
			if (!isRight(p)) {
				if (bro -> right != nullptr && bro->right->color == RED) { //right child is red
					bro->color = p->parent->color;
					if (bro->right != nullptr)
						bro->right->color = BLACK;
					bro->parent->color = BLACK;
					rotateLeft(bro->parent);
					return;
				}
				if (bro->left->color == RED && (bro->right == nullptr || bro->right->color == BLACK)) { //left child is red and right is black
					std::swap(bro->left->color, bro->color);
					rotateRight(bro);
					fixdelete(p);
					return;
				}
			}
			else {
				if (bro -> left != nullptr && bro->left->color == RED) { //right child is red
					bro->color = p->parent->color;
					if (bro->left != nullptr)
						bro->left->color = BLACK;
					bro->parent->color = BLACK;
					rotateRight(bro->parent);
					return;
				}
				if (bro->right->color == RED && (bro->left == nullptr || bro->left->color == BLACK)) { //left child is red and right is black
					std::swap(bro->right->color, bro->color);
					rotateLeft(bro);
					fixdelete(p);
					return;
				}
			}
		}
		//red bro
		p->parent->color = RED;
		bro->color = BLACK;
		if (!isRight(p)) {
			rotateLeft(p->parent);
		}
		else {
			rotateRight(p->parent);
		}
		fixdelete(p);
		return;
	}

	void move_node(node* first, node* second) {
		node* parent = first->parent;
		node* left = first->left;
		node* right = first->right;
		_color clr = first->color;
		first = new node(second);
		first->left = left;
		first->right = right;
		first->parent = parent;
		first->color = clr;
	}

	void delete_node(node* cur) noexcept {
		
		//0 children
		if (cur->left == nullptr && cur->right == nullptr) {
			if (cur->parent == nullptr) {//root
				root = nullptr;
				delete cur;
				return;
			}
			if (cur->color == RED) { //red
				if (isRight(cur)) {
					cur->parent->right = nullptr;
				}
				else {
					cur->parent->left = nullptr;
				}
				delete cur;
				return;
			}
			
			//black node deleting
			fixdelete(cur);
			if (isRight(cur)) cur->parent->right = nullptr;
			else cur->parent->left = nullptr;
			delete cur;
			return;
		}

		//1 child
		if ((cur->left == nullptr) ^ (cur->right == nullptr)) {
			if (cur->parent == nullptr) { //root
				this->root = cur->right != nullptr ? cur->right : cur->left;
				if (cur->right != nullptr) {
					cur->right->color = BLACK;
					cur->right->parent = nullptr;
				}
				else {
					cur->left->color = BLACK;
					cur->left->parent = nullptr;
				}
				delete cur;
				return;
			}
			
			//red impossible
			
			//black node deleting
			if (cur->right != nullptr) {
				move_node(cur, cur->right);
				delete_node(cur->right);
				return;
			}
			else {
				move_node(cur, cur->left);

				delete_node(cur->left);
				return;
			}
		}

		//2 children
		node* nearest = nearest_key(cur);
		move_node(cur, nearest);
		delete_node(nearest);
		return;
	}

	void make_tree(node* cur, const node* other) {
		if (other == nullptr) return;
		if (other->left != nullptr) {
			cur->left = new node(*other->left);
			cur->left->parent = cur;
		}
		if (other->right != nullptr) {
			cur->right = new node(*other->right);
			cur->right->parent = cur;
		}
		make_tree(cur->left, other->left);
		make_tree(cur->right, other->right);
	}

	bool Equals(const node* cur, const node* other) const noexcept {
		if (cur == other && cur == nullptr) return true;
		if (cur->data != other->data) return false;
		if (cur->color != other->color) return false;
		return Equals(cur->left, other->left) && Equals(cur->right, other->right);
	}

	void fix_traversal_order(node* cur) {
		if (cur->left == nullptr && cur->right == nullptr) {
			traversal_order.push_back(cur);
			return;
		}
		if (cur->left == nullptr) {
			traversal_order.push_back(cur);
			fix_traversal_order(cur->right);
			return;
		}
		fix_traversal_order(cur->left);
		traversal_order.push_back(cur);
		if (cur->right != nullptr)
			fix_traversal_order(cur->right);
	}

	void fix_traversal_order() {
		traversal_order.clear();
		fix_traversal_order(this->root);
	}

	size_t count(const node* cur) const {
		if (cur == nullptr) return 0;
		return cur->data.second.GetLength() + count(cur->right) + count(cur->left);
	}
public:
	using iterator = typename RBTIterator<_Key, _Value, CanChangeValue, IsMulti, _cmp, false>;
	using const_iterator = typename RBTIterator<_Key, _Value, CanChangeValue, IsMulti, _cmp, true>;
	iterator begin() {
		typename LinkedList<node*>::iterator* it = new typename LinkedList<node*>::iterator(traversal_order.begin());
		iterator* it_res = reinterpret_cast<iterator*>(it);
		return *it_res;
	}

	iterator end() {
		typename LinkedList<node*>::iterator* it = new typename LinkedList<node*>::iterator(traversal_order.end());
		iterator* it_res = reinterpret_cast<iterator*>(it);
		return *it_res;
	}

	const_iterator cbegin() {
		typename LinkedList<node*>::const_iterator* it = new typename LinkedList<node*>::const_iterator(traversal_order.cbegin());
		const_iterator* it_res = reinterpret_cast<const_iterator*>(it);
		return *it_res;
	}

	const_iterator cend() {
		typename LinkedList<node*>::const_iterator* it = new typename LinkedList<node*>::const_iterator(traversal_order.cend());
		const_iterator* it_res = reinterpret_cast<const_iterator*>(it);
		return *it_res;
	}
	RBTree() : traversal_order() {
		root = nullptr;
		_size = 0;
	}
	RBTree(const RBTree<_Key, _Value>& other) : traversal_order() {
		root = new node(*other.root);
		_size = other._size;
		make_tree(root, other.root);
	}
	~RBTree(){
		clear(root);
	}

	size_t get_height(const node* cur) const noexcept {
		if (cur == nullptr) return 0;
		size_t cur_Heigth = cur->data.second.GetLength();
		return get_height(cur->left) + get_height(cur->right) + cur_Heigth;
	}

	size_t get_height() const noexcept {
		return get_height(root);
	}

	size_t get_unique() {
		return _size;
	}

	size_t count() const {
		return count(this->root);
	}
	void insert(const _Key& key, const _Value& value) {
		if (this->root == nullptr) {
			this->root = new node(key, value, BLACK);
			_size++;
			fix_traversal_order();
			return;
		}
		node* cur = this->root;
		node* parent = cur;
		while (cur != nullptr) {
			if (comparator(cur->data.first, key) > 0) {
				parent = cur;
				cur = cur->left;
			}
			else if (comparator(cur->data.first, key) < 0) {
				parent = cur;
				cur = cur->right;
			}
			else {
				if (IsMulti) {
					cur->data.second.Append(value);
				}
				else cur->data.second[0] = value;
				return;
			}
		}
		_size++;
		if (comparator(parent->data.first, key) < 0) {
			parent->right = new node(key, value, RED);
			parent->right->parent = parent;
			fixnode(parent->right);
		}
		else {
			parent->left = new node(key, value, RED);
			parent->left->parent = parent;
			fixnode(parent->left);
		}
		fix_traversal_order();
	}

	std::conditional_t<CanChangeValue, LinkedList<_Value>&, const LinkedList<_Value>&> get(const _Key& key) {
		node* cur = root;
		while (cur != nullptr) {

			if (comparator(cur->data.first, key) < 0) {
				cur = cur->right;
			}
			else if (comparator(cur->data.first, key) > 0) {
				cur = cur->left;
			}
			else {
				return cur->data.second;
			}
		}
		if (cur == nullptr) throw SetException(NoSuchElement);
	}

	void remove(const _Key& key) {
		node* cur = root;
		while (cur != nullptr) {
			if (comparator(cur->data.first, key) > 0) {
				cur = cur->left;
			}
			else if (comparator(cur->data.first, key) < 0) {
				cur = cur->right;
			}
			else {
				_size--;
				delete_node(cur);
				fix_traversal_order();
				return;
			}
		}
		throw SetException(NoSuchElement);
	}
	void remove(const _Key& key, const _Value& val) {
		node* cur = root;
		while (cur != nullptr) {
			if (comparator(cur->data.first, key) > 0) {
				cur = cur->left;
			}
			else if (comparator(cur->data.first, key) < 0) {
				cur = cur->right;
			}
			else {
				_size--;

				typename LinkedList<_Value>::iterator it = cur->data.second.find(val);
				if (it == cur->data.second.end()) throw SetException(NoSuchElement);
				cur->data.second.del_item(it);
				if (cur->data.second.GetLength() == 0) {
					delete_node(cur);
					fix_traversal_order();
				}
				return;
			}
		}
		throw SetException(NoSuchElement);
	}

	size_t size() const noexcept {
		return this->_size;
	}

	bool find(const _Key& key) const noexcept {
		node* cur = root;
		while (cur != nullptr) {
			if (comparator(key, cur->data.first) < 0) {
				cur = cur->left;
			}
			else if (comparator(key, cur->data.first) > 0) {
				cur = cur->right;
			}
			else {
				return true;
			}
		}
		return false;
	}

	bool Equals(const RBTree<_Key, _Value, CanChangeValue, IsMulti, _cmp>& other) const noexcept {
		return Equals(this->root, other.root);
	}

	bool operator==(const RBTree<_Key, _Value, CanChangeValue, IsMulti, _cmp>& other) const noexcept {
		return this->Equals(other);
	}
	bool operator!=(const RBTree<_Key, _Value, CanChangeValue, IsMulti, _cmp>& other) const noexcept {
		return !this->Equals(other);
	}

	void clear() {
		_size = 0;
		clear(root);
		root = nullptr;
	}

	std::conditional_t<CanChangeValue, LinkedList<_Value>&, const LinkedList<_Value>&> operator[](const _Key& key) {
		try {
			return this->get(key);
		}
		catch (SetException e) {
			if (e.id == NoSuchElement) {
				this->insert(key, _Value());
				return this->get(key);
			}
			else {
				throw e;
			}
		}
	}
};

template<typename _Key, typename _Value, bool CanChangeValue, bool IsMulti, class _cmp, bool IsConst>
class RBTIterator : public BidirectionalIterator<typename RBTree<_Key, _Value, CanChangeValue, IsMulti, _cmp>::node*, true> {
private:
	using thisiterator = BidirectionalIterator<typename RBTree<_Key, _Value, CanChangeValue, IsMulti, _cmp>::node*, true>;
public:
	RBTIterator(const RBTIterator& other) : thisiterator(other) {}
	std::conditional_t<CanChangeValue && !IsConst, std::pair <const _Key, LinkedList<_Value>>&, const std::pair <const _Key, LinkedList<_Value>>&> operator*() {
		return this->item->data->data;
	}
};