#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_midna.h"
#include "d/d_meter2.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"
#include "m_Do/m_Do_audio.h"

bool armorQuickToggleVar1 = false;
bool armorQuickToggleVar2 = false;
uint32_t countyer = 0;
uint32_t armorQuickToggleTimer1 = 0;
uint32_t armorQuickToggleTimer2 = 0;

void daAlink_c::handleArmorsQuickToggle() {
    if (!dusk::getSettings().game.enableArmorsQuickToggle) return;

    // temporary if statement code for automatically having magic and zora armors unlocked to test the mod
    if (!dComIfGs_isItemFirstBit(dItemNo_WEAR_ZORA_e)) {
        dComIfGs_onItemFirstBit(dItemNo_WEAR_ZORA_e); dComIfGs_onItemFirstBit(dItemNo_ARMOR_e);
    }

    // Have magic and zora armors
    if (!dComIfGs_isItemFirstBit(dItemNo_WEAR_ZORA_e) && !dComIfGs_isItemFirstBit(dItemNo_ARMOR_e)) return;

    // Be Human
    if (dComIfGs_getTransformStatus()) return;

    if (!armorQuickToggleVar1 && mDoCPd_c::getTrigDown(PAD_1) && countyer < 2)
    {
        armorQuickToggleVar2 = true;
        countyer++;
    }

    if (armorQuickToggleVar2) {
        armorQuickToggleTimer1++;
        if (armorQuickToggleTimer1 >= 15) {
            armorQuickToggleVar2 = false;
            armorQuickToggleVar1 = true;
            armorQuickToggleTimer1 = 0;
            armorQuickToggleTimer2 = 0;
            setClothesChange(0);
            mDoAud_seStart(0x4F, 0, 0, 0);
            if (countyer == 1) {
                if (dComIfGs_getSelectEquipClothes() != dItemNo_WEAR_ZORA_e) dComIfGs_setSelectEquipClothes(dItemNo_WEAR_ZORA_e);
                else dComIfGs_setSelectEquipClothes(dItemNo_WEAR_KOKIRI_e);
            } else if (countyer == 2 && dComIfGs_isItemFirstBit(dItemNo_ARMOR_e)) {
                if (dComIfGs_getSelectEquipClothes() != dItemNo_ARMOR_e) dComIfGs_setSelectEquipClothes(dItemNo_ARMOR_e);
                else dComIfGs_setSelectEquipClothes(dItemNo_WEAR_KOKIRI_e);
            }
            countyer = 0;
        }
    }
    if (armorQuickToggleVar1) {
        armorQuickToggleTimer2++;
        if (armorQuickToggleTimer2 >= 20) {
            armorQuickToggleVar1 = false;
            armorQuickToggleTimer2 = 0;
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
