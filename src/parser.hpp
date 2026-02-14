#ifndef FLEECE_PARSER_H_

#define FLEECE_PARSER_H_

#include "lexer.hpp"
#include <string>
#include <unordered_map>
#include <vector>

struct ParsedNode;

struct ParsedStringNode {
  std::string val;

  // destruction handled by tagged union `ParsedNode`
  ~ParsedStringNode() {}
};

struct ParsedArrayNode {
  std::vector<ParsedNode*> children;

  ~ParsedArrayNode();
};

struct ParsedObjectNode {
  std::unordered_map<std::string, ParsedNode*> children;

  ~ParsedObjectNode();
};

struct ParsedNumericNode {
  double val;
};

union ParsedNodeInner {
  ParsedStringNode s;
  ParsedArrayNode a;
  ParsedObjectNode o;
  ParsedNumericNode n;

  // destruction handled by tagged union `ParsedNode`
  ~ParsedNodeInner() {}
};

enum ParsedNodeType {
  STRING,
  ARRAY,
  OBJECT,
  NUMBER,
};

struct ParsedNode {
  ParsedNodeType t;
  ParsedNodeInner inner;

  ~ParsedNode();
};

struct ParsedTree {
  ParsedNode root;
};

ParsedNode* parse_tokens(TokenSequence tokens);
ParsedNode* parse_tokens(TokenSequence tokens, size_t start, size_t end, size_t* process_end);

#endif
