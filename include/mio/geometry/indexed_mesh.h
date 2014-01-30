#ifndef __MIO_GEOMETRY_INDEXED_MESH_H__
#define __MIO_GEOMETRY_INDEXED_MESH_H__

#include <mio/mio.h>
#include <mio/array.h>
#include <mio/vector3.h>
#include <mio/reference.h>
#include <mio/geometry/mesh.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
	namespace geometry
	{
		class MIO_API IndexedMesh : public Mesh
		{
		private:
			mutable Box bbox_;
			mutable bool bboxValid_;

			Array<Point3> vertices_;
			Array<Vector3> normals_;

			class Polygons : public ReferenceCounted
			{
			public:
				Polygons() : ReferenceCounted()
				{
				}

				virtual ~Polygons()
				{
				}

				Array<int> vertex_count;
				Array<int> index_start;
				Array<int> vertex_indices;
				Array<int> normal_indices;
			};
			Reference<Polygons> polygons_;

			IndexedMesh(Polygons *polygons);

		public:
			IndexedMesh(const char *name="");
			virtual ~IndexedMesh();

			virtual void accept(mio::geometry::MeshVisitor *visitor);

			inline Box get_bounding_box() const
			{
				if (!bboxValid_)
					validate_bounding_box();
				return bbox_;
			}

			inline void validate_bounding_box() const
			{
				bbox_ = Box();
				for(int i=0;i<vertices_.size();i++)
				{
					bbox_ = union_(bbox_, vertices_[i]);
				}
				bboxValid_ = true;
			}

			inline void invalidate_bounding_box() const
			{
				bboxValid_ = false;
			}

			inline virtual Point3 _get_vertex(int index) const
			{
				return vertices_[index];
			}

			inline virtual Vector3 _get_normal(int index) const
			{
				return normals_[index];
			}

			inline virtual int get_vertex_count() const
			{
				return vertices_.size();
			}

			inline virtual int get_normal_count() const
			{
				return normals_.size();
			}

			inline void append_vertex(const mio::Point3 &vertex)
			{
				vertices_.push_back(vertex);
			}

			inline void append_normal(const mio::Vector3 &normal)
			{
				normals_.push_back(normal);
			}

			inline void _set_normal(int normalIndex, const Vector3 &normal)
			{
				normals_[normalIndex] = normal;
			}

			inline void set_normal(int normalIndex, const Vector3 &normal)
			{
				if (normalIndex < 0 || normalIndex >= get_normal_count())
					throw mio::IndexException("normal index out of range");
				else
					_set_normal(normalIndex, normal);
			}

			inline void _set_vertex(int vertex_index, const Point3 &position)
			{
				vertices_[vertex_index] = position;
			}

			inline void set_vertex(int vertex_index, const Point3 &position)
			{
				if (vertex_index < 0 || vertex_index >= get_vertex_count())
					throw mio::IndexException("IndexedMesh::set_vertex() : vertex index out of range");
				else
					_set_vertex(vertex_index, position);
			}

			inline void append_new_polygon()
			{
				polygons_->vertex_count.push_back(0);
				polygons_->index_start.push_back(polygons_->vertex_indices.size());
			}

			inline void append_vertex_normal_index_to_last_polygon(int vertex_index, int normal_index)
			{
				polygons_->vertex_indices.push_back(vertex_index);
				polygons_->normal_indices.push_back(normal_index);
				int last_polygon_index = polygons_->vertex_count.size() - 1;
				polygons_->vertex_count[last_polygon_index]++;
			}

			virtual int get_polygon_count()
			{
				return polygons_->vertex_count.size();
			}

			virtual int _get_polygon_vertex_count(int polygon_index)
			{
				return polygons_->vertex_count[polygon_index];
			}

			virtual Point3 _get_polygon_vertex(int polygon_index, int vertex_offset)
			{
				int start = polygons_->index_start[polygon_index];
				int vertex_index = polygons_->vertex_indices[start + vertex_offset];
				return vertices_[vertex_index];
			}

			virtual Vector3 _get_polygon_normal(int polygon_index, int vertex_offset)
			{
				int start = polygons_->index_start[polygon_index];
				int normal_index = polygons_->normal_indices[start + vertex_offset];
				return normals_[normal_index];
			}

			virtual int _get_polygon_vertex_index(int polygon_index, int vertex_offset)
			{
				int start = polygons_->index_start[polygon_index];
				return polygons_->vertex_indices[start + vertex_offset];				
			}

			virtual int get_polygon_vertex_index(int polygon_index, int vertex_offset)
			{
				if (polygon_index < 0 || polygon_index >= get_polygon_count())
					throw mio::IndexException("Mesh::get_polygon_vertex_index() : polygon index out of range");
				if (vertex_offset < 0 || vertex_offset >= get_polygon_vertex_count(polygon_index))
					throw mio::IndexException("Mesh::get_polygon_vertex_index() : vertex offset out of range");
				return _get_polygon_vertex_index(polygon_index, vertex_offset);
			}

			virtual int _get_polygon_normal_index(int polygon_index, int vertex_offset)
			{
				int start = polygons_->index_start[polygon_index];
				return polygons_->normal_indices[start + vertex_offset];				
			}

			virtual int get_polygon_normal_index(int polygon_index, int vertex_offset)
			{
				if (polygon_index < 0 || polygon_index >= get_polygon_count())
					throw mio::IndexException("Mesh::get_polygon_normal_index() : polygon index out of range");
				if (vertex_offset < 0 || vertex_offset >= get_polygon_vertex_count(polygon_index))
					throw mio::IndexException("Mesh::get_polygon_normal_index() : vertex offset out of range");
				return _get_polygon_normal_index(polygon_index, vertex_offset);
			}

			IndexedMesh *__copy__();
		};
	}
}

#pragma warning(pop)

#endif