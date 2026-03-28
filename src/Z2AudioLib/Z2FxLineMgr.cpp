#include "Z2AudioLib/Z2FxLineMgr.h"
#include "JSystem/JAHostIO/JAHUGlobal.h"
#include "JSystem/JAHostIO/JAHioMessage.h"
#include "JSystem/JAHostIO/JAHioUtil.h"
#include "JSystem/JAudio2/JASDSPInterface.h"
#include "JSystem/JHostIO/JORFile.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JSupport/JSUMemoryStream.h"
#include "Z2AudioLib/SpotName.h"
#include "Z2AudioLib/Z2SceneMgr.h"
#include "os_report.h"

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

    mFxLineBuffer[0] = JKR_NEW_ARRAY_ARGS(u8, 0x2800, heap, 0x20);
    mFxLineBuffer[1] = JKR_NEW_ARRAY_ARGS(u8, 0x2800, heap, 0x20);
    mFxLineBuffer[2] = JKR_NEW_ARRAY_ARGS(u8, 0x4B00, heap, 0x20);
    mFxLineBuffer[3] = JKR_NEW_ARRAY_ARGS(u8, 0x4B00, heap, 0x20);

    JUT_ASSERT(53, mFxLineBuffer[0]);
    JUT_ASSERT(54, mFxLineBuffer[1]);
    JUT_ASSERT(55, mFxLineBuffer[2]);
    JUT_ASSERT(56, mFxLineBuffer[3]);

    mFxDataNum = arc->countResource();
    mConfig = JKR_NEW_ARRAY_ARGS(Z2FxLineConfig, mFxDataNum, heap, 0);

    for (u8 i = 0; i < mFxDataNum; i++) {
        void* res = arc->getResource(i);
        u32 size = ALIGN_NEXT(arc->getResSize(res), 32);

        JSUMemoryInputStream stream(res, size);
        stream >> mConfig[i].unk0.unk0;
        stream >> mConfig[i].unk0.unk1;
        stream >> mConfig[i].unk0.unk2;
        stream >> mConfig[i].unk0.unk3;
        stream >> mConfig[i].unk0.unk4;
        stream >> mConfig[i].unk0.unk6;
        stream >> mConfig[i].unk0.unk8[0];
        stream >> mConfig[i].unk0.unk8[1];
        stream >> mConfig[i].unk0.unk8[2];
        stream >> mConfig[i].unk0.unk8[3];
        stream >> mConfig[i].unk0.unk8[4];
        stream >> mConfig[i].unk0.unk8[5];
        stream >> mConfig[i].unk0.unk8[6];
        stream >> mConfig[i].unk0.unk8[7];
        stream >> mConfig[i].unk18.unk0;
        stream >> mConfig[i].unk18.unk1;
        stream >> mConfig[i].unk18.unk2;
        stream >> mConfig[i].unk18.unk3;
        stream >> mConfig[i].unk18.unk4;
        stream >> mConfig[i].unk18.unk6;
        stream >> mConfig[i].unk18.unk8[0];
        stream >> mConfig[i].unk18.unk8[1];
        stream >> mConfig[i].unk18.unk8[2];
        stream >> mConfig[i].unk18.unk8[3];
        stream >> mConfig[i].unk18.unk8[4];
        stream >> mConfig[i].unk18.unk8[5];
        stream >> mConfig[i].unk18.unk8[6];
        stream >> mConfig[i].unk18.unk8[7];
        JKRFree(res);
    }

    setLineID(-1, false, false);
    setLineID(-1, true, false);

#if DEBUG
    if (mHIOEdit)
        mHIOEdit->update(false);
    if (mHIOEdit)
        mHIOEdit->update(true);
#endif
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
    dsp_config.field_0x0 = config->unk0.unk0;
    dsp_config.field_0x2 = config->unk0.unk1;
    dsp_config.field_0x6 = config->unk0.unk2;

    if (param_2) {
        dsp_config.field_0x4 = 0;
        dsp_config.field_0x8 = 0;
    } else {
        dsp_config.field_0x4 = config->unk0.unk4;
        dsp_config.field_0x8 = config->unk0.unk6;
    }

    dsp_config.field_0xc = config->unk0.unk3;
    dsp_config.field_0x10[0] = config->unk0.unk8[0];
    dsp_config.field_0x10[1] = config->unk0.unk8[1];
    dsp_config.field_0x10[2] = config->unk0.unk8[2];
    dsp_config.field_0x10[3] = config->unk0.unk8[3];
    dsp_config.field_0x10[4] = config->unk0.unk8[4];
    dsp_config.field_0x10[5] = config->unk0.unk8[5];
    dsp_config.field_0x10[6] = config->unk0.unk8[6];
    dsp_config.field_0x10[7] = config->unk0.unk8[7];
    JASDsp::setFXLine(buf1, (s16*)mFxLineBuffer[buf1], &dsp_config);
#if DEBUG
    if (mHIOEdit)
        mHIOEdit->syncSetting(buf1, &dsp_config);
#endif

    dsp_config.field_0x0 = config->unk18.unk0;
    dsp_config.field_0x2 = config->unk18.unk1;
    dsp_config.field_0x6 = config->unk18.unk2;

    if (param_2) {
        dsp_config.field_0x4 = 0;
        dsp_config.field_0x8 = 0;
    } else {
        dsp_config.field_0x4 = config->unk18.unk4;
        dsp_config.field_0x8 = config->unk18.unk6;
    }

    dsp_config.field_0xc = config->unk18.unk3;
    dsp_config.field_0x10[0] = config->unk18.unk8[0];
    dsp_config.field_0x10[1] = config->unk18.unk8[1];
    dsp_config.field_0x10[2] = config->unk18.unk8[2];
    dsp_config.field_0x10[3] = config->unk18.unk8[3];
    dsp_config.field_0x10[4] = config->unk18.unk8[4];
    dsp_config.field_0x10[5] = config->unk18.unk8[5];
    dsp_config.field_0x10[6] = config->unk18.unk8[6];
    dsp_config.field_0x10[7] = config->unk18.unk8[7];
    JASDsp::setFXLine(buf2, (s16*)mFxLineBuffer[buf2], &dsp_config);

#if DEBUG
    if (mHIOEdit)
        mHIOEdit->syncSetting(buf2, &dsp_config);
#endif
}

void Z2FxLineMgr::setFxForceOff(bool param_0) {
    setLineID(mLineID, true, param_0);
}

void Z2FxLineMgr::setUnderWaterFx(bool isUnderWaterFx) {
    if (mSetUnderWaterFx != isUnderWaterFx) {
        if (isUnderWaterFx) {
            OS_REPORT("[Z2FxLineMgr::setUnderWaterFx] → underWaterFx!\n");
            setLineID(12, true, false);

#if DEBUG
            if (mHIOEdit)
                mHIOEdit->update(true);
#endif
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
                break;
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
#if DEBUG
    if (mHIOEdit)
        mHIOEdit->update(true);
#endif
}

Z2FxLineEditNode::Z2FxLineEditNode(JKRExpHeap* heap) : JAHFrameNode("FxLineEdit"), mHeap(heap) {
    JUT_ASSERT(340, mHeap);
    unk74 = JKR_NEW_ARGS(mHeap, 0) LineStereo(false);
    appendNode(unk74, NULL);
    unk78 = JKR_NEW_ARGS(mHeap, 0) LineStereo(true);
    appendNode(unk78, NULL);
    Z2GetFxLineMgr()->setHIOEdit(this);
}

void Z2FxLineEditNode::message(JAHControl& control) {
    control.makeComment("Line0 : normal_mixing L", 0, 5, 0);
    control.makeComment("Line1 : normal_mixing R", 0, 5, 0);
    control.makeComment("Line2 : auto_mixing L", 0, 5, 0);
    control.makeComment("Line3 : auto_mixing R", 0, 5, 0);
    control.returnY(1);
    control.makeComment("-- ミックスモード --", 0, 5, 0);
    control.indent(1);
    control.makeComment("DFX_MIX_STOP   非動作", 0, 5, 0);
    control.makeComment("DFX_MIX_BEFORE 出力前にミキシング(第１反射音もFx効果あり)", 0, 5, 0);
    control.makeComment("DFX_MIX_AFTER  出力後にミキシング(第１反射音はFx効果なし)", 0, 5, 0);
    control.indent(-1);
    control.makeComment("-- センド先 --", 0, 5, 0);
    control.indent(1);
    control.makeComment("DFXS_MAIN_L", 0, 5, 0);
    control.makeComment("DFXS_MAIN_R", 0, 5, 0);
    control.makeComment("DFXS_FX1", 0, 5, 0);
    control.makeComment("DFXS_FX2", 0, 5, 0);
    control.makeComment("DFXS_FX3", 0, 5, 0);
    control.makeComment("DFXS_FX4", 0, 5, 0);
    control.makeComment("DFXS_FM", 0, 5, 0);
    control.makeComment("DFXS_DOLBY", 0, 5, 0);
    control.makeComment("DFXS_REV", 0, 5, 0);
    control.makeComment("DFXS_CHORUS", 0, 5, 0);
    control.makeComment("DFXS_MONO", 0, 5, 0);
    control.makeComment("DFXS_NONE", 0, 5, 0);
    control.indent(-1);
}

void Z2FxLineEditNode::syncSetting(u8 param_1, JASDsp::FxlineConfig_* config) {
    JUT_ASSERT(387, config);
    Z2FxLineConfigSingle* line_HIO;
    switch (param_1) {
    case 0:
        line_HIO = &unk74->unk70.unk0;
        break;
    case 1:
        line_HIO = &unk74->unk70.unk18;
        break;
    case 2:
        line_HIO = &unk78->unk70.unk0;
        break;
    case 3:
        line_HIO = &unk78->unk70.unk18;
        break;
    }
    JUT_ASSERT(407, line_HIO);
    line_HIO->unk0 = config->field_0x0;
    line_HIO->unk1 = config->field_0x2;
    line_HIO->unk4 = config->field_0x4;
    line_HIO->unk2 = config->field_0x6;
    line_HIO->unk6 = config->field_0x8;
    line_HIO->unk3 = config->field_0xc;
    line_HIO->unk8[0] = config->field_0x10[0];
    line_HIO->unk8[1] = config->field_0x10[1];
    line_HIO->unk8[2] = config->field_0x10[2];
    line_HIO->unk8[3] = config->field_0x10[3];
    line_HIO->unk8[4] = config->field_0x10[4];
    line_HIO->unk8[5] = config->field_0x10[5];
    line_HIO->unk8[6] = config->field_0x10[6];
    line_HIO->unk8[7] = config->field_0x10[7];
}

void Z2FxLineEditNode::update(bool param_1) {
    if (param_1)
        unk78->updateNode();
    else
        unk74->updateNode();
}

static const char* cBfcMask = "bfcファイル(*.bfc)\0*.bfc\0";

void Z2FxLineEditNode::saveLine(LineStereo* node) {
    JUT_ASSERT(436, node);
    JORFile file;
    if (file.open(NULL, 0x6, cBfcMask, NULL, NULL, NULL)) {
        // clang-format off
        JUT_ASSERT(441, mHeap!=0);
        // clang-format on
        u32 size = ALIGN_PREV(mHeap->getFreeSize(), 0x20);
        JKRHeap* backup = JKRGetCurrentHeap();
        JKRSetCurrentHeap(mHeap);
        void* saveBuffer = JKRAlloc(size, -1);
        JUT_ASSERT(452, saveBuffer);
        JKRSetCurrentHeap(backup);

        JSUMemoryOutputStream JStack_15c(saveBuffer, size);
        JStack_15c << node->unk70.unk0.unk0;
        JStack_15c << node->unk70.unk0.unk1;
        JStack_15c << node->unk70.unk0.unk2;
        JStack_15c << node->unk70.unk0.unk3;
        JStack_15c << node->unk70.unk0.unk4;
        JStack_15c << node->unk70.unk0.unk6;
        JStack_15c << node->unk70.unk0.unk8[0];
        JStack_15c << node->unk70.unk0.unk8[1];
        JStack_15c << node->unk70.unk0.unk8[2];
        JStack_15c << node->unk70.unk0.unk8[3];
        JStack_15c << node->unk70.unk0.unk8[4];
        JStack_15c << node->unk70.unk0.unk8[5];
        JStack_15c << node->unk70.unk0.unk8[6];
        JStack_15c << node->unk70.unk0.unk8[7];
        JStack_15c << node->unk70.unk18.unk0;
        JStack_15c << node->unk70.unk18.unk1;
        JStack_15c << node->unk70.unk18.unk2;
        JStack_15c << node->unk70.unk18.unk3;
        JStack_15c << node->unk70.unk18.unk4;
        JStack_15c << node->unk70.unk18.unk6;
        JStack_15c << node->unk70.unk18.unk8[0];
        JStack_15c << node->unk70.unk18.unk8[1];
        JStack_15c << node->unk70.unk18.unk8[2];
        JStack_15c << node->unk70.unk18.unk8[3];
        JStack_15c << node->unk70.unk18.unk8[4];
        JStack_15c << node->unk70.unk18.unk8[5];
        JStack_15c << node->unk70.unk18.unk8[6];
        JStack_15c << node->unk70.unk18.unk8[7];
        file.writeData(saveBuffer, JStack_15c.getPosition());
        file.close();
        JKRFree(saveBuffer);
    } else {
        JAHUDialog_OK("セーブに失敗しました.", "save", NULL);
    }
}

void Z2FxLineEditNode::loadLine(LineStereo* node) {
    JUT_ASSERT(436, node);
    JORFile file;
    void* dataArray = NULL;
    u32 length = 0;
    if (file.open(NULL, 0x1, cBfcMask, NULL, NULL, NULL)) {
        length = ALIGN_NEXT(file.getFileSize(), 0x20);
        dataArray = JKRAllocFromHeap(mHeap, length, 0x20);
        JUT_ASSERT(517, dataArray);
        u32 readSize = file.readData(dataArray, 0);
        file.close();
        if (readSize < 1) {
            JAHUDialog_OK("読み込み失敗。JORFile::readData()がfalseを返しました", "読み込み", NULL);
            JKRFreeToHeap(mHeap, dataArray);
        }
    } else {
        file.close();
        JAHUDialog_OK(JAHioUtil::getString("%s を読み込めません", file.getFilename()),
                      "データを読み込む", NULL);
    }

    JSUMemoryInputStream stream(dataArray, length);

    stream >> node->unk70.unk0.unk0;
    stream >> node->unk70.unk0.unk1;
    stream >> node->unk70.unk0.unk2;
    stream >> node->unk70.unk0.unk3;
    stream >> node->unk70.unk0.unk4;
    stream >> node->unk70.unk0.unk6;
    stream >> node->unk70.unk0.unk8[0];
    stream >> node->unk70.unk0.unk8[1];
    stream >> node->unk70.unk0.unk8[2];
    stream >> node->unk70.unk0.unk8[3];
    stream >> node->unk70.unk0.unk8[4];
    stream >> node->unk70.unk0.unk8[5];
    stream >> node->unk70.unk0.unk8[6];
    stream >> node->unk70.unk0.unk8[7];
    stream >> node->unk70.unk18.unk0;
    stream >> node->unk70.unk18.unk1;
    stream >> node->unk70.unk18.unk2;
    stream >> node->unk70.unk18.unk3;
    stream >> node->unk70.unk18.unk4;
    stream >> node->unk70.unk18.unk6;
    stream >> node->unk70.unk18.unk8[0];
    stream >> node->unk70.unk18.unk8[1];
    stream >> node->unk70.unk18.unk8[2];
    stream >> node->unk70.unk18.unk8[3];
    stream >> node->unk70.unk18.unk8[4];
    stream >> node->unk70.unk18.unk8[5];
    stream >> node->unk70.unk18.unk8[6];
    stream >> node->unk70.unk18.unk8[7];

    JKRFree(dataArray);
    node->updateNode();
}

void Z2FxLineEditNode::LineStereo::message(JAHControl& control) {
    control.makeButton("reset", 0, 3, 0);
    control.makeButton("save file", 2, 3, 0);
    control.makeButton("load file", 3, 3, 0);
    control.returnY(1);
    control.makeComment("----- Line L -----", 0, 5, 0);
    control.indent(1);
    lineEditSingle(&unk70.unk0, control);
    control.indent(-1);
    control.returnY(1);
    control.makeComment("----- Line R -----", 0, 5, 0);
    control.indent(1);
    lineEditSingle(&unk70.unk18, control);
    control.indent(-1);
}

void Z2FxLineEditNode::LineStereo::propertyEvent(JAH_P_Event param_1, uintptr_t param_2) {
    if (param_1 == JAH_P_EVENT0)
        return;

    Z2FxLineEditNode* parent = (Z2FxLineEditNode*)getParent();
    switch (param_2) {
    case 0:
        Z2GetFxLineMgr()->setLineID(-1, mIsDolby, false);
        break;

    case 2:
        parent->saveLine(this);
        break;

    case 3:
        parent->loadLine(this);
        break;
    }

    Z2GetFxLineMgr()->setLine(&unk70, mIsDolby, false);
}

void Z2FxLineEditNode::LineStereo::lineEditSingle(Z2FxLineConfigSingle* param_1,
                                                  JAHControl& param_2) {
    if (param_1 == NULL)
        return;

    param_2.startComboBox("mixmode", &param_1->unk0, 3, 0);
    param_2.makeComboBoxItem("DFX_MIX_STOP", (u8)0);
    param_2.makeComboBoxItem("DFX_MIX_BEFORE", (u8)1);
    param_2.makeComboBoxItem("DFX_MIX_AFTER", (u8)2);
    param_2.endComboBox();

    param_2.returnY(1);

    param_2.startComboBox("send1", &param_1->unk1, 3, 0);
    param_2.makeComboBoxItem("DFXS_MAIN_L", (u8)0);
    param_2.makeComboBoxItem("DFXS_MAIN_R", (u8)1);
    param_2.makeComboBoxItem("DFXS_FX1", (u8)2);
    param_2.makeComboBoxItem("DFXS_FX2", (u8)3);
    param_2.makeComboBoxItem("DFXS_FX3", (u8)4);
    param_2.makeComboBoxItem("DFXS_FX4", (u8)5);
    param_2.makeComboBoxItem("DFXS_FM", (u8)6);
    param_2.makeComboBoxItem("DFXS_DOLBY", (u8)7);
    param_2.makeComboBoxItem("DFXS_REV", (u8)8);
    param_2.makeComboBoxItem("DFXS_CHORUS", (u8)9);
    param_2.makeComboBoxItem("DFXS_MONO", (u8)10);
    param_2.makeComboBoxItem("DFXS_NONE", (u8)11);
    param_2.endComboBox();

    param_2.returnY(1);
    param_2.makeSlider("mix1", &param_1->unk4, 0, 0x7fff, 0);

    param_2.startComboBox("send2", &param_1->unk2, 3, 0);
    param_2.makeComboBoxItem("DFXS_MAIN_L", (u8)0);
    param_2.makeComboBoxItem("DFXS_MAIN_R", (u8)1);
    param_2.makeComboBoxItem("DFXS_FX1", (u8)2);
    param_2.makeComboBoxItem("DFXS_FX2", (u8)3);
    param_2.makeComboBoxItem("DFXS_FX3", (u8)4);
    param_2.makeComboBoxItem("DFXS_FX4", (u8)5);
    param_2.makeComboBoxItem("DFXS_FM", (u8)6);
    param_2.makeComboBoxItem("DFXS_DOLBY", (u8)7);
    param_2.makeComboBoxItem("DFXS_REV", (u8)8);
    param_2.makeComboBoxItem("DFXS_CHORUS", (u8)9);
    param_2.makeComboBoxItem("DFXS_MONO", (u8)10);
    param_2.makeComboBoxItem("DFXS_NONE", (u8)11);
    param_2.endComboBox();

    param_2.returnY(1);
    param_2.makeSlider("mix2", &param_1->unk6, 0, 0x7fff, 0);

    if (mIsDolby)
        param_2.makeSlider("feedback_frame", &param_1->unk3, 0, 120, 0);
    else
        param_2.makeSlider("feedback_frame", &param_1->unk3, 0, 64, 0);

    param_2.makeSlider("feedback_filter 0", &param_1->unk8[0], -0x7fff, 0x7fff, 0);
    param_2.makeSlider("feedback_filter 1", &param_1->unk8[1], -0x7fff, 0x7fff, 0);
    param_2.makeSlider("feedback_filter 2", &param_1->unk8[2], -0x7fff, 0x7fff, 0);
    param_2.makeSlider("feedback_filter 3", &param_1->unk8[3], -0x7fff, 0x7fff, 0);
    param_2.makeSlider("feedback_filter 4", &param_1->unk8[4], -0x7fff, 0x7fff, 0);
    param_2.makeSlider("feedback_filter 5", &param_1->unk8[5], -0x7fff, 0x7fff, 0);
    param_2.makeSlider("feedback_filter 6", &param_1->unk8[6], -0x7fff, 0x7fff, 0);
    param_2.makeSlider("feedback_filter 7", &param_1->unk8[7], -0x7fff, 0x7fff, 0);
}
