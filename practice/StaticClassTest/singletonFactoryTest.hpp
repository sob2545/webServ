#pragma once

#include <iostream>

template<typename T>
class Singleton {
public:
	static T& instance(int a) {
		static T _instance(a);
		std::cout << a << " " << _instance.getVar() << std::endl;
		return _instance;
	}

	static T& instance() {
		static T _instance;
		return _instance;
	}
};