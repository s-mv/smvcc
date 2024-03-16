#include "parser.h"

Parser *parser;

void advance();

ParserStatus parse(Parser* p) {
  parser = p;
}

void parser_free(Parser* p) {
  // TODO
}
