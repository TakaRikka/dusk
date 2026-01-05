/**
 * @file d_a_e_dd.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_dd.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_bomb.h"
#include "f_op/f_op_actor_enemy.h"

class daE_DD_HIO_c : public JORReflexible {
public:
    daE_DD_HIO_c();
    virtual ~daE_DD_HIO_c() {}

    void genMessage(JORMContext*);

    /* ドドンゴ - Dodongo */
    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 basic_size;      // 基本サイズ - Basic Size
    /* 0x0C */ f32 walk_speed;      // 歩速度 - Walk Speed
    /* 0x10 */ f32 run_speed;       // 走速度 - Run Speed
    /* 0x14 */ f32 search_area;     // サーチエリア - Search Area
    /* 0x18 */ f32 flame_distance;  // 炎距離 - Flame Distance
    /* 0x1C */ f32 horned_head;     // 首曲角 - Horned Head
    /* 0x20 */ s16 flame_time;      // 炎時間 - Flame Time
    /* 0x24 */ f32 flame_mot_speed; // 炎モーション速 - Flame Motion Speed
};

enum E_dd_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_DD_DAMAGE_L = 0x5,
    /* 0x06 */ BCK_DD_DAMAGE_R,
    /* 0x07 */ BCK_DD_DIE,
    /* 0x08 */ BCK_DD_DIE_ARROW,
    /* 0x09 */ BCK_DD_DIE_BOMB,
    /* 0x0A */ BCK_DD_DIE_FALL_A_1,
    /* 0x0B */ BCK_DD_DIE_FALL_A_2,
    /* 0x0C */ BCK_DD_DIE_FALL_B,
    /* 0x0D */ BCK_DD_DIE_FALL_C,
    /* 0x0E */ BCK_DD_FIRE_A,
    /* 0x0F */ BCK_DD_FIRE_B,
    /* 0x10 */ BCK_DD_FIRE_C,
    /* 0x11 */ BCK_DD_GUARD,
    /* 0x12 */ BCK_DD_RUN,
    /* 0x13 */ BCK_DD_TURN_L,
    /* 0x14 */ BCK_DD_TURN_R,
    /* 0x15 */ BCK_DD_WAIT,
    /* 0x16 */ BCK_DD_WALK,

    /* BMDR */
    /* 0x19 */ BMDR_DD = 0x19,

    /* BRK */
    /* 0x1C */ BRK_DD = 0x1C,
    /* 0x1D */ BRK_DD_DEAD,
    /* 0x1E */ BRK_DD_FIRE_A,
    /* 0x1F */ BRK_DD_FIRE_B,
    /* 0x20 */ BRK_DD_FIRE_C,
};

enum Joint {
    /* 0x00 */ JNT_WORLD_ROOT,
    /* 0x01 */ JNT_BACKBONE_1,
    /* 0x02 */ JNT_BACKBONE_2,
    /* 0x03 */ JNT_BACKBONE_3,
    /* 0x04 */ JNT_ARM_L_1,
    /* 0x05 */ JNT_ARM_L_2,
    /* 0x06 */ JNT_HAND_L,
    /* 0x07 */ JNT_ARM_R_1,
    /* 0x08 */ JNT_ARM_R_2,
    /* 0x09 */ JNT_HAND_R,
    /* 0x0A */ JNT_NECK,
    /* 0x0B */ JNT_CHIN,
    /* 0x0C */ JNT_WAIST,
    /* 0x0D */ JNT_LEG_L_1,
    /* 0x0E */ JNT_LEG_L_2,
    /* 0x0F */ JNT_FOOT_L,
    /* 0x10 */ JNT_LEG_R_1,
    /* 0x11 */ JNT_LEG_R_2,
    /* 0x12 */ JNT_FOOT_R,
    /* 0x13 */ JNT_TAIL_1,
    /* 0x14 */ JNT_TAIL_2,
    /* 0x15 */ JNT_TAIL3,
    /* 0x16 */ JNT_TAIL_4,
};

enum Action {
    /* 0x0 */ ACTION_NORMAL,
    /* 0x3 */ ACTION_RUN = 0x3,
    /* 0x4 */ ACTION_ATTACK,
    /* 0x5 */ ACTION_GUARD,
    /* 0x6 */ ACTION_BOMBDAMAGE,
    /* 0x7 */ ACTION_TAILDAMAGE,
    /* 0x8 */ ACTION_ARROWDAMAGE,
    /* 0x9 */ ACTION_DEAD,
    /* 0xA */ ACTION_FALL,
};

daE_DD_HIO_c::daE_DD_HIO_c() {
    field_0x4 = -1;
    basic_size = 1.0f;
    walk_speed = 4.0f;
    run_speed = 10.0f;
    search_area = 800.0f;
    flame_distance = 550.0f;
    horned_head = 30.0f;
    flame_time = 110;
    flame_mot_speed = 1.5f;
}

static void anm_init(e_dd_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_DD", i_index), i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->mAnm = i_index;
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        int jntNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_dd_class* i_this = (e_dd_class*)model->getUserArea();
        if (i_this != 0) {
            if (jntNo == JNT_NECK) {
                MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
                cMtx_ZrotM(*calc_mtx, i_this->field_0x6b8 + i_this->field_0x6bc);
                model->setAnmMtx(jntNo, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            } else if (jntNo == JNT_BACKBONE_2 || jntNo == JNT_BACKBONE_3) {
                MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
                cMtx_YrotM(*calc_mtx, i_this->field_0x6bc);
                model->setAnmMtx(jntNo, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            } else if (jntNo == JNT_TAIL_1 || jntNo == JNT_TAIL_2 || jntNo == JNT_TAIL3) {
                MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
                cMtx_YrotM(*calc_mtx, -i_this->field_0x6bc);
                model->setAnmMtx(jntNo, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static int daE_DD_Draw(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    J3DModel* model = i_this->mpModelMorf->getModel();

    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);
    i_this->mpBrkAnms[i_this->field_0x67c]->entry(model->getModelData());
    i_this->mpModelMorf->entryDL();

    if (i_this->mShadowKey != 0x564FF) {
        cXyz sp28;
        sp28.set(a_this->current.pos.x, a_this->current.pos.y + 50.0f + BREG_F(18), a_this->current.pos.z);
        i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &sp28, 1200.0f, 0.0f, a_this->current.pos.y, i_this->mObjAcch.GetGroundH(),
                                                i_this->mObjAcch.m_gnd, &a_this->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static BOOL otherBgCheckS(fopAc_ac_c* param_1, fopAc_ac_c* param_2) {
    dBgS_LinChk lin_chk;
    cXyz start, end;
    
    end = param_2->current.pos;
    end.y += 100.0f;
    start = param_1->current.pos;
    start.y += 50.0f;
    lin_chk.Set(&start, &end, param_1);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return TRUE;
    }

    return FALSE;
}

static BOOL pl_check(e_dd_class* i_this, f32 param_2, s16 param_3) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (i_this->field_0x6a4 < param_2) {
        s16 sVar1 = (a_this->shape_angle.y - i_this->field_0x6a0);
        
        if (sVar1 < param_3 && sVar1 > (s16)-param_3 && !otherBgCheckS(a_this, player)) {
            return TRUE;
        }
    }

    return FALSE;
}

static BOOL way_gake_check(e_dd_class* i_this, f32 param_1) {
    fopAc_ac_c* a_this = &i_this->actor;
    dBgS_GndChk gnd_chk;
    cXyz sp78, sp84;

    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    sp78.x = 0.0f;
    sp78.y = 100.0f;
    sp78.z = param_1;
    MtxPosition(&sp78, &sp84);
    sp84 += a_this->current.pos;
    gnd_chk.SetPos(&sp84);

    if (a_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 300.0f) {
        i_this->field_0x6d0 = 1;
        return TRUE;
    }

    i_this->field_0x6d0 = 0;
    return FALSE;
}

static void damage_check(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    i_this->mStts.Move();

    if (i_this->field_0x6b2 == 0) {
        for (int i = 0; i < 2; i++) {
            if (i_this->field_0xa74[i].ChkTgHit() && i_this->field_0xa74[i].ChkTgShield()) {
                i_this->field_0x6b2 = 6;
                i_this->mAtInfo.mpCollider = i_this->field_0xa74[i].GetTgHitObj();
                def_se_set(&i_this->mSound, i_this->mAtInfo.mpCollider, 42, NULL);
                break;
            }
        }

        if (i_this->field_0xe5a != 0 && i_this->field_0xa74[0].ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->field_0xa74[0].GetTgHitObj();

            if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_ARROW)) {
                i_this->field_0x6b2 = 6;
                i_this->mAction = 8;
                i_this->field_0x68c = 0;
            }
        }

        if (i_this->field_0xa74[2].ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->field_0xa74[2].GetTgHitObj();
            at_power_check(&i_this->mAtInfo);

            if (i_this->mAtInfo.mHitType == HIT_TYPE_STUN) {
                if (i_this->field_0x6aa[2] == 0) {
                    i_this->mAction = 7;
                    i_this->field_0x68c = 0;
                    i_this->field_0x6aa[2] = 90;
                }

                i_this->mSound.startCreatureVoice(Z2SE_EN_DD_V_GUARD, -1);
            } else {
                cc_at_check(a_this, &i_this->mAtInfo);

                s8 bVar1 = 0;
                if (i_this->field_0x6d1 != 0) {
                    i_this->field_0x6d1 = 0;
                    bVar1 = 1;
                }

                if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP && player->checkCutJumpCancelTurn()) {
                    i_this->field_0x6b2 = 3;
                    i_this->field_0x6d1 = 1;
                } else {
                    i_this->field_0x6b2 = 10;
                }

                if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_ARROW)) {
                    i_this->field_0xe5b++;

                    if (i_this->field_0xe5b >= 3) {
                        a_this->health = 0;
                    }
                }

                if (bVar1 || (i_this->field_0x6d4 == 0 && a_this->health <= 0)) {
                    i_this->mAction = 9;
                    i_this->field_0x68c = 0;
                    a_this->health = 0;
                } else if (i_this->field_0x6aa[2] == 0) {
                    i_this->mAction = 7;
                    i_this->field_0x68c = 0;
                    i_this->field_0x6aa[2] = 90;
                }

                if (a_this->health <= 1) {
                    a_this->health = 0;
                    i_this->field_0xa74[2].SetTgHitMark(CcG_Tg_UNK_MARK_3);
                }
            }
        } else {
            for (int i = 0; i < 2; i++) {
                if (i_this->field_0xa74[i].ChkTgHit()) {
                    i_this->mAtInfo.mpCollider = i_this->field_0xa74[i].GetTgHitObj();
                    at_power_check(&i_this->mAtInfo);

                    if (i_this->mAtInfo.mHitType == HIT_TYPE_STUN) {
                        i_this->mAction = 5;
                        i_this->field_0x68c = 0;
                        i_this->mSound.startCreatureVoice(Z2SE_EN_DD_V_GUARD, -1);
                    }

                    i_this->field_0x6b2 = 6;
                }
            }
        }
    }
}

static BOOL way_bg_check(e_dd_class* i_this, f32 param_2) {
    fopAc_ac_c* a_this = &i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz sp98, start, end;

    start = a_this->current.pos;
    start.y += 50.0f;
    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    sp98.x = 0.0f;
    sp98.y = 50.0f;
    sp98.z = param_2;
    MtxPosition(&sp98, &end);
    end += a_this->current.pos;
    lin_chk.Set(&start, &end, a_this);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return TRUE;
    }

    return FALSE;
}

u8 hio_set;

static daE_DD_HIO_c l_HIO;

static BOOL path_check(e_dd_class* i_this) {
    static u8 check_index[255];
    fopAc_ac_c* a_this = &i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz start, end;

    start = a_this->current.pos;
    start.y += WREG_F(0) + 10.0f;
    dPnt* points = i_this->mpPath->m_points;

    for (int i = 0; i < i_this->mpPath->m_num; i++, points++) {
        if (i < 0xFF) {
            end.x = points->m_position.x;
            end.y = points->m_position.y + 10.0f + WREG_F(1);
            end.z = points->m_position.z;
            lin_chk.Set(&start, &end, a_this);

            if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
                check_index[i] = 1;
            } else {
                check_index[i] = 0;
            }
        }
    }

    f32 fVar2, fVar3, fVar4;
    f32 fVar1 = 100.0f;
    bool bVar1 = false;

    for (int i = 0; i < 100; i++) {
        points = i_this->mpPath->m_points;

        for (int j = 0; j < i_this->mpPath->m_num; j++, points++) {
            if (j < 0xFF && check_index[j] != 0) {
                fVar2 = a_this->current.pos.x - points->m_position.x;
                fVar3 = a_this->current.pos.y - points->m_position.y;
                fVar4 = a_this->current.pos.z - points->m_position.z;
                if (JMAFastSqrt(fVar2 * fVar2 + fVar3 * fVar3 + fVar4 * fVar4) < fVar1) {
                    i_this->field_0x5bc = j;
                    bVar1 = 1;
                    break;
                }
            }
        }

        if (bVar1) break;
        fVar1 += 50.0f;
    }

    if (!bVar1) {
        return FALSE;
    }

    if (cM_rndF(1.0f) < 0.5f) {
        i_this->field_0x5bd = 1;
    } else {
        i_this->field_0x5bd = -1;
    }

    return TRUE;
}

static void e_dd_wall(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    int frame = i_this->mpModelMorf->getFrame();
    f32 fVar1 = 0.0f;
    cXyz sp40, sp4c;

    switch (i_this->field_0x68c) {
        case 0:
            i_this->field_0x6a8 = 0;

            if (i_this->field_0x6aa[0] == 0) {
                if (i_this->mpPath != NULL && path_check(i_this)) {
                    i_this->field_0x68c = 100;
                } else {
                    sp4c.x = a_this->home.pos.x + cM_rndFX(200.0f);
                    sp4c.y = a_this->home.pos.y;
                    sp4c.z = a_this->home.pos.z + cM_rndFX(200.0f);
                    sp40 = sp4c - a_this->current.pos;
                    cMtx_XrotS(*calc_mtx, -i_this->field_0x6d6.x);
                    cMtx_YrotM(*calc_mtx, -i_this->field_0x6d6.y);
                    MtxPosition(&sp40, &sp4c);
                    i_this->field_0x69c = cM_atan2s(sp4c.x, sp4c.z);
                    anm_init(i_this, BCK_DD_WALK, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                    i_this->field_0x6aa[0] = cM_rndF(50.0f) + 50.0f;
                    i_this->field_0x68c = 1;
                }
            }
            break;

        case 1:
            if ((frame >= AREG_S(0) + 12 && frame <= AREG_S(1) + 29) || frame <= AREG_S(2) + 2 || frame >= AREG_S(3) + 37) {
                fVar1 = l_HIO.walk_speed;
            }

            if (i_this->field_0x6aa[0] == 0 && (frame == 7 || frame == 33)) {
                i_this->field_0x68c = 0;
                i_this->field_0x6aa[0] = cM_rndF(100.0f) + 100.0f;
                anm_init(i_this, BCK_DD_WAIT, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;

        case 100: {
            anm_init(i_this, BCK_DD_WALK, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->field_0x68c = 101;
            dPnt* points = i_this->mpPath->m_points;
            s8 sVar1 = i_this->field_0x5bc;
            i_this->field_0x690.x = points[sVar1].m_position.x;
            i_this->field_0x690.y = points[sVar1].m_position.y;
            i_this->field_0x690.z = points[sVar1].m_position.z;
        }
            // fallthrough
        case 101:
            if ((frame >= AREG_S(0) + 12 && frame <= AREG_S(1) + 29) || frame <= AREG_S(2) + 2 || frame >= AREG_S(3) + 37) {
                fVar1 = l_HIO.walk_speed;
            }

            sp40 = i_this->field_0x690 - a_this->current.pos;
            i_this->field_0x69c = cM_atan2s(sp40.x, sp40.z);

            if ((frame == 7 || frame == 33) && sp40.abs() < 100.0f) {
                i_this->field_0x5bc += i_this->field_0x5bd;
                if (i_this->field_0x5bc >= (i_this->mpPath->m_num & 0xFF)) {
                    if (dPath_ChkClose(i_this->mpPath)) {
                        i_this->field_0x5bc = 0;
                    } else {
                        i_this->field_0x5bd = -1;
                        i_this->field_0x5bc = i_this->mpPath->m_num - 2;
                    }
                } else if (i_this->field_0x5bc < 0) {
                    i_this->field_0x5bd = 1;
                    i_this->field_0x5bc = 1;
                }

                i_this->field_0x68c = 102;
                i_this->field_0x6aa[0] = cM_rndF(100.0f) + 100.0f;
                anm_init(i_this, BCK_DD_WAIT, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;

        case 102:
            if (i_this->field_0x6aa[0] == 0) {
                i_this->field_0x68c = 100;
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, l_HIO.walk_speed * 0.5f);
    i_this->field_0x6a8 = a_this->speedF * (BREG_F(1) + 64.0f);

    if (i_this->mAnm == BCK_DD_WALK) {
        if (a_this->speedF >= 0.5f) {
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x69c, 8, i_this->field_0x6a8);
        }

        s16 sVar2 = i_this->field_0x69c - a_this->current.angle.y;

        if (sVar2 > 0x800 || sVar2 < -0x800) {
            if (sVar2 > 0) {
                i_this->field_0x6be = BREG_S(6) + 2560;
            } else {
                i_this->field_0x6be = -(BREG_S(6) + 2560);
            }

            i_this->field_0x6c0 = BREG_S(1) + 128;
        }
    }

    if (pl_check(i_this, l_HIO.search_area, 0x7FFF)) {
        i_this->mAction = 4;
        i_this->field_0x68c = 0;
        anm_init(i_this, BCK_DD_RUN, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
    }
}

static void e_dd_normal(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    if (i_this->field_0x6d4 != 0) {
        e_dd_wall(i_this);
        return;
    }

    int frame = i_this->mpModelMorf->getFrame();
    f32 fVar1 = 0.0f;
    cXyz sp40, sp4c;
    s16 sVar1;

    switch (i_this->field_0x68c) {
        case 0:
            i_this->field_0x6a8 = 0;

            if (i_this->field_0x6aa[0] == 0) {
                if (i_this->mpPath != NULL && path_check(i_this)) {
                    i_this->field_0x68c = 100;
                } else {
                    if (way_bg_check(i_this, 200.0f) || i_this->field_0x6d0 != 0) {
                        sVar1 = cM_rndFX(10000.0f) + 32768.0f;
                        i_this->field_0x6aa[1] = 200;
                    } else {
                        sp4c.x = a_this->home.pos.x + cM_rndFX(400.0f);
                        sp4c.y = a_this->home.pos.y;
                        sp4c.z = a_this->home.pos.z + cM_rndFX(400.0f);
                        sp40 = sp4c - a_this->current.pos;
                        sVar1 = cM_atan2s(sp40.x, sp40.z) - a_this->current.angle.y;
                    }

                    i_this->field_0x69c = a_this->current.angle.y + sVar1;
                    anm_init(i_this, BCK_DD_WALK, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                    i_this->field_0x6aa[0] = cM_rndF(100.0f) + 100.0f;
                    i_this->field_0x68c = 1;
                }
            }
            break;

        case 1:
            if ((frame >= AREG_S(0) + 12 && frame <= AREG_S(1) + 29) || frame <= AREG_S(2) + 2 || frame >= AREG_S(3) + 37) {
                fVar1 = l_HIO.walk_speed;
            }

            if ((i_this->field_0x6aa[0] == 0 && (frame == 7 || frame == 33)) || (i_this->field_0x6aa[1] == 0 && (way_bg_check(i_this, 200.0f) || i_this->field_0x6d0 != 0))) {
                i_this->field_0x68c = 0;
                i_this->field_0x6aa[0] = cM_rndF(100.0f) + 100.0f;
                anm_init(i_this, BCK_DD_WAIT, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;

        case 100: {
            anm_init(i_this, BCK_DD_WALK, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->field_0x68c = 101;
            dPnt* points = i_this->mpPath->m_points;
            s8 sVar2 = i_this->field_0x5bc;
            i_this->field_0x690.x = points[sVar2].m_position.x;
            i_this->field_0x690.y = points[sVar2].m_position.y;
            i_this->field_0x690.z = points[sVar2].m_position.z;
        }
            // fallthrough
        case 101:
            if ((frame >= AREG_S(0) + 12 && frame <= AREG_S(1) + 29) || frame <= AREG_S(2) + 2 || frame >= AREG_S(3) + 37) {
                fVar1 = l_HIO.walk_speed;
            }

            sp40 = i_this->field_0x690 - a_this->current.pos;
            i_this->field_0x69c = cM_atan2s(sp40.x, sp40.z);

            if ((frame == 7 || frame == 33) && sp40.abs() < 100.0f) {
                i_this->field_0x5bc += i_this->field_0x5bd;
                if (i_this->field_0x5bc >= (i_this->mpPath->m_num & 0xFF)) {
                    if (dPath_ChkClose(i_this->mpPath)) {
                        i_this->field_0x5bc = 0;
                    } else {
                        i_this->field_0x5bd = -1;
                        i_this->field_0x5bc = i_this->mpPath->m_num - 2;
                    }
                } else if (i_this->field_0x5bc < 0) {
                    i_this->field_0x5bd = 1;
                    i_this->field_0x5bc = 1;
                }

                i_this->field_0x68c = 102;
                i_this->field_0x6aa[0] = cM_rndF(100.0f) + 100.0f;
                anm_init(i_this, BCK_DD_WAIT, 15.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;

        case 102:
            if (i_this->field_0x6aa[0] == 0) {
                i_this->field_0x68c = 100;
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, l_HIO.walk_speed * 0.5f);
    i_this->field_0x6a8 = a_this->speedF * (BREG_F(1) + 64.0f);

    if (i_this->mAnm == BCK_DD_WALK) {
        if (a_this->speedF >= 0.5f) {
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x69c, 8, i_this->field_0x6a8);
        }

        s16 sVar3 = i_this->field_0x69c - a_this->current.angle.y;

        if (sVar3 > 0x800 || sVar3 < -0x800) {
            if (sVar3 > 0) {
                i_this->field_0x6be = BREG_S(6) + 2560;
            } else {
                i_this->field_0x6be = -(BREG_S(6) + 2560);
            }

            i_this->field_0x6c0 = BREG_S(1) + 128;
        }
    }

    if (pl_check(i_this, l_HIO.search_area, 0x7FFF) && i_this->field_0x6d0 == 0) {
        i_this->mAction = 3;
        i_this->field_0x68c = 0;
    }
}

static void e_dd_run(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    if (i_this->field_0x6d4 != 0) {
        i_this->mAction = 0;
        i_this->field_0x68c = 0;
        i_this->field_0x6aa[0] = 0;
        a_this->speedF = 0.0f;
    } else {
        int frame = i_this->mpModelMorf->getFrame();
        f32 fVar1 = 0.0f;
        cXyz sp48, sp54;

        switch (i_this->field_0x68c) {
            case 0:
                anm_init(i_this, BCK_DD_RUN, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->field_0x68c = 1;

                if (!pl_check(i_this, 10000.0f, 0x2000)) {
                    i_this->field_0x6aa[1] = JREG_S(7) + 25;
                    i_this->mpModelMorf->setFrame(JREG_F(7) + 12.0f);
                }

                i_this->mSound.startCreatureVoice(Z2SE_EN_DD_V_FIND, -1);
                break;

            case 1:
                fVar1 = l_HIO.run_speed;

                if (i_this->field_0x6a4 < l_HIO.flame_distance) {
                    i_this->mAction = 4;
                    i_this->field_0x68c = 0;
                    i_this->field_0x6aa[0] = 40;
                }
                break;
        }

        if (i_this->field_0x6aa[1] != 0 && i_this->field_0x6aa[1] < JREG_S(6) + 20) {
            fVar1 = 0.0f;

            if (frame == 3) {
                i_this->mpModelMorf->setFrame(2.0f);
            }
        }

        cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, l_HIO.run_speed * 0.5f);
        i_this->field_0x6a8 = a_this->speedF * (BREG_F(1) + 100.0f);

        if (a_this->speedF >= 0.5f) {
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x6a0, 1, i_this->field_0x6a8);
        }

        s16 sVar1 = i_this->field_0x6a0 - a_this->current.angle.y;
        if (sVar1 > 0x1800 || sVar1 < -0x1800) {
            if (sVar1 > 0) {
                i_this->field_0x6be = BREG_S(6) + 0xD00;
            } else {
                i_this->field_0x6be = -(BREG_S(6) + 0xD00);
            }

            i_this->field_0x6c0 = BREG_S(1) + 0x200;
        } else {
            i_this->field_0x6c0 = 0x180;
        }

        if (i_this->field_0x6d0 != 0 || !pl_check(i_this, l_HIO.search_area + 300.0f, 0x7FFF)) {
            i_this->mAction = 0;
            i_this->field_0x68c = 0;
            i_this->field_0x6aa[0] = 0;
            a_this->speedF = 0.0f;
        }
    }
}

static void* s_b_sub(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && dBomb_c::checkBombActor((fopAc_ac_c*)i_actor) && !((dBomb_c*)i_actor)->checkStateExplode()) {
        cXyz sp2c(((e_dd_class*)i_data)->field_0xe40);
        sp2c.y -= KREG_F(11) + 60.0f;
        f32 fVar1 = (sp2c - ((fopAc_ac_c*)i_actor)->current.pos).abs();

        if (fVar1 < NREG_F(6) + 200.0f) {
            cLib_addCalc2(&((fopAc_ac_c*)i_actor)->current.pos.x, sp2c.x, 1.0f, ((e_dd_class*)i_data)->field_0x6c4);
            cLib_addCalc2(&((fopAc_ac_c*)i_actor)->current.pos.y, sp2c.y, 1.0f, ((e_dd_class*)i_data)->field_0x6c4);
            cLib_addCalc2(&((fopAc_ac_c*)i_actor)->current.pos.z, sp2c.z, 1.0f, ((e_dd_class*)i_data)->field_0x6c4);
            cLib_addCalc2(&((e_dd_class*)i_data)->field_0x6c4, 50.0f, 1.0f, 5.0f);
            cLib_addCalc0(&((fopAc_ac_c*)i_actor)->speedF, 1.0f, 3.0f);

            ((fopAc_ac_c*)i_actor)->speed.y = 0.0f;

            if (fVar1 < NREG_F(6) + 40.0f) {
                fopAcM_delete((fopAc_ac_c*)i_actor);
                return i_actor;
            }
        }
    }

    return NULL;
}

static void e_dd_attack(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    i_this->field_0x6c0 = 0x100;
    int frame = i_this->mpModelMorf->getFrame();
    s8 bVar1 = 0;

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);

    switch (i_this->field_0x68c) {
        case 0: {
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x6a0, 2, 1000);
            s16 sVar1 = i_this->field_0x6a0 - a_this->current.angle.y;

            if (i_this->field_0x6aa[0] == 0 ||
                /* This was likely intended to be a logical 'and', but it seems one '&' was mistakenly left out, causing a bitwise 'and' */
                (sVar1 <= 0x200 & sVar1 >= -0x200)) {
                i_this->field_0x68c = 1;
            } else {
                if (sVar1 > 0) {
                    i_this->field_0x6be = BREG_S(6) + 0xD00;
                } else {
                    i_this->field_0x6be = -(BREG_S(6) + 0xD00);
                }

                i_this->field_0x6c0 = BREG_S(1) + 0x200;
            }
            break;
        }

        case 1:
            anm_init(i_this, BCK_DD_FIRE_A, 5.0f, J3DFrameCtrl::EMode_NONE, l_HIO.flame_mot_speed);
            i_this->mSound.startCreatureVoice(Z2SE_EN_DD_V_FIRE_A, -1);
            i_this->field_0x67c = 1;
            i_this->mpBrkAnms[1]->setFrame(0.0f);
            i_this->field_0x68c = 2;
            i_this->field_0x6c4 = 20.0f;
            break;

        case 2:
            if (frame > 20) {
                i_this->field_0xa74[0].OffCoSPrmBit(0x40);

                if (fpcM_Search(s_b_sub, i_this) != NULL) {
                    i_this->mAction = 6;
                    i_this->field_0x68c = 0;
                    i_this->field_0x6b2 = 20;
                    return;
                }
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_DD_FIRE_B, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->field_0x67c = 2;
                i_this->mpBrkAnms[2]->setFrame(0.0f);
                i_this->field_0x68c = 3;
                i_this->field_0x6aa[0] = l_HIO.flame_time;
            }
            break;

        case 3:
            bVar1 = 1;

            if (i_this->field_0x6aa[0] == 0 || !pl_check(i_this, 10000.0f, 0x2000)) {
                anm_init(i_this, BCK_DD_FIRE_C, 5.0f, J3DFrameCtrl::EMode_NONE, l_HIO.flame_mot_speed);
                i_this->field_0x6aa[0] = 0;
                i_this->field_0x67c = 3;
                i_this->mpBrkAnms[3]->setFrame(0.0f);
                i_this->field_0x68c = 4;
            }
            break;

        case 4:
            bVar1 = 1;

            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = 0;
                i_this->field_0x68c = 0;
                i_this->field_0x67c = 0;
                i_this->mpBrkAnms[0]->setFrame(0.0f);
            }
            break;
    }

    if (bVar1 != 0 && pl_check(i_this, 1000.0f, 0x3800)) {
        i_this->field_0x6ba = (BREG_S(7) + -1) * (a_this->shape_angle.y - i_this->field_0x6a0);
    }
}

static void e_dd_guard(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);
    i_this->field_0x6b2 = 5;

    switch (i_this->field_0x68c) {
        case 0:
            anm_init(i_this, BCK_DD_GUARD, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->field_0x68c = 1;
            break;
        
        case 1:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = 3;
                i_this->field_0x68c = 0;
            }
            break;
    }
}

static void e_dd_bombdamage(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp2c;
    int frame = i_this->mpModelMorf->getFrame();

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);
    i_this->field_0x6b2 = 5;

    switch (i_this->field_0x68c) {
        case 0:
            anm_init(i_this, BCK_DD_DIE_BOMB, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->field_0x68c = 1;
            i_this->mSound.startCreatureVoice(Z2SE_EN_DD_V_DIE_BOMB, -1);
            break;

        case 1:
            if (frame == 50) {
                i_this->mSound.startCreatureSound(Z2SE_EN_DD_DIE_BOMB, 0, -1);
            }

            if (frame == 65) {
                i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
            }

            if (i_this->field_0x6d4 != 0 && frame == 56) {
                i_this->mAction = 10;
                i_this->field_0x68c = 0;
                anm_init(i_this, BCK_DD_DIE_FALL_B, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);

                if (i_this->field_0x6d4 != 5) {
                    cMtx_YrotS(*calc_mtx, i_this->field_0x6d6.y);
                    cMtx_XrotM(*calc_mtx, i_this->field_0x6d6.x);
                    sp2c.x = 0.0f;
                    sp2c.y = KREG_F(11) + 10.0f;
                    sp2c.z = 0.0f;
                    MtxPosition(&sp2c, &i_this->field_0x6dc);
                }

                i_this->field_0x6d4 = 0;
                i_this->mShadowKey = 0;
            } else if (i_this->mpModelMorf->isStop()) {
                i_this->field_0x68c = 2;
                i_this->field_0x6aa[0] = 40;
                i_this->field_0x67c = 4;
                i_this->mpBrkAnms[4]->setFrame(0.0f);
            }
            break;

        case 2:
            if (i_this->field_0x6aa[0] == 0) {
                i_this->field_0x5b6 = 1;
            }
            break;
    }
}

static void e_dd_taildamage(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp24;

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);
    s16 sVar1 = i_this->field_0x6a0 - a_this->shape_angle.y;

    switch (i_this->field_0x68c) {
        case 0:
            if (i_this->field_0x6d4 != 0 && a_this->health <= 0) {
                i_this->field_0x6b2 = 100;
                i_this->field_0x68c = 5;

                if (i_this->field_0x6d6.x == -0x8000) {
                    anm_init(i_this, BCK_DD_DIE_FALL_A_1, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                } else {
                    anm_init(i_this, BCK_DD_DIE_FALL_A_2, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                }
            } else {
                if (i_this->field_0x6d3 == 0) {
                    if (sVar1 > 0) {
                        anm_init(i_this, BCK_DD_DAMAGE_R, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                        i_this->field_0x6d2 = 0;
                    } else {
                        anm_init(i_this, BCK_DD_DAMAGE_L, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                        i_this->field_0x6d2 = 1;
                    }

                    i_this->field_0x6d3 = 20;
                } else {
                    if ((i_this->field_0x6d2 & 1) != 0) {
                        anm_init(i_this, BCK_DD_DAMAGE_R, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                    } else {
                        anm_init(i_this, BCK_DD_DAMAGE_L, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                    }
                    
                    i_this->field_0x6d2++;
                }

                i_this->field_0x68c = 1;
            }
            break;
        
        case 1:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = 3;
                i_this->field_0x68c = 0;
            }
            break;

        case 5:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = 10;
                i_this->field_0x68c = 0;
                anm_init(i_this, BCK_DD_DIE_FALL_B, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);

                if (i_this->field_0x6d4 != 5) {
                    cMtx_YrotS(*calc_mtx, i_this->field_0x6d6.y);
                    cMtx_XrotM(*calc_mtx, i_this->field_0x6d6.x);
                    sp24.x = 0.0f;
                    sp24.y = KREG_F(11) + 10.0f;
                    sp24.z = 0.0f;
                    MtxPosition(&sp24, &i_this->field_0x6dc);
                }

                i_this->field_0x6d4 = 0;
                i_this->mShadowKey = 0;
                i_this->mSound.startCreatureSound(Z2SE_EN_DD_FALL, 0, -1);
            }
            break;
    }
}

static void e_dd_fall(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    i_this->field_0x6b2 = 100;

    switch (i_this->field_0x68c) {
        case 0:
            cLib_addCalcAngleS2(&i_this->field_0x6d6.x, -0x8000, 2, 0x800);

            if (i_this->mObjAcch.ChkGroundHit()) {
                i_this->field_0x68c = 2;
                i_this->field_0x6d6.x = -0x8000;
                anm_init(i_this, BCK_DD_DIE_FALL_C, 0.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->field_0x6dc.z = 0.0f;
                i_this->field_0x6dc.x = 0.0f;
                fopAcM_effSmokeSet1(&i_this->field_0xe60, &i_this->field_0xe64, &a_this->current.pos, NULL, 1.5f, &a_this->tevStr, 1);
                i_this->field_0x6aa[0] = 50;
                i_this->field_0x67c = 4;
                i_this->mpBrkAnms[4]->setFrame(0.0f);
                fopAcM_seStartCurrent(a_this, Z2SE_CM_BODYFALL_M, 0);
            }
            break;

        case 1:
            break;

        case 2:
            if (i_this->field_0x6aa[0] == 0) {
                i_this->field_0x5b6 = 1;
            }
            break;
    }

    a_this->current.pos.x += i_this->field_0x6dc.x;
    a_this->current.pos.z += i_this->field_0x6dc.z;
}

static void e_dd_dead(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);
    i_this->field_0x6b2 = 5;

    switch (i_this->field_0x68c) {
        case 0:
            anm_init(i_this, BCK_DD_DIE, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->field_0x68c = 1;
            break;

        case 1:
            if (i_this->mpModelMorf->isStop()) {
                i_this->field_0x68c = 2;
                i_this->field_0x6aa[0] = 40;
                i_this->field_0x67c = 4;
                i_this->mpBrkAnms[4]->setFrame(0.0f);
            }
            break;

        case 2:
            if (i_this->field_0x6aa[0] == 0) {
                i_this->field_0x5b6 = 1;
            }
            break;
    }
}

static void e_dd_arrowdamage(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp24;
    int frame = i_this->mpModelMorf->getFrame();

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);
    i_this->field_0x6b2 = 5;

    switch (i_this->field_0x68c) {
        case 0:
            anm_init(i_this, BCK_DD_DIE_ARROW, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->field_0x68c = 1;
            i_this->mSound.startCreatureVoice(Z2SE_EN_DD_V_DIE_ARROW, -1);
            break;

        case 1:
            if (frame == 91) {
                fopAcM_seStartCurrent(a_this, Z2SE_CM_BODYFALL_S, 0);
            }

            if (i_this->field_0x6d4 != 0 && frame == 96) {
                i_this->mAction = 10;
                i_this->field_0x68c = 0;
                anm_init(i_this, BCK_DD_DIE_FALL_B, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);

                if (i_this->field_0x6d4 != 5) {
                    cMtx_YrotS(*calc_mtx, i_this->field_0x6d6.y);
                    cMtx_XrotM(*calc_mtx, i_this->field_0x6d6.x);
                    sp24.x = 0.0f;
                    sp24.y = KREG_F(11) + 10.0f;
                    sp24.z = 0.0f;
                    MtxPosition(&sp24, &i_this->field_0x6dc);
                }

                i_this->field_0x6d4 = 0;
                i_this->mShadowKey = 0;
            } else if (i_this->mpModelMorf->isStop()) {
                i_this->field_0x68c = 2;
                i_this->field_0x6aa[0] = 40;
                i_this->field_0x67c = 4;
                i_this->mpBrkAnms[4]->setFrame(0.0f);
            }
            break;

        case 2:
            if (i_this->field_0x6aa[0] == 0) {
                i_this->field_0x5b6 = 1;
            }
            break;
    }
}

static void action(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz spa4, spb0;

    if (i_this->field_0x6d4 != 0) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cMtx_XrotS(*calc_mtx, -i_this->field_0x6d6.x);
        cMtx_YrotM(*calc_mtx, -i_this->field_0x6d6.y);
        spa4 = player->current.pos - a_this->current.pos;
        MtxPosition(&spa4, &spb0);
        i_this->field_0x6a0 = cM_atan2s(spb0.x, spb0.z);
    } else {
        i_this->field_0x6a0 = fopAcM_searchPlayerAngleY(a_this);
    }

    i_this->field_0x6a4 = fopAcM_searchPlayerDistance(a_this);
    damage_check(i_this);
    i_this->field_0xe5a = 0;
    i_this->field_0x6be = 0;
    i_this->field_0x6ba = 0;
    i_this->field_0x6c0 = 64;

    s8 bVar1 = 1;
    s8 bVar2 = 0;
    s8 bVar3 = 0;

    switch (i_this->mAction) {
        case ACTION_NORMAL:
            e_dd_normal(i_this);
            bVar3 = 1;
            break;

        case ACTION_RUN:
            e_dd_run(i_this);
            bVar3 = 1;
            break;

        case ACTION_ATTACK:
            e_dd_attack(i_this);
            bVar2 = 1;
            bVar3 = 1;
            break;

        case ACTION_GUARD:
            e_dd_guard(i_this);
            bVar2 = 1;
            bVar3 = 1;
            break;

        case ACTION_BOMBDAMAGE:
            e_dd_bombdamage(i_this);
            bVar1 = 0;
            break;

        case ACTION_TAILDAMAGE:
            e_dd_taildamage(i_this);
            break;

        case ACTION_ARROWDAMAGE:
            e_dd_arrowdamage(i_this);
            bVar1 = 0;
            break;

        case ACTION_DEAD:
            e_dd_dead(i_this);
            bVar1 = 0;
            break;

        case ACTION_FALL:
            e_dd_fall(i_this);
            bVar1 = 0;
            break;
    }

    if (bVar2 != 0) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    if (bVar1 != 0) {
        fopAcM_OnStatus(a_this, 0);
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    }

    if (i_this->field_0x6d4 != 0) {
        cMtx_YrotS(*calc_mtx, i_this->field_0x6d6.y);
        cMtx_XrotM(*calc_mtx, i_this->field_0x6d6.x);
        cMtx_YrotM(*calc_mtx, a_this->current.angle.y);
        spa4.x = 0.0f;
        spa4.y = 0.0f;
        spa4.z = a_this->speedF * l_HIO.basic_size;
        MtxPosition(&spa4, &a_this->speed);
        a_this->current.pos += a_this->speed;
    } else {
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        spa4.x = 0.0f;
        spa4.y = 0.0f;
        spa4.z = a_this->speedF * l_HIO.basic_size;

        if (i_this->field_0x6d0 != 0) {
            spa4.z = 0.0f;
        }

        MtxPosition(&spa4, &spb0);
        a_this->speed.x = spb0.x;
        a_this->speed.z = spb0.z;
        a_this->current.pos += a_this->speed;
        a_this->speed.y -= 5.0f;

        if (a_this->speed.y < -100.0f) {
            a_this->speed.y = -100.0f;
        }

        cXyz* ccMoveP = i_this->mStts.GetCCMoveP();
        if (ccMoveP != NULL) {
            a_this->current.pos.x += ccMoveP->x;
            a_this->current.pos.y += ccMoveP->y;
            a_this->current.pos.z += ccMoveP->z;
        }

        f32 fVar1 = 0.0f;
        if (i_this->field_0x6d6.x != 0) {
            fVar1 = (JREG_F(7) + 162.0f) * l_HIO.basic_size;
        }

        if (bVar3 != 0) {
            way_gake_check(i_this, 200.0f);
        } else {
            i_this->field_0x6d0 = 0;
        }

        a_this->current.pos.y -= fVar1;
        a_this->old.pos.y -= fVar1;

        i_this->mObjAcch.CrrPos(dComIfG_Bgsp());
        
        a_this->current.pos.y += fVar1;
        a_this->old.pos.y += fVar1;

        dBgS_ObjGndChk_Spl gnd_chk_spl;

        spa4 = a_this->current.pos;
        spa4.y += 150.0f;
        gnd_chk_spl.SetPos(&spa4);
        fVar1 = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);

        if (a_this->current.pos.y < fVar1) {
            a_this->current.pos.y = fVar1;
            i_this->field_0x5b6 = 1;
        }
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&i_this->field_0x6bc, i_this->field_0x6be, 2, i_this->field_0x6c0);
    s16 sVar1 = (l_HIO.horned_head / 360.0f) * 65536.0f;

    if (i_this->field_0x6ba > sVar1) {
        i_this->field_0x6ba = sVar1;
    } else if (i_this->field_0x6ba < (s16)-sVar1) {
        i_this->field_0x6ba = -sVar1;
    }

    cLib_addCalcAngleS2(&i_this->field_0x6b8, i_this->field_0x6ba, 2, 0x200);
    cXyz i_effSize(1.0f, 1.0f, 1.0f);
    setMidnaBindEffect(&i_this->actor, &i_this->mSound, &a_this->eyePos, &i_effSize);
}

static void anm_se_set(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    if (i_this->mAnm == BCK_DD_FIRE_B) {
        i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_DD_V_FIRE_B, -1);
    }

    if (i_this->mAnm == BCK_DD_FIRE_C) {
        if (i_this->mpModelMorf->checkFrame(14.5f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_DD_V_FIRE_C, -1);
        } else if (i_this->mpModelMorf->checkFrame(21.0f)) {
            fopAcM_seStartCurrent(a_this, Z2SE_CM_BODYFALL_S, 0);
        }
    }

    if (i_this->mAnm == BCK_DD_DIE && i_this->mpModelMorf->checkFrame(27.0f)) {
        fopAcM_seStartCurrent(a_this, Z2SE_CM_BODYFALL_M, 0);
    }

    if (i_this->mpModelMorf->checkFrame(1.0f)) {
        if (i_this->mAnm == BCK_DD_WAIT) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_DD_V_WAIT, -1);
        }

        if ((i_this->mAnm == BCK_DD_DAMAGE_R || i_this->mAnm == BCK_DD_DAMAGE_L) && i_this->mAtInfo.mAttackPower != 0) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_DD_V_DAMAGE, -1);
        }

        if (i_this->mAnm == BCK_DD_DIE || i_this->mAnm == BCK_DD_DIE_FALL_A_1 || i_this->mAnm == BCK_DD_DIE_FALL_A_2) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_DD_V_DIE, -1);
        }

        if (i_this->mAnm == BCK_DD_DIE_BOMB) {
            static u16 bomb_eff_name[4] = {
                0x84C6, 0x84C7, 0x84C8, 0x84C9,
            };

            for (int i = 0; i < 4; i++) {
                JPABaseEmitter* emitter = dComIfGp_particle_set(bomb_eff_name[i], &a_this->current.pos, NULL, NULL);
                if (emitter != NULL) {
                    emitter->setGlobalSRTMatrix(i_this->mpModelMorf->getModel()->getAnmMtx(JNT_NECK));
                }
            }
        }
    }

    if (i_this->mAnm == BCK_DD_WAIT && i_this->mpModelMorf->checkFrame(1.0f)) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_DD_V_WAIT, -1);
    }

    if (i_this->mAnm == BCK_DD_WALK) {
        if (i_this->mpModelMorf->checkFrame(31.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_DD_WALK_L, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(5.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_DD_WALK_R, 0, -1);
        }
    }

    if (i_this->mAnm == BCK_DD_RUN) {
        if (i_this->mpModelMorf->checkFrame(9.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_DD_RUN_L, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_DD_RUN_R, 0, -1);
        }
    }

    if (i_this->mAnm == BCK_DD_FIRE_A) {
        i_this->field_0xe5a = 1;
    }

    if (i_this->mAnm == BCK_DD_FIRE_B) {
        i_this->field_0xe58 = 1;
        i_this->field_0xe5a = 1;
    }

    if ((i_this->mAnm == BCK_DD_FIRE_A && i_this->mpModelMorf->getFrame() >= 44.0f) || i_this->mAnm == BCK_DD_FIRE_B) {
        for (int i = 0; i < 4; i++) {
            static u16 fire_eff_name[4] = {
                0x846C, 0x846D, 0x846E, 0x846F,
            };

            i_this->field_0xe68[i] = dComIfGp_particle_set(i_this->field_0xe68[i], fire_eff_name[i], &a_this->current.pos, NULL, NULL);
            JPABaseEmitter* fire_emitter = dComIfGp_particle_getEmitter(i_this->field_0xe68[i]);
            if (fire_emitter != NULL) {
                fire_emitter->setGlobalSRTMatrix(i_this->mpModelMorf->getModel()->getAnmMtx(JNT_NECK));
            }
        }
    }

    if (i_this->mAnm == BCK_DD_FIRE_A || i_this->mAnm == BCK_DD_FIRE_B) {
        i_this->field_0xe78 = dComIfGp_particle_set(i_this->field_0xe78, 0x8470, &a_this->current.pos, NULL, NULL);
        JPABaseEmitter* emitter_3 = dComIfGp_particle_getEmitter(i_this->field_0xe78);
        if (emitter_3 != NULL) {
            emitter_3->setGlobalSRTMatrix(i_this->mpModelMorf->getModel()->getAnmMtx(JNT_BACKBONE_2));
        }
    }

    if (i_this->mAnm == BCK_DD_FIRE_A) {
        for (int i = 0; i < 2; i++) {
            static u16 fire_eff_name[2] = {
                0x8471, 0x8472,
            };

            i_this->field_0xe7c[i] = dComIfGp_particle_set(i_this->field_0xe7c[i], fire_eff_name[i], &a_this->current.pos, NULL, NULL);
            JPABaseEmitter* emitter_4 = dComIfGp_particle_getEmitter(i_this->field_0xe7c[i]);
            if (emitter_4 != NULL) {
                emitter_4->setGlobalSRTMatrix(i_this->mpModelMorf->getModel()->getAnmMtx(i + 10));
            }
        }
    }

    if (i_this->field_0x67c != 0 && i_this->field_0x67c != 4 && i_this->mAnm != BCK_DD_FIRE_A && i_this->mAnm != BCK_DD_FIRE_B && i_this->mAnm != BCK_DD_FIRE_C) {
        i_this->field_0x67c = 0;
        i_this->mpBrkAnms[0]->setFrame(0.0f);
    }
}

static int daE_DD_Execute(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp1c, sp28;

    i_this->field_0x688++;

    for (int i = 0; i < 4; i++) {
        if (i_this->field_0x6aa[i] != 0) {
            i_this->field_0x6aa[i]--;
        }
    }

    if (i_this->field_0x6b2 != 0) {
        i_this->field_0x6b2--;
    }

    if (i_this->field_0x6d3 != 0){
        i_this->field_0x6d3--;
    }

    i_this->field_0xa74[0].OnCoSPrmBit(0x40);

    action(i_this);

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->field_0x6d6.y);
    mDoMtx_stack_c::XrotM(i_this->field_0x6d6.x);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(a_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.basic_size, l_HIO.basic_size, l_HIO.basic_size);

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpBrkAnms[i_this->field_0x67c]->play();

    if (i_this->mpBrkAnms[1]->getFrame() >= 57.0f) {
        i_this->mpBrkAnms[1]->setFrame(57.0f);
    }

    if (i_this->mpBrkAnms[3]->getFrame() >= 46.0f) {
        i_this->mpBrkAnms[3]->setFrame(46.0f);
    }

    if (i_this->mpBrkAnms[4]->getFrame() >= 40.0f) {
        i_this->mpBrkAnms[4]->setFrame(40.0f);
    }

    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    i_this->mpModelMorf->modelCalc();

    anm_se_set(i_this);

    MTXCopy(model->getAnmMtx((int)AREG_S(4) + JNT_NECK), *calc_mtx);
    sp1c.set(AREG_F(0) + 40.0f, AREG_F(1), AREG_F(2) + -40.0f);
    MtxPosition(&sp1c, &i_this->field_0xe40);
    sp1c.set(AREG_F(13), AREG_F(14), AREG_F(15));
    MtxPosition(&sp1c, &sp28);

    i_this->field_0xa74[0].SetC(sp28);
    i_this->field_0xa74[0].SetR((AREG_F(6) + 60.0f) * l_HIO.basic_size);
    dComIfG_Ccsp()->Set(&i_this->field_0xa74[0]);

    MTXCopy(model->getAnmMtx(JNT_BACKBONE_2), *calc_mtx);
    sp1c.set(AREG_F(7) + -25.0f, AREG_F(8), AREG_F(9));
    MtxPosition(&sp1c, &sp28);
    i_this->field_0xa74[1].SetC(sp28);
    i_this->field_0xa74[1].SetR((AREG_F(10) + 20.0f + 55.0f) * l_HIO.basic_size);
    dComIfG_Ccsp()->Set(&i_this->field_0xa74[1]);

    MTXCopy(model->getAnmMtx(JNT_TAIL3), *calc_mtx);
    sp1c.set(AREG_F(11), AREG_F(12), AREG_F(13));
    MtxPosition(&sp1c, &i_this->field_0xe4c);
    i_this->field_0xa74[2].SetC(i_this->field_0xe4c);
    i_this->field_0xa74[2].SetR((AREG_F(14) + 50.0f) * l_HIO.basic_size);
    dComIfG_Ccsp()->Set(&i_this->field_0xa74[2]);

    if (i_this->field_0x6b2 != 0) {
        i_this->field_0xa74[0].OffTgSetBit();
        i_this->field_0xa74[1].OffTgSetBit();
        i_this->field_0xa74[2].OffTgSetBit();
    } else {
        i_this->field_0xa74[0].OnTgSetBit();
        i_this->field_0xa74[1].OnTgSetBit();
        i_this->field_0xa74[2].OnTgSetBit();
    }

    if (i_this->field_0xe58 != 0) {
        cMtx_YrotS(*calc_mtx, i_this->field_0x6d6.y);
        cMtx_XrotM(*calc_mtx, i_this->field_0x6d6.x);
        cMtx_YrotM(*calc_mtx, a_this->shape_angle.y + i_this->field_0x6b8);
        cMtx_XrotM(*calc_mtx, a_this->shape_angle.x);
        sp1c.x = 0.0f;
        sp1c.y = 0.0f;
        sp1c.z = i_this->field_0xe5c;
        MtxPosition(&sp1c, &sp28);
        sp28 += a_this->eyePos;

        if (i_this->field_0xe59 == 0) {
            i_this->field_0xe59 = 1;
            i_this->mFireSph.StartCAt(sp28);
        } else {
            i_this->mFireSph.MoveCAt(sp28);
        }

        i_this->mFireSph.SetR(KREG_F(5) + 10.0f);
        dComIfG_Ccsp()->Set(&i_this->mFireSph);

        i_this->field_0xe5c += 40.0f;

        if (i_this->field_0xe5c >= 250.0f) {
            i_this->field_0xe5c = 0.0f;
        }

        i_this->field_0xe58 = 0;
    } else {
        i_this->field_0xe59 = 0;
        i_this->field_0xe5c = 0.0f;
    }

    s16 sVar1 = i_this->field_0x6a0 - a_this->shape_angle.y;
    s8 bVar1 = 0;

    if (sVar1 < 0x4000 && sVar1 > -0x4000) {
        a_this->eyePos = i_this->field_0xe40;
    } else {
        a_this->eyePos = i_this->field_0xe4c;
        bVar1 = 1;
    }

    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += (AREG_F(4) + 70.0f) * l_HIO.basic_size;
    s8 bVar2 = 0;

    if (i_this->field_0x6a4 > (NREG_F(12) + 350.0f)) {
        if (i_this->field_0xe5a == 0) {
            bVar2 = 1;
        }
    } else {
        if (bVar1 == 0 && daPy_getPlayerActorClass()->getCutAtFlg() != 0) {
            bVar2 = 1;
        }
    }

    if (bVar2 != 0) {
        i_this->field_0xa74[0].OnTgShield();
        i_this->field_0xa74[0].SetTgHitMark(CcG_Tg_UNK_MARK_2);
        i_this->field_0xa74[1].OnTgShield();
        i_this->field_0xa74[1].SetTgHitMark(CcG_Tg_UNK_MARK_2);
    } else {
        i_this->field_0xa74[0].OffTgShield();
        i_this->field_0xa74[0].SetTgHitMark(CcG_Tg_UNK_MARK_0);
        i_this->field_0xa74[1].OffTgShield();
        i_this->field_0xa74[1].SetTgHitMark(CcG_Tg_UNK_MARK_0);
    }

    if (i_this->field_0x5b6 != 0) {
        MTXCopy(model->getAnmMtx(JNT_BACKBONE_2), *calc_mtx);
        sp1c.set(AREG_F(7) + -25.0f, AREG_F(8), AREG_F(9));
        MtxPosition(&sp1c, &sp28);
        fopAcM_createDisappear(a_this, &sp28, 13, 0, 32);
        fopAcM_delete(a_this);

        if ((int)(fopAcM_GetParam(a_this) >> 24) != 0xFF) {
            dComIfGs_onSwitch(fopAcM_GetParam(a_this) >> 24, fopAcM_GetRoomNo(a_this));
        }
    }

    return 1;
}

static int daE_DD_IsDelete(e_dd_class* i_this) {
    return 1;
}

static int daE_DD_Delete(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    dComIfG_resDelete(&i_this->mPhase, "E_dd");

    if (i_this->field_0xe90 != 0) {
        hio_set = 0;
    }

    if (a_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    e_dd_class* i_this = (e_dd_class*)a_this;

    i_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_dd", BMDR_DD), NULL, NULL,
                                               (J3DAnmTransform*)dComIfG_getObjectRes("E_dd", BCK_DD_WAIT), 2, 1.0f, 0, -1,
                                               &i_this->mSound, 0x80000, 0x11020084);
    if (i_this->mpModelMorf == NULL || i_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setUserArea((uintptr_t)i_this);
    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        if (i != 0) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
    }

    static int brk_d[5] = {
        BRK_DD, BRK_DD_FIRE_A, BRK_DD_FIRE_B, BRK_DD_FIRE_C, BRK_DD_DEAD,
    };

    for (int i = 0; i < 5; i++) {
        i_this->mpBrkAnms[i] = new mDoExt_brkAnm();
        if (i_this->mpBrkAnms[i] == NULL) {
            return 0;
        }

        int i_attr;
        if (i == 4) {
            i_attr = J3DFrameCtrl::EMode_NONE;
        } else {
            i_attr = J3DFrameCtrl::EMode_LOOP;
        }

        f32 i_rate = 1.0f;
        if (i == 1 || i == 3) {
            i_rate = l_HIO.flame_mot_speed;
        }

        if (i_this->mpBrkAnms[i]->init(i_this->mpModelMorf->getModel()->getModelData(),
                                       (J3DAnmTevRegKey*)dComIfG_getObjectRes("E_dd", brk_d[i]), 1,
                                       i_attr, i_rate, 0, -1) == 0) {
            return 0;
        }
    }

    return 1;
}

static s16 ya[5] = {
    -0x4000, 0x4000, 0x0000, -0x8000, 0x0000,
};

static s16 xa[5] = {
    -0x4000, -0x4000, -0x4000, -0x4000, -0x8000,
};

static int kabe_initial_pos_set(e_dd_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz sp98, start, end;

    for (int i = 0; i < 5; i++) {
        MtxTrans(a_this->current.pos.x, a_this->current.pos.y + 5.0f, a_this->current.pos.z, 0);
        cMtx_YrotM(*calc_mtx, ya[i]);
        cMtx_XrotM(*calc_mtx, xa[i]);
        sp98.x = 0.0f;
        sp98.y = 200.0f;
        sp98.z = 0.0f;
        MtxPosition(&sp98, &start);
        sp98.y = -400.0f;
        MtxPosition(&sp98, &end);
        lin_chk.Set(&start, &end, NULL);
        
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            OS_REPORT("//////////////E_DD ON BG !! %d\n");
            cXyz* cross = &lin_chk.GetCross();
            a_this->current.pos = *cross;
            a_this->home.pos = a_this->current.pos;
            i_this->field_0x6d4 = i + 1;
            i_this->field_0x6d6.y = ya[i];
            i_this->field_0x6d6.x = xa[i];
            i_this->mShadowKey = 0x564FF;
            return 1;
        }
    }

    return 0;
}

static cPhs__Step daE_DD_Create(fopAc_ac_c* a_this) {
    e_dd_class* i_this = (e_dd_class*)a_this;
    fopAcM_ct(a_this, e_dd_class);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->mPhase, "E_dd");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_DD PARAM %x\n", fopAcM_GetParam(a_this));

        u8 uVar1 = fopAcM_GetParam(a_this) >> 16 & 0xFF;
        if ((int)(fopAcM_GetParam(a_this) >> 24) != 0xFF) {
            if (dComIfGs_isSwitch(fopAcM_GetParam(a_this) >> 24, fopAcM_GetRoomNo(a_this))) {
                OS_REPORT("E_DD やられ後なので再セットしません\n"); // Since I was attacked in E_DD, I will not reset\n
                return cPhs_ERROR_e;
            }
        }

        i_this->field_0x5b4 = fopAcM_GetParam(a_this);
        i_this->field_0x5b5 = (fopAcM_GetParam(a_this) >> 8) & 15;

        OS_REPORT("E_DD//////////////E_DD SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x20E0)) {
            OS_REPORT("//////////////E_DD SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_DD SET 2 !!\n");

        if (uVar1 != 0xFF) {
            i_this->mpPath = dPath_GetRoomPath(uVar1, fopAcM_GetRoomNo(a_this));

            OS_REPORT("//////////////E_DD PPD %x!!\n");

            if (i_this->mpPath == NULL) {
                OS_REPORT("......DD  NONONONONONO PATH !!!!\n");
                return cPhs_ERROR_e;
            }
        }

        if (hio_set == 0) {
            i_this->field_0xe90 = 1;
            hio_set = 1;
            l_HIO.field_0x4 = -1;
        }

        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(a_this, i_this->mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(a_this, -500.0f, -200.0f, -500.0f);
        fopAcM_SetMax(a_this, 500.0f, 200.0f, 500.0f);
        a_this->health = 60;
        a_this->field_0x560 = 60;
        i_this->mStts.Init(250, 0, a_this);

        static dCcD_SrcSph fire_sph_src = {
            {
                {0x0, {{0x100, 0x2, 0x1d}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_SWORD, 0x0, 0x1, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x1f}, {0xd8fbfdff, 0x43}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x3}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        for (int i = 0; i < 3; i++) {
            i_this->field_0xa74[i].Set(cc_sph_src);
            i_this->field_0xa74[i].SetStts(&i_this->mStts);

            if (i == 2) {
                i_this->field_0xa74[i].OffTgShield();
                i_this->field_0xa74[i].SetTgHitMark(CcG_Tg_UNK_MARK_0);
            } else {
                i_this->field_0xa74[i].OnTgShield();
                i_this->field_0xa74[i].SetTgHitMark(CcG_Tg_UNK_MARK_2);
            }
        }

        i_this->mFireSph.Set(fire_sph_src);
        i_this->mFireSph.SetStts(&i_this->mStts);
        i_this->mFireSph.SetAtType(AT_TYPE_100);
        i_this->mFireSph.SetAtMtrl(dCcD_MTRL_FIRE);
        i_this->mObjAcch.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1, &i_this->mAcchCir,
                             fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(50.0f, 100.0f);
        i_this->mSound.init(&a_this->current.pos, &a_this->eyePos, 3, 1);
        i_this->mSound.setEnemyName("E_dd");

        i_this->mAtInfo.mPowerType = 2;
        i_this->mAtInfo.mpSound = &i_this->mSound;
        i_this->field_0x688 = cM_rndF(65535.0f);

        if (i_this->field_0x5b4 == 2 || i_this->field_0x5b4 == 3) {
            kabe_initial_pos_set(i_this);
        }

        daE_DD_Execute(i_this);
    }

    return phase;
}

actor_method_class l_daE_DD_Method = {
    (process_method_func)daE_DD_Create,
    (process_method_func)daE_DD_Delete,
    (process_method_func)daE_DD_Execute,
    (process_method_func)daE_DD_IsDelete,
    (process_method_func)daE_DD_Draw,
};

actor_process_profile_definition g_profile_E_DD = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_DD,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_dd_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  117,                    // mPriority
  &l_daE_DD_Method,       // sub_method
  0x10040100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
