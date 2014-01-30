#ifndef __Rgb_H__
#define __Rgb_H__

#include <mio/mio.h>
#include <mio/definitions.h>

namespace mio
{
	struct MIO_API Rgb 
	{
	public:
		float r, g, b;

		inline Rgb()
		{
			r = g = b = 0.0f;
		}

		inline Rgb(float c)
		{
			r = g = b = c;
		}

		inline Rgb(float _r, float _g, float _b)
		{
			r = _r;
			g = _g;
			b = _b;
		}

		inline Rgb operator+(const Rgb &other) const {
			return Rgb(r + other.r, g + other.g, b + other.b);
		}

		inline Rgb operator-(const Rgb &other) const {
			return Rgb(r - other.r, g - other.g, b - other.b);
		}

		inline Rgb operator+(float c) const {
			return Rgb(r + c, g + c, b + c);
		}

		inline Rgb &operator+=(const Rgb &other)
		{
			r += other.r;
			g += other.g;
			b += other.b;
			return *this;
		}

		inline Rgb operator*(const Rgb &other) const {
			return Rgb(r * other.r, g * other.g, b * other.b);
		}

		inline Rgb operator*(float c) const {
			return Rgb(r * c, g * c, b * c);
		}

		inline Rgb &operator*=(const Rgb &other)
		{
			r *= other.r;
			g *= other.g;
			b *= other.b;
			return *this;
		}

		inline Rgb operator/(const Rgb &other) const {
			return Rgb(r / other.r, g / other.g, b / other.b);
		}

		inline Rgb operator/(float c) const {
			float cinv = 1.0f / c;
			return Rgb(r * cinv, g * cinv, b * cinv);
		}

		Rgb __copy__() const
		{
			return Rgb(r,g,b);
		}

		Rgb __deepcopy__() const
		{
			return Rgb(r,g,b);
		}

		bool is_near(const Rgb &other, float threshold=EPSILON)
		{
			float absr = fabs(r - other.r);
			float absg = fabs(g - other.g);
			float absb = fabs(b - other.b);
			return (absr < EPSILON && absg < EPSILON && absb < EPSILON);
		}
	};

	inline Rgb operator+(float c, const Rgb &rgb)
	{
		return Rgb(rgb.r + c, rgb.g + c, rgb.b + c);
	}

	inline Rgb operator*(float c, const Rgb &rgb)
	{
		return Rgb(rgb.r * c, rgb.g * c, rgb.b * c);
	}

	/*
	inline ostream &operator<<(ostream &os, const Rgb &c) {
		os << c.r << ", " << c.g << ", " << c.b;
		return os;
	}
	*/
}

#endif