#pragma once

#include "celestia/ast/RootNode.hpp"
#include "celestia/compiler/CompileFlags.hpp"
#include "celestia/core/source/Source.hpp"
#include "celestia/core/token/token_stream.hpp"
#include "celestia/diagnostic/DiagnosticContext.hpp"
#include "celestia/ir/IRIds.hpp"
#include "celestia/semantic/SemanticInfo.hpp"

struct CompilationUnit {

  celestia::semantic::CompilationUnitId id;

  core::source::Source &source;

  core::memory::Arena arena;

  diagnostic::DiagnosticContext diagnostics;

  core::token::TokenStream tokens;

  celestia::ast::RootNode *_root = nullptr;

  celestia::ir::ModuleId ir_module = celestia::ir::ModuleId::invalid();

  CompileFlags flags = CompileFlags::None;

  std::vector<celestia::semantic::ModuleId> modules;

  celestia::semantic::SemanticInfo semantic;

  void set_root(celestia::ast::RootNode *root) { _root = root; }

  CompilationUnit(core::source::Source &source) : source(source) {}
};