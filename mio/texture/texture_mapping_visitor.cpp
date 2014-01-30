#include <mio/texture/texture_mapping_visitor.h>
#include <mio/exceptions.h>

namespace mio
{
	namespace texture
	{
		TextureMappingVisitor::TextureMappingVisitor()
		{
		}

		TextureMappingVisitor::~TextureMappingVisitor()
		{
		}

		void TextureMappingVisitor::visit_IndexedTextureMapping(mio::texture::IndexedTextureMapping *mapping)
		{
			throw NotImplementedException();
		}
	}
}