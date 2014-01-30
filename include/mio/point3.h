#ifndef __MIO_POINT3_H__
#define __MIO_POINT3_H__

#include <mio/mio.h>
#include <mio/definitions.h>
#include <mio/vector3.h>
#include <mio/point3.h>

namespace mio
{
	struct MIO_API Point3 {
	public:
		// Point3 Public Data
		float x,y,z;

		// Point3 Methods
		Point3(): x(0), y(0), z(0) 
		{
		}
		
		Point3(float c): x(c), y(c), z(c) 
		{
		}
		
		Point3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) 
		{
		}

		Point3 operator+(const Vector3 &v) const 
		{
			return Point3(x + v.x, y + v.y, z + v.z);
		}
		
		Point3 &operator+=(const Vector3 &v) 
		{
			x += v.x; y += v.y; z += v.z;
			return *this;
		}

		Vector3 operator-(const Point3 &p) const 
		{
			return Vector3(x - p.x, y - p.y, z - p.z);
		}
		
		Point3 operator-(const Vector3 &v) const 
		{
			return Point3(x - v.x, y - v.y, z - v.z);
		}
		
		Point3 &operator-=(const Vector3 &v) 
		{
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}
		
		Point3 &operator+=(const Point3 &p) 
		{
			x += p.x; y += p.y; z += p.z;
			return *this;
		}
		
		Point3 operator+(const Point3 &p) const 
		{
			return Point3(x + p.x, y + p.y, z + p.z);
		}
		
		Point3 operator* (float f) const 
		{
			return Point3(f*x, f*y, f*z);
		}
		
		Point3 &operator*=(float f) 
		{
			x *= f; y *= f; z *= f;
			return *this;
		}
		
		Point3 operator/ (float f) const 
		{
			float inv = 1.f/f;
			return Point3(inv*x, inv*y, inv*z);
		}
		
		Point3 &operator/=(float f) 
		{
			float inv = 1.f/f;
			x *= inv; y *= inv; z *= inv;
			return *this;
		}
		
		bool operator==(const Point3 &other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}

		bool operator!=(const Point3 &other) const
		{
			return x != other.x || y != other.y || z != other.z;
		}

		float operator[](int i) const 
		{ 
			return (&x)[i]; 
		}
		
		float &operator[](int i) 
		{ 
			return (&x)[i]; 
		}
		
		void fill_array_3(float *a)
		{
			a[0] = x;
			a[1] = y;
			a[2] = z;
		}

		void fill_array_4(float *a)
		{
			a[0] = x;
			a[1] = y;
			a[2] = z;
			a[3] = 1.0f;
		}

		Point3 __copy__() const
		{
			return Point3(x,y,z);
		}

		Point3 __deepcopy__() const
		{
			return Point3(x,y,z);
		}

		bool is_near(const Point3 &other, float threshold=EPSILON) const
		{
			float absx = fabs(x-other.x);
			float absy = fabs(y-other.y);
			float absz = fabs(z-other.z);
			return (absx < EPSILON && absy < EPSILON && absz < EPSILON);
		}
	};

	MIO_API inline float distance(const Point3 &p1, const Point3 &p2) 
	{
		return (p1 - p2).length();
	}
	
	MIO_API inline float distance_squared(const Point3 &p1, const Point3 &p2) 
	{
		return (p1 - p2).length_squared();
	}
	
	inline ostream &operator<<(ostream &os, const Point3 &v) {
		os << v.x << ", " << v.y << ", " << v.z;
		return os;
	}

	MIO_API inline Point3 operator*(float f, const Point3 &p) 
	{
		return p*f;
	}

}

#endif