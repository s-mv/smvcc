#ifndef smv_smvcc_preprocessor_hpp
#define smv_smvcc_preprocessor_hpp

typedef struct File File;
typedef class Program Program;

class Preprocessor {
  Program *program;
  Position position;

  void preprocess_file(File *file);

public:
  Preprocessor(Program *p);
  void process();
};

#endif
