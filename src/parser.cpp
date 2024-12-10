#include "parser.hpp"

#include <iostream>

#include "lexer.hpp"
#include "program.hpp"

Parser::Parser(Program *p, std::vector<Token> *tokens_)
    : program(p), tokens(tokens_), index(0) {}

void Parser::advance() { current = tokens->at(index++); }

void Parser::consume(TokenType expected) {
  if (current.type == expected)
    advance(); // Consume the token.
  // else throw error TODO
}

void Parser::consume_keyword(KeywordType expected) {
  if (current.type == KEYWORD and keywords[current.index].second == expected)
    advance();
  // error TODO
}

void Parser::consume_symbol(char expected) {
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
  }
  return statement;
}

Assignment Parser::parse_assignment() {
  consume_keyword(TYPE_SPECIFIER);
  int identifier;
  advance();
  consume(IDENTIFIER);
  consume(SYMBOL);
  Expression expr = parse_expression();
  Assignment assignment = Assignment();
  assignment.identifier = identifier;
  assignment.expression = expr;
  return assignment;
}

Expression Parser::parse_expression() {
  Expression expr = Expression();
  expr.first = parse_term();
  consume_symbol('+');
  expr.second = parse_term();
  expr.type = Expression::ADDITION;
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
    advance();
  } else if (current.type == SYMBOL and current.character == '(') {
    consume_symbol('(');
    factor.type = Factor::EXPRESSION;
    // I hate this 2.0
    factor.expression = new Expression(parse_expression());
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

Parser::~Parser() {
  if (!program)
    return;

  for (auto &statement : code.statements) {
    if (statement.type == Statement::ASSIGNMENT) {
      Expression *expr = &statement.assignment.expression;

      auto freeExpression = [](Expression *expression) {
        if (expression->first.first.type == Factor::EXPRESSION) {
          delete expression->first.first.expression;
        }

        if (expression->second.first.type == Factor::EXPRESSION) {
          delete expression->second.first.expression;
        }

        delete expression;
      };

      freeExpression(expr);
    }
  }
}