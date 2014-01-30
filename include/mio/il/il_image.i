%{
#include <mio/il/il_image.h>
%}

%include <mio/il/il_image.h>

%extend mio::il::ILImage
{
	%pythoncode 
	%{
		def accept(self, visitor):
			visitor.visit_ILImage(self)
	%}
}