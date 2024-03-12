#include <stdio.h>

#ifdef smv_smvcc_tests
#define smv_testmv_enable
#endif
#include "tests.h"

int main(int argc, char **argv) {
#ifndef smv_smvcc_tests
  if (argc < 2) {
    printf("%s: error: no input files\n", argv[0]);
    return 0;
  }
  // TODO: next steps for production
#else
  testmv_session_start("lexer");
  testmv_session_end();
#endif
  return 0;
}
