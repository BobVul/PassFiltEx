#define WIN32_LEAN_AND_MEAN

#ifndef  UNICODE

#define UNICODE

#endif

#include "trie.h"
#include "logging.h"

#pragma warning(push, 0) 
#include <Windows.h>
#pragma warning(pop) 

struct trie_node {
	char key; // character identifying the node. can be a utf8
	unsigned char depth; // depth in trie if node is a leaf node, i.e. explicitly added, otherwise zero
	TrieNode *firstChild; // first child of a linked list at the next level
	TrieNode *nextSibling; // next element in the linked list at this level
};

static TrieNode *create_node(void)
{
	TrieNode *t = HeapAlloc(GetProcessHeap(), 0, sizeof(TrieNode));
	if (t == NULL)
	{
		EventWriteStringW2(L"[%s:%s@%d] ERROR: Failed to allocate memory for trie node!", __FILENAMEW__, __FUNCTIONW__, __LINE__);
		return NULL;
	}
	t->key = '\0';
	t->depth = 0;
	t->nextSibling = NULL;
	t->firstChild = NULL;
	return t;
}

static int destroy_node(TrieNode *root)
{
	if (root == NULL)
	{
		return 1;
	}

	int status = 1;
	if ((status = destroy_node(root->firstChild)) == 0)
	{
		return status;
	}
	root->firstChild = NULL;
	if ((status = destroy_node(root->nextSibling)) == 0)
	{
		return status;
	}
	root->nextSibling = NULL;

	if ((status = HeapFree(GetProcessHeap(), 0, root)) == 0)
	{
		EventWriteStringW2(L"[%s:%s@%d] HeapFree failed while destroying trie! Error 0x%08lx", __FILENAMEW__, __FUNCTIONW__, __LINE__, GetLastError());
		return status;
	}

	return 1;
}

static BOOL add(TrieNode *root, const char key[], unsigned char depth)
{
	if (key[0] == '\0')
	{
		root->depth = depth;
		return TRUE;
	}

	TrieNode *lastChild = NULL;
	TrieNode *child = root->firstChild;
	while (child != NULL)
	{
		if (child->key == key[0])
		{
			return add(child, key + 1, depth + 1);
		}
		lastChild = child;
		child = child->nextSibling;
	}

	// child == NULL
	TrieNode *t = create_node();
	if (t == NULL)
	{
		return FALSE;
	}

	t->key = key[0];

	if (lastChild == NULL)
	{
		root->firstChild = t;
	}
	else
	{
		lastChild->nextSibling = t;
	}

	return add(t, key + 1, depth + 1);
}

static int match_length(TrieNode *root, const char key[])
{
	if (key[0] == '\0')
	{
		return root->depth;
	}

	TrieNode *child = root->firstChild;
	while (child != NULL)
	{
		if (child->key == key[0])
		{
			// if no deeper leaf was found, return value was 0
			int res = match_length(child, key + 1);
			// if deeper was 0, then take the current node (if it's a leaf, i.e. non-0)
			return res != 0 ? res : root->depth;
		}
		child = child->nextSibling;
	}

	// 0 if current node is not a leaf in the trie, otherwise current depth
	return root->depth;
}

Trie trie_create(void)
{
	return create_node();
}

int trie_destroy(Trie trie)
{
	return destroy_node(trie);
}

BOOL trie_add(Trie trie, const char key[])
{
	if (key[0] == '\0')
	{
		EventWriteStringW2(L"[%s:%s@%d] Trie: Ignoring empty key", __FILENAMEW__, __FUNCTIONW__, __LINE__);
		return TRUE;
	}
	return add(trie, key, 0);
}

int trie_longest_match(Trie trie, const char key[])
{
	return match_length(trie, key);
}
