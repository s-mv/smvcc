#ifndef smv_smvcc_lexer_hpp
#define smv_smvcc_lexer_hpp

#include <string>
#include <vector>

#include "common.hpp"

typedef class Program Program;

enum TokenType {
  SYMBOL,
  KEYWORD,
  IDENTIFIER,
  LITERAL,
};

struct Token {
  TokenType type;
  union {
    long long num;
    double fnum;
    char character;
  };
  std::string str;
};

class Lexer {
private:
  std::vector<Token> tokens;
  Program *program;
  Position position;
  void lex_file(File *file); // lex just one File object

public:
  Lexer(Program &p);
  void lex(); // lex entire program
};

#endif