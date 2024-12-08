#include <iostream>

#include "program.hpp"

int main(int argc, char **argv) {
  Program program;

  for (int i = 1; i < argc; ++i) {
    program.add_file(argv[i]);
  }

  File *file = program.first_source;

  while (file != NULL) {
    std::cout << file->name << ":" << std::endl;
    std::cout << "```\n" << file->content << "```" << std::endl;
    file = file->next;
  }

  return 0;
}
