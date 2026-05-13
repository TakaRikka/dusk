#include "file_select.hpp"

#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_video.h>

#import <AppKit/AppKit.h>

namespace dusk {
namespace {

struct MacOSFolderDialogState {
    FileCallback callback;
    void* userdata;
};

void finish_folder_dialog(MacOSFolderDialogState* state, NSURL* url, const char* error) {
    if (state == nullptr) {
        return;
    }

    if (error != nullptr) {
        state->callback(state->userdata, nullptr, error);
        delete state;
        return;
    }

    if (url == nil) {
        state->callback(state->userdata, nullptr, nullptr);
        delete state;
        return;
    }

    state->callback(state->userdata, [[url path] UTF8String], nullptr);
    delete state;
}

void configure_default_location(NSOpenPanel* panel, const char* defaultLocation) {
    if (panel == nil || defaultLocation == nullptr || defaultLocation[0] == '\0') {
        return;
    }

    NSString* path = [NSString stringWithUTF8String:defaultLocation];
    if (path == nil) {
        return;
    }

    BOOL isDirectory = NO;
    NSFileManager* fileManager = [NSFileManager defaultManager];
    NSURL* url = [NSURL fileURLWithPath:path];
    if ([fileManager fileExistsAtPath:path isDirectory:&isDirectory] && isDirectory) {
        [panel setDirectoryURL:url];
    } else {
        [panel setDirectoryURL:[url URLByDeletingLastPathComponent]];
    }
}

NSWindow* window_for_sdl_window(SDL_Window* window) {
    if (window == nullptr) {
        return nil;
    }

    auto props = SDL_GetWindowProperties(window);
    return (__bridge NSWindow*)SDL_GetPointerProperty(
        props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
}

}  // namespace

bool ShowMacOSFolderSelect(
    FileCallback callback, void* userdata, SDL_Window* window, const char* defaultLocation) {
    if (callback == nullptr) {
        return false;
    }

    auto* state = new MacOSFolderDialogState{
        .callback = callback,
        .userdata = userdata,
    };

    NSOpenPanel* panel = [NSOpenPanel openPanel];
    [panel setCanChooseFiles:NO];
    [panel setCanChooseDirectories:YES];
    [panel setAllowsMultipleSelection:NO];
    [panel setCanCreateDirectories:YES];
    configure_default_location(panel, defaultLocation);

    NSWindow* modalWindow = window_for_sdl_window(window);
    if (modalWindow != nil) {
        [panel beginSheetModalForWindow:modalWindow
                      completionHandler:^(NSModalResponse result) {
                          finish_folder_dialog(
                              state, result == NSModalResponseOK ? [panel URL] : nil, nullptr);
                      }];
        return true;
    }

    const NSModalResponse result = [panel runModal];
    finish_folder_dialog(state, result == NSModalResponseOK ? [panel URL] : nil, nullptr);
    return true;
}

}  // namespace dusk
