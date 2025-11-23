#pragma once

#include<algorithm>
template<typename T>
class Wector {
private:
	T* arr=nullptr;
	size_t vsize = 0;
	size_t vcapacity = 0;
public:
	Wector(const Wector<T>& other) {
		vsize = other.vsize;
		vcapacity = other.vcapacity;
		arr = new T[vcapacity];
		std::copy(other.arr, other.arr + vsize, arr);
	}
	Wector(const size_t sz = 0,const T& elem=T()) {
		vsize = sz;
		vcapacity = vsize * 1.3+2;
		arr = new T[vcapacity];
		std::fill(arr, arr + vsize, elem);
	}
	~Wector() {
		/*for (int i = 0; i < vsize; i++) {
			arr[i].~T();
		}*/
		delete[] arr;
	}
	Wector<T>& operator=(const Wector<T>& other) {
		if (this != &other) {
			delete[] arr;
			vsize = other.vsize;
			vcapacity = other.vcapacity;
			arr = new T[vcapacity];
			std::copy(other.arr, other.arr + vsize, arr);
		}
		return *this;
	}
	void reserve(const size_t newsz) {
		vcapacity = std::max(newsz,vcapacity);
		T* tmp = new T[vcapacity];
		std::copy(arr, arr + vsize, tmp);
		/*std::swap(tmp, arr);
		delete[] tmp;*/
		delete[] arr;
		arr = tmp;         
		vcapacity = newsz; 
	}
	void resize(const size_t newsz) {
		vcapacity = newsz * 1.3 + 2;
		T* tmp = new T[vcapacity];
		std::copy(arr, arr + vsize, tmp);
		std::fill(tmp + vsize, tmp + newsz, T());
		/*std::swap(tmp, arr);
		vsize = newsz;
		delete[] tmp;*/
		delete[] arr;
		arr = tmp;
		vsize = newsz;
	}
	void push_back(const T& el) {
		++vsize;
		if (vcapacity <= vsize) {
			vcapacity = vcapacity * 1.3 + 2;
			T* tmp = new T[vcapacity];
			std::copy(arr, arr + vsize - 1, tmp);
			/*std::swap(tmp, arr);
			delete[] tmp;*/
			delete[] arr;
			arr = tmp;
		}
		arr[vsize - 1] = el;
	}
	void insert(const size_t pos, const T& el) {
		++vsize;
		if (vcapacity <= vsize) {
			vcapacity = vcapacity * 1.3 + 1;
		}
		++vcapacity;
		T* tmp = new T[vcapacity];
		if (pos == 0) {
			tmp[0] = el;
			std::copy(arr, arr + vsize - 1, tmp + 1);
			/*std::swap(tmp, arr);
			delete[] tmp;*/
			delete[] arr;
			arr = tmp;
			return;
		}
		std::copy(arr, arr + pos, tmp);
		tmp[pos] = el;
		std::copy(arr + pos, arr + vsize - 1, tmp + pos + 1);
		std::swap(tmp, arr);
		delete[] tmp;
	}
	void erase(const size_t pos) {
		if (vsize == 1) {
			vsize = 0;
			arr[pos] = T();
			return;
		}
		--vsize;
		T* tmp = new T[vcapacity];
		std::copy(arr, arr + pos, tmp);
		std::copy(arr + pos + 1, arr + vsize + 1, tmp + pos);
		/*std::swap(arr, tmp);
		delete[] tmp;*/
		delete[] arr;
		arr = tmp;

	}
	size_t size() const{
		return vsize;
	}
	size_t capacity() const {
		return vcapacity;
	}
	T& operator[](const size_t pos) const {
		return arr[pos];
	}
	T& operator[](const size_t pos) {
		return arr[pos];
	}
};