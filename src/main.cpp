#include "debug.hpp"
#include "fleece.hpp"
#include <cstdio>

void test(const char* text) {
  auto res = parse(text);

  print_fleece_node(res);
  printf("\n");
}

int main() {
  try {
    test("[ 45, 23 ,  \"STRING\"]");
    test("\"hello world\"");
    test("{\"hello\"      : 42, \"world\"     : 23}");
    test("{\"hello\"      : 42, \"world\"  :  null}");
  } catch (char const* c) {
    printf("%s\n", c);
  }
}
