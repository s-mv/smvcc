#ifndef smv_smvcc_token_h
#define smv_smvcc_token_h

#include <stdbool.h>
#include <stdio.h>

typedef enum TokenType {
  NONE,
  IDENTIFIER,
  OPERATOR,
  SYMBOL,
  KEYWORD,
  NUMBER,
  STRING,
  COMMENT,
  NEWLINE,  // mostly a helper
} TokenType;

typedef struct Position {
  int line;
  int col;
  const char* fname;
} Position;

typedef struct Token {
  TokenType type;

  // union holds relevant data
  union {
    char c;
    const char* str;
    unsigned int inum;
    int num;
    unsigned long lnum;
    unsigned long long llnum;
    void* any;
  };

  Position pos;

  // true if there's whitespace following the token
  bool whitespace;
  // just keeping track of the last open bracket
  const char* between_brackets;
} Token;

#define NO_TOKEN ((Token){.type = NONE})

#endif