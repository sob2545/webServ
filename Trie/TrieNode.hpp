#pragma once

#include <map>
#include <string>

struct TrieNode {
	typedef std::map<char, TrieNode*>::iterator TrieIterator;
	typedef std::map<char, TrieNode*> TrieMap;

	std::map<char, TrieNode*> children;
	bool isEndOfWord;

	TrieNode();
	TrieNode(const TrieNode& other);
	TrieNode& operator=(const TrieNode& other);
	~TrieNode();
};
