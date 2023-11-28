#include "utilABNFFunctions.hpp"

bool	isLF(const std::string& file, size_t& pos) {
	return ((file[pos] == ABNF::LF) ? true : false);
}

bool	isWSP(const std::string& file, size_t& pos) {
	switch (file.at(pos)) {
		case (ABNF::HTAB):
		case (ABNF::SP):
		case (ABNF::LF):
			return true;
		default :
			return false;
	}
}

bool	isComment(const std::string& file, size_t& pos) {
	if (file.at(pos) == ABNF::SEMICOLON) {
		pos++;
	} else {
		return false;
	}
	while (pos < file.size() &&  (isWSP(file, pos) || isprint(static_cast<int>(file.at(pos))))) {
		pos++;
	}
	isLF(file, pos);
}

bool	isC_nl(const std::string& file, size_t& pos) {
	if (isComment(file, pos) || isLF(file, pos)) {
		pos++;
		return true;
	} else {
		return false;
	}
}