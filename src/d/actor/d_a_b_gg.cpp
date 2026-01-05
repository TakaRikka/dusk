/**
 * @file d_a_b_gg.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_gg.h"
#include "d/d_com_inf_game.h"
#include "c/c_damagereaction.h"
#include "m_Do/m_Do_graphic.h"
#include "Z2AudioLib/Z2Instances.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"

class daB_GG_HIO_c : public JORReflexible {
public:
    daB_GG_HIO_c();
    virtual ~daB_GG_HIO_c() {}
    void genMessage(JORMContext*) {}

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 base_size;
    /* 0x0C */ s16 takeoff_time;
    /* 0x0E */ s16 fly_attack_wait_time;
    /* 0x10 */ f32 fly_attack_speed;
    /* 0x14 */ s16 takeoff2_time;
    /* 0x16 */ s16 max_hp;
    /* 0x18 */ f32 field_0x18;
};

enum daE_GG_TYPE {
    TYPE_L7_MBOSS = 0,
    TYPE_L7_W = 1,
    TYPE_L7_M = 2,
    TYPE_HYRULE_CASTLE = 3,
    TYPE_CAVE = 4,

    TYPE_DEFAULT = 0xFF,
};

enum daE_GG_ACTION {
    ACTION_FLY,
    ACTION_GROUND,
    ACTION_F_A,
    ACTION_DEMO,
    ACTION_STOP,
};

enum {
    SUBACT_WAIT   = 0,
    SUBACT_MOVE   = 1,
    SUBACT_ATTACK = 2,
    SUBACT_DAMAGE = 3,
    SUBACT_DEATH  = 4,

    // DEMO
    SUBACT_DEMO_ST = 0,
    SUBACT_DEMO_MD = 1,
    SUBACT_DEMO_GN = 2,
};

enum B_GG_RES_FILE_ID {
    /* BCK */
    /* 0x04 */ BCK_GGA_ATTACK_1 = 4,
    /* 0x05 */ BCK_GGA_ATTACK_2,
    /* 0x06 */ BCK_GGA_ATTACK_3,
    /* 0x07 */ BCK_GGA_ATTACK_4,
    /* 0x08 */ BCK_GGA_FS_HIT,
    /* 0x09 */ BCK_GGA_FS_OFF,
    /* 0x0A */ BCK_GGA_GUARD,
    /* 0x0B */ BCK_GGA_PREPARATION,
    /* 0x0C */ BCK_GGA_PULL,
    /* 0x0D */ BCK_GGA_SHOUT,
    /* 0x0E */ BCK_GGA_WAIT,
    /* 0x0F */ BCK_GGB_ATTACK_A,
    /* 0x10 */ BCK_GGB_ATTACK_B,
    /* 0x11 */ BCK_GGB_ATTACK_C,
    /* 0x12 */ BCK_GGB_BACK_STEP,
    /* 0x13 */ BCK_GGB_DAMAGE_L,
    /* 0x14 */ BCK_GGB_DAMAGE_R,
    /* 0x15 */ BCK_GGB_DIE,
    /* 0x16 */ BCK_GGB_FAINT,
    /* 0x17 */ BCK_GGB_FS_HIT,
    /* 0x18 */ BCK_GGB_GUARD,
    /* 0x19 */ BCK_GGB_PULL,
    /* 0x1A */ BCK_GGB_SIDE_STEP,
    /* 0x1B */ BCK_GGB_TAKE_OFF,
    /* 0x1C */ BCK_GGB_WAIT_A,
    /* 0x1D */ BCK_GGB_WAIT_B,
    /* 0x1E */ BCK_GGB_WALK,
    /* 0x1F */ BCK_GG_DIE,
    /* 0x20 */ BCK_GG_WAIT,

    /* BMDR */
    /* 0x23 */ BMDR_GG = 0x23,
    /* 0x24 */ BMDR_GG_MET,
    /* 0x25 */ BMDR_GG_SHIELD,
    /* 0x26 */ BMDR_GG_SWORD,
};

enum Joint {
    /* 0x00 */ JNT_WORLD_ROOT,
    /* 0x01 */ JNT_BACKBONE01,
    /* 0x02 */ JNT_BACKBONE02,
    /* 0x03 */ JNT_NECK01,
    /* 0x04 */ JNT_NECK02,
    /* 0x05 */ JNT_HEAD,
    /* 0x06 */ JNT_JAW,
    /* 0x07 */ JNT_NECKLACE,
    /* 0x08 */ JNT_SHOULDER_L,
    /* 0x09 */ JNT_ARM_L_01,
    /* 0x0A */ JNT_ARM_L_02,
    /* 0x0B */ JNT_HAND_L,
    /* 0x0C */ JNT_FINGER_L,
    /* 0x0D */ JNT_SHOULDER_R,
    /* 0x0E */ JNT_ARM_R01,
    /* 0x0F */ JNT_ARM_R02,
    /* 0x10 */ JNT_HAND_R,
    /* 0x11 */ JNT_FINGER_R,
    /* 0x12 */ JNT_WING_L01,
    /* 0x13 */ JNT_WING_L02,
    /* 0x14 */ JNT_WING_R01,
    /* 0x15 */ JNT_WING_R02,
    /* 0x16 */ JNT_WAIST,
    /* 0x17 */ JNT_LEG_L01,
    /* 0x18 */ JNT_LEG_L02,
    /* 0x19 */ JNT_LEG_L03,
    /* 0x1A */ JNT_FOOT_L,
    /* 0x1B */ JNT_LEG_R01,
    /* 0x1C */ JNT_LEG_R02,
    /* 0x1D */ JNT_LEG_R03,
    /* 0x1E */ JNT_FOOT_R,
    /* 0x1F */ JNT_MAEKAKE,
    /* 0x20 */ JNT_TAIL01,
    /* 0x21 */ JNT_TAIL02,
    /* 0x22 */ JNT_TAIL03,
    /* 0x23 */ JNT_TAIL04,
};

daB_GG_HIO_c::daB_GG_HIO_c() {
    id = -1;
    base_size = 0.8f;
    takeoff_time = 400;
    fly_attack_speed = 3.0f;
    fly_attack_wait_time = 80;
    takeoff2_time = 250;
    max_hp = 700;
    field_0x18 = 0.5f;
}

int daB_GG_c::CreateHeap() {
    J3DModelData* modelData;
    modelData = (J3DModelData*)dComIfG_getObjectRes("B_gg", 0x23);
    JUT_ASSERT(207, modelData != NULL);

    mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("B_gg", 0xE), 2, 1.0f, 0, -1, &mSound, 0x80000, 0x11000084);
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("B_gg", BMDR_GG_SHIELD);
    mpShieldModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpShieldModel == NULL) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("B_gg", BMDR_GG_SWORD);
    mpSwordModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpSwordModel == NULL) {
        return 0;
    }

    if (mType == TYPE_L7_MBOSS || mType == TYPE_DEFAULT) {
        modelData = (J3DModelData*)dComIfG_getObjectRes("B_gg", BMDR_GG_MET);
        mpHelmetModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (mpHelmetModel == NULL) {
            return 0;
        }
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    daB_GG_c* a_this = (daB_GG_c*)i_this;
    return a_this->CreateHeap();
}

void daB_GG_c::initCc() {
    const static dCcD_SrcCyl ccCylSrc = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x3}, {0xd8fbbdff, 0x43}, 0x75}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x2, 0x0, 0x0, 0x1}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {
                {0.0f, 0.0f, 0.0f}, // mCenter
                130.0f, // mRadius
                200.0f // mHeight
            } // mCyl
        } // mCylAttr
    };

    const static dCcD_SrcCyl ccHookSrc = {
        {
            {0x0, {{0x0, 0x1, 0x0}, {0x4000, 0x3}, 0x0}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x2, 0x0, 0x0, 0x0}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {
                {0.0f, 0.0f, 0.0f}, // mCenter
                130.0f, // mRadius
                200.0f // mHeight
            } // mCyl
        } // mCylAttr
    };

    const static dCcD_SrcSph ccShieldSphSrc = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x0}, {0xd8fbbdff, 0x3}, 0x75}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 50.0f} // mSph
        } // mSphAttr
    };

    const static dCcD_SrcSph ccSphSrc = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
            {dCcD_SE_SWORD, 0x0, 0x7, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x1}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 50.0f} // mSph
        } // mSphAttr
    };

    mCcStts.Init(0xFE, 0xFF, this);
    mCcCyl.Set(ccCylSrc);
    mCcCyl.SetStts(&mCcStts);
    mCcCyl.SetC(current.pos);
    mCcCyl.OffAtSetBit();
    mCcCyl.OnTgNoHitMark();
    mCcCyl.OnTgShield();

    mCcHookCyl.Set(ccHookSrc);
    mCcHookCyl.SetStts(&mCcStts);
    mCcHookCyl.SetC(current.pos);
    mCcHookCyl.OffAtSetBit();
    mCcHookCyl.OnTgNoHitMark();

    mCcShieldSph.Set(ccShieldSphSrc);
    mCcShieldSph.SetStts(&mCcStts);
    mCcShieldSph.SetC(current.pos);
    mCcShieldSph.OnTgNoHitMark();
    mCcShieldSph.OnTgShield();

    for (int i = 0; i < 3; i++) {
        mCcSph[i].Set(ccSphSrc);
        mCcSph[i].SetStts(&mCcStts);
        mCcSph[i].SetC(current.pos);
        mCcSph[i].OffTgNoHitMark();
        mCcSph[i].OffTgSetBit();
    }
}

static u8 hio_set;

static daB_GG_HIO_c l_HIO;

void daB_GG_c::setCcCylinder() {
    mCcCyl.SetC(current.pos);
    mCcCyl.SetR(100.0f * l_HIO.base_size);
    dComIfG_Ccsp()->Set(&mCcCyl);

    mCcHookCyl.SetC(current.pos);
    mCcHookCyl.SetR(100.0f * l_HIO.base_size);
    dComIfG_Ccsp()->Set(&mCcHookCyl);

    cXyz center_pos;
    for (int i = 0; i < 3; i++) {
        center_pos.set(0.0f, (50.0f + (100.0f * i)) * l_HIO.base_size, 0.0f);

        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0x10));
        mDoMtx_stack_c::multVec(&center_pos, &center_pos);

        mCcSph[i].SetC(center_pos);
        mCcSph[i].SetR(100.0f * l_HIO.base_size);
        dComIfG_Ccsp()->Set(&mCcSph[i]);
    }

    center_pos.set(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0xB));
    mDoMtx_stack_c::multVec(&center_pos, &center_pos);

    mCcShieldSph.SetC(center_pos);
    mCcShieldSph.SetR(50.0f * l_HIO.base_size);
    dComIfG_Ccsp()->Set(&mCcShieldSph);
}

void daB_GG_c::G_setCcCylinder() {
    cXyz center_pos;

    mCcCyl.SetC(current.pos);
    mCcCyl.SetR(100.0f * l_HIO.base_size);
    dComIfG_Ccsp()->Set(&mCcCyl);

    mCcHookCyl.SetC(current.pos);
    mCcHookCyl.SetR(100.0f * l_HIO.base_size);
    dComIfG_Ccsp()->Set(&mCcHookCyl);

    center_pos.set(yREG_F(15), yREG_F(16), 50.0f + yREG_F(17));
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0xB));
    mDoMtx_stack_c::multVec(&center_pos, &center_pos);

    mCcShieldSph.SetC(center_pos);
    mCcShieldSph.SetR((150.0f + yREG_F(18)) * l_HIO.base_size);
    dComIfG_Ccsp()->Set(&mCcShieldSph);
}

void daB_GG_c::MoveCo() {
    cXyz center_pos;
    center_pos = current.pos;
    center_pos.y += 10000.0f;

    mCcCyl.SetC(center_pos);
    mCcShieldSph.SetC(center_pos);
    mCcHookCyl.SetC(center_pos);
}

int daB_GG_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* temp_r28 = i_joint;

    int jnt_no = temp_r28->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));

    csXyz spC;
    switch (jnt_no) {
    case JNT_BACKBONE01:
        mDoMtx_stack_c::ZrotM(0.3f * field_0x6c4);
        break;
    case JNT_HEAD:
        mDoMtx_stack_c::YrotM(-field_0x6be);
        mDoMtx_stack_c::ZrotM(0.7f * field_0x6c4);
        break;
    case JNT_ARM_L_01:
        mDoMtx_stack_c::ZrotM((s16)mArmL1Rot.z);
        mDoMtx_stack_c::XrotM((s16)mArmL1Rot.x);
        mDoMtx_stack_c::YrotM((s16)mArmL1Rot.y);
        break;
    case JNT_ARM_L_02:
        mDoMtx_stack_c::ZrotM((s16)mArmL2Rot.z);
        mDoMtx_stack_c::XrotM((s16)mArmL2Rot.x);
        mDoMtx_stack_c::YrotM((s16)mArmL2Rot.y);
        break;
    }

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

static int JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daB_GG_c* a_this = (daB_GG_c*)model->getUserArea();

        if (a_this != NULL) {
            a_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

int daB_GG_c::HeadctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jnt_no = i_joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));

    switch (jnt_no) {
    case JNT_BACKBONE01:
        mDoMtx_stack_c::YrotM(0.5f * field_0x6d4);
        mDoMtx_stack_c::ZrotM(0.5f * field_0x6d6);
        break;
    case JNT_BACKBONE02:
        mDoMtx_stack_c::YrotM(0.5f * field_0x6d4);
        mDoMtx_stack_c::ZrotM(0.5f * field_0x6d6);
        break;
    }

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

static int HeadJointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daB_GG_c* a_this = (daB_GG_c*)model->getUserArea();

        if (a_this != NULL) {
            a_this->HeadctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void daB_GG_c::SetAnm(int i_anm, int i_mode, f32 i_morf, f32 i_speed) {
    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_gg", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
    mAnm = i_anm;
}

static BOOL way_bg_check2(daB_GG_c* a_this, cXyz i_start, cXyz i_end) {
    dBgS_LinChk line_chk;
    i_start.y += 30.0f;
    line_chk.Set(&i_start, &i_end, a_this);

    if (dComIfG_Bgsp().LineCross(&line_chk)) {
        return TRUE;
    }

    return FALSE;
}

static f32 dummyFloat() {
    return 300.0f;
}

static BOOL roofchk(daB_GG_c* a_this) {
    dBgS_LinChk line_chk;
    cXyz sp24;
    cXyz start;
    cXyz end;

    start = a_this->current.pos;
    end = a_this->current.pos;
    end.y += 700.0f;
    line_chk.Set(&start, &end, a_this);

    if (dComIfG_Bgsp().LineCross(&line_chk)) {
        a_this->current.pos.y = a_this->old.pos.y;
        return TRUE;
    }

    return FALSE;
}

static BOOL other_bg_check(daB_GG_c* a_this, fopAc_ac_c* i_other) {
    fopAc_ac_c* i_this = (fopAc_ac_c*)a_this;
    dBgS_LinChk line_chk;
    cXyz sp28;
    cXyz start;
    cXyz end;

    end = i_other->current.pos;
    end.y += 100.0f;

    start = i_this->current.pos;
    start.y += 100.0f;

    line_chk.Set(&start, &end, i_this);

    if (dComIfG_Bgsp().LineCross(&line_chk)) {
        return TRUE;
    }

    return FALSE;
}

static f32 dummyFloat2() {
    return 800.0f;
}

static cXyz Hanekaeri(daB_GG_c* a_this, cXyz param_1, cXyz param_2, f32 param_3) {
    cXyz sp28;

    f32 temp_f1 = param_2.abs();
    sp28.x = param_2.x / temp_f1;
    sp28.y = param_2.y / temp_f1;
    sp28.z = param_2.z / temp_f1;

    cXyz ret;
    cXyz sp1C;
    cXyz sp10;

    sp1C -= param_1;
    sp28 *= 2.0f * sp1C.inprod(param_2);
    sp28 += param_1;

    f32 temp_f30 = param_3 * param_1.abs();

    ret.x = 0.8f * ((sp28.x / sp28.abs()) * param_1.abs());
    ret.y = temp_f30 * (sp28.y / sp28.abs());
    ret.z = 0.8f * ((sp28.z / sp28.abs()) * param_1.abs());
    return ret;
}

void daB_GG_c::Yazirushi() {
    eyePos = current.pos;
    eyePos.y += 150.0f;

    attention_info.position = eyePos;
    attention_info.position.y += 150.0f;
}

void daB_GG_c::SpeedClear() {
    speedF = 0.0f;
    field_0x5bc = 0.0f;
    speed.y = 0.0f;
    field_0x5c0 = 0.0f;
}

void daB_GG_c::SetAction(u8 i_action, u8 i_subAction, u8 i_mode) {
    mPrevAction = mAction;
    mPrevSubAction = mSubAction;
    mPrevMode = mMode;

    mAction = i_action;
    mSubAction = i_subAction;
    mMode = i_mode;
}

u8 daB_GG_c::GetAction(u8 i_action, u8 i_subAction, u8 i_mode) {
    if (mPrevAction == i_action && mPrevSubAction == i_subAction && mPrevMode == i_mode) {
        return true;
    }

    return false;
}

namespace {
static f32 s_dis;

static cXyz* s_LinkPos;

static s16 s_TargetAngle;

static cXyz s_centerpos(217.0f, 1275.0f, -57.0f);

static s16 s_M_Action;

static s16 s_W_Action;
}

void daB_GG_c::DemoSkip(int param_0) {
    cXyz offset(0.0f, 200.0f, 300.0f);
    cXyz sp14(0.0f, 0.0f, 0.0f);
    cXyz sp8(0.0f, 0.0f, 0.0f);

    cDmr_SkipInfo = 1;

    if (param_0 == 0) {
        dStage_changeScene(1, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
        Z2GetAudioMgr()->subBgmStop();
    } else if (param_0 == 1) {
        cDmr_SkipInfo = 0;

        cLib_offsetPos(&sp8, s_LinkPos, s_TargetAngle, &offset);
        mCamEye = sp8;

        SetReleaseCam();

        mAction = ACTION_FLY;
        mSubAction = SUBACT_WAIT;
        mMode = 0;

        current.pos.set(5930.0f, 3157.0f, 6993.0f);
        old.pos = current.pos;
    }

    dComIfGp_getVibration().StopQuake(0x1F);
}

int daB_GG_c::DemoSkipCallBack(void* i_process, int param_1) {
    if (i_process != NULL) {
        ((daB_GG_c*)i_process)->DemoSkip(param_1);
        return 1;
    }

    return 0;
}

bool daB_GG_c::CameraSet() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    dCamera_c* cam_body = dCam_getBody();

    if (!eventInfo.checkCommandDemoAccrpt()) {
        fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
        eventInfo.onCondition(dEvtCnd_NONE_e);
        return false;
    }

    camera->mCamera.Stop();
    mCamFovy = cam_body->Fovy();
    mCamCenter = cam_body->Center();
    field_0x60c = mCamCenter;
    mCamEye = cam_body->Eye();
    field_0x618 = mCamEye;
    camera->mCamera.SetTrimSize(3);
    return true;
}

void daB_GG_c::SetStopingCam() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovy, 0);
}

void daB_GG_c::SetStopCam(cXyz param_0, f32 param_1, f32 param_2, s16 param_3) {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    cXyz sp38(0.0f, param_2, param_1);
    cXyz sp2C(0.0f, 0.0f, 0.0f);
    mCamCenter.set(param_0.x, param_0.y, param_0.z);
    field_0x60c = mCamCenter;
    mCamCenterTarget = mCamCenter;

    cLib_offsetPos(&sp2C, &param_0, param_3, &sp38);
    mCamEye = sp2C;
    field_0x618 = sp2C;
    mCamEyeTarget = sp2C;

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovy, 0);
}

void daB_GG_c::SetMoveCam(cXyz param_0, f32 param_1, f32 param_2, s16 param_3, f32 param_4,
                          f32 param_5) {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp38(0.0f, param_2, param_1);
    cXyz sp2C(0.0f, 0.0f, 0.0f);
    field_0x60c = param_0;

    cLib_offsetPos(&sp2C, &param_0, param_3, &sp38);
    field_0x618 = sp2C;

    cLib_addCalcPos(&mCamCenterTarget, field_0x60c, param_4, param_5, 0.0f);
    cLib_addCalcPos(&mCamEyeTarget, field_0x618, param_4, param_5, 0.0f);
    cLib_addCalcPos(&mCamCenter, mCamCenterTarget, param_4, param_5, 0.0f);
    cLib_addCalcPos(&mCamEye, mCamEyeTarget, param_4, param_5, 0.0f);

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovy, 0);
}

void daB_GG_c::SetMoveCam1(f32 i_scale, f32 i_step) {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    cLib_addCalcPos(&mCamCenterTarget, field_0x60c, i_scale, i_step, 0.0f);
    cLib_addCalcPos(&mCamEyeTarget, field_0x618, i_scale, i_step, 0.0f);
    cLib_addCalcPos(&mCamCenter, field_0x60c, i_scale, i_step, 0.0f);
    cLib_addCalcPos(&mCamEye, field_0x618, i_scale, i_step, 0.0f);

    camera->mCamera.Set(mCamCenter, mCamEye, mCamFovy, 0);
}

void daB_GG_c::SetReleaseCam() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera->mCamera.Reset(mCamCenter, mCamEye);
    camera->mCamera.Start();
    camera->mCamera.SetTrimSize(0);
    dComIfGp_event_reset();
}

static cXyz out_pos[] = {
    cXyz(5714.0f, 1200.0f, 218.0f),
    cXyz(20.0f, 950.0f, -4937.0),
    cXyz(-5560.0f, 1200.0f, 222.0f),
};

static cXyz in_pos[]= {
    cXyz(1185.0f, 1200.0f, 152.0f),
    cXyz(-121.0f, 950.0f, -1190.0f),
    cXyz(-1080.0f, 1200.0f, 7.0f),
};

void daB_GG_c::St_CamAction() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    cXyz sp58(*s_LinkPos);
    cXyz sp4C(0.0f, 0.0f, -300.0f);
    cXyz sp40;

    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    switch (mCamMode) {
    case 0:
        if (s_LinkPos->y < 1000.0f) {
            mTimers[0] = 0;
            if (CameraSet()) {
                Z2GetAudioMgr()->subBgmStart(Z2BGM_GG_MBOSS_D01);
                mSound.startCreatureVoice(Z2SE_EN_GG_V_DEMO_OP, -1);
                mCamMode = 12;

                sp40.set(0.0f, 0.0f, 550.0f + MREG_F(8));
                player->setPlayerPosAndAngle(&sp40, cLib_targetAngleY(&sp40, &sp4C), 0);
                player->changeOriginalDemo();
                player->changeDemoMode(1, 0, 0, 0);
            }
        }
        break;
    case 12:
        sp58 = fopAcM_GetPosition(player);
        sp58.y += 100.0f;

        if (daPy_py_c::checkNowWolf()) {
            sp58.set(0.0f, 0.0f, 0.0f);
            MTXCopy(daPy_getLinkPlayerActorClass()->getModelJointMtx(6), *calc_mtx);
            MtxPosition(&sp58, &sp58);
            sp58.y += 50.0f + yREG_F(13);
        }

        SetStopCam(sp58, 300.0f, 0.0f, cLib_targetAngleY(&sp58, &sp4C) + 0x2000);
        field_0x5ba = cLib_targetAngleY(&sp58, &sp4C) + 0x2000;
        mTimers[0] = 30;
        mCamMode = 1;
        field_0x560 = l_HIO.max_hp;
        health = l_HIO.max_hp;
        field_0x648 = 300.0f;
        break;
    case 1:
        if (mTimers[0] == 0) {
            player->changeDemoMode(0x14, 0, 0, 0);
            player->changeDemoMoveAngle(cLib_targetAngleY(s_LinkPos, &sp4C));
            mTimers[0] = 160;
            mCamMode++;
            field_0x648 = 300.0f;
        }
        break;
    case 2:
        sp58.y += 100.0f;

        if (mTimers[0] == 0) {
            sp58.y += 50.0f;
            player->changeDemoMode(1, 0, 0, 0);
            mCamMode++;
        } else {
            field_0x5ba -= yREG_S(0) + 0x20;

            f32 var_f31;
            f32 var_f30;
            if (!daPy_py_c::checkNowWolf()) {
                var_f31 = 120.0f;
            } else {
                var_f31 = 180.0f + yREG_F(1);
            }

            cLib_chaseF(&field_0x648, var_f31, 1.2f);

            if (!daPy_py_c::checkNowWolf()) {
                var_f31 = 60.0f;
                var_f30 = 0.3f;
            } else {
                var_f31 = 40.0f + yREG_F(9);
                var_f30 = 0.15f;
            }

            cLib_chaseF(&field_0x64c, var_f31, var_f30);
            sp58.y += field_0x64c;

            if (!daPy_py_c::checkNowWolf()) {
                var_f31 = -60.0f;
                var_f30 = 0.3f;
            } else {
                var_f31 = -110.0f + yREG_F(2);
                var_f30 = 0.6f + yREG_F(7);
            }

            cLib_chaseF(&field_0x650, var_f31, var_f30);
            SetMoveCam(sp58, field_0x648, field_0x650, field_0x5ba, 0.2f, 50.0f);
        }
        break;
    case 3:
        if (mTimers[0] == 0) {
            mCamMode++;
            mTimers[0] = 35.0f + HREG_F(0);

            sp58.set(110.0f, XREG_F(2), 0.0f);
            cLib_offsetPos(&field_0x60c, &field_0x60c, cLib_targetAngleY(s_LinkPos, &sp4C), &sp58);

            sp58.set(-90.0f, -25.0f, 0.0f);
            cLib_offsetPos(&field_0x618, &field_0x618, cLib_targetAngleY(s_LinkPos, &sp4C), &sp58);

            mMode = 0;
        }
        break;
    case 4:
        if (mTimers[0] == 0) {
            mCamMode++;

            if (!daPy_py_c::checkNowWolf()) {
                sp58.set(-20.0f + MREG_F(0), 120.0f + MREG_F(5), -300.0f + MREG_F(3));
            } else {
                sp58.set(-20.0f + MREG_F(0), 120.0f + MREG_F(5), -300.0f + MREG_F(3));
            }

            cLib_offsetPos(&field_0x60c, &field_0x60c, cLib_targetAngleY(s_LinkPos, &sp4C), &sp58);

            if (!daPy_py_c::checkNowWolf()) {
                sp58.set(-30.0f + MREG_F(1), MREG_F(6), -10.0f + MREG_F(4));
            } else {
                sp58.set(-30.0f + MREG_F(1), MREG_F(6), -10.0f + MREG_F(4));
            }

            cLib_offsetPos(&field_0x618, &field_0x618, cLib_targetAngleY(s_LinkPos, &sp4C), &sp58);
            mTimers[0] = 50;
            mMode = 1;
            player->changeDemoMoveAngle(-0x3000);
            mTimers[1] = 0;
        }

        if ((f32)mTimers[0] == 30.0f + HREG_F(0)) {
            player->changeDemoMode(0x17, 1, 0, 0);
            player->changeDemoMoveAngle(0x3000);
        }

        if ((f32)mTimers[0] == 25.0f + HREG_F(0)) {
            sp58.set(-60.0f + MREG_F(13), 0.0f, 0.0f);
            cLib_offsetPos(&field_0x60c, &field_0x60c, cLib_targetAngleY(s_LinkPos, &sp4C), &sp58);
        }

        SetMoveCam1(0.1f + yREG_F(9), 40.0f + yREG_F(8));
        attention_info.position = in_pos[2];
        attention_info.position.z += 400.0f;
        eyePos = in_pos[2];
        break;
    case 5:
        SetMoveCam1(0.2f + yREG_F(10), 50.0f + yREG_F(8));

        if (mTimers[0] < 45) {
            attention_info.position = in_pos[0];
            attention_info.position.z += 400.0f;
            eyePos = in_pos[0];
        } else {
            attention_info.position = in_pos[2];
            attention_info.position.z += 400.0f;
            eyePos = in_pos[2];
        }

        if (mTimers[0] == 45) {
            player->changeDemoMode(0x19, 1, 0, 0);
            player->changeDemoMoveAngle(0x3000);
        }

        if ((f32)mTimers[0] == (40.0f + MREG_F(12))) {
            sp58.set(80.0f + MREG_F(10), 0.0f, MREG_F(11));
            cLib_offsetPos(&field_0x60c, &field_0x60c, cLib_targetAngleY(s_LinkPos, &sp4C), &sp58);
        }

        if ((f32)mTimers[0] == HREG_F(1)) {
            mCamMode = 20;

            if (!daPy_py_c::checkNowWolf()) {
                sp58.set(0.0f, -50.0f, 100.0f);
            } else {
                sp58.set(0.0f, 0.0f, 100.0f);
            }

            cLib_offsetPos(&field_0x618, &field_0x618, cLib_targetAngleY(s_LinkPos, &sp4C), &sp58);
            field_0x60c.x = s_LinkPos->x;
            field_0x60c.y = 50.0f + s_LinkPos->y + MREG_F(14);
            field_0x60c.z = s_LinkPos->z;

            mTimers[0] = 70;
        }
        break;
    case 6:
        sp58 = current.pos;
        sp58.y += 250.0f + BREG_F(16);
        SetMoveCam(sp58, 250.0f + BREG_F(17), -200.0f, cLib_targetAngleY(&current.pos, s_LinkPos), 0.1f, 1.0f);
        break;
    case 7:
        sp58 = current.pos;
        sp58.y += 250.0f + BREG_F(16);
        SetMoveCam(sp58, 400.0f + BREG_F(18), -200.0f, (cLib_targetAngleY(&current.pos, s_LinkPos) - 0x500), 0.1f, 1.0f);
        break;
    case 8:
        if (!daPy_py_c::checkNowWolf()) {
            sp4C = current.pos;
            sp4C += *s_LinkPos;
            field_0x60c.set(100.0f + (0.5f * sp4C.x) + aREG_F(15), 100.0f + (0.5f * sp4C.y) + aREG_F(14), (0.5f * sp4C.z) + aREG_F(16));
            sp58.set(150.0f, 52.0f, -120.0f);
            cLib_offsetPos(&field_0x618, s_LinkPos, cLib_targetAngleY(s_LinkPos, &current.pos), &sp58);
        } else {
            sp4C = current.pos;
            sp4C += *s_LinkPos;
            field_0x60c.set(100.0f + (0.5f * sp4C.x) + aREG_F(15), 80.0f + (0.5f * sp4C.y) + aREG_F(14), (0.5f * sp4C.z) + aREG_F(16));
            sp58.set(150.0f, 52.0f, -120.0f);
            cLib_offsetPos(&field_0x618, s_LinkPos, cLib_targetAngleY(s_LinkPos, &current.pos), &sp58);
        }

        SetMoveCam1(0.2f, 3.5f);
        break;
    case 11:
        SetMoveCam1(0.2f, 1.0f + TREG_F(11));
        break;
    case 20:
        if (mTimers[0] == 0) {
            sp58.set(0.0f, 150.0f, -80.0f);
            if (daPy_py_c::checkNowWolf()) {
                sp58.y -= 100.0f;
                sp58.z -= 50.0f;
            }

            cLib_offsetPos(&field_0x618, s_LinkPos, cLib_targetAngleY(s_LinkPos, &sp4C), &sp58);

            sp58.set(0.0f, 340.0f + aREG_F(7), 0.0f);
            if (daPy_py_c::checkNowWolf()) {
                sp58.y -= 80.0f;
            }

            cLib_offsetPos(&field_0x60c, &sp4C, cLib_targetAngleY(s_LinkPos, &sp4C), &sp58);
            mCamEye = field_0x618;
            mCamCenter = field_0x60c;
            SetStopingCam();
            mMode = 2;
            mCamMode = 11;
            SpeedClear();
        }

        SetMoveCam1(0.1f, 3.0f + MREG_F(17));

        if ((f32)mTimers[0] == (30.0f + HREG_F(2))) {
            player->changeDemoMode(0x17, 1, 0, 0);
        }

        if (!((f32)mTimers[0] > (30.0f + HREG_F(2)))) {
            attention_info.position = in_pos[1];
            eyePos = in_pos[1];
        }
        break;
    }

    if (mCamMode != 0) {
        dComIfGp_getEvent()->onSkipFade();
        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 0);
    }
}

void daB_GG_c::St_DemoAction() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    
    cXyz sp58(*s_LinkPos);
    cXyz sp4C(0.0f, 0.0f, -300.0f);
    cXyz sp40;
    cXyz sp34;

    s16 sp8 = s_TargetAngle - shape_angle.y;

    switch (mMode) {
    case 0:
        sp58.set(-400.0f, 100.0f, -250.0f);
        cLib_offsetPos(&current.pos, s_LinkPos, cLib_targetAngleY(s_LinkPos, &sp4C), &sp58);
        old.pos = current.pos;

        sp34.set(100.0f, 200.0f, 0.0f);
        sp58.set(0.0f, 0.0f, 0.0f);
        cLib_offsetPos(&sp58, s_LinkPos, cLib_targetAngleY(s_LinkPos, &sp4C), &sp34);
        field_0x5ba = cLib_targetAngleY(&current.pos, &sp58);
        current.angle.y = field_0x5ba;
        shape_angle.y = current.angle.y;

        SetAnm(BCK_GGA_ATTACK_2, 2, 5.0f, 1.0f);

        field_0x5bc = 100.0f + yREG_F(9);
        speedF = field_0x5bc;
        field_0x5c0 = (sp58.y - current.pos.y) / (current.pos.absXZ(sp58) / speedF);
        speed.y = field_0x5c0;
        mMode = 10;
        break;
    case 1:
        sp58.set(600.0f, 100.0f, -150.0f);
        cLib_offsetPos(&current.pos, s_LinkPos, cLib_targetAngleY(s_LinkPos, &sp4C), &sp58);
        old.pos = current.pos;

        sp34.set(-300.0f, 200.0f, -500.0f + MREG_F(2));
        sp58.set(0.0f, 0.0f, 0.0f);
        cLib_offsetPos(&sp58, s_LinkPos, cLib_targetAngleY(s_LinkPos, &sp4C), &sp34);
        field_0x5ba = cLib_targetAngleY(&current.pos, &sp58);
        current.angle.y = field_0x5ba;
        shape_angle.y = current.angle.y;

        SetAnm(BCK_GGA_ATTACK_2, 2, 5.0f, 1.0f);

        field_0x5bc = 100.0f + yREG_F(9);
        speedF = field_0x5bc;
        field_0x5c0 = (sp58.y - current.pos.y) / (current.pos.absXZ(sp58) / speedF);
        speed.y = field_0x5c0;
        mMode = 10;
        break;
    case 2:
        if (mAnm != BCK_GGA_WAIT) {
            sp58.set(0.0f, 1000.0f + TREG_F(0), 500.0f);
            cLib_offsetPos(&current.pos, s_LinkPos, cLib_targetAngleY(s_LinkPos, &sp4C), &sp58);
            old.pos = current.pos;

            SetAnm(BCK_GGA_WAIT, 2, 5.0f, 1.0f);
            field_0x5c0 = -5.0f;

            sp58.set(0.0f, 50.0f, -250.0f);
            cLib_offsetPos(&field_0x618, s_LinkPos, cLib_targetAngleY(s_LinkPos, &sp4C), &sp58);
            player->changeDemoMode(0x17, 1, 0, 0);
        } else if (mAnm == BCK_GGA_WAIT && (current.pos.y - s_LinkPos->y) < 300.0f) {
            speed.y = 0.0f;
            field_0x5c0 = 0.0f;
            mTimers[0] = 0;
            mMode++;
        }

        field_0x6be = s_TargetAngle - shape_angle.y;
        shape_angle.y = s_TargetAngle;
        break;
    case 3:
        if (mTimers[0] == 0) {
            mMode++;
            mTimers[0] = 130;
        }

        shape_angle.y = s_TargetAngle;
        break;
    case 4:
        if (mTimers[0] == 110) {
            mCamMode = 7;
            sp58 = current.pos;
            sp58.y += 300.0f + BREG_F(16);
            SetStopCam(sp58, 250.0f + BREG_F(17), -200.0f, (cLib_targetAngleY(&current.pos, s_LinkPos) + 0x1000));
        }

        if (mAnm != BCK_GGA_SHOUT && mTimers[0] == 0) {
            SetAnm(BCK_GGA_SHOUT, 0, 5.0f, 1.0f);
            mCamMode = 7;
        } else if (mAnm == BCK_GGA_SHOUT && mpModelMorf->checkFrame(33.0f)) {
            dComIfGp_getVibration().StartQuake(6, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        } else if (mAnm == BCK_GGA_SHOUT && mpModelMorf->isStop()) {
            mMode++;
            SetAnm(BCK_GGA_WAIT, 2, 5.0f, 1.0f);
            mTimers[1] = 200;
            mCamMode = 8;
            dComIfGp_getVibration().StopQuake(0x1F);
            player->changeDemoMode(0x17, 1, 2, 0);
        }

        shape_angle.y = s_TargetAngle;
        break;
    case 5:
        if (mTimers[1] == 0) {
            if (field_0x5ce != 0xFF && !dComIfGs_isSwitch(142, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(142, fopAcM_GetRoomNo(this));
            }

            cDmr_SkipInfo = 1;
            SetReleaseCam();

            mAction = ACTION_FLY;
            mSubAction = SUBACT_WAIT;
            mMode = 1;
            mCamMode = 0;

            Z2GetAudioMgr()->subBgmStart(Z2BGM_GG_MBOSS);
            dComIfGs_onSwitch(226, fopAcM_GetRoomNo(this));
        }

        shape_angle.y = s_TargetAngle;
        break;
    }

    if (mMode > 1 && mMode != 10) {
        sp40 = *s_LinkPos - current.pos;
        cLib_addCalcAngleS2(&field_0x6c4, -cM_atan2s(sp40.y, JMAFastSqrt((sp40.x * sp40.x) + (sp40.z * sp40.z))), 2, 0x600);
        cLib_addCalcAngleS2(&field_0x6be, sp8, 2, 0x100);
    } else {
        field_0x6be = 0;
        field_0x6c4 = 0;
    }
}

void daB_GG_c::Md_CamAction() {
    cXyz sp2C;
    cXyz sp20;

    daPy_py_c* player = daPy_getPlayerActorClass();
    int sp10 = 0;
    int spC = 0;

    switch (mCamMode) {
    case 0:
        if (CameraSet()) {
            mSound.startCreatureVoice(Z2SE_EN_GG_V_DEMO_MID, -1);
            mInOutPosIdx = 0;

            sp2C.set(217.0f, 0.0f, -57.0f);
            current.pos = sp2C;
            old.pos = current.pos;
            shape_angle.y = cLib_targetAngleY(&current.pos, &in_pos[mInOutPosIdx]);
            current.angle.y = shape_angle.y;

            sp20 = current.pos;
            sp20.y += 150.0f + BREG_F(13);
            SetStopCam(sp20, 300.0f, 0.0f, shape_angle.y);
            mCamMode++;

            sp20.set(0.0f, mAcch.GetGroundH(), 0.0f);
            sp2C.set(0.0f, 0.0f, -300.0f);
            cLib_offsetPos(&sp20, &mCamEye, cLib_targetAngleY(&mCamEye, &current.pos), &sp2C);
            player->setPlayerPosAndAngle(&sp20, shape_angle.y, 0);

            field_0x658 = shape_angle.y;
            field_0x648 = 300.0f;
            if (field_0x5ce != 0xFF && dComIfGs_isSwitch(142, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(142, fopAcM_GetRoomNo(this));
            }

            field_0x5b0 = 0;
        }
        break;
    case 1:
        sp20.set(217.0f, 0.0f, -57.0f);
        sp20.y += 150.0f + BREG_F(13);

        field_0x60c = current.pos;
        field_0x60c.y += 150.0f + BREG_F(18);
        field_0x658 += 80;
        field_0x648 += 3.0f;

        sp2C.set(0.0f, 0.0f, field_0x648);
        cLib_offsetPos(&field_0x618, &sp20, field_0x658, &sp2C);

        SetMoveCam1(0.4f, 50.0f);

        sp20 = current.pos;
        sp20.set(0.0f, s_LinkPos->y, 0.0f);
        sp2C.set(0.0f, 0.0f, -300.0f);
        cLib_offsetPos(&sp20, &mCamEye, cLib_targetAngleY(&mCamEye, &current.pos), &sp2C);

        player->setPlayerPosAndAngle(&sp20, shape_angle.y, 0);
        break;
    }
}

void daB_GG_c::Md_DemoAction() {
    s16 sp10 = 0;
    u8 var_r29 = 0;
    s16 i;
    cXyz sp3C;
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (mMode) {
    case 0:
        mCcShieldSph.OnTgShield();
        mCcCyl.OnTgShield();
        mCcCyl.OffTgSetBit();
        mCcShieldSph.OffTgSetBit();
        mCcHookCyl.OffTgSetBit();

        if (mAnm != BCK_GGB_TAKE_OFF && mpModelMorf->isStop() && mAnm != 0x1D) {
            mCcCyl.OnTgSetBit();
            mCcShieldSph.OnTgSetBit();

            SetAnm(BCK_GGB_WAIT_B, 2, 5.0f, 1.0f);

            if (field_0x5b0 == 0) {
                mTimers[0] = 50;
                field_0x5b0 = 1;
            } else {
                mTimers[0] = 0;
            }
        } else if (mpModelMorf->checkFrame(18.0f) && mAnm == BCK_GGB_TAKE_OFF) {
            speed.y = 90.0f;
            field_0x5c0 = 0.0f;
            gravity = -4.2f;
            var_r29 = 1;
        } else if (mpModelMorf->isStop() && mAnm == BCK_GGB_TAKE_OFF) {
            mMode++;
            gravity = 0.0f;
            speed.y = 0.0f;

            if (field_0x5ce != 0xFF && !dComIfGs_isSwitch(142, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(142, fopAcM_GetRoomNo(this));
            }

            SetAnm(BCK_GGA_ATTACK_1, 0, 5.0f, 1.0f);
            var_r29 = 1;
        } else if (mpModelMorf->getFrame() > 18.0f && mAnm == BCK_GGB_TAKE_OFF) {
            var_r29 = 1;
        } else if (mAnm == BCK_GGB_WAIT_B && mTimers[0] == 0) {
            fopAcM_OffStatus(this, 0x200000);
            fopAcM_OnStatus(this, 0x80000);
            SetAnm(BCK_GGB_TAKE_OFF, 0, 5.0f, 1.0f);

            mSound.startCreatureVoice(Z2SE_EN_GG_V_TAKE_OFF, -1);
            SpeedClear();

            s16 spE = cLib_targetAngleY(&current.pos, &in_pos[0]) - shape_angle.y;
            for (i = 1; i < 3; i++) {
                s16 spC = cLib_targetAngleY(&current.pos, &in_pos[i]) - shape_angle.y;
                if (abs(spC) < abs(spE)) {
                    sp10 = i;
                    spE = spC;
                }
            }

            mInOutPosIdx = sp10;
            field_0x5ba = cLib_targetAngleY(&current.pos, &in_pos[mInOutPosIdx]);
        }
        break;
    case 1:
        if (mAnm == BCK_GGA_ATTACK_1 && mpModelMorf->isStop()) {
            SetAnm(BCK_GGA_ATTACK_2, 2, 5.0f, 1.0f);
            field_0x5bc = 100.0f;
            speedF = field_0x5bc;
            field_0x5c0 = (out_pos[mInOutPosIdx].y - current.pos.y) / (current.pos.absXZ(out_pos[mInOutPosIdx]) / speedF);
            speed.y = field_0x5c0;
        } else if (mAnm == BCK_GGA_ATTACK_2) {
            mSound.startCreatureSoundLevel(Z2SE_EN_GG_ATK_MOVE, 0, -1);
            if (F_A_TargetMove(in_pos[mInOutPosIdx])) {
                mMode++;
                mTimers[0] = 20;
                attention_info.flags = 0;
            }
        }

        var_r29 = 1;
        break;
    case 2:
        mSound.startCreatureSoundLevel(Z2SE_EN_GG_ATK_MOVE, 0, -1);

        if (mAnm == BCK_GGA_ATTACK_2) {
            if (F_A_TargetMove(out_pos[mInOutPosIdx], 1) || (!GetAction(2, 0, 0) && mTimers[0] == 0)) {
                SetAction(ACTION_F_A, SUBACT_MOVE, 0);
                mTimers[0] = 50;

                if (field_0x5b6 == 0) {
                    fopAcM_SetSpeed(player, 0.0f, 0.0f, 0.0f);
                    attention_info.flags = 0;
                    SetReleaseCam();

                    sp3C.set(217.0f, 0.0f, -57.0f);
                    player->setPlayerPosAndAngle(&sp3C, shape_angle.y, 0);

                    if (field_0x5ce != 0xFF && dComIfGs_isSwitch(142, fopAcM_GetRoomNo(this))) {
                        dComIfGs_onSwitch(142, fopAcM_GetRoomNo(this));
                    }

                    field_0x5b6 = 1;
                    current.pos.y += 10000.0f;
                    old.pos.y = current.pos.y;
                    mTimers[0] = 40;
                    field_0x648 = s_LinkPos->y;
                }
            }
        }

        var_r29 = 1;
        break;
    }

    if (var_r29 == 1) {
        cLib_addCalcAngleS(&current.angle.y, field_0x5ba, 5, 0x1000, 0);
        cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 2, 0x1000, 0);
    }
}

void daB_GG_c::Gn_CamAction() {
    cXyz sp24;
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (mCamMode) {
    case 0:
        if (field_0x5d0 != 0xFF && dComIfGs_isSwitch(field_0x5d0, fopAcM_GetRoomNo(this))) {
            mCamMode++;
            current.pos.set(-7853.0f + HREG_F(0), 10535.0f + HREG_F(1), 8619.0f + HREG_F(2));
            old.pos = current.pos;
        }
        break;
    case 1:
        if (CameraSet()) {
            sp24 = current.pos;
            SetStopCam(sp24, 4000.0f + aREG_F(5), -4000.0f, shape_angle.y);

            mCamMode++;
            mTimers[0] = 90;

            player->changeOriginalDemo();
            player->changeDemoMode(4, 1, 0, 0);
            mSound.startCreatureVoice(Z2SE_EN_GG_V_DEMO_OP_LV9, -1);
        }
        break;
    case 2:
        if (mTimers[0] == 0) {
            mMode++;
            mCamMode++;

            if (daPy_py_c::checkNowWolf()) {
                sp24.set(6469.0f, s_LinkPos->y, 6465.0f + aREG_F(6));
            } else {
                sp24.set(6469.0f, s_LinkPos->y, 6133.0f + aREG_F(6));
            }

            player->setPlayerPosAndAngle(&sp24, s_TargetAngle + 0x8000, 0);
            player->changeDemoMode(0x17, 1, 0, 0);

            mTimers[0] = 100;
            mTimers[2] = 125;
        }

        if (mTimers[0] < 70) {
            sp24 = current.pos;
            sp24.y += 100.0f + aREG_F(12);

            SetMoveCam(sp24, 700.0f + aREG_F(1), -100.0f + aREG_F(2), shape_angle.y, 0.2f + aREG_F(10), 200.0f + aREG_F(10));
        }
        break;
    case 3:
        if (mTimers[2] == 0) {
            mCamMode++;
            mCamCenter = current.pos;

            sp24.set(50.0f + TREG_F(4), 30.0f + TREG_F(5), 150.0f + TREG_F(6));
            cLib_offsetPos(&mCamEye, s_LinkPos, s_TargetAngle, &sp24);
            field_0x618 = mCamEye;

            mTimers[0] = 150;
            current.pos.set(1146.0f, 4907.0f, 7269.0f);
            old.pos = current.pos;
        }

        if (mTimers[0] != 0) {
            field_0x60c = current.pos;
        }

        SetMoveCam1(0.2f, 100.0f);
        break;
    case 4:
        field_0x60c = current.pos;
        SetMoveCam1(0.2f, 100.0f);
        break;
    case 5:
        sp24.set(0.0f, 0.0f, 0.0f);
        MTXCopy(daPy_getLinkPlayerActorClass()->getModelJointMtx(6), *calc_mtx);
        MtxPosition(&sp24, &sp24);
        field_0x60c = sp24;

        sp24.set(TREG_F(4), 10.0f + TREG_F(5), 200.0f + TREG_F(6));
        cLib_offsetPos(&field_0x618, s_LinkPos, fopAcM_GetShapeAngle_p(player)->y + 0x8000, &sp24);
        SetMoveCam1(0.5f + HREG_F(4), 1000.0f + HREG_F(5));

        if (mTimers[0] == 1) {
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
        }

        if (mTimers[1] == 0) {
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
            mCamMode++;
            mMode++;
            mTimers[0] = 150;
        }
        break;
    case 6:
        if (mTimers[0] == 0) {
            SetReleaseCam();
            mAction = ACTION_FLY;
            mSubAction = SUBACT_WAIT;
            mMode = 0;
            mType = TYPE_HYRULE_CASTLE;
        }

        field_0x60c = current.pos;
        SetMoveCam1(0.2f, 5.0f);
        break;
    }

    if (mCamMode > 1) {
        dComIfGp_getEvent()->onSkipFade();
        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 1);
    }
}

void daB_GG_c::Gn_DemoAction() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp34;

    switch (mMode) {
    case 0:
        if (mAnm != BCK_GG_WAIT) {
            SetAnm(BCK_GG_WAIT, 2, 5.0f, 1.0f);
        }
        break;
    case 1:
        if (mAnm != BCK_GGB_TAKE_OFF) {
            SetAnm(BCK_GGB_TAKE_OFF, 0, 5.0f, 1.0f);
            mSound.startCreatureSound(Z2SE_EN_GG_V_TAKE_OFF, 0, -1);
            speed.y = 0.0f;
            gravity = 0.0f;
            field_0x5c0 = 0.0f;
        } else if (mpModelMorf->checkFrame(18.0f)) {
            speed.y = 170.0f + TREG_F(0);
            field_0x5c0 = TREG_F(1);
            gravity = -4.2f + TREG_F(2);
        } else if (mpModelMorf->isStop()) {
            gravity = 0.0f;
            SetAnm(BCK_GGA_WAIT, 2, 5.0f, 1.0f);
            mMode++;
            mTimers[1] = 50;
            speed.y = 0.0f;
            field_0x5c0 = 0.0f;
        }
        break;
    case 2:
        if (mTimers[1] == 0 && mAnm != BCK_GGA_ATTACK_1) {
            SetAnm(BCK_GGA_ATTACK_1, 0, 5.0f, 1.0f);
        } else if (mpModelMorf->isStop() && mAnm == BCK_GGA_ATTACK_1) {
            SetAnm(BCK_GGA_ATTACK_2, 0, 5.0f, 1.0f);
            field_0x5ba = s_TargetAngle;
            current.angle.y = s_TargetAngle;
            field_0x5bc = 140.0f + TREG_F(3);
            speedF = field_0x5bc;
            field_0x5c0 = (100.0f + (s_LinkPos->y - current.pos.y)) / ((0.9f * fopAcM_searchPlayerDistanceXZ(this)) / speedF);
            speed.y = field_0x5c0;
            mMode++;
        }
        break;
    case 3:
        mSound.startCreatureSoundLevel(Z2SE_EN_GG_ATK_MOVE, 0, -1);
        field_0x5c0 = (100.0f + (s_LinkPos->y - current.pos.y)) / ((0.9f * fopAcM_searchPlayerDistanceXZ(this)) / speedF);
        field_0x5ba = s_TargetAngle;

        if (fopAcM_searchPlayerDistance(this) < (1400.0f + HREG_F(7))) {
            if (daPy_py_c::checkNowWolf()) {
                daPy_getPlayerActorClass()->changeDemoMode(0x10, 3, 0, 0);
            } else {
                daPy_getPlayerActorClass()->changeDemoMode(0x49, 0, 0, 0);
            }
        }

        if (fopAcM_searchPlayerDistance(this) < (900.0f + HREG_F(6))) {
            SetAnm(BCK_GGA_ATTACK_3, 0, 5.0f, 1.0f);
            field_0x5bc *= 0.5f;
            field_0x648 = current.pos.y;
            mMode++;
            mCamMode++;

            mTimers[0] = 20;
            mTimers[1] = 40;
        }
        break;
    case 4:
        mSound.startCreatureSoundLevel(Z2SE_EN_GG_ATK_MOVE, 0, -1);

        if (mpModelMorf->checkFrame(15.0f)) {
            field_0x5c0 = 50.0f;
        }

        if (mAnm == BCK_GGA_ATTACK_3 && mpModelMorf->isStop()) {
            SetAnm(BCK_GGA_ATTACK_4, 2, 5.0f, 1.0f);
        } else if ((current.pos.y - field_0x648) > 600.0f || mAcch.ChkRoofHit()) {
            SetAnm(BCK_GGA_WAIT, 2, 5.0f, 1.0f);
            current.pos.y -= 100.0f + HREG_F(9);
            old.pos.y = current.pos.y;

            mMode++;
            field_0x5c0 = -2.0f;
            field_0x5bc = 0.0f;
        }
        break;
    case 5:
        break;
    }

    s16 sp8 = s_TargetAngle - shape_angle.y;
    cXyz sp28;
    sp28 = *s_LinkPos - current.pos;
    cLib_addCalcAngleS2(&field_0x6c4, -cM_atan2s(sp28.y, JMAFastSqrt((sp28.x * sp28.x) + (sp28.z * sp28.z))), 2, 0x600);
    cLib_addCalcAngleS2(&field_0x6be, sp8, 2, 0x100);
    field_0x5ba = s_TargetAngle;
    shape_angle.y = current.angle.y;
}

void daB_GG_c::DemoAction() {
    switch (mSubAction) {
    case 0:
        St_CamAction();
        St_DemoAction();
        break;
    case 1:
        Md_CamAction();
        Md_DemoAction();
        break;
    case 2:
        Gn_DemoAction();
        Gn_CamAction();
        break;
    }

    if (mSubAction != 1 || mMode != 0) {
        cLib_addCalc2(&speed.y, field_0x5c0, 0.2f, 100.0f);
    }

    cLib_addCalc2(&speedF, field_0x5bc, 0.2f, 100.0f);
    cLib_addCalcAngleS2(&current.angle.y, field_0x5ba, 0x10, 0x1000);
}

void daB_GG_c::F_WaitAction() {
    switch (mMode) {
    case 0:
        SetAnm(BCK_GGA_WAIT, 2, 5.0f, 1.0f);
        field_0x5bc = 0.0f;
        field_0x5c0 = 0.0f;
        mTimers[0] = 50.0f + cM_rndF(50.0f);
        mMode++;

        mCcShieldSph.OnTgShield();
        mCcCyl.OnTgShield();
        mCcHookCyl.OffTgSetBit();
        break;
    case 1:
        if (mTimers[0] == 0 && !dComIfGp_event_runCheck()) {
            SetAction(ACTION_FLY, SUBACT_MOVE, 0);
        }
        break;
    }

    cXyz sp8 = *s_LinkPos - current.pos;
    cLib_addCalcAngleS2(&field_0x6c4, -cM_atan2s(sp8.y, JMAFastSqrt((sp8.x * sp8.x) + (sp8.z * sp8.z))), 0x10, 0x600);
    cLib_addCalcAngleS2(&field_0x6be, (cLib_targetAngleY(&current.pos, s_LinkPos) - shape_angle.y), 0x10, 0x100);

    if (field_0x6be > 0x2000) {
        field_0x6be = 0x2000;
    } else if (field_0x6be < -0x2000) {
        field_0x6be = -0x2000;
    }
}

void daB_GG_c::F_FookChk() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz* hookshot_top = player->getHookshotTopPos();

    if (hookshot_top != NULL) {
        s16 temp_r26 = (s_TargetAngle - fopAcM_GetShapeAngle_p(player)->y) + 0x8000;
        if (current.pos.abs(*hookshot_top) < 1500.0f && daPy_getPlayerActorClass()->checkHookshotShootReturnMode() && !daPy_getPlayerActorClass()->checkHookshotReturnMode() && temp_r26 < 0x500 && temp_r26 > -0x500) {
            mMode = 2;
            mTimers[1] = 30;
            mTimers[2] = 50;

            field_0x5bc = 20.0f;
            speedF = 30.0f;
            mModelPlaySpeed = 1.8f;
            field_0x644 = 1.4f;
            field_0x5c0 = 2.0f;
            speed.y = -30.0f;

            if ((int)cM_rndF(100.0f) > 50) {
                current.angle.y = s_TargetAngle + 0x4000;
                field_0x5b7 = 1;
            } else {
                current.angle.y = s_TargetAngle - 0x4000;
                field_0x5b7 = 0;
            }

            field_0x5ba = current.angle.y;
        }
    }
}

void daB_GG_c::F_MoveAction() {
    cXyz sp34;
    cXyz sp28;
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (mMode) {
    case 0:
        if (mType == TYPE_L7_MBOSS) {
            mTimers[0] = 200.0f + cM_rndF(100.0f);
        } else {
            mTimers[0] = 200.0f + cM_rndF(200.0f);

            if (s_dis > 5000.0f) {
                mSound.setLinkSearch(false);
            } else {
                mSound.setLinkSearch(true);
            }
        }

        mMode++;
        mCcHookCyl.OffTgSetBit();
        break;
    case 1:
        if (mTimers[1] == 0) {
            field_0x5bc = 5.0f + cM_rndF(5.0f);
            mTimers[1] = 40.0f + cM_rndF(40.0f);

            sp34 = *s_LinkPos - current.pos;

            if (mType == TYPE_HYRULE_CASTLE) {
                sp34.x += cM_rndFX(1550.0f);
                sp34.z += cM_rndFX(1550.0f);
            } else {
                sp34.x += 1500.0f + cM_rndFX(1550.0f);
                sp34.z += 1500.0f + cM_rndFX(1550.0f);
            }

            field_0x5ba = cM_atan2s(sp34.x, sp34.z);
            gravity = 0.0f;
        }

        if (mTimers[2] == 0) {
            mTimers[2] = 40.0f + cM_rndF(40.0f);

            field_0x5c0 = cM_rndFX(5.0f);
            if (current.pos.y < 400.0f + BREG_F(10)) {
                field_0x5c0 = 8.0f + BREG_F(9);
            }

            if (strcmp("D_MN07", dComIfGp_getStartStageName()) == 0 && dComIfGp_getStartStageRoomNo() == 15 && s_LinkPos->y > 1500.0f) {
                if (current.pos.y > 1500.0f) {
                    field_0x5c0 = -3.0f;
                } else if (current.pos.y < 500.0f) {
                    field_0x5c0 = 3.0f;
                }
            } else if (current.pos.y - s_LinkPos->y < 500.0f) {
                field_0x5c0 = 3.0f;
            } else if (current.pos.y - s_LinkPos->y > 800.0f) {
                field_0x5c0 = -3.0f;
            }

            field_0x644 = 1.0f + (field_0x5c0 / 60.0f);
        }

        if (s_LinkPos->absXZ(current.pos) < 1000.0f + BREG_F(6)) {
            field_0x5bc = 10.0f;

            if (field_0x5b7 != 0) {
                field_0x5ba = s_TargetAngle - 0x5000;
            } else {
                field_0x5ba = s_TargetAngle + 0x5000;
            }

            mTimers[1] = 20;
        } else if (s_LinkPos->absXZ(current.pos) < 500.0f) {
            field_0x5bc = 10.0f;

            if (field_0x5b7 != 0) {
                field_0x5ba = s_TargetAngle - 0x7000;
            } else {
                field_0x5ba = s_TargetAngle + 0x7000;
            }

            mTimers[1] = 20;

            if (mAcch.ChkWallHit()) {
                if (field_0x5b7 != 0) {
                    field_0x5ba = s_TargetAngle - 0x3000;
                } else {
                    field_0x5ba = s_TargetAngle + 0x3000;
                }
            }
        }

        F_FookChk();

        if ((strcmp("D_MN07", dComIfGp_getStartStageName()) != 0 || dComIfGp_getStartStageRoomNo() != 15 || !(s_LinkPos->y > 1500.0f)) && mTimers[0] == 0 && (mType != TYPE_HYRULE_CASTLE || !other_bg_check(this, player))) {
            if (mType == TYPE_L7_M && s_W_Action == 2) {
                mTimers[0] = 100;
            } else if (!dComIfGp_event_runCheck()) {
                SetAction(ACTION_FLY, SUBACT_ATTACK, 10);
            } else {
                mTimers[0] = 200;
            }
        }
        break;
    case 2:
        if (field_0x5b7 != 0) {
            s_TargetAngle -= 0x1500;
        } else {
            s_TargetAngle += 0x1500;
        }

        if (mTimers[1] == 0) {
            field_0x644 = 1.0f;
        }

        if (mTimers[2] == 0) {
            mMode = 1;
        }

        cLib_addCalcAngleS2(&field_0x6be, 0, 1, 0x100);
        break;
    }

    sp34 = *s_LinkPos - current.pos;
    cLib_addCalcAngleS2(&field_0x6c4, -cM_atan2s(sp34.y, JMAFastSqrt((sp34.x * sp34.x) + (sp34.z * sp34.z))), 0x10, 0x600);
    s16 sp8 = cLib_targetAngleY(&current.pos, s_LinkPos) - shape_angle.y;
    cLib_addCalcAngleS2(&field_0x6be, sp8, 0x10, 0x100);

    if (field_0x6be > 0x2000) {
        field_0x6be = 0x2000;
    } else if (field_0x6be < -0x2000) {
        field_0x6be = -0x2000;
    }

    if (mType == TYPE_L7_MBOSS) {
        if (current.pos.x > 1200.0f) {
            current.pos.x = 1200.0f;
        } else if (current.pos.x < -1200.0f) {
            current.pos.x = -1200.0f;
        } else if (current.pos.z < -1200.0f) {
            current.pos.z = -1200.0f;
        } else if (current.pos.z > 1200.0f) {
            current.pos.z = 1200.0f;
        }
    }
}

void daB_GG_c::F_LV7_W_MoveAction() {
    cXyz sp20;
    cXyz sp14;
    daPy_getPlayerActorClass();

    switch (mMode) {
    case 0:
        mTimers[0] = 200.0f + cM_rndF(200.0f);
        mMode++;
        field_0x644 = 1.0f;
        mCcHookCyl.OffTgSetBit();
        break;
    case 1:
        F_FookChk();

        if ((strcmp("D_MN07", dComIfGp_getStartStageName()) != 0 || dComIfGp_getStartStageRoomNo() != 15 || !(s_LinkPos->y > 2350.0f)) && mTimers[0] == 0) {
            if (s_M_Action != 2) {
                if (!dComIfGp_event_runCheck()) {
                    SetAction(ACTION_FLY, SUBACT_ATTACK, 10);
                } else {
                    mTimers[0] = 200;
                }
            } else {
                mTimers[0] = 100;
            }
        }
        break;
    case 2:
        if (field_0x5b7 != 0) {
            s_TargetAngle -= 0x1500;
        } else {
            s_TargetAngle += 0x1500;
        }

        if (mTimers[1] == 0) {
            field_0x5bc = 0.0f;
            mMode = 1;
        }

        cLib_addCalcAngleS2(&field_0x6be, 0, 1, 0x100);
        break;
    }

    sp20 = *s_LinkPos - current.pos;
    cLib_addCalcAngleS2(&field_0x6c4, -cM_atan2s(sp20.y, JMAFastSqrt((sp20.x * sp20.x) + (sp20.z * sp20.z))), 0x10, 0x600);
    cLib_addCalcAngleS2(&field_0x6be, (cLib_targetAngleY(&current.pos, s_LinkPos) - shape_angle.y), 0x10, 0x100);

    if (field_0x6be > 0x2000) {
        field_0x6be = 0x2000;
    } else if (field_0x6be < -0x2000) {
        field_0x6be = -0x2000;
    }
}

void daB_GG_c::F_AttackAction() {
    switch (mMode) {
    case 0:
        if (mAnm != BCK_GGA_ATTACK_1) {
            for (s16 i = 0; i < 3; i++) {
                mCcSph[i].OnAtSetBit();
            }

            SetAnm(BCK_GGA_ATTACK_1, 0, 5.0f, 1.0f);
            field_0x5ba = s_TargetAngle + 0x8000;
            field_0x5c0 = 10.0f;

            if (field_0x5b6 != 0) {
                field_0x5ba = s_TargetAngle;
            }
        } else if (mAnm == BCK_GGA_ATTACK_1 && mpModelMorf->isStop()) {
            mMode++;
            SetAnm(BCK_GGA_ATTACK_2, 2, 5.0f, 1.0f);
            field_0x5ba = s_TargetAngle;
            current.angle.y = s_TargetAngle;

            f32 var_f31 = fopAcM_searchPlayerDistanceXZ(this);
            if (var_f31 < 40.0f) {
                var_f31 = 40.0f;
            }
            field_0x5bc = 60.0f * (var_f31 / 1000.0f);

            speedF = field_0x5bc;
            field_0x5c0 = (100.0f + (s_LinkPos->y - current.pos.y)) / ((0.9f * var_f31) / speedF);
            speed.y = field_0x5c0;
            fopAcM_OffStatus(this, 0x80000);
        }
        break;
    case 1: {
        mSound.startCreatureSoundLevel(Z2SE_EN_GG_ATK_MOVE, 0, -1);

        f32 var_f31 = fopAcM_searchPlayerDistanceXZ(this);
        if (var_f31 < 40.0f) {
            var_f31 = 40.0f;
        }

        field_0x5c0 = (100.0f + (s_LinkPos->y - current.pos.y)) / ((0.9f * var_f31) / speedF);
        field_0x5ba = cLib_targetAngleY(&current.pos, s_LinkPos);

        if (fopAcM_searchPlayerDistance(this) < 600.0f || mAcch.ChkGroundHit() || mAcch.ChkWallHit()) {
            SetAnm(BCK_GGA_ATTACK_3, 0, 5.0f, 1.0f);
            mMode++;
            field_0x5bc *= 0.5f;
            field_0x648 = current.pos.y;
        }

        speedF = l_HIO.fly_attack_speed * field_0x5bc;
        speed.y = l_HIO.fly_attack_speed * field_0x5c0;
        break;
    }
    case 2:
        mSound.startCreatureSoundLevel(Z2SE_EN_GG_ATK_MOVE, 0, -1);

        if (mpModelMorf->checkFrame(15.0f)) {
            field_0x5c0 = 60.0f;
        }

        if (mAnm == BCK_GGA_ATTACK_3 && mpModelMorf->isStop()) {
            SetAnm(BCK_GGA_ATTACK_4, 2, 5.0f, 1.0f);

            for (int i = 0; i < 3; i++) {
                mCcSph[i].OffAtSetBit();
            }
        } else if (current.pos.y - field_0x648 > 500.0f || (field_0x650 == current.pos.y - field_0x648 && mAnm == BCK_GGA_ATTACK_4)) {
            mCcHookCyl.OnTgSetBit();
            fopAcM_OnStatus(this, 0x80000);

            if (field_0x5b6 != 0) {
                SetAnm(BCK_GGA_ATTACK_2, 2, 5.0f, 1.0f);
                SetAction(ACTION_F_A, SUBACT_MOVE, 2);
                field_0x5bc = 70.0f;

                s16 i;
                s16 sp8 = 0;
                for (i = 0; i < 3; i++) {
                    if (in_pos[sp8].absXZ(current.pos) > in_pos[i].absXZ(current.pos)) {
                        sp8 = i;
                    }
                }

                mInOutPosIdx = sp8;
                field_0x5ba = cLib_targetAngleY(&current.pos, &in_pos[mInOutPosIdx]);

                mCcCyl.OffTgSetBit();
                mCcShieldSph.OffTgSetBit();
                mCcHookCyl.OffTgSetBit();
            } else {
                SetAnm(BCK_GGA_WAIT, 2, 5.0f, 1.0f);
                SetAction(ACTION_FLY, SUBACT_MOVE, 0);

                field_0x5c0 = 0.0f;
                field_0x5bc = 0.0f;

                mCcShieldSph.OnTgShield();
                mCcCyl.OnTgShield();
            }
        }

        if (mAnm == BCK_GGA_ATTACK_4 || mpModelMorf->getFrame() > 15.0f) {
            field_0x5ba = current.angle.y;
            s_TargetAngle = field_0x5ba;
        }

        field_0x650 = current.pos.y - field_0x648;
        break;
    case 10:
        if (mAnm != BCK_GGA_PREPARATION) {
            mCcHookCyl.OnTgSetBit();
            SetAnm(BCK_GGA_PREPARATION, 2, 5.0f, 1.0f);

            fopAcM_OffStatus(this, 0x200000);
            fopAcM_OnStatus(this, 0x80000);

            if (field_0x6ba != 0) {
                mTimers[0] = l_HIO.fly_attack_wait_time * 0.5f;
            } else {
                mTimers[0] = l_HIO.fly_attack_wait_time;
            }

            field_0x5bc = 0.0f;
            field_0x5c0 = 0.0f;

            for (int i = 0; i < 3; i++) {
                if (mType == TYPE_L7_MBOSS) {
                    mCcSph[i].SetAtAtp(4);
                } else {
                    mCcSph[i].SetAtAtp(2);
                }

                mCcSph[i].OnAtSetBit();
                mCcSph[i].SetAtSpl((dCcG_At_Spl) 7);
            }
        } else if (mAnm == BCK_GGA_PREPARATION) {
            field_0x6ba = 0;

            if (mTimers[0] == 0) {
                mMode = 0;
            }

            if (strcmp("D_MN07", dComIfGp_getStartStageName()) == 0 && dComIfGp_getStartStageRoomNo() == 15 && s_LinkPos->y > 1500.0f) {
                SetAction(ACTION_FLY, SUBACT_MOVE, 0);
                SetAnm(BCK_GGA_WAIT, 2, 5.0f, 1.0f);
            }
        }
        break;
    }

    cLib_addCalc2(&speedF, field_0x5bc, 0.8f, 1000.0f);
    cLib_addCalc2(&speed.y, field_0x5c0, 0.8f, 1000.0f);
    cLib_addCalcAngleS2(&field_0x6c4, 0, 0x10, 0x600);
    cLib_addCalcAngleS2(&field_0x6be, 0, 0x10, 0x1000);

    F_AtHit();

    if (mType == TYPE_L7_MBOSS) {
        if (current.pos.x > 1200.0f) {
            current.pos.x = 1200.0f;
        } else if (current.pos.x < -1200.0f) {
            current.pos.x = -1200.0f;
        } else if (current.pos.z < -1200.0f) {
            current.pos.z = -1200.0f;
        } else if (current.pos.z > 1200.0f) {
            current.pos.z = 1200.0f;
        }
    }
}

void daB_GG_c::F_DamageAction() {
    cXyz sp30;
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (mMode) {
    case 0:
        mCcHookCyl.OnTgSetBit();
        SetAnm(BCK_GGA_GUARD, 0, 1.0f, 1.0f);
        field_0x5c0 = 0.0f;
        field_0x5bc = 0.0f;
        mMode += 2;
        break;
    case 1:
        SetAnm(BCK_GGA_GUARD, 0, 1.0f, 1.0f);
        current.angle.y = s_TargetAngle + 0x8000;
        field_0x5bc = 40.0f;
        field_0x5c0 = -40.0f;
        mMode++;
        break;
    case 2:
        if (mpModelMorf->isStop()) {
            SetAction(ACTION_FLY, SUBACT_ATTACK, 10);
            mCcShieldSph.OnTgShield();
            mCcCyl.OnTgShield();
            field_0x6ba = 1;
        }
        break;
    case 10:
        if (mAnm == BCK_GGB_BACK_STEP || mAnm == BCK_GGA_FS_OFF) {
            mAction = ACTION_GROUND;
            mSubAction = SUBACT_DAMAGE;
            mMode = 10;
            speedF = 0.0f;
            field_0x5bc = 0.0f;

            if (field_0x5b6 != 0) {
                mTimers[2] = 0.8f * l_HIO.takeoff2_time;
            } else {
                mTimers[2] = 0.8f * l_HIO.takeoff_time;
            }

            if (field_0x5ce != 0xFF && dComIfGs_isSwitch(142, fopAcM_GetRoomNo(this))) {
                dComIfGs_offSwitch(142, fopAcM_GetRoomNo(this));
            }
        } else if (mAnm != BCK_GGA_FS_HIT && mAnm != BCK_GGA_PULL) {
            SetAnm(BCK_GGA_FS_HIT, 0, 5.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_GG_V_PULL, -1);
            gravity = -9.0f;
            dComIfGs_onZoneSwitch(31, fopAcM_GetRoomNo(this));
        } else if (mAnm == BCK_GGA_FS_HIT && mpModelMorf->isStop()) {
            SetAnm(BCK_GGA_PULL, 2, 5.0f, 1.0f);
            SpeedClear();
            mTimers[0] = 1;
        }

        if (mTimers[0] == 1 && mAnm == BCK_GGA_PULL) {
            mMode++;
            fopAcM_cancelHookCarryNow(this);
            fopAcM_OffStatus(this, 0x80000);
            fopAcM_OnStatus(this, 0x200000);
            mTimers[0] = 5;
        }
        break;
    case 11:
        if (!daPy_getPlayerActorClass()->checkHookshotReturnMode()) {
            gravity = -9.0f;
            SetAnm(BCK_GGA_FS_OFF, 0, 5.0f, 1.0f);
            speed.y = 0.0f;
            speedF = 0.0f;
            field_0x5bc = 0.0f;
            mMode++;

            for (int i = 0; i < 3; i++) {
                mCcSph[i].SetAtSpl((dCcG_At_Spl) 0);
                mCcSph[i].OffAtSetBit();
            }

            field_0x5cc = 0;
            field_0x648 = s_LinkPos->y - 50.0f;
            
            mCcCyl.OffTgShield();
            mCcShieldSph.OffTgSetBit();
            mCcShieldSph.OffTgShield();

            s_M_Action = 0;
            s_W_Action = 0;

            fopAcM_OffStatus(this, 0x80000);
            fopAcM_OnStatus(this, 0x200000);
        } else {
            cXyz* hookshot_top = player->getHookshotTopPos();
            cXyz sp24(0.0f, 0.0f, 0.0f);
            cXyz sp18;

            mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0xB));
            mDoMtx_stack_c::multVec(&sp24, &sp24);
            sp24 = *hookshot_top;
            sp24.y -= 100.0f + BREG_F(7);
            cLib_chasePos(&current.pos, sp24, 120.0f);

            if (mAcch.ChkWallHit() && mTimers[0] == 0) {
                SetAction(ACTION_FLY, SUBACT_MOVE, 0);
                fopAcM_OffStatus(this, 0x200000);
                fopAcM_OnStatus(this, 0x80000);

                SetAnm(BCK_GGA_WAIT, 2, 5.0f + BREG_F(12), 1.0f);

                mCcCyl.OnTgShield();
                mCcShieldSph.OnTgShield();
                mCcShieldSph.OnTgSetBit();

                speed.y = 0.0f;
                field_0x644 = 1.2f;
                gravity = 0.0f;
            }
        }

        cLib_chaseAngleS(&field_0x6c4, 0, 0x2000);
        break;
    case 12:
        cLib_chaseAngleS(&field_0x6c4, 0, 0x2000);

        if (mpModelMorf->isStop() && mAcch.ChkGroundHit()) {
            field_0x648 = current.pos.y;
            gravity = -9.0f;
            mMode++;
            speedF = 0.0f;
            field_0x5bc = 0.0f;

            if (field_0x5b6 != 0) {
                mTimers[2] = 0.8f * l_HIO.takeoff2_time;
            } else {
                mTimers[2] = 0.8f * l_HIO.takeoff_time;
            }

            if (field_0x5ce != 0xFF && dComIfGs_isSwitch(142, fopAcM_GetRoomNo(this))) {
                dComIfGs_offSwitch(142, fopAcM_GetRoomNo(this));
            }
        } else if (current.pos.y < (field_0x648 - 50.0f)) {
            SetAction(ACTION_FLY, SUBACT_MOVE, 0);
            fopAcM_OffStatus(this, 0x200000);
            fopAcM_OnStatus(this, 0x80000);

            SetAnm(BCK_GGA_WAIT, 2, 5.0f + BREG_F(12), 1.0f);

            mCcCyl.OnTgShield();
            mCcShieldSph.OnTgShield();
            mCcShieldSph.OnTgSetBit();

            speed.y = 0.0f;
            field_0x5c0 = 10.0f + BREG_F(11);
            field_0x644 = 1.2f;
            gravity = 0.0f;
        }
        break;
    case 13:
        if (mAnm != BCK_GGB_BACK_STEP) {
            SetAnm(BCK_GGB_BACK_STEP, 0, 5.0f, 1.0f);
            field_0x5ba = s_TargetAngle + 0x8000;
            current.angle.y = field_0x5ba;
        } else if (mAnm == BCK_GGB_BACK_STEP && mpModelMorf->checkFrame(5.0f)) {
            field_0x5bc = 50.0f;
            speedF = 50.0f;
            speed.y = 30.0f;
        } else if (mAnm == BCK_GGB_BACK_STEP && mpModelMorf->checkFrame(13.0f)) {
            speedF = 0.0f;
            speed.y = 0.0f;
            field_0x5bc = 0.0f;

            mSound.startCreatureSound(Z2SE_EN_GG_FOOTNOTE, 0, -1);

            mCcCyl.OffTgSetBit();
            mCcShieldSph.OffTgSetBit();

            SetAction(ACTION_GROUND, SUBACT_MOVE, 0);
        }
        break;
    }
}

void daB_GG_c::AttentionChk() {
    daPy_getPlayerActorClass();

    if (s_dis < 3000.0f) {
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        if (field_0x5b0 != 0 && mSubAction == SUBACT_ATTACK) {
            s16 temp_r0 = fopAcM_toPlayerShapeAngleY(this) + 0x8000;
            if (temp_r0 < 0x3000 && temp_r0 > -0x3000) {
                attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            } else {
                attention_info.flags = 0;
            }
        }
    } else {
        attention_info.flags = 0;
    }
}

void daB_GG_c::FlyAction() {
    s16 var_r29 = 0x1000;

    switch (mSubAction) {
    case SUBACT_WAIT:
        F_WaitAction();
        break;
    case SUBACT_MOVE:
        if (mType == TYPE_L7_MBOSS) {
            F_MoveAction();
        } else if (mType == TYPE_L7_W) {
            F_LV7_W_MoveAction();
            s_W_Action = mSubAction;
        } else if (mType == TYPE_L7_M) {
            F_MoveAction();
            s_M_Action = mSubAction;
        } else if (mType == TYPE_HYRULE_CASTLE) {
            F_MoveAction();
        }

        cLib_addCalc2(&mModelPlaySpeed, field_0x644, 0.2f, 10.0f);
        mpModelMorf->setPlaySpeed(mModelPlaySpeed);
        break;
    case SUBACT_ATTACK:
        F_AttackAction();
        if (mType == TYPE_L7_W || mType == TYPE_L7_M) {
            var_r29 = 0x500;
        }
        break;
    case SUBACT_DAMAGE:
        F_DamageAction();
        break;
    }

    AttentionChk();

    if (daPy_getPlayerActorClass()->checkWolfLock(this)) {
        daPy_getPlayerActorClass()->cancelWolfLock(this);
    }

    cLib_addCalc2(&speed.y, field_0x5c0, 0.2f, 100.0f);
    cLib_addCalc2(&speedF, field_0x5bc, 0.2f, 100.0f);
    cLib_addCalcAngleS2(&current.angle.y, field_0x5ba, 0x10, var_r29);
    cLib_addCalcAngleS2(&shape_angle.y, s_TargetAngle, 0x10, 0x1000);

    if (mSubAction != SUBACT_DAMAGE) {
        roofchk(this);
    }
}

bool daB_GG_c::F_A_TargetMove(cXyz param_0, u8 param_1) {
    field_0x5ba = cLib_targetAngleY(&current.pos, &param_0);
    field_0x5c0 = (param_0.y - current.pos.y) / (current.pos.absXZ(param_0) / speedF);
    speedF = field_0x5bc;
    speed.y = field_0x5c0;

    if (param_1 != 0) {
        Mtx inv;
        mDoMtx_inverse(mpModelMorf->getModel()->getBaseTRMtx(), inv);
        mDoMtx_stack_c::copy(inv);
        mDoMtx_stack_c::multVec(&param_0, &param_0);

        if (param_0.z < 0.0f) {
            return true;
        }
    } else {
        s16 temp_r28 = cLib_targetAngleY(&param_0, &out_pos[mInOutPosIdx]);
        s16 temp_r3 = cLib_targetAngleY(&param_0, &current.pos);
        if ((temp_r28 - temp_r3) < 0x4000 && (temp_r28 - temp_r3) > -0x4000) {
            return true;
        }
    }

    return false;
}

bool daB_GG_c::F_A_TargetMove(cXyz param_0) {
    field_0x5ba = cLib_targetAngleY(&current.pos, &param_0);
    field_0x5c0 = (param_0.y - current.pos.y) / (current.pos.absXZ(param_0) / speedF);
    speedF = field_0x5bc;
    speed.y = field_0x5c0;

    return way_bg_check2(this, current.pos, out_pos[mInOutPosIdx]) == false;
}

void daB_GG_c::F_A_MoveAction() {
    gravity = 0.0f;
    cXyz sp38(0.0f, 1000.0f, 3000.0f);
    mSound.startCreatureSoundLevel(Z2SE_EN_GG_ATK_MOVE, 0, -1);

    switch (mMode) {
    case 0:
        if (mTimers[0] == 0) {
            s16 new_idx;
            do {
                new_idx = cM_rndF(2.9f);
            } while (mInOutPosIdx == new_idx);

            mInOutPosIdx = new_idx;
            current.pos = out_pos[mInOutPosIdx];
            old.pos = current.pos;

            field_0x5bc = 70.0f;
            speedF = field_0x5bc;
            field_0x5c0 = (in_pos[mInOutPosIdx].y - current.pos.y) / (current.pos.absXZ(in_pos[mInOutPosIdx]) / speedF);
            speed.y = field_0x5c0;
            current.angle.y = cLib_targetAngleY(&out_pos[mInOutPosIdx], &in_pos[mInOutPosIdx]);
            mMode++;
        }
        break;
    case 1:
        if (F_A_TargetMove(in_pos[mInOutPosIdx], 1)) {
            int sp10 = cM_rndF(100.0f);

            s16 new_idx;
            do {
                new_idx = cM_rndF(2.9f);
            } while (mInOutPosIdx == new_idx);
            mInOutPosIdx = new_idx;

            if (sp10 > 70) {
                mMode++;
                field_0x5bc = 100.0f;
            } else {
                SpeedClear();
                SetAction(ACTION_FLY, SUBACT_ATTACK, 10);

                mCcCyl.OnTgSetBit();
                mCcShieldSph.OnTgSetBit();
                mCcHookCyl.OnTgSetBit();
            }
        }
        break;
    case 2:
        sp38 = in_pos[mInOutPosIdx];
        sp38.y += 400.0f;

        if (F_A_TargetMove(sp38)) {
            mMode++;
        }
        break;
    case 3:
        if (F_A_TargetMove(out_pos[mInOutPosIdx], 1)) {
            current.pos = s_centerpos;
            current.pos.y += 10000.0f;
            current.pos.x += 10000.0f;
            old.pos = current.pos;

            mMode = 0;
            mTimers[0] = 50;
            attention_info.flags = 0;
        }
        break;
    }

    if (field_0x5bc != 100.0f) {
        cLib_addCalcAngleS2(&current.angle.y, field_0x5ba, 2, 0x1000);
    } else {
        cLib_addCalcAngleS2(&current.angle.y, field_0x5ba, 9, 0x1000);
    }

    shape_angle.y = current.angle.y;

    if (current.pos.z > 1200.0f) {
        current.pos.z = 1200.0f;
    }
}

void daB_GG_c::F_A_WaitAction() {
    Md_DemoAction();
}

void daB_GG_c::F_A_Action() {
    daPy_py_c::cancelBoomerangLockActor(this);
 
    switch (mSubAction) {
    case SUBACT_WAIT:
        F_A_WaitAction();
        break;
    case SUBACT_MOVE:
        F_A_MoveAction();
        cLib_addCalc2(&speed.y, field_0x5c0, 0.2f, 100.0f);
        break;
    }

    if (daPy_getPlayerActorClass()->checkWolfLock(this)) {
        daPy_getPlayerActorClass()->cancelWolfLock(this);
    }

    cLib_addCalc2(&speedF, field_0x5bc, 0.2f, 100.0f);

    cXyz sp8 = *s_LinkPos - current.pos;
    cLib_addCalcAngleS2(&field_0x6c4, (-cM_atan2s(sp8.y, JMAFastSqrt((sp8.x * sp8.x) + (sp8.z * sp8.z))) - shape_angle.x), 0x10, 0x600);
}

void daB_GG_c::FookChk() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz* hookshot_top = player->getHookshotTopPos();

    if (hookshot_top != NULL) {
        s16 temp_r27 = (s_TargetAngle - fopAcM_GetShapeAngle_p(player)->y) + 0x8000;
        if (hookshot_top->absXZ(current.pos) > 100.0f && daPy_getPlayerActorClass()->checkHookshotShootReturnMode() && !daPy_getPlayerActorClass()->checkHookshotReturnMode() && temp_r27 < 0x500 && temp_r27 > -0x500) {
            SpeedClear();
            mMode = 5;
            SetAnm(BCK_GGB_PULL, 2, 5.0f, 1.0f);
        }
    }
}

void daB_GG_c::CutChk() {
    int rnd = cM_rndF(100.0f);
    u32 cut_type = daPy_getPlayerActorClass()->getCutType();

    if (s_dis < 400.0f && mMode != 4 && rnd < (20.0f + XREG_F(0)) && cut_type != 0 && !mCcCyl.ChkTgHit() && !mCcShieldSph.ChkTgHit() && cut_type != daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
        SetAction(ACTION_GROUND, SUBACT_MOVE, 0);
    }
}

void daB_GG_c::G_MoveAction() {
    CutChk();

    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz& player_pos = fopAcM_GetPosition(player);

    switch (mMode) {
    case 0:
        if (mAnm != BCK_GGB_BACK_STEP) {
            SetAnm(BCK_GGB_BACK_STEP, 0, 5.0f, 1.0f);
            field_0x5ba = s_TargetAngle + 0x8000;
            current.angle.y = field_0x5ba;

            mCcCyl.OffTgSetBit();
            mCcShieldSph.OffTgSetBit();
        } else if (mAnm == BCK_GGB_BACK_STEP && mpModelMorf->checkFrame(5.0f)) {
            field_0x5bc = 50.0f;
            speedF = 50.0f;
            speed.y = 30.0f;
        } else if (mAnm == BCK_GGB_BACK_STEP && mpModelMorf->checkFrame(13.0f)) {
            speedF = 0.0f;
            speed.y = 0.0f;
            field_0x5bc = 0.0f;

            mSound.startCreatureSound(Z2SE_EN_GG_FOOTNOTE, 0, -1);
        } else if (mAnm == BCK_GGB_BACK_STEP && mpModelMorf->isStop()) {
            SpeedClear();
            if (current.pos.y < (field_0x648 - 10.0f)) {
                mCcCyl.OnTgSetBit();
                mCcShieldSph.OnTgSetBit();
                mCcCyl.OnTgShield();
                mCcShieldSph.OnTgShield();

                SetAction(ACTION_GROUND, SUBACT_MOVE, 3);
                FallChk();
            } else if (current.pos.y < (field_0x648 - 10.0f) && mAcch.ChkGroundHit() && mType == TYPE_L7_MBOSS) {
                mCcCyl.OnTgSetBit();
                mCcShieldSph.OnTgSetBit();
                mCcCyl.OnTgShield();
                mCcShieldSph.OnTgShield();

                mMode = 4;
            } else if (mTimers[2] == 0 && field_0x5b0 != 0) {
                SetAction(ACTION_F_A, SUBACT_WAIT, 0);
            } else if (mTimers[2] == 0) {
                mCcCyl.OnTgSetBit();
                mCcShieldSph.OnTgSetBit();
                mCcCyl.OnTgShield();
                mCcShieldSph.OnTgShield();

                mMode = 4;
            } else {
                mCcCyl.OnTgSetBit();
                mCcShieldSph.OnTgSetBit();
                mCcCyl.OnTgShield();
                mCcShieldSph.OnTgShield();

                SetAction(ACTION_GROUND, SUBACT_MOVE, 2);
            }
        } else {
            FallChk();
        }
        break;
    case 1:
        if (mAnm != BCK_GGB_SIDE_STEP) {
            SetAnm(BCK_GGB_SIDE_STEP, 2, 5.0f, 1.0f);
            int sp1C = cM_rndF(100.0f);
            field_0x5bc = 5.0f;
        }

        if (mTimers[0] == 0 && mpModelMorf->checkFrame(0.0f)) {
            mMode = 3;
        }

        if (mpModelMorf->getPlaySpeed() > 0.0f) {
            field_0x5ba = s_TargetAngle + 0x3000;
        } else {
            field_0x5ba = s_TargetAngle - 0x3000;
        }

        if (mpModelMorf->checkFrame(13.0f) || mpModelMorf->checkFrame(1.0f)) {
            speedF = 0.0f;
            field_0x5bc = 0.0f;
        } else {
            field_0x5bc = 5.0f;
        }

        current.angle.y = field_0x5ba;
        FookChk();

        if (s_dis < 300.0f && (s_TargetAngle - shape_angle.y) < 0x2000 && (s_TargetAngle - shape_angle.y) > -0x2000) {
            SetAction(ACTION_GROUND, SUBACT_ATTACK, 0);
            speedF = 0.0f;
            field_0x5bc = 0.0f;
        }

        FallChk();
        break;
    case 2:
        if (mAnm != BCK_GGB_WALK) {
            SetAnm(BCK_GGB_WALK, 2, 5.0f, 1.0f);
            mTimers[0] = 50.0f + cM_rndF(50.0f);
            int sp1C = cM_rndF(100.0f);
            field_0x5bc = 10.0f;
            current.angle.y = s_TargetAngle;
        }

        field_0x5ba = s_TargetAngle;

        if (mTimers[0] == 0 && mAnm == BCK_GGB_WALK && s_dis < 400.0f) {
            mMode = 3;
        }

        FookChk();

        if (s_dis < 300.0f && (s_TargetAngle - shape_angle.y) < 0x2000 && (s_TargetAngle - shape_angle.y) > -0x2000) {
            SetAction(ACTION_GROUND, SUBACT_ATTACK, 0);
        }

        FallChk();
        break;
    case 3:
        if (mAnm != BCK_GGB_WAIT_B) {
            SetAnm(BCK_GGB_WAIT_B, 2, 5.0f, 1.0f);
            mTimers[0] = 10.0f + cM_rndF(10.0f);
            speedF = 0.0f;
            field_0x5bc = 0.0f;
            current.angle.y = 0;
        }

        if (mTimers[0] == 0) {
            mMode = 1;
            if (s_dis < 500.0f) {
                mTimers[0] = 100.0f + cM_rndF(100.0f);

                int rnd = cM_rndF(100.0f);
                if (rnd > 50) {
                    SetAnm(BCK_GGB_SIDE_STEP, 2, 5.0f, 1.0f);
                } else {
                    SetAnm(BCK_GGB_SIDE_STEP, 2, 5.0f, -1.0f);
                }
            } else {
                field_0x5bc = 4.0f;
                mMode = 2;
            }
        }

        FookChk();
        FallChk();
        break;
    case 4:
        eyePos = current.pos;
        eyePos.y += 300.0f;

        attention_info.position = eyePos;
        attention_info.position.y += 50.0f;

        if (mAnm != BCK_GGB_TAKE_OFF) {
            SetAnm(BCK_GGB_TAKE_OFF, 0, 5.0f, 1.0f);
            mSound.startCreatureSound(Z2SE_EN_GG_V_TAKE_OFF, 0, -1);

            mCcCyl.OffTgSetBit();
            mCcShieldSph.OffTgSetBit();
            mCcHookCyl.OffTgSetBit();

            SpeedClear();
            gravity = -2.0f;

            if (field_0x5ce != 0xFF && !dComIfGs_isSwitch(142, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(142, fopAcM_GetRoomNo(this));
            }

            if (daPy_getPlayerActorClass()->checkWolfLock(this)) {
                daPy_getPlayerActorClass()->cancelWolfLock(this);
            }
        } else {
            if (mpModelMorf->checkFrame(18.0f)) {
                speed.y = 70.0f;
                field_0x5c0 = 0.0f;
                gravity = -4.2f;
            } else if (mpModelMorf->isStop()) {
                SetAction(ACTION_FLY, SUBACT_MOVE, 0);
                mTimers[0] = 0;
                mTimers[1] = 0;
                gravity = 0.0f;

                fopAcM_OffStatus(this, 0x200000);
                fopAcM_OnStatus(this, 0x80000);

                SetAnm(BCK_GGA_WAIT, 2, 5.0f, 1.0f);

                mCcCyl.OnTgShield();
                mCcShieldSph.OnTgShield();
                field_0x644 = 1.0f;

                for (int i = 0; i < 3; i++) {
                    mCcSph[i].SetAtSpl((dCcG_At_Spl) 7);
                }

                mCcCyl.OnTgSetBit();
                mCcShieldSph.OnTgSetBit();
            }
        }
        break;
    case 5:
        if (!daPy_getPlayerActorClass()->checkHookshotShootReturnMode()) {
            mMode = 3;
        }
        break;
    }
}

void daB_GG_c::FallChk() {
    if (current.pos.y < (field_0x648 - 10.0f) && field_0x5b6 != 0) {
        if (!mAcch.ChkGroundHit()) {
            SetAction(ACTION_FLY, SUBACT_MOVE, 0);
            fopAcM_OffStatus(this, 0x200000);
            fopAcM_OnStatus(this, 0x80000);

            SetAnm(BCK_GGA_WAIT, 2, 5.0f + BREG_F(12), 1.0f);

            mCcCyl.OnTgShield();
            mCcShieldSph.OnTgShield();

            for (int i = 0; i < 3; i++) {
                mCcSph[i].SetAtSpl((dCcG_At_Spl) 7);
            }

            speed.y = 0.0f;
            field_0x5c0 = 10.0f + BREG_F(11);
            field_0x644 = 1.5f;
            mModelPlaySpeed = 1.8f;
            mTimers[2] = 50;
            gravity = 0.0f;

            if (field_0x5ce != 0xFF && !dComIfGs_isSwitch(142, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(142, fopAcM_GetRoomNo(this));
            }
        } else {
            SetAnm(BCK_GGB_WAIT_B, 2, 5.0f, 1.0f);
            SetAction(ACTION_F_A, SUBACT_WAIT, 0);
            mTimers[0] = 0;
        }

        mCcCyl.OnTgSetBit();
        mCcShieldSph.OnTgSetBit();
    } else if (current.pos.y < (field_0x648 - 10.0f) && mAcch.ChkGroundHit()) {
        SpeedClear();
        SetAction(ACTION_GROUND, SUBACT_MOVE, 4);

        mCcCyl.OnTgSetBit();
        mCcShieldSph.OnTgSetBit();
    } else if (current.pos.y < (field_0x648 - 10.0f)) {
        SpeedClear();
        SetAction(ACTION_FLY, SUBACT_MOVE, 0);
        fopAcM_OffStatus(this, 0x200000);
        fopAcM_OnStatus(this, 0x80000);

        SetAnm(BCK_GGA_WAIT, 2, 5.0f + BREG_F(12), 1.0f);

        mCcCyl.OnTgShield();
        mCcShieldSph.OnTgShield();

        for (int i = 0; i < 3; i++) {
            mCcSph[i].SetAtSpl((dCcG_At_Spl) 7);
        }

        speed.y = 0.0f;
        field_0x5c0 = 10.0f + BREG_F(11);
        field_0x644 = 1.5f;
        mModelPlaySpeed = 1.8f;
        mTimers[2] = 50;
        gravity = 0.0f;

        if (field_0x5ce != 0xFF && !dComIfGs_isSwitch(142, fopAcM_GetRoomNo(this))) {
            dComIfGs_onSwitch(142, fopAcM_GetRoomNo(this));
        }

        mCcCyl.OnTgSetBit();
        mCcShieldSph.OnTgSetBit();
    } else if (mTimers[2] == 0 && field_0x5b6 != 0) {
        SetAction(ACTION_GROUND, SUBACT_MOVE, 0);

        mCcCyl.OffTgSetBit();
        mCcShieldSph.OffTgSetBit();
    } else if (mTimers[2] == 0) {
        SetAction(ACTION_GROUND, SUBACT_MOVE, 0);

        mCcCyl.OffTgSetBit();
        mCcShieldSph.OffTgSetBit();
    }
}

void daB_GG_c::G_AttackAction() {
    int rnd;
    switch (mMode) {
    case 0:
        SpeedClear();

        rnd = cM_rndF(100.0f);
        if (rnd > 70) {
            SetAnm(BCK_GGB_ATTACK_A, 0, 5.0f, 1.0f);

            for (int i = 0; i < 3; i++) {
                mCcSph[i].SetAtAtp(2);
            }
        } else if (rnd > 30) {
            SetAnm(BCK_GGB_ATTACK_B, 0, 5.0f, 1.0f);

            for (int i = 0; i < 3; i++) {
                mCcSph[i].SetAtAtp(2);
            }
        } else {
            SetAnm(BCK_GGB_ATTACK_C, 0, 1.0f, 1.0f);

            for (int i = 0; i < 3; i++) {
                mCcSph[i].OnAtSetBit();
                mCcSph[i].SetAtSpl((dCcG_At_Spl) 0xA);

                if (mType == TYPE_L7_MBOSS) {
                    mCcSph[i].SetAtAtp(2);
                } else {
                    mCcSph[i].SetAtAtp(1);
                }
            }
        }

        mMode++;
        break;
    case 1:
        if (mpModelMorf->checkFrame(2.0f) && mAnm == BCK_GGB_ATTACK_C) {
            for (int i = 0; i < 3; i++) {
                mCcSph[i].OnAtSetBit();
                mCcSph[i].SetAtSpl((dCcG_At_Spl) 0xA);
            }
        }

        if (mpModelMorf->checkFrame(20.0f) && mAnm != BCK_GGB_ATTACK_C) {
            for (int i = 0; i < 3; i++) {
                mCcSph[i].OnAtSetBit();
                mCcSph[i].SetAtSpl((dCcG_At_Spl) 0);
            }
        }

        if (mpModelMorf->checkFrame(37.0f) && mAnm == BCK_GGB_ATTACK_A) {
            for (int i = 0; i < 3; i++) {
                mCcSph[i].OffAtSetBit();
            }
        } else if (mpModelMorf->checkFrame(37.0f) && mAnm == BCK_GGB_ATTACK_B) {
            speedF = 8.0f;
            field_0x5bc = 0.0f;
        } else if (mpModelMorf->checkFrame(52.0f) && mAnm == BCK_GGB_ATTACK_B) {
            for (int i = 0; i < 3; i++) {
                mCcSph[i].OffAtSetBit();
            }
        } else if (mpModelMorf->checkFrame(11.0f) && mAnm == BCK_GGB_ATTACK_C) {
            for (int i = 0; i < 3; i++) {
                mCcSph[i].OffAtSetBit();
                mCcSph[i].SetAtAtp(2);
            }
        }

        if (mpModelMorf->isStop()) {
            mCcCyl.OnTgShield();
            mCcShieldSph.OnTgShield();

            rnd = cM_rndF(100.0f);
            if (rnd < 30 || mAnm == BCK_GGB_ATTACK_C) {
                SetAction(ACTION_GROUND, SUBACT_MOVE, 0);
            } else if (mAnm != BCK_GGB_ATTACK_C && rnd > 50) {
                SetAnm(BCK_GGB_ATTACK_C, 0, 1.0f, 1.0f);

                for (int i = 0; i < 3; i++) {
                    if (mType == TYPE_L7_MBOSS) {
                        mCcSph[i].SetAtAtp(2);
                    } else {
                        mCcSph[i].SetAtAtp(1);
                    }

                    mCcSph[i].OnAtSetBit();
                    mCcSph[i].SetAtSpl((dCcG_At_Spl) 0xA);
                }
            } else if (rnd > 30) {
                SetAction(ACTION_GROUND, SUBACT_MOVE, 1);
                mTimers[1] = 50;
            } else {
                SetAction(ACTION_GROUND, SUBACT_MOVE, 2);
                mTimers[1] = 50;
            }
        }

        if (mpModelMorf->getFrame() > 37.0f && mAnm == BCK_GGB_ATTACK_B) {
            cLib_addCalcAngleS2(&current.angle.y, field_0x5ba, 10, 0x1000);
            cLib_addCalcAngleS2(&shape_angle.y, s_TargetAngle, 10, 0x1000);
            cLib_addCalcAngleS2(&field_0x6be, (s_TargetAngle - shape_angle.y), 10, 0x100);
        }
        break;
    case 10:
        SpeedClear();
        SetAnm(BCK_GGB_ATTACK_C, 0, 1.0f, 1.0f);
        field_0x6c4 = 0;
        mMode = 1;

        for (int i = 0; i < 3; i++) {
            if (mType == TYPE_L7_MBOSS) {
                mCcSph[i].SetAtAtp(2);
            } else {
                mCcSph[i].SetAtAtp(1);
            }

            mCcSph[i].OnAtSetBit();
            mCcSph[i].SetAtSpl((dCcG_At_Spl) 0xA);
        }
    }

    F_AtHit();
}

void daB_GG_c::G_DamageAction() {
    cXyz sp38;
    if (mAnm != BCK_GGB_ATTACK_C && mAnm != BCK_GGB_GUARD) {
        for (int i = 0; i < 3; i++) {
            mCcSph[i].OffAtSetBit();
        }
    }

    switch (mMode) {
    case 0:
        CutChk();
        if (mAnm == BCK_GGB_GUARD && mpModelMorf->isStop()) {
            SetAction(ACTION_GROUND, SUBACT_MOVE, 1);
            speedF = 0.0f;

            if (s_dis < 300.0f) {
                int rnd = cM_rndF(100.0f);
                if (rnd <= 50) {
                    if (rnd > 20) {
                        mMode = 0;
                    } else {
                        SetAnm(BCK_GGB_ATTACK_C, 0, 1.0f, 1.0f);

                        for (int i = 0; i < 3; i++) {
                            if (mType == TYPE_L7_MBOSS) {
                                mCcSph[i].SetAtAtp(2);
                            } else {
                                mCcSph[i].SetAtAtp(1);
                            }

                            mCcSph[i].OnAtSetBit();
                            mCcSph[i].SetAtSpl((dCcG_At_Spl) 0xA);
                            field_0x65a = 0;
                        }

                        SetAction(ACTION_GROUND, SUBACT_ATTACK, 1);
                        HitChk();
                        mMode = 1;
                    }
                }

                u32 cut_type = daPy_getPlayerActorClass()->getCutType();
                if (cut_type == daPy_py_c::CUT_TYPE_TURN_LEFT || cut_type == daPy_py_c::CUT_TYPE_TURN_RIGHT || cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT || cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT) {
                    SetAnm(BCK_GGB_ATTACK_C, 0, 1.0f, 1.0f);

                    for (int i = 0; i < 3; i++) {
                        if (mType == TYPE_L7_MBOSS) {
                            mCcSph[i].SetAtAtp(2);
                        } else {
                            mCcSph[i].SetAtAtp(1);
                        }

                        mCcSph[i].OnAtSetBit();
                        mCcSph[i].SetAtSpl((dCcG_At_Spl) 0xA);
                        field_0x65a = 0;
                    }

                    SetAction(ACTION_GROUND, SUBACT_ATTACK, 1);
                    HitChk();
                    mMode = 1;
                }
            }

            field_0x5f0 = 0;
        } else {
            u32 cut_type = daPy_getPlayerActorClass()->getCutType();
            int rnd = cM_rndF(100.0f);

            if (field_0x5f0 > 1) {
                SetAnm(BCK_GGB_ATTACK_C, 0, 1.0f, 1.0f);
                for (int i = 0; i < 3; i++) {
                    if (mType == TYPE_L7_MBOSS) {
                        mCcSph[i].SetAtAtp(2);
                    } else {
                        mCcSph[i].SetAtAtp(1);
                    }

                    mCcSph[i].OnAtSetBit();
                    mCcSph[i].SetAtSpl((dCcG_At_Spl) 0xA);
                    field_0x65a = 0;
                }

                SetAction(ACTION_GROUND, SUBACT_ATTACK, 1);
                mCcCyl.OffTgShield();
                mCcShieldSph.OffTgShield();

                mMode = 1;
                field_0x5f0 = 0;
                speedF = 0.0f;
            } else if (cut_type != 0) {
                if (rnd < (10.0f + nREG_F(0)) && mpModelMorf->getFrame() > (5.0f + nREG_F(6))) {
                    SetAnm(BCK_GGB_ATTACK_C, 0, 1.0f, 1.0f);

                    for (int i = 0; i < 3; i++) {
                        if (mType == TYPE_L7_MBOSS) {
                            mCcSph[i].SetAtAtp(2);
                        } else {
                            mCcSph[i].SetAtAtp(1);
                        }

                        mCcSph[i].OnAtSetBit();
                        mCcSph[i].SetAtSpl((dCcG_At_Spl) 0xA);
                        field_0x65a = 0;
                        field_0x5f0 = 0;
                    }

                    SetAction(ACTION_GROUND, SUBACT_ATTACK, 1);
                    mCcCyl.OffTgShield();
                    mCcShieldSph.OffTgShield();

                    mMode = 1;
                    speedF = 0.0f;
                }
            }
        }
        break;
    case 1:
        if (mAnm != BCK_GGB_GUARD) {
            SetAnm(BCK_GGB_GUARD, 0, 1.0f, 1.0f);
            speedF = 30.0f;
            field_0x5bc = 0.0f;
            current.angle.y = s_TargetAngle + 0x8000;
        } else if (mAnm == BCK_GGB_GUARD && mpModelMorf->isStop()) {
            int sp30 = cM_rndF(100.0f);
            SetAction(ACTION_GROUND, SUBACT_MOVE, 1);
            speedF = 0.0f;
        }

        s_TargetAngle += (s16) 0x4000;
        break;
    case 2:
        cXyz* tg_hit_pos;
        s16 spA;
        tg_hit_pos = mCcCyl.GetTgHitPosP();
        spA = cLib_targetAngleY(&current.pos, tg_hit_pos);
        if ((spA - shape_angle.y) > 0) {
            if (mAnm != BCK_GGB_DAMAGE_R) {
                SetAnm(BCK_GGB_DAMAGE_R, 0, 5.0f, 1.0f);
            } else {
                SetAnm(BCK_GGB_DAMAGE_L, 0, 5.0f, 1.0f);
            }
        } else if (mAnm != BCK_GGB_DAMAGE_L) {
            SetAnm(BCK_GGB_DAMAGE_L, 0, 5.0f, 1.0f);
        } else {
            SetAnm(BCK_GGB_DAMAGE_R, 0, 5.0f, 1.0f);
        }

        SetAction(ACTION_GROUND, SUBACT_DAMAGE, 3);

        if (health < (s16)(field_0x560 * l_HIO.field_0x18) && field_0x5b0 == 0 &&
            mType == TYPE_L7_MBOSS)
        {
            mpModelMorf->setPlaySpeed(1.0f);
            SetAction(ACTION_DEMO, 1, 0);
            mCamMode = 0;
            attention_info.flags = 0;
        }

        field_0x5bc = 0.0f;
        speedF = -8.0f + yREG_F(0);
        current.angle.y = s_TargetAngle;
        shape_angle.y = current.angle.y;
        field_0x5ba = current.angle.y;

        if (health <= 0) {
            Z2GetAudioMgr()->subBgmStop();
            SetAction(ACTION_GROUND, SUBACT_DEATH, 0);
            attention_info.flags = 0;

            if (mType == TYPE_L7_MBOSS) {
                SetAnm(BCK_GGB_DIE, 0, 5.0f, 1.0f);
            } else {
                SetAnm(BCK_GG_DIE, 0, 5.0f, 1.0f);

                mCcCyl.OffTgSetBit();
                mCcShieldSph.OffTgSetBit();
                mCcHookCyl.OffTgSetBit();
            }

            mCamMode = 0;
        }
        break;
    case 3: {
        u8 cut_count = daPy_getPlayerActorClass()->getCutCount();
        u32 cut_type = daPy_getPlayerActorClass()->getCutType();

        if ((cut_type == daPy_py_c::CUT_TYPE_TURN_LEFT ||
             cut_type == daPy_py_c::CUT_TYPE_TURN_RIGHT ||
             cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT ||
             cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT) &&
            field_0x6bc > 0)
        {
            mCcCyl.OnTgShield();
            mCcShieldSph.OnTgShield();
            mCcShieldSph.OnTgSetBit();
        } else if (field_0x5e0 < 4) {
            mCcCyl.OffTgShield();
            mCcShieldSph.OffTgSetBit();
        } else {
            mCcCyl.OnTgShield();
            mCcShieldSph.OnTgShield();
            mCcShieldSph.OnTgSetBit();
        }

        if (mpModelMorf->isStop()) {
            field_0x6bc = 0;
            field_0x5e0 = 0;
            SetAction(ACTION_GROUND, SUBACT_MOVE, 0);
            mTimers[1] = 60;

            mCcCyl.OnTgShield();
            mCcShieldSph.OnTgShield();
            mCcShieldSph.OnTgSetBit();
        }

        FallChk();
        break;
    }
    case 4:
        sp38 = eyePos;
        sp38.y += 180.0f + NREG_F(7);
        setHeadLockPos(&sp38);

        if (mpModelMorf->isStop()) {
            field_0x5f0 = 0;
            mCcShieldSph.OnTgSetBit();
            mCcCyl.OnTgShield();
            mCcShieldSph.OnTgShield();

            if (s_dis < 300.0f) {
                SetAction(ACTION_GROUND, SUBACT_ATTACK, 0);
            } else {
                SetAction(ACTION_GROUND, SUBACT_MOVE, 2);
            }
        }
        break;
    case 10:
        if (fopAcM_checkHookCarryNow(this) && mAnm != BCK_GGB_PULL) {
            SetAnm(BCK_GGB_PULL, 2, 1.0f, 1.0f);
        } else if (!fopAcM_checkHookCarryNow(this)) {
            SetAction(ACTION_GROUND, SUBACT_ATTACK, 10);
        }
        break;
    }

    FallChk();
}

void daB_GG_c::G_DeathAction() {
    cXyz sp8;
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (mMode) {
    case 0:
        if (mpModelMorf->isStop()) {
            if (mType == TYPE_L7_MBOSS) {
                mTimers[1] = 130;
            } else {
                mTimers[1] = 70;
            }

            mMode++;
        }
        break;
    case 1:
        if (mTimers[1] == 0) {
            fopAcM_createDisappear(this, &current.pos, 10, 0, 0x2E);
            fopAcM_createDisappear(this, &mSwordPos, 5, 0, 0xFF);
            fopAcM_delete(this);

            if (mType == TYPE_L7_MBOSS) {
                dComIfGs_onSwitch(field_0x5cf, fopAcM_GetRoomNo(this));
                SetReleaseCam();
            } else if (mType == TYPE_HYRULE_CASTLE) {
                cDmr_SkipInfo = 0;
                dComIfGs_onSwitch(field_0x5cf, fopAcM_GetRoomNo(this));
            } else if (field_0x5cf != 0xFF && !dComIfGs_isSwitch(field_0x5cf, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(field_0x5cf, fopAcM_GetRoomNo(this));
            }
        }

        if (mTimers[1] == (90.0f + aREG_F(1)) && mType == TYPE_L7_MBOSS) {
            player->changeOriginalDemo();
            player->changeDemoMode(0x1E, 0, 0, 0);
        }
        break;
    }
}

void daB_GG_c::G_DeathAction1() {
    cXyz sp50(0.0f, 0.0f, 0.0f);
    cXyz sp44(0.0f, 0.0f, 0.0f);
    cXyz sp38(0.0f, 0.0f, 0.0f);
    daPy_getPlayerActorClass();

    cXyz sp2C;
    dBgS_LinChk sp70;

    if (mpModelMorf->checkFrame(2.0f)) {
        field_0x66d = 1;
        mDoMtx_stack_c::copy(mpSwordModel->getBaseTRMtx());
        mDoMtx_stack_c::multVecZero(&mSwordPos);
        mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &mSwordRot);

        sp50.set(BREG_F(3), BREG_F(4), BREG_F(5));
        cLib_offsetPos(&mSwordPos, &mSwordPos, shape_angle.y, &sp50);

        if (mType != TYPE_HYRULE_CASTLE) {
            sp44.set(cM_rndFX(30.0f), 80.0f + aREG_F(8), cM_rndFX(30.0f));
        } else {
            sp44.set(0.0f, 5.0f + aREG_F(8), -1.0f);
        }

        cLib_offsetPos(&sp50, &sp38, shape_angle.y, &sp44);
        field_0x684 = sp50;
    } else if (mpModelMorf->getFrame() > 2.0f && 0.0f != field_0x684.z) {
        sp2C = mSwordPos;
        field_0x684.y += -9.0f + aREG_F(9);
        mSwordPos += field_0x684;

        cLib_chaseAngleS(&mSwordRot.z, -0x3500, 0x500);
        cLib_chaseAngleS(&mSwordRot.x, 0x4FA0, 0x500);

        sp70.Set(&sp2C, &mSwordPos, NULL);
        if (dComIfG_Bgsp().LineCross(&sp70)) {
            field_0x5b2++;

            cM3dGPla sp5C;
            dComIfG_Bgsp().GetTriPla(sp70, &sp5C);

            field_0x684 = Hanekaeri(this, field_0x684, *sp5C.GetNP(), 0.5f);
            if (field_0x5b2 > 2) {
                field_0x684.set(0.0f, 0.0f, 0.0f);
            }

            mSwordRot.y += 0x500;
            mSwordPos = sp70.GetCross();
            mSwordPos.y += 15.0f;
        }
    }

    field_0x66c = 1;

    if (mpModelMorf->isStop()) {
        cLib_chaseF(&field_0x668, 0.4f + aREG_F(5), 0.001f + aREG_F(6));
    }
}

void daB_GG_c::DeathCam() {
    cXyz sp48(0.0f, 0.0f, 0.0f);
    cXyz sp3C(0.0f, 0.0f, 0.0f);
    cXyz sp30(0.0f, 0.0f, 0.0f);

    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp24;
    dBgS_LinChk sp58;

    if (mpModelMorf->checkFrame(176.0f)) {
        field_0x66d = 1;

        mDoMtx_stack_c::copy(mpSwordModel->getBaseTRMtx());
        mDoMtx_stack_c::multVecZero(&mSwordPos);
        mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &mSwordRot);

        sp48.set(BREG_F(3), BREG_F(4), BREG_F(5));
        cLib_offsetPos(&mSwordPos, &mSwordPos, shape_angle.y, &sp48);

        sp3C.set(-5.0f, -1.0f, 5.0f);
        cLib_offsetPos(&sp48, &sp30, shape_angle.y, &sp3C);

        field_0x684 = sp48;
    } else if ((mpModelMorf->getFrame() > 176.0f || mpModelMorf->isStop()) && 0.0f != field_0x684.z) {
        sp24 = mSwordPos;
        field_0x684.y += -7.0f + aREG_F(9);
        mSwordPos += field_0x684;

        cLib_chaseAngleS(&mSwordRot.z, -0x3500, 0x500);
        cLib_chaseAngleS(&mSwordRot.x, 0x4FA0, 0x500);

        sp58.Set(&sp24, &mSwordPos, NULL);
        if (dComIfG_Bgsp().LineCross(&sp58)) {
            mSwordPos = sp58.GetCross();
            mSwordPos.y += 10.0f;

            field_0x684.y = field_0x684.y * -(0.6f + BREG_F(7));
            if (field_0x684.y < (8.0f + BREG_F(6))) {
                field_0x684.set(0.0f, 0.0f, 0.0f);
            }
        }
    }

    switch (mCamMode) {
    case 0:
        if (CameraSet()) {
            sp48.set(0.0f, 0.0f, -57.0f);
            current.pos = sp48;
            old.pos = current.pos;
            shape_angle.y = cLib_targetAngleY(&current.pos, &in_pos[mInOutPosIdx]);
            current.angle.y = shape_angle.y;

            mCamMode++;
            field_0x658 = 0;
            field_0x650 = 350.0f + BREG_F(3);

            sp30.set(0.0f, 0.0f, 0.0f);
            sp30 = current.pos;
            sp30.y += 150.0f + BREG_F(5);
            SetStopCam(sp30, 350.0f + BREG_F(3), -100.0f - BREG_F(5), shape_angle.y);

            sp30.set(0.0f, mAcch.GetGroundH(), 0.0f);
            sp48.set(0.0f, 0.0f, -300.0f);
            cLib_offsetPos(&sp30, &mCamEye, cLib_targetAngleY(&mCamEye, &current.pos), &sp48);

            player->setPlayerPosAndAngle(&sp30, shape_angle.y, 0);
            player->changeOriginalDemo();
        }
        break;
    case 1:
        sp30.set(0.0f, 0.0f, -57.0f);
        field_0x658 -= 0x10;
        sp48.set(0.0f, 0.0f, 0.0f);

        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(6));
        mDoMtx_stack_c::multVec(&sp48, &sp48);
        field_0x60c = sp48;
        field_0x60c.y -= 80.0f + BREG_F(1);
        field_0x650 += 0.5f + aREG_F(11);

        if (field_0x60c.y > (150.0f + BREG_F(4))) {
            field_0x60c.y = 150.0f + BREG_F(4);
        } else if (field_0x60c.y < (100.0f + BREG_F(2))) {
            field_0x60c.y = 100.0f + BREG_F(2);
        }

        sp48.set(0.0f, field_0x618.y, field_0x650);
        cLib_offsetPos(&field_0x618, &sp30, (shape_angle.y + field_0x658), &sp48);
        SetMoveCam1(0.1f + BREG_F(15), 50.0f + BREG_F(16));

        sp30 = current.pos;
        sp30.set(0.0f, s_LinkPos->y, 0.0f);
        sp48.set(0.0f, 0.0f, -300.0f);
        cLib_offsetPos(&sp30, &mCamEye, cLib_targetAngleY(&mCamEye, &current.pos), &sp48);
        player->setPlayerPosAndAngle(&sp30, shape_angle.y, 0);

        if (mpModelMorf->checkFrame(200.0f)) {
            field_0x66c = 1;
            field_0x668 = 0.0f;

            sp3C.set(300.0f, 0.0f, 400.0f);
            cLib_offsetPos(&sp48, &current.pos, cLib_targetAngleY(&current.pos, &mCamEye), &sp3C);
            fopAcM_SetSpeed(player, 0.0f, 0.0f, 0.0f);

            player->setPlayerPosAndAngle(&sp48, cLib_targetAngleY(&sp48, &current.pos), 0);
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
            mCamMode++;
        }
        break;
    case 2:
        sp30.set(0.0f, 0.0f, -57.0f);
        field_0x658 -= 0x10;
        sp48.set(0.0f, 0.0f, 0.0f);

        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(6));
        mDoMtx_stack_c::multVec(&sp48, &sp48);
        field_0x60c = sp48;
        field_0x60c.y -= 80.0f + BREG_F(1);
        field_0x650 += 0.5f + aREG_F(11);

        if (mpModelMorf->getFrame() < 200.0f) {
            if (field_0x60c.y > (150.0f + BREG_F(4))) {
                field_0x60c.y = 150.0f + BREG_F(4);
            } else if (field_0x60c.y < (100.0f + BREG_F(2))) {
                field_0x60c.y = 100.0f + BREG_F(2);
            }
        } else {
            field_0x60c.x = current.pos.x;
            field_0x60c.z = current.pos.z;
            if (field_0x60c.y < (100.0f + BREG_F(2))) {
                field_0x60c.y = 100.0f + BREG_F(2);
            }
        }

        if (mpModelMorf->isStop() && cLib_chaseF(&field_0x668, 0.5f + aREG_F(5), 0.01f + aREG_F(6))) {
            cDmr_SkipInfo = 0;
            fopAcM_createDisappear(this, &current.pos, 10, 0, 0xFF);
            fopAcM_createDisappear(this, &mSwordPos, 5, 0, 0xFF);

            current.pos.y += 10000.0f;
            old.pos.y = current.pos.y;
            mSwordPos.y += 10000.0f;
            mTimers[0] = 30;
            mCamMode++;
        }

        sp48.set(0.0f, field_0x618.y, field_0x650);
        cLib_offsetPos(&field_0x618, &sp30, (shape_angle.y + field_0x658), &sp48);
        SetMoveCam1(0.1f + BREG_F(15), 50.0f + BREG_F(16));
        break;
    case 3:
        current.pos.y = 10000.0f;

        if (mTimers[0] == 0) {
            sp48 = *s_LinkPos;
            sp48.y += 100.0f;

            if (!daPy_py_c::checkNowWolf()) {
                SetStopCam(sp48, 200.0f, 0.0f, (s_TargetAngle + 0x7000));
            } else {
                SetReleaseCam();
                if (field_0x5cf != 0xFF && !dComIfGs_isSwitch(field_0x5cf, fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(field_0x5cf, fopAcM_GetRoomNo(this));
                    dComIfGs_onStageMiddleBoss();
                }
                fopAcM_delete(this);
            }

            player->changeOriginalDemo();

            if (!daPy_py_c::checkNowWolf()) {
                player->changeDemoMode(0x1E, 0, 0, 0);
            }

            mTimers[2] = 110;
            mTimers[0] = 1000;
        } else if (mTimers[2] == 1) {
            if (field_0x5cf != 0xFF && !dComIfGs_isSwitch(field_0x5cf, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(field_0x5cf, fopAcM_GetRoomNo(this));
                dComIfGs_onStageMiddleBoss();
            }

            fopAcM_delete(this);

            sp48.set(0.0f, 100.0f, 300.0f);
            mCamCenter.set(s_LinkPos->x, 200.0f + s_LinkPos->y, s_LinkPos->z);
            cLib_offsetPos(&mCamEye, s_LinkPos, s_TargetAngle, &sp48);
            SetReleaseCam();
        }

        SetStopingCam();
        break;
    }
}

void daB_GG_c::GroundAction() {
    s16 var_r30 = 0;

    switch (mSubAction) {
    case SUBACT_MOVE:
        G_MoveAction();
        var_r30 = 10;
        break;
    case SUBACT_ATTACK:
        G_AttackAction();
        break;
    case SUBACT_DAMAGE:
        G_DamageAction();
        var_r30 = 2;
        break;
    case SUBACT_DEATH:
        if (mType == TYPE_L7_MBOSS) {
            DeathCam();
        } else {
            G_DeathAction1();
            G_DeathAction();
        }
        break;
    }

    if (mAnm != BCK_GGB_FAINT) {
        offHeadLockFlg();
    }

    cLib_addCalc2(&speedF, field_0x5bc, 0.2f, 100.0f);
    cLib_addCalcAngleS2(&current.angle.y, field_0x5ba, var_r30, 0x1000);
    cLib_addCalcAngleS2(&shape_angle.y, s_TargetAngle, var_r30, 0x1000);
    cLib_addCalcAngleS2(&field_0x6be, (s_TargetAngle - shape_angle.y), var_r30, 0x100);
    cLib_addCalcAngleS2(&mArmL1Rot.x, 0, 0x10, 0x1000);
    cLib_addCalcAngleS2(&mArmL1Rot.y, 0, 0x10, 0x1000);
    cLib_addCalcAngleS2(&mArmL1Rot.z, 0, 0x10, 0x1000);
    cLib_addCalcAngleS2(&mArmL2Rot.x, 0, 0x10, 0x1000);
    cLib_addCalcAngleS2(&mArmL2Rot.y, 0, 0x10, 0x1000);
    cLib_addCalcAngleS2(&mArmL2Rot.z, 0, 0x10, 0x1000);
}

void daB_GG_c::StopAction() {
    if (!other_bg_check(this, daPy_getPlayerActorClass())) {
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        attention_info.flags = 0;
    }

    cXyz sp20;
    switch (mMode) {
    case 0:
        if (s_LinkPos->y < (500.0f + current.pos.y)) {
            mAction = ACTION_GROUND;
            mSubAction = SUBACT_MOVE;
            mMode = 4;
        }

        if (mpModelMorf->isStop() && mAnm != BCK_GGB_WAIT_B) {
            SetAnm(BCK_GGB_WAIT_B, 2, 5.0f, 1.0f);
        }

        FookChk();
        break;
    case 5:
        if (!dComIfGp_checkPlayerStatus0(0, 0x4000)) {
            mMode = 0;
        }
        break;
    }

    if (field_0x5cc > 0) {
        mCcCyl.ClrTgHit();
        mCcShieldSph.ClrTgHit();
        mCcHookCyl.ClrTgHit();
        return;
    }

    if ((mCcCyl.ChkTgShield() && mCcCyl.ChkTgHit()) || mCcHookCyl.ChkTgHit()) {
        mAtInfo.field_0x18 = 0x2A;

        cCcD_Obj* tg_hit_obj;
        if (mCcCyl.ChkTgHit()) {
            mAtInfo.mpCollider = mCcCyl.GetTgHitObj();
            tg_hit_obj = mCcCyl.GetTgHitObj();
            mCcCyl.GetTgHitPosP();
        } else if (mCcHookCyl.ChkTgHit()) {
            mAtInfo.mpCollider = mCcHookCyl.GetTgHitObj();
            tg_hit_obj = mCcHookCyl.GetTgHitObj();
            mCcHookCyl.GetTgHitPosP();
        }

        At_Check();

        if (tg_hit_obj->ChkAtType(AT_TYPE_HOOKSHOT)) {
            mAction = ACTION_GROUND;
            mMode = 10;
            mSubAction = SUBACT_DAMAGE;
            mTimers[2] = l_HIO.takeoff2_time;
            gravity = -9.0f;
        } else {
            sp20.set(0.0f, 0.0f, 0.0f);

            cXyz sp14(0.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0xB));
            mDoMtx_stack_c::multVec(&sp14, &sp20);
            dComIfGp_setHitMark(2, this, &sp20, NULL, NULL, 0);
            SetAnm(BCK_GGB_GUARD, 0, 1.0f, 1.0f);
        }

        mCcHookCyl.ClrTgHit();
        mCcCyl.ClrTgHit();
        mCcShieldSph.ClrTgHit();
        field_0x5cc = 15;
    }

    cXyz sp8 = *s_LinkPos - current.pos;
    cLib_addCalcAngleS2(&field_0x6c4, -cM_atan2s(sp8.y, JMAFastSqrt((sp8.x * sp8.x) + (sp8.z * sp8.z))), 2, 0x600);
    cLib_addCalcAngleS2(&field_0x6be, (s_TargetAngle - shape_angle.y), 2, 0x100);
    current.angle.y = s_TargetAngle;
    shape_angle.y = s_TargetAngle;
}

void daB_GG_c::HitChk() {
    if (mAction == ACTION_GROUND && mSubAction == SUBACT_ATTACK) {
        if ((s_TargetAngle - shape_angle.y) < 0x3000 && (s_TargetAngle - shape_angle.y) > 0x300) {
            mCcShieldSph.OnTgShield();
            mCcCyl.OnTgShield();
        } else {
            mCcShieldSph.OffTgShield();
            mCcCyl.OffTgShield();
        }
    }
}

void daB_GG_c::Action() {
    if (mType != TYPE_L7_MBOSS) {
        if (s_dis > 5000.0f) {
            mSound.setLinkSearch(0);
        } else {
            mSound.setLinkSearch(1);
        }
    }

    HitChk();

    if (mAction != ACTION_F_A && mAction != ACTION_STOP) {
        ObjHit();
    }

    switch (mAction) {
    case ACTION_FLY:
        FlyAction();
        break;
    case ACTION_GROUND:
        GroundAction();
        break;
    case ACTION_F_A:
        F_A_Action();
        break;
    case ACTION_DEMO:
        DemoAction();
        break;
    case ACTION_STOP:
        StopAction();
        break;
    }

    if (field_0x5b1 == 1) {
        HeadAction();
    }
}

void daB_GG_c::HeadAction() {
    cXyz sp18;
    cXyz spC;
    dBgS_LinChk sp28;

    switch (field_0x5c4) {
    case 0:
        field_0x6a8.set(5.0f, 50.0f, 5.0f);
        field_0x6b4.x = 0x500;
        field_0x6b4.z = 0x500;
        field_0x5c4++;
        break;
    case 1:
        spC = mHeadPos;
        field_0x6a8.y += -7.0f;
        mHeadPos += field_0x6a8;
        mHeadRot += field_0x6b4;

        sp28.Set(&spC, &mHeadPos, NULL);
        if (dComIfG_Bgsp().LineCross(&sp28)) {
            mHeadPos = sp28.GetCross();
            Z2GetAudioMgr()->seStart(0x705B6, &mHeadPos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);

            field_0x6a8.y = field_0x6a8.y * -0.6f;
            if (field_0x6a8.y < (2.5f + BREG_F(6))) {
                field_0x6a8.set(0.0f, 0.0f, 0.0f);
                fopAcM_createDisappear(this, &mHeadPos, 3, 0, 0xFF);
                field_0x5b1 = 2;
            }
        }

        cLib_chaseAngleS(&field_0x6b4.x, 0, 5);
        cLib_chaseAngleS(&field_0x6b4.z, 0, 5);
        break;
    case 2:
        if (mHeadPos.absXZ(current.pos) < 100.0f && mTimers[3] == 0) {
            sp18.set(0.0f, 20.0f + MREG_F(8), 10.0f + MREG_F(9));

            mDoMtx_stack_c::YrotS(cLib_targetAngleY(&current.pos, &mHeadPos));
            mDoMtx_stack_c::multVec(&sp18, &sp18);
            field_0x6a8 = sp18;

            mTimers[3] = 50;
            field_0x6b4.x = 0x500;
            field_0x6b4.z = 0x500;
        } else if (mHeadPos.absXZ(*s_LinkPos) < 100.0f && mTimers[3] == 0) {
            sp18.set(0.0f, 20.0f + MREG_F(8), 10.0f + MREG_F(9));

            mDoMtx_stack_c::YrotS(cLib_targetAngleY(s_LinkPos, &mHeadPos));
            mDoMtx_stack_c::multVec(&sp18, &sp18);
            field_0x6a8 = sp18;

            mTimers[3] = 50;
            field_0x6b4.x = 0x500;
            field_0x6b4.z = 0x500;
        }

        field_0x6a8.y += -5.0f;
        mHeadPos += field_0x6a8;
        mHeadRot.x += field_0x6b4.x;
        mHeadRot.y += field_0x6b4.y;
        mHeadRot.z += field_0x6b4.z;

        if (mHeadPos.y < 15.0f) {
            mHeadPos.y = 15.0f;
            field_0x6a8.y = field_0x6a8.y * -0.5f;
            if (field_0x6a8.y < 4.0f) {
                field_0x6a8.set(0.0f, 0.0f, 0.0f);
                field_0x6b4.x = 0;
                field_0x6b4.z = 0;
                mTimers[3] = 0;
            }
        }

        cLib_chaseAngleS(&field_0x6b4.x, 0, 5);
        cLib_chaseAngleS(&field_0x6b4.z, 0, 5);
        break;
    }
}

void daB_GG_c::SetHeadAngle() {
    cXyz sp20;
    cXyz sp14(0.0f, 0.0f, 0.0f);

    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(5));
    mDoMtx_stack_c::multVec(&sp14, &sp14);
    sp20 = sp14 - field_0x65c;
    
    s16 temp_r30 = -cM_atan2s(sp20.y, JMAFastSqrt((sp20.x * sp20.x) + (sp20.z * sp20.z)));
    s16 var_r29 = cM_atan2s(sp20.x, sp20.z);
    if (mAction != ACTION_GROUND) {
        var_r29 = 0x3000;
    }

    cLib_addCalcAngleS2(&field_0x6d4, temp_r30, 2, 0x1000);
    cLib_addCalcAngleS2(&field_0x6d6, temp_r30, 2, 0x1000);
    field_0x65c = sp14;
}

void daB_GG_c::SoundChk() {
    switch (mAnm) {
    case BCK_GGA_ATTACK_1:
        if (mpModelMorf->checkFrame(0.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_WING, 0, -1);
            mSound.startCreatureVoice(Z2SE_EN_GG_V_ATTACK, -1);
        } else if (mpModelMorf->checkFrame(7.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_WING, 0, -1);
        }
        break;
    case BCK_GGA_ATTACK_3:
        if (mpModelMorf->checkFrame(3.0f) || mpModelMorf->checkFrame(16.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_WING, 0, -1);
        } else if (mpModelMorf->checkFrame(0.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_ATTACK, 0, -1);
        }
        break;
    case BCK_GGA_FS_OFF:
        if (mpModelMorf->checkFrame(6.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_WING, 0, -1);
        }
        break;
    case BCK_GGA_GUARD:
        if (mpModelMorf->checkFrame(27.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_WING, 0, -1);
        }
        break;
    case BCK_GGA_PREPARATION:
        if (mpModelMorf->checkFrame(5.0f) || mpModelMorf->checkFrame(13.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_WING, 0, -1);
        }
        break;
    case BCK_GGA_SHOUT:
        if (mpModelMorf->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_GG_V_SHOUT, -1);
        }
        break;
    case BCK_GGA_WAIT:
        if (mpModelMorf->checkFrame(10.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_WING, 0, -1);
        }
        break;
    case BCK_GGB_ATTACK_A:
        if (mpModelMorf->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_GG_V_ATTACK_A, -1);
        } else if (mpModelMorf->checkFrame(26.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_ATTACK_A, 0, -1);
        }
        break;
    case BCK_GGB_ATTACK_B:
        if (mpModelMorf->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_GG_V_ATTACK_B, -1);
        } else if (mpModelMorf->checkFrame(29.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_ATTACK_B, 0, -1);
        }
        break;
    case BCK_GGB_ATTACK_C:
        if (mpModelMorf->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_GG_V_ATTACK_C, -1);
        } else if (mpModelMorf->checkFrame(5.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_ATTACK_C, 0, -1);
        }
        break;
    case BCK_GGB_BACK_STEP:
        if (mpModelMorf->checkFrame(4.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_WING, 0, -1);
        }
        break;
    case BCK_GGB_DIE:
        if (mpModelMorf->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_GG_V_DIE_BOSS, -1);
        } else if (mpModelMorf->checkFrame(190.0f)) {
            mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
        } else if (mpModelMorf->checkFrame(226.0f)) {
            mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
        }
        break;
    case BCK_GGB_FAINT:
        if (mpModelMorf->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_GG_V_FAINT, -1);
        }
        break;
    case BCK_GGB_TAKE_OFF:
        if (mpModelMorf->checkFrame(23.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_WING, 0, -1);
        }
        break;
    case BCK_GGB_WAIT_B:
        if (mpModelMorf->checkFrame(5.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_GG_V_BREATH, -1);
        }
        break;
    case BCK_GGB_WALK:
        if (mpModelMorf->checkFrame(11.0f) || mpModelMorf->checkFrame(1.0f)) {
            mSound.startCreatureSound(Z2SE_EN_GG_FOOTNOTE, 0, -1);
        }
        break;
    case BCK_GG_DIE:
        if (mpModelMorf->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_GG_V_DIE, -1);
        } else if (mpModelMorf->checkFrame(22.0f)) {
            mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
        } else if (mpModelMorf->checkFrame(42.0f)) {
            mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
        }
        break;
    }
}

int daB_GG_c::Execute() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    s_LinkPos = &fopAcM_GetPosition(player);
    s_TargetAngle = fopAcM_searchPlayerAngleY(this);
    s_dis = current.pos.abs(*s_LinkPos);

    for (int i = 0; i < 4; i++) {
        mTimers[i]--;
        if (mTimers[i] <= 0) {
            mTimers[i] = 0;
        }
    }

    field_0x5cc--;
    if (field_0x5cc <= 0) {
        field_0x5cc = 0;
    }

    Yazirushi();
    Action();
    SoundChk();

    if (mType == TYPE_L7_MBOSS && (mAction != ACTION_DEMO || mSubAction != 0)) {
        daPy_getPlayerActorClass()->onBossRoomWait();
    }

    cXyz eff_size(1.5f, 1.5f, 1.5f);
    setMidnaBindEffect(this, &mSound, &current.pos, &eff_size);
    SetHeadAngle();
    mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());

    setBaseMtx();
    setShieldMtx();
    setSwordMtx();

    if (mAction == ACTION_GROUND) {
        G_setCcCylinder();
    } else {
        setCcCylinder();
    }

    if (field_0x65a != 0) {
        MoveAt();
    } else {
        cXyz spC;
        for (int i = 0; i < 3; i++) {
            spC.set(0.0f, (10.0f + (100.0f * i)) * l_HIO.base_size, 0.0f);

            mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0x10));
            mDoMtx_stack_c::multVec(&spC, &spC);

            mCcSph[i].SetC(spC);
            mCcSph[i].SetR(70.0f * l_HIO.base_size);
            dComIfG_Ccsp()->Set(&mCcSph[i]);
        }
    }

    if (mType == TYPE_L7_MBOSS) {
        setHeadMtx();
    }

    return 1;
}

void daB_GG_c::ObjHit() {
    Guard();
}

void daB_GG_c::At_Check() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    mAtInfo.mpActor = at_power_check(&mAtInfo);
    if (mAtInfo.mpActor != NULL) {
        if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
            mAtInfo.mAttackPower = 0;
        } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB)) {
            mAtInfo.mAttackPower = 30;
        } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_NORMAL_SWORD) && daPy_py_c::checkMasterSwordEquip()) {
            mAtInfo.mAttackPower *= 2;
        }

        if ((s16)mAtInfo.mAttackPower > 0) {
            if (mAtInfo.mAttackPower > 80) {
                mAtInfo.mAttackPower = 80;
            }

            health -= (s16)mAtInfo.mAttackPower;
        }

        int pause_time;
        if (mAtInfo.mAttackPower != 0) {
            mAtInfo.mHitStatus = 2;
            pause_time = 5;
        } else {
            pause_time = 2;
        }

        if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_MIDNA_LOCK)) {
            pause_time = 0;
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

        if ((mAtInfo.mHitType == HIT_TYPE_LINK_NORMAL_ATTACK || mAtInfo.mpCollider->ChkAtType(AT_TYPE_THROW_OBJ)) && !player->checkCutJumpCancelTurn()) {
            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_THROW_OBJ)) {
                pause_time = 4;
            }

            dScnPly_c::setPauseTimer(pause_time);
        }
    }
}

void daB_GG_c::MoveAt() {
    cXyz pos(0.0f, 10000.0f, 0.0f);
    for (int i = 0; i < 3; i++) {
        mCcSph[i].SetC(pos);
    }
}

void daB_GG_c::F_AtHit() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    field_0x65a--;
    if (field_0x65a <= 0) {
        field_0x65a = 0;
    }

    if (field_0x65a > 0) {
        for (int i = 0; i < 3; i++) {
            mCcSph[i].ClrAtHit();
        }
        return;
    }

    for (int i = 0; i < 3; i++) {
        if (mCcSph[i].ChkAtHit() && !player->checkPlayerGuard()) {
            if (mAction != ACTION_FLY && mAction == ACTION_GROUND && mCcSph[i].GetAtSpl() == 0) {
                player->setThrowDamage(s_TargetAngle, 20.0f + nREG_F(4), 15.0f + nREG_F(5), 0, 0, 0);
            }

            field_0x65a = 20;
            break;
        }
    }
}

void daB_GG_c::ChanceTime() {
    if ((mCcCyl.ChkTgHit() && !mCcCyl.ChkTgShield()) || mCcHookCyl.ChkTgHit()) {
        s16 prev_hp = health;
        if (mAction == ACTION_FLY && (mMode == 12 || mMode == 13)) {
            mAction = ACTION_GROUND;

            if (field_0x5ce != 0xFF && dComIfGs_isSwitch(142, fopAcM_GetRoomNo(this))) {
                dComIfGs_offSwitch(142, fopAcM_GetRoomNo(this));
            }

            if (field_0x5b6 != 0) {
                mTimers[2] = 0.8f * l_HIO.takeoff2_time;
            } else {
                mTimers[2] = 0.8f * l_HIO.takeoff_time;
            }
        }

        mAtInfo.field_0x18 = 0x1E;

        cCcD_Obj* tg_hit_obj;
        if (mCcCyl.ChkTgHit()) {
            tg_hit_obj = mCcCyl.GetTgHitObj();
            mAtInfo.mpCollider = mCcCyl.GetTgHitObj();
        } else {
            tg_hit_obj = mCcHookCyl.GetTgHitObj();
            mAtInfo.mpCollider = mCcHookCyl.GetTgHitObj();
        }

        u8 sp8 = 0;

        if (!tg_hit_obj->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
            At_Check();
        }

        u8 spA = mSubAction;
        field_0x5cc = 10;
        mSubAction = SUBACT_DAMAGE;
        mMode = 2;

        if (tg_hit_obj->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
            if (field_0x5f0 <= 1) {
                field_0x5f0++;
                SetAnm(BCK_GGB_FAINT, 0, 5.0f, 1.0f);
                onHeadLockFlg();
                mMode = 4;
                mSubAction = SUBACT_DAMAGE;

                mCcShieldSph.OffTgSetBit();
                mCcCyl.OffTgShield();
                mCcShieldSph.OffTgShield();

                speedF = 0.0f;
                field_0x5bc = 0.0f;
                At_Check();
                field_0x5cc = 20;
            } else {
                mAtInfo.field_0x18 = 0x2A;
                field_0x5cc = 15;
                mSubAction = SUBACT_DAMAGE;
                SetAnm(BCK_GGB_GUARD, 0, 1.0f, 1.0f);

                mCcShieldSph.OnTgSetBit();
                mCcCyl.OnTgShield();
                mCcShieldSph.OnTgShield();
                mMode = 0;

                cXyz sp14(0.0f, 0.0f, 0.0f);
                mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0xB));
                mDoMtx_stack_c::multVec(&sp14, &sp14);
                dComIfGp_setHitMark(2, this, &sp14, NULL, NULL, 0);

                At_Check();
            }
        } else if (tg_hit_obj->ChkAtType(AT_TYPE_HOOKSHOT)) {
            mMode = 10;
            field_0x5f0 = 0;
        } else {
            field_0x5e0++;
            field_0x5f0 = 0;

            if (mCcCyl.ChkTgHit()) {
                dComIfGp_setHitMark(1, this, mCcCyl.GetTgHitPosP(), NULL, NULL, 0);
            } else {
                dComIfGp_setHitMark(1, this, mCcHookCyl.GetTgHitPosP(), NULL, NULL, 0);
            }

            mSound.startCreatureVoice(Z2SE_EN_GG_V_DAMAGE, -1);
            
            u32 cut_type = daPy_getPlayerActorClass()->getCutType();
            if (cut_type == daPy_py_c::CUT_TYPE_TURN_LEFT || cut_type == daPy_py_c::CUT_TYPE_TURN_RIGHT) {
                field_0x6bc++;
                field_0x5cc = 20;
            }
        }

        if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_HEAD_JUMP && mType == TYPE_L7_MBOSS) {
            if (field_0x5b1 == 0) {
                field_0x5b1 = 1;
                health = prev_hp;
                mSound.startCreatureSound(Z2SE_EN_GG_MET_BREAK, 0, -1);
            }

            mDoMtx_stack_c::copy(mpHelmetModel->getBaseTRMtx());
            mDoMtx_stack_c::multVecZero(&mHeadPos);
            mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &mHeadRot);

            field_0x5c4 = 0;
            SetAction(ACTION_GROUND, SUBACT_DAMAGE, 2);
        }

        MoveCo();
        mCcCyl.ClrTgHit();
        mCcHookCyl.ClrTgHit();
        mCcShieldSph.OffTgSetBit();
    }
}

void daB_GG_c::ArmAngleSet() {
    daPy_getPlayerActorClass()->getCutType();

    switch (daPy_getPlayerActorClass()->getCutType()) {
    case daPy_py_c::CUT_TYPE_NM_RIGHT:
    case daPy_py_c::CUT_TYPE_FINISH_RIGHT:
    case daPy_py_c::CUT_TYPE_MORTAL_DRAW_B:
        mArmL2Rot.y = -0x1130;
        mArmL1Rot.z = 0x1B58;
        return;
    case daPy_py_c::CUT_TYPE_NM_LEFT:
    case daPy_py_c::CUT_TYPE_FINISH_LEFT:
    case daPy_py_c::CUT_TYPE_TURN_LEFT:
        mArmL1Rot.x = -0x3000;
        return;
    case daPy_py_c::CUT_TYPE_HEAD_JUMP:
    case daPy_py_c::CUT_TYPE_FINISH_VERTICAL:
    case daPy_py_c::CUT_TYPE_JUMP:
        mArmL2Rot.x = -0x2328;
        mArmL1Rot.y = 0x3BC4;
    default:
        return;
    }
}

void daB_GG_c::Guard() {
    mCcStts.Move();

    if (field_0x5cc > 0) {
        mCcCyl.ClrTgHit();
        mCcShieldSph.ClrTgHit();
        mCcHookCyl.ClrTgHit();
        return;
    }

    if (mCcCyl.ChkTgHit() && !mCcCyl.ChkTgShield()) {
        ChanceTime();
        return;
    }

    if (mCcShieldSph.ChkTgHit() || (mCcCyl.ChkTgShield() && mCcCyl.ChkTgHit()) || mCcHookCyl.ChkTgHit()) {
        current.angle.y = s_TargetAngle;
        field_0x5ba = s_TargetAngle;
        shape_angle.y = s_TargetAngle;

        u8 spC = mSubAction;
        field_0x6bc = 0;
        field_0x5e0 = 0;
        u8 sp8 = 0;
        mAtInfo.field_0x18 = 0x2A;

        cCcD_Obj* tg_hit_obj;
        if (mCcCyl.ChkTgHit()) {
            mAtInfo.mpCollider = mCcCyl.GetTgHitObj();
            tg_hit_obj = mCcCyl.GetTgHitObj();
            mCcCyl.GetTgHitPosP();
        } else if (mCcHookCyl.ChkTgHit()) {
            mAtInfo.mpCollider = mCcHookCyl.GetTgHitObj();
            tg_hit_obj = mCcHookCyl.GetTgHitObj();
            mCcHookCyl.GetTgHitPosP();
        } else {
            mAtInfo.mpCollider = mCcShieldSph.GetTgHitObj();
            tg_hit_obj = mCcShieldSph.GetTgHitObj();
            mCcShieldSph.GetTgHitPosP();
        }

        s16 prev_hp = health;
        At_Check();
        health = prev_hp;
        field_0x5cc = 10;

        if (mSubAction == SUBACT_ATTACK && mAction != ACTION_GROUND) {
            if (mMode == 0 || mMode == 10) {
                if (tg_hit_obj->ChkAtType(AT_TYPE_HOOKSHOT)) {
                    mMode = 10;
                    mSubAction = SUBACT_DAMAGE;
                } else {
                    cXyz sp34(0.0f, 0.0f, 0.0f);
                    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0xB));
                    mDoMtx_stack_c::multVec(&sp34, &sp34);
                    dComIfGp_setHitMark(2, this, &sp34, NULL, NULL, 0);
                }
            }
        } else {
            cXyz sp28(0.0f, 120.0f, 100.0f);
            cXyz sp1C(0.0f, 0.0f, 0.0f);
            cLib_offsetPos(&sp1C, &current.pos, shape_angle.y, &sp28);

            cXyz sp10(1.0f, 1.0f, 1.0f);

            if (mAnm == BCK_GGB_GUARD) {
                mAnm = BCK_GGB_FS_HIT;
            }

            if (tg_hit_obj->ChkAtType(AT_TYPE_HOOKSHOT)) {
                mMode = 10;
                mSubAction = SUBACT_DAMAGE;
                field_0x5f0 = 0;
            } else if (tg_hit_obj->ChkAtType(AT_TYPE_8000)) {
                mMode = 1;
                mSubAction = SUBACT_DAMAGE;
                dComIfGp_setHitMark(2, this, &sp1C, NULL, NULL, 0);
                field_0x5f0 = 0;
            } else if (tg_hit_obj->ChkAtType(AT_TYPE_SHIELD_ATTACK) && field_0x5f0 <= 1) {
                field_0x5f0++;
                cM_rndF(100.0f);
                SetAnm(BCK_GGB_FAINT, 0, 5.0f, 1.0f);
                onHeadLockFlg();

                mMode = 4;
                mSubAction = SUBACT_DAMAGE;

                mCcShieldSph.OffTgSetBit();
                mCcCyl.OffTgShield();
                mCcShieldSph.OffTgShield();

                speedF = nREG_F(7);
                field_0x5bc = 0.0f;
            } else if (mSubAction != SUBACT_ATTACK) {
                mSubAction = SUBACT_DAMAGE;
                mMode = 0;

                if (mAction == ACTION_GROUND) {
                    SetAnm(BCK_GGB_GUARD, 0, 1.0f, 1.0f);
                    ArmAngleSet();
                }

                if (tg_hit_obj->ChkAtType(AT_TYPE_NORMAL_SWORD)) {
                    speedF = -10.0f + nREG_F(7);
                    field_0x5bc = 0.0f;
                } else {
                    speedF = 0.0f;
                    field_0x5bc = 0.0f;
                }

                dComIfGp_setHitMark(2, this, &sp1C, NULL, NULL, 0);
                field_0x5f0 = 0;
            } else {
                At_Check();
                mCcShieldSph.OnTgShield();
                mCcCyl.OnTgShield();
                mCcShieldSph.OnTgShield();

                field_0x5f0 = 0;
                sp1C.set(0.0f, 0.0f, 0.0f);
                mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0xB));
                mDoMtx_stack_c::multVec(&sp1C, &sp1C);
                dComIfGp_setHitMark(2, this, &sp1C, NULL, NULL, 0);
            }
        }

        mCcHookCyl.ClrTgHit();
        mCcCyl.ClrTgHit();
        mCcShieldSph.ClrTgHit();
        return;
    }

    ChanceTime();
}

int daB_GG_c::Draw() {
    J3DModel* model = mpModelMorf->getModel();

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpShieldModel, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpSwordModel, &tevStr);

    if (mType == TYPE_L7_MBOSS && field_0x5b1 != 2) {
        g_env_light.setLightTevColorType_MAJI(mpHelmetModel, &tevStr);
    }

    if (field_0x66c != 0) {
        J3DMaterial* material = model->getModelData()->getMaterialNodePointer(0);
        material->getTevColor(0)->r = -255.0f * field_0x668;
        material->getTevColor(0)->g = -255.0f * field_0x668;
        material->getTevColor(0)->b = -255.0f * field_0x668;
    }

    mpModelMorf->entryDL();
    mDoExt_modelUpdateDL(mpShieldModel);
    mDoExt_modelUpdateDL(mpSwordModel);

    if (mType == TYPE_L7_MBOSS && field_0x5b1 != 2) {
        mDoExt_modelUpdateDL(mpHelmetModel);
    }

    cXyz sp8;
    sp8.set(current.pos.x, 100.0f + current.pos.y, current.pos.z);

    mShadowId = dComIfGd_setShadow(mShadowId, 1, model, &sp8, 3000.0f, 0.0f, current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    dComIfGd_addRealShadow(mShadowId, mpShieldModel);
    dComIfGd_addRealShadow(mShadowId, mpSwordModel);

    if (mType == TYPE_L7_MBOSS && field_0x5b1 != 2) {
        dComIfGd_addRealShadow(mShadowId, mpHelmetModel);
    }

    if (mAnm == BCK_GGA_SHOUT && mpModelMorf->getFrame() > 32.0f) {
        cLib_chaseF(&field_0x5ac, 0.0f, 0.5f);
        mDoGph_gInf_c::setBlureRate(field_0x5ac);
        mDoGph_gInf_c::onBlure();
    } else {
        mDoGph_gInf_c::offBlure();
    }

    return 1;
}

int daB_GG_c::Delete() {
    dComIfG_resDelete(&mPhase, "B_gg");

    if (mHIOInit) {
        hio_set = 0;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (heap != NULL) {
        mpModelMorf->stopZelAnime();
    }

    return 1;
}

void daB_GG_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(l_HIO.base_size, l_HIO.base_size, l_HIO.base_size);

    mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModelMorf->modelCalc();
}

void daB_GG_c::setShieldMtx() {
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0xB));
    mpShieldModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daB_GG_c::setSwordMtx() {
    if (field_0x66d == 0) {
        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0x10));
        mpSwordModel->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        setSwordMtx1();
    }
}

void daB_GG_c::setSwordMtx1() {
    cXyz sp8(-20.0f, 60.0f, 0.0f);
    mDoMtx_stack_c::transS(mSwordPos);
    mDoMtx_stack_c::ZXYrotM(mSwordRot);
    mpSwordModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daB_GG_c::setHeadMtx() {
    if (field_0x5b1 == 0) {
        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(5));
        mpHelmetModel->setBaseTRMtx(mDoMtx_stack_c::get());
    } else if (field_0x5b1 == 1) {
        setHeadMtx1();
    }
}

void daB_GG_c::setHeadMtx1() {
    mDoMtx_stack_c::transS(mHeadPos);
    mDoMtx_stack_c::ZXYrotM(mHeadRot);
    mpHelmetModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static int daB_GG_Draw(daB_GG_c* a_this) {
    return a_this->Draw();
}

static int daB_GG_Execute(daB_GG_c* a_this) {
    return a_this->Execute();
}

static int daB_GG_IsDelete(daB_GG_c* a_this) {
    return 1;
}

static int daB_GG_Delete(daB_GG_c* a_this) {
    return a_this->Delete();
}

int daB_GG_c::Create() {
    fopAcM_ct(this, daB_GG_c);

    int phase_state = dComIfG_resLoad(&mPhase, "B_gg");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("B_GG PARAM %x\n", fopAcM_GetParam(this));
        mType = fopAcM_GetParam(this);
        field_0x5cf = (fopAcM_GetParam(this) & 0xFF0000) >> 0x10;

        if ((mType == TYPE_L7_MBOSS || mType == TYPE_DEFAULT) && dComIfGs_isStageMiddleBoss() && strcmp("D_MN07B", dComIfGp_getStartStageName()) == 0) {
            dComIfGs_onSwitch(field_0x5cf, fopAcM_GetRoomNo(this));
            return cPhs_ERROR_e;
        }

        if (dComIfGs_isSwitch(field_0x5cf, fopAcM_GetRoomNo(this))) {
            return cPhs_ERROR_e;
        }

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x4A20)) {
            return cPhs_ERROR_e;
        }

        if (!hio_set) {
            hio_set = TRUE;
            mHIOInit = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("ガーゴイル", &l_HIO);
        }

        field_0x5ce = 0x7C;
        field_0x5d0 = (fopAcM_GetParam(this) & 0xFF000000) >> 0x18;

        gravity = 0.0f;

        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        attention_info.distances[fopAc_attn_BATTLE_e] = 82;

        fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -700.0f, 0.0f, -700.0f);
        fopAcM_SetMax(this, 700.0f, 700.0f, 700.0f);

        mAcchCir.SetWall(50.0f, 150.0f);

        if (mType != TYPE_L7_MBOSS && mType != TYPE_DEFAULT) {
            mAcch.ClrRoofNone();
            mAcch.SetRoofCrrHeight(200.0f);
        }

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);

        cXyz sp34(0.0f, 0.0f, -57.0f);
        field_0x6bc = 0;
        field_0x5e0 = 0;

        if (mType == TYPE_L7_MBOSS || mType == TYPE_DEFAULT) {
            if (dComIfGp_getStartStagePoint() == 2 || dComIfGp_getStartStagePoint() == -1) {
                cDmr_SkipInfo = 0;
                cXyz sp28(0.0f, 0.0f, -300.0f);

                mAction = ACTION_FLY;
                mSubAction = SUBACT_WAIT;
                mMode = 1;
                mCamMode = 0;

                cXyz sp1C(0.0f, 300.0f + TREG_F(0), -500.0f);
                cLib_offsetPos(&current.pos, &sp34, cLib_targetAngleY(&sp28, &sp34), &sp1C);
                Z2GetAudioMgr()->subBgmStart(Z2BGM_GG_MBOSS);
                old.pos = current.pos;
                SetAnm(BCK_GGA_WAIT, 2, 5.0f, 1.0f);

                mCcCyl.OnTgShield();
                mCcShieldSph.OnTgShield();

                shape_angle.y = cLib_targetAngleY(&current.pos, &sp34);
                field_0x6be = 0;

                mTimers[0] = 30;
                mTimers[1] = 0;

                if (field_0x5ce != 0xFF && !dComIfGs_isSwitch(142, fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(142, fopAcM_GetRoomNo(this));
                }

                field_0x560 = l_HIO.max_hp;
                health = l_HIO.max_hp;

                setBaseMtx();
                mType = TYPE_L7_MBOSS;
                dComIfGs_onSwitch(226, fopAcM_GetRoomNo(this));
            } else {
                mAction = ACTION_DEMO;
                mSubAction = SUBACT_WAIT;
                mMode = 10;
                mCamMode = 0;

                current.pos = in_pos[2];
                current.pos.y += 10000.0f;
                old.pos = current.pos;

                mType = TYPE_L7_MBOSS;
            }
        } else if (mType == TYPE_L7_W) {
            mType = TYPE_L7_W;
            field_0x560 = 0.5f * l_HIO.max_hp;
            health = 0.5f * l_HIO.max_hp;
            mAction = ACTION_FLY;
            mSubAction = SUBACT_WAIT;
            mMode = 0;
        } else if (mType == TYPE_L7_M) {
            mType = TYPE_L7_M;
            field_0x560 = 0.5f * l_HIO.max_hp;
            health = 0.5f * l_HIO.max_hp;
            mAction = ACTION_FLY;
            mSubAction = SUBACT_WAIT;
            mMode = 0;
        } else if (mType == TYPE_HYRULE_CASTLE) {
            field_0x560 = 0.5f * l_HIO.max_hp;
            health = 0.5f * l_HIO.max_hp;
            mAction = ACTION_DEMO;
            mSubAction = 2;
            mMode = 0;

            current.pos.set(-7853.0f + HREG_F(0), -60535.0f + HREG_F(1), 8619.0f + HREG_F(2));
            old.pos = current.pos;

            if (cDmr_SkipInfo != 0) {
                cDmr_SkipInfo = 0;
                mAction = ACTION_FLY;
                mSubAction = SUBACT_WAIT;
                mMode = 0;
                current.pos.set(5930.0f, 3157.0f, 6993.0f);
                old.pos = current.pos;
            }
        } else if (mType == TYPE_CAVE) {
            mType = TYPE_L7_M;
            field_0x560 = 0.5f * l_HIO.max_hp;
            health = 0.5f * l_HIO.max_hp;
            mAction = ACTION_STOP;
            mSubAction = SUBACT_WAIT;
            mMode = 0;

            SetAnm(BCK_GG_WAIT, 2, 5.0f, 1.0f);
            fopAcM_OnStatus(this, 0x200000);

            if (-G_CM3D_F_INF != mAcch.GetGroundH()) {
                current.pos.y = mAcch.GetGroundH();
                field_0x648 = mAcch.GetGroundH();
            }
        }

        mSound.setEnemyName("B_GG");
        mSound.init(&current.pos, &eyePos, 3, 1);
        mAtInfo.mpSound = &mSound;
        mAtInfo.mPowerType = 1;

        initCc();
    
        fopAcM_setCullSizeFar(this, 3.0f);
    
        J3DModel* model = mpModelMorf->getModel();
        model->setUserArea((uintptr_t)this);

        for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
            if (i != 0) {
                model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
            }
        }

        field_0x5ac = 150.0f;

        if (mType == TYPE_L7_MBOSS) {
            mpHelmetModel->setUserArea((uintptr_t)this);
            mpHelmetModel->getModelData()->getJointNodePointer(1)->setCallBack(HeadJointCallBack);
            mpHelmetModel->getModelData()->getJointNodePointer(2)->setCallBack(HeadJointCallBack);
        }

        daB_GG_Execute(this);
    }

    return phase_state;
}

static int daB_GG_Create(fopAc_ac_c* i_this) {
    return ((daB_GG_c*)i_this)->Create();
}

static actor_method_class l_daB_GG_Method = {
    (process_method_func)daB_GG_Create,
    (process_method_func)daB_GG_Delete,
    (process_method_func)daB_GG_Execute,
    (process_method_func)daB_GG_IsDelete,
    (process_method_func)daB_GG_Draw,
};

actor_process_profile_definition g_profile_B_GG = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_B_GG,              // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daB_GG_c),       // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    230,                    // mPriority
    &l_daB_GG_Method,       // sub_method
    0x000C0100,             // mStatus
    fopAc_ENEMY_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES
