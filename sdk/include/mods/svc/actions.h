#pragma once

#include <mods/api.h>

#if !defined(DUSK_BUILDING_GAME) && !defined(DUSK_MOD_FEATURE_ACTIONS)
#error "mods/svc/actions.h requires add_mod(... FEATURES actions)"
#endif

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_scancode.h>

#define ACTIONS_SERVICE_ID "dev.twilitrealm.dusklight.actions"
#define ACTIONS_SERVICE_MAJOR 1u
#define ACTIONS_SERVICE_MINOR 0u

/* Handle for a live Action registration. 0 is never a valid handle. */
typedef uint64_t ActionHandle;
/* Handle for an in-progress rebind capture. 0 is never a valid handle. */
typedef uint64_t ActionCaptureHandle;
/* Handle for a host-owned rebind session. 0 is never a valid handle. */
typedef uint64_t ActionRebindSessionHandle;

typedef enum ActionDeliveryPolicy {
    /* Deliver only when the host has not captured input for UI. */
    ACTION_DELIVERY_GAMEPLAY = 0,
    /* Deliver whenever the Binding fires, including while menus hold focus. */
    ACTION_DELIVERY_ALWAYS = 1,
} ActionDeliveryPolicy;

typedef enum ActionBindingKind {
    ACTION_BINDING_UNBOUND = 0,
    ACTION_BINDING_KEYBOARD = 1,
    ACTION_BINDING_GAMEPAD_BUTTON = 2,
    ACTION_BINDING_GAMEPAD_AXIS = 3,
} ActionBindingKind;

/*
 * Tagged discrete control that fires an Action. Only the field matching `kind` is meaningful.
 * Persisted by ActionService; cleared (unbound) is first-class and survives reload.
 * Gamepad buttons and trigger axes are evaluated on the primary player slot (PAD_1); keyboard is
 * global. ACTION_BINDING_GAMEPAD_AXIS accepts only LEFT_TRIGGER / RIGHT_TRIGGER; "down" means the
 * axis exceeds that controller's left/right trigger activation zone from Controller Config.
 */
typedef struct ActionBinding {
    uint32_t struct_size;
    ActionBindingKind kind;
    SDL_Scancode scancode;              /* ACTION_BINDING_KEYBOARD */
    SDL_GamepadButton gamepad_button;   /* ACTION_BINDING_GAMEPAD_BUTTON */
    SDL_GamepadAxis gamepad_axis;       /* ACTION_BINDING_GAMEPAD_AXIS (triggers only) */
} ActionBinding;

#define ACTION_BINDING_INIT                                                                        \
    {sizeof(ActionBinding), ACTION_BINDING_UNBOUND, SDL_SCANCODE_UNKNOWN,                          \
        (SDL_GamepadButton)SDL_GAMEPAD_BUTTON_INVALID,                                             \
        (SDL_GamepadAxis)SDL_GAMEPAD_AXIS_INVALID}

typedef enum ActionEvent {
    ACTION_EVENT_PRESSED = 0,  /* Binding went down this frame */
    ACTION_EVENT_RELEASED = 1, /* Binding went up this frame */
} ActionEvent;

/*
 * Optional observer registered with the Action. Invoked on the game thread when a press or release
 * edge is delivered. Poll APIs remain available regardless of whether a callback is set.
 */
typedef void (*ActionCallbackFn)(
    ModContext* ctx, ActionHandle action, ActionEvent event, void* user_data);

/*
 * Descriptor for register_action. Identity is (owning mod id, name). Registration is host-shaped:
 * display name, delivery policy, and a single default Binding. A persisted Binding for that
 * identity wins over the default. Callback may be NULL.
 */
typedef struct ActionDesc {
    uint32_t struct_size;
    /* Local name: 1-64 characters from [A-Za-z0-9_-]. */
    const char* name;
    /* UTF-8 label for rebind UIs. Required. */
    const char* display_name;
    ActionDeliveryPolicy delivery;
    /* Default applied only when no persisted Binding exists. */
    ActionBinding default_binding;
    ActionCallbackFn callback;
    void* user_data;
} ActionDesc;

#define ACTION_DESC_INIT                                                                           \
    {sizeof(ActionDesc), NULL, NULL, ACTION_DELIVERY_GAMEPLAY, ACTION_BINDING_INIT, NULL, NULL}

typedef enum ActionCaptureFilter {
    ACTION_CAPTURE_EITHER = 0,   /* first keyboard key, gamepad button, or trigger */
    ACTION_CAPTURE_KEYBOARD = 1, /* keyboard only */
    ACTION_CAPTURE_GAMEPAD = 2,  /* gamepad button or trigger only */
} ActionCaptureFilter;

/*
 * Fired on the game thread when capture completes with a Binding. Escape during capture cancels
 * (same as cancel_capture: no callback, no Binding change) and is never assignable. Matching input
 * completes as that Binding. The host does not auto-apply it; call set_binding if you want to keep
 * it. Cancelled captures do not fire. Unbound is assigned only via set_binding (Clear), not Escape.
 */
typedef void (*ActionCaptureFn)(ModContext* ctx, ActionHandle action, const ActionBinding* binding,
    void* user_data);

/*
 * Register Actions, manage Bindings, poll/callback observation, and rebind capture.
 *
 * Observation only: the service never performs game or mod side effects on the caller's behalf.
 * Game thread only. Independent of InputService at the mod ABI. Live registrations are owned by
 * the calling mod and cleared when it is disabled, reloaded, or fails; persisted Bindings remain
 * for the next register of the same identity. Multiple Actions may share a Binding (fan-out).
 * Evaluation uses the primary player (PAD_1) for gamepad buttons and triggers; keyboard is global.
 */
typedef struct ActionService {
    ServiceHeader header;

    /* Register an Action. Duplicate live local name for the calling mod is MOD_CONFLICT. */
    ModResult (*register_action)(
        ModContext* ctx, const ActionDesc* desc, ActionHandle* out_handle);
    /* Unregister a live Action. Persisted Bindings for its identity are kept. */
    ModResult (*unregister_action)(ModContext* ctx, ActionHandle action);

    ModResult (*get_binding)(ModContext* ctx, ActionHandle action, ActionBinding* out_binding);
    /* Read the Binding supplied at register_action (registration default). Does not change the
     * current Binding. Use with get_binding to choose Clear vs Reset and to disable Reset when
     * already at default. */
    ModResult (*get_default_binding)(
        ModContext* ctx, ActionHandle action, ActionBinding* out_binding);
    /* Set or clear (ACTION_BINDING_UNBOUND) the Binding. Persisted immediately (debounced with
     * other config writes). Shared Bindings fan out; set does not conflict. Unbound is assigned
     * only here (Clear), never by Escape during capture. */
    ModResult (*set_binding)(
        ModContext* ctx, ActionHandle action, const ActionBinding* binding);
    /* Restore the registration default and persist it. Pair a rebind row with Clear when the
     * default is unbound, or Reset when it is a concrete Binding — never both. */
    ModResult (*reset_binding)(ModContext* ctx, ActionHandle action);

    /* Poll. out_* may not be NULL. Returns the delivered state after delivery-policy gating. */
    ModResult (*is_down)(ModContext* ctx, ActionHandle action, bool* out_down);
    ModResult (*was_pressed)(ModContext* ctx, ActionHandle action, bool* out_pressed);
    ModResult (*was_released)(ModContext* ctx, ActionHandle action, bool* out_released);
    ModResult (*is_held)(ModContext* ctx, ActionHandle action, bool* out_held);

    /*
     * Listen for the next discrete input matching filter and invoke callback when one arrives.
     * Escape cancels capture (same as cancel_capture: no callback, no Binding change), regardless
     * of filter; it is not assignable via capture. cancel_capture aborts without firing the
     * callback or changing the Binding. Only one capture may be active per calling mod at a time;
     * starting another cancels the previous. out_handle may be NULL. Gamepad capture listens on the
     * primary player (PAD_1) for buttons and LT/RT trigger pulls (Controller Config activation
     * zones).
     */
    ModResult (*begin_capture)(ModContext* ctx, ActionHandle action, ActionCaptureFilter filter,
        ActionCaptureFn callback, void* user_data, ActionCaptureHandle* out_handle);
    ModResult (*cancel_capture)(ModContext* ctx, ActionCaptureHandle handle);
    /* True while this capture handle is still live (false after complete, Escape cancel, or
     * cancel_capture). out_active may not be NULL. */
    ModResult (*is_capture_active)(
        ModContext* ctx, ActionCaptureHandle handle, bool* out_active);

    /*
     * Format a Binding as a UTF-8 display label (scancode / gamepad button / trigger name).
     * Uses the primary player's assigned gamepad when available for layout-aware names.
     * out_buf must be non-NULL; out_buf_size includes the NUL terminator. Truncates if needed.
     * Unbound → "Not Bound". Unknown → "Unknown".
     */
    ModResult (*format_binding_label)(
        ModContext* ctx, const ActionBinding* binding, char* out_buf, uint32_t out_buf_size);

    /*
     * Host-owned rebind session lifecycle. Prefer these over composing begin_capture + set_binding
     * for rebind UIs: begin listens for input, applies the Binding on complete, and cancel / Escape
     * leave the Binding unchanged. Sessions are owned by the calling mod and cleared on detach.
     * Lower-level capture APIs remain available beside sessions.
     */
    ModResult (*create_rebind_session)(ModContext* ctx, ActionRebindSessionHandle* out_session);
    ModResult (*destroy_rebind_session)(ModContext* ctx, ActionRebindSessionHandle session);
    ModResult (*begin_rebind_session)(ModContext* ctx, ActionRebindSessionHandle session,
        ActionHandle action, ActionCaptureFilter filter);
    ModResult (*cancel_rebind_session)(ModContext* ctx, ActionRebindSessionHandle session);
    /* out_capturing may not be NULL. out_action may be NULL; 0 when not capturing. */
    ModResult (*query_rebind_session)(ModContext* ctx, ActionRebindSessionHandle session,
        bool* out_capturing, ActionHandle* out_action);
} ActionService;

#ifdef __cplusplus
#include "mods/service.hpp"

template <>
struct mods::ServiceTraits<ActionService> {
    static constexpr const char* id = ACTIONS_SERVICE_ID;
    static constexpr uint16_t major_version = ACTIONS_SERVICE_MAJOR;
    static constexpr uint16_t minor_version = ACTIONS_SERVICE_MINOR;
};
#endif
