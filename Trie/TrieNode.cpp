#include "TrieNode.hpp"

TrieNode::TrieNode() : isEndOfWord(false) {}

TrieNode::TrieNode(const TrieNode& other) {
	*this = other;
}

TrieNode& TrieNode::operator=(const TrieNode& other) {
	if (this != &other) {
		for (TrieIterator it = children.begin(); it != children.end(); ++it) {
			if (it->second) {
				delete it->second;
				it->second = NULL;
			}
		}
		children.clear();

		for (TrieMap::const_iterator it = other.children.begin(); it != other.children.end(); ++it) {
            children[it->first] = new TrieNode(*(it->second));
        }
        isEndOfWord = other.isEndOfWord;
	}
	return *this;
}

TrieNode::~TrieNode() {
	for (TrieIterator it = children.begin(); it != children.end(); ++it) {
		if (it->second) {
			delete it->second;
			it->second = NULL;
		}
	}
}
