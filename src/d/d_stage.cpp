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
#include "stdio.h"

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
    OBJNAME("Grass",   PROC_GRASS,             0xFF),
    OBJNAME("kusax1",  PROC_GRASS,             0xFF),
    OBJNAME("kusax7",  PROC_GRASS,             0xFF),
    OBJNAME("kusax21", PROC_GRASS,             0xFF),
    OBJNAME("flower",  PROC_GRASS,             0xFF),
    OBJNAME("flwr7",   PROC_GRASS,             0xFF),
    OBJNAME("flwr17",  PROC_GRASS,             0xFF),
    OBJNAME("pflower", PROC_GRASS,             0xFF),
    OBJNAME("pflwrx7", PROC_GRASS,             0xFF),
    OBJNAME("door",    PROC_DOOR20,            0xFF),
    OBJNAME("kdoor",   PROC_KNOB20,            0xFF),
    OBJNAME("ddoor",   PROC_DBDOOR,            0xFF),
    OBJNAME("bdoor",   PROC_BOSS_DOOR,         0xFF),
    OBJNAME("ndoor",   PROC_DOOR20,            0xFF),
    OBJNAME("tadoor",  PROC_DOOR20,            0xFF),
    OBJNAME("yodoor",  PROC_DOOR20,            0xFF),
    OBJNAME("nadoor",  PROC_DOOR20,            0xFF),
    OBJNAME("l9door",  PROC_DOOR20,            0xFF),
    OBJNAME("l7door",  PROC_DOOR20,            0xFF),
    OBJNAME("pdoor",   PROC_PushDoor,          0xFF),
    OBJNAME("bigdoor", PROC_DOOR20,            0xFF),
    OBJNAME("kshtr00", PROC_Obj_Kshutter,      0xFF),
    OBJNAME("vshuter", PROC_Obj_Kshutter,      0xFF),
    OBJNAME("IzmGate", PROC_Izumi_Gate,        0xFF),
    OBJNAME("NoneCam", PROC_CAMERA,            0xFF),
    OBJNAME("NormCam", PROC_CAMERA,            0xFF),
    OBJNAME("Link",    PROC_ALINK,             0xFF),
    OBJNAME("carry00", PROC_Obj_Carry,         0xFF),
    OBJNAME("carry01", PROC_Obj_Carry,         0xFF),
    OBJNAME("carry02", PROC_Obj_Carry,         0xFF),
    OBJNAME("carry04", PROC_Obj_Carry,         0xFF),
    OBJNAME("carry05", PROC_Obj_Carry,         0xFF),
    OBJNAME("carry06", PROC_Obj_Carry,         0xFF),
    OBJNAME("carry07", PROC_Obj_Carry,         0xFF),
    OBJNAME("carry08", PROC_Obj_Carry,         0xFF),
    OBJNAME("carry09", PROC_Obj_Carry,         0xFF),
    OBJNAME("ball1",   PROC_Obj_Carry,         0xFF),
    OBJNAME("ball2",   PROC_Obj_Carry,         0xFF),
    OBJNAME("ball3",   PROC_Obj_Carry,         0xFF),
    OBJNAME("carry12", PROC_Obj_Carry,         0xFF),
    OBJNAME("carry13", PROC_Obj_Carry,         0xFF),
    OBJNAME("TagLite", PROC_Tag_LightBall,     0xFF),
    OBJNAME("InoBone", PROC_Obj_InoBone,       0xFF),
    OBJNAME("osiBLK0", PROC_Obj_Movebox,       0x00),
    OBJNAME("dmblk",   PROC_Obj_Movebox,       0x00),
    OBJNAME("Kkiba",   PROC_Obj_Movebox,       0x02),
    OBJNAME("smblk",   PROC_Obj_Movebox,       0x02),
    OBJNAME("Lv4blk",  PROC_Obj_Movebox,       0xFF),
    OBJNAME("Lv4blk2", PROC_Obj_Movebox,       0xFF),
    OBJNAME("gstone",  PROC_Obj_Movebox,       0xFF),
    OBJNAME("TagAtt",  PROC_Tag_Attp,          0xFF),
    OBJNAME("mmvbg",   PROC_SET_BG_OBJ,        0xFF),
    OBJNAME("Drop",    PROC_Obj_Drop,          0xFF),
    OBJNAME("BkLeaf",  PROC_Obj_BkLeaf,        0xFF),
    OBJNAME("cylwind", PROC_Obj_Tornado,       0xFF),
    OBJNAME("TagTrnd", PROC_Obj_Tornado2,      0xFF),
    OBJNAME("mvstair", PROC_Obj_MvStair,       0xFF),
    OBJNAME("UHDoor",  PROC_Obj_Cowdoor,       0xFF),
    OBJNAME("SwBoom",  PROC_Obj_Swpropeller,   0xFF),
    OBJNAME("Swboom",  PROC_Obj_Swpropeller,   0xFF),
    OBJNAME("BoomSht", PROC_Obj_BoomShutter,   0xFF),
    OBJNAME("haihai",  PROC_Obj_Hfuta,         0xFF),
    OBJNAME("stopper", PROC_Obj_Stopper,       0xFF),
    OBJNAME("dstop",   PROC_Obj_Stopper2,      0xFF),
    OBJNAME("mhole",   PROC_Obj_MHole,         0xFF),
    OBJNAME("mhole2",  PROC_Obj_MHole,         0xFF),
    OBJNAME("TagMag",  PROC_Tag_Magne,         0xFF),
    OBJNAME("P_Warp",  PROC_Obj_BossWarp,      0xFF),
    OBJNAME("fpillar", PROC_Obj_FirePillar,    0xFF),
    OBJNAME("yfire",   PROC_Obj_FirePillar2,   0xFF),
    OBJNAME("pfire",   PROC_Obj_FirePillar2,   0xFF),
    OBJNAME("Huriko1", PROC_Obj_WoodPendulum,  0xFF),
    OBJNAME("VolBall", PROC_Obj_VolcanicBall,  0xFF),
    OBJNAME("VolBom",  PROC_Obj_VolcanicBall,  0xFF),
    OBJNAME("BigVolc", PROC_Obj_VolcanicBomb,  0xFF),
    OBJNAME("geyser",  PROC_Obj_Geyser,        0xFF),
    OBJNAME("Cwall",   PROC_Obj_ChainWall,     0xFF),
    OBJNAME("Tansu",   PROC_Obj_Chest,         0xFF),
    OBJNAME("Onsen",   PROC_Obj_Onsen,         0xFF),
    OBJNAME("Obj_bm",  PROC_Obj_Bemos,         0xFF),
    OBJNAME("lv6bm",   PROC_Obj_Lv6bemos,      0xFF),
    OBJNAME("E_bm6",   PROC_Obj_Lv6bemos2,     0xFF),
    OBJNAME("stone",   PROC_Obj_Stone,         0xFF),
    OBJNAME("stoneB",  PROC_Obj_Stone,         0xFF),
    OBJNAME("TagSpin", PROC_Tag_Spinner,       0xFF),
    OBJNAME("L_RopeS", PROC_Obj_RopeBridge,    0xFF),
    OBJNAME("L_RopeB", PROC_Obj_RopeBridge,    0xFF),
    OBJNAME("wcover",  PROC_Obj_WellCover,     0xFF),
    OBJNAME("rstair",  PROC_Obj_RotStair,      0xFF),
    OBJNAME("marm",    PROC_Obj_MagneArm,      0xFF),
    OBJNAME("turnSw",  PROC_Obj_SwTurn,        0xFF),
    OBJNAME("chainSw", PROC_Obj_SwChain,       0xFF),
    OBJNAME("wsword",  PROC_Obj_WoodenSword,   0xFF),
    OBJNAME("iceblk",  PROC_Obj_IceBlock,      0xFF),
    OBJNAME("KkrSmk",  PROC_Obj_Smoke,         0xFF),
    OBJNAME("dmele",   PROC_Obj_Elevator,      0xFF),
    OBJNAME("wfall",   PROC_Obj_WaterFall,     0xFF),
    OBJNAME("izora",   PROC_Obj_ZoraCloth,     0xFF),
    OBJNAME("smgdoor", PROC_Obj_SmgDoor,       0xFF),
    OBJNAME("smkdoor", PROC_Obj_SmgDoor,       0xFF),
    OBJNAME("Obj_ava", PROC_Obj_Avalanche,     0xFF),
    OBJNAME("thdoor",  PROC_Obj_TDoor,         0xFF),
    OBJNAME("mstrsrd", PROC_Obj_MasterSword,   0xFF),
    OBJNAME("wimage",  PROC_Obj_WoodStatue,    0xFF),
    OBJNAME("sword",   PROC_Obj_Sword,         0xFF),
    OBJNAME("spring",  PROC_Tag_Spring,        0xFF),
    OBJNAME("BkDoorL", PROC_Obj_BkDoor,        0xFF),
    OBJNAME("BkDoorR", PROC_Obj_BkDoor,        0xFF),
    OBJNAME("IGateL",  PROC_Obj_MGate,         0xFF),
    OBJNAME("IGateR",  PROC_Obj_MGate,         0xFF),
    OBJNAME("HGateL",  PROC_Obj_MGate,         0xFF),
    OBJNAME("HGateR",  PROC_Obj_MGate,         0xFF),
    OBJNAME("K_Gate",  PROC_Obj_KkrGate,       0xFF),
    OBJNAME("R_Gate",  PROC_Obj_RiderGate,     0xFF),
    OBJNAME("kkanban", PROC_Obj_KKanban,       0xFF),
    OBJNAME("Mhsg6",   PROC_Obj_Ladder,        0x00),
    OBJNAME("Mhsg9",   PROC_Obj_Ladder,        0x01),
    OBJNAME("Mhsg12",  PROC_Obj_Ladder,        0x02),
    OBJNAME("Mhsg15",  PROC_Obj_Ladder,        0x03),
    OBJNAME("Mhsg4h",  PROC_Obj_Ladder,        0x04),
    OBJNAME("Mhsg3",   PROC_Obj_Ladder,        0x05),
    OBJNAME("bonbori", PROC_EP,                0xFF),
    OBJNAME("Cow",     PROC_COW,               0xFF),
    OBJNAME("Peru",    PROC_PERU,              0xFF),
    OBJNAME("Kago",    PROC_KAGO,              0xFF),
    OBJNAME("TagFall", PROC_Tag_WaterFall,     0xFF),
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
    OBJNAME("ObjTime", PROC_Obj_Timer,         0xFF),
    OBJNAME("AND_SW",  PROC_ANDSW,             0xFF),
    OBJNAME("AND_SW2", PROC_ANDSW2,            0xFF),
    OBJNAME("ALLdie",  PROC_ALLDIE,            0xFF),
    OBJNAME("rvback",  PROC_Tag_RiverBack,     0xFF),
    OBJNAME("kagoFal", PROC_Tag_KagoFall,      0xFF),
    OBJNAME("CrvGate", PROC_Obj_CRVGATE,       0xFF),
    OBJNAME("CrvSaku", PROC_Obj_CRVFENCE,      0xFF),
    OBJNAME("CrvWood", PROC_Obj_CRVHAHEN,      0xFF),
    OBJNAME("CrvStel", PROC_Obj_CRVSTEEL,      0xFF),
    OBJNAME("CrvLH",   PROC_Obj_CRVLH_DW,      0xFF),
    OBJNAME("RvRock",  PROC_Obj_RIVERROCK,     0xFF),
    OBJNAME("P2a",     PROC_NPC_P2,            0x00),
    OBJNAME("P2b",     PROC_NPC_P2,            0x01),
    OBJNAME("P2c",     PROC_NPC_P2,            0x02),
    OBJNAME("Gnd",     PROC_NPC_GND,           0xFF),
    OBJNAME("grA",     PROC_NPC_GRA,           0xFF),
    OBJNAME("Obj_grA", PROC_OBJ_GRA,           0xFF),
    OBJNAME("Tag_grA", PROC_TAG_GRA,           0xFF),
    OBJNAME("TagYami", PROC_TAG_YAMI,          0xFF),
    OBJNAME("grC",     PROC_NPC_GRC,           0xFF),
    OBJNAME("grM",     PROC_NPC_GRM,           0xFF),
    OBJNAME("grMC",    PROC_NPC_GRMC,          0xFF),
    OBJNAME("grS",     PROC_NPC_GRS,           0xFF),
    OBJNAME("grD1",    PROC_NPC_GRD,           0xFF),
    OBJNAME("grD",     PROC_NPC_WRESTLER,      0x01),
    OBJNAME("grR",     PROC_NPC_GRR,           0xFF),
    OBJNAME("grO",     PROC_NPC_GRO,           0xFF),
    OBJNAME("grZ",     PROC_NPC_GRZ,           0xFF),
    OBJNAME("grARock", PROC_Obj_GraRock,       0xFF),
    OBJNAME("grZRock", PROC_Obj_GrzRock,       0xFF),
    OBJNAME("osnFire", PROC_OBJ_ONSEN_FIRE,    0xFF),
    OBJNAME("yamiD",   PROC_NPC_YAMID,         0xFF),
    OBJNAME("yamiT",   PROC_NPC_YAMIT,         0xFF),
    OBJNAME("yamiS",   PROC_NPC_YAMIS,         0xFF),
    OBJNAME("Blue_NS", PROC_NPC_BLUENS,        0xFF),
    OBJNAME("Kakashi", PROC_NPC_KAKASHI,       0x00),
    OBJNAME("Kdk",     PROC_NPC_KDK,           0xFF),
    OBJNAME("Aru",     PROC_NPC_ARU,           0xFF),
    OBJNAME("Bans",    PROC_NPC_BANS,          0xFF),
    OBJNAME("Besu",    PROC_NPC_BESU,          0xFF),
    OBJNAME("Bou",     PROC_NPC_BOU,           0xFF),
    OBJNAME("BouS",    PROC_NPC_BOU_S,         0x00),
    OBJNAME("clerkA",  PROC_NPC_CLERKA,        0x00),
    OBJNAME("clerkB",  PROC_NPC_CLERKB,        0x00),
    OBJNAME("clerkT",  PROC_NPC_CLERKT,        0x00),
    OBJNAME("Ash",     PROC_NPC_ASH,           0xFF),
    OBJNAME("AshB",    PROC_NPC_ASHB,          0xFF),
    OBJNAME("Shad",    PROC_NPC_SHAD,          0xFF),
    OBJNAME("Rafrel",  PROC_NPC_RAFREL,        0xFF),
    OBJNAME("MoiR",    PROC_NPC_MOIR,          0xFF),
    OBJNAME("impal",   PROC_NPC_IMPAL,         0xFF),
    OBJNAME("Coach",   PROC_NPC_COACH,         0xFF),
    OBJNAME("TheB",    PROC_NPC_THEB,          0xFF),
    OBJNAME("TgArena", PROC_Tag_Arena,         0xFF),
    OBJNAME("TagInst", PROC_Tag_Instruction,   0xFF),
    OBJNAME("GWolf",   PROC_NPC_GWOLF,         0xFF),
    OBJNAME("Len",     PROC_NPC_LEN,           0xFF),
    OBJNAME("Lud",     PROC_NPC_LUD,           0xFF),
    OBJNAME("Doc",     PROC_NPC_DOC,           0xFF),
    OBJNAME("FSeirei", PROC_NPC_FAIRY_SEIREI,  0xFF),
    OBJNAME("Fairy",   PROC_NPC_FAIRY,         0xFF),
    OBJNAME("Hanjo",   PROC_NPC_HANJO,         0xFF),
    OBJNAME("Hoz",     PROC_NPC_HOZ,           0xFF),
    OBJNAME("Jagar",   PROC_NPC_JAGAR,         0xFF),
    OBJNAME("Kkri",    PROC_NPC_KKRI,          0xFF),
    OBJNAME("Kn",      PROC_NPC_KN,            0xFF),
    OBJNAME("KnBlt",   PROC_KN_BULLET,         0xFF),
    OBJNAME("Knj",     PROC_NPC_KNJ,           0xFF),
    OBJNAME("Kolin",   PROC_NPC_KOLIN,         0xFF),
    OBJNAME("Kolinb",  PROC_NPC_KOLINB,        0xFF),
    OBJNAME("Kyury",   PROC_NPC_KYURY,         0xFF),
    OBJNAME("Maro",    PROC_NPC_MARO,          0xFF),
    OBJNAME("midP",    PROC_NPC_MIDP,          0xFF),
    OBJNAME("Moi",     PROC_NPC_MOI,           0xFF),
    OBJNAME("Raca",    PROC_NPC_RACA,          0xFF),
    OBJNAME("Post",    PROC_NPC_POST,          0xFF),
    OBJNAME("Pouya",   PROC_NPC_POUYA,         0xFF),
    OBJNAME("Saru",    PROC_NPC_SARU,          0xFF),
    OBJNAME("seiB",    PROC_NPC_SEIB,          0xFF),
    OBJNAME("seiC",    PROC_NPC_SEIC,          0xFF),
    OBJNAME("seiD",    PROC_NPC_SEID,          0xFF),
    OBJNAME("Seira",   PROC_NPC_SEIRA,         0xFF),
    OBJNAME("Seira2",  PROC_NPC_SERA2,         0xFF),
    OBJNAME("Seirei",  PROC_NPC_SEIREI,        0xFF),
    OBJNAME("Sha",     PROC_NPC_SHAMAN,        0xFF),
    OBJNAME("sMaro",   PROC_NPC_SMARO,         0xFF),
    OBJNAME("solA",    PROC_NPC_SOLA,          0xFF),
    OBJNAME("Taro",    PROC_NPC_TARO,          0xFF),
    OBJNAME("PA_Besu", PROC_NPC_PACHI_BESU,    0xFF),
    OBJNAME("PA_Taro", PROC_NPC_PACHI_TARO,    0xFF),
    OBJNAME("PA_Maro", PROC_NPC_PACHI_MARO,    0xFF),
    OBJNAME("TagPati", PROC_TAG_PATI,          0xFF),
    OBJNAME("The",     PROC_NPC_THE,           0xFF),
    OBJNAME("Tkj",     PROC_NPC_TKJ,           0xFF),
    OBJNAME("Tks",     PROC_NPC_TKS,           0xFF),
    OBJNAME("ObjTks",  PROC_OBJ_TKS,           0xFF),
    OBJNAME("Tkc",     PROC_NPC_TKC,           0xFF),
    OBJNAME("Tkj2",    PROC_NPC_TKJ2,          0xFF),
    OBJNAME("Toby",    PROC_NPC_TOBY,          0xFF),
    OBJNAME("Uri",     PROC_NPC_URI,           0xFF),
    OBJNAME("Yelia",   PROC_NPC_YELIA,         0xFF),
    OBJNAME("ykM",     PROC_NPC_YKM,           0xFF),
    OBJNAME("ykW",     PROC_NPC_YKW,           0xFF),
    OBJNAME("zanB",    PROC_NPC_ZANB,          0xFF),
    OBJNAME("Zant",    PROC_NPC_ZANT,          0xFF),
    OBJNAME("Zelda",   PROC_NPC_ZELDA,         0xFF),
    OBJNAME("ZelR",    PROC_NPC_ZELR,          0xFF),
    OBJNAME("ZelRo",   PROC_NPC_ZELRO,         0xFF),
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
    OBJNAME("zrC",     PROC_NPC_ZRC,           0xFF),
    OBJNAME("zrZ",     PROC_NPC_ZRZ,           0xFF),
    OBJNAME("GrvStn",  PROC_Obj_GraveStone,    0xFF),
    OBJNAME("zrARock", PROC_Obj_ZraRock,       0xFF),
    OBJNAME("StAndGl", PROC_START_AND_GOAL,    0xFF),
    OBJNAME("zrAMark", PROC_ZRA_MARK,          0xFF),
    OBJNAME("myna2",   PROC_MYNA2,             0xFF),
    OBJNAME("myn2tag", PROC_TAG_MYNA2,         0xFF),
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
    OBJNAME("shoe",    PROC_NPC_SHOE,          0xFF),
    OBJNAME("DoorBoy", PROC_NPC_DOORBOY,       0xFF),
    OBJNAME("prayer",  PROC_NPC_PRAYER,        0xFF),
    OBJNAME("km_Hana", PROC_NPC_KASIHANA,      0xFF),
    OBJNAME("km_Kyu",  PROC_NPC_KASIKYU,       0xFF),
    OBJNAME("km_Mich", PROC_NPC_KASIMICH,      0xFF),
    OBJNAME("NpcChat", PROC_NPC_CHAT,          0xFF),
    OBJNAME("chtSolA", PROC_NPC_SOLDIERa,      0xFF),
    OBJNAME("chtSolB", PROC_NPC_SOLDIERb,      0xFF),
    OBJNAME("DrSol1",  PROC_NPC_DRSOL,         0x00),
    OBJNAME("DrSol2",  PROC_NPC_DRSOL,         0x01),
    OBJNAME("chin",    PROC_NPC_CHIN,          0xFF),
    OBJNAME("ins",     PROC_NPC_INS,           0xFF),
    OBJNAME("Shop0",   PROC_NPC_SHOP0,         0xFF),
    OBJNAME("TagSch",  PROC_Tag_Schedule,      0xFF),
    OBJNAME("TagEsc",  PROC_Tag_Escape,        0xFF),
    OBJNAME("passer",  PROC_PASSER_MNG,        0x00),
    OBJNAME("passerL", PROC_PASSER_MNG,        0x01),
    OBJNAME("fmtion",  PROC_FORMATION_MNG,     0x00),
    OBJNAME("fmtionL", PROC_FORMATION_MNG,     0x01),
    OBJNAME("guard",   PROC_GUARD_MNG,         0xFF),
    OBJNAME("TgGuard", PROC_TAG_GUARD,         0xFF),
    OBJNAME("Mk",      PROC_NPC_MK,            0xFF),
    OBJNAME("Obj_Tbi", PROC_Obj_Yobikusa,      0x00),
    OBJNAME("Obj_Uma", PROC_Obj_Yobikusa,      0x01),
    OBJNAME("Obj_knk", PROC_Obj_KazeNeko,      0xFF),
    OBJNAME("Obj_nmp", PROC_Obj_NamePlate,     0xFF),
    OBJNAME("Obj_Tie", PROC_Obj_OnCloth,       0xFF),
    OBJNAME("Obj_Lrp", PROC_Obj_LndRope,       0xFF),
    OBJNAME("Obj_Sit", PROC_Obj_ItaRope,       0xFF),
    OBJNAME("Ikada",   PROC_Obj_Ikada,         0xFF),
    OBJNAME("Dust",    PROC_Obj_DUST,          0xFF),
    OBJNAME("Obj_Ita", PROC_Obj_ITA,           0xFF),
    OBJNAME("Ice_l",   PROC_Obj_Ice_l,         0xFF),
    OBJNAME("Ice_s",   PROC_Obj_Ice_s,         0xFF),
    OBJNAME("E_Make",  PROC_Obj_E_CREATE,      0xFF),
    OBJNAME("Bhhashi", PROC_Obj_Bhbridge,      0xFF),
    OBJNAME("Yousei",  PROC_Obj_Yousei,        0xFF),
    OBJNAME("G_Kabe",  PROC_Obj_GOMIKABE,      0xFF),
    OBJNAME("B_ling",  PROC_Obj_Mato,          0xFF),
    OBJNAME("kab_o",   PROC_Obj_Kabuto,        0xFF),
    OBJNAME("I_Cho",   PROC_Obj_Cho,           0xFF),
    OBJNAME("I_Kuw",   PROC_Obj_Kuw,           0xFF),
    OBJNAME("I_Nan",   PROC_Obj_Nan,           0xFF),
    OBJNAME("I_Dan",   PROC_Obj_Dan,           0xFF),
    OBJNAME("I_Kam",   PROC_Obj_Kam,           0xFF),
    OBJNAME("I_Ten",   PROC_Obj_Ten,           0xFF),
    OBJNAME("I_Ari",   PROC_Obj_Ari,           0xFF),
    OBJNAME("I_Kag",   PROC_Obj_Kag,           0xFF),
    OBJNAME("I_Tom",   PROC_Obj_Tombo,         0xFF),
    OBJNAME("I_Bat",   PROC_Obj_Batta,         0xFF),
    OBJNAME("I_Kat",   PROC_Obj_Kat,           0xFF),
    OBJNAME("H_Saku",  PROC_Obj_H_Saku,        0xFF),
    OBJNAME("sm_door", PROC_Obj_SM_DOOR,       0xFF),
    OBJNAME("Kaisou",  PROC_Obj_Kaisou,        0xFF),
    OBJNAME("Ni",      PROC_NI,                0xFF),
    OBJNAME("Fr",      PROC_FR,                0xFF),
    OBJNAME("Sq",      PROC_SQ,                0xFF),
    OBJNAME("Bd",      PROC_BD,                0xFF),
    OBJNAME("Obj_ki",  PROC_OBJ_KI,            0xFF),
    OBJNAME("Do",      PROC_DO,                0xFF),
    OBJNAME("Lm",      PROC_NPC_FISH,          0x00),
    OBJNAME("Tr",      PROC_NPC_FISH,          0x05),
    OBJNAME("Npc_tr",  PROC_NPC_TR,            0xFF),
    OBJNAME("Npc_lf",  PROC_NPC_LF,            0xFF),
    OBJNAME("Fish",    PROC_MG_FISH,           0xFF),
    OBJNAME("NPC_TK",  PROC_NPC_TK,            0xFF),
    OBJNAME("Obj_fd",  PROC_OBJ_FOOD,          0xFF),
    OBJNAME("MYNA",    PROC_MYNA,              0xFF),
    OBJNAME("SSITEM",  PROC_OBJ_SSITEM,        0xFF),
    OBJNAME("SSDRINK", PROC_OBJ_SSDRINK,       0xFF),
    OBJNAME("TGDRINK", PROC_TAG_SSDRINK,       0xFF),
    OBJNAME("Tag_Btl", PROC_TAG_BTLITM,        0xFF),
    OBJNAME("lv5soup", PROC_TAG_LV5SOUP,       0xFF),
    OBJNAME("TGMNLIG", PROC_TAG_MNLIGHT,       0xFF),
    OBJNAME("TGSPCAM", PROC_TAG_SHOPCAM,       0xFF),
    OBJNAME("TGSPITM", PROC_TAG_SHOPITM,       0xFF),
    OBJNAME("OilTubo", PROC_OBJ_OILTUBO,       0xFF),
    OBJNAME("Roten",   PROC_OBJ_ROTEN,         0xFF),
    OBJNAME("Pleaf",   PROC_OBJ_PLEAF,         0xFF),
    OBJNAME("KBacket", PROC_OBJ_KBACKET,       0xFF),
    OBJNAME("YBag",    PROC_OBJ_YBAG,          0xFF),
    OBJNAME("Pumpkin", PROC_OBJ_PUMPKIN,       0xFF),
    OBJNAME("Obj_kg",  PROC_OBJ_KAGO,          0xFF),
    OBJNAME("BbKago",  PROC_OBJ_KAGO,          0x00),
    OBJNAME("OnKago",  PROC_OBJ_KAGO,          0x01),
    OBJNAME("ObjHasi", PROC_Obj_HHASHI,        0xFF),
    OBJNAME("Thashi",  PROC_Obj_THASHI,        0xFF),
    OBJNAME("B_Hashi", PROC_Obj_BHASHI,        0xFF),
    OBJNAME("E_MD",    PROC_E_MD,              0xFF),
    OBJNAME("E_s1",    PROC_E_S1,              0xFF),
    OBJNAME("E_wb",    PROC_E_WB,              0xFF),
    OBJNAME("E_rd",    PROC_E_RD,              0xFF),
    OBJNAME("E_rdb",   PROC_E_RDB,             0xFF),
    OBJNAME("E_rdy",   PROC_E_RDY,             0xFF),
    OBJNAME("E_fm",    PROC_E_FM,              0xFF),
    OBJNAME("E_dn",    PROC_E_DN,              0xFF),
    OBJNAME("E_mf",    PROC_E_MF,              0xFF),
    OBJNAME("TreeSh",  PROC_TREESH,            0xFF),
    OBJNAME("E_bug",   PROC_E_BUG,             0xFF),
    OBJNAME("E_bee",   PROC_E_BEE,             0xFF),
    OBJNAME("E_nest",  PROC_E_NEST,            0xFF),
    OBJNAME("E_kr",    PROC_E_KR,              0xFF),
    OBJNAME("E_st",    PROC_E_ST,              0xFF),
    OBJNAME("E_tk",    PROC_E_TK,              0xFF),
    OBJNAME("E_tk2",   PROC_E_TK2,             0xFF),
    OBJNAME("E_th",    PROC_E_TH,              0xFF),
    OBJNAME("E_cr",    PROC_E_CR,              0xFF),
    OBJNAME("E_df",    PROC_E_DF,              0xFF),
    OBJNAME("E_ph",    PROC_E_PH,              0xFF),
    OBJNAME("E_fs",    PROC_E_FS,              0xFF),
    OBJNAME("E_SB",    PROC_E_SB,              0xFF),
    OBJNAME("E_HM",    PROC_E_HM,              0xFF),
    OBJNAME("E_fk",    PROC_E_FK,              0xFF),
    OBJNAME("E_pm",    PROC_E_PM,              0xFF),
    OBJNAME("Npc_ne",  PROC_NPC_NE,            0xFF),
    OBJNAME("E_db",    PROC_E_DB,              0xFF),
    OBJNAME("E_kg",    PROC_E_KG,              0xFF),
    OBJNAME("E_sh",    PROC_E_SH,              0xFF),
    OBJNAME("E_sf",    PROC_E_SF,              0xFF),
    OBJNAME("Obj_lb",  PROC_OBJ_LBOX,          0xFF),
    OBJNAME("Obj_kb",  PROC_OBJ_KBOX,          0xFF),
    OBJNAME("E_bs",    PROC_E_BS,              0xFF),
    OBJNAME("Sekizoa", PROC_OBJ_SEKIZOA,       0x00),
    OBJNAME("Sekizob", PROC_OBJ_SEKIZOA,       0x01),
    OBJNAME("E_sm",    PROC_E_SM,              0xFF),
    OBJNAME("E_sm2",   PROC_E_SM2,             0xFF),
    OBJNAME("ky_tag0", PROC_KYTAG00,           0xFF),
    OBJNAME("ky_tag1", PROC_KYTAG01,           0x00),
    OBJNAME("ky_tag2", PROC_KYTAG02,           0xFF),
    OBJNAME("ky_tag3", PROC_KYTAG01,           0x01),
    OBJNAME("kytag3",  PROC_KYTAG03,           0xFF),
    OBJNAME("DK_tag",  PROC_KYTAG04,           0xFF),
    OBJNAME("PPHole",  PROC_KYTAG05,           0xFF),
    OBJNAME("KY_chg",  PROC_KYTAG06,           0xFF),
    OBJNAME("Plight",  PROC_KYTAG07,           0xFF),
    OBJNAME("KY_mk",   PROC_KYTAG08,           0xFF),
    OBJNAME("Bound",   PROC_KYTAG09,           0xFF),
    OBJNAME("WC_tag",  PROC_KYTAG01,           0x02),
    OBJNAME("Sparks",  PROC_KYTAG10,           0xFF),
    OBJNAME("Tcntrl",  PROC_KYTAG11,           0xFF),
    OBJNAME("Drkmst",  PROC_KYTAG12,           0xFF),
    OBJNAME("Blsnow",  PROC_KYTAG13,           0xFF),
    OBJNAME("Savmem",  PROC_KYTAG14,           0xFF),
    OBJNAME("Zshake",  PROC_KYTAG15,           0xFF),
    OBJNAME("Pikari",  PROC_KYTAG16,           0xFF),
    OBJNAME("Lmask",   PROC_KYTAG17,           0xFF),
    OBJNAME("E_is",    PROC_E_IS,              0xFF),
    OBJNAME("E_ai",    PROC_E_AI,              0xFF),
    OBJNAME("E_ba",    PROC_E_BA,              0xFF),
    OBJNAME("E_bu",    PROC_E_BU,              0xFF),
    OBJNAME("E_sw",    PROC_E_SW,              0xFF),
    OBJNAME("E_ge",    PROC_E_GE,              0xFF),
    OBJNAME("TagGe",   PROC_Tag_WatchGe,       0xFF),
    OBJNAME("E_ym",    PROC_E_YM,              0xFF),
    OBJNAME("E_ymt",   PROC_E_YM_TAG,          0xFF),
    OBJNAME("E_ymb",   PROC_E_YMB,             0xFF),
    OBJNAME("Fwall",   PROC_Tag_FWall,         0xFF),
    OBJNAME("E_hz",    PROC_E_HZ,              0xFF),
    OBJNAME("E_ws",    PROC_E_WS,              0xFF),
    OBJNAME("E_oc",    PROC_E_OC,              0xFF),
    OBJNAME("E_dt",    PROC_E_DT,              0xFF),
    OBJNAME("E_bg",    PROC_E_BG,              0xFF),
    OBJNAME("E_octbg", PROC_E_OctBg,           0xFF),
    OBJNAME("E_tt",    PROC_E_TT,              0xFF),
    OBJNAME("E_dk",    PROC_E_DK,              0xFF),
    OBJNAME("E_vt",    PROC_E_VT,              0xFF),
    OBJNAME("E_ww",    PROC_E_WW,              0xFF),
    OBJNAME("E_gi",    PROC_E_GI,              0xFF),
    OBJNAME("E_mk",    PROC_E_MK,              0xFF),
    OBJNAME("B_gm",    PROC_B_GM,              0xFF),
    OBJNAME("E_gm",    PROC_E_GM,              0xFF),
    OBJNAME("E_gs",    PROC_E_GS,              0xFF),
    OBJNAME("Npc_ne",  PROC_NPC_NE,            0xFF),
    OBJNAME("Obj_gm",  PROC_OBJ_GM,            0xFF),
    OBJNAME("E_mm",    PROC_E_MM,              0xFF),
    OBJNAME("E_mm2",   PROC_E_MM,              0x01),
    OBJNAME("E_fz",    PROC_E_FZ,              0xFF),
    OBJNAME("E_fb",    PROC_E_FB,              0xFF),
    OBJNAME("E_Zs",    PROC_E_ZS,              0xFF),
    OBJNAME("E_kk",    PROC_E_KK,              0xFF),
    OBJNAME("E_hp",    PROC_E_HP,              0xFF),
    OBJNAME("E_zh",    PROC_E_ZH,              0xFF),
    OBJNAME("E_zm",    PROC_E_ZM,              0xFF),
    OBJNAME("E_zm2",   PROC_E_ZM,              0x01),
    OBJNAME("E_pz",    PROC_E_PZ,              0xFF),
    OBJNAME("Kita",    PROC_OBJ_KITA,          0xFF),
    OBJNAME("E_nz",    PROC_E_NZ,              0xFF),
    OBJNAME("E_po",    PROC_E_PO,              0xFF),
    OBJNAME("Obj_w0",  PROC_OBJ_WEB0,          0xFF),
    OBJNAME("Obj_w1",  PROC_OBJ_WEB1,          0xFF),
    OBJNAME("Obj_cb",  PROC_OBJ_CB,            0xFF),
    OBJNAME("Obj_mak", PROC_OBJ_MAKI,          0xFF),
    OBJNAME("Obj_brg", PROC_OBJ_BRG,           0xFF),
    OBJNAME("E_rb",    PROC_E_RB,              0xFF),
    OBJNAME("E_sg",    PROC_E_SG,              0xFF),
    OBJNAME("Obj_tby", PROC_OBJ_TOBY,          0xFF),
    OBJNAME("Obj_sw",  PROC_OBJ_SW,            0xFF),
    OBJNAME("Npc_ks",  PROC_NPC_KS,            0xFF),
    OBJNAME("Obj_so",  PROC_OBJ_SO,            0xFF),
    OBJNAME("E_bi",    PROC_E_BI,              0xFF),
    OBJNAME("Obj_key", PROC_OBJ_KEY,           0xFF),
    OBJNAME("E_hb",    PROC_E_HB,              0xFF),
    OBJNAME("Obj_hb",  PROC_OBJ_HB,            0xFF),
    OBJNAME("B_bq",    PROC_B_BQ,              0xFF),
    OBJNAME("B_bh",    PROC_B_BH,              0xFF),
    OBJNAME("B_go",    PROC_B_GO,              0xFF),
    OBJNAME("Obj_fw",  PROC_OBJ_FW,            0xFF),
    OBJNAME("E_ga",    PROC_E_GA,              0xFF),
    OBJNAME("E_gb",    PROC_E_GB,              0xFF),
    OBJNAME("E_wap",   PROC_E_WAP,             0xFF),
    OBJNAME("Obj_sui", PROC_OBJ_SUISYA,        0xFF),
    OBJNAME("Obj_bal", PROC_OBJ_BALLOON,       0xFF),
    OBJNAME("Obj_lp",  PROC_OBJ_LP,            0xFF),
    OBJNAME("Obj_nd",  PROC_OBJ_NDOOR,         0xFF),
    OBJNAME("Obj_ud",  PROC_OBJ_UDOOR,         0xFF),
    OBJNAME("Obj_us",  PROC_OBJ_USAKU,         0xFF),
    OBJNAME("Obj_kn2", PROC_OBJ_KANBAN2,       0xFF),
    OBJNAME("Obj_kyh", PROC_OBJ_KEYHOLE,       0xFF),
    OBJNAME("Bed",     PROC_OBJ_BED,           0xFF),
    OBJNAME("BouMato", PROC_OBJ_BOUMATO,       0xFF),
    OBJNAME("AutoMt",  PROC_OBJ_AUTOMATA,      0xFF),
    OBJNAME("Gadget",  PROC_OBJ_GADGET,        0xFF),
    OBJNAME("ItaMato", PROC_OBJ_ITAMATO,       0xFF),
    OBJNAME("Nougu",   PROC_OBJ_NOUGU,         0xFF),
    OBJNAME("Stick",   PROC_OBJ_STICK,         0xFF),
    OBJNAME("Mie",     PROC_OBJ_MIE,           0xFF),
    OBJNAME("SkDoor",  PROC_OBJ_SEKIDOOR,      0xFF),
    OBJNAME("Sekizo",  PROC_OBJ_SEKIZO,        0xFF),
    OBJNAME("SMTile",  PROC_OBJ_SMTILE,        0xFF),
    OBJNAME("H_kage",  PROC_OBJ_KAGE,          0xFF),
    OBJNAME("E_ms",    PROC_E_MS,              0xFF),
    OBJNAME("Obj_tp",  PROC_OBJ_TP,            0xFF),
    OBJNAME("Obj_ms",  PROC_OBJ_MSIMA,         0xFF),
    OBJNAME("E_gob",   PROC_E_GOB,             0xFF),
    OBJNAME("E_yk",    PROC_E_YK,              0xFF),
    OBJNAME("E_yr",    PROC_E_YR,              0xFF),
    OBJNAME("E_yg",    PROC_E_YG,              0xFF),
    OBJNAME("Obj_fm",  PROC_OBJ_FMOBJ,         0xFF),
    OBJNAME("B_ob",    PROC_B_OB,              0xFF),
    OBJNAME("OcHashi", PROC_OCTHASHI,          0xFF),
    OBJNAME("E_dd",    PROC_E_DD,              0xFF),
    OBJNAME("Obj_rw",  PROC_OBJ_RW,            0xFF),
    OBJNAME("Obj_ih",  PROC_OBJ_IHASI,         0xFF),
    OBJNAME("E_yc",    PROC_E_YC,              0xFF),
    OBJNAME("B_ds",    PROC_B_DS,              0xFF),
    OBJNAME("B_dr",    PROC_B_DR,              0xFF),
    OBJNAME("B_gg",    PROC_B_GG,              0xFF),
    OBJNAME("B_yo",    PROC_B_YO,              0xFF),
    OBJNAME("B_tn",    PROC_B_TN,              0xFF),
    OBJNAME("B_zant",  PROC_B_ZANT,            0xFF),
    OBJNAME("B_zntm",  PROC_B_ZANTZ,           0xFF),
    OBJNAME("B_znts",  PROC_B_ZANTS,           0xFF),
    OBJNAME("E_yd",    PROC_E_YD,              0xFF),
    OBJNAME("E_yh",    PROC_E_YH,              0xFF),
    OBJNAME("Henna",   PROC_NPC_HENNA,         0xFF),
    OBJNAME("Henna0",  PROC_NPC_HENNA0,        0xFF),
    OBJNAME("Npc_du",  PROC_NPC_DU,            0x00),
    OBJNAME("Econt",   PROC_ECONT,             0xFF),
    OBJNAME("B_mgn",   PROC_B_MGN,             0xFF),
    OBJNAME("Hzelda",  PROC_E_HZELDA,          0xFF),
    OBJNAME("Worm",    PROC_NPC_WORM,          0xFF),
    OBJNAME("B_gnd",   PROC_B_GND,             0xFF),
    OBJNAME("Obj_gb",  PROC_OBJ_GB,            0xFF),
    OBJNAME("tboxA0",  PROC_TBOX,              0xFF),
    OBJNAME("tboxA1",  PROC_TBOX,              0xFF),
    OBJNAME("tboxA2",  PROC_TBOX,              0xFF),
    OBJNAME("tboxB0",  PROC_TBOX,              0xFF),
    OBJNAME("tboxB1",  PROC_TBOX,              0xFF),
    OBJNAME("tboxB2",  PROC_TBOX,              0xFF),
    OBJNAME("tboxJ0",  PROC_TBOX,              0xFF),
    OBJNAME("tboxP0",  PROC_TBOX,              0xFF),
    OBJNAME("tboxB00", PROC_TBOX,              0xFF),
    OBJNAME("tboxB01", PROC_TBOX,              0xFF),
    OBJNAME("tboxW0",  PROC_TBOX,              0xFF),
    OBJNAME("tboxEL0", PROC_TBOX2,             0xFF),
    OBJNAME("tboxEL1", PROC_TBOX2,             0xFF),
    OBJNAME("l8warp",  PROC_Obj_BossWarp,      0xFF),
    OBJNAME("L1Bdoor", PROC_L1BOSS_DOOR,       0xFF),
    OBJNAME("L2Bdoor", PROC_L1BOSS_DOOR,       0xFF),
    OBJNAME("L3Bdoor", PROC_Obj_Kshutter,      0xFF),
    OBJNAME("L4Bdoor", PROC_L1BOSS_DOOR,       0xFF),
    OBJNAME("L5Bdoor", PROC_L5BOSS_DOOR,       0xFF),
    OBJNAME("L6Bdoor", PROC_L1BOSS_DOOR,       0xFF),
    OBJNAME("L7Bdoor", PROC_L1BOSS_DOOR,       0xFF),
    OBJNAME("L8Bdoor", PROC_L1BOSS_DOOR,       0xFF),
    OBJNAME("L9Bdoor", PROC_L1BOSS_DOOR,       0xFF),
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
    OBJNAME("Horse",   PROC_HORSE,             0xFF),
    OBJNAME("Canoe",   PROC_CANOE,             0xFF),
    OBJNAME("Crope",   PROC_Obj_Crope,         0xFF),
    OBJNAME("Cstatue", PROC_CSTATUE,           0xFF),
    OBJNAME("CstaF",   PROC_CSTAF,             0xFF),
    OBJNAME("HoZelda", PROC_HOZELDA,           0xFF),
    OBJNAME("Mhint",   PROC_Tag_Mhint,         0xFF),
    OBJNAME("Mmsg",    PROC_Tag_Mmsg,          0xFF),
    OBJNAME("Mwait",   PROC_Tag_Mwait,         0xFF),
    OBJNAME("Mstop",   PROC_Tag_Mstop,         0xFF),
    OBJNAME("MstopE",  PROC_Tag_Mstop,         0xFF),
    OBJNAME("Hinit",   PROC_Tag_Hinit,         0xFF),
    OBJNAME("Hjump",   PROC_Tag_Hjump,         0xFF),
    OBJNAME("Hstop",   PROC_Tag_Hstop,         0xFF),
    OBJNAME("AJnot",   PROC_Tag_AJnot,         0xFF),
    OBJNAME("Wljump",  PROC_Tag_Wljump,        0xFF),
    OBJNAME("Sppath",  PROC_Tag_Sppath,        0xFF),
    OBJNAME("Stream",  PROC_Tag_Stream,        0xFF),
    OBJNAME("Gstart",  PROC_Tag_Gstart,        0xFF),
    OBJNAME("TgTGate", PROC_Tag_TWGate,        0xFF),
    OBJNAME("Lv6Gate", PROC_Tag_Lv6Gate,       0xFF),
    OBJNAME("Lv7Gate", PROC_Tag_Lv7Gate,       0xFF),
    OBJNAME("Lv8Gate", PROC_Tag_Lv8Gate,       0xFF),
    OBJNAME("TgTHint", PROC_Tag_TheBHint,      0xFF),
    OBJNAME("TagAsst", PROC_Tag_Assist,        0xFF),
    OBJNAME("Dmidna",  PROC_DMIDNA,            0xFF),
    OBJNAME("Fchain",  PROC_Obj_Fchain,        0xFF),
    OBJNAME("Wchain",  PROC_Obj_Wchain,        0xFF),
    OBJNAME("Cdoor",   PROC_Obj_Cdoor,         0xFF),
    OBJNAME("Cgate",   PROC_Obj_Cdoor,         0xFF),
    OBJNAME("Water00", PROC_GRDWATER,          0xFF),
    OBJNAME("Digpl",   PROC_Obj_Digpl,         0xFF),
    OBJNAME("Digholl", PROC_Obj_Digholl,       0xFF),
    OBJNAME("Bombf",   PROC_Obj_Bombf,         0xFF),
    OBJNAME("hider",   PROC_SUSPEND,           0xFF),
    OBJNAME("SwAreaC", PROC_SWC00,             0xFF),
    OBJNAME("SwAreaS", PROC_SWC00,             0xFF),
    OBJNAME("SwLBall", PROC_SwLBall,           0xFF),
    OBJNAME("SwBall",  PROC_SwBall,            0xFF),
    OBJNAME("thouse",  PROC_Obj_TobyHouse,     0xFF),
    OBJNAME("ironbox", PROC_Obj_MetalBox,      0xFF),
    OBJNAME("l6swtrn", PROC_Obj_Lv6SwTurn,     0xFF),
    OBJNAME("l2bmtag", PROC_Tag_Lv2PrChk,      0xFF),
    OBJNAME("swspin",  PROC_Obj_SwSpinner,     0xFF),
    OBJNAME("spnGear", PROC_Obj_Lv4Gear,       0xFF),
    OBJNAME("fan",     PROC_Obj_Fan,           0xFF),
    OBJNAME("tgake",   PROC_Obj_Gake,          0xFF),
    OBJNAME("tmoon",   PROC_Obj_TMoon,         0xFF),
    OBJNAME("gwall",   PROC_Obj_GanonWall,     0xFF),
    OBJNAME("gwall2",  PROC_Obj_GanonWall2,    0xFF),
    OBJNAME("scnChg",  PROC_SCENE_EXIT,        0xFF),
    OBJNAME("scnChg2", PROC_SCENE_EXIT2,       0xFF),
    OBJNAME("noChgRm", PROC_NO_CHG_ROOM,       0xFF),
    OBJNAME("readRm",  PROC_NO_CHG_ROOM,       0x00),
    OBJNAME("Tetd",    PROC_DSHUTTER,          0xFF),
    OBJNAME("fence",   PROC_DSHUTTER,          0xFF),
    OBJNAME("lv7saku", PROC_DSHUTTER,          0xFF),
    OBJNAME("YkgrON",  PROC_Ykgr,              0xFF),
    OBJNAME("YkgrOFF", PROC_Ykgr,              0xFF),
    OBJNAME("TagEv",   PROC_TAG_EVENT,         0xFF),
    OBJNAME("TagEvC",  PROC_TAG_EVENT,         0xFF),
    OBJNAME("TagEvt",  PROC_TAG_EVT,           0x00),
    OBJNAME("TagEvt1", PROC_TAG_EVT,           0x01),
    OBJNAME("TagHowl", PROC_TAG_HOWL,          0x01),
    OBJNAME("Tag_ms",  PROC_TAG_MSG,           0x00),
    OBJNAME("LntnTag", PROC_TAG_LANTERN,       0xFF),
    OBJNAME("EvtArea", PROC_TAG_EVTAREA,       0xFF),
    OBJNAME("AllMato", PROC_TAG_ALLMATO,       0xFF),
    OBJNAME("EvtMsg",  PROC_TAG_EVTMSG,        0xFF),
    OBJNAME("KMsg",    PROC_TAG_KMSG,          0xFF),
    OBJNAME("Push",    PROC_TAG_PUSH,          0xFF),
    OBJNAME("Teloper", PROC_TAG_TELOP,         0xFF),
    OBJNAME("CamChg",  PROC_TAG_CAMERA,        0xFF),
    OBJNAME("CamArea", PROC_TAG_CAMERA,        0xFF),
    OBJNAME("CamAreC", PROC_TAG_CAMERA,        0xFF),
    OBJNAME("ChkPntS", PROC_TAG_CHKPOINT,      0xFF),
    OBJNAME("ChkPntC", PROC_TAG_CHKPOINT,      0xFF),
    OBJNAME("setball", PROC_Tag_SetBall,       0xFF),
    OBJNAME("atkItem", PROC_Tag_AttackItem,    0xFF),
    OBJNAME("TagStat", PROC_Tag_Statue,        0xFF),
    OBJNAME("zdoor",   PROC_Obj_ZDoor,         0xFF),
    OBJNAME("rdoor",   PROC_Obj_ZDoor,         0xFF),
    OBJNAME("STDoa",   PROC_Obj_ZDoor,         0xFF),
    OBJNAME("hasi00",  PROC_Obj_Pillar,        0xFF),
    OBJNAME("hasi01",  PROC_Obj_Pillar,        0xFF),
    OBJNAME("window",  PROC_Obj_Window,        0xFF),
    OBJNAME("bbox",    PROC_Obj_BBox,          0xFF),
    OBJNAME("spiralC", PROC_Obj_Lv3R10Saka,    0xFF),
    OBJNAME("spiral2", PROC_Obj_Lv3R10Saka,    0xFF),
    OBJNAME("digsnow", PROC_Obj_DigSnow,       0xFF),
    OBJNAME("digsand", PROC_Obj_Lv4DigSand,    0xFF),
    OBJNAME("fallobj", PROC_Obj_FallObj,       0xFF),
    OBJNAME("l7brg",   PROC_Obj_Lv7Bridge,     0xFF),
    OBJNAME("dr",      PROC_DR,                0xFF),
    OBJNAME("L7lowDr", PROC_L7lowDr,           0xFF),
    OBJNAME("swTime",  PROC_SwTime,            0xFF),
    OBJNAME("pdrobj",  PROC_Obj_PushDoor,      0xFF),
    OBJNAME("crystal", PROC_Obj_Crystal,       0xFF),
    OBJNAME("WarpBrg", PROC_Obj_KakarikoBrg,   0xFF),
    OBJNAME("WarpB2",  PROC_Obj_KakarikoBrg,   0xFF),
    OBJNAME("WarpOB1", PROC_Obj_OrdinBrg,      0x00),
    OBJNAME("WarpOB2", PROC_Obj_OrdinBrg,      0x01),
    OBJNAME("VolcGnd", PROC_Obj_VolcGnd,       0xFF),
    OBJNAME("r09wtr",  PROC_Obj_WaterEff,      0xFF),
    OBJNAME("ballSw",  PROC_Obj_SwBallA,       0xFF),
    OBJNAME("lballSw", PROC_Obj_SwBallB,       0xFF),
    OBJNAME("lbsw",    PROC_Obj_SwBallC,       0xFF),
    OBJNAME("R50Sand", PROC_Obj_Lv4Sand,       0xFF),
    OBJNAME("rwall",   PROC_Obj_Lv4RailWall,   0xFF),
    OBJNAME("l4brg",   PROC_Obj_Lv4Bridge,     0xFF),
    OBJNAME("l4floor", PROC_Obj_Lv4Floor,      0xFF),
    OBJNAME("item",    PROC_ITEM,              0xFF),
    OBJNAME("itemKey", PROC_Obj_SmallKey,      0xFF),
    OBJNAME("kantera", PROC_Obj_Kantera,       0xFF),
    OBJNAME("witem",   PROC_ITEM,              0xFF),
    OBJNAME("wshield", PROC_Obj_Shield,        0xFF),
    OBJNAME("htPiece", PROC_Obj_LifeContainer, 0xFF),
    OBJNAME("htCase",  PROC_Obj_LifeContainer, 0xFF),
    OBJNAME("Mbrid15", PROC_Obj_RotBridge,     0xFF),
    OBJNAME("Mbrid9",  PROC_Obj_RotBridge,     0xFF),
    OBJNAME("maglift", PROC_Obj_MagLift,       0xFF),
    OBJNAME("Cldst00", PROC_Obj_Lv1Cdl00,      0xFF),
    OBJNAME("Cldst01", PROC_Obj_Lv1Cdl01,      0xFF),
    OBJNAME("RMback0", PROC_Tag_Restart,       0xFF),
    OBJNAME("RMback1", PROC_Tag_ChgRestart,    0xFF),
    OBJNAME("Mist",    PROC_Tag_Mist,          0xFF),
    OBJNAME("magLifR", PROC_Obj_MagLiftRot,    0xFF),
    OBJNAME("TCdlst",  PROC_Obj_TvCdlst,       0xFF),
    OBJNAME("wdstick", PROC_Obj_WdStick,       0xFF),
    OBJNAME("stBlock", PROC_Obj_StairBlock,    0xFF),
    OBJNAME("hsMato",  PROC_Obj_HsTarget,      0xFF),
    OBJNAME("ktFire",  PROC_Tag_KtOnFire,      0xFF),
    OBJNAME("hswitch", PROC_Obj_HeavySw,       0xFF),
    OBJNAME("goGate",  PROC_Obj_GoGate,        0xFF),
    OBJNAME("taAmi",   PROC_Obj_TaFence,       0xFF),
    OBJNAME("fireWd",  PROC_Obj_FireWood,      0xFF),
    OBJNAME("fireWd2", PROC_Obj_FireWood2,     0xFF),
    OBJNAME("altar",   PROC_Obj_Saidan,        0xFF),
    OBJNAME("gpTaru",  PROC_Obj_GpTaru,        0xFF),
    OBJNAME("spinLf",  PROC_Obj_SpinLift,      0xFF),
    OBJNAME("bmWin",   PROC_Obj_BmWindow,      0xFF),
    OBJNAME("roofHl",  PROC_Obj_RfHole,        0xFF),
    OBJNAME("syRock",  PROC_Obj_SyRock,        0xFF),
    OBJNAME("wColumn", PROC_Obj_WaterPillar,   0xFF),
    OBJNAME("kpot",    PROC_Obj_KiPot,         0xFF),
    OBJNAME("bsGate",  PROC_Obj_BsGate,        0xFF),
    OBJNAME("amiShut", PROC_Obj_AmiShutter,    0xFF),
    OBJNAME("kwhel00", PROC_Obj_KWheel00,      0xFF),
    OBJNAME("kwhel01", PROC_Obj_KWheel01,      0xFF),
    OBJNAME("PRElvtr", PROC_Obj_PRElvtr,       0xFF),
    OBJNAME("swhel00", PROC_Obj_KWheel00,      0xFF),
    OBJNAME("M_hasu",  PROC_Obj_MHasu,         0xFF),
    OBJNAME("yiblltr", PROC_Obj_YIblltray,     0xFF),
    OBJNAME("l6egate", PROC_Obj_Lv6EGate,      0xFF),
    OBJNAME("l6eleva", PROC_Obj_Lv6ElevtA,     0xFF),
    OBJNAME("wtGate",  PROC_Obj_WtGate,        0xFF),
    OBJNAME("candlL2", PROC_Obj_Lv2Candle,     0xFF),
    OBJNAME("togeTp",  PROC_Obj_TogeTrap,      0xFF),
    OBJNAME("rotTrap", PROC_Obj_RotTrap,       0xFF),
    OBJNAME("klift00", PROC_Obj_KLift00,       0xFF),
    OBJNAME("l4chand", PROC_Obj_Lv4Chan,       0xFF),
    OBJNAME("Ychndlr", PROC_Obj_Ychndlr,       0xFF),
    OBJNAME("hbmbkoy", PROC_Obj_HBombkoya,     0xFF),
    OBJNAME("TagCsw",  PROC_TAG_CSW,           0xFF),
    OBJNAME("TagCswO", PROC_TAG_CSW,           0xFF),
    OBJNAME("P_Rwall", PROC_Obj_Lv4PRwall,     0xFF),
    OBJNAME("PDtile",  PROC_Obj_PDtile,        0xFF),
    OBJNAME("PDwall",  PROC_Obj_PDwall,        0xFF),
    OBJNAME("Qs",      PROC_TAG_QS,            0xFF),
    OBJNAME("mirror",  PROC_MIRROR,            0xFF),
    OBJNAME("rGate",   PROC_Obj_SwallShutter,  0xFF),
    OBJNAME("l3water", PROC_Obj_Lv3Water,      0xFF),
    OBJNAME("l3wat02", PROC_Obj_Lv3Water2,     0xFF),
    OBJNAME("l3watB",  PROC_OBJ_LV3WATERB,     0xFF),
    OBJNAME("szbridg", PROC_Obj_SZbridge,      0xFF),
    OBJNAME("kjgjs",   PROC_Obj_KJgjs,         0xFF),
    OBJNAME("kjs",     PROC_Obj_KJgjs,         0xFF),
    OBJNAME("candlL3", PROC_Obj_Lv3Candle,     0xFF),
    OBJNAME("l5icewl", PROC_Obj_IceWall,       0xFF),
    OBJNAME("Turara",  PROC_Obj_Turara,        0xFF),
    OBJNAME("twGate",  PROC_Obj_TwGate,        0xFF),
    OBJNAME("L4cdlTg", PROC_Tag_Lv4Candle,     0xFF),
    OBJNAME("L4cddTg", PROC_Tag_Lv4CandleDm,   0xFF),
    OBJNAME("togeRol", PROC_Obj_Lv6TogeRoll,   0xFF),
    OBJNAME("l6TogeT", PROC_Obj_Lv6TogeTrap,   0xFF),
    OBJNAME("Tenbin",  PROC_Obj_Lv6Tenbin,     0xFF),
    OBJNAME("l6Lblk",  PROC_Obj_Lv6Lblock,     0xFF),
    OBJNAME("l6SwGt",  PROC_Obj_Lv6SwGate,     0xFF),
    OBJNAME("l6ChBlk", PROC_Obj_Lv6ChgGate,    0xFF),
    OBJNAME("l6FuriT", PROC_Obj_Lv6FuriTrap,   0xFF),
    OBJNAME("KHdesk",  PROC_Obj_BarDesk,       0xFF),
    OBJNAME("Ytaihou", PROC_Obj_Ytaihou,       0xFF),
    OBJNAME("L4eShut", PROC_Obj_Lv4EdShutter,  0xFF),
    OBJNAME("pofire",  PROC_Obj_poFire,        0xFF),
    OBJNAME("pofiTg",  PROC_Tag_poFire,        0xFF),
    OBJNAME("poCandl", PROC_Obj_poCandle,      0xFF),
    OBJNAME("L4Gate",  PROC_Obj_Lv4Gate,       0xFF),
    OBJNAME("L4Pgate", PROC_Obj_Lv4PoGate,     0xFF),
    OBJNAME("L4SWall", PROC_Obj_Lv4SlideWall,  0xFF),
    OBJNAME("L4hmato", PROC_Obj_Lv4HsTarget,   0xFF),
    OBJNAME("propy",   PROC_Obj_Lv7PropY,      0xFF),
    OBJNAME("L7BsGt",  PROC_Obj_Lv7BsGate,     0xFF),
    OBJNAME("optLift", PROC_Obj_Lv8OptiLift,   0xFF),
    OBJNAME("kkiTrap", PROC_Obj_Lv8KekkaiTrap, 0xFF),
    OBJNAME("L8LiftX", PROC_Obj_Lv8Lift,       0xFF),
    OBJNAME("swStep",  PROC_Obj_Lv8UdFloor,    0xFF),
    OBJNAME("L9SwSht", PROC_Obj_Lv9SwShutter,  0xFF),
    OBJNAME("L5SwIce", PROC_Obj_Lv5SwIce,      0xFF),
    OBJNAME("glwSph",  PROC_Obj_glowSphere,    0xFF),
    OBJNAME("MR_Scrw", PROC_Obj_MirrorScrew,   0xFF),
    OBJNAME("MR_Sand", PROC_Obj_MirrorSand,    0xFF),
    OBJNAME("MR_Tble", PROC_Obj_MirrorTable,   0xFF),
    OBJNAME("MR_Chin", PROC_Obj_MirrorChain,   0xFF),
    OBJNAME("MR_Pole", PROC_Obj_Mirror6Pole,   0xFF),
    OBJNAME("PPolamp", PROC_PPolamp,           0xFF),
    OBJNAME("l5hYuka", PROC_Obj_Lv5FBoard,     0xFF),
    OBJNAME("BYRock",  PROC_BkyRock,           0xFF),
    OBJNAME("zrDrock", PROC_Obj_zrTurara,      0xFF),
    OBJNAME("tkrDai",  PROC_Obj_TakaraDai,     0xFF),
    OBJNAME("Table",   PROC_Obj_Table,         0xFF),
    OBJNAME("CatDoor", PROC_Obj_CatDoor,       0xFF),
    OBJNAME("RetTag",  PROC_Tag_RetRoom,       0xFF),
    OBJNAME("WdStone", PROC_Obj_WindStone,     0xFF),
    OBJNAME("HwlWara", PROC_Tag_WaraHowl,      0xFF),
    OBJNAME("SCannon", PROC_Obj_SCannon,       0xFF),
    OBJNAME("szGate",  PROC_Obj_Lv6SzGate,     0xFF),
    OBJNAME("onsTaru", PROC_Obj_OnsenTaru,     0xFF),
    OBJNAME("WStoneF", PROC_Obj_SmWStone,      0xFF),
    OBJNAME("SCanCrs", PROC_Obj_SCannonCrs,    0xFF),
    OBJNAME("snwEfTg", PROC_Tag_SnowEff,       0xFF),
    OBJNAME("tmFire",  PROC_Obj_TimeFire,      0xFF),
    OBJNAME("L7Prop",  PROC_Obj_Prop,          0xFF),
    OBJNAME("awaPlar", PROC_Obj_awaPlar,       0xFF),
    OBJNAME("poTbox",  PROC_Obj_poTbox,        0xFF),
    OBJNAME("SnwSoup", PROC_Obj_SnowSoup,      0xFF),
    OBJNAME("Nagaisu", PROC_Obj_Nagaisu,       0xFF),
    OBJNAME("RCircle", PROC_Obj_RCircle,       0xFF),
    OBJNAME("L9Chand", PROC_Obj_Chandelier,    0xFF),
    OBJNAME("L9Pictr", PROC_Obj_Picture,       0xFF),
    OBJNAME("SmkEmt",  PROC_Tag_SmkEmt,        0xFF),
    OBJNAME("HFtr",    PROC_Obj_HFtr,          0xFF),
    OBJNAME("HBarrel", PROC_Obj_HBarrel,       0xFF),
    OBJNAME("SCanTen", PROC_Obj_SCannonTen,    0xFF),
    OBJNAME("cstaSw",  PROC_Tag_CstaSw,        0xFF),
    OBJNAME("l6cstSw", PROC_Tag_Lv6CstaSw,     0xFF),
    OBJNAME("Hata",    PROC_Obj_Hata,          0xFF),
    OBJNAME("RmbitSw", PROC_Tag_RmbitSw,       0xFF),
    OBJNAME("T_Maki",  PROC_Obj_ToaruMaki,     0xFF),
    OBJNAME("d_act",   PROC_DEMO00,            0xFF),
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
    OBJNAME("TLogo",   PROC_TITLE,             0xFF),
#ifdef DEBUG
    OBJNAME("unitC00", PROC_Obj_TestCube,      0xFF),
    OBJNAME("unitS00", PROC_Obj_TestCube,      0xFF),
    OBJNAME("unitCy0", PROC_Obj_TestCube,      0xFF),
    OBJNAME("damCps",  PROC_Obj_DamCps,        0xFF),
    OBJNAME("railDrw", PROC_PATH_LINE,         0xFF),
    OBJNAME("sdoor",   PROC_SPIRAL_DOOR,       0xFF),
#endif
    OBJNAME("ClearB",  PROC_Obj_Cboard,        0xFF),
    OBJNAME("Bg",      PROC_BG,                0xFF),
    OBJNAME("Boom",    PROC_BOOMERANG,         0xFF),
    OBJNAME("Nbomb",   PROC_NBOMB,             0xFF),
    OBJNAME("Arrow",   PROC_ARROW,             0xFF),
    OBJNAME("E_Arrow", PROC_E_ARROW,           0xFF),
    OBJNAME("Spinner", PROC_SPINNER,           0xFF),
    OBJNAME("Crod",    PROC_CROD,              0xFF),
    OBJNAME("Midna",   PROC_MIDNA,             0xFF),
    OBJNAME("Vrbox",   PROC_VRBOX,             0xFF),
    OBJNAME("Vrbox2",  PROC_VRBOX2,            0xFF),
    OBJNAME("mvbg_a",  PROC_BG_OBJ,            0xFF),
    OBJNAME("burnBox", PROC_Obj_BurnBox,       0xFF),
    OBJNAME("stnMark", PROC_Obj_StoneMark,     0xFF),
#ifndef DEBUG
    OBJNAME("Passer",  PROC_NPC_PASSER,        0xFF),
#endif
    OBJNAME("Passer2", PROC_NPC_PASSER2,       0xFF),
    OBJNAME("ShopItm", PROC_ShopItem,          0xFF),
    OBJNAME("E_yd_lf", PROC_E_YD_LEAF,         0xFF),
    OBJNAME("E_db_lf", PROC_E_DB_LEAF,         0xFF),
    OBJNAME("E_hb_lf", PROC_E_HB_LEAF,         0xFF),
    OBJNAME("E_bi_lf", PROC_E_BI_LEAF,         0xFF),
    OBJNAME("O_Mato",  PROC_Obj_Mato,          0xFF),
    OBJNAME("O_Flag",  PROC_Obj_Flag,          0xFF),
    OBJNAME("O_Flag2", PROC_Obj_Flag2,         0xFF),
    OBJNAME("O_Flag3", PROC_Obj_Flag3,         0xFF),
    OBJNAME("CRVLH_U", PROC_Obj_CRVLH_UP,      0xFF),
    OBJNAME("DemoItm", PROC_Demo_Item,         0xFF),
    OBJNAME("EndCode", PROC_PLAY_SCENE,        0xFF),
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
#ifdef DEBUG
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
#ifdef DEBUG
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
    static char tmp_name[8];

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
        snprintf(tmp_name, 8, "%d%+0d", procName, argument);
        tmp = tmp_name;
    }
    return tmp;
}

const char* dStage_getName2(s16 procName, s8 argument) {
    return dStage_getName(procName, argument);
}

u32 dStage_roomControl_c::mProcID;

/* 80450D64 0001+00 data_80450D64 None */
s8 dStage_roomControl_c::mStayNo;

/* 80450D65 0001+00 data_80450D65 None */
s8 dStage_roomControl_c::mOldStayNo;

/* 80450D66 0001+00 data_80450D66 None */
s8 dStage_roomControl_c::mNextStayNo;

/* 80450D67 0001+00 data_80450D67 None */
u8 dStage_roomControl_c::m_time_pass;

u8 dStage_roomControl_c::mNoChangeRoom;

dStage_roomControl_c::nameData* dStage_roomControl_c::mArcBankName;

dStage_roomControl_c::bankData* dStage_roomControl_c::mArcBankData;

s8 dStage_roomControl_c::mRoomReadId = 0xFF;

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
        if ((int)rtbl[i] < 0x80000000) {
            rtbl[i] = (roomRead_data_class*)((int)rtbl[i] + (int)param_3);
            rtbl[i]->m_rooms = (u8*)((int)rtbl[i]->m_rooms + (int)param_3);
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
    (void)i_num;
    (void)param_3;
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
    (void)param_2;
    (void)param_3;
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
    (void)param_2;
    (void)param_3;
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
    (void)entryNum;
    (void)param_3;
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
