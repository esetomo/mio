#ifndef __MIO_UV_H__
#define __MIO_UV_H__

#include <mio/mio.h>

namespace mio
{
	struct MIO_API Uv
	{
	public:
		float u, v;

		Uv();		
		Uv(float c);		
		Uv(float _u, float _v);		
		bool operator==(const Uv &other) const;		
	};
}

#endif