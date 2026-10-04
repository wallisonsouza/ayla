#pragma once

#include "celestia/ast/NodeKind.hpp"

// Declarations
#include "celestia/ast/RootNode.hpp"

#include "celestia/ast/declaration/CapabilityDeclaration.hpp"
#include "celestia/ast/declaration/EnumDeclaration.hpp"
#include "celestia/ast/declaration/FunctionDeclaration.hpp"
#include "celestia/ast/declaration/ImplementationDeclaration.hpp"
#include "celestia/ast/declaration/ImportDeclaration.hpp"
#include "celestia/ast/declaration/ModuleDeclaration.hpp"
#include "celestia/ast/declaration/StructDeclaration.hpp"
#include "celestia/ast/declaration/TypeDeclaration.hpp"
#include "celestia/ast/declaration/VariableDeclaration.hpp"

// Expressions
#include "celestia/ast/expression/ArrayLiteral.hpp"
#include "celestia/ast/expression/AssignmentExpression.hpp"
#include "celestia/ast/expression/BinaryExpression.hpp"
#include "celestia/ast/expression/CallExpression.hpp"
#include "celestia/ast/expression/IdentifierExpression.hpp"
#include "celestia/ast/expression/IfExpression.hpp"
#include "celestia/ast/expression/IndexAcessExpression.hpp"
#include "celestia/ast/expression/MatchExpression.hpp"
#include "celestia/ast/expression/MemberAccessExpression.hpp"
#include "celestia/ast/expression/NullLiteral.hpp"
#include "celestia/ast/expression/NumberLiteral.hpp"
#include "celestia/ast/expression/ObjectLiteral.hpp"
#include "celestia/ast/expression/BoolLiteral.hpp"
#include "celestia/ast/expression/StringLiteral.hpp"
#include "celestia/ast/expression/StructLiteral.hpp"
#include "celestia/ast/expression/UnaryExpression.hpp"
#include "celestia/ast/expression/WhileExpression.hpp"

// Statements
#include "celestia/ast/statements/ExpressionStatementNode.hpp"
#include "celestia/ast/statements/ReturnStatementNode.hpp"

// Names
#include "celestia/ast/names/Identifier.hpp"
#include "celestia/ast/names/Qualified.hpp"

// Types
#include "celestia/ast/types/FunctionType.hpp"
#include "celestia/ast/types/GenericType.hpp"
#include "celestia/ast/types/NamedType.hpp"
#include "celestia/ast/types/Type.hpp"

// Patterns
#include "celestia/ast/patterns/NamedPatternNode.hpp"
#include "celestia/ast/patterns/PatternNode.hpp"

namespace celestia::ast {

template <typename T> struct NodeTraits;

template <> struct NodeTraits<WhileExpression> {
  static constexpr NodeKind kind = NodeKind::WhileExpression;
};

template <> struct NodeTraits<MatchArm> {
  static constexpr NodeKind kind = NodeKind::MatchArm;
};

template <> struct NodeTraits<MatchExpression> {
  static constexpr NodeKind kind = NodeKind::MatchExpression;
};

template <> struct NodeTraits<EnumVariantPattern> {
  static constexpr NodeKind kind = NodeKind::EnumVariantPattern;
};

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

template <> struct NodeTraits<NumberLiteral> {
  static constexpr NodeKind kind = NodeKind::NumberLiteral;
};

template <> struct NodeTraits<StringLiteral> {
  static constexpr NodeKind kind = NodeKind::StringLiteral;
};

template <> struct NodeTraits<BoolLiteral> {
  static constexpr NodeKind kind = NodeKind::BooleanLiteral;
};

template <> struct NodeTraits<NullLiteral> {
  static constexpr NodeKind kind = NodeKind::NullLiteral;
};

template <> struct NodeTraits<StructField> {
  static constexpr NodeKind kind = NodeKind::StructFieldInitializer;
};

template <> struct NodeTraits<StructLiteral> {
  static constexpr NodeKind kind = NodeKind::StructLiteral;
};

template <> struct NodeTraits<ArrayLiteral> {
  static constexpr NodeKind kind = NodeKind::ArrayLiteral;
};

template <> struct NodeTraits<ObjectLiteral> {
  static constexpr NodeKind kind = NodeKind::ObjectLiteral;
};

template <> struct NodeTraits<ObjectField> {
  static constexpr NodeKind kind = NodeKind::ObjectField;
};

template <> struct NodeTraits<BlockExpression> {
  static constexpr NodeKind kind = NodeKind::BlockExpression;
};

template <> struct NodeTraits<IfExpression> {
  static constexpr NodeKind kind = NodeKind::IfExpression;
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
