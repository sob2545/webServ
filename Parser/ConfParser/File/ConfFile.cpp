#include "ConfFile.hpp"

CONF::ConfFile*	CONF::ConfFile::instance = NULL;

void	CONF::ConfFile::InitInstance(const std::string &fileName) {
	if (!CONF::ConfFile::instance) {
		CONF::ConfFile::instance = new CONF::ConfFile(fileName);
	}
}

CONF::ConfFile*	CONF::ConfFile::getInstance() {
	return CONF::ConfFile::instance;
}

CONF::ConfFile::ConfFile(const std::string& fileName): ReadFile(fileName) {
	m_Pos[E_INDEX::FILE] = 0;
	m_Pos[E_INDEX::LINE] = 1;
	m_Pos[E_INDEX::COLUMN] = 1;
}

CONF::ConfFile::~ConfFile() {
}

void	CONF::ConfFile::Destroy() {
	delete CONF::ConfFile::instance;
	CONF::ConfFile::instance = NULL;
}

std::size_t*	CONF::ConfFile::Pos() {
	return m_Pos;
}