#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_midna.h"
#include "d/d_meter2.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"

cXyz currentGamepadColor = {0, 0, 0};
cXyz finalGamepadColor = {0, 0, 0};
float lerpSpeed = 0.0f;
const cXyz duskColor = {30, 30, -30};

const cXyz heartColor1 = {255, 0, 0};
const cXyz heartColor2 = {155, 5, 5};
const cXyz heartColor3 = {55, 5, 5};

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

cXyz LerpColor(cXyz a, cXyz b, float t) {
    return {lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t)};
}

void FadeLED(cXyz newColor, float speed) {
    finalGamepadColor = newColor;
    lerpSpeed = speed / 30.0f;
}

void SetLED(cXyz newColor) {
    currentGamepadColor = newColor;
    finalGamepadColor = newColor;
}

void AddGamepadCurrentColor(cXyz addColor) {
    finalGamepadColor.x += addColor.x;
    finalGamepadColor.y += addColor.y;
    finalGamepadColor.z += addColor.z;
}

void daAlink_c::handleGamepadColor() {
    bool setColor = false;

    fopAc_ac_c* zhint = dComIfGp_att_getZHint();
    if (zhint != NULL) {
        FadeLED({50, 50, 175}, 2.0f);
        setColor = true;
    }

    u8 linkHp = Z2GetLink()->getLinkHp();
    if (linkHp <= 2) {
        FadeLED(heartColor1, 2.0f);
        setColor = true;
    } else if (linkHp <= 4) {
        FadeLED(heartColor2, 2.0f);
        setColor = true;
    } else if (linkHp <= 6) {
        FadeLED(heartColor3, 2.0f);
        setColor = true;
    }

    if (!setColor) {
        if (checkWolf()) {
            FadeLED({115, 115, 75}, 5.0f);
            setColor = true;
        } else {
            switch (dComIfGs_getSelectEquipClothes()) {
            case dItemNo_WEAR_KOKIRI_e:
                FadeLED({0, 100, 0}, 5.0f);
                setColor = true;
                break;
            case dItemNo_WEAR_ZORA_e:
                FadeLED({0, 0, 100}, 5.0f);
                setColor = true;
                break;
            case dItemNo_ARMOR_e:
                if (checkMagicArmorHeavy()) {
                    FadeLED({5, 100, 100}, 5.0f);
                } else {
                    FadeLED({100, 0, 5}, 5.0f);
                }
                setColor = true;
                break;
            default:
                FadeLED({235, 230, 115}, 5.0f);
                setColor = true;
                break;
            }
        }
    }

    if (dKy_darkworld_check()) {
        AddGamepadCurrentColor(duskColor);
    }

    if (finalGamepadColor.x > 255)
        finalGamepadColor.x = 255;
    if (finalGamepadColor.x < 0)
        finalGamepadColor.x = 0;

    if (finalGamepadColor.y > 255)
        finalGamepadColor.y = 255;
    if (finalGamepadColor.y < 0)
        finalGamepadColor.y = 0;

    if (finalGamepadColor.z > 255)
        finalGamepadColor.z = 255;
    if (finalGamepadColor.z < 0)
        finalGamepadColor.z = 0;

    currentGamepadColor = LerpColor(currentGamepadColor, finalGamepadColor, lerpSpeed);
    PADSetColor(PAD_1, (u8)currentGamepadColor.x, (u8)currentGamepadColor.y, (u8)currentGamepadColor.z);
}

void daAlink_c::handleWolfHowl() {
    if (checkWolf()) {
        if (!dusk::getSettings().game.sunsSong) {
            return;
        }

        // Check to see if Link has the ability to transform.
        if (!dComIfGs_isEventBit(dSv_event_flag_c::M_077)) {
            return;
        }

        // Ensure there is a proper pointer to the mMeterClass and mpMeterDraw structs in
        // g_meter2_info.
        const auto meterClassPtr = g_meter2_info.getMeterClass();
        if (!meterClassPtr) {
            return;
        }

        const auto meterDrawPtr = meterClassPtr->getMeterDrawPtr();
        if (!meterDrawPtr) {
            return;
        }

        // Ensure that link is not in a cutscene.
        if (checkEventRun()) {
            Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            return;
        }

        mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags = 0;

        // Ensure that the Z Button is not dimmed
        if (meterDrawPtr->getButtonZAlpha() != 1.f) {
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

        getWolfHowlMgrP()->setCorrectCurve(9);
        procWolfHowlDemoInit();
    }
}

void daAlink_c::handleQuickTransform() {
    if (!dusk::getSettings().game.enableQuickTransform) {
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

    // Ensure that link is not in a cutscene.
    if (checkEventRun()) {
        Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        return;
    }

    mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags = 0;

    // Don't allow quick transform while in the STAR tent.
    if (checkStageName("R_SP161")) {
        Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        return;
    }

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

bool daAlink_c::checkGyroAimItemContext() {
    if (checkWolf()) {
        return false;
    }

    switch (mProcID) {
    case PROC_BOW_SUBJECT:
    case PROC_BOOMERANG_SUBJECT:
    case PROC_COPY_ROD_SUBJECT:
    case PROC_HOOKSHOT_SUBJECT:
    case PROC_SWIM_HOOKSHOT_SUBJECT:
    case PROC_HORSE_BOW_SUBJECT:
    case PROC_HORSE_BOOMERANG_SUBJECT:
    case PROC_HORSE_HOOKSHOT_SUBJECT:
    case PROC_CANOE_BOW_SUBJECT:
    case PROC_CANOE_BOOMERANG_SUBJECT:
    case PROC_CANOE_HOOKSHOT_SUBJECT:
    case PROC_HOOKSHOT_ROOF_WAIT:
    case PROC_HOOKSHOT_ROOF_SHOOT:
    case PROC_HOOKSHOT_WALL_WAIT:
    case PROC_HOOKSHOT_WALL_SHOOT:
        return true;
    case PROC_IRON_BALL_SUBJECT:
        return itemButton() && mItemVar0.field_0x3018 == 2;
    default:
        return false;
    }
}
