#include "debug.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <cstdio>

void simple_string_test() {
  const char example[] = "\"hello wor{ld\"";
  // const char example[] = "{\"message\": \"hello world\"}";

  auto tokens = lex_bytes(example, sizeof(example) / sizeof(char));

  print_lex_output(tokens);

  ParsedNode* res = parse_tokens(tokens);

  printf("%s\n", res->inner.s.val.c_str());

  delete res;
}

void simple_array_test() {
  const char example[] = "[\"hello wor{ld\", \"another element\"]";
  // const char example[] = "{\"message\": \"hello world\"}";

  auto tokens = lex_bytes(example, sizeof(example) / sizeof(char));

  print_lex_output(tokens);

  ParsedNode* res = parse_tokens(tokens);

  if (res->t != ParsedNodeType::ARRAY) {
    throw "Expected array as top level";
  }

  size_t n_children = res->inner.a.children.size();
  printf("Array has length %zu\n", n_children);
  for (const ParsedNode* t : res->inner.a.children) {
    printf("%s\n", t->inner.s.val.c_str());
  }

  delete res;
}

int main() {
  try {
    simple_string_test();
    simple_array_test();
  } catch (char const* c) {
    printf("%s\n", c);
  }

  return 0;
}
