#pragma once

#include <stdbool.h>

#include <SDL3/SDL_dialog.h>

struct SDL_Window;

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*IOSFileCallback)(void* userdata, const char* path, const char* error);

void Dusk_iOS_ShowFileSelect(IOSFileCallback callback, void* userdata, SDL_Window* window,
                             const SDL_DialogFileFilter* filters, int nfilters,
                             const char* default_location, bool allow_many);

#ifdef __cplusplus
}
#endif
