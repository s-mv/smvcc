#ifndef smv_smvcc_lexer_h
#define smv_smvcc_lexer_h

#include <stdlib.h>

#include "list.h"

/* Token implementation */

typedef enum TokenType {
  TOKEN_NONE,
  TOKEN_IDENTIFIER,  // TODO
  TOKEN_OPERATOR,    // TODO
  TOKEN_SYMBOL,      // TODO
  TOKEN_KEYWORD,     // TODO
  TOKEN_STRING,      // TODO
  TOKEN_INT,         // TODO
  TOKEN_FLOAT,       // TODO
  TOKEN_COMMENT,     // TODO
  TOKEN_INCLUDE,     // TODO, macro yay
} TokenType;

typedef struct Position {
  int index;
  int line;
  int column;
} Position;

typedef struct Token {
  TokenType type;
  // union holds relevant data
  union {
    long long integer;
    double real;
    char character;
    char *string;
    void *any;
  };
  // width of relevant data e.g. width 4 TOKEN_INT = long
  // this is majorly TODO and I might not even implement it
  int width;
  Position position;
} Token;

#define NO_TOKEN ((Token){.type = TOKEN_NONE})

typedef enum LexerStatus {
  LEXER_OK = 0,
  LEXER_ERROR,
} LexerStatus;

typedef struct Lexer {
  char *file;
  char *source;
  int srclen;
  Position position;
  List tokens;
} Lexer;

#define position_initial ((Position){.line = 1, .column = 1, .index = 0})

// TODO: void lexer_init();

LexerStatus lex(Lexer *l);
void lexer_print(Lexer *l);
void lexer_free(Lexer *l);

#endif
