#include "dusk/mouse.h"
#include "dusk/settings.h"
#include "dusk/ui/ui.hpp"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"

#include <aurora/lib/window.hpp>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_video.h>

namespace dusk::mouse {
namespace {
constexpr float kMousePixelToRad = 0.0025f;

float s_aim_yaw_rad      = 0.0f;
float s_aim_pitch_rad    = 0.0f;
float s_camera_yaw_rad   = 0.0f;
float s_camera_pitch_rad = 0.0f;

void reset_deltas() {
    s_aim_yaw_rad = s_aim_pitch_rad = 0.0f;
    s_camera_yaw_rad = s_camera_pitch_rad = 0.0f;
}

bool queryMouseAimContext() {
    if (!static_cast<bool>(getSettings().game.enableMouseAim)) {
        return false;
    }

    daAlink_c* link = daAlink_getAlinkActorClass();
    if (link == nullptr) {
        return false;
    }

    return link->checkAimContext() && dComIfGp_checkCameraAttentionStatus(link->field_0x317c, 0x10);
}

bool wantMouseCapture() {
    return (static_cast<bool>(getSettings().game.enableMouseCamera) &&
            static_cast<bool>(getSettings().game.freeCamera)) ||
            static_cast<bool>(getSettings().game.enableMouseAim);
}

bool isWindowFocused(SDL_Window* window) {
    if (window == nullptr) {
        return false;
    }
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS) != 0;
}

bool shouldCaptureMouse(SDL_Window* window) {
    if (window == nullptr) {
        return false;
    }
    if (ui::any_document_visible()) {
        return false;
    }
    return wantMouseCapture() && isWindowFocused(window);
}

bool queryActualCaptureState(SDL_Window* window) {
    if (window == nullptr) {
        return false;
    }
    return SDL_GetWindowRelativeMouseMode(window);
}

bool syncCaptureState(SDL_Window* window, bool should_capture) {
    if (window == nullptr) {
        reset_deltas();
        return false;
    }

    if (!isWindowFocused(window)) {
        should_capture = false;
    }

    const bool was_captured = queryActualCaptureState(window);
    if (was_captured != should_capture) {
        SDL_SetWindowRelativeMouseMode(window, should_capture);
    }

    const bool is_captured = queryActualCaptureState(window);
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

    if (camera_active) {
        s_camera_yaw_rad = -mx_rel * kMousePixelToRad * game.mouseCameraSensitivityX.getValue();
        s_camera_pitch_rad = -my_rel * kMousePixelToRad * game.mouseCameraSensitivityY.getValue();
        s_camera_yaw_rad = game.enableMirrorMode.getValue() ? -s_camera_yaw_rad : s_camera_yaw_rad;
    } else {
        s_camera_yaw_rad = s_camera_pitch_rad = 0.0f;
    }

    if (aim_active) {
        s_aim_yaw_rad = -mx_rel * kMousePixelToRad * game.mouseAimSensitivityX.getValue();
        s_aim_pitch_rad = my_rel * kMousePixelToRad * game.mouseAimSensitivityY.getValue();
        s_aim_yaw_rad = game.enableMirrorMode.getValue() ? -s_aim_yaw_rad : s_aim_yaw_rad;
    } else {
        s_aim_yaw_rad = s_aim_pitch_rad = 0.0f;
    }
}
}  // namespace

void read() {
    SDL_Window* window = aurora::window::get_sdl_window();
    const bool capture_active = syncCaptureState(window, shouldCaptureMouse(window));

    if (!capture_active) {
        return;
    }

    const bool aim_active = capture_active && queryMouseAimContext();
    const bool camera_active = capture_active &&
                               static_cast<bool>(getSettings().game.enableMouseCamera) &&
                               static_cast<bool>(getSettings().game.freeCamera);

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

    if (!static_cast<bool>(getSettings().game.enableMouseCamera) ||
        !static_cast<bool>(getSettings().game.freeCamera))
    {
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
        SDL_SetWindowRelativeMouseMode(window, false);
    }
    SDL_ShowCursor();
    reset_deltas();
}

void onFocusGained() {
    SDL_Window* window = aurora::window::get_sdl_window();
    syncCaptureState(window, shouldCaptureMouse(window));
}
}  // namespace dusk::mouse
