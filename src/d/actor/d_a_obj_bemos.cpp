/**
 * @file d_a_obj_bemos.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "SSystem/SComponent/c_lib.h"
#include "d/actor/d_a_obj_bemos.h"
#include "d/d_camera.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#if DEBUG
#include "d/d_debug_viewer.h"
#endif
#include "d/d_s_play.h"
#include "Z2AudioLib/Z2Instances.h"

static Vec const l_eye_offset = {0.0f, 330.0f, 75.0f};

static Vec const l_high_beam_offset = {0.0f, 255.0f, 65.0f};

static Vec const l_mid_beam_offset = {0.0f, 205.0f, 65.0f};

static Vec const l_low_beam_offset = {0.0f, 160.0f, 65.0f};

static Vec const l_craw_offset[4] = {
    {-95.0f, 0.0f, 0.0f},
    {95.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, -95.0f},
    {0.0f, 0.0f, 95.0f},
};

s16 const daObjBm_c::M_dir_base[4] = {
    0x0000,
    0x4000,
    0x8000,
    0xC000,
};

#if DEBUG
class daObjBm_HIO_c : public mDoHIO_entry_c {
public:
    daObjBm_HIO_c();

    void genMessage(JORMContext*);

    /* 0x08 */ cXyz mBeamScale;
    /* 0x14 */ f32 mBeamLowerDistance;
    /* 0x18 */ f32 mBeamMiddleDistance;
    /* 0x1C */ f32 field_0x1C;
    /* 0x20 */ f32 mSearchDistance;
    /* 0x24 */ f32 mTargetOffsetDistance;
    /* 0x28 */ s16 mSearchAngle;
    /* 0x2A */ s16 mPatrolRotationSpeed;
    /* 0x0C */ s16 mAttackRotationSpeed;
    /* 0x2E */ s16 mNoSearchTimeAfterSwitchActivation;
    /* 0x30 */ s16 mDetectionAngle;
    /* 0x32 */ s16 mSearchSpeedOnDetection;
    /* 0x34 */ f32 mBodyScale;
    /* 0x38 */ f32 mSoundPositionMoveSpeed;
    /* 0x3C */ u8 mNoSearchAttackTime;
    /* 0x3D */ u8 mDebugDraw;
    /* 0x3E */ u8 mCheckDraw;
};

daObjBm_HIO_c l_HIO;

daObjBm_HIO_c::daObjBm_HIO_c() {
    mBeamScale.set(0.5f, 0.5f, 1.0f);
    mSearchDistance = 1000.0f;
    mSearchAngle = 30000;
    mPatrolRotationSpeed = 500;
    mAttackRotationSpeed = 450;
    mBeamLowerDistance = 300.0f;
    mBeamMiddleDistance = 600.0f;
    field_0x1C = 1000.0f;
    mTargetOffsetDistance = 0.0f;
    mNoSearchTimeAfterSwitchActivation = 20;
    mDetectionAngle = 25000;
    mSoundPositionMoveSpeed = 60.0f;
    mNoSearchAttackTime = 0;
    mSearchSpeedOnDetection = 1000;
    mBodyScale = 1.0f;
    mDebugDraw = 0;
    mCheckDraw = 0;
}

void daObjBm_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("ビーモス", 0, 0, NULL, -1, -1, 512, 24);
    ctx->genCheckBox("デバッグ描画", &mDebugDraw, 1, 0, NULL, -1, -1, 512, 24);
    ctx->genSlider("サーチ距離", &mSearchDistance, 100.0f, 5000.0f, 0, NULL, -1, -1, 512, 24);
    ctx->genSlider("サーチ角度", &mSearchAngle, 0.0f, 32767.0f, 0, NULL, -1, -1, 512, 24);
    ctx->genSlider("発見角度", &mDetectionAngle, 0.0f, 32767.0f, 0, NULL, -1, -1, 512, 24);
    ctx->genSlider("巡回回転速度", &mPatrolRotationSpeed, 0.0f, 1000.0f, 0, NULL, -1, -1, 512, 24);
    ctx->genSlider("攻撃回転速度", &mAttackRotationSpeed, 0.0f, 1000.0f, 0, NULL, -1, -1, 512, 24);
    ctx->genSlider("ビーム発射・下距離", &mBeamLowerDistance, 0.0f, 1000.0f, 0, NULL, -1, -1, 512,
                   24);
    ctx->genSlider("ビーム発射・中距離", &mBeamMiddleDistance, 0.0f, 1000.0f, 0, NULL, -1, -1, 512,
                   24);
    ctx->genSlider("目標オフセット距離", &mTargetOffsetDistance, 0.0f, 500.0f, 0, NULL, -1, -1, 512,
                   24);
    ctx->genSlider("ＳＷ起動後サーチ無し時間", &mNoSearchTimeAfterSwitchActivation, 0.0f, 1000.0f,
                   0, NULL, -1, -1, 512, 24);
    ctx->genSlider("発音位置移動速度", &mSoundPositionMoveSpeed, 0.0f, 1000.0f, 0, NULL, -1, -1,
                   512, 24);
    ctx->genSlider("サーチ無し攻撃時間", &mNoSearchAttackTime, 0.0f, 100.0f, 0, NULL, -1, -1, 512,
                   24);
    ctx->genSlider("発見時のサーチ速度", &mSearchSpeedOnDetection, 0.0f, 10000.0f, 0, NULL, -1, -1,
                   512, 24);
    ctx->genSlider("本体スケール", &mBodyScale, 0.1f, 5.0f, 0, NULL, -1, -1, 512, 24);
    ctx->genSlider("ビームＸスケール", &mBeamScale.x, 0.1f, 10.0f, 0, NULL, -1, -1, 512, 24);
    ctx->genSlider("ビームＹスケール", &mBeamScale.y, 0.1f, 10.0f, 0, NULL, -1, -1, 512, 24);
    ctx->genCheckBox("チェック描画", &mCheckDraw, 1, 0, NULL, -1, -1, 512, 24);
}
#endif

fopAc_ac_c* daObjBm_c::PPCallBack(fopAc_ac_c* i_bgActor, fopAc_ac_c* i_actorP, s16 param_2,
                                  dBgW_Base::PushPullLabel pp_field) {
    dBgW::PushPullLabel pp_label = cLib_checkBit<dBgW::PushPullLabel>(pp_field, dBgW::PPLABEL_3);
    u8 l_swNo3 = ((daObjBm_c*)i_bgActor)->getSwNo3();
    u8 l_moveType = ((daObjBm_c*)i_bgActor)->getMoveType();

    if (pp_label != dBgW::PPLABEL_NONE && !fopAcM_isSwitch(i_bgActor, l_swNo3)) {
        s16 spE = cLib_checkBit<dBgW::PushPullLabel>(pp_label, dBgW::PPLABEL_PULL) ?
                      (s16)param_2 - 0x8000 :
                      (s16)param_2;
        s16 angle = spE - ((daObjBm_c*)i_bgActor)->home.angle.y;

        JUT_ASSERT(513, pp_label != pp_field);

        ((daObjBm_c*)i_bgActor)->mPPLabel = pp_field;

        if (angle >= -0x2000 && angle < 0x2000) {
            pp_label = dBgW::PPLABEL_NONE;
        } else if (angle >= 0x2000 && angle < 0x6000) {
            pp_label = dBgW::PPLABEL_PUSH;
        } else if (angle >= 0x6000 || angle < -0x6000) {
            pp_label = dBgW::PPLABEL_PULL;
        } else {
            pp_label = dBgW::PPLABEL_3;
        }

        if (l_moveType != 0 || (l_moveType == 0 && pp_label == dBgW::PPLABEL_NONE)) {
            for (int i = 0; i < 4; i++) {
                if (i == pp_label) {
                    ((daObjBm_c*)i_bgActor)->mMomentCnt[i]++;
                } else {
                    ((daObjBm_c*)i_bgActor)->mMomentCnt[i] = 0;
                }
            }

            ((daObjBm_c*)i_bgActor)->field_0x10d2 = 1;
        }
    }

    return i_bgActor;
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        daObjBm_c* i_this = (daObjBm_c*)model->getUserArea();

        cMtx_copy(model->getAnmMtx(jnt_no), mDoMtx_stack_c::get());
        s16 sVar1 = i_this->field_0x1000 * cM_scos(i_this->field_0xff0 * 0x2cec + KREG_S(6));

        if (jnt_no == i_this->getHeadJoint()) {
            mDoMtx_stack_c::XrotM(i_this->field_0xf96 + sVar1);
        } else if (jnt_no == i_this->getBigGearJoint()) {
            mDoMtx_stack_c::XrotM(i_this->field_0xf9a);
        } else if (jnt_no == i_this->getSmallGear0Joint()) {
            mDoMtx_stack_c::XrotM(i_this->field_0xf9e);
        } else if (jnt_no == i_this->getSmallGear1Joint()) {
            mDoMtx_stack_c::XrotM(i_this->field_0xfa2);
        } else if (jnt_no == i_this->getSmallGear2Joint()) {
            mDoMtx_stack_c::XrotM(i_this->field_0xfa6);
        }

        cMtx_copy(mDoMtx_stack_c::get(), model->getAnmMtx(jnt_no));
    }

    return 1;
}

void daObjBm_c::initBaseMtx() {
    field_0x10a0.set(0.5f, 0.5f, 1.0f);
    mpModel->setBaseScale(scale);
    mpModel3->setBaseScale(scale);
    mpModel2->setBaseScale(field_0x10a0);
    setBaseMtx();
}

void daObjBm_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::scaleM(1.0f, 1.0f, 1.0f);
    PSMTXCopy(mDoMtx_stack_c::get(), mBgMtx);

#if DEBUG
    field_0x10b8 = l_HIO.mBeamScale.x;
    field_0x10bc = l_HIO.mBeamScale.y;
#endif

    mpModel2->setBaseScale(field_0x10a0);
    mDoMtx_stack_c::transS(field_0xfb8);
    mDoMtx_stack_c::ZXYrotM(field_0xfc4);
    mpModel2->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(home.angle);
    mpModel3->setBaseTRMtx(mDoMtx_stack_c::get());
}

static const char* l_arcName = "Obj_bm";

static const char* l_eye_matName = "bmEye";

static char* l_head_joint = "head";

static char* l_bigGear_joint = "cogRed";

static char* l_smallGear0_joint = "cogBlue";

static char* l_smallGear1_joint = "cogGreen";

static char* l_smallGear2_joint = "cogYellow";

static char* l_joint_table[5] = {
    l_head_joint, l_bigGear_joint, l_smallGear0_joint, l_smallGear1_joint, l_smallGear2_joint,
};

static cull_box l_cull_box = {
    {-1200.0f, -100.0f, -1200.0f},
    {1200.0f, 400.0f, 1200.0f},
};

static dCcD_SrcSph l_sph_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x402020, 0x11}, 0x58}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                // mGObjTg
        {0x0},                                             // mGObjCo
    },                                                     // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 50.0f}  // mSph
    }  // mSphAttr
};

static dCcD_SrcCps l_cps_src = {
    {
        {0x0, {{0x100, 0x1, 0x1d}, {0x0, 0x0}, 0x0}},  // mObj
        {dCcD_SE_SWORD, 0x0, 0x0, 0x1, 0x0},           // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},            // mGObjTg
        {0x0},                                         // mGObjCo
    },                                                 // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 20.0f},  // mCps
    }  // mCpsAttr
};

static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x1f}, {0x0, 0x0}, 0x79}},  // mObj
        {dCcD_SE_NONE, 0x1, 0x0, 0x0, 0x0},           // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},           // mGObjTg
        {0x0},                                        // mGObjCo
    },                                                // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        15.0f,               // mRadius
        60.0f                // mHeight
    }  // mCyl
};

int daObjBm_c::Create() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    mStts.Init(0xff, 0xff, this);
    mSph.Set(l_sph_src);
    mSph.SetStts(&mStts);
    mCps.Set(l_cps_src);
    mCps.SetStts(&mStts);

    for (int i = 0; i < 4; i++) {
        mCyl[i].Set(l_cyl_src);
        mCyl[i].SetStts(&mStts);
    }

    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x,
                          l_cull_box.max.y, l_cull_box.max.z);
    JUTNameTab* joint_name = mpModel->getModelData()->getJointTree().getJointName();
    for (int i = 0; i < 5; i++) {
        for (u16 j = 0; j < mpModel->getModelData()->getJointNum(); j++) {
            if (strcmp(joint_name->getName(j), l_joint_table[i]) == 0) {
                mJoints[i] = j;
                mpModel->getModelData()->getJointNodePointer(j)->setCallBack(nodeCallBack);
            }
        }
    }

    mpModel->setUserArea((uintptr_t)this);
    JUTNameTab* material_name = mpModel->getModelData()->getMaterialTable().getMaterialName();

    for (u16 i = 0; i < mpModel->getModelData()->getMaterialNum(); i++) {
        if (strcmp(material_name->getName(i), l_eye_matName) == 0) {
            mpMaterial = mpModel->getModelData()->getMaterialNodePointer(i);
        }
    }

#if DEBUG
    if (field_0xfcc.x == 0) {
        OSReport_Error("ブ［モス]。苔のマテリアルがありません");
        return 0;
    }
#endif

    mSound.init(&field_0x1048, 1);
    mSound2.init(&field_0x1054, 1);

    field_0x1039 = 1;

    if (fopAcM_isSwitch(this, getSwNo2())) {
        field_0xff7 = 0x0;

        if (getMoveType() != 1) {
            mpBckAnm3->setFrame(mpBckAnm3->getEndFrame());
        }

        initActionObjBemos();
    } else {
        initActionEnBemos();

        if (getSwNo() != 0xFF && !fopAcM_isSwitch(this, getSwNo())) {
            field_0xff7 = 0;
            initActionSwWait();
        } else {
            field_0xff7 = 1;
            J3DAnmTevRegKey* model_data = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 0x14);
            mpBrkAnm->init(mpModel->getModelData(), model_data, 1, 2, 1.0f, 0, -1);
            mpBckAnm->setFrame(mpBckAnm->getEndFrame());
            initActionWarning();
        }
    }

    initBaseMtx();
    return 1;
}

int daObjBm_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 0xd);
    JUT_ASSERT(767, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 0x13);
    JUT_ASSERT(780, pbrk != NULL);
    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL || mpBrkAnm->init(modelData, pbrk, 1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }

    J3DAnmTransform* pbck;
#if DEBUG
    pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 7);
    JUT_ASSERT(791, pbck != NULL);
#endif

    pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 7);
    JUT_ASSERT(798, pbck != NULL);
    mpBckAnm = new mDoExt_bckAnm();
    if (mpBckAnm == NULL || mpBckAnm->init(pbck, 1, 0, 1.0f, 0, -1, false) == 0) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 0xe);
    JUT_ASSERT(813, modelData != NULL);
    mpModel2 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (mpModel2 == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 0x1a);
    JUT_ASSERT(827, pbtk != NULL);
    mpBtkAnm = new mDoExt_btkAnm();
    if (mpBtkAnm == NULL || mpBtkAnm->init(modelData, pbtk, 1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }
    mpBtkAnm->setFrame(mpBtkAnm->getEndFrame());

#if DEBUG
    pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 0x18);
    JUT_ASSERT(840, pbtk != NULL);
#endif

    pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 0x18);
    JUT_ASSERT(846, pbtk != NULL);
    mpBtkAnm2 = new mDoExt_btkAnm();
    if (mpBtkAnm2 == NULL || mpBtkAnm2->init(modelData, pbtk, 1, 2, 1.0f, 0, -1) == 0) {
        return 0;
    }

    J3DAnmTransform* pbck2 = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 9);
    JUT_ASSERT(858, pbck != NULL);
    mpBckAnm2 = new mDoExt_bckAnm();
    if (mpBckAnm2 == NULL || mpBckAnm2->init(pbck2, 1, 2, 1.0f, 0, -1, false) == 0) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 0xf);
    JUT_ASSERT(873, modelData != NULL);
    mpModel3 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel3 == NULL) {
        return 0;
    }

    J3DAnmTransform* pbck3 = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 8);
    JUT_ASSERT(885, pbck3 != NULL);
    mpBckAnm3 = new mDoExt_bckAnm();
    if (mpBckAnm3 == NULL || mpBckAnm3->init(pbck3, 1, 0, 1.0f, 0, -1, false) == 0) {
        return 0;
    }
    mpBckAnm3->setPlaySpeed(0.0f);

    return 1;
}

int daObjBm_c::create1st() {
    if (field_0x100e == 0) {
        field_0x100a = home.angle.x;
        field_0x100c = home.angle.z;
        home.angle.x = home.angle.z = 0;
        current.angle.x = current.angle.z = 0;
        shape_angle.x = shape_angle.z = 0;
        field_0x100e = 1;
    }

    cPhs__Step phase_state = (cPhs__Step)dComIfG_resLoad(&mPhase, l_arcName);

    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state =
            (cPhs__Step)MoveBGCreate(l_arcName, 0x1e, dBgS_MoveBGProc_TypicalRotY, 0x5500, NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }

        OS_REPORT("新ビーモス：0x%x\n", fopAcM_GetParam(this));
#if DEBUG
        l_HIO.entryHIO("ビーモス");
#endif
    }

    return phase_state;
}

int daObjBm_c::Execute(Mtx** param_0) {
#if DEBUG
    scale.setall(l_HIO.mBodyScale);
    mpModel->setBaseScale(scale);
    daPy_py_c* player = daPy_getPlayerActorClass();
#endif

    field_0xff0++;

    if (field_0xf96 != field_0xf98 && field_0xfaa == 0) {
        field_0xfaa = 0xFA;
    }

    cLib_chaseAngleS(&field_0xf96, field_0xf98, field_0xfaa);
    cLib_chaseAngleS(&field_0xf9a, field_0xf9c, field_0xfae);
    cLib_chaseAngleS(&field_0xf9e, field_0xfa0, field_0xfae);
    cLib_chaseAngleS(&field_0xfa2, field_0xfa4, field_0xfae);
    cLib_chaseAngleS(&field_0xfa6, field_0xfa8, field_0xfae);

    main_proc_call();

    eyePos = l_eye_offset;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(home.angle.y + field_0xf96);
    mDoMtx_stack_c::multVec(&eyePos, &eyePos);
    attention_info.position = eyePos;

    *param_0 = &mBgMtx;
    setBaseMtx();

    cLib_addCalcAngleS(&field_0xfaa, field_0xfac, 0x1e, 0x14, 10);
    cLib_addCalcAngleS(&field_0xfae, field_0xfb0, 0x1e, 0x14, 10);
    cLib_addCalc(&field_0x1000, 0.0f, 0.2f, KREG_F(2) + 50.0f, KREG_F(3) + 10.0f);
    field_0xfb2 = field_0xf96;

    if (mpBckAnm3->getFrame() == 0.0f) {
        setCrawCO();
    }

    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mSound2.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    return 1;
}

void daObjBm_c::main_proc_call() {
    static daObjBm_c::procFunc l_func[] = {
        &daObjBm_c::actionEnBemos,
        &daObjBm_c::actionObjBemos,
    };

    (this->*l_func[field_0xfeb])();
}

void daObjBm_c::initActionEnBemos() {
    if (mpBgW && mpBgW->ChkUsed()) {
        mpBgW->OffPushPullOk();
    }

    field_0xfeb = 0;
}

void daObjBm_c::initActionObjBemos() {
    if (mpBgW && mpBgW->ChkUsed()) {
        mpBgW->OnPushPullOk();
    }

    mpBgW->SetPushPullCallback((dBgW_Base::PushPull_CallBack)PPCallBack);

    u8 move_type = getMoveType();
    switch (move_type) {
    case 0:
        if (fopAcM_isSwitch(this, getSwNo3())) {
            field_0x10b4 = 4;
            mode_wait();
            mode_dead_init();
        }
        break;
    case 1:
        if (fopAcM_isSwitch(this, getSwNo2())) {
            mode_wait();
            mode_dead_init();
        }
        break;
    }

    field_0xfeb = 1;
}

void daObjBm_c::actionEnBemos() {
    static daObjBm_c::procFunc l_func[] = {
        &daObjBm_c::actionSwWait, &daObjBm_c::actionWarning, &daObjBm_c::actionFindPlayer,
        &daObjBm_c::actionAttack, &daObjBm_c::actionDead,
    };

    if (mSph.ChkTgHit()) {
        cCcD_Obj* hitobj = mSph.GetTgHitObj();

        if (hitobj) {
            if (hitobj->ChkAtType(AT_TYPE_ARROW) || hitobj->ChkAtType(AT_TYPE_BOMB) ||
                hitobj->ChkAtType(AT_TYPE_IRON_BALL))
            {
                mDoAud_seStart(Z2SE_EN_BM_EYE_BREAK, &eyePos, 0,
                               dComIfGp_getReverb((int)fopAcM_GetRoomNo(this)));
                initEffectSet0();
                fopAcM_onSwitch(this, getSwNo2());
                initActionDead();
                OS_REPORT("ビーモス死亡！<%d>\n", getSwNo2());
            }
        }
    }

    mSph.ClrTgHit();
    calcBeamPos();
    effect_proc();
    (this->*l_func[field_0xfe4])();
    calcBeamLenAndAt();
    mpBckAnm2->play();
    mpBtkAnm->play();
    mpBtkAnm2->play();
    mpBrkAnm->play();
    mpBckAnm->play();
    mpBckAnm3->play();

    if (field_0xfaa != 0) {
        fopAcM_seStartLevel(this, Z2SE_EN_BM_TURN, 0);
    }

    if (field_0xfe4 != 4 && field_0xfe4 != 0) {
        mSph.SetC(eyePos);
        dComIfG_Ccsp()->Set(&mSph);
    }

    if (field_0xfe4 == 3) {
        dComIfG_Ccsp()->Set(&mCps);
        mSound.startLevelSound(Z2SE_EN_BM_BEAM, 0, -1);
    }
}

void daObjBm_c::actionObjBemos() {
    static daObjBm_c::modeFunc mode_proc[] = {
        &daObjBm_c::mode_wait,
        &daObjBm_c::mode_walk,
        &daObjBm_c::mode_afl,
        &daObjBm_c::mode_dead,
    };

    (this->*mode_proc[field_0x10ac])();
}

void daObjBm_c::setCrawCO() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(home.angle.y);
    cXyz acStack_28;

    for (int i = 0; i < 4; i++) {
        mDoMtx_stack_c::multVec(&l_craw_offset[i], &acStack_28);
        mCyl[i].SetC(acStack_28);
        dComIfG_Ccsp()->Set(&mCyl[i]);
    }
}

void daObjBm_c::calcBeamPos() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 dist = fopAcM_searchPlayerDistanceXZ(this);
    cXyz cStack_30 = l_low_beam_offset;

    if (dist > 600.0f) {
        cStack_30 = l_high_beam_offset;
    } else {
        if (dist > 300.0f) {
            cStack_30 = l_mid_beam_offset;
        }
    }

    s16 sVar2 = field_0xf96;
    cLib_chasePos(&field_0xfd8, cStack_30, 5.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(sVar2 + current.angle.y);
    mDoMtx_stack_c::multVec(&field_0xfd8, &field_0xfb8);

    f32 fVar1 = field_0x1004;
    if (fVar1 < 300.0f)
        fVar1 = 300.0f;

    cXyz cStack_3c(0.0f, 0.0f, fVar1);
    mDoMtx_stack_c::multVec(&cStack_3c, &field_0xfcc);
    field_0xfcc = player->current.pos;
    cXyz cStack_48 = field_0xfb8 - field_0xfcc;
    field_0xfc4.x = cM_atan2s(cStack_48.y, field_0xfcc.absXZ(field_0xfb8));
    field_0xfc4.y = sVar2 + current.angle.y;
    field_0xfc4.z = 0;

    if (field_0xfc4.x < cM_deg2s(DREG_F(15) + 20.0f))
        field_0xfc4.x = cM_deg2s(DREG_F(15) + 20.0f);

    mDoMtx_stack_c::transS(field_0xfb8);
    mDoMtx_stack_c::ZXYrotM(field_0xfc4.x, sVar2 + current.angle.y, 0);
    mDoMtx_stack_c::multVec(&cStack_3c, &field_0xfcc);
}

void daObjBm_c::calcBeamLenAndAt() {
    cXyz cStack_50 = field_0xfcc - field_0xfb8;
    cXyz cStack_5c(field_0xfb8);
    cStack_50.normalizeZP();
    cStack_5c += cStack_50 * 1200.0f;

#if DEBUG
    f32 dVar4 = mpBtkAnm->getFrame();
#endif

    if (fopAcM_lc_c::lineCheck(&field_0xfb8, &cStack_5c, this) && fopAcM_lc_c::checkGroundHit()) {
        field_0x10a0.z = field_0xfb8.abs(fopAcM_lc_c::getCross()) / 1200.0f;
    } else {
        field_0x10a0.z = 1.0f;
    }

    cStack_5c = field_0xfb8 + (cStack_50 * (field_0x10a0.z * 1200.0f));
    mCps.cM3dGCps::Set(field_0xfb8, cStack_5c, 20.0f);
    mCps.CalcAtVec();

    if (field_0x1039 > 0) {
        cXyz cStack_68(field_0xfcc);

        if (fopAcM_lc_c::lineCheck(&field_0xfb8, &cStack_5c, this) && fopAcM_lc_c::checkGroundHit())
        {
            cStack_68 = fopAcM_lc_c::getCross();
        }

        if (cLib_chasePos(&field_0x1048, cStack_68, 60.0f) != 0) {
            field_0x1039 = -1;
        }
    } else {
        if (cLib_chasePos(&field_0x1048, cStack_5c, 60.0f) != 0) {
            field_0x1039 = 1;
        }
    }
}

s8 daObjBm_c::checkFindPlayer() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 searchDistance = getSearchDistance();
#if DEBUG
    s16 search_angle = l_HIO.mDetectionAngle;
#else
    s16 search_angle = 25000;
#endif

    s8 ret = -1;

    field_0xfe8 = 0;

    f32 playerDistanceXZ = fopAcM_searchPlayerDistanceXZ(this);
    f32 fVar1 = current.pos.y - player->current.pos.y;
    if (playerDistanceXZ > searchDistance) {
        return ret;
    } else {
        if (fVar1 < 0.0f) {
            if (std::fabs(fVar1) > (1.0f - (playerDistanceXZ / searchDistance)) * l_eye_offset.y) {
                return ret;
            }
        }

        if ((s16)abs((s16)(fopAcM_searchPlayerAngleY(this) - field_0xf96 - home.angle.y)) <
            (s16)(search_angle / 2))
        {
            ret = 0;
        }

        if (ret == 0) {
            cXyz eye(eyePos);
            cXyz eye2(player->eyePos);

            if (fopAcM_lc_c::lineCheck(&eye, &eye2, this)) {
                field_0xfe8 = 1;
            }
        }
    }

    return ret;
}

s8 daObjBm_c::checkSearchPlayer() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 searchDistance = getSearchDistance();
#if DEBUG
    s16 search_angle = l_HIO.mSearchAngle;
#else
    s16 search_angle = 30000;
#endif

    s8 ret = -1;

    field_0xfe8 = 0;

    f32 playerDistanceXZ = fopAcM_searchPlayerDistanceXZ(this);
    f32 fVar1 = current.pos.y - player->current.pos.y;
    if (playerDistanceXZ > searchDistance) {
        return ret;
    }

    if (fVar1 < 0.0f) {
        if (std::fabs(fVar1) > (1.0f - (playerDistanceXZ / searchDistance)) * l_eye_offset.y) {
            return ret;
        }
    }

    if ((s16)abs((s16)(fopAcM_searchPlayerAngleY(this) - field_0xf96 - home.angle.y)) <
        (s16)(search_angle / 2))
    {
        ret = 0;
    }

    if (ret == 0) {
        cXyz eye(eyePos);
        cXyz eye2(player->eyePos);

        if (fopAcM_lc_c::lineCheck(&eye, &eye2, this)) {
            field_0xfe8 = 1;
        }
    }

    return ret;
}

f32 daObjBm_c::getSearchDistance() {
#if DEBUG
    f32 ret = l_HIO.mSearchDistance;
#else
    f32 ret = 1000.0f;
#endif

    if (getSearchDist() != 15 && getSearchDist()) {
        ret -= getSearchDist() * 50.0f;
    }

    return ret;
}

f32 daObjBm_c::getBeamSearchDistance() {
#if DEBUG
    f32 ret = l_HIO.mSearchDistance;
#else
    f32 ret = 1000.0f;
#endif

    if (getBeamSearchDist() != 15 && getBeamSearchDist()) {
        ret -= getBeamSearchDist() * 50.0f;
    }

    return ret;
}

void daObjBm_c::effect_proc() {
    static daObjBm_c::effectFunc l_eff_func[4] = {
        &daObjBm_c::effectWait,
        &daObjBm_c::effectSet0,
        &daObjBm_c::effectSet1,
        &daObjBm_c::effectEnd,
    };

    (this->*l_eff_func[field_0xff6])();

    for (int i = 0; i < 2; i++) {
        if (field_0xff8[i] != NULL) {
            field_0xff8[i]->setGlobalRTMatrix(mpModel->getAnmMtx(getHeadJoint()));

            if (cLib_calcTimer(&field_0xff5) == 0) {
                field_0xff8[i]->becomeInvalidEmitter();
                field_0xff8[i]->quitImmortalEmitter();
                field_0xff8[i] = NULL;
            }
        }
    }

    if (field_0x1034 != NULL) {
        J3DJoint* joint = mpModel->getModelData()->getJointNodePointer(getHeadJoint());
        J3DTransformInfo* info = &joint->getTransformInfo();
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(home.angle.y);
        mDoMtx_stack_c::transM(info->mTranslate.y, info->mTranslate.x, info->mTranslate.z);
        mDoMtx_stack_c::YrotM(field_0xf96);
        field_0x1034->setGlobalRTMatrix(mDoMtx_stack_c::get());
    }

    if (field_0x102c != NULL) {
        mDoMtx_stack_c::copy(mpModel->getAnmMtx((u16)getBigGearJoint()));
        mDoMtx_stack_c::multVecZero(&field_0x103c);
        mDoAud_seStartLevel(Z2SE_EN_BM_SPARK, &field_0x103c, 0, 0);
    }
}

daObjBm_c::BgcSrc_c daObjBm_c::Bgc_c::M_lin5[] = {
    {0.0f, 0.0f, 0.0f, 0.0f}, {-0.5f, -0.5f, -1.0f, -1.0f}, {0.5f, -0.5f, 1.0f, -1.0f},
    {0.5f, 0.5f, 1.0f, 1.0f}, {-0.5f, 0.5f, -1.0f, 1.0f},
};

void daObjBm_c::effectWait() {}

void daObjBm_c::initEffectSet0() {
    static u16 const eff_id_f[2] = {
        dPa_RM(ID_ZF_S_BM_BROKENEYE_F_L),
        dPa_RM(ID_ZF_S_BM_BROKENEYE_F_S),
    };

    static u16 const hit_eff_id[4] = {
        dPa_RM(ID_ZF_S_BM_BROKENEYE_HIT01),
        dPa_RM(ID_ZF_S_BM_BROKENEYE_HIT02),
        dPa_RM(ID_ZF_S_BM_BROKENEYE_HIT03),
        dPa_RM(ID_ZF_S_BM_BROKENEYE_HIT04),
    };

    csXyz i_rotation(home.angle);
    i_rotation.y += field_0xf96;

    for (int i = 0; i < 2; i++) {
        dComIfGp_particle_set(eff_id_f[i], &eyePos, &i_rotation, &scale, 0xFF, NULL, -1, NULL, NULL,
                              NULL);
    }

    for (int i = 0; i < 4; i++) {
        dComIfGp_particle_set(hit_eff_id[i], &eyePos, &i_rotation, &scale, 0xFF, NULL, -1, NULL,
                              NULL, NULL);
    }

    field_0xff8[0] = dComIfGp_particle_set(dPa_RM(ID_ZF_S_BM_BROKENSMK_F), &current.pos,
                                           &home.angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
    if (field_0xff8[0] != NULL) {
        field_0xff8[0]->becomeImmortalEmitter();
    }

    field_0xff5 = 200;
    field_0xff6 = 1;
}

void daObjBm_c::effectSet0() {}

void daObjBm_c::effectSet1() {}

void daObjBm_c::effectEnd() {}

int daObjBm_c::check_to_walk() {
    int ret = -1;
    bool bVar1 = true;

    if (field_0x10d2) {
        dBgW::PushPullLabel iVar2 =
            cLib_checkBit<dBgW::PushPullLabel>(mPPLabel, dBgW::PPLABEL_PULL);
        dBgW::PushPullLabel iVar3 = cLib_checkBit<dBgW::PushPullLabel>(mPPLabel, dBgW::PPLABEL_4);

        s16 sVar5;
        if (iVar2) {
            if (iVar3) {
                sVar5 = 8;
            } else {
                sVar5 = 0x13;
            }
        } else if (iVar3) {
            sVar5 = 8;
        } else {
            sVar5 = 0x13;
        }

        for (int i = 0; i < 4; i++) {
            if (mMomentCnt[i] >= sVar5) {
                if (!mBgc.chk_wall_pre(this, mBgc.M_lin5, 5, M_dir_base[i])) {
                    ret = i;
                }
            } else {
                bVar1 = false;
            }
        }
    }

    if (bVar1) {
        clr_moment_cnt();
    }

    field_0x10d2 = 0;
    return ret;
}

void daObjBm_c::mode_wait_init() {
    fopAcM_SetSpeedF(this, 0.0f);
    fopAcM_SetGravity(this, -4.0f);
    mpBgW->SetCrrFunc(dBgS_MoveBGProc_Trans);
    clr_moment_cnt();
    field_0x10bc = -1;
    field_0x10ac = 0;
}

void daObjBm_c::mode_wait() {
    f32 tmp1 = l_eye_offset.x;
    f32 tmp2 = l_eye_offset.z;

    if (field_0x10ce > 0)
        field_0x10ce--;

    int walk_check = check_to_walk();
    cXyz local_38;
    mDoMtx_stack_c::transS(home.pos);
    mDoMtx_stack_c::YrotM(home.angle.y);
    mDoMtx_stack_c::transM(field_0x10b0 * 75.0f, 0.0f, field_0x10b4 * 75.0f);
    mDoMtx_stack_c::multVecZero(&local_38);
    current.pos.x = local_38.x;
    current.pos.z = local_38.z;

    if (walk_check != -1) {
        field_0x10bc = walk_check;

        ((daPy_py_c*)dComIfGp_getPlayer(0))->onPushPullKeep();
        mode_walk_init();

        if (cLib_checkBit<dBgW::PushPullLabel>(mPPLabel, dBgW::PPLABEL_PULL) != 0) {
            field_0x10cc = 13;
            field_0x10b8 = 2520.6155f;
        } else {
            field_0x10cc = 13;
            field_0x10b8 = 2520.6155f;
        }
    }
}

void daObjBm_c::mode_walk_init() {
    fopAcM_SetSpeedF(this, 0.0f);
    field_0x10ac = 1;
}

// static u8 dir_vec[48];

void daObjBm_c::mode_walk() {
    static cXyz dir_vec[] = {
        cXyz(0.0f, 0.0f, 1.0f),
        cXyz(1.0f, 0.0f, 0.0f),
        cXyz(0.0f, 0.0f, -1.0f),
        cXyz(-1.0f, 0.0f, 0.0f),
    };

    bool uVar5 = --field_0x10cc <= 0;
    f32 fVar6 = (cM_scos(field_0x10cc * field_0x10b8) + 1.0f) * 0.5f;

    cXyz local_58;
    mDoMtx_stack_c::transS(home.pos);
    mDoMtx_stack_c::YrotM(home.angle.y);
    mDoMtx_stack_c::transM((field_0x10b0 + fVar6 * dir_vec[field_0x10bc].x) * 75.0f, 0.0f,
                           (field_0x10b4 + fVar6 * dir_vec[field_0x10bc].z) * 75.0f);
    mDoMtx_stack_c::multVecZero(&local_58);

    current.pos.x = local_58.x;
    current.pos.z = local_58.z;
    eyePos = current.pos;

    if (uVar5 != 0) {
        mBgc.chk_wall_pre(this, mBgc.M_lin5, 5, M_dir_base[field_0x10bc]);
    }

    current.pos.x = local_58.x;
    current.pos.z = local_58.z;
    fopAcM_seStartLevel(this, Z2SE_EN_BM_SLIDE, 0);

    if (uVar5 != 0) {
        if (field_0x10bc == 0) {
            field_0x10b4++;
        } else if (field_0x10bc == 1) {
            field_0x10b0++;
        } else if (field_0x10bc == 2) {
            field_0x10b4--;
        } else if (field_0x10bc == 3) {
            field_0x10b0--;
        }

        ((daPy_py_c*)dComIfGp_getPlayer(0))->offPushPullKeep();

        if (getMoveType() == 0 && field_0x10b4 == 4) {
            fopAcM_onSwitch(this, getSwNo3());
            mode_dead_init();
        } else {
            mode_wait_init();
        }
    }
}

void daObjBm_c::mode_afl() {
    fopAcM_SetSpeedF(this, 0.0f);
    field_0x10ac = 2;
}

void daObjBm_c::mode_dead_init() {
    mpBgW->OffPushPullOk();
    fopAcM_SetSpeedF(this, 0.0f);
    field_0x10ac = 3;
}

void daObjBm_c::mode_dead() {}

void daObjBm_c::clr_moment_cnt() {
    for (int i = 0; i < 4; i++) {
        mMomentCnt[i] = 0;
    }
}

daObjBm_c::Bgc_c::Bgc_c() {
    for (int i = 0; i < 23; i++) {
        field_0x0[i] = 0.0f;
        field_0x64[i] = cXyz::Zero;
    }

    field_0x5c = 0xFFFFFFFF;
    field_0x178 = 0xFFFFFFFF;
    field_0x17c = FLT_MAX;
    field_0x60 = 0.0f;
    mState = STATE_0_e;
}

const daObjBm_c::BgcSrc_c daObjBm_c::Bgc_c::M_lin20[] = {
    {0.0f, 0.0f, 0.0f, 0.0f},    {-0.25f, -0.25f, 0.0f, 0.0f}, {-0.5f, -0.5f, -1.0f, -1.0f},
    {0.0f, -0.5f, 1.0f, -1.0f},  {0.0f, 0.0f, 1.0f, 1.0f},     {-0.5f, 0.0f, -1.0f, 1.0f},
    {0.25f, -0.25f, 0.0f, 0.0f}, {0.0f, -0.5f, -1.0f, -1.0f},  {0.5f, -0.5f, 1.0f, -1.0f},
    {0.5f, 0.0f, 1.0f, 1.0f},    {0.0f, 0.0f, -1.0f, 1.0f},    {0.25f, 0.25f, 0.0f, 0.0f},
    {0.0f, 0.0f, -1.0f, -1.0f},  {0.5f, 0.0f, 1.0f, -1.0f},    {0.5f, 0.5f, 1.0f, 1.0f},
    {0.0f, 0.5f, -1.0f, 1.0f},   {-0.25f, 0.25f, 0.0f, 0.0f},  {-0.5f, 0.0f, -1.0f, -1.0f},
    {0.0f, 0.0f, 1.0f, -1.0f},   {0.0f, 0.5f, 1.0f, 1.0f},     {-0.5f, 0.5f, -1.0f, 1.0f},
    {-0.75f, 0.25f, 0.0f, 0.0f}, {-0.75f, -0.25f, 0.0f, 0.0f},
};

static f32 dummy_rodata_5157() {
    return -G_CM3D_F_INF;
}

dBgS_ObjGndChk daObjBm_c::Bgc_c::M_gnd_work[23];

dBgS_WtrChk daObjBm_c::Bgc_c::M_wrt_work;

dBgS_ObjLinChk daObjBm_c::Bgc_c::M_wall_work[23];

static f32 dummy_rodata_5158() {
    // needed for correct rodata ordering:
    return 200.0f;
}

void daObjBm_c::Bgc_c::wall_pos(fopAc_ac_c const* i_actor, daObjBm_c::BgcSrc_c const* i_bgcSrc,
                                int i_num, s16 param_3, f32 param_4) {
    const s16 sp8C = i_actor->home.angle.y + param_3;

    cXyz sp48;
    cXyz sp54;
    cXyz sp60;
    cXyz sp6C;
    cXyz sp78;

    field_0x178 = -1;
    field_0x17c = FLT_MAX;

    mDoMtx_stack_c::YrotS(sp8C);
    mDoMtx_stack_c::XrotM(0x4000);
    mDoMtx_stack_c::multVec(&cXyz::BaseY, &sp6C);
    sp6C *= param_4 + 100.0f;

    for (int i = 0; i < i_num; i++, i_bgcSrc++) {
        mDoMtx_stack_c::XrotS(0x4000);
        sp78.set(i_bgcSrc->field_0xc, 0.0f, i_bgcSrc->field_0x8);
        mDoMtx_stack_c::multVec(&sp78, &sp60);

        mDoMtx_stack_c::YrotS(sp8C);
        mDoMtx_stack_c::transM(sp60);
        mDoMtx_stack_c::scaleM(200.0f, 400.0f, 200.0f);
        mDoMtx_stack_c::transM(0.0f, 0.5f, 0.0f);
        mDoMtx_stack_c::XrotM(0x4000);

        sp78.set(i_bgcSrc->field_0x4, 0.0f, i_bgcSrc->field_0x0);
        mDoMtx_stack_c::multVec(&sp78, &sp48);
        sp48 += i_actor->current.pos;
        sp54 = sp48 + sp6C;

        M_wall_work[i].Set(&sp48, &sp54, i_actor);
        M_wall_work[i].SetActorPid(i_actor->base.base.id);

        if (dComIfG_Bgsp().LineCross(&M_wall_work[i])) {
            field_0x64[i] = M_wall_work[i].GetCross();

            f32 var_f31 = sp48.abs2(field_0x64[i]);
            if (var_f31 < field_0x17c) {
                field_0x17c = var_f31;
                field_0x178 = i;
            }
        } else {
            field_0x64[i] = cXyz::Zero;
        }
    }
}

bool daObjBm_c::Bgc_c::chk_wall_pre(fopAc_ac_c const* i_actor, daObjBm_c::BgcSrc_c const* i_bgcSrc,
                                    int i_num, s16 param_3) {
    wall_pos(i_actor, i_bgcSrc, i_num, param_3, 76.0f);
    return field_0x178 >= 0;
}

void daObjBm_c::initActionSwWait() {
    field_0xfaa = 0;
    field_0xfac = 0;
    field_0xfae = field_0xfaa;
    field_0xfae = field_0xfac;
    mpBrkAnm->setPlaySpeed(0.0f);
    mpBckAnm->setPlaySpeed(0.0f);
    field_0xfea = 0;
    field_0xfe4 = 0;
}

void daObjBm_c::actionSwWait() {
    cXyz i_sePos;
    switch (field_0xfea) {
    case 0:
        if (fopAcM_isSwitch(this, getSwNo())) {
            mpBrkAnm->init(mpModel->getModelData(),
                           (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 0x15), 1, 0, 1.0f, 0,
                           -1);
            i_sePos = home.pos;
            i_sePos.y += 200.0f;
            mDoAud_seStart(Z2SE_EN_BM_BOOT, &i_sePos, 0,
                           dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            field_0xff7 = 1;
            field_0xfea = 1;
        }
        break;

    case 1:
        if (mpBrkAnm->isStop()) {
            field_0xfe5 = 0;
            field_0xfea = 2;
        }
        break;

    case 2:
        if (cLib_calcTimer(&field_0xfe5) == 0) {
            mpBtkAnm->init(mpModel2->getModelData(),
                           (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 0x19), 1, 0, 0.0f,
                           0, -1);
            mpBtkAnm2->setPlaySpeed(0.0f);
            mpBrkAnm->init(mpModel->getModelData(),
                           (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 0x14), 1, 2, 1.0f, 0,
                           -1);
            mpBckAnm->init((J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 7), 1, 0, 1.0f, 0, -1,
                           true);
            fopAcM_seStart(this, Z2SE_EN_BM_UP, 0);
            field_0x1008 = 20;
            initActionWarning();
        }
        break;
    }
}

void daObjBm_c::initActionWarning() {
    if (field_0x102c) {
        field_0x102c->becomeInvalidEmitter();
        field_0x102c->quitImmortalEmitter();
        field_0x102c = NULL;
    }

    if (field_0x1030) {
        field_0x1030->becomeInvalidEmitter();
        field_0x1030->quitImmortalEmitter();
        field_0x1030 = NULL;
    }

    field_0xfea = 0;
    field_0xfe4 = 1;
}

void daObjBm_c::actionWarning() {
#if DEBUG
    field_0xfac = l_HIO.mPatrolRotationSpeed;
#else
    field_0xfac = 500;
#endif

    switch (field_0xfea) {
    case 0:
        field_0xfea = 1;
        break;

    case 1:
        if (cLib_calcTimer(&field_0x1008) == 0) {
            field_0xfe7 = checkFindPlayer();
            if (field_0xfe7 != -1 && field_0xfe8 == 0) {
                field_0xfe5 = 0;
                initActionFindPlayer();
            }
        }
        break;
    }

    field_0xfb0 = field_0xfac + 100;

    if (field_0xfea != 5) {
        field_0xf98 = field_0xf96 + field_0xfaa;
    }

    field_0xf9c = field_0xf9a - field_0xfae;
    field_0xfa0 = field_0xf9e + field_0xfae;
    field_0xfa4 = field_0xfa2 + field_0xfae;
    field_0xfa8 = field_0xfa6 + field_0xfae;
}

void daObjBm_c::initActionFindPlayer() {
    mpBrkAnm->init(mpModel->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 0x13),
                   1, 0, 1.0f, 0, -1);
    field_0x1034 = dComIfGp_particle_set(dPa_RM(ID_ZF_S_BM_NESSENHEAT), &current.pos, 0, 0, 0xff, 0,
                                         -1, 0, 0, 0);
    field_0x1038 = -1;
    fopAcM_seStart(this, Z2SE_EN_BM_FIND, 0);
    fopAcM_seStart(this, Z2SE_EN_BM_HEAT, 0);
    field_0xfe5 = 0;
    field_0xfea = 0;
#if DEBUG
    field_0xfac = l_HIO.mSearchSpeedOnDetection;
#else
    field_0xfac = 1000;
#endif
    field_0xfaa = field_0xfac;
    field_0xfb0 = field_0xfac;
    field_0xfe4 = 2;
}

void daObjBm_c::actionFindPlayer() {
    switch (field_0xfea) {
    case 0:
        field_0xf98 = fopAcM_searchPlayerAngleY(this) - home.angle.y;
        field_0xf9c = field_0xf9a - field_0xfae;
        field_0xfa0 = field_0xf9e + field_0xfae;
        field_0xfa4 = field_0xfa2 + field_0xfae;
        field_0xfa8 = field_0xfa6 + field_0xfae;

        if ((s16)abs((s16)(fopAcM_searchPlayerAngleY(this) - field_0xf96 - home.angle.y)) <
            KREG_S(4) + 2000)
        {
            field_0xfea = 1;
        }
        break;
    case 1:
        field_0xf98 = field_0xf96;
        field_0xfac = 0;
        field_0xfaa = 0;
        field_0x1000 = KREG_F(10) + 300.0f;
        field_0xfea = 2;
        break;
    case 2:
        field_0xf98 = field_0xf96;
        field_0xfac = 0;
        field_0xfaa = 0;

        if (field_0x1000 == 0.0f) {
            field_0xfe5 = 0;
            initActionAttack();
        }
    }
}

static f32 dummy_rodata_5299() {
    return 110.0f;
}

void daObjBm_c::initActionAttack() {
    static u16 const l_eff_id[2] = {
        dPa_RM(ID_ZF_S_BM_NESSENSRC00),
        dPa_RM(ID_ZF_S_BM_NESSENSRC01),
    };

    f32 dist = fopAcM_searchPlayerDistanceXZ(this);
    field_0x1004 = dist;  // debug diff here
    daPy_py_c* player = daPy_getPlayerActorClass();

    mpBtkAnm->init(mpModel2->getModelData(),
                   (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 0x19), 1, 0, 1.0f, 0, -1);
    mpBtkAnm2->setPlaySpeed(1.0f);

    field_0xfe5 = 0x14;
    field_0xfe6 = 0;  // debug diff here

    if (field_0x102c == NULL)
        field_0x102c = dComIfGp_particle_set(dPa_RM(ID_ZF_S_BM_GEARSPARK), &current.pos, NULL, NULL,
                                             0xff, NULL, -1, NULL, NULL, NULL);

    if (field_0x1030 == NULL)
        field_0x1030 = dComIfGp_particle_set(dPa_RM(ID_ZF_S_BM_GEARROCK), &current.pos, NULL, NULL,
                                             0xff, NULL, -1, NULL, NULL, NULL);

    if (field_0x102c != NULL) {
        field_0x102c->setRateStep(9);
        field_0x102c->setLifeTime(6);
        field_0x102c->setRate(6.5f);
    }

    if (field_0x1030 != NULL) {
        field_0x1030->setRateStep(2);
        field_0x1030->setAwayFromCenterSpeed(32.0f);
    }

    for (int i = 0; i < 2; i++) {
        csXyz cStack_20(0, 0, 0);
        cStack_20.y = home.angle.y + field_0xf96;
        field_0x1018[i] = dComIfGp_particle_set(l_eff_id[i], &field_0xfb8, &cStack_20, NULL, 0xff,
                                                NULL, -1, NULL, NULL, NULL);
    }

    field_0xfed = 0;
    field_0xfea = 0;
    field_0xfe4 = 3;
}

void daObjBm_c::actionAttack() {
    static u16 const l_eff_id[2] = {
        dPa_RM(ID_ZF_S_BM_NESSENBURN00),
        dPa_RM(ID_ZF_S_BM_NESSENBURN01),
    };

    static u16 const l_eff_id2[3] = {
        dPa_RM(ID_ZF_S_BM_NESSENSPLASH00),
        dPa_RM(ID_ZF_S_BM_NESSENSPLASH01),
        dPa_RM(ID_ZF_S_BM_NESSENSPLASH02),
    };

    f32 dVar17 = fopAcM_searchPlayerDistanceXZ(this);
    f32 dVar18 = getBeamSearchDistance();

    if (dVar17 > dVar18) {
        dVar17 = getBeamSearchDistance();
    }

    bool bVar4 = true;

    if (checkSearchPlayer() == -1 && cLib_calcTimer(&field_0xfe5) == 0 || field_0xfe8 != 0) {
        bVar4 = false;
    }

    switch (field_0xfea) {
    case 0:
        if (cLib_calcTimer(&field_0xfe6) == 0) {
            field_0xfea = 2;
        }
        break;
    case 1:
        field_0xfac = 200 - KREG_S(0);  // debug diff here
        field_0xf98 = fopAcM_searchPlayerAngleY(this) - home.angle.y;
        cLib_chaseF(&field_0x1004, dVar17, KREG_F(16) + 5.0f);

        if (!bVar4) {
            field_0xfea = 3;
        }

        if (cLib_calcTimer(&field_0xfec) == 0) {
            field_0xfea = 2;
        }
        break;
    case 2:
        field_0xfac = 0x1c2;  // debug diff here
        field_0xf98 = fopAcM_searchPlayerAngleY(this) - home.angle.y;
        cLib_chaseF(&field_0x1004, dVar17, KREG_F(16) + 15.0f);

        if (!bVar4) {
            field_0xfea = 0x3;
        }
        break;
    case 3:
        mpBtkAnm->init(mpModel2->getModelData(),
                       (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 0x1a), 1, 0, 1.0f, 0,
                       -1);
        mpBrkAnm->init(mpModel->getModelData(),
                       (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 0x14), 1, 2, 1.0f, 0, -1);
        field_0x1038 = 0x14;

        if (field_0x1034 != NULL) {
            field_0x1034->becomeInvalidEmitter();
            field_0x1034->quitImmortalEmitter();
            field_0x1034 = NULL;
        }

        for (int i = 0; i < 2; i++) {
            if (field_0x1018[i] != NULL) {
                field_0x1018[i]->becomeInvalidEmitter();
                field_0x1018[i]->quitImmortalEmitter();
                field_0x1018[i] = NULL;
            }
        }

        field_0xfea = 4;
        break;
    case 4:
        if (mpBtkAnm->isStop()) {
            for (int i = 0; i < 2; i++) {
                if (field_0x1010[i] != NULL) {
                    field_0x1010[i]->becomeInvalidEmitter();
                    field_0x1010[i]->quitImmortalEmitter();
                    field_0x1010[i] = NULL;
                }
            }

            for (int i = 0; i < 3; i++) {
                if (field_0x1020[i] != NULL) {
                    field_0x1020[i]->becomeInvalidEmitter();
                    field_0x1020[i]->quitImmortalEmitter();
                    field_0x1020[i] = NULL;
                }
            }

            initActionWarning();
        }
    }

    for (int i = 0; i < 2; i++) {  // local_a4
        csXyz cStack_8c(0, 0, 0);
        cStack_8c.y = home.angle.y + field_0xf96;
        mDoMtx_stack_c::transS(field_0xfb8);
        mDoMtx_stack_c::YrotM(cStack_8c.y);

        if (field_0x1018[i] != 0) {
            field_0x1018[i]->setGlobalRTMatrix(mDoMtx_stack_c::get());
        }
    }

    field_0x1054 = home.pos;
    cXyz cStack60 = field_0xfcc - field_0xfb8;
    cXyz cStack_6c(field_0xfb8);
    cStack60.normalizeZP();
    cStack_6c += cStack60 * 1200.0f;

    if (fopAcM_lc_c::lineCheck(&field_0xfb8, &cStack_6c, this) && fopAcM_lc_c::checkGroundHit()) {
        if (field_0xfed == 0) {
            for (int i = 0; i < 2; i++) {
                if (field_0x1010[i] == NULL) {
                    field_0x1010[i] =
                        dComIfGp_particle_set(l_eff_id[i], fopAcM_lc_c::getCrossP(), NULL, NULL,
                                              0xff, NULL, -1, NULL, NULL, NULL);
                }
            }

            for (int i = 0; i < 3; i++) {
                if (field_0x1020[i] == NULL) {
                    field_0x1020[i] =
                        dComIfGp_particle_set(l_eff_id2[i], fopAcM_lc_c::getCrossP(), NULL, NULL,
                                              0xff, NULL, -1, NULL, NULL, NULL);
                }
            }

            field_0xfed = 1;
        } else {
            cXyz* pfVar7 = fopAcM_lc_c::getCrossP();
            f32 fVar2 = pfVar7->z;
            pfVar7 = fopAcM_lc_c::getCrossP();
            f32 fVar3 = pfVar7->y;
            cXyz* pfVar9 = fopAcM_lc_c::getCrossP();
            mDoMtx_stack_c::transS(pfVar9->x, fVar3 + 2.0f, fVar2);
            cM3dGPla cStack_54;
            csXyz local_94;

            if (fopAcM_lc_c::checkGroundHit() && fopAcM_lc_c::getTriPla(&cStack_54)) {
                cM3d_CalcVecAngle(cStack_54.mNormal, &local_94.x, &local_94.z);
                mDoMtx_stack_c::XYZrotM(local_94.x, 0, local_94.z);
            }

            for (int i = 0; i < 2; i++) {
                if (field_0x1010[i] != NULL) {
                    field_0x1010[i]->setGlobalRTMatrix(mDoMtx_stack_c::get());
                }
            }

            for (int i = 0; i < 3; i++) {
                if (field_0x1020[i] != NULL) {
                    field_0x1020[i]->setGlobalRTMatrix(mDoMtx_stack_c::get());
                }
            }

            field_0x1054 = fopAcM_lc_c::getCross();
            field_0x1054.y += 2.0f;
        }
    }

    if (field_0x1054 != home.pos) {
        mSound2.startLevelSound(Z2SE_EN_BM_BURN, 0, -1);
    }

    if (mpBrkAnm->isStop()) {
        mpBrkAnm->init(mpModel->getModelData(),
                       (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 0x12), 1, 2, 1.0f, 0, -1);
    }

    field_0xfb0 = field_0xfac * 3;
    field_0xf9c = field_0xf9a - field_0xfae;
    field_0xfa0 = field_0xf9e + field_0xfae;
    field_0xfa4 = field_0xfa2 + field_0xfae;
    field_0xfa8 = field_0xfa6 + field_0xfae;

    if (checkLockOnCamera() == 0) {
        dCam_getBody()->ForceLockOn(this);
    }
}

void daObjBm_c::initActionDead() {
    if (field_0xf96 > -0x4000 && field_0xf96 < 0x4000) {
        field_0xf98 = 0;
    } else {
        field_0xf98 = 0x7FFF;
    }

    field_0xfaa = 0;
    field_0xfac = 0;

    mpBckAnm->init((J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 10), 1, 0, 0.0f, 0, -1, true);
    if (mpBtkAnm->getBtkAnm() == (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 0x19)) {
        mpBtkAnm->init(mpModel2->getModelData(),
                       (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 0x1a), 1, 0, 1.0f, 0,
                       -1);
    }

    field_0x1038 = 0x14;

    for (int i = 0; i < 2; i++) {
        if (field_0x1018[i] != NULL) {
            field_0x1018[i]->becomeInvalidEmitter();
            field_0x1018[i]->quitImmortalEmitter();
            field_0x1018[i] = NULL;
        }

        if (field_0x1010[i] != NULL) {
            field_0x1010[i]->becomeInvalidEmitter();
            field_0x1010[i]->quitImmortalEmitter();
            field_0x1010[i] = NULL;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (field_0x1020[i] != NULL) {
            field_0x1020[i]->becomeInvalidEmitter();
            field_0x1020[i]->quitImmortalEmitter();
            field_0x1020[i] = NULL;
        }
    }

    if (field_0x1034 != NULL) {
        field_0x1034->becomeInvalidEmitter();
        field_0x1034->quitImmortalEmitter();
        field_0x1034 = NULL;
    }

    field_0xfe5 = 20;
    field_0xfe9 = 0;
    field_0xff7 = 0;
    fopAcM_OffStatus(this, 0);
    field_0xfea = 0;
    field_0xfe4 = 4;
}

void daObjBm_c::actionDead() {
    switch (field_0xfea) {
    case 0:
        if (!cLib_calcTimer(&field_0xfe5)) {
            field_0xfea = 1;
        }
        break;

    case 1:
        field_0xfac = 500;
        if (field_0x102c != NULL) {
            field_0x102c->becomeInvalidEmitter();
            field_0x102c->quitImmortalEmitter();
            field_0x102c = NULL;
        }

        if (field_0x1030 != NULL) {
            field_0x1030->becomeInvalidEmitter();
            field_0x1030->quitImmortalEmitter();
            field_0x1030 = NULL;
        }

        field_0xfea = 2;
        break;

    case 2:
        cLib_addCalcAngleS(&field_0xfac, 0, 0x5A, 10, 5);
        if (field_0xf96 == field_0xf98) {
            field_0xfac = 0;
            field_0x1000 = 200.0f;
            field_0xfea = 3;
            fopAcM_seStart(this, Z2SE_EN_BM_TURN_END, 0);
        }
        break;

    case 3:
        field_0xfe5 = 20;
        field_0xfea = 4;
        break;

    case 4:
        if (!cLib_calcTimer(&field_0xfe5)) {
            mpBckAnm->setPlaySpeed(1.0f);
            dComIfGp_particle_set(dPa_RM(ID_ZF_S_BM_SMOKE), &current.pos, NULL, NULL, 0xff, NULL,
                                  -1, NULL, NULL, NULL);
            field_0xfea = 5;
        }
        break;

    case 5:
        if (mpBckAnm->isStop()) {
            field_0xfe5 = 10;
            fopAcM_seStart(this, Z2SE_EN_BM_DOWN, 0);
            field_0xfea = 6;
            fopAcM_seStart(this, Z2SE_EN_BM_UNLOCK, 0);
        }
        break;

    case 6:
        if (getMoveType() == 1) {
            initActionObjBemos();
        } else {
            if (cLib_calcTimer(&field_0xfe5) == 0) {
                mpBckAnm3->setPlaySpeed(1.0f);

                if (mpBckAnm3->isStop()) {
                    field_0xfea = 7;
                }
            }
        }

        break;
    case 7:
        initActionObjBemos();
    }

    field_0xfb0 = field_0xfac;
    field_0xf9c = field_0xf9a - field_0xfae;
    field_0xfa0 = field_0xf9e + field_0xfae;
    field_0xfa4 = field_0xfa2 + field_0xfae;
    field_0xfa8 = field_0xfa6 + field_0xfae;
}

int daObjBm_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();

    J3DShape* shape = mpMaterial->getShape();
    if (field_0xff7 == 0) {
        shape->hide();
    }

    mpBrkAnm->entry(mpModel->getModelData());
    mpBckAnm->entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    mDoExt_brkAnmRemove(mpModel->getModelData());
    mDoExt_bckAnmRemove(mpModel->getModelData());
    shape->show();
    dComIfGd_setList();
    g_env_light.setLightTevColorType_MAJI(mpModel2, &tevStr);

    mpBtkAnm2->entry(mpModel2->getModelData());
    mpBtkAnm->entry(mpModel2->getModelData());
    mpBckAnm2->entry(mpModel2->getModelData());
    mDoExt_modelUpdateDL(mpModel2);
    mDoExt_btkAnmRemove(mpModel2->getModelData());
    mDoExt_bckAnmRemove(mpModel2->getModelData());

    g_env_light.setLightTevColorType_MAJI(mpModel3, &tevStr);
    mpBckAnm3->entry(mpModel3->getModelData());
    mDoExt_modelUpdateDL(mpModel3);

#if DEBUG
    debugDraw();
#endif

    return 1;
}

#if DEBUG
void daObjBm_c::debugDraw() {
    if (l_HIO.mDebugDraw) {
        GXColor local_4c = {
            0xff,
            0,
            0,
            0x80,
        };

        GXColor local_50 = {
            0xff,
            0xff,
            0xff,
            0x80,
        };

        cXyz* pVVar2 = mCps.GetAtVecP();

        if (pVVar2 != NULL) {
            cXyz cStack_24 = mCps.GetStart();
            cXyz cStack_30 = mCps.GetStart();
            cStack_30 += *pVVar2;
            dDbVw_drawArrowXlu(cStack_24, cStack_30, local_4c, 1, 0xc);
        }

        cXyz cStack_3c(current.pos);
        cStack_3c.y += 50.0f;

        dDbVw_drawCircleXlu(cStack_3c, getSearchDistance(), local_4c, 1, 0xc);
        dDbVw_drawCircleXlu(cStack_3c, getBeamSearchDistance(), local_50, 1, 0xc);
        dDbVw_drawCircleXlu(cStack_3c, l_HIO.mBeamMiddleDistance, local_4c, 1, 0xc);
        dDbVw_drawCircleXlu(cStack_3c, l_HIO.mBeamLowerDistance, local_4c, 1, 0xc);

        GXColor local_54 = {
            0,
            0,
            0xff,
            0xff,
        };

        local_4c = local_54;
        cXyz cStack_48;

        cStack_48.set(0.0f, 10.0f, getSearchDistance());

        s16 sVar1 = l_HIO.mSearchAngle / 2;
        mDoMtx_stack_c::YrotS((home.angle.y + field_0xf96) + sVar1);
        mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
        cStack_48 += cStack_3c;
        dDbVw_drawLineOpa(cStack_3c, cStack_48, local_4c, 1, 0xc);
        cStack_48.set(0.0f, 10.0f, getSearchDistance());

        mDoMtx_stack_c::YrotS((home.angle.y + field_0xf96) - sVar1);
        mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
        cStack_48 += cStack_3c;
        dDbVw_drawLineOpa(cStack_3c, cStack_48, local_4c, 1, 0xc);

        GXColor local_58 = {
            0,
            0xff,
            0xff,
            0xff,
        };

        local_4c = local_58;
        cStack_48.set(0.0f, 10.0f, getSearchDistance());

        sVar1 = l_HIO.mDetectionAngle / 2;
        mDoMtx_stack_c::YrotS((home.angle.y + field_0xf96) + sVar1);
        mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
        cStack_48 += cStack_3c;
        dDbVw_drawLineOpa(cStack_3c, cStack_48, local_4c, 1, 0xc);

        cStack_48.set(0.0f, 10.0f, getSearchDistance());
        mDoMtx_stack_c::YrotS((home.angle.y + field_0xf96) - sVar1);
        mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
        cStack_48 += cStack_3c;
        dDbVw_drawLineOpa(cStack_3c, cStack_48, local_4c, 1, 0xc);

        GXColor local_5c = {
            0xff,
            0xff,
            0xff,
            0xff,
        };

        dDbVw_drawSphereOpa(field_0xfcc, 0.0f, local_5c, 1);
    }

    if (l_HIO.mCheckDraw) {
        mpBgW->CalcPlane();
    }
}
#endif

int daObjBm_c::Delete() {
    for (int i = 0; i < 2; i++) {
        if (field_0xff8[i]) {
            field_0xff8[i]->becomeInvalidEmitter();
            field_0xff8[i]->quitImmortalEmitter();
            field_0xff8[i] = NULL;
        }
    }

    if (field_0x102c) {
        field_0x102c->becomeInvalidEmitter();
        field_0x102c->quitImmortalEmitter();
        field_0x102c = NULL;
    }

    if (field_0x1030) {
        field_0x1030->becomeInvalidEmitter();
        field_0x1030->quitImmortalEmitter();
        field_0x1030 = NULL;
    }

    if (field_0x1034) {
        field_0x1034->becomeInvalidEmitter();
        field_0x1034->quitImmortalEmitter();
        field_0x1034 = NULL;
    }

    for (int i = 0; i < 2; i++) {
        if (field_0x1010[i]) {
            field_0x1010[i]->becomeInvalidEmitter();
            field_0x1010[i]->quitImmortalEmitter();
            field_0x1010[i] = NULL;
        }

        if (field_0x1018[i]) {
            field_0x1018[i]->becomeInvalidEmitter();
            field_0x1018[i]->quitImmortalEmitter();
            field_0x1018[i] = NULL;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (field_0x1020[i]) {
            field_0x1020[i]->becomeInvalidEmitter();
            field_0x1020[i]->quitImmortalEmitter();
            field_0x1020[i] = NULL;
        }
    }

    mpBgW->SetPushPullCallback(NULL);
    mSound.deleteObject();
    mSound2.deleteObject();
    dComIfG_resDelete(&mPhase, l_arcName);
#if DEBUG
    l_HIO.removeHIO();
#endif
    return 1;
}

static int daObjBm_create1st(daObjBm_c* i_this) {
    fopAcM_ct(i_this, daObjBm_c);
    return i_this->create1st();
}

static int daObjBm_MoveBGDelete(daObjBm_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjBm_MoveBGExecute(daObjBm_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjBm_MoveBGDraw(daObjBm_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjBm_METHODS = {
    (process_method_func)daObjBm_create1st,     (process_method_func)daObjBm_MoveBGDelete,
    (process_method_func)daObjBm_MoveBGExecute, 0,
    (process_method_func)daObjBm_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_Bemos = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Bemos,          // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjBm_c),       // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    604,                     // mPriority
    &daObjBm_METHODS,        // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

static void dummy_touch_work() {
    static dBgS_ObjLinChk touch_work;
}

AUDIO_INSTANCES;
