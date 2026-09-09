#include "FileDiscovery.hpp"

namespace celestia::project {

FileDiscovery::FileDiscovery(std::vector<std::filesystem::path> roots) : roots(std::move(roots)) {}

std::vector<std::filesystem::path> FileDiscovery::discover() const {

  std::vector<std::filesystem::path> files;

  for (const auto &root : roots) {

    if (!std::filesystem::exists(root)) continue;

    if (!std::filesystem::is_directory(root)) continue;

    discover_directory(root, files);
  }

  return files;
}

void FileDiscovery::discover_directory(const std::filesystem::path &root, std::vector<std::filesystem::path> &files) const {

  for (const auto &entry : std::filesystem::recursive_directory_iterator(root)) {

    if (!entry.is_regular_file()) continue;

    if (entry.path().extension() != ".ayla") continue;

    files.push_back(entry.path());
  }
}

} // namespace celestia::project