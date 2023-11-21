#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <exception>
#include <cstring>

enum Reserved {
	R_SEMICOLON = ';',
	R_SLASH = '/',
	R_QUESTION_MARK = '?',
	R_COLON = ':',
	R_AT_SIGN = '@',
	R_AMPERSAND = '&',
	R_EQUALS = '=',
	R_PLUS = '+',
	R_DOLLAR_SIGN = '$',
	R_COMMA = ','
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

enum Pchar {
	P_COLON = ':',
	P_AT_SIGN = '@',
	P_AMPERSAND = '&',
	P_PLUS = '+',
	P_DOLLAR_SIGN = '$',
	P_COMMA = ','
};

struct URIvalue {
	unsigned short	port;
	std::string		scheme;
	std::string		host;
	std::string		absPath;
};