#include <mio/quaternion.h>

namespace mio
{
	static int next[3] = {1, 2, 0};

	Quaternion::Quaternion(const Matrix4x4 &m)
	{		
		float trace = m[0][0] + m[1][1] + m[2][2];
		
		if (trace > 0.0f)
		{
			float s = sqrtf(trace + 1.0f);
			w = s / 2.0f;
			s = 0.5f / s;
			x = (-m[1][2] + m[2][1]) * s;
			y = (-m[2][0] + m[0][2]) * s;
            z = (-m[0][1] + m[1][0]) * s;
		}
		else
		{
			int i = 0;
			if (m[1][1] > m[0][0])
                i = 1;
            if (m[2][2] > m[i][i])
                i = 2;
			int j = next[i];
			int k = next[j];

			float s = (float)sqrtf(1.0f + (m[i][i] - m[j][j] - m[k][k]));
			float q[4] = {0.0f, 0.0f, 0.0f, 0.0f};
			q[i] = s * 0.5f;
			if (fabs(s) > EPSILON)
				s = 0.5f / s;
			q[3] = (-m[j][k] + m[k][j]) * s;
			q[j] = ( m[i][j] + m[j][i]) * s;
			q[k] = ( m[i][k] + m[k][i]) * s;

			x = q[0];
			y = q[1];
			z = q[2];
			w = q[3];
		}
	}
	
	Matrix4x4 Quaternion::get_matrix() const
	{
		float x2 = x+x;
		float y2 = y+y;
		float z2 = z+z;
		
		float xx = x*x2;
		float xy = x*y2;
		float xz = x*z2;

		float yy = y*y2;
		float yz = y*z2;
		float zz = z*z2;

		float wx = w*x2;
		float wy = w*y2;
		float wz = w*z2;
    
		return Matrix4x4(
			1.0f - (yy + zz),	      xy - wz ,	        xz + wy , 0.0f,
					xy + wz , 1.0f - (xx + zz),	        yz - wx , 0.0f,
					xz - wy ,         yz + wx , 1.0f - (xx + yy), 0.0f,
						0.0f,			 0.0f,			    0.0f, 1.0f);        
	}
}