#include <mio/gl/gl_exception.h>

namespace mio
{
    namespace gl
    {
        GLException::GLException(const char *message)
        {
			message_ = message;
        }

        GLException::~GLException()
        {
        }

		const char* GLException::what() const throw()
		{
			return message_.c_str();
		}
    }
}
