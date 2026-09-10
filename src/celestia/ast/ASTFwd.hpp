#pragma once

namespace celestia::ast {


class Node;


class FunctionType;
class Expression;

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

class AssignmentExpressionNode;
class BinaryExpressionNode;
class CallExpressionNode;
class IdentifierExpressionNode;
class IndexAccessExpressionNode;
class LiteralExpressionNode;
class MemberAccessExpressionNode;
class UnaryExpressionNode;


class BlockStatement;
class ExpressionStatement;
class IfStatement;
class ReturnStatement;
class WhileStatement;


class TypeNode;
class GenericTypeNode;
class NamedType;


class PatternNode;
class NamedPattern;


class NumberLiteral;
class StringLiteralNode;
class BoolLiteralNode;
class ArrayLiteralNode;
class StructLiteralNode;

}