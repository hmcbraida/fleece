#include "parser.hpp"
#include "lexer.hpp"
#include <cstdio>
#include <string>
#include <vector>

/*
  Return a boolean expression based on whether the TokenNode is the given type.
*/
#define SIMPLE_EQ(token, token_id)                                             \
  (token.t == TokenType::Simple) && (token.inner.s == SimpleToken::token_id)

#define SIMPLE_LOOKAHEAD(var_name, token_id)                                   \
  size_t var_name = 0;                                                         \
  for (size_t lookahead_idx = start + 1; lookahead_idx < end;                  \
       lookahead_idx++) {                                                      \
    if (SIMPLE_EQ(tokens[lookahead_idx], token_id)) {                          \
      var_name = lookahead_idx;                                                \
      break;                                                                   \
    }                                                                          \
  }                                                                            \
  if (var_name == 0) {                                                         \
    throw "No matching " #token_id;                                            \
  }

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

ParsedNode* parse_string(TokenSequence tokens, size_t start, size_t end) {
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

ParsedNode* parse_array(TokenSequence tokens, size_t start, size_t end) {
  ParsedNode* result = new ParsedNode{
      .t = ParsedNodeType::ARRAY,
      .inner = ParsedNodeInner{
          .a = ParsedArrayNode{.children = std::vector<ParsedNode*>()}}};

  size_t next_child_start = start;
  while (1) {
    size_t child_end;

    auto child = parse_tokens(tokens, next_child_start, end, &child_end);

    result->inner.a.children.push_back(child);

    // End of the list of children
    if (child_end == end) {
      break;
    }

    // We're not at the end, so there better be a comma now...
    if (!SIMPLE_EQ(tokens[child_end], COMMA)) {
      throw "Expected comma splitting array children";
    }

    next_child_start = child_end + 1;
  }

  return result;
}

ParsedNode* parse_tokens(TokenSequence tokens) {
  size_t process_end;
  auto result = parse_tokens(tokens, 0, tokens.size(), &process_end);

  if (process_end != tokens.size()) {
    throw "Extra data after expected end of input";
  }

  return result;
}

/*
  Process a sequence of tokens until it picks out an entity.

  Anything after the first atomic node found will be ignored, and `process_end`
  will be written with the end of what was processed.
*/
ParsedNode* parse_tokens(TokenSequence tokens, size_t start, size_t end,
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
    SIMPLE_LOOKAHEAD(next_quote, QUOTE)

    // printf("%zu,%zu\n", start + 1, next_quote);

    // start is the position of the first quote
    // next_quote is the position of its pair
    ParsedNode* string_node = parse_string(tokens, start + 1, next_quote);
    *process_end = next_quote + 1;

    return string_node;
  }

  if (SIMPLE_EQ(first_token, LSQUAREB)) {
    SIMPLE_LOOKAHEAD(next_sqbrack, RSQUAREB)

    ParsedNode* array_node = parse_array(tokens, start + 1, next_sqbrack);
    *process_end = next_sqbrack + 1;

    return array_node;
  }

  throw "Unexpected end of input";
}

ParsedNode::~ParsedNode() {
  if (t == ParsedNodeType::STRING) {
    inner.s.val.~basic_string();
  }
}

ParsedArrayNode::~ParsedArrayNode() {
  for (auto child : children) {
    delete child;
  }
}
