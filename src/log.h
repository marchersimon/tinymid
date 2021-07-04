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
	std::string hex_to_string(int num);
}

#endif // LOG_H