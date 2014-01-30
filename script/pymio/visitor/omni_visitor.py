import cmio

class OmniVisitor(cmio.OmniVisitor):
    def _visit_SimplePrimitive(self, primitive):
        raise NotImplementedError()    
    def visit_SimplePrimitive(self, *args):
        if len(args) != 1:
            raise TypeError("visit_SimplePrimitive() takes exactly 1 argument")
        else:
            self._visit_SimplePrimitive(args[0])    
    
    def _visit_Group(self, group):
        raise NotImplementedError() 
    def visit_Group(self, *args): 
        if len(args) != 1:
            raise TypeError("visit_Group() takes exactly 1 argument")
        else:
            self._visit_Group(args[0])
            
    def _visit_TransformNode(self, transform_node):
        raise NotImplementedError() 
    def visit_TransformNode(self, *args): 
        if len(args) != 1:
            raise TypeError("visit_TransformNode() takes exactly 1 argument")
        else:
            self._visit_Group(args[0])
            
    def _visit_IndexedTextureMapping(self, mapping):
        raise NotImplementedError()
    def visit_IndexedTextureMapping(self, *args):
        if len(args) != 1:
            raise TypeError("visit_IndexedTextureMapping() takes exactly 1 argument")
        else:
            self._visit_IndexedTextureMapping(args[0])
    
    def _visit_ILTexture(self, mapping):
        raise NotImplementedError()
    def visit_ILTexture(self, *args):
        if len(args) != 1:
            raise TypeError("visit_ILTexture() takes exactly 1 argument")
        else:
            self._visit_ILTexture(args[0])
    
    def _visit_LazyILTexture(self, mapping):
        raise NotImplementedError()
    def visit_LazyILTexture(self, *args):
        if len(args) != 1:
            raise TypeError("visit_LazyILTexture() takes exactly 1 argument")
        else:
            self._visit_LazyILTexture(args[0])
            
    def _visit_ByPolygonMaterialMapping(self, mapping):
        raise NotImplementedError()
    def visit_ByPolygonMaterialMapping(self, *args):
        if len(args) != 1:
            raise TypeError("visit_ByPolygonMaterialMapping() takes exactly 1 argument")
        else:
            self._visit_ByPolygonMaterialMapping(args[0])
    
    def _visit_PhongMaterial(self, material):
        raise NotImplementedError() 
    def visit_PhongMaterial(self, *args): 
        if len(args) != 1:
            raise TypeError("visit_PhongMaterial() takes exactly 1 argument")
        else:
            self._visit_PhongMaterial(args[0])
    
    def _visit_TexturedPhongMaterial(self, material):
        raise NotImplementedError()
    def visit_TexturedPhongMaterial(self, *args):
        if len(args) != 1:
            raise TypeError("visit_TexturedPhongMaterial() takes exactly 1 argument")
        else:
            self._visit_TexturedPhongMaterial(args[0])
            
    def _visit_MqoMaterial(self, material):
        raise NotImplementedError()
    def visit_MqoMaterial(self, *args):
        if len(args) != 1:
            raise TypeError("visit_MqoMaterial() takes exactly 1 argument")
        else:
            self._visit_MqoMaterial(args[0])
    
    def _visit_TexturedMqoMaterial(self, material):
        raise NotImplementedError()
    def visit_TexturedMqoMaterial(self, *args):
        if len(args) != 1:
            raise TypeError("visit_TexturedMqoMaterial() takes exactly 1 argument")
        else:
            self._visit_TexturedMqoMaterial(args[0])
            
    def _visit_VertexNormalIndexedMesh(self, mesh):
        raise NotImplementedError()
    def visit_VertexNormalIndexedMesh(self, *args):
        if len(args) != 1:
            raise TypeError("visit_VertexNormalIndexedMesh() takes exactly 1 argument")
        else:
            self._visit_VertexNormalIndexedMesh(args[0])
    
    def _visit_Vector3(self, vector):
        raise NotImplementedError()
    def visit_Vector3(self, *args):
        if len(args) != 1:
            raise TypeError("visit_Vector3() takes exactly 1 argument")
        else:
            self._visit_Vector3(args[0])
            
    def _visit_Point3(self, point):
        raise NotImplementedError()
    def visit_Point3(self, *args):
        if len(args) != 1:
            raise TypeError("visit_Point3() takes exactly 1 argument")
        else:
            self._visit_Point3(args[0])
    
    def _visit_Int3(self, int3):
        raise NotImplementedError()
    def visit_Int3(self, *args):
        if len(args) != 1:
            raise TypeError("visit_Int3() takes exactly 1 argument")
        else:
            self._visit_Int3(args[0])
    
    def _visit_Rgb(self, rgb):
        raise NotImplementedError()
    def visit_Rgb(self, *args):
        if len(args) != 1:
            raise TypeError("visit_Rgb() takes exactly 1 argument")
        else:
            self._visit_Rgb(args[0])
    
    def _visit_Rgba(self, rgba):
        raise NotImplementedError()
    def visit_Rgba(self, *args):
        if len(args) != 1:
            raise TypeError("visit_Rgba() takes exactly 1 argument")
        else:
            self._visit_Rgba(args[0])
    
    def _visit_Box(self, box):
        raise NotImplementedError()
    def visit_Box(self, *args):
        if len(args) != 1:
            raise TypeError("visit_Box() takes exactly 1 argument")
        else:
            self._visit_Box(args[0])
    
    def _visit_Ray(self, ray):
        raise NotImplementedError()
    def visit_Ray(self, *args):
        if len(args) != 1:
            raise TypeError("visit_Ray() takes exactly 1 argument")
        else:
            self._visit_Rya(args[0])
    
    def _visit_ILImage(self, image):
        raise NotImplementedError()
    def visit_ILImage(self, *args):
        if len(args) != 1:
            raise TypeError("visit_ILImage() takes exactly 1 argument")
        else:
            self._visit_ILImage(args[0])
    
    def _visit_GLTexture(self, texture):
        raise NotImplementedError()
    def visit_GLTexture(self, *args):
        if len(args) != 1:
            raise TypeError("visit_GLTexture() takes exactly 1 argument")
        else:
            self._visit_GLTexture(args[0])
    
    def _visit_Matrix4x4(self, matrix):
        raise NotImplementedError()
    def visit_Matrix4x4(self, *args):
        if len(args) != 1:
            raise TypeError("visit_Matrix4x4() takes exactly 1 argument")
        else:
            self._visit_Matrix4x4(args[0])
    
    def _visit_Transform(self, transform):
        raise NotImplementedError()
    def visit_Transform(self, *args):
        if len(args) != 1:
            raise TypeError("visit_Transform() takes exactly 1 argument")
        else:
            self._visit_Transform(args[0])
            
    def _visit_ByTriangleMaterialMapping(self, mapping):
        raise NotImplementedError()
    def visit_Transform(self, *args):
        if len(args) != 1:
            raise TypeError("visit_ByTriangleMaterialMapping() takes exactly 1 argument")
        else:
            self._visit_ByTriangleMaterialMapping(args[0])