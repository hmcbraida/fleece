#include "fleece.hpp"

#define DOWNC(new_type, old_name, new_name) new_type* new_name = dynamic_cast<new_type*>(old_name);

void print_fleece_node(const FleeceNode* node) {
  if (node->type() == FleeceNodeType::String) {
    DOWNC(const FleeceStringNode, node, node_str)

    printf("\"%s\"", node_str->value.c_str());
  } else if (node->type() == FleeceNodeType::Array) {
    DOWNC(const FleeceArrayNode, node, node_array)
    printf("[");
    short initial = 1;
    for (const FleeceNode* child : node_array->children) {
      if (!initial) {
        printf(",");
      } else {
        initial = 0;
      }
      print_fleece_node(child);
    }
    printf("]");
  } else if (node->type() == FleeceNodeType::Object) {
    DOWNC(const FleeceObjectNode, node, node_object)
    printf("{");
    short initial = 1;
    for (const auto& [key, child] : node_object->children) {
      if (!initial) {
        printf(",");
      } else {
        initial = 0;
      }
      printf("\"%s\":", key.c_str());
      print_fleece_node(child);
    }
    printf("}");
  } else if (node->type() == FleeceNodeType::Number) {
    DOWNC(const FleeceNumberNode, node, node_number)
    printf("%g", node_number->value);
  } else if (node->type() == FleeceNodeType::Null) {
    printf("null");
  } else {
    throw "Unknown fleece node type";
  }
}
