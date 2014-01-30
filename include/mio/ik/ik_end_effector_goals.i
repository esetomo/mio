%{
#include <mio/ik/ik_end_effector_goals.h>
%}

%include <mio/ik/ik_end_effector_goals.h>

%extend mio::ik::IKEndEffectorPositionIterator
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