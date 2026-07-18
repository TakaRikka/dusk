#pragma once

#include <mods/api.h>

/*
 * Intercept game functions by address. Prefer the typed helpers in mods/hook.hpp
 * (hook_add_pre/hook_add_post/hook_replace over a &Class::method): they generate the
 * trampoline and hide install/dispatch, which are the low-level primitives those helpers
 * build. resolve() maps a symbol name to an address for targets you can't name at compile time
 * (file-local statics included).
 *
 * Every call is game-thread-only. Install and removal must run with no hooked function on the
 * stack; the loader guarantees this by applying mod lifecycle changes between frames, which is
 * why hooking a function that never returns (the outermost loop) makes a mod un-unloadable.
 */

#define HOOK_SERVICE_ID "dev.twilitrealm.dusklight.hook"
#define HOOK_SERVICE_MAJOR 1u
#define HOOK_SERVICE_MINOR 0u

/* Symbol flags reported by resolve() */
typedef enum HookSymbolFlags {
    HOOK_SYMBOL_CODE = 1u << 0u,
    HOOK_SYMBOL_DATA = 1u << 1u,
    /* Not exported/dynamically visible: hookable, but never linkable. */
    HOOK_SYMBOL_LOCAL = 1u << 2u,
    /* Other names share this address (ICF fold/alias): a hook intercepts them all. */
    HOOK_SYMBOL_MULTI_NAME = 1u << 3u,
    /* Resolved through a demangled display-name alias rather than the real symbol. */
    HOOK_SYMBOL_DISPLAY = 1u << 6u,
} HookSymbolFlags;

/* A pre-hook's return value: whether to run the original function. */
typedef enum HookAction {
    HOOK_CONTINUE = 0,      /* run the original (and any lower-priority pre-hooks) */
    HOOK_SKIP_ORIGINAL = 1, /* cancel the original and remaining pre-hooks; post-hooks still run */
} HookAction;

/* How replace resolves a second replace-hook on a target that already has one. */
typedef enum HookReplacePolicy {
    HOOK_REPLACE_CONFLICT = 0, /* refuse with MOD_CONFLICT (the default) */
    HOOK_REPLACE_PRIORITY = 1, /* take over only if this options.priority is strictly higher */
    HOOK_REPLACE_OVERRIDE = 2, /* take over unconditionally */
} HookReplacePolicy;

/*
 * Hook callbacks. `args` is an array of pointers to the call's arguments (index 0 is `this`
 * for member functions); `retval` points at the return slot (NULL for void). Read and write
 * them through mods::arg<T> / arg_ref<T> from mods/hook.hpp. `userdata` is the pointer
 * from HookOptions. All run on the game thread, in the hooked call's own stack frame.
 */
typedef HookAction (*HookPreFn)(ModContext* ctx, void* args, void* retval, void* userdata);
typedef void (*HookPostFn)(ModContext* ctx, void* args, void* retval, void* userdata);
typedef void (*HookReplaceFn)(ModContext* ctx, void* args, void* retval, void* userdata);

typedef struct HookOptions {
    uint32_t struct_size;
    /* Higher runs first; ties break by registration order. Applies to pre/post ordering and,
     * with HOOK_REPLACE_PRIORITY, to replace-hook takeover. */
    int32_t priority;
    HookReplacePolicy replace_policy;
    void* userdata; /* passed back to the callback */
} HookOptions;

#define HOOK_OPTIONS_INIT {sizeof(HookOptions), 0, HOOK_REPLACE_CONFLICT, NULL}

typedef struct HookService {
    ServiceHeader header;

    /*
     * Install a trampoline detour on fn_addr and return the address to call the original through in
     * *out_original_fn. The typed helpers generate the trampoline and call this; mods normally
     * don't. The first mod to install a given target owns the live detour; later mods register as
     * candidates so a hook survives the owner unloading (the detour is handed off and every
     * original pointer is rewritten). Idempotent per (mod, out slot).
     */
    ModResult (*install)(
        ModContext* ctx, void* fn_addr, void* trampoline_fn, void** out_original_fn);

    /*
     * Register a callback on an already-installed target. Pre runs before the original (and can
     * cancel it), post runs after (even if cancelled). Any number of mods may add pre/post to the
     * same target; they run in priority then registration order. replace installs a single
     * substitute for the original, managed by options.replace_policy, MOD_CONFLICT if refused.
     */
    ModResult (*add_pre)(
        ModContext* ctx, void* fn_addr, HookPreFn callback, const HookOptions* options);
    ModResult (*add_post)(
        ModContext* ctx, void* fn_addr, HookPostFn callback, const HookOptions* options);
    ModResult (*replace)(
        ModContext* ctx, void* fn_addr, HookReplaceFn callback, const HookOptions* options);

    /*
     * Run the registered callbacks for a target. The generated trampoline calls these; they
     * are not a mod-facing entry point. dispatch_pre reports through *out_skip_original
     * whether the original should be skipped (a pre-hook returned HOOK_SKIP_ORIGINAL, or a
     * replace-hook ran).
     */
    ModResult (*dispatch_pre)(
        ModContext* ctx, void* fn_addr, void* args, void* retval, int* out_skip_original);
    ModResult (*dispatch_post)(ModContext* ctx, void* fn_addr, void* args, void* retval);

    /*
     * Resolve a game symbol by name from the symbol manifest, including non-exported (static)
     * functions. Names can be either the platform's mangled name (i.e. the name passed to dlopen;
     * no Mach-O leading underscore) or the qualified function name without parameters (e.g.
     * "daAlink_c::execute"). out_flags (optional) receives HookSymbolFlags.
     *
     * Results: MOD_OK; MOD_UNSUPPORTED (no manifest for this build, missing or stale);
     * MOD_UNAVAILABLE (symbol not found); MOD_CONFLICT (name maps to more than one address: C++
     * overloads or per-TU statics; use the mangled name).
     */
    ModResult (*resolve)(
        ModContext* ctx, const char* symbol, void** out_addr, HookSymbolFlags* out_flags);
} HookService;

#ifdef __cplusplus
#include "mods/service.hpp"

template <>
struct mods::ServiceTraits<HookService> {
    static constexpr const char* id = HOOK_SERVICE_ID;
    static constexpr uint16_t major_version = HOOK_SERVICE_MAJOR;
    static constexpr uint16_t minor_version = HOOK_SERVICE_MINOR;
};
#endif
