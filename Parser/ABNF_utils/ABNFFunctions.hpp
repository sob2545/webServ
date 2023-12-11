#pragma once

#include <string>

namespace E_ABNF {
	enum E_SPECIAL_LETTERS{
		CR = '\r',
		LF = '\n',
		HTAB = '\t',
		SP = ' ',
		UNDERSCORE = '_',
		DQUOTE = '\"',
		SEMICOLON = ';'
	};
}

namespace ABNF {
    bool	isLF(const std::string& file, const std::size_t& pos);
    bool	isWSP(const std::string& file, std::size_t& pos);
    bool	isComment(const std::string& file, std::size_t& pos);
    bool	isC_nl(const std::string& file, std::size_t& pos);
}