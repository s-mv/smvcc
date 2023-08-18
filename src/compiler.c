#include "compiler.h"

CompilerStatus compile_file(const char *inpath, const char *outpath,
                            int flags) {
  CompileProcess *proc = compile_process_create(inpath, outpath, flags);
  if (!proc) return COMPILER_ERROR;

  /* TODO */
  // lexical analysis

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