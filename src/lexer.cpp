#include "lexer.hpp"

#include "program.hpp"

void Lexer::lex_file(File *file) {
  int len = file->content.length();

  for (int i = 0; i < len; i++) {
    // TODO
  }
}

Lexer::Lexer(Program &p) {
  tokens = std::vector<Token>();
  program = &p;
  position = {};
}

void Lexer::lex() {
  File *source = program->get_source();
  while (source != NULL) {
    lex_file(source);
    source = source->next;
  }
}
