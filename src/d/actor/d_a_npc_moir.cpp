/**
 * @file d_a_npc_moir.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_moir.h"
#include "d/actor/d_a_ni.h"
#include "d/actor/d_a_npc_tk.h"

enum MoiR_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_MOIR_F_HOLD_S = 0x6,
    /* 0x07 */ BCK_MOIR_F_TALK_A,
    /* 0x08 */ BCK_MOIR_FH_HOLD_S,
    /* 0x09 */ BCK_MOIR_FH_SERIOUS,
    /* 0x0A */ BCK_MOIR_STEP,
    /* 0x0B */ BCK_MOIR_TALK_A,
    /* 0x0C */ BCK_MOIR_WAIT_A,

    /* BMDR */
    /* 0x0F */ BMDR_MOIR = 0xF,
    /* 0x10 */ BMDR_HEADGEAR,

    /* BTK */
    /* 0x13 */ BTK_MOIR = 0x13,

    /* BTP */
    /* 0x16 */ BTP_MOIR = 0x16,
    /* 0x17 */ BTP_MOIR_FH_SERIOUS,
};

enum MoiR1_RES_File_ID {
    /* BCK */
    /* 0x3 */ BCK_MOIR_SIT = 0x3,
    /* 0x4 */ BCK_MOIR_SIT_TALK,
    /* 0x5 */ BCK_MOIR_TAKE_MET_SIT,
    /* 0x6 */ BCK_MOIR_TAKEOFF_SIT,
    /* 0x7 */ BCK_MOIR_TAKING_OFF_SIT,
    /* 0x8 */ BCK_MOIR_TAKING_OFF_SIT_TALK,
};

enum MoiR2_RES_File_ID {
    /* BCK */ 
    /* 0x04 */ BCK_MOIR_DRIVEAWAY = 0x4,
    /* 0x05 */ BCK_MOIR_F_DRIVEAWAY,
    /* 0x06 */ BCK_MOIR_F_FUE,
    /* 0x07 */ BCK_MOIR_F_SURPRISED,
    /* 0x08 */ BCK_MOIR_FH_DRIVEAWAY,
    /* 0x09 */ BCK_MOIR_FUE,
    /* 0x0A */ BCK_MOIR_LOOK_AROUND,
    /* 0x0B */ BCK_MOIR_SQUAREUP,
    /* 0x0C */ BCK_MOIR_SQUAREUP_STEP,
    /* 0x0D */ BCK_MOIR_SURPRISED,
    /* 0x0E */ BCK_MOIR_TAKAJO_NI,
    /* 0x0F */ BCK_MOIR_TAKAJO_WAIT_NI,
    /* 0x10 */ BCK_MOIR_TAKE_MET,
    /* 0x11 */ BCK_MOIR_TAKEOFF,
    /* 0x12 */ BCK_MOIR_TAKING_OFF,
    /* 0x13 */ BCK_MOIR_TAKING_OFF_TALK,
    /* 0x14 */ BCK_MOIR_WAIT_B,
    /* 0x15 */ BCK_MOIR_WAIT_C,

    /* EVT */
    /* 0x18 */ EVT_EVENT_LIST = 0x18,
};

enum MoiR3_RES_File_ID {
    /* BCK */
    /* 0x3 */ BCK_MOIR_F_TAKAJO_HI = 0x3,
    /* 0x4 */ BCK_MOIR_FH_TAKAJO_HI,
    /* 0x5 */ BCK_MOIR_RUN,
    /* 0x6 */ BCK_MOIR_TAKAJO,
    /* 0x7 */ BCK_MOIR_TAKAJO_HI,
    /* 0x8 */ BCK_MOIR_TAKAJO_HI_WAIT,
    /* 0x9 */ BCK_MOIR_TAKAJO_WAIT,
};

enum RES_Name {
    /* 0x0 */ MoiR,
    /* 0x1 */ MoiR1,
    /* 0x2 */ MoiR2,
    /* 0x3 */ MoiR3,
};

enum Material {
    /* 0x0 */ MAT_SC_EYE,
    /* 0x1 */ MAT_MOIR_EYEBALL_M,
    /* 0x2 */ MAT_MOIR_FACE_M,
    /* 0x3 */ MAT_MOIR_HEADGEAR_M,
    /* 0x4 */ MAT_MOIR_M,
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_BACKBONE1,
    /* 0x02 */ JNT_BACKBONE2,
    /* 0x03 */ JNT_NECK,
    /* 0x04 */ JNT_HEAD,
    /* 0x05 */ JNT_CHIN,
    /* 0x06 */ JNT_MAYU_L,
    /* 0x07 */ JNT_MAYU_R,
    /* 0x08 */ JNT_MOUTH,
    /* 0x09 */ JNT_SHOULDERL,
    /* 0x0A */ JNT_ARML1,
    /* 0x0B */ JNT_ARML2,
    /* 0x0C */ JNT_HANDL,
    /* 0x0D */ JNT_FINGERL,
    /* 0x0E */ JNT_SHOULDERR,
    /* 0x0F */ JNT_ARMR1,
    /* 0x10 */ JNT_ARMR2,
    /* 0x11 */ JNT_HANDR,
    /* 0x12 */ JNT_FINGERR,
    /* 0x13 */ JNT_WAIST,
    /* 0x14 */ JNT_LEGL1,
    /* 0x15 */ JNT_LEGL2,
    /* 0x16 */ JNT_FOOTL,
    /* 0x17 */ JNT_LEGR1,
    /* 0x18 */ JNT_LEGR2,
    /* 0x19 */ JNT_FOOTR
};

static daNpc_GetParam2 l_bckGetParamList[39] = {
    {-1, J3DFrameCtrl::EMode_NONE, MoiR},
    {BCK_MOIR_F_TALK_A, J3DFrameCtrl::EMode_NONE, MoiR},
    {BCK_MOIR_F_HOLD_S, J3DFrameCtrl::EMode_NONE, MoiR},
    {5, J3DFrameCtrl::EMode_NONE, MoiR},
    {BCK_MOIR_F_SURPRISED, J3DFrameCtrl::EMode_NONE, MoiR2},
    {BCK_MOIR_F_HOLD_S, J3DFrameCtrl::EMode_NONE, MoiR},
    {BCK_MOIR_F_TAKAJO_HI, J3DFrameCtrl::EMode_NONE, MoiR3},
    {BCK_MOIR_FH_HOLD_S, J3DFrameCtrl::EMode_LOOP, MoiR},
    {BCK_MOIR_FH_SERIOUS, J3DFrameCtrl::EMode_LOOP, MoiR},
    {BCK_MOIR_FH_HOLD_S, J3DFrameCtrl::EMode_LOOP, MoiR},
    {BCK_MOIR_FH_TAKAJO_HI, J3DFrameCtrl::EMode_NONE, MoiR3},
    {BCK_MOIR_WAIT_A, J3DFrameCtrl::EMode_LOOP, MoiR},
    {BCK_MOIR_WAIT_B, J3DFrameCtrl::EMode_LOOP, MoiR2},
    {BCK_MOIR_WAIT_C, J3DFrameCtrl::EMode_LOOP, MoiR2},
    {BCK_MOIR_TALK_A, J3DFrameCtrl::EMode_NONE, MoiR},
    {BCK_MOIR_STEP, J3DFrameCtrl::EMode_NONE, MoiR},
    {BCK_MOIR_LOOK_AROUND, J3DFrameCtrl::EMode_LOOP, MoiR2},
    {BCK_MOIR_FUE, J3DFrameCtrl::EMode_NONE, MoiR2},
    {BCK_MOIR_TAKAJO_NI, J3DFrameCtrl::EMode_NONE, MoiR2},
    {BCK_MOIR_TAKAJO_WAIT_NI, J3DFrameCtrl::EMode_LOOP, MoiR2},
    {BCK_MOIR_SURPRISED, J3DFrameCtrl::EMode_NONE, MoiR2},
    {BCK_MOIR_SQUAREUP, J3DFrameCtrl::EMode_LOOP, MoiR2},
    {BCK_MOIR_SQUAREUP_STEP, J3DFrameCtrl::EMode_LOOP, MoiR2},
    {BCK_MOIR_DRIVEAWAY, J3DFrameCtrl::EMode_NONE, MoiR2},
    {BCK_MOIR_TAKEOFF, J3DFrameCtrl::EMode_NONE, MoiR2},
    {BCK_MOIR_TAKING_OFF, J3DFrameCtrl::EMode_LOOP, MoiR2},
    {BCK_MOIR_TAKING_OFF_TALK, J3DFrameCtrl::EMode_NONE, MoiR2},
    {BCK_MOIR_TAKE_MET, J3DFrameCtrl::EMode_NONE, MoiR2},
    {BCK_MOIR_SIT, J3DFrameCtrl::EMode_LOOP, MoiR1},
    {BCK_MOIR_SIT_TALK, J3DFrameCtrl::EMode_NONE, MoiR1},
    {BCK_MOIR_TAKEOFF_SIT, J3DFrameCtrl::EMode_NONE, MoiR1},
    {BCK_MOIR_TAKING_OFF_SIT, J3DFrameCtrl::EMode_LOOP, MoiR1},
    {BCK_MOIR_TAKING_OFF_SIT_TALK, J3DFrameCtrl::EMode_NONE, MoiR1},
    {BCK_MOIR_TAKE_MET_SIT, J3DFrameCtrl::EMode_NONE, MoiR1},
    {BCK_MOIR_TAKAJO, J3DFrameCtrl::EMode_NONE, MoiR3},
    {BCK_MOIR_TAKAJO_WAIT, J3DFrameCtrl::EMode_LOOP, MoiR3},
    {BCK_MOIR_TAKAJO_HI, J3DFrameCtrl::EMode_NONE, MoiR3},
    {BCK_MOIR_TAKAJO_HI_WAIT, J3DFrameCtrl::EMode_LOOP, MoiR3},
    {BCK_MOIR_RUN, J3DFrameCtrl::EMode_LOOP, MoiR3},
};

static daNpc_GetParam2 l_btpGetParamList[2] = {
    {BTP_MOIR, J3DFrameCtrl::EMode_LOOP, MoiR},
    {BTP_MOIR_FH_SERIOUS, J3DFrameCtrl::EMode_LOOP, MoiR},
};

static daNpc_GetParam2 l_btkGetParamList[1] = {
    {BTK_MOIR, J3DFrameCtrl::EMode_LOOP, MoiR},
};

static int l_loadRes_MOIR0[3] = {
    0, 1, -1,
};

static int l_loadRes_MOIR1[3] = {
    0, 2, -1,
};

static int l_loadRes_MOIR2[3] = {
    0, 3, -1,
};

static int* l_loadRes_list[3] = {
    l_loadRes_MOIR0,
    l_loadRes_MOIR1,
    l_loadRes_MOIR2,
};

static char* l_arcNames[4] = {
    "MoiR",
    "MoiR1",
    "MoiR2",
    "MoiR3",
};

static char* l_evtNames[4] = {
    "",
    "THE_INTRODUCTION",
    "MOIR_APPEAR",
    "MOIR_APPEAR2",
};

static char* l_myName = "MoiR";

daNpcMoiR_HIOParam const daNpcMoiR_Param_c::m = {
    35.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    200.0f,
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
    3,
    6,
    5,
    6,
    80.0f,
    500.0f,
    300.0f,
    -300.0f,
    60,
    8,
    0,
    0,
    0,
    false,
    false,
    300.0f,
    0x15CD,
    0,
};

#if DEBUG
daNpcMoiR_HIO_c::daNpcMoiR_HIO_c() {
    m = daNpcMoiR_Param_c::m;
}

void daNpcMoiR_HIO_c::genMessage(JORMContext* ctext) {
    ctext->genSlider("目玉横可動範囲", &m.horizontal_eye_movement_range, 0, 0x7fff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genSlider("デモ開始距離", &m.demo_start_dist, 200.0f, 600.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    ctext->genCheckBox("デモ開始エリア", &m.demo_start_area, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    daNpcF_commonGenMessage(ctext, &m.common);
}
#endif

static NPC_MOIR_HIO_CLASS l_HIO;

daNpcMoiR_c::EventFn daNpcMoiR_c::mEvtSeqList[4] = {
    NULL,
    &daNpcMoiR_c::EvCut_Introduction,
    &daNpcMoiR_c::EvCut_Appear,
    &daNpcMoiR_c::EvCut_Appear2,
};

daNpcMoiR_c::daNpcMoiR_c() {}

daNpcMoiR_c::~daNpcMoiR_c() {
    for (int i = 0; l_loadRes_list[mMode][i] >= 0; i++) {
        dComIfG_resDelete(&mPhase[i], l_arcNames[l_loadRes_list[mMode][i]]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
#if DEBUG
    if (mpHIO) {
        mpHIO->removeHIO();
    }
#endif
}

cPhs__Step daNpcMoiR_c::Create() {
    fopAcM_ct(this, daNpcMoiR_c);

    if (strcmp(dComIfGp_getStartStageName(), "R_SP116") == 0 && dComIfG_play_c::getLayerNo(0) == 4) {
        if (daNpcF_chkEvtBit(0x169) /* dSv_event_flag_c::F_0361 - Arbiter's Grounds - Spun the spinning pillars */
            && daNpcF_chkEvtBit(0x10A) /* dSv_event_flag_c::F_0266 - Snowpeak Ruins - Snowpeak Ruins clear */
            && !daNpcF_chkEvtBit(0x10B) /* dSv_event_flag_c::F_0267 - Temple of Time - Temple of Time clear */) {
            return cPhs_ERROR_e;
        }

        if (dComIfGs_isStageSwitch(0x18, 0x4B)) {
            return cPhs_ERROR_e;
        }

        mMode = MODE_SIT;
    } else if (strcmp(dComIfGp_getStartStageName(), "F_SP108") == 0) {
        if (daNpcF_chkEvtBit(0x10B) /* dSv_event_flag_c::F_0267 - Temple of Time - Temple of Time clear */) {
            return cPhs_ERROR_e;
        }

        mMode = MODE_STAND;
    } else if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
        if (dComIfGs_isSaveSwitch(0x14)) {
            return cPhs_ERROR_e;
        }

        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
        mMode = MODE_2;
    }

    mMsgNo = getMessageNo();
    cPhs__Step phase = cPhs_ERROR_e;
    for (int i = 0; l_loadRes_list[mMode][i] >= 0; i++) {
        phase = (cPhs__Step)dComIfG_resLoad(&mPhase[i], l_arcNames[l_loadRes_list[mMode][i]]);
        if (phase != cPhs_COMPLEATE_e) {
            return phase;
        }
    }

    if (phase == cPhs_COMPLEATE_e) {
        u32 i_size = 0;
        switch (mMode) {
            case MODE_SIT:
                i_size = 0x4850;
                break;

            case MODE_STAND:
                i_size = 0x4860;
                break;

            case MODE_2:
                i_size = 0x4850;
                break;

            default:
                JUT_ASSERT(343, FALSE);
                break;
        }

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, i_size)) {
            return cPhs_ERROR_e;
        }

        J3DModelData* model_data = mAnm_p->getModel()->getModelData();
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -100.0f, -50.0f, -100.0f, 100.0f, 220.0f, 100.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
#if DEBUG
        mpHIO = &l_HIO;
        // "Moi (Resistance)"
        mpHIO->entryHIO("モイ（レジスタンス）");
#endif

        mAcchCir.SetWall(mpHIO->m.common.width, mpHIO->m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                  &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.SetRoofNone();
        mAcch.SetWaterNone();
        mAcch.CrrPos(dComIfG_Bgsp());

        mCcStts.Init(mpHIO->m.common.weight, 0, this);
        field_0xca0.Set(mCcDCyl);
        field_0xca0.SetStts(&mCcStts);
        field_0xca0.SetTgType(0);
        field_0xca0.SetTgSPrm(0);

        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();
        mAnm_p->modelCalc();
        reset();
        Execute();
    }

    return phase;
}

int daNpcMoiR_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;
    J3DModel* mdl_p = NULL;
    mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcNames[0], BMDR_MOIR));

    JUT_ASSERT(403, NULL != mdlData_p);

    u32 sp_0x18 = 0x11020284;
    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, sp_0x18);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    mdl_p = mAnm_p->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    mdl_p->setUserArea((uintptr_t)this);

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcNames[0], BMDR_HEADGEAR));

    JUT_ASSERT(436, mdlData_p != NULL);

    mpHeadgearModel = mDoExt_J3DModel__create(mdlData_p, 0x80000, 0x11000084);

    if (!setExpressionAnm(ANM_F_TALK_A, false)) {
        return 0;
    }

    switch (mMode) {
        case MODE_SIT:
            setMotionAnm(ANM_SIT, 0.0f);
            break;

        case MODE_STAND:
            setMotionAnm(ANM_WAIT_C, 0.0f);
            break;

        case MODE_2:
            setMotionAnm(ANM_TAKAJO_HI_WAIT, 0.0f);
            break;

        default:
            JUT_ASSERT(448, FALSE);
            break;
    }

    return 1;
}

int daNpcMoiR_c::Delete() {
    fopAcM_RegisterDeleteID(this, "NPC_MOIR");
    this->~daNpcMoiR_c();
    return 1;
}

int daNpcMoiR_c::Execute() {
    execute();
    return 1;
}

int daNpcMoiR_c::Draw() {
    J3DModelData* model_data = mAnm_p->getModel()->getModelData();
    model_data->getMaterialNodePointer(MAT_MOIR_EYEBALL_M)->setMaterialAnm(mpMatAnm);
    draw(FALSE, FALSE, mpHIO->m.common.real_shadow_size, NULL, FALSE);
    return 1;
}

int daNpcMoiR_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* my_joint = i_joint;
    int jntNo = my_joint->getJntNo();
    int i_jointList[3] = {JNT_BACKBONE1, JNT_NECK, JNT_HEAD};

    if (jntNo == 0) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_BACKBONE1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);

        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_NECK));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);

        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));

    switch (jntNo) {
        case JNT_BACKBONE1:
        case JNT_NECK:
        case JNT_HEAD:
            setLookatMtx(jntNo, i_jointList, mpHIO->m.common.neck_rotation_ratio);
            break;

    }

    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((jntNo == JNT_HEAD || jntNo == JNT_MOUTH) && (mAnmFlags & ANM_PLAY_BCK) != 0) {
        J3DAnmTransform* anm = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(anm);
    }

    return 1;
}

int daNpcMoiR_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpcMoiR_c* i_this = (daNpcMoiR_c*)a_this;
    return i_this->CreateHeap();
}

int daNpcMoiR_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpcMoiR_c* i_this = (daNpcMoiR_c*)model->getUserArea();

        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daNpcMoiR_c::searchActors() {
    if (mActorMngr[1].getActorP() == NULL) {
        mActorMngr[1].entry(fopAcM_SearchByName(PROC_NPC_ASH));
    }

    if (mActorMngr[2].getActorP() == NULL) {
        mActorMngr[2].entry(fopAcM_SearchByName(PROC_NPC_SHAD));
    }

    if (mActorMngr[3].getActorP() == NULL) {
        mActorMngr[3].entry(fopAcM_SearchByName(PROC_NPC_RAFREL));
    }
}

#if DEBUG
static s16 dummy_lit_120628(int sel) {
    const s16 arr[2] = {0x00C8, 0x00FF};
    return arr[sel];
}

static s16 dummy_lit_120631(int sel) {
    const s16 arr[2] = {0xC800, 0x00FF};
    return arr[sel];
}

static s16 dummy_lit_120638(int sel) {
    const s16 arr[2] = {0x0000, 0xC8FF};
    return arr[sel];
}

static s16 dummy_lit_120641(int sel) {
    const s16 arr[2] = {0x0000, 0xC8FF};
    return arr[sel];
}

static s16 dummy_lit_120644(int sel) {
    const s16 arr[2] = {0x0000, 0xC8FF};
    return arr[sel];
}

static s16 dummy_lit_120647(int sel) {
    const s16 arr[2] = {0x8080, 0x80A0};
    return arr[sel];
}

static s16 dummy_lit_120650(int sel) {
    const s16 arr[2] = {0x8080, 0x80A0};
    return arr[sel];
}
#endif

bool daNpcMoiR_c::setExpressionAnm(int i_index, bool i_modify) {
    mAnmFlags &= 0xFFFFE03F;

    J3DAnmTransform* anmTransform = (l_bckGetParamList[i_index].fileIdx >= 0)
        ? getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[i_index].arcIdx], l_bckGetParamList[i_index].fileIdx)
        : NULL;

    int i_attr = l_bckGetParamList[i_index].attr;
    bool bVar1 = false;
    switch (i_index) {
        case ANM_NONE:
            bVar1 = setExpressionBtp(EXPR_BTP_MOIR);
            break;

        case ANM_F_TALK_A:
            bVar1 = setExpressionBtp(EXPR_BTP_MOIR);
            break;

        case ANM_F_HOLD_S:
            bVar1 = setExpressionBtp(EXPR_BTP_MOIR);
            break;

        case ANM_3:
            bVar1 = setExpressionBtp(EXPR_BTP_MOIR);
            break;

        case ANM_F_SURPRISED:
            bVar1 = setExpressionBtp(EXPR_BTP_MOIR);
            break;

        case ANM_F_HOLD_S_LOOP:
            bVar1 = setExpressionBtp(EXPR_BTP_MOIR);
            break;

        case ANM_F_TAKAJO_HI:
            bVar1 = setExpressionBtp(EXPR_BTP_MOIR);
            break;

        case ANM_FH_HOLD_S:
            bVar1 = setExpressionBtp(EXPR_BTP_MOIR);
            break;

        case ANM_FH_SERIOUS:
            bVar1 = setExpressionBtp(EXPR_BTP_FH_SERIOUS);
            break;

        case ANM_FH_HOLD_S_2:
            bVar1 = setExpressionBtp(EXPR_BTP_MOIR);
            break;

        case ANM_FH_TAKAJO_HI:
            bVar1 = setExpressionBtp(EXPR_BTP_MOIR);
            break;

        default:
            anmTransform = NULL;
            break;
    }

    if (!bVar1) {
        return false;
    }

    if (anmTransform == NULL) {
        return true;
    }

    if (setBckAnm(anmTransform, 1.0f, i_attr, 0, -1, i_modify)) {
        mAnmFlags |= ANM_PLAY_BCK | ANM_PAUSE_BCK;
        mExpressionLoops = 0;
        return true;
    }

    OS_REPORT("%s: 表情Bckアニメーションの登録に失敗しました！\n", "d_a_npc_moir.cpp");
    return false;
}

bool daNpcMoiR_c::setExpressionBtp(int i_index) {
    J3DAnmTexPattern* anmTexPattern = getTexPtrnAnmP(l_arcNames[l_btpGetParamList[i_index].arcIdx], l_btpGetParamList[i_index].fileIdx);
    int i_attr = l_btpGetParamList[i_index].attr;

    mAnmFlags &= 0xFFFFF57F;

    if (anmTexPattern == NULL) {
        return true;
    }

    if (setBtpAnm(anmTexPattern, mAnm_p->getModel()->getModelData(), 1.0f, i_attr)) {
        mAnmFlags |= ANM_PLAY_BTP | ANM_PAUSE_BTP;

        if (i_index == EXPR_BTP_MOIR) {
            mAnmFlags |= ANM_FLAG_800;
        }

        return true;
    }

    OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", "d_a_npc_moir.cpp");
    return false;
}

void daNpcMoiR_c::setMotionAnm(int i_index, f32 i_morf) {
    if (i_index < 11 || i_index >= 0x27) {
        return;
    }
    int reg_r29 = 0;

    J3DAnmTransformKey* morfAnm = getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[i_index].arcIdx], l_bckGetParamList[i_index].fileIdx);
    J3DAnmTextureSRTKey* btkAnm = getTexSRTKeyAnmP(l_arcNames[l_btkGetParamList[reg_r29].arcIdx], l_btkGetParamList[reg_r29].fileIdx);
    int i_morfAttr = l_bckGetParamList[i_index].attr;
    int i_btkAttr = l_btkGetParamList[reg_r29].attr;

    mAnmFlags &= 0xFFFFFFC0;

    if (morfAnm != NULL && setMcaMorfAnm(morfAnm, 1.0f, i_morf, i_morfAttr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }

    if (btkAnm != NULL) {
        if (setBtkAnm(btkAnm, mAnm_p->getModel()->getModelData(), 1.0f, i_btkAttr)) {
            mAnmFlags |= ANM_PLAY_BTK | ANM_PAUSE_BTK;
        }
    }
}

void daNpcMoiR_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mLookat.initialize();

    for (int i = 0; i < 4; i++) {
        mActorMngr[i].initialize();
    }

    field_0xdfc = 0;
    field_0xe00 = 0;
    mAction = NULL;
    mLookMode = -1;
    field_0xe08 = 0;
    current.pos.set(home.pos);
    old.pos.set(current.pos);
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);
    field_0xdf8 = -1;
    mOrderEvtNo = 0;
    mExpressionMorfOverride = 0.0f;
    mMotionMorfOverride = 0.0f;
    mWearHelmet = true;
    setWaitAction();
}

void daNpcMoiR_c::setWaitAction() {
    switch (mMode) {
        case MODE_SIT:
            setAction(&daNpcMoiR_c::wait_type0);
            break;

        case MODE_STAND:
            setAction(&daNpcMoiR_c::wait_type1);
            break;

        case MODE_2:
            setAction(&daNpcMoiR_c::wait_type2);
            break;

        default:
            JUT_ASSERT(1315, FALSE);
            break;
    }
}

void daNpcMoiR_c::playExpression() {
    daNpcF_anmPlayData dat0 = {ANM_F_TALK_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {ANM_F_HOLD_S_LOOP, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2 = {ANM_3, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3 = {ANM_FH_SERIOUS, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4 = {ANM_F_TAKAJO_HI, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5 = {ANM_FH_TAKAJO_HI, 0.0f, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData dat6 = {ANM_FH_HOLD_S_2, 0.0f, 0};
    daNpcF_anmPlayData* pDat6[1] = {&dat6};
    daNpcF_anmPlayData dat7 = {ANM_F_SURPRISED, 4.0f, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8 = {ANM_F_HOLD_S, 0.0f, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};
    daNpcF_anmPlayData dat9 = {ANM_FH_HOLD_S, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat9[1] = {&dat9};
    daNpcF_anmPlayData dat10 = {ANM_NONE, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat10[1] = {&dat10};
    daNpcF_anmPlayData* pDat11[1] = {&dat0};

    daNpcF_anmPlayData** ppDat[19] = {
        pDat0,
        pDat11,
        pDat11,
        pDat1,
        pDat11,
        pDat11,
        pDat11,
        pDat11,
        pDat11,
        pDat11,
        pDat2,
        pDat3,
        pDat4,
        pDat5,
        pDat6,
        pDat7,
        pDat8,
        pDat9,
        pDat10,
    };

    if (mExpression >= 0 && mExpression < 0x13) {
        playExpressionAnm(ppDat);
    }
}

void daNpcMoiR_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_TALK_A, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_WAIT_C, 0.0f, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2 = {ANM_WAIT_B, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3 = {ANM_WAIT_C, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4a = {ANM_LOOK_AROUND, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat4b = {ANM_WAIT_C, 0.0f, 0};
    daNpcF_anmPlayData* pDat4[2] = {&dat4a, &dat4b};
    daNpcF_anmPlayData dat5 = {ANM_TAKAJO_WAIT_NI, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData dat6 = {ANM_SIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat6[1] = {&dat6};
    daNpcF_anmPlayData dat7a = {ANM_SIT_TALK, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat7b = {ANM_SIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat7[2] = {&dat7a, &dat7b};
    daNpcF_anmPlayData dat8 = {ANM_SQUAREUP, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};
    daNpcF_anmPlayData dat9a = {ANM_DRIVEAWAY, 0.0f, 1};
    daNpcF_anmPlayData dat9b = {ANM_SQUAREUP, 0.0f, 0};
    daNpcF_anmPlayData* pDat9[2] = {&dat9a, &dat9b};
    daNpcF_anmPlayData dat10a = {ANM_TAKEOFF, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat10b = {ANM_TAKING_OFF, 0.0f, 0};
    daNpcF_anmPlayData* pDat10[2] = {&dat10a, &dat10b};
    daNpcF_anmPlayData dat11 = {ANM_TAKING_OFF, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat11[1] = {&dat11};
    daNpcF_anmPlayData dat12a = {ANM_TAKING_OFF_TALK, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat12b = {ANM_TAKING_OFF, 0.0f, 0};
    daNpcF_anmPlayData* pDat12[2] = {&dat12a, &dat12b};
    daNpcF_anmPlayData dat13a = {ANM_TAKE_MET, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat13b = {ANM_WAIT_C, 0.0f, 0};
    daNpcF_anmPlayData* pDat13[2] = {&dat13a, &dat13b};
    daNpcF_anmPlayData dat14a = {ANM_TAKEOFF_SIT, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat14b = {ANM_TAKING_OFF_SIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat14[2] = {&dat14a, &dat14b};
    daNpcF_anmPlayData dat15 = {ANM_TAKING_OFF_SIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat15[1] = {&dat15};
    daNpcF_anmPlayData dat16a = {ANM_TAKING_OFF_SIT_TALK, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat16b = {ANM_TAKING_OFF_SIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat16[2] = {&dat16a, &dat16b};
    daNpcF_anmPlayData dat17a = {ANM_TAKE_MET_SIT, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat17b = {ANM_SIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat17[2] = {&dat17a, &dat17b};
    daNpcF_anmPlayData dat18 = {ANM_RUN, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat18[1] = {&dat18};
    daNpcF_anmPlayData dat19 = {ANM_TAKAJO_HI_WAIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat19[1] = {&dat19};
    daNpcF_anmPlayData dat20a = {ANM_TAKAJO_HI, 0.0f, 1};
    daNpcF_anmPlayData dat20b = {ANM_TAKAJO_HI_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat20[2] = {&dat20a, &dat20b};
    daNpcF_anmPlayData dat21a = {ANM_SURPRISED, 0.0f, 1};
    daNpcF_anmPlayData dat21b = {ANM_SQUAREUP, 0.0f, 0};
    daNpcF_anmPlayData* pDat21[2] = {&dat21a, &dat21b};
    daNpcF_anmPlayData dat22 = {ANM_SQUAREUP_STEP, 2.0f, 1};
    daNpcF_anmPlayData* pDat22[1] = {&dat22};
    daNpcF_anmPlayData dat23a = {ANM_TAKAJO_NI, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat23b = {ANM_TAKAJO_WAIT_NI, 0.0f, 0};
    daNpcF_anmPlayData* pDat23[2] = {&dat23a, &dat23b};
    daNpcF_anmPlayData dat24 = {ANM_FUE, 0.0f, 1};
    daNpcF_anmPlayData* pDat24[2] = {&dat24};
    daNpcF_anmPlayData dat25 = {ANM_LOOK_AROUND, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat25[1] = {&dat25};
    daNpcF_anmPlayData dat26 = {ANM_STEP, 4.0f, 1};
    daNpcF_anmPlayData* pDat26[1] = {&dat26};
    daNpcF_anmPlayData dat27 = {ANM_NONE, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat27[1] = {&dat27};

    daNpcF_anmPlayData** ppDat[43] = {
        pDat0,
        pDat1,
        pDat2,
        pDat27,
        pDat27,
        pDat27,
        pDat27,
        pDat3,
        pDat27,
        pDat27,
        pDat27,
        pDat27,
        pDat27,
        pDat27,
        pDat27,
        pDat27,
        pDat27,
        pDat27,
        pDat27,
        pDat27,
        pDat4,
        pDat5,
        pDat6,
        pDat7,
        pDat8,
        pDat9,
        pDat10,
        pDat11,
        pDat12,
        pDat13,
        pDat14,
        pDat15,
        pDat16,
        pDat17,
        pDat18,
        pDat19,
        pDat20,
        pDat21,
        pDat22,
        pDat23,
        pDat24,
        pDat25,
        pDat26,
    };

    if (mMotion >= 0 && mMotion < 0x2B) {
        playMotionAnm(ppDat);
    }
}

void daNpcMoiR_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL bVar1 = FALSE;
    f32 body_angleX_min = mpHIO->m.common.body_angleX_min;
    f32 body_angleX_max = mpHIO->m.common.body_angleX_max;
    f32 body_angleY_min = mpHIO->m.common.body_angleY_min;
    f32 body_angleY_max = mpHIO->m.common.body_angleY_max;
    f32 head_angleX_min = mpHIO->m.common.head_angleX_min;
    f32 head_angleX_max = mpHIO->m.common.head_angleX_max;
    f32 head_angleY_min = mpHIO->m.common.head_angleY_min;
    f32 head_angleY_max = mpHIO->m.common.head_angleY_max;
    s16 sVar1 = mCurAngle.y - mOldAngle.y;
    cXyz spd0[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* local_dc[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};
    cXyz spe8;

    switch (mLookMode) {
        case LOOK_NONE:
            break;
            
        case LOOK_RESET:
            bVar1 = TRUE;
            break;

        case LOOK_PLAYER:
        case LOOK_PLAYER_TALK:
            player = daPy_getPlayerActorClass();

            if (mMode == MODE_STAND) {
                if (mLookMode == LOOK_PLAYER_TALK) {
                    head_angleY_min = -80.0f;
                    head_angleY_max = 80.0f;
                }
            }

            if (mMode == MODE_SIT) {
                J3DAnmTransform* anm = mAnm_p->getAnm();
                if (anm == getTrnsfrmKeyAnmP(l_arcNames[1], l_bckGetParamList[30].fileIdx) ||
                    anm == getTrnsfrmKeyAnmP(l_arcNames[1], l_bckGetParamList[33].fileIdx)) {
                    player = NULL;
                }
            }
            break;

        default:
            break;
    }

    if (player != NULL) {
        mLookPos = player->attention_info.position;

        if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK) {
            mLookPos.y -= 40.0f;
        }

        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(0);
    }

    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max, 0.0f, 0.0f, 0.0f, 0.0f, 
                        head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max, mCurAngle.y, spd0);
    mLookat.calc(this, model->getBaseTRMtx(), local_dc, bVar1, sVar1, FALSE);
}

BOOL daNpcMoiR_c::chkFindPlayer() {
    f32 fVar1 = mMode == MODE_SIT ? 135.0f : mpHIO->m.common.fov;
    int reg_r30;

    if (!chkActorInSight(daPy_getPlayerActorClass(), fVar1)) {
        mActorMngr[0].remove();
        return FALSE;
    } else {
        reg_r30 = 0;
        if (mActorMngr[0].getActorP() == NULL) {
            reg_r30 = chkPlayerInSpeakArea(this);
        } else {
            reg_r30 = chkPlayerInTalkArea(this);
        }

        if (reg_r30) {
            mActorMngr[0].entry(daPy_getPlayerActorClass());
        } else {
            mActorMngr[0].remove();
        }
    }

    return reg_r30;
}

void daNpcMoiR_c::setLookMode(int i_lookMode) {
    if (i_lookMode >= LOOK_NONE && i_lookMode < LOOK_ACTOR && i_lookMode != mLookMode) {
        mLookMode = i_lookMode;
    }
}

BOOL daNpcMoiR_c::step(s16 param_1, int param_2) {
    if (mTurnMode == 0) {
        if (param_2 != 0) {
            s16 angle_diff = param_1 - mCurAngle.y;
            if ((s32)fabsf(cM_sht2d(angle_diff)) > 40) {
                setExpression(EXPR_NONE, -1.0f);

                if (chkAction(&daNpcMoiR_c::fight)) {
                    setMotion(MOT_SQUAREUP_STEP, -1.0f, 0);
                } else {
                    setMotion(MOT_STEP, -1.0f, 0);
                }
            }
        }

        mTurnTargetAngle = param_1;
        mTurnAmount = 0;

        if (mCurAngle.y == mTurnTargetAngle) {
            mTurnMode++;
        }

        current.angle.y = mCurAngle.y;
        shape_angle.y = current.angle.y;
        mTurnMode++;
    } else if (mTurnMode == 1) {
        f32 turn_val = (mMotion == MOT_SURPRISED && mMotionPhase <= 0) ? 8.0f : 15.0f;
        if (turn(mTurnTargetAngle, turn_val, 0)) {
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

#if VERSION != VERSION_SHIELD_DEBUG
static daNpcMoiR_c::Action2Fn dummy_4854() {
    return &daNpcMoiR_c::multiTalk;
}
#endif

bool daNpcMoiR_c::wait_type0(void* param_1) {
    switch (field_0xe08) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_SIT, -1.0f, 0);
            mTurnMode = 0;
            speedF = 0.0f;
            field_0xe08 = 2;
            break;

        case 2:
            if (mActorMngr[0].getActorP() != NULL) {
                if (!chkFindPlayer()) {
                    mTurnMode = 0;
                }
            } else if (chkFindPlayer()) {
                mTurnMode = 0;
            }

            if (mActorMngr[0].getActorP() != NULL) {
                setLookMode(LOOK_PLAYER);
            } else {
                setLookMode(LOOK_NONE);
            }

            if (dComIfGp_event_runCheck()) {
                if (eventInfo.checkCommandTalk()) {
                    if (!dComIfGp_event_chkTalkXY() || dComIfGp_evmng_ChkPresentEnd()) {
                        setAction(&daNpcMoiR_c::talk);
                    }
                } else {
                    int staff_id = dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0);
                    if (staff_id != -1) {
                        setAction(&daNpcMoiR_c::demo);
                    }
                }
            } else {
                if (mOrderEvtNo != 0) {
                    eventInfo.setArchiveName(l_arcNames[1]);
                }

                orderEvent(mOrderSpeakEvt, (mOrderEvtNo != 0) ? l_evtNames[mOrderEvtNo] : NULL, 0xFFFF, 0x28, 0xFF, 1);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1652, FALSE);
            break;
    }

    return true;
}

void daNpcMoiR_c::setMotion(int i_motion, f32 i_morf, int param_3) {
    s16 motion = i_motion;
    if ((param_3 != 0 || mMotion != motion) && i_motion >= 0 && i_motion < 0x2B) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

void daNpcMoiR_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 0x13) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

bool daNpcMoiR_c::wait_type1(void* param_1) {
    f32 fVar1;

    switch (field_0xe08) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_C, -1.0f, 0);
            field_0xe00 = cLib_getRndValue(3, 5);
            mTurnMode = 0;
            speedF = 0.0f;
            field_0xe08 = 2;
            break;

        case 2: {
            if (mActorMngr[0].getActorP() != NULL) {
                if (!chkFindPlayer()) {
                    mTurnMode = 0;
                }
            } else if (chkFindPlayer()) {
                mTurnMode = 0;
            }

            if (mActorMngr[0].getActorP() != NULL) {
                setLookMode(LOOK_PLAYER);
            } else {
                setLookMode(LOOK_NONE);
            }

            if (mOrderEvtNo == 0 && home.angle.y != mCurAngle.y && step(home.angle.y, 1)) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_C, -1.0f, 0);
                field_0xe00 = cLib_getRndValue(3, 5);
                mTurnMode = 0;
            }

            if (mMotionLoops >= field_0xe00) {
                if (mMotion == MOT_LOOK_AROUND) {
                    setMotion(MOT_WAIT_C, -1.0f, 0);
                    field_0xe00 = cLib_getRndValue(3, 5);
                } else {
                    setMotion(MOT_LOOK_AROUND, -1.0f, 0);
                    field_0xe00 = 1;
                }
            }

            if (daPy_py_c::checkNowWolf()) {
                fVar1 = std::pow(300.0f, 2.0f);
                if (fopAcM_searchPlayerDistanceXZ2(this) < fVar1) {
                    setAction(&daNpcMoiR_c::fight);
                }
            } else if (!daNpcF_chkEvtBit(0x21F) /* dSv_event_flag_c::F_0543 - Faron Woods - Met Resistance Rusl again in the woods */
                       && fopAcM_searchPlayerDistance(this) < mpHIO->m.demo_start_dist) {
                daNpcF_onEvtBit(0x21F); // dSv_event_flag_c::F_0543 - Faron Woods - Met Resistance Rusl again in the woods
                daNpcF_offTmpBit(0xB); // dSv_event_tmp_flag_c::T_0010 - General use - General use temporary flag (flow control) A
                daNpcF_offTmpBit(0xC); // dSv_event_tmp_flag_c::T_0011 - General use - General use temporary flag (flow control) B
                daNpcF_offTmpBit(0xD); // dSv_event_tmp_flag_c::T_0012 - General use - General use temporary flag (flow control) C
                daNpcF_offTmpBit(0xE); // dSv_event_tmp_flag_c::T_0013 - General use - General use temporary flag (flow control) D
                daNpcF_offTmpBit(0xF); // dSv_event_tmp_flag_c::T_0014 - General use - General use temporary flag (flow control) E
                mOrderEvtNo = 2;
            }

#if VERSION != VERSION_SHIELD_DEBUG
            // This is fake, but it matches on GCN:
            Action2Fn dummy_5013 = &daNpcMoiR_c::multiTalk;
#endif

            if (dComIfGp_event_runCheck()) {
                if (eventInfo.checkCommandTalk()) {
                    if (!dComIfGp_event_chkTalkXY() || dComIfGp_evmng_ChkPresentEnd()) {
                        setAction(&daNpcMoiR_c::talk);
                    }
                } else {
                    int staff_id = dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0);
                    if (staff_id != -1) {
                        setAction(&daNpcMoiR_c::demo);
                    }
                }
            } else {
                if (mOrderEvtNo != 0) {
                    eventInfo.setArchiveName(l_arcNames[2]);
                }

                orderEvent(mOrderSpeakEvt, (mOrderEvtNo != 0) ? l_evtNames[mOrderEvtNo] : NULL, 0xFFFF, 0x28, 0xFF, 1);
            }
            break;
        }

        case 3:
            break;

        default:
            JUT_ASSERT(1756, FALSE);
            break;
    }

    return true;
}

bool daNpcMoiR_c::wait_type2(void* param_1) {
    switch (field_0xe08) {
        case 0:
            setExpression(EXPR_FH_TAKAJO_HI, -1.0f);
            setMotion(MOT_TAKAJO_HI_WAIT, -1.0f, 0);
            mTurnMode = 0;
            speedF = 0.0f;
            mActorMngr[0].entry(daPy_getPlayerActorClass());
            setLookMode(LOOK_NONE);
            field_0xe08 = 2;
            break;

        case 2:
            switch (field_0xe00) {
                case 0:
                    if (daNpcF_chkTmpBit(0x76) /* dSv_event_tmp_flag_c::MN09_03 - Hyrule Castle - Auru's cannon hits Flag */) {
                        current.pos.set(-4080.086f, 0.0f, 8094.616f);
                        old.pos = current.pos;
                        setAngle(-0x6000);
                        field_0xe00++;
                    }
                    break;

                case 1:
                    if (daNpcF_chkTmpBit(0x75) /* dSv_event_tmp_flag_c::MN09_02 - Hyrule Castle - Rusl waves at Link Flag */) {
                        setExpression(EXPR_F_TAKAJO_HI, -1.0f);
                        setMotion(MOT_TAKAJO_HI, -1.0f, 0);
                        field_0xe00++;
                    }
                    break;

                case 2:
                    if (daNpcF_chkTmpBit(0x74) /* dSv_event_tmp_flag_c::WXXTO - Hyrule Castle - Resistance leaves Flag */) {
                        setAction(&daNpcMoiR_c::leave);
                    }
                    break;

                default:
                    JUT_ASSERT(1805, FALSE);
                    break;
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1810, FALSE);
            break;
    }

    return true;
}

void daNpcMoiR_c::setExpressionTalkAfter() {
    switch (mExpression) {
        case 3:
            setExpression(EXPR_FH_HOLD_S, -1.0f);
            break;

        case 10:
            setExpression(EXPR_FH_HOLD_S_LOOP, -1.0f);
            break;

        default:
            setExpression(EXPR_NONE, -1.0f);
            break;
    }
}

bool daNpcMoiR_c::talk(void* param_1) {
    int sp_0x14 = 0;
    int itemNo, i_expression, i_actionID;
    bool rv = false;

    switch (field_0xe08) {
        case 0:
            initTalk(mMsgNo, NULL);
            mTurnMode = 0;
            mMsgTimer = 0;
            speedF = 0.0f;
            setLookMode(LOOK_PLAYER_TALK);
            field_0xe08 = 2;
            break;

        case 2:
            if (mMode == MODE_SIT || fopAcM_searchPlayerAngleY(this) == mCurAngle.y) {
                if (talkProc(NULL, TRUE, NULL)) {
                    mActorMngr[0].entry(daPy_getPlayerActorClass());
                    itemNo = 0;
                    u16 endConversationID = mFlow.getEventId(&itemNo);

                    OS_REPORT("会話終了時 イベントID=%d アイテムNo=%d\n", endConversationID, itemNo);

                    if (endConversationID == 1) {
                        field_0xdf8 = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, -1, NULL, NULL);

                        if (field_0xdf8 != -1) {
                            s16 i_eventID = dComIfGp_getEventManager().getEventIdx(this, "DEFAULT_GETITEM", 0xFF);
                            dComIfGp_getEvent()->reset(this);
                            fopAcM_orderChangeEventId(this, i_eventID, 1, 0xFFFF);
                            field_0x9ec = true;
                        }
                    } else if (mMode == MODE_STAND && mFlow.getChoiceNo() == 0) {
                        mOrderEvtNo = 3;
                        changeEvent(l_arcNames[2], l_evtNames[mOrderEvtNo], 1, 0xFFFF);
                    }

                    setWaitAction();
                    rv = true;

                } else {
                    int msgTimer = mMsgTimer;
                    if (ctrlMsgAnm(i_expression, i_actionID, this, FALSE)) {
                        OS_REPORT("レジスタンスモイ 表情番号=%d 動作番号=%d\n", i_expression, i_actionID); // "Resistance Moi Expression Number=%d Action Number=%d\n"
                        setExpression(i_expression, -1.0f);
                        setMotion(i_actionID, -1.0f, 0);
                    } else if (msgTimer != 0 && mMsgTimer == 0) {
                        setExpressionTalkAfter();
                    }
                }
            } else if (step(fopAcM_searchPlayerAngleY(this), 1)) {
                setMotion(MOT_WAIT_C, -1.0f, 0);
                mTurnMode = 0;
            }
            break;

        case 3:
            setExpression(EXPR_NONE, -1.0f);

            if (!field_0x9ec) {
                dComIfGp_event_reset();
            }
            break;

        default:
            JUT_ASSERT(1893, FALSE);
            break;
    }

    return rv;
}

BOOL daNpcMoiR_c::multiTalk(void* param_1) {
    BOOL rv = FALSE;
    fopAc_ac_c* speakers[4] = {this, mActorMngr[1].getActorP(), mActorMngr[2].getActorP(), mActorMngr[3].getActorP()};

    dComIfGp_setMesgCameraInfoActor(speakers[0], speakers[1], speakers[2], speakers[3], NULL, NULL, NULL, NULL, NULL, NULL);

    switch (field_0xe08) {
        case 0:
            initTalk(mMsgNo, NULL);
            mMsgTimer = 0;

            if (mLookMode != LOOK_NONE) {
                mLookMode = LOOK_NONE;
            }

            field_0xe08 = 2;
            break;

        case 2:
            if (talkProc(NULL, TRUE, (fopAc_ac_c**)&speakers)) {
                setWaitAction();
                rv = TRUE;
            } else {
                int msgTimer = mMsgTimer;
                int i_expression, i_motion;

                if (ctrlMsgAnm(i_expression, i_motion, this, FALSE) != 0) {
                    setExpression(i_expression, -1.0f);
                    setMotion(i_motion, -1.0f, 0);
                } else if (msgTimer != 0 && mMsgTimer == 0) {
                    switch (mExpression) {
                        case 3:
                            setExpression(EXPR_FH_HOLD_S, -1.0f);
                            break;

                        case 10:
                            setExpression(EXPR_FH_HOLD_S_LOOP, -1.0f);
                            break;

                        default:
                            setExpression(EXPR_NONE, -1.0f);
                            break;
                    }
                }
            }
            break;

        case 3:
            setExpression(EXPR_NONE, -1.0f);
            dComIfGp_event_reset();
            break;
    }

    return rv;
}

bool daNpcMoiR_c::fight(void* param_1) {
    f32 fVar1;
    int iVar1 = 1;

    switch (field_0xe08) {
        case 0:
            setExpression(EXPR_F_SURPRISED, -1.0f);
            setMotion(MOT_SURPRISED, -1.0f, 0);
            mSound.startCreatureVoice(Z2SE_MOIR_V_WOLF_SURPRISE, -1);
            setLookMode(LOOK_PLAYER);
            mTurnMode = 0;
            field_0xe08 = 2;
            break;

        case 2:
            if (mMotion == MOT_SURPRISED && mMotionPhase <= 0) {
                iVar1 = 0;
            }

            if (step(fopAcM_searchPlayerAngleY(this), iVar1)) {
                if (mMotion == MOT_SQUAREUP_STEP) {
                    setMotion(MOT_DRIVEAWAY, -1.0f, 0);
                    field_0xe00 = cLib_getRndValue(1, 2);
                }

                mTurnMode = 0;
            }

            if (mMotion == MOT_SURPRISED && mMotionPhase > 0) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_DRIVEAWAY, -1.0f, 0);
                field_0xe00 = cLib_getRndValue(1, 2);
            } else if (mMotion == MOT_SQUAREUP &&  mMotionLoops >= field_0xe00) {
                setExpression(EXPR_ANM_3, -1.0f);
                setMotion(MOT_DRIVEAWAY, -1.0f, 0);
            } else if (mMotion == MOT_DRIVEAWAY && mMotionPhase > 0) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_SQUAREUP, -1.0f, 0);
                field_0xe00 = cLib_getRndValue(1, 2);
            }

            if (mMotion == MOT_DRIVEAWAY && mMotionPhase == 0 && (mAnm_p->checkFrame(11.0f) || mAnm_p->checkFrame(36.0f))) {
                mSound.startCreatureVoice(Z2SE_MOIR_V_SHIT_H, -1);
            }

            if (daPy_py_c::checkNowWolf()) {
                fVar1 = std::pow(700.0f, 2.0f);
                if (!(fopAcM_searchPlayerDistanceXZ2(this) > fVar1)) {
                    break;
                }
            }

            setAction(&daNpcMoiR_c::wait_type1);
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2030, FALSE);
            break;
    }

    return false;
}

bool daNpcMoiR_c::demo(void* param_1) {
    dEvent_manager_c* evt_mng = &dComIfGp_getEventManager();
    bool rv = false;

    switch (field_0xe08) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_C, -1.0f, 0);
            field_0xe08 = 2;
            // fallthrough
        case 2:
            if (dComIfGp_event_runCheck() && eventInfo.checkCommandTalk() == 0) {
                evt_mng = &dComIfGp_getEventManager();
                s32 staffIdx = evt_mng->getMyStaffId(l_myName, NULL, 0);
                if (staffIdx != -1) {
                    mStaffID = staffIdx;
                    int actIdx = evt_mng->getMyActIdx(staffIdx, l_evtNames, 4, 0, 0);
                    if (actIdx > 0 && actIdx < 4) {
                        mOrderEvtNo = actIdx;
                    }

                    JUT_ASSERT(2069, mEvtSeqList[mOrderEvtNo] != NULL);

                    if ((this->*mEvtSeqList[mOrderEvtNo])(staffIdx)) {
                        evt_mng->cutEnd(staffIdx);
                    }

                    rv = true;
                }

                if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && evt_mng->endCheck(mEventIdx)) {
                    dComIfGp_event_reset();
                    mOrderEvtNo = 0;
                    mEventIdx = -1;
                    setWaitAction();
                }
            } else {
                mOrderEvtNo = 0;
                mEventIdx = -1;
                setWaitAction();
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2101, FALSE);
            break;
    }

    return rv;
}

bool daNpcMoiR_c::leave(void* param_1) {
    cXyz sp24(-6253.387, 0.0f, 6251.7603f);
    daNPC_TK_c* actor_p = NULL;

    switch (field_0xe08) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_RUN, -1.0f, 0);
            mTurnMode = 0;
            speedF = 0.0f;
            fopAcM_SearchByName(PROC_NPC_TK, (fopAc_ac_c**)&actor_p);

            if (actor_p != NULL) {
                actor_p->setAway();
            }

            field_0xe08 = 2;
            break;

        case 2:
            step(cLib_targetAngleY(&current.pos, &sp24), 1);
            speedF = 12.0f;

            if (mAcch.ChkWallHit() || current.pos.abs(sp24) < speedF) {
                fopAcM_delete(this);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2143, FALSE);
            break;
    }

    return true;
}

BOOL daNpcMoiR_c::EvCut_Introduction(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* prm = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (prm == NULL) {
        return FALSE;
    }

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*prm) {
            case 0:
            case 2:
                setLookMode(LOOK_NONE);
                return TRUE;

            case 1:
                setLookMode(LOOK_PLAYER);
                return TRUE;

            default:
                JUT_ASSERT(2178, FALSE);
                break;
        }
    }

    return FALSE;
}

BOOL daNpcMoiR_c::EvCut_Appear(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* cutName = (int*)eventManager->getMyNowCutName(i_cutIndex);

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*cutName) {
            case '0001':
                break;

            case '0002':
            case '0003':
            case '0004':
            case '0005':
                initTalk(mMsgNo, NULL);
                mMsgTimer = 0;
                break;

            default:
                JUT_ASSERT(2211, FALSE);
                break;
        }
    }

    int i_expression, i_motion;
    int msgTimer = mMsgTimer;
    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
        setExpression(i_expression, -1.0f);
        setMotion(i_motion, -1.0f, 0);
    } else if (msgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    switch (*cutName) {
        case '0001':
            return TRUE;

        case '0002':
        case '0003':
        case '0004':
            if (talkProc(NULL, TRUE, NULL)) {
                return TRUE;
            }
            break;

        case '0005':
            if (fopAcM_searchPlayerAngleY(this) == mCurAngle.y) {
                if (talkProc(NULL, TRUE, NULL)) {
                    int choice_no = mFlow.getChoiceNo();
                    OS_REPORT("二択分岐 %s\n", choice_no == 0 ? "はい" : "いいえ");

                    if (choice_no == 0) {
                        mOrderEvtNo = 3;
                        changeEvent(l_arcNames[2], l_evtNames[mOrderEvtNo], 1, 0xFFFF);
                    }
                    return TRUE;
                }
            } else {
                if (step(fopAcM_searchPlayerAngleY(this), 1)) {
                    setMotion(MOT_WAIT_C, -1.0f, 0);
                    mTurnMode = 0;
                }
            }
            break;

        default:
            JUT_ASSERT(2259, FALSE);
            break;
    }

    return FALSE;
}

BOOL daNpcMoiR_c::EvCut_Appear2(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* cutName = (int*)eventManager->getMyNowCutName(i_cutIndex);

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*cutName) {
            case '0001':
            case '0005':
                setLookMode(LOOK_PLAYER_TALK);
                initTalk(mMsgNo, NULL);
                mMsgTimer = 0;
                break;

            case '0002':
                setLookMode(LOOK_NONE);
                break;

            case '0003':
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_B, 0.0f, 0);
                break;

            case '0004': {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_TAKAJO_NI, -1.0f, 0);
                mTurnMode = 0;
                ni_class* actor_p = (ni_class*)fopAcM_SearchByName(PROC_NI);

                if (actor_p != NULL) {
                    actor_p->changeMode();
                }

                field_0xdfc = 90;
                break;
            }

            default:
                JUT_ASSERT(2305, FALSE);
        }
    }

    int i_expression, i_motion;
    int msgTimer = mMsgTimer;
    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
        setExpression(i_expression, -1.0f);
        setMotion(i_motion, -1.0f, 0);
    } else if (msgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    switch (*cutName) {
        case '0001':
            if (fopAcM_searchPlayerAngleY(this) == mCurAngle.y) {
                if (talkProc(NULL, TRUE, NULL)) {
                    return TRUE;
                }
                break;
            }

            if (step(fopAcM_searchPlayerAngleY(this), 1)) {
                setMotion(MOT_WAIT_C, -1.0f, 0);
                mTurnMode = 0;
            }
            break;

        case '0002':
            if (step(0x1221, 1)) {
                setExpression(EXPR_F_HOLD_S, -1.0f);
                setMotion(MOT_FUE, -1.0f, 0);
            }

            if (mMotion != MOT_FUE) {
                break;
            }

            if (mMotionPhase == 0 && mAnm_p->checkFrame(25.0f)) {
                mDoAud_seStart(Z2SE_PLAY_REED_TOBIKUSA, NULL, 0, 0);
                break;
            }

            if (mMotionPhase > 0) {
                return TRUE;
            }
            break;

        case '0003':
            return TRUE;

        case '0004':
            if (cLib_calcTimer<int>((int*)&field_0xdfc) == 0) {
                return TRUE;
            }
            break;

        case '0005':
            if (talkProc(NULL, TRUE, NULL)) {
                ni_class* actor_p = (ni_class*)fopAcM_SearchByName(PROC_NI);

                if (actor_p != NULL) {
                    actor_p->changeMode();
                }

                return TRUE;
            }
            break;

        default:
            JUT_ASSERT(2363, FALSE);
            break;
    }

    return FALSE;
}

static int daNpcMoiR_Create(void* a_this) {
    return static_cast<daNpcMoiR_c*>(a_this)->Create();
}

static int daNpcMoiR_Delete(void* a_this) {
    return static_cast<daNpcMoiR_c*>(a_this)->Delete();
}

static int daNpcMoiR_Execute(void* a_this) {
    return static_cast<daNpcMoiR_c*>(a_this)->Execute();
}

static int daNpcMoiR_Draw(void* a_this) {
    return static_cast<daNpcMoiR_c*>(a_this)->Draw();
}

static int daNpcMoiR_IsDelete(void* a_this) {
    return 1;
}

void daNpcMoiR_c::setParam() {
    searchActors();

    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    if (mMode == MODE_SIT) {
        attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(3, 5);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(2, 5);
    } else {
        attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(mpHIO->m.common.attention_distance, mpHIO->m.common.attention_angle);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(mpHIO->m.common.talk_distance, mpHIO->m.common.talk_angle);

        if (mMode == MODE_STAND && daPy_py_c::checkNowWolf()) {
            attention_info.flags = 0;
        }
    }

    #if DEBUG
    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale, mpHIO->m.common.scale);
    mAcchCir.SetWallR(mpHIO->m.common.width);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    gravity = mpHIO->m.common.gravity;
    #endif
}

BOOL daNpcMoiR_c::main() {
    if (mAction) {
        (this->*mAction)(NULL);
    }

    if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk() && field_0xdf8 != -1) {
        dComIfGp_event_setItemPartnerId(field_0xdf8);
        field_0xdf8 = -1;
    }

    playExpression();
    playMotion();
    checkHeadGear();

    return TRUE;
}

void daNpcMoiR_c::checkHeadGear() {
    J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();
    J3DAnmTransform* anm = mAnm_p->getAnm();

    if (mMode == MODE_SIT) {
        if (anm == getTrnsfrmKeyAnmP(l_arcNames[1], l_bckGetParamList[30].fileIdx)) {
            if (mAnm_p->checkFrame(14.0f)) {
                mWearHelmet = false;
                mdlData_p->getMaterialNodePointer(MAT_MOIR_HEADGEAR_M)->getShape()->hide();
            }
        } else if (anm == getTrnsfrmKeyAnmP(l_arcNames[1], l_bckGetParamList[31].fileIdx)) {
            mWearHelmet = false;
            mdlData_p->getMaterialNodePointer(MAT_MOIR_HEADGEAR_M)->getShape()->hide();
        } else if (anm == getTrnsfrmKeyAnmP(l_arcNames[1], l_bckGetParamList[32].fileIdx)) {
            mWearHelmet = false;
            mdlData_p->getMaterialNodePointer(MAT_MOIR_HEADGEAR_M)->getShape()->hide();
        } else if (anm == getTrnsfrmKeyAnmP(l_arcNames[1], l_bckGetParamList[33].fileIdx) && mAnm_p->checkFrame(24.0f)) {
            mWearHelmet = true;
            mdlData_p->getMaterialNodePointer(MAT_MOIR_HEADGEAR_M)->getShape()->show();
        }
    } else if (mMode == MODE_STAND) {
        if (anm == getTrnsfrmKeyAnmP(l_arcNames[2], l_bckGetParamList[24].fileIdx)) {
            if (mAnm_p->checkFrame(15.0f)) {
                mWearHelmet = false;
                mdlData_p->getMaterialNodePointer(MAT_MOIR_HEADGEAR_M)->getShape()->hide();
            }
        } else if (anm == getTrnsfrmKeyAnmP(l_arcNames[2], l_bckGetParamList[25].fileIdx)) {
            mWearHelmet = false;
            mdlData_p->getMaterialNodePointer(MAT_MOIR_HEADGEAR_M)->getShape()->hide();
        } else if (anm == getTrnsfrmKeyAnmP(l_arcNames[2], l_bckGetParamList[26].fileIdx)) {
            mWearHelmet = false;
            mdlData_p->getMaterialNodePointer(MAT_MOIR_HEADGEAR_M)->getShape()->hide();
        } else if (anm == getTrnsfrmKeyAnmP(l_arcNames[2], l_bckGetParamList[27].fileIdx) && mAnm_p->checkFrame(33.0f)) {
            mWearHelmet = true;
            mdlData_p->getMaterialNodePointer(MAT_MOIR_HEADGEAR_M)->getShape()->show();
        }
    }
}

static void dummy() {
    // This dummy function is needed to fix JMath::TSinCosTable::sinShort not being inlined in daNpcMoiR_c::ctrlBtk.
    // It's unclear why that happens, or why an unused dummy function calling the inline fixes it.
    // Seems like a compiler bug of some sort.
    cM_ssin(0);
}

BOOL daNpcMoiR_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* anm = NULL;
        anm = getTexSRTKeyAnmP(l_arcNames[l_btkGetParamList[0].arcIdx], l_btkGetParamList[0].fileIdx);

        if (anm == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.2f * -1.0f);
            mpMatAnm->setNowOffsetY(cM_ssin(mEyeAngle.x) * 0.2f);
            mpMatAnm->onEyeMoveFlag();
            return TRUE;
        }

        mpMatAnm->offEyeMoveFlag();
    }

    return FALSE;
}

void daNpcMoiR_c::setAttnPos() {
    if (mLookMode == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();

    cXyz sp40;
    if (mMode == MODE_SIT) {
        sp40.set(-10.0f, -15.0f, -0.15f);
    } else {
        sp40.set(-15.0f, 10.0f, 2.0f);
    }

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&sp40, &eyePos);
    sp40.x = 0.0f;
    mDoMtx_stack_c::multVec(&sp40, &sp40);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp40);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp40);

    cXyz* attnPos = mLookat.getAttnPos();
    if (attnPos != NULL) {
        cXyz sp4c = *attnPos - eyePos;
        mEyeAngle.y = (-mLookatAngle[2].y) - mCurAngle.y;
        mEyeAngle.y += cM_atan2s(sp4c.x, sp4c.z);
        mEyeAngle.x = mHeadAngle.x - cM_atan2s(sp4c.y, sp4c.absXZ());

        if (mEyeAngle.y > mpHIO->m.horizontal_eye_movement_range) {
            mEyeAngle.y = mpHIO->m.horizontal_eye_movement_range;
        } else if (mEyeAngle.y < -mpHIO->m.horizontal_eye_movement_range) {
            mEyeAngle.y = -mpHIO->m.horizontal_eye_movement_range;
        }
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    attention_info.position.set(mHeadPos.x, mHeadPos.y + mpHIO->m.common.attention_offset, mHeadPos.z);
    cXyz sp58;
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_BACKBONE2));
    mDoMtx_stack_c::multVecZero(&sp58);
    sp58.y = current.pos.y;
    field_0xca0.SetC(sp58);
    field_0xca0.SetH(mpHIO->m.common.height);
    
    f32 fVar1;
    if (mMode == MODE_SIT) {
        fVar1 = 40.0f;
    } else {
        fVar1 = mpHIO->m.common.width;
    }

    field_0xca0.SetR(fVar1);
    dComIfG_Ccsp()->Set(&field_0xca0);
}

void daNpcMoiR_c::drawOtherMdls() {
    if (!mWearHelmet) {
        g_env_light.setLightTevColorType_MAJI(mpHeadgearModel, &tevStr);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HANDR));
        mpHeadgearModel->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoExt_modelUpdateDL(mpHeadgearModel);
    }
}

BOOL daNpcMoiR_c::drawDbgInfo() {
    return FALSE;
}

static actor_method_class daNpcMoiR_MethodTable = {
    (process_method_func)daNpcMoiR_Create,
    (process_method_func)daNpcMoiR_Delete,
    (process_method_func)daNpcMoiR_Execute,
    (process_method_func)daNpcMoiR_IsDelete,
    (process_method_func)daNpcMoiR_Draw,
};

actor_process_profile_definition g_profile_NPC_MOIR = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_MOIR,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpcMoiR_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  415,                    // mPriority
  &daNpcMoiR_MethodTable, // sub_method
  0x00040100,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
