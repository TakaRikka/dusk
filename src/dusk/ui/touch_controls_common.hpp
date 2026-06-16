#pragma once

#include "controls.hpp"

#include <RmlUi/Core.h>
#include <SDL3/SDL_touch.h>

#include <cstddef>
#include <optional>
#include <span>
#include <string_view>

namespace dusk::ui {

constexpr std::size_t kTouchLayoutControlCount = 9;

struct TouchLayoutControlInfo {
    std::string_view layoutId;
    const char* elementId = nullptr;
    ControlProps props;
    Control control = Control::COUNT;
    bool hasControl = false;
};

std::string_view touch_controls_rml_fragment() noexcept;
std::span<const TouchLayoutControlInfo> touch_layout_controls() noexcept;
const TouchLayoutControlInfo* find_touch_layout_control(std::string_view layoutId) noexcept;
const TouchLayoutControlInfo* find_touch_layout_control(Control control) noexcept;

SDL_FingerID touch_event_id(const Rml::Event& event) noexcept;
Rml::Vector2f touch_event_position(const Rml::Event& event) noexcept;
Rml::Vector2f mouse_event_position(const Rml::Event& event) noexcept;
float touch_dp_scale(Rml::Context* context = nullptr) noexcept;
ControlLayoutSize touch_document_size_dp(Rml::Context* context) noexcept;
ControlAnchor touch_control_dock_anchor(ControlRect visual, ControlLayoutSize docSize) noexcept;

bool control_float_near(float a, float b) noexcept;
bool control_rect_near(ControlRect a, ControlRect b) noexcept;
void apply_control_box_if_changed(
    Rml::Element* element, std::optional<ControlRect>& appliedBox, ControlRect box) noexcept;
void apply_control_transform_if_changed(
    Rml::Element* element, std::optional<float>& appliedTransform, float scale) noexcept;
void apply_control_dock_classes(Rml::Element* element, ControlAnchor anchor) noexcept;

}  // namespace dusk::ui
