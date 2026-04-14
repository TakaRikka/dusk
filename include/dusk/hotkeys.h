#ifndef DUSK_HOTKEYS_H
#define DUSK_HOTKEYS_H

namespace dusk::hotkeys {

#if __APPLE__
constexpr const char* DO_RESET = "Cmd+R";
#else
constexpr const char* DO_RESET = "Ctrl+R";
#endif

constexpr const char* TOGGLE_FULLSCREEN = "F11";

constexpr const char* SHOW_PROCESS_MANAGEMENT = "F2";
constexpr const char* SHOW_DEBUG_OVERLAY = "F3";
constexpr const char* SHOW_HEAP_VIEWER = "F4";
constexpr const char* SHOW_STUB_LOG = "F5";
constexpr const char* SHOW_CAMERA_DEBUG = "F6";
constexpr const char* SHOW_AUDIO_DEBUG = "F7";
constexpr const char* SHOW_STATE_SHARE = "F8";

constexpr const char* TURBO = "Tab";

}

#endif // DUSK_HOTKEYS_H
