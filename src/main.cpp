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

  print_parsed_node(res);
  printf("\n");

  delete res;
}

void simple_array_test() {
  const char example[] =
      "[\"hello wor{ld\", \"another element\", [\"subarray\"]]";
  // const char example[] = "{\"message\": \"hello world\"}";

  auto tokens = lex_bytes(example, sizeof(example) / sizeof(char));

  print_lex_output(tokens);

  ParsedNode* res = parse_tokens(tokens);

  print_parsed_node(res);
  printf("\n");

  delete res;
}

void simple_object_test() {
  const char example[] =
      "{\"hello\": \"world\", \"another\": \"pair\", \"this "
      "time\": [\"two\", \"things\", {\"and\": \"this ;)\"}]}";
  // const char example[] = "{\"message\": \"hello world\"}";
  printf("%s\n", example);

  auto tokens = lex_bytes(example, sizeof(example) / sizeof(char));

  print_lex_output(tokens);

  ParsedNode* res = parse_tokens(tokens);

  print_parsed_node(res);
  printf("\n");

  // if (res->t != ParsedNodeType::OBJECT) {
  //   throw "Expected object at top level";
  // }

  // printf("%zu\n", res->inner.o.children.size());

  delete res;
}

int main() {
  try {
    simple_string_test();
    simple_array_test();
    simple_object_test();
  } catch (char const* c) {
    printf("%s\n", c);
  }

  return 0;
}
