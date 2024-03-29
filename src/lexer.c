#include "lexer.h"

#include <stdbool.h>
#include <stdio.h>

#define NUM_OPERATORS (13)
#define NUM_KEYWORDS (32)

// currently the compiler only addresses single-character operators
// TODO: double-char operators
// NOTE: / isn't included since it's also a part of `//`
char operators[NUM_OPERATORS] = {
    '+', '-', '*', '&', '<', '>', '=', '(', ')', '{', '}', ';', ',',
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
static char peek();
static char next();
static void advance();
static bool is_alpha(char c);
static bool is_digit(char c);
static bool is_whitespace(char c);
static void update_position(char c);
// will these ever be needed? let's hope so
static void push_position();
static void pop_position();
static void print_token(Token token);
static void free_token(Token token);

// I forgot what this kind of thing is called
// let's call these "lexing stuff" for now
static void lex_whitespace();
static void lex_comment();  // single- and multi-line
static void lex_operator();
static void lex_string();
static void lex_number();
static void lex_keyword();
static void lex_identifier();

LexerStatus lex(Lexer *l) {
  lexer = l;
  while (peek() != '\0') {
    lex_whitespace();
    lex_comment();
    lex_operator();
    lex_string();
    lex_number();  // TODO
    // if lex_keyword is run before lex_identifier we don't even need to check
    // whether lex_identifier is accidentally lexing a literal
    lex_keyword();
    lex_identifier();
  }

  Token end_token = {.type = TOKEN_END};

  list_push(&lexer->tokens, &end_token);

  return LEXER_OK;
}

void lexer_free(Lexer *l) {
  for (int i = 0; i < lexer->tokens.len; i++) {
    Token *token = list_at(&lexer->tokens, i);
    free_token(*token);
  }
  list_free(&lexer->tokens);
}

// helpers implemented here
static char peek() { return lexer->source[lexer->position.index]; }
static char next() { return lexer->source[lexer->position.index + 1]; }
static void advance() {
  update_position(lexer->source[++lexer->position.index]);
}

static bool is_alpha(char c) {
  // for all C knows `_` is an alphabet
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

static bool is_digit(char c) { return '0' <= c && c <= '9'; }
static bool is_whitespace(char c) { return c == ' ' || c == '\n' || c == '\t'; }

static void update_position(char c) {
  if (c == '\n') {
    lexer->position.line++;
    lexer->position.column = 1;
    return;
  }
  lexer->position.column++;
}

static void push_position() { position = lexer->position; }
static void pop_position() { lexer->position = position; }

// "lexing stuff" implemented here
static void lex_whitespace() {
  while (is_whitespace(peek())) advance();
}

static void lex_comment() {
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

static void lex_operator() {
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

// TODO list:
// 1. lex escape characters
// 2. error handling
static void lex_string() {
  if (peek() != '"') return;

  advance();
  push_position();

  while (peek() != '"') advance();

  Token token = {
      .type = TOKEN_STRING,
      .position = position,
  };

  int len = lexer->position.index - position.index;
  token.string = malloc((len + 1) * sizeof(char));  // ownership goes to `token`
  strncpy(token.string, lexer->source + position.index, len);
  token.string[len] = '\0';

  list_push(&lexer->tokens, &token);

  advance();  // eat the `"`
}

// TODO list:
// 1. lex float
// 2. lex other bases
static void lex_number() {
  if (!is_digit(peek())) return;

  push_position();
  advance();

  while (is_digit(next())) advance();

  if (lexer->position.index == position.index) return;

  // convert string to long long
  const int len = lexer->position.index - position.index + 1;
  char number_repr[len];
  strncpy(number_repr, lexer->source + lexer->position.index - 1, len - 1);
  number_repr[len] = '\0';

  Token token = {
      .type = TOKEN_INT,
      .integer = atoll(number_repr),
      .position = position,
  };

  list_push(&lexer->tokens, &token);
}

static void lex_keyword() {
  for (int i = 0; i < NUM_KEYWORDS; i++) {
    char *keyword = keywords[i];
    int len = strlen(keyword);

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

static void lex_identifier() {
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
      printf("Operator:   [%-8c] -- (%-16s:%d:%d)\n", token.character,
             lexer->file, token.position.line, token.position.column);
      break;
    case TOKEN_KEYWORD:
      printf("Keyword:    [%-8s] -- (%-16s:%d:%d)\n", token.string, lexer->file,
             token.position.line, token.position.column);
      break;
    case TOKEN_IDENTIFIER:
      printf("Identifier: [%-8s] -- (%-16s:%d:%d)\n", token.string, lexer->file,
             token.position.line, token.position.column);
      break;
    case TOKEN_INT:
      printf("Integer:    [%-8llu] -- (%16s:%d:%d)\n", token.integer, lexer->file,
             token.position.line, token.position.column);
      break;
    case TOKEN_STRING:
      printf("String:     [%.8s...] -- (%16s:%d:%d)\n", token.string,
             lexer->file, token.position.line, token.position.column);
      break;
    default:
      break;
  }
}

void free_token(Token token) {
  switch (token.type) {
    case TOKEN_IDENTIFIER:
      free(token.string);
      break;
    default:
      break;
  }
}
