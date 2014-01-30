/*
 * pbrt source code Copyright(c) 1998-2005 Matt Pharr and Greg Humphreys
 *
 * All Rights Reserved.
 * For educational use only; commercial use expressly forbidden.
 * NO WARRANTY, express or implied, for this software.
 * (See file License.txt for complete license)
 */

#ifndef __MIO_VECTOR_H__
#define __MIO_VECTOR_H__

#include <mio/mio.h>
#include <mio/definitions.h>
#include <mio/exceptions.h>

namespace mio
{

	struct Point;
	struct Normal;

	struct MIO_API Vector {
	public:
		// Vector Public Methods
		Vector(): x(0), y(0), z(0) {}
		Vector(float c): x(c), y(c), z(c) {}
		Vector(float _x, float _y, float _z)
			: x(_x), y(_y), z(_z) {
		}
		explicit Vector(const Point &p);
		Vector operator+(const Vector &v) const {
			return Vector(x + v.x, y + v.y, z + v.z);
		}
		
		Vector& operator+=(const Vector &v) {
			x += v.x; y += v.y; z += v.z;
			return *this;
		}
		Vector operator-(const Vector &v) const {
			return Vector(x - v.x, y - v.y, z - v.z);
		}
		
		Vector& operator-=(const Vector &v) {
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}
		bool operator==(const Vector &v) const {
			return x == v.x && y == v.y && z == v.z;
		}
		Vector operator*(float f) const {
			return Vector(f*x, f*y, f*z);
		}
		
		Vector &operator*=(float f) {
			x *= f; y *= f; z *= f;
			return *this;
		}
		Vector operator/(float f) const {		
			float inv = 1.f / f;
			return Vector(x * inv, y * inv, z * inv);
		}
		
		Vector &operator/=(float f) {		
			float inv = 1.f / f;
			x *= inv; y *= inv; z *= inv;
			return *this;
		}
		Vector operator-() const {
			return Vector(-x, -y, -z);
		}
		float operator[](int i) const {		
			return (&x)[i];
		}
		bool operator!=(const Vector &other) const
		{
			return !(*this == other);
		}
		float &operator[](int i) {
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
		float length_squared() const { return x*x + y*y + z*z; }
		float length() const { return sqrtf(length_squared()); }
		explicit Vector(const Normal &n);
		// Vector Public Data
		float x, y, z;
	};

	struct MIO_API Point {
	public:
		// Point Methods
		Point(): x(0), y(0), z(0) {}
		Point(float c): x(c), y(c), z(c) {}
		Point(float _x, float _y, float _z)
			: x(_x), y(_y), z(_z) {
		}
		Point operator+(const Vector &v) const {
			return Point(x + v.x, y + v.y, z + v.z);
		}
		
		Point &operator+=(const Vector &v) {
			x += v.x; y += v.y; z += v.z;
			return *this;
		}
		Vector operator-(const Point &p) const {
			return Vector(x - p.x, y - p.y, z - p.z);
		}
		
		Point operator-(const Vector &v) const {
			return Point(x - v.x, y - v.y, z - v.z);
		}
		
		Point &operator-=(const Vector &v) {
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}
		Point &operator+=(const Point &p) {
			x += p.x; y += p.y; z += p.z;
			return *this;
		}
		Point operator+(const Point &p) const {
			return Point(x + p.x, y + p.y, z + p.z);
		}
		Point operator* (float f) const {
			return Point(f*x, f*y, f*z);
		}
		Point &operator*=(float f) {
			x *= f; y *= f; z *= f;
			return *this;
		}
		Point operator/ (float f) const {
			float inv = 1.f/f;
			return Point(inv*x, inv*y, inv*z);
		}
		Point &operator/=(float f) {
			float inv = 1.f/f;
			x *= inv; y *= inv; z *= inv;
			return *this;
		}
		bool operator==(const Point &other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}
		bool operator!=(const Point &other) const
		{
			return x != other.x || y != other.y || z != other.z;
		}
		float operator[](int i) const { return (&x)[i]; }
		float &operator[](int i) { return (&x)[i]; }
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
		// Point Public Data
		float x,y,z;
	};

	struct MIO_API Normal {
	public:
		// Normal Methods
		Normal(): x(0), y(0), z(0) {}
		Normal(float c): x(c), y(c), z(c) {}
		Normal(float _x, float _y, float _z)
			: x(_x), y(_y), z(_z) {}
		Normal operator-() const {
			return Normal(-x, -y, -z);
		}
		Normal operator+ (const Normal &v) const {
			return Normal(x + v.x, y + v.y, z + v.z);
		}
		
		Normal& operator+=(const Normal &v) {
			x += v.x; y += v.y; z += v.z;
			return *this;
		}
		Normal operator- (const Normal &v) const {
			return Normal(x - v.x, y - v.y, z - v.z);
		}
		
		Normal& operator-=(const Normal &v) {
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}
		Normal operator* (float f) const {
			return Normal(f*x, f*y, f*z);
		}
		
		Normal &operator*=(float f) {
			x *= f; y *= f; z *= f;
			return *this;
		}
		Normal operator/ (float f) const {
			float inv = 1.f/f;
			return Normal(x * inv, y * inv, z * inv);
		}
		
		Normal &operator/=(float f) {
			float inv = 1.f/f;
			x *= inv; y *= inv; z *= inv;
			return *this;
		}
		bool operator==(const Normal &other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}
		bool operator!=(const Normal &other) const
		{
			return !(*this == other);
		}
		float length_squared() const { return x*x + y*y + z*z; }
		float length() const        { return sqrtf(length_squared()); }
		
		explicit Normal(const Vector &v)
		  : x(v.x), y(v.y), z(v.z) {}
		float operator[](int i) const { return (&x)[i]; }
		float &operator[](int i) { return (&x)[i]; }
		// Normal Public Data
		float x,y,z;
	};

	// Geometry Inline Functions
	MIO_API inline Vector::Vector(const Point &p) : x(p.x), y(p.y), z(p.z) 
	{
	}
	/*
	inline ostream &operator<<(ostream &os, const Vector &v) {
		os << v.x << ", " << v.y << ", " << v.z;
		return os;
	}
	*/
	MIO_API inline Vector operator*(float f, const Vector &v) {
		return v*f;
	}
	MIO_API inline float dot(const Vector &v1, const Vector &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	MIO_API inline float absdot(const Vector &v1, const Vector &v2) {
		return fabsf(dot(v1, v2));
	}
	MIO_API inline Vector cross(const Vector &v1, const Vector &v2) {
		return Vector((v1.y * v2.z) - (v1.z * v2.y),
					  (v1.z * v2.x) - (v1.x * v2.z),
					  (v1.x * v2.y) - (v1.y * v2.x));
	}
	MIO_API inline Vector cross(const Vector &v1, const Normal &v2) {
		return Vector((v1.y * v2.z) - (v1.z * v2.y),
					  (v1.z * v2.x) - (v1.x * v2.z),
					  (v1.x * v2.y) - (v1.y * v2.x));
	}
	MIO_API inline Vector cross(const Normal &v1, const Vector &v2) {
		return Vector((v1.y * v2.z) - (v1.z * v2.y),
					  (v1.z * v2.x) - (v1.x * v2.z),
					  (v1.x * v2.y) - (v1.y * v2.x));
	}
	MIO_API inline Vector normalize(const Vector &v) {
		return v / v.length();
	}
	MIO_API inline void coordinate_system(const Vector &v1, Vector *v2, Vector *v3) {
		if (fabsf(v1.x) > fabsf(v1.y)) {
			float invLen = 1.f / sqrtf(v1.x*v1.x + v1.z*v1.z);
			*v2 = Vector(-v1.z * invLen, 0.f, v1.x * invLen);
		}
		else {
			float invLen = 1.f / sqrtf(v1.y*v1.y + v1.z*v1.z);
			*v2 = Vector(0.f, v1.z * invLen, -v1.y * invLen);
		}
		*v3 = cross(v1, *v2);
	}
	MIO_API inline float distance(const Point &p1, const Point &p2) {
		return (p1 - p2).length();
	}
	MIO_API inline float distance_squared(const Point &p1, const Point &p2) {
		return (p1 - p2).length_squared();
	}
	/*
	inline ostream &operator<<(ostream &os, const Point &v) {
		os << v.x << ", " << v.y << ", " << v.z;
		return os;
	}
	*/
	MIO_API inline Point operator*(float f, const Point &p) {
		return p*f;
	}
	MIO_API inline Normal operator*(float f, const Normal &n) {
		return Normal(f*n.x, f*n.y, f*n.z);
	}
	MIO_API inline Normal normalize(const Normal &n) {
		return n / n.length();
	}
	MIO_API inline Vector::Vector(const Normal &n)
	  : x(n.x), y(n.y), z(n.z) { }
	MIO_API inline float dot(const Normal &n1, const Vector &v2) {
		return n1.x * v2.x + n1.y * v2.y + n1.z * v2.z;
	}
	MIO_API inline float dot(const Vector &v1, const Normal &n2) {
		return v1.x * n2.x + v1.y * n2.y + v1.z * n2.z;
	}
	MIO_API inline float dot(const Normal &n1, const Normal &n2) {
		return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z;
	}
	MIO_API inline float absdot(const Normal &n1, const Vector &v2) {
		return fabsf(n1.x * v2.x + n1.y * v2.y + n1.z * v2.z);
	}
	MIO_API inline float absdot(const Vector &v1, const Normal &n2) {
		return fabsf(v1.x * n2.x + v1.y * n2.y + v1.z * n2.z);
	}
	MIO_API inline float absdot(const Normal &n1, const Normal &n2) {
		return fabsf(n1.x * n2.x + n1.y * n2.y + n1.z * n2.z);
	}
	

	/*
	inline ostream &operator<<(ostream &os, const Normal &v) {
		os << v.x << ", " << v.y << ", " << v.z;
		return os;
	}
	
	inline ostream &operator<<(ostream &os, Ray &r) {
		os << "org: " << r.o << "dir: " << r.d;
		
			//<< " range [" <<
			//r.mint << "," << r.maxt << "] time = " <<
			//r.time;
			
		return os;
	}
	
	inline ostream &operator<<(ostream &os, const BBox &b) {
		os << b.pMin << " -> " << b.pMax;
		return os;
	}
	*/
	
	MIO_API inline Vector spherical_direction(float sintheta,
								  float costheta, float phi) {
		return Vector(sintheta * cosf(phi),
					  sintheta * sinf(phi),
					  costheta);
	}

	MIO_API inline Vector spherical_direction(float sintheta,
									   float costheta,
									   float phi,
									   const Vector &x,
									   const Vector &y,
									   const Vector &z) {
		return sintheta * cosf(phi) * x +
			sintheta * sinf(phi) * y + costheta * z;
	}

	MIO_API inline float spherical_theta(const Vector &v) {
		return acosf(clamp(v.z, -1.f, 1.f));
	}

	MIO_API inline float spherical_phi(const Vector &v) {
		float p = atan2f(v.y, v.x);
		return (p < 0.f) ? p + 2.f*M_PI : p;
	}
}

#endif
