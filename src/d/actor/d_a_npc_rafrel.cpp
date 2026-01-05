/**
 * @file d_a_npc_rafrel.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_rafrel.h"
#include "d/actor/d_a_npc.h"
#include "d/d_debug_viewer.h"

static daNpc_GetParam2 l_bckGetParamList[29] = {
    {-1, 2, 0},
    {9, 0, 0},
    {7, 0, 0},
    {10, 0, 0},
    {8, 0, 0},
    {11, 2, 0},
    {13, 2, 0},
    {12, 2, 0},
    {14, 2, 0},
    {3, 2, 2},
    {4, 0, 2},
    {5, 0, 2},
    {6, 2, 2},
    {12, 0, 1},
    {13, 0, 1},
    {14, 0, 1},
    {15, 2, 1},
    {18, 2, 1},
    {11, 2, 1},
    {10, 2, 1},
    {8, 0, 1},
    {9, 2, 1},
    {6, 0, 1},
    {7, 2, 1},
    {16, 0, 1},
    {17, 2, 1},
    {6, 2, 3},
    {4, 0, 3},
    {5, 2, 3},
};

static daNpc_GetParam2 l_btpGetParamList[4] = {
    {23, 2, 0},
    {24, 0, 0},
    {25, 2, 0},
    {30, 2, 0},
};
static daNpc_GetParam2 l_btkGetParamList[5] = {
    {20, 2, 0},
    {24, 0, 1},
    {25, 2, 1},
    {26, 0, 1},
    {27, 2, 1},
};

static int l_loadRes_RAFREL0[] = {
    0,
    2,
    -1,
};

static int l_loadRes_RAFREL1[] = {
    0,
    1, 
    -1,
};

static int l_loadRes_RAFREL2[] = {
    0,
    3,
    -1,
};

static int* l_loadRes_list[3] = {
    l_loadRes_RAFREL0,
    l_loadRes_RAFREL1,
    l_loadRes_RAFREL2,
};

static char* l_arcNames[4] = {
    "Rafrel",
    "Rafrel1",
    "Rafrel2",
    "Rafrel3",
};

static char* l_evtNames[7] = {
    "",
    "THE_INTRODUCTION",
    "RESIST_MEETING",
    "RAFREL_APPEAR",
    "RESIST_WIRETAP_ASH",
    "RESIST_WIRETAP_SHAD",
    "RESIST_WIRETAP_RAFREL",
};

const daNpcRafrel_HIOParam daNpcRafrel_Param_c::m = {
    35.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    200.0f,
    35.0f,
    40.0f,
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
    4,
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
    0,
    0,
    320.0f,
    0,
};

static char* l_myName = "Rafrel";

#if DEBUG
daNpcRafrel_HIO_c::daNpcRafrel_HIO_c() {
    m = daNpcRafrel_Param_c::m;
}

void daNpcRafrel_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genSlider("デモ開始距離", &m.demo_start_dist, 0.0f, 500.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genCheckBox("デモ開始エリア", &m.demo_start_area, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    daNpcF_commonGenMessage(ctx, &m.common);
}
#endif

static NPC_RAFREL_HIO_CLASS l_HIO;

typedef BOOL (daNpcRafrel_c::*EventFn)(int);
EventFn daNpcRafrel_c::mEvtSeqList[] = {
    NULL,
    &daNpcRafrel_c::EvCut_Introduction,
    &daNpcRafrel_c::EvCut_Meeting,
    &daNpcRafrel_c::EvCut_Appear,
    &daNpcRafrel_c::EvCut_WiretapEntrant,
    &daNpcRafrel_c::EvCut_WiretapEntrant,
    &daNpcRafrel_c::EvCut_WiretapSponsor,
};

daNpcRafrel_c::daNpcRafrel_c() {}

daNpcRafrel_c::~daNpcRafrel_c() {
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        dComIfG_resDelete(&mPhase[i], l_arcNames[l_loadRes_list[mType][i]]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }

    #if DEBUG
    if (mpHIO != NULL) {
        mpHIO->removeHIO();
    }
    #endif
}

int daNpcRafrel_c::Create() {
    fopAcM_ct(this, daNpcRafrel_c);

    if (strcmp(dComIfGp_getStartStageName(), "R_SP116") == 0 && dComIfG_play_c::getLayerNo(0) == 4) {
        if (!daNpcF_chkEvtBit(0x169)) {
            return cPhs_ERROR_e;
        }

        if (dComIfGs_isStageSwitch(24, 0x4B)) {
            return cPhs_ERROR_e;
        }

        mType = 0;
    } else if (strcmp(dComIfGp_getStartStageName(), "F_SP115") == 0 && dComIfGp_getStartStageRoomNo() == 0) {
        if (daNpcF_chkEvtBit(0x169) || !daNpcF_chkEvtBit(0x108)) {
            return cPhs_ERROR_e;
        }

        mType = 1;
    } else if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
        if (dComIfGs_isSaveSwitch(0x14)) {
            return cPhs_ERROR_e;
        }

        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
        mType = 2;
    }

    field_0xe0c = getMessageNo();

    int phase_state = cPhs_ERROR_e;
    for (int i = 0; l_loadRes_list[mType][i] >= 0; i++) {
        phase_state = dComIfG_resLoad(&mPhase[i], l_arcNames[l_loadRes_list[mType][i]]);
        if (phase_state != cPhs_COMPLEATE_e) {
            return phase_state;
        }
    }

    if (phase_state == cPhs_COMPLEATE_e) {
        u32 heapsize = 0;
        switch (mType) {
        case 0:
            heapsize = 0x5CA0;
            break;
        case 1:
            heapsize = 0x6490;
            break;
        case 2:
            heapsize = 0x6490;
            break;
        default:
            JUT_ASSERT(343, FALSE);
            break;
        }

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapsize)) {
            return cPhs_ERROR_e;
        }

        J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -160.0f, -50.0f, -160.0f, 160.0f, 220.0f, 160.0f);

        mSound.init(&current.pos, &eyePos, 3, 1);

        #if DEBUG
        mpHIO = &l_HIO;
        mpHIO->entryHIO("ラフレル");
        #endif

        mAcchCir.SetWall(mpHIO->m.common.width, mpHIO->m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.SetRoofNone();
        mAcch.SetWaterNone();
        mAcch.CrrPos(dComIfG_Bgsp());

        mCcStts.Init(mpHIO->m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgType(0);
        mCyl.SetTgSPrm(0);

        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        setEnvTevColor();
        setRoomNo();
        mAnm_p->modelCalc();

        reset();
        Execute();
    }

    return phase_state;
}

int daNpcRafrel_c::CreateHeap() {
    void* mdlData_p = NULL;
    J3DModel* mdl_p = NULL;

    mdlData_p = dComIfG_getObjectRes(l_arcNames[0], 0x11);
    JUT_ASSERT(403, NULL != mdlData_p);

    u32 mdl_diff_flags = 0x11020284;
    mAnm_p = new mDoExt_McaMorfSO((J3DModelData*)mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, J3DMdlFlag_DifferedDLBuffer, mdl_diff_flags);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    mdl_p = mAnm_p->getModel();

    for (u16 i = 0; i < ((J3DModelData*)mdlData_p)->getJointNum(); i++) {
        ((J3DModelData*)mdlData_p)->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    mdl_p->setUserArea((uintptr_t)this);

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    mpItemModel = NULL;

    if (mType == 1) {
        mdlData_p = dComIfG_getObjectRes(l_arcNames[1], 0x15);
        JUT_ASSERT(438, mdlData_p != NULL);
  
        mpItemModel = mDoExt_J3DModel__create((J3DModelData*)mdlData_p, J3DMdlFlag_DifferedDLBuffer, BMD_DEFAULT_DIFF_FLAGS);
        if (mpItemModel == NULL) {
            return 0;
        }
    } else if (mType == 2) {
        mdlData_p = dComIfG_getObjectRes(l_arcNames[3], 9);
        JUT_ASSERT(445, mdlData_p != NULL);

        mpItemModel = mDoExt_J3DModel__create((J3DModelData*)mdlData_p, J3DMdlFlag_DifferedDLBuffer, BMD_DEFAULT_DIFF_FLAGS);
        if (mpItemModel == NULL) {
            return 0;
        }
    }

    if (!setExpressionAnm(1, 0)) {
        return 0;
    }

    switch (mType) {
    case 0:
        setMotionAnm(9, 0.0f);
        break;
    case 1:
        setMotionAnm(8, 0.0f);
        break;
    case 2:
        setMotionAnm(0x1A, 0.0f);
        break;
    default:
        JUT_ASSERT(460, FALSE);
        break;
    }

    return 1;
}

int daNpcRafrel_c::Delete() {
    "Delete -> NPC_RAFREL(id=%d)\n";
    
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpcRafrel_c();
    return 1;
}

int daNpcRafrel_c::Execute() {
    execute();
    return 1;
}

int daNpcRafrel_c::Draw() {
    J3DModelData* modelData = mAnm_p->getModel()->getModelData();
    modelData->getMaterialNodePointer(1)->setMaterialAnm(mpMatAnm);
    draw(FALSE, FALSE, mpHIO->m.common.real_shadow_size, NULL, FALSE);

    if (mpItemModel != NULL) {
        enum { JNT_19 = 19, JNT_20 = 20};
        int item_jnt_no = mType == 1 ? JNT_20 : JNT_19;
 
        g_env_light.setLightTevColorType_MAJI(mpItemModel, &tevStr);
        mpItemModel->setBaseTRMtx(mAnm_p->getModel()->getAnmMtx(item_jnt_no));
        mDoExt_modelUpdateDL(mpItemModel);
    }

    return 1;
}

int daNpcRafrel_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jnt_no = i_joint->getJntNo();
    u16 jointNo = jnt_no;
    int spC[] = {1, 3, 4};

    if (jnt_no == 0) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);

        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);

        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));

    switch (jnt_no) {
    case 1:
    case 3:
    case 4:
        setLookatMtx(jnt_no, spC, mpHIO->m.common.neck_rotation_ratio);
    case 2:
    default:
        i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
        cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

        if ((jnt_no == 4 || jnt_no == 9) && (mAnmFlags & 0x100)) {
            J3DAnmTransform* anm = mBckAnm.getBckAnm();
            mBckAnm.changeBckOnly(mAnm_p->getAnm());
            mAnm_p->changeAnm(anm);
        }
    }

    return 1;
}

int daNpcRafrel_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpcRafrel_c* actor = (daNpcRafrel_c*)i_this;
    return actor->CreateHeap();
}

int daNpcRafrel_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == NULL) {
        J3DModel* mdl_p = j3dSys.getModel();
        daNpcRafrel_c* i_this = (daNpcRafrel_c*)mdl_p->getUserArea();

        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, mdl_p);
        }
    }

    return 1;
}

BOOL daNpcRafrel_c::main() {
    if (field_0xde0 != NULL) {
        (this->*field_0xde0)(NULL);
    }

    if (field_0xe00 != fpcM_ERROR_PROCESS_ID_e && dComIfGp_event_runCheck()) {
        if (strcmp(dComIfGp_getEventManager().getRunEventName(), "DEFAULT_GETITEM") == 0) {
            dComIfGp_event_setItemPartnerId(field_0xe00);
            field_0xe00 = -1;
        }
    }

    playExpression();
    playMotion();
    return 1;
}

bool daNpcRafrel_c::setExpressionAnm(int i_idx, bool i_modify) {
    mAnmFlags &= ~ANM_EXPRESSION_FLAGS;

    J3DAnmTransform* bckAnm = l_bckGetParamList[i_idx].fileIdx >= 0 ?
                              getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[i_idx].arcIdx], l_bckGetParamList[i_idx].fileIdx) : NULL;

    s32 attr = l_bckGetParamList[i_idx].attr;
    bool res = false;
    switch (i_idx) {
    case 0:
        res = setExpressionBtp(0);
        break;
    case 1:
        res = setExpressionBtp(0);
        break;
    case 2:
        res = setExpressionBtp(0);
        break;
    case 5:
        res = setExpressionBtp(0);
        break;
    case 3:
        res = setExpressionBtp(1);
        break;
    case 6:
        res = setExpressionBtp(2);
        break;
    case 4:
        res = setExpressionBtp(0);
        break;
    case 7:
        res = setExpressionBtp(0);
        break;
    default:
        bckAnm = NULL;
        break;
    }

    if (!res) {
        return false;
    }

    if (bckAnm == NULL) {
        return true;
    }

    if (setBckAnm(bckAnm, 1.0f, attr, 0, -1, i_modify)) {
        mAnmFlags |= (ANM_PAUSE_BCK | ANM_PLAY_BCK);
        mExpressionLoops = 0;
        return true;
    }

    OS_REPORT("%s: 表情Bckアニメーションの登録に失敗しました！\n", __FILE__);
    return false;
}

bool daNpcRafrel_c::setExpressionBtp(int i_idx) {
    J3DAnmTexPattern* btpAnm = getTexPtrnAnmP(l_arcNames[l_btpGetParamList[i_idx].arcIdx],
                                              l_btpGetParamList[i_idx].fileIdx);
    int attr = l_btpGetParamList[i_idx].attr;
    mAnmFlags &= ~(ANM_PAUSE_BTP | ANM_PLAY_BTP | ANM_FLAG_800);
    if (btpAnm == NULL) {
        return true;
    }

    if (setBtpAnm(btpAnm, mAnm_p->getModel()->getModelData(), 1.0f, attr)) {
        mAnmFlags |= ANM_PAUSE_BTP | ANM_PLAY_BTP;
        if (i_idx == 0) {
            mAnmFlags |= ANM_FLAG_800;
        }
        return true;
    }

    OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", __FILE__);
    return false;
}

void daNpcRafrel_c::setMotionAnm(int i_idx, f32 i_morf) {
    int var_r30 = 0;
    switch (i_idx) {
    case 22:
        var_r30 = 1;
        break;
    case 23:
        var_r30 = 2;
        break;
    case 24:
        var_r30 = 3;
        break;
    case 25:
        var_r30 = 4;
        break;
    }

    J3DAnmTransformKey* morfAnm = getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[i_idx].arcIdx],
                                                    l_bckGetParamList[i_idx].fileIdx);
    J3DAnmTextureSRTKey* btkAnm = getTexSRTKeyAnmP(l_arcNames[l_btkGetParamList[var_r30].arcIdx],
                                                   l_btkGetParamList[var_r30].fileIdx);
    int morfAttr = l_bckGetParamList[i_idx].attr;
    int btkAttr = l_btkGetParamList[var_r30].attr;

    mAnmFlags &= ~ANM_MOTION_FLAGS;

    if (i_idx == 0x13) {
        setExpressionBtp(3);
    }

    if (morfAnm != NULL) {
        if (setMcaMorfAnm(morfAnm, 1.0f, i_morf, morfAttr, 0, -1)) {
            mAnmFlags |= ANM_PAUSE_MORF | ANM_PLAY_MORF;
            mMotionLoops = 0;
        }
    }

    if (btkAnm != NULL) {
        if (setBtkAnm(btkAnm, mAnm_p->getModel()->getModelData(), 1.0f, btkAttr)) {
            mAnmFlags |= ANM_PAUSE_BTK | ANM_PLAY_BTK;
        }
    }
}

void daNpcRafrel_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mLookat.initialize();

    for (int i = 0; i < 4; i++) {
        field_0xc7c[i].initialize();
    }

    field_0xe04 = 0;
    field_0xe08 = 0;
    field_0xde0 = NULL;
    field_0xe0e = -1;
    field_0xe10 = 0;

    current.pos.set(home.pos);
    old.pos.set(current.pos);

    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;

    speedF = 0.0f;
    speed.setall(0.0f);

    field_0xe00 = -1;
    mOrderEvtNo = 0;
    mExpressionMorfOverride = 0.0f;
    mMotionMorfOverride = 0.0f;

    field_0xe15 = daNpcF_chkEvtBit(0x130) != 0;
    field_0xe14 = 0;

    J3DModelData* mdlData_p = mAnm_p->getModel()->getModelData();
    if (mType == 1) {
        mdlData_p->getMaterialNodePointer(5)->getShape()->show();
        mdlData_p->getMaterialNodePointer(4)->getShape()->hide();
        mdlData_p->getMaterialNodePointer(6)->getShape()->hide();
    } else {
        mdlData_p->getMaterialNodePointer(5)->getShape()->hide();
        mdlData_p->getMaterialNodePointer(4)->getShape()->show();
    }

    if (isSneaking()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
    }

    setWaitAction();
}

void daNpcRafrel_c::setParam() {
    searchActors();
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    if (isSneaking()) {
        attention_info.distances[fopAc_attn_LOCK_e] = 78;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = 77;
        attention_info.flags |= fopAc_AttnFlag_UNK_0x800000;
    } else if (mType == 0) {
        attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(3, 5);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(2, 5);
    } else {
        attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(mpHIO->m.common.attention_distance, mpHIO->m.common.attention_angle);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(mpHIO->m.common.talk_distance, mpHIO->m.common.talk_angle);
    
        if (mType == 1 && field_0xe15 != 0 && field_0xe16 == 0) {
            Vec sp8 = {current.pos.x + 150.0f, current.pos.y, current.pos.z - 100.0f};
            if (fopAcM_GetPosition(daPy_getPlayerActorClass()).abs(sp8) > 160.0f) {
                attention_info.flags = 0;
            }
        }
    }

    #if DEBUG
    mAcchCir.SetWallR(mpHIO->m.common.width);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    gravity = mpHIO->m.common.gravity;
    #endif
}

BOOL daNpcRafrel_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* temp_r29 = NULL;
        temp_r29 = getTexSRTKeyAnmP(l_arcNames[l_btkGetParamList[0].arcIdx], l_btkGetParamList[0].fileIdx);
        if (temp_r29 == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.2f * -1.0f);
            mpMatAnm->setNowOffsetY(cM_ssin(mEyeAngle.x) * 0.2f);
            mpMatAnm->onEyeMoveFlag();
            return TRUE;
        } else {
            mpMatAnm->offEyeMoveFlag();
        }
    }

    return FALSE;
}

BOOL daNpcRafrel_c::drawDbgInfo() {
#if DEBUG
    if (!mpHIO->m.common.debug_info_ON) {
        return false;
    }
    
    f32 distMax1 = dComIfGp_getAttention()
                        ->getDistTable(attention_info.distances[fopAc_attn_SPEAK_e])
                        .mDistMax;
    f32 distMax2 = dComIfGp_getAttention()
                        ->getDistTable(attention_info.distances[fopAc_attn_TALK_e])
                        .mDistMax;
    s16 targetAngleX = cLib_targetAngleX(&current.pos, &eyePos);
    s16 targetAngleY = cLib_targetAngleY(&current.pos, &eyePos);
    cXyz pos(eyePos);
    pos.y += 100.0f;

    dDbVw_drawArrowOpa(pos, eyePos,
                        (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, 1, 12);
    dDbVw_drawCircleOpa(attention_info.position, distMax1,
                        (GXColor){0x0, 0xc8, 0x0, 0xff}, 1, 12);
    dDbVw_drawCircleOpa(attention_info.position, distMax2,
                        (GXColor){0xc8, 0x0, 0x0, 0xff}, 1, 12);

    f32 fov = mType == 0 ? 135.0f : mpHIO->m.common.fov;
    if (fov != 180.0f) {
        cXyz end;
        cXyz cStack_54(0.0f, 0.0f, mpHIO->m.common.search_distance);
        mDoMtx_stack_c::transS(attention_info.position);
        mDoMtx_stack_c::YrotM(mHeadAngle.y);
        mDoMtx_stack_c::YrotM(cM_deg2s(-fov));
        mDoMtx_stack_c::multVec(&cStack_54, &end);
        dDbVw_drawLineOpa(attention_info.position, end, (GXColor){0x0, 0x0, 0xc8, 0xff},
                            1, 12);

        mDoMtx_stack_c::transS(attention_info.position);
        mDoMtx_stack_c::YrotM(mHeadAngle.y);
        mDoMtx_stack_c::YrotM(cM_deg2s(fov));
        mDoMtx_stack_c::multVec(&cStack_54, &end);
        dDbVw_drawLineOpa(attention_info.position, end, (GXColor){0x0, 0x0, 0xc8, 0xff},
                            1, 12);
    }

    dDbVw_drawCircleOpa(attention_info.position, mpHIO->m.common.search_distance,
                        (GXColor){0x0, 0x0, 0xc8, 0xff}, 1, 12);
    dDbVw_drawSphereXlu(eyePos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xa0}, 1);
    dDbVw_drawSphereXlu(mLookPos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xa0}, 1);
#endif

    return false;
}

void daNpcRafrel_c::playExpression() {
    daNpcF_anmPlayData dat0 = {1, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm0[1] = {&dat0};

    daNpcF_anmPlayData dat1 = {2, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm1[1] = {&dat1};

    daNpcF_anmPlayData dat2 = {3, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm2[1] = {&dat2};

    daNpcF_anmPlayData dat3 = {4, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm3[1] = {&dat3};

    daNpcF_anmPlayData dat4 = {5, 0.0f, 0};
    daNpcF_anmPlayData* anm4[1] = {&dat4};

    daNpcF_anmPlayData dat5 = {6, 0.0f, 0};
    daNpcF_anmPlayData* anm5[1] = {&dat5};

    daNpcF_anmPlayData dat6 = {7, 0.0f, 0};
    daNpcF_anmPlayData* anm6[1] = {&dat6};

    daNpcF_anmPlayData dat7 = {0, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm7[1] = {&dat7};

    daNpcF_anmPlayData** anmData_p[8] = {
        anm0,
        anm1,
        anm2,
        anm3,
        anm4,
        anm5,
        anm6,
        anm7,
    };

    if (mExpression >= 0 && mExpression < 8) {
        playExpressionAnm(anmData_p);
    }
}

void daNpcRafrel_c::playMotion() {
    daNpcF_anmPlayData anm0_phase1 = {8, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm0[] = {&anm0_phase1};
    
    daNpcF_anmPlayData anm1_phase1 = {9, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm1[] = {&anm1_phase1};

    daNpcF_anmPlayData anm2_phase1 = {16, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData anm2_phase2 = {17, 0.0f, 0};
    daNpcF_anmPlayData* anm2[] = {&anm2_phase1, &anm2_phase2};

    daNpcF_anmPlayData anm3_phase1 = {21, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm3[] = {&anm3_phase1};

    daNpcF_anmPlayData anm4_phase1 = {25, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm4[] = {&anm4_phase1};

    daNpcF_anmPlayData anm5_phase1 = {24, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData anm5_phase2 = {25, 0.0f, 0};
    daNpcF_anmPlayData* anm5[] = {&anm5_phase1, &anm5_phase2};

    daNpcF_anmPlayData anm6_phase1 = {20, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData anm6_phase2 = {21, 0.0f, 0};
    daNpcF_anmPlayData* anm6[] = {&anm6_phase1, &anm6_phase2};

    daNpcF_anmPlayData anm7_phase1 = {22, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData anm7_phase2 = {23, 0.0f, 0};
    daNpcF_anmPlayData* anm7[] = {&anm7_phase1, &anm7_phase2};

    daNpcF_anmPlayData anm8_phase1 = {23, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm8[] = {&anm8_phase1};

    daNpcF_anmPlayData anm9_phase1 = {17, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm9[] = {&anm9_phase1};

    daNpcF_anmPlayData anm10_phase1 = {10, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData anm10_phase2 = {9, 0.0f, 0};
    daNpcF_anmPlayData* anm10[] = {&anm10_phase1, &anm10_phase2};

    daNpcF_anmPlayData anm11_phase1 = {11, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData anm11_phase2 = {12, 0.0f, 0};
    daNpcF_anmPlayData* anm11[] = {&anm11_phase1, &anm11_phase2};

    daNpcF_anmPlayData anm12_phase1 = {15, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData anm12_phase2 = {17, 0.0f, 0};
    daNpcF_anmPlayData* anm12[] = {&anm12_phase1, &anm12_phase2};

    daNpcF_anmPlayData anm13_phase1 = {12, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm13[] = {&anm13_phase1};

    daNpcF_anmPlayData anm14_phase1 = {26, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm14[] = {&anm14_phase1};

    daNpcF_anmPlayData anm15_phase1 = {27, 0.0f, 1};
    daNpcF_anmPlayData anm15_phase2 = {28, 6.0f, 0};
    daNpcF_anmPlayData* anm15[] = {&anm15_phase1, &anm15_phase2};

    daNpcF_anmPlayData anm16_phase1 = {18, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* anm16[] = {&anm16_phase1, NULL};

    daNpcF_anmPlayData anm17_phase1 = {14, 0.0f, 0};
    daNpcF_anmPlayData* anm17[] = {&anm17_phase1};

    daNpcF_anmPlayData anm18_phase1 = {13, 4.0f, 0};
    daNpcF_anmPlayData* anm18[] = {&anm18_phase1};

    daNpcF_anmPlayData** anmData_p[] = {
        anm0, anm1, anm2, anm3,
        anm4, anm5, anm6, anm7,
        anm8, anm9, anm10, anm11,
        anm12, anm13, anm14, anm15,
        anm16, anm17, anm18
    };

    if (mMotion >= 0 && mMotion < 19) {
        playMotionAnm(anmData_p);
    }
}

void daNpcRafrel_c::setAttnPos() {
    if (field_0xe0e == LOOK_RESET) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();

    cXyz sp20;
    if (mType == 0) {
        sp20.set(-15.0f, 0.0f, 0.0f);
    } else {
        sp20.set(-15.0f, 10.0f, 2.0f);
    }

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&sp20, &eyePos);
    sp20.x = 0.0f;
    mDoMtx_stack_c::multVec(&sp20, &sp20);

    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp20);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp20);

    cXyz* var_r27 = mLookat.getAttnPos();
    if (var_r27 != NULL) {
        cXyz sp14 = *var_r27 - eyePos;
        mEyeAngle.y = -mLookatAngle[2].y - mCurAngle.y;
        mEyeAngle.y += cM_atan2s(sp14.x, sp14.z);
        mEyeAngle.x = mHeadAngle.x - cM_atan2s(sp14.y, sp14.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    if (mType == 1) {
        attention_info.position.set(current.pos.x, current.pos.y + 220.0f, current.pos.z);
    } else {
        attention_info.position.set(mHeadPos.x, mHeadPos.y + mpHIO->m.common.attention_offset, mHeadPos.z);
    }

    cXyz cyl_center;
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(2));
    mDoMtx_stack_c::multVecZero(&cyl_center);
    cyl_center.y = current.pos.y;

    mCyl.SetC(cyl_center);
    mCyl.SetH(mpHIO->m.common.height);

    f32 var_f31;
    if (mType == 0) {
        var_f31 = 50.0f;
    } else {
        var_f31 = mpHIO->m.common.width;
    }

    mCyl.SetR(var_f31);
    dComIfG_Ccsp()->Set(&mCyl);
}

void daNpcRafrel_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model_p = mAnm_p->getModel();

    int var_r28 = 0;
    f32 body_angleX_min = mpHIO->m.common.body_angleX_min;
    f32 body_angleX_max = mpHIO->m.common.body_angleX_max;
    f32 body_angleY_min = mpHIO->m.common.body_angleY_min;
    f32 body_angleY_max = mpHIO->m.common.body_angleY_max;
    f32 head_angleX_min = mpHIO->m.common.head_angleX_min;
    f32 head_angleX_max = mpHIO->m.common.head_angleX_max;
    f32 head_angleY_min = mpHIO->m.common.head_angleY_min;
    f32 head_angleY_max = mpHIO->m.common.head_angleY_max;

    s16 temp_r26 = mCurAngle.y - mOldAngle.y;
    cXyz sp30[] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* sp24[] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};

    cXyz sp18;
    switch (field_0xe0e) {
    case LOOK_NONE:
        break;
    case LOOK_RESET:
        var_r28 = 1;
        break;
    case LOOK_PLAYER:
    case LOOK_PLAYER_TALK:
        player = daPy_getPlayerActorClass();
        break;
    case 4:
        player = (daPy_py_c*)field_0xc9c;
    }

    if (player != NULL) {
        mLookPos = player->attention_info.position;
        if (field_0xe0e != LOOK_PLAYER && field_0xe0e != LOOK_PLAYER_TALK) {
            mLookPos.y -= 40.0f;
        }
        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }

    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max,
                     0.0f, 0.0f, 0.0f, 0.0f,
                     head_angleX_min, head_angleX_max, head_angleY_min, head_angleY_max,
                     mCurAngle.y, sp30);
    mLookat.calc(this, model_p->getBaseTRMtx(), sp24, var_r28, temp_r26, 0);
}

void daNpcRafrel_c::setWaitAction() {
    switch (mType) {
    case 0:
        setAction(&daNpcRafrel_c::wait_type01);
        return;
    case 1:
        setAction(&daNpcRafrel_c::wait_type01);
        return;
    case 2:
        setAction(&daNpcRafrel_c::wait_type2);
        return;
    default:
        JUT_ASSERT(1274, FALSE);
        return;
    }
}

bool daNpcRafrel_c::isSneaking() {
    if (mType == 0) {
        if (dComIfGp_getStartStageRoomNo() == 5 && dComIfG_play_c::getLayerNo(0) == 2) {
            return true;
        }
    }

    return false;
}

void daNpcRafrel_c::setLookMode(int i_lookMode, fopAc_ac_c* i_talkPartner) {
    if (i_lookMode >= LOOK_NONE && i_lookMode < LOOK_ATTN) {
        if (i_lookMode != field_0xe0e || i_talkPartner != field_0xc9c) {
            field_0xe0e = i_lookMode;
            field_0xc9c = i_talkPartner;
        }
    }
}

bool daNpcRafrel_c::step(s16 i_angY, int param_1, f32 param_2) {
    if (mTurnMode == 0) {
        s16 var_r29 = i_angY - mCurAngle.y;
        if ((int)fabsf(cM_sht2d(var_r29)) > 40) {
            setExpression(7, -1.0f);
            setMotion(param_1, -1.0f, false);
        }
    
        mTurnTargetAngle = i_angY;
        mTurnAmount = 0;

        if (mCurAngle.y == mTurnTargetAngle) {
            mTurnMode++;
        }

        current.angle.y = mCurAngle.y;
        shape_angle.y = current.angle.y;
        mTurnMode++;
    } else if (mTurnMode == 1) {
        if (turn(mTurnTargetAngle, param_2, 0)) {
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

void daNpcRafrel_c::searchActors() {
    if (field_0xc7c[1].getActorP() == NULL) {
        field_0xc7c[1].entry(fopAcM_SearchByName(PROC_NPC_ASH));
    }

    if (field_0xc7c[2].getActorP() == NULL) {
        field_0xc7c[2].entry(fopAcM_SearchByName(PROC_NPC_SHAD));
    }

    if (field_0xc7c[3].getActorP() == NULL) {
        field_0xc7c[3].entry(fopAcM_SearchByName(PROC_NPC_MOIR));
    }
}

BOOL daNpcRafrel_c::chkFindPlayer() {
    f32 var_f31 = mType == 0 ? 135.0f : mpHIO->m.common.fov;

    if (!chkActorInSight(daPy_getPlayerActorClass(), var_f31)) {
        field_0xc7c[0].remove();
        return false;
    }

    BOOL ret = false;
    if (field_0xc7c[0].getActorP() == NULL) {
        ret = chkPlayerInSpeakArea(this);
    } else {
        ret = chkPlayerInTalkArea(this);
    }

    if (ret) {
        field_0xc7c[0].entry(daPy_getPlayerActorClass());
    } else {
        field_0xc7c[0].remove();
    }

    return ret;
}

bool daNpcRafrel_c::wait_type01(void* param_0) {
    switch (field_0xe10) {
    case 0:
        setExpression(7, -1.0f);
        setMotion(0, -1.0f, 0);
        field_0xe12 = mCurAngle.y;
        mTurnMode = 0;
        speedF = 0.0f;
        field_0xe10 = 2;
        break;

    case 2: {
        if (isSneaking()) {
            setLookMode(LOOK_NONE, NULL);
        } else if (field_0xe14 != 0 && mType == 1) {
            setLookMode(LOOK_PLAYER, NULL);
            if (mCurAngle.y == field_0xe12) {
                s16 spA = cM_deg2s(mpHIO->m.common.body_angleY_max + mpHIO->m.common.head_angleY_max);
                if (fopAcM_seenPlayerAngleY(this) > spA) {
                    field_0xe12 = fopAcM_searchPlayerAngleY(this);
                }
            } else if (step(field_0xe12, 0x12, 15.0f)) {
                setExpression(7, -1.0f);
                setMotion(0, -1.0f, 0);
                mTurnMode = 0;
            }

            f32 temp_f31 = current.pos.y - fopAcM_GetPosition_p(daPy_getPlayerActorClass())->y;
            if (temp_f31 < -200.0f || temp_f31 > 200.0f) {
                field_0xe14 = 0;
            }
        } else {
            if (field_0xc7c[0].getActorP() != NULL) {
                if (!chkFindPlayer()) {
                    mTurnMode = 0;
                }
            } else if (chkFindPlayer()) {
                mTurnMode = 0;
            }

            if (field_0xc7c[0].getActorP() != NULL) {
                setLookMode(LOOK_PLAYER, NULL);
            } else {
                setLookMode(LOOK_NONE, NULL);
                if (home.angle.y != mCurAngle.y && step(home.angle.y, 0x12, 15.0f)) {
                    setExpression(7, -1.0f);
                    setMotion(0, -1.0f, 0);
                    mTurnMode = 0;
                }
            }
        }

        if (field_0xe15 == 0 && mType == 1 && fopAcM_searchPlayerDistance(this) < mpHIO->m.demo_start_dist) {
            daNpcF_offTmpBit(0xB);
            daNpcF_offTmpBit(0xC);
            daNpcF_offTmpBit(0xD);
            daNpcF_offTmpBit(0xE);
            daNpcF_offTmpBit(0xF);
            daNpcF_offTmpBit(0x33);
            daNpcF_offTmpBit(0x34);
            mOrderEvtNo = 3;
            field_0xe15 = 1;
        }

        if (dComIfGp_event_runCheck()) {
            if (eventInfo.checkCommandTalk()) {
                if (isSneaking()) {
                    mOrderEvtNo = 6;
                    changeEvent(l_arcNames[0], l_evtNames[mOrderEvtNo], 1, 0xFFFF);
                } else if (dComIfGp_event_chkTalkXY()) {
                    if (dComIfGp_evmng_ChkPresentEnd() == 0) {
                        return 1;
                    }

                    if (dComIfGp_event_getPreItemNo() == 0x90) {
                        field_0xe0c = 0x21;
                        setAction(&daNpcRafrel_c::talk);
                    } else {
                        s16 sp8 = dComIfGp_getEventManager().getEventIdx(this, "NO_RESPONSE", 0xFF);
                        dComIfGp_getEvent()->reset(this);
                        fopAcM_orderChangeEventId(this, sp8, 1, 0xFFFF);
                    }
                } else {
                    setAction(&daNpcRafrel_c::talk);
                }
            } else {
                s32 staff_id = dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0);
                if (staff_id != -1) {
                    setAction(&daNpcRafrel_c::demo);
                }
            }
        } else {
            if (mOrderEvtNo != 0) {
                eventInfo.setArchiveName(l_arcNames[0]);
            }

            orderEvent(field_0xe16, (mOrderEvtNo != 0) ? l_evtNames[mOrderEvtNo] : NULL, 0xFFFF, 0x28, 0xFF, 1);

            if (field_0xe15 != 0) {
                eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);
            }
        }
        break;
    }

    case 3:
        break;

    default:
        JUT_ASSERT(1634, FALSE);
        break;
    }

    return 1;
}

void daNpcRafrel_c::setMotion(int param_0, f32 param_1, int param_2) {
    s16 var_r31 = param_0;
    switch (var_r31) {
    case 0:
        if (mType == 0) {
            var_r31 = 1;
        }
        break;
    case 1:
        if (mType == 1) {
            var_r31 = 0;
        }
        break;
    }

    if (var_r31 == 0 && mCurAngle.y == home.angle.y) {
        var_r31 = 0x10;
    }

    if ((param_2 != 0 || mMotion != var_r31) && param_0 >= 0 && param_0 < 0x13) {
        mMotion = var_r31;
        mMotionMorfOverride = param_1;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

void daNpcRafrel_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 8) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

bool daNpcRafrel_c::wait_type2(void* param_0) {
    switch (field_0xe10) {
    case 0:
        setExpression(4, -1.0f);
        setMotion(14, -1.0f, 0);
        mTurnMode = 0;
        speedF = 0.0f;
        field_0xc7c[0].entry(daPy_getPlayerActorClass());
        setLookMode(LOOK_NONE, NULL);
        field_0xe10 = 2;
        break;
        
    case 2:
        switch (field_0xe08) {
        case 0:
            if (daNpcF_chkTmpBit(0x76)) {
                current.pos.set(-4269.2476f, 0.0f, 8444.243f);
                old.pos = current.pos;
                setAngle(-0x6000);
                field_0xdfc = dComIfGp_particle_set(dPa_RM(ID_ZI_S_RAFBAZ_SMOKE_A), &current.pos, &shape_angle, NULL);
                field_0xe08++;
            }
            break;
        case 1:
            if (field_0xdfc != NULL) {
                field_0xdfc->setGlobalRTMatrix(mAnm_p->getModel()->getAnmMtx(0x13));
                if (daNpcF_chkTmpBit(0x75)) {
                    field_0xdfc->becomeInvalidEmitter();
                }
            }

            if (daNpcF_chkTmpBit(0x74)) {
                setAction(&daNpcRafrel_c::leave);
            }
            break;
        default:
            JUT_ASSERT(1683, FALSE);
            break;
        }
        break;

    case 3:
        break;

    default:
        JUT_ASSERT(1688, FALSE);
        break;
    }

    return 1;
}

void daNpcRafrel_c::setExpressionTalkAfter() {
    switch (mExpression) {
    case 1:
        setExpression(4, -1.0f);
        return;
    case 2:
        setExpression(5, -1.0f);
        return;
    case 3:
        setExpression(6, -1.0f);
        return;
    default:
        setExpression(7, -1.0f);
        return;
    }
}

bool daNpcRafrel_c::talk(void* param_0) {
    int sp14 = 0;
    bool var_r27 = 0;

    switch (field_0xe10) {
    case 0:
        initTalk(field_0xe0c, NULL);
        mTurnMode = 0;
        mMsgTimer = 0;
        field_0xe16 = 0;
        field_0xe14 = 1;
        speedF = 0.0f;
        setLookMode(LOOK_PLAYER_TALK, NULL);
        field_0xe10 = 2;
        break;
    case 2:
        if (mType == 0 || mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
            if (talkProc(NULL, 1, NULL)) {
                field_0xc7c[0].entry(daPy_getPlayerActorClass());

                int itemNo = 0;
                u16 eventId = mFlow.getEventId(&itemNo);
                OS_REPORT("会話終了時 イベントID=%d アイテムNo=%d\n", eventId, itemNo);

                if (eventId == 1) {
                    field_0xe00 = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, -1, NULL, NULL);
                    if (field_0xe00 != fpcM_ERROR_PROCESS_ID_e) {
                        s16 eventIdx = dComIfGp_getEventManager().getEventIdx(this, "DEFAULT_GETITEM", 0xFF);
                        dComIfGp_getEvent()->reset(this);
                        fopAcM_orderChangeEventId(this, eventIdx, 1, 0xFFFF);
                        field_0xe16 = 1;
                        field_0x9ec = 1;
                    }
                }

                setWaitAction();
                var_r27 = 1;
            } else {
                int temp_r24 = mMsgTimer;
                int spC, sp8;
                if (ctrlMsgAnm(spC, sp8, this, FALSE)) {
                    setExpression(spC, -1.0f);
                    setMotion(sp8, -1.0f, 0);
                } else if (temp_r24 != 0 && mMsgTimer == 0) {
                    setExpressionTalkAfter();
                }
            }
        } else if (step(fopAcM_searchPlayerAngleY(this), 0x12, 15.0f)) {
            setMotion(0, -1.0f, 0);
            mTurnMode = 0;
        }
        break;
    case 3:
        field_0xe0c = getMessageNo();
        setExpression(7, -1.0f);
        if (field_0x9ec == 0) {
            dComIfGp_event_reset();
        }
        break;
    default:
        JUT_ASSERT(1770, FALSE);
        break;
    }

    return var_r27;
}

bool daNpcRafrel_c::demo(void* param_0) {
    dEvent_manager_c* evtmgr = &dComIfGp_getEventManager();
    int unused = 0;

    switch (field_0xe10) {
    case 0:
        setExpression(7, -1.0f);
        setMotion(0, -1.0f, 0);
        field_0xe10 = 2;
        /* fallthrough */
    case 2:
        if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
            evtmgr = &dComIfGp_getEventManager();
            
            const int staffId = evtmgr->getMyStaffId(l_myName, NULL, 0);
            if (staffId != -1) {
                mStaffID = staffId;
                
                int actIdx = evtmgr->getMyActIdx(staffId, l_evtNames, ARRAY_SIZE(l_evtNames), 0, 0);
                if (actIdx > 0 && actIdx < 7) {
                    mOrderEvtNo = actIdx;
                }

                JUT_ASSERT(1810, NULL != mEvtSeqList[mOrderEvtNo]);
                if ((this->*mEvtSeqList[mOrderEvtNo])(staffId)) {
                    evtmgr->cutEnd(staffId);
                }

                unused = 1;
            }


            if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && evtmgr->endCheck(mEventIdx)) {
                switch (mOrderEvtNo) {
                    default:
                        break;
                }

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
        JUT_ASSERT(1842, FALSE);
        break;
    }

    return 1;
}

bool daNpcRafrel_c::leave(void* param_0) {
    cXyz sp8(-6253.387f, 0.0f, 6251.7603f);

    switch (field_0xe10) {
    case 0:
        setExpression(7, -1.0f);
        setMotion(15, -1.0f, 0);
        mTurnMode = 0;
        speedF = 0.0f;
        field_0xe10 = 2;
        break;

    case 2:
        if (mMotionPhase > 0) {
            step(cLib_targetAngleY(&current.pos,  &sp8), 0x12, 15.0f);
            speedF = 13.0f;

            if (mAcch.ChkWallHit() || current.pos.abs(sp8) < speedF) {
                fopAcM_delete(this);
            }
        }
        break;

    case 3:
        break;

    default:
        JUT_ASSERT(1881, FALSE);
        break;
    }

    return 1;
}

int daNpcRafrel_c::EvCut_Introduction(int i_staffId) {
    dEvent_manager_c* evtmgr = &dComIfGp_getEventManager();
    
    int* idata = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (idata == NULL) {
        return 0;
    }

    if (evtmgr->getIsAddvance(i_staffId)) {
        switch (*idata) {
        case 0:
        case 2:
            setLookMode(LOOK_NONE, NULL);
            return 1;
        case 1:
            setLookMode(LOOK_PLAYER, NULL);
            return 1;
        default:
            JUT_ASSERT(1916, FALSE);
            break;
        }
    }

    return 0;
}

int daNpcRafrel_c::EvCut_Meeting(int i_staffId) {
    dEvent_manager_c* evtmgr = &dComIfGp_getEventManager();
    
    int* idata = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (idata == NULL) {
        return 0;
    }

    if (evtmgr->getIsAddvance(i_staffId)) {
        switch (*idata) {
        case 0:
            setExpression(7, -1.0f);
            setMotion(1, -1.0f, 0);
            break;
        case 1:
            setLookMode(LOOK_PLAYER, NULL);
            break;
        default:
            JUT_ASSERT(1953, FALSE);
            break;
        }
    }

    int temp_r26 = mMsgTimer;
    int spC, sp8;
    if (ctrlMsgAnm(spC, sp8, this, FALSE)) {
        setExpression(spC, -1.0f);
        setMotion(sp8, -1.0f, 0);
    } else if (temp_r26 != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    switch (*idata) {
    case 0: {
        fopAc_ac_c* var_r28 = dComIfGp_event_getTalkPartner();
        if (var_r28 == this) {
            var_r28 = NULL;
        }
        setLookMode(LOOK_ACTOR, var_r28);
        return 1;
    }
    case 1:
        return 1;
    default:
        JUT_ASSERT(1978, FALSE);
        return 0;
    }

    return 0;
}

int daNpcRafrel_c::EvCut_Appear(int i_staffId) {
    char* cutname;
    dEvent_manager_c& evtmgr = dComIfGp_getEventManager();
    cutname = evtmgr.getMyNowCutName(i_staffId);

    if (evtmgr.getIsAddvance(i_staffId)) {
        switch (*(u32*)cutname) {
        case '0001':
            setLookMode(LOOK_NONE, NULL);
            initTalk(field_0xe0c, NULL);
            mMsgTimer = 0;
            field_0xe14 = 1;
            break;
        case '0003':
        case '0005':
        case '0006':
        case '0007':
        case '0008':
        case '0009':
        case '0011':
            setLookMode(LOOK_PLAYER_TALK, NULL);
            initTalk(field_0xe0c, NULL);
            mMsgTimer = 0;
            break;
        case '0002':
            mTurnMode = 0;
            break;
        case '0004': {
            J3DModelData* temp_r3_2 = mAnm_p->getModel()->getModelData();
            temp_r3_2->getMaterialNodePointer(5)->getShape()->hide();
            temp_r3_2->getMaterialNodePointer(4)->getShape()->show();
            setMotion(12, -1.0f, 0);
            break;
        }
        case '0010': {
            int itemNo = 0;
            u16 eventId = mFlow.getEventId(&itemNo);
            if (eventId == 1) {
                field_0xe00 = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, -1, NULL, NULL);
                dComIfGp_event_setItemPartnerId(field_0xe00);
                field_0xe00 = fpcM_ERROR_PROCESS_ID_e;
            }
        }
        break;
        default:
            JUT_ASSERT(2040, FALSE);
        }
    }

    switch (*(u32*)cutname) {
    case '0001':
        if (talkProc(NULL, 1, NULL)) {
            return 1;
        }
        break;
    case '0003':
    case '0005':
    case '0006':
    case '0007':
    case '0008':
    case '0009':
    case '0011':
        if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
            if (talkProc(NULL, 1, NULL)) {
                int choiceNo = mFlow.getChoiceNo();
                OS_REPORT("二択分岐 %s\n", choiceNo == 0 ? "はい" : "いいえ");

                if (choiceNo > 0) {
                    dComIfGp_getEvent()->reset(this);
                }

                return 1;
            }
        } else if (step(fopAcM_searchPlayerAngleY(this), 0x12, 15.0f)) {
            setMotion(0, -1.0f, 0);
            mTurnMode = 0;
        }
        break;
    case '0002':
        if (step(fopAcM_searchPlayerAngleY(this), 0x11, 30.0f)) {
            setExpression(7, -1.0f);
            setMotion(0, -1.0f, 0);
            return 1;
        }
        break;
    case '0004':
        return 1;
    case '0010':
        field_0x9eb = 1;
        return 1;
    default:
        JUT_ASSERT(2087, FALSE);
    }

    int expression, motion;
    int prevMsgTimer = mMsgTimer;
    if (ctrlMsgAnm(expression, motion, this, 0)) {
        if (field_0x9eb == 0) {
            setExpression(expression, -1.0f);
            setMotion(motion, -1.0f, 0);
        }
    } else if (prevMsgTimer != 0 && mMsgTimer == 0 && field_0x9eb == 0) {
        setExpressionTalkAfter();
    }

    return 0;
}

int daNpcRafrel_c::EvCut_WiretapSponsor(int i_staffId) {
    int* cutname;
    dEvent_manager_c& evtmgr = dComIfGp_getEventManager();
    fopAc_ac_c* actors[] = {
        this,
        field_0xc7c[1].getActorP(),
        field_0xc7c[2].getActorP(),
        field_0xc7c[3].getActorP(),
    };
    cutname = (int*)evtmgr.getMyNowCutName(i_staffId);

    if (evtmgr.getIsAddvance(i_staffId)) {
        switch (*cutname) {
        case '0001':
            dComIfGp_setMesgCameraInfoActor(actors[0], actors[1], actors[2], actors[3], NULL, NULL, NULL, NULL, NULL, NULL);
            initTalk(field_0xe0c, actors);
            break;
        default:
            JUT_ASSERT(2138, FALSE);
        }
    }

    int expression, motion;
    int prevMsgTimer = mMsgTimer;
    if (ctrlMsgAnm(expression, motion, this, 0)) {
        setExpression(expression, -1.0f);
        setMotion(motion, -1.0f, 0);
    } else if (prevMsgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    switch (*cutname) {
    case '0001': {
        if (talkProc(NULL, 1, actors)) {
            return 1;
        }

        fopAc_ac_c* actor_p = dComIfGp_event_getTalkPartner();
        if (actor_p == this) {
            actor_p = field_0xc7c[2].getActorP();
        }

        setLookMode(LOOK_ACTOR, actor_p);
        break;
    }
    default:
        JUT_ASSERT(2168, FALSE);
    }

    return 0;
}

int daNpcRafrel_c::EvCut_WiretapEntrant(int i_staffId) {
    dEvent_manager_c& evtmgr = dComIfGp_getEventManager();
    
    int* idata = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (idata == NULL) {
        return 0;
    }

    if (evtmgr.getIsAddvance(i_staffId)) {
        switch (*idata) {
        case 0:
            setExpression(7, -1.0f);
            setMotion(1, -1.0f, 0);
            break;
        default:
            JUT_ASSERT(2201, FALSE);
        }
    }

    int expression, motion;
    int prevMsgTimer = mMsgTimer;
    if (ctrlMsgAnm(expression, motion, this, 0)) {
        setExpression(expression, -1.0f);
        setMotion(motion, -1.0f, 0);
    } else if (prevMsgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    switch (*idata) {
    case 0: {
        fopAc_ac_c* actor_p = dComIfGp_event_getTalkPartner();
        if (actor_p == this) {
            actor_p = field_0xc7c[2].getActorP();
        }

        setLookMode(LOOK_ACTOR, actor_p);
        return 1;
    }
    default:
        JUT_ASSERT(2224, FALSE);
    }

    return 0;
}

static int daNpcRafrel_Create(void* i_this) {
    return ((daNpcRafrel_c*)i_this)->Create();
}

static int daNpcRafrel_Delete(void* i_this) {
    return ((daNpcRafrel_c*)i_this)->Delete();
}

static int daNpcRafrel_Execute(void* i_this) {
    return ((daNpcRafrel_c*)i_this)->Execute();
}

static int daNpcRafrel_Draw(void* i_this) {
    return ((daNpcRafrel_c*)i_this)->Draw();
}

static int daNpcRafrel_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpcRafrel_MethodTable = {
    (process_method_func)daNpcRafrel_Create,
    (process_method_func)daNpcRafrel_Delete,
    (process_method_func)daNpcRafrel_Execute,
    (process_method_func)daNpcRafrel_IsDelete,
    (process_method_func)daNpcRafrel_Draw,
};

actor_process_profile_definition g_profile_NPC_RAFREL = {
  fpcLy_CURRENT_e,          // mLayerID
  7,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_NPC_RAFREL,          // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daNpcRafrel_c),    // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  414,                      // mPriority
  &daNpcRafrel_MethodTable, // sub_method
  0x00040100,               // mStatus
  fopAc_NPC_e,              // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};
