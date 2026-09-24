
#include "celestia/ast/declarations/CapabilityDeclaration.hpp"
#include "celestia/ast/declarations/EnumDeclaration.hpp"
#include "celestia/ast/declarations/FunctionDeclaration.hpp"
#include "celestia/ast/declarations/ModuleDeclaration.hpp"
#include "celestia/ast/declarations/StructDeclaration.hpp"
#include "celestia/ast/declarations/TypeDeclaration.hpp"
#include "celestia/ast/declarations/VariableDeclaration.hpp"
#include "celestia/ast/declarations/ImplementationDeclaration.hpp"
#include "celestia/debug/ast/AstDumper.hpp"

namespace celestia::debug {

void AstDumper::dump_impl_declaration(const ast::ImplDeclaration *node) {

  auto object = context.object(ast::node_kind_name(node->kind));

  object.field("Capability", node->capability);
  object.field("Target", node->target);
  object.list("Members", node->members);
}

void AstDumper::dump_capability_declaration(const ast::CapabilityDeclaration *node) {

  auto object = context.object(ast::node_kind_name(node->kind));

  object.field("Name", node->name);
  object.list("GenericParameters", node->generic_parameters);
  object.list("Members", node->members);
}

void AstDumper::dump_function_declaration(const ast::FunctionDeclaration *node) {

  auto object = context.object(ast::node_kind_name(node->kind));

  object.field("Name", node->name);
  object.list("GenericParameters", node->generic_parameters);
  object.list("Parameters", node->parameters);
  object.field("ReturnType", node->return_type);
  object.field("Body", node->body);
}

void AstDumper::dump_variable_declaration(const ast::VariableDeclaration *node) {

  auto object = context.object(ast::node_kind_name(node->kind));

  object.field("Pattern", node->pattern);
  object.field("Initializer", node->initializer);
}

void AstDumper::dump_module_declaration(const ast::ModuleDeclaration *node) {

  auto object = context.object(ast::node_kind_name(node->kind));

  object.field("Name", node->name);
  object.list("Declarations", node->declarations);
}

void AstDumper::dump_module_init(const ast::ModuleInitDeclaration *node) {

  auto object = context.object(ast::node_kind_name(node->kind));

  object.field("Body", node->body);
}

void AstDumper::dump_field_declaration(const ast::FieldDeclaration *node) {

  auto object = context.object(ast::node_kind_name(node->kind));

  object.field("Name", node->name);
  object.field("Type", node->type);
}

void AstDumper::dump_struct_declaration(const ast::StructDeclaration *node) {

  auto object = context.object(ast::node_kind_name(node->kind));

  object.field("Name", node->name);
  object.list("GenericParameters", node->generic_parameters);
  object.list("Compositions", node->compositions);
  object.list("Fields", node->fields);
}

void AstDumper::dump_type_declaration(const ast::TypeDeclaration *node) {

  auto object = context.object(ast::node_kind_name(node->kind));

  object.field("Name", node->name);
  object.list("GenericParameters", node->generic_parameters);
}


void AstDumper::dump_enum_variant(const ast::EnumVariant *node) {

  auto g = context.object("EnumVariant");

  g.field("Name", node->name);
  g.list("payload", node->payload);
}


void AstDumper::dump_enum_declaration(const ast::EnumDeclaration *node) {

  auto g = context.object("EnumDeclaration");

  g.field("Name", node->name);
  g.list("Generics", node->generic_parameters);
  g.list("Variants", node->variants);
}

} // namespace celestia::debug
