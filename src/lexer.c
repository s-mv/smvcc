#include "lexer.h"

#include <stdbool.h>

// for the sake of convenience we'll store the lexer here
Lexer *lexer;
Position position;  // someday this will become a stack, who knows

// helpers
char peek();
char next();
bool is_alpha(char c);
bool is_digit(char c);
void update_position(char c);
// will these ever be needed? let's hope so
void push_position();
void pop_position();

// I forgot what this kind of thing is called
// let's call these "lexing stuff" for now
void lex_comment();  // single- and multi-line
void lex_whitespace();
void lex_number();
void lex_operator();
void lex_keyword();
void lex_literal();

void lex(Lexer *l) {
  lexer = &l;
  for (int i = 0; l->source[i] != '\0'; i++) {
    lex_comment();
    lex_whitespace();
    lex_number();
    // if lex_keyword is run before lex_literal we don't even need to check
    // whether lex_literal is accidentally lexing a literal
    lex_keyword();
    lex_literal();
  }
}

void lexer_free(Lexer *l) {
  // TODO
}

// helpers implemented here
char peek() { return lexer->source[lexer->position.index + 1]; }

char next() {
  char c = lexer->source[lexer->position.index++];
  update_position(c);
  return c;
}

bool is_alpha(char c) { return 'a' <= c <= 'z' || 'A' <= c <= 'Z'; }
bool is_digit(char c) { return '0' <= c <= '9'; }

void update_position(char c) {
  if (c == '\n') {
    lexer->position.line++;
    lexer->position.column = 1;
    return;
  }
  lexer->position.column++;
}

void push_position() { position = lexer->position; }
void pop_position() { lexer->position = position; }

// "lexing stuff" implemented here
void lex_comment() {}

void lex_whitespace() {}

void lex_number() {}

void lex_operator() {}

void lex_keyword() {}

void lex_literal() {}
