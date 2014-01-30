class Material(object):
    def __init__(self, ambient, diffuse, specular, shininess,
                 shader_number, shader_edge_number, triangle_count, 
                 texture_filename):        
        self.ambient = ambient
        self.diffuse = diffuse
        self.specular = specular
        self.shininess = shininess
        self.shader_number = shader_number
        self.shader_edge_number = shader_edge_number
        self.triangle_count = triangle_count
        self.texture_filename = texture_filename