#ifndef smv_smvcc_compiler_h
#define smv_smvcc_compiler_h

#include "lexer.h"
#include "parser.h"

typedef struct Compiler {
  Lexer lexer;
  Parser parser;
} Compiler;

#endif