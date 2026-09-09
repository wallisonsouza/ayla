#pragma once
#include "TrieNode.hpp"
#include "celestia/core/token/TokenDescriptor.hpp"
#include "celestia/core/token/TokenGroup.hpp"
#include "celestia/core/token/TokenKind.hpp"
#include <deque>
#include <string>
#include <unordered_map>

namespace core::table {

class TokenTable {
public:
  celestia::TokenDescriptor &add(TokenKind kind, const std::string &name, TokenGroup group) {
    storage_.emplace_back(kind, group, name);
    celestia::TokenDescriptor &desc = storage_.back();
    by_kind_[kind] = &desc;
    by_name_[name] = &desc;
    trie_.insert(name, &desc);
    return desc;
  }

  // Adiciona um token sem nome (ex.: TokenKind::Identifier)
  celestia::TokenDescriptor &add(TokenKind kind, TokenGroup group) {
    storage_.emplace_back(kind, group, "");
    celestia::TokenDescriptor &desc = storage_.back();
    by_kind_[kind] = &desc;
    return desc;
  }

  // Adiciona um alias para um celestia::TokenDescriptor existente
  void add_alias(TokenKind kind, const std::string &alias) {

    auto desc = lookup_by_kind(kind);
    if (!desc) { return; }

    by_name_[alias] = desc;
    trie_.insert(alias, desc);
    desc->aliases.push_back(alias);
  }

  celestia::TokenDescriptor *lookup_by_kind(TokenKind kind) {
    auto it = by_kind_.find(kind);
    return it != by_kind_.end() ? it->second : nullptr;
  }

  const celestia::TokenDescriptor *lookup_by_kind(TokenKind kind) const {
    auto it = by_kind_.find(kind);
    return it != by_kind_.end() ? it->second : nullptr;
  }

  celestia::TokenDescriptor *lookup_by_name(std::string_view name) {
    auto it = by_name_.find(std::string(name));

    return it != by_name_.end() ? it->second : nullptr;
  }
  bool has_prefix(const std::string_view &prefix) const { return trie_.has_prefix(prefix); }

  const std::deque<celestia::TokenDescriptor> &all() const { return storage_; }

  Trie<celestia::TokenDescriptor> &trie() { return trie_; }

private:
  std::deque<celestia::TokenDescriptor> storage_;

  std::unordered_map<TokenKind, celestia::TokenDescriptor *> by_kind_;

  std::unordered_map<std::string, celestia::TokenDescriptor *> by_name_;
  
  Trie<celestia::TokenDescriptor> trie_;
};

} // namespace core::table
