/*
 * pbrt source code Copyright(c) 1998-2005 Matt Pharr and Greg Humphreys
 *
 * All Rights Reserved.
 * For educational use only; commercial use expressly forbidden.
 * NO WARRANTY, express or implied, for this software.
 * (See file License.txt for complete license)
 */

#ifndef __MIO_VECTOR3_H__
#define __MIO_VECTOR3_H__

#include <mio/mio.h>
#include <mio/definitions.h>
#include <mio/exceptions.h>
#include <iostream>

namespace mio
{
	struct Point3;

	struct MIO_API Vector3 
	{
	public:
		// Vector3 Public Data
		float x, y, z;

		// Vector3 Public Methods
		Vector3(): x(0), y(0), z(0) 
		{
		}
		
		Vector3(float c): x(c), y(c), z(c) 
		{
		}
		
		Vector3(float _x, float _y, float _z)
			: x(_x), y(_y), z(_z) 
		{
		}
		
		explicit Vector3(const Point3 &p);

		Vector3 operator+(const Vector3 &v) const 
		{
			return Vector3(x + v.x, y + v.y, z + v.z);
		}
		
		Vector3& operator+=(const Vector3 &v) 
		{
			x += v.x; y += v.y; z += v.z;
			return *this;
		}

		Vector3 operator-(const Vector3 &v) const 
		{
			return Vector3(x - v.x, y - v.y, z - v.z);
		}
		
		Vector3& operator-=(const Vector3 &v) 
		{
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}
		
		bool operator==(const Vector3 &v) const 
		{
			return x == v.x && y == v.y && z == v.z;
		}
		
		Vector3 operator*(float f) const 
		{
			return Vector3(f*x, f*y, f*z);
		}
		
		
		Vector3 &operator*=(float f) 
		{
			x *= f; y *= f; z *= f;
			return *this;
		}
		
		Vector3 operator/(float f) const 
		{
			float inv = 1.f / f;
			return Vector3(x * inv, y * inv, z * inv);
		}
		
		Vector3 &operator/=(float f) 
		{
			float inv = 1.f / f;
			x *= inv; y *= inv; z *= inv;
			return *this;
		}
		
		Vector3 operator-() const 
		{
			return Vector3(-x, -y, -z);
		}
		
		float operator[](int i) const 
		{
			return (&x)[i];
		}
		
		bool operator!=(const Vector3 &other) const
		{
			return !(*this == other);
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
			a[3] = 0.0f;
		}
		
		float length_squared() const 
		{ 
			return x*x + y*y + z*z; 
		}
		
		float length() const 
		{ 
			return sqrtf(length_squared()); 
		}

		Vector3 __copy__() const
		{
			return Vector3(x,y,z);
		}

		Vector3 __deepcopy__() const
		{
			return Vector3(x,y,z);
		}

		bool is_near(const Vector3 &other, float threshold=EPSILON) const
		{
			float absx = fabs(x-other.x);
			float absy = fabs(y-other.y);
			float absz = fabs(z-other.z);
			return (absx < EPSILON && absy < EPSILON && absz < EPSILON);
		}
	};	

	inline std::ostream &operator<<(std::ostream &os, const Vector3 &v)
	{
		os << v.x << ", " << v.y << ", " << v.z;
		return os;
	}
	
	MIO_API inline Vector3 operator*(float f, const Vector3 &v) 
	{
		return v*f;
	}

	MIO_API inline float dot(const Vector3 &v1, const Vector3 &v2) 
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	MIO_API inline float absdot(const Vector3 &v1, const Vector3 &v2) 
	{
		return fabsf(dot(v1, v2));
	}

	MIO_API inline Vector3 cross(const Vector3 &v1, const Vector3 &v2) 
	{
		return Vector3((v1.y * v2.z) - (v1.z * v2.y),
					  (v1.z * v2.x) - (v1.x * v2.z),
					  (v1.x * v2.y) - (v1.y * v2.x));
	}
	
	MIO_API inline Vector3 normalize(const Vector3 &v) 
	{
		return v / v.length();
	}

	MIO_API inline void coordinate_system(const Vector3 &v1, Vector3 *v2, Vector3 *v3) 
	{
		if (fabsf(v1.x) > fabsf(v1.y)) 
		{
			float invLen = 1.f / sqrtf(v1.x*v1.x + v1.z*v1.z);
			*v2 = Vector3(-v1.z * invLen, 0.f, v1.x * invLen);
		}
		else 
		{
			float invLen = 1.f / sqrtf(v1.y*v1.y + v1.z*v1.z);
			*v2 = Vector3(0.f, v1.z * invLen, -v1.y * invLen);
		}
		*v3 = cross(v1, *v2);
	}
	
	MIO_API inline Vector3 spherical_direction(float sintheta,
		float costheta, float phi) 
	{
		return Vector3(sintheta * cosf(phi),
					  sintheta * sinf(phi),
					  costheta);
	}

	MIO_API inline Vector3 spherical_direction(
		float sintheta,
		float costheta,
		float phi,
		const Vector3 &x,
		const Vector3 &y,
		const Vector3 &z) 
	{
		return sintheta * cosf(phi) * x +
			sintheta * sinf(phi) * y + costheta * z;
	}

	MIO_API inline float spherical_theta(const Vector3 &v) 
	{
		return acosf(clamp(v.z, -1.f, 1.f));
	}

	MIO_API inline float spherical_phi(const Vector3 &v) {
		float p = atan2f(v.y, v.x);
		return (p < 0.f) ? p + 2.f*M_PI : p;
	}
}

#endif
