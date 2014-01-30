import sys
import math
import time

from PyQt4 import QtCore, QtGui, QtOpenGL
from pymio.ui.arcball_camera import ArcballCamera
from pymio import matrix4x4
from pymio.point3 import Point3
from pymio.gl.gl_renderer import GLRenderer
from pymio.rgb import Rgb

from pymio.mmd.pmd import pmd_parser
from pymio.mmd.vmd import vmd_parser
from pymio.scenegraph.simple_primitive import SimplePrimitive
from pymio.rigging.pose import Pose

from pymio.exporter import bvh_exporter

try:
    from OpenGL.GL import *
    from OpenGL.GLU import *
    from OpenGL.GLUT import *
except ImportError:
    app = QtGui.QApplication(sys.argv)
    QtGui.QMessageBox.critical(None, "VMD Viewer",
                               "PyOpenGL must be installed to run this program.",
                               QtGui.QMessageBox.Ok | QtGui.QMessageBox.Default,
                               QtGui.QMessageBox.NoButton)
    sys.exit(1)
    
class MainWindow(QtGui.QWidget):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)
        
        self.period = 30
        
        self.get_system_settings()
        
        self.setWindowTitle(self.tr("VMD Viewer"))
        self.resize(800, 600)        

        main_layout = QtGui.QVBoxLayout()
        self.setLayout(main_layout)
        
        layout_1 = QtGui.QHBoxLayout()
        main_layout.addLayout(layout_1)
        
        settings_layout = QtGui.QVBoxLayout()
        layout_1.addLayout(settings_layout)                    
    
        self.gl_widget = GLWidget(self)
        self.gl_widget.setSizePolicy(QtGui.QSizePolicy(
            QtGui.QSizePolicy.Expanding, 
            QtGui.QSizePolicy.Expanding))
        layout_1.addWidget(self.gl_widget)
        
        settings_layout.addWidget(self.__create_lighting_setting_group_box())
        settings_layout.addWidget(self.__create_display_setting_group_box())
        
        self.reset_camera_button = QtGui.QPushButton("Reset Camera")
        self.connect(self.reset_camera_button, QtCore.SIGNAL("clicked()"), self.reset_camera)
        settings_layout.addWidget(self.reset_camera_button)        
                                                    
        settings_layout.addStretch()
        
        mmd_data_group_box = self.__create_mmd_data_group_box()
        animation_control_frame = self.__create_animation_control_frame()
        main_layout.addWidget(animation_control_frame)
        main_layout.addWidget(mmd_data_group_box)  
                
        self.pmd_model = None
        self.vmd_animation = None
        self.primitive = None        
        self.pose_animation = None
        self.pose = Pose()
        
        self.current_frame = -1
        self.first_frame = 0
        self.last_frame = 0
        
        self.timer = QtCore.QTimer()        
        self.connect(self.timer, QtCore.SIGNAL("timeout()"),
                     self.advance_frame)        
        
    def get_system_settings(self):
        settings = QtCore.QSettings()
        
        d = os.path.dirname
        data_path = os.path.join(d(d(d(d(os.path.abspath(__file__))))), "data")
        self.last_pmd_directory = settings.value("last_pmd_directory",
                                                 QtCore.QVariant(data_path)).toString()
        self.last_vmd_directory = settings.value("last_vmd_directory",
                                                 QtCore.QVariant(data_path)).toString()
                
        self.initial_light_position = Point3(
            settings.value("light_position_x", QtCore.QVariant(0.0)).toDouble()[0],
            settings.value("light_position_y", QtCore.QVariant(10.0)).toDouble()[0],
            settings.value("light_position_z", QtCore.QVariant(20.0)).toDouble()[0])
        self.initial_ambient = Rgb(
            settings.value("ambient_r", QtCore.QVariant(1.0)).toDouble()[0],
            settings.value("ambient_g", QtCore.QVariant(1.0)).toDouble()[0],
            settings.value("ambient_b", QtCore.QVariant(1.0)).toDouble()[0])
        self.initial_diffuse = Rgb(
            settings.value("diffuse_r", QtCore.QVariant(1.0)).toDouble()[0],
            settings.value("diffuse_g", QtCore.QVariant(1.0)).toDouble()[0],
            settings.value("diffuse_b", QtCore.QVariant(1.0)).toDouble()[0])
        self.initial_specular = Rgb(
            settings.value("specular_r", QtCore.QVariant(0.0)).toDouble()[0],
            settings.value("specular_g", QtCore.QVariant(0.0)).toDouble()[0],
            settings.value("specular_b", QtCore.QVariant(0.0)).toDouble()[0])        
        self.initial_display_axes = settings.value("display_axes", 
                                                   QtCore.QVariant(True)).toBool()
        self.initial_display_plane = settings.value("display_plane", 
                                                   QtCore.QVariant(True)).toBool()
        self.initial_display_light_position = settings.value("display_light_position",
                                                             QtCore.QVariant(False)).toBool()

        self.last_save_directory = settings.value("last_save_directory",
                                                  QtCore.QVariant(data_path)).toString()
        self.language_to_save_in = settings.value("language_to_save_in",
                                                  QtCore.QVariant("english")).toString()
        self.scaling_factor = settings.value("scaling_factor",
                                             QtCore.QVariant(1.0)).toDouble()[0]        
                                                             
    def closeEvent(self, event):        
        settings = QtCore.QSettings()
        
        settings.setValue("last_pmd_directory",
                          QtCore.QVariant(self.last_pmd_directory))
        settings.setValue("last_vmd_directory",
                          QtCore.QVariant(self.last_vmd_directory))
        
        settings.setValue("light_position_x", QtCore.QVariant(self.light_position_x_spinbox.value()))
        settings.setValue("light_position_y", QtCore.QVariant(self.light_position_y_spinbox.value()))
        settings.setValue("light_position_z", QtCore.QVariant(self.light_position_z_spinbox.value()))
        
        settings.setValue("ambient_r", QtCore.QVariant(self.ambient_r_spinbox.value()))
        settings.setValue("ambient_g", QtCore.QVariant(self.ambient_g_spinbox.value()))
        settings.setValue("ambient_b", QtCore.QVariant(self.ambient_b_spinbox.value()))
        
        settings.setValue("diffuse_r", QtCore.QVariant(self.diffuse_r_spinbox.value()))
        settings.setValue("diffuse_g", QtCore.QVariant(self.diffuse_g_spinbox.value()))
        settings.setValue("diffuse_b", QtCore.QVariant(self.diffuse_b_spinbox.value()))
        
        settings.setValue("specular_r", QtCore.QVariant(self.specular_r_spinbox.value()))
        settings.setValue("specular_g", QtCore.QVariant(self.specular_g_spinbox.value()))
        settings.setValue("specular_b", QtCore.QVariant(self.specular_b_spinbox.value()))
                    
        display_axes = self.display_axes_checkbox.isChecked()
        display_plane = self.display_plane_checkbox.isChecked()        
        display_light_position = self.display_light_position_checkbox.isChecked()        
        settings.setValue("display_axes", QtCore.QVariant(display_axes))
        settings.setValue("display_plane", QtCore.QVariant(display_plane))
        settings.setValue("display_light_position", QtCore.QVariant(display_light_position))
        
        settings.setValue("last_save_directory", QtCore.QVariant(self.last_save_directory))
        settings.setValue("language_to_save_in", QtCore.QVariant(self.language_to_save_in))
        settings.setValue("scaling_factor", QtCore.QVariant(self.scaling_factor))
        
    def __create_display_setting_group_box(self):
        display_setting_group_box = QtGui.QGroupBox(self.tr(self.tr("Display Settings")))        
        display_setting_group_box_layout = QtGui.QVBoxLayout()
        display_setting_group_box.setLayout(display_setting_group_box_layout)
        
        self.display_axes_checkbox = QtGui.QCheckBox(self.tr("Display axes"))
        self.display_axes_checkbox.setChecked(self.initial_display_axes)
        self.connect(self.display_axes_checkbox, 
                     QtCore.SIGNAL("stateChanged(int)"), 
                     self.gl_widget.updateGL)
        display_setting_group_box_layout.addWidget(self.display_axes_checkbox)
        
        self.display_plane_checkbox = QtGui.QCheckBox(self.tr("Display plane"))
        self.display_plane_checkbox.setChecked(self.initial_display_plane)
        self.connect(self.display_plane_checkbox,
                     QtCore.SIGNAL("stateChanged(int)"),
                     self.gl_widget.updateGL)
        display_setting_group_box_layout.addWidget(self.display_plane_checkbox)
        
        return display_setting_group_box
        
    def __create_lighting_setting_group_box(self):
        lighting_setting_group_box = QtGui.QGroupBox(self.tr(self.tr("Light Settings")))                
        lighting_setting_group_box_layout = QtGui.QVBoxLayout()
        lighting_setting_group_box.setLayout(lighting_setting_group_box_layout)
        
        grid_layout = QtGui.QGridLayout()
        lighting_setting_group_box_layout.addLayout(grid_layout)
                
        self.__create_ambient_layout(grid_layout, 0)           
        self.__create_diffuse_layout(grid_layout, 1)
        self.__create_specular_layout(grid_layout, 2)
        self.__create_light_position_layout(grid_layout, 3)
            
        self.display_light_position_checkbox = QtGui.QCheckBox(self.tr("Display light position"))
        lighting_setting_group_box_layout.addWidget(self.display_light_position_checkbox)
        self.display_light_position_checkbox.setChecked(self.initial_display_light_position)
        self.connect(self.display_light_position_checkbox,
                     QtCore.SIGNAL("stateChanged(int)"),
                     self.gl_widget.updateGL)
        
        return lighting_setting_group_box
    
    def __create_light_position_layout(self, grid_layout, row):
        grid_layout.addWidget(QtGui.QLabel(self.tr("Position:")), row, 0)
        
        self.light_position_x_spinbox = QtGui.QDoubleSpinBox()        
        self.light_position_x_spinbox.setDecimals(2)
        self.light_position_x_spinbox.setSingleStep(0.1)
        self.light_position_x_spinbox.setValue(self.initial_light_position.x)
        self.light_position_x_spinbox.setRange(-100, 100)
        self.connect(self.light_position_x_spinbox, 
                     QtCore.SIGNAL("valueChanged(double)"),
                     self.gl_widget.updateGL)
        grid_layout.addWidget(self.light_position_x_spinbox, row, 1)
        
        self.light_position_y_spinbox = QtGui.QDoubleSpinBox()        
        self.light_position_y_spinbox.setDecimals(2)
        self.light_position_y_spinbox.setSingleStep(0.1)
        self.light_position_y_spinbox.setValue(self.initial_light_position.y)
        self.light_position_y_spinbox.setRange(-100, 100)
        self.connect(self.light_position_y_spinbox, 
                     QtCore.SIGNAL("valueChanged(double)"),
                     self.gl_widget.updateGL)
        grid_layout.addWidget(self.light_position_y_spinbox, row, 2)
        
        self.light_position_z_spinbox = QtGui.QDoubleSpinBox()        
        self.light_position_z_spinbox.setDecimals(2)
        self.light_position_z_spinbox.setSingleStep(0.1)
        self.light_position_z_spinbox.setValue(self.initial_light_position.z)
        self.light_position_z_spinbox.setRange(-100, 100)
        self.connect(self.light_position_z_spinbox, 
                     QtCore.SIGNAL("valueChanged(double)"),
                     self.gl_widget.updateGL)
        grid_layout.addWidget(self.light_position_z_spinbox, row, 3)        
    
    def __create_ambient_layout(self, grid_layout, row):
        grid_layout.addWidget(QtGui.QLabel(self.tr("Ambient:")), row, 0)
        
        self.ambient_r_spinbox = QtGui.QDoubleSpinBox()
        self.ambient_r_spinbox.setRange(0.0, 1.0)
        self.ambient_r_spinbox.setDecimals(2)
        self.ambient_r_spinbox.setSingleStep(0.01)
        self.ambient_r_spinbox.setValue(self.initial_ambient.r)
        self.connect(self.ambient_r_spinbox, 
                     QtCore.SIGNAL("valueChanged(double)"),
                     self.gl_widget.updateGL)
        grid_layout.addWidget(self.ambient_r_spinbox, row, 1)        
        
        self.ambient_g_spinbox = QtGui.QDoubleSpinBox()
        self.ambient_g_spinbox.setRange(0.0, 1.0)
        self.ambient_g_spinbox.setDecimals(2)
        self.ambient_g_spinbox.setSingleStep(0.01)
        self.ambient_g_spinbox.setValue(self.initial_ambient.g)
        self.connect(self.ambient_g_spinbox, 
                     QtCore.SIGNAL("valueChanged(double)"),
                     self.gl_widget.updateGL)
        grid_layout.addWidget(self.ambient_g_spinbox, row, 2)        
        
        self.ambient_b_spinbox = QtGui.QDoubleSpinBox()
        self.ambient_b_spinbox.setRange(0.0, 1.0)
        self.ambient_b_spinbox.setDecimals(2)
        self.ambient_b_spinbox.setSingleStep(0.01)
        self.ambient_b_spinbox.setValue(self.initial_ambient.b)
        self.connect(self.ambient_b_spinbox, 
                     QtCore.SIGNAL("valueChanged(double)"),
                     self.gl_widget.updateGL)
        grid_layout.addWidget(self.ambient_b_spinbox, row, 3)
        
    def __create_diffuse_layout(self, grid_layout, row):
        grid_layout.addWidget(QtGui.QLabel(self.tr("Diffuse:")), row, 0)
        
        self.diffuse_r_spinbox = QtGui.QDoubleSpinBox()
        self.diffuse_r_spinbox.setRange(0.0, 1.0)
        self.diffuse_r_spinbox.setDecimals(2)
        self.diffuse_r_spinbox.setSingleStep(0.01)
        self.diffuse_r_spinbox.setValue(self.initial_diffuse.r)
        self.connect(self.diffuse_r_spinbox, 
                     QtCore.SIGNAL("valueChanged(double)"),
                     self.gl_widget.updateGL)
        grid_layout.addWidget(self.diffuse_r_spinbox, row, 1)        
        
        self.diffuse_g_spinbox = QtGui.QDoubleSpinBox()
        self.diffuse_g_spinbox.setRange(0.0, 1.0)
        self.diffuse_g_spinbox.setDecimals(2)
        self.diffuse_g_spinbox.setSingleStep(0.01)
        self.diffuse_g_spinbox.setValue(self.initial_diffuse.g)
        self.connect(self.diffuse_g_spinbox, 
                     QtCore.SIGNAL("valueChanged(double)"),
                     self.gl_widget.updateGL)
        grid_layout.addWidget(self.diffuse_g_spinbox, row, 2)        
        
        self.diffuse_b_spinbox = QtGui.QDoubleSpinBox()
        self.diffuse_b_spinbox.setRange(0.0, 1.0)
        self.diffuse_b_spinbox.setDecimals(2)
        self.diffuse_b_spinbox.setSingleStep(0.01)
        self.diffuse_b_spinbox.setValue(self.initial_diffuse.b)
        self.connect(self.diffuse_b_spinbox, 
                     QtCore.SIGNAL("valueChanged(double)"),
                     self.gl_widget.updateGL)
        grid_layout.addWidget(self.diffuse_b_spinbox, row, 3)        
    
    def __create_specular_layout(self, grid_layout, row):
        grid_layout.addWidget(QtGui.QLabel(self.tr("Specular:")), row, 0)
        
        self.specular_r_spinbox = QtGui.QDoubleSpinBox()
        self.specular_r_spinbox.setRange(0.0, 1.0)
        self.specular_r_spinbox.setDecimals(2)
        self.specular_r_spinbox.setSingleStep(0.01)
        self.specular_r_spinbox.setValue(self.initial_specular.r)
        self.connect(self.specular_r_spinbox, 
                     QtCore.SIGNAL("valueChanged(double)"),
                     self.gl_widget.updateGL)
        grid_layout.addWidget(self.specular_r_spinbox, row, 1)        
        
        self.specular_g_spinbox = QtGui.QDoubleSpinBox()
        self.specular_g_spinbox.setRange(0.0, 1.0)
        self.specular_g_spinbox.setDecimals(2)
        self.specular_g_spinbox.setSingleStep(0.01)
        self.specular_g_spinbox.setValue(self.initial_specular.g)
        self.connect(self.specular_g_spinbox, 
                     QtCore.SIGNAL("valueChanged(double)"),
                     self.gl_widget.updateGL)
        grid_layout.addWidget(self.specular_g_spinbox, row, 2)        
        
        self.specular_b_spinbox = QtGui.QDoubleSpinBox()
        self.specular_b_spinbox.setRange(0.0, 1.0)
        self.specular_b_spinbox.setDecimals(2)
        self.specular_b_spinbox.setSingleStep(0.01)
        self.specular_b_spinbox.setValue(self.initial_specular.b)
        self.connect(self.specular_b_spinbox, 
                     QtCore.SIGNAL("valueChanged(double)"),
                     self.gl_widget.updateGL)
        grid_layout.addWidget(self.specular_b_spinbox, row, 3)
        
    def __create_mmd_data_group_box(self):
        mmd_data_group = QtGui.QGroupBox(self.tr("MMD Data"))        
        mmd_data_group_layout = QtGui.QVBoxLayout()
        mmd_data_group.setLayout(mmd_data_group_layout)
        
        pmd_file_layout = QtGui.QHBoxLayout()
        mmd_data_group_layout.addLayout(pmd_file_layout)
        pmd_file_label = QtGui.QLabel(self.tr("&PMD File:"))            
        self.pmd_filename_edit = QtGui.QLineEdit()
        pmd_file_label.setBuddy(self.pmd_filename_edit)
        self.pmd_filename_edit.setEnabled(False)
        self.pmd_file_load_button = QtGui.QPushButton(self.tr("Load..."))            
        pmd_file_layout.addWidget(pmd_file_label)
        pmd_file_layout.addWidget(self.pmd_filename_edit)
        pmd_file_layout.addWidget(self.pmd_file_load_button)
        
        vmd_file_layout = QtGui.QHBoxLayout()
        mmd_data_group_layout.addLayout(vmd_file_layout)
        vmd_file_label = QtGui.QLabel(self.tr("&VMD File:"))            
        self.vmd_filename_edit = QtGui.QLineEdit()
        vmd_file_label.setBuddy(self.vmd_filename_edit)
        self.vmd_filename_edit.setEnabled(False)
        self.vmd_file_load_button = QtGui.QPushButton(self.tr("Load..."))        
        vmd_file_layout.addWidget(vmd_file_label)
        vmd_file_layout.addWidget(self.vmd_filename_edit)
        vmd_file_layout.addWidget(self.vmd_file_load_button)
        self.clear_vmd_animation_button = QtGui.QPushButton(self.tr("Clear"))
        vmd_file_layout.addWidget(self.clear_vmd_animation_button)
        self.clear_vmd_animation_button.setEnabled(False)
        
        export_layout = QtGui.QHBoxLayout()
        mmd_data_group_layout.addLayout(export_layout)
        export_layout.addStretch()
        self.export_bvh_button = QtGui.QPushButton("Export BVH...")
        self.export_bvh_button.setMinimumWidth(200)
        export_layout.addWidget(self.export_bvh_button)        
        
        self.connect(self.pmd_file_load_button, QtCore.SIGNAL("clicked()"),
                     self.load_pmd_file)
        self.connect(self.vmd_file_load_button, QtCore.SIGNAL("clicked()"),
                     self.load_vmd_file)
        self.connect(self.clear_vmd_animation_button, QtCore.SIGNAL("clicked()"),
                     self.clear_vmd_animation)
        self.connect(self.export_bvh_button, QtCore.SIGNAL("clicked()"),
                     self.export_bvh)
        return mmd_data_group
    
    def __create_animation_control_frame(self):
        animation_control_frame = QtGui.QFrame()        
        animation_control_layout = QtGui.QHBoxLayout()
        animation_control_frame.setLayout(animation_control_layout)
        
        self.playback_button = QtGui.QPushButton("Play")
        self.stop_button = QtGui.QPushButton("Stop")        
        self.frame_slider = QtGui.QSlider(QtCore.Qt.Horizontal)
        self.frame_slider.setTracking(True)
        self.frame_slider.setSizePolicy(QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding,
                                                          QtGui.QSizePolicy.Minimum))
        self.frame_number_spinbox = QtGui.QSpinBox()
        
        self.connect(self.frame_slider, QtCore.SIGNAL("valueChanged(int)"),
                     self.frame_changed)
        self.connect(self.frame_number_spinbox, QtCore.SIGNAL("valueChanged(int)"),
                     self.frame_changed)
                
        animation_control_layout.addWidget(self.playback_button)
        animation_control_layout.addWidget(self.stop_button)
        animation_control_layout.addWidget(self.frame_slider)
        animation_control_layout.addWidget(self.frame_number_spinbox)
        
        self.connect(self.stop_button, QtCore.SIGNAL("clicked()"),
                     self.stop_animation)
        
        self.disable_animation()
        
        self.animation_control_frame = animation_control_frame        
        return animation_control_frame
    
    def reset_camera(self):
        self.gl_widget.init_camera()
        self.gl_widget.updateGL()
        
    def load_pmd_file(self):
        pmd_filename = QtGui.QFileDialog.getOpenFileName(self,
                                                         self.tr("Choose PMD File"),
                                                         self.last_pmd_directory,
                                                         self.tr("PMD Model File (*.pmd);;Any File (*.*)"))
        pmd_filename = str(pmd_filename)
        if len(pmd_filename) <= 0:
            return
        try:
            self.pmd_model = pmd_parser.parse(pmd_filename)
        except Exception, e:
            QtGui.QMessageBox.critical(None, self.tr("VMD Viewer"),
                                       str(e),
                                       QtGui.QMessageBox.Ok | QtGui.QMessageBox.Default,
                                       QtGui.QMessageBox.NoButton)
            return
        
        if self.vmd_animation is None:
            self.primitive = SimplePrimitive(self.pmd_model.get_rest_mesh(),
                                             self.pmd_model.get_texture_mapping(),
                                             self.pmd_model.get_material_mapping(),
                                             self.pmd_model.get_material_list())   
            update_pmd_filename = True         
        else:
            update_pmd_filename = self.compute_ik_less_animation()
        
        if update_pmd_filename:
            self.pmd_filename_edit.setText(pmd_filename)
            self.last_pmd_directory = os.path.dirname(pmd_filename)
            self.gl_widget.updateGL()   
        
    def load_vmd_file(self):
        if self.pmd_model is None:
            QtGui.QMessageBox.critical(None, self.tr("VMD Viewer"),
                                       self.tr("Load an MMD model first!"),
                                       QtGui.QMessageBox.Ok | QtGui.QMessageBox.Default,
                                       QtGui.QMessageBox.NoButton)
            return
        
        vmd_filename = QtGui.QFileDialog.getOpenFileName(self,
                                                         self.tr("Choose VMD File"),
                                                         self.last_vmd_directory,
                                                         self.tr("VMD Animation File (*.vmd);;Any File (*.*)"))
        vmd_filename = str(vmd_filename)
        if len(vmd_filename) <= 0:
            return
        try:
            self.vmd_animation = vmd_parser.parse(vmd_filename)
        except Exception, e:
            QtGui.QMessageBox.critical(None, self.tr("VMD Viewer"),
                                       str(e),
                                       QtGui.QMessageBox.Ok | QtGui.QMessageBox.Default,
                                       QtGui.QMessageBox.NoButton)
            return
        
        if self.pmd_model is not None:
            if self.compute_ik_less_animation():
                self.vmd_filename_edit.setText(vmd_filename)
                self.last_vmd_directory = os.path.dirname(vmd_filename)
                self.gl_widget.updateGL()                
    
    def compute_ik_less_animation(self):
        first_frame, last_frame = self.vmd_animation.get_frame_range()
        
        progress_dialog = QtGui.QProgressDialog("Computing IK...", "Cancel",
                                                first_frame, last_frame, self)
        progress_dialog.setWindowTitle("VMD Viewer")            
        progress_dialog.setWindowModality(QtCore.Qt.WindowModal)
        
        
        def progress_hook(frame_number):
            frame_number = int(math.ceil(frame_number))
            progress_dialog.setValue(frame_number)
            if progress_dialog.wasCanceled():
                return False
            else:
                return True
        self.pose_animation = self.vmd_animation.get_ik_less_pose_animation(self.pmd_model, progress_hook)        
        
        if self.pose_animation is not None:
            self.primitive = self.pmd_model.get_rigged_primitive()
            self.first_frame = first_frame
            self.last_frame = last_frame        
            self.initialize_animation_control()
            return True
        else:
            return False
        
    def morph_primitive(self):        
        self.pose.clear()
        self.pose_animation.get_pose(self.pose, self.current_frame)
        self.primitive.morph(self.pose)        
        self.gl_widget.updateGL()
        
    def frame_changed(self, frame_number):
        if frame_number != self.current_frame:
            self.current_frame = frame_number
            self.morph_primitive()
            self.frame_slider.setValue(frame_number)
            self.frame_number_spinbox.setValue(frame_number)
            
    def initialize_animation_control(self):
        self.current_frame = self.first_frame
        self.frame_slider.setValue(self.current_frame)
        self.frame_number_spinbox.setValue(self.current_frame)
        self.frame_slider.setRange(self.first_frame, self.last_frame);
        self.frame_number_spinbox.setRange(self.first_frame, self.last_frame)        
        self.morph_primitive()
        self.enable_animation()
        
    def go_into_animation_mode(self):
        self.frame_slider.setEnabled(False)
        self.frame_number_spinbox.setEnabled(False)
        self.pmd_file_load_button.setEnabled(False)
        self.vmd_file_load_button.setEnabled(False)
        
        self.stop_button.setEnabled(True)
        self.playback_button.setEnabled(True)
        self.connect(self.playback_button, QtCore.SIGNAL("clicked()"),
                     self.pause_animation)
        self.playback_button.setText("Pause")
        self.export_bvh_button.setEnabled(False)
        self.clear_vmd_animation_button.setEnabled(False)
            
    def go_out_of_animation_mode(self):
        self.frame_slider.setEnabled(True)
        self.frame_number_spinbox.setEnabled(True)
        self.pmd_file_load_button.setEnabled(True)
        self.vmd_file_load_button.setEnabled(True)
        
        self.stop_button.setEnabled(False)
        self.playback_button.setEnabled(True)
        self.connect(self.playback_button, QtCore.SIGNAL("clicked()"),
                     self.start_animation)
        self.playback_button.setText("Play")
        self.export_bvh_button.setEnabled(True)
        self.clear_vmd_animation_button.setEnabled(True)
        
    def disable_animation(self):
        self.playback_button.setEnabled(False)
        self.stop_button.setEnabled(False)
        self.frame_slider.setEnabled(False)
        self.frame_number_spinbox.setEnabled(False)
        self.clear_vmd_animation_button.setEnabled(False)
        self.export_bvh_button.setEnabled(False)
        self.clear_vmd_animation_button.setEnabled(False)
        
    def enable_animation(self):
        self.go_out_of_animation_mode()
        self.clear_vmd_animation_button.setEnabled(True)
        
    def start_animation(self):
        self.go_into_animation_mode()
        self.timer.start(self.period)
        
    def pause_animation(self):
        self.timer.stop()
        self.go_out_of_animation_mode()
        
    def stop_animation(self):
        self.pause_animation()
        self.frame_changed(self.first_frame)
        
    def advance_frame(self):
        if self.current_frame >= self.last_frame:
            self.frame_changed(self.first_frame)
        else:
            self.frame_changed(self.current_frame+1)
            
    def clear_vmd_animation(self):
        self.primitive = SimplePrimitive(self.pmd_model.get_rest_mesh(),
                                             self.pmd_model.get_texture_mapping(),
                                             self.pmd_model.get_material_mapping(),
                                             self.pmd_model.get_material_list())
        self.vmd_animation = None
        self.vmd_filename_edit.setText("")
        self.disable_animation()
        self.gl_widget.updateGL()
        
    def export_bvh(self):
        export_dialog = ExportBVHDialog(self.last_save_directory,
                                        self.language_to_save_in,
                                        self.scaling_factor)
        if export_dialog.exec_() == QtGui.QDialog.Accepted:
            save_file_name = export_dialog.save_file_name
            self.scaling_factor = export_dialog.scaling_factor
            self.language_to_save_in = export_dialog.language_to_save_in
            
            saved_content = bvh_exporter.export(self.pmd_model.get_rest_armature(), 
                                                self.pose_animation,
                                                self.language_to_save_in,
                                                30,
                                                self.scaling_factor)
            
            if os.path.isdir(save_file_name):
                QtGui.QMessageBox.critical(None, self.tr("VMD Viewer"),
                                           self.tr("The specified path is a directory. Cannot save to a directory."),
                                           QtGui.QMessageBox.Ok | QtGui.QMessageBox.Default,
                                           QtGui.QMessageBox.NoButton)
                return
                        
            try:
                f = open(save_file_name, "wt")
                f.write(saved_content)                
            except Exception, e:                
                QtGui.QMessageBox.critical(None, self.tr("VMD Viewer"),
                                           str(e),
                                           QtGui.QMessageBox.Ok | QtGui.QMessageBox.Default,
                                           QtGui.QMessageBox.NoButton)
                return
            finally:
                f.close()                
            self.last_save_directory = os.path.dirname(save_file_name)
    
class ExportBVHDialog(QtGui.QDialog):
    def __init__(self, last_save_directory,                  
                 language_to_save_in, scaling_factor, parent=None):
        QtGui.QDialog.__init__(self, parent)
        self.setWindowTitle("Export BVH")
        
        self.last_save_directory = last_save_directory
        self.language_to_save_in = language_to_save_in
        self.scaling_factor = scaling_factor
        
        layout = QtGui.QGridLayout()
        self.setLayout(layout)
        
        layout.addWidget(QtGui.QLabel(self.tr("File Name:")), 0, 0)
        filename_layout = QtGui.QHBoxLayout()
        layout.addLayout(filename_layout, 0, 1)
        self.save_file_name_edit = QtGui.QLineEdit()
        self.save_file_name_edit.setMinimumWidth(300)
        filename_layout.addWidget(self.save_file_name_edit)
        self.choose_file_name_button = QtGui.QPushButton("Choose...")        
        filename_layout.addWidget(self.choose_file_name_button)
        
        layout.addWidget(QtGui.QLabel(self.tr("Scaling Factor:")), 1, 0)
        self.scaling_factor_edit = QtGui.QLineEdit()
        layout.addWidget(self.scaling_factor_edit, 1, 1)        
        
        layout.addWidget(QtGui.QLabel(self.tr("Language:")), 2, 0)
        language_layout = QtGui.QHBoxLayout()
        layout.addLayout(language_layout, 2, 1)
        self.english_radio_button = QtGui.QRadioButton("English (unicode escaped)")
        self.japanese_radio_button = QtGui.QRadioButton("Japanese")
        language_layout.addWidget(self.english_radio_button)
        language_layout.addWidget(self.japanese_radio_button)
        language_layout.addStretch()
        
        buttons_layout = QtGui.QHBoxLayout()
        self.save_button = QtGui.QPushButton(self.tr("Save"))
        self.cancel_button = QtGui.QPushButton(self.tr("Cancel"))
        buttons_layout.addStretch()
        buttons_layout.addWidget(self.save_button)
        buttons_layout.addWidget(self.cancel_button)
        layout.addLayout(buttons_layout, 3, 1)
        
        if self.language_to_save_in == "english":
            self.english_radio_button.setChecked(True)
        elif self.language_to_save_in == "japanese":
            self.japanese_radio_button.setChecked(True)
            
        self.scaling_factor_edit.setText(str(self.scaling_factor))
    
        self.connect(self.cancel_button, QtCore.SIGNAL("clicked()"),
                     self.reject)
        self.connect(self.save_button, QtCore.SIGNAL("clicked()"),
                     self.save)
        self.connect(self.choose_file_name_button, QtCore.SIGNAL("clicked()"),
                     self.choose_file)
    
    def save(self):
        self.save_file_name = str(self.save_file_name_edit.text())
        if len(self.save_file_name) <= 0:
            QtGui.QMessageBox.critical(None, self.tr("VMD Viewer"),
                                       self.tr("Please specify a file to save to!"),
                                       QtGui.QMessageBox.Ok | QtGui.QMessageBox.Default,
                                       QtGui.QMessageBox.NoButton)
            return
        
        try:
            self.scaling_factor = float(str(self.scaling_factor_edit.text()))
        except Exception, e:
            QtGui.QMessageBox.critical(None, self.tr("VMD Viewer"),
                                       self.tr("The specified scaling factor is not valid. You must specify a nonnegative real number."),
                                       QtGui.QMessageBox.Ok | QtGui.QMessageBox.Default,
                                       QtGui.QMessageBox.NoButton)
            return
        if self.scaling_factor < 0:
            QtGui.QMessageBox.critical(None, self.tr("VMD Viewer"),
                                       self.tr("The specified scaling factor is not valid. You must specify a nonnegative real number."),
                                       QtGui.QMessageBox.Ok | QtGui.QMessageBox.Default,
                                       QtGui.QMessageBox.NoButton)
            return
        
        if self.english_radio_button.isChecked():
            self.language_to_save_in = "english"
        elif self.japanese_radio_button.isChecked():
            self.language_to_save_in = "japanese"
        
        self.accept()
    
    def choose_file(self):
        save_file_name = QtGui.QFileDialog.getSaveFileName(self, 
                                             "Choose File Name to Export to",
                                             self.last_save_directory,
                                             self.tr("Biovision Hierarchical Data (*.bvh);;Any File (*.*)"))
        if len(str(save_file_name)) > 0:
            save_file_name = os.path.abspath(str(save_file_name))
            self.save_file_name_edit.setText(save_file_name)            
                    
class GLWidget(QtOpenGL.QGLWidget):
    def __init__(self, main_window, parent=None):
        QtOpenGL.QGLWidget.__init__(self, parent)
        
        self.main_window = main_window
        
        self.camera = ArcballCamera()
        self.init_camera()        
        
        self.renderer = GLRenderer()
                
        self.axis_length = 30.0        
        self.cell_width = 4
        self.cell_count = 8
        
        self.setMouseTracking(False)
        
    def init_camera(self):
        self.camera.set_center(Point3(0,10,0))
        self.camera.set_distance(30)
        self.camera.set_rotation(matrix4x4.identity())  
        self.camera.set_perspective(60.0)
        self.camera.set_viewport(0, 0, self.width(), self.height())
    
    def minimumSizeHint(self):
        return QtCore.QSize(50, 50)

    def initializeGL(self):        
        glClearColor(0.9, 0.9, 0.9, 1.0)
        glClearDepth(1.0)
        glShadeModel(GL_SMOOTH)
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_NORMALIZE)
        glDepthFunc(GL_LEQUAL)
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)
        glPolygonMode(GL_FRONT, GL_FILL)
        glDisable(GL_CULL_FACE)
        
    def resizeGL(self, width, height):
        self.camera.set_viewport(0,0,width,height)
        
    def mousePressEvent(self, event):
        button = event.button()
        x = event.x()
        y = event.y()
        if button == QtCore.Qt.LeftButton:
            self.camera.mouse_click(ArcballCamera.ROTATE, x, y)            
        elif button == QtCore.Qt.MidButton:
            self.camera.mouse_click(ArcballCamera.TRANSLATE, x, y)            
        elif button == QtCore.Qt.RightButton:
            self.camera.mouse_click(ArcballCamera.ZOOM, x, y)            
        self.updateGL()

    def mouseMoveEvent(self, event):        
        self.camera.mouse_drag(event.x(),event.y())
        self.updateGL()
        
    def mouseReleaseEvent(self, event):
        self.camera.mouse_release(event.x(), event.y())
        self.updateGL()

    def paintGL(self):
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT)

        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        self.camera.apply_viewport()
        self.camera.apply_perspective()
        
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        light_position = [self.main_window.light_position_x_spinbox.value(),
                          self.main_window.light_position_y_spinbox.value(),
                          self.main_window.light_position_z_spinbox.value(),
                          1]
        light_ambient = [self.main_window.ambient_r_spinbox.value(),
                         self.main_window.ambient_g_spinbox.value(), 
                         self.main_window.ambient_b_spinbox.value(), 
                         1]
        light_diffuse = [self.main_window.diffuse_r_spinbox.value(),
                         self.main_window.diffuse_g_spinbox.value(), 
                         self.main_window.diffuse_b_spinbox.value(), 
                         1]
        light_specular = [self.main_window.specular_r_spinbox.value(),
                          self.main_window.specular_g_spinbox.value(), 
                          self.main_window.specular_b_spinbox.value(), 
                          1.0]

        glLightfv(GL_LIGHT0, GL_POSITION, light_position)
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse)
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular)

        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity();
        self.camera.apply_modelview()

        self.renderer.reset_state()
        if self.main_window.primitive is not None:
            self.main_window.primitive.accept(self.renderer)
                    
        if self.main_window.display_plane_checkbox.isChecked():
            self.draw_plane()
        if self.main_window.display_axes_checkbox.isChecked():
            self.draw_axis()
        if self.main_window.display_light_position_checkbox.isChecked():
            self.draw_light_position()
        
    def draw_axis(self):                    
        # Save current state of OpenGL
        glPushAttrib(GL_ALL_ATTRIB_BITS)        

        # This is to draw the axes when the mouse button is down
        glDisable(GL_LIGHTING)
        glDisable(GL_TEXTURE_2D)
        glLineWidth(2)
        glPushMatrix()
        glScaled(self.axis_length, self.axis_length, self.axis_length)
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
        glEnd()        
        glPopMatrix()        
        glPopAttrib()
    
    def draw_plane(self):
        glPushAttrib(GL_ALL_ATTRIB_BITS)
        
        glDisable(GL_LIGHTING)
        glDisable(GL_TEXTURE_2D)
        glLineWidth(1)
        
        s = self.cell_width * self.cell_count
        glColor3f(0.5, 0.5, 0.5)
        glBegin(GL_LINES)
        for x in xrange(-s, s+1, self.cell_width):
            glVertex3f(x, 0.0, -s)
            glVertex3f(x, 0.0,  s)
        for z in xrange(-s, s+1, self.cell_width):
            glVertex3f(-s, 0.0, z)
            glVertex3f( s, 0.0, z)            
        glEnd()
        
        glPopAttrib()
        
    def draw_light_position(self):
        glPushAttrib(GL_ALL_ATTRIB_BITS)
        glDisable(GL_LIGHTING)
        glDisable(GL_TEXTURE_2D)
        glPushMatrix()
        glColor3f(1.0, 1.0, 0.0)
        glTranslate(self.main_window.light_position_x_spinbox.value(),
                    self.main_window.light_position_y_spinbox.value(),
                    self.main_window.light_position_z_spinbox.value())
        glutSolidSphere(0.5, 20, 20)
        glPopMatrix()
        
        glPopAttrib()
        
def main():
    app = QtGui.QApplication(sys.argv)
    app.setOrganizationName("dragonmeteor")
    app.setOrganizationDomain("dragonmeteor")
    app.setApplicationName("VMD Viewer")    
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())