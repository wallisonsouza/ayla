#include "color.hpp"
#include <ostream>

namespace debug {
class StyledStream {
  std::ostream &os;
  bool autoReset;
  bool colorActive = false;

public:
  explicit StyledStream(std::ostream &out, bool autoReset_ = false) : os(out), autoReset(autoReset_) {}

  // Foreground Color
  StyledStream &operator<<(const Color &color) {
    os << color.fg();
    colorActive = true;
    return *this;
  }

  // Qualquer outro tipo
  template <typename T> StyledStream &operator<<(const T &value) {
    os << value;
    if (autoReset && colorActive) {
      os << Color::reset();
      colorActive = false;
    }
    return *this;
  }

  // manipuladores padrão (endl, etc)
  StyledStream &operator<<(std::ostream &(*manip)(std::ostream &)) {
    os << manip;
    if (manip == static_cast<std::ostream &(*)(std::ostream &)>(std::endl) && autoReset && colorActive) {
      os << Color::reset();
      colorActive = false;
    }
    return *this;
  }

  std::ostream &getStream() { return os; }

  void resetColor() {
    if (colorActive) {
      os << Color::reset();
      colorActive = false;
    }
  }
};

} // namespace debug