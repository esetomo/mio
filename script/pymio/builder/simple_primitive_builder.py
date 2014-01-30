import copy
from pymio.vector3 import Vector3
from pymio.point3 import Point3
from pymio.uv import Uv
from pymio.transform import Transform
from pymio import transform
from pymio.int3 import Int3
from pymio.geometry.indexed_mesh import IndexedMesh
from pymio.texture.indexed_texture_mapping import IndexedTextureMapping
from pymio.material.by_polygon_material_mapping import ByPolygonMaterialMapping
from pymio.material.material_list import MaterialList
from pymio.scenegraph.simple_primitive import SimplePrimitive

class SimplePrimitiveBuilder:
    def __init__(self):
        self.current_material_index = -1
        self.current_normal_index = 0
        self.current_tex_coord_index = 0                
        self.current_transform = Transform()        
        
        self.vertices = None
        self.normals = None
        self.tex_coords = None
        
        self.xform_stack = []
        self.begun = False
        
        self.primitive = None
        self.mesh = None
        self.texture_mapping = None
        self.material_mapping = None        
        
    def new_primitive(self):
        self.mesh = IndexedMesh()
        self.texture_mapping = IndexedTextureMapping()
        self.material_mapping = ByPolygonMaterialMapping()        
        self.primitive = SimplePrimitive(self.mesh, self.texture_mapping, self.material_mapping, None)
        
        self.begun = False        
        
    def load_identity(self):
        self.current_transform = Transform()
        
    def push_matrix(self):
        self.xform_stack.append(copy.copy(self.current_transform))
    
    def pop_matrix(self):
        self.current_transform = self.xform_stack.pop()
        
    def translate(self, dx, dy, dz):
        self.current_transform *= transform.translate(dx,dy,dz)
        
    def scale(self, sx, sy, sz):
        self.current_transform *= transform.scale(sx, sy, sz)
        
    def rotate_x(self, degrees):
        self.current_transform *= transform.rotate_x(degrees)

    def rotate_y(self, degrees):
        self.current_transform *= transform.rotate_y(degrees)

    def rotate_z(self, degrees):
        self.current_transform *= transform.rotate_z(degrees)
    
    def rotate(self, degrees, ax, ay, az):
        self.current_transform *= transform.rotate(degrees, Vector3(ax,ay,az))
        
    def set_material(self, material):
        self.current_material_index = material
        
    def add_vertex(self, x, y, z):
        vertex = self.current_transform.m * Point3(x,y,z)
        self.mesh.append_vertex(vertex)
        if self.begun:
            self.add_vertex_index(self.mesh.get_vertex_count()-1)
            
    def add_vertex_index(self, index):
        self.vertices.append(index)
        self.normals.append(self.current_normal_index)
        self.tex_coords.append(self.current_tex_coord_index)                
    
    def add_normal(self, x, y, z):
        normal = self.current_transform.mit * Vector3(x,y,z)
        self.mesh.append_normal(normal)             
        self.current_normal_index = self.mesh.get_normal_count()-1 
        
    def set_normal_index(self, index):
        self.current_normal_index = index
        
    def add_tex_coord(self, u, v):
        self.texture_mapping.append_tex_coord(Uv(u,v))        
        self.current_tex_coord_index = self.texture_mapping.get_tex_coord_count()-1
        
    def set_tex_coord_index(self, index):
        self.current_tex_coord_index = index
                
    def set_no_tex_coord(self):
        self.current_tex_coord_index = -1
        
    def begin_polygon(self):
        self.begun = True
        self.vertices = []
        self.tex_coords = []
        self.normals = []
        
    def end_polygon(self):
        self.mesh.append_new_polygon()
        for i in xrange(len(self.vertices)):
            self.mesh.append_vertex_normal_index_to_last_polygon(self.vertices[i],
                                                                 self.normals[i])
        self.texture_mapping.append_new_polgon()
        for i in xrange(len(self.tex_coords)):
            self.texture_mapping.append_tex_coord_index_to_last_polygon(self.tex_coords[i])
            
        self.material_mapping.append_mapping(self.current_material_index)        
        
        self.begun = False
                    
    def get_primitive(self):
        return self.primitive