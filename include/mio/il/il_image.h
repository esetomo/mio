#ifndef __MIO_IL_IL_IMAGE_H__
#define __MIO_IL_IL_IMAGE_H__

#include <mio/mio.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <string>
#include <mio/mio_object.h>
#include <mio/reference.h>
#include <mio/gl/gl_texture.h>
#include <mio/il/il_exception.h>
#include <string>

using namespace std;

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
	namespace il
	{
		using namespace gl;
		
		class MIO_API ILImage : public MioObject
		{
		private:
			void Load(const string &_fileName);

			ILuint imageID_;
			bool mipmapBuilt_;
			std::string fileName_;
			int width_;
			int height_;
			Reference<GLTexture> texture_;			

		public:
			ILImage(const string &_fileName);
			virtual ~ILImage();			

			inline const char *get_file_name()
			{
				return fileName_.c_str();
			}

			void build_mipmap();
			void bind();
			GLTexture *get_texture();

			////////////////
			// Properties //
			////////////////

			inline int get_image_id()
			{
				return imageID_;
			}			
		};
	}
}

#pragma warning(pop)

#endif