#ifndef __MIO_BOX_H__
#define __MIO_BOX_H__

#include <mio/mio.h>
#include <mio/vector3.h>
#include <mio/point3.h>
#include <mio/ray.h>
#include <iostream>

namespace mio
{
	struct MIO_API Box 
	{
	public:
		// BBox Public Methods
		Box() 
		{
			min_point = Point3( INFINITY,  INFINITY,  INFINITY);
			max_point = Point3(-INFINITY, -INFINITY, -INFINITY);
		}
		
		Box(const Point3 &p) : min_point(p), max_point(p) 
		{ 
		}
		
		Box(const Point3 &p1, const Point3 &p2) 
		{
			min_point = Point3(min(p1.x, p2.x),
						 min(p1.y, p2.y),
						 min(p1.z, p2.z));
			max_point = Point3(max(p1.x, p2.x),
						 max(p1.y, p2.y),
						 max(p1.z, p2.z));
		}

		/*
		friend inline ostream &
			operator<<(ostream &os, const BBox &b);
		*/

		bool overlaps(const Box &b) const 
		{
			bool x = (max_point.x >= b.min_point.x) && (min_point.x <= b.max_point.x);
			bool y = (max_point.y >= b.min_point.y) && (min_point.y <= b.max_point.y);
			bool z = (max_point.z >= b.min_point.z) && (min_point.z <= b.max_point.z);
			return (x && y && z);
		}
		
		bool inside(const Point3 &pt) const 
		{
			return (pt.x >= min_point.x && pt.x <= max_point.x &&
					pt.y >= min_point.y && pt.y <= max_point.y &&
					pt.z >= min_point.z && pt.z <= max_point.z);
		}
		
		void expand(float delta) 
		{
			min_point -= Vector3(delta, delta, delta);
			max_point += Vector3(delta, delta, delta);
		}
		
		float volume() const 
		{
			Vector3 d = max_point - min_point;
			return d.x * d.y * d.z;
		}
		
		float surface_area() const 
		{
			Vector3 d = max_point - min_point;
			return 2 * ((d.x*d.y) + (d.y*d.z) + (d.z*d.x));
		}
		
		float diameter() const
		{
			return (centroid() - min_point).length();
		}
		
		Point3 centroid() const
		{
			return (max_point + min_point) / 2.0f;
		}
		
		int maximum_extent() const 
		{
			Vector3 diag = max_point - min_point;
			if (diag.x > diag.y && diag.x > diag.z)
				return 0;
			else if (diag.y > diag.z)
				return 1;
			else
				return 2;
		}
		
		void bounding_sphere(Point3 *c, float *rad) const;
		
		bool intersect_predicate(const Ray &ray,
						float *hitt0 = NULL,
						float *hitt1 = NULL) const;
		
		// BBox Public Data
		Point3 min_point, max_point;
	};

	MIO_API Box union_(const Box &b, const Point3 &p);
	MIO_API Box union_(const Box &b, const Box &b2);

	inline std::ostream &operator<<(std::ostream &os, const Box &b) 
	{
		os << b.min_point << " -> " << b.max_point;
		return os;
	}
}

#endif