#ifndef smv_testmv_h
#define smv_testmv_h

/*
 * Author: Shreerang Vaidya
 * Originally a part of: https://github.com/s-mv/testmv/
 * testmv, a simple header-only testing library for C, by smv! :D
 */

// NOTE:
// if testing isn't enabled just completely ignore the tests
// this makes testing easier since the developer doesn't have to remove tests
// but simply comment out `#define testmv_enable` from their code!

#ifdef smv_testmv_enable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *test_name;
char *session_name;
int passed = 0;
int failed = 0;
#endif

// extends a macro, hence _smv_testmv
static inline void _smv_testmv_assert(char did_pass, const char *condition) {
#ifdef smv_testmv_enable
  if (did_pass) passed++;
  printf("(testmv) Session %s: Condition %s %s -- (%s)\n", session_name,
         test_name, did_pass ? "passed" : "failed", condition);
#endif
}

static inline void testmv_start(const char *name) {
#ifdef smv_testmv_enable
  int len = (strlen(name) + 1) * sizeof(char);
  test_name = malloc(len);
  memcpy(test_name, name, len);
#endif
}

static inline void testmv_end() {
#ifdef smv_testmv_enable
  free(test_name);
#endif
}

static inline void testmv_session_start(const char *name) {
#ifdef smv_testmv_enable
  int len = strlen(name) + 1;
  session_name = malloc(len);
  memcpy(session_name, name, len);
  printf("(testmv) Session started: %s\n", name);
#endif
}

static inline void testmv_session_end() {
#ifdef smv_testmv_enable
  printf("(testmv) Session ended: %s\n", session_name);
  printf("(testmv) Session summary:\n");
  if (failed > 0)
    printf("         Session FAILED!\n");
  else
    printf("         Session PASSED!\n");

  printf(
      "         Passed: %d\n"
      "         Failed: %d\n",
      passed, failed);

  // reset everything
  passed = 0;
  failed = 0;
  free(session_name);
#endif
}

#define testmv_assert(_condition) \
  _smv_testmv_assert((_condition), (#_condition))

#endif