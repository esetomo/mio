%{
#include <mio/quaternion.h>
%}

%include <mio/quaternion.h>

%extend mio::Quaternion
{
	%pythoncode 
	%{
		def accept(self, visitor):
			visitor.visit_Quaternion(self)
			
		def __repr__(self):
			return "Quaternion<%f, %f, %f, %f>" % (self.x, self.y, self.z, self.w)
	%}
}