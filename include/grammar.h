#ifndef smv_smvcc_grammar_h
#define smv_smvcc_grammar_h

/* this is a temporary file, probably
 * I have no clue what I'm doing
 * 
 * pain
 */

#include "list.h"

typedef struct Factor Factor;
typedef struct Term Term;
typedef struct Expression Expression;
typedef struct Function Function;
typedef struct Parameter Parameter;
typedef List Parameters;
typedef struct Argument Argument;
typedef List Arguments;
typedef struct Return Return;
typedef struct Program Program;

struct Factor {
  union {
    long long number;
    double real;
  };
  bool is_float;
};

struct Term {
  Factor left;
  char operator;  // * or /
  Factor right;
};

struct Expression {
  Factor left;
  char operator;  // + or -
  Factor right;
};

struct Function {
  int type;
  char *identifier;
  Arguments arguments;
};

struct Parameter {
  char *identifier;
  Expression expression;
};

struct Argument {
  int type;
  Parameter *parameter;
};

struct Return {
  Expression expression;
};

struct Program {

};

#endif