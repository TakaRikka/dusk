#include "dusk/mouse.h"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "dusk/menu_pointer.h"
#include "dusk/settings.h"
#include "dusk/ui/ui.hpp"

#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_video.h>
#include <aurora/lib/window.hpp>
#include <imgui.h>

#include <chrono>

namespace dusk::mouse {
namespace {
using Clock = std::chrono::steady_clock;

constexpr float kMousePixelToRad = 0.0025f;
constexpr auto kCursorIdleDuration = std::chrono::seconds(1);

float s_aim_yaw_rad = 0.0f;
float s_aim_pitch_rad = 0.0f;
float s_camera_yaw_rad = 0.0f;
float s_camera_pitch_rad = 0.0f;
Clock::time_point s_last_cursor_motion = Clock::now();

void reset_deltas() {
    s_aim_yaw_rad = s_aim_pitch_rad = 0.0f;
    s_camera_yaw_rad = s_camera_pitch_rad = 0.0f;
}

bool query_mouse_aim_context() {
    return getSettings().game.enableMouseAim.getValue() && dCamera_c::isAimActive();
}

bool want_mouse_capture() {
    return getSettings().game.enableMouseCamera.getValue() || query_mouse_aim_context();
}

bool mouse_input_enabled() {
    const auto& game = getSettings().game;
    return game.enableMouseAim.getValue() || game.enableMouseCamera.getValue();
}

bool is_window_focused(SDL_Window* window) {
    if (window == nullptr) {
        return false;
    }
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS) != 0;
}

bool imgui_windows_visible() {
    return ImGui::GetIO().MetricsRenderWindows > 0;
}

bool should_capture_mouse(SDL_Window* window) {
    if (window == nullptr || ui::any_document_visible() || imgui_windows_visible() ||
        menu_pointer::active())
    {
        return false;
    }
    return want_mouse_capture() && is_window_focused(window);
}

bool sync_capture_state(SDL_Window* window, bool should_capture) {
    if (window == nullptr) {
        reset_deltas();
        return false;
    }

    const bool was_captured = SDL_GetWindowRelativeMouseMode(window);
    if (was_captured != should_capture) {
        SDL_SetWindowMouseGrab(window, should_capture);
        SDL_SetWindowRelativeMouseMode(window, should_capture);
    }

    const bool is_captured = SDL_GetWindowRelativeMouseMode(window);
    if (is_captured && !was_captured) {
        const AuroraWindowSize sz = aurora::window::get_window_size();
        const float cx = static_cast<float>(sz.width) * 0.5f;
        const float cy = static_cast<float>(sz.height) * 0.5f;
        SDL_WarpMouseInWindow(window, cx, cy);
        float discard_x = 0.0f;
        float discard_y = 0.0f;
        SDL_GetRelativeMouseState(&discard_x, &discard_y);
    }

    if (!is_captured) {
        reset_deltas();
    }

    return is_captured;
}

void accumulate_deltas(float mx_rel, float my_rel, bool camera_active, bool aim_active) {
    const auto& game = getSettings().game;
    const bool mirror_mode = game.enableMirrorMode.getValue();
    const bool invert_y = game.invertMouseY.getValue();

    if (aim_active) {
        const float aimSens = game.mouseAimSensitivity.getValue();
        s_aim_yaw_rad = -mx_rel * kMousePixelToRad * aimSens;
        s_aim_pitch_rad = my_rel * kMousePixelToRad * aimSens;
        s_aim_yaw_rad = mirror_mode ? -s_aim_yaw_rad : s_aim_yaw_rad;
        s_aim_pitch_rad = invert_y ? -s_aim_pitch_rad : s_aim_pitch_rad;
    } else {
        s_aim_yaw_rad = s_aim_pitch_rad = 0.0f;
    }

    if (camera_active) {
        const float camSens = game.mouseCameraSensitivity.getValue();
        s_camera_yaw_rad = -mx_rel * kMousePixelToRad * camSens;
        s_camera_pitch_rad = -my_rel * kMousePixelToRad * camSens;
        s_camera_yaw_rad = mirror_mode ? -s_camera_yaw_rad : s_camera_yaw_rad;
        s_camera_pitch_rad = invert_y ? -s_camera_pitch_rad : s_camera_pitch_rad;
    } else {
        s_camera_yaw_rad = s_camera_pitch_rad = 0.0f;
    }
}

void set_cursor_visible(bool visible) {
    if (visible) {
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
        SDL_ShowCursor();
    } else {
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        SDL_HideCursor();
    }
}

bool cursor_idle() {
    return Clock::now() - s_last_cursor_motion >= kCursorIdleDuration;
}

bool should_show_cursor(bool captured) {
    if (captured) {
        return false;
    }
    if (ui::any_document_visible()) {
        return true;
    }
    if (imgui_windows_visible()) {
        return true;
    }
    if (menu_pointer::enabled() && menu_pointer::active()) {
        return true;
    }
    if (mouse_input_enabled()) {
        return false;
    }
    return !cursor_idle();
}

void update_cursor_visibility(SDL_Window* window, bool captured) {
    if (window == nullptr || !is_window_focused(window)) {
        return;
    }

    set_cursor_visible(should_show_cursor(captured));
}
}  // namespace

void read() {
    SDL_Window* window = aurora::window::get_sdl_window();
    const bool capture_active = sync_capture_state(window, should_capture_mouse(window));
    update_cursor_visibility(window, capture_active);

    if (!capture_active) {
        return;
    }

    const bool aim_active = capture_active && query_mouse_aim_context();
    const bool camera_active = capture_active && getSettings().game.enableMouseCamera;

    float mx_rel = 0.0f;
    float my_rel = 0.0f;
    SDL_GetRelativeMouseState(&mx_rel, &my_rel);
    accumulate_deltas(mx_rel, my_rel, camera_active, aim_active);
}

void get_aim_deltas(float& out_yaw, float& out_pitch) {
    out_yaw = s_aim_yaw_rad;
    out_pitch = s_aim_pitch_rad;
}

void get_camera_deltas(float& out_yaw, float& out_pitch) {
    out_yaw = 0.0f;
    out_pitch = 0.0f;

    if (!getSettings().game.enableMouseCamera) {
        return;
    }

    out_yaw = s_camera_yaw_rad;
    out_pitch = s_camera_pitch_rad;
}

void handle_event(const SDL_Event& event) noexcept {
    switch (event.type) {
    case SDL_EVENT_MOUSE_MOTION:
        s_last_cursor_motion = Clock::now();
        break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        on_focus_lost();
        break;
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        on_focus_gained();
        break;
    }
}

void on_focus_lost() {
    SDL_Window* window = aurora::window::get_sdl_window();
    if (window != nullptr) {
        sync_capture_state(window, false);
    }
    set_cursor_visible(true);
}

void on_focus_gained() {
    SDL_Window* window = aurora::window::get_sdl_window();
    sync_capture_state(window, should_capture_mouse(window));
}
}  // namespace dusk::mouse
