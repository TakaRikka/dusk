#pragma once

#include <RmlUi/Core/Types.h>

#include <string>
#include <string_view>

namespace dusk::ui::i18n {

bool initialize() noexcept;
void shutdown() noexcept;
bool set_language(std::string_view language) noexcept;
const std::string& language() noexcept;
bool is_simplified_chinese() noexcept;
bool use_harmonyos_font() noexcept;
int translate(Rml::String& translated, const Rml::String& input);

}  // namespace dusk::ui::i18n
