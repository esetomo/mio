#ifndef __OMNI_VISITOR_H__
#define __OMNI_VISITOR_H__

#include <mio/geometry/mesh_visitor.h>
#include <mio/material/material_visitor.h>
#include <mio/material/material_mapping_visitor.h>
#include <mio/scenegraph/node_visitor.h>
#include <mio/texture/texture_visitor.h>
#include <mio/texture/texture_mapping_visitor.h>

namespace mio
{
	namespace visitor
	{
		class OmniVisitor : 
			public mio::scenegraph::NodeVisitor, 
			public mio::material::MaterialVisitor,
			public mio::material::MaterialMappingVisitor,
			public mio::texture::TextureVisitor, 
			public mio::texture::TextureMappingVisitor,
			public mio::geometry::MeshVisitor
		{
		};
	}
}

#endif