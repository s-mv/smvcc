#include "lexer.h"

char lexer_next_char(Lexer *l) {
  CompileProcess *compiler = l->compiler;
  compiler->pos.col += 1;

  char c = getc(compiler->file.fp);
  if (c == '\n') {
    compiler->pos.line += 1;
    compiler->pos.col = 1;
  }

  return c;
}

char lexer_peek_char(Lexer *l) {
  CompileProcess *compiler = l->compiler;
  char c = getc(compiler->file.fp);
  // this is equivalent of uh... un-pop
  ungetc(c, compiler->file.fp);
  return c;
}

void lexer_push_char(Lexer *l, char c) {
  CompileProcess *compiler = l->compiler;
  ungetc(c, compiler->file.fp);  // so nifty!
}

LexerFunctions lexer_fns = (LexerFunctions){
    .next_char = lexer_next_char,
    .peek_char = lexer_peek_char,
    .push_char = lexer_push_char,
};

Lexer lexer_create(CompileProcess *compiler, LexerFunctions *fns,
                   void *private_data) {
  return (Lexer){
      .function = &lexer_fns,
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