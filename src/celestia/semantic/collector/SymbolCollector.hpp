#pragma once

#include "celestia/ast/ASTFwd.hpp"
#include "celestia/ast/AstDispacher.hpp"
#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/Compiler.hpp"
#include "celestia/compiler/CompilerEnvironment.hpp"
#include "celestia/semantic/resolver/ContextStack.hpp"
#include "celestia/semantic/scope/Scope.hpp"

namespace celestia::semantic {

struct SymbolCollectorContext {
  Compiler &compiler;
  CompilationUnit &unit;

  ContextStack<ScopeId> stack;

  CompilerEnvironment &env() const { return compiler.environment(); }

  SymbolCollectorContext(Compiler &compiler, CompilationUnit &unit) : compiler(compiler), unit(unit), stack(ScopeId::invalid()) {}
};

class SymbolCollector {

public:
  SymbolCollector(Compiler &compiler, CompilationUnit &unit);

  void collect();

private:
  SymbolCollectorContext context;

  void collect_node(ast::Node *node);
  void collect_root(ast::RootNode *node);
  void collect_module(ast::ModuleDeclaration *node);
  void collect_declaration(ast::Declaration *node);
  void collect_module_init(ast::ModuleInitDeclaration *node);
  void collect_struct(ast::StructDeclaration *node);
  void collect_function(ast::FunctionDeclaration *node);
  void collect_variable(ast::VariableDeclaration *node);
  void collect_type(ast::TypeDeclaration *node);
  void collect_block(ast::BlockStatement *node);

  void collect_pattern(ast::PatternNode *pattern);
  void collect_named_pattern(ast::NamedPattern *pattern);
  void collect_generics(const std::vector<ast::IdentifierNode *> &parameters);
  void collect_field(ast::FieldDeclaration *node);
  SymbolId declare_symbol(const std::string &name, SymbolKind kind, Visibility visibility, ast::Node *node);

  bool can_have_visibility(core::ScopeKind kind, Visibility visibility);
};

} // namespace celestia::semantic