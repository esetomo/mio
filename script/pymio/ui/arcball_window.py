from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
from arcball_camera import ArcballCamera
from pymio.vector3 import Vector3
from pymio.point3 import Point3
from glut_window import GlutWindow
import math
from exceptions import *
import sys

class ArcballWindow(GlutWindow):
    def __init__(self, window_title, window_width=512, window_height=512, axis_length=5.0):
        GlutWindow.__init__(self, window_title, window_width, window_height)        
        self.camera = ArcballCamera()
        self.display_axis = False
        self.axis_length = 5.0
    
    def init_camera(self):
        raise NotImplementedError()
    
    def mouse(self, button, state, x, y):
        if state == GLUT_DOWN:
            if button == GLUT_LEFT_BUTTON:
                self.camera.mouse_click(ArcballCamera.ROTATE, x, y)
            elif button == GLUT_MIDDLE_BUTTON:
                self.camera.mouse_click(ArcballCamera.TRANSLATE, x, y)
            elif button == GLUT_RIGHT_BUTTON:
                self.camera.mouse_click(ArcballCamera.ZOOM, x, y)
            self.display_axis = True            
        else:
            self.camera.mouse_release(x, y)
            self.display_axis = False
            glutPostRedisplay()
    
    def motion(self, x, y):
        self.camera.mouse_drag(x,y)
        glutPostRedisplay()
        
    def reshape(self, width, height):
        self.camera.set_viewport(0,0,width,height)
        
    def display(self):
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT)

        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        self.camera.apply_viewport()
        self.camera.apply_perspective()  

        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        self.camera.apply_modelview()

        self.draw()
        
        if self.display_axis:
        # This draws the coordinate axes when you're rotating, to
            glPushMatrix()
            center = self.camera.get_center()
            glTranslated(center[0], center[1], center[2])
            
            # Save current state of OpenGL
            glPushAttrib(GL_ALL_ATTRIB_BITS)
    
            # This is to draw the axes when the mouse button is down
            glDisable(GL_LIGHTING)
            glLineWidth(3)
            glPushMatrix()
            glScaled(self.axis_length,self.axis_length,self.axis_length)
            glBegin(GL_LINES)
            glColor4f(1,0.5,0.5,1)
            glVertex3d(0,0,0)
            glVertex3d(1,0,0)
            glColor4f(0.5,1,0.5,1)
            glVertex3d(0,0,0)
            glVertex3d(0,1,0)
            glColor4f(0.5,0.5,1,1)
            glVertex3d(0,0,0)
            glVertex3d(0,0,1)
    
            glColor4f(0.5,0.5,0.5,1)
            glVertex3d(0,0,0)
            glVertex3d(-1,0,0)
            glVertex3d(0,0,0)
            glVertex3d(0,-1,0)
            glVertex3d(0,0,0)
            glVertex3d(0,0,-1)
    
            glEnd()
            glPopMatrix()        
            glPopAttrib()
            glPopMatrix()

        glutSwapBuffers()
    
    def run(self, argv):
        self.initialize()
        self.init_camera()
        
        glutInit(sys.argv)
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH)
        glutInitWindowSize(self.window_width, self.window_height)
        glutCreateWindow(self.window_title)
    
        glutDisplayFunc(self.display)
        glutIdleFunc(self.idle)
        glutReshapeFunc(self.reshape)
        glutMouseFunc(self.mouse)
        glutMotionFunc(self.motion)
        glutKeyboardFunc(self.keyboard)
        glutSpecialFunc(self.special)
        
        self.init_display()
    
        glutMainLoop()