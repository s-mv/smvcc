#ifndef smv_smvcc_program_hpp
#define smv_smvcc_program_hpp

#include <string>

#include "common.hpp"
#include "lexer.hpp"

class Program {
private:
  Lexer lexer;

public:
  // later make these private
  File *first_source;
  File *last_source; // for convenience

  Program();
  bool add_file(std::string filename);
  bool add_file(std::string filename, std::string content);
  void run();
  ~Program();

  File *get_source();
};

#endif
