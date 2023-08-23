#include "token.h"

#include "lexer.h"
#include "list.h"

static Token token;

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

Token *token_read_next() {
  Token *t = NULL;
  // last token
  char c = peek_char();

  // check for numbers
  if ('0' <= c && c <= '9') {
    t = token_make_number();
    return t;
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