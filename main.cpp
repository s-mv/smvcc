#include <iostream>

#include "program.hpp"

int main(int argc, char **argv) {
  Program program;

  for (int i = 1; i < argc; ++i) {
    program.add_file(argv[i]);
  }

  program.compile();

  return 0;
}
