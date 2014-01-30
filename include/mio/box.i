%{
#include <mio/box.h>
%}

%include <mio/box.h>

%extend mio::Box
{
	%pythoncode 
	%{
	def accept(self, visitor):
		visitor.visit_Box(self)	
	%}	
}