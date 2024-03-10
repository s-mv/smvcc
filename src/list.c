#include "list.h"

#include <stdio.h>

List list_create(int size) {
  return (List){
      .len = 0,
      .cap = 0,
      .size = size,
  };
}

// get the smallest power of 2 greater than n
static int smallest_pow_2(int n);

void list_free(List *ls) {
  free(ls->data);
  ls->cap = 0;
  ls->len = 0;
  ls->size = 0;
}

void list_grow(List *list, int cap) {
  if (cap <= list->cap) return;
  list->data = realloc(list->data, cap * list->size);
  list->cap = cap;
}

void *list_at(List *ls, int index) { return ls->data + index * ls->size; }

void list_push(List *ls, void *data) {
  if (ls->len >= ls->cap) {
    ls->cap = ls->cap < 8 ? 8 : smallest_pow_2(ls->cap + 1);
    ls->data = realloc(ls->data, ls->cap * ls->size);
  }

  void *ptr = list_at(ls, ls->len++);
  memcpy(ptr, data, ls->size);
}

void list_push_array(List *ls, void **data, int data_len) {
  for (int i = 0; i < data_len; i++) list_push(ls, data[i]);
}

void *list_pop(List *ls) {
  if (ls->len == 0) return NULL;
  return ls->data + --ls->len * ls->size;
}

void *list_peek(List *ls) {
  if (ls->len == 0) return NULL;
  return list_at(ls, ls->len - 1);
}

static int smallest_pow_2(int n) {
  const int size = sizeof(int);
  int ret = 1;
  while (n > 0) {
    ret <<= 1;
    n >>= 1;
  }
  return ret;
}