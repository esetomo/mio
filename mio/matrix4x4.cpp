#include <mio/definitions.h>
#include <mio/exceptions.h>
#include <mio/matrix4x4.h>

namespace mio
{

Matrix4x4 Matrix4x4::identity()
{
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::translate(float x, float y, float z)
{
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, x,
		0.0f, 1.0f, 0.0f, y,
		0.0f, 0.0f, 1.0f, z,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::translate_x(float x)
{
	return Matrix4x4::translate(x, 0.0f, 0.0f);
}

Matrix4x4 Matrix4x4::translate_y(float y)
{
	return Matrix4x4::translate(0.0f, y, 0.0f);
}

Matrix4x4 Matrix4x4::translate_z(float z)
{
	return Matrix4x4::translate(0.0f, 0.0f, z);
}

Matrix4x4 Matrix4x4::scale(float sx, float sy, float sz)
{
	return Matrix4x4(
		sx,   0.0f, 0.0f, 0.0f,
		0.0f, sy,   0.0f, 0.0f,
		0.0f, 0.0f, sz,   0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::scale(float s)
{
	return Matrix4x4::scale(s,s,s);
}

Matrix4x4 Matrix4x4::scale_x(float x)
{
	return Matrix4x4::scale(x, 1.0f, 1.0f);
}

Matrix4x4 Matrix4x4::scale_y(float y)
{
	return Matrix4x4::scale(1.0f, y, 1.0f);
}

Matrix4x4 Matrix4x4::scale_z(float z)
{
	return Matrix4x4::scale(1.0f, 1.0f, z);
}

Matrix4x4 Matrix4x4::rotate(float degrees, Vector3 axis)
{	
	float il = 1.0f / axis.length();
	float x = axis.x * il;
	float y = axis.y * il;
	float z = axis.z * il;
	
	float theta = radians(degrees);
	float sinTheta = sinf(theta);
	float cosTheta = cosf(theta);

	Matrix4x4 result;	

	float oneMinusC = 1 - cosTheta;
	float xx = x * x;
	float xy = x * y;
	float xz = x * z;
	float yy = y * y;
	float yz = y * z;
	float zz = z * z;
	float xS = x * sinTheta;
	float yS = y * sinTheta;
	float zS = z * sinTheta;
	
	result.m[0][0] = xx * oneMinusC + cosTheta;
	result.m[0][1] = xy * oneMinusC - zS;
	result.m[0][2] = xz * oneMinusC + yS;
	result.m[0][3] = 0;

	result.m[1][0] = xy * oneMinusC + zS;
	result.m[1][1] = yy * oneMinusC + cosTheta;
	result.m[1][2] = yz * oneMinusC - xS;
	result.m[1][3] = 0;

	result.m[2][0] = xz * oneMinusC - yS;
	result.m[2][1] = yz * oneMinusC + xS;
	result.m[2][2] = zz * oneMinusC + cosTheta;
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	/*
	result.m[0][0] = x * x + (1.0f - x * x) * c;
	result.m[0][1] = x * y * (1.0f - c) - z * s;
	result.m[0][2] = x * z * (1.0f - c) + y * s;
	

	result.m[1][0] = x * y * (1.0f - c) + z * s;
	result.m[1][1] = y * y + (1.0f - y * y) * c;
	result.m[1][2] = y * z * (1.0f - c) - x * s;
	

	result.m[2][0] = x * z * (1.0f - c) - y * s;
	result.m[2][1] = y * z * (1.0f - c) + x * s;
	result.m[2][2] = z * z + (1.0f - z * z) * c;
	*/


	return result;
}

Matrix4x4 Matrix4x4::rotate_x(float degrees)
{
	return Matrix4x4::rotate(degrees, Vector3(1.0f, 0.0f, 0.0f));
}

Matrix4x4 Matrix4x4::rotate_y(float degrees)
{
	return Matrix4x4::rotate(degrees, Vector3(0.0f, 1.0f, 0.0f));
}

Matrix4x4 Matrix4x4::rotate_z(float degrees)
{
	return Matrix4x4::rotate(degrees, Vector3(0.0f, 0.0f, 1.0f));
}

Matrix4x4 Matrix4x4::look_at(Point3 eye, Point3 at, Vector3 up)
{
	Vector3 z = normalize(eye - at);
	up = normalize(up);
	Vector3 x = cross(up, z);
	Vector3 y = cross(z, x);

	return Matrix4x4(
		x[0], y[0], z[0], eye[0],
		x[1], y[1], z[1], eye[1],
		x[2], y[2], z[2], eye[2],
		0.0f, 0.0f, 0.0f, 1.0f);
}

// This section of the code is taken from:
// http://www.geometrictools.com/LibFoundation/Mathematics/Wm4Matrix4.inl.
// It seems that it is licensed in LGPL.

Matrix4x4 inverse(const Matrix4x4 &m)
{
	float fA0 = m[0][0]*m[1][1] - m[0][1]*m[1][0];
	float fA1 = m[0][0]*m[1][2] - m[0][2]*m[1][0];
	float fA2 = m[0][0]*m[1][3] - m[0][3]*m[1][0];
	float fA3 = m[0][1]*m[1][2] - m[0][2]*m[1][1];
	float fA4 = m[0][1]*m[1][3] - m[0][3]*m[1][1];
	float fA5 = m[0][2]*m[1][3] - m[0][3]*m[1][2];
	float fB0 = m[2][0]*m[3][1] - m[2][1]*m[3][0];
	float fB1 = m[2][0]*m[3][2] - m[2][2]*m[3][0];
	float fB2 = m[2][0]*m[3][3] - m[2][3]*m[3][0];
	float fB3 = m[2][1]*m[3][2] - m[2][2]*m[3][1];
	float fB4 = m[2][1]*m[3][3] - m[2][3]*m[3][1];
	float fB5 = m[2][2]*m[3][3] - m[2][3]*m[3][2];

	float fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
	if (fabs(fDet) < EPSILON)	
		throw std::exception("matrix is singular");	

	Matrix4x4 kInv;
	kInv[0][0] =
		+ m[1][1]*fB5 - m[1][2]*fB4 + m[1][3]*fB3;
	kInv[1][0] =
		- m[1][0]*fB5 + m[1][2]*fB2 - m[1][3]*fB1;
	kInv[2][0] =
		+ m[1][0]*fB4 - m[1][1]*fB2 + m[1][3]*fB0;
	kInv[3][0] =
		- m[1][0]*fB3 + m[1][1]*fB1 - m[1][2]*fB0;
	kInv[0][1] =
		- m[0][1]*fB5 + m[0][2]*fB4 - m[0][3]*fB3;
	kInv[1][1] =
		+ m[0][0]*fB5 - m[0][2]*fB2 + m[0][3]*fB1;
	kInv[2][1] =
		- m[0][0]*fB4 + m[0][1]*fB2 - m[0][3]*fB0;
	kInv[3][1] =
		+ m[0][0]*fB3 - m[0][1]*fB1 + m[0][2]*fB0;
	kInv[0][2] =
		+ m[3][1]*fA5 - m[3][2]*fA4 + m[3][3]*fA3;
	kInv[1][2] =
		- m[3][0]*fA5 + m[3][2]*fA2 - m[3][3]*fA1;
	kInv[2][2] =
		+ m[3][0]*fA4 - m[3][1]*fA2 + m[3][3]*fA0;
	kInv[3][2] =
		- m[3][0]*fA3 + m[3][1]*fA1 - m[3][2]*fA0;
	kInv[0][3] =
		- m[2][1]*fA5 + m[2][2]*fA4 - m[2][3]*fA3;
	kInv[1][3] =
		+ m[2][0]*fA5 - m[2][2]*fA2 + m[2][3]*fA1;
	kInv[2][3] =
		- m[2][0]*fA4 + m[2][1]*fA2 - m[2][3]*fA0;
	kInv[3][3] =
		+ m[2][0]*fA3 - m[2][1]*fA1 + m[2][2]*fA0;

	float fInvDet = ((float)1.0)/fDet;
	kInv[0][0] *= fInvDet;
	kInv[0][1] *= fInvDet;
	kInv[0][2] *= fInvDet;
	kInv[0][3] *= fInvDet;
	kInv[1][0] *= fInvDet;
	kInv[1][1] *= fInvDet;
	kInv[1][2] *= fInvDet;
	kInv[1][3] *= fInvDet;
	kInv[2][0] *= fInvDet;
	kInv[2][1] *= fInvDet;
	kInv[2][2] *= fInvDet;
	kInv[2][3] *= fInvDet;
	kInv[3][0] *= fInvDet;
	kInv[3][1] *= fInvDet;
	kInv[3][2] *= fInvDet;
	kInv[3][3] *= fInvDet;

	return kInv;
}

float det(const Matrix4x4 &m)
{
    float fA0 = m[0][0]*m[1][1] - m[0][1]*m[1][0];
    float fA1 = m[0][0]*m[1][2] - m[0][2]*m[1][0];
    float fA2 = m[0][0]*m[1][3] - m[0][3]*m[1][0];
    float fA3 = m[0][1]*m[1][2] - m[0][2]*m[1][1];
    float fA4 = m[0][1]*m[1][3] - m[0][3]*m[1][1];
    float fA5 = m[0][2]*m[1][3] - m[0][3]*m[1][2];
    float fB0 = m[2][0]*m[3][1] - m[2][1]*m[3][0];
    float fB1 = m[2][0]*m[3][2] - m[2][2]*m[3][0];
    float fB2 = m[2][0]*m[3][3] - m[2][3]*m[3][0];
    float fB3 = m[2][1]*m[3][2] - m[2][2]*m[3][1];
    float fB4 = m[2][1]*m[3][3] - m[2][3]*m[3][1];
    float fB5 = m[2][2]*m[3][3] - m[2][3]*m[3][2];
    float fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
    return fDet;
}


}