#include "program.hpp"

#include <fstream>
#include <iostream>

Program::Program()
    : first_source(NULL), last_source(NULL), lexer(Lexer(this)),
      parser(Parser(this, lexer.get_tokens())), table(SymbolTable()) {}

bool Program::add_file(std::string filename) {
  std::ifstream file(filename);
  if (!file or !file.is_open())
    return false;

  std::string content = std::string(std::istreambuf_iterator<char>(file),
                                    std::istreambuf_iterator<char>());
  add_file(filename, content);
  file.close();
  return true;
}

bool Program::add_file(std::string filename, std::string content) {
  if (first_source == NULL) {
    first_source = new File();
    first_source->name = filename;
    first_source->content = content;
    first_source->next = NULL;
    first_source->last = NULL;
    last_source = first_source;
    return true;
  }

  // otherwise...

  File *new_file = new File();
  new_file->name = filename;
  new_file->content = content;
  new_file->next = NULL;
  new_file->last = last_source;
  last_source->next = new_file;
  last_source = new_file;
  return true;
}

Program::~Program() {
  while (first_source != NULL) {
    File *file = first_source->next;
    delete first_source;
    first_source = file;
  }
}

File *Program::get_source() { return first_source; }

void Program::compile() {
  lexer.lex();
  parser.parse();
  // parser.print_ast();
}
