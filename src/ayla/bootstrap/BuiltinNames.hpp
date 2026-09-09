#pragma once
#include <string_view>

namespace ayla::language {

namespace types {

inline constexpr std::string_view Int = "Int";
inline constexpr std::string_view UInt = "UInt";

inline constexpr std::string_view Int8 = "Int8";
inline constexpr std::string_view Int16 = "Int16";
inline constexpr std::string_view Int32 = "Int32";
inline constexpr std::string_view Int64 = "Int64";

inline constexpr std::string_view UInt8 = "UInt8";
inline constexpr std::string_view UInt16 = "UInt16";
inline constexpr std::string_view UInt32 = "UInt32";
inline constexpr std::string_view UInt64 = "UInt64";

inline constexpr std::string_view F32 = "F32";
inline constexpr std::string_view F64 = "F64";

inline constexpr std::string_view Bool = "Bool";
inline constexpr std::string_view Char = "Char";
inline constexpr std::string_view String = "String";
inline constexpr std::string_view Void = "Void";

} // namespace types

namespace intrinsics {

inline constexpr std::string_view Array = "Array";
inline constexpr std::string_view Map = "Map";
inline constexpr std::string_view Set = "Set";
inline constexpr std::string_view Ref = "Ref";

} // namespace intrinsics

} // namespace ayla::language
