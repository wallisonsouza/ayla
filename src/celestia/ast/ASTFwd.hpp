#pragma once

namespace celestia::ast {


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
class AssignmentExpressionNode;
class BinaryExpressionNode;
class CallExpressionNode;
class IdentifierExpressionNode;
class IndexAccessExpressionNode;
class LiteralExpressionNode;
class MemberAccessExpressionNode;
class UnaryExpressionNode;


class Statement;
// class BlockStatement;
class ExpressionStatement;
class IfExpression;
class ReturnStatement;
class WhileExpression;


class Type;
class FunctionType;
class GenericType;
class NamedType;


class PatternNode;
class NamedPattern;

class QualifiedName;

class EnumVariant;

class GenericParameter;
class Identifier;


class NumberLiteralNode;
class StringLiteralNode;
class BoolLiteralNode;
class ObjectLiteral;
class ArrayLiteralNode;
class StructLiteral;

}