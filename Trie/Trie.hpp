#pragma once

#include "TrieNode.hpp"

class Trie {
private:
	TrieNode* root;
public:
	Trie();
	Trie(const Trie& other);
	Trie& operator=(const Trie& other);
	~Trie();

	void	insert(const std::string& key);
	bool	search(const std::string& key);

};
