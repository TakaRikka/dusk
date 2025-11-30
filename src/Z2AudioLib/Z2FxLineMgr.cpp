#include "Z2AudioLib/Z2FxLineMgr.h"
#include "Z2AudioLib/Z2SceneMgr.h"
#include "JSystem/JAudio2/JASDSPInterface.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JSupport/JSUMemoryStream.h"

Z2FxLineMgr::Z2FxLineMgr() : JASGlobalInstance<Z2FxLineMgr>(true) {
    mConfig = NULL;
    mLineID = -1;
    mFxDataNum = 0;
    mSetUnderWaterFx = false;

    #if DEBUG
    mHIOEdit = NULL;
    #endif
}

void Z2FxLineMgr::initDataArc(JKRArchive* arc, JKRHeap* heap) {
    JUT_ASSERT(44, arc);
    JUT_ASSERT(45, heap);

    mFxLineBuffer[0] = new (heap, 0x20) u8[0x2800];
    mFxLineBuffer[1] = new (heap, 0x20) u8[0x2800];
    mFxLineBuffer[2] = new (heap, 0x20) u8[0x4B00];
    mFxLineBuffer[3] = new (heap, 0x20) u8[0x4B00];

    JUT_ASSERT(53, mFxLineBuffer[0]);
    JUT_ASSERT(54, mFxLineBuffer[1]);
    JUT_ASSERT(55, mFxLineBuffer[2]);
    JUT_ASSERT(56, mFxLineBuffer[3]);

    mFxDataNum = arc->countResource();
    mConfig = new (heap, 0) Z2FxLineConfig[mFxDataNum];

    for (u8 i = 0; i < mFxDataNum; i++) {
        void* res = arc->getResource(i);
        u32 size = (arc->getResSize(res) + 0x1f) & ~0x1f;

        JSUMemoryInputStream stream(res, size);
        stream >> mConfig[i].field_0x0;
        stream >> mConfig[i].field_0x1;
        stream >> mConfig[i].field_0x2;
        stream >> mConfig[i].field_0x3;
        stream >> mConfig[i].field_0x4;
        stream >> mConfig[i].field_0x6;
        stream >> mConfig[i].field_0x8[0];
        stream >> mConfig[i].field_0x8[1];
        stream >> mConfig[i].field_0x8[2];
        stream >> mConfig[i].field_0x8[3];
        stream >> mConfig[i].field_0x8[4];
        stream >> mConfig[i].field_0x8[5];
        stream >> mConfig[i].field_0x8[6];
        stream >> mConfig[i].field_0x8[7];
        stream >> mConfig[i].field_0x18;
        stream >> mConfig[i].field_0x19;
        stream >> mConfig[i].field_0x1a;
        stream >> mConfig[i].field_0x1b;
        stream >> mConfig[i].field_0x1c;
        stream >> mConfig[i].field_0x1e;
        stream >> mConfig[i].field_0x20[0];
        stream >> mConfig[i].field_0x20[1];
        stream >> mConfig[i].field_0x20[2];
        stream >> mConfig[i].field_0x20[3];
        stream >> mConfig[i].field_0x20[4];
        stream >> mConfig[i].field_0x20[5];
        stream >> mConfig[i].field_0x20[6];
        stream >> mConfig[i].field_0x20[7];
        JKRFree(res);
    }

    setLineID(-1, false, false);
    setLineID(-1, true, false);
}

void Z2FxLineMgr::setLineID(s8 fxID, bool param_1, bool param_2) {
    JUT_ASSERT(109, fxID < mFxDataNum);

    if (fxID < 0) {
        mLineID = 0;
    } else {
        mLineID = fxID;
    }

    setLine(&mConfig[mLineID], param_1, param_2);
}

void Z2FxLineMgr::setLine(Z2FxLineConfig* config, bool param_1, bool param_2) {
    JUT_ASSERT(121, config);

    u8 buf1 = 0;
    u8 buf2 = 1;
    if (param_1) {
        buf1 = 2;
        buf2 = 3;
    }

    if (mFxLineBuffer[buf1] == NULL || mFxLineBuffer[buf2] == NULL) {
        OS_REPORT("[Z2FxLineMgr::setLine] FxBuffer is NOT CREATED!!\n");
        return;
    }

    JASDsp::FxlineConfig_ dsp_config;
    dsp_config.field_0x0 = config->field_0x0;
    dsp_config.field_0x2 = config->field_0x1;
    dsp_config.field_0x6 = config->field_0x2;

    if (param_2) {
        dsp_config.field_0x4 = 0;
        dsp_config.field_0x8 = 0;
    } else {
        dsp_config.field_0x4 = config->field_0x4;
        dsp_config.field_0x8 = config->field_0x6;
    }

    dsp_config.field_0xc = config->field_0x3;
    dsp_config.field_0x10[0] = config->field_0x8[0];
    dsp_config.field_0x10[1] = config->field_0x8[1];
    dsp_config.field_0x10[2] = config->field_0x8[2];
    dsp_config.field_0x10[3] = config->field_0x8[3];
    dsp_config.field_0x10[4] = config->field_0x8[4];
    dsp_config.field_0x10[5] = config->field_0x8[5];
    dsp_config.field_0x10[6] = config->field_0x8[6];
    dsp_config.field_0x10[7] = config->field_0x8[7];
    JASDsp::setFXLine(buf1, (s16*)mFxLineBuffer[buf1], &dsp_config);

    dsp_config.field_0x0 = config->field_0x18;
    dsp_config.field_0x2 = config->field_0x19;
    dsp_config.field_0x6 = config->field_0x1a;

    if (param_2) {
        dsp_config.field_0x4 = 0;
        dsp_config.field_0x8 = 0;
    } else {
        dsp_config.field_0x4 = config->field_0x1c;
        dsp_config.field_0x8 = config->field_0x1e;
    }

    dsp_config.field_0xc = config->field_0x1b;
    dsp_config.field_0x10[0] = config->field_0x20[0];
    dsp_config.field_0x10[1] = config->field_0x20[1];
    dsp_config.field_0x10[2] = config->field_0x20[2];
    dsp_config.field_0x10[3] = config->field_0x20[3];
    dsp_config.field_0x10[4] = config->field_0x20[4];
    dsp_config.field_0x10[5] = config->field_0x20[5];
    dsp_config.field_0x10[6] = config->field_0x20[6];
    dsp_config.field_0x10[7] = config->field_0x20[7];
    JASDsp::setFXLine(buf2, (s16*)mFxLineBuffer[buf2], &dsp_config);
}

void Z2FxLineMgr::setFxForceOff(bool param_0) {
    setLineID(mLineID, true, param_0);
}

void Z2FxLineMgr::setUnderWaterFx(bool isUnderWaterFx) {
    if (mSetUnderWaterFx != isUnderWaterFx) {
        if (isUnderWaterFx) {
            OS_REPORT("[Z2FxLineMgr::setUnderWaterFx] → underWaterFx!\n");
            setLineID(12, true, false);
        } else {
            OS_REPORT("[Z2FxLineMgr::setUnderWaterFx] → sceneFx!\n");
            setSceneFx(Z2GetSceneMgr()->getCurrentSceneNum());
        }
        mSetUnderWaterFx = isUnderWaterFx;
    }
}

void Z2FxLineMgr::setSceneFx(s32 sceneNo) {
    if (Z2GetSceneMgr()->isInDarkness()) {
        setLineID(11, true, false);
    } else {
        switch (sceneNo) {
        case Z2SCENE_SHADES_REALM:
            setLineID(9, true, false);
            break;
        case Z2SCENE_KAKARIKO_VILLAGE:
        case Z2SCENE_DEATH_MOUNTAIN:
        case Z2SCENE_HIDDEN_VILLAGE:
        case Z2SCENE_ZORAS_RIVER:
        case Z2SCENE_HYRULE_FIELD:
            setLineID(10, true, false);
            break;
        case Z2SCENE_SACRED_GROVE:
        case Z2SCENE_FOREST_TEMPLE:
        case Z2SCENE_FOREST_TEMPLE_MINIBOSS:
        case Z2SCENE_FOREST_TEMPLE_BOSS:
        case Z2SCENE_GORGE_LANTERN_CAVE:
        case Z2SCENE_LAKE_LANTERN_CAVE:
        case Z2SCENE_FARON_WOODS_CAVE:
            if (Z2GetSceneMgr()->getCurrentRoomNum() == 2) {
                return;
            }
            // fallthrough
        case Z2SCENE_FARON_WOODS:
            setLineID(1, true, false);
            break;
        case Z2SCENE_GORON_MINES:
        case Z2SCENE_GORON_MINES_MINIBOSS:
        case Z2SCENE_GORON_MINES_BOSS:
        case Z2SCENE_ELDIN_MAGNET_CAVE:
            setLineID(2, true, false);
            break;
        case Z2SCENE_LAKE_HYLIA:
        case Z2SCENE_LAKEBED_TEMPLE:
        case Z2SCENE_LAKEBED_TEMPLE_MINIBOSS:
        case Z2SCENE_LAKEBED_TEMPLE_BOSS:
            if (Z2GetSceneMgr()->getCurrentRoomNum() == 1) {
                setLineID(3, true, false);
            }
            break;
        case Z2SCENE_ARBITERS_GROUNDS:
        case Z2SCENE_ARBITERS_GROUNDS_MINIBOSS:
        case Z2SCENE_ARBITERS_GROUNDS_BOSS:
            setLineID(4, true, false);
            break;
        case Z2SCENE_SNOWPEAK_RUINS:
        case Z2SCENE_SNOWPEAK_RUINS_MINIBOSS:
        case Z2SCENE_SNOWPEAK_RUINS_BOSS:
        case Z2SCENE_ICE_BLOCK_PUZZLE:
            setLineID(5, true, false);
            break;
        case Z2SCENE_TEMPLE_OF_TIME:
        case Z2SCENE_TEMPLE_OF_TIME_MINIBOSS:
        case Z2SCENE_TEMPLE_OF_TIME_BOSS:
            setLineID(6, true, false);
            break;
        case Z2SCENE_CITY_IN_THE_SKY:
        case Z2SCENE_CITY_IN_THE_SKY_MINIBOSS:
        case Z2SCENE_CITY_IN_THE_SKY_BOSS:
            setLineID(7, true, false);
            break;
        case Z2SCENE_HYRULE_CASTLE:
        case Z2SCENE_FINAL_BATTLE_THRONE_ROOM:
            setLineID(8, true, false);
            break;
        default:
            setLineID(-1, true, false);
            break;
        }
    }
}
