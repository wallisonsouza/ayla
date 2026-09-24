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
class BlockStatement;
class ExpressionStatement;
class IfStatement;
class ReturnStatement;
class WhileStatement;


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


class NumberLiteral;
class StringLiteralNode;
class BoolLiteralNode;
class ObjectLiteralNode;
class ArrayLiteralNode;
class StructLiteralNode;

}