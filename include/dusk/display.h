#pragma once

#include <vector>

namespace dusk::display {

struct Resolution {
    int width;
    int height;
};

// Distinct fullscreen resolutions offered by the display the window is currently on, sorted from
// largest to smallest. Includes driver-injected modes (e.g. NVIDIA DSR/DLDSR) when enabled.
std::vector<Resolution> enumerate_resolutions();

// Refresh rates offered for a given resolution on the current display, sorted descending.
std::vector<float> enumerate_refresh_rates(int width, int height);

// Applies the fullscreen resolution/refresh rate stored in user settings to the window. A width or
// height of 0 selects borderless-desktop fullscreen (the default). If the saved resolution is not
// available on the current display, the setting is reset to Desktop so the user isn't stranded.
void apply_fullscreen_mode();

}  // namespace dusk::display
