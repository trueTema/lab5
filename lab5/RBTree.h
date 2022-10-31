#pragma once

template<typename _Key, typename _Value>
class RBTree {
private:
	enum _color { BLACK, RED };

	struct node {
	private:
		node* right;
		node* left;
		node* parent;
		_Key key;
		_Value value;
		_color color;
		size_t height;
	public:
		_color _GetColor() { return color; }
		_Value _GetValue() { return value; }
		_Key _GetKey() { return key; }
		node(_Key key, _Value value, _color color) {
			this->key = key;
			this->value = value;
			this->color = color;
			left = nullptr;
			right = nullptr;
			parent = nullptr;
		}
		bool operator==(_Value T) {
			return this->value == T;
		}
		bool operator<(_Value T) {
			return this->value < T;
		}
		bool operator>(_Value T) {
			return this->value > T;
		}
		//привет это тест. как дела, дружище?
		bool operator==(node& other) {
			return (this->value == other.value) && (this->key == other.key) && (this->color == other.color);
		}
		bool operator!=(node& other) {
			return !((this->value == other.value) && (this->key == other.key) && (this->color == other.color));
		}
		~node() {}
	};
	void clear(node* cur) {
		node* right = cur->right;
		node* left = cur->left;
		delete cur;
		clear(right);
		clear(left);
	}

	node* root;
	size_t size;
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

	void insert(_Value value) {
		this->root = new node();
	}
	bool operator==(const RBTree<_Key, _Value>& other) {
		return root == other.root;
	}
	bool operator!=(const RBTree<_Key, _Value>& other) {
		return root == other.root;
	}
};

