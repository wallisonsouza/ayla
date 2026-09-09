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
  }
}

int AylaApplication::run(const CommandLine &cmd) {

  if (cmd.command != Command::Run) return 0;

  auto lang = ayla::language::create_definition();

  Compiler compiler(lang);

  compiler.environment().root = "../src/ayla/scripts/";

  discover_modules(compiler);

  ayla::language::LanguageBootstrap::bootstrap_builtin(compiler.environment());

  auto *user = compiler.add_script(*cmd.input);

  compiler.require(*user, stages::Parser, CompilationRules::normal());

  compiler.show_diagnostics();

  celestia::debug::AstDumper dump;
  dump.dump(user->_root);

  return 0;
}