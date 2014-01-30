#ifndef __MIO_SCENEGRAPH_BY_TRIANGLE_PRIMITIVE_H__
#define __MIO_SCENEGRAPH_BY_TRIANGLE_PRIMITIVE_H__

#include <mio/mio.h>
#include <mio/reference.h>
#include <mio/geometry/mesh.h>
#include <mio/texture/texture_mapping.h>
#include <mio/material/material_mapping.h>
#include <mio/material/material_list.h>
#include <mio/scenegraph/node.h>

namespace mio
{
	namespace scenegraph
	{
		using namespace geometry;
		using namespace texture;
		using namespace material;

		class MIO_API SimplePrimitive : public Node
		{
		protected:
			Reference<Mesh> mesh_;
			Reference<MaterialMapping> material_mapping_;
			Reference<TextureMapping> texture_mapping_;
			Reference<MaterialList> material_list_;

		public:				
			SimplePrimitive(const char *uid="");
			SimplePrimitive(mio::geometry::Mesh *mesh, 
				mio::texture::TextureMapping *texture_mapping,
				mio::material::MaterialMapping *material_mapping,
				mio::material::MaterialList *material_list,
				const char *uid="");
			virtual ~SimplePrimitive();			

			inline Mesh *get_mesh()
			{
				return mesh_.get_pointer();
			}

			inline void set_mesh(Mesh *mesh)
			{
				mesh_ = mesh;
			}

			inline MaterialMapping *get_material_mapping()
			{
				return material_mapping_.get_pointer();
			}

			inline void set_material_mapping(MaterialMapping *material_mapping)
			{
				material_mapping_ = material_mapping;
			}

			inline TextureMapping *get_texture_mapping()
			{
				return texture_mapping_.get_pointer();
			}

			inline void set_texture_mapping(TextureMapping *texture_mapping)
			{
				texture_mapping_ = texture_mapping;
			}

			inline MaterialList *get_material_list()
			{
				return material_list_.get_pointer();
			}

			inline void set_material_list(MaterialList *material_list)
			{
				material_list_ = material_list;
			}

			virtual void accept(NodeVisitor *visitor);
		};
	}
}

#endif