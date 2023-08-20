#include "lexer.h"

#include "compiler.h"
#include "list.h"
#include "token.h"

Lexer *lexer;

char peek_char() { return lexer->function->peek_char(lexer); }
char next_char() { return lexer->function->next_char(lexer); }
void push_char(char c) { return lexer->function->push_char(lexer, c); }

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
  return (Lexer){
      .function = &default_lexer_fns,
      .tokens = list_create(sizeof(Token)),
      .compiler = compiler,
      .private = private_data,

      .pos =
          (Position){
              .line = 1,
              .col = 1,
          },
  };
}

void lexer_free(Lexer *l) { list_free(&l->tokens); }