from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import sys

class GlutWindow:
    def __init__(self, window_title, window_width=512, window_height=512):
        self.window_title = window_title
        self.window_width = window_width
        self.window_height = window_height
        
    def reshape(self, width, height):
        pass
    
    def idle(self):
        pass
    
    def mouse(self, button, state, x, y):
        pass
    
    def motion(self, x, y):
        pass
    
    def keyboard(self, key, x, y):
        pass
    
    def special(self, key, x, y):
        pass
    
    def draw(self):
        pass
        
    def display(self):
        glClearColor(0,0,0,0)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        self.draw()
        glutSwapBuffers()
    
    def initialize(self):
        pass
    
    def init_display(self):
        pass
    
    def run(self, argv):
        self.initialize()
        
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