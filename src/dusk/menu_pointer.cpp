#include "dusk/menu_pointer.h"

#include "m_Do/m_Do_graphic.h"
#include "d/d_pane_class.h"
#include "dusk/settings.h"

#include <aurora/rmlui.hpp>
#include <dolphin/pad.h>

#include <algorithm>

namespace dusk::menu_pointer {
namespace {
State s_state;
bool s_clickConsumed = false;
Context s_lastContext = Context::None;
Context s_currentContext = Context::None;
u8 s_lastDialogChoice = 0xFF;
u8 s_currentDialogChoice = 0xFF;
bool s_lastDialogChoiceValid = false;
bool s_currentDialogChoiceValid = false;
bool s_lastDialogClicked = false;
bool s_currentDialogClicked = false;
bool s_mouseActive = false;
bool s_mouseButtonCaptured = false;
s32 s_mouseButton = -1;
u32 s_suppressedPadHoldMask = 0;
u32 s_suppressedPadNextReadMask = 0;
Context s_deferredActivationContext = Context::None;
u8 s_deferredActivationTarget = 0xFF;

s32 scancode_from_rml_button(s32 button) noexcept {
    switch (button) {
    case 0:
        return PAD_KEY_MOUSE_LEFT;
    case 1:
        return PAD_KEY_MOUSE_RIGHT;
    case 2:
        return PAD_KEY_MOUSE_MIDDLE;
    default:
        return PAD_KEY_INVALID;
    }
}

bool is_mouse_scancode(s32 scancode) noexcept {
    return scancode >= PAD_KEY_MOUSE_X2 && scancode <= PAD_KEY_MOUSE_LEFT;
}

PADButton pad_button_for_scancode(u32 port, s32 scancode) noexcept {
    u32 count = 0;
    PADKeyButtonBinding* bindings = PADGetKeyButtonBindings(port, &count);
    if (bindings == nullptr) {
        return 0;
    }

    for (u32 i = 0; i < count; ++i) {
        if (bindings[i].scancode == scancode) {
            return bindings[i].padButton;
        }
    }

    return 0;
}

s32 menu_confirm_mouse_scancode() noexcept {
    constexpr u32 port = PAD_CHAN0;
    u32 count = 0;
    PADKeyButtonBinding* bindings = PADGetKeyButtonBindings(port, &count);
    if (bindings == nullptr) {
        return PAD_KEY_MOUSE_LEFT;
    }

    for (u32 i = 0; i < count; ++i) {
        if (bindings[i].padButton == PAD_BUTTON_A && is_mouse_scancode(bindings[i].scancode)) {
            return bindings[i].scancode;
        }
    }

    return pad_button_for_scancode(port, PAD_KEY_MOUSE_LEFT) != 0 ? PAD_KEY_INVALID :
                                                                    PAD_KEY_MOUSE_LEFT;
}

bool mouse_button_is_menu_confirm(s32 button) noexcept {
    const s32 scancode = scancode_from_rml_button(button);
    return scancode != PAD_KEY_INVALID && scancode == menu_confirm_mouse_scancode();
}

void suppress_pad_for_mouse_button(s32 button, bool held) noexcept {
    const s32 scancode = scancode_from_rml_button(button);
    if (scancode == PAD_KEY_INVALID) {
        return;
    }

    const PADButton padButton = pad_button_for_scancode(PAD_CHAN0, scancode);
    if (padButton == 0) {
        return;
    }

    s_suppressedPadNextReadMask |= padButton;
    if (held) {
        s_suppressedPadHoldMask |= padButton;
    } else {
        s_suppressedPadHoldMask &= ~padButton;
    }
}

void set_position_from_rml(f32 x, f32 y) noexcept {
    auto* context = aurora::rmlui::get_context();
    if (context == nullptr) {
        return;
    }

    const auto dimensions = context->GetDimensions();
    const f32 width = std::max(static_cast<f32>(dimensions.x), 1.0f);
    const f32 height = std::max(static_cast<f32>(dimensions.y), 1.0f);

    s_state.x = mDoGph_gInf_c::getMinXF() + x / width * mDoGph_gInf_c::getWidthF();
    s_state.y = mDoGph_gInf_c::getMinYF() + y / height * mDoGph_gInf_c::getHeightF();
    s_state.valid = true;
}

void clear_input_state() noexcept {
    s_state = {};
    s_clickConsumed = false;
    s_lastDialogChoice = 0xFF;
    s_currentDialogChoice = 0xFF;
    s_lastDialogChoiceValid = false;
    s_currentDialogChoiceValid = false;
    s_lastDialogClicked = false;
    s_currentDialogClicked = false;
    s_mouseActive = false;
    s_mouseButtonCaptured = false;
    s_mouseButton = -1;
    s_suppressedPadHoldMask = 0;
    s_suppressedPadNextReadMask = 0;
    s_deferredActivationContext = Context::None;
    s_deferredActivationTarget = 0xFF;
}

}  // namespace

bool handle_fallthrough_pointer(f32 x, f32 y, Phase phase, bool touch, s32 mouseButton) noexcept {
    if (!enabled()) {
        return false;
    }

    s_clickConsumed = false;

    if (!touch) {
        if (phase == Phase::Press) {
            if (!mouse_button_is_menu_confirm(mouseButton)) {
                return false;
            }
            s_mouseButtonCaptured = true;
            s_mouseButton = mouseButton;
            suppress_pad_for_mouse_button(mouseButton, true);
        } else if (phase == Phase::Release) {
            if (!s_mouseButtonCaptured || s_mouseButton != mouseButton) {
                return false;
            }
            suppress_pad_for_mouse_button(mouseButton, false);
            s_mouseButtonCaptured = false;
            s_mouseButton = -1;
        } else if (phase == Phase::Cancel) {
            if (s_mouseButtonCaptured) {
                suppress_pad_for_mouse_button(s_mouseButton, false);
                s_mouseButtonCaptured = false;
                s_mouseButton = -1;
            } else if (!s_mouseActive) {
                return false;
            }
        }
        s_mouseActive = true;
    }

    if (phase != Phase::Cancel) {
        set_position_from_rml(x, y);
    }
    s_state.touch = touch;

    switch (phase) {
    case Phase::Press:
        s_state.down = true;
        s_state.pressed = true;
        break;
    case Phase::Release:
        s_state.down = false;
        s_state.released = true;
        s_state.clicked = true;
        break;
    case Phase::Cancel:
        s_state.down = false;
        break;
    case Phase::Move:
    default:
        break;
    }

    return true;
}

void begin_game_frame() noexcept {
    s_currentContext = Context::None;
    s_currentDialogChoice = 0xFF;
    s_currentDialogChoiceValid = false;
    s_currentDialogClicked = false;
    s_clickConsumed = false;
    if (!enabled()) {
        clear_input_state();
    }
}

void end_game_frame() noexcept {
    s_lastContext = s_currentContext;
    s_lastDialogChoice = s_currentDialogChoice;
    s_lastDialogChoiceValid = s_currentDialogChoiceValid;
    s_lastDialogClicked = s_currentDialogClicked;
    s_state.pressed = false;
    s_state.released = false;
    s_state.clicked = false;
    if (!s_state.down) {
        s_state.valid = false;
    }
    s_clickConsumed = false;
}

void begin_context(Context context) noexcept {
    if (context == Context::None) {
        return;
    }

    if (s_lastContext == Context::None && s_currentContext == Context::None) {
        s_state = {};
        s_mouseActive = false;
        s_mouseButtonCaptured = false;
        s_mouseButton = -1;
        s_suppressedPadHoldMask = 0;
        s_suppressedPadNextReadMask = 0;
        s_deferredActivationContext = Context::None;
        s_deferredActivationTarget = 0xFF;
    }

    s_currentContext = context;
}

bool active() noexcept {
    return s_currentContext != Context::None || s_lastContext != Context::None;
}

bool enabled() noexcept {
    return getSettings().game.enableMenuPointer.getValue();
}

bool mouse_capture_active() noexcept {
    return enabled() && s_mouseButtonCaptured;
}

const State& state() noexcept {
    return s_state;
}

bool consume_click() noexcept {
    if (!s_state.clicked || s_clickConsumed) {
        return false;
    }

    s_clickConsumed = true;
    return true;
}

void set_dialog_choice(u8 choice, bool clicked) noexcept {
    s_currentDialogChoice = choice;
    s_currentDialogChoiceValid = true;
    s_currentDialogClicked = clicked;
}

bool get_dialog_choice(u8& choice) noexcept {
    if (s_currentDialogChoiceValid) {
        choice = s_currentDialogChoice;
        return true;
    }
    if (s_lastDialogChoiceValid) {
        choice = s_lastDialogChoice;
        return true;
    }
    return false;
}

bool consume_dialog_click(u8& choice) noexcept {
    if (s_currentDialogChoiceValid && s_currentDialogClicked) {
        choice = s_currentDialogChoice;
        s_currentDialogClicked = false;
        return true;
    }
    if (s_lastDialogChoiceValid && s_lastDialogClicked) {
        choice = s_lastDialogChoice;
        s_lastDialogClicked = false;
        return true;
    }
    return false;
}

void defer_activation(Context context, u8 target) noexcept {
    s_deferredActivationContext = context;
    s_deferredActivationTarget = target;
}

bool consume_deferred_activation(Context context, u8 target) noexcept {
    if (s_deferredActivationContext != context || s_deferredActivationTarget != target) {
        return false;
    }

    s_deferredActivationContext = Context::None;
    s_deferredActivationTarget = 0xFF;
    return true;
}

void clear_deferred_activation(Context context) noexcept {
    if (s_deferredActivationContext != context) {
        return;
    }

    s_deferredActivationContext = Context::None;
    s_deferredActivationTarget = 0xFF;
}

u32 suppressed_pad_buttons(u32 port) noexcept {
    if (port != PAD_CHAN0) {
        return 0;
    }

    return s_suppressedPadHoldMask | s_suppressedPadNextReadMask;
}

void finish_pad_suppression_read(u32 port) noexcept {
    if (port != PAD_CHAN0) {
        return;
    }

    s_suppressedPadNextReadMask = 0;
}

bool hit_rect(f32 left, f32 top, f32 right, f32 bottom, f32 padding) noexcept {
    const auto& state = menu_pointer::state();
    if (!state.valid) {
        return false;
    }

    if (left > right) {
        std::swap(left, right);
    }
    if (top > bottom) {
        std::swap(top, bottom);
    }

    return state.x >= left - padding && state.x <= right + padding && state.y >= top - padding &&
           state.y <= bottom + padding;
}

bool hit_pane(CPaneMgr* pane, f32 padding) noexcept {
    if (pane == nullptr || pane->getPanePtr() == nullptr) {
        return false;
    }

    Mtx mtx;
    Vec v0 = pane->getGlobalVtx(&mtx, 0, false, 0);
    Vec v1 = pane->getGlobalVtx(&mtx, 1, false, 0);
    Vec v2 = pane->getGlobalVtx(&mtx, 2, false, 0);
    Vec v3 = pane->getGlobalVtx(&mtx, 3, false, 0);
    const f32 left = std::min({v0.x, v1.x, v2.x, v3.x});
    const f32 right = std::max({v0.x, v1.x, v2.x, v3.x});
    const f32 top = std::min({v0.y, v1.y, v2.y, v3.y});
    const f32 bottom = std::max({v0.y, v1.y, v2.y, v3.y});
    return hit_rect(left, top, right, bottom, padding);
}

bool hit_pane(J2DPane* pane, f32 padding) noexcept {
    if (pane == nullptr || !pane->isVisible()) {
        return false;
    }

    const JGeometry::TBox2<f32>& bounds = pane->getBounds();
    return hit_rect(bounds.i.x, bounds.i.y, bounds.f.x, bounds.f.y, padding);
}

}  // namespace dusk::menu_pointer
