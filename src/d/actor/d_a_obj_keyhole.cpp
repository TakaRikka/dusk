/**
 * @file d_a_obj_keyhole.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_keyhole.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/actor/d_a_player.h"
#include "Z2AudioLib/Z2Instances.h"

daObj_Keyhole_HIO_c::daObj_Keyhole_HIO_c() {
    id = -1;
    obj_size = 1.0f;
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        obj_keyhole_class* i_this = (obj_keyhole_class*)model->getUserArea();

        if (i_this != NULL) {
            if (jnt_no == 1) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                MtxTrans(i_this->field_0x2afc, 0.0f, 0.0f, 1);
                cMtx_XrotM(*calc_mtx, i_this->field_0x2b00);
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static int daObj_Keyhole_Draw(obj_keyhole_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    g_env_light.settingTevStruct(16, &actor->current.pos, &actor->tevStr);

    if (!i_this->hide_lock) {
        if (i_this->lock_model != NULL) {
            g_env_light.setLightTevColorType_MAJI(i_this->lock_model, &actor->tevStr);
            mDoExt_modelUpdateDL(i_this->lock_model);
        } else {
            g_env_light.setLightTevColorType_MAJI(i_this->lock_modelMorf->getModel(), &actor->tevStr);
            i_this->lock_modelMorf->entryDL();
        }
    }

    for (int i = 0; i < 6; i++) {
        kh_chain_s* chain_s = &i_this->chain_s[i];
        for (int j = 0; j < i_this->chain_num; j++) {
            dComIfGp_entrySimpleModel(chain_s->model[j], fopAcM_GetRoomNo(actor));
        }
    }

    return 1;
}

static void chain_control1(obj_keyhole_class* i_this, kh_chain_s* i_chain_s, int unused) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp24;
    cXyz sp18;
    int i;
    s16 spA, sp8;

    i_chain_s->field_0x58[0] = i_chain_s->field_0x40;

    cXyz* var_r31 = &i_chain_s->field_0x58[1];
    csXyz* sp10 = &i_chain_s->field_0x1d8[1];

    f32 var_f28;
    if (i_this->arg0 == 0) {
        var_f28 = 10.0f + i_this->field_0x2ae8 + ZREG_F(4);
    } else {
        var_f28 = 10.0f + (10.0f + i_this->field_0x2ae8 + ZREG_F(4));
    }

    sp24.x = 0.0f;
    sp24.y = 0.0f;

    if (i_this->arg0 == 0) {
        sp24.z = 10.0f + TREG_F(0);
    } else {
        sp24.z = 30.0f + TREG_F(0);
    }

    f32 temp_f26 = -13.0f + TREG_F(1);

    for (i = 1; i < i_this->chain_num; i++, var_r31++, sp10++) {
        f32 temp_f31 = var_r31[0].x - var_r31[-1].x;
        f32 temp_f27;
        f32 temp_f30 = var_r31[0].z - var_r31[-1].z;
        f32 var_f29  = var_r31[0].y + temp_f26;

        if (var_f29 <= var_f28) {
            var_f29 = var_f28;
        }

        temp_f27 = var_f29 -  var_r31[-1].y;
        
        spA = (s16)cM_atan2s(temp_f31, temp_f30);
        sp8 = -cM_atan2s(temp_f27, JMAFastSqrt(SQUARE(temp_f31) + SQUARE(temp_f30)));

        cMtx_YrotS(*calc_mtx, spA);
        cMtx_XrotM(*calc_mtx, sp8);
        MtxPosition(&sp24, &sp18);

        var_r31[0].x = var_r31[-1].x + sp18.x;
        var_r31[0].y = var_r31[-1].y + sp18.y;
        var_r31[0].z = var_r31[-1].z + sp18.z;
    }
}

static void chain_control1_2(obj_keyhole_class* i_this, kh_chain_s* i_chain_s, int unused) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp48;
    cXyz sp3C;
    int i;
    s16 spA, sp8;

    i_chain_s->field_0x58[0] = i_chain_s->field_0x40;

    cXyz* var_r31 = &i_chain_s->field_0x58[1];
    cXyz* var_r28 = &i_chain_s->field_0x118[1];
    csXyz* var_r27 = &i_chain_s->field_0x1d8[1];

    f32 temp_f25;
    f32 var_f28;
    if (i_this->arg0 == 0) {
        var_f28 = 10.0f + i_this->field_0x2ae8 + ZREG_F(4);
    } else {
        var_f28 = 10.0f + (10.0f + i_this->field_0x2ae8 + ZREG_F(4));
    }

    sp48.x = 0.0f;
    sp48.y = 0.0f;

    f32 var_f27, var_f26;
    if (i_this->arg0 != 0) {
        sp48.z = 30.0f + TREG_F(0);
        var_f26 = 0.8f + TREG_F(9);
        var_f27 = -20.0f + TREG_F(1);
    } else {
        sp48.z = 10.0f + TREG_F(0);
        var_f26 = 0.66f + TREG_F(9);
        var_f27 = -13.0f + TREG_F(1);
    }

    cXyz sp30;

    for (i = 1; i < i_this->chain_num; i++, var_r31++, var_r27++, var_r28++) {
        sp30.x = (0.2f + TREG_F(7)) * (i_chain_s->field_0x238 * cM_ssin((i_this->field_0x61a * (TREG_S(6) + 5500)) + (i * (TREG_S(5) - 5300))));
        sp30.z = (0.2f + TREG_F(7)) * (i_chain_s->field_0x238 * cM_ssin((i_this->field_0x61a * (TREG_S(6) + 5800)) + (i * (TREG_S(5) - 5500))));

        temp_f25 = sp30.x + (var_r28[0].x + (var_r31[0].x - var_r31[-1].x));
        f32 var_f29;
        f32 temp_f31;
        f32 temp_f30 = sp30.z + (var_r28[0].z + (var_r31[0].z - var_r31[-1].z));
    
        var_f29 = var_r31[0].y + var_f27;
        if (var_f29 <= var_f28) {
            var_f29 = var_f28;
        }
    
        temp_f31 = var_f29 -  var_r31[-1].y;

        sp8 = -cM_atan2s(temp_f31, temp_f30);
        spA = (s16)cM_atan2s(temp_f25, JMAFastSqrt(SQUARE(temp_f31) + SQUARE(temp_f30)));

        cMtx_XrotS(*calc_mtx, sp8);
        cMtx_YrotM(*calc_mtx, spA);
        MtxPosition(&sp48, &sp3C);
        *var_r28 = *var_r31;

        var_r31[0].x = var_r31[-1].x + sp3C.x;
        var_r31[0].y = var_r31[-1].y + sp3C.y;
        var_r31[0].z = var_r31[-1].z + sp3C.z;

        *var_r28 = (*var_r31 - *var_r28) * var_f26;
        var_r27[-1].y = spA;
        var_r27[-1].x = sp8;

        if (i == i_this->chain_num - 1) {
            var_r27[0] = var_r27[-1];
        }
    }
}

static void chain_control2(obj_keyhole_class* i_this, kh_chain_s* i_chain_s, int unused) {
    cXyz sp30;
    cXyz sp24;
    fopAc_ac_c* actor = &i_this->actor;
    int i;
    s16 spE, spC;

    i_chain_s->field_0x58[i_this->chain_num - 1] = i_chain_s->field_0x4c;

    cXyz* var_r31 = &i_chain_s->field_0x58[i_this->chain_num - 2];
    csXyz* var_r28 = &i_chain_s->field_0x1d8[i_this->chain_num - 2];
    
    sp30.x = 0.0f;
    sp30.y = 0.0f;

    if (i_this->arg0 == 0) {
        sp30.z = 10.0f + TREG_F(0);
    } else {
        sp30.z = 30.0f + TREG_F(0);
    }

    cXyz sp18;

    for (i = i_this->chain_num - 2; i >= 0; i--, var_r31--, var_r28--) {
        sp18.x = (0.2f + TREG_F(7)) * (i_chain_s->field_0x238 * cM_ssin(((i_this->field_0x61a * (TREG_S(6) + 5500)) + (i * (TREG_S(5) + 3300)))));
        sp18.y = (0.5f + TREG_F(6)) * (i_chain_s->field_0x238 * cM_ssin(((i_this->field_0x61a * (TREG_S(7) + 6000)) + (i * (TREG_S(5) + 3800)))));
        sp18.z = (0.2f + TREG_F(7)) * (i_chain_s->field_0x238 * cM_ssin(((i_this->field_0x61a * (TREG_S(6) + 5800)) + (i * (TREG_S(5) + 3500)))));

        f32 temp_f31 = sp18.x + (var_r31[0].x - var_r31[1].x);
        f32 temp_f29 = sp18.y + (var_r31[0].y - var_r31[1].y);
        f32 temp_f30 = sp18.z + (var_r31[0].z - var_r31[1].z);

        spC = (s16)cM_atan2s(temp_f31, temp_f30);
        spE = -cM_atan2s(temp_f29, JMAFastSqrt(SQUARE(temp_f31) + SQUARE(temp_f30)));

        cMtx_YrotS(*calc_mtx, spC);
        cMtx_XrotM(*calc_mtx, spE);
        MtxPosition(&sp30, &sp24);

        var_r31[0].x = var_r31[1].x + sp24.x;
        var_r31[0].y = var_r31[1].y + sp24.y;
        var_r31[0].z = var_r31[1].z + sp24.z;
        var_r28[0].x = -spE;
        var_r28[0].y = spC + 0x8000;

        if (i == i_this->chain_num - 2) {
            var_r28[1] = var_r28[0];
        }
    }
}

static void chain_control3(obj_keyhole_class* i_this, kh_chain_s* i_chain_s, int unused) {
    cXyz sp30;
    cXyz sp24;

    sp30 = i_chain_s->field_0x58[0] - i_chain_s->field_0x40;
    
    cXyz* var_r28 = i_chain_s->field_0x58;
    for (int i = 0; i < i_this->chain_num; i++, var_r28++) {
        *var_r28 -= sp30 * ((f32)((i_this->chain_num - 1) - i) / (f32)(i_this->chain_num - 1));
    }
}

static u8 hio_set;

static daObj_Keyhole_HIO_c l_HIO;

static void chain_move(obj_keyhole_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    static cXyz lock_pos[] = {
        cXyz(140.0f, 130.0f, 0.0f),
        cXyz(-140.0f, 130.0f, 0.0f),
        cXyz(170.0f, 30.0f, 0.0f),
        cXyz(-170.0f, 30.0f, 0.0f),
        cXyz(140.0f, -100.0f, 0.0f),
        cXyz(-140.0f, -100.0f, 0.0f),
    };

    f32 var_f31 = 1.0f;
    if (i_this->arg0 != 0) {
        var_f31 = 2.0f + ZREG_F(0);
    }

    for (int i = 0; i < 6; i++) {
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        MtxScale(var_f31, var_f31, var_f31, 1);
        MtxPosition(&lock_pos[i], &i_this->chain_s[i].field_0x40);
        i_this->chain_s[i].field_0x40 += actor->home.pos;
    
        if (i_this->field_0x2ca6 == 0) {
            chain_control1(i_this, &i_this->chain_s[i], i);
            i_this->chain_s[i].field_0x4c = i_this->field_0x2ca8;
            chain_control2(i_this, &i_this->chain_s[i], i);
            chain_control3(i_this, &i_this->chain_s[i], i);
            cLib_addCalc0(&i_this->chain_s[i].field_0x238, 0.2f, 0.5f + TREG_F(5));
        } else {
            chain_control1_2(i_this, &i_this->chain_s[i], i);
            f32 var_f28 = 0.5f + TREG_F(5);
            cLib_addCalc0(&i_this->chain_s[i].field_0x238, 0.2f, var_f28);
        }

        if (daPy_getPlayerActorClass() != NULL && daPy_getPlayerActorClass()->checkFrontRollCrash()) {
            f32 temp_f30 = i_this->chain_s[i].field_0x58[0].x - player->current.pos.x;
            f32 temp_f29 = i_this->chain_s[i].field_0x58[0].z - player->current.pos.z;

            if (JMAFastSqrt(SQUARE(temp_f30) + SQUARE(temp_f29)) < 150.0f) {
                i_this->chain_s[i].field_0x238 = 7.0f + TREG_F(4);
            } else {
                temp_f30 = i_this->chain_s[i].field_0x58[i_this->chain_num - 1].x - player->current.pos.x;
                temp_f29 = i_this->chain_s[i].field_0x58[i_this->chain_num - 1].z - player->current.pos.z;
                if (JMAFastSqrt(SQUARE(temp_f30) + SQUARE(temp_f29)) < 150.0f) {
                    i_this->chain_s[i].field_0x238 = 7.0f + TREG_F(4);
                }
            }
        } else {
            for (int j = 0; j < 3; j++) {
                cXyz sp28;
                if (i_this->arg0 == 0) {
                    sp28 = i_this->chain_s[i].field_0x58[(j * 4) + 4];
                    i_this->chain_s[i].ccSph[j].SetR(20.0f + JREG_F(4));
                } else {
                    sp28 = i_this->chain_s[i].field_0x58[(j * 3) + 3];
                    i_this->chain_s[i].ccSph[j].SetR(35.0f + JREG_F(4));
                }

                if (i_this->hit_timer != 0) {
                    sp28.y -= 13000.0f;
                }

                i_this->chain_s[i].ccSph[j].SetC(sp28);
                dComIfG_Ccsp()->Set(&i_this->chain_s[i].ccSph[j]);

                if (i_this->chain_s[i].ccSph[j].ChkTgHit()) {
                    i_this->atInfo.mpCollider = i_this->chain_s[i].ccSph[j].GetTgHitObj();
                    i_this->hit_timer = 10;
                    at_power_check(&i_this->atInfo);

                    if (i_this->atInfo.mHitType == 2) {
                        i_this->chain_s[i].field_0x238 = 25.0f + TREG_F(8);
                    } else {
                        i_this->chain_s[i].field_0x238 = 12.0f + TREG_F(4);
                    }

                    i_this->atInfo.mpCollider = i_this->chain_s[i].ccSph[j].GetTgHitObj();
                    def_se_set(&i_this->sound, i_this->atInfo.mpCollider, 0x35, NULL);
                }
            }
        }
    }

    f32 temp_f31 = 0.25f + (0.7f + TREG_F(2));

    kh_chain_s* chain;
    s16 sp8;
    for (int i = 0; i < 6; i++) {
        chain = &i_this->chain_s[i];
        sp8 = 0;

        for (int j = 0; j < i_this->chain_num; j++) {
            MtxTrans(chain->field_0x58[j].x, chain->field_0x58[j].y, chain->field_0x58[j].z, 0);

            if (i_this->field_0x2ca6 == 0) {
                cMtx_YrotM(*calc_mtx, chain->field_0x1d8[j].y);
                cMtx_XrotM(*calc_mtx, chain->field_0x1d8[j].x);
            } else {
                cMtx_XrotM(*calc_mtx, chain->field_0x1d8[j].x);
                cMtx_YrotM(*calc_mtx, chain->field_0x1d8[j].y);
            }

            cMtx_ZrotM(*calc_mtx, sp8);
            MtxScale(temp_f31, temp_f31, temp_f31, 1);
            MtxTrans(0.0f, 0.0f, -5.0f + TREG_F(3), 1);
            cMtx_YrotM(*calc_mtx, 0x4000);
            chain->model[j]->setBaseTRMtx(*calc_mtx);

            sp8 += (s16)(TREG_S(0) + 0x3D00);
        }
    }
}

static void open(obj_keyhole_class* i_this) {
    switch (i_this->mode) {
    case 0:
        cLib_addCalc2(&i_this->field_0x2afc, 8.0f, 1.0f, 1.0f);
        if (i_this->field_0x2afc >= 8.0f) {
            i_this->mode++;
        }
        break;
    case 1:
        cLib_addCalcAngleS2(&i_this->field_0x2b00, 0x1900, 1, 0x320);
        if (i_this->field_0x2b00 >= 0x1900) {
            i_this->action = 2;
            i_this->mode = 0;
            i_this->field_0x2ca6 = 1;
        }
        break;
    case 10:
        i_this->lock_modelMorf->setPlaySpeed(1.0f);

        if (i_this->lock_modelMorf->checkFrame(30.0f)) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER3, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->lock_modelMorf->isStop()) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER3, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            i_this->action = 2;
            i_this->mode = 0;
        }
        break;
    }
}

static void drop(obj_keyhole_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp18;

    i_this->hit_timer = 10;
    cLib_addCalc0(&i_this->field_0x2af4, 1.0f, 1.5f);
    cLib_addCalc0(&i_this->field_0x2af8, 1.0f, 4.0f);

    f32 var_f31;
    if (i_this->arg0 == 0) {
        var_f31 = 0.0f;
    } else {
        var_f31 = (40.0f + ZREG_F(7)) * l_HIO.obj_size;
    }

    switch (i_this->mode) {
    case 0:
        if (i_this->arg0 == 0) {
            var_f31 = 70.0f;
        } else {
            var_f31 = (100.0f + ZREG_F(8)) * l_HIO.obj_size;
        }

        if (actor->current.pos.y <= i_this->field_0x2ae8 + var_f31) {
            actor->current.pos.y = i_this->field_0x2ae8 + var_f31;
            i_this->mode = 1;
            dComIfGp_getVibration().StartShock(4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));

            f32 var_f30;
            if (i_this->arg0 == 0) {
                i_this->sound.startCreatureSound(Z2SE_OBJ_KEYLOCK_BOUND, 0, -1);
                var_f30 = 0.8f;
            } else {
                var_f30 = 1.5f;
            }

            fopAcM_effSmokeSet1(&i_this->field_0x2c9c, &i_this->field_0x2ca0, &actor->current.pos, NULL, var_f30, &actor->tevStr, 1);

            cMtx_YrotS(*calc_mtx, actor->current.angle.y);
            if (cM_rndF(1.0f) < 0.5f) {
                sp18.x = 5.0f;
            } else {
                sp18.x = -5.0f;
            }
            sp18.y = 12.0f;
            sp18.z = 3.0f;
            MtxPosition(&sp18, &actor->speed);

            i_this->timers[0] = 10;
            i_this->field_0x2ae4 = 10000.0f + ZREG_F(1);

            if (i_this->arg0 != 0) {
                actor->speed.x *= 1.5f;
                actor->speed.z *= 1.5f;
                i_this->field_0x2ca6 = 1;
            }
        }
        break;
    case 1:
        cLib_addCalcAngleS2(&actor->shape_angle.x, 0, 1, (BREG_S(7) + 0x400));
        i_this->field_0x2aec += (BREG_S(8) + 0x17B8);
        if (i_this->timers[0] == 1) {
            i_this->setOpenEnd();
        }
        break;
    }

    actor->current.pos += actor->speed;
    actor->speed.y -= 5.0f;

    if (actor->current.pos.y <= (i_this->field_0x2ae8 + var_f31)) {
        actor->current.pos.y = i_this->field_0x2ae8 + var_f31;
        actor->speed.x *= 0.7f;
        actor->speed.z *= 0.7f;
        actor->speed.y = 0.0f;
    }
}

static int daObj_Keyhole_Execute(obj_keyhole_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp34;
    cXyz sp28;

    i_this->field_0x61a++;

    for (int i = 0; i < 2; i++) {
        if (i_this->timers[i] != 0) {
            i_this->timers[i]--;
        }
    }

    if (i_this->hit_timer != 0) {
        i_this->hit_timer--;
    }

    switch (i_this->action) {
    case 0:
        if (i_this->checkOpen()) {
            i_this->action = 1;
            i_this->field_0x2ca8 = actor->home.pos;
            if (i_this->arg0 != 0) {
                i_this->mode = 10;
            }
        }
        break;
    case 1:
        open(i_this);
        break;
    case 2:
        drop(i_this);
        break;
    }

    s16 sp8 = 0;
    if (i_this->action == 0) {
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        sp34.x = 0.2f * (i_this->field_0x2ae4 * cM_ssin(i_this->field_0x61a * (TREG_S(2) + 5800)));
        sp34.y = i_this->field_0x2ae4 * cM_ssin(i_this->field_0x61a * (TREG_S(3) + 6400));
        sp34.z = i_this->field_0x2ae4 * cM_ssin(i_this->field_0x61a * (TREG_S(4) + 6000));
        MtxPosition(&sp34, &sp28);
        actor->current.pos = actor->home.pos + sp28;
        i_this->field_0x2ca8 = actor->current.pos;

        sp8 = 300.0f * (i_this->field_0x2ae4 * cM_ssin((i_this->field_0x61a * (TREG_S(2) + 5800))));
        cLib_addCalc0(&i_this->field_0x2ae4, 0.2f, 0.5f + TREG_F(5));
    } else if (i_this->action == 2) {
        sp8 = i_this->field_0x2ae4;
        cLib_addCalc0(&i_this->field_0x2ae4, 1.0f, 500.0f + ZREG_F(2));
    }

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::YrotM(i_this->field_0x2aec);
    mDoMtx_stack_c::XrotM((s16)sp8);
    mDoMtx_stack_c::YrotM(-i_this->field_0x2aec);

    if (i_this->arg0 == 0) {
        mDoMtx_stack_c::XrotM(actor->shape_angle.x);
    } else {
        mDoMtx_stack_c::XrotM(actor->shape_angle.x - 0x4000);
    }

    mDoMtx_stack_c::transM(i_this->field_0x2af0, i_this->field_0x2af4, i_this->field_0x2af8);

    J3DModel* spC;
    if (i_this->arg0 == 0) {
        spC = i_this->lock_model;
        spC->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        spC = i_this->lock_modelMorf->getModel();
        mDoMtx_stack_c::scaleM(l_HIO.obj_size, l_HIO.obj_size, l_HIO.obj_size);
        spC->setBaseTRMtx(mDoMtx_stack_c::get());

        i_this->lock_modelMorf->play(&actor->eyePos, 0, 0);
        i_this->lock_modelMorf->modelCalc();
    }

    actor->eyePos = actor->current.pos;

    cXyz sp1C;
    MTXCopy(mDoMtx_stack_c::get(), *calc_mtx);
    sp34.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp34, &sp1C);

    if (i_this->hit_timer != 0) {
        sp1C.y -= 17000.0f;
    }

    i_this->ccSph.SetC(sp1C);

    if (i_this->arg0 == 0) {
        i_this->ccSph.SetR(40.0f + JREG_F(3));
    } else {
        i_this->ccSph.SetR((90.0f + JREG_F(3)) * l_HIO.obj_size);
    }

    dComIfG_Ccsp()->Set(&i_this->ccSph);

    if (i_this->hit_timer == 0 && i_this->ccSph.ChkCoHit()) {
        i_this->field_0x2ae4 = 1.5f + TREG_F(14);
    }

    if (i_this->ccSph.ChkTgHit()) {
        i_this->hit_timer = 10;
        i_this->atInfo.mpCollider = i_this->ccSph.GetTgHitObj();
        at_power_check(&i_this->atInfo);

        if (i_this->atInfo.mHitType == 2) {
            i_this->field_0x2ae4 = 17.0f + TREG_F(8);
        } else {
            i_this->field_0x2ae4 = 8.0f + TREG_F(4);
        }

        if (i_this->arg0 != 0) {
            i_this->field_0x2ae4 *= 0.5f;
        }

        def_se_set(&i_this->sound, i_this->atInfo.mpCollider, 0x35, NULL);
    }

    if (daPy_getPlayerActorClass() != NULL && daPy_getPlayerActorClass()->checkFrontRollCrash() && fopAcM_searchPlayerDistanceXZ(actor) < 150.0f) {
        i_this->field_0x2ae4 = 7.0f + TREG_F(4);
        if (i_this->arg0 != 0) {
            i_this->field_0x2ae4 *= 0.5f;
        }
    }

    chain_move(i_this);
    i_this->sound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    return 1;
}

static int daObj_Keyhole_IsDelete(obj_keyhole_class* i_this) {
    return 1;
}

static int daObj_Keyhole_Delete(obj_keyhole_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fpc_ProcID id = fopAcM_GetID(actor);

    if (i_this->field_0x2cb4 != 0) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->arcname, i_this->field_0x580);
        dComIfGp_removeSimpleModel(modelData, fopAcM_GetRoomNo(actor));
        i_this->field_0x2cb4 = 0;
    }

    dComIfG_resDelete(&i_this->phase, i_this->arcname);

    if (i_this->HIOInit) {
        hio_set = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (actor->heap != NULL) {
        i_this->sound.stopAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    obj_keyhole_class* a_this = (obj_keyhole_class*)i_this;
    void* modelData;

    static int bmd_d[] = {
        7, 11, 11, 7, 11, 11, 11, 11, 11, 11,
    };

    static u32 mdl_f[] = {
        0x80000, 0, 0x80000, 0, 0, 0, 0, 0, 0, 0,
    };

    static int bck_d[] = {
        0, 5, 5, 0, 5, 5, 5, 5, 5, 5,
    };

    static int cbmd_d[] = {
        4, 8, 8, 4, 8, 8, 8, 8, 8, 8,
    };

    if (a_this->arg0 == 0 || a_this->arg0 == 3) {
        modelData = dComIfG_getObjectRes(a_this->arcname, bmd_d[a_this->arg0]);
        JUT_ASSERT(1189, modelData != NULL);

        a_this->lock_model = mDoExt_J3DModel__create((J3DModelData*)modelData, mdl_f[a_this->arg0], 0x11000084);
        if (a_this->lock_model == NULL) {
            return 0;
        }

        a_this->lock_model->setUserArea((uintptr_t)a_this);

        for (u16 i = 0; i < a_this->lock_model->getModelData()->getJointNum(); i++) {
            if (i == 1) {
                a_this->lock_model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
            }
        }
    } else {
        a_this->lock_modelMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes(a_this->arcname, bmd_d[a_this->arg0]), NULL, NULL, NULL, 0, 1.0f, 0, -1, 1, NULL, mdl_f[a_this->arg0], 0x11000084);
        if (a_this->lock_modelMorf == NULL || a_this->lock_modelMorf->getModel() == NULL) {
            return 0;
        }

        a_this->lock_modelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(a_this->arcname, bck_d[a_this->arg0]), 0, 0.0f, 0.0f, 0.0f, -1.0f, NULL);
    }

    a_this->field_0x580 = cbmd_d[a_this->arg0];
    modelData = dComIfG_getObjectRes(a_this->arcname, a_this->field_0x580);
    JUT_ASSERT(1270, modelData != NULL);

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < a_this->chain_num; j++) {
            a_this->chain_s[i].model[j] = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x20000, 0x11000084);
            if (a_this->chain_s[i].model[j] == NULL) {
                return 0;
            }
        }
    }

    return 1;
}

static int daObj_Keyhole_Create(fopAc_ac_c* a_this) {
    obj_keyhole_class* i_this = (obj_keyhole_class*)a_this;
    fopAcM_ct(a_this, obj_keyhole_class);

    static char* arc_name[] = {
        "Obj_keyh",  "Obj_bkey", "Obj_bkey2", "Obj_bkey3", "Obj_bkey",
        "Obj_bkey5", "Obj_bkey", "Obj_bkey",  "Obj_bkey",  "Obj_bkey",
    };

    i_this->arg0 = fopAcM_GetParam(a_this);

    if (i_this->arg0 == 0xFF) {
        i_this->arg0 = 0;
    }
    if (i_this->arg0 == 0 || i_this->arg0 == 3) {
        i_this->chain_num = 16;
    } else {
        i_this->chain_num = TREG_S(8) + 12;
    }

    i_this->arcname = arc_name[i_this->arg0];
    int phase_state = dComIfG_resLoad(&i_this->phase, i_this->arcname);
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_KEYHOLE PARAM %x\n", fopAcM_GetParam(a_this));
        u8 swbit = (fopAcM_GetParam(a_this) & 0xFF000000) >> 0x18;
        if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(a_this))) {
            i_this->action = 1;
        }

        OS_REPORT("OBJ_KEYHOLE//////////////OBJ_KEYHOLE SET 1 !!\n");

        static u16 const estimateSizeTbl[] = {
            0xBF40, 0xA4E0, 0x9360, 0xD060, 0xA4E0,
            0xA420, 0xA4E0, 0xA4E0, 0xA4E0, 0xA4E0,
        };

        ASSERTLINE(1360, i_this->arg0 < ARRAY_SIZEU(estimateSizeTbl));
        u32 heapsize = estimateSizeTbl[i_this->arg0];
        OS_REPORT("OBJ_KEYHOLE: arg0=%u 0x%04x\n", i_this->arg0, heapsize);

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, heapsize)) {
            OS_REPORT("//////////////OBJ_KEYHOLE SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_KEYHOLE SET 2 !!\n");

        if (i_this->arg0 == 3) {
            i_this->arg0 = 0;
        }

        if (!hio_set) {
            i_this->HIOInit = TRUE;
            hio_set = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("リアル施錠", &l_HIO);
        }

        if (i_this->lock_model != NULL) {
            fopAcM_SetMtx(a_this, i_this->lock_model->getBaseTRMtx());
        } else {
            fopAcM_SetMtx(a_this, i_this->lock_modelMorf->getModel()->getBaseTRMtx());
        }

        fopAcM_SetMin(a_this, -400.0f, -400.0f, -400.0f);
        fopAcM_SetMax(a_this, 400.0f, 400.0f, 400.0f);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdbf, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_METAL, 0x2, 0x0, 0x0, 0x3}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 60.0f} // mSph
            } // mSphAttr
        };

        i_this->ccStts.Init(0xFF, 0, a_this);
        i_this->ccSph.Set(cc_sph_src);
        i_this->ccSph.SetStts(&i_this->ccStts);

        for (int i = 0; i < 6; i++) {
            i_this->chain_s[i].ccStts.Init(100, 0, a_this);
            for (int j = 0; j < 3; j++) {
                i_this->chain_s[i].ccSph[j].Set(cc_sph_src);
                i_this->chain_s[i].ccSph[j].SetStts(&i_this->chain_s[i].ccStts);
            }
        }

        i_this->sound.init(&a_this->current.pos, &a_this->eyePos, 3, 1);

        if (i_this->arg0 != 0) {
            cXyz sp28(a_this->current.pos.x, 100.0f + a_this->current.pos.y, a_this->current.pos.z);
            if (fopAcM_gc_c::gndCheck(&sp28)) {
                a_this->home.pos.y = fopAcM_gc_c::getGroundY();
            }
        }

        i_this->field_0x2ae8 = a_this->home.pos.y;
        a_this->shape_angle.x = 0x4000;

        if (i_this->arg0 == 0) {
            a_this->home.pos.y += 150.0f + BREG_F(5);
            i_this->field_0x2af4 = BREG_F(0) - 15.0f;
            i_this->field_0x2af8 = 40.0f + BREG_F(1);
        } else {
            a_this->home.pos.y += 45.0f + (100.0f + (150.0f + BREG_F(5)));
            i_this->field_0x2af4 = BREG_F(0) - 15.0f;
            i_this->field_0x2af8 = 20.0f + (40.0f + BREG_F(1));
        }

        void* modelData = dComIfG_getObjectRes(i_this->arcname, i_this->field_0x580);
        JUT_ASSERT(1507, modelData != NULL);

        if (dComIfGp_addSimpleModel((J3DModelData*)modelData, fopAcM_GetRoomNo(a_this), 1) == -1) {
            OS_REPORT("\x1B[43;30m施錠鎖：シンプルモデル登録失敗しました。\n\x1B[m");
        }

        i_this->field_0x2cb4 = 1;
        i_this->atInfo.field_0x18 = 53;
        fopAcM_setCullSizeFar(a_this, 1.5f + hREG_F(8));
        daObj_Keyhole_Execute(i_this);
    }

    return phase_state;
}

static actor_method_class l_daObj_Keyhole_Method = {
    (process_method_func)daObj_Keyhole_Create,
    (process_method_func)daObj_Keyhole_Delete,
    (process_method_func)daObj_Keyhole_Execute,
    (process_method_func)daObj_Keyhole_IsDelete,
    (process_method_func)daObj_Keyhole_Draw,
};

actor_process_profile_definition g_profile_OBJ_KEYHOLE = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_OBJ_KEYHOLE,        // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(obj_keyhole_class),              // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    45,                      // mPriority
    &l_daObj_Keyhole_Method, // sub_method
    0x00044100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES
