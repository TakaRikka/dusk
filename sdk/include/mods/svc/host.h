#pragma once

#include <mods/api.h>

/*
 * The host service: the calling mod's identity and its runtime interface to the loader.
 * Always available; every other service can be reached from it.
 */

#define HOST_SERVICE_ID "dev.twilitrealm.dusklight.host"
#define HOST_SERVICE_MAJOR 2u
#define HOST_SERVICE_MINOR 1u

/*
 * Ignore unknown values: later service minors may add events.
 */
typedef enum ModLifecycleEvent {
    /*
     * The subject mod is gone: its mod_shutdown has run (when it initialized at all) and
     * every service has already dropped the state it held for it. The subject's library is
     * still mapped, so pointers into it are valid to compare against, but they must not be
     * called or dereferenced after the callback returns. Drop everything keyed to the
     * mod: callbacks it registered, its ModContext*, state indexed by it.
     */
    MOD_LIFECYCLE_DETACHED = 0,
} ModLifecycleEvent;

/*
 * ctx is the watching mod's own context; subject identifies the mod the event is about.
 * subject_id is valid only for the duration of the call.
 */
typedef void (*ModLifecycleFn)(ModContext* ctx, ModContext* subject, const char* subject_id,
    ModLifecycleEvent event, void* user_data);

typedef struct HostService {
    ServiceHeader header;

    /* Version string of the current Dusklight build. (e.g. "1.4.2") */
    const char* version;

    /* Build id of the running game binary: PDB GUID+age on Windows, LC_UUID on macOS, GNU build-id
     * on Linux. May be empty (len 0) if the identity could not be determined. */
    const uint8_t* build_id;
    uint32_t build_id_len;

    /*
     * Look up a service by id at call time. Unlike a manifest import, this sees whatever is
     * currently published and carries no initialization-order guarantee (see mods/api.h).
     * MOD_UNAVAILABLE if no matching service is published; *out_service is null on failure.
     */
    ModResult (*get_service)(ModContext* ctx, const char* service_id, uint16_t major_version,
        uint16_t min_minor_version, const void** out_service);

    /*
     * Publish a service the calling mod declared as a deferred export in its manifest.
     * Must happen during mod_initialize so importers can resolve it; `service` must stay
     * valid until the mod shuts down.
     */
    ModResult (*publish_service)(
        ModContext* ctx, const char* service_id, uint16_t major_version, const void* service);

    /*
     * Report an unrecoverable failure. The calling mod's services stop resolving immediately
     * and the loader fully disables it at the next safe point; `message` is shown to the user.
     * Safe to call from any mod callback.
     */
    void (*fail)(ModContext* ctx, ModResult code, const char* message);

    /*
     * The calling mod's manifest metadata. Returned strings remain valid while the mod is
     * loaded.
     */
    const char* (*mod_id)(ModContext* ctx);
    const char* (*mod_name)(ModContext* ctx);
    const char* (*mod_version)(ModContext* ctx);

    /*
     * A writable scratch directory reserved for the calling mod. Contents survive disable
     * and reload within a session, but the directory is wiped at game startup.
     */
    const char* (*mod_dir)(ModContext* ctx);

    /*
     * Observe other mods' lifecycle events. Any mod whose service hands out per-caller state
     * (registrations, callbacks, handles) should watch for MOD_LIFECYCLE_DETACHED and drop what it
     * holds for the subject.
     *
     * Callbacks fire on the game thread at a lifecycle safe point (never mid-frame), for
     * every mod but the watcher itself (use mod_shutdown for self-cleanup).
     */
    ModResult (*watch_mod_lifecycle)(
        ModContext* ctx, ModLifecycleFn fn, void* user_data, uint64_t* out_handle);
    ModResult (*unwatch_mod_lifecycle)(ModContext* ctx, uint64_t handle);

    /*
     * Read-only directory containing this platform's packaged native runtime: the mod module
     * and any RUNTIME_LIBRARIES. The path is absolute and remains valid until mod_shutdown
     * returns. Libraries loaded dynamically from here are owned by the mod and must be unloaded
     * during mod_shutdown.
     *
     * Added in minor version 1.
     */
    const char* (*native_dir)(ModContext* ctx);
} HostService;

#ifdef __cplusplus
#include "mods/service.hpp"

template <>
struct dusk::mods::ServiceTraits<HostService> {
    static constexpr const char* id = HOST_SERVICE_ID;
    static constexpr uint16_t major_version = HOST_SERVICE_MAJOR;
    static constexpr uint16_t minor_version = HOST_SERVICE_MINOR;
};
#endif
