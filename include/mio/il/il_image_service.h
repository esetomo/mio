#ifndef __MIO_SERVICE_IMAGE_SERVICE_H__
#define __MIO_SERVICE_IMAGE_SERVICE_H__

#include <mio/mio.h>
#include <windows.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <map>
#include <string>
#include <mio/il/il_image.h>
#include <mio/reference.h>

#pragma warning(push)
#pragma warning(disable : 4251)

using namespace std;

namespace mio
{
	namespace il
	{		
		class MIO_API ILImageService
		{
		private:
			ILImageService();			
			void purge(ILuint id);
			void create_image(const string &filename);

			static bool initialized_;
			static ILImageService *instance_;
			map< string, Reference<ILImage> > by_file_name_;
			map< ILuint, Reference<ILImage> > by_id_;

			bool internal_has_image_with_file_name(const string &filename);

		public:
			virtual ~ILImageService();

			ILImage *get_image(const string &filename);
			ILImage *get_image(ILuint id);

			bool has_image_with_file_name(const string &filename);
			bool has_image_with_id(ILuint id);

			void remove(const string &filename);
			void remove(ILuint id);
			void remove(ILImage *ptr);

			static ILImageService *v();
			static void init();
		};
	}
}

#pragma warning(pop)

#endif