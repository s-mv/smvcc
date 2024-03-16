#include "lexer.h"

#include <stdbool.h>
#include <stdio.h>

#define NUM_OPERATORS (14)
#define NUM_KEYWORDS (32)

// currently the compiler only addresses single-character operators
// TODO: double-char operators
char operators[NUM_OPERATORS] = {
    '+', '-', '*', '/', '&', '<', '>', '=', '(', ')', '{', '}', ';', ',',
};

char *keywords[NUM_KEYWORDS] = {
    "auto",   "double", "int",    "struct",   "break",   "else",     "long",
    "switch", "case",   "enum",   "register", "typedef", "char",     "extern",
    "return", "union",  "const",  "float",    "short",   "unsigned", "continue",
    "for",    "signed", "void",   "default",  "goto",    "sizeof",   "volatile",
    "do",     "if",     "static", "while",
};

// for the sake of convenience we'll store the lexer here
Lexer *lexer;
Position position;  // someday this will become a stack, who knows

// helpers
char peek();
char next();
void advance();
bool is_alpha(char c);
bool is_digit(char c);
bool is_whitespace(char c);
void update_position(char c);
// will these ever be needed? let's hope so
void push_position();
void pop_position();
void print_token(Token token);

// I forgot what this kind of thing is called
// let's call these "lexing stuff" for now
void lex_comment();  // single- and multi-line
void lex_whitespace();
void lex_number();
void lex_operator();
void lex_keyword();
void lex_identifier();

LexerStatus lex(Lexer *l) {
  lexer = l;
  while (peek() != '\0') {
    lex_operator();
    lex_whitespace();
    lex_comment();
    // lex_number(); // TODO
    // if lex_keyword is run before lex_identifier we don't even need to check
    // whether lex_identifier is accidentally lexing a literal
    lex_keyword();
    lex_identifier();
  }
}

void lexer_free(Lexer *l) {
  // TODO
}

// helpers implemented here
char peek() { return lexer->source[lexer->position.index]; }
char next() { return lexer->source[lexer->position.index + 1]; }
void advance() { update_position(lexer->source[++lexer->position.index]); }

bool is_alpha(char c) {
  // for all C knows `_` is an alphabet
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

bool is_digit(char c) { return '0' <= c && c <= '9'; }
bool is_whitespace(char c) { return c == ' ' || c == '\n' || c == '\t'; }

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
void lex_comment() {
  if (peek() == '/') {
    if (next() == '/') {
      while (peek() != '\n' && peek() != '\0') advance();
      if (peek() == '\n') advance();  // consume the \n
    } else if (next() == '*') {
      // consume the `/*`
      advance();
      advance();
      while (peek() != '*' && next() != '/') {
        if (peek() == '\0') return;  // TODO generate error
        advance();
      }
      // consume the `*/`
      advance();
      advance();
    }
  }
}

void lex_whitespace() {
  while (is_whitespace(peek())) advance();
}

// TODO, after barebones parser implementation
void lex_number() {}

void lex_operator() {
  char c = peek();
  bool is_operator = true;
  for (int i = 0; i < NUM_OPERATORS; i++) {
    if (c != operators[i]) continue;

    Token token = {
        .position = lexer->position,
        .type = TOKEN_OPERATOR,
        .character = c,
    };

    list_push(&lexer->tokens, &token);
    advance();
    return;
  }
}

void lex_keyword() {
  for (int i = 0; i < NUM_KEYWORDS; i++) {
    char *keyword = keywords[i];
    int len = strlen(keyword);

    char *word = malloc((len + 1) * sizeof(char));
    strncpy(word, lexer->source + lexer->position.index, len);
    word[len] = '\0';

    if (lexer->srclen - lexer->position.index < len) continue;
    if (strncmp(keyword, lexer->source + lexer->position.index, len - 1))
      continue;

    push_position();
    // if it's a keyword update the counter
    int index = lexer->position.index;

    for (int n = 0; n < len + 1; n++) {
      advance();
    }

    Token token = {
        .position = position,  // from `push_position`
        .type = TOKEN_KEYWORD,
        .string = keywords[i],  // no need to free this
    };

    list_push(&lexer->tokens, &token);
    return;
  }
}

void lex_identifier() {
  if (!is_alpha(peek())) return;

  push_position();
  advance();

  while (is_alpha(peek()) || is_digit(peek())) advance();

  Token token = {
      .type = TOKEN_IDENTIFIER,
      .position = position,
  };

  // adding the indentifier as a string to the token
  int len = lexer->position.index - position.index;
  token.string = malloc((len + 1) * sizeof(char));  // ownership goes to `token`
  strncpy(token.string, lexer->source + position.index, len);
  token.string[len] = '\0';

  list_push(&lexer->tokens, &token);
}

void lexer_print(Lexer *l) {
  for (int i = 0; i < l->tokens.len; i++) {
    const Token *token = list_at(&l->tokens, i);
    print_token(*token);
  }
}

void print_token(Token token) {
  switch (token.type) {
    case TOKEN_OPERATOR:
      printf("Operator:   [%-8c] -- (%-16s:%d:%d) \n", token.character,
             lexer->file, token.position.line, token.position.column);
      break;
    case TOKEN_KEYWORD:
      printf("Keyword:    [%-8s] -- (%-16s:%d:%d) \n", token.string,
             lexer->file, token.position.line, token.position.column);
      break;
    case TOKEN_IDENTIFIER:
      printf("Identifier: [%-8s] -- (%-16s:%d:%d) \n", token.string,
             lexer->file, token.position.line, token.position.column);
      break;
    default:
      break;
  }
}
