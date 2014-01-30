#ifndef __MIO_H__
#define __MIO_H__

#ifdef MIO_DLL

#ifndef MIO_API
#ifdef MIO_EXPORTS
#define MIO_API __declspec(dllexport)
#else
#define MIO_API __declspec(dllimport)
#endif
#endif

#else

#ifndef MIO_API
#define MIO_API
#endif

#endif

#endif
