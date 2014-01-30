%module(directors="1") cmio
%include <windows.i>
%include <exception.i>

%exception {
	try 
	{
		$action
	} 
	catch (mio::IndexException &e) 
	{
		SWIG_exception(SWIG_IndexError, const_cast<char*>(e.what()));
	}
	catch (mio::TypeException &e) 
	{
		SWIG_exception(SWIG_TypeError, const_cast<char*>(e.what()));
	}
	catch (mio::ValueException &e) 
	{
		SWIG_exception(SWIG_ValueError, const_cast<char*>(e.what()));
	}
	catch (std::exception &e)
	{
		SWIG_exception(SWIG_RuntimeError, const_cast<char*>(e.what()));
	}
	catch (...)
	{
		SWIG_exception(SWIG_RuntimeError, "unknown exception");
	}
}

%include <mio/mio.h>
%include <mio/vector3.i>
%include <mio/point3.i>
%include <mio/ray.i>
%include <mio/box.i>
%include <mio/matrix4x4.i>
%include <mio/transform.i>
%include <mio/quaternion.i>
%include <mio/rgb.i>
%include <mio/rgba.i>
%include <mio/int3.i>
%include <mio/uv.i>

%include <mio/reference_counted.i>
%include <mio/mio_object.i>

// Geometry
%include <mio/geometry/mesh.i>
%include <mio/geometry/indexed_mesh.i>

// GL
%include <mio/gl/gl_texture.i>

// IL
%include <mio/il/il_image.i>
%include <mio/il/il_image_service.i>

// Texture
%include <mio/texture/texture.i>
%include <mio/texture/texture_mapping.i>
%include <mio/texture/indexed_texture_mapping.i>
%include <mio/texture/il_texture.i>
%include <mio/texture/lazy_il_texture.i>

// Material
%include <mio/material/material.i>
%include <mio/material/material_list.i>
%include <mio/material/material_mapping.i>
%include <mio/material/mqo_material.i>
%include <mio/material/phong_material.i>
%include <mio/material/textured_mqo_material.i>
%include <mio/material/textured_phong_material.i>
%include <mio/material/by_polygon_material_mapping.i>

// Rigging
%include <mio/rigging/joint_change.i>
%include <mio/rigging/joint.i>
%include <mio/rigging/pose.i>
%include <mio/rigging/armature.i>

// IK
%include <mio/ik/ik_joint_parameters.i>
%include <mio/ik/ik_end_effector.i>
%include <mio/ik/ik_joint.i>
%include <mio/ik/ik_end_effector_goals.i>
%include <mio/ik/ik_pose.i>
%include <mio/ik/ik_armature.i>

// Skinning
%include <mio/skinning/joint_weight_mapping.i>
%include <mio/skinning/indexed_joint_weight_mapping.i>

// Curve
%include <mio/curve/curve.i>
%include <mio/curve/polyline.i>

// Rigging
%include <mio/rigging/pose_animation.i>

// Scene Graph
%include <mio/scenegraph/node.i>
%include <mio/scenegraph/transform_node.i>
%include <mio/scenegraph/group.i>
%include <mio/scenegraph/simple_primitive.i>
%include <mio/scenegraph/rigged_primitive.i>

// Visitors
%include <mio/geometry/mesh_visitor.i>
%include <mio/material/material_visitor.i>
%include <mio/material/material_mapping_visitor.i>
%include <mio/texture/texture_visitor.i>
%include <mio/texture/texture_mapping_visitor.i>
%include <mio/scenegraph/node_visitor.i>
%include <mio/visitor/omni_visitor.i>

%include <mio/gl/gl_renderer.i>