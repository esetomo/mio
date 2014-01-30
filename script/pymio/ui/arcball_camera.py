from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
from pymio import vector3
from pymio.vector3 import Vector3
from pymio.point3 import Point3
from pymio import matrix4x4
from pymio.matrix4x4 import Matrix4x4
import copy
import math

class ArcballCamera:
    NONE = 0
    ROTATE = 1
    TRANSLATE = 2
    ZOOM = 3
    
    def __init__(self):
        self.dimensions = (0,0)
        self.start_click_point = (0,0)
        self.button_state = ArcballCamera.NONE
        
        self.start_rotation = Matrix4x4()
        self.current_rotation = Matrix4x4()
        
        self.perspective = (0,0)
        self.viewport = (0,0,0,0)
        self.start_center = Vector3(0,0,0)
        self.current_center = Vector3(0,0,0)
        
        self.start_distance = 0
        self.current_distance = 0
        
    def get_center(self):
        return self.current_center
    
    def get_rotation(self):
        return self.current_rotation
    
    def get_distance(self):
        return self.current_distance
    
    # You must call all of the Set*() functions before you use this!
    # I didn't put it into the constructor because it's inconvenient
    # to initialize stuff in my opengl application.
    
    def set_dimension(self, w, h):
        self.dimensions = (w,h)
        
    def set_perspective(self, fovy):
        self.perspective = (fovy, self.perspective[1])
        
    def set_viewport(self, x, y, w, h):
        self.viewport = (x,y,w,h)
        self.set_dimension(w,h)
        if h == 0:
            h = 1
        if w == 0:
            w = 1
        aspect = w * 1.0 / h
        self.perspective = (self.perspective[0], aspect)
        
    def set_center(self, center):
        self.start_center = copy.copy(center)
        self.current_center = copy.copy(center)        
    
    def set_rotation(self, rotation):
        self.start_rotation = copy.copy(rotation)
        self.current_rotation = copy.copy(rotation)
        
    def set_distance(self, distance):
        self.start_distance = self.current_distance = distance
    
    def mouse_click(self, button, x, y):
        self.start_click_point = (x,y)
        self.button_state = button        
        
        if button == ArcballCamera.ROTATE:
            self.current_rotation = copy.copy(self.start_rotation)
        elif button == ArcballCamera.TRANSLATE:
            self.current_center = copy.copy(self.start_center)
        elif button == ArcballCamera.ZOOM:
            self.current_distance = self.start_distance
    
    def mouse_drag(self, x, y):        
        if self.button_state == ArcballCamera.ROTATE:
            self.arcball_rotation(x,y)
        elif self.button_state == ArcballCamera.TRANSLATE:
            self.plane_translation(x,y)
        elif self.button_state == ArcballCamera.ZOOM:
            self.distance_zoom(x,y)
            
    def mouse_release(self, x, y):
        self.start_rotation = copy.copy(self.current_rotation)
        self.start_center = copy.copy(self.current_center)
        self.start_distance = self.current_distance
        self.button_state = ArcballCamera.NONE        
        
    def arcball_rotation(self, x, y):        
        # find vectors from center of window
        sx = self.start_click_point[0] - (self.dimensions[0]  / 2)
        sy = self.start_click_point[1] - (self.dimensions[1] / 2)
        ex = x - (self.dimensions[0] / 2)
        ey = y - (self.dimensions[1] / 2)
        
        # invert y coordinates (raster versus device coordinates)
        sy = -sy
        ey = -ey
        
        # scale by inverse of size of window and magical sqrt2 factor
        if self.dimensions[0] > self.dimensions[1]:
            scale = 1.0 * self.dimensions[1]
        else:
            scale = 1.0 * self.dimensions[0]
            
        scale = 1.0/scale
        
        sx *= scale
        sy *= scale
        ex *= scale
        ey *= scale
        
        # project points to unit circle
        sl = math.hypot(sx, sy)
        el = math.hypot(ex, ey)
    
        if sl > 1.0:
            sx /= sl
            sy /= sl
            sl = 1.0
        if el > 1.0:
            ex /= el
            ey /= el
            el = 1.0
            
        # project up to unit sphere - find Z coordinate
        sz = math.sqrt(1.0 - sl * sl)
        ez = math.sqrt(1.0 - el * el)
        
        # rotate (sx,sy,sz) into (ex,ey,ez)
    
        # compute angle from dot-product of unit vectors (and double
        # it).  compute axis from cross product.
        dotprod = sx * ex + sy * ey + sz * ez;

        if dotprod <= 1 and dotprod >= -1:
            axis = vector3.normalize(Vector3(sy * ez - ey * sz, 
                                             sz * ex - ez * sx, 
                                             sx * ey - ex * sy))
            rotation = matrix4x4.rotate(2.0 * 180.0 * math.acos(dotprod) / math.pi, axis)            
            self.current_rotation = rotation * self.start_rotation
        else:
            self.current_rotation = copy.copy(self.start_rotation)
            
    def plane_translation(self, x, y):
        # map window x,y into viewport x,y
    
        # start
        sx = self.start_click_point[0] - self.viewport[0]
        sy = self.start_click_point[1] - self.viewport[1]
    
        # current
        cx = x - self.viewport[0]
        cy = y - self.viewport[1]
    
    
        # compute "distance" of image plane (wrt projection matrix)
        d = self.viewport[3] / 2.0 / math.tan(self.perspective[0]*math.pi / 180.0 / 2.0)
        
        # compute up plane intersect of clickpoint (wrt fovy)
        su = -sy + self.viewport[3]/2.0
        cu = -cy + self.viewport[3]/2.0
    
        # compute right plane intersect of clickpoint (ASSUMED FOVY is 1)
        sr = (sx - self.viewport[2]/2.0)
        cr = (cx - self.viewport[2]/2.0)
    
        move = Vector3(cr-sr, cu-su, 0)        
    
        # this maps move
        move = move * (-self.current_distance/d)
            
        self.current_center = (self.start_center +
            Vector3(self.current_rotation[0,0],
                    self.current_rotation[1,0],
                    self.current_rotation[2,0]) * move.x +
            Vector3(self.current_rotation[0,1],
                    self.current_rotation[1,1],
                    self.current_rotation[2,1]) * move.y)        
        
    def apply_viewport(self):    
        glViewport(self.viewport[0],
                   self.viewport[1],
                   self.viewport[2],
                   self.viewport[3])
    
    def apply_perspective(self):        
        gluPerspective(self.perspective[0],
                       self.perspective[1], 
                       1.0, 
                       1000.0)
    
    def apply_modelview(self):
        # back up distance
        #print self.current_distance
        gluLookAt(0,0,self.current_distance,
                  0,0,0,
                  0.0, 1.0, 0.0)
    
        # rotate object
        #print self.current_rotation.to_opengl()
        matrix = []
        for i in xrange(4):
            for j in xrange(4):
                matrix.append(self.current_rotation[j,i])
        glMultMatrixd(matrix)
    
        #translate object to center
        glTranslated(-self.current_center.x,
                     -self.current_center.y,
                     -self.current_center.z);
    
    def distance_zoom(self, x,y):
        sy = self.start_click_point[1] - self.viewport[1]
        cy = y - self.viewport[1]
    
        delta = (cy - sy) * 1.0 / self.viewport[3]        
    
        # exponential zoom factor
        self.current_distance = self.start_distance * math.exp(delta)