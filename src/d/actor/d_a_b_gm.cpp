/**
 * @file d_a_b_gm.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_gm.h"
#include "d/actor/d_a_e_gm.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_obj_ystone.h"
#include "d/d_s_play.h"
#include "d/d_com_inf_game.h"
#include "c/c_damagereaction.h"
#include "SSystem/SComponent/c_math.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_msg_mng.h"
#include "Z2AudioLib/Z2Instances.h"

#define ANM_EYE_TEST            6
#define ANM_GM_BEAM             7
#define ANM_GOMA_ATTACK_01      8
#define ANM_GOMA_ATTACK_A       9
#define ANM_GOMA_ATTACK_B       10
#define ANM_GOMA_ATTACK_C       11
#define ANM_GOMA_DAMAGE_01      12
#define ANM_GOMA_DAMAGE_02      13
#define ANM_GOMA_DAMAGE_WAIT    14
#define ANM_GOMA_DASH           15
#define ANM_GOMA_DEATH          16
#define ANM_GOMA_FALL_LOOP      17
#define ANM_GOMA_LANDING        18
#define ANM_GOMA_LANDING_DAMAGE 19
#define ANM_GOMA_LANDING_WAIT   20
#define ANM_GOMA_LAY_EGGS       21
#define ANM_GOMA_MOVE           22
#define ANM_GOMA_RETURN         23
#define ANM_GOMA_ROOF_DAMAGE    24
#define ANM_GOMA_SLOW_MOVE      25
#define ANM_GOMA_STEP_L         26
#define ANM_GOMA_STEP_R         27
#define ANM_GOMA_UP             28
#define ANM_GOMA_UP_02          29
#define ANM_GOMA_WAIT           30

enum daB_GM_ACTION {
    ACTION_WAIT,
    ACTION_MOVE,

    ACTION_BEAM = 5,
    ACTION_KOGOMA,

    ACTION_DAMAGE = 10,
    ACTION_DROP,
};

class daB_GM_HIO_c {
public:
    daB_GM_HIO_c();
    virtual ~daB_GM_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 model_size;
    /* 0x0C */ f32 check_size;
    /* 0x10 */ f32 dash_speed;
    /* 0x14 */ f32 dash_anm_speed;
    /* 0x18 */ f32 move_speed;
    /* 0x1C */ f32 move_anm_speed;
    /* 0x20 */ f32 wait_anm_speed;
    /* 0x24 */ f32 range;
    /* 0x28 */ s16 smoke_prim_R;
    /* 0x2A */ s16 smoke_prim_G;
    /* 0x2C */ s16 smoke_prim_B;
    /* 0x2E */ s16 smoke_env_R;
    /* 0x30 */ s16 smoke_env_G;
    /* 0x32 */ s16 smoke_env_B;
    /* 0x34 */ s16 smoke_alpha;
    /* 0x36 */ s16 field_0x36;
    /* 0x38 */ f32 smoke_blend;
    /* 0x3C */ f32 bend_degree_1;
    /* 0x40 */ f32 bend_degree_2;
    /* 0x44 */ f32 bend_degree_3;
    /* 0x48 */ u8 foot_pos_check;
    /* 0x49 */ u8 eye_check;
};

daB_GM_HIO_c::daB_GM_HIO_c() {
    field_0x4 = 0xFF;
    model_size = 1.0f;
    check_size = 1.5f;

    dash_speed = 35.0f;
    dash_anm_speed = 1.0f;
    move_speed = 15.0f;
    move_anm_speed = 1.0f;
    wait_anm_speed = 1.0f;
    range = 1300.0f;

    smoke_prim_R = 205;
    smoke_prim_G = 200;
    smoke_prim_B = 180;
    smoke_env_R = 103;
    smoke_env_G = 100;
    smoke_env_B = 90;
    smoke_alpha = 110;
    smoke_blend = 0.3;

    bend_degree_1 = 1.0f;
    bend_degree_2 = 1.0f;
    bend_degree_3 = -1.0f;

    foot_pos_check = false;
    eye_check = false;
}

static void anm_init(b_gm_class* i_this, int i_anmID, f32 i_morf, u8 i_attr, f32 i_speed) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_gm", i_anmID), i_attr, i_morf, i_speed, 0.0f, -1.0f);
    i_this->mAnmID = i_anmID;
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        
        b_gm_class* a_this = (b_gm_class*)model->getUserArea();
        if (a_this != NULL) {
            MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);

            if (jnt_no >= 0x19 && jnt_no <= 0x1B) {
                cMtx_ZrotM(*calc_mtx, a_this->mFoot[0].field_0x8[jnt_no - 0x19]);
            } else if (jnt_no >= 0x1D && jnt_no <= 0x1F) {
                cMtx_ZrotM(*calc_mtx, a_this->mFoot[1].field_0x8[jnt_no - 0x1D]);
            } else if (jnt_no >= 0x21 && jnt_no <= 0x23) {
                cMtx_ZrotM(*calc_mtx, a_this->mFoot[2].field_0x8[jnt_no - 0x21]);
            } else if (jnt_no >= 0x25 && jnt_no <= 0x27) {
                cMtx_ZrotM(*calc_mtx, a_this->mFoot[3].field_0x8[jnt_no - 0x25]);
            } else if (jnt_no >= 0x29 && jnt_no <= 0x2B) {
                cMtx_ZrotM(*calc_mtx, a_this->mFoot[4].field_0x8[jnt_no - 0x29]);
            } else if (jnt_no >= 0x2D && jnt_no <= 0x2F) {
                cMtx_ZrotM(*calc_mtx, a_this->mFoot[5].field_0x8[jnt_no - 0x2D]);
            } else if (jnt_no >= 0x31 && jnt_no <= 0x33) {
                cMtx_ZrotM(*calc_mtx, a_this->mFoot[6].field_0x8[jnt_no - 0x31]);
            } else if (jnt_no >= 0x35 && jnt_no <= 0x37) {
                cMtx_ZrotM(*calc_mtx, a_this->mFoot[7].field_0x8[jnt_no - 0x35]);
            } else if (jnt_no >= 0x39 && jnt_no <= 0x3B) {
                cMtx_ZrotM(*calc_mtx, a_this->mFoot[8].field_0x8[jnt_no - 0x39]);
            } else if (jnt_no >= 0x3E && jnt_no <= 0x40) {
                cMtx_ZrotM(*calc_mtx, a_this->mFoot[8].field_0x8[jnt_no - 0x3E]);
            } else if (jnt_no == 0x16) {
                cMtx_ZrotM(*calc_mtx, a_this->field_0x1ad8);
            } else if (jnt_no == 0x17) {
                cMtx_ZrotM(*calc_mtx, -a_this->field_0x1ad8);
            } else if (jnt_no == 0x15) {
                cMtx_YrotM(*calc_mtx, a_this->field_0x1ada);
                cMtx_ZrotM(*calc_mtx, a_this->field_0x1adc);
            }

            model->setAnmMtx(jnt_no, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static u8 l_initHIO;

static daB_GM_HIO_c l_HIO;

static int daB_GM_Draw(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    #if !DEBUG
    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    #endif

    for (int i = 0; i < 4; i++) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpSpotLightModel[i], &a_this->tevStr);
        i_this->mpSpotLightBtk->entry(i_this->mpSpotLightModel[i]->getModelData());
        mDoExt_modelUpdateDL(i_this->mpSpotLightModel[i]);
    }

    if (i_this->mIsDisappear) {
        return 1;
    }

    J3DModel* model = i_this->mpModelMorf->getModel();
    #if DEBUG
    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    #endif
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);
    
    i_this->mpZoomBtk->entry(model->getModelData());

    if (fabsf(i_this->mBodyColorIntensity) > 1.0f) {
        J3DModelData* modelData = model->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* material = modelData->getMaterialNodePointer(i);

            material->getTevColor(0)->r = i_this->mBodyColorIntensity;
            material->getTevColor(0)->g = i_this->mBodyColorIntensity;
            material->getTevColor(0)->b = i_this->mBodyColorIntensity;
        }
    }

    i_this->mpModelMorf->entryDL();

    cXyz shadow_pos;
    shadow_pos.set(a_this->current.pos.x, BREG_F(18) + (a_this->current.pos.y + l_HIO.model_size * 400.0f), a_this->current.pos.z);
    i_this->mShadowID = dComIfGd_setShadow(i_this->mShadowID, 1, model, &shadow_pos, (l_HIO.model_size * 3000.0f) + BREG_F(19), 0.0f, a_this->current.pos.y, i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd, &a_this->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    if (i_this->field_0x6c0 > 0.1f) {
        J3DModel* model = i_this->mpBeamModelMorf->getModel();
        g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);
        i_this->mpBeamBtk->entry(model->getModelData());
        i_this->mpBeamModelMorf->entryDL();
    }

    return 1;
}

static void* s_ko_del(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_GM && fopAcM_GetParam(i_actor) == 0) {
        ((daE_GM_c*)i_actor)->InstantKill();
    }

    return NULL;
}

static void damage_check(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    i_this->mCcStts.Move();

    if (i_this->mInvincibilityTimer == 0) {
        if ((i_this->mAction != ACTION_DROP || (i_this->mMode != 2 && i_this->mMode != 3)) && i_this->mCoreSph.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mCoreSph.GetTgHitObj();
            cc_at_check(i_this, &i_this->mAtInfo);

            i_this->mInvincibilityTimer = 10;
            i_this->mInvincibilityTimer = 5;
            i_this->mAction = ACTION_DAMAGE;
            i_this->mMode = 0;
            i_this->field_0x6c0 = 0.0f;

            cXyz eff_pos;
            MtxP now_mtx = mDoMtx_stack_c::get();
            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(0x15), now_mtx);
            mDoMtx_stack_c::multVecZero(&eff_pos);

            for (int i = 0; i < 3; i++) {
                static u16 name[] = {0x8B8C, 0x8B8D, 0x8B8E};
                dComIfGp_particle_set(name[i], &eff_pos, NULL, NULL);
            }
            return;
        }

        for (int i = 0; i < 8; i++) {
            if (i < 2 && i_this->mBodySph[i].ChkTgHit()) {
                i_this->mInvincibilityTimer = 6;
                if (i_this->mAction == ACTION_DROP && (i_this->mMode == 2 || i_this->mMode == 3)) {
                    if (i_this->mBodySph[i].GetTgHitObj()->ChkAtType(AT_TYPE_CSTATUE_BOSS_SWING)) {
                        i_this->mHitCount++;
                        dComIfGs_onOneZoneSwitch(4, -1);
                        Z2GetAudioMgr()->changeBgmStatus(2);

                        if (i_this->mHitCount >= 3) {
                            i_this->mMode = 20;
                            i_this->mDemoMode = 30;
                            fpcM_Search(s_ko_del, i_this);
                            Z2GetAudioMgr()->bgmStop(0x1E, 0);
                        } else {
                            i_this->mMode = 10;
                            i_this->field_0x1ad5 = 2;
                            i_this->mDemoMode = 20;
                        }

                        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0x4F, cXyz(0.0f, 1.0f, 0.0f));

                        for (int j = 0; j < 4; j++) {
                            static u16 name[] = {0x8AF6, 0x8AF7, 0x8AF8, 0x8AF9};
                            dComIfGp_particle_set(name[j], &a_this->current.pos, NULL, NULL);
                        }
                        return;
                    }
                    dComIfGs_onOneZoneSwitch(3, -1);
                }
                return;
            } else if (i_this->mFootSph[i].ChkTgHit()) {
                i_this->mInvincibilityTimer = 6;
                def_se_set(&i_this->mCreatureSound, i_this->mFootSph[i].GetTgHitObj(), 0x28, NULL);
                dComIfGs_onOneZoneSwitch(3, -1);
                return;
            }
        }
    }
}

static BOOL bg_check(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    dBgS_LinChk line_chk;
    cM3dGPla plane;
    cXyz start;
    cXyz end;
    cXyz sp14;
    cXyz unused;

    cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
    sp14.x = 0.0f;
    sp14.y = 180.0f;
    sp14.z = 0.0f;
    MtxPosition(&sp14, &start);

    start += a_this->current.pos;
    sp14.x = 0.0f;
    sp14.y = 0.0f;
    sp14.z = 600.0f;
    MtxPosition(&sp14, &end);
    end += start;

    line_chk.Set(&start, &end, i_this);
    if (dComIfG_Bgsp().LineCross(&line_chk)) {
        if (a_this->current.angle.x < 0x1000 && a_this->current.angle.x > -0x1000) {
            dComIfG_Bgsp().GetTriPla(line_chk, &plane);
            sp14.x = plane.GetNP()->x;
            sp14.z = plane.GetNP()->z;
            i_this->field_0x5ce = cM_atan2s(sp14.x, sp14.z) + 0x8000;
        }

        i_this->field_0x73c = line_chk.GetCross();
        return TRUE;
    }

    return FALSE;
}

static int ko_ct;

static void* s_ko_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_GM) {
        ko_ct++;
    }

    return NULL;
}

static void* s_ko2_move(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_GM && fopAcM_GetParam(i_actor) == 3) {
        ((daE_GM_c*)i_actor)->MoveStart(0);
        return i_actor;
    }

    return NULL;
}

static void* s_ko2_get(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_GM && fopAcM_GetParam(i_actor) == 3) {
        return i_actor;
    }

    return NULL;
}

static void* s_ko_move(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_GM && fopAcM_GetParam(i_actor) == 1) {
        ((daE_GM_c*)i_actor)->MoveStart(cM_rndF(15.0f));
    }

    return NULL;
}

static void b_gm_wait(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_GOMA_WAIT, 20.0f, J3DFrameCtrl::EMode_LOOP, l_HIO.wait_anm_speed);
        i_this->mTimers[0] = 100.0f + cM_rndF(100.0f);
        i_this->mMode = 1;
        /* fallthrough */
    case 1:
        if (i_this->mTimers[0] == 0) {
            i_this->mAction = ACTION_MOVE;
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 4.0f);
    i_this->mAcch.CrrPos(dComIfG_Bgsp());
}

static cXyz target_pos[] = {
    cXyz(-1350.0f, 0.0f, -1350.0f),
    cXyz(-1350.0f, 0.0f, 1350.0f),
    cXyz(1350.0f, 0.0f, -1350.0f),
    cXyz(1350.0f, 0.0f, 1350.0f),
};

static void b_gm_move(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz sp1C;
    f32 move_speed = l_HIO.dash_speed;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_GOMA_DASH, 10.0f, J3DFrameCtrl::EMode_LOOP, l_HIO.dash_anm_speed);
        i_this->mMode = 1;
        i_this->unk_0x5cc = 0;
        /* fallthrough */
    case 1:
        if (a_this->current.angle.x < 0x1000 && a_this->current.angle.x > -0x1000) {
            for (int i = 0; i < 4; i++) {
                sp1C.x = target_pos[i].x * 1.7f - a_this->current.pos.x;
                sp1C.z = target_pos[i].z * 1.7f - a_this->current.pos.z;

                s16 sp8 = a_this->current.angle.y - cM_atan2s(sp1C.x, sp1C.z);
                if (sp8 < 0x1800 && sp8 > -0x1800) {
                    if (sp8 > 0) {
                        a_this->current.angle.y += 0x200;
                    } else {
                        a_this->current.angle.y -= 0x200;
                    }
                    break;
                }
            }
        }

        if (bg_check(i_this)) {
            i_this->mMode = 2;

            if (i_this->field_0x5d8 == 0.0f) {
                i_this->field_0x5d8 = -(f32)0x4000;
            } else if (i_this->field_0x5d8 == -(f32)0x4000) {
                i_this->field_0x5d8 = -(f32)0x8000;
            } else if (i_this->field_0x5d8 == -(f32)0x8000) {
                i_this->field_0x5d8 = -(f32)0xC000;
            } else if (i_this->field_0x5d8 == -(f32)0xC000) {
                i_this->field_0x5d8 = -(f32)0x10000;
            }
        }
        break;
    case 2:
        cLib_addCalc2(&i_this->field_0x5d4, i_this->field_0x5d8, 1.0f, 1024.0f);

        if (i_this->field_0x5d4 == -(f32)0x10000) {
            i_this->field_0x5d4 = 0.0f;
        }

        if (i_this->field_0x5d4 == 0.0f || i_this->field_0x5d4 == -(f32)0x4000 || i_this->field_0x5d4 == -(f32)0x8000 || i_this->field_0x5d4 == -(f32)0xC000) {
            if (i_this->field_0x5d4 == -(f32)0x8000) {
                i_this->mMode = 3;
                i_this->mTimers[1] = 60;
            } else {
                i_this->mMode = 1;
            }
        }

        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5ce, 4, 0x400);
        a_this->current.angle.x = i_this->field_0x5d4;
        break;
    case 3:
        if (i_this->mTimers[1] == 0) {
            sp1C = i_this->mTargetMovePos - a_this->current.pos;
            cLib_addCalcAngleS2(&a_this->current.angle.y, cM_atan2s(sp1C.x, sp1C.z) + 0x8000, 4, 0x300);

            if (sp1C.abs() < 200.0f) {
                if (i_this->mTimers[2] != 0) {
                    i_this->mTimers[0] = 0;
                    i_this->mMode = 4;
                } else if (i_this->field_0x1ad5 == 2 || dComIfGs_getArrowNum() <= 3) {
                    i_this->mAction = ACTION_KOGOMA;
                    i_this->mMode = 0;
                    i_this->field_0x1ad5 = 1;
                } else {
                    i_this->mTimers[0] = 0;
                    i_this->mMode = 4;

                    if (cM_rndF(1.0f) < 0.5f) {
                        i_this->mAction = ACTION_BEAM;
                        i_this->mMode = 0;
                    }
                }
            }
        }
        break;
    case 4:
        move_speed = 0.0f;
        if (i_this->mTimers[0] == 0) {
            if (i_this->mAnmID != ANM_GOMA_DASH) {
                anm_init(i_this, ANM_GOMA_DASH, 10.0f, J3DFrameCtrl::EMode_LOOP, l_HIO.dash_anm_speed);
            }

            i_this->mMode = 3;
            i_this->field_0x5b6 += (int)(cM_rndF(2.99f) + 1.0f);
            i_this->field_0x5b6 &= 3;

            i_this->mTargetMovePos.x = target_pos[i_this->field_0x5b6].x;
            i_this->mTargetMovePos.y = a_this->current.pos.y;
            i_this->mTargetMovePos.z = target_pos[i_this->field_0x5b6].z;
        }
        break;
    case 10:
        move_speed = 0.0f;
        a_this->current.angle.y = i_this->mAngleToPlayer + 0x8000;
        break;
    }

    cLib_addCalc2(&a_this->speedF, move_speed, 1.0f, l_HIO.dash_speed * 0.5f + 1.0f);

    if (a_this->current.angle.x < 0x1000 && a_this->current.angle.x > -0x1000) {
        cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x73c.y, 1.0f, 20.0f);
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
    } else if (a_this->current.angle.x > 0x7000 || a_this->current.angle.x < -0x7000) {
        cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x73c.y, 1.0f, 20.0f);
    } else {
        cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x73c.x, 1.0f, 20.0f);
        cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x73c.z, 1.0f, 20.0f);
    }
}

static void b_gm_beam(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz unused;
    f32 move_speed = 0.0f;

    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 2;
        i_this->field_0x6f4 = 0;
        anm_init(i_this, ANM_GOMA_ATTACK_A, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        break;
    case 2:
        i_this->field_0x1ad6 = 1;
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, ANM_GOMA_ATTACK_B, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mMode = 3;
            i_this->mTimers[0] = 200;
        }
        break;
    case 3:
        i_this->field_0x1ad6 = 1;
        cLib_addCalc2(&i_this->field_0x6c0, 1.0f, 1.0f, 0.1f);
        if (i_this->mTimers[0] == 0) {
            i_this->mMode = 4;
            anm_init(i_this, ANM_GOMA_ATTACK_C, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        }
        break;
    case 4:
        cLib_addCalc0(&i_this->field_0x6c0, 1.0f, 0.1f);
        if (i_this->mpModelMorf->isStop()) {
            i_this->field_0x6c0 = 0.0f;
            i_this->mAction = ACTION_MOVE;
            i_this->mTimers[0] = 0;
            i_this->mMode = 4;
        }
        break;
    }

    cLib_addCalc2(&i_this->speedF, move_speed, 1.0f, 1.0f + (0.5f * l_HIO.dash_speed));
}

static void b_gm_kogoma(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz kogoma_pos;
    cXyz sp54;
    cXyz sp48;
    f32 move_speed = l_HIO.dash_speed;

    J3DModel* model = i_this->mpModelMorf->getModel();

    MtxP now_mtx = mDoMtx_stack_c::get();
    MTXCopy(model->getAnmMtx(4), now_mtx);
    mDoMtx_stack_c::multVecZero(&sp54);

    now_mtx = mDoMtx_stack_c::get();
    MTXCopy(model->getAnmMtx(5), now_mtx);
    mDoMtx_stack_c::multVecZero(&sp48);

    kogoma_pos = sp54 + ((sp48 - sp54) * 0.5f);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_GOMA_DASH, 10.0f, J3DFrameCtrl::EMode_LOOP, l_HIO.dash_anm_speed);
        i_this->mTargetMovePos.x = 0.0f;
        i_this->mTargetMovePos.z = 0.0f;
        i_this->mMode = 1;
        /* fallthrough */
    case 1:
        sp54 = i_this->mTargetMovePos - a_this->current.pos;
        cLib_addCalcAngleS2(&a_this->current.angle.y, cM_atan2s(sp54.x, sp54.z) + 0x8000, 4, 0x300);

        if (i_this->field_0x1cf9 == 0 && i_this->mDemoMode == 0 && sp54.abs() < 400.0f) {
            i_this->mDemoMode = 10;
            i_this->field_0x1cf9 = 1;
        }

        if (sp54.abs() < 200.0f) {
            anm_init(i_this, ANM_GOMA_LAY_EGGS, 20.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimers[0] = 200;
            i_this->mMode = 2;
        }
        break;
    case 2:
        move_speed = 0.0f;
        if (i_this->mTimers[0] < 150 && i_this->mTimers[0] > 30 && (i_this->mTimers[0] & 3) == 0) {
            ko_ct = 0;
            fpcM_Search(s_ko_sub, a_this);
            if (ko_ct < 100) {
                csXyz angle(0, a_this->shape_angle.y, 0);
                fopAcM_createChild(PROC_E_GM, fopAcM_GetID(a_this), 0, &kogoma_pos, fopAcM_GetRoomNo(a_this), &angle, NULL, -1, 0);
            }
        }

        if (i_this->mTimers[0] == 0) {
            i_this->mAction = ACTION_MOVE;
            i_this->mTimers[0] = 0;
            i_this->mMode = 4;
            i_this->mTimers[2] = 200.0f + cM_rndF(100.0f);
        }
        break;
    }

    cLib_addCalc2(&a_this->speedF, move_speed, 1.0f, 1.0f + (0.5f * l_HIO.dash_speed));
}

static void b_gm_damage(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    int anm_frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        anm_init(i_this, ANM_GOMA_ROOF_DAMAGE, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        break;
    case 1:
        if (anm_frame >= 50) {
            if (anm_frame == 50) {
                a_this->speed.y = -100.0f;
            }

            a_this->current.pos.y += a_this->speed.y;
            a_this->speed.y -= 10.0f;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = ACTION_DROP;
            i_this->mMode = 0;
            anm_init(i_this, ANM_GOMA_FALL_LOOP, 0.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }
        break;
    }

    i_this->mInvincibilityTimer = 20;
    cLib_addCalc0(&a_this->speedF, 1.0f, 5.0f);
}

static void b_gm_drop(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp20;
    cXyz sp14;

    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        a_this->speedF = 0.0f;
        /* fallthrough */
    case 1:
        a_this->current.pos.y += a_this->speed.y;
        a_this->speed.y -= 10.0f;

        if (a_this->current.pos.y <= 1200.0f) {
            a_this->current.pos.y = 0.0f;
            a_this->speed.y = 0.0f;

            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0x4F, cXyz(0.0f, 1.0f, 0.0f));

            i_this->mMode = 2;
            anm_init(i_this, ANM_GOMA_LANDING, 0.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mTimers[0] = 300;

            fopAcM_effSmokeSet1(&i_this->field_0x1c84, &i_this->field_0x1c88, &a_this->current.pos, NULL, 15.0f, &a_this->tevStr, 1);
            Z2GetAudioMgr()->changeBgmStatus(1);
            dComIfGs_onOneZoneSwitch(1, -1);
        }
        break;
    case 2:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, ANM_GOMA_LANDING_WAIT, 0.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
        }

        a_this->current.pos.y = 0.0f;
        if (i_this->mTimers[0] == 0) {
            i_this->mMode = 3;
            anm_init(i_this, ANM_GOMA_RETURN, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            Z2GetAudioMgr()->changeBgmStatus(2);
        }
        break;
    case 3:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, ANM_GOMA_WAIT, 0.0f, J3DFrameCtrl::EMode_LOOP, l_HIO.wait_anm_speed);
            a_this->current.angle.x = 0;
            a_this->shape_angle.x = 0;
            i_this->mTimers[0] = 5;
            i_this->mMode = 4;

            cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
            sp20.x = 0.0f;
            sp20.y = 0.0f;
            sp20.z = -400.0f;
            MtxPosition(&sp20, &sp14);
            a_this->current.pos += sp14;
        }
        break;
    case 4:
        i_this->mInvincibilityTimer = 5;
        if (i_this->mTimers[0] == 0) {
            i_this->field_0x5d8 = 0.0f;
            i_this->field_0x5d4 = 0.0f;
            i_this->field_0x73c.y = 0.0f;
            i_this->mAction = ACTION_MOVE;
            i_this->mMode = 0;
        }
        break;
    case 10:
        anm_init(i_this, ANM_GOMA_LANDING_DAMAGE, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 11;
    case 11:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mMode = 3;
            anm_init(i_this, ANM_GOMA_RETURN, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mInvincibilityTimer = 200;
        }
        break;
    case 20:
        anm_init(i_this, ANM_GOMA_DEATH, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
        i_this->mMode = 21;
    case 21:
        if (i_this->mpModelMorf->getFrame() >= 160.0f) {
            cLib_addCalc2(&i_this->mBodyColorIntensity, -30.0f, 1.0f, 0.3f);
        }
    }
}

static void action(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz local_move;
    cXyz move_vec;

    i_this->mAngleToPlayer = fopAcM_searchPlayerAngleY(i_this);
    i_this->mPlayerDistance = fopAcM_searchPlayerDistance(i_this);

    damage_check(i_this);

    i_this->field_0x1ad6 = 0;

    switch (i_this->mAction) {
    case ACTION_WAIT:
        b_gm_wait(i_this);
        break;
    case ACTION_MOVE:
        b_gm_move(i_this);
        break;
    case ACTION_BEAM:
        b_gm_beam(i_this);
        break;
    case ACTION_KOGOMA:
        b_gm_kogoma(i_this);
        break;
    case ACTION_DAMAGE:
        b_gm_damage(i_this);
        break;
    case ACTION_DROP:
        b_gm_drop(i_this);
        break;
    }

    cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
    local_move.x = 0.0f;
    local_move.y = 0.0f;
    local_move.z = a_this->speedF;
    MtxPosition(&local_move, &move_vec);
    a_this->current.pos += move_vec;

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x1000);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 2, 0x1000);

    if (i_this->field_0x566 != 0) {
        daPy_getPlayerActorClass()->onBossRoomWait();
    }
}

static int foot_IK(b_gm_class* i_this, b_gm_foot* i_foot) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    int var_r28 = 0;
    dBgS_LinChk line_chk;

    i_foot->field_0x1c = i_foot->field_0x10;
    line_chk.Set(&i_this->field_0x730, &i_foot->field_0x10, i_this);
    if (dComIfG_Bgsp().LineCross(&line_chk)) {
        i_foot->field_0x1c = line_chk.GetCross();
        f32 temp_f31 = (i_foot->field_0x1c - i_foot->field_0x10).abs();

        i_foot->field_0x0 += 10.0f + (10.0f + (temp_f31 * 4.0f));
        if (i_foot->field_0x0 > 20000.0f) {
            i_foot->field_0x0 = 20000.0f;
        } else {
            var_r28 = 1;
        }

        i_foot->field_0x4 = 0.0f;
    } else {
        cLib_addCalc2(&i_foot->field_0x0, 0.0f, 1.0f, i_foot->field_0x4);
        cLib_addCalc2(&i_foot->field_0x4, 1000.0f, 1.0f, 50.0f + (2.0f * a_this->speedF));
    }

    i_foot->field_0x8[0] = i_foot->field_0x0 * l_HIO.bend_degree_1;

    if (i_foot->field_0x0 > 0.0f) {
        i_foot->field_0xa = i_foot->field_0x0 * l_HIO.bend_degree_2;
        i_foot->field_0xc = i_foot->field_0x0 * l_HIO.bend_degree_3;
    } else {
        i_foot->field_0xc = 0;
        i_foot->field_0xa = 0;
    }

    return var_r28;
}

static cXyz top_pos_data[] = {
    cXyz(260.0f, 0.0f, 0.0f),
    cXyz(280.0f, 0.0f, 0.0f),
    cXyz(300.0f, 0.0f, 0.0f),
    cXyz(280.0f, 0.0f, 0.0f),
};

static int top_j[] = {
    0x1B,
    0x1F,
    0x23,
    0x27,
    0x2B,
    0x2F,
    0x33,
    0x37,
    0x3C,
    0x41,
};

static void foot_IK_main(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz spC;
    b_gm_foot* foot_p = i_this->mFoot;
    J3DModel* model = i_this->mpModelMorf->getModel();

    cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
    spC.x = 0.0f;
    spC.y = 180.0f;
    spC.z = 0.0f;
    MtxPosition(&spC, &i_this->field_0x730);
    i_this->field_0x730 += a_this->current.pos;

    int var_r26 = 0;

    for (int i = 0; i < 10; i++, foot_p++) {
        MTXCopy(model->getAnmMtx(top_j[i]), *calc_mtx);
        MtxScale(l_HIO.check_size, l_HIO.check_size, l_HIO.check_size, 1);

        if (i >= 8) {
            spC.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&spC, &foot_p->field_0x10);
        } else {
            MtxPosition(&top_pos_data[i & 3], &foot_p->field_0x10);
        }

        if (foot_IK(i_this, foot_p)) {
            var_r26++;
            if (var_r26 < 80) {
                i_this->mpModelMorf->updateDL();
                i--;
                foot_p--;
            }
        }
    }
}

static void anm_se_set(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    int foot_idx = -1;
    if (i_this->mAnmID == ANM_GOMA_DASH) {
        if (i_this->mpModelMorf->checkFrame(3)) {
            foot_idx = 1;
        } else if (i_this->mpModelMorf->checkFrame(4)) {
            foot_idx = 6;
        } else if (i_this->mpModelMorf->checkFrame(6)) {
            foot_idx = 3;
        } else if (i_this->mpModelMorf->checkFrame(8)) {
            foot_idx = 5;
        } else if (i_this->mpModelMorf->checkFrame(10)) {
            foot_idx = 0;
        } else if (i_this->mpModelMorf->checkFrame(11)) {
            foot_idx = 7;
        } else if (i_this->mpModelMorf->checkFrame(14)) {
            foot_idx = 2;
        } else if (i_this->mpModelMorf->checkFrame(15)) {
            foot_idx = 4;
        } else if (i_this->mpModelMorf->checkFrame(18)) {
            foot_idx = 1;
        } else if (i_this->mpModelMorf->checkFrame(19)) {
            foot_idx = 6;
        } else if (i_this->mpModelMorf->checkFrame(21)) {
            foot_idx = 3;
        } else if (i_this->mpModelMorf->checkFrame(23)) {
            foot_idx = 5;
        } else if (i_this->mpModelMorf->checkFrame(25)) {
            foot_idx = 0;
        } else if (i_this->mpModelMorf->checkFrame(26)) {
            foot_idx = 7;
        } else if (i_this->mpModelMorf->checkFrame(29)) {
            foot_idx = 2;
        } else if (i_this->mpModelMorf->checkFrame(0)) {
            foot_idx = 4;
        }
    }

    if (i_this->mpModelMorf->checkFrame(1)) {
        if (i_this->mAnmID == ANM_GOMA_ATTACK_A) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_GO_V_BEAMATTACK_A, -1);
        } else if (i_this->mAnmID == ANM_GOMA_ATTACK_C) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_GO_V_BEAMATTACK_C, -1);
        } else if (i_this->mAnmID == ANM_GOMA_LANDING) {
            i_this->mCreatureSound.startCreatureSound(Z2SE_EN_GO_LANDING, 0, -1);
        } else if (i_this->mAnmID == ANM_GOMA_LANDING_DAMAGE) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_GO_V_LANDING_DAMAGE, -1);
        } else if (i_this->mAnmID == ANM_GOMA_RETURN) {
            i_this->mCreatureSound.startCreatureSound(Z2SE_EN_GO_RETURN, 0, -1);
        } else if (i_this->mAnmID == ANM_GOMA_ROOF_DAMAGE) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_GO_V_ROOF_DAMAGE, -1);
        } else if (i_this->mAnmID == ANM_GOMA_DEATH) {
            i_this->mCreatureSound.startCreatureVoice(Z2SE_EN_GO_V_DEATH, -1);
        }
    }

    if (i_this->mDemoMode == 0 && i_this->mAnmID == ANM_GOMA_DASH) {
        i_this->mCreatureSound.startCreatureSoundLevel(Z2SE_EN_GO_FOOTNOTE, 0, -1);
    } else if (i_this->mAnmID == ANM_GOMA_LANDING_WAIT) {
        i_this->mCreatureSound.startCreatureSoundLevel(Z2SE_EN_GO_LANDING_WAIT, 0, -1);
    } else if (i_this->mAnmID == ANM_GOMA_WAIT) {
        i_this->mCreatureSound.startCreatureSoundLevel(Z2SE_EN_GO_WAIT, 0, -1);
    }

    if (foot_idx >= 0) {
        cXyz sp60;
        csXyz angle;

        sp60 = a_this->current.pos - i_this->mFoot[foot_idx].field_0x1c;
        angle.y = cM_atan2s(sp60.x, sp60.z);
        angle.x = cM_atan2s(sp60.y, JMAFastSqrt(SQUARE(sp60.x) + SQUARE(sp60.z)));
        angle.z = 0;

        GXColor prim_col;
        prim_col.r = l_HIO.smoke_prim_R;
        prim_col.g = l_HIO.smoke_prim_G;
        prim_col.b = l_HIO.smoke_prim_B;
        prim_col.a = 0xFF;

        GXColor env_col;
        env_col.r = l_HIO.smoke_env_R;
        env_col.g = l_HIO.smoke_env_G;
        env_col.b = l_HIO.smoke_env_B;
        env_col.a = 0xFF;

        dComIfGp_particle_setColor(0x81EF, &i_this->mFoot[foot_idx].field_0x1c, &a_this->tevStr, &prim_col, &env_col, l_HIO.smoke_blend, l_HIO.smoke_alpha, &angle, NULL, NULL, -1, NULL);
        dComIfGp_particle_set(0x81EE, &i_this->mFoot[foot_idx].field_0x1c, &angle, NULL);
    }
}

static void cam_3d_morf(b_gm_class* i_this, f32 i_scale) {
    cLib_addCalc2(&i_this->mDemoCamCenter.x, i_this->mDemoCamCenterTarget.x, i_scale, i_this->mDemoCamCenterSpeed.x * i_this->field_0x1cdc);
    cLib_addCalc2(&i_this->mDemoCamCenter.y, i_this->mDemoCamCenterTarget.y, i_scale, i_this->mDemoCamCenterSpeed.y * i_this->field_0x1cdc);
    cLib_addCalc2(&i_this->mDemoCamCenter.z, i_this->mDemoCamCenterTarget.z, i_scale, i_this->mDemoCamCenterSpeed.z * i_this->field_0x1cdc);

    cLib_addCalc2(&i_this->mDemoCamEye.x, i_this->mDemoCamEyeTarget.x, i_scale, i_this->mDemoCamEyeSpeed.x * i_this->field_0x1cdc);
    cLib_addCalc2(&i_this->mDemoCamEye.y, i_this->mDemoCamEyeTarget.y, i_scale, i_this->mDemoCamEyeSpeed.y * i_this->field_0x1cdc);
    cLib_addCalc2(&i_this->mDemoCamEye.z, i_this->mDemoCamEyeTarget.z, i_scale, i_this->mDemoCamEyeSpeed.z * i_this->field_0x1cdc);
}

static void cam_spd_set(b_gm_class* i_this) {
    i_this->mDemoCamEyeSpeed.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
    i_this->mDemoCamEyeSpeed.y = fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
    i_this->mDemoCamEyeSpeed.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);

    i_this->mDemoCamCenterSpeed.x = fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
    i_this->mDemoCamCenterSpeed.y = fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
    i_this->mDemoCamCenterSpeed.z = fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);

    i_this->field_0x1cdc = 0.0f;
}

static void demo_camera(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* sp20 = dComIfGp_getCamera(0);
    cXyz spC8;
    cXyz spBC;
    cXyz spB0;
    cXyz spA4;
    
    s8 sp10 = 0;

    switch (i_this->mDemoMode) {
    case 1:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoMode = 2;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 55.0f;

        player->changeOriginalDemo();
        player->changeDemoMode(daPy_demo_c::DEMO_LOOK_AROUND_e, 0, 0, 0);

        spBC.set(0.0f, 0.0f, 2418.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&spBC, 0x8000, 0);

        dComIfGp_getEvent().startCheckSkipEdge(i_this);
        
        camera->mCamera.SetTrimSize(3);
        mDoAud_seStart(Z2SE_EN_GO_DEMO_OP, NULL, 0, 0);
        Z2GetAudioMgr()->subBgmStart(Z2BGM_GOMA_D01);
    case 2:
        if (i_this->mDemoModeTimer >= 1) {
            if (i_this->mDemoModeTimer == 1) {
                i_this->mDemoCamCenter.set(-400.0f, 130.0f, 1883.0f);
                i_this->mDemoCamEye.set(0.0f, 197.0f, 2680.0f);
            }

            cLib_addCalc2(&i_this->mDemoCamCenter.x, 400.0f, 0.05f, i_this->field_0x1cdc * 4.0f);
            cLib_addCalc2(&i_this->field_0x1cdc, 1.0f, 1.0f, 0.04f);
            i_this->mDemoCamEye.z -= 1.0f;
        }

        if (i_this->mDemoModeTimer == 140) {
            i_this->mDemoMode = 3;
            i_this->mDemoModeTimer = 0;

            spBC.set(-781.0f, 0.0f, -601.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&spBC, -0x6249, 0);

            player->changeDemoMode(daPy_demo_c::DEMO_UNK_2_e, 0, 0, 0);
            spBC.set(-1267.0f, 0.0f, -1208.0f);
            player->changeDemoPos0(&spBC);
            
            i_this->mDemoCamCenter.set(-1090.0f, 227.0f, -1070.0f);
            i_this->mDemoCamEye.set(-688.0f, 155.0f, -680.0f);
        }
        break;
    case 3:
        if (i_this->mDemoModeTimer >= 40) {
            cLib_addCalc2(&i_this->mDemoCamCenter.y, 317.0f, 0.05f, 0.8f);
        }

        if (i_this->mDemoModeTimer == 170) {
            daPy_getPlayerActorClass()->changeDemoMode(daPy_demo_c::DEMO_LOOK_AROUND_e, 0, 0, 0);
        }

        if (i_this->mDemoModeTimer == 230) {
            i_this->field_0x6f5 = 1;
            i_this->field_0x1cfc = 1;
        }

        if (i_this->mDemoModeTimer == 250) {
            player->changeDemoMode(daPy_demo_c::DEMO_TURN_BACK_e, 0, 0, 0);
        }

        if (i_this->mDemoModeTimer == 277) {
            i_this->mDemoMode = 4;
            i_this->mDemoModeTimer = 0;
            
            i_this->field_0x1ce4 = player->shape_angle.y;
            i_this->mDemoCamCenter.set(-973.0f, 192.0f, -991.0f);
            i_this->mDemoCamEye.set(-1368.0f, 158.0f, -1397.0f);
            i_this->field_0x1cdc = 0.0f;
            i_this->field_0x6f5 = 0;
            i_this->field_0x1cfc = 2;
        }
        break;
    case 4:
        cLib_addCalcAngleS2(&i_this->field_0x1ce4, i_this->mAngleToPlayer + 0x8000, 1, 0x1000);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&player->current.pos, i_this->field_0x1ce4, 0);

        if (i_this->mDemoModeTimer == 25) {
            daPy_getPlayerActorClass()->changeDemoMode(daPy_demo_c::DEMO_UNK_23_e, 1, 2, 0);
        }

        if (i_this->mDemoModeTimer >= 55) {
            cLib_addCalc2(&i_this->mDemoCamCenter.y, 780.0f, 0.3f, 50.0f);

            if (i_this->mDemoModeTimer >= 260) {
                if (i_this->mDemoModeTimer == 260) {
                    i_this->field_0x1cdc = 0.0f;
                }

                cLib_addCalc2(&i_this->mDemoCamFovy, 40.0f, 0.2f, i_this->field_0x1cdc);
                cLib_addCalc2(&i_this->field_0x1cdc, 1.0f, 1.0f, 0.05f);

                if (i_this->mDemoModeTimer == 280) {
                    fopMsgM_messageSetDemo(0x484);
                }

                if (i_this->mDemoModeTimer == 390) {
                    i_this->mMode = 4;
                    i_this->mTimers[2] = cM_rndF(100.0f) + 200.0f;
                }

                if (i_this->mDemoModeTimer == 420) {
                    sp10 = 1;
                    Z2GetAudioMgr()->bgmStart(Z2BGM_GOMA_BTL01, 0, 0);
                    a_this->field_0x566 = 1;
                }
            } else if (i_this->mDemoModeTimer >= 205) {
                cLib_addCalc2(&i_this->mDemoCamFovy, 10.0f, 0.2f, 3.0f);
            } else if (i_this->mDemoModeTimer >= 85) {
                cLib_addCalc2(&i_this->mDemoCamFovy, 20.0f, 0.05f, i_this->field_0x1cdc * 5.0f);
                cLib_addCalc2(&i_this->field_0x1cdc, 1.0f, 1.0f, 0.02f);
            }

            if (i_this->mDemoModeTimer >= 125) {
                i_this->field_0x1ad6 = 1;

                if (i_this->mDemoModeTimer == 125) {
                    i_this->field_0x1ade = 60;
                }

                if (i_this->mDemoModeTimer == 205) {
                    i_this->field_0x1ae0 = 0.0f;
                }
            }
        }
        break;
    case 10:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoMode = 11;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 55.0f;
        
        camera->mCamera.SetTrimSize(3);
        i_this->field_0x1ce0 = 900.0f;
        i_this->field_0x1cdc = 0.0f;
    case 11:
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        spC8.x = i_this->field_0x1ce0;
        spC8.y = -1500.0f;
        spC8.z = 1300.0f;
        MtxPosition(&spC8, &spB0);

        spB0 += a_this->current.pos;
        spA4 = a_this->eyePos;

        cLib_addCalc2(&i_this->field_0x1ce0, -900.0f, 0.1f, i_this->field_0x1cdc * 12.0f);
        cLib_addCalc2(&i_this->field_0x1cdc, 1.0f, 1.0f, 0.05f);

        if (i_this->mDemoModeTimer == 0) {
            i_this->mDemoCamCenter = spA4;
            i_this->mDemoCamEye = spB0;
        } else {
            cLib_addCalc2(&i_this->mDemoCamCenter.x, spA4.x, 0.1f, 5.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, spA4.z, 0.1f, 5.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.x, spB0.x, 0.1f, 30.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.z, spB0.z, 0.1f, 30.0f);
        }

        if (i_this->mAction != ACTION_KOGOMA) {
            sp10 = 1;
        }
        break;
    case 20:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoMode = 21;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 55.0f;
        
        camera->mCamera.SetTrimSize(3);
        i_this->field_0x1cdc = 0.0f;
    case 21:
        cMtx_YrotS(*calc_mtx, cM_atan2s(-a_this->current.pos.x, -a_this->current.pos.z));
        spC8.x = 300.0f;
        spC8.y = 1200.0f;
        spC8.z = 1300.0f;
        MtxPosition(&spC8, &spB0);

        spB0 += a_this->current.pos;
        spA4 = a_this->eyePos;
        spA4.y += 350.0f;

        if (i_this->mDemoModeTimer == 0) {
            i_this->mDemoCamCenter = spA4;
            i_this->mDemoCamEye = spB0;
        } else {
            cLib_addCalc2(&i_this->mDemoCamCenter.x, spA4.x, 0.1f, 5.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, spA4.z, 0.1f, 5.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.x, spB0.x, 0.1f, 30.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.z, spB0.z, 0.1f, 30.0f);
        }

        if (i_this->mDemoModeTimer == 40) {
            sp10 = 1;
        }
        break;
    case 30:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoMode = 31;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 55.0f;
        camera->mCamera.SetTrimSize(3);

        i_this->field_0x1cdc = 0.0f;
        i_this->field_0x1ce0 = 1800.0f;
        i_this->field_0x1ce4 = 0;
        i_this->field_0x1ce8 = -10000;
        i_this->field_0x1cdc = 0.0f;

        player->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(daPy_demo_c::DEMO_UNK_23_e, 1, 2, 0);        

        Z2GetAudioMgr()->bgmStreamPrepare(0x200004E);
        Z2GetAudioMgr()->bgmStreamPlay();
    case 31:
        if (i_this->mDemoModeTimer < 10) {
            cMtx_YrotS(*calc_mtx, cM_atan2s(-a_this->current.pos.x, -a_this->current.pos.z));
            spC8.x = 1100.0f;
            spC8.y = 0.0f;
            spC8.z = -1000.0f;
            MtxPosition(&spC8, &spBC);
            spBC += a_this->current.pos;
            spBC.y = 0.0f;

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&spBC, i_this->mAngleToPlayer + 0x8000, 0);
        }

        cMtx_YrotS(*calc_mtx, i_this->field_0x1ce4 + cM_atan2s(-a_this->current.pos.x, -a_this->current.pos.z));
        cMtx_XrotM(*calc_mtx, i_this->field_0x1ce8);

        spC8.x = 0.0f;
        spC8.y = 0.0f;
        spC8.z = i_this->field_0x1ce0;
        MtxPosition(&spC8, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += a_this->current.pos;
        i_this->mDemoCamCenter = a_this->eyePos;
        i_this->mDemoCamCenter.y += 350.0f;

        if (i_this->mDemoModeTimer > 120) {
            cLib_addCalcAngleS2(&i_this->field_0x1ce4, 23000, 0x10, i_this->field_0x1cdc * 230.0f);
            cLib_addCalcAngleS2(&i_this->field_0x1ce8, -700, 0x10, i_this->field_0x1cdc * 93.0f);
            cLib_addCalc2(&i_this->field_0x1cdc, 1.0f, 1.0f, 0.04f);
        }

        if (i_this->mDemoModeTimer == 280) {
            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(2), mDoMtx_stack_c::get());
            mDoMtx_stack_c::multVecZero(&spC8);

            dComIfGp_particle_set(0x8B42, &spC8, NULL, NULL);
            fopAcM_createDisappear(a_this, &spC8, 49, 0, 0xFF);
            i_this->mIsDisappear = true;
            dComIfGs_onOneZoneSwitch(5, -1);
        }

        if (i_this->mDemoModeTimer == 320) {
            player->changeDemoMode(daPy_demo_c::DEMO_SWORD_UNEQUIP_SP_e, 1, 0, 0);
        }

        if (i_this->mDemoModeTimer != 340) {
            break;
        }
        
        i_this->mDemoMode = 32;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 65.0f;
    case 32:
        cMtx_YrotS(*calc_mtx, player->shape_angle.y + 8000);
        spC8.x = 0.0f;
        spC8.y = 90.0f;
        spC8.z = 110.0f;
        MtxPosition(&spC8, &i_this->mDemoCamEye);

        i_this->mDemoCamEye += player->current.pos;
        i_this->mDemoCamCenter = player->current.pos;
        i_this->mDemoCamCenter.y += 140.0f;

        if (i_this->mDemoModeTimer & 1) {
            int sp1C = i_this->mDemoModeTimer >> 1;
            int params;
            if (sp1C == 0) {
                params = 3;
            } else {
                params = 1;
            }

            cMtx_YrotS(*calc_mtx, sp1C * 0x1300);
            spC8.x = 0.0f;
            spC8.y = 0.0f;

            if (sp1C == 0) {
                spC8.z = 0.0f;
            } else {
                spC8.z = sp1C * 5.0f + 300.0f;
            }

            MtxPosition(&spC8, &spBC);
            spBC += a_this->eyePos;

            csXyz angle(0, cM_rndF(0x10000), 0);
            fopAcM_createChild(PROC_E_GM, fopAcM_GetID(a_this), params, &spBC, fopAcM_GetRoomNo(a_this), &angle, NULL, -1, NULL);
        }

        if (i_this->mDemoModeTimer == 90) {
            player->changeDemoParam1(1);
        }

        if (i_this->mDemoModeTimer < 90) {
            cLib_addCalc2(&i_this->mDemoCamFovy, 55.0f, 0.05f, 0.1f);
        } else {
            cLib_addCalc2(&i_this->mDemoCamFovy, 45.0f, 0.1f, 0.5f);
        }

        if (i_this->mDemoModeTimer == 130) {
            i_this->mDemoCamCenterTarget.set(a_this->eyePos.x, 0.0f, a_this->eyePos.z);
            i_this->mDemoCamEyeTarget = i_this->mDemoCamEye;
            i_this->mDemoCamEyeTarget.y += 40.0f;

            cam_spd_set(i_this);
            i_this->mDemoMode = 33;
            i_this->mDemoModeTimer = 0;
        }
        break;
    case 33:
        if (i_this->mDemoModeTimer >= 60) {
            cLib_addCalc2(&i_this->mDemoCamFovy, 55.0f, 0.02f, 0.1f);
        }

        cam_3d_morf(i_this, 0.1f);
        cLib_addCalc2(&i_this->mDemoCamEye.y, 400.0f, 0.05f, i_this->field_0x1cdc * 300.0f);
        cLib_addCalc2(&i_this->field_0x1cdc, 0.02f, 1.0f, 0.001f);

        if (i_this->mDemoModeTimer == 60) {
            fpcM_Search(s_ko2_move, i_this);
        }

        if (i_this->mDemoModeTimer == 230) {
            fpcM_Search(s_ko_move, i_this);
        }

        if (i_this->mDemoModeTimer == 290) {
            sp10 = 1;
            Z2GetAudioMgr()->bgmStart(Z2BGM_GOMA_BTL02, 0, 0);
        }
        break;
    case 40:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoMode = 41;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 55.0f;
        camera->mCamera.SetTrimSize(3);

        i_this->field_0x1cdc = 0.0f;
        player->changeOriginalDemo();
        
        a_this->current.pos = i_this->field_0x1cec;
        a_this->eyePos = i_this->field_0x1cec;

        i_this->field_0x1ce4 = -25000;
        i_this->field_0x1ce8 = -2500;
        i_this->field_0x1ce0 = 600.0f;
        i_this->field_0x1ce6 = cM_atan2s(-i_this->field_0x1cec.x, -i_this->field_0x1cec.z);

        if (daPy_py_c::checkNowWolf()) {
            daPy_getPlayerActorClass()->changeDemoMode(daPy_demo_c::DEMO_METAMORPHOSE_UNK2_e, 0, 0, 0);
            a_this->field_0x567 = 1;
        } else {
            daPy_getPlayerActorClass()->changeDemoMode(daPy_demo_c::DEMO_UNK_23_e, 1, 2, 0);
        }
    case 41:
        if (i_this->mDemoModeTimer == 55 && a_this->field_0x567 != 0) {
            daPy_getPlayerActorClass()->changeDemoMode(daPy_demo_c::DEMO_UNK_23_e, 1, 2, 0);
        }

        cLib_addCalcAngleS2(&i_this->field_0x1ce4, -4000, 0x10, 200);

        cMtx_YrotS(*calc_mtx, i_this->field_0x1ce6);
        spC8.x = 0.0f;
        spC8.y = 0.0f;
        spC8.z = 350.0f;
        MtxPosition(&spC8, &spBC);
        spBC += i_this->field_0x1cec;

        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&spBC, i_this->field_0x1ce6 + 0x8000, 0);

        cMtx_YrotS(*calc_mtx, i_this->field_0x1ce6 + i_this->field_0x1ce4);
        cMtx_XrotM(*calc_mtx, i_this->field_0x1ce8);

        spC8.x = 0.0f;
        spC8.y = 0.0f;
        spC8.z = i_this->field_0x1ce0;
        MtxPosition(&spC8, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += i_this->field_0x1cec;
        i_this->mDemoCamCenter = i_this->field_0x1cec;
        i_this->mDemoCamCenter.y += 30.0f;

        if (i_this->mDemoModeTimer == 160) {
            csXyz angle(0, player->shape_angle.y - 0x4000, 0);
            fopAcM_createItemForBoss(&i_this->field_0x1cec, fpcNm_ITEM_UTAWA_HEART, fopAcM_GetRoomNo(a_this), &angle, NULL, 10.0f, 30.0f, -1);
        }

        if (i_this->mDemoModeTimer == 190) {
            i_this->field_0x1cec.y += 600.0f;
            fopAcM_create(PROC_OBJ_YSTONE, 0, &i_this->field_0x1cec, fopAcM_GetRoomNo(a_this), NULL, NULL, -1);
            i_this->mDemoMode = 42;
            i_this->mDemoModeTimer = 0;
        }
        break;
    case 42: {
        if (i_this->mDemoModeTimer == 30) {
            daPy_getPlayerActorClass()->changeDemoMode(daPy_demo_c::DEMO_UNEQUIP_e, 0, 0, 0);
        }

        cLib_addCalcAngleS2(&i_this->field_0x1ce4, 0, 0x10, 40);

        cMtx_YrotS(*calc_mtx, i_this->field_0x1ce6 + i_this->field_0x1ce4);
        cMtx_XrotM(*calc_mtx, i_this->field_0x1ce8);
        spC8.x = 0.0f;
        spC8.y = 0.0f;
        spC8.z = i_this->field_0x1ce0;
        MtxPosition(&spC8, &i_this->mDemoCamEye);
        i_this->mDemoCamEye.x += i_this->field_0x1cec.x;
        i_this->mDemoCamEye.z += i_this->field_0x1cec.z;

        cLib_addCalc2(&i_this->mDemoCamCenter.y, i_this->field_0x1cec.y - 60.0f, 0.05f, 5.0f);

        if (i_this->mDemoModeTimer > 200) {
            cLib_addCalc2(&i_this->field_0x1cec.y, 100.0f, 0.05f, 0.7f);
        }

        obj_ystone_class* ystone = (obj_ystone_class*)fopAcM_SearchByName(PROC_OBJ_YSTONE);
        if (ystone != NULL) {
            if (i_this->mDemoModeTimer > 200) {
                cLib_addCalc2(&i_this->field_0x1cec.y, 100.0f, 0.05f, 1.0f);
            }

            ystone->setCurrentPos(i_this->field_0x1cec);
        }

        if (i_this->mDemoModeTimer == 335) {
            cXyz pos(0.0f, 0.0f, 0.0f);
            csXyz angle(0, 0, 0);
            fopAcM_createWarpHole(&pos, &angle, fopAcM_GetRoomNo(a_this), 1, 1, 0xFF);
            dComIfGs_onStageBossEnemy();
        }

        if (i_this->mDemoModeTimer == 341) {
            sp10 = 2;
        }
        break;
    }
    case 50:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoMode = 51;
        i_this->mDemoModeTimer = 0;
        i_this->mDemoCamFovy = 55.0f;
        
        camera->mCamera.SetTrimSize(3);
        i_this->field_0x1cdc = 0.0f;
    case 51:
        cMtx_YrotS(*calc_mtx, cM_atan2s(-a_this->current.pos.x, -a_this->current.pos.z));
        spC8.x = 300.0f;
        spC8.y = 1200.0f;
        spC8.z = 1300.0f;
        MtxPosition(&spC8, &spB0);

        spB0 += i_this->field_0x1cec;
        spA4 = i_this->field_0x1cec;
        spA4.y += 350.0f;

        if (i_this->mDemoModeTimer == 0) {
            i_this->mDemoCamCenter = spA4;
            i_this->mDemoCamEye = spB0;
        } else {
            cLib_addCalc2(&i_this->mDemoCamCenter.x, spA4.x, 0.1f, 5.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, spA4.z, 0.1f, 5.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.x, spB0.x, 0.1f, 30.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.z, spB0.z, 0.1f, 30.0f);
        }

        if (i_this->mDemoModeTimer == 60) {
            i_this->mDemoMode = 41;
            i_this->mDemoModeTimer = 40;
            i_this->mDemoCamFovy = 55.0f;
            camera->mCamera.SetTrimSize(3);
            i_this->field_0x1cdc = 0.0f;

            player->changeOriginalDemo();
        
            a_this->current.pos = i_this->field_0x1cec;
            a_this->eyePos = i_this->field_0x1cec;

            i_this->field_0x1ce4 = -8000;
            i_this->field_0x1ce8 = -2500;
            i_this->field_0x1ce0 = 600.0f;
            i_this->field_0x1ce6 = cM_atan2s(-i_this->field_0x1cec.x, -i_this->field_0x1cec.z);

            if (daPy_py_c::checkNowWolf()) {
                daPy_getPlayerActorClass()->changeDemoMode(daPy_demo_c::DEMO_METAMORPHOSE_UNK2_e, 0, 0, 0);
                a_this->field_0x567 = 1;
            } else {
                daPy_getPlayerActorClass()->changeDemoMode(daPy_demo_c::DEMO_UNK_23_e, 1, 2, 0);
            }
        }
        break;
    }

    if (i_this->mDemoMode != 0 && i_this->mDemoMode < 10) {
        if (dComIfGp_getEvent().checkSkipEdge()) {
            i_this->mMode = 4;
            i_this->mTimers[2] = cM_rndF(100.0f) + 200.0f;
            sp10 = 1;
            i_this->field_0x1cfc = 2;
            i_this->field_0x6f5 = 0;

            Z2GetAudioMgr()->subBgmStop();
            Z2GetAudioMgr()->bgmStart(Z2BGM_GOMA_BTL01, 0, 0);
            a_this->field_0x566 = 1;
        }
    }

    if (sp10 != 0) {
        if (sp10 == 1) {
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            spC8.x = 0.0f;
            spC8.y = 100.0f;
            spC8.z = -250.0f;
            MtxPosition(&spC8, &i_this->mDemoCamEye);
            i_this->mDemoCamEye += player->current.pos;
            i_this->mDemoCamCenter = player->current.pos;
            i_this->mDemoCamCenter.y += 120.0f;
        }

        camera->mCamera.Reset(i_this->mDemoCamCenter, i_this->mDemoCamEye);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();

        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->mDemoMode = 0;
    }

    if (i_this->mDemoMode > 0) {
        cXyz center;
        cXyz eye;
        center = i_this->mDemoCamCenter;
        eye = i_this->mDemoCamEye;

        camera->mCamera.Set(center, eye, i_this->mDemoCamFovy, 0);
        i_this->mDemoModeTimer++;
    }
}

static int daB_GM_Execute(b_gm_class* i_this) {
    if (cDmrNowMidnaTalk()) {
        return 1;
    }

    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz spD4, spC8;

    #if DEBUG
    if (mDoCPd_c::getTrigA(2) && i_this->mDemoMode == 0 && !i_this->mIsDisappear) {
        i_this->mAction = 0xB;
        i_this->mMode = 0x14;
        i_this->mDemoMode = 0x1E;
        a_this->current.pos.y = VREG_F(18);
        a_this->speedF = 0.0f;
    }
    #endif

    for (int i = 0; i < 4; i++) {
        mDoMtx_stack_c::transS(target_pos[i].x, target_pos[i].y, target_pos[i].z);

        f32 target_size = 1.0f;
        if (!i_this->mIsDisappear && a_this->current.pos.y > 1000.0f) {
            spD4.x = a_this->eyePos.x - target_pos[i].x;
            spD4.z = a_this->eyePos.z - target_pos[i].z;
            if (JMAFastSqrt((spD4.x * spD4.x) + (spD4.z * spD4.z)) < NREG_F(18) + 500.0f) {
                target_size = 0.0f;
            }
        }

        if (target_size > 0.1f) {
            if (i_this->field_0x1cfc == i + 3) {
                cLib_addCalc0(&i_this->mKankyoBlend, 1.0f, 0.1f);
            }
        } else if (i_this->field_0x6c0 < 0.1f) {
            i_this->field_0x1cfc = i + 3;
            cLib_addCalc2(&i_this->mKankyoBlend, 1.0f, 1.0f, 0.1f);
        }

        if (i == 0 && i_this->field_0x6f5 != 0) {
            target_size = 0.0f;
        }

        cLib_addCalc2(&i_this->mSpotLightSize[i], target_size, 1.0f, 0.1f);
        mDoMtx_stack_c::scaleM(i_this->mSpotLightSize[i], 1.0f, i_this->mSpotLightSize[i]);
        mDoMtx_stack_c::YrotM(i * 0x3100);
        i_this->mpSpotLightModel[i]->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    i_this->mpSpotLightBtk->play();

    if (i_this->mIsDisappear) {
        if (i_this->mDemoMode == 0) {
            daE_GM_c* kogoma = (daE_GM_c*)fpcM_Search(s_ko2_get, a_this);
            if (kogoma != NULL) {
                fopAc_ac_c* ko_actor = kogoma;
                i_this->field_0x1cec = ko_actor->current.pos;
                i_this->field_0x1cf8 = kogoma->CheckDeath();

                if (i_this->field_0x1cf8 != 0) {
                    if (i_this->field_0x1cf8 == 1) {
                        i_this->mDemoMode = 40;
                    } else {
                        i_this->mDemoMode = 50;
                    }
                    Z2GetAudioMgr()->bgmStop(0x1E, 0);
                    Z2GetAudioMgr()->bgmStreamPrepare(0x200004F);
                    Z2GetAudioMgr()->bgmStreamPlay();
                    a_this->field_0x566 = 0;
                }
            }
        }

        demo_camera(i_this);
        return 1;
    }

    daPy_getPlayerActorClass()->onNeckSearchWide();
    i_this->mCounter++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->mInvincibilityTimer != 0) {
        i_this->mInvincibilityTimer--;
    }

    action(i_this);

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
    mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
    mDoMtx_stack_c::XrotM(a_this->shape_angle.x);
    mDoMtx_stack_c::scaleM(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    mDoMtx_stack_c::transM(0.0f, 0.0f, KREG_F(8) + 200.0f);

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    i_this->mBeamSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));

    anm_se_set(i_this);

    cLib_addCalc2(&i_this->mZoomBtkFrame, i_this->field_0x1ae0, 1.0f, 1.0f);
    i_this->mpZoomBtk->setFrame(i_this->mZoomBtkFrame);
    i_this->mpModelMorf->modelCalc();

    foot_IK_main(i_this);

    f32 sph_base_size = l_HIO.check_size * l_HIO.model_size;
    MTXCopy(model->getAnmMtx(0x15), *calc_mtx);
    MtxScale(l_HIO.check_size, l_HIO.check_size, l_HIO.check_size, 1);
    spD4.set(JREG_F(8) + -20.0f, JREG_F(9) + 0.0f, JREG_F(10) + 0.0f);
    MtxPosition(&spD4, &spC8);

    if (i_this->mInvincibilityTimer != 0 || i_this->field_0x1ad6 == 0) {
        spC8.y += 20000.0f;
    }

    i_this->mCoreSph.SetC(spC8);
    i_this->mCoreSph.SetR((JREG_F(11) + 160.0f) * sph_base_size);
    dComIfG_Ccsp()->Set(&i_this->mCoreSph);

    spD4.set(JREG_F(12) + 50.0f, JREG_F(13) + -20.0f, JREG_F(14) + 0.0f);
    MtxPosition(&spD4, &a_this->eyePos);
    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += JREG_F(15) + 70.0f;

    if (i_this->field_0x1ad6 == 0) {
        MTXCopy(model->getAnmMtx(6), *calc_mtx);
        MtxScale(l_HIO.check_size, l_HIO.check_size, l_HIO.check_size, 1);
        spD4.set(JREG_F(0) + 170.0f, JREG_F(1) + 0.0f, JREG_F(2) + 0.0f);
        MtxPosition(&spD4, &spC8);

        if (i_this->mInvincibilityTimer != 0) {
            spC8.y += 20000.0f;
        }

        i_this->mBodySph[0].SetC(spC8);
        i_this->mBodySph[0].SetR((JREG_F(3) + 190.0f) * sph_base_size);
    }

    MTXCopy(model->getAnmMtx(2), *calc_mtx);
    MtxScale(l_HIO.check_size, l_HIO.check_size, l_HIO.check_size, 1);
    spD4.set(JREG_F(4) + 100.0f, JREG_F(5) + 0.0f, JREG_F(6) + 0.0f);
    MtxPosition(&spD4, &spC8);

    if (i_this->mInvincibilityTimer != 0) {
        spC8.y += 20000.0f;
    }
    i_this->mBodySph[1].SetC(spC8);
    i_this->mBodySph[1].SetR((JREG_F(7) + 180.0f) * sph_base_size);

    for (int i = 0; i < 2; i++) {
        dComIfG_Ccsp()->Set(&i_this->mBodySph[i]);
    }

    for (int i = 0; i < 8; i++) {
        MTXCopy(model->getAnmMtx(top_j[i]), *calc_mtx);
        MtxScale(l_HIO.check_size, l_HIO.check_size, l_HIO.check_size, 1);
        spD4.set(AREG_F(10) + 150.0f, AREG_F(11) + 0.0f, AREG_F(12) + 0.0f);
        MtxPosition(&spD4, &spC8);

        i_this->mFootSph[i].SetR((TREG_F(13) + 50.0f) * sph_base_size);
        i_this->mFootSph[i].SetC(spC8);

        if (i_this->mAnmID == ANM_GOMA_DASH && a_this->current.angle.x < 0x1000 && a_this->current.angle.x > -0x1000) {
            i_this->mFootSph[i].OnAtSetBit();
        } else {
            i_this->mFootSph[i].OffAtSetBit();
        }

        dComIfG_Ccsp()->Set(&i_this->mFootSph[i]);
    }

    cXyz spBC(0.0f, 0.0f, 0.0f);
    if (i_this->field_0x1aac == 0) {
        spBC.y = -20000.0f;
    }

    MTXCopy(model->getAnmMtx(0x3B), *calc_mtx);
    MtxScale(l_HIO.check_size, l_HIO.check_size, l_HIO.check_size, 1);
    spD4.set(XREG_F(0) + 80.0f, XREG_F(1), XREG_F(2));
    MtxPosition(&spD4, &spC8);
    i_this->mHandSph[0].SetC(spC8 + spBC);
    i_this->mHandSph[0].SetR((XREG_F(3) + 80.0f) * sph_base_size);

    MTXCopy(model->getAnmMtx(0x40), *calc_mtx);
    MtxScale(l_HIO.check_size, l_HIO.check_size, l_HIO.check_size, 1);
    spD4.set(XREG_F(0), XREG_F(1), XREG_F(2));
    MtxPosition(&spD4, &spC8);
    i_this->mHandSph[1].SetC(spC8 + spBC);
    i_this->mHandSph[1].SetR((XREG_F(3) + 60.0f) * sph_base_size);

    dComIfG_Ccsp()->Set(&i_this->mHandSph[0]);
    dComIfG_Ccsp()->Set(&i_this->mHandSph[1]);

    spC8 = player->eyePos - a_this->eyePos;

    cMtx_XrotS(*calc_mtx, -a_this->shape_angle.x);
    cMtx_YrotM(*calc_mtx, -a_this->shape_angle.y);
    MtxPosition(&spC8, &spD4);

    s16 sp14, sp12;
    sp12 = -cM_atan2s(spD4.z, spD4.y);
    sp14 = (s16)cM_atan2s(spD4.x, spD4.y);
    if (sp12 > 0x1400) {
        sp12 = 0x1400;
    } else if (sp12 < -0x1400) {
        sp12 = -0x1400;
    }

    if (sp14 > 0x1400) {
        sp14 = 0x1400;
    } else if (sp14 < -0x1400) {
        sp14 = -0x1400;
    }

    s16 sp10, spE;
    if (i_this->field_0x1ade != 0) {
        i_this->field_0x1ade--;

        spE = 120.0f * (i_this->field_0x1ade * cM_ssin(i_this->mCounter * 1200));
        sp10 = 120.0f * (i_this->field_0x1ade * cM_ssin(i_this->mCounter * 2000));
    } else {
        spE = 0;
        sp10 = 0;
    }

    cLib_addCalcAngleS2(&i_this->field_0x1adc, sp12 + spE, 4, 0x1000);
    cLib_addCalcAngleS2(&i_this->field_0x1ada, sp14 + sp10, 4, 0x1000);

    if (i_this->field_0x6c0 > 0.1f) {
        dBgS_LinChk line_chk;
        MTXCopy(model->getAnmMtx(0x15), *calc_mtx);
        spD4.set(XREG_F(8) + 120.0f, XREG_F(9), XREG_F(10));
        MtxPosition(&spD4, &spC8);

        if (i_this->field_0x6f4 == 0) {
            i_this->field_0x6cc = spC8;
            i_this->field_0x6cc.y = 0.0f;
            i_this->field_0x6c4 = 0.0f;
        }

        cXyz spB0;
        cLib_addCalc2(&i_this->field_0x6c4, AREG_F(17) + 40.0f, 1.0f, 1.0f);
        spD4 = player->current.pos - i_this->field_0x6cc;

        if (i_this->field_0x6f4 == 0) {
            i_this->field_0x6c8 = cM_atan2s(spD4.x, spD4.z);
        } else {
            f32 var_f30 = (JMAFastSqrt((spD4.x * spD4.x) + (spD4.z * spD4.z)) - 400.0f);
            var_f30 *= 10.0f;
            if (var_f30 < 0.0f) {
                var_f30 = 0.0f;
            }

            if (var_f30 > 6000.0f) {
                var_f30 = 6000.0f;
            }

            s16 spC = var_f30 * cM_ssin(i_this->mCounter * 1000);
            cLib_addCalcAngleS2(&i_this->field_0x6c8, spC + cM_atan2s(spD4.x, spD4.z), 8, AREG_S(7) + 0x400);
        }

        i_this->field_0x6f4 = 1;

        cMtx_YrotS(*calc_mtx, i_this->field_0x6c8);
        spD4.x = 0.0f;
        spD4.y = 0.0f;
        spD4.z = i_this->field_0x6c4;
        MtxPosition(&spD4, &spB0);
        i_this->field_0x6cc += spB0;

        spD4 = i_this->field_0x6cc - spC8;
        spD4.y += 80.0f;

        s16 spA = -cM_atan2s(spD4.y, spD4.z);
        s16 sp8 = (s16)cM_atan2s(spD4.x, JMAFastSqrt((spD4.y * spD4.y) + (spD4.z * spD4.z)));

        mDoMtx_stack_c::transS(spC8.x, spC8.y, spC8.z);
        mDoMtx_stack_c::XrotM(spA);
        mDoMtx_stack_c::YrotM(sp8);

        f32 sp24 = XREG_F(12) + 5.0f;
        mDoMtx_stack_c::scaleM(XREG_F(11) + 1.0f, XREG_F(11) + 1.0f, sp24 * i_this->field_0x6c0);

        i_this->mpBeamModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoMtx_stack_c::transM(0.0f, 0.0f, i_this->field_0x6c0 * (XREG_F(6) + 2400.0f));
        mDoMtx_stack_c::multVecZero(&spD4);
    
        line_chk.Set(&spC8, &spD4, i_this);

        if (dComIfG_Bgsp().LineCross(&line_chk)) {
            spD4 = line_chk.GetCross();
            i_this->field_0x6d8 = spD4;

            i_this->mBeamSound.startLevelSound(Z2SE_EN_GO_BEAM, 0, -1);

            cM3dGPla spE0;
            csXyz sp48;
            dComIfG_Bgsp().GetTriPla(line_chk, &spE0);

            sp48.y = (s16)cM_atan2s(spE0.mNormal.x, spE0.mNormal.z);
            sp48.x = -cM_atan2s(spE0.mNormal.y, JMAFastSqrt((spE0.mNormal.x * spE0.mNormal.x) + (spE0.mNormal.z * spE0.mNormal.z)));
            sp48.x += 0x4000;
            sp48.z = 0;

            for (int i = 0; i < 4; i++) {
                static u16 name[] = {0x8AF2, 0x8AF3, 0x8AF4, 0x8AF5};
                i_this->mBeamEmitterIDs[i] = dComIfGp_particle_set(i_this->mBeamEmitterIDs[i], name[i], &spD4, &sp48, NULL);
            }

            f32 sp1C = 0.013f * (i_this->mCounter & 3);
            spD4 = spD4 + ((spC8 - spD4) * (sp1C));

            i_this->mBeamSph.SetC(spD4);
            dComIfG_Ccsp()->Set(&i_this->mBeamSph);
        }

        i_this->mpBeamBtk->play();
        i_this->mpBeamModelMorf->play(NULL, 0, 0);
        i_this->mpBeamModelMorf->modelCalc();
    }

    if (i_this->field_0x1cfc == 7 || i_this->field_0x6c0 > 0.1f) {
        i_this->field_0x1cfc = 7;
        cLib_addCalc2(&i_this->mKankyoBlend, 1.0f - i_this->field_0x6c0, 1.0f, 0.1f);
    }

    demo_camera(i_this);

    if (i_this->field_0x1ad6 != 0) {
        cLib_addCalcAngleS2(&i_this->field_0x1ad8, 6000, 4, 800);
    } else {
        cLib_addCalcAngleS2(&i_this->field_0x1ad8, BREG_S(7) + -3900, 1, 800);
    }

    f32 target_blend = 1.0f;
    f32 blend_step = 0.1f;
    if (i_this->field_0x1cfc == 0) {
        dKy_custom_colset(0, 0, i_this->mKankyoBlend);
    } else if (i_this->field_0x1cfc == 1) {
        dKy_custom_colset(0, 5, i_this->mKankyoBlend);
    } else if (i_this->field_0x1cfc == 2) {
        dKy_custom_colset(1, 5, i_this->mKankyoBlend);
        target_blend = 0.0f;
        blend_step = 0.1f;
    } else if (i_this->field_0x1cfc == 3) {
        dKy_custom_colset(1, 5, i_this->mKankyoBlend);
        i_this->field_0x1cfa = 5;
        target_blend = -1.0f;
    } else if (i_this->field_0x1cfc == 4) {
        dKy_custom_colset(1, 4, i_this->mKankyoBlend);
        i_this->field_0x1cfa = 4;
        target_blend = -1.0f;
    } else if (i_this->field_0x1cfc == 5) {
        dKy_custom_colset(1, 2, i_this->mKankyoBlend);
        i_this->field_0x1cfa = 2;
        target_blend = -1.0f;
    } else if (i_this->field_0x1cfc == 6) {
        dKy_custom_colset(1, 3, i_this->mKankyoBlend);
        i_this->field_0x1cfa = 3;
        target_blend = -1.0f;
    } else if (i_this->field_0x1cfc == 7) {
        dKy_custom_colset(6, i_this->field_0x1cfa, i_this->mKankyoBlend);
        target_blend = -1.0f;
    }

    if (target_blend >= 0.0f) {
        cLib_addCalc2(&i_this->mKankyoBlend, target_blend, 1.0f, blend_step);
    }

    return 1;
}

static int daB_GM_IsDelete(b_gm_class* i_this) {
    return 1;
}

static int daB_GM_Delete(b_gm_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    dComIfG_resDelete(&i_this->mPhase, "B_gm");

    if (i_this->mInitHIO) {
        l_initHIO = false;
    }

    if (a_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
        i_this->mBeamSound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    b_gm_class* a_this = (b_gm_class*)i_this;

    a_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("B_gm", 0x25), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("B_gm", 30), 2, 0.6f, 0, -1, &a_this->mCreatureSound, 0, 0x11000284);
    if (a_this->mpModelMorf == NULL || a_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = a_this->mpModelMorf->getModel();
    model->setUserArea((uintptr_t)a_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    a_this->mpZoomBtk = new mDoExt_btkAnm();
    if (a_this->mpZoomBtk == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* temp_r28_2 = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("B_gm", 0x28);
    if (!a_this->mpZoomBtk->init(a_this->mpModelMorf->getModel()->getModelData(), temp_r28_2, 1, 0, 1.0f, 0, -1)) {
        return 0;
    }

    a_this->mpBeamModelMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("B_gm", 0x21), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("B_gm", 7), 2, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11000284);
    if (a_this->mpBeamModelMorf == NULL || a_this->mpBeamModelMorf->getModel() == NULL) {
        return 0;
    }

    a_this->mpBeamBtk = new mDoExt_btkAnm();
    if (a_this->mpBeamBtk == NULL) {
        return 0;
    }

    if (!a_this->mpBeamBtk->init(a_this->mpBeamModelMorf->getModel()->getModelData(), (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("B_gm", 0x29), 1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    J3DModelData* modelData;
    for (int i = 0; i < 4; i++) {
        modelData = (J3DModelData*)dComIfG_getObjectRes("B_gm", 0x22);
        JUT_ASSERT(0xCE9, modelData != NULL);
        a_this->mpSpotLightModel[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
        if (a_this->mpSpotLightModel[i] == NULL) {
            return 0;
        }

        a_this->mSpotLightSize[i] = 1.0f;
    }

    a_this->mpSpotLightBtk = new mDoExt_btkAnm();
    if (a_this->mpSpotLightBtk == NULL) {
        return 0;
    }

    if (!a_this->mpSpotLightBtk->init(modelData, (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("B_gm", 0x2A), 1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    return 1;
}

static int daB_GM_Create(fopAc_ac_c* i_this) {
    b_gm_class* a_this = (b_gm_class*)i_this;
    fopAcM_ct(a_this, b_gm_class);

    int phase_state = dComIfG_resLoad(&a_this->mPhase, "B_gm");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("B_gm PARAM %x\n", fopAcM_GetParam(i_this));
        OS_REPORT("B_gm//////////////B_GM SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x8C00)) {
            OS_REPORT("//////////////B_GM SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////B_GM SET 2 !!\n");
        if (!l_initHIO) {
            a_this->mInitHIO = true;
            l_initHIO = true;
            l_HIO.field_0x4 = -1;
        }

        if (dComIfGs_isStageBossEnemy()) {
            a_this->mIsDisappear = true;
            cXyz pos(0.0f, 0.0f, 0.0f);
            csXyz angle(0, 0, 0);
            fopAcM_createWarpHole(&pos, &angle, fopAcM_GetRoomNo(i_this), 0, 0, 0xFF);

            if (!dComIfGs_isStageLife()) {
                cXyz size(1.0f, 1.0f, 1.0f);
                pos.set(400.0f, 0.0f, 0.0f);
                fopAcM_createItemForBoss(&pos, fpcNm_ITEM_UTAWA_HEART, fopAcM_GetRoomNo(i_this), &i_this->shape_angle, &size, 0.0f, 0.0f, -1);
            }

            return phase_state;
        }

        fopAcM_SetMtx(i_this, a_this->mpModelMorf->getModel()->getBaseTRMtx());

        a_this->mCcStts.Init(0xFF, 0, i_this);

        static dCcD_SrcSph body_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0xd}, {0xd8fbfdff, 0x43}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_METAL, 0x2, 0x0, 0x0, 0x3}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };
    
        static dCcD_SrcSph core_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0xd}, {0x2000, 0x43}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };
    
        static dCcD_SrcSph hand_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xd}, {0xd8fbfdff, 0x43}, 0x0}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_METAL, 0x2, 0x0, 0x0, 0x3}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 30.0f} // mSph
            } // mSphAttr
        };
    
        static dCcD_SrcSph foot_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x1f}, {0xd8fbfdff, 0x43}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_METAL, 0x2, 0x0, 0x0, 0x3}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };
    
        static dCcD_SrcSph beam_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x4, 0x1f}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x1, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 20.0f} // mSph
            } // mSphAttr
        };

        for (int i = 0; i < 8; i++) {
            if (i < 2) {
                a_this->mBodySph[i].Set(body_sph_src);
                a_this->mBodySph[i].SetStts(&a_this->mCcStts);

                a_this->mHandSph[i].Set(hand_sph_src);
                a_this->mHandSph[i].SetStts(&a_this->mCcStts);
            }

            a_this->mFootSph[i].Set(foot_sph_src);
            a_this->mFootSph[i].SetStts(&a_this->mCcStts);
        }

        a_this->mCoreSph.Set(core_sph_src);
        a_this->mCoreSph.SetStts(&a_this->mCcStts);

        a_this->mBeamSph.Set(beam_sph_src);
        a_this->mBeamSph.SetStts(&a_this->mCcStts);
        a_this->mBeamSph.SetAtMtrl(dCcD_MTRL_FIRE);

        a_this->mCreatureSound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        a_this->mAtInfo.mpSound = &a_this->mCreatureSound;
        a_this->mBeamSound.init(&a_this->field_0x6d8, 1);

        i_this->health = 500;
        i_this->field_0x560 = 500;

        a_this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->mAcchCir.SetWall(200.0f, 100.0f);

        a_this->mAction = ACTION_MOVE;
        a_this->mMode = 10;
        a_this->mDemoMode = 1;

        a_this->field_0x1ae0 = 31.0f;
        a_this->field_0x5d8 = -32768.0f;
        a_this->field_0x5d4 = -32768.0f;;

        i_this->shape_angle.x = 0x8000;
        i_this->current.angle.x = 0x8000;
        a_this->field_0x73c.y = 2520.0f;
        i_this->current.pos.y = 2520.0f;
        a_this->mTargetMovePos = i_this->current.pos;

        daB_GM_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daB_GM_Method = {
    (process_method_func)daB_GM_Create,
    (process_method_func)daB_GM_Delete,
    (process_method_func)daB_GM_Execute,
    (process_method_func)daB_GM_IsDelete,
    (process_method_func)daB_GM_Draw,
};

extern actor_process_profile_definition g_profile_B_GM = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_B_GM,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(b_gm_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  213,                    // mPriority
  &l_daB_GM_Method,       // sub_method
  0x00044000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
