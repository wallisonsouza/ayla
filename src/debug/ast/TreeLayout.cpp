#include "TreeLayout.hpp"

TreeLayout::TreeLayout(std::ostream &out) : out(out) {}

void TreeLayout::enter(Branch branch) {
  print_prefix();

  switch (branch) {
  case Branch::More:
    out << "├─ ";
    branches.push_back(true);
    break;

  case Branch::Last:
    out << "└─ ";
    branches.push_back(false);
    break;
  }
}

void TreeLayout::leave() {
  if (!branches.empty()) branches.pop_back();
}

void TreeLayout::blank_line() {
  print_prefix();
  out << "│\n";
}

void TreeLayout::print_prefix() {
  for (bool alive : branches) { out << (alive ? "│  " : "   "); }
}
