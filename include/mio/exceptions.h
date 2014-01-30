#ifndef __MIO_EXCEPTIONS_H__
#define __MIO_EXCEPTIONS_H__

#include <mio/mio.h>
#include <exception>
#include <string>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace mio
{
	using namespace std;

	class MIO_API std::exception;

	class MIO_API NotImplementedException : public std::exception
	{
		virtual const char* what() const throw()
		{
			return "Not implmented yet!";
		}
	};

	class MIO_API IndexException : public std::exception
	{
	private:
		string message_;
    public:
        IndexException(const char *message)
		{
			message_ = message;
		}

		virtual ~IndexException() {}

		virtual const char* what() const throw()
		{
			return message_.c_str();
		}
	};

	class MIO_API TypeException : public std::exception
	{
	private:
		string message_;
    public:
        TypeException(const char *message)
		{
			message_ = message;
		}

		virtual ~TypeException() {}

		virtual const char* what() const throw()
		{
			return message_.c_str();
		}
	};

	class MIO_API ValueException : public std::exception
	{
	private:
		string message_;
    public:
        ValueException(const char *message)
		{
			message_ = message;
		}

		virtual ~ValueException() {}

		virtual const char* what() const throw()
		{
			return message_.c_str();
		}
	};
}

#pragma warning(pop)

#endif