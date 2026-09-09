#pragma once

#include "celestia/ast/Node.hpp"
#include "celestia/ast/declarations/ModuleDeclaration.hpp"

#include <vector>

namespace celestia::ast {

class RootNode : public Node {
public:
  std::vector<ModuleDeclaration *> modules;

  RootNode() : Node(NodeKind::Root) {};
};

} // namespace celestia::ast