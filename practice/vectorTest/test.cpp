#include <iostream>
#include <string>
#include <vector>

struct tmp {
	int	var;
};

class Class1 {
private:
	struct tmp	mVar;

public:
	Class1() {}
	Class1(const struct tmp& other) {
		this->mVar.var = other.var;
	}
	~Class1() {}

	void	print() const {
		std::cout << this->mVar.var << std::endl;
	}
};

int	main() {
	std::vector<Class1>	tmpVec;
	for (int i(0); i < 10; ++i) {
		const struct tmp	a = {i};
		tmpVec.push_back(a);
	}
	for (int i(0); i < 10; ++i) {
		tmpVec.at(i).print();
	}

}
