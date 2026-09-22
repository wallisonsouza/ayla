#include "Parser.hpp"

#include "ParserContext.hpp"

#include "celestia/ast/declarations/ModuleDeclaration.hpp"
#include "celestia/ast/names/QualifiedNameNode.hpp"

#include <vector>

namespace celestia::syntax {

Parser::Parser(ParseContext &context) : context(context) {}

Parser::~Parser() = default;

void Parser::run() {

  auto *root = context.get_ast().alloc<ast::RootNode>();

  auto &tokens = context.tokens();

  // Primeiro conteúdo do arquivo
  while (!tokens.is_end() && tokens.kind() == TokenKind::NEW_LINE) { tokens.consume(); }

  // Primeiro módulo
  if (tokens.kind() == TokenKind::MODULE_KEYWORD) {

    // O arquivo começa com um módulo explícito.
    auto *module = parse_explicit_module();

    if (!module) return;

    root->modules.push_back(module);

  } else {

    // O arquivo começa com declarações/expressões.
    // Portanto, elas pertencem ao módulo implícito.
    auto *module = create_implicit_module();

    if (!module) return;

    root->modules.push_back(module);

    parse_module_body(module);
  }

  // Módulos explícitos seguintes
  while (!tokens.is_end()) {

    // Ignora linhas vazias entre módulos.
    while (!tokens.is_end() && tokens.kind() == TokenKind::NEW_LINE) { tokens.consume(); }

    if (tokens.is_end()) break;

    if (tokens.kind() != TokenKind::MODULE_KEYWORD) {
      tokens.consume();
      continue;
    }

    auto *module = parse_explicit_module();

    if (!module) return;

    root->modules.push_back(module);
  }

  context.unit.set_root(root);
}

// Implicit module
ast::ModuleDeclaration *Parser::create_implicit_module() {

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

  auto *name = context.get_ast().alloc<ast::QualifiedNameNode>(std::move(parts));

  return context.get_ast().alloc<ast::ModuleDeclaration>(name);
}

// Explicit module
ast::ModuleDeclaration *Parser::parse_explicit_module() {

  auto result = parse_module_declaration();

  if (result.is_error()) return nullptr;

  if (result.is_no_match()) return nullptr;

  auto *module = result.value();

  if (!module) return nullptr;

  parse_module_body(module);

  return module;
}

// Module body
void Parser::parse_module_body(ast::ModuleDeclaration *module) {

  assert(module);

  auto &tokens = context.tokens();

  ast::BlockStatement *script = nullptr;

  while (!tokens.is_end()) {

    // Um novo module inicia outro módulo.
    if (tokens.kind() == TokenKind::MODULE_KEYWORD) { break; }

    auto declaration = parse_declaration();

    if (declaration.is_error()) return;

    if (declaration.is_ok()) {

      module->declarations.push_back(declaration.value());

      continue;
    }

    if (auto *statement = parse_statement()) {

      if (!script) { script = context.get_ast().alloc<ast::BlockStatement>(); }

      script->items.push_back(statement);

      continue;
    }

    tokens.consume();
  }

  if (script) {

    auto *init = context.get_ast().alloc<ast::ModuleInitDeclaration>(script);

    module->declarations.push_back(init);
  }
}

} // namespace celestia::syntax