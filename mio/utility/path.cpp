#include <mio/utility/path.h>
#include <mio/utility/string.h>
#include <direct.h>
#include <stdlib.h>

namespace mio
{
	namespace utility
	{
		Path::Path()
		{
			char *cwd = _getcwd(NULL, 0);
			if (cwd == NULL)
			{
				fprintf(stderr, "Fatal Error: Cannot get the current working directory.\n");
				exit(1);
			}

			currentWorkingDirectory = cwd;
			currentWorkingDirectory = convert_to_unix_path(currentWorkingDirectory);

			free(cwd);
		}

		Path *Path::instance = NULL;

		Path *Path::v()
		{
			if (instance == NULL)			
				instance = new Path();
			return instance;			
		}

		string Path::get_cwd()
		{
			return currentWorkingDirectory;
		}

		string Path::convert_to_unix_path(const string &path)
		{
			string result = path;
			size_t pos;
			while ((pos = result.find("\\")) != string::npos)
				result.replace(pos, 1, "/");
			return result;
		}

		bool Path::is_absolute_path(const string &_path)
		{
			string path = convert_to_unix_path(_path);			
			return (path[1] == ':' && path[2] == '/') || (path[0] == '/');
		}

		string Path::join(const string &path1, const string &path2)
		{
			return convert_to_unix_path(path1) + '/' + convert_to_unix_path(path2);
		}

		string Path::normalize_path(const string &_path)
		{
			if (_path.length() == 0)
				return "";
			string path = convert_to_unix_path(_path);
			
			vector<string> components;

			bool absolute = false;
			if (path[0] == '/')
			{
				components.push_back("c:");
				absolute = true;
			}
			
			vector<string> exploded = split(path, '/');
			vector<string>::iterator ii;
			for(ii=exploded.begin();ii!=exploded.end();ii++)
			{
				if (ii->length() == 0 || *ii == ".")
					continue;
				else if (*ii == "..")
				{
					if (!absolute && components.size() == 0)
						components.push_back("..");
					else if (!absolute && components.size() > 0 && components[components.size()-1] == "..")
						components.push_back("..");
					else if ((absolute && components.size() > 1) ||
						(!absolute && components.size() > 0))
						components.pop_back();
					else if (absolute && components.size() == 1)
						continue;
				}
				else
					components.push_back(*ii);
			}
			
			string result = "";
			int count = 0;			
			for(ii=components.begin();ii!=components.end();ii++)
			{
				result += *ii;
				if (count != components.size()-1)
					result += "/";
				count++;
			}
			if (absolute && components.size() == 1)
				result += "/";

			return result;
		}

		string Path::get_absolute_path(const string &_path)
		{
			string path = convert_to_unix_path(_path);
			if (path.length() == 0)
				return currentWorkingDirectory;
			else if (path[0] == '/')
				return normalize_path("c:" + path);
			else if (is_absolute_path(path))
				return normalize_path(path);
			else
				return normalize_path(currentWorkingDirectory + "/" + path);
		}

		string Path::get_directory_name(const string &_path)
		{
			string path = convert_to_unix_path(_path);
			size_t position = path.rfind('/');
			if (position == string::npos)
				return "";
			path = path.substr(0, position);
			int beforeSlash = (int)path.length()-1;
			while (beforeSlash >= 0 && path[beforeSlash] == '/')
				beforeSlash--;
			if (beforeSlash < 0)
				return "/";
			else
				return path.substr(0, beforeSlash+1);
		}
	}
}