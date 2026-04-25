#ifndef DUSK_HOTKEYS_H
#define DUSK_HOTKEYS_H

namespace dusk::hotkeys {

#if __APPLE__
constexpr const char* DO_RESET = "Cmd+R";
#else
constexpr const char* DO_RESET = "Ctrl+R";
#endif

constexpr const char* SHOW_PROCESS_MANAGEMENT = "F2";
constexpr const char* SHOW_DEBUG_OVERLAY = "F3";
constexpr const char* SHOW_HEAP_VIEWER = "F4";
constexpr const char* SHOW_PLAYER_INFO = "F5";
constexpr const char* SHOW_SAVE_EDITOR = "F6";
constexpr const char* SHOW_MAP_LOADER = "F7";
constexpr const char* SHOW_STATE_SHARE = "F8";
constexpr const char* SHOW_DEBUG_CAMERA = "F9";
constexpr const char* SHOW_AUDIO_DEBUG = "F10";

constexpr const char* TOGGLE_FULLSCREEN = "F11";

constexpr const char* TURBO = "Tab";

}

#endif // DUSK_HOTKEYS_H
