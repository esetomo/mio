%{
#include <mio/uv.h>
%}

%include <mio/uv.h>

%extend mio::Uv
{
	%pythoncode 
	%{
	def accept(self, visitor):
		visitor.visit_Uv(self)
	%}
}