#include "token.h"

#include "lexer.h"
#include "list.h"

static Token token;

static bool is_operator(char c);
static bool is_lonely_op(char c);
static bool is_eq(const char *str1, const char *str2);
static bool is_keyword(const char *str);

Token *token_create(Token *t) {
  memcpy(&token, t, sizeof(Token));
  t->pos = *lexer->pos;
  return &token;
}

static Token *token_make_number() {
  const char *num = NULL;
  List list = list_create(sizeof(char));

  for (char c = next_char(); '0' <= c && c <= '9'; c = next_char()) {
    list_push(&list, &c);
  }
  // now list->data contains a number that we assign to token

  token_create(&token);

  token.llnum = atoll((const char *)list.data);

  list_free(&list);

  printf("number token: %llu\n", token.llnum);

  return &token;
}

// TODO
static Token *token_make_operator(char op) { char c = next_char(); }

static Token *token_make_lonely_op(char op) {
  Token *t;
  t->type = SYMBOL;
}

static Token *token_make_keyword(char *str) {}

Token *token_read_next() {
  Token *t = NULL;
  // last token
  const char c = peek_char();

  // check for numbers
  if ('0' <= c && c <= '9') {
    t = token_make_number();
    return t;
  }

  // check for lonely operators
  if (is_lonely_op(c)) {
    t = token_make_lonely_op(c);
    return;
  }
  // check for non-lonely operators
  if (is_operator(c)) {
    t = token_make_operator(c);
    return;
  }

  // check for non-lonely operators
  for (int i = 0; i < LONELY_OP_LEN; i++) {
    if (c != lonely_op[i]) continue;
    t = token_make_lonely_op(c);
    return;
  }
  // TODO:NEXT
  // strings

  switch (c) {
    case ' ':
    case '\t': {
      // TODO make this more efficient
      Token *temp = list_peek(&lexer->tokens);
      temp->whitespace = true;
    }
    case '\n': {
      t->type = NEWLINE;
      break;
    }
    case EOF: {
      break;
    }
    default: {
      compiler_error(lexer, "Unexpected token: (char:%c).", c);
      break;
    }
  }

  return t;
}

static bool is_operator(char c) {
  for (int i = 0; i < LONELY_OP_LEN; i++) {
    if (c == lonely_op[i]) return true;
  }
  return false;
}

static bool is_lonely_op(char c) {
  for (int i = 0; i < OPERATORS_LEN; i++) {
    if (c == operators[i]) return true;
  }
  return false;
}

static bool is_keyword(const char *str) {
  // TODO
}