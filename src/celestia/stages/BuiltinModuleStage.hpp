// #include "celestia/compiler/CompilationUnit.hpp"
// #include "celestia/compiler/Compiler.hpp"
// #include "celestia/core/visitor/Stage.hpp"

// class BuiltinModuleStage : public Stage {

// public:
//   void run(Compiler &compiler, CompilationUnit &unit) override {

//     auto builtin_id = compiler.environment().builtin_module;

//     if (!builtin_id.is_valid()) { throw std::runtime_error("builtin module not initialized"); }

//     unit.modules.push_back(builtin_id);

//     compiler.environment().modules.add_provider(builtin_id, unit.id);
//   }
// };