#include "file_select.hpp"

#include <memory>

#include <SDL3/SDL_dialog.h>
#include <SDL3/SDL_error.h>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if defined(__APPLE__) && TARGET_OS_IOS && !TARGET_OS_MACCATALYST
#define USE_IOS_DIALOG 1
#include "ios/FileSelectDialog.h"
#else
#define USE_IOS_DIALOG 0
#endif

namespace dusk {
namespace {

#if USE_IOS_DIALOG
struct IOSDialogCallbackState {
    FileCallback callback;
    void* userdata;
};

void onIOSDialogFinished(void* userdata, const char* path, const char* error) {
    std::unique_ptr<IOSDialogCallbackState> state(static_cast<IOSDialogCallbackState*>(userdata));

    if (error != nullptr) {
        state->callback(state->userdata, nullptr, error);
        return;
    }

    if (path == nullptr) {
        state->callback(state->userdata, nullptr, nullptr);
        return;
    }

    state->callback(state->userdata, path, nullptr);
}
#else
struct SDLDialogCallbackState {
    FileCallback callback;
    void* userdata;
};

void onSDLDialogFinished(void* userdata, const char* const* filelist, [[maybe_unused]] int filter) {
    std::unique_ptr<SDLDialogCallbackState> state(static_cast<SDLDialogCallbackState*>(userdata));

    if (filelist == nullptr) {
        state->callback(state->userdata, nullptr, SDL_GetError());
        return;
    }

    if (filelist[0] == nullptr) {
        state->callback(state->userdata, nullptr, nullptr);
        return;
    }

    state->callback(state->userdata, filelist[0], nullptr);
}
#endif

}  // namespace

void ShowFileSelect(FileCallback callback, void* userdata, SDL_Window* window,
                    const SDL_DialogFileFilter* filters, int nfilters, const char* default_location,
                    bool allow_many) {
    if (callback == nullptr) {
        return;
    }

#if USE_IOS_DIALOG
    auto state = std::make_unique<IOSDialogCallbackState>();
    state->callback = callback;
    state->userdata = userdata;

    Dusk_iOS_ShowFileSelect(&onIOSDialogFinished, state.release(), window, filters, nfilters,
                            default_location, allow_many);
#else
    auto state = std::make_unique<SDLDialogCallbackState>();
    state->callback = callback;
    state->userdata = userdata;

    SDL_ShowOpenFileDialog(&onSDLDialogFinished, state.release(), window, filters, nfilters,
                           default_location, allow_many);
#endif
}
}  // namespace dusk
