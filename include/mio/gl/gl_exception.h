#ifndef __MIO_GL_GL_EXCEPTION_H__
#define __MIO_GL_GL_EXCEPTION_H__

#include <mio/mio.h>
#include <exception>
#include <string>

using namespace std;

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace gl
    {
		class MIO_API std::exception;

		class MIO_API GLException : public std::exception
        {
		private:
			string message_;
        public:
            GLException(const char *message);
            virtual ~GLException();
			virtual const char* what() const throw();
        };
    }
}

#pragma warning(pop)

#endif