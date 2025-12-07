#include "d/actor/d_a_npc4.h"
#include "d/actor/d_a_npc2.h"
#include "d/actor/d_a_npc_tk.h"
#include "d/d_debug_viewer.h"
#include "d/d_msg_object.h"
#include "d/actor/d_a_tag_evtarea.h"

static u16 daNpcF_putNurbs(dPnt* param_0, int param_1, int param_2, dPnt* param_3, int param_4);
u8 daNpcF_getDistTableIdx(int param_0, int param_1);

static BOOL daNpcF_addIdx(int i_step, int i_num, u16& i_idx, BOOL i_isClosed) {
    int ret = false;
    int new_idx = i_idx;
    new_idx += i_step;

    if (i_num <= new_idx) {
        if (i_isClosed) {
            new_idx %= i_num;
        } else {
            new_idx = i_num - 1;
            ret = true;
        }
    }

    i_idx = new_idx;
    return ret;
}

static BOOL daNpcF_subIdx(int i_step, int i_num, u16& i_idx, BOOL i_isClosed) {
    BOOL ret = false;
    int new_idx = i_idx;
    new_idx -= i_step;

    if (0 > new_idx) {
        if (i_isClosed) {
            new_idx = (i_num + (new_idx % i_num)) % i_num;
        } else {
            new_idx = 0;
            ret = true;
        }
    }

    i_idx = new_idx;
    return ret;
}

static BOOL daNpcF_incIdx(int i_num, u16& i_idx, BOOL i_isClosed, int i_reverse) {
    BOOL result = FALSE;
    if (i_reverse) {
        result = daNpcF_subIdx(1, i_num, i_idx, i_isClosed);
    } else {
        result = daNpcF_addIdx(1, i_num, i_idx, i_isClosed);
    }
    return result;
}

static BOOL daNpcF_decIdx(int i_num, u16& i_idx, BOOL i_isClosed, int i_reverse) {
    BOOL result = FALSE;
    if (i_reverse) {
        result = daNpcF_addIdx(1, i_num, i_idx, i_isClosed);
    } else {
        result = daNpcF_subIdx(1, i_num, i_idx, i_isClosed);
    }
    return result;
}

void daNpcF_ActorMngr_c::initialize() {
    mActorID = fpcM_ERROR_PROCESS_ID_e;
}

void daNpcF_ActorMngr_c::entry(fopAc_ac_c* i_actor) {
    mActorID = fopAcM_GetID(i_actor);
}

void daNpcF_ActorMngr_c::entry(fpc_ProcID i_actorID) {
    mActorID = i_actorID;
}

void daNpcF_ActorMngr_c::remove() {
    mActorID = fpcM_ERROR_PROCESS_ID_e;
}

fopAc_ac_c* daNpcF_ActorMngr_c::getActorP() {
    fopAc_ac_c* actor = NULL;

    BOOL foundActor = fopAcM_SearchByID(mActorID, &actor);
    if (foundActor == TRUE && actor != NULL) {
        return actor;
    }

    return NULL;
}

void daNpcF_MatAnm_c::initialize() {
    field_0xF4 = 0.0f;
    field_0xF8 = 0.0f;
    mNowOffsetX = 0.0f;
    mNowOffsetY = 0.0f;
    mEyeMoveFlag = 0;
    mMorfFrm = 0;
}

void daNpcF_MatAnm_c::calc(J3DMaterial* i_material) const {
    J3DMaterialAnm::calc(i_material);

    for (u32 i = 0; i < 8; i++) {
        if (J3DMaterialAnm::getTexMtxAnm(i).getAnmFlag()) {
            J3DTextureSRTInfo* curr_mtx_info =
                &i_material->getTexGenBlock()->getTexMtx(i)->getTexMtxInfo().mSRT;

            if (mMorfFrm != 0) {
                f32 tmp8 = 1.0f / (mMorfFrm + 1);

                curr_mtx_info->mTranslationX =
                    field_0xF4 * (1.0f - tmp8) + curr_mtx_info->mTranslationX * tmp8;
                curr_mtx_info->mTranslationY =
                    field_0xF8 * (1.0f - tmp8) + curr_mtx_info->mTranslationY * tmp8;

            } else {
                if (mEyeMoveFlag != 0) {
                    curr_mtx_info->mTranslationX = mNowOffsetX;
                    curr_mtx_info->mTranslationY = mNowOffsetY;
                }
            }

            field_0xF4 = curr_mtx_info->mTranslationX;
            field_0xF8 = curr_mtx_info->mTranslationY;
        }
    }
}

void daNpcF_SPCurve_c::initialize(dPath* i_path, int i_isReversed) {
    mNurbs = 0;
    field_0x02 = 0;
    mIsReversed = i_isReversed;
    mIsClosed = false;

    for (int i = 0; i < 96; i++) {
        mPoints[i].m_position.x = 0.0f;
        mPoints[i].m_position.y = 0.0f;
        mPoints[i].m_position.z = 0.0f;
    }

    if (i_path) {
        mIsClosed = dPath_ChkClose(i_path);
        mNurbs = daNpcF_putNurbs(i_path->m_points, i_path->m_num, 96, mPoints, mIsClosed);
    }
}

void daNpcF_Path_c::initialize() {
    mIdx = 0;
    mIsReversed = false;
    mIsClosed = false;
    mPathInfo = NULL;

    mSPCurve.initialize(NULL, false);

    mRange = 0.0f;
    mPosDst = 0.0f;
    field_0x10 = 0.0f;
}

#if DEBUG
void daNpcF_Path_c::drawDbgInfoXyz() {
    GXColor green = {0x80, 0xff, 0x80, 0xff};
    GXColor blue = {0x80, 0x80, 0xff, 0xff};
    GXColor red = {0xff, 0x80, 0x80, 0xff};
    GXColor yellow = {0xff, 0xff, 0x80, 0xff};

    cXyz unkXyz1;
    cXyz unkXyz2;

    for (int i = 0; i <= getNumPnts(); i++) {
        if (i < getNumPnts()) {
            unkXyz1.set(getPntPos(i).x, getPntPos(i).y, getPntPos(i).z);
            if (i != 0) {
                unkXyz2.set(getPntPos(i - 1).x, getPntPos(i - 1).y, getPntPos(i - 1).z);
                dDbVw_drawLineOpa(unkXyz1, unkXyz2, red, 1, 12);
            }

            if (i == getIdx()) {
                dDbVw_drawSphereOpa(unkXyz1, 50.0f, yellow, 1);
            } else {
                dDbVw_drawSphereOpa(unkXyz1, 9.0f, (GXColor){0xff, 0xff, 0xff, 0xff}, 1);
            }
        } else if (mIsClosed) {
            unkXyz1.set(getPntPos(i - 1).x, getPntPos(i - 1).y, getPntPos(i - 1).z);
            unkXyz2.set(getPntPos(0).x, getPntPos(0).y, getPntPos(0).z);
            dDbVw_drawLineOpa(unkXyz1, unkXyz2, red, 1, 12);
        }
    }
}
#endif

int daNpcF_Path_c::setPathInfo(u8 i_pathNo, s8 i_roomNo, u8 i_isReversed) {
    mPathInfo = NULL;
    mIdx = 0;
    mIsReversed = i_isReversed;

    if (i_pathNo != 0xFF) {
        mPathInfo = dPath_GetRoomPath(i_pathNo, i_roomNo);
        if (mPathInfo == NULL) {
            return 0;
        }

        mSPCurve.initialize(mPathInfo, mIsReversed);
        mIsClosed = dPath_ChkClose(mPathInfo);
        mPosition = getPntPos(getIdx());
    }

    return 1;
}

BOOL daNpcF_Path_c::chkPassed(cXyz i_pos) {
    return daNpcF_chkPassed(i_pos, (dPnt*)mPathInfo->m_points, mIdx, mPathInfo->m_num,
                            mIsClosed, mIsReversed);
}

BOOL daNpcF_Path_c::chkPassedDst(cXyz i_pos) {
    return mPosDst + field_0x10 <= (mPosition - i_pos).abs();
}

void daNpcF_Path_c::reverse() {
    if (chkReverse()) {
        offReverse();
    } else {
        onReverse();
    }
}

BOOL daNpcF_Path_c::setNextIdx() {
    return daNpcF_incIdx(mPathInfo->m_num, mIdx, chkClose(), mIsReversed);
}

int daNpcF_Path_c::getNextIdx() {
    u16 ret = mIdx;
    daNpcF_incIdx(mPathInfo->m_num, ret, chkClose(), mIsReversed);
    return ret;
}

int daNpcF_Path_c::getBeforeIdx() {
    u16 ret = mIdx;
    daNpcF_decIdx(mPathInfo->m_num, ret, chkClose(), mIsReversed);
    return ret;
}

BOOL daNpcF_Path_c::getBeforePos(cXyz& o_pos) {
    BOOL ret = FALSE;
    u16 idx = mIdx;
    ret = daNpcF_decIdx(mPathInfo->m_num, idx, chkClose(), mIsReversed);

    o_pos = getPntPos(idx);
    return ret;
}

BOOL daNpcF_Path_c::getNextPos(cXyz& o_pos) {
    BOOL ret = FALSE;
    u16 idx = mIdx;

    ret = daNpcF_incIdx(mPathInfo->m_num, idx, chkClose(), mIsReversed);

    o_pos = getPntPos(idx);
    return ret;
}

int daNpcF_Path_c::getDstPos(cXyz i_pos1, cXyz& o_pos2) {
    BOOL ret = FALSE;

    while (!ret) {
        o_pos2 = getPntPos(getIdx());

        if (chkPassed(i_pos1)) {
            if (setNextIdx()) {
                ret = true;
            }
        } else {
            break;
        }
    }

    return ret;
}

void daNpcF_Path_c::setNextIdxDst(cXyz i_pos) {
    if (!setNextIdx()) {
        mPosDst = (i_pos - getPntPos(getIdx())).abs();
        mPosition = i_pos;
    }
}

void daNpcF_Lookat_c::initialize() {
    for (int i = 0; i < 4; i++) {
        mJointPos[i].setall(0.0f);
        mAngularMoveDis[i].setall(0);
        mMinAngle[i].setall(0);
        mMaxAngle[i].setall(0);
        mRotAngle[i].setall(0);
        field_0x94[i] = false;
    }

    mAttnPos_p = NULL;
}

void daNpcF_Lookat_c::setParam(f32 param_0, f32 param_1, f32 param_2, f32 param_3, f32 param_4,
                               f32 param_5, f32 param_6, f32 param_7, f32 param_8, f32 param_9,
                               f32 param_10, f32 param_11, s16 i_rotAngle, cXyz* i_jointPos) {
    static cXyz vec(0.0f, 0.0f, 32.0f);

    mJointPos[0] = i_jointPos[0];
    mMinAngle[0].x = cM_deg2s(-param_1) - 0x4000;
    mMinAngle[0].y = cM_deg2s(param_2);
    mMinAngle[0].z = 0;
    mMaxAngle[0].x = cM_deg2s(-param_0) - 0x4000;
    mMaxAngle[0].y = cM_deg2s(param_3);
    mMaxAngle[0].z = 0;

    mJointPos[1] = i_jointPos[1];
    mMinAngle[1].set(-0x4000, 0, 0);
    mMaxAngle[1].set(-0x4000, 0, 0);

    mJointPos[2] = i_jointPos[2];
    mMinAngle[2].x = cM_deg2s(-param_9);
    mMinAngle[2].y = cM_deg2s(param_10);
    mMinAngle[2].z = 0;
    mMaxAngle[2].x = cM_deg2s(-param_8);
    mMaxAngle[2].y = cM_deg2s(param_11);
    mMaxAngle[2].z = 0;

    mDoMtx_stack_c::transS(mJointPos[2]);
    mDoMtx_stack_c::YrotM(i_rotAngle);
    mDoMtx_stack_c::multVec(&vec, &mJointPos[3]);

    mMinAngle[3].set(0, 0, 0);
    mMaxAngle[3].set(0, 0, 0);
}

void daNpcF_Lookat_c::calc(fopAc_ac_c* i_actor, Mtx i_baseTransformMtx, csXyz** o_lookatAngle,
                           BOOL i_snap, int i_axis, BOOL i_debug) {
    cXyz sp68[4];
    csXyz sp50[4];
    cXyz sp40;
    f32 sp30[4];
    f32 sp1C;
    f32 angY;

    if (mAttnPos_p != NULL) {
        initCalc(i_actor, i_baseTransformMtx, sp68, sp50, sp30, sp40, i_debug);

        for (int i = 2; i >= -1; i--) {
            update(sp68, sp50, sp30);
            if (i >= 0) {
                calcMoveDisAngle(i, sp68, sp50, sp40, i_axis, i_debug);
            }
        }

        setRotAngle();
    } else {
        clrRotAngle();
    }

    int i, j;
    for (i = 2, j = 1; i >= 0; i--, j++) {
        if (i_snap) {
            *(o_lookatAngle[i]) = mRotAngle[i];
        } else {
            sp1C = o_lookatAngle[i]->x;
            angY = mRotAngle[i].x;
            cLib_addCalc2(&sp1C, angY, 0.25f, (f32)(0x800 / j));
            o_lookatAngle[i]->x = sp1C;

            f32 var_f30 = mRotAngle[i].y - o_lookatAngle[i]->y;
            sp1C = o_lookatAngle[i]->y;
            angY = mRotAngle[i].y;
            cLib_addCalc2(&sp1C, angY, 0.25f, (f32)(0x800 / j));

            if (i_debug) {
                OS_REPORT("|%d| a:%f(%d), b:%f(%d), axis:%d\n", i, sp1C, o_lookatAngle[i]->y, angY, mRotAngle[i].y, i_axis);
            }

            if (i_axis > 0 && angY - sp1C < 0.0f) {
                sp1C = angY;
            }
            if (i_axis < 0 && 0.0f < angY - sp1C) {
                sp1C = angY;
            }
            o_lookatAngle[i]->y = (s16)sp1C;

            o_lookatAngle[i]->z = 0;
        }
    }

    if (i_debug) {
        OS_REPORT("\n");
    }
}

void daNpcF_Lookat_c::adjustMoveDisAngle(s16& delta, s16 angle, s16 min, s16 max) {
    int newAngle = 0;

    newAngle = angle + delta;
    if (max < newAngle) {
        if (angle < max) {
            delta = delta - (newAngle - max);
        } else {
            delta = 0;
        }
    }

    newAngle = angle + delta;
    if (newAngle < min) {
        if (min < angle) {
            delta = delta - (newAngle - min);
        } else {
            delta = 0;
        }
    }
}

void daNpcF_Lookat_c::initCalc(fopAc_ac_c* i_actor, Mtx i_baseTransformMtx, cXyz* param_2,
                               csXyz* param_3, f32* param_4, cXyz& param_5, int i_debug) {
    Mtx mtx;
    cXyz local_90;
    cMtx_copy(i_baseTransformMtx, mtx);
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;

    mDoMtx_stack_c::copy(mtx);
    mDoMtx_stack_c::inverse();
    cMtx_copy(mDoMtx_stack_c::get(), mtx);
    mDoMtx_stack_c::transS(i_actor->current.pos);
    mDoMtx_stack_c::concat(mtx);

    for (int i = 0; i < 4; i++) {
        local_90 = mJointPos[i] - i_actor->current.pos;
        mDoMtx_stack_c::multVec(&local_90, &param_2[i]);
    }

    JUT_ASSERT(1244, NULL != mAttnPos_p);

    local_90 = *mAttnPos_p - i_actor->current.pos;
    mDoMtx_stack_c::multVec(&local_90, &param_5);

    for (int i = 0; i < 3; i++) {
        local_90 = (param_2 + i)[1] - (param_2 + i)[0];
        param_4[i] = local_90.abs();
        param_3[i].setall(0);
        param_3[i].x = -cM_atan2s(local_90.y, local_90.absXZ());

        #if DEBUG
        if (i_debug) {
            f32 var_f31 = fabsf(cM_ssin(param_3[i].x));
            OS_REPORT(">>>>>>>%d: sin(%d):%f\n", i, abs(param_3[i].x), var_f31);
        }
        #endif

        if (fabsf(cM_ssin(param_3[i].x)) < 0.63f) {
            param_3[i].y = cM_atan2s(local_90.x, local_90.z);
        } else {
            param_3[i].y = 0;
        }
    }
}

void daNpcF_Lookat_c::update(cXyz* param_0, csXyz* param_1, f32* param_2) {
    csXyz ang = csXyz::Zero;
    cXyz vec;
    Mtx mtx;
    cXyz* pparam0i;

    for (int i = 0; i < 3; i++) {
        mDoMtx_stack_c::XYZrotS(param_1[i]);
        cMtx_copy(mDoMtx_stack_c::get(), mtx);
        mDoMtx_stack_c::transS(param_0[i]);

        ang.x += mAngularMoveDis[i].x;
        ang.y += mAngularMoveDis[i].y;
        mDoMtx_stack_c::ZXYrotM(ang);
        mDoMtx_stack_c::concat(mtx);

        vec.set(0.0f, 0.0f, param_2[i]);
        mDoMtx_stack_c::multVec(&vec, &param_0[i] + 1);
    }
}

void daNpcF_Lookat_c::calcMoveDisAngle(int i_no, cXyz* param_1, csXyz* param_2, cXyz param_3,
                                       int i_axis, int i_debug) {
    cXyz vec1;
    cXyz vec2;

    if (i_axis == 0) {
        field_0x94[i_no] = false;
    }

    s16 pitch1, yaw1, pitch2, yaw2, mvx, mvy;
    if (!field_0x94[i_no]) {
        vec1 = param_3 - param_1[i_no];
        if (!vec1.isZero()) {
            vec1.normalize();
            vec2 = param_1[3] - param_1[i_no];
            if (!vec2.isZero()) {
                vec2.normalize();
                pitch1 = -cM_atan2s(vec1.y, vec1.absXZ());
                yaw1 = cM_atan2s(vec1.x, vec1.z);
                pitch2 = -cM_atan2s(vec2.y, vec2.absXZ());
                yaw2 = cM_atan2s(vec2.x, vec2.z);
                mvx = pitch1 - pitch2;
                mvy = yaw1 - yaw2;
                mAngularMoveDis[i_no].x += mvx;
                mAngularMoveDis[i_no].y += mvy;
                if (i_debug) {
                    OS_REPORT("*******%d: mvy:%d, mAngularMoveDis:%d, angle:%d(%d/%d)\n",
                              i_no, mvy, mAngularMoveDis[i_no].y, param_2[i_no].y,
                              mMinAngle[i_no].y, mMaxAngle[i_no].y);
                }
            }
        }
    }

    adjustMoveDisAngle(mAngularMoveDis[i_no].x, param_2[i_no].x,
                       mMinAngle[i_no].x, mMaxAngle[i_no].x);
    adjustMoveDisAngle(mAngularMoveDis[i_no].y, param_2[i_no].y,
                       mMinAngle[i_no].y, mMaxAngle[i_no].y);

    if (i_debug) {
        OS_REPORT("+++++++%d: mvy:%d, mAngularMoveDis:%d\n",
                  i_no, mvy, mAngularMoveDis[i_no].y);
    }

    if (i_axis < 0 && mAngularMoveDis[i_no].y > 0) {
        field_0x94[i_no] = false;
    }
    if (i_axis > 0 && mAngularMoveDis[i_no].y < 0) {
        field_0x94[i_no] = false;
    }
}

void daNpcF_Lookat_c::setRotAngle() {
    int ang_x = 0;
    int ang_y = 0;
    for (int i = 0; i < 4; i++) {
        ang_x += mAngularMoveDis[i].x;
        ang_x = cLib_minMaxLimit(ang_x, -0x8000, 0x7fff);
        ang_y += mAngularMoveDis[i].y;
        ang_y = cLib_minMaxLimit(ang_y, -0x8000, 0x7fff);

        mRotAngle[i].x = ang_x;
        mRotAngle[i].y = ang_y;
        mRotAngle[i].z = 0;
    }
}

void daNpcF_Lookat_c::clrRotAngle() {
    for (int i = 0; i < 4; i++) {
        mRotAngle[i].x = 0;
        mRotAngle[i].y = 0;
        mRotAngle[i].z = 0;
    }
}

BOOL daNpcF_c::execute() {
    setParam();

    if (main()) {
        fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        if (mGroundH != -G_CM3D_F_INF) {
            field_0x998 = daNpcF_getGroundAngle(&mGndChk, mCurAngle.y);
            setEnvTevColor();
            setRoomNo();
            mPolySound = dKy_pol_sound_get(&mAcch.m_gnd);
            mReverb = dComIfGp_getReverb(mCcStts.GetRoomId());
            if (mAcch.ChkWaterHit() && mAcch.m_wtr.GetHeight() > mGroundH) {
                mPolySound = dKy_pol_sound_get(&mAcch.m_wtr);
            }
        }
    }

    playAllAnm();
    adjustShapeAngle();
    setAttnPos();

    cLib_chaseF(&mExpressionMorf, 0.0f, 1.0f);
    setCollisions();

    if (mTwilight) {
        attention_info.flags |= fopAc_AttnFlag_UNK_0x400000;
        attention_info.flags |= fopAc_AttnFlag_UNK_0x800000;
        setHitodamaPrtcl();
    }

    mOldAngle = mCurAngle;
    mCutType = 0;
    mOrderSpeakEvt = false;
    mOrderNewEvt = false;
    field_0x9eb = false;
    field_0x9ef = 0;
    field_0x9f3 = 0;

    for (int i = 0; i < 5; i++) {
        if ((mAttnChangeTimer == 0 || mAttnIdx != i) && mAttnActorTimer[i] != 0 &&
            cLib_calcTimer(&mAttnActorTimer[i]) == 0)
        {
            mAttnActor[i].remove();
        }
    }

    if (mDamageTimer != 0) {
        cLib_calcTimer(&mDamageTimer);
    }

    return true;
}

int daNpcF_c::draw(BOOL i_isTest, BOOL param_1, f32 i_shadowDepth, _GXColorS10* i_fogColor,
                   BOOL i_hideDamage) {
    f32 damage_ratio, frame;
    J3DModel* model = mAnm_p->getModel();
    J3DModelData* modelData = model->getModelData();
    field_0x9f3 = 1;

    if (!checkHide()) {
        if (!i_hideDamage && mDamageTimer != 0 && mTotalDamageTimer != 0) {
            damage_ratio = (f32)mDamageTimer / (f32)mTotalDamageTimer;
        } else {
            damage_ratio = 0.0f;
        }

        if (cM3d_IsZero(damage_ratio) == FALSE) {
            tevStr.TevColor.r = (s16)(damage_ratio * 20.0f);
            tevStr.TevColor.g = 0;
        } else if (i_isTest) {
            tevStr.TevColor.g = 20;
            tevStr.TevColor.r = 0;
        } else if (i_fogColor != NULL) {
            tevStr.TevColor.r = i_fogColor->r;
            tevStr.TevColor.g = i_fogColor->g;
            tevStr.TevColor.b = i_fogColor->b;
            tevStr.TevColor.a = i_fogColor->a;
        } else {
            tevStr.TevColor.g = 0;
            tevStr.TevColor.r = 0;
        }

        if (mTwilight) {
            g_env_light.settingTevStruct(4, &current.pos, &tevStr);
        } else {
            g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        }
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);

        if (!drawDbgInfo()) {
            if (mAnmFlags & ANM_PLAY_BTP) {
                mBtpAnm.entry(modelData);
            }
            if (mAnmFlags & ANM_PLAY_BTK) {
                mBtkAnm.entry(modelData);
            }
            if (mAnmFlags & ANM_PLAY_BRK) {
                mBrkAnm.entry(modelData);
            }

            if (param_1) {
                fopAcM_setEffectMtx(this, modelData);
            }

            if (mTwilight) {
                dComIfGd_setListDark();
                mAnm_p->entryDL();
                dComIfGd_setList();
            } else {
                mAnm_p->entryDL();
            }

            if (mAnmFlags & ANM_PLAY_BTP) {
                mBtpAnm.remove(modelData);
            }
            if (mAnmFlags & ANM_PLAY_BTK) {
                mBtkAnm.remove(modelData);
            }
            if (mAnmFlags & ANM_PLAY_BRK) {
                mBrkAnm.remove(modelData);
            }

            mShadowKey = dComIfGd_setShadow(mShadowKey, true, model, &current.pos, i_shadowDepth,
                                            20.0f, current.pos.y, mGroundH, mGndChk, &tevStr, 0,
                                            1.0f, dDlst_shadowControl_c::getSimpleTex());

            drawOtherMdls();
        }
    }

    return 1;
}

void daNpcF_c::tgHitCallBack(fopAc_ac_c* i_this, dCcD_GObjInf* param_1, fopAc_ac_c* i_actor,
                             dCcD_GObjInf* param_3) {
    if (i_actor != NULL && fopAcM_GetProfName(i_actor) == PROC_ALINK) {
        daNpcF_c* npc = static_cast<daNpcF_c*>(i_this);
        if (daPy_getPlayerActorClass() == i_actor) {
            npc->setCutType(static_cast<daPy_py_c*>(i_actor)->getCutType());
        } else {
            npc->setCutType(daPy_py_c::CUT_TYPE_NM_VERTICAL);
        }
    }
}

dCcD_SrcCyl daNpcF_c::mCcDCyl = {
    daNpcF_c::mCcDObjInfo,
    {
                {0.0f, 0.0f, 0.0f},  // mCenter
                0.0f,  // mRadius
                0.0f  // mHeight
            }  // mCyl
};

dCcD_SrcSph daNpcF_c::mCcDSph = {
    daNpcF_c::mCcDObjInfo,
    {
                {{0.0f, 0.0f, 0.0f}, 0.0f}  // mSph
    }  // mSphAttr
};

char daNpcF_c::mFileNameBuf[0x15];

fopAc_ac_c* daNpcF_c::mFindActorPList[100];

s32 daNpcF_c::mFindCount;

void* daNpcF_c::srchAttnActor1(void* i_proc, void* param_1) {
    void* param_1_copy = param_1;
    fopAc_ac_c* actor = NULL;
    if (mFindCount < 100 && fopAcM_IsActor(i_proc)
        && (fopAcM_GetName(i_proc) == PROC_NI || fopAcM_GetName(i_proc) == PROC_COW
            || fopAcM_GetName(i_proc) == PROC_NPC_NE || fopAcM_GetName(i_proc) == PROC_DO
            || fopAcM_GetName(i_proc) == PROC_SQ || fopAcM_GetName(i_proc) == PROC_BD))
    {
        actor = static_cast<fopAc_ac_c*>(i_proc);
        mFindActorPList[mFindCount] = actor;
        mFindCount++;
    }
    return NULL;
}

s16 daNpcF_c::mSrchActorName;

void* daNpcF_c::srchActor(void* i_proc, void* i_this) {
    void* i_this_copy = i_this;
    fopAc_ac_c* actor = NULL;
    if (mFindCount < 100 && fopAcM_IsActor(i_proc) && i_proc != i_this
        && mSrchActorName == fopAcM_GetName(i_proc))
    {
        actor = static_cast<fopAc_ac_c*>(i_proc);
        mFindActorPList[mFindCount] = actor;
        mFindCount++;
    }

    return NULL;
}

void daNpcF_c::setMtx() {
    J3DModel* model = mAnm_p->getModel();
    J3DModelData* modelData = model->getModelData();
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle);
    mDoMtx_stack_c::scaleM(scale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    model->setUserArea((uintptr_t)this);

    if (mAnmFlags & 0x100) {
        mBckAnm.getBckAnm()->setFrame(mBckAnm.getFrame());
        mAnm_p->modelCalc();
    } else {
        mAnm_p->modelCalc();
    }
}

void daNpcF_c::setMtx2() {
    J3DModel* model = mAnm_p->getModel();
    J3DModelData* modelData = model->getModelData();
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle);
    mDoMtx_stack_c::scaleM(scale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    model->setUserArea((uintptr_t)this);

    if (cM3d_IsZero(mExpressionMorf) == FALSE) {
        mAnm_p->onMorfNone();
    } else {
        mAnm_p->offMorfNone();
    }

    if (mAnmFlags & 0x100) {
        mBckAnm.getBckAnm()->setFrame(mBckAnm.getFrame());
        mAnm_p->modelCalc();
    } else {
        mAnm_p->modelCalc();
    }
}

void daNpcF_c::initialize() {
    field_0x824.remove();

    for (int i = 0; i < 5; i++) {
        mAttnActor[i].remove();
    }

    for (int i = 0; i < 3; i++) {
        mLookatPos[i].setall(0.0f);
    }

    mLookPos.setall(0.0f);
    field_0x884.setall(0.0f);
    field_0x890.setall(0.0f);
    field_0x89c.setall(0.0f);
    field_0x8a8.setall(0.0f);
    mHeadPos.setall(0.0f);
    mCurAngle.setall(0);
    mOldAngle.setall(0);
    mEyeAngle.setall(0);
    mHeadAngle.setall(0);

    for (int i = 0; i < 3; i++) {
        field_0x908[i].setall(0);
        mLookatAngle[i].setall(0);
    }

    mStaffID = -1;
    mCutType = 0;
    mAttnIdx = 0;
    mAttnChangeTimer = 0;

    for (int i = 0; i < 5; i++) {
        mAttnActorTimer[i] = 0;
    }

    mMsgTimer = 0;
    mDamageTimer = 0;
    mTotalDamageTimer = 0;
    field_0x95c = 0;
    mEventTimer = 0;
    mBtpTimer = 0;
    mTurnAmount = 0;
    mTurnMode = 0;
    mTurnStepNum = 0.0f;
    mExpressionMorfOverride = 0.0f;
    mMotionMorfOverride = 0.0f;
    mGroundH = 0.0f;

    for (int i = 0; i < 3; i++) {
        field_0x984[i] = 0.0f;
    }

    field_0x990 = 0;
    field_0x992 = 0;
    mTurnStartAngle = 0;
    mTurnTargetAngle = 0;
    field_0x998 = 0;
    mAnmFlags = 0;
    mShadowKey = 0;
    field_0x9a4 = -1;
    mHitodamaParticleKey[0] = -1;
    mHitodamaParticleKey[1] = -1;
    mEventIdx = -1;
    mExpressionPhase = 0;
    mExpressionPrevPhase = 0;
    mMotionPhase = 0;
    mMotionPrevPhase = 0;
    mExpression = -1;
    mMotion = -1;
    mMotionLoops = 0;
    mExpressionLoops = 0;
    mOrderEvtNo = 0;
    mOrderSpeakEvt = false;
    field_0x9ea = false;
    field_0x9eb = false;
    field_0x9ec = false;
    mOrderNewEvt = false;
    field_0x9ee = false;
    field_0x9ef = 0;
    mIsDamaged = false;
    field_0x9f1 = 0;
    mHide = false;
    field_0x9f3 = 0;
    field_0x9f5 = 1;
    field_0x9f6 = 1;
    field_0x9d2 = cM_rndF(0x10000);
}

J3DAnmTransformKey* daNpcF_c::getTrnsfrmKeyAnmP(char* i_arcName, int i_resIdx) {
    J3DAnmTransformKey* key = (J3DAnmTransformKey*)dComIfG_getObjectRes(i_arcName, i_resIdx);
    return key;
}

J3DAnmTexPattern* daNpcF_c::getTexPtrnAnmP(char* i_arcName, int i_resIdx) {
    J3DAnmTexPattern* pattern = (J3DAnmTexPattern*)dComIfG_getObjectRes(i_arcName, i_resIdx);
    return pattern;
}

J3DAnmTextureSRTKey* daNpcF_c::getTexSRTKeyAnmP(char* i_arcName, int i_resIdx) {
    J3DAnmTextureSRTKey* key = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(i_arcName, i_resIdx);
    return key;
}

J3DAnmTevRegKey* daNpcF_c::getTevRegKeyAnmP(char* i_arcName, int i_resIdx) {
    J3DAnmTevRegKey* key = (J3DAnmTevRegKey*)dComIfG_getObjectRes(i_arcName, i_resIdx);
    return key;
}

BOOL daNpcF_c::setMcaMorfAnm(J3DAnmTransformKey* i_anm, f32 i_speed, f32 i_morf, int i_mode,
                             int i_start, int i_end) {
    f32 start = i_start;
    f32 end = i_end;
    mAnm_p->setAnm(i_anm, i_mode, i_morf, i_speed, start, end);
    return true;
}

BOOL daNpcF_c::setBckAnm(J3DAnmTransform* i_bck, f32 i_speed, int i_mode, int i_start, int i_end,
                         bool i_modify) {
    s16 start = i_start;
    s16 end = i_end;
    return mBckAnm.init(i_bck, true, i_mode, i_speed, start, end, i_modify);
}

BOOL daNpcF_c::setBtpAnm(J3DAnmTexPattern* i_btp, J3DModelData* i_modelData, f32 i_speed,
                         int i_mode) {
    return mBtpAnm.init(i_modelData, i_btp, true, i_mode, i_speed, 0, -1);
}

BOOL daNpcF_c::setBtkAnm(J3DAnmTextureSRTKey* i_btk, J3DModelData* i_modelData, f32 i_speed,
                         int i_mode) {
    return mBtkAnm.init(i_modelData, i_btk, true, i_mode, i_speed, 0, -1);
}

BOOL daNpcF_c::setBrkAnm(J3DAnmTevRegKey* i_brk, J3DModelData* i_modelData, f32 i_speed,
                         int i_mode) {
    return mBrkAnm.init(i_modelData, i_brk, true, i_mode, i_speed, 0, -1);
}

void daNpcF_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daNpcF_c::setRoomNo() {
    s8 room_id = dComIfG_Bgsp().GetRoomId(mGndChk);
    fopAcM_SetRoomNo(this, room_id);
    mCcStts.SetRoomId(room_id);
}

BOOL daNpcF_c::chkEndAnm(f32 param_0) {
    switch ((u8)mAnm_p->getPlayMode()) {
    case J3DFrameCtrl::EMode_LOOP:
        return mAnm_p->isLoop();
    case J3DFrameCtrl::EMode_NONE:
    case J3DFrameCtrl::EMode_RESET:
        return mAnm_p->isStop() && mAnm_p->getPlaySpeed() == 0.0f && param_0 != 0.0f;
    case J3DFrameCtrl::EMode_REVERSE:
    case J3DFrameCtrl::EMode_LOOP_REVERSE:
    default:
        return false;
    }
}

/**
 * Check whether an animation has ended or looped.
 * @param frameCtrl The frame controller for the animation.
 * @param prevRate The previous rate of animation.
 * @return `true` if the animation has just stopped or just looped, `false` otherwise. Returns
 * `false` if the animation is two-way (attribute 3 or 4).
 */
BOOL daNpcF_c::chkEndAnm(J3DFrameCtrl* frameCtrl, f32 prevRate) {
    switch (frameCtrl->getAttribute()) {
    case J3DFrameCtrl::EMode_LOOP:
        return frameCtrl->checkState(2);
    case J3DFrameCtrl::EMode_NONE:
    case J3DFrameCtrl::EMode_RESET:
        return frameCtrl->checkState(1) && frameCtrl->getRate() == 0.0f && prevRate != 0.0f;
    case J3DFrameCtrl::EMode_REVERSE:
    case J3DFrameCtrl::EMode_LOOP_REVERSE:
    default:
        return false;
    }
}

BOOL daNpcF_c::playAllAnm() {
    f32 rate;
    if (!(mAnmFlags & ANM_PAUSE_EXPRESSION)) {
        if (mAnmFlags & ANM_PLAY_BCK) {
            rate = mBckAnm.getPlaySpeed();
            if (mAnmFlags & ANM_PAUSE_BCK) {
                mBckAnm.setPlaySpeed(0.0f);
            }

            mBckAnm.play();

            if (chkEndAnm(mBckAnm.getFrameCtrl(), rate)) {
                mExpressionLoops++;
            }

            mBckAnm.setPlaySpeed(rate);
        }

        if (mAnmFlags & ANM_PLAY_BTP) {
            rate = mBtpAnm.getPlaySpeed();
            if (mAnmFlags & ANM_PAUSE_BTP) {
                mBtpAnm.setPlaySpeed(0.0f);
            }

            mBtpAnm.play();
            mBtpAnm.setPlaySpeed(rate);

            if (mAnmFlags & ANM_FLAG_800) {
                if (chkEndAnm(mBtpAnm.getFrameCtrl(), rate)) {
                    mBtpTimer = cLib_getRndValue(90, 90);
                    mBtpAnm.setPlaySpeed(0.0f);
                }
            } else {
                mBtpTimer = 0;
            }

            if (cLib_calcTimer(&mBtpTimer) == 0) {
                mBtpAnm.setPlaySpeed(1.0f);
            }
        }
    }

    if (mAnmFlags & ANM_PLAY_MORF) {
        rate = mAnm_p->getPlaySpeed();
        if (mAnmFlags & ANM_PAUSE_MORF) {
            mAnm_p->setPlaySpeed(0.0f);
            mAnm_p->play(mPolySound, mReverb);
            mAnm_p->setPlaySpeed(rate);
        } else {
            mAnm_p->play(mPolySound, mReverb);
            if (chkEndAnm(rate)) {
                mMotionLoops++;
            }
        }
    }

    if (mAnmFlags & ANM_PLAY_BTK) {
        rate = mBtkAnm.getPlaySpeed();
        if (mAnmFlags & ANM_PAUSE_BTK) {
            mBtkAnm.setPlaySpeed(0.0f);
        }
        if (!ctrlBtk()) {
            mBtkAnm.play();
        }
        mBtkAnm.setPlaySpeed(rate);
    }

    if (mAnmFlags & ANM_PLAY_BRK) {
        rate = mBrkAnm.getPlaySpeed();
        if (mAnmFlags & ANM_PAUSE_BRK) {
            mBrkAnm.setPlaySpeed(0.0f);
        }
        mBrkAnm.play();
        mBrkAnm.setPlaySpeed(rate);
    }

    mAnmFlags &= ~ANM_PAUSE_ALL;
    return true;
}

/**
 * Set the expression animation.
 * @param anm The animation data, a two-dimensional array of data pointers, indexed first by
 *  the expression index and second by the expression phase. Each piece of data consists of
 *  an animation index for that phase of the expression, the number of interpolation frames, and
 *  the number of loops before moving on to the next phase (or 0 for the last phase).
 */
void daNpcF_c::playExpressionAnm(daNpcF_c::daNpcF_anmPlayData*** anm) {
    daNpcF_anmPlayData* playData = NULL;
    f32 morf;

    if (anm[mExpression] != NULL) {
        playData = anm[mExpression][mExpressionPhase];
    }

    if (playData != NULL) {
        if (mExpressionPrevPhase == mExpressionPhase && playData->numLoops > 0 &&
            playData->numLoops <= mExpressionLoops)
        {
            mExpressionPhase++;
            playData = anm[mExpression][mExpressionPhase];
        }

        if (playData != NULL && mExpressionPrevPhase != mExpressionPhase) {
            setExpressionAnm(playData->idx, true);
            morf = playData->morf;
            if (mExpressionPhase == 0 && 0.0f <= mExpressionMorfOverride) {
                morf = mExpressionMorfOverride;
            }
            mExpressionMorf = morf;
            mAnm_p->setMorf(morf);
        }
    }

    mExpressionPrevPhase = mExpressionPhase;
}

/**
 * Set the motion animation.
 * @param anm The animation data, a two-dimensional array of data pointers, indexed first by
 *  the motion index and second by the motion phase. Each piece of data consists of
 *  an animation index for that phase of the motion, the number of interpolation frames, and the
 *  number of loops before moving on to the next phase (or 0 for the last phase).
 */
void daNpcF_c::playMotionAnm(daNpcF_c::daNpcF_anmPlayData*** anm) {
    daNpcF_anmPlayData* playData = NULL;
    f32 morf;

    if (anm[mMotion] != NULL) {
        playData = anm[mMotion][mMotionPhase];
    }

    if (playData != NULL) {
        if (mMotionPrevPhase == mMotionPhase && playData->numLoops > 0 &&
            playData->numLoops <= mMotionLoops)
        {
            mMotionPhase++;
            playData = anm[mMotion][mMotionPhase];
        }
    
        if (playData != NULL && mMotionPrevPhase != mMotionPhase) {
            setMotionAnm(playData->idx, 0.0f);
            morf = playData->morf;
            if (mMotionPhase == 0 && 0.0f <= mMotionMorfOverride) {
                morf = mMotionMorfOverride;
            }
            mExpressionMorf = 0.0f;
            mAnm_p->setMorf(morf);
        }
    }

    mMotionPrevPhase = mMotionPhase;
}

/**
 * Compute the animation matrix for the backbone, neck, or head joint.
 * This matrix is applied to the matrix in `mDoMtx_stack_c::now`.
 * @param i_jointNo The index of the joint in question.
 * @param i_jointList An array of the three indices of the joints for the backbone, neck, and head.
 * @param i_neckHeadAngleRatio The ratio between the neck angle and the head angle.
 */
void daNpcF_c::setLookatMtx(int i_jointNo, int* i_jointList, f32 i_neckHeadAngleRatio) {
    cXyz pos;
    Mtx baseMtx, invParentMtx;

    if (i_jointList[0] == i_jointNo || i_jointList[1] == i_jointNo || i_jointList[2] == i_jointNo) {
        mLookatAngle[1].x = (s16)((f32)mLookatAngle[2].x * i_neckHeadAngleRatio);
        mLookatAngle[1].y = (s16)((f32)mLookatAngle[2].y * i_neckHeadAngleRatio);
        mLookatAngle[1].z = (s16)((f32)mLookatAngle[2].z * i_neckHeadAngleRatio);

        cMtx_copy(mDoMtx_stack_c::get(), baseMtx);
        pos.set(baseMtx[0][3], baseMtx[1][3], baseMtx[2][3]);
        baseMtx[0][3] = baseMtx[1][3] = baseMtx[2][3] = 0.0f;

        mDoMtx_stack_c::ZXYrotS(mCurAngle);
        if (i_jointList[0] != i_jointNo) {
            if (i_jointList[1] == i_jointNo) {
                mDoMtx_stack_c::ZXYrotM(mLookatAngle[0]);
            } else if (i_jointList[2] == i_jointNo) {
                mDoMtx_stack_c::ZXYrotM(mLookatAngle[1]);
            }
        }
        mDoMtx_stack_c::inverse();
        cMtx_copy(mDoMtx_stack_c::get(), invParentMtx);

        mDoMtx_stack_c::transS(pos);
        mDoMtx_stack_c::XYZrotM(mCurAngle);
        if (i_jointList[0] == i_jointNo) {
            mDoMtx_stack_c::ZXYrotM(mLookatAngle[0]);
        } else if (i_jointList[1] == i_jointNo) {
            mDoMtx_stack_c::ZXYrotM(mLookatAngle[1]);
        } else if (i_jointList[2] == i_jointNo) {
            mDoMtx_stack_c::ZXYrotM(mLookatAngle[2]);
        }
        mDoMtx_stack_c::concat(invParentMtx);
        mDoMtx_stack_c::concat(baseMtx);
    }
}

BOOL daNpcF_c::hitChk2(dCcD_Cyl* i_ccCyl, BOOL param_1, BOOL param_2) {
    fopAc_ac_c* hit_actor = NULL;
    s16 actorAngleY;
    static_cast<dCcD_Stts*>(i_ccCyl->GetStts())->Move();

    if (mDamageTimer == 0 && i_ccCyl->ChkTgHit()) {
        hit_actor = i_ccCyl->GetTgHitAc();
        if (hit_actor != NULL) {
            actorAngleY = fopAcM_searchActorAngleY(this, hit_actor);
            field_0x990 = actorAngleY - mCurAngle.y;
            if (fopAcM_GetName(hit_actor) == PROC_NPC_TK) {
                if (param_1) {
                    static_cast<daNPC_TK_c*>(hit_actor)->setBump();
                    return true;
                } else {
                    return false;
                }
            } else {
                if (param_2) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }

    i_ccCyl->ClrTgHit();
    return false;
}

void daNpcF_c::setDamage(int i_timer, int i_expression, int i_motion) {
    if (i_expression >= 0) {
        setExpression(i_expression, 0.0f);
    }
    if (i_motion >= 0) {
        setMotion(i_motion, 0.0f, true);
    }

    field_0x984[0] = cM_deg2s(45.0f);
    field_0x984[2] = cM_deg2s(70.0f);
    field_0x992 = 0x4000;
    mTotalDamageTimer = i_timer;
    mDamageTimer = mTotalDamageTimer;
    speed.setall(0.0f);
    speedF = 0.0f;
    mIsDamaged = true;
}

int daNpcF_c::ctrlMsgAnm(int& o_expression, int& o_motion, fopAc_ac_c* param_2, BOOL param_3) {
    o_expression = -1;
    o_motion = -1;

    if (param_3 || eventInfo.checkCommandTalk() || mStaffID != -1) {
        if (dComIfGp_event_getTalkPartner() == param_2) {
            msg_class* msg_p = dMsgObject_c::getActor();
            JUT_ASSERT(2780, NULL != msg_p);

            if (msg_p->mode == 2 || msg_p->mode == 3) {
                field_0x9a4 = -1;
            } else if (msg_p->mode == dEvtCmd_INCATCH_e) {
                if (msg_p->msg_idx != field_0x9a4) {
                    o_expression = dComIfGp_getMesgFaceAnimeAttrInfo();
                    o_motion = dComIfGp_getMesgAnimeAttrInfo();
                    field_0x9a4 = msg_p->msg_idx;
                }

                if (dMsgObject_isMouthCheck()) {
                    mAnmFlags &= ~ANM_PAUSE_EXPRESSION;
                } else {
                    mAnmFlags |= ANM_PAUSE_EXPRESSION;
                }
                mMsgTimer = 20;
            } else {
                if (mAnmFlags & ANM_PAUSE_EXPRESSION) {
                    mAnmFlags &= ~ANM_PAUSE_EXPRESSION;
                }
                field_0x9a4 = -1;
            }
        } else {
            field_0x9a4 = -1;
            if (mMsgTimer != 0) {
                mMsgTimer = 1;
            }
        }
    }

    if (!param_3) {
        cLib_calcTimer(&mMsgTimer);
    }

    return mMsgTimer;
}

void daNpcF_c::orderEvent(int i_speak, char* i_evtName, u16 param_2, u16 i_priority, u8 i_mapToolID,
                          u16 i_flag) {
    if (i_evtName != NULL) {
        mEventIdx = dComIfGp_getEventManager().getEventIdx(this, i_evtName, 0xff);
        fopAcM_orderOtherEventId(this, mEventIdx, i_mapToolID, param_2, i_priority, i_flag);
    } else if (
#if PLATFORM_SHIELD
        true
#else
        !mTwilight|| daPy_py_c::checkNowWolfEyeUp()
#endif
    ) {
        if ((attention_info.flags & fopAc_AttnFlag_SPEAK_e) || (attention_info.flags & fopAc_AttnFlag_TALK_e)) {
            eventInfo.onCondition(dEvtCnd_CANTALK_e);
            if (i_speak) {
                fopAcM_orderSpeakEvent(this, 0, 0);
            }
        }
    }
}

void daNpcF_c::changeEvent(char* i_arcName, char* i_evtName, u16 param_2, u16 param_3) {
    if (i_arcName != NULL) {
        eventInfo.setArchiveName(i_arcName);
        dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
    }

    JUT_ASSERT(2887, NULL != i_evtName);

    mEventIdx = dComIfGp_getEventManager().getEventIdx(this, i_evtName, 0xFF);
    dComIfGp_getEvent().reset(this);
    fopAcM_orderChangeEventId(this, mEventIdx, param_2, param_3);
}

BOOL daNpcF_c::chkActorInSight(fopAc_ac_c* i_actor, f32 i_fovY) {
    s16 angle = mHeadAngle.y;
    angle = fopAcM_searchActorAngleY(this, i_actor) - angle;
    angle = abs(angle);
    return i_fovY == 180.0f || angle < cM_deg2s(i_fovY);
}

/**
 * Check whether an actor is in an elliptic cylinder region.
 * @param i_actor The actor to check.
 * @param i_center The center of the cylinder.
 * @param i_bounds The dimensions of the bounding box.
 * @param i_angleY The angle to align the axis of the ellipse.
 * @return Whether the actor's position vector lies in the elliptic cylinder.
 */
BOOL daNpcF_c::chkActorInArea(fopAc_ac_c* i_actor, cXyz i_center, cXyz i_bounds, s16 i_angleY) {
    BOOL ret = false;
    cXyz delta, relPos;

    mDoMtx_stack_c::YrotS(-i_angleY);
    mDoMtx_stack_c::transM(-i_center.x, -i_center.y, -i_center.z);
    mDoMtx_stack_c::multVec(&i_actor->current.pos, &relPos);
    f32 boundX = fabsf(i_bounds.x);
    f32 boundZ = fabsf(i_bounds.z);
    f32 posX = fabsf(relPos.x);
    f32 posZ = fabsf(relPos.z);
    delta = i_center - i_actor->current.pos;

    if ((posX * posX) / (boundX * boundX) + (posZ * posZ) / (boundZ * boundZ) <= 1.0f
        && -i_bounds.y < delta.y && delta.y < i_bounds.y)
    {
        ret = true;
    }

    return ret;
}

/**
 * Check whether an actor lies within another actor's region of attention.
 * @param i_actorCheck The actor to check.
 * @param i_actorAttn The actor whose attention region we are using.
 * @param i_distIndex An index into the distance table to determine the attention region.
 * @return Whether `actorCheck` is within the region of attention of `actorAttn`.
 */
BOOL daNpcF_c::chkActorInAttnArea(fopAc_ac_c* i_actorCheck, fopAc_ac_c* i_actorAttn,
                                  int i_distIndex) {
    BOOL ret = false;
    f32 neg_lower_y = dComIfGp_getAttention()->getDistTable(i_distIndex).mLowerY * -1.0f;
    f32 neg_upper_y = dComIfGp_getAttention()->getDistTable(i_distIndex).mUpperY * -1.0f;
    cXyz center = i_actorAttn->current.pos;
    cXyz bounds;

    bounds.x = dComIfGp_getAttention()->getDistTable(i_distIndex).mDistMax;
    bounds.y = (fabsf(neg_lower_y) + fabsf(neg_upper_y)) / 2.0f;
    bounds.z = dComIfGp_getAttention()->getDistTable(i_distIndex).mDistMax;
    center.y = bounds.y + (center.y + neg_upper_y);

    if (chkActorInArea(i_actorCheck, center, bounds, i_actorAttn->shape_angle.y)) {
        ret = true;
    }

    return ret;
}

BOOL daNpcF_c::initTalk(int i_flowID, fopAc_ac_c** param_1) {
    mFlow.init(this, i_flowID, 0, param_1);
    speedF = 0.0f;
    speed.setall(0.0f);
    mOrderSpeakEvt = false;
    field_0x9ec = false;
    return true;
}

BOOL daNpcF_c::talkProc(int* param_0, BOOL param_1, fopAc_ac_c** i_talkPartners_p) {
    BOOL ret = false;
    int i1, motion, expression, i2;
    motion = -1;
    expression = -1;

    if (dMsgObject_isMsgSendControl()) {
        if (param_1) {
            dMsgObject_offMsgSendControl();
        }
    } else if (mFlow.doFlow(this, i_talkPartners_p, 0)) {
        ret = true;
    } else {
        if (mFlow.getMsg() != NULL && (mFlow.getMsg()->mode == 2 || mFlow.getMsg()->mode == 6) && param_0 != NULL) {
            for (int i = 0; param_0[i] > 0; i++) {
                if (param_0[i] == mFlow.getMsgNo()) {
                    ret = true;
                    break;
                }
            }
        }

        if (mFlow.checkEventRender(&i1, &motion, &expression, &i2)) {
            if (expression != -1) {
                setExpression(expression, -1.0f);
            }
            if (motion != -1) {
                setMotion(motion, -1.0f, false);
            }
        }
    }

    return ret;
}

BOOL daNpcF_c::turn(s16 i_targetAngle, f32 i_rate, int i_direction) {
    int target_angle = i_targetAngle;
    if (mTurnAmount == 0) {
        mTurnStepNum = fabsf(cM_sht2d((s16)(current.angle.y - i_targetAngle)));
        mTurnStepNum = (mTurnStepNum / 180.0f) * i_rate;
        mTurnStepNum = (int)mTurnStepNum + 1;
        if (mTurnStepNum < 8.0f) {
            mTurnStepNum = 8.0f;
        }
        mTurnStartAngle = current.angle.y;
    }

    s16 turn_start_angle = mTurnStartAngle;
    if (i_direction == 0) {
        i_direction = (s16)(i_targetAngle - mTurnStartAngle) >= 0 ? 1 : -1;
    }

    u32 turn_angle = (u16)(target_angle - turn_start_angle);
    if (i_direction < 0) {
        turn_angle = -(u16)(0xffff - turn_angle);
    }

    turn_angle = (int)((int)turn_angle * cM_ssin(mTurnAmount));
    mTurnAmount = (int)((f32)mTurnAmount + (f32)0x4000 / mTurnStepNum);
    if ((u16)mTurnAmount >= 0x4000) {
        current.angle.y = i_targetAngle;
        mTurnAmount = 0x4000;
    } else {
        current.angle.y = mTurnStartAngle + turn_angle;
    }

    return current.angle.y - (s16)i_targetAngle == 0;
}

BOOL daNpcF_c::step(s16 i_targetAngle, int i_expression, int i_motion, int i_rate) {
    if (mTurnMode == 0) {
        s16 angleDiff = i_targetAngle - mCurAngle.y;
        if ((int)fabsf(cM_sht2d(angleDiff)) > 40) {
            if (i_expression >= 0) {
                setExpression(i_expression, -1.0f);
            }
            if (i_motion >= 0) {
                setMotion(i_motion, -1.0f, false);
            }
        }

        mTurnTargetAngle = i_targetAngle;
        mTurnAmount = 0;
        if (mCurAngle.y == mTurnTargetAngle) {
            mTurnMode++;
        }

        current.angle.y = mCurAngle.y;
        shape_angle.y = current.angle.y;
        mTurnMode++;
    } else if (mTurnMode == 1) {
        if (turn(mTurnTargetAngle, i_rate, 0)) {
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

void daNpcF_c::setAngle(s16 i_angle) {
    current.angle.y = i_angle;
    shape_angle.y = current.angle.y;
    mCurAngle.y = current.angle.y;
    mOldAngle.y = mCurAngle.y;
    old.angle.y = current.angle.y;
}

u8 daNpcF_c::getDistTableIdx(int param_0, int param_1) {
    return daNpcF_getDistTableIdx(param_0, param_1);
}

fopAc_ac_c* daNpcF_c::getEvtAreaTagP(int i_type, int i_no) {
    int r29 = 0;
    f32 f31 = G_CM3D_F_INF;
    mFindCount = 0;
    mSrchActorName = PROC_TAG_EVTAREA;
    fpcM_Search(this->srchActor, this);

    for (int i = 0; i < mFindCount; i++) {
        if (i_type == static_cast<daTag_EvtArea_c*>(mFindActorPList[i])->getType() && i_no == static_cast<daTag_EvtArea_c*>(mFindActorPList[i])->getNo()) {
            return static_cast<daTag_EvtArea_c*>(mFindActorPList[i]);
        }
    }

    return NULL;
}

fopAc_ac_c* daNpcF_c::getAttnActorP(BOOL i_playerAttn, fpcLyIt_JudgeFunc i_searchFunc, f32 i_radius,
                                    f32 i_offsetY1, f32 i_offsetY2, f32 i_fovY, s16 i_sightAngle,
                                    int param_7, BOOL param_8) {
    fopAc_ac_c* actor = NULL;
    int sp40 = 450 + (int)((cM_rnd() - 0.5f) * 450.0f);
    int sp3C = param_7 + (int)((cM_rnd() - 0.5f) * param_7);
    int sp38 = 0;
    int sp34;

    f32 minDistance = G_CM3D_F_INF;
    f32 var_f30 = i_radius;

    if (mAttnChangeTimer != 0) {
        fopAcM_SearchByID(daPy_getPlayerActorClass()->getGrabActorID(), &actor);
        if (mAttnActor[mAttnIdx].getActorP() != actor) {
            cLib_calcTimer(&mAttnChangeTimer);
        }

        actor = mAttnActor[mAttnIdx].getActorP();
    }

    if (actor == NULL) {
        mAttnChangeTimer = 0;

        if (i_playerAttn) {
            var_f30 = fopAcM_searchPlayerDistanceXZ(this);
        }

        mFindCount = 0;
        fpcM_Search(i_searchFunc, this);

        int i, j;
        BOOL sp30;
        int unusedInt1;
        for (i = 0; i < mFindCount; i++) {
            sp30 = chkPointInArea(getAttentionPos(mFindActorPList[i]), attention_info.position,
                                       var_f30, i_offsetY1, i_offsetY2, 0);
            if (sp30 && chkActorInSight2(mFindActorPList[i], i_fovY, i_sightAngle)) {
                for (j = 0; j < 5; j++) {
                    // !@Bug I think mAttnActorTimer index should be j
                    if (mFindActorPList[i] == mAttnActor[j].getActorP() && mAttnActorTimer[i] != 0)
                    {
                        break;
                    }
                }

                if (j == 5) {
                    f32 dist2 = fopAcM_searchActorDistanceXZ(this, mFindActorPList[i]);
                    if (dist2 < minDistance) {
                        actor = mFindActorPList[i];
                        minDistance = dist2;
                    }
                }
            }
        }

        if (actor == NULL && i_playerAttn) {
            fopAcM_SearchByID(daPy_getPlayerActorClass()->getGrabActorID(), &actor);
            sp38 = 1;
        }

        if (actor != NULL) {
            sp34 = 900;
            i = 0;
            j = unusedInt1 = 0;

            for (; j < 5; j++) {
                if (mAttnActorTimer[j] < sp34) {
                    sp34 = mAttnActorTimer[j];
                    i = j;
                }
            }

            mAttnActor[i].entry(actor);

            if (param_8) {
                mAttnActorTimer[i] = sp38 == 1 ? 0 : sp40;
            } else {
                mAttnActorTimer[i] = 0;
            }

            mAttnChangeTimer = sp38 == 1 ? 30 : sp3C;
            mAttnIdx = i;
        }
    }

    if (actor == NULL) {
        mAttnChangeTimer = 0;
    }

    return actor;
}

BOOL daNpcF_c::chkActorInSight2(fopAc_ac_c* i_actor, f32 i_fovY, s16 i_sightAngle) {
    if (i_fovY < 180.0f) {
        s16 fov_y = cM_deg2s(i_fovY);
        cXyz actor_attn_pos = getAttentionPos(i_actor);
        cSGlobe globe(attention_info.position - actor_attn_pos);
        cSAngle s_ang_diff = cSAngle(globe.U().Inv()) - i_sightAngle;
        s16 ang_diff = s_ang_diff.Val();
        if (ang_diff < 0) {
            ang_diff = -ang_diff;
        }
        return (ang_diff > fov_y) == FALSE;
    }

    return true;
}

BOOL daNpcF_c::chkPointInArea(cXyz i_point, cXyz param_1, f32 i_radius, f32 i_offsetY1,
                              f32 i_offsetY2, s16 i_angleY) {
    cXyz bounds, center;

    f32 upper = i_offsetY1;
    f32 lower = i_offsetY2;

    if (upper < lower) {
        lower = i_offsetY1;
        upper = i_offsetY2;
    }

    bounds.set(i_radius, fabsf(upper - lower) / 2.0f, i_radius);

    center = param_1;
    center.y += lower;
    center.y += bounds.y;

    return chkPointInArea(i_point, center, bounds, i_angleY);
}

BOOL daNpcF_c::chkPointInArea(cXyz i_point, cXyz i_center, cXyz i_bounds, s16 i_angleY) {
    return daNpcF_chkPointInArea(i_point, i_center, i_bounds, i_angleY);
}

cXyz daNpcF_c::getAttentionPos(fopAc_ac_c* i_actor_p) {
    cXyz pos = i_actor_p->attention_info.position;

    if (fopAcM_GetName(i_actor_p) == PROC_ALINK) {
        pos.y -= daPy_py_c::getAttentionOffsetY();
    }

    return pos;
}

BOOL daNpcF_c::chkFindPlayer2(BOOL i_hasAttn, s16 i_angle) {
    BOOL sp20;

    cXyz attn_pos = getAttentionPos(daPy_getPlayerActorClass());
    int attn_no = i_hasAttn == false ? attention_info.distances[fopAc_attn_SPEAK_e]
                                     : attention_info.distances[fopAc_attn_TALK_e];
    f32 dist_max = i_hasAttn == false ? dComIfGp_getAttention()->getDistTable(attn_no).mDistMax
                                     : dComIfGp_getAttention()->getDistTable(attn_no).mDistMaxRelease;
    f32 lower_y = dComIfGp_getAttention()->getDistTable(attn_no).mLowerY * -1.0f;
    f32 upper_y = dComIfGp_getAttention()->getDistTable(attn_no).mUpperY * -1.0f;
    u32 angle_select = dComIfGp_getAttention()->getDistTable(attn_no).mAngleSelect;

    f32 fovy = 180.0f;
    if (angle_select & 8) {
        fovy = 30.0f;
    } else if (angle_select & 0x10) {
        fovy = 45.0f;
    } else if (angle_select & 0x20) {
        fovy = 60.0f;
    } else if (angle_select & 0x40) {
        fovy = 90.0f;
    } else if (angle_select & 0x80) {
        fovy = 110.0f;
    } else if (angle_select & 0x100) {
        fovy = 135.0f;
    }

    if (chkPointInArea(attn_pos, attention_info.position, dist_max, lower_y, upper_y, 0)) {
        if (chkActorInSight2(daPy_getPlayerActorClass(), fovy, i_angle)) {
            return true;
        }
    }

    return false;
}

void daNpcF_c::setHitodamaPrtcl() {
    JPABaseEmitter* emitter = NULL;
    cXyz pos;

    field_0x9d0 = (u16)(field_0x9d2 * 2);
    field_0x9b8.x = cM_ssin(field_0x9d2) * 8.0f;
    field_0x9b8.y = cM_ssin(field_0x9d0) * 4.0f;
    field_0x9b8.z = field_0x9b8.x * -cM_ssin(shape_angle.y);

    field_0x9b8.x *= cM_scos(shape_angle.y);
    field_0x9d2 += (s16)0x400;

    pos.x = eyePos.x + field_0x9b8.x + field_0x9c4.x;
    pos.y = eyePos.y + field_0x9b8.y + field_0x9c4.y;
    pos.z = eyePos.z + field_0x9b8.z + field_0x9c4.z;

    for (int i = 0; i < 2; i++) {
        static const u16 id[2] = {0x8497, 0x8498};
        mHitodamaParticleKey[i] = dComIfGp_particle_set(mHitodamaParticleKey[i], id[i], &pos, &mCurAngle, NULL);

        emitter = dComIfGp_particle_getEmitter(mHitodamaParticleKey[i]);
        if (emitter != NULL) {
            u8 alpha = dComIfGs_wolfeye_effect_check() == 0 ? 0xFF : 0;
            emitter->setGlobalTranslation(pos.x, pos.y, pos.z);
            emitter->setGlobalAlpha(alpha);
        }
    }
}

BOOL daNpcF_pntVsLineSegmentLengthSquare2D(f32 i_pntX, f32 i_pntZ, f32 i_startX, f32 i_startZ,
                                           f32 i_endX, f32 i_endZ, f32* o_projX, f32* o_projZ,
                                           f32* o_dist2) {
    BOOL ret = false;
    f32 prm;
    f32 len2;
    f32 line_x = i_endX - i_startX;
    f32 line_z = i_endZ - i_startZ;
    len2 = line_x * line_x + line_z * line_z;

    if (0.0f == len2) {
        *o_dist2 = 0.0f;
        return false;
    }

    prm = (line_x * (i_pntX - i_startX) + line_z * (i_pntZ - i_startZ)) / len2;

    if (0.0f <= prm && prm <= 1.0f) {
        ret = true;
    }

    *o_projX = i_startX + line_x * prm;
    *o_projZ = i_startZ + line_z * prm;
    //!@bug The last `o_projX` should be `o_projZ`
    *o_dist2 = (*o_projX - i_pntX) * (*o_projX - i_pntX)
             + (*o_projZ - i_pntZ) * (*o_projX - i_pntZ);
    return ret;
}

static u16 daNpcF_putNurbs(dPnt* param_0, int param_1, int param_2, dPnt* param_3, int param_4) {
    return 0;
}

BOOL daNpcF_chkPassed(cXyz i_pos, dPnt* i_points, u16 i_idx, u16 i_num, BOOL i_isClosed,
                      BOOL i_isReversed) {
    cXyz prev_pos, cur_pos, next_pos, pos;
    u16 prev_idx = i_idx;
    u16 next_idx = i_idx;
    daNpcF_incIdx(i_num, next_idx, i_isClosed, false);
    daNpcF_decIdx(i_num, prev_idx, i_isClosed, false);

    prev_pos.set(i_points[prev_idx].m_position.x,
                 i_points[prev_idx].m_position.y,
                 i_points[prev_idx].m_position.z);
    cur_pos.set(i_points[i_idx].m_position.x,
                i_points[i_idx].m_position.y,
                i_points[i_idx].m_position.z);
    next_pos.set(i_points[next_idx].m_position.x,
                 i_points[next_idx].m_position.y,
                 i_points[next_idx].m_position.z);
    
    f32 dist;
    s16 angle;
    if (prev_idx == i_idx && next_idx == i_idx) {
        JUT_ASSERT(0xff9, FALSE);
    } else {
        if (prev_idx < i_idx && i_idx < next_idx) {
            dist = (next_pos - prev_pos).absXZ();
            angle = cM_atan2s(next_pos.x - prev_pos.x, next_pos.z - prev_pos.z);
            pos = prev_pos;
            prev_pos.x = pos.x + dist * -1.0f * cM_ssin(angle);
            prev_pos.z = pos.z + dist * -1.0f * cM_scos(angle);
            next_pos.x = pos.x + dist * 2.0f * cM_ssin(angle);
            next_pos.z = pos.z + dist * 2.0f * cM_scos(angle);
        } else if (prev_idx < i_idx) {
            dist = (cur_pos - prev_pos).absXZ();
            angle = cM_atan2s(cur_pos.x - prev_pos.x, cur_pos.z - prev_pos.z);
            pos = cur_pos;
            prev_pos.x = pos.x + dist * -2.0f * cM_ssin(angle);
            prev_pos.z = pos.z + dist * -2.0f * cM_scos(angle);
            next_pos.x = pos.x + dist * 2.0f * cM_ssin(angle);
            next_pos.z = pos.z + dist * 2.0f * cM_scos(angle);
        } else if (i_idx < next_idx) {
            dist = (next_pos - cur_pos).absXZ();
            angle = cM_atan2s(next_pos.x - cur_pos.x, next_pos.z - cur_pos.z);
            pos = cur_pos;
            prev_pos.x = pos.x + dist * -2.0f * cM_ssin(angle);
            prev_pos.z = pos.z + dist * -2.0f * cM_scos(angle);
            next_pos.x = pos.x + dist * 2.0f * cM_ssin(angle);
            next_pos.z = pos.z + dist * 2.0f * cM_scos(angle);
        } else {
            JUT_ASSERT(0x1029, FALSE);
        }

        f32 proj_x, proj_z, proj2_x, proj2_z;
        daNpcF_pntVsLineSegmentLengthSquare2D(i_pos.x, i_pos.z, prev_pos.x, prev_pos.z,
                                              next_pos.x, next_pos.z, &proj_x, &proj_z, &dist);
        if (cM3d_IsZero(dist)) {
            return false;
        } else {
            daNpcF_pntVsLineSegmentLengthSquare2D(cur_pos.x, cur_pos.z, prev_pos.x, prev_pos.z,
                                                next_pos.x, next_pos.z, &proj2_x, &proj2_z, &dist);
            if (i_isReversed) {
                angle = cM_atan2s(prev_pos.x - next_pos.x, prev_pos.z - next_pos.z);
            } else {
                angle = cM_atan2s(next_pos.x - prev_pos.x, next_pos.z - prev_pos.z);
            }
            angle = angle - cM_atan2s(proj2_x - proj_x, proj2_z - proj_z);
            return (u16)abs(angle) > 0x4000;
        }
    }

    return false;
}

s16 daNpcF_getGroundAngle(cBgS_PolyInfo* param_0, s16 param_1) {
    cM3dGPla plane;

    if (dComIfG_Bgsp().ChkPolySafe(*param_0)) {
        BOOL triPla = dComIfG_Bgsp().GetTriPla(*param_0, &plane);
        if (!triPla || !cBgW_CheckBGround(plane.mNormal.y)) {
            return 0;
        } else {
            return fopAcM_getPolygonAngle(&plane, param_1);
        }
    }

    return 0;
}

BOOL daNpcF_chkEvtBit(u32 i_no) {
    return dComIfGs_isEventBit((u16)dSv_event_flag_c::saveBitLabels[i_no]);
}

void daNpcF_onEvtBit(u32 i_no) {
    dComIfGs_onEventBit((u16)dSv_event_flag_c::saveBitLabels[i_no]);
}

BOOL daNpcF_chkTmpBit(u32 i_no) {
    return dComIfGs_isTmpBit((u16)dSv_event_tmp_flag_c::tempBitLabels[i_no]);
}

void daNpcF_onTmpBit(u32 i_no) {
    dComIfGs_onTmpBit((u16)dSv_event_tmp_flag_c::tempBitLabels[i_no]);
}

void daNpcF_offTmpBit(u32 i_no) {
    dComIfGs_offTmpBit((u16)dSv_event_tmp_flag_c::tempBitLabels[i_no]);
}

int daNpcF_getPlayerInfoFromPlayerList(int param_0, int i_roomNo, cXyz& param_2, csXyz& param_3) {
    int rv = 0;

    dStage_roomDt_c* room_p = dComIfGp_roomControl_getStatusRoomDt(i_roomNo);
    stage_actor_data_class* entries = room_p->getPlayer()->m_entries;

    for (int i = 0; i < room_p->getPlayerNum(); i++) {
        if (param_0 == (u8)entries->base.angle.z) {
            param_2 = entries->base.position;
            param_3 = entries->base.angle;
            rv = 1;
            break;
        }
        entries++;
    }

    return rv;
}

BOOL daNpcF_chkDoBtnEqSpeak(fopAc_ac_c* i_actor_p) {
    BOOL ret = FALSE;

    if (daPy_getPlayerActorClass()->checkPriActorOwn(i_actor_p)) {
        JUT_ASSERT(0x10dd, NULL != dComIfGp_getAttention());

        for (int i = 0; i < dComIfGp_getAttention()->GetActionCount(); i++) {
            if (dComIfGp_getAttention()->ActionTarget(i) == i_actor_p &&
                dComIfGp_getAttention()->getActionBtnB() &&
                dComIfGp_getAttention()->getActionBtnB()->mType == fopAc_attn_SPEAK_e)
            {
                ret = TRUE;
            }
        }

        for (int i = 0; i < dComIfGp_getAttention()->GetLockonCount(); i++) {
            if (dComIfGp_getAttention()->LockonTarget(i) == i_actor_p &&
                dComIfGp_getAttention()->getActionBtnB() &&
                (dComIfGp_getAttention()->getActionBtnB()->mType == fopAc_attn_TALK_e
                #if !PLATFORM_GCN
                || dComIfGp_getAttention()->getActionBtnB()->mType == fopAc_attn_UNK_4
                #endif
                ))
            {
                ret = TRUE;
            }
        }
    }

    return ret;
}

BOOL daNpcF_chkPointInArea(cXyz i_point, cXyz i_center, cXyz i_bounds, s16 i_angleY) {
    cXyz delta_aligned;

    mDoMtx_stack_c::YrotS(-i_angleY);
    mDoMtx_stack_c::transM(-i_center.x, -i_center.y, -i_center.z);
    mDoMtx_stack_c::multVec(&i_point, &delta_aligned);

    f32 bound_x = fabsf(i_bounds.x);
    f32 bound_z = fabsf(i_bounds.z);
    f32 delta_x = fabsf(delta_aligned.x);
    f32 delta_z = fabsf(delta_aligned.z);

    cXyz delta = i_center - i_point;

    f32 tmp_float1;
    f32 tmp_float2;
    f32 tmp_float3;
    if (delta.y <= -i_bounds.y || i_bounds.y <= delta.y) {
        return FALSE;
    } else {
        if ((delta_x * delta_x) / (bound_x * bound_x) +
            (delta_z * delta_z) / (bound_z * bound_z) <= 1.0f) {
            return TRUE;
        }
    }

    return FALSE;
}

u8 daNpcF_getDistTableIdx(int param_0, int param_1) {
    return param_0 + param_1 * 0x14 + 0x5e;
}

#if DEBUG
void daNpcF_commonListenPropertyEvent(char* param_1, int* param_2, daNpcF_HIOParam* param_3) {
    sprintf(param_1 + *param_2, "%.3ff,\t//  注目オフセット\n", param_3->attention_offset);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  重力\n", param_3->gravity);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  スケ−ル\n", param_3->scale);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  リアル影サイズ\n", param_3->real_shadow_size);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  体重\n", param_3->weight);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  高さ\n", param_3->height);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  ひざ丈\n", param_3->knee_length);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  幅\n", param_3->width);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  腰のＸ角上限\n", param_3->body_angleX_max);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  腰のＸ角下限\n", param_3->body_angleX_min);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  腰のＹ角上限\n", param_3->body_angleY_max);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  腰のＹ角下限\n", param_3->body_angleY_min);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  頭のＸ角上限\n", param_3->head_angleX_max);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  頭のＸ角下限\n", param_3->head_angleX_min);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  頭のＹ角上限\n", param_3->head_angleY_max);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  頭のＹ角下限\n", param_3->head_angleY_min);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  首の回転比率\n", param_3->neck_rotation_ratio);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  補間フレ−ム\n", param_3->morf_frame);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%d,   \t//  会話距離\n", param_3->talk_distance);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%d,   \t//  会話角度\n", param_3->talk_angle);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%d,   \t//  注目距離\n", param_3->attention_distance);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%d,   \t//  注目角度\n", param_3->attention_angle);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  視界\n", param_3->fov);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  サ−チ距離\n", param_3->search_distance);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  サ−チ高さ\n", param_3->search_height);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%.3ff,\t//  サ−チ低さ\n", param_3->search_depth);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%d,   \t//  注目時間\n", param_3->attention_time);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%d,   \t//  ダメ－ジ期間\n", param_3->damage_time);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%d,   \t//  表情\n", param_3->face_expression);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%d,   \t//  動作\n", param_3->motion);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%d,   \t//  注視モ−ド\n", param_3->look_mode);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%d,   \t//  デバグモ−ドＯＮ\n", param_3->debug_mode_ON);
    *param_2 = strlen(param_1);
    sprintf(param_1 + *param_2, "%d,   \t//  デバグ情報ＯＮ\n", param_3->debug_info_ON);
    *param_2 = strlen(param_1);
    return;
}

void daNpcF_commonGenMessage(JORMContext* i_ctx, daNpcF_HIOParam* i_hio) {
    i_ctx->genSlider("注目オフセット  ", &i_hio->attention_offset, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("重力            ", &i_hio->gravity, -100.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("スケ−ル        ", &i_hio->scale, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("リアル影サイズ  ", &i_hio->real_shadow_size, 0.0f, 10000.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("体重            ", &i_hio->weight, 0.0f, 255.0f, 0, NULL, 0xffff, 0xffff,0x200, 0x18);
    i_ctx->genSlider("高さ            ", &i_hio->height, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,0x200, 0x18);
    i_ctx->genSlider("ひざ丈          ", &i_hio->knee_length, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,0x200, 0x18);
    i_ctx->genSlider("幅              ", &i_hio->width, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,0x200, 0x18);
    i_ctx->genSlider("腰のＸ角上限    ", &i_hio->body_angleX_max, -90.0f, 90.0f, 0, NULL, 0xffff,0xffff, 0x200, 0x18);
    i_ctx->genSlider("腰のＸ角下限    ", &i_hio->body_angleX_min, -90.0f, 90.0f, 0, NULL, 0xffff,0xffff, 0x200, 0x18);
    i_ctx->genSlider("腰のＹ角上限    ", &i_hio->body_angleY_max, -180.0, 179.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("腰のＹ角下限    ", &i_hio->body_angleY_min, -180.0, 179.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("頭のＸ角上限    ", &i_hio->head_angleX_max, -90.0f, 90.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("頭のＸ角下限    ", &i_hio->head_angleX_min, -90.0f, 90.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("頭のＹ角上限    ", &i_hio->head_angleY_max, -180.0f, 179.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("頭のＹ角下限    ", &i_hio->head_angleY_min, -180.0f, 179.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("首の回転比率    ", &i_hio->neck_rotation_ratio, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("補間フレ－ム    ", &i_hio->morf_frame, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->startComboBox("会話距離        ", &i_hio->talk_distance, 0, NULL, 0xffff, 0xffff, 0x100, 0x1a);
    i_ctx->genComboBoxItem("  50", 0);
    i_ctx->genComboBoxItem(" 100", 1);
    i_ctx->genComboBoxItem(" 150", 2);
    i_ctx->genComboBoxItem(" 200", 3);
    i_ctx->genComboBoxItem(" 250", 4);
    i_ctx->genComboBoxItem(" 300", 5);
    i_ctx->genComboBoxItem(" 350", 6);
    i_ctx->genComboBoxItem(" 400", 7);
    i_ctx->genComboBoxItem(" 450", 8);
    i_ctx->genComboBoxItem(" 500", 9);
    i_ctx->genComboBoxItem(" 550", 10);
    i_ctx->genComboBoxItem(" 600", 11);
    i_ctx->genComboBoxItem(" 650", 12);
    i_ctx->genComboBoxItem(" 700", 13);
    i_ctx->genComboBoxItem(" 750", 14);
    i_ctx->genComboBoxItem(" 800", 15);
    i_ctx->genComboBoxItem(" 850", 16);
    i_ctx->genComboBoxItem(" 900", 17);
    i_ctx->genComboBoxItem(" 950", 18);
    i_ctx->genComboBoxItem("1000", 19);
    i_ctx->endComboBox();
    i_ctx->startComboBox("会話角度        ", &i_hio->talk_angle, 0, NULL, 0xffff, 0xffff, 0x100, 0x1a);
    i_ctx->genComboBoxItem("  30", 0);
    i_ctx->genComboBoxItem("  45", 1);
    i_ctx->genComboBoxItem("  60", 2);
    i_ctx->genComboBoxItem("  90", 3);
    i_ctx->genComboBoxItem(" 110", 4);
    i_ctx->genComboBoxItem(" 135", 5);
    i_ctx->genComboBoxItem(" 180", 6);
    i_ctx->endComboBox();
    i_ctx->startComboBox("注目距離        ", &i_hio->attention_distance, 0, NULL, 0xffff, 0xffff, 0x100, 0x1a);
    i_ctx->genComboBoxItem("  50", 0);
    i_ctx->genComboBoxItem(" 100", 1);
    i_ctx->genComboBoxItem(" 150", 2);
    i_ctx->genComboBoxItem(" 200", 3);
    i_ctx->genComboBoxItem(" 250", 4);
    i_ctx->genComboBoxItem(" 300", 5);
    i_ctx->genComboBoxItem(" 350", 6);
    i_ctx->genComboBoxItem(" 400", 7);
    i_ctx->genComboBoxItem(" 450", 8);
    i_ctx->genComboBoxItem(" 500", 9);
    i_ctx->genComboBoxItem(" 550", 10);
    i_ctx->genComboBoxItem(" 600", 11);
    i_ctx->genComboBoxItem(" 650", 12);
    i_ctx->genComboBoxItem(" 700", 13);
    i_ctx->genComboBoxItem(" 750", 14);
    i_ctx->genComboBoxItem(" 800", 15);
    i_ctx->genComboBoxItem(" 850", 16);
    i_ctx->genComboBoxItem(" 900", 17);
    i_ctx->genComboBoxItem(" 950", 18);
    i_ctx->genComboBoxItem("1000", 19);
    i_ctx->endComboBox();
    i_ctx->startComboBox("注目角度        ", &i_hio->attention_angle, 0, NULL, 0xffff, 0xffff, 0x100, 0x1a);
    i_ctx->genComboBoxItem("  30", 0);
    i_ctx->genComboBoxItem("  45", 1);
    i_ctx->genComboBoxItem("  60", 2);
    i_ctx->genComboBoxItem("  90", 3);
    i_ctx->genComboBoxItem(" 110", 4);
    i_ctx->genComboBoxItem(" 135", 5);
    i_ctx->genComboBoxItem(" 180", 6);
    i_ctx->endComboBox();
    i_ctx->genSlider("視界            ", &i_hio->fov, 0.0f, 180.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("サ－チ距離      ", &i_hio->search_distance, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("サ－チ高さ      ", &i_hio->search_height, -10000.0,10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("サ－チ低さ      ", &i_hio->search_depth, -10000.0,10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("注目時間        ", &i_hio->attention_time, 0, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("ダメ－ジ期間    ", &i_hio->damage_time, 0,10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("表情            ", &i_hio->face_expression, 0,10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("動作            ", &i_hio->motion, 0, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genSlider("注視モ－ド      ", &i_hio->look_mode, 0, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genCheckBox("デバグモ－ドＯＮ", &i_hio->debug_mode_ON, 1, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    i_ctx->genCheckBox("デバグ情報ＯＮ  ", &i_hio->debug_info_ON, 1, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}
#endif

void daNpcF_clearMessageTmpBit() {
    daNpcF_offTmpBit(0xb);
    daNpcF_offTmpBit(0xc);
    daNpcF_offTmpBit(0xd);
    daNpcF_offTmpBit(0xe);
    daNpcF_offTmpBit(0xf);
    daNpcF_offTmpBit(0x33);
    daNpcF_offTmpBit(0x34);
    daNpcF_offTmpBit(0x35);
    daNpcF_offTmpBit(0x36);
    daNpcF_offTmpBit(0x37);
}

// TODO: dummy to generate weak functions, proper fix later
static void dummyVirtual(daNpcF_MoveBgActor_c* dummy) {
    dummy->~daNpcF_MoveBgActor_c();
    dummy->CreateHeap();
    dummy->Create();
    dummy->Execute(NULL);
    dummy->Draw();
    dummy->Delete();
    dummy->IsDelete();
    dummy->ToFore();
    dummy->ToBack();
}

dCcD_SrcGObjInf const daBaseNpc_c::mCcDObj = {
    {0, {{0, 0, 0}, {0x0, 0x0}, {0x79}}},
    {dCcD_SE_NONE, 0, 0, 0, 0},
    {dCcD_SE_NONE, 0, 0, 0, 0},
    {0}
};

dCcD_SrcGObjInf const daNpcF_c::mCcDObjInfo = {
    {0, {{0, 0, 0}, {0x0, 0x0}, {0x79}}},
    {dCcD_SE_NONE, 0, 0, 0, 0},
    {dCcD_SE_NONE, 0, 0, 0, 0},
    {0}
};

static void dummyStrings() {
    DEAD_STRING(".bck");
    DEAD_STRING(".btp");
    DEAD_STRING(".btk");
    DEAD_STRING("%.3ff,\t//  注目オフセット\n");
    DEAD_STRING("%.3ff,\t//  重力\n");
    DEAD_STRING("%.3ff,\t//  スケ−ル\n");
    DEAD_STRING("%.3ff,\t//  リアル影サイズ\n");
    DEAD_STRING("%.3ff,\t//  体重\n");
    DEAD_STRING("%.3ff,\t//  高さ\n");
    DEAD_STRING("%.3ff,\t//  ひざ丈\n");
    DEAD_STRING("%.3ff,\t//  幅\n");
    DEAD_STRING("%.3ff,\t//  腰のＸ角上限\n");
    DEAD_STRING("%.3ff,\t//  腰のＸ角下限\n");
    DEAD_STRING("%.3ff,\t//  腰のＹ角上限\n");
    DEAD_STRING("%.3ff,\t//  腰のＹ角下限\n");
    DEAD_STRING("%.3ff,\t//  頭のＸ角上限\n");
    DEAD_STRING("%.3ff,\t//  頭のＸ角下限\n");
    DEAD_STRING("%.3ff,\t//  頭のＹ角上限\n");
    DEAD_STRING("%.3ff,\t//  頭のＹ角下限\n");
    DEAD_STRING("%.3ff,\t//  首の回転比率\n");
    DEAD_STRING("%.3ff,\t//  補間フレ−ム\n");
    DEAD_STRING("%d,   \t//  会話距離\n");
    DEAD_STRING("%d,   \t//  会話角度\n");
    DEAD_STRING("%d,   \t//  注目距離\n");
    DEAD_STRING("%d,   \t//  注目角度\n");
    DEAD_STRING("%.3ff,\t//  視界\n");
    DEAD_STRING("%.3ff,\t//  サ−チ距離\n");
    DEAD_STRING("%.3ff,\t//  サ−チ高さ\n");
    DEAD_STRING("%.3ff,\t//  サ−チ低さ\n");
    DEAD_STRING("%d,   \t//  注目時間\n");
    DEAD_STRING("%d,   \t//  ダメ−ジ期間\n");
    DEAD_STRING("%d,   \t//  表情\n");
    DEAD_STRING("%d,   \t//  動作\n");
    DEAD_STRING("%d,   \t//  注視モ−ド\n");
    DEAD_STRING("%d,   \t//  デバグモ−ドＯＮ\n");
    DEAD_STRING("%d,   \t//  デバグ情報ＯＮ\n");
}
