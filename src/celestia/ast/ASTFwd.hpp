#pragma once

namespace celestia::ast {

class RootNode;

class Node;

class Declaration;

class EnumDeclaration;
class CapabilityDeclaration;
class FunctionDeclaration;
class ImportDeclaration;
class ModuleDeclaration;
class ModuleInitDeclaration;
class StructDeclaration;
class VariableDeclaration;
class TypeDeclaration;
class ImplDeclaration;
class FieldDeclaration;

class Expression;

class IfExpression;
class WhileExpression;
class AssignmentExpressionNode;
class BinaryExpression;
class CallExpressionNode;
class IdentifierExpressionNode;
class IndexAccessExpressionNode;
class LiteralExpressionNode;
class MemberAccessExpressionNode;
class UnaryExpressionNode;

class Statement;
class ExpressionStatement;
class ReturnStatement;

class Type;
class FunctionType;
class GenericType;
class NamedType;

class PatternNode;
class NamedPattern;

class QualifiedName;
class GenericName;
class EnumVariant;
class EnumVariantPattern;

class GenericParameter;
class Identifier;

class NumberLiteral;
class StringLiteral;
class BoolLiteral;
class ObjectLiteral;
class ArrayLiteral;
class StructLiteral;

} // namespace celestia::ast