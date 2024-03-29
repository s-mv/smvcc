#ifndef smv_smvcc_parser_h
#define smv_smvcc_parser_h

#include "lexer.h"
#include "list.h"

typedef enum NodeType {
  NODE_FACTOR,      // TODO
  NODE_EXPRESSION,  // TODO
  NODE_FUNCTION,    // TODO
  NODE_ARGUMENTS,   // TODO
  NODE_CALL,        // TODO
  NODE_RETURN,      // TODO
} NodeType;

typedef struct Factor {
  long long number;
} Factor;
typedef struct Expression {
  long long number1;
  char operator;
  long long number2;
} Expression;
typedef struct Function {
  int type;
  char *identifier;
  Arguments arguments;
} Function;
typedef struct Parameter {
  char *identifier;
  Expression expression;
} Parameter;
typedef struct Parameters {
  Parameter parameter;
  Parameter next;
  char last;  // bool
} Parameters;
typedef struct Argument {
  int type;
  Parameter parameter;
} Argument;
typedef struct Arguments {
  Argument argument;
  Argument next;
  char last;
} Arguments;
typedef struct Return {
  Expression expression;
} Return;

typedef struct Node {
  // this is the input from our lexer
  List *tokens;

  NodeType type;
  Node left;
  Node right;

} Node;

typedef enum ParserStatus {
  PARSER_OK = 0,
  PARSER_ERROR,
} ParserStatus;

typedef struct Parser {
  List tokens;
  Node AST;
  Node *current;
} Parser;

ParserStatus parse(Parser *p);
void parser_print(Parser *p);
void parser_free(Parser *p);

#endif
