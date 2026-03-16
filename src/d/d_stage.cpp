#include "d/dolzel.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRAramArchive.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "SSystem/SComponent/c_malloc.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_suspend.h"
#include "d/d_com_inf_game.h"
#include "d/d_lib.h"
#include "d/d_map_path_dmap.h"
#include "d/d_map_path_fmap.h"
#include "d/d_path.h"
#include "d/d_save_HIO.h"
#include "d/d_stage.h"
#include "d/d_bg_parts.h"
#include "f_ap/f_ap_game.h"
#include "f_op/f_op_kankyo_mng.h"
#include "f_op/f_op_msg_mng.h"
#include "f_op/f_op_scene_mng.h"
#include "global.h"
#include "m_Do/m_Do_Reset.h"
#include <cstdio>
#include <cstring>

void dStage_nextStage_c::set(const char* i_stage, s8 i_roomId, s16 i_point, s8 i_layer, s8 i_wipe,
                             u8 i_speed) {
    if (!enabled) {
        enabled = true;
        wipe = i_wipe;
        wipe_speed = i_speed;
#if DEBUG
        if (g_kankyoHIO.navy.fade_test_speed) {
            wipe_speed = g_kankyoHIO.navy.fade_test_speed;
        }
#endif
        dStage_startStage_c::set(i_stage, i_roomId, i_point, i_layer);
        if (!strcmp(i_stage, "OPENING")) {
            mDoRst::onReset();
        }
    }
}

void dStage_SetErrorRoom() {
    // "Room information might be corrupted. \nPlease try to re-convert. \n"
    OSReport_Error("部屋情報が、多分ですが壊れています。\n変換し直してみてください。\n");
    OS_REPORT("または、ステージの情報が部屋にあります。↓を参考に。\n");
}

void dStage_SetErrorStage() {
    // "Stage information might be corrupted. \nPlease try to re-convert. "
    OSReport_Error("ステージ情報が、多分ですが壊れています。\n変換し直してみてください。");
    OS_REPORT("または、部屋の情報がステージにあります。↓を参考に。\n");
}

static dStage_KeepDoorInfo DoorInfo;

dStage_KeepDoorInfo* dStage_GetKeepDoorInfo() {
    return &DoorInfo;
}

static bool dStage_isBossStage(dStage_dt_c* i_stage) {
    stage_stag_info_class* pstag = i_stage->getStagInfo();
    JUT_ASSERT(127, pstag != 0);

    if (pstag == NULL) {
        return false;
    }

    u32 stType = dStage_stagInfo_GetSTType(pstag);
    if (stType == ST_BOSS_ROOM) {
        return true;
    } else {
        return false;
    }
}

static void dStage_KeepDoorInfoInit(dStage_dt_c* i_stage) {
    if (dStage_isBossStage(i_stage) == 0) {
        DoorInfo.mNum = 0;
    }
}

static void dStage_KeepDoorInfoProc(dStage_dt_c* i_stage, stage_tgsc_class* i_drtg) {
    UNUSED(i_stage);
    if (i_drtg == NULL) {
        DoorInfo.mNum = 0;
        return;
    }
    if (i_drtg->num >= ARRAY_SIZE(DoorInfo.mDrTgData) || i_drtg->num < 0) {
        DoorInfo.mNum = 0;
        return;
    }
    DoorInfo.mNum = i_drtg->num;
    if (DoorInfo.mNum == 0) {
        return;
    }
    stage_tgsc_data_class* pSrcEntry = i_drtg->m_entries;
    stage_tgsc_data_class* pDstEntry = DoorInfo.mDrTgData;
    for (int i = 0; i < DoorInfo.mNum; i++) {
        *pDstEntry = *pSrcEntry;
        pDstEntry++;
        pSrcEntry++;
    }
}

static dStage_KeepDoorInfo l_RoomKeepDoorInfo;

dStage_KeepDoorInfo* dStage_GetRoomKeepDoorInfo() {
    return &l_RoomKeepDoorInfo;
}

static void dStage_initRoomKeepDoorInfo() {
    l_RoomKeepDoorInfo.mNum = 0;
}

static void dStage_RoomKeepDoorInfoProc(dStage_dt_c* i_stage, stage_tgsc_class* param_2) {
    if (param_2 == NULL) {
        return;
    }
    if (param_2->num + l_RoomKeepDoorInfo.mNum >= 0x40 || param_2->num < 0) {
        return;
    }
    if (param_2->num == 0) {
        return;
    }
    stage_tgsc_data_class* psVar4 = param_2->m_entries;
    stage_tgsc_data_class* pTgData = &l_RoomKeepDoorInfo.mDrTgData[l_RoomKeepDoorInfo.mNum];
    int iVar3 = 0;
    for (int i = 0; i < param_2->num; i++) {
        *pTgData = *psVar4;
        pTgData->base.angle.x &= ~0x3f;
        pTgData->base.angle.x |= i_stage->getRoomNo() & 0x3fU;
        pTgData++;
        psVar4++;
        iVar3++;
    }
    l_RoomKeepDoorInfo.mNum += iVar3;
}

static int dStage_RoomKeepDoorInit(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                   void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    stage_tgsc_class* tgsc = (stage_tgsc_class*)((char*)i_data + 4);
    dStage_RoomKeepDoorInfoProc(i_stage, tgsc);
    return 1;
}

void dStage_startStage_c::set(const char* i_Name, s8 i_RoomNo, s16 i_Point, s8 i_Layer) {
#if TARGET_PC
    // UB fix.
    if (mName != i_Name) {
        strncpy_s(mName, sizeof(mName), i_Name, sizeof(mName));
    }
#else
    strcpy(mName, i_Name);
#endif
    mRoomNo = i_RoomNo;
    mPoint = i_Point;
    mLayer = i_Layer;
}

dStage_roomStatus_c dStage_roomControl_c::mStatus[0x40];

void dStage_roomControl_c::init() {
    mStayNo = -1;
    mOldStayNo = -1;
    mNextStayNo = -1;
    mNoChangeRoom = 0;
    mRoomReadId = -1;

#if PLATFORM_GCN
    if (dComIfGp_getStartStagePoint() >= 0 || dComIfGp_getStartStagePoint() == -4) {
        initZone();
    }
#else
    if (dComIfGp_getStartStagePoint() >= 0) {
        dComIfGs_initZone();
    }
#endif

    dStage_roomStatus_c* status = mStatus;
    for (int i = 0; i < 0x40; i++) {
        status->mRoomDt.init();
        status->mRoomDt.initFileList2();
        setStatusFlag(i, 0);
        status->mDraw = false;

        if (!PLATFORM_GCN && dComIfGp_getStartStagePoint() >= 0) {
            status->mZoneNo = -1;
        } else if (status->mZoneNo >= 0) {
            dComIfGs_clearRoomSwitch(status->mZoneNo);
            dComIfGs_clearRoomItem(status->mZoneNo);
        }

        status->mMemBlockID = -1;
        status->mpBgW = NULL;
        status++;
    }

    mArcBankName = (dStage_roomControl_c::dStage_bankName*)dComIfG_getStageRes("name.bin");
    mArcBankData = (dStage_roomControl_c::dStage_bankData*)dComIfG_getStageRes("bank.bin");

    if (mArcBankName == NULL) {
        JUT_ASSERT(449, mArcBankData == NULL);
        mArcBankData = NULL;
    } else {
        JUT_ASSERT(452, mArcBankData != NULL);
    }
#if DEBUG
    if (mNoArcBank) {
        mArcBankName = NULL;
        mArcBankData = NULL;
    }
#endif
}

void dStage_roomControl_c::initZone() {
    dComIfGs_initZone();

    dStage_roomStatus_c* status = mStatus;
    for (int i = 0; i < 0x40; i++) {
        status->mZoneNo = -1;
        status++;
    }
}

dStage_roomDt_c* dStage_roomControl_c::getStatusRoomDt(int i_statusIdx) {
    if (i_statusIdx < 0 || i_statusIdx >= 0x40) {
        OS_REPORT_ERROR("getStatusRoomDt():部屋番号範囲外:%d(0～%d)\n", i_statusIdx, 0x40);
        return NULL;
    }
    return &mStatus[i_statusIdx].mRoomDt;
}

JKRExpHeap* dStage_roomControl_c::getMemoryBlock(int i_roomNo) {
    int blockId = getMemoryBlockID(i_roomNo);

    if (blockId < 0) {
        return NULL;
    }
    return mMemoryBlock[blockId];
}

void dStage_roomControl_c::setStayNo(int i_roomNo) {
    mOldStayNo = mStayNo;
    mStayNo = i_roomNo;
    mNextStayNo = mStayNo;

    if (mStayNo < 0) {
        return;
    }

    onStatusDraw(mStayNo);
}

void dStage_roomControl_c::setNextStayNo(int nextStayNo) {
    mNextStayNo = nextStayNo;
}

static int stayRoomCheck(int param_0, u8* param_1, int param_2) {
    for (int i = 0; i < param_0; i++) {
        if (param_2 == dStage_roomRead_dt_c_GetLoadRoomIndex(param_1[i])) {
            return 1;
        }
    }
    return 0;
}

static int createRoomScene(int param_0) {
    int* ptr = (int*)JKRAlloc(4, -4);

    if (ptr == NULL) {
        return 0;
    }
    *ptr = param_0;

    if (!fopScnM_CreateReq(fpcNm_ROOM_SCENE_e, 0x7FFF, 0, (uintptr_t)ptr)) {
        JKRFree(ptr);
        return 0;
    }

    return 1;
}

BOOL dStage_roomControl_c::checkRoomDisp(int i_roomNo) const {
    if (checkStatusFlag(i_roomNo, 8)) {
        return 0;
    }

    if (checkStatusFlag(i_roomNo, 16)) {
        return true;
    } else {
        return false;
    }
}

int dStage_roomControl_c::loadRoom(int roomCount, u8* rooms, bool param_2) {
    if (mRoomReadId < 0 && mNoChangeRoom != 0) {
        return 0;
    }

    for (int roomNo = 0; roomNo < ARRAY_SIZE(mStatus); roomNo++) {
        if (checkStatusFlag(roomNo, 0x02 | 0x04)) {
            return 0;
        }
    }
    
    BOOL r26 = TRUE;
    for (int roomNo = 0; roomNo < ARRAY_SIZE(mStatus); roomNo++) {
        if (dStage_roomControl_c::checkStatusFlag(roomNo, 0x01)) {
            if (!stayRoomCheck(roomCount, rooms, roomNo)) {
                onStatusFlag(roomNo, 0xc);
                OS_REPORT("kill !!<%d>\n", roomNo);
                r26 = FALSE;
            }
        }
    }
    if (!r26) {
        return FALSE;
    }
    
    for (int i = 0; i < roomCount; i++) {
        int roomNo = dStage_roomRead_dt_c_GetLoadRoomIndex(rooms[i]);
        dStage_roomControl_c::setZoneCount(roomNo, 2);
        if (!checkStatusFlag(roomNo, 0x01)) {
            if (param_2) {
                if (dStage_roomRead_dt_c_ChkBg(rooms[i]) && createRoomScene(roomNo)) {
                    onStatusFlag(roomNo, 2);
                }
            } else {
                if (createRoomScene(roomNo)) {
                    int flag;
                    if (dStage_roomRead_dt_c_ChkBg(rooms[i])) {
                        flag = 0x02;
                    } else {
                        flag = 0x4A;
                    }
                    onStatusFlag(roomNo, flag);
                }
                return TRUE;
            }
        } else {
            if (dStage_roomRead_dt_c_ChkBg(rooms[i])) {
                if (checkStatusFlag(roomNo, 0x40)) {
                    offStatusFlag(roomNo, 0x48);
                }
            } else {
                if (!checkStatusFlag(roomNo, 0x60)) {
                    onStatusFlag(roomNo, 0x48);
                } else {
                    onStatusFlag(roomNo, 0x40);
                }
            }
        }
    }
    
    return TRUE;
}

void dStage_roomControl_c::zoneCountCheck(int i_roomNo) const {
    dStage_roomStatus_c* status = mStatus;
    for (int i = 0; i < 0x40; i++) {
        if (status->mZoneNo >= 0 && status->mZoneCount > 0) {
            dComIfGs_clearRoomSwitch(status->mZoneNo);
            dComIfGs_clearRoomItem(status->mZoneNo);

            if (dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) != ST_FIELD &&
                i_roomNo != mOldStayNo)
            {
                if (--status->mZoneCount == 0) {
                    dComIfGs_removeZone(status->mZoneNo);
                    status->mZoneNo = -1;
                }
            }
        }
        status++;
    }

    setStayNo(i_roomNo);
}

static void dummy1(dStage_roomControl_c* roomControl) {
    roomControl->getFileList2(0);
}

JKRExpHeap* dStage_roomControl_c::createMemoryBlock(int i_blockIdx, u32 i_heapSize) {
    if (mMemoryBlock[i_blockIdx] == NULL) {
        mMemoryBlock[i_blockIdx] = JKRCreateExpHeap(i_heapSize, mDoExt_getArchiveHeap(), false);
    }

    return mMemoryBlock[i_blockIdx];
}

void dStage_roomControl_c::destroyMemoryBlock() {
    for (int i = 0; i < MEMORY_BLOCK_MAX; i++) {
        if (mMemoryBlock[i] != NULL) {
            mDoExt_destroyExpHeap(mMemoryBlock[i]);
            mMemoryBlock[i] = NULL;
        }
    }
}

void dStage_roomControl_c::setArcBank(int i_bank, char const* bankName) {
    JUT_ASSERT(1053, 0 <= i_bank && i_bank < 32);
    strncpy(&mArcBank[i_bank][0], bankName, 9);
}

char* dStage_roomControl_c::getArcBank(int i_bank) {
    JUT_ASSERT(1067, 0 <= i_bank && i_bank < 32);
    return mArcBank[i_bank];
}

bool dStage_roomControl_c::resetArchiveBank(int i_bank) {
    for (int i = i_bank; i < 32; i++) {
        char* bank = getArcBank(i);

        if (strcmp(bank, "")) {
            s32 syncStatus = dComIfG_syncObjectRes(bank);
            if (syncStatus < 0) {
                OSReport_Error("Bank[%d] : %s.arc Sync Read Error !!\n", i, bank);
            } else {
                if (syncStatus > 0) {
                    return 0;
                }
                OS_REPORT(">>>>>>>>>>> delete Bank[%d] : %s\n", i, bank);
                dComIfG_deleteObjectResMain(bank);
                setArcBank(i, "");
            }
        }
    }
    return 1;
}

void dStage_roomControl_c::roomDzs_c::create(u8 i_num) {
    JUT_ASSERT(1112, !m_num && 0 < i_num && i_num < 64);
    m_dzs = JKR_NEW_ARRAY_ARGS(void*, i_num, mDoExt_getArchiveHeap(), -4);
    JUT_ASSERT(1114, m_dzs != NULL);

    if (m_dzs != NULL) {
        u32** dzs = (u32**)m_dzs;
        for (u8 i = 0; i < i_num; i++) {
            *dzs = NULL;
            dzs++;
        }
        m_num = i_num;
    }
}

void dStage_roomControl_c::roomDzs_c::remove() {
    if (m_num != 0) {
        u32** dzs = (u32**)m_dzs;
        for (u8 i = 0; i < m_num; i++) {
            if (*dzs != NULL) {
                mDoExt_getArchiveHeap()->free(*dzs);
            }
            dzs++;
        }
        JKR_DELETE_ARRAY(m_dzs);
        m_num = 0;
    }
}

void* dStage_roomControl_c::roomDzs_c::add(u8 i_no, u8 roomNo) {
    if (m_num == 0) {
        return NULL;
    }

    JUT_ASSERT(1160, i_no < m_num);
    void** dzs = m_dzs + i_no;
    if (*dzs == NULL) {
        char dzsName[20];
        sprintf(dzsName, "%s/room%d.dzs", dComIfGp_getStartStageName(), roomNo);
        JUT_ASSERT(1167, strlen(dzsName) <= sizeof(dzsName));

        u32 expandSize =
            dLib_getExpandSizeFromAramArchive(dComIfGp_getFieldMapArchive2(), (const char*)dzsName);
        JUT_ASSERT(1172, expandSize);
        OS_REPORT("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ <%s> <%d>\n", dzsName, expandSize);
        *dzs = mDoExt_getArchiveHeap()->alloc(expandSize, -0x20);
        JUT_ASSERT(1179, *dzs != NULL);

        if (*dzs != NULL) {
            u32 readSize = dComIfGp_getFieldMapArchive2()->readResource(*dzs, expandSize, dzsName);
            JUT_ASSERT(1186, readSize <= expandSize);
        }
    }

    return *dzs;
}

// clang-format off
#define OBJNAME(name, proc, sub)                                                                   \
    { name, proc, sub }

static dStage_objectNameInf l_objectName[] = {
    OBJNAME("Grass",   fpcNm_GRASS_e,             -1),
    OBJNAME("kusax1",  fpcNm_GRASS_e,             -1),
    OBJNAME("kusax7",  fpcNm_GRASS_e,             -1),
    OBJNAME("kusax21", fpcNm_GRASS_e,             -1),
    OBJNAME("flower",  fpcNm_GRASS_e,             -1),
    OBJNAME("flwr7",   fpcNm_GRASS_e,             -1),
    OBJNAME("flwr17",  fpcNm_GRASS_e,             -1),
    OBJNAME("pflower", fpcNm_GRASS_e,             -1),
    OBJNAME("pflwrx7", fpcNm_GRASS_e,             -1),
    OBJNAME("door",    fpcNm_DOOR20_e,            -1),
    OBJNAME("kdoor",   fpcNm_KNOB20_e,            -1),
    OBJNAME("ddoor",   fpcNm_DBDOOR_e,            -1),
    OBJNAME("bdoor",   fpcNm_BOSS_DOOR_e,         -1),
    OBJNAME("ndoor",   fpcNm_DOOR20_e,            -1),
    OBJNAME("tadoor",  fpcNm_DOOR20_e,            -1),
    OBJNAME("yodoor",  fpcNm_DOOR20_e,            -1),
    OBJNAME("nadoor",  fpcNm_DOOR20_e,            -1),
    OBJNAME("l9door",  fpcNm_DOOR20_e,            -1),
    OBJNAME("l7door",  fpcNm_DOOR20_e,            -1),
    OBJNAME("pdoor",   fpcNm_PushDoor_e,          -1),
    OBJNAME("bigdoor", fpcNm_DOOR20_e,            -1),
    OBJNAME("kshtr00", fpcNm_Obj_Kshutter_e,      -1),
    OBJNAME("vshuter", fpcNm_Obj_Kshutter_e,      -1),
    OBJNAME("IzmGate", fpcNm_Izumi_Gate_e,        -1),
    OBJNAME("NoneCam", fpcNm_CAMERA_e,            -1),
    OBJNAME("NormCam", fpcNm_CAMERA_e,            -1),
    OBJNAME("Link",    fpcNm_ALINK_e,             -1),
    OBJNAME("carry00", fpcNm_Obj_Carry_e,         -1),
    OBJNAME("carry01", fpcNm_Obj_Carry_e,         -1),
    OBJNAME("carry02", fpcNm_Obj_Carry_e,         -1),
    OBJNAME("carry04", fpcNm_Obj_Carry_e,         -1),
    OBJNAME("carry05", fpcNm_Obj_Carry_e,         -1),
    OBJNAME("carry06", fpcNm_Obj_Carry_e,         -1),
    OBJNAME("carry07", fpcNm_Obj_Carry_e,         -1),
    OBJNAME("carry08", fpcNm_Obj_Carry_e,         -1),
    OBJNAME("carry09", fpcNm_Obj_Carry_e,         -1),
    OBJNAME("ball1",   fpcNm_Obj_Carry_e,         -1),
    OBJNAME("ball2",   fpcNm_Obj_Carry_e,         -1),
    OBJNAME("ball3",   fpcNm_Obj_Carry_e,         -1),
    OBJNAME("carry12", fpcNm_Obj_Carry_e,         -1),
    OBJNAME("carry13", fpcNm_Obj_Carry_e,         -1),
    OBJNAME("TagLite", fpcNm_Tag_LightBall_e,     -1),
    OBJNAME("InoBone", fpcNm_Obj_InoBone_e,       -1),
    OBJNAME("osiBLK0", fpcNm_Obj_Movebox_e,       0x00),
    OBJNAME("dmblk",   fpcNm_Obj_Movebox_e,       0x00),
    OBJNAME("Kkiba",   fpcNm_Obj_Movebox_e,       0x02),
    OBJNAME("smblk",   fpcNm_Obj_Movebox_e,       0x02),
    OBJNAME("Lv4blk",  fpcNm_Obj_Movebox_e,       -1),
    OBJNAME("Lv4blk2", fpcNm_Obj_Movebox_e,       -1),
    OBJNAME("gstone",  fpcNm_Obj_Movebox_e,       -1),
    OBJNAME("TagAtt",  fpcNm_Tag_Attp_e,          -1),
    OBJNAME("mmvbg",   fpcNm_SET_BG_OBJ_e,        -1),
    OBJNAME("Drop",    fpcNm_Obj_Drop_e,          -1),
    OBJNAME("BkLeaf",  fpcNm_Obj_BkLeaf_e,        -1),
    OBJNAME("cylwind", fpcNm_Obj_Tornado_e,       -1),
    OBJNAME("TagTrnd", fpcNm_Obj_Tornado2_e,      -1),
    OBJNAME("mvstair", fpcNm_Obj_MvStair_e,       -1),
    OBJNAME("UHDoor",  fpcNm_Obj_Cowdoor_e,       -1),
    OBJNAME("SwBoom",  fpcNm_Obj_Swpropeller_e,   -1),
    OBJNAME("Swboom",  fpcNm_Obj_Swpropeller_e,   -1),
    OBJNAME("BoomSht", fpcNm_Obj_BoomShutter_e,   -1),
    OBJNAME("haihai",  fpcNm_Obj_Hfuta_e,         -1),
    OBJNAME("stopper", fpcNm_Obj_Stopper_e,       -1),
    OBJNAME("dstop",   fpcNm_Obj_Stopper2_e,      -1),
    OBJNAME("mhole",   fpcNm_Obj_MHole_e,         -1),
    OBJNAME("mhole2",  fpcNm_Obj_MHole_e,         -1),
    OBJNAME("TagMag",  fpcNm_Tag_Magne_e,         -1),
    OBJNAME("P_Warp",  fpcNm_Obj_BossWarp_e,      -1),
    OBJNAME("fpillar", fpcNm_Obj_FirePillar_e,    -1),
    OBJNAME("yfire",   fpcNm_Obj_FirePillar2_e,   -1),
    OBJNAME("pfire",   fpcNm_Obj_FirePillar2_e,   -1),
    OBJNAME("Huriko1", fpcNm_Obj_WoodPendulum_e,  -1),
    OBJNAME("VolBall", fpcNm_Obj_VolcanicBall_e,  -1),
    OBJNAME("VolBom",  fpcNm_Obj_VolcanicBall_e,  -1),
    OBJNAME("BigVolc", fpcNm_Obj_VolcanicBomb_e,  -1),
    OBJNAME("geyser",  fpcNm_Obj_Geyser_e,        -1),
    OBJNAME("Cwall",   fpcNm_Obj_ChainWall_e,     -1),
    OBJNAME("Tansu",   fpcNm_Obj_Chest_e,         -1),
    OBJNAME("Onsen",   fpcNm_Obj_Onsen_e,         -1),
    OBJNAME("Obj_bm",  fpcNm_Obj_Bemos_e,         -1),
    OBJNAME("lv6bm",   fpcNm_Obj_Lv6bemos_e,      -1),
    OBJNAME("E_bm6",   fpcNm_Obj_Lv6bemos2_e,     -1),
    OBJNAME("stone",   fpcNm_Obj_Stone_e,         -1),
    OBJNAME("stoneB",  fpcNm_Obj_Stone_e,         -1),
    OBJNAME("TagSpin", fpcNm_Tag_Spinner_e,       -1),
    OBJNAME("L_RopeS", fpcNm_Obj_RopeBridge_e,    -1),
    OBJNAME("L_RopeB", fpcNm_Obj_RopeBridge_e,    -1),
    OBJNAME("wcover",  fpcNm_Obj_WellCover_e,     -1),
    OBJNAME("rstair",  fpcNm_Obj_RotStair_e,      -1),
    OBJNAME("marm",    fpcNm_Obj_MagneArm_e,      -1),
    OBJNAME("turnSw",  fpcNm_Obj_SwTurn_e,        -1),
    OBJNAME("chainSw", fpcNm_Obj_SwChain_e,       -1),
    OBJNAME("wsword",  fpcNm_Obj_WoodenSword_e,   -1),
    OBJNAME("iceblk",  fpcNm_Obj_IceBlock_e,      -1),
    OBJNAME("KkrSmk",  fpcNm_Obj_Smoke_e,         -1),
    OBJNAME("dmele",   fpcNm_Obj_Elevator_e,      -1),
    OBJNAME("wfall",   fpcNm_Obj_WaterFall_e,     -1),
    OBJNAME("izora",   fpcNm_Obj_ZoraCloth_e,     -1),
    OBJNAME("smgdoor", fpcNm_Obj_SmgDoor_e,       -1),
    OBJNAME("smkdoor", fpcNm_Obj_SmgDoor_e,       -1),
    OBJNAME("Obj_ava", fpcNm_Obj_Avalanche_e,     -1),
    OBJNAME("thdoor",  fpcNm_Obj_TDoor_e,         -1),
    OBJNAME("mstrsrd", fpcNm_Obj_MasterSword_e,   -1),
    OBJNAME("wimage",  fpcNm_Obj_WoodStatue_e,    -1),
    OBJNAME("sword",   fpcNm_Obj_Sword_e,         -1),
    OBJNAME("spring",  fpcNm_Tag_Spring_e,        -1),
    OBJNAME("BkDoorL", fpcNm_Obj_BkDoor_e,        -1),
    OBJNAME("BkDoorR", fpcNm_Obj_BkDoor_e,        -1),
    OBJNAME("IGateL",  fpcNm_Obj_MGate_e,         -1),
    OBJNAME("IGateR",  fpcNm_Obj_MGate_e,         -1),
    OBJNAME("HGateL",  fpcNm_Obj_MGate_e,         -1),
    OBJNAME("HGateR",  fpcNm_Obj_MGate_e,         -1),
    OBJNAME("K_Gate",  fpcNm_Obj_KkrGate_e,       -1),
    OBJNAME("R_Gate",  fpcNm_Obj_RiderGate_e,     -1),
    OBJNAME("kkanban", fpcNm_Obj_KKanban_e,       -1),
    OBJNAME("Mhsg6",   fpcNm_Obj_Ladder_e,        0x00),
    OBJNAME("Mhsg9",   fpcNm_Obj_Ladder_e,        0x01),
    OBJNAME("Mhsg12",  fpcNm_Obj_Ladder_e,        0x02),
    OBJNAME("Mhsg15",  fpcNm_Obj_Ladder_e,        0x03),
    OBJNAME("Mhsg4h",  fpcNm_Obj_Ladder_e,        0x04),
    OBJNAME("Mhsg3",   fpcNm_Obj_Ladder_e,        0x05),
    OBJNAME("bonbori", fpcNm_EP_e,                -1),
    OBJNAME("Cow",     fpcNm_COW_e,               -1),
    OBJNAME("Peru",    fpcNm_PERU_e,              -1),
    OBJNAME("Kago",    fpcNm_KAGO_e,              -1),
    OBJNAME("TagFall", fpcNm_Tag_WaterFall_e,     -1),
    OBJNAME("Kbota_A", fpcNm_Obj_Swpush_e,        0x00),
    OBJNAME("Kbota_B", fpcNm_Obj_Swpush_e,        0x01),
    OBJNAME("KbotaC",  fpcNm_Obj_Swpush_e,        0x02),
    OBJNAME("Swpush",  fpcNm_Obj_Swpush_e,        0x00),
    OBJNAME("Lv3bota", fpcNm_Obj_Swpush_e,        0x00),
    OBJNAME("Ksw00_C", fpcNm_Obj_Swpush2_e,       0x02),
    OBJNAME("hvySw",   fpcNm_Obj_Swpush5_e,       0x00),
    OBJNAME("buraA",   fpcNm_Obj_SwHang_e,        0x00),
    OBJNAME("buraB",   fpcNm_Obj_SwHang_e,        0x00),
    OBJNAME("buraA2",  fpcNm_Obj_SwHang_e,        0x00),
    OBJNAME("buraB2",  fpcNm_Obj_SwHang_e,        0x00),
    OBJNAME("bura7A",  fpcNm_Obj_SwHang_e,        0x00),
    OBJNAME("bura7B",  fpcNm_Obj_SwHang_e,        0x00),
    OBJNAME("bura7C",  fpcNm_Obj_SwHang_e,        0x00),
    OBJNAME("bura7D",  fpcNm_Obj_SwHang_e,        0x00),
    OBJNAME("bura7E",  fpcNm_Obj_SwHang_e,        0x00),
    OBJNAME("tbox_sw", fpcNm_TBOX_SW_e,           0x00),
    OBJNAME("swLight", fpcNm_Obj_SwLight_e,       0x00),
    OBJNAME("iceleaf", fpcNm_Obj_IceLeaf_e,       0x00),
    OBJNAME("swHit",   fpcNm_SWHIT0_e,            0x00),
    OBJNAME("swHit2",  fpcNm_SWHIT0_e,            0x01),
    OBJNAME("ObjTime", fpcNm_Obj_Timer_e,         -1),
    OBJNAME("AND_SW",  fpcNm_ANDSW_e,             -1),
    OBJNAME("AND_SW2", fpcNm_ANDSW2_e,            -1),
    OBJNAME("ALLdie",  fpcNm_ALLDIE_e,            -1),
    OBJNAME("rvback",  fpcNm_Tag_RiverBack_e,     -1),
    OBJNAME("kagoFal", fpcNm_Tag_KagoFall_e,      -1),
    OBJNAME("CrvGate", fpcNm_Obj_CRVGATE_e,       -1),
    OBJNAME("CrvSaku", fpcNm_Obj_CRVFENCE_e,      -1),
    OBJNAME("CrvWood", fpcNm_Obj_CRVHAHEN_e,      -1),
    OBJNAME("CrvStel", fpcNm_Obj_CRVSTEEL_e,      -1),
    OBJNAME("CrvLH",   fpcNm_Obj_CRVLH_DW_e,      -1),
    OBJNAME("RvRock",  fpcNm_Obj_RIVERROCK_e,     -1),
    OBJNAME("P2a",     fpcNm_NPC_P2_e,            0x00),
    OBJNAME("P2b",     fpcNm_NPC_P2_e,            0x01),
    OBJNAME("P2c",     fpcNm_NPC_P2_e,            0x02),
    OBJNAME("Gnd",     fpcNm_NPC_GND_e,           -1),
    OBJNAME("grA",     fpcNm_NPC_GRA_e,           -1),
    OBJNAME("Obj_grA", fpcNm_OBJ_GRA_e,           -1),
    OBJNAME("Tag_grA", fpcNm_TAG_GRA_e,           -1),
    OBJNAME("TagYami", fpcNm_TAG_YAMI_e,          -1),
    OBJNAME("grC",     fpcNm_NPC_GRC_e,           -1),
    OBJNAME("grM",     fpcNm_NPC_GRM_e,           -1),
    OBJNAME("grMC",    fpcNm_NPC_GRMC_e,          -1),
    OBJNAME("grS",     fpcNm_NPC_GRS_e,           -1),
    OBJNAME("grD1",    fpcNm_NPC_GRD_e,           -1),
    OBJNAME("grD",     fpcNm_NPC_WRESTLER_e,      0x01),
    OBJNAME("grR",     fpcNm_NPC_GRR_e,           -1),
    OBJNAME("grO",     fpcNm_NPC_GRO_e,           -1),
    OBJNAME("grZ",     fpcNm_NPC_GRZ_e,           -1),
    OBJNAME("grARock", fpcNm_Obj_GraRock_e,       -1),
    OBJNAME("grZRock", fpcNm_Obj_GrzRock_e,       -1),
    OBJNAME("osnFire", fpcNm_OBJ_ONSEN_FIRE_e,    -1),
    OBJNAME("yamiD",   fpcNm_NPC_YAMID_e,         -1),
    OBJNAME("yamiT",   fpcNm_NPC_YAMIT_e,         -1),
    OBJNAME("yamiS",   fpcNm_NPC_YAMIS_e,         -1),
    OBJNAME("Blue_NS", fpcNm_NPC_BLUENS_e,        -1),
    OBJNAME("Kakashi", fpcNm_NPC_KAKASHI_e,       0x00),
    OBJNAME("Kdk",     fpcNm_NPC_KDK_e,           -1),
    OBJNAME("Aru",     fpcNm_NPC_ARU_e,           -1),
    OBJNAME("Bans",    fpcNm_NPC_BANS_e,          -1),
    OBJNAME("Besu",    fpcNm_NPC_BESU_e,          -1),
    OBJNAME("Bou",     fpcNm_NPC_BOU_e,           -1),
    OBJNAME("BouS",    fpcNm_NPC_BOU_S_e,         0x00),
    OBJNAME("clerkA",  fpcNm_NPC_CLERKA_e,        0x00),
    OBJNAME("clerkB",  fpcNm_NPC_CLERKB_e,        0x00),
    OBJNAME("clerkT",  fpcNm_NPC_CLERKT_e,        0x00),
    OBJNAME("Ash",     fpcNm_NPC_ASH_e,           -1),
    OBJNAME("AshB",    fpcNm_NPC_ASHB_e,          -1),
    OBJNAME("Shad",    fpcNm_NPC_SHAD_e,          -1),
    OBJNAME("Rafrel",  fpcNm_NPC_RAFREL_e,        -1),
    OBJNAME("MoiR",    fpcNm_NPC_MOIR_e,          -1),
    OBJNAME("impal",   fpcNm_NPC_IMPAL_e,         -1),
    OBJNAME("Coach",   fpcNm_NPC_COACH_e,         -1),
    OBJNAME("TheB",    fpcNm_NPC_THEB_e,          -1),
    OBJNAME("TgArena", fpcNm_Tag_Arena_e,         -1),
    OBJNAME("TagInst", fpcNm_Tag_Instruction_e,   -1),
    OBJNAME("GWolf",   fpcNm_NPC_GWOLF_e,         -1),
    OBJNAME("Len",     fpcNm_NPC_LEN_e,           -1),
    OBJNAME("Lud",     fpcNm_NPC_LUD_e,           -1),
    OBJNAME("Doc",     fpcNm_NPC_DOC_e,           -1),
    OBJNAME("FSeirei", fpcNm_NPC_FAIRY_SEIREI_e,  -1),
    OBJNAME("Fairy",   fpcNm_NPC_FAIRY_e,         -1),
    OBJNAME("Hanjo",   fpcNm_NPC_HANJO_e,         -1),
    OBJNAME("Hoz",     fpcNm_NPC_HOZ_e,           -1),
    OBJNAME("Jagar",   fpcNm_NPC_JAGAR_e,         -1),
    OBJNAME("Kkri",    fpcNm_NPC_KKRI_e,          -1),
    OBJNAME("Kn",      fpcNm_NPC_KN_e,            -1),
    OBJNAME("KnBlt",   fpcNm_KN_BULLET_e,         -1),
    OBJNAME("Knj",     fpcNm_NPC_KNJ_e,           -1),
    OBJNAME("Kolin",   fpcNm_NPC_KOLIN_e,         -1),
    OBJNAME("Kolinb",  fpcNm_NPC_KOLINB_e,        -1),
    OBJNAME("Kyury",   fpcNm_NPC_KYURY_e,         -1),
    OBJNAME("Maro",    fpcNm_NPC_MARO_e,          -1),
    OBJNAME("midP",    fpcNm_NPC_MIDP_e,          -1),
    OBJNAME("Moi",     fpcNm_NPC_MOI_e,           -1),
    OBJNAME("Raca",    fpcNm_NPC_RACA_e,          -1),
    OBJNAME("Post",    fpcNm_NPC_POST_e,          -1),
    OBJNAME("Pouya",   fpcNm_NPC_POUYA_e,         -1),
    OBJNAME("Saru",    fpcNm_NPC_SARU_e,          -1),
    OBJNAME("seiB",    fpcNm_NPC_SEIB_e,          -1),
    OBJNAME("seiC",    fpcNm_NPC_SEIC_e,          -1),
    OBJNAME("seiD",    fpcNm_NPC_SEID_e,          -1),
    OBJNAME("Seira",   fpcNm_NPC_SEIRA_e,         -1),
    OBJNAME("Seira2",  fpcNm_NPC_SERA2_e,         -1),
    OBJNAME("Seirei",  fpcNm_NPC_SEIREI_e,        -1),
    OBJNAME("Sha",     fpcNm_NPC_SHAMAN_e,        -1),
    OBJNAME("sMaro",   fpcNm_NPC_SMARO_e,         -1),
    OBJNAME("solA",    fpcNm_NPC_SOLA_e,          -1),
    OBJNAME("Taro",    fpcNm_NPC_TARO_e,          -1),
    OBJNAME("PA_Besu", fpcNm_NPC_PACHI_BESU_e,    -1),
    OBJNAME("PA_Taro", fpcNm_NPC_PACHI_TARO_e,    -1),
    OBJNAME("PA_Maro", fpcNm_NPC_PACHI_MARO_e,    -1),
    OBJNAME("TagPati", fpcNm_TAG_PATI_e,          -1),
    OBJNAME("The",     fpcNm_NPC_THE_e,           -1),
    OBJNAME("Tkj",     fpcNm_NPC_TKJ_e,           -1),
    OBJNAME("Tks",     fpcNm_NPC_TKS_e,           -1),
    OBJNAME("ObjTks",  fpcNm_OBJ_TKS_e,           -1),
    OBJNAME("Tkc",     fpcNm_NPC_TKC_e,           -1),
    OBJNAME("Tkj2",    fpcNm_NPC_TKJ2_e,          -1),
    OBJNAME("Toby",    fpcNm_NPC_TOBY_e,          -1),
    OBJNAME("Uri",     fpcNm_NPC_URI_e,           -1),
    OBJNAME("Yelia",   fpcNm_NPC_YELIA_e,         -1),
    OBJNAME("ykM",     fpcNm_NPC_YKM_e,           -1),
    OBJNAME("ykW",     fpcNm_NPC_YKW_e,           -1),
    OBJNAME("zanB",    fpcNm_NPC_ZANB_e,          -1),
    OBJNAME("Zant",    fpcNm_NPC_ZANT_e,          -1),
    OBJNAME("Zelda",   fpcNm_NPC_ZELDA_e,         -1),
    OBJNAME("ZelR",    fpcNm_NPC_ZELR_e,          -1),
    OBJNAME("ZelRo",   fpcNm_NPC_ZELRO_e,         -1),
    OBJNAME("zrA",     fpcNm_NPC_ZRA_e,           0x00),
    OBJNAME("zrS",     fpcNm_NPC_ZRA_e,           0x01),
    OBJNAME("zrWF",    fpcNm_NPC_ZRA_e,           0x02),
    OBJNAME("zrR",     fpcNm_NPC_ZRA_e,           0x03),
    OBJNAME("zrD",     fpcNm_NPC_ZRA_e,           0x04),
    OBJNAME("zrSP",    fpcNm_NPC_ZRA_e,           0x05),
    OBJNAME("zrSPA",   fpcNm_NPC_ZRA_e,           0x06),
    OBJNAME("zrF",     fpcNm_OBJ_ZRAFREEZE_e,     0x00),
    OBJNAME("zrF2",    fpcNm_OBJ_ZRAFREEZE_e,     0x01),
    OBJNAME("zrF3",    fpcNm_OBJ_ZRAFREEZE_e,     0x02),
    OBJNAME("zrC",     fpcNm_NPC_ZRC_e,           -1),
    OBJNAME("zrZ",     fpcNm_NPC_ZRZ_e,           -1),
    OBJNAME("GrvStn",  fpcNm_Obj_GraveStone_e,    -1),
    OBJNAME("zrARock", fpcNm_Obj_ZraRock_e,       -1),
    OBJNAME("StAndGl", fpcNm_START_AND_GOAL_e,    -1),
    OBJNAME("zrAMark", fpcNm_ZRA_MARK_e,          -1),
    OBJNAME("myna2",   fpcNm_MYNA2_e,             -1),
    OBJNAME("myn2tag", fpcNm_TAG_MYNA2_e,         -1),
    OBJNAME("MAN_a",   fpcNm_NPC_CD3_e,           0x00),
    OBJNAME("MAD_a",   fpcNm_NPC_CD3_e,           0x01),
    OBJNAME("MCN_a",   fpcNm_NPC_CD3_e,           0x02),
    OBJNAME("MON_a",   fpcNm_NPC_CD3_e,           0x03),
    OBJNAME("MAN_b",   fpcNm_NPC_CD3_e,           0x04),
    OBJNAME("MAN_c",   fpcNm_NPC_CD3_e,           0x05),
    OBJNAME("MAS_a",   fpcNm_NPC_CD3_e,           0x06),
    OBJNAME("MBN_a",   fpcNm_NPC_CD3_e,           0x07),
    OBJNAME("MAN_a2",  fpcNm_NPC_CD3_e,           0x08),
    OBJNAME("MAD_a2",  fpcNm_NPC_CD3_e,           0x09),
    OBJNAME("MCN_a2",  fpcNm_NPC_CD3_e,           0x0A),
    OBJNAME("MON_a2",  fpcNm_NPC_CD3_e,           0x0B),
    OBJNAME("MAN_b2",  fpcNm_NPC_CD3_e,           0x0C),
    OBJNAME("MAN_c2",  fpcNm_NPC_CD3_e,           0x0D),
    OBJNAME("MAS_a2",  fpcNm_NPC_CD3_e,           0x0E),
    OBJNAME("MBN_a2",  fpcNm_NPC_CD3_e,           0x0F),
    OBJNAME("WAN_a",   fpcNm_NPC_CD3_e,           0x10),
    OBJNAME("WAD_a",   fpcNm_NPC_CD3_e,           0x11),
    OBJNAME("MAT_a",   fpcNm_NPC_CD3_e,           0x12),
    OBJNAME("WCN_a",   fpcNm_NPC_CD3_e,           0x13),
    OBJNAME("WON_a",   fpcNm_NPC_CD3_e,           0x14),
    OBJNAME("WGN_a",   fpcNm_NPC_CD3_e,           0x15),
    OBJNAME("WAN_b",   fpcNm_NPC_CD3_e,           0x16),
    OBJNAME("WAN_a2",  fpcNm_NPC_CD3_e,           0x17),
    OBJNAME("WAD_a2",  fpcNm_NPC_CD3_e,           0x18),
    OBJNAME("MAT_a2",  fpcNm_NPC_CD3_e,           0x19),
    OBJNAME("WCN_a2",  fpcNm_NPC_CD3_e,           0x1A),
    OBJNAME("WON_a2",  fpcNm_NPC_CD3_e,           0x1B),
    OBJNAME("WGN_a2",  fpcNm_NPC_CD3_e,           0x1C),
    OBJNAME("WAN_b2",  fpcNm_NPC_CD3_e,           0x1D),
    OBJNAME("shoe",    fpcNm_NPC_SHOE_e,          -1),
    OBJNAME("DoorBoy", fpcNm_NPC_DOORBOY_e,       -1),
    OBJNAME("prayer",  fpcNm_NPC_PRAYER_e,        -1),
    OBJNAME("km_Hana", fpcNm_NPC_KASIHANA_e,      -1),
    OBJNAME("km_Kyu",  fpcNm_NPC_KASIKYU_e,       -1),
    OBJNAME("km_Mich", fpcNm_NPC_KASIMICH_e,      -1),
    OBJNAME("NpcChat", fpcNm_NPC_CHAT_e,          -1),
    OBJNAME("chtSolA", fpcNm_NPC_SOLDIERa_e,      -1),
    OBJNAME("chtSolB", fpcNm_NPC_SOLDIERb_e,      -1),
    OBJNAME("DrSol1",  fpcNm_NPC_DRSOL_e,         0x00),
    OBJNAME("DrSol2",  fpcNm_NPC_DRSOL_e,         0x01),
    OBJNAME("chin",    fpcNm_NPC_CHIN_e,          -1),
    OBJNAME("ins",     fpcNm_NPC_INS_e,           -1),
    OBJNAME("Shop0",   fpcNm_NPC_SHOP0_e,         -1),
    OBJNAME("TagSch",  fpcNm_Tag_Schedule_e,      -1),
    OBJNAME("TagEsc",  fpcNm_Tag_Escape_e,        -1),
    OBJNAME("passer",  fpcNm_PASSER_MNG_e,        0x00),
    OBJNAME("passerL", fpcNm_PASSER_MNG_e,        0x01),
    OBJNAME("fmtion",  fpcNm_FORMATION_MNG_e,     0x00),
    OBJNAME("fmtionL", fpcNm_FORMATION_MNG_e,     0x01),
    OBJNAME("guard",   fpcNm_GUARD_MNG_e,         -1),
    OBJNAME("TgGuard", fpcNm_TAG_GUARD_e,         -1),
    OBJNAME("Mk",      fpcNm_NPC_MK_e,            -1),
    OBJNAME("Obj_Tbi", fpcNm_Obj_Yobikusa_e,      0x00),
    OBJNAME("Obj_Uma", fpcNm_Obj_Yobikusa_e,      0x01),
    OBJNAME("Obj_knk", fpcNm_Obj_KazeNeko_e,      -1),
    OBJNAME("Obj_nmp", fpcNm_Obj_NamePlate_e,     -1),
    OBJNAME("Obj_Tie", fpcNm_Obj_OnCloth_e,       -1),
    OBJNAME("Obj_Lrp", fpcNm_Obj_LndRope_e,       -1),
    OBJNAME("Obj_Sit", fpcNm_Obj_ItaRope_e,       -1),
    OBJNAME("Ikada",   fpcNm_Obj_Ikada_e,         -1),
    OBJNAME("Dust",    fpcNm_Obj_DUST_e,          -1),
    OBJNAME("Obj_Ita", fpcNm_Obj_ITA_e,           -1),
    OBJNAME("Ice_l",   fpcNm_Obj_Ice_l_e,         -1),
    OBJNAME("Ice_s",   fpcNm_Obj_Ice_s_e,         -1),
    OBJNAME("E_Make",  fpcNm_Obj_E_CREATE_e,      -1),
    OBJNAME("Bhhashi", fpcNm_Obj_Bhbridge_e,      -1),
    OBJNAME("Yousei",  fpcNm_Obj_Yousei_e,        -1),
    OBJNAME("G_Kabe",  fpcNm_Obj_GOMIKABE_e,      -1),
    OBJNAME("B_ling",  fpcNm_Obj_Mato_e,          -1),
    OBJNAME("kab_o",   fpcNm_Obj_Kabuto_e,        -1),
    OBJNAME("I_Cho",   fpcNm_Obj_Cho_e,           -1),
    OBJNAME("I_Kuw",   fpcNm_Obj_Kuw_e,           -1),
    OBJNAME("I_Nan",   fpcNm_Obj_Nan_e,           -1),
    OBJNAME("I_Dan",   fpcNm_Obj_Dan_e,           -1),
    OBJNAME("I_Kam",   fpcNm_Obj_Kam_e,           -1),
    OBJNAME("I_Ten",   fpcNm_Obj_Ten_e,           -1),
    OBJNAME("I_Ari",   fpcNm_Obj_Ari_e,           -1),
    OBJNAME("I_Kag",   fpcNm_Obj_Kag_e,           -1),
    OBJNAME("I_Tom",   fpcNm_Obj_Tombo_e,         -1),
    OBJNAME("I_Bat",   fpcNm_Obj_Batta_e,         -1),
    OBJNAME("I_Kat",   fpcNm_Obj_Kat_e,           -1),
    OBJNAME("H_Saku",  fpcNm_Obj_H_Saku_e,        -1),
    OBJNAME("sm_door", fpcNm_Obj_SM_DOOR_e,       -1),
    OBJNAME("Kaisou",  fpcNm_Obj_Kaisou_e,        -1),
    OBJNAME("Ni",      fpcNm_NI_e,                -1),
    OBJNAME("Fr",      fpcNm_FR_e,                -1),
    OBJNAME("Sq",      fpcNm_SQ_e,                -1),
    OBJNAME("Bd",      fpcNm_BD_e,                -1),
    OBJNAME("Obj_ki",  fpcNm_OBJ_KI_e,            -1),
    OBJNAME("Do",      fpcNm_DO_e,                -1),
    OBJNAME("Lm",      fpcNm_NPC_FISH_e,          0x00),
    OBJNAME("Tr",      fpcNm_NPC_FISH_e,          0x05),
    OBJNAME("Npc_tr",  fpcNm_NPC_TR_e,            -1),
    OBJNAME("Npc_lf",  fpcNm_NPC_LF_e,            -1),
    OBJNAME("Fish",    fpcNm_MG_FISH_e,           -1),
    OBJNAME("NPC_TK",  fpcNm_NPC_TK_e,            -1),
    OBJNAME("Obj_fd",  fpcNm_OBJ_FOOD_e,          -1),
    OBJNAME("MYNA",    fpcNm_MYNA_e,              -1),
    OBJNAME("SSITEM",  fpcNm_OBJ_SSITEM_e,        -1),
    OBJNAME("SSDRINK", fpcNm_OBJ_SSDRINK_e,       -1),
    OBJNAME("TGDRINK", fpcNm_TAG_SSDRINK_e,       -1),
    OBJNAME("Tag_Btl", fpcNm_TAG_BTLITM_e,        -1),
    OBJNAME("lv5soup", fpcNm_TAG_LV5SOUP_e,       -1),
    OBJNAME("TGMNLIG", fpcNm_TAG_MNLIGHT_e,       -1),
    OBJNAME("TGSPCAM", fpcNm_TAG_SHOPCAM_e,       -1),
    OBJNAME("TGSPITM", fpcNm_TAG_SHOPITM_e,       -1),
    OBJNAME("OilTubo", fpcNm_OBJ_OILTUBO_e,       -1),
    OBJNAME("Roten",   fpcNm_OBJ_ROTEN_e,         -1),
    OBJNAME("Pleaf",   fpcNm_OBJ_PLEAF_e,         -1),
    OBJNAME("KBacket", fpcNm_OBJ_KBACKET_e,       -1),
    OBJNAME("YBag",    fpcNm_OBJ_YBAG_e,          -1),
    OBJNAME("Pumpkin", fpcNm_OBJ_PUMPKIN_e,       -1),
    OBJNAME("Obj_kg",  fpcNm_OBJ_KAGO_e,          -1),
    OBJNAME("BbKago",  fpcNm_OBJ_KAGO_e,          0x00),
    OBJNAME("OnKago",  fpcNm_OBJ_KAGO_e,          0x01),
    OBJNAME("ObjHasi", fpcNm_Obj_HHASHI_e,        -1),
    OBJNAME("Thashi",  fpcNm_Obj_THASHI_e,        -1),
    OBJNAME("B_Hashi", fpcNm_Obj_BHASHI_e,        -1),
    OBJNAME("E_MD",    fpcNm_E_MD_e,              -1),
    OBJNAME("E_s1",    fpcNm_E_S1_e,              -1),
    OBJNAME("E_wb",    fpcNm_E_WB_e,              -1),
    OBJNAME("E_rd",    fpcNm_E_RD_e,              -1),
    OBJNAME("E_rdb",   fpcNm_E_RDB_e,             -1),
    OBJNAME("E_rdy",   fpcNm_E_RDY_e,             -1),
    OBJNAME("E_fm",    fpcNm_E_FM_e,              -1),
    OBJNAME("E_dn",    fpcNm_E_DN_e,              -1),
    OBJNAME("E_mf",    fpcNm_E_MF_e,              -1),
    OBJNAME("TreeSh",  fpcNm_TREESH_e,            -1),
    OBJNAME("E_bug",   fpcNm_E_BUG_e,             -1),
    OBJNAME("E_bee",   fpcNm_E_BEE_e,             -1),
    OBJNAME("E_nest",  fpcNm_E_NEST_e,            -1),
    OBJNAME("E_kr",    fpcNm_E_KR_e,              -1),
    OBJNAME("E_st",    fpcNm_E_ST_e,              -1),
    OBJNAME("E_tk",    fpcNm_E_TK_e,              -1),
    OBJNAME("E_tk2",   fpcNm_E_TK2_e,             -1),
    OBJNAME("E_th",    fpcNm_E_TH_e,              -1),
    OBJNAME("E_cr",    fpcNm_E_CR_e,              -1),
    OBJNAME("E_df",    fpcNm_E_DF_e,              -1),
    OBJNAME("E_ph",    fpcNm_E_PH_e,              -1),
    OBJNAME("E_fs",    fpcNm_E_FS_e,              -1),
    OBJNAME("E_SB",    fpcNm_E_SB_e,              -1),
    OBJNAME("E_HM",    fpcNm_E_HM_e,              -1),
    OBJNAME("E_fk",    fpcNm_E_FK_e,              -1),
    OBJNAME("E_pm",    fpcNm_E_PM_e,              -1),
    OBJNAME("Npc_ne",  fpcNm_NPC_NE_e,            -1),
    OBJNAME("E_db",    fpcNm_E_DB_e,              -1),
    OBJNAME("E_kg",    fpcNm_E_KG_e,              -1),
    OBJNAME("E_sh",    fpcNm_E_SH_e,              -1),
    OBJNAME("E_sf",    fpcNm_E_SF_e,              -1),
    OBJNAME("Obj_lb",  fpcNm_OBJ_LBOX_e,          -1),
    OBJNAME("Obj_kb",  fpcNm_OBJ_KBOX_e,          -1),
    OBJNAME("E_bs",    fpcNm_E_BS_e,              -1),
    OBJNAME("Sekizoa", fpcNm_OBJ_SEKIZOA_e,       0x00),
    OBJNAME("Sekizob", fpcNm_OBJ_SEKIZOA_e,       0x01),
    OBJNAME("E_sm",    fpcNm_E_SM_e,              -1),
    OBJNAME("E_sm2",   fpcNm_E_SM2_e,             -1),
    OBJNAME("ky_tag0", fpcNm_KYTAG00_e,           -1),
    OBJNAME("ky_tag1", fpcNm_KYTAG01_e,           0x00),
    OBJNAME("ky_tag2", fpcNm_KYTAG02_e,           -1),
    OBJNAME("ky_tag3", fpcNm_KYTAG01_e,           0x01),
    OBJNAME("kytag3",  fpcNm_KYTAG03_e,           -1),
    OBJNAME("DK_tag",  fpcNm_KYTAG04_e,           -1),
    OBJNAME("PPHole",  fpcNm_KYTAG05_e,           -1),
    OBJNAME("KY_chg",  fpcNm_KYTAG06_e,           -1),
    OBJNAME("Plight",  fpcNm_KYTAG07_e,           -1),
    OBJNAME("KY_mk",   fpcNm_KYTAG08_e,           -1),
    OBJNAME("Bound",   fpcNm_KYTAG09_e,           -1),
    OBJNAME("WC_tag",  fpcNm_KYTAG01_e,           0x02),
    OBJNAME("Sparks",  fpcNm_KYTAG10_e,           -1),
    OBJNAME("Tcntrl",  fpcNm_KYTAG11_e,           -1),
    OBJNAME("Drkmst",  fpcNm_KYTAG12_e,           -1),
    OBJNAME("Blsnow",  fpcNm_KYTAG13_e,           -1),
    OBJNAME("Savmem",  fpcNm_KYTAG14_e,           -1),
    OBJNAME("Zshake",  fpcNm_KYTAG15_e,           -1),
    OBJNAME("Pikari",  fpcNm_KYTAG16_e,           -1),
    OBJNAME("Lmask",   fpcNm_KYTAG17_e,           -1),
    OBJNAME("E_is",    fpcNm_E_IS_e,              -1),
    OBJNAME("E_ai",    fpcNm_E_AI_e,              -1),
    OBJNAME("E_ba",    fpcNm_E_BA_e,              -1),
    OBJNAME("E_bu",    fpcNm_E_BU_e,              -1),
    OBJNAME("E_sw",    fpcNm_E_SW_e,              -1),
    OBJNAME("E_ge",    fpcNm_E_GE_e,              -1),
    OBJNAME("TagGe",   fpcNm_Tag_WatchGe_e,       -1),
    OBJNAME("E_ym",    fpcNm_E_YM_e,              -1),
    OBJNAME("E_ymt",   fpcNm_E_YM_TAG_e,          -1),
    OBJNAME("E_ymb",   fpcNm_E_YMB_e,             -1),
    OBJNAME("Fwall",   fpcNm_Tag_FWall_e,         -1),
    OBJNAME("E_hz",    fpcNm_E_HZ_e,              -1),
    OBJNAME("E_ws",    fpcNm_E_WS_e,              -1),
    OBJNAME("E_oc",    fpcNm_E_OC_e,              -1),
    OBJNAME("E_dt",    fpcNm_E_DT_e,              -1),
    OBJNAME("E_bg",    fpcNm_E_BG_e,              -1),
    OBJNAME("E_octbg", fpcNm_E_OctBg_e,           -1),
    OBJNAME("E_tt",    fpcNm_E_TT_e,              -1),
    OBJNAME("E_dk",    fpcNm_E_DK_e,              -1),
    OBJNAME("E_vt",    fpcNm_E_VT_e,              -1),
    OBJNAME("E_ww",    fpcNm_E_WW_e,              -1),
    OBJNAME("E_gi",    fpcNm_E_GI_e,              -1),
    OBJNAME("E_mk",    fpcNm_E_MK_e,              -1),
    OBJNAME("B_gm",    fpcNm_B_GM_e,              -1),
    OBJNAME("E_gm",    fpcNm_E_GM_e,              -1),
    OBJNAME("E_gs",    fpcNm_E_GS_e,              -1),
    OBJNAME("Npc_ne",  fpcNm_NPC_NE_e,            -1),
    OBJNAME("Obj_gm",  fpcNm_OBJ_GM_e,            -1),
    OBJNAME("E_mm",    fpcNm_E_MM_e,              -1),
    OBJNAME("E_mm2",   fpcNm_E_MM_e,              0x01),
    OBJNAME("E_fz",    fpcNm_E_FZ_e,              -1),
    OBJNAME("E_fb",    fpcNm_E_FB_e,              -1),
    OBJNAME("E_Zs",    fpcNm_E_ZS_e,              -1),
    OBJNAME("E_kk",    fpcNm_E_KK_e,              -1),
    OBJNAME("E_hp",    fpcNm_E_HP_e,              -1),
    OBJNAME("E_zh",    fpcNm_E_ZH_e,              -1),
    OBJNAME("E_zm",    fpcNm_E_ZM_e,              -1),
    OBJNAME("E_zm2",   fpcNm_E_ZM_e,              0x01),
    OBJNAME("E_pz",    fpcNm_E_PZ_e,              -1),
    OBJNAME("Kita",    fpcNm_OBJ_KITA_e,          -1),
    OBJNAME("E_nz",    fpcNm_E_NZ_e,              -1),
    OBJNAME("E_po",    fpcNm_E_PO_e,              -1),
    OBJNAME("Obj_w0",  fpcNm_OBJ_WEB0_e,          -1),
    OBJNAME("Obj_w1",  fpcNm_OBJ_WEB1_e,          -1),
    OBJNAME("Obj_cb",  fpcNm_OBJ_CB_e,            -1),
    OBJNAME("Obj_mak", fpcNm_OBJ_MAKI_e,          -1),
    OBJNAME("Obj_brg", fpcNm_OBJ_BRG_e,           -1),
    OBJNAME("E_rb",    fpcNm_E_RB_e,              -1),
    OBJNAME("E_sg",    fpcNm_E_SG_e,              -1),
    OBJNAME("Obj_tby", fpcNm_OBJ_TOBY_e,          -1),
    OBJNAME("Obj_sw",  fpcNm_OBJ_SW_e,            -1),
    OBJNAME("Npc_ks",  fpcNm_NPC_KS_e,            -1),
    OBJNAME("Obj_so",  fpcNm_OBJ_SO_e,            -1),
    OBJNAME("E_bi",    fpcNm_E_BI_e,              -1),
    OBJNAME("Obj_key", fpcNm_OBJ_KEY_e,           -1),
    OBJNAME("E_hb",    fpcNm_E_HB_e,              -1),
    OBJNAME("Obj_hb",  fpcNm_OBJ_HB_e,            -1),
    OBJNAME("B_bq",    fpcNm_B_BQ_e,              -1),
    OBJNAME("B_bh",    fpcNm_B_BH_e,              -1),
    OBJNAME("B_go",    fpcNm_B_GO_e,              -1),
    OBJNAME("Obj_fw",  fpcNm_OBJ_FW_e,            -1),
    OBJNAME("E_ga",    fpcNm_E_GA_e,              -1),
    OBJNAME("E_gb",    fpcNm_E_GB_e,              -1),
    OBJNAME("E_wap",   fpcNm_E_WAP_e,             -1),
    OBJNAME("Obj_sui", fpcNm_OBJ_SUISYA_e,        -1),
    OBJNAME("Obj_bal", fpcNm_OBJ_BALLOON_e,       -1),
    OBJNAME("Obj_lp",  fpcNm_OBJ_LP_e,            -1),
    OBJNAME("Obj_nd",  fpcNm_OBJ_NDOOR_e,         -1),
    OBJNAME("Obj_ud",  fpcNm_OBJ_UDOOR_e,         -1),
    OBJNAME("Obj_us",  fpcNm_OBJ_USAKU_e,         -1),
    OBJNAME("Obj_kn2", fpcNm_OBJ_KANBAN2_e,       -1),
    OBJNAME("Obj_kyh", fpcNm_OBJ_KEYHOLE_e,       -1),
    OBJNAME("Bed",     fpcNm_OBJ_BED_e,           -1),
    OBJNAME("BouMato", fpcNm_OBJ_BOUMATO_e,       -1),
    OBJNAME("AutoMt",  fpcNm_OBJ_AUTOMATA_e,      -1),
    OBJNAME("Gadget",  fpcNm_OBJ_GADGET_e,        -1),
    OBJNAME("ItaMato", fpcNm_OBJ_ITAMATO_e,       -1),
    OBJNAME("Nougu",   fpcNm_OBJ_NOUGU_e,         -1),
    OBJNAME("Stick",   fpcNm_OBJ_STICK_e,         -1),
    OBJNAME("Mie",     fpcNm_OBJ_MIE_e,           -1),
    OBJNAME("SkDoor",  fpcNm_OBJ_SEKIDOOR_e,      -1),
    OBJNAME("Sekizo",  fpcNm_OBJ_SEKIZO_e,        -1),
    OBJNAME("SMTile",  fpcNm_OBJ_SMTILE_e,        -1),
    OBJNAME("H_kage",  fpcNm_OBJ_KAGE_e,          -1),
    OBJNAME("E_ms",    fpcNm_E_MS_e,              -1),
    OBJNAME("Obj_tp",  fpcNm_OBJ_TP_e,            -1),
    OBJNAME("Obj_ms",  fpcNm_OBJ_MSIMA_e,         -1),
    OBJNAME("E_gob",   fpcNm_E_GOB_e,             -1),
    OBJNAME("E_yk",    fpcNm_E_YK_e,              -1),
    OBJNAME("E_yr",    fpcNm_E_YR_e,              -1),
    OBJNAME("E_yg",    fpcNm_E_YG_e,              -1),
    OBJNAME("Obj_fm",  fpcNm_OBJ_FMOBJ_e,         -1),
    OBJNAME("B_ob",    fpcNm_B_OB_e,              -1),
    OBJNAME("OcHashi", fpcNm_OCTHASHI_e,          -1),
    OBJNAME("E_dd",    fpcNm_E_DD_e,              -1),
    OBJNAME("Obj_rw",  fpcNm_OBJ_RW_e,            -1),
    OBJNAME("Obj_ih",  fpcNm_OBJ_IHASI_e,         -1),
    OBJNAME("E_yc",    fpcNm_E_YC_e,              -1),
    OBJNAME("B_ds",    fpcNm_B_DS_e,              -1),
    OBJNAME("B_dr",    fpcNm_B_DR_e,              -1),
    OBJNAME("B_gg",    fpcNm_B_GG_e,              -1),
    OBJNAME("B_yo",    fpcNm_B_YO_e,              -1),
    OBJNAME("B_tn",    fpcNm_B_TN_e,              -1),
    OBJNAME("B_zant",  fpcNm_B_ZANT_e,            -1),
    OBJNAME("B_zntm",  fpcNm_B_ZANTZ_e,           -1),
    OBJNAME("B_znts",  fpcNm_B_ZANTS_e,           -1),
    OBJNAME("E_yd",    fpcNm_E_YD_e,              -1),
    OBJNAME("E_yh",    fpcNm_E_YH_e,              -1),
    OBJNAME("Henna",   fpcNm_NPC_HENNA_e,         -1),
    OBJNAME("Henna0",  fpcNm_NPC_HENNA0_e,        -1),
    OBJNAME("Npc_du",  fpcNm_NPC_DU_e,            0x00),
    OBJNAME("Econt",   fpcNm_ECONT_e,             -1),
    OBJNAME("B_mgn",   fpcNm_B_MGN_e,             -1),
    OBJNAME("Hzelda",  fpcNm_E_HZELDA_e,          -1),
    OBJNAME("Worm",    fpcNm_NPC_WORM_e,          -1),
    OBJNAME("B_gnd",   fpcNm_B_GND_e,             -1),
    OBJNAME("Obj_gb",  fpcNm_OBJ_GB_e,            -1),
    OBJNAME("tboxA0",  fpcNm_TBOX_e,              -1),
    OBJNAME("tboxA1",  fpcNm_TBOX_e,              -1),
    OBJNAME("tboxA2",  fpcNm_TBOX_e,              -1),
    OBJNAME("tboxB0",  fpcNm_TBOX_e,              -1),
    OBJNAME("tboxB1",  fpcNm_TBOX_e,              -1),
    OBJNAME("tboxB2",  fpcNm_TBOX_e,              -1),
    OBJNAME("tboxJ0",  fpcNm_TBOX_e,              -1),
    OBJNAME("tboxP0",  fpcNm_TBOX_e,              -1),
    OBJNAME("tboxB00", fpcNm_TBOX_e,              -1),
    OBJNAME("tboxB01", fpcNm_TBOX_e,              -1),
    OBJNAME("tboxW0",  fpcNm_TBOX_e,              -1),
    OBJNAME("tboxEL0", fpcNm_TBOX2_e,             -1),
    OBJNAME("tboxEL1", fpcNm_TBOX2_e,             -1),
    OBJNAME("l8warp",  fpcNm_Obj_BossWarp_e,      -1),
    OBJNAME("L1Bdoor", fpcNm_L1BOSS_DOOR_e,       -1),
    OBJNAME("L2Bdoor", fpcNm_L1BOSS_DOOR_e,       -1),
    OBJNAME("L3Bdoor", fpcNm_Obj_Kshutter_e,      -1),
    OBJNAME("L4Bdoor", fpcNm_L1BOSS_DOOR_e,       -1),
    OBJNAME("L5Bdoor", fpcNm_L5BOSS_DOOR_e,       -1),
    OBJNAME("L6Bdoor", fpcNm_L1BOSS_DOOR_e,       -1),
    OBJNAME("L7Bdoor", fpcNm_L1BOSS_DOOR_e,       -1),
    OBJNAME("L8Bdoor", fpcNm_L1BOSS_DOOR_e,       -1),
    OBJNAME("L9Bdoor", fpcNm_L1BOSS_DOOR_e,       -1),
    OBJNAME("L1Mdoor", fpcNm_L1MBOSS_DOOR_e,      0x01),
    OBJNAME("L2Mdoor", fpcNm_L1MBOSS_DOOR_e,      0x02),
    OBJNAME("L3Mdoor", fpcNm_L1MBOSS_DOOR_e,      0x03),
    OBJNAME("L4Mdoor", fpcNm_L1MBOSS_DOOR_e,      0x04),
    OBJNAME("L5Mdoor", fpcNm_L1MBOSS_DOOR_e,      0x05),
    OBJNAME("L6Mdoor", fpcNm_L1MBOSS_DOOR_e,      0x06),
    OBJNAME("L7Mdoor", fpcNm_L1MBOSS_DOOR_e,      0x07),
    OBJNAME("L8Mdoor", fpcNm_L1MBOSS_DOOR_e,      0x08),
    OBJNAME("L7door",  fpcNm_L1MBOSS_DOOR_e,      0x0A),
    OBJNAME("L5door",  fpcNm_L1MBOSS_DOOR_e,      0x0B),
    OBJNAME("Horse",   fpcNm_HORSE_e,             -1),
    OBJNAME("Canoe",   fpcNm_CANOE_e,             -1),
    OBJNAME("Crope",   fpcNm_Obj_Crope_e,         -1),
    OBJNAME("Cstatue", fpcNm_CSTATUE_e,           -1),
    OBJNAME("CstaF",   fpcNm_CSTAF_e,             -1),
    OBJNAME("HoZelda", fpcNm_HOZELDA_e,           -1),
    OBJNAME("Mhint",   fpcNm_Tag_Mhint_e,         -1),
    OBJNAME("Mmsg",    fpcNm_Tag_Mmsg_e,          -1),
    OBJNAME("Mwait",   fpcNm_Tag_Mwait_e,         -1),
    OBJNAME("Mstop",   fpcNm_Tag_Mstop_e,         -1),
    OBJNAME("MstopE",  fpcNm_Tag_Mstop_e,         -1),
    OBJNAME("Hinit",   fpcNm_Tag_Hinit_e,         -1),
    OBJNAME("Hjump",   fpcNm_Tag_Hjump_e,         -1),
    OBJNAME("Hstop",   fpcNm_Tag_Hstop_e,         -1),
    OBJNAME("AJnot",   fpcNm_Tag_AJnot_e,         -1),
    OBJNAME("Wljump",  fpcNm_Tag_Wljump_e,        -1),
    OBJNAME("Sppath",  fpcNm_Tag_Sppath_e,        -1),
    OBJNAME("Stream",  fpcNm_Tag_Stream_e,        -1),
    OBJNAME("Gstart",  fpcNm_Tag_Gstart_e,        -1),
    OBJNAME("TgTGate", fpcNm_Tag_TWGate_e,        -1),
    OBJNAME("Lv6Gate", fpcNm_Tag_Lv6Gate_e,       -1),
    OBJNAME("Lv7Gate", fpcNm_Tag_Lv7Gate_e,       -1),
    OBJNAME("Lv8Gate", fpcNm_Tag_Lv8Gate_e,       -1),
    OBJNAME("TgTHint", fpcNm_Tag_TheBHint_e,      -1),
    OBJNAME("TagAsst", fpcNm_Tag_Assist_e,        -1),
    OBJNAME("Dmidna",  fpcNm_DMIDNA_e,            -1),
    OBJNAME("Fchain",  fpcNm_Obj_Fchain_e,        -1),
    OBJNAME("Wchain",  fpcNm_Obj_Wchain_e,        -1),
    OBJNAME("Cdoor",   fpcNm_Obj_Cdoor_e,         -1),
    OBJNAME("Cgate",   fpcNm_Obj_Cdoor_e,         -1),
    OBJNAME("Water00", fpcNm_GRDWATER_e,          -1),
    OBJNAME("Digpl",   fpcNm_Obj_Digpl_e,         -1),
    OBJNAME("Digholl", fpcNm_Obj_Digholl_e,       -1),
    OBJNAME("Bombf",   fpcNm_Obj_Bombf_e,         -1),
    OBJNAME("hider",   fpcNm_SUSPEND_e,           -1),
    OBJNAME("SwAreaC", fpcNm_SWC00_e,             -1),
    OBJNAME("SwAreaS", fpcNm_SWC00_e,             -1),
    OBJNAME("SwLBall", fpcNm_SwLBall_e,           -1),
    OBJNAME("SwBall",  fpcNm_SwBall_e,            -1),
    OBJNAME("thouse",  fpcNm_Obj_TobyHouse_e,     -1),
    OBJNAME("ironbox", fpcNm_Obj_MetalBox_e,      -1),
    OBJNAME("l6swtrn", fpcNm_Obj_Lv6SwTurn_e,     -1),
    OBJNAME("l2bmtag", fpcNm_Tag_Lv2PrChk_e,      -1),
    OBJNAME("swspin",  fpcNm_Obj_SwSpinner_e,     -1),
    OBJNAME("spnGear", fpcNm_Obj_Lv4Gear_e,       -1),
    OBJNAME("fan",     fpcNm_Obj_Fan_e,           -1),
    OBJNAME("tgake",   fpcNm_Obj_Gake_e,          -1),
    OBJNAME("tmoon",   fpcNm_Obj_TMoon_e,         -1),
    OBJNAME("gwall",   fpcNm_Obj_GanonWall_e,     -1),
    OBJNAME("gwall2",  fpcNm_Obj_GanonWall2_e,    -1),
    OBJNAME("scnChg",  fpcNm_SCENE_EXIT_e,        -1),
    OBJNAME("scnChg2", fpcNm_SCENE_EXIT2_e,       -1),
    OBJNAME("noChgRm", fpcNm_NO_CHG_ROOM_e,       -1),
    OBJNAME("readRm",  fpcNm_NO_CHG_ROOM_e,       0x00),
    OBJNAME("Tetd",    fpcNm_DSHUTTER_e,          -1),
    OBJNAME("fence",   fpcNm_DSHUTTER_e,          -1),
    OBJNAME("lv7saku", fpcNm_DSHUTTER_e,          -1),
    OBJNAME("YkgrON",  fpcNm_Ykgr_e,              -1),
    OBJNAME("YkgrOFF", fpcNm_Ykgr_e,              -1),
    OBJNAME("TagEv",   fpcNm_TAG_EVENT_e,         -1),
    OBJNAME("TagEvC",  fpcNm_TAG_EVENT_e,         -1),
    OBJNAME("TagEvt",  fpcNm_TAG_EVT_e,           0x00),
    OBJNAME("TagEvt1", fpcNm_TAG_EVT_e,           0x01),
    OBJNAME("TagHowl", fpcNm_TAG_HOWL_e,          0x01),
    OBJNAME("Tag_ms",  fpcNm_TAG_MSG_e,           0x00),
    OBJNAME("LntnTag", fpcNm_TAG_LANTERN_e,       -1),
    OBJNAME("EvtArea", fpcNm_TAG_EVTAREA_e,       -1),
    OBJNAME("AllMato", fpcNm_TAG_ALLMATO_e,       -1),
    OBJNAME("EvtMsg",  fpcNm_TAG_EVTMSG_e,        -1),
    OBJNAME("KMsg",    fpcNm_TAG_KMSG_e,          -1),
    OBJNAME("Push",    fpcNm_TAG_PUSH_e,          -1),
    OBJNAME("Teloper", fpcNm_TAG_TELOP_e,         -1),
    OBJNAME("CamChg",  fpcNm_TAG_CAMERA_e,        -1),
    OBJNAME("CamArea", fpcNm_TAG_CAMERA_e,        -1),
    OBJNAME("CamAreC", fpcNm_TAG_CAMERA_e,        -1),
    OBJNAME("ChkPntS", fpcNm_TAG_CHKPOINT_e,      -1),
    OBJNAME("ChkPntC", fpcNm_TAG_CHKPOINT_e,      -1),
    OBJNAME("setball", fpcNm_Tag_SetBall_e,       -1),
    OBJNAME("atkItem", fpcNm_Tag_AttackItem_e,    -1),
    OBJNAME("TagStat", fpcNm_Tag_Statue_e,        -1),
    OBJNAME("zdoor",   fpcNm_Obj_ZDoor_e,         -1),
    OBJNAME("rdoor",   fpcNm_Obj_ZDoor_e,         -1),
    OBJNAME("STDoa",   fpcNm_Obj_ZDoor_e,         -1),
    OBJNAME("hasi00",  fpcNm_Obj_Pillar_e,        -1),
    OBJNAME("hasi01",  fpcNm_Obj_Pillar_e,        -1),
    OBJNAME("window",  fpcNm_Obj_Window_e,        -1),
    OBJNAME("bbox",    fpcNm_Obj_BBox_e,          -1),
    OBJNAME("spiralC", fpcNm_Obj_Lv3R10Saka_e,    -1),
    OBJNAME("spiral2", fpcNm_Obj_Lv3R10Saka_e,    -1),
    OBJNAME("digsnow", fpcNm_Obj_DigSnow_e,       -1),
    OBJNAME("digsand", fpcNm_Obj_Lv4DigSand_e,    -1),
    OBJNAME("fallobj", fpcNm_Obj_FallObj_e,       -1),
    OBJNAME("l7brg",   fpcNm_Obj_Lv7Bridge_e,     -1),
    OBJNAME("dr",      fpcNm_DR_e,                -1),
    OBJNAME("L7lowDr", fpcNm_L7lowDr_e,           -1),
    OBJNAME("swTime",  fpcNm_SwTime_e,            -1),
    OBJNAME("pdrobj",  fpcNm_Obj_PushDoor_e,      -1),
    OBJNAME("crystal", fpcNm_Obj_Crystal_e,       -1),
    OBJNAME("WarpBrg", fpcNm_Obj_KakarikoBrg_e,   -1),
    OBJNAME("WarpB2",  fpcNm_Obj_KakarikoBrg_e,   -1),
    OBJNAME("WarpOB1", fpcNm_Obj_OrdinBrg_e,      0x00),
    OBJNAME("WarpOB2", fpcNm_Obj_OrdinBrg_e,      0x01),
    OBJNAME("VolcGnd", fpcNm_Obj_VolcGnd_e,       -1),
    OBJNAME("r09wtr",  fpcNm_Obj_WaterEff_e,      -1),
    OBJNAME("ballSw",  fpcNm_Obj_SwBallA_e,       -1),
    OBJNAME("lballSw", fpcNm_Obj_SwBallB_e,       -1),
    OBJNAME("lbsw",    fpcNm_Obj_SwBallC_e,       -1),
    OBJNAME("R50Sand", fpcNm_Obj_Lv4Sand_e,       -1),
    OBJNAME("rwall",   fpcNm_Obj_Lv4RailWall_e,   -1),
    OBJNAME("l4brg",   fpcNm_Obj_Lv4Bridge_e,     -1),
    OBJNAME("l4floor", fpcNm_Obj_Lv4Floor_e,      -1),
    OBJNAME("item",    fpcNm_ITEM_e,              -1),
    OBJNAME("itemKey", fpcNm_Obj_SmallKey_e,      -1),
    OBJNAME("kantera", fpcNm_Obj_Kantera_e,       -1),
    OBJNAME("witem",   fpcNm_ITEM_e,              -1),
    OBJNAME("wshield", fpcNm_Obj_Shield_e,        -1),
    OBJNAME("htPiece", fpcNm_Obj_LifeContainer_e, -1),
    OBJNAME("htCase",  fpcNm_Obj_LifeContainer_e, -1),
    OBJNAME("Mbrid15", fpcNm_Obj_RotBridge_e,     -1),
    OBJNAME("Mbrid9",  fpcNm_Obj_RotBridge_e,     -1),
    OBJNAME("maglift", fpcNm_Obj_MagLift_e,       -1),
    OBJNAME("Cldst00", fpcNm_Obj_Lv1Cdl00_e,      -1),
    OBJNAME("Cldst01", fpcNm_Obj_Lv1Cdl01_e,      -1),
    OBJNAME("RMback0", fpcNm_Tag_Restart_e,       -1),
    OBJNAME("RMback1", fpcNm_Tag_ChgRestart_e,    -1),
    OBJNAME("Mist",    fpcNm_Tag_Mist_e,          -1),
    OBJNAME("magLifR", fpcNm_Obj_MagLiftRot_e,    -1),
    OBJNAME("TCdlst",  fpcNm_Obj_TvCdlst_e,       -1),
    OBJNAME("wdstick", fpcNm_Obj_WdStick_e,       -1),
    OBJNAME("stBlock", fpcNm_Obj_StairBlock_e,    -1),
    OBJNAME("hsMato",  fpcNm_Obj_HsTarget_e,      -1),
    OBJNAME("ktFire",  fpcNm_Tag_KtOnFire_e,      -1),
    OBJNAME("hswitch", fpcNm_Obj_HeavySw_e,       -1),
    OBJNAME("goGate",  fpcNm_Obj_GoGate_e,        -1),
    OBJNAME("taAmi",   fpcNm_Obj_TaFence_e,       -1),
    OBJNAME("fireWd",  fpcNm_Obj_FireWood_e,      -1),
    OBJNAME("fireWd2", fpcNm_Obj_FireWood2_e,     -1),
    OBJNAME("altar",   fpcNm_Obj_Saidan_e,        -1),
    OBJNAME("gpTaru",  fpcNm_Obj_GpTaru_e,        -1),
    OBJNAME("spinLf",  fpcNm_Obj_SpinLift_e,      -1),
    OBJNAME("bmWin",   fpcNm_Obj_BmWindow_e,      -1),
    OBJNAME("roofHl",  fpcNm_Obj_RfHole_e,        -1),
    OBJNAME("syRock",  fpcNm_Obj_SyRock_e,        -1),
    OBJNAME("wColumn", fpcNm_Obj_WaterPillar_e,   -1),
    OBJNAME("kpot",    fpcNm_Obj_KiPot_e,         -1),
    OBJNAME("bsGate",  fpcNm_Obj_BsGate_e,        -1),
    OBJNAME("amiShut", fpcNm_Obj_AmiShutter_e,    -1),
    OBJNAME("kwhel00", fpcNm_Obj_KWheel00_e,      -1),
    OBJNAME("kwhel01", fpcNm_Obj_KWheel01_e,      -1),
    OBJNAME("PRElvtr", fpcNm_Obj_PRElvtr_e,       -1),
    OBJNAME("swhel00", fpcNm_Obj_KWheel00_e,      -1),
    OBJNAME("M_hasu",  fpcNm_Obj_MHasu_e,         -1),
    OBJNAME("yiblltr", fpcNm_Obj_YIblltray_e,     -1),
    OBJNAME("l6egate", fpcNm_Obj_Lv6EGate_e,      -1),
    OBJNAME("l6eleva", fpcNm_Obj_Lv6ElevtA_e,     -1),
    OBJNAME("wtGate",  fpcNm_Obj_WtGate_e,        -1),
    OBJNAME("candlL2", fpcNm_Obj_Lv2Candle_e,     -1),
    OBJNAME("togeTp",  fpcNm_Obj_TogeTrap_e,      -1),
    OBJNAME("rotTrap", fpcNm_Obj_RotTrap_e,       -1),
    OBJNAME("klift00", fpcNm_Obj_KLift00_e,       -1),
    OBJNAME("l4chand", fpcNm_Obj_Lv4Chan_e,       -1),
    OBJNAME("Ychndlr", fpcNm_Obj_Ychndlr_e,       -1),
    OBJNAME("hbmbkoy", fpcNm_Obj_HBombkoya_e,     -1),
    OBJNAME("TagCsw",  fpcNm_TAG_CSW_e,           -1),
    OBJNAME("TagCswO", fpcNm_TAG_CSW_e,           -1),
    OBJNAME("P_Rwall", fpcNm_Obj_Lv4PRwall_e,     -1),
    OBJNAME("PDtile",  fpcNm_Obj_PDtile_e,        -1),
    OBJNAME("PDwall",  fpcNm_Obj_PDwall_e,        -1),
    OBJNAME("Qs",      fpcNm_TAG_QS_e,            -1),
    OBJNAME("mirror",  fpcNm_MIRROR_e,            -1),
    OBJNAME("rGate",   fpcNm_Obj_SwallShutter_e,  -1),
    OBJNAME("l3water", fpcNm_Obj_Lv3Water_e,      -1),
    OBJNAME("l3wat02", fpcNm_Obj_Lv3Water2_e,     -1),
    OBJNAME("l3watB",  fpcNm_OBJ_LV3WATERB_e,     -1),
    OBJNAME("szbridg", fpcNm_Obj_SZbridge_e,      -1),
    OBJNAME("kjgjs",   fpcNm_Obj_KJgjs_e,         -1),
    OBJNAME("kjs",     fpcNm_Obj_KJgjs_e,         -1),
    OBJNAME("candlL3", fpcNm_Obj_Lv3Candle_e,     -1),
    OBJNAME("l5icewl", fpcNm_Obj_IceWall_e,       -1),
    OBJNAME("Turara",  fpcNm_Obj_Turara_e,        -1),
    OBJNAME("twGate",  fpcNm_Obj_TwGate_e,        -1),
    OBJNAME("L4cdlTg", fpcNm_Tag_Lv4Candle_e,     -1),
    OBJNAME("L4cddTg", fpcNm_Tag_Lv4CandleDm_e,   -1),
    OBJNAME("togeRol", fpcNm_Obj_Lv6TogeRoll_e,   -1),
    OBJNAME("l6TogeT", fpcNm_Obj_Lv6TogeTrap_e,   -1),
    OBJNAME("Tenbin",  fpcNm_Obj_Lv6Tenbin_e,     -1),
    OBJNAME("l6Lblk",  fpcNm_Obj_Lv6Lblock_e,     -1),
    OBJNAME("l6SwGt",  fpcNm_Obj_Lv6SwGate_e,     -1),
    OBJNAME("l6ChBlk", fpcNm_Obj_Lv6ChgGate_e,    -1),
    OBJNAME("l6FuriT", fpcNm_Obj_Lv6FuriTrap_e,   -1),
    OBJNAME("KHdesk",  fpcNm_Obj_BarDesk_e,       -1),
    OBJNAME("Ytaihou", fpcNm_Obj_Ytaihou_e,       -1),
    OBJNAME("L4eShut", fpcNm_Obj_Lv4EdShutter_e,  -1),
    OBJNAME("pofire",  fpcNm_Obj_poFire_e,        -1),
    OBJNAME("pofiTg",  fpcNm_Tag_poFire_e,        -1),
    OBJNAME("poCandl", fpcNm_Obj_poCandle_e,      -1),
    OBJNAME("L4Gate",  fpcNm_Obj_Lv4Gate_e,       -1),
    OBJNAME("L4Pgate", fpcNm_Obj_Lv4PoGate_e,     -1),
    OBJNAME("L4SWall", fpcNm_Obj_Lv4SlideWall_e,  -1),
    OBJNAME("L4hmato", fpcNm_Obj_Lv4HsTarget_e,   -1),
    OBJNAME("propy",   fpcNm_Obj_Lv7PropY_e,      -1),
    OBJNAME("L7BsGt",  fpcNm_Obj_Lv7BsGate_e,     -1),
    OBJNAME("optLift", fpcNm_Obj_Lv8OptiLift_e,   -1),
    OBJNAME("kkiTrap", fpcNm_Obj_Lv8KekkaiTrap_e, -1),
    OBJNAME("L8LiftX", fpcNm_Obj_Lv8Lift_e,       -1),
    OBJNAME("swStep",  fpcNm_Obj_Lv8UdFloor_e,    -1),
    OBJNAME("L9SwSht", fpcNm_Obj_Lv9SwShutter_e,  -1),
    OBJNAME("L5SwIce", fpcNm_Obj_Lv5SwIce_e,      -1),
    OBJNAME("glwSph",  fpcNm_Obj_glowSphere_e,    -1),
    OBJNAME("MR_Scrw", fpcNm_Obj_MirrorScrew_e,   -1),
    OBJNAME("MR_Sand", fpcNm_Obj_MirrorSand_e,    -1),
    OBJNAME("MR_Tble", fpcNm_Obj_MirrorTable_e,   -1),
    OBJNAME("MR_Chin", fpcNm_Obj_MirrorChain_e,   -1),
    OBJNAME("MR_Pole", fpcNm_Obj_Mirror6Pole_e,   -1),
    OBJNAME("PPolamp", fpcNm_PPolamp_e,           -1),
    OBJNAME("l5hYuka", fpcNm_Obj_Lv5FBoard_e,     -1),
    OBJNAME("BYRock",  fpcNm_BkyRock_e,           -1),
    OBJNAME("zrDrock", fpcNm_Obj_zrTurara_e,      -1),
    OBJNAME("tkrDai",  fpcNm_Obj_TakaraDai_e,     -1),
    OBJNAME("Table",   fpcNm_Obj_Table_e,         -1),
    OBJNAME("CatDoor", fpcNm_Obj_CatDoor_e,       -1),
    OBJNAME("RetTag",  fpcNm_Tag_RetRoom_e,       -1),
    OBJNAME("WdStone", fpcNm_Obj_WindStone_e,     -1),
    OBJNAME("HwlWara", fpcNm_Tag_WaraHowl_e,      -1),
    OBJNAME("SCannon", fpcNm_Obj_SCannon_e,       -1),
    OBJNAME("szGate",  fpcNm_Obj_Lv6SzGate_e,     -1),
    OBJNAME("onsTaru", fpcNm_Obj_OnsenTaru_e,     -1),
    OBJNAME("WStoneF", fpcNm_Obj_SmWStone_e,      -1),
    OBJNAME("SCanCrs", fpcNm_Obj_SCannonCrs_e,    -1),
    OBJNAME("snwEfTg", fpcNm_Tag_SnowEff_e,       -1),
    OBJNAME("tmFire",  fpcNm_Obj_TimeFire_e,      -1),
    OBJNAME("L7Prop",  fpcNm_Obj_Prop_e,          -1),
    OBJNAME("awaPlar", fpcNm_Obj_awaPlar_e,       -1),
    OBJNAME("poTbox",  fpcNm_Obj_poTbox_e,        -1),
    OBJNAME("SnwSoup", fpcNm_Obj_SnowSoup_e,      -1),
    OBJNAME("Nagaisu", fpcNm_Obj_Nagaisu_e,       -1),
    OBJNAME("RCircle", fpcNm_Obj_RCircle_e,       -1),
    OBJNAME("L9Chand", fpcNm_Obj_Chandelier_e,    -1),
    OBJNAME("L9Pictr", fpcNm_Obj_Picture_e,       -1),
    OBJNAME("SmkEmt",  fpcNm_Tag_SmkEmt_e,        -1),
    OBJNAME("HFtr",    fpcNm_Obj_HFtr_e,          -1),
    OBJNAME("HBarrel", fpcNm_Obj_HBarrel_e,       -1),
    OBJNAME("SCanTen", fpcNm_Obj_SCannonTen_e,    -1),
    OBJNAME("cstaSw",  fpcNm_Tag_CstaSw_e,        -1),
    OBJNAME("l6cstSw", fpcNm_Tag_Lv6CstaSw_e,     -1),
    OBJNAME("Hata",    fpcNm_Obj_Hata_e,          -1),
    OBJNAME("RmbitSw", fpcNm_Tag_RmbitSw_e,       -1),
    OBJNAME("T_Maki",  fpcNm_Obj_ToaruMaki_e,     -1),
    OBJNAME("d_act",   fpcNm_DEMO00_e,            -1),
    OBJNAME("d_act0",  fpcNm_DEMO00_e,            0x00),
    OBJNAME("d_act1",  fpcNm_DEMO00_e,            0x01),
    OBJNAME("d_act2",  fpcNm_DEMO00_e,            0x02),
    OBJNAME("d_act3",  fpcNm_DEMO00_e,            0x03),
    OBJNAME("d_act4",  fpcNm_DEMO00_e,            0x04),
    OBJNAME("d_act5",  fpcNm_DEMO00_e,            0x05),
    OBJNAME("d_act6",  fpcNm_DEMO00_e,            0x06),
    OBJNAME("d_act7",  fpcNm_DEMO00_e,            0x07),
    OBJNAME("d_act8",  fpcNm_DEMO00_e,            0x08),
    OBJNAME("d_act9",  fpcNm_DEMO00_e,            0x09),
    OBJNAME("d_act10", fpcNm_DEMO00_e,            0x0A),
    OBJNAME("d_act11", fpcNm_DEMO00_e,            0x0B),
    OBJNAME("d_act12", fpcNm_DEMO00_e,            0x0C),
    OBJNAME("d_act13", fpcNm_DEMO00_e,            0x0D),
    OBJNAME("d_act14", fpcNm_DEMO00_e,            0x0E),
    OBJNAME("d_act15", fpcNm_DEMO00_e,            0x0F),
    OBJNAME("d_act16", fpcNm_DEMO00_e,            0x10),
    OBJNAME("d_act17", fpcNm_DEMO00_e,            0x11),
    OBJNAME("d_act18", fpcNm_DEMO00_e,            0x12),
    OBJNAME("d_act19", fpcNm_DEMO00_e,            0x13),
    OBJNAME("d_act20", fpcNm_DEMO00_e,            0x14),
    OBJNAME("d_act21", fpcNm_DEMO00_e,            0x15),
    OBJNAME("d_act22", fpcNm_DEMO00_e,            0x16),
    OBJNAME("d_act23", fpcNm_DEMO00_e,            0x17),
    OBJNAME("d_act24", fpcNm_DEMO00_e,            0x18),
    OBJNAME("d_act25", fpcNm_DEMO00_e,            0x19),
    OBJNAME("d_act26", fpcNm_DEMO00_e,            0x1A),
    OBJNAME("d_act27", fpcNm_DEMO00_e,            0x1B),
    OBJNAME("d_act28", fpcNm_DEMO00_e,            0x1C),
    OBJNAME("d_act29", fpcNm_DEMO00_e,            0x1D),
    OBJNAME("d_act30", fpcNm_DEMO00_e,            0x1E),
    OBJNAME("d_act31", fpcNm_DEMO00_e,            0x1F),
    OBJNAME("TLogo",   fpcNm_TITLE_e,             -1),
#if DEBUG
    OBJNAME("unitC00", fpcNm_Obj_TestCube_e,      -1),
    OBJNAME("unitS00", fpcNm_Obj_TestCube_e,      -1),
    OBJNAME("unitCy0", fpcNm_Obj_TestCube_e,      -1),
    OBJNAME("damCps",  fpcNm_Obj_DamCps_e,        -1),
    OBJNAME("railDrw", fpcNm_PATH_LINE_e,         -1),
    OBJNAME("sdoor",   fpcNm_SPIRAL_DOOR_e,       -1),
#endif
    OBJNAME("ClearB",  fpcNm_Obj_Cboard_e,        -1),
    OBJNAME("Bg",      fpcNm_BG_e,                -1),
    OBJNAME("Boom",    fpcNm_BOOMERANG_e,         -1),
    OBJNAME("Nbomb",   fpcNm_NBOMB_e,             -1),
    OBJNAME("Arrow",   fpcNm_ARROW_e,             -1),
    OBJNAME("E_Arrow", fpcNm_E_ARROW_e,           -1),
    OBJNAME("Spinner", fpcNm_SPINNER_e,           -1),
    OBJNAME("Crod",    fpcNm_CROD_e,              -1),
    OBJNAME("Midna",   fpcNm_MIDNA_e,             -1),
    OBJNAME("Vrbox",   fpcNm_VRBOX_e,             -1),
    OBJNAME("Vrbox2",  fpcNm_VRBOX2_e,            -1),
    OBJNAME("mvbg_a",  fpcNm_BG_OBJ_e,            -1),
    OBJNAME("burnBox", fpcNm_Obj_BurnBox_e,       -1),
    OBJNAME("stnMark", fpcNm_Obj_StoneMark_e,     -1),
#if PLATFORM_GCN
    OBJNAME("Passer",  fpcNm_NPC_PASSER_e,        -1),
#endif
    OBJNAME("Passer2", fpcNm_NPC_PASSER2_e,       -1),
    OBJNAME("ShopItm", fpcNm_ShopItem_e,          -1),
    OBJNAME("E_yd_lf", fpcNm_E_YD_LEAF_e,         -1),
    OBJNAME("E_db_lf", fpcNm_E_DB_LEAF_e,         -1),
    OBJNAME("E_hb_lf", fpcNm_E_HB_LEAF_e,         -1),
    OBJNAME("E_bi_lf", fpcNm_E_BI_LEAF_e,         -1),
    OBJNAME("O_Mato",  fpcNm_Obj_Mato_e,          -1),
    OBJNAME("O_Flag",  fpcNm_Obj_Flag_e,          -1),
    OBJNAME("O_Flag2", fpcNm_Obj_Flag2_e,         -1),
    OBJNAME("O_Flag3", fpcNm_Obj_Flag3_e,         -1),
    OBJNAME("CRVLH_U", fpcNm_Obj_CRVLH_UP_e,      -1),
    OBJNAME("DemoItm", fpcNm_Demo_Item_e,         -1),
    OBJNAME("EndCode", fpcNm_PLAY_SCENE_e,        -1),
};
// clang-format on

void dStage_stageDt_c::init() {
    mCamera = NULL;
    mArrow = NULL;
    mPlayer = NULL;
    mRoom = NULL;
    mMapInfo = NULL;
    mMapInfoBase = NULL;
    mPaletteInfo = NULL;
    mPselectInfo = NULL;
    mEnvrInfo = NULL;
    mVrboxInfo = NULL;
    mVrboxcolInfo = NULL;
    mPlightInfo = NULL;
    mPlightNumInfo = 0;
    mMapEventInfo = NULL;
    mStagInfo = NULL;
    mSclsInfo = NULL;
    mPntInfo = NULL;
    mPathInfo = NULL;
    mPnt2Info = NULL;
    mPath2Info = NULL;
    mSoundInf = NULL;
    mSoundInfCL = NULL;
    mFloorInfo = NULL;
    mMemoryConfig = NULL;
    mMemoryMap = NULL;
    mMulti = NULL;
    mTresure = NULL;
    mDMap = NULL;
    mDrTg = NULL;
    mDoor = NULL;
    mElst = NULL;
    mWorldRollAngleX = 0;
    mWorldRollDirAngleY = 0;
#if DEBUG
    mPaletteNumInfo = 0;
    mPselectNumInfo = 0;
    mVrboxNumInfo = 0;
    mPlightNumInfo = 0;
#endif
}

void dStage_roomDt_c::initFileList2() {
    mFileList2Info = NULL;
}

void dStage_roomDt_c::init() {
    mLightVecInfo = NULL;
    mLightVecInfoNum = 0;
    mMapInfo = NULL;
    mMapInfoBase = NULL;
    mVrboxInfo = NULL;
    mVrboxcolInfo = NULL;
    mFileListInfo = NULL;
    mPlayer = NULL;
    mPnt2Info = NULL;
    mPath2Info = NULL;
    mCamera = NULL;
    mArrow = NULL;
    mMapEventInfo = NULL;
    mSoundInf = NULL;
    mSoundInfCL = NULL;
    mSclsInfo = NULL;
    mLbnk = NULL;
    mTresure = NULL;
    mDrTg = NULL;
    mDoor = NULL;
    mFloorInfo = NULL;
#if DEBUG
    mUnit = NULL;
#endif
}

static int dStage_roomInit(int i_roomNo) {
    dComIfGp_roomControl_setStayNo(i_roomNo);

    roomRead_class* room = dComIfGp_getStageRoom();
    if (room != NULL && room->num > i_roomNo) {
        dComIfGp_roomControl_setTimePass(dStage_roomRead_dt_c_GetTimePass(*room->m_entries[i_roomNo]));

        return dComIfGp_roomControl_loadRoom(room->m_entries[i_roomNo]->num,
                                             room->m_entries[i_roomNo]->m_rooms, true);
    }

    return 1;
}

static void dummy0() {
    dComIfGp_roomControl_setTimePass(0);
    ((dStage_stageDt_c*)dComIfGp_getStage())->getRoom();
}

dStage_objectNameInf* dStage_searchName(char const* objName) {
    dStage_objectNameInf* obj = l_objectName;

    for (u32 i = 0; i < ARRAY_SIZEU(l_objectName); i++) {
        if (!strcmp(obj->name, objName)) {
            return obj;
        }
        obj++;
    }

    OS_REPORT("オブジェクト名無し！！<%s>\n", objName);
    return NULL;
}

const char* dStage_getName(s16 procName, s8 argument) {
    static char tmp_name[dStage_NAME_LENGTH];

    dStage_objectNameInf* obj = l_objectName;
    char* tmp = NULL;

    for (int i = 0; i < ARRAY_SIZEU(l_objectName); i++) {
        if (obj->procname == procName) {
            if (obj->argument == argument) {
                return obj->name;
            }
            if (tmp == NULL) {
                tmp = obj->name;
            }
        }
        obj++;
    }

    if (tmp == NULL) {
        snprintf(tmp_name, dStage_NAME_LENGTH, "%d%+0d", procName, argument);
        tmp = tmp_name;
    }
    return tmp;
}

const char* dStage_getName2(s16 procName, s8 argument) {
    return dStage_getName(procName, argument);
}

u8 data_8074C568_debug;
u8 data_8074C569_debug;
u8 data_8074C56A_debug;
u8 data_8074C56B_debug;
u8 data_8074C56C_debug;

fpc_ProcID dStage_roomControl_c::mProcID;

s8 dStage_roomControl_c::mStayNo;

s8 dStage_roomControl_c::mOldStayNo;

s8 dStage_roomControl_c::mNextStayNo;

u8 dStage_roomControl_c::m_time_pass;

u8 dStage_roomControl_c::mNoChangeRoom;

dStage_roomControl_c::dStage_bankName* dStage_roomControl_c::mArcBankName;

dStage_roomControl_c::dStage_bankData* dStage_roomControl_c::mArcBankData;

dStage_roomControl_c::roomDzs_c dStage_roomControl_c::m_roomDzs;
#if DEBUG
u8 dStage_roomControl_c::mNoArcBank;
#endif

static void dStage_actorCreate(stage_actor_data_class* i_actorData, fopAcM_prm_class* i_actorPrm) {
    dStage_objectNameInf* actorInf = dStage_searchName(i_actorData->name);

    if (actorInf == NULL) {
        OS_REPORT("\x1B""[43;30mStage Actor Name Nothing !! <%s>\n\x1B[m", i_actorData->name);
        JKRFree(i_actorPrm);
    } else {
        i_actorPrm->argument = actorInf->argument;
        if (actorInf->procname == fpcNm_SUSPEND_e) {
            fopAc_ac_c* actor = (fopAc_ac_c*)fopAcM_FastCreate(actorInf->procname, NULL, NULL, i_actorPrm);

            if (actor != NULL) {
                fopAcM_delete(actor);
            }
            return;
        }

        fopAcM_Create(actorInf->procname, NULL, i_actorPrm);
    }
}

static int dStage_cameraCreate(stage_camera2_data_class* i_cameraData, int i_cameraIdx,
                               int param_2) {
    s16 procname = fpcNm_CAMERA_e;
    if (procname < 0) {
        return 0;
    }
    fopCamM_prm_class* params = (fopCamM_prm_class*)cMl::memalignB(-4, sizeof(fopCamM_prm_class));

    if (params != NULL) {
        params->base.position.x = 0.0f;
        params->base.position.y = 0.0f;
        params->base.position.x = 0.0f;
        params->base.parameters = param_2;
        fopCamM_Create(i_cameraIdx, procname, params);
    }

    return 1;
}

static void dummy2() {
    // Needed to fix weak function order.
    // This is likely caused by the dStage_chkPlayerId function from TWW using these functions.
    // dStage_chkPlayerId isn't used in TP, so it gets stripped out, but the effect it has on weak order remains.
    ((dStage_stageDt_c*)dComIfGp_getStage())->getPlayer();
    dComIfGp_roomControl_getStatusRoomDt(0)->getPlayer();
}

static int dStage_playerInit(dStage_dt_c* i_stage, void* i_data, int num, void* param_3) {
    UNUSED(param_3);
    stage_actor_class* player = (stage_actor_class*)((int*)i_data + 1);
    stage_actor_data_class* player_data = player->m_entries;
    i_stage->setPlayer(player);
    i_stage->setPlayerNum(num);

    if (dComIfGp_getPlayer(0) != NULL || dComIfGp_getStartStageRoomNo() != i_stage->getRoomNo()) {
        return 1;
    }

    fopAcM_prm_class* appen = fopAcM_CreateAppend();
    JUT_ASSERT(1586, appen != NULL);

    int point = dComIfGp_getStartStagePoint();
    u32 roomParam = dComIfGs_getRestartRoomParam();
    if (point == -2 || point == -3) {
        appen->base.parameters = dComIfGs_getTurnRestartParam();
        appen->base.position = dComIfGs_getTurnRestartPos();
        appen->base.angle.set(0, dComIfGs_getTurnRestartAngleY(), -0x100);
    } else if (point == -1) {
        appen->base.parameters = roomParam;
        appen->base.position = dComIfGs_getRestartRoomPos();
        appen->base.angle.set(0, dComIfGs_getRestartRoomAngleY(), -0x100);
    } else {
        int unk;
        if (point == -4) {
            unk = dComIfGs_getTurnRestartAngleY();
        } else {
            unk = point;
        }

        int i;
        for (i = 0; i < num; i++) {
            if ((u8)player_data->base.angle.z == unk) {
                break;
            }
            player_data++;
        }
        if (i == num) {
            OS_REPORT_ERROR("プレイヤーが発見できません。[No.%d]\n切り替えの情報や処理の確認をお願いします。\n", point);
        }
        JUT_ASSERT(1636, i != num);

        appen->base = player_data->base;

        if (point == -4) {
            appen->base.parameters = dComIfGs_getTurnRestartParam();
        } else if (roomParam != 0) {
            appen->base.parameters = (roomParam & 0xFFFFFFC0) | (dComIfGp_getStartStageRoomNo() & 0x3F);
        } else if (roomParam == 0) {
            appen->base.parameters =
                (appen->base.parameters & 0xFFFFFFC0) | (dComIfGp_getStartStageRoomNo() & 0x3F);
        }
    }

    dComIfGs_setRestartRoomParam(0);
    appen->base.setID = 0xFFFF;
    appen->room_no = -1;

    dComIfGp_getStartStage()->set(dComIfGp_getStartStageName(), appen->base.parameters & 0x3F,
                                  dComIfGp_getStartStagePoint(), dComIfGp_getStartStageLayer());
    dStage_actorCreate(player_data, appen);

    base_process_class* stageProc =
        (base_process_class*)fopScnM_SearchByID(dStage_roomControl_c::getProcID());
    JUT_ASSERT(1683, stageProc != NULL);
    if (fpcM_GetName(stageProc) == fpcNm_PLAY_SCENE_e) {
        if (strcmp(dComIfGp_getStartStageName(), "S_MV000")) {
            fopMsgM_Create(fpcNm_METER2_e, NULL, NULL);
        }
    }

    return 1;
}

static int dStage_cameraInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    UNUSED(param_2);
    UNUSED(param_3);
    stage_camera_class* camera = (stage_camera_class*)((char*)i_data + 4);
    int r30 = 0;
    i_stage->setCamera(camera);
    stage_camera2_data_class* camera2 = &camera->m_entries[r30];
    dStage_cameraCreate(camera2, 0, 0);
    return 1;
}

static int dStage_RoomCameraInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    UNUSED(param_2);
    UNUSED(param_3);
    stage_camera_class* camera = (stage_camera_class*)((char*)i_data + 4);
    i_stage->setCamera(camera);
    return 1;
}

static int dStage_arrowInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    UNUSED(param_2);
    UNUSED(param_3);
    stage_arrow_class* arrow = (stage_arrow_class*)((char*)i_data + 4);
    i_stage->setArrow(arrow);
    return 1;
}

stage_map_info_class* dStage_roomDt_c::getMapInfo2(int param_0) const {
    stage_map_info_dummy_class* map_info_p = NULL;
    map_info_p = getMapInfoBase();

    if (map_info_p == NULL || map_info_p->num == 0 || map_info_p->m_entries == NULL) {
        return NULL;
    }

    stage_map_info_class* data_p = map_info_p->m_entries;

    for (int i = 0; i < map_info_p->num; i++) {
        if (param_0 == data_p->field_0x35) {
            return data_p;
        }

        data_p++;
    }

    return NULL;
}

static void dummy3() {
    dComIfGp_roomControl_getStatusRoomDt(0)->getMapInfoBase();
}

stage_map_info_class* dStage_stageDt_c::getMapInfo2(int param_0) const {
    stage_map_info_dummy_class* map_info_p = NULL;
    map_info_p = getMapInfoBase();

    if (map_info_p == NULL || map_info_p->num == 0 || map_info_p->m_entries == NULL) {
        return NULL;
    }

    stage_map_info_class* data_p = map_info_p->m_entries;

    for (int i = 0; i < map_info_p->num; i++) {
        if (param_0 == data_p->field_0x35) {
            return data_p;
        }

        data_p++;
    }

    return NULL;
}

static void dummy4() {
    ((dStage_stageDt_c*)dComIfGp_getStage())->getMapInfoBase();
}

static int dStage_paletteInfoInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    UNUSED(param_3);
    dStage_nodeHeader* pal_info = (dStage_nodeHeader*)(i_data);
    i_stage->setPaletteInfo((stage_palette_info_class*)pal_info->m_offset);
#if DEBUG
    i_stage->setPaletteNumInfo(param_2);
#endif
    return 1;
}

static int dStage_pselectInfoInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    UNUSED(param_3);
    dStage_nodeHeader* psel_info = (dStage_nodeHeader*)(i_data);
    i_stage->setPselectInfo((stage_pselect_info_class*)psel_info->m_offset);
#if DEBUG
    i_stage->setPselectNumInfo(param_2);
#endif
    return 1;
}

static int dStage_envrInfoInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    UNUSED(param_3);
    dStage_nodeHeader* envr_info = (dStage_nodeHeader*)(i_data);
    i_stage->setEnvrInfo((stage_envr_info_class*)envr_info->m_offset);
#if DEBUG
    i_stage->setEnvrNumInfo(param_2);
#endif
    return 1;
}

static int dStage_filiInfo2Init(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    if (entryNum == 0) {
        return 1;
    }

    dStage_FileList2_c* fili_header = (dStage_FileList2_c*)((char*)i_data + 4);
    dStage_roomControl_c::setFileList2(i_stage->getRoomNo(), fili_header->entries);
    return 1;
}

static int dStage_fieldMapFiliInfo2Init(dStage_dt_c* i_stage, void* i_data, int i_entryNum,
                                        void* param_3) {
    if (i_entryNum == 0) {
        return 1;
    }

    dMenu_Fmap_data_c* data = (dMenu_Fmap_data_c*)i_stage;
    dStage_FileList2_c* fili_header = (dStage_FileList2_c*)((char*)i_data + 4);
    data->setFileList2(fili_header->entries);
    return 1;
}

static int dStage_filiInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    dStage_nodeHeader* fili_info = (dStage_nodeHeader*)(i_data);
    if (entryNum == 0) {
        i_stage->setFileListInfo(NULL);
    } else {
        i_stage->setFileListInfo((dStage_FileList_dt_c*)fili_info->m_offset);
    }

    return 1;
}

static int dStage_vrboxInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(param_3);
    dStage_nodeHeader* vrbox_info = (dStage_nodeHeader*)(i_data);
    i_stage->setVrboxInfo((stage_vrbox_info_class*)vrbox_info->m_offset);
#if DEBUG
    i_stage->setVrboxNumInfo(entryNum);
#endif
    return 1;
}

static int dStage_vrboxcolInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                   void* param_3) {
    UNUSED(param_3);
    dStage_nodeHeader* vrcol_info = (dStage_nodeHeader*)(i_data);
    i_stage->setVrboxcolInfo((stage_vrboxcol_info_class*)vrcol_info->m_offset);
#if DEBUG
    i_stage->setVrboxcolNumInfo(entryNum);
#endif
    return 1;
}

static int dStage_plightInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    dStage_nodeHeader* plight_info = (dStage_nodeHeader*)(i_data);
    i_stage->setPlightInfo((stage_plight_info_class*)plight_info->m_offset);
    i_stage->setPlightNumInfo(entryNum);
    return 1;
}

static int dStage_lgtvInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    i_stage->setLightVecInfoNum(entryNum);

    if (entryNum == 0) {
        i_stage->setLightVecInfo(NULL);
    } else {
        dStage_nodeHeader* lgtv_info = (dStage_nodeHeader*)(i_data);
        i_stage->setLightVecInfo((stage_pure_lightvec_info_class*)lgtv_info->m_offset);
    }

    return 1;
}

u32 dStage_stagInfo_GetParticleNo(stage_stag_info_class* p_info, int layer) {
    JUT_ASSERT(2220, 0 <= layer && layer < 15);
    return p_info->mParticleNo[layer];
}

static int dStage_stagInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    dStage_nodeHeader* stag_info = (dStage_nodeHeader*)(i_data);
    i_stage->setStagInfo((stage_stag_info_class*)stag_info->m_offset);

    if (!dStage_isBossStage(i_stage)) {
        dComIfG_deleteStageRes("Xtg_00");
        dComIfGp_resetOldMulti();
    }

    int stageNo = dStage_stagInfo_GetSaveTbl(i_stage->getStagInfo());
    dComIfGs_getSave(stageNo);
    g_save_bit_HIO.init();
#if DEBUG
    OS_REPORT("Save StageNo=%d sizeof(dSv_save_c)=%d\n", stageNo, sizeof(dSv_save_c));
    char* r30 = &dComIfGs_getSaveInfo()->unk_0x1;
    if (dComIfGs_getSaveInfo()->unk_0x0 && *r30 && !strcmp(r30, dComIfGp_getStartStageName())) {
        OS_REPORT("stage: keep dungeon save data\n");
    } else {
        OS_REPORT("stage: clear dungeon save data\n");
    }
#endif
    dComIfGs_initDan(stageNo);
#if DEBUG
    dComIfGs_getSaveInfo()->unk_0x0 = 0;
#endif
    dStage_KeepDoorInfoInit(i_stage);
    return 1;
}

static int dStage_sclsInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    i_stage->setSclsInfo((stage_scls_info_dummy_class*)((char*)i_data + 4));
    return 1;
}

static int dStage_actorCommonLayerInit(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                       void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    stage_actor_class* actor = (stage_actor_class*)((int*)i_data + 1);
    stage_actor_data_class* actor_data = actor->m_entries;

    for (int i = 0; i < actor->num; i++) {
        if (!dComIfGs_isActor(actor_data->base.setID, i_stage->getRoomNo())) {
            if (!daSus_c::check(i_stage->getRoomNo(), actor_data->base.position)) {
                fopAcM_prm_class* appen = fopAcM_CreateAppend();

                if (appen != NULL) {
                    appen->base = actor_data->base;
                    appen->room_no = (int)i_stage->getRoomNo();
                    dStage_actorCreate(actor_data, appen);
                }
            }
        }
        actor_data++;
    }

    return 1;
}

static int dStage_tgscCommonLayerInit(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                      void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    stage_tgsc_class* actor = (stage_tgsc_class*)((int*)i_data + 1);
    stage_tgsc_data_class* tgsc_data = actor->m_entries;

    for (int i = 0; i < actor->num; i++) {
        stage_actor_data_class* actor_data = (stage_actor_data_class*)tgsc_data;
        if (!daSus_c::check(i_stage->getRoomNo(), actor_data->base.position)) {
            fopAcM_prm_class* appen = fopAcM_CreateAppend();

            if (appen != NULL) {
                appen->base = tgsc_data->base;
                appen->room_no = (int)i_stage->getRoomNo();
                appen->scale = tgsc_data->scale;
                dStage_actorCreate(actor_data, appen);
            }
        }
        tgsc_data++;
    }

    return 1;
}

static int dStage_actorInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    stage_actor_class* actor = (stage_actor_class*)((int*)i_data + 1);
    stage_actor_data_class* actor_data = actor->m_entries;

    for (int i = 0; i < actor->num; i++) {
        if (!dComIfGs_isActor(actor_data->base.setID, i_stage->getRoomNo())) {
            if (!daSus_c::check(i_stage->getRoomNo(), actor_data->base.position)) {
                fopAcM_prm_class* appen = fopAcM_CreateAppend();

                if (appen != NULL) {
                    appen->base = actor_data->base;
                    appen->room_no = (int)i_stage->getRoomNo();
                    dStage_actorCreate(actor_data, appen);
                }
            }
        }
        actor_data++;
    }

    return 1;
}

static int dStage_actorInit_always(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                   void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    stage_actor_class* actor = (stage_actor_class*)((int*)i_data + 1);
    stage_actor_data_class* actor_data = actor->m_entries;

    for (int i = 0; i < actor->num; i++) {
        if (!dComIfGs_isActor(actor_data->base.setID, i_stage->getRoomNo())) {
            fopAcM_prm_class* appen = fopAcM_CreateAppend();

            if (appen != NULL) {
                appen->base = actor_data->base;
                appen->room_no = (int)i_stage->getRoomNo();
                dStage_actorCreate(actor_data, appen);
            }
        }
        actor_data++;
    }

    return 1;
}

static int dStage_tgscInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    stage_tgsc_class* actor = (stage_tgsc_class*)((int*)i_data + 1);
    stage_tgsc_data_class* tgsc_data = actor->m_entries;

    for (int i = 0; i < actor->num; i++) {
        stage_actor_data_class* actor_data = (stage_actor_data_class*)tgsc_data;
        if (!daSus_c::check(i_stage->getRoomNo(), actor_data->base.position)) {
            fopAcM_prm_class* appen = fopAcM_CreateAppend();

            if (appen != NULL) {
                appen->base = actor_data->base;
                appen->room_no = (int)i_stage->getRoomNo();
                appen->scale = tgsc_data->scale;
                dStage_actorCreate(actor_data, appen);
            }
        }
        tgsc_data++;
    }

    return 1;
}

static int dStage_doorInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    stage_tgsc_class* actor = (stage_tgsc_class*)((int*)i_data + 1);
    stage_tgsc_data_class* tgsc_data = actor->m_entries;

    for (int i = 0; i < actor->num; i++) {
        stage_actor_data_class* actor_data = (stage_actor_data_class*)tgsc_data;
        fopAcM_prm_class* appen = fopAcM_CreateAppend();

        if (appen != NULL) {
            appen->base = actor_data->base;
            appen->room_no = (int)i_stage->getRoomNo();
            appen->scale = tgsc_data->scale;
            dStage_actorCreate(actor_data, appen);
        }
        tgsc_data++;
    }

    return 1;
}

static int dStage_roomReadInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    UNUSED(param_2);
    roomRead_class* p_node = (roomRead_class*)((int*)i_data + 1);
    OFFSET_PTR(roomRead_data_class)* rtbl = p_node->m_entries;

    i_stage->setRoom(p_node);

    for (int i = 0; i < p_node->num; i++) {
#if TARGET_PC
        rtbl[i].setBase(param_3);
        rtbl[i]->m_rooms.setBase(param_3);
#else
        if ((intptr_t)rtbl[i] < 0x80000000) {
            rtbl[i] = (roomRead_data_class*)((intptr_t)rtbl[i] + (intptr_t)param_3);
            rtbl[i]->m_rooms = (u8*)((intptr_t)rtbl[i]->m_rooms + (intptr_t)param_3);
        }
#endif
    }

    return 1;
}

s8 dStage_roomRead_dt_c_GetReverbStage(roomRead_class& room, int index) {
    if (index < 0 || index >= room.num) {
        index = 0;
    }

    roomRead_data_class* data = room.m_entries[index];
    return dStage_roomRead_dt_c_GetReverb(*data);
}

static int dStage_ppntInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    dStage_dPnt_c* pnt = (dStage_dPnt_c*)((char*)i_data + 4);
    i_stage->setPntInfo(pnt);
    return 1;
}

static int dStage_pathInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    dStage_dPath_c* path_c = (dStage_dPath_c*)((char*)i_data + 4);
    dPath* path = path_c->m_path;

    i_stage->setPathInfo(path_c);

    for (int i = 0; i < path_c->m_num; i++) {
#if TARGET_PC
        path->m_points.setBase(i_stage->getPntInf()->m_pnt_offset);
#else
        if ((uintptr_t)path->m_points < 0x80000000) {
            path->m_points = (dPnt*)((uintptr_t)path->m_points + i_stage->getPntInf()->m_pnt_offset);
        }
#endif
        path++;
    }

    return 1;
}

static int dStage_rppnInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    dStage_dPnt_c* pnt = (dStage_dPnt_c*)((char*)i_data + 4);
    i_stage->setPnt2Info(pnt);
    return 1;
}

static int dStage_rpatInfoInit(dStage_dt_c* i_stage, void* i_data, int i_num, void* param_3) {
    UNUSED(i_num);
    UNUSED(param_3);
    dStage_dPath_c* pStagePath = (dStage_dPath_c*)((char*)i_data + 4);
    dPath* pPath = pStagePath->m_path;

    i_stage->setPath2Info(pStagePath);
    for (s32 i = 0; i < pStagePath->m_num; pPath++, i++, (void)0) {
#if TARGET_PC
        pPath->m_points.setBase(i_stage->getPnt2Inf()->m_pnt_offset);
#else
        if ((uintptr_t)pPath->m_points >= 0x80000000) {
            continue;
        }
        pPath->m_points = (dPnt*)((uintptr_t)pPath->m_points + i_stage->getPnt2Inf()->m_pnt_offset);
#endif
    }
    return 1;
}

static int dStage_soundInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    dStage_SoundInfo_c* soundInfo = (dStage_SoundInfo_c*)((char*)i_data + 4);
    i_stage->setSoundInf(soundInfo);
    return 1;
}

static int dStage_soundInfoInitCL(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    dStage_SoundInfo_c* soundInfo = (dStage_SoundInfo_c*)((char*)i_data + 4);
    i_stage->setSoundInfCL(soundInfo);
    return 1;
}

static void dStage_setLayerTagName(FuncTable* funcTbl, int tblSize, int i_layerNo) {
    JUT_ASSERT(3007, 0 <= i_layerNo && i_layerNo < 15);
    char layerTag = i_layerNo + (i_layerNo < 10 ? '0' : 'W');

    for (int i = 0; i < tblSize; i++) {
        funcTbl->identifier[3] = layerTag;
        funcTbl++;
    }
}

static void dStage_dt_c_decode(void* i_data, dStage_dt_c* i_stage, FuncTable* funcTbl,
                               int tblSize) {
    if (i_data != NULL) {
        dStage_fileHeader* file = (dStage_fileHeader*)i_data;
        dStage_nodeHeader* node1 = file->m_nodes;
        for (int i = 0; i < tblSize; i++) {
            node1 = file->m_nodes;

            FuncTable* nodeFunc = funcTbl + i;

            for (int j = 0; j < file->m_chunkCount; j++) {
                dStage_nodeHeader* node2 = node1;
                if ((int)node2->m_tag == *(int*)nodeFunc->identifier) {
                    if (funcTbl[i].function != NULL) {
                        funcTbl[i].function(i_stage, node1, node1->m_entryNum, i_data);
                    }
                    break;
                }
                node1++;
            }
        }
    } else {
        OSReport_Error("dStage_dt_c_decode: i_data is NULL\n");
    }
}

static int dStage_stEventInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    dStage_MapEventInfo_c* mapEvent = (dStage_MapEventInfo_c*)((char*)i_data + 4);
    i_stage->setMapEventInfo(mapEvent);
    OS_REPORT("\nステージイベントデータ初期化！！ %d %x\n\n", mapEvent->num, mapEvent->m_entries);
    return 1;
}

static int dStage_mapEventInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                   void* param_3) {
    dStage_MapEventInfo_c* mapEvent = (dStage_MapEventInfo_c*)((char*)i_data + 4);
    i_stage->setMapEventInfo(mapEvent);
    return 1;
}

static int dStage_floorInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    dStage_FloorInfo_c* floorInfo = (dStage_FloorInfo_c*)((char*)i_data + 4);
    i_stage->setFloorInfo(floorInfo);
    return 1;
}

static int dStage_memaInfoInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    UNUSED(param_2);
    UNUSED(param_3);
    dStage_MemoryMap_c* pd = (dStage_MemoryMap_c*)((char*)i_data + 4);
#if DEBUG
    if (fapGmHIO_getMemoryBlockOff()) {
        pd = NULL;
    }
#endif
    i_stage->setMemoryMap(pd);

    if (pd != NULL) {
        OS_REPORT("Memory Block Create !\n");
        BE(u32)* entry_p = pd->field_0x4;

        JUT_ASSERT(3208, pd->m_num <= dStage_roomControl_c::MEMORY_BLOCK_MAX);
        for (int i = 0; i < pd->m_num; i++) {
            JKRExpHeap* heap = dStage_roomControl_c::createMemoryBlock(i, *entry_p + 0x380);
            JUT_ASSERT(3216, heap != NULL);
            OS_REPORT("\t%02d : size=%d\n", i, *entry_p);
            entry_p++;
        }
    }

    return 1;
}

static int dStage_mecoInfoInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    UNUSED(param_2);
    UNUSED(param_3);
    dStage_MemoryConfig_c* pd = (dStage_MemoryConfig_c*)((char*)i_data + 4);
#if DEBUG
    if (fapGmHIO_getMemoryBlockOff()) {
        pd = NULL;
    }
#endif
    i_stage->setMemoryConfig(pd);

    if (pd != NULL) {
        dStage_MemoryConfig_data* entry_p = pd->field_0x4;

        for (int i = 0; i < pd->m_num; i++) {
            dStage_roomControl_c::setMemoryBlockID(entry_p->m_roomNo, entry_p->m_blockID);
            entry_p++;
        }
    }

    return 1;
}

static int dStage_stageKeepTresureInit(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                       void* param_3) {
    dTres_c::list_class* tresure_p = (dTres_c::list_class*)((char*)i_data + 4);
    dTres_c::addData(tresure_p, i_stage->getRoomNo());
    return 1;
}

static int dStage_fieldMapTresureInit(dStage_dt_c* i_stage, void* i_data, int i_entryNum,
                                      void* param_3) {
    dMenu_Fmap_data_c* data = (dMenu_Fmap_data_c*)i_stage;
    data->setTresure((dTres_c::list_class*)((char*)i_data + 4));
    return 1;
}

static void dStage_dt_c_offsetToPtr(void* i_data) {
    dStage_fileHeader* file = (dStage_fileHeader*)i_data;
    dStage_nodeHeader* p_tno = file->m_nodes;

    for (int i = 0; i < file->m_chunkCount; i++) {
#if TARGET_PC
        p_tno->m_offset.setBase(i_data);
#else
        JUT_ASSERT(3381, p_tno->m_offset != 0);
        if (p_tno->m_offset != 0 && p_tno->m_offset < 0x80000000) {
            p_tno->m_offset += (uintptr_t)i_data;
        }
#endif
        p_tno++;
    }
}

static int dStage_mapPathInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    dStage_nodeHeader* map_path = (dStage_nodeHeader*)((int*)i_data + 1);
    dMpath_c::setPointer(i_stage->getRoomNo(), map_path, 0);
    return 1;
}

static int dStage_mapPathInitCommonLayer(dStage_dt_c* i_stage, void* i_data, int param_2,
                                         void* param_3) {
    map_path_class* map_path = (map_path_class*)((int*)i_data + 1);
    dMpath_c::setPointer(i_stage->getRoomNo(), map_path, 1);
    return 1;
}

static int dStage_fieldMapMapPathInit(dStage_dt_c* i_stage, void* i_data, int param_2,
                                         void* param_3) {
    map_path_class* map_path = (map_path_class*)((int*)i_data + 1);
    dDrawPath_c::room_class* room_p = (dDrawPath_c::room_class*)map_path->m_entries;
    if (room_p == NULL) {
        return 1;
    }

    s8 sp8, sp9;
    dMpath_c::setPointer(room_p, &sp8, &sp9);
    dMenu_Fmap_data_c* data = (dMenu_Fmap_data_c*)i_stage;
    data->setMapPath(room_p);
    return 1;
}

static void readMult(dStage_dt_c* i_stage, dStage_Multi_c* multi, bool useOldRes) {
    static FuncTable l_roomFuncTable[] = {
        {"TRES", dStage_stageKeepTresureInit},
        {"FILI", dStage_filiInfo2Init},
        {"MPAT", dStage_mapPathInitCommonLayer},
        {"Door", dStage_RoomKeepDoorInit},
    };

    static FuncTable l_layerFuncTable[] = {
        {"TRE0", dStage_stageKeepTresureInit},
        {"MPA0", dStage_mapPathInit},
        {"Doo0", dStage_RoomKeepDoorInit},
    };

    if (multi != NULL) {
        dStage_Mult_info* info = multi->m_entries;

        if (dStage_stagInfo_GetUpButton(dComIfGp_getStage()->getStagInfo()) == 0 ||
            dStage_stagInfo_GetUpButton(dComIfGp_getStage()->getStagInfo()) == 6)
        {
            dStage_roomControl_c::createRoomDzs(multi->num);
        }

        for (s8 i = 0; i < multi->num; i++) {
            void* dzs = dStage_roomControl_c::addRoomDzs(i, info->mRoomNo);

            if (dzs == NULL) {
                char dzsName[11];
                sprintf(dzsName, "room%d.dzs", info->mRoomNo);
                JUT_ASSERT(3548, strlen(dzsName) <= sizeof(dzsName));

                if (useOldRes) {
                    dzs = dComIfG_getOldStageRes(dzsName);
                } else {
                    dzs = dComIfG_getStageRes(dzsName);
                }
            } else {
                OS_REPORT("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Field Map Dzs Find !!\n");
            }

            if (dzs != NULL) {
                dStage_dt_c_offsetToPtr(dzs);
                i_stage->setRoomNo(info->mRoomNo);
                dStage_dt_c_decode(dzs, i_stage, l_roomFuncTable, ARRAY_SIZEU(l_roomFuncTable));
                dStage_setLayerTagName(l_layerFuncTable, ARRAY_SIZEU(l_layerFuncTable),
                                       dComIfG_play_c::getLayerNo(0));
                dStage_dt_c_decode(dzs, i_stage, l_layerFuncTable, ARRAY_SIZEU(l_layerFuncTable));
            }

            info++;
        }
    }
}

static int dStage_multInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    dStage_Multi_c* multi = (dStage_Multi_c*)((char*)i_data + 4);
    i_stage->setMulti(multi);
    dStage_initRoomKeepDoorInfo();

    readMult(i_stage, i_stage->getMulti(), false);
    readMult(i_stage, i_stage->getOldMulti(), true);
    i_stage->setRoomNo(-1);

    return 1;
}

static int dStage_lbnkInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    dStage_Lbnk_c* lbnk = (dStage_Lbnk_c*)((char*)i_data + 4);
    i_stage->setLbnk(lbnk);
    return 1;
}

static int dStage_roomTresureInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    stage_tresure_class* tresure_p = (stage_tresure_class*)((char*)i_data + 4);
    i_stage->setTresure(tresure_p);
    dStage_actorInit(i_stage, i_data, entryNum, param_3);
    return 1;
}

static int dStage_layerTresureInit(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                   void* param_3) {
    dStage_actorInit(i_stage, i_data, entryNum, param_3);
    return 1;
}

static int dStage_dmapInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    UNUSED(entryNum);
    UNUSED(param_3);
    dStage_DMap_c* dmap = (dStage_DMap_c*)((char*)i_data + 4);
    i_stage->setDMap(dmap);
    return 1;
}

static int dStage_stageDrtgInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                    void* param_3) {
    stage_tgsc_class* tgsc = (stage_tgsc_class*)((char*)i_data + 4);
    i_stage->setDrTg(tgsc);
    dStage_tgscInfoInit(i_stage, i_data, entryNum, param_3);
    dStage_KeepDoorInfoProc(i_stage, tgsc);
    return 1;
}

static int dStage_roomDrtgInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                   void* param_3) {
    stage_tgsc_class* tgsc = (stage_tgsc_class*)((char*)i_data + 4);
    i_stage->setDrTg(tgsc);
    dStage_doorInfoInit(i_stage, i_data, entryNum, param_3);
    return 1;
}

#if DEBUG
static int dStage_unitInit(dStage_dt_c* i_stage, void* i_data, int, void*) {
    void* unit = (void*)((char*)i_data + 4);
    dBgp_c::setPointer(unit);
    i_stage->setUnit(unit);
    return 1;
}
#endif

static int dStage_elstInfoInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    UNUSED(param_3);
    dStage_Elst_c* elst = (dStage_Elst_c*)((char*)i_data + 4);

    if (param_2 == 0) {
        i_stage->setElst(NULL);
    } else {
        i_stage->setElst(elst);
    }

    return 1;
}

static void dKankyo_create() {
    fopKyM_fastCreate(fpcNm_KANKYO_e, 0, NULL, NULL, NULL);
    fopKyM_fastCreate(fpcNm_KYEFF_e, 0, NULL, NULL, NULL);
    fopKyM_fastCreate(fpcNm_KYEFF2_e, 0, NULL, NULL, NULL);
    fopKyM_fastCreate(fpcNm_ENVSE_e, 0, NULL, NULL, NULL);
}

static void layerMemoryInfoLoader(void* i_data, dStage_dt_c* i_stage, int param_2) {
    UNUSED(param_2);
    static FuncTable l_layerFuncTable[] = {
        {"MEM0", dStage_memaInfoInit},
        {"MEC0", dStage_mecoInfoInit},
    };

    dStage_dt_c_decode(i_data, i_stage, l_layerFuncTable, ARRAY_SIZEU(l_layerFuncTable));
}

static void dStage_dt_c_stageInitLoader(void* i_data, dStage_dt_c* i_stage) {
    static FuncTable l_funcTable[] = {"STAG", dStage_stagInfoInit};

    JUT_ASSERT(3959, i_data != NULL);
    JUT_ASSERT(3960, i_stage != NULL);

    dStage_dt_c_offsetToPtr(i_data);
    i_stage->init();
    dStage_dt_c_decode(i_data, i_stage, l_funcTable, ARRAY_SIZEU(l_funcTable));
    layerMemoryInfoLoader(i_data, i_stage, -1);
}

#if DEBUG
void dStage_DebugDisp() {
    if (data_8074C569_debug) {
        JUTReport(30, 270, "envLayerSet: EnvRoom None");
    }
    if (data_8074C56A_debug) {
        JUTReport(30, 300, "envLayerSet: Color None");
    }
    if (data_8074C56B_debug) {
        JUTReport(30, 330, "envLayerSet: Palette None");
    }
    if (data_8074C56C_debug) {
        JUTReport(30, 360, "envLayerSet: Vrbox None");
    }
}
#endif

static void layerTableLoader(void* i_data, dStage_dt_c* i_stage, int roomNo) {
#if DEBUG
    static FuncTable l_layer0FuncTableA[] = {{"SON0", dStage_soundInfoInit}, {"UNI0", dStage_unitInit}, {"UNI1", dStage_unitInit}};
    static FuncTable l_layer1FuncTableA[] = {{"SON1", dStage_soundInfoInit}, {"UNI0", dStage_unitInit}, {"UNI1", dStage_unitInit}};
    static FuncTable l_layer2FuncTableA[] = {{"SON2", dStage_soundInfoInit}, {"UNI2", dStage_unitInit}, {"UNI3", dStage_unitInit}};
    static FuncTable l_layer3FuncTableA[] = {{"SON3", dStage_soundInfoInit}, {"UNI2", dStage_unitInit}, {"UNI3", dStage_unitInit}};
    static FuncTable l_layer4FuncTableA[] = {{"SON4", dStage_soundInfoInit}, {"UNI0", dStage_unitInit}, {"UNI1", dStage_unitInit}};
    static FuncTable l_layer5FuncTableA[] = {{"SON5", dStage_soundInfoInit}, {"UNI0", dStage_unitInit}, {"UNI1", dStage_unitInit}};
    static FuncTable l_layer6FuncTableA[] = {{"SON6", dStage_soundInfoInit}, {"UNI2", dStage_unitInit}, {"UNI3", dStage_unitInit}};
    static FuncTable l_layer7FuncTableA[] = {{"SON7", dStage_soundInfoInit}, {"UNI2", dStage_unitInit}, {"UNI3", dStage_unitInit}};
    static FuncTable l_layer8FuncTableA[] = {{"SON8", dStage_soundInfoInit}, {"UNI0", dStage_unitInit}, {"UNI1", dStage_unitInit}};
    static FuncTable l_layer9FuncTableA[] = {{"SON9", dStage_soundInfoInit}, {"UNI0", dStage_unitInit}, {"UNI1", dStage_unitInit}};
    static FuncTable l_layerAFuncTableA[] = {{"SONa", dStage_soundInfoInit}, {"UNI2", dStage_unitInit}, {"UNI3", dStage_unitInit}};
    static FuncTable l_layerBFuncTableA[] = {{"SONb", dStage_soundInfoInit}, {"UNI2", dStage_unitInit}, {"UNI3", dStage_unitInit}};
    static FuncTable l_layerCFuncTableA[] = {{"SONc", dStage_soundInfoInit}, {"UNI0", dStage_unitInit}, {"UNI1", dStage_unitInit}};
    static FuncTable l_layerDFuncTableA[] = {{"SONd", dStage_soundInfoInit}, {"UNI0", dStage_unitInit}, {"UNI1", dStage_unitInit}};
    static FuncTable l_layerEFuncTableA[] = {{"SONe", dStage_soundInfoInit}, {"UNI2", dStage_unitInit}, {"UNI3", dStage_unitInit}};
    static FuncTable* l_layerFuncTableA_p[] = {
        l_layer0FuncTableA, l_layer1FuncTableA, l_layer2FuncTableA, l_layer3FuncTableA,
        l_layer4FuncTableA, l_layer5FuncTableA, l_layer6FuncTableA, l_layer7FuncTableA,
        l_layer8FuncTableA, l_layer9FuncTableA, l_layerAFuncTableA, l_layerBFuncTableA,
        l_layerCFuncTableA, l_layerDFuncTableA, l_layerEFuncTableA,
    };
#else
    static FuncTable l_layerFuncTableA[] = {"SON0", dStage_soundInfoInit};
#endif
    static FuncTable l_envLayerFuncTable[] = {
        {"LGT0", dStage_lgtvInfoInit},     {"Env0", dStage_envrInfoInit},
        {"Col0", dStage_pselectInfoInit},  {"PAL0", dStage_paletteInfoInit},
        {"VRB0", dStage_vrboxcolInfoInit},
    };

    int newRoomNo;
    if (roomNo != -1) {
        newRoomNo = roomNo;
    } else {
        newRoomNo = dComIfGp_getStartStageRoomNo();
    }

#if DEBUG
    dStage_dt_c_decode(i_data, i_stage, l_layerFuncTableA_p[dComIfG_play_c::getLayerNo(0)], ARRAY_SIZE(l_layer0FuncTableA));
#else
    dStage_setLayerTagName(l_layerFuncTableA, ARRAY_SIZEU(l_layerFuncTableA),
                           dComIfG_play_c::getLayerNo(0));
    dStage_dt_c_decode(i_data, i_stage, l_layerFuncTableA, ARRAY_SIZEU(l_layerFuncTableA));
#endif


    dStage_Elst_c* elst = dComIfGp_getStage()->getElst();
    if (elst != NULL && newRoomNo >= 0 && elst->m_entryNum > newRoomNo) {
        dStage_Elst_dt_c* d = elst->m_entries;
        int l = dComIfG_play_c::getLayerNo(0);
        JUT_ASSERT(4174, 0 <= l && l < 15);
        int env_layer = d[newRoomNo].m_layerTable[l];
        JUT_ASSERT(4182, 0 <= env_layer && env_layer < 15);
        dStage_setLayerTagName(l_envLayerFuncTable, ARRAY_SIZEU(l_envLayerFuncTable), env_layer);
        dStage_dt_c_decode(i_data, i_stage, l_envLayerFuncTable, ARRAY_SIZEU(l_envLayerFuncTable));
#if DEBUG
        if (roomNo != -1 && !i_stage->getLightVecInfo()) {
            data_8074C568_debug = true;
        }
        if (roomNo == -1 && !i_stage->getEnvrInfo()) {
            data_8074C569_debug = true;
        }
        if (roomNo == -1 && !i_stage->getPselectInfo()) {
            data_8074C56A_debug = true;
        }
        if (roomNo == -1 && !i_stage->getPaletteInfo()) {
            data_8074C56B_debug = true;
        }
        if (roomNo == -1 && !i_stage->getVrboxcolInfo()) {
            data_8074C56C_debug = true;
        }
#endif
    } else {
        dStage_setLayerTagName(l_envLayerFuncTable, ARRAY_SIZEU(l_envLayerFuncTable), 0);
        dStage_dt_c_decode(i_data, i_stage, l_envLayerFuncTable, ARRAY_SIZEU(l_envLayerFuncTable));
    }
}

dStage_Elst_c* dStage_stageDt_c::getElst(void) { return mElst; }

static void layerActorLoader(void* i_data, dStage_dt_c* i_stage, int param_2) {
    UNUSED(param_2);
    static FuncTable l_layerFuncTable[] = {
#if PLATFORM_GCN
        {"Doo0", dStage_roomDrtgInfoInit},
#endif
        {"SCO0", dStage_tgscInfoInit},
        {"ACT0", dStage_actorInit},
        {"TRE0", dStage_layerTresureInit},
#if !PLATFORM_GCN
        {"Doo0", dStage_roomDrtgInfoInit},
#endif
    };

    dStage_setLayerTagName(l_layerFuncTable, 4, dComIfG_play_c::getLayerNo(0));
    dStage_dt_c_decode(i_data, i_stage, l_layerFuncTable, ARRAY_SIZEU(l_layerFuncTable));
}

void dStage_dt_c_stageLoader(void* i_data, dStage_dt_c* i_stage) {
    static FuncTable l_funcTable[] = {
        {"EVLY", dStage_elstInfoInit},      {"RPPN", dStage_rppnInfoInit},
        {"RPAT", dStage_rpatInfoInit},      {"MULT", dStage_multInfoInit},
        {"PLYR", dStage_playerInit},        {"CAMR", dStage_cameraInit},
        {"RCAM", dStage_cameraInit},        {"ACTR", dStage_actorInit_always},
        {"TGOB", dStage_actorInit_always},  {"RTBL", dStage_roomReadInit},
        {"AROB", dStage_arrowInit},         {"RARO", dStage_arrowInit},
        {"Virt", dStage_vrboxInfoInit},     {"SCLS", dStage_sclsInfoInit},
        {"TGSC", dStage_tgscInfoInit},      {"LGHT", dStage_plightInfoInit},
        {"PPNT", dStage_ppntInfoInit},      {"PATH", dStage_pathInfoInit},
        {"SCOB", dStage_tgscInfoInit},      {"FILI", dStage_filiInfoInit},
        {"Door", dStage_stageDrtgInfoInit}, {"FLOR", dStage_floorInfoInit},
        {"TGDR", dStage_tgscInfoInit},      {"DMAP", dStage_dmapInfoInit},
        {"REVT", dStage_stEventInfoInit},   {"SOND", dStage_soundInfoInitCL},
    };

    dStage_dt_c_decode(i_data, i_stage, l_funcTable, ARRAY_SIZEU(l_funcTable));
    layerTableLoader(i_data, i_stage, -1);
    layerActorLoader(i_data, i_stage, -1);
}

void dStage_dt_c_roomLoader(void* i_data, dStage_dt_c* i_stage, int param_2) {
    static FuncTable l_funcTable[] = {
        {"PLYR", dStage_playerInit},   {"RCAM", dStage_RoomCameraInit},
        {"RARO", dStage_arrowInit},    {"RTBL", dStage_roomReadInit},
        {"AROB", dStage_arrowInit},    {"Virt", dStage_vrboxInfoInit},
        {"SCLS", dStage_sclsInfoInit}, {"LGHT", dStage_plightInfoInit},
        {"RPPN", dStage_rppnInfoInit}, {"RPAT", dStage_rpatInfoInit},
        {"FILI", dStage_filiInfoInit}, {"FLOR", dStage_floorInfoInit},
        {"LBNK", dStage_lbnkInfoInit}, {"SOND", dStage_soundInfoInitCL},
    };

    dStage_dt_c_offsetToPtr(i_data);
    i_stage->init();
    dStage_dt_c_decode(i_data, i_stage, l_funcTable, ARRAY_SIZEU(l_funcTable));
    layerTableLoader(i_data, i_stage, param_2);
}

void dStage_dt_c_roomReLoader(void* i_data, dStage_dt_c* i_stage, int param_2) {
    static FuncTable l_funcTable[] = {
#if PLATFORM_GCN
        {"Door", dStage_roomDrtgInfoInit},
#endif
        {"ACTR", dStage_actorCommonLayerInit},
        {"TGOB", dStage_actorCommonLayerInit},
        {"TRES", dStage_roomTresureInit},
        {"TGSC", dStage_tgscCommonLayerInit},
        {"SCOB", dStage_tgscCommonLayerInit},
#if !PLATFORM_GCN
        {"Door", dStage_roomDrtgInfoInit},
#endif
        {"TGDR", dStage_tgscInfoInit},
        {"REVT", dStage_mapEventInfoInit},
    };

    dStage_dt_c_decode(i_data, i_stage, l_funcTable, ARRAY_SIZEU(l_funcTable));
    layerActorLoader(i_data, i_stage, param_2);
}

void dStage_dt_c_fieldMapLoader(void* i_data, dStage_dt_c* i_stage) {
    static FuncTable l_funcTable[] = {
        {"TRES", dStage_fieldMapTresureInit},
        {"FILI", dStage_fieldMapFiliInfo2Init},
        {"MPAT", dStage_fieldMapMapPathInit},
    };

    JUT_ASSERT(4428, i_data != NULL);

    dStage_dt_c_offsetToPtr(i_data);
    dStage_dt_c_decode(i_data, i_stage, l_funcTable, ARRAY_SIZEU(l_funcTable));
}

JKRExpHeap* dStage_roomControl_c::mMemoryBlock[MEMORY_BLOCK_MAX] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

char dStage_roomControl_c::mArcBank[32][10] = {0};

void dStage_infoCreate() {
    OS_REPORT("dStage_Create\n");
    void* stageRsrc = dComIfG_getStageRes("stage.dzs");
    JUT_ASSERT(4451, stageRsrc != NULL);

    dComIfGp_roomControl_init();
    dStage_dt_c_stageInitLoader(stageRsrc, dComIfGp_getStage());
}

char dStage_roomControl_c::mDemoArcName[10];

void dStage_Create() {
    void* stageRsrc = dComIfG_getStageRes("stage.dzs");
    JUT_ASSERT(4451, stageRsrc != NULL);
#if DEBUG
    data_8074C568_debug = false;
    data_8074C569_debug = false;
    data_8074C56A_debug = false;
    data_8074C56B_debug = false;
    data_8074C56C_debug = false;
#endif
    dStage_dt_c_stageLoader(stageRsrc, dComIfGp_getStage());
    daSus_c::execute();

    if (dComIfGp_getStartStageRoomNo() >= 0) {
        int status = dStage_roomInit(dComIfGp_getStartStageRoomNo());
        JUT_ASSERT(4517, status);
    }

    *dStage_roomControl_c::getDemoArcName() = 0;
    dKankyo_create();

    if (dComIfG_getStageRes("vrbox_sora.bmd")) {
        fopAcM_Create(fpcNm_VRBOX_e, NULL, NULL);
        fopAcM_Create(fpcNm_VRBOX2_e, NULL, NULL);
    }

    dComIfGp_evmng_create();
}

void dStage_Delete() {
    OS_REPORT("dStage_Delete\n");

    char* demoArcName = dStage_roomControl_c::getDemoArcName();
    if (*demoArcName != '\0') {
        dComIfG_deleteObjectResMain(demoArcName);
    }

    dComIfGs_putSave(dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo()));
    dStage_roomControl_c::removeRoomDzs();

    if (mDoRst::isReset() || !dComIfGp_isEnableNextStage() ||
        strcmp(dComIfGp_getNextStageName(), dComIfGp_getStartStageName()))
    {
        dStage_roomControl_c::destroyMemoryBlock();

        if (dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) == ST_DUNGEON) {
            dRes_info_c* info = dComIfG_getStageResInfo("Stg_00");
            JUT_ASSERT(4579, info != NULL);
            *info->getArchiveName() = 'X';
            dComIfGp_setOldMulti();
        } else {
            dComIfG_deleteStageRes("Stg_00");
            dComIfG_deleteStageRes("Xtg_00");
            dComIfGp_resetOldMulti();
        }
    } else {
        OS_REPORT(">>> ステージデータ保持！\n");
    }

    dComIfG_deleteObjectResMain("Event");
    const char* cam_filename = dComIfGp_getCameraParamFileName(0);
    dComIfG_deleteObjectResMain(cam_filename);
    dComIfGp_evmng_remove();
    dComIfGp_getStage()->init();
}

s8 dStage_roomControl_c::mRoomReadId = -1;

int dStage_RoomCheck(cBgS_GndChk* gndChk) {
    int roomReadId = dStage_roomControl_c::getRoomReadId();
    int roomId;

    if (gndChk == NULL) {
        JUT_ASSERT(4627, roomReadId >= 0);
        roomId = roomReadId;
    } else {
        roomId = dComIfG_Bgsp().GetRoomId(*gndChk);
    }

    if (roomId < 0) {
        return 0;
    }

    if (roomId != dComIfGp_roomControl_getStayNo()) {
        dComIfGp_roomControl_zoneCountCheck(roomId);
    }

    if (roomReadId < 0) {
        roomReadId = dComIfGp_roomControl_getStayNo();
    }

    roomRead_class* room = dComIfGp_getStageRoom();
    if (room != NULL && room->num > roomReadId) {
        dComIfGp_roomControl_setTimePass(dStage_roomRead_dt_c_GetTimePass(*room->m_entries[dComIfGp_roomControl_getStayNo()]));

        roomRead_data_class* room_data = room->m_entries[roomReadId];
        return dComIfGp_roomControl_loadRoom(room_data->num, room_data->m_rooms, false);
    }

    return 1;
}

int dStage_changeSceneExitId(cBgS_PolyInfo& param_0, f32 speed, u32 mode, s8 roomNo, s16 angle) {
    s32 exit_id = dComIfG_Bgsp().GetExitId(param_0);
    return dStage_changeScene(exit_id, speed, mode, roomNo, angle, -1);
}

int dStage_changeScene(int i_exitId, f32 speed, u32 mode, s8 room_no, s16 angle, int param_5) {
    stage_scls_info_dummy_class* scls;

    if (room_no == -1) {
        scls = dComIfGp_getStageSclsInfo();
    } else {
        JUT_ASSERT(4783, 0 <= room_no && room_no < 64);
        dStage_roomDt_c* room = dComIfGp_roomControl_getStatusRoomDt(room_no);
        scls = room->getSclsInfo();
    }

    if (scls == NULL) {
        return 0;
    }

    JUT_ASSERT(4793, 0 <= i_exitId && i_exitId < scls->num);

    stage_scls_info_class* scls_info = &scls->m_entries[i_exitId];
    s32 wipe = dStage_sclsInfo_getWipe(scls_info);
    s32 wipe_time = dStage_sclsInfo_getWipeTime(scls_info);
    s32 layer = dStage_sclsInfo_getSceneLayer(scls_info);
    int timeH = dStage_sclsInfo_getTimeH(scls_info);

    if (layer >= 15) {
        layer = -1;
    }

    if (layer == -1 && param_5 != -1) {
        layer = param_5;
    }

    if (timeH < 31) {
        dKy_set_nexttime(15.0f * timeH);
    }

    dComIfGp_setNextStage(scls_info->mStage, scls_info->mStart, (s8)scls_info->mRoom, (s8)layer,
                          speed, mode, 1, wipe == 15 ? 0 : wipe, angle, 1, wipe_time);
    return 1;
}

static void dummy5() {
    dComIfGp_roomControl_getStatusRoomDt(0)->getSclsInfo();
    ((dStage_stageDt_c*)dComIfGp_getStage())->getSclsInfo();
}

int dStage_changeScene4Event(int i_exitId, s8 room_no, int i_wipe, bool param_3, f32 speed,
                             u32 mode, s16 angle, int param_7) {
    stage_scls_info_dummy_class* scls;

    if (room_no == -1) {
        scls = dComIfGp_getStageSclsInfo();
    } else {
        JUT_ASSERT(4853, 0 <= room_no && room_no < 64);
        dStage_roomDt_c* room = dComIfGp_roomControl_getStatusRoomDt(room_no);
        scls = room->getSclsInfo();
    }

    if (scls == NULL) {
        // "%d: %d: Scene List doesn't exist. \n"
        OSReport_Error("%s: %d: シーンリストがありません。\n", "d_stage.cpp", 4865);
        return 0;
    }

    JUT_ASSERT(4865, 0 <= i_exitId && i_exitId < scls->num);
    stage_scls_info_class* scls_info = &scls->m_entries[i_exitId];

    s32 wipe = i_wipe == -1 ? dStage_sclsInfo_getWipe(scls_info) : i_wipe;
    s32 wipe_time = i_wipe == -1 ? dStage_sclsInfo_getWipeTime(scls_info) : 0;

    s32 layer = dStage_sclsInfo_getSceneLayer(scls_info);
    int timeH = dStage_sclsInfo_getTimeH(scls_info);

    if (layer >= 15) {
        layer = -1;
    }

    if (layer == -1 && param_7 != -1) {
        layer = param_7;
    }

    if (timeH < 31) {
        dKy_set_nexttime(15.0f * timeH);
    }

    dComIfGp_setNextStage(scls_info->mStage, scls_info->mStart, (s8)scls_info->mRoom, layer,
                          speed, mode, 1, wipe == 15 ? 0 : wipe, angle, param_3 ? 1 : 0,
                          wipe_time);
    return 1;
}

static void dummy6() {
    dComIfGs_getStartPoint();
}

void dStage_restartRoom(u32 roomParam, u32 mode, int param_2) {
    dComIfGp_setNextStage(dComIfGp_getStartStageName(), -1, dComIfGs_getRestartRoomNo(), -1, 0.0f,
                          mode, 0, 0, 0, param_2, 0);
    dComIfGs_setRestartRoomParam(roomParam);
}

#if VERSION == VERSION_WII_USA_R0
void dStage_escapeRestart() {
    daAlink_c* player_p = daAlink_getAlinkActorClass();
    dComIfGs_setTurnRestart(player_p->current.pos, player_p->shape_angle.y, fopAcM_GetRoomNo(player_p), fopAcM_GetParam(player_p));
    dComIfGp_setNextStage(dComIfGp_getStartStageName(), -2, dComIfGs_getTurnRestartRoomNo(), -1, 0.0f, 0, 0, 9, 0, 1, 0);
}
#endif
