#pragma once

namespace diagnostic {

enum class ExpectedKind {
  Token,

  Identifier,

  Expression,
  Statement,
  Pattern,

  Type,

  Declaration
};

} // namespace diagnostic