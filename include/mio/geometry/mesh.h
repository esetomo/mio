#ifndef __MIO_GEOMETRY_MESH_H__
#define __MIO_GEOMETRY_MESH_H__

#include <mio/mio.h>
#include <mio/exceptions.h>
#include <mio/box.h>
#include <mio/mio_object.h>

namespace mio
{
	namespace geometry
	{
		class MeshVisitor;

		class MIO_API Mesh : public MioObject
		{
		public:
			Mesh(const char *name="");
			virtual ~Mesh();

			virtual Box get_bounding_box() const = 0;
			
			virtual Point3 _get_vertex(int index) const = 0;
			
			virtual Point3 get_vertex(int index) const
			{
				if (index < 0 || index >= get_vertex_count())
					throw mio::IndexException("Mesh::get_vertex() : vertex index out of range");
				else
					return _get_vertex(index);
			}

			virtual Vector3 _get_normal(int index) const = 0;

			virtual Vector3 get_normal(int index) const
			{
				if (index < 0 || index >= get_normal_count())
					throw mio::IndexException("Mesh::get_normal() : normal index out of range");
				else
					return _get_normal(index);
			}

			virtual int get_polygon_count() = 0;

			virtual int _get_polygon_vertex_count(int polygon_index) = 0;

			virtual int get_polygon_vertex_count(int polygon_index)
			{
				if (polygon_index < 0 || polygon_index >= get_polygon_count())
					throw mio::IndexException("Mesh::get_polygon_vertex_count() : polygon index out of range");
				else
					return _get_polygon_vertex_count(polygon_index);
			}

			virtual Point3 _get_polygon_vertex(int polygon_index, int vertex_offset) = 0;

			virtual Point3 get_polygon_vertex(int polygon_index, int vertex_offset)
			{
				if (polygon_index < 0 || polygon_index >= get_polygon_count())
					throw mio::IndexException("Mesh::get_polygon_vertex() : polygon index out of range");
				if (vertex_offset < 0 || vertex_offset >= get_polygon_vertex_count(polygon_index))
					throw mio::IndexException("Mesh::get_polygon_vertex() : vertex offset out of range");
				return _get_polygon_vertex(polygon_index, vertex_offset);
			}

			virtual Vector3 _get_polygon_normal(int polygon_index, int vertex_offset) = 0;

			virtual Vector3 get_polygon_normal(int polygon_index, int vertex_offset)
			{
				if (polygon_index < 0 || polygon_index >= get_polygon_count())
					throw mio::IndexException("Mesh::get_polygon_normal() : polygon index out of range");
				if (vertex_offset < 0 || vertex_offset >= get_polygon_vertex_count(polygon_index))
					throw mio::IndexException("Mesh::get_polygon_normal() : vertex offset out of range");
				return _get_polygon_normal(polygon_index, vertex_offset);
			}

			virtual int get_vertex_count() const = 0;
			virtual int get_normal_count() const = 0;

			virtual void accept(mio::geometry::MeshVisitor *visitor);
		};
	}
}

#endif