#pragma once

#include <map>
#include <string>
#include "../utils/SmartPointer.hpp"

struct TrieNode {
	typedef std::map<char, ft::shared_ptr<TrieNode> >::iterator TrieIterator;
	typedef std::map<char, ft::shared_ptr<TrieNode> > TrieMap;

	std::map<char, ft::shared_ptr<TrieNode> > children;
	bool isEndOfWord;

	TrieNode();
	TrieNode(const TrieNode& other);
	TrieNode& operator=(const TrieNode& other);
	~TrieNode();
};
