#pragma once

#include "celestia/core/source/Source.hpp"

#include "celestia/core/token/token_stream.hpp"
#include "celestia/language/LanguageDefinition.hpp"

struct LexerContext {
  
  celestia::LanguageDefinition &language;

  core::source::Source &source;

  core::token::TokenStream &tokens;

  LexerContext(celestia::LanguageDefinition &language, core::source::Source &source, core::token::TokenStream &tokens) : language(language), source(source), tokens(tokens) {}
};