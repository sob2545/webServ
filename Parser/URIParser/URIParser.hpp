#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <exception>
#include <cstring>

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

struct URIvalue {
	std::string		scheme;
	std::string		host;
	unsigned short	port;
};