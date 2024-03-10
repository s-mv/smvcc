#ifndef smv_smvcc_tests_preprocessors_h
#define smv_smvcc_tests_preprocessors_h

struct Person {
#ifdef TESTING
  char *ID;
#endif
  int age;
};

#endif