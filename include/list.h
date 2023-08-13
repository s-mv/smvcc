#ifndef smv_smvcc_list_h
#define smv_smvcc_list_h

#include <stdlib.h>
#include <string.h>

typedef struct list {
  void *data;
  int size;  // size of 1 element
  int len;
  int cap;
} list;

list *list_create(int size);
void list_free(list *ls);

void *list_at(list *ls, int index);

void list_push(list *ls, void *data);
void list_push_array(list *ls, void **data, int data_len);
void *list_pop(list *ls);
void *list_peek(list *ls);

#endif