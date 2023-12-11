#include "MIMEFile.hpp"

MIME::MIMEFile*	MIME::MIMEFile::instance = NULL;

void	MIME::MIMEFile::InitInstance(const std::string &fileName) {
	if (!MIME::MIMEFile::instance) {
		MIME::MIMEFile::instance = new MIME::MIMEFile(fileName);
	}
}

MIME::MIMEFile*	MIME::MIMEFile::getInstance() {
	return MIME::MIMEFile::instance;
}

MIME::MIMEFile::MIMEFile(const std::string& fileName): ReadFile(fileName) {
	m_Pos[E_INDEX::FILE] = 0;
	m_Pos[E_INDEX::LINE] = 1;
	m_Pos[E_INDEX::COLUMN] = 1;
}

MIME::MIMEFile::~MIMEFile() {
}

void	MIME::MIMEFile::Destroy() {
	delete MIME::MIMEFile::instance;
	MIME::MIMEFile::instance = NULL;
}

std::size_t*	MIME::MIMEFile::Pos() {
	return m_Pos;
}