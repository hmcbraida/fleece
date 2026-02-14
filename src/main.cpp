#include "debug.hpp"
#include "lexer.hpp"

int main() {
  const char example[] = "\"hello wor{ld\"";
  // const char example[] = "{\"message\": \"hello world\"}";

  auto tokens = lex_bytes(example, sizeof(example) / sizeof(char));

  print_lex_output(tokens);

  return 0;
}
