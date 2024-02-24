#pragma once
#include <string>
#include <fstream>
#include <iostream>

namespace lemon {

	enum log_level {
		FATAL,
		ERROR,
		WARN,
		INFO,
		DEBUG,
		TRACE
	};
	std::string logLevelToString(log_level level);

	class Logger
	{
	private:
		std::fstream logFile;
		std::string filename;

		Logger() {};
	public:
		static Logger& getInstance();

		Logger(Logger const&) = delete; // Copy constructor is private for
		void operator=(Logger const&) = delete;

		void init();
		void terminate();
		void log(const std::string& message, log_level level);
		inline void FATAL(const std::string& message) {
			log(message, log_level::FATAL);
		};
		inline void ERROR(const std::string& message) {
			log(message, log_level::ERROR);
		};
		inline void WARN(const std::string& message) {
			log(message, log_level::WARN);
		};
		inline void INFO(const std::string& message) {
			log(message, log_level::INFO);
		};
		inline void DEBUG(const std::string& message) {
			log(message, log_level::DEBUG);
		};
		inline void TRACE(const std::string& message) {
			log(message, log_level::TRACE);
		};

		std::string& getFilename();
	};

}

