/**
 * d_event_data.cpp
 * Event Data Processor
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_event_data.h"
#include "d/actor/d_a_player.h"
#include "d/d_camera.h"
#include "d/d_demo.h"
#include "d/d_gameover.h"
#include "d/d_msg_object.h"
#include "m_Do/m_Do_graphic.h"
#include "SSystem/SComponent/c_counter.h"

inline BOOL dEvDtFlagCheck(int i_flag) {
    return dComIfGp_getEventManager().getFlags().flagCheck(i_flag);
}

inline void dEvDtFlagSet(int i_flag) {
    dComIfGp_getEventManager().getFlags().flagSet(i_flag);
}

struct TelopDataStruct {
    s32 name;
    u16 telopNo;
    const u16 flag;
    u16 type;
};

static TelopDataStruct TelopData[] = {
    {'D05\0', 1101, 0x1069, 3},
    {'D05A', 1101, 0xFFFF, 0},
    {'D05B', 1101, 0xFFFF, 0},
    {'D04\0', 1102, 0xFFFF, 1},
    {'D04A', 1102, 0xFFFF, 0},
    {'D04B', 1102, 0xFFFF, 0},
    {'D01\0', 1103, 0x1278, 3},
    {'D01A', 1103, 0xFFFF, 0},
    {'D01B', 1103, 0xFFFF, 0},
    {'D10\0', 1104, 0x132C, 3},
    {'D10A', 1104, 0xFFFF, 0},
    {'D10B', 1104, 0xFFFF, 0},
    {'D11\0', 1105, 0x1478, 3},
    {'D11A', 1105, 0xFFFF, 0},
    {'D11B', 1105, 0xFFFF, 0},
    {'D06\0', 1106, 0x1559, 3},
    {'D06A', 1106, 0xFFFF, 0},
    {'D06B', 1106, 0xFFFF, 0},
    {'D07\0', 1107, 0x1639, 3},
    {'D07A', 1107, 0xFFFF, 0},
    {'D07B', 1107, 0xFFFF, 0},
    {'D08\0', 1108, 0xFFFF, 1},
    {'D08A', 1108, 0xFFFF, 0},
    {'D08B', 1108, 0xFFFF, 0},
    {'D08C', 1108, 0xFFFF, 0},
    {'D08D', 1108, 0xFFFF, 0},
    {'D09\0', 1109, 0xFFFF, 1},
    {'D09A', 1109, 0xFFFF, 0},
    {'D09B', 1109, 0xFFFF, 0},
    {'D09C', 1109, 0xFFFF, 0},
    {'F00\0', 1110, 0xFFFF, 1},
    {'F103', 1111, 10, 3},
    {'R01\0', 1111, 0xFFFF, 0},
    {'F109', 1112, 0xFFFF, 1},
    {'R109', 1112, 0xFFFF, 0},
    {'R209', 1112, 0xFFFF, 0},
    {'F112', 1113, 0x409, 3},
    {'F113', 1114, 0x400, 3},
    {'F108', 1115, 0xFFFF, 1},
    {'R108', 1115, 0xFFFF, 0},
    {'F115', 1116, 0x41E, 3},
    {'F111', 1118, 0xFFFF, 1},
    {'F124', 1119, 0xFFFF, 1},
    {'F114', 1120, 0x800, 3},
    {'F125', 1121, 0xA0C, 3},
    {'F121', 1122, 0x62D, 3},
    {'F122', 1122, 0x62D, 3},
    {'F123', 1122, 0x62D, 3},
    {'F117', 1123, 0xFFFF, 1},
    {'F128', 1134, 0x32E, 3},
    {'R128', 1134, 0xFFFF, 0},
    {'F127', 1140, 0xFFFF, 1},
    {'R127', 1140, 0xFFFF, 0},
    {'F126', 1141, 0xFFFF, 1},
};

static int numTelopData = ARRAY_SIZE(TelopData);

static u16 getTelopNo(char const* i_name) {
    int i;
    s32 name = ((i_name[3] == 'B' ? 'd' : i_name[0]) << 0x18 | i_name[4] << 0x10 | i_name[5] << 8 | i_name[6]);
    for (i = 0; i < numTelopData; i++) {
        if (name == TelopData[i].name) {
            return TelopData[i].telopNo;
        }
    }

    return 0xFFFF;
}

static u16 getTelopNo() {
    return getTelopNo(dComIfGp_getStartStageName());
}

static u16 getStartTelopNo() {
    int i;
    const char* startStageName = dComIfGp_getStartStageName();
    const char* lastPlayStateName = dComIfGp_getLastPlayStageName();

    if (strcmp(startStageName, lastPlayStateName) == 0) {
        return 0xFFFF;
    }

    
    TelopDataStruct* telopData = NULL;
    s32 name = ((startStageName[3] == 'B' ? 'd' : startStageName[0]) << 0x18 |
               startStageName[4] << 0x10 |
               startStageName[5] << 8 |
               startStageName[6]);

    for (i = 0; i < numTelopData; i++) {
        if (name == TelopData[i].name) {
            telopData = &TelopData[i];
            break;
        }
    }

    if (telopData == NULL || telopData->type == 0) {
        return 0xFFFF;
    }
    
    if (telopData->type == 2 && telopData->flag != 0xFFFF && !dComIfGs_isEventBit(telopData->flag)) {
        return 0xFFFF;
    }
    
    if (telopData->type == 3 && telopData->flag != 0xFFFF && !dComIfGs_isStageSwitch(telopData->flag >> 8, telopData->flag & 0xFF)) {
        return 0xFFFF;
    }
  
    TelopDataStruct* lastTelopData = NULL;
    name = ((lastPlayStateName[3] == 'B' ? 'd' : lastPlayStateName[0]) << 0x18 |
            lastPlayStateName[4] << 0x10 |
            lastPlayStateName[5] << 8 |
            lastPlayStateName[6]);

    for (i = 0; i < numTelopData; i++) {
        if (name == TelopData[i].name) {
            lastTelopData = &TelopData[i];
            break;
        }
    }

    if (lastTelopData != NULL && telopData->telopNo == lastTelopData->telopNo) {
        return 0xFFFF;
    }

    return telopData->telopNo;
}

#if DEBUG
u8 event_debug_evdt() {
    dEvM_HIO_c& hio = dComIfGp_getEventManager().getEventHIO();
    return hio.m_evd_debug;
}

u8 event_debug_evdt_endcheck() {
    dEvM_HIO_c& hio = dComIfGp_getEventManager().getEventHIO();
    return hio.m_evd_unfinished;
}

u8 event_debug_evdt_reg1() {
    dEvM_HIO_c& hio = dComIfGp_getEventManager().getEventHIO();
    return hio.m_prevent_scene_switch;
}

s32 event_debug_evdt_sound_adjust() {
    dEvM_HIO_c& hio = dComIfGp_getEventManager().getEventHIO();
    return hio.m_corrective_sound_adjustment;
}
#endif

static int dEvDt_Next_Stage(int i_staffId, int i_wipe) {
    char* stage;
    s16 start;
    int mode;
    s8 room;
    s8 layer;

    mode = 0;

    s8 wipe = i_wipe;
    BOOL noVisit = true;
    int wipe_time = 0;
    f32 hour = 0.0f;
    bool do_set_nexttime = false;
    f32 speed = 0.0f;

    char* sdata;
    f32* fdata;
    int* idata;
    
    idata = dComIfGp_evmng_getMyIntegerP(i_staffId, "ID");
    if (idata != NULL) {
        int id = *idata;
        s8 room_no = dComIfGp_roomControl_getStayNo();

        stage_scls_info_dummy_class* info;
        if (room_no == -1) {
            info = dComIfGp_getStageSclsInfo();
        } else {
            JUT_ASSERT(297, 0 <= room_no && room_no < 64);
            dStage_roomDt_c* roomdt = dComIfGp_roomControl_getStatusRoomDt(room_no);
            info = roomdt->getSclsInfo();
        }

        if (info != NULL && id >= 0 && id < info->num) {
            stage_scls_info_class* scls_p = &info->m_entries[id];
            stage = scls_p->mStage;
            start = scls_p->mStart;
            room = (s8)scls_p->mRoom;
            layer = dStage_sclsInfo_getSceneLayer(scls_p);
            wipe = dStage_sclsInfo_getWipe(scls_p);
            wipe_time = dStage_sclsInfo_getWipeTime(scls_p);

            if (wipe == 15) {
                wipe = 0;
            }

            int timeH = dStage_sclsInfo_getTimeH(scls_p);
            if (timeH >= 0 && timeH <= 23) {
                do_set_nexttime = true;
                hour = timeH;
            }
        } else {
            // "Couldn't get Scene info(%d)!!"
            OS_REPORT("\x1b[43;30mシーン情報がとれない(%d)!!\n\x1b[m", id);
        }
    }

    sdata = dComIfGp_evmng_getMyStringP(i_staffId, "Stage");
    if (sdata != NULL) {
        stage = sdata;
    }

    idata = dComIfGp_evmng_getMyIntegerP(i_staffId, "StartCode");
    if (idata != NULL) {
        start = *idata;
    }

    idata = dComIfGp_evmng_getMyIntegerP(i_staffId, "RoomNo");
    if (idata != NULL) {
        room = (s8)*idata;
    }

    idata = dComIfGp_evmng_getMyIntegerP(i_staffId, "Layer");
    if (idata != NULL) {
        layer = *idata;
    }

    idata = dComIfGp_evmng_getMyIntegerP(i_staffId, "Wipe");
    if (idata != NULL) {
        wipe = *idata;
    }

    idata = dComIfGp_evmng_getMyIntegerP(i_staffId, "Mode");
    if (idata != NULL) {
        mode = *idata;
    }

    fdata = dComIfGp_evmng_getMyFloatP(i_staffId, "Speed");
    if (fdata != NULL) {
        speed = *fdata;
    }

    fdata = dComIfGp_evmng_getMyFloatP(i_staffId, "Hour");
    if (fdata != NULL) {
        hour = *fdata;
        do_set_nexttime = true;
    }

    idata = dComIfGp_evmng_getMyIntegerP(i_staffId, "NoVisit");
    if (idata != NULL) {
        noVisit = false;
    }

    #if DEBUG
    if (event_debug_evdt_reg1()) {
        // "Cancelled for debug!!"
        OS_REPORT("デバックのためキャンセル!!\n");
        return 1;
    }
    #endif

    if (stage != NULL && start != -1) {
        if (do_set_nexttime) {
            dKy_set_nexttime(15.0f * hour);
        }

        dComIfGp_setNextStage(stage, start, room, layer, speed, mode, 1, wipe, 0, noVisit, wipe_time);

        #if DEBUG
        if (event_debug_evdt()) {
            OS_REPORT("!!\n!!\n");
            OS_REPORT("\x1b[46;30m%06d: event: scene change: \n        stage %s  start %d  room %d  layer %d  wipe %d\n\x1b[m",
                      g_Counter.mCounter0, stage, start, room, layer, wipe);
            OS_REPORT("!!\n!!\n");
        }
        #endif
    } else {
        // "Scene transition data is not complete!!"
        OS_REPORT_ERROR("シーン切り替えのデータ指定が足りない!!\n");
        JUT_ASSERT(379, FALSE);
    }

    return 1;
}

BOOL dEvDtFlag_c::flagCheck(int flag) {
    if (flagMaxCheck(flag)) {
        return FALSE;
    }

    if (mFlags[(u32)(flag & ~0x1F) / 32] & (1 << (flag & 0x1F))) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL dEvDtFlag_c::flagSet(int flag) {
    if (flagMaxCheck(flag)) {
        return FALSE;
    }

    mFlags[(u32)(flag & ~0x1F) / 32] |= (1 << (flag & 0x1F));
    return TRUE;
}

BOOL dEvDtFlag_c::flagMaxCheck(int i_flag) {
    int flag_id = i_flag;
    if (flag_id == -1) {
        return TRUE;
    }

    if (flag_id >= FlagMax) {
        JUT_ASSERT(445, flag_id < FlagMax);
        return TRUE;
    }

    return FALSE;
}

void dEvDtFlag_c::init() {
    int i;
    for (i = 0; i < ARRAY_SIZEU(mFlags); i++) {
        mFlags[i] = 0;
    }
}

int dEvDtEvent_c::finishCheck() {
    int i;
    for (i = 0; i < 3; i++) {
        if (mFlags[i] == -1) {
            return 1;
        }

        if (!dEvDtFlagCheck(mFlags[i])) {
            return 0;
        }
    }

    return 1;
}

int dEvDtEvent_c::forceFinish() {
    int i;
    for (i = 0; i < 3; i++) {
        if (mFlags[i] == -1) {
            return 1;
        }

        dEvDtFlagSet(mFlags[i]);
    }

    return 1;
}

void dEvDtEvent_c::specialStaffProc(dEvDtStaff_c* i_staffList) {
    int i;
    dEvDtStaff_c* staff;

    for (i = 0; i < mNStaff; i++) {
        staff = &i_staffList[getStaff(i)];
        staff->specialProc();
    }
}

void dEvDtStaff_c::specialProc_WaitStart(int i_staffId) {
    int* idata = dComIfGp_evmng_getMyIntegerP(i_staffId, "Timer");
    if (idata == NULL) {
        mWaitTimer = 0;
    } else {
        mWaitTimer = *idata;
    }
}

void dEvDtStaff_c::specialProc_WaitProc(int i_staffId) {
    if (mWaitTimer > 0) {
        mWaitTimer--;
    } else {
        dComIfGp_evmng_cutEnd(i_staffId);
    }
}

void dEvDtStaff_c::specialProc() {
    switch (mType) {
    case TYPE_PACKAGE:
        specialProcPackage();
        break;
    case TYPE_DIRECTOR:
        specialProcDirector();
        break;
    case TYPE_TIMEKEEPER:
        specialProcTimekeeper();
        break;
    case TYPE_ALL:
        dComIfGp_evmng_cutEnd(mIndex);
        break;
    case TYPE_EFFECT:
        specialProcEffect();
        break;
    case TYPE_CREATE:
        specialProcCreate();
        dComIfGp_evmng_cutEnd(mIndex);
        break;
    case TYPE_SOUND:
        specialProcSound();
        break;
    case TYPE_MESSAGE:
        specialProcMessage();
        break;
    case TYPE_LIGHT:
        specialProcLight();
        break;
    }
}

void dEvDtStaff_c::init() {
    mCurrentCut = mStartCut;
    field_0x40 = true;

    #if DEBUG
    if (event_debug_evdt()) {
        dEvDtCut_c* cut = dComIfGp_getEventManager().getBase().getCutP(mCurrentCut);
        OS_REPORT("\x1b[32m%06d: event: cut: START %s::%s[%d]\n\x1b[m", g_Counter.mCounter0, getName(), cut->getName(), cut->getTagId());
    }
    #endif

    field_0x41 = false;
    field_0x3c = -1;

    switch (mType) {
    case TYPE_CAMERA:
        dComIfGp_getEventManager().setCameraPlay(1);
        break;
    }
}

void dEvDtStaff_c::advanceCut(int i_cut) {
    mCurrentCut = i_cut;
    field_0x40 = true;
    field_0x41 = false;
    field_0x3c = -1;
}

void dEvDtStaff_c::specialProcLight() {
    u8* data = mData;

    int staffId = dComIfGp_evmng_getMyStaffId("LIGHT", NULL, 0);
    if (staffId == -1) {
        JUT_ASSERT(663, FALSE);
        return;
    }
    
    f32* fdata;
    char* nowCutName;
    int* idata;

    char* var_r27 = dComIfGp_getEventManager().getMyNowCutName(staffId);
    nowCutName = var_r27;

    f32 time;
    if (dComIfGp_evmng_getIsAddvance(staffId)) {
        switch (*(int*)nowCutName) {
        case 'CHAN':
            fdata = dComIfGp_evmng_getMyFloatP(staffId, "Hour");
            if (fdata != NULL) {
                dKy_instant_timechg(*fdata * 15.0f);
            }

            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Weather");
            if (idata != NULL) {
                switch (*idata) {
                case 0:
                    dKy_instant_rainchg();
                }
            }
            break;
        case 'ADD_':
            fdata = dComIfGp_evmng_getMyFloatP(staffId, "Hour");
            if (fdata != NULL) {
                time = dComIfGs_getTime() * (1.0f / 15.0f);
                time += *fdata;
                while (time >= 24.0) {
                    time -= 24.0;
                }
                dKy_instant_timechg(time * 15.0f);
            }
        }
    }

    dComIfGp_evmng_cutEnd(staffId);
}

void dEvDtStaff_c::specialProcMessage() {
    MessageData* data = (MessageData*)&mData;
    StaffWork* wk = (StaffWork*)&mWork;

    int staffId = dComIfGp_evmng_getMyStaffId("MESSAGE", NULL, 0);
    if (staffId == -1) {
        JUT_ASSERT(741, FALSE);
        return;
    }

    int* idata;
    char* sdata;

    char* spC = dComIfGp_getEventManager().getMyNowCutName(staffId);
    char* nowCutName = spC;

    if (dComIfGp_evmng_getIsAddvance(staffId)) {
        switch (*(int*)nowCutName) {
        case 'WAIT':
            specialProc_WaitStart(staffId);
            break;
        case 'CREA':
        case 'SHOW':
            wk->l_msgId = fpcM_ERROR_PROCESS_ID_e;
            wk->l_msg = NULL;

            wk->msgs.n = dComIfGp_evmng_getMySubstanceNum(staffId, "msgNo");
            wk->msgs.d = dComIfGp_evmng_getMyIntegerP(staffId, "msgNo");
            if (wk->msgs.n < 1) {
                wk->msgs.n = dComIfGp_evmng_getMySubstanceNum(staffId, "No");
                wk->msgs.d = dComIfGp_evmng_getMyIntegerP(staffId, "No");
            }

            JUT_ASSERT(764, wk->msgs.d);

            wk->l_msgNo = *wk->msgs.d;
            data->unk = 0;
            break;
        case 'END\0':
        case 'FINI':
            JUT_ASSERT(770, wk->l_msg);
            wk->l_msg->mode = fopMsg_MODE_MSG_END_e;
            break;
        case 'CONT':
            JUT_ASSERT(774, wk->l_msg);
            wk->l_msg->mode = fopMsg_MODE_MSG_CONTINUE_e;

            idata = dComIfGp_evmng_getMyIntegerP(staffId, "msgNo");
            if (idata == NULL) {
                idata = dComIfGp_evmng_getMyIntegerP(staffId, "No");
            }

            JUT_ASSERT(778, idata);
            wk->l_msgNo = *idata;
            fopMsgM_messageSet(wk->l_msgNo, 1000);
            break;
        case 'SAVE':
            wk->l_msgNo = 1;

            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Type");
            if (idata != NULL && *idata == 1) {
                wk->l_msgNo = 2;
            }

            wk->l_msgId = d_GameOver_Create(wk->l_msgNo);
            break;
        case 'TELO':
            idata = dComIfGp_evmng_getMyIntegerP(staffId, "ForStart");

            if (idata != NULL && *idata != 0) {
                wk->l_msgNo = getStartTelopNo();
            } else {
                sdata = dComIfGp_evmng_getMyStringP(staffId, "Stage");
                idata = dComIfGp_evmng_getMyIntegerP(staffId, "No");
                if (idata != NULL) {
                    wk->l_msgNo = *idata;
                } else if (sdata != NULL) {
                    wk->l_msgNo = getTelopNo(sdata);
                } else {
                    wk->l_msgNo = getTelopNo();
                }
            }

            data->unk = 0;
            if (wk->l_msgNo != 0xFFFF) {
                wk->l_msgId = fopMsgM_messageSetDemo(wk->l_msgNo);
            }
        }
    }

    switch (*(int*)nowCutName) {
    case 'WAIT':
        specialProc_WaitProc(staffId);
        break;
    case 'CREA':
        switch (data->unk) {
        case 0:
            wk->l_msgId = fopMsgM_messageSet(wk->l_msgNo, 1000);
            if (wk->l_msgId != fpcM_ERROR_PROCESS_ID_e) {
                data->unk++;
            }
            break;
        case 1:
            wk->l_msg = fopMsgM_SearchByID(wk->l_msgId);
            if (wk->l_msg != NULL) {
                data->unk++;
            }
            break;
        case 2:
            dComIfGp_evmng_cutEnd(staffId);
            break;
        }
        break;
    case 'PUSH':
        JUT_ASSERT(846, wk->l_msg);
        if (wk->l_msg->mode == fopMsg_MODE_MSG_DISPLAYED_e) {
            dComIfGp_evmng_cutEnd(staffId);
        }
        break;
    case 'TELO':
        dComIfGp_evmng_cutEnd(staffId);
        break;
    case 'SHOW':
        switch (data->unk) {
        case 0:
            wk->l_msgId = fopMsgM_messageSet(wk->l_msgNo, 1000);
            if (wk->l_msgId != fpcM_ERROR_PROCESS_ID_e) {
                data->unk = 1;
            } else {
                break;
            }
        case 1:
            wk->l_msg = fopMsgM_SearchByID(wk->l_msgId);
            if (wk->l_msg == NULL) {
                break;
            }

            switch (wk->l_msg->mode) {
            case fopMsg_MODE_MSG_DISPLAYED_e:
                wk->l_msg->mode = fopMsg_MODE_MSG_END_e;
                break;
            case fopMsg_MODE_BOX_CLOSING_e:
                break;
            case fopMsg_MODE_BOX_CLOSED_e:
                wk->l_msg->mode = fopMsg_MODE_MSG_DESTROYED_e;
                wk->l_msgId = fpcM_ERROR_PROCESS_ID_e;
                wk->l_msg = NULL;

                wk->msgs.n--;
                if (wk->msgs.n != 0) {
                    data->unk = 0;
                    wk->msgs.d++;
                    wk->l_msgNo = *wk->msgs.d;
                } else {
                    dComIfGp_evmng_cutEnd(staffId);
                    data->unk = 99;
                }
                break;
            default:
                break;
            }
            break;
        default:
            dComIfGp_evmng_cutEnd(staffId);
            break;
        }
        break;
    case 'DELE':
    case 'FINI':
        if (wk->l_msg == NULL) {
            dComIfGp_evmng_cutEnd(staffId);
        } else if (wk->l_msg->mode == fopMsg_MODE_BOX_CLOSED_e) {
            wk->l_msg->mode = fopMsg_MODE_MSG_DESTROYED_e;
            wk->l_msgId = fpcM_ERROR_PROCESS_ID_e;
            wk->l_msg = NULL;
            dComIfGp_evmng_cutEnd(staffId);
        }
        break;
    case 'CONT':
        dComIfGp_evmng_cutEnd(staffId);
        break;
    case 'END\0':
        switch (wk->l_msg->mode) {
        case fopMsg_MODE_BOX_CLOSING_e:
        case fopMsg_MODE_BOX_CLOSED_e:
            dComIfGp_evmng_cutEnd(staffId);
            break;
        }
        break;
    case 'SAVE':
        if (d_GameOver_CheckDelete(wk->l_msgId) && dComIfGp_getGameoverStatus() == 1) {
            dComIfGp_evmng_cutEnd(staffId);
            d_GameOver_Delete(wk->l_msgId);
        }
        break;
    default:
        dComIfGp_evmng_cutEnd(staffId);
        break;
    }
}

void dEvDtStaff_c::specialProcSound() {
    SoundData* data = (SoundData*)&mData;

    int staffId = dComIfGp_evmng_getMyStaffId("SOUND", NULL, 0);
    if (staffId == -1) {
        JUT_ASSERT(960, FALSE);
        return;
    }

    int* idata;

    char* var_r26 = dComIfGp_getEventManager().getMyNowCutName(staffId);
    char* nowCutName = var_r26;

    if (dComIfGp_evmng_getIsAddvance(staffId)) {
        switch (*(int*)nowCutName) {
        case 'WAIT':
            specialProc_WaitStart(staffId);
            break;
        case 'STRM':
            mDoAud_bgmStreamPlay();

            #if DEBUG
            if (event_debug_evdt()) {
                // "d_event_data:: bgmStreamPlay called!!"
                OS_REPORT("d_event_data:: bgmStreamPlay を呼んだ!!\n");
            }
            #endif
            break;
        case 'NOMS':
            specialProc_WaitStart(staffId);

            #if DEBUG
            if (event_debug_evdt()) {
                // "SOUND wait %d"
                OS_REPORT("ＳＯＵＮＤ 待ち %d  \n", data->timer);
            }
            #endif
            break;
        case 'RIDD':
            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Type");
            if (idata != NULL && *idata == 1) {
                mDoAud_seStart(Z2SE_READ_RIDDLE_A, NULL, 0, 0);
            } else {
                mDoAud_seStart(Z2SE_READ_RIDDLE_B, NULL, 0, 0);
            }
            break;
        case 'BGMS':
            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Timer");
            if (idata != NULL) {
                mDoAud_bgmStop(*idata);
            }
        }
    }

    switch (*(int*)nowCutName) {
    case 'WAIT':
        specialProc_WaitProc(staffId);
        break;
    case 'NOMS':
        if (data->timer <= dDemo_c::getFrameNoMsg()) {
            dComIfGp_evmng_cutEnd(staffId);
        }
        break;
    default:
        dComIfGp_evmng_cutEnd(staffId);
    }
}

void dEvDtStaff_c::specialProcCreate() {
    u8* data = mData;

    char* name;
    int* idata;
    cXyz* xyzdata;

    u32 arg;
    cXyz pos;
    csXyz angle;
    cXyz scale;
    dStage_objectNameInf* objectName;

    int staffId = dComIfGp_evmng_getMyStaffId("CREATER", NULL, 0);
    if (staffId == -1) {
        JUT_ASSERT(1055, FALSE);
        return;
    }

    char* sp14 = dComIfGp_getEventManager().getMyNowCutName(staffId);
    char* nowCutName = sp14;

    if (dComIfGp_evmng_getIsAddvance(staffId)) {
        switch (*(int*)nowCutName) {
        case 'WAIT':
            break;
        case 'CREA':
            name = dComIfGp_evmng_getMyStringP(staffId, "MAKECAST");
            JUT_ASSERT(1071, name);

            objectName = dStage_searchName(name);
            JUT_ASSERT(1073, objectName);

            idata = dComIfGp_evmng_getMyIntegerP(staffId, "ARG");
            if (idata == NULL) {
                arg = 0xFFFFFFFF;
            } else {
                arg = *idata;
            }

            xyzdata = dComIfGp_evmng_getMyXyzP(staffId, "POS");
            if (xyzdata == NULL) {
                fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
                pos = player_p->current.pos;
            } else {
                pos = *xyzdata;
            }

            idata = dComIfGp_evmng_getMyIntegerP(staffId, "ANGLE");
            if (idata == NULL) {
                angle.setall(0);
            } else {
                angle.set(idata[0], idata[1], idata[2]);
            }

            xyzdata = dComIfGp_evmng_getMyXyzP(staffId, "SCALE");
            if (xyzdata == NULL) {
                scale.setall(1.0f);
            } else {
                scale = *xyzdata;
            }

            fopAcM_create(objectName->procname, arg, &pos, dComIfGp_roomControl_getStayNo(),
                          &angle, &scale, objectName->argument);
            break;
        }
    }
}

void dEvDtStaff_c::specialProcDirector() {
    DirectorData* data = (DirectorData*)mData;
    int* idata;
    int* idata2;
    f32* rate;

    daPy_py_c* player = dComIfGp_getLinkPlayer();
    dEvt_control_c& evtControl = dComIfGp_getEvent();
    GXColor color;

    int staffId = dComIfGp_evmng_getMyStaffId("DIRECTOR", NULL, 0);
    if (staffId == -1) {
        JUT_ASSERT(1146, FALSE);
        return;
    }

    char* sp2C = dComIfGp_getEventManager().getMyNowCutName(staffId);
    char* nowCutName = sp2C;

    if (dComIfGp_evmng_getIsAddvance(staffId)) {
        int* idata_flag = dComIfGp_evmng_getMyIntegerP(staffId, "EventFlag");
        if (idata_flag != NULL) {
            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("%06d: event: derector: event flag %d\n", g_Counter.mCounter0, *idata_flag);
            }
            #endif

            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[*idata_flag]);
        }

        idata_flag = dComIfGp_evmng_getMyIntegerP(staffId, "EventFlagOff");
        if (idata_flag != NULL) {
            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("%06d: event: derector: event flag off %d\n", g_Counter.mCounter0, *idata_flag);
            }
            #endif

            dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[*idata_flag]);
        }

        idata_flag = dComIfGp_evmng_getMyIntegerP(staffId, "SwitchTable");
        int switchTable;
        if (idata_flag != NULL) {
            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("%06d: event: derector: switchbit table %d\n", g_Counter.mCounter0, *idata_flag);
            }
            #endif

            switchTable = *idata_flag;
        } else {
            switchTable = dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo());
        }

        idata_flag = dComIfGp_evmng_getMyIntegerP(staffId, "SwitchBit");
        if (idata_flag != NULL) {
            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("%06d: event: derector: switchbit on %d\n", g_Counter.mCounter0, *idata_flag);
            }
            #endif

            dComIfGs_onStageSwitch(switchTable, *idata_flag);
        }

        idata_flag = dComIfGp_evmng_getMyIntegerP(staffId, "SwitchOff");
        if (idata_flag != NULL) {
            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("%06d: event: derector: switchbit off %d\n", g_Counter.mCounter0, *idata_flag);
            }
            #endif

            dComIfGs_offStageSwitch(switchTable, *idata_flag);
        }

        idata_flag = dComIfGp_evmng_getMyIntegerP(staffId, "TmpBit");
        if (idata_flag != NULL) {
            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("%06d: event: derector: temporary bit on %d\n", g_Counter.mCounter0, *idata_flag);
            }
            #endif

            dComIfGs_onTmpBit(dSv_event_tmp_flag_c::tempBitLabels[*idata_flag]);
        }

        idata_flag = dComIfGp_evmng_getMyIntegerP(staffId, "TmpBitOff");
        if (idata_flag != NULL) {
            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("%06d: event: derector: temporary bit off %d\n", g_Counter.mCounter0, *idata_flag);
            }
            #endif

            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[*idata_flag]);
        }

        switch (*(int*)nowCutName) {
        case 'WAIT':
            specialProc_WaitStart(staffId);
            break;
        case 'PLAY':
            if (strcmp(sp2C, "PLAYER_NODRAW") == 0) {
                player->onPlayerNoDraw();
            } else if (strcmp(sp2C, "PLAYER_DRAW") == 0) {
                player->offPlayerNoDraw();
            }
            break;
        case 'NEXT':
            dEvDt_Next_Stage(staffId, 13);
            break;
        case 'SKIP': {
            char* zev = dComIfGp_evmng_getMyStringP(staffId, "Zev");
            void* pt = (void*)evtControl.getPt1();
            if (NULL == pt) {
                pt = evtControl.getPt2();
            }

            if (zev != NULL) {
                evtControl.setSkipZev(pt, zev);
            } else {
                evtControl.setSkipProc(pt, dEv_defaultSkipProc, 0);
            }
            break;
        }
        case 'FADE':
            rate = dComIfGp_evmng_getMyFloatP(staffId, "Rate");
            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Color");

            JUT_ASSERT(1261, rate);

            if (*rate > 0) {
                mDoGph_gInf_c::setFadeRate(0);
            }

            if (idata != NULL) {
                color.r = idata[0];
                color.g = idata[1];
                color.b = idata[2];
                color.a = idata[3];
                mDoGph_gInf_c::fadeOut(*rate, color);

                #if DEBUG
                if (event_debug_evdt()) {
                    OS_REPORT("%06d: event: derector: fade %f (%d %d %d %d)!!\n", g_Counter.mCounter0, *rate, color.r, color.g, color.b, color.a);
                }
                #endif
                break;
            } else {
                mDoGph_gInf_c::fadeOut(*rate);

                #if DEBUG
                if (event_debug_evdt()) {
                    OS_REPORT("%06d: event: derector: fade %f!!\n", g_Counter.mCounter0, *rate);
                }
                #endif
            }
            break;
        case 'VIBR':
            specialProc_WaitStart(staffId);
            if (data->unk2 == 0) {
                JUT_ASSERT(1291, FALSE);
            }

            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Pattern");
            idata2 = dComIfGp_evmng_getMyIntegerP(staffId, "Type");

            if (idata == NULL || idata2 == NULL) {
                JUT_ASSERT(1295, FALSE);
            }

            dComIfGp_getVibration().StartQuake((u8*)idata, 0, *idata2, cXyz(0.0f, 1.0f, 0.0f));
            break;
        case 'WIPE':
            rate = dComIfGp_evmng_getMyFloatP(staffId, "Rate");
            JUT_ASSERT(1302, rate);

            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("%06d: event: derector: wipe %f!!\n", g_Counter.mCounter0, *rate);
            }
            #endif

            dDlst_list_c::wipeOut(*rate);
            if (*rate > 0) {
                data->unk = 0;
            } else {
                data->unk = 1;
            }

            idata2 = dComIfGp_evmng_getMyIntegerP(staffId, "SOUND");
            if (idata2 != NULL) {
                switch (*idata2) {
                case 0:
                    mDoAud_seStart(Z2SE_SY_DUMMY, NULL, 0, 0);
                }
            }
            break;
        case 'MAPT': {
            int* idP = dComIfGp_evmng_getMyIntegerP(staffId, "ID");
            data->unk = *idP;

            dStage_MapEvent_dt_c* mapEvent = dEvt_control_c::searchMapEventData(data->unk);
            if (mapEvent != NULL) {
                if (mapEvent->type == dStage_MapEvent_dt_TYPE_MAPTOOLCAMERA) {
                    data->unk2 = mapEvent->data.maptool.field_0x14;
                } else {
                    data->unk2 = -1;
                }

                #if DEBUG
                data->unk2 += (s16)event_debug_evdt_sound_adjust();
                #endif
            } else {
                data->unk2 = 0;
            }
            break;
        }
        case 'CAST': {
            char* sdata = dComIfGp_evmng_getMyStringP(staffId, "Pt2");
            fopAc_ac_c* evt_actor_p;
            if (sdata != NULL) {
                evt_actor_p = fopAcM_searchFromName4Event(sdata, -1);
                if (evt_actor_p != NULL) {
                    evtControl.setPt2(evt_actor_p);
                    OS_REPORT("%06d: event: derector: set %s as Pt2!\n", g_Counter.mCounter0, sdata);
                } else {
                    OS_REPORT("%06d: event: derector: casting: %s not found!\n", g_Counter.mCounter0, sdata);
                }
            }

            sdata = dComIfGp_evmng_getMyStringP(staffId, "PtT");
            if (sdata != NULL) {
                evt_actor_p = fopAcM_searchFromName4Event(sdata, -1);
                if (evt_actor_p != NULL) {
                    evtControl.setPtT(evt_actor_p);
                    OS_REPORT("%06d: event: derector: set %s as PtT!\n", g_Counter.mCounter0, sdata);
                } else {
                    OS_REPORT("%06d: event: derector: casting: %s not found!\n", g_Counter.mCounter0, sdata);
                }
            }

            sdata = dComIfGp_evmng_getMyStringP(staffId, "PtI");
            if (sdata != NULL) {
                evt_actor_p = fopAcM_searchFromName4Event(sdata, -1);
                if (evt_actor_p != NULL) {
                    evtControl.setPtI(evt_actor_p);
                    OS_REPORT("%06d: event: derector: set %s as PtI!\n", g_Counter.mCounter0, sdata);
                } else {
                    OS_REPORT("%06d: event: derector: casting: %s not found!\n", g_Counter.mCounter0, sdata);
                }
            }

            sdata = dComIfGp_evmng_getMyStringP(staffId, "PtD");
            if (sdata != NULL) {
                evt_actor_p = fopAcM_searchFromName4Event(sdata, -1);
                if (evt_actor_p != NULL) {
                    evtControl.setPtD(evt_actor_p);
                    OS_REPORT("%06d: event: derector: set %s as PtD!\n", g_Counter.mCounter0, sdata);
                } else {
                    OS_REPORT("%06d: event: derector: casting: %s not found!\n", g_Counter.mCounter0, sdata);
                }
            }
            break;
        }
        case 'SPEC': {
            int* type = dComIfGp_evmng_getMyIntegerP(staffId, "Type");
            if (type != NULL) {
                switch (*type) {
                case 8:
                        /* dSv_event_flag_c::F_0506 - N/A - Only met Ooccoo Sr. - not yet son (shared Forest - Snowpeak) */
                    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[506])) {
                        /* dSv_event_flag_c::F_0566 - Ooccoo Sr. stuff - Letter from Ooccoo Sr. came in OK (no son) */
                        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[566]);
                        OS_REPORT("%06d: event: derector: obachan letter special on 566\n", g_Counter.mCounter0);
                    }

                        /* dSv_event_flag_c::F_0507 - N/A - Also met the son in dungeon with first Ooccoo Sr. meeting - 
                                                            turns 506 OFF (shared Forest - Snowpeak) */
                    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[507])) {
                        /* dSv_event_flag_c::F_0567 - Ooccoo Sr. stuff - Letter from Ooccoo Sr. came in OK (yes son) */
                        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[567]);
                        OS_REPORT("%06d: event: derector: obachan letter special on 567\n", g_Counter.mCounter0);
                    }
                }
            }
            break;
        }
        }
    }

    switch (*(int*)nowCutName) {
    case 'WAIT':
        specialProc_WaitProc(staffId);
        break;
    case 'FADE':
        #if DEBUG
        if (event_debug_evdt_reg1()) {
            dComIfGp_evmng_cutEnd(staffId);
            break;
        }
        #endif

        if (!mDoGph_gInf_c::isFade()) {
            dComIfGp_evmng_cutEnd(staffId);
            break;
        }

        if (mDoGph_gInf_c::getFadeRate() >= 1) {
            rate = dComIfGp_evmng_getMyFloatP(staffId, "Rate");
            JUT_ASSERT(1454, rate);

            if (*rate > 0) {
                dComIfGp_evmng_cutEnd(staffId);
            }
        }
        break;
    case 'NEXT':
        break;
    case 'VIBR':
        if (data->unk2 > 0) {
            data->unk2--;
            if (data->unk2 == 0) {
                dComIfGp_getVibration().StopQuake(0x1F);
            }
        } else {
            dComIfGp_evmng_cutEnd(staffId);
        }
        break;
    case 'WIPE':
        if (data->unk != 0) {
            if (dDlst_list_c::getWipeRate() == 0) {
                dComIfGp_evmng_cutEnd(staffId);
            }
        } else {
            if (dDlst_list_c::getWipeRate() >= 1) {
                dComIfGp_evmng_cutEnd(staffId);
            }
        }
        break;
    case 'MAPT':
        data->unk2--;
        if (data->unk2 <= 0) {
            dComIfGp_evmng_cutEnd(staffId);
        }
        break;
    default:
        dComIfGp_evmng_cutEnd(staffId);
        break;
    }
}

void dEvDtStaff_c::specialProcPackage() {
    u8* data = mData;
    char* sdata;
    f32* fdata;
    cXyz* xyzdata;
    int* idata;

    if (dMsgObject_getMsgObjectClass() != NULL) {
        dMsgObject_demoMessageGroup();
    }

    u8 sp8 = 0;
    u8* demo_data = NULL;

    int staffId = dComIfGp_evmng_getMyStaffId("PACKAGE", NULL, 0);
    if (staffId == -1) {
        JUT_ASSERT(1528, FALSE);
        return;
    }

    char* sp14 = dComIfGp_getEventManager().getMyNowCutName(staffId);
    char* nowCutName = sp14;

    if (dComIfGp_evmng_getIsAddvance(staffId)) {
        sp8 = 1;

        switch (*(int*)nowCutName) {
        case 'WAIT':
            specialProc_WaitStart(staffId);
            break;
        case 'PLAY':
            sdata = dComIfGp_evmng_getMyStringP(staffId, "FileName");
            
            #if DEBUG
            if (event_debug_evdt()) {
                printf("%06d: event: package: %s !!!\n", g_Counter.mCounter0, sdata);
            }
            #endif

            xyzdata = dComIfGp_evmng_getMyXyzP(staffId, "OffsetPos");
            fdata = dComIfGp_evmng_getMyFloatP(staffId, "OffsetAngY");

            f32 offsetAngY;
            if (fdata != NULL) {
                offsetAngY = *fdata;
            } else {
                offsetAngY = 0.0f;
            }

            // fake match: should not save event from g_dComIfG_gameInfo to register
            dComIfG_play_c& info = g_dComIfG_gameInfo.play;
            demo_data = (u8*)info.getEvent().getStbDemoData(sdata);
            JUT_ASSERT(1571, demo_data);

            dDemo_c::start(demo_data, xyzdata, offsetAngY);
            info.getEvent().setCullRate(10.0f); // fake match: suppoed to be dComIfGp_event_setCullRate

            idata = dComIfGp_evmng_getMyIntegerP(staffId, "EventFlag");
            if (idata != NULL) {
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[*idata]);
            }
        }
    }

    switch (*(int*)nowCutName) {
    case 'WAIT':
        specialProc_WaitProc(staffId);
        break;
    case 'PLAY': {
        dEvt_control_c& evtControl = dComIfGp_getEvent();
        if (dDemo_c::getMode() == 2) {
            dStage_MapEvent_dt_c* event = dComIfGp_getEvent().getStageEventDt();
            if (event != NULL && event->field_0x7 != 0xFF && !evtControl.chkFlag2(1)) {
                dDemo_c::getControl()->suspend(100);
                dComIfGp_evmng_cutEnd(staffId);
            } else {
                dDemo_c::end();
            }
        }

        if (dDemo_c::getMode() == 0) {
            dComIfGp_evmng_cutEnd(staffId);
        }
        break;
    }
    default:
        dComIfGp_evmng_cutEnd(staffId);
        break;
    }
}

void dEvDtStaff_c::specialProcTimekeeper() {
    TimerKeeperData* data = (TimerKeeperData*)&mData;
    int* idata;

    int staffId = dComIfGp_evmng_getMyStaffId("TIMEKEEPER", NULL, 0);
    if (staffId == -1) {
        JUT_ASSERT(1650, FALSE);
        return;
    }

    char* var_r27 = dComIfGp_getEventManager().getMyNowCutName(staffId);
    char* nowCutName = var_r27;

    if (dComIfGp_evmng_getIsAddvance(staffId)) {
        switch (*(int*)nowCutName) {
        case 'COUN':
            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Timer");

            #if PLATFORM_SHIELD
            if (idata == NULL) {
                idata = dComIfGp_evmng_getMyIntegerP(staffId, "S");
            }
            #endif

            JUT_ASSERT(1668, idata);
            data->timer = *idata;

            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("%06d: event: timekeeper: COUNTDOWN::START %d\n", g_Counter.mCounter0, *idata);
            }
            #endif
            break;
        case 'WAIT':
            break;
        }
    }

    switch (*(int*)nowCutName) {
    case 'COUN':
        if (data->timer > 0) {
            data->timer--;
        } else {
            dComIfGp_evmng_cutEnd(staffId);
        }
        break;
    case 'WAIT':
    default:
        dComIfGp_evmng_cutEnd(staffId);
    }
}

void dEvDtStaff_c::specialProcEffect() {
    EffectData* data = (EffectData*)mData;
    int* idata;
    f32* fdata;

    int staffId = dComIfGp_evmng_getMyStaffId("EFFECT", NULL, 0);
    if (staffId == -1) {
        JUT_ASSERT(1720, FALSE);
        return;
    }

    dEvent_manager_c& evtMgr = dComIfGp_getEventManager();
    char* var_r27 = evtMgr.getMyNowCutName(staffId);
    char* nowCutName = var_r27;

    if (evtMgr.getIsAddvance(staffId)) {
        data->unk = 0;

        switch (*(int*)nowCutName) {
        case 'WAIT': {
            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Timer");
            data->unk = idata != NULL ? *idata : 0;
            break;
        }
        case 'SHOC': {
            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Power");
            int power = idata != NULL ? *idata : 5;

            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Type");
            int type = idata != NULL ? *idata : 15;

            dComIfGp_getVibration().StartShock(power, type, cXyz(0, 1, 0));
        
            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("%06d: event: effect: start shock %d %x\n", g_Counter.mCounter0, power, type & 0x1F);
            }
            #endif
            break;
        }
        case 'QUAK': {
            int power;
            int type;
            int random;
            u8* pattern;

            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Timer");
            data->unk = idata != NULL ? *idata : 30;

            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Power");
            power = idata != NULL ? *idata : 5;

            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Type");
            type = idata != NULL ? *idata : 15;

            pattern = (u8*)dComIfGp_evmng_getMyStringP(staffId, "Pattern");
            if (pattern != NULL) {
                idata = dComIfGp_evmng_getMyIntegerP(staffId, "Random");
                random = idata != NULL ? *idata : 0;

                dComIfGp_getVibration().StartQuake(pattern, random, type, cXyz(0, 1, 0));

                #if DEBUG
                if (event_debug_evdt()) {
                    OS_REPORT("%06d: event: effect: start quake %s %d %x\n", g_Counter.mCounter0, pattern, power, type & 0x1F);
                }
                #endif
            } else {
                dComIfGp_getVibration().StartQuake(power, type, cXyz(0, 1, 0));

                #if DEBUG
                if (event_debug_evdt()) {
                    OS_REPORT("%06d: event: effect: start quake %d\n", g_Counter.mCounter0, power);
                }
                #endif
            }
            break;
        }
        case 'BLUR': {
            idata = dComIfGp_evmng_getMyIntegerP(staffId, "Timer");
            int timer = data->unk = idata != NULL ? *idata : 30;

            fdata = dComIfGp_evmng_getMyFloatP(staffId, "Alpha");
            f32 alpha = fdata != NULL ? *fdata : 0.8f;

            fdata = dComIfGp_evmng_getMyFloatP(staffId, "Scale");
            f32 scale = fdata != NULL ? *fdata : 1.0f;

            dCam_getBody()->StartBlure(timer, NULL, alpha, scale);

            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("%06d: event: effect: blure %d %f %f\n", g_Counter.mCounter0, timer, alpha, scale);
            }
            #endif
            break;
        }
        }
    }

    if (data->unk != 0) {
        data->unk--;
    }

    switch (*(int*)nowCutName) {
    case 'WAIT':
        if (data->unk == 0) {
            dComIfGp_evmng_cutEnd(staffId);
        }
        break;
    case 'QUAK':
        if (data->unk == 0) {
            dComIfGp_getVibration().StopQuake(0x1F);

            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("%06d: event: effect: stop quake\n", g_Counter.mCounter0);
            }
            #endif

            dComIfGp_evmng_cutEnd(staffId);
        }
        break;
    case 'BLUR':
        if (data->unk == 0) {
            dComIfGp_evmng_cutEnd(staffId);
        }
        break;
    default:
        dComIfGp_evmng_cutEnd(staffId);
        break;
    }
}

int dEvDtCut_c::startCheck() {
    int i;
    int flag;

    for (i = 0; i < 3; i++) {
        flag = getStartFlag(i);
        if (flag == -1) {
            if (i == 0) {
                return -1;
            } else {
                return 1;
            }
        }

        if (!dEvDtFlagCheck(flag)) {
            return 0;
        }
    }

    return 1;
}

int dEvDtBase_c::init() {
    mHeaderP = NULL;
    mEventP = NULL;
    mStaffP = NULL;
    mCutP = NULL;
    mDataP = NULL;
    mFDataP = NULL;
    mIDataP = NULL;
    mSDataP = NULL;
    mRoomNo = -1;
    return 0;
}

int dEvDtBase_c::init(char* i_data, int i_roomNo) {
    init();
    if (i_data == NULL) {
        return 0;
    }

    setHeaderP((event_binary_data_header*)i_data);

    if (getEventNum() > 0) {
        setEventP((dEvDtEvent_c*)(i_data + getEventTop()));
    }

    if (getStaffNum() > 0) {
        setStaffP((dEvDtStaff_c*)(i_data + getStaffTop()));
    }

    if (getCutNum() > 0) {
        setCutP((dEvDtCut_c*)(i_data + getCutTop()));
    }

    if (getDataNum() > 0) {
        setDataP((dEvDtData_c*)(i_data + getDataTop()));
    }

    if (getFDataNum() > 0) {
        setFDataP((f32*)(i_data + getFDataTop()));
    }

    if (getIDataNum() > 0) {
        setIDataP((int*)(i_data + getIDataTop()));
    }

    if (getSDataNum() > 0) {
        setSDataP((char*)(i_data + getSDataTop()));
    }

    mRoomNo = i_roomNo;
    return getEventNum();
}

void dEvDtBase_c::advanceCut(dEvDtEvent_c* i_event) {
    int i;
    dEvDtStaff_c* staff;

    for (i = 0; i < i_event->getNStaff(); i++) {
        staff = mStaffP + i_event->getStaff(i);
        advanceCutLocal(staff);
    }
}

BOOL dEvDtBase_c::advanceCutLocal(dEvDtStaff_c* i_staff) {
    dEvDtCut_c* cut = &mCutP[i_staff->getCurrentCut()];

    #if DEBUG
    if (event_debug_evdt_endcheck()) {
        if (!dEvDtFlagCheck(cut->getFlagId())) {
            OS_REPORT("\x1b[32m%06d: event: cut: %s::%s was NOT stoped\n\x1b[m", g_Counter.mCounter0, i_staff->getName(), cut->getName());
        } else {
            OS_REPORT("\x1b[32m%06d: event: cut: %s::%s was stoped\n\x1b[m", g_Counter.mCounter0, i_staff->getName(), cut->getName());
        }
    }
    #endif

    dEvDtCut_c* cut_next;
    if (dEvDtFlagCheck(cut->getFlagId()) && cut->getNext() != -1) {
        cut_next = &mCutP[cut->getNext()];
        switch (cut_next->startCheck()) {
        case -1:
            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("\x1b[32m%06d: event: cut: STOP %s::%s[%d]\n\x1b[m", g_Counter.mCounter0, i_staff->getName(), cut->getName(), cut->getTagId());
            }
            #endif

            i_staff->advanceCut(cut->getNext());

            #if DEBUG
            if (event_debug_evdt()) {
                cut = &mCutP[i_staff->getCurrentCut()];
                OS_REPORT("\x1b[32m%06d: event: cut: START %s::%s[%d]\n\x1b[m", g_Counter.mCounter0, i_staff->getName(), cut->getName(), cut->getTagId());
            }
            #endif
            return TRUE;
        case 1:
            dEvDtFlagSet(cut->getFlagId());

            #if DEBUG
            if (event_debug_evdt()) {
                OS_REPORT("\x1b[32m%06d: event: cut: STOP %s::%s[%d]\n\x1b[m", g_Counter.mCounter0, i_staff->getName(), cut->getName(), cut->getTagId(), g_Counter.mCounter0);
            }
            #endif

            i_staff->advanceCut(cut->getNext());

            #if DEBUG
            if (event_debug_evdt()) {
                cut = &mCutP[i_staff->getCurrentCut()];
                OS_REPORT("\x1b[32m%06d: event: cut: START %s::%s[%d]\n\x1b[m", g_Counter.mCounter0, i_staff->getName(), cut->getName(), cut->getTagId());
            }
            #endif
            return TRUE;
        }
    }

    i_staff->field_0x40 = false;
    return FALSE;
}
