# Fleece: simple JSON parser

Simple JSON parser.

Usage:

```cpp
#include "fleece.hpp"
#include <cassert>

int main() {
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
```
