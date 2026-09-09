#pragma once

#include <cstdint>
#include <utility>
#include <vector>

namespace celestia::ir {

template <typename T, typename Id> class Table {
private:
  std::vector<T> items;

public:
  template <typename Factory> Id create(Factory &&factory) {

    Id id{static_cast<uint32_t>(items.size())};

    items.push_back(std::forward<Factory>(factory)(id));

    return id;
  }

  T &get(Id id) { return items[id.index()]; }

  const T &get(Id id) const { return items[id.index()]; }

  size_t size() const { return items.size(); }

  auto begin() { return items.begin(); }

  auto end() { return items.end(); }

  auto begin() const { return items.begin(); }

  auto end() const { return items.end(); }
};

} // namespace celestia::ir