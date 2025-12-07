#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc2.h"
#include "d/d_bg_w.h"
#include "d/d_debug_viewer.h"

static s32 daBaseNpc_chkPnt(cXyz param_0, dPnt* param_1, u16 param_2, u16 param_3, int param_4, int param_5);
static u16 daBaseNpc_putNurbs(dPnt* i_CPnts, int i_CPntNum, int i_maxPntNum, dPnt* o_curve_p, BOOL i_isClosed);
static s16 daBaseNpc_getGroundAngle(cBgS_PolyInfo* param_0, s16 param_1);

dCcD_SrcCyl daBaseNpc_c::mCcDCyl = {
    daBaseNpc_c::mCcDObj,
    {
            {0.0f, 0.0f, 0.0f},  // mCenter
            0.0f,  // mRadius
            0.0f  // mHeight
        }  // mCyl
};

dCcD_SrcSph daBaseNpc_c::mCcDSph = {
    daBaseNpc_c::mCcDObj,
    {
            {{0.0f, 0.0f, 0.0f}, 0.0f}  // mSph
    }  // mSphAttr
};

static int daBaseNpc_addIdx(int i_step, int i_num, u16* i_idx, BOOL i_closed) {
    BOOL new_idx = *i_idx;
    BOOL ret;

    ret = FALSE;
    new_idx += i_step;

    if (i_num <= new_idx) {
        if (i_closed) {
            new_idx -= i_num * (new_idx / i_num);
        } else {
            if (i_num != 0) {
                new_idx = i_num - 1;
            } else {
                new_idx = 0;
            }
            ret = TRUE;
        }
    }

    *i_idx = (u16)new_idx;
    return ret;
}

static BOOL daBaseNpc_subIdx(int i_step, int i_num, u16* i_idx, BOOL i_closed) {
    int new_idx = *i_idx;
    BOOL ret;

    ret = FALSE;
    new_idx -= i_step;

    if (new_idx <= 0) {
        if (i_closed) {
            new_idx += i_num * (new_idx / i_num + 1);
        } else {
            new_idx = 0;
            ret = TRUE;
        }
    }

    *i_idx = (u16)new_idx;
    return ret;
}

static BOOL daBaseNpc_incIdx(int i_pathNo, u16* i_pathIdx, BOOL i_pathClosed, int i_pathDirection) {
    BOOL result = FALSE;
    if (i_pathDirection < 0) {
        result = daBaseNpc_subIdx(1, i_pathNo, i_pathIdx, i_pathClosed);
    } else {
        result = daBaseNpc_addIdx(1, i_pathNo, i_pathIdx, i_pathClosed);
    }
    return result;
}

static BOOL daBaseNpc_decIdx(int i_pathNo, u16* i_pathIdx, BOOL i_pathClosed, int i_pathDirection) {
    BOOL result = FALSE;
    if (i_pathDirection < 0) {
        result = daBaseNpc_addIdx(1, i_pathNo, i_pathIdx, i_pathClosed);
    } else {
        result = daBaseNpc_subIdx(1, i_pathNo, i_pathIdx, i_pathClosed);
    }
    return result;
}

static f32 daNpcBase_BBasis(int i_deg, int i_pntNo, int i_knotNo, int* i_knotVector_p) {
    f32 basis;
    f32 var_f31;
    f32 var_f30;

    if (i_deg != 0) {
        var_f31 = (f32)(i_knotVector_p[i_pntNo + i_deg + 1] - i_knotVector_p[i_pntNo + 1]);
        if (var_f31 != 0.0f) {
            var_f31 = ((f32)(i_knotVector_p[i_pntNo + i_deg + 1] - i_knotNo) / var_f31) * daNpcBase_BBasis(i_deg - 1, i_pntNo + 1, i_knotNo, i_knotVector_p);
        }

        var_f30 = (f32)(i_knotVector_p[i_pntNo + i_deg] - i_knotVector_p[i_pntNo]);
        if (var_f30 != 0.0f) {
            var_f30 = ((f32)(i_knotNo - i_knotVector_p[i_pntNo]) / var_f30) * daNpcBase_BBasis(i_deg - 1, i_pntNo, i_knotNo, i_knotVector_p);
        }

        basis = var_f31 + var_f30;
    } else if (i_knotVector_p[i_pntNo] <= i_knotNo && i_knotNo <= i_knotVector_p[i_pntNo + 1]) {
        basis = 1.0f;
    } else {
        basis = 0.0f;
    }

    return basis;
}

daBaseNpc_acMngr_c::daBaseNpc_acMngr_c() {
    mActorId = fpcM_ERROR_PROCESS_ID_e;
}

void daBaseNpc_acMngr_c::entry(fopAc_ac_c* i_ActorP) {
    mActorId = fopAcM_GetID(i_ActorP);
}

fopAc_ac_c* daBaseNpc_acMngr_c::getActor() {
    fopAc_ac_c* ret = NULL;
    BOOL foundActor = fopAcM_SearchByID(mActorId, &ret);
    if (foundActor && ret != NULL) {
        return ret;
    }

    return NULL;
}

void daBaseNpc_matAnm_c::calc(J3DMaterial* i_material) const {
    J3DMaterialAnm::calc(i_material);

    for (u32 i = 0; i < 8; i++) {
        if (getTexMtxAnm(i).getAnmFlag()) {
            J3DTextureSRTInfo* curr_mtx_info =
                &i_material->getTexGenBlock()->getTexMtx(i)->getTexMtxInfo().mSRT;

            if (mMorfFrm != 0) {
                f32 var_f31 = 1.0f / (mMorfFrm + 1);
                curr_mtx_info->mTranslationX = field_0xF4 * (1.0f - var_f31) + curr_mtx_info->mTranslationX * var_f31;
                curr_mtx_info->mTranslationY = field_0xF8 * (1.0f - var_f31) + curr_mtx_info->mTranslationY * var_f31;
            } else if (mEyeMoveFlg != 0) {
                curr_mtx_info->mTranslationX = mNowOffsetX;
                curr_mtx_info->mTranslationY = mNowOffsetY;
            }

            field_0xF4 = curr_mtx_info->mTranslationX;
            field_0xF8 = curr_mtx_info->mTranslationY;
        }
    }
}

daBaseNpc_path_c::daBaseNpc_path_c() {}

BOOL daBaseNpc_path_c::isPath() {
    return NULL != mPathInfo;
}

int daBaseNpc_path_c::setPathInfo(u8 i_pathIdx, s8 i_roomNo, s8 i_direction) {
    mPathInfo = NULL;
    mIdx = 0;
    mDirection = i_direction;

    if (i_pathIdx != 0xFF) {
        mPathInfo = dPath_GetRoomPath(i_pathIdx, i_roomNo);
        if (mPathInfo == NULL) {
            return 0;
        }

        mCurvePntNum = daBaseNpc_putNurbs(mPathInfo->m_points, mPathInfo->m_num, 160,
                                          mCurvePnts, dPath_ChkClose(mPathInfo));
    }

    return isPath();
}

void daBaseNpc_path_c::reverseDir() {
    s8 direction;
    if (mDirection > 0) {
        direction = -1;
    } else {
        direction = 1;
    }

    mDirection = (int)direction;
}

s32 daBaseNpc_path_c::chkPnt(cXyz i_pos) {
    return daBaseNpc_chkPnt(i_pos, mPathInfo->m_points, mIdx, mPathInfo->m_num,
                            dPath_ChkClose(mPathInfo), mDirection);
}

BOOL daBaseNpc_path_c::setNextPnt() {
    return daBaseNpc_incIdx(mPathInfo->m_num, &mIdx, dPath_ChkClose(mPathInfo), mDirection);
}

u16 daBaseNpc_path_c::getIdx() {
    return mIdx;
}

void daBaseNpc_path_c::setIdx(u16 i_idx) {
    mIdx = i_idx;
}

Vec daBaseNpc_path_c::getPntPos(u16 iIdx) {
    JUT_ASSERT(676, 0 <= iIdx && iIdx < mPathInfo->m_num);
    return mPathInfo->m_points[iIdx].m_position;
}

void daBaseNpc_path_c::dbgDraw(f32 param_0) {
    GXColor green = {0x80, 0xff, 0x80, 0xff};
    GXColor blue = {0x80, 0x80, 0xff, 0xff};
    GXColor red = {0xff, 0x80, 0x80, 0xff};
    GXColor yellow = {0xff, 0xff, 0x80, 0xff};

    cXyz unkXyz1;
    cXyz unkXyz2;

    for (int i = 0; i <= mPathInfo->m_num; i++) {
        unkXyz1.set(mPathInfo->m_points[i].m_position.x, param_0,
                    mPathInfo->m_points[i].m_position.z);
        dDbVw_drawSphereOpa(unkXyz1, 9.0f, red, 1);

        if (i != 0) {
            unkXyz1.set(mPathInfo->m_points[i - 1].m_position.x, param_0,
                        mPathInfo->m_points[i - 1].m_position.z);
            if (mPathInfo->m_num == i) {
                if (!dPath_ChkClose(mPathInfo)) {
                    return;
                }
                unkXyz2.set(mPathInfo->m_points[0].m_position.x, param_0,
                            mPathInfo->m_points[0].m_position.z);
            } else {
                unkXyz2.set(mPathInfo->m_points[i].m_position.x, param_0,
                            mPathInfo->m_points[i].m_position.z);
            }

            dDbVw_drawLineOpa(unkXyz1, unkXyz2, red, 1, 12);
        }
    }
}

void daBaseNpc_lookat_c::limitter(s16 i_value, s16* o_value_p, s16 i_max, s16 i_min) {
    int tmp = 0;
    tmp = (int)i_value + *o_value_p;

    if (i_max < tmp) {
        if ((int)i_value < i_max) {
            *o_value_p -= tmp - i_max;
        } else {
            *o_value_p = 0;
        }
    }

    tmp = (int)i_value + *o_value_p;

    if (tmp >= i_min) {
        return;
    }

    if (i_min < i_value) {
        //TODO: fakematch
#if VERSION == VERSION_SHIELD_DEBUG
        *o_value_p -= tmp - i_min;
#else
        *o_value_p -= tmp - (s16)i_min;
#endif
        return;
    }

    *o_value_p = 0;
}

// NONMATCHING - regalloc, equivalent ? (matches debug)
void daBaseNpc_lookat_c::calc(fopAc_ac_c* param_0, Mtx param_1, s16 param_2) {
    if (mpAttnPos == NULL) {
        for (int i = 0; i < 4; i++) {
            f32 sp28 = mRot[i].x;
            f32 sp24 = mRot[i].y;
            f32 sp20 = mRot[i].z;

            cLib_addCalc2(&sp28, 0.0f, 0.25f, 0x800 / (i + 1));
            cLib_addCalc2(&sp24, 0.0f, 0.25f, 0x800 / (i + 1));
            cLib_addCalc2(&sp20, 0.0f, 0.25f, 0x800 / (i + 1));

            mRot[i].set(sp28, sp24, sp20);
        }
        return;
    }

    cXyz sp140[4];
    Mtx sp110;
    Mtx spE0;
    csXyz spC8[4];
    f32 spB8[4];
    cXyz spAC;

    cMtx_copy(param_1, sp110);
    sp110[0][3] = sp110[1][3] = sp110[2][3] = 0.0f;

    mDoMtx_stack_c::copy(sp110);
    mDoMtx_stack_c::inverse();
    cMtx_copy(mDoMtx_stack_c::get(), sp110);
    mDoMtx_stack_c::transS(param_0->current.pos);
    mDoMtx_stack_c::concat(sp110);

    for (int i = 0; i < 4; i++) {
        spAC = mJntPos[i] - param_0->current.pos;
        mDoMtx_stack_c::multVec(&spAC, &sp140[i]);
    }

    cXyz spA0(*mpAttnPos);
    spAC = spA0 - param_0->current.pos;
    mDoMtx_stack_c::multVec(&spAC, &spA0);

    for (int i = 0; i < 3; i++) {
        spAC = sp140[i + 1] - sp140[i];
        
        spC8[i].set(0, 0, 0);
        spC8[i].x = -cM_atan2s(spAC.y, spAC.absXZ());

        if (fabsf(cM_ssin(spC8[i].x)) < 0.8f) {
            spC8[i].y = cM_atan2s(spAC.x, spAC.z);
        } else {
            spC8[i].y = 0;
        }

        spB8[i] = spAC.abs();
    }

    csXyz sp2C;
    cXyz sp94;
    cXyz sp88;

    for (int i = 2; i >= -1; i--) {
        sp2C = csXyz::Zero;
        for (int j = 0; j < 3; j++) {
            mDoMtx_stack_c::XYZrotS(spC8[j]);
            cMtx_copy(mDoMtx_stack_c::get(), spE0);
            mDoMtx_stack_c::transS(sp140[j]);

            sp2C.x += field_0x64[j].x;
            sp2C.y += field_0x64[j].y;

            mDoMtx_stack_c::ZXYrotM(sp2C);
            mDoMtx_stack_c::concat(spE0);
            spAC.set(0.0f, 0.0f, spB8[j]);
            mDoMtx_stack_c::multVec(&spAC, &sp140[j + 1]);
        }

        if (i == -1) {
            break;
        }

        if (param_2 == 0) {
            field_0x94[i] = 0;
        }

        if (field_0x94[i] == 0) {
            sp94 = spA0 - sp140[i];
            if (!sp94.isZero()) {
                sp94.normalize();
                sp88 = sp140[3] - sp140[i];

                if (!sp88.isZero()) {
                    sp88.normalize();
                    s16 sp16, sp14, sp12, sp10;
                    sp16 = -cM_atan2s(sp94.y, sp94.absXZ());
                    sp12 = cM_atan2s(sp94.x, sp94.z);
                    sp14 = -cM_atan2s(sp88.y, sp88.absXZ());
                    sp10 = cM_atan2s(sp88.x, sp88.z);

                    s16 spE = sp16 - sp14;
                    s16 spC = sp12 - sp10;
                    field_0x64[i].x += spE;
                    field_0x64[i].y += spC;

                    limitter(spC8[i].x, &field_0x64[i].x, mMaxJntLmt[i].x, mMinJntLmt[i].x);
                    limitter(spC8[i].y, &field_0x64[i].y, mMaxJntLmt[i].y, mMinJntLmt[i].y);
                }
            }
        }

        if ((param_2 > 0 && field_0x64[i].y < 0) || (param_2 < 0 && field_0x64[i].y > 0)) {
            field_0x94[i] = 1;
        }
    }

    sp2C = csXyz::Zero;
    for (int i = 0; i < 4; i++) {
        sp2C += field_0x64[i];
        mRot[i] = sp2C;
    }
}

void daBaseNpc_lookat_c::setMaxJntLmt(csXyz i_max, int i_no) {
    mMaxJntLmt[i_no] = i_max;
}

void daBaseNpc_lookat_c::setMinJntLmt(csXyz i_min, int i_no) {
    mMinJntLmt[i_no] = i_min;
}

void daBaseNpc_lookat_c::setJntPos(cXyz i_pos, int i_no) {
    mJntPos[i_no] = i_pos;
}

int daBaseNpc_lookat_c::setAttnPos(cXyz* i_pos) {
    mpAttnPos = i_pos;
    return 1;
}

csXyz daBaseNpc_lookat_c::getRot(int i_no) {
    return mRot[i_no];
}

daBaseNpc_c::daBaseNpc_c() {}

daBaseNpc_c::~daBaseNpc_c() {}

int daBaseNpc_c::execute() {
    setParam();
    mainProc();

    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    field_0x9d4 = mAcch.m_gnd;

    mGroundY = mAcch.GetGroundH();
    if (mGroundY != -G_CM3D_F_INF) {
        field_0x918 = daBaseNpc_getGroundAngle(&field_0x9d4, field_0x91a.y);
        setEnvTevColor();
        setRoomNo();
    }

    allAnmPlay();
    adjustShapeAngle();
    setMtx(0);
    setCollisions();
    setAttnPos();
    return 1;
}

int daBaseNpc_c::draw(f32 param_0) {
    J3DModel* model_p = mpModelMorf->getModel();
    J3DModelData* modelData_p = model_p->getModelData();

    if (dKy_darkworld_check()) {
        g_env_light.settingTevStruct(4, &current.pos, &tevStr);
    } else {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    }

    g_env_light.setLightTevColorType_MAJI(model_p, &tevStr);

    if (!dbgDraw()) {
        return 1;
    }

    if (cLib_checkBit<u16>((u16)field_0x840, 0x40)) {
        mBtp.entry(modelData_p);
    }

    if (cLib_checkBit<u16>((u16)field_0x840, 0x80)) {
        mBtk.entry(modelData_p);
    }

    if (dKy_darkworld_check()) {
        dComIfGd_setListDark();
        mpModelMorf->entryDL();
        dComIfGd_setList();
    } else {
        mpModelMorf->entryDL();
    }

    if (cLib_checkBit<u16>((u16)field_0x840, 0x40)) {
        mBtp.remove(modelData_p);
    }

    if (cLib_checkBit<u16>((u16)field_0x840, 0x80)) {
        mBtk.remove(modelData_p);
    }

    mShadowId = dComIfGd_setShadow(mShadowId, 1, model_p, &current.pos, param_0, 20.0f, current.pos.y, mGroundY, field_0x9d4, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    drawOtherMdls();
    return 1;
}

const char* daBaseNpc_c::getResName() {
    return NULL;
}

J3DAnmTransform* daBaseNpc_c::getTrnsfrmKeyAnmP(char* i_arcName, int i_resIdx) {
    void* objectRes = dComIfG_getObjectRes(i_arcName, i_resIdx);
    return (J3DAnmTransform*)objectRes;
}

int daBaseNpc_c::setMcaMorfAnm(J3DAnmTransformKey* i_anm, f32 i_speed, f32 i_morf,
                                int i_mode, int i_start, int i_end) {
    f32 start = i_start;
    f32 end = i_end;
    mpModelMorf->setAnm(i_anm, i_mode, i_morf, i_speed, start, end);
    return 1;
}

int daBaseNpc_c::setBckAnm(J3DAnmTransform* i_anm, f32 i_speed, int i_mode, int i_start,
                            int i_end, bool i_modify) {
    s16 start = i_start;
    s16 end = i_end;
    return mBck.init(i_anm, TRUE, i_mode, i_speed, start, end, i_modify);
}

J3DAnmTransform* daBaseNpc_c::getTexPtrnAnmP(char* i_arcName, int i_resIdx) {
    void* objectRes = dComIfG_getObjectRes(i_arcName, i_resIdx);
    return (J3DAnmTransform*)objectRes;
}

int daBaseNpc_c::setBtpAnm(J3DAnmTexPattern* i_anm, J3DModelData* i_modelData, f32 i_speed,
                            int i_mode) {
    return mBtp.init(i_modelData, i_anm, TRUE, i_mode, i_speed, 0, -1);
}

void daBaseNpc_c::attnSttsOn(int param_0, int param_1) {
    u32 attn_flags = 0;

    if (daPy_py_c::checkNowWolf()) {
        if (param_1 != 0) {
            attn_flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
        } else {
            attn_flags = fopAc_AttnFlag_LOCK_e;
        }
    } else if (param_0 != 0) {
        attn_flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
    } else {
        attn_flags = fopAc_AttnFlag_LOCK_e;
    }

    attention_info.flags = attn_flags;
}

void daBaseNpc_c::setParam() {}

void daBaseNpc_c::orderEvent(int param_0, char* i_evtName) {
    if (i_evtName != NULL) {
        mEvtIdx = dComIfGp_getEventManager().getEventIdx(this, i_evtName, -1);
        fopAcM_orderOtherEventId(this, mEvtIdx, 0xFF, 0xFFFF, 0, 1);
    } else if (field_0x848 >= 0 && attention_info.flags == (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e)) {
        eventInfo.onCondition(1);
        if (param_0 != 0) {
            fopAcM_orderSpeakEvent(this, 0, 0);
        }
    }
}

void daBaseNpc_c::mainProc() {}

void daBaseNpc_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(field_0x9d4);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(field_0x9d4);
}

void daBaseNpc_c::setRoomNo() {
    s8 room_id = dComIfG_Bgsp().GetRoomId(field_0x9d4);
    fopAcM_SetRoomNo(this, room_id);
    mCcStts.SetRoomId(room_id);
}

BOOL daBaseNpc_c::btkCtrl() {
    return false;
}

int daBaseNpc_c::checkEndAnm(f32 param_0) {
    switch ((u8)mpModelMorf->getPlayMode()) {
    case J3DFrameCtrl::EMode_LOOP:
        return mpModelMorf->isLoop();
    case J3DFrameCtrl::EMode_NONE:
    case J3DFrameCtrl::EMode_RESET:
        return mpModelMorf->isStop() && param_0 != mpModelMorf->getPlaySpeed();
    case J3DFrameCtrl::EMode_REVERSE:
    case J3DFrameCtrl::EMode_LOOP_REVERSE:
        break;
    }

    return 0;
}

int daBaseNpc_c::checkEndAnm(J3DFrameCtrl* i_framectrl) {
    switch (i_framectrl->getAttribute()) {
    case J3DFrameCtrl::EMode_LOOP:
        return i_framectrl->checkState(2);
    case J3DFrameCtrl::EMode_NONE:
    case J3DFrameCtrl::EMode_RESET:
        return i_framectrl->checkState(1) || i_framectrl->getRate() == 0.0f;
    case J3DFrameCtrl::EMode_REVERSE:
    case J3DFrameCtrl::EMode_LOOP_REVERSE:
        break;
    }

    return 0;
}

void daBaseNpc_c::allAnmPlay() {
    u32 offMask = 0x10F;
    if (cLib_checkBit<u16>((u16)field_0x840, 0x10) &&
        !cLib_checkBit<u16>((u16)field_0x840, 1)) {
        f32 temp_f31 = mpModelMorf->getPlaySpeed();
        mpModelMorf->play(0, 0);

        if (checkEndAnm(temp_f31)) {
            field_0x842++;
        }
    }

    if (cLib_checkBit<u16>((u16)field_0x840, 0x80) &&
        !cLib_checkBit<u16>((u16)field_0x840, 8) && !btkCtrl()) {
        mBtk.play();
    }

    if (!cLib_checkBit<u16>((u16)field_0x840, 0x100)) {
        if (cLib_checkBit<u16>((u16)field_0x840, 0x20) &&
            !cLib_checkBit<u16>((u16)field_0x840, 2)) {
            mBck.play();
        }

        if (cLib_checkBit<u16>((u16)field_0x840, 0x40) &&
            !cLib_checkBit<u16>((u16)field_0x840, 4)) {
            f32 frame = mBtp.getFrame();
            mBtp.play();

            if (mBtp.getFrameCtrl()->getAttribute() == 2) {
                if (checkEndAnm(mBtp.getFrameCtrl())) {
                    field_0x838 = cLib_getRndValue<int>(60, 100);
                    mBtp.setPlaySpeed(0.0f);
                }
            } else {
                field_0x838 = 0;
            }

            if (cLib_calcTimer<s16>(&field_0x838) == 0) {
                mBtp.setPlaySpeed(1.0f);
            }
        }
    }

    cLib_offBit<u16>(field_0x840, offMask);
}

void daBaseNpc_c::adjustShapeAngle() {}

void daBaseNpc_c::setMtx(int param_0) {
    J3DModel* model = mpModelMorf->getModel();
    J3DModelData* modelData = model->getModelData();

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x91a);
    mDoMtx_stack_c::scaleM(scale);

    model->setBaseTRMtx(mDoMtx_stack_c::get());

    if (param_0) {
        model->setUserArea((uintptr_t)this);
    } else {
        model->setUserArea(0);
    }

    if (cLib_checkBit<u16>((u16)field_0x840, 0x10) && field_0x83c) {
        u16 tmp = 0;
        while (field_0x83c[tmp] != 0xFFFF) {
            mBck.entryJoint(modelData, field_0x83c[tmp]);
            tmp++;
        }

        mpModelMorf->modelCalc();

        u16 tmp2 = 0;
        while (field_0x83c[tmp2] != 0xFFFF) {
            mBck.removeJoint(modelData, field_0x83c[tmp2]);
            tmp2++;
        }
    } else {
        mpModelMorf->modelCalc();
    }
}

void daBaseNpc_c::setCollisions() {}

void daBaseNpc_c::setAttnPos() {}

void daBaseNpc_c::drawOtherMdls() {}

bool daBaseNpc_c::dbgDraw() {
    return true;
}

daBaseNpc_moveBgActor_c::daBaseNpc_moveBgActor_c() {
    mpBgw = 0;
}

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    daBaseNpc_moveBgActor_c* moveBgActor = (daBaseNpc_moveBgActor_c*)i_this;
    return moveBgActor->MoveBGCreateHeap();
}

const char* daBaseNpc_moveBgActor_c::m_name;

int daBaseNpc_moveBgActor_c::m_dzb_id;

MoveBGActor_SetFunc daBaseNpc_moveBgActor_c::m_set_func;

int daBaseNpc_moveBgActor_c::MoveBGCreateHeap() {
    if (!CreateHeap()) {
        return 0;
    }

    if (m_dzb_id == -1) {
        mpBgw = NULL;
        return 1;
    }

    mpBgw = new dBgW();

    if (mpBgw != NULL && !mpBgw->Set((cBgD_t*)dComIfG_getObjectRes(m_name, m_dzb_id), 1, &mBgMtx)) {
        if (m_set_func != NULL) {
            mpBgw->SetCrrFunc(m_set_func);
        }
    } else {
        mpBgw = NULL;
        return 0;
    }

    return 1;
}

/* 8014F60C-8014F6FC 149F4C 00F0+00 0/0 0/0 1/1 .text
 * MoveBGCreate__23daBaseNpc_moveBgActor_cFPCciPFP4dBgWPvRC13cBgS_PolyInfobP4cXyzP5csXyzP5csXyz_vUl
 */
int daBaseNpc_moveBgActor_c::MoveBGCreate(char const* i_arcName, int i_dzbId, MoveBGActor_SetFunc i_setFunc,
                                          u32 i_heapSize) {
    int unusedInt1 = 1;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::scaleM(scale);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);

    m_name = i_arcName;
    m_dzb_id = i_dzbId;
    m_set_func = i_setFunc;

    if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, i_heapSize)) {
        return cPhs_ERROR_e;
    }

#if DEBUG
    if (mpBgw != NULL && mpBgw->ChkUsed()) {
        BOOL isConnect = mDoCPd_c::isConnect(PAD_3);
        if (isConnect) {
            JUT_WARN(2019, "%s", "dBgS_MoveBgActor::MoveBGCreate() Don't Regist CreateHeap\n");
        }
    }
#endif

    if (mpBgw != NULL && dComIfG_Bgsp().Regist(mpBgw, this)) {
        return cPhs_ERROR_e;
    }

    //TODO: Possible fakematch - retail definitely looks like a ternary but
    //      debug looks like if-else and has inverted order
#if DEBUG
    if (!Create()) {
        return 5;
    } else {
        return 4;
    }
#else
    //return Create() ? cPhs_COMPLEATE_e : cPhs_ERROR_e;  // using enums here changes code gen
    return Create() ? 4 : 5;
#endif
}

int daBaseNpc_moveBgActor_c::MoveBGDelete() {
    int rt = Delete();
    if (mpBgw != NULL && mpBgw->ChkUsed()) {
        bool result = dComIfG_Bgsp().Release(mpBgw);
        if (result) {
            OS_REPORT("Release Error\n");
        }
    }

    return rt;
}

int daBaseNpc_moveBgActor_c::MoveBGExecute() {
    Mtx* sp8 = NULL;
    int rt = Execute(&sp8);

    if (sp8 == NULL) {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::scaleM(scale);
        MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
    } else {
        MTXCopy(*sp8, mBgMtx);
    }

    if (mpBgw != NULL) {
        mpBgw->Move();
    }

    return rt;
}

static int daBaseNpc_PntVsLineSegmentLengthSquare2D(f32 param_0, f32 param_1, f32 param_2,
                                                    f32 param_3, f32 param_4, f32 param_5,
                                                    f32* param_6, f32* param_7, f32* param_8) {
    int var_r31 = 0;

    f32 temp_f31 = param_4 - param_2;
    f32 temp_f30 = param_5 - param_3;
    f32 temp_f28 = (temp_f31 * temp_f31) + (temp_f30 * temp_f30);
    if (temp_f28 == 0.0f) {
        *param_8 = 0.0f;
        return 0;
    }

    f32 temp_f29 = ((temp_f31 * (param_0 - param_2)) + (temp_f30 * (param_1 - param_3))) / temp_f28;
    if (0.0f <= temp_f29 && temp_f29 <= 1.0f) {
        var_r31 = 1;
    }

    *param_6 = param_2 + (temp_f31 * temp_f29);
    *param_7 = param_3 + (temp_f30 * temp_f29);
    *param_8 = ((*param_6 - param_0) * (*param_6 - param_0)) + ((*param_7 - param_1) * (*param_7 - param_1));
    return var_r31;
}

static u16 daBaseNpc_putNurbs(dPnt* i_CPnts, int i_CPntNum, int i_maxPntNum, dPnt* o_curve_p, BOOL i_isClosed) {
    int knotVector[daBaseNpc_path_c::MAXNUMCONTROLPNT_e + 4];
    int weights[daBaseNpc_path_c::MAXNUMCONTROLPNT_e];

    int i;
    int aDeg = 2;  // curve degree
    int aMaxNumCPnt;  // max control points
    if (i_isClosed) {
        aMaxNumCPnt = i_CPntNum + 1;
    } else {
        aMaxNumCPnt = i_CPntNum;
    }

    JUT_ASSERT(2190, aDeg < aMaxNumCPnt && aMaxNumCPnt < daBaseNpc_path_c::MAXNUMCONTROLPNT_e);
    JUT_ASSERT(2191, aDeg == 2 || aDeg == 3);

    // create knot vector
    for (i = 0; i < aDeg + aMaxNumCPnt + 1; i++) {
        if (aMaxNumCPnt < i) {
            knotVector[i] = knotVector[i - 1];
        } else if (aDeg < i) {
            knotVector[i] = knotVector[i - 1] + 10;
        } else {
            knotVector[i] = 0;
        }
    }

    // initialize weights
    for (i = 0; i < aMaxNumCPnt; i++) {
        weights[i] = 1;
    }

    int cur_knot;
    u16 out_pnt_no = 0;
    f32 total_weight;
    f32 basis;
    for (cur_knot = knotVector[aDeg]; cur_knot < knotVector[aDeg + aMaxNumCPnt]; cur_knot += 2, out_pnt_no++) {
        if (i_maxPntNum <= out_pnt_no) {
            break;
        }

        o_curve_p[out_pnt_no].m_position.x = 0.0f;
        o_curve_p[out_pnt_no].m_position.z = 0.0f;

        total_weight = 0.0f;
        for (i = 0; i < aMaxNumCPnt; i++) {
            basis = daNpcBase_BBasis(aDeg, i, cur_knot, knotVector);
            if (i == i_CPntNum) {
                o_curve_p[out_pnt_no].m_position.x += i_CPnts[0].m_position.x * basis;
                o_curve_p[out_pnt_no].m_position.z += i_CPnts[0].m_position.z * basis;
            } else {
                o_curve_p[out_pnt_no].m_position.x += i_CPnts[i].m_position.x * basis;
                o_curve_p[out_pnt_no].m_position.z += i_CPnts[i].m_position.z * basis;
            }

            total_weight += basis * weights[i];
        }

        o_curve_p[out_pnt_no].m_position.x /= total_weight;
        o_curve_p[out_pnt_no].m_position.z /= total_weight;
    }

    // add final point for open curves
    if (!i_isClosed) {
        o_curve_p[out_pnt_no].m_position.x = i_CPnts[i_CPntNum - 1].m_position.x;
        o_curve_p[out_pnt_no].m_position.z = i_CPnts[i_CPntNum - 1].m_position.z;
        out_pnt_no++;
    }

    return out_pnt_no;
}

static s32 daBaseNpc_chkPnt(cXyz param_0, dPnt* param_1, u16 param_2, u16 param_3, int param_4,
                            int param_5) {
    u16 var_r29 = param_2;
    u16 sp12, sp10;
    sp10 = var_r29;
    sp12 = var_r29;
    daBaseNpc_incIdx(param_3, &sp10, param_4, 1);
    daBaseNpc_decIdx(param_3, &sp12, param_4, 1);

    cXyz sp70;
    cXyz sp64(param_1[sp12].m_position.x, param_1[sp12].m_position.y, param_1[sp12].m_position.z);
    cXyz sp58(param_1[var_r29].m_position.x, param_1[var_r29].m_position.y, param_1[var_r29].m_position.z);
    cXyz sp4C(param_1[sp10].m_position.x, param_1[sp10].m_position.y, param_1[sp10].m_position.z);

    f32 sp24;
    s16 var_r31;
    if (sp12 == var_r29 && var_r29 == sp10) {
        JUT_ASSERT(2288, FALSE);
    } else if (sp12 < var_r29 && var_r29 < sp10) {
        sp24 = (sp4C - sp64).absXZ();
        var_r31 = cM_atan2s(sp4C.x - sp64.x, sp4C.z - sp64.z);

        sp70 = sp64;
        sp64.x = sp70.x + (-1.0f * sp24 * cM_ssin(var_r31));
        sp64.z = sp70.z + (-1.0f * sp24 * cM_scos(var_r31));
        sp4C.x = sp70.x + (2.0f * sp24 * cM_ssin(var_r31));
        sp4C.z = sp70.z + (2.0f * sp24 * cM_scos(var_r31));
    } else if (sp12 < var_r29) {
        sp24 = (sp58 - sp64).absXZ();
        var_r31 = cM_atan2s(sp58.x - sp64.x, sp58.z - sp64.z);

        sp70 = sp58;
        sp64.x = sp70.x + (-2.0f * sp24 * cM_ssin(var_r31));
        sp64.z = sp70.z + (-2.0f * sp24 * cM_scos(var_r31));
        sp4C.x = sp70.x + (2.0f * sp24 * cM_ssin(var_r31));
        sp4C.z = sp70.z + (2.0f * sp24 * cM_scos(var_r31));
    } else if (var_r29 < sp10) {
        sp24 = (sp4C - sp58).absXZ();
        var_r31 = cM_atan2s(sp4C.x - sp58.x, sp4C.z - sp58.z);

        sp70 = sp58;
        sp64.x = sp70.x + (-2.0f * sp24 * cM_ssin(var_r31));
        sp64.z = sp70.z + (-2.0f * sp24 * cM_scos(var_r31));
        sp4C.x = sp70.x + (2.0f * sp24 * cM_ssin(var_r31));
        sp4C.z = sp70.z + (2.0f * sp24 * cM_scos(var_r31));
    } else {
        JUT_ASSERT(2322, FALSE);
    }

    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;

    daBaseNpc_PntVsLineSegmentLengthSquare2D(param_0.x, param_0.z, sp64.x, sp64.z, sp4C.x, sp4C.z, &sp20, &sp1C, &sp24);

    if (cM3d_IsZero(sp24)) {
        return 0;
    }

    daBaseNpc_PntVsLineSegmentLengthSquare2D(sp58.x, sp58.z, sp64.x, sp64.z, sp4C.x, sp4C.z, &sp18, &sp14, &sp24);

    if (param_5 < 0) {
        var_r31 = cM_atan2s(sp64.x - sp4C.x, sp64.z - sp4C.z);
    } else {
        var_r31 = cM_atan2s(sp4C.x - sp64.x, sp4C.z - sp64.z);
    }

    var_r31 -= cM_atan2s(sp18 - sp20, sp14 - sp1C);
    return (u16)abs(var_r31) > 0x4000;
}

static s16 daBaseNpc_getGroundAngle(cBgS_PolyInfo* param_0, s16 param_1) {
    if (dComIfG_Bgsp().ChkPolySafe(*param_0)) {
        cM3dGPla spC;
        bool triPla = dComIfG_Bgsp().GetTriPla(*param_0, &spC);
        if (!triPla || !cBgW_CheckBGround(spC.mNormal.y)) {
            return 0;
        }

        return fopAcM_getPolygonAngle(&spC, param_1);
    }

    return 0;
}
