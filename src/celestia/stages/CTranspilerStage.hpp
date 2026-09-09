#include "celestia/compiler/CompilationUnit.hpp"
#include "celestia/compiler/Compiler.hpp"
#include "celestia/core/visitor/Stage.hpp"
#include "celestia/transpiler/CGenerator.hpp"
#include <filesystem>
#include <fstream>

class CTranspileStage : public Stage {
public:
  void run(Compiler &compiler, CompilationUnit &unit) override {

    std::filesystem::path output_dir = "out";

    std::filesystem::create_directories(output_dir);

    std::filesystem::path source = unit.source.path;

    std::filesystem::path output = output_dir / source.filename();

    output.replace_extension(".c");

    std::ofstream out(output);

    if (!out) return;

    celestia::codegen::CGenerator generator(out, compiler.program);

    generator.generate();
  }
};