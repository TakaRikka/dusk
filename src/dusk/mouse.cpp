#include "dusk/mouse.h"
#include "dusk/gyro.h"
#include "dusk/settings.h"
#include "dusk/ui/ui.hpp"
#include "d/actor/d_a_alink.h"

#include <aurora/lib/window.hpp>
#include <SDL3/SDL_mouse.h>

namespace dusk::mouse {
namespace {
constexpr float kMousePixelToRad = 0.0025f;

bool  s_mouse_enabled  = false;
bool  s_mouse_relative = false;
float s_yaw_rad    = 0.0f;
float s_pitch_rad  = 0.0f;

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

void reset_aim_state() {
    s_mouse_enabled = false;
    s_yaw_rad = s_pitch_rad = 0.0f;
}
}  // namespace

void read() {
    const bool mouse_aim_active = queryMouseAimContext();
    const bool mouse_gyro_active =
        !ui::any_document_visible() && getSettings().game.enableMouseAim && mouse_aim_active;

    SDL_Window* window = aurora::window::get_sdl_window();
    if (window != nullptr && mouse_gyro_active != s_mouse_relative &&
        SDL_SetWindowRelativeMouseMode(window, mouse_gyro_active))
    {
        s_mouse_relative = mouse_gyro_active;
    }

    if (mouse_gyro_active && !s_mouse_enabled && window != nullptr) {
        const AuroraWindowSize sz = aurora::window::get_window_size();
        const float cx = static_cast<float>(sz.width) * 0.5f;
        const float cy = static_cast<float>(sz.height) * 0.5f;
        SDL_WarpMouseInWindow(window, cx, cy);
        float discard_x = 0.0f;
        float discard_y = 0.0f;
        SDL_GetRelativeMouseState(&discard_x, &discard_y);
    }
    s_mouse_enabled = mouse_gyro_active;

    if (!dusk::gyro::get_sensor_keep_alive() && !mouse_aim_active) {
        reset_aim_state();
        return;
    }

    if (getSettings().game.enableMouseAim && !mouse_gyro_active) {
        s_pitch_rad = 0.0f;
        s_yaw_rad = 0.0f;
        return;
    }

    if (!getSettings().game.enableMouseAim) {
        s_pitch_rad = 0.0f;
        s_yaw_rad = 0.0f;
        return;
    }

    float mx_rel = 0.0f;
    float my_rel = 0.0f;
    SDL_GetRelativeMouseState(&mx_rel, &my_rel);

    s_pitch_rad = my_rel * kMousePixelToRad * getSettings().game.mouseSensitivityY;
    s_yaw_rad = -mx_rel * kMousePixelToRad * getSettings().game.mouseSensitivityX;

    s_yaw_rad = getSettings().game.enableMirrorMode ? -s_yaw_rad : s_yaw_rad;
}

void getAimDeltas(float& out_yaw, float& out_pitch) {
    out_yaw = s_yaw_rad;
    out_pitch = s_pitch_rad;
}
}  // namespace dusk::mouse
