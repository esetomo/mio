%{
#include <mio/rigging/armature.h>
%}

%include <mio/rigging/armature.h>

%extend mio::rigging::Armature
{
	%pythoncode
	%{
		def get_joints(self):
			return self.get_joint_iterator()
	%}
}

%extend mio::rigging::JointIterator
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