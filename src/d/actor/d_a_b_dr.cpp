/**
 * @file d_a_b_dr.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_dr.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_menu_window.h"
#include "d/d_s_play.h"
#include "d/d_debug_viewer.h"
#include "d/actor/d_a_npc4.h"
#include "d/d_camera.h"
#include "c/c_damagereaction.h"
#include "f_op/f_op_camera_mng.h"
#include "Z2AudioLib/Z2Instances.h"


class daB_DR_HIO_c : public JORReflexible {
public:
    daB_DR_HIO_c();
    virtual ~daB_DR_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 model_size;
    /* 0x0C */ f32 fall_accel;
    /* 0x10 */ f32 fall_accel_max;
    /* 0x14 */ f32 wind_large_attack_lockon_range;
    /* 0x18 */ f32 unk_0x18;
    /* 0x1C */ f32 wind_small_attack_power;
    /* 0x20 */ f32 wind_attack_power;
    /* 0x24 */ f32 glide_wind_power;
    /* 0x28 */ f32 breath_attack_threshold;
    /* 0x2C */ f32 unk_0x2c;
    /* 0x30 */ f32 breath_move_speed_max;
    /* 0x34 */ f32 breath_move_speed_min;
    /* 0x38 */ s16 tail_hit_chance_time;
    /* 0x3A */ s16 center_wind_time;
    /* 0x3C */ s16 breath_continue_time;
    /* 0x3E */ s16 breath_continue_no_armor_time;
    /* 0x40 */ s16 breath_feint1_time;
    /* 0x42 */ s16 breath_feint2_time;
    /* 0x44 */ s16 breath_feint3_time;
    /* 0x46 */ s16 no_attack_time;
    /* 0x48 */ s16 unk_0x48;
    /* 0x4A */ s16 no_attack_no_armor_time;
    /* 0x4C */ s16 unk_0x4c;
    /* 0x4E */ s16 back_hit_chance_time;
    /* 0x50 */ s16 after_breath_chance_time;
    /* 0x52 */ s16 neck_search_speed;
    /* 0x54 */ s16 neck_search2_speed;
    /* 0x56 */ s16 neck_search_speed_max;
    /* 0x58 */ s16 body_search_speed;
    /* 0x5A */ s16 body_search2_speed;
    /* 0x5C */ s16 body_search_speed_max;
    /* 0x5E */ s16 body_search_feint_speed_max;
    /* 0x60 */ s16 feint_angle;
    /* 0x62 */ s16 unk_0x62;
    /* 0x64 */ u8 display_range;
    /* 0x65 */ u8 breath_feint2_OFF;
};

#define ANM_DR_BOOT_A 0x10
#define ANM_DR_BOOT_A_DEMO 0x11
#define ANM_DR_BOOT_B 0x12
#define ANM_DR_BOOT_C 0x13
#define ANM_DR_BRAKE_TO_HOVER 0x14
#define ANM_DR_BRIDG_DESTROYA 0x15
#define ANM_DR_BRIDG_DESTROYB 0x16
#define ANM_DR_BURU 0x17
#define ANM_DR_CATCH 0x18
#define ANM_DR_CRAZY 0x19
#define ANM_DR_DAMAGE_L 0x1A
#define ANM_DR_DAMAGE_LAST 0x1B
#define ANM_DR_DAMAGE_R 0x1C
#define ANM_DR_DEATH 0x1D
#define ANM_DR_DEMO_DESTROY 0x1E
#define ANM_DR_DOWN_WAIT 0x1F
#define ANM_DR_FALL 0x20
#define ANM_DR_FALL_DEATH 0x21
#define ANM_DR_FALL_DOWN 0x22
#define ANM_DR_FALL_REVIVE 0x23
#define ANM_DR_FALL_SCREAM 0x24
#define ANM_DR_FEINT_A 0x25
#define ANM_DR_FEINT_B 0x26
#define ANM_DR_FEINT_C 0x27
#define ANM_DR_FIREA 0x28
#define ANM_DR_FIREB 0x29
#define ANM_DR_FIREC 0x2A
#define ANM_DR_FIRED 0x2B
#define ANM_DR_FIREE 0x2C
#define ANM_DR_FLAP 0x2D
#define ANM_DR_FLAP_TO_FLY 0x2E
#define ANM_DR_FLY 0x2F
#define ANM_DR_FLY_TO_GLIDE 0x30
#define ANM_DR_GLIDE 0x31
#define ANM_DR_GLIDE_BRAKE 0x32
#define ANM_DR_GLIDE_TO_FLY 0x33
#define ANM_DR_HOVER_TO_FLY 0x34
#define ANM_DR_HOVERING 0x35
#define ANM_DR_HOVERING_B 0x36
#define ANM_DR_IMPACT 0x37
#define ANM_DR_MID_WAKEUP 0x38
#define ANM_DR_OP_SCREAM 0x39
#define ANM_DR_POLE_STAYA 0x3A
#define ANM_DR_POLE_STAYB 0x3B
#define ANM_DR_POLE_STAYC 0x3C
#define ANM_DR_POLE_STAYD 0x3D
#define ANM_DR_RIDE 0x3E
#define ANM_DR_SHAKEA 0x3F
#define ANM_DR_SHAKEB 0x40
#define ANM_DR_TAKE_OFF 0x41
#define ANM_DR_TURN 0x42
#define ANM_DR_WIND_ATTACKA 0x43
#define ANM_DR_WIND_ATTACKATOB 0x44
#define ANM_DR_WIND_ATTACKB 0x45
#define ANM_DR_WIND_ATTACKBTOFLY 0x46

enum daB_DR_Action {
    ACTION_WAIT,
    ACTION_FLY_WAIT,
    ACTION_TAIL_HIT,
    ACTION_WEEK_HIT,
    ACTION_WIND_ATTACK,
    ACTION_BREATH_ATTACK,
    ACTION_BREATH_ATTACK2,
    ACTION_GLIDER_ATTACK,
    ACTION_PILLAR_SEARCH,
    ACTION_PILLAR_WAIT,
    ACTION_DAMAGE_DEMO = 11,
    ACTION_MIDDLE_DEMO,
    ACTION_DEAD,
};

namespace {
static dCcD_SrcCyl cc_dr_week_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x4002, 0x43}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x2, 0x0, 0x0, 0x302}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            40.0f, // mRadius
            0.0f // mHeight
        } // mCyl
    } // mCylAttr
};

static dCcD_SrcSph cc_dr_tail_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x4000, 0x43}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x2, 0x0, 0x0, 0x302}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

static dCcD_SrcSph cc_dr_breath_at_src = {
    {
        {0x0, {{0x100000, 0x4, 0xD}, {0x0, 0x0}, 0x0}}, // mObj
        {dCcD_SE_13, 0x0, 0xD, dCcD_MTRL_FIRE, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

static dCcD_SrcSph cc_dr_wind_at_src = {
    {
        {0x0, {{0x200, 0x0, 0x1D}, {0x0, 0x0}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, dCcD_MTRL_WIND, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

static dCcD_SrcSph cc_dr_foot_at_src = {
    {
        {0x0, {{0x400, 0x4, 0xD}, {0x0, 0x0}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0xD, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

static dCcD_SrcCyl cc_pillar_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x4000, 0x43}, 0x15}}, // mObj
        {dCcD_SE_NONE, 0x0, 0xD, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x1407}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            40.0f, // mRadius
            0.0f // mHeight
        } // mCyl
    } // mCylAttr
};

static dCcD_SrcCyl cc_down_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xD8FBFDFF, 0x43}, 0x15}}, // mObj
        {dCcD_SE_13, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x2, 0x0, 0x0, 0x1407}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            50.0f, // mRadius
            300.0f // mHeight
        } // mCyl
    } // mCylAttr
};

static dCcD_SrcSph cc_downSph_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xD8FBFDFF, 0x43}, 0x15}}, // mObj
        {dCcD_SE_13, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x2, 0x0, 0x0, 0x1407}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};
}

daB_DR_HIO_c::daB_DR_HIO_c() {
    field_0x4 = -1;
    model_size = 0.8f;
    fall_accel = 2.0f;
    fall_accel_max = 50.0f;
    wind_large_attack_lockon_range = 2000.0f;
    wind_small_attack_power = 15.0f;
    wind_attack_power = 25.0f;
    glide_wind_power = 50.0f;
    breath_attack_threshold = 1000.0f;
    breath_move_speed_max = 20.0f;
    breath_move_speed_min = 5.0f;
    unk_0x2c = 10.0f;
    unk_0x18 = 1800.0f;
    tail_hit_chance_time = 180;
    breath_continue_time = 50;
    breath_continue_no_armor_time = 300;
    unk_0x4c = 100;
    breath_feint1_time = 240;
    breath_feint2_time = 200;
    breath_feint3_time = 300;
    no_attack_time = 300;
    no_attack_no_armor_time = 300;
    center_wind_time = 450;
    unk_0x48 = 450;
    back_hit_chance_time = 150;
    after_breath_chance_time = 390;
    neck_search_speed = 450;
    neck_search2_speed = 400;
    neck_search_speed_max = 500;
    body_search_speed = 15;
    body_search2_speed = 1;
    body_search_speed_max = 100;
    body_search_feint_speed_max = 500;
    feint_angle = 180;
    unk_0x62 = 300;

    display_range = false;
    breath_feint2_OFF = true;
}

static u8 hio_set;

static daB_DR_HIO_c l_HIO;

int daB_DR_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    cXyz unused_sp20;
    cXyz unused_sp14;
    csXyz unused_spC;

    int jnt_no = i_joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));

    switch (jnt_no) {
    case 19:
        mDoMtx_stack_c::YrotM(mHeadAngle.y);
        mDoMtx_stack_c::ZrotM(mHeadAngle.x);
        break;
    case 18:
        mDoMtx_stack_c::YrotM(mHeadAngle.y >> 1);
        mDoMtx_stack_c::ZrotM(mHeadAngle.x >> 1);
        break;
    case 17:
        mDoMtx_stack_c::YrotM(mHeadAngle.y >> 1);
        mDoMtx_stack_c::ZrotM(mHeadAngle.x >> 1);
        break;
    }

    if (jnt_no == TREG_S(0)) {
        mDoMtx_stack_c::XrotM(TREG_S(1));
        mDoMtx_stack_c::YrotM(TREG_S(2));
        mDoMtx_stack_c::ZrotM(TREG_S(3));
    }

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

int daB_DR_c::JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model_p = j3dSys.getModel();
        daB_DR_c* dr_p = (daB_DR_c*)model_p->getUserArea();

        if (dr_p != NULL) {
            dr_p->ctrlJoint(i_joint, model_p);
        }
    }

    return 1;
}

int daB_DR_c::draw() {
    if (BREG_S(6) == 0 && (arg0 == 0xA || arg0 == 0xFF || arg0 == 0xFE)) {
        return 1;
    }

    if (arg0 == 0x14 || arg0 == 0x15) {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mpPartModel, &tevStr);

        if (mPartNo < 2) {
            J3DShape* shape_p;

            shape_p = mpPartModel->getModelData()->getMaterialNodePointer((u8)(mPartNo ^ 1))->getShape();
            if (shape_p != NULL) {
                shape_p->hide();
            }

            shape_p = mpPartModel->getModelData()->getMaterialNodePointer(mPartNo)->getShape();
            if (shape_p != NULL) {
                shape_p->show();
            }
        }

        mDoExt_modelUpdateDL(mpPartModel);
        return 1;
    }

#if DEBUG
    if (WREG_S(0) != 0 || l_HIO.display_range) {
        cXyz sp24;
        cXyz sp18;

        GXColor color;
        color.r = 0x82;
        color.g = 0xFA;
        color.b = 0xFA;
        color.a = 0xFA;

        sp24.set(home.pos);
        sp24.y += 40.0f;
        sp24.x = -l_HIO.unk_0x18;

        sp18.set(home.pos);
        sp18.y += 40.0f;
        sp18.x = l_HIO.unk_0x18;

        dDbVw_drawLineXlu(sp24, sp18, color, TRUE, 12);

        if (field_0x7d1 == 2 ||
            mActionMode == ACTION_BREATH_ATTACK ||
            mActionMode == ACTION_BREATH_ATTACK2 ||
            mActionMode == ACTION_FLY_WAIT ||
            mActionMode == ACTION_PILLAR_SEARCH ||
            mActionMode == ACTION_PILLAR_WAIT ||
            mActionMode == ACTION_GLIDER_ATTACK)
        {
            color.r = 0xFF;
            color.g = 0x82;
            color.b = 0x82;
            color.a = 0xFA;

            sp24.set(home.pos);
            if (mActionMode == ACTION_FLY_WAIT || mActionMode == ACTION_PILLAR_WAIT || mActionMode == ACTION_GLIDER_ATTACK) {
                sp24.set(current.pos);
            }

            if (mActionMode == ACTION_BREATH_ATTACK || mActionMode == ACTION_BREATH_ATTACK2) {
                sp24.set(field_0x790);
                field_0x7a8.set(field_0x784);
            }
            dDbVw_drawLineXlu(sp24, field_0x7a8, color, TRUE, 12);

            color.r = 0x9B;
            color.g = 0x82;
            color.b = 0x82;
            color.a = 0xFA;
            dDbVw_drawLineXlu(field_0x7b4, field_0x7a8, color, TRUE, 12);
        } else if (mActionMode == ACTION_WIND_ATTACK) {
            color.r = 0xFF;
            color.g = 0x82;
            color.b = 0xFF;
            color.a = 0xFA;

            sp24.set(current.pos);
            sp24.y = 10.0f + mAcch.GetGroundH();

            dDbVw_drawCircleXlu(sp24, l_HIO.wind_large_attack_lockon_range, color, TRUE, 12);
        }

        color.r = 0xAA;
        color.g = 0xAA;
        color.b = 0xFA;
        color.a = 0xFA;

        sp24.set(home.pos);
        sp24.y = l_HIO.breath_attack_threshold;

        dDbVw_drawLineXlu(home.pos, sp24, color, TRUE, 12);
    }

    mAcch.DrawWall(dComIfG_Bgsp());
#endif

    J3DModel* model_p = mpModelMorf->getModel();

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model_p, &tevStr);

    if (field_0x7d1 != 0) {
        for (int i = 1; i < mBreakPartsNo + 1; i++) {
            static u8 mBreakDt[4] = {0, 3, 4, 2};

            J3DShape* shape_p = mpModelMorf->getModel()->getModelData()->getMaterialNodePointer(mBreakDt[i])->getShape();
            if (shape_p != NULL) {
                shape_p->hide();
            }
        }
    } else {
        for (int i = 1; i < 4; i++) {
            J3DShape* shape_p = mpModelMorf->getModel()->getModelData()->getMaterialNodePointer((u8)i)->getShape();
            if (shape_p != NULL) {
                shape_p->show();
            }
        }
    }

    mpCoreBrk->entry(model_p->getModelData());
    mpModelMorf->entryDL();

    cXyz pos;
    pos.set(current.pos.x, 10.0f + current.pos.y, current.pos.z);
    mShadowKey = dComIfGd_setShadow(mShadowKey, 0, model_p, &pos, 6000.0f + BREG_F(19), 0.0f, current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static int daB_DR_Draw(daB_DR_c* i_this) {
    return i_this->draw();
}

void daB_DR_c::setBck(int i_anm, u8 i_mode, f32 i_morf, f32 i_speed) {
    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_DR", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
    mAnm = i_anm;
}

void daB_DR_c::setActionMode(int i_action, int i_mode) {
    if (mActionMode != i_action) {
        mActionMode = i_action;
        mMoveMode = i_mode;
    }
}

void daB_DR_c::mGlider_WindPosSet() {
    cXyz mae;
    cMtx_YrotS(*calc_mtx, shape_angle.y);

    mae.x = 0.0f;
    mae.y = 0.0f;
    mae.z = JREG_F(2) + -1000.0f;
    MtxPosition(&mae, &mGlider_WindPos);
    mGlider_WindPos += current.pos;
}

void daB_DR_c::mBoot_c_tranceSet(bool param_0) {
    cXyz mae;
    cXyz ato;
    if (!param_0) {
        if (mAnm == ANM_DR_BOOT_C && (int)mpModelMorf->getFrame() <= 45) {
            cMtx_YrotS(*calc_mtx, shape_angle.y);
            mae.x = -500.0f;
            mae.y = 0.0f;
            mae.z = 500.0f;
            MtxPosition(&mae, &ato);

            mBoot_c_trance.x += ato.x / 45.0f;
            mBoot_c_trance.y += ato.y / 45.0f;
            mBoot_c_trance.z += ato.z / 45.0f;
        }
    } else {
        ato.zero();
        cLib_addCalcPos(&mBoot_c_trance, ato, 0.8f, 20.0f, 5.0f);
    }
}

void daB_DR_c::mStatusONOFF(int i_status) {
    switch (i_status) {
    case 0:
        attention_info.distances[fopAc_attn_BATTLE_e] = 0;
        attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
        fopAcM_OffStatus(this, 0);
        fopAcM_OffStatus(this, 0x200000);
        return;
    case 1:
        attention_info.distances[fopAc_attn_BATTLE_e] = 85;
        attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
        fopAcM_OnStatus(this, 0);
        fopAcM_OnStatus(this, 0x200000);
        return;
    case 2:
        attention_info.distances[fopAc_attn_BATTLE_e] = 60;
        fopAcM_OnStatus(this, 0);
        fopAcM_OffStatus(this, 0x200000);
        attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
        return;
    }    
}

void daB_DR_c::setSe() {
    bool play_wing_se = false;

    switch (mAnm) {
    case ANM_DR_FEINT_A:
        if (mpModelMorf->checkFrame(14)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_FEINT_B:
        if (mpModelMorf->checkFrame(3)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_FEINT_C:
        if (mpModelMorf->checkFrame(18)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_RIDE:
        if (mpModelMorf->checkFrame(1)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_HOVERING_B:
        if (mpModelMorf->checkFrame(0)) {
            mSound.startCreatureVoice(Z2SE_EN_DR_V_HOVERING, -1);
        }
        if (mpModelMorf->checkFrame(5) || mpModelMorf->checkFrame(30)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_POLE_STAYD:
        if (mpModelMorf->checkFrame(0)) {
            mSound.startCreatureVoice(Z2SE_EN_DR_V_POLE_STAY, -1);
        }
        if (mpModelMorf->checkFrame(9) || mpModelMorf->checkFrame(76)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_FIREB:
        if (mpModelMorf->checkFrame(18) || mpModelMorf->checkFrame(42)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_FIREA:
        if (mpModelMorf->checkFrame(0)) {
            mSound.startCreatureVoice(Z2SE_EN_DR_V_FIRE_A, -1);
        }
        if (mpModelMorf->checkFrame(12) || mpModelMorf->checkFrame(55) || mpModelMorf->checkFrame(103) || mpModelMorf->checkFrame(143)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_BURU:
        if (mpModelMorf->checkFrame(0)) {
            mSound.startCreatureSound(Z2SE_EN_DR_BREAK_ARMOR, 0, -1);
        }
        if (mpModelMorf->checkFrame(7) || mpModelMorf->checkFrame(40) || mpModelMorf->checkFrame(75) || mpModelMorf->checkFrame(143) || mpModelMorf->checkFrame(177) || mpModelMorf->checkFrame(210) || mpModelMorf->checkFrame(243) || mpModelMorf->checkFrame(270) || mpModelMorf->checkFrame(309)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_DAMAGE_L:
    case ANM_DR_DAMAGE_R:
        if (mpModelMorf->checkFrame(0)) {
            mSound.startCreatureVoice(Z2SE_EN_DR_V_DAMAGE, -1);
        }
        break;
    case ANM_DR_DAMAGE_LAST:
        if (mpModelMorf->checkFrame(0)) {
            mSound.startCreatureVoice(Z2SE_EN_DR_V_DAMAGE_LAST, -1);
        }
        break;
    case ANM_DR_FALL:
        mSound.startCreatureSoundLevel(Z2SE_EN_DR_FALL, 0, -1);
        break;
    case ANM_DR_GLIDE:
    case ANM_DR_GLIDE_BRAKE:
    case ANM_DR_WIND_ATTACKB:
        mSound.startCreatureSoundLevel(Z2SE_EN_DR_GLIDE, 0, -1);
        break;
    case ANM_DR_FLAP:
        if (mpModelMorf->checkFrame(5)) {
            if (field_0x7da == 0) {
                play_wing_se = true;
                field_0x7da = 1;
            }
        } else {
            field_0x7da = 0;
        }
        break;
    case ANM_DR_BOOT_A:
        if (mpModelMorf->checkFrame(0)) {
            mSound.startCreatureVoice(Z2SE_EN_DR_V_BOOT_A, -1);
        }
    case ANM_DR_BRAKE_TO_HOVER:
    case ANM_DR_FIREE:
    case ANM_DR_GLIDE_TO_FLY:
    case ANM_DR_POLE_STAYA:
    case ANM_DR_WIND_ATTACKA:
    case ANM_DR_WIND_ATTACKATOB:
        if (mpModelMorf->checkFrame(5)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_BOOT_A_DEMO:
        if (mpModelMorf->checkFrame(53)) {
            mSound.startCreatureVoice(Z2SE_EN_DR_V_BOOT_A, -1);
        }
        if (mpModelMorf->checkFrame(76) || mpModelMorf->checkFrame(95) || mpModelMorf->checkFrame(115)) {
            mSound.startCreatureVoice(Z2SE_EN_DR_V_BOOT_B, -1);
        }
        if (mpModelMorf->checkFrame(14) || mpModelMorf->checkFrame(55) || mpModelMorf->checkFrame(72) || mpModelMorf->checkFrame(88) || mpModelMorf->checkFrame(107)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_BOOT_B:
        if (mpModelMorf->checkFrame(0)) {
            mSound.startCreatureVoice(Z2SE_EN_DR_V_BOOT_B, -1);
        }
        if (mpModelMorf->checkFrame(1) || mpModelMorf->checkFrame(17)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_CATCH:
        if (mpModelMorf->checkFrame(10)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_FIREC:
        if (mpModelMorf->checkFrame(14)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_FIRED:
        if (mpModelMorf->checkFrame(6) || mpModelMorf->checkFrame(36)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_FLY:
    case ANM_DR_FLY_TO_GLIDE:
        if (mpModelMorf->checkFrame(8)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_HOVERING:
    case ANM_DR_TURN:
        if (mpModelMorf->checkFrame(7)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_POLE_STAYC:
        if (mpModelMorf->checkFrame(9)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_SHAKEA:
        if (mpModelMorf->checkFrame(5) || mpModelMorf->checkFrame(21)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_SHAKEB:
        if (mpModelMorf->checkFrame(5) || mpModelMorf->checkFrame(26)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_TAKE_OFF:
        if (mpModelMorf->checkFrame(31) || mpModelMorf->checkFrame(53)) {
            play_wing_se = true;
        }
        break;
    case ANM_DR_WIND_ATTACKBTOFLY:
        if (mpModelMorf->checkFrame(10)) {
            play_wing_se = true;
        }
        break;
    }

    if (play_wing_se) {
        mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
    }
}

void daB_DR_c::setDeathLightEffect() {
    for (int i = 0; i < 4; i++) {
        static u16 w_eff_id[] = {0x854C, 0x854D, 0x854E, 0x854F};
        dComIfGp_particle_set(w_eff_id[i], &current.pos, &tevStr, NULL, NULL);
    }

    fopAcM_seStart(this, JA_SE_CM_MONS_EXPLODE, 0);
}

void daB_DR_c::chkPartCreate(bool param_0) {    
    static const struct {
        int jnt_no;
        int prm;
        f32 size;
    } BREAK_PARTS_DT[] = {
        {17, 0, 1.0f},
        {17, 1, 1.0f},
        {18, 0, 1.0f},
        {18, 1, 1.0f},
        {19, 0, 1.0f},
        {19, 1, 1.0f},
        {2, 0, 1.0f},
        {2, 1, 1.0f},
        {43, 0, 1.0f},
        {43, 1, 1.0f},
        {44, 0, 0.5f},
        {44, 1, 0.5f},
        {45, 0, 0.5f},
        {45, 1, 0.5f},
        {46, 0, 0.5f},
        {46, 1, 0.5f},
        {6, 4, 1.0f},
        {18, 0, 1.0f},
        {18, 1, 1.0f},
        {19, 0, 1.0f},
        {19, 1, 1.0f},
        {16, 0, 1.0f},
        {16, 1, 1.0f},
        {26, 0, 1.0f},
        {26, 1, 1.0f},
        {43, 0, 1.0f},
        {43, 1, 1.0f},
        {46, 0, 0.5f},
        {46, 1, 0.5f},
        {12, 5, 1.0f},
        {19, 2, 1.0f},
        {18, 0, 1.0f},
        {18, 1, 1.0f},
        {17, 0, 1.0f},
        {17, 1, 1.0f},
        {16, 0, 1.0f},
        {16, 1, 1.0f},
        {16, 0, 1.0f},
        {16, 1, 1.0f},
        {26, 0, 1.0f},
        {26, 1, 1.0f},
        {26, 0, 1.0f},
        {26, 1, 1.0f},
        {26, 0, 1.0f},
        {26, 1, 1.0f},
        {43, 0, 1.0f},
        {43, 1, 1.0f},
        {44, 0, 0.5f},
        {44, 1, 0.5f},
        {45, 0, 0.5f},
        {45, 1, 0.5f},
        {46, 0, 0.5f},
        {46, 1, 0.5f},
        {47, 3, 1.0f},
    };

    cXyz pos;
    cXyz size;
    csXyz rot;

    int part_no = 0;
    int var_r27 = 0;
    int part_max = 0;

    if (param_0 || field_0x7d1 != 2) {
        int break_no = mBreakPartsNo;
        if (param_0) {
            break_no = 3;
        }

        switch (break_no) {
        case 1:
            part_no = field_0x7e0;
            part_max = 17;
            break;
        case 2:
            part_no = field_0x7e0 + 17;
            part_max = 30;
            break;
        case 3:
            part_no = field_0x7e0 + 30;
            part_max = 54;
            break;
        }

        if (part_no >= part_max) {
            return;
        }

        for (; part_no < part_max && var_r27 < 4; var_r27++) {
            mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(BREAK_PARTS_DT[part_no].jnt_no));
            mDoMtx_stack_c::multVecZero(&pos);
            mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &rot);

            int prm = 0x14;
            if (break_no == 3) {
                prm = 0x15;
            }

            prm |= (BREAK_PARTS_DT[part_no].prm << 8);
            size.x = size.y = size.z = BREAK_PARTS_DT[part_no].size;
            fopAcM_create(PROC_B_DR, prm, &pos, fopAcM_GetRoomNo(this), &rot, &size, -1);

            field_0x7e0++;
            part_no++;
        }
    }
}

void daB_DR_c::mHabatakiAnmSet(int param_0) {
    switch (mAnm) {
    case ANM_DR_FLY_TO_GLIDE:
        if (mpModelMorf->isStop()) {
            setBck(ANM_DR_GLIDE, 2, 3.0f, 1.0f);
        }
        break;
    case ANM_DR_GLIDE_TO_FLY:
        if (mpModelMorf->isStop()) {
            setBck(ANM_DR_BRAKE_TO_HOVER, 0, 3.0f, 1.0f);
        }
        break;
    case ANM_DR_GLIDE:
        setBck(ANM_DR_BRAKE_TO_HOVER, 0, 3.0f, 1.0f);
        break;
    case ANM_DR_FLY:
        if ((int)mpModelMorf->getFrame() >= (int)(mpModelMorf->getEndFrame() - 1.0f)) {
            setBck(ANM_DR_BRAKE_TO_HOVER, 0, l_HIO.unk_0x2c, 1.0f);
        }
        break;
    case ANM_DR_FLAP:
        if ((int)mpModelMorf->getFrame() >= (int)(mpModelMorf->getEndFrame() - 1.0f)) {
            if (param_0 != 5 && param_0 != 6) {
                setBck(ANM_DR_FLAP_TO_FLY, 0, 3.0f, 1.0f);
            } else {
                setBck(ANM_DR_HOVERING, 2, 3.0f, 1.0f);
            }
        }
        break;
    case ANM_DR_FLAP_TO_FLY:
        if (mpModelMorf->isStop()) {
            setBck(ANM_DR_FLY, 2, 10.0f, 1.0f);
        }
        break;
    case ANM_DR_BRAKE_TO_HOVER:
        if (!mpModelMorf->isStop()) {
            break;
        }
        if (param_0 == 4) {
            setBck(ANM_DR_FLAP, 2, 3.0f, 1.0f);
            break;
        }
        /* fallthrough */
    default:
        setBck(ANM_DR_HOVERING, 2, 3.0f, 1.0f);
        /* fallthrough */
    case ANM_DR_HOVERING:
        if ((int)mpModelMorf->getFrame() >= (int)(mpModelMorf->getEndFrame() - 1.0f) && param_0 == 4) {
            setBck(ANM_DR_FLAP, 2, 3.0f, 1.0f);
        }
        break;
    }
}

static u8 dummy(u8 param_0) {
    return cLib_calcTimer<u8>(&param_0);
}

void daB_DR_c::mGlider_AniSet(bool param_0) {
    if (param_0 && mTimer[0] != 0 && mAnm == ANM_DR_FLY) {
        mTimer[0] = 0;
    }

    if (cLib_calcTimer<int>(&mTimer[0]) == 0) {
        switch (mAnm) {
        case ANM_DR_WIND_ATTACKB:
            if ((int)mpModelMorf->getFrame() >= (int)(mpModelMorf->getEndFrame() - 1.0f)) {
                setBck(ANM_DR_WIND_ATTACKBTOFLY, 2, 3.0f, 1.0f);
            }
            break;
        case ANM_DR_HOVERING:
        case ANM_DR_WIND_ATTACKBTOFLY:
            if ((int)mpModelMorf->getFrame() >= (int)(mpModelMorf->getEndFrame() - 1.0f)) {
                if (mAnm == ANM_DR_WIND_ATTACKBTOFLY) {
                    setBck(ANM_DR_FLY, 2, 5.0f, 1.0f);
                } else {
                    setBck(ANM_DR_FLY, 2, 20.0f, 1.0f);
                }
            }
            break;
        case ANM_DR_FLY:
            if ((int)mpModelMorf->getFrame() >= (int)(mpModelMorf->getEndFrame() - 1.0f)) {
                setBck(ANM_DR_FLY_TO_GLIDE, 0, 3.0f, 1.0f);
            }
            break;
        case ANM_DR_FLY_TO_GLIDE:
            if (mpModelMorf->isStop()) {
                setBck(ANM_DR_GLIDE, 2, 3.0f, 1.0f);
                mTimer[0] = cM_rndF(50.0f) + 50.0f;
            }
            break;
        case ANM_DR_GLIDE:
            if (!param_0) {
                if ((int)mpModelMorf->getFrame() >= (int)(mpModelMorf->getEndFrame() - 1.0f)) {
                    setBck(ANM_DR_GLIDE_TO_FLY, 0, 3.0f, 1.0f);
                }
            }
            break;
        case ANM_DR_FLAP_TO_FLY:
        case ANM_DR_GLIDE_TO_FLY:
            if (mpModelMorf->isStop()) {
                setBck(ANM_DR_FLY, 2, 3.0f, 1.0f);
                mTimer[0] = cM_rndF(34.0f) + 34.0f;
            }
            break;
        }
    }
}

void daB_DR_c::mGliderAtAniSet() {
    switch (mAnm) {
    case ANM_DR_FLY_TO_GLIDE:
    case ANM_DR_GLIDE_TO_FLY:
        if (mpModelMorf->isStop()) {
            setBck(ANM_DR_WIND_ATTACKATOB, 0, 3.0f, 1.0f);
        }
        break;
    case ANM_DR_GLIDE:
        setBck(ANM_DR_WIND_ATTACKATOB, 0, 3.0f, 1.0f);
        break;
    case ANM_DR_FLY:
        if ((int)mpModelMorf->getFrame() >= (int)(mpModelMorf->getEndFrame() - 1.0f)) {
            setBck(ANM_DR_WIND_ATTACKATOB, 0, 3.0f, 1.0f);
        }
        break;
    case ANM_DR_WIND_ATTACKATOB:
        if (mpModelMorf->isStop()) {
            field_0x7d8 = 0;
            setBck(ANM_DR_WIND_ATTACKB, 2, 3.0f, 1.0f);
        }
        break;
    }
}

void daB_DR_c::tail_hit_check() {
    if (health > 0) {
        daPy_getPlayerActorClass()->onBossRoomWait();

        if (fopAcM_CheckStatus(this, 0x200000) && cLib_calcTimer<u8>(&field_0x7d0) == 0 && mActionMode != ACTION_TAIL_HIT && field_0x7d1 != 2) {
            if (mTailCc.ChkTgHit()) {
                speedF = 0.0f;
                dComIfGs_onZoneSwitch(21, fopAcM_GetRoomNo(this));
                setActionMode(ACTION_TAIL_HIT, 0);
            }

            mColliderStts.Move();
        }
    }
}

void daB_DR_c::week_hit_check() {
    if (mWeekCc.ChkTgSet()) {
        daPy_getPlayerActorClass()->onBossRoomWait();

        if (health > 0 && fopAcM_CheckStatus(this, 0x200000) && cLib_calcTimer<u8>(&field_0x7d0) == 0 && mActionMode != ACTION_WEEK_HIT && field_0x7d1 == 2) {
            if (mWeekCc.ChkTgHit() && mWeekCc.GetTgHitObj()->ChkAtType(AT_TYPE_HOOKSHOT)) {
                speedF = 0.0f;
                Z2GetAudioMgr()->changeBgmStatus(2);
                field_0x560 = health;
                field_0x7e8 = 0;
                setActionMode(ACTION_WEEK_HIT, 0);
            }

            mColliderStts.Move();
        }
    }
}

void daB_DR_c::mTimerClr() {
    for (int i = 0; i < 4; i++) {
        mTimer[i] = 0;
    }
}

void daB_DR_c::mCountClr() {
    for (int i = 0; i < 4; i++) {
        mCount[i] = 0;
    }
}

void daB_DR_c::mAllClr() {
    mTimerClr();
    mCountClr();
}

void daB_DR_c::mHeadAngleSet() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz unused;

    if (field_0x7d6 == 0) {
        s16 target_angle_y = -(cLib_targetAngleY(&field_0x760, &player->current.pos) - shape_angle.y);
        s16 target_angle_x = -cLib_targetAngleX(&field_0x760, &player->current.pos);

        if (mActionMode != ACTION_PILLAR_WAIT) {
            if (target_angle_y > 0x1000) {
                target_angle_y = 0x1000;
            }
            if (target_angle_y < -0x1000) {
                target_angle_y = -0x1000;
            }
        } else {
            if (target_angle_y > 0x2000) {
                target_angle_y = 0x2000;
            }
            if (target_angle_y < -0x2000) {
                target_angle_y = -0x2000;
            }
        }

        if (target_angle_x > 0x1000) {
            target_angle_x = 0x1000;
        }
        if (target_angle_x < -0x1000) {
            target_angle_x = -0x1000;
        }

        cLib_addCalcAngleS2(&mHeadAngle.y, target_angle_y, 8, 0x500);
        cLib_addCalcAngleS2(&mHeadAngle.x, target_angle_x, 8, 0x500);
    } else {
        cLib_addCalcAngleS2(&mHeadAngle.y, 0, 8, 0x500);
        cLib_addCalcAngleS2(&mHeadAngle.x, 0, 8, 0x500);
    }
}

bool daB_DR_c::flapMove(bool param_0) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    f32 target_y = 2000.0f + NREG_F(1);

    if (field_0x7d1 == 1) {
        target_y = 4000.0f + NREG_F(2);
        if (target_y < player->current.pos.y) {
            target_y = 1000.0f + player->current.pos.y;
        }
    }

    if (-G_CM3D_F_INF == mAcch.GetGroundH()) {
        target_y += home.pos.y;
    } else {
        target_y += mAcch.GetGroundH();
    }

    if (fabsf(current.pos.y - target_y) > 300.0f && field_0x724 < 20.0f) {
        field_0x724 = 20.0f;
    }

    cLib_addCalc2(&current.pos.y, target_y, 0.8f, field_0x724);

    if (!param_0) {
        cLib_addCalc2(&field_0x724, 4.0f, 0.8f, 2.0f);
    }

    if (fabsf(current.pos.y - target_y) < 100.0f) {
        return true;
    }

    return false;
}

bool daB_DR_c::revolutionMove() {
    cXyz mae;
    cXyz unused;

    if (NREG_S(0) != 0) {
        OS_REPORT("\n\n");
        OS_REPORT("どこ？ %d\n", mActionMode);
        OS_REPORT("地面＋高さ %f\n", mTargetHeight);
        OS_REPORT("現在高さ %f\n", fabsf(mTargetHeight - current.pos.y));
    }

    arg0 = 1;

    if (fabsf(current.pos.y - mTargetHeight) > 100.0f) {
        cLib_addCalc2(&current.pos.y, mTargetHeight, 0.8f, 50.0f);
        mae = current.pos - home.pos;
        mae.y = 0.0f;
        field_0x750 = 200;
        field_0x74c = (mae.atan2sX_Z() & 0xF000) + 0x1000;
        return false;
    }

    cLib_addCalc0(&speed.y, 0.7f, 5.0f);
    cMtx_YrotS(*calc_mtx, field_0x74c);
    mae.x = 0.0f;
    mae.y = 0.0f;
    mae.z = 1500.0f;
    MtxPosition(&mae, &field_0x7a8);
    field_0x7a8 += home.pos;
    field_0x7a8.y = mTargetHeight;

    mae = field_0x7a8 - current.pos;
    mae.y = 0.0f;

    cLib_addCalcAngleS2(&current.angle.y, (s16)mae.atan2sX_Z(), field_0x750, 0x400);
    cLib_addCalcAngleS2(&field_0x750, 8, 10, 40);
    shape_angle.y = current.angle.y;

    if (abs((s16)((s16)mae.atan2sX_Z() - shape_angle.y)) > 0x1000) {
        return false;
    }

    return true;
}

bool daB_DR_c::mPlayerHighCheck() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    dBgS_GndChk gndchk;
    cXyz chk_pos;

    chk_pos.set(current.pos);
    chk_pos.y += 300.0f;
    gndchk.SetPos(&chk_pos);
    field_0x734 = dComIfG_Bgsp().GroundCross(&gndchk);

    if (-G_CM3D_F_INF == field_0x734 || field_0x734 < 0.0f) {
        field_0x734 = home.pos.y;
    }

    if (field_0x734 + l_HIO.breath_attack_threshold <= player->current.pos.y) {
        return true;
    }

    return false;
}

bool daB_DR_c::mBgFallGroundCheck() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    dBgS_GndChk gndchk;
    cXyz chkpos;
    cXyz sp34;

    chkpos.set(current.pos);
    chkpos.y += 300.0f;
    gndchk.SetPos(&chkpos);
    chkpos.y = dComIfG_Bgsp().GroundCross(&gndchk);
    if (-G_CM3D_F_INF == chkpos.y) {
        chkpos.y = home.pos.y;
    }
    if (chkpos.y < home.pos.y) {
        chkpos.y = home.pos.y;
    }

    if (!mAcch.ChkGroundHit() && !(80.0f + chkpos.y > current.pos.y)) {
        return false;
    }

    speed.y = 0.0f;

    if (current.pos.y < 80.0f + home.pos.y) {
        current.pos.y = 80.0f + home.pos.y;
    }
    if (current.pos.y < 80.0f + chkpos.y) {
        current.pos.y = 80.0f + chkpos.y;
    }

    gravity = 0.0f;

    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(15));
    mDoMtx_stack_c::multVecZero(&sp34);
    sp34.y = field_0x734;

    cXyz size(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    field_0x280c = dComIfGp_particle_setPolyColor(field_0x280c, 0x899F, mAcch.m_gnd, &sp34, &tevStr, &shape_angle, &size, 0, NULL, -1, NULL);
    
    if (mAnm == ANM_DR_BOOT_B || mAnm == ANM_DR_FALL_DOWN) {
        mSound.startCreatureSound(Z2SE_EN_DR_LAND, 0, -1);
    }

    dComIfGp_getVibration().StartShock(VIBMODE_S_POWER7, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
    mStatusONOFF(0);

    if (bitSw != 0xFF) {
        fopAcM_offSwitch(this, bitSw);
    }

    return true;
}

void daB_DR_c::normalHoverMove() {
    cXyz target_pos;
    cXyz target_vec;

    target_pos.set(400.0f * cM_ssin(mCount[3] * field_0x752), 0.0f, 400.0f * cM_scos(mCount[3] * field_0x752));
    target_pos += home.pos;
    target_pos.y = current.pos.y;

    cLib_addCalcPos(&current.pos, target_pos, 0.4f, 5.0f, field_0x744);
    cLib_addCalc2(&field_0x744, 1.0f, 0.3f, 0.1f);

    target_vec = target_pos - current.pos;
    target_vec.y = 0.0f;

    if (target_vec.abs() < 100.0f) {
        field_0x744 = 0.0f;
        field_0x752 = 4096.0f + cM_rndFX(2048.0f);
        mCount[3]++;
    }
}

void daB_DR_c::executeWait() {
    cXyz sp20;
    cXyz sp14;

#if DEBUG
    dDbVw_Report(30, 100, "wait tyuu No Attack Timer:%d ", mTimer[2]);
#endif

    bool temp_r27 = flapMove(0);
    mHabatakiAnmSet(0);

    switch (mMoveMode) {
    case 0:
        mAllClr();
        mTimer[2] = l_HIO.no_attack_time;
        field_0x750 = JREG_S(8) + 200;
        mMoveMode++;
        /* fallthrough */
    case 1:
        sp20.set(home.pos);
        sp20.y = current.pos.y;
        cLib_addCalcPos(&current.pos, sp20, 0.8f, 20.0f, 5.0f);
        sp14 = sp20 - current.pos;

        cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), 10, 0x800);
        cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, field_0x750, 0x400);
        cLib_addCalcAngleS2(&field_0x750, 8, 8, 20);

        if (daPy_getPlayerActorClass()->getDamageWaitTimer() == 0 && temp_r27 && sp14.abs() < 100.0f) {
            if (mBreakPartsNo == 0) {
                mStatusONOFF(1);
            }
            speed.y = 0.0f;
            gravity = 0.0f;
            mMoveMode++;
        }
        break;
    case 2:
        normalHoverMove();
        break;
    }

    if (mBreakPartsNo == 1) {
        if (dComIfGp_getPlayer(0)->current.pos.y < l_HIO.breath_attack_threshold) {
            mStatusONOFF(0);
        } else {
            mStatusONOFF(1);
        }
    }

    cLib_addCalc0(&speedF, 0.8f, 10.0f);
    cLib_addCalc0(&speed.y, 0.8f, 10.0f);

    if (mMoveMode == 2 && (cLib_calcTimer<int>(&mTimer[2]) == 0 || dComIfGp_checkPlayerStatus0(0, 0x100))) {
        if (mPlayerHighCheck()) {
            setActionMode(ACTION_BREATH_ATTACK, 0);
        } else if (field_0x7d1 != 0) {
            setActionMode(ACTION_GLIDER_ATTACK, 0);
        } else {
            setActionMode(ACTION_WIND_ATTACK, 0);
        }
    }
}

void daB_DR_c::executeFlyWait() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp8;

#if DEBUG
    if (mMoveMode > 1) {
        if (mTimer[2] != 0) {
            dDbVw_Report(30, 100, "wait tyuu No Attack Timer:%d ", mTimer[2]);
        }
    } else {
        dDbVw_Report(30, 100, "JYOU SYOU TYU ");
    }
#endif

    cLib_addCalc0(&speedF, 0.8f, 10.0f);
    mHabatakiAnmSet(0);

    switch (mMoveMode) {
    case 0:
        mWeekCc.OffTgSetBit();
        mStatusONOFF(0);
        mAllClr();

        field_0x724 = 0.0f;
        if (revolutionMove()) {
            field_0x750 = JREG_S(8) + 200;
            mTimer[2] = l_HIO.no_attack_no_armor_time;
            mMoveMode = 2;
        } else {
            mMoveMode = 1;
        }
        break;
    case 1:
        if (!revolutionMove()) {
            break;
        }

        field_0x750 = JREG_S(8) + 200;
        mTimer[2] = l_HIO.no_attack_no_armor_time;
        mpModelMorf->setPlaySpeed(1.0f);
        mMoveMode = 2;
        // fallthrough
    case 2:
        field_0x7d6 = 0;
        normalHoverMove();
        cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), field_0x750, 0x200);

        sp8.set(home.pos);
        sp8.y = current.pos.y;
        cLib_addCalcPos(&current.pos, sp8, 0.8f, 20.0f, 5.0f);

        if (mTargetHeight < player->current.pos.y || mPlayerHighCheck()) {
            setActionMode(ACTION_BREATH_ATTACK, 0);
            return;
        }
        if (cLib_calcTimer<int>(&mTimer[2]) == 0) {
            setActionMode(ACTION_GLIDER_ATTACK, 0);
            return;
        }
    }

    if (mMoveMode < 10) {
        cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, field_0x750, 0x400);
    } else {
        cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, field_0x750, 0x200);
    }

    cLib_addCalcAngleS2(&field_0x750, 8, 10, 20);
}

void daB_DR_c::executeTailHit() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz mae;
    cXyz ato;

#if DEBUG
    if (mTimer[0] != 0) {
        if (mMoveMode < 13) {
            dDbVw_Report(30, 100, "Tail Hit chance Timer:%d ", mTimer[0]);
        } else {
            dDbVw_Report(30, 100, "wait tyuu:%d ", mTimer[0]);
        }
    }
#endif

    bool sp10 = mPlayerHighCheck();
    if (daPy_getPlayerActorClass()->getDamageWaitTimer() != 0) {
        dMw_onPauseWindow();
        if (field_0x7d1 != 2) {
            field_0x724 = 50.0f;
            setActionMode(ACTION_WAIT, 0);
        } else {
            setActionMode(ACTION_FLY_WAIT, 0);
        }
        return;
    }

    if ((mMoveMode == 0 || mMoveMode == 1) && current.pos.y < (1000.0f + l_HIO.breath_attack_threshold)) {
        cLib_addCalc2(&current.pos.y, 1000.0f + l_HIO.breath_attack_threshold, 0.7f, 20.0f);
    }

    switch (mMoveMode) {
    case 0:
        if (mAnm != ANM_DR_HOVERING) {
            setBck(ANM_DR_HOVERING, 2, 10.0f, 1.0f);
        }

        mAllClr();
        dMw_offPauseWindow();
        speedF = 0.0f;

        if (dComIfGp_checkPlayerStatus1(0, 0x10000)) {
            field_0x728 = 0.0f;
            mTimer[0] = l_HIO.tail_hit_chance_time;
            mMoveMode++;
        }
        break;
    case 1:
        if (cLib_calcTimer<int>(&mTimer[0]) != 0) {
            if (dComIfGp_checkPlayerStatus1(0, 0x10000)) {
                if (player->checkBootsOrArmorHeavy()) {
                    field_0x7d6 = 1;
                    if (mBreakPartsNo == 0) {
                        mTimer[0] = 0;
                        setActionMode(ACTION_DAMAGE_DEMO, 0);
                        return;
                    }

                    if (bitSw != 0xFF) {
                        fopAcM_onSwitch(this, bitSw);
                    }

                    if (mAnm != ANM_DR_BOOT_A) {
                        setBck(ANM_DR_BOOT_A, 0, 10.0f, 1.0f);
                    }

                    mTimer[0] = 0;
                    mMoveMode = 10;
                }
            } else {
                mTimer[0] = 0;
            }
        } else {
            if (bitSw != 0xFF) {
                fopAcM_offSwitch(this, bitSw);
            }
    
            setBck(ANM_DR_SHAKEA, 0, 20.0f, 1.0f);
            mMoveMode++;
        }
        break;
    case 2:
        if ((int)mpModelMorf->getFrame() < 28) {
            if (player->checkBootsOrArmorHeavy() && dComIfGp_checkPlayerStatus1(0, 0x10000)) {
                field_0x7d6 = 1;
                if (mBreakPartsNo == 0) {
                    setActionMode(ACTION_DAMAGE_DEMO, 0);
                    return;
                }

                if (bitSw != 0xFF) {
                    fopAcM_onSwitch(this, bitSw);
                }

                field_0x7d7 = 0;
                if (mAnm != ANM_DR_BOOT_A) {
                    setBck(ANM_DR_BOOT_A, 0, 10.0f, 1.0f);
                }

                mMoveMode = 10;
            }
        } else {
            if (mpModelMorf->checkFrame(28)) {
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 0x4F, cXyz(0.0f, 1.0f, 0.0f));
            }

            if (dComIfGp_checkPlayerStatus1(0, 0x10000)) {
                fopAcM_cancelHookCarryNow(this);
            }

            if (mpModelMorf->isStop()) {
                dMw_onPauseWindow();

                if (field_0x7d1 != 2) {
                    if (field_0x7d1 != 0) {
                        setActionMode(ACTION_GLIDER_ATTACK, 0);
                    } else {
                        setActionMode(ACTION_WIND_ATTACK, 0);
                    }
                } else {
                    field_0x750 = JREG_S(8) + 200;
                    setActionMode(ACTION_FLY_WAIT, 0);
                }
            }
        }
        break;
    case 10:
        mStatusONOFF(0);
        dComIfGs_onZoneSwitch(22, fopAcM_GetRoomNo(this));

        if (mBreakPartsNo == 0) {
            dCam_getBody()->SetTrimTypeForce(2);
        }

        if (!mpModelMorf->isStop()) {
            break;
        }

        setBck(ANM_DR_BOOT_B, 2, 3.0f, 1.0f);
        mBgFallGroundCheck();
        mMoveMode++;
    case 11:
        if (mBreakPartsNo == 0) {
            dCam_getBody()->SetTrimTypeForce(2);
        }

        field_0x728 += l_HIO.fall_accel;
        if (field_0x728 >= l_HIO.fall_accel_max) {
            field_0x728 = l_HIO.fall_accel_max;
        }

        cLib_addCalc2(&current.pos.y, field_0x734, 0.8f, field_0x728);
        cLib_addCalc2(&current.pos.x, home.pos.x, 0.8f, 8.0f);
        cLib_addCalc2(&current.pos.z, home.pos.z, 0.8f, 8.0f);

        mAcch.SetWallNone();
        mAcch.SetGroundUpY(75.0f);
        mAcch.CrrPos(dComIfG_Bgsp());

        if (player->getFootOnGround() || player->current.pos.y < 10.0f + field_0x734) {
            mSound.startCreatureSound(Z2SE_EN_DR_LAND, 0, -1);

            if (mBreakPartsNo != 0) {
                Z2GetAudioMgr()->bgmStop(0x1E, 0);
            }

            shape_angle.x = 0;
            mBoot_c_trance.zero();

            if (bitSw != 0xFF) {
                fopAcM_offSwitch(this, bitSw);
            }

            mStatusONOFF(0);

            if (mBreakPartsNo == 0) {
                mTimer[0] = 0;
                setActionMode(ACTION_DAMAGE_DEMO, 10);
                return;
            }

            mBreakPartsNo++;
            mTailCc.OffTgSetBit();
            mMoveMode++;
        }
        break;
    case 12:
        if (startDemoCheck()) {
            shape_angle.y = current.angle.y = home.angle.y;
            current.pos.x = home.pos.x;
            current.pos.z = home.pos.z;

            setBck(ANM_DR_IMPACT, 0, 3.0f, 1.0f);
            mMoveMode++;
        }
        break;
    case 13:
        if (mAnm == ANM_DR_BOOT_C) {
            if ((int)mpModelMorf->getFrame() >= 34) {
                cLib_addCalc2(&mDemoCamCenter.y, player->current.pos.y, 0.7f, 150.0f);
            }

            if (mpModelMorf->isStop()) {
                setBck(ANM_DR_DOWN_WAIT, 2, 3.0f, 1.0f);
                mTimer[0] = 35;
            }
        }

        if (mAnm == ANM_DR_IMPACT && mpModelMorf->isStop()) {
            setBck(ANM_DR_BOOT_C, 0, 3.0f, 1.0f);
            gravity = -1.0f;
        }

        if (!mBgFallGroundCheck()) {
            break;
        }

        dMw_onPauseWindow();
        mMoveMode++;
    case 14:
        if (mAnm == ANM_DR_BOOT_C) {
            if ((int)mpModelMorf->getFrame() >= 34) {
                cLib_addCalc2(&mDemoCamCenter.y, player->current.pos.y, 0.7f, 150.0f);
            }
            if (!mpModelMorf->isStop()) {
                break;
            }
        }

        if (mAnm != ANM_DR_DOWN_WAIT) {
            setBck(ANM_DR_DOWN_WAIT, 2, 3.0f, 1.0f);
            mTimer[0] = 35;
        }
    
        if (cLib_calcTimer<int>(&mTimer[0]) != 0) {
            break;
        }

        mMoveMode++;
    case 15:
        ato = current.pos + mBoot_c_trance;
        parentActorID = fopAcM_createChild(PROC_B_DRE, fopAcM_GetID(this), 1, &ato, fopAcM_GetRoomNo(this), &shape_angle, NULL, -1, NULL);

        Z2GetAudioMgr()->subBgmStart(Z2BGM_DRAGON_D02);
        dComIfGs_onZoneSwitch(23, fopAcM_GetRoomNo(this));
        field_0x7e0 = 0;
        mTimer[0] = BREG_S(3) + 100;
        mMoveMode = 1000;
        break;
    case 20:
        if (mAnm == ANM_DR_DOWN_WAIT && cLib_calcTimer<int>(&mTimer[0]) == 0) {
            dMw_onPauseWindow();
            mTargetHeight = 6000.0f + home.pos.y;
            setBck(ANM_DR_TAKE_OFF, 0, 10.0f, 1.0f);
            field_0x724 = 50.0f;
            mMoveMode++;
        }
        break;
    case 21:
        if (mAnm == ANM_DR_TAKE_OFF) {
            if (mpModelMorf->getFrame() >= 27.0f) {
                field_0x7d7 = 0;
                mBoot_c_tranceSet(1);

                if (field_0x7d1 != 2) {
                    flapMove(1);
                } else {
                    revolutionMove();
                    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 8, 0x400);
                }
            }
        }
        /* fallthrough */
    case 100:
        if (mAnm != ANM_DR_HOVERING) {
            if (!mpModelMorf->isStop()) {
                break;
            }

            dMw_onPauseWindow();
            setBck(ANM_DR_HOVERING, 2, 10.0f, 1.0f);

            if (mPlayerHighCheck() != 0) {
                setActionMode(ACTION_BREATH_ATTACK, 0);
            } else {
                setActionMode(ACTION_GLIDER_ATTACK, 0);
            }
        }

        if (WREG_S(1) != 0) {
            setActionMode(ACTION_WIND_ATTACK, 0);
        }
        break;
    case 1000:
        mMoveMode = 10000;
        /* fallthrough */
    case 10000:
        if (cLib_calcTimer<int>(&mTimer[0]) == 0) {
            arg0 = 0xFE;
        }
        break;
    }

    if (mMoveMode >= 12 && mMoveMode <= 14) {
        if (mAnm != ANM_DR_BOOT_C && mAnm != ANM_DR_DOWN_WAIT) {
            ato.set(home.pos);
            ato.y += 1200.0f + JREG_F(10);
            mDemoCamCenter.set(ato);
        } else {
            ato.set(-1500.0f, 0.0f, -1500.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&ato, -0x8000, 0);
        }

        cMtx_YrotS(*calc_mtx, shape_angle.y - (JREG_S(8) + 0x1000));
        mae.x = 0.0f;
        mae.y = 200.0f + JREG_F(11);
        mae.z = 1000.0f + JREG_F(12);
        MtxPosition(&mae, &ato);
        mDemoCamEye = (ato + current.pos) + mBoot_c_trance;

        daPy_getPlayerActorClass()->onBossRoomWait();
        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);
    }

    if (mMoveMode != 1000 && mAnm == ANM_DR_BOOT_C) {
        if (mpModelMorf->checkFrame(45.0f)) {
            field_0x7e0 = 0;
            mSound.startCreatureSound(Z2SE_EN_DR_CRASH, 0, -1);
        }

        mBoot_c_tranceSet(0);

        if ((int)mpModelMorf->getFrame() >= 45) {
            chkPartCreate(0);
        }
    }

    if (mMoveMode >= 10 && mMoveMode <= 11) {
        cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), 40, 0x200);
        shape_angle.y = current.angle.y;
    }

    if (mMoveMode <= 11) {
        cXyz offset;
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::inverse();
        mDoMtx_stack_c::multVec(&eyePos, &offset);
        daPy_getPlayerActorClass()->setHookshotCarryOffset(fopAcM_GetID(this), &offset);
    }
}

void daB_DR_c::executeWeekHit() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp50;

#if DEBUG
    if (mTimer[2] != 0) {
        dDbVw_Report(30, 100, "week HIT chance Timer:%d ", mTimer[2]);
    }
    if (mTimer[0] != 0) {
        dDbVw_Report(30, 100, "wait tyuu:%d ", mTimer[0]);
    }
#endif

    switch (mMoveMode) {
    case 0:
        setBck(ANM_DR_CATCH, 0, 7.0f, 1.0f);

        if (bitSw2 != 0xFF) {
            fopAcM_onSwitch(this, bitSw2);
        }

        mAllClr();

        mCount[0] = cM_rndF(2.0f);
        mCount[1] = 0;
        mTimer[2] = l_HIO.back_hit_chance_time;
        field_0x7d0 = 0;
        field_0x7d6 = 1;
        mMoveMode++;
        /* fallthrough */
    case 1:
        if (cLib_calcTimer<int>(&mTimer[2]) == 0) {
            setBck(ANM_DR_SHAKEB, 0, 10.0f, 1.0f);
            Z2GetAudioMgr()->changeBgmStatus(1);
            mMoveMode = 100;
            field_0x744 = 0.0f;
            break;
        } else {
            if (mCount[1] == 0 && player->checkDragonHangRide()) {
                setBck(ANM_DR_RIDE, 0, 3.0f, 1.0f);
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER2, 0x4F, cXyz(0.0f, 1.0f, 0.0f));
                mCount[1] = 1;
            }

            if ((mAnm == ANM_DR_RIDE || mAnm == ANM_DR_DAMAGE_L || mAnm == ANM_DR_DAMAGE_R || mAnm == ANM_DR_CATCH) && mpModelMorf->isStop()) {
                setBck(ANM_DR_HOVERING_B, 2, 3.0f, 1.0f);
            }

            if (!checkCutDownHitFlg() || cLib_calcTimer<u8>(&field_0x7d0) != 0) {
                break;
            }

            offCutDownHitFlg();
            field_0x7d0 = 4;

            mSound.startCreatureExtraSound(Z2SE_EN_DR_CORE_HIT, 0, -1);
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 0x1F, cXyz(0.0f, 1.0f, 0.0f));

            if (player->checkNormalSwordEquip()) {
                health--;
                if (player->getSwordAtUpTime()) {
                    health--;
                }
            } else if (player->checkMasterSwordEquip()) {
                health -= 2;
                if (player->getSwordAtUpTime()) {
                    health -= 2;
                }
            }

            field_0x7e8++;
            mTimer[2] = l_HIO.back_hit_chance_time;

            mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(15));
            mDoMtx_stack_c::multVecZero(&sp50);
            dComIfGp_particle_set(0x8999, &sp50, NULL, NULL);
            dComIfGp_particle_set(0x899A, &sp50, NULL, NULL);

            if (health <= 0 || player->getCutType() == 0x20) {
                health = 0;
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x20);

                gravity = -1.0f;
                setBck(ANM_DR_DAMAGE_LAST, 0, 3.0f, 1.0f);
                mTimer[2] = 0;

                if (bitSw2 != 0xFF) {
                    fopAcM_offSwitch(this, bitSw2);
                }
                if (bitSw3 != 0xFF) {
                    fopAcM_onSwitch(this, bitSw3);
                }

                Z2GetAudioMgr()->bgmStop(0x1E, 0);
                Z2GetAudioMgr()->bgmStreamPrepare(0x2000049);
                Z2GetAudioMgr()->bgmStreamPlay();

                cXyz sp44(current.pos);
                sp44.z = 0.0f;
                sp44.x = 0.0f;

                parentActorID = fopAcM_createChild(PROC_B_DRE, fopAcM_GetID(this), 0, &sp44, fopAcM_GetRoomNo(this), &shape_angle, NULL, -1, NULL);
                mTimer[0] = BREG_S(3) + 20;
                mMoveMode = 1000;
                return;
            }

            if (field_0x7e8 >= 4) {
                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x20);

                if (bitSw2 != 0xFF) {
                    fopAcM_offSwitch(this, bitSw2);
                }
                if (bitSw3 != 0xFF) {
                    fopAcM_onSwitch(this, bitSw3);
                }

                if (health <= 8) {
                    onDownFlg();
                }

                gravity = -1.0f;
                setBck(ANM_DR_DAMAGE_LAST, 0, 3.0f, 1.0f);
                mTimer[2] = 0;
                Z2GetAudioMgr()->changeBgmStatus(1);
                dComIfGs_onZoneSwitch(0x19, fopAcM_GetRoomNo(this));
                mMoveMode++;
            } else {
                if (mCount[0] == 0) {
                    setBck(ANM_DR_DAMAGE_L, 0, 3.0f, 1.0f);
                } else {
                    setBck(ANM_DR_DAMAGE_R, 0, 3.0f, 1.0f);
                }

                mCount[0]++;
                mCount[0] &= 1;

                mSound.startCollisionSE(Z2SE_HIT_SWORD, 0x1F);
            }
            break;
        }
    case 2:
        if (mAnm == ANM_DR_DAMAGE_LAST && mpModelMorf->isStop()) {
            setBck(ANM_DR_FALL, 2, 3.0f, 1.0f);
        }

        if (current.pos.y < 870.0f && mAnm != ANM_DR_FALL_DOWN) {
            player->cancelDragonHangBackJump();
            gravity = -13.0f;
            setBck(ANM_DR_FALL_DOWN, 0, 10.0f, 1.0f);
        }

        if (mBgFallGroundCheck()) {
            if (bitSw3 != 0xFF) {
                fopAcM_offSwitch(this, bitSw3);
            }
            if (mAnm != ANM_DR_FALL_DOWN) {
                setBck(ANM_DR_FALL_DOWN, 0, 10.0f, 1.0f);
            }
            mMoveMode++;
        }
        break;
    case 3:
        if (mAnm == ANM_DR_FALL_DOWN && mpModelMorf->isStop()) {
            setBck(ANM_DR_DOWN_WAIT, 2, 10.0f, 1.0f);
            field_0x7d7 = 1;
            mTimer[0] = 35;
        }

        if (mAnm == ANM_DR_DOWN_WAIT && cLib_calcTimer<int>(&mTimer[0]) == 0) {
            setBck(ANM_DR_TAKE_OFF, 0, 10.0f, 1.0f);
            field_0x724 = 50.0f;
            mMoveMode++;
        }
        break;
    case 4:
        if (mAnm == ANM_DR_TAKE_OFF) {
            if (mpModelMorf->getFrame() >= 27.0f) {
                revolutionMove();
                field_0x7d7 = 0;
            }
        }

        if (mAnm != ANM_DR_HOVERING) {
            if (!mpModelMorf->isStop()) {
                break;
            }
            setBck(ANM_DR_HOVERING, 2, 10.0f, 1.0f);
        }

        if (mPlayerHighCheck()) {
            setActionMode(ACTION_BREATH_ATTACK, 0);
        } else {
            setActionMode(ACTION_FLY_WAIT, 0);
        }
        break;
    case 100:
        if (mpModelMorf->checkFrame(30.0f)) {
            player->cancelOctaIealHang();
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 0x4F, cXyz(0.0f, 1.0f, 0.0f));

            if (bitSw2 != 0xFF) {
                fopAcM_offSwitch(this, bitSw2);
            }
            if (bitSw3 != 0xFF) {
                fopAcM_offSwitch(this, bitSw3);
            }
        }

        if (mpModelMorf->isStop()) {
            setBck(ANM_DR_HOVERING, 2, 10.0f, 1.0f);
            setActionMode(ACTION_FLY_WAIT, 0);
        }
        break;
    case 1000:
        if (cLib_calcTimer<int>(&mTimer[0]) == 0) {
            /* City in the Sky - City in the Sky clear */
            dComIfGs_onEventBit(dSv_event_flag_c::F_0268);
            dComIfGs_onStageBossEnemy(0x16);
            fopAcM_onSwitch(this, 0x38);
            fopAcM_delete(this);
        }
        break;
    }

    cXyz offset;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::inverse();
    mDoMtx_stack_c::multVec(&eyePos, &offset);
    daPy_getPlayerActorClass()->setHookshotCarryOffset(fopAcM_GetID(this), &offset);
}

static u8 data_805C7974;

void daB_DR_c::mBreathSet() {
    cXyz mae;
    cXyz ato;

    cLib_addCalc2(&field_0x72c, 50.0f, 0.7f, 4.0f);
    data_805C7974 = field_0x72c;

    csXyz sp1C(shape_angle);
    for (int i = 0; i < 4; i++) {
        static u16 effId[] = {0x899B, 0x899C, 0x899D, 0x899E};
        field_0x27dc[i] = dComIfGp_particle_set(field_0x27dc[i], effId[i], &field_0x760, &sp1C, NULL);
        
        JPABaseEmitter* emitter_p = dComIfGp_particle_getEmitter(field_0x27dc[i]);
        if (emitter_p != NULL) {
            emitter_p->setGlobalSRTMatrix(mpModelMorf->getModel()->getAnmMtx(0x13));

            cMtx_YrotS(*calc_mtx, fopAcM_searchPlayerAngleY(this));
            mae.x = 0.0f;
            mae.y = 0.0f;
            mae.z = 2200.0f;
            MtxPosition(&mae, &ato);
            ato += field_0x790;
            field_0x784.set(ato);

            mSound.startCreatureVoiceLevel(Z2SE_EN_DR_V_FIRE, -1);
            mSound2.startCreatureSoundLevel(Z2SE_EN_DR_V_FIRE, 0, -1);
        }
    }

    if (cLib_calcTimer<int>(&mTimer[1]) == 0) {
        if (mActionMode == ACTION_BREATH_ATTACK2) {
            fopAc_ac_c* player = dComIfGp_getPlayer(0);
            ato = field_0x778 - field_0x760;
            sp1C.y = ato.atan2sX_Z();
        }

        sp1C.x = NREG_S(8) - 1000;
        mTimer[1] = 3;
        fopAcM_createChild(PROC_B_DR, fopAcM_GetID(this), 10, &field_0x760, fopAcM_GetRoomNo(this), &sp1C, NULL, -1, NULL);
    }
}

bool daB_DR_c::mBreathHighSet(bool param_0) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz mae;
    cXyz ato;

    if (param_0 == 0) {
        mHabatakiAnmSet(mActionMode);
        if (mAnm != ANM_DR_HOVERING) {
            return false;
        }
    }

    cMtx_YrotS(*calc_mtx, fopAcM_searchPlayerAngleY(this));
    mae.x = 0.0f;
    mae.y = 1200.0f;
    mae.z = 1200.0f;
    MtxPosition(&mae, &ato);

    if (mActionMode != ACTION_BREATH_ATTACK2) {
        field_0x7a8.x = ato.x + home.pos.x;
        field_0x7a8.y = player->current.pos.y + BREG_F(7);
        field_0x7a8.z = ato.z + home.pos.z;
    } else {
        field_0x7a8.x = home.pos.x;
        if (player->current.pos.y >= 0.0f) {
            field_0x7a8.y = (player->current.pos.y - 200.0f) + BREG_F(7);
        } else {
            field_0x7a8.y = 0.0f;
        }
        field_0x7a8.z = home.pos.z;
    }

    if (param_0 == 0) {
        cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), field_0x750, 0x800);
        cLib_addCalcAngleS2(&field_0x750, 4, 4, 40);
        cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 8, 0x300);
    }

    cLib_addCalcPos(&current.pos, field_0x7a8, 0.8f, l_HIO.breath_move_speed_max, l_HIO.breath_move_speed_min);

    ato = field_0x7a8 - current.pos;
    if (ato.abs() < 100.0f && fabsf(field_0x7a8.y - current.pos.y) < 40.0f && (param_0 || abs((s16)(shape_angle.y - fopAcM_searchPlayerAngleY(this))) < 0x200)) {
        return true;
    }

    return false;
}

bool daB_DR_c::mFeintBreath() {
    s16 temp_r30 = field_0x754;

    switch (field_0x718) {
    case 0:
        mTimer[2] = l_HIO.breath_feint1_time;
        field_0x718++;
        /* fallthrough */
    case 1:
        if (cLib_calcTimer<int>(&mTimer[2]) == 0) {
            setBck(ANM_DR_FEINT_A, 0, 3.0f, 1.0f);
            mTimer[2] = l_HIO.breath_feint2_time;
            field_0x718++;
        } else {
            mBreathSet();

            cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), l_HIO.body_search2_speed, l_HIO.body_search_speed_max);
            field_0x756 = current.angle.y - shape_angle.y;
            cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 4, 0x400);
            field_0x754 = current.angle.y;
        }
        break;
    case 2:
        if (mAnm == ANM_DR_FEINT_A && mpModelMorf->isStop()) {
            setBck(ANM_DR_FEINT_B, 2, 3.0f, 1.0f);
        }

        if (cLib_calcTimer<int>(&mTimer[2]) == 0) {
            field_0x718++;
        } else {
            if (field_0x756 > 0) {
                temp_r30 += (s16)(l_HIO.feint_angle * 0xB6);
            } else {
                temp_r30 -= (s16)(l_HIO.feint_angle * 0xB6);
            }

            if (abs((s16)(current.angle.y - temp_r30)) > 0x100) {
                cLib_addCalcAngleS2(&current.angle.y, temp_r30, 1, l_HIO.body_search_feint_speed_max);
                cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 2, 0x800);
            } else {
                mTimer[2] = 0;
            }
        }
        break;
    case 3:
        if (mAnm == ANM_DR_FEINT_B && (int)mpModelMorf->getFrame() >= 23) {
            setBck(ANM_DR_FEINT_C, 0, 3.0f, 1.0f);
            field_0x718++;
        }
        break;
    case 4:
        if (mpModelMorf->isStop()) {
            setBck(ANM_DR_FIREB, 2, 3.0f, 1.0f);
            mTimer[2] = l_HIO.breath_feint3_time;
            field_0x718++;
        }
        break;
    case 5:
        break;
    }

    if (field_0x718 < 5) {
        return false;
    }

    return true;
}

void daB_DR_c::executeBreathAttack() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp1C;
    cXyz sp10;

    s16 var_r28 = 0;
    s16 spC = 10;
    s16 spA = 0;
    bool sp8 = false;

#if DEBUG
    if (mTimer[3] != 0) {
        if (mMoveMode < 100) {
            dDbVw_Report(30, 100, "Breath tyuu No Attack Timer:%d ", mTimer[3]);
        } else {
            dDbVw_Report(30, 100, "Breath after chance timer:%d ", mTimer[3]);
        }
    }

    if (mTimer[2] != 0) {
        dDbVw_Report(30, 130, "Breath Timer:%d ", mTimer[2]);
    }
#endif

    if ((daPy_getPlayerActorClass()->getDamageWaitTimer() != 0 || !mPlayerHighCheck()) && (mMoveMode == 4 || mMoveMode == 20)) {
        mStatusONOFF(0);
        mAllClr();
        mMoveMode = 10;
        setBck(ANM_DR_FIREC, 0, 3.0f, 1.0f);
        return;
    }

    cLib_addCalc0(&speedF, 0.5f, 2.0f);
    cLib_addCalc0(&speed.y, 0.5f, 2.0f);

    switch (mMoveMode) {
    case 0:
        mAllClr();
        gravity = 0.0f;
        field_0x750 = 200;
        if (dComIfGp_checkPlayerStatus1(0, 0x2000000)) {
            mTimer[3] = l_HIO.unk_0x4c;
        }
        mMoveMode++;
        /* fallthrough */
    case 1:
        mBreathHighSet(0);
        if (cLib_calcTimer<int>(&mTimer[3]) != 0) {
            if (mAnm == ANM_DR_HOVERING && field_0x7d1 != 2) {
                mStatusONOFF(1);
            }   
            break;
        }

        mMoveMode++;
    case 2:
        sp8 = mBreathHighSet(0);
        if (mAnm == ANM_DR_HOVERING) {
            if (field_0x7d1 != 2) {
                mStatusONOFF(1);
            }
            if ((int)mpModelMorf->getFrame() >= 33 && sp8) {
                setBck(ANM_DR_FIREA, 0, 3.0f, 1.0f);
                mMoveMode++;
            }
        }
        break;
    case 3:
        mBreathHighSet(1);
        if (!mpModelMorf->isStop()) {
            break;
        }

        if (field_0x7d1 != 2 || mTargetHeight > player->current.pos.y || dComIfGp_checkPlayerStatus1(0, 0x2000000)) {
            mTimer[2] = l_HIO.breath_continue_time;
        } else {
            mTimer[2] = l_HIO.breath_continue_no_armor_time;
            if (health <= 8) {
                mTimer[2] = l_HIO.breath_feint1_time;
            }
            mActionMode = ACTION_BREATH_ATTACK2;
        }

        if (BREG_S(0) != 0) {
            mTimer[2] = 10;
        }

        field_0x72c = 0.0f;
        setBck(ANM_DR_FIREB, 2, 3.0f, 1.0f);
        mMoveMode++;
        mCount[3] = 0;
        field_0x718 = 0;

        if (l_HIO.breath_feint2_OFF) {
            mMoveMode = 20;
        }
    case 20:
    case 4:
        mBreathHighSet(1);

        if (mActionMode == ACTION_BREATH_ATTACK2) {
            var_r28 = -(cLib_targetAngleY(&field_0x760, &player->current.pos) - shape_angle.y);
            if (var_r28 > 0x2000) {
                var_r28 = 0x2000;
            }
            if (var_r28 < -0x2000) {
                var_r28 = -0x2000;
            }

            if (health <= 0x10) {
                spC = l_HIO.neck_search2_speed;
                spA = l_HIO.body_search2_speed;
            } else {
                spC = l_HIO.neck_search_speed;
                spA = l_HIO.body_search_speed;
            }

            if (mMoveMode == 20 && mActionMode == ACTION_BREATH_ATTACK2 && health <= 8 && !mFeintBreath()) {
                break;
            }

            cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), spA, l_HIO.body_search_speed_max);
            field_0x756 = current.angle.y - shape_angle.y;
            cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 4, 0x400);

            if (mTargetHeight - 300.0f > player->current.pos.y) {
                mTimer[2] = 0;
            }
        }

        if (mAnm == ANM_DR_FIREB) {
            mBreathSet();
        }
    case 9:
        if (cLib_calcTimer<int>(&mTimer[2]) == 0 && (int)mpModelMorf->getFrame() >= 48) {
            setBck(ANM_DR_FIREC, 0, 3.0f, 1.0f);
            mMoveMode = 10;
        }
        break;
    case 10:
        mBreathHighSet(1);

        if (!mpModelMorf->isStop()) {
            cLib_addCalcAngleS2(&mHeadAngle.y, 0, 100, 0x200);
        } else if (mActionMode == ACTION_BREATH_ATTACK2 && field_0x7d1 == 2 && mTargetHeight < player->current.pos.y) {
            setBck(ANM_DR_FIRED, 2, 10.0f, 1.0f);
            mTimer[3] = l_HIO.after_breath_chance_time;
            mWeekCc.OnTgSetBit();
            mStatusONOFF(1);
            mMoveMode = 100;
        } else {
            setBck(ANM_DR_FIREE, 0, 3.0f, 1.0f);
            mMoveMode++;
        }
        break;
    case 11:
        if (!mpModelMorf->isStop()) {
            break;
        }

        setBck(ANM_DR_HOVERING, 2, 3.0f, 1.0f);
        mMoveMode++;
    case 12:
        field_0x724 = 20.0f;
        field_0x7d6 = 1;

        if (!mPlayerHighCheck()) {
            if (field_0x7d1 != 2) {
                field_0x724 = 100.0f;
                if (field_0x7d1 != 0) {
                    setActionMode(ACTION_WAIT, 0);
                } else {
                    setActionMode(ACTION_WIND_ATTACK, 0);
                }
            } else {
                field_0x750 = JREG_S(8) + 200;
                setActionMode(ACTION_FLY_WAIT, 0);
            }
            return;
        }

        if (field_0x7d1 != 2) {
            mStatusONOFF(1);
        }

        if (field_0x7d1 != 2) {
            mTimer[3] = l_HIO.no_attack_time;
        } else {
            mTimer[3] = l_HIO.no_attack_no_armor_time;
        }
        mMoveMode = 1;
        break;
    case 100:
        mBreathHighSet(1);

        if (BREG_S(7) != 0) {
            mTimer[3] = 1000;
        }

        attention_info.flags |= fopAc_AttnFlag_BATTLE_e;

        if (cLib_calcTimer<int>(&mTimer[3]) != 0 && mTargetHeight - 300.0f < player->current.pos.y) {
            if (abs((s16)(fopAcM_searchPlayerAngleY(this) - shape_angle.y)) < ZREG_S(0) + 0x5000) {
                mWeekCc.OffTgSetBit();
                attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
            } else {
                mWeekCc.OnTgSetBit();
                if (mTarget != 0 || (dComIfGp_getAttention()->GetLockonList(0) != NULL && dComIfGp_getAttention()->LockonTruth() && dComIfGp_getAttention()->GetLockonList(0)->getActor() == this)) {
                    mTarget = 0;
                } else {
                    attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
                    mTarget = 0;
                }
            }
        } else {
            mWeekCc.OffTgSetBit();
            mStatusONOFF(0);
            setBck(ANM_DR_FIREE, 0, 3.0f, 1.0f);
            mMoveMode = 11;
        }
        break;
    }

    cLib_addCalcAngleS2(&mHeadAngle.x, 0, 100, 0x200);
    cLib_addCalcAngleS2(&mHeadAngle.y, var_r28, spC, l_HIO.neck_search_speed_max);

    if (mMoveMode < 9 && !mPlayerHighCheck()) {
        if (mAnm == ANM_DR_FIREB) {
            mTimer[2] = 0;
            mMoveMode = 9;
        } else {
            mMoveMode = 12;
        }
    }
}

void daB_DR_c::executeWindAttack() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp30;
    cXyz sp24;

    bool sp8 = 0;

#if DEBUG
    dDbVw_Report(30, 100, "normal Timer:%d center Timer:%d ", mTimer[1], mTimer[2]);
#endif

    if (mAnm != ANM_DR_FLAP) {
        mHabatakiAnmSet(mActionMode);
    }

    cLib_addCalc0(&speedF, 0.5f, 5.0f + WREG_F(7));
    cLib_addCalc0(&speed.y, 0.5f, 5.0f);

    switch (mMoveMode) {
    case 0:
        field_0x7d6 = 0;
        mAllClr();

        field_0x7b4.set(player->current.pos);
        if (field_0x7b4.z < 0.0f) {
            field_0x7b4.z = 3000.0f;
        } else {
            field_0x7b4.z = -3000.0f;
        }

        gravity = 0.0f;
        field_0x750 = 400;
        mMoveMode++;
        /* fallthrough */
    case 1:
        field_0x7a8.set(home.pos);
        cLib_addCalcPos(&current.pos, field_0x7a8, 0.8f, 40.0f, 20.0f);
        sp30 = field_0x7a8 - current.pos;
        sp30.y = 0.0f;

        if (!(sp30.abs() > 1000.0f)) {
            mMoveMode++;
        case 2:
            normalHoverMove();
        }
        break;
    }

    if (mMoveMode == 2 && mPlayerHighCheck() && !daPy_getPlayerActorClass()->getDamageWaitTimer()) {
        setActionMode(ACTION_BREATH_ATTACK, 0);
        return;
    }

    if (dComIfGp_checkPlayerStatus0(0, 0x100)) {
        mCount[1] = 0;
        mTimer[1] = 0;
        mCount[2] = 0;
        mTimer[2] = 0;
        setActionMode(ACTION_GLIDER_ATTACK, 0);
        return;
    }

    sp24 = home.pos - player->current.pos;
    sp24.y = 0.0f;

    if (mAnm == ANM_DR_FLAP) {
        if (sp24.abs() > 100.0f + l_HIO.wind_large_attack_lockon_range) {
            mpModelMorf->setPlaySpeed(1.0f);

            mCount[2] = 0;
            mTimer[2] = 0;
            if (mCount[1] == 0 && mTimer[1] == 0) {
                mTimer[1] = l_HIO.unk_0x48;
                mCount[1] = 1;
                if (WREG_S(1) != 0) {
                    mTimer[1] = 200;
                }
            }
        } else {
            field_0x7d3 = 2;
            mpModelMorf->setPlaySpeed(1.25f);

            if (mCount[2] == 0) {
                mCount[2] = 1;
                mTimer[2] = l_HIO.center_wind_time;
                if (WREG_S(1) != 0) {
                    mTimer[2] = 200;
                }
            }
        }

        if (NREG_S(0) != 0) {
            OS_REPORT("mCount[1] %d\n", mCount[1]);
            OS_REPORT("mCount[2] %d\n", mCount[2]);
            OS_REPORT("mTimer[1] %d\n", mTimer[1]);
            OS_REPORT("mTimer[2] %d\n\n", mTimer[2]);
        }

        if (WREG_S(1) == 0 && ((mCount[2] != 0 && cLib_calcTimer<int>(&mTimer[2]) == 0) || (mCount[1] != 0 && cLib_calcTimer<int>(&mTimer[1]) == 0))) {
            mCount[1] = 0;
            mTimer[1] = 0;
            mCount[2] = 0;
            mTimer[2] = 0;
            setActionMode(ACTION_GLIDER_ATTACK, 0);
            return;
        }

        if (sp24.abs() < l_HIO.wind_large_attack_lockon_range) {
            mStatusONOFF(1);
        } else {
            mStatusONOFF(0);
        }
    }

    cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), field_0x750, 0x400);
    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, field_0x750, 0x400);
    cLib_addCalcAngleS2(&field_0x750, 8, 8, 20);

    if (WREG_S(1) != 0) {
        mTargetHeight = 1500.0f + mAcch.GetGroundH();
    }

    cLib_addCalc2(&current.pos.y, 400.0f + mTargetHeight, 0.8f, 20.0f);
}

bool daB_DR_c::mGliderMoveSub(f32 param_0) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp18;

    field_0x7a8.set(field_0x73c * cM_ssin(mCount[0] * (NREG_S(5) + 0x1000)), 0.0f, field_0x73c * cM_scos(mCount[0] * (NREG_S(5) + 0x1000)));
    field_0x7a8 += field_0x7b4;

    field_0x7a8.y = player->current.pos.y;
    if (player->current.pos.y < 0.0f) {
        field_0x7a8.y = 0.0f;
    }
    if (mPlayerHighCheck()) {
        field_0x7a8.y = home.pos.y;
    }
    field_0x7a8.y += field_0x740;

    cLib_addCalc2(&field_0x740, param_0, 0.8f, 25.0f + WREG_F(0));

    sp18 = field_0x7a8 - current.pos;
    if (sp18.abs() > 1300.0f + NREG_F(6)) {
        return false;
    }

    if (field_0x7e9 == 0) {
        mCount[0]++;
    } else {
        mCount[0]--;
    }

    return true;
}

void daB_DR_c::executeGliderAttack() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz mae;
    cXyz ato;

    f32 var_f31 = 0.0f;

    switch (mMoveMode) {
    case 0:
        mStatusONOFF(0);
        field_0x750 = 8;
        field_0x740 = 4000.0f;
        mAllClr();

        if (cM_rnd() < 0.5f) {
            field_0x7e9 = 0;
            mCount[0] = 1;
        } else {
            field_0x7e9 = 1;
            mCount[0] = 79;
        }

        if (BREG_S(4) != 0) {
            field_0x7e9 = BREG_S(4) - 1;
            if (field_0x7e9 == 0) {
                mCount[0] = 1;
            } else {
                mCount[0] = 79;
            }
        }

        field_0x7d6 = 1;
        field_0x7a8.set((14000.0f + NREG_F(3)) * cM_ssin(mCount[0] * (NREG_S(5) + 0x1000)), 0.0f, (14000.0f + NREG_F(3)) * cM_scos(mCount[0] * (NREG_S(5) + 0x1000)));
        field_0x7dc = 0;
        field_0x7a8 += home.pos;
        field_0x7a8.y += field_0x740;
        mMoveMode++;
    case 1:
        ato = field_0x7a8 - current.pos;
        ato.y = 0.0f;
        if (abs((s16)(current.angle.y - (s16)ato.atan2sX_Z())) > 0x200) {
            break;
        }

        mTimer[0] = 0;
        mMoveMode++;
    case 2:
    case 3:
        if (mMoveMode == 2 && mAnm != ANM_DR_HOVERING) {
            mHabatakiAnmSet(0);
            if (mAnm == ANM_DR_FLY || (mAnm == ANM_DR_FLAP_TO_FLY && (int)mpModelMorf->getFrame() >= 15)) {
                mMoveMode++;
                break;
            }
        } else {
            mGlider_AniSet(0);
        }

        ato = field_0x7a8 - current.pos;
        if (ato.abs() < 3000.0f + NREG_F(5)) {
            if (field_0x7e9 == 0) {
                mCount[0]++;
            } else {
                mCount[0]--;
            }

            if (mCount[3] == 0) {
                field_0x750 = JREG_S(8) + 400;
                mCount[3] = 1;
            }

            field_0x7a8.set((14000.0f + NREG_F(3)) * cM_ssin(mCount[0] * (NREG_S(5) + 0x1000)), 0.0f, (14000.0f + NREG_F(3)) * cM_scos(mCount[0] * (NREG_S(5) + 0x1000)));
            field_0x7a8 += home.pos;
            field_0x7a8.y += field_0x740;
        }

        if (NREG_S(0) != 0) {
            OS_REPORT("mCount[0] %d\n", mCount[0]);
            OS_REPORT("ato.abs()  %f\n", ato.abs());
            OS_REPORT(" current.pos.z %f\n", current.pos.z);
            OS_REPORT("\n");
        }

        if (mCount[3] != 0) {
            if (current.pos.z <= 9000.0f + BREG_F(4) && (field_0x7d1 == 0 || mPlayerHighCheck() || !dComIfGp_checkPlayerStatus0(0, 0x100))) {
                mTimer[0] = 0;

                ato = home.pos - player->current.pos;
                ato.y = 0.0f;
                if ((field_0x7d1 == 0 || !mPlayerHighCheck()) && player->current.pos.x >= -l_HIO.unk_0x18 && player->current.pos.x <= l_HIO.unk_0x18) {
                    ato = home.pos - current.pos;
                    ato.y = 0.0f;
                    field_0x73c = ato.abs() / (1.0f + WREG_F(2));

                    field_0x7b4.set(home.pos);
                    if (field_0x7e9 == 0) {
                        mCount[0]++;
                    } else {
                        mCount[0]--;
                    }

                    mMoveMode = 10;
                    field_0x748 = 350.0f;
                } else {
                    ato = home.pos - current.pos;
                    ato.y = 0.0f;
                    field_0x73c = ato.abs() / (1.0f + WREG_F(2));

                    field_0x7b4.set(home.pos);
                    field_0x748 = mTargetHeight;
                    mMoveMode = 0xB;
                }

                if (field_0x7e9 == 0) {
                    mCount[0]++;
                } else {
                    mCount[0]--;
                }

                field_0x7d6 = 0;
                field_0x7a8.set(field_0x73c * cM_ssin(mCount[0] * (NREG_S(5) + 0x1000)), 0.0f, field_0x73c * cM_scos(mCount[0] * (NREG_S(5) + 0x1000)));
                field_0x7a8 += home.pos;

                if (player->current.pos.y >= 0.0f) {
                    field_0x7a8.y = player->current.pos.y + field_0x740;
                } else {
                    field_0x7a8.y = field_0x740;
                }
            }
        }
        break;
    case 11:
        var_f31 = 200.0f + mTargetHeight;
        ato = home.pos - current.pos;
        ato.y = 0.0f;

        if (field_0x73c < 6000.0f + BREG_F(12)) {
            field_0x7dc = 1;
            mGlider_AniSet(1);
        }

        if (field_0x73c < 8000.0f + BREG_F(13) && field_0x7d1 != 0 && !mPlayerHighCheck()) {
            setActionMode(ACTION_PILLAR_SEARCH, 0);
            return;
        }

        if (field_0x7dc != 0) {
            mGlider_AniSet(1);
        } else {
            mGlider_AniSet(0);
        }
    case 10:
        if (mMoveMode == 10) {
            var_f31 = 5000.0f + WREG_F(1);
            if (field_0x73c > 14000.0f || mAnm == ANM_DR_FLY_TO_GLIDE || mAnm == ANM_DR_GLIDE_TO_FLY) {
                mGlider_AniSet(1);
            } else if (current.pos.x >= -(4000.0f + WREG_F(10)) && current.pos.x <= (4000.0f + WREG_F(10))) {
                mGliderAtAniSet();
                field_0x7dc = 1;
            }

            if (-G_CM3D_F_INF != mAcch.GetGroundH()) {
                if (mAnm == ANM_DR_WIND_ATTACKB) {
                    mCount[1]++;
                    if (mCount[1] > WREG_S(9)) {
                        var_f31 = 350.0f;
                        mCount[1] = WREG_S(9) + 1;
                    }
                }
                field_0x7dc = 1;
            }

            if (field_0x7dc != 0) {
                mGliderAtAniSet();
            }
        }

        cLib_addCalc2(&field_0x748, var_f31, 0.1f, 0.025f);

        if (mGliderMoveSub(field_0x748) != 0) {
            if (field_0x73c < 3000.0f + BREG_F(14)) {
                if (mMoveMode == 11) {
                    if (mPlayerHighCheck() != 0) {
                        setActionMode(ACTION_BREATH_ATTACK, 0);
                    } else if (field_0x7d1 == 2) {
                        setActionMode(ACTION_FLY_WAIT, 0);
                    } else if (field_0x7d1 == 0) {
                        setActionMode(ACTION_WIND_ATTACK, 0);
                    }
                    return;
                }

                if (field_0x7e9 == 0) {
                    mCount[0] = 1;
                } else {
                    mCount[0] = 0x4F;
                }

                field_0x73c = 14000.0f + NREG_F(3);
                field_0x740 = 350.0f;

                field_0x7a8.set(field_0x73c * cM_ssin(mCount[0] * (NREG_S(5) + 0x1000)), 0.0f, field_0x73c * cM_scos(mCount[0] * (NREG_S(5) + 0x1000)));
                field_0x7a8 += home.pos;
                field_0x7a8.y += field_0x740;

                mTimer[0] = 0;
                mCount[2] = 0;
                mMoveMode = 20;
            } else {
                field_0x73c -= 4000.0f + BREG_F(15);
            }
        }
        break;
    case 20:
        mGlider_WindPosSet();
        var_f31 = 350.0f;

        if (mCount[2] == 0 && current.pos.z > 3000.0f + BREG_F(16)) {
            mCount[2] = 1;
        }

        if (mCount[2] != 0) {
            var_f31 = 5000.0f + WREG_F(1);
            mGlider_AniSet(0);
        }

        cLib_addCalc2(&field_0x748, var_f31, 0.1f, 0.025f);

        if (mGliderMoveSub(field_0x748) != 0) {
            mStatusONOFF(0);
            attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
            fopAcM_OffStatus(this, 0);

            mCount[2] = 0;
            mTimer[1] = 20;
            mMoveMode++;
        }
        break;
    case 21:
        mGliderMoveSub(4000.0f);
        mGlider_AniSet(0);

        if (cLib_calcTimer<int>(&mTimer[1]) != 0) {
            mGlider_WindPosSet();
            if (mTimer[1] == 1) {
                mGlider_WindPos.zero();
            }
            break;
        }

        field_0x73c = 14000.0f;
        mMoveMode++;
    case 22:
        mGliderMoveSub(4000.0f);

        if (current.pos.z < -4000.0f) {
            ato = home.pos - current.pos;
            ato.y = 0.0f;

            field_0x73c = ato.abs() / (1.0f + WREG_F(2));
            if (field_0x7e9 == 0) {
                mCount[0]++;
            } else {
                mCount[0]--;
            }

            field_0x7b4.set(home.pos);
            field_0x748 = mTargetHeight;
            mMoveMode = 0xB;
        } else {
            mGlider_AniSet(0);
        }
    }

    ato = field_0x7a8 - current.pos;
    cLib_addCalcAngleS2(&current.angle.y, (s16)ato.atan2sX_Z(), field_0x750, JREG_S(2) + 0x500);
    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, field_0x750, JREG_S(3) + 0x500);
    cLib_addCalcAngleS2(&field_0x750, JREG_S(4) + 8, JREG_S(5) + 20, JREG_S(6) + 40);
    cLib_addCalcAngleS2(&current.angle.x, ato.atan2sY_XZ(), 20, NREG_S(3) + 0x400);
    cLib_addCalcAngleS2(&shape_angle.x, current.angle.x, NREG_S(2) + 20, NREG_S(3) + 0x400);

    if (mMoveMode >= 2) {
        f32 var_f30 = 90.0f + NREG_F(9);
        if (mAnm == ANM_DR_FLY) {
            var_f30 = 120.0f + NREG_F(9);
        }
        cLib_addCalc2(&speedF, var_f30, 0.5f, 2.0f);
    }

    if (mMoveMode < 10 || mMoveMode > 22) {
        cMtx_YrotS(*calc_mtx, current.angle.y);
        cMtx_XrotM(*calc_mtx, current.angle.x);
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = speedF;
        MtxPosition(&mae, &ato);
        cLib_addCalc2(&speed.y, ato.y, 0.8f, 10.0f);
    } else {
        cLib_addCalc0(&speed.y, 0.8f, 10.0f);
        cLib_addCalc2(&current.pos.y, field_0x7a8.y, 0.8f, 25.0f);
    }
}

void daB_DR_c::executePillarSearch() {
    static cXyz mPillarTop_dt[] = {
        cXyz(3100.0f, 5500.0f, 1500.0f),
        cXyz(3100.0f, 5500.0f, -1500.0f),
        cXyz(-3100.0f, 5500.0f, 1500.0f),
        cXyz(-3100.0f, 5500.0f, -1500.0f),
    };

    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp30;

    switch (mMoveMode) {
    case 0:
        mTimer[0] = 0;
        field_0x750 = JREG_S(8) + 800;

        if (field_0x7e9 == 0) {
            mCount[1] = 2;
        } else {
            mCount[1] = 0;
        }

        mCount[1] += (int)cM_rndF(2.0f);
        mCount[1] &= 3;
        field_0x73c = 14000.0f;
        mMoveMode++;
        break;
    case 1:
        mGlider_AniSet(1);

        field_0x7a8.set(mPillarTop_dt[mCount[1]]);
        field_0x7b4.set(mPillarTop_dt[mCount[1]]);
        field_0x7b4.y = 6000.0f;

        sp30 = field_0x7a8 - current.pos;
        sp30.y = 0.0f;
        if (sp30.abs() > 2000.0f + NREG_F(12)) {
            break;
        }

        mMoveMode++;
    case 2:
        cLib_addCalc2(&current.pos.y, field_0x7b4.y, 0.8f + NREG_F(10), 5.0f + NREG_F(11));
        cLib_addCalc2(&speedF, 20.0f + NREG_F(14), 0.8f + NREG_F(15), 4.0f + NREG_F(16));

        if (!(fabsf(current.pos.y - field_0x7b4.y) > 20.0f) && !(speedF > 22.0f + NREG_F(14))) {
            mHabatakiAnmSet(0);

            field_0x7a8.set(home.pos);
            cLib_addCalc2(&current.pos.x, field_0x7b4.x, 0.8f + NREG_F(10), speedF);
            cLib_addCalc2(&current.pos.z, field_0x7b4.z, 0.8f + NREG_F(10), speedF);
            sp30 = field_0x7b4 - current.pos;

            if (mAnm == ANM_DR_HOVERING) {
                if ((int)mpModelMorf->getFrame() >= (int)(mpModelMorf->getEndFrame() - 1.0f) && sp30.abs() < 20.0f) {
                    speedF = 0.0f;
                    field_0x7a8.set(mPillarTop_dt[mCount[1]]);
                    field_0x7a8.y = 6000.0f;
                    setActionMode(ACTION_PILLAR_WAIT, 0);
                }
            }
        }
        break;
    }

    if (mActionMode != ACTION_PILLAR_WAIT) {
        sp30 = field_0x7a8 - current.pos;
        cLib_addCalcAngleS2(&current.angle.y, (s16)sp30.atan2sX_Z(), field_0x750, JREG_S(2) + 0x400);
        cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, field_0x750, JREG_S(3) + 0x400);
        cLib_addCalcAngleS2(&field_0x750, JREG_S(4) + 10, JREG_S(5) + 10, JREG_S(6) + 20);
        cLib_addCalcAngleS2(&current.angle.x, sp30.atan2sY_XZ(), 20, NREG_S(3) + 0x400);
        cLib_addCalcAngleS2(&shape_angle.x, current.angle.x, NREG_S(2) + 20, NREG_S(3) + 0x400);

        if (mPlayerHighCheck() && -G_CM3D_F_INF != mAcch.GetGroundH()) {
            setActionMode(ACTION_BREATH_ATTACK, 0);
        }
    }
}

void daB_DR_c::executePillarWait() {
#if DEBUG
    dDbVw_Report(30, 100, "Pole wait tyuu Timer:%d ", mTimer[0]);
    if (mTimer[1] != 0) {
        dDbVw_Report(30, 100, "sukosi zensin :%d ", mTimer[1]);
    }
#endif

    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz at_vec((40.0f + JREG_F(3)) * cM_ssin(shape_angle.y), 0.0f, (40.0f + JREG_F(3)) * cM_scos(shape_angle.y));
    cXyz sp14;

    switch (mMoveMode) {
    case 0:
        mStatusONOFF(2);
        dComIfGs_onSaveDunSwitch(22);
        field_0x7d6 = 0;
        mAllClr();

        mTimer[2] = 50;
        mTimer[0] = l_HIO.unk_0x62;

        mPillarCc.OnCoSetBit();
        speed.y = 0.0f;
        gravity = 0.0f;
        setBck(ANM_DR_POLE_STAYA, 0, 3.0f, 1.0f);
        mMoveMode++;
        break;
    case 1:
        if (!(fabsf(current.pos.y - field_0x7a8.y) > 2.0f) && mpModelMorf->isStop()) {
            current.pos.y = field_0x7a8.y;
            setBck(ANM_DR_POLE_STAYB, 2, 20.0f, 1.0f);
            mMoveMode++;
        }
        break;
    case 2:
        if (mAnm == ANM_DR_POLE_STAYD && mpModelMorf->isStop()) {
            setBck(ANM_DR_POLE_STAYB, 2, 20.0f, 1.0f);
        }

        if (cLib_calcTimer<int>(&mTimer[2]) == 0 && mCount[0] == 0) {
            setBck(ANM_DR_POLE_STAYD, 0, 10.0f, 1.0f);
            mCount[0] = 1;
        }

        if (player->getHookshotTopPos()) {
            sp14.set(current.pos);
            sp14.y -= 500.0f;
            if (player->getHookshotTopPos()->abs(sp14) < 500.0f) {
                mStatusONOFF(0);
                mPillarCc.ClrTgHit();
                mPillarCc.OffCoSetBit();

                field_0x7a8.y = mTargetHeight;
                setBck(ANM_DR_POLE_STAYC, 0, 3.0f, 1.0f);
                mMoveMode++;
                break;
            }
        }

        if (mPlayerHighCheck()) {
            mTimer[0] = l_HIO.unk_0x62;
        }

        if ((fopAcM_searchPlayerDistanceXZ(this) < 350.0f && player->current.pos.y > 4500.0f) || mTargetHeight < player->current.pos.y || cLib_calcTimer<int>(&mTimer[0]) == 0 || (player->current.pos.y >= 5000.0f && fopAcM_searchPlayerDistanceXZ(this) > 3100.0f) || mPillarCc.ChkTgHit()) {
            mStatusONOFF(0);
            mPillarCc.ClrTgHit();
            mPillarCc.OffCoSetBit();

            field_0x7a8.set(current.pos);
            field_0x7a8.y = 100.0f + mTargetHeight;
            setBck(ANM_DR_POLE_STAYC, 0, 3.0f, 1.0f);
            mMoveMode++;
        }
        break;
    case 3:
        if ((int)mpModelMorf->getFrame() >= 14) {
            field_0x7a8.y = 6150.0f;
            cLib_addCalcPos(&current.pos, field_0x7a8, 0.8f, 40.0f, 10.0f);
        }

        if (!mpModelMorf->isStop()) {
            break;
        }

        field_0x7a8.set(home.pos);
        field_0x7a8.y = mTargetHeight;
        dComIfGs_offSaveDunSwitch(22);
        setBck(ANM_DR_HOVERING, 2, 3.0f, 1.0f);
        mMoveMode++;
    case 4:
        cLib_addCalcPos(&current.pos, field_0x7a8, 0.8f, 20.0f, 5.0f);

        if (!(mAcch.GetGroundH() > 1000.0f)) {
            if (home.pos.y + l_HIO.breath_attack_threshold <= player->current.pos.y) {
                setActionMode(ACTION_BREATH_ATTACK, 0);
            } else if (field_0x7d1 != 2) {
                setActionMode(ACTION_WAIT, 0);
            } else {
                setActionMode(ACTION_FLY_WAIT, 0);
            }
        }
        break;
    }

    sp14.set(current.pos);
    sp14.y += -200.0f;

    mPillarCc.SetC(sp14);
    mPillarCc.SetH(1000.0f);
    mPillarCc.SetR(400.0f);
    mPillarCc.SetAtVec(at_vec);
    dComIfG_Ccsp()->Set(&mPillarCc);

    cLib_addCalc0(&speedF, 0.8f, 5.0f);
    sp14 = home.pos - current.pos;
    cLib_addCalcAngleS2(&current.angle.y, (s16)sp14.atan2sX_Z(), 10, 0x400);
    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 10, 0x400);
}

bool daB_DR_c::startDemoCheck() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = dComIfGp_getCamera(0);

    if (!eventInfo.checkCommandDemoAccrpt()) {
        fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
        eventInfo.onCondition(2);
        mDemoCamEye = camera0->lookat.eye;
        mDemoCamCenter = camera0->lookat.center;
        return false;
    } else {
        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(3);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        return true;
    }
}

void daB_DR_c::executeDamageDemo() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz unused;
    cXyz unused2;
    cXyz mae;
    cXyz ato;

    switch (mMoveMode) {
    case 0:
        if (startDemoCheck()) {
            mAllClr();
            current.pos.x = home.pos.x;
            current.pos.z = home.pos.z;
            setBck(ANM_DR_BOOT_A_DEMO, 0, 0.0f, 1.0f);
            mMoveMode++;
        }
        break;
    case 1:
        cMtx_YrotS(*calc_mtx, (shape_angle.y + 0x8000));
        mae.x = -500.0f + ZREG_F(0);
        mae.y = -1400.0f + ZREG_F(1);
        mae.z = 1100.0f + ZREG_F(2);
        MtxPosition(&mae, &ato);
        ato += current.pos;
        if (ato.y < 200.0f) {
            ato.y = 200.0f;
        }
        mDemoCamEye.set(ato);

        cMtx_YrotS(*calc_mtx, (shape_angle.y + 0x8000));
        mae.x = ZREG_F(3);
        mae.y = -850.0f + ZREG_F(4);
        mae.z = ZREG_F(5);
        MtxPosition(&mae, &ato);
        ato += current.pos;
        if (ato.y < 200.0f) {
            ato.y = 200.0f;
        }
        mDemoCamCenter.set(ato);

        if ((int)mpModelMorf->getFrame() < 50) {
            break;
        }
        mMoveMode++;
    case 2:
        cMtx_YrotS(*calc_mtx, (shape_angle.y + 0x8000));
        mae.x = 400.0f + ZREG_F(0);
        mae.y = 700.0f + ZREG_F(1);
        mae.z = -1200.0f + ZREG_F(2);
        MtxPosition(&mae, &ato);
        ato += current.pos;
        mDemoCamEye.set(ato);

        cMtx_YrotS(*calc_mtx, (shape_angle.y + 0x8000));
        mae.x = ZREG_F(3);
        mae.y = 400.0f + ZREG_F(4);
        mae.z = ZREG_F(5);
        MtxPosition(&mae, &ato);
        ato += current.pos;
        mDemoCamCenter.set(ato);

        if (mpModelMorf->isStop() && BREG_S(2) == 0) {
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();

            if (bitSw != 0xFF) {
                fopAcM_onSwitch(this, bitSw);
            }

            setActionMode(ACTION_TAIL_HIT, 10);

            if (mAnm != ANM_DR_BOOT_A) {
                setBck(ANM_DR_BOOT_A, 0, 10.0f, 1.0f);
            }
        }
        break;
    case 10:
        if (startDemoCheck()) {
            shape_angle.y = current.angle.y = home.angle.y;
            current.pos.x = home.pos.x;
            current.pos.z = home.pos.z;
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&player->current.pos, home.angle.y, 0);
            setBck(ANM_DR_IMPACT, 0, 3.0f, 1.0f);
            mMoveMode++;
        }
        break;
    case 11:
        if (mAnm == ANM_DR_BOOT_C) {
            if ((int)mpModelMorf->getFrame() >= 34) {
                cLib_addCalc2(&mDemoCamCenter.y, player->current.pos.y, 0.7f, 150.0f);
            }

            if (mpModelMorf->isStop()) {
                setBck(ANM_DR_DOWN_WAIT, 2, 3.0f, 1.0f);
                mTimer[0] = 35;
            }
        }

        if (mAnm == ANM_DR_IMPACT && mpModelMorf->isStop()) {
            setBck(ANM_DR_BOOT_C, 0, 3.0f, 1.0f);
            gravity = -1.0f;
        }

        if (!mBgFallGroundCheck()) {
            break;
        }

        dComIfGs_onZoneSwitch(1, fopAcM_GetRoomNo(this));
        mMoveMode++;
    case 12:
        if (mAnm == ANM_DR_BOOT_C) {
            if ((int)mpModelMorf->getFrame() >= 34) {
                cLib_addCalc2(&mDemoCamCenter.y, player->current.pos.y, 0.7f, 150.0f);
            }

            mTimer[0] = 50;

            if (!mpModelMorf->isStop()) {
                break;
            }
        }

        if (mAnm != ANM_DR_DOWN_WAIT) {
            setBck(ANM_DR_DOWN_WAIT, 2, 10.0f, 1.0f);
            mTimer[0] = 35;
        }

        if (cLib_calcTimer<int>(&mTimer[0]) != 0) {
            break;
        }

        mTimer[0] = 70;
        mMoveMode++;
    case 13:
        if (cLib_calcTimer<int>(&mTimer[0]) == 0) {
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
 
            field_0x7d7 = 1;
            mTimer[0] = 35;
            setActionMode(ACTION_TAIL_HIT, 20);
        }
    }

    if (mMoveMode >= 10 && mMoveMode <= 12) {
        if (mAnm != ANM_DR_BOOT_C && mAnm != ANM_DR_DOWN_WAIT) {
            ato.set(home.pos);
            ato.y += 1200.0f + JREG_F(10);
            mDemoCamCenter.set(ato);
        } else {
            ato.set(1500.0f, 0.0f, 1500.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&ato, -0x8000, 0);
        }

        cMtx_YrotS(*calc_mtx, shape_angle.y - (JREG_S(8) + 0x1000));
        mae.x = 0.0f;
        mae.y = 200.0f + JREG_F(11);
        mae.z = 1000.0f + JREG_F(12);
        MtxPosition(&mae, &ato);
        mDemoCamEye = (ato + current.pos) + mBoot_c_trance;
        daPy_getPlayerActorClass()->onBossRoomWait();
    }

    if (mMoveMode != 0 && mMoveMode != 10) {
        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);
    }

    if (mAnm == ANM_DR_BOOT_C) {
        if (mpModelMorf->checkFrame(45.0f)) {
            if (field_0x7d1 == 0) {
                mSound.startCreatureSound(Z2SE_EN_DR_CRASH, 0, -1);
                field_0x7d1 = 1;
                mBreakPartsNo++;
            }
            field_0x7e0 = 0;
        }
    
        mBoot_c_tranceSet(0);

        if ((int)mpModelMorf->getFrame() >= 45) {
            chkPartCreate(0);
        }
    }

    cXyz offset;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::inverse();
    mDoMtx_stack_c::multVec(&eyePos, &offset);
    daPy_getPlayerActorClass()->setHookshotCarryOffset(fopAcM_GetID(this), &offset);
}

void daB_DR_c::executeMiddleDemo() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz unused;
    cXyz unused2;
    cXyz mae;
    cXyz ato;

    switch (mMoveMode) {
    case 0:
        if (startDemoCheck()) {
            mAllClr();
            mMoveMode++;
        }
        break;
    case 1:
        cMtx_YrotS(*calc_mtx, fopAcM_searchPlayerAngleY(this) + 0x8000);
        mae.x = ZREG_F(0);
        mae.y = ZREG_F(1);
        mae.z = ZREG_F(2);
        MtxPosition(&mae, &ato);
        ato += player->current.pos;
        mDemoCamCenter.set(ato);
        break;
    }

    if (mMoveMode >= 1) {
        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);
    }
}

void daB_DR_c::executeDead() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz unused;
    cXyz unused2;
    cXyz mae;
    cXyz ato;

    switch (mMoveMode) {
    case 0:
        if (!startDemoCheck()) {
            break;
        }
        mAllClr();
    case 1:
        cMtx_YrotS(*calc_mtx, shape_angle.y);
        mae.x = ZREG_F(0);
        mae.y = ZREG_F(1);
        mae.z = ZREG_F(2);
        MtxPosition(&mae, &ato);
        ato += current.pos;
        mDemoCamCenter.set(ato);

        cMtx_YrotS(*calc_mtx, shape_angle.y);
        mae.x = ZREG_F(3);
        mae.y = ZREG_F(4);
        mae.z = 100.0f + ZREG_F(5);
        MtxPosition(&mae, &ato);
        ato += current.pos;
        mDemoCamEye.set(ato);
        break;
    }

    if (mMoveMode >= 1) {
        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);
    }
}

void daB_DR_c::executeBullet() {
    cXyz mae;

    switch (mMoveMode) {
    case 0:
        field_0x730 = 100.0f;
        speedF = 100.0f;
        mAllClr();

        mTimer[0] = data_805C7974;
        mMoveMode++;
        return;
    case 1:
        if (cLib_calcTimer<int>(&mTimer[0]) == 0) {
            fopAcM_delete(this);
            return;
        }

        if (parentActorID != 0) {
            daB_DR_c* dr_p = (daB_DR_c*)fopAcM_SearchByID(fopAcM_GetLinkId(this));
            if (dr_p != NULL) {
                current.angle.y -= (s16)((s16)(dr_p->mHeadAngle.y - home.angle.y) * (0.002f + JREG_F(13)));
            }
        }

        cMtx_YrotS(*calc_mtx, current.angle.y);
        cMtx_XrotM(*calc_mtx, current.angle.x);
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = speedF;
        MtxPosition(&mae, &speed);

        field_0x730 += 10.0f;
        if (field_0x730 > 100.0f) {
            field_0x730 = 100.0f;
        }

        mBreathAtCc.SetC(current.pos);
        mBreathAtCc.SetR(field_0x730);
        dComIfG_Ccsp()->Set(&mBreathAtCc);
    }
}

void daB_DR_c::executeParts() {
    camera_class* camera = dComIfGp_getCamera(0);
    cXyz mae;
    cXyz ato;

    switch (mMoveMode) {
    case 0:
        speedF = 20.0f + ZREG_F(13) + cM_rndF(10.0f + ZREG_F(14));
        current.angle.y += (s16)cM_rndFX(65536.0f);
        speed.y = 30.0f + ZREG_F(10);
        speed.y += cM_rndFX(10.0f + ZREG_F(11));

        field_0x752 = cM_rndF(2048.0f + ZREG_F(9));
        mTimer[0] = 100;
        gravity = -3.0f + ZREG_F(12);
        mMoveMode++;
        break;
    case 1:
        shape_angle.x += field_0x752;
        if (!mAcch.ChkGroundHit()) {
            if (cLib_calcTimer<int>(&mTimer[0]) == 0) {
                fopAcM_delete(this);
            }
            break;
        }

        field_0x752 *= 2;
        mMoveMode++;
    case 2:
        shape_angle.x += field_0x752;
        cLib_addCalc0(&speedF, 0.7f, 1.0f + ZREG_F(15));
        cLib_addCalcAngleS2(&field_0x752, 0, 8, speedF);
        if (speedF > 0.2f) {
            break;
        }
        speedF = 0.0f;
        mTimer[0] = 10;
        mMoveMode++;
    case 3:
        if (cLib_calcTimer<int>(&mTimer[0]) == 0) {
            setDeathLightEffect();
            fopAcM_delete(this);
        }
        break;
    case 10:
        speedF = 40.0f + ZREG_F(13) + cM_rndF(10.0f + ZREG_F(14));
        mTimer[0] = 50;
        current.angle.x += (s16)cM_rndFX(16384.0f);

        mae = camera->lookat.center - current.pos;
        current.angle.y = mae.atan2sX_Z();
        current.angle.y += (s16)cM_rndFX(16384.0f);

        cMtx_YrotS(*calc_mtx, current.angle.y);
        cMtx_XrotM(*calc_mtx, current.angle.x);
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = speedF;
        MtxPosition(&mae, &ato);
        speed.y = ato.y;

        mMoveMode++;
        break;
    case 11:
        if (cLib_calcTimer<int>(&mTimer[0]) == 0) {
            fopAcM_delete(this);
        }
        break;
    }
}

void daB_DR_c::action() {
    tail_hit_check();
    week_hit_check();
    field_0x7d4 = 0;
    field_0x7d3 = 0;

    switch (mActionMode) {
    case ACTION_WAIT:
        executeWait();
        break;
    case ACTION_FLY_WAIT:
        executeFlyWait();
        break;
    case ACTION_TAIL_HIT:
        field_0x7d4 = 1;
        executeTailHit();
        break;
    case ACTION_WEEK_HIT:
        field_0x7d4 = 1;
        executeWeekHit();
        break;
    case ACTION_WIND_ATTACK:
        field_0x7d3 = 2;
        executeWindAttack();
        break;
    case ACTION_BREATH_ATTACK:
    case ACTION_BREATH_ATTACK2:
        executeBreathAttack();
        break;
    case ACTION_GLIDER_ATTACK:
        field_0x7d4 = 1;
        executeGliderAttack();
        break;
    case ACTION_PILLAR_SEARCH:
        field_0x7d4 = 1;
        executePillarSearch();
        break;
    case ACTION_PILLAR_WAIT:
        field_0x7d4 = 1;
        executePillarWait();
        break;
    case ACTION_DAMAGE_DEMO:
        field_0x7d4 = 1;
        executeDamageDemo();
        break;
    case ACTION_MIDDLE_DEMO:
        field_0x7d4 = 1;
        executeMiddleDemo();
        break;
    case ACTION_DEAD:
        field_0x7d4 = 1;
        executeDead();
        break;
    }

    if (NREG_S(1) != 0) {
        OS_REPORT("mActionMode %d\n", mActionMode);
        OS_REPORT("mMoveMode %d\n", mMoveMode);
    }

    if (mAnm == ANM_DR_TAKE_OFF) {
        if ((int)mpModelMorf->getFrame() >= 30) {
            field_0x7d3 = 2;
        }
    }

    if (mAnm == ANM_DR_BOOT_A_DEMO) {
        if (mpModelMorf->checkFrame(8.0f)) {
            dComIfGp_getVibration().StartShock(5, 0x4F, cXyz(0.0f, 1.0f, 0.0f));
        }
        if (mpModelMorf->checkFrame(50.0f)) {
            field_0x7db = 1;
            dComIfGp_getVibration().StartQuake(7, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        }
    } else if (field_0x7db == 1) {
        field_0x7db = 0;
        dComIfGp_getVibration().StopQuake(0x1F);
    }

    setSe();

    if (daPy_getPlayerActorClass()->checkWolfLock(this)) {
        daPy_getPlayerActorClass()->cancelWolfLock(this);
    }

    if (mActionMode != ACTION_GLIDER_ATTACK || mMoveMode == 1000) {
        if (current.angle.x != 0 || shape_angle.x != 0) {
            cLib_addCalcAngleS2(&current.angle.x, 0, 8, 0x400);
            cLib_addCalcAngleS2(&shape_angle.x, current.angle.x, 8, 0x400);
        }
        field_0x74e = 0;
    } else {
        s16 temp_r28 = -3.0f * (current.angle.y - shape_angle.y);
        if (abs(temp_r28) < 0x2000) {
            field_0x74e = temp_r28;
        }
        if (mAnm == ANM_DR_WIND_ATTACKATOB || mAnm == ANM_DR_WIND_ATTACKB || -G_CM3D_F_INF != mAcch.GetGroundH() || mCount[3] == 0) {
            field_0x74e = 0;
        }
    }

    cLib_addCalcAngleS2(&shape_angle.z, field_0x74e, NREG_S(6) + 40, NREG_S(7) + 0x400);

    if (BREG_S(1) != 0) {
        mWeekCc.OnTgSetBit();
        speedF = 0.0f;
        speed.zero();
    }

    fopAcM_posMoveF(this, mColliderStts.GetCCMoveP());

    if (mActionMode != ACTION_BREATH_ATTACK && mActionMode != ACTION_BREATH_ATTACK2) {
        mHeadAngleSet();
    }

    mAcch.SetWallNone();
    mAcch.SetGroundUpY(75.0f);
    mAcch.CrrPos(dComIfG_Bgsp());

    mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mSound2.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mpCoreBrk->play();
}

void daB_DR_c::action_fire() {
    executeBullet();
    shape_angle = current.angle;
    fopAcM_posMove(this, mColliderStts.GetCCMoveP());
}

void daB_DR_c::action_parts() {
    executeParts();
    fopAcM_posMoveF(this, mColliderStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daB_DR_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(mBoot_c_trance.x, mBoot_c_trance.y, mBoot_c_trance.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModelMorf->modelCalc();

    if (mAnm == ANM_DR_POLE_STAYA) {
        cXyz sp1C;
        dBgS_GndChk sp28;

        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(0x22));
        mDoMtx_stack_c::multVecZero(&sp1C);
        sp28.SetPos(&sp1C);
        
        f32 var_f31 = dComIfG_Bgsp().GroundCross(&sp28);
        if (-G_CM3D_F_INF == var_f31 || var_f31 < 5250.0f + JREG_F(4)) {
            var_f31 = 5250.0f + JREG_F(4);
        }
        field_0x7a8.y = var_f31;
    
        if ((int)mpModelMorf->getFrame() < 35.0f + JREG_F(5)) {
            cLib_addCalcPos(&current.pos, field_0x7a8, 0.8f, 10.0f + JREG_F(6), 5.0f + JREG_F(7));
        } else {
            cLib_addCalcPos(&current.pos, field_0x7a8, 0.8f, 20.0f + JREG_F(8), 5.0f + JREG_F(9));
        }

        if (current.pos.y < field_0x7a8.y) {
            current.pos.y = field_0x7a8.y;
        }
    }

    if (mAnm == ANM_DR_WIND_ATTACKB && -G_CM3D_F_INF != mAcch.GetGroundH()) {
        cXyz sp10 = current.pos - home.pos;
        if (sp10.abs() > 3000.0f || current.pos.y > 370.0f) {
            if (field_0x7d8 == 0) {
                mSound.startCreatureVoice(Z2SE_EN_DR_V_TUME, -1);
                field_0x7d8 = 1;
            }
        } else {
            for (int i = 0; i < 4; i++) {
                static u16 effLId[] = {0x89E4, 0x89E5, 0x89E6, 0x89E7};
                field_0x27ec[i] = dComIfGp_particle_set(field_0x27ec[i], effLId[i], &current.pos, NULL, NULL);
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0x27ec[i]);
                if (emitter != NULL) {
                    emitter->setGlobalSRTMatrix(mpModelMorf->getModel()->getAnmMtx(0x22));
                }

                mSound.startCreatureSoundLevel(Z2SE_EN_DR_TUME, 0, -1);

                static u16 effRId[] = {0x89E8, 0x89E9, 0x89EA, 0x89EB};
                field_0x27fc[i] = dComIfGp_particle_set(field_0x27fc[i], effRId[i], &current.pos, NULL, NULL);
                emitter = dComIfGp_particle_getEmitter(field_0x27fc[i]);
                if (emitter != NULL) {
                    emitter->setGlobalSRTMatrix(mpModelMorf->getModel()->getAnmMtx(0x2A));
                }
            }
        }
    }
}

void daB_DR_c::parts_mtx_set() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale.x, scale.y, scale.z);
    mpPartModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daB_DR_c::cc_set() {
    cXyz mae;
    cXyz ato;
    cXyz at_vec;
    J3DModel* model_p = mpModelMorf->getModel();

    if (mActionMode == ACTION_PILLAR_WAIT || mActionMode == ACTION_GLIDER_ATTACK) {
        mDoMtx_stack_c::copy(model_p->getAnmMtx(0x13));
        mDoMtx_stack_c::multVecZero(&eyePos);
    } else if (field_0x7d1 != 2) {
        mDoMtx_stack_c::copy(model_p->getAnmMtx(0x31));
        mDoMtx_stack_c::multVecZero(&eyePos);
    } else if (mWeekCc.ChkTgSet()) {
        mDoMtx_stack_c::copy(model_p->getAnmMtx(15));
        mDoMtx_stack_c::multVecZero(&eyePos);
    }

    attention_info.position = eyePos;
    attention_info.position.y += 100.0f;

    mDoMtx_stack_c::copy(model_p->getAnmMtx(0x31));
    mae.set(0.0f, 0.0f, 50.0f);
    mDoMtx_stack_c::multVec(&mae, &ato);
    mTailCc.SetC(ato);
    mTailCc.SetR(250.0f);
    dComIfG_Ccsp()->Set(&mTailCc);

    mDoMtx_stack_c::copy(model_p->getAnmMtx(0x13));
    mDoMtx_stack_c::multVecZero(&field_0x760);
    mDoMtx_stack_c::multVecZero(&field_0x790);
    mDoMtx_stack_c::copy(model_p->getAnmMtx(0x14));
    mae.set(100.0f + ZREG_F(17), -200.0f + ZREG_F(18), ZREG_F(19));
    mDoMtx_stack_c::multVec(&mae, &field_0x778);

    at_vec.set(l_HIO.wind_small_attack_power * cM_ssin(shape_angle.y), 0.0f, l_HIO.wind_small_attack_power * cM_scos(shape_angle.y));

    f32 wind_at_radius = 1000.0f;
    bool var_r27 = false;

    if (field_0x7d4 == 0) {
        mDoMtx_stack_c::copy(model_p->getAnmMtx(0));
        mDoMtx_stack_c::multVecZero(&ato);
        mWindAtCc.SetC(ato);
        mWindAtCc.SetR(wind_at_radius);
        mWindAtCc.SetAtVec(at_vec);
        dComIfG_Ccsp()->Set(&mWindAtCc);
    }

    if (mActionMode == ACTION_GLIDER_ATTACK) {
        ato.set(mGlider_WindPos);

        if (!mGlider_WindPos.x && !mGlider_WindPos.y && !mGlider_WindPos.z) {
            wind_at_radius = 0.0f;
        } else {
            at_vec.set(l_HIO.glide_wind_power * cM_ssin(shape_angle.y), 0.0f, l_HIO.glide_wind_power * cM_scos(shape_angle.y));
            wind_at_radius = 2000.0f;
            field_0x784.set(mGlider_WindPos);
            mSound2.startCreatureSoundLevel(Z2SE_EN_DR_WIND_ATTACK, 0, -1);
        }

        if (mAnm == ANM_DR_WIND_ATTACKB && -G_CM3D_F_INF != mAcch.GetGroundH() && mCount[1] > WREG_S(9)) {
            var_r27 = 1;
        }

        mWindAtCc2.SetC(ato);
        mWindAtCc2.SetR(wind_at_radius);
        mWindAtCc2.SetAtVec(at_vec);
        dComIfG_Ccsp()->Set(&mWindAtCc2);
    }

    if (field_0x7d3 != 0) {
        mPlayerHighCheck();
        ato.set(current.pos);
        ato.y = 600.0f + field_0x734;
        mWindAtCc3.SetC(ato);
        mWindAtCc3.SetR(4500.0f);

        if (field_0x7d3 == 2) {
            at_vec.set(l_HIO.wind_attack_power * cM_ssin(shape_angle.y), 0.0f, l_HIO.wind_attack_power * cM_scos(shape_angle.y));
            var_r27 = 1;
        }

        mSound.startCreatureSoundLevel(Z2SE_EN_DR_FLAP, 0, -1);
        mWindAtCc3.SetAtVec(at_vec);
        dComIfG_Ccsp()->Set(&mWindAtCc3);
    }

    if (var_r27 == 0) {
        cLib_addCalc(&field_0x738, 0.6f, 0.05f, 0.003f, 0.00001f);
    } else {
        cLib_addCalc(&field_0x738, 0.9f, 0.5f, 0.2f, 0.001f);
        dKyw_evt_wind_set(0, shape_angle.y);
    }

    dKyw_custom_windpower(field_0x738);

    if (mAnm == ANM_DR_WIND_ATTACKB) {
        at_vec.set((40.0f + JREG_F(3)) * cM_ssin(shape_angle.y), 0.0f, (40.0f + JREG_F(3)) * cM_scos(shape_angle.y));
        mDoMtx_stack_c::copy(model_p->getAnmMtx(0x1D));
        mae.set(200.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::multVec(&mae, &ato);
        mFootAtCc[0].SetC(ato);
        mFootAtCc[0].SetR(150.0f);
        mFootAtCc[0].SetAtVec(at_vec);
        dComIfG_Ccsp()->Set(&mFootAtCc[0]);

        mDoMtx_stack_c::copy(model_p->getAnmMtx(0x25));
        mae.set(200.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::multVec(&mae, &ato);
        mFootAtCc[1].SetC(ato);
        mFootAtCc[1].SetR(150.0f);
        mFootAtCc[1].SetAtVec(at_vec);
        dComIfG_Ccsp()->Set(&mFootAtCc[1]);
    }

    mDoMtx_stack_c::copy(model_p->getAnmMtx(0xF));
    mae.set(-200.0f + WREG_F(2), WREG_F(3), WREG_F(4));
    mDoMtx_stack_c::multVec(&mae, &ato);
    mWeekCc.SetC(ato);
    mWeekCc.SetH(300.0f + WREG_F(5));
    mWeekCc.SetR(150.0f + WREG_F(6));
    dComIfG_Ccsp()->Set(&mWeekCc);
}

void daB_DR_c::down_cc_set() {
    cXyz mae;
    cXyz ato;

    for (int i = 0; i < 5; i++) {
        static const struct {
            int jnt_no;
            Vec pos;
            f32 height;
            f32 radius;
        } DOWN_CYL_DT[] = {
            { 0, 0.0f, -100.0f, 140.0f, 200.0f, 300.0f },
            { 6, -50.0f, 100.0f, 250.0f, 200.0f, 300.0f },
            { 7, -50.0f, 100.0f, 170.0f, 200.0f, 370.0f },
            { 12, 0.0f, 0.0f, -150.0f, 200.0f, 300.0f },
            { 13, 0.0f, 0.0f, -200.0f, 200.0f, 350.0f },
        };

        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(DOWN_CYL_DT[i].jnt_no));
        mae.x = DOWN_CYL_DT[i].pos.x;
        mae.y = DOWN_CYL_DT[i].pos.y;
        mae.z = DOWN_CYL_DT[i].pos.z;
        mDoMtx_stack_c::multVec(&mae, &ato);

        mDownCc[i].SetC(ato);
        mDownCc[i].SetH(DOWN_CYL_DT[i].height);
        mDownCc[i].SetR(DOWN_CYL_DT[i].radius);
        dComIfG_Ccsp()->Set(&mDownCc[i]);
    }

    for (int i = 0; i < 9; i++) {
        static const struct {
            int jnt_no;
            Vec pos;
            f32 radius;
        } DOWN_SPH_DT[] = {
            { 19, 50.0f, 0.0f, 0.0f, 140.0f },
            { 43, 0.0f, 0.0f, 0.0f, 150.0f },
            { 44, 0.0f, 0.0f, 0.0f, 130.0f },
            { 45, 0.0f, 0.0f, 0.0f, 130.0f },
            { 46, 0.0f, 0.0f, 0.0f, 130.0f },
            { 47, 0.0f, 0.0f, 0.0f, 130.0f },
            { 48, 0.0f, 0.0f, 0.0f, 130.0f },
            { 29, 150.0f, 0.0f, -50.0f, 130.0f },
            { 37, 180.0f, 0.0f, -50.0f, 130.0f },
        };

        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(DOWN_SPH_DT[i].jnt_no));
        mae.x = DOWN_SPH_DT[i].pos.x;
        mae.y = DOWN_SPH_DT[i].pos.y;
        mae.z = DOWN_SPH_DT[i].pos.z;
        mDoMtx_stack_c::multVec(&mae, &ato);

        mDownSphCc[i].SetC(ato);
        mDownSphCc[i].SetR(DOWN_SPH_DT[i].radius);
        dComIfG_Ccsp()->Set(&mDownSphCc[i]);
    }
}

void daB_DR_c::demo_skip(int) {
    fopAc_ac_c* parent;
    if (cDmr_SkipInfo == 0 && fopAcM_SearchByID(parentActorID, &parent) && parent != NULL) {
        camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
        dStage_changeScene(1, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
        Z2GetAudioMgr()->subBgmStop();
        dKy_getEnvlight()->wether = 1;

        cDmr_SkipInfo = 1;
        dComIfGs_onZoneSwitch(0, fopAcM_GetRoomNo(this));
        dComIfGs_onZoneSwitch(20, fopAcM_GetRoomNo(this));

        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        fopAcM_delete(parent);
    }
}

int daB_DR_c::DemoSkipCallBack(void* i_proc, int param_1) {
    if (i_proc != NULL) {
        ((daB_DR_c*)i_proc)->demo_skip(param_1);
        return 1;
    }

    return 0;
}

int daB_DR_c::execute() {
    if (field_0x7d5 != 0) {
        cXyz restart_pos;
        csXyz restart_angle;

        if (field_0x7d5 > 10) {
            field_0x7d5 -= 10;
            return 1;
        }

        if (field_0x7d5 == 2) {
            daNpcF_getPlayerInfoFromPlayerList(2, fopAcM_GetRoomNo(this), restart_pos, restart_angle);
            dComIfGs_setRestartRoom(restart_pos, restart_angle.y, fopAcM_GetRoomNo(this));
            OS_REPORT("２階からスタート \n");
            OS_REPORT("２階からスタート \n");
            OS_REPORT("２階からスタート \n");
        } else {
            daNpcF_getPlayerInfoFromPlayerList(0, fopAcM_GetRoomNo(this), restart_pos, restart_angle);
            dComIfGs_setRestartRoom(restart_pos, restart_angle.y, fopAcM_GetRoomNo(this));
        }

        field_0x7d5 = 0;
    }

    if (arg0 == 0xFF) {
        if (parentActorID != 0 && health > 1 && cLib_calcTimer<int>(&mTimer[0]) == 0) {
            fopAc_ac_c* parent;
            if (fopAcM_SearchByID(parentActorID, &parent) != 0 && parent != NULL && parent->argument != 1) {
                if (dComIfGs_isZoneSwitch(20, fopAcM_GetRoomNo(this))) {
                    dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 0);
                }
            } else if (!dComIfGs_isZoneSwitch(0, fopAcM_GetRoomNo(this)) && cLib_calcTimer<int>(&mTimer[0]) == 0) {
                dComIfGs_onZoneSwitch(0, fopAcM_GetRoomNo(this));
                dComIfGs_onZoneSwitch(20, fopAcM_GetRoomNo(this));
                Z2GetAudioMgr()->bgmStart(Z2BGM_DRAGON_BTL01, 0, 0);
                arg0 = 0;
            }
        }

        if (arg0 == 0xFF) {
            return 1;
        }
    }

    if (arg0 == 0xFE && cLib_calcTimer<int>(&mTimer[0]) == 0) {
        if (parentActorID != 0) {
            fopAc_ac_c* parent = NULL;
            if (!fopAcM_SearchByID(parentActorID, &parent)) {
                if (!dComIfGp_event_runCheck()) {
                    arg0 = 1;
                    field_0x7d1 = 2;
                    parentActorID = 0;
                    mBreakPartsNo = 3;
                    mTargetHeight = 6000.0f + home.pos.y;
                    setBck(ANM_DR_TAKE_OFF, 0, 10.0f, 1.0f);
                    mBoot_c_trance.zero();
                    field_0x724 = 50.0f;
                    mMoveMode = 100;
                    dComIfGs_onZoneSwitch(2, fopAcM_GetRoomNo(this));
                }
            } else {
                current.pos.set(parent->current.pos);
                current.angle = parent->current.angle;
                shape_angle = parent->shape_angle;

                if (parent->argument == 1 && field_0x7e0 < 54) {
                    if (mAnm != ANM_DR_BURU) {
                        setBck(ANM_DR_BURU, 0, 0.0f, 1.0f);
                        mpModelMorf->setStartFrame(111.0f);
                        mSound.startCreatureSound(Z2SE_EN_DR_CRASH, 0, -1);
                    }

                    mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
                    mDoMtx_stack_c::transS(current.pos);
                    mDoMtx_stack_c::ZXYrotM(shape_angle);
                    mDoMtx_stack_c::scaleM(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size); 
                    mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
                    mpModelMorf->modelCalc();

                    chkPartCreate(1);
                }
            }
        }
        return 1;
    }

    switch (arg0) {
    case 10:
        action_fire();
        break;
    case 20:
    case 21:
        action_parts();
        parts_mtx_set();
        break;
    default:
        action();
        mtx_set();
        cc_set();

        if (field_0x7d7 != 0) {
            down_cc_set();
        }
    }

    return 1;
}

static int daB_DR_Execute(daB_DR_c* i_this) {
    return i_this->execute();
}

static int daB_DR_IsDelete(daB_DR_c* i_this) {
    return 1;
}

int daB_DR_c::_delete() {
    dComIfG_resDelete(&mPhase, "B_DR");

    if (arg0 == 10) {
        return 1;
    }

    if (arg0 == 0x14 || arg0 == 0x15) {
        if (heap != NULL) {
            mSound.deleteObject();
            return 1;
        }
    }

    if (mInitHIO) {
        hio_set = 0;
    }

    if (heap != NULL) {
        mSound.deleteObject();
        mSound2.deleteObject();
    }

    return 1;
}

static int daB_DR_Delete(daB_DR_c* i_this) {
    return i_this->_delete();
}

int daB_DR_c::CreateHeap() {
    if (arg0 == 0x14 || arg0 == 0x15) {
        static int mPartDt[] = {74, 74, 75, 76, 77, 78};
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("B_DR", mPartDt[mPartNo]);
        JUT_ASSERT(6312, modelData != NULL);

        mpPartModel = mDoExt_J3DModel__create(modelData, 0, 0x11020203);
        if (mpPartModel == NULL) {
            return 0;
        }

        return 1;
    }

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("B_DR", 0x49);
    JUT_ASSERT(6324, modelData != NULL);
    
    mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("B_DR", 0x2F), 2, 1.0f, 0, -1, &mSound, 0, 0x11000084);
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    mpCoreBrk = new mDoExt_brkAnm();
    if (mpCoreBrk == NULL) {
        return 0;
    }

    J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("B_DR", 0x51);
    if (!mpCoreBrk->init(mpModelMorf->getModel()->getModelData(), pbrk, 1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    this->model = mpModelMorf->getModel();
    this->model->setUserArea((uintptr_t)this);

    for (u16 i = 0; i < this->model->getModelData()->getJointNum(); i++) {
        if (i != 0) {
            this->model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
        }
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return ((daB_DR_c*)i_this)->CreateHeap();
}

int daB_DR_c::create() {
    fopAcM_ct(this, daB_DR_c);

    int phase_state = dComIfG_resLoad(&mPhase, "B_DR");
    if (phase_state == cPhs_COMPLEATE_e) {
        u32 heap_size = 0x68E0;

        arg0 = fopAcM_GetParam(this);
        bitSw = (fopAcM_GetParam(this) & 0xFF00) >> 8;
        bitSw2 = (fopAcM_GetParam(this) & 0xFF0000) >> 0x10;
        bitSw3 = (fopAcM_GetParam(this) & 0xFF000000) >> 0x18;

        if (arg0 == 0xFF) {
            arg0 = 0;
        }

        switch (arg0) {
        case 20:
        case 21:
            mPartNo = bitSw;
            if (mPartNo > 5) {
                mPartNo = 0;
            }
            bitSw = 0xFF;
            OS_REPORT("パーツ %d\n", mPartNo);
        case 10:
            attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;

            fopAcM_SetGroup(this, 0);
            fopAcM_OffStatus(this, 0);

            gravity = 0.0f;
            shape_angle = current.angle;

            if (arg0 == 0xA) {
                mColliderStts.Init(0xFF, 0, this);
                mBreathAtCc.Set(cc_dr_breath_at_src);
                mBreathAtCc.SetStts(&mColliderStts);
                setActionMode(0xE, 0);
            } else {
                static u32 mParts_EstimateSizeDt[] = {0x1980, 0x1990, 0xD50, 0xD40, 0xD40, 0xD40};
                if (arg0 != 0xA && !fopAcM_entrySolidHeap(this, useHeapInit, mParts_EstimateSizeDt[mPartNo])) {
                    return cPhs_ERROR_e;
                }

                mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
                mAcchCir.SetWall(20.0f, 40.0f);
                mAcch.CrrPos(dComIfG_Bgsp());
                gravity = -3.0f;

                mSound.init(&current.pos, &eyePos, 3, 1);
                mColliderStts.Init(0x19, NULL, this);

                fopAcM_OnStatus(this, 0x4000);

                if (arg0 == 0x14) {
                    setActionMode(0xF, 0);
                } else {
                    setActionMode(0xF, 10);
                }   
            }
            break;
        default:
            if (dComIfGs_isStageBossEnemy()) {
                cXyz warp_pos(-325.0f, 0.0f, 215.0f);
                OS_REPORT("B_DR やられ後なので再セットしません\n");
                fopAcM_createWarpHole(&warp_pos, &shape_angle, fopAcM_GetRoomNo(this), 2, 0, 0xFF);

                if (!dComIfGs_isStageLife()) {
                    cXyz size(1.0f, 1.0f, 1.0f);
                    cXyz pos;
                    csXyz angle;

                    angle.x = angle.y = angle.z = 0;
                    pos.set(224.0f, 0.0f, 762.0f);

                    fopAcM_createItemForBoss(&pos, fpcNm_ITEM_UTAWA_HEART, fopAcM_GetRoomNo(this), &angle, &size, 0.0f, 0.0f, -1);
                }

                dScnKy_env_light_c* kankyo = dKy_getEnvlight();
                kankyo->wether = 0;
                return cPhs_ERROR_e;
            }

            if (!fopAcM_entrySolidHeap(this, useHeapInit, heap_size)) {
                return cPhs_ERROR_e;
            }

            OS_REPORT("B_DR PARAM %x\n", fopAcM_GetParam(this));
            OS_REPORT("B_DR arg0 %x\n", arg0);
            OS_REPORT("cDmr_SkipInfo %d\n", cDmr_SkipInfo);
            OS_REPORT("B_DR bitSw %d\n", bitSw);
            OS_REPORT("B_DR bitSw2 %d\n", bitSw2);
            OS_REPORT("B_DR bitSw3 %d\n\n", bitSw3);

            if (bitSw != 0xFF) {
                fopAcM_offSwitch(this, bitSw);
            }
            if (bitSw2 != 0xFF) {
                fopAcM_offSwitch(this, bitSw2);
            }
            if (bitSw3 != 0xFF) {
                fopAcM_offSwitch(this, bitSw3);
            }

            dComIfGs_offSaveDunSwitch(22);
            onWolfNoLock();

            health = 24;
            field_0x560 = 24;

            if (!hio_set) {
                hio_set = true;
                mInitHIO = true;
                l_HIO.field_0x4 = mDoHIO_CREATE_CHILD("翼竜", &l_HIO);
            }

            shape_angle.z = 0;
            shape_angle.x = 0;
            shape_angle.y = current.angle.y;

            attention_info.distances[fopAc_attn_BATTLE_e] = 85;
            attention_info.flags = fopAc_AttnFlag_BATTLE_e;

            fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
            fopAcM_SetMin(this, -20000.0f, -20000.0f, -20000.0f);
            fopAcM_SetMax(this, 20000.0f, 20000.0f, 20000.0f);

            mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
            mAcchCir.SetWall(0.0f, 300.0f);
            mAcch.CrrPos(dComIfG_Bgsp());

            mColliderStts.Init(0xFF, 0, this);
            mPillarCc.Set(cc_pillar_src);
            mPillarCc.SetStts(&mColliderStts);

            mWeekCc.Set(cc_dr_week_src);
            mWeekCc.SetStts(&mColliderStts);
            mWeekCc.OffTgSetBit();

            mWindAtCc.Set(cc_dr_wind_at_src);
            mWindAtCc.SetStts(&mColliderStts);

            mWindAtCc2.Set(cc_dr_wind_at_src);
            mWindAtCc2.SetStts(&mColliderStts);

            mWindAtCc3.Set(cc_dr_wind_at_src);
            mWindAtCc3.SetStts(&mColliderStts);

            mTailCc.Set(cc_dr_tail_src);
            mTailCc.SetStts(&mColliderStts);

            for (int i = 0; i < 5; i++) {
                mDownCc[i].Set(cc_down_src);
                mDownCc[i].SetStts(&mColliderStts);
            }

            for (int i = 0; i < 9; i++) {
                mDownSphCc[i].Set(cc_downSph_src);
                mDownSphCc[i].SetStts(&mColliderStts);
            }

            for (int i = 0; i < 2; i++) {
                mFootAtCc[i].Set(cc_dr_foot_at_src);
                mFootAtCc[i].SetStts(&mColliderStts);
            }

            field_0x784.set(current.pos);
            field_0x790.set(current.pos);
            mSound.init(&current.pos, &field_0x790, &eyePos, 3, 1, 1);
            mSound2.init(&field_0x784, &eyePos, 3, 1);

            field_0x7a8.set(current.pos);

            field_0x27a8 = &mSound;
            field_0x27be = 11;
            field_0x752 = 4096.0f + cM_rndFX(2048.0f);
            field_0x724 = 100.0f;

            current.pos.y += 1500.0f;
            setBck(ANM_DR_FLY, 2, 3.0f, 1.0f);
            setActionMode(ACTION_GLIDER_ATTACK, 0);

            mTargetHeight = 1500.0f + mAcch.GetGroundH();
            if (-G_CM3D_F_INF == mAcch.GetGroundH()) {
                mTargetHeight = 1500.0f + current.pos.y;
            }
            if (dComIfGs_isZoneSwitch(1, fopAcM_GetRoomNo(this))) {
                mTargetHeight = 6000.0f + home.pos.y;
            }

            if (WREG_S(1) != 0) {
                setActionMode(ACTION_WIND_ATTACK, 0);
            }

            if (dComIfGs_isZoneSwitch(2, fopAcM_GetRoomNo(this))) {
                field_0x724 = 100.0f;
                fopAcM_onSwitch(this, 16);
                arg0 = 1;
                field_0x7d1 = 2;
                mBreakPartsNo = 3;
                field_0x750 = JREG_S(8) + 200;
                setActionMode(ACTION_FLY_WAIT, 0);
                
                dScnKy_env_light_c* kankyo = dKy_getEnvlight();
                kankyo->wether = 2;
            }

            if (arg0 == 0) {
                mtx_set();
                if (cDmr_SkipInfo == 0 && !dComIfGs_isZoneSwitch(0, fopAcM_GetRoomNo(this))) {
                    arg0 = 0xFF;
                    field_0x7d5 = 0xB;
                    mTimer[0] = 2;

                    if (BREG_S(5) == 0) {
                        parentActorID = fopAcM_createChild(PROC_L7ODR, fopAcM_GetID(this), 0, &current.pos, fopAcM_GetRoomNo(this), &shape_angle, NULL, -1, NULL);
                        mStatusONOFF(0);
                    } else if (BREG_S(0) != 0) {
                        parentActorID = fopAcM_createChild(PROC_B_DRE, fopAcM_GetID(this), 0, &current.pos, fopAcM_GetRoomNo(this), &shape_angle, NULL, -1, NULL);
                    }
    
                    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
                    kankyo->wether = 0;
                }

                if (cDmr_SkipInfo != 0 || dComIfGs_isZoneSwitch(0, fopAcM_GetRoomNo(this)) ) {
                    Z2GetAudioMgr()->bgmStart(Z2BGM_DRAGON_BTL01, 0, 0);
                    field_0x7d5 = 0xC;
                    cDmr_SkipInfo = 0;

                    dComIfGs_onZoneSwitch(0, fopAcM_GetRoomNo(this));
                    dComIfGs_onZoneSwitch(20, fopAcM_GetRoomNo(this));
                    if (dComIfGs_isZoneSwitch(1, fopAcM_GetRoomNo(this))) {
                        field_0x7d1 = 1;
                        mBreakPartsNo++;
                    }
                }
            }
        }

        daB_DR_Execute(this);
    }

    return phase_state;
}

daB_DR_c::daB_DR_c() {}

static int daB_DR_Create(daB_DR_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daB_DR_Method = {
    (process_method_func)daB_DR_Create,
    (process_method_func)daB_DR_Delete,
    (process_method_func)daB_DR_Execute,
    (process_method_func)daB_DR_IsDelete,
    (process_method_func)daB_DR_Draw,
};

actor_process_profile_definition g_profile_B_DR = {
  fpcLy_CURRENT_e,        // mLayerID
  4,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_B_DR,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daB_DR_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  221,                    // mPriority
  &l_daB_DR_Method,       // sub_method
  0x04040000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
