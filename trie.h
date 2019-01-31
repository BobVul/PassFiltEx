#ifndef TRIE_H
#define TRIE_H
#pragma once

#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

typedef struct trie_node TrieNode, *Trie;

struct trie_node {
	char key;
	int value;
	TrieNode *firstChild;
	TrieNode *nextSibling;
};

Trie trie_create(void);
void trie_destroy(Trie trie_node);
BOOL trie_set(Trie trie_node, const char key[], int value);
int trie_get(Trie trie_node, const char key[]);

#endif