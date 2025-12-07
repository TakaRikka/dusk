/**
 * @file d_a_npc_kasi_hana.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_kasi_hana.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc_chin.h"
#include "d/actor/d_a_npc_kasi_kyu.h"
#include "d/actor/d_a_npc_kasi_mich.h"
#include "d/d_timer.h"
#include "Z2AudioLib/Z2Instances.h"

enum kasi_hana_RES_File_ID {
    /* BMDR */
    /* 0x5 */ BMDR_HANA = 0x5,

    /* BTP */
    /* 0x8 */ BTP_HANA = 0x8,

    /* EVT */
    /* 0xB */ EVT_EVENT_LIST = 0xB,
};

enum girls_RES_File_ID {
    /* BCK */
    /* 0x3 */ BCK_MICH_IYAN_WAIT = 0x3,
    /* 0x4 */ BCK_MICH_KYA_TALK,
    /* 0x5 */ BCK_MICH_OUEN_WAIT_A,
    /* 0x6 */ BCK_MICH_OUEN_WAIT_B,
};

enum Wgeneral_RES_File_ID {
    /* BCK */
    /* 0x03 */ BCK_W_2LADYTALK_A = 0x3,
    /* 0x04 */ BCK_W_2LADYTALK_B,
    /* 0x05 */ BCK_W_2NORMALTALK_A,
    /* 0x06 */ BCK_W_2NORMALTALK_B,
    /* 0x07 */ BCK_W_BROWSE_A,
    /* 0x08 */ BCK_W_BROWSE_B,
    /* 0x09 */ BCK_W_CELLME,
    /* 0x0A */ BCK_W_LOOK_A,
    /* 0x0B */ BCK_W_LOOK_B,
    /* 0x0C */ BCK_W_MARO_DANCE,
    /* 0x0D */ BCK_W_RUN_A,
    /* 0x0E */ BCK_W_RUN_B,
    /* 0x0F */ BCK_W_SING,
    /* 0x10 */ BCK_W_SIT_TO_WOLF_A,
    /* 0x11 */ BCK_W_SIT_TO_WOLF_B,
    /* 0x12 */ BCK_W_SITTALK_A,
    /* 0x13 */ BCK_W_SITTALK_A_B,
    /* 0x14 */ BCK_W_SITTALK_B,
    /* 0x15 */ BCK_W_SITTALK_B_B,
    /* 0x16 */ BCK_W_SITWAIT_A,
    /* 0x17 */ BCK_W_SITWAIT_B,
    /* 0x18 */ BCK_W_SURPRISE,
    /* 0x19 */ BCK_W_TALK_A,
    /* 0x1A */ BCK_W_TALK_B,
    /* 0x1B */ BCK_W_TALK_B_WALL,
    /* 0x1C */ BCK_W_TALK_C,
    /* 0x1D */ BCK_W_TALK_WALL,
    /* 0x1E */ BCK_W_TO_WOLF,
    /* 0x1F */ BCK_W_WAIT_A,
    /* 0x20 */ BCK_W_WAIT_B,
    /* 0x21 */ BCK_W_WAIT_WALL,
    /* 0x22 */ BCK_W_WALK_A,
    /* 0x23 */ BCK_W_WALK_B,
};

enum RES_Name {
    /* 0x0 */ KASI_HANA,
    /* 0x1 */ GIRLS,
    /* 0x2 */ WGENERAL,
};

enum Animation {
    /* 0x0 */ ANM_MICH_IYAN_WAIT,
    /* 0x1 */ ANM_MICH_KYA_TALK,
    /* 0x2 */ ANM_MICH_OUEN_WAIT_A,
    /* 0x3 */ ANM_MICH_OUEN_WAIT_B,
    /* 0x4 */ ANM_W_WAIT_A,
    /* 0x5 */ ANM_W_TALK_B,
    /* 0x6 */ ANM_W_TO_WOLF,
    /* 0x7 */ ANM_W_WALK_A,
    /* 0x8 */ ANM_W_LOOK_B,
    /* 0x9 */ ANM_W_RUN_A,
    /* 0xA */ ANM_W_SURPRISE,
    /* 0xB */ ANM_W_WAIT_A_2,
    /* 0xC */ ANM_W_TALK_A,
    /* 0xD */ ANM_W_2LADYTALK_B,
    /* 0xE */ ANM_W_2NORMALTALK_B,
};

enum Motion {
    /* 0x0 */ MOT_MICH_KYA_TALK,
    /* 0x1 */ MOT_MICH_IYAN_WAIT,
    /* 0x2 */ MOT_MICH_OUEN_WAIT_A,
    /* 0x3 */ MOT_MICH_OUEN_WAIT_B,
    /* 0x4 */ MOT_W_WAIT_A,
    /* 0x5 */ MOT_W_TALK_B,
    /* 0x6 */ MOT_W_TO_WOLF,
    /* 0x7 */ MOT_W_WALK_A,
    /* 0x8 */ MOT_W_LOOK_B,
    /* 0x9 */ MOT_W_RUN_A,
    /* 0xA */ MOT_W_SURPRISE,
    /* 0xB */ MOT_W_WAIT_A_LOOP,
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_BACKBONE,
    /* 0x02 */ JNT_NECK,
    /* 0x03 */ JNT_HEAD,
    /* 0x04 */ JNT_SHOULDERL,
    /* 0x05 */ JNT_ARML1,
    /* 0x06 */ JNT_ARML2,
    /* 0x07 */ JNT_HANDL,
    /* 0x08 */ JNT_SHOULDERR,
    /* 0x09 */ JNT_ARMR1,
    /* 0x0A */ JNT_ARMR2,
    /* 0x0B */ JNT_HANDR,
    /* 0x0C */ JNT_WAIST,
    /* 0x0D */ JNT_LEGL1,
    /* 0x0E */ JNT_LEGL2,
    /* 0x0F */ JNT_FOOTL,
    /* 0x10 */ JNT_SKIRTL,
    /* 0x11 */ JNT_LEGR1,
    /* 0x12 */ JNT_LEGR2,
    /* 0x13 */ JNT_FOOTR,
    /* 0x14 */ JNT_SKIRTR,
};

enum Event_Names {
    /* 0x0 */ EVT_NONE,
    /* 0x1 */ EVT_APPEAR,
    /* 0x2 */ EVT_TALK,
    /* 0x3 */ EVT_HEART,
    /* 0x4 */ EVT_CHEER,
    /* 0x5 */ EVT_CHEER2,
};

enum Type {
    /* 0x0 */ TYPE_WAIT,
    /* 0x1 */ TYPE_CHEER,
};

static NPC_KASI_HANA_HIO_CLASS l_HIO;

void daNpcKasi_Mng_c::calcEscapeForm() {
    fopAc_ac_c* hana_p = mHanaActorMngr.getActorP();
    if (hana_p != NULL){
        cXyz sp1c(0.0f, 0.0f, -70.0f);
        cXyz sp28(0.0f, 0.0f, -140.0f);
        cXyz sp34 = fopAcM_GetPosition(hana_p);
        cXyz sp40;
        getOffsetFromLeader(sp1c, sp40);
        setKyuPos(sp34 + sp40);
        getOffsetFromLeader(sp28, sp40);
        setMichPos(sp34 + sp40);
    }
}

BOOL daNpcKasi_Mng_c::calcCenterPos() {
    fopAc_ac_c* hana_p = mHanaActorMngr.getActorP();
    fopAc_ac_c* kyu_p = mKyuActorMngr.getActorP();
    fopAc_ac_c* mich_p = mMichActorMngr.getActorP();

    if (hana_p == NULL || kyu_p == NULL || mich_p == NULL) {
        return FALSE;
    }
    
    cXyz hana_pos(*fopAcM_GetPosition_p(hana_p));
    cXyz kyu_pos(*fopAcM_GetPosition_p(kyu_p));
    cXyz mich_pos(*fopAcM_GetPosition_p(mich_p));
    f32 pX = (hana_pos.x + kyu_pos.x + mich_pos.x) / 3.0f;
    f32 pY = (hana_pos.y + kyu_pos.y + mich_pos.y) / 3.0f;
    f32 pZ = (hana_pos.z + kyu_pos.z + mich_pos.z) / 3.0f;
    mCenterPos.set(pX, pY, pZ);

    return TRUE;
}

f32 daNpcKasi_Mng_c::getDistFromCenter() {
    f32 rv;

    if (!calcCenterPos()) {
        return -1.0f;
    }

    cXyz sp1c = fopAcM_GetPosition(daPy_getPlayerActorClass());
    cXyz sp28 = sp1c - mCenterPos;
    return sp28.absXZ();
}

void daNpcKasi_Mng_c::getOffsetFromLeader(cXyz const& param_1, cXyz& param_2) {
    param_2 = param_1;

    fopAc_ac_c* hana_p = mHanaActorMngr.getActorP();
    if (hana_p != NULL) {
        csXyz angle(*fopAcM_GetShapeAngle_p(hana_p));
        mDoMtx_stack_c::ZXYrotS(angle);
        mDoMtx_stack_c::multVec(&param_2, &param_2);
    }
}

void daNpcKasi_Mng_c::initPath(u8 i_pathNo, u16 i_idx) {
    if (!mIsPathInit) {
        fopAc_ac_c* hana_p = mHanaActorMngr.getActorP();
        if (hana_p != NULL) {
            mPath.initialize();
            mPath.setPathInfo(i_pathNo, fopAcM_GetRoomNo(hana_p), 0);
            mPath.setIdx(i_idx);

            OS_REPORT("---------------------dpath initialize!!\n");

            mIsPathInit = true;
        }
    }
}

int daNpcKasi_Mng_c::getPlNearPoint() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp58(*fopAcM_GetPosition_p(player));
    int rv = 0;
    f32 fVar1, fVar2, fVar3;
    fVar3 = 0.0f;
    cXyz sp64, sp70;

    for (int i = 0; i < 10; i++) {
        sp64 = mPath.getPntPos(i);
        sp70 = sp64 - sp58;
        fVar1 = sp70.abs2XZ();

        if (i == 0) {
            rv = 0;
            fVar3 = fVar1;
        } else if (fVar3 > fVar1) {
            rv = i;
            fVar3 = fVar1;
        } else if (fVar1 > fVar2) break;

        fVar2 = fVar1;
    }

    mPlPoint = rv;
    return rv;
}

int daNpcKasi_Mng_c::getSygnalInfo() {
    int iVar1 = 1;
    for (int i = 0; i < 20; i++) {
        if (chkSygnal(iVar1)) {
            return i;
        }

        iVar1 <<= 1;
    }

    return -1;
}

void daNpcKasi_Mng_c::sendInfo() {
    int sygnalInfo = getSygnalInfo();
    daNpcKasiKyu_c* kyu_p = (daNpcKasiKyu_c*)mKyuActorMngr.getActorP();
    if (kyu_p != NULL) {
        kyu_p->setSygnal(sygnalInfo);
        kyu_p->setChacePos(mKyuPos);
        kyu_p->setCenterPos(mCenterPos);
        kyu_p->setPlPoint(mPlPoint);
    }

    daNpcKasiMich_c* michP = (daNpcKasiMich_c*)mMichActorMngr.getActorP();
    if (michP != NULL) {
        michP->setSygnal(sygnalInfo);
        michP->setChacePos(mMichPos);
        michP->setCenterPos(mCenterPos);
        michP->setPlPoint(mPlPoint);
    }
}

void daNpcKasi_Mng_c::offTalk() {
    daNpcKasiHana_c* hana_p = (daNpcKasiHana_c*)mHanaActorMngr.getActorP();
    daNpcKasiKyu_c* kyu_p = (daNpcKasiKyu_c*)mKyuActorMngr.getActorP();
    daNpcKasiMich_c* mich_p = (daNpcKasiMich_c*)mMichActorMngr.getActorP();

    if (hana_p != NULL) {
        hana_p->off_talked();
    }

    if (kyu_p != NULL) {
        kyu_p->off_talked();
    }

    if (mich_p != NULL) {
        mich_p->off_talked();
    }
}

BOOL daNpcKasi_Mng_c::chkTalk() {
    daNpcKasiHana_c* hana_p = (daNpcKasiHana_c*)mHanaActorMngr.getActorP();
    daNpcKasiKyu_c* kyu_p = (daNpcKasiKyu_c*)mKyuActorMngr.getActorP();
    daNpcKasiMich_c* mich_p = (daNpcKasiMich_c*)mMichActorMngr.getActorP();

    if (hana_p == NULL || kyu_p == NULL || mich_p == NULL) {
        OS_REPORT("---------------------some girl no exist!!\n");

        return FALSE;
    }

    bool rv = true;
    bool bVar1 = true;

    if (!hana_p->is_talked() && !kyu_p->is_talked()) {
        bVar1 = false;
    }

    if (!bVar1 && !mich_p->is_talked()) {
        rv = false;
    }

    return rv;
}

BOOL daNpcKasi_Mng_c::chkFear() {
    daNpcKasiKyu_c* kyu_p = (daNpcKasiKyu_c*)mKyuActorMngr.getActorP();
    if (kyu_p != NULL && kyu_p->is_fear()) {
        return TRUE;
    }

    daNpcKasiMich_c* mich_p = (daNpcKasiMich_c*)mMichActorMngr.getActorP();
    if (mich_p != NULL && mich_p->is_fear()) {
        return TRUE;
    }

    return FALSE;
}

BOOL daNpcKasi_Mng_c::chkEscape() {
    daNpcKasiHana_c* hana_p = (daNpcKasiHana_c*)mHanaActorMngr.getActorP();
    daNpcKasiKyu_c* kyu_p = (daNpcKasiKyu_c*)mKyuActorMngr.getActorP();
    daNpcKasiMich_c* mich_p = (daNpcKasiMich_c*)mMichActorMngr.getActorP();

    if (hana_p == NULL || kyu_p == NULL || mich_p == NULL) {
        OS_REPORT("-------- caution : some girl not exist!!\n");

        return TRUE;
    }

    bool rv = false;
    bool bVar1 = false;

    if (hana_p->is_escape() && kyu_p->is_escape()) {
        bVar1 = true;
    }

    if (bVar1 && mich_p->is_escape()) {
        rv = true;
    }

    return rv;
}

BOOL daNpcKasi_Mng_c::createHearts() {
    fopAc_ac_c* actors[3] = {NULL, NULL, NULL};
    actors[0] = mHanaActorMngr.getActorP();
    actors[1] = mKyuActorMngr.getActorP();
    actors[2] = mMichActorMngr.getActorP();
    f32 i_speedF = 0.0f;
    f32 i_speedY = 30.0f;
    cXyz i_scale(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < 3; i++) {
        fopAc_ac_c* actor_p = actors[i];
        if (actor_p != NULL) {
            cXyz i_pos(*fopAcM_GetPosition_p(actor_p));
            i_pos.y += 115.0f;
            fopAc_ac_c* heart_p = NULL;
            csXyz i_angle(0, cLib_getRndValue<int>(0, 7) << 13, 0);
            heart_p = fopAcM_fastCreateItem(&i_pos, fpcNm_ITEM_HEART, fopAcM_GetRoomNo(actor_p), &i_angle,
                                           &i_scale, &i_speedF, &i_speedY, -1, 0, NULL);
            if (heart_p != NULL) {
                Z2GetAudioMgr()->seStart(Z2SE_GIRLS_HEART, &i_pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                fopAcM_OnStatus(heart_p, fopAcM_STATUS_UNK_0x4000);
            }
        }
    }

    return TRUE;
}

void daNpcKasi_Mng_c::allDemoMove() {
    daNpcKasiHana_c* hana_p = (daNpcKasiHana_c*)mHanaActorMngr.getActorP();
    if (hana_p != NULL) {
        fopAcM_OnStatus(hana_p, fopAcM_STATUS_UNK_0x4000);
    }

    daNpcKasiKyu_c* kyu_p = (daNpcKasiKyu_c*)mKyuActorMngr.getActorP();
    if (kyu_p != NULL) {
        fopAcM_OnStatus(kyu_p, fopAcM_STATUS_UNK_0x4000);
    }

    daNpcKasiMich_c* mich_p = (daNpcKasiMich_c*)mMichActorMngr.getActorP();
    if (mich_p != NULL) {
        fopAcM_OnStatus(mich_p, fopAcM_STATUS_UNK_0x4000);
    }
}

void daNpcKasi_Mng_c::allDemoNotMove() {
    daNpcKasiHana_c* hana_p = (daNpcKasiHana_c*)mHanaActorMngr.getActorP();
    if (hana_p != NULL) {
        fopAcM_OffStatus(hana_p, fopAcM_STATUS_UNK_0x4000);
    }

    daNpcKasiKyu_c* kyu_p = (daNpcKasiKyu_c*)mKyuActorMngr.getActorP();
    if (kyu_p != NULL) {
        fopAcM_OffStatus(kyu_p, fopAcM_STATUS_UNK_0x4000);
    }

    daNpcKasiMich_c* mich_p = (daNpcKasiMich_c*)mMichActorMngr.getActorP();
    if (mich_p != NULL) {
        fopAcM_OffStatus(mich_p, fopAcM_STATUS_UNK_0x4000);
    }
}

BOOL daNpcKasi_Mng_c::isMiniGamePlaying() {
    BOOL rv = FALSE;
    daNpcChin_c* dancho_p = (daNpcChin_c*)mDanchoActorMngr.getActorP();

    if (dancho_p != NULL) {
        rv = dancho_p->isMiniGamePlaying();
    }

    return rv;
}

void daNpcKasi_Mng_c::schMemberActor() {
    if (mKyuActorMngr.getPId() == fpcM_ERROR_PROCESS_ID_e) {
        fopAc_ac_c* kyu_p = NULL;
        if (fopAcM_SearchByName(PROC_NPC_KASIKYU, (fopAc_ac_c**)&kyu_p) != 0) {
            setKyuActor(kyu_p);
        }
    }

    if (mMichActorMngr.getPId() == fpcM_ERROR_PROCESS_ID_e) {
        fopAc_ac_c* mich_p = NULL;

        if (fopAcM_SearchByName(PROC_NPC_KASIMICH, (fopAc_ac_c**)&mich_p) != 0) {
            setMichActor(mich_p);
        }
    }
}

void daNpcKasi_Mng_c::schDanchoActor() {
    if (mDanchoActorMngr.getPId() == fpcM_ERROR_PROCESS_ID_e) {
        fopAc_ac_c* dancho_p = NULL;

        if (fopAcM_SearchByName(PROC_NPC_CHIN, (fopAc_ac_c**)&dancho_p) != 0) {
            setDanchoActor(dancho_p);
        }
    }
}

void daNpcKasi_Mng_c::deleteAllMember() {
    fopAc_ac_c* kyu_p = mKyuActorMngr.getActorP();
    if (kyu_p != NULL) {
        fopAcM_delete(kyu_p);
        mKyuActorMngr.remove();
    }

    fopAc_ac_c* mich_p = mMichActorMngr.getActorP();
    if (mich_p != NULL) {
        fopAcM_delete(mich_p);
        mMichActorMngr.remove();
    }

    fopAc_ac_c* hana_p = mHanaActorMngr.getActorP();
    if (hana_p != NULL) {
        fopAcM_delete(hana_p);
        mHanaActorMngr.remove();
    }

    OS_REPORT("------------- three girl all delete!!\n");
}

void daNpcKasi_Mng_c::chgWeightHeavy() {
    daNpcKasiHana_c* hana_p = (daNpcKasiHana_c*)mHanaActorMngr.getActorP();
    if (hana_p != NULL) {
        hana_p->chgWeightHeavy();
    }

    daNpcKasiKyu_c* kyu_p = (daNpcKasiKyu_c*)mKyuActorMngr.getActorP();
    if (kyu_p != NULL) {
        kyu_p->chgWeightHeavy();
    }

    daNpcKasiMich_c* mich_p = (daNpcKasiMich_c*)mMichActorMngr.getActorP();
    if (mich_p != NULL) {
        mich_p->chgWeightHeavy();
    }
}

void daNpcKasi_Mng_c::chgWeightLight() {
    daNpcKasiHana_c* hana_p = (daNpcKasiHana_c*)mHanaActorMngr.getActorP();
    if (hana_p != NULL) {
        hana_p->chgWeightLight();
    }

    daNpcKasiKyu_c* kyu_p = (daNpcKasiKyu_c*)mKyuActorMngr.getActorP();
    if (kyu_p != NULL) {
        kyu_p->chgWeightLight();
    }

    daNpcKasiMich_c* mich_p = (daNpcKasiMich_c*)mMichActorMngr.getActorP();
    if (mich_p != NULL) {
        mich_p->chgWeightLight();
    }
}

static daNpc_GetParam2 l_bckGetParamList[15] = {
    {BCK_MICH_IYAN_WAIT, J3DFrameCtrl::EMode_LOOP, GIRLS}, 
    {BCK_MICH_KYA_TALK, J3DFrameCtrl::EMode_LOOP, GIRLS},
    {BCK_MICH_OUEN_WAIT_A, J3DFrameCtrl::EMode_LOOP, GIRLS},
    {BCK_MICH_OUEN_WAIT_B, J3DFrameCtrl::EMode_LOOP, GIRLS},
    {BCK_W_WAIT_A, J3DFrameCtrl::EMode_LOOP, WGENERAL},
    {BCK_W_TALK_B, J3DFrameCtrl::EMode_NONE, WGENERAL},
    {BCK_W_TO_WOLF, J3DFrameCtrl::EMode_LOOP, WGENERAL},
    {BCK_W_WALK_A, J3DFrameCtrl::EMode_LOOP, WGENERAL},
    {BCK_W_LOOK_B, J3DFrameCtrl::EMode_LOOP, WGENERAL},
    {BCK_W_RUN_A, J3DFrameCtrl::EMode_LOOP, WGENERAL},
    {BCK_W_SURPRISE, J3DFrameCtrl::EMode_NONE, WGENERAL},
    {BCK_W_WAIT_A, J3DFrameCtrl::EMode_NONE, WGENERAL},
    {BCK_W_TALK_A, J3DFrameCtrl::EMode_NONE, WGENERAL},
    {BCK_W_2LADYTALK_B, J3DFrameCtrl::EMode_NONE, WGENERAL},
    {BCK_W_2NORMALTALK_B, J3DFrameCtrl::EMode_NONE, WGENERAL},
};

static daNpc_GetParam2 l_btpGetParamList[1] = {
    {BTP_HANA, J3DFrameCtrl::EMode_LOOP, KASI_HANA},
};

static char* l_arcNames[3] = {
    "kasi_hana",
    "girls",
    "Wgeneral",
};

static char* l_evtNames[6] = {
    NULL,
    "KASIMASI_APPEAR",
    "KASIMASI_TALK",
    "KASIMASI_HEART",
    "KASIMASI_CHEER",
    "KASIMASI_CHEER2",
};

static char* l_myName = "kasi_hana";

daNpcKasiHana_HIOParam const daNpcKasiHana_Param_c::m = {
    55.0f,
    -3.0f,
    1.0f,
    500.0f,
    216.0f,
    140.0f,
    35.0f,
    30.0f,
    0.0f,
    0.0f,
    10.0f,
    -10.0f,
    30.0f,
    -10.0f,
    45.0f,
    -45.0f,
    0.6f,
    12.0f,
    2,
    6,
    3,
    6,
    60.0f,
    500.0f,
    300.0f,
    -300.0f,
    60,
    0,
    0,
    0,
    0,
    false,
    false,
    800.0f,
    400.0f,
    14.0f,
    16.0f,
};

#if DEBUG
daNpcKasiHana_HIO_c::daNpcKasiHana_HIO_c() {
    m = daNpcKasiHana_Param_c::m;
}

void daNpcKasiHana_HIO_c::genMessage(JORMContext* ctext) {
    ctext->genSlider("追跡開始距離", &m.track_start_dist, 0.0f, 5000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("追跡停止距離", &m.track_stop_dist, 0.0f, 5000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("追跡速度", &m.track_spd, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("逃げる速度", &m.escape_spd, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    daNpcF_commonGenMessage(ctext, &m.common);
}
#endif

daNpcKasiHana_c::EventFn daNpcKasiHana_c::mEvtSeqList[6] = {
    NULL,
    &daNpcKasiHana_c::_Evt_Kasi_Appear,
    &daNpcKasiHana_c::_Evt_Kasi_Talk,
    &daNpcKasiHana_c::_Evt_Kasi,
    &daNpcKasiHana_c::_Evt_Kasi_Cheer,
    &daNpcKasiHana_c::_Evt_Kasi_Cheer2,
};

daNpcKasiHana_c::daNpcKasiHana_c() {}

daNpcKasiHana_c::~daNpcKasiHana_c() {
    for (int i = 0; i < 3; i++) {
        dComIfG_resDelete(&mPhases[i], l_arcNames[i]);
    }
    
    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }

#if DEBUG
    if (mHIO) {
        mHIO->removeHIO();
    }
#endif
}

cPhs__Step daNpcKasiHana_c::Create() {
    fopAcM_ct(this, daNpcKasiHana_c);

    mMessageNo = getMessageNo();
    mType = getType();

    cPhs__Step phase = cPhs_ERROR_e;
    for (int i = 0; i < 3; i++) {
        phase = (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_arcNames[i]);

        if (phase != cPhs_COMPLEATE_e) {
            return phase;
        }
    }

    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x1940)) {
            return cPhs_ERROR_e;
        }

        J3DModelData* model_data = mAnm_p->getModel()->getModelData();
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -60.0f, -10.0f, -60.0f, 60.0f, 160.0f, 60.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
#if DEBUG
        mHIO = &l_HIO;
        // three daughters: Hana:
        mHIO->entryHIO("三人娘：ハナ");
#endif

        mAcchCir.SetWall(mHIO->m.common.width, mHIO->m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                  fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.CrrPos(dComIfG_Bgsp());
        mCcStts.Init(0xFE, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgType(0);
        mCyl.SetTgSPrm(0);
        mCyl.SetH(mHIO->m.common.height);
        mCyl.SetR(mHIO->m.common.width);
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();
        reset();
        Execute();
    }

    return phase;
}

int daNpcKasiHana_c::CreateHeap() {
    J3DModelData* mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_arcNames[0], 5);

    JUT_ASSERT(950, NULL != mdlData_p);
    u32 reg_r25 = 0x11020084;

    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, reg_r25);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    cXyz i_scale(1.0f, 0.8630768f, 1.0f);
    mAnm_p->offTranslate();
    mAnm_p->setTranslateScale(i_scale);

    J3DModel* model = mAnm_p->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    setMotion(MOT_W_WAIT_A, -1.0f, 0);

    return 1;
}

int daNpcKasiHana_c::Delete() {
    this->~daNpcKasiHana_c();
    return 1;
}

int daNpcKasiHana_c::Execute() {
    execute();
    return 1;
}

int daNpcKasiHana_c::Draw() {
    if (!mEscape) {
        draw(FALSE, FALSE, mHIO->m.common.real_shadow_size, NULL, FALSE);
    }

    return 1;
}

int daNpcKasiHana_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jntNo = i_joint->getJntNo();
    int i_jointList[3] = {JNT_BACKBONE, JNT_NECK, JNT_HEAD};

    if (jntNo == JNT_CENTER) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_BACKBONE));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_NECK));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));

    switch (jntNo) {
        case JNT_BACKBONE:
        case JNT_NECK:
        case JNT_HEAD:
            setLookatMtx(jntNo, i_jointList, mHIO->m.common.neck_rotation_ratio);
            break;
    }

    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

int daNpcKasiHana_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpcKasiHana_c* i_this = (daNpcKasiHana_c*)a_this;
    return i_this->CreateHeap();
}

int daNpcKasiHana_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpcKasiHana_c* i_this = (daNpcKasiHana_c*)model->getUserArea();

        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daNpcKasiHana_c::setParam() {
    if (mType == TYPE_WAIT) {
        mKasiMng.initPath(getRailNo(), 6);
    }

    attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(mHIO->m.common.attention_distance, mHIO->m.common.attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(mHIO->m.common.talk_distance, mHIO->m.common.talk_angle);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

#if DEBUG
    scale.set(mHIO->m.common.scale, mHIO->m.common.scale, mHIO->m.common.scale);
    mAcchCir.SetWallR(mHIO->m.common.width);
    mAcchCir.SetWallH(mHIO->m.common.knee_length);
    gravity = mHIO->m.common.gravity;
#endif
}

BOOL daNpcKasiHana_c::main() {
    mKasiMng.SygnalClear();
    mKasiMng.schMemberActor();

    if (mType == TYPE_CHEER) {
        mKasiMng.schDanchoActor();
    }

    if (mKasiMng.chkTalk()) {
        mKasiMng.offTalk();

        if (mType == TYPE_WAIT) {
            if (chkAction(&daNpcKasiHana_c::wait)) {
                if (fopAcM_isSwitch(this, getSwitchBitNo())) {
                    mKasiMng.chgWeightHeavy();
                    mOrderEvtNo = EVT_HEART;
                    changeEvent("kasi_hana", l_evtNames[mOrderEvtNo], 2, 0xFFFF);
                    setAction(&daNpcKasiHana_c::demo);
                } else {
                    mKasiMng.allDemoMove();
                    mOrderEvtNo = EVT_TALK;
                    changeEvent("kasi_hana", l_evtNames[mOrderEvtNo], 2, 0xFFFF);
                    setAction(&daNpcKasiHana_c::demo);
                }
            }
        } else if (mType == TYPE_CHEER) {
            if (chkAction(&daNpcKasiHana_c::cheer)) {
                /* dSv_event_flag_c::F_0293 - Castle Town - Star Game 2 cleared */
                if (daNpcF_chkEvtBit(293)) {
                    mOrderEvtNo = EVT_CHEER2;
                } else {
                    mOrderEvtNo = EVT_CHEER;
                }

                dComIfG_TimerStop(6);
                dTimer_hide();
                changeEvent("kasi_hana", l_evtNames[mOrderEvtNo], 2, 0xFFFF);
                setAction(&daNpcKasiHana_c::demo);
                mKasiMng.onSygnal(1);
            }
        }
    }

    if (mKasiMng.chkFear()) {
        setAction(&daNpcKasiHana_c::fear);
    }

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mKasiMng.sendInfo();
    playMotion();

    return TRUE;
}

void daNpcKasiHana_c::setAttnPos() {
    if (mLookMode == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    daNpcF_c::setMtx();
    lookat();

    cXyz sp1c(10.0f, 15.0f, 0.0f);

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&sp1c, &eyePos);
    sp1c.x = 0.0f;
    mDoMtx_stack_c::multVec(&sp1c, &sp1c);

    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp1c);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp1c);
    attention_info.position.set(mHeadPos.x, mHeadPos.y + mHIO->m.common.attention_offset, mHeadPos.z);

    cXyz sp28;

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(1));
    mDoMtx_stack_c::multVecZero(&sp28);
    sp28.y = current.pos.y;
    mCyl.SetC(sp28);
    #if DEBUG
    mCyl.SetH(mHIO->m.common.height);
    mCyl.SetR(mHIO->m.common.width);
    #endif
    dComIfG_Ccsp()->Set(&mCyl);
}

void daNpcKasiHana_c::setMotionAnm(int i_index, f32 i_morf) {
    J3DAnmTransformKey* i_anm = getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[i_index].arcIdx], l_bckGetParamList[i_index].fileIdx);
    int i_attr = l_bckGetParamList[i_index].attr;
    mAnmFlags &= 0xFFFFFFF6;

    if (i_anm != NULL && setMcaMorfAnm(i_anm, 1.0f, i_morf, i_attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }
}

void daNpcKasiHana_c::setMotion(int i_motion, f32 i_morf, int param_3) {
    s16 motion = i_motion;
    if ((param_3 != 0 || mMotion != motion) && i_motion >= 0 && i_motion < 0xC) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
        field_0x143e = 0;
    }
}

int daNpcKasiHana_c::drawDbgInfo() {
    return 0;
}

void daNpcKasiHana_c::reset() {
    initialize();
    mLookat.initialize();

    for (int i = 0; i < 1; i++) {
        mActorMngr[i].initialize();
    }

    mPath.initialize();
    mKasiMng.setHanaActor(this);

    if (mType == TYPE_WAIT) {
        mPath.setPathInfo(getRailNo(), fopAcM_GetRoomNo(this), 0);
        mPath.setIdx(6);
    } else if (mType == TYPE_CHEER) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
    }

    field_0x1441 = 0;
    field_0x1430 = 0;
    field_0x1434 = 0;
    mAction = NULL;
    mLookMode = -1;
    mMode = 0;
    current.pos.set(home.pos);
    old.pos.set(current.pos);
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);
    field_0x142c = fpcM_ERROR_PROCESS_ID_e;
    mOrderEvtNo = EVT_NONE;

    J3DAnmTexPattern* i_btp = getTexPtrnAnmP(l_arcNames[0], 8);
    mAnmFlags &= 0xFFFFF57F;

    if (setBtpAnm(i_btp, mAnm_p->getModel()->getModelData(), 1.0f, J3DFrameCtrl::EMode_LOOP)) {
        mAnmFlags |= ANM_FLAG_800 | ANM_PLAY_BTP | ANM_PAUSE_BTP;
    }

    mTalked = false;
    field_0x1445 = false;
    mEscape = false;
    setAction(&daNpcKasiHana_c::wait);
    mMotionMorfOverride = 0.0f;
}

void daNpcKasiHana_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_MICH_KYA_TALK, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {ANM_MICH_IYAN_WAIT, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2 = {ANM_MICH_OUEN_WAIT_A, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3 = {ANM_MICH_OUEN_WAIT_B, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4 = {ANM_W_WAIT_A, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5 = {ANM_W_TALK_B, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData dat6 = {ANM_W_TO_WOLF, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat6[1] = {&dat6};
    daNpcF_anmPlayData dat7 = {ANM_W_WALK_A, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8 = {ANM_W_LOOK_B, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};
    daNpcF_anmPlayData dat9 = {ANM_W_RUN_A, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat9[1] = {&dat9};
    daNpcF_anmPlayData dat10 = {ANM_W_SURPRISE, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat10[1] = {&dat10};
    daNpcF_anmPlayData dat11a = {ANM_W_WAIT_A_2, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat11b = {ANM_W_2LADYTALK_B, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat11c = {ANM_W_TALK_A, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat11d = {ANM_W_WAIT_A_2, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat11e = {ANM_W_TALK_B, mHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat11f = {ANM_W_2NORMALTALK_B, mHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat11[6] = {&dat11a, &dat11b, &dat11c, &dat11d, &dat11e, &dat11f};

    daNpcF_anmPlayData** ppDat[12] = {
        pDat0,
        pDat1,
        pDat2,
        pDat3,
        pDat4,
        pDat5,
        pDat6,
        pDat7,
        pDat8,
        pDat9,
        pDat10,
        pDat11,
    };

    if (mMotion >= 0 && mMotion < 0xC) {
        field_0x143e = mMotionPhase;

        if (mMotion == MOT_W_WAIT_A_LOOP) {
            playMotionAnmLoop(ppDat);
        } else {
            playMotionAnm(ppDat);
        }
    }
}

void daNpcKasiHana_c::playMotionAnmLoop(daNpcF_c::daNpcF_anmPlayData*** i_data) {
    daNpcF_anmPlayData* playData = NULL;

    if (i_data[mMotion] != NULL) {
        playData = i_data[mMotion][mMotionPhase];
    }

    if (playData != NULL) {
        if (mMotionPrevPhase == mMotionPhase) {
            if (playData->numLoops > 0 && playData->numLoops <= mMotionLoops) {
                mMotionPhase++;
                playData = i_data[mMotion][mMotionPhase];
            } else if (playData->numLoops == 0 && mMotionLoops >= 1) {
                mMotionPhase = 0;
                playData = i_data[mMotion][mMotionPhase];
            }
        }

        if (playData != NULL && mMotionPrevPhase != mMotionPhase) {
            setMotionAnm(playData->idx, 0.0f);
            f32 i_morf = playData->morf;

            if (mMotionPhase == 0) {
                if (mMotion == MOT_W_WAIT_A_LOOP) {
                    i_morf = mHIO->m.common.morf_frame;
                } else if (0.0f <= mMotionMorfOverride) {
                    i_morf = mMotionMorfOverride;
                }
            }

            mExpressionMorf = 0.0f;
            mAnm_p->setMorf(i_morf);
        }
    }

    mMotionPrevPhase = mMotionPhase;
}

BOOL daNpcKasiHana_c::setAction(actionFunc action) {
    mMode = -1;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = 0;
    mAction = action;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    return TRUE;
}

void daNpcKasiHana_c::setLookMode(int i_lookMode) {
    if (i_lookMode >= 0 && i_lookMode < 4 && i_lookMode != mLookMode) {
        mLookMode = i_lookMode;
    }
}

void daNpcKasiHana_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL i_snap = FALSE;
    f32 body_angleX_min = mHIO->m.common.body_angleX_min;
    f32 body_angleX_max = mHIO->m.common.body_angleX_max;
    f32 body_angleY_min = mHIO->m.common.body_angleY_min;
    f32 body_angleY_max = mHIO->m.common.body_angleY_max;
    f32 head_angleX_min = mHIO->m.common.head_angleX_min;
    f32 head_angleX_max = mHIO->m.common.head_angleX_max;
    f32 head_angleY_min = mHIO->m.common.head_angleY_min;
    f32 head_angleY_max = mHIO->m.common.head_angleY_max;
    s16 angle_delta = mCurAngle.y - mOldAngle.y;
    cXyz lookatPos[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookatAngle[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};
    cXyz spe8;

    switch (mLookMode) {
        case LOOK_NONE:
            break;

        case LOOK_RESET:
            i_snap = TRUE;
            break;

        case LOOK_PLAYER:
        case LOOK_PLAYER_TALK:
            player = daPy_getPlayerActorClass();

            if (mLookMode != LOOK_PLAYER_TALK) {
                break;
            }

            head_angleY_min = -80.0f;
            head_angleY_max = 80.0f;
            break;
    }

    if (player != NULL) {
        mLookPos = player->attention_info.position;

        if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK) {
            mLookPos.y -= 40.0f;
        }

        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }
    
    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max, 0.0f, 0.0f, 0.0f, 0.0f, 
                     head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max, mCurAngle.y, lookatPos);
    mLookat.calc(this, model->getBaseTRMtx(), lookatAngle, i_snap, angle_delta, FALSE);
}

BOOL daNpcKasiHana_c::step(s16 i_targetAngle, int param_2) {
    if (mTurnMode == 0) {
        mTurnTargetAngle = i_targetAngle;
        mTurnAmount = 0;

        if (mCurAngle.y == mTurnTargetAngle) {
            mTurnMode++;
        }

        current.angle.y = mCurAngle.y;
        shape_angle.y = current.angle.y;
        mTurnMode++;
    } else if (mTurnMode == 1) {
        if (daNpcF_c::turn(mTurnTargetAngle, 15.0f, 0)) {
            shape_angle.y = current.angle.y;
            mCurAngle.y = current.angle.y;
            mOldAngle.y = current.angle.y;
            mTurnMode++;
        } else {
            shape_angle.y = current.angle.y;
            mCurAngle.y = current.angle.y;
        }
    }

    return mTurnMode > 1;
}

BOOL daNpcKasiHana_c::chkFindPlayer() {
    if (!chkActorInSight(daPy_getPlayerActorClass(), mHIO->m.common.fov)) {
        mActorMngr[0].remove();
        return FALSE;
    }

    bool rv;
    if (mActorMngr[0].getActorP() == NULL) {
        rv = chkPlayerInSpeakArea(this);
    } else {
        rv = chkPlayerInTalkArea(this);
    }

    if (rv) {
        mActorMngr[0].entry(daPy_getPlayerActorClass());
    } else {
        mActorMngr[0].remove();
    }

    return rv;
}

int daNpcKasiHana_c::wait(int param_1) {
    switch (mMode) {
        case -1:
            break;

        case 0:
            if (mType == TYPE_WAIT && !field_0x1445) {
                setMotion(MOT_W_WAIT_A_LOOP, 12.0f, 0);
            } else {
                setMotion(MOT_W_WAIT_A, -1.0f, 0);
            }

            speedF = 0.0f;
            mMode = 1;
            break;

        case 1:
            BOOL bVar1 = FALSE;
            chkFindPlayer();

            if (mActorMngr[0].getActorP() != NULL) {
                setLookMode(LOOK_PLAYER);
                bVar1 = TRUE;
            } else if (!fopAcM_isSwitch(this, getSwitchBitNo())) {
                setLookMode(LOOK_NONE);
                if (home.angle.y != mCurAngle.y && step(home.angle.y, 1)) {
                    if (mType == TYPE_WAIT && !field_0x1445) {
                        setMotion(MOT_W_WAIT_A_LOOP, 12.0f, 0);
                    } else {
                        setMotion(MOT_W_WAIT_A, -1.0f, 0);
                    }

                    mTurnMode = 0;
                }
            }

            if (mType == TYPE_WAIT) {
                /* dSv_event_flag_c::F_290 - Castle Town - Star Game 1 cleared */
                if (daNpcF_chkEvtBit(290)) {
                    f32 actorDistance = fopAcM_searchActorDistanceXZ(this, daPy_getPlayerActorClass());

                    if (!fopAcM_isSwitch(this, getSwitchBitNo())) {
                        if (actorDistance < 350.0f) {
                            mOrderEvtNo = EVT_APPEAR;
                        }
                    } else if (!daPy_py_c::checkNowWolf()) {
                        if (field_0x1441 == 0) {
                            if (bVar1) {
                                field_0x1441 = 1;
                                mKasiMng.onSygnal(0x200);
                                setAction(&daNpcKasiHana_c::chace_st);
                                return 1;
                            }

                            f32 distFromCenter = mKasiMng.getDistFromCenter();
                            if (distFromCenter >= 0.0f && distFromCenter < mHIO->m.track_start_dist) {
                                field_0x1441 = 1;
                                mKasiMng.onSygnal(0x200);
                                setAction(&daNpcKasiHana_c::chace_st);
                                return 1;
                            }
                        } else if (pl_front_check()) {
                            if (actorDistance >= mHIO->m.track_stop_dist + 50.0f) {
                                setAction(&daNpcKasiHana_c::chace);
                                return 1;
                            }
                        } else {
                            setAction(&daNpcKasiHana_c::turn);
                            return 1;
                        }
                    }
                }
            } else if (mType == TYPE_CHEER && mKasiMng.isMiniGamePlaying()) {
                setAction(&daNpcKasiHana_c::cheer);
                return 1;
            }

            mKasiMng.setKyuPos(current.pos);
            mKasiMng.setMichPos(current.pos);

            if (dComIfGp_event_runCheck()) {
                if (eventInfo.checkCommandTalk()) {
                    if (!dComIfGp_event_chkTalkXY() || dComIfGp_evmng_ChkPresentEnd()) {
                        setAction(&daNpcKasiHana_c::talk);
                    }
                } else if (dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0) != -1) {
                    setAction(&daNpcKasiHana_c::demo);
                }
            } else {
                if (!dKy_darkworld_check()) {
                    if (daPy_py_c::checkNowWolf()) {
                        f32 fVar1 = pow(500.0, 2.0);
                        if (fopAcM_searchPlayerDistanceXZ2(this) < fVar1) {
                            setAction(&daNpcKasiHana_c::fear);
                            break;
                        }
                    }
                }

                if (mOrderEvtNo != EVT_NONE) {
                    eventInfo.setArchiveName(l_arcNames[0]);
                }

                orderEvent(field_0x1444, l_evtNames[mOrderEvtNo], 0xFFFF, 4, 0xFF, 2);
            }
            break;
    }

    return 1;
}

int daNpcKasiHana_c::chace_st(int param_1) {
    switch (mMode) {
        case -1:
            break;

        case 0:
            field_0x1445 = true;
            fopAcM_SetSpeedF(this, 0.0f);
            mMode = 1;
            break;

        case 1:
            if (_turn_to_link(0xC00)) {
                setAction(&daNpcKasiHana_c::chace);
            }
            break;
    }

    return 1;
}

int daNpcKasiHana_c::chace(int param_1) {
    switch (mMode) {
        case -1:
            break;

        case 0:
            field_0x1445 = true;
            setMotion(MOT_W_RUN_A, -1.0f, 0);
            setLookMode(LOOK_NONE);
            fopAcM_SetSpeed(this, 0.0f, 0.0f, 0.0f);
            fopAcM_SetSpeedF(this, mHIO->m.track_spd);
            mKasiMng.chgWeightLight();
            mMode = 1;
            break;

        case 1:
            cXyz chacePos = getChacePos();
            _turn_pos(chacePos, 0x800);

            if (pl_front_check()) {
                if (fopAcM_searchActorDistanceXZ(this, daPy_getPlayerActorClass()) < mHIO->m.track_stop_dist) {
                    setAction(&daNpcKasiHana_c::wait);
                }
            } else {
                setAction(&daNpcKasiHana_c::chace_st);
            }
            break;
    }

    return 1;
}

cXyz daNpcKasiHana_c::getChacePos() {
    cXyz rv(*fopAcM_GetPosition_p(daPy_getPlayerActorClass()));

    if (!mKasiMng.isPathInit()) {
        OS_REPORT("-------------------------path not initialize!!\n");
        return rv;
    }

    int plNearPoint = mKasiMng.getPlNearPoint();
    int idx = mPath.getIdx();

    if (idx == plNearPoint) {
        return rv;
    }

    if (idx < plNearPoint) {
        mPath.offReverse();
    } else if (idx > plNearPoint) {
        mPath.onReverse();
    }

    cXyz sp3c;
    mPath.getDstPos(current.pos, sp3c);
    
    return sp3c;
}

int daNpcKasiHana_c::turn(int param_1) {
    switch (mMode) {
        case -1:
            break;

        case 0:
            setMotion(MOT_W_WAIT_A, -1.0f, 0);
            speedF = 0.0f;
            mKasiMng.onSygnal(4);
            mMode = 1;
            break;

        case 1:
            if (_turn_to_link(0x1000)) {
                setAction(&daNpcKasiHana_c::wait);
            }
            break;

        default:
            JUT_ASSERT(2155, FALSE);
    }

    return 1;
}

int daNpcKasiHana_c::fear(int param_1) {
    switch (mMode) {
        case -1:
            break;

        case 0: {
            mKasiMng.onSygnal(0x80000);
            dComIfGs_onSaveDunSwitch(60);

            daTagEscape_c* tagEscape_p = srchWolfTag();
            if (tagEscape_p != NULL) {
                mPath.initialize();
                mPath.setPathInfo(tagEscape_p->getPathID(), fopAcM_GetRoomNo(this), 0);
                mPath.setIdx(getWolfPathNearIdx());
                mMode = 1;
            }
            break;
        }

        case 1:
            if (_turn_to_link(0x1000)) {
                setMotion(MOT_W_SURPRISE, -1.0f, 0);
                mMode = 3;
            }
            break;

        case 3:
            if (mAnm_p->isStop()) {
                setMotion(MOT_W_RUN_A, -1.0f, 0);
                fopAcM_SetSpeedF(this, 15.0f);
                mSound.startCreatureVoice(Z2SE_HANA_V_FEAR, -1);
                mMode = 4;
            }
            break;

        case 4: {
            cXyz sp28;
            mAnm_p->setPlaySpeed(1.5f);
            
            if (mPath.getDstPos(current.pos, sp28)) {
                mEscape = true;
                mMode = 5;
            } else {
                _turn_pos(sp28, 800);
            }
            break;
        }

        case 5:
            if (mKasiMng.chkEscape()) {
                mKasiMng.deleteAllMember();
            }
            break;
    }

    return 1;
}

daTagEscape_c* daNpcKasiHana_c::mTargetTag;

f32 daNpcKasiHana_c::mTargetTagDist;

s16 daNpcKasiHana_c::mWolfAngle;

daTagEscape_c* daNpcKasiHana_c::srchWolfTag() {
    mTargetTag = NULL;
    mWolfAngle = fopAcM_searchPlayerAngleY(this);
    fpcM_Search(_srch_escape_tag, this);
    return mTargetTag;
}

void* daNpcKasiHana_c::_srch_escape_tag(void* i_actor, void* i_data) {
    if (!fopAcM_IsActor(i_actor)) {
        return NULL;
    }
    
    if (fopAcM_GetName(i_actor) != PROC_Tag_Escape) {
        return NULL;
    }

    int iVar1 = cLib_distanceAngleS(mWolfAngle, fopAcM_searchActorAngleY((fopAc_ac_c*)i_data, (fopAc_ac_c*)i_actor));

    if (mTargetTag == NULL) {
        mTargetTagDist = fopAcM_searchActorDistanceXZ2((fopAc_ac_c*)i_data, (fopAc_ac_c*)i_actor);
        mTargetTag = (daTagEscape_c*)i_actor;
        return NULL;
    }
    
    if (iVar1 > 0x4000) {
        f32 fVar1 = fopAcM_searchActorDistanceXZ2((fopAc_ac_c*)i_data, (fopAc_ac_c*)i_actor);
        if (fVar1 < mTargetTagDist) {
            mTargetTagDist = fVar1;
            mTargetTag = (daTagEscape_c*)i_actor;
        }
    }

    return NULL;
}

int daNpcKasiHana_c::getWolfPathNearIdx() {
    int iVar1 = 0;
    f32 fVar1 = 0.0f;
    int numPnts = mPath.getNumPnts();
    cXyz sp44, sp50;

    int rv = 0;
    for (; rv < numPnts; rv++) {
        sp44 = mPath.getPntPos(rv);
        sp50 = sp44 - current.pos;
        f32 fVar2 = sp50.abs2XZ();

        if (rv == 0) {
            fVar1 = fVar2;
            iVar1 = 0;
        } else if (fVar2 < fVar1) {
            fVar1 = fVar2;
            iVar1 = rv;
        }
    }

    rv = iVar1 + 1;

    if (iVar1 == numPnts) {
        rv = iVar1;
    }

    return rv;
}

int daNpcKasiHana_c::talk(int param_1) {
    bool rv = false;

    switch (mMode) {
        case 0:
            mTalked = true;
            setAction(&daNpcKasiHana_c::wait);
            rv = true;
            break;

        case -1:
            if (!field_0x9ec) {
                dComIfGp_event_reset();
            }
            break;

        case 1:
            if (talkProc(NULL, TRUE, NULL)) {
                mActorMngr[0].entry(daPy_getPlayerActorClass());

                if (mType == TYPE_WAIT) {
                    setAction(&daNpcKasiHana_c::wait);
                }

                rv = true;
            } else {
                int iVar1, i_motion;
                if (ctrlMsgAnm(iVar1, i_motion, this, FALSE)) {
                    setMotion(i_motion, -1.0f, 0);
                }
            }
            break;

        case 2:
            break;
    }

    return rv;
}

int daNpcKasiHana_c::demo(int param_1) {
    switch (mMode) {
        case -1:
            break;

        case 0:
            setMotion(MOT_W_WAIT_A, -1.0f, 0);
            mMode = 1;
            break;

        case 1:
            if (dComIfGp_event_runCheck() != FALSE && !eventInfo.checkCommandTalk()) {
                dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                s32 staffId = dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0);
                if (staffId != -1) {
                    mStaffID = staffId;

                    JUT_ASSERT(2459, NULL != mEvtSeqList[mOrderEvtNo]);

                    if ((this->*mEvtSeqList[mOrderEvtNo])(staffId)) {
                        dComIfGp_getEventManager().cutEnd(staffId);
                    }
                }

                if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager.endCheck(mEventIdx)) {
                    switch (mOrderEvtNo) {
                        case EVT_APPEAR:
                            dComIfGp_event_reset();
                            mOrderEvtNo = EVT_NONE;
                            mEventIdx = -1;
                            fopAcM_onSwitch(this, getSwitchBitNo());
                            mKasiMng.onSygnal(1);
                            setAction(&daNpcKasiHana_c::wait);
                            break;

                        case EVT_HEART:
                            setAction(&daNpcKasiHana_c::escape);
                            dComIfGp_event_reset();
                            mOrderEvtNo = EVT_NONE;
                            mEventIdx = -1;
                            break;

                        case EVT_TALK:
                            mKasiMng.allDemoNotMove();
                            dComIfGp_event_reset();
                            mOrderEvtNo = EVT_NONE;
                            mEventIdx = -1;
                            setAction(&daNpcKasiHana_c::wait);
                            mKasiMng.onSygnal(1);
                            break;

                        case EVT_CHEER:
                        case EVT_CHEER2:
                            dTimer_show();
                            // fallthrough
                        default:
                            dComIfGp_event_reset();
                            mOrderEvtNo = EVT_NONE;
                            mEventIdx = -1;

                            if (mType == TYPE_WAIT) {
                                setAction(&daNpcKasiHana_c::wait);
                            } else if (mType == TYPE_CHEER) {
                                OS_REPORT("-----------demo return!!\n");

                                dComIfG_TimerReStart(6);
                                setAction(&daNpcKasiHana_c::cheer);
                            }
                            break;
                    }
                }
            }
            break;
    }

    return 1;
}

int daNpcKasiHana_c::escape(int param_1) {
    switch (mMode) {
        case -1:
            break;

        case 0:
            setMotion(MOT_W_RUN_A, -1.0f, 0);
            fopAcM_SetSpeed(this, 0.0f, 0.0f, 0.0f);
            fopAcM_SetSpeedF(this, mHIO->m.escape_spd);
            setEscapePathDir();
            mKasiMng.onSygnal(0x20);
            mSound.startCreatureVoice(Z2SE_HANA_V_FEAR, -1);
            mMode = 1;
            break;

        case 1:
            cXyz sp28;
            if (mPath.getDstPos(current.pos, sp28)) {
                mKasiMng.deleteAllMember();
            } else {
                _turn_pos(sp28, 0x600);
                mKasiMng.calcEscapeForm();
                mAnm_p->setPlaySpeed(1.75f);
            }
            break;
    }

    return 1;
}

int daNpcKasiHana_c::cheer(int param_1) {
    switch (mMode) {
        case -1:
            break;

        case 0:
            /* dSv_event_flag_c::F_290 - Castle Town - Star Game 1 cleared */
            if (daNpcF_chkEvtBit(290)) {
                setMotion(MOT_MICH_OUEN_WAIT_B, -1.0f, 0);
            } else {
                setMotion(MOT_MICH_OUEN_WAIT_A, -1.0f, 0);
            }

            setLookMode(LOOK_PLAYER);
            fopAcM_SetSpeed(this, 0.0f, 0.0f, 0.0f);
            fopAcM_SetSpeedF(this, 0.0f);
            mTalked = false;
            field_0x1440 = 0;
            mMode = 1;
            break;

        case 1:
            if (!mKasiMng.isMiniGamePlaying()) {
                mKasiMng.onSygnal(1);
                setAction(&daNpcKasiHana_c::wait);
                return 1;
            }

            if (field_0x1440 == 0) {
                mKasiMng.onSygnal(0x100);
                field_0x1440 = 1;
            }

#if VERSION != VERSION_SHIELD_DEBUG
            // TODO: gameInfo fake match to force reuse of pointer
            dComIfG_play_c* play = &g_dComIfG_gameInfo.play;
            if (play->getEvent().runCheck())
#else
            if (dComIfGp_event_runCheck())
#endif
            {
                if (eventInfo.checkCommandTalk()) {
                    if (!dComIfGp_event_chkTalkXY() || dComIfGp_evmng_ChkPresentEnd()) {
                        mTalked = true;
#if VERSION != VERSION_SHIELD_DEBUG
                        play->getEvent().reset();
#else
                        dComIfGp_event_reset();
#endif
                    }
                }
            } else {
                if (mOrderEvtNo != EVT_NONE) {
                    eventInfo.setArchiveName(l_arcNames[0]);
                }

                orderEvent(field_0x1444, l_evtNames[mOrderEvtNo], 0xFFFF, 4, 0xFF, 2);
            }
            break;
    }

    return 1;
}

BOOL daNpcKasiHana_c::_turn_to_link(s16 param_1) {
    cXyz sp20(*fopAcM_GetPosition_p(daPy_getPlayerActorClass()));
    return _turn_pos(sp20, param_1);
}

BOOL daNpcKasiHana_c::_turn_pos(cXyz const& param_1, s16 param_2) {
    BOOL rv = cLib_chaseAngleS(&mCurAngle.y, cLib_targetAngleY(&current.pos, &param_1), param_2);
    current.angle.y = mCurAngle.y;
    shape_angle.y = mCurAngle.y;
    return rv;
}

BOOL daNpcKasiHana_c::actor_front_check(fopAc_ac_c* actor_p) {
    if (actor_p == NULL) {
        return TRUE;
    }

    cXyz sp24(*fopAcM_GetPosition_p(actor_p));
    cXyz sp30(sp24 - current.pos);
    cXyz sp3c(0.0f, 0.0f, 1.0f);
    cXyz sp48;
    _getOffset(sp3c, sp48);

    return (sp30.x * sp48.x + sp30.z * sp48.z) >= 0.0f;
}

void daNpcKasiHana_c::_getOffset(cXyz const& param_1, cXyz& param_2) {
    param_2 = param_1;
    csXyz angle(*fopAcM_GetShapeAngle_p(this));
    mDoMtx_stack_c::ZXYrotS(angle);
    mDoMtx_stack_c::multVec(&param_2, &param_2);
}

BOOL daNpcKasiHana_c::_Evt_Kasi_Appear(int i_index) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int i_cutIndex = -1;

    int* cutId = dComIfGp_evmng_getMyIntegerP(i_index, "cutId");
    if (cutId != NULL) {
        i_cutIndex = *cutId;
    } else {
        return TRUE;
    }
    
    if (eventManager.getIsAddvance(i_index)) {
        _Evt_Kasi_Appear_CutInit(i_cutIndex);
    }

    return _Evt_Kasi_Appear_CutMain(i_cutIndex);
    
}

BOOL daNpcKasiHana_c::_Evt_Kasi_Appear_CutInit(int const& i_cutIndex) {
    switch (i_cutIndex) {
        case 10:
            mKasiMng.onSygnal(4);
            setMotion(MOT_W_WAIT_A, -1.0f, 0);
            field_0x1430 = 20;
            break;
        
        case 20:
            mKasiMng.onSygnal(0x400);
            setMotion(MOT_MICH_KYA_TALK, -1.0f, 0);
            initTalk(mMessageNo, NULL);
            break;

        case 21:
            mKasiMng.onSygnal(0x1000);
            field_0x1430 = 24;
            break;
    }

    return TRUE;
}

BOOL daNpcKasiHana_c::_Evt_Kasi_Appear_CutMain(int const& i_cutIndex) {
    BOOL rv = FALSE;

    switch (i_cutIndex) {
        case 10:
            if (_turn_to_link(0x1000)) {
                setLookMode(LOOK_PLAYER);
            }

            if (--field_0x1430 <= 0) {
                rv = TRUE;
            }
            break;
        
        case 20:
            if (talkProc(NULL, TRUE, NULL)) {
                rv = TRUE;
            }
            break;
        
        case 21:
            if (mMotion == MOT_MICH_KYA_TALK && (mAnm_p->getFrame() == 19.0f || mAnm_p->getFrame() == 1.0f)) {
                setMotion(MOT_W_WAIT_A, -1.0f, 0);
            }

            field_0x1430--;
            if (field_0x1430 == 0) {
                rv = TRUE;
            }
            break;

        default:
            rv = TRUE;
    }

    return rv;
}

BOOL daNpcKasiHana_c::_Evt_Kasi_Talk(int i_index) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int i_cutIndex = -1;

    int* cutID = dComIfGp_evmng_getMyIntegerP(i_index, "cutId");
    if (cutID != NULL) {
        i_cutIndex = *cutID;
    } else {
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_index)) {
        _Evt_Kasi_Talk_CutInit(i_cutIndex);
    }

    int iVar2 = -1;
    int* mesNo;

    /* dSv_event_flag_c::F_289 - Castle Town - Heard conversation about entering Star Game 1 */
    if (daNpcF_chkEvtBit(289)) {
        mesNo = dComIfGp_evmng_getMyIntegerP(i_index, "mesNo2");    
    } else {
        mesNo = dComIfGp_evmng_getMyIntegerP(i_index, "mesNo1");
    }

    if (mesNo != NULL) {
        iVar2 = *mesNo;
    }

    return _Evt_Kasi_Talk_CutMain(i_cutIndex, iVar2);
}

BOOL daNpcKasiHana_c::_Evt_Kasi_Talk_CutInit(int const& i_cutIndex) {
    fopAc_ac_c* actors[3] = {mKasiMng.getHanaActor(), mKasiMng.getKyuActor(), mKasiMng.getMichActor()};

    switch (i_cutIndex) {
        case 10:
            setLookMode(LOOK_NONE);
            setMotion(MOT_MICH_KYA_TALK, -1.0f, 0);
            initTalk(mMessageNo, actors);
            break;
        
        case 20:
            mKasiMng.onSygnal(0x20000);
            break;

        case 30:
            mKasiMng.onSygnal(0x40000);
            break;
            
        case 31:
            mKasiMng.onSygnal(0x1000);
            break;
    }

    return TRUE;
}

BOOL daNpcKasiHana_c::_Evt_Kasi_Talk_CutMain(int const& i_cutIndex, int param_2) {
    fopAc_ac_c* actors[3] = {mKasiMng.getHanaActor(), mKasiMng.getKyuActor(), mKasiMng.getMichActor()};
    BOOL rv = FALSE;
    int iVar1[2] = {-1, -1};

    switch (i_cutIndex) {
        case 10:
            iVar1[0] = param_2;

            if (talkProc(iVar1, FALSE, actors)) {
                rv = TRUE;
            }
            break;
        
        case 20:
            if (mMotion == MOT_MICH_KYA_TALK && (mAnm_p->getFrame() == 19.0f || mAnm_p->getFrame() == 1.0f)) {
                setMotion(MOT_W_WAIT_A, -1.0f, 0);
            }

            iVar1[0] = param_2;

            if (talkProc(iVar1, FALSE, actors)) {
                rv = TRUE;
            }
            break;

        case 30:
            if (talkProc(NULL, FALSE, actors)) {
                rv = TRUE;
            }
            break;
            
        case 31:
            rv = TRUE;
            break;

        default:
            rv = TRUE;
            break;
    }

    return rv;
}

BOOL daNpcKasiHana_c::_Evt_Kasi(int i_index) {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int i_cutIndex = -1;

    int* cutId = dComIfGp_evmng_getMyIntegerP(i_index, "cutId");
    if (cutId != NULL) {
        i_cutIndex = *cutId;
    } else {
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_index) != 0) {
        _Evt_Kasi_CutInit(i_cutIndex);
    }

    return _Evt_Kasi_CutMain(i_cutIndex);
}

BOOL daNpcKasiHana_c::_Evt_Kasi_CutInit(int const& i_cutIndex) {
    switch (i_cutIndex) {
        case 10:
            mKasiMng.onSygnal(4);
            break;

        case 20:
            setMotion(MOT_MICH_KYA_TALK, -1.0f, 0);
            mKasiMng.onSygnal(0x400);
            initTalk(mMessageNo, NULL);
            break;
        
        case 21:
            mKasiMng.onSygnal(0x1000);
            field_0x1430 = 24;
            break;

        case 30:
            setLookMode(LOOK_NONE);
            mKasiMng.onSygnal(0x40);
            mKasiMng.calcCenterPos();
            break;

        case 40:
            field_0x1430 = 86;
            mKasiMng.onSygnal(0x800);
            setMotion(MOT_MICH_IYAN_WAIT, -1.0f, 0);
            break;
    }

    return TRUE;
}

BOOL daNpcKasiHana_c::_Evt_Kasi_CutMain(int const& i_cutIndex) {
    BOOL rv = FALSE;

    switch (i_cutIndex) {
        case 10:
            if (_turn_to_link(0xA00)) {
                rv = TRUE;
            }
            break;

        case 20:
            if (talkProc(NULL, TRUE, NULL)) {
                rv = TRUE;
            }
            break;

        case 21:
            if (mMotion == MOT_MICH_KYA_TALK && (mAnm_p->getFrame() == 19.0f || mAnm_p->getFrame() == 1.0f)) {
                setMotion(MOT_W_WAIT_A, -1.0f, 0);
            }

            field_0x1430--;
            if (field_0x1430 == 0) {
                rv = TRUE;
            }
            break;

        case 30: {
            cXyz sp38(mKasiMng.getCenterPos());
            
            if (_turn_pos(sp38, 0xA00)) {
                rv = TRUE;
            }
            break;
        }

        case 40:
            field_0x1430--;
            if (field_0x1430 <= 0) {
                rv = TRUE;
            } else if (field_0x1430 == 66) {
                mKasiMng.createHearts();
            }
            break;

        default:
            rv = TRUE;
    }

    return rv;
}

BOOL daNpcKasiHana_c::_Evt_Kasi_Cheer(int i_index) {
    OS_REPORT("-------------------daNpcKasiHana_c::_Evt_Kasi_Cheer\n");

    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int i_cutIndex = -1;
    
    int* cutId = dComIfGp_evmng_getMyIntegerP(i_index, "cutId");
    if (cutId != NULL) {
        i_cutIndex = *cutId;
    } else {
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_index) != 0) {
        _Evt_Kasi_Cheer_CutInit(i_cutIndex);
    }

    int iVar2 = -1;
    int* mesNo;

    /* dSv_event_flag_c::F_290 - Castle Town - Star Game 1 cleared */
    if (daNpcF_chkEvtBit(290)) {
        mesNo = dComIfGp_evmng_getMyIntegerP(i_index, "mesNo2");
    } else {
        mesNo = dComIfGp_evmng_getMyIntegerP(i_index, "mesNo1");
    }

    if (mesNo != NULL) {
        iVar2 = *mesNo;
    }

    return _Evt_Kasi_Cheer_CutMain(i_cutIndex, iVar2);
}

BOOL daNpcKasiHana_c::_Evt_Kasi_Cheer_CutInit(int const& i_cutIndex) {
    fopAc_ac_c* actors[3] = {mKasiMng.getHanaActor(), mKasiMng.getKyuActor(), mKasiMng.getMichActor()};

    switch (i_cutIndex) {
        case 10:
            setMotion(MOT_MICH_IYAN_WAIT, -1.0f, 0);
            setLookMode(LOOK_NONE);
            field_0x1430 = 86;
            initTalk(mMessageNo, actors);
            break;

        case 20:
            mKasiMng.onSygnal(0x8000);
            break;

        case 30:
            mKasiMng.onSygnal(0x10000);
            break;
    }

    return TRUE;
}

BOOL daNpcKasiHana_c::_Evt_Kasi_Cheer_CutMain(int const& i_cutIndex, int param_2) {
    fopAc_ac_c* actors[3] = {mKasiMng.getHanaActor(), mKasiMng.getKyuActor(), mKasiMng.getMichActor()};
    int iVar1[2] = {-1, -1};
    BOOL rv = FALSE;

    switch (i_cutIndex) {
        case 10:
            if (field_0x1430 > 0) {
                field_0x1430--;
                if (field_0x1430 == 0) {
                    setMotion(MOT_W_WAIT_A, -1.0f, 0);
                }
            }

            iVar1[0] = param_2;

            if (talkProc(iVar1, FALSE, actors)) {
                rv = TRUE;
            }
            break;
        
        case 20:
            if (field_0x1430 > 0) {
                field_0x1430--;
                if (field_0x1430 == 0) {
                    setMotion(MOT_W_WAIT_A, -1.0f, 0);
                }
            }

            iVar1[0] = param_2;

            if (talkProc(iVar1, FALSE, actors)) {
                rv = TRUE;
            }
            break;

        case 30:
            if (field_0x1430 > 0) {
                field_0x1430--;
                if (field_0x1430 == 0) {
                    setMotion(MOT_W_WAIT_A, -1.0f, 0);
                }
            }

            if (talkProc(NULL, FALSE, actors)) {
                rv = TRUE;
            }
            break;

        default:
            rv = TRUE;
    }

    return rv;
}

BOOL daNpcKasiHana_c::_Evt_Kasi_Cheer2(int i_index) {
    OS_REPORT("-------------------daNpcKasiHana_c::_Evt_Kasi_Cheer2\n");

    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    int* cutId = NULL;
    int i_cutIndex = -1;

    cutId = dComIfGp_evmng_getMyIntegerP(i_index, "cutId");
    if (cutId != NULL) {
        i_cutIndex = *cutId;
    } else {
        return TRUE;
    }

    if (eventManager.getIsAddvance(i_index)) {
        _Evt_Kasi_Cheer2_CutInit(i_cutIndex);
    }

    int* mesNo = NULL;
    int iVar2 = -1;
    mesNo = dComIfGp_evmng_getMyIntegerP(i_index, "mesNo1");
    if (mesNo != NULL) {
        iVar2 = *mesNo;
    }

    return _Evt_Kasi_Cheer2_CutMain(i_cutIndex, iVar2);
}

BOOL daNpcKasiHana_c::_Evt_Kasi_Cheer2_CutInit(int const& i_cutIndex) {
    fopAc_ac_c* actors[3] = {mKasiMng.getHanaActor(), mKasiMng.getKyuActor(), mKasiMng.getMichActor()};

    switch (i_cutIndex) {
        case 10:
            setMotion(MOT_MICH_IYAN_WAIT, -1.0f, 0);
            setLookMode(LOOK_NONE);
            field_0x1430 = 86;
            initTalk(mMessageNo, actors);
            break;
        
        case 20:
            mKasiMng.onSygnal(0x2000);
            break;

        case 30:
            mKasiMng.onSygnal(0x4000);
            break;
    }

    return TRUE;
}

BOOL daNpcKasiHana_c::_Evt_Kasi_Cheer2_CutMain(int const& i_cutIndex, int param_2) {
    fopAc_ac_c* actors[3] = {mKasiMng.getHanaActor(), mKasiMng.getKyuActor(), mKasiMng.getMichActor()};
    int iVar1[2] = {-1, -1};
    BOOL rv = FALSE;

    switch (i_cutIndex) {
        case 10:
            if (field_0x1430 > 0) {
                field_0x1430--;
                if (field_0x1430 == 0) {
                    setMotion(MOT_W_WAIT_A, -1.0f, 0);
                }
            }

            iVar1[0] = param_2;

            if (talkProc(iVar1, FALSE, actors)) {
                rv = TRUE;
            }
            break;
        
        case 20:
            if (field_0x1430 > 0) {
                field_0x1430--;
                if (field_0x1430 == 0) {
                    setMotion(MOT_W_WAIT_A, -1.0f, 0);
                }
            }

            iVar1[0] = param_2;

            if (talkProc(iVar1, FALSE, actors)) {
                rv = TRUE;
            }
            break;

        case 30:
            if (field_0x1430 > 0) {
                field_0x1430--;
                if (field_0x1430 == 0) {
                    setMotion(MOT_W_WAIT_A, -1.0f, 0);
                }
            }

            if (talkProc(NULL, FALSE, actors)) {
                rv = TRUE;
            }
            break;

        default:
            rv = TRUE;
    }

    return rv;
}

static int daNpcKasiHana_Create(void* a_this) {
    return static_cast<daNpcKasiHana_c*>(a_this)->Create();
}

static int daNpcKasiHana_Delete(void* a_this) {
    return static_cast<daNpcKasiHana_c*>(a_this)->Delete();
}

static int daNpcKasiHana_Execute(void* a_this) {
    return static_cast<daNpcKasiHana_c*>(a_this)->Execute();
}

static int daNpcKasiHana_Draw(void* a_this) {
    return static_cast<daNpcKasiHana_c*>(a_this)->Draw();
}

static int daNpcKasiHana_IsDelete(void* a_this) {
    return 1;
}

AUDIO_INSTANCES;

static actor_method_class daNpcKasiHana_MethodTable = {
    (process_method_func)daNpcKasiHana_Create,
    (process_method_func)daNpcKasiHana_Delete,
    (process_method_func)daNpcKasiHana_Execute,
    (process_method_func)daNpcKasiHana_IsDelete,
    (process_method_func)daNpcKasiHana_Draw,
};

extern actor_process_profile_definition g_profile_NPC_KASIHANA = {
  fpcLy_CURRENT_e,            // mLayerID
  7,                          // mListID
  fpcPi_CURRENT_e,            // mListPrio
  PROC_NPC_KASIHANA,          // mProcName
  &g_fpcLf_Method.base,      // sub_method
  sizeof(daNpcKasiHana_c),    // mSize
  0,                          // mSizeOther
  0,                          // mParameters
  &g_fopAc_Method.base,       // sub_method
  420,                        // mPriority
  &daNpcKasiHana_MethodTable, // sub_method
  0x00040108,                 // mStatus
  fopAc_NPC_e,                // mActorType
  fopAc_CULLBOX_CUSTOM_e,     // cullType
};
