#ifndef __MIO_MATERIAL_TEXTURE_VISITOR_H__
#define __MIO_MATERIAL_TEXTURE_VISITOR_H__

#include <mio/mio.h>
#include <mio/texture/texture.h>
#include <mio/texture/il_texture.h>
#include <mio/texture/lazy_il_texture.h>
#include <mio/mio_object.h>

namespace mio
{
	namespace texture
	{
		class MIO_API TextureVisitor
		{
		public:
			TextureVisitor();
			virtual ~TextureVisitor();

			virtual void visit_ILTexture(mio::texture::ILTexture *texture);
			virtual void visit_LazyILTexture(mio::texture::LazyILTexture *texture);
		};
	}
}

#endif