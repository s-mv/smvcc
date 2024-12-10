#ifndef smv_smvcc_parser_hpp
#define smv_smvcc_parser_hpp

#include <vector>

#include "lexer.hpp"

typedef struct Program Program;
typedef struct Expression Expression;

struct Factor {
  enum FactorType {
    NUMBER,
    IDENTIFIER,
    EXPRESSION,
  } type;
  union {
    long long number;
    // I hate this, maybe I'll extend C just to remove this
    Expression *expression;
  };
};

struct Term {
  Factor first; // , second; // (TODO)
};

struct Expression {
  Term first, second;
  enum ExpressionType {
    ADDITION,
  } type;
};

struct Assignment {
  std::string identifier;
  Expression expression;
};

enum StatementType {
  ASSIGNMENT,
};

struct Statement {
  StatementType type;
  Assignment assignment;
};

struct Code {
  std::vector<Statement> statements;
};

class Parser {
private:
  Program *program;
  std::vector<Token> *tokens; // reminder: I don't like references
  int index;                  // token index
  Token current;

  void advance();
  void consume(TokenType expected);
  Code *parse_code();
  Statement *parse_statement();
  Assignment *parse_assignment();
  Expression *parse_expression();
  Factor *parse_factor();

public:
  Parser();
  Parser(std::vector<Token> *tokens_);
  void parse();
  ~Parser();
};

#endif