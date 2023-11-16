#include "lexer.h"

#include "compiler.h"
#include "list.h"
#include "token.h"

#define OPERATORS_LEN (13)
#define LONELY_OP_LEN (8)
#define KEYWORDS_LEN (32)

Lexer *lexer = {0};
static Token token = {0};

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
const char *keywords[32] = {
    "auto",     "break",  "case",    "char",   "const",    "continue",
    "default",  "do",     "double",  "else",   "enum",     "extern",
    "float",    "for",    "goto",    "if",     "int",      "long",
    "register", "return", "short",   "signed", "sizeof",   "static",
    "struct",   "switch", "typedef", "union",  "unsigned", "void",
    "volatile", "while",
};

char lexer_peek_char();
char lexer_next_char();
void lexer_push_char(char c);

static bool is_operator(char c);
static bool is_lonely_op(char c);
static inline bool str_eq(const char *str1, const char *str2);
static bool is_keyword(const char *str);

Token token_read_next();

static Token token_make_number();
static Token token_make_operator(char op);
static Token token_make_lonely_op(char op);
static Token token_make_keyword(char *str);
static Token token_make_comment_oneline();
static Token token_make_comment_multiline();

static Token token_make_number() {
  const char *num = NULL;
  List list = list_create(sizeof(char));
  Token t = {0};

  for (char c = lexer_next_char(); '0' <= c && c <= '9';
       c = lexer_next_char()) {
    list_push(&list, &c);
  }
  // now list->data contains a number that we assign to token

  t.llnum = atoll((const char *)list.data);

  list_free(&list);
  return t;
}

// TODO
static Token token_make_operator(char op) {
  char c = lexer_next_char();
  return NO_TOKEN;
}

static Token token_make_lonely_op(char op) {
  Token t = {0};
  t.type = SYMBOL;

  return t;
}

static Token token_make_keyword(char *str) { return NO_TOKEN; }

static Token token_make_comment_oneline() {
  List ls = list_create(sizeof(char));
  for (char c = lexer_next_char(); c != '\n' && c != EOF; c = lexer_next_char())
    ;
  // ownership of ls.data goes to token
  return (Token){.type = COMMENT, .str = ls.data};
}

static Token token_make_comment_multiline() {
  List ls = list_create(sizeof(char));
  char c;
  for (c = lexer_next_char(); c != EOF; c = lexer_next_char()) {
    if (c == '*') {
      lexer_next_char();
      if (lexer_peek_char() == '/') break;
    }
  }

  if (c == EOF) {
    compiler_error(lexer, "Multiline comment left open.");
    return NO_TOKEN;  // the program will exit but eh common courtesy
  }

  return (Token){.type = COMMENT, .str = ls.data};
}

static Token token_make_string(char start, char end) {
  List ls = list_create(sizeof(char));
  char c = lexer_next_char();

  for (c = lexer_next_char(); c != end && c != EOF; c = lexer_next_char()) {
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
  return (Token){.type = STRING, .str = ls.data};
}

Token token_read_next() {
  Token t = {0};
  // last token
  const char c = lexer_peek_char();

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
    case '/': {
      char c = lexer_peek_char();
      if (c == '/') {
        lexer_next_char();
        token_make_comment_oneline();
      } else if (c == '*') {
        lexer_next_char();
        token_make_comment_multiline();
      }
      break;
    }
    case '\n': {
      lexer_next_char();
      t.type = NEWLINE;
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

  Token token = token_read_next();
  while (token.type != NONE) {
    list_push(&l->tokens, &token);
    token = token_read_next();
  }

  return LEXER_OK;
}

// the less imporant functions :)

Lexer lexer_create(Compiler *compiler, void *private_data) {
  compiler->pos = (Position){.col = 1, .line = 1, .fname = compiler->file.path};

  return (Lexer){
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

char lexer_next_char() {
  Compiler *compiler = lexer->compiler;
  char c = getc(compiler->file.fp);
  if (c == '\n') {
    compiler->pos.line += 1;
    compiler->pos.col = 1;
  } else
    compiler->pos.col += 1;

  return c;
}

char lexer_peek_char() {
  Compiler *compiler = lexer->compiler;
  char c = getc(compiler->file.fp);
  // this is equivalent of uh... un-pop
  ungetc(c, compiler->file.fp);
  return c;
}

void lexer_push_char(char c) {
  Compiler *compiler = lexer->compiler;
  ungetc(c, compiler->file.fp);  // so nifty!
}

// helpers

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
