#include "dusk/autosave.h"

#include "dusk/config.hpp"
#include "dusk/ui/ui.hpp"
#include "imgui/ImGuiConsole.hpp"

bool shouldAutoSave = false;
u8 mSaveBuffer[QUEST_LOG_SIZE * 3];
u8 mAutoSaveProc = 0;
int autoSaveWriteState = 0;

typedef void (*AutoSaveFuncs)();
static AutoSaveFuncs AutoSaveFuncsProc[] = {
    noAutoSave, enterAutoSave, autoSaving, waitingForWrite, endAutoSave,
};

void noAutoSave() {}

bool canAutoSave() {
    daAlink_c* player = (daAlink_c*)daAlink_getAlinkActorClass();
    if (player == nullptr) {
        return false;
    }

    if (player->checkCargoCarry() || player->checkCanoeRide()) {
        return false;
    }

    return dusk::getSettings().game.autoSave && shouldAutoSave && mAutoSaveProc == 0 &&
           strcmp(dComIfGp_getStartStageName(), "F_SP102") != 0 &&
           strcmp(dComIfGp_getStartStageName(), "F_SP112") != 0;
}

void triggerAutoSave() {
    if (canAutoSave())
    {
        mAutoSaveProc = 1;
    }
}

void updateAutoSave() {
    (AutoSaveFuncsProc[mAutoSaveProc])();
}

bool writeAutoSave() {
    stage_stag_info_class* stagInfo = dComIfGp_getStageStagInfo();
    if (stagInfo == nullptr) {
        return false;
    }
    int stageNo = dStage_stagInfo_GetSaveTbl(stagInfo);

    dComIfGs_putSave(stageNo);
    dComIfGs_setMemoryToCard(mSaveBuffer, dComIfGs_getDataNum());
    mDoMemCdRWm_SetCheckSumGameData(mSaveBuffer, dComIfGs_getDataNum());

    // Save randomizer hash. Only do this while the randomizer is actually active (which already
    // requires a non-empty hash) - this used to run unconditionally, so if randomizer_GetContext()
    // ever got reset to a blank context for any reason (e.g. a failed/interrupted Archipelago
    // reconnect) while a file kept autosaving, the next autosave would silently overwrite that
    // file's stored seed hash with an empty string, permanently unlinking it from its Archipelago
    // session with no way to recover other than reconnecting fresh and losing the association.
    if (randomizer_IsActive()) {
        dusk::getSettings().randomizer.seedHashes[dComIfGs_getDataNum()].setValue(randomizer_GetContext().mHash);
        dusk::config::Save();
        g_randomizerState.mFileNum = dComIfGs_getDataNum();
    }

    u8* save = mSaveBuffer;
    for (int i = 0; i < 3; i++) {
        mDoMemCdRWm_TestCheckSumGameData(save);
        save += QUEST_LOG_SIZE;
    }

    g_mDoMemCd_control.save(mSaveBuffer, sizeof(mSaveBuffer), 0);
    return true;
}

void autoSaving() {
    int cardState = g_mDoMemCd_control.LoadSync(mSaveBuffer, sizeof(mSaveBuffer), 0);
    if (cardState != 0) {
        if (cardState == 2) {
            mAutoSaveProc = 1;
        } else if (cardState == 1) {
            if (writeAutoSave()) {
                mAutoSaveProc = 3;
            }
        }
    }
}

void enterAutoSave() {
    u32 cardStatus = g_mDoMemCd_control.getStatus(0);

    if (cardStatus != 14) {
        switch (cardStatus) {
        case 2:
            g_mDoMemCd_control.load();
            mAutoSaveProc = 2;
            break;
        case 3:
        case 4:
        case 5:
            break;
        default:
            mAutoSaveProc = 0;
            break;
        }
    }
}

void waitingForWrite() {
    autoSaveWriteState = g_mDoMemCd_control.SaveSync();

    if (autoSaveWriteState == 2) {
        mAutoSaveProc = 0;
    } else if (autoSaveWriteState == 1) {
        mAutoSaveProc = 4;
    }
}

void endAutoSave() {
    dusk::ui::push_toast({
        .type = "autosave",
        .duration = std::chrono::milliseconds(1500),
    });
    mAutoSaveProc = 0;
}

void toggleAutoSave(bool enabled) {
    shouldAutoSave = enabled;
}