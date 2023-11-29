#pragma once

#include "../../utils/utilFunctions.hpp"
#include <vector>
#include <map>

namespace URI {
	namespace E_RESERVED {
		enum E_RESERVED {
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

	namespace E_MARK {
		enum E_MARK {
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

	namespace E_PCHAR {
		enum E_PCHAR {
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
	bool		absPath(const std::string& inputURI, size_t& pos, std::vector<std::string>& absPath);
	URI::data	Parser(const std::string& inputURI);
}

