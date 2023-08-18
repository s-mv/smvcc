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

typedef struct LexerFunctions {
  LEX_PROCESS_NEXT_CHAR next_char;
  LEX_PROCESS_PEEK_CHAR peek_char;
  LEX_PROCESS_PUSH_CHAR push_char;
} LexerFunctions;

typedef struct Lexer {
  Position pos;
  List tokens;
  CompileProcess *compiler;

  // how many brackets deep are we?
  // I don't want to waste time implementing a stack just yet
  int current_expr_count;
  // TODO define struct buffer
  // buffer paren_buffer;

  LexerFunctions *function;

  // private data
  void *private;
} Lexer;

Lexer lexer_create(CompileProcess *compiler, LexerFunctions *fns,
                   void *private_data);
void lexer_free(Lexer *l);

#endif