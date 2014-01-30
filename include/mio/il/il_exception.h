#ifndef __MIO_IL_IL_EXCEPTION_H__
#define __MIO_IL_IL_EXCEPTION_H__

#include <mio/mio.h>
#include <exception>
#include <string>

using namespace std;

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
    namespace il
    {
		class MIO_API std::exception;

		class MIO_API ILException : public std::exception
        {
		private:
			string message_;
        public:
            ILException(const char *message);
            virtual ~ILException();
			virtual const char* what() const throw();
        };
    }
}

#pragma warning(pop)

#endif