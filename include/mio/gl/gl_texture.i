%{
#include <mio/gl/gl_texture.h>
%}

%include <mio/gl/gl_texture.h>

%extend mio::gl::GLTexture
{
	%pythoncode 
	%{
		def accept(self, visitor):
			visitor.visit_GLTexture(self)
	%}
}