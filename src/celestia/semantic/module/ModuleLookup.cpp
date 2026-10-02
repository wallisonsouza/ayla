#include "celestia/semantic/module/ModuleLookup.hpp"

#include "celestia/ast/NodeCast.hpp"
#include "celestia/semantic/scope/ScopeLookup.hpp"

namespace celestia::semantic {

SymbolId NameLookup::lookup(const CompilerEnvironment &env, ast::NameNode *name, ScopeId scope) {
  assert(name);

  switch (name->kind) {

  case ast::NodeKind::Identifier: return lookup_identifier_name(env, scope, ast::as<ast::Identifier>(name));

  case ast::NodeKind::QualifiedName: return lookup_qualified_name(env, ast::as<ast::QualifiedName>(name));

  default: assert(false && "Unsupported NameNode kind lookup"); return SymbolId::invalid();
  }
}

SymbolId NameLookup::lookup_identifier_name(const CompilerEnvironment &env, ScopeId scope, ast::Identifier *name) {
  assert(name);

  return ScopeLookup::lookup(env, scope, name->get_str());
}

SymbolId NameLookup::lookup_qualified_name(const CompilerEnvironment &env, ast::QualifiedName *name) {
  assert(name);

  const auto &parts = name->parts;

  if (parts.size() < 2) return SymbolId::invalid();

  std::string module_name;

  for (std::size_t i = 0; i + 1 < parts.size(); ++i) {

    if (!module_name.empty()) module_name += '.';

    module_name += parts[i]->str;
  }

  ModuleId module_id = env.modules.find(module_name);

  if (!module_id.is_valid()) return SymbolId::invalid();

  const auto &module = env.modules.get(module_id);

  return ScopeLookup::lookup(env, module.scope_id(), parts.back()->str);
}

} // namespace celestia::semantic