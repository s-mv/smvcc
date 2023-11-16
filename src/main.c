#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "tests.h"

int main(int argc, char **argv) {
#ifdef smv_smvcc_tests

  // 100% TODO (well not 100% but most of it)

  CompilerStatus success;

  test("numeric lexing");
  success = compile_file("tests/numeric_lexing.c", "tests/bin/numeric_lexing",
                         COMPILER_FLAG_NONE);

  printf("Result: %s\n", success == COMPILER_OK ? "ok" : "failure");
  endtest();

#else

  // temporary
  // this isn't exactly good code but eh
  // TODO? compiler commands compiler? :P
  if (argc < 3) {
    printf("%s (v0.0.1): usage:\n%s <filename>.c <outname>\n", argv[0],
           argv[0]);
    return 1;
  }
  // ...otherwise

  CompilerStatus success = compile_file(argv[1], argv[2], COMPILER_FLAG_NONE);

  if (success == COMPILER_ERROR) {
    printf("errors handling code...\n");
    return 1;
  }
  // ...otherwise

  if (success != COMPILER_OK) {
    printf("unknown error while compiling...\n");
    return 1;
  }
  // ...otherwise

#endif

  return 0;
}