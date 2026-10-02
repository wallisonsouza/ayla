#pragma once

#include <string_view>

enum class BinaryOperation
{
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,

   Assign,
    // Arrow,

    And,
    Or,

    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual
};

constexpr std::string_view to_string(BinaryOperation op)
{
    switch (op)
    {
        case BinaryOperation::Add:          return "+";
        case BinaryOperation::Subtract:     return "-";
        case BinaryOperation::Multiply:     return "*";
        case BinaryOperation::Divide:       return "/";
        case BinaryOperation::Modulo:       return "%";

        // case BinaryOperation::Assign:       return "=";
        // case BinaryOperation::Arrow:        return "->";

        case BinaryOperation::And:          return "&&";
        case BinaryOperation::Or:           return "||";

        case BinaryOperation::Equal:        return "==";
        case BinaryOperation::NotEqual:     return "!=";
        case BinaryOperation::Less:         return "<";
        case BinaryOperation::LessEqual:    return "<=";
        case BinaryOperation::Greater:      return ">";
        case BinaryOperation::GreaterEqual: return ">=";
    }

    return "?";
}