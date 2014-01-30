from pymio.vector3 import Vector3
from pymio.point3 import Point3
from pymio.quaternion import Quaternion
from pymio.mmd.dictionary import Dictionary
from pymio.rigging.joint_change import JointChange
from pymio.ik.ik_pose import IKPose
from vpd_pose import VPDPose
import os

def parse(file_or_filename):
    if type(file_or_filename) == str:
        f = open(file_or_filename, "rb")        
    else:
        f = file_or_filename
        
    vpd_pose = VPDPose()
    
    # ignore the first two lines
    f.readline()
    f.readline()
    
    # read the model name
    model_name_line = f.readline()
    model_file = model_name_line.split()[0]
    model_name = Dictionary.to_english(model_file[:-5].decode('shift_jis').encode('UTF-8'))
    
    # read the number of bones
    bone_count_line = f.readline()
    bone_count = int(bone_count_line.split()[0][:-1])
    
    # read a blank line
    f.readline()
    
    # read the bones, one by one
    for bbb in xrange(bone_count):
        bone_name_line = f.readline()
        position_line = f.readline()
        orientation_line = f.readline()
        
        bone_name = Dictionary.to_english(bone_name_line.split('{')[1].strip().decode('shift_jis').encode('UTF-8')).strip()
        
        xyz = position_line.split()[0][:-1].split(",")
        position = Vector3(float(xyz[0]), float(xyz[1]), float(xyz[2]))
        
        xyzw = orientation_line.split()[0][:-1].split(",")
        orientation = Quaternion(float(xyzw[0]), float(xyzw[1]), float(xyzw[2]), float(xyzw[3]))
        
        vpd_pose.set_joint_change(bone_name, JointChange(position, orientation))
        
        f.readline()
        f.readline()
        
    if type(file_or_filename) == str:
        f.close()
    return vpd_pose