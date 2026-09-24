#pragma once
#include "celestia/ast/names/Name.hpp"
#include "string"

namespace celestia::ast {

struct Identifier : NameNode {
  std::string str;
  
  std::string get_str() const override { return str; }

  explicit Identifier(std::string n) : NameNode(NodeKind::Identifier), str(std::move(n)) {}
};

} // namespace celestia::ast