#ifndef __MIO_RAY_H__
#define __MIO_RAY_H__

#include <mio/mio.h>
#include <mio/vector3.h>
#include <mio/point3.h>
#include <iostream>

namespace mio
{
	struct MIO_API Ray {
	public:
		// Ray Public Methods
		Ray() : origin(0,0,0), direction(0,0,1)
		{
		}

		Ray(const Point3 &origin, const Vector3 &direction)
			: origin(origin), direction(direction)
		{ 
		}

		Point3 operator()(float t) const 
		{ 
			return origin + direction * t; 
		}
		// Ray Public Data
		Point3 origin;
		Vector3 direction;
		//mutable float mint, maxt;
		//float time;
	};
		
	inline std::ostream &operator<<(std::ostream &os, Ray &r) 
	{
		os << "org: " << r.origin << "dir: " << r.direction;
		
			//<< " range [" <<
			//r.mint << "," << r.maxt << "] time = " <<
			//r.time;
			
		return os;
	}
}

#endif

