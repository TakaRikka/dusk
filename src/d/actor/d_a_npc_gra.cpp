/**
 * @file d_a_npc_gra.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/d_debug_viewer.h"
#include "d/d_meter2_info.h"
#include "d/actor/d_a_obj_onsenTaru.h"
#include "JSystem/JHostIO/JORFile.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc_gra.h"
#include "d/actor/d_a_tag_gra.h"
#include "Z2AudioLib/Z2Instances.h"

const daNpc_grA_HIOParam daNpc_grA_Param_c::m = {
    {90.0f,  -4.0f,  1.0f,   850.0f,  255.0f, 280.0f, 40.0f, 100.0f, 0.0f, 0.0f, 20.0f,
     -20.0f, 40.0f,  -30.0f, 40.0f,   -40.0f, 0.4f,   12.0f, 4,      6,    6,    6,
     60.0f,  500.0f, 300.0f, -300.0f, 0x3c,   8,      0,     0,      0,    0,    0},
    15,
    6.0f,
    3.0f,
    0.17f,
    0x300,
    4,
    1.35f,
};

#if DEBUG
#define GET_HIO(x) mpHio->mHioParams.x
#else
#define GET_HIO(x) daNpc_grA_Param_c::m.x
#endif

static int l_bmdGetParamList[][2] = {
    3, 1,
    3, 2,
    3, 12,
    13, 14,
    11, 15,
};

static int l_bckGetParamList[][2] = {
    0x0E, 0x00,
    0x0C, 0x00,
    0x0D, 0x00,
    0x0B, 0x00,
    0x07, 0x00,
    0x08, 0x00,
    0x09, 0x00,
    0x0A, 0x00,
    0x07, 0x03,
    0x05, 0x03,
    0x08, 0x03,
    0x06, 0x03,
    0x08, 0x0A,
    0x04, 0x0B,
    0x06, 0x00,
    0x07, 0x0A,
    0x06, 0x0A,
    0x07, 0x0E,
    0x06, 0x0E,
    0x08, 0x0E,
    0x06, 0x0F,
    0x05, 0x00,
    0x15, 0x00,
    0x04, 0x04,
    0x16, 0x00,
    0x0F, 0x00,
    0x10, 0x00,
    0x14, 0x00,
    0x11, 0x00,
    0x12, 0x00,
    0x13, 0x00,
    0x03, 0x04,
    0x05, 0x04,
    0x06, 0x04,
    0x0B, 0x03,
    0x09, 0x03,
    0x0C, 0x03,
    0x0A, 0x03,
    0x03, 0x07,
    0x05, 0x0A,
    0x09, 0x0A,
    0x05, 0x0B,
    0x0A, 0x0A,
    0x0B, 0x0A,
    0x04, 0x0A,
    0x09, 0x0E,
    0x05, 0x0E,
    0x0A, 0x0E,
    0x08, 0x0F,
    0x05, 0x0F,
    0x07, 0x0F,
};

static int l_btpGetParamList[][2] = {
    0x1C, 0x00,
    0x1F, 0x00,
    0x20, 0x00,
    0x11, 0x03,
    0x0F, 0x03,
    0x10, 0x03,
    0x10, 0x0a,
    0x08, 0x0B,
    0x1E, 0x00,
    0x0F, 0x0A,
    0x0E, 0x0A,
    0x11, 0x0E,
    0x10, 0x0E,
    0x12, 0x0E,
    0x0E, 0x0F,
    0x1D, 0x00,
};

static int l_btkGetParamList[][2] = {-1, 0};

static int l_evtGetParamList[][2] = {
    0x00, 0x00,
    0x01, 0x03,
    0x02, 0x05,
    0x03, 0x06,
    0x04, 0x06,
    0x05, 0x06,
    0x06, 0x08,
    0x07, 0x09,
    0x08, 0x09,
    0x09, 0x09,
    0x0A, 0x0D,
};

#if DEBUG
daNpc_grA_HIO_c::daNpc_grA_HIO_c() {
    mHioParams = daNpc_grA_Param_c::m;
}
#endif

NPC_GRA_HIO_CLASS l_HIO;

static char* l_evtNames[11] = {
    NULL,
    "TALK_SPA",
    "TEACH_ELV",
    "KICK_OUT",
    "KICK_OUT_BREAK",
    "NONE_LOOK",
    "RESCUE_ROCK",
    "CARRY_SPA_WATER",
    "CARRY_SPA_WATER_FAILURE",
    "TALK_SPA_WATER",
    "ROLL_ROCK_CRASH",
};

static int l_loadRes_GRAa[] = {
    0, 1, -1, -1, -1, -1, -1, -1,
};

static int l_loadRes_GRA_Tw[] = {
    0, 2, -1, -1, -1, -1, -1, -1,
};

static int l_loadRes_GRA_Spa[] = {
    0, 1, 3, -1, -1, -1, -1, -1,
};

static int l_loadRes_GRA_SDEMO[] = {
    0, 1, 4, -1, -1, -1, -1, -1,
};

static int l_loadRes_GRA_GATE[] = {
    0, 1, 4, 5, -1, -1, -1, -1,
};

static int l_loadRes_GRA_KICKOUT[] = {
    0, 1, 6, 10, -1, -1, -1, -1,
};

static int l_loadRes_GRA_ROCK[] = {
    0, 1, 7, 8, -1, -1, -1, -1,
};

static int l_loadRes_GRA_SPAWATER[] = {
    0, 1, 3, 9, 10, 11, 12, -1,
};

static int l_loadRes_GRA_SPABUYER[] = {
    0, 1, 15, -1, -1, -1, -1, -1,
};

static int l_loadRes_GRA_SPABUYER_TW[] = {
    0, 2, 15, -1, -1, -1, -1, -1,
};

static int l_loadRes_GRA_ROCKCRASHER[] = {
    0, 1, 13, 11, 14, -1, -1, -1,
};

static int l_loadRes_GRA_BUYER[] = {
    0, 1, 4, -1, -1, -1, -1, -1,
};

static int l_loadRes_GRA0[] = {
    0, 1, -1, -1, -1, -1, -1, -1,
};

static int* l_loadRes_list[13] = {
    l_loadRes_GRAa,
    l_loadRes_GRA_Tw,
    l_loadRes_GRA_Spa,
    l_loadRes_GRA_SDEMO,
    l_loadRes_GRA_GATE,
    l_loadRes_GRA_KICKOUT,
    l_loadRes_GRA_ROCK,
    l_loadRes_GRA_SPAWATER,
    l_loadRes_GRA_SPABUYER,
    l_loadRes_GRA_SPABUYER_TW,
    l_loadRes_GRA_ROCKCRASHER,
    l_loadRes_GRA_BUYER,
    l_loadRes_GRA0,
};

static char* l_resNames[16] = {
    "grA_base",
    "grA_mdl",
    "grA_TW",
    "grA_SPA",
    "grA_Sdemo",
    "grA_Elv",
    "grA_Kick",
    "grA_Rock",
    "grA_RockD",
    "grA_SWD",
    "grA_onsen",
    "grA_onsn2",
    "maroTaru",
    "grA_RCD",
    "grA_gate",
    "grA_town",
};

static char* l_myName[3] = {
    "grA",
    "grA_spa",
    "grA_Elv",
};


#if DEBUG
void daNpc_grA_HIO_c::listenPropertyEvent(const JORPropertyEvent* i_event) {
    JORReflexible::listenPropertyEvent(i_event);
    int len;
    JORFile file;
    char buff[2000];
    switch ((uintptr_t)i_event->id) {
    case 0x40000002:
        if (file.open(6, "すべてのファイル(*.*)\0*.*\0", NULL, NULL, NULL)) {
            memset(buff, 0, sizeof(buff));
            len = 0;
            daNpcF_commonListenPropertyEvent(buff, &len, &this->mHioParams.mNpcFParams);
            sprintf(buff + len, "%d,   \t//  お辞儀タイマー\n", this->mHioParams.mBowTimer);
            len = strlen(buff);
            sprintf(buff + len, "%3.3f,   \t//  回転移動速度係数\n", this->mHioParams.mRotationalSpeed);
            len = strlen(buff);
            sprintf(buff + len, "%3.3f,   \t//  歩き移動速度\n", this->mHioParams.mWalkingSpeed);
            len = strlen(buff);
            sprintf(buff + len, "%3.3f,   \t//  歩きアニメ速度\n", this->mHioParams.mWalkingAnimationSpeed);
            len = strlen(buff);
            sprintf(buff + len, "%d,   \t//  歩行回転速度\n", this->mHioParams.mWalkingRotationSpeed);
            len = strlen(buff);
            sprintf(buff + len, "%d,   \t//  歩行回転分割数\n", this->mHioParams.mWalkingRotationDivisions);
            len = strlen(buff);
            sprintf(buff + len, "%3.3f,   \t//  パーティクルサイズ\n", this->mHioParams.mParticleSize);
            // @BUG: should update len here, otherwise the final length won't include the final sprintf
            file.writeData(buff, len);
            file.close();
            OS_REPORT("write append success!::%6d\n", len);
        } else {
            OS_REPORT("write append failure!\n");
        }
    }
}

void daNpc_grA_HIO_c::genMessage(JORMContext* context) {
    daNpcF_commonGenMessage(context, &mHioParams.mNpcFParams);
    context->genSlider("お辞儀タイマー  ", &mHioParams.mBowTimer, 0, 1000, 0, NULL, -1, -1, 0x200, 0x18);
    context->genSlider("回転移動速度係数", &mHioParams.mRotationalSpeed, 0.0f, 100.0f, 0, NULL, -1, -1, 0x200, 0x18);
    context->genSlider("歩き移動速度    ", &mHioParams.mWalkingSpeed, 0.0f, 100.0f, 0, NULL, -1, -1, 0x200, 0x18);
    context->genSlider("歩きアニメ速度  ", &mHioParams.mWalkingAnimationSpeed, 0.0f, 10.0f, 0, NULL, -1, -1, 0x200, 0x18);
    context->genSlider("歩行回転角度    ", &mHioParams.mWalkingRotationSpeed, 0, 0x7fff, 0, NULL, -1, -1, 0x200, 0x18);
    context->genSlider("歩行回転分割数  ", &mHioParams.mWalkingRotationDivisions, 0, 0x100, 0, NULL, -1, -1, 0x200, 0x18);
    context->genSlider("エフェクトサイズ", &mHioParams.mParticleSize, 0.0f, 10.0f, 0, NULL, -1, -1, 0x200, 0x18);
    context->genButton("ファイル書き出し", 0x40000002, 0, NULL, -1, -1, 0x200, 0x18);
}
#endif

char* daNpc_grA_c::mEvtCutNameList[12] = {
    "",
    "TALK_SPA",
    "GRDS_ROLL",
    "GRDS_GATE",
    "TEACH_ELV",
    "KICK_OUT",
    "NONE_LOOK",
    "RESCUE_ROCK",
    "CARRY_SPA_WATER",
    "CARRY_SPA_WATER_FAILURE",
    "TALK_SPA_WATER",
    "ROLL_ROCK_CRASH",
};

daNpc_grA_c::cut_type daNpc_grA_c::mEvtCutList[] = {
    NULL, 
    &daNpc_grA_c::ECut_talkSpa,
    &daNpc_grA_c::ECut_grDSRoll,
    &daNpc_grA_c::ECut_grDSGate,
    &daNpc_grA_c::ECut_teachElevator,
    &daNpc_grA_c::ECut_kickOut,
    &daNpc_grA_c::ECut_noneLook,
    &daNpc_grA_c::ECut_rescueRock,
    &daNpc_grA_c::ECut_carrySpaWater,
    &daNpc_grA_c::ECut_carrySpaWaterFailure,
    &daNpc_grA_c::ECut_talkSpaWater,
    &daNpc_grA_c::ECut_rollRockCrash,
};

daNpc_grA_c::daNpc_grA_c() : daNpcF_c() {
}

daNpc_grA_c::~daNpc_grA_c() {
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        dComIfG_resDelete(&mPhase[i], l_resNames[l_loadRes_list[mType][i]]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }

#if DEBUG
    if (mpHio != NULL) {
        mpHio->removeHIO();
    }
#endif
}

BOOL daNpc_grA_c::create() {
    fopAcM_ct(this, daNpc_grA_c);
    mTwilight = dKy_darkworld_check();
    mType = getTypeFromParam();
    mMode = getMode();
    mSwBit = getSwBit();
    if (home.angle.x != 0xffff) {
        field_0x146C = home.angle.x;
    } else{
        field_0x146C = -1;
    }
    if (isDelete()) {
        return cPhs_ERROR_e;
    }
    int phase;
    int i = 0, j = 0;
    J3DModelData* unused;
    int unused2 = j;

    for (; l_loadRes_list[mType][j] >= 0; j++) {
        phase = dComIfG_resLoad(&mPhase[j], l_resNames[l_loadRes_list[mType][j]]);
        if (phase == cPhs_ERROR_e || phase == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }
        if (phase == cPhs_COMPLEATE_e) {
            i++;
        }
    }
    if (i == j) {
        if (!fopAcM_entrySolidHeap(this, &daNpc_grA_c::createHeapCallBack, 0x800045d0)) {
            return cPhs_ERROR_e;
        }
        unused = mAnm_p->getModel()->getModelData();

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);
        mCreature.init(&current.pos, &eyePos, 3, 1);

#if DEBUG
        mpHio = &l_HIO;
        mpHio->entryHIO("ゴロン一般");
#endif

        f32 v;
        if (mType == 2) {
            v = 120.0f;
        } else {
            v = GET_HIO(mNpcFParams.width);
        }
        mAcchCir.SetWall(v, GET_HIO(mNpcFParams.knee_length));

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(GET_HIO(mNpcFParams.weight), 0, this);

        field_0xC98.Set(mCcDCyl);
        field_0xC98.SetStts(&mCcStts);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        setEnvTevColor();
        setRoomNo();
        reset();
        Execute();
        return cPhs_COMPLEATE_e;
    }
    return cPhs_INIT_e;
}

BOOL daNpc_grA_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;
    J3DModel* mdlData2_p = NULL;
    if (mTwilight) {
        if (l_bmdGetParamList[1][0] >= 0) {
            mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[1][1]],
                                                            l_bmdGetParamList[1][0]);
        }
    } else if (l_bmdGetParamList[0][0] >= 0) {
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[0][1]],
                                                        l_bmdGetParamList[0][0]);
    }
    JUT_ASSERT(774, NULL != mdlData_p);
    int v = 0x11020284;
    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mCreature, 0x80000, v);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }
    if (mAnm_p == NULL) {
        return FALSE;
    }
    mdlData2_p = mAnm_p->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    mdlData2_p->setUserArea((uintptr_t)this);
    int bmdIndex = -1;
    int modelFlag = 0x80000;
    switch (mType) {
    case 7:
        bmdIndex = 2;
        break;
    case 8:
    case 9:
        bmdIndex = 4;
        break;
    case 10:
        bmdIndex = 3;
        modelFlag = 0;
        break;
    }

    if (bmdIndex > 0) {
        J3DModelData* m = NULL;
        m = (J3DModelData*)dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[bmdIndex][1]],
                                                l_bmdGetParamList[bmdIndex][0]);
        if (m != NULL) {
            mpModel = mDoExt_J3DModel__create(m, modelFlag, 0x11000084);
            if (mpModel == NULL) {
                return FALSE;
            }
        } else {
            return FALSE;
        }
    } else {
        mpModel = NULL;
    }
    if (mType == 2) {
        if (getMode() == 1) {
            if (!setExpressionAnm(10, false)) {
                return false;
            }
            setMotionAnm(0x24, 0.0f);
        } else {
            if (!setExpressionAnm(8, false)) {
                return false;
            }
            setMotionAnm(0x22, 0.0f);
        }
    } else {
        if (!setExpressionAnm(0, false)) {
            return false;
        }
        setMotionAnm(0x16, 0.0f);
    }
    return TRUE;
}

BOOL daNpc_grA_c::Delete() {
    fopAcM_RegisterDeleteID(this, "NPC_GRA");
    this->~daNpc_grA_c();
    return TRUE;
}

BOOL daNpc_grA_c::Execute() {
    int ret = execute();
    setPrtcl();
    return ret;
}

BOOL daNpc_grA_c::Draw() {
    return draw(chkAction(&daNpc_grA_c::test), 0, GET_HIO(mNpcFParams.real_shadow_size), NULL, 0);
}

BOOL daNpc_grA_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* joint = i_joint;
    int jntNo = joint->getJntNo();
    int arr[] = {1, 3, 4};
    if (jntNo == 0) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));

    switch (jntNo) {
    case 1:
    case 3:
    case 4:
        setLookatMtx(jntNo, arr, GET_HIO(mNpcFParams.neck_rotation_ratio));
    }

    if (jntNo == 1) {
        mDoMtx_stack_c::YrotM(-field_0x908[0].z);
        mDoMtx_stack_c::ZrotM(field_0x908[0].x);
    } else if (jntNo == 4) {
        mDoMtx_stack_c::YrotM(-field_0x908[2].z);
        mDoMtx_stack_c::ZrotM(field_0x908[2].x);
    }

    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if (jntNo == 4 || jntNo == 7) {
        if ((mAnmFlags & 0x100) != 0) {
            J3DAnmTransform* a = mBckAnm.getBckAnm();
            mBckAnm.changeBckOnly(mAnm_p->getAnm());
            mAnm_p->changeAnm(a);
        }
    }
    return TRUE;
}

BOOL daNpc_grA_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpc_grA_c* this_gra = (daNpc_grA_c*)i_this;
    return this_gra->CreateHeap();
}

BOOL daNpc_grA_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0)
    {
        J3DModel* model = j3dSys.getModel();
        daNpc_grA_c* area = (daNpc_grA_c*)model->getUserArea();
        if (area != NULL)
        {
            area->ctrlJoint(i_joint, model);
        }
    }
    return TRUE;
}

BOOL daNpc_grA_c::setTagJump() {
    if (field_0x1489) {
        current.pos = old.pos = home.pos = field_0x148C;
        shape_angle = mCurAngle = mOldAngle = current.angle = old.angle = home.angle = field_0x1498;
        field_0x14D0 = 0;
        return TRUE;
    }
    return FALSE;
}

BOOL daNpc_grA_c::setTagJump2() {
    if (field_0x149E) {
        current.pos = old.pos = home.pos = field_0x14A0;
        shape_angle = mCurAngle = mOldAngle = current.angle = old.angle = home.angle = field_0x14AC;
        field_0x14D0 = 0;
        return TRUE;
    }
    return FALSE;
}

BOOL daNpc_grA_c::setHomeJump() {
    current.pos = old.pos = home.pos = field_0x14BC;
    shape_angle = mCurAngle = mOldAngle = current.angle = old.angle = home.angle = field_0x14C8;
    field_0x14D0 = 0;
    field_0x14D1 = 0;
    return TRUE;
}

static void* s_sub(void* i_actor, void* i_this) {
    // a bit ugly, but the debug version can't have any local variables
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_TAG_GRA) {
        if (((daNpc_grA_c*)i_this)->checkTagGraSub((fopAc_ac_c*)i_actor)) {
            return i_actor;
        }
    }
    return NULL;
}

bool daNpc_grA_c::checkTagGraSub(fopAc_ac_c* i_this) {
    daTagGra_c* this_gra = (daTagGra_c*)i_this;
    if ((mType == 3 && this_gra->getMode() == 3) || (mType == 4 && this_gra->getMode() == 4)) {
        if (this_gra->getGraNo() == mMode)
        {
            return TRUE;
        }
    }
    return FALSE;
}

static void* s_sub2(void* i_actor, void* i_this) {
    // a bit ugly, but the debug version can't have any local variables
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_TAG_GRA) {
        if (((daNpc_grA_c*)i_this)->checkTagGraSub2((fopAc_ac_c*)i_actor)) {
            return i_actor;
        }
    }
    return NULL;
}

bool daNpc_grA_c::checkTagGraSub2(fopAc_ac_c* i_this) {
    daTagGra_c* this_gra = (daTagGra_c*)i_this;
    if ((mType == 3 && this_gra->getMode() == 5)) {
        if (this_gra->getGraNo() == mMode)
        {
            return TRUE;
        }
    }
    return FALSE;
}

static void* s_subShop(void* i_actor, void* i_this) {
    UNUSED(i_this);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_GRM) {
        return i_actor;
    }
    return NULL;
}

static void* s_subGRD(void* i_actor, void* i_this) {
    UNUSED(i_this);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_WRESTLER) {
        return i_actor;
    }
    return NULL;
}

static void *s_subOnsenTaru(void* i_actor, void* i_this) {
    UNUSED(i_this);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_OnsenTaru &&
        !fpcM_IsCreating(fopAcM_GetID((i_actor))))
    {
        return i_actor;
    }
    return NULL;
}

static void *s_subCarry(void* i_actor, void* i_this) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_Carry) {
        ((daNpc_grA_c*)i_this)->addCarryNum();
    }
    return NULL;
}

static void *s_subCrashed(void* i_actor, void* i_this) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_TAG_GRA &&
        ((daNpc_grA_c*)i_this)->checkTagGraSubCrashed((fopAc_ac_c*)i_actor))
    {
        return i_actor;
    }
    return NULL;
}

bool daNpc_grA_c::checkTagGraSubCrashed(fopAc_ac_c* i_this) {
    daTagGra_c* this_gra = (daTagGra_c*)i_this;
    if (mType == 10 && this_gra->getMode() == 6) {
        return TRUE;
    }
    return FALSE;
}

void daNpc_grA_c::setParam() {
    daNpc_grA_c_Action act = mAction2;
    s16 attnDistance, attnAngle, talkDistance, talkAngle;
    int flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    selectAction();

    if (!mTwilight && daPy_py_c::checkNowWolf()) {
        flags = 0;
    }

    if (mAction2 != act) {
        for (int i = 3; i < 3; i++) {  // matches debug, no loop?
            mNpcfActorManager[i].initialize();
        }
    }
    if (mTwilight) {
        field_0x145C = 0;
        field_0x1460 = 0;
        if (mType == 9) {
            talkDistance = 0xb;
            talkAngle = 0x4;
            attnDistance = 0xb;
            attnAngle = 0x4;
        } else {
            talkDistance = 0xf;
            talkAngle = 0x6;
            attnDistance = 0xf;
            attnAngle = 0x6;
        }
    } else {
        field_0x145C = 0;
        field_0x1460 = 0;
        talkDistance = GET_HIO(mNpcFParams.talk_distance);
        talkAngle = GET_HIO(mNpcFParams.talk_angle);
        attnDistance = GET_HIO(mNpcFParams.attention_distance);
        attnAngle = GET_HIO(mNpcFParams.attention_angle);
    }

    if (mType == 5)
    {
        attention_info.distances[fopAc_attn_LOCK_e] = 0x4e;
        attention_info.distances[fopAc_attn_TALK_e] = 0x4e;
        attention_info.distances[fopAc_attn_SPEAK_e] = 0x4d;
        attention_info.flags = fopAc_AttnFlag_UNK_0x800000 | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    } else {
        if (chkAction(&daNpc_grA_c::waitSpaWater)) {
            talkDistance = 0xa;
            attnDistance = 0xc;
        }
        attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(attnDistance, attnAngle);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(talkDistance, talkAngle);
        if (field_0x14D8 == 0x1a) {
            attention_info.flags = 0;
        }else{
            attention_info.flags = flags;
        }
    }
    scale.set(GET_HIO(mNpcFParams.scale), GET_HIO(mNpcFParams.scale), GET_HIO(mNpcFParams.scale));
    f32 width;
    if (mType == 2) {
        width = 120.0f;
    } else {
        width = GET_HIO(mNpcFParams.width);
    }
    mAcchCir.SetWallR(width);
    mAcchCir.SetWallH(GET_HIO(mNpcFParams.knee_length));
    if (mType != 6) {
        gravity = GET_HIO(mNpcFParams.gravity);
    }
}

BOOL daNpc_grA_c::main() {
    int a = 0xffff;
    JUT_ASSERT(1367, mAnm_p != NULL); // TODO: update "mAnm_p" to "mAnm_p"
    if (doEvent() == 0) {
        if (hitChk2(&field_0xC98, TRUE, FALSE)) {
            setDamage(GET_HIO(mNpcFParams.damage_time), 0x17, 0);
            setLookMode(1);
        } else if (mIsDamaged && mDamageTimer == 0) {
            field_0x1472 = 0;
            mIsDamaged = 0;
        }
        mOrderEvtNo = 0;
        if (mAction2) {
            if (mAction == mAction2) {
                (this->*mAction)(0);
            } else {
                setAction(mAction2);
            }
        }
    }

    if (checkHide()) {
        attention_info.flags = 0;
    }
    if (GET_HIO(mNpcFParams.debug_mode_ON) == 0 &&
        (!dComIfGp_event_runCheck() || (mOrderNewEvt && dComIfGp_getEvent()->isOrderOK())))
    {
        u16 j = 1;
        if (mOrderEvtNo != 0) {
            switch (mOrderEvtNo) {
            case 3:
            case 4:
            case 5:
                j = 3;
            }
            eventInfo.setArchiveName(l_resNames[l_evtGetParamList[mOrderEvtNo][1]]);
        }
        orderEvent(field_0x1691, l_evtNames[l_evtGetParamList[mOrderEvtNo][0]], a, 4, 0xff, j);
    }
    if (field_0x9ee != 0) {
        mExpressionMorfOverride = 0.0f;
        mMotionMorfOverride = 0.0f;
        field_0x9ee = 0;
    }
    playExpression();
    playMotion();
    JUT_ASSERT(1487, mAnm_p != NULL);
    return TRUE;
}

BOOL daNpc_grA_c::ctrlBtk() {
    return false;
}

void daNpc_grA_c::setAttnPos() {
    static cXyz eyeOffset(15.0f, 25.0f, 0.0f);
    cXyz* attentionPos = NULL;
    int unused = 0;
    cXyz viewVector, offset, o_pos;
    f32 attnOffset = 0.0f;
    if (mOrderEvtNo != 1) {
        attnOffset = GET_HIO(mNpcFParams.attention_offset);
    }
    mDoMtx_stack_c::YrotS(field_0x990);
    cLib_addCalc2(&field_0x984[0], 0.0f, 0.1f, 125.0f);
    cLib_addCalc2(&field_0x984[2], 0.0f, 0.1f, 125.0f);
    for (int i = 0; i < 3; i++) {
        offset.set(0.0f, 0.0f, cM_ssin(field_0x992) * field_0x984[i]);
        mDoMtx_stack_c::multVec(&offset, &o_pos);
        field_0x908[i].x = -o_pos.z;
        field_0x908[i].z = -o_pos.x;
    }
    cLib_chaseS(&field_0x992, 0, 0x555);
    if (mLookMode == 1) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }
    J3DModelData* modelData = mAnm_p->getModel()->getModelData();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(&daNpc_grA_c::ctrlJointCallBack);
    }
    setMtx();
    setOtherObjMtx();
    lookat();
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&eyeOffset, &eyePos);
    mDoMtx_stack_c::multVec(&eyeOffset, &offset);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &offset);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &offset);
    
    attentionPos = mNpcfLookAt.getAttnPos();
    if (attentionPos != NULL) {
        viewVector = *attentionPos - eyePos;
        mEyeAngle.y = -mLookatAngle[2].y - mCurAngle.y;
        mEyeAngle.y += cM_atan2s(viewVector.x, viewVector.z);
        mEyeAngle.x = -cM_atan2s(viewVector.y, viewVector.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }
    
    if (chkAction(&daNpc_grA_c::moveRock) || field_0x14D8 == 0x1a) {
        attention_info.position.set(current.pos.x, current.pos.y + 300.0f, current.pos.z);
    } else {
        attention_info.position.set(mHeadPos.x, mHeadPos.y + attnOffset, mHeadPos.z);
    }

    if (!mHide) {
        if (mDamageTimer == 0) {
            field_0xC98.SetTgType(0xd8fbfdff);
            field_0xC98.SetTgSPrm(0x1f);
            field_0xC98.OnTgNoHitMark();
            if (mType == 7) {
                field_0xC98.OnTgNoConHit();
                field_0xC98.OffTgShield();
            }
        } else {
            field_0xC98.SetTgType(0);
            field_0xC98.SetTgSPrm(0);
        }
        if (mType == 10) {
            if (field_0x14D8 == 0x1a) {
                field_0xC98.OnAtSPrmBit(0xc);
                field_0xC98.SetAtSe(dCcD_SE_STONE);
                field_0xC98.SetAtSpl(dCcG_At_Spl_UNK_D);
                field_0xC98.SetAtType(AT_TYPE_CSTATUE_SWING);
                field_0xC98.OnAtSetBit();
            } else {
                field_0xC98.OffAtSetBit();
            }
        }
        f32 width = 0.0f;
        f32 height = GET_HIO(mNpcFParams.height);
        if (mType == 2) {
            width = 120.0f;
            height = 215.0f;
        } else {
            width = GET_HIO(mNpcFParams.width);
        }
        if (chkAction(&daNpc_grA_c::waitSpaWater)) {
            cXyz center(0.0f, 0.0f, -80.0f);
            mDoMtx_stack_c::YrotS(shape_angle.y);
            mDoMtx_stack_c::multVec(&center, &center);
            center += current.pos;
            field_0xC98.SetC(center);
            width = 150.0f;
            height = 150.0f;
        } else {
            field_0xC98.SetC(current.pos);
        }
        field_0xC98.SetCoSPrm(0x79);
        field_0xC98.SetH(height);
        field_0xC98.SetR(width);
        dComIfG_Ccsp()->Set(&field_0xC98);
    }
    field_0xC98.ClrAtHit();
    field_0xC98.ClrTgHit();
    field_0xC98.ClrCoHit();
}

bool daNpc_grA_c::setExpressionAnm(int i_expression, bool i_modify) {
    J3DAnmTransformKey *bck = NULL;
    int attr = 0;
    mAnmFlags &= ~0x1fc0;
    if (l_bckGetParamList[i_expression][0] >= 0) {
        bck = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_expression][1]],
                                l_bckGetParamList[i_expression][0]);
    }
    bool ret = 0;
    switch (i_expression) {
    case 0x00:
        ret = setExpressionBtp(0x00);
        break;
    case 0x01:
        ret = setExpressionBtp(0x00);
        break;
    case 0x02:
        ret = setExpressionBtp(0x00);
        break;
    case 0x03:
        ret = setExpressionBtp(0x00);
        break;
    case 0x04:
        attr = 2;
        ret = setExpressionBtp(0x00);
        break;
    case 0x05:
        ret = setExpressionBtp(0x01);
        break;
    case 0x06:
        attr = 2;
        ret = setExpressionBtp(0x00);
        break;
    case 0x07:
        ret = setExpressionBtp(0x02);
        break;
    case 0x08:
        attr = 2;
        ret = setExpressionBtp(0x03);
        break;
    case 0x09:
        ret = setExpressionBtp(0x04);
        break;
    case 0x0a:
        attr = 2;
        ret = setExpressionBtp(0x00);
        break;
    case 0x0b:
        ret = setExpressionBtp(0x05);
        break;
    case 0x0c:
        attr = 2;
        ret = setExpressionBtp(0x06);
        break;
    case 0x0d:
        ret = setExpressionBtp(0x07);
        break;
    case 0x0e:
        attr = 2;
        ret = setExpressionBtp(0x08);
        break;
    case 0x0f:
        ret = setExpressionBtp(0x09);
        break;
    case 0x10:
        ret = setExpressionBtp(0x0a);
        break;
    case 0x11:
        attr = 2;
        ret = setExpressionBtp(0x0b);
        break;
    case 0x12:
        ret = setExpressionBtp(0x0c);
        break;
    case 0x13:
        ret = setExpressionBtp(0x0d);
        break;
    case 0x15:
        ret = setExpressionBtp(0x0F);
        break;
    case 0x14:
        ret = setExpressionBtp(0x0E);
        break;
    default:
        bck = NULL;
    }
    if (!ret) {
        return false;
    }
    if (bck == NULL) {
        return true;
    }
    if (setBckAnm(bck, 1.0f, attr, 0, -1, i_modify)) {
        mAnmFlags |= 0x140;
        mExpressionLoops = 0;
        return true;
    }
    OS_REPORT("%s: 表情Bckアニメーションの登録に失敗しました！\n", __FILE__);
    return false;
}

bool daNpc_grA_c::setExpressionBtp(int i_expression) {
    J3DAnmTexPattern* bck = NULL;
    int attr = 0;
    mAnmFlags &= ~0xa80;
    if (l_btpGetParamList[i_expression][0] >= 0) {
        bck = getTexPtrnAnmP(l_resNames[l_btpGetParamList[i_expression][1]],
                             l_btpGetParamList[i_expression][0]);
    }
    switch(i_expression)
    {
        case 0:
            attr = 2;
            break;
        case 3:
            attr = 2;
            break;
        case 6:
            attr = 2;
            break;
        case 8:
            attr = 2;
            break;
        case 11:
            attr = 2;
            break;
        default:
            bck = NULL;
        case 1:
        case 2:
        case 4:
        case 5:
        case 7:
        case 9:
        case 10:
        case 12:
        case 13:
        case 14:
            break;
    }
    if (bck == NULL) {
        return true;
    }
    if (setBtpAnm(bck, mAnm_p->getModel()->getModelData(), 1.0f, attr)) {
        mAnmFlags |= 0x280;
        if (i_expression == 0) {
            mAnmFlags |= 0x800;
        }
        return true;
    }
    OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", __FILE__);
    return false;

}

void daNpc_grA_c::setExpression(int i_expression, f32 i_morfOverride) {
    if (i_expression >= 0 && i_expression < 0x18) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morfOverride;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

void daNpc_grA_c::setMotionAnm(int i_motion, f32 i_morf) {
    J3DAnmTransformKey* bck = NULL;
    J3DAnmTextureSRTKey* btk = NULL;
    int btkIndex = 0;
    int attr2 = 2;
    int attr = 2;
    mAnmFlags &= ~0x3f;
    if (l_bckGetParamList[i_motion][0] >= 0) {
        bck = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[i_motion][1]],
                                l_bckGetParamList[i_motion][0]);
    }
    switch(i_motion)
    {
    case 25:
        attr2 = 0;
        break;
    case 27:
    case 28:
        attr2 = 0;
        break;
    case 31:
    case 32:
    case 33:
        attr2 = 0;
        break;
    case 35:
    case 37:
        attr2 = 0;
        break;
    case 40:
        attr2 = 0;
        break;
    case 42:
        attr2 = 0;
        break;
    case 44:
        attr2 = 0;
        break;
    case 46:
        attr2 = 0;
        break;
    case 47:
        attr2 = 0;
        break;
    case 49:
        attr2 = 0;
        break;
    case 50:
        attr2 = 0;
        break;
    default:
        bck = NULL;
        btk = NULL;
    case 22:
    case 23:
    case 24:
    case 26:
    case 29:
    case 30:
    case 34:
    case 36:
    case 38:
    case 39:
    case 41:
    case 43:
    case 45:
    case 48:
        break;
    }
    if (l_btkGetParamList[btkIndex][0] >= 0) {
        btk = getTexSRTKeyAnmP(l_resNames[l_btkGetParamList[btkIndex][1]], l_btkGetParamList[btkIndex][0]);
    }
    if (btk != NULL && setBtkAnm(btk, mAnm_p->getModel()->getModelData(), 1.0f, attr)) {
        mAnmFlags |= 0x12;
    }
    if (bck != NULL && setMcaMorfAnm(bck, 1.0f, i_morf, attr2, 0, -1)) {
        mAnmFlags |= 0x9;
        mMotionLoops = 0;
        field_0x14D8 = i_motion;
    }
}

void daNpc_grA_c::setMotion(int i_motion, f32 i_MorfOverride, int param_2) {
    s16 motion = i_motion;
    if ((param_2 != 0 || mMotion != motion) && (i_motion >= 0 && i_motion < 0x1d)) {
        mMotion = motion;
        mMotionMorfOverride = i_MorfOverride;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

BOOL daNpc_grA_c::drawDbgInfo() {
#if DEBUG
    if (GET_HIO(mNpcFParams.debug_info_ON) != 0) {
        cXyz o_pos;
        f32 maxSpeakDist = dComIfGp_getAttention()->getDistTable(attention_info.distances[fopAc_attn_SPEAK_e]).mDistMax;
        f32 maxTalkDist = dComIfGp_getAttention()->getDistTable(attention_info.distances[fopAc_attn_TALK_e]).mDistMax;
        s16 ang = cLib_targetAngleX(&current.pos, &eyePos);
        s16 ang2 = cLib_targetAngleY(&current.pos, &eyePos);

        dDbVw_drawCircleOpa(attention_info.position, maxSpeakDist, (GXColor){0, 0xc8, 0, 0xff}, 1,
                            12);
        dDbVw_drawCircleOpa(attention_info.position, maxTalkDist, (GXColor){0xc8, 0, 0, 0xff}, 1,
                            12);

        if (GET_HIO(mNpcFParams.fov) != 180.0f) {
            cXyz offset(0.0f, 0.0f, GET_HIO(mNpcFParams.search_distance));
            mDoMtx_stack_c::transS(attention_info.position);
            mDoMtx_stack_c::YrotM(mHeadAngle.y);
            mDoMtx_stack_c::YrotM(cM_deg2s(-GET_HIO(mNpcFParams.fov)));
            mDoMtx_stack_c::multVec(&offset, &o_pos);

            dDbVw_drawLineOpa(attention_info.position, o_pos, (GXColor){0, 0, 0xc8, 0xff}, 1, 12);

            mDoMtx_stack_c::transS(attention_info.position);
            mDoMtx_stack_c::YrotM(mHeadAngle.y);
            mDoMtx_stack_c::YrotM(cM_deg2s(GET_HIO(mNpcFParams.fov)));
            mDoMtx_stack_c::multVec(&offset, &o_pos);

            dDbVw_drawLineOpa(attention_info.position, o_pos, (GXColor){0, 0, 0xc8, 0xff}, 1, 12);
        }
        dDbVw_drawCircleOpa(attention_info.position, GET_HIO(mNpcFParams.search_distance),
                            (GXColor){0, 0, 0xc8, 0xff}, 1, 12);

        dDbVw_drawSphereXlu(eyePos, 18.f, (GXColor){0x80, 0x80, 0x80, 0xA0}, 1);

        dDbVw_drawSphereXlu(mLookPos, 18.f, (GXColor){0x80, 0x80, 0x80, 0xA0}, 1);

        int tableidx = getDistTableIdx(field_0x145C, field_0x1460);
        maxSpeakDist = dComIfGp_getAttention()->getDistTable(tableidx).mDistMax;

        dDbVw_drawCircleOpa(attention_info.position, maxSpeakDist,
                            (GXColor){0xc8, 0xc8, 0xc8, 0xff}, 1, 0xc);

        if (field_0xDD4.getPathInfo() != NULL) {
            field_0xDD4.drawDbgInfoXyz();
        }
    }
#endif
    return false;
}

void daNpc_grA_c::drawOtherMdls() {
    if (field_0x150C && mpModel) {
        g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
        switch (mType) {
        case 7:
            mpModel->setBaseTRMtx(field_0x14DC);
            break;
        case 8:
        case 9:
        case 10:
            mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(17));
            mDoMtx_stack_c::scaleM(scale);
            mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
            J3DModelData* data = mpModel->getModelData();
            fopAcM_setEffectMtx(this, data);
            // fallthrough to match debug
        }
        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_addRealShadow(mShadowKey, mpModel);
    }
}

u8 daNpc_grA_c::getMode() {
    return fopAcM_GetParam(this);
}

u8 daNpc_grA_c::getMode1() {
    return (fopAcM_GetParam(this) & 0xFF00) >> 8;
}

u8 daNpc_grA_c::getTypeFromParam() {
    u8 v = (fopAcM_GetParam(this) & 0xF0000000) >> 28;
    switch (v) {
    case 0:
        if (mTwilight) {
            return 1;
        } else {
            return 0;
        }
    case 1:
        return 2;
    case 2:
        return 3;
    case 3:
        return 4;
    case 4:
        return 5;
    case 5:
        return 6;
    case 6:
        if (getMode() == 1) {
            if (mTwilight) {
                return 9;
            } else {
                return 8;
            }
        } else {
            return 7;
        }
    case 7:
        return 10;
    case 8:
        return 11;
    default:
        return 12;
    }
}

u8 daNpc_grA_c::getSwBit() {
    return (fopAcM_GetParam(this) & 0xff0000) >> 16;
}

BOOL daNpc_grA_c::isDelete() {
    if (mType == 0xc || mType == 0 || mType == 1 || mType == 3 || mType == 4 || mType == 5 ||
        (mType == 6 && daNpcF_chkEvtBit(0x150)) || (mType == 8 && daNpcF_chkEvtBit(0x179)) ||
        mType == 9 || mType == 0xa || mType == 0xb)
    {
        return FALSE;
    }
    if (mType == 2)
    {
        if ((strcmp(dComIfGp_getStartStageName(), "F_SP109") == 0 && !daNpcF_chkEvtBit(0x40)) ||
            (getMode1() == 1 && !daNpcF_chkEvtBit(0x108)))
        {
            return TRUE;
        } else {
            return FALSE;
        }
    }
    if (mType == 7) {
        if (!daNpcF_chkEvtBit(0x67)) {
            return TRUE;
        }
        if (!daNpcF_chkEvtBit(0x179)) {
            return FALSE;
        }
        fopAcM_createItemForBoss(&current.pos, 0x21, fopAcM_GetRoomNo(this), NULL, NULL, 0.0f, 0.0f,
                                 0x80);
        return TRUE;
    }
    return TRUE;
}

void daNpc_grA_c::reset() {
    initialize();
    field_0xDD4.initialize();
    if (mType == 5 || mType == 7) {
        if (field_0xDD4.setPathInfo(getPathNoFromParam(), fopAcM_GetRoomNo(this), 0)) {
            field_0xDD4.setRange(100.0f);
        }
    }
    mNpcfLookAt.initialize();
    for (int i = 0; i < 3; i++) {
        mNpcfActorManager[i].initialize();
    }
    mAction2 = NULL;
    mAction = NULL;
    field_0x145C = 0;
    field_0x1460 = 0;
    field_0x1464 = 0;
    field_0x1468 = 0;
    mLookMode = -1;
    field_0x1472 = 0;
    mHide = false;
    field_0x1480 = -1;
    mDoMtx_identity(field_0x14DC);
    field_0x150C = 0;
    current.pos = home.pos;
    old.pos = current.pos;
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);
    for (int i = 0; i < 4; i++) {
        if (i == 3) {
            mParticleManager[i].mManager.init(&mAcch, 215.f, 215.f);
        } else {
            mParticleManager[i].mManager.init(&mAcch, 0.f, 0.f);
        }
    }
    field_0x1489 = 0;
    field_0x149E = 0;
    field_0x14D4 = 0.0f;
    field_0x1691 = 0;
    field_0x1486 = 0;
    field_0x1690 = 0;
    field_0x1487 = 0;
    if (mType == 2 && getMode1() == 0) {
        mName = l_myName[1];
        field_0x1488 = 1;
        field_0x1487 = 1;
    } else if (mType == 7) {
        mName = l_myName[0];
        if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0 && fopAcM_GetRoomNo(this) == 0 &&
            dComIfGp_getStartStageLayer() == 8 && dComIfGs_getStartPoint() == 8)
        {
            field_0x1488 = 1;
        } else {
            field_0x1488 = 0;
        }
    } else if (mType == 8 || mType == 9) {
        mName = l_myName[0];
        field_0x1488 = 0;
        field_0x150C = 1;
    } else if (mType == 10) {
        mName = l_myName[0];
        if (daNpcF_chkEvtBit(0x187) == 0) {
            field_0x1486 = 0;
            field_0x1488 = 0;
        } else if (!dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this))) {
            field_0x1486 = 1;
            field_0x1488 = 0;
        } else {
            field_0x1486 = 3;
            field_0x1488 = 1;
        }
    } else {
        mName = l_myName[0];
        if (mType == 3) {
            if (mMode < 6) {
                eventInfo.setIdx(mMode + 3);
            }
        } else if (mType == 4) {
            if (mMode < 2) {
                eventInfo.setIdx(mMode + 1);
                if (daNpcF_chkEvtBit(0x3e) == 0) {
                    cXyz offset(0.0f, 0.0f, -150.0f);
                    mDoMtx_stack_c::transS(home.pos);
                    mDoMtx_stack_c::YrotM(home.angle.y);
                    mDoMtx_stack_c::multVec(&offset, &offset);
                    parentActorID =
                        fopAcM_createChild(0x8b, fopAcM_GetID(this), -1, &offset, fopAcM_GetRoomNo(this),
                                           &current.angle, NULL, -1, NULL);
                }
            } else {
                mName = l_myName[2];
                if (dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this)) == 0) {
                    cXyz a(0.0f, 0.0f, -150.0f);
                    mDoMtx_stack_c::transS(home.pos);
                    mDoMtx_stack_c::YrotM(home.angle.y);
                    mDoMtx_stack_c::multVec(&a, &a);
                    parentActorID =
                        fopAcM_createChild(0x8b, fopAcM_GetID(this), mSwBit, &a,
                                           fopAcM_GetRoomNo(this), &current.angle, NULL, -1, NULL);
                }
            }
        } else if (mType == 6 ) {
            if (mMode != 0) {
                field_0x1486 = 0;
            } else {
                field_0x1486 = 1;
            }
        }
        field_0x1488 = 0;
    }
    field_0x1692 = field_0x1693 = 0;
    field_0x1694 = 0;
    for (int i = 0; i < 3; i++) {
        mParticleID[i] = -1;
    }
    field_0x151C = -1;
    field_0x9ee = 1;
}

void daNpc_grA_c::playExpression() {
    daNpcF_anmPlayData dat0 = {1, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat0_ = {0, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat0[] = {&dat0, &dat0_};
 
    daNpcF_anmPlayData dat1 = {2, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat1_ = {0, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat1[] = {&dat1, &dat1_};
 
    daNpcF_anmPlayData dat2 = {7, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat2_ = {6, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat2[] = {&dat2, &dat2_ };
    
    daNpcF_anmPlayData dat3 = {5, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat3_ = {4, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat3[] = {&dat3, &dat3_};
 
    daNpcF_anmPlayData dat5 = {15, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat5_ = {12, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat5[] = {&dat5, &dat5_};
    
    daNpcF_anmPlayData dat6 = {13, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat6_ = {14, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat6[] = {&dat6, &dat6_};
 
    daNpcF_anmPlayData dat7 = {0x13, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat7_ = {0x17, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat7[] = {&dat7, &dat7_};
    
    daNpcF_anmPlayData dat8 = {0x14, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat8_ = {0x17, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat8[] = {&dat8, &dat8_};
 
    daNpcF_anmPlayData dat9 = {0x15, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat9_ = {0xe, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat9[] = {&dat9, &dat9_};

    daNpcF_anmPlayData dat11 = {8, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat11[] = {&dat11};

    daNpcF_anmPlayData dat12 = {9, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat12_ = {8, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat12[] = {&dat12, &dat12_};

    daNpcF_anmPlayData dat13 = {10, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat13[] = {&dat13};

    daNpcF_anmPlayData dat14 = {0xb, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat14_ = {0xa, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat14[] = {&dat14, &dat14_};

    daNpcF_anmPlayData dat15 = {6, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat15[] = {&dat15};

    daNpcF_anmPlayData dat16 = {4, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat16[] = {&dat16};

    daNpcF_anmPlayData dat17 = {0xc, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat17[] = {&dat17};

    daNpcF_anmPlayData dat18 = {0xe, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat18[] = {&dat18};

    daNpcF_anmPlayData dat19 = {16, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat19[] = {&dat19};

    daNpcF_anmPlayData dat20 = {17, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat20[] = {&dat20};

    daNpcF_anmPlayData dat21 = {18, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat21_ = {0xe, 0.0f, 0};
    daNpcF_anmPlayData* pDat21[] = {&dat21, &dat21_};
    
    daNpcF_anmPlayData dat22 = {12, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat22[] = {&dat22};

    daNpcF_anmPlayData dat23 = {0, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat23[] = {&dat23};
    
    daNpcF_anmPlayData** ppDat[24] = {  
        pDat0,
        pDat1,
        pDat2,
        pDat3,
        NULL,
        pDat5,
        pDat6,
        pDat7,
        pDat8,
        pDat9,
        NULL,
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
    };

    if (mExpression >= 0 && mExpression < 24) {
        playExpressionAnm(ppDat);
    }
}

void daNpc_grA_c::playMotion() {
    daNpcF_anmPlayData dat0 = {0x16, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat0[] = {&dat0};

    daNpcF_anmPlayData dat1 = {0x18, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat1[] = {&dat1};

    daNpcF_anmPlayData dat2 = {0x27, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat2[] = {&dat2};

    daNpcF_anmPlayData dat3 = {0x1D, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat3_ = {0x16, 0.0f, 0};
    daNpcF_anmPlayData* pDat3[] = {&dat3, &dat3_};

    daNpcF_anmPlayData dat4 = {0x1e, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat4_ = {0x16, 0.0f, 0};
    daNpcF_anmPlayData* pDat4[] = {&dat4, &dat4_};

    daNpcF_anmPlayData dat5 = {0x17, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat5[] = {&dat5};

    daNpcF_anmPlayData dat6 = {0x28, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat6_ = {0x29, 0.0f, 0};
    daNpcF_anmPlayData* pDat6[] = {&dat6, &dat6_};

    daNpcF_anmPlayData dat7 = {0x29, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat7[] = {&dat7};

    daNpcF_anmPlayData dat8 = {0x2a, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat8_ = {0x2b, 0.0f, 0};
    daNpcF_anmPlayData* pDat8[] = {&dat8, &dat8_};

    daNpcF_anmPlayData dat9 = {0x2b, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat9[] = {&dat9};

    daNpcF_anmPlayData dat10 = {0x2f, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat10_ = {0x16, 0.0f, 0};
    daNpcF_anmPlayData* pDat10[] = {&dat10, &dat10_};

    daNpcF_anmPlayData dat11 = {0x22, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat11[] = {&dat11};

    daNpcF_anmPlayData dat12 = {0x23, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat12_ = {0x22, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat12[] = {&dat12, &dat12_};

    daNpcF_anmPlayData dat13 = {0x24, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat13[] = {&dat13};

    daNpcF_anmPlayData dat14 = {0x25, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat14_ = {0x24, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat14[] = {&dat14, &dat14_};

    daNpcF_anmPlayData dat15 = {0x32, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat15_ = {0x30, 0.0f, 0};
    daNpcF_anmPlayData* pDat15[] = {&dat15, &dat15_};

    daNpcF_anmPlayData dat16 = {0x30, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat16[] = {&dat16};

    daNpcF_anmPlayData dat17 = {0x31, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat17_ = {0x30, 0.0f, 0};
    daNpcF_anmPlayData* pDat17[] = {&dat17, &dat17_};

    daNpcF_anmPlayData dat18 = {0x1b, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat18[] = {&dat18};

    daNpcF_anmPlayData dat19 = {0x1a, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat19[] = {&dat19};

    daNpcF_anmPlayData dat20 = {0x19, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat20_ = {0x16, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat20[] = {&dat20, &dat20_};

    daNpcF_anmPlayData dat21 = {0x1f, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat21_ = {0x16, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat21[] = {&dat21, &dat21_};

    daNpcF_anmPlayData dat22 = {0x1c, 4.0f, 0};
    daNpcF_anmPlayData* pDat22[] = {&dat22};

    daNpcF_anmPlayData dat23 = {0x20, 2.0f, 0};
    daNpcF_anmPlayData* pDat23[] = {&dat23};

    daNpcF_anmPlayData dat24 = {0x21, 2.0f, 0};
    daNpcF_anmPlayData* pDat24[] = {&dat24};

    daNpcF_anmPlayData dat25 = {0x2c, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat25[] = {&dat25};

    daNpcF_anmPlayData dat26 = {0x2d, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat26[] = {&dat26};

    daNpcF_anmPlayData dat27 = {0x2e, GET_HIO(mNpcFParams.morf_frame), 1};
    daNpcF_anmPlayData dat27_ = {0x29, 0.0f, 0};
    daNpcF_anmPlayData* pDat27[] = {&dat27, &dat27_};

    daNpcF_anmPlayData dat28 = {38, GET_HIO(mNpcFParams.morf_frame), 0};
    daNpcF_anmPlayData* pDat28[] = {&dat28};

    daNpcF_anmPlayData** ppDat[29] = {  
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
        pDat27,
        pDat28,
     };

    if (mMotion >= 0 && mMotion < 29) {
        playMotionAnm(ppDat);
    }
}

BOOL daNpc_grA_c::chkAction(daNpc_grA_c_Action i_action) {
    return mAction == i_action;
}

BOOL daNpc_grA_c::setAction(daNpc_grA_c_Action i_action) {
    field_0x1472 = 3;
    if (mAction) {
        (this->*mAction)(NULL);
    }
    field_0x1472 = 0;
    mAction = i_action;
    if (mAction) {
        (this->*mAction)(NULL);
    }
    return TRUE;
}

BOOL daNpc_grA_c::selectAction() {
    mAction2 = NULL;
    if (GET_HIO(mNpcFParams.debug_mode_ON)) {
        mAction2 = &daNpc_grA_c::test;
    } else {
        switch (mType) {
        case 2:
            mAction2 = &daNpc_grA_c::spaWarm;
            break;
        case 3:
            mAction2 = &daNpc_grA_c::waitCheer;
            break;
        case 4:
            mAction2 = &daNpc_grA_c::waitGate;
            break;
        case 5:
            mAction2 = &daNpc_grA_c::waitKickOut;
            break;
        case 6:
            if (field_0x1486 == 0) {
                mAction2 = &daNpc_grA_c::wait;
            } else {
                mAction2 = &daNpc_grA_c::moveRock;
            }
            break;
        case 7:
            mAction2 = &daNpc_grA_c::waitSpaWater;
            break;
        case 8:
            mAction2 = &daNpc_grA_c::waitSpaBuyer;
            break;
        case 9:
            mAction2 = &daNpc_grA_c::waitSpaBuyerTW;
            break;
        case 10:
            switch (field_0x1486) {
            case 0:
                mAction2 = &daNpc_grA_c::beforeCrashWait;
                break;
            case 1:
                mAction2 = &daNpc_grA_c::crashRoll;
                break;
            case 2:
                mAction2 = &daNpc_grA_c::crashRollWait;
                break;
            case 3:
                mAction2 = &daNpc_grA_c::crashedWait;
                break;
            }
            break;
        case 11:
            if (mMode == 1) {
                mAction2 = &daNpc_grA_c::waitBuyer;
            } else {
                mAction2 = &daNpc_grA_c::wait;
            }
            break;
        default:
            if (mTwilight) {
                mAction2 = &daNpc_grA_c::waitTW;
            } else {
                mAction2 = &daNpc_grA_c::wait;
            }
        }
    }
    return TRUE;
}

enum Event_Cut_Nums {
    /* 0x2 */ NUM_EVT_CUTS_e = 0xc,
};

BOOL daNpc_grA_c::doEvent() {
    dEvent_manager_c* manager = NULL;
    BOOL ret = FALSE;
    if (dComIfGp_event_runCheck()) {
        manager = &dComIfGp_getEventManager();
        if (field_0x1691 == 0) {
            mOrderNewEvt = 0;
        } else {
            mOrderNewEvt = 1;
        }
        if (eventInfo.checkCommandTalk()) {
            if (chkAction(&daNpc_grA_c::talk)) {
                (this->*mAction)(NULL);
            } else if (dComIfGp_event_chkTalkXY() == FALSE || dComIfGp_evmng_ChkPresentEnd()) {
                if (field_0x1487 == 0) {
                    setAction(&daNpc_grA_c::talk);
                } else {
                    mOrderEvtNo = 1;
                    changeEvent(l_resNames[l_evtGetParamList[mOrderEvtNo][1]],
                                l_evtNames[l_evtGetParamList[mOrderEvtNo][0]], 2, -1);
                }
            }
            ret = TRUE;
        } else {
            if (field_0x1480 != fpcM_ERROR_PROCESS_ID_e) {
                dComIfGp_event_setItemPartnerId(field_0x1480);
                field_0x1480 = -1;
            }
            int id;
            if (eventInfo.getIdx() > 0) {
                id = manager->getMyStaffId(mName, this, 0);
            } else {
                id = manager->getMyStaffId(mName, NULL, 0);
            }
            if (id != -1) {
                mStaffID = id;
                int evtCutNo =
                    manager->getMyActIdx(id, mEvtCutNameList, ARRAY_SIZE(mEvtCutNameList), FALSE, FALSE);
                JUT_ASSERT(3154, (0 <= evtCutNo) && (evtCutNo < NUM_EVT_CUTS_e));
                JUT_ASSERT(3155, NULL != mEvtCutList[evtCutNo]);
                if ((this->*(mEvtCutList[evtCutNo]))(id)) {
                    manager->cutEnd(id);
                }
                ret = TRUE;
            }
            if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && manager->endCheck(mEventIdx)) {
                switch (mOrderEvtNo) {
                case 5:
                    dComIfGp_event_reset();
                    mOrderEvtNo = 0;
                    mEventIdx = -1;
                    field_0x1693 = field_0x1692 = 0;
                    fpcM_Search(s_subCarry, this);
                    break;
                case 6:
                    dComIfGp_event_reset();
                    mOrderEvtNo = 0;
                    mEventIdx = -1;
                    break;
                case 7:
                    dComIfGp_event_reset();
                    mOrderEvtNo = 0;
                    mEventIdx = -1;
                    fopAcM_delete(this);
                    break;
                default:
                    dComIfGp_event_reset();
                    mOrderEvtNo = 0;
                    mEventIdx = -1;
                    break;
                }
            }
        }
        int expression, motion;
        int timer = mMsgTimer;
        if (ctrlMsgAnm(expression, motion, this, FALSE)) {
            if (!field_0x9eb) {
                setExpression(expression, -1.0f);
                setMotion(motion, -1.0f, 0);
            }
        } else if (timer && !field_0x9eb) {
            setExpressionTalkAfter();
        }

        switch (mExpression) {
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
            mAnmFlags &= ~0x1000;
        }
    } else {
        mMsgTimer = 0;
        if (mStaffID != -1) {
            mAction = NULL;
            mStaffID = -1;
        }
        mMsgTimer = 0;
    }

    return ret;
}

void daNpc_grA_c::setLookMode(int i_lookMode) {
    if (i_lookMode >= 0 && i_lookMode < 9 && i_lookMode != mLookMode) {
        mLookMode = i_lookMode;
    }
}

void daNpc_grA_c::lookat() {
    fopAc_ac_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();
    BOOL r27 = FALSE;
    f32 bodyAngleXMin = GET_HIO(mNpcFParams.body_angleX_min);
    f32 bodyAngleXMax = GET_HIO(mNpcFParams.body_angleX_max);
    f32 bodyAngleYMin = GET_HIO(mNpcFParams.body_angleY_min);
    f32 bodyAngleYMax = GET_HIO(mNpcFParams.body_angleY_max);
    f32 headAngleXMin = GET_HIO(mNpcFParams.head_angleX_min);
    f32 headAngleXMan = GET_HIO(mNpcFParams.head_angleX_max);
    f32 headAnglyMin = GET_HIO(mNpcFParams.head_angleY_min);
    f32 headAngleYMax = GET_HIO(mNpcFParams.head_angleY_max);
    s16 diffInAngle = mCurAngle.y - mOldAngle.y;
    cXyz pLookPositions[] = {
        mLookatPos[0],
        mLookatPos[1],
        mLookatPos[2],
    };
    csXyz* pLookAngles[] = {
        &mLookatAngle[0],
        &mLookatAngle[1],
        &mLookatAngle[2],
    };
    cXyz unused;
    switch (mLookMode) {
    case 1:
        r27 = TRUE;
        break;
    case 2:
    case 3:
        player = daPy_getPlayerActorClass();
        if (mLookMode == 3) {
            headAnglyMin = -80.0f;
            headAngleYMax = 80.0f;
        }
        break;
    case 5:
        player = mNpcfActorManager[2].getActorP();
        break;
    case 0:
    case 6:
    case 7:
    case 8:
        break;
    }
    if (player != NULL) {
        mLookPos = player->attention_info.position;
        if (mLookMode != 2 && mLookMode != 3 && mLookMode != 4) {
            mLookPos.y -= 40.0f;
        }
        mNpcfLookAt.setAttnPos(&mLookPos);
    } else if (mLookMode == 6) {
        mLookPos = field_0x14A0;
        mLookPos.y += 160.0f;
        mNpcfLookAt.setAttnPos(&mLookPos);
    } else if (mLookMode == 7) {
        cXyz f(0.0f, 0.0f, 300.0f);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVec(&f, &mLookPos);
        mNpcfLookAt.setAttnPos(&mLookPos);
    } else if (mLookMode == 8) {
        cXyz f(0.0f, 0.0f, -300.0f);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVec(&f, &mLookPos);
        mNpcfLookAt.setAttnPos(&mLookPos);
    } else {
        mNpcfLookAt.setAttnPos(NULL);
    }
    mNpcfLookAt.setParam(bodyAngleXMin, bodyAngleXMax, bodyAngleYMin, bodyAngleYMax, 0.0f, 0.0f, 0.0f, 0.0f, headAngleXMin, headAngleXMan, headAnglyMin, headAngleYMax,
                         mCurAngle.y, pLookPositions);

    mNpcfLookAt.calc(this, model->getBaseTRMtx(), pLookAngles, r27, diffInAngle, FALSE);
}

BOOL daNpc_grA_c::chkFindPlayer() {
    BOOL ret = FALSE;
    s16 searchAngle = fopAcM_searchPlayerAngleY(this) - current.angle.y;
    if (abs(searchAngle) < 0x3c00) {
        if (mType == 8) {
            if (mNpcfActorManager[0].getActorP() == NULL) {
                ret = chkActorInAttnArea(daPy_getPlayerActorClass(), this, getDistTableIdx(8, 6));
            } else {
                ret = chkActorInAttnArea(daPy_getPlayerActorClass(), this, getDistTableIdx(10, 6));
            }
        } else {
            if (mNpcfActorManager[0].getActorP() == NULL) {
                ret = chkActorInAttnArea(daPy_getPlayerActorClass(), this, getDistTableIdx(10, 6));
            } else {
                ret = chkActorInAttnArea(daPy_getPlayerActorClass(), this, getDistTableIdx(12, 6));
            }
        }
    } else {
        if (mNpcfActorManager[0].getActorP() == NULL) {
            ret = chkPlayerInSpeakArea(this);
        } else {
            ret = chkPlayerInTalkArea(this);
        }
    }

    if (ret) {
        mNpcfActorManager[0].entry(daPy_getPlayerActorClass());
    } else {
        mNpcfActorManager[0].remove();
    }
    return ret;
}

void daNpc_grA_c::setExpressionTalkAfter() {
    switch (mExpression) {
    case 5:
        setExpression(0x11, -1.0f);
        break;
    case 9:
        setExpression(0x12, -1.0f);
        break;
    case 2:
        setExpression(0xf, -1.0f);
        break;
    case 3:
        setExpression(0x10, -1.0f);
        break;
    case 6:
        break;
    default:
        setExpression(0x17, -1.0f);
        break;
    }
}

BOOL daNpc_grA_c::setPrtcl() {
    JPABaseEmitter* particleEmitter = NULL;
    int sp20 = 0;
    int sp1C = 0;
    cXyz* sp18 = NULL;
    cXyz c(2.0f, 2.0f, 2.0f);
    for (int i = 0; i < 4; i++) {
        if (i == 0 || i == 1 || i == 2) {
            sp20 = 1;
        } else if (i == 3 && mType == 2) {
            sp20 = 1;
            sp1C = 0x40002;
            mParticleManager[i].mPos = current.pos;
            sp18 = &c;
        } else {
            sp20 = 0;
        }

        mParticleManager[i].mManager.setEffectCenter(
            &tevStr, &mParticleManager[i].mPos, sp20, sp1C,
            (mParticleManager[i].field_0x0 == 1 ? &mParticleManager[i].mPos : NULL),
            (mParticleManager[i].field_0x0 == 1 ? &mParticleManager[i].mAngle : NULL), sp18,
            fopAcM_GetRoomNo(this), 0.0f, speedF);
        if (mParticleManager[i].field_0x0 != 0) {
            particleEmitter = mParticleManager[i].mManager.getCenterEmitter(0, 0);
            if (particleEmitter != NULL) {
                switch (i) {
                case 0: {
                    mParticleManager[i].mScale.set(3.f, 3.f, 3.f);
                    particleEmitter->setGlobalParticleScale(mParticleManager[i].mScale);
                    particleEmitter->setGlobalAlpha(0x40);
                } break;
                case 1:
                case 2:
                    mParticleManager[i].mScale.set(2.f, 2.f, 2.f);
                    particleEmitter->setGlobalParticleScale(mParticleManager[i].mScale);
                    particleEmitter->setGlobalAlpha(0x40);
                    break;
                }
            }
        }
        mParticleManager[i].field_0x0 = 0;
    }
    return TRUE;
}


void daNpc_grA_c::setRollPrtcl(cXyz const& i_pos, f32 i_scale) {
    static u16 const l_prticles_id[] = {
        0x83B5,
        0x83B6,
        0x83B7,
    };
    JPABaseEmitter* particleEmitter = NULL;
    cXyz pos(i_pos);
    cXyz scale(i_scale, i_scale, i_scale);
    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            mParticleID[i] =
                dComIfGp_particle_set(mParticleID[i], l_prticles_id[i], &pos, &tevStr,
                                      &current.angle, &scale, 0xff, NULL, -1, NULL, NULL, NULL);
        } else {
            mParticleID[i] =
                dComIfGp_particle_setPolyColor(mParticleID[i], l_prticles_id[i], mAcch.m_gnd, &pos,
                                               &tevStr, &current.angle, &scale, 0, 0, -1, 0);
        }
        particleEmitter = dComIfGp_particle_getEmitter(mParticleID[i]);
        if (particleEmitter != NULL) {
            particleEmitter->setGlobalTranslation(pos.x, pos.y, pos.z);
            JGeometry::TVec3<float> f;
            cXyz globalScale;
            particleEmitter->getGlobalDynamicsScale(&f);
            globalScale.x = f.x;
            globalScale.y = f.y;
            globalScale.z = f.z;
            cLib_chasePos(&globalScale, scale, 0.07f);
            particleEmitter->setGlobalScale(globalScale);
        }
    }
}

static u8 l_offsetHeight[32] = {
    0xB6, 0xB6, 0xBA, 0xBE, 0xC6, 0xCE, 0xCE, 0xCE, 0xCA, 0xCA, 0xC6, 0xC2, 0xBE, 0xBA, 0xB6, 0xB6,
    0xBA, 0xBA, 0xBE, 0xC6, 0xCA, 0xCE, 0xD1, 0xCE, 0xC6, 0xBA, 0xCA, 0xCE, 0xCA, 0xBE, 0xB6, 0xB6,
};

void daNpc_grA_c::setOtherObjMtx() {
    if (field_0x150C != 0) {
        switch (mType) {
        case 7:
            cXyz pos = current.pos;
            pos.y += l_offsetHeight[(u32)mAnm_p->getFrame() % 0x1f]; // maybe meant to be & 0x1f
            mDoMtx_stack_c::transS(pos);
            mDoMtx_stack_c::ZXYrotM(mCurAngle);
            mDoMtx_stack_c::scaleM(scale);
            mDoMtx_copy(mDoMtx_stack_c::get(), field_0x14DC);
        }
    }
}

BOOL daNpc_grA_c::ECut_talkSpa(int i_staffID) {
    dEvent_manager_c& manager = dComIfGp_getEventManager();
    int unusedr26 = 0;
    daPy_py_c* r25 = daPy_getPlayerActorClass();
    BOOL r30 = FALSE;
    int* intp = NULL;
    int r28 = -1;
    int unusedr24 = 0;
    fopAc_ac_c* arr[2] = {this, mNpcfActorManager[2].getActorP()};
    intp = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (intp != NULL) {
        r28 = *intp;
    }
    if (manager.getIsAddvance(i_staffID)) {
        switch (r28) {
        case 0:
            ((daNpcT_c*)mNpcfActorManager[2].getActorP())->mJntAnm.lookActor(this, 0.0f, FALSE);
            setLookMode(5);
            break;
        case 1:
            initTalk(field_0x146C, arr);
            break;
        case 2:
            if (daNpcF_chkEvtBit(6) && daNpcF_chkEvtBit(0x3e) == FALSE) {
                mEventTimer = 0x3c;
                setLookMode(2);
            } else {
                mEventTimer = 0;
            }
            break;
        }
    }
    switch (r28) {
    case 0:
        r30 =   TRUE;
        break;
    case 1:
        if (talkProc(NULL, 0, arr)) {
            r30 = TRUE;
        }
        break;
    case 2:
        if (!cLib_calcTimer(&mEventTimer)) {
            r30 = TRUE;
            setLookMode(5);
        }
        break;
    default:
        r30 = 1;
        break;
    }
    return r30;
}

BOOL daNpc_grA_c::ECut_grDSRoll(int i_staffID) {
    dEvent_manager_c& sp34 = dComIfGp_getEventManager();
    int sp30 = 0;
    daPy_py_c* sp2c = daPy_getPlayerActorClass();
    int r28 = 0;
    int* sp28 = NULL;
    int sp24 = -1;
    int sp20 = 0;
    
    sp28 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (sp28 != NULL) {
        sp24 = *sp28;
    }

    if (sp34.getIsAddvance(i_staffID)) {
        switch (sp24) {
        case 0xa:
            mTurnMode = 0;
            break;
        case 0x14:
            mCreature.startCreatureVoice(Z2SE_GORON_V_SMR_CAUTION, -1);
            break;
        case 0x1e:
            setMotion(0x12, -1.0f, 0);
            setLookMode(0);
            mCreature.startCreatureVoice(Z2SE_GORON_V_SMR_TO_STONE, -1);
            break;
        case 0x28:
            setMotion(0x13, -1.0f, 0);
            setLookMode(0);
            field_0x14D4 = 0.1f;
            break;
        case 0x32:
            setLookMode(0);
            break;
        case 0x3c: {
            fopAc_ac_c* sp1c = (fopAc_ac_c*)fpcM_Search(s_subGRD, this);
            if (sp1c != NULL) {
                mNpcfActorManager[2].entry(sp1c);
            }
            setLookMode(5);
        } break;
        case 0x46:
            setTagJump2();
            setLookMode(2);
            break;
        case 0x50:
            setLookMode(2);
            mEventTimer = GET_HIO(mBowTimer);
            break;
        case 0x2d:
            break;
        }
    }

    switch (sp24) {
    case 0xa:
        r28 = 1;
        break;
    case 0x14: {
        s16 spA = current.angle.y;
        cLib_chaseAngleS(&spA, fopAcM_searchPlayerAngleY(this), 0x800);
        setAngle(spA);
        r28 = 1;
    } break;
    case 0x1e:
        if (field_0x14D8 == 0x1b) {
            if (mAnm_p->isStop()) {
                r28 = 1;
            } else if (mAnm_p->checkFrame(14.0f)) {
                cXyz c(0.0f, 0.0f, 20.0f);
                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(mCurAngle.y);
                mDoMtx_stack_c::multVec(&c, &mParticleManager[0].mPos);
                mParticleManager[0].mAngle = mCurAngle;
                mParticleManager[0].field_0x0 = 1;
            }
        }
        break;
    case 0x28:
        if (field_0x14D8 == 0x1a) {
            mAnm_p->setEndFrame(29.0f);
            if (field_0x14D4 < 3.0f) {
                field_0x14D4 += 0.2f;
            }
            mAnm_p->setPlaySpeed(field_0x14D4);
            speedF = 0.0f;
        }
        r28 = 1;
        break;
    case 0x2d:
        if (field_0x14D8 == 0x1a) {
            mAnm_p->setEndFrame(29.0f);
            if (field_0x14D4 < 3.0f) {
                field_0x14D4 += 0.2f;
            }
            mAnm_p->setPlaySpeed(field_0x14D4);
            speedF = field_0x14D4 * GET_HIO(mRotationalSpeed);
        }
        r28 = 1;
        break;
    case 0x32:
        if (field_0x14D8 == 0x1a) {
            if (field_0x14D4 > 1.0f) {
                field_0x14D4 -= 0.25f;
                mAnm_p->setPlaySpeed(field_0x14D4);
                speedF = field_0x14D4 * GET_HIO(mRotationalSpeed);
            } else {
                if (mAnm_p->getFrame() >= 0.0f && mAnm_p->getFrame() <= 5.0f) {
                    setMotion(0x14, -1.0f, 0);
                    field_0x14D4 = 0.0f;
                }
                speedF = 0.0f;
            }
        } else if (field_0x14D8 == 0x19) {
            if (mAnm_p->checkFrame(12.0f)) {
                mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(0x1a));
                mDoMtx_stack_c::multVecZero(&mParticleManager[1].mPos);
                mParticleManager[1].mPos.y = current.pos.y;
                mParticleManager[1].mAngle = mCurAngle;
                mParticleManager[1].field_0x0 = 1;
                mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(0x17));
                mDoMtx_stack_c::multVecZero(&mParticleManager[2].mPos);
                mParticleManager[2].mPos.y = current.pos.y;
                mParticleManager[2].mAngle = mCurAngle;
                mParticleManager[2].field_0x0 = 1;
            }
        } else if (field_0x14D8 == 0x16) {
            r28 = 1;
        }
        break;
    case 0x3c:
        r28 = 1;
        break;
    case 0x46: {
        s16 sp8 = current.angle.y;
        cLib_chaseAngleS(&sp8, fopAcM_searchPlayerAngleY(this), 0x800);
        setAngle(sp8);
        home.angle.y = sp8;
        r28 = 1;
    } break;
    case 0x50:
        if (fopAcM_searchPlayerAngleY(this) == mCurAngle.y) {
            mTurnMode = 0;
        } else if (step(fopAcM_searchPlayerAngleY(this), -1, 0x16, 0xf)) {
            mTurnMode = 0;
            if (field_0x14D8 == 0x1c) {
                setMotion(0, -1.0f, 0);
            }
        }
        if (cLib_calcTimer(&mEventTimer) == 0) {
            if (mMotion == 0x15) {
                if (field_0x14D8 == 0x16 && fopAcM_searchPlayerAngleY(this) == mCurAngle.y) {
                    r28 = 1;
                }
                if (field_0x14D8 == 0x1f && mAnm_p->getFrame() >= 10.0f &&
                    mAnm_p->getFrame() < 11.0f)
                {
                    mCreature.startCreatureVoice(Z2SE_GORON_V_SMR_REPLY, -1);
                }
            } else {
                setMotion(0x15, -1.0f, 0);
                setLookMode(0);
            }
        }
        break;
    default:
        r28 = 1;
        break;
    }

    if (field_0x14D8 == 0x1a) {
        if (field_0x14D4 >= 1.0f) {
            setRollPrtcl(current.pos, 1.0f);
        }
        f32 v = cLib_minMaxLimit(fabsf(field_0x14D4) * 20.0f, 1.0f, 127.0f);
        mCreature.startCreatureSound(Z2SE_GORON_ROLLING, v, -1);
    }
    return r28;
}

BOOL daNpc_grA_c::ECut_grDSGate(int i_staffID) {
    dEvent_manager_c& manager = dComIfGp_getEventManager();
    int sp10 = 0;
    daPy_py_c* spc = daPy_getPlayerActorClass();
    int r30 = 0;
    int* r29 = 0;
    int r28 = -1;
    int sp8 = 0;
    r29 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (r29 != NULL) {
        r28 = *r29;
    }
    if (manager.getIsAddvance(i_staffID)) {
        switch (r28) {
        case 0xa:
            mTurnMode = 0;
            break;
        case 0x14:
            if (mMode == 0) {
                setMotion(0x18, -1.0f, 0);
            } else {
                setMotion(0x17, -1.0f, 0);
            }
            break;
        case 0x1e:
            setHomeJump();
            setMotion(5, -1.0f, 0);
            setLookMode(2);
            break;
        }
    }
    switch (r28) {
    case 0xa:
        r30 = 1;
        break;
    case 0x14:
        if (mMotion != 5) {
            if (field_0x14D8 != 0x17) {
                cLib_chasePosXZ(&current.pos, field_0x148C, 30.f / 11.f);
                if (mAnm_p->isStop()) {
                    setMotion(5, -1.0f, 0);
                    home.pos = field_0x148C;
                    home.angle = field_0x1498;
                    field_0x14D1 = 1;
                    r30 = 1;
                }
            }
        } else {
            r30 = 1;
        }
        break;
    case 0x1e:
        r30 = 1;
        break;
    default:
        r30 = 1;
        break;
    }
    return r30;
}

BOOL daNpc_grA_c::ECut_teachElevator(int i_staffID) {
    dEvent_manager_c& manager = dComIfGp_getEventManager();
    int sp10 = 0;
    daPy_py_c* spc = daPy_getPlayerActorClass();
    int r30 = 0;
    int* r29 = 0;
    int r28 = -1;
    int sp8 = 0;
    r29 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (r29 != NULL) {
        r28 = *r29;
    }
    if (manager.getIsAddvance(i_staffID)) {
        switch (r28) {
        case 0:
            if (!dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(mSwBit, fopAcM_GetRoomNo(this));
                setGateWalk();
            }
            initTalk(0xc, NULL);
            setLookMode(6);
            break;
        case 0xa:
            mTurnMode = 0;
            break;
        case 0x14:
            initTalk(0xc, NULL);
            break;
        case 0x1e:
            mTurnMode = 0;
            break;
        case 0x28:
            if (mMode == 0) {
                setMotion(0x18, -1, 0);
            } else {
                setMotion(0x17, -1, 0);
            }
            mAcch.SetWallNone();
            break;
        }
    }

    switch (r28) {
    case 0:
        if (talkProc(NULL, 1, NULL)) {
            r30 = 1;
        }
        break;
    case 0xa:
        if (fopAcM_searchPlayerAngleY(this) == mCurAngle.y) {
            mTurnMode = 0;
            r30 = 1;
        } else if (step(fopAcM_searchPlayerAngleY(this), -1, -1, 0xf)) {
            mTurnMode = 0;
            setMotion(5, -1.0f, 0);
        }
        break;
    case 0x14:
        if (talkProc(NULL, 1, NULL)) {
            r30 = 1;
        }
        break;
    case 0x1e:
        if (home.angle.y == mCurAngle.y) {
            mTurnMode = 0;
            r30 = 1;
        } else if (step(home.angle.y, -1, -1, 0xf)) {
            mTurnMode = 0;
            setMotion(5, -1.0f, 0);
        }
        break;
    case 0x28:
        if (mMotion != 5) {
            if (field_0x14D8 != 0x17) {
                cLib_chasePosXZ(&current.pos, field_0x148C, 30.0f / 11.0f);
                if (mAnm_p->isStop()) {
                    setMotion(5, -1.0f, 0);
                    home.pos = field_0x148C;
                    home.angle = field_0x1498;
                    field_0x14D1 = 1;
                    r30 = 1;
                }
            }
        } else {
            r30 = 1;
            setLookMode(0);
        }
        break;
    default:
        r30 = 1;
        break;
    }
    return r30;
}

BOOL daNpc_grA_c::ECut_kickOut(int i_staffID) {
    dEvent_manager_c& sp40 = dComIfGp_getEventManager();
    int sp3C = 0;
    daPy_py_c* sp38 = daPy_getPlayerActorClass();
    int r28 = 0;
    int* sp34 = NULL;
    int sp30 = -1;
    int sp2C = 0;

    sp34 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (sp34 != NULL) {
        sp30 = *sp34;
    }

    if (sp40.getIsAddvance(i_staffID)) {
        switch (sp30) {
        case 0:
            setExpression(0x17, -1.0f);
            setMotion(0, -1.0f, 0);
            setLookMode(2);
            speedF = 0.0f;
            break;
        case 0xa:
            initTalk(0x428, NULL);
            break;
        case 0x14:
            setExpression(0x17, -1.0f);
            setMotion(1, -1.0f, 0);
            speedF = GET_HIO(mWalkingSpeed);
            mAnm_p->setPlaySpeed(GET_HIO(mWalkingSpeed) * GET_HIO(mWalkingAnimationSpeed));
            break;
        case 0x1e:
            Z2GetAudioMgr()->seStart(0x600b2, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            break;
        }
    }
    switch (sp30) {
    case 0:
        if (fopAcM_searchPlayerAngleY(this) == mCurAngle.y) {
            mTurnMode = 0;
            r28 = 1;
        } else if (step(fopAcM_searchPlayerAngleY(this), 0x17, 0x16, 0xf)) {
            mTurnMode = 0;
            setMotion(0, -1.0f, 0);
        }
        break;
    case 0xa:
        if (talkProc(NULL, 1, NULL)) {
            r28 = 1;
        }
        break;
    case 0x14:
        speedF = GET_HIO(mWalkingSpeed);
        mAnm_p->setPlaySpeed(GET_HIO(mWalkingSpeed) * GET_HIO(mWalkingAnimationSpeed));
        r28 = 1;
        break;
    case 0x1e:
        r28 = 1;
        break;
    default:
        r28 = 1;
        break;
    }

    return r28;
}

BOOL daNpc_grA_c::ECut_noneLook(int i_staffID) {
    dEvent_manager_c& manager = dComIfGp_getEventManager();
    int r26 = 0;
    daPy_py_c* player = daPy_getPlayerActorClass();
    int r30 = 0;
    int* r29 = 0;
    int r28 = -1;
    int r24 = 0;
    r29 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (r29 != NULL) {
        r28 = *r29;
    }
    if (manager.getIsAddvance(i_staffID)) {
        switch (r28) {
        case 0:
            setExpression(0x17, -1.0f);
            setMotion(0, -1.0f, 0);
            setLookMode(0);
            speedF = 0.0f;
            field_0x1690 = 1;
            break;
        case 0xa:
            mEventTimer = 0x41;
            setLookMode(7);
            break;
        case 0x14:
            initTalk(0x3ea, NULL);
            break;
        }
    }
    switch(r28)
    {
    case 0:
        r30 = 1;
        break;
    case 0xa:
        if (cLib_calcTimer(&mEventTimer) != 0) {
            if (mEventTimer == 0xf) {
                setLookMode(0);
                r30 = 1;
                break;
            }
            if (mEventTimer == 0x28) {
                setLookMode(8);
            }
        }
        break;
    case 0x14:
        if (talkProc(NULL, TRUE, NULL)) {
            r30 = 1;
        }
        break;
    default:
        r30 = 1;
    }
    return r30;
}

BOOL daNpc_grA_c::ECut_rescueRock(int i_staffID) {
    dEvent_manager_c& sp40 = dComIfGp_getEventManager();
    int sp3C = 0;
    daPy_py_c* sp38 = daPy_getPlayerActorClass();
    int r28 = 0;
    int* sp34 = NULL;
    int sp30 = -1;
    int sp2C = 0;
    
    sp34 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (sp34 != NULL) {
        sp30 = *sp34;
    }

    if (sp40.getIsAddvance(i_staffID)) {
        switch (sp30) {
        case 0:
            setExpression(0x17, 0.0f);
            setMotion(0x13, 0.0f, 0);
            setLookMode(0);
            gravity = -1.0f;
            speed.y = 41.0f;
            field_0x14D4 = 2.0f;
            field_0x1486 = 0;
            break;
        case 0x14:
            mAcch.ClrGrndNone();
            mAcch.ClrWallNone();
            break;
        case 0xa:
            break;
        }
    }
    switch (sp30) {
    case 0:
        if (speed.y < 10.0f) {
            if (field_0x14D8 != 0x19) {
                setExpression(0x17, -1.0f);
                setMotion(0x14, -1.0f, 0);
                gravity = -0.5f;
            } else {
                r28 = 1;
            }
        } else {
            cLib_chaseF(&field_0x14D4, 1.0f, 0.03f);
            mAnm_p->setPlaySpeed(field_0x14D4);
            mAnm_p->setEndFrame(27.0f);
        }
        break;
    case 0xa:
        if (field_0x14D8 == 0x19) {
            if (mAnm_p->getFrame() >= 8.0f) {
                setExpression(0x17, -1.0f);
                setMotion(0x1c, -1.0f, 0);
            }
        } else {
            r28 = 1;
        }
        break;
    case 0x14:
        if (mAcch.ChkGroundHit()) {
            r28 = 1;
        }
        if (mAcch.ChkGroundLanding()) {
            mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(0x1a));
            mDoMtx_stack_c::multVecZero(&mParticleManager[1].mPos);
            mParticleManager[1].mPos.y = current.pos.y;
            mParticleManager[1].mAngle = mCurAngle;
            mParticleManager[1].field_0x0 = 1;

            mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(0x17));
            mDoMtx_stack_c::multVecZero(&mParticleManager[2].mPos);
            mParticleManager[2].mPos.y = current.pos.y;
            mParticleManager[2].mAngle = mCurAngle;
            mParticleManager[2].field_0x0 = 1;
            setExpression(0x17, -1.0f);
            setMotion(0, -1.0f, 0);
            mCreature.startCreatureSound(Z2SE_GORON_ROLL_BOUND, 0, -1);
        }
        break;
    default:
        r28 = 1;
        break;
    }
    return r28;
}

BOOL daNpc_grA_c::ECut_carrySpaWater(int i_staffID) {
    dEvent_manager_c& sp40 = dComIfGp_getEventManager();
    int sp3C = 0;
    daPy_py_c* sp38 = daPy_getPlayerActorClass();
    int r28 = 0;
    int* sp34 = NULL;
    int sp30 = -1;
    int sp2C = 0;
    
    sp34 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (sp34 != NULL) {
        sp30 = *sp34;
    }

    if (sp40.getIsAddvance(i_staffID)) {
        switch (sp30) {
        case 0:{
            daNpcF_offTmpBit(0xb);
            daNpcF_offTmpBit(0xc);
            daNpcF_offTmpBit(0x52);
            setExpression(0x13, -1.0f);
            setMotion(0x19, -1.0f, 0);
            setLookMode(0);
            cXyz c;
            mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
            mDoMtx_stack_c::multVecZero(&c);
        } break;
        case 0xa:
            initTalk(0x29, NULL);
            break;
        case 0x14: {
            setLookMode(0);
            field_0x14D4 = 1.0f;
            cXyz c(250.0f, 0.0f, 100.0f);
            mDoMtx_stack_c::YrotS(shape_angle.y);
            mDoMtx_stack_c::multVec(&c, &c);
            c += current.pos;
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&c, shape_angle.y + 0x2000, 0);
        } break;
        case 0x1e:
            initTalk(0x29, NULL);
            break;
        case 0x23:
            setLookMode(3);
            mNpcfActorManager[0].entry(daPy_getPlayerActorClass());
            break;
        case 0x28:
            initTalk(0x29, NULL);
            break;
        case 0x2d:
            setLookMode(0);
            break;
        case 0x32:
            setMotion(0x12, 0.0f, 1);
            setExpression(0x17, -1.0f);
            break;
        case 0x3c: 
            field_0x14D4 = 0.0f;
            mAnm_p->setPlaySpeed(field_0x14D4);
            mEventTimer = 0x12c;
            break;
        case 0x41:
            mHide = 1;
            setAngle(home.angle.y + 0x8000);
            speedF = 0.0f;
            field_0x14D4 = 0.0f;
            break;
        case 0x46:
            field_0x150C = 1;
            mHide = 0;
            field_0xDD4.reverse();
            field_0xDD4.setNextIdx();
            mEventTimer = 0x118;
            mCreature.startCreatureVoice(Z2SE_GRA_V_ROLLING_RETURN, -1);
            break;
        case 0x50: {
            cXyz c = current.pos;
            c.y += 200.0f;
            csXyz c2(0, fopAcM_searchPlayerAngleY(this), 0);
            fopAcM_createItemForBoss(&c, 0x21, fopAcM_GetRoomNo(this), &c2, NULL, 0.0f, 20.0f,
                                     0x80);
        } break;
        }
    }

    switch (sp30) {
    case 0:
        if (mAnm_p->isStop()) {
            r28 = 1;
        } else {
            JPABaseEmitter* sp28 = 0;
            cXyz c;
            mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
            mDoMtx_stack_c::multVecZero(&c);
            field_0x151C = dComIfGp_particle_set(field_0x151C, 0x2a3, &c, &tevStr, &current.angle, NULL, 0xff,
                                  NULL, -1, NULL, NULL, NULL);
            sp28 = dComIfGp_particle_getEmitter(field_0x151C);
            if (sp28 != NULL)
            {
                sp28->setGlobalTranslation(c.x, c.y, c.z);
            }
        }
        break;
    case 0xa:
        if (talkProc(NULL, 1, NULL)) {
            r28 = 1;
        }
        break;
    case 0x14:
        if (mMotion == 6 && mMotionPhase > 0) {
            r28 = 1;
        }
        break;
    case 0x23:
        if (fopAcM_searchPlayerAngleY(this) == mCurAngle.y) {
            r28 = 1;
        } else if (step(fopAcM_searchPlayerAngleY(this), 0x17, 0x16, 0xf)) {
            setExpression(0x17, -1.0f);
            setMotion(0, -1.0f, 0);
            mTurnMode = 0;
        }
        break;
    case 0x1e:
    case 0x28:
        if (talkProc(NULL, 1, NULL)) {
            r28 = 1;
        }
        break;
    case 0x2d:
        if (home.angle.y == mCurAngle.y) {
            r28 = 1;
        } else {
            if (step(home.angle.y, 0x17, 0x16, 0xf)) {
                setExpression(0x17, -1.0f);
                setMotion(0, -1.0f, 0);
                mTurnMode = 0;
            }
        }
        break;
    case 0x32:
    if (field_0x14D8 == 0x1b) {
        if (mAnm_p->isStop()) {
                r28 = 1;
                setMotion(0x13, -1.0f, 0);
                mCreature.startCreatureVoice(Z2SE_GRA_V_ROLLING, -1);
            } else if (mAnm_p->checkFrame(14.0f)) {
                cXyz c(0.0f, 0.0f, 20.0f);
                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(mCurAngle.y);
                mDoMtx_stack_c::multVec(&c, &mParticleManager[0].mPos);
                mParticleManager[0].mAngle = mCurAngle;
                mParticleManager[0].field_0x0 = 1;
            }
        } else if (field_0x14D8 == 0x1a) {
            mAnm_p->setPlaySpeed(0.0f);
            mAnm_p->setEndFrame(29.0f);
            r28 = 1;
        }
        break;
    case 0x46:
    case 0x3c:
        if (field_0xDD4.getPathInfo() != NULL) {
            cXyz c;
            BOOL b = field_0xDD4.getDstPos(current.pos, c);
            if (!b) {
                cLib_addCalcAngleS2(&mCurAngle.y, cLib_targetAngleY(&current.pos, &c), 6, 0x400);
                setAngle(mCurAngle.y);
            }
            cLib_addCalc2(&field_0x14D4, 5.0f, 0.1f, 0.1f);
            mAnm_p->setPlaySpeed(field_0x14D4);
            speedF = field_0x14D4 * 10.0f;
        }
        if (!cLib_calcTimer(&mEventTimer)) {
            r28 = 1;
        }
        break;
    case 0x41:
        r28 = 1;
        break;
    case 0x50:
        r28 = 1;
        break;
    default:
        r28 = 1;
        break;
    }

    if ((field_0x14D8 == 0x1a && field_0x14D4 >= 1.0f) ||
        field_0x14D8 == 0x28 && mAnm_p->getFrame() >= 18.0f && mAnm_p->getFrame() <= 20.0f)
    {
        setRollPrtcl(current.pos, 1.0f);
        if (field_0x14D8 == 0x1a) {
            f32 v = cLib_minMaxLimit(fabsf(field_0x14D4) * 20.0f, 1.0f, 127.0f);
            mCreature.startCreatureSound(Z2SE_GORON_ROLLING, v, -1);
        }
    }
    return r28;
    
}

BOOL daNpc_grA_c::ECut_carrySpaWaterFailure(int i_staffID) {
    dEvent_manager_c& sp34 = dComIfGp_getEventManager();
    int sp30 = 0;
    daPy_py_c* sp2c = daPy_getPlayerActorClass();
    int r28 = 0;
    int* sp28 = NULL;
    int sp24 = -1;
    int sp20 = 0;
    
    sp28 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (sp28 != NULL) {
        sp24 = *sp28;
    }

    if (sp34.getIsAddvance(i_staffID)) {
        switch (sp24) {
        case 0:
            setExpression(0x16, -1.0f);
            setMotion(2, -1.0f, 0);
            setLookMode(0);
            field_0x1690 = 2;
            break;
        case 0xa:
            setExpression(0x16, -1.0f);
            setMotion(8, -1.0f, 0);
            break;
        case 0x14:
            daNpcF_onTmpBit(0x52);
            initTalk(0x29, NULL);
            break;
        }
    }
    switch (sp24) {
    case 0:
        r28 = 1;
        break;
    case 0xa:
        if (mMotion == 8) {
            if (mMotionPhase > 0) {
                r28 = 1;
            } else if (mAnm_p->checkFrame(21.0f)) {
                cXyz c(0.0f, 0.0f, -150.0f);
                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(mCurAngle.y);
                mDoMtx_stack_c::multVec(&c, &mParticleManager[0].mPos);
                mParticleManager[0].mAngle = mCurAngle;
                mParticleManager[0].field_0x0 = 1;
            }
        }
        break;
    case 0x14:
        if (talkProc(NULL, TRUE, NULL)) {
            r28 = 1;
        }
        break;
    default:
        r28 = 1;
        break;
    }
    return r28;
}

BOOL daNpc_grA_c::ECut_rollRockCrash(int i_staffID) {
    dEvent_manager_c& sp34 = dComIfGp_getEventManager();
    int sp30 = 0;
    daPy_py_c* sp2c = daPy_getPlayerActorClass();
    int r28 = 0;
    int* sp28 = NULL;
    int sp24 = -1;
    int sp20 = 0;
    
    sp28 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (sp28 != NULL) {
        sp24 = *sp28;
    }

    if (sp34.getIsAddvance(i_staffID)) {
        switch (sp24) {
        case 0:
            setExpression(0x17, -1.0f);
            setMotion(0, -1.0f, 0);
            setLookMode(2);
            field_0x1486 = 2;
            break;
        case 0xa:
            setExpression(0x15, -1.0f);
            setMotion(0x1b, -1.0f, 0);
            setLookMode(0);
            break;
        case 0x14: {
            setLookMode(0);
            initTalk(field_0x146C, NULL);
            cXyz c(200.0f, 100.0f, 0.0f);
            mDoMtx_stack_c::YrotS(home.angle.y);
            mDoMtx_stack_c::multVec(&c, &c);
            c += current.pos;
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(
                &c, cLib_targetAngleY(&c, &current.pos), 0);
        } break;
        case 0x1e:
            setLookMode(0);
            break;
        case 0x28:
            setExpression(0x17, -1.0f);
            setMotion(0x12, -1.0f, 0);
            break;
        case 0x3c: {
            dComIfGp_getVibration().StartShock(8, 0xf, cXyz(0.0f, 1.0f, 0.0f));
            speedF = 0.0f;
            mEventTimer = 0x2d;
        } break;
        case 0x32:
            break;
        }
    }

    switch (sp24) {
    case 0:
        r28 = 1;
        break;
    case 0xa:
        if (field_0x14D8 == 0x29) {
            r28 = 1;
        } else {
            if (mAnm_p->getFrame() >= 79.0f) {
                field_0x150C = 0;
            } else if (mAnm_p->getFrame() >= 7.0f) {
                field_0x150C = 1;
            } else {
                field_0x150C = 0;
            }
            if (mAnm_p->getFrame() >= 93.0f && mAnm_p->getFrame() < 94.0f) {
                mCreature.startCreatureVoice(Z2SE_GRA_V_SPAWATER_SHOUT, -1);
            }
        }
        break;
    case 0x14:
        if (talkProc(NULL, 1, NULL)) {
            r28 = 1;
        }
        break;
    case 0x1e:
        if (home.angle.y != mCurAngle.y) {
            if (step(home.angle.y, 0x17, 0x16, 0xf)) {
                setExpression(0x17, -1.0f);
                setMotion(0, -1.0f, 0);
            }
        } else {
            r28 = 1;
        }
        break;
    case 0x28:
        if (field_0x14D8 == 0x1b) {
            if (mAnm_p->isStop()) {
                setMotion(0x13, -1.0f, 0);
            } else if (mAnm_p->checkFrame(14.0f)) {
                cXyz c(0.0f, 0.0f, 20.0f);
                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(mCurAngle.y);
                mDoMtx_stack_c::multVec(&c, &mParticleManager[0].mPos);
                mParticleManager[0].mAngle = mCurAngle;
                mParticleManager[0].field_0x0 = 1;
            }
        } else if (field_0x14D8 == 0x1a) {
            mAnm_p->setPlaySpeed(0.0f);
            mAnm_p->setEndFrame(29.0f);
            r28 = 1;
            field_0x14D4 = 0.0f;
        }
        break;
    case 0x32:
        cLib_addCalc2(&field_0x14D4, 5.0f, 0.1f, 0.1f);
        mAnm_p->setPlaySpeed(field_0x14D4);
        speedF = field_0x14D4 * 10.0f;
        if (mAcch.ChkWallHit()) {
            r28 = 1;
        }
        break;
    case 0x3c:
        cLib_addCalc2(&field_0x14D4, 5.0f, 0.1f, 0.1f);
        mAnm_p->setPlaySpeed(field_0x14D4);
        if (!cLib_calcTimer(&mEventTimer)) {
            r28 = 1;
        }
        break;
    default:
        r28 = 1;
        break;
    }

    if (field_0x14D8 == 0x1a) {
        if (field_0x14D4 >= 1.0f) {
            f32 f31 = 1.0f;
            if (sp24 >= 0x3c) {
                f31 = GET_HIO(mParticleSize);
            }
            setRollPrtcl(current.pos, f31);
        }
        f32 v = cLib_minMaxLimit(fabsf(field_0x14D4) * 20.0f, 1.0f, 127.0f);
        mCreature.startCreatureSound(Z2SE_GORON_ROLLING, v, -1);
    }
    return r28;
}

BOOL daNpc_grA_c::ECut_talkSpaWater(int i_staffID) {
    dEvent_manager_c& manager = dComIfGp_getEventManager();
    int r26 = 0;
    daPy_py_c* player = daPy_getPlayerActorClass();
    int r30 = 0;
    int* r29 = 0;
    int r28 = -1;
    int r24 = 0;
    r29 = dComIfGp_evmng_getMyIntegerP(i_staffID, "prm");
    if (r29 != NULL) {
        r28 = *r29;
    }
    if (manager.getIsAddvance(i_staffID)) {
        switch (r28) {
        case 0:
            setLookMode(0);
            field_0x1690 = 1;
            break;
        case 0xa: {
            initTalk(0x30, NULL);
            cXyz c(0.0f, 0.0f, 500.0f);
            mDoMtx_stack_c::transS(home.pos);
            if ((daPy_getPlayerActorClass()->current.pos.z - current.pos.z) < 0.0f) {
                mDoMtx_stack_c::YrotM(home.angle.y + 0x1000);
            } else {
                mDoMtx_stack_c::YrotM(home.angle.y - 0x1000);
            }
            mDoMtx_stack_c::multVec(&c, &c);
            cXyz c2 = c;
            c2.y += 500.0f;
            if (fopAcM_gc_c::gndCheck(&c2)) {
                c.y = fopAcM_gc_c::getGroundY();
            }
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&c, cLib_targetAngleY(c, home.pos), 0);
        } break;
        }
    }
    switch (r28) {
    case 0:
        r30 = 1;
        break;
    case 10:
        if (talkProc(NULL, 1, NULL)) {
            r30 = 1;
        }
        break;
    default:
        r30 = 1;
    }
    return r30;
}

BOOL daNpc_grA_c::wait(void*) {
    switch (field_0x1472) {
    case 0:
        setExpression(0x17, -1.0f);
        setMotion(0, -1.0f, 0);
        setLookMode(0);
        mTurnMode = 0;
        field_0x1472 = 2;
        // fallthrough
    case 2:
        if (mDamageTimer == 0) {
            if (mNpcfActorManager[0].getActorP() != NULL) {
                if (!chkFindPlayer()) {
                    mTurnMode = 0;
                }
            } else if (chkFindPlayer()) {
                mTurnMode = 0;
            }

            if (mNpcfActorManager[0].getActorP() != NULL) {
                setLookMode(2);
            } else{
                setLookMode(0);
                if (home.angle.y != mCurAngle.y && step(home.angle.y, 0x17, 0x16, 0xf)) {
                    setExpression(0x17, -1.0f);
                    setMotion(0, -1.0f, 0);
                    field_0x1472 = 0;
                }
            }
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::waitTW(void*) {
        switch (field_0x1472) {
    case 0:
        setExpression(0x17, -1.0f);
        setMotion(0, 0.0f, 0);
        setLookMode(0);
        mTurnMode = 0;
        field_0x1472 = 2;
        // fallthrough
    case 2:
        if (!mIsDamaged) {
            setLookMode(0);
            if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 0xf)) {
                field_0x1472 = 0;
            }
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::waitCheer(void*) {
    switch (field_0x1472) {
    case 0:
        setExpressionAnm(0, TRUE);
        setMotion(0, -1.0f, FALSE);
        setLookMode(0);
        mTurnMode = 0;
        field_0x1472 = 2;
        // fallthrough
    case 2:
        if (field_0x1489 == 0) {
            fopAc_ac_c* r29 = (fopAc_ac_c*)fpcM_Search(s_sub, this);
            if (r29 != NULL) {
                daTagGra_c* r27 = (daTagGra_c*)r29;
                field_0x148C = r27->getGraPos();
                field_0x1498 = r27->getGraAngle();
                field_0x14BC = home.pos;
                field_0x14C8 = home.angle;
                fopAcM_delete(r29);
                field_0x1489 = 1;
            }
        }
        if (field_0x149E == 0) {
            fopAc_ac_c* r28 = (fopAc_ac_c*)fpcM_Search(s_sub2, this);
            if (r28 != NULL) {
                daTagGra_c* r26 = (daTagGra_c*)r28;
                field_0x14A0 = r26->getGraPos();
                field_0x14AC = r26->getGraAngle();
                fopAcM_delete(r28);
                field_0x149E = 1;
                if (daNpcF_chkEvtBit(0x3f)) {
                    setTagJump2();
                }
            }
        }
        if (mDamageTimer == 0) {
            if (mNpcfActorManager[0].getActorP() != NULL) {
                if (!chkFindPlayer()) {
                    mTurnMode = 0;
                }
            } else if (chkFindPlayer()) {
                mTurnMode = 0;
            }
            if (mNpcfActorManager[0].getActorP() != NULL) {
                setLookMode(2);
            } else {
                setLookMode(0);
                if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 0xf)) {
                    field_0x1472 = 0;
                }
            }
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::waitGate(void*) {
    switch (field_0x1472) {
    case 0:
        setExpressionAnm(0, TRUE);
        setMotion(5, -1.0f, FALSE);
        setLookMode(0);
        mTurnMode = 0;
        field_0x9ea = 1;
        field_0x1472 = 2;
        // fallthrough
    case 2:
        if (field_0x1489 == 0 || field_0x149E == 0) {
            if (getMode1() == 1 && field_0x149E == 0) {
                fopAc_ac_c* r27 = (fopAc_ac_c*)fpcM_Search(s_sub, this);
                if (r27 != NULL) {
                    daTagGra_c* r28 = (daTagGra_c*)r27;
                    field_0x14A0 = r28->getGraPos();
                    field_0x14AC = r28->getGraAngle();
                    field_0x14B4 = r28->getGraExtent();
                    field_0x14B8 = r28->getGraHeight();
                    fopAcM_delete(r27);
                    field_0x149E = 1;
                    if (daNpcF_chkEvtBit(0x40) &&
                        !dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this)))
                    {
                        dComIfGs_onSwitch(mSwBit, fopAcM_GetRoomNo(this));
                    }
                }
            } else {
                field_0x149E = 1;
            }
            if (field_0x149E == 1) {
                cXyz c(150.0f, 0.0f, 0.0f);
                if (mMode == 0) {
                    c.x = -150.0f;
                }
                cLib_offsetPos(&field_0x148C, &current.pos, current.angle.y, &c);
                field_0x1498 = home.angle;
                field_0x14BC = home.pos;
                field_0x14C8 = home.angle;
                field_0x1489 = 1;
                if (daNpcF_chkEvtBit(0x3e)) {
                    if (getMode1() == 1) {
                        if (dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this))) {
                            setTagJump();
                            field_0x14D0 = 1;
                            field_0x14D1 = 1;
                        }
                    } else {
                        setTagJump();
                        field_0x14D0 = 1;
                        field_0x14D1 = 1;
                    }
                } else if (daNpcF_chkEvtBit(0x3f) && getMode1() != 1) {
                    setTagJump();
                    field_0x14D0 = 1;
                    field_0x14D1 = 1;
                }
            }
        } else {
            if (getMode1() == 1 && daNpcF_chkEvtBit(0x3e) &&
                !dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this)))
            {
                cXyz diff = (daPy_getPlayerActorClass()->current.pos - field_0x14A0);
                if (diff.absXZ() < field_0x14B4) {
                    mOrderEvtNo = 2;
                    break;
                }
            }
            if (field_0x14D1 == 0 && field_0x14D0 == 1) {
                if (mMotion == 5) {
                    if (mMode == 0) {
                        setMotion(0x18, -1.0f, 0);
                    } else {
                        setMotion(0x17, -1.0f, 0);
                    }
                    mAcch.SetWallNone();
                }
                setLookMode(0);
                if (field_0x14D8 != 0x17) {
                    cLib_chaseAngleS(&current.angle.y, home.angle.y, 0x400);
                    setAngle(current.angle.y);
                    cLib_chasePosXZ(&current.pos, field_0x148C, 30.f / 11.f);
                    if (mAnm_p->isStop()) {
                        field_0x14D1 = 1;
                        setMotion(5, -1.0f, 0);
                        home.pos = field_0x148C;
                        home.angle = field_0x1498;
                    } else {
                        return TRUE;
                    }
                } else {
                    return TRUE;
                }
            }
        }
        if (mDamageTimer == 0) {
            if (mNpcfActorManager[0].getActorP() != NULL) {
                if (!chkFindPlayer()) {
                    mTurnMode = 0;
                }
            } else if (chkFindPlayer()) {
                mTurnMode = 0;
            }
            if (mNpcfActorManager[0].getActorP() != NULL) {
                setLookMode(2);
            } else {
                setLookMode(0);
                if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 0xf)) {
                    field_0x1472 = 0;
                }
            }
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::waitKickOut(void*) {
    switch (field_0x1472) {
    case 0:
        setExpression(0x17, -1.0f);
        setMotion(1, 0.0f, 0);
        setLookMode(0);
        mTurnMode = 0;
        field_0x1694 = 0;
        field_0x1472 = 2;
        // fallthrough
    case 2:
        field_0x1693 = field_0x1692;
        field_0x1692 = 0;
        fpcM_Search(s_subCarry, this);
        if (field_0x1693 > field_0x1692) {
            if (field_0x1690 == 1) {
                mOrderEvtNo = 4;
            } else {
                mOrderEvtNo = 5;
            }
        } else if (daPy_getPlayerActorClass()->current.pos.y - current.pos.y < 200.0f &&
                   daPy_getPlayerActorClass()->checkPlayerFly() == 0)
        {
            mOrderEvtNo = 3;
        } else if (daPy_getPlayerActorClass()->checkWolfRopeHang() &&
                   chkActorInSight(daPy_getPlayerActorClass(), GET_HIO(mNpcFParams.fov)))
        {
            mOrderEvtNo = 3;
        } else if (field_0xDD4.getPathInfo() != NULL) {
            cXyz c;
            if (field_0x1694 == 0) {
                BOOL b = field_0xDD4.getDstPos(current.pos, c);
                if (b) {
                    field_0xDD4.reverse();
                    setExpression(0x17, -1.0f);
                    setMotion(0, -1.0f, 0);
                    speedF = 0.0f;
                    setLookMode(7);
                    field_0x1694 = 0xb4;
                    break;
                } else {
                    s16 r27 = cLib_targetAngleY(&current.pos, &c);
                    speedF = 5.0f;
                    mAnm_p->setPlaySpeed(speedF * GET_HIO(mWalkingAnimationSpeed));
                    cLib_addCalcAngleS2(&current.angle.y, r27, GET_HIO(mWalkingRotationDivisions),
                                        GET_HIO(mWalkingRotationSpeed));
                    setAngle(current.angle.y);
                }
            } else if (cLib_calcTimer(&field_0x1694) == 0) {
                setExpression(0x17, -1.0f);
                setMotion(1, -1.0f, 0);
            } else if (field_0x1694 == 0x78) {
                setLookMode(8);
            } else if (field_0x1694 == 0x3c) {
                setLookMode(0);
            }
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::moveRock(void*) {
    switch (field_0x1472) {
    case 0:
        setExpression(0x17, 0.0f);
        setMotion(0x13, 0.0f, 0);
        setLookMode(0);
        mTurnMode = 0;
        mAcch.SetGrndNone();
        mAcch.SetWallNone();
        field_0x1472 = 2;
        // fallthrough
    case 2:
        gravity = 0.0f;
        mOrderEvtNo = 6;
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::spaWarm(void*) {
    switch (field_0x1472) {
    case 0:
        if (getMode() == 1) {
            setExpressionAnm(0xa, 1);
            setMotionAnm(0x24, 0);
        } else {
            setExpressionAnm(0x8, 1);
            setMotionAnm(0x22, 0);
        }
        setLookMode(0);
        mTurnMode = 0;
        field_0x1472 = 2;
        field_0x1478 = cLib_getRndValue(3, 7);
        field_0x9ea = 1;
        // fallthrough
    case 2:
        if (field_0x1488 == 1) {
            fopAc_ac_c* p = (fopAc_ac_c*)fpcM_Search(s_subShop, this);
            if (p != NULL) {
                mNpcfActorManager[2].entry(p);
                field_0x1488 = 0;
            }
        }

        if (mDamageTimer == 0) {
            if (field_0x1487 == 1) {
                setLookMode(5);
            } else {
                if (mNpcfActorManager[0].getActorP() != NULL) {
                    if (!chkFindPlayer()) {
                        mTurnMode = 0;
                    }
                } else if (chkFindPlayer()) {
                    mTurnMode = 0;
                }

                if (mNpcfActorManager[0].getActorP() != NULL) {
                    setLookMode(2);
                } else {
                    setLookMode(0);

                    if (home.angle.y != mCurAngle.y && step(home.angle.y, -1, -1, 0xf)) {
                        field_0x1472 = 0;
                    }
                }
            }
        }

        if (getMode() == 1) {
            if (mMotionLoops >= field_0x1478) {
                setMotion(0xe, 0.0f, 1);
                setExpression(0xe, -1.0f);
                field_0x1478 = cLib_getRndValue(3, 7);
            }
        } else if (mMotionLoops >= field_0x1478) {
            setMotion(0xc, 0.0f, 1);
            setExpression(0xc, -1.0f);
            field_0x1478 = cLib_getRndValue(3, 7);
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::waitSpaWater(void*) {
    switch (field_0x1472) {
    case 0:
        if (field_0x1690 == 2) {
            setExpression(0x16, -1.0f);
            setMotion(0x9, -1.0f, 0);
        } else {
            setExpression(0x16, -1.0f);
            setMotion(0x2, -1.0f, 0);
        }
        setLookMode(0);
        mTurnMode = 0;
        field_0x146C = 0x28;
        field_0x1472 = 2;
        field_0x9ea = 1;
        // fallthrough
    case 2:
        if (field_0x1488 == 1) {
            daOnsTaru_c* p = (daOnsTaru_c*)fpcM_Search(s_subOnsenTaru, this);
            if (p != NULL) {
                if (!daPy_getPlayerActorClass()->setForceGrab(p, 1, 0)) {
                    break;
                }
                p->startTimer();
                mNpcfActorManager[2].entry(p);
                field_0x1488 = 0;
            }
        }

        if (mNpcfActorManager[2].getActorP() != NULL && !dComIfGp_event_runCheck()) {
            daOnsTaru_c* p = (daOnsTaru_c*)mNpcfActorManager[2].getActorP();
            if (fopAcM_GetName(p) == 0x16c) {
                if (!p->getTempStat()) {
                    dMeter2Info_setFloatingMessage(0x7fd, 0x96, 0);
                    mNpcfActorManager[2].initialize();
                } else if (field_0x1690 == 0) {
                    fopAc_ac_c* g = NULL;
                    s32 r = fopAcM_SearchByID(daPy_getPlayerActorClass()->getGrabActorID(), &g);
                    if (r == 1 && p == g) {
                        attention_info.flags = 0;
                        if ((attention_info.position -
                             daPy_getPlayerActorClass()->attention_info.position)
                                .absXZ() <= 1200.0f)
                        {
                            mOrderEvtNo = 9;
                            break;
                        }
                    }
                }
            }
        }

        if (field_0xC98.ChkTgHit()) {
            daOnsTaru_c* hit = (daOnsTaru_c*)field_0xC98.GetTgHitAc();
            if (fopAcM_GetName(hit) == 0x16c) {
                fopAcM_OnStatus(hit, 0x4000);
                if (hit->getTempStat()) {
                    mOrderEvtNo = 7;
                } else {
                    mOrderEvtNo = 8;
                }
                break;
            }
        }

        if (mDamageTimer == 0) {
            if (field_0x1690 == 2) {
                setLookMode(0);
            } else {
                if (mNpcfActorManager[0].getActorP() != NULL) {
                    if (!chkFindPlayer()) {
                        mTurnMode = 0;
                    }
                } else if (chkFindPlayer()) {
                    mTurnMode = 0;
                }

                if (mNpcfActorManager[0].getActorP() != NULL) {
                    setLookMode(2);
                } else {
                    setLookMode(0);
                }
            }
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::waitSpaBuyer(void*) {
    switch (field_0x1472) {
    case 0:
        setExpression(0x17, -1.0f);
        setMotion(0x10, -1.0f, 0);
        setLookMode(0);
        mTurnMode = 0;
        field_0x1478 = cLib_getRndValue(3, 7);
        field_0x9ea = 1;
        field_0x1472 = 2;
        // fallthrough
    case 2:
        if (mDamageTimer == 0) {
            if (field_0x14D8 == 0x31) {
                mNpcfActorManager[0].remove();
            } else if (mNpcfActorManager[0].getActorP()) {
                if (!chkFindPlayer()) {
                    mTurnMode = 0;
                }
            } else {
                if (chkFindPlayer()) {
                    mTurnMode = 0;
                }
            }

            if (mNpcfActorManager[0].getActorP()) {
                setLookMode(2);
                mMotionLoops = 0;
                field_0x1478 = cLib_getRndValue(1, 7);
            } else {
                setLookMode(0);
                if (home.angle.y != mCurAngle.y) {
                    if (step(home.angle.y, 0x17, 0x16, 0xf)) {
                        field_0x1472 = 0;
                    }
                }
            }
        }

        if (mNpcfActorManager[0].getActorP() == NULL && mMotionLoops >= field_0x1478) {
            setMotion(0x11, -1.0f, 1);
            setExpression(0x17, -1.0f);
            field_0x1478 = cLib_getRndValue(3, 7);
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::waitSpaBuyerTW(void*) {
    switch (field_0x1472) {
    case 0:
        setExpression(0x17, -1.0f);
        setMotion(0x10, -1.0f, 0);
        setLookMode(0);
        mTurnMode = 0;
        field_0x1478 =  cLib_getRndValue(3, 7);
        field_0x9ea = 1;
        field_0x1472 = 2;
    case 2:
        if (mDamageTimer == 0) {
            setLookMode(0);
            if (home.angle.y != mCurAngle.y) {
                if (step(home.angle.y, 0x17, 0x16, 0xf)) {
                    field_0x1472 = 0;
                }
            }
        }
        if (mMotionLoops >= field_0x1478) {
            setMotion(0x11, -1.0f, 1);
            setExpression(0x17, -1.0f);
            field_0x1478 = cLib_getRndValue(3, 7);
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::beforeCrashWait(void*) {
    switch (field_0x1472) {
    case 0:
        setExpression(0x14, -1.0f);
        setMotion(0x1a, -1.0f, 0);
        setLookMode(0);
        mTurnMode = 0;
        field_0x1472 = 2;
        // fallthrough
    case 2:
        if (field_0x1488 == 1) {
            fopAc_ac_c* p = (fopAc_ac_c*)fpcM_Search(s_subCrashed, this);
            if (p != NULL) {
                home.pos = current.pos = old.pos = p->current.pos;
                setAngle(p->current.angle.y);
                home.angle.y = current.angle.y;
                field_0x1488 = 0;
            }
        }

        if (mDamageTimer == 0) {
            if (mNpcfActorManager[0].getActorP()) {
                if (!chkFindPlayer()) {
                    mTurnMode = 0;
                }
            } else {
                if (chkFindPlayer()) {
                    mTurnMode = 0;
                }
            }

            if (mNpcfActorManager[0].getActorP()) {
                setLookMode(2);
                setExpression(0x17, -1.0f);
                setMotion(0, -1.0f, 0);
            } else {
                setLookMode(0);
                if (home.angle.y != mCurAngle.y) {
                    if (step(home.angle.y, 0x17, 0x16, 0xf)) {
                        field_0x1472 = 0;
                    }
                } else {
                    setExpression(0x14, -1.0f);
                    setMotion(0x1a, -1.0f, 0);
                }
            }
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::crashRoll(void*) {
   switch (field_0x1472) {
    case 0:
        setExpression(0x17, -1.0f);
        setMotion(0x13, -1.0f, 0);
        setLookMode(0);
        mTurnMode = 0;
        speedF = 50.0f;
        field_0x1472 = 2;
        // fallthrough
    case 2:
        if (mAcch.ChkWallHit()) {
            field_0x1486 = 2;
        }
        field_0x14D4 = 5.0f;
        mAnm_p->setPlaySpeed(field_0x14D4);
        mAnm_p->setEndFrame(29.0f);
        if (field_0x14D8 == 0x1a) {
            if (field_0x14D4 >= 1.0f) {
                setRollPrtcl(current.pos, 1.0f);
            }
            f32 a = cLib_minMaxLimit(fabsf(field_0x14D4) * 20.0f, 1.0f, 127.0f);
            u32 b = a;
            mCreature.startCreatureSound(Z2SE_GORON_ROLLING, b, -1);
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::crashRollWait(void*) {
    switch (field_0x1472) {
    case 0:
        setExpression(0x17, -1.0f);
        setMotion(0x13, -1.0f, 0);
        setLookMode(0);
        mTurnMode = 0;
        speedF = 0.0f;
        field_0x1472 = 2;
        // fallthrough
    case 2:
        field_0x14D4 = 5.0f;
        mAnm_p->setPlaySpeed(field_0x14D4);
        mAnm_p->setEndFrame(29.0f);
        if (field_0x14D8 == 0x1a) {
            if (field_0x14D4 >= 1.0f) {
                cXyz c(0.0f, 0.0f, 0.0f);
                mDoMtx_stack_c::YrotS(shape_angle.y);
                mDoMtx_stack_c::multVec(&c, &c);
                c += current.pos;
                setRollPrtcl(c, GET_HIO(mParticleSize));
            }
            f32 a = cLib_minMaxLimit(fabsf(field_0x14D4) * 20.0f, 1.0f, 127.0f);
            u32 b = a;
            mCreature.startCreatureSound(Z2SE_GORON_ROLLING, b, -1);
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::crashedWait(void*) {
    switch (field_0x1472) {
    case 0:
        setExpression(0x17, -1.0f);
        setMotion(0, -1.0f, 0);
        setLookMode(0);
        mTurnMode = 0;
        field_0x1472 = 2;
        // fallthrough
    case 2:
        if (field_0x1488 == 1) {
            fopAc_ac_c* found = (fopAc_ac_c*)fpcM_Search(s_subCrashed, this);
            if (found) {
                home.pos = current.pos = old.pos = found->current.pos;
                setAngle(found->current.angle.y);
                home.angle.y = current.angle.y;
                field_0x1488 = 0;
            }
        }
        if (mDamageTimer == 0) {
            if (mNpcfActorManager[0].getActorP() != NULL) {
                if (chkFindPlayer() == 0) {
                    mTurnMode = 0;
                }
            } else if (chkFindPlayer() != 0) {
                mTurnMode = 0;
            }
            if (mNpcfActorManager[0].getActorP() != NULL) {
                setLookMode(2);
            } else {
                setLookMode(0);
                if (home.angle.y != mCurAngle.y && step(home.angle.y, 0x17, 0x16, 0xf)) {
                    field_0x1472 = 0;
                }
            }
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::waitBuyer(void*) {
    switch (field_0x1472) {
    case 0:
        if (field_0x1691 == 0 && home.angle.y == mCurAngle.y) {
            setExpressionAnm(0, 1.0f);
            setMotion(5, -1.0f, 0);
        }
        mTurnMode = 0;
        field_0x1472 = 2;
        // fallthrough
    case 2:
        if (mDamageTimer == 0) {
            if (mNpcfActorManager[0].getActorP() != NULL) {
                if (chkFindPlayer() == 0) {
                    mTurnMode = 0;
                }
            } else if (chkFindPlayer() != 0) {
                mTurnMode = 0;
            }
            if (mNpcfActorManager[0].getActorP() != NULL) {
                setLookMode(2);
            } else {
                setLookMode(0);
                if (home.angle.y != mCurAngle.y && step(home.angle.y, 0x17, 0x16, 0xf)) {
                    setExpression(0x17, -1.0f);
                    setMotion(0, -1.0f, 0);
                    field_0x1472 = 0;
                }
            }
        }
        break;
    case 3:
        break;
    }
    return TRUE;
}

BOOL daNpc_grA_c::talk(void*) {
    int spc = 0;
    BOOL r29 = 0;
    int r27 = 0;
    BOOL r26 = 0;
    int sp8 = 0;
    switch (field_0x1472)
    {
    case 0:
        if (mDamageTimer != 0)
            break;
        if (mType == 4 && getMode1() == 1 &&
            dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this)) == 0)
        {
            r27 = 0xa;
            if (daNpcF_chkEvtBit(0x3e)) {
                dComIfGs_onSwitch(mSwBit, fopAcM_GetRoomNo(this));
                setGateWalk();
            }
        } else {
            r27 = field_0x146C;
        }
        initTalk(r27, NULL);
        field_0x1484 = fopAcM_searchPlayerAngleY(this);
        if (mType == 0xb) {
            if (field_0x1691 == 0) {
                daNpcF_offTmpBit(0xb);
                daNpcF_offTmpBit(0xc);
            }
        } else if (mType == 0xa) {
            if (field_0x1486 == 0) {
                daNpcF_offTmpBit(0xb);
            }
        }
        mTurnMode = 0;
        field_0x1691 = 0;
        field_0x1472 = 2;
        // fallthrough
    case 2:
        if (field_0x9ea || mTwilight || mType == 5) {
            r26 = 1;
            if (mType == 5) {
                setLookMode(0);
            } else if (!mTwilight && mType != 5) {
                if (mType != 7 || field_0x1690 != 2) {
                    setLookMode(2);
                }else{

                }
            }
        } else {
            setLookMode(3);
            mNpcfActorManager[0].entry(daPy_getPlayerActorClass());
            if (field_0x1484 == mCurAngle.y) {
                r26 = 1;
            } else if (step(field_0x1484, 0x17, 0x16, 0xf)) {
                setExpression(0x17, -1.0f);
                if (mType == 8) {
                    setMotion(0x10, -1.0f, 0);
                } else {
                    setMotion(0, -1.0f, 0);
                }
                mTurnMode = 0;
            }
        }
        if (r26 && talkProc(NULL, TRUE, NULL)) {
            if (mFlow.getEventId(&sp8) == 1) {
                field_0x1480 =
                    fopAcM_createItemForPresentDemo(&current.pos, sp8, 0, -1, -1, NULL, NULL);
                if (field_0x1480 != fpcM_ERROR_PROCESS_ID_e) {
                    s16 r25 = dComIfGp_getEventManager().getEventIdx(this, "DEFAULT_GETITEM", 0xff);
                    dComIfGp_getEvent()->reset(this);
                    fopAcM_orderChangeEventId(this, r25, 1, -1);
                    field_0x9ec = 1;
                    r29 = 1;
                    mOrderNewEvt = 1;
                    if (mType == 0xb) {
                        field_0x1691 = 1;
                    }
                }
            } else {
                if (mType == 0xa && field_0x1486 == 0 && daNpcF_chkEvtBit(0x187)) {
                    dComIfGp_getEvent()->reset(this);
                    field_0x9ec = 1;
                    mOrderNewEvt = 1;
                    mOrderEvtNo = 0xa;
                    changeEvent(l_resNames[l_evtGetParamList[mOrderEvtNo][1]],
                                l_evtNames[l_evtGetParamList[mOrderEvtNo][0]], 1, -1);
                }
                r29 = 1;
            }
        }
        if (r29) {
            field_0x1472 = 3;
            if (field_0x9ec == 0) {
                dComIfGp_event_reset();
            }
            field_0x9ec = 0;
        }

        break;
    case 3:
        break;
    }
    return r29;
}

BOOL daNpc_grA_c::test(void*) {
    switch (field_0x1472) {
    case 0:
        speedF = 0.0f;
        speed.setall(0.0f);
        field_0x1472 = 2;
        // fallthrough

    case 2:
        if (GET_HIO(mNpcFParams.face_expression) != mExpression) {
            setExpression(GET_HIO(mNpcFParams.face_expression), GET_HIO(mNpcFParams.morf_frame));
        }
        setMotion(GET_HIO(mNpcFParams.motion), GET_HIO(mNpcFParams.morf_frame), 0);
        setLookMode(GET_HIO(mNpcFParams.look_mode));
        mOrderEvtNo = 0;
        attention_info.flags = 0;
    case 1:
    case 3:
        break;
    }
    return TRUE;
}

static BOOL daNpc_grA_Create(void* i_this) {
    return ((daNpc_grA_c*)i_this)->create();
}

static BOOL daNpc_grA_Delete(void* i_this) {
    return ((daNpc_grA_c*)i_this)->Delete();
}

static BOOL daNpc_grA_Execute(void* i_this) {
    return ((daNpc_grA_c*)i_this)->Execute();
}

static BOOL daNpc_grA_Draw(void* i_this) {
    return ((daNpc_grA_c*)i_this)->Draw();
}

static BOOL daNpc_grA_IsDelete(void* i_this) {
    return true;
}

static actor_method_class daNpc_grA_MethodTable = {
    (process_method_func)daNpc_grA_Create,
    (process_method_func)daNpc_grA_Delete,
    (process_method_func)daNpc_grA_Execute,
    (process_method_func)daNpc_grA_IsDelete,
    (process_method_func)daNpc_grA_Draw,
};

actor_process_profile_definition g_profile_NPC_GRA = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_GRA,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpc_grA_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  306,                    // mPriority
  &daNpc_grA_MethodTable, // sub_method
  0x00044100,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
