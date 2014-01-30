#ifndef __REFERENCE_COUNTED_H__
#define __REFERENCE_COUNTED_H__

#include <mio/mio.h>

namespace mio
{

class MIO_API ReferenceCounted
{
private:
	int ref_count_;

public:
	ReferenceCounted() 
	{
		ref_count_ = 0;
	}

	virtual ~ReferenceCounted()
	{
	}

	inline void inc_ref()
	{
		ref_count_++;
	}

	inline void dec_ref()
	{
		ref_count_--;
		if (ref_count_ <= 0)
			delete this;
	}

	inline int get_ref_count()
	{
		return ref_count_;
	}
};

}

#endif