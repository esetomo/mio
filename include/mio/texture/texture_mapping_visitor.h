#ifndef __MIO_MATERIAL_TEXTURE_MAPPING_VISITOR_H__
#define __MIO_MATERIAL_TEXTURE_MAPPING_VISITOR_H__

#include <mio/mio.h>
#include <mio/texture/texture_mapping.h>
#include <mio/texture/indexed_texture_mapping.h>
#include <mio/mio_object.h>

namespace mio
{
	namespace texture
	{
		class MIO_API TextureMappingVisitor
		{
		public:
			TextureMappingVisitor();
			virtual ~TextureMappingVisitor();
			
			virtual void visit_IndexedTextureMapping(mio::texture::IndexedTextureMapping *mapping);
		};
	}
}

#endif