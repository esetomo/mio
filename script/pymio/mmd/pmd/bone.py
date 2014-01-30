class Bone:
    BONE_ROTATE = 0
    BONE_ROTATE_TRANSLATE = 1
    BONE_IK = 2
    BONE_UNKNOWN = 3
    BONE_IK_ROTATION_INFLUENCED = 4
    BONE_ROTATION_INFLUENCED = 5
    BONE_IK_TARGET = 6
    BONE_INVISIBLE = 7

    def __init__(self, name, bone_type, position, 
                 parent_index, tip_index, ik_bone_index):
        self.name = name
        self.bone_type = bone_type
        self.position = position
        if parent_index == 65535:
            self.parent_index = -1
        else:
            self.parent_index = parent_index
        self.tip_index = tip_index
        self.ik_bone_index = ik_bone_index
        self.index = -1