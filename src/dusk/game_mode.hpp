#pragma once

#include "d/d_file_select.h"
#include "mods/svc/game_mode.h"

#include <functional>
#include <map>
#include <string>
#include <utility>

namespace dusk::gamemode {
using GameModeId = std::string;

constexpr const char* kVanillaGameModeId = "vanilla";
constexpr const char* kDefaultGameModeSaveName = "gczelda2";

// Holds a game mode definition and its lifecycle callbacks.
class GameMode {
public:
    using Callback = std::function<bool()>;
    using NewSaveSelectCallback = std::function<bool(GameModeNewSaveState* state)>;

    GameMode(GameModeId id, std::string fullName, std::string saveName = {})
        : mId{std::move(id)}, mFullName{std::move(fullName)},
          mSaveName{saveName.empty() ? kDefaultGameModeSaveName : std::move(saveName)} {}
    const GameModeId& getId() const { return mId; }
    const std::string& getFullName() const { return mFullName; }
    const std::string& getSaveName() const { return mSaveName; }

    GameModeId mId;
    std::string mFullName;
    std::string mSaveName;

    bool invokeOnActivatedFunction() const {
        if (mOnActivatedFunction) {
            return mOnActivatedFunction();
        }
        return true;
    }

    bool invokeOnDeactivatedFunction() const {
        if (mOnDeactivatedFunction) {
            return mOnDeactivatedFunction();
        }
        return true;
    }

    bool invokeOnPlayFunction() const {
        if (mOnPlayFunction) {
            return mOnPlayFunction();
        }
        return true;
    }

    bool invokeOnSaveLoadedFunction() const {
        if (mOnSaveLoadedFunction) {
            return mOnSaveLoadedFunction();
        }
        return true;
    }

    bool invokeOnNewSaveFunction() const {
        if (mOnNewSaveFunction) {
            return mOnNewSaveFunction();
        }
        return true;
    }

    bool invokeOnNewSaveSelectFunction(GameModeNewSaveState* state) const {
        *state = GAME_MODE_STATE_PENDING;
        if (mOnNewSaveSelectFunction) {
            if (mOnNewSaveSelectFunction(state)) {
                return true;
            }
            *state = GAME_MODE_STATE_RETURN;
            return false;
        }
        *state = GAME_MODE_STATE_PROCEED;
        return true;
    }

    bool invokeOnGameResetFunction() const {
        if (mOnGameResetFunction) {
            return mOnGameResetFunction();
        }
        return true;
    }

    bool invokeOnTickFunction() const {
        if (mOnTickFunction) {
            return mOnTickFunction();
        }
        return true;
    }

    Callback mOnActivatedFunction;
    Callback mOnDeactivatedFunction;
    Callback mOnPlayFunction;
    Callback mOnSaveLoadedFunction;
    Callback mOnNewSaveFunction;
    NewSaveSelectCallback mOnNewSaveSelectFunction;
    Callback mOnGameResetFunction;
    Callback mOnTickFunction;
};

class GameModeManager {
public:
    GameModeManager();
    void registerGameMode(const GameMode& gameMode);
    void unregisterGameMode(const GameModeId& gameModeId);

    const GameMode* getCurrentGameMode() const {
        const auto& it = mRegisteredGameModes.find(mCurrentGameModeId);
        return it != mRegisteredGameModes.end() ? &it->second :
                                                  &mRegisteredGameModes.at(kVanillaGameModeId);
    }
    bool isCurrentGameMode(const GameModeId& id) const {
        const GameMode* gameMode = getCurrentGameMode();
        if (gameMode && gameMode->getId() == id) {
            return true;
        }
        return false;
    }
    bool setCurrentGameMode(const GameModeId& id);
    void setGameModeToPrevious();

    const std::map<GameModeId, GameMode>& getRegisteredGameModes() const {
        return mRegisteredGameModes;
    }

private:
    GameModeId mCurrentGameModeId;
    std::map<GameModeId, GameMode> mRegisteredGameModes;
};

extern GameModeManager g_GameModeManager;

inline GameModeManager& getGameModeManager() {
    return g_GameModeManager;
}

}  // namespace dusk::gamemode
