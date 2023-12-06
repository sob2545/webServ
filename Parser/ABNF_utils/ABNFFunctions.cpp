#include "ABNFFunctions.hpp"

bool	ABNF::isLF(const std::string& file, const size_t& pos) {
	return ((file[pos] == E_ABNF::LF) ? true : false);
}

bool	ABNF::isWSP(const std::string& file, size_t& pos) {
	switch (file.at(pos)) {
		case (E_ABNF::HTAB):
		case (E_ABNF::SP):
			return true;
		default :
			return false;
	}
}

bool	ABNF::isComment(const std::string& file, size_t& pos) {
	if (file.at(pos) == E_ABNF::SEMICOLON) {
		pos++;
	} else {
		return false;
	}
	// isprint()만 사용하면 알파벳 이외의 한글을 읽지 못함
	while (pos < file.size() && file[pos] != E_ABNF::LF) {
		pos++;
	}
	return (isLF(file, pos));
}

bool	ABNF::isC_nl(const std::string& file, size_t& pos) {
	if (isComment(file, pos) || isLF(file, pos)) {
		pos++;
		return true;
	} else {
		return false;
	}
}