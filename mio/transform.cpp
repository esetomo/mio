#include <mio/transform.h>

namespace mio
{

Transform::Transform()
{
	m = Matrix4x4::identity();
	mi = Matrix4x4::identity();
	mit = Matrix4x4::identity();
}

Transform::Transform(const Matrix4x4 &_m, const Matrix4x4 &_mi)
{
	m = _m;
	mi = _mi;
	mit = transpose(mi);
}

Transform::Transform(const Matrix4x4 &_m, const Matrix4x4 &_mi, const Matrix4x4 &_mit)
{
	m = _m;
	mi = _mi;
	mit = _mit;
}

Transform Transform::operator*(const Transform &other) const
{
	return Transform(m * other.m, mi * other.mi, other.mit * mit);
}

Transform &Transform::operator*=(const Transform &other)
{
	m = m * other.m;
	mi = other.mi * mi;
	mit = mit * other.mit;
	return *this;
}

Transform Transform::identity()
{
	return Transform();
}

Transform Transform::translate(float x, float y, float z)
{
	return Transform(
		Matrix4x4::translate(x,y,z),
		Matrix4x4::translate(-x,-y,-z));
}

Transform Transform::translate_x(float x)
{
	return translate(x, 0, 0);
}

Transform Transform::translate_y(float y)
{
	return translate(0, y, 0);
}

Transform Transform::translate_z(float z)
{
	return translate(0, 0, z);
}

Transform Transform::scale(float x, float y, float z)
{
	return Transform(
		Matrix4x4::scale(x,y,z),
		Matrix4x4::scale(1/x, 1/y, 1/z));
}

Transform Transform::scale(float s)
{
	return scale(s,s,s);
}

Transform Transform::scale_x(float x)
{
	return scale(x,1,1);
}

Transform Transform::scale_y(float y)
{
	return scale(1,y,1);
}

Transform Transform::scale_z(float z)
{
	return scale(1,1,z);
}

Transform Transform::rotate(float degrees, Vector3 axis)
{
	return Transform(
		Matrix4x4::rotate(degrees, axis),
		Matrix4x4::rotate(-degrees, axis));
}

Transform Transform::rotate_x(float degrees)
{
	return rotate(degrees, Vector3(1,0,0));
}

Transform Transform::rotate_y(float degrees)
{
	return rotate(degrees, Vector3(1,0,0));
}

Transform Transform::rotate_z(float degrees)
{
	return rotate(degrees, Vector3(1,0,0));
}

}