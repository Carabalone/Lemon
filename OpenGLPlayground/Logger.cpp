#include "Logger.h"
#include "utils.h"

namespace lemon {

	std::string logLevelToString(log_level level) {
		switch (level) {
		case FATAL:
			return "FATAL";
		case ERROR:
			return "ERROR";
		case WARN:
			return "WARN";
		case INFO:
			return "INFO";
		case DEBUG:
			return "DEBUG";
		case TRACE:
			return "TRACE";
		}	
		
		return "UNKNOWN";
	}
	
	std::string& Logger::getFilename() {
		return filename;
	}

	void Logger::init() {
		std::string filename = "logs/" + getCurrentDate() + " Lemon.log";
		this->filename = filename;
		logFile.open(filename, std::ios::out);
		
		if (!logFile) {
			std::cerr << "Couldn't create log file with name " << filename << "\n terminating." << std::endl;
		}
	}

	void Logger::log(const std::string& message, log_level level) {
		if (logFile.is_open()) {
			logFile << "[" << logLevelToString(level) << "] " << message << std::endl;
			std::cout << "[" << logLevelToString(level) << "] " << message << std::endl;
			return;
		}
		std::cerr << "couldn't write to log file " << filename << std::endl;
	}

	void Logger::terminate() {
		if (logFile) {
			logFile.close();
		}
	}

	Logger& Logger::getInstance() {
		static Logger instance;

		if (!instance.logFile.is_open()) {
			instance.init();
		}
		return instance;
	}

}
