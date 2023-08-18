#include "list.h"

list list_create(int size) {
  return (list){
      .cap = 0,
      .len = 0,
      .size = size,
  };
}
void list_free(list *ls) {
  free(ls->data);
  ls->cap = 0;
  ls->len = 0;
  ls->size = 0;
}

void *list_at(list *ls, int index) { return ls->data + index * ls->size; }

void list_push(list *ls, void *data) {
  if (ls->len >= ls->cap) {
    ls->cap = ls->cap < 8 ? 8 : ls->cap * 2;
    ls->data = realloc(ls->data, ls->cap * ls->size);
  }

  void *ptr = list_at(ls, ls->len++);
  memcpy(ptr, data, ls->size);
}

void list_push_array(list *ls, void **data, int data_len) {
  for (int i = 0; i < data_len; i++) list_push(ls, data[i]);
}

void *list_pop(list *ls) {
  if (ls->len == 0) return NULL;
  return ls->data + --ls->len * ls->size;
}

void *list_peek(list *ls) {
  if (ls->len == 0) return NULL;
  return list_at(ls, ls->len - 1);
}
