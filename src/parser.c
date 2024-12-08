#include "parser.h"

#include <stdio.h>

Parser* parser;

// helper functions
static Token peek();
static Token next();
static void advance();

// grammar
static void expression();
static void function();
static void arguments();
static void parameters();
static void call();
static void return_statement();

ParserStatus parse(Parser* p) {
  parser = p;
  while (peek().type != TOKEN_END) {
    printf("H %d\n", peek().type);
  }
}

void parser_print(Parser* p) {
  // TODO
}

void parser_free(Parser* p) {
  // TODO
}

static Token peek() { ; }
static Token next() { ; }
static void advance() { ; }

static void expression() {
  if (peek().type == TOKEN_IDENTIFIER) {
    advance();
    // add identifier to parser->AST;
  }

  if (peek().type == TOKEN_OPERATOR && peek().character == '(')
    ;
}
static void function() { ; }
static void arguments() { ; }
static void parameters() { ; }
static void call() { ; }
static void return_statement() { ; }
