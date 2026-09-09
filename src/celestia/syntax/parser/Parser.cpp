#include "Parser.hpp"

#include "ParserContext.hpp"

#include "celestia/ast/declarations/ModuleDeclaration.hpp"
#include "celestia/ast/names/QualifiedNameNode.hpp"
#include "celestia/syntax/parser/expressions/Expression.hpp"
#include <vector>

namespace celestia::syntax {
Parser::Parser(ParseContext &context) : context(context) { expression_parser = std::make_unique<ExpressionParser>(context, *this); }

Parser::~Parser() = default;

void Parser::run() {

  std::cout << "[Parser] << \n";

  auto *root = context.get_ast().alloc<ast::RootNode>();

  auto &tokens = context.tokens();

  std::cout << "[Parser] initial token = " << token_kind_name(tokens.kind()) << '\n';

  std::cout << "[Parser] initial is_end = " << tokens.is_end() << '\n';

  ast::ModuleDeclaration *implicit_module = nullptr;
  ast::BlockStatement *implicit_script = nullptr;

  while (!tokens.is_end()) {

    std::cout << "[Parser] token = " << token_kind_name(tokens.kind()) << '\n';

    // --------------------------------------------------
    // Explicit module
    // --------------------------------------------------

    auto before = tokens.position(); // se TokenStream tiver isso

    auto module_result = parse_module_declaration();

    auto after = tokens.position();

    if (module_result.is_ok() && before == after) {
      std::cerr << "[Parser] BUG: module parser returned OK without consuming tokens\n";
      return;
    }

    if (module_result.is_error()) {
      std::cout << "[Parser] module ERROR\n";
      return;
    }

    if (module_result.is_ok()) {

      auto *module = module_result.value();

      if (module) { root->modules.push_back(module); }

      continue;
    }

    // --------------------------------------------------
    // Create implicit module
    // --------------------------------------------------

    if (!implicit_module) {

      auto relative = std::filesystem::relative(context.unit.source.path, context.env.root);

      std::vector<ast::IdentifierNode *> parts;

      for (const auto &part : relative) {

        std::string name;

        if (part.extension() == ".ayla")
          name = part.stem().string();
        else
          name = part.string();

        if (name.empty() || name == ".") continue;

        parts.push_back(context.get_ast().alloc<ast::IdentifierNode>(name));
      }

      auto *module_name = context.get_ast().alloc<ast::QualifiedNameNode>(std::move(parts));

      implicit_module = context.get_ast().alloc<ast::ModuleDeclaration>(module_name);

      implicit_script = context.get_ast().alloc<ast::BlockStatement>();

      root->modules.push_back(implicit_module);
    }

    // --------------------------------------------------
    // Declaration
    // --------------------------------------------------

    auto declaration = parse_declaration();

    if (declaration.is_error()) {
      std::cout << "[Parser] declaration ERROR\n";
      return;
    }

    if (declaration.is_ok()) {

      std::cout << "[Parser] declaration MATCH\n";

      implicit_module->declarations.push_back(declaration.value());

      continue;
    }

    // --------------------------------------------------
    // Statement
    // --------------------------------------------------

    if (auto *stmt = parse_statement()) {

      implicit_script->statements.push_back(stmt);

      continue;
    }

    tokens.consume();
  }

  // --------------------------------------------------
  // Finish implicit module
  // --------------------------------------------------

  if (implicit_module) {

    auto *init = context.get_ast().alloc<ast::ModuleInitDeclaration>(implicit_script);

    implicit_module->declarations.push_back(init);
  }

  context.unit.set_root(root);
}
} // namespace celestia::syntax
