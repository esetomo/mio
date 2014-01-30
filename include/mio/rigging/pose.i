%{
#include <mio/rigging/pose.h>
%}

%include <mio/rigging/pose.h>

%extend mio::rigging::Pose
{
	%pythoncode
	%{
		def get_name_joint_change_pairs(self):
			return self.get_iterator()
	%}
}

%extend mio::rigging::NameJointChangePairIterator
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