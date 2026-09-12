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

  rules.add<LexerStage>(StageId::Lex, {});

  rules.add<ParserStage>(StageId::Parser, {StageId::Lex});

  // rules.add<ModuleDiscoveryStage>(stages::ModuleParser, {stages::Parser});

  return rules;
}

CompilationRules CompilationRules::normal() {

  CompilationRules rules;

  rules.add<LexerStage>(StageId::Lex, {});

  rules.add<ParserStage>(StageId::Parser, {StageId::Lex});

  // rules.add<ModuleDiscoveryStage>(StageId::ModuleParser, {StageId::Parser});

  rules.add<celestia::semantic::SymbolCollectorStage>(StageId::SymbolCollector, {StageId::Parser});

  rules.add<celestia::semantic::ResolverStage>(StageId::Resolver, {StageId::SymbolCollector});

  rules.add<celestia::semantic::CheckStage>(StageId::Check, {StageId::Resolver});

  rules.add<LoweringStage>(StageId::Lowering, {StageId::Check});

  rules.add<CTranspileStage>(StageId::Transpile, {StageId::Lowering});

  return rules;
}