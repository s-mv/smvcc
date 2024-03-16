#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

#ifdef smv_smvcc_tests
#define smv_testmv_enable
#include "testmv.h"
#endif

int main(int argc, char **argv) {
#ifndef smv_smvcc_tests
  if (argc < 2) {
    printf("%s: error: no input files\n", argv[0]);
    return 0;
  }

  // TODO: move file operations into a function

  FILE *fp = fopen(argv[1], "r");

  if (fp == NULL) {
    fprintf(stderr, "Error opening file %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  fseek(fp, 0, SEEK_END);
  int len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char *buffer = (char *)malloc((len + 1) * sizeof(char));

  if (buffer == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    fclose(fp);
    return EXIT_FAILURE;
  }

  char bytes = fread(buffer, sizeof(char), len, fp);

  if (bytes != len) {
    fprintf(stderr, "Error reading file %s\n", argv[1]);
    free(buffer);
    fclose(fp);
    return EXIT_FAILURE;
  }

  buffer[len] = '\0';
  fclose(fp);

  Lexer lexer = {
      .source = buffer,
      .srclen = len,
      .file = argv[1],
      .tokens = list_create(sizeof(Token)),
      .position = position_initial,
  };

  lex(&lexer);

  lexer_print(&lexer);

#else
// TODO: setup sane testing mechanism
// testmv_session_start("lexer");
// testmv_start("");
// testmv_end();
// testmv_session_end();
#endif
  return 0;
}
