#pragma once

#include "../ConfParser/Parser/Exception/ConfParserException.hpp"
#include "../../FileDescriptor/File/ReadFile.hpp"
#include "../ABNF_utils/ABNFFunctions.hpp"

#include <string>
#include <vector>
#include <map>

namespace E_MIME {
	enum E_MIME {
		SLASH = '/',
		SEMICOLON = ';',
		PERIOD = '.',
		HYPHEN = '-',
		PLUS = '+',
		LF = '\n',
		LBRACE = '{',
		RBRACE = '}'
	};
}

namespace MIME {
	typedef std::map<std::string, std::vector<std::string> >	TypeMap;
	
	const std::string	type(const std::string& fileName, size_t& Pos);
	void				Parser(const std::string& fileName, TypeMap& MIMEType);
}