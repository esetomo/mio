%{
#include <mio/geometry/mesh.h>
%}

%include <mio/geometry/mesh.h>

%extend mio::geometry::Mesh
{
	%pythoncode
	%{
		def vertices(self):
			for i in xrange(self.get_vertex_count()):
				yield self.get_vertex(i)
				
		def normals(self):
			for i in xrange(self.get_normal_count()):
				yield self.get_normal(i)
	%}
}