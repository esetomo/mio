#include <mio/geometry/mesh.h>

namespace mio
{
	namespace geometry
	{
		Mesh::Mesh(const char *name) : MioObject(name)
		{
		}

		Mesh::~Mesh()
		{
		}

		void Mesh::accept(mio::geometry::MeshVisitor *visitor)
		{
			throw "Not implmeneted yet!";
		}
	}
}