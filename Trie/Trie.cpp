#include "Trie.hpp"
#include <string>

Trie::Trie() : root(new TrieNode()) {}

Trie::Trie(const Trie& other) : root(new TrieNode(*other.root)) {}

Trie& Trie::operator=(const Trie& other) {
	if (this != &other) {
		delete root;
		root = new TrieNode(*other.root);
	}
	return *this;
}

Trie::~Trie() {
	delete root;
}

void	Trie::insert(const std::string& key) {
	TrieNode* node = root;

	for (size_t i(0); i < key.length(); ++i) {
		if (node->children.find(key[i]) == node->children.end()) {
			node->children[key[i]] = new TrieNode();
		}
		node = node->children[key[i]];
	}
	node->isEndOfWord = true;
}

bool	Trie::search(const std::string& key) {
	TrieNode* node = root;

	for (size_t i(0); i < key.length(); ++i) {
		if (node->children.find(key[i]) == node->children.end()) {
			return false;
		}
		node = node->children[key[i]];
	}
	return true;
}

const std::string	Trie::find(const std::string& key) const {
	TrieNode* node = root;
	std::string	result;

	for (size_t i(0); i < key.length(); ++i) {
		const char&	c = key[i];
		if (node->children.find(key[i]) == node->children.end()) {
			return result;
		}
		node = node->children[key[i]];
		if (node->isEndOfWord) {
			result = key.substr(0, i + 1);
		}
	}
	return result;
}