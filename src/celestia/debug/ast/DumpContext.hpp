#pragma once

#include "TreeLayout.hpp"
#include "celestia/ast/Node.hpp"
#include "celestia/debug/console/color.hpp"
#include "celestia/debug/console/console.hpp"

#include <functional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

class DumpContext {

public:
  using Dispatch = std::function<void(const celestia::ast::Node *)>;

  using FieldValue = std::variant<const celestia::ast::Node *, std::string>;

  DumpContext(std::ostream &out, Dispatch dispatch) : out(out), layout(out), dispatch(std::move(dispatch)) {}

  struct Field {
    std::string name;
    FieldValue value;
  };

  class Object {

    const debug::Color NAME_COLOR = debug::Color::Blue;
    const debug::Color FIELD_COLOR = debug::Color::DarkGray;

  public:
    Object(DumpContext &ctx, std::string_view name) : context(ctx) { debug::Console::log(NAME_COLOR, name); }

    void field(std::string_view name, std::string_view value) { fields.push_back(Field{std::string(name), std::string(value)}); }

    void field(std::string_view name, const celestia::ast::Node *node) {

      if (!node) return;

      fields.push_back(Field{std::string(name), node});
    }

    template <typename T> void list(std::string_view name, const std::vector<T *> &nodes) {

      if (nodes.empty()) return;

      lists.push_back(List{std::string(name), std::vector<const celestia::ast::Node *>(nodes.begin(), nodes.end())});
    }

    ~Object() { flush(); }

  private:
    struct List {
      std::string name;
      std::vector<const celestia::ast::Node *> nodes;
    };

    void dump_field_value(const FieldValue &value) {

      std::visit(
          [this](const auto &value) {
            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, const celestia::ast::Node *>) {

              context.dispatch(value);

            } else {

              debug::Console::log(FIELD_COLOR, value);
            }
          },
          value);
    }

    void flush() {

      size_t total = fields.size() + lists.size();
      size_t index = 0;

      for (auto &f : fields) {

        bool last = ++index == total;

        context.layout.enter(last ? Branch::Last : Branch::More);

        debug::Console::log(FIELD_COLOR, f.name, ":");

        context.layout.enter(Branch::Last);

        dump_field_value(f.value);

        context.layout.leave();
        context.layout.leave();
      }

      for (auto &l : lists) {

        bool last = ++index == total;

        context.layout.enter(last ? Branch::Last : Branch::More);

        debug::Console::log(FIELD_COLOR, l.name, ":");

        for (size_t i = 0; i < l.nodes.size(); ++i) {

          bool childLast = i + 1 == l.nodes.size();

          context.layout.enter(childLast ? Branch::Last : Branch::More);

          context.dispatch(l.nodes[i]);

          context.layout.leave();
        }

        context.layout.leave();
      }
    }

    DumpContext &context;

    std::vector<Field> fields;
    std::vector<List> lists;
  };

  Object object(std::string_view name) { return Object(*this, name); }

private:
  std::ostream &out;
  TreeLayout layout;
  Dispatch dispatch;
};