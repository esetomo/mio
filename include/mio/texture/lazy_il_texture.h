#ifndef __MIO_TEXTURE_LAZY_IL_TEXTURE_H__
#define __MIO_TEXTURE_LAZY_IL_TEXTURE_H__

#include <mio/mio.h>
#include <mio/reference.h>
#include <mio/texture/texture.h>
#include <mio/texture/il_texture.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
	namespace texture
	{
		class TextureVisitor;

		class MIO_API LazyILTexture : public Texture
		{
		private:
			string full_path_;
			Reference<ILTexture> il_texture_;

		public:
			LazyILTexture(const char *full_path, const char *name="");
			~LazyILTexture();

			ILTexture *get_il_texture();

			virtual void accept(TextureVisitor *visitor);
		};
	}
}

#pragma warning(pop)

#endif