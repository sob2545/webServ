#include <iostream>
#include "SmartPointer.hpp"
#include <sys/_types/_pid_t.h>
#include <unistd.h>
#include <map>
#include <string>
#include <sys/wait.h>

class UniqueTest {
public:
    UniqueTest() { std::cout << "Test Created\n"; }
    ~UniqueTest() { std::cout << "Test Destroyed\n"; }
    void hello() { std::cout << "Hello from Test\n"; }
};


class SharedTest {
public:
    ft::weak_ptr<SharedTest> other;

    SharedTest() { std::cout << "Test Created\n"; }
    ~SharedTest() { std::cout << "Test Destroyed\n"; }
};

class tmp {
public:
    int a = 10;
    std::string b = "hello";

    const int&  getA() const {return a;}
    const std::string& getB() const {return b;}
	void		setB(const std::string& c) {b = c;}
};

void	leakTester() {
	system("leaks a.out");
}

void    mapLeakTester() {
    ft::shared_ptr<tmp> a(new tmp());
    // std::map<std::string, ft::shared_ptr<tmp> > testMap;
    // testMap.insert(std::make_pair("1", a));
	// std::cout << a.use_count() << std::endl;
    // testMap.insert(std::make_pair("2", a));
	// std::cout << a.use_count() << std::endl;
    // testMap.insert(std::make_pair("3", a));
	// std::cout << a.use_count() << std::endl;

	const pid_t	tmpPid = fork();
	
	if (tmpPid == 0) {
		a = ft::shared_ptr<tmp>(new tmp());		
		a->setB("fork test");
		std::cout << a->getB() << std::endl;
	} else {
		wait(NULL);
		std::cout << a.use_count() << std::endl;
		std::cout << a->getB() << std::endl;
	}

	atexit(leakTester);

    // std::cout << testMap["1"]->getB() << std::endl;
}

int main() {

	{
        ft::unique_ptr<UniqueTest> uPtr(new UniqueTest());
        uPtr->hello();
    }

    std::cout << "unique_ptr test completed\n";

    {
        ft::shared_ptr<SharedTest> a(new SharedTest());
        ft::shared_ptr<SharedTest> b(new SharedTest());

        a->other = b;
        b->other = a;
    }

    std::cout << "Circular reference created (potential memory leak)\n";

    // {
    //     ft::shared_ptr<tmp> a(new tmp());
    //     std::map<std::string, ft::shared_ptr<tmp> > testMap;
    //     testMap.insert(std::make_pair("1", a));
    //     testMap.insert(std::make_pair("2", a));
    //     testMap.insert(std::make_pair("3", a));

    //     std::cout << testMap["1"]->getB() << std::endl;
    // }

	mapLeakTester();

    return 0;
}