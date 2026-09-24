#pragma once

#include "ayla/bootstrap/BuiltinNames.hpp"
#include "celestia/compiler/CompilerEnvironment.hpp"
#include <stdexcept>
#include <string>
#include <string_view>

namespace ayla::language {

class LanguageBootstrap {
public:
  static void bootstrap_builtin(CompilerEnvironment &env) {

    celestia::semantic::Module &builtin = env.modules.get(env.builtin_module);

    env.builtins.int_type = bind_primitive(env, builtin, types::Int, celestia::semantic::PrimitiveKind::Int);

    env.builtins.uint_type = bind_primitive(env, builtin, types::UInt, celestia::semantic::PrimitiveKind::UInt);

    env.builtins.int8_type = bind_primitive(env, builtin, types::Int8, celestia::semantic::PrimitiveKind::Int8);

    env.builtins.int16_type = bind_primitive(env, builtin, types::Int16, celestia::semantic::PrimitiveKind::Int16);

    env.builtins.int32_type = bind_primitive(env, builtin, types::Int32, celestia::semantic::PrimitiveKind::Int32);

    env.builtins.int64_type = bind_primitive(env, builtin, types::Int64, celestia::semantic::PrimitiveKind::Int64);

    env.builtins.uint8_type = bind_primitive(env, builtin, types::UInt8, celestia::semantic::PrimitiveKind::UInt8);

    env.builtins.uint16_type = bind_primitive(env, builtin, types::UInt16, celestia::semantic::PrimitiveKind::UInt16);

    env.builtins.uint32_type = bind_primitive(env, builtin, types::UInt32, celestia::semantic::PrimitiveKind::UInt32);

    env.builtins.uint64_type = bind_primitive(env, builtin, types::UInt64, celestia::semantic::PrimitiveKind::UInt64);

    env.builtins.f32_type = bind_primitive(env, builtin, types::F32, celestia::semantic::PrimitiveKind::F32);

    env.builtins.f64_type = bind_primitive(env, builtin, types::F64, celestia::semantic::PrimitiveKind::F64);

    env.builtins.bool_type = bind_primitive(env, builtin, types::Bool, celestia::semantic::PrimitiveKind::Bool);

    env.builtins.char_type = bind_primitive(env, builtin, types::Char, celestia::semantic::PrimitiveKind::Char);

    env.builtins.string_type = bind_primitive(env, builtin, types::String, celestia::semantic::PrimitiveKind::String);

    env.builtins.void_type = bind_primitive(env, builtin, types::Void, celestia::semantic::PrimitiveKind::Void);

    env.constructors.array_constructor = bind_generic(env, builtin, constructors::Array, 1);

    env.constructors.map_constructor = bind_generic(env, builtin, constructors::Map, 2);

    env.constructors.set_constructor = bind_generic(env, builtin, constructors::Set, 1);

    env.constructors.ref_constructor = bind_generic(env, builtin, constructors::Ref, 1);
  }

private:
  static celestia::semantic::TypeId bind_primitive(CompilerEnvironment &env, celestia::semantic::Module &module, std::string_view name, celestia::semantic::PrimitiveKind kind) {

    auto scope_id = module.scope_id();

    if (!scope_id.is_valid()) { throw std::runtime_error("LanguageBootstrap: builtin module has no scope"); }

    auto symbol_id = env.symbols.create_symbol(std::string(name), SymbolKind::Type, Visibility::Public, false, nullptr);

    if (!symbol_id.is_valid()) { throw std::runtime_error("LanguageBootstrap: failed to create builtin type '" + std::string(name) + "'"); }

    auto &scope = env.scopes.get(scope_id);
    scope.symbols.insert(std::string(name), symbol_id);

    auto type_id = env.types.get_or_create_primitive(symbol_id, kind);

    auto &symbol = env.symbols.get(symbol_id);
    symbol.type = type_id;

    return type_id;
  }

  static celestia::semantic::TypeId bind_generic(CompilerEnvironment &env, celestia::semantic::Module &module, std::string_view name, size_t arity) {

    auto scope_id = module.scope_id();

    if (!scope_id.is_valid()) { throw std::runtime_error("LanguageBootstrap: builtin module has no scope"); }

    auto symbol_id = env.symbols.create_symbol(std::string(name), SymbolKind::Type, Visibility::Public, false, nullptr);

    if (!symbol_id.is_valid()) { throw std::runtime_error("LanguageBootstrap: failed to create builtin type '" + std::string(name) + "'"); }

    auto &scope = env.scopes.get(scope_id);

    scope.symbols.insert(std::string(name), symbol_id);

    auto type_id = env.types.get_or_create_generic(symbol_id, arity);

    auto &symbol = env.symbols.get(symbol_id);

    symbol.type = type_id;

    return type_id;
  }
};
} // namespace ayla::language