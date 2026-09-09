#include "celestia/compiler/Compiler.hpp"
#include "celestia/core/visitor/Stage.hpp"
#include "celestia/syntax/parser/Parser.hpp"
#include "celestia/syntax/parser/ParserContext.hpp"

#include <iostream>

class ModuleDiscoveryStage : public Stage {
public:
  std::vector<std::string> discover_modules(celestia::syntax::ParseContext &context, celestia::syntax::Parser &parser) {


    

    std::vector<std::string> modules;

    auto &tokens = context.tokens();

    tokens.add_checkpoint();


    while (!tokens.is_end()) {

      if (!tokens.match(TokenKind::MODULE_KEYWORD)) {
        tokens.consume();
        continue;
      }

      auto name_result = parser.parse_name();

      if (name_result.is_error()) { break; }

      if (name_result.is_no_match()) { break; }

      modules.push_back(name_result.value()->get_str());
    }

    tokens.rollback_checkpoint();

    return modules;
  }

  void run(Compiler &compiler, CompilationUnit &unit) override {

    celestia::syntax::ParseContext context(unit, compiler.environment());
    celestia::syntax::Parser parser(context);

    auto modules = discover_modules(context, parser);

    if (modules.empty()) {

      auto relative = std::filesystem::relative(unit.source.path, compiler.environment().root);

      std::string module_name;

      for (const auto &part : relative) {

        if (!module_name.empty()) module_name += ".";

        if (part.extension() == ".ayla")
          module_name += part.stem().string();
        else
          module_name += part.string();
      }

      modules.push_back(std::move(module_name));
    }

    for (const auto &module_name : modules) {

      std::cout << "[ModuleDiscovery] module = " << module_name << '\n';

      auto module_id = compiler.environment().modules.register_module(module_name);

      if (!module_id.is_valid()) continue;

      // Unidade fornece esse módulo.
      unit.modules.push_back(module_id);

      // ModuleManager registra a relação inversa.
      compiler.environment().modules.add_provider(module_id, unit.id);

      if (module_id != compiler.environment().builtin_module) {

        auto &module = compiler.environment().modules.get(module_id);

        module.add_import(compiler.environment().builtin_module);
      }
    }
  }
};