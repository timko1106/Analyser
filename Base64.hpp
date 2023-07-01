#ifndef BASE64_H
#define BASE64_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Vars.hpp"

namespace base64 {
	char* encode(const char* msg, size_t size = FULL);
	char* decode(const char* msg, size_t size = FULL);
}


#endif