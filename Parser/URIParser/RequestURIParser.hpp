#pragma once

#include "BNF_utils/BNFdata.hpp"

class	RequestURIParser {
private:
    RequestURIParser(const RequestURIParser& other);
	RequestURIParser& operator=(const RequestURIParser& other);


public:
	RequestURIParser();
	~RequestURIParser();

	URI	Parser(const std::string& uri);
};