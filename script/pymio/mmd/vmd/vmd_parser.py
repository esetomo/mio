import sys
import os
import struct
from pymio.mmd.dictionary import Dictionary
from pymio.vector3 import Vector3
from pymio.point3 import Point3
from pymio.quaternion import Quaternion
from vmd_animation import VMDAnimation
from header import Header
from bone_frame import BoneFrame
from expression_frame import ExpressionFrame
from camera_frame import CameraFrame
from light_frame import LightFrame
from pymio import quaternion

def read_null_terminated_string(raw):
    pos = raw.find("\x00")
    if pos >= 0:
        return raw[:pos]
    else:
        return raw

def read_struct(f, format):
    len = struct.calcsize(format)
    return struct.unpack(format, f.read(len))

def get_english_name(raw):
    return Dictionary.to_english(read_null_terminated_string(raw).decode('shift-jis').encode('utf-8'))

def read_header(f):
    header_string = read_null_terminated_string(f.read(26))
    padding = read_struct(f, "4b")
    if header_string == "Vocaloid Motion Data file":
        pmd_model_name_length = 10
    else:
        pmd_model_name_length = 20    
    pmd_model_name = get_english_name(f.read(pmd_model_name_length))    
    return Header(header_string, padding, pmd_model_name, pmd_model_name_length)

def read_bone_frames(f):
    bone_frames = []
    bone_frame_count = read_struct(f, "i")[0]
    
    for iii in xrange(bone_frame_count):        
        bone_name = get_english_name(f.read(15))
        raw = read_struct(f, "i3f4f")
        frame_number = raw[0] 
        position = Vector3(raw[1], raw[2], raw[3])
        orientation =  Quaternion(raw[4], raw[5], raw[6], raw[7])
        padding = f.read(64)        
        bone_frames.append(BoneFrame(bone_name, frame_number, position, orientation, padding))
        
    def compare_name_and_frame_number(x, y):
        if x.bone_name < y.bone_name:
            return -1
        elif x.bone_name > y.bone_name:
            return 1
        else:
            return x.frame_number - y.frame_number        
    bone_frames = sorted(bone_frames, compare_name_and_frame_number)    
    
    return bone_frames

def read_expression_frames(f):
    expression_frames = []
    expression_frame_count = read_struct(f, "i")[0]
    
    for iii in xrange(expression_frame_count):
        expression_name = get_english_name(f.read(15))
        raw = read_struct(f, "if")
        frame_number = raw[0]
        weight = raw[1]
        expression_frame = ExpressionFrame(expression_name, frame_number, weight)
        expression_frames.append(expression_frame)
        
    def compare_name_and_frame_number(x, y):
        if x.expression_name < y.expression_name:
            return -1
        elif x.expression_name > y.expression_name:
            return 1
        else:
            return x.frame_number - y.frame_number        
    expression_frames = sorted(expression_frames, compare_name_and_frame_number)
    
    return expression_frames

def read_camera_frames(f):
    camera_frames = []
    camera_frame_count = read_struct(f, "i")[0]    
    
    for iii in xrange(camera_frame_count):        
        raw = read_struct(f, "if3f3f")
        frame_number = raw[0]
        distance = raw[1]
        at = Point3(raw[2], raw[3], raw[4])
        x_rotate = raw[5]
        y_rotate = raw[6]
        z_rotate = raw[7]
        padding = ""        
        #padding = f.read(29)
        camera_frame = CameraFrame(frame_number, distance, at,
                                   x_rotate, y_rotate, z_rotate,
                                   padding)
        camera_frames.append(camera_frame)
        
    def compare_frame_number(x, y):
        return x.frame_number - y.frame_number
    camera_frames = sorted(camera_frames, compare_frame_number)
    
    return camera_frames

def read_light_frames(f):
    light_frames = []
    light_frame_count = read_struct(f, "i")[0]
    
    for iii in xrange(light_frame_count):
        raw = read_struct(f, "i3f3f")
        frame_number = raw[0]
        position = Vector3(raw[1], raw[2], raw[3])
        direction = Vector3(raw[4], raw[5], raw[6])
        light_frame = LightFrame(frame_number, position, direction)
        light_frames.append(light_frame)
    
    return light_frames

def parse(file_or_filename):
    """
    Read VMD data from the given file and return the VMDAnimation corresponding to it.    
    """
    if type(file_or_filename) == str:
        f = open(file_or_filename, "rb")
        directory = os.path.abspath(os.path.dirname(file_or_filename))
    else:
        f = file_or_filename
        directory =  os.getcwd()
        
    header = read_header(f)
    bone_frames = read_bone_frames(f)
    expression_frames = read_expression_frames(f)
    camera_frames = []
    light_frames = []
    #camera_frames = read_camera_frames(f)
    #light_frames = read_light_frames(f)
        
    vmd_animation = VMDAnimation(header, 
                                 bone_frames, 
                                 expression_frames, 
                                 camera_frames, 
                                 light_frames)    
        
    if type(file_or_filename) == str:
        f.close()
    
    return vmd_animation