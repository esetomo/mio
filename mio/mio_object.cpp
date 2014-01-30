#include <mio/mio_object.h>

namespace mio
{
	MioObject::MioObject(const char *uid)
	{
		uid_ = uid;
	}

	MioObject::~MioObject()
	{
	}

	const char *MioObject::get_uid() const
	{
		return uid_.c_str();
	}

	void MioObject::set_uid(const char *name)
	{
		uid_ = name;
	}
}