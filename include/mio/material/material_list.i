%{
#include <mio/material/material_list.h>
%}

%include <mio/material/material_list.h>

%extend mio::material::MaterialList
{
	%pythoncode 
	%{
		def accept(self, visitor):
			visitor.visit_MaterialList(self)
			
		def materials(self):
			for i in xrange(self.get_material_count()):
				yield self.get_material(i)
	%}
}