#ifndef LOG_H
#define LOG_H

#include <iostream>

namespace Log {
	enum Level : unsigned char {Status, Error, Warn, Debug};
	void setLevel(Level level);
	void enableColor(bool c);
	void status(std::string message);
	void error(std::string message);
	void warn(std::string message);
	void debug(std::string message);
	std::string to_hex_string(uint32_t num, bool prefix = true);
}

#endif // LOG_H