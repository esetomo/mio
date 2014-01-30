# Content copied from pmd.py
# By BouKiCHi

import sys
import os
import csv
from struct import *
from pymio.mmd.dictionary import Dictionary
from pymio.mmd.pmd.pmd_model import make_pmd_model

def StringWithNull(string):
    pos = string.find("\x00")
    if pos >= 0:
        return string[:pos]
    else:
        return string

def ReadStruct(f,format):
    len = calcsize(format)
    
    return unpack(format,f.read(len))
    
def ReadStructDebug(f,format):
    len = calcsize(format)
    
    print "len = %d" % len
    
    return unpack(format,f.read(len))

def DispFilePos(f):
    print "pos = %x" % f.tell()

#
# t_header = (magic,version,model_name,comment)
#
def ReadHeader(f,d_pmd):
    hdr_string = ReadStruct(f,"3s")[0]
    hdr_version = ReadStruct(f,"f")[0]
    
    model_name = StringWithNull(ReadStruct(f,"20s")[0])
    comment = StringWithNull(ReadStruct(f,"256s")[0])
    
    d_pmd["Header"] = (hdr_string , hdr_version , model_name, comment)
    
#
# l_vert = [ ( x,y,z,nx,ny,nz,u,v) ]
# l_weight = [ ( bone1,bone2,weight,flag ) ]
#
def ReadVertex(f,d_pmd):
    l_vert   = []
    l_weight = []
    vert_size = ReadStruct(f,"L")[0]
    #print "vert = %d" % vert_size
    
    for i in range(vert_size):
        t_vert = ReadStruct(f,"8f2H2B")
        l_vert.append(t_vert[0:8])
        l_weight.append(t_vert[8:])
    
    d_pmd["Vertex"] = l_vert
    d_pmd["Weight"] = l_weight
    
#
# l_poly = [ vertex number ] 
#
def ReadPoly(f,d_pmd):
    l_poly = []
    poly_size = ReadStruct(f,"L")[0]
    #print "poly = %d" % poly_size
    
    for i in range(poly_size):
        poly = ReadStruct(f,"H")[0]
        l_poly.append(poly)
    
    d_pmd["Poly"] = l_poly

#
# [ ( r,g,b,a, shininess, specular(r,g,b), ambient(r,g,b) , toon, toon_edge , edges, file ) ]
#    
def ReadMaterial(f,d_pmd):
    list = []
    size = ReadStruct(f,"L")[0]
    #print "material = %d" % size
    
    for i in range(size):
        t_data = ReadStruct(f,"4ff3f3f") 
        t_toon_tex  = ReadStruct(f,"B")
        t_toon_edge  = ReadStruct(f,"B")
        t_edge = ReadStruct(f,"L")
        bmpfile = StringWithNull(ReadStruct(f,"20s")[0])

        t_data = t_data + t_toon_tex + t_toon_edge + t_edge + (bmpfile,) 
        list.append(t_data)
    
    d_pmd["Material"] = list

#
# [ ( name, parent , to , kind , knum, x,y,z ) ] 
#
def ReadBone(f,d_pmd):
    list = []
    size = ReadStruct(f,"H")[0]
    #print "Bone = %d" % size
    
    for i in range(size):
        name = StringWithNull(ReadStruct(f,"20s")[0]).decode('shift_jis').encode('UTF-8')
        t_data = ReadStruct(f,"HH")
        t_kind = ReadStruct(f,"B")
        t_knum = ReadStruct(f,"H")
        t_pos  = ReadStruct(f,"3f")
        
        t_data = t_data + t_kind + t_knum + t_pos
        
        list.append((name,) + t_data)
        #print (name,) + t_data
    
    d_pmd["Bone"] = list


#
# [ ( ik,target,p1,p2,[node]) ]
#
def ReadIK(f,d_pmd):
    list = []
    size = ReadStruct(f,"H")[0]
    #print "IK = %d" % size
    
    for i in range(size):
        t_parm1 = ReadStruct(f,"2H")
        t_parm2 = ReadStruct(f,"B")
        t_parm3 = ReadStruct(f,"H")
        t_parm4 = ReadStruct(f,"f")

        l_bone = []
        for j in range(t_parm2[0]):
            node = ReadStruct(f,"H")[0]
            l_bone.append(node)

        list.append(t_parm1 + t_parm3 + t_parm4 + (l_bone,))
    
    d_pmd["IK"] = list

#
# [ ( name,type,[vertex]) ]
#
def ReadSkin(f,d_pmd):
    list = []

    size = ReadStruct(f,"H")[0]
    #print "Skin = %d" % size
    
    for i in range(size):
        name = StringWithNull(ReadStruct(f,"20s")[0]).decode('shift_jis').encode('UTF-8')        
        skin_size = ReadStruct(f,"L")[0]
        t_parm = ReadStruct(f,"B")
        
        l_vert = []
        for j in range(skin_size):
            t_vdata = ReadStruct(f,"L3f")
            l_vert.append(t_vdata)
            
        list.append((name,) + t_parm + (l_vert,))
    
    d_pmd["Skin"] = list

#
# [ ( name,type,[vertex]) ]
#
def ReadSkinIndex(f,d_pmd):
    list = []

    size = ReadStruct(f,"B")[0]
    #print "SkinIndex = %d" % size
    
    for i in range(size):
        skin_index = ReadStruct(f,"H")[0]
        list.append(skin_index)
    
    d_pmd["SkinIndex"] = list

#
# [ name ]
#
def ReadDispName(f,d_pmd):
    list = []

    size = ReadStruct(f,"B")[0]
    #print "DispName = %d" % size
    
    for i in range(size):
        name = StringWithNull(ReadStruct(f,"50s")[0])
        list.append(name)
    
    d_pmd["DispName"] = list

#
# [ ( bone , type ) ]
#
def ReadBoneIndex(f,d_pmd):
    list = []

    size = ReadStruct(f,"L")[0]
    #print "BoneIndex = %d" % size
    
    for i in range(size):

        bone = ReadStruct(f,"H")[0]
        type = ReadStruct(f,"B")[0]
        
        list.append((bone,type))
    
    d_pmd["BoneIndex"] = list

def EncString(string):
    enc = sys.getfilesystemencoding()
    
    return unicode(string,"shift-jis").encode(enc,"replace")

def DispBone(list):
    for bone in list:    
        print "Name = ",EncString(bone[0])
        print "Parent = %d" % bone[1]
        print "To = %d" % bone[2]
        print "kind = %d" % bone[3]
        print "knum = %d" % bone[4]
        print "Pos = (%f,%f,%f)" % bone[5:8]
        print

def DispMaterial(list):
    for material in list:
        print "RGBA = (%f,%f,%f,%f)" % material[:4]
        print "shininess = %f" % material[4]
        print "specular RGB = (%f,%f,%f)" % material[5:8]
        print "ambient  RGB = (%f,%f,%f)" % material[8:11]
        print "Unknown = ",material[11]
        print "Edges = %d" % material[12]
        print "Texture = ",EncString(material[13])
        print

def DispSkin(list):
    for skin in list:
        print "Name = ",EncString(skin[0])
        print "type = %d" % skin[1]

def DispBoneIndex(list):
    for bone in list:
        print bone
        print "Bone = %d , Type = %d" % (bone[0],bone[1])

#
# Note : this method is required some newer version of python 
# (for "shift-jis" convertion)
#
def DispInfo(d_pmd):
    enc = sys.getfilesystemencoding()

    header = d_pmd["Header"]
    print "ID String =",header[0]
    print "Version =",header[1]
    print "Model = ",unicode(header[2],"shift-jis").encode(enc,"replace")
    print "Copyright =",unicode(header[3],"shift-jis").encode(enc,"replace")
    print
    #    DispMaterial(d_pmd["Material"])
    
def read(file_or_filename):
    """
    Read PMD data from the given file and return it as a dictionary.
    
    @param file_or_filename: either a binary or a file name
    @type f: binary file or string
    """
    if type(file_or_filename) == str:
        f = open(file_or_filename, "rb")
        directory = os.path.abspath(os.path.dirname(file_or_filename))
    else:
        f = file_or_filename
        directory =  os.getcwd()
        
    d_pmd = {}
    ReadHeader(f,d_pmd)
    if d_pmd["Header"][0] != "Pmd":
        f.close()
        raise RuntimeError("The file is not a valid PMD file.")

    ReadVertex(f,d_pmd)
    ReadPoly(f,d_pmd)
    ReadMaterial(f,d_pmd)
    ReadBone(f,d_pmd)
    
    ReadIK(f,d_pmd)
    ReadSkin(f,d_pmd)
    ReadSkinIndex(f,d_pmd)
    ReadDispName(f,d_pmd)
    ReadBoneIndex(f,d_pmd)    
    
    for i in xrange(len(d_pmd["Material"])):
        material = d_pmd["Material"][i]
        if len(material[-1]) > 0:
            material = material[:-1] + (os.path.abspath(os.path.join(directory, material[-1])),)
            d_pmd["Material"][i] = material
    
    # DispInfo(d_pmd)
    for i, bone in enumerate(d_pmd["Bone"]):        
        d_pmd["Bone"][i] = (Dictionary.to_english(bone[0]),) + bone[1:]             
    for i, morph in enumerate(d_pmd["Skin"]):
        d_pmd["Skin"][i] = (Dictionary.to_english(morph[0]),) + morph[1:]    
    
    if type(file_or_filename) == str:
        f.close()
    return d_pmd

def parse(file_or_filename):
    raw = read(file_or_filename)
    return make_pmd_model(raw)