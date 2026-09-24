#pragma once

#include "celestia/ast/ASTFwd.hpp"
#include "celestia/ast/declarations/EnumDeclaration.hpp"
#include "celestia/ast/names/Generic.hpp"
#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/Compiler.hpp"
#include "celestia/semantic/collector/SymbolCollectorContext.hpp"
#include "celestia/semantic/scope/Scope.hpp"

namespace celestia::semantic {

enum class DeclareResultKind {
  Success,
  Redeclaration,
  InvalidScope,
  CreationFailed,
};
struct DeclareResult {
  DeclareResultKind kind;
  SymbolId symbol = SymbolId::invalid();
};

class SymbolCollector {

public:
  SymbolCollector(Compiler &compiler, CompilationUnit &unit);

  void collect();

private:
  SymbolCollectorContext context;
  bool has_symbol(const std::string &name) const;
  void collect_node(ast::Node *node);
  void collect_root(ast::RootNode *node);
  void collect_module(ast::ModuleDeclaration *node);
  void collect_declaration(ast::Declaration *node);
  void collect_module_init(ast::ModuleInitDeclaration *node);
  void collect_struct_declaration(ast::StructDeclaration *node);
  void collect_function_declaration(ast::FunctionDeclaration *node);
  void collect_variable_declaration(ast::VariableDeclaration *node);
  void collect_type_declaration(ast::TypeDeclaration *node);
  void collect_block_statement(ast::BlockStatement *node);
  void collect_capability_declaration(ast::CapabilityDeclaration *node);
  void collect_impl_declaration(ast::ImplDeclaration *node);
  void collect_enum_variant(ast::EnumVariant *node);
  void collect_enum_declaration(ast::EnumDeclaration *node);
  void collect_named_pattern(ast::NamedPattern *pattern);
  void collect_field_declaration(ast::FieldDeclaration *node);
  void collect_generic_parameter(ast::GenericParameter *node);

  SymbolId declare_named_symbol(ast::Identifier *name, SymbolKind kind, Visibility visibility, ast::Node *node);
  SymbolId declare_symbol(const std::string &name, SymbolKind kind, Visibility visibility, ast::Node *node);

  ScopeId enter_scope(core::ScopeKind kind, ast::Node *node);
};

} // namespace celestia::semantic