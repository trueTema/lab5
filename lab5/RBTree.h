#pragma once
#include "exception.h"

template<typename T, typename V>
struct my_pair {
public:
	T first;
	V second;
	my_pair() = default;
	my_pair(const my_pair<T, V>& other) {
		this->first = other.first;
		this->second = other.second;
	}
	my_pair(T a, V b) {
		first = a;
		second = b;
	}
	~my_pair() = default;
	bool operator<(const my_pair<T, V>& other) const {
		return this -> first < other.first;
	}
	bool operator>(const my_pair<T, V>& other) const {
		return this -> first > other.first;
	}
	bool operator==(const my_pair<T, V>& other) const {
		return this -> first == other.first;
	}
	bool operator!=(const my_pair<T, V>& other) const {
		return this->first != other.first;
	}
};

template<typename _Key, bool CanChangeValue = true>
class RBTree {
private:
	
	enum _color { BLACK, RED, None };

	struct node {
		node* right;
		node* left;
		node* parent;
		_Key key;
		_color color;
		node(const node& other) {
			this->key = other.key;
			this->color = other.color;
			left = nullptr;
			right = nullptr;
			parent = nullptr;
		}
		node(const _Key key, const _color color) {
			this->key = key;
			this->color = color;
			left = nullptr;
			right = nullptr;
			parent = nullptr;
		}
		~node() {}
	};
	void clear(node* cur) noexcept {
		if (cur == nullptr) return;
		node* right = cur->right;
		node* left = cur->left;
		
		delete cur;
		clear(right);
		clear(left);
	}

	node* root;
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
				cur -> key = cur->right->key;
				delete_node(cur->right);
				return;
			}
			else {
				cur->key = cur->left->key;
				delete_node(cur->left);
				return;
			}
		}

		//2 children
		node* nearest = nearest_key(cur);
		cur->key = nearest->key;
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
		if (cur->key != other->key) return false;
		if (cur->color != other->color) return false;
		return Equals(cur->left, other->left) && Equals(cur->right, other->right);
	}
public:
	RBTree() {
		root = nullptr;
		_size = 0;
	}
	RBTree(const RBTree<_Key>& other) {
		root = new node(*other.root);
		_size = other._size;
		make_tree(root, other.root);
	}
	~RBTree(){
		clear(root);
	}

	size_t get_height(const node* cur) const noexcept {
		if (cur == nullptr) return 0;
		return std::max(get_height(cur->left), get_height(cur->right)) + 1;
	}

	size_t get_height() const noexcept {
		return get_height(root);
	}

	void insert(const _Key key) {
		if (this->root == nullptr) {
			this->root = new node(key, BLACK);
			return;
		}
		node* cur = this->root;
		node* parent = cur;
		while (cur != nullptr) {
			if (cur->key > key) {
				parent = cur;
				cur = cur->left;
			}
			else if (cur->key < key) {
				parent = cur;
				cur = cur->right;
			}
			else {
				cur->key = key;
				return;
			}
		}
		_size++;
		if (parent->key < key) {
			parent->right = new node(key, RED);
			parent->right->parent = parent;
			fixnode(parent->right);
		}
		else {
			parent->left = new node(key, RED);
			parent->left->parent = parent;
			fixnode(parent->left);
		}
	}

	std::conditional_t<CanChangeValue, _Key&, const _Key&> get(const _Key& key) {
		node* cur = root;
		while (cur != nullptr) {

			if (cur->key < key) {
				cur = cur->right;
			}
			else if (cur->key > key) {
				cur = cur->left;
			}
			else {
				return cur->key;
			}
		}
		if (cur == nullptr) throw SetException(NoSuchElement);
	}

	void remove(const _Key& key) {
		node* cur = root;
		while (cur != nullptr) {
			if (key < cur->key) {
				cur = cur->left;
			}
			else if (key > cur->key) {
				cur = cur->right;
			}
			else {
				_size--;
				delete_node(cur);
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
			if (key < cur->key) {
				cur = cur->left;
			}
			else if (key > cur->key) {
				cur = cur->right;
			}
			else {
				return true;
			}
		}
		return false;
	}

	bool Equals(const RBTree<_Key>& other) const noexcept {
		return Equals(this->root, other.root);
	}
};
