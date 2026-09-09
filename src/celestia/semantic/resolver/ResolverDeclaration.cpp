#include "celestia/semantic/resolver/Resolver.hpp"

namespace celestia::semantic {

// var
void Resolver::resolve_variable_declaration(ast::VariableDeclaration *node) {

  if (!node) return;

  if (node->pattern) resolve_node(node->pattern);

  if (node->initializer) resolve_node(node->initializer);
}

// type
void Resolver::resolve_type_declaration(ast::TypeDeclaration *node) {}

// struct
void Resolver::resolve_struct_declaration(ast::StructDeclaration *node) {

  if (!node) return;

  auto scope = context.unit.semantic.scope(node);

  if (!scope.is_valid()) { throw std::runtime_error("struct has no scope"); }

  context.stack.push(scope);

  for (auto *composition : node->compositions) {
    if (composition) resolve_node(composition);
  }

  for (auto *field : node->fields) {
    if (field) resolve_node(field);
  }

  context.stack.pop();
}

// import
void Resolver::resolve_import_declaration(ast::ImportDeclaration *node) {

  if (!node || !node->name) return;

  const auto name = node->name->get_str();

  auto imported = context.get_env().modules.find(name);

  if (!imported.is_valid()) {

    context.unit.diagnostics.report({
        .severity = diagnostic::Severity::Error,
        .code = diagnostic::DiagnosticCode::UnknownModule,
        .arguments =
            {
                diagnostic::name(name),
            },
        .labels =
            {
                diagnostic::location(node->name->slice),
            },
    });

    return;
  }

  // auto current = context.unit.module;

  // assert(current.is_valid() && "Resolver has no current module");

  // if (!current.is_valid()) return;

  // auto &current_module = context.get_env().modules.get(current);

  // current_module.add_import(imported);
}

// function
void Resolver::resolve_function_declaration(ast::FunctionDeclaration *node) {

  if (!node || !node->name) return;

  // params
  for (auto *parameter : node->parameters) {
    if (parameter) { resolve_node(parameter); }
  }

  // return type
  if (node->return_type) { resolve_node(node->return_type); }

  // body
  if (node->body) { resolve_node(node->body); }
}

void Resolver::resolve_field_declaration(ast::FieldDeclaration *node) {

  if (!node) return;

  if (node->type) { resolve_node(node->type); }
}

void Resolver::resolve_capability_declaration(ast::CapabilityDeclaration *node) {

  // assert(node && "Resolver::resolve_capability_declaration received null");
  // assert(node->name && "CapabilityDeclaration has no name");

  // const std::string &name = node->name->str;

  // debug::trace(debug::Category::Resolver, "resolving capability '{}'", name);

  // SymbolId symbol_id = declare_symbol(name, SymbolKind::Capability, node->specifiers.visibility, node);

  // if (!symbol_id.is_valid()) return;

  // auto *parent_scope = context.scopes.current();

  // assert(parent_scope && "Resolver has no active scope while resolving capability");

  // auto *capability_scope = context.scope_manager.create_scope(core::ScopeKind::Capability, parent_scope);

  // assert(capability_scope && "failed to create capability scope");

  // if (!capability_scope) return;

  // context.scopes.push(*capability_scope);

  // debug::trace(debug::Category::Resolver, "entered capability scope '{}' -> SymbolId({})", name, symbol_id.index());

  // // Comparable<T>
  // declare_generics(node->generic_parameters);

  // // Members
  // for (auto *member : node->members) {

  //   assert(member && "CapabilityDeclaration contains null member");

  //   resolve_node(member);
  // }

  // context.scopes.pop();

  // debug::trace(debug::Category::Resolver, "resolved capability '{}'", name);
}

void Resolver::resolve_impl_declaration(ast::ImplDeclaration *node) {

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
} // namespace celestia::semantic