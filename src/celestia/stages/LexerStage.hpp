#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/Compiler.hpp"
#include "celestia/core/visitor/Stage.hpp"
#include "celestia/syntax/lexer/Lexer.hpp"
#include "celestia/syntax/lexer/LexerContext.hpp"

class LexerStage : public Stage {
public:
  void run(Compiler &compiler, CompilationUnit &unit) override {

    LexerContext ctx(compiler.environment().language, unit.source, unit.tokens);

    Lexer lexer(ctx);

    lexer.tokenize();
  }
};