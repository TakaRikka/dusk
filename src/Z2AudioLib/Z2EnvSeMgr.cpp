#include "Z2AudioLib/Z2EnvSeMgr.h"
#include "Z2AudioLib/Z2Calc.h"
#include "JSystem/JAudio2/JASCalc.h"
#include "d/d_save.h"
#include "d/d_com_inf_game.h"

#if DEBUG
static const char* sSpotName[] = {
    "F_SP00",
    "F_SP103",
    "R_SP01",
    "F_SP104",
    "R_SP107",
    "F_SP108",
    "R_SP108",
    "F_SP117",
    "F_SP109",
    "R_SP109",
    "R_SP209",
    "F_SP110",
    "R_SP110",
    "F_SP111",
    "F_SP128",
    "R_SP128",
    "F_SP115",
    "F_SP112",
    "F_SP126",
    "F_SP127",
    "R_SP127",
    "F_SP113",
    "F_SP116",
    "R_SP116",
    "R_SP160",
    "R_SP161",
    "F_SP114",
    "F_SP118",
    "F_SP124",
    "F_SP125",
    "F_SP121",
    "F_SP122",
    "F_SP123",
    "F_SP200",
    "F_SP102",
    "",
    "R_SP300",
    "R_SP301",
    "T_ENEMY",
    "D_MN54",
    "D_MN05",
    "D_MN05B",
    "D_MN05A",
    "D_MN04",
    "D_MN04B",
    "D_MN04A",
    "D_MN01",
    "D_MN01B",
    "D_MN01A",
    "D_MN10",
    "D_MN10B",
    "D_MN10A",
    "D_MN11",
    "D_MN11B",
    "D_MN11A",
    "D_MN06",
    "D_MN06B",
    "D_MN06A",
    "D_MN07",
    "D_MN07B",
    "D_MN07A",
    "D_MN08",
    "D_MN08B",
    "D_MN08C",
    "D_MN08A",
    "D_MN08D",
    "D_MN09",
    "D_MN09A",
    "D_MN09B",
    "D_MN09C",
    "D_SB00",
    "D_SB01",
    "D_SB02",
    "D_SB03",
    "D_SB04",
    "D_SB05",
    "D_SB06",
    "D_SB07",
    "D_SB08",
    "D_SB09",
    "D_SB10",
};
#endif

Z2EnvSeBase::Z2EnvSeBase(Vec* posPtr) {
    mPos = posPtr;
}

Z2EnvSeBase::~Z2EnvSeBase() {}

bool Z2EnvSeBase::startEnvSe(JAISoundID soundID, f32 fxMix, f32 volume, f32 pan, f32 dolby, f32 pitch, u32 moveSteps) {
    if (Z2GetStatusMgr()->isPaused()) {
        volume *= 0.33f;
    }

    f32 dVar16 = 1.0f;
    f32 fVar1 = Z2GetStatusMgr()->getCameraInWaterDepth() > 0.0f;
    if (fVar1) {
        dVar16 = Z2Calc::getParamByExp(
            fVar1, 1500.0f, 0.0f,
            0.1f, 0.0f,
            1.0f, Z2Calc::CURVE_NEGATIVE);
        volume *= dVar16;
    }

    bool rv = Z2GetSoundStarter()->startSound(soundID, &mHandle, (JGeometry::TVec3<f32>*)mPos, 0, fxMix, pitch, volume, pan, dolby, moveSteps);
    if (mHandle) {
        if (dVar16 < 1.0f) {
            Z2GetSoundMgr()->setIIR(mHandle.getSound(), JASCalc::CUTOFF_TO_IIR_TABLE[64]);
        } else {
            Z2GetSoundMgr()->setIIR(mHandle.getSound(), JASCalc::CUTOFF_TO_IIR_TABLE[127]);
        }
    }

    return rv;
}

bool Z2EnvSeBase::startEnvSeLevel(JAISoundID soundID, f32 fxMix, f32 volume, f32 pan,
                                  f32 dolby, f32 pitch, u32 moveSteps) {
    if (Z2GetStatusMgr()->isPaused()) {
        volume *= 0.33f;
    }

    f32 dVar16 = 1.0f;
    f32 fVar1 = Z2GetStatusMgr()->getCameraInWaterDepth() > 0.0f;
    if (fVar1) {
        dVar16 = Z2Calc::getParamByExp(
            fVar1, 1500.0f, 0.0f,
            0.1f, 0.0f,
            1.0f, Z2Calc::CURVE_NEGATIVE);
        volume *= dVar16;
    }

    bool rv = Z2GetSoundStarter()->startLevelSound(soundID, &mHandle, (JGeometry::TVec3<f32>*)mPos);
    if (mHandle) {
        if (dVar16 < 1.0f) {
            Z2GetSoundMgr()->setIIR(mHandle.getSound(), JASCalc::CUTOFF_TO_IIR_TABLE[64]);
        } else {
            Z2GetSoundMgr()->setIIR(mHandle.getSound(), JASCalc::CUTOFF_TO_IIR_TABLE[127]);
        }

        mHandle->getAuxiliary().moveVolume(volume, moveSteps);
        mHandle->getAuxiliary().movePan(pan, moveSteps);
        mHandle->getAuxiliary().moveDolby(dolby, moveSteps);
        mHandle->getAuxiliary().moveFxMix(fxMix, moveSteps);
        mHandle->getAuxiliary().movePitch(pitch, moveSteps);
    }

    return rv;
}

Z2EnvSeDir::Z2EnvSeDir(Vec* dir) {
    mPan = 0.5f;
    mDolby = 0.5f;

    if (dir) {
        setPanDir(dir);
    } else {
        mPanDir = Z2Calc::cNullVec;
    }
}

Z2EnvSeDir::~Z2EnvSeDir() {}

void Z2EnvSeDir::setPanDir(Vec* dir) {
    JUT_ASSERT(249, dir);
    mPanDir.x = dir->x * -1000.0f;
    mPanDir.y = dir->y * -1000.0f;
    mPanDir.z = dir->z * -1000.0f;
}

void Z2EnvSeDir::calcPan(f32 param_0) {
    if (*getHandle()) {
        if (0.0f == mPanDir.x && 0.0f == mPanDir.y && 0.0f == mPanDir.z) {
            (*getHandle())->getAuxiliary().movePan(0.5f, 0);
            (*getHandle())->getAuxiliary().moveDolby(0.5f, 0);
        } else {
            Z2GetAudience()->convertAbsToRel(mPanDir, &mPanDir, 0);
            JMath::fastVECNormalize(&mPanDir, &mPanDir);

            mPan = Z2GetAudience()->calcRelPosPan(mPanDir, 0);
            mDolby = Z2GetAudience()->calcRelPosDolby(mPanDir, 0);

            (*getHandle())->getAuxiliary().movePan(mPan, 0);
            (*getHandle())->getAuxiliary().moveDolby(mDolby, 0);
        }
    }
}

bool Z2EnvSeDir::startEnvSeDirLevel(JAISoundID soundID, f32 fxMix, f32 volume) {
    f32 var_f31 = mDolby;
    return Z2EnvSeBase::startEnvSeLevel(soundID, fxMix, volume, mPan, mDolby, 1.0f, 0);
}

Z2EnvSeAutoPan::Z2EnvSeAutoPan() {
    mPan = 0.5f;
    mDolby = 0.5f;
    field_0x10 = 0.0018554000416770577f;
    field_0x14 = 0.0008553999941796064f;
    field_0x18 = 1;
    field_0x19 = 1;
}

Z2EnvSeAutoPan::~Z2EnvSeAutoPan() {}

void Z2EnvSeAutoPan::setPanParam(f32 pan, f32 dolby, bool param_3, bool param_4, f32 param_5,
                                 f32 param_6) {
    mPan = pan;
    mDolby = dolby;
    field_0x10 = param_5;
    field_0x14 = param_6;
    field_0x18 = param_3;
    field_0x19 = param_4;
}

void Z2EnvSeAutoPan::calcPan() {
    if (field_0x18) {
        mPan += field_0x10;
        if (mPan > 1.0f) {
            mPan = 2.0f - mPan;
            field_0x18 = false;
        }
    } else {
        mPan -= field_0x10;
        if (mPan < 0.0f) {
            mPan *= -1.0f;
            field_0x18 = true;
        }
    }

    if (field_0x19) {
        mDolby += field_0x14;
        if (mDolby > 1.0f) {
            mDolby = 2.0f - mDolby;
            field_0x19 = false;
        }
    } else {
        mDolby -= field_0x14;
        if (mDolby < 0.0f) {
            mDolby *= -1.0f;
            field_0x19 = true;
        }
    }

    if (*getHandle()) {
        (*getHandle())->getAuxiliary().movePan(mPan, 0);
        (*getHandle())->getAuxiliary().moveDolby(mDolby, 0);
    }
}

bool Z2EnvSeAutoPan::startEnvSeAutoPanLevel(JAISoundID soundID, f32 fxMix, f32 volume,
                                            f32 pitch) {
    f32 var_f31 = mDolby;
    return Z2EnvSeBase::startEnvSeLevel(soundID, fxMix, volume, mPan, mDolby, pitch, 0);
}

Z2EnvSeMgr::Z2EnvSeMgr()
    : JASGlobalInstance<Z2EnvSeMgr>(true), field_0x38(), field_0x48(1.0f), field_0x4c(1.0f),
      field_0x50(1.0f), field_0x140(), mSnowPower(), mWindType(), field_0x188(), field_0x1ac(1.0f),
      field_0x1b0(1.0f), field_0x1b4(1.0f), field_0x1b8(), field_0x1e0(), field_0x1e4(1.0f),
      field_0x1e8(1.0f), field_0x1ec(1.0f), field_0x214(), field_0x218(1.0f), field_0x21c(1.0f),
      field_0x220(1.0f), field_0x248(), field_0x24c(1.0f), field_0x250(1.0f), field_0x254(1.0f),
      field_0x27c(), field_0x280(1.0f), field_0x284(1.0f), field_0x288(1.0f), field_0x2b0(),
      field_0x2b4(1.0f), field_0x2b8(1.0f), field_0x2bc(1.0f), field_0x2e4(), mFogDensity()
{
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x54 = 0.0f;
    field_0x58 = 0.0f;
    field_0x5c = 0.0f;
    field_0x39 = 0;
    field_0x2e8.setParam(0.1f, 0.007823764f, 0.06f);
    field_0x2f8.setParam(0.2f, 0.007823764f, 0.06f);
    field_0x90.setParam(0.1f, 0.026302524f, 0.005f);
    field_0xa0.setParam(0.9f, 0.023310114f, 0.003f);
    field_0xb0.setParam(0.2f, 0.0063851574f, 0.004f);
    field_0xc0.setParam(0.8f, 0.006188805f, 0.0045f);
    field_0xd0.setParam(0.3f, 0.053432982f, 0.002f);
    field_0xe0.setParam(0.7f, 0.042566273f, 0.0025f);
    field_0xf0.setParam(0.4f, 0.012933955f, 0.006f);
    field_0x100.setParam(0.6f, 0.012157413f, 0.0065f);
    field_0x18c.setParam(0.1f, 0.023310114f, 0.005f);
    field_0x19c.setParam(0.5f, 0.053432982f, 0.0025f);
    field_0x64.setVolumeScale(1.0f);
    mWolfSmellSeMgr.setVolumeScale(1.0f);
    field_0x120.setParam(0.5f, 0.020923574f, 0.001f);
    field_0x130.setParam(0.1f, 0.0037908785f, 0.005f);
}

Z2EnvSeMgr::~Z2EnvSeMgr() {}

void Z2EnvSeMgr::framework() {
    int sceneNum = Z2GetSceneMgr()->getCurrentSceneNum();
    if (field_0x64.getPosCount() >= 0) {
        if (field_0x214 == 0x12 ||
             /* dSv_event_flag_c::M_071 - Cutscene - [cutscene: 20] Zant appears (during Midna's desperate hour) */
            (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[104])
                /* dSv_event_flag_c::F_0250 - Cutscene - [cutscene: 21] reunion with Zelda / Midna revived (Hyrule Castle barrier appears) */
            && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[250])))
        {
            int lVar12 = 0;
            if (sceneNum == Z2SCENE_CASTLE_TOWN_INTERIOR || sceneNum == Z2SCENE_CASTLE_TOWN_SHOPS || (sceneNum == Z2SCENE_STAR_TENT)) {
                lVar12 = 1;
            }
            startRainSe(lVar12, 100);
        }

        field_0x64.resetMultiSePos();
    }

    if (sceneNum == Z2SCENE_LAKEBED_TEMPLE) {
        startLv3WaterSe(-1);
        field_0x140 = 0;
    }

    if (Z2GetSceneMgr()->isSceneExist()) {
        playSceneEnvSe();
        if (field_0x140 > 0 && mSnowPower == 0) {
            JAISoundHandle* handle = field_0x80.getHandle();
            if (*handle) {
                (*handle)->updateLifeTime(field_0x140);
            }
            handle = field_0x88.getHandle();
            if (*handle) {
                (*handle)->updateLifeTime(field_0x140);
            }
            field_0x140 = 0;
        }
    }

    if (mFogDensity > 0.0f) {
        mFogDensity -= 0.01f;
        if (mFogDensity < 0.0f) {
            mFogDensity = 0.0f;
        }
    }

    if (field_0x1b8 > 0 && field_0x1b8 < 100) {
        field_0x1b8++;
    } else if (field_0x1b8 < 0 && field_0x1b8 > -100) {
        field_0x1b8--;
    }

    if (mSnowPower > 0) {
        mSnowPower--;
    }

    if (Z2GetLink() != NULL) {
        if (Z2GetLink()->isWolfSmellOpen()) {
            if (field_0x2e4 < 30) {
                field_0x2e4++;
            }

            if (mWolfSmellSeMgr.getPosCount() >= 0) {
                f32 maxVolume = mWolfSmellSeMgr.getMaxVolume();
                field_0x2dc.startEnvSeLevel(Z2SE_ENV_SCENT_GRAVE, 0.0f,
                    (field_0x2e4 / 30.0f) * maxVolume, mWolfSmellSeMgr.getPanPower(), mWolfSmellSeMgr.getDolbyPower(), 1.0f, 0);
                mWolfSmellSeMgr.resetMultiSePos();
            }
            return;
        }
    }

    if (field_0x2e4 != 0) {
        field_0x2e4--;
    }
}

void Z2EnvSeMgr::resetScene() {}

void Z2EnvSeMgr::resetSceneInner() {
    if (-1.0f == field_0x60) {
        field_0x38 = 0;
        field_0x48 = 1.0f;
        field_0x4c = 1.0f;
        field_0x50 = 1.0f;
        field_0x54 = 0.0f;
        field_0x58 = 0.0f;
        field_0x5c = 0.0f;
        field_0x39 = 0;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
    }

    field_0x140 = 0;
    mWindType = WIND_TYPE_0;
    field_0x188 = 0;
    field_0x1e0 = 0;
    field_0x214 = 0;
    field_0x248 = 0;
    field_0x27c = 0;
    field_0x2b0 = 0;
    field_0x1b8 = 0;
    field_0x1ac = 1.0f;
    field_0x1b0 = 1.0f;
    field_0x1b4 = 1.0f;
    field_0x1e4 = 1.0f;
    field_0x1e8 = 1.0f;
    field_0x1ec = 1.0f;
    field_0x218 = 1.0f;
    field_0x21c = 1.0f;
    field_0x220 = 1.0f;
    field_0x24c = 1.0f;
    field_0x250 = 1.0f;
    field_0x254 = 1.0f;
    field_0x280 = 1.0f;
    field_0x284 = 1.0f;
    field_0x288 = 1.0f;
    field_0x2b4 = 1.0f;
    field_0x2b8 = 1.0f;
    field_0x2bc = 1.0f;

    mRiverSeMgr.resetMultiSePos();
    mFallSeMgr.resetMultiSePos();
    mEtcSeMgr.resetMultiSePos();
    field_0x224.resetMultiSePos();
    field_0x258.resetMultiSePos();
    field_0x28c.resetMultiSePos();
    mWolfSmellSeMgr.resetMultiSePos();
    field_0x64.resetMultiSePos();
}

void Z2EnvSeMgr::initSceneEnvSe(s32 sceneID, s8 room, f32 param_3) {
    field_0x60 = -1.0f;
    if (-1.0f == param_3) {
        return;
    }

    u8 r6 = 100.0f * param_3;
    u8 uVar3 = 0;

    switch(sceneID) {
    case Z2SCENE_HYRULE_FIELD:
        switch(room) {
        case 13:
            uVar3 = 10;
            break;
        case 7:
        case 9:
        case 11:
        case 12:
            uVar3 = 2;
            break;
        case 8:
        case 10:
        default:
            uVar3 = 6;
            break;
        }
        break;
    case Z2SCENE_CASTLE_TOWN_GATES:
        switch(room) {
        case 8:
        case 16:
            uVar3 = 6;
            break;
        case 17:
            uVar3 = 10;
            break;
        }
        break;
    case Z2SCENE_HYLIA_BRIDGE_BATTLE:
        uVar3 = 10;
        break;
    case Z2SCENE_ELDIN_BRIDGE_BATTLE:
        uVar3 = 6;
        break;
    case Z2SCENE_FINAL_BATTLE_CUTSCENE:
        uVar3 = 7;
        break;
    }
    
    initStaticEnvSe(uVar3, r6, 0xff, 0xff, NULL);
    field_0x60 = param_3;
}

void Z2EnvSeMgr::playSceneEnvSe() {
    if (-1.0f != field_0x60) {
        if (field_0x1b8 != 100) {
            startStaticEnvSe(Z2GetSceneMgr()->getRoomReverb());
        }
        field_0x1b8 = 0;
    }
}

void Z2EnvSeMgr::initStaticEnvSe(u8 param_1, u8 param_2, u8 param_3, u8 param_4, Vec* param_5) {
    f32 dVar7;
    f32 dVar6;
    f32 fVar1;

    if (-1.0f == field_0x60) {
        if (param_2 >= 100) {
            dVar7 = 1.0f;
        } else {
            dVar7 = 0.01f * param_2;
        }

        if (param_3 == 0xff) {
            dVar6 = 1.0f;
        } else {
            dVar6 = 0.01f * param_3;
        }

        if (param_4 == 0xff) {
            fVar1 = 0.0f;
        } else {
            field_0x3c = *param_5;
            fVar1 = 0.1f * param_4;
        }

        if (field_0x38 != 0 && field_0x38 == param_1) {
            field_0x39 = 100;
            if (dVar7 != field_0x48) {
                field_0x54 =
                    (dVar7 - field_0x48) / 100.0f;
            } else {
                field_0x48 = dVar7;
                field_0x54 = 0.0f;
            }

            if (dVar6 != field_0x4c) {
                field_0x58 =
                    (dVar6 - field_0x4c) / 100.0f;
            } else {
                field_0x4c = dVar6;
                field_0x58 = 0.0f;
            }

            if (fVar1 != field_0x50) {
                field_0x5c =
                    (fVar1 - field_0x50) / 100.0f;
            } else {
                field_0x50 = fVar1;
                field_0x5c = 0.0f;
            }
        } else {
            field_0x48 = dVar7;
            field_0x4c = dVar6;
            field_0x50 = fVar1;
            field_0x54 = 0.0f;
            field_0x58 = 0.0f;
            field_0x5c = 0.0f;
        }

        switch (param_1) {
        case 1:
            field_0x0.setPanParam(
                1.0f, 0.0f,
                true, false, 0.0018554000416770577f, 0.0008553999941796064f);
            field_0x1c.setPanParam(
                0.0f,
                1.0f, false, true, 0.0018554000416770577f,
                0.0008553999941796064f);
            break;
        case 2:
            field_0x0.setPanParam(
                1.0f, 0.0f,
                true, false, 0.0018554000416770577f, 0.0008553999941796064f);
            field_0x1c.setPanParam(
                0.0f,
                1.0f, false, true, 0.0018554000416770577f,
                0.0008553999941796064f);
            break;
        case 3:
            field_0x0.setPanParam(
                1.0f, 0.0f,
                true, false, 0.0018554000416770577f, 0.0008553999941796064f);
            field_0x1c.setPanParam(
                0.0f,
                1.0f, false, true, 0.0018554000416770577f,
                0.0008553999941796064f);
            break;
        case 4:
            field_0x0.setPanParam(
                1.0f, 0.0f,
                true, false, 0.0018554000416770577f, 0.0008553999941796064f);
            field_0x1c.setPanParam(
                0.0f,
                1.0f, false, true, 0.0018554000416770577f,
                0.0008553999941796064f);
            break;
        case 5:
        case 11:
            field_0x0.setPanParam(
                1.0f, 0.0f,
                true, false, 0.0018554000416770577f, 0.0008553999941796064f);
            field_0x1c.setPanParam(
                0.0f,
                1.0f, false, true, 0.0018554000416770577f,
                0.0008553999941796064f);
            break;
        case 6:
        case 10:
            field_0x0.setPanParam(
                1.0f, 0.0f,
                true, false, 0.0018554000416770577f, 0.0008553999941796064f);
            field_0x1c.setPanParam(
                0.0f,
                1.0f, false, true, 0.0018554000416770577f,
                0.0008553999941796064f);
            break;
        case 7:
            field_0x0.setPanParam(
                1.0f, 0.0f,
                true, false, 0.0018554000416770577f, 0.0008553999941796064f);
            field_0x1c.setPanParam(
                0.0f,
                1.0f, false, true, 0.0018554000416770577f,
                0.0008553999941796064f);
            break;
        case 8:
            field_0x0.setPanParam(
                1.0f, 0.0f,
                true, false, 0.0018554000416770577f, 0.0008553999941796064f);
            field_0x1c.setPanParam(
                0.0f,
                1.0f, false, true, 0.0018554000416770577f,
                0.0008553999941796064f);
            break;
        case 9:
            field_0x0.setPanParam(
                1.0f, 0.0f,
                true, false, 0.0018554000416770577f, 0.0008553999941796064f);
            field_0x1c.setPanParam(
                0.0f,
                1.0f, false, true, 0.0018554000416770577f,
                0.0008553999941796064f);
        }

        field_0x38 = param_1;
    }
}

bool Z2EnvSeMgr::startStaticEnvSe(s8 reverb) {
    field_0x1b8 = 100;

    f32 local_90 = reverb / 127.0f;
    if (field_0x39 != 0) {
        field_0x48 += field_0x54;
        field_0x4c += field_0x58;
        field_0x50 += field_0x5c;
        field_0x39--;
        if (field_0x39 == 0) {
            field_0x54 = 0.0f;
            field_0x58 = 0.0f;
            field_0x5c = 0.0f;
        }
    }

    field_0x0.calcPan();
    field_0x1c.calcPan();

    JAISoundID aJStack_94;
    JAISoundID aJStack_98 = 0xFFFFFFFF;
    f32 local_9c = 1.0f;
    if (0.0f != field_0x50) {
        Vec VStack_8c;
        Z2GetAudience()->convertAbsToRel(field_0x3c, &VStack_8c, 0);
        local_9c = Z2GetAudience()->calcRelPosVolume(VStack_8c, field_0x50, 0);
    }

    f32 dVar22 = field_0x48 * local_9c;
    f32 dVar21 = field_0x48 * local_9c;
    f32 dVar20 = field_0x4c;
    f32 dVar18 = field_0x4c;
    u8 hour = Z2GetStatusMgr()->getHour();

    switch (field_0x38) {
    case 0:
        return false;
    case 1:
        if (hour >= 6 && hour <= 18) {
            aJStack_94 = Z2SE_ENV_FOREST_DAY;
            aJStack_98 = Z2SE_ENV_FORESTWIND;
            dVar21 *= 0.7f;
        } else {
            aJStack_94 = Z2SE_ENV_FORESTWIND;
            aJStack_98 = Z2SE_ENV_FORESTWIND;
        }
        break;
    case 2:
        if (hour >= 6 && hour <= 18) {
            dVar22 *= 0.7f;
            dVar21 *= 0.7f;
        }
        aJStack_94 = JA_SE_ATM_WIND_HOT;
        aJStack_98 = JA_SE_ATM_WIND_HOT;
        dVar20 = 2.0f;
        dVar18 = 2.1f;
        break;
    case 3:
        aJStack_94 = JA_SE_ATM_WIND_HOT;
        aJStack_98 = JA_SE_ATM_WIND_HOT;
        break;
    case 4:
        aJStack_94 = Z2SE_ENV_SNOW_MT_WIND_L;
        aJStack_98 = Z2SE_ENV_SNOW_MT_WIND_R;
        if (mSnowPower > 64) {
            dVar22 *= Z2Calc::getParamByExp(mSnowPower,
                64.0f, 127.0f,
                0.4f, 0.35f,
                1.0f, Z2Calc::CURVE_POSITIVE);
            dVar20 *= Z2Calc::getParamByExp(mSnowPower,
                64.0f, 127.0f,
                0.4f, 1.15f,
                1.6f, Z2Calc::CURVE_POSITIVE);
            dVar21 *= Z2Calc::getParamByExp(mSnowPower,
                64.0f, 127.0f,
                0.3f, 0.2f,
                1.0f, Z2Calc::CURVE_POSITIVE);
            dVar18 *= Z2Calc::getParamByExp(mSnowPower,
                127.0f, 64.0f,
                0.4f, 0.65f,
                0.9f, Z2Calc::CURVE_NEGATIVE);
        } else {
             dVar22 *= Z2Calc::getParamByExp(mSnowPower,
                0.0f, 64.0f,
                0.4f, 0.2f,
                0.35f, Z2Calc::CURVE_NEGATIVE);
            dVar20 *= Z2Calc::getParamByExp(mSnowPower,
                0.0f, 64.0f,
                0.3f, 1.0f,
                1.15f, Z2Calc::CURVE_NEGATIVE);
            dVar21 *= Z2Calc::getParamByExp(mSnowPower,
                0.0f, 64.0f,
                0.3f, 0.1f,
                0.2f, Z2Calc::CURVE_NEGATIVE);
            dVar18 *= ::Z2Calc::getParamByExp(mSnowPower,
                64.0f, 0.0f,
                0.4f, 0.9f,
                1.0f, Z2Calc::CURVE_POSITIVE);
        }
        break;
    case 11:
        dVar22 = 0.5f;
        dVar21 = 0.6f;
    case 5:
        aJStack_94 = Z2SE_ENV_WINDTAG_STRONG;
        aJStack_98 = Z2SE_ENV_WINDTAG_SLOW;
        break;
    case 6:
        if (hour >= 6 && hour <= 18) {
            dVar22 *= 0.1f;
            dVar21 *= 0.12f;
        } else {
            dVar22 *= 0.2f;
            dVar21 *= 0.2f;
        }
        aJStack_94 = Z2SE_ENV_FIELD_WIND_L;
        aJStack_98 = Z2SE_ENV_FIELD_WIND_R;
        break;
    case 10:
        if (hour >= 6 && hour <= 18) {
            dVar22 *= 0.1f;
            dVar21 *= 0.6f;
        } else {
            dVar22 *= 0.2f;
        }
        aJStack_94 = Z2SE_ENV_FIELD_WIND_L;
        aJStack_98 = Z2SE_ENV_VALLEY_WIND;
        break;
    case 7:
        dVar22 *= 0.85f;
        dVar21 *= 0.6f;
        aJStack_94 = Z2SE_ENV_WINDTAG_STRONG;
        aJStack_98 = Z2SE_ENV_WINDTAG_SLOW;
        break;
    case 8:
        aJStack_94 = Z2SE_ENV_ICED_VILLAGE_L;
        aJStack_98 = Z2SE_ENV_ICED_VILLAGE_R;
        break;
    case 9:
        aJStack_94 = Z2SE_ENV_OUGI_FIELD_L;
        aJStack_98 = Z2SE_ENV_OUGI_FIELD_R;
        break;    
    default:
        return false;
    }

    dVar22 *= 1.0f - getFogDensity();
    dVar21 *= 1.0f - getFogDensity();
    if (Z2GetSceneMgr()->isInDarkness()) {
        dVar22 *= 0.75f;
        dVar21 *= 0.75f;
    }

    JAISoundHandle* handle = field_0x0.getHandle();
    if (*handle) {
        if ((u32)(*handle)->getID() != (u32)aJStack_94) {
            (*handle)->stop(30);
            (*handle)->releaseHandle();
        }
    }

    handle = field_0x1c.getHandle();
    if (*handle) {
        if ((u32)(*handle)->getID() != (u32)aJStack_98) {
            (*handle)->stop(30);
            (*handle)->releaseHandle();
        }
    }

    bool iVar9 = field_0x0.startEnvSeAutoPanLevel(aJStack_94, local_90, dVar22, dVar20);
    bool iVar11 = field_0x1c.startEnvSeAutoPanLevel(aJStack_98, local_90, dVar21, dVar18);

    if (field_0x38 == 4) {
        JAISoundHandle* handle = field_0x0.getHandle();
        JAISoundHandle* handle2 = field_0x1c.getHandle();
        if (*handle) {
            Z2GetSoundStarter()->setPortData(handle, 6, mSnowPower, -1);
        }
        if (*handle2) {
            Z2GetSoundStarter()->setPortData(handle2, 6, mSnowPower, -1);
        }
    }

    return iVar9 && iVar11;
}

void Z2EnvSeMgr::registWindowPos(Vec* posPtr) {
    field_0x64.registMultiSePos(posPtr);
}

bool Z2EnvSeMgr::startRainSe(s32 param_1, s8 param_2) {
    if (mSnowPower > 0) {
        return false;
    }

    mSnowPower = 1;

    JAISoundID aJStack_118 = 0xffffffff;
    JAISoundID aJStack_11c = 0xffffffff;
    JAISoundID aJStack_120 = 0xffffffff;
    f32 dVar21 = param_2 / 127.0f;
    s8 unaff_r29;

    switch (param_1) {
    case 0:
        aJStack_118 = Z2SE_ENV_RAIN_NORMAL_L;
        aJStack_11c = Z2SE_ENV_RAIN_NORMAL_R;
        aJStack_120 = Z2SE_ENV_RAIN_ROOM;
        unaff_r29 = 100;
        break;
    case 1:
        aJStack_118 = Z2SE_ENV_RAIN_HARD_L;
        aJStack_11c = Z2SE_ENV_RAIN_HARD_R;
        aJStack_120 = Z2SE_ENV_RAIN_HARD_ROOM;
        unaff_r29 = 50;
        break;
    }

    if (field_0x140 < unaff_r29) {
        field_0x140++;
    } else if (field_0x140 > unaff_r29) {
        field_0x140 = unaff_r29;
    }

    if (field_0x64.getPosCount() < 0) {
        f32 dVar13 = Z2Calc::linearTransform(field_0x90.calcNoise1f(), 0.0f, 1.0f, 0.7f, 1.0f, true);
        f32 dVar14 = Z2Calc::linearTransform(field_0xa0.calcNoise1f(), 0.0f, 1.0f, 0.7f, 1.0f, true);
        f32 dVar15 = Z2Calc::linearTransform(field_0xd0.calcNoise1f(), 0.0f, 1.0f, 0.8f, 1.1f, true);
        f32 dVar16 = Z2Calc::linearTransform(field_0xe0.calcNoise1f(), 0.0f, 1.0f, 0.8f, 1.1f, true);
        f32 dVar17 = Z2Calc::getParamByExp(field_0xb0.calcNoise1f(), 0.0f, 1.0f, 0.3f, 0.0f, 0.3f, Z2Calc::CURVE_POSITIVE);
        f32 dVar18 = Z2Calc::getParamByExp(field_0xc0.calcNoise1f(), 0.0f, 1.0f, 0.3f, 1.0f, 0.7f, Z2Calc::CURVE_POSITIVE);
        f32 dVar19 = Z2Calc::getParamByExp(field_0xf0.calcNoise1f(), 0.0f, 1.0f, 0.3f, 0.0f, 1.0f, Z2Calc::CURVE_POSITIVE);
        f32 dVar20 = Z2Calc::getParamByExp(field_0x100.calcNoise1f(), 0.0f, 1.0f, 0.3f, 0.0f, 1.0f, Z2Calc::CURVE_POSITIVE);

        if (Z2GetSceneMgr()->isInDarkness()) {
            dVar13 *= 0.5f;
            dVar14 *= 0.5f;
        }

        JAISoundHandle* handle = field_0x80.getHandle();
        if (*handle) {
            if ((*handle)->getID() != (u32)aJStack_118) {
                (*handle)->stop(30);
                (*handle)->releaseHandle();
            }
        }

        handle = field_0x88.getHandle();
        if (*handle) {
            if ((*handle)->getID() != (u32)aJStack_11c) {
                (*handle)->stop(30);
                (*handle)->releaseHandle();
            }
        }

        bool iVar7 = field_0x80.startEnvSeLevel(aJStack_118, dVar21, dVar13, dVar17, dVar19, dVar15, 0);
        bool iVar9 = field_0x88.startEnvSeLevel(aJStack_11c, dVar21, dVar14, dVar18, dVar20, dVar16, 0);
        return iVar7 && iVar9;
    }

    f32 dVar13 = Z2Calc::linearTransform(field_0x90.calcNoise1f(), 0.0f, 1.0f, 0.7f, 1.0f, true);
    f32 dVar14 = Z2Calc::linearTransform(field_0xd0.calcNoise1f(), 0.0f, 1.0f, 0.8f, 1.1f, true);

    JAISoundHandle* handle = field_0x80.getHandle();
    if (*handle) {
        if ((*handle)->getID() != (u32)aJStack_120) {
            (*handle)->stop(30);
            (*handle)->releaseHandle();
        }
    }

    f32 maxVolume = field_0x64.getMaxVolume();
    return field_0x80.startEnvSeLevel(aJStack_120, dVar21, dVar13 * maxVolume,
                                      field_0x64.getPanPower(), field_0x64.getDolbyPower(), dVar14,
                                      0);
}

void Z2EnvSeMgr::startNearThunderSe(s8 param_1) {
    if (field_0x64.getPosCount() < 0) {
        f32 dVar10 = Z2Calc::getRandom(0.25f, Z2Calc::cEqualCSlope, Z2Calc::cEqualPSlope);
        dVar10 += 0.5f;
        f32 dVar8 = Z2Calc::getRandom(0.25f, Z2Calc::cEqualCSlope, Z2Calc::cEqualPSlope);
        dVar8 += 0.5f;
        field_0x118.startEnvSe(Z2SE_ENV_THUNDER_NEAR, param_1 / 127.0f, 1.0f, dVar10, dVar8, 1.0f, 0);
    } else {
        f32 maxVolume = field_0x64.getMaxVolume();
        field_0x118.startEnvSe(Z2SE_ENV_THUND_NEAR_WIN, param_1 / 127.0f, maxVolume,
                               field_0x64.getPanPower(), field_0x64.getDolbyPower(), 1.0f, 0);
    }
}

void Z2EnvSeMgr::startFarThunderSe(Vec* posPtr, s8 reverb) {
    if (field_0x64.getPosCount() < 0) {
        field_0x110.setPos(posPtr);
        field_0x110.startEnvSe(Z2SE_ENV_THUNDER_FAR, reverb / 127.0f, 1.0f, 0.5f, 0.0f, 1.0f, 0);
    } else {
        f32 maxVolume = field_0x64.getMaxVolume();
        field_0x110.startEnvSe(Z2SE_ENV_THUND_FAR_WIN, reverb / 127.0f, maxVolume,
                               field_0x64.getPanPower(), field_0x64.getDolbyPower(), 1.0f, 0);
    }
}

void Z2EnvSeMgr::setSnowPower(s8 power) {
    mSnowPower = power;
}

void Z2EnvSeMgr::initStrongWindSe() {
    mWindType = WIND_TYPE_3;
}

void Z2EnvSeMgr::setWindDirection(Vec* dir) {
    if (mWindType == WIND_TYPE_3) {
        field_0x144.setPanDir(const_cast<Vec*>(&Z2Calc::cNullVec));
    } else {
        field_0x144.setPanDir(dir);
    }
}

bool Z2EnvSeMgr::startStrongWindSe(s8 reverb) {
    field_0x144.calcPan(1.0f);

    JAISoundID aJStack_24 = 0xffffffff;
    switch (mWindType) {
    case WIND_TYPE_1:
        aJStack_24 = Z2SE_ENV_WINDTAG_STRONG;
        break;
    case WIND_TYPE_2:
        aJStack_24 = Z2SE_ENV_WINDTAG_SLOW;
        break;
    case WIND_TYPE_3:
        aJStack_24 = Z2SE_ENV_WINDTAG_CEASE;
        break;
    default:
        return false;
    }
  
    f32 fxMix = reverb / 127.0f;
    return field_0x144.startEnvSeDirLevel(aJStack_24, fxMix, 1.0f);
}

void Z2EnvSeMgr::initRiverSe(u8 param_1, u8 param_2, u8 param_3, u8 param_4) {
    field_0x188 = param_1;
    if (param_2 >= 100) {
        field_0x1ac = 1.0f;
    } else {
        field_0x1ac = 0.01f * param_2;
    }

    if (param_3 == 0xff) {
        field_0x1b0 = 1.0f;
    } else {
        field_0x1b0 = 0.01f * param_3;
    }

    if (param_4 == 0xff) {
        field_0x1b4 = 1.0f;
    } else {
        field_0x1b4 = 0.1f * param_4;
    }

    switch (param_1) {
    case 1:
        mRiverSeMgr.setVolumeScale(1.5f * field_0x1b4);
        break;
    case 2:
        mRiverSeMgr.setVolumeScale(3.0f * field_0x1b4);
        break;
    case 3:
        mRiverSeMgr.setVolumeScale(0.5f * field_0x1b4);
        break;
    case 5:
        mRiverSeMgr.setVolumeScale(1.5f * field_0x1b4);
        break;
    }
}

void Z2EnvSeMgr::registRiverSePos(Vec* posPtr) {
    f32 var_f31;
    if (posPtr != NULL) {
        
    } else {
        var_f31 = posPtr->z;
    }
    
    mRiverSeMgr.registMultiSePos(posPtr);
}

void Z2EnvSeMgr::setHyrulSewerOpen(bool isSewerOpen) {
    if (isSewerOpen) {
        field_0x1b8 = 1;
        Z2GetSeMgr()->seStart(Z2SE_ENV_SEWER_FILL, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else {
        field_0x1b8 = 0xff;
        Z2GetSeMgr()->seStart(Z2SE_ENV_SEWER_OUT, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }
}

bool Z2EnvSeMgr::startRiverSe(s8 param_1) {
    #if DEBUG
    const char** spotName = sSpotName;
    #endif
    
    if (Z2GetStatusMgr()->mCameraMapInfo == 6) {
        switch (Z2GetSceneMgr()->getCurrentSceneNum()) {
        case Z2SCENE_FARON_WOODS:
            if (Z2GetSceneMgr()->getCurrentRoomNum() != 1) {
                break;
            }
        case Z2SCENE_LAKE_HYLIA:
            if (Z2GetStatusMgr()->mPolygonPosition.y < -14500.0f) {
                break;
            }
        case 8:
            if ((Z2GetStatusMgr()->mPolygonPosition.y > 2000.0f)) {
                break;
            }
        case Z2SCENE_ZORAS_DOMAIN:
        case Z2SCENE_HYRULE_FIELD:
        case Z2SCENE_CASTLE_TOWN_GATES:
        case Z2SCENE_LAKEBED_TEMPLE:
            break;
        default:
            registRiverSePos(&Z2GetStatusMgr()->mPolygonPosition);
            break;
        }
    }

    if (mRiverSeMgr.getPosCount() < 0) {
        return false;
    }
    JAISoundID sp3C = -1;
    bool r30 = false;
    f32 f31;
    f32 f29;
    f32 dolbyPower;
    f32 f30;
    f32 f27;
    f27 = param_1 / 127.0f;
    f31 = field_0x1ac * mRiverSeMgr.getMaxVolume();
    f29 = mRiverSeMgr.getPanPower();
    dolbyPower = mRiverSeMgr.getDolbyPower();
    f30 = field_0x1b0;

    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 sp0C;

    switch (field_0x188) {
    case 0:
        return false;
    case 1:
        sp3C = Z2SE_ENV_RIVER_SMALL;
        sp18 = dolbyPower;
        f31 *= Z2Calc::linearTransform(field_0x18c.calcNoise1f(), 0.0f, 1.0f, 0.7f, 1.0f,
                                          true);
        f30 *= Z2Calc::linearTransform(field_0x19c.calcNoise1f(), 0.0f, 1.0f, 0.8f, 1.1f,
                                          true);
        r30 = field_0x180.startEnvSeLevel(sp3C, f27, f31, f29, dolbyPower, f30, 0);
        break;
    case 2:
        sp3C = Z2SE_ENV_RIVER_NORMAL;
        mRiverSeMgr.getMaxPowL();
        mRiverSeMgr.getMaxPowR();
        sp14 = mRiverSeMgr.getDolbyPower();
        r30 = field_0x180.startEnvSeLevel(sp3C, f27, f31, f29, dolbyPower, f30, 0);
        break;
    case 3:
        if (field_0x1b8 > 0) {
            f31 *= (field_0x1b8 + 50) / 150.0f;
        } else {
            if (field_0x1b8 < 0) {
                f31 *= (field_0x1b8 + 150) / 150.0f;
            } else {
                f31 *= 0.3f;
            }
        }
        sp3C = Z2SE_ENV_SEWER;
        sp10 = dolbyPower;
        f31 *= Z2Calc::linearTransform(field_0x18c.calcNoise1f(), 0.0f, 1.0f, 0.6f, 1.0f, true);
        f30 *= Z2Calc::linearTransform(field_0x19c.calcNoise1f(), 0.0f, 1.0f, 0.8f, 1.0f, true);
        r30 = field_0x180.startEnvSeLevel(sp3C, f27, f31, f29, dolbyPower, f30, 0);
        break;
    case 5:
        sp3C = Z2SE_ENV_SPIRIT_FOUNTAIN;
        sp0C = dolbyPower;
        f31 *= Z2Calc::linearTransform(field_0x18c.calcNoise1f(), 0.0f, 1.0f, 0.6f, 1.0f, true);
        f30 *= Z2Calc::linearTransform(field_0x19c.calcNoise1f(), 0.0f, 1.0f, 0.8f, 1.0f, true);
        r30 = field_0x180.startEnvSeLevel(sp3C, f27, f31, f29, dolbyPower, f30, 0);
        break;
    }

    mRiverSeMgr.resetMultiSePos();
    return r30;
}

void Z2EnvSeMgr::initFallSe(u8 param_1, u8 param_2, u8 param_3, u8 param_4) {
    field_0x1e0 = param_1;
    if (param_2 >= 100) {
        field_0x1e4 = 1.0f;
    } else {
        field_0x1e4 = 0.01f * param_2;
    }

    if (param_3 == 0xff) {
        field_0x1e8 = 1.0f;
    } else {
        field_0x1e8 = 0.01f * param_3;
    }

    if (param_4 == 0xff) {
        field_0x1ec = 1.0f;
    } else {
        field_0x1ec = 0.1f * param_4;
    }

    switch (param_1) {
    case 8:
        mFallSeMgr.setVolumeScale(3.0f * field_0x1ec);
        break;
    case 9:
        mFallSeMgr.setVolumeScale(3.0f * field_0x1ec);
        break;
    case 10:
        mFallSeMgr.setVolumeScale(1.5f * field_0x1ec);
        break;
    case 11:
        mFallSeMgr.setVolumeScale(1.5f * field_0x1ec);
        break;
    }
}

void Z2EnvSeMgr::registFallSePos(Vec* posPtr) {
    f32 var_f31;
    if (posPtr != NULL) {
        
    } else {
        var_f31 = posPtr->z;
    }

    mFallSeMgr.registMultiSePos(posPtr);
}

bool Z2EnvSeMgr::startFallSe(s8 reverb) {
    if (mFallSeMgr.getPosCount() < 0) {
        return false;
    }

    JAISoundID soundID = 0xffffffff;
    bool rv = false;

    f32 fxMix = reverb / 127.0f;
    f32 volume = ((field_0x1e4 * mFallSeMgr.getMaxVolume()) * Z2Calc::linearTransform(field_0x90.calcNoise1f(), 0.0f, 1.0f, 0.7f, 1.0f, true));
    f32 pitch = (field_0x1e8 * Z2Calc::linearTransform(field_0xd0.calcNoise1f(), 0.0f, 1.0f, 0.8f, 1.1f, true));
    f32 pan = mFallSeMgr.getPanPower();
    f32 dolby = mFallSeMgr.getDolbyPower();

    switch (field_0x1e0) {
    case 8:
        soundID = Z2SE_ENV_WATERFALL_NORMAL;
        break;
    case 9:
        soundID = Z2SE_ENV_FALL_LV1BOSS;
        break;
    case 10:
        soundID = Z2SE_ENV_SPIRIT_FALL;
        pitch *= 0.75f;
        break;
    case 11:
        soundID = Z2SE_ENV_SAND_FALL;
        break;
    }

    rv = field_0x1d8.startEnvSeLevel(soundID, fxMix, volume, pan, dolby, pitch, 0);
    mFallSeMgr.resetMultiSePos();
    return rv;
}

void Z2EnvSeMgr::initEtcSe(u8 param_1, u8 param_2, u8 param_3, u8 param_4) {
    field_0x214 = param_1;
    if (param_2 >= 100) {
        field_0x218 = 1.0f;
    } else {
        field_0x218 = 0.01f * param_2;
    }

    if (param_3 == 0xff) {
        field_0x21c = 1.0f;
    } else {
        field_0x21c = 0.01f * param_3;
    }

    if (param_4 == 0xff) {
        field_0x220 = 1.0f;
    } else {
        field_0x220 = 0.1f * param_4;
    }

    switch (param_1) {
    case 17:
        mEtcSeMgr.setVolumeScale(2.0f * field_0x220);
        break;
    case 18:
        mEtcSeMgr.setVolumeScale(0.7f * field_0x220);
        break;
    case 25:
        field_0x1c.setPanParam(0.0f, 1.0f, false,
                               true, 0.0018554000416770577f, 0.0008553999941796064f);
    case 26:
    case 30:
    case 31:
        mEtcSeMgr.setVolumeScale(2.0f * field_0x220);
        break;
    case 20:
        mEtcSeMgr.setVolumeScale(field_0x220);
        break;
    case 16:
        mEtcSeMgr.setVolumeScale(2.0f * field_0x220);
        break;
    case 24:
        mEtcSeMgr.setVolumeScale(4.0f * field_0x1b4);
        break;
    }
}

void Z2EnvSeMgr::registEtcSePos(Vec* posPtr) {
    mEtcSeMgr.registMultiSePos(posPtr);
}

bool Z2EnvSeMgr::startEtcSe(s8 reverb) {
    #if DEBUG
    const char** spotName = sSpotName;
    #endif

    f32 dVar17 = reverb / 127.0f;
    f32 dVar19 = (field_0x218 * mEtcSeMgr.getMaxVolume());
    f32 dVar14 = mEtcSeMgr.getPanPower();
    f32 dVar15 = mEtcSeMgr.getDolbyPower();
    bool rv = false;
    f32 dVar18 = field_0x21c;
    f32 local_90, local_98, local_9c, local_a0;
    bool iVar5, iVar6, iVar7, iVar8;

    switch (field_0x214) {
    case 0x11:
        local_90 = Z2Calc::linearTransform(field_0x120.calcNoise1f(), 0.0f, 1.0f, 0.01f, 1.0f,
                                           true);
        dVar18 *= Z2Calc::linearTransform(field_0x130.calcNoise1f(), 0.0f, 1.0f, 0.8f, 1.2f,
                                          true);
        iVar8 = field_0x20c.startEnvSeLevel(Z2SE_OBJ_RANCH_WOODPLATE, dVar17,
                                                 (dVar19 * local_90), dVar14, dVar15, dVar18, 0);
        local_90 = Z2Calc::linearTransform(local_90, 0.0f, 1.0f, 0.3f, 1.0f, true);
        field_0x0.calcPan();
        field_0x1c.calcPan();
        iVar6 = field_0x0.startEnvSeAutoPanLevel(JA_SE_ATM_WIND_1, dVar17, local_90, dVar18);
        iVar7 = field_0x1c.startEnvSeAutoPanLevel(JA_SE_ATM_WIND_1, dVar17, 0.7f * local_90,
                                                       (0.8f * dVar18));
        rv = iVar8 && iVar6 && iVar7;
        break;
    case 0x12:
        local_90 = ::Z2Calc::linearTransform(field_0x120.calcNoise1f(), 0.0f, 1.0f, 0.4f, 0.8f,
                                             true);
        dVar18 *= ::Z2Calc::linearTransform(field_0x130.calcNoise1f(), 0.0f, 1.0f, 0.8f, 1.3f,
                                            true);
        rv = field_0x20c.startEnvSeLevel(JA_SE_OBJ_WIND_TAG, dVar17, (dVar19 * local_90), dVar14,
                                         dVar15, dVar18, 0);
        break;
    case 0x18:
        if (mEtcSeMgr.getPosCount() < 0) {
            return false;
        }
        rv = field_0x20c.startEnvSeLevel(JA_SE_MAGMA_L, dVar17, dVar19, dVar14, dVar15, dVar18, 0);
        break;
    case 0x1a:
    case 0x1e:
        if (mEtcSeMgr.getPosCount() < 0) {
            return false;
        }
        rv = field_0x20c.startEnvSeLevel(Z2SE_ENV_WATER_SURFACE, dVar17, dVar19, dVar14, dVar15,
                                         dVar18, 0);
        break;
    case 0x1f:
        if (mEtcSeMgr.getPosCount() < 0) {
            return false;
        }
        rv = field_0x20c.startEnvSeLevel(Z2SE_ENV_RIVER_NORMAL, dVar17, dVar19, dVar14, dVar15,
                                         0.89f * dVar18, 0);
        break;
    case 0x19:
        if (Z2GetStatusMgr()->mCameraMapInfo == 6 &&
            (Z2GetSceneMgr()->getCurrentSceneNum() != 0x10 ||
             Z2GetStatusMgr()->mPolygonPosition.y < -14500.0f))
        {
            f32 local_94 = (1.0f - dVar19);
            registEtcSePos(&Z2GetStatusMgr()->mPolygonPosition);
            dVar14 = mEtcSeMgr.getMaxVolume();
            dVar19 = field_0x218 * dVar14;
            dVar14 = mEtcSeMgr.getPanPower();
            dVar15 = mEtcSeMgr.getDolbyPower();
            if (Z2GetSceneMgr()->isInDarkness()) {
                dVar19 *= 0.75f;
            }
            local_94 *= dVar19;
            field_0x1c.calcPan();
            field_0x1c.startEnvSeAutoPanLevel(Z2SE_ENV_LAKE_OFFSHORE, dVar17, local_94, 1.0f);
        } else if (Z2GetSceneMgr()->isInDarkness()) {
            dVar19 *= 0.75f;
        }
        rv = field_0x20c.startEnvSeLevel(Z2SE_ENV_LAKEWATER, dVar17, dVar19, dVar14, dVar15, dVar18,
                                         0);
        break;
    case 0x14:
        local_90 = ::Z2Calc::linearTransform(field_0x120.calcNoise1f(), 0.0f, 1.0f, 0.4f, 0.8f,
                                             true);
        dVar18 *= ::Z2Calc::linearTransform(field_0x130.calcNoise1f(), 0.0f, 1.0f, 0.8f, 1.3f,
                                            true);
        rv = field_0x20c.startEnvSeLevel(Z2SE_ENV_SNOW_MT_WIND_ROOM, dVar17, (dVar19 * local_90),
                                         dVar14, dVar15, dVar18, 0);
        break;
    case 0x0:
        break;
    case 0x10:
        local_98 = Z2Calc::getParamByExp(getFogDensity(), 0.4f, 1.0f, 0.2f, 0.2f, 1.0f,
                                             Z2Calc::CURVE_NEGATIVE);
        local_9c = Z2Calc::getParamByExp(getFogDensity(), 0.4f, 1.0f, 0.2f, 0.75f, 1.0f,
                                             Z2Calc::CURVE_NEGATIVE);
        local_a0 = field_0x2e8.calcNoise1f();
        rv = field_0x240.startEnvSeLevel(Z2SE_ENV_PUPPET_FOG_L, 0.3f,
                                         (dVar19 * ((0.332f + 0.668f * local_a0) * local_98)),
                                         dVar14, dVar15, 0.8f + 0.2f * local_a0, 0);
        local_a0 = field_0x2f8.calcNoise1f();
        field_0x274.startEnvSeLevel(Z2SE_ENV_PUPPET_FOG_R, 0.3f,
                                    local_98 * (dVar19 * (0.332f + 0.668f * local_a0)), dVar14,
                                    dVar15, 0.8f + 0.2f * local_a0, 0);
        break;
    default:
        return false;
    }

    mEtcSeMgr.resetMultiSePos();
    return rv;
}

void Z2EnvSeMgr::registWolfSmellSePos(Vec* posPtr) {
    mWolfSmellSeMgr.registMultiSePos(posPtr);
}

void Z2EnvSeMgr::startFogWipeTrigger(Vec* pos) {
    JUT_ASSERT(1919, pos);

    if (*field_0x240.getHandle()) {
        f32 fogDensity = getFogDensity();
        u32 fogDensity2 = 340.0f * getFogDensity();
        Z2GetSoundStarter()->startSound(Z2SE_ENV_FOG_WIPE, field_0x20c.getHandle(),
                                               (JGeometry::TVec3<f32>*)pos, fogDensity2, 0.3f,
                                               1.0f, fogDensity, -1.0f, -1.0f, 0);
    }
}

void Z2EnvSeMgr::setFogWipeWidth(f32 width) {
    mFogDensity = Z2Calc::linearTransform(width, 0.0f, 1.0f, 1.0f, 0.4f, true);
}

f32 Z2EnvSeMgr::getFogDensity() {
    if (field_0x214 == 16) {
        return mFogDensity;
    }

    return 0.0f;
}

bool Z2EnvSeMgr::startFogSe() { 
    bool iVar5 = false;
    f32 dVar10 = field_0x2e8.calcNoise1f();
    f32 ratio = 0.332f;
    f32 fVar1 = 0.2f;
    f32 fVar2 = 0.4f;
    f32 dVar14 = ratio + ((1.0f - ratio) * dVar10);
    f32 dVar13 = (1.0f - fVar1) + (dVar10 * fVar1);
    f32 dVar12 = 0.5f - (dVar10 * fVar2) / 2;
    int iVar1 = 20;
    f32 fVar3 = 0.5f;
    bool iVar6 = field_0x240.startEnvSeLevel(Z2SE_ENV_PUPPET_FOG_L, iVar1, dVar14, dVar12, fVar3, dVar13, 0);

    dVar10 = field_0x2f8.calcNoise1f();
    dVar14 = (ratio + ((1.0f - ratio) * dVar10));
    dVar13 = ((1.0f - fVar1) + dVar10 * fVar1);
    dVar12 = (0.5f + (dVar10 * fVar2) / 2);
    iVar1 = 20;
    fVar3 = 0.5f;
    iVar5 = field_0x274.startEnvSeLevel(Z2SE_ENV_PUPPET_FOG_R, iVar1, dVar14, dVar12, fVar3, dVar13, 0);
    return (iVar6 && iVar5);
}

void Z2EnvSeMgr::initLv3WaterSe(u8 param_1, u8 param_2, u8 param_3, u8 param_4) {
    switch (param_1) {
    case 110:
        field_0x248 = param_1;
        if (param_2 >= 100) {
            field_0x24c = 1.0f;
        } else {
            field_0x24c = 0.01f * param_2;
        }
        
        if (param_3 == 0xff) {
            field_0x250 = 1.0f;
        } else {
            field_0x250 = 0.01f * param_3;
        }

        if (param_4 == 0xff) {
            field_0x254 = 1.0f;
        } else {
            field_0x254 = 0.1f * param_4;
        }
        field_0x224.setVolumeScale(2.0f * field_0x254);
        break;
    case 115:
        field_0x248 = param_1;
        if (param_2 >= 100) {
            field_0x24c = 1.0f;
        } else {
            field_0x24c = 0.01f * param_2;
        }

        if (param_3 == 0xff) {
            field_0x250 = 1.0f;
        } else {
            field_0x250 = 0.01f * param_3;
        }

        if (param_4 == 0xff) {
            field_0x254 = 1.0f;
        } else {
            field_0x254 = 0.1f * param_4;
        }
        field_0x224.setVolumeScale(1.5f * field_0x254);
        break;
    case 111:
        field_0x27c = param_1;
        if (param_2 >= 100) {
            field_0x280 = 1.0f;
        } else {
            field_0x280 = 0.01f * param_2;
        }

        if (param_3 == 0xff) {
            field_0x284 = 1.0f;
        } else {
            field_0x284 = 0.01f * param_3;
        }

        if (param_4 == 0xff) {
            field_0x288 = 1.0f;
        } else {
            field_0x288 = 0.1f * param_4;
        }

        field_0x258.setVolumeScale(2.0f * field_0x288);
        break;
    case 116:
        field_0x27c = param_1;
        if (param_2 >= 100) {
            field_0x280 = 1.0f;
        } else {
            field_0x280 = 0.01f * param_2;
        }

        if (param_3 == 0xff) {
            field_0x284 = 1.0f;
        } else {
            field_0x284 = 0.01f * param_3;
        }

        if (param_4 == 0xff) {
            field_0x288 = 1.0f;
        } else {
            field_0x288 = 0.1f * param_4;
        }

        field_0x258.setVolumeScale(1.5f * field_0x288);
        break;
    case 112:
        field_0x2b0 = param_1;
        if (param_2 >= 100) {
            field_0x2b4 = 1.0f;
        } else {
            field_0x2b4 = 0.01f *param_2;
        }

        if (param_3 == 0xff) {
            field_0x2b8 = 1.0f;
        } else {
            field_0x2b8 = 0.01f * param_3;
        }

        if (param_4 == 0xff) {
            field_0x2bc = 1.0f;
        } else {
            field_0x2bc = 0.1f * param_4;
        }

        field_0x28c.setVolumeScale(2.0f * field_0x2bc);
        break;
    case 117:
        field_0x2b0 = param_1;
        if (param_2 >= 100) {
            field_0x2b4 = 1.0f;
        } else {
            field_0x2b4 = 0.01f * param_2;
        }

        if (param_3 == 0xff) {
            field_0x2b8 = 1.0f;
        } else {
            field_0x2b8 = 0.01f * param_3;
        }

        if (param_4 == 0xff) {
            field_0x2bc = 1.0f;
        } else {
            field_0x2bc = 0.1f * param_4;
        }

        field_0x28c.setVolumeScale(1.5f * field_0x2bc);
        break;
    case 0x77:
        field_0x2b0 = param_1;
        if (param_2 >= 100) {
            field_0x2b4 = 1.0f;
        } else {
            field_0x2b4 = 0.01f * param_2;
        }

        if (param_3 == 0xff) {
            field_0x2b8 = 1.0f;
        } else {
            field_0x2b8 = 0.01f * param_3;
        }

        if (param_4 == 0xff) {
            field_0x2bc = 1.0f;
        } else {
            field_0x2bc = 0.1f * param_4;
        }

        field_0x28c.setVolumeScale(3.0f * field_0x2bc);
        break;
    default:
        if (Z2GetSceneMgr()->getCurrentSceneNum() == Z2SCENE_LAKEBED_TEMPLE) {
            switch (Z2GetSceneMgr()->getCurrentRoomNum()) {
            case 1:
                if (param_1 == 107) {
                    initFallSe(9, param_2, 0x82, 5);
                }
            case 0:
            case 5:
            case 6:
            case 9:
                if (param_1 == 100) {
                    initEtcSe(0x1e, param_2, param_3, param_4);
                }
                break;
            case 2:
                if (param_1 == 100) {
                    initEtcSe(0x1f, param_2, param_3, 20);
                } else if ((((param_1 == 103) || (param_1 == 104)) || (param_1 == 105)) ||
                           (param_1 == 106))
                {
                    initLv3WaterSe(115, param_2, param_3, param_4);
                }
                break;
            case 3:
                if (((param_1 == 100) || (param_1 == 101)) || (param_1 == 102)) {
                    initEtcSe(0x1e, param_2, param_3, param_4);
                } else if ((param_1 == 103) || (param_1 == 104)) {
                    initLv3WaterSe(115, param_2, param_3, param_4);
                } else if ((param_1 == 105) || (param_1 == 106)) {
                    initLv3WaterSe(117, param_2, param_3, param_4);
                } else if ((param_1 == 107) || (param_1 == 108)) {
                    initLv3WaterSe(111, param_2, param_3, 15);
                }
                break;
            case 7:
                if (param_1 == 103) {
                    initLv3WaterSe(115, param_2, param_3, param_4);
                } else if (param_1 == 107) {
                    initFallSe(9, param_2, 0x82, param_4);
                }
                break;
            case 8:
                if ((param_1 == 100) || (param_1 == 101)) {
                    initEtcSe(0x1e, param_2, param_3, param_4);
                } else if (param_1 == 103) {
                    initLv3WaterSe(115, param_2, param_3, param_4);
                } else if (param_1 == 107) {
                    initLv3WaterSe(111, param_2, param_3, param_4);
                }
                break;
            case 10:
                if (param_1 == 100) {
                    initEtcSe(0x1e, param_2, param_3, param_4);
                } else if (param_1 == 103) {
                    initLv3WaterSe(117, param_2, param_3, param_4);
                } else if (param_1 == 104) {
                    initLv3WaterSe(116, param_2, param_3, param_4);
                } else if (param_1 == 107) {
                    initFallSe(9, param_2, 0xa0, param_4);
                }
                break;
            case 11:
                if (param_1 == 100 || param_1 == 101) {
                    initEtcSe(0x1e, param_2, param_3, param_4);
                } else if (param_1 == 103) {
                    initLv3WaterSe(115, param_2, param_3, param_4);
                } else if (param_1 == 107) {
                    initFallSe(9, param_2, 0x82, param_4);
                }
                break;
            case 12:
                if (param_1 == 103) {
                    initLv3WaterSe(115, param_2, param_3, param_4);
                } else if (param_1 == 107) {
                    initLv3WaterSe(111, param_2, 0x82, param_4);
                }
                break;
            case 13:
                if ((param_1 == 100) || (param_1 == 101)) {
                    initEtcSe(0x1e, param_2, param_3, param_4);
                } else if (param_1 == 103) {
                    initLv3WaterSe(117, param_2, param_3, param_4);
                } else if (param_1 == 104) {
                    initLv3WaterSe(116, param_2, param_3, param_4);
                } else if (param_1 == 107) {
                    initFallSe(9, param_2, 0xa0, param_4);
                }
                break;
            }
        }
        break;
    }
}

void Z2EnvSeMgr::registLv3WaterSePos(u8 param_1, Vec* posPtr) {
    switch (param_1) {
    case 110:
    case 115:
        field_0x224.registMultiSePos(posPtr);
        break;
    case 111:
    case 116:
        field_0x258.registMultiSePos(posPtr);
        break;
    case 112:
    case 117:
    case 119:
        field_0x28c.registMultiSePos(posPtr);
        break;
    default:
        switch (Z2GetSceneMgr()->getCurrentRoomNum()) {
        case 1:
            if (param_1 == 107) {
                mFallSeMgr.registMultiSePos(posPtr);
            }
        case 0:
        case 5:
        case 6:
        case 9:
            if (param_1 == 100) {
                mEtcSeMgr.registMultiSePos(posPtr);
            }
            break;
        case 2:
            if (param_1 == 100) {
                mEtcSeMgr.registMultiSePos(posPtr);
            } else if (param_1 == 103 && dComIfGs_isSaveSwitch(8)) {
                field_0x224.registMultiSePos(posPtr);
            } else if (param_1 == 104 && dComIfGs_isSaveSwitch(9)) {
                field_0x224.registMultiSePos(posPtr);
            } else if (param_1 == 105 && dComIfGs_isSaveSwitch(8) && dComIfGs_isSaveSwitch(3)) {
                field_0x224.registMultiSePos(posPtr);
            } else if (param_1 == 106 && dComIfGs_isSaveSwitch(9) && dComIfGs_isSaveSwitch(1)) {
                field_0x224.registMultiSePos(posPtr);
            }
            break;
        case 3:
            if (param_1 == 100 && !dComIfGs_isSaveSwitch(8) && !dComIfGs_isSaveSwitch(9)) {
                mEtcSeMgr.registMultiSePos(posPtr);
            } else if ((param_1 == 101 && dComIfGs_isSaveSwitch(8) &&
                        (!dComIfGs_isSaveSwitch(9))) ||
                       (!dComIfGs_isSaveSwitch(8) && dComIfGs_isSaveSwitch(9)))
            {
                mEtcSeMgr.registMultiSePos(posPtr);
            } else if (param_1 == 102 && dComIfGs_isSaveSwitch(8) && dComIfGs_isSaveSwitch(9)) {
                mEtcSeMgr.registMultiSePos(posPtr);
            }
            if (param_1 == 103 && dComIfGs_isSaveSwitch(8)) {
                field_0x224.registMultiSePos(posPtr);
            }
            if (param_1 == 104 && dComIfGs_isSaveSwitch(9)) {
                field_0x224.registMultiSePos(posPtr);
            }
            if (param_1 == 105 && dComIfGs_isSaveSwitch(8) && dComIfGs_isSaveSwitch(3)) {
                field_0x28c.registMultiSePos(posPtr);
            } else if (param_1 == 107 && dComIfGs_isSaveSwitch(8) && !dComIfGs_isSaveSwitch(3)) {
                field_0x258.registMultiSePos(posPtr);
            }
            if (param_1 == 106 && dComIfGs_isSaveSwitch(9) && dComIfGs_isSaveSwitch(1)) {
                field_0x28c.registMultiSePos(posPtr);
            } else if (param_1 == 108 && dComIfGs_isSaveSwitch(9) && !dComIfGs_isSaveSwitch(1)) {
                field_0x258.registMultiSePos(posPtr);
            }
            break;
        case 7:
            if (dComIfGs_isSaveSwitch(8)) {
                if (param_1 == 103) {
                    field_0x224.registMultiSePos(posPtr);
                } else if (param_1 == 107) {
                    mFallSeMgr.registMultiSePos(posPtr);
                }
            }
            break;
        case 8:
            if (dComIfGs_isSaveSwitch(9) && dComIfGs_isSaveSwitch(1)) {
                if (param_1 == 101) {
                    mEtcSeMgr.registMultiSePos(posPtr);
                } else if (param_1 == 103) {
                    field_0x224.registMultiSePos(posPtr);
                } else if (param_1 == 107) {
                    field_0x258.registMultiSePos(posPtr);
                }
            } else if (param_1 == 100) {
                mEtcSeMgr.registMultiSePos(posPtr);
            }
            break;
        case 10:
            if ((param_1 == 100) && dComIfGs_isSaveSwitch(0x31)) {
                mEtcSeMgr.registMultiSePos(posPtr);
            } else if (param_1 == 103 && dComIfGs_isSaveSwitch(0x31)) {
                field_0x28c.registMultiSePos(posPtr);
            } else if (param_1 == 104 && (dComIfGs_isSaveSwitch(8))) {
                field_0x258.registMultiSePos(posPtr);
            } else if (param_1 == 107 && dComIfGs_isSaveSwitch(0x31)) {
                mFallSeMgr.registMultiSePos(posPtr);
            }
            break;
        case 11:
            if (dComIfGs_isSaveSwitch(9)) {
                if (param_1 == 101) {
                    mEtcSeMgr.registMultiSePos(posPtr);
                } else if (param_1 == 103) {
                    field_0x224.registMultiSePos(posPtr);
                } else if (param_1 == 107) {
                    mFallSeMgr.registMultiSePos(posPtr);
                }
            } else if (param_1 == 100) {
                mEtcSeMgr.registMultiSePos(posPtr);
            }
            break;
        case 12:
            if (dComIfGs_isSaveSwitch(8) && dComIfGs_isSaveSwitch(3)) {
                if (param_1 == 103) {
                    field_0x224.registMultiSePos(posPtr);
                } else if (param_1 == 107) {
                    field_0x258.registMultiSePos(posPtr);
                }
            }
            break;
        case 13:
            if (dComIfGs_isSaveSwitch(0x30)) {
                if (param_1 == 101) {
                    mEtcSeMgr.registMultiSePos(posPtr);
                } else if (param_1 == 103) {
                    field_0x28c.registMultiSePos(posPtr);
                } else if ((param_1 == 104) && (dComIfGs_isSaveSwitch(9))) {
                    field_0x258.registMultiSePos(posPtr);
                } else if (param_1 == 107) {
                    mFallSeMgr.registMultiSePos(posPtr);
                }
            } else if (param_1 == 100) {
                mEtcSeMgr.registMultiSePos(posPtr);
            }
        }
    }
}

void Z2EnvSeMgr::startLv3WaterSe(s8 reverb) {
    if (Z2GetSceneMgr()->getCurrentSceneNum() == Z2SCENE_LAKEBED_TEMPLE) {
        if (reverb >= 0) {
            field_0x140 = reverb;
            return;
        }
        reverb = field_0x140;
    }

    JAISoundID aJStack_60 = 0xffffffff;
    f32 dVar12 = reverb / 127.0f;
    f32 maxVolume;
    f32 panPower;
    f32 dolbyPower;

    if (field_0x224.getPosCount() >= 0) {
        if (field_0x248 == 0x73) {
            aJStack_60 = Z2SE_ENV_WATER_WAY;
        } else if (field_0x248 == 0x6e) {
            aJStack_60 = Z2SE_ENV_WATERFALL_CALM;
        }
        maxVolume = field_0x24c * field_0x224.getMaxVolume();
        panPower = field_0x224.getPanPower();
        dolbyPower = field_0x224.getDolbyPower();
        field_0x240.startEnvSeLevel(aJStack_60, dVar12,
                                     maxVolume, panPower, dolbyPower,
                                     field_0x250, 0);
        field_0x224.resetMultiSePos();
    }

    if (field_0x258.getPosCount() >= 0) {
        if (field_0x27c == 0x74) {
            aJStack_60 = Z2SE_ENV_WATER_WAY;
        } else if (field_0x27c == 0x6f) {
            aJStack_60 = Z2SE_ENV_WATERFALL_CALM;
        }

        maxVolume = field_0x280 * field_0x258.getMaxVolume();
        panPower = field_0x258.getPanPower();
        dolbyPower = field_0x258.getDolbyPower();
        field_0x274.startEnvSeLevel(aJStack_60, dVar12,
                                     maxVolume, panPower, dolbyPower,
                                     field_0x284, 0);
        field_0x258.resetMultiSePos();
    }

    if (field_0x28c.getPosCount() >= 0) {
        if (field_0x2b0 == 0x75) {
            aJStack_60 = Z2SE_ENV_WATER_WAY_L;
        } else if (field_0x2b0 == 0x70) {
            aJStack_60 = Z2SE_ENV_WATERFALL_CALM;
        } else if (field_0x2b0 == 0x77) {
            aJStack_60 = Z2SE_ENV_VALLEY_WIND;
        }
        maxVolume = field_0x2b4 * field_0x28c.getMaxVolume();
        panPower = field_0x28c.getPanPower();
        dolbyPower = field_0x28c.getDolbyPower();
        field_0x2a8.startEnvSeLevel(aJStack_60, dVar12,
                                     maxVolume, panPower, dolbyPower,
                                     field_0x2b8, 0);
        field_0x28c.resetMultiSePos();
    }

    startRiverSe(reverb);
    startFallSe(reverb);
    startEtcSe(reverb);
}
