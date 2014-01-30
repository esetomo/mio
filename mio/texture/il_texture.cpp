#include <mio/texture/il_texture.h>
#include <mio/texture/texture_visitor.h>

namespace mio
{
	namespace texture
	{
		using namespace mio::il;

		ILTexture::ILTexture(ILImage *_image, const char *name) : Texture(name)
		{
			image_ = _image;
		}

		ILTexture::~ILTexture()
		{
			image_ = NULL;
		}

		void ILTexture::accept(TextureVisitor *visitor)
		{
			visitor->visit_ILTexture(this);
		}
	}
}