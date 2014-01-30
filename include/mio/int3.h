#ifndef __MIO_INT3_H__
#define __MIO_INT3_H__

#include <mio/mio.h>

namespace mio
{
	struct MIO_API Int3
	{
	private:
		int values[3];

	public:
		Int3()
		{
			values[0] = values[1] = values[2] = 0;
		}

		Int3(int v0, int v1, int v2)
		{
			values[0] = v0;
			values[1] = v1;
			values[2] = v2;
		}

		inline int operator[](int i) const
		{
			return values[i];
		}

		inline int &operator[](int i)
		{
			return values[i];
		}

		inline int _get(int i)
		{
			return values[i];
		}

		inline void _set(int i, int value)
		{
			values[i] = value;
		}
	};
}

#endif