%{
#include <mio/texture/indexed_texture_mapping.h>
%}

%include <mio/texture/indexed_texture_mapping.h>

%extend mio::texture::IndexedTextureMapping
{
	%pythoncode
	%{
		def tex_coords(self):
			for i in xrange(self.get_tex_coord_count()):
				yield self.get_tex_coord(i)
		
		def triangle_tex_coord_indices(self):
			for i in xrange(self.get_mapping_count()):
				yield self.get_triangle_tex_coord_indices(i)
	%}
}