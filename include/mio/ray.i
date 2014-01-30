%{
#include <mio/ray.h>
%}

%include <mio/ray.h>

%extend mio::Ray
{
	%pythoncode 
	%{
	def accept(self, visitor):
		visitor.visit_Ray(self)
	%}
}