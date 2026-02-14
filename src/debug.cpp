#include "lexer.hpp"
#include <cstdio>
#include <vector>

#define SIMPLETOKENCASE(token_id)                                              \
  case token_id:                                                               \
    result = #token_id;                                                        \
    break;

const char* simple_token_str(SimpleToken token) {
  const char* result;
  using enum SimpleToken;
  switch (token) {
    SIMPLETOKENCASE(LCURLYB)
    SIMPLETOKENCASE(RCURLYB)
    SIMPLETOKENCASE(LSQUAREB)
    SIMPLETOKENCASE(RSQUAREB)
    SIMPLETOKENCASE(COLON)
    SIMPLETOKENCASE(COMMA)
    SIMPLETOKENCASE(QUOTE)
  }

  return result;
}

void print_lex_output(std::vector<TokenNode>& tokens) {
  for (const TokenNode& token : tokens) {
    switch (token.t) {
    case TokenType::Char:
      printf("'%c'", token.inner.c);
      break;
    case TokenType::Simple:
      printf("%s", simple_token_str(token.inner.s));
      break;
    }

    printf("\n");
  }
}
