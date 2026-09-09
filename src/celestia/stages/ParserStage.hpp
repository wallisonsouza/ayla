#include "celestia/compiler/Compiler.hpp"
#include "celestia/core/visitor/Stage.hpp"
#include "celestia/syntax/parser/Parser.hpp"
#include "celestia/syntax/parser/ParserContext.hpp"

class ParserStage : public Stage {
public:
  void run(Compiler &compiler, CompilationUnit &unit) {

    celestia::syntax::ParseContext context(unit, compiler.environment());
    celestia::syntax::Parser parser(context);

    parser.run();
  }
};
