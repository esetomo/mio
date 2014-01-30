#ifndef __MIO_QUATERNION_H__
#define __MIO_QUATERNION_H__

#include <mio/mio.h>
#include <mio/point3.h>
#include <mio/vector3.h>
#include <mio/matrix4x4.h>
#include <mio/exceptions.h>

namespace mio
{
	struct MIO_API Quaternion
	{
		float x, y, z, w;

		inline Quaternion()
		{
			x = y = z = 0.0f;
			w = 1.0f;
		}

		inline Quaternion(float _x, float _y, float _z, float _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}

		inline Quaternion(float degree, const Vector3 &axis)
		{
			float radian = radians(degree);
			w = cosf(radian / 2.0f);
			float len = axis.length();
			if (len < EPSILON)
				throw std::exception("axis is too short to normalize");			
			float s = sinf(radian / 2.0f);
			x = axis.x * s / len;
			y = axis.y * s / len;
			z = axis.z * s / len;
		}

		Quaternion(const Matrix4x4 &m);

		inline Quaternion operator+(const Quaternion &other) const
		{
			return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		inline Quaternion &operator+=(const Quaternion &other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}

		inline Quaternion operator-(const Quaternion &other) const
		{
			return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		inline Quaternion &operator-=(const Quaternion &other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}

		inline Quaternion operator*(const Quaternion &q) const
		{
			float _x =   x*q.w + y*q.z - z*q.y + w*q.x;
			float _y = - x*q.z + y*q.w + z*q.x + w*q.y;
			float _z =   x*q.y - y*q.x + z*q.w + w*q.z;
			float _w = - x*q.x - y*q.y - z*q.z + w*q.w;
			return Quaternion(_x, _y, _z, _w);
		}

		inline Quaternion operator*(float c) const
		{			
			return Quaternion(x*c, y*c, z*c, w*c);
		}

		inline Quaternion &operator*=(float c)
		{
			x *= c;
			y *= c;
			z *= c;
			w *= c;
			return *this;
		}

		Matrix4x4 get_matrix() const;				

		Quaternion __copy__() const
		{
			return Quaternion(x,y,z,w);
		}

		Quaternion __deepcopy__() const
		{
			return Quaternion(x,y,z,w);
		}

		bool is_near(const Quaternion &other, float threshold=EPSILON) const
		{
			float absx = fabs(x-other.x);
			float absy = fabs(y-other.y);
			float absz = fabs(z-other.z);
			float absw = fabs(w-other.w);
			return (absx < EPSILON && absy < EPSILON && absz < EPSILON && absw < EPSILON);
		}

		inline Vector3 rotate(const Vector3 &v) const
		{
			Quaternion vv = (*this) * Quaternion(v.x, v.y, v.z, 0) * Quaternion(-x, -y, -z, w);
			return Vector3(vv.x, vv.y, vv.z);
		}

		inline Point3 rotate(const Point3 &v) const
		{
			Quaternion vv = (*this) * Quaternion(v.x, v.y, v.z, 0) * Quaternion(-x, -y, -z, w);
			return Point3(vv.x, vv.y, vv.z);
		}
	};

	MIO_API inline Quaternion conjugate(const Quaternion &q)
	{
		return Quaternion(-q.x, -q.y, -q.z, q.w);
	}

	MIO_API inline float norm(const Quaternion &q)
	{
		return sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
	}

	MIO_API inline float norm_squared(const Quaternion &q)
	{
		return q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
	}

	MIO_API inline Quaternion normalize(const Quaternion &q)
	{
		float len = norm(q);
		if (len  < EPSILON)
			throw std::exception("quaternion is too short to normalize");
		float invlen = 1.0f / len;
		return Quaternion(q.x * invlen, q.y * invlen, q.z * invlen, q.w * invlen);
	}

	MIO_API inline Quaternion inverse(const Quaternion &q)
	{
		float len = norm(q);		
		if (len  < EPSILON)
			throw std::exception("quaternion is too short to find inverse");
		float invlen = 1.0f / len;
		return Quaternion(-q.x * invlen, -q.y * invlen, -q.z * invlen, q.w * invlen);
	}

	MIO_API inline float dot(const Quaternion &a, const Quaternion &b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}
	
	MIO_API inline Quaternion slerp(const Quaternion &a, const Quaternion &b, float x)
	{
		float cos_half_theta = dot(a,b);		
		if (cos_half_theta < 0)
			return slerp(a, b*(-1.0f), x);
		else if (fabsf(cos_half_theta) >= 1.0)
			return a;		
		else
		{
			float half_theta = acosf(cos_half_theta);
			float sin_half_theta = sqrtf(1.0f - cos_half_theta * cos_half_theta);
			if (fabsf(sin_half_theta) < EPSILON)
				return a;
				/*
				return Quaternion(
					(a.x + b.x) * 0.5f,
					(a.y + b.y) * 0.5f,
					(a.z + b.z) * 0.5f,
					(a.w + b.w) * 0.5f);
				*/
			else
			{
				float ratio_0 = sinf((1-x) * half_theta) / sin_half_theta;
				float ratio_1 = sinf(x * half_theta) / sin_half_theta;
				return Quaternion(
					ratio_0 * a.x + ratio_1 * b.x,
					ratio_0 * a.y + ratio_1 * b.y,
					ratio_0 * a.z + ratio_1 * b.z,
					ratio_0 * a.w + ratio_1 * b.w);
			}
		}
	}
}

#endif