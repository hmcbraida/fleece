#include "parser.hpp"
#include "lexer.hpp"
#include <string>

typedef std::vector<TokenNode> Tokens;

char simple_token_to_char(const SimpleToken& token) {
  using enum SimpleToken;
  switch (token) {
  case LCURLYB:
    return '{';
  case RCURLYB:
    return '}';
  case LSQUAREB:
    return '[';
  case RSQUAREB:
    return ']';
  case COLON:
    return ':';
  case COMMA:
    return ',';
  case QUOTE:
    return '"';
  }
}

ParsedNode* parse_string(Tokens tokens, size_t start, size_t end) {
  std::string result;
  result.reserve(end - start);

  for (size_t idx = start; idx < end; idx++) {
    const TokenNode& token = tokens[idx];

    if (token.t == TokenType::Simple) {
      result += simple_token_to_char(token.inner.s);
    } else {
      result += token.inner.c;
    }
  }

  return new ParsedNode{.t = ParsedNodeType::STRING,
                        .inner = {{.val = result}}};
}

ParsedNode* parse_tokens(Tokens tokens) {
  return parse_tokens(tokens, 0, tokens.size());
}

ParsedNode* parse_tokens(Tokens tokens, size_t start, size_t end) {
  const TokenNode& first_token = tokens[start];
  const TokenNode& last_token = tokens[end - 1];

  if (first_token.t != TokenType::Simple) {
    throw "Invalid token (1)";
  }

  if (last_token.t != TokenType::Simple) {
    throw "Invalid token (2)";
  }

  // ASSERT: last_token.t == TokenType::Simple

  // Check for string
  if (first_token.inner.s == SimpleToken::QUOTE) {
    // unclosed quote mark
    if (last_token.inner.s != SimpleToken::QUOTE) {
      throw "Invalid token (3)";
    }

    ParsedNode* string_node = parse_string(tokens, start + 1, end - 1);

    return string_node;
  }

  throw "Invalid token (4)";

  // if (first_token.t == TokenType::Simple && first_token.inner.s ==
  // SimpleToken::QUOTE)

  return nullptr;
}

ParsedNode::~ParsedNode() {
  if (t == ParsedNodeType::STRING) {
    inner.s.val.~basic_string();
  }
}
