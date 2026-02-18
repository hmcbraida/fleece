#ifndef FLEECE_FLEECE_H_

#define FLEECE_FLEECE_H_

#include <string>
#include <unordered_map>
#include <vector>

enum FleeceNodeType {
  String,
  Number,
  Object,
  Array,
  Null,
};

class FleeceNode {
public:
  virtual ~FleeceNode() {};
  virtual FleeceNodeType type() const = 0;
};

class FleeceStringNode : public FleeceNode {
public:
  std::string value;
  FleeceStringNode(std::string val) : value(val) {}
  virtual FleeceNodeType type() const override;
};

class FleeceNumberNode : public FleeceNode {
public:
  double value;
  FleeceNumberNode(double val) : value(val) {}
  virtual FleeceNodeType type() const override;
};

class FleeceObjectNode : public FleeceNode {
  ~FleeceObjectNode();
public:
  std::unordered_map<std::string, FleeceNode*> children;
  FleeceObjectNode(std::unordered_map<std::string, FleeceNode*> children)
      : children(children) {}
  virtual FleeceNodeType type() const override;
};

class FleeceArrayNode : public FleeceNode {
  ~FleeceArrayNode();
public:
  std::vector<FleeceNode*> children;
  FleeceArrayNode(std::vector<FleeceNode*> children) : children(children) {}
  virtual FleeceNodeType type() const override;
};

class FleeceNullNode : public FleeceNode {
public:
  FleeceNullNode() = default;
  virtual FleeceNodeType type() const override;
};

FleeceNode* parse(std::string input);

#endif
