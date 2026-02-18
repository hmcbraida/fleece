#include "debug.hpp"
#include "fleece.hpp"
#include <cassert>

void test(const char* text) {
  auto res = parse(text);

  print_fleece_node(res);
  printf("\n");
}

void example() {
  const char* test_input =
      "{\"my_array\": [1, 2, 3], \"my_object\": {\"my_number\": 42, "
      "\"my_string\": \"salutations planet\"}}";

  auto result = dynamic_cast<FleeceObjectNode*>(parse(test_input));

  auto first_child = result->children["my_array"];
  assert(first_child->type() == FleeceNodeType::Array);
  printf("Array has %zu children\n",
         dynamic_cast<FleeceArrayNode*>(first_child)->children.size());

  printf("As for the object:\n");
  auto second_child = result->children["my_object"];
  assert(second_child->type() == FleeceNodeType::Object);
  for (const auto& [key, value] :
       dynamic_cast<FleeceObjectNode*>(second_child)->children) {
    printf("Key: %s; ", key.c_str());
    switch (value->type()) {
    case FleeceNodeType::Number:
      printf("Value: %f\n", dynamic_cast<FleeceNumberNode*>(value)->value);
      break;
    case FleeceNodeType::String:
      printf("Value: %s\n",
             dynamic_cast<FleeceStringNode*>(value)->value.c_str());
      break;
    default:
      assert(1 == 2);
      break;
    }
  }
}

void tests() {
  try {
    test("[ 45, 23 ,  \"STRING\"]");
    test("\"hello world\"");
    test("{\"hello\"      : 42, \"world\"     : 23}");
    test("{\"hello\"      : 42, \"world\"  :  null}");
  } catch (char const* c) {
    printf("%s\n", c);
  }
}

int main() {
  tests();
  example();

  return 0;
}
