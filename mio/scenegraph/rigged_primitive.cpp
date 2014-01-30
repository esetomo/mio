#include <mio/scenegraph/rigged_primitive.h>
#include <mio/scenegraph/node_visitor.h>

namespace mio
{
    namespace scenegraph
    {
		RiggedPrimitive::RiggedPrimitive(const char *uid) : Node(uid)
        {
        }

		RiggedPrimitive::RiggedPrimitive(
			mio::geometry::IndexedMesh *rest_mesh,
			mio::texture::TextureMapping *texture_mapping,
			mio::material::MaterialMapping *material_mapping,
			mio::material::MaterialList *material_list,
			mio::rigging::Armature *rest_armature,
			mio::skinning::IndexedJointWeightMapping *joint_weight_mapping,
			const char *uid) : Node(uid)
		{
			if (rest_mesh == NULL)
				throw mio::ValueException("RiggedPrimitive::RiggedPrimitive() : rest_mesh cannot be NULL");
			if (rest_mesh->get_vertex_count() != rest_mesh->get_normal_count())
				throw mio::ValueException("RiggedPrimitive::RiggedPrimitive() : the number of vertices is not equal to the number of normals");

			if (texture_mapping == NULL)
				throw mio::ValueException("RiggedPrimitive::RiggedPrimitive() : texture_mapping cannot be NULL");
			if (material_mapping == NULL)
				throw mio::ValueException("RiggedPrimitive::RiggedPrimitive() : material_mapping cannot be NULL");
			if (material_list == NULL)
				throw mio::ValueException("RiggedPrimitive::RiggedPrimitive() : material_list cannot be NULL");
			if (rest_armature == NULL)
				throw mio::ValueException("RiggedPrimitive::RiggedPrimitive() : rest_armature cannot be NULL");
			if (joint_weight_mapping == NULL)
				throw mio::ValueException("RiggedPrimitive::RiggedPrimitive() : joint_weight_mapping cannot be NULL");

			rest_mesh_ = rest_mesh;
			texture_mapping_ = texture_mapping;
			material_mapping_ = material_mapping;
			material_list_ = material_list;
			rest_armature_ = rest_armature;
			joint_weight_mapping_ = joint_weight_mapping;

			morphed_mesh_ = rest_mesh_->__copy__();
			morphed_armature_ = rest_armature_->__copy__();

			morphed_primitive_ = new SimplePrimitive(
				morphed_mesh_.get_pointer(),
				texture_mapping_.get_pointer(),
				material_mapping_.get_pointer(),
				material_list_.get_pointer());

			update_rest_joint_transforms();
		}

        RiggedPrimitive::~RiggedPrimitive()
        {
        }

		mio::scenegraph::SimplePrimitive *RiggedPrimitive::get_morphed_primitive()
		{
			return morphed_primitive_.get_pointer();
		}

		mio::geometry::IndexedMesh *RiggedPrimitive::get_rest_mesh()
		{
			return rest_mesh_.get_pointer();
		}

		mio::geometry::IndexedMesh *RiggedPrimitive::get_moprhed_mesh()
		{
			return morphed_mesh_.get_pointer();
		}

		mio::texture::TextureMapping *RiggedPrimitive::get_texture_mapping()
		{
			return texture_mapping_.get_pointer();
		}

		mio::material::MaterialMapping *RiggedPrimitive::get_material_mapping()
		{
			return material_mapping_.get_pointer();
		}

		mio::material::MaterialList *RiggedPrimitive::get_material_list()
		{
			return material_list_.get_pointer();
		}

		mio::rigging::Armature *RiggedPrimitive::get_rest_armature()
		{
			return rest_armature_.get_pointer();
		}

		mio::rigging::Armature *RiggedPrimitive::get_morphed_armature()
		{
			return morphed_armature_.get_pointer();
		}

		mio::skinning::IndexedJointWeightMapping *RiggedPrimitive::get_joint_weight_mapping()
		{
			return joint_weight_mapping_.get_pointer();
		}

		void RiggedPrimitive::update_rest_joint_transforms()
		{
			ADJUST_ARRAY_SIZE(rest_joint_transforms_, rest_armature_->get_joint_count(), Transform::identity());
			FOR(joint_index, rest_armature_->get_joint_count())			
			{
				rest_joint_transforms_[joint_index] = rest_armature_->_get_joint_to_world_transform(joint_index);
			}
		}

		void RiggedPrimitive::update_morphed_joint_transforms()
		{			
			ADJUST_ARRAY_SIZE(vertex_transforms_, morphed_armature_->get_joint_count(), Matrix4x4::identity());
			ADJUST_ARRAY_SIZE(normal_transforms_, morphed_armature_->get_joint_count(), Matrix4x4::identity());
			FOR(joint_index, morphed_armature_->get_joint_count())
			{
				Transform morphed_joint_to_world = morphed_armature_->_get_joint_to_world_transform(joint_index);
				Transform rest_joint_to_world = rest_joint_transforms_[joint_index];
				vertex_transforms_[joint_index] = morphed_joint_to_world.m * rest_joint_transforms_[joint_index].mi;
				normal_transforms_[joint_index] = morphed_joint_to_world.mit * transpose(rest_joint_transforms_[joint_index].m);
			}
		}

		void RiggedPrimitive::morph(const mio::rigging::Pose &pose)
		{
			rest_armature_->morph(*morphed_armature_.get_pointer(), pose);
			update_morphed_joint_transforms();
			
			FOR(vertex_index, rest_mesh_->get_vertex_count())
			{
				Point3 vertex = rest_mesh_->_get_vertex(vertex_index);
				Vector3 normal = rest_mesh_->_get_normal(vertex_index);
				
				Point3 morphed_vertex(0,0,0);
				Vector3 morphed_normal(0,0,0);
				
				FOR(joint_order, joint_weight_mapping_->get_influencing_joint_count(vertex_index))
				{
					int joint_index = joint_weight_mapping_->get_influencing_joint_index(vertex_index, joint_order);
					float weight = joint_weight_mapping_->get_influencing_joint_weight(vertex_index, joint_order);					

					Point3 vv = vertex_transforms_[joint_index] * vertex;
					Vector3 v(vv.x, vv.y, vv.z);
					
					Vector3 n = normal_transforms_[joint_index] * normal;
					
					morphed_vertex = morphed_vertex + (v * weight);
					morphed_normal = morphed_normal + (n * weight);
				}

				if (morphed_normal.length() > EPSILON)
					morphed_normal = normalize(morphed_normal);				
								
				morphed_mesh_->set_vertex(vertex_index, morphed_vertex);
				morphed_mesh_->set_normal(vertex_index, morphed_normal);
			}
		}

		void RiggedPrimitive::accept(mio::scenegraph::NodeVisitor *visitor)
		{
			visitor->visit_RiggedPrimitive(this);
		}
    }
}
