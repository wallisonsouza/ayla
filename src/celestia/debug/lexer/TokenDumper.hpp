#pragma once

#include "celestia/core/source/SourceBuffer.hpp"
#include "celestia/core/token/Token.hpp"
#include "celestia/core/token/token_stream.hpp"

#include <string>

namespace celestia::debug {

class TokenDumper {
public:
  std::string dump_token(const Token &token, const core::source::SourceBuffer &source);

  std::string dump(const core::token::TokenStream &tokens, const core::source::SourceBuffer &source);


  static std::string sanitize_text(std::string text, size_t max = 40);
};

} // namespace celestia::debug