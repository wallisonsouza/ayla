#pragma once

#include <ostream>
#include <vector>

enum class Branch { More, Last };

class TreeLayout {

public:
  
  explicit TreeLayout(std::ostream &out);

  void enter(Branch branch);

  void leave();

  void blank_line();

private:
  void print_prefix();

private:
  std::ostream &out;

  std::vector<bool> branches;
};