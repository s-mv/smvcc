#define TESTING
#include "preprocessors.h"

int main() {
  struct Person person = {.ID = "smv", .age = 0};
  return person.age;
}