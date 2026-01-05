/**
 * @file d_a_e_th.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_th.h"
#include "d/actor/d_a_e_th_ball.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "c/c_damagereaction.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"

class daE_TH_HIO_c : public JORReflexible {
public:
    daE_TH_HIO_c();
    virtual ~daE_TH_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 no;
    /* 0x08 */ f32 base_size;
    /* 0x0C */ u8 unk_0xC[0x10 - 0xC];
    /* 0x10 */ f32 move_range;
    /* 0x14 */ f32 middle_move_range;
};

enum daE_TH_ACTION {
    ACTION_WAIT,
    ACTION_SPIN,
    ACTION_SPIN_B,
    ACTION_SHOT,
    ACTION_RETURN,
    ACTION_DAMAGE = 20,
    ACTION_START = 22,
    ACTION_END = 25,
};

daE_TH_HIO_c::daE_TH_HIO_c() {
    no = -1;
    base_size = 1.0f;
    move_range = 1300.0f;
    middle_move_range = 700.0f;
}

static void* s_md_sub1(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_MD && (fopAcM_GetParam(i_actor) & 0xFF00) == 0xFF00 && ((fopAc_ac_c*)i_actor)->current.pos.z == 560.0f) {
        fopAcM_delete((fopAc_ac_c*)i_actor);
    }

    return NULL;
}

static void* s_md_sub2(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_MD && (fopAcM_GetParam(i_actor) & 0xFF00) != 0xFF00 && ((fopAc_ac_c*)i_actor)->current.pos.z == -950.0f) {
        fopAcM_delete((fopAc_ac_c*)i_actor);
    }

    return NULL;
}

static void anm_init(e_th_class* i_this, int i_anm, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_th", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
    i_this->mAnm = i_anm;
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model_p = j3dSys.getModel();
        e_th_class* i_this = (e_th_class*)model_p->getUserArea();
        
        if (i_this != NULL && (jnt_no == 3 || jnt_no == 4)) {
            MTXCopy(model_p->getAnmMtx(jnt_no), *calc_mtx);
            cMtx_YrotM(*calc_mtx, i_this->mHeadRotY);
            cMtx_ZrotM(*calc_mtx, i_this->mHeadRotZ);
            model_p->setAnmMtx(jnt_no, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static void ke_control(e_th_class* i_this, th_ke_s* i_ke_s, int i_no) {
    cXyz sp4C;
    cXyz sp40;

    int i;
    cXyz* pos_p = &i_ke_s->pos[1];
    cXyz* var_r29 = &i_ke_s->field_0xb4[1];
    cXyz sp34;
    cXyz sp28;
    cXyz sp1C;

    cMtx_YrotS(*calc_mtx, i_ke_s->field_0x16a);
    cMtx_XrotM(*calc_mtx, i_ke_s->field_0x168);
    sp4C.x = 0.0f;
    sp4C.y = 3.0f + (10.0f + TREG_F(3));
    sp4C.z = -10.0f + TREG_F(4);
    MtxPosition(&sp4C, &sp34);

    for (i = 1; i < 15; i++, pos_p++, var_r29++) {
        MtxPush();
        sp28.x = 0.1f * (sp34.x * (15 - i));
        sp28.y = 0.1f * (sp34.y * (15 - i));
        sp28.z = 0.1f * (sp34.z * (15 - i));
        sp4C.x = cM_rndFX2(2.0f + KREG_F(7));
        sp4C.y = cM_rndFX2(2.0f + KREG_F(7));
        sp4C.z = 0.0f;
        MtxPosition(&sp4C, &sp1C);

        f32 temp_f28 = sp1C.x + (sp28.x + (var_r29[0].x + (pos_p[0].x - pos_p[-1].x)));
        f32 temp_f30 = sp1C.z + (sp28.z + (var_r29[0].z + (pos_p[0].z - pos_p[-1].z)));
        f32 temp_f31 = (TREG_F(2) + ((sp1C.y + (sp28.y + (pos_p[0].y + var_r29[0].y))) - 10.0f)) - pos_p[-1].y;
        s16 sp12 = -cM_atan2s(temp_f31, temp_f30);
        s16 sp10 = (s16)cM_atan2s(temp_f28, JMAFastSqrt((temp_f31 * temp_f31) + (temp_f30 * temp_f30)));

        cMtx_XrotS(*calc_mtx, sp12);
        cMtx_YrotM(*calc_mtx, sp10);
        sp4C.x = 0.0f;
        sp4C.y = 0.0f;
        sp4C.z = 15.0f + TREG_F(5);
        MtxPosition(&sp4C, &sp40);

        *var_r29 = *pos_p;
        pos_p[0].x = pos_p[-1].x + sp40.x;
        pos_p[0].y = pos_p[-1].y + sp40.y;
        pos_p[0].z = pos_p[-1].z + sp40.z;
        
        f32 temp_f29 = 0.75f + (0.001f * i_no);
        var_r29[0].x = temp_f29 * (pos_p[0].x - var_r29[0].x);
        var_r29[0].y = temp_f29 * (pos_p[0].y - var_r29[0].y);
        var_r29[0].z = temp_f29 * (pos_p[0].z - var_r29[0].z);
        MtxPull();
    }
}

static void ke_pos_set(e_th_class* i_this, th_ke_s* i_ke_s, int param_2) {
    cXyz* pos_p = i_this->mKeLineMat.getPos(param_2);
    for (int i = 0; i < 15; i++, pos_p++) {
        *pos_p = i_ke_s->pos[i];
    }
}

static void ke_disp(e_th_class* i_this) {
    static GXColor l_color = {0x50, 0x0A, 0x0A, 0xFF};
    i_this->mKeLineMat.update(15, 3.5f + TREG_F(11), l_color, 1, &i_this->tevStr);
    dComIfGd_set3DlineMat(&i_this->mKeLineMat);
}

static void ke_move(e_th_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp28;
    cXyz sp1C;

    cM_initRnd2(12, 123, 70);

    MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(4), *calc_mtx);
    sp28.x = 0.0f;
    sp28.y = 27.0f + TREG_F(15);
    sp28.z = TREG_F(16);
    MtxPosition(&sp28, &sp1C);

    sp28 = i_this->mKe_s[0].pos[0] - sp1C;
    s16 temp_r25 = cM_atan2s(sp28.x, sp28.z);
    sp28.x = 35.0f + TREG_F(14);
    sp28.y = 27.0f + TREG_F(15);
    sp28.z = TREG_F(16);

    for (int i = 0; i < 3; i++) {
        MtxPosition(&sp28, &i_this->mKe_s[i].pos[0]);
        i_this->mKe_s[i].field_0x168 = -i_this->mHeadRotZ;
        i_this->mKe_s[i].field_0x16a = temp_r25 + ((i - 1) << 11);

        if (i == 1) {
            i_this->mKe_s[i].field_0x168 += 0x500;
        }
    }

    th_ke_s* ke_s = i_this->mKe_s;
    for (int i = 0; i < 3; i++, ke_s++) {
        ke_control(i_this, ke_s, i);
        ke_pos_set(i_this, ke_s, i);
    }
}

static int daE_TH_Draw(e_th_class* i_this) {
    J3DModel* model_p = i_this->mpModelMorf->getModel();
    if (i_this->mNoDraw) {
        return 1;
    }

    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model_p, &i_this->tevStr);

    if (i_this->mDoEndColorEff) {
        J3DModelData* modelData_p = model_p->getModelData();
        for (u16 i = 0; i < modelData_p->getMaterialNum(); i++) {
            J3DMaterial* material_p = modelData_p->getMaterialNodePointer(i);
            material_p->getTevColor(0)->r = i_this->mEndBodyColor;
            material_p->getTevColor(0)->g = i_this->mEndBodyColor;
            material_p->getTevColor(0)->b = i_this->mEndBodyColor;
        }
    }

    i_this->mpModelMorf->entryDL();

    cXyz sp8;
    sp8.set(i_this->current.pos.x, 50.0f + i_this->current.pos.y + BREG_F(18), i_this->current.pos.z);
    i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model_p, &sp8, 1200.0f + BREG_F(19), 0.0f, i_this->current.pos.y, i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd, &i_this->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    ke_disp(i_this);
    return 1;
}

static void e_th_wait(e_th_class* i_this) {
    i_this->field_0x6a4 = 5;

    switch (i_this->mMode) {
    case 0:
        if (i_this->mTimers[0] == 0) {
            anm_init(i_this, 0x1B, 10.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->mTimers[1] = 10;
            i_this->mTimers[0] = 60.0f + cM_rndF(60.0f);
        }
        break;
    case 1:
        if (i_this->mTimers[0] == 0 && i_this->mDemoCamMode == 0) {
            i_this->mMode = 2;
            anm_init(i_this, 0x12, 5.0f, 0, 1.0f);
        }
        break;
    case 2:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, 0x1B, 10.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->mTimers[0] = 60.0f + cM_rndF(60.0f);
        }
        break;
    }

    if (i_this->mTimers[1] == 0) {
        i_this->mAction = ACTION_SPIN;
        i_this->mMode = 0;
    }
}

static void e_th_spin(e_th_class* i_this) {
    cXyz sp14;
    cXyz sp8;

    i_this->field_0x6a4 = 5;
    i_this->field_0x6e4 = 1;
    i_this->mAction = ACTION_SPIN_B;
    i_this->mMode = 0;
}

static void e_th_spin_B(e_th_class* i_this) {
    cXyz sp18;
    cXyz spC;

    i_this->field_0x6a4 = 5;
    i_this->field_0x6e4 = 1;

    s8 var_r26 = 0;
    s16 var_r27;
    if (i_this->mPlayerAngleY > 0x4000 || i_this->mPlayerAngleY < -0x4000) {
        var_r27 = -0x8000;
    } else {
        var_r27 = 0;
    }

    cMtx_YrotS(*calc_mtx, var_r27);
    sp18.x = 0.0f;
    sp18.y = 0.0f;
    sp18.z = i_this->speedF * i_this->mSpinAnmSpeed;
    MtxPosition(&sp18, &spC);
    i_this->current.pos += spC;

    f32 speed_target;
    f32 anm_frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, 5, 10.0f, 0, 1.0f);
        i_this->mMode = 1;
        i_this->field_0x68a |= 1;
        i_this->mSpinAnmSpeed = 0.5f;
        break;
    case 1:
        if (i_this->mpModelMorf->isStop()) {
            i_this->field_0x68a |= 1;
            if (i_this->mDemoCamMode != 0 && i_this->mDemoCamMode < 4) {
                anm_init(i_this, 4, 3.0f, 2, i_this->mSpinAnmSpeed);
                i_this->mMode = 10;
            } else {
                anm_init(i_this, 6, 3.0f, 2, i_this->mSpinAnmSpeed);
                i_this->mMode = 3;
            }

            i_this->mpModelMorf->setFrame(anm_frame);
            i_this->field_0x5dc = 0;
        }
        break;
    case 3:
        var_r26 = 1;
        speed_target = 2.0f;
        var_r27 = i_this->mPlayerAngleY - i_this->shape_angle.y;

        i_this->field_0x5dc++;
        if (i_this->field_0x5dc > 80 && (var_r27 > 0x2000 || var_r27 < -0x2000) && i_this->mPlayerDist > 200.0f + BREG_F(10)) {
            i_this->mTimers[0] = 40;
            i_this->mMode = 4;
        }
        break;
    case 4:
        var_r26 = 1;
        if (i_this->mTimers[0] == 0 && i_this->mpModelMorf->checkFrame(4.0f + NREG_F(18))) {
            i_this->mAction = ACTION_SHOT;
            i_this->mMode = 0;
            i_this->field_0x68a |= 1;
        }
        break;
    case 10:
        var_r26 = 1;
        if (i_this->mDemoCamMode == 3 && i_this->mDemoCamTimer >= VREG_S(6) + 80 && i_this->mpModelMorf->checkFrame(4.0f + NREG_F(18))) {
            i_this->mAction = ACTION_SHOT;
            i_this->mMode = 0;
            i_this->field_0x68a |= 1;
        }
        break;
    }

    if (var_r26 != 0) {
        f32 anm_speed_step;
        if (i_this->field_0x5b7 == 1) {
            anm_speed_step = 0.025f;
        } else {
            anm_speed_step = 0.01f;
        }

        cLib_addCalc2(&i_this->mSpinAnmSpeed, 1.5f, 1.0f, anm_speed_step);
        i_this->mpModelMorf->setPlaySpeed(i_this->mSpinAnmSpeed);

        if (i_this->mpModelMorf->checkFrame(10.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_TH_BRANDISH, 0, -1);
        }
    }

    cLib_addCalc2(&i_this->speedF, speed_target, 1.0f, 0.1f);
}

static s16 e_th_shot(e_th_class* i_this) {
    s16 angle_chase_step = 0;
    int anm_frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, 0x18, 4.0f + NREG_F(19), 0, 1.0f);
        i_this->mMode = 1;
        /* fallthrough */
    case 1:
        i_this->field_0x6a4 = 5;
        if (anm_frame < 20) {
            angle_chase_step = 0x500;
        }

        if (NREG_S(1) + 25 == (int)i_this->mpModelMorf->getFrame()) {
            i_this->field_0x68a |= 2;
        }

        if (i_this->mpModelMorf->isStop()) {
            if (i_this->mDemoCamMode == 0) {
                i_this->mTimers[0] = JREG_S(4) + 100;
            } else {
                i_this->mTimers[0] = JREG_S(4) + 90;
            }

            i_this->mMode = 2;
            anm_init(i_this, 0x19, 3.0f, 2, 1.0f);
        }
        break;
    case 2:
        i_this->field_0x6e4 = 1;
        if (i_this->mTimers[0] == 0) {
            i_this->mAction = ACTION_RETURN;
            i_this->mMode = 0;
        }
        break;
    }

    return angle_chase_step;
}

static void e_th_return(e_th_class* i_this) {
    e_th_ball_class* ball_p = (e_th_ball_class*)fopAcM_SearchByID(i_this->mBallID);
    cXyz ball_vec;

    i_this->field_0x6e4 = 2;
    ball_vec = ball_p->current.pos - i_this->current.pos;

    switch (i_this->mMode) {
    case 0:
        if (JMAFastSqrt((ball_vec.x * ball_vec.x) + (ball_vec.z * ball_vec.z)) < 300.0f + AREG_F(17)) {
            i_this->field_0x68a |= 1;
            i_this->mAction = ACTION_WAIT;
            i_this->mMode = 0;
        } else {
            anm_init(i_this, 0x17, 5.0f, 0, 1.0f);
            i_this->mMode = 1;
        }
        break;
    case 1:
        if (NREG_S(2) + 9 == (int)i_this->mpModelMorf->getFrame()) {
            i_this->field_0x68a |= 1;
        }

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, 0x1B, 0.0f, 2, 1.0f);
            i_this->mMode = 2;
        }

        i_this->mTimers[0] = 100;
        /* fallthrough */
    case 2:
        if (JMAFastSqrt((ball_vec.x * ball_vec.x) + (ball_vec.z * ball_vec.z)) < 300.0f + AREG_F(17)) {
            anm_init(i_this, 0x1A, 3.0f, 0, 1.0f);
            i_this->mMode = 3;
            i_this->mTimers[0] = 0;
        } else if (i_this->mTimers[0] == 0) {
            i_this->mAction = ACTION_WAIT;
            i_this->mMode = 0;
        }
        break;
    case 3:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = ACTION_WAIT;
            i_this->mMode = 0;
        }
        break;
    }
}

static void e_th_damage(e_th_class* i_this) {
    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        if (cM_rndF(1.0f) < 0.5f) {
            anm_init(i_this, 7, 3.0f, 0, 1.0f);
        } else {
            anm_init(i_this, 8, 3.0f, 0, 1.0f);
        }
        break;
    case 1:
        if (i_this->mTimers[0] == 0 || i_this->mpModelMorf->isStop()) {
            i_this->field_0x6a4 = 5;
            i_this->mAction = ACTION_SHOT;
            i_this->mMode = 2;
        }
        break;
    }
}

static void e_th_start(e_th_class* i_this) {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    i_this->field_0x6a4 = 5;
    e_th_ball_class* ball_p = (e_th_ball_class*)fopAcM_SearchByID(i_this->mBallID);

    switch (i_this->mMode) {
    case 0:
        if (ball_p != NULL) {
            i_this->mMode = 1;
            ball_p->mPlayerGet = 1;
        }
        break;
    case 1:
        if (!dComIfGp_checkPlayerStatus1(0, 0x2000000) && !dComIfGp_checkPlayerStatus1(0, 0x10000) && !daPy_getPlayerActorClass()->checkHookshotShootReturnMode() && player->current.pos.z < -1200.0f && !player->checkNowWolf()) {
            fpcM_Search(s_md_sub1, i_this);
            Z2GetAudioMgr()->subBgmStart(Z2BGM_IB_MBOSS_D01);
            i_this->mDemoCamMode = 1;
            i_this->mNoDraw = FALSE;
            ball_p->mPlayerGet = 0;
            i_this->current.pos.y = i_this->home.pos.y;
            i_this->mAction = ACTION_SPIN;
            i_this->mMode = 0;
            dComIfGs_onOneZoneSwitch(1, -1);
        }
        break;
    }
}

static void e_th_end(e_th_class* i_this) {
    i_this->field_0x6a4 = 5;
    dComIfGp_getPlayer(0);

    switch (i_this->mMode) {
    case 0:
    case 2:
        break;
    case 1:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mMode = 2;
            dComIfGs_onStageMiddleBoss();
        }
        break;
    case 3:
        i_this->current.pos.y = 10000.0f + i_this->home.pos.y;
        break;
    }

    if (i_this->mDoEndColorEff) {
        cLib_addCalc2(&i_this->mEndBodyColor, -30.0f, 1.0f, 0.4f);
    }
}

static void damage_check(e_th_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    i_this->mCcStts.Move();

    for (int i = 0; i <= 2; i++) {
        if (i_this->mCcSph[i].ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mCcSph[i].GetTgHitObj();

            if (i_this->mCcSph[i].ChkTgShield()) {
                if (i_this->field_0x6a6 == 0) {
                    i_this->field_0x6a6 = 6;
                    def_se_set(&i_this->mSound, i_this->mCcSph[i].GetTgHitObj(), 0x28, NULL);
                    return;
                }
                break;
            }

            if (i_this->field_0x6a4 == 0 && i_this->mAction != ACTION_SPIN) {
                daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
                OS_REPORT("E_th HP1 %d\n", i_this->health);
                cc_at_check(i_this, &i_this->mAtInfo);
                OS_REPORT("E_th HP2 %d\n", i_this->health);

                if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP && player->checkCutJumpCancelTurn()) {
                    i_this->field_0x6a4 = 3;
                } else {
                    i_this->field_0x6a4 = 10;
                }

                dComIfGs_onOneZoneSwitch(3, -1);

                if (i_this->health <= 0) {
                    i_this->mAction = ACTION_END;
                    i_this->mMode = 0;
                    i_this->field_0x68a |= 4;
                    i_this->mDemoCamMode = 10;
                    Z2GetAudioMgr()->subBgmStop();
                } else if (i_this->mAction == ACTION_SHOT || i_this->mAction == ACTION_DAMAGE) {
                    i_this->mAction = ACTION_DAMAGE;
                    i_this->mMode = 0;
                }
            }
            break;
        }
    }

    for (int i = 0; i <= 2; i++) {
        if (i_this->health <= 1) {
            i_this->mCcSph[i].SetTgHitMark((CcG_Tg_HitMark)3);
        }
    }
}

static void action(e_th_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp50;
    cXyz sp44;

    i_this->field_0x6e4 = 0;
    i_this->mPlayerDist = fopAcM_searchPlayerDistance(a_this);
    i_this->mPlayerAngleY = fopAcM_searchPlayerAngleY(a_this);

    damage_check(i_this);

    s16 angle_chase_step = 0;
    s8 on_attention = TRUE;

    switch (i_this->mAction) {
    case ACTION_WAIT:
        e_th_wait(i_this);
        break;
    case ACTION_SPIN:
        e_th_spin(i_this);
        angle_chase_step = KREG_S(5) + 0x400;
        break;
    case ACTION_SPIN_B:
        e_th_spin_B(i_this);
        angle_chase_step = KREG_S(5) + 0x300;
        break;
    case ACTION_SHOT:
        angle_chase_step = e_th_shot(i_this);
        break;
    case ACTION_RETURN:
        e_th_return(i_this);
        break;
    case ACTION_DAMAGE:
        e_th_damage(i_this);
        break;
    case ACTION_START:
        e_th_start(i_this);
        on_attention = FALSE;
        break;
    case ACTION_END:
        e_th_end(i_this);
        on_attention = FALSE;
        break;
    }

    if (angle_chase_step != 0) {
        cLib_addCalcAngleS2(&a_this->shape_angle.y, i_this->mPlayerAngleY, 4, angle_chase_step);
    }

    if (on_attention) {
        fopAcM_OnStatus(a_this, 0);
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    }

    if (i_this->field_0x6e4 != 0 && i_this->mDemoCamMode == 0) {
        cXyz sp38;
        s16 var_r27 = 0;
        s16 var_r28;

        if (i_this->field_0x6e4 == 3) {
            var_r28 = 10000;
        } else {
            if (i_this->field_0x6e4 == 1) {
                sp38 = player->eyePos - a_this->current.pos;
                sp38.y += -150.0f + KREG_F(17);
            } else if (i_this->field_0x6e4 == 2) {
                fopAc_ac_c* ball_p = fopAcM_SearchByID(i_this->mBallID);
                sp38 = ball_p->current.pos - a_this->current.pos;
                sp38.y += -50.0f + KREG_F(16);
            }

            var_r27 = cM_atan2s(sp38.x, sp38.z) - a_this->shape_angle.y;
            var_r28 = cM_atan2s(sp38.y, JMAFastSqrt((sp38.x * sp38.x) + (sp38.z * sp38.z))) - a_this->shape_angle.x;
            if (var_r27 > (TREG_S(8) + 8000)) {
                var_r27 = TREG_S(8) + 8000;
            } else if (var_r27 < -(TREG_S(8) + 8000)) {
                var_r27 = -(TREG_S(8) + 8000);
            }
        }
        cLib_addCalcAngleS2(&i_this->mHeadRotY, var_r27 / 2, 4, 0x800);
    
        if (var_r28 > (TREG_S(8) + 8000)) {
            var_r28 = TREG_S(8) + 8000;
        } else if (var_r28 < -(TREG_S(8) + 8000)) {
            var_r28 = -(TREG_S(8) + 8000);
        }
        cLib_addCalcAngleS2(&i_this->mHeadRotZ, var_r28 / 2, 4, 0x800);
    }

    cXyz bind_eff_size(1.8f, 1.8f, 1.8f);
    setMidnaBindEffect(a_this, &i_this->mSound, &a_this->eyePos, &bind_eff_size);

    if (dComIfGs_isOneZoneSwitch(1, -1) && player->current.pos.y > 500.0f) {
        dComIfGs_onOneZoneSwitch(2, -1);
    }
}

static void cam_3d_morf(e_th_class* i_this, f32 param_1) {
    cLib_addCalc2(&i_this->mDemoCamCenter.x, i_this->mDemoCamCenterTarget.x, param_1, i_this->mDemoCamCenterSpd.x * i_this->field_0xd88);
    cLib_addCalc2(&i_this->mDemoCamCenter.y, i_this->mDemoCamCenterTarget.y, param_1, i_this->mDemoCamCenterSpd.y * i_this->field_0xd88);
    cLib_addCalc2(&i_this->mDemoCamCenter.z, i_this->mDemoCamCenterTarget.z, param_1, i_this->mDemoCamCenterSpd.z * i_this->field_0xd88);

    cLib_addCalc2(&i_this->mDemoCamEye.x, i_this->mDemoCamEyeTarget.x, param_1, i_this->mDemoCamEyeSpd.x * i_this->field_0xd88);
    cLib_addCalc2(&i_this->mDemoCamEye.y, i_this->mDemoCamEyeTarget.y, param_1, i_this->mDemoCamEyeSpd.y * i_this->field_0xd88);
    cLib_addCalc2(&i_this->mDemoCamEye.z, i_this->mDemoCamEyeTarget.z, param_1, i_this->mDemoCamEyeSpd.z * i_this->field_0xd88);
}

static void demo_camera(e_th_class* i_this) {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = dComIfGp_getCamera(0);
    cXyz sp8C;
    cXyz sp80;
    cXyz sp74;
    cXyz sp68;

    s8 sp8 = 0;

    switch (i_this->mDemoCamMode) {
    case 1:
        if (!i_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(i_this, 2, 0xFFFF, 0);
            i_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoCamMode = 2;
        i_this->mDemoCamTimer = 0;
        i_this->mDemoCamFovy = 55.0f;
        camera->mCamera.SetTrimSize(3);

        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
        dComIfGp_getEvent()->startCheckSkipEdge(i_this);
    case 2:
        sp80.set(1.9f, 0.0f, -1127.0f);
        player->setPlayerPosAndAngle(&sp80, -0x8000, 0);
        player->speedF = 0.0f;

        if (i_this->mDemoCamTimer >= 1) {
            i_this->mDemoCamCenter.set(35.0f, 140.0f, -1262.0f);
            i_this->mDemoCamEye.set(130.0f, 98.0f, -963.0f);

            if (i_this->mDemoCamTimer == 1) {
                int bitSw = (fopAcM_GetParam(i_this) & 0xFF000000) >> 0x18;
                dComIfGs_offSwitch(bitSw, fopAcM_GetRoomNo(i_this));
            }

            if (i_this->mDemoCamTimer == 90) {
                i_this->mDemoCamMode = 3;
                i_this->mDemoCamTimer = 0;
                i_this->mDemoCamFovy = 30.0f;
                i_this->mDemoCamCenter.set(0.0f, 180.0f, -1043.0f);
                i_this->mDemoCamEye.set(0.0f, 84.0f, -1344.0f);
                daPy_getPlayerActorClass()->changeDemoMode(0x48, 0, 0, 0);
            }
        }
        break;
    case 3:
        if (i_this->mDemoCamTimer > 20) {
            cLib_addCalc2(&i_this->mDemoCamFovy, 55.0f, 0.1f, 0.2f * i_this->field_0xd88);
            cLib_addCalc2(&i_this->field_0xd88, 1.0f, 1.0f, 0.05f);

            if (i_this->mDemoCamTimer == 145) {
                daPy_getPlayerActorClass()->changeDemoMode(0x49, 0, 0, 0);
                i_this->mDemoCamMode = 4;
                i_this->mDemoCamTimer = 0;
                i_this->mDemoCamCenterTarget.set(-172.0f, 99.7f, -1016.0f);
                i_this->mDemoCamEyeTarget.set(-210.0f, 84.0f, -1330.0f);
                i_this->mDemoCamEyeSpd.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
                i_this->mDemoCamEyeSpd.y = fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
                i_this->mDemoCamEyeSpd.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);
                i_this->mDemoCamCenterSpd.x = fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
                i_this->mDemoCamCenterSpd.y = fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
                i_this->mDemoCamCenterSpd.z = fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);
                i_this->field_0xd88 = 0.0f;
            }
        }
        break;
    case 4:
        if (i_this->mAction == ACTION_WAIT) {
            i_this->mTimers[1] = 10;
        }

        cLib_addCalc2(&i_this->mDemoCamFovy, 55.0f, 0.1f, 0.2f);

        if (i_this->mDemoCamTimer >= 20 && i_this->mDemoCamTimer == 20) {
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
            sp80.set(-300.0f, 0.0f, -1104.0f);
            player->setPlayerPosAndAngle(&sp80, 0, 0);
        }

        if (i_this->mDemoCamTimer >= 150) {
            if (i_this->mDemoCamTimer == 150) {
                i_this->mDemoCamCenterTarget.set(-56.0f, 224.7f, 598.0f);
                i_this->mDemoCamEyeTarget.set(-54.5f, 177.0f, 285.0f);
                i_this->mDemoCamEyeSpd.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
                i_this->mDemoCamEyeSpd.y = fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
                i_this->mDemoCamEyeSpd.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);
                i_this->mDemoCamCenterSpd.x = fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
                i_this->mDemoCamCenterSpd.y = fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
                i_this->mDemoCamCenterSpd.z = fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);
                i_this->field_0xd88 = 0.0f;
            }

            cam_3d_morf(i_this, 0.2f);
            cLib_addCalc2(&i_this->field_0xd88, 0.05f, 1.0f, 0.002f);

            if (i_this->mDemoCamTimer == 270) {
                i_this->mAction = ACTION_SPIN;
                i_this->mMode = 0;

                i_this->mDemoCamMode = 5;
                i_this->mDemoCamTimer = 0;
                i_this->mDemoCamCenterTarget.set(-184.0f, 90.7f,-1028.0f);
                i_this->mDemoCamEyeTarget.set(-256.5f, 34.0f, -1331.0f);
                i_this->mDemoCamEyeSpd.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
                i_this->mDemoCamEyeSpd.y = fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
                i_this->mDemoCamEyeSpd.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);
                i_this->mDemoCamCenterSpd.x = fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
                i_this->mDemoCamCenterSpd.y = fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
                i_this->mDemoCamCenterSpd.z = fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);
                i_this->field_0xd88 = 0.0f;

                fpcM_Search(s_md_sub2, i_this);
                dComIfGs_onSwitch(106, fopAcM_GetRoomNo(i_this));
            }
        } else if (i_this->mDemoCamTimer >= 15) {
            i_this->field_0xd88 = 0.05f;
            cam_3d_morf(i_this, 0.2f);
        }
        break;
    case 5:
        if (i_this->mDemoCamTimer > 70) {
            cam_3d_morf(i_this, 0.1f);
            cLib_addCalc2(&i_this->field_0xd88, 0.01f, 1.0f, 0.0003f);

            if (i_this->mDemoCamTimer == 250) {
                sp8 = 1;
                Z2GetAudioMgr()->subBgmStart(Z2BGM_IB_MBOSS);
            }
        }
        break;
    case 10: {
        if (!i_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(i_this, 2, 0xFFFF, 0);
            i_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoCamMode = 11;
        i_this->mDemoCamTimer = 0;
        i_this->mDemoCamFovy = 55.0f;
        i_this->field_0xd78 = 0.0f;
        i_this->field_0xd88 = 0.0f;
        camera->mCamera.SetTrimSize(3);

        if (i_this->shape_angle.y > 0x4000 || i_this->shape_angle.y < -0x4000) {
            i_this->current.angle.y = -0x8000;
        } else {
            i_this->current.angle.y = 0;
        }
        i_this->shape_angle.y = i_this->current.angle.y;

        e_th_ball_class* ball_p = (e_th_ball_class*)fopAcM_SearchByID(i_this->mBallID);
        cMtx_YrotS(*calc_mtx, i_this->shape_angle.y);
        sp8C.x = AREG_F(3);
        sp8C.y = AREG_F(4);
        sp8C.z = 400.0f + AREG_F(5);
        MtxPosition(&sp8C, &ball_p->current.pos);

        ball_p->current.pos += i_this->current.pos;
        ball_p->shape_angle.set(0, 0, 0x1700);
        ball_p->current.angle.y = i_this->shape_angle.y;
        ball_p->current.angle.x = -0x1000;
        ball_p->speedF = 0.0f;
        daPy_getPlayerActorClass()->changeOriginalDemo();

        if (player->checkNowWolf()) {
            daPy_getPlayerActorClass()->changeDemoMode(0x3A, 0, 0, 0);
            i_this->field_0x567 = 1;
        } else {
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
        }

        anm_init(i_this, 0xE, 2.0f, 0, 1.0f);
        i_this->mMode = 1;
        dComIfGs_onSaveSwitch(94);
    }
    case 11:
        if (i_this->mDemoCamTimer == 55 && i_this->field_0x567 != 0) {
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
        }

        cMtx_YrotS(*calc_mtx, i_this->shape_angle.y);
        sp8C.x = 100.0f + AREG_F(7);
        sp8C.y = AREG_F(8);
        sp8C.z = -300.0f + AREG_F(9);
        MtxPosition(&sp8C, &sp80);
        sp80 += i_this->current.pos;
        player->setPlayerPosAndAngle(&sp80, i_this->shape_angle.y, 0);

        sp8C.x = 300.0f + AREG_F(10);
        sp8C.y = 80.0f + AREG_F(11);
        sp8C.z = 400.0f + AREG_F(12);
        MtxPosition(&sp8C, &sp74);
        sp74 += i_this->current.pos;

        sp8C.x = AREG_F(13);
        sp8C.y = AREG_F(14);
        sp8C.z = AREG_F(15);
        MtxPosition(&sp8C, &sp68);
        sp68 += i_this->current.pos;
        sp68.y = 30.0f + i_this->mHeadPos.y + AREG_F(16);
        if (sp68.y > 150.0f + (i_this->current.pos.y + AREG_F(17))) {
            sp68.y = 150.0f + (i_this->current.pos.y + AREG_F(17));
        }

        if (i_this->mDemoCamTimer == 0) {
            i_this->mDemoCamEye = sp74;
            i_this->mDemoCamCenter = sp68;
        } else {
            cLib_addCalc2(&i_this->mDemoCamCenter.x, sp68.x, 0.1f, 20.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, sp68.y, 0.1f, 20.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, sp68.z, 0.1f, 20.0f);
        }

        if (i_this->mDemoCamTimer == 279) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER4, 0x4F, cXyz(0.0f, 1.0f, 0.0f));
            fopAcM_effSmokeSet1(&i_this->field_0xd94, &i_this->field_0xd98, &i_this->mHeadPos, &i_this->shape_angle, 2.5f + TREG_F(18), &i_this->tevStr, 1);
            i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_L, 0, -1);
            i_this->mDoEndColorEff = TRUE;
        }

        if (i_this->mDemoCamTimer == 380) {
            J3DModel* model_p = i_this->mpModelMorf->getModel();
            MTXCopy(model_p->getAnmMtx(1), *calc_mtx);
            sp8C.set(BREG_F(11), BREG_F(12), BREG_F(13));
            MtxPosition(&sp8C, &sp80);
            fopAcM_createDisappear(i_this, &sp80, 20, 0, 0xFF);
        }

        if (i_this->mDemoCamTimer == 382) {
            i_this->mNoDraw = TRUE;
        }

        if (i_this->mDemoCamTimer != 430) {
            break;
        }

        i_this->mDemoCamMode = 12;
        i_this->mDemoCamTimer = 0;
        player->changeDemoMode(0x1E, 0, 0, 0);

        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        sp8C.x = ZREG_F(7);
        sp8C.y = 130.0f + ZREG_F(8);
        sp8C.z = 170.0f + ZREG_F(9);
        MtxPosition(&sp8C, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += player->current.pos;

        sp8C.x = ZREG_F(10);
        sp8C.y = 130.0f + ZREG_F(11);
        sp8C.z = ZREG_F(12);
        MtxPosition(&sp8C, &i_this->mDemoCamCenter);
        i_this->mDemoCamCenter += player->current.pos;
    case 12:
        if (i_this->mDemoCamTimer != 100) {
            break;
        }
        daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
        i_this->mDemoCamMode = 13;
        i_this->mDemoCamTimer = 0;
    case 13:
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        sp8C.x = -80.0f + ZREG_F(13);
        sp8C.y = 80.0f + ZREG_F(14);
        sp8C.z = -200.0f + ZREG_F(15);
        MtxPosition(&sp8C, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += player->current.pos;

        sp8C.x = -30.0f + ZREG_F(16) + i_this->field_0xd78;
        sp8C.y = 70.0f + ZREG_F(17);
        sp8C.z = ZREG_F(18);
        MtxPosition(&sp8C, &i_this->mDemoCamCenter);
        i_this->mDemoCamCenter += player->current.pos;

        if (i_this->mDemoCamTimer > 50) {
            cLib_addCalc2(&i_this->mDemoCamFovy, 25.0f, 0.1f, i_this->field_0xd88);
            cLib_addCalc2(&i_this->field_0xd78, -55.0f, 0.1f, 1.9f * i_this->field_0xd88);
            cLib_addCalc2(&i_this->field_0xd88, 1.0f, 1.0f, 0.05f);

            if (i_this->mDemoCamTimer == 170) {
                sp8 = 1;
                i_this->mMode = 3;
                
                int bitSw = (fopAcM_GetParam(i_this) & 0xFF000000) >> 0x18;
                dComIfGs_onSwitch(bitSw, fopAcM_GetRoomNo(i_this));
                dComIfGs_offSwitch(106, fopAcM_GetRoomNo(i_this));
            }
        }
    }

    if (sp8 != 0) {
        camera->mCamera.Reset(i_this->mDemoCamCenter, i_this->mDemoCamEye);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);

        dComIfGp_event_reset();
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->mDemoCamMode = 0;
    }

    if (i_this->mDemoCamMode != 0) {
        camera->mCamera.Set(i_this->mDemoCamCenter, i_this->mDemoCamEye, i_this->mDemoCamFovy, 0);
        i_this->mDemoCamTimer++;

        if (i_this->mDemoCamMode >= 2 && i_this->mDemoCamMode < 10 && dComIfGp_getEvent()->checkSkipEdge()) {
            Z2GetAudioMgr()->subBgmStop();
            cDmr_SkipInfo = 30;
            dStage_changeScene(4, 0.0f, 0, fopAcM_GetRoomNo(i_this), 0, -1);
        }
    }
}

static void anm_se_set(e_th_class* i_this) {
    u32 sound_id = 0;
    u32 voice_id = 0;
    
    if (i_this->mAnm == 5 && i_this->mpModelMorf->checkFrame(13.0f)) {
        sound_id = Z2SE_EN_TH_BRANDISH_START;
    } else if ((i_this->mAnm == 7 || i_this->mAnm == 8) && i_this->mpModelMorf->checkFrame(1.0f)) {
        voice_id = Z2SE_EN_TH_V_DAMAGE;
    } else if (i_this->mAnm == 0xE && i_this->mpModelMorf->checkFrame(1.0f)) {
        voice_id = Z2SE_EN_TH_V_ENDDEMO;
    } else if (i_this->mAnm == 0x12 && (i_this->mpModelMorf->checkFrame(1.0f) || i_this->mpModelMorf->checkFrame(45.0f))) {
        voice_id = Z2SE_EN_TH_V_LOOKAROUND;
    } else if (i_this->mAnm == 0x17 && i_this->mpModelMorf->checkFrame(7.0f)) {
        voice_id = Z2SE_EN_TH_V_PULLBACK;
    } else if (i_this->mAnm == 0x18) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            sound_id = Z2SE_EN_TH_BRANDISH;
        } else if (i_this->mpModelMorf->checkFrame(18.0f)) {
            voice_id = Z2SE_EN_TH_V_THROW;
        }
    } else if (i_this->mAnm == 0x1A && i_this->mpModelMorf->checkFrame(1.0f)) {
        voice_id = Z2SE_EN_TH_V_TRAP;
    }

    if (sound_id != 0) {
        i_this->mSound.startCreatureSound(sound_id, 0, -1);
    }

    if (voice_id != 0) {
        i_this->mSound.startCreatureVoice(voice_id, -1);
    }
}

static u8 hio_set;

static daE_TH_HIO_c l_HIO;

static int daE_TH_Execute(e_th_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp2C;
    cXyz sp20;
    cXyz sp14;

    if (cDmr_SkipInfo != 0) {
        cDmr_SkipInfo--;
    }

    i_this->mCounter++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->field_0x6a4 != 0) {
        i_this->field_0x6a4--;
    }

    if (i_this->field_0x6a6 != 0) {
        i_this->field_0x6a6--;
    }

    action(i_this);
    i_this->mAcch.CrrPos(dComIfG_Bgsp());

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM((s16)a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM((s16)a_this->shape_angle.x);
    mDoMtx_stack_c::scaleM(l_HIO.base_size, l_HIO.base_size, l_HIO.base_size);
    mDoMtx_stack_c::transM(50.0f + KREG_F(10), 0.0f, 50.0f + KREG_F(1));

    J3DModel* model_p = i_this->mpModelMorf->getModel();
    model_p->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    anm_se_set(i_this);
    i_this->mpModelMorf->modelCalc();

    MTXCopy(model_p->getAnmMtx(4), *calc_mtx);
    sp2C.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp2C, &i_this->mHeadPos);
    i_this->mCcSph[0].SetC(i_this->mHeadPos);
    i_this->mCcSph[0].SetR(35.0f);

    MTXCopy(model_p->getAnmMtx(1), *calc_mtx);
    sp2C.set(BREG_F(11), BREG_F(12), BREG_F(13));
    MtxPosition(&sp2C, &sp20);
    i_this->mCcSph[1].SetC(sp20);
    i_this->mCcSph[1].SetR(60.0f);

    MTXCopy(model_p->getAnmMtx(2), *calc_mtx);
    sp2C.set(BREG_F(14), BREG_F(15), BREG_F(16));
    MtxPosition(&sp2C, &sp20);
    i_this->mCcSph[2].SetC(sp20);
    i_this->mCcSph[2].SetR(75.0f);

    i_this->field_0x6ea = 0;

    s16 sp8 = i_this->mPlayerAngleY - a_this->shape_angle.y;
    if (i_this->mAnm != 0x14 && sp8 < 0x4000 && sp8 > -0x4000) {
        a_this->eyePos = i_this->mHeadPos;
        i_this->field_0x6ea = 1;
    } else if (i_this->field_0x6a4 != 0) {
        sp20.set(-50000.0f, 20000.0f, 18000.0f);
        for (int i = 0; i <= 2; i++) {
            i_this->mCcSph[i].SetC(sp20);
        }
    } else {
        MTXCopy(model_p->getAnmMtx(0x1B), *calc_mtx);
        sp2C.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&sp2C, &i_this->mTailPos);
        a_this->eyePos = i_this->mTailPos;

        sp2C.set(-50.0f + BREG_F(11), 60.0f + BREG_F(12), BREG_F(13));
        MtxPosition(&sp2C, &sp20);
        i_this->mCcSph[1].SetC(sp20);
    }

    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += (40.0f + AREG_F(4)) * l_HIO.base_size;

    for (int i = 0; i <= 2; i++) {
        if (i_this->field_0x6ea != 0) {
            i_this->mCcSph[i].OnTgShield();
            i_this->mCcSph[i].SetTgHitMark((CcG_Tg_HitMark)2);
        } else {
            i_this->mCcSph[i].OffTgShield();
            i_this->mCcSph[i].SetTgHitMark((CcG_Tg_HitMark)0);
        }

        dComIfG_Ccsp()->Set(&i_this->mCcSph[i]);
    }

    MTXCopy(model_p->getAnmMtx(0xC), *calc_mtx);
    sp2C.set(15.0f, -20.0f, -20.0f);
    MtxPosition(&sp2C, &i_this->mHandL_Pos1);
    sp2C.set(15.0f, 20.0f, -10.0f);
    MtxPosition(&sp2C, &i_this->mHandL_Pos2);

    MTXCopy(model_p->getAnmMtx(0x11), *calc_mtx);
    sp2C.set(20.0f, -20.0f, 5.0f);
    MtxPosition(&sp2C, &i_this->mHandR_Pos1);
    sp2C.set(5.0f, 20.0f, 10.0f);
    MtxPosition(&sp2C, &i_this->mHandR_Pos2);

    ke_move(i_this);
    demo_camera(i_this);
    return 1;
}

static int daE_TH_IsDelete(e_th_class* i_this) {
    return 1;
}

static int daE_TH_Delete(e_th_class* i_this) {
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, "E_th");

    if (i_this->mInitHIO) {
        hio_set = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.no);
    }

    if (i_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    e_th_class* i_this = (e_th_class*)a_this;

    i_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_th", 0x1F), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_th", 0x1B), 0, 1.0f, 0, -1, &i_this->mSound, 0, 0x11000084);
    if (i_this->mpModelMorf == NULL || i_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model_p = i_this->mpModelMorf->getModel();
    model_p->setUserArea((uintptr_t)i_this);

    for (u16 i = 0; i < model_p->getModelData()->getJointNum(); i++) {
        model_p->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    if (!i_this->mKeLineMat.init(3, 15, 0)) {
        return 0;
    }

    return 1;
}

static int daE_TH_Create(fopAc_ac_c* a_this) {
    e_th_class* i_this = (e_th_class*)a_this;
    fopAcM_ct(i_this, e_th_class);

    int phase_state = dComIfG_resLoad(&i_this->mPhase, "E_th");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_th PARAM %x\n", fopAcM_GetParam(a_this));

        i_this->field_0x5b6 = fopAcM_GetParam(a_this);
        if (i_this->field_0x5b6 == 2 && dComIfGs_isStageMiddleBoss() && checkItemGet(fpcNm_ITEM_IRONBALL, TRUE)) {
            return cPhs_ERROR_e;
        }
    
        i_this->field_0x5b7 = (fopAcM_GetParam(i_this) & 0xF00) >> 8;
        i_this->field_0x5b8 = (fopAcM_GetParam(i_this) & 0xF000) >> 12;

        if (i_this->field_0x5b6 == 0xFF) {
            i_this->field_0x5b6 = 0;
        }

        if (i_this->field_0x5b7 == 0xF) {
            i_this->field_0x5b7 = 0;
        }

        int sp10 = (fopAcM_GetParam(i_this) & 0xFF0000) >> 0x10;
        i_this->field_0x5b9 = (fopAcM_GetParam(i_this) & 0xFF000000) >> 0x18;

        OS_REPORT("E_th//////////////E_TH SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x4B000)) {
            OS_REPORT("//////////////E_TH SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (!hio_set) {
            i_this->mInitHIO = TRUE;
            hio_set = TRUE;
            l_HIO.no = mDoHIO_CREATE_CHILD("鉄球兵", &l_HIO);
        }

        fopAcM_SetMtx(i_this, i_this->mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_this, -300.0f, -500.0f, -300.0f);
        fopAcM_SetMax(i_this, 300.0f, 500.0f, 300.0f);

        i_this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &i_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        i_this->mAcchCir.SetWall(100.0f, 400.0f + BREG_F(11));

        i_this->health = 300;
        i_this->field_0x560 = 300;

        i_this->mCcStts.Init(0xFA, 0, i_this);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        for (int i = 0; i <= 2; i++) {
            i_this->mCcSph[i].Set(cc_sph_src);
            i_this->mCcSph[i].SetStts(&i_this->mCcStts);
        }

        i_this->mCounter = (u16)cM_rndF(65535.0f) & 0xFF00;
        i_this->mSound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        i_this->mAtInfo.mpSound = &i_this->mSound;

        i_this->mBallID = fopAcM_createChild(PROC_E_TH_BALL, fopAcM_GetID(i_this), 0, &i_this->current.pos, fopAcM_GetRoomNo(i_this), &i_this->shape_angle, NULL, -1, NULL);

        if (i_this->field_0x5b6 == 2 && dComIfGs_isStageMiddleBoss()) {
            i_this->mAction = ACTION_END;
            i_this->mMode = 3;
            i_this->field_0x68a |= 4;
            i_this->mNoDraw = TRUE;
            dComIfGs_offSwitch(106, fopAcM_GetRoomNo(i_this));
        } else {
            if (cDmr_SkipInfo != 0) {
                i_this->mAction = ACTION_WAIT;
                Z2GetAudioMgr()->subBgmStart(Z2BGM_IB_MBOSS);
                dComIfGs_onSwitch(106, fopAcM_GetRoomNo(i_this));
                dComIfGs_onOneZoneSwitch(1, -1);
            } else {
                i_this->mAction = ACTION_START;
                i_this->mNoDraw = TRUE;
                i_this->current.pos.z = 560.0f;
                dComIfGs_offSwitch(106, fopAcM_GetRoomNo(i_this));
            }

            i_this->shape_angle.y = -0x8000;
            i_this->current.angle.y = -0x8000;
        }

        i_this->attention_info.distances[fopAc_attn_BATTLE_e] = 60;
        daE_TH_Execute(i_this);
    }

    return phase_state;
}

static actor_method_class l_daE_TH_Method = {
    (process_method_func)daE_TH_Create,
    (process_method_func)daE_TH_Delete,
    (process_method_func)daE_TH_Execute,
    (process_method_func)daE_TH_IsDelete,
    (process_method_func)daE_TH_Draw,
};

actor_process_profile_definition g_profile_E_TH = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_TH,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_th_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  134,                    // mPriority
  &l_daE_TH_Method,       // sub_method
  0x00040100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
