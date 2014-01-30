#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <mio/mio.h>
#include <mio/matrix4x4.h>

namespace mio
{

struct MIO_API Transform
{
public:
	Matrix4x4 m;
	Matrix4x4 mi;
	Matrix4x4 mit;

	Transform();
	Transform(const Matrix4x4 &_m, const Matrix4x4 &_mi);
	Transform(const Matrix4x4 &_m, const Matrix4x4 &_mi, const Matrix4x4 &_mit);

	Transform operator*(const Transform &other) const;
	Transform &operator*=(const Transform &other);

	static Transform identity();

	static Transform translate(float x, float y, float z);
	static Transform translate_x(float x);
	static Transform translate_y(float y);
	static Transform translate_z(float z);

	static Transform scale(float x, float y, float z);
	static Transform scale(float s);
	static Transform scale_x(float x);
	static Transform scale_y(float y);
	static Transform scale_z(float z);

	static Transform rotate(float degrees, Vector3 axis);
	static Transform rotate_x(float degrees);
	static Transform rotate_y(float degrees);
	static Transform rotate_z(float degrees);
};

}

#endif