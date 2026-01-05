#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_event_debug.h"
#include "JSystem/JHostIO/JORFile.h"
#include "JSystem/JHostIO/JORServer.h"
#include "d/actor/d_a_player.h"
#include "m_Do/m_Do_controller_pad.h"

static dEvDb_bit_c dEvDb_flag_bit_table[799] = {
    #include "d/d_event_debug_bit_table.inc"
};

static dEvDb_reg_c dEvDb_flag_reg_table[21] = {
    #include "d/d_event_debug_reg_table.inc"
};

static dEvDb_flag_base_c dEvDb_flag_base_table = {
    dEvDb_flag_bit_table, // mBitTable
    dEvDb_flag_reg_table, // mRegTable
    799,                  // mTotalBitNum
    21,                   // mTotalRegNum
    43,                   // mNumRootBits (exact value)
    6                     // mNumRootRegs (There's actually 5 unique root regs)
};

static dEvDb_bit_c dEvDb_flag_bit_table_tmp[169] = {
    #include "d/d_event_debug_bit_table_tmp.inc"
};

static dEvDb_reg_c dEvDb_flag_reg_table_tmp[14] = {
    #include "d/d_event_debug_reg_table_tmp.inc"
};

static dEvDb_flag_base_c dEvDb_flag_base_table_tmp = {
    dEvDb_flag_bit_table_tmp, // mBitTable
    dEvDb_flag_reg_table_tmp, // mRegTable
    169,                      // mTotalBitNum
    14,                       // mTotalRegNum
    22,                       // mNumRootBits (Exact)
    4                         // mNumRootRegs (There's actually 3 unique root regs)
};

dEvM_HIO_c::dEvM_HIO_c() {
    field_0x004 = -1;
    m_EVdata_output = false;
    m_evm_debug = false;
    m_evd_debug = false;
    m_eve_debug = false;
    m_evd_unfinished = false;

    m_prioritize_file = false;  
    if (mDoCPd_c::getHoldL(PAD_2))
        m_prioritize_file = true;

    m_enable_skip = false;
    field_0x00C = false;
    m_prevent_scene_switch = false;

    field_0x010 = 80.0f;
    field_0x014 = 110.0f;
    field_0x018 = 210.0f;

    m_corrective_sound_adjustment = 0;

    m_save_bit.mFlagTables = &dEvDb_flag_base_table;
    m_temp_bit.mFlagTables = &dEvDb_flag_base_table_tmp;
    m_save_reg.mFlagTables = &dEvDb_flag_base_table;
    m_temp_reg.mFlagTables = &dEvDb_flag_base_table_tmp;
}

void dEvM_HIO_c::genMessage(JORMContext* mctx) {
    mctx->genLabel("- ", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    /* "- Event Manager Testing" */
    mctx->genLabel("- イベントマネージャーテスト用", LBL_EVENT_MANAGER_TESTING, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Playback test"
    mctx->genNode("再生テスト", &m_playtest, 0, 0);

    // "Save B"
    mctx->genNode("セーブＢ", &m_save_bit, 0, 0);

    // "Temporary B"
    mctx->genNode("一時Ｂ", &m_temp_bit, 0, 0);

    // "Save R"
    mctx->genNode("セーブＲ", &m_save_reg, 0, 0);

    // "Temporary R"
    mctx->genNode("一時Ｒ", &m_temp_reg, 0, 0);

    // "Read"
    mctx->genButton("読み込み", BTN_READ, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "evM Debug"
    mctx->genCheckBox("evM デバック", &m_evm_debug, 1, 0, NULL, 0xffff,  0xffff, 0x200, 0x18);

    // "evD Debug"
    mctx->genCheckBox("evD デバック", &m_evd_debug, 1, 0, NULL, 0xffff,  0xffff, 0x200, 0x18);

    // "eve Debug"
    mctx->genCheckBox("eve デバック", &m_eve_debug, 1, 0, NULL, 0xffff,  0xffff, 0x200, 0x18);

    // "EV data output"
    mctx->genCheckBox("ＥＶデータ出力", &m_EVdata_output, 1, 0, NULL , 0xffff, 0xffff, 0x200, 0x18);

    // "evD unfinished"
    mctx->genCheckBox("evD 未終了", &m_evd_unfinished, 1, 0, NULL, 0xffff , 0xffff, 0x200, 0x18);

    // "Scene switching prohibited"
    mctx->genCheckBox("シーン切替禁止", &m_prevent_scene_switch, 1, 0,  NULL, 0xffff, 0xffff, 0x200, 0x18);

  
    mctx->genLabel("- ", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    /* "- Map Tool Data" */
    mctx->genLabel("- マップツールデータ", 0, 0, NULL, 0xffff, 0xffff, 0x200 , 0x18);

    // "Corrective Sound Adjustment"
    mctx->genSlider("　正解音調整",&m_corrective_sound_adjustment, -200, 200, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);


    mctx->genLabel("- ",0,0,NULL,0xffff,0xffff,0x200,0x18);

    /* "- Event Data" */
    mctx->genLabel("- イベントデータ", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Continuously prioritize"
    mctx->genCheckBox("常駐優先", &m_prioritize_file, 1, 0, NULL, 0xffff , 0xffff, 0x200, 0x18);


    /* "- Apply then Reset" */
    mctx->genLabel("- つけてから要リセット", 0, 0, NULL, 0xffff, 0xffff,  0x200, 0x18);

    // "Skip enabled"
    mctx->genCheckBox("スキップ有効", &m_enable_skip, 1, 0, NULL,  0xffff, 0xffff, 0x200, 0x18);


    mctx->genLabel("- ", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    mctx->genLabel("- ", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Forced termination"
    mctx->genButton("強制終了", BTN_FORCED_TERMINATION, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

void dEvM_HIO_c::listenPropertyEvent(const JORPropertyEvent* param_0) {
    static char* debug_data;
    JORFile eventDataFile;

    JORReflexible::listenPropertyEvent(param_0);

    switch (reinterpret_cast<u32>(param_0->id)) {
    case BTN_READ:
        if(!debug_data) {
            // "Fly? Soar?" or maybe something along the lines of "Order change?", not entirely sure of translation...
            OS_REPORT("飛ぶか？そるか？\n");
            debug_data = static_cast<char*>(JKRAlloc(0x32000,0));
        }

        // "Event data (*.dat)"
        // Extra null terminators & *.dat needed for .data section to match??
        // Maybe the result of some kind of macro for formatting extension masks?
        if (debug_data && eventDataFile.open(1, "イベントデータ(*.dat)\0*.dat\0\0\0", NULL, NULL, NULL)) {
            eventDataFile.readData(debug_data, 0);
            eventDataFile.close();

            // Update available events
            m_playtest.removeComboBox();
            dComIfGp_getPEvtManager()->setDbgData(debug_data);
            m_playtest.addComboBox();
        }
        break;
    case BTN_READ + 1:
        break;
    case BTN_FORCED_TERMINATION:
        // "Forced termination" button
        dComIfGp_getEvent()->reset();
        break;
    }
}

bool dEvM_HIO_c::setDebugCameraData(void* buffer) {
    if (m_playtest.mEventCameraMode != dEvM_play_HIO_c::UNSET)
        return false;

    if (dComIfGp_event_runCheck())
        return false;

    dComIfGp_getPEvtManager()->setDbgData(static_cast<const char*>(buffer));

    m_playtest.mTargetEvent = 0;
    m_playtest.mEventIdx = 0;
    m_playtest.mEventCameraMode = dEvM_play_HIO_c::PLAYBACK;
    m_playtest.field_0xA = 0;

    return true;
}

void dEvM_root_bit_HIO_c::genMessage(JORMContext* mctx) {
    u8 i = 0;
    for (; i < ARRAY_SIZEU(mBit) && i < mFlagTables->mNumRootBits; i++) {
        mBit[i].mFlagTables = mFlagTables;
        mBit[i].mRootBitIdx = i;

        if (mFlagTables->searchDirNameBit(i))
            mctx->genNode(mFlagTables->searchDirNameBit(i), &mBit[i], 0, 0);
    }
}

void dEvM_root_reg_HIO_c::genMessage(JORMContext* mctx) {
    u8 i = 0;
    for (; i < ARRAY_SIZEU(mReg) && i < mFlagTables->mNumRootRegs; i++) {
        mReg[i].mFlagTables = mFlagTables;
        mReg[i].mRootRegIdx = i;

        if (mFlagTables->searchDirNameReg(i))
            mctx->genNode(mFlagTables->searchDirNameReg(i), &mReg[i], 0, 0);
    }
}

// "None"
const char* non_text = "無し";

dEvM_play_HIO_c::dEvM_play_HIO_c() {
    mTargetEvent = 0;
    mEventIdx = 0;
    mEventCameraMode = UNSET;
    field_0xA = 0;
}

void dEvM_play_HIO_c::genMessage(JORMContext* mctx) {
    event_binary_data_header* const header = dComIfGp_getPEvtManager()->getHeader(1);
    dEvDtEvent_c* const list = dComIfGp_getPEvtManager()->getEventList(1);

    /* "Event Playback Test" */
    mctx->genLabel("イベントの再生テスト", LBL_EVENT_PLAYBACK_TESTING, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Playback"
    mctx->genButton("再生", BTN_PLAYBACK, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Stop"
    mctx->genButton("停止", BTN_STOP, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Target event"
    mctx->startComboBox("対象イベント", &mTargetEvent, 0, NULL, 0xffff, 0xffff, 0x100, 0x1a);

    int i;
    if (!header) {
        mctx->genComboBoxItem(non_text,0);
    } else {
        for (i = 0; i < header->eventNum; i++) {
            mctx->genComboBoxItem(list[i].getName(), i);
        }
    }

    mctx->endComboBox();
}

void dEvM_play_HIO_c::listenPropertyEvent(const JORPropertyEvent* param_0) {
    JORReflexible::listenPropertyEvent(param_0);

    mEventCameraMode = UNSET;
    switch (reinterpret_cast<u32>(param_0->id)) {
    case BTN_PLAYBACK:
        mEventCameraMode = PLAYBACK;
        break;
    case BTN_STOP:
        mEventCameraMode = STOP;
        break;
    }
}

void dEvM_play_HIO_c::removeComboBox() {}

void dEvM_play_HIO_c::addComboBox() {
    JORMContext* mctx = attachJORMContext(5);
    mctx->invalidNode(this, 3);
    releaseJORMContext(mctx);
}

dEvM_bit_HIO_c::dEvM_bit_HIO_c() {
    field_0x008 = 0;
}

void dEvM_bit_HIO_c::genMessage(JORMContext* mctx) {
    /* "----Event Save Bit---" */
    mctx->genLabel("----イベント セーブビット---", LBL_EVENT_SAVE_BIT, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // "Update"
    mctx->genButton("更新", BTN_UPDATE, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    int i = 0;
    u32 labelPosY = 0;
    int unused = labelPosY;
    for (; i < mFlagTables->mTotalBitNum; i++) {
        if (mRootBitIdx == mFlagTables->mBitTable[i].mRootBit) {
            if (mFlagTables == &dEvDb_flag_base_table)
                mFlagTables->mBitTable[i].mIsSet = dComIfGs_isEventBit(mFlagTables->mBitTable[i].mFlagValue);
            else
                mFlagTables->mBitTable[i].mIsSet = dComIfGs_isTmpBit(mFlagTables->mBitTable[i].mFlagValue);

            mctx->genCheckBox(mFlagTables->mBitTable[i].mFlagName, &mFlagTables->mBitTable[i].mIsSet, 1, 0, NULL, 0xffff, 0xffff, 0xfa, 0x18);
            mctx->genLabel(mFlagTables->mBitTable[i].mFlagDescription, i + (1 << 31) + (1 << 1), 0, NULL, 0x100, labelPosY * 25 + 50, 0x200, 0x18);

            labelPosY++;
        }
    }
}

void dEvM_bit_HIO_c::listenPropertyEvent(const JORPropertyEvent* param_0) {
    int i;
    JORReflexible::listenPropertyEvent(param_0);

    switch (reinterpret_cast<u32>(param_0->id)) {
    case BTN_UPDATE:
        update();
    case BTN_UPDATE + 1:
        break;
    default:
        for (i = 0; i < mFlagTables->mTotalBitNum; i++) {
            if (mRootBitIdx == mFlagTables->mBitTable[i].mRootBit && param_0->id == reinterpret_cast<char*>(&mFlagTables->mBitTable[i].mIsSet)) {
                if (mFlagTables == &dEvDb_flag_base_table) {
                    if (mFlagTables->mBitTable[i].mIsSet)
                        dComIfGs_onEventBit(mFlagTables->mBitTable[i].mFlagValue);
                    else
                        dComIfGs_offEventBit(mFlagTables->mBitTable[i].mFlagValue);
                } else if (mFlagTables->mBitTable[i].mIsSet) {
                    dComIfGs_onTmpBit(mFlagTables->mBitTable[i].mFlagValue);
                } else {
                    dComIfGs_offTmpBit(mFlagTables->mBitTable[i].mFlagValue);
                }
            }
        }
        break;
    }
}

void dEvM_bit_HIO_c::update() {
    JORMContext* mctx = attachJORMContext(8);

    if (mFlagTables->mTotalBitNum > 0)
        mctx->startUpdateNode(this);

    int i = 0;
    for (; i < mFlagTables->mTotalBitNum; i++) {
        if (mRootBitIdx == mFlagTables->mBitTable[i].mRootBit) {
            if (mFlagTables == &dEvDb_flag_base_table)
                mFlagTables->mBitTable[i].mIsSet = dComIfGs_isEventBit(mFlagTables->mBitTable[i].mFlagValue);
            else
                mFlagTables->mBitTable[i].mIsSet = dComIfGs_isTmpBit(mFlagTables->mBitTable[i].mFlagValue);

            mctx->updateCheckBox(2, &mFlagTables->mBitTable[i].mIsSet, 1, 0);
        }
    }

    releaseJORMContext(mctx);
}

dEvM_reg_HIO_c::dEvM_reg_HIO_c() {
    field_0x008 = 0;
}

void dEvM_reg_HIO_c::genMessage(JORMContext* mctx) {
    /* "----Event Save Register---" */
    mctx->genLabel("----イベント セーブレジスター---", LBL_EVENT_SAVE_REGISTER, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    
    // "Update"
    mctx->genButton("更新", BTN_UPDATE, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    int i = 0;
    u8 flagVal;
    for (; i < mFlagTables->mTotalRegNum; i++) {
        if (mRootRegIdx == mFlagTables->mRegTable[i].mRootReg) {
            if (mFlagTables == &dEvDb_flag_base_table)
                mFlagTables->mRegTable[i].mIsSet = dComIfGs_getEventReg(mFlagTables->mRegTable[i].mFlagValue);
            else
                mFlagTables->mRegTable[i].mIsSet = dComIfGs_getTmpReg(mFlagTables->mRegTable[i].mFlagValue);

            mctx->genLabel(mFlagTables->mRegTable[i].mFlagDescription, 0x80000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
            flagVal = mFlagTables->mRegTable[i].mFlagValue;
            mctx->genSlider(mFlagTables->mRegTable[i].mFlagName, &mFlagTables->mRegTable[i].mIsSet, 0, flagVal, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
        }
    }
}

void dEvM_reg_HIO_c::listenPropertyEvent(const JORPropertyEvent* param_0) {
    int i;
    JORReflexible::listenPropertyEvent(param_0);

    switch(reinterpret_cast<u32>(param_0->id)) {
    case BTN_UPDATE:
        update();
    case BTN_UPDATE + 1:
        break;
    default:
        for (i = 0; i < mFlagTables->mTotalRegNum; i++) {
            if (mRootRegIdx == mFlagTables->mRegTable[i].mRootReg && param_0->id == reinterpret_cast<char*>(&mFlagTables->mRegTable[i].mIsSet)) {
                if (mFlagTables == &dEvDb_flag_base_table)
                    dComIfGs_setEventReg(mFlagTables->mRegTable[i].mFlagValue, mFlagTables->mRegTable[i].mIsSet);
                else
                    dComIfGs_setTmpReg(mFlagTables->mRegTable[i].mFlagValue, mFlagTables->mRegTable[i].mIsSet);
            }
        }
        break;
    }
}

void dEvM_reg_HIO_c::update() {
    JORMContext* mctx = attachJORMContext(8);

    if (mFlagTables->mTotalRegNum > 0)
        mctx->startUpdateNode(this);
    
    int i = 0;
    for (; i < mFlagTables->mTotalRegNum; i++) {
        if (mRootRegIdx == mFlagTables->mRegTable[i].mRootReg) {
            if (mFlagTables == &dEvDb_flag_base_table)
                mFlagTables->mRegTable[i].mIsSet = dComIfGs_getEventReg(mFlagTables->mRegTable[i].mFlagValue);
            else
                mFlagTables->mRegTable[i].mIsSet = dComIfGs_getTmpReg(mFlagTables->mRegTable[i].mFlagValue);

            mctx->updateSlider(2, &mFlagTables->mRegTable[i].mIsSet, 0, 0, 0);
        }
    }

    releaseJORMContext(mctx);
}

char* dEvDb_flag_base_c::searchDirNameBit(int bit) {
    int i = 0;
    for (; i < mTotalBitNum; i++) {
        if (bit == mBitTable[i].mRootBit)
            return mBitTable[i].mArea;
    }
    
    return NULL;
}

char* dEvDb_flag_base_c::searchDirNameReg(int bit) {
    int i = 0;
    for (; i < mTotalRegNum; i++) {
        if (bit == mRegTable[i].mRootReg)
            return mRegTable[i].mArea;
    }

    return NULL;
}
