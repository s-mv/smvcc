#include "lexer.hpp"

#include <iostream>

#include "program.hpp"

const std::vector<char> single_char_symbols = {
    '=', '+', '-', '*', ';', '(', ')', '{', '}', '>', '<',
};

const std::vector<char[2]> double_char_symbols = {

};

// this is so annoying
const std::vector<std::pair<std::string, KeywordType>> keywords = {
    {"int", TYPE_SPECIFIER},
    // {"char", TYPE_SPECIFIER},
    // {"float", TYPE_SPECIFIER},
    // {"double", TYPE_SPECIFIER},
    // {"void", TYPE_SPECIFIER},

    // {"const", TYPE_QUALIFIER},
    // {"volatile", TYPE_QUALIFIER},
    // {"restrict", TYPE_QUALIFIER},

    // {"static", STORAGE_SPECIFIER},
    // {"extern", STORAGE_SPECIFIER},
    // {"register", STORAGE_SPECIFIER},

    // {"if", CONTROL_FLOW},
    // {"else", CONTROL_FLOW},
    // {"switch", CONTROL_FLOW},
    // {"case", CONTROL_FLOW},

    // {"for", LOOP_CONTROL},
    // {"while", LOOP_CONTROL},
    // {"do", LOOP_CONTROL},

    // {"break", JUMP_STATEMENT},
    // {"continue", JUMP_STATEMENT},
    // {"return", JUMP_STATEMENT},

    // {"sizeof", MEMORY_MANAGEMENT},

    // {"typedef", DECLARATION_KEYWORD},
    // {"struct", DECLARATION_KEYWORD},
    // {"union", DECLARATION_KEYWORD},
    // {"enum", DECLARATION_KEYWORD},
};

void Lexer::lex_file(File *file) {
  current_file = file;
  int len = current_file->content.length();
  position = &current_file->position;

  for (; position->index < len; position->index++) {
    //// lex literals
    // lex numbers
    // (only integers for now)
    if (isdigit(current())) {
      Token token = Token();
      token.position = *position;

      int start = position->index;
      while (isdigit(next()))
        advance();

      int end = position->index;
      long long number =
          std::stoll(current_file->content.substr(start, end - start));

      token.type = INT_LITERAL;
      token.num = number;

      tokens.push_back(token);
    }
    // lex characters (TODO)
    // lex strings (TODO)

    //// lex symbols
    // TODO:
    // maybe TokenType::SYMBOL and TokenTYPE::OPERATOR should be two different
    // types? idk right now
    for (char symbol : single_char_symbols) {
      if (current() != symbol)
        continue;

      Token token = Token();
      token.type = SYMBOL;
      token.character = symbol;
      token.position = *position;
      tokens.push_back(token);
      advance();
    }

    //// lex keywords
    if (isalpha(current())) {
      for (int i = 0; i < keywords.size(); i++) {
        std::string keyword = keywords.at(i).first;
        if (current_file->content.length() - position->index < keyword.length())
          continue;

        std::string word =
            current_file->content.substr(position->index, keyword.length());

        if (keyword.compare(word) != 0)
          continue;

        Token token = Token();
        token.position = *position;
        token.type = KEYWORD;
        token.index = i;
        tokens.push_back(token);
        advance(keyword.length());
      }
    }

    //// lex identifiers
    if (isalpha(current()) or current() == '_') {
      int start = position->index;

      while (isalnum(next()) or peek() == '_')
        ;

      int end = position->index;
      std::string ident = current_file->content.substr(start, end - start);
      bool symtable_has_ident = false;
      int index = program->table.size();

      for (int i = 0; i < program->table.size(); i++) {
        Symbol symbol = program->table.at(i);

        if (symbol.name != ident)
          continue;

        symtable_has_ident = true;
        index = i;
        break;
      }

      if (!symtable_has_ident) {
        Symbol symbol = Symbol();
        symbol.name = ident;
        symbol.type = UNDEFINED;
        program->table.push_back(symbol);
      }

      Token token = Token();
      token.type = IDENTIFIER;
      token.index = index;
      token.position = *position;
      tokens.push_back(token);
    }

    //// everything else is just an error (except for whitespace)
    // TODO: error handling (in the far, far future)
  }
}

char Lexer::current() { return current_file->content[position->index]; }

char Lexer::peek(int step) {
  return current_file->content[position->index + step];
}
char Lexer::next(int step) {
  advance();
  return current();
}

void Lexer::advance(int step) {
  for (int i = 0; i < step; i++) {
    if (current() == '\n') {
      position->column = 1;
      position->row++;
    } else
      position->column++;
  }
  position->index += step;
}

Lexer::Lexer(Program *p) {
  tokens = std::vector<Token>();
  program = p;
  position = {};
}

void Lexer::lex() {
  File *source = program->get_source();
  while (source != NULL) {
    lex_file(source);
    source = source->next;
  }
}

void Lexer::print_tokens() {
  for (Token token : tokens) {
    switch (token.type) {
    case KEYWORD:
      std::cout << "Keyword: " << keywords[token.index].first << std::endl;
      break;

    case IDENTIFIER:
      std::cout << "Identifier: " << program->table[token.index].name
                << std::endl;
      break;

    case INT_LITERAL:
      std::cout << "Integer: " << token.num << std::endl;
      break;

    case SYMBOL:
      std::cout << "Symbol: " << token.character << std::endl;
      break;

    default:
      break;
    }
  }
}

std::vector<Token> *Lexer::get_tokens() { return &tokens; }
