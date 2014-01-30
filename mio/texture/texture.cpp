#include <mio/texture/texture.h>

namespace mio
{
	namespace texture
	{
		Texture::Texture(const char *name) : MioObject(name)
		{
		}

		Texture::~Texture()
		{
		}

		void Texture::accept(mio::texture::TextureVisitor *visitor)
		{
			throw "Not implmented yet!";
		}
	}
}