/**
* @file d_a_e_hp.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_hp.h"
#include "d/actor/d_a_alink.h"
#include "d/d_debug_viewer.h"
#include "f_op/f_op_actor_enemy.h"

class daE_HP_HIO_c : public JORReflexible {
public:
    daE_HP_HIO_c();
    virtual ~daE_HP_HIO_c() {}

#if DEBUG
    void genMessage(JORMContext*);
#endif

    s8 mChild;
    f32 modelSize;
    s16 waitTimeAfterAttack;
    s16 resurrectionTime;
    s16 attackDelayOnApproach;
    u8 rangeDisplay;
};

namespace {
static dCcD_SrcCyl cc_hp_src = {
    {
        {0, {{0x0, 0x0, 0x0}, {(s32)0xd8000000, 0x3}, 0x0}},
        {dCcD_SE_13, 0, 0, 0, {0}},
        {dCcD_SE_NONE, 0, 0, 0, {6}},
        {0},
    },
    {
        {0.0f, 0.0f, 0.0f},
        50.0f,
        300.0f,
    }
};

static dCcD_SrcSph cc_lamp_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {(s32)0xD8FBFDFF, 0x43}, 0x75}},  // mObj
        {dCcD_SE_13, 0x0, 0x0, 0x0, 0x0},                         // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},                       // mGObjTg
        {0x0},                                                    // mGObjCo
    },                                                            // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
    }  // mSphAttr
};

static dCcD_SrcCyl cc_hp_at_src = {
    {
        {0, {{0x400, 0x1, 0xd}, {0x0, 0x0}, 0x0}},
        {dCcD_SE_METAL, 0, 1, 0, {0}},
        {dCcD_SE_NONE, 0, 0, 0, {2}},
        {0},
    },
    {
        {0.0f, 0.0f, 0.0f},
        50.0f,
        300.0f,
    }
};
}  // namespace

daE_HP_HIO_c::daE_HP_HIO_c() {
    mChild = -1;
    modelSize = 1.2f;
    waitTimeAfterAttack = 40;
    resurrectionTime = 100;
    attackDelayOnApproach = 50;
    rangeDisplay = 0;
}

#if DEBUG
void daE_HP_HIO_c::genMessage(JORMContext* ctx) {
    // General-purpose Poe
    ctx->genLabel("汎用ポゥ", 0x80000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("モデルサイズ", &modelSize, 0.0, 10.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("攻撃終了後待ち時間", &waitTimeAfterAttack, 0, 1000, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("復活時間", &resurrectionTime, 0, 1000, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("接近後攻撃しない時間", &attackDelayOnApproach, 0, 0xfe, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genCheckBox("範囲表示(FINALでは不可)", &rangeDisplay, 1, 0, NULL, 0xffff, 0xffff, 0x200,
                     0x18);
}
#endif

int daE_HP_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* joint = i_joint;
    s32 jointNo = joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jointNo));

    switch (jointNo) {
    case 8:
        mDoMtx_stack_c::XrotM(field_0x7aa);
        break;
    }

    if (TREG_S(0) != 0 && jointNo == TREG_S(0)) {
        mDoMtx_stack_c::XrotM(TREG_S(1));
        mDoMtx_stack_c::YrotM(TREG_S(2));
        mDoMtx_stack_c::ZrotM(TREG_S(3));
    }

    i_model->setAnmMtx(jointNo, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

int daE_HP_c::JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == NULL) {
        J3DModel* model = j3dSys.getModel();
        daE_HP_c* poe = (daE_HP_c*)model->getUserArea();
        if (poe != NULL) {
            poe->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

int daE_HP_c::LampCtrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* joint = i_joint;
    s32 jointNo = joint->getJntNo();

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jointNo));

    switch (jointNo) {
    case 2:
        mDoMtx_stack_c::XrotM(field_0x76c.x);
        mDoMtx_stack_c::ZrotM(field_0x76c.z);
    }

    if (TREG_S(0) != 0 && jointNo == (s32)TREG_S(0)) {
        mDoMtx_stack_c::XrotM(TREG_S(1));
        mDoMtx_stack_c::YrotM(TREG_S(2));
        mDoMtx_stack_c::ZrotM(TREG_S(3));
    }

    i_model->setAnmMtx(jointNo, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

int daE_HP_c::LampJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == NULL) {
        J3DModel* model = j3dSys.getModel();
        daE_HP_c* poe = (daE_HP_c*)model->getUserArea();
        if (poe != NULL) {
            poe->LampCtrlJoint(i_joint, model);
        }
    }

    return 1;
}

static bool l_HIOInit;

static daE_HP_HIO_c l_HIO;

int daE_HP_c::draw() {
    /* 806EA3E8-806EA3EC 000100 0004+00 1/1 0/0 0/0 .data            particleNmaeDt$4030 */
    static u16 particleNmaeDt[2] = {
        0x8789,
        0x878A,
    };

    if (field_0x77c != 0xff && fopAcM_isSwitch(this, field_0x77c) == 0) {
        return 1;
    }

    if (mNight == 1 && !dKy_daynight_check()) {
        return 1;
    }

    if (attention_info.distances[fopAc_attn_BATTLE_e] == 0) {
        attention_info.distances[fopAc_attn_BATTLE_e] = 0x04;
        fopAcM_SetGroup(this, 2);
        fopAcM_OnStatus(this, 0);
        attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
    }

    J3DModel* model = mpMorfSO->getModel();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    J3DModelData* modelData = model->getModelData();
    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        J3DMaterial* material = modelData->getMaterialNodePointer(i);
        if (material != NULL) {
            material->getTevKColor(3)->a = field_0x790;
        }
    }

    if (mAction == 6 || field_0x790 && (!dComIfGp_event_runCheck() ||
                                        fopAcM_getTalkEventPartner(dComIfGp_getLinkPlayer()) !=
                                            daPy_py_c::getMidnaActor()))
    {
        mInvModel.entryDL(0);
    } else {
        field_0x790 = 0.0f;
    }

    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    mDoExt_modelUpdateDL(mpModel);

    cXyz curPosWithOffset;
    curPosWithOffset.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);

    field_0x718 =
        dComIfGd_setShadow(field_0x718, 1, mpModel, &curPosWithOffset, 600.0f + BREG_F(0x12),
                           BREG_F(0x13), current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd,
                           &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    cXyz unkXyz1;
    mDoMtx_stack_c::copy(mpModel->getAnmMtx(2));
    unkXyz1.set(30.0f + JREG_F(10), 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&unkXyz1, &field_0x75c);
    mDoMtx_stack_c::transS(field_0x75c);

    unkXyz1 = dComIfGp_getCamera(0)->lookat.eye - field_0x75c;
    mDoMtx_stack_c::YrotM(unkXyz1.atan2sX_Z());
    mDoMtx_stack_c::XrotM((s16)unkXyz1.atan2sY_XZ());
    mDoMtx_stack_c::scaleM(l_HIO.modelSize, l_HIO.modelSize, l_HIO.modelSize);

    model = mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();

    mDoMtx_stack_c::copy(mpModel->getAnmMtx(2));
    mDoMtx_stack_c::multVecZero(&unkXyz1);

    for (s32 i = 0; i < 2; i++) {
        field_0xdd4[i] = dComIfGp_particle_set(field_0xdd4[i], particleNmaeDt[i], &unkXyz1, 0, 0);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0xdd4[i]);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(mpModel->getAnmMtx(2));
        }
    }

    mpMorf->getModel();
    mpMorf->entryDL();

    if (checkDownFlg()) {
        cXyz local_50;
        cXyz cStack_5c;
        cMtx_YrotS(*calc_mtx, shape_angle.y);
        local_50.x = 0.0f;
        local_50.y = field_0x7a0;
        local_50.z = -20.0f + JREG_F(1);
        MtxPosition(&local_50, &cStack_5c);
        field_0x738 = cStack_5c + current.pos;
        setDownPos(&field_0x738);
        cLib_addCalc2(&field_0x7a0, 40.0f + JREG_F(2), 0.7, JREG_F(7) + 4.0f);
    }

    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    if (!player->checkWolfDownAttackPullOut() && mAction != 6) {
        drawBallModel(&tevStr);
    }

#if DEBUG
    if (WREG_S(0) != 0 || l_HIO.rangeDisplay != 0) {
        cXyz cStack_68;
        GXColor color;
        color.r = 0xff;
        color.g = 0x82;
        color.b = 0x82;
        color.a = 0xfa;
        dDbVw_drawCircleXlu(home.pos, mDisHani, color, 1, 0xc);
    }
#endif

    return 1;
}

static int daE_HP_Draw(daE_HP_c* param_0) {
    return param_0->draw();
}

void daE_HP_c::setBck(int param_0, u8 param_1, f32 param_2, f32 param_3) {
    mpMorfSO->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_HP", param_0), param_1, param_2,
                     param_3, 0.0f, -1.0f);
    field_0x780 = param_0;
}

void daE_HP_c::setActionMode(int param_0, int i_mode) {
    mAction = param_0;
    movemode = i_mode;
}

bool daE_HP_c::mChkDistance(f32 param_0) {
    cXyz vecToPlayer = dComIfGp_getPlayer(0)->current.pos - current.pos;
    if (vecToPlayer.abs() < param_0) {
        return true;
    } else {
        return false;
    }
}

void daE_HP_c::damage_check() {
    if (health <= 1 || !daPy_py_c::checkNowWolf() || field_0x71e != 0 || field_0x790 < 250.0f ||
        mAction == 5 || mAction == 6)
    {
        return;
    }

    cXyz cStack_1c;
    mDoMtx_stack_c::copy(mpMorfSO->getModel()->getAnmMtx(1));
    mDoMtx_stack_c::multVecZero(&cStack_1c);

    scale.set(l_HIO.modelSize, l_HIO.modelSize, l_HIO.modelSize);

    setMidnaBindEffect(this, &mSound1, &cStack_1c, &scale);

    mStts.Move();

    if (mCyl1.ChkTgHit()) {
        csXyz unkSXyz;
        cXyz unkXyz1;
        cXyz unkXyz2;

        mAtInfo.mpCollider = mCyl1.GetTgHitObj();
        mAtInfo.field_0x18 = 31;
        cc_at_check(this, &mAtInfo);

        unkXyz1 = current.pos - *mCyl1.GetTgHitPosP();
        unkXyz2.set(*mCyl1.GetTgHitPosP());
        unkSXyz.x = 0;
        unkSXyz.y = (s32)unkXyz1.atan2sX_Z();
        unkSXyz.z = 0;
        dComIfGp_setHitMark(3, this, &unkXyz2, &unkSXyz, NULL, 0);

        field_0x71e = 8;

        mCyl1.ClrTgHit();

        if (health > 1) {
            setActionMode(4, 0);
        } else {
            health = 0;

            setActionMode(5, 0);
        }
    }
}

void daE_HP_c::executeWait() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz homeToPlayer;

    switch (movemode) {
    case 0:
        mStts.SetWeight(250);
        setBck(13, 2, 3.0f, 1.0f);

        mSound1.startCreatureVoice(Z2SE_EN_HP_V_LAUGH, -1);

        movemode = 1;
    case 1:
        homeToPlayer = home.pos - player->current.pos;
        if (homeToPlayer.abs() < mDisHani) {
            mStts.SetWeight(100);

            setActionMode(1, 0);
        }

        break;
    case 10:
        mStts.SetWeight(100);
        setBck(10, 0, 3.0f, 1.0f);

        mSound1.startCreatureVoice(Z2SE_EN_HP_V_FOUND, -1);

        movemode++;

        break;
    case 11:
        if (mpMorfSO->isStop()) {
            setActionMode(1, 0);
        }

        break;
    case 100:
        field_0x71c = 40;

        setBck(0xd, 0x02, 3.0f, 1.0f);

        mSound1.startCreatureVoice(Z2SE_EN_HP_V_LAUGH, -1);

        movemode++;
    case 101:
        if (field_0x71c == 0) {
            setActionMode(2, 0);
        }

        break;
    }

    cLib_addCalc0(&speedF, 0.7f, 5.0f);
}

void daE_HP_c::executeMove() {
    cXyz vecToHome;
    switch (movemode) {
    case 0:
        field_0x71f = 0;
        setBck(12, 2, 10.0f, 1.0f);
        mSound1.startCreatureVoice(Z2SE_EN_HP_V_LAUGH, -1);
        movemode = 1;
    case 1:
        speedF = 20.0f + NREG_F(5);
        vecToHome = home.pos - current.pos;
        if (vecToHome.abs() > mDisHani) {
            setActionMode(0, 100);
        } else {
            if (mChkDistance(300.0f + NREG_F(7))) {
                if (!daPy_py_c::checkNowWolfPowerUp()) {
                    field_0x71f++;
                    if (field_0x71f < l_HIO.attackDelayOnApproach) {
                        speedF = 0.0f;
                        break;
                    }
                }

                setActionMode(3, 0);
            }
        }

        break;
    }

    cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), 4, 0x800);
}

void daE_HP_c::executeRetMove() {
    cXyz vecToHome;

    switch (movemode) {
    case 0:
        field_0x71f = 0;
        setBck(12, 2, 3.0f, 1.0f);

        mSound1.startCreatureVoice(Z2SE_EN_HP_V_LAUGH, -1);

        movemode = 1;
    case 1:
        speedF = 20.0f + NREG_F(6);

        vecToHome = home.pos - current.pos;
        vecToHome.y = 0.0f;

        if (vecToHome.abs() < mDisHani * 0.5f && mChkDistance(300.0f + NREG_F(7)) != 0) {
            if (daPy_py_c::checkNowWolfPowerUp() == 0) {
                field_0x71f++;
                if (field_0x71f < l_HIO.attackDelayOnApproach) {
                    speedF = 0.0f;

                    break;
                }
            }
            setActionMode(3, 0);
        } else {
            current.angle.y = (s16)vecToHome.atan2sX_Z();

            if (vecToHome.abs() < 20.0f) {
                setActionMode(0, 0);
            }
        }

        break;
    }
}

void daE_HP_c::executeAttack() {
    cXyz vecToHome;
    switch (movemode) {
    case 0:
        current.angle.y = fopAcM_searchPlayerAngleY(this);
        speedF = 20.0f + NREG_F(10);
        setBck(5, 0, 3.0f, 1.0f);

        movemode++;

        mSound1.startCreatureVoice(Z2SE_EN_HP_V_ATTACK, -1);

        field_0x71c = 1;

        break;
    case 1:
        if (mpMorfSO->checkFrame(20.0f) || mpMorfSO->checkFrame(32.0f)) {
            mSound1.startCreatureSound(Z2SE_EN_HP_SWING, 0, -1);
        }

        if (mpMorfSO->isStop()) {
            setBck(14, 2, 3.0f, 1.0f);

            mSound1.startCreatureVoice(Z2SE_EN_HP_V_LAUGH, -1);

            field_0x71c = l_HIO.waitTimeAfterAttack;

            movemode++;
        }

        break;
    case 2:
        if (field_0x71c == 0) {
            if (!mChkDistance(300.0f + NREG_F(7))) {
                setActionMode(1, 0);
            } else {
                movemode = 0;
            }

            vecToHome = home.pos - current.pos;
            if (vecToHome.abs() > mDisHani) {
                setActionMode(0, 100);
            }
        }
        break;
    }

    cLib_addCalc0(&speedF, 0.7f, 1.0f);
    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 1, 0x1000);
}

void daE_HP_c::executeDamage() {
    switch (movemode) {
    case 0: {
        field_0x789 = 1;
        setBck(6, 0, 3.0f, 1.0f);

        mSound1.startCreatureVoice(Z2SE_EN_HP_V_DAMAGE, -1);

        movemode = 1;

        break;
    }
    case 1: {
        if (mpMorfSO->isStop()) {
            cXyz vecToHome;
            if (mChkDistance(300.0f + NREG_F(7))) {
                setActionMode(3, 0);
            } else {
                setActionMode(1, 0);
            }

            vecToHome = home.pos - current.pos;
            if (vecToHome.abs() > mDisHani) {
                setActionMode(0, 100);
            }
        }

        break;
    }
    }

    cLib_addCalc0(&speedF, 0.7f, 5.0f);
}

void daE_HP_c::executeDown() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    dBgS_LinChk dStack_98;
    cXyz unkXyz1;
    cXyz cStack_b0;

    s16 latDist;
    switch (movemode) {
    case 0: {
        field_0x7a4 = 0.0f;
        field_0x7a8 = 0;
        field_0x768 = 0.0f;
        field_0x789 = 1;

        unkXyz1 = home.pos - current.pos;
        latDist = unkXyz1.atan2sX_Z();
        cMtx_YrotS(*calc_mtx, latDist + 0x8000);

        unkXyz1.x = 0.0f;
        unkXyz1.y = 100.0f;
        unkXyz1.z = 200.0f;
        MtxPosition(&unkXyz1, &cStack_b0);
        unkXyz1.set(current.pos);
        unkXyz1.y += field_0x7a4;
        cStack_b0 += current.pos;
        cStack_b0.y += field_0x7a4;
        field_0x720.set(cStack_b0);

        dStack_98.Set(&unkXyz1, &cStack_b0, this);
        if (dComIfG_Bgsp().LineCross(&dStack_98)) {
            current.angle.y = latDist;
            speedF = 10.0f;
        }

        if (daPy_getPlayerActorClass()->checkWolfLock(this)) {
            daPy_getPlayerActorClass()->cancelWolfLock(this);
        }
        onWolfNoLock();

        field_0x78a = 0;
        speed.y = NREG_F(8) + 20.0f;
        setBck(8, 0, 3.0f, 1.0f);

        mSound1.startCreatureVoice(Z2SE_EN_HP_V_DOWN, -1);

        movemode = 1;
        break;
    }
    case 1: {
        if (field_0x780 == 9 && mpMorfSO->isStop()) {
            field_0x7a0 = 0.0f;
            field_0x71d = 7 + BREG_S(7);
            field_0x71c = (u8)l_HIO.resurrectionTime;
            setBck(15, 2, 3.0f, 1.0f);
            movemode = 10;
        } else if (field_0x780 == 8 && mpMorfSO->isStop()) {
            setBck(9, 0, 3.0f, 1.0f);
        }
        break;
    }
    case 10: {
        onDownFlg();
        if (checkWolfDownPullFlg()) {
            setBck(7, 0, 3.0f, 1.0f);
            mSound1.startCreatureVoice(Z2SE_EN_HP_V_DEAD, -1);
            field_0x71c = 0x1f;
            offDownFlg();
            setActionMode(6, 0);
        } else {
            if (field_0x71c == 0) {
                offDownFlg();
                setBck(0xb, 0, 3.0f, 1.0f);
                mSound1.startCreatureVoice(Z2SE_EN_HP_V_RETURN, -1);
                mStts.SetWeight(100);
                movemode = this->movemode + 1;
            }
        }
        break;
    }
    case 11: {
        cLib_addCalcAngleS2(&field_0x772.x, 0, 4, 0x100);
        cLib_addCalcAngleS2(&field_0x772.z, 0, 4, 0x100);
        if ((s32)mpMorfSO->getFrame() >= 16 && (s32)mpMorfSO->getFrame() <= 23) {
            field_0x7a4 += (170.0f + JREG_F(0)) / 7.0f * (2.0f + JREG_F(3));
            if (field_0x7a4 > 170.0f + JREG_F(0)) {
                field_0x7a4 = 170.0f + JREG_F(0);
            }
            cLib_addCalcAngleS2(&field_0x76c.z, -0x4000 + JREG_S(0), 1, 0x1000);
        } else {
            if ((s32)mpMorfSO->getFrame() >= 28) {
                field_0x7a8++;
                cLib_addCalcAngleS2(&field_0x76c.z, 0, 4, 0x400);
            }
        }

        cLib_addCalc2(&field_0x768,
                      field_0x7a4 +
                          (20.0f + NREG_F(4)) * (cM_ssin((f32)field_0x7a8 * (1000.0f + NREG_F(3)))),
                      0.8f, 10.0f);
        if (mpMorfSO->isStop()) {
            health = 40;

            offWolfNoLock();

            field_0x772.x = 0;
            field_0x772.z = 0;

            unkXyz1 = home.pos - current.pos;
            latDist = unkXyz1.atan2sX_Z();
            cMtx_YrotS(*calc_mtx, latDist);

            unkXyz1.x = 0.0f;
            unkXyz1.y = 100.0f;
            unkXyz1.z = 2000.0f;
            MtxPosition(&unkXyz1, &cStack_b0);

            unkXyz1.set(current.pos);
            unkXyz1.y += field_0x7a4;
            cStack_b0 += current.pos;
            cStack_b0.y += field_0x7a4;
            field_0x720.set(cStack_b0);
            dStack_98.OnBackFlag();
            dStack_98.Set(&unkXyz1, &cStack_b0, this);

            if (dComIfG_Bgsp().LineCross(&dStack_98)) {
                setActionMode(2, 0);
            } else {
                setActionMode(1, 0);
            }
        }
    }
    }

    if (movemode <= 2) {
        cLib_addCalcAngleS2(&(field_0x76c).z, -0x4a38, 2, 0x800);
    }
}

// NONMATCHING - g_dComIfG_gameInfo loading
void daE_HP_c::executeDead() {
    if (field_0x78d == 0 && field_0x71c == 1) {
        mSound1.startCreatureSound(Z2SE_EN_PO_SOUL_PULLOUT, 0, -1);
        mSound1.startCollisionSE(0x40003, 0x20);
        field_0x78d = 1;
    }

    switch (movemode) {
    case 0: {
        field_0x789 = 0x01;
        if (field_0x71c > 1) {
            return;
        }

        if (bitSw != 0xff) {
            fopAcM_onSwitch(this, bitSw);
        }

        dComIfGs_addPohSpiritNum();

        field_0x784 = -1;

        if (dComIfGs_getPohSpiritNum() == 20) {
            dComIfGs_onEventBit((u16)dSv_event_flag_c::saveBitLabels[0x1c9]);
        }

        movemode++;
    }
    case 1: {
#if VERSION != VERSION_SHIELD_DEBUG
        // TODO: gameInfo fake match to force reuse of pointer
        dComIfG_play_c* play = &g_dComIfG_gameInfo.play;
        if (play->getEvent().runCheck())
#else
        if (dComIfGp_event_runCheck())
#endif
        {
            if (eventInfo.checkCommandDemoAccrpt()) {
                if (dComIfGp_getEventManager().endCheck(field_0x778)) {
#if VERSION != VERSION_SHIELD_DEBUG
                    play->getEvent().reset();
#else
                    dComIfGp_event_reset();
#endif
                } else if (strcmp(dComIfGp_getEventManager().getRunEventName(),
                                  "DEFAULT_GETITEM") == 0 &&
                           field_0x784 != -1)
                {
                    dComIfGp_event_setTalkPartner(NULL);
                    dComIfGp_event_setItemPartnerId(field_0x784);

                    field_0x784 = -1;
                    field_0x788 = 1;
                }
            }
        } else if (field_0x788 != 0) {
            fopAcM_createDisappear(this, &current.pos, 8, 3, 0xff);
            fopAcM_delete(this);
        } else {
            if (field_0x784 == -1) {
                field_0x784 = fopAcM_createItemForPresentDemo(&current.pos, fpcNm_ITEM_POU_SPIRIT, 0, -1,
                                                              -1, 0, 0);
            }

            if (fopAcM_IsExecuting(field_0x784) != FALSE) {
                fopAc_ac_c* player = dComIfGp_getPlayer(0);
                if (player->eventInfo.chkCondition(8)) {
                    field_0x778 =
                        dComIfGp_getEventManager().getEventIdx(this, "DEFAULT_GETITEM", 0xff);
                    fopAcM_orderOtherEventId(this, field_0x778, 0xff, 0xffff, 6, 1);
                }
            }
        }

        break;
    }
    }
}

void daE_HP_c::action() {
    s16 angleDiff = 0;
    s32 unkFlag1 = 0;

    if (field_0x78b != 0) {
        field_0x78b = 0;
        mSound1.setEnemyName("E_hp");
    }

    damage_check();

    switch (mAction) {
    case 0:
        executeWait();
        break;
    case 1:
        executeMove();
        unkFlag1 = 1;
        break;
    case 2:
        executeRetMove();
        break;
    case 3:
        executeAttack();
        unkFlag1 = 1;
        break;
    case 4:
        executeDamage();
        break;
    case 5:
        executeDown();
        break;
    case 6:
        executeDead();
        break;
    }

    if (checkBallModelDraw()) {
        mSound2.startCreatureSoundLevel(Z2SE_EN_PO_SOUL, 0, -1);
    }

    if (field_0x780 == 12 || field_0x780 == 13 || field_0x780 == 14) {
        mSound1.startCreatureSoundLevel(Z2SE_EN_HP_MOVE, 0, -1);
    }

    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    if (mObjAcch.GetGroundH() != -G_CM3D_F_INF) {
        if (arg0 != 2 || mAction == 5 || mAction == 6) {
            gravity = -3.0f;
            if ((mAction == 5 && field_0x780 == 9 || mAction == 6) && mObjAcch.ChkGroundHit()) {
                dBgS_ObjLinChk linChk;
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(mObjAcch.m_gnd, &plane);

                cXyz* planeNormal = plane.GetNP();
                mStts.SetWeight(250);

                speedF = 0.0f;

                if (field_0x780 == 9 && field_0x78a == 0) {
                    Z2GetAudioMgr()->seStart(0x70523, &field_0x744, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                             0);
                    field_0x78a = 0x01;
                }

                field_0x772.x = -cM_atan2s(planeNormal->z, planeNormal->y);
                field_0x772.z = cM_atan2s(planeNormal->x, planeNormal->y);
            }
        } else if (mAction != 1 && mAction != 3) {
            gravity = 0.0f;
            cLib_addCalc2(&current.pos.y, home.pos.y, 0.8f, 10.0f + BREG_F(17));
        } else {
            cLib_addCalc2(&current.pos.y, player->current.pos.y, 0.8f, 10.0f + BREG_F(17));
        }
    } else {
        gravity = 0.0f;
        speed.y = 0.0f;

        if (field_0x72c.y < home.pos.y) {
            cLib_addCalc2(&(field_0x72c).y, home.pos.y, 0.8f, 10.0f + BREG_F(17));
        }

        current.pos.y = (field_0x72c).y;
    }
    if (mAction != 5 && mAction != 6) {
        if (JREG_F(0)) {
            field_0x7a4 = 170.0f + JREG_F(0);
        }
        field_0x7a8++;
    }

    fopAcM_posMoveF(this, mStts.GetCCMoveP());

    cXyz unkXyz1;

    mObjAcch.SetWallNone();
    mAcchCir.SetWall(30.0f + BREG_F(1), 60.0f + BREG_F(2));

    field_0x768 =
        field_0x7a4 + (20.0f + NREG_F(4)) * cM_ssin((f32)field_0x7a8 * (1000.0f + NREG_F(3)));

    if (mObjAcch.GetGroundH() != -G_CM3D_F_INF) {
        field_0x72c.y = mObjAcch.GetGroundH();
    }

    mObjAcch.SetGroundUpY(BREG_F(5));
    mObjAcch.CrrPos(dComIfG_Bgsp());

    if (arg0 != 0x02 || mAction == 5 || mAction == 6) {
        if (field_0x72c.y + 5.0f + BREG_F(6) < current.pos.y) {
            current.pos.y = (field_0x72c).y;
        }
    }

    if (mAction != 3) {
        cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 8, 0x800);
    }

    if (daPy_py_c::checkNowWolfPowerUp() || mAction == 5) {
        if (field_0x789 == 0) {
            unkXyz1 = home.pos - player->current.pos;
            if (unkXyz1.abs() < mDisHani) {
                setActionMode(0, 10);
                field_0x789 = 1;
            }
        }
        field_0x794 = 255.0f;
    } else {
        field_0x794 = 0.0f;
    }

    cLib_addCalc2(&field_0x790, field_0x794, 1.0f, 60.0f);

    if (BREG_S(9) != 0) {
        OS_REPORT("mAction %d\n", mAction);
        OS_REPORT("movemode %d\n", movemode);
    }

    if (BREG_S(8) != 0) {
        field_0x790 = 255.0f;
    }

    if (field_0x790 > 250.0f) {
        unkXyz1 = home.pos - player->current.pos;
        fopAcM_OnStatus(this, 0);
        attention_info.flags |= fopAc_AttnFlag_BATTLE_e;

        if (unkXyz1.abs() < mDisHani) {
            if (abs((s16)(current.angle.y - fopAcM_searchPlayerAngleY(this))) < 0x4000 &&
                (mAction == 0 || mAction == 1 || mAction == 3 && movemode == 2))
            {
                angleDiff = -(shape_angle.y - fopAcM_searchPlayerAngleY(this));
                if (angleDiff < -10000) {
                    angleDiff = -10000;
                } else if (angleDiff > 10000) {
                    angleDiff = 10000;
                }
            }
        }
    } else {
        fopAcM_OffStatus(this, 0);
        attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
    }

    cLib_addCalcAngleS2(&field_0x7aa, angleDiff, 8, 0x400);

    mpMorfSO->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mpMorf->play(NULL, 0, 0);

    setBallModelEffect(&tevStr);

    mSound1.setLinkSearch(unkFlag1);
}

void daE_HP_c::mtx_set() {
    J3DModel* model = mpMorfSO->getModel();

    cXyz unkXyz1;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(0.0f, field_0x768, 0.0f);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::ZXYrotM(field_0x772);
    mDoMtx_stack_c::scaleM(l_HIO.modelSize, l_HIO.modelSize, l_HIO.modelSize);
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    mpMorfSO->modelCalc();
    field_0x750.set(field_0x744);

    mDoMtx_stack_c::copy(model->getAnmMtx(13));
    unkXyz1.set(12.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&unkXyz1, &field_0x744);
    mDoMtx_stack_c::transS(field_0x744);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(l_HIO.modelSize, l_HIO.modelSize, l_HIO.modelSize);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    if (mAction != 5 && mAction != 6) {
        unkXyz1 = field_0x750 - field_0x744;
        f32 unkFloat1 = (400.0f + BREG_F(10)) * unkXyz1.abs();
        unkFloat1 += 1000.0f + BREG_F(11);
        if (unkFloat1 > 6000.0f + BREG_F(9)) {
            unkFloat1 = 6000.0f + BREG_F(9);
        }

        cLib_addCalc2(&field_0x798, unkFloat1, 0.1f, unkFloat1 * 0.1f);
        cLib_addCalcAngleS2(&field_0x76c.x,
                            field_0x798 * cM_ssin((f32)field_0x7a8 * (2000.0f + BREG_F(12))), 8,
                            0x400);

        field_0x76c.z = field_0x798 * cM_ssin((f32)field_0x7a8 * (2500.0f + BREG_F(13)));
    } else {
        field_0x798 = 0.0f;
        field_0x76c.x = 0;
    }

    if (mAction == 3 && field_0x71c == 0 && daPy_py_c::checkNowWolfPowerUp()) {
        field_0xddc = dComIfGp_particle_set(field_0xddc, 0x878b, &field_0xde0, &shape_angle, 0);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0xddc);
        if (emitter != NULL) {
            J3DModel* model = mpMorfSO->getModel();
            emitter->setGlobalSRTMatrix(model->getAnmMtx(7));
        }
    }
}

void daE_HP_c::cc_set() {
    cXyz cStack_1c;
    cXyz cStack_28;

    J3DModel* model = mpMorfSO->getModel();

    mDoMtx_stack_c::copy(model->getAnmMtx(8));
    mDoMtx_stack_c::multVecZero(&eyePos);
    attention_info.position = eyePos;
    attention_info.position.y += 25.0f;

    mDoMtx_stack_c::copy(model->getAnmMtx(0));
    cStack_1c.set(JREG_F(11), -40.0f + JREG_F(12), JREG_F(13));
    mDoMtx_stack_c::multVec(&cStack_1c, &cStack_28);
    mCyl1.SetC(cStack_28);
    mCyl1.SetH(170.0f + JREG_F(4));
    mCyl1.SetR(30.0f + JREG_F(5));
    dComIfG_Ccsp()->Set(&mCyl1);

    mDoMtx_stack_c::copy(model->getAnmMtx(10));
    cStack_1c.set(120.0f + JREG_F(6), 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&cStack_1c, &cStack_28);
    mCyl2.SetC(cStack_28);
    mCyl2.SetH(200.0f + JREG_F(8));
    mCyl2.SetR(200.0f + JREG_F(9));

    mDoMtx_stack_c::copy(model->getAnmMtx(7));
    mDoMtx_stack_c::multVecZero(&field_0xde0);

    if (field_0x780 == 5) {
        if (mpMorfSO->getFrame() >= 23.0f) {
            if (mpMorfSO->getFrame() <= 42.0f) {
                dComIfG_Ccsp()->Set(&mCyl2);
            }
        }
    }

    mSph.SetC(field_0x75c);
    mSph.SetR(30.0f + JREG_F(14));

    dComIfG_Ccsp()->Set(&mSph);
}

int daE_HP_c::execute() {
    if (field_0x77c != 0xff && !fopAcM_isSwitch(this, field_0x77c)) {
        attention_info.distances[fopAc_attn_BATTLE_e] = 0;

        fopAcM_SetGroup(this, 0);
        fopAcM_OffStatus(this, 0);

        attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;

        if (field_0x78b == 0) {
            field_0x790 = 0.0f;
            field_0x78b = 1;

            mSound1.setEnemyName(NULL);
        }

        return 1;
    }

    if (mNight == 1 && !dKy_daynight_check()) {
        attention_info.distances[fopAc_attn_BATTLE_e] = 0;

        fopAcM_SetGroup(this, 0);
        fopAcM_OffStatus(this, 0);

        attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;

        if (field_0x78b == 0) {
            field_0x790 = 0.0f;
            field_0x78b = 1;

            mSound1.setEnemyName(NULL);
        }

        return 1;
    }

    if (attention_info.distances[fopAc_attn_BATTLE_e] == 0) {
        attention_info.distances[fopAc_attn_BATTLE_e] = 0x04;

        fopAcM_SetGroup(this, 2);
        fopAcM_OnStatus(this, 0);

        attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
    }

    if (field_0x71c != 0) {
        field_0x71c--;
    }

    if (field_0x71e != 0) {
        field_0x71e--;
    }

    action();
    mtx_set();
    cc_set();

    return 1;
}

static int daE_HP_Execute(daE_HP_c* i_this) {
    return i_this->execute();
}

static int daE_HP_IsDelete(daE_HP_c* i_this) {
    return 1;
}

int daE_HP_c::_delete() {
    dComIfG_resDelete(&mPhaseReq, "E_HP");

    if (field_0xdf9 != 0) {
        l_HIOInit = false;
        mDoHIO_DELETE_CHILD(l_HIO.mChild);
    }

    if (heap != NULL) {
        mSound1.deleteObject();
        mSound2.deleteObject();
    }

    return 1;
}

static int daE_HP_Delete(daE_HP_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    return i_this->_delete();
}

int daE_HP_c::CreateHeap() {
    J3DModelData* modeldata = (J3DModelData*)dComIfG_getObjectRes("E_HP", 19);
    JUT_ASSERT(0x764, modeldata != NULL);
    mpMorfSO = new mDoExt_McaMorfSO(modeldata, NULL, NULL,
                                    (J3DAnmTransform*)dComIfG_getObjectRes("E_HP", 13), 2, 1.0f, 0,
                                    -1, &mSound1, 0x80000, 0x11000084);
    if (mpMorfSO == NULL || mpMorfSO->getModel() == NULL) {
        return 0;
    }

    if (mInvModel.create(mpMorfSO->getModel(), 1) == 0) {
        return 0;
    }

    model = mpMorfSO->getModel();
    model->setUserArea((uintptr_t)this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        if (i != 0) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
        }
    }

    modeldata = (J3DModelData*)dComIfG_getObjectRes("E_HP", 20);
    JUT_ASSERT(0x78b, modeldata != NULL);

    mpModel = mDoExt_J3DModel__create(modeldata, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    mpModel->setUserArea((uintptr_t)this);
    for (u16 i = 0; i < mpModel->getModelData()->getJointNum(); i++) {
        if (i != 0) {
            mpModel->getModelData()->getJointNodePointer(i)->setCallBack(LampJointCallBack);
        }
    }

    modeldata = (J3DModelData*)dComIfG_getObjectRes("E_HP", 18);
    mpMorf =
        new mDoExt_McaMorf(modeldata, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_HP", 4),
                           2, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11000084);

    if (mpMorf == NULL || mpMorf->getModel() == 0) {
        return 0;
    }

    if (initBallModel() == 0) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    daE_HP_c* poe = (daE_HP_c*)i_this;
    return poe->CreateHeap();
}

int daE_HP_c::create() {
    fopAcM_ct(this, daE_HP_c);

    s32 loadResult = dComIfG_resLoad(&mPhaseReq, "E_HP");
    if (loadResult == cPhs_COMPLEATE_e) {
        OS_REPORT("\n\n");
        OS_REPORT("E_HP PARAM %x\n", fopAcM_GetParam(this));

        arg0 = fopAcM_GetParam(this);
        bitSw = (fopAcM_GetParam(this) & 0xff00) >> 8;
        mDisHani = (f32)((fopAcM_GetParam(this) & 0xff0000) >> 0x10);
        field_0x77c = (fopAcM_GetParam(this) & 0xff000000) >> 24;

        mNight = current.angle.z & 0x1;

        if (BREG_S(0) == 0 && bitSw != 0xff && fopAcM_isSwitch(this, bitSw) != 0) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("arg0 %d\n", arg0);
        OS_REPORT("bitSw %d\n", bitSw);
        OS_REPORT("mDisHani %f\n", mDisHani);
        OS_REPORT("mNight %d\n", mNight);
        OS_REPORT("dKy_daynight_check() %d\n", dKy_daynight_check());
        OS_REPORT("\n\n");

        current.angle.z = 0;

        if (fopAcM_entrySolidHeap(this, useHeapInit, 0x3b30) == 0) {
            return cPhs_ERROR_e;
        }

        if (!l_HIOInit) {
            l_HIOInit = true;
            field_0xdf9 = 1;
            // "General-purpose Poe"
            l_HIO.mChild = mDoHIO_CREATE_CHILD("汎用ポゥ", &l_HIO);
        }
        if (mDisHani == 255.0f || mDisHani == 0.0f) {
            mDisHani = 2000.0f;
        } else {
            mDisHani *= 100.0f;
        }
        // "After change"
        OS_REPORT("変動後・mDisHani %f\n", mDisHani);

        if (BREG_S(2) != 0) {
            home.pos.y = current.pos.y + 100.0f;
            arg0 = 0x02;
        }
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        fopAcM_SetMtx(this, mpMorfSO->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);

        mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this,
                     1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
        mObjAcch.SetWallNone();
        mAcchCir.SetWall(BREG_F(1) + 30.0f, BREG_F(2) + 60.0f);

        mStts.Init(250, 0, this);
        field_0x560 = health = 40;
        field_0x7a4 = JREG_F(0) + 170.0f;
        field_0x72c.set(current.pos);
        field_0x75c.set(current.pos);

        mSph.Set(cc_lamp_src);
        mSph.SetStts(&mStts);
        mCyl1.Set(cc_hp_src);
        mCyl1.SetStts(&mStts);
        mCyl2.Set(cc_hp_at_src);
        mCyl2.SetStts(&mStts);

        mSound1.init(&current.pos, &eyePos, 3, 1);
        mSound2.init(&current.pos, &eyePos, 3, 1);

        if (field_0x77c != 0xff && !fopAcM_isSwitch(this, field_0x77c) ||
            mNight == 1 && !dKy_daynight_check())
        {
            field_0x78b = 1;
            mSound1.setEnemyName(NULL);
        } else {
            field_0x78b = 0;
            mSound1.setEnemyName("E_hp");
        }

        mAtInfo.mpSound = &mSound1;
        mAtInfo.mPowerType = 1;

        setActionMode(0, 0);

        shape_angle.x = shape_angle.z = 0;

        if (BREG_S(0) != 0 || mNight == 0 || dKy_daynight_check()) {
            attention_info.distances[fopAc_attn_BATTLE_e] = 0x04;
            fopAcM_OffStatus(this, 0);
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
        } else {
            attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            fopAcM_SetGroup(this, 0);
            fopAcM_OffStatus(this, 0);
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
        }

        mtx_set();
    }

    return loadResult;
}

daE_HP_c::daE_HP_c() {
}

static int daE_HP_Create(daE_HP_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daE_HP_Method = {
    (process_method_func)daE_HP_Create,  (process_method_func)daE_HP_Delete,
    (process_method_func)daE_HP_Execute, (process_method_func)daE_HP_IsDelete,
    (process_method_func)daE_HP_Draw,
};

extern actor_process_profile_definition g_profile_E_HP = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_E_HP,               // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daE_HP_c),        // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    172,                     // mPriority
    &l_daE_HP_Method,        // sub_method
    0x00040100,              // mStatus
    fopAc_ENEMY_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
