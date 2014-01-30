#ifndef __MIO_UTILITY_STRING_H__
#define __MIO_UTILITY_STRING_H__

#include <mio/mio.h>
#include <vector>
#include <string>

std::string strip(const std::string &s);
std::vector<std::string> split(const std::string &s, char delim);

#endif