#pragma once

#include "celestia/core/ids/Id.hpp"

namespace celestia::semantic {
struct ModuleIdTag{};
using ModuleId = Id<ModuleIdTag>;

struct SymbolIdTag {};
using SymbolId = Id<SymbolIdTag>;

struct TypeIdTag {};
using TypeId = Id<TypeIdTag>;

struct ScopeIdTag {};
using ScopeId = Id<ScopeIdTag>;

struct CompilationTag {};
using CompilationUnitId = Id<CompilationTag>;
} // namespace celestia::semantic
