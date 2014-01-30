#include <mio/texture/texture_mapping.h>
#include <mio/exceptions.h>

namespace mio
{
	namespace texture
	{
		TextureMapping::TextureMapping(const char *name) : MioObject(name)
		{
		}

		TextureMapping::~TextureMapping()
		{
		}

		void TextureMapping::accept(mio::texture::TextureMappingVisitor *visitor)
		{
			throw NotImplementedException();
		}
	}
}