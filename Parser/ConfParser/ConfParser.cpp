#include "ConfParser.hpp"

bool	isLF(const std::string& file, size_t& pos) {
	return ((file[pos] == CONF::LF) ? true : false);
}

bool	isWSP(const std::string& file, size_t& pos) {
	switch (file.at(pos)) {
		case (CONF::HTAB):
		case (CONF::SP):
		case (CONF::LF):
			return true;
		default :
			return false;
	}
}

bool	comment(const std::string& file, size_t& pos) {
	Utils::compareOneCharacter(file, pos, CONF::SEMICOLON);
	
}

bool	c_nl(const std::string& file, size_t& pos) {
	comment(file, pos) || isLF(file, pos);
}


void	contextLines(const std::string& file, size_t& pos, CONF::main& main_block) {
	while (pos < file.size() && isWSP(file, pos)) {
		context(file, pos, main_block) || c_nl(file, pos);
	}
}

CONF::main	Parser(const std::string& file) {
	CONF::main  main_block;
	size_t  pos(0);

	contextLines(file, pos, main_block);
}

