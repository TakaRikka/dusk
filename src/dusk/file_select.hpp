#pragma once

#include <SDL3/SDL_dialog.h>

struct SDL_Window;

namespace dusk {

using FileCallback = void (*)(void* userdata, const char* path, const char* error);

void ShowFileSelect(FileCallback callback, void* userdata, SDL_Window* window,
                    const SDL_DialogFileFilter* filters, int nfilters, const char* default_location,
                    bool allow_many);

}  // namespace dusk
