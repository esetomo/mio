#ifndef __MIO_UTILITY_PATH_H__
#define __MIO_UTILITY_PATH_H__

#include <mio/mio.h>
#include <string>

using namespace std;

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
	namespace utility
	{
		class MIO_API Path
		{
		private:
			string currentWorkingDirectory;

			Path();
			static Path *instance;

		public:
			static Path* v();

			string get_cwd();
			string convert_to_unix_path(const string &path);
			bool is_absolute_path(const string &path);
			string join(const string &path1, const string &path2);
			string get_absolute_path(const string &path);
			string normalize_path(const string &path);
			string get_directory_name(const string &path);
		};
	}
}

#pragma warning(pop)

#endif