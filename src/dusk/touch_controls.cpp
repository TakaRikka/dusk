#include "dusk/touch_controls.hpp"

#include "dusk/io.hpp"
#include "dusk/main.h"
#include "dusk/ui/ui.hpp"
#include <dolphin/pad.h>
#include "imgui.h"
#include "nlohmann/json.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <limits>
#include <string_view>
#include <utility>
#include <vector>

namespace dusk::touch_controls {
namespace {

using json = nlohmann::json;

constexpr float kReferenceWidth = 1280.0f;
constexpr float kReferenceHeight = 720.0f;
constexpr float kPi = 3.14159265358979323846f;
constexpr size_t kMaxTouches = 16;

enum class ControlId {
    LeftStick,
    CStick,
    A,
    B,
    X,
    Y,
    L,
    R,
    Z,
    Start,
    DpadUp,
    DpadDown,
    DpadLeft,
    DpadRight,
};

struct TouchControl {
    ControlId id;
    const char* label;
    float x;
    float y;
    float radius;
    float scale;
    bool stick;
};

struct ActiveTouch {
    SDL_FingerID fingerId = 0;
    int controlIndex = -1;
    float x = 0.0f;
    float y = 0.0f;
    bool editing = false;
    bool active = false;
};

struct StickState {
    float x = 0.0f;
    float y = 0.0f;
    float value = 0.0f;
    s16 angle = 0;
};

std::vector<TouchControl> sControls;
std::array<ActiveTouch, kMaxTouches> sTouches;
StickState sMainStick;
StickState sCStick;
u32 sHeldButtons = 0;
u32 sLastMergedButtons = 0;
bool sLoaded = false;

std::filesystem::path layout_path() {
    if (ConfigPath.empty()) {
        return {};
    }
    return ConfigPath / "touch_controls.json";
}

const char* id_name(ControlId id) noexcept {
    switch (id) {
    case ControlId::LeftStick:
        return "left_stick";
    case ControlId::CStick:
        return "c_stick";
    case ControlId::A:
        return "a";
    case ControlId::B:
        return "b";
    case ControlId::X:
        return "x";
    case ControlId::Y:
        return "y";
    case ControlId::L:
        return "l";
    case ControlId::R:
        return "r";
    case ControlId::Z:
        return "z";
    case ControlId::Start:
        return "start";
    case ControlId::DpadUp:
        return "dpad_up";
    case ControlId::DpadDown:
        return "dpad_down";
    case ControlId::DpadLeft:
        return "dpad_left";
    case ControlId::DpadRight:
        return "dpad_right";
    default:
        return "";
    }
}

bool parse_id(std::string_view name, ControlId& out) noexcept {
    for (ControlId id : {
             ControlId::LeftStick,
             ControlId::CStick,
             ControlId::A,
             ControlId::B,
             ControlId::X,
             ControlId::Y,
             ControlId::L,
             ControlId::R,
             ControlId::Z,
             ControlId::Start,
             ControlId::DpadUp,
             ControlId::DpadDown,
             ControlId::DpadLeft,
             ControlId::DpadRight,
         })
    {
        if (name == id_name(id)) {
            out = id;
            return true;
        }
    }
    return false;
}

u32 button_mask(ControlId id) noexcept {
    switch (id) {
    case ControlId::A:
        return PAD_BUTTON_A;
    case ControlId::B:
        return PAD_BUTTON_B;
    case ControlId::X:
        return PAD_BUTTON_X;
    case ControlId::Y:
        return PAD_BUTTON_Y;
    case ControlId::L:
        return PAD_TRIGGER_L;
    case ControlId::R:
        return PAD_TRIGGER_R;
    case ControlId::Z:
        return PAD_TRIGGER_Z;
    case ControlId::Start:
        return PAD_BUTTON_START;
    case ControlId::DpadUp:
        return PAD_BUTTON_UP;
    case ControlId::DpadDown:
        return PAD_BUTTON_DOWN;
    case ControlId::DpadLeft:
        return PAD_BUTTON_LEFT;
    case ControlId::DpadRight:
        return PAD_BUTTON_RIGHT;
    default:
        return 0;
    }
}

std::vector<TouchControl> default_layout(ControllerOverlayLayout preset) {
    switch (preset) {
    case ControllerOverlayLayout::WiiU:
        return {
            {ControlId::LeftStick, "L", 0.16f, 0.72f, 58.0f, 1.0f, true},
            {ControlId::CStick, "C", 0.70f, 0.78f, 44.0f, 1.0f, true},
            {ControlId::A, "A", 0.89f, 0.61f, 32.0f, 1.0f, false},
            {ControlId::B, "B", 0.82f, 0.71f, 32.0f, 1.0f, false},
            {ControlId::X, "X", 0.82f, 0.51f, 32.0f, 1.0f, false},
            {ControlId::Y, "Y", 0.75f, 0.61f, 32.0f, 1.0f, false},
            {ControlId::L, "L", 0.15f, 0.18f, 34.0f, 1.0f, false},
            {ControlId::R, "R", 0.85f, 0.18f, 34.0f, 1.0f, false},
            {ControlId::Z, "Z", 0.92f, 0.31f, 30.0f, 1.0f, false},
            {ControlId::Start, "S", 0.53f, 0.82f, 26.0f, 1.0f, false},
            {ControlId::DpadUp, "Up", 0.28f, 0.76f, 22.0f, 0.95f, false},
            {ControlId::DpadDown, "Dn", 0.28f, 0.86f, 22.0f, 0.95f, false},
            {ControlId::DpadLeft, "Lt", 0.23f, 0.81f, 22.0f, 0.95f, false},
            {ControlId::DpadRight, "Rt", 0.33f, 0.81f, 22.0f, 0.95f, false},
        };
    case ControllerOverlayLayout::XBox:
        return {
            {ControlId::LeftStick, "L", 0.16f, 0.72f, 58.0f, 1.0f, true},
            {ControlId::CStick, "C", 0.70f, 0.78f, 44.0f, 1.0f, true},
            {ControlId::A, "A", 0.82f, 0.71f, 32.0f, 1.0f, false},
            {ControlId::B, "B", 0.89f, 0.61f, 32.0f, 1.0f, false},
            {ControlId::X, "X", 0.75f, 0.61f, 32.0f, 1.0f, false},
            {ControlId::Y, "Y", 0.82f, 0.51f, 32.0f, 1.0f, false},
            {ControlId::L, "L", 0.15f, 0.18f, 34.0f, 1.0f, false},
            {ControlId::R, "R", 0.85f, 0.18f, 34.0f, 1.0f, false},
            {ControlId::Z, "Z", 0.92f, 0.31f, 30.0f, 1.0f, false},
            {ControlId::Start, "S", 0.53f, 0.82f, 26.0f, 1.0f, false},
            {ControlId::DpadUp, "Up", 0.28f, 0.76f, 22.0f, 0.95f, false},
            {ControlId::DpadDown, "Dn", 0.28f, 0.86f, 22.0f, 0.95f, false},
            {ControlId::DpadLeft, "Lt", 0.23f, 0.81f, 22.0f, 0.95f, false},
            {ControlId::DpadRight, "Rt", 0.33f, 0.81f, 22.0f, 0.95f, false},
        };
    case ControllerOverlayLayout::GameCube:
    default:
        return {
            {ControlId::LeftStick, "L", 0.16f, 0.72f, 58.0f, 1.0f, true},
            {ControlId::CStick, "C", 0.63f, 0.78f, 44.0f, 1.0f, true},
            {ControlId::A, "A", 0.85f, 0.66f, 36.0f, 1.0f, false},
            {ControlId::B, "B", 0.76f, 0.74f, 28.0f, 1.0f, false},
            {ControlId::X, "X", 0.93f, 0.62f, 26.0f, 1.0f, false},
            {ControlId::Y, "Y", 0.82f, 0.53f, 26.0f, 1.0f, false},
            {ControlId::L, "L", 0.15f, 0.18f, 34.0f, 1.0f, false},
            {ControlId::R, "R", 0.85f, 0.18f, 34.0f, 1.0f, false},
            {ControlId::Z, "Z", 0.93f, 0.31f, 28.0f, 1.0f, false},
            {ControlId::Start, "S", 0.50f, 0.82f, 26.0f, 1.0f, false},
            {ControlId::DpadUp, "Up", 0.28f, 0.76f, 22.0f, 0.95f, false},
            {ControlId::DpadDown, "Dn", 0.28f, 0.86f, 22.0f, 0.95f, false},
            {ControlId::DpadLeft, "Lt", 0.23f, 0.81f, 22.0f, 0.95f, false},
            {ControlId::DpadRight, "Rt", 0.33f, 0.81f, 22.0f, 0.95f, false},
        };
    }
}

ImVec2 display_size() noexcept {
    if (ImGui::GetCurrentContext() != nullptr) {
        const ImVec2 size = ImGui::GetIO().DisplaySize;
        if (size.x > 1.0f && size.y > 1.0f) {
            return size;
        }
    }
    return {kReferenceWidth, kReferenceHeight};
}

float display_scale(ImVec2 size) noexcept {
    return std::clamp(std::min(size.x / kReferenceWidth, size.y / kReferenceHeight), 0.85f, 1.8f);
}

float control_radius(const TouchControl& control, ImVec2 size) noexcept {
    return control.radius * control.scale * getSettings().game.touchControlsScale.getValue() *
           display_scale(size);
}

ImVec2 control_center(const TouchControl& control, ImVec2 size) noexcept {
    return {control.x * size.x, control.y * size.y};
}

float distance_squared(ImVec2 a, ImVec2 b) noexcept {
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    return dx * dx + dy * dy;
}

int find_control_at(float x, float y, bool editing) noexcept {
    const ImVec2 size = display_size();
    const ImVec2 pos{x * size.x, y * size.y};
    int best = -1;
    float bestDistance = std::numeric_limits<float>::max();

    for (int i = static_cast<int>(sControls.size()) - 1; i >= 0; --i) {
        const auto& control = sControls[static_cast<size_t>(i)];
        const float radius = control_radius(control, size) * (editing ? 2.1f : 1.25f);
        const float dist = distance_squared(pos, control_center(control, size));
        if (dist <= radius * radius && dist < bestDistance) {
            best = i;
            bestDistance = dist;
        }
    }

    return best;
}

ActiveTouch* find_touch(SDL_FingerID fingerId) noexcept {
    for (auto& touch : sTouches) {
        if (touch.active && touch.fingerId == fingerId) {
            return &touch;
        }
    }
    return nullptr;
}

ActiveTouch* free_touch() noexcept {
    for (auto& touch : sTouches) {
        if (!touch.active) {
            return &touch;
        }
    }
    return nullptr;
}

bool control_is_active(ControlId id) noexcept {
    for (const auto& touch : sTouches) {
        if (!touch.active || touch.editing || touch.controlIndex < 0) {
            continue;
        }
        if (sControls[static_cast<size_t>(touch.controlIndex)].id == id) {
            return true;
        }
    }
    return false;
}

s16 stick_angle(float x, float y) noexcept {
    if (x == 0.0f && y == 0.0f) {
        return 0;
    }
    if (y == 0.0f) {
        return x > 0.0f ? 0x4000 : static_cast<s16>(-0x4000);
    }
    return static_cast<s16>(std::atan2(x, -y) * 32768.0f / kPi);
}

StickState stick_state_for(const TouchControl& control, const ActiveTouch& touch) noexcept {
    const ImVec2 size = display_size();
    const ImVec2 center = control_center(control, size);
    const float radius = std::max(control_radius(control, size), 1.0f);
    const float px = touch.x * size.x;
    const float py = touch.y * size.y;
    float x = (px - center.x) / radius;
    float y = (center.y - py) / radius;
    const float magnitude = std::sqrt(x * x + y * y);
    if (magnitude > 1.0f) {
        x /= magnitude;
        y /= magnitude;
    }

    const float value = std::min(1.0f, std::sqrt(x * x + y * y));
    return {
        .x = x,
        .y = y,
        .value = value,
        .angle = stick_angle(x, y),
    };
}

void recompute_state() noexcept {
    sHeldButtons = 0;
    sMainStick = {};
    sCStick = {};

    for (const auto& touch : sTouches) {
        if (!touch.active || touch.editing || touch.controlIndex < 0 ||
            touch.controlIndex >= static_cast<int>(sControls.size()))
        {
            continue;
        }

        const auto& control = sControls[static_cast<size_t>(touch.controlIndex)];
        if (control.id == ControlId::LeftStick) {
            sMainStick = stick_state_for(control, touch);
            continue;
        }
        if (control.id == ControlId::CStick) {
            sCStick = stick_state_for(control, touch);
            continue;
        }

        sHeldButtons |= button_mask(control.id);
    }
}

json layout_to_json() {
    json root;
    root["version"] = 1;
    root["controls"] = json::array();
    for (const auto& control : sControls) {
        root["controls"].push_back({
            {"id", id_name(control.id)},
            {"x", control.x},
            {"y", control.y},
            {"scale", control.scale},
        });
    }
    return root;
}

bool apply_layout_json(const json& root) noexcept {
    if (!root.is_object()) {
        return false;
    }

    try {
        const auto controls = root.find("controls");
        if (controls == root.end() || !controls->is_array()) {
            return false;
        }

        bool applied = false;
        for (const auto& item : *controls) {
            if (!item.is_object()) {
                continue;
            }
            const auto idText = item.value("id", std::string{});
            ControlId id{};
            if (!parse_id(idText, id)) {
                continue;
            }
            auto found = std::find_if(sControls.begin(), sControls.end(),
                [id](const TouchControl& control) { return control.id == id; });
            if (found == sControls.end()) {
                continue;
            }
            found->x = std::clamp(item.value("x", found->x), 0.02f, 0.98f);
            found->y = std::clamp(item.value("y", found->y), 0.02f, 0.98f);
            found->scale = std::clamp(item.value("scale", found->scale), 0.5f, 1.8f);
            applied = true;
        }
        return applied;
    } catch (...) {
        return false;
    }
}

void ensure_loaded() noexcept {
    if (sLoaded) {
        return;
    }

    sControls = default_layout(getSettings().game.touchControlsPreset.getValue());
    sLoaded = true;

    const auto path = layout_path();
    if (path.empty()) {
        return;
    }

    try {
        const auto data = io::FileStream::ReadAllBytes(path);
        const json root = json::parse(data);
        apply_layout_json(root);
    } catch (...) {
    }
}

ImU32 color(int r, int g, int b, float alphaScale = 1.0f) noexcept {
    const float opacity = getSettings().game.touchControlsOpacity.getValue();
    const int alpha = std::clamp(static_cast<int>(255.0f * opacity * alphaScale), 0, 255);
    return IM_COL32(r, g, b, alpha);
}

void draw_label(ImDrawList* dl, ImVec2 center, const char* label, ImU32 textColor) noexcept {
    const ImVec2 textSize = ImGui::CalcTextSize(label);
    dl->AddText({center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f}, textColor, label);
}

void draw_control(ImDrawList* dl, const TouchControl& control, bool editing) noexcept {
    const ImVec2 size = display_size();
    const ImVec2 center = control_center(control, size);
    const float radius = control_radius(control, size);
    const bool active = control_is_active(control.id);

    const ImU32 baseFill = active ? color(255, 255, 255, 0.55f) : color(35, 38, 42, 0.55f);
    const ImU32 baseStroke = editing ? color(255, 216, 96, 0.95f) : color(255, 255, 255, 0.75f);
    const ImU32 labelColor = color(255, 255, 255, 0.95f);

    if (control.stick) {
        dl->AddCircleFilled(center, radius, color(20, 24, 28, 0.5f), 48);
        dl->AddCircle(center, radius, baseStroke, 48, 2.0f);

        const StickState stick =
            control.id == ControlId::LeftStick ? sMainStick : sCStick;
        const ImVec2 knob{
            center.x + stick.x * radius * 0.55f,
            center.y - stick.y * radius * 0.55f,
        };
        dl->AddCircleFilled(knob, radius * 0.42f, active ? color(255, 255, 255, 0.8f) : baseFill,
            32);
        draw_label(dl, center, control.label, labelColor);
        return;
    }

    dl->AddCircleFilled(center, radius, baseFill, 36);
    dl->AddCircle(center, radius, baseStroke, 36, editing ? 2.5f : 1.6f);
    draw_label(dl, center, control.label, labelColor);
}

}  // namespace

bool HandleEvent(const SDL_Event& event) noexcept {
    const bool touchEvent = event.type == SDL_EVENT_FINGER_DOWN ||
                            event.type == SDL_EVENT_FINGER_MOTION ||
                            event.type == SDL_EVENT_FINGER_UP ||
                            event.type == SDL_EVENT_FINGER_CANCELED;
    if (!touchEvent) {
        return false;
    }

    ensure_loaded();

    const bool editMode = getSettings().game.touchControlsEditMode.getValue();
    if (!getSettings().game.enableTouchControls.getValue() && !editMode) {
        ResetInputState();
        return false;
    }

    switch (event.type) {
    case SDL_EVENT_FINGER_DOWN: {
        const int controlIndex = find_control_at(event.tfinger.x, event.tfinger.y, editMode);
        if (controlIndex < 0) {
            return false;
        }

        auto* touch = free_touch();
        if (touch == nullptr) {
            return false;
        }
        *touch = {
            .fingerId = event.tfinger.fingerID,
            .controlIndex = controlIndex,
            .x = event.tfinger.x,
            .y = event.tfinger.y,
            .editing = editMode && controlIndex >= 0,
            .active = true,
        };
        if (touch->editing) {
            auto& control = sControls[static_cast<size_t>(controlIndex)];
            control.x = std::clamp(touch->x, 0.02f, 0.98f);
            control.y = std::clamp(touch->y, 0.02f, 0.98f);
        }
        recompute_state();
        return controlIndex >= 0;
    }
    case SDL_EVENT_FINGER_MOTION: {
        auto* touch = find_touch(event.tfinger.fingerID);
        if (touch == nullptr) {
            return false;
        }
        touch->x = event.tfinger.x;
        touch->y = event.tfinger.y;
        if (touch->editing && touch->controlIndex >= 0) {
            auto& control = sControls[static_cast<size_t>(touch->controlIndex)];
            control.x = std::clamp(touch->x, 0.02f, 0.98f);
            control.y = std::clamp(touch->y, 0.02f, 0.98f);
        }
        recompute_state();
        return true;
    }
    case SDL_EVENT_FINGER_UP:
    case SDL_EVENT_FINGER_CANCELED: {
        auto* touch = find_touch(event.tfinger.fingerID);
        if (touch == nullptr) {
            return false;
        }
        const bool edited = touch->editing;
        *touch = {};
        recompute_state();
        if (edited) {
            SaveLayout();
        }
        return true;
    }
    default:
        return false;
    }
}

void DrawOverlay() noexcept {
    ensure_loaded();

    const bool editMode = getSettings().game.touchControlsEditMode.getValue();
    if (!getSettings().game.enableTouchControls.getValue() && !editMode) {
        return;
    }
    if (!editMode && ui::any_document_visible()) {
        return;
    }
    if (!editMode && !IsGameLaunched) {
        return;
    }
    if (ImGui::GetCurrentContext() == nullptr) {
        return;
    }

    ImDrawList* dl = ImGui::GetForegroundDrawList();
    for (const auto& control : sControls) {
        draw_control(dl, control, editMode);
    }
}

void MergeToPad(interface_of_controller_pad& pad) noexcept {
    ensure_loaded();

    if (!getSettings().game.enableTouchControls.getValue() || ui::any_document_visible()) {
        sLastMergedButtons = sHeldButtons;
        return;
    }

    const StickState mainStick = sMainStick;
    if (mainStick.value > pad.mMainStickValue) {
        pad.mMainStickPosX = mainStick.x;
        pad.mMainStickPosY = mainStick.y;
        pad.mMainStickValue = mainStick.value;
        pad.mMainStickAngle = mainStick.angle;
    }

    const StickState cStick = sCStick;
    if (cStick.value > pad.mCStickValue) {
        pad.mCStickPosX = cStick.x;
        pad.mCStickPosY = cStick.y;
        pad.mCStickValue = cStick.value;
        pad.mCStickAngle = cStick.angle;
    }

    if ((sHeldButtons & PAD_TRIGGER_L) != 0) {
        pad.mTriggerLeft = std::max(pad.mTriggerLeft, 1.0f);
    }
    if ((sHeldButtons & PAD_TRIGGER_R) != 0) {
        pad.mTriggerRight = std::max(pad.mTriggerRight, 1.0f);
    }

    pad.mPressedButtonFlags |= sHeldButtons & ~sLastMergedButtons;
    pad.mButtonFlags |= sHeldButtons;
    sLastMergedButtons = sHeldButtons;
}

void ResetInputState() noexcept {
    sTouches.fill(ActiveTouch{});
    sHeldButtons = 0;
    sLastMergedButtons = 0;
    sMainStick = {};
    sCStick = {};
}

void ApplyPreset(ControllerOverlayLayout preset) noexcept {
    sControls = default_layout(preset);
    sLoaded = true;
    ResetInputState();
    SaveLayout();
}

void SaveLayout() noexcept {
    ensure_loaded();

    const auto path = layout_path();
    if (path.empty()) {
        return;
    }

    try {
        io::FileStream::WriteAllText(path, layout_to_json().dump(4));
    } catch (...) {
    }
}

nlohmann::json ExportLayout() {
    ensure_loaded();
    return layout_to_json();
}

bool ImportLayout(const nlohmann::json& root) noexcept {
    sControls = default_layout(getSettings().game.touchControlsPreset.getValue());
    sLoaded = true;
    const bool applied = apply_layout_json(root);
    ResetInputState();
    if (applied) {
        SaveLayout();
    }
    return applied;
}

}  // namespace dusk::touch_controls
