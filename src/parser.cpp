#include "parser.hpp"
#include "lexer.hpp"
#include <string>
#include <vector>

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

ParsedNode* parse_array(Tokens tokens, size_t start, size_t end) {
  // WIP!
  std::vector<ParsedNode*> children;

  size_t next_child_start = 0;
  while (1) {
    
  }
}

ParsedNode* parse_tokens(Tokens tokens) {
  size_t process_end;
  auto result = parse_tokens(tokens, 0, tokens.size(), &process_end);

  if (process_end != tokens.size()) {
    throw "Extra data after expected end of input";
  }

  return result;
}

/*
  Return a boolean expression based on whether the TokenNode is the given type.
*/
#define SIMPLE_EQ(token, token_id)                                             \
  (token.t == TokenType::Simple) && (token.inner.s == SimpleToken::token_id)

/*
  Process a sequence of tokens until it picks out an entity.

  Anything after the first atomic node found will be ignored, and `process_end`
  will be written with the end of what was processed.
*/
ParsedNode* parse_tokens(Tokens tokens, size_t start, size_t end,
                         size_t* process_end) {
  const TokenNode& first_token = tokens[start];
  const TokenNode& last_token = tokens[end - 1];

  if (first_token.t != TokenType::Simple) {
    throw "Invalid token (1)";
  }

  if (last_token.t != TokenType::Simple) {
    throw "Invalid token (2)";
  }

  // Check for string
  if (SIMPLE_EQ(first_token, QUOTE)) {
    // Look ahead for the next quotation mark
    size_t next_quote = 0;
    for (size_t lookahead_idx = 1; lookahead_idx < end; lookahead_idx++) {
      if (SIMPLE_EQ(tokens[lookahead_idx], QUOTE)) {
        next_quote = lookahead_idx;
      }
    }
    if (next_quote == 0) {
      throw "No matching quote";
    }

    // start is the position of the first quote
    // next_quote is the position of its pair
    ParsedNode* string_node = parse_string(tokens, start + 1, next_quote);
    *process_end = next_quote + 1;

    return string_node;
  }

  if (first_token.inner.s == SimpleToken::LSQUAREB) {
    if (last_token.inner.s != SimpleToken::RSQUAREB) {
      throw "Invalid token (5)";
    }
  }

  throw "Invalid token (4)";
}

ParsedNode::~ParsedNode() {
  if (t == ParsedNodeType::STRING) {
    inner.s.val.~basic_string();
  }
}
