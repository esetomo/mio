#ifndef __MIO_ARRAY_H__
#define __MIO_ARRAY_H__

#include <stdlib.h> 

#define ARRAY_INITIAL_CAPACITY 16

#define COPY_OLD_DATA true
#define DONT_COPY_OLD_DATA false

namespace mio
{

template <typename T>
class Array {
private:
	T *data_;
	int size_;
	int capacity_;

public:
	void allocate(int newCapacity, bool copyOldData);

	inline T &operator[](int index) const
	{
		return data_[index];
	}

	inline T &get_element(int index) const
	{
		return data_[index];
	}

	inline T *get_pointer(int index=0) const 
	{
		return data_ + index;
	}

	inline void push_back(const T &item) 
	{
		while (size_ >= capacity_) {
			allocate(capacity_ * 2, true);
		}
		data_[size_] = item;
		size_++;
	}

	Array(int initialCapacity);
	Array();

	inline void adjust_capacity(int newCapacity, bool copyOldData)
	{
		if (capacity_ >= newCapacity)
			return;
		int toAllocCapacity = capacity_;
		while (newCapacity > toAllocCapacity)
			toAllocCapacity *= 2;
		allocate(toAllocCapacity, copyOldData);
	}

	inline void adjust_size(int newSize, bool copyOldData)
	{
		adjust_capacity(newSize, copyOldData);
		size_ = newSize;
	}

	inline void clear() 
	{
		size_ = 0;
	}

	int size() const 
	{
		return size_;
	}

	inline void trim(int newSize)
	{
		size_ = newSize;
	}

	virtual ~Array(); 

	Array &operator=(const Array &other) 
	{
		allocate(other.capacity_, false);
		size_ = other.size_;
		for(int i=0;i<size_;i++) {
			data_[i] = other.data_[i];
		}
		return *this;
	}

	inline void pop_back() 
	{
		if (size_ > 0)
			size_--;
	}
	
	inline T &back() const 
	{
		return data[size_-1];
	}
};

/**
* @brief Quicksort the portion of the vector from Index \a first to Index \a last inclusive.
**/
template <typename T>
void sort(Array<T> &a, int first, int last);

#define ADJUST_ARRAY_SIZE(__list, __desiredSize, __newElement) \
	do { \
		while(__list.size() < __desiredSize) \
			__list.push_back(__newElement); \
		__list.trim(__desiredSize); \
	} while (false);

#define ADJUST_POINTER_ARRAY_SIZE(__list, __desiredSize, __itemType, __constructor) \
	do { \
		while(__list.size() < __desiredSize) \
			__list.push_back(__constructor); \
		while(__list.size() > __desiredSize) \
		{ \
			__itemType *ptr = __list.back(); \
			if (ptr != NULL) delete ptr; \
			__list.pop_back(); \
		} \
	} while(false); 

#define DEALLOCATE_POINTER_ARRAY(__list) \
	do { \
		FOR(i, (int)__list.size()) \
			if (__list[i] != NULL) \
				delete __list[i]; \
	} while(false);

template <typename T>
void Array<T>::allocate(int newCapacity, bool copyOldData) 
{
	T *newArray = new T[newCapacity];		
	if (data_ != NULL && copyOldData) 
	{
		try
		{
			for(int i=0;i<size_;i++) 		
				newArray[i] = data_[i];
		}
		catch(...)
		{
			delete newArray;
			throw;
		}
	}
	capacity_ = newCapacity;
	if (data_ != NULL) 
		delete data_;
	data_ = newArray;
}

template <typename T>
void sort(Array<T> &a, int first, int last) 
{
	if (first >= last) return;
	int pivot = first + rand() % (last-first+1);
	T key = a[pivot];
	T temp = a[first]; a[first] = a[pivot]; a[pivot] = temp;
	int s = first;
	for(int k = first+1; k <= last; k++) 
	{
		if (a[k] < key) {
			s++;
			temp = a[k];
			a[k] = a[s];
			a[s] = temp;
		}
	}
	temp = a[first]; a[first] = a[s]; a[s] = temp;
	sort(a, first, s-1);
	sort(a, s+1, last);
}

template <typename T>
Array<T>::Array(int initialCapacity) {
	data = NULL;
	size_ = 0;
	allocate(initialCapacity, false);	
}

template <typename T>
Array<T>::Array() {
	data_ = NULL;
	size_ = 0;
	allocate(ARRAY_INITIAL_CAPACITY, false);	
}

template <typename T>
Array<T>::~Array()
{
	if (data_ != NULL)
		delete data_;		
	data_ = NULL;
}

}

#endif