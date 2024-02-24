#pragma once
#include <string>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace lemon {

	std::string getCurrentDate();
	bool startsWith(std::string str, std::string substr);

}