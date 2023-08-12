#include <stdio.h>
#include <string.h>

#include "compiler.h"

int main(int argc, char **argv) {
  // temporary
  // this isn't exactly good code but eh
  if (argc < 3) {
    printf("%s (v0.0.1): usage:\n%s <filename>.c <outname>\n", argv[0], argv[0]);
    return 1;
  }
  // ...otherwise

  CompilerStatus success = compile_file(argv[1], argv[2], 0);

  if(success == COMPILER_ERROR) {
    printf("errors handling code\n");
    return 1;
  }
  // ...otherwise

  printf("wow this works!\n");


  return 0;
}