#pragma once

#include <map>

namespace dusk::ui {

struct EquipTarget {
    float left = 0.0f;
    float top = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    bool valid = false;
};

enum class Control {
    A,
    B,
    X,
    Y,
    Z,
    L,
    R,
    FIRST_PERSON,
    ITEMS,
    COLLECTIONS,
    MAP,
    SKIP,
    DPAD_UP,
    DPAD_DOWN,
    DPAD_LEFT,
    DPAD_RIGHT,
    COUNT,
};

enum class ControlAnchor : u8 {
    None,
    Top,
    Left,
    Bottom,
    Right,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
};

struct ControlProps {
    float x = 0.0f;
    float y = 0.0f;
    float w = 0.0f;
    float h = 0.0f;
    float scale = 1.0f;
    ControlAnchor anchor = ControlAnchor::None;
};

struct ControlRect {
    float l = 0.0f;
    float t = 0.0f;
    float w = 0.0f;
    float h = 0.0f;
};

struct ResolvedControlLayout {
    ControlRect visual;
    ControlRect box;
    float scale = 1.0f;
};

struct ControlLayoutSize {
    float w = 0.0f;
    float h = 0.0f;
};

struct ControlLayout {
    static constexpr int Version = 1;

    int version = Version;
    std::map<std::string, ControlProps, std::less<> > controls;
};

constexpr std::array<std::string_view, 9> kControlLayoutIds = {
    "actionBar",
    "buttonA",
    "buttonB",
    "buttonX",
    "buttonY",
    "buttonZ",
    "skip",
    "triggerL",
    "triggerR",
};

constexpr bool is_control_layout_id(std::string_view id) noexcept {
    for (const auto knownId : kControlLayoutIds) {
        if (id == knownId) {
            return true;
        }
    }
    return false;
}

constexpr ControlRect resolve_anchored_rect(
    ControlAnchor anchor, float x, float y, float w, float h, ControlLayoutSize docSize) noexcept {
    switch (anchor) {
    case ControlAnchor::None:
        return {x * docSize.w - w * 0.5f, y * docSize.h - h * 0.5f, w, h};
    case ControlAnchor::Top:
        return {x * docSize.w - w * 0.5f, y, w, h};
    case ControlAnchor::Bottom:
        return {x * docSize.w - w * 0.5f, docSize.h - y - h, w, h};
    case ControlAnchor::Left:
        return {x, y * docSize.h - h * 0.5f, w, h};
    case ControlAnchor::Right:
        return {docSize.w - x - w, y * docSize.h - h * 0.5f, w, h};
    case ControlAnchor::TopLeft:
        return {x, y, w, h};
    case ControlAnchor::TopRight:
        return {docSize.w - x - w, y, w, h};
    case ControlAnchor::BottomLeft:
        return {x, docSize.h - y - h, w, h};
    case ControlAnchor::BottomRight:
        return {docSize.w - x - w, docSize.h - y - h, w, h};
    }
    return {};
}

constexpr ResolvedControlLayout resolve_control_layout(
    ControlProps props, ControlLayoutSize docSize) noexcept {
    const float visualW = props.w * props.scale;
    const float visualH = props.h * props.scale;
    const ControlRect visual =
        resolve_anchored_rect(props.anchor, props.x, props.y, visualW, visualH, docSize);
    const ControlRect box = {
        visual.l + (visual.w - props.w) * 0.5f,
        visual.t + (visual.h - props.h) * 0.5f,
        props.w,
        props.h,
    };
    return {
        .visual = visual,
        .box = box,
        .scale = props.scale,
    };
}

constexpr ControlProps encode_control_props(ControlRect visual, ControlLayoutSize docSize,
    ControlProps props, ControlAnchor anchor) noexcept {
    props.anchor = anchor;

    switch (anchor) {
    case ControlAnchor::None:
        props.x = (visual.l + visual.w * 0.5f) / docSize.w;
        props.y = (visual.t + visual.h * 0.5f) / docSize.h;
        break;
    case ControlAnchor::Top:
        props.x = (visual.l + visual.w * 0.5f) / docSize.w;
        props.y = visual.t;
        break;
    case ControlAnchor::Bottom:
        props.x = (visual.l + visual.w * 0.5f) / docSize.w;
        props.y = docSize.h - visual.t - visual.h;
        break;
    case ControlAnchor::Left:
        props.x = visual.l;
        props.y = (visual.t + visual.h * 0.5f) / docSize.h;
        break;
    case ControlAnchor::Right:
        props.x = docSize.w - visual.l - visual.w;
        props.y = (visual.t + visual.h * 0.5f) / docSize.h;
        break;
    case ControlAnchor::TopLeft:
        props.x = visual.l;
        props.y = visual.t;
        break;
    case ControlAnchor::TopRight:
        props.x = docSize.w - visual.l - visual.w;
        props.y = visual.t;
        break;
    case ControlAnchor::BottomLeft:
        props.x = visual.l;
        props.y = docSize.h - visual.t - visual.h;
        break;
    case ControlAnchor::BottomRight:
        props.x = docSize.w - visual.l - visual.w;
        props.y = docSize.h - visual.t - visual.h;
        break;
    }

    return props;
}

}  // namespace dusk::ui
