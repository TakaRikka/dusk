#include "Z2AudioLib/Z2SeMgr.h"
#include "Z2AudioLib/Z2Audience.h"
#include "Z2AudioLib/Z2Calc.h"
#include "Z2AudioLib/Z2LinkMgr.h"
#include "Z2AudioLib/Z2SoundMgr.h"
#include "Z2AudioLib/Z2SceneMgr.h"
#include "Z2AudioLib/Z2SpeechMgr2.h"
#include "Z2AudioLib/Z2StatusMgr.h"
#include "Z2AudioLib/Z2Param.h"
#include "Z2AudioLib/Z2SeqMgr.h"
#include "JSystem/J3DU/J3DUD.h"

Z2SeMgr::Z2SeMgr() : JASGlobalInstance(true), mSoundHandles(mSoundHandle, 24) {
    field_0x3c0 = 0;
    field_0x3c1 = 0;
    field_0x3c2 = 0;
    field_0x3c3 = 0;
    field_0x3c4 = 0;
    field_0x3c5 = 0;

    field_0x3c9 = 0;
    field_0x3ca = 0;
    field_0x3cb = 0;
    mCrowdSize = 0;
}

Z2MultiSeObj::Z2MultiSeObj() {
    soundID_ = 0xFFFFFFFF;
    reverb_ = 0;
}

void Z2SeMgr::initSe() {
    #if VERSION == VERSION_SHIELD_DEBUG
    dbg_field_0x68.releaseSound();
    dbg_field_0x6c = 0.5f;
    dbg_field_0x70 = 0.0f;
    dbg_field_0x74 = 0.0f;
    dbg_field_0x78 = 0.0f;
    #endif

    for (u8 i = 0; i < 10; i++) {
        mLevelObjSe[i].resetMultiSePos();
        mLevelObjSe[i].soundID_ = 0xFFFFFFFF;
        mLevelObjSe[i].reverb_ = 0;
    }

    mLevelObjectSeCount = 0;

    for (u8 i = 0; i < 10; i++) {
        mMultiTriggerSe[i].resetMultiSePos();
        mMultiTriggerSe[i].soundID_ = 0xFFFFFFFF;
        mMultiTriggerSe[i].reverb_ = 0;
    }

    mMultiTriggerSeCount = 0;
    resetModY();
}

void Z2SeMgr::resetModY() {
    for (int i = 0; i < 8; i++) {
        mModY[i] = NULL;
    }
}

void Z2SeMgr::modHeightAtCamera(const Vec** param_0) {
    if (*param_0 != NULL) {
        int idx = 9999;
        for (int i = 0; i < 8; i++) {
            if (mModY[i] == *param_0) {
                idx = i;
                break;
            }
        }

        if (idx == 9999) {
            int i = 0;
            for (i = 0; i < 8; i++) {
                if (mModY[i] == NULL) {
                    idx = i;
                    mModY[i] = *param_0;
                    break;
                }
            }

            if (i == 8) {
                JUT_WARN_DEVICE(126, 1, "%s", "Z2SeMgr: cannot mod cam_y");
                return;
            }
        }

        field_0x340[idx] = **param_0;
        Vec cam_pos = Z2GetAudience()->getAudioCamPos(0);
        if (field_0x340[idx].y > cam_pos.y) {
            field_0x340[idx].y = cam_pos.y;
        }

        *param_0 = &field_0x340[idx];
    }
}

void Z2SeMgr::incrCrowdSize() {
    mCrowdSize++;

    if (mCrowdSize > 100)
        mCrowdSize = 100;
}

void Z2SeMgr::decrCrowdSize() {
    mCrowdSize--;

    //! @bug probably copypasta from incrCrowdSize(), but semantically it's still correct:
    //! mCrowdSize is a u8, so an underflow would result in mCrowdSize > 100, triggering the clamp.
    if (mCrowdSize > 100)
        mCrowdSize = 0;
}

bool Z2SeMgr::seStart(JAISoundID soundID, const Vec* posPtr, u32 param_2, s8 reverb,
                      f32 pitch, f32 volume, f32 pan, f32 dolby, u8 param_8) {
    if (soundID == 0xFFFFFFFF) {
        return false;
    }

    if (param_8 != 1) {
        switch (soundID) {
        case Z2SE_OBJ_L8_STAIR_ON:
        case Z2SE_OBJ_L8_STAIR_MV_UP:
        case Z2SE_OBJ_L8_STAIR_MV_DW:
        case Z2SE_OBJ_L8_STAIR_ST_UP:
        case Z2SE_OBJ_L8_STAIR_ST_DW:
        case Z2SE_OBJ_L8_L_BALL_SW_ON:
        case Z2SE_OBJ_L8_L_LIFT_ON:
        case Z2SE_OBJ_GANON_BARRIER_APPR:
            setMultiTriggerSE(soundID, const_cast<Vec*>(posPtr), reverb);
            return false;
        }
    }

    switch (soundID) {
    case Z2SE_QUIT_GAME:
        Z2GetStatusMgr()->menuOut();
        break;
    case Z2SE_OBJ_DOOR_STEEL_BAR:
        seStop(Z2SE_OBJ_DOOR_STEEL_BAR, 0);
        break;
    case JA_SE_OBJ_STN_DOOR_MOVE_U:
        seStop(JA_SE_OBJ_STN_DOOR_MOVE_U, 0);
        seStop(JA_SE_OBJ_STN_DOOR_MOVE_D, 0);
        break;
    case JA_SE_OBJ_STN_DOOR_STOP_D:
        seStop(JA_SE_OBJ_STN_DOOR_MOVE_U, 0);
        seStop(JA_SE_OBJ_STN_DOOR_MOVE_D, 0);
        break;
    case JA_SE_OBJ_STN_DOOR_MOVE_D:
    case JA_SE_OBJ_STN_DOOR_STOP_U:
        seStop(JA_SE_OBJ_STN_DOOR_MOVE_U, 0);
        seStop(JA_SE_OBJ_STN_DOOR_MOVE_D, 0);
        break;
    case Z2SE_OBJ_WOOD_DOOR_ROLLOPEN:
    case Z2SE_OBJ_WOOD_DOOR_ROLLOPEN2:
    case Z2SE_OBJ_WOOD_DR_OP_MDN:
    case Z2SE_OBJ_WOOD_DR_OP_MDN_FX:
        seStop(Z2SE_OBJ_WOOD_DOOR_ROLLOPEN, 0);
        seStop(Z2SE_OBJ_WOOD_DOOR_ROLLOPEN2, 0);
        seStop(Z2SE_OBJ_WOOD_DOOR_ROLLCLOSE, 0);
        break;
    case Z2SE_OBJ_WOOD_DOOR_CLOSE_STOP:
        seStop(Z2SE_OBJ_WOOD_DOOR_ROLLOPEN, 0);
        seStop(Z2SE_OBJ_WOOD_DOOR_ROLLOPEN2, 0);
        seStop(Z2SE_OBJ_WOOD_DOOR_ROLLCLOSE, 0);
        break;
    case Z2SE_OBJ_WOOD_DOOR_ROLLCLOSE:
    case Z2SE_OBJ_WOOD_DOOR_OPEN_STOP:
        seStop(Z2SE_OBJ_WOOD_DOOR_ROLLOPEN, 0);
        seStop(Z2SE_OBJ_WOOD_DOOR_ROLLOPEN2, 0);
        seStop(Z2SE_OBJ_WOOD_DOOR_ROLLCLOSE, 0);
        break;
    case Z2SE_SY_DUMMY:
    case Z2SE_OBJ_LUPY_BOUND:
    case Z2SE_OBJ_MAGIC_POT_BOUND:
    case Z2SE_OBJ_ARROW_BOUND:
        if (param_2 >= 100) {
            param_2 = 100;
        }
        if (param_2 == 0) {
            return false;
        }
        param_2 = param_2 * param_2;
        volume = param_2 / 10000.0f;
        break;
    case Z2SE_OBJ_BOMB_BOUND:
        if (volume >= 100.0f) {
            volume = 100.0f;
        }
        if (volume <= 0.0f) {
            return false;
        }
        volume = (volume * volume) / 10000.0f;
        break;
    case Z2SE_OBJ_KEY_BOUND:
        if (param_2 == 0) {
            return false;
        }
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 100.0f, 0.2f, 0.0f, 1.0f,
                                         Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_MAGIC_METER_DEC:
        if (mSoundHandles.getHandleSoundID(Z2SE_MAGIC_METER_DEC) != NULL || mSoundHandles.getHandleSoundID(Z2SE_MAGIC_METER_FINISH) != NULL) {
            return false;
        }
        break;
    case Z2SE_MAGIC_METER_FINISH:
        seStop(Z2SE_MAGIC_METER_DEC, 0);
        break;
    case Z2SE_ITEM_RING_IN:
    case Z2SE_SY_MENU_IN:
    case Z2SE_SY_MAP_OPEN_L:
        Z2GetStatusMgr()->menuIn();
        break;
    case Z2SE_ITEM_RING_OUT:
    case Z2SE_SY_MENU_OUT:
    case Z2SE_SY_CONTINUE_OK:
    case Z2SE_SY_MAP_CLOSE_L:
        Z2GetStatusMgr()->menuOut();
        break;
    case Z2SE_OBJ_BOARD_BOUND:
    case Z2SE_OBJ_BOARD_PUT:
        if (param_2 < 90) {
            volume = 0.7f;
            pitch = 1.25f;
        } else if (param_2 < 60) {
            volume = 0.4f;
            pitch = 1.5f;
        }
        break;
    case Z2SE_SY_TALK_START:
    case Z2SE_SY_TALK_END:
        if (Z2GetStatusMgr()->getDemoStatus() != 0) {
            return false;
        }
        break;
    case Z2SE_SY_TALK_NEXT:
        if (Z2GetStatusMgr()->isMovieDemo()) {
            soundID = Z2SE_SY_DEMO_MESSAGE_NEXT;
        }
        break;
    case Z2SE_SY_TALK_WIN_CLOSE:
        if (Z2GetStatusMgr()->getDemoStatus() == 1) {
            soundID = Z2SE_SY_TALK_END;
        } else if (Z2GetStatusMgr()->isMovieDemo()) {
            soundID = Z2SE_SY_DEMO_MESSAGE_END;
        }
        break;
    case Z2SE_HP_GAUGE_INC:
        if (!Z2GetStatusMgr()->isHeartGuageOn() || !Z2GetSceneMgr()->isSceneExist()) {
            return false;
        }
        break;
    case Z2SE_OBJ_CHANDELIER_SWING: {
        Vec tmp = Z2GetAudience()->getAudioCamPos(0);
        if (posPtr->y > tmp.y) {
            return false;
        }
        break;
    }
    case Z2SE_OBJ_CHANDLV5_SWING:
        volume = Z2Calc::getParamByExp(param_2, 35.0f, 261.0f, 0.4f, 0.05f, 1.0f,
                                         Z2Calc::CURVE_LINEAR);
        break;
    case Z2SE_OBJ_IRONBALL_HIT:
        if (param_2 > 150) {
            volume = 1.4f;
        } else {
            volume = Z2Calc::getParamByExp(param_2, 0.0f, 47.0f, 0.4f, 0.5f, 0.9f,
                                             Z2Calc::CURVE_LINEAR);
        }
        break;
    case Z2SE_OBJ_IRONBALL_HIT_SNOW:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 41.0f, 0.4f, 0.3f, 1.0f,
                                         Z2Calc::CURVE_LINEAR);
        break;
    case Z2SE_OBJ_IRONBALL_HIT_ICE:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 41.0f, 0.4f, 0.3f, 1.0f,
                                         Z2Calc::CURVE_LINEAR);
        break;
    case Z2SE_OBJ_L8_B_FOG_FLY: {
        Vec tmp = Z2GetAudience()->getAudioCamPos(0);
        if (posPtr->y > tmp.y) {
            return false;
        }
        break;
    }
    case Z2SE_OBJ_BLN_BREAK_S:
    case Z2SE_OBJ_BLN_BREAK_M:
    case Z2SE_OBJ_BLN_BREAK_L:
        if (param_2 > 10) {
            param_2 = 10;
        }
        seStart(param_2 + 0x8C, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        break;
    case Z2SE_OBJ_AMATA_ACTION:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 400.0f, 0.4f, 0.5f, 1.0f,
                                         Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 0.0f, 400.0f, 0.4f, 0.8f, 1.1f,
                                        Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_TOY_BONE_BOUND:
        volume = Z2Calc::getParamByExp(param_2, 15.0f, 60.0f, 0.4f, 0.0f, 1.0f,
                                         Z2Calc::CURVE_POSITIVE);
        break;
    case Z2SE_FN_ROPE_CREAK_ADD:
        param_2 = Z2Calc::getParamByExp(param_2, 0.0f, 127.0f, 0.2f, 0.0f, 127.0f,
                                        Z2Calc::CURVE_NEGATIVE);
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 127.0f, 0.2f, 0.2f, 1.0f,
                                         Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 127.0f, 0.0f, 0.2f, 0.8f, 1.2f,
                                        Z2Calc::CURVE_POSITIVE);
        break;
    case Z2SE_OBJ_DARK_GATE_RIPPLE:
        if (Z2GetSceneMgr()->getCurrentSceneNum() == 3) {
            soundID = Z2SE_OBJ_DARK_GATE_RIPPLE_WLF;
        } else if (Z2GetSceneMgr()->getCurrentSceneNum() == 30
                    && Z2GetSceneMgr()->getCurrentRoomNum() == 7) {
            soundID = Z2SE_OBJ_DARK_GATE_RIPPLE_WLF;
        }
        break;
    case Z2SE_OBJ_ARMOR_SWING:
    case Z2SE_OBJ_ARMOR_HIT:
        break;
    }

    if (isLevelSe(soundID)) {
        return seStartLevel(soundID, posPtr, param_2, reverb, pitch,
                            volume, pan, dolby, param_8);
    }

    if (isSoundCulling(soundID)) {
        return false;
    }

    JAISoundHandle* handle = mSoundHandles.getFreeHandle();
    if (handle == NULL) {
        return false;
    }

    return Z2GetSoundStarter()->startSound(soundID, handle, (JGeometry::TVec3<f32>*)posPtr,
                                           param_2, reverb / 127.0f,
                                           pitch, volume, pan, dolby, 0);
}

bool Z2SeMgr::seStartLevel(JAISoundID soundID, const Vec* posPtr, u32 param_2, s8 reverb,
                           f32 pitch, f32 volume, f32 pan, f32 dolby, u8 param_8) {
    if (param_8 != 1) {
        switch (soundID) {
        case Z2SE_OBJ_FIRE_BURNING:
            if (Z2GetStatusMgr()->getDemoStatus() == 2 || !Z2GetSceneMgr()->isInGame()) {
                return false;
            }
            // fallthrough
        case Z2SE_FAIRY_S_LV:
        case Z2SE_EN_ZZ_MV:
        case Z2SE_OBJ_L8_L_BALL_SW:
        case Z2SE_OBJ_GANON_BARRIER:
        case Z2SE_OBJ_CRVN_BURNING:
            setLevObjSE(soundID, const_cast<Vec*>(posPtr), reverb);
            return false;
        }
    }

    switch (soundID) {
    case Z2SE_OBJ_FLAG_TRAILING:
        volume = Z2Calc::getParamByExp(param_2, 25.0f, 127.0f, 0.4f, 0.1f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 25.0f, 127.0f, 0.4f, 0.7f, 1.2f,
                                      Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_BRIDGE_TRAILING:
    case Z2SE_OBJ_BROKENBRIDGE:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 127.0f, 0.4f, 0.1f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 0.0f, 127.0f, 0.4f, 0.7f, 1.2f,
                                      Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_BOOMSHTR_SWITCH:
        if (param_2 < 10) {
            param_2 = 10;
        }
        volume = Z2Calc::getParamByExp(param_2, 10.0f, 127.0f, 2.0f, 0.0f, 1.0f,
                                       Z2Calc::CURVE_POSITIVE);
        pitch = Z2Calc::getParamByExp(param_2, 10.0f, 127.0f, 2.0f, 0.7f, 1.2f,
                                      Z2Calc::CURVE_POSITIVE);
        break;
    case Z2SE_OBJ_FAN_WIND_S:
        modHeightAtCamera(&posPtr);
        break;
    case Z2SE_OBJ_FAN_ROLL_S:
    case Z2SE_OBJ_FAN_ROLL_M:
    case Z2SE_OBJ_FAN_ROLL_L:
        if (param_2 < 10) {
            param_2 = 10;
        }
        volume = Z2Calc::getParamByExp(param_2, 10.0f, 127.0f, 2.0f, 0.0f, 1.0f,
                                       Z2Calc::CURVE_POSITIVE);
        pitch = Z2Calc::getParamByExp(param_2, 10.0f, 127.0f, 2.0f, 0.7f, 1.2f,
                                      Z2Calc::CURVE_POSITIVE);
        break;
    case Z2SE_OBJ_AMI_ROLL:
        if (param_2 < 10) {
            param_2 = 10;
        }
        volume = Z2Calc::getParamByExp(param_2, 10.0f, 127.0f, 2.0f, 0.0f, 1.0f,
                                       Z2Calc::CURVE_POSITIVE);
        pitch = Z2Calc::getParamByExp(param_2, 10.0f, 127.0f, 2.0f, 0.7f, 1.2f,
                                      Z2Calc::CURVE_POSITIVE);
        break;
    case Z2SE_OBJ_BRDG_MOVE:
        pitch = Z2Calc::getParamByExp(param_2, 0.0f, 127.0f, 2.0f, 0.7f, 1.2f,
                                      Z2Calc::CURVE_POSITIVE);
        break;
    case Z2SE_ENV_WIND_SARUDAN:
        volume = Z2Calc::getParamByExp(param_2, 25.0f, 127.0f, 0.4f, 0.4f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 25.0f, 127.0f, 0.4f, 0.8f, 1.0f,
                                      Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_CHANDLV9_SWING:
        if (param_2 < 10) {
            param_2 = 10;
        }
        volume = Z2Calc::getParamByExp(param_2, 10.0f, 127.0f, 2.0f, 0.0f, 1.0f,
                                       Z2Calc::CURVE_POSITIVE);
        pitch = Z2Calc::getParamByExp(param_2, 10.0f, 127.0f, 2.0f, 0.7f, 1.2f,
                                      Z2Calc::CURVE_POSITIVE);
        break;
    case Z2SE_OBJ_BOMB_HOUSE_BURN:
        if (param_2 > 300) {
            seStartLevel(Z2SE_OBJ_BOMB_HOUSE_BURN_S, NULL, param_2, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
        if (param_2 > 500) {
            volume = 1.0f;
            pitch = 1.3f;
        } else {
            volume = param_2 * 0.0006 + 0.7;
            pitch = param_2 * 0.0006 + 1.0;
        }
        break;
    case Z2SE_OBJ_BOMB_HOUSE_BURN_S:
        if (param_2 > 500) {
            volume = 1.0f;
            pitch = (param_2 - 500) * 0.0015 + 1.0;
        } else if (param_2 < 300) {
            volume = 0.0f;
        } else {
            volume = (param_2 - 300) * 0.005;
        }
        break;
    case Z2SE_OBJ_WTR_CLMN_UP:
        if (param_2 <= 500) {
            pitch = 0.8f;
        } else if (param_2 > 500 && param_2 <= 1000) {
            pitch = 1.0f;
        } else if (param_2 > 1000 && param_2 <= 1500) {
            pitch = 1.0f;
        } else {
            pitch = 1.2f;
        }
        modHeightAtCamera(&posPtr);
        break;
    case Z2SE_OBJ_WTR_CLMN_DOWN:
        if (param_2 > 700) {
            pitch = 1.0f;
        } else {
            pitch = param_2 * 0.001 + 0.3;
        }
        modHeightAtCamera(&posPtr);
        break;
    case Z2SE_OBJ_STN_SPRL_RAIL:
    case Z2SE_OBJ_STN_SPRL_RAIL_DW:
        modHeightAtCamera(&posPtr);
        break;
    case Z2SE_OBJ_SPNR_GEAR_S:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 4000.0f, 0.4f, 0.3f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 0.0f, 4000.0f, 0.4f, 0.7f, 1.2f,
                                      Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_SPNR_GEAR_L:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 3500.0f, 0.4f, 0.3f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 0.0f, 3500.0f, 0.4f, 0.7f, 1.2f,
                                      Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_TOGE_SPIN:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 26.0f, 0.4f, 0.6f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 0.0f, 26.0f, 0.4f, 0.8f, 1.2f,
                                      Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_STN_WL_RL:
    case Z2SE_OBJ_STN_WL_RL_OP:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 64.0f, 0.4f, 0.3f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 0.0f, 64.0f, 0.4f, 0.6f, 1.2f,
                                      Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_SPNR_SW_RL:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 4500.0f, 0.4f, 0.6f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 0.0f, 4500.0f, 0.4f, 0.7f, 1.2f,
                                      Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_TOGE_ROLL_MV:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 40.0f, 0.4f, 0.0f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 0.0f, 40.0f, 0.4f, 0.8f, 1.2f,
                                      Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_L8_L_TORCH_SW:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 120.0f, 0.4f, 0.0f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 0.0f, 120.0f, 0.4f, 0.9f, 1.0f,
                                      Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_L8_B_FOG_STAY:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 100.0f, 0.4f, 0.0f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_L8_B_FALL_S:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 100.0f, 0.4f, 0.0f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_L8_B_FALL_L:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 100.0f, 0.4f, 0.0f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_L8_L_LIFT_MV:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 5.0f, 0.4f, 0.5f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 0.0f, 5.0f, 0.4f, 0.7f, 1.0f,
                                      Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_AMATA_CRK:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 400.0f, 0.4f, 0.5f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        pitch = Z2Calc::getParamByExp(param_2, 0.0f, 400.0f, 0.4f, 0.8f, 1.1f,
                                      Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_OBJ_HYRULE_BARRIER:
        if (Z2GetSceneMgr()->getCurrentSceneNum() != Z2SCENE_CASTLE_TOWN) {
            break;
        }
        // fallthrough
    case Z2SE_OBJ_DARK_GATE:
        if (!Z2GetSceneMgr()->isInDarkness() && posPtr != NULL && Z2GetLink() != NULL && Z2GetLink()->getCurrentPos() != NULL)
        {
            f32 dist = sqrtf(VECSquareDistance(posPtr, Z2GetLink()->getCurrentPos()));
            f32 gate_volume = 1.0f;
            if (dist < 2000.0f) {
                gate_volume = 0.0f;
            } else if (dist < 4000.0f) {
                gate_volume = Z2Calc::getParamByExp(dist, 2000.0f, 5000.0f, 0.45f, 0.0f, 1.0f,
                                                    Z2Calc::CURVE_NEGATIVE);
            }
            Z2GetSeqMgr()->setTwilightGateVol(gate_volume);
        }
        break;
    case Z2SE_OBJ_HASU_WTR:
        volume = Z2Calc::getParamByExp(param_2, 0.0f, 50.0f, 0.4f, 0.5f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        break;
    case Z2SE_ENV_PEOPLE_CROWD: {
        f32 scale = 1.0f;
        if (Z2GetSceneMgr()->getCurrentRoomNum() == 3) {
            scale = Z2Calc::linearTransform(J3DUD::JMAAbs(Z2GetLink()->getCurrentPos()->x),
                                            600.0f, 3500.0f, 1.0f, 0.1f, false);
        }

        switch (Z2GetStatusMgr()->getHour()) {
        case 0:
        case 1:
        case 2:
        case 3:
            scale *= 0.1f;
            break;
        case 4:
            scale *= 0.2f;
            break;
        case 5:
            scale *= 0.3f;
            break;
        case 6:
            scale *= 0.4f;
            break;
        case 7:
            scale *= 0.5f;
            break;
        case 8:
            scale *= 0.6f;
            break;
        case 9:
            scale *= 0.7f;
            break;
        case 10:
            scale *= 0.8f;
            break;
        case 11:
            scale *= 0.9f;
            break;
        case 18:
            scale *= 0.9f;
            break;
        case 19:
            scale *= 0.8f;
            break;
        case 20:
            scale *= 0.7f;
            break;
        case 21:
            scale *= 0.5f;
            break;
        case 22:
            scale *= 0.4f;
            break;
        case 23:
            scale *= 0.2f;
            break;
        }
        volume = Z2Calc::getParamByExp(mCrowdSize * scale, 5.0f, 90.0f, 0.2f, 0.0f, 1.0f,
                                       Z2Calc::CURVE_NEGATIVE);
        break;
    }
    case Z2SE_AL_COPYROD_WAIT:
        param_2++;
        break;
    }

    for (int i = 0; i < 24; i++) {
        if (mSoundHandles[i] && mSoundHandles[i]->getID() == soundID && posPtr == (Vec*)mSoundHandles[i]->getUserData()) {
            if (param_8 == 1) {
                volume = Z2GetAudience()->calcOffMicSound(volume);
            }

            mSoundHandles[i]->updateLifeTime(1);

            if (posPtr != NULL) {
                mSoundHandles[i]->setPos((JGeometry::TVec3<f32>)*posPtr);
            }

            if (param_2 != 0) {
                JAISoundHandle* handle = &mSoundHandles[i];
                Z2GetSoundStarter()->setPortData(handle, 6, param_2, -1);
            }

            if (reverb != 0) {
                mSoundHandles[i]->getAuxiliary().moveFxMix(reverb / 127.0f, 0);
            }
            if (pitch != 1.0f) {
                mSoundHandles[i]->getAuxiliary().movePitch(pitch, 0);
            }
            if (volume != 1.0f) {
                mSoundHandles[i]->getAuxiliary().moveVolume(volume, 0);
            }
            if (pan != -1.0f) {
                mSoundHandles[i]->getAuxiliary().movePan(pan, 0);
            }
            if (dolby != -1.0f) {
                mSoundHandles[i]->getAuxiliary().moveDolby(dolby, 0);
            }

            return true;
        }
    }

    if (isSoundCulling(soundID)) {
        return false;
    }

    JAISoundHandle* handle = mSoundHandles.getFreeHandle();
    if (handle == NULL) {
        return false;
    }

    bool ret = Z2GetSoundStarter()->startSound(soundID, handle, (JGeometry::TVec3<f32>*)posPtr,
                                               param_2, reverb / 127.0f, pitch, volume,
                                               pan, dolby, false);
    
    if (handle != NULL && *handle) {
        if (soundID == Z2SE_OBJ_BOMB_HOUSE_BURN) {
            Z2GetSeqMgr()->bgmStart(Z2BGM_EVENT02, 0, 0);
        }

        if (param_8 == 1) {
            Z2GetAudience()->calcOffMicSound(volume);
        }

        (*handle)->setUserData((uintptr_t)posPtr);

        if (ret) {
            (*handle)->setLifeTime(1, false);
        }
    }

    return ret;
}

void Z2SeMgr::seStop(JAISoundID soundID, u32 fadeTime) {
    if (soundID != -1) {
        for (u8 i = 0; i < 24; i++) {
            if (mSoundHandles[i] && mSoundHandles[i]->getID() == soundID) {
                mSoundHandles[i]->stop(fadeTime);
                mSoundHandles[i].releaseSound();
            }
        }
    }
}

void Z2SeMgr::seStopAll(u32 fadeTime) {
    JAISeMgr* seMgr = Z2GetSoundMgr()->getSeMgr();
    seMgr->getCategory(0)->stop(fadeTime);
    seMgr->getCategory(1)->stop(fadeTime);
    seMgr->getCategory(2)->stop(fadeTime);
    seMgr->getCategory(3)->stop(fadeTime);
    seMgr->getCategory(4)->stop(fadeTime);
    seMgr->getCategory(5)->stop(fadeTime);
    seMgr->getCategory(6)->stop(fadeTime);
    seMgr->getCategory(7)->stop(fadeTime);
    seMgr->getCategory(8)->stop(fadeTime);
    seMgr->getCategory(9)->stop(fadeTime);
}

void Z2SeMgr::seMoveVolumeAll(f32 volume, u32 moveTime) {
    if (Z2GetStatusMgr()->getDemoStatus() == 13) {
        volume = 0.0f;
    }

    JAISeMgr* seMgr = Z2GetSoundMgr()->getSeMgr();
    seMgr->getCategory(1)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_VOICE_DEFAULT * volume, moveTime);
    seMgr->getCategory(2)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_MOTION_DEFAULT * volume, moveTime);
    seMgr->getCategory(3)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_FOOTNOTE_DEFAULT * volume, moveTime);
    seMgr->getCategory(4)->getParams()->moveVolume(Z2Param::VOL_SE_OBJECT_DEFAULT * volume, moveTime);
    seMgr->getCategory(5)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_VOICE_DEFAULT * volume, moveTime);
    seMgr->getCategory(6)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_MOVE_DEFAULT * volume, moveTime);
    seMgr->getCategory(7)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_VOICE_DEFAULT * volume, moveTime);
    seMgr->getCategory(8)->getParams()->moveVolume(Z2Param::VOL_SE_OBJECT_DEFAULT * volume, moveTime);

    if (Z2GetStatusMgr()->getDemoStatus() != 12) {
        seMgr->getCategory(9)->getParams()->moveVolume(Z2Param::VOL_SE_ATMOSPHERE_DEFAULT * volume, moveTime);
    }
}

void Z2SeMgr::messageSePlay(u16 param_0, Vec* param_1, s8 param_2) {
    Z2GetSpeechMgr2()->playOneShotVoice(0, param_0, param_1, param_2);
}

void Z2SeMgr::talkInSe() {
    if (Z2GetSceneMgr()->isInGame()) {
        if (Z2GetStatusMgr()->getDemoStatus() == 13) {
            seMoveVolumeAll(0.0f, 0);
        }

        JAISeMgr* seMgr = Z2GetSoundMgr()->getSeMgr();
        seMgr->getCategory(0)->getParams()->moveVolume(Z2Param::VOL_SE_SYSTEM_TALKING, 2);
        seMgr->getCategory(1)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_VOICE_TALKING, 2);
        seMgr->getCategory(2)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_MOTION_TALKING, 2);
        seMgr->getCategory(3)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_FOOTNOTE_TALKING, 2);
        seMgr->getCategory(4)->getParams()->moveVolume(Z2Param::VOL_SE_OBJECT_TALKING, 2);
        seMgr->getCategory(5)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_VOICE_TALKING, 2);
        seMgr->getCategory(6)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_MOVE_TALKING, 2);
        seMgr->getCategory(7)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_VOICE_TALKING, 2);
        seMgr->getCategory(8)->getParams()->moveVolume(Z2Param::VOL_SE_OBJECT_TALKING, 2);

        if (Z2GetStatusMgr()->getDemoStatus() != 12) {
            seMgr->getCategory(9)->getParams()->moveVolume(Z2Param::VOL_SE_ATMOSPHERE_TALKING, 2);
        }
    }
}

void Z2SeMgr::talkOutSe() {
    if (Z2GetSceneMgr()->isInGame()) {
        if (Z2GetStatusMgr()->getDemoStatus() == 13) {
            seMoveVolumeAll(0.0f, 0);
        }

        JAISeMgr* seMgr = Z2GetSoundMgr()->getSeMgr();
        seMgr->getCategory(0)->getParams()->moveVolume(Z2Param::VOL_SE_SYSTEM_DEFAULT, 2);
        seMgr->getCategory(1)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_VOICE_DEFAULT, 2);
        seMgr->getCategory(2)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_MOTION_DEFAULT, 2);
        seMgr->getCategory(3)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_FOOTNOTE_DEFAULT, 2);
        seMgr->getCategory(4)->getParams()->moveVolume(Z2Param::VOL_SE_OBJECT_DEFAULT, 2);
        seMgr->getCategory(5)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_VOICE_DEFAULT, 2);
        seMgr->getCategory(6)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_MOVE_DEFAULT, 2);
        seMgr->getCategory(7)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_VOICE_DEFAULT, 2);
        seMgr->getCategory(8)->getParams()->moveVolume(Z2Param::VOL_SE_OBJECT_DEFAULT, 2);

        if (Z2GetStatusMgr()->getDemoStatus() != 12) {
            seMgr->getCategory(9)->getParams()->moveVolume(Z2Param::VOL_SE_ATMOSPHERE_DEFAULT, 2);
        }
    }
}

void Z2SeMgr::menuInSe() {
    if (Z2GetSceneMgr()->isInGame()) {
        if (Z2GetStatusMgr()->getDemoStatus() == 13) {
            seMoveVolumeAll(0.0f, 0);
        }

        JAISeMgr* seMgr = Z2GetSoundMgr()->getSeMgr();
        seMgr->getCategory(0)->getParams()->moveVolume(Z2Param::VOL_SE_SYSTEM_PAUSING, 2);
        seMgr->getCategory(1)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_VOICE_PAUSING, 2);
        seMgr->getCategory(2)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_MOTION_PAUSING, 2);
        seMgr->getCategory(3)->getParams()->moveVolume(Z2Param::VOL_SE_LINK_FOOTNOTE_PAUSING, 2);
        seMgr->getCategory(4)->getParams()->moveVolume(Z2Param::VOL_SE_OBJECT_PAUSING, 2);
        seMgr->getCategory(5)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_VOICE_PAUSING, 2);
        seMgr->getCategory(6)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_MOVE_PAUSING, 2);
        seMgr->getCategory(7)->getParams()->moveVolume(Z2Param::VOL_SE_CHAR_VOICE_PAUSING, 2);
        seMgr->getCategory(8)->getParams()->moveVolume(Z2Param::VOL_SE_OBJECT_PAUSING, 2);

        if (Z2GetStatusMgr()->getDemoStatus() != 12) {
            seMgr->getCategory(9)->getParams()->moveVolume(Z2Param::VOL_SE_ATMOSPHERE_PAUSING, 2);
        }
    }
}

void Z2SeMgr::setLevObjSE(u32 soundID, Vec* posPtr, s8 reverb) {
    u8 i = 0;
    for (; i < mLevelObjectSeCount; i++) {
        if (mLevelObjSe[i].soundID_ == soundID) {
            break;
        }
    }

    if (i == mLevelObjectSeCount) {
        if (mLevelObjectSeCount == 10) {
            return;
        }
        mLevelObjSe[i].soundID_ = soundID;
        mLevelObjectSeCount++;
    }

    f32 volume_scale = 1.0f;
    Vec pos = {0.0f, 0.0f, -50.0f};
    if (posPtr != NULL) {
        pos = *posPtr;
    } else {
        pos = Z2GetAudience()->getAudioCamPos(0);
    }

    switch (soundID) {
    case Z2SE_OBJ_FIRE_BURNING:
        volume_scale = 0.7f;
        break;
    case Z2SE_EN_ZZ_MV:
        volume_scale = 4.0f;
        break;
    case Z2SE_FAIRY_S_LV:
    case Z2SE_OBJ_L8_L_BALL_SW:
        break;
    }

    mLevelObjSe[i].setVolumeScale(volume_scale);
    mLevelObjSe[i].registMultiSePos((Vec*)&pos);
    if (mLevelObjSe[i].reverb_ < reverb) {
        mLevelObjSe[i].reverb_ = reverb;
    }
}

void Z2SeMgr::setMultiTriggerSE(u32 soundID, Vec* posPtr, s8 reverb) {
    u8 i = 0;
    for (; i < mMultiTriggerSeCount; i++) {
        if (mMultiTriggerSe[i].soundID_ == soundID) {
            break;
        }
    }

    if (i == mMultiTriggerSeCount) {
        if (mMultiTriggerSeCount == 10) {
            return;
        }
        mMultiTriggerSe[i].soundID_ = soundID;
        mMultiTriggerSeCount++;
    }

    f32 volume_scale = 1.0f;
    Vec pos = {0.0f, 0.0f, -50.0f};
    if (posPtr != NULL) {
        pos = *posPtr;
    } else {
        pos = Z2GetAudience()->getAudioCamPos(0);
    }

    switch (soundID) {
    case Z2SE_OBJ_L8_STAIR_ON:
    case Z2SE_OBJ_L8_STAIR_MV_UP:
    case Z2SE_OBJ_L8_STAIR_MV_DW:
    case Z2SE_OBJ_L8_STAIR_ST_UP:
    case Z2SE_OBJ_L8_STAIR_ST_DW:
    case Z2SE_OBJ_L8_L_LIFT_ON:
        volume_scale = 3.0f;
        break;
    case Z2SE_OBJ_L8_L_BALL_SW_ON:
        volume_scale = 1.3f;
        break;
    }

    mMultiTriggerSe[i].setVolumeScale(volume_scale);
    mMultiTriggerSe[i].registMultiSePos((Vec*)&pos);
    if (mMultiTriggerSe[i].reverb_ < reverb) {
        mMultiTriggerSe[i].reverb_ = reverb;
    }
}

void Z2SeMgr::processSeFramework() {
    if (Z2GetSceneMgr()->isSceneExist() && mCrowdSize != 0) {
        seStartLevel(Z2SE_ENV_PEOPLE_CROWD, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }

    for (u8 i = 0; i < mLevelObjectSeCount; i++) {
        f32 volume = mLevelObjSe[i].getMaxVolume();
        seStartLevel(mLevelObjSe[i].soundID_, NULL, 0, mLevelObjSe[i].reverb_, 1.0f,
                     volume, mLevelObjSe[i].getPanPower(), mLevelObjSe[i].getDolbyPower(), 1);
    }

    for (u8 i = 0; i < 10; i++) {
        mLevelObjSe[i].resetMultiSePos();
        mLevelObjSe[i].soundID_ = -1;
        mLevelObjSe[i].reverb_ = 0;
    }

    mLevelObjectSeCount = 0;
    for (u8 i = 0; i < mMultiTriggerSeCount; i++) {
        f32 volume = mMultiTriggerSe[i].getMaxVolume();
        seStart(mMultiTriggerSe[i].soundID_, NULL, 0, mMultiTriggerSe[i].reverb_, 1.0f,
                volume, mMultiTriggerSe[i].getPanPower(),
                mMultiTriggerSe[i].getDolbyPower(), 1);
    }

    for (u8 i = 0; i < 10; i++) {
        mMultiTriggerSe[i].resetMultiSePos();
        mMultiTriggerSe[i].soundID_ = -1;
        mMultiTriggerSe[i].reverb_ = 0;
    }

    mMultiTriggerSeCount = 0;
    field_0x3c9 = 0;
    field_0x3ca = 0;
    field_0x3cb = 0;

    if (field_0x3c2 != 0) {
        field_0x3c2--;
    } else {
        field_0x3c2 = 0;
    }

    if (field_0x3c3 != 0) {
        field_0x3c3--;
    } else {
        field_0x3c3 = 0;
    }

    if (field_0x3c4 != 0) {
        field_0x3c4--;
    } else {
        field_0x3c4 = 0;
    }

    if (field_0x3c5 != 0) {
        field_0x3c5--;
    } else {
        field_0x3c5 = 0;
    }

    if (field_0x3c0 != 0) {
        field_0x3c0--;
    } else {
        field_0x3c0 = 0;
    }

    if (field_0x3c1 != 0) {
        field_0x3c1--;
    } else {
        field_0x3c1 = 0;
    }

    if (field_0x3c6 != 0) {
        field_0x3c6--;
    } else {
        field_0x3c6 = 0;
    }

    if (field_0x3c7 != 0) {
        field_0x3c7--;
    } else {
        field_0x3c7 = 0;
    }

    if (field_0x3c8 != 0) {
        field_0x3c8--;
    } else {
        field_0x3c8 = 0;
    }
}


bool Z2SeMgr::isLevelSe(JAISoundID soundID) {
    switch (soundID) {
    case JA_SE_LK_MOVE_ROCK:
    case JA_SE_LK_MOVE_WBLOCK:
    case Z2SE_OBJ_FIRE_BURNING:
    case JA_SE_MAGMA_L:
    case JA_SE_MAGMA_R:
    case JA_SE_MAGMA_SR:
    case JA_SE_FIREBLAST_NOISE:
    case JA_SE_ATM_WIND_HOT:
        return true;
    default:
        return false;
    }
}

bool Z2SeMgr::isSoundCulling(JAISoundID soundID) {
    if (Z2GetSeqMgr()->isItemGetDemo()) {
        switch (soundID) {
        case Z2SE_HP_GAUGE_INC:
        case Z2SE_SY_TALK_NEXT:
        case Z2SE_SY_TALK_END:
        case Z2SE_SY_DEMO_MESSAGE_NEXT:
        case Z2SE_SY_DEMO_MESSAGE_END:
        case Z2SE_AL_FISH_JUMP_DOWN:
        case Z2SE_CAT_CRY_ANNOY_D:
        case Z2SE_CAT_CRY_ATTACK_D:
        case Z2SE_MIDNA_APPEAR:
        case Z2SE_HENA_CLAP:
        case Z2SE_EN_PO_V_DEATH:
        case Z2SE_EN_SG_BOUND:
        case Z2SE_OBJ_TBOX_OPEN_A:
        case Z2SE_OBJ_TBOX_OPEN_B:
        case Z2SE_OBJ_TBOX_OPEN_B_SLOW:
        case Z2SE_OBJ_TBOX_OPEN_C:
        case Z2SE_OBJ_NIOIMASU_BOUND:
            return false;
        default:
            return true;
        }
    }

    switch (soundID) {
    case Z2SE_MIDNA_APPEAR:
    case Z2SE_MIDNA_DISAPPEAR:
        if (!Z2GetSceneMgr()->isSceneExist()) {
            return true;
        }
        break;
    case Z2SE_EN_BE_DEATH:
    case Z2SE_EN_BUG_DIE:
        if (field_0x3c0 != 0) {
            return true;
        }
        field_0x3c0 = 4;
        break;
    case Z2SE_EN_BE_GO_HOME:
    case Z2SE_EN_BUG_JUMP:
        if (field_0x3c1 != 0) {
            return true;
        }
        field_0x3c1 = 4;
        break;
    case JA_SE_FT_ADD_GRASS:
        if (field_0x3c2 != 0) {
            return true;
        }
        field_0x3c2 = 2;
        break;
    case JA_SE_OBJ_BREAK_WRAILING:
        if (field_0x3c9 != 0) {
            return true;
        }
        field_0x3c9 = 1;
        break;
    case JA_SE_LK_CUT_GRASS:
        if (field_0x3c3 != 0) {
            return true;
        }
        field_0x3c3 = 4;
        break;
    case JA_SE_OBJ_BREAK_CHINA_S:
        if (field_0x3c4 != 0) {
            return true;
        }
        field_0x3c4 = 2;
        break;
    case Z2SE_OBJ_SKULL_BREAK:
        if (field_0x3c5 != 0) {
            return true;
        }
        field_0x3c5 = 2;
        break;
    case JA_SE_CM_MONS_EXPLODE:
        if (field_0x3ca != 0) {
            return true;
        }
        field_0x3ca = 1;
        Z2GetSoundMgr()->multiVolumeSoundID(soundID, 0.5f);
        break;
    case Z2SE_DARK_VANISH:
        if (field_0x3cb != 0) {
            return true;
        }
        field_0x3cb = 1;
        Z2GetSoundMgr()->stopSoundID(soundID);
        Z2GetSoundMgr()->multiVolumeSoundID(soundID, 0.5f);
        break;
    case Z2SE_EN_BS_V_DEAD:
    case Z2SE_EN_FZ_BOUND:
        if (field_0x3c6 != 0) {
            return true;
        }
        field_0x3c6 = 3;
        break;
    case Z2SE_EN_DB_BERON:
    case Z2SE_EN_YK_V_DEATH:
    case Z2SE_EN_YG_FOOTNOTE:
        if (field_0x3c0 != 0) {
            return true;
        }
        field_0x3c0 = 3;
        break;
    case Z2SE_EN_DB_APPEAR:
    case Z2SE_EN_BS_DEAD:
        if (field_0x3c1 != 0) {
            return true;
        }
        field_0x3c1 = 3;
        break;
    case Z2SE_EN_YK_WING:
        if (field_0x3c2 != 0) {
            return true;
        }
        field_0x3c2 = 4;
        break;
    case Z2SE_EN_YK_V_FURA:
        if (field_0x3c3 != 0) {
            return true;
        }
        field_0x3c3 = 4;
        break;
    case Z2SE_EN_GBA_FOOTNOTE:
        if (field_0x3c7 != 0) {
            return true;
        }
        field_0x3c7 = 1;
        break;
    case Z2SE_EN_GBA_V_DEATH:
    case Z2SE_EN_GBA_LAYEGG:
    case Z2SE_EN_GBA_FUKA:
    case Z2SE_EN_GBA_LAND:
        if (field_0x3c8 != 0) {
            return true;
        }
        field_0x3c8 = 1;
        break;
    case Z2SE_EN_BS_APPEAR:
        break;
    }

    return false;
}


Z2MultiSeMgr::Z2MultiSeMgr() {
    resetMultiSePos();
}

Z2MultiSeMgr::~Z2MultiSeMgr() {}

s8 Z2MultiSeMgr::registMultiSePos(Vec* posPtr) {
    if (posPtr == NULL) {
        return 0;
    }

    if (posCount_ < 0) {
        posCount_ = 0;
    }

    Vec rel_pos;
    Z2GetAudience()->convertAbsToRel(*posPtr, &rel_pos, 0);

    f32 volume = Z2GetAudience()->calcRelPosVolume(rel_pos, volumeScale_, 0);
    if (volume <= 0.0f) {
        return posCount_;
    }

    f32 pan = Z2GetAudience()->calcRelPosPan(rel_pos, 0);
    f32 dolby = Z2GetAudience()->calcRelPosDolby(rel_pos, 0);
    f32 pow_l = (1.0f - pan) * volume;
    f32 pow_r = pan * volume;
    f32 pow_f = (1.0f - dolby) * volume;
    f32 pow_b = dolby * volume;
    if (volume > maxVolume_) {
        maxVolume_ = volume;
    }
    if (pow_l > maxPowL_) {
        maxPowL_ = pow_l;
    }
    if (pow_r > maxPowR_) {
        maxPowR_ = pow_r;
    }
    if (pow_f > maxPowF_) {
        maxPowF_ = pow_f;
    }
    if (pow_b > maxPowB_) {
        maxPowB_ = pow_b;
    }

    posCount_++;
    return posCount_;
}


void Z2MultiSeMgr::resetMultiSePos() {
    posCount_ = -1;
    maxPowL_ = 0.0f;
    maxPowR_ = 0.0f;
    maxPowF_ = 0.0f;
    maxPowB_ = 0.0f;
    maxVolume_ = 0.0f;
}

f32 Z2MultiSeMgr::getPanPower() {
    f32 pow_r = getMaxPowR();
    f32 pow_l = getMaxPowL();
    if (pow_l + pow_r > 0.0f) {
        f32 pow_r = getMaxPowR();
        f32 pow_l = getMaxPowL();
        return getMaxPowR() / (pow_l + pow_r);
    } else {
        return 0.5f;
    }
}

f32 Z2MultiSeMgr::getDolbyPower() {
    f32 pow_b = getMaxPowB();
    f32 pow_f = getMaxPowF();
    if (pow_f + pow_b > 0.0f) {
        f32 pow_b = getMaxPowB();
        f32 pow_f = getMaxPowF();
        return getMaxPowB() / (pow_f + pow_b);
    } else {
        return 0.5f;
    }
}
