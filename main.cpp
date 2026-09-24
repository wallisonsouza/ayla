#include "ayla/AylaMain.hpp"
#include "ayla/comand/CommandLine.hpp"
#include "celestia/semantic/resolver/Trace.hpp"

int main(int argc, char **argv) {

  celestia::debug::Trace::enable(celestia::debug::Category::SymbolCollector | celestia::debug::Category::Resolver | celestia::debug::Category::TypeChecker);

  return AylaApplication{}.run(parse_command_line(argc, argv));
}
