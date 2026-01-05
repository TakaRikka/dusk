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
#include "f_op/f_op_kankyo_mng.h"
#include "f_op/f_op_msg_mng.h"
#include "f_op/f_op_scene_mng.h"
#include "global.h"
#include "m_Do/m_Do_Reset.h"
#include <stdio>

void dStage_nextStage_c::set(const char* i_stage, s8 i_roomId, s16 i_point, s8 i_layer, s8 i_wipe,
                             u8 i_speed) {
    if (!enabled) {
        enabled = true;
        wipe = i_wipe;
        wipe_speed = i_speed;
        dStage_startStage_c::set(i_stage, i_roomId, i_point, i_layer);
        if (!strcmp(i_stage, "OPENING")) {
            mDoRst::onReset();
        }
    }
}

void dStage_SetErrorRoom() {
    // "Room information might be corrupted. \nPlease try to re-convert. \n"
    OSReport_Error("部屋情報が、多分ですが壊れています。\n変換し直してみてください。\n");
}

void dStage_SetErrorStage() {
    // "Stage information might be corrupted. \nPlease try to re-convert. "
    OSReport_Error("ステージ情報が、多分ですが壊れています。\n変換し直してみてください。");
}

static dStage_KeepDoorInfo DoorInfo;

dStage_KeepDoorInfo* dStage_GetKeepDoorInfo() {
    return &DoorInfo;
}

static u8 dStage_isBossStage(dStage_dt_c* i_stage) {
    stage_stag_info_class* pstag = i_stage->getStagInfo();

    if (pstag == NULL) {
        return false;
    }

    return dStage_stagInfo_GetSTType(pstag) == ST_BOSS_ROOM;
}

static void dStage_KeepDoorInfoInit(dStage_dt_c* i_stage) {
    if (dStage_isBossStage(i_stage) == 0) {
        DoorInfo.mNum = 0;
    }
}

static void dStage_KeepDoorInfoProc(dStage_dt_c* i_stage, stage_tgsc_class* i_drtg) {
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
    for (int i = 0; i < DoorInfo.mNum; pDstEntry++, pSrcEntry++, i++) {
        *pDstEntry = *pSrcEntry;
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
    if (param_2 == NULL || param_2->num + l_RoomKeepDoorInfo.mNum >= 0x40 || param_2->num < 0) {
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
    dStage_RoomKeepDoorInfoProc(i_stage, (stage_tgsc_class*)((char*)i_data + 4));
    return 1;
}

void dStage_startStage_c::set(const char* i_Name, s8 i_RoomNo, s16 i_Point, s8 i_Layer) {
    strcpy(mName, i_Name);
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

    if (dComIfGp_getStartStagePoint() >= 0 || dComIfGp_getStartStagePoint() == -4) {
        initZone();
    }

    dStage_roomStatus_c* status = mStatus;
    for (int i = 0; i < 0x40; i++) {
        status->mRoomDt.init();
        status->mRoomDt.initFileList2();
        setStatusFlag(i, 0);
        status->mDraw = false;

        if (status->mZoneNo >= 0) {
            dComIfGs_clearRoomSwitch(status->mZoneNo);
            dComIfGs_clearRoomItem(status->mZoneNo);
        }

        status->mMemBlockID = -1;
        status->mpBgW = NULL;
        status++;
    }

    mArcBankName = (dStage_roomControl_c::nameData*)dComIfG_getStageRes("name.bin");
    mArcBankData = (dStage_roomControl_c::bankData*)dComIfG_getStageRes("bank.bin");

    if (mArcBankName == NULL) {
        mArcBankData = NULL;
    }
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
        return NULL;
    }
    return &mStatus[i_statusIdx].mRoomDt;
}

// clang-format off
#define OBJNAME(name, proc, sub)                                                                   \
    { name, proc, sub }

static dStage_objectNameInf l_objectName[] = {
    OBJNAME("Grass",   PROC_GRASS,             -1),
    OBJNAME("kusax1",  PROC_GRASS,             -1),
    OBJNAME("kusax7",  PROC_GRASS,             -1),
    OBJNAME("kusax21", PROC_GRASS,             -1),
    OBJNAME("flower",  PROC_GRASS,             -1),
    OBJNAME("flwr7",   PROC_GRASS,             -1),
    OBJNAME("flwr17",  PROC_GRASS,             -1),
    OBJNAME("pflower", PROC_GRASS,             -1),
    OBJNAME("pflwrx7", PROC_GRASS,             -1),
    OBJNAME("door",    PROC_DOOR20,            -1),
    OBJNAME("kdoor",   PROC_KNOB20,            -1),
    OBJNAME("ddoor",   PROC_DBDOOR,            -1),
    OBJNAME("bdoor",   PROC_BOSS_DOOR,         -1),
    OBJNAME("ndoor",   PROC_DOOR20,            -1),
    OBJNAME("tadoor",  PROC_DOOR20,            -1),
    OBJNAME("yodoor",  PROC_DOOR20,            -1),
    OBJNAME("nadoor",  PROC_DOOR20,            -1),
    OBJNAME("l9door",  PROC_DOOR20,            -1),
    OBJNAME("l7door",  PROC_DOOR20,            -1),
    OBJNAME("pdoor",   PROC_PushDoor,          -1),
    OBJNAME("bigdoor", PROC_DOOR20,            -1),
    OBJNAME("kshtr00", PROC_Obj_Kshutter,      -1),
    OBJNAME("vshuter", PROC_Obj_Kshutter,      -1),
    OBJNAME("IzmGate", PROC_Izumi_Gate,        -1),
    OBJNAME("NoneCam", PROC_CAMERA,            -1),
    OBJNAME("NormCam", PROC_CAMERA,            -1),
    OBJNAME("Link",    PROC_ALINK,             -1),
    OBJNAME("carry00", PROC_Obj_Carry,         -1),
    OBJNAME("carry01", PROC_Obj_Carry,         -1),
    OBJNAME("carry02", PROC_Obj_Carry,         -1),
    OBJNAME("carry04", PROC_Obj_Carry,         -1),
    OBJNAME("carry05", PROC_Obj_Carry,         -1),
    OBJNAME("carry06", PROC_Obj_Carry,         -1),
    OBJNAME("carry07", PROC_Obj_Carry,         -1),
    OBJNAME("carry08", PROC_Obj_Carry,         -1),
    OBJNAME("carry09", PROC_Obj_Carry,         -1),
    OBJNAME("ball1",   PROC_Obj_Carry,         -1),
    OBJNAME("ball2",   PROC_Obj_Carry,         -1),
    OBJNAME("ball3",   PROC_Obj_Carry,         -1),
    OBJNAME("carry12", PROC_Obj_Carry,         -1),
    OBJNAME("carry13", PROC_Obj_Carry,         -1),
    OBJNAME("TagLite", PROC_Tag_LightBall,     -1),
    OBJNAME("InoBone", PROC_Obj_InoBone,       -1),
    OBJNAME("osiBLK0", PROC_Obj_Movebox,       0x00),
    OBJNAME("dmblk",   PROC_Obj_Movebox,       0x00),
    OBJNAME("Kkiba",   PROC_Obj_Movebox,       0x02),
    OBJNAME("smblk",   PROC_Obj_Movebox,       0x02),
    OBJNAME("Lv4blk",  PROC_Obj_Movebox,       -1),
    OBJNAME("Lv4blk2", PROC_Obj_Movebox,       -1),
    OBJNAME("gstone",  PROC_Obj_Movebox,       -1),
    OBJNAME("TagAtt",  PROC_Tag_Attp,          -1),
    OBJNAME("mmvbg",   PROC_SET_BG_OBJ,        -1),
    OBJNAME("Drop",    PROC_Obj_Drop,          -1),
    OBJNAME("BkLeaf",  PROC_Obj_BkLeaf,        -1),
    OBJNAME("cylwind", PROC_Obj_Tornado,       -1),
    OBJNAME("TagTrnd", PROC_Obj_Tornado2,      -1),
    OBJNAME("mvstair", PROC_Obj_MvStair,       -1),
    OBJNAME("UHDoor",  PROC_Obj_Cowdoor,       -1),
    OBJNAME("SwBoom",  PROC_Obj_Swpropeller,   -1),
    OBJNAME("Swboom",  PROC_Obj_Swpropeller,   -1),
    OBJNAME("BoomSht", PROC_Obj_BoomShutter,   -1),
    OBJNAME("haihai",  PROC_Obj_Hfuta,         -1),
    OBJNAME("stopper", PROC_Obj_Stopper,       -1),
    OBJNAME("dstop",   PROC_Obj_Stopper2,      -1),
    OBJNAME("mhole",   PROC_Obj_MHole,         -1),
    OBJNAME("mhole2",  PROC_Obj_MHole,         -1),
    OBJNAME("TagMag",  PROC_Tag_Magne,         -1),
    OBJNAME("P_Warp",  PROC_Obj_BossWarp,      -1),
    OBJNAME("fpillar", PROC_Obj_FirePillar,    -1),
    OBJNAME("yfire",   PROC_Obj_FirePillar2,   -1),
    OBJNAME("pfire",   PROC_Obj_FirePillar2,   -1),
    OBJNAME("Huriko1", PROC_Obj_WoodPendulum,  -1),
    OBJNAME("VolBall", PROC_Obj_VolcanicBall,  -1),
    OBJNAME("VolBom",  PROC_Obj_VolcanicBall,  -1),
    OBJNAME("BigVolc", PROC_Obj_VolcanicBomb,  -1),
    OBJNAME("geyser",  PROC_Obj_Geyser,        -1),
    OBJNAME("Cwall",   PROC_Obj_ChainWall,     -1),
    OBJNAME("Tansu",   PROC_Obj_Chest,         -1),
    OBJNAME("Onsen",   PROC_Obj_Onsen,         -1),
    OBJNAME("Obj_bm",  PROC_Obj_Bemos,         -1),
    OBJNAME("lv6bm",   PROC_Obj_Lv6bemos,      -1),
    OBJNAME("E_bm6",   PROC_Obj_Lv6bemos2,     -1),
    OBJNAME("stone",   PROC_Obj_Stone,         -1),
    OBJNAME("stoneB",  PROC_Obj_Stone,         -1),
    OBJNAME("TagSpin", PROC_Tag_Spinner,       -1),
    OBJNAME("L_RopeS", PROC_Obj_RopeBridge,    -1),
    OBJNAME("L_RopeB", PROC_Obj_RopeBridge,    -1),
    OBJNAME("wcover",  PROC_Obj_WellCover,     -1),
    OBJNAME("rstair",  PROC_Obj_RotStair,      -1),
    OBJNAME("marm",    PROC_Obj_MagneArm,      -1),
    OBJNAME("turnSw",  PROC_Obj_SwTurn,        -1),
    OBJNAME("chainSw", PROC_Obj_SwChain,       -1),
    OBJNAME("wsword",  PROC_Obj_WoodenSword,   -1),
    OBJNAME("iceblk",  PROC_Obj_IceBlock,      -1),
    OBJNAME("KkrSmk",  PROC_Obj_Smoke,         -1),
    OBJNAME("dmele",   PROC_Obj_Elevator,      -1),
    OBJNAME("wfall",   PROC_Obj_WaterFall,     -1),
    OBJNAME("izora",   PROC_Obj_ZoraCloth,     -1),
    OBJNAME("smgdoor", PROC_Obj_SmgDoor,       -1),
    OBJNAME("smkdoor", PROC_Obj_SmgDoor,       -1),
    OBJNAME("Obj_ava", PROC_Obj_Avalanche,     -1),
    OBJNAME("thdoor",  PROC_Obj_TDoor,         -1),
    OBJNAME("mstrsrd", PROC_Obj_MasterSword,   -1),
    OBJNAME("wimage",  PROC_Obj_WoodStatue,    -1),
    OBJNAME("sword",   PROC_Obj_Sword,         -1),
    OBJNAME("spring",  PROC_Tag_Spring,        -1),
    OBJNAME("BkDoorL", PROC_Obj_BkDoor,        -1),
    OBJNAME("BkDoorR", PROC_Obj_BkDoor,        -1),
    OBJNAME("IGateL",  PROC_Obj_MGate,         -1),
    OBJNAME("IGateR",  PROC_Obj_MGate,         -1),
    OBJNAME("HGateL",  PROC_Obj_MGate,         -1),
    OBJNAME("HGateR",  PROC_Obj_MGate,         -1),
    OBJNAME("K_Gate",  PROC_Obj_KkrGate,       -1),
    OBJNAME("R_Gate",  PROC_Obj_RiderGate,     -1),
    OBJNAME("kkanban", PROC_Obj_KKanban,       -1),
    OBJNAME("Mhsg6",   PROC_Obj_Ladder,        0x00),
    OBJNAME("Mhsg9",   PROC_Obj_Ladder,        0x01),
    OBJNAME("Mhsg12",  PROC_Obj_Ladder,        0x02),
    OBJNAME("Mhsg15",  PROC_Obj_Ladder,        0x03),
    OBJNAME("Mhsg4h",  PROC_Obj_Ladder,        0x04),
    OBJNAME("Mhsg3",   PROC_Obj_Ladder,        0x05),
    OBJNAME("bonbori", PROC_EP,                -1),
    OBJNAME("Cow",     PROC_COW,               -1),
    OBJNAME("Peru",    PROC_PERU,              -1),
    OBJNAME("Kago",    PROC_KAGO,              -1),
    OBJNAME("TagFall", PROC_Tag_WaterFall,     -1),
    OBJNAME("Kbota_A", PROC_Obj_Swpush,        0x00),
    OBJNAME("Kbota_B", PROC_Obj_Swpush,        0x01),
    OBJNAME("KbotaC",  PROC_Obj_Swpush,        0x02),
    OBJNAME("Swpush",  PROC_Obj_Swpush,        0x00),
    OBJNAME("Lv3bota", PROC_Obj_Swpush,        0x00),
    OBJNAME("Ksw00_C", PROC_Obj_Swpush2,       0x02),
    OBJNAME("hvySw",   PROC_Obj_Swpush5,       0x00),
    OBJNAME("buraA",   PROC_Obj_SwHang,        0x00),
    OBJNAME("buraB",   PROC_Obj_SwHang,        0x00),
    OBJNAME("buraA2",  PROC_Obj_SwHang,        0x00),
    OBJNAME("buraB2",  PROC_Obj_SwHang,        0x00),
    OBJNAME("bura7A",  PROC_Obj_SwHang,        0x00),
    OBJNAME("bura7B",  PROC_Obj_SwHang,        0x00),
    OBJNAME("bura7C",  PROC_Obj_SwHang,        0x00),
    OBJNAME("bura7D",  PROC_Obj_SwHang,        0x00),
    OBJNAME("bura7E",  PROC_Obj_SwHang,        0x00),
    OBJNAME("tbox_sw", PROC_TBOX_SW,           0x00),
    OBJNAME("swLight", PROC_Obj_SwLight,       0x00),
    OBJNAME("iceleaf", PROC_Obj_IceLeaf,       0x00),
    OBJNAME("swHit",   PROC_SWHIT0,            0x00),
    OBJNAME("swHit2",  PROC_SWHIT0,            0x01),
    OBJNAME("ObjTime", PROC_Obj_Timer,         -1),
    OBJNAME("AND_SW",  PROC_ANDSW,             -1),
    OBJNAME("AND_SW2", PROC_ANDSW2,            -1),
    OBJNAME("ALLdie",  PROC_ALLDIE,            -1),
    OBJNAME("rvback",  PROC_Tag_RiverBack,     -1),
    OBJNAME("kagoFal", PROC_Tag_KagoFall,      -1),
    OBJNAME("CrvGate", PROC_Obj_CRVGATE,       -1),
    OBJNAME("CrvSaku", PROC_Obj_CRVFENCE,      -1),
    OBJNAME("CrvWood", PROC_Obj_CRVHAHEN,      -1),
    OBJNAME("CrvStel", PROC_Obj_CRVSTEEL,      -1),
    OBJNAME("CrvLH",   PROC_Obj_CRVLH_DW,      -1),
    OBJNAME("RvRock",  PROC_Obj_RIVERROCK,     -1),
    OBJNAME("P2a",     PROC_NPC_P2,            0x00),
    OBJNAME("P2b",     PROC_NPC_P2,            0x01),
    OBJNAME("P2c",     PROC_NPC_P2,            0x02),
    OBJNAME("Gnd",     PROC_NPC_GND,           -1),
    OBJNAME("grA",     PROC_NPC_GRA,           -1),
    OBJNAME("Obj_grA", PROC_OBJ_GRA,           -1),
    OBJNAME("Tag_grA", PROC_TAG_GRA,           -1),
    OBJNAME("TagYami", PROC_TAG_YAMI,          -1),
    OBJNAME("grC",     PROC_NPC_GRC,           -1),
    OBJNAME("grM",     PROC_NPC_GRM,           -1),
    OBJNAME("grMC",    PROC_NPC_GRMC,          -1),
    OBJNAME("grS",     PROC_NPC_GRS,           -1),
    OBJNAME("grD1",    PROC_NPC_GRD,           -1),
    OBJNAME("grD",     PROC_NPC_WRESTLER,      0x01),
    OBJNAME("grR",     PROC_NPC_GRR,           -1),
    OBJNAME("grO",     PROC_NPC_GRO,           -1),
    OBJNAME("grZ",     PROC_NPC_GRZ,           -1),
    OBJNAME("grARock", PROC_Obj_GraRock,       -1),
    OBJNAME("grZRock", PROC_Obj_GrzRock,       -1),
    OBJNAME("osnFire", PROC_OBJ_ONSEN_FIRE,    -1),
    OBJNAME("yamiD",   PROC_NPC_YAMID,         -1),
    OBJNAME("yamiT",   PROC_NPC_YAMIT,         -1),
    OBJNAME("yamiS",   PROC_NPC_YAMIS,         -1),
    OBJNAME("Blue_NS", PROC_NPC_BLUENS,        -1),
    OBJNAME("Kakashi", PROC_NPC_KAKASHI,       0x00),
    OBJNAME("Kdk",     PROC_NPC_KDK,           -1),
    OBJNAME("Aru",     PROC_NPC_ARU,           -1),
    OBJNAME("Bans",    PROC_NPC_BANS,          -1),
    OBJNAME("Besu",    PROC_NPC_BESU,          -1),
    OBJNAME("Bou",     PROC_NPC_BOU,           -1),
    OBJNAME("BouS",    PROC_NPC_BOU_S,         0x00),
    OBJNAME("clerkA",  PROC_NPC_CLERKA,        0x00),
    OBJNAME("clerkB",  PROC_NPC_CLERKB,        0x00),
    OBJNAME("clerkT",  PROC_NPC_CLERKT,        0x00),
    OBJNAME("Ash",     PROC_NPC_ASH,           -1),
    OBJNAME("AshB",    PROC_NPC_ASHB,          -1),
    OBJNAME("Shad",    PROC_NPC_SHAD,          -1),
    OBJNAME("Rafrel",  PROC_NPC_RAFREL,        -1),
    OBJNAME("MoiR",    PROC_NPC_MOIR,          -1),
    OBJNAME("impal",   PROC_NPC_IMPAL,         -1),
    OBJNAME("Coach",   PROC_NPC_COACH,         -1),
    OBJNAME("TheB",    PROC_NPC_THEB,          -1),
    OBJNAME("TgArena", PROC_Tag_Arena,         -1),
    OBJNAME("TagInst", PROC_Tag_Instruction,   -1),
    OBJNAME("GWolf",   PROC_NPC_GWOLF,         -1),
    OBJNAME("Len",     PROC_NPC_LEN,           -1),
    OBJNAME("Lud",     PROC_NPC_LUD,           -1),
    OBJNAME("Doc",     PROC_NPC_DOC,           -1),
    OBJNAME("FSeirei", PROC_NPC_FAIRY_SEIREI,  -1),
    OBJNAME("Fairy",   PROC_NPC_FAIRY,         -1),
    OBJNAME("Hanjo",   PROC_NPC_HANJO,         -1),
    OBJNAME("Hoz",     PROC_NPC_HOZ,           -1),
    OBJNAME("Jagar",   PROC_NPC_JAGAR,         -1),
    OBJNAME("Kkri",    PROC_NPC_KKRI,          -1),
    OBJNAME("Kn",      PROC_NPC_KN,            -1),
    OBJNAME("KnBlt",   PROC_KN_BULLET,         -1),
    OBJNAME("Knj",     PROC_NPC_KNJ,           -1),
    OBJNAME("Kolin",   PROC_NPC_KOLIN,         -1),
    OBJNAME("Kolinb",  PROC_NPC_KOLINB,        -1),
    OBJNAME("Kyury",   PROC_NPC_KYURY,         -1),
    OBJNAME("Maro",    PROC_NPC_MARO,          -1),
    OBJNAME("midP",    PROC_NPC_MIDP,          -1),
    OBJNAME("Moi",     PROC_NPC_MOI,           -1),
    OBJNAME("Raca",    PROC_NPC_RACA,          -1),
    OBJNAME("Post",    PROC_NPC_POST,          -1),
    OBJNAME("Pouya",   PROC_NPC_POUYA,         -1),
    OBJNAME("Saru",    PROC_NPC_SARU,          -1),
    OBJNAME("seiB",    PROC_NPC_SEIB,          -1),
    OBJNAME("seiC",    PROC_NPC_SEIC,          -1),
    OBJNAME("seiD",    PROC_NPC_SEID,          -1),
    OBJNAME("Seira",   PROC_NPC_SEIRA,         -1),
    OBJNAME("Seira2",  PROC_NPC_SERA2,         -1),
    OBJNAME("Seirei",  PROC_NPC_SEIREI,        -1),
    OBJNAME("Sha",     PROC_NPC_SHAMAN,        -1),
    OBJNAME("sMaro",   PROC_NPC_SMARO,         -1),
    OBJNAME("solA",    PROC_NPC_SOLA,          -1),
    OBJNAME("Taro",    PROC_NPC_TARO,          -1),
    OBJNAME("PA_Besu", PROC_NPC_PACHI_BESU,    -1),
    OBJNAME("PA_Taro", PROC_NPC_PACHI_TARO,    -1),
    OBJNAME("PA_Maro", PROC_NPC_PACHI_MARO,    -1),
    OBJNAME("TagPati", PROC_TAG_PATI,          -1),
    OBJNAME("The",     PROC_NPC_THE,           -1),
    OBJNAME("Tkj",     PROC_NPC_TKJ,           -1),
    OBJNAME("Tks",     PROC_NPC_TKS,           -1),
    OBJNAME("ObjTks",  PROC_OBJ_TKS,           -1),
    OBJNAME("Tkc",     PROC_NPC_TKC,           -1),
    OBJNAME("Tkj2",    PROC_NPC_TKJ2,          -1),
    OBJNAME("Toby",    PROC_NPC_TOBY,          -1),
    OBJNAME("Uri",     PROC_NPC_URI,           -1),
    OBJNAME("Yelia",   PROC_NPC_YELIA,         -1),
    OBJNAME("ykM",     PROC_NPC_YKM,           -1),
    OBJNAME("ykW",     PROC_NPC_YKW,           -1),
    OBJNAME("zanB",    PROC_NPC_ZANB,          -1),
    OBJNAME("Zant",    PROC_NPC_ZANT,          -1),
    OBJNAME("Zelda",   PROC_NPC_ZELDA,         -1),
    OBJNAME("ZelR",    PROC_NPC_ZELR,          -1),
    OBJNAME("ZelRo",   PROC_NPC_ZELRO,         -1),
    OBJNAME("zrA",     PROC_NPC_ZRA,           0x00),
    OBJNAME("zrS",     PROC_NPC_ZRA,           0x01),
    OBJNAME("zrWF",    PROC_NPC_ZRA,           0x02),
    OBJNAME("zrR",     PROC_NPC_ZRA,           0x03),
    OBJNAME("zrD",     PROC_NPC_ZRA,           0x04),
    OBJNAME("zrSP",    PROC_NPC_ZRA,           0x05),
    OBJNAME("zrSPA",   PROC_NPC_ZRA,           0x06),
    OBJNAME("zrF",     PROC_OBJ_ZRAFREEZE,     0x00),
    OBJNAME("zrF2",    PROC_OBJ_ZRAFREEZE,     0x01),
    OBJNAME("zrF3",    PROC_OBJ_ZRAFREEZE,     0x02),
    OBJNAME("zrC",     PROC_NPC_ZRC,           -1),
    OBJNAME("zrZ",     PROC_NPC_ZRZ,           -1),
    OBJNAME("GrvStn",  PROC_Obj_GraveStone,    -1),
    OBJNAME("zrARock", PROC_Obj_ZraRock,       -1),
    OBJNAME("StAndGl", PROC_START_AND_GOAL,    -1),
    OBJNAME("zrAMark", PROC_ZRA_MARK,          -1),
    OBJNAME("myna2",   PROC_MYNA2,             -1),
    OBJNAME("myn2tag", PROC_TAG_MYNA2,         -1),
    OBJNAME("MAN_a",   PROC_NPC_CD3,           0x00),
    OBJNAME("MAD_a",   PROC_NPC_CD3,           0x01),
    OBJNAME("MCN_a",   PROC_NPC_CD3,           0x02),
    OBJNAME("MON_a",   PROC_NPC_CD3,           0x03),
    OBJNAME("MAN_b",   PROC_NPC_CD3,           0x04),
    OBJNAME("MAN_c",   PROC_NPC_CD3,           0x05),
    OBJNAME("MAS_a",   PROC_NPC_CD3,           0x06),
    OBJNAME("MBN_a",   PROC_NPC_CD3,           0x07),
    OBJNAME("MAN_a2",  PROC_NPC_CD3,           0x08),
    OBJNAME("MAD_a2",  PROC_NPC_CD3,           0x09),
    OBJNAME("MCN_a2",  PROC_NPC_CD3,           0x0A),
    OBJNAME("MON_a2",  PROC_NPC_CD3,           0x0B),
    OBJNAME("MAN_b2",  PROC_NPC_CD3,           0x0C),
    OBJNAME("MAN_c2",  PROC_NPC_CD3,           0x0D),
    OBJNAME("MAS_a2",  PROC_NPC_CD3,           0x0E),
    OBJNAME("MBN_a2",  PROC_NPC_CD3,           0x0F),
    OBJNAME("WAN_a",   PROC_NPC_CD3,           0x10),
    OBJNAME("WAD_a",   PROC_NPC_CD3,           0x11),
    OBJNAME("MAT_a",   PROC_NPC_CD3,           0x12),
    OBJNAME("WCN_a",   PROC_NPC_CD3,           0x13),
    OBJNAME("WON_a",   PROC_NPC_CD3,           0x14),
    OBJNAME("WGN_a",   PROC_NPC_CD3,           0x15),
    OBJNAME("WAN_b",   PROC_NPC_CD3,           0x16),
    OBJNAME("WAN_a2",  PROC_NPC_CD3,           0x17),
    OBJNAME("WAD_a2",  PROC_NPC_CD3,           0x18),
    OBJNAME("MAT_a2",  PROC_NPC_CD3,           0x19),
    OBJNAME("WCN_a2",  PROC_NPC_CD3,           0x1A),
    OBJNAME("WON_a2",  PROC_NPC_CD3,           0x1B),
    OBJNAME("WGN_a2",  PROC_NPC_CD3,           0x1C),
    OBJNAME("WAN_b2",  PROC_NPC_CD3,           0x1D),
    OBJNAME("shoe",    PROC_NPC_SHOE,          -1),
    OBJNAME("DoorBoy", PROC_NPC_DOORBOY,       -1),
    OBJNAME("prayer",  PROC_NPC_PRAYER,        -1),
    OBJNAME("km_Hana", PROC_NPC_KASIHANA,      -1),
    OBJNAME("km_Kyu",  PROC_NPC_KASIKYU,       -1),
    OBJNAME("km_Mich", PROC_NPC_KASIMICH,      -1),
    OBJNAME("NpcChat", PROC_NPC_CHAT,          -1),
    OBJNAME("chtSolA", PROC_NPC_SOLDIERa,      -1),
    OBJNAME("chtSolB", PROC_NPC_SOLDIERb,      -1),
    OBJNAME("DrSol1",  PROC_NPC_DRSOL,         0x00),
    OBJNAME("DrSol2",  PROC_NPC_DRSOL,         0x01),
    OBJNAME("chin",    PROC_NPC_CHIN,          -1),
    OBJNAME("ins",     PROC_NPC_INS,           -1),
    OBJNAME("Shop0",   PROC_NPC_SHOP0,         -1),
    OBJNAME("TagSch",  PROC_Tag_Schedule,      -1),
    OBJNAME("TagEsc",  PROC_Tag_Escape,        -1),
    OBJNAME("passer",  PROC_PASSER_MNG,        0x00),
    OBJNAME("passerL", PROC_PASSER_MNG,        0x01),
    OBJNAME("fmtion",  PROC_FORMATION_MNG,     0x00),
    OBJNAME("fmtionL", PROC_FORMATION_MNG,     0x01),
    OBJNAME("guard",   PROC_GUARD_MNG,         -1),
    OBJNAME("TgGuard", PROC_TAG_GUARD,         -1),
    OBJNAME("Mk",      PROC_NPC_MK,            -1),
    OBJNAME("Obj_Tbi", PROC_Obj_Yobikusa,      0x00),
    OBJNAME("Obj_Uma", PROC_Obj_Yobikusa,      0x01),
    OBJNAME("Obj_knk", PROC_Obj_KazeNeko,      -1),
    OBJNAME("Obj_nmp", PROC_Obj_NamePlate,     -1),
    OBJNAME("Obj_Tie", PROC_Obj_OnCloth,       -1),
    OBJNAME("Obj_Lrp", PROC_Obj_LndRope,       -1),
    OBJNAME("Obj_Sit", PROC_Obj_ItaRope,       -1),
    OBJNAME("Ikada",   PROC_Obj_Ikada,         -1),
    OBJNAME("Dust",    PROC_Obj_DUST,          -1),
    OBJNAME("Obj_Ita", PROC_Obj_ITA,           -1),
    OBJNAME("Ice_l",   PROC_Obj_Ice_l,         -1),
    OBJNAME("Ice_s",   PROC_Obj_Ice_s,         -1),
    OBJNAME("E_Make",  PROC_Obj_E_CREATE,      -1),
    OBJNAME("Bhhashi", PROC_Obj_Bhbridge,      -1),
    OBJNAME("Yousei",  PROC_Obj_Yousei,        -1),
    OBJNAME("G_Kabe",  PROC_Obj_GOMIKABE,      -1),
    OBJNAME("B_ling",  PROC_Obj_Mato,          -1),
    OBJNAME("kab_o",   PROC_Obj_Kabuto,        -1),
    OBJNAME("I_Cho",   PROC_Obj_Cho,           -1),
    OBJNAME("I_Kuw",   PROC_Obj_Kuw,           -1),
    OBJNAME("I_Nan",   PROC_Obj_Nan,           -1),
    OBJNAME("I_Dan",   PROC_Obj_Dan,           -1),
    OBJNAME("I_Kam",   PROC_Obj_Kam,           -1),
    OBJNAME("I_Ten",   PROC_Obj_Ten,           -1),
    OBJNAME("I_Ari",   PROC_Obj_Ari,           -1),
    OBJNAME("I_Kag",   PROC_Obj_Kag,           -1),
    OBJNAME("I_Tom",   PROC_Obj_Tombo,         -1),
    OBJNAME("I_Bat",   PROC_Obj_Batta,         -1),
    OBJNAME("I_Kat",   PROC_Obj_Kat,           -1),
    OBJNAME("H_Saku",  PROC_Obj_H_Saku,        -1),
    OBJNAME("sm_door", PROC_Obj_SM_DOOR,       -1),
    OBJNAME("Kaisou",  PROC_Obj_Kaisou,        -1),
    OBJNAME("Ni",      PROC_NI,                -1),
    OBJNAME("Fr",      PROC_FR,                -1),
    OBJNAME("Sq",      PROC_SQ,                -1),
    OBJNAME("Bd",      PROC_BD,                -1),
    OBJNAME("Obj_ki",  PROC_OBJ_KI,            -1),
    OBJNAME("Do",      PROC_DO,                -1),
    OBJNAME("Lm",      PROC_NPC_FISH,          0x00),
    OBJNAME("Tr",      PROC_NPC_FISH,          0x05),
    OBJNAME("Npc_tr",  PROC_NPC_TR,            -1),
    OBJNAME("Npc_lf",  PROC_NPC_LF,            -1),
    OBJNAME("Fish",    PROC_MG_FISH,           -1),
    OBJNAME("NPC_TK",  PROC_NPC_TK,            -1),
    OBJNAME("Obj_fd",  PROC_OBJ_FOOD,          -1),
    OBJNAME("MYNA",    PROC_MYNA,              -1),
    OBJNAME("SSITEM",  PROC_OBJ_SSITEM,        -1),
    OBJNAME("SSDRINK", PROC_OBJ_SSDRINK,       -1),
    OBJNAME("TGDRINK", PROC_TAG_SSDRINK,       -1),
    OBJNAME("Tag_Btl", PROC_TAG_BTLITM,        -1),
    OBJNAME("lv5soup", PROC_TAG_LV5SOUP,       -1),
    OBJNAME("TGMNLIG", PROC_TAG_MNLIGHT,       -1),
    OBJNAME("TGSPCAM", PROC_TAG_SHOPCAM,       -1),
    OBJNAME("TGSPITM", PROC_TAG_SHOPITM,       -1),
    OBJNAME("OilTubo", PROC_OBJ_OILTUBO,       -1),
    OBJNAME("Roten",   PROC_OBJ_ROTEN,         -1),
    OBJNAME("Pleaf",   PROC_OBJ_PLEAF,         -1),
    OBJNAME("KBacket", PROC_OBJ_KBACKET,       -1),
    OBJNAME("YBag",    PROC_OBJ_YBAG,          -1),
    OBJNAME("Pumpkin", PROC_OBJ_PUMPKIN,       -1),
    OBJNAME("Obj_kg",  PROC_OBJ_KAGO,          -1),
    OBJNAME("BbKago",  PROC_OBJ_KAGO,          0x00),
    OBJNAME("OnKago",  PROC_OBJ_KAGO,          0x01),
    OBJNAME("ObjHasi", PROC_Obj_HHASHI,        -1),
    OBJNAME("Thashi",  PROC_Obj_THASHI,        -1),
    OBJNAME("B_Hashi", PROC_Obj_BHASHI,        -1),
    OBJNAME("E_MD",    PROC_E_MD,              -1),
    OBJNAME("E_s1",    PROC_E_S1,              -1),
    OBJNAME("E_wb",    PROC_E_WB,              -1),
    OBJNAME("E_rd",    PROC_E_RD,              -1),
    OBJNAME("E_rdb",   PROC_E_RDB,             -1),
    OBJNAME("E_rdy",   PROC_E_RDY,             -1),
    OBJNAME("E_fm",    PROC_E_FM,              -1),
    OBJNAME("E_dn",    PROC_E_DN,              -1),
    OBJNAME("E_mf",    PROC_E_MF,              -1),
    OBJNAME("TreeSh",  PROC_TREESH,            -1),
    OBJNAME("E_bug",   PROC_E_BUG,             -1),
    OBJNAME("E_bee",   PROC_E_BEE,             -1),
    OBJNAME("E_nest",  PROC_E_NEST,            -1),
    OBJNAME("E_kr",    PROC_E_KR,              -1),
    OBJNAME("E_st",    PROC_E_ST,              -1),
    OBJNAME("E_tk",    PROC_E_TK,              -1),
    OBJNAME("E_tk2",   PROC_E_TK2,             -1),
    OBJNAME("E_th",    PROC_E_TH,              -1),
    OBJNAME("E_cr",    PROC_E_CR,              -1),
    OBJNAME("E_df",    PROC_E_DF,              -1),
    OBJNAME("E_ph",    PROC_E_PH,              -1),
    OBJNAME("E_fs",    PROC_E_FS,              -1),
    OBJNAME("E_SB",    PROC_E_SB,              -1),
    OBJNAME("E_HM",    PROC_E_HM,              -1),
    OBJNAME("E_fk",    PROC_E_FK,              -1),
    OBJNAME("E_pm",    PROC_E_PM,              -1),
    OBJNAME("Npc_ne",  PROC_NPC_NE,            -1),
    OBJNAME("E_db",    PROC_E_DB,              -1),
    OBJNAME("E_kg",    PROC_E_KG,              -1),
    OBJNAME("E_sh",    PROC_E_SH,              -1),
    OBJNAME("E_sf",    PROC_E_SF,              -1),
    OBJNAME("Obj_lb",  PROC_OBJ_LBOX,          -1),
    OBJNAME("Obj_kb",  PROC_OBJ_KBOX,          -1),
    OBJNAME("E_bs",    PROC_E_BS,              -1),
    OBJNAME("Sekizoa", PROC_OBJ_SEKIZOA,       0x00),
    OBJNAME("Sekizob", PROC_OBJ_SEKIZOA,       0x01),
    OBJNAME("E_sm",    PROC_E_SM,              -1),
    OBJNAME("E_sm2",   PROC_E_SM2,             -1),
    OBJNAME("ky_tag0", PROC_KYTAG00,           -1),
    OBJNAME("ky_tag1", PROC_KYTAG01,           0x00),
    OBJNAME("ky_tag2", PROC_KYTAG02,           -1),
    OBJNAME("ky_tag3", PROC_KYTAG01,           0x01),
    OBJNAME("kytag3",  PROC_KYTAG03,           -1),
    OBJNAME("DK_tag",  PROC_KYTAG04,           -1),
    OBJNAME("PPHole",  PROC_KYTAG05,           -1),
    OBJNAME("KY_chg",  PROC_KYTAG06,           -1),
    OBJNAME("Plight",  PROC_KYTAG07,           -1),
    OBJNAME("KY_mk",   PROC_KYTAG08,           -1),
    OBJNAME("Bound",   PROC_KYTAG09,           -1),
    OBJNAME("WC_tag",  PROC_KYTAG01,           0x02),
    OBJNAME("Sparks",  PROC_KYTAG10,           -1),
    OBJNAME("Tcntrl",  PROC_KYTAG11,           -1),
    OBJNAME("Drkmst",  PROC_KYTAG12,           -1),
    OBJNAME("Blsnow",  PROC_KYTAG13,           -1),
    OBJNAME("Savmem",  PROC_KYTAG14,           -1),
    OBJNAME("Zshake",  PROC_KYTAG15,           -1),
    OBJNAME("Pikari",  PROC_KYTAG16,           -1),
    OBJNAME("Lmask",   PROC_KYTAG17,           -1),
    OBJNAME("E_is",    PROC_E_IS,              -1),
    OBJNAME("E_ai",    PROC_E_AI,              -1),
    OBJNAME("E_ba",    PROC_E_BA,              -1),
    OBJNAME("E_bu",    PROC_E_BU,              -1),
    OBJNAME("E_sw",    PROC_E_SW,              -1),
    OBJNAME("E_ge",    PROC_E_GE,              -1),
    OBJNAME("TagGe",   PROC_Tag_WatchGe,       -1),
    OBJNAME("E_ym",    PROC_E_YM,              -1),
    OBJNAME("E_ymt",   PROC_E_YM_TAG,          -1),
    OBJNAME("E_ymb",   PROC_E_YMB,             -1),
    OBJNAME("Fwall",   PROC_Tag_FWall,         -1),
    OBJNAME("E_hz",    PROC_E_HZ,              -1),
    OBJNAME("E_ws",    PROC_E_WS,              -1),
    OBJNAME("E_oc",    PROC_E_OC,              -1),
    OBJNAME("E_dt",    PROC_E_DT,              -1),
    OBJNAME("E_bg",    PROC_E_BG,              -1),
    OBJNAME("E_octbg", PROC_E_OctBg,           -1),
    OBJNAME("E_tt",    PROC_E_TT,              -1),
    OBJNAME("E_dk",    PROC_E_DK,              -1),
    OBJNAME("E_vt",    PROC_E_VT,              -1),
    OBJNAME("E_ww",    PROC_E_WW,              -1),
    OBJNAME("E_gi",    PROC_E_GI,              -1),
    OBJNAME("E_mk",    PROC_E_MK,              -1),
    OBJNAME("B_gm",    PROC_B_GM,              -1),
    OBJNAME("E_gm",    PROC_E_GM,              -1),
    OBJNAME("E_gs",    PROC_E_GS,              -1),
    OBJNAME("Npc_ne",  PROC_NPC_NE,            -1),
    OBJNAME("Obj_gm",  PROC_OBJ_GM,            -1),
    OBJNAME("E_mm",    PROC_E_MM,              -1),
    OBJNAME("E_mm2",   PROC_E_MM,              0x01),
    OBJNAME("E_fz",    PROC_E_FZ,              -1),
    OBJNAME("E_fb",    PROC_E_FB,              -1),
    OBJNAME("E_Zs",    PROC_E_ZS,              -1),
    OBJNAME("E_kk",    PROC_E_KK,              -1),
    OBJNAME("E_hp",    PROC_E_HP,              -1),
    OBJNAME("E_zh",    PROC_E_ZH,              -1),
    OBJNAME("E_zm",    PROC_E_ZM,              -1),
    OBJNAME("E_zm2",   PROC_E_ZM,              0x01),
    OBJNAME("E_pz",    PROC_E_PZ,              -1),
    OBJNAME("Kita",    PROC_OBJ_KITA,          -1),
    OBJNAME("E_nz",    PROC_E_NZ,              -1),
    OBJNAME("E_po",    PROC_E_PO,              -1),
    OBJNAME("Obj_w0",  PROC_OBJ_WEB0,          -1),
    OBJNAME("Obj_w1",  PROC_OBJ_WEB1,          -1),
    OBJNAME("Obj_cb",  PROC_OBJ_CB,            -1),
    OBJNAME("Obj_mak", PROC_OBJ_MAKI,          -1),
    OBJNAME("Obj_brg", PROC_OBJ_BRG,           -1),
    OBJNAME("E_rb",    PROC_E_RB,              -1),
    OBJNAME("E_sg",    PROC_E_SG,              -1),
    OBJNAME("Obj_tby", PROC_OBJ_TOBY,          -1),
    OBJNAME("Obj_sw",  PROC_OBJ_SW,            -1),
    OBJNAME("Npc_ks",  PROC_NPC_KS,            -1),
    OBJNAME("Obj_so",  PROC_OBJ_SO,            -1),
    OBJNAME("E_bi",    PROC_E_BI,              -1),
    OBJNAME("Obj_key", PROC_OBJ_KEY,           -1),
    OBJNAME("E_hb",    PROC_E_HB,              -1),
    OBJNAME("Obj_hb",  PROC_OBJ_HB,            -1),
    OBJNAME("B_bq",    PROC_B_BQ,              -1),
    OBJNAME("B_bh",    PROC_B_BH,              -1),
    OBJNAME("B_go",    PROC_B_GO,              -1),
    OBJNAME("Obj_fw",  PROC_OBJ_FW,            -1),
    OBJNAME("E_ga",    PROC_E_GA,              -1),
    OBJNAME("E_gb",    PROC_E_GB,              -1),
    OBJNAME("E_wap",   PROC_E_WAP,             -1),
    OBJNAME("Obj_sui", PROC_OBJ_SUISYA,        -1),
    OBJNAME("Obj_bal", PROC_OBJ_BALLOON,       -1),
    OBJNAME("Obj_lp",  PROC_OBJ_LP,            -1),
    OBJNAME("Obj_nd",  PROC_OBJ_NDOOR,         -1),
    OBJNAME("Obj_ud",  PROC_OBJ_UDOOR,         -1),
    OBJNAME("Obj_us",  PROC_OBJ_USAKU,         -1),
    OBJNAME("Obj_kn2", PROC_OBJ_KANBAN2,       -1),
    OBJNAME("Obj_kyh", PROC_OBJ_KEYHOLE,       -1),
    OBJNAME("Bed",     PROC_OBJ_BED,           -1),
    OBJNAME("BouMato", PROC_OBJ_BOUMATO,       -1),
    OBJNAME("AutoMt",  PROC_OBJ_AUTOMATA,      -1),
    OBJNAME("Gadget",  PROC_OBJ_GADGET,        -1),
    OBJNAME("ItaMato", PROC_OBJ_ITAMATO,       -1),
    OBJNAME("Nougu",   PROC_OBJ_NOUGU,         -1),
    OBJNAME("Stick",   PROC_OBJ_STICK,         -1),
    OBJNAME("Mie",     PROC_OBJ_MIE,           -1),
    OBJNAME("SkDoor",  PROC_OBJ_SEKIDOOR,      -1),
    OBJNAME("Sekizo",  PROC_OBJ_SEKIZO,        -1),
    OBJNAME("SMTile",  PROC_OBJ_SMTILE,        -1),
    OBJNAME("H_kage",  PROC_OBJ_KAGE,          -1),
    OBJNAME("E_ms",    PROC_E_MS,              -1),
    OBJNAME("Obj_tp",  PROC_OBJ_TP,            -1),
    OBJNAME("Obj_ms",  PROC_OBJ_MSIMA,         -1),
    OBJNAME("E_gob",   PROC_E_GOB,             -1),
    OBJNAME("E_yk",    PROC_E_YK,              -1),
    OBJNAME("E_yr",    PROC_E_YR,              -1),
    OBJNAME("E_yg",    PROC_E_YG,              -1),
    OBJNAME("Obj_fm",  PROC_OBJ_FMOBJ,         -1),
    OBJNAME("B_ob",    PROC_B_OB,              -1),
    OBJNAME("OcHashi", PROC_OCTHASHI,          -1),
    OBJNAME("E_dd",    PROC_E_DD,              -1),
    OBJNAME("Obj_rw",  PROC_OBJ_RW,            -1),
    OBJNAME("Obj_ih",  PROC_OBJ_IHASI,         -1),
    OBJNAME("E_yc",    PROC_E_YC,              -1),
    OBJNAME("B_ds",    PROC_B_DS,              -1),
    OBJNAME("B_dr",    PROC_B_DR,              -1),
    OBJNAME("B_gg",    PROC_B_GG,              -1),
    OBJNAME("B_yo",    PROC_B_YO,              -1),
    OBJNAME("B_tn",    PROC_B_TN,              -1),
    OBJNAME("B_zant",  PROC_B_ZANT,            -1),
    OBJNAME("B_zntm",  PROC_B_ZANTZ,           -1),
    OBJNAME("B_znts",  PROC_B_ZANTS,           -1),
    OBJNAME("E_yd",    PROC_E_YD,              -1),
    OBJNAME("E_yh",    PROC_E_YH,              -1),
    OBJNAME("Henna",   PROC_NPC_HENNA,         -1),
    OBJNAME("Henna0",  PROC_NPC_HENNA0,        -1),
    OBJNAME("Npc_du",  PROC_NPC_DU,            0x00),
    OBJNAME("Econt",   PROC_ECONT,             -1),
    OBJNAME("B_mgn",   PROC_B_MGN,             -1),
    OBJNAME("Hzelda",  PROC_E_HZELDA,          -1),
    OBJNAME("Worm",    PROC_NPC_WORM,          -1),
    OBJNAME("B_gnd",   PROC_B_GND,             -1),
    OBJNAME("Obj_gb",  PROC_OBJ_GB,            -1),
    OBJNAME("tboxA0",  PROC_TBOX,              -1),
    OBJNAME("tboxA1",  PROC_TBOX,              -1),
    OBJNAME("tboxA2",  PROC_TBOX,              -1),
    OBJNAME("tboxB0",  PROC_TBOX,              -1),
    OBJNAME("tboxB1",  PROC_TBOX,              -1),
    OBJNAME("tboxB2",  PROC_TBOX,              -1),
    OBJNAME("tboxJ0",  PROC_TBOX,              -1),
    OBJNAME("tboxP0",  PROC_TBOX,              -1),
    OBJNAME("tboxB00", PROC_TBOX,              -1),
    OBJNAME("tboxB01", PROC_TBOX,              -1),
    OBJNAME("tboxW0",  PROC_TBOX,              -1),
    OBJNAME("tboxEL0", PROC_TBOX2,             -1),
    OBJNAME("tboxEL1", PROC_TBOX2,             -1),
    OBJNAME("l8warp",  PROC_Obj_BossWarp,      -1),
    OBJNAME("L1Bdoor", PROC_L1BOSS_DOOR,       -1),
    OBJNAME("L2Bdoor", PROC_L1BOSS_DOOR,       -1),
    OBJNAME("L3Bdoor", PROC_Obj_Kshutter,      -1),
    OBJNAME("L4Bdoor", PROC_L1BOSS_DOOR,       -1),
    OBJNAME("L5Bdoor", PROC_L5BOSS_DOOR,       -1),
    OBJNAME("L6Bdoor", PROC_L1BOSS_DOOR,       -1),
    OBJNAME("L7Bdoor", PROC_L1BOSS_DOOR,       -1),
    OBJNAME("L8Bdoor", PROC_L1BOSS_DOOR,       -1),
    OBJNAME("L9Bdoor", PROC_L1BOSS_DOOR,       -1),
    OBJNAME("L1Mdoor", PROC_L1MBOSS_DOOR,      0x01),
    OBJNAME("L2Mdoor", PROC_L1MBOSS_DOOR,      0x02),
    OBJNAME("L3Mdoor", PROC_L1MBOSS_DOOR,      0x03),
    OBJNAME("L4Mdoor", PROC_L1MBOSS_DOOR,      0x04),
    OBJNAME("L5Mdoor", PROC_L1MBOSS_DOOR,      0x05),
    OBJNAME("L6Mdoor", PROC_L1MBOSS_DOOR,      0x06),
    OBJNAME("L7Mdoor", PROC_L1MBOSS_DOOR,      0x07),
    OBJNAME("L8Mdoor", PROC_L1MBOSS_DOOR,      0x08),
    OBJNAME("L7door",  PROC_L1MBOSS_DOOR,      0x0A),
    OBJNAME("L5door",  PROC_L1MBOSS_DOOR,      0x0B),
    OBJNAME("Horse",   PROC_HORSE,             -1),
    OBJNAME("Canoe",   PROC_CANOE,             -1),
    OBJNAME("Crope",   PROC_Obj_Crope,         -1),
    OBJNAME("Cstatue", PROC_CSTATUE,           -1),
    OBJNAME("CstaF",   PROC_CSTAF,             -1),
    OBJNAME("HoZelda", PROC_HOZELDA,           -1),
    OBJNAME("Mhint",   PROC_Tag_Mhint,         -1),
    OBJNAME("Mmsg",    PROC_Tag_Mmsg,          -1),
    OBJNAME("Mwait",   PROC_Tag_Mwait,         -1),
    OBJNAME("Mstop",   PROC_Tag_Mstop,         -1),
    OBJNAME("MstopE",  PROC_Tag_Mstop,         -1),
    OBJNAME("Hinit",   PROC_Tag_Hinit,         -1),
    OBJNAME("Hjump",   PROC_Tag_Hjump,         -1),
    OBJNAME("Hstop",   PROC_Tag_Hstop,         -1),
    OBJNAME("AJnot",   PROC_Tag_AJnot,         -1),
    OBJNAME("Wljump",  PROC_Tag_Wljump,        -1),
    OBJNAME("Sppath",  PROC_Tag_Sppath,        -1),
    OBJNAME("Stream",  PROC_Tag_Stream,        -1),
    OBJNAME("Gstart",  PROC_Tag_Gstart,        -1),
    OBJNAME("TgTGate", PROC_Tag_TWGate,        -1),
    OBJNAME("Lv6Gate", PROC_Tag_Lv6Gate,       -1),
    OBJNAME("Lv7Gate", PROC_Tag_Lv7Gate,       -1),
    OBJNAME("Lv8Gate", PROC_Tag_Lv8Gate,       -1),
    OBJNAME("TgTHint", PROC_Tag_TheBHint,      -1),
    OBJNAME("TagAsst", PROC_Tag_Assist,        -1),
    OBJNAME("Dmidna",  PROC_DMIDNA,            -1),
    OBJNAME("Fchain",  PROC_Obj_Fchain,        -1),
    OBJNAME("Wchain",  PROC_Obj_Wchain,        -1),
    OBJNAME("Cdoor",   PROC_Obj_Cdoor,         -1),
    OBJNAME("Cgate",   PROC_Obj_Cdoor,         -1),
    OBJNAME("Water00", PROC_GRDWATER,          -1),
    OBJNAME("Digpl",   PROC_Obj_Digpl,         -1),
    OBJNAME("Digholl", PROC_Obj_Digholl,       -1),
    OBJNAME("Bombf",   PROC_Obj_Bombf,         -1),
    OBJNAME("hider",   PROC_SUSPEND,           -1),
    OBJNAME("SwAreaC", PROC_SWC00,             -1),
    OBJNAME("SwAreaS", PROC_SWC00,             -1),
    OBJNAME("SwLBall", PROC_SwLBall,           -1),
    OBJNAME("SwBall",  PROC_SwBall,            -1),
    OBJNAME("thouse",  PROC_Obj_TobyHouse,     -1),
    OBJNAME("ironbox", PROC_Obj_MetalBox,      -1),
    OBJNAME("l6swtrn", PROC_Obj_Lv6SwTurn,     -1),
    OBJNAME("l2bmtag", PROC_Tag_Lv2PrChk,      -1),
    OBJNAME("swspin",  PROC_Obj_SwSpinner,     -1),
    OBJNAME("spnGear", PROC_Obj_Lv4Gear,       -1),
    OBJNAME("fan",     PROC_Obj_Fan,           -1),
    OBJNAME("tgake",   PROC_Obj_Gake,          -1),
    OBJNAME("tmoon",   PROC_Obj_TMoon,         -1),
    OBJNAME("gwall",   PROC_Obj_GanonWall,     -1),
    OBJNAME("gwall2",  PROC_Obj_GanonWall2,    -1),
    OBJNAME("scnChg",  PROC_SCENE_EXIT,        -1),
    OBJNAME("scnChg2", PROC_SCENE_EXIT2,       -1),
    OBJNAME("noChgRm", PROC_NO_CHG_ROOM,       -1),
    OBJNAME("readRm",  PROC_NO_CHG_ROOM,       0x00),
    OBJNAME("Tetd",    PROC_DSHUTTER,          -1),
    OBJNAME("fence",   PROC_DSHUTTER,          -1),
    OBJNAME("lv7saku", PROC_DSHUTTER,          -1),
    OBJNAME("YkgrON",  PROC_Ykgr,              -1),
    OBJNAME("YkgrOFF", PROC_Ykgr,              -1),
    OBJNAME("TagEv",   PROC_TAG_EVENT,         -1),
    OBJNAME("TagEvC",  PROC_TAG_EVENT,         -1),
    OBJNAME("TagEvt",  PROC_TAG_EVT,           0x00),
    OBJNAME("TagEvt1", PROC_TAG_EVT,           0x01),
    OBJNAME("TagHowl", PROC_TAG_HOWL,          0x01),
    OBJNAME("Tag_ms",  PROC_TAG_MSG,           0x00),
    OBJNAME("LntnTag", PROC_TAG_LANTERN,       -1),
    OBJNAME("EvtArea", PROC_TAG_EVTAREA,       -1),
    OBJNAME("AllMato", PROC_TAG_ALLMATO,       -1),
    OBJNAME("EvtMsg",  PROC_TAG_EVTMSG,        -1),
    OBJNAME("KMsg",    PROC_TAG_KMSG,          -1),
    OBJNAME("Push",    PROC_TAG_PUSH,          -1),
    OBJNAME("Teloper", PROC_TAG_TELOP,         -1),
    OBJNAME("CamChg",  PROC_TAG_CAMERA,        -1),
    OBJNAME("CamArea", PROC_TAG_CAMERA,        -1),
    OBJNAME("CamAreC", PROC_TAG_CAMERA,        -1),
    OBJNAME("ChkPntS", PROC_TAG_CHKPOINT,      -1),
    OBJNAME("ChkPntC", PROC_TAG_CHKPOINT,      -1),
    OBJNAME("setball", PROC_Tag_SetBall,       -1),
    OBJNAME("atkItem", PROC_Tag_AttackItem,    -1),
    OBJNAME("TagStat", PROC_Tag_Statue,        -1),
    OBJNAME("zdoor",   PROC_Obj_ZDoor,         -1),
    OBJNAME("rdoor",   PROC_Obj_ZDoor,         -1),
    OBJNAME("STDoa",   PROC_Obj_ZDoor,         -1),
    OBJNAME("hasi00",  PROC_Obj_Pillar,        -1),
    OBJNAME("hasi01",  PROC_Obj_Pillar,        -1),
    OBJNAME("window",  PROC_Obj_Window,        -1),
    OBJNAME("bbox",    PROC_Obj_BBox,          -1),
    OBJNAME("spiralC", PROC_Obj_Lv3R10Saka,    -1),
    OBJNAME("spiral2", PROC_Obj_Lv3R10Saka,    -1),
    OBJNAME("digsnow", PROC_Obj_DigSnow,       -1),
    OBJNAME("digsand", PROC_Obj_Lv4DigSand,    -1),
    OBJNAME("fallobj", PROC_Obj_FallObj,       -1),
    OBJNAME("l7brg",   PROC_Obj_Lv7Bridge,     -1),
    OBJNAME("dr",      PROC_DR,                -1),
    OBJNAME("L7lowDr", PROC_L7lowDr,           -1),
    OBJNAME("swTime",  PROC_SwTime,            -1),
    OBJNAME("pdrobj",  PROC_Obj_PushDoor,      -1),
    OBJNAME("crystal", PROC_Obj_Crystal,       -1),
    OBJNAME("WarpBrg", PROC_Obj_KakarikoBrg,   -1),
    OBJNAME("WarpB2",  PROC_Obj_KakarikoBrg,   -1),
    OBJNAME("WarpOB1", PROC_Obj_OrdinBrg,      0x00),
    OBJNAME("WarpOB2", PROC_Obj_OrdinBrg,      0x01),
    OBJNAME("VolcGnd", PROC_Obj_VolcGnd,       -1),
    OBJNAME("r09wtr",  PROC_Obj_WaterEff,      -1),
    OBJNAME("ballSw",  PROC_Obj_SwBallA,       -1),
    OBJNAME("lballSw", PROC_Obj_SwBallB,       -1),
    OBJNAME("lbsw",    PROC_Obj_SwBallC,       -1),
    OBJNAME("R50Sand", PROC_Obj_Lv4Sand,       -1),
    OBJNAME("rwall",   PROC_Obj_Lv4RailWall,   -1),
    OBJNAME("l4brg",   PROC_Obj_Lv4Bridge,     -1),
    OBJNAME("l4floor", PROC_Obj_Lv4Floor,      -1),
    OBJNAME("item",    PROC_ITEM,              -1),
    OBJNAME("itemKey", PROC_Obj_SmallKey,      -1),
    OBJNAME("kantera", PROC_Obj_Kantera,       -1),
    OBJNAME("witem",   PROC_ITEM,              -1),
    OBJNAME("wshield", PROC_Obj_Shield,        -1),
    OBJNAME("htPiece", PROC_Obj_LifeContainer, -1),
    OBJNAME("htCase",  PROC_Obj_LifeContainer, -1),
    OBJNAME("Mbrid15", PROC_Obj_RotBridge,     -1),
    OBJNAME("Mbrid9",  PROC_Obj_RotBridge,     -1),
    OBJNAME("maglift", PROC_Obj_MagLift,       -1),
    OBJNAME("Cldst00", PROC_Obj_Lv1Cdl00,      -1),
    OBJNAME("Cldst01", PROC_Obj_Lv1Cdl01,      -1),
    OBJNAME("RMback0", PROC_Tag_Restart,       -1),
    OBJNAME("RMback1", PROC_Tag_ChgRestart,    -1),
    OBJNAME("Mist",    PROC_Tag_Mist,          -1),
    OBJNAME("magLifR", PROC_Obj_MagLiftRot,    -1),
    OBJNAME("TCdlst",  PROC_Obj_TvCdlst,       -1),
    OBJNAME("wdstick", PROC_Obj_WdStick,       -1),
    OBJNAME("stBlock", PROC_Obj_StairBlock,    -1),
    OBJNAME("hsMato",  PROC_Obj_HsTarget,      -1),
    OBJNAME("ktFire",  PROC_Tag_KtOnFire,      -1),
    OBJNAME("hswitch", PROC_Obj_HeavySw,       -1),
    OBJNAME("goGate",  PROC_Obj_GoGate,        -1),
    OBJNAME("taAmi",   PROC_Obj_TaFence,       -1),
    OBJNAME("fireWd",  PROC_Obj_FireWood,      -1),
    OBJNAME("fireWd2", PROC_Obj_FireWood2,     -1),
    OBJNAME("altar",   PROC_Obj_Saidan,        -1),
    OBJNAME("gpTaru",  PROC_Obj_GpTaru,        -1),
    OBJNAME("spinLf",  PROC_Obj_SpinLift,      -1),
    OBJNAME("bmWin",   PROC_Obj_BmWindow,      -1),
    OBJNAME("roofHl",  PROC_Obj_RfHole,        -1),
    OBJNAME("syRock",  PROC_Obj_SyRock,        -1),
    OBJNAME("wColumn", PROC_Obj_WaterPillar,   -1),
    OBJNAME("kpot",    PROC_Obj_KiPot,         -1),
    OBJNAME("bsGate",  PROC_Obj_BsGate,        -1),
    OBJNAME("amiShut", PROC_Obj_AmiShutter,    -1),
    OBJNAME("kwhel00", PROC_Obj_KWheel00,      -1),
    OBJNAME("kwhel01", PROC_Obj_KWheel01,      -1),
    OBJNAME("PRElvtr", PROC_Obj_PRElvtr,       -1),
    OBJNAME("swhel00", PROC_Obj_KWheel00,      -1),
    OBJNAME("M_hasu",  PROC_Obj_MHasu,         -1),
    OBJNAME("yiblltr", PROC_Obj_YIblltray,     -1),
    OBJNAME("l6egate", PROC_Obj_Lv6EGate,      -1),
    OBJNAME("l6eleva", PROC_Obj_Lv6ElevtA,     -1),
    OBJNAME("wtGate",  PROC_Obj_WtGate,        -1),
    OBJNAME("candlL2", PROC_Obj_Lv2Candle,     -1),
    OBJNAME("togeTp",  PROC_Obj_TogeTrap,      -1),
    OBJNAME("rotTrap", PROC_Obj_RotTrap,       -1),
    OBJNAME("klift00", PROC_Obj_KLift00,       -1),
    OBJNAME("l4chand", PROC_Obj_Lv4Chan,       -1),
    OBJNAME("Ychndlr", PROC_Obj_Ychndlr,       -1),
    OBJNAME("hbmbkoy", PROC_Obj_HBombkoya,     -1),
    OBJNAME("TagCsw",  PROC_TAG_CSW,           -1),
    OBJNAME("TagCswO", PROC_TAG_CSW,           -1),
    OBJNAME("P_Rwall", PROC_Obj_Lv4PRwall,     -1),
    OBJNAME("PDtile",  PROC_Obj_PDtile,        -1),
    OBJNAME("PDwall",  PROC_Obj_PDwall,        -1),
    OBJNAME("Qs",      PROC_TAG_QS,            -1),
    OBJNAME("mirror",  PROC_MIRROR,            -1),
    OBJNAME("rGate",   PROC_Obj_SwallShutter,  -1),
    OBJNAME("l3water", PROC_Obj_Lv3Water,      -1),
    OBJNAME("l3wat02", PROC_Obj_Lv3Water2,     -1),
    OBJNAME("l3watB",  PROC_OBJ_LV3WATERB,     -1),
    OBJNAME("szbridg", PROC_Obj_SZbridge,      -1),
    OBJNAME("kjgjs",   PROC_Obj_KJgjs,         -1),
    OBJNAME("kjs",     PROC_Obj_KJgjs,         -1),
    OBJNAME("candlL3", PROC_Obj_Lv3Candle,     -1),
    OBJNAME("l5icewl", PROC_Obj_IceWall,       -1),
    OBJNAME("Turara",  PROC_Obj_Turara,        -1),
    OBJNAME("twGate",  PROC_Obj_TwGate,        -1),
    OBJNAME("L4cdlTg", PROC_Tag_Lv4Candle,     -1),
    OBJNAME("L4cddTg", PROC_Tag_Lv4CandleDm,   -1),
    OBJNAME("togeRol", PROC_Obj_Lv6TogeRoll,   -1),
    OBJNAME("l6TogeT", PROC_Obj_Lv6TogeTrap,   -1),
    OBJNAME("Tenbin",  PROC_Obj_Lv6Tenbin,     -1),
    OBJNAME("l6Lblk",  PROC_Obj_Lv6Lblock,     -1),
    OBJNAME("l6SwGt",  PROC_Obj_Lv6SwGate,     -1),
    OBJNAME("l6ChBlk", PROC_Obj_Lv6ChgGate,    -1),
    OBJNAME("l6FuriT", PROC_Obj_Lv6FuriTrap,   -1),
    OBJNAME("KHdesk",  PROC_Obj_BarDesk,       -1),
    OBJNAME("Ytaihou", PROC_Obj_Ytaihou,       -1),
    OBJNAME("L4eShut", PROC_Obj_Lv4EdShutter,  -1),
    OBJNAME("pofire",  PROC_Obj_poFire,        -1),
    OBJNAME("pofiTg",  PROC_Tag_poFire,        -1),
    OBJNAME("poCandl", PROC_Obj_poCandle,      -1),
    OBJNAME("L4Gate",  PROC_Obj_Lv4Gate,       -1),
    OBJNAME("L4Pgate", PROC_Obj_Lv4PoGate,     -1),
    OBJNAME("L4SWall", PROC_Obj_Lv4SlideWall,  -1),
    OBJNAME("L4hmato", PROC_Obj_Lv4HsTarget,   -1),
    OBJNAME("propy",   PROC_Obj_Lv7PropY,      -1),
    OBJNAME("L7BsGt",  PROC_Obj_Lv7BsGate,     -1),
    OBJNAME("optLift", PROC_Obj_Lv8OptiLift,   -1),
    OBJNAME("kkiTrap", PROC_Obj_Lv8KekkaiTrap, -1),
    OBJNAME("L8LiftX", PROC_Obj_Lv8Lift,       -1),
    OBJNAME("swStep",  PROC_Obj_Lv8UdFloor,    -1),
    OBJNAME("L9SwSht", PROC_Obj_Lv9SwShutter,  -1),
    OBJNAME("L5SwIce", PROC_Obj_Lv5SwIce,      -1),
    OBJNAME("glwSph",  PROC_Obj_glowSphere,    -1),
    OBJNAME("MR_Scrw", PROC_Obj_MirrorScrew,   -1),
    OBJNAME("MR_Sand", PROC_Obj_MirrorSand,    -1),
    OBJNAME("MR_Tble", PROC_Obj_MirrorTable,   -1),
    OBJNAME("MR_Chin", PROC_Obj_MirrorChain,   -1),
    OBJNAME("MR_Pole", PROC_Obj_Mirror6Pole,   -1),
    OBJNAME("PPolamp", PROC_PPolamp,           -1),
    OBJNAME("l5hYuka", PROC_Obj_Lv5FBoard,     -1),
    OBJNAME("BYRock",  PROC_BkyRock,           -1),
    OBJNAME("zrDrock", PROC_Obj_zrTurara,      -1),
    OBJNAME("tkrDai",  PROC_Obj_TakaraDai,     -1),
    OBJNAME("Table",   PROC_Obj_Table,         -1),
    OBJNAME("CatDoor", PROC_Obj_CatDoor,       -1),
    OBJNAME("RetTag",  PROC_Tag_RetRoom,       -1),
    OBJNAME("WdStone", PROC_Obj_WindStone,     -1),
    OBJNAME("HwlWara", PROC_Tag_WaraHowl,      -1),
    OBJNAME("SCannon", PROC_Obj_SCannon,       -1),
    OBJNAME("szGate",  PROC_Obj_Lv6SzGate,     -1),
    OBJNAME("onsTaru", PROC_Obj_OnsenTaru,     -1),
    OBJNAME("WStoneF", PROC_Obj_SmWStone,      -1),
    OBJNAME("SCanCrs", PROC_Obj_SCannonCrs,    -1),
    OBJNAME("snwEfTg", PROC_Tag_SnowEff,       -1),
    OBJNAME("tmFire",  PROC_Obj_TimeFire,      -1),
    OBJNAME("L7Prop",  PROC_Obj_Prop,          -1),
    OBJNAME("awaPlar", PROC_Obj_awaPlar,       -1),
    OBJNAME("poTbox",  PROC_Obj_poTbox,        -1),
    OBJNAME("SnwSoup", PROC_Obj_SnowSoup,      -1),
    OBJNAME("Nagaisu", PROC_Obj_Nagaisu,       -1),
    OBJNAME("RCircle", PROC_Obj_RCircle,       -1),
    OBJNAME("L9Chand", PROC_Obj_Chandelier,    -1),
    OBJNAME("L9Pictr", PROC_Obj_Picture,       -1),
    OBJNAME("SmkEmt",  PROC_Tag_SmkEmt,        -1),
    OBJNAME("HFtr",    PROC_Obj_HFtr,          -1),
    OBJNAME("HBarrel", PROC_Obj_HBarrel,       -1),
    OBJNAME("SCanTen", PROC_Obj_SCannonTen,    -1),
    OBJNAME("cstaSw",  PROC_Tag_CstaSw,        -1),
    OBJNAME("l6cstSw", PROC_Tag_Lv6CstaSw,     -1),
    OBJNAME("Hata",    PROC_Obj_Hata,          -1),
    OBJNAME("RmbitSw", PROC_Tag_RmbitSw,       -1),
    OBJNAME("T_Maki",  PROC_Obj_ToaruMaki,     -1),
    OBJNAME("d_act",   PROC_DEMO00,            -1),
    OBJNAME("d_act0",  PROC_DEMO00,            0x00),
    OBJNAME("d_act1",  PROC_DEMO00,            0x01),
    OBJNAME("d_act2",  PROC_DEMO00,            0x02),
    OBJNAME("d_act3",  PROC_DEMO00,            0x03),
    OBJNAME("d_act4",  PROC_DEMO00,            0x04),
    OBJNAME("d_act5",  PROC_DEMO00,            0x05),
    OBJNAME("d_act6",  PROC_DEMO00,            0x06),
    OBJNAME("d_act7",  PROC_DEMO00,            0x07),
    OBJNAME("d_act8",  PROC_DEMO00,            0x08),
    OBJNAME("d_act9",  PROC_DEMO00,            0x09),
    OBJNAME("d_act10", PROC_DEMO00,            0x0A),
    OBJNAME("d_act11", PROC_DEMO00,            0x0B),
    OBJNAME("d_act12", PROC_DEMO00,            0x0C),
    OBJNAME("d_act13", PROC_DEMO00,            0x0D),
    OBJNAME("d_act14", PROC_DEMO00,            0x0E),
    OBJNAME("d_act15", PROC_DEMO00,            0x0F),
    OBJNAME("d_act16", PROC_DEMO00,            0x10),
    OBJNAME("d_act17", PROC_DEMO00,            0x11),
    OBJNAME("d_act18", PROC_DEMO00,            0x12),
    OBJNAME("d_act19", PROC_DEMO00,            0x13),
    OBJNAME("d_act20", PROC_DEMO00,            0x14),
    OBJNAME("d_act21", PROC_DEMO00,            0x15),
    OBJNAME("d_act22", PROC_DEMO00,            0x16),
    OBJNAME("d_act23", PROC_DEMO00,            0x17),
    OBJNAME("d_act24", PROC_DEMO00,            0x18),
    OBJNAME("d_act25", PROC_DEMO00,            0x19),
    OBJNAME("d_act26", PROC_DEMO00,            0x1A),
    OBJNAME("d_act27", PROC_DEMO00,            0x1B),
    OBJNAME("d_act28", PROC_DEMO00,            0x1C),
    OBJNAME("d_act29", PROC_DEMO00,            0x1D),
    OBJNAME("d_act30", PROC_DEMO00,            0x1E),
    OBJNAME("d_act31", PROC_DEMO00,            0x1F),
    OBJNAME("TLogo",   PROC_TITLE,             -1),
#if DEBUG
    OBJNAME("unitC00", PROC_Obj_TestCube,      -1),
    OBJNAME("unitS00", PROC_Obj_TestCube,      -1),
    OBJNAME("unitCy0", PROC_Obj_TestCube,      -1),
    OBJNAME("damCps",  PROC_Obj_DamCps,        -1),
    OBJNAME("railDrw", PROC_PATH_LINE,         -1),
    OBJNAME("sdoor",   PROC_SPIRAL_DOOR,       -1),
#endif
    OBJNAME("ClearB",  PROC_Obj_Cboard,        -1),
    OBJNAME("Bg",      PROC_BG,                -1),
    OBJNAME("Boom",    PROC_BOOMERANG,         -1),
    OBJNAME("Nbomb",   PROC_NBOMB,             -1),
    OBJNAME("Arrow",   PROC_ARROW,             -1),
    OBJNAME("E_Arrow", PROC_E_ARROW,           -1),
    OBJNAME("Spinner", PROC_SPINNER,           -1),
    OBJNAME("Crod",    PROC_CROD,              -1),
    OBJNAME("Midna",   PROC_MIDNA,             -1),
    OBJNAME("Vrbox",   PROC_VRBOX,             -1),
    OBJNAME("Vrbox2",  PROC_VRBOX2,            -1),
    OBJNAME("mvbg_a",  PROC_BG_OBJ,            -1),
    OBJNAME("burnBox", PROC_Obj_BurnBox,       -1),
    OBJNAME("stnMark", PROC_Obj_StoneMark,     -1),
#if !DEBUG
    OBJNAME("Passer",  PROC_NPC_PASSER,        -1),
#endif
    OBJNAME("Passer2", PROC_NPC_PASSER2,       -1),
    OBJNAME("ShopItm", PROC_ShopItem,          -1),
    OBJNAME("E_yd_lf", PROC_E_YD_LEAF,         -1),
    OBJNAME("E_db_lf", PROC_E_DB_LEAF,         -1),
    OBJNAME("E_hb_lf", PROC_E_HB_LEAF,         -1),
    OBJNAME("E_bi_lf", PROC_E_BI_LEAF,         -1),
    OBJNAME("O_Mato",  PROC_Obj_Mato,          -1),
    OBJNAME("O_Flag",  PROC_Obj_Flag,          -1),
    OBJNAME("O_Flag2", PROC_Obj_Flag2,         -1),
    OBJNAME("O_Flag3", PROC_Obj_Flag3,         -1),
    OBJNAME("CRVLH_U", PROC_Obj_CRVLH_UP,      -1),
    OBJNAME("DemoItm", PROC_Demo_Item,         -1),
    OBJNAME("EndCode", PROC_PLAY_SCENE,        -1),
};
// clang-format on

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
    for (; param_0 > 0; param_0--) {
        if (param_2 == dStage_roomRead_dt_c_GetLoadRoomIndex(*param_1)) {
            return 1;
        }
        param_1++;
    }
    return 0;
}

static int createRoomScene(int param_0) {
    int* ptr = (int*)JKRHeap::alloc(4, -4, NULL);

    if (ptr == NULL) {
        return 0;
    }
    *ptr = param_0;

    if (!fopScnM_CreateReq(PROC_ROOM_SCENE, 0x7FFF, 0, (uintptr_t)ptr)) {
        JKRHeap::free(ptr, NULL);
        return 0;
    }

    return 1;
}

BOOL dStage_roomControl_c::checkRoomDisp(int i_roomNo) const {
    if (checkStatusFlag(i_roomNo, 8)) {
        return 0;
    }

    return checkStatusFlag(i_roomNo, 16) ? 1 : 0;
}

int dStage_roomControl_c::loadRoom(int roomCount, u8* rooms, bool param_2) {
    if (mRoomReadId < 0 && mNoChangeRoom != 0) {
        return 0;
    }

    for (int roomNo = 0; roomNo < ARRAY_SIZEU(mStatus); roomNo++) {
        if (checkStatusFlag(roomNo, 0x02 | 0x04)) {
            return 0;
        }
    }
    
    BOOL r26 = TRUE;
    for (int roomNo = 0; roomNo < ARRAY_SIZE(mStatus); roomNo++) {
        if (dStage_roomControl_c::checkStatusFlag(roomNo, 0x01)) {
            if (!stayRoomCheck(roomCount, rooms, roomNo)) {
                onStatusFlag(roomNo, 0xc);
                r26 = FALSE;
            }
        }
    }
    if (!r26) {
        return FALSE;
    }
    
    for (int i = 0; i < roomCount; i++) {
        u8 roomNo = dStage_roomRead_dt_c_GetLoadRoomIndex(rooms[i]);
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

static void dummy1() {
    ((dStage_stageDt_c*)dComIfGp_getStage())->getStagInfo();
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
    m_dzs = new (mDoExt_getArchiveHeap(), -4) void*[i_num];
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
        delete[] m_dzs;
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
    field_0x60 = 0;
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

u32 dStage_roomControl_c::mProcID;

s8 dStage_roomControl_c::mStayNo;

s8 dStage_roomControl_c::mOldStayNo;

s8 dStage_roomControl_c::mNextStayNo;

u8 dStage_roomControl_c::m_time_pass;

u8 dStage_roomControl_c::mNoChangeRoom;

dStage_roomControl_c::nameData* dStage_roomControl_c::mArcBankName;

dStage_roomControl_c::bankData* dStage_roomControl_c::mArcBankData;

s8 dStage_roomControl_c::mRoomReadId = -1;

static void dStage_actorCreate(stage_actor_data_class* i_actorData, fopAcM_prm_class* i_actorPrm) {
    dStage_objectNameInf* actorInf = dStage_searchName(i_actorData->name);

    if (actorInf == NULL) {
        OS_REPORT("\x1B""[43;30mStage Actor Name Nothing !! <%s>\n\x1B[m", i_actorData->name);
        JKRFree(i_actorPrm);
    } else {
        i_actorPrm->argument = actorInf->argument;
        if (actorInf->procname == PROC_SUSPEND) {
            fopAc_ac_c* actor = (fopAc_ac_c*)fopAcM_FastCreate(actorInf->procname, NULL, NULL, i_actorPrm);

            if (actor != NULL) {
                fopAcM_delete(actor);
            }
            return;
        }

        fopAcM_create(actorInf->procname, NULL, i_actorPrm);
    }
}

static int dStage_cameraCreate(stage_camera2_data_class* i_cameraData, int i_cameraIdx,
                               int param_2) {
    s16 procname = PROC_CAMERA;
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
    if (fpcM_GetName(stageProc) == PROC_PLAY_SCENE) {
        if (strcmp(dComIfGp_getStartStageName(), "S_MV000")) {
            fopMsgM_Create(PROC_METER2, NULL, NULL);
        }
    }

    return 1;
}

static int dStage_cameraInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    stage_camera_class* camera = (stage_camera_class*)((char*)i_data + 4);
    i_stage->setCamera(camera);
    dStage_cameraCreate(camera->m_entries, 0, 0);
    return 1;
}

static int dStage_RoomCameraInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    i_stage->setCamera((stage_camera_class*)((char*)i_data + 4));
    return 1;
}

static int dStage_arrowInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    i_stage->setArrow((stage_arrow_class*)((char*)i_data + 4));
    return 1;
}

stage_map_info_class* dStage_roomDt_c::getMapInfo2(int param_0) const {
    stage_map_info_dummy_class* map_info_p = getMapInfoBase();

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
    stage_map_info_dummy_class* map_info_p = getMapInfoBase();

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
    dStage_nodeHeader* pal_info = (dStage_nodeHeader*)(i_data);
    i_stage->setPaletteInfo((stage_palette_info_class*)pal_info->m_offset);
    return 1;
}

static int dStage_pselectInfoInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    dStage_nodeHeader* psel_info = (dStage_nodeHeader*)(i_data);
    i_stage->setPselectInfo((stage_pselect_info_class*)psel_info->m_offset);
    return 1;
}

static int dStage_envrInfoInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    dStage_nodeHeader* envr_info = (dStage_nodeHeader*)(i_data);
    i_stage->setEnvrInfo((stage_envr_info_class*)envr_info->m_offset);
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
    if (entryNum == 0) {
        i_stage->setFileListInfo(NULL);
    } else {
        dStage_nodeHeader* fili_info = (dStage_nodeHeader*)(i_data);
        i_stage->setFileListInfo((dStage_FileList_dt_c*)fili_info->m_offset);
    }

    return 1;
}

static int dStage_vrboxInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    dStage_nodeHeader* vrbox_info = (dStage_nodeHeader*)(i_data);
    i_stage->setVrboxInfo((stage_vrbox_info_class*)vrbox_info->m_offset);
    return 1;
}

static int dStage_vrboxcolInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                   void* param_3) {
    dStage_nodeHeader* vrcol_info = (dStage_nodeHeader*)(i_data);
    i_stage->setVrboxcolInfo((stage_vrboxcol_info_class*)vrcol_info->m_offset);
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
    dStage_nodeHeader* stag_info = (dStage_nodeHeader*)(i_data);
    i_stage->setStagInfo((stage_stag_info_class*)stag_info->m_offset);

    if (!dStage_isBossStage(i_stage)) {
        dComIfG_deleteStageRes("Xtg_00");
        dComIfGp_resetOldMulti();
    }

    int stageNo = dStage_stagInfo_GetSaveTbl(i_stage->getStagInfo());
    dComIfGs_getSave(stageNo);
    g_save_bit_HIO.init();
    dComIfGs_initDan(stageNo);
    dStage_KeepDoorInfoInit(i_stage);
    return 1;
}

void dStage_stageDt_c::resetOldMulti() {
    mOldMulti = NULL;
}

static int dStage_sclsInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    i_stage->setSclsInfo((stage_scls_info_dummy_class*)((char*)i_data + 4));
    return 1;
}

static int dStage_actorCommonLayerInit(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                       void* param_3) {
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
    roomRead_class* p_node = (roomRead_class*)((int*)i_data + 1);
    roomRead_data_class** rtbl = p_node->m_entries;

    i_stage->setRoom(p_node);

    for (int i = 0; i < p_node->num; i++) {
        if ((intptr_t)rtbl[i] < 0x80000000) {
            rtbl[i] = (roomRead_data_class*)((intptr_t)rtbl[i] + (intptr_t)param_3);
            rtbl[i]->m_rooms = (u8*)((intptr_t)rtbl[i]->m_rooms + (intptr_t)param_3);
        }
    }

    return 1;
}

s8 dStage_roomRead_dt_c_GetReverbStage(roomRead_class& room, int index) {
    if (index < 0 || index >= room.num) {
        index = 0;
    }

    return dStage_roomRead_dt_c_GetReverb(*room.m_entries[index]);
}

static int dStage_ppntInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    i_stage->setPntInfo((dStage_dPnt_c*)((char*)i_data + 4));
    return 1;
}

static int dStage_pathInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    dStage_dPath_c* path_c = (dStage_dPath_c*)((char*)i_data + 4);
    dPath* path = path_c->m_path;

    i_stage->setPathInfo(path_c);

    for (int i = 0; i < path_c->m_num; i++) {
        if ((uintptr_t)path->m_points < 0x80000000) {
            path->m_points = (dPnt*)((uintptr_t)path->m_points + i_stage->getPntInf()->m_pnt_offset);
        }
        path++;
    }

    return 1;
}

static int dStage_rppnInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    i_stage->setPnt2Info((dStage_dPnt_c*)((char*)i_data + 4));
    return 1;
}

static int dStage_rpatInfoInit(dStage_dt_c* i_stage, void* i_data, int i_num, void* param_3) {
    UNUSED(i_num);
    UNUSED(param_3);
    dStage_dPath_c* pStagePath = (dStage_dPath_c*)((char*)i_data + 4);
    dPath* pPath = pStagePath->m_path;

    i_stage->setPath2Info(pStagePath);
    for (s32 i = 0; i < pStagePath->m_num; pPath++, i++, (void)0) {
        if ((uintptr_t)pPath->m_points >= 0x80000000) {
            continue;
        }
        pPath->m_points = (dPnt*)((uintptr_t)pPath->m_points + i_stage->getPnt2Inf()->m_pnt_offset);
    }
    return 1;
}

static int dStage_soundInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    i_stage->setSoundInf((dStage_SoundInfo_c*)((char*)i_data + 4));
    return 1;
}

static int dStage_soundInfoInitCL(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    i_stage->setSoundInfCL((dStage_SoundInfo_c*)((char*)i_data + 4));
    return 1;
}

static void dStage_setLayerTagName(FuncTable* funcTbl, int tblSize, int i_layerNo) {
    char layerTag;
    if (i_layerNo < 10) {
        layerTag = '0';
    } else {
        layerTag = 'W';
    }
    layerTag = i_layerNo + layerTag;

    for (int i = 0; i < tblSize; i++) {
        funcTbl->identifier[3] = layerTag;
        funcTbl++;
    }
}

static void dStage_dt_c_decode(void* i_data, dStage_dt_c* i_stage, FuncTable* funcTbl,
                               int tblSize) {
    if (i_data != NULL) {
        for (int i = 0; i < tblSize; i++) {
            dStage_fileHeader* file = (dStage_fileHeader*)i_data;
            dStage_nodeHeader* p_tno = file->m_nodes;

            FuncTable* nodeFunc = funcTbl + i;

            for (int j = 0; j < file->m_chunkCount; j++) {
                if ((int)p_tno->m_tag == *(int*)nodeFunc->identifier) {
                    if (nodeFunc->function != NULL) {
                        nodeFunc->function(i_stage, p_tno, p_tno->m_entryNum, i_data);
                    }
                    break;
                }
                p_tno++;
            }
        }
    } else {
        OSReport_Error("dStage_dt_c_decode: i_data is NULL\n");
    }
}

static int dStage_stEventInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    i_stage->setMapEventInfo((dStage_MapEventInfo_c*)((char*)i_data + 4));
    return 1;
}

static int dStage_mapEventInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                   void* param_3) {
    i_stage->setMapEventInfo((dStage_MapEventInfo_c*)((char*)i_data + 4));
    return 1;
}

static int dStage_floorInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    i_stage->setFloorInfo((dStage_FloorInfo_c*)((char*)i_data + 4));
    return 1;
}

static int dStage_memaInfoInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    UNUSED(param_2);
    UNUSED(param_3);
    dStage_MemoryMap_c* pd = (dStage_MemoryMap_c*)((char*)i_data + 4);
    i_stage->setMemoryMap(pd);

    if (pd != NULL) {
        OS_REPORT("Memory Block Create !\n");
        u32* entry_p = pd->field_0x4;

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
        JUT_ASSERT(3381, p_tno->m_offset != 0);
        if (p_tno->m_offset != 0 && p_tno->m_offset < 0x80000000) {
            p_tno->m_offset += (uintptr_t)i_data;
        }
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

dStage_roomControl_c::roomDzs_c dStage_roomControl_c::m_roomDzs;

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
            dStage_roomControl_c::m_roomDzs.create(multi->num);
        }

        for (s8 i = 0; i < multi->num; i++) {
            void* dzs = dStage_roomControl_c::addRoomDzs(i, info->mRoomNo);

            if (dzs == NULL) {
                char dzsName[11];
                sprintf(dzsName, "room%d.dzs", info->mRoomNo);

                if (useOldRes) {
                    dzs = dComIfG_getOldStageRes(dzsName);
                } else {
                    dzs = dComIfG_getStageRes(dzsName);
                }
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
    i_stage->setMulti((dStage_Multi_c*)((char*)i_data + 4));
    dStage_initRoomKeepDoorInfo();

    readMult(i_stage, i_stage->getMulti(), false);
    readMult(i_stage, i_stage->getOldMulti(), true);
    i_stage->setRoomNo(-1);

    return 1;
}

static int dStage_lbnkInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum, void* param_3) {
    i_stage->setLbnk((dStage_Lbnk_c*)((char*)i_data + 4));
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
    i_stage->setDrTg((stage_tgsc_class*)((char*)i_data + 4));
    dStage_tgscInfoInit(i_stage, i_data, entryNum, param_3);
    dStage_KeepDoorInfoProc(i_stage, (stage_tgsc_class*)((char*)i_data + 4));
    return 1;
}

static int dStage_roomDrtgInfoInit(dStage_dt_c* i_stage, void* i_data, int entryNum,
                                   void* param_3) {
    i_stage->setDrTg((stage_tgsc_class*)((char*)i_data + 4));
    dStage_doorInfoInit(i_stage, i_data, entryNum, param_3);
    return 1;
}

static int dStage_elstInfoInit(dStage_dt_c* i_stage, void* i_data, int param_2, void* param_3) {
    dStage_Elst_c* elst = (dStage_Elst_c*)((char*)i_data + 4);

    if (param_2 == 0) {
        i_stage->setElst(NULL);
    } else {
        i_stage->setElst(elst);
    }

    return 1;
}

static void dKankyo_create() {
    fopKyM_fastCreate(PROC_KANKYO, NULL, NULL, NULL, NULL);
    fopKyM_fastCreate(PROC_KYEFF, NULL, NULL, NULL, NULL);
    fopKyM_fastCreate(PROC_KYEFF2, NULL, NULL, NULL, NULL);
    fopKyM_fastCreate(PROC_ENVSE, NULL, NULL, NULL, NULL);
}

static void layerMemoryInfoLoader(void* i_data, dStage_dt_c* i_stage, int param_2) {
    static FuncTable l_layerFuncTable[] = {
        {"MEM0", dStage_memaInfoInit},
        {"MEC0", dStage_mecoInfoInit},
    };

    dStage_dt_c_decode(i_data, i_stage, l_layerFuncTable, ARRAY_SIZEU(l_layerFuncTable));
}

static void dStage_dt_c_stageInitLoader(void* i_data, dStage_dt_c* i_stage) {
    static FuncTable l_funcTable[] = {"STAG", dStage_stagInfoInit};

    dStage_dt_c_offsetToPtr(i_data);
    i_stage->init();
    dStage_dt_c_decode(i_data, i_stage, l_funcTable, ARRAY_SIZEU(l_funcTable));
    layerMemoryInfoLoader(i_data, i_stage, -1);
}

static void layerTableLoader(void* i_data, dStage_dt_c* i_stage, int roomNo) {
    static FuncTable l_layerFuncTableA[] = {"SON0", dStage_soundInfoInit};

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

    dStage_setLayerTagName(l_layerFuncTableA, ARRAY_SIZEU(l_layerFuncTableA),
                           dComIfG_play_c::getLayerNo(0));
    dStage_dt_c_decode(i_data, i_stage, l_layerFuncTableA, ARRAY_SIZEU(l_layerFuncTableA));

    dStage_Elst_c* elst = dComIfGp_getStage()->getElst();
    if (elst != NULL && newRoomNo >= 0 && elst->m_entryNum > newRoomNo) {
        dStage_Elst_dt_c* d = elst->m_entries;
        int layer = dComIfG_play_c::getLayerNo(0);
        dStage_setLayerTagName(l_envLayerFuncTable, ARRAY_SIZEU(l_envLayerFuncTable),
                               d[newRoomNo].m_layerTable[layer]);
        dStage_dt_c_decode(i_data, i_stage, l_envLayerFuncTable, ARRAY_SIZEU(l_envLayerFuncTable));
    } else {
        dStage_setLayerTagName(l_envLayerFuncTable, ARRAY_SIZEU(l_envLayerFuncTable), 0);
        dStage_dt_c_decode(i_data, i_stage, l_envLayerFuncTable, ARRAY_SIZEU(l_envLayerFuncTable));
    }
}

dStage_Elst_c* dStage_stageDt_c::getElst(void) { return mElst; }

static void layerActorLoader(void* i_data, dStage_dt_c* i_stage, int param_2) {
    static FuncTable l_layerFuncTable[] = {
        {"Doo0", dStage_roomDrtgInfoInit},
        {"SCO0", dStage_tgscInfoInit},
        {"ACT0", dStage_actorInit},
        {"TRE0", dStage_layerTresureInit},
    };

    dStage_setLayerTagName(l_layerFuncTable, 4, dComIfG_play_c::getLayerNo(0));
    dStage_dt_c_decode(i_data, i_stage, l_layerFuncTable, ARRAY_SIZEU(l_layerFuncTable));
}

static void dStage_dt_c_stageLoader(void* i_data, dStage_dt_c* i_stage) {
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
        {"Door", dStage_roomDrtgInfoInit},     {"ACTR", dStage_actorCommonLayerInit},
        {"TGOB", dStage_actorCommonLayerInit}, {"TRES", dStage_roomTresureInit},
        {"TGSC", dStage_tgscCommonLayerInit},  {"SCOB", dStage_tgscCommonLayerInit},
        {"TGDR", dStage_tgscInfoInit},         {"REVT", dStage_mapEventInfoInit},
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
    dStage_dt_c_stageLoader(stageRsrc, dComIfGp_getStage());
    daSus_c::execute();

    if (dComIfGp_getStartStageRoomNo() >= 0) {
        int status = dStage_roomInit(dComIfGp_getStartStageRoomNo());
        JUT_ASSERT(4517, status);
    }

    *dStage_roomControl_c::getDemoArcName() = NULL;
    dKankyo_create();

    if (dComIfG_getStageRes("vrbox_sora.bmd")) {
        fopAcM_Create(PROC_VRBOX, NULL, NULL);
        fopAcM_Create(PROC_VRBOX2, NULL, NULL);
    }

    dComIfGp_evmng_create();
}

void dStage_Delete() {
    OS_REPORT("dStage_Delete\n");

    char* demoArcName = dStage_roomControl_c::getDemoArcName();
    if (*demoArcName != NULL) {
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

void dStage_stageDt_c::setOldMulti() {
    mOldMulti = mMulti;
}

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
