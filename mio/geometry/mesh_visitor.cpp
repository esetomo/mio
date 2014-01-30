#include <mio/geometry/mesh_visitor.h>
#include <mio/exceptions.h>

namespace mio
{
	namespace geometry
	{
		MeshVisitor::MeshVisitor()
		{
		}

		MeshVisitor::~MeshVisitor()
		{
		}		

		void MeshVisitor::visit_IndexedMesh(mio::geometry::IndexedMesh *mesh)
		{
			throw NotImplementedException();
		}
	}
}