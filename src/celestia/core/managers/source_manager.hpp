#pragma once

#include "celestia/core/memory/Arena.hpp"
#include "celestia/core/source/Source.hpp"

#include <filesystem>
#include <unordered_map>

static std::filesystem::path normalize(const std::filesystem::path &path) { return std::filesystem::absolute(path).lexically_normal(); }

class SourceManager {
  core::memory::Arena arena;
  std::unordered_map<std::filesystem::path, core::source::Source *> sources;

public:
  core::source::Source *get_or_create(const std::filesystem::path &path) {

    auto normalized = normalize(path);

    if (auto it = sources.find(normalized); it != sources.end()) return it->second;

    auto *source = arena.alloc<core::source::Source>(normalized.string());

    sources.emplace(normalized, source);

    return source;
  }

  core::source::Source *find(const std::filesystem::path &path) const {

    auto normalized = normalize(path);

    auto it = sources.find(normalized);

    if (it == sources.end()) return nullptr;

    return it->second;
  }
};