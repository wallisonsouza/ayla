#pragma once

#include "celestia/core/memory/Arena.hpp"
#include "celestia/core/token/Token.hpp"

#include <utility>
#include <vector>

namespace core::token {

class TokenStream {

public:
  TokenStream() = default;

  // ============================================================
  // Token creation
  // ============================================================

  template <typename T, typename... Args> T *create_token(Args &&...args) {
    auto *token = arena_.alloc<T>(std::forward<Args>(args)...);
    tokens_.push_back(token);
    return token;
  }

  // ============================================================
  // Inspection
  // ============================================================

  Token *at(size_t distance = 0) const noexcept {
    const auto index = pos_ + distance;

    if (index >= tokens_.size()) return nullptr;

    return tokens_[index];
  }

  TokenKind kind(size_t distance = 0) const noexcept {
    auto *token = at(distance);
    return token ? token->kind() : TokenKind::EndOfFile;
  }

  bool check(TokenKind expected, size_t distance = 0) const noexcept { return kind(distance) == expected; }

  bool is_end() const noexcept { return pos_ >= tokens_.size(); }

  size_t position() const noexcept { return pos_; }

  Token *current() const noexcept { return at(); }

  Token *previous() const noexcept { return pos_ > 0 ? tokens_[pos_ - 1] : nullptr; }

  Token *next() const noexcept { return at(1); }

  bool is_trivia() const noexcept { return kind() == TokenKind::NEW_LINE; }

  void skip_trivia() noexcept {
    while (is_trivia()) consume();
  }

  // ============================================================
  // Consumption
  // ============================================================

  Token *consume() noexcept {
    auto *token = current();

    if (!token) return nullptr;

    last_token_ = token;
    ++pos_;

    return token;
  }

  Token *match(TokenKind expected) noexcept {
    if (!check(expected)) return nullptr;

    return consume();
  }

  bool consume_if(TokenKind expected) noexcept { return match(expected) != nullptr; }

  void consume(size_t count) noexcept {
    while (count-- > 0 && !is_end()) consume();
  }

  size_t size() const noexcept { return tokens_.size(); }

  // ============================================================
  // Checkpoints
  // ============================================================

  void add_checkpoint() noexcept {
    checkpoints_.push_back({
        .position = pos_,
        .last_token = last_token_,
    });
  }

  void rollback_checkpoint() noexcept {
    if (checkpoints_.empty()) return;

    auto checkpoint = checkpoints_.back();
    checkpoints_.pop_back();

    pos_ = checkpoint.position;
    last_token_ = checkpoint.last_token;
  }

  void discard_checkpoint() noexcept {
    if (checkpoints_.empty()) return;

    checkpoints_.pop_back();
  }

private:
  struct Checkpoint {
    size_t position;
    Token *last_token;
  };

  core::memory::Arena arena_;
  std::vector<Token *> tokens_;

  size_t pos_ = 0;
  std::vector<Checkpoint> checkpoints_;

  Token *last_token_ = nullptr;
};

} // namespace core::token