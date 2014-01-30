from pymio.vector3 import Vector3
from pymio.point3 import Point3
from pymio.quaternion import Quaternion
from pymio import matrix4x4
from pymio.mmd.dictionary import Dictionary
from pymio.mmd.pmd.bone import Bone
from pymio.rigging.pose import Pose
from pymio.rigging.joint_change import JointChange
from pymio.ik.ik_pose import IKPose

class VPDPose:
    def __init__(self):
        self.model_name = None
        self.joint_change_by_bone_name = {}
        
    def set_joint_change(self, joint_name, joint_change):
        self.joint_change_by_bone_name[joint_name] = joint_change
        
    def get_right_handed_joint_change(self, bone_name):
        joint_change = self.joint_change_by_bone_name[bone_name]
        z_flip_matrix = matrix4x4.scale(1,1,-1)        
        orientation = Quaternion(joint_change.orientation.x,
                                 joint_change.orientation.y,
                                 -joint_change.orientation.z,
                                 -joint_change.orientation.w,)  
        joint_change = JointChange(z_flip_matrix * joint_change.position, orientation)
        return joint_change
    
    def get_left_handed_joint_change(self, bone_name):
        joint_change = self.joint_change_by_bone_name[bone_name]
        return joint_change
                     
    def get_ik_pose(self, pmd_model):        
        ik_pose = IKPose()
        normal_pose = ik_pose.get_normal_pose()
        end_effector_goals = ik_pose.get_end_effector_goals()
        
        assignable_bone_types = [Bone.BONE_ROTATE, 
                                 Bone.BONE_ROTATE_TRANSLATE,                                  
                                 Bone.BONE_IK_ROTATION_INFLUENCED,
                                 Bone.BONE_IK_TARGET]
        
        for bone_index, bone in enumerate(pmd_model.bones):
            if bone.bone_type in assignable_bone_types:
                try:
                    joint_change = self.get_right_handed_joint_change(bone.name)
                except KeyError:
                    continue
                normal_pose.set_joint_change(bone.name, joint_change)                
           
            elif bone.bone_type == Bone.BONE_ROTATION_INFLUENCED:
                source_bone = pmd_model.bones[bone.ik_bone_index]
                try:
                    joint_change = self.get_right_handed_joint_change(source_bone.name)
                except KeyError:
                    continue                                                                                
                normal_pose.set_joint_change(bone.name, joint_change)
            
            elif bone.bone_type == Bone.BONE_IK:            
                ik_chain = pmd_model.get_ik_chain_by_ik_bone_index(bone_index)
                end_effector_bone = pmd_model.bones[ik_chain.end_effector_index]
                                                
                new_position = pmd_model.get_morphed_world_position(bone_index, self)                                      
                end_effector_goals.set_end_effector_position(end_effector_bone.name, new_position)
            
        return ik_pose