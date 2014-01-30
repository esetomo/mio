#ifndef __SOA_DATA_STRUCTURES_H__
#define __SOA_DATA_STRUCTURES_H__

#include <mio/mio.h>
#include <mio/vector3.h>
#include <mio/array.h>
#include <mio/uv.h>
#include <mio/rgb.h>

namespace mio
{
	struct MIO_API SoaFloat2
	{
	public:
		Array<float> x, y;

		inline Uv get_Uv(int i) const
		{
			return Uv(x[i], y[i]);
		}

		inline void set(int i, float _x, float _y)
		{
			x[i] = _x;
			y[i] = _y;
		}

		inline void set(int i, float c)
		{
			x[i] = c;
			y[i] = c;
		}

		inline void set(int i, const Uv &uv)
		{
			x[i] = uv.u;
			y[i] = uv.v;
		}

		inline void push_back(float _x, float _y)
		{
			x.push_back(_x);
			y.push_back(_y);
		}

		inline void push_back(const Uv &uv)
		{
			x.push_back(uv.u);
			y.push_back(uv.v);
		}

		inline void push_back(float c)
		{
			x.push_back(c);
			y.push_back(c);
		}

		inline void trim(int newSize)
		{
			x.trim(newSize);
			y.trim(newSize);
		}

		inline void adjust_size(int newSize, bool copyOldData)
		{
			x.adjust_size(newSize, copyOldData);
			y.adjust_size(newSize, copyOldData);
		}

		inline void adjust_capacity(int newCapacity, bool copyOldData)
		{
			x.adjust_capacity(newCapacity, copyOldData);
			y.adjust_capacity(newCapacity, copyOldData);
		}
	};

	struct MIO_API SoaFloat3
	{
	public:
		Array<float> x, y, z;	

		inline Point3 get_Point(int i) const
		{
			return Point3(x[i], y[i], z[i]);
		}

		inline Vector3 get_Vector(int i) const
		{
			return Vector3(x[i], y[i], z[i]);
		}

		inline Rgb get_Rgb(int i) const 
		{
			return Rgb(x[i], y[i], z[i]);
		}

		inline void set(int i, float _x, float _y, float _z)
		{
			x[i] = _x;
			y[i] = _y;
			z[i] = _z;
		}

		inline void set(int i, float c)
		{
			set(i,c,c,c);
		}

		inline void set(int i, const Point3 &p)
		{
			set(i, p.x, p.y, p.z);
		}

		inline void set(int i, const Vector3 &v)
		{
			set(i, v.x, v.y, v.z);
		}

		inline void set(int i, const Rgb &color)
		{
			set(i, color.r, color.g, color.b);
		}

		inline void push_back(float _x, float _y, float _z)
		{
			x.push_back(_x);
			y.push_back(_y);
			z.push_back(_z);
		}

		inline void push_back(float c)
		{
			push_back(c,c,c);
		}

		inline void push_back(const Point3 &p)
		{
			push_back(p.x, p.y, p.z);
		}

		inline void push_back(const Vector3 &v)
		{
			push_back(v.x, v.y, v.z);
		}

		inline void push_back(const Rgb &color)
		{
			push_back(color.r, color.g, color.b);
		}

		inline void trim(int newSize)
		{
			x.trim(newSize);
			y.trim(newSize);
			z.trim(newSize);
		}

		void adjust_size(int newSize, bool copyOldData)
		{
			x.adjust_size(newSize, copyOldData);
			y.adjust_size(newSize, copyOldData);
			z.adjust_size(newSize, copyOldData);
		}

		void adjust_capacity(int newCapacity, bool copyOldData)
		{
			x.adjust_capacity(newCapacity, copyOldData);
			y.adjust_capacity(newCapacity, copyOldData);
			z.adjust_capacity(newCapacity, copyOldData);
		}	

		int size() const
		{
			return x.size();
		}
	};

	struct MIO_API SoaInt3
	{
	public:
		Array<int> v[3];

		/*
		inline void push_back(const VertexIndexOnlyTriangle &t)
		{
			v[0].push_back(t.v[0]);
			v[1].push_back(t.v[1]);
			v[2].push_back(t.v[2]);
		}

		inline void push_back(const SceneVertexIndexOnlyTriangle &t)
		{
			v[0].push_back(t.v[0]);
			v[1].push_back(t.v[1]);
			v[2].push_back(t.v[2]);
		}
		
		inline VertexIndexOnlyTriangle GetVertexIndexOnlyTriangle(int i) const
		{
			return VertexIndexOnlyTriangle(i, v[0][i], v[1][i], v[2][i]);
		}

		inline SceneVertexIndexOnlyTriangle GetSceneVertexIndexOnlyTriangle(int i) const
		{
			SceneVertexIndexOnlyTriangle result;
			result.id = i;
			result.v[0] = v[0][i];
			result.v[1] = v[1][i];
			result.v[2] = v[2][i];
			return result;
		}
		*/

		inline void set(int i, int v0, int v1, int v2)
		{
			v[0][i] = v0;
			v[1][i] = v1;
			v[2][i] = v2;
		}

		/*
		void set(int i, const VertexIndexOnlyTriangle &t)
		{
			set(i, t.v[0], t.v[1], t.v[2]);
		}
		
		void set(int i, const SceneVertexIndexOnlyTriangle &t)
		{
			set(i, t.v[0], t.v[1], t.v[2]);
		}
		*/

		inline void adjust_size(int newSize, bool copyOldData)
		{
			v[0].adjust_size(newSize, copyOldData);
			v[1].adjust_size(newSize, copyOldData);
			v[2].adjust_size(newSize, copyOldData);
		}

		inline void adjust_capacity(int newCapacity, bool copyOldData)
		{
			v[0].adjust_capacity(newCapacity, copyOldData);
			v[1].adjust_capacity(newCapacity, copyOldData);
			v[2].adjust_capacity(newCapacity, copyOldData);
		}

		inline void trim(int newSize)
		{
			v[0].trim(newSize);
			v[1].trim(newSize);
			v[2].trim(newSize);
		}

		int size() const
		{
			return v[0].size();
		}
	};

	struct MIO_API SoaBox
	{
	public:
		SoaFloat3 min_point, max_point;

		inline Box get_Box(int i) const 
		{
			return Box(min_point.get_Point(i), max_point.get_Point(i));
		}

		void set(int i, const Box &bbox)
		{
			min_point.set(i, bbox.min_point);
			max_point.set(i, bbox.max_point);
		}

		void adjust_size(int newSize, bool copyOldData)
		{
			min_point.adjust_size(newSize, copyOldData);
			max_point.adjust_size(newSize, copyOldData);
		}

		void adjust_capacity(int newCapacity, bool copyOldData)
		{
			min_point.adjust_capacity(newCapacity, copyOldData);
			max_point.adjust_capacity(newCapacity, copyOldData);
		}

		void push_back(const Box &bbox)
		{
			min_point.push_back(bbox.min_point);
			max_point.push_back(bbox.max_point);
		}

		void trim(int newSize)
		{
			min_point.trim(newSize);
			max_point.trim(newSize);
		}

		int size() const
		{
			return min_point.size();
		}
	};
}

#endif
