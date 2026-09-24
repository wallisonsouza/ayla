#pragma once

#include "celestia/ast/types/Type.hpp"
#include <vector>

namespace celestia::ast {

struct FunctionType : public Type {

  std::vector<Type *> parameters;
  Type *return_type;

public:
  FunctionType(std::vector<Type *> parameters, Type *return_type) : Type(NodeKind::FunctionType), parameters(std::move(parameters)), return_type(return_type) {}
};

} // namespace celestia::ast