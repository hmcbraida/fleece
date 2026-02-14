#include "lexer.hpp"
#include <cstddef>
#include <vector>

TokenNode simple_token(SimpleToken token) {
  Token token_union = Token{.s = token};
  return TokenNode{
      .t = TokenType::Simple,
      .inner = token_union,
  };
}

TokenNode char_token(char c) {
  Token token_union = Token{.c = c};

  return TokenNode{
      .t = TokenType::Char,
      .inner = token_union,
  };
}

// Lex Case Simple
// Used to pick out single char tokens in `lex_bytes`
#define LCSIMPLE(char, token_id)                                               \
  if (c == char) {                                                             \
    result.push_back(simple_token(SimpleToken::token_id));                     \
    continue;                                                                  \
  }

std::vector<TokenNode> lex_bytes(const char in_bytes[]) {
  std::vector<TokenNode> result = {};

  // TODO: Support '\' escaping

  for (size_t idx = 0; ; idx++) {
    const char c = in_bytes[idx];
    if (c == '\0') {
      break;
    }

    // Special control character
    LCSIMPLE('{', LCURLYB)
    LCSIMPLE('}', RCURLYB)
    LCSIMPLE('[', LSQUAREB)
    LCSIMPLE(']', RSQUAREB)
    LCSIMPLE(':', COLON)
    LCSIMPLE(',', COMMA)
    LCSIMPLE('"', QUOTE)

    if (c == '\0') {
      break;
    }

    // // unknown character
    // // ' ' and '~' are the boundaries of printable characters
    // if (c < ' ' || c > '~') {
    //   throw new LexingException();
    // }

    // Standard character
    result.push_back(char_token(c));
  }

  return result;
}

const char* LexingException::what() const throw() { return "Lexing failed!"; }
