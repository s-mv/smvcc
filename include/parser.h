#ifndef smv_smvcc_parser_h
#define smv_smvcc_parser_h

#include "lexer.h"

typedef enum ParserStatus {
  PARSER_OK = 0,
  PARSER_ERROR,
} ParserStatus;

typedef struct Parser {
  Token previous;
  Token current;
} Parser;

ParserStatus parse(Parser *p);
void parser_free(Parser *p);

#endif
