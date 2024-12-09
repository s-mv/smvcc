#ifndef smv_smvcc_lexer_hpp
#define smv_smvcc_lexer_hpp

#include <string>
#include <vector>

typedef class Program Program;
typedef class File File;

enum TokenType {
  SYMBOL,
  KEYWORD,
  IDENTIFIER,
  INT_LITERAL,
  FLOAT_LITERAL,
};

struct Position {
  unsigned int row = 1;
  unsigned int column = 1;
  int index = 0;
};

struct Token {
  TokenType type;
  union {
    long long num;
    double fnum;
    char character;
  };
  std::string str;
  Position position;
};

class Lexer {
private:
  std::vector<Token> tokens;
  Program *program;
  File *current_file;
  Position *position;

  void lex_file(File *file); // lex just one File object
  inline char current();
  inline char peek(int step = 1);
  inline char next(int step = 1);
  void advance(int step = 1);

public:
  Lexer(Program *p);
  void lex(); // lex entire program
  void print_tokens();
  // ~Lexer();
};

#endif