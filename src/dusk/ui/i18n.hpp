#pragma once

#include <RmlUi/Core.h>

#include "dusk/settings.h"

#include <string_view>

namespace dusk::ui::i18n {

UiLanguage language() noexcept;
Rml::String tr(std::string_view text);
Rml::String tr_rml(std::string_view text);

}  // namespace dusk::ui::i18n
