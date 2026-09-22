#include "celestia/semantic/checker/TypeChecker.hpp"

void celestia::semantic::TypeChecker::check_block_statement(ast::BlockStatement *node) {
  if (!node) return;

  std::cout << "[TypeChecker] checking block\n";

  for (auto *item : node->items) {

    if (!item) continue;

    check(item);
  }
}