#include "celestia/compiler/Compiler.hpp"
#include "celestia/core/visitor/Stage.hpp"

#include <iostream>

class ModuleDiscoveryStage : public Stage {
public:
  void run(Compiler &compiler, CompilationUnit &unit) override {

    if (!unit._root) return;

    for (auto *module_node : unit._root->modules) {

      if (!module_node || !module_node->name) continue;

      const auto module_name = module_node->name->get_str();

      std::cout << "[ModuleDiscovery] module = " << module_name << '\n';

      auto module_id = compiler.environment().modules.register_module(module_name);

      if (!module_id.is_valid()) continue;

      unit.modules.push_back(module_id);

      compiler.environment().modules.add_provider(module_id, unit.id);

      unit.semantic.set_module(module_node, module_id);

      if (module_id != compiler.environment().builtin_module) {

        auto &module = compiler.environment().modules.get(module_id);

        module.add_import(compiler.environment().builtin_module);
      }
    }
  }
};