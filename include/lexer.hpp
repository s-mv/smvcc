#ifndef smv_smvcc_lexer_hpp
#define smv_smvcc_lexer_hpp

#include <string>
#include <unordered_map>
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

enum DataType {
  UNDEFINED,
  INT_32,
};

struct Symbol {
  std::string name;
  DataType type;
  // auto scope; // TODO
};

// this took a long, long conversation with claude to finalize
// if there's a better, more optimal way to deal with keywords such that parsing
// isn't a pain, I don't know about it
enum KeywordType {
  TYPE_SPECIFIER,    // int, char, float, etc.
  TYPE_QUALIFIER,    // const, volatile, etc.
  STORAGE_SPECIFIER, // static, extern, register

  CONTROL_FLOW,   // if, else, switch, etc.
  LOOP_CONTROL,   // for, while, do
  JUMP_STATEMENT, // break, continue, return

  MEMORY_MANAGEMENT, // sizeof, malloc, free

  DECLARATION_KEYWORD, // typedef, struct, union
};

struct Keyword {};

typedef std::vector<Symbol> SymbolTable;

struct Token {
  TokenType type;
  union {
    long long num;
    unsigned long long unum;
    double fnum;
    int index; // symbol/keyword index
    char character;
  };
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
  std::vector<Token> *get_tokens();
  // ~Lexer();
};

// TODO, maybe make this a map
// but then indexing would require strings
extern const std::vector<std::pair<std::string, KeywordType>> keywords;

#endif
