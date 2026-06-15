#include "dusk/mouse.h"
#include "dusk/menu_pointer.h"
#include "dusk/settings.h"
#include "dusk/ui/ui.hpp"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"

#include <aurora/lib/window.hpp>
#include <imgui.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_video.h>

namespace dusk::mouse {
namespace {
constexpr float kMousePixelToRad = 0.0025f;
constexpr int kIdleHideFrames = 99; // Approx. 3 seconds with 33ms ticks

float s_aim_yaw_rad      = 0.0f;
float s_aim_pitch_rad    = 0.0f;
float s_camera_yaw_rad   = 0.0f;
float s_camera_pitch_rad = 0.0f;
int s_idle_frames = 0;

void reset_deltas() {
    s_aim_yaw_rad = s_aim_pitch_rad = 0.0f;
    s_camera_yaw_rad = s_camera_pitch_rad = 0.0f;
}

bool queryMouseAimContext() {
    return getSettings().game.enableMouseAim.getValue() && dCamera_c::isAimActive();
}

bool wantMouseCapture() {
    return getSettings().game.enableMouseCamera.getValue() || queryMouseAimContext();
}

bool isWindowFocused(SDL_Window* window) {
    if (window == nullptr) {
        return false;
    }
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS) != 0;
}

bool shouldCaptureMouse(SDL_Window* window) {
    if (window == nullptr || ui::any_document_visible() || menu_pointer::active()) {
        return false;
    }
    return wantMouseCapture() && isWindowFocused(window);
}

bool syncCaptureState(SDL_Window* window, bool should_capture) {
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

void accumulateDeltas(float mx_rel, float my_rel, bool camera_active, bool aim_active) {
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

void update_cursor_visibility(SDL_Window* window, bool captured) {
    if (window == nullptr || !isWindowFocused(window)) {
        return;
    }

    if (captured) {
        s_idle_frames = 0;
        set_cursor_visible(false);
        return;
    }

    const ImGuiIO& io = ImGui::GetIO();
    if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f) {
        s_idle_frames = 0;
        set_cursor_visible(true);
        return;
    }

    if (s_idle_frames < kIdleHideFrames) {
        ++s_idle_frames;
        set_cursor_visible(true);
    } else {
        set_cursor_visible(false);
    }
}
}  // namespace

void read() {
    SDL_Window* window = aurora::window::get_sdl_window();
    const bool capture_active = syncCaptureState(window, shouldCaptureMouse(window));
    update_cursor_visibility(window, capture_active);

    if (!capture_active) {
        return;
    }

    const bool aim_active = capture_active && queryMouseAimContext();
    const bool camera_active = capture_active && getSettings().game.enableMouseCamera;

    float mx_rel = 0.0f;
    float my_rel = 0.0f;
    SDL_GetRelativeMouseState(&mx_rel, &my_rel);
    accumulateDeltas(mx_rel, my_rel, camera_active, aim_active);
}

void getAimDeltas(float& out_yaw, float& out_pitch) {
    out_yaw = s_aim_yaw_rad;
    out_pitch = s_aim_pitch_rad;
}

void getCameraDeltas(float& out_yaw, float& out_pitch) {
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
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        onFocusLost();
        break;
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        onFocusGained();
        break;
    }
}

void onFocusLost() {
    SDL_Window* window = aurora::window::get_sdl_window();
    if (window != nullptr) {
        syncCaptureState(window, false);
    }
    s_idle_frames = 0;
    set_cursor_visible(true);
}

void onFocusGained() {
    SDL_Window* window = aurora::window::get_sdl_window();
    syncCaptureState(window, shouldCaptureMouse(window));
}
}  // namespace dusk::mouse
