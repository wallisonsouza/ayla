#include "ayla/AylaMain.hpp"
#include "ayla/bootstrap/LanguageBootstrap.hpp"
#include "ayla/language/AylaLanguage.hpp"
#include "celestia/compiler/Compiler.hpp"
#include "celestia/file/FileDiscovery.hpp"

void AylaApplication::discover_modules(Compiler &compiler) {

  celestia::project::FileDiscovery discovery({compiler.environment().root});

  auto files = discovery.discover();

  auto rules = CompilationRules::discovery();

  for (const auto &file : files) {

    auto *unit = compiler.add_script(file);

    compiler.require(*unit, stages::ModuleParser, rules);

    celestia::debug::AstDumper dump;
    dump.dump(unit->_root);
  }
}

int AylaApplication::run(const CommandLine &cmd) {

  if (cmd.command != Command::Run) return 0;

  auto lang = ayla::language::create_definition();

  Compiler compiler(lang);

  compiler.environment().root = "../src/ayla/scripts/";

  discover_modules(compiler);

  ayla::language::LanguageBootstrap::bootstrap_builtin(compiler.environment());

  auto *user = compiler.find_script(*cmd.input);

  if (!user) {
    std::cerr << "script not found: " << *cmd.input << '\n';
    return 1;
  }

  compiler.require(*user, stages::Parser, CompilationRules::normal());

  compiler.show_diagnostics();

  return 0;
}