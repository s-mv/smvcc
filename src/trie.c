#include "trie.h"

#include <stdlib.h>

TrieNode *trie_create() {
  TrieNode *root = malloc(sizeof(TrieNode));
  for (int i = 0; i < TRIE_ORDER; i++) root->children[i] = NULL;
  root->end = false;
  return root;
}

void trie_insert(TrieNode *root, const char *word, int len) {
  TrieNode *pointer = root;

  for (int lvl = 0; lvl < len; lvl++) {
    int i = alphabet_index(word[lvl]);
    if (!pointer->children[i]) pointer->children[i] = trie_create();
    pointer = pointer->children[i];
  }
  pointer->end = true;
}

void trie_free(TrieNode *root) {
  // TODO
}
