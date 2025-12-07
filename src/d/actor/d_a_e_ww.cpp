/**
 * @file d_a_e_ww.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_ww.h"
#include "f_op/f_op_camera_mng.h"
#include "d/d_debug_viewer.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_enemy.h"
#include "Z2AudioLib/Z2Instances.h"

enum E_ww_RES_File_ID {
    /* BCK */
    /* 0x04 */ BCK_WW_APPEAR = 0x4,
    /* 0x05 */ BCK_WW_BACKSTEP,
    /* 0x06 */ BCK_WW_DAMAGE,
    /* 0x07 */ BCK_WW_DEAD,
    /* 0x08 */ BCK_WW_JUMPATTACKA,
    /* 0x09 */ BCK_WW_JUMPATTACKB,
    /* 0x0A */ BCK_WW_JUMPATTACKC,
    /* 0x0B */ BCK_WW_RUN,
    /* 0x0C */ BCK_WW_SIDESTEPL,
    /* 0x0D */ BCK_WW_SIDESTEPR,
    /* 0x0E */ BCK_WW_TURN,
    /* 0x0F */ BCK_WW_WAIT,
    /* 0x10 */ BCK_WW_WALK,

    /* BMDR */
    /* 0x13 */ BMDR_WW = 0x13,
};

enum Joint {
    /* 0x00 */ JNT_WORLD_ROOT,
    /* 0x01 */ JNT_BODY01,
    /* 0x02 */ JNT_BODY02,
    /* 0x03 */ JNT_NECK,
    /* 0x04 */ JNT_HEAD,
    /* 0x05 */ JNT_AGO,
    /* 0x06 */ JNT_EARL,
    /* 0x07 */ JNT_EARR,
    /* 0x08 */ JNT_SHOULDERL,
    /* 0x09 */ JNT_ARML01,
    /* 0x0A */ JNT_ARML02,
    /* 0x0B */ JNT_HANDL01,
    /* 0x0C */ JNT_HANDL02,
    /* 0x0D */ JNT_SHOULDERR,
    /* 0x0E */ JNT_ARMR01,
    /* 0x0F */ JNT_ARMR02,
    /* 0x10 */ JNT_HANDR01,
    /* 0x11 */ JNT_HANDR02,
    /* 0x12 */ JNT_WAIST,
    /* 0x13 */ JNT_LEGL01,
    /* 0x14 */ JNT_LEGL02,
    /* 0x15 */ JNT_LEGL03,
    /* 0x16 */ JNT_FOOTL,
    /* 0x17 */ JNT_LEGR01,
    /* 0x18 */ JNT_LEGR02,
    /* 0x19 */ JNT_LEGR03,
    /* 0x1A */ JNT_FOOTR,
    /* 0x1B */ JNT_TAIL01,
    /* 0x1C */ JNT_TAIL02,
};

enum Action {
    /* 0x0 */ ACTION_MASTER,
    /* 0x1 */ ACTION_WAIT,
    /* 0x2 */ ACTION_ATTACK,
    /* 0x3 */ ACTION_CHASE,
    /* 0x4 */ ACTION_DAMAGE,
    /* 0x5 */ ACTION_MOVE_OUT,
    /* 0x6 */ ACTION_WALK,
};

enum Action_Mode {
    /* 0x00 */ ACTION_MODE_0,
    /* 0x01 */ ACTION_MODE_1,
    /* 0x02 */ ACTION_MODE_2,
    /* 0x03 */ ACTION_MODE_3,
    /* 0x05 */ ACTION_MODE_5 = 0x5,
    /* 0x06 */ ACTION_MODE_6,
    /* 0x0A */ ACTION_MODE_10 = 0xA,
    /* 0x0B */ ACTION_MODE_11,
    /* 0x0C */ ACTION_MODE_12,
    /* 0x0D */ ACTION_MODE_13,
    /* 0x0E */ ACTION_MODE_14,
    /* 0x0F */ ACTION_MODE_15,
    /* 0x10 */ ACTION_MODE_16,
    /* 0x14 */ ACTION_MODE_20 = 0x14,
    /* 0x15 */ ACTION_MODE_21,
    /* 0x16 */ ACTION_MODE_22,
    /* 0x19 */ ACTION_MODE_25 = 0x19,
    /* 0x1A */ ACTION_MODE_26,
    /* 0x1B */ ACTION_MODE_27,
    /* 0x1C */ ACTION_MODE_28,
    /* 0xC8 */ ACTION_MODE_200 = 0xC8,
};

class daE_WW_HIO_c : public JORReflexible {
public:
    daE_WW_HIO_c();
    virtual ~daE_WW_HIO_c() {}

    void genMessage(JORMContext*);

    /* ホワイトウルフォス - White Werewolf */
    /* 0x04 */ s8 field_0x04;
    /* 0x08 */ f32 model_size;              // モデルサイズ - Model Size      
    /* 0x0C */ f32 attack_interval;         // 攻撃間隔 - Attack Interval
    /* 0x10 */ f32 run_anm;                 // 走りアニメーション - Run Animation
    /* 0x14 */ f32 walk_anm;                // 歩きアニメーション - Walk Animation
    /* 0x18 */ f32 run_speed;               // 走る速度 - Run Speed
    /* 0x1C */ f32 wolf_escape_speed;       // 狼から逃亡速度 - Wolf Escape Speed
    /* 0x20 */ f32 link_escape_speed;       // リンクから逃亡速度 - Link Escape Speed
    /* 0x24 */ u8 move_range_debug_display; // 移動範囲デバック表示 - Move Range Debug Display
};

namespace {
    /* 807EF904-807EF944 000038 0040+00 1/1 0/0 0/0 .data            cc_ww_src__22@unnamed@d_a_e_ww_cpp@ */
    dCcD_SrcSph cc_ww_src = {
        {
            {0x0, {{0x400, 0x1, 0xC}, {0x0, 0x0}, 0x75}}, // mObj
            {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };
    
    /* 807EF944-807EF984 000078 0040+00 1/1 0/0 0/0 .data cc_ww_tg_src__22@unnamed@d_a_e_ww_cpp@ */
    dCcD_SrcSph cc_ww_tg_src = {
        {
            {0x0, {{0x0, 0x1, 0x0}, {0xD8FBFDFF, 0x43}, 0x0}}, // mObj
            {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };
} // namespace

static u8 l_HIOInit;

static daE_WW_HIO_c l_HIO;

static u8 lbl_237_bss_30; // What name for this variable ?

daE_WW_HIO_c::daE_WW_HIO_c() {
    field_0x04 = -1;
    model_size = 1.0f;
    attack_interval = 150.0f;
    run_anm = 1.2f;
    walk_anm = 0.5f;
    run_speed = 40.0f;
    wolf_escape_speed = 25.0f;
    link_escape_speed = 40.0f;
    move_range_debug_display = 0;
}

#if DEBUG
void daE_WW_HIO_c::genMessage(JORMContext* mctx) {
    mctx->genLabel("ホワイトウルフォス", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genSlider("モデルサイズ", &model_size, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genSlider("攻撃間隔", &attack_interval, 0.0f, 2000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genSlider("走りアニメーション", &run_anm, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genSlider("歩きアニメーション", &walk_anm, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genSlider("走る速度", &run_speed, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genSlider("狼から逃亡速度", &wolf_escape_speed, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genSlider("リンクから逃亡速度", &link_escape_speed, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genCheckBox("移動範囲デバック表示", &move_range_debug_display, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
}
#endif

int daE_WW_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* _joint = i_joint;

    int jnt_no = _joint->getJntNo();;
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));

    switch (jnt_no) {
    case JNT_NECK:
    case JNT_HEAD:
        mDoMtx_stack_c::YrotM(field_0x67c);
        mDoMtx_stack_c::ZrotM(field_0x67e);
    }

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    return 1;
}

int daE_WW_c::JointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model_p = j3dSys.getModel();
        daE_WW_c* i_this = (daE_WW_c*)model_p->getUserArea();
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model_p);
        }
    }

    return 1;
}

void daE_WW_c::setHeadAngle() {
    cXyz player_eye_pos = daPy_getPlayerActorClass()->eyePos;
    cXyz temp_r1; // Zero position ? effpos effective pos ?
    if (mAction != ACTION_MASTER) {
        if (field_0x75a != 0) {
            mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(JNT_HEAD));
            mDoMtx_stack_c::multVecZero(&temp_r1);
            s16 var_r29 = cLib_targetAngleY(&temp_r1, &player_eye_pos) - shape_angle.y;
            if (var_r29 > 0x4000) {
                var_r29 = 0x4000;
            }

            if (var_r29 < -0x4000) {
                var_r29 = -0x4000;
            }

            cLib_addCalcAngleS(&field_0x67c, (int)var_r29/2, 4, 0x400, 0x100);
            s16 var_r28 = cLib_targetAngleX(&temp_r1, &player_eye_pos) - shape_angle.x;
            if (var_r28 > 0x3000) {
                var_r28 = 0x3000;
            }

            if (var_r28 < -0x3000) {
                var_r28 = -0x3000;
            }

            cLib_addCalcAngleS(&field_0x67a, (int)var_r28/2, 4, 0x400, 0x100);
            return;
        }

        cLib_addCalcAngleS(&field_0x67c, 0, 4, 0x400, 0x100);
        cLib_addCalcAngleS(&field_0x67a, 0, 4, 0x400, 0x100);
    }
}

int daE_WW_c::draw() {
    if (mAction == ACTION_MASTER) {
        #if DEBUG
        if (l_HIO.move_range_debug_display != 0) {
            cXyz curr_pos = current.pos;
            curr_pos.y += 100.0f;
            GXColor circle1Color = { 200, 0, 0, 255 };
            dDbVw_drawCircleOpa(curr_pos, field_0x6a8, circle1Color, 1, 0xC);
            curr_pos.y = current.pos.y - field_0x6ac;
            GXColor circle2Color = { 0, 0, 200, 255 };
            dDbVw_drawCircleOpa(curr_pos, field_0x6a8, circle2Color, 1, 0xC);
            curr_pos.y = current.pos.y + field_0x6ac;
            GXColor circle3Color = { 0, 0, 200, 255 };
            dDbVw_drawCircleOpa(curr_pos, field_0x6a8, circle3Color, 1, 0xC);
        }
        #endif
        return 1;
    }
    
    J3DModel* model_p = mpModelMorf->getModel();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model_p, &tevStr);
    mpModelMorf->entryDL();
    cXyz sp14;
    sp14.set(current.pos.x, 100.0f + current.pos.y, current.pos.z);
    field_0x6d0 = dComIfGd_setShadow(field_0x6d0, 1, model_p, &sp14, 700.0f + nREG_F(19), 0.0f, current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static int daE_WW_Draw(daE_WW_c* i_this) {
    return static_cast<daE_WW_c*>(i_this)->draw();
}

void daE_WW_c::setBck(int i_index, u8 i_attr, f32 i_morf, f32 i_rate) {
    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_WW", i_index), i_attr, i_morf, i_rate, 0.0f, -1.0f);
}

bool daE_WW_c::checkBck(int i_index) {
    if (mpModelMorf->getAnm() == (J3DAnmTransform*)dComIfG_getObjectRes("E_WW", i_index)) {
        return 1;
    }

    return 0;
}

void daE_WW_c::setActionMode(int i_action_id, int i_attack_action_id) {
    gravity = -5.0f;
    mAction = i_action_id;
    mActionMode = i_attack_action_id;
    field_0x756 = 0;
    mObjAcch.ClrGrndNone();
    mObjAcch.OnLineCheck();
    mObjAcch.OffLineCheckNone();
}

void daE_WW_c::damage_check() {
    if (field_0x724 == 0) {
        mCcStts.Move();
        if (mSph1[1].ChkAtShieldHit() != 0) {
            mSph1[1].OffAtShieldHit();
            if (daPy_getPlayerActorClass()->checkPlayerGuard()) {
                setActionMode(ACTION_DAMAGE, ACTION_MODE_0);
                return;
            }
        }

        cCcD_Obj* var_r29 = NULL; // ObjHit ? Where damage is applied ?
        if (mSph2[0].ChkTgHit() != 0) {
            var_r29 = mSph2[0].GetTgHitObj();
        }

        if (mSph2[1].ChkTgHit() != 0) {
            var_r29 = mSph2[1].GetTgHitObj();
        }

        if (var_r29 != NULL) {
            mAtInfo.mpCollider = var_r29;
            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_IRON_BALL) != 0) {
                if (fopAcM_GetName(dCc_GetAc(mAtInfo.mpCollider->GetAc())) == PROC_Obj_Carry) {
                    health += (s16) 150;
                } else if (dComIfGp_checkPlayerStatus0(0, 0x400) != 0) {
                    health += (s16) 180;
                } else {
                    health = 0;
                }
            }

            cc_at_check(this, &mAtInfo);
            if (mAtInfo.mpCollider->GetAtAtp() >= 1) {
                cXyz temp_r1;
                mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(JNT_HEAD));
                mDoMtx_stack_c::transM(-10.0f, -20.0f, 0.0f);
                mDoMtx_stack_c::multVecZero(&temp_r1);

                if (mAtInfo.mHitStatus == 0) {
                    dComIfGp_setHitMark(1, this, &temp_r1, NULL, NULL, 0);
                } else {
                    dComIfGp_setHitMark(3, this, &temp_r1, NULL, NULL, 0);
                }
            }

            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK) != 0) {
                field_0x724 = 20;
            } else {
                field_0x724 = 10;
            }

            if (mAtInfo.mAttackPower <= 1) {
                field_0x724 = (u8)(KREG_S(8) + 10);
            }

            setActionMode(ACTION_DAMAGE, ACTION_MODE_0);
        }
    }
}

void daE_WW_c::setRandamNumber() {
    int var_r27 = 0;
    int var_r26 = field_0x6b6;
    if (field_0x6b6 < 5) {
        var_r26 = 5;
    }

    for (int i=0; i<var_r26; i++){
        int var_r29 = cM_rndF(var_r26 - 0.1f);
        if ((var_r27 & (1 << var_r29)) != 0) {
            var_r29 = cM_rndF(var_r26 - 0.1f);
            if ((var_r27 & (1 << var_r29)) != 0) {
                for (int j=0; j<var_r26; j++){
                    if ((var_r27 & (1 << j)) == 0) {
                        var_r29 = j;
                        break;
                    }
                }
            }
        }

        var_r27 |= 1 << var_r29;
        field_0x6fc[i] = var_r29;
        field_0x6d4[i] = 8.0f + cM_rndFX(3.0f);
    }
}

s16 daE_WW_c::getNearPlayerAngle() {
    s16 player_angle = fopAcM_searchPlayerAngleY(this);
    s16 angle_y = player_angle - shape_angle.y;
    if (abs(angle_y) >= 0x1800) {
        if (angle_y < 0) {
            return player_angle + 0x1800;
        }
        return player_angle - 0x1800;
    }

    return shape_angle.y;
}

void daE_WW_c::setGroundAngle() {
    if (field_0x75b != 0 && !mObjAcch.ChkGroundHit()) {
        cLib_addCalcAngleS(&field_0x674.x, 0, 4, 0x1000, 0x200);
        cLib_addCalcAngleS(&field_0x674.z, 0, 4, 0x1000, 0x200);
        cLib_addCalcAngleS(&field_0x6ce, 0, 4, 0x1000, 0x200);
        return;
    }

    cXyz sp14 = current.pos;
    sp14.y = mObjAcch.GetGroundH();
    dBgS_GndChk gnd_chk;
    Vec gnd_pos;
    
    if (field_0x75c == 0) {
        gnd_pos.x = sp14.x + 100.0f * cM_ssin(shape_angle.y);
        gnd_pos.y = 150.0f + sp14.y;
        gnd_pos.z = sp14.z + 100.0f * cM_scos(shape_angle.y);
        gnd_chk.SetPos(&gnd_pos);
        gnd_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
        if (-G_CM3D_F_INF == gnd_pos.y) {
            gnd_pos.y = (f32) sp14.y;
        }

        if (std::abs(gnd_pos.y - sp14.y) > 150.0f) {
            if (gnd_pos.y > sp14.y) {
                gnd_pos.y = sp14.y + 150.0f;
            } else {
                gnd_pos.y = sp14.y - 150.0f;
            }
        }

        field_0x6ce = -cLib_targetAngleX(&sp14, (cXyz*)&gnd_pos);
        field_0x674.x = 0;
        field_0x674.z = 0;
    } else {
        gnd_pos.x = sp14.x;
        gnd_pos.y = 150.0f + sp14.y;
        gnd_pos.z = 100.0f + sp14.z;
        gnd_chk.SetPos(&gnd_pos);
        gnd_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
        if (-G_CM3D_F_INF != gnd_pos.y) {
            field_0x674.x = -cM_atan2s(gnd_pos.y - sp14.y, gnd_pos.z - sp14.z);
        }

        gnd_pos.x = sp14.x + 50.0f;
        gnd_pos.y = sp14.y + 150.0f;
        gnd_pos.z = sp14.z;
        gnd_chk.SetPos(&gnd_pos);
        gnd_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
        if (-G_CM3D_F_INF != gnd_pos.y) {
            field_0x674.z = (s16)cM_atan2s(gnd_pos.y - sp14.y, gnd_pos.x - sp14.x);
        }

        field_0x6ce = 0;
    }
}

f32 daE_WW_c::checkCreateBg(cXyz i_vector) {
    cXyz sp14 = i_vector; // gnd_check_pos ?
    sp14.y += field_0x6ac;
    dBgS_GndChk gnd_chk;
    gnd_chk.SetPos(&sp14);
    f32 temp_f1 = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (temp_f1 != -G_CM3D_F_INF) {
        if (std::abs(temp_f1 - current.pos.y) > field_0x6ac) {
            return -G_CM3D_F_INF;
        }

        if (dComIfG_Bgsp().GetSpecialCode(gnd_chk) == 5 || dComIfG_Bgsp().GetPolyAtt0(gnd_chk) == 0xD) {
            cXyz temp_r1 = daPy_getPlayerActorClass()->current.pos;
            temp_r1.y += 100.0f;
            sp14 = i_vector;
            sp14.y = 100.0f + temp_f1;
            dBgS_LinChk lin_chk;
            lin_chk.Set(&temp_r1, &sp14, NULL);
            if (dComIfG_Bgsp().LineCross(&lin_chk) != 0) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
                return -G_CM3D_F_INF;
            }

            return temp_f1;
        }
    }

    return -G_CM3D_F_INF;
}

bool daE_WW_c::checkAttackWall() {
    cXyz curr_pos = current.pos;
    curr_pos.y += 100.0f;
    cXyz player_eye_pos = daPy_getPlayerActorClass()->eyePos;
    dBgS_LinChk line_chk;
    line_chk.Set(&curr_pos, &player_eye_pos, NULL);
    if (dComIfG_Bgsp().LineCross(&line_chk)) {
        return false;
    }

    return true;
}

void daE_WW_c::setBlurEffect() {
    mParticleKey = dComIfGp_particle_set(mParticleKey, 0x8602, &current.pos, &tevStr);
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey);
    if (emitter != NULL) {
        emitter->setGlobalSRTMatrix(mpModelMorf->getModel()->getAnmMtx(JNT_BODY02));
    }
}

void daE_WW_c::setAppearEffect() {
    dBgS_ObjGndChk_All gnd_chk;
    cXyz gnd_pos;
    gnd_pos.set(100.0f * cM_ssin(shape_angle.y), 100.0f, 100.0f * cM_scos(shape_angle.y));
    gnd_pos += current.pos;
    gnd_chk.SetPos(&gnd_pos);
    gnd_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (gnd_pos.y != -G_CM3D_F_INF) {
        csXyz sp10(0, 0, 0);
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(gnd_chk, &plane);
        cXyz* normal = plane.GetNP();
        sp10.y = (s16) cM_atan2s(normal->x, normal->z);
        sp10.x = (s16) cM_atan2s(normal->absXZ(), normal->y);
        dComIfGp_particle_setPolyColor(0x8A56, gnd_chk, &gnd_pos, &tevStr, &sp10, NULL, 0, NULL, -1, NULL);
        dComIfGp_particle_setPolyColor(0x8A57, gnd_chk, &gnd_pos, &tevStr, &sp10, NULL, 0, NULL, -1, NULL);
    }
}

static void* s_child_ww(void* i_actor, void* i_data) {
    if (i_actor != i_data && fopAcM_IsActor(i_actor)
    && fopAcM_GetName(i_actor) == PROC_E_WW && !fpcM_IsCreating(fopAcM_GetID(i_actor))) {
        if (fopAcM_GetLinkId((fopAc_ac_c*)i_actor) == fopAcM_GetID(i_data)) {
            lbl_237_bss_30++;
        }
    }
    return NULL;
}

static void* s_obj_sub(void* i_proc, void* i_data) {
    if (fopAcM_IsActor(i_proc) && !fpcM_IsCreating(fopAcM_GetID(i_proc)) && fopAcM_checkCarryNow((fopAc_ac_c *) i_proc) == 0) {
        f32 dist = fopAcM_searchActorDistance((fopAc_ac_c *) i_proc, (fopAc_ac_c *) i_data);
        if (dist < 100.0f) {
            f32 fVar2 = fopAcM_GetSpeedF((fopAc_ac_c *) i_proc);
            f32 fVar3 = 0.0f;
            if (fVar2 == fVar3) {
                f32 fVar4 = fopAcM_GetSpeed((fopAc_ac_c *) i_proc).y;
                if (fVar4 == fVar3) {
                    if (fopAcM_GetName(i_proc) == PROC_Obj_Carry) {
                        return i_proc;
                    }

                    if (fopAcM_GetName(i_proc) == PROC_NBOMB) {
                        return i_proc;
                    }
                }
            }
        }

        if (dist < 500.0f && (fopAcM_GetName(i_proc) == PROC_BOOMERANG || fopAcM_GetName(i_proc) == PROC_NBOMB)) {
            return i_proc;
        }
        
        if (dist < 700.0f && fopAcM_GetName(i_proc) == PROC_ARROW) {
            f32 fVar5 = fopAcM_GetSpeedF((fopAc_ac_c *) i_proc);
            f32 fVar6 = 0.0f;
            if (fVar5 != fVar6) {
                return i_proc;
            }
        }
    }

    return NULL;
}

bool daE_WW_c::checkSideStep() {
    cXyz* temp_r3;

    if (dComIfGp_checkPlayerStatus0(0, 0x4000) != 0) {
        temp_r3 = daPy_getPlayerActorClass()->getHookshotTopPos();
        if (temp_r3 != NULL && temp_r3->absXZ(current.pos) < 500.0f) {
            if ((s16)(cLib_targetAngleY(&current.pos, temp_r3) - shape_angle.y) < 0) {
                field_0x6c0 = 0;
            } else {
                field_0x6c0 = 1;
            }

            return true;
        }
    }

    fopAc_ac_c* temp_r3_2 = (fopAc_ac_c*) fpcM_Search(s_obj_sub, this);
    if (temp_r3_2 != NULL) {
        if ((s16)(fopAcM_searchActorAngleY(this, temp_r3_2) - shape_angle.y) < 0) {
            field_0x6c0 = 0;
        } else {
            field_0x6c0 = 1;
        }

        return true;
    }

    return false;
}

static void* s_attack_ww(void* i_actor, void* i_data) {
    if (i_actor != i_data && fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_WW && !fpcM_IsCreating(fopAcM_GetID(i_actor)) && ((daE_WW_c*)i_actor)->isAttack() != 0) {
        return i_actor;
    }

    return NULL;
}

void daE_WW_c::createWolf(cXyz param_0, u8 param_1) {
    fpc_ProcID var_r30;
    u8 var_r29;
    u8 var_r28;

    cXyz sp1C = daPy_getPlayerActorClass()->current.pos;
    csXyz sp14(0, cLib_targetAngleY(&param_0, &sp1C) + cM_rndFX(4096.0f), 0);
    u8 temp_r27 = field_0x6a8 / 100.0f;
    if (param_1 == 0) {
        var_r30 = fopAcM_GetLinkId(this);
        var_r29 = 0;
        var_r28 = 2;
    } else {
        var_r30 = fopAcM_GetID(this);
        var_r29 = field_0x6c0;
        var_r28 = 1;
    }

    mChildID[var_r29] = fopAcM_createChild(PROC_E_WW, var_r30, (var_r28 << 24) | (field_0x6b4 | (0xF0FF0000 | 0xF000) | (temp_r27 * 0x10)), &param_0, fopAcM_GetRoomNo(this), &sp14, NULL, -1, NULL);
}

static cXyz create_pos[15] = {
    cXyz(1000.0f, 0.0f, 200.0f),
    cXyz(-700.0f, 0.0f, 100.0f),
    cXyz(700.0f, 0.0f, 100.0f),
    cXyz(-1000.0f, 0.0f, 200.0f),
    cXyz(-300.0f, 0.0f, -200.0f),
    cXyz(700.0f, 0.0f, -100.0f),
    cXyz(200.0f, 0.0f, 300.0f),
    cXyz(400.0f, 0.0f, -200.0f),
    cXyz(-700.0f, 0.0f, -100.0f),
    cXyz(-400.0f, 0.0f, 200.0f),
    cXyz(250.0f, 0.0f, 200.0f),
    cXyz(250.0f, 0.0f, -250.0f),
    cXyz(0.0f, 0.0f, 0.0f),
    cXyz(-250.0f, 0.0f, 250.0f),
    cXyz(-250.0f, 0.0f, -250.0f),
};

void daE_WW_c::executeMaster() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp48 = daPy_getPlayerActorClass()->current.pos;
    f32 temp_f30 = sp48.absXZ(current.pos);
    cXyz sp3C;
    cXyz sp30;
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x4000);
    field_0x724 = 10;
    switch (mActionMode) {
    case ACTION_MODE_0:
        if ((field_0x6b4 != 0 || !(std::abs(current.pos.y - sp48.y) > 500.0f)) && temp_f30 < field_0x6a8) {
            if (field_0x6b4 == 0) {
                sp30.set(0.0f, 0.0f, 2500.0f + nREG_F(11));
            } else {
                sp30.set(0.0f, 0.0f, 3000.0f);
            }

            cLib_offsetPos(&sp3C, &sp48, fopCamM_GetAngleY(camera), &sp30);
            if (current.pos.abs(sp3C) < field_0x6a8) {
                f32 temp_f31 = checkCreateBg(sp3C);
                if (-G_CM3D_F_INF != temp_f31) {
                    sp3C.y = temp_f31;
                    int var_r30 = 0;
                    lbl_237_bss_30 = 0;
                    fpcM_Search(s_child_ww, this);
                    if (lbl_237_bss_30 < field_0x6b6) {
                        var_r30 = field_0x6b6 - lbl_237_bss_30;
                        if (field_0x6b5 != 0xFF && var_r30 > (field_0x6b5 - field_0x6c8)) {
                            var_r30 = field_0x6b5 - field_0x6c8;
                        }
                    }

                    if (var_r30 > 0) {
                        field_0x65c = sp3C;
                        mActionMode = ACTION_MODE_1;
                        field_0x6b7 = var_r30;
                        field_0x6c0 = 0;
                        setRandamNumber();
                        field_0x728 = 0;
                        return;
                    }
                }

                mActionMode = ACTION_MODE_10;
                field_0x728 = 30;
            }
        }
        break;

    case ACTION_MODE_1:
        if (field_0x728 == 0) {
            int var_r30_2;
            if (field_0x6b4 == 0) {
                var_r30_2 = field_0x6fc[field_0x6c0] + 5;
            } else {
                var_r30_2 = field_0x6fc[field_0x6c0];
            }

            sp30 = create_pos[var_r30_2];
            sp30.x += cM_rndFX(200.0f);
            cLib_offsetPos(&sp3C, &field_0x65c, (s16)fopCamM_GetAngleY(camera), &sp30);
            f32 temp_f31_2 = checkCreateBg(sp3C);
            if (-G_CM3D_F_INF != temp_f31_2) {
                sp3C.y = temp_f31_2;
            } else {
                sp3C = field_0x65c;
            }

            createWolf(sp3C, 1);
            field_0x6c8++;
            field_0x6c0++;
            if (field_0x6c0 >= field_0x6b7) {
                mActionMode = ACTION_MODE_3;
                field_0x6c0 = 0;
                field_0x6c4 = 0;
                setRandamNumber();
                field_0x728 = 20;
                return;
            }

            field_0x728 = field_0x6d4[field_0x6c0];
        }
        break;

    case 2:
        if (field_0x728 == 0) {
            mActionMode = ACTION_MODE_3;
            field_0x6c0 = 0;
            field_0x6c4 = 0;
            setRandamNumber();
            field_0x728 = 0;
        }
        break;

    case 3:
        if (field_0x728 == 0) {
            if (field_0x6fc[field_0x6c4] < field_0x6b7) {
                fopAc_ac_c* sp8;
                fopAcM_SearchByID(mChildID[field_0x6fc[field_0x6c4]], &sp8);
                if (sp8 != NULL) {
                    ((daE_WW_c*)sp8)->setAttack();
                }

                field_0x6c0++;
                if (field_0x6c0 >= field_0x6b7) {
                    field_0x6c4 = 10;
                }
            }

            field_0x6c4++;
            if (field_0x6c4 >= 10) {
                mActionMode = ACTION_MODE_10;
                field_0x728 = 50;
                return;
            }

            if (field_0x6fc[field_0x6c4] < field_0x6b7) {
                field_0x728 = field_0x6d4[field_0x6c4] * 2;
            }
        }
        break;

    case 10:
        if (field_0x728 == 0) {
            mActionMode = ACTION_MODE_0;
        }

        if (field_0x6b5 != 0xFF && field_0x6c8 >= field_0x6b5) {
            fopAcM_delete(this);
        }
        break;
    }
}

void daE_WW_c::executeWait() {
    switch (mActionMode) {
    case ACTION_MODE_0:
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
        setBck(BCK_WW_APPEAR, J3DFrameCtrl::EMode_NONE, 0.0f, 1.0f);
        mActionMode = ACTION_MODE_1;
        break;

    case ACTION_MODE_1:
        if (mpModelMorf->checkFrame(3.0f)) {
            mSound.startCreatureSound(Z2SE_EN_WW_APPEAR, 0, -1);
            setAppearEffect();
        }

        if (mpModelMorf->checkFrame(15.0f) || mpModelMorf->checkFrame(20.0f)) {
            mSound.startCreatureSound(Z2SE_EN_WW_FOOTNOTE, 0, -1);
        }

        if (mpModelMorf->isStop()) {
            if (field_0x758 != 0) {
                mActionMode = ACTION_MODE_10;
                return;
            }
            
            setBck(BCK_WW_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            return;
        }
        break;

    case ACTION_MODE_10:
        BOOL var_r28 = TRUE;
        if (checkAttackWall() == 0) {
            var_r28 = FALSE;
        } else if (field_0x6b4 == 1) {
            if (cM_rnd() < 0.3f) {
                var_r28 = FALSE;
            }
        } else if (field_0x6b4 == 0) {
            if (std::abs(current.pos.y - daPy_getPlayerActorClass()->current.pos.y) > 500.0f) {
                var_r28 = FALSE;
            }
        }

        fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x4000);
        if (var_r28) {
            setActionMode(ACTION_ATTACK, ACTION_MODE_0);
            field_0x756 = 1;
            return;
        }

        field_0x734 = l_HIO.attack_interval + cM_rndFX(30.0f);
        setBck(BCK_WW_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        setActionMode(ACTION_CHASE, ACTION_MODE_10);
        break;
    }
}

int daE_WW_c::calcJumpSpeed() {
    f32 var_f31 = field_0x65c.y - current.pos.y;
    int var_r28 = 0;
    int var_r29 = 0;
    int unused = 0;
    if (var_f31 > 0.0f) {
        while (var_f31 >= 35.0f) {
            var_f31 -= 35.0f + 3.0f * var_r28;
            var_r28++;
            var_r29++;
        }
    } else {
        while (var_f31 <= -35.0f) {
            var_f31 += 35.0f + 3.0f * var_r28;
            var_r29++;
        }
    }

    speed.y = 35.0f + 3.0f * var_r28 + nREG_F(7);
    field_0x75b = 2;
    speedF = current.pos.absXZ(field_0x65c) / (17.0f + var_r29 + nREG_F(8));
    if (speedF < 20.0f + nREG_F(10)) {
        speedF = 20.0f + nREG_F(10);
    }

    gravity = -3.0f + nREG_F(9);
    return var_r29; // Jump speed ?
}

void daE_WW_c::executeAttack() {
    daPy_py_c* player_p = daPy_getPlayerActorClass();
    
    switch (mActionMode) {
    case ACTION_MODE_0:
        setBck(BCK_WW_RUN, J3DFrameCtrl::EMode_LOOP, 3.0f, l_HIO.run_anm); // Change BCK index to enum ?
        mActionMode = ACTION_MODE_1;
        field_0x728 = 10;
        speedF = l_HIO.run_speed;
        field_0x72c = 300;
        /* fallthrough */
    case ACTION_MODE_1:
        field_0x75a = 1;
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 4, 0x800, 0x100);
        current.angle.y = shape_angle.y;
        if (checkAttackWall() == 0) {
            setActionMode(ACTION_CHASE, ACTION_MODE_2);
        }

        if (field_0x728 == 0) {
            if (checkMoveOut() != 0) {
                return;
            }

            if (fopAcM_searchPlayerDistance(this) < (800.0f + nREG_F(18) + mDistCheckModifier)) {
                mActionMode = ACTION_MODE_20;
                setBck(BCK_WW_JUMPATTACKA, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_WW_V_ATTACK, -1);
                speed.y = 0.0f;
                speedF = 0.0f;
            }
        }

        if (mpModelMorf->getFrame() <= 5.0f && checkSideStep() != 0) {
            mActionMode = ACTION_MODE_10;
            speed.y = 0.0f;
            speedF = 0.0f;
        }

        if (field_0x72c == 0 && fopAcM_otherBgCheck(this, daPy_getPlayerActorClass()) != 0) {
            setActionMode(ACTION_CHASE, ACTION_MODE_2);
            return;
        }
        break;
    
    case ACTION_MODE_10:
        field_0x6cc = fopAcM_searchPlayerAngleY(this);
        shape_angle.y = field_0x6cc;
        if (field_0x6c0 == 0) {
            setBck(BCK_WW_SIDESTEPL, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            current.angle.y = field_0x6cc + 0x1800 + cM_rndFX(2000.0f);
        } else {
            setBck(BCK_WW_SIDESTEPR, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            current.angle.y = field_0x6cc - 0x1800 + cM_rndFX(2000.0f);
        }
        mActionMode = ACTION_MODE_11;
        /* fallthrough */
    case ACTION_MODE_11:
        field_0x75a = 1;
        if (mpModelMorf->checkFrame(5.0f) != 0) {
            field_0x75b = 1;
            speed.y = 20.0f + nREG_F(15);
            speedF = nREG_F(16) + 40.0f + cM_rndFX(5.0f);
            mActionMode = ACTION_MODE_12;
            mSound.startCreatureSound(Z2SE_EN_WW_JUMP, 0, -1);
        }
        break;

    case ACTION_MODE_12:
        field_0x75a = 1;
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 4, 0x800, 0x100);
        if (mObjAcch.ChkGroundHit() != 0) {
            speedF = 0.0f;
            if (mpModelMorf->isStop() != 0) {
                if (checkMoveOut() != 0) {
                    return;
                }

                f32 var_f31 = 800.0f + nREG_F(18) + mDistCheckModifier;
                if (player_p->getSpeedF() >= 18.0f) {
                    var_f31 = 1000.0f + nREG_F(18) + mDistCheckModifier;
                }
                
                if (fopAcM_searchPlayerDistance(this) < var_f31 && checkAttackWall() != 0) {
                    mActionMode = ACTION_MODE_20;
                    setBck(BCK_WW_JUMPATTACKA, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f); // Change to ANM_JUMP ?
                    mSound.startCreatureVoice(Z2SE_EN_WW_V_ATTACK, -1);
                } else {
                    mActionMode = ACTION_MODE_0;
                }
                break;
            }
        }
        break;

    case ACTION_MODE_20: {
        cXyz sp2C(0.0f, 0.0f, (17.0f + nREG_F(8)) * player_p->getSpeedF());
        cLib_offsetPos(&field_0x65c, &player_p->current.pos, player_p->shape_angle.y, &sp2C);
        cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &field_0x65c), 4, 0x1000, 0x100);
        current.angle.y = shape_angle.y;
        if (mpModelMorf->checkFrame(2.0f) != 0) {
            mSound.startCreatureSound(Z2SE_EN_WW_JUMP, 0, -1);
        }

        if (mpModelMorf->isStop() != 0) {
            calcJumpSpeed();
            if (field_0x6b4 == 0) {
                mActionMode = ACTION_MODE_21;
                field_0x756 = 0;
            } else {
                mActionMode = ACTION_MODE_25;
                field_0x756 = 0;
                mObjAcch.SetGrndNone();
                mObjAcch.OffLineCheck();
                mObjAcch.OnLineCheckNone();
            }

            setBck(BCK_WW_JUMPATTACKB, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            shape_angle.x = cM_atan2s(speedF, speed.y) - 0x4000;
        }
        break;
    }

    case ACTION_MODE_21:
        mSph2[0].SetTgType(0xD8FBFDFF);
        mSph2[1].SetTgType(0xD8FBFDFF);
        cLib_chaseF(&speedF, 0.0f, 1.0f);
        mSph1[1].OnAtSetBit();

        if (mObjAcch.ChkGroundHit()) {
            speedF = 20.0f + ZREG_F(0);
            setBck(BCK_WW_JUMPATTACKC, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mActionMode = ACTION_MODE_22;
        }
        break;

    case ACTION_MODE_22:
        cLib_chaseF(&speedF, 0.0f, 1.0f + ZREG_F(1));
        if (mpModelMorf->checkFrame(2.0f) != 0) {
            mSound.startCreatureSound(Z2SE_EN_WW_FOOTNOTE, 0, -1);
        }

        if (mpModelMorf->isStop() != 0) {
            setActionMode(ACTION_CHASE, ACTION_MODE_0);
        }
        break;

    case ACTION_MODE_25:
    case ACTION_MODE_26:
        mObjAcch.ClrGroundHit();
        mSph2[0].SetTgType(0xD8FBFDFF);
        mSph2[1].SetTgType(0xD8FBFDFF);
        cLib_chaseF(&speedF, 0.0f, 1.0f);
        mSph1[1].OnAtSetBit();
        cXyz sp20(current.pos.x, 1000.0f + current.pos.y, current.pos.z);
        dBgS_GndChk gnd_chk;
        gnd_chk.SetPos(&sp20);
        sp20.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
        if (-G_CM3D_F_INF != sp20.y) {
            if (mActionMode == ACTION_MODE_25 && current.pos.y < sp20.y) {
                setAppearEffect();
                mSound.startCreatureSound(Z2SE_EN_WW_HIDE, 0, -1);
                mActionMode = ACTION_MODE_26;
            }

            if (attention_info.position.y < sp20.y) {
                field_0x75d = 0;
            }

            if (current.pos.y < (sp20.y - 500.0f)) {
                fopAcM_delete(this);
            }
        }
        break;
    }

    if (mObjAcch.ChkGroundHit()) {
        cLib_addCalcAngleS(&shape_angle.x, 0, 4, 0x1000, 0x200);
        return;
    }

    cLib_addCalcAngleS(&shape_angle.x, cM_atan2s(speedF, speed.y) - 0x4000, 4, 0x800, 0x200);
}

bool daE_WW_c::checkAttackStart() {
    if (field_0x734 == 0) {
        camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
        s16 temp_r28 = (fopCamM_GetAngleY(camera) + 0x8000) - fopAcM_searchPlayerAngleY(this);
        cXyz sp14 = daPy_getPlayerActorClass()->current.pos;

        if (field_0x6b4 != 1) {
            if (fpcM_Search(s_attack_ww, this) != NULL) {
                return 0;
            }
        } else if (abs(temp_r28) > 0x5000) {
            if (current.pos.absXZ(sp14) > 1400.0f + nREG_F(18) + mDistCheckModifier) {
                fopAcM_delete(this);
                return 1;
            }
        }

        if (field_0x6b4 == 0 && std::abs(current.pos.y - sp14.y) > 500.0f) {
            return 0;
        }

        if (abs(temp_r28) < nREG_S(9) + 0x1555 && field_0x668.absXZ(sp14) < 800.0f + field_0x6a8) {
            if (checkCreateBg(current.pos) != -G_CM3D_F_INF && checkAttackWall() != 0) {
                setActionMode(ACTION_ATTACK, ACTION_MODE_0);
                field_0x756 = 1;
                return 1;
            }
        }
    }

    return 0;
}

void daE_WW_c::executeChase() {
    s16 sp8;
    cXyz sp28 = daPy_getPlayerActorClass()->current.pos;
    f32 temp_f31 = sp28.absXZ(current.pos);
    
    switch (mActionMode) {
    case ACTION_MODE_5:
        if (temp_f31 > 1500.0f + nREG_F(18) + mDistCheckModifier) {
            mActionMode = ACTION_MODE_10;
            setBck(BCK_WW_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            return;
        }

    case ACTION_MODE_0:
        setBck(BCK_WW_RUN, J3DFrameCtrl::EMode_LOOP, 3.0f, l_HIO.run_anm);
        mActionMode = ACTION_MODE_1;
        speedF = l_HIO.run_speed;
        field_0x734 = l_HIO.attack_interval + cM_rndFX(30.0f);
        field_0x728 = 150;
        // fallthrough
    case ACTION_MODE_1: {
        if (calcMoveDir(&sp8, fopAcM_searchPlayerAngleY(this) - 0x8000) != 0) {
            cLib_addCalcAngleS(&shape_angle.y, sp8, 4, 0x800, 0x100);
        } else {
            cLib_addCalcAngleS(&shape_angle.y, sp8, 8, 0x200, 0x80);
        }

        current.angle.y = shape_angle.y;
        BOOL var_r29 = FALSE;
        if (temp_f31 > 1500.0f + nREG_F(18) + mDistCheckModifier - 200.0f) {
            var_r29 = TRUE;
        }

        if (field_0x6b4 == 0 && field_0x728 == 0) {
            f32 fVar7 = daPy_getPlayerActorClass()->speedF;
            f32 fVar8 = 0.0f;
            if (fVar7 == fVar8) {
                var_r29 = TRUE;
            }
        }

        if (var_r29) {
            mActionMode = ACTION_MODE_2;
            setBck(BCK_WW_TURN, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureSound(Z2SE_EN_WW_JUMP, 0, -1);
        }
        break;
    }

    case ACTION_MODE_2: {
        field_0x75a = 1;
        cLib_addCalcAngleS(&shape_angle.y, getNearPlayerAngle(), 8, 0x800, 0x100);
        cLib_chaseF(&speedF, 0.0f, 1.5f);
        f32 fVar1 = 0.0f;
        if (speedF == fVar1 && abs((s16)(shape_angle.y - fopAcM_searchPlayerAngleY(this))) < 0x2000) {
            mActionMode = ACTION_MODE_10;
            setBck(BCK_WW_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
        }
        break;
    }

    case ACTION_MODE_200: {
        field_0x75a = 1;
        cLib_addCalcAngleS(&shape_angle.y, getNearPlayerAngle(), 8, 0x800, 0x100);
        cLib_chaseF(&speedF, 0.0f, 1.5f);

        f32 fVar2 = 0.0f;
        if (speedF == fVar2) {
            if (abs((s16)(shape_angle.y - fopAcM_searchPlayerAngleY(this))) < 0x2000) {
                speedF = 0.0f;
                setActionMode(ACTION_MOVE_OUT, ACTION_MODE_10);
                setBck(BCK_WW_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            }
        }
        break;
    }

    case ACTION_MODE_10:
        mActionMode = ACTION_MODE_11;
        field_0x740 = 30;
        /// fallthrough
    case ACTION_MODE_11: {
        field_0x75a = 1;
        cLib_addCalcAngleS(&shape_angle.y, getNearPlayerAngle(), 4, 0x800, 0x100);
        current.angle.y = shape_angle.y;
        
        f32 fVar5 = daPy_getPlayerActorClass()->speedF;
        f32 fVar6 = 0.0f;
        if (fVar5 != fVar6) {
            if (temp_f31 < 1400.0f + nREG_F(18) + mDistCheckModifier) {
                if (field_0x730 != 0 || temp_f31 < 1200.0f + nREG_F(18) + mDistCheckModifier) {
                    mActionMode = ACTION_MODE_12;
                    setBck(BCK_WW_RUN, J3DFrameCtrl::EMode_LOOP, 3.0f, l_HIO.run_anm);
                } else {
                    setBck(BCK_WW_BACKSTEP, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                    mSound.startCreatureSound(Z2SE_EN_WW_JUMP, 0, -1);
                    mActionMode = ACTION_MODE_15;
                    speedF = -40.0f;
                    speed.y = 25.0f;
                    field_0x75b = 1;
                    field_0x730 = 0x3C;
                }
            } else if (temp_f31 > 1800.0f + nREG_F(18) + mDistCheckModifier) {
                setBck(BCK_WW_RUN, J3DFrameCtrl::EMode_LOOP, 3.0f, l_HIO.run_anm);
                mActionMode = ACTION_MODE_20;
                speedF = l_HIO.run_speed - 10.0f;
            }
        }

        if (checkSideStep() != 0) {
            mActionMode = ACTION_MODE_25;
            speed.y = 0.0f;
            speedF = 0.0f;
        }

        if (checkWalkStart()) {
            return;
        }

        if (checkAttackStart()) {
            return;
        }

        if (checkMoveOut()) {
            return;
        }
        break;
    }

    case 12:
        field_0x728 = 30;
        mActionMode = ACTION_MODE_13;
        // fallthrough
    case 13:
        if (daPy_getPlayerActorClass()->checkNowWolf() != 0) {
            speedF = l_HIO.wolf_escape_speed;
        } else {
            speedF = l_HIO.link_escape_speed;
        }

        if (calcMoveDir(&sp8, fopAcM_searchPlayerAngleY(this) - 0x8000) != 0) {
            cLib_addCalcAngleS(&shape_angle.y, sp8, 4, 0x800, 0x200);
        } else {
            cLib_addCalcAngleS(&shape_angle.y, sp8, 8, 0x200, 0x80);
        }

        current.angle.y = shape_angle.y;
        if (field_0x728 == 0) {
            mActionMode = ACTION_MODE_14;
            field_0x728 = 150;
        }
        break;

    case ACTION_MODE_14: {
        if (daPy_getPlayerActorClass()->checkNowWolf() != 0) {
            cLib_chaseF(&speedF, l_HIO.wolf_escape_speed, 1.0f);
        } else {
            cLib_chaseF(&speedF, l_HIO.link_escape_speed, 1.0f);
        }

        if (calcMoveDir(&sp8, (fopAcM_searchPlayerAngleY(this) - 0x8000)) != 0) {
            cLib_addCalcAngleS(&shape_angle.y, sp8, 4, 0x800, 0x200);
        } else {
            cLib_addCalcAngleS(&shape_angle.y, sp8, 8, 0x200, 0x80);
        }

        current.angle.y = shape_angle.y;

        if (field_0x668.absXZ(current.pos) > field_0x6a8) {
            mActionMode = ACTION_MODE_200;
            setBck(BCK_WW_TURN, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureSound(Z2SE_EN_WW_JUMP, 0, -1);
            return;
        }

        BOOL var_r29_2 = FALSE;
        if (temp_f31 > 1500.0f + nREG_F(18) + mDistCheckModifier) {
            var_r29_2 = TRUE;
        }

        if (field_0x6b4 == 0 && field_0x728 == 0) {
            f32 fVar3 = daPy_getPlayerActorClass()->speedF;
            f32 fVar4 = 0.0f;
            if (fVar3 == fVar4) {
                var_r29_2 = TRUE;
            }
        }

        if (var_r29_2) {
            mActionMode = ACTION_MODE_2;
            setBck(BCK_WW_TURN, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            mSound.startCreatureSound(Z2SE_EN_WW_JUMP, 0, -1);
        }
        break;
    }

    case ACTION_MODE_15:
        field_0x75a = 1;
        if (!mObjAcch.ChkGroundHit()) {
            cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 4, 0x800, 0x100);
            current.angle.y = shape_angle.y;
        } else {
            speedF = 0.0f;
            if (mpModelMorf->isStop()) {
                setBck(BCK_WW_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
                mActionMode = ACTION_MODE_10;
            }
        }
        break;

    case ACTION_MODE_20:
        field_0x73c = 0;
        // fallthrough
    case ACTION_MODE_21:
        field_0x75a = 1;
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 4, 0x800, 0x100);
        current.angle.y = shape_angle.y;
        if (temp_f31 < 1600.0f + nREG_F(18) + mDistCheckModifier) {
            setBck(BCK_WW_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
            mActionMode = ACTION_MODE_10;
            speedF = 0.0f;
        }

        if (mpModelMorf->getFrame() <= 5.0f) {
            if (checkSideStep() != 0) {
                mActionMode = ACTION_MODE_25;
                speed.y = 0.0f;
                speedF = 0.0f;
            }

            if (field_0x73c == 0 && checkMoveOut() != 0) {
                return;
            }
        }
        break;

    case ACTION_MODE_25:
        field_0x6cc = fopAcM_searchPlayerAngleY(this);
        shape_angle.y = field_0x6cc;
        if (field_0x6c0 == 0) {
            setBck(BCK_WW_SIDESTEPL, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            current.angle.y = field_0x6cc + 0x3800 + cM_rndFX(2000.0f);
        } else {
            setBck(BCK_WW_SIDESTEPR, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
            current.angle.y = field_0x6cc - 0x3800 + cM_rndFX(2000.0f);
        }

        mActionMode = ACTION_MODE_27;
        // fallthrough
    case ACTION_MODE_27:
        field_0x75a = 1;
        if (mpModelMorf->checkFrame(5.0f) != 0) {
            speed.y = 20.0f + nREG_F(15);
            speedF = nREG_F(16) + (40.0f + cM_rndFX(5.0f));
            field_0x75b = 1;
            mActionMode = ACTION_MODE_28;
            mSound.startCreatureSound(Z2SE_EN_WW_JUMP, 0, -1);
        }
        break;

    case ACTION_MODE_28:
        field_0x75a = 1;
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 4, 0x800, 0x100);
        if (mObjAcch.ChkGroundHit() != 0) {
            speedF = 0.0f;
            if (mpModelMorf->isStop() != 0) {
                setBck(BCK_WW_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
                mActionMode = ACTION_MODE_10;
            }
        }
        break;
    }

    cLib_addCalcAngleS(&shape_angle.x, 0, 8, 0x400, 0x100);
}

void daE_WW_c::executeDamage() {
    if (health <= 0) {
        field_0x724 = 5;
        mSph2[0].OffTgSetBit();
        mSph2[1].OffTgSetBit();
        field_0x75d = 0;
        attention_info.flags = 0;
    }

    switch (mActionMode) {
        case ACTION_MODE_0:
            if (health > 0) {
                setBck(BCK_WW_DAMAGE, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_WW_V_DAMAGE, -1);

                if (mObjAcch.ChkGroundHit()) {
                    speed.y = 20.0f;
                }

                speedF = 40.0f;
                mActionMode = ACTION_MODE_1;
            } else {
                setBck(BCK_WW_DEAD, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_WW_V_DEAD, -1);

                if (mObjAcch.ChkGroundHit()) {
                    speed.y = 20.0f;
                }

                speedF = 30.0f;
                mActionMode = ACTION_MODE_5;
                field_0x75c = 1;
            }

            field_0x75b = 1;
            shape_angle.x = 0;
            current.angle.y = mAtInfo.mHitDirection.y + 0x8000;
            shape_angle.y = mAtInfo.mHitDirection.y;
            break;

        case ACTION_MODE_1:
            if (mpModelMorf->checkFrame(14.0f)) {
                mpModelMorf->setPlaySpeed(0.0f);
            }

            if (mObjAcch.ChkGroundHit()) {
                field_0x75b = 0;
                mActionMode = ACTION_MODE_2;
                mpModelMorf->setPlaySpeed(1.0f);
            }
            break;

        case ACTION_MODE_2:
            cLib_chaseF(&speedF, 0.0f, 2.0f);

            if (mpModelMorf->isStop()) {
                setActionMode(ACTION_CHASE, ACTION_MODE_0);
            }
            break;

        case ACTION_MODE_5:
            if (mpModelMorf->checkFrame(21.0f)) {
                mpModelMorf->setPlaySpeed(0.0f);
            }

            if (mObjAcch.ChkGroundHit()) {
                field_0x75b = 0;
                mActionMode = ACTION_MODE_6;
                mpModelMorf->setPlaySpeed(1.0f);
            }
            break;

        case ACTION_MODE_6:
            if (mObjAcch.ChkGroundHit()) {
                cLib_chaseF(&speedF, 0.0f, 2.0f);
            }

            if (mpModelMorf->isStop()) {
                fopAcM_createDisappear(this, &current.pos, 10, 0, 0x1B);
                fopAcM_delete(this);
            }
            break;
    }
}

bool daE_WW_c::checkMoveOut() {
    if (field_0x668.absXZ(current.pos) > field_0x6a8) {
        setActionMode(ACTION_MOVE_OUT, ACTION_MODE_0);
        return true;
    }

    return false;
}

void daE_WW_c::executeMoveOut() {
    cXyz sp3c = daPy_getPlayerActorClass()->current.pos;

    switch (mActionMode) {
        case ACTION_MODE_0:
            field_0x728 = 30;
            field_0x734 = l_HIO.attack_interval + cM_rndFX(30.0f);
            // fallthrough
        case ACTION_MODE_10: {
            f32 fVar1 = sp3c.absXZ(current.pos);
            field_0x75a = 1;
            cLib_addCalcAngleS(&shape_angle.y, getNearPlayerAngle(), 4, 0x800, 0x100);
            current.angle.y = shape_angle.y;

            if (field_0x668.absXZ((sp3c)) > field_0x6a8) {
                if (fVar1 < nREG_F(18) + 1200.0f + mDistCheckModifier) {
                    if (field_0x728 == 0) {
                        mActionMode = ACTION_MODE_11;
                    }
                } else if (fVar1 > nREG_F(18) + 3000.0f + mDistCheckModifier) {
                    if (fopAcM_CheckCondition(this, fopAcM_STATUS_UNK_0x4)) {
                        fopAcM_delete(this);
                        break;
                    }
                } else if (field_0x668.absXZ(current.pos) > field_0x6a8 + 200.0f && field_0x728 == 0) {
                    mActionMode = ACTION_MODE_11;
                }
            } else if (fVar1 < nREG_F(18) + 1200.0f + mDistCheckModifier) {
                if (field_0x728 == 0) {
                    mActionMode = ACTION_MODE_15;
                }
            } else {
                if (checkAttackStart() || checkWalkStart()) {
                    break;
                }

                if (fVar1 > nREG_F(18) + 1800.0f + mDistCheckModifier) {
                    setBck(BCK_WW_RUN, J3DFrameCtrl::EMode_LOOP, 3.0f, l_HIO.run_anm);
                    setActionMode(ACTION_CHASE, ACTION_MODE_21);
                    speedF = l_HIO.run_speed - 10.0f;
                    field_0x73c = 30;
                    break;
                }
            }

            if (checkSideStep()) {
                mActionMode = ACTION_MODE_25;
                speed.y = 0.0f;
                speedF = 0.0f;
            }

            if (mActionMode == ACTION_MODE_0) {
                speedF = 0.0f;
                setBck(BCK_WW_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
                mActionMode = ACTION_MODE_10;
            }
            break;
        }

        case ACTION_MODE_11:
            setBck(BCK_WW_RUN, J3DFrameCtrl::EMode_LOOP, 3.0f, l_HIO.run_anm);
            speedF = l_HIO.run_speed;
            mActionMode = ACTION_MODE_12;
            field_0x728 = 60;
            // fallthrough
        case ACTION_MODE_12:
            cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &field_0x668), 4, 0x800, 0x100);
            current.angle.y = shape_angle.y;

            if (field_0x668.absXZ(current.pos) < field_0x6a8 - 100.0f && (field_0x728 == 0 ||
                sp3c.absXZ(current.pos) > nREG_F(18) + 1500.0f + mDistCheckModifier)) {
                setActionMode(ACTION_CHASE, ACTION_MODE_2);
                setBck(BCK_WW_TURN, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                mSound.startCreatureSound(Z2SE_EN_WW_JUMP, 0, -1);
            }
            break;

        case ACTION_MODE_15:
            setBck(BCK_WW_RUN, J3DFrameCtrl::EMode_LOOP, 3.0f, l_HIO.run_anm);
            speedF = l_HIO.run_speed;
            mActionMode = ACTION_MODE_16;
            field_0x728 = 60;
            
            if ((s16)(cLib_targetAngleY(&current.pos, &field_0x668) - fopAcM_searchPlayerAngleY(this)) < 0) {
                field_0x6cc = 0x2000;
            } else {
                field_0x6cc = -0x2000;
            }
            // fallthrough
        case ACTION_MODE_16:
            cLib_addCalcAngleS(&shape_angle.y, field_0x6cc + cLib_targetAngleY(&current.pos, &field_0x668), 4, 0x800, 0x100);
            current.angle.y = shape_angle.y;

            if (field_0x668.absXZ(current.pos) < field_0x6a8 - 100.0f && (field_0x728 == 0 ||
                sp3c.absXZ(current.pos) > nREG_F(18) + 1500.0f + mDistCheckModifier)) {
                setActionMode(ACTION_CHASE, ACTION_MODE_2);
                setBck(BCK_WW_TURN, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                mSound.startCreatureSound(Z2SE_EN_WW_JUMP, 0, -1);
            }
            break;

        case ACTION_MODE_25:
            field_0x6cc = fopAcM_searchPlayerAngleY(this);
            shape_angle.y = field_0x6cc;

            if (field_0x6c0 == 0) {
                setBck(BCK_WW_SIDESTEPL, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                current.angle.y = field_0x6cc + 0x3800 + cM_rndFX(2000.0f);
            } else {
                setBck(BCK_WW_SIDESTEPR, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                current.angle.y = field_0x6cc - 0x3800 + cM_rndFX(2000.0f);
            }

            mActionMode = ACTION_MODE_27;
            // fallthrough
        case ACTION_MODE_27:
            field_0x75a = 1;

            if (mpModelMorf->checkFrame(5.0f)) {
                field_0x75b = 1;
                speed.y = nREG_F(15) + 20.0f;
                speedF = nREG_F(16) + cM_rndFX(5.0f) + 40.0f;
                mActionMode = ACTION_MODE_28;
                mSound.startCreatureSound(Z2SE_EN_WW_JUMP, 0, -1);
            }
            break;

        case ACTION_MODE_28:
            field_0x75a = 1;
            cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 4, 0x800, 0x100);

            if (mObjAcch.ChkGroundHit()) {
                speedF = 0.0f;

                if (mpModelMorf->isStop()) {
                    setBck(BCK_WW_WAIT, J3DFrameCtrl::EMode_LOOP, 3.0f, 1.0f);
                    mActionMode = ACTION_MODE_10;
                }
            }
            break;

    }
}

bool daE_WW_c::checkWalkStart() {
    if (field_0x6b4 != 0) {
        return false;
    }

    s16 angleY = fopCamM_GetAngleY(dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0)));
    cXyz spd0, spdc;
    cXyz spe8 = daPy_getPlayerActorClass()->current.pos;

    if (field_0x740 == 0) {
        field_0x740 = 30;
        s16 sVar3 = fopAcM_searchPlayerAngleY(this) - 0x8000;
        s16 sVar2 = angleY - sVar3;
        if (abs(sVar2) < 0x3000) {
            if (cM_rnd() < nREG_F(20) + 0.6f) {
                return false;
            }

            angleY = cM_rndFX(1024.0f) + 3072.0f;
            if (abs(sVar2) < 0x2000) {
                if (cM_rnd() < 0.5f) {
                    angleY = -angleY;
                }
            } else if (sVar2 > 0) {
                angleY = -angleY;
            }

            f32 fVar2 = spe8.absXZ(current.pos);
            if (fVar2 < nREG_F(18) + 1400.0f + 100.0f) {
                fVar2 = nREG_F(18) + 1400.0f + 100.0f;
            }

            if (fVar2 > nREG_F(18) + 1800.0f - 100.0f) {
                fVar2 = nREG_F(18) + 1800.0f - 100.0f;
            }

            spdc.set(0.0f, 100.0f, fVar2);
            cLib_offsetPos(&spd0, &spe8, sVar3 + angleY, &spdc);
            cXyz spf4(current.pos.x, current.pos.y + 100.0f, current.pos.z);
            dBgS_LinChk lin_chk;
            lin_chk.Set(&spf4, &spd0, NULL);

            if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
                field_0x65c = spd0;
                setActionMode(ACTION_WALK, ACTION_MODE_0);

                return true;
            }
        } else {
            if (cM_rnd() < nREG_F(20) + 0.7f) {
                return false;
            }

            u8 uVar1 = 0;
            spdc.set(0.0f, 100.0f, nREG_F(18) + 1500.0f + 100.0f);
            cLib_offsetPos(&spd0, &spe8, (s16)(angleY + 0x1400 + cM_rndFX(1024.0f)), &spdc);
            
            f32 fVar1 = checkCreateBg(spd0);
            if (fVar1 != -G_CM3D_F_INF) {
                uVar1 = 1;
                spd0.y = fVar1;
            }

            cXyz sp100;
            cLib_offsetPos(&sp100, &spe8, (s16)(angleY - 0x1400 + cM_rndFX(1024.0f)), &spdc);

            fVar1 = checkCreateBg(sp100);
            if (fVar1 != -G_CM3D_F_INF) {
                uVar1 |= 2;
                sp100.y = fVar1;
            }

            if (uVar1 == 0) {
                return false;
            }

            if (uVar1 == 3) {
                if (cM_rnd() < 0.5f) {
                    spd0 = sp100;
                }
            } else if (uVar1 == 2) {
                spd0 = sp100;
            }

            createWolf(spd0, 0);
            fopAcM_delete(this);

            return true;
        }
    }

    return false;
}

void daE_WW_c::executeWalk() {
    f32 fVar1 = daPy_getPlayerActorClass()->current.pos.absXZ(current.pos);

    switch (mActionMode) {
        case ACTION_MODE_0:
            setBck(BCK_WW_WALK, J3DFrameCtrl::EMode_LOOP, 3.0f, l_HIO.walk_anm);
            mActionMode = ACTION_MODE_1;
            field_0x728 = 150;
            // fallthrough
        case ACTION_MODE_1: {
            if (mpModelMorf->checkFrame(5.0f) || mpModelMorf->checkFrame(10.0f) || mpModelMorf->checkFrame(15.0f) || mpModelMorf->checkFrame(20.0f)) {
                mSound.startCreatureSound(Z2SE_EN_WW_FOOTNOTE, 0, -1);
            }

            field_0x75a = 1;
            s16 sVar1 = cLib_targetAngleY(&current.pos, &field_0x65c);
            cLib_addCalcAngleS(&current.angle.y, sVar1, 8, 0x400, 0x100);
            s16 sVar2 = fopAcM_searchPlayerAngleY(this);

            if ((s16)(sVar1 - sVar2) > 0x4000) {
                sVar1 = sVar2 + 0x4000;
            }

            if ((s16)(sVar1 - sVar2) < -0x4000) {
                sVar1 = sVar2 - 0x4000;
            }

            cLib_addCalcAngleS(&shape_angle.y, sVar1, 8, 0x400, 0x100);
            cLib_chaseF(&speedF, nREG_F(1) + 5.0f, 1.0f);

            if (field_0x65c.absXZ(current.pos) < 150.0f || field_0x728 == 0) {
                setActionMode(ACTION_CHASE, ACTION_MODE_2);
            }

            if (fVar1 < nREG_F(18) + 1400.0f + mDistCheckModifier) {
                if (field_0x730 != 0 || fVar1 < nREG_F(18) + 1200.0f + mDistCheckModifier) {
                    setActionMode(ACTION_CHASE, ACTION_MODE_12);
                    setBck(BCK_WW_RUN, J3DFrameCtrl::EMode_LOOP, 3.0f, l_HIO.run_anm);
                } else {
                    setBck(BCK_WW_BACKSTEP, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                    mSound.startCreatureSound(Z2SE_EN_WW_JUMP, 0, -1);
                    setActionMode(ACTION_CHASE, ACTION_MODE_15);
                    speedF = -40.0f;
                    speed.y = 25.0f;
                    field_0x730 = 60;
                    field_0x75b = 1;
                }
            } else if (fVar1 > nREG_F(18) + 1800.0f + mDistCheckModifier) {
                setBck(BCK_WW_RUN, J3DFrameCtrl::EMode_LOOP, 3.0f, l_HIO.run_anm);
                setActionMode(ACTION_CHASE, ACTION_MODE_20);
                speedF = l_HIO.run_speed - 10.0f;
            }

            if (checkSideStep()) {
                setActionMode(ACTION_CHASE, ACTION_MODE_25);
                speed.y = 0.0f;
                speedF = 0.0f;
            }

            if (checkMoveOut()) return;
            break;
        }
        
        case ACTION_MODE_10:
            setBck(BCK_WW_RUN, J3DFrameCtrl::EMode_LOOP, 3.0f, l_HIO.run_anm);
            mActionMode = ACTION_MODE_11;
            field_0x728 = 150;
            // fallthrough
        case ACTION_MODE_11:
            if (daPy_getPlayerActorClass()->checkNowWolf()) {
                speedF = l_HIO.wolf_escape_speed;
            } else {
                speedF = l_HIO.link_escape_speed;
            }

            s16 sVar1;
            if (calcMoveDir(&sVar1, cLib_targetAngleY(&current.pos, &field_0x65c))) {
                cLib_addCalcAngleS(&shape_angle.y, sVar1, 4, 0x800, 0x200);
            } else {
                cLib_addCalcAngleS(&shape_angle.y, sVar1, 8, 0x200, 0x80);
            }

            current.angle.y = shape_angle.y;

            if (field_0x65c.abs(current.pos) < 300.0f || field_0x728 == 0) {
                if (fVar1 > nREG_F(18) + 1400.0f + mDistCheckModifier) {
                    setActionMode(ACTION_CHASE, ACTION_MODE_2);
                    setBck(BCK_WW_TURN, J3DFrameCtrl::EMode_NONE, 3.0f, 1.0f);
                    mSound.startCreatureSound(Z2SE_EN_WW_JUMP, 0, -1);
                } else {
                    setActionMode(ACTION_CHASE, ACTION_MODE_12);
                    setBck(BCK_WW_RUN, J3DFrameCtrl::EMode_LOOP, 3.0f, l_HIO.run_anm);
                }
            }
            break;
    }
}

void daE_WW_c::eWW_posMoveF() {
    if (field_0x75b != 0 && speed.y < 0.0f && mObjAcch.ChkGroundHit()) {
        field_0x75b = 0;
    }

    switch (field_0x75b) {
    case 0:
        gravity = -30.0f + nREG_F(0);
        break;

    case 1:
        gravity = -5.0f;
        break;

    case 2:
        gravity = -3.0f;
        break;
    }

    f32 temp_f31 = speedF;
    if (mObjAcch.ChkGroundHit()) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(mObjAcch.m_gnd, &plane);
        speedF *= plane.GetNP()->y;
    }

    fopAcM_posMoveF(this,mCcStts.GetCCMoveP());
    speedF = temp_f31;
}

void daE_WW_c::action() {
    field_0x75a = 0;
    setGroundAngle();
    damage_check();
    mSph1[1].OffAtSetBit();
    mSph2[0].SetTgType(0xD8000012);
    mSph2[1].SetTgType(0xD8000012);
    BOOL bVar1 = FALSE;
    s16 sVar1 = shape_angle.y;

    switch (mAction) {
        case ACTION_MASTER:
            executeMaster();
            return;

        case ACTION_WAIT:
            executeWait();
            break;

        case ACTION_ATTACK:
            executeAttack();
            bVar1 = TRUE;
            break;

        case ACTION_CHASE:
            executeChase();
            bVar1 = TRUE;
            break;

        case ACTION_DAMAGE:
            executeDamage();
            bVar1 = TRUE;
            break;

        case ACTION_MOVE_OUT:
            executeMoveOut();
            break;

        case ACTION_WALK:
            executeWalk();
            bVar1 = TRUE;
            break;
    }

    setBlurEffect();
    field_0x759 = 0;
    if (shape_angle.y != sVar1) {
        if ((s16)(shape_angle.y - sVar1) < 0) {
            field_0x759 = 1;
        } else {
            field_0x759 = 2;
        }
    }

    mSound.setLinkSearch(bVar1);

    if (field_0x75d) {
        dBgS_LinChk lin_chk;
        lin_chk.Set(&dComIfGp_getCamera(0)->lookat.eye, &attention_info.position, this);

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
        } else {
            attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
        }
    }

    eWW_posMoveF();
    mObjAcch.CrrPos(dComIfG_Bgsp());
    mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (checkBck(0xB)) {
        if (mpModelMorf->checkFrame(0.0f) || mpModelMorf->checkFrame(6.0f) || mpModelMorf->checkFrame(8.0f) || mpModelMorf->checkFrame(13.0f)) {
            mSound.startCreatureSound(Z2SE_EN_WW_FOOTNOTE, 0, -1);
        }
    } else if (checkBck(0xF) && mpModelMorf->checkFrame(5.0f)) {
        mSound.startCreatureVoice(Z2SE_EN_WW_V_WAIT, -1);
    }
}

void daE_WW_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x674);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::XrotM(field_0x6ce);
    mDoMtx_stack_c::scaleM(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    
    mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModelMorf->modelCalc();
}

void daE_WW_c::cc_set() {
    cXyz sp8;
    if (mAction != ACTION_MASTER) {
        J3DModel* model_p = mpModelMorf->getModel();
        mDoMtx_stack_c::copy(model_p->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::multVecZero(&eyePos);
        attention_info.position = eyePos;
        attention_info.position.y += 20.0f;
        mDoMtx_stack_c::copy(model_p->getAnmMtx(JNT_BODY01));
        mDoMtx_stack_c::transM(0.0f, -20.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&sp8);
        mSph1[0].SetC(sp8);
        mSph1[0].SetR(40.0f);
        dComIfG_Ccsp()->Set(&mSph1[0]);
        mSph2[0].SetC(sp8);
        mSph2[0].SetR(70.0f + nREG_F(17));
        dComIfG_Ccsp()->Set(&mSph2[0]);
        mDoMtx_stack_c::copy(model_p->getAnmMtx(JNT_HEAD)), 
        mDoMtx_stack_c::transM(-10.0f, -20.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&sp8);
        mSph1[1].SetC(sp8);
        mSph1[1].SetR(40.0f);
        dComIfG_Ccsp()->Set(&mSph1[1]);
        mSph2[1].SetC(sp8);
        mSph2[1].SetR(70.0f + nREG_F(17));
        dComIfG_Ccsp()->Set(&mSph2[1]);
    }
}

int daE_WW_c::execute() {
    if (field_0x728 != 0) {
        field_0x728--;
    }

    if (field_0x72c != 0) {
        field_0x72c--;
    }

    if (field_0x724 != 0) {
        field_0x724--;
    }

    if (field_0x730 != 0) {
        field_0x730--;
    }

    if (field_0x734 != 0) {
        field_0x734--;
    }

    if (field_0x738 != 0) {
        field_0x738--;
    }

    if (field_0x73c != 0) {
        field_0x73c--;
    }

    if (field_0x740 != 0) {
        field_0x740--;
    }

    for (int i = 0; i++ != 0;) {
        if (field_0x744[i] != 0) {
            field_0x744[i]--;
        }
    }

    action();
    mtx_set();
    cc_set();
    setHeadAngle();

    cXyz i_effPos;
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(JNT_BODY02));
    mDoMtx_stack_c::transM(nREG_F(0), nREG_F(1), nREG_F(2));
    mDoMtx_stack_c::multVecZero(&i_effPos);
    cXyz i_effSize(nREG_F(3) + 1.0f, nREG_F(4) + 1.0f, nREG_F(5) + 2.0f);
    setMidnaBindEffect(this, &mSound, &i_effPos, &i_effSize);

    return 1;
}

static int daE_WW_Execute(daE_WW_c* i_this) {
    return i_this->execute();
}

bool daE_WW_c::calcMoveDir(s16* param_1, s16 param_2) {
    dBgS_LinChk lin_chk;
    cM3dGPla plane;
    s16 sVar4, sVar2[3], sVar3[3];
    s16 sVar1[3] = {0, -0x2000, 0x2000};
    f32 fVar1[3];
    cXyz spc0, spcc;
    cXyz spd8(0.0f, 0.0f, 500.0f);
    spcc.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);

    for (int i = 0; i < 3; i++) {
        fVar1[i] = 1000.0f;
        cLib_offsetPos(&spc0, &spcc, shape_angle.y + sVar1[i], &spd8);
        lin_chk.Set(&spcc, &spc0, NULL);
        sVar2[i] = 0;

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            fVar1[i] = current.pos.abs(lin_chk.GetCross());
            dComIfG_Bgsp().GetTriPla(lin_chk, &plane);

            if (plane.mNormal.y < 0.5f) {
                field_0x750[i] = sVar3[i] = cM_atan2s(plane.mNormal.x, plane.mNormal.z);
                sVar2[i] = 1;
                field_0x744[i] = nREG_S(0) + 30;
            }
        }
    }

    if (strcmp("D_MN11", dComIfGp_getStartStageName()) == 0 && dComIfGp_getStartStageRoomNo() == 4) {
        if (sVar2[0] != 0) {
            field_0x74c = 0;
            field_0x744[1] = 0;
        } else {
            if (field_0x744[1] != 0) {
                sVar3[1] = field_0x750[1];
                sVar2[1] = 1;
            }

            if (field_0x74c != 0) {
                sVar3[2] = field_0x750[2];
                sVar2[2] = 1;
            }
        }
    }

    if (sVar2[1] != 0 && sVar2[2] != 0) {
        if (sVar2[0] != 0) {
            if (field_0x759 == 1) {
                sVar4 = shape_angle.y - 0x2000;
            } else {
                sVar4 = shape_angle.y + 0x2000;
            }
        } else if (field_0x759 == 1) {
            sVar4 = shape_angle.y - 0x2000;
        } else if (field_0x759 == 2) {
            sVar4 = shape_angle.y + 0x2000;
        } else {
            sVar4 = param_2;
        }

        field_0x738 = 30;
    } else if (sVar2[0] != 0) {
        if (sVar2[1] != 0) {
            sVar4 = shape_angle.y + 0x2000;
        } else if (sVar2[2] != 0) {
            sVar4 = shape_angle.y - 0x2000;
        } else if ((s16)(sVar3[0] - shape_angle.y) > 0) {
            sVar4 = shape_angle.y + 0x2000;
        } else {
            sVar4 = shape_angle.y - 0x2000;
        }

        if (field_0x738 == 0) {
            if (abs((s16)(fopAcM_searchPlayerAngleY(this) - sVar4)) < 0x4000) {
                sVar4 = param_2;
            }
        }
    } else if (sVar2[1] != 0) {
        if ((s16)(sVar3[1] - shape_angle.y) < 0) {
            sVar4 = sVar3[1] + 0x4000;
        } else {
            sVar4 = sVar3[1] - 0x4000;
        }

        if (field_0x738 == 0) {
            if (abs((s16)(fopAcM_searchPlayerAngleY(this) - sVar4)) < 0x2000) {
                sVar4 = sVar3[1];
            }
        }

        if (fVar1[1] < 300.0f) {
            sVar4 = sVar3[1];
        }
    } else if (sVar2[2] != 0) {
        if ((s16)(sVar3[2] - shape_angle.y) < 0) {
            sVar4 = sVar3[2] + 0x4000;
        } else {
            sVar4 = sVar3[2] - 0x4000;
        }

        if (field_0x738 == 0) {
            if (abs((s16)(fopAcM_searchPlayerAngleY(this) - sVar4)) < 0x2000) {
                sVar4 = sVar3[2];
            }
        }

        if (fVar1[2] < 300.0f) {
            sVar4 = sVar3[2];
        }
    } else {
        sVar4 = param_2;
    }

    *param_1 = sVar4;

    for (int i = 0; i < 3; i++) {
        if (sVar2[i] != 0) {
            return true;
        }
    }

    return false;
}

static int daE_WW_IsDelete(daE_WW_c* i_this) {
    return 1;
}

int daE_WW_c::_delete() {
    dComIfG_resDelete(&mPhase, "E_WW");
    if (field_0xec4 != 0) {
        l_HIOInit = false;
        mDoHIO_DELETE_CHILD(l_HIO.field_0x04);
    }

    if (heap != 0) {
        mSound.deleteObject();
    }
    
    return 1;
}

static int daE_WW_Delete(daE_WW_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "E_WW");
    return static_cast<daE_WW_c*>(i_this)->_delete();
}

int daE_WW_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_WW", BMDR_WW);

    JUT_ASSERT(2823, modelData != NULL);

    mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_WW", BCK_WW_APPEAR), 0, 1.0f, 0, -1, &mSound, 0x80000, 0x11000084);
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpModelMorf->getModel();
    model->setUserArea((u32)this);
    for (u16 i = JNT_BODY01; i < model->getModelData()->getJointNum(); i++) {
        if (i == JNT_NECK || i == JNT_HEAD) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
        }
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    daE_WW_c* a_this = (daE_WW_c*) i_this;
    return a_this->CreateHeap();
}

int daE_WW_c::create() {
    fopAcM_ct(this, daE_WW_c);

    int phase_state = dComIfG_resLoad(&mPhase, "E_WW");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_WW PARAM %x\n", fopAcM_GetParam(this));

        field_0x6b4 = fopAcM_GetParam(this) & 0xF;
        if (field_0x6b4 == 0xF) {
            field_0x6b4 = 0;
        }
        
        u8 var_r26 = fopAcM_GetParam(this) >> 4; // Arg1 ?
        if (var_r26 == 0xFF || var_r26 == 0) {
            var_r26 = 30;
        }

        field_0x6a8 = 100.0f * var_r26;
        field_0x6b5 = fopAcM_GetParam(this) >> 0xC;
        if (field_0x6b5 == 0) {
            field_0x6b5 = 5;
        }

        field_0x6b6 = (fopAcM_GetParam(this) >> 0x14) & 0xF;
        if (field_0x6b6 == 0xF || field_0x6b6 == 0) {
            field_0x6b6 = 3;
        }

        if (field_0x6b6 > 10) {
            field_0x6b6 = 10;
        }

        field_0x757 = (fopAcM_GetParam(this) >> 0x18) & 0xF;
        if (field_0x757 == 0xF || field_0x757 == 0) {
            field_0x757 = 1;
        } else {
            if (field_0x757 == 2) {
                field_0x758 = 1;
            }

            field_0x757 = 0;
        }

        u8 var_r27 = current.angle.x;
        if (var_r27 == 0xFF || var_r27 == 0) {
            if (field_0x6b4 == 0) {
                var_r27 = 5;
            } else {
                var_r27 = 10;
            }
        }
        
        field_0x6ac = 100.0f * var_r27;
        shape_angle.x = 0;
        current.angle.x = 0;
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x1F00)) {
            return cPhs_ERROR_e;
        }

        if (l_HIOInit == false) {
            l_HIOInit = true;
            field_0xec4 = 1;
            l_HIO.field_0x04 = mDoHIO_CREATE_CHILD("ホワイトウルフォス", (JORReflexible*)&l_HIO);
        }

        fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);

        if (field_0x757 != 0) {
            setActionMode(ACTION_MASTER, ACTION_MODE_0);
        } else {
            mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
            mAcchCir.SetWall(65.0f, 130.0f);
            mObjAcch.OnLineCheck();

            if (field_0x6b4 == 1) {
                health = 1;
                field_0x560 = 1;
            } else {
                health = 100;
                field_0x560 = 100;
            }
            
            mCcStts.Init(0xFE, 0, this);
            field_0x75d = 1;
            attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            mSph1[0].Set(cc_ww_src);
            mSph1[0].SetStts(&mCcStts);
            mSph1[1].Set(cc_ww_src);
            mSph1[1].SetStts(&mCcStts);
            mSph2[0].Set(cc_ww_tg_src);
            mSph2[0].SetStts(&mCcStts);
            mSph2[0].OnTgNoHitMark();
            mSph2[1].Set(cc_ww_tg_src);
            mSph2[1].SetStts(&mCcStts);
            mSph2[1].OnTgNoHitMark();
            mSound.init(&current.pos, &eyePos, 3, 1);
            mSound.setEnemyName("E_ww");
            mAtInfo.mpSound = &mSound;
            mAtInfo.mPowerType = 1;
            gravity = -5.0f;
            mDistCheckModifier = cM_rndFX(100.0f);
            field_0x668 = home.pos;

            fopAc_ac_c* actor_p;
            if (fopAcM_SearchByID(fopAcM_GetLinkId(this), (fopAc_ac_c**)&actor_p) != 0) {
                field_0x668 = actor_p->current.pos;
            }

            setActionMode(ACTION_WAIT, ACTION_MODE_0);
        }

        attention_info.distances[fopAc_attn_BATTLE_e] = 0x34;
        daE_WW_Execute(this);
    }

    return phase_state;
}

static int daE_WW_Create(daE_WW_c* i_this) {
    return static_cast<daE_WW_c*>(i_this)->create();
}

static actor_method_class l_daE_WW_Method = {
    (process_method_func)daE_WW_Create,
    (process_method_func)daE_WW_Delete,
    (process_method_func)daE_WW_Execute,
    (process_method_func)daE_WW_IsDelete,
    (process_method_func)daE_WW_Draw,
};

extern actor_process_profile_definition g_profile_E_WW = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_WW,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_WW_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  209,                    // mPriority
  &l_daE_WW_Method,       // sub_method
  0x00044100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
