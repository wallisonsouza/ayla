#pragma once

#include "SourceBuffer.hpp"
#include "celestia/utils/TextLoader.hpp"

#include <filesystem>
#include <utility>

namespace core::source {

struct Source {
  std::filesystem::path path;
  SourceBuffer buffer;

  explicit Source(std::filesystem::path p) : path(std::move(p)), buffer(utils::TextLoader::load_file(path)) {}

  std::filesystem::path filename() const { return path.filename(); }

  std::filesystem::path stem() const { return path.stem(); }
};

} // namespace core::source