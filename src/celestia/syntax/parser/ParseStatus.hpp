// #pragma once
// #include <utility>

// namespace celestia::syntax {

// enum class ParseStatus { Success, NoMatch, Error };

// template <typename T> class ParseResult {

//   ParseStatus status_;
//   T value_;

//   ParseResult(ParseStatus status, T value) : status_(status), value_(std::move(value)) {}

// public:
//   static ParseResult ok(T value) { return ParseResult(ParseStatus::Success, std::move(value)); }

//   static ParseResult fail() { return ParseResult(ParseStatus::Error, T{}); }

//   static ParseResult no_match() { return ParseResult(ParseStatus::NoMatch, T{}); }

//   bool is_ok() const { return status_ == ParseStatus::Success; }

//   bool is_error() const { return status_ == ParseStatus::Error; }

//   bool is_no_match() const { return status_ == ParseStatus::NoMatch; }

//   T &value() { return value_; }

//   const T &value() const { return value_; }
// };

// } // namespace celestia::syntax