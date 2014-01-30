#ifndef __MIO_GEOMETRY_MESH_VISITOR_H__
#define __MIO_GEOMETRY_MESH_VISITOR_H__

#include <mio/mio.h>
#include <mio/geometry/mesh.h>
#include <mio/mio_object.h>
#include <mio/geometry/indexed_mesh.h>

namespace mio
{
	namespace geometry
	{
		class MIO_API MeshVisitor
		{
		public:
			MeshVisitor();
			virtual ~MeshVisitor() = 0;

			virtual void visit_IndexedMesh(mio::geometry::IndexedMesh *mesh);
		};
	}
}

#endif