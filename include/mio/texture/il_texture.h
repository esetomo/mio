#ifndef __MIO_MATERIAL_IL_TEXTURE_H__
#define __MIO_MATERIAL_IL_TEXTURE_H__

#include <mio/mio.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <mio/texture/texture.h>
#include <mio/il/il_image.h>

namespace mio
{
	namespace texture
	{
		using namespace mio::il;

		class MIO_API ILTexture : public Texture
		{
		private:
			Reference<ILImage> image_;
		
		public:
			ILTexture(ILImage *_image, const char *name="");
			virtual ~ILTexture();

			inline ILImage *get_image()
			{
				return image_.get_pointer();
			}

			void accept(TextureVisitor *visitor);
		};
	}
}

#endif