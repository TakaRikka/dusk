#pragma once

#include "SSystem/SComponent/c_API_controller_pad.h"
#include "dusk/settings.h"
#include "nlohmann/json.hpp"

#include <SDL3/SDL_events.h>

namespace dusk::touch_controls {

bool HandleEvent(const SDL_Event& event) noexcept;
void DrawOverlay() noexcept;
void MergeToPad(interface_of_controller_pad& pad) noexcept;
void ResetInputState() noexcept;
void ApplyPreset(ControllerOverlayLayout preset) noexcept;
void SaveLayout() noexcept;
nlohmann::json ExportLayout();
bool ImportLayout(const nlohmann::json& root) noexcept;

}  // namespace dusk::touch_controls
