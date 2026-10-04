#pragma once

#include "Declaration.hpp"
#include "celestia/ast/expression/Expression.hpp"
#include "celestia/ast/patterns/PatternNode.hpp"
#include "celestia/syntax/parser/DeclarationSpecifiers.hpp"

namespace celestia::ast {

struct VariableDeclaration : Declaration {
  PatternNode *pattern;
  Expression *initializer;
  DeclarationSpecifiers specifiers;

  VariableDeclaration(PatternNode *pattern, Expression *initializer, DeclarationSpecifiers specifiers = {})
      : Declaration(NodeKind::VariableDeclaration), pattern(pattern), initializer(initializer), specifiers(specifiers) {}
};
;

} // namespace celestia::ast
