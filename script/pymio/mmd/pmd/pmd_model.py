import copy
import math

from pymio.material.material_list import MaterialList
from pymio.material.phong_material import PhongMaterial
from pymio.material.textured_phong_material import TexturedPhongMaterial
from pymio.material.by_polygon_material_mapping import ByPolygonMaterialMapping

from pymio.geometry.indexed_mesh import IndexedMesh

from pymio.texture.indexed_texture_mapping import IndexedTextureMapping
from pymio.texture.lazy_il_texture import LazyILTexture

from pymio.rigging.armature import Armature
from pymio.rigging.joint import Joint
from pymio.rigging.pose import Pose
from pymio.rigging.joint_change import JointChange

from pymio.ik.ik_armature import IKArmature
from pymio.ik.ik_joint_parameters import IKJointParameters
from pymio.ik.ik_end_effector import IKEndEffector
from pymio.ik.ik_joint import IKJoint
from pymio.ik.ik_pose import IKPose

from pymio.skinning.indexed_joint_weight_mapping import IndexedJointWeightMapping

from pymio.scenegraph.rigged_primitive import RiggedPrimitive

from pymio import vector3
from pymio.vector3 import Vector3
from pymio import vector3
from pymio.point3 import Point3
from pymio.rgba import Rgba
from pymio.uv import Uv
from pymio.quaternion import Quaternion
from pymio import quaternion
from pymio.matrix4x4 import Matrix4x4
from pymio import matrix4x4

from bone import Bone
from expression import Expression

def coordinate_system(v1):
    v1 = vector3.normalize(v1)
    if abs(v1.x) > abs(v1.y):
        inv_len = 1.0 / math.sqrt(v1.x**2 + v1.z**2)
        v2 = Vector3(-v1.z * inv_len, 0.0, v1.x * inv_len)
    else:
        inv_len = 1.0 / math.sqrt(v1.y**2 + v1.z**2)
        v2 = Vector3(0.0, v1.z * inv_len, -v1.y * inv_len)
    v3 = vector3.cross(v1,v2)
    return (v1, v2, v3)

class PMDModel(object):
    """
    Represent the data stored in a .pmd file.
    """
    
    def __init__(self):
        """
        Create an empty instance of C{PMDModel}.
        """
        self.vertices = []
        self.bones = []
        self.ik_chains = []
        self.expressions = []
        self.base_expression = Expression("base")
        self.triangle_count = 0
        self.materials = []
        self.triangle_vertex_indices = []
        
        self.bones_by_name = {}
        
    def append_vertex(self, vertex):
        """
        Add a vertex data item.
        
        @param vertex: a vertex data item
        @type vertex: L{pymio.mmd.pmd.vertex.Vertex}
        """        
        self.vertices.append(vertex)
        
    def append_bone(self, bone):
        """
        Add a bone data item.
        
        @param bone: a bone data item
        @type bone: L{pymio.mmd.pmd.bone.Bone}
        """
        self.bones.append(bone)
        bone.index = len(self.bones)-1
        self.bones_by_name[bone.name] = bone
        
    def get_bone_by_name(self, name):
        """
        Return a bone with the given name.
        
        @param name: a bone's name
        @type name: string
        """
        return self.bones_by_name[name]
        
    def append_expression(self, expression):
        self.expressions.append(expression)
        
    def append_material(self, material):
        self.materials.append(material)
        
    def append_triangle_vertex_index(self, index):
        self.triangle_vertex_indices.append(index)
        
    def append_ik_chain(self, ik_chain):
        self.ik_chains.append(ik_chain)
        
    def get_ik_chain_by_ik_bone_index(self, ik_bone_index):
        for ik_chain in self.ik_chains:
            if ik_chain.ik_bone_index == ik_bone_index:
                return ik_chain
        raise KeyError("cannot find IK chain represented with a bone with the given index")
    
    def get_ik_chain_by_ik_bone_name(self, ik_bone_name):
        for ik_chain in self.ik_chains:
            if self.bones[ik_chain.ik_bone_index].name == ik_bone_name:
                return ik_chain
        raise KeyError("cannot find IK chain represented with a bone with the given index")
        
    def get_material_list(self):
        material_list = MaterialList()
        for material in self.materials:
            if len(material.texture_filename) > 0:
                texture = LazyILTexture(material.texture_filename)
                pymio_material = TexturedPhongMaterial(material.ambient, 
                                                       material.diffuse, 
                                                       Rgba(0,0,0,0), 
                                                       material.specular, 
                                                       material.shininess, 
                                                       texture)
            else:
                pymio_material = PhongMaterial(material.ambient, 
                                               material.diffuse, 
                                               Rgba(0,0,0,0), 
                                               material.specular, 
                                               material.shininess)
            material_list.append_material(pymio_material)
        return material_list
    
    def get_rest_mesh(self):
        mesh = IndexedMesh()        
        for vertex in self.vertices:
            mesh.append_vertex(Point3(vertex.position.x, vertex.position.y, -vertex.position.z))
            mesh.append_normal(Vector3(vertex.normal.x, vertex.normal.y, -vertex.normal.z))            
            #mesh.append_vertex(Point3(vertex.position.x, vertex.position.y, vertex.position.z))
            #mesh.append_normal(Vector3(vertex.normal.x, vertex.normal.y, vertex.normal.z))
        for i in xrange(self.triangle_count):            
            v0 = self.triangle_vertex_indices[3*i]
            v1 = self.triangle_vertex_indices[3*i+1]
            v2 = self.triangle_vertex_indices[3*i+2]
            mesh.append_new_polygon()
            mesh.append_vertex_normal_index_to_last_polygon(v2, v2)
            mesh.append_vertex_normal_index_to_last_polygon(v1, v1)
            mesh.append_vertex_normal_index_to_last_polygon(v0, v0)            
        return mesh
    
    def get_texture_mapping(self):
        mapping = IndexedTextureMapping()
        for vertex in self.vertices:
            mapping.append_tex_coord(Uv(vertex.tex_coord.u, 1.0 - vertex.tex_coord.v))            
        for i in xrange(self.triangle_count):
            v0 = self.triangle_vertex_indices[3*i]
            v1 = self.triangle_vertex_indices[3*i+1]
            v2 = self.triangle_vertex_indices[3*i+2]
            mapping.append_new_polgon()
            mapping.append_tex_coord_index_to_last_polygon(v2)
            mapping.append_tex_coord_index_to_last_polygon(v1)
            mapping.append_tex_coord_index_to_last_polygon(v0)
        return mapping
    
    def get_material_mapping(self):
        mapping = ByPolygonMaterialMapping()
        for material_index, material in enumerate(self.materials):
            for i in xrange(material.triangle_count):
                mapping.append_mapping(material_index)
        return mapping
    
    def get_rest_armature(self):
        armature = Armature()
            
        necessary = [False for i in xrange(len(self.bones))]
        
        for ik_chain in self.ik_chains:
            for index in ik_chain.affected_bone_indices:
                necessary[index] = True
            necessary[ik_chain.end_effector_index] = True
        
        for vertex in self.vertices:
            necessary[vertex.bone0_number] = True
            necessary[vertex.bone1_number] = True
        
        for bone_index, bone in enumerate(self.bones):
            if (necessary[bone_index] or
                (bone.bone_type in [Bone.BONE_ROTATE, 
                                  Bone.BONE_ROTATE_TRANSLATE,       
                                  Bone.BONE_IK_ROTATION_INFLUENCED,
                                  Bone.BONE_IK_TARGET,
                                  Bone.BONE_ROTATION_INFLUENCED])):
                current = bone_index
                while current >= 0:
                    necessary[current] = True
                    current = self.bones[current].parent_index
        
        for bone_index, bone in enumerate(self.bones):
            if necessary[bone_index]:
                if bone.parent_index >= 0:
                    parent = self.bones[bone.parent_index]                    
                    position = bone.position - parent.position
                    position = Vector3(position.x, position.y, -position.z)
                    #position = Vector3(position.x, position.y, position.z)
                else:
                    position = Vector3(bone.position.x, bone.position.y, -bone.position.z)
                    #position = Vector3(bone.position.x, bone.position.y, bone.position.z)
                joint = Joint(bone.name, position, Quaternion())
                armature.append_joint(joint)
                    
        for bone_index, bone in enumerate(self.bones):
            try:
                joint = armature.get_joint(bone.name)
            except ValueError:
                continue
            if bone.parent_index >= 0:                                
                armature.set_parent(joint.get_name(), self.bones[bone.parent_index].name)
                
        return armature    
         
    def get_ik_armature(self):
        rest_armature = self.get_rest_armature()
        ik_armature = IKArmature(rest_armature)
        
        def get_corresponding_rest_joint(joint_index):            
            bone = self.bones[joint_index]            
            rest_joint = rest_armature.get_joint(bone.name)
            return rest_joint
                
        ik_joint_index_set = set()
        for ik_chain in self.ik_chains:
            end_effector_joint = get_corresponding_rest_joint(ik_chain.end_effector_index)            
            end_effector = ik_armature.make_end_effector(end_effector_joint.index)                         
                        
            for ik_bone_index in ik_chain.affected_bone_indices:
                ik_rest_joint = get_corresponding_rest_joint(ik_bone_index)
                ik_joint_index = ik_rest_joint.index
                end_effector.append_ik_joint_index(ik_rest_joint.index)                                
                         
                joint = rest_armature.get_joint(ik_rest_joint.index)
                joint_name = joint.get_name()
                
                ik_joint_index_set.add(ik_joint_index)
        
        for ik_joint_index in ik_joint_index_set:                
            ik_armature.make_ik_joint(ik_joint_index)            
        
        return ik_armature
    
    def get_joint_weight_mapping(self):
        rest_armature = self.get_rest_armature()        
        joint_weight_mapping = IndexedJointWeightMapping()
        for vertex in self.vertices:
            joint_weight_mapping.append_new_vertex()
            
            bone0 = self.bones[vertex.bone0_number]
            bone1 = self.bones[vertex.bone1_number]
            
            joint0 = rest_armature.get_joint(bone0.name)
            joint1 = rest_armature.get_joint(bone1.name)
                    
            if vertex.weight == 1.0:                
                joint_weight_mapping.append_joint_weight(joint0.index, 1.0)
            elif vertex.weight == 0:
                joint_weight_mapping.append_joint_weight(joint1.index, 1.0)
            else:
                joint_weight_mapping.append_joint_weight(joint0.index, vertex.weight)
                joint_weight_mapping.append_joint_weight(joint1.index, 1.0 - vertex.weight)
            
        return joint_weight_mapping
    
    def get_bone_displacement_from_parent(self, bone_index):
        bone = self.bones[bone_index]
        if bone.parent_index >= 0:
            parent = self.bones[bone.parent_index]
            return bone.position - parent.position
        else:
            return Vector3(bone.position.x, bone.position.y, bone.position.z)
        
    def get_rigged_primitive(self):
        rest_mesh = self.get_rest_mesh()
        texture_mapping = self.get_texture_mapping()
        material_mapping = self.get_material_mapping()
        material_list = self.get_material_list()
        rest_armature = self.get_rest_armature()
        joint_weight_mapping = self.get_joint_weight_mapping()
        
        return RiggedPrimitive(
            rest_mesh, 
            texture_mapping, 
            material_mapping,
            material_list,
            rest_armature,
            joint_weight_mapping)
        
    def get_ik_chain_ik_armature(self, ik_chain_index):
        ik_chain = self.ik_chains[ik_chain_index]
        rest_armature = Armature()
                        
        joint_indices = ik_chain.affected_bone_indices + [ik_chain.end_effector_index]                
        for joint_index in joint_indices:
            rest_armature.append_joint(Joint(self.bones[joint_index].name))
        root_name = None
        for joint_index in joint_indices:
            bone = self.bones[joint_index]
            if bone.parent_index in joint_indices:
                child_name = self.bones[joint_index].name
                parent_index = self.bones[joint_index].parent_index
                parent_name = self.bones[parent_index].name
                rest_armature.set_parent(child_name, parent_name)
                joint = rest_armature.get_joint(child_name)
                position = matrix4x4.scale(1,1,-1) * (self.bones[joint_index].position - self.bones[parent_index].position)
                joint.position = position
            else:
                root_name = bone.name
        
        ik_bone_name = self.bones[ik_chain.ik_bone_index].name 
        ik_armature = IKArmature(rest_armature)
        end_effector_name = self.bones[ik_chain.end_effector_index].name
        ik_armature.make_end_effector(end_effector_name)
        for joint_index in ik_chain.affected_bone_indices:
            bone = self.bones[joint_index]
            ik_joint = ik_armature.make_ik_joint(bone.name)
            ik_armature.add_ik_joint_to_end_effector(bone.name, end_effector_name)
            ik_joint.disable_parameter(IKJointParameters.X_TRANSLATE)             
            ik_joint.disable_parameter(IKJointParameters.Y_TRANSLATE)
            ik_joint.disable_parameter(IKJointParameters.Z_TRANSLATE)
                        
            #limit_0 = ik_chain.constant_0
            #limit_1 = 360 * ik_chain.constant_1                            
            #if ik_joint.get_name() != root_name:
                #ik_joint.set_limit(IKJointParameters.X_ROTATE, -limit_1, limit_1)
                #ik_joint.disable_parameter(IKJointParameters.Y_ROTATE)
                #ik_joint.set_limit(IKJointParameters.Y_ROTATE, -limit_1, limit_1)                    
                #ik_joint.set_limit(IKJointParameters.Z_ROTATE, -limit_1, limit_1)                
                
        if ik_bone_name in ["leg_ik.R", "leg_ik.L"]:
            if ik_bone_name == "leg_ik.R":
                knee_joint = ik_armature.get_ik_joint("knee.R")
                leg_joint = ik_armature.get_ik_joint("leg.R")
            else:
                leg_joint = ik_armature.get_ik_joint("leg.L")
                knee_joint = ik_armature.get_ik_joint("knee.L")
            
            knee_joint.disable_parameter(IKJointParameters.Y_ROTATE)    
            knee_joint.disable_parameter(IKJointParameters.Z_ROTATE)
            knee_joint.set_limit(IKJointParameters.X_ROTATE, 0, 180)
            
            leg_joint.disable_parameter(IKJointParameters.Y_ROTATE)
            leg_joint.disable_parameter(IKJointParameters.Z_ROTATE)
        #else:
            #root_joint = ik_armature.get_ik_joint(root_name)
            #root_joint.disable_parameter(IKJointParameters.Y_ROTATE)
            #root_joint.disable_parameter(IKJointParameters.Z_ROTATE)            
        
        return ik_armature, root_name, end_effector_name
    
    def get_morphed_world_position(self, bone_index, vpd_pose):
        bone = self.bones[bone_index]
        result = Point3(0,0,0)    
        while bone is not None:
            if bone.name in vpd_pose.joint_change_by_bone_name:
                joint_change = vpd_pose.get_right_handed_joint_change(bone.name)
                result = joint_change.orientation.rotate(result)
                result += joint_change.position            
            result += matrix4x4.scale(1,1,-1) * self.get_bone_displacement_from_parent(bone_index)            
            if bone.parent_index >= 0:
                bone_index = bone.parent_index
                bone = self.bones[bone.parent_index]                
            else:
                bone = None        
        return result
    
    def get_morphed_world_orientation(self, bone_index, vpd_pose):
        bone = self.bones[bone_index]
        result = Quaternion()    
        while bone is not None:            
            if bone.name in vpd_pose.joint_change_by_bone_name:
                joint_change = vpd_pose.get_right_handed_joint_change(bone.name)
                result = joint_change.orientation * result                            
            if bone.parent_index >= 0:
                bone_index = bone.parent_index
                bone = self.bones[bone.parent_index]                
            else:
                bone = None        
        return result
    
    def __compute_ik_chain_ik(self, target_pose, rest_armature, morphed_armature, vpd_pose,
                            ik_bone_name, ik_armature, root_name, end_effector_name):        
        root_bone = self.bones_by_name[root_name]
        root_position = morphed_armature.get_joint_world_position(root_bone.name)             
        root_orientation = morphed_armature.get_joint_world_orientation(root_bone.name)
        
        end_effector_bone = self.bones_by_name[end_effector_name]
        end_effector_position = morphed_armature.get_joint_world_position(end_effector_name)            
        
        ik_bone = self.bones_by_name[ik_bone_name]
        goal_position = self.get_morphed_world_position(ik_bone.index, vpd_pose)
        goal_orientation = self.get_morphed_world_orientation(ik_bone.index, vpd_pose)        
        
        if ik_bone_name in ["leg_ik.R", "leg_ik.L", "tiptoe_ik.R", "tiptoe_ik.L"]:    
            # calculate the rotation need to rotate the vector
            # (end_effector - root) to (goal - root)
            root_to_end_effector = end_effector_position - root_position
            root_to_end_effector_length = root_to_end_effector.length()
            root_to_goal = goal_position - root_position
            root_to_goal_length = root_to_goal.length()
            dot_prod = vector3.dot(root_to_end_effector, root_to_goal)
                    
            if root_to_end_effector_length > 0.00001:
                dot_prod /= root_to_end_effector_length
            if root_to_goal_length > 0.00001:
                dot_prod /= root_to_goal_length
            
            if abs(dot_prod-1) > 0.0001:
                cos_theta = dot_prod
                theta = math.acos(cos_theta)                
                axis = vector3.normalize(vector3.cross(root_to_end_effector, root_to_goal))
                
                rotation = Quaternion(theta * 180 / math.pi, axis)
                
                root_orientation = rotation * root_orientation            
                
        chain_rest_armature = ik_armature.get_rest_armature()
        chain_rest_armature.set_joint_parameter(root_name, 
                                                Vector3(root_position.x, root_position.y, root_position.z), 
                                                root_orientation)
                
        # create a pose with joint changes
        # derived from the VPD pose. 
        chain_normal_pose = Pose()
        for ik_joint in ik_armature.get_ik_joints():
            ik_joint_name = ik_joint.get_name()
            chain_normal_pose.set_joint_change(
                ik_joint_name, 
                target_pose.get_joint_change(ik_joint_name))
        chain_normal_pose.set_joint_change(root_name, JointChange())
            
        chain_ik_pose = IKPose()
        chain_ik_pose.set_normal_pose(chain_normal_pose)
        chain_ik_pose.get_end_effector_goals().set_end_effector_position(end_effector_name, goal_position)
        
        result_pose = Pose()            
        ik_armature.solve_for_pose(result_pose, chain_ik_pose, 30, 0.0001)
                
        for ik_joint in ik_armature.get_ik_joints():
            joint_change = result_pose.get_joint_change(ik_joint.get_name())
            target_pose.set_joint_change(ik_joint.get_name(), joint_change)
        
        if ik_bone_name in ["leg_ik.R", "leg_ik.L", "tiptoe_ik.R", "tiptoe_ik.L"]:                        
            # Fix the root joint change so that it is given
            # with respect to its parent.
            root_parent_orientation = morphed_armature.get_joint_world_orientation(self.bones[self.bones_by_name[root_name].parent_index].name)
            fixed_root_orientation = quaternion.inverse(root_parent_orientation) * root_orientation * result_pose.get_joint_change(root_name).orientation
            target_pose.set_joint_change(root_name, 
                                         JointChange(Vector3(0,0,0), fixed_root_orientation))
        
        rest_armature.morph(morphed_armature, target_pose)
        
    def __topologically_sort_ik_armatures(self, ik_armatures_and_other_info):
        class Node:
            def __init__(self, quintet):
                self.quintet = quintet
                self.indegree = 0
                self.points_to = []
            
            def add_edge_to(self, other):
                other.indegree += 1
                self.points_to.append(other)
                
        nodes = []
        for quintet in ik_armatures_and_other_info:
            new_node = Node(quintet)
            nodes.append(new_node)
        for node in nodes:
            for other in nodes:
                if other == node:
                    continue
                ik_chain_node = self.ik_chains[node.quintet[0]]
                ik_chain_other = self.ik_chains[other.quintet[0]]
                
                if ik_chain_node.end_effector_index in ik_chain_other.affected_bone_indices:
                    node.add_edge_to(other)
        
        # topological sort
        ik_armatures_and_other_info = []
        while len(nodes) > 0:
            indegree_zero = filter(lambda x: x.indegree==0, nodes)
            for node in indegree_zero:
                ik_armatures_and_other_info.append(node.quintet)
                for other in node.points_to:
                    other.indegree -= 1
                nodes.remove(node)
        
        return ik_armatures_and_other_info        
        
    def get_ik_less_pose(self, vpd_pose):
        ik_pose = vpd_pose.get_ik_pose(self)        
        normal_pose = Pose(ik_pose.get_normal_pose())        
        rest_armature = self.get_rest_armature()
        morphed_armature = Armature(rest_armature)
        rest_armature.morph(morphed_armature, normal_pose)
        
        ik_armatures_and_other_info = []
        for i in xrange(len(self.ik_chains)):
            ik_chain = self.ik_chains[i]
            ik_armature, root_name, end_effector_name = self.get_ik_chain_ik_armature(i)
            ik_armatures_and_other_info.append((i, self.bones[ik_chain.ik_bone_index].name, ik_armature, root_name, end_effector_name))            
        ik_armatures_and_other_info = self.__topologically_sort_ik_armatures(ik_armatures_and_other_info)        
        
        for data_quadruple in ik_armatures_and_other_info:       
            ik_chain_index, ik_bone_name, ik_armature, root_name, end_effector_name = data_quadruple
            self.__compute_ik_chain_ik(normal_pose,
                                       rest_armature, morphed_armature,
                                       vpd_pose,
                                       ik_bone_name, ik_armature, 
                                       root_name, end_effector_name)                                
        return normal_pose
        
def make_pmd_model(raw):
    """
    Create an instance of PMDModel from raw data
    parsed from .pmd file by pymio.parser.pmd_parser.PMDParser.
    """
    model = PMDModel()
    
    from vertex import Vertex
    from pymio.vector3 import Vector3
    from pymio.uv import Uv
    
    for i in xrange(len(raw["Vertex"])):
        vertex_info = raw["Vertex"][i]
        weight_info = raw["Weight"][i]
        
        position = Point3(vertex_info[0], vertex_info[1], vertex_info[2])
        normal = Vector3(vertex_info[3], vertex_info[4], vertex_info[5])
        tex_coord = Uv(vertex_info[6], vertex_info[7])
                 
        model.append_vertex(Vertex(position, normal, tex_coord,
                                   weight_info[0], weight_info[1],
                                   weight_info[2]/100.0))
        
    model.triangle_count = len(raw["Poly"]) / 3
    for index in raw["Poly"]:
        model.append_triangle_vertex_index(index)
            
    from material import Material   
    from pymio.rgba import Rgba
        
    for i in xrange(len(raw["Material"])):
        material_info = raw["Material"][i]
        
        diffuse = Rgba(material_info[0],
                       material_info[1],
                       material_info[2],
                       material_info[3])
        shininess = material_info[4]
        specular = Rgba(material_info[5],
                        material_info[6],
                        material_info[7],
                        diffuse.a)
        ambient = Rgba(material_info[8],
                       material_info[9],
                       material_info[10],
                       diffuse.a)
        shader_number = material_info[11]
        shader_edge_number = material_info[12]
        triangle_count = material_info[13] / 3
        texture_filename = material_info[14]
        
        model.append_material(Material(ambient, diffuse, specular, shininess, 
                                       shader_number, shader_edge_number, 
                                       triangle_count, texture_filename))
    
    from bone import Bone
        
    for i in xrange(len(raw["Bone"])):
        bone_info = raw["Bone"][i]
        
        name = bone_info[0]
        parent_index = bone_info[1]
        tip_index = bone_info[2]        
        bone_type = bone_info[3]
        ik_bone_index = bone_info[4]
        position = Point3(bone_info[5],
                          bone_info[6],
                          bone_info[7])
        
        model.append_bone(Bone(name, bone_type, position, 
                               parent_index, tip_index, ik_bone_index))
        
    from ik_chain import IKChain
    # [ ( ik,target,p1,p2,[node]) ]
    for i in xrange(len(raw["IK"])):
        ik_chain_info = raw["IK"][i]
        
        ik_bone_index = ik_chain_info[0]
        end_effector_index = ik_chain_info[1]
        affected_bone_indices = ik_chain_info[4]
        constant_0 = ik_chain_info[2] 
        constant_1 = ik_chain_info[3]
        
        model.append_ik_chain(IKChain(ik_bone_index,
                                      end_effector_index,
                                      affected_bone_indices,
                                      constant_0, constant_1))
    
    from expression import Expression
    from vertex_modification import VertexModification    
    # [ ( name,type,[vertex]) ]    
    for i in xrange(len(raw["Skin"])):
        skin_info = raw["Skin"][i]
        
        name = skin_info[0]
        vertices = skin_info[2]
        
        expression = Expression(name)
        for vertex in vertices:
            expression.append_vertex_modification(VertexModification(vertex[0], Vector3(vertex[1], vertex[2], vertex[3])))            
        
        if i > 0:
            model.append_expression(expression)
        else:
            model.base_expression = expression
    
    return model