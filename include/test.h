#ifndef smv_smvcc_test_h
#define smv_smvcc_test_h

#define test(_name)                       \
  do printf("Running test: %s\n", _name); \
  while (0)

#define endtest() printf("Test completed.\n")

#endif