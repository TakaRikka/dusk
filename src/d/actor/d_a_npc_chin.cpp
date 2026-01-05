/**
 * @file d_a_npc_chin.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc_chin.h"
#include "d/d_debug_viewer.h"
#include "d/d_meter2_info.h"
#include "d/d_timer.h"

enum Event {
    EVT_NONE,
    EVT_GAME_START,
    EVT_GAME_FAILED,
    EVT_GAME_SUCCEED,
    EVT_SELECT_GAME_GIVEUP,
    EVT_GAME_GIVEUP,
    EVT_CHIN_APPEAR,
    EVT_SPOTLIGHT_OFF,
};

static s16 const sLoadResPat_Normal[6] = {0, 1, 2, 4, 5, -1};

static s16 const sLoadResPat_Tw[3] = {0, 3, -1};

daNpcChin_HIOParam const daNpcChin_Param_c::m = {
    40.0f, -3.0f, 1.0f, 400.0f, 255.0f, 130.0f, 35.0f, 30.0f,
    0.0f, 0.0f, 10.0f, -10.0f, 30.0f, -10.0f, 45.0f, -45.0f,
    0.6f, 12.0f,
    3, 6, 5, 6,
    80.0f,
    500.0f, 300.0f, -300.0f,
    60, 8, 0, 0, 0,
    FALSE, FALSE,
};

static daNpc_GetParam2 l_bckGetParamList[33] = {
    {-1, 2, 0},
    {8, 0, 0},
    {7, 0, 0},
    {9, 2, 0},
    {8, 0, 1},
    {6, 0, 1},
    {12, 2, 1},
    {7, 0, 1},
    {13, 2, 1},
    {9, 0, 1},
    {14, 2, 1},
    {10, 0, 1},
    {15, 2, 1},
    {11, 0, 1},
    {16, 2, 1},
    {10, 2, 0},
    {5, 0, 0},
    {6, 2, 0},
    {5, 0, 1},
    {19, 0, 1},
    {24, 0, 1},
    {25, 2, 1},
    {30, 2, 1},
    {20, 0, 1},
    {21, 2, 1},
    {17, 0, 1},
    {18, 2, 1},
    {22, 0, 1},
    {23, 2, 1},
    {26, 0, 1},
    {27, 2, 1},
    {28, 0, 1},
    {29, 2, 1},
};

static daNpc_GetParam2 l_btpGetParamList[15] = {
    {18, 2, 0},
    {20, 0, 0},
    {19, 0, 0},
    {21, 2, 0},
    {42, 0, 1},
    {40, 0, 1},
    {46, 2, 1},
    {41, 0, 1},
    {47, 2, 1},
    {43, 0, 1},
    {48, 2, 1},
    {44, 0, 1},
    {49, 2, 1},
    {45, 0, 1},
    {50, 2, 1},
};

static daNpc_GetParam2 l_btkGetParamList[9] = {
    {13, 2, 0},
    {14, 0, 0},
    {15, 2, 0},
    {33, 0, 1},
    {34, 0, 1},
    {35, 2, 1},
    {36, 0, 1},
    {37, 0, 1},
    {7, 2, 5},
};

static char* l_arcNames[6] = {
    "Chin",
    "chin1",
    "chin_mdl",
    "chin_tmdl",
    "chin1_evt",
    "SpotLight",
};

static s16* sLoadResInfo[2] = {
    (s16*)sLoadResPat_Normal,
    (s16*)sLoadResPat_Tw,
};

static char* l_evtNames[8] = {
    NULL,
    "GAME_START",
    "GAME_FAILED",
    "GAME_SUCCEED",
    "SELECT_GAME_GIVEUP",
    "GAME_GIVEUP",
    "CHIN_APPEAR",
    "SPOTLIGHT_OFF",
};

static u8 lit_3861[12];

daNpcChin_c::eventFunc daNpcChin_c::mEvtSeqList[8] = {
    NULL,
    &daNpcChin_c::_Evt_GameStart,
    &daNpcChin_c::_Evt_GameFailed,
    &daNpcChin_c::_Evt_GameSucceed,
    &daNpcChin_c::_Evt_SelectGiveUp,
    &daNpcChin_c::_Evt_GameGiveUp,
    &daNpcChin_c::_Evt_Appear,
    &daNpcChin_c::_Evt_SpotLight_Off,
};

static NPC_CHIN_HIO_CLASS l_HIO;

static char* l_myName = "chin";

#if DEBUG
daNpcChin_HIO_c::daNpcChin_HIO_c() {
    m = daNpcChin_Param_c::m;
}

void daNpcChin_HIO_c::genMessage(JORMContext* ctx) {
    daNpcF_commonGenMessage(ctx, &m.common);
}
#endif

int daNpcChin_c::_SpotLight_c::loadModel() {
    void* modelData = NULL;
    void* anmTexSRTKey = NULL;

    modelData = dComIfG_getObjectRes(l_arcNames[5], 4);
    anmTexSRTKey = dComIfG_getObjectRes(l_arcNames[5], 7);
    JUT_ASSERT(337, modelData != NULL);

    mpModel = mDoExt_J3DModel__create((J3DModelData*)modelData,0x80000,0x11000284);
    if (mpModel == NULL) {
        return 0;
    }

    if (mBtkAnm.init((J3DModelData*)modelData, (J3DAnmTextureSRTKey*)anmTexSRTKey, 1, 2, 1.0f, 0, -1) == 0) {
        return 0;
    }

    switch_off();

    return 1;
}

void daNpcChin_c::_SpotLight_c::setMtx() {
    if (is_switch_on()) {
        mDoMtx_stack_c::transS(mPos);
        mDoMtx_stack_c::ZXYrotM(mRot);
        mDoMtx_stack_c::scaleM(mScale);
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

void daNpcChin_c::_SpotLight_c::draw(dKy_tevstr_c* i_tevStr) {
    if (is_switch_on()) {
        g_env_light.settingTevStruct(0, &mPos, i_tevStr);
        g_env_light.setLightTevColorType_MAJI(mpModel, i_tevStr);

        mBtkAnm.entry(mpModel->getModelData());
        mDoExt_modelUpdateDL(mpModel);
        mBtkAnm.remove(mpModel->getModelData());
    }
}

daNpcChin_c::daNpcChin_c() {
    mpSphMng = NULL;
}

daNpcChin_c::~daNpcChin_c() {
    if (!mTwilight) {
        dComIfG_TimerDeleteRequest(6);
    }

    s16* idx_p = sLoadResInfo[mTwilight];
    int i = 0;
    while (*idx_p >= 0) {
        // this decl forces r31 to be used for the stack pointer instead of r1
        int unused;

        OS_REPORT("--- arc : %s\n", l_arcNames[*idx_p]);
        dComIfG_resDelete(&mPhases[i], l_arcNames[*idx_p]);
        idx_p++;
        i++;
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

int daNpcChin_c::Create() {
    fopAcM_ct(this, daNpcChin_c);

    if (dKy_darkworld_check()) {
        mTwilight = true;
    } else {
        mTwilight = false;
    }

    s16* idx = sLoadResInfo[mTwilight];
    int phase_state = cPhs_ERROR_e;
    int i = 0;
    while (*idx >= 0) {
        OS_REPORT("--- arc : %s\n", l_arcNames[*idx]);
        phase_state = (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_arcNames[*idx]);
        if (phase_state != cPhs_COMPLEATE_e) {
            return phase_state;
        }

        idx++;
        i++;
    }

    if (phase_state == cPhs_COMPLEATE_e) {
        field_0xe24 = 0;
        if (!mTwilight) {
            dKy_change_colpat(0);
            if (!fopAcM_isSwitch(this, getSWBit())) {
                dKy_change_colpat(1);
                field_0xe24 = 1;
            }
        }

        if (fopAcM_entrySolidHeap(this, createHeapCallBack, 0x4660) == 0) {
                    return cPhs_ERROR_e;
                }

        field_0xe00 = getMessageNo();

        J3DModelData* modelData = mAnm_p->getModel()->getModelData();

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());

        if (field_0xe24 != 0) {
            fopAcM_setCullSizeBox(this, -160.0f, -1300.0f, -160.0f, 160.0f, 1300.0f, 160.0f);
        } else {
            fopAcM_setCullSizeBox(this, -160.0f, -50.0f, -160.0f, 160.0f, 220.0f, 160.0f);
        }

        mSound.init(&current.pos, &eyePos, 3, 1);

        #if DEBUG
        mpHIO = &l_HIO;
        // "W Hook Shot Shop"
        mpHIO->entryHIO("Wフックショット屋");
        #endif

        mAcchCir.SetWall(mpHIO->m.common.width, mpHIO->m.common.knee_length);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.SetRoofNone();
        mAcch.SetWaterNone();
        mAcch.CrrPos(dComIfG_Bgsp());

        mCcStts.Init(mpHIO->m.common.weight, 0, this);

        mObjInf.Set(mCcDCyl);
        mObjInf.SetStts(&mCcStts);
        mObjInf.SetTgType(0);
        mObjInf.SetTgSPrm(0);

        mGndChk = mAcch.m_gnd;

        mGroundH = mAcch.GetGroundH();

        if (!mTwilight) {
            mpSphMng = &daGlwSph_c::getSphMng();
        }

        setEnvTevColor();
        setRoomNo();
        mAnm_p->modelCalc();
        reset();
        Execute();
    }

    return phase_state;
}

int daNpcChin_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;
    J3DModel* model = NULL;
    int motionAnmParam = 15;
    int exprAnmParam = 1;

    if (mTwilight != 0) {
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_arcNames[3], 3);
    } else {
        mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_arcNames[2], 3);
    }
    OS_REPORT("mdlData_p = %x\n", mdlData_p);
    JUT_ASSERT(625, NULL != mdlData_p);

    u32 differedDlistFlag = 0x15020284;
    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000,
                                  differedDlistFlag);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }
    if (mAnm_p == NULL) {
        return 0;
    }

    model = mAnm_p->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    model->setUserArea((uintptr_t)this);

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    if (setExpressionAnm(exprAnmParam, false) == 0) {
        return 0;
    }

    setMotionAnm(motionAnmParam, 0.0f);

    if (field_0xe24 != 0 && mSpotLight.loadModel() == NULL) {
        return 0;
    }

    return 1;
}

static void dummy() {
    OS_REPORT("Delete -> NPC_CHIN(id=%d)\n");
}

int daNpcChin_c::Delete() {
    fpc_ProcID unusedId = fopAcM_GetID(this);
    OS_REPORT("----------------------daNpcChin_c::Delete!!\n");
    this->~daNpcChin_c();
    return 1;
}

int daNpcChin_c::Execute() {
    execute();

    if (mSpotLight.is_switch_on()) {
        mSpotLight.setPos(current.pos);
        mSpotLight.playBtk();
    }

    return 1;
}

int daNpcChin_c::Draw() {
    J3DModelData* modelData = mAnm_p->getModel()->getModelData();
    modelData->getMaterialNodePointer(3)->setMaterialAnm(mpMatAnm);

    draw(FALSE, FALSE, mpHIO->m.common.real_shadow_size, NULL, FALSE);
    mSpotLight.draw(&tevStr);

    return 1;
}

int daNpcChin_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* joint = i_joint;
    int jointNo = joint->getJntNo();

    int lit_4473[3] = {1, 3, 4};

    if (jointNo == 0) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(mLookatPos);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(3));
        mDoMtx_stack_c::multVecZero(mLookatPos + 1);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
        mDoMtx_stack_c::multVecZero(mLookatPos + 2);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jointNo));
    switch (jointNo) {
    case 1:
    case 3:
    case 4:
        setLookatMtx(jointNo, lit_4473, this->mpHIO->m.common.neck_rotation_ratio);
        break;
    }

    i_model->setAnmMtx(jointNo, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((jointNo == 4 || jointNo == 8) && (mAnmFlags & 0x100) != 0) {
        J3DAnmTransform* anmTransform = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(anmTransform);
    }

    return 1;
}

int daNpcChin_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpcChin_c* chin = (daNpcChin_c*)i_this;
    return chin->CreateHeap();
}

int daNpcChin_c::ctrlJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == NULL) {
        J3DModel* model = j3dSys.getModel();
        daNpcChin_c* chin = (daNpcChin_c*)model->getUserArea();
        if (chin != NULL) {
            chin->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daNpcChin_c::setParam() {
    attention_info.distances[fopAc_attn_LOCK_e] =
        getDistTableIdx(mpHIO->m.common.attention_distance, mpHIO->m.common.attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(mpHIO->m.common.talk_distance,
                                      mpHIO->m.common.talk_angle);

    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    if (daPy_py_c::checkNowWolf()) {
        attention_info.flags =
            fopAc_AttnFlag_UNK_0x800000 | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    }

    if (checkHide()) {
        attention_info.flags = 0;
    }

#if DEBUG
    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale,
              mpHIO->m.common.scale);

    mAcchCir.SetWallR(mpHIO->m.common.width);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);

    gravity = mpHIO->m.common.gravity;
#endif
}

int daNpcChin_c::main() {
    Action();

    if (dComIfGp_event_runCheck() && eventInfo.checkCommandTalk() == FALSE &&
        field_0xdf4 != fpcM_ERROR_PROCESS_ID_e)
    {
        dComIfGp_event_setItemPartnerId(field_0xdf4);
        field_0xdf4 = fpcM_ERROR_PROCESS_ID_e;
    }

    playExpression();
    playMotion();

    return 1;
}

int daNpcChin_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* var_r29 = 0;
        var_r29 =
            getTexSRTKeyAnmP(l_arcNames[l_btkGetParamList[0].arcIdx], l_btkGetParamList[0].fileIdx);
        if (var_r29 == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(field_0xe1c * cM_ssin(mEyeAngle.y) * -1.0f);
            mpMatAnm->setNowOffsetY(field_0xe20 * cM_ssin(mEyeAngle.x));
            mpMatAnm->onEyeMoveFlag();
            return 1;
        }
        mpMatAnm->offEyeMoveFlag();
    }
    return 0;
}

void daNpcChin_c::setAttnPos() {
    if (mLookMode == 1) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();

    cXyz acStack_2c(-30.0f, 10.0f, 0.0f);

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&acStack_2c, &eyePos);
    acStack_2c.y = 0.0f;
    mDoMtx_stack_c::multVec(&acStack_2c, &acStack_2c);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &acStack_2c);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &acStack_2c);

    cXyz* attnPos = mLookat1.getAttnPos();
    if (attnPos != NULL) {
        cXyz unkXyz1 = *attnPos - eyePos;
        mEyeAngle.y = -mLookatAngle[2].y - mCurAngle.y;
        mEyeAngle.y += cM_atan2s(unkXyz1.x, unkXyz1.z);
        mEyeAngle.x = -cM_atan2s(unkXyz1.y, unkXyz1.absXZ());
        mEyeAngle.x += mHeadAngle.x;
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    attention_info.position.set(mHeadPos.x,
              mHeadPos.y + mpHIO->m.common.attention_offset, mHeadPos.z);

    mObjInf.SetC(current.pos);
    mObjInf.SetH(mpHIO->m.common.height);
    mObjInf.SetR(mpHIO->m.common.width);

    dComIfG_Ccsp()->Set(&mObjInf);
}

bool daNpcChin_c::setExpressionAnm(int param_0, bool param_1) {
    mAnmFlags = mAnmFlags & ~0x1fc0;

    J3DAnmTransform* anmTransform = NULL;
    if (l_bckGetParamList[param_0].fileIdx >= 0) {
        anmTransform = (J3DAnmTransform*)getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[param_0].arcIdx],
            l_bckGetParamList[param_0].fileIdx);
    }

    int iVar3 = l_bckGetParamList[param_0].attr;
    bool iVar1 = false;
    switch (param_0) {
    case 1:
        iVar1 = setExpressionBtp(1);
        break;
    case 2:
        iVar1 = setExpressionBtp(2);
        break;
    case 3:
        iVar1 = setExpressionBtp(3);
        break;
    case 4:
        iVar1 = setExpressionBtp(4);
        break;
    case 5:
        iVar1 = setExpressionBtp(5);
        break;
    case 6:
        iVar1 = setExpressionBtp(6);
        break;
    case 7:
        iVar1 = setExpressionBtp(7);
        break;
    case 8:
        iVar1 = setExpressionBtp(8);
        break;
    case 9:
        iVar1 = setExpressionBtp(9);
        break;
    case 10:
        iVar1 = setExpressionBtp(10);
        break;
    case 11:
        iVar1 = setExpressionBtp(11);
        break;
    case 12:
        iVar1 = setExpressionBtp(12);
        break;
    case 13:
        iVar1 = setExpressionBtp(13);
        break;
    case 14:
        iVar1 = setExpressionBtp(14);
        break;
    default:
        iVar1 = setExpressionBtp(0);
        break;
    }

    if (iVar1 == 0) {
        return false;
    }

    if (anmTransform == NULL) {
        return true;
    }

    if (setBckAnm(anmTransform, 1.0f, iVar3, 0, -1, param_1)) {
        mAnmFlags = mAnmFlags | 0x140;
        mExpressionLoops = 0;
        return true;
    } else {
        // "Failed to register facial expression Bck animation"
        OS_REPORT("%s: 表情Bckアニメーションの登録に失敗しました！\n", __FILE__);
        return false;
    }
}

bool daNpcChin_c::setExpressionBtp(int param_0) {
    J3DAnmTexPattern* anmTexPattern = NULL;
    if (l_btpGetParamList[param_0].fileIdx >= 0) {
        anmTexPattern = getTexPtrnAnmP(l_arcNames[l_btpGetParamList[param_0].arcIdx],
                                       l_btpGetParamList[param_0].fileIdx);
    }

    int iVar3 = l_btpGetParamList[param_0].attr;

    mAnmFlags = mAnmFlags & ~0xa80;

    if (anmTexPattern == NULL) {
        return true;
    }

    if (setBtpAnm(anmTexPattern, mAnm_p->getModel()->getModelData(), 1.0f, iVar3)) {
        mAnmFlags = mAnmFlags | 0x280;
        if (param_0 == 0) {
            mAnmFlags = mAnmFlags | 0x800;
        }
        return true;
    } else {
        // "Failed to register facial expression Btp animation"
        OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", __FILE__);
        return false;
    }
}

void daNpcChin_c::setMotionAnm(int param_0, f32 param_1) {
    int iVar5;
    switch (param_0) {
    case 16:
        iVar5 = 1;
        break;
    case 21:
        iVar5 = 5;
        break;
    case 19:
        iVar5 = 3;
        break;
    case 22:
        iVar5 = 7;
        break;
    case 29:
        iVar5 = 6;
        break;
    case 23:
    case 24:
        field_0xe1c = 0.2f;
        field_0xe20 = -0.05f;
        iVar5 = 0;
        break;
    case 25:
    case 26:
        field_0xe1c = 0.2f;
        field_0xe20 = 0.15f;
        iVar5 = 0;
        break;
    case 27:
    case 28:
        field_0xe1c = -0.2f;
        field_0xe20 = 0.1f;
        iVar5 = 0;
        break;
    case 17:
        field_0xe1c = 1.5999999f;
        field_0xe20 = -0.1f;
        iVar5 = 0;
        break;
    default:
        field_0xe1c = 0.1f;
        field_0xe20 = -0.05f;
        iVar5 = 0;
        break;
    }

    J3DAnmTransformKey* trnsfrmKey = getTrnsfrmKeyAnmP(
        l_arcNames[l_bckGetParamList[param_0].arcIdx], l_bckGetParamList[param_0].fileIdx);
    J3DAnmTextureSRTKey* texSRTKey = getTexSRTKeyAnmP(l_arcNames[l_btkGetParamList[iVar5].arcIdx],
                                                      l_btkGetParamList[iVar5].fileIdx);

    int iVar1 = l_bckGetParamList[param_0].attr;
    int iVar6 = l_btkGetParamList[iVar5].attr;

    mAnmFlags = mAnmFlags & ~0x3f;

    if (trnsfrmKey != NULL) {
        if (setMcaMorfAnm(trnsfrmKey, 1.0f, param_1, iVar1, 0, -1)) {
            mAnmFlags = mAnmFlags | 0x9;
            mMotionLoops = 0;
        } else {
            OS_REPORT("BCK SET MISS\n");
        }
    } else {
        OS_REPORT("BCK LOAD MISS\n");
    }

    if (texSRTKey != NULL) {
        if (setBtkAnm(texSRTKey, mAnm_p->getModel()->getModelData(), 1.0f, iVar6)) {
            mAnmFlags = mAnmFlags | 0x12;
        } else {
            OS_REPORT("BTK SET MISS\n");
        }
    } else {
        OS_REPORT("BTK LOAD MISS\n");
    }
}

void daNpcChin_c::setExpression(int i_index, f32 i_morf) {
    if (i_index >= 0 && i_index < 15) {
        mExpression = i_index;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

void daNpcChin_c::setMotion(int i_index, f32 i_morf, int param_2) {
    s16 param_0_copy = i_index;
    if ((param_2 != 0 || mMotion != param_0_copy) && i_index >= 0 && i_index < 18) {
        mMotion = param_0_copy;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

int daNpcChin_c::drawDbgInfo() {
#if DEBUG
    if (mpHIO->m.common.debug_info_ON == 0) {
        return 0;
    }

    cXyz searchDistFinal;

    f32 speakDistMax = dComIfGp_getAttention()
                           ->getDistTable(attention_info.distances[fopAc_attn_SPEAK_e])
                           .mDistMax;
    f32 talkDistMax =
        dComIfGp_getAttention()->getDistTable(attention_info.distances[fopAc_attn_TALK_e]).mDistMax;

    s16 targetAngleX = cLib_targetAngleX(&current.pos, &eyePos);
    s16 targetAngleY = cLib_targetAngleY(&current.pos, &eyePos);
    dDbVw_drawCircleOpa(attention_info.position, speakDistMax, (GXColor){0x00, 0xc8, 0x00, 0xff}, 1,
                        12);
    dDbVw_drawCircleOpa(attention_info.position, talkDistMax, (GXColor){0xc8, 0x00, 0x00, 0xff}, 1,
                        12);

    if (mpHIO->m.common.fov != 180.0f) {
        cXyz searchDistBase(0.0f, 0.0f, mpHIO->m.common.search_distance);
        mDoMtx_stack_c::transS(attention_info.position);
        mDoMtx_stack_c::YrotM(mHeadAngle.y);
        mDoMtx_stack_c::YrotM(cM_deg2s(-mpHIO->m.common.fov));
        mDoMtx_stack_c::multVec(&searchDistBase, &searchDistFinal);
        dDbVw_drawLineOpa(attention_info.position, searchDistFinal,
                          (GXColor){0x00, 0x00, 0xc8, 0xff}, 1, 12);

        mDoMtx_stack_c::transS(attention_info.position);
        mDoMtx_stack_c::YrotM(mHeadAngle.y);
        mDoMtx_stack_c::YrotM(cM_deg2s(mpHIO->m.common.fov));
        mDoMtx_stack_c::multVec(&searchDistBase, &searchDistFinal);
        dDbVw_drawLineOpa(attention_info.position, searchDistFinal,
                          (GXColor){0x00, 0x00, 0xc8, 0xff}, 1, 12);
    }

    dDbVw_drawCircleOpa(attention_info.position, mpHIO->m.common.search_distance,
                        (GXColor){0x00, 0x00, 0xc8, 0xff}, 1, 12);

    dDbVw_drawSphereXlu(eyePos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xa0}, 1);
    dDbVw_drawSphereXlu(mLookPos, 18.0f, (GXColor){0x80, 0x80, 0x80, 0xa0}, 1);
#endif

    return 0;
}

void daNpcChin_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mLookat1.initialize();
    for (int i = 0; i < 1; i++) {
        mActorMngrs[i].initialize();
    }

    field_0xdf8 = 0;
    field_0xdfc = 0;
    mAction = 0;
    mLookMode = -1;
    mActionMode = 0;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    field_0xdf4 = fpcM_ERROR_PROCESS_ID_e;
    mOrderEvtNo = EVT_NONE;

    if (mTwilight) {
        field_0xe25 = 1;
    }
    else {
        field_0xe25 = 0;
    }

    mIsMiniGamePlaying = 0;
    mExpressionMorfOverride = 0.0f;
    mMotionMorfOverride = 0.0f;

    setAction(&daNpcChin_c::wait);
}

void daNpcChin_c::playExpression() {
    daNpcF_anmPlayData dat0 = {1, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1 = {2, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat1[1] = {&dat1};
    daNpcF_anmPlayData dat2 = {5, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3 = {7, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat3[1] = {&dat3};
    daNpcF_anmPlayData dat4 = {4, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5 = {9, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData dat6 = {11, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat6[1] = {&dat6};
    daNpcF_anmPlayData dat7 = {13, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8 = {3, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};
    daNpcF_anmPlayData dat9 = {6, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat9[1] = {&dat9};
    daNpcF_anmPlayData dat10 = {8, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat10[1] = {&dat10};
    daNpcF_anmPlayData dat11 = {10, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat11[1] = {&dat11};
    daNpcF_anmPlayData dat12 = {12, 0.0f, 0};
    daNpcF_anmPlayData* pDat12[1] = {&dat12};
    daNpcF_anmPlayData dat13 = {14, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat13[1] = {&dat13};
    daNpcF_anmPlayData dat14 = {0, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat14[1] = {&dat14};

    daNpcF_anmPlayData** ppDat[15] = {
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
    };

    if (mExpression >= 0 && mExpression < 15) {
        playExpressionAnm(ppDat);
    }
}

void daNpcChin_c::playMotion() {

    daNpcF_anmPlayData dat0a = {15, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0a};
    daNpcF_anmPlayData dat1a = {16, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {17, 0.0f, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2a = {23, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {24, 0.0f, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3a = {20, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {21, 0.0f, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4a = {27, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat4b = {28, 0.0f, 0};
    daNpcF_anmPlayData* pDat4[2] = {&dat4a, &dat4b};
    daNpcF_anmPlayData dat5a = {25, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat5b = {26, 0.0f, 0};
    daNpcF_anmPlayData* pDat5[2] = {&dat5a, &dat5b};
    daNpcF_anmPlayData dat6a = {29, 0.0f, 1};
    daNpcF_anmPlayData dat6b = {30, 0.0f, 0};
    daNpcF_anmPlayData* pDat6[2] = {&dat6a, &dat6b};
    daNpcF_anmPlayData dat7a = {18, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat7b = {26, 0.0f, 0};
    daNpcF_anmPlayData* pDat7[2] = {&dat7a, &dat7b};
    daNpcF_anmPlayData dat8a = {31, 0.0f, 1};
    daNpcF_anmPlayData dat8b = {32, 0.0f, 0};
    daNpcF_anmPlayData* pDat8[2] = {&dat8a, &dat8b};
    daNpcF_anmPlayData dat9a = {19, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat9b = {15, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat9[2] = {&dat9a, &dat9b};
    daNpcF_anmPlayData dat10a = {21, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat10[1] = {&dat10a};
    daNpcF_anmPlayData dat11a = {28, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat11[1] = {&dat11a};
    daNpcF_anmPlayData dat12a = {26, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat12[1] = {&dat12a};
    daNpcF_anmPlayData dat13a = {32, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat13[1] = {&dat13a};
    daNpcF_anmPlayData dat14a = {24, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat14[1] = {&dat14a};
    daNpcF_anmPlayData dat15a = {30, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat15[1] = {&dat15a};
    daNpcF_anmPlayData dat16a = {17, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat16[1] = {&dat16a};
    daNpcF_anmPlayData dat17a = {22, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat17[1] = {&dat17a};

    daNpcF_anmPlayData** ppDat[18] = {
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
    };

    if (mMotion >= 0 && mMotion < 18) {
        playMotionAnm(ppDat);
    }
}

int daNpcChin_c::setAction(actionFunc i_func) {
    mActionMode = -1;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mActionMode = 0;

    mAction = i_func;
    if (mAction) {
        (this->*mAction)(NULL);
    }

    return 1;
}

void daNpcChin_c::setLookMode(int i_mode) {
    if (i_mode >= 0 && i_mode < 4 && i_mode != mLookMode) {
        mLookMode = i_mode;
    }
}

void daNpcChin_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();

    BOOL snap = 0;
    f32 bodyAngleXMin = mpHIO->m.common.body_angleX_min;
    f32 bodyAngleXMax = mpHIO->m.common.body_angleX_max;
    f32 bodyAngleYMin = mpHIO->m.common.body_angleY_min;
    f32 bodyAngleYMax = mpHIO->m.common.body_angleY_max;
    f32 headAngleXMin = mpHIO->m.common.head_angleX_min;
    f32 headAngleXMax = mpHIO->m.common.head_angleX_max;
    f32 headAngleYMin = mpHIO->m.common.head_angleY_min;
    f32 headAngleYMax = mpHIO->m.common.head_angleY_max;

    s16 angleDiff = mCurAngle.y - mOldAngle.y;

    cXyz lookatPositions[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* lookatAngles[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};

    cXyz unusedXyz1;

    switch (mLookMode) {
    case 0:
        break;
    case 1:
        snap = TRUE;
        break;
    case 2:
    case 3:
        player = daPy_getPlayerActorClass();
        if (mLookMode == 3) {
            headAngleYMin = -80.0f;
            headAngleYMax = 80.0f;
        }
        break;
    }

    if (player != NULL) {
        mLookPos = player->attention_info.position;
        if (mLookMode != 2 && mLookMode != 3) {
            mLookPos.y = mLookPos.y - 40.0f;
        }
        mLookat1.setAttnPos(&mLookPos);
    } else {
        mLookat1.setAttnPos(0);
    }

    mLookat1.setParam(bodyAngleXMin, bodyAngleXMax, bodyAngleYMin, bodyAngleYMax, 0.0f, 0.0f, 0.0f,
                      0.0f, headAngleXMin, headAngleXMax, headAngleYMin, headAngleYMax, mCurAngle.y,
                      lookatPositions);
    mLookat1.calc(this, model->getBaseTRMtx(), lookatAngles, snap, angleDiff, 0);
}

bool daNpcChin_c::step(s16 param_0, int param_1, int param_2) {
    if (mTurnMode == 0) {
        s16 angleDiff = param_0 - mCurAngle.y;
        if (param_1 != 0) {
            if ((int)fabsf(cM_sht2d(angleDiff)) > 40) {
                setExpression(14, -1.0f);
            }
        }

        mTurnTargetAngle = param_0;
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

BOOL daNpcChin_c::chkFindPlayer() {
    if (!chkActorInSight(daPy_getPlayerActorClass(), mpHIO->m.common.fov)) {
        mActorMngrs[0].remove();
        return FALSE;
    }

    BOOL inTalkArea = FALSE;
    if (mActorMngrs[0].getActorP() == NULL) {
        inTalkArea = chkPlayerInTalkArea(this);
    } else {
        inTalkArea = chkPlayerInTalkArea(this);
    }

    if (inTalkArea) {
        mActorMngrs[0].entry(daPy_getPlayerActorClass());
    } else {
        mActorMngrs[0].remove();
    }

    return inTalkArea;
}

void daNpcChin_c::setExpressionTalkAfter() {
    switch (mExpression) {
    case 1:
        setExpression(8, -1.0f);
        break;
    case 2:
        setExpression(9, -1.0f);
        break;
    case 3:
        setExpression(10, -1.0f);
        break;
    case 5:
        setExpression(11, -1.0f);
        break;
    case 6:
        setExpression(12, -1.0f);
        break;
    case 7:
        setExpression(13, -1.0f);
        break;
    default:
        setExpression(14, -1.0f);
        break;
    }
}

bool daNpcChin_c::wait(void* param_0) {
    switch (mActionMode) {
    case 0:
        setExpression(14, -1.0f);
        setMotion(0, -1.0f, 0);
        mTurnMode = 0;
        speedF = 0.0f;
        if (field_0xe24 != 0) {
            field_0xdfc = 1;
            mActionMode = 1;
        } else {
            mActionMode = 2;
        }

        break;
    case 1:
        field_0xdfc--;
        if (field_0xdfc > 0) {
            break;
        }

        if (daPy_py_c::checkNowWolf()) {
            if (field_0xe25 == 0) {
                ForcibleTalk_On();
                dKy_change_colpat(0);
                field_0xe25 = 1;
                field_0xe24 = 0;
            }
        } else {
            Z2GetAudioMgr()->bgmStart(Z2BGM_WCS_D01, 0, 0);
            mOrderEvtNo = EVT_CHIN_APPEAR;
        }

        mActionMode = 2;

        break;
    case 2:
        if (daPy_py_c::checkNowWolf() && field_0xe25 == 0) {
            ForcibleTalk_On();
            dKy_change_colpat(0);
            field_0xe25 = 1;
        }

        if (mActorMngrs[0].getActorP() != NULL) {
            if (!chkFindPlayer()) {
                mTurnMode = 0;
            }
        } else {
            if (chkFindPlayer()) {
                mTurnMode = 0;
            }
        }

        if (mActorMngrs[0].getActorP() != NULL) {
            if (mTwilight) {
                setLookMode(0);
            } else {
                setLookMode(2);
            }
        } else {
            setLookMode(0);
            if (home.angle.y != mCurAngle.y && step(home.angle.y, 1, 15)) {
                setExpression(14, -1.0f);
                setMotion(0, -1.0f, 0);
                mTurnMode = 0;
            }
        }

        if (dComIfGp_event_runCheck()) {
            if (eventInfo.checkCommandTalk()) {
                if (!dComIfGp_event_chkTalkXY() || dComIfGp_evmng_ChkPresentEnd()) {
                    setAction(&daNpcChin_c::talk);
                }
            } else {
                int staffId = dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0);
                if (staffId != -1) {
                    setAction(&daNpcChin_c::demo);
                }
            }
        } else {
            if (mOrderEvtNo != EVT_NONE) {
                eventInfo.setArchiveName(l_arcNames[4]);
            }

            if (!dComIfGp_getEvent()->isOrderOK()) {
                return true;
            }

            u16 eventMask = 0xffff;
            if (mOrderEvtNo == EVT_CHIN_APPEAR) {
                // maybe fakematch? this still doesn't quite match for debug but it's closer
#if PLATFORM_SHIELD
                eventMask = eventMask & 0xff7f;
#else
                eventMask &= ~0x80;
#endif
            }

            orderEvent(getForcibleTalk2(), l_evtNames[mOrderEvtNo], eventMask, 4, 0xff, 2);

            break;
        }

        break;
    case -1:
        break;
    default:
        JUT_ASSERT(2139, FALSE);
    }

    return true;
}

bool daNpcChin_c::talk(void* param_0) {
    int unusedInt1 = 0;
    bool result = false;
    u16 eventId;

    switch (mActionMode) {
    case 0:
        initTalk(field_0xe00, NULL);
        dMeter2Info_setMsgTimeMs(dComIfGs_getHookGameTime());
        mTurnMode = 0;
        mMsgTimer = 0;
        ForcibleTalk_Off();

        if (mTwilight) {
            setLookMode(0);
        } else {
            setLookMode(3);
        }

        mActionMode = 1;

        break;
    case -1:
        setExpression(14, -1.0f);

        if (!field_0x9ec) {
            dComIfGp_event_reset();
        }

        break;
    case 1:
        if (mTwilight || fopAcM_searchPlayerAngleY(this) == mCurAngle.y) {
            if (talkProc(NULL, 1, NULL)) {
                mActorMngrs[0].entry(daPy_getPlayerActorClass());

                int itemId = 0;
                eventId = mFlow.getEventId(&itemId);
                // "End of conversation Event ID=%d Item No=%d"
                OS_REPORT("会話終了時 イベントID=%d アイテムNo=%d\n", eventId, itemId);
                if (eventId == 18) {
                    field_0xe07 = itemId;
                    mOrderEvtNo = EVT_GAME_START;
                }

                setAction(&daNpcChin_c::wait);

                result = true;
                break;
            }

            _talk_motion();
        } else if (step(fopAcM_searchPlayerAngleY(this), 1, 15)) {
            setMotion(0, -1.0f, 0);
            mTurnMode = 0;
        }

        break;
    default:
        OS_REPORT("-----------------CHIN::TALK ASSERT\n");
        JUT_ASSERT(2270, FALSE);
    }

    return result;
}

void daNpcChin_c::_talk_motion() {
    int exprIdx;
    int motionIdx;

    int msgTimerOld = mMsgTimer;
    if (ctrlMsgAnm(exprIdx, motionIdx, this, 0)) {
        setExpression(exprIdx, -1.0f);
        setMotion(motionIdx, -1.0f, 0);
    } else if (msgTimerOld != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }
}

bool daNpcChin_c::demo(void* param_0) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int unusedInt1 = 0;

    switch (mActionMode) {
    case 0: {
        setExpression(14, -1.0f);
        setMotion(0, -1.0f, 0);

        mActionMode = 1;

        break;
    }
    case -1: {
        break;
    }
    case 1: {
        if (!dComIfGp_event_runCheck()) {
            break;
        }

        if (eventInfo.checkCommandTalk()) {
            break;
        }

        eventManager = &dComIfGp_getEventManager();
        s32 staffId = eventManager->getMyStaffId(l_myName, NULL, 0);
        if (staffId != -1) {
            mStaffID = staffId;
            JUT_ASSERT(2353, NULL != mEvtSeqList[mOrderEvtNo]);
            if ((this->*mEvtSeqList[mOrderEvtNo])(staffId)) {
                eventManager->cutEnd(staffId);
            }

            unusedInt1 = 1;
        }

        if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 &&
            eventManager->endCheck(mEventIdx))
        {
            Event_DT();
        }

        break;
    }
    default: {
        JUT_ASSERT(2377, FALSE);
    }
    }

    return true;
}

bool daNpcChin_c::watch_game(void* param_0) {
    switch (mActionMode) {
    case 0: {
        if (dComIfG_getTimerPtr() == NULL) {
            mOrderEvtNo = EVT_GAME_START;
            setAction(&daNpcChin_c::wait);
            return true;
        }

        setExpression(14, -1.0f);
        setMotion(17, -1.0f, 0);
        setLookMode(0);
        setAngle(0x6547);
        mIsMiniGamePlaying = 1;
        mActionMode = 1;

        break;
    }
    case -1: {
        mIsMiniGamePlaying = 0;

        break;
    }
    case 1: {
        daPy_getPlayerActorClass()->onSwordTriggerNon();
        int local_58 = -1;

        int gameEnd = isGameEnd(local_58);
        if (gameEnd == 1) {
            Z2GetAudioMgr()->subBgmStop();

            dComIfG_TimerDeleteRequest(6);
            mOrderEvtNo = EVT_GAME_FAILED;
            setAction(&daNpcChin_c::wait);
        } else if (gameEnd == 2) {
            Z2GetAudioMgr()->subBgmStop();
            Z2GetAudioMgr()->subBgmStart(Z2BGM_MINIGAME_WIN02);

            if (field_0xe07 == 2 || field_0xe07 == 1) {
                dComIfGs_setHookGameTime(local_58);
            }

            dComIfG_TimerDeleteRequest(6);
            mOrderEvtNo = EVT_GAME_SUCCEED;
            setAction(&daNpcChin_c::wait);
        } else {
            mpSphMng->SphSeProc();

            if (dComIfGp_event_runCheck()) {
                if (eventInfo.checkCommandTalk() && (!dComIfGp_event_chkTalkXY() || dComIfGp_evmng_ChkPresentEnd())) {
                    dComIfG_TimerStop(6);
                    mOrderEvtNo = EVT_SELECT_GAME_GIVEUP;
                    changeEvent(l_arcNames[4], l_evtNames[mOrderEvtNo], 2, 0xffff);
                    setAction(&daNpcChin_c::demo);
                }
            } else {
                if (mOrderEvtNo != EVT_NONE) {
                    eventInfo.setArchiveName(l_arcNames[4]);
                }

                orderEvent(getForcibleTalk2(), l_evtNames[mOrderEvtNo], 0xffff, 4, 0xff, 2);
            }
        }

        break;
    }
    default: {
        JUT_ASSERT(2547, FALSE);
    }
    }

    return true;
}

void daNpcChin_c::Event_DT() {
    int local_60 = 0;

    switch (mOrderEvtNo) {
    case EVT_GAME_START:
        Event_DT_Base();
        setAction(&daNpcChin_c::watch_game);

        break;
    case EVT_GAME_SUCCEED:
        field_0xe00 = 0x439;
        Event_DT_Base();
        ForcibleTalk_Off();
        setAction(&daNpcChin_c::wait);

        break;
    case EVT_GAME_FAILED:
        if (mFlow.getChoiceNo() == 0 && mFlow.getEventId(&local_60) == 18) {
            field_0xe07 = local_60;
            mOrderEvtNo = EVT_GAME_START;
            changeEvent(l_arcNames[4], "GAME_START", 2, 0xffff);

            break;
        }

        field_0xe00 = 0x439;
        Event_DT_Base();
        ForcibleTalk_Off();
        setAction(&daNpcChin_c::wait);

        break;
    case EVT_SELECT_GAME_GIVEUP:
        Event_DT_Base();

        if (mFlow.getChoiceNo() == 0) {
            dComIfG_TimerDeleteRequest(6);
            Z2GetAudioMgr()->subBgmStop();
            mOrderEvtNo = EVT_GAME_GIVEUP;
            changeEvent(l_arcNames[4], "GAME_GIVEUP", 1, 0xffff);

            break;
        }

        dComIfG_TimerReStart(6);
        setAction(&daNpcChin_c::watch_game);

        break;
    case EVT_GAME_GIVEUP:
        field_0xe00 = 0x439;
        Event_DT_Base();
        ForcibleTalk_Off();
        setAction(&daNpcChin_c::wait);

        break;
    case EVT_CHIN_APPEAR:
        Event_DT_Base();
        field_0xe24 = 0;
        fopAcM_onSwitch(this, getSWBit());
        ForcibleTalk_Off();

        if (mFlow.getChoiceNo() == 0 && mFlow.getEventId(&local_60) == 18) {
            mOrderEvtNo = EVT_GAME_START;
            changeEvent(l_arcNames[4], "GAME_START", 2, 0xffff);

            break;
        }

        mOrderEvtNo = EVT_SPOTLIGHT_OFF;
        changeEvent(l_arcNames[4], "SPOTLIGHT_OFF", 2, 0xffff);

        break;
    default:
        Event_DT_Base();
        setAction(&daNpcChin_c::wait);

        break;
    }
}

BOOL daNpcChin_c::isGameEnd(int& param_0) {
    param_0 = -1;

    int restTimeMs = dTimer_getRestTimeMs();
    if (restTimeMs <= 0) {
        Z2GetAudioMgr()->seStart(Z2SE_SY_MG_TIMEUP,NULL,0,0,1.0f,1.0f,-1.0f,-1.0f,0);
        return 1;
    }

    if (restTimeMs <= 20000) {
        int unkInt1 = field_0xe14 * 1000;
        if (field_0xe18 > unkInt1 && restTimeMs <= unkInt1) {
            if (field_0xe14 > 0 && field_0xe14 <= 5) {
                Z2GetAudioMgr()->seStart(Z2SE_SY_MG_TIMER_5, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
            } else if (field_0xe14 > 5 && field_0xe14 <= 10) {
                Z2GetAudioMgr()->seStart(Z2SE_SY_MG_TIMER_10, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            } else {
                Z2GetAudioMgr()->seStart(Z2SE_SY_MG_TIMER_20, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            }

            field_0xe14--;
        }

        field_0xe18 = restTimeMs;
    } else {
        field_0xe18 = 1000000;
        field_0xe14 = 20;
    }

    if (_chkGameClear()) {
        param_0 = mGameTime - restTimeMs;
        return 2;
    }

    return 0;
}

BOOL daNpcChin_c::_chkGameClear() {
    return mpSphMng->isAllGet();
}

void daNpcChin_c::SphereReset() {
    mpSphMng->setting();
}

int daNpcChin_c::_Evt_GameStart(int param_0) {
    dEvent_manager_c* this_00 = &dComIfGp_getEventManager();

    int* a_nowCut_p = NULL;
    int a_nowCut = -1;

    a_nowCut_p = dComIfGp_evmng_getMyIntegerP(param_0, "cutId");
    if (a_nowCut_p != NULL) {
        a_nowCut = *a_nowCut_p;
    } else {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    OS_REPORT("a_nowCut=%d\n", a_nowCut);

    if (this_00->getIsAddvance(param_0)) {
        _Evt_GameStart_CutInit(a_nowCut);
    }

    return _Evt_GameStart_CutMain(a_nowCut);
}

int daNpcChin_c::_Evt_GameStart_CutInit(const int& param_0) {
    switch (param_0) {
    case 10:
        mSpotLight.switch_off();
        SphereReset();
        mIsMiniGamePlaying = 1;

        break;
    case 20:
        if (field_0xe07 == 0) {
            mGameTime = 30000;
        }
        else if (field_0xe07 == 1) {
            mGameTime = 90000;
        }
        else if (field_0xe07 == 2) {
            mGameTime = dComIfGs_getHookGameTime();
        }

        OS_REPORT("GAME_TIME=%d\n", mGameTime);

        if (dTimer_createTimer(6, mGameTime, 1, 0, 210.0f, 410.0f, 32.0f, 419.0f) == -1) {
            OS_REPORT("-------daNpcChin_c :: GAME TIMER CREATE MISS!!\n");
            JUT_ASSERT(2914, FALSE);
        }

        Z2GetAudioMgr()->subBgmStart(Z2BGM_WCS_GAME);
    case 30:
        break;
    default:
        JUT_ASSERT(2933, FALSE);
    }

    return 1;
}

int daNpcChin_c::_Evt_GameStart_CutMain(const int& param_0) {
    int result = 0;
    switch (param_0) {
    case 10:
        return 1;
    case 20:
        if (dComIfG_getTimerPtr() != NULL) {
            dComIfG_TimerStart(6,0);
            result = 1;
        }
        break;
    case 30:
        if (dTimer_isStart()) {
            result = 1;
        }
        break;
    default:
        JUT_ASSERT(2993, FALSE);
        result = 1; // unreachable
    }

    return result;
}

int daNpcChin_c::_Evt_GameFailed(int param_0) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* piVar1 = NULL;
    int local_18 = -1;

    piVar1 = dComIfGp_evmng_getMyIntegerP(param_0, "cutId");
    if (piVar1 != NULL) {
        local_18 = *piVar1;
    } else {
        return 1;
    }

    if (eventManager->getIsAddvance(param_0)) {
        _Evt_GameFailed_CutInit(local_18);
    }

    return _Evt_GameFailed_CutMain(local_18);
}

int daNpcChin_c::_Evt_GameFailed_CutInit(const int& param_0) {
    switch (param_0) {
    case 10:
        setMotion(0, -1.0f, 0);
        setAngle(fopAcM_searchPlayerAngleY(this));

        break;
    case 20:
        field_0xe00 = 0x43a;
        mMsgTimer = 0;

        dComIfGp_event_offHindFlag(0x80);
        daNpcF_offTmpBit(11);

        initTalk(field_0xe00, NULL);
        ForcibleTalk_Off();

        setExpression(0, -1.0f);
        setLookMode(3);

        break;
    }
    return 1;
}

int daNpcChin_c::_Evt_GameFailed_CutMain(const int& param_0) {
    int result = 0;

    switch (param_0) {
    case 10:
        return 1;
    case 20:
        if (talkProc(NULL, 1, NULL)) {
            setExpression(14, -1.0f);
            result = 1;
        } else {
            _talk_motion();
        }
        break;
    }

    return result;
}

int daNpcChin_c::_Evt_GameSucceed(int param_0) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();

    int* cutId_p = NULL;
    int cutId = -1;
    cutId_p = dComIfGp_evmng_getMyIntegerP(param_0, "cutId");
    if (cutId_p != NULL) {
        cutId = *cutId_p;
    } else {
        return 1;
    }

    if (eventManager->getIsAddvance(param_0)) {
        _Evt_GameSucceed_CutInit(cutId);
    }

    return _Evt_GameSucceed_CutMain(cutId);
}

int daNpcChin_c::_Evt_GameSucceed_CutInit(const int& param_0) {
    switch (param_0) {
    case 10:
        break;
    case 20:
        field_0xe00 = 0x43a;
        mMsgTimer = 0;

        if (field_0xe07 == 0) {
            daNpcF_onTmpBit(0x4e);
        } else if (field_0xe07 == 1 || field_0xe07 == 2) {
            daNpcF_onTmpBit(0x4f);
        }

        daNpcF_offTmpBit(11);
        initTalk(field_0xe00, NULL);
        ForcibleTalk_Off();
        setExpression(0, -1.0f);
        setLookMode(3);

        break;
    case 30:
        break;
    case 40:
        field_0xe00 = 0x43a;
        mMsgTimer = 0;

        if (field_0xe07 == 0) {
            daNpcF_onTmpBit(0x4e);
        } else if (field_0xe07 == 1 || field_0xe07 == 2) {
            daNpcF_onTmpBit(0x4f);
        }

        initTalk(field_0xe00, NULL);
        ForcibleTalk_Off();
        setLookMode(3);

        break;
    }
    return 1;
}

int daNpcChin_c::_Evt_GameSucceed_CutMain(const int& param_0) {
    int itemId1;

    int result = 0;
    switch (param_0) {
    case 10:
        setAngle(fopAcM_searchPlayerAngleY(this));
        return 1;
    case 20:
        if (talkProc(NULL, 1, NULL)) {
            mActorMngrs[0].entry(daPy_getPlayerActorClass());

            itemId1 = 0;

            u16 eventId = mFlow.getEventId(&itemId1);
            if (eventId != 1) {
                itemId1 = 0;
            }

            fpc_ProcID itemId2 = fopAcM_createItemForPresentDemo(&current.pos, itemId1, 0, -1, -1,
                                                                 0, 0);
            if (itemId2 != -1) {
                dComIfGp_event_setItemPartnerId(itemId2);
            }

            setExpression(14, -1.0f);

            result = 1;
        } else {
            _talk_motion();
        }
        break;
    case 30:
        return 1;
    case 40: {
        if (talkProc(NULL, 1, NULL)) {
            setExpression(14, -1.0f);
            result = 1;
            break;
        }

        _talk_motion();

        break;
    }
    }
    return result;
}

int daNpcChin_c::_Evt_SelectGiveUp(int param_0) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* cutId_p = NULL;

    int cutId = -1;
    cutId_p = dComIfGp_evmng_getMyIntegerP(param_0, "cutId");
    if (cutId_p != NULL) {
        cutId = *cutId_p;
    } else {
        return 1;
    }

    if (eventManager->getIsAddvance(param_0)) {
        _Evt_SelectGiveUp_CutInit(cutId);
    }

    return _Evt_SelectGiveUp_CutMain(cutId);
}

int daNpcChin_c::_Evt_SelectGiveUp_CutInit(const int& param_0) {
    switch (param_0) {
    case 10:
        mTurnMode = 0;
        dTimer_hide();
        setLookMode(0);
        setAngle(25899);

        break;
    case 20:
        daNpcF_offTmpBit(11);
        setExpression(0, -1.0f);
        field_0xe00 = 0x43e;
        initTalk(field_0xe00, NULL);

        break;
    }

    return 1;
}

int daNpcChin_c::_Evt_SelectGiveUp_CutMain(const int& param_0) {
    int result = 0;

    switch (param_0) {
    case 10:
        result = 1;

        break;
    case 20:
        if (talkProc(NULL, 1, NULL)) {
            mTurnMode = 0;
            dTimer_show();
            result = 1;
        } else {
            _talk_motion();
        }

        break;
    }

    return result;
}

int daNpcChin_c::_Evt_GameGiveUp(int param_0) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();

    int* cutId_p = NULL;
    int cutId = -1;
    cutId_p = dComIfGp_evmng_getMyIntegerP(param_0, "cutId");

    if (cutId_p != NULL) {
        cutId = *cutId_p;
    } else {
        return 1;
    }

    if (eventManager->getIsAddvance(param_0)) {
        _Evt_GameGiveUp_CutInit(cutId);
    }

    return _Evt_GameGiveUp_CutMain(cutId);
}

int daNpcChin_c::_Evt_GameGiveUp_CutInit(const int& param_0) {
    switch (param_0) {
    case 10:
        setMotion(0, -1.0f, 0);
        setExpression(14, -1.0f);
        setAngle(fopAcM_searchPlayerAngleY(this));

        break;
    case 20:
        field_0xe00 = 0x43e;
        mMsgTimer = 0;

        initTalk(field_0xe00,NULL);
        ForcibleTalk_Off();
        setExpression(0, -1.0f);
        setLookMode(3);

        break;
    }

    return 1;
}

int daNpcChin_c::_Evt_GameGiveUp_CutMain(const int& param_0) {
    int result = 0;

    switch (param_0) {
    case 10:
        return 1;
    case 20:
        if (talkProc(NULL, 1, NULL)) {
            setExpression(14, -1.0f);
            result = 1;
        } else {
            _talk_motion();
        }

        break;
    }

    return result;
}

int daNpcChin_c::_Evt_Appear(int param_0) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* cutId_p = NULL;

    int cutId = -1;
    cutId_p = dComIfGp_evmng_getMyIntegerP(param_0, "cutId");
    if (cutId_p != NULL) {
        cutId = *cutId_p;
    } else {
        return 1;
    }

    if (eventManager->getIsAddvance(param_0)) {
        _Evt_Appear_CutInit(cutId);
    }

    return _Evt_Appear_CutMain(cutId);
}

int daNpcChin_c::_Evt_Appear_CutInit(const int& param_0) {
    switch (param_0) {
    case 10:
        dKy_change_colpat(0);
        mSpotLight.switch_on();

        break;
    case 20:
        mMsgTimer = 0;
        daNpcF_offTmpBit(11);
        initTalk(field_0xe00, NULL);
        ForcibleTalk_Off();
        setExpression(0, -1.0f);
        setLookMode(3);

        break;
    case 30:
        mMsgTimer = 0;
        Z2GetAudioMgr()->bgmStart(Z2BGM_MINIGAME_ROOM, 0, 0);
        initTalk(field_0xe00, NULL);
        ForcibleTalk_Off();
        setExpression(0, -1.0f);
        setLookMode(3);

        break;
    }

    return 1;
}

int daNpcChin_c::_Evt_Appear_CutMain(const int& param_0) {
    int result = 0;

    switch (param_0) {
    case 10:
        result = 1;
        break;
    case 20:
        if (talkProc(NULL, 1, NULL)) {
            result = 1;
        } else {
            _talk_motion();
        }

        break;
    case 30:
        if (talkProc(NULL,1,NULL)) {
            result = 1;
        } else {
            _talk_motion();
        }

        break;
    default:
        result = 1;
        break;
    }

    return result;
}

int daNpcChin_c::_Evt_SpotLight_Off(int param_0) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int *cutId_p = NULL;
    int cutId = -1;

    cutId_p = dComIfGp_evmng_getMyIntegerP(param_0,"cutId");
    if (cutId_p != NULL) {
        cutId = *cutId_p;
    } else {
        return 1;
    }

    if (eventManager->getIsAddvance(param_0)) {
        _Evt_SpotLight_Off_CutInit(cutId);
    }

    return _Evt_SpotLight_Off_CutMain(cutId);
}

int daNpcChin_c::_Evt_SpotLight_Off_CutInit(const int& param_0) {
    switch (param_0) {
    case 10:
        setExpression(14, -1.0f);
        setMotion(0, -1.0f, 0);

        mSpotLight.switch_off();

        field_0xdf8 = 10;

        break;
    }

    return 1;
}

int daNpcChin_c::_Evt_SpotLight_Off_CutMain(const int& param_0) {
    int result = 0;

    switch (param_0) {
    case 10:
        if (--field_0xdf8 <= 0) {
            result = 1;
        }

        break;
    default:
        result = 1;

        break;
    }

    return result;
}

static int daNpcChin_Create(void* i_this) {
    return ((daNpcChin_c*)i_this)->Create();
}

static int daNpcChin_Delete(void* i_this) {
    return ((daNpcChin_c*)i_this)->Delete();
}

static int daNpcChin_Execute(void* i_this) {
    return ((daNpcChin_c*)i_this)->Execute();
}

static int daNpcChin_Draw(void* i_this) {
    return ((daNpcChin_c*)i_this)->Draw();
}

static int daNpcChin_IsDelete(void* param_0) {
    return TRUE;
}

static actor_method_class daNpcChin_MethodTable = {
    (process_method_func)daNpcChin_Create,
    (process_method_func)daNpcChin_Delete,
    (process_method_func)daNpcChin_Execute,
    (process_method_func)daNpcChin_IsDelete,
    (process_method_func)daNpcChin_Draw,
};

actor_process_profile_definition g_profile_NPC_CHIN = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_NPC_CHIN,          // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daNpcChin_c),    // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    424,                    // mPriority
    &daNpcChin_MethodTable, // sub_method
    0x00044100,             // mStatus
    fopAc_NPC_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
  };
