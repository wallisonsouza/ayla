
#include "celestia/compiler/CompilerEnvironment.hpp"
#include "celestia/semantic/id/ids.hpp"

namespace celestia::semantic {
class NameLookup {
public:
  static SymbolId lookup(const CompilerEnvironment &env, ast::NameNode *name, ScopeId scope);

private:
  static SymbolId lookup_qualified_name(const CompilerEnvironment &env, ast::QualifiedName *name);
  static SymbolId lookup_identifier_name(const CompilerEnvironment &env, ScopeId scope, ast::Identifier *name);
};

} // namespace celestia::semantic