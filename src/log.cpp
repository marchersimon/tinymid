#include <iostream>

namespace log {
    namespace {
        static int logLevel = 2;
        // 0 ... Status
        // 1 ... Error
        // 2 ... Warnings
        // 3 ... Debug
    }

    void setLevel(std::string level) {
    if(level == "status") {
        logLevel = 0;
    } else if (level == "error") {
        logLevel = 1;
    } else if (level == "warn") {
        logLevel = 2;
    } else if (level == "debug") {
        logLevel = 3;
    }
}

    void status(std::string message) {
        std::cout << message << std::endl;
    }

    void error(std::string message) {
        if(logLevel >= 1) {
            std::cout << message << std::endl;
        }
    }

    void warn(std::string message) {
        if(logLevel >= 2) {
            std::cout << message << std::endl;
        }
    }

    void debug(std::string message) {
        if(logLevel >= 3) {
            std::cout << message << std::endl;
        }
    }
}