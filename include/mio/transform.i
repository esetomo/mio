%{
#include <mio/transform.h>
%}

%include <mio/transform.h>

%extend mio::Transform
{
	%pythoncode 
	%{
	def accept(self, visitor):
		visitor.visit_Transform(self)
	%}
}