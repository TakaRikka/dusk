/**
 * @file d_a_e_fk.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_fk.h"
#include "d/d_s_play.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_horse.h"
#include "Z2AudioLib/Z2Instances.h"
#include "f_op/f_op_actor_enemy.h"

class daE_FK_HIO_c : public JORReflexible {
public:
    daE_FK_HIO_c();
    virtual ~daE_FK_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 model_size;
    /* 0x0C */ f32 alpha; // "α"
    /* 0x10 */ s16 effect_alpha; // "Effectα"
    /* 0x12 */ s16 generation_time;
};

daE_FK_HIO_c::daE_FK_HIO_c() {
    id = -1;
    model_size = 0.8f;
    alpha = 2.0f;
    effect_alpha = 20;
    generation_time = 750;
}

int daE_FK_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_fk", 15);
    JUT_ASSERT(189, modelData != NULL);
    mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_fk", 12), 2, 1.0f, 0, -1, &mSound, 0x80000, 0x11000084);
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    if (!mInvisibleModel.create(mpModelMorf->getModel(), 1)) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("E_fk", 16);
    mpEffModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpEffModel == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return ((daE_FK_c*)i_this)->CreateHeap();
}

void daE_FK_c::initBodySph() {
    const static dCcD_SrcSph ccBodySphSrc = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x4000000, 0x43}, 0x0}}, // mObj
            {dCcD_SE_NONE, 0x0, 0xd, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x1, 0x0, 0x0, 0x0}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 100.0f} // mSph
        } // mSphAttr
    };

    mCcStts.Init(125, 0xFF, this);
    for (int i = 0; i < 2; i++) {
        mCcBodySph[i].Set(ccBodySphSrc);
        mCcBodySph[i].SetStts(&mCcStts);
        mCcBodySph[i].SetC(current.pos);
    }
}

void daE_FK_c::initSwordSph() {
    const static dCcD_SrcSph ccSphSrc = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
            {dCcD_SE_SWORD, 0x0, 0xd, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x1}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 80.0f} // mSph
        } // mSphAttr
    };

    mCcStts.Init(254, 0xFF, this);
    for (int i = 0; i < 3; i++) {
        mCcSwordSph[i].Set(ccSphSrc);
        mCcSwordSph[i].SetStts(&mCcStts);
        mCcSwordSph[i].SetC(current.pos);
        mCcSwordSph[i].OffTgNoHitMark();
        mCcSwordSph[i].OffTgSetBit();
        mCcSwordSph[i].OffAtSetBit();
    }

    OnAt();
}

static u8 l_HIOInit;

static daE_FK_HIO_c l_HIO;

void daE_FK_c::SetBodySph() {
    cXyz pos(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(1));
    mDoMtx_stack_c::multVec(&pos, &pos);
    mCcBodySph[0].SetC(pos);
    mCcBodySph[0].SetR(400.0f * l_HIO.model_size);
    dComIfG_Ccsp()->Set(&mCcBodySph[0]);

    pos.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(3));
    mDoMtx_stack_c::multVec(&pos, &pos);
    mCcBodySph[1].SetC(pos);
    mCcBodySph[1].SetR(400.0f * l_HIO.model_size);
    dComIfG_Ccsp()->Set(&mCcBodySph[1]);
}

void daE_FK_c::SetSwordSph() {
    cXyz pos0(0.0f, 0.0f, 0.0f);
    cXyz pos1(0.0f, 100.0f, -500.0f);

    pos0.set(0.0f, 350.0f + BREG_F(5), -500.0f + BREG_F(4));
    mDoMtx_stack_c::YrotS(shape_angle.y);
    mDoMtx_stack_c::multVec(&pos0, &pos0);
    mDoMtx_stack_c::multVec(&pos1, &pos1);

    pos0 += current.pos;
    mCcSwordSph[0].SetC(pos0);
    mCcSwordSph[0].SetR((250.0f + BREG_F(0)) * l_HIO.model_size);
    dComIfG_Ccsp()->Set(&mCcSwordSph[0]);

    mCcSwordSph[1].SetC(pos1);
    mCcSwordSph[1].SetR((250.0f + BREG_F(0)) * l_HIO.model_size);
    dComIfG_Ccsp()->Set(&mCcSwordSph[1]);
}

int daE_FK_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jnt_no = i_joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));
    csXyz spC;

    switch (jnt_no) {
    case 1:
        mDoMtx_stack_c::YrotM(mHNeckRot);
        break;
    case 2:
        mDoMtx_stack_c::YrotM(0.3f * mHNeckRot);
        break;
    case 12:
        mDoMtx_stack_c::XrotM(mKNeckRot);
        break;
    }

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

static int JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model_p = j3dSys.getModel();
        daE_FK_c* a_this = (daE_FK_c*)model_p->getUserArea();

        if (a_this != NULL) {
            a_this->ctrlJoint(i_joint, model_p);
        }
    }

    return 1;
}

void daE_FK_c::SetAnm(int i_anm, int i_mode, f32 i_morf, f32 i_speed) {
    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("e_fk", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
    mAnm = i_anm;
}

static f32 dummyFloat3(int _) {
    switch (_) {
    case 0:
        return 30.0f;
    case 1:
        return 300.0f;
    }
}

void daE_FK_c::Yazirushi() {
    eyePos = current.pos;
    eyePos.y += 150.0f;

    attention_info.position = eyePos;
    attention_info.position.y += 150.0f;
}

void daE_FK_c::SpeedSet() {
    field_0x5d2 = current.angle.y;
    cLib_addCalc2(&speedF, mMaxSpeed, 0.1f, 5.0f);
    cLib_addCalcAngleS2(&current.angle.y, field_0x5d0, 2, 0x2000);
    shape_angle.y = current.angle.y;
    field_0x5dc = 1.0f - (5.0f / mMaxSpeed);
}

void daE_FK_c::TnNeckSet(s16 i_rot) {
    cLib_addCalcAngleS2(&mKNeckRot, i_rot, 5, 0x1000);

    if (mKNeckRot > 0x3000) {
        mKNeckRot = 0x3000;
    } else if (mKNeckRot < -0x3000) {
        mKNeckRot = -0x3000;
    }
}

void daE_FK_c::HsAngleSet() {}

void daE_FK_c::SpeedChk() {}

void daE_FK_c::SetAnmSpeed() {}

void daE_FK_c::TgChk() {
    for (int i = 0; i < 2; i++) {
        if (mCcBodySph[i].ChkTgHit()) {
            cCcD_Obj* hit_obj = mCcBodySph[i].GetTgHitObj();

            mAtInfo.field_0x18 = 30;
            mAtInfo.mpCollider = mCcBodySph[i].GetTgHitObj();
            At_Check(i);

            mCcTimer[0] = 15;
            mCcBodySph[i].ClrTgHit();
            mAction = 3;
            mMode = 0;

            mSound.startCreatureSound(Z2SE_EN_FK_DEATH, 0, -1);
        }
    }
}

void daE_FK_c::AtChk() {
    for (int i = 0; i < 2; i++) {
        if (mCcSwordSph[i].ChkAtHit()) {
            mCcTimer[1] = 30;
            mCcSwordSph[i].ClrAtHit();
        }
    }
}

void daE_FK_c::OnAt() {
    for (int i = 0; i < 2; i++) {
        mCcSwordSph[i].OnAtSetBit();
    }
}

static f32 dummyFloat4(int _) {
    switch (_) {
    case 0:
        return 2000.0f;
    case 1:
        return 1000.0f;
    case 2:
        return -4000.0f;
    }
}

namespace {
static f32 s_dis;

static cXyz* s_LinkPos;

static s16 s_TargetAngle;

static cXyz s_centerpos(217.0f, 1275.0f, -57.0f);

static cXyz s_TargetPos;

static s16 s_PointAngle;

static f32 s_TargetDis;
}

void daE_FK_c::R_MoveAction() {
    daPy_py_c* actor = daPy_getPlayerActorClass();
    cXyz pos(current.pos);

    field_0x5d0 = s_PointAngle;
    if (s_TargetDis < 4000.0f) {
        mAction = 2;
        mMode = 0;
    }

    SpeedChk();
    SpeedSet();
    SetAnmSpeed();
    HsAngleSet();
    TnNeckSet(fopAcM_searchPlayerAngleY(this) - shape_angle.y);
    TgChk();
}

static f32 dummyFloat5() {
    return -100.0f;
}

void daE_FK_c::StartAction() {
    if (cLib_chaseF(&mAlpha, 0.5f, 0.02f)) {
        mAction = 1;
    }

    f32 speedf = fopAcM_GetSpeedF(daPy_getPlayerActorClass());
    cXyz sp8;

    field_0x5dc = 1.0f;

    SpeedChk();
    SpeedSet();
    SetAnmSpeed();
    HsAngleSet();
    TnNeckSet(fopAcM_searchPlayerAngleY(this) - shape_angle.y);
    TgChk();
}

void daE_FK_c::RunAction() {
    switch (mAction) {
    case 1:
        R_MoveAction();
        cLib_chaseF(&mAlpha, 0.5f, 0.02f);
        cLib_addCalc2(&mSize, l_HIO.model_size, 0.2f, 0.5f);
        break;
    case 2:
        AttackAction();
        cLib_chaseF(&mAlpha, 0.5f, 0.02f);
        cLib_addCalc2(&mSize, l_HIO.model_size, 0.2f, 0.5f);
        break;
    case 3:
        DamageAction();
        break;
    case 4:
        DeathAction();
        break;
    case 5:
        StartAction();
        break;
    }
}

void daE_FK_c::Action() {
    RunAction();
}

void daE_FK_c::DeathAction() {
    switch (mMode) {
    case 0:
        if (cLib_chaseF(&mAlpha, 0.2f, 0.03f + yREG_F(0))) {
            fopAcM_createDisappear(this, &current.pos, 15, 3, field_0x564);
            fopAcM_delete(this);
        }

        mCcBodySph[0].OffTgSetBit();
        mCcBodySph[1].OffTgSetBit();

        if (mpModelMorf->isStop()) {
            SetAnm(9, 2, 5.0f, 1.0f);
            mMode++;
            mMaxSpeed = 0.0f;
            mTimers[0] = 0;
        }
        break;
    case 1:
        if (cLib_chaseF(&mAlpha, 0.2f, 0.05f + yREG_F(0))) {
            fopAcM_createDisappear(this, &current.pos, 15, 3, field_0x564);
            fopAcM_delete(this);
        }
        break;
    }

    SpeedChk();
    SpeedSet();
    SetAnmSpeed();
    HsAngleSet();
}

void daE_FK_c::DamageChk() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (health <= 0) {
        SetAnm(8, 0, 5.0f, 1.0f);
        mAction = 4;
        mMode = 0;
        attention_info.flags = 0;

        mCcBodySph[0].OffCoSetBit();
        mCcBodySph[1].OffCoSetBit();

        if ((fopAcM_GetShapeAngle_p(player)->y - s_TargetAngle) >= 0) {
            current.angle.y += 0x2000;
        } else {
            current.angle.y -= 0x2000;
        }
    }
}

void daE_FK_c::DamageAction() {
    switch (mMode) {
    case 0:
        DamageChk();
        break;
    case 1:
        if (mpModelMorf->isStop()) {
            mAction = 1;
            mMode = 0;
        }
        break;
    }

    SpeedChk();
    SpeedSet();
    SetAnmSpeed();
    HsAngleSet();
    TnNeckSet(fopAcM_searchPlayerAngleY(this) - shape_angle.y);
    TgChk();
}

bool daE_FK_c::checkViewArea() {
    Vec proj;
    mDoLib_project(&current.pos, &proj);
    return (proj.x >= 0.0f && proj.x <= FB_WIDTH) && (proj.y >= 0.0f && proj.y <= FB_HEIGHT);
}

namespace {
/* 806BB974 0002+00 data_806BB974 s_HorseAngle__22@unnamed@d_a_e_fk_cpp@ */
static s16 s_HorseAngle;

/* 806BB976 0002+00 data_806BB976 s_AttackChk__22@unnamed@d_a_e_fk_cpp@ */
static s16 s_AttackChk;
}

void daE_FK_c::AttackAction() {
    switch (mMode) {
    case 0:
        if (mAnm != 12) {
            SetAnm(12, 2, 5.0f, 1.0f);
        }

        mMode++;
        TnNeckSet(fopAcM_searchPlayerAngleY(this) - shape_angle.y);
        field_0x5d0 = s_PointAngle;

        if (s_TargetDis < 1000.0f + aREG_F(0)) {
            mMode = 1;
        }
        break;
    case 1:
        if (s_TargetDis < 1500.0f + aREG_F(0)) {
            mTimers[0] = 100;
            mMode++;
        }

        TnNeckSet(fopAcM_searchPlayerAngleY(this) - shape_angle.y);

        if (checkViewArea()) {
            field_0x5d0 = s_PointAngle;
        } else {
            field_0x5d0 = s_PointAngle;

            s16 temp_r0_2 = s_HorseAngle - current.angle.y;
            if ((temp_r0_2 < 0x4000 && temp_r0_2 > 0) || (temp_r0_2 > -0x4000 && temp_r0_2 < 0)) {
                mMode = 3;
            }
        }
        break;
    case 2:
        if (!checkViewArea()) {
            s16 temp_r0_3 = s_HorseAngle - current.angle.y;
            if ((temp_r0_3 < 0x4000 && temp_r0_3 > 0) || (temp_r0_3 > -0x4000 && temp_r0_3 < 0)) {
                mMode = 3;
            }
        }

        if (mTimers[0] == 0) {
            SetAnm(10, 2, 5.0f, 1.0f);
            mMode++;
        }

        TnNeckSet(0);
        break;
    case 3:
        if (cLib_chaseF(&mAlpha, 0.2f, 0.1f + yREG_F(0))) {
            fopAcM_delete(this);
        }

        TnNeckSet(0);
        break;
    }

    SpeedSet();
    HsAngleSet();
    TgChk();
    AtChk();
}

void daE_FK_c::UpDown() {
    switch (mUpDownMode) {
    case 0:
        if (mTimers[2] == 0) {
            mTimers[2] = 15;
            mUpDownMode++;
        }
        cLib_chaseF(&speed.y, 3.5f + yREG_F(3), 0.5f + yREG_F(5));
        break;
    case 1:
        if (mTimers[2] == 0) {
            mTimers[2] = 15;
            mUpDownMode--;
        }
        cLib_chaseF(&speed.y, -3.5f - yREG_F(3), 0.5f + yREG_F(5));
        break;
    }

    field_0x5e8 += speed.y;
    current.pos.y += field_0x5e8;
}

int daE_FK_c::Execute() {
    if (dComIfGp_event_runCheck()) {
        return 1;
    }

    mSound.startCreatureSoundLevel(Z2SE_EN_FK_MOVE, 0, -1);
    
    daPy_py_c* player = daPy_getPlayerActorClass();
    s_LinkPos = &fopAcM_GetPosition(player);
    s_TargetAngle = cLib_targetAngleY(&current.pos, s_LinkPos);
    s_dis = current.pos.abs(*s_LinkPos);

    daHorse_c* horse = dComIfGp_getHorseActor();
    if (horse != NULL) {
        s_HorseAngle = horse->current.angle.y;
    }

    cXyz targetPos(field_0x60c, 0.0f, 0.0f);
    cLib_offsetPos(&targetPos, s_LinkPos, s_TargetAngle, &targetPos);
    s_TargetPos = targetPos;

    s_PointAngle = cLib_targetAngleY(&current.pos, &s_TargetPos);
    s_TargetDis = current.pos.absXZ(s_TargetPos);

    int i;
    for (i = 0; i < 4; i++) {
        mTimers[i]--;
        if (mTimers[i] <= 0) {
            mTimers[i] = 0;
        }
    }

    for (i = 0; i < 2; i++) {
        mCcTimer[i]--;
        if (mCcTimer[i] <= 0) {
            mCcTimer[i] = 0;
        }
    }

    Yazirushi();
    Action();

    if (mTimers[1] == 0) {
        mAction = 4;
        mMode = 0;
        attention_info.flags = 0;
        mCcBodySph[0].OffCoSetBit();
        mCcBodySph[1].OffCoSetBit();
        mTimers[1] = 1000;
    }

    setBaseMtx();
    setEfMtx();

    if (mCcTimer[0] == 0) {
        SetBodySph();
        SetSwordSph();
    }

    mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    fopAcM_calcSpeed(this);

    cXyz* pos_p = fopAcM_GetPosition_p(this);
    cXyz* speed_p = fopAcM_GetSpeed_p(this);
    current.pos.x += speed_p->x;
    current.pos.y += speed_p->y;
    current.pos.z += speed_p->z;

    cXyz sp14(current.pos.x, 100.0f + current.pos.y, current.pos.z);
    if (fopAcM_gc_c::gndCheck(&sp14)) {
        current.pos.y = 50.0f + fopAcM_gc_c::getGroundY();
    }

    UpDown();
    mAcch.CrrPos(dComIfG_Bgsp());
    return 1;
}

void daE_FK_c::At_Check(int i_sphIdx) {
    mAtInfo.mpCollider = mCcBodySph[i_sphIdx].GetTgHitObj();
    mAtInfo.mpActor = at_power_check(&mAtInfo);

    if (mAtInfo.mpActor != 0) {
        if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
            mAtInfo.mAttackPower = 0;
        }

        if ((s16)mAtInfo.mAttackPower > 0) {
            health -= (s16)mAtInfo.mAttackPower;
        }

        int unk_r29 = 0;

        if (mAtInfo.mAttackPower != 0) {
            mAtInfo.mHitStatus = 2;
            unk_r29 = 5;
        } else {
            unk_r29 = 2;
        }

        if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_MIDNA_LOCK)) {
            unk_r29 = 0;
        }

        u8 at_se = ((dCcD_GObjInf*)mAtInfo.mpCollider)->GetAtSe();
        u32 var_r28 = 0x1E;
        if (mAtInfo.mHitStatus == 1) {
            var_r28 = 0x1F;
        } else if (mAtInfo.mHitStatus == 2) {
            var_r28 = 0x20;
        }

        if (mAtInfo.mpSound != NULL) {
            if (mAtInfo.field_0x18 != 0) {
                mAtInfo.mpSound->startCollisionSE(dCcD_GObjInf::getHitSeID(at_se, 0), mAtInfo.field_0x18);
            } else {
                mAtInfo.mpSound->startCollisionSE(dCcD_GObjInf::getHitSeID(at_se, 0), var_r28);
            }
        }

        if ((mAtInfo.mHitType == 1 || mAtInfo.mpCollider->ChkAtType(AT_TYPE_THROW_OBJ)) && mAtInfo.mpCollider->ChkAtType(AT_TYPE_THROW_OBJ)) {
            unk_r29 = 4;
        }
    }
}

int daE_FK_c::Draw() {
    J3DModel* model_p = mpModelMorf->getModel();
    
    g_env_light.settingTevStruct(7, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model_p, &tevStr);

    model_p->getModelData()->getMaterialNodePointer(0)->getTevKColor(3)->a = 255.0f * mAlpha * l_HIO.alpha;
    mInvisibleModel.entryDL(NULL);

    cXyz sp8;
    sp8.set(current.pos.x, 100.0f + current.pos.y, current.pos.z);
    return 1;
}

int daE_FK_c::Delete() {
    dComIfG_resDelete(&mPhase, "E_fk");

    if (mHIOInit) {
        l_HIOInit = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    mSound.deleteObject();
    return 1;
}

void daE_FK_c::setBaseMtx() {
    cXyz sp8(0.0f, 50.0f, 0.0f);
    scale.set(mSize, mSize, mSize);

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(sp8);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);

    mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModelMorf->modelCalc();
}

void daE_FK_c::setEfMtx() {
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0x18));
    mpEffModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static int daE_FK_Draw(daE_FK_c* i_this) {
    return i_this->Draw();
}

static int daE_FK_Execute(daE_FK_c* i_this) {
    return i_this->Execute();
}

static int daE_FK_IsDelete(daE_FK_c* i_this) {
    return 1;
}

static int daE_FK_Delete(daE_FK_c* i_this) {
    return i_this->Delete();
}

void daE_FK_c::CreateChk() {
    u8 arg0 = fopAcM_GetParam(this) & 0xF;
    if (arg0 == 0) {
        field_0x60c = 0.0f;
        mMaxSpeed = 100.0f;
    } else if (arg0 == 1) {
        mMaxSpeed = 85.0f;
        field_0x60c = 0.0f;
    } else if (arg0 == 2) {
        mMaxSpeed = 75.0f;
        field_0x60c = 0.0f;
    } else if (arg0 == 3) {
        mMaxSpeed = 85.0f;
        field_0x60c = 0.0f;
    } else {
        mMaxSpeed = 100.0f;
        field_0x60c = 0.0f;
    }
}

int daE_FK_c::Create() {
    fopAcM_ct(this, daE_FK_c);

    int phase_state = dComIfG_resLoad(&mPhase, "E_fk");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_FK PARAM %x\n", fopAcM_GetParam(this));
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x1C20)) {
            return cPhs_ERROR_e;
        }

        if (!l_HIOInit) {
            l_HIOInit = TRUE;
            mHIOInit = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("ファントム騎馬兵", &l_HIO);
        }

        fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -700.0f, 0.0f, -700.0f);
        fopAcM_SetMax(this, 700.0f, 700.0f, 700.0f);

        CreateChk();

        gravity = 0.0f;
        attention_info.flags = 0;

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);

        mSound.init(&current.pos, &eyePos, 3, 1);
        mAtInfo.mpSound = &mSound;
        mAtInfo.mPowerType = 1;

        initBodySph();
        initSwordSph();

        fopAcM_setCullSizeFar(this, 3.0f);
        mSound.setEnemyName("E_FK");
        scale.set(0.0f, 0.0f, 0.0f);
        mSize = 0.0f;

        J3DModel* model_p = mpModelMorf->getModel();
        model_p->setUserArea((uintptr_t)this);

        for (u16 i = 0; i < model_p->getModelData()->getJointNum(); i++) {
            if (i != 0) {
                model_p->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
            }
        }

        health = 10;

        cXyz sp10(current.pos.x, 100.0f + current.pos.y, current.pos.z);
        if (fopAcM_gc_c::gndCheck(&sp10)) {
            current.pos.y = fopAcM_gc_c::getGroundY();
        }

        mAction = 1;
        mMode = 0;
        SetAnm(12, 2, 1.0f, 1.0f);
        mTimers[1] = 1000;

        daE_FK_Execute(this);
    }

    return phase_state;
}

static int daE_FK_Create(fopAc_ac_c* i_this) {
    return ((daE_FK_c*)i_this)->Create();
}

AUDIO_INSTANCES

static actor_method_class l_daE_FK_Method = {
    (process_method_func)daE_FK_Create,
    (process_method_func)daE_FK_Delete,
    (process_method_func)daE_FK_Execute,
    (process_method_func)daE_FK_IsDelete,
    (process_method_func)daE_FK_Draw,
};

extern actor_process_profile_definition g_profile_E_FK = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_FK,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_FK_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  159,                    // mPriority
  &l_daE_FK_Method,       // sub_method
  0x00040100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
