#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_midna.h"
#include "d/d_meter2.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"
#include "m_Do/m_Do_audio.h"

bool armorQuickToggleCooldown = false;
bool armorQuickToggleInitiated = false;
u32 armorQuickToggleCounter = 0;
u32 armorQuickToggleInitiatedTimer = 0;
u32 armorQuickToggleCooldownTimer = 0;

void daAlink_c::handleArmorsQuickToggle() {
    // By Captain Kitty Cat
    if (!dusk::getSettings().game.enableArmorsQuickToggle) {
        return;
    }

    // Have magic and zora armors
    if (!dComIfGs_isItemFirstBit(dItemNo_WEAR_ZORA_e) && !dComIfGs_isItemFirstBit(dItemNo_ARMOR_e)) {
        return;
    }

    // Be Human
    if (dComIfGs_getTransformStatus()) {
        return;
    }

    if (!armorQuickToggleCooldown && mDoCPd_c::getHoldR(PAD_1) && mDoCPd_c::getTrigX(PAD_1) && armorQuickToggleCounter < 2)
    {
        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags = 0;
        armorQuickToggleInitiated = true;
        armorQuickToggleCounter++;
    }

    if (armorQuickToggleInitiated) {
        armorQuickToggleInitiatedTimer++;
        if (armorQuickToggleInitiatedTimer >= 15) {
            armorQuickToggleInitiated = false;
            armorQuickToggleCooldown = true;
            armorQuickToggleInitiatedTimer = 0;
            armorQuickToggleCooldownTimer = 0;
            setClothesChange(0);
            mDoAud_seStart(Z2SE_SY_ITEM_SET_X, 0, 0, 0);
            if (armorQuickToggleCounter == 1) {
                if (dComIfGs_getSelectEquipClothes() != dItemNo_WEAR_ZORA_e) {
                    dComIfGs_setSelectEquipClothes(dItemNo_WEAR_ZORA_e);
                }
                else {
                    dComIfGs_setSelectEquipClothes(dItemNo_WEAR_KOKIRI_e);
                }
            } else if (armorQuickToggleCounter == 2 && dComIfGs_isItemFirstBit(dItemNo_ARMOR_e)) {
                if (dComIfGs_getSelectEquipClothes() != dItemNo_ARMOR_e) {
                    dComIfGs_setSelectEquipClothes(dItemNo_ARMOR_e);
                }
                else {
                    dComIfGs_setSelectEquipClothes(dItemNo_WEAR_KOKIRI_e);
                }
            }
            armorQuickToggleCounter = 0;
        }
    }

    if (armorQuickToggleCooldown) {
        armorQuickToggleCooldownTimer++;
        if (armorQuickToggleCooldownTimer >= 20) {
            armorQuickToggleCooldown = false;
            armorQuickToggleCooldownTimer = 0;
        }
    }
}

void daAlink_c::handleQuickTransform() {
    if (!dusk::getSettings().game.enableQuickTransform) {
        return;
    }

    // Ensure that link is not in a cutscene.
    if (checkEventRun()) {
        return;
    }

    // Check to see if Link has the ability to transform.
    if (!dComIfGs_isEventBit(dSv_event_flag_c::M_077)) {
        return;
    }

    // Ensure there is a proper pointer to the mMeterClass and mpMeterDraw structs in g_meter2_info.
    const auto meterClassPtr = g_meter2_info.getMeterClass();
    if (!meterClassPtr) {
        return;
    }

    const auto meterDrawPtr = meterClassPtr->getMeterDrawPtr();
    if (!meterDrawPtr) {
        return;
    }

    mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags = 0;

    // Ensure that the Z Button is not dimmed
    if (meterDrawPtr->getButtonZAlpha() != 1.f) {
        Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        return;
    }

    // The game will crash if trying to quick transform while holding the Ball and Chain
    if (mEquipItem == dItemNo_IRONBALL_e) {
        Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        return;
    }

    // Use the game's default checks for if the player can currently transform
    if (!m_midnaActor->checkMetamorphoseEnableBase()) {
        Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        return;
    }

    bool canTransform = false;

    if (mLinkAcch.ChkGroundHit() && !checkModeFlg(MODE_PLAYER_FLY) && !checkMagneBootsOn()) {
        if (!checkForestOldCentury()) {
            if (checkMidnaRide()) {
                if ((checkWolf() &&
                     (checkModeFlg(MODE_UNK_1000) || dComIfGp_checkPlayerStatus0(0, 0x10))) ||
                    (!checkWolf() &&
                     (checkEventRun() || getMidnaActor()->checkMetamorphoseEnable()) &&
                     (checkModeFlg(4) || dComIfGp_checkPlayerStatus0(0, 0x10))))
                {
                    canTransform = true;
                }
            }
        }
    }

    if (!canTransform)
    {
        Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        return;
    }

    OSReport("Running quick transform!");
    procCoMetamorphoseInit();
}
