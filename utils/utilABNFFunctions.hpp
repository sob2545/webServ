#pragma once

#include <string>


namespace ABNF {
	enum SpecialLetter{
		CR = '\r',
		LF = '\n',
		HTAB = '\t',
		SP = ' ',
		UNDERSCORE = '_',
		DQUOTE = '\"',
		SEMICOLON = ';'
	};
    
    bool	isLF(const std::string& file, size_t& pos);
    bool	isWSP(const std::string& file, size_t& pos);
    bool	isComment(const std::string& file, size_t& pos);
    bool	isC_nl(const std::string& file, size_t& pos);
}