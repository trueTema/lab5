#pragma once
#include "DynamicArray.h"
#include "Dictionary.h"

template<typename T>
class sparse_matrix {
protected:
	size_t hor = 0;
	size_t ver = 0;
	Dictionary<std::pair<size_t, size_t>, T> matrix;
public:
	sparse_matrix() = default;
	sparse_matrix(size_t hor, size_t ver) {
		this->hor = hor;
		this->ver = ver;
	}
	sparse_matrix(const std::initializer_list<std::initializer_list<T>>& list) {
		this->hor = list.size();
		this->ver = (*list.begin()).size();
		for (auto i : list) {
			if (i.size() != ver) throw SetException(IncorrectArraySize);
		}
		size_t i_index = 0;
		for (auto i : list) {
			size_t j_index = 0;
			for (auto j : i) {
				if (j != T()) {
					this->matrix.insert(std::make_pair(i_index, j_index), j);
				}
				j_index++;
			}
			i_index++;
		}
	}
	sparse_matrix(DynamicArray<DynamicArray<T>>& matrix) {
		this->hor = matrix.GetSize();
		this->ver = matrix[0].GetSize();
		for (size_t i = 0; i < matrix.GetSize(); i++) {
			if (matrix[i].GetSize() != ver) throw SetException(IncorrectArraySize);
		}
		for (size_t i = 0; i < hor; i++) {
			for (size_t j = 0; j < ver; j++) {
				if (matrix[i][j] != T()) {
					this->matrix.insert(std::make_pair(i, j), matrix[i][j]);
				}
			}
		}
	}
	sparse_matrix(const sparse_matrix<T>& other) : sparse_matrix(other.hor, other.ver) {
		matrix = other.matrix;
	}
	sparse_matrix(sparse_matrix<T>&& other) : sparse_matrix(other.hor, other.ver) {
		matrix = other.matrix;
	}
	~sparse_matrix() = default;

	

	void set(size_t row, size_t column, const T& value) {
		if (row > hor || column > ver) throw SetException(IncorrectRange);
		if (value != T()) {
			matrix[std::make_pair(row - 1, column - 1)] = value;
		}
		else {
			if (matrix.find(std::make_pair(row - 1, column - 1)))
				matrix.remove(std::make_pair(row - 1, column - 1));
		}
	}
	void add_row(const std::initializer_list<T>& list) {
		if (list.size() != ver && ver != 0) throw SetException(IncorrectArraySize);
		hor++;
		if (ver == 0) ver = list.size();
		size_t pos = 0;
		for (auto i : list) {
			if (i != T()) matrix[std::make_pair(hor - 1, pos)] = i;
			pos++;
		}
	}
	void add_column(const std::initializer_list<T>& list) {
		if (list.size() != hor && hor != 0) throw SetException(IncorrectArraySize);
		ver++;
		if (hor == 0) hor = list.size();
		size_t pos = 0;
		for (auto i : list) {
			if (i != T()) matrix[std::make_pair(pos, ver - 1)] = i;
			pos++;
		}
	}
	void remove_row(size_t number) {
		if (number < 1 || number > hor) throw SetException(IncorrectRange);
		for (size_t i = 0; i < ver; i++) {
			if (matrix.find(std::make_pair(number - 1, i)))
				matrix.remove(std::make_pair(number - 1, i));
		}
		for (size_t i = number; i < hor; i++) {
			for (size_t j = 0; j < ver; j++) {
				if (matrix.find(std::make_pair(i, j))) {
					matrix[std::make_pair(i - 1, j)] = matrix[std::make_pair(i, j)];
					matrix.remove(std::make_pair(i, j));
				}
			}
		}
		hor--;
	}
	void remove_column(size_t number) {
		if (number < 1 || number > ver) throw SetException(IncorrectRange);
		for (size_t i = 0; i < hor; i++) {
			if (matrix.find(std::make_pair(i, number - 1)))
				matrix.remove(std::make_pair(i, number - 1));
		}
		for (size_t i = 0; i < hor; i++) {
			for (size_t j = number; j < ver; j++) {
				if (matrix.find(std::make_pair(i, j))) {
					matrix[std::make_pair(i, j - 1)] = matrix[std::make_pair(i, j)];
					matrix.remove(std::make_pair(i, j));
				}
			}
		}
		ver--;
	}
	const T& get(size_t row, size_t column) {
		row--;
		column--;
		if (row >= hor) throw SetException(IncorrectRange);
		if (column >= ver) throw SetException(IncorrectRange);
		if (matrix.find(std::make_pair(row, column)))
			return matrix[std::make_pair(row, column)];
		return T();
	}

	sparse_matrix<T>& operator=(const sparse_matrix<T>& other) {
		this->matrix = other.matrix;
		this->hor = other.hor;
		this->ver = other.ver;
		return *this;
	}
	sparse_matrix<T>& operator=(sparse_matrix<T>&& other) {
		this->matrix = other.matrix;
		this->hor = other.hor;
		this->ver = other.ver;
		other.hor = 0;
		other.ver = 0;
		return *this;
	}
	T& operator[](std::pair<size_t, size_t> pos) {
		size_t row = pos.first - 1;
		size_t column = pos.second - 1;
		if (row >= hor) throw SetException(IncorrectRange);
		if (column >= ver) throw SetException(IncorrectRange);
		if (matrix.find(std::make_pair(row, column)))
			return matrix[std::make_pair(row, column)];
		return T();
	}
	bool operator==(sparse_matrix<T>& other) {
		return hor == other.hor && ver == other.ver && matrix == other.matrix;
	}
	bool operator!=(sparse_matrix<T>& other) {
		return !(hor == other.hor && ver == other.ver && matrix == other.matrix);
	}
	template<typename U>
	friend std::ostream& operator<<(std::ostream&, sparse_matrix<U>&);
};

template<typename T>
std::ostream& operator<<(std::ostream& os, sparse_matrix<T>& matrix) {
	os << "\n\n";
	for (size_t i = 0; i < matrix.hor; i++) {
		for (size_t j = 0; j < matrix.ver; j++) {
			if (matrix.matrix.find(std::make_pair(i, j))) {
				os << matrix.matrix[std::make_pair(i, j)] << " ";
			} else os << T() << " ";
		}
		os << "\n";
	}
	os << "\n";
	return os;
}

template<typename T>
class sparse_mmatrix : public sparse_matrix<T> {
private:
	using base = sparse_matrix<T>;
public:
	sparse_mmatrix() = default;
	sparse_mmatrix(size_t hor, size_t ver) : base(hor, ver) {}
	sparse_mmatrix(const std::initializer_list<std::initializer_list<T>>&list) : base(list) {}
	sparse_mmatrix(DynamicArray<DynamicArray<T>>& matrix) : base(matrix) {}
	sparse_mmatrix(const sparse_mmatrix<T>& other) : base(other) {}
	sparse_mmatrix(sparse_mmatrix<T>&& other) {}
	~sparse_mmatrix() = default;

	sparse_mmatrix<T>& operator*=(sparse_mmatrix<T>& other) {
		if (other.ver != base::hor || other.hor != base::ver) throw SetException(IncorrectArraySize);
		sparse_mmatrix<T> cur = *this;
		this->ver = other.ver;
		for (size_t i = 0; i < cur.hor; i++) {
			for (size_t j = 0; j < other.ver; j++) {
				T value = T();
				for (size_t k = 0; k < cur.ver; k++) {
					value += cur.get(i + 1, k + 1) * other.get(k + 1, j + 1);
				}
				set(i + 1, j + 1, value);
			}
		}
		return *this;
	}
	sparse_mmatrix<T> operator*(sparse_mmatrix<T>& other) {
		if (other.ver != base::hor || other.hor != base::ver) throw SetException(IncorrectArraySize);
		sparse_mmatrix<T> res = sparse_mmatrix<T>(base::hor, other.ver);
		for (size_t i = 0; i < base::hor; i++) {
			for (size_t j = 0; j < other.ver; j++) {
				T value = T();
				for (size_t k = 0; k < base::ver; k++) {
					value += base::get(i + 1, k + 1) * other.get(k + 1, j + 1);
				}
				res.set(i + 1, j + 1, value);
			}
		}
		return res;
	}
	sparse_mmatrix<T> operator*(const T& other) {
		sparse_mmatrix<T> res(base::hor, base::ver);
		for (typename Dictionary<std::pair<size_t, size_t>, T>::iterator it = base::matrix.begin(); it != base::matrix.end(); it++) {
			res.matrix[(*it).first] = (*it).second[0] * other;
		}
		return res;
	}
	sparse_mmatrix<T>& operator*=(const T& other) {
		for (typename Dictionary<std::pair<size_t, size_t>, T>::iterator it = base::matrix.begin(); it != base::matrix.end(); it++) {
			(*it).second[0] *= other;
		}
		return *this;
	}
	sparse_mmatrix<T>& operator+=(sparse_mmatrix<T>& other) {
		if (other.hor != base::hor || other.ver != base::ver) throw SetException(IncorrectArraySize);
		for (size_t i = 0; i < base::hor; i++) {
			for (size_t j = 0; j < base::ver; j++) {
				T value = this->get(i + 1, j + 1) + other.get(i + 1, j + 1);
				set(i + 1, j + 1, value);
			}
		}
		return *this;
	}
	sparse_mmatrix<T> operator+(sparse_mmatrix<T>& other) {
		if (other.hor != base::hor || other.ver != base::ver) throw SetException(IncorrectArraySize);
		sparse_mmatrix<T> res = sparse_mmatrix<T>(base::hor, base::ver);
		for (size_t i = 0; i < base::hor; i++) {
			for (size_t j = 0; j < base::ver; j++) {
				T value = this->get(i + 1, j + 1) + other.get(i + 1, j + 1);
				res.set(i + 1, j + 1, value);
			}
		}
		return res;
	}
	sparse_mmatrix<T> operator+(const T& other) {
		sparse_mmatrix<T> res(base::hor, base::ver);
		for (size_t i = 0; i < base::hor; i++) {
			for (size_t j = 0; j < base::ver; j++) {
				T value = this->get(i + 1, j + 1) + other;
				res.set(i + 1, j + 1, value);
			}
		}
		return res;
	}
	sparse_mmatrix<T>& operator+=(const T& other) {
		for (size_t i = 0; i < base::hor; i++) {
			for (size_t j = 0; j < base::ver; j++) {
				T value = this->get(i + 1, j + 1) + other;
				set(i + 1, j + 1, value);
			}
		}
		return *this;
	}
	sparse_mmatrix<T>& operator-=(sparse_mmatrix<T>& other) {
		if (other.hor != base::hor || other.ver != base::ver) throw SetException(IncorrectArraySize);
		for (size_t i = 0; i < base::hor; i++) {
			for (size_t j = 0; j < base::ver; j++) {
				T value = this->get(i + 1, j + 1) - other.get(i + 1, j + 1);
				set(i + 1, j + 1, value);
			}
		}
		return *this;
	}
	sparse_mmatrix<T> operator-(sparse_mmatrix<T>& other) {
		if (other.hor != base::hor || other.ver != base::ver) throw SetException(IncorrectArraySize);
		sparse_mmatrix<T> res = sparse_mmatrix<T>(base::hor, base::ver);
		for (size_t i = 0; i < base::hor; i++) {
			for (size_t j = 0; j < base::ver; j++) {
				T value = this->get(i + 1, j + 1) - other.get(i + 1, j + 1);
				res.set(i + 1, j + 1, value);
			}
		}
		return res;
	}
	sparse_mmatrix<T> operator-(const T& other) {
		sparse_mmatrix<T> res(base::hor, base::ver);
		for (size_t i = 0; i < base::hor; i++) {
			for (size_t j = 0; j < base::ver; j++) {
				T value = this->get(i + 1, j + 1) - other;
				res.set(i + 1, j + 1, value);
			}
		}
		return res;
	}
	sparse_mmatrix<T>& operator-=(const T& other) {
		for (size_t i = 0; i < base::hor; i++) {
			for (size_t j = 0; j < base::ver; j++) {
				T value = this->get(i + 1, j + 1) - other;
				set(i + 1, j + 1, value);
			}
		}
		return *this;
	}
	sparse_mmatrix<T>& operator=(const sparse_mmatrix<T>& other) {
		base::operator=(other);
		return *this;
	}
	sparse_mmatrix<T>& operator=(sparse_mmatrix<T>&& other) {
		base::operator=(other);
		return *this;
	}
};

template<typename T>
class sparse_vector : private sparse_matrix<T> {
private:
	using base = sparse_matrix<T>;
	size_t _size;
public:
	sparse_vector() = default;
	sparse_vector(size_t sz) : base(1, sz) {
		_size = 0;
	}
	sparse_vector(const std::initializer_list<T>& list) : base({ list }) {
		_size = list.size();
	}
	sparse_vector(const sparse_vector<T>& other) : base(other) {
		_size = other._size;
	}
	sparse_vector(sparse_vector<T>&& other) : base(other) {
		_size = other._size;
		other._size = 0;
	}
	~sparse_vector() = default;
	size_t size() const noexcept {
		return _size;
	}
	void append(const T& item) {
		base::add_column({ item });
	}
	void erase(size_t pos) {
		base::remove_column(pos);
	}

	const T& get(size_t pos) {
		return base::get(1, pos);
	}

	template<typename U>
	friend std::ostream& operator<<(std::ostream&, sparse_vector<U>&);
};

template<typename T>
std::ostream& operator<<(std::ostream& os, sparse_vector<T>& matrix) {
	os << "{ ";
		for (size_t j = 0; j < matrix.ver; j++) {
			if (matrix.matrix.find(std::make_pair(0, j))) {
				os << matrix.matrix[std::make_pair(0, j)];
			}
			else os << T();
			if (j != matrix.ver - 1) os << ',';
			os << " ";
		}
	os << "}";
	return os;
}
