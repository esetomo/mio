#ifndef __RGBA_H__
#define __RGBA_H__

#include <mio/mio.h>
#include "rgb.h"

namespace mio
{
	struct MIO_API Rgba
	{
	public:
		float r, g, b, a;

		inline Rgba()
		{
			r = g = b = a = 0.0f;
		}

		inline Rgba(float c)
		{
			r = g = b = a = c;
		}

		inline Rgba(float _r, float _g, float _b)
		{
			r = _r;
			g = _g;
			b = _b;
			a = 1.0f;
		}

		inline Rgba(float _r, float _g, float _b, float _a)
		{
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}

		inline Rgba operator+(const Rgba &other) const {
			return Rgba(r + other.r, g + other.g, b + other.b, a + other.a);
		}

		inline Rgba operator-(const Rgba &other) const {
			return Rgba(r - other.r, g - other.g, b - other.b, a - other.a);
		}

		inline Rgba operator+(float c) const {
			return Rgba(r + c, g + c, b + c, a + c);
		}

		inline Rgba &operator+=(const Rgba &other)
		{
			r += other.r;
			g += other.g;
			b += other.b;
			a += other.a;
			return *this;
		}

		inline Rgba operator*(const Rgba &other) const {
			return Rgba(r * other.r, g * other.g, b * other.b, a * other.a);
		}

		inline Rgba operator*(float c) const {
			return Rgba(r * c, g * c, b * c, a * c);
		}

		inline Rgba &operator*=(const Rgba &other)
		{
			r *= other.r;
			g *= other.g;
			b *= other.b;
			a *= other.a;
			return *this;
		}

		inline Rgba operator/(const Rgba &other) const {
			return Rgba(r / other.r, g / other.g, b / other.b, a / other.a);
		}

		inline Rgba operator/(float c) const {
			float cinv = 1.0f / c;
			return Rgba(r * cinv, g * cinv, b * cinv, a * cinv);
		}

		inline void fill_array_4(float *_a)
		{
			_a[0] = r;
			_a[1] = g;
			_a[2] = b;
			_a[3] = a;
		}

		inline float *get_pointer() const
		{
			return (float *)this;
		}

		Rgba __copy__() const
		{
			return Rgba(r,g,b,a);
		}

		Rgba __deepcopy__() const
		{
			return Rgba(r,g,b,a);
		}

		bool is_near(const Rgba &other, float threshold=EPSILON)
		{
			float absr = fabs(r - other.r);
			float absg = fabs(g - other.g);
			float absb = fabs(b - other.b);
			float absa = fabs(a - other.a);
			return (absr < EPSILON && absg < EPSILON && absb < EPSILON && absa < EPSILON);
		}
	};

	inline Rgba operator+(float c, const Rgba &rgba)
	{
		return Rgba(rgba.r + c, rgba.g + c, rgba.b + c, rgba.a + c);
	}

	inline Rgba operator*(float c, const Rgba &rgba)
	{
		return Rgba(rgba.r * c, rgba.g * c, rgba.b * c, rgba.a * c);
	}

	/*
	inline ostream &operator<<(ostream &os, const Rgb &c) {
		os << c.r << ", " << c.g << ", " << c.b;
		return os;
	}
	*/
}

#endif