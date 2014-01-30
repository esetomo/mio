from pymio.curve.polyline import Polyline_JointChange
from pymio.rigging.joint_change import JointChange
from pymio.point3 import Point3
from pymio.vector3 import Vector3
from pymio import matrix4x4
from pymio.quaternion import Quaternion
from pymio.rigging.pose import Pose
from pymio.rigging.pose_animation import PoseAnimation
from pymio.ik.ik_pose import IKPose 
from pymio.mmd.pmd.bone import Bone
from pymio.mmd.vpd.vpd_pose import VPDPose
import math

class VMDAnimation:
    def __init__(self, header, bone_frames, expression_frames, camera_frames, light_frames):
        self.header = header
        self.bone_frames = bone_frames
        self.expression_frames = expression_frames
        self.camera_frames = camera_frames
        self.light_frames = light_frames
        
    def get_bone_frame(self, bone_name, frame_number):
        for bone_frame in self.bone_frames:
            if bone_frame.bone_name == bone_name and bone_frame.frame_number == frame_number:
                return bone_frame
        raise RuntimeError() 
        
    def get_bone_curves(self):
        curves = {}
        for bone_frame in self.bone_frames:
            if not curves.has_key(bone_frame.bone_name):
                curves[bone_frame.bone_name] = Polyline_JointChange()
                
            position = bone_frame.position
            orientation = bone_frame.orientation                    
            curves[bone_frame.bone_name].set_control_point(float(bone_frame.frame_number),
                                                           JointChange(position, orientation))
        return curves
    
    def update_ik_pose(self, curves, pmd_model, time):
        vpd_pose = VPDPose()
        for bone_name, curve in curves.items():            
            #if pmd_model.bones_by_name.has_key(bone_name):
            joint_change = curve.evaluate(time)
            vpd_pose.set_joint_change(bone_name, joint_change)
        return vpd_pose.get_ik_pose(pmd_model)
    
    def get_vpd_pose(self, curves, pmd_model, time):
        vpd_pose = VPDPose()
        for bone_name, curve in curves.items():            
            joint_change = curve.evaluate(time)
            vpd_pose.set_joint_change(bone_name, joint_change)
        return vpd_pose
    
    def get_frame_range(self):
        first_frame = 100000000
        last_frame = -100000000
        
        for bone_frame in self.bone_frames:
            if bone_frame.frame_number < first_frame:
                first_frame = bone_frame.frame_number
            if bone_frame.frame_number > last_frame:
                last_frame = bone_frame.frame_number
        
        return first_frame, last_frame
    
    def get_ik_less_pose_animation(self, pmd_model, progress_hook=None):
        curves = self.get_bone_curves()
        ik_armature = pmd_model.get_ik_armature()
        ik_pose = IKPose()
        pose = Pose()
        
        iterators = {}
        for bone_name, curve in curves.items():
            iterators[bone_name] = curve.get_control_point_iterator()
        
        frames = {}
        for bone_name, iterator in iterators.items():
            if iterator.has_next():
                frames[bone_name] = iterator.get_next()
                
        ik_joint_curves = {}
        for ik_joint in ik_armature.get_ik_joints():
            ik_joint_curves[ik_joint.get_name()] = Polyline_JointChange()
                    
        while len(frames) > 0:
            earliest_time = 1e20
            for bone_name, frame in frames.items():
                if frame.time < earliest_time:
                    earliest_time = frame.time            
                        
            vpd_pose = self.get_vpd_pose(curves, pmd_model, earliest_time)
            pose = pmd_model.get_ik_less_pose(vpd_pose)            
                        
            for ik_joint_name, curve in ik_joint_curves.items():
                curve.set_control_point(earliest_time, pose.get_joint_change(ik_joint_name))
                            
            bone_names_with_earliest_time = []
            for bone_name, frame in frames.items():
                if abs(frame.time - earliest_time) < 0.0001:
                    bone_names_with_earliest_time.append(bone_name)
                    
            for bone_name in bone_names_with_earliest_time:
                iterator = iterators[bone_name]
                if iterator.has_next():
                    frames[bone_name] = iterator.get_next()
                else:
                    del(frames[bone_name])
                    
            if progress_hook is not None:
                continuing = progress_hook(earliest_time)
                if not continuing:
                    return None
        
        pose_animation = PoseAnimation()
        
        curves = {}
        for bone_frame in self.bone_frames:            
            if not curves.has_key(bone_frame.bone_name):
                curves[bone_frame.bone_name] = Polyline_JointChange()
            position = Vector3(bone_frame.position.x, bone_frame.position.y, -bone_frame.position.z)
            z_flip_matrix = matrix4x4.scale(1,1,-1)            
            orientation = Quaternion(bone_frame.orientation.x,
                                     bone_frame.orientation.y,
                                     -bone_frame.orientation.z,
                                     -bone_frame.orientation.w)            
            curves[bone_frame.bone_name].set_control_point(float(bone_frame.frame_number),
                                                           JointChange(position, orientation))            
        for bone_name, curve in curves.items():
            if pmd_model.bones_by_name.has_key(bone_name):                
                bone = pmd_model.get_bone_by_name(bone_name)
                if bone.bone_type != Bone.BONE_IK:
                    pose_animation.set_joint_curve(bone_name, curve)                
        for bone_name, ik_joint_curve in ik_joint_curves.items():
            pose_animation.set_joint_curve(bone_name, ik_joint_curve)
        for bone in pmd_model.bones:
            if bone.bone_type == Bone.BONE_ROTATION_INFLUENCED:                
                source_bone = pmd_model.bones[bone.ik_bone_index]
                bone_curve = Polyline_JointChange()
                if curves.has_key(source_bone.name):
                    source_curve = curves[source_bone.name]                    
                    for control_point in source_curve.get_control_points():
                        time = control_point.time
                        jc = control_point.value
                        bone_curve.set_control_point(time, JointChange(Vector3(0,0,0), jc.orientation))
                pose_animation.set_joint_curve(bone.name, bone_curve)                    
        pose_animation.update_range()
        return pose_animation