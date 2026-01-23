/**
 * @file d_a_obj_bemos.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "SSystem/SComponent/c_lib.h"
#include "d/actor/d_a_obj_bemos.h"
#include "res/Object/Obj_bm.h"
#include "d/d_camera.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#if DEBUG
#include "d/d_debug_viewer.h"
#endif
#include "d/d_s_play.h"
#include "Z2AudioLib/Z2Instances.h"

enum Action_e {
    /* 0x0 */ ACTION_SW_WAIT_e,
    /* 0x1 */ ACTION_WARNING_e,
    /* 0x2 */ ACTION_FIND_PLAYER_e,
    /* 0x3 */ ACTION_ATTACK_e,
    /* 0x4 */ ACTION_DEAD_e,
};

enum Mode_e {
    /* 0x0 */ MODE_WAIT_e,
    /* 0x1 */ MODE_WALK_e,
    /* 0x2 */ MODE_AFL_e,
    /* 0x3 */ MODE_DEAD_e,
};

#if DEBUG
#pragma nosyminline on
#endif

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
    -0x8000,
    -0x4000,
};

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
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            15.0f,               // mRadius
            60.0f                // mHeight
        }  // mCyl
    }
};


#if DEBUG
class daObjBm_HIO_c : public mDoHIO_entry_c {
public:
    daObjBm_HIO_c();

    void genMessage(JORMContext*);

    /* 0x08 */ cXyz beam_scale;
    /* 0x14 */ f32 beam_lower_dist;
    /* 0x18 */ f32 beam_middle_dist;
    /* 0x1C */ f32 field_0x1C;
    /* 0x20 */ f32 search_dist;
    /* 0x24 */ f32 target_offset_dist;
    /* 0x28 */ s16 search_angle;
    /* 0x2A */ s16 patrol_rot_spd;
    /* 0x2C */ s16 attack_rot_spd;
    /* 0x2E */ s16 after_switch_activate_no_search_time;
    /* 0x30 */ s16 detection_angle;
    /* 0x32 */ s16 search_spd_on_detect;
    /* 0x34 */ f32 body_scale;
    /* 0x38 */ f32 sound_pos_move_spd;
    /* 0x3C */ u8 no_search_attack_time;
    /* 0x3D */ u8 debug_draw;
    /* 0x3E */ u8 check_draw;
};

daObjBm_HIO_c l_HIO;

daObjBm_HIO_c::daObjBm_HIO_c() {
    beam_scale.set(0.5f, 0.5f, 1.0f);
    search_dist = 1000.0f;
    search_angle = 30000;
    patrol_rot_spd = 500;
    attack_rot_spd = 450;
    beam_lower_dist = 300.0f;
    beam_middle_dist = 600.0f;
    field_0x1C = 1000.0f;
    target_offset_dist = 0.0f;
    after_switch_activate_no_search_time = 20;
    detection_angle = 25000;
    sound_pos_move_spd = 60.0f;
    no_search_attack_time = 0;
    search_spd_on_detect = 1000;
    body_scale = 1.0f;
    debug_draw = 0;
    check_draw = 0;
}

void daObjBm_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("ビーモス", 0);
    ctx->genCheckBox("デバッグ描画", &debug_draw, 0x1);
    ctx->genSlider("サーチ距離", &search_dist, 100.0f, 5000.0f);
    ctx->genSlider("サーチ角度", &search_angle, 0.0f, 32767.0f);
    ctx->genSlider("発見角度", &detection_angle, 0.0f, 32767.0f);
    ctx->genSlider("巡回回転速度", &patrol_rot_spd, 0.0f, 1000.0f);
    ctx->genSlider("攻撃回転速度", &attack_rot_spd, 0.0f, 1000.0f);
    ctx->genSlider("ビーム発射・下距離", &beam_lower_dist, 0.0f, 1000.0f);
    ctx->genSlider("ビーム発射・中距離", &beam_middle_dist, 0.0f, 1000.0f);
    ctx->genSlider("目標オフセット距離", &target_offset_dist, 0.0f, 500.0f);
    ctx->genSlider("ＳＷ起動後サーチ無し時間", &after_switch_activate_no_search_time, 0.0f, 1000.0f);
    ctx->genSlider("発音位置移動速度", &sound_pos_move_spd, 0.0f, 1000.0f);
    ctx->genSlider("サーチ無し攻撃時間", &no_search_attack_time, 0.0f, 100.0f);
    ctx->genSlider("発見時のサーチ速度", &search_spd_on_detect, 0.0f, 10000.0f);
    ctx->genSlider("本体スケール", &body_scale, 0.1f, 5.0f);
    ctx->genSlider("ビームＸスケール", &beam_scale.x, 0.1f, 10.0f);
    ctx->genSlider("ビームＹスケール", &beam_scale.y, 0.1f, 10.0f);
    ctx->genCheckBox("チェック描画", &check_draw, 0x1);
}
#define BEAM_SCALE_X l_HIO.beam_scale.x
#define BEAM_SCALE_Y l_HIO.beam_scale.y
#define BEAM_SCALE_Z l_HIO.beam_scale.z
#define BEAM_LOWER_DIST l_HIO.beam_lower_dist
#define BEAM_MIDDLE_DIST l_HIO.beam_middle_dist
#define SEARCH_DIST l_HIO.search_dist
#define TARGET_OFFSET_DIST l_HIO.target_offset_dist
#define SEARCH_ANGLE l_HIO.search_angle
#define PATROL_ROT_SPD l_HIO.patrol_rot_spd
#define ATTACK_ROT_SPD l_HIO.attack_rot_spd
#define AFTER_SWITCH_ACTIVATE_NO_SEARCH_TIME l_HIO.after_switch_activate_no_search_time
#define DETECTION_ANGLE l_HIO.detection_angle
#define SEARCH_SPD_ON_DETECT l_HIO.search_spd_on_detect
#define BODY_SCALE l_HIO.body_scale
#define SOUND_POS_MOVE_SPD l_HIO.sound_pos_move_spd
#define NO_SEARCH_ATTACK_TIME l_HIO.no_search_attack_time
#define DEBUG_DRAW l_HIO.debug_draw
#define CHECK_DRAW l_HIO.check_draw
#else
#define BEAM_SCALE_X 0.5f
#define BEAM_SCALE_Y 0.5f
#define BEAM_SCALE_Z 1.0f
#define BEAM_LOWER_DIST 300.0f
#define BEAM_MIDDLE_DIST 600.0f
#define SEARCH_DIST 1000.0f
#define TARGET_OFFSET_DIST 0.0f
#define SEARCH_ANGLE 30000
#define PATROL_ROT_SPD 500
#define ATTACK_ROT_SPD 450
#define AFTER_SWITCH_ACTIVATE_NO_SEARCH_TIME 20
#define DETECTION_ANGLE 25000
#define SEARCH_SPD_ON_DETECT 1000
#define BODY_SCALE 1.0f
#define SOUND_POS_MOVE_SPD 60.0f
#define NO_SEARCH_ATTACK_TIME 0
#define DEBUG_DRAW 0
#define CHECK_DRAW 0
#endif

fopAc_ac_c* daObjBm_c::PPCallBack(fopAc_ac_c* i_bgActor, fopAc_ac_c* i_actorP, s16 param_2, dBgW_Base::PushPullLabel pp_field) {
    daObjBm_c* actor_p = (daObjBm_c*)i_bgActor;
    dBgW::PushPullLabel pp_label = cLib_checkBit<dBgW::PushPullLabel>(pp_field, dBgW::PPLABEL_3);
    u8 l_swNo3 = actor_p->getSwNo3();
    u8 l_moveType = actor_p->getMoveType();

    if (pp_label != dBgW::PPLABEL_NONE && !fopAcM_isSwitch(actor_p, l_swNo3)) {
        s16 spE = cLib_checkBit<dBgW::PushPullLabel>(pp_label, dBgW::PPLABEL_PULL) ?
                      (s16)param_2 - 0x8000 : (s16)param_2;
        s16 angle = spE - actor_p->home.angle.y;

        JUT_ASSERT(513, pp_label != pp_field);

        actor_p->mPPLabel = pp_field;

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
                    actor_p->mMomentCnt[i]++;
                } else {
                    actor_p->mMomentCnt[i] = 0;
                }
            }

            actor_p->field_0x10d2 = 1;
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
        s16 sVar1 = i_this->field_0x1000 * cM_scos(i_this->field_0xff0 * 0x2CEC + KREG_S(6));

        if (jnt_no == i_this->getHeadJoint()) {
            mDoMtx_stack_c::XrotM(i_this->field_0xf96 + sVar1);
        } else if (jnt_no == i_this->getBigGearJoint()) {
            mDoMtx_stack_c::XrotM(i_this->mBigGearRotX);
        } else if (jnt_no == i_this->getSmallGear0Joint()) {
            mDoMtx_stack_c::XrotM(i_this->mSmallGear0RotX);
        } else if (jnt_no == i_this->getSmallGear1Joint()) {
            mDoMtx_stack_c::XrotM(i_this->mSmallGear1RotX);
        } else if (jnt_no == i_this->getSmallGear2Joint()) {
            mDoMtx_stack_c::XrotM(i_this->mSmallGear2RotX);
        }

        cMtx_copy(mDoMtx_stack_c::get(), model->getAnmMtx(jnt_no));
    }

    return 1;
}

void daObjBm_c::initBaseMtx() {
    mBeamScale.set(0.5f, 0.5f, 1.0f);
    mpModel->setBaseScale(scale);
    mBmfModel->setBaseScale(scale);
    mBeamModel->setBaseScale(mBeamScale);
    setBaseMtx();
}

void daObjBm_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::scaleM(1.0f, KREG_F(4) + 1.0f, 1.0f);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);

#if DEBUG
    mBeamScale.x = BEAM_SCALE_X;
    mBeamScale.y = BEAM_SCALE_Y;
#endif

    mBeamModel->setBaseScale(mBeamScale);
    mDoMtx_stack_c::transS(field_0xfb8);
    mDoMtx_stack_c::ZXYrotM(field_0xfc4);
    mBeamModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(home.angle);
    mBmfModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daObjBm_c::Create() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    mStts.Init(0xFF, 0xFF, this);
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
    if (mpMaterial == NULL) {
        OSReport_Error("ビーモス：目のマテリアルがありません\n");
        return 0;
    }
#endif

    mBeamSound.init(&mBeamSoundPos, 1);
    mBurnSound.init(&mBurnSoundPos, 1);

    field_0x1039 = 1;

    if (fopAcM_isSwitch(this, getSwNo2())) {
        mShowFlag = 0;

        if (getMoveType() != 1) {
            mBmfOffBck->setFrame(mBmfOffBck->getEndFrame());
        }

        initActionObjBemos();
    } else {
        initActionEnBemos();

        if (getSwNo() != 0xFF && !fopAcM_isSwitch(this, getSwNo())) {
            mShowFlag = 0;
            initActionSwWait();
        } else {
            mShowFlag = 1;
            mSerchBrk->init(mpModel->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BRK_TURN_e),
                            1, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);
            mBeamosBck->setFrame(mBeamosBck->getEndFrame());
            initActionWarning();
        }
    }

    initBaseMtx();
    return 1;
}

// FAKEMATCH
int daObjBm_c::CreateHeap() {
    J3DModelData* modelData;
    
    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BMD_BM_e);
    JUT_ASSERT(767, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BRK_SERCH_e);
    JUT_ASSERT(780, pbrk != NULL);
    mSerchBrk = new mDoExt_brkAnm();
    if (mSerchBrk == NULL || mSerchBrk->init(modelData, pbrk, 1, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1) == 0) {
        return 0;
    }

#if DEBUG
    pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BRK_TURN_e);
    JUT_ASSERT(791, pbrk != NULL);
#endif

    J3DAnmTransform* pbck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BCK_BM_UP_e);
    JUT_ASSERT(798, pbck != NULL);
    mBeamosBck = new mDoExt_bckAnm();
    if (mBeamosBck == NULL || mBeamosBck->init(pbck, 1, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false) == 0) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BMD_EF_BIMOBEAM_e);
    JUT_ASSERT(813, modelData != NULL);
    mBeamModel = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000284);
    if (mBeamModel == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BTK_EF_BIMOBEAMB_OFF_e);
    JUT_ASSERT(827, pbtk != NULL);
    mBeamBtk = new mDoExt_btkAnm();
    if (mBeamBtk == NULL || mBeamBtk->init(modelData, pbtk, 1, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1) == 0) {
        return 0;
    }
    mBeamBtk->setFrame(mBeamBtk->getEndFrame());

#if DEBUG
    J3DAnmTextureSRTKey* pbtk2 = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BTK_EF_BIMOBEAM_ON_e);
    JUT_ASSERT(840, pbtk != NULL);
#endif

    pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BTK_EF_BIMOBEAM_e);
    JUT_ASSERT(846, pbtk != NULL);
    mBeamEffBtk = new mDoExt_btkAnm();
    if (mBeamEffBtk == NULL || mBeamEffBtk->init(modelData, pbtk, 1, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1) == 0) {
        return 0;
    }

    J3DAnmTransform* pbck2 = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BCK_EF_BIMOBEAM_e);
    JUT_ASSERT(858, pbck != NULL);
    mBeamEffBck = new mDoExt_bckAnm();
    if (mBeamEffBck == NULL || mBeamEffBck->init(pbck2, 1, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false) == 0) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BMD_P_BMF_e);
    JUT_ASSERT(873, modelData != NULL);
    mBmfModel = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
    if (mBmfModel == NULL) {
        return 0;
    }

    J3DAnmTransform* pbck3 = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BCK_BMF_OFF_e);
    JUT_ASSERT(885, pbck != NULL);
    mBmfOffBck = new mDoExt_bckAnm();
    if (mBmfOffBck == NULL || mBmfOffBck->init(pbck3, 1, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false) == 0) {
        return 0;
    }
    mBmfOffBck->setPlaySpeed(0.0f);

    return 1;
}

int daObjBm_c::create1st() {
    if (mCreateFirstFlag == 0) {
        field_0x100a = home.angle.x;
        field_0x100c = home.angle.z;
        home.angle.x = home.angle.z = 0;
        current.angle.x = current.angle.z = 0;
        shape_angle.x = shape_angle.z = 0;
        mCreateFirstFlag = 1;
    }

    cPhs__Step phase_state = (cPhs__Step)dComIfG_resLoad(&mPhase, l_arcName);

    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state =
            (cPhs__Step)MoveBGCreate(l_arcName, 0x1E, dBgS_MoveBGProc_TypicalRotY, 0x5500, NULL);
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

int daObjBm_c::Execute(Mtx** i_mtx) {
#if DEBUG
    scale.setall(BODY_SCALE);
    mpModel->setBaseScale(scale);
    daPy_py_c* player = daPy_getPlayerActorClass();
#endif

    field_0xff0++;

    if (field_0xf96 != field_0xf98 && field_0xfaa == 0) {
        field_0xfaa = 0xFA;
    }

    cLib_chaseAngleS(&field_0xf96, field_0xf98, field_0xfaa);
    cLib_chaseAngleS(&mBigGearRotX, mBigGearRotXTarget, mGearRotationSpd);
    cLib_chaseAngleS(&mSmallGear0RotX, mSmallGear0RotXTarget, mGearRotationSpd);
    cLib_chaseAngleS(&mSmallGear1RotX, mSmallGear1RotXTarget, mGearRotationSpd);
    cLib_chaseAngleS(&mSmallGear2RotX, mSmallGear2RotXTarget, mGearRotationSpd);

    main_proc_call();

    eyePos = l_eye_offset;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(home.angle.y + field_0xf96);
    mDoMtx_stack_c::multVec(&eyePos, &eyePos);
    attention_info.position = eyePos;

    *i_mtx = &mBgMtx;
    setBaseMtx();

    cLib_addCalcAngleS(&field_0xfaa, field_0xfac, 0x1E, 0x14, 10);
    cLib_addCalcAngleS(&mGearRotationSpd, mGearRotSpdTarget, 0x1E, 0x14, 10);
    cLib_addCalc(&field_0x1000, 0.0f, 0.2f, KREG_F(2) + 50.0f, KREG_F(3) + 10.0f);
    field_0xfb2 = field_0xf96;

    if (mBmfOffBck->getFrame() == 0.0f) {
        setCrawCO();
    }

    mBeamSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mBurnSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    return 1;
}

void daObjBm_c::main_proc_call() {
    static daObjBm_c::procFunc l_func[] = {
        &daObjBm_c::actionEnBemos,
        &daObjBm_c::actionObjBemos,
    };

    (this->*l_func[mActionTypeIdx])();
}

void daObjBm_c::initActionEnBemos() {
    if (mpBgW && mpBgW->ChkUsed()) {
        mpBgW->OffPushPullOk();
    }

    mActionTypeIdx = 0;
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

    mActionTypeIdx = 1;
}

void daObjBm_c::actionEnBemos() {
    static daObjBm_c::procFunc l_func[] = {
        &daObjBm_c::actionSwWait, &daObjBm_c::actionWarning, &daObjBm_c::actionFindPlayer,
        &daObjBm_c::actionAttack, &daObjBm_c::actionDead,
    };

    if (mSph.ChkTgHit()) {
        cCcD_Obj* hitobj = mSph.GetTgHitObj();

        if (hitobj) {
            if (hitobj->ChkAtType(AT_TYPE_ARROW) || hitobj->ChkAtType(AT_TYPE_BOMB) || hitobj->ChkAtType(AT_TYPE_IRON_BALL)) {
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
    (this->*l_func[mActionIdx])();
    calcBeamLenAndAt();
    mBeamEffBck->play();
    mBeamBtk->play();
    mBeamEffBtk->play();
    mSerchBrk->play();
    mBeamosBck->play();
    mBmfOffBck->play();

    if (field_0xfaa != 0) {
        fopAcM_seStartLevel(this, Z2SE_EN_BM_TURN, 0);
    }

    if (mActionIdx != ACTION_DEAD_e && mActionIdx != ACTION_SW_WAIT_e) {
        mSph.SetC(eyePos);
        dComIfG_Ccsp()->Set(&mSph);
    }

    if (mActionIdx == ACTION_ATTACK_e) {
        dComIfG_Ccsp()->Set(&mCps);
        mBeamSound.startLevelSound(Z2SE_EN_BM_BEAM, 0, -1);
    }
}

void daObjBm_c::actionObjBemos() {
    static const daObjBm_c::modeFunc mode_proc[] = {
        &daObjBm_c::mode_wait,
        &daObjBm_c::mode_walk,
        &daObjBm_c::mode_afl,
        &daObjBm_c::mode_dead,
    };

    (this->*mode_proc[mModeProcIdx])();
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
    cXyz offset = l_low_beam_offset;

    if (dist > BEAM_MIDDLE_DIST) {
        offset = l_high_beam_offset;
    } else if (dist > BEAM_LOWER_DIST) {
        offset = l_mid_beam_offset;
    }

    s16 sVar2 = field_0xf96;
    cLib_chasePos(&field_0xfd8, offset, 5.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(sVar2 + current.angle.y);
    mDoMtx_stack_c::multVec(&field_0xfd8, &field_0xfb8);

    dist = mPlayerDist;
    if (dist < 300.0f)
        dist = 300.0f;

    cXyz work(0.0f, 0.0f, dist);
    mDoMtx_stack_c::multVec(&work, &field_0xfcc);
    field_0xfcc = player->current.pos;
    cXyz cStack_48 = field_0xfb8 - field_0xfcc;
    field_0xfc4.x = cM_atan2s(cStack_48.y, field_0xfcc.absXZ(field_0xfb8));
    field_0xfc4.y = sVar2 + current.angle.y;
    field_0xfc4.z = 0;

    if (field_0xfc4.x < cM_deg2s(KREG_F(15) + 20.0f)) {
        field_0xfc4.x = cM_deg2s(KREG_F(15) + 20.0f);
    }

    mDoMtx_stack_c::transS(field_0xfb8);
    mDoMtx_stack_c::ZXYrotM(field_0xfc4.x, sVar2 + current.angle.y, 0);
    mDoMtx_stack_c::multVec(&work, &field_0xfcc);
}

void daObjBm_c::calcBeamLenAndAt() {
    cXyz work = field_0xfcc - field_0xfb8;
    cXyz end(field_0xfb8);
    work.normalizeZP();
    end += work * 1200.0f;
    int frame = mBeamBtk->getFrame() + 0.5f;
    f32 fVar1 = 360.0f;

    if (fopAcM_lc_c::lineCheck(&field_0xfb8, &end, this) && fopAcM_lc_c::checkGroundHit()) {
        f32 absVal = field_0xfb8.abs(fopAcM_lc_c::getCross());
        mBeamScale.z = absVal / 1200.0f;
    } else {
        mBeamScale.z = 1.0f;
    }

    end = field_0xfb8 + (work * (mBeamScale.z * 1200.0f));
    mCps.cM3dGCps::Set(field_0xfb8, end, 20.0f);
    mCps.CalcAtVec();

    if (field_0x1039 > 0) {
        cXyz target(field_0xfcc);

        if (fopAcM_lc_c::lineCheck(&field_0xfb8, &end, this) && fopAcM_lc_c::checkGroundHit()) {
            target = fopAcM_lc_c::getCross();
        }

        if (cLib_chasePos(&mBeamSoundPos, target, SOUND_POS_MOVE_SPD) != 0) {
            field_0x1039 = -1;
        }
    } else if (cLib_chasePos(&mBeamSoundPos, end, SOUND_POS_MOVE_SPD) != 0) {
        field_0x1039 = 1;
    }
}

s8 daObjBm_c::checkFindPlayer() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 searchDistance = getSearchDistance();
    s16 search_angle = DETECTION_ANGLE;
    s8 ret = -1;

    field_0xfe8 = 0;

    f32 playerDistanceXZ = fopAcM_searchPlayerDistanceXZ(this);
    f32 posDelta = current.pos.y - player->current.pos.y;
    if (playerDistanceXZ > searchDistance) {
        return ret;
    }

    if (posDelta < 0.0f) {
        if (std::fabs(posDelta) > (1.0f - (playerDistanceXZ / searchDistance)) * l_eye_offset.y) {
            return ret;
        }
    }

    s16 angle = (fopAcM_searchPlayerAngleY(this) - field_0xf96 - home.angle.y);
    angle = abs(angle);
    if ((search_angle / 2) > (s16)angle) {
        ret = 0;
    }

    if (ret == 0) {
        cXyz eye(eyePos);
        cXyz playerEye(player->eyePos);

        if (fopAcM_lc_c::lineCheck(&eye, &playerEye, this)) {
            field_0xfe8 = 1;
        }
    }

    return ret;
}

s8 daObjBm_c::checkSearchPlayer() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 searchDistance = getSearchDistance();
    s16 search_angle = SEARCH_ANGLE;

    s8 ret = -1;

    field_0xfe8 = 0;

    f32 playerDistanceXZ = fopAcM_searchPlayerDistanceXZ(this);
    f32 playerDistanceY = current.pos.y - player->current.pos.y;
    if (playerDistanceXZ > searchDistance) {
        return ret;
    }

    if (playerDistanceY < 0.0f) {
        if (std::fabs(playerDistanceY) > (1.0f - (playerDistanceXZ / searchDistance)) * l_eye_offset.y) {
            return ret;
        }
    }

    s16 angle = (fopAcM_searchPlayerAngleY(this) - field_0xf96 - home.angle.y);
    angle = abs(angle);
    if ((search_angle / 2) > (s16)angle) {
        ret = 0;
    }

    if (ret == 0) {
        cXyz eye(eyePos);
        cXyz playerEye(player->eyePos);

        if (fopAcM_lc_c::lineCheck(&eye, &playerEye, this)) {
            field_0xfe8 = 1;
        }
    }

    return ret;
}

f32 daObjBm_c::getSearchDistance() {
    f32 ret = SEARCH_DIST;

    if (getSearchDist() != 15 && getSearchDist()) {
        ret -= getSearchDist() * 50.0f;
    }

    return ret;
}

f32 daObjBm_c::getBeamSearchDistance() {
    f32 ret = SEARCH_DIST;

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

    (this->*l_eff_func[mEffFuncIdx])();

    for (int i = 0; i < 2; i++) {
        if (mBrokenSmokeEmitter[i] != NULL) {
            mBrokenSmokeEmitter[i]->setGlobalRTMatrix(mpModel->getAnmMtx(getHeadJoint()));

            if (cLib_calcTimer(&mEmitterTimer) == 0) {
                mBrokenSmokeEmitter[i]->becomeInvalidEmitter();
                mBrokenSmokeEmitter[i]->quitImmortalEmitter();
                mBrokenSmokeEmitter[i] = NULL;
            }
        }
    }

    if (mNessenHeatPrtclEmitter != NULL) {
        J3DJoint* joint = mpModel->getModelData()->getJointNodePointer(getHeadJoint());
        J3DTransformInfo* info = &joint->getTransformInfo();
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(home.angle.y);
        mDoMtx_stack_c::transM(info->mTranslate.y, info->mTranslate.x, info->mTranslate.z);
        mDoMtx_stack_c::YrotM(field_0xf96);
        mNessenHeatPrtclEmitter->setGlobalRTMatrix(mDoMtx_stack_c::get());
    }

    if (mGearSparkEmitter != NULL) {
        mDoMtx_stack_c::copy(mpModel->getAnmMtx((u16)getBigGearJoint()));
        mDoMtx_stack_c::multVecZero(&mSparkSePos);
        mDoAud_seStartLevel(Z2SE_EN_BM_SPARK, &mSparkSePos, 0, 0);
    }
}

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

    mBrokenSmokeEmitter[0] = dComIfGp_particle_set(dPa_RM(ID_ZF_S_BM_BROKENSMK_F), &current.pos,
                                           &home.angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
    if (mBrokenSmokeEmitter[0] != NULL) {
        mBrokenSmokeEmitter[0]->becomeImmortalEmitter();
    }

    mEmitterTimer = 200;
    mEffFuncIdx = 1;
}

void daObjBm_c::effectSet0() {}

void daObjBm_c::effectSet1() {}

void daObjBm_c::effectEnd() {}

int daObjBm_c::check_to_walk() {
    int ret = -1;
    bool flag = true;

    if (field_0x10d2) {
        dBgW::PushPullLabel iVar2 = cLib_checkBit<dBgW::PushPullLabel>(mPPLabel, dBgW::PPLABEL_PULL);
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
                const daObjBm_c::BgcSrc_c* bgcSrc = mBgc.M_lin5;
                if (!mBgc.chk_wall_pre(this, bgcSrc, 5, M_dir_base[i])) {
                    ret = i;
                }
            } else {
                flag = false;
            }
        }
    }

    if (flag) {
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
    mModeProcIdx = MODE_WAIT_e;
}

void daObjBm_c::mode_wait() {
    if (field_0x10ce > 0) {
        field_0x10ce--;
    }

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

const daObjBm_c::BgcSrc_c daObjBm_c::Bgc_c::M_lin5[] = {
    {0.0f, 0.0f, 0.0f, 0.0f}, {-0.5f, -0.5f, -1.0f, -1.0f}, {0.5f, -0.5f, 1.0f, -1.0f},
    {0.5f, 0.5f, 1.0f, 1.0f}, {-0.5f, 0.5f, -1.0f, 1.0f},
};

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

void daObjBm_c::mode_walk_init() {
    fopAcM_SetSpeedF(this, 0.0f);
    mModeProcIdx = MODE_WALK_e;
}

void daObjBm_c::mode_walk() {
    static cXyz dir_vec[] = {
        cXyz(0.0f, 0.0f, 1.0f),
        cXyz(1.0f, 0.0f, 0.0f),
        cXyz(0.0f, 0.0f, -1.0f),
        cXyz(-1.0f, 0.0f, 0.0f),
    };

    bool flag = --field_0x10cc <= 0;
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

    if (flag) {
        const daObjBm_c::BgcSrc_c* bgcSrc = mBgc.M_lin5;
        mBgc.chk_wall_pre(this, bgcSrc, 5, M_dir_base[field_0x10bc]);
    }

    current.pos.x = local_58.x;
    current.pos.z = local_58.z;
    fopAcM_seStartLevel(this, Z2SE_EN_BM_SLIDE, 0);

    if (flag) {
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
    mModeProcIdx = MODE_AFL_e;
}

void daObjBm_c::mode_dead_init() {
    mpBgW->OffPushPullOk();
    fopAcM_SetSpeedF(this, 0.0f);
    mModeProcIdx = MODE_DEAD_e;
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

static f32 dummy_rodata_5157() {
    return -G_CM3D_F_INF;
}

dBgS_ObjGndChk daObjBm_c::Bgc_c::M_gnd_work[23];

dBgS_WtrChk daObjBm_c::Bgc_c::M_wrt_work;

dBgS_ObjLinChk daObjBm_c::Bgc_c::M_wall_work[23];

void daObjBm_c::Bgc_c::chk_wall_touch(const fopAc_ac_c*, const daObjBm_c::BgcSrc_c*, short) {
    static dBgS_ObjLinChk touch_work;
    const static float bgc_add = 10.0f;
}

static f32 dummy_rodata_5158() {
    // needed for correct rodata ordering:
    return 200.0f;
}

void daObjBm_c::Bgc_c::wall_pos(fopAc_ac_c const* i_actor, daObjBm_c::BgcSrc_c const* i_bgcSrc, int i_num, s16 i_dir, f32 param_4) {
    const s16 angle = i_actor->home.angle.y + i_dir;

    cXyz sp48;
    cXyz sp54;
    cXyz sp60;
    cXyz sp6C;
    cXyz work;

    field_0x178 = -1;
    field_0x17c = FLT_MAX;

    mDoMtx_stack_c::YrotS(angle);
    mDoMtx_stack_c::XrotM(0x4000);
    mDoMtx_stack_c::multVec(&cXyz::BaseY, &sp6C);
    sp6C *= param_4 + 100.0f;

    for (int i = 0; i < i_num; i++, i_bgcSrc++) {
        mDoMtx_stack_c::XrotS(0x4000);
        work.set(i_bgcSrc->field_0xc, 0.0f, i_bgcSrc->field_0x8);
        mDoMtx_stack_c::multVec(&work, &sp60);

        mDoMtx_stack_c::YrotS(angle);
        mDoMtx_stack_c::transM(sp60);
        mDoMtx_stack_c::scaleM(200.0f, 400.0f, 200.0f);
        mDoMtx_stack_c::transM(0.0f, 0.5f, 0.0f);
        mDoMtx_stack_c::XrotM(0x4000);

        work.set(i_bgcSrc->field_0x4, 0.0f, i_bgcSrc->field_0x0);
        mDoMtx_stack_c::multVec(&work, &sp48);
        sp48 += i_actor->current.pos;
        sp54 = sp48 + sp6C;

        M_wall_work[i].Set(&sp48, &sp54, i_actor);
        M_wall_work[i].SetActorPid(i_actor->base.base.id);

        if (dComIfG_Bgsp().LineCross(&M_wall_work[i])) {
            field_0x64[i] = M_wall_work[i].GetCross();

            f32 absVal = sp48.abs2(field_0x64[i]);
            if (absVal < field_0x17c) {
                field_0x17c = absVal;
                field_0x178 = i;
            }
        } else {
            field_0x64[i] = cXyz::Zero;
        }
    }
}

bool daObjBm_c::Bgc_c::chk_wall_pre(fopAc_ac_c const* i_actor, daObjBm_c::BgcSrc_c const* i_bgcSrc, int i_num, s16 i_dir) {
    wall_pos(i_actor, i_bgcSrc, i_num, i_dir, 76.0f);
    return field_0x178 >= 0;
}

void daObjBm_c::initActionSwWait() {
    field_0xfaa = 0;
    field_0xfac = 0;
    mGearRotationSpd = field_0xfaa;
    mGearRotationSpd = field_0xfac;
    mSerchBrk->setPlaySpeed(0.0f);
    mBeamosBck->setPlaySpeed(0.0f);
    mActionMode = 0;
    mActionIdx = ACTION_SW_WAIT_e;
}

void daObjBm_c::actionSwWait() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (mActionMode) {
    case 0:
        if (fopAcM_isSwitch(this, getSwNo())) {
            mSerchBrk->init(mpModel->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BRK_UP_e),
                           1, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1);
            cXyz i_sePos(home.pos);
            i_sePos.y += 200.0f;
            mDoAud_seStart(Z2SE_EN_BM_BOOT, &i_sePos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            mShowFlag = 1;
            mActionMode = 1;
        }
        break;

    case 1:
        if (mSerchBrk->isStop()) {
            field_0xfe5 = 0;
            mActionMode = 2;
        }
        break;

    case 2:
        if (cLib_calcTimer(&field_0xfe5) == 0) {
            mBeamBtk->init(mBeamModel->getModelData(),
                           (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, dRes_ID_OBJ_BM_BTK_EF_BIMOBEAM_ON_e), 1,
                           J3DFrameCtrl::EMode_NONE, 0.0f, 0, -1);
            mBeamEffBtk->setPlaySpeed(0.0f);
            mSerchBrk->init(mpModel->getModelData(),
                           (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, dRes_ID_OBJ_BM_BRK_TURN_e), 1,
                           J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);
            mBeamosBck->init((J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, dRes_ID_OBJ_BM_BCK_BM_UP_e), 1,
                         J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, true);
            fopAcM_seStart(this, Z2SE_EN_BM_UP, 0);
            mAfterSwitchActivateNoSearchTimer = AFTER_SWITCH_ACTIVATE_NO_SEARCH_TIME;
            initActionWarning();
        }
        break;
    }
}

void daObjBm_c::initActionWarning() {
    if (mGearSparkEmitter) {
        mGearSparkEmitter->becomeInvalidEmitter();
        mGearSparkEmitter->quitImmortalEmitter();
        mGearSparkEmitter = NULL;
    }

    if (mGearRockEmitter) {
        mGearRockEmitter->becomeInvalidEmitter();
        mGearRockEmitter->quitImmortalEmitter();
        mGearRockEmitter = NULL;
    }

    mActionMode = 0;
    mActionIdx = ACTION_WARNING_e;
}

void daObjBm_c::actionWarning() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    field_0xfac = PATROL_ROT_SPD;

    switch (mActionMode) {
    case 0:
        mActionMode = 1;
        break;

    case 1:
        if (cLib_calcTimer(&mAfterSwitchActivateNoSearchTimer) == 0) {
            mFindPlayerFlag = checkFindPlayer();
            if (mFindPlayerFlag != -1 && field_0xfe8 == 0) {
                field_0xfe5 = 0;
                initActionFindPlayer();
            }
        }
        break;
    }

    mGearRotSpdTarget = field_0xfac + 100;

    if (mActionMode != 5) {
        field_0xf98 = field_0xf96 + field_0xfaa;
    }

    mBigGearRotXTarget = mBigGearRotX - mGearRotationSpd;
    mSmallGear0RotXTarget = mSmallGear0RotX + mGearRotationSpd;
    mSmallGear1RotXTarget = mSmallGear1RotX + mGearRotationSpd;
    mSmallGear2RotXTarget = mSmallGear2RotX + mGearRotationSpd;
}

void daObjBm_c::initActionFindPlayer() {
    mSerchBrk->init(mpModel->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, dRes_ID_OBJ_BM_BRK_SERCH_e),
                   1, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1);
    mNessenHeatPrtclEmitter = dComIfGp_particle_set(dPa_RM(ID_ZF_S_BM_NESSENHEAT), &current.pos, 0, 0, 0xFF, 0,
                                         -1, 0, 0, 0);
    field_0x1038 = -1;
    fopAcM_seStart(this, Z2SE_EN_BM_FIND, 0);
    fopAcM_seStart(this, Z2SE_EN_BM_HEAT, 0);
    field_0xfe5 = 0;
    mActionMode = 0;
    field_0xfac = SEARCH_SPD_ON_DETECT;
    field_0xfaa = field_0xfac;
    mGearRotSpdTarget = field_0xfac;
    mActionIdx = ACTION_FIND_PLAYER_e;
}

void daObjBm_c::actionFindPlayer() {
    switch (mActionMode) {
    case 0: {
        field_0xf98 = fopAcM_searchPlayerAngleY(this) - home.angle.y;
        mBigGearRotXTarget = mBigGearRotX - mGearRotationSpd;
        mSmallGear0RotXTarget = mSmallGear0RotX + mGearRotationSpd;
        mSmallGear1RotXTarget = mSmallGear1RotX + mGearRotationSpd;
        mSmallGear2RotXTarget = mSmallGear2RotX + mGearRotationSpd;

        s16 angle = (fopAcM_searchPlayerAngleY(this) - field_0xf96 - home.angle.y);
        angle = abs(angle);
        if (angle < KREG_S(4) + 2000) {
            mActionMode = 1;
        }
        break;
    }

    case 1:
        field_0xf98 = field_0xf96;
        field_0xfaa = field_0xfac = 0;
        field_0x1000 = KREG_F(10) + 300.0f;
        mActionMode = 2;
        break;

    case 2:
        field_0xf98 = field_0xf96;
        field_0xfaa = field_0xfac = 0;

        if (field_0x1000 == 0.0f) {
            field_0xfe5 = 0;
            initActionAttack();
            return;
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

    #if DEBUG
    mPlayerDist = fopAcM_searchPlayerDistanceXZ(this) - TARGET_OFFSET_DIST;
    #else
    mPlayerDist = fopAcM_searchPlayerDistanceXZ(this);
    #endif
    daPy_py_c* player = daPy_getPlayerActorClass();

    mBeamBtk->init(mBeamModel->getModelData(),
                   (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BTK_EF_BIMOBEAM_ON_e),
                   1, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1);
    mBeamEffBtk->setPlaySpeed(1.0f);

    field_0xfe5 = 20;
    mSearchAttackTimer = NO_SEARCH_ATTACK_TIME;

    if (mGearSparkEmitter == NULL)
        mGearSparkEmitter = dComIfGp_particle_set(dPa_RM(ID_ZF_S_BM_GEARSPARK), &current.pos, NULL, NULL,
                                             0xFF, NULL, -1, NULL, NULL, NULL);

    if (mGearRockEmitter == NULL)
        mGearRockEmitter = dComIfGp_particle_set(dPa_RM(ID_ZF_S_BM_GEARROCK), &current.pos, NULL, NULL,
                                             0xFF, NULL, -1, NULL, NULL, NULL);

    if (mGearSparkEmitter != NULL) {
        mGearSparkEmitter->setRateStep(9);
        mGearSparkEmitter->setLifeTime(6);
        mGearSparkEmitter->setRate(6.5f);
    }

    if (mGearRockEmitter != NULL) {
        mGearRockEmitter->setRateStep(2);
        mGearRockEmitter->setAwayFromCenterSpeed(32.0f);
    }

    for (int i = 0; i < 2; i++) {
        csXyz rotation(0, 0, 0);
        rotation.y = home.angle.y + field_0xf96;
        mNessenSrcPrtclEmitter[i] = dComIfGp_particle_set(l_eff_id[i], &field_0xfb8, &rotation, NULL, 0xFF,
                                                NULL, -1, NULL, NULL, NULL);
    }

    field_0xfed = 0;
    mActionMode = 0;
    mActionIdx = ACTION_ATTACK_e;
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

    s8 sVar3 = 0;
    f32 playerDistanceXZ = fopAcM_searchPlayerDistanceXZ(this);
    if (playerDistanceXZ > getBeamSearchDistance()) {
        playerDistanceXZ = getBeamSearchDistance();
    }
    s16 sVar1 = 0x7530;
    s16 sVar2 = 0x1C2;
    bool flag = true;
    s8 searchPlayer = checkSearchPlayer();
    if (searchPlayer == -1 && cLib_calcTimer(&field_0xfe5) == 0 || field_0xfe8 != 0) {
        flag = false;
    }

    switch (mActionMode) {
    case 0:
        if (cLib_calcTimer(&mSearchAttackTimer) == 0) {
            mActionMode = 2;
        }
        break;
        
    case 1:
        field_0xfac = (ATTACK_ROT_SPD - 250) - KREG_S(0);
        field_0xf98 = fopAcM_searchPlayerAngleY(this) - home.angle.y;
        cLib_chaseF(&mPlayerDist, playerDistanceXZ, KREG_F(16) + 5.0f);

        if (!flag) {
            mActionMode = 3;
        }

        if (cLib_calcTimer(&field_0xfec) == 0) {
            mActionMode = 2;
        }
        break;

    case 2:
        field_0xfac = ATTACK_ROT_SPD;
        field_0xf98 = fopAcM_searchPlayerAngleY(this) - home.angle.y;
        cLib_chaseF(&mPlayerDist, playerDistanceXZ, KREG_F(16) + 15.0f);

        if (!flag) {
            mActionMode = 3;
        }
        break;

    case 3:
        mBeamBtk->init(mBeamModel->getModelData(),
                       (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BTK_EF_BIMOBEAMB_OFF_e),
                       1, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1);
        mSerchBrk->init(mpModel->getModelData(),
                       (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BRK_TURN_e),
                       1, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);
        field_0x1038 = 20;

        if (mNessenHeatPrtclEmitter != NULL) {
            mNessenHeatPrtclEmitter->becomeInvalidEmitter();
            mNessenHeatPrtclEmitter->quitImmortalEmitter();
            mNessenHeatPrtclEmitter = NULL;
        }

        for (int i = 0; i < 2; i++) {
            if (mNessenSrcPrtclEmitter[i] != NULL) {
                mNessenSrcPrtclEmitter[i]->becomeInvalidEmitter();
                mNessenSrcPrtclEmitter[i]->quitImmortalEmitter();
                mNessenSrcPrtclEmitter[i] = NULL;
            }
        }

        mActionMode = 4;
        break;

    case 4:
        if (mBeamBtk->isStop()) {
            for (int i = 0; i < 2; i++) {
                if (mNessenBurnPrtclEmitter[i] != NULL) {
                    mNessenBurnPrtclEmitter[i]->becomeInvalidEmitter();
                    mNessenBurnPrtclEmitter[i]->quitImmortalEmitter();
                    mNessenBurnPrtclEmitter[i] = NULL;
                }
            }

            for (int i = 0; i < 3; i++) {
                if (mNessenSplashPrtclEmitter[i] != NULL) {
                    mNessenSplashPrtclEmitter[i]->becomeInvalidEmitter();
                    mNessenSplashPrtclEmitter[i]->quitImmortalEmitter();
                    mNessenSplashPrtclEmitter[i] = NULL;
                }
            }

            initActionWarning();
        }
    }

    for (int i = 0; i < 2; i++) {
        csXyz rotation(0, 0, 0);
        rotation.y = home.angle.y + field_0xf96;
        mDoMtx_stack_c::transS(field_0xfb8);
        mDoMtx_stack_c::YrotM(rotation.y);

        if (mNessenSrcPrtclEmitter[i] != 0) {
            mNessenSrcPrtclEmitter[i]->setGlobalRTMatrix(mDoMtx_stack_c::get());
        }
    }

    mBurnSoundPos = home.pos;
    cXyz cStack60 = field_0xfcc - field_0xfb8;
    cXyz end(field_0xfb8);
    cStack60.normalizeZP();
    end += cStack60 * 1200.0f;

    if (fopAcM_lc_c::lineCheck(&field_0xfb8, &end, this) && fopAcM_lc_c::checkGroundHit()) {
        if (field_0xfed == 0) {
            for (int i = 0; i < 2; i++) {
                if (mNessenBurnPrtclEmitter[i] == NULL) {
                    mNessenBurnPrtclEmitter[i] =
                        dComIfGp_particle_set(l_eff_id[i], fopAcM_lc_c::getCrossP(), NULL, NULL,
                                              0xFF, NULL, -1, NULL, NULL, NULL);
                }
            }

            for (int i = 0; i < 3; i++) {
                if (mNessenSplashPrtclEmitter[i] == NULL) {
                    mNessenSplashPrtclEmitter[i] =
                        dComIfGp_particle_set(l_eff_id2[i], fopAcM_lc_c::getCrossP(), NULL, NULL,
                                              0xFF, NULL, -1, NULL, NULL, NULL);
                }
            }

            field_0xfed = 1;
        } else {
            f32 y, z;
            z = fopAcM_lc_c::getCrossP()->z;
            y = fopAcM_lc_c::getCrossP()->y + 2.0f;
            mDoMtx_stack_c::transS(fopAcM_lc_c::getCrossP()->x, y, z);
            cM3dGPla plane;
            csXyz angle;

            if (fopAcM_lc_c::checkGroundHit() && fopAcM_lc_c::getTriPla(&plane)) {
                cM3d_CalcVecAngle(plane.mNormal, &angle.x, &angle.z);
                mDoMtx_stack_c::XYZrotM(angle.x, 0, angle.z);
            }

            for (int i = 0; i < 2; i++) {
                if (mNessenBurnPrtclEmitter[i] != NULL) {
                    mNessenBurnPrtclEmitter[i]->setGlobalRTMatrix(mDoMtx_stack_c::get());
                }
            }

            for (int i = 0; i < 3; i++) {
                if (mNessenSplashPrtclEmitter[i] != NULL) {
                    mNessenSplashPrtclEmitter[i]->setGlobalRTMatrix(mDoMtx_stack_c::get());
                }
            }

            mBurnSoundPos = fopAcM_lc_c::getCross();
            mBurnSoundPos.y += 2.0f;
        }
    }

    if (mBurnSoundPos != home.pos) {
        mBurnSound.startLevelSound(Z2SE_EN_BM_BURN, 0, -1);
    }

    if (mSerchBrk->isStop()) {
        mSerchBrk->init(mpModel->getModelData(),
                       (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, dRes_ID_OBJ_BM_BRK_LOOK_e), 1,
                       J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);
    }

    mGearRotSpdTarget = field_0xfac * 3;
    mBigGearRotXTarget = mBigGearRotX - mGearRotationSpd;
    mSmallGear0RotXTarget = mSmallGear0RotX + mGearRotationSpd;
    mSmallGear1RotXTarget = mSmallGear1RotX + mGearRotationSpd;
    mSmallGear2RotXTarget = mSmallGear2RotX + mGearRotationSpd;

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

    field_0xfac = field_0xfaa = 0;

    mBeamosBck->init((J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, dRes_ID_OBJ_BM_BCK_OC_DOWN_e), 1,
                 J3DFrameCtrl::EMode_NONE, 0.0f, 0, -1, true);
    if (mBeamBtk->getBtkAnm() == (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, dRes_ID_OBJ_BM_BTK_EF_BIMOBEAM_ON_e)) {
        mBeamBtk->init(mBeamModel->getModelData(),
                       (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, dRes_INDEX_OBJ_BM_BTK_EF_BIMOBEAMB_OFF_e),
                       1, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1);
    }

    field_0x1038 = 0x14;

    for (int i = 0; i < 2; i++) {
        if (mNessenSrcPrtclEmitter[i] != NULL) {
            mNessenSrcPrtclEmitter[i]->becomeInvalidEmitter();
            mNessenSrcPrtclEmitter[i]->quitImmortalEmitter();
            mNessenSrcPrtclEmitter[i] = NULL;
        }

        if (mNessenBurnPrtclEmitter[i] != NULL) {
            mNessenBurnPrtclEmitter[i]->becomeInvalidEmitter();
            mNessenBurnPrtclEmitter[i]->quitImmortalEmitter();
            mNessenBurnPrtclEmitter[i] = NULL;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (mNessenSplashPrtclEmitter[i] != NULL) {
            mNessenSplashPrtclEmitter[i]->becomeInvalidEmitter();
            mNessenSplashPrtclEmitter[i]->quitImmortalEmitter();
            mNessenSplashPrtclEmitter[i] = NULL;
        }
    }

    if (mNessenHeatPrtclEmitter != NULL) {
        mNessenHeatPrtclEmitter->becomeInvalidEmitter();
        mNessenHeatPrtclEmitter->quitImmortalEmitter();
        mNessenHeatPrtclEmitter = NULL;
    }

    field_0xfe5 = 20;
    field_0xfe9 = 0;
    mShowFlag = 0;
    fopAcM_OffStatus(this, 0);
    mActionMode = 0;
    mActionIdx = ACTION_DEAD_e;
}

void daObjBm_c::actionDead() {
    switch (mActionMode) {
    case 0:
        if (!cLib_calcTimer(&field_0xfe5)) {
            mActionMode = 1;
        }
        break;

    case 1:
        field_0xfac = 500;
        if (mGearSparkEmitter != NULL) {
            mGearSparkEmitter->becomeInvalidEmitter();
            mGearSparkEmitter->quitImmortalEmitter();
            mGearSparkEmitter = NULL;
        }

        if (mGearRockEmitter != NULL) {
            mGearRockEmitter->becomeInvalidEmitter();
            mGearRockEmitter->quitImmortalEmitter();
            mGearRockEmitter = NULL;
        }

        mActionMode = 2;
        break;

    case 2:
        cLib_addCalcAngleS(&field_0xfac, 0, 0x5A, 10, 5);
        if (field_0xf96 == field_0xf98) {
            field_0xfac = 0;
            field_0x1000 = 200.0f;
            mActionMode = 3;
            fopAcM_seStart(this, Z2SE_EN_BM_TURN_END, 0);
        }
        break;

    case 3:
        field_0xfe5 = 20;
        mActionMode = 4;
        break;

    case 4:
        if (!cLib_calcTimer(&field_0xfe5)) {
            mBeamosBck->setPlaySpeed(1.0f);
            dComIfGp_particle_set(dPa_RM(ID_ZF_S_BM_SMOKE), &current.pos, NULL, NULL, 0xFF, NULL,
                                  -1, NULL, NULL, NULL);
            mActionMode = 5;
        }
        break;

    case 5:
        if (mBeamosBck->isStop()) {
            field_0xfe5 = 10;
            fopAcM_seStart(this, Z2SE_EN_BM_DOWN, 0);
            mActionMode = 6;
            fopAcM_seStart(this, Z2SE_EN_BM_UNLOCK, 0);
        }
        break;

    case 6:
        if (getMoveType() == 1) {
            initActionObjBemos();
        } else if (cLib_calcTimer(&field_0xfe5) == 0) {
            mBmfOffBck->setPlaySpeed(1.0f);

            if (mBmfOffBck->isStop()) {
                mActionMode = 7;
            }
        }

        break;
    case 7:
        initActionObjBemos();
    }

    mGearRotSpdTarget = field_0xfac;
    mBigGearRotXTarget = mBigGearRotX - mGearRotationSpd;
    mSmallGear0RotXTarget = mSmallGear0RotX + mGearRotationSpd;
    mSmallGear1RotXTarget = mSmallGear1RotX + mGearRotationSpd;
    mSmallGear2RotXTarget = mSmallGear2RotX + mGearRotationSpd;
}

int daObjBm_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();

    J3DShape* shape = mpMaterial->getShape();
    if (mShowFlag == 0) {
        shape->hide();
    }

    mSerchBrk->entry(mpModel->getModelData());
    mBeamosBck->entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    mDoExt_brkAnmRemove(mpModel->getModelData());
    mDoExt_bckAnmRemove(mpModel->getModelData());
    shape->show();
    dComIfGd_setList();
    g_env_light.setLightTevColorType_MAJI(mBeamModel, &tevStr);

    mBeamEffBtk->entry(mBeamModel->getModelData());
    mBeamBtk->entry(mBeamModel->getModelData());
    mBeamEffBck->entry(mBeamModel->getModelData());
    mDoExt_modelUpdateDL(mBeamModel);
    mDoExt_btkAnmRemove(mBeamModel->getModelData());
    mDoExt_bckAnmRemove(mBeamModel->getModelData());

    g_env_light.setLightTevColorType_MAJI(mBmfModel, &tevStr);
    mBmfOffBck->entry(mBmfModel->getModelData());
    mDoExt_modelUpdateDL(mBmfModel);

#if DEBUG
    debugDraw();
#endif

    return 1;
}

#if DEBUG
void daObjBm_c::debugDraw() {
    if (DEBUG_DRAW) {
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
        dDbVw_drawCircleXlu(cStack_3c, BEAM_MIDDLE_DIST, local_4c, 1, 0xc);
        dDbVw_drawCircleXlu(cStack_3c, BEAM_LOWER_DIST, local_4c, 1, 0xc);
        local_4c = (GXColor){0, 0, 0xFF, 0xFF};
        cXyz cStack_48;

        cStack_48.set(0.0f, 10.0f, getSearchDistance());

        s16 sVar1 = SEARCH_ANGLE / 2;
        mDoMtx_stack_c::YrotS((home.angle.y + field_0xf96) + sVar1);
        mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
        cStack_48 += cStack_3c;
        dDbVw_drawLineOpa(cStack_3c, cStack_48, local_4c, 1, 0xc);
        cStack_48.set(0.0f, 10.0f, getSearchDistance());

        mDoMtx_stack_c::YrotS((home.angle.y + field_0xf96) - sVar1);
        mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
        cStack_48 += cStack_3c;
        dDbVw_drawLineOpa(cStack_3c, cStack_48, local_4c, 1, 0xc);
        local_4c = (GXColor){0, 0xFF, 0xFF, 0xFF};
        cStack_48.set(0.0f, 10.0f, getSearchDistance());

        sVar1 = DETECTION_ANGLE / 2;
        mDoMtx_stack_c::YrotS((home.angle.y + field_0xf96) + sVar1);
        mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
        cStack_48 += cStack_3c;
        dDbVw_drawLineOpa(cStack_3c, cStack_48, local_4c, 1, 0xc);

        cStack_48.set(0.0f, 10.0f, getSearchDistance());
        mDoMtx_stack_c::YrotS((home.angle.y + field_0xf96) - sVar1);
        mDoMtx_stack_c::multVec(&cStack_48, &cStack_48);
        cStack_48 += cStack_3c;
        dDbVw_drawLineOpa(cStack_3c, cStack_48, local_4c, 1, 0xC);
        dDbVw_drawSphereOpa(field_0xfcc, 30.0f, (GXColor){0x0, 0x0, 0xFF, 0xFF}, 1);
    }

    if (CHECK_DRAW) {
        mpBgW->DebugDraw();
    }
}
#endif

int daObjBm_c::Delete() {
    for (int i = 0; i < 2; i++) {
        if (mBrokenSmokeEmitter[i]) {
            mBrokenSmokeEmitter[i]->becomeInvalidEmitter();
            mBrokenSmokeEmitter[i]->quitImmortalEmitter();
            mBrokenSmokeEmitter[i] = NULL;
        }
    }

    if (mGearSparkEmitter) {
        mGearSparkEmitter->becomeInvalidEmitter();
        mGearSparkEmitter->quitImmortalEmitter();
        mGearSparkEmitter = NULL;
    }

    if (mGearRockEmitter) {
        mGearRockEmitter->becomeInvalidEmitter();
        mGearRockEmitter->quitImmortalEmitter();
        mGearRockEmitter = NULL;
    }

    if (mNessenHeatPrtclEmitter) {
        mNessenHeatPrtclEmitter->becomeInvalidEmitter();
        mNessenHeatPrtclEmitter->quitImmortalEmitter();
        mNessenHeatPrtclEmitter = NULL;
    }

    for (int i = 0; i < 2; i++) {
        if (mNessenBurnPrtclEmitter[i]) {
            mNessenBurnPrtclEmitter[i]->becomeInvalidEmitter();
            mNessenBurnPrtclEmitter[i]->quitImmortalEmitter();
            mNessenBurnPrtclEmitter[i] = NULL;
        }

        if (mNessenSrcPrtclEmitter[i]) {
            mNessenSrcPrtclEmitter[i]->becomeInvalidEmitter();
            mNessenSrcPrtclEmitter[i]->quitImmortalEmitter();
            mNessenSrcPrtclEmitter[i] = NULL;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (mNessenSplashPrtclEmitter[i]) {
            mNessenSplashPrtclEmitter[i]->becomeInvalidEmitter();
            mNessenSplashPrtclEmitter[i]->quitImmortalEmitter();
            mNessenSplashPrtclEmitter[i] = NULL;
        }
    }

    mpBgW->SetPushPullCallback(NULL);
    mBeamSound.deleteObject();
    mBurnSound.deleteObject();
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

actor_process_profile_definition g_profile_Obj_Bemos = {
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

AUDIO_INSTANCES;
