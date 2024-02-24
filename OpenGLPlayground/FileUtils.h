#pragma once

#include <string>
#ifdef _WIN32
#include <io.h> 
#define access    _access_s
#else
#include <unistd.h>
#endif

namespace lemon {
	bool fileExists(const std::string& source);
}
