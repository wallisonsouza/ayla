
#include "celestia/semantic/id/ids.hpp"
#include <filesystem>

class ModuleIndex {
public:
  void add(celestia::semantic::ModuleId module, std::filesystem::path file) {
    auto &files = providers_[module];

    for (const auto &existing : files) {
      if (existing == file) return;
    }

    files.push_back(std::move(file));
  }

  const std::vector<std::filesystem::path> &files(celestia::semantic::ModuleId module) const {
    static const std::vector<std::filesystem::path> empty;

    auto it = providers_.find(module);

    if (it == providers_.end()) return empty;

    return it->second;
  }

private:
  std::unordered_map<celestia::semantic::ModuleId, std::vector<std::filesystem::path>> providers_;
};
