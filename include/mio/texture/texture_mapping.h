#ifndef __MIO_MATERIAL_TEXTURE_MAPPING_H__
#define __MIO_MATERIAL_TEXTURE_MAPPING_H__

#include <mio/mio.h>
#include <mio/exceptions.h>
#include <mio/array.h>
#include <mio/uv.h>
#include <mio/mio_object.h>

namespace mio
{
	namespace texture
	{		
		class TextureMappingVisitor;

		class MIO_API TextureMapping : public MioObject
		{
		public:
			TextureMapping(const char *name="");
			virtual ~TextureMapping();

			virtual int get_polygon_count() = 0;

			virtual int _get_polygon_vertex_count(int polygon_index) = 0;

			virtual int get_polygon_vertex_count(int polygon_index)
			{
				if (polygon_index < 0 || polygon_index >= get_polygon_count())
					throw mio::IndexException("TextureMapping::get_polygon_vertex_count() : polygon index out of range");
				else
					return _get_polygon_vertex_count(polygon_index);
			}

			virtual Uv _get_polygon_tex_coord(int polygon_index, int vertex_offset) = 0;

			virtual Uv get_polygon_tex_coord(int polygon_index, int vertex_offset)
			{
				if (polygon_index < 0 || polygon_index >= get_polygon_count())
					throw mio::IndexException("TextureMapping::get_polygon_tex_coord() : polygon index out of range");
				if (vertex_offset < 0 || vertex_offset >= get_polygon_vertex_count(polygon_index))
					throw mio::IndexException("TextureMapping::get_polygon_tex_coord() : vertex offset out of range");
				return _get_polygon_tex_coord(polygon_index, vertex_offset);
			}

			virtual void accept(mio::texture::TextureMappingVisitor *visitor);
		};
	}
}

#endif