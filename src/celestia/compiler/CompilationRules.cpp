#include "CompilationRules.hpp"

#include "celestia/stages/CTranspilerStage.hpp"
#include "celestia/stages/CheckStage.hpp"
#include "celestia/stages/LexerStage.hpp"
#include "celestia/stages/LoweringStage.hpp"
#include "celestia/stages/ModuleDiscovery.hpp"
#include "celestia/stages/ParserStage.hpp"
#include "celestia/stages/ResolverStage.hpp"
#include "celestia/stages/SymbolCollectorStage.hpp"

CompilationRules CompilationRules::discovery() {

  CompilationRules rules;

  rules.add<LexerStage>(stages::Lex, {});

  rules.add<ParserStage>(stages::Parser, {stages::Lex});

  // rules.add<ModuleDiscoveryStage>(stages::ModuleParser, {stages::Parser});

  return rules;
}

CompilationRules CompilationRules::normal() {

  CompilationRules rules;

  rules.add<LexerStage>(stages::Lex, {});

  rules.add<ParserStage>(stages::Parser, {stages::Lex});

  // rules.add<ModuleDiscoveryStage>(stages::ModuleParser, {stages::Parser});

  rules.add<celestia::semantic::SymbolCollectorStage>(stages::SymbolCollector, {stages::Parser});

  rules.add<celestia::semantic::ResolverStage>(stages::Resolver, {stages::SymbolCollector});

  rules.add<celestia::semantic::CheckStage>(stages::Check, {stages::Resolver});

  rules.add<LoweringStage>(stages::Lowering, {stages::Check});

  rules.add<CTranspileStage>(stages::Transpile, {stages::Lowering});

  return rules;
}