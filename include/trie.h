#ifndef smv_smvcc_trie_h
#define smv_smvcc_trie_h


#include "stdbool.h"

#define TRIE_ORDER (26)
#define alphabet_index(_alphabet) (_alphabet - 'a')

typedef struct TrieNode {
  struct TrieNode *children[TRIE_ORDER];
  bool end;
} TrieNode;

TrieNode *trie_create();
void trie_insert(TrieNode *root, const char *word, int len);
void trie_free(TrieNode *root);

#endif