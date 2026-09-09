#include "ayla/AylaMain.hpp"
#include "ayla/comand/CommandLine.hpp"

int main(int argc, char **argv) {
    
  celestia::debug::Trace::enable(celestia::debug::Category::Parser);
  celestia::debug::Trace::enable(celestia::debug::Category::Resolver);

  return AylaApplication{}.run(parse_command_line(argc, argv));
}