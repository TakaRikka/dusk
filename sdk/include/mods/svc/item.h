#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#define ITEM_SERVICE_ID "dev.twilitrealm.dusklight.item"
#define ITEM_SERVICE_MAJOR 2u
#define ITEM_SERVICE_MINOR 2u

/* 0 is never a valid handle. */
typedef uint64_t ItemCheckHandle;
typedef uint64_t ItemGiveHandle;

/*
 * Item check resolution and inventory grants.
 *
 * Check names are case-sensitive. Resolvers must be free of side effects because previews may be
 * resolved repeatedly. For checks representing a non-item side effect, vanilla_item is 0xFF:
 * resolving to 0xFF preserves that side effect, while resolving to another item replaces it.
 *
 * Registrations and pending grants are removed when the calling mod is detached. Callbacks run on
 * the game thread. Check names are listed in <mods/items.h>.
 */

/* Host-owned callback data, valid only for the duration of the callback. */
typedef struct ItemCheckInfo {
    const char* name;
    const void* giver_actor; /* fopAc_ac_c*, or NULL when unavailable */
    uint8_t vanilla_item;
    uint8_t current_item;
    uint8_t current_display_item;
} ItemCheckInfo;

typedef struct ItemCheckResolution {
    uint8_t item;
    uint8_t display_item; /* leave unset (0xFF/NONE) to use the item */
} ItemCheckResolution;

/* Return true and write out_result to replace the result, or false to leave it unchanged. */
typedef bool (*ItemCheckResolveFn)(
    ModContext* ctx, const ItemCheckInfo* info, ItemCheckResolution* out_result, void* user_data);

typedef enum ItemGiveOrigin {
    ITEM_GIVE_ORIGIN_GAME = 0,
    ITEM_GIVE_ORIGIN_QUEUE = 1,
    ITEM_GIVE_ORIGIN_QUEUE_SILENT = 2,
} ItemGiveOrigin;

/* Host-owned callback data, valid only for the duration of the callback. */
typedef struct ItemGiveInfo {
    const char* check_name;  /* NULL when the grant is not attributed to a check */
    const void* giver_actor; /* fopAc_ac_c*, or NULL when unavailable */
    uint8_t item;
    uint8_t origin; /* ItemGiveOrigin */
} ItemGiveInfo;

typedef void (*ItemGiveObserveFn)(ModContext* ctx, const ItemGiveInfo* info, void* user_data);

enum {
    /* Apply the inventory change without a get-item demo. */
    ITEM_GIVE_SILENT = 1u << 0,
    /* Resolve item_no as the named check's vanilla item when the queue entry is dispatched. */
    ITEM_GIVE_RESOLVE = 1u << 1,
};

typedef struct ItemService {
    ServiceHeader header;

    /* Set or replace the calling mod's fixed override for name. */
    ModResult (*set_check_override)(ModContext* ctx, const char* name, uint8_t item_no);

    ModResult (*clear_check_override)(ModContext* ctx, const char* name);

    /* Register for name, or for every check when name is NULL. out_handle may be NULL. */
    ModResult (*set_check_resolver)(ModContext* ctx, const char* name, ItemCheckResolveFn fn,
        void* user_data, ItemCheckHandle* out_handle);

    ModResult (*clear_check_resolver)(ModContext* ctx, ItemCheckHandle handle);

    /* Resolve a live preview without granting an item or notifying give observers. */
    ModResult (*resolve_check)(
        ModContext* ctx, const char* name, uint8_t vanilla_item, uint8_t* out_item);

    /*
     * Add a grant to the global FIFO. check_name may be NULL unless ITEM_GIVE_RESOLVE is set.
     * Entries wait until gameplay is in a safe state and are cleared when the active save slot
     * changes.
     */
    ModResult (*give_item)(
        ModContext* ctx, const char* check_name, uint8_t item_no, uint32_t flags);

    /* Observe inventory grants. out_handle may be NULL. */
    ModResult (*observe_gives)(
        ModContext* ctx, ItemGiveObserveFn fn, void* user_data, ItemGiveHandle* out_handle);

    ModResult (*unobserve_gives)(ModContext* ctx, ItemGiveHandle handle);

    /* Minor version 2 */

    /* Resolve a live preview (item + display item) without granting an item or notifying give
     * observers. */
    ModResult (*resolve_check_full)(ModContext* ctx, const char* name, uint8_t vanilla_item,
        ItemCheckResolution* out_resolution);
} ItemService;

MOD_DECLARE_SERVICE(ItemService, svc_item, ITEM_SERVICE_ID, ITEM_SERVICE_MAJOR, ITEM_SERVICE_MINOR);
