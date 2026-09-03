#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

/*
 * Hooks allow intercepting calls to game functions, allowing you to:
 * - Modify arguments
 * - Perform your own work before (pre), after (post) or instead of (replace) the original call
 * - From a pre hook, conditionally skip the original call and return your own value
 *
 * In most cases, you'll want to instead use the C++ helpers in mods/svc/hook.hpp
 * (mods::hook::add_pre/add_post/replace). They generate the trampoline passed to
 * install and provide compile-time type checking.
 *
 * resolve() resolves an address by symbol name for targets you can't name at compile time
 * (file-local statics included).
 */

#define HOOK_SERVICE_ID "dev.twilitrealm.dusklight.hook"
#define HOOK_SERVICE_MAJOR 1u
#define HOOK_SERVICE_MINOR 1u

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
 * them through mods::arg<T> / arg_ref<T> from mods/svc/hook.hpp. `userdata` is the pointer
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
     * Install a hook on fn_addr.
     *
     * trampoline_fn must point to a function that matches the original function's signature and
     * dispatches pre- and post- hooks. This dispatch trampoline is normally generated at compile
     * time using C++ template instantiation (see mods/svc/hook.hpp).
     *
     * The first hook install on a target will implicitly install a detour (patched instructions
     * on the target that jump to the dispatch trampoline). When all hooks are uninstalled from a
     * target, the detour is completely uninstalled.
     *
     * The address that the dispatch trampoline should call the original function through is written
     * to out_original_fn.
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

    /* Minor version 1 */

    /*
     * Uninstall the current mod's hook on fn_addr and unregister all callbacks.
     * If no other mods have a hook installed on the target, the detour is uninstalled entirely.
     *
     * original_fn_slot must match the out_original_fn passed to install.
     */
    ModResult (*uninstall)(ModContext* ctx, void* fn_addr, void** original_fn_slot);
} HookService;

MOD_DECLARE_SERVICE(HookService, svc_hook, HOOK_SERVICE_ID, HOOK_SERVICE_MAJOR, HOOK_SERVICE_MINOR);
