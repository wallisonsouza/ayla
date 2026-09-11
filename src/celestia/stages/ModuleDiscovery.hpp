// #include "celestia/compiler/Compiler.hpp"
// #include "celestia/core/visitor/Stage.hpp"

// #include <iostream>

// class ModuleDiscoveryStage : public Stage {
// public:
//   void run(Compiler &compiler, CompilationUnit &unit) override {

//     if (!unit._root) return;

//     auto &environment = compiler.environment();

//     auto builtin_id = environment.builtin_module;

//     auto &builtin = environment.modules.get(builtin_id);

//     for (auto *module_node : unit._root->modules) {

//       if (!module_node || !module_node->name) continue;

//       const auto module_name = module_node->name->get_str();

//       std::cout << "[ModuleDiscovery] module = " << module_name << '\n';

//       auto module_id = environment.modules.register_module(module_name, builtin_id);

//       if (!module_id.is_valid()) continue;

//       unit.modules.push_back(module_id);

//       environment.modules.add_provider(module_id, unit.id);

//       unit.semantic.set_module(module_node, module_id);

//       auto &module = environment.modules.get(module_id);

//       // Cria o scope do módulo.
//       auto module_scope = environment.scopes.create_scope(core::ScopeKind::Module, builtin.scope_id());

//       if (!module_scope.is_valid()) {
//         std::cerr << "failed to create module scope\n";
//         continue;
//       }

//       module.set_scope(module_scope);

//       // O builtin fica implicitamente disponível.
//       if (module_id != builtin_id) { module.add_import(builtin_id); }
//     }
//   }
// };