#ifndef smv_smvcc_compiler_h
#define smv_smvcc_compiler_h

#include <stdio.h>
#include <stdlib.h>

typedef enum CompilerStatus {
  COMPILER_OK,
  COMPILER_ERROR,
} CompilerStatus;

typedef struct CompileProcess {
  int flags;

  struct InputFile {
    FILE *fp;
    const char *path;
  } file;

  FILE *out_fp;

} CompileProcess;

CompilerStatus compile_file(const char *inpath, const char *outpath, int flags);
CompileProcess *compile_process_create(const char *inpath, const char *outpath,
                                       int flags);

#endif