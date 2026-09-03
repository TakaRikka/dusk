#pragma once

#include <mods/api.h>
#include <mods/svc/config.h>

#define GAME_MODE_SERVICE_ID "dev.twilitrealm.dusklight.gamemode"
#define GAME_MODE_SERVICE_MAJOR 1u
#define GAME_MODE_SERVICE_MINOR 0u

/* MOD_OK on success; failures disable the mod. */
typedef ModResult (*GameModeCallback)(void* user_data, ModError* out_error);

typedef enum GameModeNewSaveState {
    GAME_MODE_STATE_PENDING = 0,
    GAME_MODE_STATE_PROCEED,
    GAME_MODE_STATE_RETURN,
} GameModeNewSaveState;

/* Set state to PROCEED after custom UI completes, or RETURN to cancel.
 * State pointer is valid until selection completes. */
typedef ModResult (*GameModeNewSaveSelectCallback)(
    void* user_data, GameModeNewSaveState* state, ModError* out_error);

typedef struct {
    uint32_t struct_size;
    const char* game_mode_id;
    const char* full_name;
    const char save_name[32];         // Empty uses default (gczelda2); max 31 chars
    void* user_data;                  // Pointer will be passed to all callbacks
    GameModeCallback on_activated;    // Called when the game mode is selected
    GameModeCallback on_deactivated;  // Called when the game mode is deselected
    GameModeCallback on_play;         // Called when play is pressed
    GameModeCallback on_save_loaded;  // Called whenever a save file is loaded
    GameModeCallback on_new_save;     // Called when a new save is created
    GameModeNewSaveSelectCallback on_new_save_select;
    GameModeCallback on_game_reset;  // Called when the game is reset
    GameModeCallback on_tick;        // Called on every game tick while active
} GameModeDesc;

#define GAME_MODE_DESC_INIT {sizeof(GameModeDesc)}

typedef struct GameModeService {
    ServiceHeader header;
    ModResult (*register_game_mode)(ModContext* ctx, const GameModeDesc* desc);
    ModResult (*unregister_game_mode)(ModContext* ctx, const char* id);
    ModResult (*is_active)(ModContext* ctx, const char* game_mode_id, bool* out_active);
} GameModeService;

MOD_DECLARE_SERVICE(GameModeService, svc_game_mode, GAME_MODE_SERVICE_ID, GAME_MODE_SERVICE_MAJOR,
    GAME_MODE_SERVICE_MINOR);
