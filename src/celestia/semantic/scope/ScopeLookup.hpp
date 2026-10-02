#include "celestia/compiler/CompilerEnvironment.hpp"
#include "celestia/semantic/id/ids.hpp"

namespace celestia::semantic {

class ScopeLookup {
public:
  static SymbolId find_local(const CompilerEnvironment &env, ScopeId scope_id, std::string_view name);

  static SymbolId lookup(const CompilerEnvironment &env, ScopeId scope_id, std::string_view name);
};

} // namespace celestia::semantic