#ifndef smv_smvcc_lexer_h
#define smv_smvcc_lexer_h

#include <stdlib.h>

#include "compiler.h"
#include "list.h"
#include "token.h"

typedef struct Lexer Lexer;

// LexerFunctions functions
typedef char (*LEX_PROCESS_NEXT_CHAR)(Lexer *lexer);
typedef char (*LEX_PROCESS_PEEK_CHAR)(Lexer *lexer);
typedef void (*LEX_PROCESS_PUSH_CHAR)(Lexer *lexer, char c);

typedef enum LexerStatus {
  LEXER_OK = 0,
  LEXER_ERROR,
} LexerStatus;

typedef struct LexerFunctions {
  LEX_PROCESS_NEXT_CHAR next_char;
  LEX_PROCESS_PEEK_CHAR peek_char;
  LEX_PROCESS_PUSH_CHAR push_char;
} LexerFunctions;

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

  LexerFunctions *function;

  // private data
  void *private;
} Lexer;

Lexer lexer_create(Compiler *compiler, LexerFunctions *fns, void *private_data);
void lexer_free(Lexer *l);

LexerStatus lex(Lexer *l);

char peek_char();
char next_char();
void push_char(char c);

extern LexerFunctions default_lexer_fns;
extern Lexer *lexer;

#endif