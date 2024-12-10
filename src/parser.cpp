#include "parser.hpp"

Parser::Parser() {}

Parser::Parser(std::vector<Token> *tokens_) : tokens(tokens_), index(0) {
  advance();
}

void Parser::advance() { current = (*tokens)[index++]; }

void Parser::consume(TokenType expected) {
  if (current.type == expected)
    advance(); // Consume the token.
  // else throw error TODO
}

Code *Parser::parse_code() {
  Code *code = new Code();
  while (index < tokens->size()) {
    Statement *statement = parse_statement();
    code->statements.push_back(*statement);
  }
  return code;
}

Statement *Parser::parse_statement() {
  Statement *statement = new Statement();
  if (current.type == TokenType::KEYWORD) {
    // this is nested because I'd probably extend it later
    // or not, idk
    // if (int, float, ... or something (TODO)) {
    statement->type = ASSIGNMENT;
    statement->assignment = *parse_assignment();
    // }
  }
  return statement;
}

Assignment *Parser::parse_assignment() {
  consume(TokenType::KEYWORD);
  std::string identifier;
  advance();
  consume(TokenType::IDENTIFIER);
  consume(TokenType::SYMBOL);
  Expression *expr = parse_expression();
  Assignment *assignment = new Assignment();
  assignment->identifier = identifier;
  assignment->expression = *expr;
  delete expr;
  return assignment;
}

Expression *Parser::parse_expression() {}

Factor *Parser::parse_factor() {}

void Parser::parse() { parse_code(); }

Parser::~Parser() {}