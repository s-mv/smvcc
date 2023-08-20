#ifndef smv_smvcc_compiler_h
#define smv_smvcc_compiler_h

#include <stdio.h>
#include <stdlib.h>

#include "token.h"

// to avoid cyclic include issues
// the header will be included in the source file
typedef struct Lexer Lexer;

typedef enum CompilerStatus {
  COMPILER_OK,
  COMPILER_ERROR,
} CompilerStatus;

typedef struct CompileProcess {
  int flags;

  Position pos;
  struct InputFile {
    FILE *fp;
    const char *path;
  } file;

  FILE *out_fp;

} CompileProcess;

void compiler_error(Lexer *l, const char *message, ...);
void compiler_warning(Lexer *l, const char *message, ...);
CompilerStatus compile_file(const char *inpath, const char *outpath, int flags);
CompileProcess *compile_process_create(const char *inpath, const char *outpath,
                                       int flags);

#endif