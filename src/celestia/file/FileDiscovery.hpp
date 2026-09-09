#pragma once

#include <filesystem>
#include <vector>

namespace celestia::project {

class FileDiscovery {
public:
  explicit FileDiscovery(std::vector<std::filesystem::path> roots);

  std::vector<std::filesystem::path> discover() const;

private:
  void discover_directory(const std::filesystem::path &root, std::vector<std::filesystem::path> &files) const;

  std::vector<std::filesystem::path> roots;
};

} // namespace celestia::project