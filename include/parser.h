#ifndef smv_smvcc_parser_h
#define smv_smvcc_parser_h

#include "lexer.h"

typedef struct Node {
  struct Node *left;
  struct Node *right;
} Node;

typedef enum ParserStatus {
  PARSER_OK = 0,
  PARSER_ERROR,
} ParserStatus;

typedef struct Parser {
} Parser;

ParserStatus parse(Parser *p);
void parser_free(Parser *p);

#endif
