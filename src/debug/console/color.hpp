#pragma once

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

namespace debug {

class Color {
public:
  uint8_t r, g, b, a;

  constexpr Color(
      uint8_t red,
      uint8_t green,
      uint8_t blue,
      uint8_t alpha = 255)
      : r(red), g(green), b(blue), a(alpha) {}

  std::string fg() const {
    std::ostringstream oss;
    oss << "\033[38;2;" << int(r) << ";" << int(g) << ";" << int(b) << "m";
    return oss.str();
  }

  std::string bg() const {
    std::ostringstream oss;
    oss << "\033[48;2;" << int(r) << ";" << int(g) << ";" << int(b) << "m";
    return oss.str();
  }

  static std::string reset() { return "\033[0m"; }

  static const Color Black;
  static const Color Red;
  static const Color Green;
  static const Color Yellow;
  static const Color Blue;
  static const Color Magenta;
  static const Color Cyan;
  static const Color White;

  static const Color BrightBlack;
  static const Color BrightRed;
  static const Color BrightGreen;
  static const Color BrightYellow;
  static const Color BrightBlue;
  static const Color BrightMagenta;
  static const Color BrightCyan;
  static const Color BrightWhite;

  static const Color Orange;
  static const Color Purple;
  static const Color Pink;
  static const Color Teal;
  static const Color Lime;
  static const Color Navy;
  static const Color Maroon;
  static const Color Olive;

  static const Color DarkRed;
  static const Color SoftRed;
  static const Color MutedRed;
  static const Color PinkRed;
  static const Color MediumSlateBlue;
  static const Color SoftYellow;
  static const Color DarkGray;

  // Cores suaves para anotações semânticas.
  static const Color SoftCyan;
  static const Color SoftGreen;
  static const Color SoftMagenta;
  static const Color SoftWhite;
  static const Color MutedCyan;
  static const Color MutedGreen;
  static const Color MutedMagenta;
};

// -----------------------------------------------------------------------------
// Basic colors
// -----------------------------------------------------------------------------

inline const Color Color::Black(0, 0, 0);
inline const Color Color::Red(220, 38, 38);
inline const Color Color::Green(0, 255, 0);
inline const Color Color::Yellow(255, 255, 0);
inline const Color Color::Blue(0, 0, 255);
inline const Color Color::Magenta(255, 0, 255);
inline const Color Color::Cyan(0, 255, 255);
inline const Color Color::White(255, 255, 255);

// -----------------------------------------------------------------------------
// Bright colors
// -----------------------------------------------------------------------------

inline const Color Color::BrightBlack(128, 128, 128);
inline const Color Color::BrightRed(255, 0, 0);
inline const Color Color::BrightGreen(0, 255, 0);
inline const Color Color::BrightYellow(255, 255, 0);
inline const Color Color::BrightBlue(0, 0, 255);
inline const Color Color::BrightMagenta(255, 0, 255);
inline const Color Color::BrightCyan(0, 255, 255);
inline const Color Color::BrightWhite(255, 255, 255);

// -----------------------------------------------------------------------------
// Extended colors
// -----------------------------------------------------------------------------

inline const Color Color::Orange(255, 165, 0);
inline const Color Color::Purple(128, 0, 128);
inline const Color Color::Pink(255, 192, 203);
inline const Color Color::Teal(0, 128, 128);
inline const Color Color::Lime(0, 255, 0);
inline const Color Color::Navy(0, 0, 128);
inline const Color Color::Maroon(128, 0, 0);
inline const Color Color::Olive(128, 128, 0);

// -----------------------------------------------------------------------------
// Diagnostic colors
// -----------------------------------------------------------------------------

inline const Color Color::DarkRed(153, 27, 27);
inline const Color Color::SoftRed(248, 113, 113);
inline const Color Color::MutedRed(185, 28, 28);
inline const Color Color::PinkRed(251, 113, 133);
inline const Color Color::MediumSlateBlue(123, 104, 238);
inline const Color Color::SoftYellow(250, 204, 21);
inline const Color Color::DarkGray(169, 169, 169);

// -----------------------------------------------------------------------------
// Soft semantic colors
// -----------------------------------------------------------------------------

inline const Color Color::SoftCyan(103, 200, 218);
inline const Color Color::SoftGreen(134, 197, 154);
inline const Color Color::SoftMagenta(196, 150, 204);
inline const Color Color::SoftWhite(220, 220, 220);

// -----------------------------------------------------------------------------
// Muted semantic colors
// -----------------------------------------------------------------------------

inline const Color Color::MutedCyan(110, 155, 165);
inline const Color Color::MutedGreen(130, 160, 135);
inline const Color Color::MutedMagenta(160, 135, 165);

} // namespace debug