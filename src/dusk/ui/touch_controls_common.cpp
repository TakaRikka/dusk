#include "touch_controls_common.hpp"

#include <aurora/rmlui.hpp>

#include <algorithm>
#include <array>
#include <cmath>

namespace dusk::ui {
namespace {

constexpr std::array<TouchLayoutControlInfo, kTouchLayoutControlCount> kLayoutControls = {{
    {
        .layoutId = "triggerL",
        .elementId = "trigger-l",
        .props =
            {
                .x = 24.f,
                .y = 18.f,
                .w = 78.f,
                .h = 46.f,
                .scale = 1.f,
                .anchor = ControlAnchor::TopLeft,
            },
        .control = Control::L,
        .hasControl = true,
    },
    {
        .layoutId = "triggerR",
        .elementId = "trigger-r",
        .props =
            {
                .x = 24.f,
                .y = 18.f,
                .w = 78.f,
                .h = 46.f,
                .scale = 1.f,
                .anchor = ControlAnchor::TopRight,
            },
        .control = Control::R,
        .hasControl = true,
    },
    {
        .layoutId = "buttonZ",
        .elementId = "button-z",
        .props =
            {
                .x = 24.f,
                .y = 72.f,
                .w = 78.f,
                .h = 46.f,
                .scale = 1.f,
                .anchor = ControlAnchor::TopRight,
            },
        .control = Control::Z,
        .hasControl = true,
    },
    {
        .layoutId = "actionBar",
        .elementId = "action-bar",
        .props =
            {
                .x = 56.f,
                .y = 0.f,
                .w = 230.f,
                .h = 46.f,
                .scale = 1.f,
                .anchor = ControlAnchor::BottomLeft,
            },
    },
    {
        .layoutId = "skip",
        .elementId = "skip",
        .props =
            {
                .x = 24.f,
                .y = 18.f,
                .w = 64.f,
                .h = 46.f,
                .scale = 1.f,
                .anchor = ControlAnchor::TopRight,
            },
        .control = Control::SKIP,
        .hasControl = true,
    },
    {
        .layoutId = "buttonY",
        .elementId = "button-y",
        .props =
            {
                .x = 124.f,
                .y = 138.f,
                .w = 58.f,
                .h = 58.f,
                .scale = 1.f,
                .anchor = ControlAnchor::BottomRight,
            },
        .control = Control::Y,
        .hasControl = true,
    },
    {
        .layoutId = "buttonX",
        .elementId = "button-x",
        .props =
            {
                .x = 28.f,
                .y = 144.f,
                .w = 58.f,
                .h = 58.f,
                .scale = 1.f,
                .anchor = ControlAnchor::BottomRight,
            },
        .control = Control::X,
        .hasControl = true,
    },
    {
        .layoutId = "buttonB",
        .elementId = "button-b",
        .props =
            {
                .x = 158.f,
                .y = 48.f,
                .w = 58.f,
                .h = 58.f,
                .scale = 1.f,
                .anchor = ControlAnchor::BottomRight,
            },
        .control = Control::B,
        .hasControl = true,
    },
    {
        .layoutId = "buttonA",
        .elementId = "button-a",
        .props =
            {
                .x = 62.f,
                .y = 64.f,
                .w = 74.f,
                .h = 74.f,
                .scale = 1.f,
                .anchor = ControlAnchor::BottomRight,
            },
        .control = Control::A,
        .hasControl = true,
    },
}};

constexpr std::string_view kTouchControlsRmlFragment = R"RML(
    <button id="trigger-l" class="control trigger trigger-l"><span>L</span></button>
    <action-bar id="action-bar" class="control">
        <button id="items" class="utility items"><icon><glyph>&#xeb0e;</glyph></icon></button>
        <separator />
        <button id="first-person" class="utility first-person"><icon><glyph>&#xf4c9;</glyph></icon></button>
        <separator />
        <button id="map" class="utility map"><icon><glyph>&#xe55b;</glyph></icon></button>
        <separator />
        <button id="collections" class="utility collections"><icon><glyph>&#xe034;</glyph></icon></button>
    </action-bar>
    <button id="skip" class="control skip"><icon><glyph>&#xe044;</glyph></icon></button>

    <button id="trigger-r" class="control trigger trigger-r"><span>R</span></button>
    <button id="button-z" class="control trigger button-z midna"><img id="z-midna-icon" class="midna-icon" /><span>Z</span></button>

    <button id="button-y" class="control face y"><img id="button-y-icon" class="item-icon" /><oil-meter id="button-y-oil" class="oil-meter"><oil-fill id="button-y-oil-fill" /></oil-meter><count id="button-y-count" class="item-count"></count><span>Y</span></button>
    <button id="button-x" class="control face x"><img id="button-x-icon" class="item-icon" /><oil-meter id="button-x-oil" class="oil-meter"><oil-fill id="button-x-oil-fill" /></oil-meter><count id="button-x-count" class="item-count"></count><span>X</span></button>
    <button id="button-b" class="control face b"><img id="button-b-icon" class="item-icon" /><span>B</span></button>
    <button id="button-a" class="control face a"><span>A</span></button>
)RML";

}  // namespace

std::string_view touch_controls_rml_fragment() noexcept {
    return kTouchControlsRmlFragment;
}

std::span<const TouchLayoutControlInfo> touch_layout_controls() noexcept {
    return kLayoutControls;
}

const TouchLayoutControlInfo* find_touch_layout_control(std::string_view layoutId) noexcept {
    for (const auto& info : kLayoutControls) {
        if (info.layoutId == layoutId) {
            return &info;
        }
    }
    return nullptr;
}

const TouchLayoutControlInfo* find_touch_layout_control(Control control) noexcept {
    for (const auto& info : kLayoutControls) {
        if (info.hasControl && info.control == control) {
            return &info;
        }
    }
    return nullptr;
}

SDL_FingerID touch_event_id(const Rml::Event& event) noexcept {
    return event.GetParameter<SDL_FingerID>("finger_id", 0);
}

Rml::Vector2f touch_event_position(const Rml::Event& event) noexcept {
    return {
        event.GetParameter("x", 0.f),
        event.GetParameter("y", 0.f),
    };
}

Rml::Vector2f mouse_event_position(const Rml::Event& event) noexcept {
    return {
        event.GetParameter("mouse_x", 0.f),
        event.GetParameter("mouse_y", 0.f),
    };
}

float touch_dp_scale(Rml::Context* context) noexcept {
    if (context == nullptr) {
        context = aurora::rmlui::get_context();
    }
    if (context == nullptr) {
        return 1.f;
    }
    return std::max(context->GetDensityIndependentPixelRatio(), 1.f);
}

ControlLayoutSize touch_document_size_dp(Rml::Context* context) noexcept {
    if (context == nullptr) {
        return {};
    }

    const auto dimensions = context->GetDimensions();
    const float scale = touch_dp_scale(context);
    return {
        .w = static_cast<float>(dimensions.x) / scale,
        .h = static_cast<float>(dimensions.y) / scale,
    };
}

ControlAnchor touch_control_dock_anchor(ControlRect visual, ControlLayoutSize docSize) noexcept {
    if (docSize.w <= 0.f || docSize.h <= 0.f || visual.w <= 0.f || visual.h <= 0.f) {
        return ControlAnchor::None;
    }

    const bool top = control_float_near(visual.t, 0.f);
    const bool bottom = control_float_near(visual.t + visual.h, docSize.h);
    const bool left = control_float_near(visual.l, 0.f);
    const bool right = control_float_near(visual.l + visual.w, docSize.w);

    if (top && left && !right) {
        return ControlAnchor::TopLeft;
    }
    if (top && right && !left) {
        return ControlAnchor::TopRight;
    }
    if (bottom && left && !right) {
        return ControlAnchor::BottomLeft;
    }
    if (bottom && right && !left) {
        return ControlAnchor::BottomRight;
    }
    if (top) {
        return ControlAnchor::Top;
    }
    if (bottom) {
        return ControlAnchor::Bottom;
    }
    if (left) {
        return ControlAnchor::Left;
    }
    if (right) {
        return ControlAnchor::Right;
    }
    return ControlAnchor::None;
}

bool control_float_near(float a, float b) noexcept {
    return std::abs(a - b) <= 0.01f;
}

bool control_rect_near(ControlRect a, ControlRect b) noexcept {
    return control_float_near(a.l, b.l) && control_float_near(a.t, b.t) &&
           control_float_near(a.w, b.w) && control_float_near(a.h, b.h);
}

void apply_control_box_if_changed(
    Rml::Element* element, std::optional<ControlRect>& appliedBox, ControlRect box) noexcept {
    if (element == nullptr || (appliedBox && control_rect_near(*appliedBox, box))) {
        return;
    }

    element->SetProperty(Rml::PropertyId::Left, Rml::Property(box.l, Rml::Unit::DP));
    element->SetProperty(Rml::PropertyId::Top, Rml::Property(box.t, Rml::Unit::DP));
    element->SetProperty(Rml::PropertyId::Width, Rml::Property(box.w, Rml::Unit::DP));
    element->SetProperty(Rml::PropertyId::Height, Rml::Property(box.h, Rml::Unit::DP));
    appliedBox = box;
}

void apply_control_transform_if_changed(
    Rml::Element* element, std::optional<float>& appliedTransform, float scale) noexcept {
    if (element == nullptr || (appliedTransform && control_float_near(*appliedTransform, scale))) {
        return;
    }

    element->SetProperty(Rml::PropertyId::Transform,
        Rml::Transform::MakeProperty({Rml::Transforms::Scale2D{scale}}));
    appliedTransform = scale;
}

void apply_control_dock_classes(Rml::Element* element, ControlAnchor anchor) noexcept {
    if (element == nullptr) {
        return;
    }

    bool top = false;
    bool bottom = false;
    bool left = false;
    bool right = false;

    switch (anchor) {
    case ControlAnchor::Top:
        top = true;
        break;
    case ControlAnchor::Bottom:
        bottom = true;
        break;
    case ControlAnchor::Left:
        left = true;
        break;
    case ControlAnchor::Right:
        right = true;
        break;
    case ControlAnchor::TopLeft:
        top = true;
        left = true;
        break;
    case ControlAnchor::TopRight:
        top = true;
        right = true;
        break;
    case ControlAnchor::BottomLeft:
        bottom = true;
        left = true;
        break;
    case ControlAnchor::BottomRight:
        bottom = true;
        right = true;
        break;
    case ControlAnchor::None:
        break;
    }

    element->SetClass("docked", top || bottom || left || right);
    element->SetClass("docked-top", top);
    element->SetClass("docked-bottom", bottom);
    element->SetClass("docked-left", left);
    element->SetClass("docked-right", right);
}

}  // namespace dusk::ui
