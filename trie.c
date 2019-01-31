#include "trie.h"
#pragma warning(push, 0) 
#include <Windows.h>
#pragma warning(pop) 

static TrieNode *create_node(void)
{
	TrieNode *t = HeapAlloc(GetProcessHeap(), 0, sizeof(TrieNode));
	t->key = '\0';
	t->value = 0;
	t->nextSibling = NULL;
	t->firstChild = NULL;
	return t;
}

static void destroy_node(TrieNode *root)
{
	if (root == NULL)
	{
		return;
	}

	destroy_node(root->firstChild);
	destroy_node(root->nextSibling);

	HeapFree(GetProcessHeap(), 0, root);
}

static void set(TrieNode *root, const char key[], int value)
{
	if (key[0] == '\0')
	{
		root->value = value;
		return;
	}

	TrieNode *lastChild = NULL;
	TrieNode *child = root->firstChild;
	while (child != NULL)
	{
		if (child->key == key[0])
		{
			set(child, key + 1, value);
			return;
		}
		lastChild = child;
		child = child->nextSibling;
	}

	// child == NULL
	TrieNode *t = create_node();
	t->key = key[0];

	if (lastChild == NULL)
	{
		root->firstChild = t;
	}
	else
	{
		lastChild->nextSibling = t;
	}

	set(t, key + 1, value);
}

static int get(TrieNode *root, const char key[])
{
	if (key[0] == '\0')
	{
		return root->value;
	}

	TrieNode *child = root->firstChild;
	while (child != NULL)
	{
		if (child->key == key[0])
		{
			return get(child, key + 1);
		}
		child = child->nextSibling;
	}

	return 0;
}

Trie trie_create(void)
{
	return create_node();
}

void trie_destroy(Trie trie)
{
	destroy_node(trie);
}

void trie_set(Trie trie, const char key[], int value)
{
	set(trie, key, value);
}

int trie_get(Trie trie, const char key[])
{
	return get(trie, key);
}
