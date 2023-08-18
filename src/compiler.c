#include "compiler.h"

#include "lexer.h"

CompilerStatus compile_file(const char *inpath, const char *outpath,
                            int flags) {
  CompileProcess *proc = compile_process_create(inpath, outpath, flags);
  if (!proc) return COMPILER_ERROR;

  Lexer lexer = lexer_create(proc, &default_lexer_fns, NULL);
  if(!lex(proc)) {
    return COMPILER_ERROR;
  }

  /* TODO */
  // parsing

  /* TODO */
  // code generation

  // finally free the process

  fclose(proc->file.fp);
  fclose(proc->out_fp);
  free(proc);

  return COMPILER_OK;
}