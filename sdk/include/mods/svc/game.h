#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

/*
 * The mod SDK imports this service automatically for mods built with FEATURES game; service-only
 * and asset-only mods do not require it.
 *
 * Major version is the game-code ABI epoch: it is bumped when game-visible struct or vtable layouts
 * change incompatibly (e.g. a TARGET_PC field added to an existing game struct). The loader's
 * ordinary version check then fails mods built against the old epoch with a clear message instead
 * of letting them corrupt memory.
 */
#define GAME_SERVICE_ID "dev.twilitrealm.dusklight.game"
#define GAME_SERVICE_MAJOR 1u
#define GAME_SERVICE_MINOR 0u

typedef struct GameService {
    ServiceHeader header;
} GameService;

MOD_DECLARE_SERVICE(GameService, svc_game, GAME_SERVICE_ID, GAME_SERVICE_MAJOR, GAME_SERVICE_MINOR);
