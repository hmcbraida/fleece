#include "lexer.hpp"
#include "parser.hpp"
#include <cstdio>
#include <vector>

#define SIMPLETOKENCASE(token_id)                                              \
  case SimpleToken::token_id:                                                               \
    result = #token_id;                                                        \
    break;

const char* simple_token_str(SimpleToken token) {
  const char* result;
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

    printf(",");
  }
  printf("\n");
}

void print_parsed_node(const ParsedNode* node) {
  if (node->t == ParsedNodeType::STRING) {
    printf("\"%s\"", node->inner.s.val.c_str());
  } else if (node ->t == ParsedNodeType::ARRAY) {
    printf("[");
    short initial = 1;
    for (const ParsedNode* child : node->inner.a.children) {
      if (!initial) {
        printf(",");
      } else {
        initial = 0;
      }
      print_parsed_node(child);
    }
    printf("]");
  } else if (node->t == ParsedNodeType::OBJECT) {
    printf("{");
    short initial = 1;
    for (const auto& [key, child] : node->inner.o.children) {
      if (!initial) {
        printf(",");
      } else {
        initial = 0;
      }
      printf("\"%s\":", key.c_str());
      print_parsed_node(child);
    }
    printf("}");
  } else if (node->t == ParsedNodeType::NUMBER) {
    printf("%g", node->inner.n.val);
  }
}
