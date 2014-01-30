#include <mio/il/il_exception.h>

namespace mio
{
    namespace il
    {
        ILException::ILException(const char *message)
        {
			message_ = message;
        }

        ILException::~ILException()
        {
        }

		const char* ILException::what() const throw()
		{
			return message_.c_str();
		}
    }
}
