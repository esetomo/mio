#ifndef __MATRIX_4X4_H__
#define __MATRIX_4X4_H__

#include <mio/mio.h>
#include <mio/vector3.h>
#include <mio/point3.h>

namespace mio
{

	struct MIO_API Matrix4x4
	{
	private:
		float m[4][4];

	public:
		inline Matrix4x4()
		{
			for(int i=0;i<4;i++)
				for(int j=0;j<4;j++)
					m[i][j] = 0.0f;
		}

		inline Matrix4x4(float c)
		{
			for(int i=0;i<4;i++)
				for(int j=0;j<4;j++)
					m[i][j] = c;
		}

		inline Matrix4x4(float mat[4][4])
		{
			for(int i=0;i<4;i++)
				for(int j=0;j<4;j++)
					m[i][j] = mat[i][j];
		}

		inline Matrix4x4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
		{
			m[0][0] = m00;
			m[0][1] = m01;
			m[0][2] = m02;
			m[0][3] = m03;

			m[1][0] = m10;
			m[1][1] = m11;
			m[1][2] = m12;
			m[1][3] = m13;

			m[2][0] = m20;
			m[2][1] = m21;
			m[2][2] = m22;
			m[2][3] = m23;

			m[3][0] = m30;
			m[3][1] = m31;
			m[3][2] = m32;
			m[3][3] = m33;
		}	

		inline float *operator[](int i) const
		{
			return (float *)m[i];
		}

		inline float _get(int i, int j) const
		{
			return m[i][j];
		}

		inline void _set(int i, int j, float value)
		{
			m[i][j] = value;
		}

		inline Point3 operator*(const Point3 &p) const
		{
			float x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3];
			float y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3];
			float z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3];
			float w = m[3][0] * p.x + m[3][1] * p.y + m[3][2] * p.z + m[3][3];

			if (w == 1.0f) 
				return Point3(x,y,z);
			else
			{
				float wi = 1.0f / w;
				return Point3(x * wi, y * wi, z * wi);
			}
		}

		inline Vector3 operator*(const Vector3 &v) const
		{
			float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
			float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
			float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
			return Vector3(x,y,z);
		}	

		inline Matrix4x4 operator*(const Matrix4x4 &other) const
		{
			Matrix4x4 result;
			for(int i=0;i<4;i++)
				for(int j=0;j<4;j++)
					for(int k=0;k<4;k++)
						result.m[i][j] += m[i][k] * other.m[k][j];
			return result;
		}

		inline void get_column_major_array(float *result)
		{
			FOR(col, 4)
				FOR(row, 4)
					result[4*col + row] = m[row][col];
		}

		Matrix4x4 __copy__()
		{
			Matrix4x4 result = *this;
			return result;
		}

		Matrix4x4 __deepcopy__()
		{
			Matrix4x4 result = *this;
			return result;
		}

		inline bool is_near(const Matrix4x4 &other, float threshold=EPSILON) const
		{
			FOR(i, 4)
				FOR(j, 4)
					if (fabs(m[i][j] - other.m[i][j]) >= EPSILON)
						return false;
			return true;
		}

		static Matrix4x4 identity();
		
		static Matrix4x4 translate(float x, float y, float z);
		static Matrix4x4 translate_x(float x);
		static Matrix4x4 translate_y(float y);
		static Matrix4x4 translate_z(float z);
		
		static Matrix4x4 scale(float x, float y, float z);
		static Matrix4x4 scale(float s);
		static Matrix4x4 scale_x(float x);
		static Matrix4x4 scale_y(float y);
		static Matrix4x4 scale_z(float z);
		
		static Matrix4x4 rotate(float degrees, Vector3 axis);
		static Matrix4x4 rotate_x(float degrees);
		static Matrix4x4 rotate_y(float degrees);
		static Matrix4x4 rotate_z(float degrees);
		static Matrix4x4 look_at(Point3 eye, Point3 at, Vector3 up);
	};

	inline Matrix4x4 transpose(const Matrix4x4 &A)
	{
		float *r0 = A[0];
		float *r1 = A[1];
		float *r2 = A[2];
		float *r3 = A[3];
			
		return Matrix4x4(
			r0[0], r1[0], r2[0], r3[0],
			r0[1], r1[1], r2[1], r3[1],
			r0[2], r1[2], r2[2], r3[2],
			r0[3], r1[3], r2[3], r3[3]);
	}

	MIO_API Matrix4x4 inverse(const Matrix4x4 &m);
	MIO_API float det(const Matrix4x4 &m);
}

#endif