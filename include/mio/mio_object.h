#ifndef __MIO_OBJECT_H__
#define __MIO_OBJECT_H__

#include <mio/mio.h>
#include <mio/reference_counted.h>
#include <string>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
	class MIO_API MioObject : public mio::ReferenceCounted
	{	
	private:
		std::string uid_;
	public:
		MioObject(const char *uid="");
		virtual ~MioObject();

		const char *get_uid() const;
		void set_uid(const char *uid);
	};
}

#pragma warning(pop)

#endif