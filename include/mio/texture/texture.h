#ifndef __MIO_MATERIAL_TEXTURE_H__
#define __MIO_MATERIAL_TEXTURE_H__

#include <mio/mio.h>
#include <mio/mio_object.h>

namespace mio
{
	namespace texture
	{
		class TextureVisitor;

		class MIO_API Texture : public MioObject
		{
		public:
			Texture(const char *name="");
			virtual ~Texture();

			virtual void accept(mio::texture::TextureVisitor *visitor);
		};
	}
}

#endif