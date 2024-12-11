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
  };
  // I hate this, maybe I'll extend C just to remove this
  Expression *expression;
};

struct Term {
  Factor first; // , second; // (TODO)
};

struct Expression {
  std::vector<Term> terms;
  enum ExpressionType {
    ADDITION,
  } type;
};

struct Assignment {
  int identifier; // index of identifer in program's symbol table
  Expression expression;
};

struct Statement {
  enum StatementType {
    ASSIGNMENT,
  } type;
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
  Code code;

  void advance();
  void consume(const TokenType expected);
  void consume_keyword(const KeywordType expected);
  void consume_symbol(const char expected);
  Code parse_code();
  Statement parse_statement();
  Assignment parse_assignment();
  Expression parse_expression();
  Term parse_term();
  Factor parse_factor();

  void print_statement(const Statement statement);
  void print_assignment(const Assignment assignment);
  void print_expression(const Expression expression);
  void print_term(const Term term);
  void print_factor(const Factor factor);

public:
  Parser(Program *p, std::vector<Token> *tokens_);
  void parse();
  void print_code();
};

#endif