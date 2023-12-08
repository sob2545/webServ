#include "TrieNode.hpp"

TrieNode::TrieNode() : isEndOfWord(false) {}

TrieNode::TrieNode(const TrieNode& other) : children(other.children), isEndOfWord(other.isEndOfWord) {}

TrieNode& TrieNode::operator=(const TrieNode& other) {
	if (this != &other) {
		for (TrieIterator it = children.begin(); it != children.end(); ++it) {
			delete it->second;
		}
		children.clear();

		children = other.children;
		isEndOfWord = other.isEndOfWord;
	}
	return *this;
}

TrieNode::~TrieNode() {
	for (TrieIterator it = children.begin(); it != children.end(); ++it) {
		delete it->second;
	}
}
