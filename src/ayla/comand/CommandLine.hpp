#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>


enum class Command {
    None,
    Run,
    Build,
    Check,
    Help,
    Version
};


enum class DumpFlags : std::uint32_t {

    None   = 0,
    Tokens = 1 << 0,
    Ast    = 1 << 1,
    Hir    = 1 << 2,
    Mir    = 1 << 3,
    Ir     = 1 << 4
};


constexpr DumpFlags operator|(DumpFlags lhs, DumpFlags rhs)
{
    return static_cast<DumpFlags>(
        static_cast<std::uint32_t>(lhs) |
        static_cast<std::uint32_t>(rhs)
    );
}


constexpr DumpFlags &operator|=(DumpFlags &lhs, DumpFlags rhs)
{
    lhs = lhs | rhs;
    return lhs;
}


constexpr bool has_flag(DumpFlags flags, DumpFlags value)
{
    return (static_cast<std::uint32_t>(flags) &
            static_cast<std::uint32_t>(value)) != 0;
}


struct CommandLine {

    Command command = Command::None;

    std::optional<std::filesystem::path> input;

    DumpFlags dumps = DumpFlags::None;
};


CommandLine parse_command_line(int argc, char *argv[]);