#include "Trie.hpp"
#include <string>

Trie::Trie() : root(ft::shared_ptr<TrieNode>(new TrieNode())) {}

Trie::Trie(const Trie& other) : root(other.root) {}

Trie& Trie::operator=(const Trie& other) {
	root = other.root;
	return *this;
}

Trie::~Trie() {}

void	Trie::insert(const std::string& key) {
	ft::shared_ptr<TrieNode> node = root;

	for (std::size_t i(0); i < key.length(); ++i) {
		if (node->children.find(key[i]) == node->children.end()) {
			node->children.insert(std::make_pair(key[i], ft::shared_ptr<TrieNode>(new TrieNode())));
		}
		node = node->children[key[i]];
	}
	node->isEndOfWord = true;
}

bool	Trie::search(const std::string& key) {
	ft::shared_ptr<TrieNode> node = root;

	for (std::size_t i(0); i < key.length(); ++i) {
		if (node->children.find(key[i]) == node->children.end()) {
			return false;
		}
		node = node->children[key[i]];
	}
	return true;
}

const std::string	Trie::find(const std::string& key) const {
	ft::shared_ptr<TrieNode> node = root;
	std::string	result;

	for (std::size_t i(0); i < key.length(); ++i) {
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