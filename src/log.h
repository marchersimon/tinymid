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

	void setLevel(const Level & level);
	void enableColor(const bool & c);
	void status(const std::string & message);
	void error(const std::string & message);
	void warn(const std::string & message);
	void debug(const std::string & message);
	std::string to_hex_string(const uint32_t & num, const bool & prefix = true);
}