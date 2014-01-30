#include <mio/texture/texture_visitor.h>
#include <mio/texture/il_texture.h>
#include <mio/texture/lazy_il_texture.h>
#include <mio/il/il_image_service.h>
#include <mio/utility/path.h>

namespace mio
{
	namespace texture
	{
		LazyILTexture::LazyILTexture(const char *full_path, const char *name)  : Texture(name)
		{
			std::string full_path_string = full_path;
			full_path_ = mio::utility::Path::v()->get_absolute_path(full_path_string);
			il_texture_ = NULL;
		}

		LazyILTexture::~LazyILTexture()
		{
		}

		ILTexture *LazyILTexture::get_il_texture()
		{
			if (il_texture_.get_pointer() == NULL)
			{
				mio::il::ILImage *image = mio::il::ILImageService::v()->get_image(full_path_);
				il_texture_ = new mio::texture::ILTexture(image);				
			}
			return il_texture_.get_pointer();			
		}

		void LazyILTexture::accept(TextureVisitor *visitor)
		{
			visitor->visit_LazyILTexture(this);
		}
	}
}