#pragma once

#include "celestia/compiler/TypeManager.hpp"
#include "celestia/compiler/UnitManager.hpp"
#include "celestia/language/LanguageDefinition.hpp"
#include "celestia/semantic/module/ModuleManager.hpp"
#include "celestia/semantic/scope/ScopeManager.hpp"
#include "celestia/semantic/symbol/SymbolManager.hpp"

struct BuiltinTypes {
  celestia::semantic::TypeId int_type = celestia::semantic::TypeId::invalid();
  celestia::semantic::TypeId uint_type = celestia::semantic::TypeId::invalid();

  celestia::semantic::TypeId int8_type = celestia::semantic::TypeId::invalid();
  celestia::semantic::TypeId int16_type = celestia::semantic::TypeId::invalid();
  celestia::semantic::TypeId int32_type = celestia::semantic::TypeId::invalid();
  celestia::semantic::TypeId int64_type = celestia::semantic::TypeId::invalid();

  celestia::semantic::TypeId uint8_type = celestia::semantic::TypeId::invalid();
  celestia::semantic::TypeId uint16_type = celestia::semantic::TypeId::invalid();
  celestia::semantic::TypeId uint32_type = celestia::semantic::TypeId::invalid();
  celestia::semantic::TypeId uint64_type = celestia::semantic::TypeId::invalid();

  celestia::semantic::TypeId f32_type = celestia::semantic::TypeId::invalid();
  celestia::semantic::TypeId f64_type = celestia::semantic::TypeId::invalid();

  celestia::semantic::TypeId bool_type = celestia::semantic::TypeId::invalid();
  celestia::semantic::TypeId char_type = celestia::semantic::TypeId::invalid();
  celestia::semantic::TypeId string_type = celestia::semantic::TypeId::invalid();
  celestia::semantic::TypeId void_type = celestia::semantic::TypeId::invalid();
};

struct BuiltinIntrinsics {
  celestia::semantic::SymbolId array = celestia::semantic::SymbolId ::invalid();
  celestia::semantic::SymbolId map = celestia::semantic::SymbolId ::invalid();
  celestia::semantic::SymbolId set = celestia::semantic::SymbolId ::invalid();
  celestia::semantic::SymbolId ref = celestia::semantic::SymbolId ::invalid();
};

struct CompilerEnvironment {

  std::filesystem::path root;

  celestia::LanguageDefinition language;

  SymbolManager symbols;

  celestia::semantic::ModuleManager modules;

  TypeManager types;

  BuiltinTypes builtins;

  UnitManager units;

  ScopeManager scopes;

  BuiltinIntrinsics intrinsics;

  celestia::semantic::ModuleId builtin_module;

  CompilerEnvironment() {
    
    builtin_module = modules.register_module("builtin");

    auto &module = modules.get(builtin_module);

    auto scope = scopes.create_scope(core::ScopeKind::Module, celestia::semantic::ScopeId::invalid());

    module.set_scope(scope);
  }
};