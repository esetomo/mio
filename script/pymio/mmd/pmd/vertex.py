class Vertex(object):
    """
    Represent a vertex of an MMD model, as stored in
    an MMD file.
    
    @ivar position: the 3D position of the vertex.
    @type position: sunnymilk.vertex.Vertex
    
    @ivar normal: the normal at the vertex
    @type normal: sunnymilk.vertex.Vertex
    
    @ivar tex_coord: the texture coordinate
    @type tex_coord: sunnymilk.uv.Uv
    
    @ivar bone0_number: the number of the first bone
        the vertex is attached to.
    @type bone0_number: integer
    
    @ivar bone1_number: the number of the second bone
        the vertex is attached to.
    @type bone1_number: integer
    
    @ivar weight: the weight assigned to the first bone.
        Must be a number between 0 to 1. The weight assigned
        to the second bone is one minus this weight.
    @type weight: float
    """
    def __init__(self, position, normal, tex_coord, 
                 bone0_number, bone1_number, weight):
        self.position = position
        self.normal = normal
        self.tex_coord = tex_coord
        self.bone0_number = bone0_number
        self.bone1_number = bone1_number
        self.weight = weight