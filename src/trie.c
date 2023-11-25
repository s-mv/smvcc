#include "trie.h"

#include <stdlib.h>
#include <string.h>

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

void trie_mass_insert(TrieNode *root, const char **words, int num) {
  for (int i = 0; i < num; i++) trie_insert(root, words[i], strlen(words[i]));
}

bool trie_search(TrieNode *root, const char *word, int len) {
  TrieNode *pointer = root;
  for (int lvl = 0; lvl < len; lvl++) {
    int i = alphabet_index(word[lvl]);
    if (!pointer->children[i]) return false;
    pointer = pointer->children[i];
  }
}

void trie_free(TrieNode *root) {
  // TODO
}
