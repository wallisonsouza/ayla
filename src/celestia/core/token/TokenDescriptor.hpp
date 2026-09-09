#pragma once

#include "celestia/core/token/TokenGroup.hpp"
#include "celestia/core/token/TokenKind.hpp"
#include <string>
#include <vector>

namespace celestia {

struct TokenDescriptor {

  TokenKind kind;

  TokenGroup group;

  std::string name;

  std::vector<std::string> aliases;

  TokenDescriptor(TokenKind kind, TokenGroup group, const std::string &lexeme) : kind(kind), group(group), name(lexeme) {}
};

} // namespace celestia