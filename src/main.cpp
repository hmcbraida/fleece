#include "debug.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <cstdio>

void json_test(const char* text) {
  printf("Input text:\n%s\n", text);
  auto tokens = lex_bytes(text);
  printf("Result of lexing:\n");
  print_lex_output(tokens);
  ParsedNode* res = parse_tokens(tokens);
  printf("Result of parsing:\n");
  print_parsed_node(res);
  printf("\n\n");
  delete res;
}

int main() {
  try {
    json_test("\"hello { world }\"");
    json_test("[\"hello wor{ld\", \"another element\", [\"subarray\"]]");
    json_test("{\"hello\": \"world\", \"another\": \"pair\", \"this time\": "
              "[\"two\", \"things\", {\"and\": \"this ;)\"}]}");
  } catch (char const* c) {
    printf("%s\n", c);
  }

  return 0;
}
