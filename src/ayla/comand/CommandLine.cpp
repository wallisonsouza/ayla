#include "CommandLine.hpp"

#include <stdexcept>
#include <string>


static DumpFlags parse_dump(std::string_view value)
{
    if (value == "tokens")
        return DumpFlags::Tokens;

    if (value == "ast")
        return DumpFlags::Ast;

    if (value == "hir")
        return DumpFlags::Hir;

    if (value == "mir")
        return DumpFlags::Mir;

    if (value == "ir")
        return DumpFlags::Ir;


    throw std::runtime_error(
        "Dump desconhecido: " + std::string(value)
    );
}


static Command parse_command(std::string_view value)
{
    if (value == "run")
        return Command::Run;

    if (value == "build")
        return Command::Build;

    if (value == "check")
        return Command::Check;

    if (value == "--help")
        return Command::Help;

    if (value == "--version")
        return Command::Version;


    throw std::runtime_error(
        "Comando desconhecido: " + std::string(value)
    );
}


CommandLine parse_command_line(int argc, char *argv[])
{
    CommandLine cmd;


    if (argc < 2)
        return cmd;


    cmd.command = parse_command(argv[1]);


    for (int i = 2; i < argc; ++i) {

        std::string arg = argv[i];


        if (arg == "-d" || arg == "--dump") {

            if (i + 1 >= argc)
                throw std::runtime_error(
                    "Esperado valor após " + arg
                );


            cmd.dumps |= parse_dump(argv[++i]);

            continue;
        }


        if (!cmd.input) {

            cmd.input = std::filesystem::path(arg);

            continue;
        }


        throw std::runtime_error(
            "Argumento inesperado: " + arg
        );
    }


    return cmd;
}