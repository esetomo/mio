#ifndef __MIO_SG_NODE_H__
#define __MIO_SG_NODE_H__

#include <mio/mio.h>
#include <mio/mio_object.h>
#include <mio/reference.h>
#include <mio/array.h>

#include <mio/point3.h>
#include <mio/vector3.h>

#include <mio/geometry/mesh.h>
#include <mio/material/material_mapping.h>
#include <mio/material/material_list.h>
#include <mio/texture/texture_mapping.h>
#include <mio/skinning/joint_weight_mapping.h>
#include <mio/rigging/armature.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
	namespace scenegraph
	{
		class NodeVisitor;

		class MIO_API Node : public MioObject
		{
		private:
			Reference<mio::geometry::Mesh> rest_mesh_;

			Array<Point3> morphed_vertices_;
			Array<Vector3> morphed_normals_;			

			Reference<mio::material::MaterialMapping> material_mapping_;
			Reference<mio::texture::TextureMapping> texture_mapping_;
			Reference<mio::material::MaterialList> material_list_;

			Reference<mio::rigging::Armature> rest_armature_;
			Reference<mio::rigging::Armature> morphed_armature_;

			Reference<mio::skinning::JointWeightMapping> joint_weight_mapping_;

		public:
			Node(const char *name="");
			virtual void accept(NodeVisitor *visitor);
			virtual ~Node();
		};
	}
}

#pragma warning(pop)

#endif