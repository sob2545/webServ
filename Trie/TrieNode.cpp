#include "TrieNode.hpp"

TrieNode::TrieNode() : isEndOfWord(false) {}

TrieNode::TrieNode(const TrieNode& other) {
	*this = other;
}

TrieNode& TrieNode::operator=(const TrieNode& other) {
	if (this != &other) {
		children.clear();

		for (TrieMap_t::const_iterator it = other.children.begin(); it != other.children.end(); ++it) {
			children.insert(std::make_pair(it->first, it->second));
        }
        isEndOfWord = other.isEndOfWord;
	}
	return *this;
}

TrieNode::~TrieNode() {}
