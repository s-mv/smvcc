#ifndef smv_smvcc_lexer_h
#define smv_smvcc_lexer_h

#include <stdlib.h>

#include "compiler.h"
#include "list.h"
#include "token.h"

typedef struct Lexer Lexer;

typedef enum LexerStatus {
  LEXER_OK = 0,
  LEXER_ERROR,
} LexerStatus;

typedef struct Lexer {
  Position *pos;
  List tokens;
  Compiler *compiler;

  // how many brackets deep are we?
  // I don't want to waste time implementing a stack just yet
  int current_expr_count;
  // TODO define struct buffer (MAYBE)
  // for now this is going to be a list
  List *paren_list;

  // private data
  void *private;
} Lexer;

Lexer lexer_create(Compiler *compiler, void *private_data);
LexerStatus lex(Lexer *l);
void lexer_free(Lexer *l);

extern Lexer *lexer;

#endif