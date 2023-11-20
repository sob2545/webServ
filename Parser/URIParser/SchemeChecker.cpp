#include "SchemeChecker.hpp"

SchemeChecker::SchemeChecker() {
    m_refScheme.push_back("http");
    m_refScheme.push_back("https");
    m_refScheme.push_back("ftp");
}

SchemeChecker::~SchemeChecker() {}

bool    SchemeChecker::isValidScheme(const std::string& target) {
    return std::find(m_refScheme.begin(), m_refScheme.end(), target) != m_refScheme.end();
}