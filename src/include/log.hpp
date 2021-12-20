#pragma once

#include <iostream>
#include <vector>
#include <exception>

#include "event.hpp"
#include "midi.hpp"

using std::string;
using std::to_string;
using std::vector;

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
	void status(const string & message);
	void error(const string & message);
	void warn(const string & message);
	void debug(const string & message);
	string to_hex_string(const uint32_t & num, bool prefix);

	void debugEvent(Event event, int startPos, int endPos, const vector<uint8_t> & file);
	void debugEvent(Event event);

	string addPadding(string str, int width);
}