#pragma once

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_scancode.h>

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dusk::mods {
struct LoadedMod;
}

namespace dusk::actions {

using Handle = uint64_t;
using CaptureHandle = uint64_t;
using RebindSessionHandle = uint64_t;

inline constexpr Handle kInvalidHandle = 0;
inline constexpr CaptureHandle kInvalidCaptureHandle = 0;
inline constexpr RebindSessionHandle kInvalidRebindSessionHandle = 0;

// Config persistence namespace for host-registered Actions (not a `.dusk` mod id).
// Keys: actions.host.<name> — does not collide with reverse-DNS mod ids.
inline constexpr std::string_view kHostActionConfigId = "host";

namespace host {
inline constexpr std::string_view kOpenDusklightMenu = "open_dusklight_menu";
inline constexpr std::string_view kTurboSpeedButton = "turbo_speed_button";
}

enum class Result {
    Ok,
    InvalidArgument,
    Conflict,
    Error,
};

enum class Delivery {
    Gameplay = 0,
    Always = 1,
};

enum class BindingKind {
    Unbound = 0,
    Keyboard = 1,
    GamepadButton = 2,
    GamepadAxis = 3,
};

enum class Event {
    Pressed = 0,
    Released = 1,
};

enum class CaptureFilter {
    Either = 0,
    Keyboard = 1,
    Gamepad = 2,
};

struct Binding {
    BindingKind kind = BindingKind::Unbound;
    SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN;
    SDL_GamepadButton gamepad_button = SDL_GAMEPAD_BUTTON_INVALID;
    SDL_GamepadAxis gamepad_axis = SDL_GAMEPAD_AXIS_INVALID;
};

inline Binding make_unbound() { return {}; }

inline Binding make_keyboard_binding(const SDL_Scancode scancode) {
    Binding binding{};
    binding.kind = BindingKind::Keyboard;
    binding.scancode = scancode;
    return binding;
}

// Optional observer. `ctx` is the owning mod's ModContext* when registered by a mod, else nullptr.
using CallbackFn = void (*)(void* ctx, Handle action, Event event, void* user_data);

// Capture completion. Binding is not auto-applied; call set_binding to keep it.
using CaptureFn = void (*)(void* ctx, Handle action, const Binding& binding, void* user_data);
// Optional; invoked when a capture ends (complete, Escape cancel, or cancel_capture) so ABI
// facades can free thunk user_data even when the completion callback never runs.
using CaptureUserDataCleanupFn = void (*)(void* user_data);

struct Desc {
    std::string_view name;
    std::string_view display_name;
    Delivery delivery = Delivery::Gameplay;
    Binding default_binding{};
    CallbackFn callback = nullptr;
    void* user_data = nullptr;
    // Optional; invoked when the Action is unregistered or its owning mod detaches.
    void (*user_data_cleanup)(void*) = nullptr;
};

struct ActionInfo {
    Handle handle = kInvalidHandle;
    std::string name;
    std::string displayName;
};

// Shared UI copy while a rebind capture is active.
inline constexpr const char* kRebindCapturingPrompt = "Press a key or button...";

// --- Presentation epoch ---
// UI opens each epoch (ui::update before ModLoader::tick); frameBegin falls back when UI did not.
void ensure_host_actions_registered();
void ensure_actions_evaluated();
void evaluate_frame();
void shutdown_host();
void remove_mod(mods::LoadedMod& mod);

// --- Host Action consumer seam (Settings / UI / gameplay conveniences) ---
std::vector<ActionInfo> list_host_actions();
Result get_host_binding(Handle action, Binding& out_binding);
Result get_host_default_binding(Handle action, Binding& out_binding);
Result set_host_binding(Handle action, const Binding& binding);
Result reset_host_binding(Handle action);
Result begin_host_capture(Handle action, CaptureFilter filter, CaptureFn callback, void* user_data,
    CaptureHandle* out_handle = nullptr, CaptureUserDataCleanupFn user_data_cleanup = nullptr);
Result cancel_host_capture(CaptureHandle handle);
bool host_capture_is_active(CaptureHandle handle);
// True while any Action capture is live (host Settings rebind or mod rebind).
bool any_capture_active();

// Open Dusklight Menu — observation + replace-when-bound policy for host UI.
bool open_dusklight_menu_was_pressed();
bool open_dusklight_menu_should_suppress_default_back();
bool open_dusklight_menu_should_suppress_default_f1();
std::string open_dusklight_menu_keyboard_label();
std::string open_dusklight_menu_gamepad_label(SDL_Gamepad* gamepad, std::string_view unbound_fallback);

// Turbo Speed — host-applied gameplay Action.
bool turbo_speed_button_is_held();

// Readable display label for a Binding. `gamepad` optional for layout-aware names.
// Unbound → "Not Bound". Unknown → "Unknown".
std::string format_binding_label(const Binding& binding, SDL_Gamepad* gamepad = nullptr);
std::string format_gamepad_button_label(SDL_Gamepad* gamepad, SDL_GamepadButton button);

/*
 * Host-owned rebind session: begin capture, apply Binding on complete, or cancel with no change.
 * Construct with no owner for host Actions; with a LoadedMod for that mod's Actions.
 */
class RebindSession {
public:
    RebindSession() = default;
    explicit RebindSession(mods::LoadedMod& mod) : mOwner(&mod) {}

    RebindSession(const RebindSession&) = delete;
    RebindSession& operator=(const RebindSession&) = delete;

    ~RebindSession() { cancel(); }

    [[nodiscard]] bool is_capturing() const;
    [[nodiscard]] bool is_capturing(Handle action) const;
    [[nodiscard]] Handle capturing_action() const;
    [[nodiscard]] const char* capturing_prompt(Handle action) const;

    Result begin(Handle action, CaptureFilter filter = CaptureFilter::Either);
    void cancel();

private:
    void clear() const;
    void sync_capture_state() const;

    static void on_complete(void* ctx, Handle action, const Binding& binding, void* user_data);

    mods::LoadedMod* mOwner = nullptr;
    mutable Handle mCapturingAction = kInvalidHandle;
    mutable CaptureHandle mCaptureHandle = kInvalidCaptureHandle;
};

// --- Mod-owned ActionService surface (facade maps ABI ↔ these) ---
Result register_action(mods::LoadedMod& mod, const Desc& desc, Handle* out_handle);
Result unregister_action(mods::LoadedMod& mod, Handle action);
Result get_binding(mods::LoadedMod& mod, Handle action, Binding& out_binding);
Result get_default_binding(mods::LoadedMod& mod, Handle action, Binding& out_binding);
Result set_binding(mods::LoadedMod& mod, Handle action, const Binding& binding);
Result reset_binding(mods::LoadedMod& mod, Handle action);
Result is_down(mods::LoadedMod& mod, Handle action, bool& out_down);
Result was_pressed(mods::LoadedMod& mod, Handle action, bool& out_pressed);
Result was_released(mods::LoadedMod& mod, Handle action, bool& out_released);
Result is_held(mods::LoadedMod& mod, Handle action, bool& out_held);
Result begin_capture(mods::LoadedMod& mod, Handle action, CaptureFilter filter, CaptureFn callback,
    void* user_data, CaptureHandle* out_handle,
    CaptureUserDataCleanupFn user_data_cleanup = nullptr);
Result cancel_capture(mods::LoadedMod& mod, CaptureHandle handle);
Result is_capture_active(mods::LoadedMod& mod, CaptureHandle handle, bool& out_active);

// Rebind session lifecycle for mods (handles owned by the calling mod).
Result create_rebind_session(mods::LoadedMod& mod, RebindSessionHandle* out_session);
Result destroy_rebind_session(mods::LoadedMod& mod, RebindSessionHandle session);
Result begin_rebind_session(mods::LoadedMod& mod, RebindSessionHandle session, Handle action,
    CaptureFilter filter);
Result cancel_rebind_session(mods::LoadedMod& mod, RebindSessionHandle session);
Result query_rebind_session(mods::LoadedMod& mod, RebindSessionHandle session, bool& out_capturing,
    Handle* out_action);

}  // namespace dusk::actions
