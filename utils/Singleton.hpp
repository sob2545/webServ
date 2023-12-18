#pragma once

#include <string>

/**
 * @brief singleton
 * @details singleton을 상속 후, instance 함수를 직접호출하여 싱글톤관리를 할 수 있다.
            e.g. Class::instance().memberFunction();
 * @author chanhyle
 * @date 2023.11.20
 */


template<typename T>
class Singleton {
public:
    static T& instance(void) {
		static T _s_instance;
        return (_s_instance);
    }

    static T& instance(const std::string& fileName, char** env) {
        static T _s_instance(fileName, env);
		return (_s_instance);
    }
};
