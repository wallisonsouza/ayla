#pragma once

#include <vector>

template <typename T> class ContextStack {
public:
  explicit ContextStack(T initial) : current_(initial) {}

  T current() const { return current_; }

  void push(T value) {
    stack_.push_back(current_);
    current_ = value;
  }

  void pop() {
    current_ = stack_.back();
    stack_.pop_back();
  }

private:
  T current_;
  std::vector<T> stack_;
};