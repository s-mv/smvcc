#include "parser.h"

Parser* parser;

// helper functions
static void advance();

ParserStatus parse(Parser* p) { parser = p; }

void parser_free(Parser* p) {
  // TODO
}

void advance() {}
