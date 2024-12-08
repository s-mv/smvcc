#ifndef smv_smvcc_common_hpp
#define smv_smvcc_common_hpp

#include <string>

struct Position {
  unsigned int row = 1;
  unsigned int column = 1;
  int index = 0;
  bool macro = false;
};

struct File {
  std::string name;
  std::string content;
  bool continuation = false; // is it a continuation of the last file?
  bool macro = false;        // is it the expansion of a macro?
  Position position = {};    // where does it start?
  File *next = NULL;
  File *last = NULL;
};

#endif
