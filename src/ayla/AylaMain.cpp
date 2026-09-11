#include "ayla/AylaMain.hpp"
#include "ayla/bootstrap/LanguageBootstrap.hpp"
#include "ayla/language/AylaLanguage.hpp"
#include "celestia/compiler/Compiler.hpp"
#include "celestia/file/FileDiscovery.hpp"

int AylaApplication::run(const CommandLine &cmd) {

  if (cmd.command != Command::Run) return 0;

  auto lang = ayla::language::create_definition();

  Compiler compiler(lang);

  ayla::language::LanguageBootstrap::bootstrap_builtin(compiler.environment());

  compiler.environment().root = "../src/ayla/scripts/";

  auto *user = compiler.add_script(*cmd.input);

  if (!user) {
    std::cerr << "script not found: " << *cmd.input << '\n';
    return 1;
  }

  compiler.require(*user, stages::Resolver, CompilationRules::normal());

  compiler.show_diagnostics();

  celestia::debug::AstDumper dump;
  dump.dump(user->_root);

  return 0;
}