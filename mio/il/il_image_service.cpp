#include <mio/il/il_image_service.h>
#include <mio/utility/path.h>
#include <mio/il/il_exception.h>
#include <sstream>
#include <assert.h>

using namespace std;

namespace mio
{
	namespace il
	{
		using namespace mio::utility;

		ILImageService *ILImageService::instance_ = 0;
		bool ILImageService::initialized_ = false;

		ILImageService::ILImageService()
		{
		}

		ILImageService::~ILImageService()
		{
		}

		void ILImageService::init()
		{
			if (!initialized_)
			{
				ilInit();
				iluInit();

				if (instance_ == 0)
					instance_ = new ILImageService();

				initialized_ = true;
			}
		}

		ILImageService *ILImageService::v()
		{
			init();
			return instance_;
		}

		void ILImageService::create_image(const string &filename)
		{
			Reference<ILImage> newImage = new ILImage(filename);
			by_file_name_[filename] = newImage;
			by_id_[newImage->get_image_id()] = newImage;
		}

		bool ILImageService::internal_has_image_with_file_name(const string &filename)
		{
			return !(by_file_name_.find(filename) == by_file_name_.end());
		}

		bool ILImageService::has_image_with_file_name(const string &filename)
		{
			string path = Path::v()->get_absolute_path(filename);
			return internal_has_image_with_file_name(path);
		}

		bool ILImageService::has_image_with_id(ILuint id)
		{
			return !(by_id_.find(id) == by_id_.end());
		}

		ILImage *ILImageService::get_image(const string &filename)
		{
			string path = Path::v()->get_absolute_path(filename);
			if (!internal_has_image_with_file_name(path))
				create_image(path);
			return by_file_name_[path].get_pointer();
		}

		ILImage *ILImageService::get_image(ILuint id)
		{
			if (!has_image_with_id(id))
			{
				stringstream ss;
				ss << "ILException: there is no image with id=" << id;
				throw ILException(ss.str().c_str());
			}
			else
				return by_id_[id].get_pointer();
		}

		void ILImageService::purge(ILuint id)
		{
			Reference<ILImage> image = by_id_[id];
			by_file_name_.erase(image->get_file_name());
			by_id_.erase(id);
		}

		void ILImageService::remove(const string &filename)
		{
			string path = Path::v()->get_absolute_path(filename);
			if (internal_has_image_with_file_name(path))
			{
				Reference<ILImage> image = by_file_name_[path];
				purge(image->get_image_id());
			}
			else
			{
				stringstream ss;
				ss << "ILException: there is no image with filename=" << filename;
				throw ILException(ss.str().c_str());
			}
		}

		void ILImageService::remove(ILuint id)
		{
			if (has_image_with_id(id))
				purge(id);
			else
			{
				stringstream ss;
				ss << "ILException: there is no image with id=" << id;
				throw ILException(ss.str().c_str());
			}
		}

		void ILImageService::remove(ILImage *image)
		{
			assert(image);
			if (image != NULL && has_image_with_id(image->get_image_id()))
				purge(image->get_image_id());
			else
			{
				stringstream ss;
				ss << "ILException: there is no image with id=" << image->get_image_id();
				throw ILException(ss.str().c_str());
			}
		}
	}
}