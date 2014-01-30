#include <mio/texture/indexed_texture_mapping.h>
#include <mio/texture/texture_mapping_visitor.h>
#include <stdio.h>

namespace mio
{
	namespace texture
	{
		IndexedTextureMapping::IndexedTextureMapping(const char *name) : TextureMapping(name)
		{
		}

		IndexedTextureMapping::~IndexedTextureMapping()
		{
		}		

		
		void IndexedTextureMapping::accept(mio::texture::TextureMappingVisitor *visitor)
		{
			visitor->visit_IndexedTextureMapping(this);
		}
	}
}