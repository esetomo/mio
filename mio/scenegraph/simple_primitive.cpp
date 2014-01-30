#include <mio/scenegraph/simple_primitive.h>
#include <mio/scenegraph/node_visitor.h>

namespace mio
{
	namespace scenegraph
	{
		SimplePrimitive::SimplePrimitive(const char *uid) : Node(uid)
		{
		}

		SimplePrimitive::SimplePrimitive(mio::geometry::Mesh *mesh, 
			mio::texture::TextureMapping *texture_mapping, 
			mio::material::MaterialMapping *material_mapping,
			mio::material::MaterialList *material_list,
			const char *uid) : Node(uid)
		{
			mesh_ = mesh;
			texture_mapping_ = texture_mapping;
			material_mapping_ = material_mapping;
			material_list_ = material_list;
		}

		SimplePrimitive::~SimplePrimitive()
		{
		}

		void SimplePrimitive::accept(NodeVisitor *visitor)
		{
			visitor->visit_SimplePrimitive(this);
		}
	}
}