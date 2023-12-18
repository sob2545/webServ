#pragma once

#include "singletonFactoryTest.hpp"

class TestClass : public Singleton<TestClass>{
private:
	static int	m_var;

public:
	TestClass(int a) {m_var = a;}
	~TestClass() {};

	static const int& getVar() {return m_var;}
};

int TestClass::m_var;


class nonParamClass : public Singleton<nonParamClass> {
private:
	static int m_var;

public:
	nonParamClass() {m_var = 10;}
	~nonParamClass() {};

	static const int& getVar() {return m_var;}
};

int	nonParamClass::m_var;