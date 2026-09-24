#pragma once

#include "celestia/ast/NodeKind.hpp"

// Declarations
#include "celestia/ast/RootNode.hpp"
#include "celestia/ast/declarations/CapabilityDeclaration.hpp"
#include "celestia/ast/declarations/EnumDeclaration.hpp"
#include "celestia/ast/declarations/FunctionDeclaration.hpp"
#include "celestia/ast/declarations/ImplementationDeclaration.hpp"
#include "celestia/ast/declarations/ImportDeclaration.hpp"
#include "celestia/ast/declarations/ModuleDeclaration.hpp"
#include "celestia/ast/declarations/StructDeclaration.hpp"
#include "celestia/ast/declarations/TypeDeclaration.hpp"
#include "celestia/ast/declarations/VariableDeclaration.hpp"
// Expressions
#include "celestia/ast/expressions/AssignmentExpression.hpp"
#include "celestia/ast/expressions/BinaryExpressionNode.hpp"
#include "celestia/ast/expressions/CallExpressionNode.hpp"
#include "celestia/ast/expressions/IdentifierExpressionNode.hpp"
#include "celestia/ast/expressions/IndexAcessExpressionNode.hpp"
#include "celestia/ast/expressions/LiteralExpressionNode.hpp"
#include "celestia/ast/expressions/MemberAccessExpressionNode.hpp"
#include "celestia/ast/expressions/UnaryExpressionNode.hpp"
// Statements
#include "celestia/ast/statements/BlockStatementNode.hpp"
#include "celestia/ast/statements/ExpressionStatementNode.hpp"
#include "celestia/ast/statements/IfStatementNode.hpp"
#include "celestia/ast/statements/ReturnStatementNode.hpp"
#include "celestia/ast/statements/WhileStatementNode.hpp"
// Names
#include "celestia/ast/names/Identifier.hpp"
#include "celestia/ast/names/Qualified.hpp"

// Types
#include "celestia/ast/types/FunctionType.hpp"
#include "celestia/ast/types/Type.hpp"

// Patterns
#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/ast/patterns/PatternNode.hpp"
#include "celestia/ast/types/GenericType.hpp"
#include "celestia/ast/types/NamedType.hpp"

namespace celestia::ast {

template <typename T> struct NodeTraits;

template <> struct NodeTraits<EnumVariant> {
  static constexpr NodeKind kind = NodeKind::EnumVariant;
};

template <> struct NodeTraits<EnumDeclaration> {
  static constexpr NodeKind kind = NodeKind::EnumDeclaration;
};

template <> struct NodeTraits<GenericName> {
  static constexpr NodeKind kind = NodeKind::GenericName;
};

template <> struct NodeTraits<GenericParameter> {
  static constexpr NodeKind kind = NodeKind::GenericParameter;
};


template <> struct NodeTraits<RootNode> {
  static constexpr NodeKind kind = NodeKind::Root;
};

template <> struct NodeTraits<TypeDeclaration> {
  static constexpr NodeKind kind = NodeKind::TypeDeclaration;
};

template <> struct NodeTraits<ModuleInitDeclaration> {
  static constexpr NodeKind kind = NodeKind::ModuleInitDeclaration;
};

template <> struct NodeTraits<GenericType> {
  static constexpr NodeKind kind = NodeKind::GenericType;
};
template <> struct NodeTraits<FunctionType> {
  static constexpr NodeKind kind = NodeKind::FunctionType;
};

template <> struct NodeTraits<CapabilityDeclaration> {
  static constexpr NodeKind kind = NodeKind::CapabilityDeclaration;
};
template <> struct NodeTraits<ImplDeclaration> {
  static constexpr NodeKind kind = NodeKind::ImplementationDeclaration;
};

template <> struct NodeTraits<NamedType> {
  static constexpr NodeKind kind = NodeKind::NamedType;
};

template <> struct NodeTraits<StructDeclaration> {
  static constexpr NodeKind kind = NodeKind::StructDeclaration;
};

template <> struct NodeTraits<FieldDeclaration> {
  static constexpr NodeKind kind = NodeKind::FieldDeclaration;
};

template <> struct NodeTraits<FunctionDeclaration> {
  static constexpr NodeKind kind = NodeKind::FunctionDeclaration;
};

template <> struct NodeTraits<VariableDeclaration> {
  static constexpr NodeKind kind = NodeKind::VariableDeclaration;
};

template <> struct NodeTraits<ModuleDeclaration> {
  static constexpr NodeKind kind = NodeKind::ModuleDeclaration;
};

template <> struct NodeTraits<BinaryExpressionNode> {
  static constexpr NodeKind kind = NodeKind::BinaryExpression;
};

template <> struct NodeTraits<UnaryExpressionNode> {
  static constexpr NodeKind kind = NodeKind::UnaryExpression;
};

template <> struct NodeTraits<CallExpressionNode> {
  static constexpr NodeKind kind = NodeKind::Call;
};

template <> struct NodeTraits<AssignmentExpressionNode> {
  static constexpr NodeKind kind = NodeKind::Assignment;
};

template <> struct NodeTraits<MemberAccessExpressionNode> {
  static constexpr NodeKind kind = NodeKind::MemberAccess;
};

template <> struct NodeTraits<IndexAccessExpressionNode> {
  static constexpr NodeKind kind = NodeKind::IndexAccess;
};

template <> struct NodeTraits<IdentifierExpressionNode> {
  static constexpr NodeKind kind = NodeKind::IdentifierExpression;
};

template <> struct NodeTraits<NumberLiteralNode> {
  static constexpr NodeKind kind = NodeKind::NumberLiteral;
};

template <> struct NodeTraits<StringLiteralNode> {
  static constexpr NodeKind kind = NodeKind::StringLiteral;
};

template <> struct NodeTraits<BoolLiteralNode> {
  static constexpr NodeKind kind = NodeKind::BooleanLiteral;
};

template <> struct NodeTraits<NullLiteralNode> {
  static constexpr NodeKind kind = NodeKind::NullLiteral;
};

template <> struct NodeTraits<StructFieldInitializerNode> {
  static constexpr NodeKind kind = NodeKind::StructFieldInitializer;
};

template <> struct NodeTraits<StructLiteralNode> {
  static constexpr NodeKind kind = NodeKind::StructLiteral;
};

template <> struct NodeTraits<ArrayLiteralNode> {
  static constexpr NodeKind kind = NodeKind::ArrayLiteral;
};

template <> struct NodeTraits<ObjectLiteralNode> {
  static constexpr NodeKind kind = NodeKind::ObjectLiteral;
};

template <> struct NodeTraits<ObjectFieldNode> {
  static constexpr NodeKind kind = NodeKind::ObjectField;
};

template <> struct NodeTraits<BlockStatement> {
  static constexpr NodeKind kind = NodeKind::BlockStatement;
};

template <> struct NodeTraits<IfStatement> {
  static constexpr NodeKind kind = NodeKind::IfStatement;
};

template <> struct NodeTraits<WhileStatement> {
  static constexpr NodeKind kind = NodeKind::WhileStatement;
};

template <> struct NodeTraits<ReturnStatement> {
  static constexpr NodeKind kind = NodeKind::ReturnStatement;
};

template <> struct NodeTraits<ExpressionStatement> {
  static constexpr NodeKind kind = NodeKind::ExpressionStatement;
};

template <> struct NodeTraits<ImportDeclaration> {
  static constexpr NodeKind kind = NodeKind::ImportDeclaration;
};

template <> struct NodeTraits<Identifier> {
  static constexpr NodeKind kind = NodeKind::Identifier;
};

template <> struct NodeTraits<QualifiedName> {
  static constexpr NodeKind kind = NodeKind::QualifiedName;
};

template <> struct NodeTraits<Type> {
  static constexpr NodeKind kind = NodeKind::Type;
};

template <> struct NodeTraits<PatternNode> {
  static constexpr NodeKind kind = NodeKind::Pattern;
};

template <> struct NodeTraits<NamedPattern> {
  static constexpr NodeKind kind = NodeKind::NamedPattern;
};

} // namespace celestia::ast
