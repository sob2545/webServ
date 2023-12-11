#pragma once

#include "../ConfParser/Parser/Exception/ConfParserException.hpp"
#include "../../FileDescriptor/File/ReadFile.hpp"
#include "../ABNF_utils/ABNFFunctions.hpp"
#include "./Exception/MIMEParserException.hpp"

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
	
	template <typename T>
	const std::string	type(const std::string& fileName, std::size_t& Pos);

	template <typename T>
	void				Parser(const std::string& fileName, TypeMap& MIMEType);
}