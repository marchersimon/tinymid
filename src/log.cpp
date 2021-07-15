#include "log.h"

void Log::setLevel(Level level) {
	logLevel = level;
}

void Log::enableColor(bool c) {
	color = c;
}

void Log::status(std::string message) {
	std::cout << message << std::endl;
}

void Log::error(std::string message) {
	if(logLevel >= Error) {
		if(color) std::cout << "\033[;31m"; // red
		std::cout <<  message;
		if(color) std::cout << "\033[1;m";
		std::cout << std::endl;
	}
}

void Log::warn(std::string message) {
	if(logLevel >= Warn) {
		if(color) std::cout << "\033[;33m"; // orange
		std::cout <<  message;
		if(color) std::cout << "\033[1;m";
		std::cout << std::endl;
	}
}

void Log::debug(std::string message) {
	if(logLevel >= Debug) {
		if(color) std::cout << "\033[;90m"; // grey
		std::cout <<  message;
		if(color) std::cout << "\033[1;m";
		std::cout << std::endl;
	}
}

std::string Log::to_hex_string(uint32_t num, bool prefix) {
	char s[9];
	sprintf(s, "%X", num);
	std::string str = s;
	if(str.size() % 2) {
		str.insert(0, "0");
	}
	if(prefix) {
		str.insert(0, "0x");
	}
	return str;
}