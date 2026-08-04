#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#define STAGE_SERVICE_ID "dev.twilitrealm.dusklight.stage"
#define STAGE_SERVICE_MAJOR 1u
#define STAGE_SERVICE_MINOR 0u

typedef uint64_t StageActorHandle;

typedef struct StageService {
    ServiceHeader header;

    /* Replace actor record that matches record_crc with given record */
    ModResult (*patch_actor)(ModContext* ctx, const char* stage, uint8_t room, int8_t layer,
        uint32_t record_crc, const void* record, size_t record_size, StageActorHandle* out_handle);

    /* Remove actor record matching record_crc */
    ModResult (*delete_actor)(ModContext* ctx, const char* stage, uint8_t room, int8_t layer,
        uint32_t record_crc, StageActorHandle* out_handle);

    /* Add new actor record */
    ModResult (*add_actor)(ModContext* ctx, const char* stage, uint8_t room, int8_t layer,
        const void* record, size_t record_size, StageActorHandle* out_handle);

    /* Remove an edit previously registered by the calling mod */
    ModResult (*remove_actor_edit)(ModContext* ctx, StageActorHandle handle);
} StageService;

MOD_DECLARE_SERVICE(
    StageService, svc_stage, STAGE_SERVICE_ID, STAGE_SERVICE_MAJOR, STAGE_SERVICE_MINOR);
