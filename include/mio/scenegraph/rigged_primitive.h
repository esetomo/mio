#ifndef __MIO_SCENEGRAPH_RIGGED_PRIMITIVE_H__
#define __MIO_SCENEGRAPH_RIGGED_PRIMITIVE_H__

#include <mio/mio.h>
#include <mio/exceptions.h>
#include <mio/scenegraph/node.h>
#include <mio/reference.h>
#include <mio/array.h>

#include <mio/transform.h>

#include <mio/geometry/indexed_mesh.h>
#include <mio/texture/texture_mapping.h>
#include <mio/material/material_mapping.h>
#include <mio/material/material_list.h>
#include <mio/rigging/armature.h>
#include <mio/skinning/indexed_joint_weight_mapping.h>
#include <mio/scenegraph/simple_primitive.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace scenegraph
    {
		class MIO_API RiggedPrimitive : public Node
        {
		public:
            RiggedPrimitive(const char *uid="");

			RiggedPrimitive(
				mio::geometry::IndexedMesh *rest_mesh,
				mio::texture::TextureMapping *texture_mapping,
				mio::material::MaterialMapping *material_mapping,
				mio::material::MaterialList *material_list,
				mio::rigging::Armature *rest_armature,
				mio::skinning::IndexedJointWeightMapping *joint_weight_mapping,
				const char *uid="");

            virtual ~RiggedPrimitive();

			mio::scenegraph::SimplePrimitive *get_morphed_primitive();
			mio::geometry::IndexedMesh *get_rest_mesh();
			mio::geometry::IndexedMesh *get_moprhed_mesh();
			mio::texture::TextureMapping *get_texture_mapping();
			mio::material::MaterialMapping *get_material_mapping();
			mio::material::MaterialList *get_material_list();
			mio::rigging::Armature *get_rest_armature();
			mio::rigging::Armature *get_morphed_armature();
			mio::skinning::IndexedJointWeightMapping *get_joint_weight_mapping();

			void update_rest_joint_transforms();
			void update_morphed_joint_transforms();

			void morph(const mio::rigging::Pose &pose);

			void accept(mio::scenegraph::NodeVisitor *visitor);

		private:
			Reference<mio::geometry::IndexedMesh> rest_mesh_;
			Reference<mio::geometry::IndexedMesh> morphed_mesh_;

			Reference<mio::texture::TextureMapping> texture_mapping_;
			Reference<mio::material::MaterialMapping> material_mapping_;
			Reference<mio::material::MaterialList> material_list_;

			Reference<mio::rigging::Armature> rest_armature_;
			Reference<mio::rigging::Armature> morphed_armature_;

			Reference<mio::skinning::IndexedJointWeightMapping> joint_weight_mapping_;

			Reference<mio::scenegraph::SimplePrimitive> morphed_primitive_;

			Array<Transform> rest_joint_transforms_;
			Array<Matrix4x4> vertex_transforms_;
			Array<Matrix4x4> normal_transforms_;
        };
    }
}

#pragma warning(pop)

#endif
