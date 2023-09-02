#include "lexer.h"

#include "compiler.h"
#include "list.h"
#include "token.h"

Lexer *lexer;

char peek_char() { return lexer->function->peek_char(lexer); }
char next_char() { return lexer->function->next_char(lexer); }
void push_char(char c) { return lexer->function->push_char(lexer, c); }

#define OPERATORS_LEN (13)
#define LONELY_OP_LEN (8)

// strictly non-lonely operators/operator constituents
// NOTE: lacks / for a GOOD REASON
static char operators[OPERATORS_LEN] = {
    '+', '=', '<', '>', '&', '%', '-', '~', ':', ',', '!', '^', '|',
};

// lonely operators:
// a lonely operator is an operator which is ALWAYS single-character
static char lonely_op[LONELY_OP_LEN] = {
    '(', ')', '[', '{', '}', '.', '*', '?',
};

static Token token;

static bool is_operator(char c);
static bool is_lonely_op(char c);
static inline bool str_eq(const char *str1, const char *str2);
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
  return &token;
}

// TODO
static Token *token_make_operator(char op) {
  char c = next_char();
  return NULL;
}

static Token *token_make_lonely_op(char op) {
  Token *t;
  token_create(t);

  t->type = SYMBOL;

  return t;
}

static Token *token_make_keyword(char *str) { return NULL; }

static Token *token_make_string(char start, char end) {
  List ls = list_create(sizeof(char));
  char c = next_char();

  for (c = next_char(); c != end && c != EOF; c = next_char()) {
    if (c == '\\') {
      // handle an escape character (TODO)
      continue;
    }
    // ...otherwise
    list_push(&ls, &c);
  }

  if (c == EOF) {
    list_free(&ls);
    compiler_error(lexer, "String never terminated.");
  }

  c = '\0';
  list_push(&ls, &c);

  // ownership of ls.data goes to token
  return token_create(&(Token){
      .type = STRING,
      .str = ls.data,
  });
}

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
    return t;
  }
  // check for non-lonely operators
  if (is_operator(c)) {
    t = token_make_operator(c);
    return t;
  }

  // check for non-lonely operators
  for (int i = 0; i < LONELY_OP_LEN; i++) {
    if (c != lonely_op[i]) continue;
    t = token_make_lonely_op(c);
    return t;
  }

  switch (c) {
      // string
    case '"': {
      t = token_make_string('"', '"');
      break;
    }
    case ' ':
    case '\t': {
      // TODO make this more efficient
      Token *temp = list_peek(&lexer->tokens);
      temp->whitespace = true;
    }
    case '\n': {
      next_char();
      t = malloc(sizeof(Token));
      t->type = NEWLINE;
      break;
    }
    case EOF: {
      break;
    }
    default: {
      compiler_error(lexer, "Unexpected token: (char: %c).", c);
      break;
    }
  }

  return t;
}

LexerStatus lex(Lexer *l) {
  // re-init the lexer
  l->current_expr_count = 0;
  l->paren_list = NULL;
  lexer = l;

  Token *token = token_read_next();
  while (token) {
    list_push(&l->tokens, token);
    token = token_read_next();
  }

  return LEXER_OK;
}

// the less imporant functions :)

char lexer_next_char(Lexer *l) {
  Compiler *compiler = l->compiler;

  char c = getc(compiler->file.fp);
  if (c == '\n') {
    compiler->pos.line += 1;
    compiler->pos.col = 1;
  } else
    compiler->pos.col += 1;

  return c;
}

char lexer_peek_char(Lexer *l) {
  Compiler *compiler = l->compiler;
  char c = getc(compiler->file.fp);
  // this is equivalent of uh... un-pop
  ungetc(c, compiler->file.fp);
  return c;
}

void lexer_push_char(Lexer *l, char c) {
  Compiler *compiler = l->compiler;
  ungetc(c, compiler->file.fp);  // so nifty!
}

LexerFunctions default_lexer_fns = (LexerFunctions){
    .next_char = lexer_next_char,
    .peek_char = lexer_peek_char,
    .push_char = lexer_push_char,
};

Lexer lexer_create(Compiler *compiler, LexerFunctions *fns,
                   void *private_data) {
  compiler->pos = (Position){.col = 1, .line = 1, .fname = compiler->file.path};

  return (Lexer){
      .function = &default_lexer_fns,
      .tokens = list_create(sizeof(Token)),
      .compiler = compiler,
      .private = private_data,

      .pos = &compiler->pos,
  };
}

void lexer_free(Lexer *l) {
  // free string token data
  for (int i = 0; i < l->tokens.len; i++) {
    Token *token = (Token *)list_at(&l->tokens, i);
    if (token->type == STRING) free((void *)token->str);
  }
  list_free(&l->tokens);
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

static inline bool str_eq(const char *str1, const char *str2) {
  return str1 && str2 && (strcmp(str1, str2) == 0);
}
