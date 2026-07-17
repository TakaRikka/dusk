#pragma once

#include "aurora/lib/input.hpp"

#include <dolphin/pad.h>

#include <cstdint>

namespace dusk::mods::svc {

// Primary player slot (PAD_1). Gamepad Bindings and InputService PAD_1 queries use this Port.
// ADR 0006: assigned controller only — no first-connected fallback when unassigned.
inline constexpr uint32_t kPrimaryPlayerPort = PAD_CHAN0;
static_assert(kPrimaryPlayerPort == 0);

// Assigned controller for `port` with a live SDL gamepad, else nullptr.
// Invalid / unassigned ports and dead assignments all return nullptr.
inline aurora::input::GameController* controller_for_port(const uint32_t port) {
    if (port >= PAD_CHANMAX) {
        return nullptr;
    }
    auto* assigned = aurora::input::get_controller_for_player(port);
    if (assigned == nullptr || assigned->m_controller == nullptr) {
        return nullptr;
    }
    return assigned;
}

inline aurora::input::GameController* primary_player_controller() {
    return controller_for_port(kPrimaryPlayerPort);
}

}  // namespace dusk::mods::svc
