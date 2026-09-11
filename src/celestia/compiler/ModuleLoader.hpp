#include "celestia/ast/declarations/ImportDeclaration.hpp"
#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/Compiler.hpp"

#include <iostream>

inline CompilationUnit *load_module(Compiler &compiler, const celestia::ast::ImportDeclaration *node, CompilationUnit &importer) {

  if (!node || !node->name) {
    std::cout << "[ModuleLoader] invalid import\n";
    return nullptr;
  }

  const auto module_name = node->name->get_str();

  std::cout << "[ModuleLoader] loading module: " << module_name << '\n';

  std::filesystem::path path;

  if (node->path) {

    std::cout << "[ModuleLoader] explicit path: " << node->path.value() << '\n';

    path = node->path.value();

    if (path.is_relative()) {
      path = importer.source.path.parent_path() / path;

      std::cout << "[ModuleLoader] resolved relative path: " << path << '\n';
    }

  } else {

    std::string name = module_name;

    for (char &c : name) {
      if (c == '.') c = '/';
    }

    path = compiler.environment().root / (name + ".ayla");

    std::cout << "[ModuleLoader] resolved module path: " << path << '\n';
  }

  path = std::filesystem::absolute(path).lexically_normal();

  std::cout << "[ModuleLoader] normalized path: " << path << '\n';

  if (!std::filesystem::exists(path)) {

    std::cout << "[ModuleLoader] file not found: " << path << '\n';

    return nullptr;
  }

  std::cout << "[ModuleLoader] file found: " << path << '\n';

  auto *unit = compiler.add_script(path.string());

  if (!unit) {
    std::cout << "[ModuleLoader] failed to create compilation unit\n";
    return nullptr;
  }

  compiler.require(*unit, stages::Parser, CompilationRules::normal());

  return unit;
}