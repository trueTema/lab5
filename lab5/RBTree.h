#pragma once

enum except {Right, Left};

struct SetException {
private:
	except id;
public:
	SetException(except e) {
		id = e;
	}
	std::string message() {
		if (id == Right) return "Right";
		if (id == Left) return "Right";
	}
};

template<typename _Key, typename _Value>
class RBTree {
private:
	enum _color { BLACK, RED, None };

	struct node {
		node* right;
		node* left;
		node* parent;
		_Key key;
		_Value value;
		_color color;
		node(const _Key key, const _Value value, const _color color) {
			this->key = key;
			this->value = value;
			this->color = color;
			left = nullptr;
			right = nullptr;
			parent = nullptr;
		}
		~node() {}
	};
	void clear(node* cur) {
		if (cur == nullptr) return;
		node* right = cur->right;
		node* left = cur->left;
		
		delete cur;
		clear(right);
		clear(left);
	}

	node* root;
	size_t size;

	node* get_uncle(const node* cur) {
		if (cur->parent->parent == nullptr) {
			return nullptr;
		}
		if (cur->parent == cur->parent->parent->left)
			return cur->parent->parent->right;
		return cur->parent->parent->left;
	}

	bool isRight(const node* cur) {
		if (cur->parent == nullptr) {
			return false;
		}
		return cur->parent->right == cur;
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

	void fixnode(node* cur) {
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

	node* nearest_key(node* cur) {
		cur = cur->right;
		while (cur->left != nullptr) {
			cur = cur->left;
		}
		return cur;
	}

	_color GetChildColors(node* cur) {
		_color res = BLACK;
		if (cur == nullptr) {
			return None;
		}
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

	void fixdelete(node* p) {
		while (p->color == BLACK && p->parent != nullptr) {
			if (!isRight(p)) {
				if (p->parent->right != nullptr && p->parent->right->color == RED) {
					p->parent->right->color = BLACK;
					p->parent->color = RED;
					rotateLeft(p->parent);
				}
				if (GetChildColors(p->parent->right) == BLACK) {
					p->parent->right->color = RED;
				}
				else {
					if (p->parent->right != nullptr) {
						if (p->parent->right->right == nullptr || p->parent->right->right->color == BLACK) {
							if (p->parent->right->left != nullptr)
								p->parent->right->left->color = BLACK;
							p->parent->right->color = RED;
							rotateRight(p->parent->right);
						}
						p->parent->right->color = p->parent->color;

						if (p->parent->right->right != nullptr) {
							p->parent->right->right->color = BLACK;
						}
					}

					p->parent->color = BLACK;
					rotateLeft(p->parent);
					p = root;
				}
			}
			else {
				if (p->parent->left != nullptr && p->parent->left->color == RED) {
					p->parent->left->color = BLACK;
					p->parent->color = RED;
					rotateRight(p->parent);
				}
				if (GetChildColors(p->parent->left) == BLACK) {
					p->parent->left->color = RED;
				}
				else {
					if (p->parent->left != nullptr) {
						if (p->parent->left->left == nullptr || p->parent->left->left->color == BLACK) {
							if (p->parent->left->right != nullptr)
								p->parent->left->right->color = BLACK;
							p->parent->left->color = RED;
							rotateLeft(p->parent->left);
						}
						p->parent->left->color = p->parent->color;

						if (p->parent->left->left != nullptr) {
							p->parent->left->left->color = BLACK;
						}
					}

					p->parent->color = BLACK;
					rotateRight(p->parent);
					p = root;
				}
			}
		}
	}

	void delete_node(node* cur) {
		
		if (cur->left == nullptr && cur->right == nullptr) {
			if (cur->parent == nullptr) {
				root = nullptr;
			}
			else if (isRight(cur)) {
				cur->parent->right = nullptr;
			}
			else {
				cur->parent->left = nullptr;
			}
			delete cur;
			return;
		}
		node* nearest = cur;
		_color deleting = cur->color;
		if ((cur->right == nullptr) ^ (cur->left == nullptr)) {
			if (cur->right != nullptr) {
				if (cur->parent != nullptr && isRight(cur)) { //подумать что будет если корень 
					cur->parent->right = cur->right;
				}
				else if (cur->parent != nullptr) {
					cur->parent->left = cur->right;
				}
				cur->right->parent = cur->parent;
				if (cur->parent == nullptr) {
					root = cur->right;
				}
				//if ((cur->parent == nullptr) || (cur->parent->color == RED)) {
				//	cur->right->color = BLACK;
				//}
			}
			else {
				if (cur->parent != nullptr && isRight(cur)) { //подумать что будет если корень 
					cur->parent->right = cur->left;
				}
				else if (cur->parent != nullptr) {
					cur->parent->left = cur->left;
				}
				cur->left->parent = cur->parent;
				if (cur->parent == nullptr) {
					root = cur->left;
				}
				//if ((cur->parent == nullptr) || (cur->parent->color == RED)) {
				//	cur->left->color = BLACK;
				//}
			}
			/*delete cur;
			return;*/
		}
		else {
			nearest = nearest_key(cur);
			if (nearest->right != nullptr) {
				nearest->right->parent = nearest->parent;
			}
			if (nearest->parent == nullptr) {
				root = nearest->right;
			}
			else {
				if (isRight(nearest)) {
					nearest->parent->right = nearest->right;
				}
				else {
					nearest->parent->left = nearest->right;
				}
			}
		}
		if (nearest != cur) {
			cur->color = nearest->color;
			cur->key = nearest->key;
			cur->value = nearest->value;
		}
		if (deleting == BLACK)
			fixdelete(cur->left != nullptr ? cur->left : cur->right);
		if (nearest != cur)
			delete nearest;
		else delete cur;
		return;
	}
public:
	RBTree() {
		root = nullptr;
		size = 0;
	}
	RBTree(const RBTree<_Key, _Value>& other) {
		root = other.root;
		size = other.size;
	}
	~RBTree(){
		clear(root);
	}

	size_t get_height(const node* cur) {
		if (cur == nullptr) return 0;
		return std::max(get_height(cur->left), get_height(cur->right)) + 1;
	}

	size_t get_height() {
		return get_height(root);
	}

	void insert(const _Key key, const _Value value) {
		if (this->root == nullptr) {
			this->root = new node(key, value, BLACK);
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
				cur->value = value;
				return;
			}
		}
		if (parent->key < key) {
			parent->right = new node(key, value, RED);
			parent->right->parent = parent;
			fixnode(parent->right);
		}
		else {
			parent->left = new node(key, value, RED);
			parent->left->parent = parent;
			fixnode(parent->left);
		}
	}

	_Value get(const _Key& key) {
		node* cur = root;
		while (cur != nullptr) {

			if (cur->key < key) {
				cur = cur->right;
			}
			else if (cur->key > key) {
				cur = cur->left;
			}
			else {
				return cur->value;
			}
		}
		if (cur == nullptr) throw "No such key";
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
				delete_node(cur);
				return;
			}
		}
		throw "No such key";
	}

	int max_check(node* cur) {
		if (cur == nullptr) return 0;
		if (cur->color == BLACK) {
			return std::max(max_check(cur->left), max_check(cur->right)) + 1;
		} 
		return std::max(max_check(cur->left), max_check(cur->right));
	}

	int min_check(node* cur) {
		if (cur == nullptr) return 0;
		if (cur->color == BLACK) {
			return std::min(max_check(cur->left), max_check(cur->right)) + 1;
		}
		return std::min(max_check(cur->left), max_check(cur->right));
	}

	bool check(node* cur) {
		if (cur == nullptr) return true;
		if (cur->color == RED) {
			if (cur->left != nullptr && cur->left->color == RED) return false;
			if (cur->right != nullptr && cur->right->color == RED) return false;
		}
		return (check(cur->left) && check(cur->right));
	}

	bool check() {
		if (root->color != BLACK) return false;
		node* cur = root;
		if (min_check(root) != max_check(root)) return false;
		if (!check(root)) return false;
		return true;
	}

	bool operator==(const RBTree<_Key, _Value>& other) {
		return root == other.root;
	}
	bool operator!=(const RBTree<_Key, _Value>& other) {
		return root == other.root;
	}
};

