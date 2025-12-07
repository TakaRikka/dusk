/**
 * @file d_a_e_mb.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_mb.h"
#include "d/actor/d_a_b_bq.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/d_bomb.h"
#include "c/c_damagereaction.h"
#include "Z2AudioLib/Z2Instances.h"

#define ACTION_STANDBY    0
#define ACTION_WALK1      1
#define ACTION_MOVE       2
#define ACTION_WALK2      3
#define ACTION_FIRST_DEMO 4
#define ACTION_END        5

#define ANM_MB_CATCH             6
#define ANM_MB_DEMO_SHOW_BOMB    7
#define ANM_MB_DEMO_SIGNAL       8
#define ANM_MB_DEMO_TURN_CATCH   9
#define ANM_MB_LANDING           10
#define ANM_MB_ROPE_WAIT_A       11
#define ANM_MB_ROPE_WAIT_A_END   12
#define ANM_MB_ROPE_WAIT_A_START 13
#define ANM_MB_ROPE_WAIT_B       14
#define ANM_MB_ROPE_WAIT_B_END   15
#define ANM_MB_ROPE_WAIT_B_START 16
#define ANM_MB_ROPE_WAIT_FLYOUT  17
#define ANM_MB_RUN               18
#define ANM_MB_WALK              19
#define ANM_MG_DEMO_SHOW_BOMB    20
#define ANM_MI_BOMB_POSE         21
#define ANM_MI_ROPE_WAIT         22

daE_MB_HIO_c::daE_MB_HIO_c() {
    no = -1;
    base_size = 1.0f;
    swing_speed = 50.0f;
    swing_descend = 1.5f;
    swing_correction = 0.0f;
    reappear_time = 70;
}

#if DEBUG
void daE_MB_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("  ボス戦ブーメラン猿", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("基本サイズ", &base_size, 0.0f, 5.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("振り子速度", &swing_speed, 0.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("振り子下弦", &swing_descend, 0.5f, 2.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("軌道修正", &swing_correction, -500.0f, 2000.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
    ctx->genSlider("再出現時間（およそ）", &reappear_time, 0.0f, 300.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
}
#endif

static void anm_init(e_mb_class* i_this, int i_anmID, f32 i_morf, u8 i_attr, f32 i_speed) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_mb", i_anmID), i_attr, i_morf, i_speed, 0.0f, -1.0f);
    i_this->mAnm = i_anmID;
}

static int daE_MB_Draw(e_mb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    if (i_this->mAction != ACTION_END) {
        J3DModel* model = i_this->mpModelMorf->getModel();
        g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);

        i_this->mpBtp->entry(model->getModelData());
        i_this->mpModelMorf->entryDL();

        if (i_this->field_0x68c == 1) {
            g_env_light.setLightTevColorType_MAJI(i_this->mpBombModelMorf->getModel(), &a_this->tevStr);
            i_this->mpBombModelMorf->entryDL();
        } else if (i_this->field_0x68c == 2) {
            g_env_light.setLightTevColorType_MAJI(i_this->mpBombModel2Morf->getModel(), &a_this->tevStr);
            i_this->mpBombModel2Morf->entryDL();
        }
    }

    static GXColor l_color = {0x14, 0x0F, 0x00, 0xFF};
    i_this->mRopeMat.update(16, l_color, &a_this->tevStr);
    dComIfGd_set3DlineMat(&i_this->mRopeMat);
    return 1;
}

/* 80708C2D 0003+00 data_80708C2D None */
static u8 l_initHIO;

static daE_MB_HIO_c l_HIO;

static cXyz L_pos1(-2575.0f, 1050.0f, 1060.0f);

static cXyz L_pos2(-1944.0f, 1050.0f, 1060.0f);

static cXyz L_Hpos(-1923.0f, 1432.0f, 1060.0f);

static cXyz R_pos1(2606.0f, 1050.0f, 1070.0f);

static cXyz R_pos2(2126.0f, 1050.0f, 1070.0f);

static cXyz R_Hpos(2099.0f, 1432.0f, 1070.0f);

static cXyz c_pos(0.0f, 1011.0f, 1060.0f);

static void e_mb_standby(e_mb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(a_this->parentActorID);

    if (bq_p != NULL) {
        switch (i_this->mMode) {
        case 0:
            a_this->current.pos = R_pos1;
            if (!bq_p->mDisableDraw) {
                i_this->mMode++;
            }
            break;
        case 1:
            if (bq_p->field_0x6ec == 0) {
                i_this->mTimers[0] = 60;
                i_this->mMode++;
            }
            break;
        case 2:
            if (i_this->mTimers[0] == 0) {
                i_this->mTimers[0] = 60;
                i_this->mSound.startCreatureVoice(Z2SE_MK_V_DEMO_HELP_APPEAR, -1);
                i_this->mMode++;
            }
            break;
        case 3:
            if (i_this->mTimers[0] == 0) {
                bq_p->mDemoMode = 30;
                i_this->mMode = 5;
            }
            break;
        case 5:
            if (bq_p->field_0x6fb == 1) {
                i_this->mTimers[0] = cM_rndF(100.0f) + 250.0f;
            } else if (bq_p->field_0x6fb == 2) {
                i_this->mTimers[0] = cM_rndF(30.0f) + 125.0f + 50.0f;
            }

            if (i_this->mTimers[0] == 0) {
                i_this->mMode++;
            }
            break;
        case 6:
            if (i_this->field_0x6b0 == 0) {
                a_this->current.pos = L_pos1;
                i_this->field_0x5b8 = L_pos2;
            } else {
                a_this->current.pos = R_pos1;
                i_this->field_0x5b8 = R_pos2;
            }

            i_this->mAction = ACTION_WALK1;
            i_this->mMode = 0;

            if (i_this->field_0x8c8 != 0) {
                i_this->field_0x68c = 1;
            }
            break;
        }
    }
}

static void e_mb_walk1(e_mb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz sp18;
    sp18 = i_this->field_0x5b8 - i_this->current.pos;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_MB_WALK, 5.0f, 2, 1.0f);
        i_this->mMode++;
        i_this->mTimers[0] = 100;
        i_this->mSound.startCreatureVoice(Z2SE_MK_V_COME_BACK, -1);
        /* fallthrough */
    case 1:
        a_this->current.angle.y = cM_atan2s(sp18.x, sp18.z);
        a_this->current.angle.x = -cM_atan2s(sp18.y, JMAFastSqrt((sp18.x * sp18.x) + (sp18.z * sp18.z)));
        a_this->speedF = 15.0f + TREG_F(10);

        if (sp18.abs() <= 20.0f + TREG_F(10)) {
            if (i_this->field_0x6b0 != 0) {
                i_this->field_0x5b8 = L_pos2;
            } else {
                i_this->field_0x5b8 = R_pos2;
            }

            i_this->mAction = ACTION_MOVE;
            i_this->mMode = 0;
            i_this->mTimers[2] = 90;
        }
        break;
    }
}

static s8 e_mb_move(e_mb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz sp24;
    sp24 = i_this->field_0x5b8 - i_this->current.pos;

    f32 anm_frame = i_this->mpModelMorf->getFrame();
    s8 var_r28 = 0;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_MB_CATCH, 3.0f, 0, 1.0f);
        i_this->mMode++;
        i_this->speedF = 0.0f;
        i_this->field_0x6a4 = i_this->current.pos;
        break;
    case 1:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, ANM_MB_ROPE_WAIT_A_START, 3.0f, 0, 1.0f);
            i_this->mMode = 2;
            i_this->field_0x6b0 = i_this->field_0x6b0 == 0;
            i_this->mSound.startCreatureVoice(Z2SE_MK_V_ROPE_START, -1);
        }
        break;
    case 2:
        if (i_this->mAnm == ANM_MB_ROPE_WAIT_FLYOUT && i_this->mpModelMorf->isStop()) {
            anm_init(i_this, ANM_MB_ROPE_WAIT_A_START, 5.0f, 0, 1.0f);
        }

        if (i_this->field_0x68c == 0) {
            if (i_this->mAnm == ANM_MB_ROPE_WAIT_A_START) {
                anm_init(i_this, ANM_MB_ROPE_WAIT_B_START, 2.0f, 0, 1.0f);
                i_this->mpModelMorf->setFrame(anm_frame);
            } else if (i_this->mAnm == ANM_MB_ROPE_WAIT_A_END) {
                anm_init(i_this, ANM_MB_ROPE_WAIT_B_END, 2.0f, 0, 1.0f);
                i_this->mpModelMorf->setFrame(anm_frame);
            }
        } else {
            i_this->mSound.startCreatureVoiceLevel(Z2SE_MK_V_SWINGING, -1);
        }

        i_this->mSound.startCreatureSoundLevel(Z2SE_MK_ROPE_SWING, 0, -1);
        i_this->current.angle.y = cM_atan2s(sp24.x, sp24.z);
        i_this->current.angle.x = -cM_atan2s(sp24.y, JMAFastSqrt((sp24.x * sp24.x) + (sp24.z * sp24.z)));
        cLib_addCalc2(&i_this->speedF, l_HIO.swing_speed, 1.0f, 1.0f + (0.1f * l_HIO.swing_speed));

        if (i_this->field_0x68c == 0) {
            if (sp24.abs() <= 400.0f + TREG_F(14) && i_this->mAnm != ANM_MB_LANDING) {
                anm_init(i_this, ANM_MB_LANDING, 5.0f, 0, 1.0f);
            }
        }

        if (sp24.abs() <= 1.1f * i_this->speedF) {
            i_this->current.pos = i_this->field_0x5b8;

            if (i_this->field_0x68c == 1) {
                i_this->field_0x6b0 = (i_this->field_0x6b0 == 0);
                if (i_this->field_0x6b0 == 0) {
                    i_this->field_0x5b8 = L_pos2;
                } else {
                    i_this->field_0x5b8 = R_pos2;
                }

                i_this->field_0x6a4 = i_this->current.pos;
            } else {
                if (i_this->field_0x6b0 == 0) {
                    i_this->field_0x5b8 = L_pos1;
                } else {
                    i_this->field_0x5b8 = R_pos1;
                }

                i_this->mAction = ACTION_WALK2;
                i_this->mMode = 0;
                i_this->mSound.startCreatureSound(Z2SE_EN_MK_FOOTNOTE, 0, -1);
            }

            i_this->speedF = 0.0f;
        }
        break;
    }

    if (i_this->speedF > 1.0f) {
        var_r28 = 1;
    }

    return var_r28;
}

static void e_mb_walk2(e_mb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(i_this->parentActorID);
    if (bq_p != NULL) {
        cXyz sp14;
        sp14 = i_this->field_0x5b8 - i_this->current.pos;

        switch (i_this->mMode) {
        case 0:
            if (i_this->mpModelMorf->isStop()) {
                if (i_this->field_0x8c8 == 0) {
                    bq_p->mDemoMode++;
                    i_this->mAction = ACTION_FIRST_DEMO;
                    i_this->mMode = 0;
                    i_this->field_0x8c8 = 1;
                } else {
                    anm_init(i_this, ANM_MB_RUN, 3.0f, 2, 1.0f);
                    i_this->mMode++;
                }
            }
            break;
        case 1:
            i_this->current.angle.y = cM_atan2s(sp14.x, sp14.z);
            i_this->current.angle.x = -cM_atan2s(sp14.y, JMAFastSqrt((sp14.x * sp14.x) + (sp14.z * sp14.z)));
            i_this->speedF = 25.0f + TREG_F(10);

            if (sp14.abs() <= 30.0f + TREG_F(10)) {
                i_this->mAction = ACTION_STANDBY;
                i_this->mMode = 5;
                i_this->mTimers[0] = l_HIO.reappear_time + cM_rndF(0.5f * l_HIO.reappear_time);
            }
            break;
        }
    }
}

static void e_mb_firstdemo(e_mb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    b_bq_class* bq_p = (b_bq_class*)fopAcM_SearchByID(i_this->parentActorID);

    if (bq_p != NULL) {
        cXyz sp20;
        sp20 = i_this->field_0x5b8 - i_this->current.pos;

        switch (i_this->mMode) {
        case 0:
            anm_init(i_this, ANM_MB_DEMO_SIGNAL, 5.0f, 2, 1.0f);
            i_this->current.angle.y += 0x8000;
            i_this->mMode++;
            i_this->mTimers[0] = 30;
            i_this->mSound.startCreatureVoice(Z2SE_MK_V_D_DELIGHT, -1);
            break;
        case 1:
            if (i_this->mTimers[0] == 0) {
                anm_init(i_this, ANM_MB_DEMO_TURN_CATCH, 5.0f, 0, 1.0f);
                i_this->mMode++;
            }
            break;
        case 2:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, ANM_MB_DEMO_SHOW_BOMB, 5.0f, 0, 1.0f);
                i_this->mpBombModel2Morf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_mb", 0x14), 0, 5.0f, 1.0f, 0.0f, -1.0f, NULL);
                i_this->field_0x68c = 2;
                i_this->mMode++;
                i_this->mTimers[0] = 120;
                i_this->mSound.startCreatureVoice(Z2SE_MK_V_D_BOMB_SHOW, -1);
            }
            break;
        case 4:
            i_this->field_0x5b8 = R_pos2;
            i_this->field_0x68c = 1;
            i_this->speedF = 0.0f;
            anm_init(i_this, ANM_MB_CATCH, 0.0f, 0, 1.0f);
            i_this->mMode = 5;
            break;
        case 5:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_MOVE;
                anm_init(i_this, ANM_MB_ROPE_WAIT_FLYOUT, 0.0f, 0, 1.0f);
                i_this->field_0x6a4 = i_this->current.pos;
                i_this->mMode = 2;
                i_this->field_0x6b0 = i_this->field_0x6b0 == 0;
                i_this->mSound.startCreatureVoice(Z2SE_MK_V_ROPE_START, -1);
            }
            break;
        }
    }
}

static void e_mb_end(e_mb_class* i_this) {
    i_this->current.pos.y = 20000.0f;
}

static s8 action(e_mb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz sp8;
    s8 var_r29 = 0;

    switch (i_this->mAction) {
    case ACTION_STANDBY:
        e_mb_standby(i_this);
        break;
    case ACTION_WALK1:
        e_mb_walk1(i_this);
        break;
    case ACTION_MOVE:
        var_r29 = e_mb_move(i_this);
        break;
    case ACTION_WALK2:
        e_mb_walk2(i_this);
        break;
    case ACTION_FIRST_DEMO:
        e_mb_firstdemo(i_this);
        break;
    case ACTION_END:
        e_mb_end(i_this);
        break;
    }

    cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
    sp8.x = 0.0f;
    sp8.y = 0.0f;
    sp8.z = a_this->speedF;
    MtxPosition(&sp8, &a_this->speed);
    a_this->current.pos += a_this->speed;

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 4, 0x800);
    return var_r29;
}

static int daE_MB_Execute(e_mb_class* i_this) {
    if (cDmrNowMidnaTalk()) {
        return 1;
    }

    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    cXyz spE4;
    cXyz spD8;
    cXyz spCC;

    i_this->mCounter++;

    for (int i = 0; i < 3; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->field_0x6a2 != 0) {
        i_this->field_0x6a2--;
    }

    f32 var_f31 = 0.0f;

    if (action(i_this) == 1) {
        spE4 = i_this->field_0x5b8 - i_this->field_0x6a4;
        f32 temp_f30 = spE4.abs();

        spE4 = i_this->field_0x5b8 - i_this->current.pos;

        f32 temp_r29 = spE4.abs();
        f32 temp_f28 = temp_f30 * ((0.1f * l_HIO.swing_descend) + BREG_F(19));
        s16 spA = 32768.0f * (temp_r29 / temp_f30);

        var_f31 = temp_f28 * cM_ssin(spA);

        if (cM_scos(spA) > -0.3f + KREG_F(16)) {
            if (i_this->mAnm == ANM_MB_ROPE_WAIT_A_START) {
                anm_init(i_this, ANM_MB_ROPE_WAIT_A_END, 35.0f + KREG_F(17), 0, 1.0f);
            } else if (i_this->mAnm == ANM_MB_ROPE_WAIT_B_START) {
                anm_init(i_this, ANM_MB_ROPE_WAIT_B_END, 35.0f + KREG_F(17), 0, 1.0f);
            }
        } else if (i_this->mAnm == ANM_MB_ROPE_WAIT_A_END) {
            anm_init(i_this, ANM_MB_ROPE_WAIT_A_START, 10.0f + KREG_F(18), 0, 1.0f);
        } else if (i_this->mAnm == ANM_MB_ROPE_WAIT_B_END) {
            anm_init(i_this, ANM_MB_ROPE_WAIT_B_START, 10.0f + KREG_F(18), 0, 1.0f);
        }
    } else {
        cLib_addCalcAngleS2(&i_this->shape_angle.x, 0, 1, 0x400);
    }

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y - var_f31, i_this->current.pos.z + l_HIO.swing_correction);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.base_size, l_HIO.base_size, l_HIO.base_size);

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));

    if (i_this->field_0x5d4 == 0) {
        i_this->field_0x5d4 = 30.0f + cM_rndF(60.0f);
    } else {
        i_this->field_0x5d4--;
        if (i_this->field_0x5d4 <= 12) {
            i_this->mBtpFrame = 12 - i_this->field_0x5d4;
        } else {
            i_this->mBtpFrame = 0.0f;
        }
    }

    i_this->mpBtp->setFrame(i_this->mBtpFrame);
    i_this->mpModelMorf->modelCalc();

    MtxPtr temp_r27 = mDoMtx_stack_c::get();
    MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(0x16), temp_r27);

    spE4.set(JREG_F(2), JREG_F(3), JREG_F(4));
    mDoMtx_stack_c::multVec(&spE4, &i_this->field_0x5dc);

    fopAcM_OffStatus(i_this, 0);
    i_this->attention_info.flags = 0;

    if (i_this->field_0x68c != 0) {
        if (i_this->field_0x68c == 1) {
            temp_r27 = mDoMtx_stack_c::get();
            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(0x1C), temp_r27);

            mDoMtx_stack_c::transM(60.0f + JREG_F(17), -35.0f + JREG_F(18), 15.0f + JREG_F(19));
            mDoMtx_stack_c::XrotM(AREG_S(1) - 500);
            mDoMtx_stack_c::ZrotM(AREG_S(2) + 20500);

            temp_r27 = mDoMtx_stack_c::get();
            i_this->mpBombModelMorf->getModel()->setBaseTRMtx(temp_r27);
            i_this->mpBombModelMorf->modelCalc();
            mDoMtx_stack_c::multVecZero(&spD8);

            i_this->mCcSph.SetC(spD8);

            if (dComIfGp_checkPlayerStatus0(0, 0x80000)) {
                i_this->mCcSph.SetR(400.0f + YREG_F(13));
            } else {
                i_this->mCcSph.SetR(100.0f);
            }

            i_this->eyePos = spD8;

            if (i_this->mCcSph.ChkTgHit()) {
                i_this->field_0x6a2 = 10;
                i_this->mAtInfo.mpCollider = i_this->mCcSph.GetTgHitObj();
                cc_at_check(i_this, &i_this->mAtInfo);

                if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                    dBomb_c::createEnemyBombBoomerang(&i_this->eyePos, &i_this->current.angle, fopAcM_GetRoomNo(i_this));
                    i_this->field_0x68c = 0;
                    i_this->mSound.startCreatureVoice(Z2SE_MK_V_LOSE_BOMB, -1);
                }
            }

            cMtx_YrotS(*calc_mtx, fopAcM_searchPlayerAngleY(i_this));
            spE4.x = 0.0f;
            spE4.y = 50.0f + TREG_F(14);
            spE4.z = 150.0f + TREG_F(15);
            MtxPosition(&spE4, &i_this->attention_info.position);
            i_this->attention_info.position += i_this->eyePos;

            fopAcM_OnStatus(i_this, 0);
            i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        } else {
            i_this->mpBombModel2Morf->play(NULL, 0, 0);

            temp_r27 = mDoMtx_stack_c::get();
            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(0x15), temp_r27);

            temp_r27 = mDoMtx_stack_c::get();
            i_this->mpBombModel2Morf->getModel()->setBaseTRMtx(temp_r27);
            i_this->mpBombModel2Morf->modelCalc();

            spD8.set(-20000.0f, 40000.0f, 55000.0f);
            i_this->mCcSph.SetC(spD8);
            i_this->eyePos = i_this->current.pos;
            i_this->eyePos.y += 200.0f;
        }
    } else {
        i_this->eyePos = i_this->current.pos;
        i_this->eyePos.y += 200.0f;
        spD8.set(-20000.0f, 40000.0f, 55000.0f);
        i_this->mCcSph.SetC(spD8);
    }

    dComIfG_Ccsp()->Set(&i_this->mCcSph);

    cXyz spC0;
    cXyz spB4(L_Hpos);
    cXyz spA8(R_Hpos);

    spC0 = spB4 + ((spA8 - spB4) * 0.5f);
    spC0.y += 1800.0f + JREG_F(8);

    cXyz* rope_pos = i_this->mRopeMat.getPos(0);
    cXyz sp9C;

    if (i_this->mAction == ACTION_MOVE && i_this->mMode >= 2) {
        sp9C = spC0 - i_this->field_0x5dc;
        cLib_addCalc0(&i_this->field_0x6f0, 1.0f, 30.0f);
    } else {
        if (i_this->field_0x6b0 == 0) {
            sp9C = spC0 - spB4;
        } else {
            sp9C = spC0 - spA8;
        }

        cLib_addCalc2(&i_this->field_0x6f0, -500.0f, 0.1f, 20.0f);
    }

    sp9C *= (1.0f / 15.0f);

    for (int i = 0; i < 16; i++, rope_pos++) {
        *rope_pos = spC0 - (sp9C * (f32)i);
        
        s16 sp8 = 32768.0f * (i / 15.0f);
        rope_pos->y += i_this->field_0x6f0 * cM_ssin(sp8);
    }

    return 1;
}

static int daE_MB_IsDelete(e_mb_class* i_this) {
    return 1;
}

static int daE_MB_Delete(e_mb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fpc_ProcID id = fopAcM_GetID(a_this);

    dComIfG_resDelete(&i_this->mPhase, "E_mb");
    
    if (i_this->mInitHIO) {
        l_initHIO = false;
        mDoHIO_DELETE_CHILD(l_HIO.no);
    }

    if (a_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_mb_class* a_this = (e_mb_class*)i_this;

    a_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_mb", 0x19), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_mb", 10), 0, 1.0f, 0, -1, &a_this->mSound, 0x80000, 0x11020084);
    if (a_this->mpModelMorf == NULL || a_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    a_this->mpBtp = new mDoExt_btpAnm();
    if (a_this->mpBtp == NULL) {
        return cPhs_ERROR_e;
    }

    J3DAnmTexPattern* btp = (J3DAnmTexPattern*)dComIfG_getObjectRes("E_mb", 0x1E);
    if (!a_this->mpBtp->init(a_this->mpModelMorf->getModel()->getModelData(), btp, TRUE, 0, 1.0f, 0, -1)) {
        return cPhs_ERROR_e;
    }

    a_this->mpBombModelMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("E_mb", 0x1B), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_mb", 0x15), 0, 1.0f, 0, 1, 0, &a_this->mSound, 0x80000, 0x11000084);
    if (a_this->mpBombModelMorf == NULL || a_this->mpBombModelMorf->getModel() == NULL) {
        return 0;
    }

    a_this->mpBombModel2Morf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("E_mb", 0x1A), NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_mb", 0x14), 0, 1.0f, 0, 1, 0, &a_this->mSound, 0x80000, 0x11000084);
    if (a_this->mpBombModel2Morf == NULL || a_this->mpBombModel2Morf->getModel() == NULL) {
        return 0;
    }

    if (!a_this->mRopeMat.init(1, 16, (ResTIMG*)dComIfG_getObjectRes("E_mb", 0x21), 1)) {
        return 0;
    }

    f32* size_p = a_this->mRopeMat.getSize(0);
    for (int i = 0; i < 16; i++) {
        *size_p = (XREG_S(0) * 0.1f) + 10.0f;
        size_p++;
    }

    return 1;
}

static int daE_MB_Create(fopAc_ac_c* i_this) {
    e_mb_class* a_this = (e_mb_class*)i_this;
    fopAcM_ct(a_this, e_mb_class);

    int phase_state = dComIfG_resLoad(&a_this->mPhase, "E_mb");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_MB PARAM %x\n", fopAcM_GetParam(i_this));
        
        u8 swbit = (fopAcM_GetParam(i_this) >> 0x18) & 0xFF;
        if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(i_this))) {
            OS_REPORT("E_MB やられ後なので再セットしません\n");
            return cPhs_ERROR_e;
        }

        a_this->field_0x5b6 = fopAcM_GetParam(i_this) & 0xFF;

        OS_REPORT("E_MB//////////////E_MB SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x58A0)) {
            OS_REPORT("//////////////E_MB SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (!l_initHIO) {
            a_this->mInitHIO = TRUE;
            l_initHIO = TRUE;
            l_HIO.no = mDoHIO_CREATE_CHILD("ボス戦ブーメラン猿", &l_HIO);
        }

        a_this->mSound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        a_this->mAtInfo.mpSound = &a_this->mSound;

        a_this->mColliderStts.Init(0xFF, 0, i_this);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x0}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 60.0f} // mSph
            } // mSphAttr
        };

        a_this->mCcSph.Set(cc_sph_src);
        a_this->mCcSph.SetStts(&a_this->mColliderStts);

        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        i_this->attention_info.distances[fopAc_attn_BATTLE_e] = 22;
        a_this->field_0x6b0 = 1;
        daE_MB_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_MB_Method = {
    (process_method_func)daE_MB_Create,
    (process_method_func)daE_MB_Delete,
    (process_method_func)daE_MB_Execute,
    (process_method_func)daE_MB_IsDelete,
    (process_method_func)daE_MB_Draw,
};

extern actor_process_profile_definition g_profile_E_MB = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_MB,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_mb_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  164,                    // mPriority
  &l_daE_MB_Method,       // sub_method
  0x00044000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
