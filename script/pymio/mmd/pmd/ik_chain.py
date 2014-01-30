class IKChain:    
    def __init__(self,
                 ik_bone_index, 
                 end_effector_index, 
                 affected_bone_indices,
                 constant_0,
                 constant_1):        
        self.ik_bone_index = ik_bone_index
        self.end_effector_index = end_effector_index
        self.affected_bone_indices = affected_bone_indices
        self.constant_0 = constant_0
        self.constant_1 = constant_1