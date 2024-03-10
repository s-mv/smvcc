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

/* this is where I say delusional stuff or as I like to call it, "brainstorming"
 *
 * a lexer may either have 2 passes
 * 1. lex preprocessor directives as tokens
 * 2. traverse token array and resolve directives
 * (biggest con: conditional directives are redundant overhead)
 * 
 * or do it on the fly by lexing recursively and returning a pointer to the
 * lexer (add TOKEN_FILE pointing to another lexer with different source) and on
 * the go also make a registry of #define's
 * (biggest con: I am mentally incapable of thinking of a clean implementation)
 * 
 * TODO eventually move comments like these into the docs
 */