#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#define SAVE_SERVICE_ID "dev.twilitrealm.dusklight.save"
#define SAVE_SERVICE_MAJOR 1u
#define SAVE_SERVICE_MINOR 0u

/* 0 is never a valid handle. */
typedef uint64_t SaveObserverHandle;

/* Maximum combined blob size per mod and save slot. */
#define SAVE_BLOB_BUDGET_BYTES 65536u

/*
 * Per-slot mod storage.
 *
 * Blobs are scoped to the calling mod and saved alongside each slot. Current-slot calls return
 * MOD_UNAVAILABLE when no slot is active.
 *
 * Callbacks run on the game thread. Observer registrations are removed when the calling mod is
 * detached.
 */

/* slot is the save-file index (0..2). */
typedef void (*SaveEventFn)(ModContext* ctx, uint32_t slot, void* user_data);

typedef struct SaveService {
    ServiceHeader header;

    /* Store a copy in the current slot. Returns MOD_UNAVAILABLE if the limit would be exceeded. */
    ModResult (*set_blob)(ModContext* ctx, const char* name, const void* data, size_t size);

    /*
     * Read a blob from the current slot. Pass NULL for buf to query its size. Otherwise,
     * inout_size is the buffer capacity on input and the blob size on success. Returns
     * MOD_UNAVAILABLE if the blob does not exist.
     */
    ModResult (*get_blob)(ModContext* ctx, const char* name, void* buf, size_t* inout_size);

    ModResult (*delete_blob)(ModContext* ctx, const char* name);

    /*
     * Register save lifecycle callbacks. At least one callback is required. on_new_save runs
     * after clearing the slot's blobs, on_save_loaded after activating the slot, and
     * on_save_written after a successful game save. out_handle may be NULL.
     */
    ModResult (*observe_saves)(ModContext* ctx, SaveEventFn on_new_save, SaveEventFn on_save_loaded,
        SaveEventFn on_save_written, void* user_data, SaveObserverHandle* out_handle);

    ModResult (*unobserve_saves)(ModContext* ctx, SaveObserverHandle handle);

    /* Read the calling mod's blob from any slot. Uses the get_blob buffer contract. */
    ModResult (*peek_blob)(
        ModContext* ctx, uint32_t slot, const char* name, void* buf, size_t* inout_size);

} SaveService;

MOD_DECLARE_SERVICE(SaveService, svc_save, SAVE_SERVICE_ID, SAVE_SERVICE_MAJOR, SAVE_SERVICE_MINOR);
