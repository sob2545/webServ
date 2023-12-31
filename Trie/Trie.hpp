#pragma once

#include "TrieNode.hpp"

class Trie {
private:
	ft::shared_ptr<TrieNode>	root;
public:
	Trie();
	Trie(const Trie& other);
	Trie& operator=(const Trie& other);
	~Trie();

	void				insert(const std::string& key);
	bool				search(const std::string& key);
	const std::string	find(const std::string& key) const;
};
