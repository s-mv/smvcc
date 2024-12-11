#include "parser.hpp"

#include <iostream>

#include "lexer.hpp"
#include "program.hpp"

Parser::Parser(Program *p, std::vector<Token> *tokens_)
    : program(p), tokens(tokens_), index(0) {}

void Parser::advance() { current = tokens->at(index++); }

void Parser::consume(const TokenType expected) {
  if (current.type == expected)
    advance(); // Consume the token.
  // else throw error TODO
}

void Parser::consume_keyword(const KeywordType expected) {
  if (current.type == KEYWORD and keywords[current.index].second == expected)
    advance();
  // error TODO
}

void Parser::consume_symbol(const char expected) {
  if (current.type == SYMBOL and current.character == expected)
    advance();
}

Code Parser::parse_code() {
  Code code = Code();
  while (index < tokens->size()) {
    Statement statement = parse_statement();
    code.statements.push_back(statement);
  }
  return code;
}

Statement Parser::parse_statement() {
  Statement statement = Statement();
  if (current.type == KEYWORD and
      keywords[current.index].second == TYPE_SPECIFIER) {

    statement.type = Statement::ASSIGNMENT;
    statement.assignment = parse_assignment();
    consume_symbol(';');
  }
  return statement;
}

Assignment Parser::parse_assignment() {
  consume_keyword(TYPE_SPECIFIER);
  consume(IDENTIFIER);
  int identifier = current.index;
  consume(SYMBOL);
  Expression expr = parse_expression();
  Assignment assignment = Assignment();
  assignment.identifier = identifier;
  assignment.expression = expr;
  return assignment;
}

Expression Parser::parse_expression() {
  Expression expr = Expression();
  expr.terms.push_back(parse_term());
  while (current.type == SYMBOL and current.character == '+') {
    expr.type = Expression::ADDITION;
    expr.terms.push_back(parse_term());
  }
  return expr;
}

Term Parser::parse_term() {
  Term term = Term();
  term.first = parse_factor();
  return term;
}

Factor Parser::parse_factor() {
  Factor factor = Factor();
  if (current.type == INT_LITERAL) {
    factor.type = Factor::NUMBER;
    factor.number = current.num;
    advance(); // Move to next token
  } else if (current.type == IDENTIFIER) {
    factor.type = Factor::IDENTIFIER;
    factor.number = current.index;
    advance();
  } else if (current.type == SYMBOL and current.character == '(') {
    consume_symbol('(');
    factor.type = Factor::EXPRESSION;
    // I hate this 2.0
    Expression *expression = new Expression(parse_expression());
    factor.expression = std::move(expression);
    consume_symbol(')');
  } else {
    // TODO error
  }
  return factor;
}

void Parser::parse() {
  advance();
  code = parse_code();
}

void Parser::print_code() {
  std::cout << "Code:\n";
  for (Statement statement : code.statements) {
    print_statement(statement);
  }
}

void Parser::print_statement(const Statement statement) {
  switch (statement.type) {
  case Statement::ASSIGNMENT:
    std::cout << "  Assignment: ";
    print_assignment(statement.assignment);
    break;
    // Add more statement types as needed
  }
}

void Parser::print_assignment(const Assignment assignment) {
  std::cout << program->table[assignment.identifier].name << " = ";
  print_expression(assignment.expression);
  std::cout << ";\n";
}

void Parser::print_expression(const Expression expression) {
  if (expression.type == Expression::ADDITION)
    std::cout << "(";

  for (size_t i = 0; i < expression.terms.size(); ++i) {
    if (i > 0)
      std::cout << " + ";
    print_term(expression.terms[i]);
  }

  if (expression.type == Expression::ADDITION)
    std::cout << ")";
}

void Parser::print_term(const Term term) { print_factor(term.first); }

void Parser::print_factor(const Factor factor) {
  switch (factor.type) {
  case Factor::NUMBER:
    std::cout << factor.number;
    break;
  case Factor::IDENTIFIER:
    std::cout << program->table[factor.number].name;
    break;
  case Factor::EXPRESSION:
    print_expression(*factor.expression);
    break;
  }
}
