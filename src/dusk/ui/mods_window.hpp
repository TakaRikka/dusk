#pragma once

#include "window.hpp"

#include <string>

namespace dusk::ui {

// A dedicated master-detail Mods window: left = the list of mods + a refresh
// button, right = the selected mod's enable toggle, settings, and any custom
// panel it builds. Opened from the title screen and the in-game quick menu.
class ModsWindow : public Window {
public:
    ModsWindow();
    void update() override;

private:
    std::string mFocusedModId;  // mod whose detail pane is shown (for tab_updates)
};

}  // namespace dusk::ui
