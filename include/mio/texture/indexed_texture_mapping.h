#ifndef __MIO_TEXTURE_INDEXED_TEXTURE_MAPPING_H__
#define __MIO_TEXTURE_INDEXED_TEXTURE_MAPPING_H__

#include <mio/mio.h>
#include <mio/int3.h>
#include <mio/array.h>
#include <mio/reference.h>
#include <mio/texture/texture_mapping.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
	namespace texture
	{
		class MIO_API IndexedTextureMapping : public TextureMapping
		{
		protected:
			Array<Uv> tex_coords_;
			Array<int> polygon_vertex_count_;
			Array<int> polygon_index_start_;
			Array<int> polygon_tex_coord_indices_;

		public:
			IndexedTextureMapping(const char *name="");
			virtual ~IndexedTextureMapping();

			inline Uv _get_tex_coord(int texCoordIndex) const
			{
				if (texCoordIndex < 0)
					return Uv(0,0);
				else
					return tex_coords_[texCoordIndex];
			}

			inline Uv get_tex_coord(int texCoordIndex) const
			{
				if (texCoordIndex >= get_tex_coord_count())
					throw mio::IndexException("index to texture coordinate out of range");
				else
					return _get_tex_coord(texCoordIndex);	
			}
		
			inline virtual void append_tex_coord(const Uv &uv)
			{
				tex_coords_.push_back(uv);
			}
			
			inline virtual int get_tex_coord_count() const
			{
				return tex_coords_.size();
			}

			virtual int get_polygon_count()
			{
				return polygon_vertex_count_.size();
			}

			virtual int _get_polygon_vertex_count(int polygon_index)
			{
				return polygon_vertex_count_[polygon_index];
			}

			virtual Uv _get_polygon_tex_coord(int polygon_index, int vertex_offset)
			{
				int start = polygon_index_start_[polygon_index];
				int vertex_index = polygon_tex_coord_indices_[start + vertex_offset];
				return tex_coords_[vertex_index];
			}

			virtual int _get_polygon_tex_coord_index(int polygon_index, int vertex_offset)
			{
				int start = polygon_index_start_[polygon_index];
				int vertex_index = polygon_tex_coord_indices_[start + vertex_offset];
				return vertex_index;
			}

			virtual int get_polygon_tex_coord_index(int polygon_index, int vertex_offset)
			{
				if (polygon_index < 0 || polygon_index >= get_polygon_count())
					throw mio::IndexException("Mesh::get_polygon_tex_coord_index() : polygon index out of range");
				if (vertex_offset < 0 || vertex_offset >= get_polygon_vertex_count(polygon_index))
					throw mio::IndexException("Mesh::get_polygon_tex_coord_index() : vertex offset out of range");
				return _get_polygon_tex_coord_index(polygon_index, vertex_offset);
			}

			inline void append_new_polgon()
			{
				polygon_vertex_count_.push_back(0);
				polygon_index_start_.push_back(polygon_tex_coord_indices_.size());
			}

			inline void append_tex_coord_index_to_last_polygon(int tex_coord_index)
			{
				polygon_tex_coord_indices_.push_back(tex_coord_index);	
				int last_polygon_index = polygon_vertex_count_.size() - 1;
				polygon_vertex_count_[last_polygon_index]++;
			}

			virtual void accept(mio::texture::TextureMappingVisitor *visitor);
		};
	}
}

#pragma warning(pop)

#endif