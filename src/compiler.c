#include "compiler.h"

#include <stdarg.h>

#include "lexer.h"

Compiler *compiler_create(const char *inpath, const char *outpath, int flags) {
  // read the file
  FILE *fp = fopen(inpath, "r");
  if (!fp) return NULL;
  // init the output
  FILE *out_fp = NULL;
  if (outpath) {
    out_fp = fopen(outpath, "w");
    if (!out_fp) return NULL;
  }

  Compiler *compiler = malloc(sizeof(Compiler));

  compiler->file.path = inpath;
  compiler->flags = flags;
  compiler->file.fp = fp;
  compiler->out_fp = out_fp;

  return compiler;
}

CompilerStatus compile_file(const char *inpath, const char *outpath,
                            int flags) {
  Compiler *compiler = compiler_create(inpath, outpath, flags);
  if (!compiler) return COMPILER_ERROR;

  Lexer lexer = lexer_create(compiler, NULL);
  if (lex(&lexer) != LEXER_OK) {
    return COMPILER_ERROR;
  }

  /* TODO */
  // parsing

  /* TODO */
  // code generation

  // finally free the process

  lexer_free(&lexer);

  fclose(compiler->file.fp);
  fclose(compiler->out_fp);

  free(compiler);

  return COMPILER_OK;
}

void compiler_error(Lexer *l, const char *message, ...) {
  fprintf(stderr, "%s:%i:%i: error:\n", l->pos->fname, l->pos->line, l->pos->col);

  va_list args;
  va_start(args, message);
  vfprintf(stderr, message, args);
  va_end(args);

  printf("\n");

  exit(-1);
}

void compiler_warning(Lexer *l, const char *message, ...) {
  fprintf(stderr, "%s:%i:%i: warning:\n", l->pos->fname, l->pos->line,
          l->pos->col);

  va_list args;
  va_start(args, message);
  vfprintf(stderr, message, args);
  va_end(args);

  printf("\n");
}
