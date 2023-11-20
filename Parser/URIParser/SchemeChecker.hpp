#pragma once
#include "Singleton.hpp"
#include <vector>
#include <string>
#include <algorithm>

/**
 * @brief schemechecker
 * @details URI scheme 리스트를 가지고 있고, 유효한 scheme을 검사하는 클래스
 * @author chanhyle
 * @date 2023.11.20
 */
class SchemeChecker : public Singleton<SchemeChecker> {
private:
    std::vector<std::string>    m_refScheme;

    SchemeChecker(const SchemeChecker& other);
    SchemeChecker&  operator=(const SchemeChecker& other);

public:
    SchemeChecker();
    ~SchemeChecker();
    
    bool    isValidScheme(const std::string& target);
};