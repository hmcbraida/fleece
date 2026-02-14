#ifndef FLEECE_LEXER_H_

#define FLEECE_LEXER_H_

#include<exception>
#include <vector>

/*
  Type definitions for lexing output:
  ===================================
*/

enum SimpleToken {
  LCURLYB,
  RCURLYB,
  LSQUAREB,
  RSQUAREB,
  COLON,
  COMMA,
  QUOTE,
};

typedef char CharToken;

union Token {
  SimpleToken s;
  CharToken c;
};

enum TokenType {
  Simple,
  Char,
};

/*
  Our output from lexing is a vector of these.
*/
struct TokenNode {
  TokenType t;
  Token inner;
};

/*
  Lex a JSON document of bytes into a series of symbols.

  At present, this can only deal with ASCII JSON, and will raise errors if the
  provided document is not valid ASCII.
*/
std::vector<TokenNode> lex_bytes(const char* in_bytes, size_t in_bytes_len);


class LexingException : public std::exception {
  virtual const char* what() const throw();
};

#endif
