%{
#include <mio/material/material_mapping.h>
%}

%include <mio/material/material_mapping.h>

%extend mio::material::MaterialMapping
{
	%pythoncode
	%{
		def mappings(self):
			for i in xrange(self.get_mapping_count()):
				yield self.get_mapping(i)
	%}
}