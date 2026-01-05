/**
 * @file d_a_e_hm.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_hm.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_enemy.h"
#include "Z2AudioLib/Z2Instances.h"

class daE_HM_HIO_c : public JORReflexible {
public:
    daE_HM_HIO_c();
    virtual ~daE_HM_HIO_c() {}

#if DEBUG
    void genMessage(JORMContext* ctx);
#endif

    s8 field_0x04;
    f32 searchArea;
    f32 flyDirSpeed;
    f32 flyDirYSpeed;
    f32 gravity;
    f32 attackRange;
    f32 galeStunTime;
};

daE_HM_HIO_c::daE_HM_HIO_c() {
     field_0x04 = -1;
     searchArea = 600.0f;
     flyDirSpeed = -16.0f;
     flyDirYSpeed = 44.0f;
     gravity = -9.0f;
     attackRange = 1.1f;
     galeStunTime = 0.0f;
}


#if DEBUG
void daE_HM_HIO_c::genMessage(JORMContext* ctx) {
    // "Himekuji"
    // appears to be a portmanteau - "ヒメ" = small (prefix), "ナメクジ" = slug)
    ctx->genLabel("ヒメクジ", 0x80000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("サーチエリア", &searchArea, 0.0f, 2000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("飛ばし方向スピード", &flyDirSpeed, 0.0f, -100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("飛ばし方向Yスピード", &flyDirYSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("重力設定", &gravity, 0.0f, -20.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("攻撃範囲", &attackRange, -1.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("疾風受け時間", &galeStunTime, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}
#endif

int daE_HM_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_HM", 17);
    JUT_ASSERT(0xbf, modelData != NULL);
    mAnm_p = new mDoExt_McaMorfSO(modelData, NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes("E_HM", 13), 2, 1.0f, 0, -1, &mSound,
        0x80000, 0x11000284);
    if (mAnm_p == NULL || mAnm_p->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mAnm_p->getModel();

    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL) {
        return 0;
    }
    if (!mpBrkAnm->init(model->getModelData(),
        (J3DAnmTevRegKey*)dComIfG_getObjectRes("E_HM", 20), 1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    mpBtkAnm = new mDoExt_btkAnm();
    if (mpBtkAnm == NULL) {
        return 0;
    }
    if (!mpBtkAnm->init(model->getModelData(),
        (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("E_HM", 23), 1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    daE_HM_c* hm = (daE_HM_c*)i_this;
    return hm->CreateHeap();
}

void daE_HM_c::initCcCylinder() {
    static const dCcD_SrcSph ccShpSrc = {
        {
            {0x0, {{0x2, 0x1, 0xd}, {(s32)0xD8FBFDFF, 0x43}, 0x75}},  // mObj
            {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0},                  // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                   // mGObjTg
            {0x0},                                                // mGObjCo
        },                                                        // mObjInf
        {
                {{0.0f, 0.0f, 0.0f}, 50.0f}  // mSph
        }  // mSphAttr
    };

    if (field_0x5d2 == 2) {
        mStts.Init(0x68,0,this);
    } else {
        mStts.Init(0xfe,0,this);
    }

    mSph.Set(ccShpSrc);
    mSph.SetStts(&mStts);
    mSph.SetC(current.pos);
    mSph.SetAtMtrl(1);
}

void daE_HM_c::setCcCylinder() {
    cXyz unkXyz1;
    switch (field_0x5d2) {
    case 1:
        unkXyz1.set(current.pos.x, current.pos.y + 50.0f, current.pos.z);
        mSph.SetC(unkXyz1);
        mSph.SetR(field_0x5b0);
        dComIfG_Ccsp()->Set(&mSph);
        break;
    case 2:
        mSph.SetC(current.pos);
        mSph.SetR(field_0x5b0);
        dComIfG_Ccsp()->Set(&mSph);
        break;
    case 3:
        unkXyz1.set(current.pos.x, current.pos.y, current.pos.z);
        if (field_0x5d4 == 3) {
            unkXyz1.y += 50.0f;
        }
        mSph.SetC(unkXyz1);
        mSph.SetR(field_0x5b0);
        dComIfG_Ccsp()->Set(&mSph);
        break;
    }
}

static int other_bg_check(daE_HM_c* i_this, fopAc_ac_c* i_other) {
    daE_HM_c* hm = (daE_HM_c*)i_this;

    dBgS_LinChk linChk;

    cXyz unusedXyz1;
    cXyz curPosWithOff;
    cXyz otherPosWithOff;
    otherPosWithOff = i_other->current.pos;
    otherPosWithOff.y += 100.0f;
    curPosWithOff = hm->current.pos;
    curPosWithOff.y += 100.0f;

    linChk.Set(&curPosWithOff, &otherPosWithOff, hm);

    if (dComIfG_Bgsp().LineCross(&linChk)) {
        return 1;
    } else {
        return 0;
    }
}

int daE_HM_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* joint = i_joint;
    u32 jointNo = joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jointNo));

    if (field_0x5d2 == 1) {
        switch (jointNo) {
        case 1:
            mDoMtx_stack_c::YrotM(((f32)field_0x5c0 * ((f32)(s32)nREG_S(1) + 6.0f)));
            break;
        case 4:
            mDoMtx_stack_c::YrotM(((f32)field_0x5c0 * ((f32)(s32)nREG_S(2) + 6.0f)));
            break;
        case 5:
            mDoMtx_stack_c::YrotM(((f32)field_0x5c0 * ((f32)(s32)nREG_S(3) + -8.0f)));
            break;
        case 6:
            mDoMtx_stack_c::YrotM(((f32)field_0x5c0 * ((f32)(s32)nREG_S(4) + -9.0f)));
            break;
        case 7:
            mDoMtx_stack_c::YrotM(
                ((f32)(s32)field_0x5c0 * ((f32)(s32)nREG_S(5) + -13.0f)));
            break;
        case 8:
            mDoMtx_stack_c::YrotM(
                ((f32)(s32)field_0x5c0 * ((f32)(s32)nREG_S(6) + -14.0f)));
        }
    } else if (field_0x5d2 == 2) {
        switch (jointNo) {
        case 1:
            mDoMtx_stack_c::YrotM(((f32)(s32)field_0x5c0 * ((f32)(s32)nREG_S(1) + 6.0f)));
            break;
        case 4:
            mDoMtx_stack_c::YrotM(((f32)(s32)field_0x5c0 * ((f32)(s32)nREG_S(2) + 6.0f)));
            break;
        case 5:
            mDoMtx_stack_c::YrotM(((f32)(s32)field_0x5c0 * ((f32)(s32)nREG_S(3) + -4.0f)));
            break;
        case 6:
            mDoMtx_stack_c::YrotM(((f32)(s32)field_0x5c0 * ((f32)(s32)nREG_S(4) + -5.0f)));
            break;
        case 7:
            mDoMtx_stack_c::YrotM(((f32)(s32)field_0x5c0 * ((f32)(s32)nREG_S(5) + -11.0f)));
            break;
        case 8:
            mDoMtx_stack_c::YrotM(((f32)(s32)field_0x5c0 * ((f32)(s32)nREG_S(6) + -12.0f)));
        }
    }

    i_model->setAnmMtx(jointNo, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

static int JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daE_HM_c* hm = (daE_HM_c*)model->getUserArea();
        if (hm != NULL) {
            hm->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daE_HM_c::Particle_Set(u32& param_0, u16 param_1) {
     param_0 = dComIfGp_particle_set(param_0, param_1, &current.pos, &tevStr, &shape_angle,
                                       &field_0x5e8, 0xff, 0, -1, 0, 0, 0);

    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(param_0);
    if (emitter != NULL) {
        emitter->setGlobalSRTMatrix(mAnm_p->getModel()->getAnmMtx(4));
    }
}

void daE_HM_c::SetAnm(int param_0, int param_1, f32 param_2, f32 param_3) {
    mAnm_p->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_HM", param_0), param_1, param_2,
                   param_3, 0.0f, -1.0f);
    field_0x61c = param_0;
}

s16 daE_HM_c::W_TargetAngle(cXyz param_0, cXyz param_1) {
     cXyz local_14;
     cXyz angleDiff = param_1 - param_0;
     mDoMtx_stack_c::ZrotS(-field_0x5cc.z);
     mDoMtx_stack_c::XrotM(-field_0x5cc.x);
     mDoMtx_stack_c::YrotM(-field_0x5cc.y);
     mDoMtx_stack_c::multVec(&angleDiff, &local_14);
     return cM_atan2s(local_14.x, local_14.z);
}

void daE_HM_c::W_DeathSpSet() {
    cXyz unkXyz1;

    daPy_py_c* player = daPy_getPlayerActorClass();

    cXyz& playerPos = fopAcM_GetPosition(player);
    s16 targetYaw = W_TargetAngle(current.pos, playerPos);
    mDoMtx_stack_c::ZXYrotS(field_0x5cc);
    mDoMtx_stack_c::YrotM(targetYaw);

    if (field_0x5af == 0) {
        unkXyz1.set(0.0f, KREG_F(5) + 20.0f, KREG_F(6) + -20.0f);
    } else {
        unkXyz1.set(0.0f, KREG_F(5), KREG_F(6) + -20.0f);
    }

    mDoMtx_stack_c::multVec(&unkXyz1, &field_0x5dc);

    mSph.OffAtSetBit();
    mSph.OffTgSetBit();

    SetAnm(7, 2, kREG_F(10) + 10.0f, nREG_F(12) + 1.0f);

    attention_info.flags = 0;
}

static f32 dummy0() {
     return 2.0f;
}

static f32 dummy1() {
    return 0.8f;
}

void daE_HM_c::W_DeathAction() {
    f32 frame = mAnm_p->getFrame();
    cXyz unusedXyz1;
    cXyz unusedXyz2;

    f32 unkFloat1 = field_0x5da + 1;
    cLib_chaseAngleS(&shape_angle.z, 0, unkFloat1 * 768.0f);
    cLib_chaseAngleS(&shape_angle.x, 0, unkFloat1 * 768.0f);
    cLib_chaseAngleS(&field_0x5cc.x, 0, 0x300);
    cLib_chaseAngleS(&field_0x5cc.z, 0, 0x300);

    if (mAnm_p->getFrame() > 30.0f) {
        field_0x5e8.x *= 0.8f;
        field_0x5e8.y *= 0.8f;
        field_0x5e8.z *= 0.8f;
        field_0x60c = 1;
        cLib_chaseF(&field_0x5f8, 0.0f, 0.04f);
    }

    if (field_0x5da > 1) {
        field_0x5dc.set(0.0f, 0.0f, 0.0f);
        if (mAnm_p->isStop()) {
            fopAcM_createDisappear(this, &current.pos, 10, 0, 0x23);
            fopAcM_delete(this);
        }
    } else {
        field_0x5dc.y += -(kREG_F(5) + 6.0f);
        if (mAcch.ChkGroundHit()) {
            if (field_0x5dc.y < 0.0f) {
                field_0x5da++;
            }
            field_0x5dc.y *= -0.2f;
        } else if (mAcch.ChkWallHit() && field_0x5dc.y < 0.0f) {
        }
        current.pos += field_0x5dc;
    }

    if (field_0x5da == 1) {
        if (mAnm_p->getFrame() > 15.0f) {
            mAnm_p->setFrame(15.0f);
        }
        if (field_0x61c != 8) {
            mAnm_p->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_HM", 8), 0, 5.0f, 1.0f, 0.0f, -1.0f);
            field_0x61c = 8;
        }
    }
}

void daE_HM_c::W_DeathMotion() {
    switch (field_0x5d6) {
    case 0:
        if (field_0x5ae != 0xff) {
            if (!dComIfGs_isSwitch(field_0x5ae, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(field_0x5ae, fopAcM_GetRoomNo(this));
            }
        }

        mSound.startCreatureVoice(Z2SE_EN_HM_V_DIE, -1);

        mAcch.SetWaterNone();
        W_DeathSpSet();

        field_0x5d6++;

        dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_DEAD_A), &current.pos, &tevStr, &shape_angle, 0);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_DEAD_B), &current.pos, &tevStr, &shape_angle, 0);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_DEAD_C), &current.pos, &tevStr, &shape_angle, 0);

        mSph.OffAtSetBit();
        mSph.OffTgSetBit();

        break;
    case 1:
        mSph.OffAtSetBit();
        W_DeathAction();

        break;
    }
}

int daE_HM_c::W_MoveCheckWall() {
    s16 unkFloat1;
    s16 unkFloat2;

    cXyz unkXyz1(0.0f, -100.0f, speedF * 3.0f);
    cXyz unkXyz2(0.0f, 100.0f, 0.0f);

    dBgS_LinChk linChk;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x5cc);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&unkXyz1, &unkXyz1);
    mDoMtx_stack_c::multVec(&unkXyz2, &unkXyz2);

    cXyz unusedXyz1;

    linChk.Set(&unkXyz2, &unkXyz1, NULL);

    if (dComIfG_Bgsp().LineCross(&linChk)) {
        cM3dGPla plane1;
        dComIfG_Bgsp().GetTriPla(linChk, &plane1);

        if (!dComIfG_Bgsp().GetMagnetCode(linChk)) {
            field_0x5b4 += (s16)0x200;
            return 1;
        }

        cM3dGPla plane2;
        dComIfG_Bgsp().GetTriPla(linChk, &plane2);

        cXyz* normal = plane2.GetNP();
        f32 normalY = normal->y;

        current.pos = linChk.GetCross();
        cXyz unkXyz3(0.0f, 0.0f, 0.0f);
        cXyz unkXyz4(0.0f, normal->y, normal->z);
        f32 temp = unkXyz3.abs(unkXyz4);
        unkFloat1 = -cM_atan2s(normal->x, temp);
        unkFloat2 = cM_atan2s(normal->z, normal->y);
    }

    cLib_chaseAngleS(&field_0x5cc.z, unkFloat1, 0x150);
    cLib_chaseAngleS(&field_0x5cc.x, unkFloat2, 0x150);

    return 0;
}

int daE_HM_c::W_WallCheck() {
    cXyz unkXyz1(0.0f, -300.0f, 300.0f);
    cXyz unkXyz2(0.0f, 200.0f, 0.0f);

    dBgS_LinChk linChk;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x5cc);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&unkXyz1, &unkXyz1);
    mDoMtx_stack_c::multVec(&unkXyz2, &unkXyz2);

    cXyz unusedXyz1;
    linChk.Set(&unkXyz2, &unkXyz1, NULL);

    if (dComIfG_Bgsp().LineCross(&linChk)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(linChk, &plane);
        if (!dComIfG_Bgsp().GetMagnetCode(linChk)) {
            field_0x5b4 += (s16)0x200;
            return 1;
        }
    }

    return 0;
}

void daE_HM_c::WallExecute() {
     switch (field_0x5d4) {
     case 4:
        WaitAction();
        break;
     case 2:
         AttackAction();
        break;
    case 1:
        mSph.OffAtSetBit();
        W_DeathMotion();
        mAcch.CrrPos(dComIfG_Bgsp());
        break;
    case 0:
        ShieldMotion();
        break;
    case 5:
        ShippuAction();
        break;
    }
}

static f32 dummy2() {
     return -30.0f;
 }

void daE_HM_c::WallCreateExecute() {
    switch (field_0x5d6) {
    case 0:
        field_0x5ad = 1;
        SetAnm(14, 2, nREG_F(9) + 5.0f, nREG_F(12) + 1.0f);
        field_0x5f4 = 0.0f;
        field_0x5d6++;
        break;
    case 1:
        cLib_chaseF(&field_0x5f4, 1.0f, 0.05f);
        if (field_0x5f4 == 1.0f) {
            field_0x5d2 = 3;
            field_0x5d4 = 3;
            MemberClear();
        }
        break;
    }
}

void daE_HM_c::CreateExecute() {
    if (fopAcM_SearchByID(field_0x5c4) == NULL && field_0x5c4 != fpcM_ERROR_PROCESS_ID_e) {
        field_0x5c4 = fpcM_ERROR_PROCESS_ID_e;
        field_0x5b8 = 300;
    }
    if (field_0x5c4 == fpcM_ERROR_PROCESS_ID_e && field_0x5b8 == 0) {
        field_0x5c4 = fopAcM_createChild(PROC_E_HM, fopAcM_GetID(this), 0xfffff05, &current.pos,
                                         fopAcM_GetRoomNo(this), &shape_angle, NULL, -1, NULL);
    }
}

void daE_HM_c::Tyakuchi_Set() {
    cXyz unkXyz1(0.0f, 30.0f, 0.0f);
    cXyz unkXyz2;
    cXyz unkXyz3(0.0f, -30.0f, 0.0f);
    cXyz unkXyz4;

    dBgS_LinChk linChk;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x5cc);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&unkXyz1, &unkXyz2);
    mDoMtx_stack_c::multVec(&unkXyz3, &unkXyz4);

    linChk.Set(&unkXyz2, &unkXyz4, NULL);

    if (dComIfG_Bgsp().LineCross(&linChk)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(linChk, &plane);

        cXyz* normal = plane.GetNP();
        cXyz origin(0.0f, 0.0f, 0.0f);
        cXyz normalYZ(0.0f, normal->y, normal->z);

        f32 magYZ = origin.abs(normalYZ);
        field_0x5cc.z = -cM_atan2s(normal->x, magYZ);
        field_0x5cc.x = cM_atan2s(normal->z, normal->y);
    }
}

void daE_HM_c::UpDeathAction() {
    switch (field_0x5d6) {
    case 0:
        if (field_0x5ad == 0 && field_0x5ae != 0xff) {
            if (!dComIfGs_isSwitch(field_0x5ae, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(field_0x5ae, fopAcM_GetRoomNo(this));
            }
        }

        cLib_chaseAngleS(&field_0x5cc.x, 0, 0x1000);
        cLib_chaseAngleS(&field_0x5cc.y, 0, 0x1000);

        mSound.startCreatureVoice(Z2SE_EN_HM_V_DIE, -1);

        attention_info.flags = 0x0;
        mSph.OffAtSetBit();
        mSph.OffTgSetBit();
        mAcch.SetWaterNone();

        if (mAcch.ChkGroundHit()) {
            DeathSpSet();
        } else {
            gravity = -9.0f;
        }

        SetAnm(7, 2, kREG_F(10) + 10.0f, nREG_F(12) + 1.0f);

        dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_DEAD_A), &current.pos, &tevStr, &shape_angle, 0);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_DEAD_B), &current.pos, &tevStr, &shape_angle, 0);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_DEAD_C), &current.pos, &tevStr, &shape_angle, 0);

        field_0x5d6++;

        break;
    case 1:
        cLib_chaseAngleS(&field_0x5cc.x, 0, 0x1000);
        cLib_chaseAngleS(&field_0x5cc.y, 0, 0x1000);
        cLib_chaseAngleS(&shape_angle.x, 0, 0x500);
        cLib_chaseAngleS(&shape_angle.y, 0, 0x500);

        DeathMotion();

        break;
    }
}

static bool hio_set;

static daE_HM_HIO_c l_HIO;

void daE_HM_c::UpMoveAction() {
    switch (field_0x5d6) {
    case 0:
        gravity = -9.0f;

        cLib_chaseAngleS(&shape_angle.x, 0, 0x1000);
        cLib_chaseAngleS(&shape_angle.y, 0, 0x1000);
        cLib_chaseAngleS(&field_0x5cc.x, 0, 0x1000);
        cLib_chaseAngleS(&field_0x5cc.y, 0, 0x1000);

        SetAnm(9, 2, nREG_F(9) + 5.0f, nREG_F(12) + 1.0f);

        if (mAcch.ChkGroundHit()) {
            Tyakuchi_Set();

            mSound.startCreatureSound(Z2SE_EN_HM_LAND, 0, -1);

            SetAnm(10, 0, nREG_F(9) + 5.0f, nREG_F(12) + 1.0f);

            dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_LANDING_A), &current.pos,
                                  &tevStr, &shape_angle, 0);
            dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_LANDING_B), &current.pos,
                                  &tevStr, &shape_angle, 0);

            cXyz unkXyz1(l_HIO.attackRange + yREG_F(4), l_HIO.attackRange + yREG_F(4),
                       l_HIO.attackRange + yREG_F(4));
            dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_ATFIRE2_A), &current.pos,
                                  &tevStr, &field_0x5cc, &unkXyz1);
            dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_ATFIRE2_B), &current.pos,
                                  &tevStr, &field_0x5cc, &unkXyz1);

            field_0x5d6++;

            mSph.OnTgSetBit();
        }
        break;
    case 1:
        if (mAnm_p->isStop()) {
            field_0x5b0 = 50.0f;
            field_0x5d2 = 1;
            field_0x5d4 = 4;

            MemberClear();
        } else {
            if (mAnm_p->getFrame() > 10.0f) {
                cLib_chaseF(&field_0x5b0, 40.0f, l_HIO.attackRange * 5.0f + yREG_F(3));
            } else {
                cLib_chaseF(&field_0x5b0, l_HIO.attackRange * 100.0f + yREG_F(2),
                            l_HIO.attackRange * 5.0f + yREG_F(3));
            }
        }
        break;
    }
}

void daE_HM_c::UpWaitAction() {
    switch (field_0x5d6) {
    case 0:
        mSound.startCreatureSound(Z2SE_EN_HM_WAIT, 0, -1);
        SetAnm(13, 2, nREG_F(9) + 5.0f, nREG_F(12) + 1.0f);
        field_0x5d6++;
        break;
    case 1:
        if (mAnm_p->getFrame() == 0.0f) {
            mSound.startCreatureSound(Z2SE_EN_HM_WAIT, 0, -1);
        }

        if (fopAcM_searchPlayerDistanceXZ(this) < l_HIO.searchArea) {
            mSph.OffTgSetBit();
            mSound.startCreatureSound(Z2SE_EN_HM_FALL, 0, -1);
            field_0x5d4 = 3;
            MemberClear();
        }

        break;
    }
}

void daE_HM_c::UpExecute() {
    switch (field_0x5d4) {
    case 5:
        ShippuAction();
        break;
    case 4:
        UpWaitAction();
        break;
    case 3:
        UpMoveAction();
        break;
    case 0:
        field_0x5d6 = 1;
        UpMoveAction();
        break;
    case 1:
        UpDeathAction();
        break;
    }
}

int daE_HM_c::WallCheck() {
    cXyz unkXyz1(0.0f, 300.0f, 150.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x5cc);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&unkXyz1, &unkXyz1);
    fopAcM_gc_c::gndCheck(&unkXyz1);
    if (fopAcM_gc_c::getGroundY() - current.pos.y < -300.0f) {
        return 1;
    } else {
        return 0;
    }
}

int daE_HM_c::MoveCheck() {
    cXyz unkXyz1(0.0f, 300.0f, 0.0f);
    cXyz unkXyz2(0.0f, -300.0f, (HREG_F(1) + 0.5f) * speedF);
    dBgS_LinChk linChk;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x5cc);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&unkXyz1, &unkXyz1);
    mDoMtx_stack_c::multVec(&unkXyz2, &unkXyz2);
    linChk.Set(&unkXyz1, &unkXyz2, NULL);

    cXyz vecToHome;

    if (!dComIfG_Bgsp().LineCross(&linChk)) {
        vecToHome = home.pos - current.pos;
        vecToHome.x += cM_rndFX(nREG_F(10) + 100.0f);
        vecToHome.z += cM_rndFX(nREG_F(10) + 100.0f);
        field_0x5b4 = (s16)cM_atan2s(vecToHome.x, vecToHome.z);
        field_0x5b8 = (s32)(cM_rndFX(50.0f) + 200.0f);

        return 1;
    } else {
        s16 yaw;
        s16 pitch;
        {
            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(linChk, &plane);
            cXyz* normal = plane.GetNP();
            cXyz origin(0.0f, 0.0f, 0.0f);
            cXyz normalYZ(0.0f, normal->y, normal->z);
            f32 magYZ = origin.abs(normalYZ);
            pitch = -cM_atan2s(normal->x, magYZ);
            yaw = cM_atan2s(normal->z, normal->y);

            current.pos = linChk.GetCross();
        }

        cLib_chaseAngleS(&field_0x5cc.x, yaw, 0x150);
        cLib_chaseAngleS(&field_0x5cc.z, pitch, 0x150);

    }

    return 0;
}

void daE_HM_c::ShippuAction() {
    bool unkFlag1;

    switch (field_0x5d6) {
    case 0:
        mSph.OffAtSetBit();
        mSound.startCreatureVoice(Z2SE_EN_HM_V_SIPPU, -1);
        SetAnm(12, 0, nREG_F(9) + 5.0f, nREG_F(12) + 1.0f);
        mSph.OffAtSetBit();
        field_0x5d6++;
        field_0x60c = 1;
        break;
    case 1:
        unkFlag1 = false;
        if (mAnm_p->getFrame() < 70.0f) {
            cLib_chaseF(&field_0x5f8, yREG_F(18) + 0.2f, 0.04f);
            field_0x5b8 = (s32)l_HIO.galeStunTime;
        } else if (mAnm_p->getFrame() == 70.0f) {
            if (field_0x5b8 > 0) {
                mAnm_p->setFrame(69.0f);
            }
        } else {
            cLib_chaseF(&field_0x5f8, 1.0f, 0.15f);
            if (field_0x5b8 == 0) {
                mpBrkAnm->setFrame(yREG_F(19));
                unkFlag1 = true;
            }
        }

        if (mAnm_p->isStop() && (u8)unkFlag1 == true) {
            mSph.OnAtSetBit();
            field_0x5d4 = 4;
            MemberClear();
            field_0x60c = 0;
            field_0x604 = 0;
            mSph.OnAtSetBit();
        }
        break;
    }
}

void daE_HM_c::ShieldMotion() {
    cLib_addCalc2(&speedF, nREG_F(2), kREG_F(5) + 0.5f, nREG_F(5) + 30.0f);

    if (!WallCheck()) {
        if (field_0x5d2 == 2) {
            W_MoveCheckWall();
        } else {
            MoveCheck();
        }
    }

    if (mAnm_p->isStop()) {
        MemberClear();
        field_0x5d4 = 4;
        current.angle.y = shape_angle.y;
        speedF = 0.0f;
        speed.y = 0.0f;
    }
}

namespace {
static f32 s_dis;

static u8 s_gnd[4];

static cXyz s_up;

static cXyz s_down;

static cXyz* sLink_Pos;

static s16 s_TargetAngle;
}

void daE_HM_c::ShieldAction() {
    switch (field_0x5d6) {
    case 0:
        SetAnm(11, 0, nREG_F(9) + 5.0f, nREG_F(12) + 1.0f);
        mSound.startCreatureVoice(Z2SE_EN_HM_V_GUARD, -1);
        current.angle.y = s_TargetAngle;
        speedF = -15.0f;
        field_0x5d6 = field_0x5d6 + 1;
        break;
    case 1:
        ShieldMotion();
        break;
    }
}

void daE_HM_c::DeathSpSet() {
    current.angle.y = s_TargetAngle + 0x500;
    speedF = l_HIO.flyDirSpeed;
    speed.y = l_HIO.flyDirYSpeed;
    gravity = l_HIO.gravity;
}

void daE_HM_c::DeathMotion() {
    f32 frame = mAnm_p->getFrame();
    if (mAcch.ChkGroundHit() && (field_0x5da++, field_0x5da) == 1) {
        speed.y = yREG_F(11) + 35.0f;
        current.angle.y -= (s16)0x1000;
        SetAnm(8, 0, nREG_F(9) + 1.0f, nREG_F(12) + 1.0f);
        mAnm_p->setPlaySpeed(yREG_F(12) + 2.0f);
    }

    if (mAcch.ChkWallHit()) {
        speedF = speedF * -0.3;
    }

    if (field_0x5da > 1) {
        mAnm_p->setPlaySpeed(1.0f);
        speedF = 0.0f;
        speed.y = 0.0f;

        if (mAnm_p->getFrame() > 30.0f) {
            field_0x5e8 *= 0.8f;
            field_0x60c = 1;
            cLib_chaseF(&field_0x5f8, 0.0f, 0.04f);
        }

        if (mAnm_p->isStop()) {
            fopAcM_createDisappear(this, &current.pos, 10, 0, 0x23);
            fopAcM_delete(this);
        }
    }
}

void daE_HM_c::DeathCutDown() {
    speedF = 0.0f;
    speed.y = 0.0f;
    field_0x5e8 *= 0.8f;
    field_0x60c = 1;
    cLib_chaseF(&field_0x5f8, 0.0f, 0.04f);
    if (mAnm_p->isStop()) {
        fopAcM_createDisappear(this, &current.pos, 10, 0, 0x23);
        fopAcM_delete(this);
    }
}

void daE_HM_c::DeathAction() {
    switch (field_0x5d6) {
    case 0:
        attention_info.flags = 0x0;
        if (field_0x5ad == 0 && field_0x5ae != 0xff) {
            if (!dComIfGs_isSwitch(field_0x5ae, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(field_0x5ae, fopAcM_GetRoomNo(this));
            }
        }
        attention_info.flags = 0x0;

        mSound.startCreatureVoice(Z2SE_EN_HM_V_DIE, -1);

        SetAnm(7, 0, kREG_F(10) + 10.0f, nREG_F(12) + 1.0f);

        dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_DEAD_A), &current.pos, &tevStr, &shape_angle, 0);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_DEAD_B), &current.pos, &tevStr, &shape_angle, 0);
        dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_DEAD_C), &current.pos, &tevStr, &shape_angle, 0);

        DeathSpSet();

        field_0x5d6++;

        mSph.OffAtSetBit();
        mSph.OffTgSetBit();
        mSph.OffCoSetBit();

        if (field_0x60d != 0) {
            SetAnm(8, 0, nREG_F(9) + 1.0f, nREG_F(12) + 1.0f);
            mAnm_p->setFrame(yREG_F(10));
        }

        break;
    case 1:
        if (field_0x60d != 0) {
            DeathCutDown();
        } else {
            DeathMotion();
        }
        break;
    }
}

void daE_HM_c::AttackAfter() {
    field_0x5b0 = 50.0f;
    field_0x5bc--;
    if (field_0x5bc == 0) {
        MemberClear();
        field_0x5d4 = 4;
    }
}

void daE_HM_c::AttackMotion() {
    f32 frame = mAnm_p->getFrame();
    if (field_0x61c == 14) {
        field_0x5bc = (s16)(cM_rndFX(15.0f) + 40.0f);
        field_0x5d6 = 2;
    } else {
        if (mAnm_p->isStop() && field_0x61c == 6) {
            SetAnm(14, 2, nREG_F(9) + 5.0f, nREG_F(12) + 1.0f);
        } else {
            if (frame == 31.0f) {
                mSound.startCreatureVoice(Z2SE_EN_HM_V_ATTACK, -1);
                mSound.startCreatureSound(Z2SE_EN_HM_ATTACK, 0, -1);

                cXyz unkXyz1(l_HIO.attackRange + yREG_F(4), l_HIO.attackRange + yREG_F(4),
                                l_HIO.attackRange + yREG_F(4));
                dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_ATFIRE2_A), &current.pos, &tevStr, &field_0x5cc, &unkXyz1);
                dComIfGp_particle_set(dPa_RM(ID_ZI_S_HM_ATFIRE2_B), &current.pos, &tevStr, &field_0x5cc, &unkXyz1);
            } else if (frame > 32.0f) {
                speedF = 0.0f;
                cLib_chaseF(&field_0x5b0, l_HIO.attackRange * 100.0f + yREG_F(2),
                            l_HIO.attackRange * 5.0f + yREG_F(3));
            }
        }
    }
}

void daE_HM_c::AttackAction() {
    switch (field_0x5d6) {
    case 0:
        if (field_0x5c0 == 0) {
            SetAnm(6, 0, nREG_F(9) + 5.0f, nREG_F(12) + 1.0f);
            field_0x5d6++;
        }
        break;
    case 1:
        Particle_Set(field_0x600, 0x84f2);
        AttackMotion();
        break;
    case 2:
        AttackAfter();
        break;
    }
}

void daE_HM_c::FreeMove() {
    f32 frame = mAnm_p->getFrame();
    if (frame > yREG_F(1) + 21.0f) {
        cLib_addCalc2(&speedF, yREG_F(0) + 0.5f, 0.5f, 5.0f);
    } else {
        cLib_addCalc2(&speedF, 0.0f, 0.5f, 5.0f);
    }

    cXyz vecToHome;
    if (field_0x5d2 == 2) {
        if (!W_WallCheck()) {
            W_MoveCheckWall();
            if (field_0x5b8 == 0) {
                vecToHome = home.pos - current.pos;
                vecToHome.x += cM_rndFX(nREG_F(10) + 200.0f);
                vecToHome.z += cM_rndFX(nREG_F(10) + 200.0f);
                field_0x5b4 = (s16)cM_atan2s(vecToHome.x, vecToHome.z);
                field_0x5b8 = (s32)(cM_rndFX(50.0f) + 200.0f);
            }
        }
    } else {
        MoveCheck();
        if (field_0x5b8 == 0 || WallCheck()) {
            vecToHome = home.pos - current.pos;
            vecToHome.x += cM_rndFX(nREG_F(10) + 100.0f);
            vecToHome.z += cM_rndFX(nREG_F(10) + 100.0f);
            field_0x5b4 = cM_atan2s(vecToHome.x, vecToHome.z);
            field_0x5b8 = (s32)(cM_rndFX(50.0f) + 200.0f);
        }
    }
}

void daE_HM_c::LinkSearch() {
    field_0x5bc--;
    if (field_0x5bc < 0) {
        field_0x5bc = 0;
    }

    if (s_dis < 500.0f) {
        if (field_0x5bc == 0) {
            MemberClear();
            field_0x5d4 = 2;
        } else {
            FreeMove();
        }
    } else {
        FreeMove();
    }
}

void daE_HM_c::WaitAction() {
    switch (field_0x5d6) {
    case 0:
        mSound.setLinkSearch(false);
        SetAnm(14, 2, nREG_F(9) + 5.0f, nREG_F(12) + 1.0f);
        field_0x5d6 = 1;
        break;
    case 1:
        if (mAnm_p->checkFrame(0.0f)) {
            mSound.startCreatureSound(Z2SE_EN_HM_WALK, 0, -1);
        }
        LinkSearch();
        break;
    }
}

void daE_HM_c::MemberClear() {
    field_0x5d6 = 0;
}

void daE_HM_c::DownExecute() {
    switch (field_0x5d4) {
    case 5:
        ShippuAction();
        break;
    case 0:
        ShieldAction();
        break;
    case 1:
        mSph.OffAtSetBit();
        DeathAction();
        break;
    case 2:
        AttackAction();
        break;
    case 4:
        WaitAction();
        break;
    }
}

void daE_HM_c::At_Check() {
    mAtInfo.mpActor = at_power_check(&mAtInfo);
    if (mAtInfo.mpActor != NULL) {
        if (mAtInfo.mpCollider->ChkAtType(0x4000)) {
            mAtInfo.mHitStatus = 2;
        }

        if ((s16)mAtInfo.mAttackPower > 0) {
            health -= (s16)mAtInfo.mAttackPower;
        }

        s8 unkByte1 = 0;
        if (mAtInfo.mAttackPower != 0) {
            mAtInfo.mHitStatus = 2;
            unkByte1 = 5;
        } else {
            unkByte1 = 2;
        }

        dCcD_GObjInf* dObj = static_cast<dCcD_GObjInf*>(mAtInfo.mpCollider);
        u32 unkUint1 = dObj->GetAtSe();
        int unkInt1 = 30;
        if (mAtInfo.mHitStatus == 1) {
            unkInt1 = 31;
        } else if (mAtInfo.mHitStatus == 2) {
            unkInt1 = 32;
        }

        if (mAtInfo.mpSound != NULL) {
            if (mAtInfo.field_0x18 != 0) {
                mAtInfo.mpSound->startCollisionSE(dCcD_GObjInf::getHitSeID(unkUint1, 0), mAtInfo.field_0x18);
            } else {
                mAtInfo.mpSound->startCollisionSE(dCcD_GObjInf::getHitSeID(unkUint1, 0), unkInt1);
            }
        }

        if (mAtInfo.mHitType == 1 || mAtInfo.mpCollider->ChkAtType(8)) {
            if (mAtInfo.mpCollider->ChkAtType(8)) {
                unkByte1 = 4;
            }

            dScnPly_c::setPauseTimer(unkByte1);
        }
    }
}

void daE_HM_c::Obj_Damage() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (field_0x5c2 > 0) {
        mSph.ClrTgHit();
        mSph.ClrAtHit();
    } else {
        mStts.Move();

        if (mSph.ChkAtHit() && (field_0x5d2 == 1 || field_0x5d2 == 3)) {
            field_0x5c2 = 10;
            if (!player->checkPlayerGuard() && field_0x5b0 > 50.0f) {
                player->setThrowDamage(s_TargetAngle, 15.0f, 20.0f, 1, 0, 0);
            }
        }

        if (checkCutDownHitFlg()) {
            field_0x60d = 1;
            field_0x5d4 = 1;
            MemberClear();
        }

        if (mSph.ChkTgHit()) {
            mAtInfo.mpCollider = mSph.GetTgHitObj();
            cCcD_ObjHitInf* tgHitObj = mSph.GetTgHitObj();
            if (!tgHitObj->ChkAtType(16)) {
                At_Check();
            }

            field_0x5c2 = 10;

            MemberClear();

            if (tgHitObj->ChkAtType(16)) {
                field_0x5d4 = 0;
            } else if (tgHitObj->ChkAtType(0x10000)) {
                field_0x5d4 = 5;
            } else {
                if (tgHitObj->ChkAtType(0x4000)) {
                    mSound.startCollisionSE(0x40000, 31);
                    dComIfGp_setHitMark(3, this, mSph.GetTgHitPosP(), NULL, NULL, 0);
                }

                field_0x5d4 = 1;
            }
        }
    }
}

void daE_HM_c::ActionMode() {

    s16 angleY = current.angle.y;
    if (field_0x5d4 != 5) {
        mSound.startCreatureSoundLevel(Z2SE_EN_HM_FIRE, 0, -1);
    }
    if (field_0x5d4 != 1) {
        Obj_Damage();
    }

    Vec* ccMoveP;
    switch (field_0x5d2) {
    case 3:
        UpExecute();
        setCcCylinder();
        Yazirushi();
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mAcch.CrrPos(dComIfG_Bgsp());
        break;
    case 1:
        if (dComIfGp_getAttention()->LockonTruth()) {
            daE_HM_c* hm = (daE_HM_c*)dComIfGp_getAttention()->LockonTarget(0);
            if (hm == this) {
                onDownFlg();
                setStabPos();
                if (dComIfGp_checkPlayerStatus1(0, 0x400000) &&
                    (field_0x5d4 != 2 || !(mAnm_p->getFrame() > 33.0f)))
                {
                    mSph.OffAtSetBit();
                } else if (field_0x5d4 == 2 && mAnm_p->getFrame() > 33.0f) {
                    mSph.OnAtSetBit();
                }
            }
        } else {
            offDownFlg();
            if (field_0x5d4 != 5) {
                mSph.OnAtSetBit();
            }
        }
        DownExecute();
        if (mAnm_p->getFrame() > 21.0f) {
            cLib_chaseAngleS(&shape_angle.y, field_0x5b4, 0x300);
            cLib_chaseAngleS(&current.angle.y, field_0x5b4, 0x300);
        }
        if (mAcch.ChkWaterIn()) {
            field_0x620 = 1;
            if (field_0x620 == 1) {
                fopAcM_delete(this);
            }
        }
        setCcCylinder();
        ccMoveP = mStts.GetCCMoveP();
        if (ccMoveP != NULL) {
            current.pos += *ccMoveP;
        }
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mAcch.CrrPos(dComIfG_Bgsp());
        Yazirushi();
        break;
    case 2:
        WallExecute();
        if (mAnm_p->getFrame() > 21.0f) {
            cLib_chaseAngleS(&shape_angle.y, field_0x5b4, 0x300);
            cLib_chaseAngleS(&current.angle.y, field_0x5b4, 0x300);
        }
        setCcCylinder();
        if (field_0x5af != 0) {
            eyePos = current.pos;
            eyePos.y += 10.0f;
            attention_info.position = eyePos;
            attention_info.position.y += 10.0f;
        } else {
            eyePos = current.pos;
            eyePos.y += 40.0f;
            attention_info.position = eyePos;
            attention_info.position.y += 40.0f;
        }
        break;
    case 4:
        CreateExecute();
        break;
    case 5:
        WallCreateExecute();
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        setCcCylinder();
        break;
    }

    if (field_0x5d4 == 5 || field_0x5d4 == 1) {
        field_0x5fc = 0;
        field_0x608 = 0;
    } else {
        Particle_Set(field_0x5fc, 0x84f6);
        Particle_Set(field_0x608, 0x852a);
    }

    if (mAnm_p->getFrame() > 21.0f) {
        if (current.angle.y - angleY != 0) {
            cLib_chaseAngleS(&field_0x5c0, (s16)(current.angle.y - angleY), 0x50);
        } else {
            cLib_chaseAngleS(&field_0x5c0, (s16)(current.angle.y - angleY), 32);
        }
    }
}

void daE_HM_c::Yazirushi() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (!other_bg_check(this, player) && (field_0x5d4 != 1)) {
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        eyePos = current.pos;
        eyePos.y += 40.0f;
        attention_info.position = eyePos;
        attention_info.position.y += 40.0f;
    } else {
        attention_info.flags = 0x0;
    }
}

void daE_HM_c::setStabPos() {
    setDownPos(&current.pos);
}

int daE_HM_c::Execute() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    sLink_Pos = &fopAcM_GetPosition(player);
    s_dis = current.pos.abs(*sLink_Pos);
    s_TargetAngle = W_TargetAngle(current.pos, *sLink_Pos);

    field_0x5b8--;
    if (field_0x5b8 < 0) {
        field_0x5b8 = 0;
    }

    ActionMode();

    cXyz bindEffectSize(1.0f, 1.0f, 1.0f);

    u32 cutType = daPy_getPlayerActorClass()->getCutType();
    if (cutType == 0x36 && mSph.ChkAtSet() != 0) {
        mSph.OffAtSetBit();
        field_0x5ac = 1;
    } else if (field_0x5ac != 0) {
        field_0x5ac = 0;
        mSph.OnAtSetBit();
    }

    setMidnaBindEffect(this, &mSound, &current.pos, &bindEffectSize);

    mAnm_p->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (field_0x5c2 != 0) {
        field_0x5c2--;
    }

    mpBrkAnm->play();
    mpBtkAnm->play();

    setBaseMtx();

    return 1;
}

int daE_HM_c::Draw() {
    J3DModel* model = mAnm_p->getModel();

    if (field_0x5d2 != 4) {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);

        mpBtkAnm->entry(model->getModelData());

        if (field_0x60c == 0) {
            mpBrkAnm->entry(model->getModelData());
        }

        if (field_0x60c != 0) {
            J3DModelData* modelData = model->getModelData();
            J3DMaterial* matNode_p = modelData->getMaterialNodePointer(0);
            matNode_p->getTevKColor(0)->r = field_0x5f8 * 255.0f;
            matNode_p->getTevKColor(0)->g = field_0x5f8 * 100.0f;
            mpBrkAnm->remove(modelData);
        }
        mAnm_p->entryDL();
    }

    cXyz shadowPos;
    shadowPos.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    field_0x62c = dComIfGd_setShadow(field_0x62c, 1, model, &shadowPos, 400.0f, 80.0f, current.pos.y,
                                   mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f,
                                   dDlst_shadowControl_c::getSimpleTex());

    return 1;
}

int daE_HM_c::Delete() {
    dComIfG_resDelete(&mPhase,"E_HM");

    if (field_0xa84 != 0) {
        hio_set = false;
        mDoHIO_DELETE_CHILD(l_HIO.field_0x04);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }

    return 1;
}

void daE_HM_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x5cc);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mAnm_p->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mAnm_p->modelCalc();
}

static int daE_HM_Draw(daE_HM_c* i_this) {
    return i_this->Draw();
}

static int daE_HM_Execute(daE_HM_c* i_this) {
    return i_this->Execute();
}

static int daE_HM_IsDelete(daE_HM_c* i_this) {
    return TRUE;
}

static int daE_HM_Delete(daE_HM_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    return i_this->Delete();
}

void daE_HM_c::CheckRoof() {
    cXyz endPos(current.pos.x, current.pos.y + 300.0f, current.pos.z);
    dBgS_LinChk linChk;
    cXyz startPos(current.pos.x, current.pos.y - 200.0f, current.pos.z);
    linChk.Set(&startPos, &endPos, NULL);
    if (dComIfG_Bgsp().LineCross(&linChk)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(linChk, &plane);
        cXyz* normal = plane.GetNP();

        cXyz unusedXyz1(0.0f, 1.0f, 0.0f);
        cXyz unusedXyz2;
        cXyz origin(0.0f, 0.0f, 0.0f);

        cXyz normalYZ(0.0f, normal->y, normal->z);
        field_0x5cc.z = -cM_atan2s(normal->x, origin.abs(normalYZ));
        field_0x5cc.x = cM_atan2s(normal->z, normal->y);

        cXyz linChkCross = linChk.GetCross();
        current.pos = linChk.GetCross();
    }
}

void daE_HM_c::CheckWall() {
    cXyz unkXyz1;
    cXyz unkXyz2(0.0f, 0.0f, 200.0f);
    cXyz unkXyz3(0.0f, 200.0f, 0.0f);
    dBgS_LinChk linChk;
    cLib_offsetPos(&unkXyz1, &current.pos, shape_angle.y, &unkXyz3);
    linChk.Set(&current.pos, &unkXyz1, NULL);

    if (dComIfG_Bgsp().LineCross(&linChk)) {
        field_0x5af = 1;

        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(linChk, &plane);
        cXyz* normal = plane.GetNP();
        cXyz unusedXyz3(0.0f, 1.0f, 0.0f);
        cXyz unusedXyz4;
        cXyz origin(0.0f, 0.0f, 0.0f);
        cXyz normalYZ(0.0f, normal->y, normal->z);
        field_0x5cc.z = -cM_atan2s(normal->x, origin.abs(normalYZ));
        field_0x5cc.x = cM_atan2s(normal->z, normal->y);
        current.pos = linChk.GetCross();

        return;
    } else {
        unkXyz1.set(0.0f, 0.0f, 0.0f);

        for (int i = 0; i < 4; i++) {
            cLib_offsetPos(&unkXyz1, &current.pos, shape_angle.y + (i * 16384), &unkXyz2);
            linChk.Set(&current.pos, &unkXyz1, NULL);

            if (dComIfG_Bgsp().LineCross(&linChk)) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(linChk, &plane);
                cXyz* normal = plane.GetNP();
                cXyz unusedXyz1(0.0f, 1.0f, 0.0f);
                cXyz unusedXyz2;
                cXyz origin(0.0f, 0.0f, 0.0f);
                cXyz normalYZ(0.0f, normal->y, normal->z);
                field_0x5cc.z = -cM_atan2s(normal->x, origin.abs(normalYZ));
                field_0x5cc.x = cM_atan2s(normal->z, normal->y);
                current.pos = linChk.GetCross();

                return;
            }
        }
    }
}

void daE_HM_c::CreateStyle() {
    switch (field_0x5d2) {
    case 1:
        gravity = nREG_F(14) + -9.0f;
        mAcchCir.SetWall(10.0f, KREG_F(3) + 30.0f);
        mAcchCir.SetWallHDirect(50.0f);

        if (fopAcM_gc_c::gndCheck(&current.pos)) {
            current.pos.y = nREG_F(4) + fopAcM_gc_c::getGroundY();
        }

        field_0x5d4 = 4;
        field_0x5d6 = 0;
        MemberClear();
        break;
    case 2:
        gravity = 0.0f;
        mAcchCir.SetWall(0.0f, 60.0f);
        shape_angle.set(0, 0, 0);
        CheckWall();
        field_0x5d4 = 4;
        MemberClear();
        break;
    case 3:
        mSph.OffTgSetBit();
        gravity = 0.0f;
        mAcchCir.SetWall(10.0f, KREG_F(3) + 30.0f);
        mAcchCir.SetWallHDirect(50.0f);
        CheckRoof();
        field_0x5d4 = 4;
        MemberClear();
        break;
    case 4:
        field_0x5c4 = 0xffffffff;
        break;
    case 5:
        gravity = nREG_F(0);
        mAcchCir.SetWall(10.0f, 30.0f);
        CheckRoof();
        shape_angle.x = shape_angle.x + -0x4000;
        field_0x5d4 = 0;
        MemberClear();
        break;
    }
}

int daE_HM_c::Create() {
    fopAcM_ct(this, daE_HM_c);

    s32 loadResult = dComIfG_resLoad(&mPhase, "E_HM");
    if (loadResult == cPhs_COMPLEATE_e) {
        OS_REPORT("E_HM PARAM %x\n", fopAcM_GetParam(this));
        field_0x5ae = fopAcM_GetParam(this) >> 24;
        if (field_0x5ae != 0xff) {
            if (dComIfGs_isSwitch(field_0x5ae, fopAcM_GetRoomNo(this))) {
                return cPhs_ERROR_e;
            }
        }

        if (fopAcM_entrySolidHeap(this, useHeapInit, 0x15a0) == 0) {
            return cPhs_ERROR_e;
        }

        if (hio_set == 0) {
            hio_set = true;
            field_0xa84 = 1;
            l_HIO.field_0x04 = mDoHIO_CREATE_CHILD("E_HM", &l_HIO);
        }

        field_0x5cc.set(0, 0, 0);

        field_0x5d2 = (u8)fopAcM_GetParam(this);
        if (field_0x5d2 == 0xff) {
            field_0x5d2 = 1;
        }

        CreateStyle();

        attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -30.0f, 0.0f, -100.0f);
        fopAcM_SetMax(this, 30.0f, 50.0f, 100.0f);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                  &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);

        mSound.init(&current.pos, &eyePos, 3, 1);

        mAtInfo.mpSound = &mSound;
        mAtInfo.mPowerType = 1;
        mAtInfo.mpSound = &mSound;

        J3DModel* model = mAnm_p->getModel();
        model->setUserArea((u32)this);

        for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
            if (i != 0) {
                model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
            }
        }

        field_0x5e8.set(1.0f, 1.0f, 1.0f);
        field_0x5f8 = 1.0f;
        field_0x5b0 = 50.0f;

        initCcCylinder();

        mSound.startCreatureSound(Z2SE_EN_DF_WAIT, 0, -1);
        mSound.setEnemyName("E_HM");

        speed.y = 0.0f;

        daE_HM_Execute(this);
    }

    return loadResult;
}

static int daE_HM_Create(fopAc_ac_c* i_this) {
    daE_HM_c* hm = (daE_HM_c*)i_this;
    return hm->Create();
}

AUDIO_INSTANCES

static actor_method_class l_daE_HM_Method = {
    (process_method_func)daE_HM_Create,
    (process_method_func)daE_HM_Delete,
    (process_method_func)daE_HM_Execute,
    (process_method_func)daE_HM_IsDelete,
    (process_method_func)daE_HM_Draw,
};

actor_process_profile_definition g_profile_E_HM = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_E_HM,              // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daE_HM_c),       // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    146,                    // mPriority
    &l_daE_HM_Method,       // sub_method
    0x00040100,             // mStatus
    fopAc_ENEMY_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
