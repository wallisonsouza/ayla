#include "ayla/AylaMain.hpp"
#include "ayla/bootstrap/LanguageBootstrap.hpp"
#include "ayla/language/AylaLanguage.hpp"
#include "celestia/compiler/Compiler.hpp"
#include "celestia/compiler/ModuleIndexer.hpp"
#include "celestia/diagnostic/diagnostic_debug.hpp"
#include <memory.h>

int AylaApplication::run(const CommandLine &cmd) {

  if (cmd.command != Command::Run) return 0;

  auto lang = ayla::language::create_definition();

  Compiler compiler(lang);

  ayla::language::LanguageBootstrap::bootstrap_builtin(compiler.environment());

  compiler.environment().root = "../src/ayla/scripts/";

  ModuleIndexer::run(compiler.environment());

  auto *user = compiler.add_script(*cmd.input);

  if (!user) {
    std::cerr << "script not found: " << *cmd.input << '\n';
    return 1;
  }

  compiler.require(*user, StageId::Resolver, CompilationRules::normal());

  for (auto &unit : compiler.environment().units.all()) {
    for (auto &diag : unit->diagnostics.all()) { diagnostic::print_diagnostic(diag, unit->source, compiler.environment()); }
  }



  // celestia::debug::AstDumper dump;
  // dump.dump(user->_root);

  return 0;
}