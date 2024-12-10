#ifndef smv_smvcc_program_hpp
#define smv_smvcc_program_hpp

#include <string>

#include "lexer.hpp"
#include "parser.hpp"

// binary encoded for absolutely no reason :D
enum FileType {
  NORMAL = 1,
  MACRO = 1 << 1,           // is it a macro
  MACRO_EXPANSION = 1 << 2, // is it a macro expansion?
};

struct File {
  std::string name;
  std::string content;
  uint8_t type = NORMAL;
  bool continuation = false; // is it a continuation of the last file?
  bool macro = false;        // is it the expansion of a macro?
  Position position = {};    // where does it start?
  File *next = NULL;
  File *last = NULL;
};

class Program {
private:
  Lexer lexer;
  Parser parser;

public:
  // later make these private
  File *first_source;
  File *last_source; // for convenience

  SymbolTable table;

  Program();
  bool add_file(std::string filename);
  bool add_file(std::string filename, std::string content);
  void compile();
  ~Program();

  File *get_source();
};

#endif
