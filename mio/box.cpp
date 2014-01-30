#include <mio/box.h>

namespace mio
{

	Box union_(const Box &b, const Point3 &p) 
	{
		Box ret = b;
		ret.min_point.x = min(b.min_point.x, p.x);
		ret.min_point.y = min(b.min_point.y, p.y);
		ret.min_point.z = min(b.min_point.z, p.z);
		ret.max_point.x = max(b.max_point.x, p.x);
		ret.max_point.y = max(b.max_point.y, p.y);
		ret.max_point.z = max(b.max_point.z, p.z);
		return ret;
	}

	Box union_(const Box &b, const Box &b2) 
	{
		Box ret;
		ret.min_point.x = min(b.min_point.x, b2.min_point.x);
		ret.min_point.y = min(b.min_point.y, b2.min_point.y);
		ret.min_point.z = min(b.min_point.z, b2.min_point.z);
		ret.max_point.x = max(b.max_point.x, b2.max_point.x);
		ret.max_point.y = max(b.max_point.y, b2.max_point.y);
		ret.max_point.z = max(b.max_point.z, b2.max_point.z);
		return ret;
	}

	void Box::bounding_sphere(Point3 *c, float *rad) const 
	{
		*c = .5f * min_point + .5f * max_point;
		*rad = inside(*c) ? distance(*c, max_point) : 0.f;
	}

	bool Box::intersect_predicate(const Ray &ray, float *hitt0, float *hitt1) const 
	{
		float t0 = RAY_EPSILON;
		float t1 = INFINITY;
			//ray.mint, t1 = ray.maxt;
		for (int i = 0; i < 3; ++i) {
			// Update interval for _i_th bounding box slab
			float invRayDir = 1.f / ray.direction[i];
			float tNear = (min_point[i] - ray.origin[i]) * invRayDir;
			float tFar  = (max_point[i] - ray.origin[i]) * invRayDir;
			// Update parametric interval from slab intersection $t$s
			if (tNear > tFar) std::swap(tNear, tFar);
			t0 = tNear > t0 ? tNear : t0;
			t1 = tFar  < t1 ? tFar  : t1;
			if (t0 > t1) 
			{
				if (hitt0) *hitt0 = t0;
				if (hitt1) *hitt1 = t1;
				return false;
			}
		}
		if (hitt0) *hitt0 = t0;
		if (hitt1) *hitt1 = t1;
		return true;
	}

}