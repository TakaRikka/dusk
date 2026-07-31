#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#define SAVE_SERVICE_ID "dev.twilitrealm.dusklight.save"
#define SAVE_SERVICE_MAJOR 1u
#define SAVE_SERVICE_MINOR 0u

/* Handle for a save-observer registration. 0 is never a valid handle. */
typedef uint64_t SaveObserverHandle;

/* Total blob storage per mod per save slot. */
#define SAVE_BLOB_BUDGET_BYTES 65536u

/*
 * Per-save-slot mod data.
 *
 * Named binary blobs that travel with a save slot: stored in a host-owned sidecar next to the
 * emulated memory card, written when the game writes the card (in-game save, autosave), and
 * following file-select copy and erase. Blobs are namespaced per mod — two mods can use the
 * same name without collision. Format versioning inside a blob is the mod's own concern.
 *
 * Blob calls target the *current* slot, which exists from save load / new-save creation until
 * the player returns to file select; MOD_UNAVAILABLE otherwise. Data written between game
 * saves lives in memory only — like vanilla save state, it persists when the player saves.
 * A new save clears the slot's previous blob data before on_new_save fires.
 *
 * Staleness: the sidecar snapshots a checksum of each slot's vanilla save data at write time
 * and warns at load when they diverge (e.g. the card file was replaced externally); blobs are
 * still delivered — treat them with suspicion in that session.
 *
 * Callbacks run on the game thread. Registrations are owned by the calling mod and removed
 * automatically when it is disabled, reloaded, or fails.
 */

/* slot is the save-file index (0..2). */
typedef void (*SaveEventFn)(ModContext* ctx, uint32_t slot, void* user_data);

typedef struct SaveService {
    ServiceHeader header;

    /*
     * Write a named blob for the current slot (copied). Fails with MOD_UNAVAILABLE when no
     * slot is active or the mod's SAVE_BLOB_BUDGET_BYTES for the slot would be exceeded.
     */
    ModResult (*set_blob)(ModContext* ctx, const char* name, const void* data, size_t size);

    /*
     * Read a named blob from the current slot. With buf NULL, writes the blob's size to
     * *inout_size. Otherwise *inout_size is the buffer capacity in, bytes written out.
     * MOD_UNAVAILABLE when no slot is active or the blob does not exist.
     */
    ModResult (*get_blob)(ModContext* ctx, const char* name, void* buf, size_t* inout_size);

    /* Remove a named blob from the current slot. */
    ModResult (*delete_blob)(ModContext* ctx, const char* name);

    /*
     * Observe save lifecycle. Any callback may be NULL:
     *  - on_new_save: a new file was finalized in file select (name entry complete); the
     *    slot's blob store is empty — write initial blobs here. Like vanilla save data,
     *    nothing hits disk until the first in-game save.
     *  - on_save_loaded: a slot was loaded into the live game (file select or quick-load);
     *    blobs are readable.
     *  - on_save_written: the game persisted the slot (menu save or autosave); blob data
     *    captured with it is on disk.
     */
    ModResult (*observe_saves)(ModContext* ctx, SaveEventFn on_new_save,
        SaveEventFn on_save_loaded, SaveEventFn on_save_written, void* user_data,
        SaveObserverHandle* out_handle);

    /* Remove an observer previously registered by the calling mod. */
    ModResult (*unobserve_saves)(ModContext* ctx, SaveObserverHandle handle);

    /*
     * Read a named blob from any slot (0..2), including at file select when no slot is
     * active. Same buffer contract as get_blob. The calling mod's own namespace only.
     */
    ModResult (*peek_blob)(ModContext* ctx, uint32_t slot, const char* name, void* buf, size_t* inout_size);

} SaveService;

MOD_DECLARE_SERVICE(
    SaveService, svc_save, SAVE_SERVICE_ID, SAVE_SERVICE_MAJOR, SAVE_SERVICE_MINOR);
