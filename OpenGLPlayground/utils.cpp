#include "utils.h"
#include "Logger.h"
#include <boost/algorithm/string.hpp> 

namespace lemon {

	std::string getCurrentDate() {
		std::chrono::time_point t = std::chrono::system_clock::now();
		time_t t_time_t = std::chrono::system_clock::to_time_t(t);

		std::tm localTime;
		localtime_s(&localTime, &t_time_t);
		
		std::stringstream ss;
		ss << std::put_time(&localTime, "%Y-%m-%d %H-%M-%S");
		return ss.str();
	}


	bool startsWith(std::string str, std::string substr) {
		if (str.empty() || substr.empty()) {
			return false;
		}

		if (str.size() < substr.size())
			return false;

		for (int i = 0; i < substr.size(); i++) {
			if (str[i] != substr[i]) {
				return false;
			}
		}
		return true;
	}

}
