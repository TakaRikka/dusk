#pragma once

namespace dusk::ui {

enum class NavCommand {
    None,
    Up,
    Down,
    Left,
    Right,
    Next,      // R1
    Previous,  // L1
    Confirm,   // A
    Cancel,    // B
};

}  // namespace dusk::ui
