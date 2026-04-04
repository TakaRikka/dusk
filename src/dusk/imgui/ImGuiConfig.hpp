#ifndef DUSK_IMGUICONFIG_HPP
#define DUSK_IMGUICONFIG_HPP

#include "dusk/config.hpp"
#include "imgui.h"

namespace dusk::config {
    inline void ImguiCheckbox(const char* title, ConfigVar<bool>& var) {
        bool copy = var.getValue();
        if (ImGui::Checkbox(title, &copy)) {
            var.setValue(copy);
            Save();
        }
    }
}

#endif  // DUSK_IMGUICONFIG_HPP
