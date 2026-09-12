
#include "celestia/compiler/CompilerEnvironment.hpp"
#include "celestia/syntax/lexer/Lexer.hpp"
#include "celestia/syntax/lexer/LexerContext.hpp"
#include "celestia/syntax/parser/Parser.hpp"
#include "celestia/syntax/parser/ParserContext.hpp"
#include <filesystem>
#include <vector>

class ModuleIndexer {

public:
  static std::vector<std::filesystem::path> discover(std::filesystem::path root) {

    std::vector<std::filesystem::path> files;

    for (const auto &entry : std::filesystem::recursive_directory_iterator(root)) {

      if (!entry.is_regular_file()) continue;

      if (entry.path().extension() != ".ayla") continue;

      files.push_back(entry.path());
    }

    return files;
  }

  static void link(CompilerEnvironment &env, std::filesystem::path file) {

    core::source::Source source(file);

    CompilationUnit unit(source);

    LexerContext lex_ctx(env.language, source, unit.tokens);
    Lexer lex(lex_ctx);
    lex.tokenize();

    celestia::syntax::ParseContext ctx(unit, env);
    celestia::syntax::Parser parser(ctx);

    while (!ctx.tokens().is_end()) {

      if (!ctx.tokens().check(TokenKind::MODULE_KEYWORD)) {
        ctx.tokens().consume();
        continue;
      }

      auto module = parser.parse_module_declaration();

      if (!module.is_ok()) {
        ctx.tokens().consume();
        continue;
      }

      auto name = module.value()->name->get_str();

      auto module_id = env.modules.find(name);

      if (!module_id.is_valid()) { module_id = env.modules.register_module(name); }

      if (module_id.is_valid()) { env.index.add(module_id, file); }
    }
  }

  static void run(CompilerEnvironment &env) {

    auto files = discover(env.root);

    for (auto &file : files) { link(env, file); }
  }
};
