#ifndef LOG_H
#define LOG_H

#include <iostream>

namespace log {
    void setLevel(std::string level);
    void status(std::string message);
    void error(std::string message);
    void warn(std::string message);
    void debug(std::string message);
}

#endif // LOG_H