#pragma once

#include <iostream>

namespace Log {

	enum Level : unsigned char {
		Status = 0,
		Error = 1,
		Warn = 2,
		Debug = 3
	};

	namespace {
		Level logLevel = Warn;
		bool color = true;
	}

	void setLevel(Level level);
	void enableColor(bool c);
	void status(std::string message);
	void error(std::string message);
	void warn(std::string message);
	void debug(std::string message);
	std::string to_hex_string(uint32_t num, bool prefix = true);
}