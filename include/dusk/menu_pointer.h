#pragma once

#include "dolphin/types.h"

class CPaneMgr;

namespace dusk::menu_pointer {

enum class Context {
    None,
    FileSelect,
    Save,
    ItemWheel,
    Collection,
    Options,
    Dialog,
};

enum class Phase {
    Move,
    Press,
    Release,
    Cancel,
};

struct State {
    f32 x = 0.0f;
    f32 y = 0.0f;
    bool valid = false;
    bool down = false;
    bool pressed = false;
    bool released = false;
    bool clicked = false;
    bool touch = false;
};

void begin_game_frame() noexcept;
void end_game_frame() noexcept;
void begin_context(Context context) noexcept;
bool handle_fallthrough_pointer(f32 x, f32 y, Phase phase, bool touch, s32 mouseButton = -1) noexcept;

bool active() noexcept;
bool enabled() noexcept;
bool mouse_capture_active() noexcept;
const State& state() noexcept;
bool consume_click() noexcept;
void set_dialog_choice(u8 choice, bool clicked) noexcept;
bool get_dialog_choice(u8& choice) noexcept;
bool consume_dialog_click(u8& choice) noexcept;
void defer_activation(Context context, u8 target) noexcept;
bool consume_deferred_activation(Context context, u8 target) noexcept;
void clear_deferred_activation(Context context) noexcept;
u32 suppressed_pad_buttons(u32 port) noexcept;
void finish_pad_suppression_read(u32 port) noexcept;

bool hit_rect(f32 left, f32 top, f32 right, f32 bottom, f32 padding = 0.0f) noexcept;
bool hit_pane(CPaneMgr* pane, f32 padding = 0.0f) noexcept;
bool hit_pane(J2DPane* pane, f32 padding = 0.0f) noexcept;

}  // namespace dusk::menu_pointer
