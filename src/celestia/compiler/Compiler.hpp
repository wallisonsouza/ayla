#pragma once

#include "celestia/compiler/CompilationPipeline.hpp"
#include "celestia/compiler/CompilationRules.hpp"
#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/CompilerEnvironment.hpp"
#include "celestia/language/LanguageDefinition.hpp"

#include <string>
#include <string_view>
#include <unordered_map>

class Compiler {
public:
  explicit Compiler(celestia::LanguageDefinition &lang);

  CompilationUnit *add_script(const std::string &path);

  void require(CompilationUnit &unit, StageId target, const CompilationRules &rules);

  void require(celestia::semantic::CompilationUnitId id, StageId target, const CompilationRules &rules);

  CompilerEnvironment &environment();

private:
  CompilerEnvironment environment_;
  CompilationPipeline pipeline_;

  std::unordered_map<std::string, celestia::semantic::CompilationUnitId> units_by_path_;
};