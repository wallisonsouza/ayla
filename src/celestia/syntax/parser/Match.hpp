#include <variant>

struct MatchSucess {};
struct NoMatch {};
struct MatchError {};

template <typename T> class Match {
public:
  using Storage = std::variant<T, NoMatch, MatchError>;

private:
  Storage m_result;

  explicit Match(Storage result) : m_result(std::move(result)) {}

public:
  static Match ok(T value) { return Match(Storage{std::in_place_type<T>, std::move(value)}); }

  static Match no_match() { return Match(Storage{std::in_place_type<NoMatch>}); }

  static Match fail() { return Match(Storage{std::in_place_type<MatchError>}); }

  bool is_ok() const { return std::holds_alternative<T>(m_result); }

  bool is_no_match() const { return std::holds_alternative<NoMatch>(m_result); }

  bool is_error() const { return std::holds_alternative<MatchError>(m_result); }

  T &value() { return std::get<T>(m_result); }

  const T &value() const { return std::get<T>(m_result); }
};