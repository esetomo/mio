#include <mio/geometry/indexed_mesh.h>
#include <mio/geometry/mesh_visitor.h>

namespace mio
{
	namespace geometry
	{
		IndexedMesh::IndexedMesh(Polygons *polygons) : Mesh("")
		{
			bbox_ = Box();
			bboxValid_ = false;
			polygons_ = polygons;
		}

		IndexedMesh::IndexedMesh(const char *name) : Mesh(name)
		{
			bbox_ = Box();
			bboxValid_ = false;
			polygons_ = new Polygons;
		}

		IndexedMesh::~IndexedMesh()
		{
			polygons_ = NULL;
		}

		void IndexedMesh::accept(mio::geometry::MeshVisitor *visitor)
		{
			visitor->visit_IndexedMesh(this);
		}

		/**
		 * Return a shallow copy.
		 */
		IndexedMesh *IndexedMesh::__copy__()
		{
			IndexedMesh *result = new IndexedMesh(polygons_.get_pointer());

			FOR(i, vertices_.size())
				result->vertices_.push_back(vertices_[i]);
			FOR(i, normals_.size())
				result->normals_.push_back(normals_[i]);

			return result;
		}
	}
}