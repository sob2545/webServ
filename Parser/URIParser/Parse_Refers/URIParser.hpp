#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <exception>
#include <cstring>

namespace Reserved {
	enum Reserved {
		SEMICOLON = ';',
		SLASH = '/',
		QUESTION_MARK = '?',
		COLON = ':',
		AT_SIGN = '@',
		AMPERSAND = '&',
		EQUALS = '=',
		PLUS = '+',
		DOLLAR_SIGN = '$',
		COMMA = ','
	};
}

namespace Mark {
	enum Mark {
		HYPHEN = '-',
		UNDERSCORE = '_',
		PERIOD = '.',
		EXCLAMATION_MARK = '!',
		TILDE = '~',
		ASTERISK = '*',
		SINGLE_QUOTE = '\'',
		LEFT_PARENTHESIS = '(',
		RIGHT_PARENTHESIS = ')'
	};
}

namespace Pchar {
	enum Pchar {
		COLON = ':',
		AT_SIGN = '@',
		AMPERSAND = '&',
		PLUS = '+',
		DOLLAR_SIGN = '$',
		COMMA = ','
	};
}

typedef	std::map<std::string, std::vector<std::string> > QueryMap_t;
typedef	std::vector<std::string>	PathVec_t;

struct URIvalue {
	unsigned short	port;
	std::string		scheme;
	std::string		host;
	std::string		fragment;
	PathVec_t			absPath;
	QueryMap_t		query;
};

