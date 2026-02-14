#ifndef FLEECE_PARSER_H_

#define FLEECE_PARSER_H_

#include "lexer.hpp"
#include <string>
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

union ParsedNodeInner {
  ParsedStringNode s;
  ParsedArrayNode a;

  // destruction handled by tagged union `ParsedNode`
  ~ParsedNodeInner() {}
};

enum ParsedNodeType {
  STRING,
  ARRAY,
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
