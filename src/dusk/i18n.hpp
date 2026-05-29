#pragma once

#include <string>
#include <RmlUi/Core.h>

namespace dusk {
    void load_translations();
    Rml::String tr(const std::string& text);
}
