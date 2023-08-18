#ifndef smv_smvcc_list_h
#define smv_smvcc_list_h

#include <stdlib.h>
#include <string.h>

typedef struct List {
  void *data;
  int size;  // size of 1 element
  int len;
  int cap;
} List;

List list_create(int size);
void list_free(List *ls);

void *list_at(List *ls, int index);

void list_push(List *ls, void *data);
void list_push_array(List *ls, void **data, int data_len);
void *list_pop(List *ls);
void *list_peek(List *ls);

#endif