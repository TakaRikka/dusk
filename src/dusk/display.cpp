#include "dusk/display.h"

#include "dusk/config.hpp"
#include "dusk/settings.h"

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <aurora/lib/window.hpp>

#include <algorithm>
#include <cmath>

namespace dusk::display {
namespace {

// Two refresh rates are treated as the same entry when within this tolerance, collapsing near
// duplicates such as 59.94 and 60.0 in the picker.
constexpr float kRefreshEpsilon = 0.5f;

SDL_DisplayID current_display() {
    SDL_Window* window = aurora::window::get_sdl_window();
    if (window == nullptr) {
        return 0;
    }
    return SDL_GetDisplayForWindow(window);
}

}  // namespace

std::vector<Resolution> enumerate_resolutions() {
    std::vector<Resolution> resolutions;
    const SDL_DisplayID display = current_display();
    if (display == 0) {
        return resolutions;
    }

    int count = 0;
    SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(display, &count);
    if (modes == nullptr) {
        return resolutions;
    }
    for (int i = 0; i < count; ++i) {
        const SDL_DisplayMode* mode = modes[i];
        const bool exists = std::any_of(resolutions.begin(), resolutions.end(),
            [&](const Resolution& r) { return r.width == mode->w && r.height == mode->h; });
        if (!exists) {
            resolutions.push_back({mode->w, mode->h});
        }
    }
    SDL_free(modes);

    std::sort(resolutions.begin(), resolutions.end(), [](const Resolution& a, const Resolution& b) {
        if (a.width != b.width) {
            return a.width > b.width;
        }
        return a.height > b.height;
    });
    return resolutions;
}

std::vector<float> enumerate_refresh_rates(int width, int height) {
    std::vector<float> rates;
    const SDL_DisplayID display = current_display();
    if (display == 0) {
        return rates;
    }

    int count = 0;
    SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(display, &count);
    if (modes == nullptr) {
        return rates;
    }
    for (int i = 0; i < count; ++i) {
        const SDL_DisplayMode* mode = modes[i];
        if (mode->w != width || mode->h != height) {
            continue;
        }
        const bool exists = std::any_of(rates.begin(), rates.end(),
            [&](float rate) { return std::fabs(rate - mode->refresh_rate) < kRefreshEpsilon; });
        if (!exists) {
            rates.push_back(mode->refresh_rate);
        }
    }
    SDL_free(modes);

    std::sort(rates.begin(), rates.end(), [](float a, float b) { return a > b; });
    return rates;
}

void apply_fullscreen_mode() {
    SDL_Window* window = aurora::window::get_sdl_window();
    if (window == nullptr) {
        return;
    }

    auto& video = getSettings().video;
    const int width = video.fullscreenWidth.getValue();
    const int height = video.fullscreenHeight.getValue();

    // Desktop / borderless fullscreen: no exclusive mode, output follows the desktop resolution.
    if (width <= 0 || height <= 0) {
        SDL_SetWindowFullscreenMode(window, nullptr);
        return;
    }

    const float requestedRefresh = video.fullscreenRefreshRate.getValue();
    const SDL_DisplayID display = SDL_GetDisplayForWindow(window);

    // SDL_GetFullscreenDisplayModes returns a single allocation owned by SDL; copy the chosen mode
    // out before freeing, since SDL_SetWindowFullscreenMode takes its own copy.
    SDL_DisplayMode chosen{};
    bool found = false;
    int count = 0;
    SDL_DisplayMode** modes =
        display != 0 ? SDL_GetFullscreenDisplayModes(display, &count) : nullptr;
    if (modes != nullptr) {
        for (int i = 0; i < count; ++i) {
            const SDL_DisplayMode* mode = modes[i];
            if (mode->w != width || mode->h != height) {
                continue;
            }
            if (!found) {
                chosen = *mode;
                found = true;
            } else if (requestedRefresh <= 0.0f) {
                // Auto: highest available refresh rate for this resolution.
                if (mode->refresh_rate > chosen.refresh_rate) {
                    chosen = *mode;
                }
            } else if (std::fabs(mode->refresh_rate - requestedRefresh) <
                       std::fabs(chosen.refresh_rate - requestedRefresh))
            {
                chosen = *mode;
            }
        }
        SDL_free(modes);
    }

    if (found) {
        SDL_SetWindowFullscreenMode(window, &chosen);
        return;
    }

    // The saved resolution is no longer available (e.g. a different monitor, or DSR/DLDSR was
    // disabled). Fall back to borderless desktop so the user isn't stranded.
    video.fullscreenWidth.setValue(0);
    video.fullscreenHeight.setValue(0);
    video.fullscreenRefreshRate.setValue(0.0f);
    config::save();
    SDL_SetWindowFullscreenMode(window, nullptr);
}

}  // namespace dusk::display
