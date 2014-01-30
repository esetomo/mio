from pymio.pretty_printer import PrettyPrinter
from pymio.rigging.pose import Pose
from pymio.mmd.dictionary import Dictionary
import math

def make_armature_tree(armature):
    class Node:
        def __init__(self, name):
            self.name = name
            self.parent = None
            self.children = []
    
    nodes = {}
    for joint in armature.get_joints():
        nodes[joint.get_name()] = Node(joint.get_name())    
    for node_name, node in nodes.items():
        joint = armature.get_joint(node_name)        
        if joint.parent_index >= 0:
            parent = armature.get_joint(joint.parent_index)            
            parent_node = nodes[parent.get_name()]
            parent_node.children.append(node)
            node.parent = parent_node    
    return nodes["root"]

def traverse_armature_tree(node, armature, pp, node_order, scaling_factor, language_to_save_in):
    node_order.append(node.name)
    joint = armature.get_joint(node.name)
    
    if node.parent is None:
        pp.append_indented("ROOT ")
    else:
        pp.append_indented("JOINT ")
    if language_to_save_in == "english":
        pp.append(node.name)
    else:
        pp.append(Dictionary.to_japanese(node.name).decode('UTF-8').encode('shift-jis'))     
    pp.newline()
    pp.append_indented_line("{")
    
    pp.indent()
    
    pp.append_indented("OFFSET ")
    pp.append("%f %f %f" % (joint.position.x * scaling_factor, 
                            joint.position.y * scaling_factor, 
                            joint.position.z * scaling_factor))
    pp.newline()
    
    if node.parent is None:
        pp.append_indented_line("CHANNELS 6 Xposition Yposition Zposition Zrotation Xrotation Yrotation")
    else:
        pp.append_indented_line("CHANNELS 3 Zrotation Xrotation Yrotation")
        
    if len(node.children) == 0:
        pp.append_indented_line("End Site")
        pp.append_indented_line("{")
        pp.indent()
        pp.append_indented_line("OFFSET 0.00 0.00 0.00")
        pp.outdent()
        pp.append_indented_line("}")
    else:
        for child in node.children:
            traverse_armature_tree(child, armature, pp, node_order, scaling_factor, language_to_save_in)
        
    pp.outdent()
    
    pp.append_indented_line("}")
    
def get_yaw_pitch_roll(quaternion):
    matrix = quaternion.get_matrix()
    if abs(matrix[0,1]) < 0.0001 and abs(matrix[1,1]) < 0.0001:
        if matrix[2,1] < 0:
            pitch = -math.pi / 2.0
        else:
            pitch = math.pi / 2.0
        roll = 0.0
        yaw = math.atan2(matrix[0,2], matrix[0,0])                
    else:
        yaw = math.atan2(-matrix[2,0], matrix[2,2])
        roll = math.atan2(-matrix[0,1], matrix[1,1])
        pitch = math.atan2(matrix[2,1], math.sqrt(matrix[2,0]**2 + matrix[2,2]**2))
    yaw = yaw * 180 / math.pi
    pitch = pitch * 180 / math.pi
    roll = roll * 180 / math.pi
    return yaw, pitch, roll                

def export(armature, pose_animation, language_to_save_in="english", frame_per_sec=30, scaling_factor=1):
    armature_tree_root = make_armature_tree(armature)
        
    pp = PrettyPrinter()
    pp.append_indented_line("HIERARCHY")
    
    node_order = []
    traverse_armature_tree(armature_tree_root, armature, pp, node_order, scaling_factor, language_to_save_in)
    
    pp.append_indented_line("MOTION")
    first_frame = int(pose_animation.get_range_lower_bound())
    last_frame = int(pose_animation.get_range_upper_bound())
    frame_count = last_frame - first_frame + 1
    pp.append_indented_line("Frames: %d" % frame_count)
    pp.append_indented_line("Frame Time: %f" % (1.0 / frame_per_sec))
    
    pose = Pose()
    root_displacement = armature.get_joint(armature_tree_root.name).position
    for time in xrange(first_frame, last_frame+1):
        pose_animation.get_pose(pose, float(time), True)
        
        pp.append_indentation()
        
        for joint_name in node_order:
            joint_change = pose.get_joint_change(joint_name)
            position = joint_change.position
            orientation = joint_change.orientation
                        
            if joint_name == armature_tree_root.name:
                position = (position + root_displacement) * scaling_factor 
                pp.append("%f %f %f " % (position.x, position.y, position.z))
    
            yaw, pitch, roll = get_yaw_pitch_roll(orientation)
            pp.append("%f %f %f " % (roll, pitch, yaw))
                    
        pp.newline()
        
    return pp.get_output()