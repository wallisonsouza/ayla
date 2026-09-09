#include "AylaLanguage.hpp"

namespace ayla::language {

celestia::LanguageDefinition create_definition() {

  celestia::LanguageDefinition definition;
  register_operators(definition);
  register_tokens(definition);

  return definition;
}

} // namespace ayla::language