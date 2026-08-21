#include "dusk/game_mode.hpp"
#include "JSystem/JUtility/JUTGamePad.h"
#include "aurora/lib/logging.hpp"
#include "dusk/config.hpp"
#include "dusk/ui/prelaunch.hpp"
#include "m_Do/m_Do_MemCard.h"

namespace dusk::gamemode {
namespace {
aurora::Module Log("dusk::gamemode");
}

GameModeManager g_GameModeManager;

GameModeManager::GameModeManager() {
    GameMode vanilla{kVanillaGameModeId, "Vanilla"};
    mRegisteredGameModes.emplace(vanilla.getId(), std::move(vanilla));
    mCurrentGameModeId = kVanillaGameModeId;
}

void GameModeManager::setGameModeToPrevious() {
    // Restore the previously selected game mode if still registered.
    GameModeId id = getSettings().game.lastSelectedGameModeId;
    if (!mRegisteredGameModes.contains(id)) {
        setCurrentGameMode(kVanillaGameModeId);
        return;
    }
    setCurrentGameMode(id);
}

void GameModeManager::registerGameMode(const GameMode& gameMode) {
    if (gameMode.getId().empty()) {
        Log.fatal("No game mode ID specified in GameModeManager::registerGameMode");
    }
    if (gameMode.getFullName().empty()) {
        Log.fatal("No display name specified for game mode {}", gameMode.getId());
    }

    if (mRegisteredGameModes.contains(gameMode.getId())) {
        Log.warn("Attempting to re-register existing game mode {}", gameMode.getId());
        return;
    }

    mRegisteredGameModes.emplace(gameMode.getId(), gameMode);
    ui::Prelaunch::refresh_menu_buttons();
}

void GameModeManager::unregisterGameMode(const GameModeId& gameModeId) {
    const auto& it = mRegisteredGameModes.find(gameModeId);
    if (it == mRegisteredGameModes.end()) {
        Log.warn("Attempting to unregister unknown game mode {}", gameModeId);
        return;
    }

    if (mCurrentGameModeId == gameModeId) {
        // Reset to prelaunch before unloading callbacks belonging to the active mod.
        ui::prelaunch_state().returnToPrelaunchOnReset = true;
        JUTGamePad::C3ButtonReset::sResetSwitchPushing = true;
        setCurrentGameMode(kVanillaGameModeId);
    }
    mRegisteredGameModes.erase(it);
    ui::Prelaunch::refresh_menu_buttons();
}

bool GameModeManager::setCurrentGameMode(const GameModeId& id) {
    if (mCurrentGameModeId == id) {
        return true;
    }
    if (!mRegisteredGameModes.contains(id)) {
        Log.warn("Attempting to configure unknown game mode {}", id);
        return false;
    }
    const GameMode* currentGameMode = getCurrentGameMode();
    if (currentGameMode) {
        currentGameMode->invokeOnDeactivatedFunction();
    }
    mCurrentGameModeId = id;

    currentGameMode = getCurrentGameMode();
    if (currentGameMode) {
        mDoMemCd_SetFileName(currentGameMode->getSaveName());
        if (!currentGameMode->invokeOnActivatedFunction()) {
            mCurrentGameModeId = kVanillaGameModeId;
            currentGameMode = getCurrentGameMode();
            mDoMemCd_SetFileName(currentGameMode->getSaveName());
            currentGameMode->invokeOnActivatedFunction();
            getSettings().game.lastSelectedGameModeId.setValue(kVanillaGameModeId);
            config::save();
            return false;
        }
    }
    getSettings().game.lastSelectedGameModeId.setValue(id);
    config::save();
    return true;
}

}  // namespace dusk::gamemode
