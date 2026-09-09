#pragma once

#include "celestia/language/LanguageDefinition.hpp"

namespace ayla::language {

celestia::LanguageDefinition create_definition();

void register_operators(celestia::LanguageDefinition &definition);
void register_tokens(celestia::LanguageDefinition &definition);

}