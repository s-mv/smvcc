#include "compiler.h"

/* just for future clarification: cprocess = *c*ompile*process* */

CompileProcess *compile_process_create(const char *inpath, const char *outpath,
                                       int flags) {
  // read the file
  FILE *fp = fopen(inpath, "r");
  if (!fp) return NULL;
  // init the output
  FILE *out_fp = NULL;
  if (outpath) {
    out_fp = fopen(outpath, "w");
    if (!out_fp) return NULL;
  }

  CompileProcess *proc = malloc(sizeof(CompileProcess));

  proc->flags = flags;
  proc->file.fp = fp;
  proc->out_fp = out_fp;

  return proc;
}
