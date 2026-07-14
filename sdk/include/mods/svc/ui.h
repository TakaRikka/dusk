#pragma once

#include <mods/api.h>
#include <mods/svc/config.h>

#define UI_SERVICE_ID "dev.twilitrealm.dusklight.ui"
#define UI_SERVICE_MAJOR 1u
#define UI_SERVICE_MINOR 0u

/*
 * UI primitives: a panel inside the host Mods window, mod-owned windows, dialogs, scoped
 * RCSS stylesheets and menu bar tabs.
 *
 * All calls must be made on the game thread from mod callbacks (initialize, update, hooks, or UI
 * callbacks). Handles are opaque, generation-checked ids; a stale or unknown handle fails with
 * MOD_INVALID_ARGUMENT. Element handles die with the content that owns them: a panel or tab rebuild
 * destroys the previous build's elements, so re-acquire handles inside the build callback rather
 * than caching them. Strings are UTF-8 and, in both directions, only valid for the duration of the
 * call.
 */

/* 0 is never a valid handle. */
typedef uint64_t UiWindowHandle;
typedef uint64_t UiDialogHandle;
typedef uint64_t UiElementHandle;
typedef uint64_t UiStyleHandle;
typedef uint64_t UiMenuTabHandle;

typedef enum UiStyleScope {
    UI_SCOPE_PRELAUNCH = 0,      /* the pre-launch menu */
    UI_SCOPE_WINDOW = 1,         /* every tabbed/small window, host and mod alike */
    UI_SCOPE_MENU_BAR = 2,       /* the in-game menu bar */
    UI_SCOPE_OVERLAY = 3,        /* the passive overlay (toasts, FPS counter, timers) */
    UI_SCOPE_TOUCH_CONTROLS = 4, /* touch controls and their editor */
    UI_SCOPE_GRAPHICS_TUNER = 5, /* the graphics tuner overlay window */
} UiStyleScope;

typedef enum UiDialogVariant {
    UI_DIALOG_NORMAL = 0,
    UI_DIALOG_WARNING = 1, /* warning icon by default */
    UI_DIALOG_DANGER = 2,  /* red styling, error icon by default */
} UiDialogVariant;

typedef enum UiControlKind {
    UI_CONTROL_BUTTON = 0, /* action button (on_pressed) */
    UI_CONTROL_TOGGLE = 1, /* boolean on/off */
    UI_CONTROL_NUMBER = 2, /* integer stepper with min/max/step */
    UI_CONTROL_STRING = 3, /* text input */
    UI_CONTROL_SELECT = 4, /* one of `options`; the value is the option index */
} UiControlKind;

typedef enum UiControlBinding {
    /* Values flow through the `get`/`set` callbacks. Getters are polled every frame while the
       control is visible and must be cheap. */
    UI_BINDING_CALLBACKS = 0,
    /* The control reads and writes `config_var` (a ConfigService handle owned by the calling mod)
     * directly: persistence, change notifications and the modified indicator (value != default) are
     * wired automatically. The var type must match the control kind: TOGGLE = bool, NUMBER and
     * SELECT = int, STRING = string. Float vars are not bindable; use callbacks. */
    UI_BINDING_CONFIG_VAR = 1,
} UiControlBinding;

/* Tagged by the control's kind: TOGGLE reads bool_value, NUMBER and SELECT read int_value, STRING
 * reads string_value. string_value passed to a setter is only valid during the call; a getter
 * should point it at storage owned by the mod (e.g. a static buffer) that stays valid until the
 * next call into the mod — the host copies it right after the getter returns. */
typedef struct UiControlValue {
    uint32_t struct_size;
    bool bool_value;
    int64_t int_value;
    const char* string_value;
} UiControlValue;

#define UI_CONTROL_VALUE_INIT {sizeof(UiControlValue), false, 0, NULL}

typedef void (*UiControlGetFn)(ModContext* ctx, void* user_data, UiControlValue* out_value);
typedef void (*UiControlSetFn)(ModContext* ctx, void* user_data, const UiControlValue* value);
/* Polled every frame while the control is visible. */
typedef bool (*UiPredicateFn)(ModContext* ctx, void* user_data);
typedef void (*UiPressedFn)(ModContext* ctx, void* user_data);

typedef struct UiControlDesc {
    uint32_t struct_size;
    UiControlKind kind;
    /* Row label (plain text). Required. */
    const char* label;
    /* Optional RML shown as contextual help when the control is focused or hovered. Only rendered
     * where a help pane exists (mod window tabs). */
    const char* help_rml;
    UiControlBinding binding;   /* ignored for BUTTON */
    ConfigVarHandle config_var; /* UI_BINDING_CONFIG_VAR */
    UiControlGetFn get;         /* UI_BINDING_CALLBACKS (all kinds but BUTTON) */
    UiControlSetFn set;         /* UI_BINDING_CALLBACKS (all kinds but BUTTON) */
    UiPressedFn on_pressed;     /* BUTTON only. Required for BUTTON. */
    UiPredicateFn is_disabled;  /* optional */
    /* Optional override for the modified indicator. CONFIG_VAR controls derive it from value !=
     * default when this is NULL. */
    UiPredicateFn is_modified;
    /* Passed to every callback above. */
    void* user_data;
    /* NUMBER: inclusive clamp range and step. min == max means the defaults (0 .. INT32_MAX); step
     * < 1 means 1. */
    int64_t min;
    int64_t max;
    int64_t step;
    const char* prefix; /* NUMBER: optional text before the value */
    const char* suffix; /* NUMBER: optional text after the value */
    /* SELECT: option labels (plain text). Required for SELECT. SELECT controls
     * present their options in the help pane, so they are only available where
     * one exists (mod window tabs); MOD_UNSUPPORTED elsewhere. */
    const char* const* options;
    size_t option_count;
    int32_t max_length; /* STRING: maximum input length; < 1 means unlimited */
} UiControlDesc;

#define UI_CONTROL_DESC_INIT                                                                       \
    {sizeof(UiControlDesc), UI_CONTROL_BUTTON, NULL, NULL, UI_BINDING_CALLBACKS, 0u, NULL, NULL,   \
        NULL, NULL, NULL, NULL, 0, 0, 1, NULL, NULL, NULL, 0u, 0}

/* Build the panel contents. `panel` accepts the pane_add_* functions; it and
 * every element created in it are destroyed (handles invalidated) whenever the
 * panel is rebuilt, e.g. on tab switches. A non-MOD_OK result fails the mod. */
typedef ModResult (*UiPanelBuildFn)(
    ModContext* ctx, UiElementHandle panel, void* user_data, ModError* out_error);
/* Called every frame while the panel is the visible tab. */
typedef ModResult (*UiPanelUpdateFn)(ModContext* ctx, void* user_data, ModError* out_error);

/* A panel rendered inside the calling mod's tab of the host Mods window. */
typedef struct UiModsPanelDesc {
    uint32_t struct_size;
    UiPanelBuildFn build; /* required */
    UiPanelUpdateFn update;
    void* user_data;
} UiModsPanelDesc;

#define UI_MODS_PANEL_DESC_INIT {sizeof(UiModsPanelDesc), NULL, NULL, NULL}

/* Build one tab of a mod window. `left_pane` is the interactive column,
 * `right_pane` shows contextual help (controls' help_rml and SELECT options
 * render there). Rebuilt on every tab activation. */
typedef ModResult (*UiTabBuildFn)(ModContext* ctx, UiWindowHandle window, UiElementHandle left_pane,
    UiElementHandle right_pane, void* user_data, ModError* out_error);

typedef struct UiTabDesc {
    uint32_t struct_size;
    const char* title;  /* required */
    UiTabBuildFn build; /* required */
    UiPanelUpdateFn update;
    void* user_data;
} UiTabDesc;

#define UI_TAB_DESC_INIT {sizeof(UiTabDesc), NULL, NULL, NULL, NULL}

typedef void (*UiWindowClosedFn)(ModContext* ctx, UiWindowHandle window, void* user_data);

typedef struct UiWindowDesc {
    uint32_t struct_size;
    const UiTabDesc* tabs; /* at least one */
    size_t tab_count;
    /* Optional RCSS applied to this window's document only (on top of any
     * UI_SCOPE_WINDOW sheets, which apply automatically). */
    const char* rcss;
    /* Fired when the window is destroyed by user close or window_close. Not
     * fired during owning-mod teardown/shutdown. The handle is already invalid. */
    UiWindowClosedFn on_closed;
    void* user_data;
} UiWindowDesc;

#define UI_WINDOW_DESC_INIT {sizeof(UiWindowDesc), NULL, 0u, NULL, NULL, NULL}

typedef void (*UiDialogActionFn)(ModContext* ctx, UiDialogHandle dialog, void* user_data);

/* Note: array element without struct_size; a future change requires appending
 * a v2 desc struct rather than growing this one. */
typedef struct UiDialogAction {
    const char* label; /* required */
    UiDialogActionFn on_pressed;
    void* user_data;
    bool keep_open; /* false = the dialog closes after on_pressed returns */
} UiDialogAction;

typedef struct UiDialogDesc {
    uint32_t struct_size;
    const char* title;    /* plain text; required */
    const char* body_rml; /* RML body; required */
    UiDialogVariant variant;
    /* Optional icon name overriding the variant default: "warning", "error",
     * "question-mark", "verifying", "celebration". */
    const char* icon;
    const UiDialogAction* actions; /* at least one */
    size_t action_count;
    /* Fired on cancel (B/Escape) before the dialog closes; the dialog always
     * closes on dismiss. */
    UiDialogActionFn on_dismiss;
    void* user_data; /* passed to on_dismiss */
} UiDialogDesc;

#define UI_DIALOG_DESC_INIT                                                                        \
    {sizeof(UiDialogDesc), NULL, NULL, UI_DIALOG_NORMAL, NULL, NULL, 0u, NULL, NULL}

/* A tab added to the in-game menu bar. */
typedef struct UiMenuTabDesc {
    uint32_t struct_size;
    const char* label;       /* plain text; required */
    UiPressedFn on_selected; /* fired when the user activates the tab; required */
    void* user_data;
} UiMenuTabDesc;

#define UI_MENU_TAB_DESC_INIT {sizeof(UiMenuTabDesc), NULL, NULL, NULL}

typedef struct UiService {
    ServiceHeader header;

    /* Register or replace the panel shown in the calling mod's Mods-window tab. */
    ModResult (*register_mods_panel)(ModContext* ctx, const UiModsPanelDesc* desc);

    /* Content builders. `pane` is a panel or tab pane handle; out_elem (where
     * present, optional) receives a handle for later elem_set_* updates. */
    ModResult (*pane_add_section)(ModContext* ctx, UiElementHandle pane, const char* title);
    ModResult (*pane_add_text)(
        ModContext* ctx, UiElementHandle pane, const char* text, UiElementHandle* out_elem);
    ModResult (*pane_add_rml)(
        ModContext* ctx, UiElementHandle pane, const char* rml, UiElementHandle* out_elem);
    ModResult (*pane_add_progress)(
        ModContext* ctx, UiElementHandle pane, float value, UiElementHandle* out_elem);
    ModResult (*pane_add_control)(ModContext* ctx, UiElementHandle pane, const UiControlDesc* desc,
        UiElementHandle* out_elem);

    /* Element updates. The handle kind must match the setter (text/rml on text
     * rows, progress on progress bars). */
    ModResult (*elem_set_text)(ModContext* ctx, UiElementHandle elem, const char* text);
    ModResult (*elem_set_rml)(ModContext* ctx, UiElementHandle elem, const char* rml);
    ModResult (*elem_set_progress)(ModContext* ctx, UiElementHandle elem, float value);
    /* Set or clear an RCSS class on any element handle (rows, progress bars,
     * controls, panes), for styling via scoped or window RCSS. */
    ModResult (*elem_set_class)(
        ModContext* ctx, UiElementHandle elem, const char* name, bool active);

    /* Push a tabbed two-pane window onto the document stack and show it. */
    ModResult (*window_push)(ModContext* ctx, const UiWindowDesc* desc, UiWindowHandle* out_window);
    ModResult (*window_close)(ModContext* ctx, UiWindowHandle window);

    /* Push a modal dialog onto the document stack and show it. */
    ModResult (*dialog_push)(ModContext* ctx, const UiDialogDesc* desc, UiDialogHandle* out_dialog);
    ModResult (*dialog_close)(ModContext* ctx, UiDialogHandle dialog);
    ModResult (*dialog_set_body)(ModContext* ctx, UiDialogHandle dialog, const char* body_rml);
    /* Replace the dialog icon ("" removes it; names as in UiDialogDesc.icon). */
    ModResult (*dialog_set_icon)(ModContext* ctx, UiDialogHandle dialog, const char* icon);
    /* Append one action button (same callback rules as at push). */
    ModResult (*dialog_add_action)(
        ModContext* ctx, UiDialogHandle dialog, const UiDialogAction* action);

    /* Whether any focus-stack document is currently visible (visible documents block gamepad
     * input). */
    ModResult (*is_any_document_visible)(ModContext* ctx, bool* out_visible);

    /* Register an RCSS sheet applied to every document of `scope`, now and in the future, until
     * unregistered or the calling mod is torn down. Sheets apply in registration order after the
     * host styles. Fails with MOD_INVALID_ARGUMENT if the RCSS does not parse. */
    ModResult (*register_styles)(
        ModContext* ctx, UiStyleScope scope, const char* rcss, UiStyleHandle* out_style);
    /* Like register_styles, but reads the RCSS from the mod bundle's res/ directory (same path
     * rules as ResourceService::load). MOD_UNAVAILABLE if the file does not exist. */
    ModResult (*register_styles_file)(
        ModContext* ctx, UiStyleScope scope, const char* path, UiStyleHandle* out_style);
    ModResult (*unregister_styles)(ModContext* ctx, UiStyleHandle style);

    /* Add a tab to the in-game menu bar, on_selected runs with the menu open; pushing a window from
     * it stacks the window over the menu like the host tabs do. The tab is removed when
     * unregistered or the mod is torn down. */
    ModResult (*register_menu_tab)(
        ModContext* ctx, const UiMenuTabDesc* desc, UiMenuTabHandle* out_tab);
    ModResult (*unregister_menu_tab)(ModContext* ctx, UiMenuTabHandle tab);
} UiService;

#ifdef __cplusplus
#include "mods/service.hpp"

template <>
struct dusk::mods::ServiceTraits<UiService> {
    static constexpr const char* id = UI_SERVICE_ID;
    static constexpr uint16_t major_version = UI_SERVICE_MAJOR;
    static constexpr uint16_t minor_version = UI_SERVICE_MINOR;
};
#endif
