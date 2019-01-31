#ifndef TRIE_H
#define TRIE_H
#pragma once

#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

typedef struct trie_node TrieNode, *Trie;

Trie trie_create(void);
int trie_destroy(Trie trie);
BOOL trie_add(Trie trie, const char key[]);
int trie_longest_match(Trie trie, const char key[]);

#endif