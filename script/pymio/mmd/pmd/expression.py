class Expression(object):
    def __init__(self, name):
        self.name = name
        self.vertex_modifications = []
        
    def append_vertex_modification(self, vertex_modification):
        self.vertex_modifications.append(vertex_modification)