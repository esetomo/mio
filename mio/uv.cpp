#include <mio/uv.h>

namespace mio
{
	Uv::Uv()
	{
		u = 0;
		v = 0;
	}

	Uv::Uv(float c)
	{
		u = v = c;
	}

	Uv::Uv(float _u, float _v)
	{
		u = _u;
		v = _v;
	}

	bool Uv::operator==(const Uv &other) const
	{
		return (u == other.u) && (v == other.v);
	}
}