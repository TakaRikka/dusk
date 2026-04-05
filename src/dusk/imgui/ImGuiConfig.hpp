#ifndef DUSK_IMGUICONFIG_HPP
#define DUSK_IMGUICONFIG_HPP

#include "dusk/config.hpp"
#include "imgui.h"

namespace dusk::config {
    inline void ImGuiCheckbox(const char* title, ConfigVar<bool>& var) {
        bool copy = var.getValue();
        if (ImGui::Checkbox(title, &copy)) {
            var.setValue(copy);
            Save();
        }
    }

    static void ImGuiSliderFloat(const char* label, ConfigVar<float>& var, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        float val = var;
        if (ImGui::SliderFloat(label, &val, v_min, v_max, format, flags)) {
            var.setValue(val);
            Save();
        }
    }

    static void ImGuiSliderInt(const char* label, ConfigVar<int>& var, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        int val = var;
        if (ImGui::SliderInt(label, &val, v_min, v_max, format, flags)) {
            var.setValue(val);
            Save();
        }
    }

    static void ImGuiMenuItem(const char* label, const char* shortcut, ConfigVar<bool>& p_selected, bool enabled = true) {
        bool copy = p_selected.getValue();
        if (ImGui::MenuItem(label, shortcut, &copy, enabled)) {
            p_selected.setValue(copy);
            Save();
        }
    }
}

#endif  // DUSK_IMGUICONFIG_HPP
