#include "celestia/compiler/ModuleLoader.hpp"
#include "celestia/semantic/resolver/Resolver.hpp"
namespace celestia::semantic {

// type
void Resolver::resolve_type_declaration(ast::TypeDeclaration *node) {}

// import
void Resolver::resolve_import_declaration(ast::ImportDeclaration *node) {

  if (!node || !node->name) return;

  if (!load_module(context.compiler, node, context.unit)) {

    context.unit.diagnostics.report({
        .severity = diagnostic::Severity::Error,
        .code = diagnostic::DiagnosticCode::UnknownModule,
        .arguments =
            {
                diagnostic::name(node->name->get_str()),
            },
        .labels =
            {
                diagnostic::location(node->name->slice),
            },
    });

    return;
  }
}

void Resolver::resolve_capability_declaration(ast::CapabilityDeclaration *node) {

  if (!node) return;

  auto scope = context.unit.semantic.scope(node);

  if (!scope.is_valid()) {
    std::cerr << "capability has no scope\n";
    return;
  }

  context.stack.push(scope);

  // for (auto *generic : node->generics) {
  //   if (generic) resolve_node(generic);
  // }

  for (auto *member : node->members) {
    if (member) resolve_node(member);
  }

  context.stack.pop();
}
void Resolver::resolve_impl_declaration(ast::ImplDeclaration *node) {

  assert(node && node->capability);

  auto scope = context.unit.semantic.scope(node);

  assert(scope.is_valid());

  context.stack.push(scope);

  resolve_node(node->capability);

  resolve_node(node->target);

  for (auto *member : node->members) { resolve_node(member); }

  // if (!node) return;

  // std::cout << "Resolvendo[impl]\n";

  // auto *impl_scope = context.scope_manager.create_scope(core::ScopeKind::Impl, context.scopes.current());

  // if (!impl_scope) return;

  // context.scopes.push(*impl_scope);

  // declare_generics(node->generic_parameters);

  // resolve_node(node->target);
  // resolve_node(node->capability);

  // for (auto *member : node->members) {

  //   if (member) resolve_node(member);
  // }

  // context.scopes.pop();
}

void Resolver::resolve_module_declaration(ast::ModuleDeclaration *node) {

  assert(node && node->name);

  auto scope = context.unit.semantic.scope(node);

  if (!scope.is_valid()) {
    std::cerr << "module has no scope\n";
    return;
  }

  context.stack.push(scope);

  for (auto *declaration : node->declarations) {
    if (declaration) resolve_node(declaration);
  }

  context.stack.pop();
}
} // namespace celestia::semantic