#pragma once

#include "color.hpp"
#include "styled_stream.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>

namespace debug {

class Console {
private:
  using Clock = std::chrono::high_resolution_clock;

  inline static std::unordered_map<std::string, Clock::time_point> timers;
  inline static Color defaultColor = Color::White;

public:
  static void setColor(const Color &color) { defaultColor = color; }

  static Color getColor() { return defaultColor; }

  static void time(const std::string &label) {
    timers[label] = Clock::now();
    log(Color::BrightBlack, "[TIME START] ", label);
  }

  static void timeEnd(const std::string &label) {
    auto it = timers.find(label);

    if (it == timers.end()) {
      warn(Color::Yellow, "[WARN] timeEnd chamado sem time(): ", label);
      return;
    }

    auto end = Clock::now();

    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - it->second).count();

    timers.erase(it);

    double seconds = elapsed_ns / 1e9;
    double millis = elapsed_ns / 1e6;
    double micros = elapsed_ns / 1e3;

    std::string formatted;

    if (seconds >= 1.0)
      formatted = std::to_string(seconds) + "s";
    else if (millis >= 1.0)
      formatted = std::to_string(millis) + "ms";
    else
      formatted = std::to_string(micros) + "µs";

    log(Color::BrightCyan, "[TIME END] ", label, " -> ", formatted);
  }

  template <typename... Args> static void write(Color color, const Args &...args) {
    StyledStream ss(std::cout);

    ss << color;
    (ss << ... << args);
    ss << Color::reset();

    std::cout.flush();
  }

  template <typename... Args> static void log(Color color, const Args &...args) {
    write(color, args...);
    std::cout << '\n';
  }

  template <typename... Args> static void warn(const Args &...args) {
    StyledStream ss(std::cerr);

    ss << Color::Yellow;
    (ss << ... << args);
    ss << Color::reset() << std::endl;
  }

  template <typename... Args> static void error(const Args &...args) {
    StyledStream ss(std::cerr);

    ss << Color::Red;
    (ss << ... << args);
    ss << Color::reset() << std::endl;
  }

  template <typename... Args> static void debug(const Args &...args) {
    StyledStream ss(std::cout);

    ss << Color::Cyan;
    (ss << ... << args);
    ss << Color::reset() << std::endl;
  }
};

} // namespace debug