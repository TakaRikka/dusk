/**
 * @file d_a_e_sw.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_sw.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"
#include "d/d_bomb.h"
#include <cmath.h>
#include "Z2AudioLib/Z2Instances.h"

class daE_SW_HIO_c {
public:
    /* サンドワーム (Sandworm) */
    daE_SW_HIO_c();
    virtual ~daE_SW_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 basic_size;                  // 基本サイズ (Basic Size)
    /* 0x0C */ f32 bomb_notice_dist;            // 爆弾認識距離 (Bomb Recognition Distance)
    /* 0x10 */ f32 bomb_attack_start_range;     // 爆弾攻撃開始範囲 (Bomb Attack Start Range)
    /* 0x14 */ f32 human_attack_init_range;     // 人用攻撃開始範囲 (Human Attack Initiation Range)
    /* 0x18 */ f32 horse_attack_init_range;     // 馬用攻撃開始範囲 (Horse Attack Initiation Range)
    /* 0x1C */ f32 character_notice_dist;       // 人認識距離(ダンジョン用) (Character Recognition Distance (For Dungeon))
    /* 0x20 */ u8 movement_range_debug_display; // 移動範囲デバック表示 (Movement Range Debug Display)
};

enum E_SW_RES_File_ID {
    /* BCK */
    /* 0x04 */ BCK_SW_ATTACK = 0x4,
    /* 0x05 */ BCK_SW_ATTACK_END,
    /* 0x06 */ BCK_SW_ATTACK_MIDDLE,
    /* 0x07 */ BCK_SW_ATTACK_START,
    /* 0x08 */ BCK_SW_CHANCE,
    /* 0x09 */ BCK_SW_DAMAGE,
    /* 0x0A */ BCK_SW_DIE,
    /* 0x0B */ BCK_SW_DIVE,
    /* 0x0C */ BCK_SW_RUN,
    /* 0x0D */ BCK_SW_WAIT,

    /* BMDR */
    /* 0x10 */ BMDR_SW = 0x10,
};

enum Action {
    /* 0x0 */ ACTION_EXECUTE_WAIT,
    /* 0x1 */ ACTION_EXECUTE_CHASE_SLOW,
    /* 0x2 */ ACTION_EXECUTE_CHASE_FAST,
    /* 0x3 */ ACTION_EXECUTE_ATTACK,
    /* 0x4 */ ACTION_EXECUTE_BOMB,
    /* 0x5 */ ACTION_EXECUTE_HOOK,
    /* 0x6 */ ACTION_EXECUTE_MOVE_OUT,
    /* 0x7 */ ACTION_EXECUTE_CHANCE,
    /* 0x8 */ ACTION_EXECUTE_DIVE,
    /* 0x9 */ ACTION_EXECUTE_DAMAGE,
    /* 0xA */ ACTION_EXECUTE_DIE,
    /* 0xB */ ACTION_EXECUTE_MASTER,
    /* 0xC */ ACTION_EXECUTE_SUDDEN_ATTACK,
    /* 0xD */ ACTION_EXECUTE_FALL,
};

enum Particle {
    /* 0x837F */ ZL2_ROCK_1 = 0x837F,
    /* 0x8380 */ ZLM_SAND00_IA_1,
    /* 0x836F */ DKM_REALSMOKE01_IA = 0x836F,
    /* 0x8370 */ ZL2_ROCK_2,
    /* 0x8371 */ ZLM_SAND00_IA_2,
    /* 0x8372 */ ZL2_SMOKE00_1,
    /* 0x837C */ ZL2_ROCK_3 = 0x837C,
    /* 0x837D */ ZLM_SAND00_IA_3,
    /* 0x837E */ ZL2_SMOKE00_2,
    /* 0x8373 */ ZL2_TSUBU_IA01_1 = 0x8373,
    /* 0x8374 */ ZL2_TSUBU_IA01_2,
    /* 0x8375 */ ZLM_SAND00_IA_4,
    /* 0x8376 */ ZL2_ROCK_4,
    /* 0x8377 */ ZLM_SAND00_IA_5,
    /* 0x8378 */ ZLM_SAND00_IA_6,
};

namespace {
    static dCcD_SrcSph cc_sph_src = {
        {
            {0, {{AT_TYPE_CSTATUE_SWING, 1, 0x1D}, {0xD8FBFDFF, 0x43}, 0x75}},
            {dCcD_SE_SOFT_BODY, 0, 0, 0, 0},
            {dCcD_SE_NONE, 0, 0, 0, 2},
            {0},
        },
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f},
        },
    };
}

daE_SW_HIO_c::daE_SW_HIO_c() {
    field_0x4 = -1;
    basic_size = 1.0f;
    bomb_notice_dist = 1500.0f;
    bomb_attack_start_range = 800.0f;
    human_attack_init_range = 400.0f;
    horse_attack_init_range = 800.0f;
    character_notice_dist = 1000.0f;
    movement_range_debug_display = 0;
}

void daE_SW_c::bckSet(int i_resID, f32 i_morf, u8 i_attr, f32 i_rate) {
    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_SW", i_resID), i_attr, i_morf, i_rate, 0.0f, -1.0f);
}

int daE_SW_c::draw() {
    if (field_0x694 == 1) {
        return 1;
    }

    J3DModel* model = mpModelMorf->getModel();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    if (field_0x6e5 == 0) {
        if (field_0x6b4) {
            J3DModelData* modelData = model->getModelData();

            for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
                J3DMaterial* matNodeP = modelData->getMaterialNodePointer(i);
                matNodeP->getTevColor(0)->r = field_0x6b4;
                matNodeP->getTevColor(0)->g = field_0x6b4;
                matNodeP->getTevColor(0)->b = field_0x6b4;
            }
        }

        mpModelMorf->entryDL();
    }

    if (field_0x6e4 == 0) {
        cXyz i_pos;
        i_pos.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
        mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &i_pos, 700.0f, 0.0f, current.pos.y, 
                                            mBgc.GetGroundH(), mBgc.m_gnd, &tevStr, 0, 1.0f, 
                                            dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static int daE_SW_Draw(daE_SW_c* i_this) {
    return i_this->draw();
}

void daE_SW_c::setBeforeJumpEffect() {
    field_0xae8 = dComIfGp_particle_set(field_0xae8, ZL2_ROCK_1, &field_0x674, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
    field_0xaec = dComIfGp_particle_set(field_0xaec, ZLM_SAND00_IA_1, &field_0x674, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
}

void daE_SW_c::setJumpEffect() {
    field_0xae8 = dComIfGp_particle_set(field_0xae8, ZL2_TSUBU_IA01_1, &field_0x674, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
    field_0xaec = dComIfGp_particle_set(field_0xaec, ZL2_TSUBU_IA01_2, &field_0x674, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
}

void daE_SW_c::setSmokeEffect() {
    if (field_0x6e4 != 0 && speedF) {
        cXyz sp14(field_0x674);
        sp14.y += 100.0f;
        field_0xaf0 = dComIfGp_particle_set(field_0xaf0, ZLM_SAND00_IA_4, &sp14, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
        field_0xaf4 = dComIfGp_particle_set(field_0xaf4, ZL2_ROCK_4, &sp14, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
        mSound.startCreatureSoundLevel(Z2SE_EN_SW_MOVE, field_0x6e9, -1);
    }
}

void daE_SW_c::setDiveEffect() {
    csXyz acStack_18(0, shape_angle.y, 0);
    static u16 l_SW_DIVE_EFFECT_ID[4] = {DKM_REALSMOKE01_IA, ZL2_ROCK_2, ZLM_SAND00_IA_2, ZL2_SMOKE00_1};
    for (int i = 0; i < 4; i++) {
        dComIfGp_particle_set(l_SW_DIVE_EFFECT_ID[i], &current.pos, &tevStr, &acStack_18, NULL);
    }
}

void daE_SW_c::setHideEffect() {
    static u16 l_SW_HIDE_EFFECT_ID[3] = {ZL2_ROCK_3, ZLM_SAND00_IA_3, ZL2_SMOKE00_2};
    for (int i = 0; i < 3; i++) {
        dComIfGp_particle_set(l_SW_HIDE_EFFECT_ID[i], &current.pos, &tevStr, &shape_angle, NULL);
    }
}

namespace {
    /* 807B01F8-807B01FC 000048 0004+00 1/2 0/0 0/0 .bss target_info__22@unnamed@d_a_e_sw_cpp@ */
    static fopAc_ac_c* target_info;

    /* 807B01FC-807B0200 00004C 0004+00 1/2 0/0 0/0 .bss target_dist__22@unnamed@d_a_e_sw_cpp@ */
    static f32 target_dist;
};

/* 807B0200 */
static u8 data_807B0200;

/* 807B0201 */
static u8 data_807B0201;

/* 807B0202 */
static u8 data_807B0202;

/* 807B0203 0001+00 data_807B0203 None */
static u8 l_initHIO;

void daE_SW_c::setActionMode(s16 i_actionMode, s16 i_moveMode) {
    if (field_0x6e6 != 0) {
        data_807B0200 = 0;
        field_0x6e6 = 0;
    }

    field_0x6f0 = 0;
    field_0x98c.SetTgType(0x4000);
    mActionMode = i_actionMode;
    mMoveMode = i_moveMode;
}

static void* s_b_sub(void* i_actor, void* i_data) {
    if (!fpcM_IsCreating(fopAcM_GetID(i_actor)) && fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NBOMB && 
        !fopAcM_checkCarryNow((fopAc_ac_c*)i_actor) && !((dBomb_c*)i_actor)->checkStateExplode()) {
        f32 fVar1 = fopAcM_searchActorDistance((fopAc_ac_c*)i_actor, (fopAc_ac_c*)i_data);
        if (fVar1 < target_dist) {
            target_info = (fopAc_ac_c*)i_actor;
            target_dist = fVar1;
        }
    }

    return NULL;
}

static daE_SW_HIO_c l_HIO;

fopAc_ac_c* daE_SW_c::search_bomb() {
    target_info = NULL;
    target_dist = l_HIO.bomb_notice_dist;
    fpcM_Search(s_b_sub, this);
    field_0x688 = fpcM_ERROR_PROCESS_ID_e;

    if (target_info != NULL) {
        field_0x688 = fopAcM_GetID(target_info);
    }

    return target_info;
}

BOOL daE_SW_c::bomb_check() {
    if (field_0x68c == 0) {
        fopAc_ac_c* bomb = search_bomb();
        if (bomb != NULL && fopAcM_searchActorDistance(this, bomb) < l_HIO.bomb_notice_dist) {
            setActionMode(4, 0);
            return TRUE;
        }
    }

    return FALSE;
}

bool daE_SW_c::setModeBack() {
    if ((field_0x6bc & 3) == 0) {
        setActionMode(0, 0);
        current.pos = old.pos;
        return true;
    }

    return false;
}

void daE_SW_c::checkFall() {
    if (mActionMode == 11 || mActionMode == 5 || mActionMode == 13 || mActionMode == 3) {
        return;
    }

    if (mBgc.ChkGroundHit()) {
        return;
    }

    dBgS_GndChk gnd_chk;
    cXyz i_pos(current.pos);
    i_pos.y += 100.0f;
    gnd_chk.SetPos(&i_pos);
    f32 fVar1 = dComIfG_Bgsp().GroundCross(&gnd_chk);

    if (speed.y < 0.0f) {
        if (fVar1 == -G_CM3D_F_INF || std::abs(fVar1 - current.pos.y) > 1000.0f || dComIfG_Bgsp().GetGroundCode(gnd_chk) == 4 || 
            dComIfG_Bgsp().GetGroundCode(gnd_chk) == 10 || dComIfG_Bgsp().GetGroundCode(gnd_chk) == 5) {
            setActionMode(13, 0);
        }
    }
}

bool daE_SW_c::checkBeforeWall() {
    if (field_0x6f8[0].ChkWallHit()) {
        if (abs((s16)(field_0x6f8[0].GetWallAngleY() - current.angle.y)) > 0x7000) {
            return true;
        }
    }

    if (field_0x6f8[1].ChkWallHit()) {
        if (abs((s16)(field_0x6f8[1].GetWallAngleY() - current.angle.y)) > 0x7000) {
            return true;
        }
    }

    return false;
}

void daE_SW_c::executeWait() {
    if (speedF > 2.0f) {
        setSmokeEffect();
    }

    switch (mMoveMode) {
        case 0:
            field_0x6e4 = 1;
            field_0x6e5 = 1;
            bckSet(BCK_SW_WAIT, 10.0f, 2, 1.0f);
            mMoveMode = 1;
            field_0x6ea = 50;
            // fallthrough
        case 1:
            if (field_0x6ea == 0) {
                mMoveMode = 2;

                if (field_0x6d0 > 1000.0f) {
                    field_0x6ea = 90;
                } else {
                    field_0x6ea = 30;
                }

                if (field_0x6d0 > 600.0f) {
                    field_0x69e = cLib_targetAngleY(&current.pos, &home.pos);
                } else {
                    field_0x69e = cM_rndFX(60000.0f);
                }
            }

            cLib_addCalc2(&speedF, 0.0f, 0.2f, 10.0f);
            break;

        case 2:
            cLib_addCalcAngleS2(&current.angle.y, field_0x69e, 8, 0x400);

            if (field_0x6ea == 0) {
                mMoveMode = 1;
                field_0x6ea = 50;
            }

            if (field_0x6d0 > 1000.0f) {
                cLib_chaseF(&speedF, 20.0f, 1.0f);
            } else {
                cLib_chaseF(&speedF, 10.0f, 1.0f);
            }
            break;
    }

    shape_angle.y = current.angle.y;

    if (!bomb_check()) {
        if (daPy_getPlayerActorClass()->current.pos.absXZ(home.pos) < field_0x690 && !checkSuddenAttack(0)) {
            setActionMode(1, 0);
        }
    }
}

int daE_SW_c::checkRunChase() {
    if (field_0x6ee == 0) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        f32 fVar1;
        if (player->checkHorseRide() == 0) {
            fVar1 = 19.0f;
        } else {
            fVar1 = 30.0f;
        }

        if (player->getSpeedF() > fVar1) {
            return 1;
        }

        return -1;
    }

    return 0;
}

void daE_SW_c::executeChaseSlow() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp3c(player->current.pos);
    cXyz sp48, sp54;
    f32 fVar1 = player->getSpeedF() + 5.0f;

    if (fVar1 < 15.0f) {
        fVar1 = 15.0f;
    }
    
    if (fVar1 > 70.0f) {
        fVar1 = 70.0f;
    }

    setSmokeEffect();

    if (!bomb_check() && !checkSuddenAttack(0)) {
        if ((field_0x6bc & 1) == 0) {
            setActionMode(7, 0);
            return;
        }

        if (checkRunChase() > 0) {
            if (current.pos.absXZ(daPy_getPlayerActorClass()->current.pos) > 400.0f) {
                setActionMode(2, 0);
                return;
            }
        }

        if (home.pos.absXZ(sp3c) > field_0x690) {
            setActionMode(6, 0);
            return;
        }

        switch (mMoveMode) {
            case 0:
                field_0x6b8 = cM_rndF(100.0f) + 800.0f;
                bckSet(BCK_SW_RUN, 10.0f, 2, 1.0f);

                if (cM_rnd() < 0.5f) {
                    field_0x6a2 = -0x200;
                } else {
                    field_0x6a2 = 0x200;
                }

                field_0x6a4 = field_0x6cc + 0x8000;
                field_0x6ea = cM_rndFX(50.0f) + 100.0f;
                field_0x6ec = cM_rndFX(50.0f) + 150.0f;
                field_0x6ee = 30;
                mMoveMode = 1;
                // fallthrough
            case 1:
                sp54.set(0.0f, 0.0f, field_0x6b8 - 200.0f);
                cLib_offsetPos(&sp48, &sp3c, field_0x6a4, &sp54);
                if (abs((s16)(field_0x6cc + 0x8000 - (s16)cLib_targetAngleY(&sp3c, &sp48))) < 0x2000) {
                    field_0x6a4 += field_0x6a2;
                }

                cLib_chaseF(&speedF, fVar1, 0.3f);
                cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &sp48), 8, 0x400);

                if (checkBeforeWall()) {
                    field_0x6ea = 30;
                    mMoveMode = 2;
                    field_0x6a4 = field_0x6cc + 0x8000;
                    field_0x6a2 = -field_0x6a2;
                }

                if (field_0x6ea == 0) {
                    s16 bVar1 = 0;
                    if (dComIfGp_getAttention()->LockonTruth()) {
                        if (dComIfGp_getAttention()->LockonTarget(0) == this) {
                            bVar1 = 1;
                        }
                    }
                    
                    if (((s16)cLib_distanceAngleS(fopCamM_GetAngleY(dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0))), field_0x6cc) > 0x6000 || bVar1) && data_807B0200 == 0) {
                        field_0x6ea = 60;
                        data_807B0200 = 1;
                        field_0x6e6 = 1;
                        mMoveMode = 5;
                        return;
                    }

                    field_0x6ea = cM_rndF(30.0f) + 10.0f;
                }

                if (field_0x6ec == 0) {
                    field_0x6ea = 30;
                    mMoveMode = 2;
                    field_0x6a2 = -field_0x6a2;
                }
                break;

            case 2:
                sp54.set(0.0f, 0.0f, field_0x6b8 + 300.0f);
                cLib_offsetPos(&sp48, &player->current.pos, field_0x6a4, &sp54);

                if (abs((s16)(field_0x6cc + 0x8000 - (s16)cLib_targetAngleY(&sp3c, &sp48))) < 0x2000) {
                    field_0x6a4 += field_0x6a2;
                }

                cLib_chaseF(&speedF, fVar1 - 5.0f, 0.3f);
                cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &sp48), 8, 0x400);

                if (field_0x6ea == 0) {
                    field_0x6b8 = cM_rndF(100.0f) + 800.0f;
                    field_0x6ea = cM_rndFX(50.0f) + 130.0f;
                    field_0x6ec = cM_rndFX(50.0f) + 150.0f;
                    mMoveMode = 1;
                }
                break;

            case 5:
                setBeforeJumpEffect();
                sp54.set(0.0f, 0.0f, field_0x6b8 + 1000.0f + nREG_F(11));
                cLib_offsetPos(&sp48, &player->current.pos, field_0x6a4, &sp54);

                if (abs((s16)(field_0x6cc + 0x8000 - (s16)cLib_targetAngleY(&sp3c, &sp48))) < 0x2000) {
                    field_0x6a4 -= field_0x6a2;
                }

                cLib_chaseF(&speedF, fVar1 + 15.0f + nREG_F(12), 0.3f);
                cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &sp48), 8, 0x400);

                if (field_0x6ea == 0) {
                    mMoveMode = 6;
                }
                break;

            case 6:
                cLib_chaseF(&speedF, fVar1 + 15.0f + nREG_F(12), 0.3f);
                cLib_addCalcAngleS2(&current.angle.y, field_0x6cc, 8, 0x400);

                if (field_0x6c8 < l_HIO.human_attack_init_range) {
                    setActionMode(3, 0);
                    field_0x69c = 0;

                    if (field_0x6e6 != 0) {
                        data_807B0200 = 0;
                        field_0x6e6 = 0;
                    }
                }
                break;
        }

        shape_angle.y = current.angle.y;
    }
}

void daE_SW_c::executeChaseFast() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp50(player->current.pos);
    cXyz sp5c, sp68;

    setSmokeEffect();

    if (field_0x6f0 != 0) {
        setBeforeJumpEffect();
    }

    if (!bomb_check() && !checkSuddenAttack(1)) {
        if ((field_0x6bc & 1) == 0) {
            setActionMode(7, 0);
            return;
        }

        if (checkRunChase() < 0 && mMoveMode != 0) {
            setActionMode(1, 0);
            return;
        }

        if (home.pos.absXZ(sp50) > field_0x690) {
            setActionMode(6, 0);
            return;
        }

        switch (mMoveMode) {
            case 0:
                mMoveMode = 1;
                field_0x6a0 = cM_rndFX(1.0f) * 12288.0f;
                field_0x6a4 = 0;
                field_0x6ea = cM_rndF(30.0f) + 30.0f;
                field_0x6ee = 30;
                // fallthrough
            case 1:
                if (field_0x6ea == 0) {
                    if (data_807B0200 == 0) {
                        if (field_0x6c8 < l_HIO.horse_attack_init_range) {
                            mMoveMode = 2;
                            field_0x6f0 = 60;
                            data_807B0200 = 1;
                            field_0x6e6 = 1;
                        }
                    } else {
                        field_0x6ea = cM_rndF(30.0f) + 30.0f;
                    }
                }
                // fallthrough
            case 2:
                if (mMoveMode == 2 && field_0x6f0 == 0) {
                    setActionMode(3, 0);
                    field_0x69c = 2;

                    if (field_0x6e6 == 0) {
                        return;
                    }

                    data_807B0200 = 0;
                    field_0x6e6 = 0;
                    return;
                }

                sp68.set(0.0f, 0.0f, -200.0f);
                cLib_offsetPos(&sp5c, &sp50, (s16)(field_0x6a0 + shape_angle.y), &sp68);

                if (field_0x6dc > 1.0) {
                    sp68.set(0.0f, 0.0f, 200.0f);
                } else {
                    sp68.set(0.0f, 0.0f, 350.0f);
                }

                cLib_offsetPos(&field_0x668, &sp5c, shape_angle.y, &sp68);

                f32 fVar1;
                if (abs((s16)(field_0x6cc - shape_angle.y)) < 0x4000 || field_0x6e7 == 0) {
                    if (current.pos.absXZ(field_0x668) < 150.0f) {
                        fVar1 = -3.0f;
                    } else if (current.pos.absXZ(field_0x668) < 250.0f) {
                        fVar1 = 0.0f;
                    } else if (current.pos.absXZ(field_0x668) > 500.0f) {
                        fVar1 = 15.0f;
                    } else {
                        fVar1 = 5.0f;
                    }

                    cLib_chaseF(&speedF, fVar1 + player->getSpeedF(), 0.5f);
                } else {
                    cLib_chaseF(&speedF, nREG_F(1) + 15.0f, 0.5f);
                }

                cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &field_0x668), 0x10, 0x400);
                field_0x6a4 += 0x200;
                sp68.x = std::abs(cM_ssin(field_0x6a4)) * 20.0f;
                sp68.z = cM_ssin(field_0x6a4) * 3.0f;
                current.pos.x += sp68.x * cM_scos(shape_angle.y) + (sp68.z * cM_ssin(shape_angle.y));
                current.pos.z += sp68.z * cM_scos(shape_angle.y) - (sp68.x * cM_ssin(shape_angle.y));
                break;
        }

        if (current.pos.absXZ(sp50) < 500.0f) {
            cLib_addCalcAngleS2(&shape_angle.y, player->shape_angle.y, 8, 0x400);
        } else {
            shape_angle.y = current.angle.y;
        }
    }
}

void daE_SW_c::executeBomb() {
    setSmokeEffect();

    if (field_0x6f0 != 0) {
        setBeforeJumpEffect();
    }

    dBomb_c* bomb = NULL;
    fopAcM_SearchByID(field_0x688, (fopAc_ac_c**)&bomb);
    if (bomb == NULL || bomb->checkStateExplode()) {
        setActionMode(0, 0);
        return;
    }

    s16 actorAngleY = fopAcM_searchActorAngleY(this, bomb);

    if (setModeBack()) {
        return;
    }

    switch (mMoveMode) {
        case 0:
            bckSet(BCK_SW_RUN, 10.0f, 2, 1.0f);
            mMoveMode = 1;
            // fallthrough
        case 1:
            if (bomb->speedF < 0.1f && fopAcM_searchActorDistance(this, bomb) < l_HIO.bomb_attack_start_range) {
                mMoveMode = 2;
                field_0x6ea = cM_rndF(30.0f);
            }
            break;

        case 2:
            if (field_0x6ea == 0) {
                mMoveMode = 3;
                field_0x6f0 = 10;
            }
            break;

        case 3:
            if (field_0x6f0 == 0) {
                field_0x6a8 = bomb->current.pos;
                shape_angle.y = actorAngleY;
                current.angle.y = actorAngleY;
                setActionMode(3, 0);
                field_0x69c = 1;
                return;
            }
            break;
    }

    cLib_addCalc2(&speedF, 15.0f, 1.0f, 3.0f);
    cLib_addCalcAngleS2(&current.angle.y, actorAngleY, 8, 0x400);
    shape_angle.y = current.angle.y;
}

void daE_SW_c::executeAttack() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    s16 sVar1;
    f32 fVar1;
    cXyz sp54, sp60;
    
    switch (mMoveMode) {
        case 0:
            field_0x98c.SetTgType(0xd8fbfdff);
            field_0x6e4 = 0;
            field_0x6e5 = 0;
            setDiveEffect();
            mSound.startCreatureSound(Z2SE_EN_SW_JUMP_OUT, field_0x6e9, -1);
            gravity = -4.0f;
            bckSet(BCK_SW_ATTACK_MIDDLE, 0.0f, 0, 1.0f);
            shape_angle.x = -0x4000;

            if (field_0x69c == 1) {
                field_0x69e = cLib_targetAngleY(&current.pos, &field_0x6a8);
            } else {
                field_0x69e = field_0x6cc;
            }

            sVar1 = field_0x69e;
            shape_angle.y = sVar1;
            current.angle.y = sVar1;
            speed.y = 60.0f;

            switch (field_0x69c) {
                case 1:
                    speedF = current.pos.abs(field_0x6a8) / 25.0f;
                    break;

                case 0:
                    speedF = field_0x6c8 / 25.0f;
                    break;

                case 2:
                    if (player->checkHorseRide() == 0) {
                        fVar1 = 19.0f;
                    } else {
                        fVar1 = 30.0f;
                    }

                    if (player->getSpeedF() < fVar1) {
                        speedF = field_0x6c8 / 24.0f;
                    } else {
                        if (player->checkHorseRide()) {
                            speedF = field_0x6c8 / 23.0f;
                        } else {
                            speedF = field_0x6c8 / 25.0f;
                        }

                        sp54.x = player->getSpeedF() * cM_ssin(player->shape_angle.y);
                        sp54.z = player->getSpeedF() * cM_scos(player->shape_angle.y);
                        sp54.x += speedF * cM_ssin(field_0x69e);
                        sp54.z += speedF * cM_scos(field_0x69e);
                        field_0x69e = cM_atan2s(sp54.x, sp54.z);
                        speedF = sp54.absXZ();
                    }
                    break;
            }

            field_0x668 = current.pos;
            mMoveMode = 1;
            break;

        case 1:
            setJumpEffect();
            cLib_chaseAngleS(&shape_angle.x, 0, 0x400);

            if (speed.y < 0.0f) {
                field_0x6d8 = 0.0f;
                current.pos.y -= 120.0f;
                bckSet(BCK_SW_ATTACK_END, 10.0f, 0, 1.0f);
                field_0x6ea = 60;
                mMoveMode = 2;
            }
            break;
            
        case 2:
            setJumpEffect();
            cLib_chaseAngleS(&shape_angle.x, 0x4000, 0x400);
            cLib_chaseF(&speedF, 0.0f, 1.0f);

            if (mBgc.ChkGroundHit()) {
                gravity = -5.0f;
                field_0x6d8 = -60.0f;

                if ((field_0x6bc & 1) == 0) {
                    setActionMode(7, 0);
                } else {
                    field_0x6e4 = 1;
                    setDiveEffect();
                    mSound.startCreatureSound(Z2SE_EN_SW_JUMP_IN, field_0x6e9, -1);
                    mMoveMode = 3;
                    return;
                }

                shape_angle.x = 0;
            } else if (field_0x6ea == 0) {
                fopAcM_delete(this);
            }

            field_0x98c.OnAtSetBit();
            break;

        case 3:
            cLib_chaseF(&speedF, 0.0f, 1.0f);

            if (cLib_chaseF(&field_0x6d8, -120.0f, 60.0f)) {
                field_0x6e5 = 1;
                mMoveMode = 4;
                shape_angle.x = 0;
                field_0x6ec = 60;

                if (player->checkHorseRide() == 0) {
                    fVar1 = 19.0f;
                } else {
                    fVar1 = 30.0f;
                }

                if (player->getSpeedF() < fVar1) {
                    field_0x6ea = 15;
                }
            }
            break;

        case 4:
            if (setModeBack()) {
                return;
            }

            if (field_0x6ea != 0) {
                cLib_chaseF(&speedF, 15.0f, 1.0f);
            } else {
                setActionMode(0, 0);
                field_0x6d8 = -120.0f;
                sp60 = current.pos;
                sp60.y += 200.0f;

                if (!fopAcM_gc_c::gndCheck(&sp60)) {
                    fopAcM_delete(this);
                    return;
                }

                current.pos.y = fopAcM_gc_c::getGroundY();
            }
            break;
    }

    cLib_addCalcAngleS2(&current.angle.y, field_0x69e, 4, 0x800);
    shape_angle.y = current.angle.y;
}

void daE_SW_c::executeHook() {
    cXyz sp24;

    switch (mMoveMode) {
        case 0:
            mSound.startCreatureVoice(Z2SE_EN_SW_V_CATCH, -1);
            field_0x6e4 = 0;
            field_0x6e5 = 0;
            field_0x98c.SetTgType(0);
            bckSet(BCK_SW_DAMAGE, 1.0f, 2, 1.0f);
            field_0x6d8 = 0.0f;
            speed.y = 0.0f;
            speedF = 0.0f;
            gravity = 0.0f;
            mMoveMode = 1;

            if ((s16)(field_0x6cc - shape_angle.y) < 0) {
                field_0x6a4 = -0x4000;
            } else {
                field_0x6a4 = 0x4000;
            }

            shape_angle.y = field_0x6cc + field_0x6a4;

            if (field_0x6e8 >= 1) {
                mDoMtx_stack_c::YrotS(shape_angle.y);

                if (field_0x6a4 < 0) {
                    mDoMtx_stack_c::transM(40.0f, 15.0f, 0.0f);
                } else {
                    mDoMtx_stack_c::transM(-40.0f, 15.0f, 0.0f);
                }

                mDoMtx_stack_c::multVecZero(&sp24);
            } else {
                sp24.set(0.0f, nREG_F(0) + 15.0f, 0.0f);
            }

            daPy_getPlayerActorClass()->setHookshotCarryOffset(fopAcM_GetID(this), &sp24);
            // fallthrough
        case 1:
            cLib_addCalcAngleS(&shape_angle.y, (s16)(field_0x6cc + field_0x6a4), 4, 0x1000, 0x100);
            current.angle.y = shape_angle.y;

            if (!fopAcM_CheckStatus(this, 0x100000)) {
                setActionMode(7, 10);
            }
            break;
    }
}

void daE_SW_c::executeMoveOut() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp3c(player->current.pos);
    cXyz sp48;
    f32 fVar1;
    s16 sVar1;

    setSmokeEffect();

    switch (mMoveMode) {
        case 0:
            bckSet(BCK_SW_RUN, 10.0f, 2, 1.0f);
            // fallthrough
        case 1:
            sVar1 = cLib_targetAngleY(&home.pos, &current.pos) + cM_rndFX(4096.0f);
            sp48.set(0.0f, 0.0f, field_0x690 - 300.0f);
            cLib_offsetPos(&field_0x668, &home.pos, sVar1, &sp48);
            field_0x6ea = 30;
            mMoveMode = 2;
            // fallthrough
        case 2:
            cLib_chaseF(&speedF, 10.0f, 1.0f);
            cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &field_0x668), 8, 0x400);
            shape_angle.y = current.angle.y;

            if (field_0x6ea == 0) {
                mMoveMode = 1;
            }

            if (home.pos.absXZ(sp3c) < field_0x690 - 100.0f) {
                setActionMode(1, 0);
            } else if (current.pos.absXZ(sp3c) > 1000.0f) {
                setActionMode(0, 0);
            }
            break;
    }
}

void daE_SW_c::executeChance() {
    s16 sVar1;

    switch (mMoveMode) {
        case 10:
            speed.y = 20.0f;
            // fallthrough
        case 0:
            field_0x6d8 = 0.0f;
            field_0x98c.SetTgType(0xd8fbfdff);
            mMoveMode = 1;
            bckSet(BCK_SW_CHANCE, 5.0f, 2, 1.0f);
            speedF = 0.0f;
            gravity = -5.0f;
            field_0x6ea = 100;
            field_0x6f2 = 10;
            shape_angle.x = 0;
            break;

        case 1:
            if (mBgc.ChkGroundHit()) {
                mSound.startCreatureSound(Z2SE_EN_SW_BTBT, field_0x6e9, -1);
                mMoveMode = 2;
            }
            // fallthrough
        case 2:
            if ((field_0x6bc & 1) != 0 && mBgc.ChkGroundHit()) {
                field_0xae8 = dComIfGp_particle_set(field_0xae8, ZLM_SAND00_IA_6, &field_0x674, &tevStr, &shape_angle, NULL, 
                                                    0xFF, NULL, -1, NULL, NULL, NULL);
            }

            if (mpModelMorf->checkFrame(19.0f) || mpModelMorf->checkFrame(36.0F)) {
                mSound.startCreatureSound(Z2SE_EN_SW_BTBT, field_0x6e9, -1);
            }

            if (mpModelMorf->checkFrame(10.0f)) {
                speedF = 7.0f;
                sVar1 = cM_rndFX(32000.0f);

                if (field_0x6bc != 3) {
                    field_0x6ea = 0;
                    sVar1 = cLib_targetAngleY(&current.pos, &field_0x65c) + cM_rndFX(8000.0f);
                }

                current.angle.y = sVar1;
                shape_angle.y = sVar1;
            } else if (mpModelMorf->checkFrame(20.0f)) {
                speedF = 0.0f;
            }

            if (field_0x6ea == 0 && field_0x6bc == 3 && mBgc.ChkGroundHit()) {
                setActionMode(8, 0);
            }
            break;

        case 5:
            field_0x98c.SetTgType(0xd8fbfdff);
            bckSet(BCK_SW_DAMAGE, 3.0f, 2, 1.0f);
            mMoveMode = 6;
            speedF = 12.0f;
            speed.y = 35.0f;
            current.angle.y = mAtInfo.mHitDirection.y + 0x8000;
            break;

        case 6:
            if (mBgc.ChkGroundHit()) {
                mMoveMode = 0;
            }
            break;
    }
}

void daE_SW_c::executeDive() {
    s16 sVar1 = 0x800;
    f32 fVar1 = 14.0f;

    switch (mMoveMode) {
        case 0:
            if (field_0x6ea == 0) {
                bckSet(BCK_SW_DIVE, 5.0f, 0, 1.0f);
                mMoveMode = 1;
                setHideEffect();
            }
            break;

        case 1:
            if (mpModelMorf->checkFrame(15.0f)) {
                mSound.startCreatureSound(Z2SE_EN_SW_DIVE, field_0x6e9, -1);
            }

            if (mpModelMorf->checkFrame(20.0f)) {
                field_0x6e4 = 1;
            }

            if (mpModelMorf->isStop()) {
                field_0x6e5 = 1;
                setActionMode(0, 0);
                field_0x6d8 = -120.0f;
            }

            sVar1 = 0;
            fVar1 = 0.0f;
            break;
    }

    cLib_addCalc2(&speedF, fVar1, 1.0f, 10.0f);
    cLib_addCalcAngleS2(&current.angle.y, field_0x69e, 4, sVar1);
    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 8, 0x400);
}

void daE_SW_c::executeDamage() {
    s16 sVar1;

    field_0x6f2 = 10;

    if (mBgc.ChkWallHit()) {
        speedF = 0.0f;
        if (speed.y > 0.0f) {
            speed.y *= 0.5f;
        }
    }

    switch (mMoveMode) {
        case 0:
            field_0x6d8 = 0.0f;
            field_0x98c.OffTgSetBit();
            field_0x6e4 = 0;
            field_0x6e5 = 0;
            bckSet(BCK_SW_DAMAGE, 3.0f, 2, 1.0f);
            speed.y = cM_rndF(3.0f) + 48.0f;
            speedF = cM_rndF(3.0f) + 15.0f;
            mMoveMode = 1;
            gravity = -5.0f;
            sVar1 = mAtInfo.mHitDirection.y + 0x8000;
            shape_angle.y = sVar1;
            current.angle.y = sVar1;
            shape_angle.x = -0x8000;
            shape_angle.z = 0x4000;
            attention_info.flags = 0;
            break;

        case 1:
            if (mBgc.ChkGroundHit()) {
                speed.y = cM_rndF(3.0f) + 28.0f;
                speedF = cM_rndF(3.0f) + 8.0f;
                mMoveMode = 2;
            }

            shape_angle.x = shape_angle.x - 1500.0f;
            break;

        case 2:
            field_0xae8 = dComIfGp_particle_set(field_0xae8, ZLM_SAND00_IA_6, &field_0x674, &tevStr, &shape_angle, NULL,
                                                0xFF, NULL, -1, NULL, NULL, NULL);
            cLib_addCalcAngleS(&shape_angle.z, 0, 8, 0x800, 0x10);

            if (mBgc.ChkGroundHit()) {
                speed.y = 0.0f;
                speedF = 0.0f;
                setActionMode(10, 0);
                shape_angle.x = 0;
                shape_angle.z = 0;
                gravity = 0.0f;
                speed.y = 0.0f;
            }
            break;
    }
}

void daE_SW_c::executeDie() {
    switch (mMoveMode) {
        case 0:
            field_0x98c.SetTgType(0);
            field_0x684 = 0;

            if (mpModelMorf->checkFrame(2.0f)) {
                mMoveMode = 1;
                field_0x6d4 = 1.0f;
                field_0x6ea = 10;
            }
            break;

        case 1:
            cLib_addCalc2(&field_0x6b4, nREG_F(9) + -50.0f, nREG_F(8) + 1.0f, nREG_F(7) + 0.5f);

            if (field_0x6ea == 0) {
                mMoveMode = 2;
                bckSet(BCK_SW_DIE, 10.0f, 0, 1.0f);
                gravity = -1.0f;
                field_0x6ea = 30;
            } else {
                field_0x6d4 -= 0.1f;
                mpModelMorf->setPlaySpeed(field_0x6d4);
            }
            break;

        case 2:
            cLib_addCalc2(&field_0x6b4, nREG_F(9) + -50.0f, nREG_F(8) + 1.0f, nREG_F(7) + 0.5f);

            if (mpModelMorf->isStop() && field_0x6ea == 0) {
                data_807B0202++;

                if (data_807B0202 >= 10) {
                    data_807B0202 = 10;
                }

                fopAcM_createDisappear(this, &current.pos, 10, 0, 14);
                fopAcM_delete(this);
            }
            break;
    }
}

static void* s_child_sub(void* i_actor, void* i_data) {
    if (!fpcM_IsCreating(fopAcM_GetID(i_actor)) && fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_SW) {
        if (fopAcM_GetLinkId((fopAc_ac_c*)i_actor) == fopAcM_GetID(i_data)) {
            data_807B0201++;
        }
    }

    return NULL;
}

void daE_SW_c::executeMaster() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x100);
    attention_info.flags = 0;
    field_0x98c.OffTgSetBit();
    field_0x98c.OffCoSetBit();

    if (field_0x6ea == 0) {
        field_0x6ea = 30;
        if ((field_0x684 == 0 || !(field_0x6c8 < 1000.0f)) && !(field_0x6c8 > 10000.0f)) {
            int iVar1 = abs((s16)(fopCamM_GetAngleY(camera) - (fopAcM_searchPlayerAngleY(this) + 0x8000)));
            
            if (iVar1 < 0x2000) {
                data_807B0201 = 0;
                fpcM_Search(s_child_sub, this);

                if (data_807B0201 < field_0x695) {
                    iVar1 = 2;

                    if (data_807B0202 >= 4) {
                        if (cM_rnd() <= 0.1f) {
                            iVar1 = 1;
                        }
                    }

                    u32 param = (iVar1 << 24) | ((fopAcM_GetParam(this) & 0xFFF) | 0x3000);
                    fopAcM_createChild(PROC_E_SW, fopAcM_GetID(this), param,
                                       &home.pos, fopAcM_GetRoomNo(this), &shape_angle, NULL, -1, NULL);
                }
            }
        }
    }
}

bool daE_SW_c::checkSuddenAttack(int param_1) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 fVar1 = current.pos.absXZ(player->current.pos);

    if (field_0x6e7 != 0) {
        if (fVar1 > nREG_F(10) + 1500.0f && mActionMode == 0) {
            field_0x6e7 = 0;
        }

        if (param_1 != 0 && fVar1 < 300.0f && abs((s16)(player->shape_angle.y - (field_0x6cc - 0x8000))) < nREG_S(1) + 0x2000) {
            setActionMode(3, 0);
            return true;
        }
    } else {
        if (player->current.pos.absXZ(home.pos) < field_0x690 && fVar1 < nREG_F(10) + 1500.0f) {
            field_0x6e7 = 1;

            if (player->checkHorseRide() == 0) {
                fVar1 = 19.0f;
            } else {
                fVar1 = 30.0f;
            }

            if (player->getSpeedF() > fVar1 && abs((s16)(player->shape_angle.y - (field_0x6cc - 0x8000))) < nREG_S(1) + 0x1800) {
                setActionMode(12, 0);
                return true;
            }
        }
    }

    return false;
}

void daE_SW_c::executeSuddenAttack() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp1c(player->current.pos);
    cXyz sp28;

    setSmokeEffect();
    setBeforeJumpEffect();

    if (bomb_check()) {
        return;
    }

    if (home.pos.absXZ(sp1c) > field_0x690) {
        setActionMode(6, 0);
        return;
    }

    if (abs(player->shape_angle.y - shape_angle.y) < 0x4000 || abs(shape_angle.y - field_0x6cc) > 0x3000) {
        setActionMode(1, 0);
        return;
    }

    switch (mMoveMode) {
        case 0:
            current.angle.y = field_0x6cc;
            mMoveMode++;
            data_807B0200 = 1;
            field_0x6e6 = 1;
            // fallthrough
        case 1:
            cLib_chaseF(&speedF, 30.0f, 0.5f);
            cLib_addCalcAngleS2(&current.angle.y, field_0x6cc, 8, 0x800);

            if (field_0x6c8 < cM_rndFX(100.0f) + 1000.0f) {
                setActionMode(3, 0);
                field_0x69c = 2;

                if (field_0x6e6 != 0) {
                    data_807B0200 = 0;
                    field_0x6e6 = 0;
                }
            }
            break;
    }

    shape_angle.y = current.angle.y;
}

void daE_SW_c::executeFall() {
    switch (mMoveMode) {
        case 0:
            attention_info.flags = 0;
            field_0x684 = 0;
            field_0x98c.ClrTgHit();
            field_0x98c.OffTgSetBit();
            field_0x6ea = 90;
            mMoveMode = 1;
            field_0x6e5 = 0;
            field_0x6d8 = 0.0f;
            gravity = -3.0f;
            maxFallSpeed = -50.0f;
            // fallthrough
        case 1:
            cLib_chaseF(&speedF, 0.0f, 1.0f);

            if (field_0x6ea == 0) {
                bckSet(BCK_SW_RUN, 10.0f, 2, 1.0f);
                fopAcM_delete(this);
            }
            break;
    }

    shape_angle.x = cM_atan2s(speedF, speed.y) - 0x4000;
}

void daE_SW_c::action() {
    field_0x98c.OffAtSetBit();
    damage_check();

    if (mpModelMorf != NULL) {
        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&field_0x674);
    }

    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mBgc.CrrPos(dComIfG_Bgsp());
    checkFall();
    bool bVar1 = false;

    switch (mActionMode) {
        case ACTION_EXECUTE_WAIT:
            executeWait();
            break;

        case ACTION_EXECUTE_CHASE_SLOW:
            bVar1 = true;
            executeChaseSlow();
            break;

        case ACTION_EXECUTE_CHASE_FAST:
            bVar1 = true;
            executeChaseFast();
            break;

        case ACTION_EXECUTE_ATTACK:
            bVar1 = true;
            executeAttack();
            break;

        case ACTION_EXECUTE_BOMB:
            executeBomb();
            break;

        case ACTION_EXECUTE_HOOK:
            executeHook();
            break;

        case ACTION_EXECUTE_MOVE_OUT:
            executeMoveOut();
            break;

        case ACTION_EXECUTE_CHANCE:
            bVar1 = true;
            executeChance();
            break;

        case ACTION_EXECUTE_DIVE:
            executeDive();
            break;

        case ACTION_EXECUTE_DAMAGE:
            executeDamage();
            break;

        case ACTION_EXECUTE_DIE:
            executeDie();
            break;
            
        case ACTION_EXECUTE_MASTER:
            executeMaster();
            break;

        case ACTION_EXECUTE_SUDDEN_ATTACK:
            executeSuddenAttack();
            break;

        case ACTION_EXECUTE_FALL:
            executeFall();
            break;
    }

    mSound.setLinkSearch(bVar1);

    if (mpModelMorf != NULL) {
        mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
}

void daE_SW_c::mtx_set() {
    if (mpModelMorf != NULL) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y + field_0x6d8, current.pos.z);
        mDoMtx_stack_c::ZXYrotM(shape_angle);

        f32 fVar1 = field_0x6dc;
        
        if (l_HIO.basic_size != 1.0f) {
            fVar1 = l_HIO.basic_size;
        }

        mDoMtx_stack_c::scaleM(fVar1, fVar1, fVar1);
        mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        mpModelMorf->modelCalc();
    }
}

void daE_SW_c::cc_set() {
    if (mpModelMorf != NULL) {
        J3DModel* model = mpModelMorf->getModel();
        
        if (field_0x694 != 1) {
            MTXCopy(model->getAnmMtx(1), mDoMtx_stack_c::get());
            mDoMtx_stack_c::multVecZero(&eyePos);

            if (eyePos.y < mBgc.GetGroundH() + 50.0f) {
                eyePos.y = mBgc.GetGroundH() + 50.0f;
            }

            f32 fVar1 = field_0x6dc;

            if (l_HIO.basic_size != 1.0f) {
                fVar1 = l_HIO.basic_size;
            }

            fVar1 = fVar1 * 50.0f;

            if (mActionMode != ACTION_EXECUTE_CHANCE && field_0x98c.GetTgType() == (u32)0x4000 && fVar1 < 100.0f) {
                fVar1 = 100.0f;
            }

            field_0x98c.SetC(eyePos);
            field_0x98c.SetR(fVar1);
            dComIfG_Ccsp()->Set(&field_0x98c);
            attention_info.position = eyePos;
            attention_info.position.y += nREG_F(8) + 50.0f;

            fVar1 = mBgc.GetGroundH();
            if (attention_info.position.y < fVar1) {
                attention_info.position.y = fVar1;
            }

            if (eyePos.y < fVar1) {
                eyePos.y = fVar1;
            }
        }
    }
}

void daE_SW_c::damage_check() {
    mStts.Move();

    if (field_0x6f2 != 0 || mActionMode == ACTION_EXECUTE_DAMAGE || mActionMode == ACTION_EXECUTE_DIE) {
        return;
    }

    if (field_0x98c.ChkAtShieldHit()) {
        field_0x98c.OffAtShieldHit();
        
        if (daPy_getPlayerActorClass()->checkPlayerGuard()) {
            setActionMode(ACTION_EXECUTE_CHANCE, 0);
            cc_at_check(this, &mAtInfo);
            return;
        }
    }

    if (field_0x98c.ChkTgHit()) {
        mAtInfo.mpCollider = field_0x98c.GetTgHitObj();

        if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_40) || field_0x698 > 1) {
            health = 100;
        } else {
            health = 10;
        }

        cc_at_check(this, &mAtInfo);

        if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
            setActionMode(ACTION_EXECUTE_HOOK, 0);
            executeHook();
        } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK) || mAtInfo.mpCollider->ChkAtType(AT_TYPE_40) || mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
            setActionMode(ACTION_EXECUTE_CHANCE, 5);
        } else {
            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                field_0x6f2 = 20;
            } else {
                field_0x6f2 = 10;
            }

            if (mAtInfo.mAttackPower <= 1) {
                field_0x6f2 = KREG_S(8) + 10;
            }

            int cutType = daPy_getPlayerActorClass()->getCutType();

            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_NORMAL_SWORD) && (cutType == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT || cutType == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT ||
                cutType == daPy_py_c::CUT_TYPE_MORTAL_DRAW_B || cutType == daPy_py_c::CUT_TYPE_LARGE_JUMP_INIT || cutType == daPy_py_c::CUT_TYPE_LARGE_JUMP ||
                cutType == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH)) {
                field_0x698 = 0;
            }

            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_IRON_BALL)) {
                field_0x698 = 0;
            }

            field_0x698--;

            if (field_0x698 <= 0) {
                mSound.startCreatureSound(Z2SE_EN_SW_V_DEAD, field_0x6e9, -1);
                setActionMode(ACTION_EXECUTE_DAMAGE, 0);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_SW_V_DMG, -1);
            }
        }
    }
}

void daE_SW_c::checkGroundSand() {
    field_0x6bc = 0;
    cXyz sp1c(0.0f, 200.0f, 0.0f);
    cXyz sp28;

    cLib_offsetPos(&sp28, &current.pos, shape_angle.y, &sp1c);

    if (fopAcM_gc_c::gndCheck(&sp28) && fopAcM_gc_c::getPolyAtt0() == 3) {
        field_0x6bc |= 1;
    }

    sp1c.z = 100.0f;
    cLib_offsetPos(&sp28, &current.pos, shape_angle.y, &sp1c);

    if (fopAcM_gc_c::gndCheck(&sp28) && fopAcM_gc_c::getPolyAtt0() == 3) {
        field_0x6bc |= 2;
    }

    if (field_0x6bc == 3) {
        field_0x65c = current.pos;
    }
}

int daE_SW_c::execute() {
    if (field_0x694 == 0) {
        return d_execute();
    }

    field_0x6c8 = fopAcM_searchPlayerDistance(this);

    if (field_0x684 != 0 && field_0x6c8 > nREG_F(15) + 5000.0f) {
        return 1;
    }

    field_0x6cc = fopAcM_searchPlayerAngleY(this);
    field_0x6d0 = current.pos.abs(home.pos);

    if (field_0x6ea != 0) {
        field_0x6ea--;
    }

    if (field_0x6ec != 0) {
        field_0x6ec--;
    }

    if (field_0x6ee != 0) {
        field_0x6ee--;
    }

    if (field_0x6f0 != 0) {
        field_0x6f0--;
    }

    if (field_0x6f2 != 0) {
        field_0x6f2--;
    }

    if (field_0x6f6 != 0) {
        field_0x6f6--;
    }

    checkGroundSand();
    action();
    mtx_set();
    cc_set();

    if (mpModelMorf != NULL) {
        if (field_0x6e5 != 0) {
            if (daPy_getPlayerActorClass()->checkWolfLock(this)) {
                daPy_getPlayerActorClass()->cancelWolfLock(this);
            }

            onWolfNoLock();
        } else {
            offWolfNoLock();
        }

        cXyz i_effPos;
        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&i_effPos);
        cXyz i_effSize(1.0f, 1.0f, 1.5f);
        setMidnaBindEffect(this, &mSound, &i_effPos, &i_effSize);
    }

    return 1;
}

static int daE_SW_Execute(daE_SW_c* i_this) {
    return i_this->execute();
}

static int daE_SW_IsDelete(daE_SW_c* i_this) {
    return 1;
}

int daE_SW_c::_delete() {
    dComIfG_resDelete(&mPhase, "E_SW");

    if (field_0xaf9 != 0) {
        l_initHIO = 0;
    }

    if (field_0xaf8 != 0) {
        mSound.deleteObject();
    }

    return 1;
}

static int daE_SW_Delete(daE_SW_c* i_this) {
    return i_this->_delete();
}

int daE_SW_c::CreateHeap() {
    mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_SW", BMDR_SW), NULL, NULL,
                                       (J3DAnmTransform*)dComIfG_getObjectRes("E_SW", BCK_SW_WAIT), 0, 1.0f, 0, -1, &mSound, 0x80000, 0x11000084);
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    daE_SW_c* i_this = (daE_SW_c*)a_this;
    return i_this->CreateHeap();
}

cPhs__Step daE_SW_c::create() {
    fopAcM_ct(this, daE_SW_c);

    field_0x68c = fopAcM_GetParam(this) & 15;
    if (field_0x68c == 15) {
        field_0x68c = 0;
    }

    u32 uVar1 = (fopAcM_GetParam(this) >> 4) & 0xFF;
    if (uVar1 == 0xFF) {
        field_0x690 = G_CM3D_F_INF;
    } else {
        field_0x690 = uVar1 * 100.0f;
    }

    field_0x694 = (fopAcM_GetParam(this) >> 12) & 15;
    if (field_0x694 == 15) {
        field_0x694 = 0;
    }

    field_0x695 = fopAcM_GetParam(this) >> 16;
    if (field_0x695 == 0xFF || field_0x695 == 0) {
        field_0x695 = 1;
    }

    u8 uVar2 = fopAcM_GetParam(this) >> 24;
    if (uVar2 == 0xFF) {
        uVar2 = 0;
    }
    field_0x6e8 = uVar2;
    
    field_0xaf8 = 0;

    if (field_0x694 == 0) {
        field_0x696 = current.angle.x;

        if (field_0x696 != 0xFF && dComIfGs_isSwitch(field_0x696, fopAcM_GetRoomNo(this))) {
            OS_REPORT("E_SW やられ後なので再セットしません\n"); // Since it's after the E_SW has been executed, I won't reset it.
            return cPhs_ERROR_e;
        }
    } else {
        field_0x696 = 0xFF;
    }

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, "E_SW");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_SW PARAM %x %d %d\n", fopAcM_GetParam(this), field_0x696, fopAcM_GetID(this));
        shape_angle.x = 0;
        current.angle.x = 0;

        if (field_0x694 != 1) {
            if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x1400)) {
                return cPhs_ERROR_e;
            }
        }

        if (l_initHIO == 0) {
            field_0xaf9 = 1;
            l_initHIO = 1;
            l_HIO.field_0x4 = -1;
        }

        attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        if (mpModelMorf != NULL) {
            fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
        }

        fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);
        mBgc.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 2, field_0x6f8, fopAcM_GetSpeed_p(this), NULL, NULL);
        field_0x6f8[0].SetWall(10.0f, 50.0f);
        field_0x6f8[1].SetWall(-10.0f, 50.0f);
        health = 10;
        field_0x560 = 10;
        mStts.Init(100, 0, this);
        field_0x98c.Set(cc_sph_src);
        field_0x98c.SetStts(&mStts);
        field_0xaf8 = 1;

        mSound.init(&current.pos, &eyePos, 3, 1);
        mSound.setEnemyName("E_sw");
        mAtInfo.mpSound = &mSound;
        mAtInfo.mPowerType = 1;
        gravity = -5.0f;

        switch (field_0x6e8) {
            case 0:
                field_0x6dc = 1.0f;
                field_0x698 = 1;
                field_0x6e9 = 2;
                break;

            case 2:
                field_0x6dc = 0.5f;
                field_0x698 = 1;
                field_0x6e9 = 1;
                break;

            case 1:
                field_0x6dc = 1.5f;
                field_0x698 = 2;
                field_0x6e9 = 3;
                break;

            case 3:
                field_0x6dc = 3.0f;
                field_0x698 = 5;
                field_0x6e9 = 3;
                break;
        }

        field_0x6d8 = -120.0f;

        if (field_0x694 == 0) {
            mBgc.OnLineCheck();
            field_0x698 = 2;
            attention_info.distances[2] = 3;
            d_setAction(daE_SW_c::d_wait);
        } else {
            attention_info.distances[2] = 4;

            if (field_0x6dc > 1.0f) {
                field_0x98c.SetAtSpl((dCcG_At_Spl)1);
            } else {
                field_0x98c.SetAtSpl((dCcG_At_Spl)0);
            }

            if (field_0x694 == 1) {
                setActionMode(11, 0);
            }
        }

        daE_SW_Execute(this);
        field_0x684 = 1;
    }

    return phase;
}

static int daE_SW_Create(daE_SW_c* i_this) {
    return i_this->create();
}

void daE_SW_c::d_setAction(void (daE_SW_c::*action)()) {
    if (field_0xafc) {
        mMoveMode = -1;
        (this->*field_0xafc)();
    }

    field_0xb0c = field_0xafc;
    field_0xafc = action;
    mMoveMode = 0;
    (this->*field_0xafc)();
}

static actor_method_class l_daE_SW_Method = {
    (process_method_func)daE_SW_Create,
    (process_method_func)daE_SW_Delete,
    (process_method_func)daE_SW_Execute,
    (process_method_func)daE_SW_IsDelete,
    (process_method_func)daE_SW_Draw,
};

extern actor_process_profile_definition g_profile_E_SW = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_E_SW,              // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daE_SW_c),       // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    191,                    // mPriority
    &l_daE_SW_Method,       // sub_method
    0x000C0100,             // mStatus
    fopAc_ENEMY_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};

void daE_SW_c::d_checkFall() {
    if (d_checkAction(daE_SW_c::d_hook) || d_checkAction(daE_SW_c::d_fall) || d_checkAction(daE_SW_c::d_attk)) {
        return;
    }

    if (mBgc.ChkGroundHit()) {
        return;
    }

    dBgS_GndChk gnd_chk;
    cXyz i_pos(current.pos);
    i_pos.y += 100.0f;
    gnd_chk.SetPos(&i_pos);
    f32 fVar1 = dComIfG_Bgsp().GroundCross(&gnd_chk);

    if (speed.y < 0.0f) {
        if ((fVar1 == -G_CM3D_F_INF || std::abs(fVar1 - current.pos.y) > 1000.0f) || dComIfG_Bgsp().GetGroundCode(gnd_chk) == 4 || 
            dComIfG_Bgsp().GetGroundCode(gnd_chk) == 10 || dComIfG_Bgsp().GetGroundCode(gnd_chk) == 5) {
            d_setAction(daE_SW_c::d_fall);
        }
    }
}

void daE_SW_c::d_damage_check() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    mStts.Move();

    if (field_0x6f2 == 0) {
        if (d_checkAction(daE_SW_c::d_damage) || d_checkAction(daE_SW_c::d_die)) {
            return;
        }

        if (field_0x98c.ChkAtShieldHit()) {
            field_0x98c.OffAtShieldHit();

            if (player->checkPlayerGuard()) {
                d_setAction(daE_SW_c::d_chance);
                return;
            }
        }

        if (field_0x98c.ChkTgHit()) {
            mAtInfo.mpCollider = field_0x98c.GetTgHitObj();

            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_40)) {
                health = 100;
            } else if (field_0x698 > 1) {
                health = 100;
            } else {
                health = 10;
            }

            cc_at_check(this, &mAtInfo);

            if (!mAtInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT)) {
                if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                    field_0x6f2 = 20;
                } else {
                    field_0x6f2 = 10;
                }

                if (mAtInfo.mHitStatus != 0) {
                    field_0x6f2 = 10;
                }

                if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_NORMAL_SWORD) && (player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT ||
                    player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT || player->getCutType() == daPy_py_c::CUT_TYPE_MORTAL_DRAW_B ||
                    player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_JUMP_INIT || player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_JUMP ||
                    player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH)) {
                    health = 0;
                    field_0x698 = 0;
                    d_setAction(daE_SW_c::d_damage);
                } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_IRON_BALL) || mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB) ||
                           mAtInfo.mpCollider->ChkAtType(AT_TYPE_MIDNA_LOCK)) {
                    health = 0;
                    field_0x698 = 0;
                    d_setAction(daE_SW_c::d_damage);
                } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                    d_setAction(daE_SW_c::d_hook);
                } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK) || mAtInfo.mpCollider->ChkAtType(AT_TYPE_40) ||
                           mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                    if (!d_checkAction(daE_SW_c::d_chance2)) {
                        d_setAction(daE_SW_c::d_chance2);
                    }
                } else {
                    field_0x698--;
                    d_setAction(daE_SW_c::d_damage);
                }
            }
        }
    }
}

void daE_SW_c::d_action() {
    field_0x98c.OffAtSetBit();
    d_damage_check();

    if (field_0x6dc > 1.0f) {
        field_0x98c.SetAtSpl((dCcG_At_Spl)1);
    } else {
        field_0x98c.SetAtSpl((dCcG_At_Spl)0);
    }

    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mBgc.CrrPos(dComIfG_Bgsp());
    d_checkFall();
    checkGroundSand();
    (this->*field_0xafc)();
    mSound.setLinkSearch(field_0xb14);

    if (mpModelMorf != NULL) {
        mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&field_0x674);
    }
}

bool daE_SW_c::d_bomb_check() {
    if (field_0x68c != 0) {
        return false;
    }

    fopAc_ac_c* bomb = search_bomb();
    if (bomb != NULL && fopAcM_searchActorDistance(this, bomb) < l_HIO.bomb_notice_dist) {
        return true;
    }

    return false;
}

void daE_SW_c::d_bm_chase() {
    if (mMoveMode != -1) {
        dBomb_c* bomb = NULL;
        fopAcM_SearchByID(field_0x688, (fopAc_ac_c**)&bomb);

        if (bomb == NULL || bomb->checkStateExplode()) {
            d_setAction(daE_SW_c::d_wait);
        } else {
            switch (mMoveMode) {
                case 0:
                    bckSet(BCK_SW_RUN, 10.0f, 2, 1.0f);
                    mMoveMode++;
                    return;

                case 1:
                    if (bomb->speedF < 0.1f && current.pos.absXZ(bomb->current.pos) < l_HIO.bomb_attack_start_range) {
                        field_0x6ea = cM_rndF(30.0f);
                        mMoveMode++;
                    }
                    break;

                case 2:
                    if (field_0x6ea == 0) {
                        field_0x6f0 = 10;
                        mMoveMode++;
                    }
                    break;

                case 3:
                    if (field_0x6f0 != 0) {
                        break;
                    }

                    field_0x6a8 = bomb->current.pos;
                    d_setAction(daE_SW_c::d_attk);
                    return;

                default:
                    return;
            }

            setSmokeEffect();
            cLib_addCalc2(&speedF, 15.0f, 1.0f, 3.0f);

            s16 sVar1;
            if (bomb != NULL) {
                sVar1 = cLib_targetAngleY(&current.pos, &bomb->current.pos);
            }

            cLib_addCalcAngleS2(&current.angle.y, sVar1, 8, 0x400);
            shape_angle.y = current.angle.y;
        }
    }
}

void daE_SW_c::d_wait() {
    switch (mMoveMode) {
        case 0:
            field_0x98c.SetTgType(0x4000);
            field_0x6e4 = 1;
            field_0x6e5 = 1;
            bckSet(BCK_SW_RUN, 10.0f, 2, 1.0f);
            field_0x6ea = 50;
            field_0x6ec = cM_rndF(30.0f) + 30.0f;
            mMoveMode++;
            return;

        case 1:
            if ((field_0x6bc & 2) != 0) {
                if (field_0x6d0 > 1000.0f) {
                    field_0x69e = cLib_targetAngleY(&current.pos, &home.pos);
                } else {
                    field_0x69e = cM_rndFX(60000.0f);
                }
            } else {
                field_0x69e = cM_rndFX(30000.0f) + 32768.0f;
            }
            // fallthrough
        case 2:
            if (field_0x6ec == 0) {
                if (field_0x6f8[0].ChkWallHit()) {
                    field_0x6ec = 96;
                    field_0x69e = field_0x6f8[0].GetWallAngleY();
                } else if (field_0x6f8[1].ChkWallHit()) {
                    field_0x6ec = 96;
                    field_0x69e = field_0x6f8[1].GetWallAngleY();
                }
            }

            cLib_addCalcAngleS2(&current.angle.y, field_0x69e, 8, 0x400);

            if (field_0x6ea == 0) {
                field_0x6ea = 50;
                mMoveMode++;
            }

            cLib_chaseF(&speedF, 15.0f, 1.0f);
            break;

        case 3:
            if (field_0x6ea == 0) {
                mMoveMode = 1;

                if (field_0x6d0 > 1000.0f) {
                    field_0x6ea = 90;
                } else {
                    field_0x6ea = 30;
                }
            }

            if (field_0x6ec == 0) {
                if (field_0x6f8[0].ChkWallHit()) {
                    field_0x6ec = 96;
                    field_0x69e = field_0x6f8[0].GetWallAngleY();
                } else if (field_0x6f8[1].ChkWallHit()) {
                    field_0x6ec = 96;
                    field_0x69e = field_0x6f8[1].GetWallAngleY();
                }
            }

            cLib_addCalcAngleS2(&current.angle.y, field_0x69e, 8, 0x400);
            cLib_addCalc2(&speedF, 0.0f, 0.2f, 10.0f);
            break;

        default:
            return;
    }

    if (speedF > 2.0f) {
        setSmokeEffect();
    }

    shape_angle.y = current.angle.y;

    if (d_bomb_check()) {
        d_setAction(daE_SW_c::d_bm_chase);
    } else if (field_0x6ec == 0 && d_chaseCheck()) {
        d_setAction(daE_SW_c::d_chase);
    }
}

bool daE_SW_c::d_chaseCheck() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 fVar1 = current.pos.absXZ(player->current.pos);
    BOOL bVar1 = false;

    if (fVar1 >= l_HIO.character_notice_dist) {
        return false;
    }

    if (dComIfGp_getAttention()->LockonTruth()) {
        if (dComIfGp_getAttention()->LockonTarget(0) == this) {
            bVar1 = true;
        }
    }

    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    
    s16 sVar1 = cLib_distanceAngleS(fopCamM_GetAngleY(camera), field_0x6cc);
    if (bVar1 || (sVar1 > 0x6000 && (abs((s16)(current.angle.y - field_0x6cc)) < 0x2000 || 
        fVar1 < l_HIO.human_attack_init_range))) {
        return true;
    }

    return false;
}

void daE_SW_c::d_chase() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    s16 sVar1;

    switch (mMoveMode) {
        case 0:
            bckSet(BCK_SW_RUN, 10.0f, 2, 1.0f);
            field_0xb14 = true;
            field_0x6ea = 60;
            field_0x6ee = 240;
            mMoveMode++;
            return;

        case 1:
            setSmokeEffect();

            if (field_0x6ee == 0) {
                d_setAction(daE_SW_c::d_wait);
                return;
            }

            if (player->current.pos.absXZ(current.pos) >= l_HIO.character_notice_dist) {
                d_setAction(daE_SW_c::d_wait);
                return;
            }
            
            if (player->current.pos.absXZ(current.pos) < l_HIO.human_attack_init_range) {
                d_setAction(daE_SW_c::d_attk);
                return;
            }
                
            if (field_0x6ec == 0) {
                if (mBgc.ChkWallHit()) {
                    if (field_0x6f8[0].ChkWallHit()) {
                        sVar1 = field_0x6f8[0].GetWallAngleY();
                    } else if (field_0x6f8[1].ChkWallHit()) {
                        sVar1 = field_0x6f8[1].GetWallAngleY();
                    }

                    if (abs((s16)(current.angle.y - (sVar1 - 0x8000))) < 0x2000) {
                        field_0x69e = sVar1;
                        field_0x6ec = 60;
                    }
                } else {
                    field_0x69e = field_0x6cc;
                }
            }
            
            cLib_addCalcAngleS2(&current.angle.y, field_0x69e, 8, 0x400);
            cLib_chaseF(&speedF, 15.0f, 1.0f);
            break;

        default:
            return;
    }
    
    shape_angle.y = current.angle.y;

    if (field_0x6ea != 0) {
        setBeforeJumpEffect();
    }
}

void daE_SW_c::d_attk() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    s16 sVar1;

    switch (mMoveMode) {
        case 0:
            field_0x98c.SetTgType(0xd8fbfdff);
            field_0x6e4 = 0;
            field_0x6e5 = 0;
            setDiveEffect();
            mSound.startCreatureSound(Z2SE_EN_SW_JUMP_OUT, field_0x6e9, -1);
            bckSet(BCK_SW_ATTACK_MIDDLE, 0.0f, 0, 1.0f);
            shape_angle.x = -0x4000;
            field_0xb14 = true;
            sVar1 = field_0x69e;
            shape_angle.y = sVar1;
            current.angle.y = sVar1;
            gravity = -4.0f;

            if (d_checkOldAction(daE_SW_c::d_bm_chase)) {
                field_0x69e = cLib_targetAngleY(&current.pos, &field_0x6a8);
                speedF = current.pos.abs(field_0x6a8) / 25.0f;
            } else {
                field_0x69e = field_0x6cc;
                field_0x6c8 = player->current.pos.absXZ(current.pos);
                speedF = field_0x6c8 / 25.0f;
            }

            speed.y = 65.0f;
            mMoveMode++;
            return;

        case 1:
            setJumpEffect();
            cLib_chaseAngleS(&shape_angle.x, 0, 0x400);

            if (speed.y <= 0.0f) {
                field_0x6d8 = 0.0f;
                current.pos.y -= 120.0f;
                bckSet(BCK_SW_ATTACK_END, 10.0f, 0, 1.0f);
                field_0x6ea = 60;
                mMoveMode++;
            }
            break;

        case 2:
            setJumpEffect();
            cLib_chaseAngleS(&shape_angle.x, 0x4000, 0x400);
            cLib_chaseF(&speedF, 0.0f, 1.0f);

            if (mBgc.ChkGroundHit()) {
                gravity = -5.0f;
                field_0x6d8 = -60.0f;

                if ((field_0x6bc & 1) == 0) {
                    d_setAction(daE_SW_c::d_chance);
                } else {
                    field_0x6e4 = 1;
                    setDiveEffect();
                    mSound.startCreatureSound(Z2SE_EN_SW_JUMP_IN, field_0x6e9, -1);
                    mMoveMode++;
                }

                current.angle.y = 0;
            } else if (field_0x6ea == 0) {
                fopAcM_delete(this);
            }

            field_0x98c.OnAtSetBit();
            break;

        case 3:
            cLib_chaseF(&speedF, 0.0f, 1.0f);
            if (cLib_chaseF(&field_0x6d8, -120.0f, 60.0f) != 0) {
                field_0x6e5 = 1;
                shape_angle.x = 0;
                f32 fVar1;
                
                if (player->checkHorseRide() == 0) {
                    fVar1 = 19.0f;
                } else {
                    fVar1 = 30.0f;
                }

                if (player->getSpeedF() < fVar1) {
                    field_0x6ea = 15;
                }

                mMoveMode++;
            }
            break;

        case 4:
            if ((field_0x6bc & 3) == 0) {
                d_setAction(daE_SW_c::d_wait);
                current.pos = old.pos;
                break;
            } else {
                if (field_0x6ea != 0) {
                    cLib_chaseF(&speedF, 15.0f, 1.0f);
                    return;
                }

                d_setAction(daE_SW_c::d_wait);
                field_0x6d8 = -120.0f;
                cXyz sp30(current.pos);
                sp30.y += 200.0f;

                if (!fopAcM_gc_c::gndCheck(&sp30)) {
                    fopAcM_delete(this);
                    return;
                }
                
                current.pos.y = fopAcM_gc_c::getGroundY();
            }
            // fallthrough
        default:
            return;
    }

    cLib_addCalcAngleS2(&current.angle.y, field_0x69e, 4, 0x800);
    shape_angle.y = current.angle.y;
}

void daE_SW_c::d_chance() {
    s16 sVar1;

    switch (mMoveMode) {
        case 0:
            field_0xb14 = true;
            field_0x6d8 = 0.0f;
            field_0x98c.OnTgSetBit();
            field_0x98c.SetTgType(0xd8fbfdff);
            gravity = -5.0f;
            mMoveMode++;
            bckSet(BCK_SW_CHANCE, 5.0f, 2, 1.0f);
            speedF = 0.0f;

            if (d_checkOldAction(daE_SW_c::d_hook)) {
                field_0x6ea = 200;
            } else {
                field_0x6ea = 100;
            }

            field_0x6f2 = 10;
            shape_angle.x = 0;
            break;

        case 1:
            if (mBgc.ChkGroundHit()) {
                mSound.startCreatureSound(Z2SE_EN_SW_BTBT, field_0x6e9, -1);
                mMoveMode++;
            }
            break;

        case 2:
            if ((field_0x6bc & 1) != 0 && mBgc.ChkGroundHit()) {
                field_0xae8 = dComIfGp_particle_set(field_0xae8, ZLM_SAND00_IA_6, &field_0x674, &tevStr, &shape_angle,
                                                    NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
            }

            if (mpModelMorf->checkFrame(19.0f) || mpModelMorf->checkFrame(36.0f)) {
                mSound.startCreatureSound(Z2SE_EN_SW_BTBT, field_0x6e9, -1);
            }

            if (mpModelMorf->checkFrame(10.0f)) {
                speedF = 7.0f;
                sVar1 = cM_rndFX(32000.0f);

                if (field_0x6bc != 3) {
                    sVar1 = cLib_targetAngleY(&current.pos, &field_0x65c) + cM_rndFX(8000.0f);
                }

                current.angle.y = sVar1;
                shape_angle.y = sVar1;
            } else if (mpModelMorf->checkFrame(20.0f)) {
                speedF = 0.0f;
            }

            if (field_0x6ea == 0 && mBgc.ChkGroundHit() && field_0x6bc == 3) {
                d_setAction(daE_SW_c::d_dive);
            }
            break;
    }
}

void daE_SW_c::d_chance2() {
    switch (mMoveMode) {
        case 0:
            field_0x98c.SetTgType(0xd8fbfdff);
            bckSet(BCK_SW_DAMAGE, 3.0f, 2, 1.0f);
            speedF = 12.0f;
            speed.y = 35.0f;
            current.angle.y = fopAcM_searchPlayerAngleY(this) + 0x8000;
            mMoveMode++;
            break;

        case 1:
            if (mpModelMorf->checkFrame(19.0f) || mpModelMorf->checkFrame(36.0f)) {
                mSound.startCreatureSound(Z2SE_EN_SW_BTBT, field_0x6e9, -1);
            }

            if (mBgc.ChkGroundHit()) {
                mSound.startCreatureSound(Z2SE_EN_SW_BTBT, field_0x6e9, -1);
                d_setAction(daE_SW_c::d_chance);
            }
            break;
    }
}

void daE_SW_c::d_dive() {
    switch (mMoveMode) {
        case 0:
            if (field_0x6ea == 0) {
                bckSet(BCK_SW_DIVE, 5.0f, 0, 1.0f);
                setHideEffect();
                mMoveMode++;
            }
            return;

        case 1:
            if (mpModelMorf->checkFrame(15.0f)) {
                mSound.startCreatureSound(Z2SE_EN_SW_DIVE, field_0x6e9, -1);
            }

            if (mpModelMorf->checkFrame(20.0f)) {
                field_0x6e4 = 1;
            }

            if (!mpModelMorf->isStop()) {
                break;
            }

            field_0x6e5 = 1;
            field_0x6d8 = -120.0f;
            d_setAction(daE_SW_c::d_wait);
            break;

        default:
            return;
    }

    cLib_addCalc2(&speedF, 0.0f, 1.0f, 10.0f);
    cLib_addCalcAngleS2(&current.angle.y, field_0x69e, 4, 0);
    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 8, 0x400);
}

void daE_SW_c::d_damage() {
    s16 sVar1;

    switch (mMoveMode) {
        case 0:
            field_0x6d8 = 0.0f;
            field_0x98c.ClrTgHit();
            field_0x98c.OffTgSetBit();
            field_0x6e4 = 0;
            field_0x6e5 = 0;
            bckSet(BCK_SW_DAMAGE, 3.0f, 2, 1.0f);
            speed.y = cM_rndF(3.0f) + 48.0f;
            speedF = cM_rndF(3.0f) + 15.0f;
            gravity = -4.0f;
            sVar1 = mAtInfo.mHitDirection.y + 0x8000;
            shape_angle.y = sVar1;
            current.angle.y = sVar1;
            shape_angle.x = -0x8000;
            shape_angle.z = 0x4000;

            if (field_0x698 <= 0) {
                attention_info.flags = 0;
                mSound.startCreatureSound(Z2SE_EN_SW_V_DEAD, field_0x6e9, -1);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_SW_V_DMG, -1);
            }

            mMoveMode++;
            return;

        case 1:
            if (mBgc.ChkGroundHit()) {
                speed.y = cM_rndF(3.0f) + 28.0f;
                speedF = cM_rndF(3.0f) + 8.0f;
                mMoveMode++; 
            }

            shape_angle.x = (int)shape_angle.x - 1500.0f;
            break;

        case 2:
            field_0xae8 = dComIfGp_particle_set(field_0xae8, ZLM_SAND00_IA_6, &field_0x674, &tevStr, &shape_angle,
                                                NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
            cLib_addCalcAngleS(&shape_angle.z, 0, 8, 0x800, 0x10);
            
            if (!mBgc.ChkGroundHit()) {
                break;
            }

            speed.y = 0.0f;
            speedF = 0.0f;
            shape_angle.x = 0;
            shape_angle.z = 0;
            gravity = 0.0f;
            speed.y = 0.0f;

            if (field_0x698 <= 0) {
                d_setAction(daE_SW_c::d_die);
                return;
            }

            d_setAction(daE_SW_c::d_chance);
            return;

        default:
            gravity = -5.0f;
            return;
    }

    if (mBgc.ChkWallHit()) {
        speedF = 0.0f;
        
        if (speed.y > 0.0f) {
            speed.y *= 0.5f;
        }
    }
}

void daE_SW_c::d_die() {
    switch (mMoveMode) {
        case 0:
            field_0x98c.SetTgType(0);
            field_0x684 = 0;

            if (mpModelMorf->checkFrame(2.0f)) {
                field_0x6d4 = 1.0f;
                field_0x6ea = 10;
                mMoveMode++;
            }
            break;

        case 1:
            cLib_addCalc2(&field_0x6b4, nREG_F(9) + -50.0f, nREG_F(8) + 1.0f, nREG_F(7) + 0.5f);

            if (field_0x6ea == 0) {
                bckSet(BCK_SW_DIE, 10.0f, 0, 1.0f);
                gravity = -1.0f;
                field_0x6ea = 30;
                mMoveMode++;
            } else {
                field_0x6d4 -= 0.1f;
                mpModelMorf->setPlaySpeed(field_0x6d4);
            }
            break;

        case 2:
            cLib_addCalc2(&field_0x6b4, nREG_F(9) + -50.0f, nREG_F(8) + 1.0f, nREG_F(7) + 0.5f);

            if (mpModelMorf->isStop() && field_0x6ea == 0) {
                data_807B0202++;

                if (data_807B0202 >= 10) {
                    data_807B0202 = 10;
                }

                if (field_0x694 == 0) {
                    dComIfGs_onSwitch(field_0x696, fopAcM_GetRoomNo(this));
                }

                fopAcM_createDisappear(this, &current.pos, 10, 0, 14);
                fopAcM_delete(this);
            }
            break;
    }
}

void daE_SW_c::d_hook() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz spb4;

    switch (mMoveMode) {
        case 0:
            field_0x6e4 = 0;
            field_0x6e5 = 0;
            field_0x98c.SetTgType(0);
            bckSet(BCK_SW_DAMAGE, 1.0f, 2, 1.0f);
            field_0x6d8 = 0.0f;
            speed.y = 0.0f;
            speedF = 0.0f;
            gravity = 0.0f;
            field_0x668 = current.pos;
            mMoveMode++;
            mSound.startCreatureVoice(Z2SE_EN_SW_V_CATCH, -1);

            if ((s16)(field_0x6cc - shape_angle.y) < 0) {
                field_0x6a4 = -0x4000;
            } else {
                field_0x6a4 = 0x4000;
            }

            shape_angle.y = field_0x6cc + field_0x6a4;

            if (field_0x6e8 >= 1) {
                mDoMtx_stack_c::YrotS(shape_angle.y);

                if (field_0x6a4 < 0) {
                    mDoMtx_stack_c::transM(40.0f, 15.0f, 0.0f);
                } else {
                    mDoMtx_stack_c::transM(-40.0f, 15.0f, 0.0f);
                }

                mDoMtx_stack_c::multVecZero(&spb4);
            } else {
                spb4.set(0.0f, nREG_F(0) + 15.0f, 0.0f);
            }

            daPy_getPlayerActorClass()->setHookshotCarryOffset(fopAcM_GetID(this), &spb4);
            break;

        case 1:
            cLib_addCalcAngleS(&shape_angle.y, (s16)(field_0x6cc + field_0x6a4), 4, 0x1000, 0x100);
            current.angle.y = shape_angle.y;
            
            if (!fopAcM_CheckStatus(this, 0x100000)) {
                speed.y = 20.0f;
                speedF = -10.0f;
                gravity = -5.0f;
                field_0x6d8 = 0.0f;
                d_setAction(daE_SW_c::d_chance);
            }
            break;

        case -1:
            if (field_0x668.absXZ(current.pos) < 200.0f) {
                dBgS_LinChk lin_chk;
                lin_chk.Set(&field_0x668, &current.pos, NULL);

                if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                    cM3dGPla plane;
                    cXyz spc0;

                    dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
                    cXyz* planeNormal = plane.GetNP();
                    spc0 = lin_chk.GetCross();
                    spc0.x += planeNormal->x * 50.0f;
                    spc0.y += planeNormal->y * 50.0f;
                    spc0.z += planeNormal->z * 50.0f;
                    current.pos = spc0;
                }
            }
            break;
    }
}

void daE_SW_c::d_fall() {
    switch (mMoveMode) {
        case 0:
            field_0x684 = 0;
            field_0x6ea = 90;
            mMoveMode = 1;
            field_0x6e5 = 0;
            field_0x6d8 = 0.0f;
            gravity = -3.0f;
            maxFallSpeed = -50.0f;
            break;

        case 1:
            cLib_chaseF(&speedF, 0.0f, 1.0f);

            if (field_0x6ea == 0) {
                bckSet(BCK_SW_RUN, 10.0f, 2, 1.0f);
                fopAcM_delete(this);
            }
            break;
    }
}

int daE_SW_c::d_execute() {
    if (field_0x6ea != 0) {
        field_0x6ea--;
    }

    if (field_0x6ec != 0) {
        field_0x6ec--;
    }

    if (field_0x6ee != 0) {
        field_0x6ee--;
    }
    
    if (field_0x6f0 != 0) {
        field_0x6f0--;
    }
    
    if (field_0x6f2 != 0) {
        field_0x6f2--;
    }

    if (field_0x6f6 != 0) {
        field_0x6f6--;
    }

    field_0x6cc = fopAcM_searchPlayerAngleY(this);
    field_0x6d0 = current.pos.abs(home.pos);
    d_action();
    mtx_set();
    cc_set();

    if (mpModelMorf != NULL) {
        if (field_0x6e5 != 0) {
            if (daPy_getPlayerActorClass()->checkWolfLock(this)) {
                daPy_getPlayerActorClass()->cancelWolfLock(this);
            }

            onWolfNoLock();
        } else {
            offWolfNoLock();
        }

        cXyz i_effPos;
        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&i_effPos);
        cXyz i_effSize(1.0f, 1.0f, 1.5f);
        setMidnaBindEffect(this, &mSound, &i_effPos, &i_effSize);
    }

    return 1;
}

AUDIO_INSTANCES;
