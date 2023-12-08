#include <iostream>
#include "Trie.hpp"
#include <unistd.h>

void a()
{
	system("leaks a.out");
}

int	main() {
	atexit(a);
	Trie trie;

	trie.insert("hello");
	trie.insert("world");
	trie.insert("hell");
	trie.insert("hi");
	trie.insert("hey");
	trie.insert("he");
	trie.insert("h");
	trie.insert("h");
	trie.insert("h");

	std::cout << trie.search("hello") << std::endl;
	std::cout << trie.search("wor") << std::endl;
	std::cout << trie.search("awor") << std::endl;
	std::cout << trie.search("heli") << std::endl;

}