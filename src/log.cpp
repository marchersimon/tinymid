#include <iostream>

namespace log {
	namespace {
		int logLevel = 2;
		// 0 ... Status
		// 1 ... Error
		// 2 ... Warnings
		// 3 ... Debug
		bool color = true;
	}

	void setLevel(std::string level) {
		if(level == "status") {
			logLevel = 0;
		} else if (level == "error") {
			logLevel = 1;
		} else if (level == "warn") { // default
			logLevel = 2;
		} else if (level == "debug") {
			logLevel = 3;
		}
	}

	void enableColor(bool c) {
		color = c;
	}

	void status(std::string message) {
		std::cout << message << std::endl;
	}

	void error(std::string message) {
		if(logLevel >= 1) {
			if(color) std::cout << "\033[;31m"; // red
			std::cout <<  message;
			if(color) std::cout << "\033[1;m";
			std::cout << std::endl;
		}
	}

	void warn(std::string message) {
		if(logLevel >= 2) {
			if(color) std::cout << "\033[;33m"; // orange
			std::cout <<  message;
			if(color) std::cout << "\033[1;m";
			std::cout << std::endl;
		}
	}

	void debug(std::string message) {
		if(logLevel >= 3) {
			if(color) std::cout << "\033[;90m"; // grey
			std::cout <<  message;
			if(color) std::cout << "\033[1;m";
			std::cout << std::endl;
		}
	}

	std::string hex_to_string(int num) {
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