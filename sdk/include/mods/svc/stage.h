#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#define STAGE_SERVICE_ID "dev.twilitrealm.dusklight.stage"
#define STAGE_SERVICE_MAJOR 1u
#define STAGE_SERVICE_MINOR 0u

/* 0 is never a valid handle. */
typedef uint64_t StageActorHandle;

/*
 * Runtime edits to Stage Info (.dzs/.dzr) data.
 */

typedef struct StageService {
    ServiceHeader header;

    /*
     * Actor Node (ACTR/TGSC/SCOB/Door) Editing:
     * stage must be a non-empty name of at most 8 characters. room 0xff and layer -1 match any room
     * or layer for patch and delete operations; add_actor requires a specific room. Later-loaded mods
     * win conflicts. record_crc is the CRC-32 of the unmodified record. Registrations are removed when
     * the calling mod is detached. out_handle may be NULL.
     */
    ModResult (*patch_actor)(ModContext* ctx, const char* stage, uint8_t room, int8_t layer,
        uint32_t record_crc, const void* record, size_t record_size, StageActorHandle* out_handle);

    ModResult (*delete_actor)(ModContext* ctx, const char* stage, uint8_t room, int8_t layer,
        uint32_t record_crc, StageActorHandle* out_handle);

    ModResult (*add_actor)(ModContext* ctx, const char* stage, uint8_t room, int8_t layer,
        const void* record, size_t record_size, StageActorHandle* out_handle);

    ModResult (*remove_actor_edit)(ModContext* ctx, StageActorHandle handle);
} StageService;

MOD_DECLARE_SERVICE(
    StageService, svc_stage, STAGE_SERVICE_ID, STAGE_SERVICE_MAJOR, STAGE_SERVICE_MINOR);
