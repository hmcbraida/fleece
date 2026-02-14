#include "parser.hpp"
#include "lexer.hpp"
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

/*
  Return a boolean expression based on whether the TokenNode is the given type.
*/
#define SIMPLE_EQ(token, token_id)                                             \
  (token.t == TokenType::Simple) && (token.inner.s == SimpleToken::token_id)

#define IS_WHITESPACE(token)                                                   \
  (token.t == TokenType::Char &&                                               \
   (token.inner.c == ' ' || token.inner.c == '\n' || token.inner.c == '\t'))

/*
  Checks for a pair starting with initial, ending with final; parse inside.

  The `parse_fn` is run on whatever tokens lie on the inside.
*/
#define PROCESS_INSIDE(initial, parse_fn)                                      \
  if (SIMPLE_EQ(first_token, initial)) {                                       \
    ParsedNode* array_node = parse_fn(tokens, start + 1, end, process_end);    \
    while ((*process_end < end) && IS_WHITESPACE(tokens[*process_end])) {      \
      (*process_end)++;                                                        \
    }                                                                          \
    return array_node;                                                         \
  }

char simple_token_to_char(const SimpleToken& token) {
  switch (token) {
  case SimpleToken::LCURLYB:
    return '{';
  case SimpleToken::RCURLYB:
    return '}';
  case SimpleToken::LSQUAREB:
    return '[';
  case SimpleToken::RSQUAREB:
    return ']';
  case SimpleToken::COLON:
    return ':';
  case SimpleToken::COMMA:
    return ',';
  case SimpleToken::QUOTE:
    return '"';
  }
}

ParsedNode* parse_string(TokenSequence tokens, size_t start, size_t end,
                         size_t* process_end) {
  // std::string result;
  ParsedNode* result = new ParsedNode{
      .t = ParsedNodeType::STRING,
      .inner = ParsedNodeInner{.s = ParsedStringNode{.val = std::string("")}}};
  result->inner.s.val.reserve(end - start);
  // result.reserve(end - start);

  for (size_t idx = start; idx < end; idx++) {
    const TokenNode& token = tokens[idx];

    if (SIMPLE_EQ(token, QUOTE)) {
      *process_end = idx + 1;
      return result;
    }

    if (token.t == TokenType::Simple) {
      result->inner.s.val += simple_token_to_char(token.inner.s);
    } else {
      result->inner.s.val += token.inner.c;
    }
  }

  throw "Unexpected end of string";
}

ParsedNode* parse_array(TokenSequence tokens, size_t start, size_t end,
                        size_t* process_end) {
  ParsedNode* result = new ParsedNode{
      .t = ParsedNodeType::ARRAY,
      .inner = ParsedNodeInner{
          .a = ParsedArrayNode{.children = std::vector<ParsedNode*>()}}};

  size_t next_child_start = start;
  while (1) {
    size_t child_end;

    auto child = parse_tokens(tokens, next_child_start, end, &child_end);

    result->inner.a.children.push_back(child);

    // End of the list of children if we meet a closing brace
    if (SIMPLE_EQ(tokens[child_end], RSQUAREB)) {
      *process_end = child_end + 1;

      return result;
    }

    // We're not at the end, so there better be a comma now...
    if (!SIMPLE_EQ(tokens[child_end], COMMA)) {
      throw "Expected comma splitting array children";
    }

    next_child_start = child_end + 1;
  }
}

ParsedNode* parse_object(TokenSequence tokens, size_t start, size_t end,
                         size_t* process_end) {
  ParsedNode* result = new ParsedNode{
      .t = ParsedNodeType::OBJECT,
      .inner = ParsedNodeInner{
          .o = ParsedObjectNode{
              .children = std::unordered_map<std::string, ParsedNode*>()}}};

  size_t next_pair_start = start;
  while (1) {
    size_t key_end;
    // bit icky contructing then deconstructing a node for this...
    ParsedNode* key_node = parse_tokens(tokens, next_pair_start, end, &key_end);
    if (key_node->t != ParsedNodeType::STRING) {
      throw "Expected string as object key";
    }
    std::string str_key = key_node->inner.s.val;
    // we have the key, time to dispose.
    delete key_node;

    // we expect a colon here to separate the key from the value.
    if (!SIMPLE_EQ(tokens[key_end], COLON)) {
      throw "Expected colon separating key and value";
    }

    size_t val_end;
    ParsedNode* val_node = parse_tokens(tokens, key_end + 1, end, &val_end);
    result->inner.o.children.insert({str_key, val_node});

    if (SIMPLE_EQ(tokens[val_end], RCURLYB)) {
      *process_end = val_end + 1;

      return result;
    }

    // we expect a comma here to separate from the next key-val pair.
    if (!SIMPLE_EQ(tokens[val_end], COMMA)) {
      throw "Expected comma separating key value pairs";
    }

    next_pair_start = val_end + 1;
  }
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
  while (IS_WHITESPACE(tokens[start])) {
    start++;
  }

  const TokenNode& first_token = tokens[start];
  const TokenNode& last_token = tokens[end - 1];

  if (first_token.t != TokenType::Simple) {
    throw "Invalid token (1)";
  }

  if (last_token.t != TokenType::Simple) {
    throw "Invalid token (2)";
  }

  // Check for string
  PROCESS_INSIDE(QUOTE, parse_string)

  // Check for array
  PROCESS_INSIDE(LSQUAREB, parse_array)

  // Check for object
  PROCESS_INSIDE(LCURLYB, parse_object)

  throw "Unexpected end of input";
}

/*
  Deconstructors for the nodes.
  =============================
*/

// I don't quite understand why this is necessary, but if I don't trivialize
// the deconstructor for the ParsedStringNode, I get a bunch of errors about the
// deconstructor being deleted. Deconstructing the string from the tagged union
// seems to work better ?

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

ParsedObjectNode::~ParsedObjectNode() {
  for (const auto& [key, value] : children) {
    delete value;
  }
}
