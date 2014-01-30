/*
 * pbrt source code Copyright(c) 1998-2005 Matt Pharr and Greg Humphreys
 *
 * All Rights Reserved.
 * For educational use only; commercial use expressly forbidden.
 * NO WARRANTY, express or implied, for this software.
 * (See file License.txt for complete license)
 */

#include <mio/vector3.h>
#include <mio/point3.h>

namespace mio
{
	// Geometry Inline Functions
	Vector3::Vector3(const Point3 &p) : x(p.x), y(p.y), z(p.z) 
	{
	}
}