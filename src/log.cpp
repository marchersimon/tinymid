#include <iostream>

namespace Log {

	enum Level : unsigned char {
		Status, Error, Warn, Debug
	};

	namespace {
		Level logLevel = Warn;
		// 0 ... Status
		// 1 ... Error
		// 2 ... Warnings
		// 3 ... Debug
		bool color = true;
	}

	void setLevel(Level level) {
		logLevel = level;
	}

	void enableColor(bool c) {
		color = c;
	}

	void status(std::string message) {
		std::cout << message << std::endl;
	}

	void error(std::string message) {
		if(logLevel >= Error) {
			if(color) std::cout << "\033[;31m"; // red
			std::cout <<  message;
			if(color) std::cout << "\033[1;m";
			std::cout << std::endl;
		}
	}

	void warn(std::string message) {
		if(logLevel >= Warn) {
			if(color) std::cout << "\033[;33m"; // orange
			std::cout <<  message;
			if(color) std::cout << "\033[1;m";
			std::cout << std::endl;
		}
	}

	void debug(std::string message) {
		if(logLevel >= Debug) {
			if(color) std::cout << "\033[;90m"; // grey
			std::cout <<  message;
			if(color) std::cout << "\033[1;m";
			std::cout << std::endl;
		}
	}

	std::string hex_to_string(std::uint32_t num) {
		char s[9];
		sprintf(s, "%X", num);
		std::string str = s;
		if(str.size() % 2) {
			str.insert(0, "0");
		}
		str.insert(0, "0x");
		return str;
	}
}