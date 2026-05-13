#include "TouchControls.hpp"

#include <SDL3/SDL_events.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <system_error>

#include "dusk/imgui/ImGuiEngine.hpp"
#include "dusk/io.hpp"
#include "dusk/main.h"
#include "dusk/ui/ui.hpp"
#include "imgui.h"

namespace dusk::ios::touch_controls {
namespace {

constexpr float kStickRadius = 74.0f;
constexpr float kButtonRadius = 34.0f;
constexpr float kSmallButtonRadius = 25.0f;
constexpr float kShoulderWidth = 118.0f;
constexpr float kShoulderHeight = 44.0f;
constexpr float kDeadZone = 0.12f;
constexpr float kAlphaIdle = 0.34f;
constexpr float kAlphaActive = 0.66f;
constexpr int kDefaultStickSensitivityPercent = 500;
constexpr int kMinStickSensitivityPercent = 50;
constexpr int kMaxStickSensitivityPercent = 2000;
constexpr std::uint32_t kSettingsMagic = 0x54434831;  // TCH1
constexpr std::uint32_t kSettingsVersion = 1;
constexpr auto kSettingsFileName = "touch_controls.dat";

enum class Control {
    None,
    MainStick,
    CStick,
    DPad,
    A,
    B,
    X,
    Y,
    L,
    R,
    Z,
    Start,
};

struct FingerState {
    SDL_FingerID id = 0;
    Control control = Control::None;
    ImVec2 start{};
    ImVec2 current{};
    bool active = false;
};

struct Layout {
    ImVec2 display{};
    float scale = 1.0f;
    ImVec2 mainStick{};
    ImVec2 cStick{};
    ImVec2 dpad{};
    ImVec2 a{};
    ImVec2 b{};
    ImVec2 x{};
    ImVec2 y{};
    ImVec2 lMin{};
    ImVec2 lMax{};
    ImVec2 rMin{};
    ImVec2 rMax{};
    ImVec2 z{};
    ImVec2 start{};
};

std::array<FingerState, 10> sFingers;
std::array<bool, PAD_MAX_CONTROLLERS> sEnabledPorts{};
bool sSettingsLoaded = false;
int sStickSensitivityPercent = kDefaultStickSensitivityPercent;

float scaled(float value, const Layout& layout) noexcept {
    return value * layout.scale;
}

float distance(ImVec2 a, ImVec2 b) noexcept {
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

ImVec2 clamp_vector(ImVec2 value, float radius) noexcept {
    const float length = std::sqrt(value.x * value.x + value.y * value.y);
    if (length <= radius || length <= 0.0f) {
        return value;
    }
    const float scale = radius / length;
    return ImVec2(value.x * scale, value.y * scale);
}

std::filesystem::path settings_path() {
    if (dusk::ConfigPath.empty()) {
        return {};
    }
    return dusk::ConfigPath / kSettingsFileName;
}

void load_settings() noexcept {
    if (sSettingsLoaded) {
        return;
    }

    sSettingsLoaded = true;
    sEnabledPorts.fill(false);
    sEnabledPorts[PAD_CHAN0] = true;
    sStickSensitivityPercent = kDefaultStickSensitivityPercent;

    const auto path = settings_path();
    std::error_code ec;
    if (path.empty() || !std::filesystem::exists(path, ec)) {
        return;
    }

    try {
        auto bytes = dusk::io::FileStream::ReadAllBytes(path);
        if (bytes.size() < 8) {
            return;
        }

        const auto read_u32 = [&bytes](std::size_t offset) {
            return static_cast<std::uint32_t>(bytes[offset]) |
                   (static_cast<std::uint32_t>(bytes[offset + 1]) << 8) |
                   (static_cast<std::uint32_t>(bytes[offset + 2]) << 16) |
                   (static_cast<std::uint32_t>(bytes[offset + 3]) << 24);
        };

        if (read_u32(0) != kSettingsMagic || read_u32(4) != kSettingsVersion) {
            return;
        }

        for (std::size_t i = 0; i < sEnabledPorts.size(); ++i) {
            const std::size_t offset = 8 + i;
            sEnabledPorts[i] = offset < bytes.size() && bytes[offset] != 0;
        }
        if (bytes.size() >= 14) {
            const int sensitivity = static_cast<int>(bytes[12]) |
                                    (static_cast<int>(bytes[13]) << 8);
            sStickSensitivityPercent = std::clamp(
                sensitivity, kMinStickSensitivityPercent, kMaxStickSensitivityPercent);
        }
    } catch (...) {
        sEnabledPorts.fill(false);
        sEnabledPorts[PAD_CHAN0] = true;
        sStickSensitivityPercent = kDefaultStickSensitivityPercent;
    }
}

void save_settings() noexcept {
    const auto path = settings_path();
    if (path.empty()) {
        return;
    }

    try {
        std::error_code ec;
        std::filesystem::create_directories(path.parent_path(), ec);
        std::array<std::uint8_t, 14> bytes{};
        const auto write_u32 = [&bytes](std::size_t offset, std::uint32_t value) {
            bytes[offset] = static_cast<std::uint8_t>(value & 0xff);
            bytes[offset + 1] = static_cast<std::uint8_t>((value >> 8) & 0xff);
            bytes[offset + 2] = static_cast<std::uint8_t>((value >> 16) & 0xff);
            bytes[offset + 3] = static_cast<std::uint8_t>((value >> 24) & 0xff);
        };
        write_u32(0, kSettingsMagic);
        write_u32(4, kSettingsVersion);
        for (std::size_t i = 0; i < sEnabledPorts.size(); ++i) {
            bytes[8 + i] = sEnabledPorts[i] ? 1 : 0;
        }
        const auto sensitivity = static_cast<std::uint16_t>(std::clamp(
            sStickSensitivityPercent, kMinStickSensitivityPercent, kMaxStickSensitivityPercent));
        bytes[12] = static_cast<std::uint8_t>(sensitivity & 0xff);
        bytes[13] = static_cast<std::uint8_t>((sensitivity >> 8) & 0xff);
        auto stream = dusk::io::FileStream::Create(path);
        stream.Write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    } catch (...) {
    }
}

Layout make_layout() noexcept {
    if (ImGui::GetCurrentContext() == nullptr) {
        return {};
    }

    const ImGuiIO& io = ImGui::GetIO();
    Layout layout{};
    layout.display = io.DisplaySize;
    layout.scale = std::clamp(std::min(layout.display.x / 844.0f, layout.display.y / 390.0f), 0.72f, 1.35f);

    const float safeBottom = ImGui::GetStyle().DisplaySafeAreaPadding.y;
    const float bottom = layout.display.y - std::max(safeBottom, scaled(26.0f, layout));
    const float left = scaled(112.0f, layout);
    const float right = layout.display.x - scaled(118.0f, layout);

    layout.mainStick = ImVec2(left, bottom - scaled(82.0f, layout));
    layout.dpad = ImVec2(left + scaled(150.0f, layout), bottom - scaled(62.0f, layout));
    layout.cStick = ImVec2(right - scaled(170.0f, layout), bottom - scaled(58.0f, layout));

    layout.a = ImVec2(right, bottom - scaled(108.0f, layout));
    layout.b = ImVec2(right - scaled(78.0f, layout), bottom - scaled(60.0f, layout));
    layout.x = ImVec2(right + scaled(74.0f, layout), bottom - scaled(108.0f, layout));
    layout.y = ImVec2(right - scaled(6.0f, layout), bottom - scaled(182.0f, layout));

    layout.lMin = ImVec2(scaled(18.0f, layout), scaled(18.0f, layout));
    layout.lMax = ImVec2(layout.lMin.x + scaled(kShoulderWidth, layout), layout.lMin.y + scaled(kShoulderHeight, layout));
    layout.rMax = ImVec2(layout.display.x - scaled(18.0f, layout), scaled(18.0f, layout) + scaled(kShoulderHeight, layout));
    layout.rMin = ImVec2(layout.rMax.x - scaled(kShoulderWidth, layout), scaled(18.0f, layout));
    layout.z = ImVec2(layout.display.x - scaled(204.0f, layout), scaled(41.0f, layout));
    layout.start = ImVec2(layout.display.x * 0.5f, bottom - scaled(30.0f, layout));
    return layout;
}

ImVec2 event_position(const SDL_TouchFingerEvent& event) noexcept {
    const ImVec2 display = ImGui::GetIO().DisplaySize;
    return ImVec2(event.x * display.x, event.y * display.y);
}

bool in_rect(ImVec2 pos, ImVec2 min, ImVec2 max) noexcept {
    return pos.x >= min.x && pos.x <= max.x && pos.y >= min.y && pos.y <= max.y;
}

Control hit_test(ImVec2 pos, const Layout& layout) noexcept {
    if (in_rect(pos, layout.lMin, layout.lMax)) {
        return Control::L;
    }
    if (in_rect(pos, layout.rMin, layout.rMax)) {
        return Control::R;
    }
    if (distance(pos, layout.z) <= scaled(kSmallButtonRadius, layout)) {
        return Control::Z;
    }
    if (distance(pos, layout.start) <= scaled(kSmallButtonRadius, layout)) {
        return Control::Start;
    }
    if (distance(pos, layout.mainStick) <= scaled(kStickRadius * 1.25f, layout)) {
        return Control::MainStick;
    }
    if (distance(pos, layout.cStick) <= scaled(kStickRadius, layout)) {
        return Control::CStick;
    }
    if (distance(pos, layout.dpad) <= scaled(56.0f, layout)) {
        return Control::DPad;
    }
    if (distance(pos, layout.a) <= scaled(kButtonRadius * 1.25f, layout)) {
        return Control::A;
    }
    if (distance(pos, layout.b) <= scaled(kButtonRadius, layout)) {
        return Control::B;
    }
    if (distance(pos, layout.x) <= scaled(kButtonRadius, layout)) {
        return Control::X;
    }
    if (distance(pos, layout.y) <= scaled(kButtonRadius, layout)) {
        return Control::Y;
    }
    return Control::None;
}

FingerState* find_finger(SDL_FingerID id) noexcept {
    for (auto& finger : sFingers) {
        if (finger.active && finger.id == id) {
            return &finger;
        }
    }
    return nullptr;
}

FingerState* free_finger() noexcept {
    for (auto& finger : sFingers) {
        if (!finger.active) {
            return &finger;
        }
    }
    return nullptr;
}

bool control_held(Control control) noexcept {
    return std::any_of(sFingers.begin(), sFingers.end(), [control](const FingerState& finger) {
        return finger.active && finger.control == control;
    });
}

ImVec2 stick_value(Control control, const Layout& layout) noexcept {
    for (const auto& finger : sFingers) {
        if (!finger.active || finger.control != control) {
            continue;
        }

        const ImVec2 layoutCenter = control == Control::MainStick ? layout.mainStick : layout.cStick;
        const ImVec2 center = finger.start;
        const float sensitivityScale = 100.0f / static_cast<float>(stick_sensitivity_percent());
        const float fullTiltRadius = scaled(kStickRadius * std::clamp(sensitivityScale, 0.05f, 1.20f), layout);
        ImVec2 value = clamp_vector(ImVec2(finger.current.x - center.x, finger.current.y - center.y),
            fullTiltRadius);
        value.x /= fullTiltRadius;
        value.y /= fullTiltRadius;
        if (std::sqrt(value.x * value.x + value.y * value.y) < kDeadZone) {
            return ImVec2(0.0f, 0.0f);
        }
        if (distance(finger.start, layoutCenter) <= scaled(kStickRadius * 0.35f, layout) &&
            distance(finger.current, layoutCenter) > scaled(kStickRadius * 0.35f, layout))
        {
            value = clamp_vector(ImVec2(finger.current.x - layoutCenter.x, finger.current.y - layoutCenter.y),
                fullTiltRadius);
            value.x /= fullTiltRadius;
            value.y /= fullTiltRadius;
        }
        return value;
    }
    return ImVec2(0.0f, 0.0f);
}

ImVec2 dpad_value(const Layout& layout) noexcept {
    for (const auto& finger : sFingers) {
        if (!finger.active || finger.control != Control::DPad) {
            continue;
        }

        ImVec2 value = ImVec2(finger.current.x - layout.dpad.x, finger.current.y - layout.dpad.y);
        const float absX = std::abs(value.x);
        const float absY = std::abs(value.y);
        if (std::max(absX, absY) < scaled(14.0f, layout)) {
            return ImVec2(0.0f, 0.0f);
        }
        if (absX > absY) {
            return ImVec2(value.x > 0.0f ? 1.0f : -1.0f, 0.0f);
        }
        return ImVec2(0.0f, value.y > 0.0f ? 1.0f : -1.0f);
    }
    return ImVec2(0.0f, 0.0f);
}

s8 axis_to_pad(float value) noexcept {
    return static_cast<s8>(std::clamp(std::lround(value * 127.0f), -127l, 127l));
}

void draw_circle_button(ImDrawList* drawList, ImVec2 center, float radius, const char* label, bool active, const Layout& layout) noexcept {
    const ImU32 fill = ImGui::GetColorU32(ImVec4(0.05f, 0.06f, 0.07f, active ? kAlphaActive : kAlphaIdle));
    const ImU32 edge = ImGui::GetColorU32(ImVec4(0.95f, 0.97f, 1.0f, active ? 0.82f : 0.46f));
    drawList->AddCircleFilled(center, scaled(radius, layout), fill, 40);
    drawList->AddCircle(center, scaled(radius, layout), edge, 40, scaled(2.0f, layout));
    const ImVec2 textSize = ImGui::CalcTextSize(label);
    drawList->AddText(ImVec2(center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f), edge, label);
}

void draw_stick(ImDrawList* drawList, ImVec2 center, Control control, const Layout& layout) noexcept {
    const bool active = control_held(control);
    const ImVec2 value = stick_value(control, layout);
    const ImVec2 knob(center.x + value.x * scaled(kStickRadius * 0.55f, layout),
        center.y + value.y * scaled(kStickRadius * 0.55f, layout));
    const ImU32 fill = ImGui::GetColorU32(ImVec4(0.02f, 0.03f, 0.035f, active ? 0.50f : 0.26f));
    const ImU32 edge = ImGui::GetColorU32(ImVec4(0.95f, 0.97f, 1.0f, active ? 0.72f : 0.36f));
    drawList->AddCircleFilled(center, scaled(kStickRadius, layout), fill, 48);
    drawList->AddCircle(center, scaled(kStickRadius, layout), edge, 48, scaled(2.0f, layout));
    drawList->AddCircleFilled(knob, scaled(28.0f, layout), ImGui::GetColorU32(ImVec4(0.9f, 0.92f, 0.96f, active ? 0.46f : 0.24f)), 32);
}

void draw_dpad(ImDrawList* drawList, const Layout& layout) noexcept {
    const bool active = control_held(Control::DPad);
    const ImU32 fill = ImGui::GetColorU32(ImVec4(0.04f, 0.05f, 0.06f, active ? 0.58f : 0.30f));
    const ImU32 edge = ImGui::GetColorU32(ImVec4(0.95f, 0.97f, 1.0f, active ? 0.75f : 0.38f));
    const float arm = scaled(22.0f, layout);
    const float len = scaled(58.0f, layout);
    const ImVec2 c = layout.dpad;
    drawList->AddRectFilled(ImVec2(c.x - arm, c.y - len), ImVec2(c.x + arm, c.y + len), fill, scaled(8.0f, layout));
    drawList->AddRectFilled(ImVec2(c.x - len, c.y - arm), ImVec2(c.x + len, c.y + arm), fill, scaled(8.0f, layout));
    drawList->AddRect(ImVec2(c.x - arm, c.y - len), ImVec2(c.x + arm, c.y + len), edge, scaled(8.0f, layout), 0, scaled(2.0f, layout));
    drawList->AddRect(ImVec2(c.x - len, c.y - arm), ImVec2(c.x + len, c.y + arm), edge, scaled(8.0f, layout), 0, scaled(2.0f, layout));
}

void draw_shoulder(ImDrawList* drawList, ImVec2 min, ImVec2 max, const char* label, bool active, const Layout& layout) noexcept {
    const ImU32 fill = ImGui::GetColorU32(ImVec4(0.04f, 0.05f, 0.06f, active ? 0.58f : 0.28f));
    const ImU32 edge = ImGui::GetColorU32(ImVec4(0.95f, 0.97f, 1.0f, active ? 0.78f : 0.40f));
    drawList->AddRectFilled(min, max, fill, scaled(14.0f, layout));
    drawList->AddRect(min, max, edge, scaled(14.0f, layout), 0, scaled(2.0f, layout));
    const ImVec2 textSize = ImGui::CalcTextSize(label);
    drawList->AddText(ImVec2((min.x + max.x - textSize.x) * 0.5f, (min.y + max.y - textSize.y) * 0.5f), edge, label);
}

}  // namespace

void handle_event(const SDL_Event& event) noexcept {
    if (!enabled_for_port(PAD_CHAN0)) {
        reset();
        return;
    }

    if (!dusk::IsGameLaunched || dusk::ui::any_document_visible() ||
        ImGui::GetCurrentContext() == nullptr)
    {
        reset();
        return;
    }

    switch (event.type) {
    case SDL_EVENT_FINGER_DOWN: {
        const Layout layout = make_layout();
        const ImVec2 position = event_position(event.tfinger);
        const Control control = hit_test(position, layout);
        if (control == Control::None) {
            return;
        }

        if (auto* finger = free_finger()) {
            *finger = FingerState{
                .id = event.tfinger.fingerID,
                .control = control,
                .start = position,
                .current = position,
                .active = true,
            };
        }
        break;
    }
    case SDL_EVENT_FINGER_MOTION:
        if (auto* finger = find_finger(event.tfinger.fingerID)) {
            finger->current = event_position(event.tfinger);
        }
        break;
    case SDL_EVENT_FINGER_UP:
    case SDL_EVENT_FINGER_CANCELED:
        if (auto* finger = find_finger(event.tfinger.fingerID)) {
            *finger = {};
        }
        break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        reset();
        break;
    default:
        break;
    }
}

void apply_pad_state(PADStatus& status, u32 port) noexcept {
    if (!enabled_for_port(port)) {
        return;
    }

    if (!dusk::IsGameLaunched || ImGui::GetCurrentContext() == nullptr) {
        return;
    }

    const Layout layout = make_layout();
    const ImVec2 mainStick = stick_value(Control::MainStick, layout);
    const ImVec2 cStick = stick_value(Control::CStick, layout);
    const ImVec2 dpad = dpad_value(layout);

    status.err = PAD_ERR_NONE;
    status.button |= control_held(Control::A) ? PAD_BUTTON_A : 0;
    status.button |= control_held(Control::B) ? PAD_BUTTON_B : 0;
    status.button |= control_held(Control::X) ? PAD_BUTTON_X : 0;
    status.button |= control_held(Control::Y) ? PAD_BUTTON_Y : 0;
    status.button |= control_held(Control::Z) ? PAD_TRIGGER_Z : 0;
    status.button |= control_held(Control::Start) ? PAD_BUTTON_START : 0;
    status.button |= dpad.x < 0.0f ? PAD_BUTTON_LEFT : 0;
    status.button |= dpad.x > 0.0f ? PAD_BUTTON_RIGHT : 0;
    status.button |= dpad.y < 0.0f ? PAD_BUTTON_UP : 0;
    status.button |= dpad.y > 0.0f ? PAD_BUTTON_DOWN : 0;

    if (control_held(Control::L)) {
        status.button |= PAD_TRIGGER_L;
        status.triggerLeft = 255;
    }
    if (control_held(Control::R)) {
        status.button |= PAD_TRIGGER_R;
        status.triggerRight = 255;
    }

    if (control_held(Control::MainStick)) {
        status.stickX = axis_to_pad(mainStick.x);
        status.stickY = axis_to_pad(-mainStick.y);
    }
    if (control_held(Control::CStick)) {
        status.substickX = axis_to_pad(cStick.x);
        status.substickY = axis_to_pad(-cStick.y);
    }
}

void draw() noexcept {
    if (!enabled_for_port(PAD_CHAN0)) {
        return;
    }

    if (!dusk::IsGameLaunched || dusk::ui::any_document_visible() ||
        ImGui::GetCurrentContext() == nullptr)
    {
        return;
    }

    const Layout layout = make_layout();
    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    draw_stick(drawList, layout.mainStick, Control::MainStick, layout);
    draw_dpad(drawList, layout);
    draw_stick(drawList, layout.cStick, Control::CStick, layout);
    draw_circle_button(drawList, layout.a, kButtonRadius * 1.18f, "A", control_held(Control::A), layout);
    draw_circle_button(drawList, layout.b, kButtonRadius, "B", control_held(Control::B), layout);
    draw_circle_button(drawList, layout.x, kButtonRadius, "X", control_held(Control::X), layout);
    draw_circle_button(drawList, layout.y, kButtonRadius, "Y", control_held(Control::Y), layout);
    draw_circle_button(drawList, layout.z, kSmallButtonRadius, "Z", control_held(Control::Z), layout);
    draw_circle_button(drawList, layout.start, kSmallButtonRadius, "S", control_held(Control::Start), layout);
    draw_shoulder(drawList, layout.lMin, layout.lMax, "L", control_held(Control::L), layout);
    draw_shoulder(drawList, layout.rMin, layout.rMax, "R", control_held(Control::R), layout);
}

void reset() noexcept {
    for (auto& finger : sFingers) {
        finger = {};
    }
}

bool enabled_for_port(u32 port) noexcept {
    if (port >= sEnabledPorts.size()) {
        return false;
    }
    load_settings();
    return sEnabledPorts[port];
}

void set_enabled_for_port(u32 port, bool enabled) noexcept {
    if (port >= sEnabledPorts.size()) {
        return;
    }

    load_settings();
    if (enabled) {
        sEnabledPorts.fill(false);
    }
    sEnabledPorts[port] = enabled;
    if (!enabled) {
        reset();
    }
    save_settings();
}

int stick_sensitivity_percent() noexcept {
    load_settings();
    return sStickSensitivityPercent;
}

void set_stick_sensitivity_percent(int value) noexcept {
    load_settings();
    sStickSensitivityPercent = std::clamp(
        value, kMinStickSensitivityPercent, kMaxStickSensitivityPercent);
    save_settings();
}

const char* controller_name() noexcept {
    return "Touch Controls";
}

}  // namespace dusk::ios::touch_controls
