#ifndef __REFERENCE_H__
#define __REFERENCE_H__

#include <mio/reference_counted.h>
#include <mio/definitions.h>
#include <mio/mio.h>

namespace mio
{

template <class T>
class MIO_API Reference
{
private:
	T *pointer_;

public:
	inline Reference(T *_pointer = NULL)
	{
		if (_pointer != NULL)
		{
			_pointer->inc_ref();
		}
		pointer_ = _pointer;
	}

	inline Reference(const Reference<T> &other)
	{
		pointer_ = other.pointer_;
		if (pointer_ != NULL)
			pointer_->inc_ref();
	}

	inline Reference &operator=(T *_ptr)
	{				
		if (_ptr != pointer_)
		{
			if (pointer_ != NULL)
			{
				pointer_->dec_ref();				
			}

			pointer_ = _ptr;
			if (pointer_ != NULL)
			{
				pointer_->inc_ref();
			}			
		}
		return *this;
	}

	inline Reference &operator=(const Reference<T> &other)
	{
		*this = other.pointer_;
		return *this;
	}

	inline T *operator->() const
	{
		return pointer_;
	}

	inline operator bool() const
	{
		return pointer_ != NULL;
	}

	inline bool operator==(T *_ptr) const
	{
		return pointer_ == _ptr;
	}

	inline bool operator!=(T *_ptr) const
	{
		return pointer_ != _ptr;
	}

	inline bool operator==(const Reference<T> &other) const
	{
		return pointer_ == other.pointer_;
	}

	inline bool operator!=(const Reference<T> &other_) const
	{
		return pointer_ != other.pointer_;
	}

	inline bool operator<(T *_ptr) const
	{
		return pointer_ < _ptr;
	}

	inline bool operator<(const Reference<T> &other) 
	{
		return pointer_ < other.pointer;
	}

	virtual ~Reference()
	{
		if (pointer_ != NULL)
		{
			pointer_->dec_ref();
			pointer_ = NULL;
			//if (pointer_->get_ref_count() <= 0)
			//	  delete pointer_;
		}
	}

	inline T *get_pointer()
	{
		return pointer_;
	}
};

template <class T>
inline bool operator==(T *_ptr, const Reference<T> ref)
{
	return ref == _ptr;
}

template <class T>
inline bool operator!=(T *_ptr, const Reference<T> ref)
{
	return ref != _ptr;
}

template <class T>
inline bool operator<(T *_ptr, const Reference<T> ref)
{
	return ref < _ptr;
}

}

#endif