#ifndef FLEECE_PARSER_H_

#define FLEECE_PARSER_H_

#include "lexer.hpp"
#include <string>
#include <vector>

struct ParsedStringNode {
  std::string val;

  // destruction handled by tagged union `ParsedNode`
  ~ParsedStringNode() {}
};

union ParsedNodeInner {
  ParsedStringNode s;

  // destruction handled by tagged union `ParsedNode`
  ~ParsedNodeInner() {}
};

enum ParsedNodeType {
  STRING
};

struct ParsedNode {
  ParsedNodeType t;
  ParsedNodeInner inner;

  ~ParsedNode();
};

struct ParsedTree {
  ParsedNode root;
};

ParsedNode* parse_tokens(std::vector<TokenNode> tokens);
ParsedNode* parse_tokens(std::vector<TokenNode> tokens, size_t start, size_t end, size_t* process_end);

#endif
