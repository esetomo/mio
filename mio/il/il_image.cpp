#include <mio/il/il_image.h>
#include <mio/il/il_image_service.h>
#include <mio/utility/path.h>
#include <sstream>

namespace mio
{
	namespace il
	{
		using namespace gl;

		ILImage::ILImage(const string &_filename) : MioObject()
		{
			ILImageService::init();
			ilGenImages(1, &imageID_);
			if (ilGetError() != IL_NO_ERROR)
				throw ILException("ILException: could not generate a new image");

			Load(_filename);
			if (ilGetError() != IL_NO_ERROR)
			{
				stringstream ss;
				ss << "ILException: could not load file " << _filename;
				throw ILException(ss.str().c_str());
			}

			texture_ = NULL;
		}

		ILImage::~ILImage()
		{
			ilDeleteImages(1, &imageID_);
			texture_ = NULL;
		}

		void ILImage::Load(const string &_filename)
		{
			ilBindImage(imageID_);
			ilLoadImage(_filename.c_str());
			ilConvertImage(IL_RGBA, IL_FLOAT);			

			mipmapBuilt_ = false;
			fileName_ = _filename;
			width_ = ilGetInteger(IL_IMAGE_WIDTH);
			height_ = ilGetInteger(IL_IMAGE_HEIGHT);
		}

		void ILImage::bind()
		{
			ilBindImage(imageID_);
		}

		void ILImage::build_mipmap()
		{
			bind();
			if (!mipmapBuilt_)
			{
				if (!iluBuildMipmaps())
				{
					throw "Cannot build mipmap for some reason!";
				}
				mipmapBuilt_ = true;
			}
		}

		GLTexture *ILImage::get_texture()
		{
			if (texture_.get_pointer() != NULL)
				return texture_.get_pointer();
			else
			{
				bind();
				texture_ = new GLTexture();				
				texture_->build_mipmap(4, width_, height_, GL_RGBA, GL_FLOAT, ilGetData());
				return texture_.get_pointer();
			}
		}
	}
}