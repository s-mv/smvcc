#ifndef smv_smvcc_parser_h
#define smv_smvcc_parser_h

#include <stdbool.h>

#include "grammar.h"
#include "lexer.h"
#include "list.h"

typedef enum NodeType {
  NODE_FACTOR,      // TODO
  NODE_EXPRESSION,  // TODO
  NODE_FUNCTION,    // TODO
  NODE_ARGUMENTS,   // TODO
  NODE_CALL,        // TODO
  NODE_RETURN,      // TODO
} NodeType;

typedef enum ParserStatus {
  PARSER_OK = 0,
  PARSER_ERROR,
} ParserStatus;

typedef struct Parser {
  List tokens;
  Program AST;
  void *current;
} Parser;

ParserStatus parse(Parser *p);
void parser_print(Parser *p);
void parser_free(Parser *p);

#endif
