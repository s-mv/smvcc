#ifndef smv_smvcc_program_hpp
#define smv_smvcc_program_hpp

#include <string>

#include "lexer.hpp"
#include "parser.hpp"
#include "preprocessor.hpp"

// binary encoded for absolutely no reason :D
enum FileType {
  NORMAL = 1,               // normal, new file
  MACRO = 1 << 1,           // is it a macro
  MACRO_EXPANSION = 1 << 2, // is it a macro expansion?
  INCLUDE = 1 << 3,         // is it an include?
  CONTINUED = 1 << 4,       // continued, not a new file
};

struct File {
  std::string name;
  std::string content;
  uint8_t type = NORMAL;
  bool continuation = false; // is it a continuation of the last file?
  bool header = false;       // is it a header?
  bool macro = false;        // is it the expansion of a macro?
  Position position = {};    // where does it start?
  File *next = NULL;
  File *last = NULL;
};

struct Macro {
  std::string name;
  enum MacroType {
    DEFINITION, // #define a    expr
    FUNCTION,   // #define a(x) f(x)
  } type;
  File *file;
};

typedef std::vector<Macro> MacroTable;

class Program {
private:
  Lexer lexer;
  Parser parser;
  Preprocessor preprocessor;

public:
  // later make these private
  File *first_source;
  File *last_source; // for convenience

  SymbolTable table;
  MacroTable macros;

  Program();
  bool add_file(std::string filename);
  bool add_file(std::string filename, std::string content);
  void compile();
  ~Program();

  File *get_source();
};

#endif
