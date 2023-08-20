#include "compiler.h"

#include <stdarg.h>

#include "lexer.h"

void compiler_error(Lexer *l, const char *message, ...) {
  fprintf(stderr, "%s:%i:%i: error:\n", l->pos.fname, l->pos.line, l->pos.col);

  va_list args;
  va_start(args, message);
  vfprintf(stderr, message, args);
  va_end(args);

  printf("\n");

  exit(-1);
}

void compiler_warning(Lexer *l, const char *message, ...) {
  fprintf(stderr, "%s:%i:%i: warning:\n", l->pos.fname, l->pos.line,
          l->pos.col);

  va_list args;
  va_start(args, message);
  vfprintf(stderr, message, args);
  va_end(args);

  printf("\n");
}

CompilerStatus compile_file(const char *inpath, const char *outpath,
                            int flags) {
  CompileProcess *proc = compile_process_create(inpath, outpath, flags);
  if (!proc) return COMPILER_ERROR;

  Lexer lexer = lexer_create(proc, &default_lexer_fns, NULL);
  if (!lex(&lexer)) {
    return COMPILER_ERROR;
  }

  /* TODO */
  // parsing

  /* TODO */
  // code generation

  // finally free the process

  lexer_free(&lexer);

  fclose(proc->file.fp);
  fclose(proc->out_fp);
  
  free(proc);

  return COMPILER_OK;
}