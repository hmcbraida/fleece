#include <math.h>
#include <string>
#include <unordered_map>

#include "fleece.hpp"

short convert_char_int(const char& c) {
  const short i = c - '0';

  if (i < 0 || i > 9) {
    return -1;
  }

  return i;
}

inline bool is_whitespace(const char& c) {
  return c == ' ' || c == '\t' || c == '\n';
}

FleeceNode* parse_number(std::string input, size_t start, size_t end,
                         size_t* process_end) {
  // this represents the largest major digit
  short exponent = -1;
  // list of digits in the number
  std::vector<short> digits = {};
  // flag that determines if we have passed the decimal point
  bool passed_decimal = false;
  // we use idx to loop over the tokens
  size_t idx = start;
  for (; idx < end; idx++) {
    auto& c = input[idx];

    // if we're still in the integer portion, we allow a . to move use into the
    // fractional portion
    if (!passed_decimal && c == '.') {
      passed_decimal = true;
      continue;
    }

    short val = convert_char_int(c);
    // non-numeric means we stop processing as a number.
    if (val == -1) {
      break;
    }

    digits.push_back(val);
    // Mark an increase in the whole digit count
    if (!passed_decimal) {
      exponent++;
    }
  }

  // If this wasn't a number, return a nullptr to mark no number processed.
  if (digits.size() == 0) {
    return nullptr;
  }

  // add up the digits
  double result = 0.0;
  for (auto& d : digits) {
    double val = pow(10.0f, exponent) * d;
    result += val;
    exponent--;
  }

  // strip any whitespace
  for (; idx < end && is_whitespace(input[idx]); idx++) {
  }
  *process_end = idx;

  return new FleeceNumberNode(result);
}

// Forward declaring this so we can use it before defined.
FleeceNode* parse(std::string input, size_t start, size_t end,
                  size_t* process_end);

FleeceNode* parse_array(std::string input, size_t start, size_t end,
                        size_t* process_end) {

  std::vector<FleeceNode*> children;

  size_t next_child_start = start;
  while (1) {
    size_t child_end;

    auto child = parse(input, next_child_start, end, &child_end);

    children.push_back(child);

    // End of the list of children if we meet a closing brace
    if (input[child_end] == ']') {
      *process_end = child_end + 1;

      return new FleeceArrayNode(children);
    }

    // We're not at the end, so there better be a comma now...
    if (input[child_end] != ',') {
      throw "Expected comma splitting array children";
    }

    next_child_start = child_end + 1;
  }
}

FleeceNode* parse_string(std::string input, size_t start, size_t end,
                         size_t* process_end) {
  std::string result;

  size_t idx = start;
  for (; idx < end; idx++) {
    const char& c = input[idx];

    if (c == '\"') {
      *process_end = idx + 1;
      return new FleeceStringNode(result);
    }

    result += c;
  }

  throw "Unexpected end of string";
}

FleeceNode* parse_object(std::string input, size_t start, size_t end,
                         size_t* process_end) {
  std::unordered_map<std::string, FleeceNode*> result;

  size_t next_pair_start = start;
  while (1) {
    size_t key_end;
    // bit icky contructing then deconstructing a node for this...
    FleeceNode* key_node = parse(input, next_pair_start, end, &key_end);
    if (key_node->type() != FleeceNodeType::String) {
      throw "Expected string as object key";
    }
    std::string str_key = dynamic_cast<FleeceStringNode*>(key_node)->value;
    // we have the key, time to dispose.
    delete key_node;

    // we expect a colon here to separate the key from the value.
    if (input[key_end] != ':') {
      throw "Expected colon separating key and value";
    }

    size_t val_end;
    FleeceNode* val_node = parse(input, key_end + 1, end, &val_end);
    result.insert({str_key, val_node});

    if (input[val_end] == '}') {
      *process_end = val_end + 1;

      return new FleeceObjectNode(result);
    }

    // we expect a comma here to separate from the next key-val pair.
    if (input[val_end] != ',') {
      throw "Expected comma separating key value pairs";
    }

    next_pair_start = val_end + 1;
  }
}

FleeceNode* parse_null(std::string input, size_t start, size_t end,
                       size_t* process_end) {
  if ((end - start) < 4) {
    return nullptr;
  }

  if (input[start] == 'n' && input[start + 1] == 'u' &&
      input[start + 2] == 'l' && input[start + 3] == 'l') {
    *process_end = start + 4;
    return new FleeceNullNode();
  }

  return nullptr;
}

FleeceNode* parse(std::string input, size_t start, size_t end,
                  size_t* process_end) {
  FleeceNode* result = nullptr;

  // Trim leading whitespace
  for (; start < end && is_whitespace(input[start]); start++) {
  }

  if (input[start] == '[') {
    // must be an array
    result = parse_array(input, start + 1, end, process_end);
  } else if (input[start] == '\"') {
    result = parse_string(input, start + 1, end, process_end);
  } else if (input[start] == '{') {
    result = parse_object(input, start + 1, end, process_end);
  } else {
    // Try parsing as number
    result = parse_number(input, start, end, process_end);

    // Finally, try null
    if (!result) {
      result = parse_null(input, start, end, process_end);
    }
  }

  if (result == nullptr) {
    throw "Could not parse input";
  }

  // Trim trailing whitespace
  for (; *process_end < end && is_whitespace(input[*process_end]);
       (*process_end)++) {
  }

  return result;
}

FleeceNode* parse(std::string input) {
  const size_t start = 0;
  const size_t end = input.size();
  size_t process_end;

  auto result = parse(input, start, end, &process_end);

  if (process_end != end) {
    throw "Unexpected data at end";
  }

  return result;
}

// String methods implementation:

FleeceNodeType FleeceStringNode::type() const { return FleeceNodeType::String; }

// Number methods implementation

FleeceNodeType FleeceNumberNode::type() const { return FleeceNodeType::Number; }

// Object methods implementation

FleeceNodeType FleeceObjectNode::type() const { return FleeceNodeType::Object; }
FleeceObjectNode::~FleeceObjectNode() {
  for (const auto& [key, child] : children) {
    delete child;
  }
}

// Array methods implementation

FleeceNodeType FleeceArrayNode::type() const { return FleeceNodeType::Array; }
FleeceArrayNode::~FleeceArrayNode() {
  for (const auto& child : children) {
    delete child;
  }
}

// Null implementation

FleeceNodeType FleeceNullNode::type() const { return FleeceNodeType::Null; }
