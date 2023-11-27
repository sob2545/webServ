#pragma once

#include "../../utils/utilFunctions.hpp"
#include <vector>
#include <map>

namespace URI {
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

	typedef	std::map<std::string, std::vector<std::string> > QueryMap;
	typedef	std::vector<std::string>	PathVec;

	struct data {
		unsigned short	port;
		std::string     host;
		std::string		fragment;
		PathVec			absPath;
		QueryMap		query;
	};
}


namespace URIParser {
	bool		isValidURI(const std::string& inputURI, std::string& host);
	URI::data	Parser(const std::string& inputURI);
}

