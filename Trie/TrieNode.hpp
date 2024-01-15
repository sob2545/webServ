#pragma once

#include <map>
#include <string>
#include "../Utils/SmartPointer.hpp"

struct TrieNode {
	typedef std::map<char, ft::shared_ptr<TrieNode> >::iterator TrieIterator_t;
	typedef std::map<char, ft::shared_ptr<TrieNode> > TrieMap_t;

	std::map<char, ft::shared_ptr<TrieNode> > children;
	bool isEndOfWord;

	TrieNode();
	TrieNode(const TrieNode& other);
	TrieNode& operator=(const TrieNode& other);
	~TrieNode();
};
