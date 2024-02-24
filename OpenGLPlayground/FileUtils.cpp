#include "FileUtils.h"

namespace lemon {

	bool fileExists(const std::string& source) {
		return access(source.c_str(), 0) == 0;
	}
}
