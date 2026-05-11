#pragma once

#include "dusk/settings.h"

namespace dusk::hud_layout {

enum class Button {
    A,
    B,
    X,
    Y,
    Z,
};

struct Transform {
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    float scale = 1.0f;
};

const char* LayoutName(ControllerOverlayLayout layout) noexcept;
Transform ButtonTransform(Button button) noexcept;
u32 LayoutStamp() noexcept;

}  // namespace dusk::hud_layout
