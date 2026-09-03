#pragma once

#include "dusk/iso_validate.hpp"
#include "dusk/settings.h"

#include <span>

namespace dusk::language {

std::span<const GameLanguage> available_languages(const iso::DiscInfo& info) noexcept;

const char* language_name(GameLanguage language) noexcept;
const char* msg_folder() noexcept;

}  // namespace dusk::language
