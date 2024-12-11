#include "preprocessor.hpp"

#include "program.hpp"

void Preprocessor::preprocess_file(File *file) {
  // TODO, brainstorm
}

Preprocessor::Preprocessor(Program *p) {}

void Preprocessor::process() {
  File *source = program->get_source();
  while (program->first_source != NULL) {
    preprocess_file(source);
    source = source->next;
  }
}
