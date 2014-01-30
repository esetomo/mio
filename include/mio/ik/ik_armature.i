%{
#include <mio/ik/ik_armature.h>
%}

%include <mio/ik/ik_armature.h>

%extend mio::ik::IKArmature
{
	%pythoncode
	%{
		def get_ik_joints(self):
			return self.get_ik_joint_iterator()
			
		def get_end_effectors(self):
			return self.get_end_effector_iterator()
	%}
}

%extend mio::ik::IKEndEffectorIterator
{
	%pythoncode
	%{
		def __iter__(self):
			return self
			
		def next(self):
			if not self.has_next():
				raise StopIteration
			else:
				return self.get_next()
	%}
}

%extend mio::ik::IKJointIterator
{
	%pythoncode
	%{
		def __iter__(self):
			return self
			
		def next(self):
			if not self.has_next():
				raise StopIteration
			else:
				return self.get_next()
	%}
}