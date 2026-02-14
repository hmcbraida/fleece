#include "debug.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include <cstdio>

int main() {
  const char example[] = "\"hello wor{ld\"";
  // const char example[] = "{\"message\": \"hello world\"}";

  auto tokens = lex_bytes(example, sizeof(example) / sizeof(char));

  print_lex_output(tokens);

  ParsedNode* res = parse_tokens(tokens);

  printf("%s\n", res->inner.s.val.c_str());

  delete res;

  return 0;
}
