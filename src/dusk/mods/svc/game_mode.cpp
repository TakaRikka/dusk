#include "mods/svc/game_mode.h"
#include "dusk/game_mode.hpp"

#include "config.hpp"
#include "internal.hpp"
#include "registry.hpp"

#include "aurora/lib/logging.hpp"
#include "dusk/mod_loader.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "fmt/format.h"

#include <algorithm>
#include <cctype>
#include <exception>
#include <string>
#include <unordered_map>
#include <vector>

namespace dusk::mods::svc::game_mode_impl {
namespace {

aurora::Module Log("dusk::mods::game_mode");

// Track ownership of mod ID to game modes
std::unordered_map<std::string, std::vector<std::string>> s_gameModesByMod;

template <typename Fn>
bool invoke_mod_callback(LoadedMod& mod, const char* what, Fn&& fn) {
    if (!mod.active) {
        return false;
    }

    ModError error = MOD_ERROR_INIT;
    ModResult result = MOD_OK;
    try {
        result = fn(&error);
    } catch (const std::exception& exception) {
        fail_mod(mod, MOD_ERROR, fmt::format("exception in {}: {}", what, exception.what()));
        return false;
    } catch (...) {
        fail_mod(mod, MOD_ERROR, fmt::format("unknown exception in {}", what));
        return false;
    }

    if (result != MOD_OK && mod.active) {
        fail_mod(mod, result,
            error.message[0] != '\0' ?
                error.message :
                fmt::format("{} failed with result {}", what, static_cast<int>(result)));
    }
    return result == MOD_OK && mod.active;
}

gamemode::GameMode::Callback wrap_callback(
    LoadedMod& mod, GameModeCallback callback, void* userData, const char* what) {
    return [&mod, callback, userData, what] {
        return invoke_mod_callback(
            mod, what, [callback, userData](ModError* error) { return callback(userData, error); });
    };
}

gamemode::GameMode::NewSaveSelectCallback wrap_new_save_select_callback(
    LoadedMod& mod, GameModeNewSaveSelectCallback callback, void* userData, const char* what) {
    return [&mod, callback, userData, what](GameModeNewSaveState* state) {
        return invoke_mod_callback(
            mod, what, [=](ModError* error) { return callback(userData, state, error); });
    };
}

std::string get_mod_game_mode_id(ModContext* ctx, const std::string& id) {
    // Include the mod ID to prevent clashes and normalize to lowercase
    std::string fullId = id + "_" + ctx->mod->metadata.id;
    std::transform(fullId.begin(), fullId.end(), fullId.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return fullId;
}

void game_mode_remove_mod(LoadedMod& mod) {
    const auto it = s_gameModesByMod.find(mod.metadata.id);
    if (it != s_gameModesByMod.end()) {
        for (const auto& id : it->second) {
            gamemode::getGameModeManager().unregisterGameMode(id);
        }
        s_gameModesByMod.erase(it);
    }
}
}  // namespace

ModResult register_game_mode(ModContext* ctx, const GameModeDesc* desc) {
    auto* owner = mod_from_context(ctx);
    if (owner == nullptr || desc == nullptr || desc->struct_size < sizeof(GameModeDesc)) {
        return MOD_INVALID_ARGUMENT;
    }

    std::string id;
    if (!desc->game_mode_id) {
        Log.error("Attempted to register a game mode with a null ID");
        return MOD_ERROR;
    }
    id = desc->game_mode_id;
    if (id.empty()) {
        Log.error("Attempted to register a game mode with an empty ID");
        return MOD_ERROR;
    }
    id = get_mod_game_mode_id(ctx, id);

    std::string fullName;
    if (!desc->full_name) {
        Log.warn("Game mode {} has no display name; using its ID", id);
        fullName = id;
    } else {
        fullName = desc->full_name;
        if (fullName.empty()) {
            Log.warn("Game mode {} has an empty display name; using its ID", id);
            fullName = id;
        }
    }

    gamemode::GameMode mode{id, fullName, desc->save_name};
    if (desc->on_activated) {
        mode.mOnActivatedFunction = wrap_callback(
            *owner, desc->on_activated, desc->user_data, "game mode activation callback");
    }
    if (desc->on_deactivated) {
        mode.mOnDeactivatedFunction = wrap_callback(
            *owner, desc->on_deactivated, desc->user_data, "game mode deactivation callback");
    }
    if (desc->on_play) {
        mode.mOnPlayFunction =
            wrap_callback(*owner, desc->on_play, desc->user_data, "game mode play callback");
    }
    if (desc->on_save_loaded) {
        mode.mOnSaveLoadedFunction = wrap_callback(
            *owner, desc->on_save_loaded, desc->user_data, "game mode save-loaded callback");
    }
    if (desc->on_new_save) {
        mode.mOnNewSaveFunction = wrap_callback(
            *owner, desc->on_new_save, desc->user_data, "game mode new-save callback");
    }
    if (desc->on_new_save_select) {
        mode.mOnNewSaveSelectFunction = wrap_new_save_select_callback(*owner,
            desc->on_new_save_select, desc->user_data, "game mode new-save selection callback");
    }
    if (desc->on_game_reset) {
        mode.mOnGameResetFunction =
            wrap_callback(*owner, desc->on_game_reset, desc->user_data, "game mode reset callback");
    }
    if (desc->on_tick) {
        mode.mOnTickFunction =
            wrap_callback(*owner, desc->on_tick, desc->user_data, "game mode tick callback");
    }

    gamemode::getGameModeManager().registerGameMode(mode);
    s_gameModesByMod[ctx->mod->metadata.id].push_back(id);
    return MOD_OK;
}

ModResult unregister_game_mode(ModContext* ctx, const char* id) {
    std::string fullId = get_mod_game_mode_id(ctx, id);
    gamemode::getGameModeManager().unregisterGameMode(fullId);

    // Remove the game mode from the ownership map
    auto it = s_gameModesByMod.find(ctx->mod->metadata.id);
    if (it != s_gameModesByMod.end()) {
        std::erase(it->second, fullId);
    }
    return MOD_OK;
}

ModResult is_active(ModContext* ctx, const char* gameModeId, bool* out_active) {
    *out_active =
        gamemode::getGameModeManager().isCurrentGameMode(get_mod_game_mode_id(ctx, gameModeId));
    return MOD_OK;
}

}  // namespace dusk::mods::svc::game_mode_impl

namespace dusk::mods::svc {
namespace {

constexpr GameModeService s_gamemodeService{
    .header = SERVICE_HEADER(GameModeService, GAME_MODE_SERVICE_MAJOR, GAME_MODE_SERVICE_MINOR),
    .register_game_mode = game_mode_impl::register_game_mode,
    .unregister_game_mode = game_mode_impl::unregister_game_mode,
    .is_active = game_mode_impl::is_active,
};

}  // namespace

constinit const ServiceModule g_gamemodeModule{
    .id = GAME_MODE_SERVICE_ID,
    .majorVersion = GAME_MODE_SERVICE_MAJOR,
    .minorVersion = GAME_MODE_SERVICE_MINOR,
    .service = &s_gamemodeService,
    .modDeactivating = game_mode_impl::game_mode_remove_mod,
};

}  // namespace dusk::mods::svc
