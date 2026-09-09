#pragma once

#include "celestia/core/table/PrecedenceTable.hpp"
#include "celestia/core/table/TokenTable.hpp"

namespace celestia {

struct LanguageDefinition {

  core::table::TokenTable tokens;
  core::table::OperatorTable operators;
};

} // namespace celestia