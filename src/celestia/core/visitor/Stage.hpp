#pragma once

class CompilationUnit;
class Compiler;

class Stage {
public:
  virtual ~Stage() = default;

  virtual void run(Compiler &compiler, CompilationUnit &unit) = 0;
};