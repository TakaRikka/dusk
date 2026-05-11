#include "dusk/hud_layout.hpp"

namespace dusk::hud_layout {
namespace {

ConfigVar<float>& offset_x_var(Button button) noexcept {
    auto& settings = getSettings().game;
    switch (button) {
    case Button::B:
        return settings.hudButtonBOffsetX;
    case Button::X:
        return settings.hudButtonXOffsetX;
    case Button::Y:
        return settings.hudButtonYOffsetX;
    case Button::Z:
        return settings.hudButtonZOffsetX;
    case Button::A:
    default:
        return settings.hudButtonAOffsetX;
    }
}

ConfigVar<float>& offset_y_var(Button button) noexcept {
    auto& settings = getSettings().game;
    switch (button) {
    case Button::B:
        return settings.hudButtonBOffsetY;
    case Button::X:
        return settings.hudButtonXOffsetY;
    case Button::Y:
        return settings.hudButtonYOffsetY;
    case Button::Z:
        return settings.hudButtonZOffsetY;
    case Button::A:
    default:
        return settings.hudButtonAOffsetY;
    }
}

ConfigVar<float>& scale_var(Button button) noexcept {
    auto& settings = getSettings().game;
    switch (button) {
    case Button::B:
        return settings.hudButtonBScale;
    case Button::X:
        return settings.hudButtonXScale;
    case Button::Y:
        return settings.hudButtonYScale;
    case Button::Z:
        return settings.hudButtonZScale;
    case Button::A:
    default:
        return settings.hudButtonAScale;
    }
}

u32 hash_float(u32 hash, float value) noexcept {
    const auto quantized = static_cast<s32>(value * 100.0f + (value >= 0.0f ? 0.5f : -0.5f));
    return (hash ^ static_cast<u32>(quantized)) * 16777619u;
}

u32 hash_button(u32 hash, Button button) noexcept {
    const Transform transform = ButtonTransform(button);
    hash = hash_float(hash, transform.offsetX);
    hash = hash_float(hash, transform.offsetY);
    return hash_float(hash, transform.scale);
}

}  // namespace

const char* LayoutName(ControllerOverlayLayout layout) noexcept {
    switch (layout) {
    case ControllerOverlayLayout::WiiU:
        return "Wii U";
    case ControllerOverlayLayout::XBox:
        return "XBox";
    case ControllerOverlayLayout::GameCube:
    default:
        return "GameCube";
    }
}

Transform ButtonTransform(Button button) noexcept {
    return {
        .offsetX = offset_x_var(button).getValue(),
        .offsetY = offset_y_var(button).getValue(),
        .scale = scale_var(button).getValue(),
    };
}

u32 LayoutStamp() noexcept {
    u32 hash = 2166136261u;
    hash = hash_button(hash, Button::A);
    hash = hash_button(hash, Button::B);
    hash = hash_button(hash, Button::X);
    hash = hash_button(hash, Button::Y);
    return hash_button(hash, Button::Z);
}

}  // namespace dusk::hud_layout
