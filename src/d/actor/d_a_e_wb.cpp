/**
 * @file d_a_e_wb.cpp
 *
 */
 
 #include "d/dolzel_rel.h" // IWYU pragma: keep
 #include "d/actor/d_a_e_rdb.h"
 #include "d/actor/d_a_obj_crvgate.h"
 #include "d/actor/d_a_obj_crvsteel.h"
 #include "d/d_msg_object.h"
#include "d/actor/d_a_e_wb.h"
#include "Z2AudioLib/Z2Instances.h"
#include "c/c_damagereaction.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_e_rd.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"


class daE_WB_HIO_c : public JORReflexible {
public:
    daE_WB_HIO_c();
    virtual ~daE_WB_HIO_c() {};
    void genMessage(JORMContext*);

    /* イノシシ - Wild Boar */
    /* 0x00 */ // vtable
    /* 0x04 */ s8 mId;
    /* 0x08 */ f32 base_size;                           // 基本サイズ - Base Size
    /* 0x0C */ f32 leader_size_ratio;                   // リーダーサイズ比 - Leader Size Ratio
    /* 0x10 */ f32 movement_speed;                      // 移動速度 - Movement Speed
    /* 0x14 */ f32 max_speed;                           // 最速度 - Max Speed
    /* 0x18 */ f32 cavalry_battle_max_speed;            // 騎馬戦最速 - Cavalry Battle Max Speed
    /* 0x1C */ f32 normal_speed_vi;
    /* 0x20 */ f32 medium_speed_vi;
    /* 0x24 */ f32 mMaxSpeedVi;
    /* 0x28 */ f32 leader_walking_speed;                // 歩き速（リ）- Leader Walking Speed
    /* 0x2C */ f32 leader_max_speed;                    // 最速度（リ）- Leader Max Speed
    /* 0x30 */ f32 leader_cavalry_battle_max_speed;     // 騎馬戦最（リ）- Leader Cavalry Battle Max Speed
    /* 0x34 */ f32 mSingleRiderSpeed;
    /* 0x38 */ f32 player_recognition_dist;             // PL認識距離 - Player Recognition Distance
    /* 0x3C */ f32 mPlayerMountedMaxSpeed;
    /* 0x40 */ f32 mPlayerMountedMotionPlaybackSpeed;
    /* 0x44 */ s16 mPlayerMountedDashTime;
    /* 0x48 */ f32 mSearchIgnoreDistance1;
    /* 0x4C */ f32 mSearchIgnoreDistance2;
    /* 0x50 */ f32 mGuidanceDisplayDistance1;
    /* 0x54 */ f32 mGuidanceDisplayDistance2;
    /* 0x58 */ u8 field_0x58;
    /* 0x59 */ u8 mNoReins;
};

STATIC_ASSERT(sizeof(daE_WB_HIO_c) == 0x5C);

static u8 const lit_3882[12] = {0};

daE_WB_HIO_c::daE_WB_HIO_c() {
    mId = -1;
    base_size = 1.2f;
    leader_size_ratio = 1.2f;
    movement_speed = 3.5f;
    player_recognition_dist = 600.0f;
    max_speed = 37.0f;
    cavalry_battle_max_speed = 50.0f;
    leader_walking_speed = 10.0f;
    leader_max_speed = 40.0f;
    leader_cavalry_battle_max_speed = 54.0f;
    normal_speed_vi = 25.0f;
    medium_speed_vi = 45.0f;
    mMaxSpeedVi = 55.0f;
    mSingleRiderSpeed = 50.0f;
    mPlayerMountedMaxSpeed = 30.0f;
    mPlayerMountedMotionPlaybackSpeed = 0.80000001f;
    mPlayerMountedDashTime = 300;
    mSearchIgnoreDistance1 = 500.0f;
    mSearchIgnoreDistance2 = 500.0f;
    mGuidanceDisplayDistance1 = 3500.0f;
    mGuidanceDisplayDistance2 = 3500.0f;
    field_0x58 = 0;
    mNoReins = 0;
}

#if DEBUG
void daE_WB_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("  イノシシ", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("基本サイズ", &base_size, 0.0f, 5.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("リーダーサイズ比", &leader_size_ratio, 0.0f, 5.0, 0, NULL, 0xFFFF, 0xFFFF, 512,
                   24);
    ctx->genSlider("移動速度", &movement_speed, 0.0f, 20.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("PL認識距離", &player_recognition_dist, 0.0f, 2000.0f, 0, NULL, 0xFFFF, 0xFFFF,
                   512, 24);
    ctx->genSlider("最速度", &max_speed, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("騎馬戦最速", &cavalry_battle_max_speed, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF,
                   512, 24);
    ctx->genSlider("歩き速（リ）", &leader_walking_speed, 0.0f, 30.0f, 0, NULL, 0xFFFF, 0xFFFF, 512,
                   24);
    ctx->genSlider("最速度（リ）", &leader_max_speed, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 512,
                   24);
    ctx->genSlider("騎馬戦最（リ）", &leader_cavalry_battle_max_speed, 0.0f, 100.0f, 0, NULL,
                   0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("通常速（車）", &normal_speed_vi, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 512,
                   24);
    ctx->genSlider("中速度（車）", &medium_speed_vi, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 512,
                   24);
    ctx->genSlider("最速度（車）", &mMaxSpeedVi, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("一騎速", &mSingleRiderSpeed, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genCheckBox("手綱ナシ", &mNoReins, 1, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genLabel("    プレイヤー騎乗時", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("最大速度", &mPlayerMountedMaxSpeed, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 512,
                   24);
    ctx->genSlider("モーション再生速度", &mPlayerMountedMotionPlaybackSpeed, 0.0f, 5.0f, 0, NULL,
                   0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("ダッシュ時間", &mPlayerMountedDashTime, 0.0f, 2000.0f, 0, NULL, 0xFFFF, 0xFFFF,
                   512, 24);
    ctx->genLabel("", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genLabel("    以下 一騎討ち用", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("サーチ無視距離(１)", &mSearchIgnoreDistance1, 0.0f, 5000.0f, 0, NULL, 0xFFFF,
                   0xFFFF, 512, 24);
    ctx->genSlider("サーチ無視距離(２)", &mSearchIgnoreDistance2, 0.0f, 5000.0f, 0, NULL, 0xFFFF,
                   0xFFFF, 512, 24);
    ctx->genSlider("案内表示距離(１)", &mGuidanceDisplayDistance1, 0.0f, 5000.0f, 0, NULL, 0xFFFF,
                   0xFFFF, 512, 24);
    ctx->genSlider("案内表示距離(２)", &mGuidanceDisplayDistance2, 0.0f, 5000.0f, 0, NULL, 0xFFFF,
                   0xFFFF, 512, 24);
}
#endif

static void anm_init(e_wb_class* i_actor, int i_anmId, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_actor->mAnmID = i_anmId;

    if (i_actor->field_0x79d != 0 && i_actor->field_0x79d >= 2 && i_anmId == 0x20) {
        i_anmId = 0x21;
    }

    i_actor->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_actor->mResName, i_anmId),
                                 i_attr, i_morf, i_rate, 0.0f, -1.0f);
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int joint_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_wb_class* bullbo_p = (e_wb_class*)model->getUserArea();

        if (bullbo_p) {
            MTXCopy(model->getAnmMtx(joint_no), *calc_mtx);

            if (joint_no == 0) {
                cMtx_ZrotM(*calc_mtx, bullbo_p->mBodyTiltAngle);
            } else if (joint_no == 2 || joint_no == bullbo_p->field_0x688 + 0xb) {
                cMtx_YrotM(*calc_mtx, bullbo_p->field_0x6d8);
            } else if (joint_no == 4 || joint_no == 8 ||
                       (joint_no == 0x13 && bullbo_p->field_0x79d == 0) ||
                       (joint_no == 0x17 && bullbo_p->field_0x79d == 0) ||
                       (joint_no == 0x16 && bullbo_p->field_0x79d != 0) ||
                       (joint_no == 0x1B && bullbo_p->field_0x79d != 0))
            {
                cMtx_YrotM(*calc_mtx, bullbo_p->mBodyTiltAngle << 1);
            }

            if (bullbo_p->field_0x79d && joint_no == 0xd) {
                cMtx_YrotM(*calc_mtx, bullbo_p->field_0x6e0);
                cMtx_XrotM(*calc_mtx, bullbo_p->field_0x6de);
                cMtx_ZrotM(*calc_mtx, bullbo_p->field_0x6e2);
            }

            model->setAnmMtx(joint_no, *calc_mtx);
            MTXCopy(*calc_mtx, j3dSys.mCurrentMtx);
        }
    }

    return 1;
}

/* 807E35B4 0001+00 data_807E35B4 @1009 */
/* 807E35B5 0001+00 data_807E35B5 None */
/* 807E35B6 0001+00 data_807E35B6 None */
/* 807E35B7 0001+00 data_807E35B7 None */
static s8 lbl_244_bss_45;
static s8 lbl_244_bss_46;
static s8 lbl_244_bss_47;
static bool l_HIOInit;

// static u8 lbl_244_bss_48;
// static u8 lbl_244_bss_49;
// static u8 lbl_244_bss_4a;
// static u8 lbl_244_bss_4b;

static daE_WB_HIO_c l_HIO;

static void himo_control1(e_wb_class* i_this, cXyz* i_pos, int i_idx, s8 param_3) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mEnemy;
    cXyz sp60;
    cXyz sp54;

    int i;
    s16 sp10;
    s16 spE;

    cXyz* var_r30 = i_this->field_0x1450[i_idx].field_0x0;
    var_r30[0] = *i_pos;

    cXyz sp48;
    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    cMtx_XrotM(*calc_mtx, a_this->shape_angle.x);

    sp60.x = 20.0f;
    if (i_idx == 0) {
        sp60.x *= -1.0f;
    }
    sp60.y = 0.0f;
    sp60.z = 0.0f;
    MtxPosition(&sp60, &sp48);

    cXyz sp3C;
    cXyz sp30;
    cXyz sp24;

    sp60.x = 0.0f;
    sp60.y = i_this->mAnimDuration;
    sp60.z = sp60.y;
    MtxPosition(&sp60, &sp30);

    if (a_this->speedF > 20.0f) {
        sp60.x = i_this->mAnimDuration * 0.4f;
    } else {
        sp60.x = i_this->mAnimDuration * 0.1f;
    }

    if (i_idx == 0) {
        sp60.x *= -1.0f;
    }
    sp60.y = 0.0f;
    sp60.z = 0.0f;
    MtxPosition(&sp60, &sp24);

    sp60.x = 0.0f;
    sp60.y = 0.0f;
    if (param_3 != 0) {
        sp60.z = 15.0f;
    } else {
        sp60.z = 20.0f;
    }

    if (i_this->field_0x79d != 0) {
        sp60.z *= l_HIO.leader_size_ratio;
        sp48 *= l_HIO.leader_size_ratio;
        sp30 *= l_HIO.leader_size_ratio;
        sp24 *= l_HIO.leader_size_ratio;
    }

    i = 1;
    cXyz* var_r30_2 = var_r30 + 1;
    for (; i < 16; i++, var_r30_2++) {
        sp3C.x = sp30.x * cM_ssin(i_this->field_0x1688 + i * 6000);
        sp3C.y = sp30.y * cM_ssin(i_this->field_0x1688 + i * 6000);
        sp3C.z = sp30.z * cM_ssin(i_this->field_0x1688 + i * 6000);

        f32 var_f29 = 1.0f;
        f32 var_f31 = sp24.x + (sp3C.x + ((var_r30_2[0].x - var_r30_2[-1].x) + (sp48.x * var_f29)));
        f32 var_f28 = sp3C.y + ((var_r30_2[0].y - var_r30_2[-1].y) - 20.0f);
        f32 var_f30 = sp24.z + (sp3C.z + ((var_r30_2[0].z - var_r30_2[-1].z) + (sp48.z * var_f29)));

        spE = (s16)cM_atan2s(var_f31, var_f30);
        sp10 = -cM_atan2s(var_f28, JMAFastSqrt(var_f31 * var_f31 + var_f30 * var_f30));

        cMtx_YrotS(*calc_mtx, spE);
        cMtx_XrotM(*calc_mtx, sp10);
        MtxPosition(&sp60, &sp54);

        var_r30_2[0].x = var_r30_2[-1].x + sp54.x;
        var_r30_2[0].y = var_r30_2[-1].y + sp54.y;
        var_r30_2[0].z = var_r30_2[-1].z + sp54.z;
    }
}

static void himo_control2(e_wb_class* i_this, cXyz* i_pos, int i_idx, s8 param_3) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp30;
    cXyz sp24;

    int i;
    s16 sp10;
    s16 spE;
    himo_s* sp1C = &i_this->field_0x1450[i_idx];
    cXyz* temp_r31 = &sp1C->field_0x0[15];
    *temp_r31 = *i_pos;

    sp30.x = 0.0f;
    sp30.y = 0.0f;

    if (param_3 != 0) {
        sp30.z = 15.0f + YREG_F(7);
    } else {
        sp30.z = 20.0f + YREG_F(6);
    }

    temp_r31--;

    for (i = 14; i >= 1; i--, temp_r31--) {
        f32 temp_f31 = temp_r31[0].x - temp_r31[1].x;
        f32 temp_f29 = temp_r31[0].y - temp_r31[1].y;
        f32 temp_f30 = temp_r31[0].z - temp_r31[1].z;
        spE = (s16)cM_atan2s(temp_f31, temp_f30);
        sp10 = -cM_atan2s(temp_f29, JMAFastSqrt((temp_f31 * temp_f31) + (temp_f30 * temp_f30)));

        cMtx_YrotS(*calc_mtx, spE);
        cMtx_XrotM(*calc_mtx, sp10);
        MtxPosition(&sp30, &sp24);

        temp_r31[0].x = temp_r31[1].x + sp24.x;
        temp_r31[0].y = temp_r31[1].y + sp24.y;
        temp_r31[0].z = temp_r31[1].z + sp24.z;
    }

    temp_r31 = sp1C->field_0x0;
    cXyz* var_r29 = i_this->field_0x15d0[i_idx].getPos(0);
    for (int i = 0; i < 16; i++, var_r29++, temp_r31++) {
        *var_r29 = *temp_r31;
    }

    var_r29 = i_this->field_0x1648.getPos(0);
    var_r29[i_idx] = i_this->field_0x1450[i_idx].field_0x0[15];
}

static int e_wb_lr_wall_check(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    dBgS_LinChk linChk;
    cXyz pos1;
    cXyz pos2;
    cXyz pos3;

    pos2 = a_this->current.pos;
    pos2.y += 300.0f;

    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    pos1.x = -400.0f;
    pos1.y = 300.0f;
    pos1.z = 400.0f;

    MtxPosition(&pos1, &pos3);
    pos3 += a_this->current.pos;
    linChk.Set(&pos2, &pos3, a_this);

    if (dComIfG_Bgsp().LineCross(&linChk)) {
        if (fopAcM_GetName(dComIfG_Bgsp().GetActorPointer(linChk)) == PROC_BG) {
            return -1;
        }
    }

    pos1.x *= -1.0f;
    MtxPosition(&pos1, &pos3);
    pos3 += a_this->current.pos;

    if (dComIfG_Bgsp().LineCross(&linChk)) {
        if (fopAcM_GetName(dComIfG_Bgsp().GetActorPointer(linChk)) == PROC_BG) {
            return 1;
        }
    }

    return 0;
}

static int daE_WB_Draw(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    static _GXColor l_color = {
        0x14,
        0x0F,
        0x00,
        0xFF,
    };
    f32 fVar9;
    int local_74;
    int local_78;

    if (i_this->field_0x1430 != 0 || i_this->mActionID == ACT_EVENT) {
        return 1;
    }

    cXyz local_3c;
    cXyz local_48;

    J3DModel* model = i_this->mpModelMorf->getModel();
    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);
    i_this->mpModelMorf->entryDL();

    cXyz cStack_54;
    cStack_54.set(a_this->current.pos.x, a_this->current.pos.y + 100.0f + BREG_F(0x12),
                  a_this->current.pos.z);

    if (i_this->field_0x79d != 0) {
        fVar9 = a_this->scale.z * 1000.0f;
    } else {
        fVar9 = a_this->scale.z * 800.0f;
    }

    fVar9 += BREG_F(0x13);

    i_this->mShadowKey =
        dComIfGd_setShadow(i_this->mShadowKey, 1, model, &cStack_54, fVar9, 0.0f,
                           a_this->current.pos.y, i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd,
                           &a_this->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    if (l_HIO.mNoReins == 0) {
        MTXCopy(model->getAnmMtx((i_this->field_0x688 + 0xb) + YREG_S(3)), *calc_mtx);
        MtxPush();

        s8 cVar7 = 0;
        e_rd_class* pfVar4 = (e_rd_class*)fopAcM_SearchByID(i_this->field_0x1434);

        if ((i_this->mStatusFlags & 1) != 0) {
            if (pfVar4 && pfVar4->field_0x9bc == 2) {
                if (i_this->field_0x7a2 == 0) {
                    model = pfVar4->mpModelMorf->getModel();
                    cVar7 = 1;
                }
            } else {
                if (daAlink_getAlinkActorClass()->checkBoarRideOwn(a_this)) {
                    if (!daAlink_getAlinkActorClass()->checkHorseNotGrab()) {
                        cVar7 = 1;
                    }
                }
            }
        }

        local_3c.x = YREG_F(0) + 80.0f;
        local_3c.y = YREG_F(1) - 20.0f;

        if (i_this->field_0x79d != 0) {
            local_3c.z = YREG_F(2) + 47.0f;
        } else {
            local_3c.z = YREG_F(0xc) + 42.0f;
        }

        MtxPosition(&local_3c, &local_48);
        himo_control1(i_this, &local_48, 0, cVar7);
        local_3c.z *= -1.0f;
        MtxPull();
        MtxPosition(&local_3c, &local_48);
        himo_control1(i_this, &local_48, 1, cVar7);

        if (cVar7 != 0) {
            cXyz local_60;

            if (daAlink_getAlinkActorClass()->checkBoarRideOwn(a_this)) {
                daAlink_getAlinkActorClass()->getHorseReinHandPos(&local_60, &local_48);
                i_this->field_0x1438[0] = local_48;
                i_this->field_0x1438[1] = local_60;
            } else {
                if (pfVar4->mBossMode != 0) {
                    local_78 = 0xf;
                    local_74 = 0x14;
                } else {
                    local_78 = 0x13;
                    local_74 = 0x18;
                }

                MTXCopy(model->getAnmMtx(local_74), *calc_mtx);
                local_3c.x = PREG_F(0xd) + 15.0f;
                local_3c.y = PREG_F(0xe);
                local_3c.z = PREG_F(0xf);
                MtxPosition(&local_3c, &local_48);

                MTXCopy(model->getAnmMtx(local_78), *calc_mtx);
                local_3c.x = PREG_F(0x10) + 15.0f;
                local_3c.y = PREG_F(0x11);
                local_3c.z = PREG_F(0x12);
                MtxPosition(&local_3c, &local_60);
            }

            if (i_this->field_0x168c > 100.0f) {
                i_this->field_0x1438[0] = local_48;
                i_this->field_0x1438[1] = local_60;

            } else {
                cLib_addCalc2(&i_this->field_0x1438[0].x, local_48.x, 1.0f, i_this->field_0x168c);
                cLib_addCalc2(&i_this->field_0x1438[0].y, local_48.y, 1.0f, i_this->field_0x168c);
                cLib_addCalc2(&i_this->field_0x1438[0].z, local_48.z, 1.0f, i_this->field_0x168c);
                cLib_addCalc2(&i_this->field_0x1438[1].x, local_60.x, 1.0f, i_this->field_0x168c);
                cLib_addCalc2(&i_this->field_0x1438[1].y, local_60.y, 1.0f, i_this->field_0x168c);
                cLib_addCalc2(&i_this->field_0x1438[1].z, local_60.z, 1.0f, i_this->field_0x168c);
                cLib_addCalc2(&i_this->field_0x168c, 1000.0f, 1.0f, 10.0f);
                local_3c = i_this->field_0x1438[0] - local_48;

                if (local_3c.abs() > 500.0f) {
                    i_this->field_0x1438[0] = local_48;
                    i_this->field_0x1438[1] = local_60;
                }
            }
        } else {
            i_this->field_0x168c = 0.0f;
            MTXCopy(model->getAnmMtx(i_this->field_0x688 + 0xe), *calc_mtx);
            local_3c.x = YREG_F(3) + 50.0f;
            local_3c.y = YREG_F(4) + 60.0f;
            local_3c.z = YREG_F(5);
            MtxPosition(&local_3c, &i_this->field_0x1438[0]);
            i_this->field_0x1438[1] = i_this->field_0x1438[0];
        }

        himo_control2(i_this, &i_this->field_0x1438[0], 0, cVar7);
        himo_control2(i_this, &i_this->field_0x1438[1], 1, cVar7);
        i_this->field_0x15d0[0].update(0x10, l_color, &a_this->tevStr);
        dComIfGd_set3DlineMat(&i_this->field_0x15d0[0]);
        i_this->field_0x15d0[1].update(0x10, l_color, &a_this->tevStr);
        dComIfGd_set3DlineMat(&i_this->field_0x15d0[1]);
        i_this->field_0x1648.update(2, l_color, &a_this->tevStr);
        dComIfGd_set3DlineMat(&i_this->field_0x1648);
    }

    return 1;
}

static s16 e_wb_wall_check(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    dBgS_LinChk linChk;
    cXyz local_a0;
    cXyz cStack_ac;
    cXyz cStack_b8;

    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    local_a0.x = 10.0f;
    local_a0.y = 120.0f;
    local_a0.z = 250.0f;
    MtxPosition(&local_a0, &cStack_b8);
    cStack_b8 += a_this->current.pos;
    cStack_ac = a_this->current.pos;
    cStack_ac.y += 200.0f;
    linChk.Set(&cStack_ac, &cStack_b8, a_this);
    if (dComIfG_Bgsp().LineCross(&linChk)) {
        fopAc_ac_c* uVar3 = dComIfG_Bgsp().GetActorPointer(linChk);
        if (fopAcM_GetName(uVar3) != PROC_BG) {
            return 0;
        }
        cXyz cStack_c4(linChk.GetCross());
        cXyz cStack_d0;
        local_a0.x *= -1.0f;
        MtxPosition(&local_a0, &cStack_b8);
        cStack_b8 += a_this->current.pos;
        linChk.Set(&cStack_ac, &cStack_b8, a_this);
        if (!dComIfG_Bgsp().LineCross(&linChk) == 0) {
            cStack_d0 = linChk.GetCross();
        } else {
            return 0;
        }
        local_a0 = cStack_d0 - cStack_c4;
        s16 sVar5 = (cM_atan2s(local_a0.x, local_a0.z) + 0x4000) - a_this->shape_angle.y;
        if (sVar5 == 0) {
            sVar5 = 1;
        }
        return sVar5;
    }

    return 0;
}

static s8 gake_check(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    dBgS_GndChk dStack_70;
    cXyz local_7c;
    cXyz cStack_88;

    static f32 chk_x[4] = {14257.0f, 34775.0f, -22864.0f, -11627.0f};
    static f32 chk_z[4] = {20075.0f, -16467.0f, 9823.0f, 22601.0f};

    if (lbl_244_bss_46 != 0) {
        if (!daAlink_getAlinkActorClass()->checkBoarRideOwn(a_this) &&
            a_this->current.pos.y < -5000.0f)
        {
            OS_REPORT(" WB NARAKU DOWN  2\n");
            fopAcM_delete(a_this);
            return 0;
        }

        for (int i = 0; i < 4; i++) {
            local_7c.x = nREG_F(3) + chk_x[i] - a_this->current.pos.x;
            local_7c.z = nREG_F(4) + chk_z[i] - a_this->current.pos.z;
            s16 sVar1 = cM_atan2s(local_7c.x, local_7c.z) - a_this->shape_angle.y;
            if (JMAFastSqrt(local_7c.x * local_7c.x + local_7c.z * local_7c.z) <
                nREG_F(8) + 2000.0f)
            {
                if (sVar1 < 0x4000 && sVar1 > -0x4000) {
                    return 2;
                }

                return -1;
            }
        }
    }

    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    local_7c.x = 500.0f;
    local_7c.y = 1000.0f;
    local_7c.z = JREG_F(0) + 1000.0f;
    MtxPosition(&local_7c, &cStack_88);
    cStack_88 += a_this->current.pos;
    dStack_70.SetPos(&cStack_88);
    f32 ground_cross = dComIfG_Bgsp().GroundCross(&dStack_70);

    if (a_this->current.pos.y - ground_cross > 2000.0f) {
        return 1;
    } else {
        local_7c.x = -500.0f;
        MtxPosition(&local_7c, &cStack_88);
        cStack_88 += a_this->current.pos;
        dStack_70.SetPos(&cStack_88);
        ground_cross = dComIfG_Bgsp().GroundCross(&dStack_70);

        if (a_this->current.pos.y - ground_cross > 2000.0f) {
            return 1;
        } else {
            local_7c.x = 0.0f;
            local_7c.y = 1000.0f;
            local_7c.z = JREG_F(1) + 2000.0f;
            MtxPosition(&local_7c, &cStack_88);
            cStack_88 += a_this->current.pos;
            dStack_70.SetPos(&cStack_88);
            ground_cross = dComIfG_Bgsp().GroundCross(&dStack_70);

            if (a_this->current.pos.y - ground_cross > 2000.0f) {
                return 1;
            } else {
                return 0;
            }
        }
    }
}

static BOOL e_wb_saku_jump_check(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    dBgS_LinChk dStack_98;
    cXyz local_a8;
    cXyz cStack_b4;
    cXyz cStack_c0;
    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    cMtx_XrotM(*calc_mtx, a_this->shape_angle.x);
    local_a8.x = 10.0f;
    local_a8.y = 150.0f;
    local_a8.z = KREG_F(8) + 500.0f;
    MtxPosition(&local_a8, &cStack_c0);
    cStack_c0 += a_this->current.pos;
    cStack_b4 = a_this->current.pos;
    cStack_b4.y += 100.0f;
    dStack_98.Set(&cStack_b4, &cStack_c0, a_this);

    if (dComIfG_Bgsp().LineCross(&dStack_98)) {
        cXyz cStack_cc(dStack_98.GetCross());
        cXyz cStack_d8;

        local_a8.x *= -1.0f;
        MtxPosition(&local_a8, &cStack_c0);
        cStack_c0 += a_this->current.pos;
        dStack_98.Set(&cStack_b4, &cStack_c0, a_this);

        if (dComIfG_Bgsp().LineCross(&dStack_98)) {
            cStack_d8 = dStack_98.GetCross();
        } else {
            return 0;
        }

        local_a8 = cStack_d8 - cStack_cc;
        s16 sVar1 = (cM_atan2s(local_a8.x, local_a8.z) + 0x4000) - a_this->shape_angle.y;

        if (sVar1 > -0x3000 && sVar1 < 0x3000) {
            return 1;
        }
    }

    return 0;
}

static int e_wb_saku_check_sub(e_wb_class* i_this, s16 param_1) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    dBgS_GndChk dStack_90;
    dBgS_LinChk dStack_100;
    cXyz local_10c;
    cXyz cStack_118;
    cXyz cStack_124;

    f32 iVar2 = (i_this->field_0x68e & 7) * 400.0f;
    f32 local_38 = 176.0f;

    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y + param_1);
    local_10c.x = 0.0f;
    local_10c.y = 2000.0f;
    local_10c.z = iVar2;
    MtxPosition(&local_10c, &cStack_118);
    cStack_118 += a_this->current.pos;
    dStack_90.SetPos(&cStack_118);
    cStack_118.y = dComIfG_Bgsp().GroundCross(&dStack_90) + 150.0f;
    local_10c.x = 0.0f;
    local_10c.y = 0.0f;
    local_10c.z = 410.0f;

    MtxPosition(&local_10c, &cStack_124);
    cStack_124 += cStack_118;
    dStack_100.Set(&cStack_118, &cStack_124, a_this);

    if (dComIfG_Bgsp().LineCross(&dStack_100)) {
        return 1;
    }

    return 0;
}

static int e_wb_saku_check(e_wb_class* i_this) {
    int uVar2 = false;
    static s16 yaa[3] = {0, -4096, 4096};
    static u32 saku_bit[3] = {1, 2, 4};

    for (int i = 0; i < 3; i++) {
        if (e_wb_saku_check_sub(i_this, yaa[i])) {
            uVar2 |= saku_bit[i];
        }
    }

    return uVar2;
}

static void e_wb_wait(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    switch (i_this->mActionMode) {
    case 0:
        anm_init(i_this, 0x25, 3.0f, 2, 1.0f);
        i_this->mActionMode = 1;
    case 1:
        if (a_this->speedF > 15.0f) {
            i_this->mMovementType = 2;
            i_this->mZ2Ride.startCreatureSoundLevel(Z2SE_EN_WB_L_SLIP, 0, -1);
        } else {
            anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
            i_this->mActionMode = 2;
            i_this->mStatusFlags |= 0x20;
        }
    case 2:
    default:
        cLib_addCalc0(&a_this->speedF, 1.0f, 1.0f);
    }
}

static void e_wb_ride(e_wb_class* i_this) {
    switch (i_this->mActionMode) {
    case 0:
        anm_init(i_this, 0x1f, 3.0f, 0, 1.0f);
        i_this->mActionMode = 1;
    case 1:
    default:
        return;
    }
}

static void e_wb_pl_ride_now(e_wb_class* i_this) {
    if (i_this->mAnmID != 0x2a) {
        anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
    }
}

static void e_wb_pl_ride(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    f32 fVar11;
    f32 fVar2;

    if (i_this->field_0x1432 != 0) {
        fVar11 = 50.0f;
        if (i_this->field_0x1432 == 1) {
            i_this->mActionID = ACT_PL_RIDE;
        }

        if (i_this->mAnmID == 0x1b) {
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 0x20, 2.0f, 2, 2.0f);
                a_this->speedF = 1.0;
            } else {
                if (!i_this->mpModelMorf->checkFrame(22.0f) &&
                    !i_this->mpModelMorf->checkFrame(42.0))
                {
                    return;
                }

                dComIfGp_getVibration().StartShock(5, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                return;
            }
        }

        if ((i_this->field_0x1432 & 7) == 0) {
            dComIfGp_getVibration().StartShock(3, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        }
    } else {
        fVar11 = l_HIO.mPlayerMountedMaxSpeed;
    }

    if (i_this->mAnmID == 0x25) {
        cLib_chaseF(&a_this->speedF, 0.0f, 2.0f);
        if (a_this->speedF < 0.01f) {
            anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
        }

        if (a_this->speedF > 10.0f) {
            i_this->mMovementType = 2;
        }
    } else {
        f32 stick_value = mDoCPd_c::getStickValue(0);
        if (daAlink_getAlinkActorClass()->checkHorseGetOffMode()) {
            stick_value = 0.0f;
        }

        f32 fVar1 = 1.5;
        s16 stick_angle;

        if (i_this->mActionID == 0x65) {
            fVar2 = stick_value;
            stick_angle = mDoCPd_c::getStickAngle3D(0);
        } else {
            fVar2 = 1.0;
            fVar1 = 10.0;

            if (dComIfGp_event_runCheck()) {
                stick_angle = -0x8000;
            } else {
                stick_angle = mDoCPd_c::getStickAngle3D(0);

                if (stick_value < 0.1f) {
                    stick_angle = -0x8000;
                }
            }
        }

        s16 angle_y = dCam_getControledAngleY(dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0))) +
                      0x10000 + stick_angle - 0x8000;
        int iVar4 = 0;
        s16 angle_delta = angle_y - a_this->current.angle.y;

        if (i_this->mActionID == 0x65) {
            if (abs(angle_delta) > 0x6000) {
                iVar4 = 1;
            } else if (angle_delta >= 0x2000) {
                iVar4 = 2;
            } else if (angle_delta <= -0x2000) {
                iVar4 = 3;
            }
        } else if (angle_delta >= 0x2000) {
            iVar4 = 2;
        } else if (angle_delta <= -0x2000) {
            iVar4 = 3;
        }

        if (i_this->mSpeedCapTimer == 2) {
            if (a_this->speedF < 0.1f) {
                if (iVar4 == 0) {
                    fVar2 = 0.0;
                }
            } else {
                iVar4 = 1;
            }

            if (i_this->mActionID != ACT_PL_RIDE) {
                i_this->mActionID = ACT_PL_RIDE;
                i_this->field_0x1432 = 0;

                if (a_this->speedF > l_HIO.mPlayerMountedMaxSpeed) {
                    a_this->speedF = l_HIO.mPlayerMountedMaxSpeed;
                }
            }
        } else if (i_this->mSpeedCapTimer < 0 && iVar4 == 1) {
            fVar2 = 0.0f;
        }

        // Bulblin Camp, Outside Arbiter's Grounds Room
        if (strcmp(dComIfGp_getStartStageName(), "F_SP118") == 0 && fopAcM_GetRoomNo(a_this) == 3 &&
                dComIfGp_event_runCheck() ||
            dComIfGp_checkPlayerStatus0(0, 0x2000))
        {
            fVar2 = 0.0f;
        }

        i_this->mWaitRollAngle = 0;
        if (iVar4 != 1 && fVar2 > 0.01f) {
            s16 curr_angle_y = a_this->current.angle.y;

            if (i_this->mActionID == ACT_PL_RIDE) {
                cLib_addCalcAngleS2(&a_this->current.angle.y, angle_y, 4, 500);
                i_this->mTurnStep = 0;
            } else {
                s16 local_94 = 0;
                s16 local_96 = KREG_S(3) + 0x40;

                if (iVar4 == 2) {
                    local_94 = KREG_S(2) + 0x300;
                } else if (iVar4 == 3) {
                    local_94 = -KREG_S(2) - 0x300;
                } else {
                    local_96 = KREG_S(4) + 0x10;
                }

                cLib_addCalcAngleS2(&i_this->mTurnStep, local_94, 4, local_96);
                a_this->current.angle.y += i_this->mTurnStep;
            }

            i_this->mWaitRollAngle = a_this->current.angle.y - curr_angle_y;
            s16 temAngle = i_this->mWaitRollAngle * (TREG_S(7) - 8);
            f32 fVar10 = temAngle * a_this->speedF / (l_HIO.mPlayerMountedMaxSpeed + 1.0f);

            if (fVar10 > NREG_F(0x11) + 4500.0f) {
                fVar10 = NREG_F(0x11) + 4500.0f;
            } else if (fVar10 < -NREG_F(0x11) - 4500.0f) {
                fVar10 = -NREG_F(0x11) - 4500.0f;
            }

            cLib_addCalcAngleS2(&i_this->mBodyTiltAngle, (s16)fVar10, 8, 0x140);
        }

        if (a_this->speedF < 0.01f) {
            if (fVar2 > 0.01f) {
                if (iVar4 == 0) {
                    i_this->mWaitRollAngle = 0;
                    a_this->speedF = fVar1;
                } else {
                    if (iVar4 == 1) {
                        i_this->mWaitRollAngle = 0;
                        cLib_chaseF(&a_this->speedF, fVar11 * fVar2 * -0.2f, 1.0f);
                    }

                    if (i_this->mAnmID != 0x28) {
                        anm_init(i_this, 0x28, 5.0f, 2, 1.5f);
                    }
                }
            } else {
                fVar11 = fabsf(a_this->speedF);

                if (fVar11 < 0.01f) {
                    if (i_this->mAnmID != 0x2a) {
                        anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
                    }
                } else {
                    cLib_chaseF(&a_this->speedF, 0.0f, 0.5f);
                }
            }
        } else {
            i_this->mWaitRollAngle = 0;

            if (fVar2 > 0.01f) {
                if (iVar4 == 1) {
                    if (a_this->speedF > fVar11 * 0.3f && i_this->mAnmID != 0x25) {
                        anm_init(i_this, 0x25, 3.0f, 2, 1.0f);
                    }

                    cLib_chaseF(&a_this->speedF, 0.0f, 2.0f);
                } else {
                    cLib_chaseF(&a_this->speedF, fVar11 * fVar2, fVar1);

                    if (a_this->speedF < TREG_F(3) + 20.0f) {
                        if (i_this->mAnmID != 0x2b) {
                            anm_init(i_this, 0x2b, 5.0f, 2, 1.0f);
                        }
                    } else if (i_this->mAnmID != 0x20) {
                        anm_init(i_this, 0x20, 5.0f, 2, 1.0f);
                    }
                }
            } else {
                cLib_chaseF(&a_this->speedF, 0.0f, 0.5);

                if (i_this->mAnmID == 0x20 && a_this->speedF < TREG_F(3) + 20.0f) {
                    anm_init(i_this, 0x2b, 5.0f, 2, 1.0f);
                }
            }
        }

        if (i_this->mAnmID == 0x20) {
            i_this->mpModelMorf->setPlaySpeed(
                BREG_F(0) + 0.4f +
                (l_HIO.mPlayerMountedMotionPlaybackSpeed * a_this->speedF) /
                    l_HIO.mPlayerMountedMaxSpeed);
            i_this->mMovementType = 1;
            i_this->mPursuitFlag = 1;
        } else if (i_this->mAnmID == 0x2b) {
            i_this->mpModelMorf->setPlaySpeed(TREG_F(4) + 0.7f +
                                              (TREG_F(5) + 0.05f) * a_this->speedF);
        }
    }
}

static void e_wb_f_wait(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    s16 new_angle = fopAcM_searchPlayerAngleY(a_this);

    switch (i_this->mActionMode) {
    case 0:
        if (lbl_244_bss_45 != 0 && (i_this->mParam2 == 1 || i_this->mParam2 == 2)) {
            i_this->mActionMode = 10;
            anm_init(i_this, 0x28, 5.0f, 2, 1.5f);

            if (cM_rndF(1.0) < 0.5f) {
                i_this->field_0x7a0 = 0x3000;
            } else {
                i_this->field_0x7a0 = -0x3000;
            }
            break;
        } else {
            anm_init(i_this, 0x28, 5.0f, 2, 1.5f);
            i_this->mActionMode = 1;
            i_this->mStatusFlags |= 4;
        }
        // fallthrough
    case 1:

        cLib_addCalcAngleS2(&a_this->current.angle.y, new_angle, 8, 0x200);
        i_this->field_0x6d0 = new_angle - a_this->current.angle.y;

        if (i_this->field_0x6d0 < 0x800 && i_this->field_0x6d0 > -0x800) {
            if (i_this->mSpeedCapTimer > 0) {
                if (i_this->mAnmID != 0x2a) {
                    anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
                }
            } else {
                fopAc_ac_c* player = dComIfGp_getPlayer(0);
                i_this->mActionID = ACT_C_F_RUN;
                i_this->mActionMode = 0;
                i_this->field_0x5d0 = player->current.pos;
            }
            break;
        } else if (i_this->mAnmID != 0x28) {
            anm_init(i_this, 0x28, 5.0f, 2, 1.0f);
        }
        break;

    case 10:
        new_angle += i_this->field_0x7a0;

        cLib_addCalcAngleS2(&a_this->current.angle.y, new_angle, 8, 0x200);
        i_this->field_0x6d0 = new_angle - a_this->current.angle.y;

        if (i_this->field_0x6d0 < 0x800 && i_this->field_0x6d0 > -0x800) {
            anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
            i_this->mActionMode = 11;
        }
        break;

    case 11:
        new_angle += i_this->field_0x7a0;
        i_this->field_0x6d0 = new_angle - a_this->current.angle.y;

        if (i_this->field_0x6d0 > 0x1000 || i_this->field_0x6d0 < -0x1000) {
            i_this->mActionMode = 10;
            anm_init(i_this, 0x28, 5.0f, 2, 1.5f);
        }

        break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 2.0f);

    if (i_this->mActionMode >= 10) {
        f32 dist = fopAcM_searchPlayerDistanceXZ(a_this);

        if (!(dist < 500.0f) && !(dist > 1500.0f)) {
            if (!daPy_getPlayerActorClass()->checkHorseRide()) {
                return;
            }

            if (!(dComIfGp_getHorseActor()->speedF >= 30.0f)) {
                return;
            }
        }

        if (i_this->mSpeedCapTimer == 0) {
            i_this->mActionID = ACT_C_F_RUN;
            i_this->mActionMode = 0;
        }
    }
}

static int pl_pass_check(e_wb_class* i_this, f32 param_1) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    cXyz pos_delta;
    cXyz cStack_24;

    pos_delta = a_this->current.pos - player->current.pos;

    cMtx_YrotS(*calc_mtx, -player->shape_angle.y);
    MtxPosition(&pos_delta, &cStack_24);

    return cStack_24.z > param_1;
}

static void e_wb_f_run(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* local_8c = dComIfGp_getPlayer(0);
    cXyz local_70;
    cXyz cStack_7c;
    f32 dVar13 = fopAcM_searchPlayerDistanceXZ(a_this);
    f32 dVar16 = 0.0f;
    f32 uVar15 = 1.0f;
    if ((i_this->mStatusFlags & 1) == 0) {
        i_this->mActionID = ACT_A_RUN;
        i_this->mActionMode = 0;
    } else {
        e_rd_class* local_90 = (e_rd_class*)fopAcM_SearchByID(i_this->field_0x1434);
        int local_94 = e_wb_saku_check(i_this);
        if (local_94 != 0) {
            i_this->field_0x6d4 = 60;
            if ((local_94 & 2) != 0) {
                i_this->field_0x6d6 = 0x3000;
            } else {
                i_this->field_0x6d6 = -0x3000;
            }
        }
        cLib_addCalcAngleS2(&i_this->field_0x6d6, 0, 1, 100);
        s8 bVar7 = false;
        if (daPy_getPlayerActorClass()->checkHorseRide() &&
            dComIfGp_getHorseActor()->speedF >= 30.0f)
        {
            bVar7 = true;
        }

        s16 sVar2 = a_this->current.angle.y;

        switch (i_this->mActionMode) {
        case 0:
            if (i_this->field_0x7a6 == 0) {
                anm_init(i_this, 0x1b, 3.0f, 0, 1.0f);
                i_this->mActionMode = 1;
                i_this->mAnimDuration = 40.0f;
                i_this->mStatusFlags |= 8;
            } else {
                i_this->field_0x7a6 = 0;
                anm_init(i_this, 0x20, 5.0f, 2, 1.0f);
                i_this->mpModelMorf->setFrame(cM_rndF(10.0f));
                i_this->mActionMode = 2;
                i_this->mStatusFlags |= 0x10;
                i_this->mZ2Ride.startCreatureSound(Z2SE_EN_WB_KICK_GROUND, 0, -1);
            }
            break;
        case 1:
            if (i_this->mpModelMorf->checkFrame(3.0f)) {
                i_this->mCollisionFlags |= 2;
                i_this->mMovementType = 10;
            } else if (i_this->mpModelMorf->checkFrame(15.0f)) {
                i_this->mCollisionFlags |= 3;
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 0x20, 5.0f, 2, 1.0f);
                i_this->mActionMode = 2;
                i_this->mStatusFlags |= 0x10;
                i_this->mZ2Ride.startCreatureSound(Z2SE_EN_WB_KICK_GROUND, 0, -1);
            }
            break;
        case 2:
            i_this->mPursuitFlag = 1;
            if (bVar7) {
                dVar16 = dComIfGp_getHorseActor()->speedF;
                if (dVar16 > l_HIO.cavalry_battle_max_speed) {
                    dVar16 = l_HIO.cavalry_battle_max_speed;
                } else if (dVar16 < l_HIO.max_speed) {
                    dVar16 = l_HIO.max_speed;
                }

                s16 local_a4 = 0x80;
                if (i_this->field_0x6c0 != 0) {
                    i_this->mTargetFacingAngle = local_8c->shape_angle.y;
                    i_this->field_0x5de = 0;
                    uVar15 = 0.5f;
                } else {
                    if (i_this->field_0x698[2] == 0) {
                        i_this->field_0x698[2] = cM_rndF(10.0f) + 3.0f;
                        local_70.x = i_this->field_0x6c4;
                        local_70.y = 0.0f;
                        local_70.z = 1500.0f;
                        cMtx_YrotS(*calc_mtx, local_8c->shape_angle.y);
                        MtxPosition(&local_70, &cStack_7c);
                        cStack_7c += local_8c->current.pos;
                        local_70 = cStack_7c - a_this->current.pos;
                        i_this->mTargetFacingAngle = cM_atan2s(local_70.x, local_70.z);
                    }

                    if (pl_pass_check(i_this, i_this->field_0x7a8)) {
                        dVar16 *= 0.95f;
                        uVar15 = 0.5f;
                    } else {
                        dVar16 *= 1.35f;
                        uVar15 = 0.5f;
                    }
                    local_a4 = 0x200;
                }

                if (local_90 != NULL && local_90->mAnmID == 0x27) {
                    i_this->field_0x6d6 = 5000.0f * cM_ssin(i_this->field_0x68e * 1000);
                    local_a4 = 0x400;
                }

                cLib_addCalcAngleS2(&a_this->current.angle.y,
                                    i_this->mTargetFacingAngle + i_this->field_0x5de +
                                        i_this->field_0x6d6,
                                    8, local_a4);
                cLib_addCalcAngleS2(&i_this->field_0x5de, 0, 1, 100);
                f32 fVar1 = a_this->speedF / 40.0f;
                if (fVar1 < 1.0f) {
                    fVar1 = 1.0f;
                }

                if (fVar1 > 1.3f) {
                    fVar1 = 1.3f;
                }
                i_this->mpModelMorf->setPlaySpeed(fVar1);
            } else {
                i_this->mpModelMorf->setPlaySpeed(1.0);
                uVar15 = 3.0f;
                dVar16 = l_HIO.max_speed;
                if (dVar13 > 700.0f) {
                    cLib_addCalcAngleS2(&a_this->current.angle.y,
                                        i_this->field_0x6d6 + fopAcM_searchPlayerAngleY(a_this), 8,
                                        0x200);
                }

                if (dVar13 < 400.0f) {
                    i_this->mActionMode = 3;
                    i_this->field_0x698[0] = 40;
                }
            }
            i_this->mMovementType = 1;
            break;
        case 3:
            i_this->mPursuitFlag = 1;
            dVar16 = l_HIO.max_speed;
            uVar15 = 3.0f;
            if (i_this->field_0x698[0] == 15) {
                i_this->mStatusFlags |= 0x20;
            }

            if (i_this->field_0x698[0] == 0) {
                i_this->mActionMode = 4;
                anm_init(i_this, 0x25, 3.0f, 2, 1.0f);
            }

            i_this->mMovementType = 1;
            break;
        case ACT_RIDE:
            if (a_this->speedF > 15.0f) {
                i_this->mMovementType = 2;
                i_this->mZ2Ride.startCreatureSoundLevel(Z2SE_EN_WB_L_SLIP, 0, -1);
            }

            if (a_this->speedF < 1.0f) {
                i_this->mActionID = ACT_F_WAIT;
                i_this->mActionMode = 0;
                return;
            }
            break;
        }

        cLib_addCalc2(&a_this->speedF, dVar16, 1.0f, uVar15);
        cLib_addCalcAngleS2(&i_this->mBodyTiltAngle, (-8) * (a_this->current.angle.y - sVar2), 8,
                            0x200);
        if (i_this->mActionMode < 4 && i_this->mSpeedCapTimer > 0) {
            i_this->mStatusFlags |= 0x20;
            anm_init(i_this, 0x25, 3.0f, 2, 1.0f);
            if (a_this->speedF > 30.0f) {
                a_this->speedF = 30.0f;
            }
            i_this->mActionMode = 4;
        }
    }
}

static void* s_wbstart_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_WB &&
        static_cast<e_wb_class*>(i_actor)->field_0x79d == 0)
    {
        static_cast<e_wb_class*>(i_actor)->field_0x7a6 = 1;
        static_cast<e_wb_class*>(i_actor)->field_0x7a7 = 1;
    }

    return NULL;
}

static void e_wb_b_wait(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    f32 dVar7 = fopAcM_searchPlayerDistanceXZ(a_this);
    s16 sVar5 = fopAcM_searchPlayerAngleY(a_this);
    if (i_this->field_0x79c != 0) {
        i_this->field_0x79c--;
        if (i_this->field_0x79c == 0) {
            fpcM_Search(s_wbstart_sub, a_this);
        }
    }

    switch (i_this->mActionMode) {
    case 0:
        anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
        i_this->mActionMode = 1;
        i_this->field_0x698[0] = cM_rndF(100.0f) + 100.0f;
        i_this->mStatusFlags |= 0x20;
        // fallthrough
    case 1:
        if (i_this->field_0x698[0] == 0 && i_this->mPathInitialized != 0) {
            i_this->mActionMode = 2;
        } else if (dVar7 < 2000.0f) {
            i_this->mActionID = ACT_B_RUN;
            i_this->mActionMode = 0;
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_RDB_V_HOICK, -1);
        }
        break;
    case 2:
        cLib_addCalcAngleS2(&a_this->current.angle.y, sVar5, 8, 0x200);
        i_this->field_0x6d0 = sVar5 - a_this->current.angle.y;
        if (i_this->field_0x6d0 < 0x800 && i_this->field_0x6d0 > -0x800) {
            if (i_this->mSpeedCapTimer > 0) {
                if (i_this->mAnmID != 0x2a) {
                    anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
                }
            } else {
                i_this->mActionID = ACT_B_WAIT2;
                i_this->mActionMode = 0;
            }
        } else if (i_this->mAnmID != 0x28) {
            anm_init(i_this, 0x28, 5.0f, 2, 1.0f);
        }
        break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 2.0f);
}

static cXyz b_path[8];

static cXyz b_path2[8];

static void e_wb_b_run2(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    cXyz pos1;
    cXyz pos2;
    f32 dist = fopAcM_searchPlayerDistanceXZ(a_this);
    f32 fVar10 = 0.0f;
    f32 fVar1 = 1.0f;
    s16 curr_angle = a_this->current.angle.y;

    switch (i_this->mActionMode) {
    case 0:
        anm_init(i_this, 0x1b, 3.0f, 0, 1.0f);
        i_this->mActionMode = 1;
        i_this->mStatusFlags |= 8;
        i_this->mAnimDuration = 40.0f;
        i_this->mPlaySpeed = 1.0f;
        break;
    case 1:
        if (i_this->mpModelMorf->checkFrame(3.0f) != 0) {
            i_this->mCollisionFlags |= 2;
            i_this->mMovementType = 10;
        } else {
            if (i_this->mpModelMorf->checkFrame(15.0f) != 0) {
                i_this->mCollisionFlags |= 3;
            }
        }

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, 0x20, 5.0f, 2, 1.0f);
            i_this->mActionMode = 2;
            cXyz loop_pos1;
            cXyz loop_pos2;
            int local_e0 = 0;

            for (int i = 1; i < 8; i++) {
                loop_pos1 = b_path2[local_e0] - a_this->current.pos;
                loop_pos2 = b_path2[i] - a_this->current.pos;

                if (loop_pos1.abs() > loop_pos2.abs()) {
                    local_e0 = i;
                }
            }

            i_this->mCurrentPathIndex = local_e0;
            i_this->field_0x698[0] = cM_rndF(100.0f) + 100.0f;
        }

        break;
    case 2:
        i_this->mPursuitFlag = 1;

        for (int i = 0; i < 8; i++) {
            cMtx_YrotS(*calc_mtx, i << 0xd);
            pos1.z = 3000.0f;
            MtxPosition(&pos1, &pos2);
            pos2 += player_p->current.pos;
            b_path2[i] = pos2;
        }

        pos1 = b_path2[i_this->mCurrentPathIndex] - a_this->current.pos;
        pos1.y = 0.0f;

        if (pos1.abs() < 600.0f) {
            int local_ec = 1;
            pos1 = b_path2[(i_this->mCurrentPathIndex + 1) & 7] - a_this->current.pos;

            s16 tmp = a_this->current.angle.y - cM_atan2s(pos1.x, pos1.z);

            if (tmp < 0x4000 && tmp > -0x4000) {
                i_this->mCurrentPathIndex += local_ec;
            } else {
                i_this->mCurrentPathIndex -= local_ec;
            }

            i_this->mCurrentPathIndex &= 7;
            i_this->mTurnStep = 0;
        }

        pos1 = b_path2[i_this->mCurrentPathIndex] - a_this->current.pos;
        i_this->mTargetFacingAngle = cM_atan2s(pos1.x, pos1.z);

        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetFacingAngle, 8,
                            i_this->mTurnStep);
        cLib_addCalcAngleS2(&i_this->mTurnStep, 0x400, 1, 0x10);

        fVar1 = 3.0f;
        fVar10 = l_HIO.leader_max_speed;
        i_this->mMovementType = 1;

        if (i_this->field_0x698[0] == 0 && dist < 5000.0f) {
            anm_init(i_this, 0x25, 3.0f, 2, 1.0f);
            i_this->mActionMode = 3;
        }
        break;
    case 3:
        if (a_this->speedF > 15.0f) {
            i_this->mMovementType = 2;
        }

        if (a_this->speedF < 1.0f) {
            i_this->mActionID = ACT_B_WAIT;
            i_this->mActionMode = 0;
            return;
        }
    }

    cLib_addCalc2(&a_this->speedF, fVar10, 1.0f, fVar1);
    cLib_addCalcAngleS2(&i_this->mBodyTiltAngle, -4 * (a_this->current.angle.y - curr_angle), 8,
                        0x200);
    i_this->mpModelMorf->setPlaySpeed(i_this->mPlaySpeed);
    cLib_addCalc2(&i_this->mPlaySpeed, 1.0f, 1.0f, 0.1f);

    if (dist < 1000.0f) {
        i_this->mActionID = ACT_B_RUN;

        if (i_this->mAnmID == 0x20) {
            i_this->mActionMode = 2;
        } else {
            i_this->mActionMode = 0;
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_RDB_V_HOICK, -1);
        }
    }

    if (i_this->mActionMode < 3 && i_this->mSpeedCapTimer > 0) {
        anm_init(i_this, 0x25, 3.0f, 2, 1.0f);

        if (a_this->speedF > 30.0f)
            a_this->speedF = 30.0f;

        i_this->mActionMode = 3;
    }
}

static void e_wb_b_wait2(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    cXyz cStack_38;
    cXyz cStack_44;
    f32 dVar7 = fopAcM_searchPlayerDistanceXZ(a_this);
    s16 sVar5 = fopAcM_searchPlayerAngleY(a_this);

    switch (i_this->mActionMode) {
    case 0:
        if (cM_rndF(1.0f) < 0.5f) {
            i_this->mTargetFacingAngle = sVar5 + 0x4000;
        } else {
            i_this->mTargetFacingAngle = sVar5 - 0x4000;
        }
        i_this->mActionMode = 1;
        // fallthrough
    case 1:
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetFacingAngle, 8, 0x200);
        i_this->field_0x6d0 = i_this->mTargetFacingAngle - a_this->current.angle.y;
        if (i_this->field_0x6d0 < 0x800 && i_this->field_0x6d0 > -0x800) {
            anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
            i_this->mActionMode = 2;
        } else if (i_this->mAnmID != 0x28) {
            anm_init(i_this, 0x28, 5.0f, 2, 1.0f);
        }
        break;
    case 2:
        if (dVar7 < 4000.0f) {
            i_this->mActionID = ACT_B_RUN;
            i_this->mActionMode = 0;
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_RDB_V_HOICK, -1);
        }
        break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 1.0f);
}

/**
 * @brief Handles the running action state for the Bullbo enemy.
 *
 * This function controls the Bullbo's behavior while running, including path initialization,
 * following a generated path, adjusting speed based on the player's horse, handling jumps over fences or walls,
 * and transitioning to other states like waiting or catching up.
 *
 * The path is an octagonal loop with randomized radii, regenerated when the Bullbo reaches a waypoint or periodically.
 * Speed and animation are adjusted dynamically to match or exceed the player's pace during chases.
 *
 * @param i_this Pointer to the Bullbo instance (e_wb_class).
 */
static void e_wb_b_run(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz target_vector;
    cXyz rotated_position;
    f32 player_xz_distance = fopAcM_searchPlayerDistanceXZ(a_this);
    f32 target_speed = 0.0;
    f32 acceleration = 1.0;

    if ((i_this->mStatusFlags & 1) == 0) {
        i_this->mActionID = ACT_A_RUN;
        i_this->mActionMode = 0;
    } else {
        s8 is_player_on_fast_horse = false;
        f32 cavalry_max_speed = l_HIO.leader_cavalry_battle_max_speed;

        if (daPy_getPlayerActorClass()->checkHorseRide() &&
            dComIfGp_getHorseActor()->speedF >= 30.0f)
        {
            is_player_on_fast_horse = true;
        }

        s16 initial_facing_angle = a_this->current.angle.y;
        f32 anim_speed_factor = 1.0f;
        s8 regenerate_path = false;

        switch (i_this->mActionMode) {
        case 0:
            i_this->mStatusFlags |= 8;
        case 1: // path initialization
            i_this->mAnimDuration = 30.0f;
            i_this->mPlaySpeed = 1.0f;
            anm_init(i_this, 0x20, 5.0f, 2, 1.0f);
            i_this->mActionMode = 2;
            i_this->mStatusFlags |= 0x10;
            i_this->mZ2Ride.startCreatureSound(Z2SE_EN_WB_KICK_GROUND, 0, -1);

            if (i_this->mPathInitialized != 0) {
                cXyz dist_to_point;
                cXyz dist_to_other;
                int closest_index = 0;

                for (int i = 1; i < 8; i++) {
                    dist_to_point = b_path[closest_index] - a_this->current.pos;
                    dist_to_other = b_path[i] - a_this->current.pos;

                    if (dist_to_point.abs() > dist_to_other.abs()) {
                        closest_index = i;
                    }
                }

                i_this->mCurrentPathIndex = closest_index;

            } else {
                if (lbl_244_bss_45 == 1) {
                    i_this->mCurrentPathIndex = 6;
                } else {
                    i_this->mCurrentPathIndex = 2;
                }

                i_this->mPathDirection = 1;
                i_this->mPathInitialized = 1;
            }
            break;
        case 2:  // path following
            i_this->mPursuitFlag = 1;
            target_vector = b_path[i_this->mCurrentPathIndex] - a_this->current.pos;
            target_vector.y = 0.0;

            if (target_vector.abs() < 800.0f) {
                if (i_this->mLapCount >= 4 && cM_rndF(1.0f) < 0.05f) {
                    i_this->mPathDirection = cM_rndF(6.0f) + 1.0f;
                }

                target_vector = b_path[(i_this->mCurrentPathIndex + 1) & 7] - a_this->current.pos;
                s16 angle_to_next =
                    a_this->current.angle.y - cM_atan2s(target_vector.x, target_vector.z);

                if (angle_to_next < 0x4000 && angle_to_next > -0x4000) {
                    i_this->mPathDirection = 1;
                } else {
                    i_this->mPathDirection = -1;
                }

                OS_REPORT(" PT   %d\n", i_this->mCurrentPathIndex);
                OS_REPORT(" DIR  %d\n", i_this->mPathDirection);

                i_this->mCurrentPathIndex += i_this->mPathDirection;
                i_this->mCurrentPathIndex &= 7;
                regenerate_path = true;
                i_this->mTurnStep = 0;
            } else {
                if (i_this->mPathAdjustCounter >= 3) {
                    i_this->mPathAdjustCounter = 0;

                    if (cM_rndF(1.0f) < 0.5f) {
                        i_this->mCurrentPathIndex += 3;
                    } else {
                        i_this->mCurrentPathIndex -= 3;
                    }

                    i_this->mCurrentPathIndex &= 7;
                    regenerate_path = true;
                }
            }

            target_vector = b_path[i_this->mCurrentPathIndex] - a_this->current.pos;
            i_this->mTargetFacingAngle = cM_atan2s(target_vector.x, target_vector.z);
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetFacingAngle, 8,
                                i_this->mTurnStep);
            cLib_addCalcAngleS2(&i_this->mTurnStep, 0x400, 1, 0x10);

            if (is_player_on_fast_horse) {
                target_speed = dComIfGp_getHorseActor()->speedF;

                if (target_speed > l_HIO.leader_cavalry_battle_max_speed) {
                    target_speed = l_HIO.leader_cavalry_battle_max_speed;
                } else {
                    if (target_speed < l_HIO.leader_max_speed) {
                        target_speed = l_HIO.leader_max_speed;
                    }
                }

                if (fopAcM_searchPlayerDistanceXZ(a_this) < 2000.0f) {
                    target_speed *= 1.2f;

                    if (a_this->speedF < l_HIO.max_speed) {
                        acceleration = 1.5f;
                    } else {
                        acceleration = 0.5f;
                    }
                }

                anim_speed_factor = a_this->speedF / 40.0f;

                if (anim_speed_factor < 1.0f)
                    anim_speed_factor = 1.0f;

                if (anim_speed_factor > 1.2f)
                    anim_speed_factor = 1.2f;
            } else {
                acceleration = 3.0;
                target_speed = l_HIO.leader_max_speed;
            }

            if (player_xz_distance > 6000.0f) {
                i_this->mActionMode = 3;
                i_this->field_0x698[0] = 13;
            }

            i_this->mMovementType = 1;
            break;
        case 3: 
            i_this->mPursuitFlag = 1;
            target_speed = l_HIO.max_speed;
            acceleration = 3.0f;

            if (i_this->field_0x698[0] == 10) {
                i_this->mStatusFlags |= 0x20;
            }

            if (i_this->field_0x698[0] == 0) {
                i_this->mActionMode = 4;
                anm_init(i_this, 0x25, 3.0f, 2, 1.0f);
            }

            i_this->mMovementType = 1;
            break;
        case 4: // decelerating back to wait action
            if (a_this->speedF > 15.0f) {
                i_this->mMovementType = 2;
            }

            if (a_this->speedF < 1.0f) {
                i_this->mActionID = ACT_B_WAIT2;
                i_this->mActionMode = 0;
            }

            break;
        case 10:  // jumping a fence / hit wall
            target_speed = l_HIO.max_speed;
            acceleration = 3.0f;
            int current_anim = i_this->mAnmID;

            if (current_anim == 0x18) {
                if (a_this->speed.y < 10.0f) {
                    anm_init(i_this, 0x19, 10.0f, 0, anim_speed_factor);
                }
            } else if (current_anim == 0x19) {
                if (a_this->speed.y < -30.0f || i_this->mAcch.ChkGroundHit()) {
                    anm_init(i_this, 0x1a, 2.0f, 0, 1.0f);
                    i_this->mCollisionFlags |= 0xc;
                    i_this->mLandingFlag = 1;
                    i_this->mStatusFlags |= 0x200;
                }
            } else if (current_anim == 0x1a) {
                i_this->mMovementType = 1;

                if (i_this->mpModelMorf->isStop()) {
                    anm_init(i_this, 0x20, 2.0f, 2, i_this->mPlaySpeed);
                    i_this->mAnimDuration = 30.0f;
                    i_this->mActionMode = 2;
                }
            }
        }

        if (i_this->mActionMode < 10) {
            cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, acceleration);
            cLib_addCalcAngleS2(&i_this->mBodyTiltAngle,
                                -4 * (a_this->current.angle.y - initial_facing_angle), 8, 0x200);
            i_this->mpModelMorf->setPlaySpeed(i_this->mPlaySpeed);
            cLib_addCalc2(&i_this->mPlaySpeed, anim_speed_factor, 1.0f, 0.1f);

            if (a_this->speedF >= 30.0f && i_this->mAcch.ChkGroundHit()) {
                if (i_this->mAcch.ChkWallHit() || e_wb_saku_jump_check(i_this)) {
                    i_this->mActionMode = 10;
                    a_this->speed.y = 55.0f;
                    anm_init(i_this, 0x18, 2.0f, 0, 1.0f);
                    i_this->mStatusFlags |= 0x100;
                }
            }
        }

        if (regenerate_path) {
            target_vector.x = 0.0f;
            target_vector.y = 1000.0f;

            for (int i = 0; i < 8; i++) {
                cMtx_YrotS(*calc_mtx, i << 0xd);

                if (lbl_244_bss_45 == 1) {
                    target_vector.z = cM_rndF(3000.0f) + 12000.0f;
                    MtxPosition(&target_vector, &rotated_position);
                } else if (i_this->mLapCount >= 4) {
                    target_vector.z = cM_rndF(1000.0f) + 9000.0f;
                    MtxPosition(&target_vector, &rotated_position);
                    rotated_position.x += 28108.0f;
                    rotated_position.z += 6640.0f;
                } else {
                    target_vector.z = cM_rndF(3000.0f) + 12000.0f;
                    MtxPosition(&target_vector, &rotated_position);
                    rotated_position.x += -2989.0f;
                    rotated_position.z += 1364.0f;
                }

                b_path[i] = rotated_position;
            }
        }

        if (i_this->mSpeedCapTimer > 0 && a_this->speedF > 30.0f) {
            a_this->speedF = 30.0f;
        }
    }
    return;
}

static void arrow_rd_set(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    static cXyz arrow_rd_pos[1] = {cXyz(36460.0f, 1040.0f, -17020.0f)};
    for (int i = 0; i < 1; i++) {
        fopAcM_create(PROC_E_RD, 0xff00a3ff, &arrow_rd_pos[i], fopAcM_GetRoomNo(a_this), 0, 0, -1);
    }
}

static cXyz saku_p(34800.0f, 0.0f, -14900.0f);

static cXyz saku_p2(34800.0f, 0.0f, -37200.0f);

static void e_wb_b_ikki(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    static cXyz ikki_pos[2] = {cXyz(34789.0f, -290.0f, -36200.0f),
                               cXyz(34789.0f, -290.0f, -16600.0f)};

    fopAc_ac_c* local_ac = dComIfGp_getPlayer(0);
    cXyz local_88;
    cXyz local_94;
    f32 dVar14 = fopAcM_searchPlayerDistanceXZ(a_this);
    s16 sVar11 = fopAcM_searchPlayerAngleY(a_this);
    sVar11 = sVar11 - a_this->current.angle.y;
    f32 dVar17 = 0.0;
    f32 dVar16 = 1.0;
    s16 local_d6 = 0x400;
    s16 local_d8 = 0;
    int local_b0 = i_this->mpModelMorf->getFrame();

    switch (i_this->mActionMode) {
    case -100:
        anm_init(i_this, 0x27, 5.0f, 2, 1.0f);
        i_this->mActionMode = -99;
        i_this->mStatusFlags |= 0x400;
        break;
    case -99:
        if (i_this->mpModelMorf->checkFrame(10.0f) || i_this->mpModelMorf->checkFrame(31.0f)) {
            i_this->mCollisionFlags |= 1;
            i_this->mMovementType = 10;
        }
        break;
    case -98:
        anm_init(i_this, 0x26, 10.0f, 0, 1.0f);
        i_this->mActionMode = 0xff9f;
        i_this->mStatusFlags |= 0x800;
        break;
    case -97:
        if (i_this->mpModelMorf->checkFrame(38.0f) || i_this->mpModelMorf->checkFrame(50.0f) ||
            i_this->mpModelMorf->checkFrame(65.0f) || i_this->mpModelMorf->checkFrame(108.0f))
        {
            i_this->mCollisionFlags |= 1;
            i_this->mMovementType = 10;
        } else if (i_this->mpModelMorf->checkFrame(82.0f)) {
            i_this->mCollisionFlags |= 3;
        } else if (i_this->mpModelMorf->checkFrame(106.0f)) {
            i_this->mCollisionFlags |= 2;
            i_this->mMovementType = 10;
        }

        if (local_b0 >= 0x70) {
            if (local_b0 == 0x70) {
                i_this->mStatusFlags |= 0x10;
                i_this->mZ2Ride.startCreatureSound(Z2SE_EN_WB_KICK_GROUND, 0, -1);
                i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_RDB_V_HOICK, -1);
                i_this->mAnimDuration = 40.0f;
            }
            dVar17 = l_HIO.mSingleRiderSpeed;
            dVar16 = l_HIO.mSingleRiderSpeed * 0.2f;
            i_this->mMovementType = 1;
        }

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, 0x20, 1.0f, 2, 1.0f);
            i_this->mActionMode = 4;
            i_this->field_0x1690 = 0;
            i_this->mCurrentPathIndex = 1;
            i_this->field_0x5c4 = ikki_pos[i_this->mCurrentPathIndex];
        }
        break;
    case 0:
        anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
        i_this->mActionMode = 1;
        i_this->field_0x698[3] = 10;
        break;
    case 1:
        if (i_this->field_0x698[3] == 0 && !dComIfGp_event_runCheck()) {
            i_this->mActionMode = 2;
            i_this->mCurrentPathIndex = 1;
        }
        break;
    case 2:
        if (!i_this->field_0x5bf) {
            anm_init(i_this, 0x1b, 3.0f, 0, 1.0f);
            i_this->mStatusFlags |= 8;
            i_this->mAnimDuration = 40.0f;
            i_this->field_0x5c4 = ikki_pos[i_this->mCurrentPathIndex];
            i_this->mActionMode = 3;
        }
        break;
    case 3:
        if (i_this->mpModelMorf->checkFrame(3.0f)) {
            i_this->mCollisionFlags |= 2;
            i_this->mMovementType = 10;
        } else if (i_this->mpModelMorf->checkFrame(15.0)) {
            i_this->mCollisionFlags |= 3;
        }

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, 0x20, 5.0f, 2, 1.0f);
            i_this->mActionMode = 4;
            i_this->mStatusFlags |= 0x10;
            i_this->mZ2Ride.startCreatureSound(Z2SE_EN_WB_KICK_GROUND, 0, -1);
            i_this->field_0x1690 = 0;
        }
        break;
    case 4: {
        dVar17 = l_HIO.mSingleRiderSpeed;
        dVar16 = 5.0f;
        i_this->mMovementType = 1;
        if (sVar11 < 0x3000 && sVar11 > -0x3000 && dVar14 < 4000.0f && dVar14 > 600.0f) {
            i_this->field_0x6dc = 13;
        } else if (sVar11 < 0x800 && sVar11 > -0x800) {
            i_this->field_0x6da = 1;
        } else if (sVar11 > 0) {
            i_this->field_0x6da = 3;
        } else {
            i_this->field_0x6da = 2;
        }
        f32 fVar1;
        f32 fVar2;
        if (i_this->mLapCount == 0) {
            fVar2 = l_HIO.mSearchIgnoreDistance2;
            fVar1 = l_HIO.mGuidanceDisplayDistance2;
        } else {
            fVar2 = l_HIO.mSearchIgnoreDistance1;
            fVar1 = l_HIO.mGuidanceDisplayDistance1;
        }
        f32 dVar15 = fVar1;

        if (sVar11 < 0x3000 && sVar11 > -0x3000) {
            if (dVar14 > fVar2) {
                cLib_addCalc2(&a_this->current.pos.x, local_ac->current.pos.x, 0.05f,
                              a_this->speedF * i_this->field_0x1690);
                cLib_addCalc2(&i_this->field_0x1690, 0.6f, 1.0f, 0.06f);
                cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
                local_88.x = a_this->current.pos.x - local_ac->current.pos.x;
                local_88.z = 0.0f;
                local_88.y = 0.0f;
                MtxPosition(&local_88, &local_94);
                local_d8 = local_94.x * -20.0f;
                if (local_d8 > 0x1000) {
                    local_d8 = 0x1000;
                } else if (local_d8 < -0x1000) {
                    local_d8 = -0x1000;
                }
            }
            if (dVar14 < dVar15) {
                daPy_getPlayerActorClass()->onSingleBoarAvoid();
            }
        }

        local_88 = i_this->field_0x5c4 - a_this->current.pos;
        i_this->mTargetFacingAngle = cM_atan2s(local_88.x, local_88.z);
        if (local_88.abs() < 600.0f) {
            anm_init(i_this, 0x25, 3.0f, 2, 1.0f);
            i_this->mActionMode = 5;
            i_this->mStatusFlags |= 0x20;
        }
        local_d6 = 0x800;
        break;
    }
    case 5:
        if (a_this->speedF > 15.0f) {
            i_this->mMovementType = 2;
            i_this->mZ2Ride.startCreatureSoundLevel(Z2SE_EN_WB_L_SLIP, 0, -1);
        } else if (a_this->speedF < 1.0f) {
            anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
            i_this->mActionMode = 6;
            i_this->mTargetFacingAngle -= 0x8000;
        }
        break;
    case 6:
        local_d6 = 0x200;
        i_this->field_0x6d0 = i_this->mTargetFacingAngle - a_this->current.angle.y;
        if (i_this->field_0x6d0 < 0x800 && i_this->field_0x6d0 > -0x800) {
            i_this->mActionMode = 2;
            i_this->mCurrentPathIndex = 1 - i_this->mCurrentPathIndex;
        } else if (i_this->mAnmID != 0x28) {
            anm_init(i_this, 0x28, 5.0f, 2, 1.5f);
        }
        break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetFacingAngle, 2, local_d6);
    cLib_addCalcAngleS2(&i_this->field_0x7a4, local_d8, 8, i_this->field_0x1690 + 10000.0f);
    cLib_addCalc2(&a_this->speedF, dVar17, 1.0f, dVar16);
    if (a_this->current.pos.x > 35350.0f) {
        a_this->current.pos.x = 35350.0f;
    } else if (a_this->current.pos.x < 34243.0f) {
        a_this->current.pos.x = 34243.0f;
    }

    if (i_this->mActionMode == 4 && (sVar11 > 0x4000 || sVar11 < -0x4000) && i_this->field_0x5bf) {
        if (i_this->mCurrentPathIndex == 1 && local_ac->current.pos.z < ikki_pos[0].z + 12000.0f &&
            a_this->current.pos.z > ikki_pos[1].z - 12000.0f)
        {
            a_this->current.pos.z = ikki_pos[1].z;
            i_this->mActionMode = 5;
            a_this->speedF = 0.0f;
            OS_REPORT("   WB CULL !!\n");
        } else {
            if (i_this->mCurrentPathIndex == 0 &&
                local_ac->current.pos.z > ikki_pos[1].z - 12000.0f &&
                a_this->current.pos.z < ikki_pos[0].z + 12000.0f)
            {
                a_this->current.pos.z = ikki_pos[0].z;
                i_this->mActionMode = 5;
                a_this->speedF = 0.0f;
                OS_REPORT("   WB CULL !!\n");
            }
        }
    }

    if (i_this->field_0x1722 != 0) {
        if (i_this->field_0x1722 == 1) {
            static cXyz f_pos[2] = {cXyz(34800.0f, -300.0f, -15150.0f),
                                    cXyz(34800.0f, -300.0f, -37200.0f)};
            csXyz cStack_a8;
            for (int i = 0; i < 2; i++) {
                static s16 f_ya[2] = {
                    0x8000,
                    0x0000,
                };

                cStack_a8.set(0, f_ya[i], 0);
                for (int j = 0; j < 2; j++) {
                    static u16 f_id[2] = {
                        dPa_RM(ID_ZI_S_UMASAKU_BURN_A),
                        dPa_RM(ID_ZI_S_UMASAKU_BURN_B),
                    };
                    dComIfGp_particle_set(f_id[j], &f_pos[i], 0, &cStack_a8, NULL);
                }
            }
            i_this->field_0x1722++;
        }
        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_STRAWFENCE_BURNING, &saku_p, 0, 0, 1.0f, 1.0f, -1.0f,
                                      -1.0f, 0);
        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_STRAWFENCE_BURNING, &saku_p2, 0, 0, 1.0f, 1.0f,
                                      -1.0f, -1.0f, 0);
    }
}

static void e_wb_b_ikki_end(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    f32 target = 0.0;
    f32 maxStep = 1.0f;

    switch (i_this->mActionMode) {
    case 0:
        anm_init(i_this, 0x20, 1.0f, 2, 1.0f);
        i_this->mActionMode = 1;
        break;
    case 1:
        target = l_HIO.mSingleRiderSpeed;
        maxStep = 5.0f;
        break;
    }

    cLib_addCalc2(&a_this->speedF, target, 1.0f, maxStep);
    if (a_this->current.pos.x > 35350.0f) {
        a_this->current.pos.x = 35350.0f;
    } else if (a_this->current.pos.x < 34243.0f) {
        a_this->current.pos.x = 34243.0f;
    }
}

static void e_wb_b_ikki2(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    static cXyz ikki2_pos[2] = {cXyz(-93620.0f, -5750.0f, 48944.0f),
                                cXyz(-93620.0f, -5750.0f, 28423.0f)};

    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz local_78;
    cXyz local_84;
    f32 dist = fopAcM_searchPlayerDistanceXZ(a_this);
    s16 angle_diff = fopAcM_searchPlayerAngleY(a_this) - a_this->current.angle.y;
    f32 target_speed = 0.0f;
    f32 speed_step = 1.0f;
    s16 turn_speed = 0x400;
    s16 local_b8 = 0;
    int local_b6;

    int frame = i_this->mpModelMorf->getFrame();
    e_rd_class* rider = (e_rd_class*)fopAcM_SearchByID(i_this->field_0x1434);

    switch (i_this->mActionMode) {
    case -100: {
        int local_a0 = dComIfGp_getEventManager().getMyStaffId("RiderBoss", NULL, 0);
        if (local_a0 != -1) {
            dComIfGp_getEventManager().cutEnd(local_a0);
            i_this->mActionMode = 0xff9d;
            anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
        }
        break;
    }
    case -99: {
        a_this->current.angle.y = 0x58f0;
        turn_speed = 0;
        local_b6 = 0;
        int local_a4 = dComIfGp_getEventManager().getMyStaffId("RiderBoss", NULL, 0);
        if (local_a4 != -1 && dComIfGp_evmng_getMyIntegerP(local_a4, "action") != 0) {
            rider->mMode = 2;
            i_this->mActionMode = -98;
        }
        break;
    }
    case -98:
        turn_speed = 0;
        break;
    case 0:
        anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
        i_this->mActionMode = 1;
        i_this->field_0x698[3] = 10;
        break;

    case 1:
        if (i_this->field_0x698[3] == 0 && !dComIfGp_event_runCheck()) {
            i_this->mActionMode = 2;
            i_this->mCurrentPathIndex = 1;
            Z2GetAudioMgr()->subBgmStart(Z2BGM_FACE_OFF_BATTLE);
        }
        break;

    case 2:
        if (!i_this->field_0x5bf) {
            anm_init(i_this, 0x1b, 3.0f, 0, 1.0f);
            i_this->mStatusFlags |= 8;
            i_this->mAnimDuration = 40.0f;
            i_this->field_0x5c4 = ikki2_pos[i_this->mCurrentPathIndex];
            i_this->mActionMode = 3;
        }
        break;

    case 3:
        if (frame < 15) {
            turn_speed = 0;
        }
        if (i_this->mpModelMorf->checkFrame(3.0f)) {
            i_this->mCollisionFlags |= 2;
            i_this->mMovementType = 10;
        } else if (i_this->mpModelMorf->checkFrame(15.0f)) {
            i_this->mCollisionFlags |= 3;
        }

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, 0x20, 5.0f, 2, 1.0f);
            i_this->mActionMode = 4;
            i_this->mStatusFlags |= 0x10;
            i_this->mZ2Ride.startCreatureSound(Z2SE_EN_WB_KICK_GROUND, 0, -1);
            i_this->field_0x1690 = 0.0f;
        }
        break;

    case 4:
        target_speed = l_HIO.mSingleRiderSpeed;
        speed_step = 5.0f;
        i_this->mMovementType = 1;

        if (angle_diff < 0x3000 && angle_diff > -0x3000 && dist < 4000.0f && dist > 600.0f) {
            i_this->field_0x6dc = 13;
        } else if (angle_diff < 0x800 && angle_diff > -0x800) {
            i_this->field_0x6da = 1;
        } else if (angle_diff > 0) {
            i_this->field_0x6da = 3;
        } else {
            i_this->field_0x6da = 2;
        }

        f32 ignore_dist;
        f32 guidance_dist;
        if (i_this->mLapCount == 0) {
            ignore_dist = l_HIO.mSearchIgnoreDistance2;
            guidance_dist = l_HIO.mGuidanceDisplayDistance2;
        } else {
            ignore_dist = l_HIO.mSearchIgnoreDistance1;
            guidance_dist = l_HIO.mGuidanceDisplayDistance1;
        }

        if (i_this->field_0x698[3] == 0 && angle_diff < 0x3000 && angle_diff > -0x3000) {
            if (dist > ignore_dist) {
                cLib_addCalc2(&a_this->current.pos.x, player->current.pos.x, 0.05f,
                              a_this->speedF * i_this->field_0x1690);
                cLib_addCalc2(&i_this->field_0x1690, 0.6f, 1.0f, 0.06f);

                cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
                local_78.x = a_this->current.pos.x - player->current.pos.x;
                local_78.z = 0.0f;
                local_78.y = 0.0f;
                MtxPosition(&local_78, &local_84);

                local_b8 = (s16)(local_84.x * -20.0f);
                if (local_b8 > 0x1000) {
                    local_b8 = 0x1000;
                } else if (local_b8 < -0x1000) {
                    local_b8 = -0x1000;
                }
            }
        }

        if (dist < guidance_dist) {
            daPy_getPlayerActorClass()->onSingleBoarAvoid();
        }

        local_78 = i_this->field_0x5c4 - a_this->current.pos;
        i_this->mTargetFacingAngle = cM_atan2s(local_78.x, local_78.z);

        if (i_this->mLapCount >= 2) {
            i_this->mTargetFacingAngle += (s16)(2500.0f * cM_ssin(i_this->field_0x68e * 800));
        }

        if (local_78.abs() < 700.0f) {
            anm_init(i_this, 0x25, 3.0f, 2, 1.0f);
            i_this->mActionMode = 5;
            i_this->mStatusFlags |= 0x20;
        }

        turn_speed = 0x800;
        break;

    case 5:
        if (a_this->speedF > 15.0f) {
            i_this->mMovementType = 2;
            i_this->mZ2Ride.startCreatureSoundLevel(Z2SE_EN_WB_L_SLIP, 0, -1);
        } else if (a_this->speedF < 1.0f) {
            anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
            i_this->mActionMode = 6;
            i_this->mTargetFacingAngle -= 0x8000;
        }
        break;

    case 6:
        turn_speed = 0x200;
        i_this->field_0x6d0 = i_this->mTargetFacingAngle - a_this->current.angle.y;

        if (i_this->field_0x6d0 < 0x800 && i_this->field_0x6d0 > -0x800) {
            i_this->mActionMode = 2;
            i_this->mCurrentPathIndex = 1 - i_this->mCurrentPathIndex;
        } else if (i_this->mAnmID != 0x28) {
            anm_init(i_this, 0x28, 5.0f, 2, 1.5f);
        }
        break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetFacingAngle, 2, turn_speed);
    cLib_addCalcAngleS2(&i_this->field_0x7a4, local_b8, 8, i_this->field_0x1690 + 10000.0f);
    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, speed_step);

    // Handle position culling logic
    if (i_this->mActionMode == 4 && (angle_diff > 0x4000 || angle_diff < -0x4000) &&
        i_this->field_0x5bf)
    {
        if (i_this->mCurrentPathIndex == 1 && player->current.pos.z < ikki2_pos[0].z + 12000.0f &&
            a_this->current.pos.z > ikki2_pos[1].z - 12000.0f)
        {
            a_this->current.pos.z = ikki2_pos[1].z;
            i_this->mActionMode = 5;
            a_this->speedF = 0.0f;
            OS_REPORT("   WB CULL !!\n");
        } else if (i_this->mCurrentPathIndex == 0 &&
                   player->current.pos.z > ikki2_pos[1].z - 12000.0f &&
                   a_this->current.pos.z < ikki2_pos[0].z + 12000.0f)
        {
            a_this->current.pos.z = ikki2_pos[0].z;
            i_this->mActionMode = 5;
            a_this->speedF = 0.0f;
            OS_REPORT("   WB CULL !!\n");
        }
    }
}

static void e_wb_b_ikki2_end(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    switch (i_this->mActionMode) {
    case 0:
        a_this->current.pos.x = -93620.0f;
        anm_init(i_this, 0x20, 1.0f, 2, 1.0f);
        i_this->mActionMode = 1;
        i_this->field_0x68e = 0;
        // fallthrough
    case 1:
        if (i_this->field_0x68e > 170 && i_this->mAcch.ChkWallHit()) {
            i_this->mActionID = ACT_BG_DAMAGE;
            i_this->mActionMode = 0;
            i_this->mAnimDuration = 50.0f;
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_DAMAGE, -1);
            dComIfGp_getVibration().StartShock(8, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
            i_this->mLandingFlag = 1;
        }
        break;
    }

    a_this->speedF = l_HIO.mSingleRiderSpeed;

    if (i_this->field_0x68e == 180) {
        i_this->field_0x7ac.SetWall(100.0f, 300.0f + BREG_F(11));
    }

    s16 angle_offset;
    if (i_this->field_0x68e > 170) {
        angle_offset = -15000;
    } else {
        angle_offset = (s16)(3000.0f * cM_ssin(i_this->field_0x68e * 1000));
    }

    s16 curr_angle = a_this->current.angle.y;
    cLib_addCalcAngleS2(&a_this->current.angle.y, angle_offset + 0x8000, 2, 0x800);
    cLib_addCalcAngleS2(&i_this->mBodyTiltAngle, -13 * (a_this->current.angle.y - curr_angle), 8,
                        0x800);
}

static void e_wb_b_lv9_end(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz local_44;
    f32 target_speed = 0.0f;
    f32 speed_step = 0.5f;
    s16 curr_angle = a_this->current.angle.y;

    switch (i_this->mActionMode) {
    case 0:
        i_this->field_0x698[0] = 30;
        i_this->mActionMode = 1;
        anm_init(i_this, 0x2b, 0.0f, 2, 1.0f);
        a_this->current.pos.set(-7080.0f, 50.0f, -6634.0f);
        local_44.x = -10008.0f - a_this->current.pos.x;
        local_44.z = -2729.0f - a_this->current.pos.z;
        a_this->current.angle.y = cM_atan2s(local_44.x, local_44.z);
        a_this->shape_angle.y = a_this->current.angle.y;
        i_this->mTargetFacingAngle = a_this->current.angle.y;
        // fallthrough
    case 1:
        if (i_this->field_0x698[0] == 0) {
            i_this->mActionMode = 2;
            i_this->field_0x698[0] = NREG_S(0) + 65;
        }
        break;
    case 2:
        target_speed = NREG_F(12) + 15.0f;
        if (i_this->field_0x698[0] == 0) {
            anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
            i_this->mActionMode = 3;
            i_this->mStatusFlags |= 8;
            i_this->field_0x698[0] = 40;
        }
        break;
    case 3:
        if (i_this->field_0x698[0] == 1) {
            i_this->mStatusFlags |= 8;
        }
        break;
    case 4:
        anm_init(i_this, 0x1b, 3.0f, 0, 1.0f);
        i_this->mActionMode = 5;
        break;
    case 5:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, 0x20, 2.0f, 2, 1.0f);
            i_this->mActionMode = 6;
            i_this->field_0x698[0] = 80;
        }
        break;
    case 6:
        target_speed = NREG_F(13) + 40.0f;
        speed_step = 5.0f;
        if (i_this->field_0x698[0] < 30) {
            i_this->mMovementType = 1;
        }
        if (i_this->field_0x698[0] == 0) {
            local_44.x = -12682.0f - a_this->current.pos.x;
            local_44.z = -2701.0f - a_this->current.pos.z;
            i_this->mTargetFacingAngle = cM_atan2s(local_44.x, local_44.z);
        }
        break;
    }

    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, speed_step);
    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetFacingAngle, 4, 0x100);
    cLib_addCalcAngleS2(&i_this->mBodyTiltAngle,
                        (NREG_S(0) + -8) * (a_this->current.angle.y - curr_angle), 8, 0x200);
}

static void e_wb_a_run(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    s16 curr_angle_y = a_this->current.angle.y;
    s16 f5b4 = i_this->mActionMode;

    switch (f5b4) {
    case 0:
        anm_init(i_this, 0x20, 5.0f, 2, 1.399999976158142f);
        i_this->mActionMode = 1;
        i_this->mTargetFacingAngle = a_this->current.angle.y;
        i_this->field_0x698[1] = cM_rndF(30.0f) + 80.0f;
    case 1:
        i_this->mPursuitFlag = 1;
        i_this->mMovementType = 1;

        if (i_this->field_0x698[0] == 0) {
            i_this->field_0x698[0] = cM_rndF(30.0f) + 10.0f;
            i_this->mTargetFacingAngle += (s16)cM_rndFX(10000.0f);
        }

        if (i_this->field_0x698[1] == 1 || i_this->mSpeedCapTimer == 2) {
            i_this->mActionID = ACT_LR_DAMAGE;
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_DAMAGE, -1);
            i_this->mActionMode = 0;
        }
    default:
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetFacingAngle, 8, 0x800);
        cLib_addCalc2(&a_this->speedF, l_HIO.max_speed * 1.2f, 1.0f, 2.0f);
        cLib_addCalcAngleS2(&i_this->mBodyTiltAngle, (a_this->current.angle.y - curr_angle_y) * -8,
                            8, 0x200);
    }
}

static void e_wb_s_damage(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz cStack_1c;
    cXyz cStack_28;

    i_this->field_0x6a0 = 10;
    switch (i_this->mActionMode) {
    case 0:
        if (a_this->speedF <= 5.0f) {
            if ((i_this->mStatusFlags & 3) != 0) {
                anm_init(i_this, 9, 2.0f, 0, 1.0f);
            } else {
                anm_init(i_this, 8, 2.0f, 0, 1.0f);
                i_this->mAnimDuration = 50.0f;
                i_this->field_0x168a = 5000;
            }
        } else {
            anm_init(i_this, 0x22, 2.0f, 0, 1.0f);
        }
        i_this->mActionMode = 1;
        break;
    case 1:
        if (i_this->mpModelMorf->isStop()) {
            if ((i_this->mStatusFlags & 3) != 0) {
                i_this->mActionID = i_this->field_0x692;
                if (i_this->mActionID == ACT_WAIT2) {
                    i_this->mActionID = ACT_C_F_RUN;
                } else if (i_this->mActionID == ACT_C_F_RUN) {
                    i_this->mActionMode = 1;
                } else {
                    if (i_this->mActionID == ACT_PL_RIDE) {
                        i_this->mActionID = ACT_PL_RIDE2;
                        i_this->field_0x1432 = l_HIO.mPlayerMountedDashTime;
                        a_this->speedF = 0.0f;
                        anm_init(i_this, 0x1b, 3.0f, 0, 1.0f);
                        i_this->mAnimDuration = 40.0f;
                        OS_REPORT(" RIDE RUN START \n");
                        return;
                    }
                    i_this->mActionMode = 0;
                }
            } else {
                i_this->mActionID = ACT_A_RUN;
                i_this->mActionMode = 0;
            }
        }
        break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 1.0f);
}

static int e_wb_damage(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz local_2c;
    cXyz local_38;

    i_this->field_0x6a0 = 10;
    BOOL local_48 = FALSE;

    switch (i_this->mActionMode) {
    case 0:
        dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(a_this), 5);
        anm_init(i_this, 8, 1.0f, 0, 1.0f);
        i_this->mActionMode = 1;
        a_this->speedF = -15.0f;
        a_this->speed.y = 50.0f + cM_rndF(20.0f);
        a_this->current.angle.y += (s16)cM_rndFX(3000.0f);
        i_this->mStatusFlags |= 0x40;
        break;
        
    case 1:
        if (i_this->mAcch.ChkGroundHit()) {
            i_this->mActionMode = 2;
            a_this->speedF *= 0.5f;
            a_this->speed.y = 40.0f;
            anm_init(i_this, 0x22, 2.0f, 0, 1.0f);
            i_this->field_0x6b6 = (s16)cM_rndFX(2000.0f);
        }
        break;
        
    case 2:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionMode = 3;
            if (cM_rndF(1.0f) < 0.5f) {
                anm_init(i_this, 0xc, 2.0f, 0, 1.0f);
                i_this->field_0x6e4 = 0;
            } else {
                anm_init(i_this, 0xd, 2.0f, 0, 1.0f);
                i_this->field_0x6e4 = 1;
            }
            i_this->field_0x698[0] = (s16)(cM_rndF(40.0f) + 70.0f);
        }
        break;
        
    case 3:
        local_48 = TRUE;
        if (i_this->mpModelMorf->isStop()) {
            if (i_this->mAnmID == 0xc) {
                anm_init(i_this, 0x12, 3.0f, 2, 1.0f);
            } else if (i_this->mAnmID == 0xd) {
                anm_init(i_this, 0x13, 3.0f, 2, 1.0f);
            }
        }
        
        if (i_this->mAcch.ChkGroundHit()) {
            a_this->speedF = 0.0f;
            cLib_addCalcAngleS2(&i_this->field_0x6b6, 0, 1, 0x96);
        }
        
        if (i_this->field_0x698[0] == 0 && i_this->field_0x6ae.x > -0x200) {
            if (i_this->field_0x6e4 == 0) {
                anm_init(i_this, 0x10, 2.0f, 0, 1.0f);
            } else {
                anm_init(i_this, 0x11, 2.0f, 0, 1.0f);
            }
            i_this->mActionMode = 4;
        }
        break;
        
    case 4:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionID = ACT_WAIT;
            i_this->mActionMode = 0;
            i_this->field_0x6ae.x = 0;
            a_this->current.angle.y += i_this->field_0x6ae.y;
            a_this->shape_angle.y = a_this->current.angle.y;
            i_this->field_0x6b6 = 0;
            i_this->field_0x6ae.y = 0;
        }
        break;
    }

    i_this->field_0x6ae.y += i_this->field_0x6b6;

    if (i_this->mActionMode >= 3) {
        cLib_addCalcAngleS2(&i_this->field_0x6ae.x, 0, 1, 0x600);
    } else {
        cLib_addCalcAngleS2(&i_this->field_0x6ae.x, -0x4000, 1, 0x200);
    }

    return local_48;
}

static int e_wb_bg_damage(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz local_30;
    BOOL return_value = FALSE;
    
    i_this->field_0x6a0 = 10;
    
    switch (i_this->mActionMode) {
    case 0:
        dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(a_this), 5);
        anm_init(i_this, 5, 2.0f, 0, 1.0f);
        i_this->mActionMode = 1;
        a_this->speedF = 0.0f;
        i_this->mStatusFlags |= 0x80;
        i_this->field_0x1432 = 0;
        break;
        
    case 1:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionMode = 2;
            anm_init(i_this, 7, 2.0f, 2, 1.0f);
            i_this->field_0x698[0] = (s16)(cM_rndF(60.0f) + 150.0f);
        }
        break;
        
    case 2:
        return_value = TRUE;
        if (i_this->field_0x698[0] == 0) {
            anm_init(i_this, 6, 3.0f, 0, 1.0f);
            i_this->mActionMode = 3;
        }
        break;
        
    case 3:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionID = ACT_WAIT;
            i_this->mActionMode = 0;
        }
        break;
    }
    
    return return_value;
}

static int e_wb_lr_damage(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz local_2c;
    cXyz local_38;
    BOOL return_value = FALSE;
    
    i_this->field_0x6a0 = 10;
    
    switch (i_this->mActionMode) {
    case 0:
        {
            f32 rnd = cM_rndF(1.0f);
            if (rnd < 0.5f || i_this->field_0x1720 != 0) {
                anm_init(i_this, 0xc, 2.0f, 0, 1.0f);
                i_this->field_0x6e4 = 0;
            } else {
                anm_init(i_this, 0xd, 2.0f, 0, 1.0f);
                i_this->field_0x6e4 = 1;
            }
            i_this->mActionMode = 1;
            i_this->mStatusFlags |= 0x40;
            i_this->field_0x698[0] = (s16)(cM_rndF(60.0f) + 150.0f);
            i_this->field_0x698[1] = 15;
            
            if (i_this->field_0x1720 != 0) {
                a_this->speedF = -50.0f;
                i_this->field_0x6b6 = -0x640;
            } else {
                i_this->field_0x6b6 = (s16)cM_rndFX(a_this->speedF * 50.0f);
            }
        }
        break;
        
    case 1:
        {
            int frame = i_this->mpModelMorf->getFrame();
            if (frame == 10) {
                i_this->mAnimDuration = 40.0f;
                i_this->field_0x168a = 4000;
                
                if (a_this->speedF > 30.0f) {
                    if (i_this->field_0x6e4 == 0) {
                        anm_init(i_this, 0xe, 2.0f, 2, 1.0f);
                    } else {
                        anm_init(i_this, 0xf, 2.0f, 2, 1.0f);
                    }
                    i_this->mActionMode = 2;
                    a_this->speedF = 30.0f;
                }
                i_this->mLandingFlag = 1;
                dKy_Sound_set(a_this->current.pos, 100, fopAcM_GetID(a_this), 5);
            }
            
            if (i_this->mpModelMorf->isStop()) {
                i_this->mActionMode = 3;
                if (i_this->mAnmID == 0xc) {
                    anm_init(i_this, 0x12, 3.0f, 2, 1.0f);
                } else if (i_this->mAnmID == 0xd) {
                    anm_init(i_this, 0x13, 3.0f, 2, 1.0f);
                }
            }
        }
        goto case3;
        
    case 2:
        if (a_this->speedF < 5.0f) {
            if (i_this->mAnmID == 14) {
                anm_init(i_this, 0x12, 3.0f, 2, 1.0f);
            } else if (i_this->mAnmID == 15) {
                anm_init(i_this, 0x13, 3.0f, 2, 1.0f);
            }
            i_this->mActionMode = 3;
        }
        // fallthrough
        
    case 3:
        case3:
        if (i_this->field_0x698[1] == 0) {
            return_value = TRUE;
        }
        
        if (a_this->speedF > 15.0f) {
            i_this->mMovementType = 2;
        }
        
        if (i_this->field_0x698[0] == 0 && i_this->field_0x1720 == 0) {
            if (i_this->field_0x6e4 == 0) {
                anm_init(i_this, 0x10, 3.0f, 0, 1.0f);
            } else {
                anm_init(i_this, 0x11, 3.0f, 0, 1.0f);
            }
            i_this->mActionMode = 4;
        }
        
        cLib_addCalcAngleS2(&i_this->field_0x6b6, 0, 1, 100);
        break;
        
    case 4:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionID = ACT_WAIT;
            i_this->mActionMode = 0;
            a_this->current.angle.y += i_this->field_0x6ae.y;
            a_this->shape_angle.y = a_this->current.angle.y;
            i_this->field_0x6b6 = 0;
            i_this->field_0x6ae.y = 0;
        }
        break;
    }
    
    cLib_addCalc0(&a_this->speedF, 1.0f, 1.5f);
    i_this->field_0x6ae.y += i_this->field_0x6b6;
    
    return return_value;
}

static void e_wb_kiba_start(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    
    switch (i_this->mActionMode) {
    case 0:
        anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
        i_this->field_0x169e = 0x1e;
        i_this->mActionMode = 1;
        i_this->field_0x7a2 = 1;
        break;
    case 1:
    default:
        break;
    }
    
    cLib_addCalc2(&a_this->speedF, 0.0f, 1.0f, 5.0f);
}

static void e_wb_kiba_end(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    
    i_this->field_0x6a0 = 10;
    
    switch (i_this->mActionMode) {
    case 0:
        if (i_this->mAnmID != 0x20) {
            anm_init(i_this, 0x20, 1.0f, 2, 1.0f);
        }
        i_this->mActionMode = 1;
        i_this->field_0x169e = 1;
        break;
        
    case 1:
        i_this->mMovementType = 1;
        i_this->mAnimDuration = 60.0f;
        i_this->field_0x168a = 5000;
        
        if (a_this->current.pos.x > 34000.0f && a_this->current.pos.x < 35500.0f &&
            a_this->current.pos.z < -14500.0f)
        {
            a_this->speed.y = 55.0f;
            anm_init(i_this, 0x18, 2.0f, 0, 1.0f);
            i_this->mActionMode = 2;
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_RDB_V_HOICK, -1);
            i_this->mStatusFlags |= 0x100;
        }
        break;
        
    case 2:
        if (a_this->speed.y < 10.0f) {
            anm_init(i_this, 0x19, 10.0f, 0, 1.0f);
            i_this->mActionMode = 3;
        }
        break;
        
    case 3:
        if (i_this->mAnmID == 0x19) {
            if (a_this->speed.y < -30.0f || i_this->mAcch.ChkGroundHit()) {
                anm_init(i_this, 0x1a, 2.0f, 0, 1.0f);
                i_this->mCollisionFlags |= 0xc;
                i_this->mLandingFlag = 1;
                i_this->mStatusFlags |= 0x200;
            }
        } else if (i_this->mAnmID == 0x1a && i_this->mpModelMorf->isStop()) {
            anm_init(i_this, 0x20, 2.0f, 2, 1.0f);
            i_this->mActionMode = 4;
        }
        break;
    case 4:
        break;
    }
    
    cLib_addCalc2(&a_this->speedF, 40.0f, 1.0f, 5.0f);
}

static void damage_check(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    
    a_this->health = 100;
    
    if (i_this->field_0x6ba != 0) {
        i_this->field_0x6ba--;
    }
    
    for (int i = 0; i <= 2; i++) {
        if (i_this->field_0xa00[i].ChkCoHit()) {
            cCcD_Obj* hit_obj = i_this->field_0xa00[i].GetCoHitObj();
            fopAc_ac_c* hit_actor = hit_obj->GetAc();
            
            if (!daAlink_getAlinkActorClass()->checkBoarRideOwn(a_this) && 
                i_this->field_0x6a0 == 0 && a_this->speedF < 1.0f &&
                fopAcM_GetName(hit_actor) == PROC_ALINK) {
                i_this->field_0x6ba += 2;
                if (i_this->field_0x6ba >= 150) {
                    i_this->field_0x692 = i_this->mActionID;
                    i_this->mActionID = ACT_S_DAMAGE;
                    i_this->mActionMode = 0;
                    i_this->field_0x6a0 = 20;
                    return;
                }
            }

            if ((lbl_244_bss_47 != 0 || lbl_244_bss_45 != 0) &&
                fopAcM_GetName(hit_actor) == PROC_E_WB && i_this->field_0x6a0 == 0)
            {
                e_wb_class* wb = (e_wb_class*)hit_actor;
                if (wb->field_0x6a0 == 0 && i_this->field_0x79d == 0 && wb->field_0x79d == 0) {
                    u8 temp = wb->mParam2;
                    wb->mParam2 = i_this->mParam2;
                    i_this->mParam2 = temp;
                    i_this->field_0x6a0 = 10;
                }
            }

            if (i_this->field_0x79d >= 2 && a_this->speedF >= 30.0f) {
                i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_RDB_V_RUNDOWN, -1);
                i_this->mZ2Ride.startCreatureSound(Z2SE_EN_BB_RUNDOWN, 0, -1);
                
                e_rd_class* rider = (e_rd_class*)fopAcM_SearchByID(i_this->field_0x1434);
                rider->field_0x998 = 20;
            } else {
                if (i_this->field_0x79d == 0 || i_this->mSpeedCapTimer == 0) {
                    fopAc_ac_c* hit_actor = i_this->field_0xa00[i].GetCoHitObj()->GetAc();
                    
                    if (a_this->speedF >= 20.0f) {
                        cCcD_Stts* hit_stts = i_this->field_0xa00[i].GetCoHitObj()->GetStts();
                        u8 weight = hit_stts->GetWeightUc();
                        
                        if (weight > 100 && 
                            (i_this->mActionID == ACT_C_F_RUN || i_this->mActionID == ACT_B_RUN) &&
                            hit_actor->speedF >= 20.0f) {
                            
                            cXyz pos_diff = hit_actor->current.pos - a_this->current.pos;
                            s16 angle = cM_atan2s(pos_diff.x, pos_diff.z) - a_this->shape_angle.y;
                            
                            if (angle < 0) {
                                i_this->field_0x5de = 0x1000;
                                a_this->current.angle.y += 0x800;
                            } else {
                                i_this->field_0x5de = -0x1000;
                                a_this->current.angle.y -= 0x800;
                            }
                        }
                    }
                }
            }
        }
    }
    
    i_this->mStts.Move();
    
    if (i_this->field_0x6a0 == 0) {
        dCcU_AtInfo at_info;
        
        for (int i = 0; i <= 6; i++) {
            if (i_this->field_0xa00[i].ChkTgHit()) {
                i_this->field_0x13c0.mpCollider = i_this->field_0xa00[i].GetTgHitObj();
                
                if (i_this->field_0x79d != 0 || (daPy_getPlayerActorClass()->checkHorseRide() &&
                        (i_this->mStatusFlags & 3) != 0)) {
                    if (i_this->field_0x79d != 0) {
                        def_se_set(&i_this->mZ2Ride, i_this->field_0x13c0.mpCollider, 0x28, NULL);
                    }
                } else {
                    at_power_check(&i_this->field_0x13c0);
                    cc_at_check(a_this, &i_this->field_0x13c0);
                    
                    if ((i_this->mStatusFlags & 3) != 0) {
                        if (i_this->field_0x13c0.mAttackPower < 30) {
                            i_this->field_0x692 = i_this->mActionID;
                            i_this->mActionID = ACT_S_DAMAGE;
                        } else {
                            i_this->mActionID = ACT_LR_DAMAGE;
                        }
                        i_this->mActionMode = 0;
                    } else {
                        i_this->mActionID = ACT_S_DAMAGE;
                        i_this->mActionMode = 0;
                    }
                }
                
                if (i_this->field_0x13c0.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                    i_this->field_0x6a0 = 20;
                } else {
                    i_this->field_0x6a0 = 10;
                }
                
                i_this->mAnimDuration = 30.0f;
                return;
            }
        }
    }
}

static void effect_set(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz pos;
    cXyz effect_pos;
    dBgS_GndChk gnd_chk;
    dBgS_ObjGndChk_Spl obj_gnd_chk;
    
    pos = a_this->current.pos;
    pos.y += 100.0f;
    gnd_chk.SetPos(&pos);
    obj_gnd_chk.SetPos(&pos);
    
    s8 is_water = false;
    s8 in_water = false;
    f32 ground_y = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (dComIfG_Bgsp().GroundCross(&obj_gnd_chk) - ground_y > 0.0f) {
        is_water = true;
    }
    
    if (i_this->mMovementType == 1) {
        if (i_this->mpModelMorf->checkFrame(11.0f)) {
            i_this->mCollisionFlags |= 4;
        } else if (i_this->mpModelMorf->checkFrame(13.0f)) {
            i_this->mCollisionFlags |= 8;
        }
    } else if (i_this->mMovementType == 2) {
        pos.set(0.0f, 0.0f, 0.0f);
        J3DModel* model = i_this->mpModelMorf->getModel();
        int foot_idx = (i_this->field_0x68e & 2) >> 1;
        
        static int footd[2] = {
            6,
            10,
        };

        static int footd_B[2] = {
            6,
            10,
        };

        if (i_this->field_0x79d != 0) {
            MTXCopy(model->getAnmMtx(footd_B[foot_idx]), *calc_mtx);
        } else {
            MTXCopy(model->getAnmMtx(footd[foot_idx]), *calc_mtx);
        }
        MtxPosition(&pos, &effect_pos);
        
        if (is_water) {
            in_water = true;
        } else if ((i_this->field_0x68e & 1) == 0) {
            fopAcM_effSmokeSet2(&i_this->field_0x13e8[foot_idx], 
                               &i_this->field_0x1408[foot_idx], &effect_pos, &a_this->shape_angle, a_this->scale.z * 2.0f, &a_this->tevStr);
        }
    }
    
    if (i_this->mCollisionFlags != 0) {
        J3DModel* model = i_this->mpModelMorf->getModel();
        pos.set(0.0f, 0.0f, 0.0f);
        
        if ((i_this->mCollisionFlags & 1) != 0) {
            MTXCopy(model->getAnmMtx(10), *calc_mtx);
            MtxPosition(&pos, &effect_pos);
            if (is_water) {
                in_water = true;
            } else {
                fopAcM_effSmokeSet2(i_this->field_0x13e8 + 1, 
                                   i_this->field_0x1408 + 1, &effect_pos, &a_this->shape_angle, a_this->scale.z * 2.0f, &a_this->tevStr);
                if (i_this->mMovementType != 0) {
                    i_this->field_0x13fc = dComIfGp_particle_set(i_this->field_0x13fc, dPa_RM(ID_ZI_S_WBCLOD_A), 
                                                                 &effect_pos, &a_this->shape_angle, 0);
                }
            }
        }
        
        if ((i_this->mCollisionFlags & 2) != 0) {
            MTXCopy(model->getAnmMtx(6), *calc_mtx);
            MtxPosition(&pos, &effect_pos);
            if (is_water) {
                in_water = true;
            } else {
                fopAcM_effSmokeSet2(i_this->field_0x13e8, 
                                   i_this->field_0x1408, &effect_pos, &a_this->shape_angle, a_this->scale.z * 2.0f, &a_this->tevStr);
                if (i_this->mMovementType != 0) {
                    i_this->field_0x13f8 = dComIfGp_particle_set(i_this->field_0x13f8, dPa_RM(ID_ZI_S_WBCLOD_A), 
                                                                 &effect_pos, &a_this->shape_angle, 0);
                }
            }
        }
        
        if ((i_this->mCollisionFlags & 4) != 0) {
            if (i_this->field_0x79d != 0) {
                MTXCopy(model->getAnmMtx(29), *calc_mtx);
            } else {
                MTXCopy(model->getAnmMtx(25), *calc_mtx);
            }
            MtxPosition(&pos, &effect_pos);
            if (is_water) {
                in_water = true;
            } else {
                fopAcM_effSmokeSet2(&i_this->field_0x13f0, 
                                   &i_this->field_0x1410, &effect_pos, &a_this->shape_angle, a_this->scale.z * 2.0f, &a_this->tevStr);
                if (i_this->mMovementType != 0) {
                    i_this->field_0x1400 = dComIfGp_particle_set(i_this->field_0x1400, dPa_RM(ID_ZI_S_WBCLOD_A), 
                                                                 &effect_pos, &a_this->shape_angle, 0);
                }
            }
        }
        
        if ((i_this->mCollisionFlags & 8) != 0) {
            if (i_this->field_0x79d != 0) {
                MTXCopy(model->getAnmMtx(24), *calc_mtx);
            } else {
                MTXCopy(model->getAnmMtx(21), *calc_mtx);
            }
            MtxPosition(&pos, &effect_pos);
            if (is_water) {
                in_water = true;
            } else {
                fopAcM_effSmokeSet2(&i_this->field_0x13f4, 
                                   &i_this->field_0x1414, &effect_pos, &a_this->shape_angle, a_this->scale.z * 2.0f, &a_this->tevStr);
                if (i_this->mMovementType != 0) {
                    i_this->field_0x1404 = dComIfGp_particle_set(i_this->field_0x1404, dPa_RM(ID_ZI_S_WBCLOD_A), 
                                                                 &effect_pos, &a_this->shape_angle, 0);
                }
            }
        }
    }
    
    if (i_this->mLandingFlag != 0) {
        if (is_water) {
            in_water = true;
        } else {
            pos.set(0.0f, 0.0f, 0.0f);
            J3DModel* model = i_this->mpModelMorf->getModel();
            MTXCopy(model->getAnmMtx(i_this->field_0x688 + 0xb), *calc_mtx);
            MtxPosition(&pos, &effect_pos);
            fopAcM_effSmokeSet1(&i_this->field_0x1418, 
                                   &i_this->field_0x141c, &effect_pos, NULL, a_this->scale.z * 2.0f, &a_this->tevStr, 1);
        }
    }
    
    if (in_water) {
        cXyz scale(2.0f, 2.0f, 2.0f);
        csXyz angle(0, 0, 0);

        static u16 w_eff_name[3] = {
            dPa_RM(ID_ZI_S_ENEMY_RUNWTRA_A),
            dPa_RM(ID_ZI_S_ENEMY_RUNWTRA_B),
            dPa_RM(ID_ZI_S_ENEMY_RUNWTRA_C),
        };

        static u16 w_eff_name2[3] = {
            dPa_RM(ID_ZI_S_ENEMY_DOWNWTRA_A),
            dPa_RM(ID_ZI_S_ENEMY_DOWNWTRA_B),
            dPa_RM(ID_ZI_S_ENEMY_DOWNWTRA_C),
        };
        
        for (int i = 0; i < 3; i++) {
            if (i_this->mMovementType == 2) {
                i_this->field_0x1420[i] = dComIfGp_particle_set(i_this->field_0x1420[i], 
                    w_eff_name2[i], &effect_pos, &a_this->tevStr, &angle, &scale, 0xff, 0, -1, 0, 0, 0);
            } else {
                i_this->field_0x1420[i] = dComIfGp_particle_set(i_this->field_0x1420[i], 
                    w_eff_name[i], &effect_pos, &a_this->tevStr, &angle, &scale, 0xff, 0, -1, 0, 0, 0);
            }
        }
    }
    
    if (is_water && i_this->field_0x1721 != 0) {
        static cXyz sc(4.0f, 4.0f, 4.0f);
        
        for (int i = 0; i < 4; i++) {
            static u16 w_eff_id[4] = {
                ID_ZI_J_DOWNWTRA_A,
                ID_ZI_J_DOWNWTRA_B,
                ID_ZI_J_DOWNWTRA_C,
                ID_ZI_J_DOWNWTRA_D,
            };

            i_this->field_0x17d0[i] = dComIfGp_particle_set(i_this->field_0x17d0[i], 
                w_eff_id[i], &a_this->current.pos, &a_this->tevStr, &a_this->shape_angle,
                &sc, 0xff, 0, -1, 0, 0, 0);
        }
        i_this->field_0x1721 = 0;
    }
    
    i_this->mMovementType = 0;
    i_this->mCollisionFlags = 0;
    i_this->mLandingFlag = 0;
}

static void wb_rd_reset(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz pos = a_this->current.pos;
    
    if (i_this->mParam2 == 1 || i_this->mParam2 == 2) {
        fopAcM_create(PROC_E_RD, 0xff010301, &pos, fopAcM_GetRoomNo(a_this), &player->shape_angle, 0, -1);
        fopAcM_create(PROC_E_RD, 0xff010302, &pos, fopAcM_GetRoomNo(a_this), &player->shape_angle, 0, -1);
    } else if (i_this->mParam2 >= 3) {
        fopAcM_create(PROC_E_RD, 0xff010101, &pos, fopAcM_GetRoomNo(a_this), &player->shape_angle, 0, -1);
        fopAcM_create(PROC_E_RD, 0xff010102, &pos, fopAcM_GetRoomNo(a_this), &player->shape_angle, 0, -1);
    }
}

static void wb_c_rd_reset(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    dComIfGp_getPlayer(0);
    u32 param = 0xff010300;
    if (i_this->field_0x17e1 != 0) {
        param = 0xff020300;
    }
    cXyz pos = a_this->current.pos;
    fopAcM_create(PROC_E_RD, param | 1, &pos, fopAcM_GetRoomNo(a_this), 0, 0, -1);
    fopAcM_create(PROC_E_RD, param | 2, &pos, fopAcM_GetRoomNo(a_this), 0, 0, -1);
}

static void* s_rddel_sub(void* param_0, void* param_1) {
    if (fopAcM_IsActor(param_0) && fopAcM_GetName(param_0) == PROC_E_RD) {
        e_rd_class* rd = (e_rd_class*)param_0;
        if (rd->mBossMode == 0) {
            fopAcM_delete((fopAc_ac_c*)param_0);
        }
    }
    return NULL;
}

static int rd_count;

static void* s_rdcount_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD) {
        e_rd_class* rd = (e_rd_class*)i_actor;
        if (rd->mBossMode == 0 && (rd->field_0x5b7 == 1 || rd->field_0x5b7 == 2)) {
            rd_count++;
        }
    }
    return NULL;
}

static void* s_rddel2_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD) {
        e_rd_class* rd = (e_rd_class*)i_actor;
        if (fopAcM_SearchByID(rd->mWbActorID) == i_data) {
            fopAcM_delete((fopAc_ac_c*)i_actor);
        }
    }
    return NULL;
}

static void e_wb_crv_wait(e_wb_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    fopAc_ac_c* king_bulblin = fopAcM_SearchByName(PROC_E_RDB);
    i_this->field_0x6a0 = 0x14;

    switch (i_this->mActionMode) {
    case 0:
        anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
        i_this->mActionMode = 1;
        // fallthrough
    case 1:
        if (player->current.pos.x > 482.0f && player->current.pos.x < 3100.0f &&
            player->current.pos.z > 0.0f && player->current.pos.z < 1500.0f)
        {
            i_this->field_0x169e = 0x46;
            i_this->mActionMode = 2;
        }
        break;
    case 2:
        if (i_this->field_0x169e == 0 && player->current.pos.z > 550.0f) {
            i_this->field_0x169e = 0x32;
            i_this->mActionMode = 3;
        }
        break;
    case 3:
        break;
    }
}

static s8 e_wb_c_run(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    fopAc_ac_c* coach = fopAcM_SearchByName(PROC_NPC_COACH);
    e_rd_class* rider = (e_rd_class*)fopAcM_SearchByID(i_this->field_0x1434);

    cXyz local_10c;
    cXyz cStack_118;
    f32 target_speed = 0.0f;

    int wall_check;
    s8 return_value;
    s8 far_from_point;
    far_from_point = false;
    return_value = false;
    f32 dist_x = -46137.0f - a_this->current.pos.x;
    f32 dist_z = 81549.0f - a_this->current.pos.z;
    f32 dist = JMAFastSqrt(dist_x * dist_x + dist_z * dist_z);

    if (dist < KREG_F(7) + 5000.0f) {
        far_from_point = true;
    } else {
        dist_x = -7650.0f - a_this->current.pos.x;
        dist_z = 56877.0f - a_this->current.pos.z;
        dist = JMAFastSqrt(dist_x * dist_x + dist_z * dist_z);

        if (dist < KREG_F(7) + 5000.0f) {
            far_from_point = true;
        }
    }

    wall_check = e_wb_lr_wall_check(i_this);

    if ((i_this->mStatusFlags & 1) == 0) {
        i_this->mActionID = ACT_A_RUN;
        i_this->mActionMode = 0;
        return false;
    }

    s16 sVar1 = a_this->current.angle.y;
    dBgS_LinChk lin_chk;

    switch (i_this->mActionMode) {
    case 0:
        anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
        i_this->mActionMode = 1;
        i_this->mStatusFlags |= 0x20;
        // fallthrough

    case 1: {
        cXyz target_pos(-62943.0f, -9045.0f, 70997.0f);
        local_10c = target_pos - a_this->current.pos;
        s16 target_angle = cM_atan2s(local_10c.x, local_10c.z);

        a_this->current.angle.y = target_angle;
        local_10c = target_pos - coach->current.pos;

        if (i_this->field_0x7a7 == 0 || JMAFastSqrt(local_10c.x * local_10c.x + local_10c.z * local_10c.z) < 500.0f)
        {
            i_this->mActionMode = 2;
            i_this->field_0x7a7 = 0;
            fopAcM_setStageLayer(i_this);
            i_this->field_0x17e1 = 1;
        }
        break;
    }
    case 2:
        if (i_this->field_0x7a6 == 0) {
            anm_init(i_this, 0x1b, 3.0f, 0, 1.0f);
            i_this->mActionMode = 0xb;
            i_this->mAnimDuration = 40.0f;
            i_this->mStatusFlags |= 8;
        } else {
            i_this->field_0x7a6 = 0;
            anm_init(i_this, 0x20, 5.0f, 2, 1.0f);
            f32 rnd_frame = cM_rndF(10.0f);
            i_this->mpModelMorf->setFrame(rnd_frame);
            i_this->mActionMode = 0xc;
            i_this->mStatusFlags |= 0x10;
            i_this->mZ2Ride.startCreatureSound(Z2SE_EN_WB_KICK_GROUND, 0, -1);
        }
        break;

    case 0xb:
        if (i_this->mpModelMorf->checkFrame(3.0f)) {
            i_this->mCollisionFlags |= 2;
            i_this->mMovementType = 10;
        } else if (i_this->mpModelMorf->checkFrame(15.0f)) {
            i_this->mCollisionFlags |= 3;
        }
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, 0x20, 5.0f, 2, 1.0f);
            i_this->mActionMode = 0xc;
            i_this->mStatusFlags |= 0x10;
            i_this->mZ2Ride.startCreatureSound(Z2SE_EN_WB_KICK_GROUND, 0, -1);
        }
        break;
    case 0xc:
        i_this->mActionMode = 0xd;
        // fallthrough
    case 0xd: {
        local_10c = a_this->current.pos;
        local_10c.y += 500.0f;
        cStack_118 = coach->eyePos;
        cStack_118.y += 500.0f;
        lin_chk.Set(&local_10c, &cStack_118, a_this);
        s8 behind_obstacle = false;
        cXyz diff_player;

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            cXyz diff_field = i_this->field_0x5d0 - a_this->current.pos;
            local_10c = player->eyePos - a_this->current.pos;
            if (diff_field.abs() < local_10c.abs()) {
                cStack_118 = i_this->field_0x5d0;
                behind_obstacle = true;
            } else {
                cMtx_YrotS(*calc_mtx, player->shape_angle.y);
                cStack_118 = player->eyePos;
            }
        } else {
            cMtx_YrotS(*calc_mtx, coach->shape_angle.y);
            cStack_118 = coach->current.pos;
            i_this->field_0x5d0 = cStack_118;
        }

        
        if (!behind_obstacle) {
            static cXyz sh_pos[3] = {
                cXyz(400.0f, 200.0f, 200.0f),
                cXyz(-400.0f, 200.0f, 300.0f),
                cXyz(0.0f, 200.0f, -700.0f),
            };

            static cXyz sh_posH[3] = {
                cXyz(150.0f, 200.0f, 200.0f),
                cXyz(-150.0f, 200.0f, 300.0f),
                cXyz(0.0f, 200.0f, -700.0f)
            };

            if (far_from_point || wall_check != 0) {
                local_10c = sh_posH[i_this->mParam2 - 1];
            } else {
                local_10c = sh_pos[i_this->mParam2 - 1];
                local_10c.x += i_this->field_0x6c4;
                local_10c.z += i_this->field_0x7a8;

                if ((i_this->field_0x68e & 0x3f) == 0 && cM_rndF(1.0f) < 0.5f) {
                    f32 rnd = cM_rndF(200.0f);
                    i_this->field_0x7a8 = -rnd;
                    rnd = cM_rndFX(100.0f);
                    i_this->field_0x6c4 = rnd;
                }
            }
        } else {
            local_10c.set(0.0f, 0.0f, 0.0f);
        }

        MtxPosition(&local_10c, &i_this->field_0x5c4);
        i_this->field_0x5c4 += cStack_118;

        local_10c = i_this->field_0x5c4 - a_this->current.pos;

        dist = JMAFastSqrt(local_10c.x * local_10c.x + local_10c.z * local_10c.z);
        
        if (dist < 500.0f) {
            target_speed = l_HIO.normal_speed_vi;
        } else if (dist > 3000.0f && i_this->field_0x5bf) {
            target_speed = l_HIO.mMaxSpeedVi;
        } else {
            target_speed = l_HIO.medium_speed_vi;
        }

        if ((i_this->mStatusFlags & 2) == 0) {
            target_speed = 20.0f;
            return_value = 1;
        }

        s16 turn_speed = 0x200;
        s16 calc_angle = cM_atan2s(local_10c.x, local_10c.z);
        i_this->mTargetFacingAngle = calc_angle;

        if (rider && rider->mAnmID == 0x27) {
            f32 sin_val = cM_ssin(i_this->field_0x68e * (BREG_S(7) + 1000));
            s16 add_angle = static_cast<s16>((BREG_F(16) + 5000.0f) * sin_val);
            i_this->mTargetFacingAngle += add_angle;
            turn_speed = 0x400;
        } else if (wall_check != 0) {
            i_this->mTargetFacingAngle += (s16)(wall_check * (BREG_S(8) + -8000));
        }

        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetFacingAngle, 8, turn_speed);
        f32 anim_speed = a_this->speedF / 40.0f;

        if (anim_speed < 1.0f) {
            anim_speed = 1.0f;
        }

        if (anim_speed > 1.3f) {
            anim_speed = 1.3f;
        }

        i_this->mpModelMorf->setPlaySpeed(anim_speed);
        i_this->mMovementType = 1;
        i_this->mPursuitFlag = 1;
        break;
    }
    case 0x14:
        anm_init(i_this, 0x25, 3.0f, 2, 1.0f);
        i_this->mActionMode = 0x15;
        // fallthrough

    case 0x15:
        if (a_this->speedF > 15.0f) {
            i_this->mMovementType = 2;
            i_this->mZ2Ride.startCreatureSoundLevel(Z2SE_EN_WB_L_SLIP, 0, -1);
        } else {
            anm_init(i_this, 0x2a, 10.0f, 2, 1.0f);
            i_this->mActionMode = 0x16;
            i_this->mStatusFlags |= 0x20;
        }
        break;
    }

    if (i_this->mActionMode < 0x14) {
        local_10c.x = 4025.0f - coach->current.pos.x;
        local_10c.z = 52319.0f - coach->current.pos.z;

        if (coach->speedF < 1.0f ||
            (dist = JMAFastSqrt(local_10c.x * local_10c.x + local_10c.z * local_10c.z),
                dist < 1500.0f))
        {
            local_10c = coach->current.pos - a_this->current.pos;
            dist = local_10c.abs();

            if (dist < 1000.0f) {
                i_this->mActionMode = 0x14;
            }
        }
    }

    cLib_addCalc2(&a_this->speedF, target_speed, 1.0f, 1.0f);
    cLib_addCalcAngleS2(&i_this->mBodyTiltAngle,
                        (TREG_S(7) + -8) * (a_this->current.angle.y - sVar1), 8, 0x200);

    return return_value;
}

// NONMATCHING - regalloc / conditional pathing
static void action(e_wb_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    cXyz pos1;
    cXyz pos2;
    cLib_addCalcAngleS2(&i_this->mBodyTiltAngle, 0, 8, 0x100);

    if (i_this->mActionID != 0x67) {
        damage_check(i_this);
    }

    s8 cVar8 = false;
    a_this->offDownFlg();

    if (lbl_244_bss_45 != 0 || lbl_244_bss_46 != 0) {
        i_this->mSpeedCapTimer = gake_check(i_this);
    }

    switch (i_this->mActionID) {
    case ACT_WAIT:
    case ACT_WAIT2:
        e_wb_wait(i_this);

        if (i_this->field_0x6a0 == 0) {
            cVar8 = true;
        }

        break;
    case ACT_RIDE:
        e_wb_ride(i_this);
        break;
    case ACT_F_WAIT:
        e_wb_f_wait(i_this);
        break;
    case ACT_C_F_RUN:
        if (i_this->mParam == 10) {
            cVar8 = e_wb_c_run(i_this);
            dComIfGp_getPlayer(0);  // unusued?

            if (a_this->home.pos.y - a_this->current.pos.y > 4500.0f) {
                OS_REPORT(" WB NARAKU DOWN  \n");
                fopAcM_delete(a_this);
            }
        } else {
            e_wb_f_run(i_this);
        }

        break;
    case ACT_A_RUN:
        e_wb_a_run(i_this);
        break;
    case ACT_B_WAIT:
        e_wb_b_wait(i_this);
        break;
    case ACT_B_WAIT2:
        e_wb_b_wait2(i_this);
        break;
    case ACT_B_RUN:
        e_wb_b_run(i_this);
        break;
    case ACT_B_RUN2:
        e_wb_b_run2(i_this);
        break;
    case ACT_B_IKKI:
        e_wb_b_ikki(i_this);
        break;
    case ACT_B_IKKI_END:
        e_wb_b_ikki_end(i_this);
        break;
    case ACT_B_IKKI2:
        e_wb_b_ikki2(i_this);
        break;
    case ACT_B_IKKI2_END:
        e_wb_b_ikki2_end(i_this);
        break;
    case ACT_B_LV9_END:
        e_wb_b_lv9_end(i_this);
        break;
    case ACT_S_DAMAGE:
        e_wb_s_damage(i_this);
        break;
    case ACT_DAMAGE:
        if (e_wb_damage(i_this)) {
            a_this->onDownFlg();
        }

        cVar8 = true;
        break;
    case ACT_BG_DAMAGE:
        if (e_wb_bg_damage(i_this)) {
            a_this->onDownFlg();
        }

        cVar8 = true;
        break;
    case ACT_LR_DAMAGE:
        if (e_wb_lr_damage(i_this)) {
            a_this->onDownFlg();
        }

        cVar8 = true;
        break;
    case ACT_KIBA_START:
        e_wb_kiba_start(i_this);
        break;
    case ACT_KIBA_END:
        e_wb_kiba_end(i_this);
        break;
    case ACT_EVENT:
        if (!dComIfGp_event_runCheck()) {
            cVar8 = true;
        }

        break;
    case ACT_PL_RIDE_NOW:
        e_wb_pl_ride_now(i_this);
        break;
    case ACT_PL_RIDE:
        e_wb_pl_ride(i_this);
        break;
    case ACT_PL_RIDE2:
        e_wb_pl_ride(i_this);
        break;
    case ACT_CRV_WAIT:
        e_wb_crv_wait(i_this);
    }

    if (a_this->speedF > 10.0f) {
        if (i_this->field_0x13e4 != 0xfd) {
            i_this->field_0x13e4 = 0xfd;
            i_this->mStts.Init(0xfd, 0, a_this);
        }
    } else if (i_this->field_0x13e4 != 0xdc) {
        i_this->field_0x13e4 = 0xdc;
        i_this->mStts.Init(0xdc, 0, a_this);
    }

    a_this->attention_info.flags &= ~fopAc_AttnFlag_CARRY_e;

    if (lbl_244_bss_45 != 0) {
        if (i_this->field_0x7a7 != 0 || (cVar8 && daPy_getPlayerActorClass()->checkHorseRide() && (dComIfGp_getHorseActor()->speedF >= 30.0f || i_this->field_0x7a6 != 0) && i_this->field_0x5bf != 0)) {
            fpcM_Search(s_rddel2_sub, i_this);
            rd_count = 0;
            fpcM_Search(s_rdcount_sub, i_this);

            if (rd_count <= 8) {
                i_this->field_0x7a7 = 0;
                i_this->mActionID = ACT_WAIT2;
                i_this->mActionMode = 0;
                i_this->field_0x6a0 = 0x1e;
                i_this->mStatusFlags = 3;
                i_this->field_0x6ae.set(0, 0, 0);
                wb_rd_reset(i_this);
                if (i_this->field_0x79d == 0) {
                    OS_REPORT("   WB RESET  \n");
                } else {
                    OS_REPORT("   BB RESET  \n");
                }
            }
        }
    }

    if (lbl_244_bss_47 != 0) {
        if (i_this->field_0x7a6 != 0 || (daPy_getPlayerActorClass()->checkHorseRide() &&
                dComIfGp_getHorseActor()->speedF >= 20.0f && cVar8 && i_this->field_0x5bf != 0)) {
            fpcM_Search(s_rddel2_sub, i_this);
            rd_count = 0;
            fpcM_Search(s_rdcount_sub, i_this);

            if (rd_count <= 4) {
                i_this->field_0x7a6 = 0;
                i_this->mActionID = ACT_WAIT2;
                i_this->mActionMode = 0;
                i_this->field_0x6a0 = 0x1e;
                i_this->mStatusFlags = 3;
                i_this->field_0x6ae.set(0, 0, 0);
                wb_c_rd_reset(i_this);
                i_this->field_0x68e = (s16)cM_rndF(65535.0f);
            }
        }
    }

    cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    pos1.x = 0.0f;
    pos1.y = 0.0f;
    pos1.z = a_this->speedF;

    MtxPosition(&pos1, &pos2);
    a_this->speed.x = pos2.x;
    a_this->speed.z = pos2.z;
    a_this->current.pos += a_this->speed;
    a_this->speed.y -= 5.0f;

    if (a_this->speed.y < -120.0f)
        a_this->speed.y = -120.0f;

    if (i_this->mActionID == ACT_WAIT && (i_this->mStatusFlags & 3) == 0 &&
        // Bulblin Camp
        (strcmp(dComIfGp_getStartStageName(), "F_SP118") == 0 ||
        // Gerudo Desert
        strcmp(dComIfGp_getStartStageName(), "F_SP124") == 0 || lbl_244_bss_46 != 0 ||
        // Hyrule Castle
        strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0))
    {
        s16 ang = cLib_targetAngleY(&a_this->attention_info.position,
                                    &dComIfGp_getPlayer(0)->current.pos) -
                  a_this->shape_angle.y;
        int target_angle = abs(ang);

        if (target_angle < 0x6000 && target_angle > 0x3000) {
            a_this->attention_info.flags |= fopAc_AttnFlag_ETC_e;
        } else {
            a_this->attention_info.flags &= ~fopAc_AttnFlag_ETC_e;
        }
    } else {
        a_this->attention_info.flags &= ~fopAc_AttnFlag_ETC_e;
    }

    s16 angle = 0;

    if (i_this->mAnmID == 0x28) {
        angle = (a_this->current.angle.y - a_this->old.angle.y) * 7 + VREG_S(2);
    }

    cLib_addCalcAngleS2(&i_this->field_0x6d8, angle, 8, 0x400);

    if (lbl_244_bss_45 != 0 && a_this->home.pos.y - a_this->current.pos.y > 5000.0f) {
        i_this->mActionID = ACT_EVENT;
        i_this->mActionMode = 0;
        i_this->mStatusFlags |= 0x40;
        a_this->current.pos = a_this->home.pos;
        a_this->old = a_this->current;
    }
}

static void* s_wbZrevise_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_WB &&
        static_cast<e_wb_class*>(i_actor)->field_0x79d == 0)
    {
        static_cast<fopEn_enemy_c*>(i_actor)->current.pos =
            static_cast<fopEn_enemy_c*>(i_actor)->home.pos;
    }

    return NULL;
}

static void* s_spd0_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_WB &&
        static_cast<e_wb_class*>(i_actor)->field_0x79d == 0)
    {
        static_cast<fopEn_enemy_c*>(i_actor)->speedF = 0.0f;
    }

    return NULL;
}

static void* s_wbdel_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_WB &&
        static_cast<e_wb_class*>(i_actor)->field_0x79d == 0)
    {
        fopAcM_delete((fopAc_ac_c*)i_actor);
    }

    return NULL;
}

static void* s_rdArrowWait_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_RD &&
        (static_cast<e_rd_class*>(i_actor)->field_0x5bc == 2 ||
         static_cast<e_rd_class*>(i_actor)->field_0x5bc == 3))
    {
        static_cast<e_rd_class*>(i_actor)->field_0x990[1] = cM_rndF(20.0) + 3.0f;
    }

    return NULL;
}

static void cam_3d_morf(e_wb_class* i_this, f32 i_scale) {
    cLib_addCalc2(&i_this->field_0x16bc.x, i_this->field_0x16e0.x, i_scale,
                  i_this->field_0x16f8.x * i_this->field_0x1710);
    cLib_addCalc2(&i_this->field_0x16bc.y, i_this->field_0x16e0.y, i_scale,
                  i_this->field_0x16f8.y * i_this->field_0x1710);
    cLib_addCalc2(&i_this->field_0x16bc.z, i_this->field_0x16e0.z, i_scale,
                  i_this->field_0x16f8.z * i_this->field_0x1710);
    cLib_addCalc2(&i_this->field_0x16a4.x, i_this->field_0x16d4.x, i_scale,
                  i_this->field_0x16ec.x * i_this->field_0x1710);
    cLib_addCalc2(&i_this->field_0x16a4.y, i_this->field_0x16d4.y, i_scale,
                  i_this->field_0x16ec.y * i_this->field_0x1710);
    cLib_addCalc2(&i_this->field_0x16a4.z, i_this->field_0x16d4.z, i_scale,
                  i_this->field_0x16ec.z * i_this->field_0x1710);
}

static void demo_camera(e_wb_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    camera_class* local_184 = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* local_188 = dComIfGp_getCamera(0);
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    daHorse_c* horse;
    e_rd_class* local_194;
    e_rdb_class* rdb;
    if (i_this->field_0x169e != 0) {
        horse = (daHorse_c*)dComIfGp_getHorseActor();
        local_194 = (e_rd_class*)fopAcM_SearchByID(i_this->field_0x1434);
        rdb = (e_rdb_class*)fopAcM_SearchByName(PROC_E_RDB);
    }
    cXyz local_3c;
    cXyz cStack_48;
    cXyz cStack_54;
    cXyz cStack_60;
    switch (i_this->field_0x169e) {
    case 1:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        local_184->mCamera.Stop();
        i_this->field_0x169e = 2;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1714 = 55.0f;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1708 = 800.0f;
        local_184->mCamera.SetTrimSize(3);
        a_this->current.pos.set(34800.0f, -66.0f, -604.0f);
        a_this->current.angle.y = 0x8000;
        a_this->shape_angle = a_this->current.angle;
        a_this->old = a_this->current;
        i_this->field_0x1704 = 0x2000;
        daPy_getPlayerActorClass()->changeOriginalDemo();
        horse->changeOriginalDemo();
        fpcM_Search(s_wbdel_sub, a_this);
        dComIfGp_getEvent().startCheckSkipEdge(a_this);
        // fallthrough
    case 2:
        if (i_this->field_0x16a0 == 0xa0) {
            cStack_48.set(34700.0f, -66.0f, 7896.0f);
            horse->setHorsePosAndAngle(&cStack_48, -0x8000);
            horse->changeDemoMode(4, 0);
            cStack_48.set(34900.0f, -280.0f, -18048.0f);
            horse->changeDemoPos0(&cStack_48);
        }
        cLib_addCalc2(&i_this->field_0x1708, 1200.0f, 0.05f, 3.0f);
        mDoMtx_stack_c::YrotS(a_this->shape_angle.y + i_this->field_0x1704);
        local_3c.x = 0.0f;
        local_3c.y = 300.0f;
        local_3c.z = i_this->field_0x1708;
        mDoMtx_stack_c::multVec(&local_3c, &i_this->field_0x16a4);
        i_this->field_0x16a4 += a_this->current.pos;
        cLib_addCalcAngleS2(&i_this->field_0x1704, 0x7800, 0x20, 0xa0);
        if (i_this->field_0x16a0 == 0) {
            i_this->field_0x16bc = a_this->current.pos;
            i_this->field_0x16bc.y += 400.0f;
        } else {
            cLib_addCalc2(&i_this->field_0x16bc.x, a_this->current.pos.x, 0.5f, 200.0f);
            cLib_addCalc2(&i_this->field_0x16bc.y, 200.0f + a_this->current.pos.y + 200.0f, 0.5f,
                          200.0f);
            cLib_addCalc2(&i_this->field_0x16bc.z, a_this->current.pos.z, 0.5f, 200.0f);
        }

        if (i_this->field_0x16a0 == 0xdc) {
            i_this->field_0x169e = 3;
            i_this->field_0x16a0 = 0;
            i_this->field_0x1708 = 1100.0f;
            i_this->field_0x1704 = 0x1000;
        }
        break;
    case 3:
        mDoMtx_stack_c::YrotS(player->shape_angle.y + i_this->field_0x1704);
        local_3c.x = 0.0f;
        local_3c.y = 0.0f;
        local_3c.z = i_this->field_0x1708;
        mDoMtx_stack_c::multVec(&local_3c, &i_this->field_0x16a4);
        i_this->field_0x16a4 += player->current.pos;
        i_this->field_0x16bc = player->current.pos;
        i_this->field_0x16bc.y += -50.0f;
        cLib_addCalc2(&i_this->field_0x1708, 500.0f, 0.05f, 10.0f);
        if (i_this->field_0x16a0 != 0x46) {
            break;
        }
        i_this->field_0x169e = 4;
        i_this->field_0x16a0 = 0;
        // fallthrough
    case 4:
        i_this->field_0x1708 = 1200.0f;
        mDoMtx_stack_c::YrotS(a_this->shape_angle.y - 0x7800);
        local_3c.x = 0.0f;
        local_3c.y = 300.0f;
        local_3c.z = i_this->field_0x1708;
        mDoMtx_stack_c::multVec(&local_3c, &i_this->field_0x16a4);
        i_this->field_0x16a4.x += a_this->current.pos.x;
        i_this->field_0x16a4.y += -300.0f;
        i_this->field_0x16a4.z += a_this->current.pos.z;
        i_this->field_0x16bc.x = a_this->current.pos.x;
        i_this->field_0x16bc.z = a_this->current.pos.z;
        if (i_this->field_0x16a0 == 0) {
            i_this->field_0x16bc.y = a_this->current.pos.y + 200.0f + 200.0f;
        } else {
            cLib_addCalc2(&i_this->field_0x16bc.y, a_this->current.pos.y + 200.0f + 200.0f, 0.1f,
                          20.0f);
        }
        if (i_this->field_0x16a0 != 0x8c) {
            break;
        }

        i_this->field_0x169e = 5;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1708 = 350.0f;
        i_this->field_0x170c = 50.0f;
        // fallthrough
    case 5:
    case 6:
        i_this->field_0x1704 = 0x7800;
        mDoMtx_stack_c::YrotS(player->shape_angle.y + i_this->field_0x1704);
        local_3c.x = i_this->field_0x170c;
        local_3c.y = 0.0f;
        local_3c.z = i_this->field_0x1708;
        mDoMtx_stack_c::multVec(&local_3c, &i_this->field_0x16a4);
        i_this->field_0x16a4.x += player->current.pos.x;
        i_this->field_0x16a4.z += player->current.pos.z;
        i_this->field_0x16a4.y = 50.0f;
        if (i_this->field_0x16a0 == 0) {
            i_this->field_0x16bc = player->current.pos;
            i_this->field_0x16bc.y += -50.0f + 50.0f;
        } else {
            i_this->field_0x16bc.x = player->current.pos.x;
            i_this->field_0x16bc.z = player->current.pos.z;
            f32 dVar13 = player->current.pos.y - 50.0f + 50.0f;
            if (dVar13 > 150.0f) {
                dVar13 = 150.0f;
            }
            cLib_addCalc2(&i_this->field_0x16bc.y, dVar13, 0.2f, 30.0f);
        }
        cLib_addCalc2(&i_this->field_0x1708, 800.0f, 0.05f, 5.0f);
        cLib_addCalc2(&i_this->field_0x170c, -50.0f, 0.05f, 1.2f);
        if (i_this->field_0x169e == 5 && player->current.pos.z < -14500.0f) {
            horse->changeDemoMode(7, 0);
            horse->onDemoJumpDistance(600.0f, 100.0f);
            i_this->field_0x169e = 6;
        }

        if (i_this->field_0x16a0 == 0x4b) {
            dComIfGp_setNextStage("F_SP102", 1, 0, 4);
        }
        break;
    case 10:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        local_184->mCamera.Stop();
        local_184->mCamera.SetTrimSize(3);
        i_this->field_0x169e = 0xb;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1714 = 55.0f;
        i_this->field_0x16a0 = 0;
        mDoMtx_stack_c::YrotS(local_194->actor.shape_angle.y +
                              (local_194->field_0xa0e - local_194->actor.shape_angle.y) / 2);
        local_3c.x = 0.0f;
        local_3c.y = 500.0f;
        local_3c.z = -500.0f;
        mDoMtx_stack_c::multVec(&local_3c, &i_this->field_0x16a4);
        i_this->field_0x16a4 += local_194->actor.current.pos;
        i_this->field_0x1710 = 10.0f;
        i_this->field_0x16bc = local_194->actor.current.pos;
        i_this->field_0x16bc.y += 200.0f;
        daPy_getPlayerActorClass()->changeOriginalDemo();
        horse->changeOriginalDemo();
        // fallthrough
    case 0xb:
        cLib_addCalc2(&i_this->field_0x1710, 1000.0f, 1.0f,
                                   100.0f);
        cLib_addCalc2(&i_this->field_0x16bc.x, local_194->actor.current.pos.x,
                                   0.5f,
                                   i_this->field_0x1710);
        cLib_addCalc2(&i_this->field_0x16bc.y, local_194->actor.current.pos.y + 200.0f,
                                   0.5f,
                                   i_this->field_0x1710);
        cLib_addCalc2(&i_this->field_0x16bc.z, local_194->actor.current.pos.z,
                                   0.5f,
                                   i_this->field_0x1710);
        cLib_addCalc2(&i_this->field_0x16a4.x, local_194->actor.current.pos.x, 0.05f,
                                   12.0f);
        if (i_this->field_0x16a0 == 0x28) {
            if (a_this->shape_angle.y < 0x4000 && -0x4000 < a_this->shape_angle.y) {
                cStack_48.set(34800.0f, -300.0f, -26400.0f);
                horse->setHorsePosAndAngle(&cStack_48, player->shape_angle.y);
                horse->changeDemoMode(3, 0);
                cStack_48.set(34800.0f, -300.0f, -36400.0f);
                horse->changeDemoPos0(&cStack_48);
            } else {
                cStack_48.set(34800.0f, -300.0f, -26400.0f);
                horse->setHorsePosAndAngle(&cStack_48, player->shape_angle.y);
                horse->changeDemoMode(3, 0);
                cStack_48.set(34800.0f, -300.0f, -16400.0f);
                horse->changeDemoPos0(&cStack_48);
            }
        }
        if (i_this->field_0x16a0 >= 40 && i_this->field_0x16a0 < 87) {
            a_this->current.pos.y = 5000.0f;
            a_this->current.pos.z = -18000.0f;
            a_this->speedF = 0.0f;
        }

        if (i_this->field_0x16a0 == 0x57) {
            a_this->current.pos.y = -300.0f;
            if (a_this->shape_angle.y < 0x4000 && -0x4000 < a_this->shape_angle.y) {
                a_this->current.pos.z = -25000.0f;
            } else {
                a_this->current.pos.z = -27800.0f;
            }
            a_this->speedF = l_HIO.mSingleRiderSpeed;
        }

        if (i_this->field_0x16a0 == 0x5a) {
            i_this->field_0x16bc.set(48545.0f, -1884.0f, -27557.0f);
            i_this->field_0x16a4.set(49099.0f, -1884.0f, -27626.0f);
            i_this->field_0x169e = 0xc;
            i_this->field_0x16a0 = 0;
            i_this->field_0x1714 = 45.0f;
            fopAc_ac_c* local_19c = fopAcM_SearchByID(i_this->field_0x1434);
            local_19c->current.pos.z = -26400.0f;
            local_19c->current.pos.y = -3500.0f;
            local_19c->current.pos.x = 34800.0f;
            local_19c->speed.set(0.0f, -50.0f, 0.0f);
            local_19c->speedF = 0.0f;
        }

        if (!daPy_getPlayerActorClass()->checkHorseRide()) {
            local_184->mCamera.Start();
            local_184->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            i_this->field_0x169e = 0;
        }
        break;
    case 0xc:
        cLib_addCalc2(&i_this->field_0x1714, 55.0f, 0.05f, 0.1f);
        if (i_this->field_0x16a0 == 0x5a) {
            Z2GetAudioMgr()->bgmStreamPrepare(0x2000010);
            Z2GetAudioMgr()->bgmStreamPlay();
            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[85]);
            dComIfGp_setNextStage("F_SP102", 0x14, 0, 8);
        }
        break;
    case 0x14:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        local_184->mCamera.Stop();
        local_184->mCamera.SetTrimSize(3);
        i_this->field_0x169e = 0x15;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1714 = 55.0f;
        i_this->field_0x16a0 = 0;
        mDoMtx_stack_c::YrotS(player->shape_angle.y);
        local_3c.x = -500.0f;
        local_3c.y = 100.0f;
        local_3c.z = 0.0f;
        mDoMtx_stack_c::multVec(&local_3c, &i_this->field_0x16a4);
        i_this->field_0x16a4 += player->current.pos;
        i_this->field_0x1710 = 0;
        i_this->field_0x16bc = a_this->current.pos;
        i_this->field_0x16bc.y += 300.0f;
        horse->changeOriginalDemo();
        horse->changeDemoMode(6, 0);
    case 0x15:
        s16 local_1cc;
        if (horse->shape_angle.y < 0x4000 && -0x4000 < horse->shape_angle.y) {
            local_1cc = 0;
        } else {
            local_1cc = -0x8000;
        }
        horse->setHorsePosAndAngle(&horse->current.pos, local_1cc);
        cLib_addCalc2(&i_this->field_0x16bc.x, a_this->current.pos.x, 0.8f,
                                   100.0f);
        cLib_addCalc2(&i_this->field_0x16bc.y, (a_this->current.pos.y + 300.0f),
                                   0.8f, 100.0f);
        cLib_addCalc2(&i_this->field_0x16bc.z, a_this->current.pos.z, 0.8f,
                                   100.0f);
        if (i_this->field_0x16a0 == 0x14) {
            local_184->mCamera.Start();
            local_184->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            i_this->field_0x169e = 0;
        }
        break;
    case 0x19: {
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        local_184->mCamera.Stop();
        local_184->mCamera.SetTrimSize(3);
        i_this->field_0x169e = 0x1a;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1714 = 55.0f;
        i_this->field_0x16a0 = 0;
        horse->changeOriginalDemo();
        horse->changeDemoMode(6, 0);
        s16 angleDiff = a_this->shape_angle.y - fopAcM_searchPlayerAngleY(a_this);
        if (angleDiff < 0) {
            i_this->field_0x170c = 200.0f;
        } else {
            i_this->field_0x170c = -200.0f;
        }
    }
    case 0x1a:
        mDoMtx_stack_c::YrotS(a_this->shape_angle.y);
        local_3c.x = i_this->field_0x170c;
        local_3c.y = 300.0f;
        local_3c.z = 300.0f;
        mDoMtx_stack_c::multVec(&local_3c, &i_this->field_0x16a4);
        i_this->field_0x16a4 += a_this->current.pos;
        i_this->field_0x16bc = a_this->current.pos;
        i_this->field_0x16bc.y += 400.0f;
        if (i_this->field_0x16a0 == 0xf) {
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            local_3c.x = 0.0f;
            local_3c.y = 400.0f;
            local_3c.z = -600.0f;
            MtxPosition(&local_3c, &i_this->field_0x16a4);
            i_this->field_0x16a4 += player->current.pos;
            i_this->field_0x16bc = player->current.pos;
            i_this->field_0x16bc.y += 120.0f;
            local_184->mCamera.Reset(i_this->field_0x16bc, i_this->field_0x16a4,
                                     i_this->field_0x1714, 0);
            local_184->mCamera.Start();
            local_184->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            i_this->field_0x169e = 0;
        }
        break;
    case 0x1e:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        local_184->mCamera.Stop();
        local_184->mCamera.SetTrimSize(3);
        i_this->field_0x169e = 0x1f;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1714 = 55.0f;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1710 = 0;
        i_this->field_0x16bc.set(-8592.0f, 342.0f, -139.0f);
        i_this->field_0x16a4.set(-8844.0f, 317.0f, 512.0f);
        i_this->field_0x16e0.set(-8416.0f, 345.0f, 6.0f);
        i_this->field_0x16d4.set(-8994.0f, 326.0f, 400.0f);
        i_this->field_0x16ec.x = std::fabsf(i_this->field_0x16d4.x - i_this->field_0x16a4.x);
        i_this->field_0x16ec.y = std::fabsf(i_this->field_0x16d4.y - i_this->field_0x16a4.y);
        i_this->field_0x16ec.z = std::fabsf(i_this->field_0x16d4.z - i_this->field_0x16a4.z);
        i_this->field_0x16f8.x = std::fabsf(i_this->field_0x16e0.x - i_this->field_0x16bc.x);
        i_this->field_0x16f8.y = std::fabsf(i_this->field_0x16e0.y - i_this->field_0x16bc.y);
        i_this->field_0x16f8.z = std::fabsf(i_this->field_0x16e0.z - i_this->field_0x16bc.z);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        horse->changeOriginalDemo();
        cStack_48.set(-10775.0f + 350.0f, 60.0f, 75.0f);
        horse->setHorsePosAndAngle(&cStack_48, horse->shape_angle.y);
        horse->changeDemoMode(3, 0);
        cStack_48.set(-8705.0f, 213.0f, 75.0f);
        horse->changeDemoPos0(&cStack_48);
        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[86]);
        Z2GetAudioMgr()->bgmStreamPrepare(0x200000f);
        Z2GetAudioMgr()->bgmStreamPlay();
    case 0x1f:
        if (i_this->field_0x16a0 > 50) {
            cam_3d_morf(i_this, 0.1f);
            cLib_addCalc2(&i_this->field_0x1710, 0.02f, 1.0f, 0.001f);
        }
        if (i_this->field_0x16a0 == 40) {
            horse->changeDemoMode(8, 0);
            dComIfGp_getEvent().startCheckSkipEdge(a_this);
        }
        if (i_this->field_0x16a0 == 80) {
            horse->changeDemoMode(9, 0);
        }
        if (i_this->field_0x16a0 == 143) {
            dKy_change_colpat(4);
        }
        if (i_this->field_0x16a0 == 144) {
            i_this->field_0x169e = 0x20;
            i_this->field_0x16a0 = 0;
            horse->changeDemoMode(1, 0);
            i_this->field_0x16bc.set(-9224.0f, 500.0f, 300.0f);
            i_this->field_0x16a4.set(-8586.0f, 447.0f, 18.0f);
            i_this->field_0x1714 = 40.0f;
        }
        break;
    case 0x20:
        if (i_this->field_0x16a0 == 10) {
            player->changeDemoParam2(1);
        }
        if (i_this->field_0x16a0 != 0x23) {
            break;
        }

        i_this->field_0x169e = 0x21;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1714 = 85.0f;
        i_this->field_0x1710 = 0;
        dKy_change_colpat(5);
        // fallthrough
    case 0x21:
        i_this->field_0x16b0.set(400.0f, 400.0f, 500.0f);
        i_this->field_0x16c8.set(-10.0f, 700.0f, 180.0f);
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        MtxPosition(&i_this->field_0x16b0, &cStack_48);
        i_this->field_0x16a4 = a_this->current.pos + cStack_48;
        MtxPosition(&i_this->field_0x16c8, &cStack_48);
        i_this->field_0x16bc = a_this->current.pos + cStack_48;
        if (i_this->field_0x16a0 > 25) {
            cLib_addCalc2(&i_this->field_0x1714, 55.0f - 30.0f, 0.8f, 6.0f);
        }
        if (i_this->field_0x16a0 == 95) {
            i_this->field_0x169e = 0x22;
            i_this->field_0x16a0 = 0;
            i_this->field_0x16d4.set(700.0f, 100.0f, 0.0f);
            i_this->field_0x16e0.set(-10.0f, 350.0f, 80.0f);
            i_this->field_0x16ec.x = std::fabsf(i_this->field_0x16d4.x - i_this->field_0x16b0.x);
            i_this->field_0x16ec.y = std::fabsf(i_this->field_0x16d4.y - i_this->field_0x16b0.y);
            i_this->field_0x16ec.z = std::fabsf(i_this->field_0x16d4.z - i_this->field_0x16b0.z);
            i_this->field_0x16f8.x = std::fabsf(i_this->field_0x16e0.x - i_this->field_0x16c8.x);
            i_this->field_0x16f8.y = std::fabsf(i_this->field_0x16e0.y - i_this->field_0x16c8.y);
            i_this->field_0x16f8.z = std::fabsf(i_this->field_0x16e0.z - i_this->field_0x16c8.z);
            i_this->field_0x1710 = 0;
        }
        break;
    case 0x22:
        cLib_addCalc2(&i_this->field_0x1714, 55.0f, 0.05f, 0.3f);
        cLib_addCalc2(&i_this->field_0x16c8.x, i_this->field_0x16e0.x, 0.05f,
                      i_this->field_0x16f8.x * i_this->field_0x1710);
        cLib_addCalc2(&i_this->field_0x16c8.y, i_this->field_0x16e0.y, 0.05f,
                      i_this->field_0x16f8.y * i_this->field_0x1710);
        cLib_addCalc2(&i_this->field_0x16c8.z, i_this->field_0x16e0.z, 0.05f,
                      i_this->field_0x16f8.z * i_this->field_0x1710);
        cLib_addCalc2(&i_this->field_0x16b0.x, i_this->field_0x16d4.x, 0.05f,
                      (i_this->field_0x16ec.x * i_this->field_0x1710));
        cLib_addCalc2(&i_this->field_0x16b0.y, i_this->field_0x16d4.y, 0.05f,
                      (i_this->field_0x16ec.y * i_this->field_0x1710));
        cLib_addCalc2(&i_this->field_0x16b0.z, i_this->field_0x16d4.z, 0.05f,
                      (i_this->field_0x16ec.z * i_this->field_0x1710));
        cLib_addCalc2(&i_this->field_0x1710, 0.01f, 1.0f, 0.0005f);
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        MtxPosition(&i_this->field_0x16b0, &i_this->field_0x16a4);
        i_this->field_0x16a4 += a_this->current.pos;
        MtxPosition(&i_this->field_0x16c8, &i_this->field_0x16bc);
        i_this->field_0x16bc += a_this->current.pos;
        if (i_this->field_0x16a0 == 130) {
            local_194->mMode++;
            player->changeDemoParam2(0);
        }
        if (i_this->field_0x16a0 == 220) {
            i_this->field_0x169e = 0x23;
            i_this->field_0x16a0 = 0;
            dKy_change_colpat(4);
        }
        break;
    case 0x23:
        i_this->field_0x1714 = 55.0f;
        i_this->field_0x16bc.set(-9110.0f, 492.0f, 331.0f);
        i_this->field_0x16a4.set(-8469.0f, 372.0f, -54.0f);
        if (i_this->field_0x16a0 == 10) {
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
        }

        if (i_this->field_0x16a0 == 50) {
            i_this->field_0x169e = 0x24;
            i_this->field_0x16a0 = 0;
        }
        break;
    case 0x24:
        i_this->field_0x16b0.set(700.0f, 100.0f, 500.0f);
        i_this->field_0x16c8.set(-10.0f, 350.0f, 80.0f);
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        MtxPosition(&i_this->field_0x16b0, &i_this->field_0x16a4);
        i_this->field_0x16a4 += a_this->current.pos;
        MtxPosition(&i_this->field_0x16c8, &i_this->field_0x16bc);
        i_this->field_0x16bc += a_this->current.pos;
        if (i_this->field_0x16a0 == 20) {
            local_194->mMode++;
        }
        if (i_this->field_0x16a0 != 92) {
            break;
        }
        i_this->field_0x169e = 0x25;
        i_this->field_0x16a0 = 0;
        fpcM_Search(s_wbstart_sub, a_this);
        i_this->field_0x16bc.set(-12800.0f, 153.0f, 51.0f);
        i_this->field_0x16a4.set(-12022.0f, 82.0f, 51.0f);
        i_this->field_0x16e0.set(-13168.0f, 163.0f, -200.0f);
        i_this->field_0x16d4.set(-12500.0f, 83.0f, -593.0f);
        i_this->field_0x16ec.x = std::fabsf(i_this->field_0x16d4.x -
                                                i_this->field_0x16a4.x);
        i_this->field_0x16ec.y = std::fabsf(i_this->field_0x16d4.y -
                                                i_this->field_0x16a4.y);
        i_this->field_0x16ec.z = std::fabsf(i_this->field_0x16d4.z -
                                                i_this->field_0x16a4.z);
        i_this->field_0x16f8.x = std::fabsf(i_this->field_0x16e0.x -
                                                i_this->field_0x16bc.x);
        i_this->field_0x16f8.y = std::fabsf(i_this->field_0x16e0.y -
                                                i_this->field_0x16bc.y);
        i_this->field_0x16f8.z = std::fabsf(i_this->field_0x16e0.z -
                                                i_this->field_0x16bc.z);
        i_this->field_0x1710 = 0;

        // fallthrough
    case 0x25:
        if (i_this->field_0x16a0 < 110) {
            fpcM_Search(s_spd0_sub, i_this);
        }
        if (i_this->field_0x16a0 < 0xd2) {
            fpcM_Search(s_rdArrowWait_sub, i_this);
        }
        if (i_this->field_0x16a0 > 165) {
            cam_3d_morf(i_this, 0.1f);
            cLib_addCalc2(&i_this->field_0x1710, 0.01f, 1.0f,
                                       0.00015f);
        }
        if (i_this->field_0x16a0 == 245) {
            dComIfGp_getVibration().StartQuake(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->field_0x16a0 == 295) {
            dComIfGp_getVibration().StopQuake(0x1f);
        }

        if (i_this->field_0x16a0 > 250 && i_this->field_0x16a0 < 285) {
            cLib_addCalc2(&i_this->field_0x171c, 35.0f - 10.0f, 1.0f,
                                       3.0f);
        }

        if (i_this->field_0x16a0 == 325) {
            fpcM_Search(s_wbZrevise_sub, i_this);
        }

        if (i_this->field_0x16a0 == 0x14f) {
            i_this->field_0x169e = 0x26;
            i_this->field_0x16a0 = 0;
            i_this->field_0x7a2 = 0;
            i_this->mActionID = ACT_B_RUN;
            i_this->mActionMode = 0;
            i_this->field_0x16bc.set(-8416.0f, 345.0f, 6.0f);
            i_this->field_0x16a4.set(-8994.0f, 326.0f, 400.0f);
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_RDB_V_HOICK, -1);
            local_194->field_0x990[3] = 700;
        }
        break;
    case 0x26:
        if (i_this->field_0x16a0 == 50) {
            local_184->mCamera.Reset(i_this->field_0x16bc, i_this->field_0x16a4);
            local_184->mCamera.Start();
            local_184->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            horse->cancelOriginalDemo();
            i_this->field_0x169e = 0;
            Z2GetAudioMgr()->subBgmStart(Z2BGM_HORSE_BATTLE);
            dKy_change_colpat(3);
        }
        break;
    case 0x28:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        local_184->mCamera.Stop();
        local_184->mCamera.SetTrimSize(3);
        i_this->field_0x169e = 0x29;
        i_this->field_0x1714 = 55.0f;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1710 = 0;
        i_this->field_0x16bc.set(36156.0f, 985.0f, -16931.0f);
        i_this->field_0x16a4.set(36496.0f, 1358.0f, -17344.0f);
        i_this->field_0x16e0.set(36453.0f, 1116.0f, -16995.0f);
        i_this->field_0x16d4.set(35835.0f, 955.0f, -16867.0f);
        i_this->field_0x16ec.x = std::fabsf(i_this->field_0x16d4.x - i_this->field_0x16a4.x) * 0.3f;
        i_this->field_0x16ec.y = std::fabsf(i_this->field_0x16d4.y - i_this->field_0x16a4.y) * 0.3f;
        i_this->field_0x16ec.z = std::fabsf(i_this->field_0x16d4.z - i_this->field_0x16a4.z) * 0.3f;
        i_this->field_0x16f8.x = std::fabsf(i_this->field_0x16e0.x - i_this->field_0x16bc.x);
        i_this->field_0x16f8.y = std::fabsf(i_this->field_0x16e0.y - i_this->field_0x16bc.y);
        i_this->field_0x16f8.z = std::fabsf(i_this->field_0x16e0.z - i_this->field_0x16bc.z);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        horse->changeOriginalDemo();
        cStack_48.set(34800.0f, -300.0f, -15200.0f);
        horse->setHorsePosAndAngle(&cStack_48, -0x8000);
        horse->changeDemoMode(3, 0);
        cStack_48.set(34800.0f, -300.0f, -20000.0f);
        horse->changeDemoPos0(&cStack_48);
        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[87]);
        Z2GetAudioMgr()->bgmStart(Z2BGM_HORSE_BATTLE_D02, 0, 0);
        dComIfGp_getEvent().startCheckSkipEdge(i_this);
        // fallthrough
    case 0x29:
        if (i_this->field_0x16a0 == 30) {
            horse->changeDemoMode(8, 0);
        }

        if (i_this->field_0x16a0 > 70) {
            cam_3d_morf(i_this, 0.2f);
            cLib_addCalc2(&i_this->field_0x1710, 0.2f, 1.0f, 0.01f);
            if (i_this->field_0x16a0 == 150) {
                i_this->field_0x16bc.set(35528.0f, 316.0f, -16987.0f);
                i_this->field_0x16a4.set(35059.0f, -128.0f, -16992.0f);
                i_this->field_0x16ec.set(0.0f, 0.0f, 0.0f);
                i_this->field_0x16e0.set(34973.0f, -148.0f, -16346.0f);
                i_this->field_0x16f8.x =
                    std::fabsf(i_this->field_0x16e0.x - i_this->field_0x16bc.x);
                i_this->field_0x16f8.y =
                    std::fabsf(i_this->field_0x16e0.y - i_this->field_0x16bc.y);
                i_this->field_0x16f8.z =
                    std::fabsf(i_this->field_0x16e0.z - i_this->field_0x16bc.z);
                i_this->field_0x1710 = 0;
                i_this->field_0x169e = 0x2a;
                i_this->field_0x16a0 = 0;
            }
        }
        break;
    case 0x2a:
        i_this->field_0x1710 = 0.075f;
        cam_3d_morf(i_this, 0.5f);
        if (i_this->field_0x16a0 == 20) {
            i_this->field_0x1722 = 1;
            i_this->field_0x1718 = 0.0f;
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_STRAWFENCE_IGNITION, &saku_p, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_STRAWFENCE_IGNITION, &saku_p2, 0, 0, 1.0f, 1.0f,
                                     -1.0f, -1.0f, 0);
        }

        if (i_this->field_0x16a0 == 23) {
            horse->changeDemoMode(9, 0);
        }

        if (i_this->field_0x16a0 == 83) {
            i_this->field_0x16bc.set(34800.0f, 15.7f, -16820.0f);
            i_this->field_0x16a4.set(34800.0f, 10.0f, -16170.0f);
            i_this->field_0x1710 = 0;
            i_this->field_0x1714 = 55.0f;
            i_this->field_0x169e = 0x2b;
            i_this->field_0x16a0 = 0;
            i_this->mActionMode = -100;
        }
        break;
    case 0x2b:
        if (i_this->field_0x16a0 == 5) {
            horse->changeDemoMode(1, 0);
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
            fpcM_Search(s_rddel_sub, i_this);
        }

        if (i_this->field_0x16a0 > 40) {
            cLib_addCalc2(&i_this->field_0x1714, 4.5f, 0.4f, 3.5f);
            if (i_this->field_0x16a0 == 130) {
                i_this->field_0x16bc.set(34843.0f, -92.7f, -35956.0f);
                i_this->field_0x16a4.set(35167.0f, -202.0f, -35403.0f);
                i_this->field_0x16e0.set(34740.0f, -92.7f, -35966.0f);
                i_this->field_0x16d4.set(34320.0, -202.0, -35490.0);
                i_this->field_0x16ec.x =
                    std::fabsf(i_this->field_0x16d4.x - i_this->field_0x16a4.x);
                i_this->field_0x16ec.y =
                    std::fabsf(i_this->field_0x16d4.y - i_this->field_0x16a4.y);
                i_this->field_0x16ec.z =
                    std::fabsf(i_this->field_0x16d4.z - i_this->field_0x16a4.z);
                i_this->field_0x16f8.x =
                    std::fabsf(i_this->field_0x16e0.x - i_this->field_0x16bc.x);
                i_this->field_0x16f8.y =
                    std::fabsf(i_this->field_0x16e0.y - i_this->field_0x16bc.y);
                i_this->field_0x16f8.z =
                    std::fabsf(i_this->field_0x16e0.z - i_this->field_0x16bc.z);
                i_this->field_0x1714 = 55.0f;
                i_this->field_0x169e = 0x2c;
                i_this->field_0x16a0 = 0;
                i_this->field_0x698[0] = 0x14;
            }
        }
        break;
    case 0x2c:
        cam_3d_morf(i_this, 0.05f);
        cLib_addCalc2(&i_this->field_0x1710, 0.005f, 1.0f, 0.0002f);
        if (i_this->field_0x16a0 == 170) {
            i_this->mActionMode++;
        }

        if (i_this->field_0x16a0 == 258) {
            cStack_48.set(34800.0f, -300.0f, -18700.0f);
            horse->setHorsePosAndAngle(&cStack_48, horse->shape_angle.y);
        }
        if (i_this->field_0x16a0 == 306) {
            local_184->mCamera.Reset(i_this->field_0x16bc, i_this->field_0x16a4);
            local_184->mCamera.Start();
            local_184->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            horse->cancelOriginalDemo();
            i_this->field_0x169e = 0;
            Z2GetAudioMgr()->subBgmStart(Z2BGM_FACE_OFF_BATTLE2);
        }
        break;
    case 0x32:
        if (!rdb->enemy.eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(&rdb->enemy, 2, 0xffff, 0);
            rdb->enemy.eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        local_184->mCamera.Stop();
        local_184->mCamera.SetTrimSize(3);
        i_this->field_0x169e = 0x33;
        i_this->field_0x1714 = 55.0f;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1710 = 0;
        i_this->field_0x16bc.set(1679.0, 100.0, 852.0);
        i_this->field_0x16a4.set(1873.0, 64.0, 527.0);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        dComIfGp_getEvent().startCheckSkipEdge(i_this);
        dComIfGs_onOneZoneSwitch(4, -1);
    case 0x33:
        if (i_this->field_0x16a0 < 30) {
            cStack_48.set(1787.0f, 1.75f, 484.0f);
            player->setPlayerPosAndAngle(&cStack_48, -0x12c3, 0);
        }

        if (i_this->field_0x16a0 == 30) {
            player->changeDemoMode(2, 1, 0, 0);
            cStack_48.set(1634.0, 1.4, 811.0);
            player->changeDemoPos0(&cStack_48);
        }

        if (i_this->field_0x16a0 == 139) {
            rdb->field_0x6d8 = 2;
        }

        if (i_this->field_0x16a0 == 0x8c) {
            i_this->field_0x169e = 0x34;
            i_this->field_0x16a0 = 0;
            cMtx_YrotS(*calc_mtx, rdb->enemy.shape_angle.y);
            local_3c.x = 0.0f;
            local_3c.y = 150.0f;
            local_3c.z = 800.0f;
            MtxPosition(&local_3c, &i_this->field_0x16a4);
            i_this->field_0x16a4 += rdb->enemy.current.pos;
            i_this->field_0x16bc = rdb->enemy.current.pos;
            i_this->field_0x16bc.y += 150.0f;
            rdb->mMode = 2;
            rdb->field_0x6b8[0] = 0x28;
        case 0x34:
            cLib_addCalc0(&rdb->field_0x6e0, 1.0f, 0.3f);
            if (i_this->field_0x16a0 == 145) {
                rdb->field_0x6d8 = 3;
            }
            if (i_this->field_0x16a0 == 0xaf) {
                rdb->mMode = 4;
            }
            if (i_this->field_0x16a0 == 0x138) {
                i_this->mActionID = 0x18;
                i_this->mActionMode = 0;
                i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_DAMAGE, -1);
                cXyz cStack_6c(a_this->current.pos);
                cStack_6c.y += 150.0f;
                dComIfGp_setHitMark(3, 0, &cStack_6c, &a_this->shape_angle, 0, 0);
            }
            if (i_this->field_0x16a0 == 315) {
                i_this->field_0x16bc.set(1717.0f, 96.0f, 910.0f);
                i_this->field_0x16a4.set(2226.0f, 56.0f, 751.0f);
                i_this->field_0x16e0.set(1780.0f, 92.0f, 967.0f);
                i_this->field_0x16d4.set(1988.0f, 69.0f, 475.0f);
                i_this->field_0x16ec.x =
                    std::fabsf(i_this->field_0x16d4.x - i_this->field_0x16a4.x);
                i_this->field_0x16ec.y =
                    std::fabsf(i_this->field_0x16d4.y - i_this->field_0x16a4.y);
                i_this->field_0x16ec.z =
                    std::fabsf(i_this->field_0x16d4.z - i_this->field_0x16a4.z);
                i_this->field_0x16f8.x =
                    std::fabsf(i_this->field_0x16e0.x - i_this->field_0x16bc.x);
                i_this->field_0x16f8.y =
                    std::fabsf(i_this->field_0x16e0.y - i_this->field_0x16bc.y);
                i_this->field_0x16f8.z =
                    std::fabsf(i_this->field_0x16e0.z - i_this->field_0x16bc.z);
                i_this->field_0x1710 = 0;
                player->setPlayerPosAndAngle(&player->current.pos, player->shape_angle.y - 4000, 0);
            }
            if (i_this->field_0x16a0 == 345) {
                daPy_getPlayerActorClass()->setThrowDamage(rdb->enemy.shape_angle.y - 8000, 40.0f,
                                                           25.0f, 0, 1, 0);
                daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
                cXyz cStack_78(player->current.pos);
                cStack_78.y += 150.0f;
                dComIfGp_setHitMark(3, 0, &cStack_78, &player->shape_angle, 0, 0);
            }

            if (i_this->field_0x16a0 == 418) {
                daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
            }

            if (i_this->field_0x16a0 >= 378) {
                cam_3d_morf(i_this, 0.1f);
                cLib_addCalc2(&i_this->field_0x1710, 0.05f, 1.0f, 0.001f);
            }

            if (i_this->field_0x16a0 >= 490) {
                local_184->mCamera.Reset(i_this->field_0x16bc, i_this->field_0x16a4);
                local_184->mCamera.Start();
                local_184->mCamera.SetTrimSize(0);
                dComIfGp_event_reset();
                daPy_getPlayerActorClass()->cancelOriginalDemo();
                i_this->field_0x169e = 0;
                rdb->mMode = 7;
                Z2GetAudioMgr()->subBgmStart(Z2BGM_FACE_OFF_BATTLE3);
            }
        }
        break;
    case 0x3c:
        if (!rdb->enemy.eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(&rdb->enemy, 2, 0xffff, 0);
            rdb->enemy.eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        local_184->mCamera.Stop();
        local_184->mCamera.SetTrimSize(3);
        i_this->field_0x169e = 0x3d;
        i_this->field_0x1714 = 55.0f;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1710 = 0;
        i_this->field_0x16bc.set(2297.0f, 81.0f, 1668.0f);
        i_this->field_0x16a4.set(2285.0f, 95.0f, 1988.0f);
        rdb->enemy.current.pos.set(2341.0f, 1.0f, 1292.0f);
        rdb->enemy.current.angle.y = 0x958;
        rdb->enemy.shape_angle.y = 0x958;
        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
        dComIfGs_onOneZoneSwitch(5, -1);
        // fallthrough
    case 0x3d:
        if (i_this->field_0x16a0 == 170) {
            daPy_getPlayerActorClass()->changeDemoMode(0xc, 0, 0, 0);
        }

        cStack_48.set(2120.0f, player->current.pos.y, 1200.0f);
        player->setPlayerPosAndAngle(&cStack_48, 5300, 0);
        cStack_60.y = rdb->enemy.eyePos.y;
        if (cStack_60.y < 81.0f) {
            cStack_60.y = 81.0f;
        }

        if (cStack_60.y > 120.0f) {
            cStack_60.y = 120.0f;
        }

        cLib_addCalc2(&i_this->field_0x16bc.y, cStack_60.y, 0.1f, 10.0f);
        if (i_this->field_0x16a0 == 260) {
            rdb->mMode = 3;
        }

        if (i_this->field_0x16a0 == 630) {
            i_this->field_0x16bc.set(2272.0f, 136.0f, 1555.0f);
            i_this->field_0x16a4.set(2056.0f, 160.0f, 1318.0f);
            i_this->field_0x169e = 0x3e;
            i_this->field_0x16a0 = 0;
        }
        break;
    case 0x3e: {
        daObjCRVSTEEL_c* crvSteel = (daObjCRVSTEEL_c*)fopAcM_SearchByName(PROC_Obj_CRVSTEEL);
        if (i_this->field_0x16a0 == 20) {
            crvSteel->OpenSet(20.0f, 350.0f);
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_URA_GATE, &crvSteel->current.pos, 0, 0, 1.0f,
                                     1.0f, -1.0f, -1.0f, 0);
        }

        if (i_this->field_0x16a0 == 130) {
            crvSteel->CloseSet(-40.0f);
        }

        if (i_this->field_0x16a0 == 140) {
            dComIfGp_getVibration().StartShock(5, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->field_0x16a0 != 170) {
            break;
        }
        rdb->mMode = 10;
        rdb->field_0x680 = 0;
        i_this->field_0x16e0.set(2834.0f, 490.0f, 1584.0f);
        i_this->field_0x16d4.set(1978.0f, 87.0f, 1225.0f);
        i_this->field_0x16ec.x = std::fabsf(i_this->field_0x16d4.x - i_this->field_0x16a4.x);
        i_this->field_0x16ec.y = std::fabsf(i_this->field_0x16d4.y - i_this->field_0x16a4.y);
        i_this->field_0x16ec.z = std::fabsf(i_this->field_0x16d4.z - i_this->field_0x16a4.z);
        i_this->field_0x16f8.x = std::fabsf(i_this->field_0x16e0.x - i_this->field_0x16bc.x);
        i_this->field_0x16f8.y = std::fabsf(i_this->field_0x16e0.y - i_this->field_0x16bc.y);
        i_this->field_0x16f8.z = std::fabsf(i_this->field_0x16e0.z - i_this->field_0x16bc.z);
        i_this->field_0x1710 = 0;
        i_this->field_0x169e = 0x3f;
        i_this->field_0x16a0 = 0;
        // fallthrough
    }
    case 0x3f:
        if (i_this->field_0x16a0 == 40) {
            rdb->field_0xfe4 = 1;
            Z2GetAudioMgr()->bgmStart(Z2BGM_EVENT02, 0, 0);
        }

        if (i_this->field_0x16a0 == 60) {
            rdb->field_0x6d8 = 4;
        }

        if (i_this->field_0x16a0 > 60) {
            cam_3d_morf(i_this, 0.05f);
            cLib_addCalc2(&i_this->field_0x1710, 0.05f, 1.0f, 0.0002);
        }

        if (i_this->field_0x16a0 == 110) {
            daPy_getPlayerActorClass()->changeDemoMode(0x14, 0, 0, 0);
        }

        if (i_this->field_0x16a0 != 220) {
            break;
        }

        i_this->field_0x16bc.set(1425.0f, 259.0f, 1583.0f);
        i_this->field_0x16a4.set(2265.0f, 127.0f, 1011.0f);
        i_this->field_0x16e0.set(1267.0f, 259.0f, 821.0f);
        i_this->field_0x16d4.set(2265.0f, 127.0f, 1011.0f);
        i_this->field_0x16ec.x = std::fabsf(i_this->field_0x16d4.x - i_this->field_0x16a4.x);
        i_this->field_0x16ec.y = std::fabsf(i_this->field_0x16d4.y - i_this->field_0x16a4.y);
        i_this->field_0x16ec.z = std::fabsf(i_this->field_0x16d4.z - i_this->field_0x16a4.z);
        i_this->field_0x16f8.x = std::fabsf(i_this->field_0x16e0.x - i_this->field_0x16bc.x);
        i_this->field_0x16f8.y = std::fabsf(i_this->field_0x16e0.y - i_this->field_0x16bc.y);
        i_this->field_0x16f8.z = std::fabsf(i_this->field_0x16e0.z - i_this->field_0x16bc.z);
        i_this->field_0x1710 = 0;
        i_this->field_0x1714 = 60.0f;
        i_this->field_0x169e = 0x40;
        i_this->field_0x16a0 = 0;
        daPy_getPlayerActorClass()->changeDemoMode(0x14, 0, 0, 0);
        // fallthrough
    case 0x40:
        if (i_this->field_0x16a0 > 60) {
            cam_3d_morf(i_this, 0.05f);
            cLib_addCalc2(&i_this->field_0x1710, 0.05f, 1.0f, 0.0002f);
        }

        if (i_this->field_0x16a0 == 170) {
            i_this->field_0x1720 = 0;
        }

        if (i_this->field_0x16a0 == 250) {
            local_184->mCamera.Reset(i_this->field_0x16bc, i_this->field_0x16a4);
            local_184->mCamera.Start();
            local_184->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            i_this->field_0x169e = 0;
        }
        break;
    case 0x46:
        if (!rdb->enemy.eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(&rdb->enemy, 2, 0xffff, 0);
            rdb->enemy.eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        local_184->mCamera.Stop();
        local_184->mCamera.SetTrimSize(3);
        i_this->field_0x169e = 0x47;
        i_this->field_0x1714 = 55.0f;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1710 = 0;
        i_this->field_0x16bc.set(1895.0, 159.0, -129.0);
        i_this->field_0x16a4.set(1984.0, 151.0, 158.0);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        fpcM_Search(s_rddel_sub, i_this);
        rdb->field_0x6d8 = 1;
        // fallthrough
    case 0x47:
        cStack_48.set(1800.0f, player->current.pos.y, -192.0f);
        player->setPlayerPosAndAngle(&cStack_48, 0, 0);
        if (i_this->field_0x16a0 == 2) {
            daObjCRVGATE_c* crvGate = (daObjCRVGATE_c*)fopAcM_SearchByName(PROC_Obj_CRVGATE);
            crvGate->SetB_Close();
        }

        if (i_this->field_0x16a0 == 25) {
            daPy_getPlayerActorClass()->changeDemoMode(0x19, 0, 0, 0);
        }

        if (i_this->field_0x16a0 == 73) {
            local_184->mCamera.Reset(i_this->field_0x16bc, i_this->field_0x16a4);
            local_184->mCamera.Start();
            local_184->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            i_this->field_0x169e = 0;
        }
        break;
    case 0x50:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        local_184->mCamera.Stop();
        local_184->mCamera.SetTrimSize(3);
        i_this->field_0x169e = 0x51;
        i_this->field_0x1714 = 55.0f;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1710 = 0.0f;
        a_this->current.pos.x = 1800.0f;
        a_this->current.pos.z = 0.0f;
        i_this->field_0x16bc.set(1631.0f, 195.0f, -224.0f);
        i_this->field_0x16a4.set(1233.0f, 167.0f, 195.0f);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        // fallthrough
    case 0x51:
        a_this->current.angle.y = 0x8000;
        a_this->shape_angle.y = 0x8000;
        i_this->mTurnStep = 0;
        cLib_addCalc2(&i_this->field_0x16bc.x, a_this->eyePos.x, 0.2f, 200.0f);
        cLib_addCalc2(&i_this->field_0x16bc.y, a_this->eyePos.y, 0.2f, 200.0f);
        cLib_addCalc2(&i_this->field_0x16bc.z, a_this->eyePos.z, 0.2f, 200.0f);
        if (i_this->field_0x16a0 > 20) {
            if (i_this->field_0x16a0 >= 52) {
                if (i_this->field_0x16a0 == 52) {
                    i_this->field_0x16a4 = a_this->current.pos;
                    i_this->field_0x16a4.x += 300.0f;
                    i_this->field_0x16a4.y += 150.0f;
                    i_this->field_0x16a4.z -= 1400.0f;
                }
            } else {
                i_this->field_0x16a4 = a_this->current.pos;
                i_this->field_0x16a4.y += 400.0f;
                i_this->field_0x16a4.z += 700.0f;
            }
        }
        if (i_this->field_0x16a0 == 140) {
            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[90]);
            dComIfGp_setNextStage("F_SP118", 0, 3, 0, 0.0f, 0, 1, 0, 0, 1, 0);
        }
        break;
    case 0x5a:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        local_184->mCamera.Stop();
        local_184->mCamera.SetTrimSize(3);
        i_this->field_0x169e = 0x5b;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1714 = 55.0;
        i_this->field_0x16a0 = 0;
        i_this->field_0x1710 = 0;
        daPy_getPlayerActorClass()->changeOriginalDemo();
        horse->changeOriginalDemo();
        a_this->current.pos.set(-93640.0f, a_this->current.pos.y, 44000.0f);
        a_this->shape_angle.y = 0x8000;
        a_this->current.angle.y = 0x8000;
        i_this->field_0x16bc = a_this->current.pos;
        i_this->field_0x16bc.y = a_this->eyePos.y + 200.0f;
        i_this->field_0x16a4 = a_this->current.pos;
        i_this->field_0x16a4.y += 300.0f;
        i_this->field_0x16a4.z += 500.0f;
        horse->changeDemoMode(1, 0);
        daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
        // fallthrough
    case 0x5b:
        cStack_48.set(-93335.0f, horse->current.pos.y, 44500.0f);
        horse->setHorsePosAndAngle(&cStack_48, 0);
        cLib_addCalc2(&i_this->field_0x16bc.x, a_this->eyePos.x, 0.2f,
                                   200.0f);
        cLib_addCalc2(&i_this->field_0x16bc.y, a_this->eyePos.y + 150.0f, 0.2f,
                                   200.0f);
        cLib_addCalc2(&i_this->field_0x16bc.z, a_this->eyePos.z, 0.2f,
                                   200.0f);
        i_this->field_0x16a4.z += a_this->speed.z * 0.8f;
        if (i_this->field_0x16a0 == 111) {
            i_this->field_0x169e = 0x5c;
            i_this->field_0x16a0 = 0;
            i_this->field_0x16a4 = a_this->current.pos;
            i_this->field_0x16a4.x += -300.0f;
            i_this->field_0x16a4.y += 100.0f;
            i_this->field_0x16a4.z += -1300.0f;
        }

        i_this->field_0x171c = 23.0f;
        if (!daPy_getPlayerActorClass()->checkHorseRide()) {
            local_184->mCamera.Start();
            local_184->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            i_this->field_0x169e = 0;
        }
        break;
    case 0x5c:
        cLib_addCalc2(&i_this->field_0x16bc.x, a_this->eyePos.x, 0.2f, 200.0f);
        cLib_addCalc2(&i_this->field_0x16bc.y, a_this->eyePos.y + 100.0f, 0.2f, 200.0f);
        cLib_addCalc2(&i_this->field_0x16bc.z, a_this->eyePos.z, 0.2f, 200.0f);
        if (i_this->field_0x16a0 != 60) {
            break;
        }

        i_this->field_0x169e = 0x5d;
        i_this->field_0x16a0 = 0;
        i_this->field_0x16a4 = a_this->current.pos;
        i_this->field_0x16a4.x += 100.0f;
        i_this->field_0x16a4.y += 600.0f;
        i_this->field_0x16a4.z += -500.0f;
        i_this->field_0x16bc = local_194->actor.current.pos;
        i_this->field_0x16bc.y = local_194->actor.current.pos.y + 100.0f;
        // fallthrough
    case 0x5d:
        if (local_194 != NULL) {
            cLib_addCalc2(&i_this->field_0x1710, 1000.0f, 1.0f, 100.0f);
            cLib_addCalc2(&i_this->field_0x16bc.x, local_194->actor.current.pos.x, 0.5f,
                          i_this->field_0x1710);
            cLib_addCalc2(&i_this->field_0x16bc.y, local_194->actor.current.pos.y + 100.0f, 0.5f,
                          i_this->field_0x1710);
            cLib_addCalc2(&i_this->field_0x16bc.z, local_194->actor.current.pos.z, 0.5f,
                          i_this->field_0x1710);
            cLib_addCalc2(&i_this->field_0x16a4.x, local_194->actor.current.pos.x - 10.0f, 0.05f,
                          12.0f);
            if (i_this->field_0x16a0 >= 40) {
                if (i_this->field_0x16a0 == 40) {
                    i_this->field_0x5c4 = local_194->actor.current.pos;
                }

                for (int i = 0; i < 2; i++) {
                    static u16 key_eno[2] = {
                        dPa_RM(ID_ZM_S_KEYLIGHT00),
                        dPa_RM(ID_ZM_S_KEYLIGHT01),
                    };
                    i_this->field_0x17d0[i] = dComIfGp_particle_set(
                        i_this->field_0x17d0[i], key_eno[i], &i_this->field_0x5c4, 0, 0);
                }
                local_3c = player->eyePos - i_this->field_0x5c4;
                cMtx_YrotS(*calc_mtx, cM_atan2s(local_3c.x, local_3c.z));
                cMtx_XrotM(*calc_mtx, -cM_atan2s(local_3c.y, JMAFastSqrt(
                    local_3c.x * local_3c.x + local_3c.z * local_3c.z)));
                local_3c.x = 0.0f;
                local_3c.y = 0.0f;
                local_3c.z = 30.0f;
                MtxPosition(&local_3c, &cStack_48);
                i_this->field_0x5c4 += cStack_48;
            }

            if (i_this->field_0x16a0 == 100) {
                i_this->field_0x169e = 0x5e;
                i_this->field_0x16a0 = 0;
                i_this->field_0x16a4.set(-93217.0f, -5783.0f, 44734.0f);
                i_this->field_0x16bc.set(-92351.0f, -4742.0f, 43295.0f);
                i_this->field_0x5c4 = i_this->field_0x16bc;
                i_this->field_0x1430 = 1;
                a_this->current.angle.y = player->shape_angle.y;
                fopAcM_delete(&local_194->actor);
            }
        }
        break;
    case 0x5e:
        for (int i = 0; i < 2; i++) {
            static u16 key_eno[2] = {
                dPa_RM(ID_ZM_S_KEYLIGHT00),
                dPa_RM(ID_ZM_S_KEYLIGHT01),
            };
            i_this->field_0x17d0[i] = dComIfGp_particle_set(i_this->field_0x17d0[i], key_eno[i],
                                                            &i_this->field_0x5c4, 0, 0);
        }
        local_3c = player->eyePos - i_this->field_0x5c4;
        if (local_3c.abs() < 50.0f) {
            i_this->field_0x169e = 0x5f;
            i_this->field_0x16a0 = 0;
            daPy_getPlayerActorClass()->changeDemoMode(0x3b, 0, 0, 0);
            i_this->field_0x5c4 = player->eyePos;
        } else {
            cMtx_YrotS(*calc_mtx, cM_atan2s(local_3c.x, local_3c.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(local_3c.y, JMAFastSqrt(local_3c.x * local_3c.x + local_3c.z * local_3c.z)));
            local_3c.x = 0.0f;
            local_3c.y = 0.0f;
            local_3c.z = 30.0f;
            MtxPosition(&local_3c, &cStack_48);
            i_this->field_0x5c4 += cStack_48;
        }
        // fallthrough
    case 0x5f:
        cLib_addCalc2(&i_this->field_0x16bc.x, i_this->field_0x5c4.x, 0.2f, 100.0f);
        cLib_addCalc2(&i_this->field_0x16bc.y, i_this->field_0x5c4.y, 0.2f, 100.0f);
        cLib_addCalc2(&i_this->field_0x16bc.z, i_this->field_0x5c4.z, 0.2f, 100.0f);
        if (i_this->field_0x169e == 95) {
            if (i_this->field_0x16a0 == 50) {
                daPy_getPlayerActorClass()->changeDemoMode(0xb, 0xf3, 0, 0);
            }
            if (i_this->field_0x16a0 >= 60) {
                cLib_addCalc2(&i_this->field_0x1714, 30.0f, 0.2f,
                                           1.5f);
            }
            if (i_this->field_0x16a0 >= 120 && !dMsgObject_isTalkNowCheck())
            {
                i_this->field_0x169e = 0x60;
                i_this->field_0x16a0 = 0;
                daPy_getPlayerActorClass()->changeDemoMode(1, 1, 0, 0);
                i_this->field_0x16bc.set(-93375.0f, -5700.0f, 44622.0f);
                i_this->field_0x16a4.set(-93153.0f, -5635.0f, 44717.0f);
                i_this->field_0x1714 = 55.0f;
                a_this->current.pos = player->current.pos;
                a_this->current.pos.z += 3000.0f;
            }
        }
        break;
    case 0x60:
        if (i_this->field_0x16a0 >= 100) {
            fopAc_ac_c* local_1b0 = fopAcM_SearchByName(PROC_NPC_COACH);
            a_this->current.pos = local_1b0->eyePos;
            a_this->current.pos.z = a_this->current.pos.z;
        }
        if (i_this->field_0x16a0 >= 220 && !dMsgObject_isTalkNowCheck())
        {
            i_this->field_0x169e = 0x61;
            i_this->field_0x16a0 = 0;
        }
        break;
    case 0x61:
        fopAc_ac_c* local_1b4 = fopAcM_SearchByName(PROC_NPC_COACH);
        a_this->current.pos = local_1b4->eyePos;
        a_this->current.pos.z = a_this->current.pos.z;
        if (i_this->field_0x16a0 == 120) {
            dComIfGp_setNextStage("F_SP121", 99, 13, 2, 0.0f, 0, 1, 0, 0, 1,
                                                  0);
        }
    }
    if (i_this->field_0x169e != 0) {
        cStack_60 = i_this->field_0x16bc;
        cStack_54 = i_this->field_0x16a4;
        s16 local_1d0 = 0;
        if (i_this->field_0x171c > 0.0f) {
            i_this->field_0x16bc.y += i_this->field_0x171c * cM_ssin(i_this->field_0x16a0 * 0x2400);
            local_1d0 = 10.0f *
                          (i_this->field_0x171c * cM_scos(i_this->field_0x16a0 * 0x2100));
            cLib_addCalc0(&i_this->field_0x171c, 1.0f, 1.5f);
        }
        local_184->mCamera.Set(cStack_60, cStack_54, local_1d0, i_this->field_0x1714);
        i_this->field_0x16a0++;
        if (i_this->field_0x16a0 > 10000) {
            i_this->field_0x16a0 = 10000;
        }

        if (i_this->field_0x169e < 10 || (i_this->field_0x169e >= 30 && i_this->field_0x169e < 60)) {
            if (dComIfGp_getEvent().checkSkipEdge()) {
                cDmr_SkipInfo = 0x28;
                Z2GetAudioMgr()->bgmStop(0, 0);
                if (i_this->field_0x169e < 10 || dComIfG_play_c::getLayerNo(0) == 4) {
                    dComIfGp_setNextStage("F_SP102", 1, 0, 4, 0.0f, 0, 1, 13,
                                                          0, 1, 0);
                    Z2GetAudioMgr()->bgmStreamStop(0x1e);
                    if (i_this->field_0x169e < 10) {
                        cDmr_SkipInfo = 0;
                    }
                    i_this->field_0x169e = 1000;
                } else if (i_this->field_0x169e < 50) {
                    dComIfGp_setNextStage("F_SP102", 0, 0, 0, 0.0f, 0, 1, 13,
                                                          0, 1, 0);
                    Z2GetAudioMgr()->bgmStreamStop(0x1e);
                    i_this->field_0x169e = 1000;
                } else {
                    cDmr_SkipInfo = 0;
                    mDoGph_gInf_c::fadeIn(0.066, g_blackColor);
                    i_this->mActionID = ACT_LR_DAMAGE;
                    anm_init(i_this, 0x12, 0.0f, 2, 1.0);
                    i_this->mActionMode = 3;
                    a_this->current.pos.x = 828.0f;
                    a_this->current.pos.z = 554.0f;
                    a_this->shape_angle.y = 0x800;
                    a_this->current.angle.y = 0x800;
                    rdb->mMode = 7;
                    rdb->field_0xfcf = 0;
                    rdb->field_0x6e0 = 0;
                    rdb->field_0x6d8 = 3;
                    local_184->mCamera.Reset(i_this->field_0x16bc, i_this->field_0x16a4);
                    local_184->mCamera.Start();
                    local_184->mCamera.SetTrimSize(0);
                    dComIfGp_event_reset();
                    daPy_getPlayerActorClass()->cancelOriginalDemo();
                    i_this->field_0x169e = 0;
                    Z2GetAudioMgr()->subBgmStart(Z2BGM_FACE_OFF_BATTLE3);
                }
            }
        }
    }
}

static void anm_se_eff_set(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    J3DModel* model = i_this->mpModelMorf->getModel();
    
    if (i_this->mAnmID == 12 || i_this->mAnmID == 13) {
        if (i_this->mpModelMorf->checkFrame(9.0f)) {
            i_this->mZ2Ride.startCreatureSound(Z2SE_CM_BODYFALL_L, 0, -1);
            i_this->field_0x1721 = 1;
        }
    } else if (i_this->mAnmID == 38) {
        i_this->field_0x17c4 = dComIfGp_particle_set(
            i_this->field_0x17c4, dPa_RM(ID_ZI_S_BB_STARTHANAIKI), 
            &a_this->current.pos, &a_this->tevStr);

        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x17c4);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(model->getAnmMtx(15));
        }
        
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            cXyz scale(a_this->scale.z, a_this->scale.z, a_this->scale.z);
            dComIfGp_particle_set(dPa_RM(ID_ZI_S_BB_STARTCLOD), &a_this->current.pos, 
                                 &a_this->tevStr, &a_this->shape_angle, &scale);
            dComIfGp_particle_set(dPa_RM(ID_ZI_S_BB_STARTFOOTMARK), &a_this->current.pos, 
                                 &a_this->tevStr, &a_this->shape_angle, &scale);
        }
    } else if (i_this->mAnmID == 5) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_CRASH, -1);
            i_this->mZ2Ride.startCreatureSound(Z2SE_EN_WB_RUNDOWN, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(15.0f)) {
            i_this->mZ2Ride.startCreatureSound(Z2SE_CM_BODYFALL_L, 0, -1);
        }
    } else if (i_this->mAnmID == 6) {
        if (i_this->mpModelMorf->checkFrame(2.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_UP, -1);
        }
    } else if (i_this->mAnmID == 7) {
        if (i_this->mpModelMorf->checkFrame(18.0f) || i_this->mpModelMorf->checkFrame(40.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_BREATH, -1);
        }
    } else if (i_this->mAnmID == 8) {
        if (i_this->mpModelMorf->checkFrame(4.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_INANAKI_DMG, -1);
        }
    } else if (i_this->mAnmID == 12) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_DOWN, -1);
        }
    } else if (i_this->mAnmID == 13) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_DOWN, -1);
        }
    } else if (i_this->mAnmID == 16) {
        if (i_this->mpModelMorf->checkFrame(12.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_BREATH, -1);
        }
    } else if (i_this->mAnmID == 17) {
        if (i_this->mpModelMorf->checkFrame(16.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_BREATH, -1);
        }
    } else if (i_this->mAnmID == 9) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_DAMAGE, -1);
        }
    } else if (i_this->mAnmID == 27) {
        if (i_this->mpModelMorf->checkFrame(14.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_INANAKI, -1);
        }
    } else if (i_this->mAnmID == 31) {
        if (i_this->mpModelMorf->checkFrame(2.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_RIDE, -1);
        }
    } else if (i_this->mAnmID == 32 || i_this->mAnmID == 33) {
        if (i_this->mpModelMorf->checkFrame(7.5f) || i_this->mpModelMorf->checkFrame(12.5f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_BREATH, -1);
        }
    } else if (i_this->mAnmID == 34) {
        if (i_this->mpModelMorf->checkFrame(3.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_DAMAGE, -1);
        }
    } else if (i_this->mAnmID == 40) {
        if (i_this->mpModelMorf->checkFrame(6.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_BREATH, -1);
        }
    } else if (i_this->mAnmID == 42) {
        if (i_this->mpModelMorf->checkFrame(11.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_BREATH, -1);
        }
    } else if (i_this->mAnmID == 43) {
        if (i_this->mpModelMorf->checkFrame(25.0f)) {
            i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_BREATH, -1);
        }
    }
    
    // Special sound effects for the leader Bulblin
    if (i_this->field_0x79d != 0) {
        if (i_this->mAnmID == 0x20 || i_this->mAnmID == 0x21) {
            if (i_this->mpModelMorf->checkFrame(4.0f)) {
                i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_BB_V_BREATH, -1);
            }
        } else if (i_this->mAnmID == 0x2B) {
            if (i_this->mpModelMorf->checkFrame(14.0f)) {
                i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_BB_V_BREATH, -1);
            }
        } else if (i_this->mAnmID == 0x2A) {
            if (i_this->mpModelMorf->checkFrame(6.0f)) {
                i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_BB_V_BREATH, -1);
            }
        } else if (i_this->mAnmID == 0x1B) {
            if (i_this->mpModelMorf->checkFrame(17.0f)) {
                i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_BB_V_INANAKI, -1);
            }
        }
    }
}

static int c_start;

static int daE_WB_Execute(e_wb_class* i_this) {    
    if (c_start == 0) {
        if (dComIfGp_event_runCheck()) {
            if (lbl_244_bss_46 != 0 || cDmrNowMidnaTalk()) {
                return 1;
            }
        }
    }
    
    if (i_this->field_0x169e == 1000) {
        return 1;
    }

    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz local_130;
    cXyz local_13c;
    
    a_this->scale.z = l_HIO.base_size;
    
    if (fopAcM_CheckCondition(a_this, fopAcCnd_NODRAW_e)) {
        i_this->field_0x5bf = 1;
    } else {
        i_this->field_0x5bf = 0;
    }
    
    if (i_this->field_0x79d != 0) {
        a_this->scale.z = a_this->scale.z * l_HIO.leader_size_ratio;
    }
    
    i_this->field_0x68e++;
    
    if (i_this->field_0x1430 == 0) {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
        i_this->mStatusFlags &= 3;
        
        for (int i = 0; i < 4; i++) {
            if (i_this->field_0x698[i] != 0) {
                i_this->field_0x698[i]--;
            }
        }
        
        if (i_this->field_0x6a0 != 0) {
            i_this->field_0x6a0--;
        }
        
        if (i_this->field_0x1432 != 0) {
            i_this->field_0x1432--;
        }
        
        action(i_this);
    }
    
    if (i_this->field_0x79d != 0 || i_this->field_0x169e != 0) {
        demo_camera(i_this);
    }
    
    if (i_this->field_0x1430 != 0) {
        a_this->eyePos = a_this->current.pos;
        return 1;
    }

    if (i_this->field_0x79d == 0 || i_this->mSpeedCapTimer == 0) {
        cXyz* move_p = i_this->mStts.GetCCMoveP();
        if (move_p != NULL) {
            f32 move_scale = TREG_F(19) + 0.3f;
            a_this->current.pos.x += move_p->x * move_scale;
            a_this->current.pos.y += move_p->y * move_scale;
            a_this->current.pos.z += move_p->z * move_scale;
        }
    }
    
    if ((i_this->field_0x5bf == 0) && !dComIfGp_event_runCheck()) {
        s16 wall_check = e_wb_wall_check(i_this);
        if (wall_check != 0 && wall_check > -0x2000 && wall_check < 0x2000) {
            if ((i_this->mActionID == ACT_C_F_RUN || i_this->mActionID == ACT_PL_RIDE2 ||
                    i_this->mActionID == ACT_A_RUN) &&
                (a_this->speedF >= 5.0f))
            {
                if (a_this->speedF > l_HIO.max_speed - 5.0f) {
                    if (i_this->mActionID == ACT_PL_RIDE2) {
                        dComIfGp_getVibration().StartShock(8, 0x4f, cXyz(0.0f, 1.0f, 0.0f));
                    }
                    i_this->mActionID = ACT_BG_DAMAGE;
                    i_this->mActionMode = 0;
                    i_this->mAnimDuration = 50.0f;
                    i_this->mZ2Ride.startCreatureVoice(Z2SE_EN_WB_V_DAMAGE, -1);
                } else {
                    if (i_this->mActionID != ACT_PL_RIDE2) {
                        if (i_this->mActionID == ACT_C_F_RUN) {
                            i_this->mActionID = ACT_F_WAIT;
                        } else {
                            i_this->mActionID = ACT_WAIT;
                        }
                        i_this->mActionMode = 0;
                    }
                }
                
                if (i_this->mActionID != ACT_PL_RIDE2) {
                    a_this->current.pos = a_this->old.pos;
                }
            }
            
            dBgS_LinChk linChk;
            cXyz pos1;
            pos1 = a_this->current.pos;
            pos1.y += 100.0f;
            linChk.Set(&pos1, &a_this->eyePos, a_this);
            
            if (dComIfG_Bgsp().LineCross(&linChk)) {
                cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
                local_130.x = 0.0f;
                local_130.y = 0.0f;
                local_130.z = KREG_F(11) + -20.0f;
                MtxPosition(&local_130, &local_13c);
                a_this->current.pos += local_13c;
                
                if (i_this->mActionID != ACT_PL_RIDE2) {
                    a_this->speedF = 0.0f;
                }
            }
        }
    }
    
    i_this->mAcch.CrrPos(dComIfG_Bgsp());
    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 4, 0x2000);
    
    if (i_this->mAcch.ChkGroundHit()) {
        dBgS_GndChk gndChk;
        Vec local_154;
        cXyz pos2;
        
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        local_130.x = 0.0f;
        local_130.y = 200.0f;
        local_130.z = 100.0f;
        MtxPosition(&local_130, &local_13c);
        local_13c += a_this->current.pos;
        
        local_130.z = -100.0f;
        MtxPosition(&local_130, &pos2);
        pos2 += a_this->current.pos;

        local_154.x = local_13c.x;
        local_154.y = local_13c.y;
        local_154.z = local_13c.z;

        gndChk.SetPos(&local_154);
        local_13c.y = dComIfG_Bgsp().GroundCross(&gndChk);
        
        local_154.x = pos2.x;
        local_154.y = pos2.y;
        local_154.z = pos2.z;

        gndChk.SetPos(&local_154);
        pos2.y = dComIfG_Bgsp().GroundCross(&gndChk);
        
        if (fabsf(local_13c.y - pos2.y) <= 200.0f) {
            a_this->current.angle.x = -cM_atan2s(local_13c.y - pos2.y, 200.0f);
        }
        
        a_this->speed.y = -20.0f;
    }
    
    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 4, 0x1000);
    cLib_addCalcAngleS2(&a_this->shape_angle.z, a_this->current.angle.z, 4, 0x1000);
    
    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y + i_this->field_0x7a4);
    mDoMtx_stack_c::XrotM((s16)a_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, 150.0f, 0.0f);
    mDoMtx_stack_c::YrotM((s16)i_this->field_0x6ae.y);
    mDoMtx_stack_c::XrotM((s16)i_this->field_0x6ae.x);
    mDoMtx_stack_c::transM(0.0f, -150.0f, -50.0f);
    
    f32 scale = a_this->scale.z;
    mDoMtx_stack_c::scaleM(scale, scale, scale);
    
    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    
    int soundParam;
    
    if (-G_CM3D_F_INF != i_this->mAcch.GetGroundH()) {
        if (i_this->mAcch.ChkWaterHit() && i_this->mAcch.m_wtr.GetHeight() > a_this->current.pos.y)
        {
            soundParam = dKy_pol_sound_get(&i_this->mAcch.m_wtr);
        } else if (i_this->mAcch.ChkGroundHit()) {
            soundParam = dKy_pol_sound_get(&i_this->mAcch.m_gnd);
        } else {
            soundParam = 0;
        }
    } else {
        soundParam = 0;
    }
    
    i_this->mpModelMorf->play(soundParam, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    i_this->mpModelMorf->modelCalc();
    
    anm_se_eff_set(i_this);
    
    if (i_this->mActionID != ACT_EVENT) {
        f32 size_scale = 1.0f;
        if (i_this->field_0x79d >= 2) {
            size_scale += JREG_F(15) + 1.1f;
        }
        
        MTXCopy(model->getAnmMtx(i_this->field_0x688 + 0xd), *calc_mtx);
        local_130.set(KREG_F(13) + -20.0f, KREG_F(14) + -30.0f, KREG_F(15));
        MtxPosition(&local_130, &a_this->eyePos);

        a_this->attention_info.position.set(
            a_this->current.pos.x - cM_ssin(a_this->shape_angle.y) * 30.0f,
            a_this->current.pos.y + 150.0f,
            a_this->current.pos.z - cM_scos(a_this->shape_angle.y) * 30.0f);

        i_this->field_0xa00[0].SetC(a_this->eyePos);
        i_this->field_0xa00[0].SetR((KREG_F(16) + 40.0f) * l_HIO.base_size * size_scale);
        
        u32 tg_group;
        if (i_this->mActionID == ACT_PL_RIDE) {
            tg_group = 4;
        } else {
            tg_group = 2;
        }
        
        for (int i = 0; i < 7; i++) {
            i_this->field_0xa00[i].SetTgGrp(tg_group);
        }
        
        dComIfG_Ccsp()->Set(&i_this->field_0xa00[0]);
        
        if (i_this->mPursuitFlag != 0) {
            
            local_130.set(KREG_F(10) + -30.0f, KREG_F(11) + -50.0f, KREG_F(12));
            MtxPosition(&local_130, &local_13c);
            i_this->field_0x1288.SetC(local_13c);
            i_this->field_0x1288.SetR((KREG_F(8) + 55.0f) * l_HIO.base_size);
            
            dComIfG_Ccsp()->Set(&i_this->field_0x1288);
            
            if ((i_this->mActionID == ACT_PL_RIDE) || (i_this->mActionID == ACT_PL_RIDE2)) {
                i_this->field_0x1288.SetAtType(AT_TYPE_NORMAL_SWORD);
            } else {
                i_this->field_0x1288.SetAtType(AT_TYPE_1000);
            }
            
            if (a_this->speedF > l_HIO.max_speed - 2.0f) {
                i_this->field_0x1288.SetAtSpl(dCcG_At_Spl_UNK_7);
                
                if (i_this->field_0x1288.ChkAtHit()) {
                    cCcD_Obj* hit_obj = i_this->field_0x1288.GetAtHitObj();
                    fopAc_ac_c* hit_actor = dCc_GetAc(hit_obj->GetAc());
                    
                    if (hit_actor != NULL) {
                        if (fopAcM_GetName(hit_actor) == PROC_E_RD) {
                            i_this->mZ2Ride.startCreatureSound(Z2SE_EN_WB_RUNDOWN, 0, -1);
                        } else if (fopAcM_GetName(hit_actor) == PROC_ALINK) {
                            if (daPy_getPlayerActorClass()->checkPlayerGuard()) {
                                dComIfGp_getVibration().StartShock(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                            }
                        }
                    }
                }
            } else {
                i_this->field_0x1288.SetAtSpl(dCcG_At_Spl_UNK_0);
            }
            
            if (daPy_getPlayerActorClass()->checkHorseRide()) {
                i_this->field_0x1288.OffAtVsPlayerBit();
            } else {
                i_this->field_0x1288.OnAtVsPlayerBit();
            }
            
            i_this->mPursuitFlag = 0;
        } else {
            cXyz temp_pos(-200000.0f, -200000.0f, 200000.0f);
            i_this->field_0x1288.SetC(temp_pos);
        }
        
        local_130.set(JREG_F(0) + -30.0f, JREG_F(1), JREG_F(2));
        MTXCopy(model->getAnmMtx(i_this->field_0x688 + 0xb), *calc_mtx);
        MtxPosition(&local_130, &local_13c);
        i_this->field_0xa00[1].SetC(local_13c);
        i_this->field_0xa00[1].SetR(l_HIO.base_size * 50.0f);
        
        MTXCopy(model->getAnmMtx(i_this->field_0x688 + 0x11), *calc_mtx);
        local_130.set(JREG_F(3) + 10.0f, JREG_F(4) + 20.0f, JREG_F(5));
        MtxPosition(&local_130, &local_13c);
        i_this->field_0xa00[2].SetC(local_13c);
        i_this->field_0xa00[2].SetR(l_HIO.base_size * 40.0f + 20.0f + JREG_F(6));
        
        dComIfG_Ccsp()->Set(&i_this->field_0xa00[1]);
        dComIfG_Ccsp()->Set(&i_this->field_0xa00[2]);
        
        local_130.set(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < 4; i++) {
            static int foot_no[4] = {5, 24, 9, 20};
            static int foot_no_B[4] = {5, 28, 9, 23};

            if (i_this->field_0x79d != 0) {
                MTXCopy(model->getAnmMtx(foot_no_B[i]), *calc_mtx);
            } else {
                MTXCopy(model->getAnmMtx(foot_no[i]), *calc_mtx);
            }
            
            MtxPosition(&local_130, &local_13c);
            i_this->field_0xa00[i + 3].SetC(local_13c);
            i_this->field_0xa00[i + 3].SetR(l_HIO.base_size * 20.0f);
            
            dComIfG_Ccsp()->Set(&i_this->field_0xa00[i + 3]);
        }
        
        for (int i = 0; i < 6; i++) {
            if (i_this->field_0x79d == 0) {
                i_this->field_0xa00[i + 1].OnTgSetBit();
            } else {
                i_this->field_0xa00[i + 1].OffTgSetBit();
            }
        }
    }
    
    i_this->field_0x1688 += i_this->field_0x168a;
    cLib_addCalcAngleS2(&i_this->field_0x168a, a_this->speedF * 50.0f + 1500.0f, 1, 0x3c);
    
    cLib_addCalc2(&i_this->mAnimDuration, a_this->speedF * (YREG_F(8) + 1.5f) + BREG_F(6), 1.0f, BREG_F(7) + 0.9f);
    
    i_this->field_0x1434 = fpcM_ERROR_PROCESS_ID_e;
    
    effect_set(i_this);
    i_this->field_0x6c0 = 0;

    return 1;
}

static int daE_WB_IsDelete(e_wb_class* i_this) {
    return 1;
}

static int daE_WB_Delete(e_wb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, i_this->mResName);
    if (i_this->field_0x17e0 != 0) {
        l_HIOInit = false;
        mDoHIO_DELETE_CHILD(l_HIO.mId);
    }

    if (a_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    e_wb_class* i_this = (e_wb_class*)a_this;

    i_this->mpModelMorf =
        new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes(i_this->mResName, 0x2e), NULL,
                             NULL, (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 0x28),
                             0, 1.0f, 0, -1, &i_this->mZ2Ride, 0x80000, 0x11000084);

    if (i_this->mpModelMorf == NULL || i_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setUserArea((uintptr_t)i_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x79d != 0) {
            int iVar6 = i_this->field_0x15d0[i].init(
                1, 0x10, (ResTIMG*)dComIfG_getObjectRes("Always", 0x54), 1);

            if (iVar6 == 0) {
                return 0;
            }
        } else {
            int iVar6 = i_this->field_0x15d0[i].init(
                1, 0x10, (ResTIMG*)dComIfG_getObjectRes("Always", 0x54), 1);

            if (iVar6 == 0) {
                return 0;
            }
        }

        f32* size = i_this->field_0x15d0[i].getSize(0);
        for (int j = 0; j < 16; j++) {
            if (i_this->field_0x79d != 0) {
                *size = 5.0f + PREG_S(0);
            } else {
                *size = 3.0f + PREG_S(1);
            }
            size++;
        }
    }

    if (i_this->field_0x79d != 0) {
        int iVar6 =
            i_this->field_0x1648.init(1, 2, (ResTIMG*)dComIfG_getObjectRes("Always", 0x54), 1);

        if (iVar6 == 0) {
            return 0;
        }
    } else {
        int iVar6 =
            i_this->field_0x1648.init(1, 2, (ResTIMG*)dComIfG_getObjectRes("Always", 0x54), 1);
        if (iVar6 == 0) {
            return 0;
        }
    }

    f32* size = i_this->field_0x1648.getSize(0);

    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x79d != 0) {
            *size = 5.0f + PREG_S(0);
        } else {
            *size = 3.0f + PREG_S(1);
        }
        size++;
    }

    return 1;
}

static int daE_WB_Create(fopAc_ac_c* a_this) {
    fopAcM_ct(a_this, e_wb_class);
    e_wb_class* i_this = (e_wb_class*)a_this;

    OS_REPORT("//////////////E_WB SET 555 !!\n");

    fopAcM_GetParam((fopAc_ac_c*)i_this);
    i_this->mParam = fopAcM_GetParam(i_this);

    if (i_this->mParam == 5 || i_this->mParam == 7 || i_this->mParam == 8 || i_this->mParam == 9) {
        i_this->mResName = "E_bb";
    } else {
        i_this->mResName = "E_wb";
    }

    int phase = dComIfG_resLoad(&i_this->mPhase, i_this->mResName);
    lbl_244_bss_46 = 0;
    lbl_244_bss_45 = 0;
    lbl_244_bss_47 = 0;

    // Title Screen
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP102")) {
        lbl_244_bss_45 = 2;
    }

    // Hyrule Field, Bridge of Eldin area
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP121") && fopAcM_GetRoomNo(a_this) == 0) {
        lbl_244_bss_46 = 1;
    }

    if (phase == cPhs_COMPLEATE_e) {
        i_this->mActionID = ACT_WAIT;

        if (i_this->mParam == 0xff) {
            i_this->mParam = 0;
        } else if (i_this->mParam == 5) {
            i_this->field_0x79d = 1;
            i_this->mActionID = ACT_B_WAIT;
        } else if (i_this->mParam == 7) {
            i_this->field_0x79d = 2;
            i_this->mActionID = ACT_B_IKKI;

            if (cDmr_SkipInfo == 0) {
                i_this->field_0x169e = 0x28;
                arrow_rd_set(i_this);
            } else {
                Z2GetAudioMgr()->subBgmStart(Z2BGM_FACE_OFF_BATTLE2);
                i_this->field_0x1722 = 1;
            }
        } else if (i_this->mParam == 8) {
            i_this->field_0x79d = 3;
            i_this->mActionID = ACT_B_IKKI2;

            // Hyrule Field
            if (!strcmp(dComIfGp_getStartStageName(), "F_SP121")) {
                i_this->mActionMode = -100;
            }

            i_this->mTargetFacingAngle = 0x8000;
        } else if (i_this->mParam == 6) {
            i_this->mActionID = ACT_EVENT;
        } else if (i_this->mParam == 10) {
            i_this->mActionID = ACT_C_F_RUN;
            i_this->field_0x7a6 = 1;
            i_this->field_0x7a7 = 1;

            if (fopAcM_gc_c::gndCheck(&a_this->current.pos)) {
                a_this->current.pos.y = fopAcM_gc_c::getGroundY();
            }

            OS_REPORT(" C_RD RESET  00\n");
        } else if (i_this->mParam == 9) {
            i_this->field_0x79d = 4;
            i_this->mActionID = ACT_B_LV9_END;
            i_this->mActionMode = 0;
        }

        i_this->mParam2 = fopAcM_GetParam(i_this) >> 8;

        if (i_this->field_0x79d != 0) {
            i_this->field_0x688 = 2;
            cXyz local_38;
            cXyz local_44;
            local_38.x = 0.0f;
            local_38.y = 1000.0f;

            for (int i = 0; i < 8; i++) {
                cMtx_YrotS(*calc_mtx, i << 0xd);

                if (lbl_244_bss_45 == 1) {
                    local_38.z = cM_rndF(3000.0f) + 12000.0f;
                    MtxPosition(&local_38, &local_44);
                } else {
                    local_38.z = 13000.0f;
                    MtxPosition(&local_38, &local_44);
                    local_44.x += -2989.0f;
                    local_44.z += 1364.0f;
                }

                b_path[i] = local_44;
            }

            if (i_this->field_0x79d == 1 || i_this->field_0x79d == 2) {
                s8 room_no = fopAcM_GetRoomNo(a_this);
                fopAcM_createChild(PROC_OBJ_WFLAG, fopAcM_GetID(i_this), 0, &a_this->current.pos,
                                   room_no, 0, 0, -1, 0);
            }

            static f32 pass_r[6] = {
                0.0f, 800.0f, 800.0f, -100.0f, -150.0f, -100.0f,
            };

            static f32 x_check_off[6] = {
                600.0f, -800.0f, 800.0f, -300.0f, 0.0f, 300.0f,
            };

            i_this->field_0x7a8 = pass_r[i_this->mParam2];
            i_this->field_0x6c4 = x_check_off[i_this->mParam2];
        }

        u32 local_50;

        if (i_this->field_0x79d != 0) {
            local_50 = 0x4300;
        } else {
            local_50 = 0x3740;
        }

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, local_50)) {
            OS_REPORT("//////////////E_WB SET NON !!\n");
            return cPhs_ERROR_e;
        } else {
            if (!l_HIOInit) {
                i_this->field_0x17e0 = 1;
                l_HIOInit = true;
                l_HIO.mId = mDoHIO_CREATE_CHILD("イノシシ", &l_HIO);
            }

            fopAcM_OnStatus(a_this, 0);

            a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            a_this->attention_info.distances[fopAc_attn_ETC_e] = 0xe;

            fopAcM_SetMtx(a_this, i_this->mpModelMorf->getModel()->getBaseTRMtx());
            fopAcM_SetMin(a_this, -800.0f, -400.0f, -800.0f);
            fopAcM_SetMax(a_this, 800.0f, 400.0f, 800.0f);

            fopAcM_setCullSizeFar(a_this, 30000.0f);

            i_this->mAcch.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this,
                              1, &i_this->field_0x7ac, fopAcM_GetSpeed_p(a_this), 0, 0);
            i_this->field_0x7ac.SetWall(100.0f, 100.0f + BREG_F(8));
            i_this->mAcch.ClrWaterNone();

            a_this->health = 10;
            a_this->field_0x560 = 10;

            if (i_this->field_0x79d != 0) {
                i_this->mStts.Init(0xfe, 0, a_this);
            } else {
                i_this->mStts.Init(0xfd, 0, a_this);
            }

            for (int i = 0; i <= 6; i++) {
                static dCcD_SrcSph cc_sph_src = {
                    {
                        {0x0, {{0x0, 0x0, 0x0}, {0x486022, 0x3}, 0x75}},  // mObj
                        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},               // mGObjAt
                        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},               // mGObjTg
                        {0x0},                                            // mGObjCo
                    },                                                    // mObjInf
                    {
                        {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
                    }  // mSphAttr
                };

                i_this->field_0xa00[i].Set(cc_sph_src);
                i_this->field_0xa00[i].SetStts(&i_this->mStts);

                if (i_this->field_0x79d != 0) {
                    i_this->field_0xa00[i].OnTgShield();
                    i_this->field_0xa00[i].SetTgHitMark(CcG_Tg_UNK_MARK_2);
                }
            }

            static dCcD_SrcSph at_sph_src = {
                {
                    {0x0, {{AT_TYPE_1000, 0x2, 0x1f}, {0x0, 0x0}, 0x0}},  // mObj
                    {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0},              // mGObjAt
                    {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                   // mGObjTg
                    {0x0},                                                // mGObjCo
                },                                                        // mObjInf
                {
                    {{0.0f, 0.0f, 0.0f}, 35.0f}  // mSph
                }  // mSphAttr
            };

            i_this->field_0x1288.Set(at_sph_src);
            i_this->field_0x1288.SetStts(&i_this->mStts);

            a_this->attention_info.distances[fopAc_attn_CARRY_e] = 0x28;
            a_this->attention_info.distances[fopAc_attn_LOCK_e] = 0x16;
            a_this->attention_info.distances[fopAc_attn_ETC_e] = 0x30;
            a_this->attention_info.flags = 0;
            i_this->field_0x68e = (s16)cM_rndF(65535.0f);

            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 16; j++) {
                    i_this->field_0x1450[i].field_0x0[j] = a_this->current.pos;
                }
            }

            i_this->field_0x1438[0] = a_this->current.pos;
            i_this->field_0x1438[1] = i_this->field_0x1438[0];

            i_this->mZ2Ride.init(&a_this->current.pos, &a_this->eyePos, 6, 1);
            i_this->field_0x13c0.mpSound = &i_this->mZ2Ride;

            // Bulblin Camp
            if (!strcmp(dComIfGp_getStartStageName(), "F_SP118")) {
                s16 room_no = fopAcM_GetRoomNo(a_this);

                if (room_no == 1) {
                    a_this->current.pos.set(1500.0f, 0.0f, 1100.0f);
                    a_this->current.angle.y = 0x2000;
                    i_this->mActionID = ACT_CRV_WAIT;
                    i_this->field_0x1720 = 1;
                } else {
                    room_no = fopAcM_GetRoomNo(a_this);

                    if (room_no == 2) {
                        i_this->mActionID = ACT_LR_DAMAGE;
                        anm_init(i_this, 0x12, 0.0f, 2, 1.0f);
                        i_this->mActionMode = 3;
                        a_this->current.pos.x = 828.0f;
                        a_this->current.pos.z = 554.0f;
                        a_this->shape_angle.y = 0x800;
                        a_this->current.angle.y = 0x800;
                        i_this->field_0x1720 = 1;
                    }
                }
            }

            if (i_this->mParam == 10) {
                lbl_244_bss_47 = 1;
            }

            fopAcM_setCullSizeFar(a_this, 30000.0f);
            i_this->mSpeedRate = l_HIO.mPlayerMountedMaxSpeed;
            c_start = 1;
            daE_WB_Execute(i_this);
            c_start = 0;

            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[88])) {
                i_this->mLapCount = 1;
            }
        }
    }

    return phase;
}

static actor_method_class l_daE_WB_Method = {
    (process_method_func)daE_WB_Create,  (process_method_func)daE_WB_Delete,
    (process_method_func)daE_WB_Execute, (process_method_func)daE_WB_IsDelete,
    (process_method_func)daE_WB_Draw,
};

extern actor_process_profile_definition g_profile_E_WB = {
    fpcLy_CURRENT_e,         // mLayerID
    4,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_E_WB,               // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(e_wb_class),      // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    152,                     // mPriority
    &l_daE_WB_Method,        // sub_method
    0x00044100,              // mStatus
    fopAc_ENEMY_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
#include "JSystem/JAudio2/JAUSectionHeap.h"
template<>
JAUSectionHeap* JASGlobalInstance<JAUSectionHeap>::sInstance;
