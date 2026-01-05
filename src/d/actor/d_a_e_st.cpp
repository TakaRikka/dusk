/**
 * @file d_a_e_st.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_st.h"
#include "f_op/f_op_kankyo_mng.h"
#include "f_op/f_op_actor_enemy.h"
#include "Z2AudioLib/Z2Instances.h"

enum E_st_RES_File_ID {
    /* BCK */
    /* 0x04 */ BCK_ST_ATTACK = 0x4,
    /* 0x05 */ BCK_ST_ATTACK02,
    /* 0x06 */ BCK_ST_ATTACKA,
    /* 0x07 */ BCK_ST_ATTACKB,
    /* 0x08 */ BCK_ST_ATTACKC,
    /* 0x09 */ BCK_ST_DAMAGE,
    /* 0x0A */ BCK_ST_DAWNA,
    /* 0x0B */ BCK_ST_DAWNB,
    /* 0x0C */ BCK_ST_DAWNC,
    /* 0x0D */ BCK_ST_DEAD,
    /* 0x0E */ BCK_ST_DEADWAIT,
    /* 0x0F */ BCK_ST_DEATH,
    /* 0x10 */ BCK_ST_DEATHA,
    /* 0x11 */ BCK_ST_DEFENSE,
    /* 0x12 */ BCK_ST_HANG,
    /* 0x13 */ BCK_ST_HANG02,
    /* 0x14 */ BCK_ST_HANG_DOWN,
    /* 0x15 */ BCK_ST_HANG_SHOCK,
    /* 0x16 */ BCK_ST_HANG_UP,
    /* 0x17 */ BCK_ST_HANGATTACK,
    /* 0x18 */ BCK_ST_HANGDAMAGE,
    /* 0x19 */ BCK_ST_HITL,
    /* 0x1A */ BCK_ST_HITR,
    /* 0x1B */ BCK_ST_HUG,
    /* 0x1C */ BCK_ST_HUG_ATTACK,
    /* 0x1D */ BCK_ST_JUMP,
    /* 0x1E */ BCK_ST_LANDING,
    /* 0x1F */ BCK_ST_MOVE,
    /* 0x20 */ BCK_ST_STAND,
    /* 0x21 */ BCK_ST_TURN,
    /* 0x22 */ BCK_ST_WAIT,
    /* 0x23 */ BCK_ST_WAIT02,
    /* 0x24 */ BCK_ST_WATERDEAD,
    /* 0x25 */ BCK_ST_WINDDAMAGE,

    /* BMDR */
    /* 0x28 */ BMDR_ST = 0x28,
    /* 0x29 */ BMDR_ST2,
};

enum Joint {
    /* 0x00 */ JNT_WORLD_ROOT,
    /* 0x01 */ JNT_HEAD,
    /* 0x02 */ JNT_LLEGA_1,
    /* 0x03 */ JNT_LLEGA_2,
    /* 0x04 */ JNT_LLEGA_3,
    /* 0x05 */ JNT_LLEGB_1,
    /* 0x06 */ JNT_LLEGB_2,
    /* 0x07 */ JNT_LLEGB_3,
    /* 0x08 */ JNT_LLEGC_1,
    /* 0x09 */ JNT_LLEGC_2,
    /* 0x0A */ JNT_LLEGC_3,
    /* 0x0B */ JNT_LLEGD_1,
    /* 0x0C */ JNT_LLEGD_2,
    /* 0x0D */ JNT_LLEGD_3,
    /* 0x0E */ JNT_RLEGA_1,
    /* 0x0F */ JNT_RLEGA_2,
    /* 0x10 */ JNT_RLEGA_3,
    /* 0x11 */ JNT_RLEGB_1,
    /* 0x12 */ JNT_RLEGB_2,
    /* 0x13 */ JNT_RLEGB_3,
    /* 0x14 */ JNT_RLEGC_1,
    /* 0x15 */ JNT_RLEGC_2,
    /* 0x16 */ JNT_RLEGC_3,
    /* 0x17 */ JNT_RLEGD_1,
    /* 0x18 */ JNT_RLEGD_2,
    /* 0x19 */ JNT_RLEGD_3,
    /* 0x1A */ JNT_AGOL,
    /* 0x1B */ JNT_AGOR,
    /* 0x1C */ JNT_HANDL1,
    /* 0x1D */ JNT_HANDL2,
    /* 0x1E */ JNT_HANDR1,
    /* 0x1F */ JNT_HANDR2,
    /* 0x20 */ JNT_WAIST,
};

enum Action {
    /* 0x00 */ ACTION_WAIT,
    /* 0x01 */ ACTION_MOVE,
    /* 0x02 */ ACTION_PL_SEARCH,
    /* 0x03 */ ACTION_SHOOT,
    /* 0x0A */ ACTION_HANG = 0xA,
    /* 0x0B */ ACTION_HANG_SHOOT,
    /* 0x0C */ ACTION_HANG_DROP,
    /* 0x0D */ ACTION_HANG_2,
    /* 0x0E */ ACTION_HANG_2_SHOOT,
    /* 0x0F */ ACTION_JUMP_ATTACK,
    /* 0x13 */ ACTION_S_DAMAGE = 0x13,
    /* 0x14 */ ACTION_DAMAGE,
    /* 0x15 */ ACTION_WATER,
    /* 0x32 */ ACTION_G_NORMAL = 0x32,
    /* 0x33 */ ACTION_G_FIGHT,
    /* 0x34 */ ACTION_G_DEF,
    /* 0x35 */ ACTION_G_DAMAGE,
    /* 0x36 */ ACTION_G_S_DAMAGE,
    /* 0x37 */ ACTION_G_WIND,
    /* 0x38 */ ACTION_G_CHANCE,
    /* 0x39 */ ACTION_G_END,
    /* 0x64 */ ACTION_UNK_100 = 0x64,
};

enum Action_Phase {
    /* 0x0 */ PHASE_INIT,

    /* e_st_wait */
    /* 0x1 */ WAIT_PHASE_WAIT = 0x1,
    /* 0x2 */ WAIT_PHASE_TURN,

    /* e_st_move */
    /* 0x1 */ MOVE_PHASE_MOVE = 0x1,

    /* e_st_pl_search */
    /* 0x1 */ PL_SEARCH_MOVE_INIT = 0x1,
    /* 0x2 */ PL_SEARCH_MOVE,
    /* 0x3 */ PL_SEARCH_ATTACK02,
    /* 0x5 */ PL_SEARCH_WAIT02 = 0x5,

    /* e_st_shoot */
    /* 0x1 */ SHOOT_PHASE_ATTACK = 0x1,
    /* 0x2 */ SHOOT_PHASE_WAIT02,

    /* e_st_jump_attack */
    /* 0x1 */ JUMP_ATTACK_PHASE_JUMP = 0x1,
    /* 0x2 */ JUMP_ATTACK_PHASE_HUG,
    /* 0x3 */ JUMP_ATTACK_PHASE_3,
    /* 0x4 */ JUMP_ATTACK_PHASE_HUG_ATTACK,
    /* 0x5 */ JUMP_ATTACK_PHASE_JUMPBACK,
    /* 0x6 */ JUMP_ATTACK_PHASE_LANDING,

    /* e_st_hang */
    /* 0x1 */ HANG_PHASE_HANG = 0x1,

    /* e_st_hang_shoot */
    /* 0x1 */ HANG_SHOOT_PHASE_HANG = 0x1,
    /* 0x2 */ HANG_SHOOT_PHASE_HANGATTACK,

    /* e_st_hang_drop */
    /* 0x1 */ HANG_DROP_PHASE_WAIT = 0x1,
    /* 0x2 */ HANG_DROP_PHASE_LANDING,

    /* e_st_hang_2 */
    /* 0x1 */ HANG_2_PHASE_HANG = 0x1,
    /* 0x2 */ HANG_2_PHASE_2,
    /* 0x3 */ HANG_2_PHASE_HANG02,
    /* 0x4 */ HANG_2_PHASE_SHOCK,

    /* e_st_hang_2_shoot */
    /* 0x1 */ HANG_2_SHOOT_PHASE_HANGATTACK = 0x1,

    /* e_st_s_damage */
    /* 0x1 */ S_DAMAGE_PHASE_DAMAGE = 0x1,

    /* e_st_damage */
    /* 0x1 */ DAMAGE_PHASE_HANGDAMAGE = 0x1,
    /* 0x2 */ DAMAGE_PHASE_DEADWAIT,
    /* 0x3 */ DAMAGE_PHASE_3,
    /* 0x4 */ DAMAGE_PHASE_4,
    /* 0x5 */ DAMAGE_PHASE_DEAD,

    /* e_st_water */
    /* 0x1 */ WATER_PHASE_DEAD = 0x1,

    /* e_st_g_normal */
    /* 0x1 */ G_NORMAL_PHASE_WAIT = 0x1,
    /* 0x2 */ G_NORMAL_PHASE_TURN,
    /* 0x3 */ G_NORMAL_PHASE_MOVE,

    /* e_st_g_fight */
    /* 0x1 */ G_FIGHT_PHASE_MOVE = 0x1,
    /* 0x2 */ G_FIGHT_PHASE_WAIT02,
    /* 0x3 */ G_FIGHT_PHASE_ATTACKA,
    /* 0x4 */ G_FIGHT_PHASE_ATTACKB,
    /* 0x5 */ G_FIGHT_PHASE_ATTACKC,

    /* e_st_g_def */
    /* 0x1 */ G_DEF_PHASE_DEFENSE = 0x1,

    /* e_st_g_damage */
    /* 0x1 */ G_DAMAGE_PHASE_HIT = 0x1,

    /* e_st_g_s_damage */
    /* 0x1 */ G_S_DAMAGE_PHASE_NODAMAGE = 0x1,

    /* e_st_g_wind */
    /* 0x1 */ G_WIND_PHASE_WINDDAMAGE = 0x1,

    /* e_st_g_chance */
    /* 0x1 */ G_CHANCE_PHASE_DAWNA = 0x1,
    /* 0x2 */ G_CHANCE_PHASE_DAWNB,
    /* 0x3 */ G_CHANCE_PHASE_STAND,

    /* e_st_g_end */
    /* 0x2 */ G_END_PHASE_DEATH2 = 0x2,
    /* 0xA */ G_END_PHASE_10 = 0xA,
    /* 0xB */ G_END_PHASE_DEATHA,
};

class daE_ST_HIO_c : public JORReflexible {
public:
    daE_ST_HIO_c();
    virtual ~daE_ST_HIO_c() {}

    void genMessage(JORMContext*);

    /* スタルチュラ - Skulltula */

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 basic_size;          // 基本サイズ - Basic Size
    /* 0x0C */ f32 pl_recognize_dist;   // PL認識距離 - PL Recognition Distance
    /* 0x10 */ f32 combat_start_dist;   // 戦闘開始距離 - Combat Start Distance
    /* 0x14 */ s16 time_before_attack;  // 攻撃前間 - Time Before Attack
    /* 0x16 */ s16 down_time;           // ダウン時間 - Down Time
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20;
};

daE_ST_HIO_c::daE_ST_HIO_c() {
    id = -1;
    basic_size = 1.7f;
    pl_recognize_dist = 1200.0f;
    combat_start_dist = 200.0f;
    time_before_attack = 0;
    down_time = 80;
    field_0x18 = 1.0f;
    field_0x1c = -0.5f;
    field_0x20 = -1.0f;
}

static void anm_init(e_st_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_st", i_index), i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->mAnm = i_index;
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        int jntNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_st_class* i_this = (e_st_class*)model->getUserArea();

        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);

            if (jntNo >= JNT_LLEGA_1 && jntNo <= JNT_LLEGA_3) {
                cMtx_ZrotM(*calc_mtx, i_this->mStFeet[0].mAngles[jntNo - 2]);
            } else if (jntNo >= JNT_LLEGB_1 && jntNo <= JNT_LLEGB_3) {
                cMtx_ZrotM(*calc_mtx, i_this->mStFeet[0].mAngles[jntNo + 7]);
            } else if (jntNo >= JNT_LLEGC_1 && jntNo <= JNT_LLEGC_3) {
                cMtx_ZrotM(*calc_mtx, i_this->mStFeet[1].mAngles[jntNo + 4]);
            } else if (jntNo >= JNT_LLEGD_1 && jntNo <= JNT_LLEGD_3) {
                cMtx_ZrotM(*calc_mtx, i_this->mStFeet[2].mAngles[jntNo + 1]);
            } else if (jntNo >= JNT_RLEGA_1 && jntNo <= JNT_RLEGA_3) {
                cMtx_ZrotM(*calc_mtx, i_this->mStFeet[3].mAngles[jntNo - 2]);
            } else if (jntNo >= JNT_RLEGB_1 && jntNo <= JNT_RLEGB_3) {
                cMtx_ZrotM(*calc_mtx, i_this->mStFeet[3].mAngles[jntNo + 7]);
            } else if (jntNo >= JNT_RLEGC_1 && jntNo <= JNT_RLEGC_3) {
                cMtx_ZrotM(*calc_mtx, i_this->mStFeet[4].mAngles[jntNo + 4]);
            } else if (jntNo >= JNT_RLEGD_1 && jntNo <= JNT_RLEGD_3) {
                cMtx_ZrotM(*calc_mtx, i_this->mStFeet[5].mAngles[jntNo + 1]);
            }

            if (jntNo == TREG_S(0)) {
                cMtx_XrotM(*calc_mtx, TREG_S(1));
                cMtx_XrotM(*calc_mtx, TREG_S(2));
                cMtx_ZrotM(*calc_mtx, TREG_S(3));
            } else if (jntNo == TREG_S(4)) {
                cMtx_XrotM(*calc_mtx, TREG_S(5));
                cMtx_XrotM(*calc_mtx, TREG_S(6));
                cMtx_ZrotM(*calc_mtx, TREG_S(7));
            }

            model->setAnmMtx(jntNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static BOOL small;

static bool hio_set;

static daE_ST_HIO_c l_HIO;

static int daE_ST_Draw(e_st_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    J3DModel* model = i_this->mpModelMorf->getModel();

    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);

    if (i_this->mDeathFlag != 0) {
        J3DModelData* modelData = model->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* matNodeP = modelData->getMaterialNodePointer(i);
            matNodeP->getTevColor(0)->r = i_this->mColor;
            matNodeP->getTevColor(0)->g = i_this->mColor;
            matNodeP->getTevColor(0)->b = i_this->mColor;
        }
    }

    i_this->mpModelMorf->entryDL();
    cXyz pos;
    pos.set(a_this->current.pos.x, a_this->current.pos.y + 400.0f + BREG_F(18), a_this->current.pos.z);
    if (fopAcM_gc_c::gndCheck(&pos)) {
        pos.y = TREG_F(7) + fopAcM_gc_c::getGroundY();
        i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &pos, l_HIO.basic_size * 700.0f, 0.0f,
                                                a_this->current.pos.y, fopAcM_gc_c::getGroundY(), *fopAcM_gc_c::getGroundCheck(),
                                                &a_this->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    if (i_this->field_0x720 != 0) {
        u8 uVar1 = JREG_S(5) + 180;
        GXColor color = {uVar1, uVar1, uVar1, 0xFF};
        i_this->mLineMat1.update(20, color, &a_this->tevStr);
        dComIfGd_set3DlineMat(&i_this->mLineMat1);

        if (i_this->field_0x760 != 0) {
            GXColor color2 = {uVar1, uVar1, uVar1, 0xFF};
            i_this->mLineMat2.update(20, color2, &a_this->tevStr);
            dComIfGd_set3DlineMat(&i_this->mLineMat2);
        }
    }

    return 1;
}

static BOOL other_bg_check(e_st_class* i_this, fopAc_ac_c* actor_p) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz pos, start, end;
    
    end = actor_p->current.pos;
    end.y += 150.0f;
    cMtx_YrotS(*calc_mtx, i_this->field_0x69c.y);
    cMtx_XrotM(*calc_mtx, i_this->field_0x69c.x);
    pos.x = 0.0f;
    pos.y = 100.0f;
    pos.z = 0.0f;
    MtxPosition(&pos, &start);
    start += a_this->current.pos;
    lin_chk.Set(&start, &end, a_this);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return TRUE;
    }

    return FALSE;
}

static BOOL pl_check(e_st_class* i_this, f32 i_distance) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz pos_delta, pos;

    if (player->current.pos.y - a_this->current.pos.y >= 750.0f || dComIfGp_event_runCheck()) {
        return FALSE;
    }
        
    if (i_this->mPlayerDistance < i_distance) {
        player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
        pos_delta = player->current.pos - a_this->current.pos;
        cMtx_XrotS(*calc_mtx, -i_this->field_0x69c.x);
        cMtx_YrotM(*calc_mtx, -i_this->field_0x69c.y);
        MtxPosition(&pos_delta, &pos);
        s16 angle = a_this->current.angle.y - cM_atan2s(pos.x, pos.z);

        if (angle < 0x6000 && angle > -0x6000 && !other_bg_check(i_this, player)) {
            return TRUE;
        }
    }

    return FALSE;
}

static void damage_check(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    i_this->mStts.Move();

    if (i_this->mInvulnerabilityTimer != 0) {
        return;
    }

    if ((i_this->mAction == ACTION_HANG || i_this->mAction == ACTION_HANG_2 || i_this->mAction == ACTION_HANG_SHOOT) && i_this->mAtSph.ChkAtHit()) {
        i_this->field_0x7ec = JREG_F(8) + 40.0f + 160.0f;
        i_this->field_0x7f0 = 0;
        i_this->field_0x7f2 = player->shape_angle.y + 0x8000;
    }

    if (i_this->mSph.ChkTgHit()) {
        i_this->mAtInfo.mpCollider = i_this->mSph.GetTgHitObj();
        cc_at_check(a_this, &i_this->mAtInfo);

        if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
            i_this->mInvulnerabilityTimer = 20;
        } else {
            i_this->mInvulnerabilityTimer = 10;
        }

        if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_THROW_OBJ) && small) {
            i_this->mSound.startCreatureSound(Z2SE_EN_ST_SHELL_BREAK, 0, -1);
            OS_REPORT("E_st DAM %d\n", i_this->mAtInfo.mAttackPower);
        }

        if (i_this->mAtInfo.mHitType == 16) {
            if (i_this->field_0x720 == 1) {
                i_this->mAction = ACTION_HANG_DROP;
                i_this->mActionPhase = PHASE_INIT;
                i_this->field_0x750 = i_this->field_0x710;
                i_this->field_0x720 = 2;
                i_this->field_0x75c = (BREG_F(3) + 0.002f) * (i_this->field_0x744.y - i_this->field_0x750.y);
            } else if (i_this->field_0x69c.x == 0) {
                i_this->mAction = ACTION_S_DAMAGE;
                i_this->mActionPhase = PHASE_INIT;
                i_this->mHitDirectionY = i_this->mAtInfo.mHitDirection.y;
                i_this->field_0x7e0 = YREG_F(8) + 10.0f;

                if (i_this->field_0x75c > 2.0f) {
                    i_this->field_0x75c = 2.0f;
                }
            } else {
                i_this->mAction = ACTION_HANG_DROP;
                i_this->mActionPhase = PHASE_INIT;
            }
        } else if (a_this->health > 0) {
            if (i_this->field_0x720 == 1) {
                i_this->mAction = ACTION_HANG_DROP;
                i_this->mActionPhase = PHASE_INIT;
                i_this->field_0x750 = i_this->field_0x710;
                i_this->field_0x720 = 2;
                i_this->field_0x75c = (BREG_F(3) + 0.002f) * (i_this->field_0x744.y - i_this->field_0x750.y);
            } else if (i_this->field_0x69c.x == 0) {
                i_this->mAction = ACTION_S_DAMAGE;
                i_this->mActionPhase = PHASE_INIT;
                i_this->mHitDirectionY = i_this->mAtInfo.mHitDirection.y;
                i_this->field_0x7e0 = YREG_F(9) + 30.0f;

                if (i_this->field_0x75c > 2.0f) {
                    i_this->field_0x75c = 2.0f;
                }

                if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP && player->checkCutJumpCancelTurn()) {
                    i_this->mInvulnerabilityTimer = 3;
                    i_this->field_0x7e0 = 10.0f;
                }
            } else {
                i_this->mAction = ACTION_HANG_DROP;
                i_this->mActionPhase = PHASE_INIT;
            }
        } else {
            if (i_this->mAtInfo.mHitType == 1 && (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_TURN_RIGHT || daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_UNK_9)) {
                i_this->field_0x7d4 = cM_rndF(1000.0f) + 4000.0f;
            } else {
                i_this->field_0x7d4 = cM_rndFX(2000.0f);
            }

            i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_DEATH, -1);
            daPy_getPlayerActorClass()->onEnemyDead();
            i_this->mDeathFlag = 1;
            i_this->mAction = ACTION_DAMAGE;
            i_this->mActionPhase = PHASE_INIT;
            i_this->mHitDirectionY = i_this->mAtInfo.mHitDirection.y + (s16)cM_rndFX(8000.0f);

            if (i_this->field_0x720 == 1) {
                i_this->field_0x750 = i_this->field_0x710;
                i_this->field_0x720 = 2;
                i_this->field_0x75c = (BREG_F(3) + 0.002f) * (i_this->field_0x744.y - i_this->field_0x750.y);

                if (i_this->field_0x75c > 2.0f) {
                    i_this->field_0x75c = 2.0f;
                }
            }
        }
    }

    if (a_this->health <= 10) {
        i_this->mSph.SetTgHitMark(CcG_Tg_UNK_MARK_3);
    }
}

static int move_bg_check(e_st_class* i_this) {
    dBgS_LinChk lin_chk;
    cXyz spa0, spac;

    int rv = 0;
    lin_chk.Set(&i_this->field_0x6bc, &i_this->field_0x6c8, NULL);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        rv = 1;
    }

    lin_chk.Set(&i_this->field_0x6c8, &i_this->field_0x6d4, NULL);
    if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
        rv |= 2;
    } else if (i_this->arg1 == 3) {
        if (dComIfG_Bgsp().GetSpecialCode(lin_chk) != 3) {
            rv |= 2;
        }
    }

    for (int i = 0; i < 3; i++) {
        lin_chk.Set(&i_this->field_0x6bc, &i_this->field_0x6e0[i], NULL);

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            cXyz spb8(i_this->field_0x6bc - i_this->field_0x6e0[i]);
            cMtx_YrotS(*calc_mtx, cM_atan2s(spb8.x, spb8.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(spb8.y, JMAFastSqrt(spb8.x * spb8.x + spb8.z * spb8.z)));
            spb8.x = 0.0f;
            spb8.y = 0.0f;
            spb8.z = TREG_F(11) + 5.0f;
            MtxPosition(&spb8, &spac);
            i_this->mBgPos += spac;
            rv |= 4;
        }
    }

    return rv;
}

static void bg_angle_set(e_st_class* i_this, s16 i_maxStep) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz sp90, sp9c, spa8, spb4, spc0;

    MtxTrans(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z, 0);
    cMtx_YrotM(*calc_mtx, i_this->field_0x69c.y);
    cMtx_XrotM(*calc_mtx, i_this->field_0x69c.x);
    MtxPush();
    
    sp90.x = 0.0f;
    sp90.y = 150.0f;
    sp90.z = 100.0f;
    MtxPosition(&sp90, &spa8);
    sp90.y = -300.0f;
    MtxPosition(&sp90, &spb4);
    lin_chk.Set(&spa8, &spb4, NULL);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        spc0 = lin_chk.GetCross();
        sp90 = spc0 - a_this->current.pos;
        cMtx_XrotS(*calc_mtx, -i_this->field_0x69c.x);
        cMtx_YrotM(*calc_mtx, -i_this->field_0x69c.y);
        MtxPosition(&sp90, &sp9c);
        cLib_addCalcAngleS2(&i_this->field_0x6a4, -cM_atan2s(sp9c.y, sp9c.z), 4, i_maxStep);
    }

    MtxPull();
    sp90.x = 100.0f;
    sp90.y = 150.0f;
    sp90.z = 0.0f;
    MtxPosition(&sp90, &spa8);
    sp90.y = -300.0f;
    MtxPosition(&sp90, &spb4);
    lin_chk.Set(&spa8, &spb4, NULL);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        spc0 = lin_chk.GetCross();
        sp90 = spc0 - a_this->current.pos;
        cMtx_XrotS(*calc_mtx, -i_this->field_0x69c.x);
        cMtx_YrotM(*calc_mtx, -i_this->field_0x69c.y);
        MtxPosition(&sp90, &sp9c);
        cLib_addCalcAngleS2(&i_this->field_0x6a2, (s16)cM_atan2s(sp9c.y, sp9c.x), 4, i_maxStep);
    }
}

static BOOL bg_pos_get(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz pos, end;

    cMtx_YrotS(*calc_mtx, i_this->field_0x69c.y);
    cMtx_XrotM(*calc_mtx, i_this->field_0x69c.x);
    pos.x = 0.0f;
    pos.y = -400.0f;
    pos.z = 0.0f;
    MtxPosition(&pos, &end);
    end += i_this->mBgPos;
    lin_chk.Set(&i_this->mBgPos, &end, NULL);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        a_this->current.pos = lin_chk.GetCross();
        pos.y = 200.0f;
        MtxPosition(&pos, &end);
        i_this->mBgPos = a_this->current.pos + end;
        bg_angle_set(i_this, 0x400);
    } else {
        return FALSE;
    }

    return TRUE;
}

static s16 next_angle_get(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp20, sp2c;

    sp20 = a_this->home.pos - a_this->current.pos;
    cMtx_XrotS(*calc_mtx, -i_this->field_0x69c.x);
    cMtx_YrotM(*calc_mtx, -i_this->field_0x69c.y);
    MtxPosition(&sp20, &sp2c);
    return cM_atan2s(sp2c.x, sp2c.z);
}

static void move_calc(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp1c, sp28;

    cMtx_YrotS(*calc_mtx, i_this->field_0x69c.y);
    cMtx_XrotM(*calc_mtx, i_this->field_0x69c.x);
    cMtx_YrotM(*calc_mtx, a_this->current.angle.y);
    sp1c.x = 0.0f;
    sp1c.y = TREG_F(8);
    sp1c.z = a_this->speedF;
    MtxPosition(&sp1c, &sp28);
    a_this->old.pos = i_this->mBgPos;
    i_this->mBgPos += sp28;

    if (!bg_pos_get(i_this)) {
        i_this->mBgPos = a_this->old.pos;
        i_this->mAction = ACTION_WAIT;
        i_this->mActionPhase = PHASE_INIT;
        OS_REPORT("/////// SORRY.. E_ST CANNOT MOVE..\n");
    }
}

static void e_st_wait(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mActionPhase = WAIT_PHASE_WAIT;
            anm_init(i_this, BCK_ST_WAIT, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimers[0] = cM_rndF(100.0f) + 50.0f;
            i_this->mTimers[1] = cM_rndF(30.0f) + 30.0f;
            fopAcM_SetParam(a_this, 0);
            i_this->field_0x6b0.y = a_this->current.pos.y;
            break;
        
        case WAIT_PHASE_WAIT:
            if ((i_this->mFrameCounter & 0xF) == 0 && cM_rndF(1.0f) < 0.5f) {
                i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_WAIT, -1);
            }

            if (i_this->mTimers[0] == 0) {
                i_this->mAngleFromPlayer = next_angle_get(i_this) + (s16)cM_rndFX(4000.0f);
                i_this->mActionPhase = WAIT_PHASE_TURN;
                anm_init(i_this, BCK_ST_TURN, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;

        case WAIT_PHASE_TURN:
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mAngleFromPlayer, 2, 0x100);
            s16 angle_delta = a_this->current.angle.y - i_this->mAngleFromPlayer;
            if (angle_delta < 0x10 && angle_delta > - 0x10) {
                i_this->mAction = ACTION_MOVE;
                i_this->mActionPhase = PHASE_INIT;
            }
            break;
    }

    if (i_this->mTimers[1] == 0 && pl_check(i_this, l_HIO.pl_recognize_dist)) {
        i_this->mAction = ACTION_PL_SEARCH;
        i_this->mActionPhase = PHASE_INIT;
        i_this->mTimers[0] = 0;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 0.5f);

    if (move_bg_check(i_this)) {
        a_this->speedF = 0.0f;
    }
    
    move_calc(i_this);
}

static s16 pl_angle_get(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz pos_delta, pos;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);

    pos_delta = player->current.pos - a_this->current.pos;
    cMtx_XrotS(*calc_mtx, -i_this->field_0x69c.x);
    cMtx_YrotM(*calc_mtx, -i_this->field_0x69c.y);
    MtxPosition(&pos_delta, &pos);
    return cM_atan2s(pos.x, pos.z);
}

static void e_st_move(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp2c, sp38;
    s8 isWait = false;
    
    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            anm_init(i_this, BCK_ST_MOVE, 9.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mActionPhase = MOVE_PHASE_MOVE;
            i_this->mTimers[0] = cM_rndF(30.0f) + 30.0f;
            i_this->field_0x6b0.y = a_this->current.pos.y;
            // fallthrough
        case MOVE_PHASE_MOVE:
            if (move_bg_check(i_this)) {
                a_this->speedF = 0.0f;
                isWait = true;
            }

            if (i_this->mTimers[0] == 0) {
                isWait = true;
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, 4.0f, 1.0f, 0.5f);
    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mAngleFromPlayer, 8, 0x100);

    if (isWait) {
        i_this->mAction = ACTION_WAIT;
        i_this->mActionPhase = PHASE_INIT;
    } else if (pl_check(i_this, l_HIO.pl_recognize_dist)) {
        i_this->mAction = ACTION_PL_SEARCH;
        i_this->mActionPhase = PL_SEARCH_MOVE_INIT;
        i_this->mTimers[0] = 0;
    }

    move_calc(i_this);

    if (i_this->field_0x7e0 > 0.1f) {
        sp2c.x = 0.0f;
        sp2c.y = 0.0f;
        sp2c.z = i_this->field_0x7e0;
        cMtx_YrotS(*calc_mtx, i_this->mHitDirectionY);
        MtxPosition(&sp2c, &sp38);
        a_this->current.pos += sp38;
        i_this->mBgPos.x = a_this->current.pos.x;
        i_this->mBgPos.z = a_this->current.pos.z;
        cLib_addCalc0(&i_this->field_0x7e0, 1.0f, TREG_F(12) + 7.0f);
        i_this->mBgc.CrrPos(dComIfG_Bgsp());
    }
}

static void e_st_pl_search(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz pos_delta, sp48;
    int frame = i_this->mpModelMorf->getFrame();
    f32 target = 0.0f;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            if (i_this->mTimers[0] == 0) {
                if (!pl_check(i_this, AREG_F(0) + 350.0f) && daPy_getPlayerActorClass()->getStCaught()) {
                    anm_init(i_this, BCK_ST_WAIT02, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                    a_this->speedF = 0.0f;
                    i_this->mActionPhase = PL_SEARCH_WAIT02;
                } else {
                    anm_init(i_this, BCK_ST_MOVE, 6.0f, J3DFrameCtrl::EMode_LOOP, 2.0f);
                    i_this->mActionPhase = PL_SEARCH_MOVE_INIT;
                }

                i_this->field_0x6b0.y = a_this->current.pos.y;
            }
            break;
        
        case PL_SEARCH_MOVE_INIT:
            i_this->mActionPhase = PL_SEARCH_MOVE;
            i_this->mTimers[0] = cM_rndF(10.0f) + 10.0f;
            i_this->mpModelMorf->setPlaySpeed(2.0f);
            // fallthrough
        case PL_SEARCH_MOVE:
            target = 8.0f;
            i_this->mAngleFromPlayer = pl_angle_get(i_this);
            pos_delta = a_this->home.pos - a_this->current.pos;

            if (pl_check(i_this, AREG_F(1) + 120.0f)) {
                anm_init(i_this, BCK_ST_ATTACK02, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mActionPhase = PL_SEARCH_ATTACK02;
                i_this->mSound.startCreatureSound(Z2SE_EN_ST_ATTACK, 0, -1);
            } else if (i_this->arg1 == 0) {
                if (!pl_check(i_this, AREG_F(0) + 350.0f) && i_this->mTimers[0] == 0) {
                    i_this->mAction = ACTION_SHOOT;
                    i_this->mActionPhase = PHASE_INIT;
                    return;
                }
            } else if (!pl_check(i_this, l_HIO.pl_recognize_dist + 100.0f)) {
                i_this->mAction = ACTION_MOVE;
                i_this->mActionPhase = PHASE_INIT;
            }
            break;
        
        case PL_SEARCH_ATTACK02:
            if (frame >= 11 && frame <= 23) {
                i_this->field_0xb94 = 1;
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_ST_WAIT02, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mTimers[0] = cM_rndF(20.0f) + 20.0f;
                i_this->mActionPhase = PHASE_INIT;
            }
            break;

        case PL_SEARCH_WAIT02:
            if (pl_check(i_this, AREG_F(0) + 350.0f) || !daPy_getPlayerActorClass()->getStCaught()) {
                i_this->mActionPhase = PHASE_INIT;
            }
            break;
    }

    if (move_bg_check(i_this)) {
        target = 0.0f;
        a_this->speedF = 0.0f;
        i_this->mAction = ACTION_WAIT;
        i_this->mActionPhase = PHASE_INIT;
    }

    cLib_addCalc2(&a_this->speedF, target, 1.0f, 2.0f);
    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mAngleFromPlayer, 4, 0x800);
    move_calc(i_this);
}

static void e_st_shoot(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp40, sp4c;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            anm_init(i_this, BCK_ST_ATTACK, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mActionPhase = SHOOT_PHASE_ATTACK;
            i_this->mParameters = 0;
            a_this->speedF = 0.0f;
            break;
        
        case SHOOT_PHASE_ATTACK: {
            i_this->mAngleFromPlayer = pl_angle_get(i_this);
            int frame = i_this->mpModelMorf->getFrame();

            if (frame >= 0x1E && frame <= 0x2A) {
                for (int i = 0; i < TREG_S(2) + 5; i++) {
                    fopAcM_createChild(PROC_E_ST_LINE, fopAcM_GetID(a_this), i_this->mParameters,
                                       &i_this->field_0x704, fopAcM_GetRoomNo(a_this), NULL, NULL, -1, NULL);
                    i_this->mParameters++;
                }
            }

            if (frame >= 0x1E && frame <= YREG_S(4) + 0x37) {
                i_this->mSound.startCreatureSoundLevel(Z2SE_EN_ST_SILK, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_ST_WAIT02, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mActionPhase = SHOOT_PHASE_WAIT02;
                i_this->mTimers[0] = 40;
            }
            break;
        }

        case SHOOT_PHASE_WAIT02:
            if (i_this->mTimers[0] == 0) {
                if (daPy_getPlayerActorClass()->getStCaught() && fopAcM_GetParam(a_this) == 1) {
                    i_this->mAction = ACTION_JUMP_ATTACK;
                    i_this->mActionPhase = PHASE_INIT;
                } else {
                    i_this->mAction = ACTION_WAIT;
                    i_this->mActionPhase = PHASE_INIT;
                    i_this->mTimers[1] = cM_rndF(50.0f) + 50.0f;
                }
            }
            break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mAngleFromPlayer, 4, 0x1000);
}

static void e_st_jump_attack(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz pos, target;
    s8 unk_flag = 0;
    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mActionPhase = JUMP_ATTACK_PHASE_JUMP;
            anm_init(i_this, BCK_ST_JUMP, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->field_0x6a6.x = i_this->field_0x69c.x;
            i_this->field_0x6a6.y = i_this->field_0x69c.y;
            i_this->field_0x6ae = i_this->field_0x6a4;
            i_this->field_0x6ac = i_this->field_0x6a2;
            i_this->field_0x5bc = a_this->current.pos;
            a_this->shape_angle.y = a_this->current.angle.y;
            break;

        case JUMP_ATTACK_PHASE_JUMP:
            if (frame >= 0x17) {
                i_this->mActionPhase = JUMP_ATTACK_PHASE_HUG;
                anm_init(i_this, BCK_ST_HUG, 5.0f, J3DFrameCtrl::EMode_NONE, 0.0f);
                i_this->mSound.startCreatureSound(Z2SE_EN_ST_JUMP, 0, -1);
            }
            break;

        case JUMP_ATTACK_PHASE_HUG:
            unk_flag = 1;
            cLib_addCalcAngleS2(&a_this->current.angle.y, fopAcM_searchPlayerAngleY(a_this), 4, 0x1000);
            pos = daPy_getPlayerActorClass()->getHeadTopPos() - a_this->current.pos;
            cMtx_YrotS(*calc_mtx, cM_atan2s(pos.x, pos.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(pos.y, JMAFastSqrt(pos.x * pos.x + pos.z * pos.z)));
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = KREG_F(5) + 80.0f;
            MtxPosition(&pos, &a_this->speed);
            a_this->current.pos += a_this->speed;
            pos = daPy_getPlayerActorClass()->getHeadTopPos() - a_this->current.pos;

            if (pos.abs() <= 85.0f) {
                i_this->mActionPhase = JUMP_ATTACK_PHASE_3;
                i_this->mTimers[0] = 65;
                i_this->mpModelMorf->setPlaySpeed(1.0f);
            }
            break;

        case JUMP_ATTACK_PHASE_3:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_ST_HUG_ATTACK, 1.0f, J3DFrameCtrl::EMode_LOOP, 2.0f);
                i_this->mActionPhase = JUMP_ATTACK_PHASE_HUG_ATTACK;
            }
            // fallthrough
        case JUMP_ATTACK_PHASE_HUG_ATTACK:
            unk_flag = 1;
            cLib_addCalcAngleS2(&a_this->current.angle.y, player->shape_angle.y + 0x8000, 2, 0x1000);
            cLib_addCalcAngleS2(&a_this->current.angle.x, YREG_S(0) + -0x2700, 2, 0x800);
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            pos.set(0.0f, -20.0f, 40.0f);
            MtxPosition(&pos, &target);
            cLib_addCalc2(&a_this->current.pos.x, target.x + daPy_getPlayerActorClass()->getHeadTopPos().x, 1.0f, 70.0f);
            cLib_addCalc2(&a_this->current.pos.y, target.y + daPy_getPlayerActorClass()->getHeadTopPos().y, 1.0f, 70.0f);
            cLib_addCalc2(&a_this->current.pos.z, target.z + daPy_getPlayerActorClass()->getHeadTopPos().z, 1.0f, 70.0f);

            if (i_this->mpModelMorf->checkFrame(4.0f)) {
                i_this->mSound.startCreatureSound(Z2SE_EN_ST_HUG_ATTACK, 0, -1);
            }
            if (i_this->mTimers[0] > 15) {
                if (!daPy_getPlayerActorClass()->getStCaught()) {
                    i_this->mTimers[0] = 15;
                }
            }

            if (i_this->mTimers[0] == 0) {
                i_this->mActionPhase = JUMP_ATTACK_PHASE_JUMPBACK;
                anm_init(i_this, BCK_ST_HUG, 2.0f, J3DFrameCtrl::EMode_NONE, 0.0f);
                i_this->mSound.startCreatureSound(Z2SE_EN_ST_JUMPBACK, 0, -1);
            }
            break;

        case JUMP_ATTACK_PHASE_JUMPBACK:
            unk_flag = 2;
            pos = i_this->field_0x5bc - a_this->current.pos;
            cMtx_YrotS(*calc_mtx, cM_atan2s(pos.x, pos.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(pos.y, JMAFastSqrt(pos.x * pos.x + pos.z * pos.z)));
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = KREG_F(5) + 80.0f;
            MtxPosition(&pos, &a_this->speed);
            a_this->current.pos += a_this->speed;
            pos = i_this->field_0x5bc - a_this->current.pos;

            if (pos.abs() <= 85.0f) {
                i_this->mActionPhase = JUMP_ATTACK_PHASE_LANDING;
                anm_init(i_this, BCK_ST_LANDING, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            }
            break;

        case JUMP_ATTACK_PHASE_LANDING:
            unk_flag = 2;
            cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x5bc.x, 1.0f, fabsf(a_this->speed.x));
            cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x5bc.y, 1.0f, fabsf(a_this->speed.y));
            cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x5bc.z, 1.0f, fabsf(a_this->speed.z));
            pos = i_this->field_0x5bc - a_this->current.pos;

            if (i_this->mpModelMorf->isStop() && pos.abs() <= 1.0f) {
                i_this->mAction = ACTION_WAIT;
                i_this->mActionPhase = PHASE_INIT;
            }
            break;
    }

    pos = i_this->field_0x5bc - player->eyePos;
    f32 fVar1 = pos.abs();
    pos = i_this->field_0x5bc - a_this->current.pos;
    f32 fVar2 = pos.abs();

    i_this->field_0x71c = cM_ssin((s16)((fVar2 / fVar1) * 32768.0f)) * 100.0f;

    if (unk_flag == 1) {
        cLib_addCalcAngleS2(&i_this->field_0x69c.y, 0, 4, 0x800);
        cLib_addCalcAngleS2(&i_this->field_0x69c.x, 0, 4, 0x800);
        cLib_addCalcAngleS2(&i_this->field_0x6a4, 0, 4, 0x800);
        cLib_addCalcAngleS2(&i_this->field_0x6a2, 0, 4, 0x800);
    } else if (unk_flag == 2) {
        cLib_addCalcAngleS2(&i_this->field_0x69c.y, i_this->field_0x6a6.y, 1, 0x1000);
        cLib_addCalcAngleS2(&i_this->field_0x69c.x, i_this->field_0x6a6.x, 1, 0x1000);
        cLib_addCalcAngleS2(&i_this->field_0x6a4, i_this->field_0x6ae, 1, 0x1000);
        cLib_addCalcAngleS2(&i_this->field_0x6a2, i_this->field_0x6ac, 1, 0x1000);
        cLib_addCalcAngleS2(&a_this->current.angle.y, a_this->shape_angle.y, 2, 0x1000);
        cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x1000);
    }
}

static void roof_line_calc(e_st_class* i_this) {
    cXyz sp40, sp4c;

    sp40 = (i_this->field_0x744 - i_this->field_0x710) * (1.0f / 19.0f);
    cXyz* pos_p = i_this->mLineMat1.getPos(0);
    for (int i = 0; i < 20; i++, pos_p++) {
        *pos_p = i_this->field_0x710 + (sp40 * i);
    }

    i_this->mLineCyl.SetH(fabsf(i_this->field_0x744.y - i_this->field_0x710.y));
    i_this->mLineCyl.SetC(i_this->field_0x710);
    dComIfG_Ccsp()->Set(&i_this->mLineCyl);

    if (i_this->mLineCyl.ChkTgHit()) {
        i_this->mAction = ACTION_HANG_DROP;
        i_this->mActionPhase = PHASE_INIT;
        def_se_set(&i_this->mSound, i_this->mLineCyl.GetTgHitObj(), 0x21, NULL);
        pos_p = i_this->mLineCyl.GetTgHitPosP();
        i_this->field_0x750 = i_this->field_0x744;
        i_this->field_0x750.y = pos_p->y;
        i_this->field_0x764 = i_this->field_0x750.y - i_this->field_0x710.y;

        if (i_this->field_0x764 < 0.0f) {
            i_this->field_0x764 = 0.0f;
        }

        i_this->field_0x760 = 1;
        sp40 = (i_this->field_0x750 - i_this->field_0x710) * 0.05263158f;
        cXyz* pos_p2 = i_this->mLineMat2.getPos(0);
        for (int i = 0; i < 20; i++, pos_p2++) {
            *pos_p2 = i_this->field_0x710 + (sp40 * i);
        }

        i_this->field_0x720 = 2;
        i_this->field_0x75c = (BREG_F(3) + 0.002f) * (i_this->field_0x744.y - i_this->field_0x750.y);

        if (i_this->field_0x75c > 2.0f) {
            i_this->field_0x75c = 2.0f;
        }
    }
}

static void roof_line_calc2(e_st_class* i_this) {
    cXyz sp50, sp5c;

    sp50 = (i_this->field_0x744 - i_this->field_0x750) * 0.05263158f;
    cXyz* pos_p = i_this->mLineMat1.getPos(0);
    cXyz sp68;
    for (int i = 0; i < 20; i++, pos_p++) {
        sp68.x = sp50.x;
        sp68.y = sp50.y * i_this->field_0x724;
        sp68.z = sp50.z;
        *pos_p = i_this->field_0x744 - (sp68 * i);
        f32 fVar1 = i * i_this->field_0x75c;
        pos_p->x += fVar1 * cM_ssin(i_this->mFrameCounter * 1500 + i * 5700);
        pos_p->z += fVar1 * cM_scos(i_this->mFrameCounter * 1800 + i * 5000);
    }

    cLib_addCalc0(&i_this->field_0x75c, 1.0f, 0.02f);
    cLib_addCalc0(&i_this->field_0x724, 1.0f, 0.005f);
}

static void tail_line_calc(e_st_class* i_this) {
    f32 fVar2, fVar5;
    int i;
    cXyz* pos_p;
    f32* pfVar1;
    Vec pos;
    f32 fVar1, fVar3, fVar4;
    dBgS_GndChk gnd_chk;
    cXyz spd4, spe0;

    pos_p = i_this->mLineMat2.getPos(0);
    *pos_p = i_this->field_0x710;
    pos_p++;
    pfVar1 = &i_this->field_0x788;
    spd4.x = 0.0f;
    spd4.y = 0.0f;
    spd4.z = i_this->field_0x764 * (TREG_F(6) + 0.05f);
    fVar1 = TREG_F(7) + -5.0f;
    for (i = 1; i < 20; i++, pos_p++, pfVar1++) {
        fVar2 = pos_p->x - (pos_p - 1)->x;
        fVar3 = pos_p->z - (pos_p - 1)->z;
        fVar4 = pos_p->y + fVar1;

        if (fVar4 < *pfVar1) {
            fVar4 = *pfVar1;
        }

        fVar5 = fVar4 - (pos_p - 1)->y;
        cMtx_YrotS(*calc_mtx, cM_atan2s(fVar2, fVar3));
        cMtx_XrotM(*calc_mtx, -cM_atan2s(fVar5, JMAFastSqrt(fVar2 * fVar2 + fVar3 * fVar3)));
        MtxPosition(&spd4, &spe0);
        pos_p->x = (pos_p - 1)->x + spe0.x;
        pos_p->y = (pos_p - 1)->y + spe0.y;
        pos_p->z = (pos_p - 1)->z + spe0.z;

        if ((i_this->mFrameCounter + i & 0xF) == 0) {
            pos = *pos_p;
            pos.y += 200.0f;
            gnd_chk.SetPos(&pos);
            *pfVar1 = dComIfG_Bgsp().GroundCross(&gnd_chk) + 5.0f;
            if (*pfVar1 - pos_p->y > 200.0f) {
                *pfVar1 = pos_p->y;
            }
        }
    }
}

static void e_st_hang(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz sp30, sp3c;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            anm_init(i_this, BCK_ST_HANG, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mpModelMorf->setFrame(cM_rndF(20.0f));
            i_this->mActionPhase = HANG_PHASE_HANG;
            fopAcM_SetParam(a_this, 0);
            // fallthrough
        case HANG_PHASE_HANG:
            if ((i_this->mFrameCounter & 0xF) == 0 && cM_rndF(1.0f) < 0.5f) {
                i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_WAIT, -1);
            }

            if (i_this->mTimers[0] == 0) {
                i_this->mTimers[0] = cM_rndF(100.0f) + 50.0f;
                i_this->mBgPos.y = a_this->home.pos.y + cM_rndFX(i_this->field_0x7d8);

                if (a_this->current.pos.y < i_this->mBgPos.y) {
                    a_this->speedF = 3.0f;
                } else {
                    a_this->speedF = 10.0f;
                }
            }
            break;
    }

    cLib_addCalc2(&a_this->current.pos.y, i_this->mBgPos.y, 0.1f, a_this->speed.y);
    cLib_addCalc2(&a_this->speed.y, a_this->speedF, 1.0f, 0.5f);
    cLib_addCalcAngleS2(&a_this->current.angle.y, fopAcM_searchPlayerAngleY(a_this), 0x10, 0x200);

    if (i_this->arg1 == 0 && i_this->mTimers[1] == 0) {
        if (!daPy_getPlayerActorClass()->getStCaught() && fopAcM_searchPlayerDistanceXZ(a_this) < 300.0f && a_this->current.pos.y - player->current.pos.y > 0.0f) {
            if (a_this->current.pos.y - dComIfGp_getPlayer(0)->current.pos.y > 1000.0f) {
                i_this->mAction = ACTION_HANG_DROP;
                i_this->field_0x750 = i_this->field_0x710;
                i_this->field_0x720 = 2;
                i_this->field_0x75c = (BREG_F(3) + 0.002f) * (i_this->field_0x744.y - i_this->field_0x750.y);

                if (i_this->field_0x75c > 2.0f) {
                    i_this->field_0x75c = 2.0f;
                }
            } else {
                i_this->mAction = ACTION_HANG_SHOOT;
            }

            i_this->mActionPhase = PHASE_INIT;
        }
    }
}

static void e_st_hang_shoot(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz sp40, sp4c;

    if (a_this->current.pos.y - player->current.pos.y <= 0.0f) {
        i_this->mAction = ACTION_HANG;
        i_this->mActionPhase = PHASE_INIT;
        i_this->mTimers[1] = cM_rndF(20.0f) + 20.0f;
        return;
    }

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            if (i_this->arg2 == 0) {
                i_this->mActionPhase = HANG_SHOOT_PHASE_HANGATTACK;
                anm_init(i_this, BCK_ST_HANGATTACK, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mTimers[0] = 80;
                i_this->mParameters = 0;
                break;
            }

            i_this->mActionPhase = HANG_SHOOT_PHASE_HANG;
            i_this->mBgPos.y = player->current.pos.y + 500.0f;

            if (i_this->mBgPos.y > i_this->field_0x744.y - 50.0f) {
                i_this->mBgPos.y = i_this->field_0x744.y - 50.0f;
            } else if (i_this->mBgPos.y < a_this->home.pos.y + 50.0f) {
                i_this->mBgPos.y = a_this->home.pos.y + 50.0f;
            }

            anm_init(i_this, BCK_ST_HANG, 5.0f, J3DFrameCtrl::EMode_LOOP, 2.5f);
            // fallthrough
        case HANG_SHOOT_PHASE_HANG:
            if (fabsf(a_this->current.pos.y - i_this->mBgPos.y) < 50.0f) {
                i_this->mActionPhase = HANG_SHOOT_PHASE_HANGATTACK;
                anm_init(i_this, BCK_ST_HANGATTACK, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mTimers[0] = 80;
                i_this->mParameters = 0;
            }
            break;
        
        case HANG_SHOOT_PHASE_HANGATTACK:
            int frame = i_this->mpModelMorf->getFrame();

            if (frame >= 0x1E && frame <= 0x2A) {
                for (int i = 0; i < TREG_S(2) + 5; i++) {
                    fopAcM_createChild(PROC_E_ST_LINE, fopAcM_GetID(a_this), i_this->mParameters,
                                       &i_this->field_0x704, fopAcM_GetRoomNo(a_this), NULL, NULL, -1, NULL);
                    i_this->mParameters++;
                }
            }

            if (frame >= 0x1E && frame <= YREG_S(4) + 0x37) {
                i_this->mSound.startCreatureSoundLevel(Z2SE_EN_ST_SILK, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                if (daPy_getPlayerActorClass()->getStCaught() && fopAcM_GetParam(a_this) == 1) {
                    i_this->mAction = ACTION_HANG_DROP;
                    i_this->mActionPhase = PHASE_INIT;
                    i_this->field_0x750 = i_this->field_0x710;
                    i_this->field_0x720 = 2;
                    i_this->field_0x75c = (BREG_F(3) + 0.002f) * (i_this->field_0x744.y - i_this->field_0x750.y);

                    if (i_this->field_0x75c > 2.0f) {
                        i_this->field_0x75c = 2.0f;
                    }
                } else {
                    i_this->mAction = ACTION_HANG;
                    i_this->mActionPhase = PHASE_INIT;
                    i_this->mTimers[1] = cM_rndF(50.0f) + 50.0f;
                }
            }
            break;
    }

    cLib_addCalc2(&a_this->current.pos.y, i_this->mBgPos.y, 0.2f, 15.0f);
    cLib_addCalcAngleS2(&a_this->current.angle.y, fopAcM_searchPlayerAngleY(a_this), 4, 0x1000);
}

static void e_st_hang_drop(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp28;

    i_this->mInvulnerabilityTimer = 5;
    cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 4, 0x400);
    cLib_addCalcAngleS2(&i_this->field_0x69c.y, 0, 1, 0x400);
    cLib_addCalcAngleS2(&i_this->field_0x69c.x, 0, 1, 0x400);

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mActionPhase = HANG_DROP_PHASE_WAIT;

            if (i_this->field_0x69c.x != 0) {
                cMtx_YrotS(*calc_mtx, i_this->field_0x69c.y);
                cMtx_XrotM(*calc_mtx, i_this->field_0x69c.x);
                sp28.x = 0.0f;
                sp28.y = YREG_F(5) + 10.0f;
                sp28.z = 0.0f;
                MtxPosition(&sp28, &a_this->speed);
            } else {
                a_this->speed.set(0.0f, 0.0f, 0.0f);
            }

            a_this->speedF = 0.0f;
            anm_init(i_this, BCK_ST_WAIT, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            break;
        
        case HANG_DROP_PHASE_WAIT:
            if (i_this->mBgc.ChkGroundHit()) {
                i_this->mActionPhase = HANG_DROP_PHASE_LANDING;
                i_this->mBgPos = a_this->current.pos;
                a_this->home.pos = a_this->current.pos;
                bg_pos_get(i_this);
                anm_init(i_this, BCK_ST_LANDING, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                a_this->speed.set(0.0f, 0.0f, 0.0f);
            }
            break;

        case HANG_DROP_PHASE_LANDING:
            cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x1000);
            cLib_addCalcAngleS2(&i_this->field_0x69c.y, 0, 1, 0x1000);
            cLib_addCalcAngleS2(&i_this->field_0x69c.x, 0, 1, 0x1000);

            if (i_this->mpModelMorf->isStop()) {
                i_this->field_0x69c.y = 0;
                i_this->field_0x69c.x = 0;
                a_this->current.angle.x = 0;
                i_this->mAction = ACTION_G_NORMAL;
                i_this->mActionPhase = PHASE_INIT;
                i_this->arg0 = 2;
            }
            break;
    }

    a_this->current.pos += a_this->speed;
    a_this->speed.y -= YREG_F(7) + 3.0f;
    i_this->mBgc.CrrPos(dComIfG_Bgsp());
}

static s8 e_st_hang_2(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz pos, sp34;
    s8 rv = 0;
    s8 unk_flag = 0;
    s8 isNotSmall = 0;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mActionPhase = HANG_2_PHASE_HANG;
            i_this->mTimers[1] = cM_rndF(100.0f);
            // fallthrough
        case HANG_2_PHASE_HANG:
            if (fabsf(a_this->current.pos.y - a_this->home.pos.y) < 5.0f && i_this->mAnm != BCK_ST_HANG) {
                anm_init(i_this, BCK_ST_HANG, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mpModelMorf->setFrame(cM_rndF(20.0f));
            }

            if ((i_this->mFrameCounter & 0xF) == 0 && cM_rndF(1.0f) < 0.5f) {
                i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_WAIT, -1);
            }

            cLib_addCalc2(&a_this->current.pos.y, a_this->home.pos.y, 0.5f, 10.0f);

            if (i_this->mTimers[1] == 0 && fabsf(a_this->current.pos.y - a_this->home.pos.y) < 10.0f) {
                rv = 1;
            }

            if (fopAcM_searchPlayerDistanceXZ(a_this) < i_this->field_0x7d8 && a_this->current.pos.y - player->current.pos.y > 0.0f) {
                i_this->mActionPhase = HANG_2_PHASE_2;
                i_this->field_0x7ec = 20.0f;
                i_this->field_0x7f0 = 0;
                i_this->field_0x7f2 = cM_rndF(65536.0f);
                i_this->mSound.startCreatureSound(Z2SE_EN_ST_APPEAR, 0, -1);
            }
            break;
        
        case HANG_2_PHASE_2:
            if (i_this->arg1 == 4) {
                i_this->mBgPos.y = player->current.pos.y + 120.0f + 150.0f + YREG_F(7);
            } else {
                i_this->mBgPos.y = player->current.pos.y + 120.0f;
            }

            pos.set(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);

            if (fopAcM_gc_c::gndCheck(&pos)) {
                pos.y = TREG_F(7) + fopAcM_gc_c::getGroundY() + 120.0f + 20.0f;

                if (i_this->mBgPos.y < pos.y) {
                    i_this->mBgPos.y = pos.y;
                }

                if (!small) {
                    isNotSmall = true;
                }
            }

            if (i_this->mTimers[0] == 58) {
                i_this->mSound.startCreatureSound(Z2SE_EN_ST_JUMPBACK, 0, -1);
            }

            cLib_addCalc2(&a_this->current.pos.y, i_this->mBgPos.y, 1.0f, 50.0f);

            if (!player->checkPlayerFly() && fabsf(a_this->current.pos.y - i_this->mBgPos.y) < 10.0f) {
                if (!daPy_getPlayerActorClass()->getStCaught() && i_this->arg1 == 4) {
                    i_this->mAction = ACTION_HANG_2_SHOOT;
                    i_this->mActionPhase = PHASE_INIT;
                    return 0;
                }

                if (isNotSmall) {
                    unk_flag = true;
                } else {
                    anm_init(i_this, BCK_ST_HANG_DOWN, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                    i_this->mActionPhase = HANG_2_PHASE_HANG02;
                    i_this->mTimers[0] = 60;
                }
            }
            break;

        case HANG_2_PHASE_HANG02:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_ST_HANG02, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }

            a_this->field_0x566 = 1;
            
            if (i_this->mTimers[0] == 0 && (fopAcM_searchPlayerDistanceXZ(a_this) > i_this->field_0x7d8 + 50.0f || a_this->current.pos.y - player->current.pos.y <= 0.0f)) {
                i_this->mActionPhase = HANG_2_PHASE_HANG;
                anm_init(i_this, BCK_ST_HANG_UP, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }

            if (i_this->mAtSph.ChkAtHit()) {
                i_this->mActionPhase = HANG_2_PHASE_SHOCK;
                anm_init(i_this, BCK_ST_HANG_SHOCK, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            }
            break;
            
        case HANG_2_PHASE_SHOCK:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_ST_HANG02, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mActionPhase = HANG_2_PHASE_HANG02;
            }
            break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, fopAcM_searchPlayerAngleY(a_this), 0x10, 0x400);

    if (unk_flag || (daPy_getPlayerActorClass()->getStCaught() && fopAcM_GetParam(a_this) == 1)) {
        i_this->mSound.startCreatureSound(Z2SE_EN_ST_SILK_RELEASE, 0, -1);
        i_this->mAction = ACTION_HANG_DROP;
        i_this->mActionPhase = PHASE_INIT;
        i_this->field_0x750 = i_this->field_0x710;
        i_this->field_0x720 = 2;
        i_this->field_0x75c = (BREG_F(3) + 0.002f) * (i_this->field_0x744.y - i_this->field_0x750.y);

        if (i_this->field_0x75c > 2.0f) {
            i_this->field_0x75c = 2.0f;
        }
    }

    return rv;
}

static void e_st_hang_2_shoot(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz sp40, sp4c;

    if (a_this->current.pos.y - player->current.pos.y <= 0.0f) {
        i_this->mAction = ACTION_HANG_2;
        i_this->mActionPhase = PHASE_INIT;
        return;
    }

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mActionPhase = HANG_2_SHOOT_PHASE_HANGATTACK;
            anm_init(i_this, BCK_ST_HANGATTACK, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mParameters = 0;
            break;
        
        case HANG_2_SHOOT_PHASE_HANGATTACK:
            int frame = i_this->mpModelMorf->getFrame();

            if (frame >= 0x1E && frame <= 0x2A) {
                for (int i = 0; i < TREG_S(2) + 5; i++) {
                    fopAcM_createChild(PROC_E_ST_LINE, fopAcM_GetID(a_this), i_this->mParameters,
                                       &i_this->field_0x704, fopAcM_GetRoomNo(a_this), NULL, NULL, -1, NULL);
                    i_this->mParameters++;
                }
            }

            if (frame >= 0x1E && frame <= YREG_S(4) + 0x37) {
                i_this->mSound.startCreatureSoundLevel(Z2SE_EN_ST_SILK, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_HANG_2;
                i_this->mActionPhase = HANG_2_PHASE_HANG02;
                anm_init(i_this, BCK_ST_HANG02, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, fopAcM_searchPlayerAngleY(a_this), 4, 0x1000);
}

static void e_st_s_damage(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mActionPhase = S_DAMAGE_PHASE_DAMAGE;
            anm_init(i_this, BCK_ST_DAMAGE, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            break;
        
        case S_DAMAGE_PHASE_DAMAGE:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_G_FIGHT;
                i_this->mActionPhase = PHASE_INIT;
                i_this->arg0 = 2;
            }
            break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, fopAcM_searchPlayerAngleY(a_this), 4, 0x1000);

    if (i_this->field_0x7e0 > 0.1f) {
        cXyz sp1c, sp28;

        sp1c.x = 0.0f;
        sp1c.y = 0.0f;
        sp1c.z = -i_this->field_0x7e0;
        cMtx_YrotS(*calc_mtx, i_this->mHitDirectionY);
        MtxPosition(&sp1c, &sp28);
        a_this->current.pos += sp28;
        i_this->mBgPos.x = a_this->current.pos.x;
        i_this->mBgPos.z = a_this->current.pos.z;
        cLib_addCalc0(&i_this->field_0x7e0, 1.0f, TREG_F(12) + 7.0f);
        i_this->mBgc.CrrPos(dComIfG_Bgsp());
    }
}

static void e_st_damage(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp54, sp60;
    s16 maxStep = 0x200;
    
    i_this->mInvulnerabilityTimer = 6;
    i_this->field_0x7ec = 0.0f;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mActionPhase = DAMAGE_PHASE_HANGDAMAGE;
            anm_init(i_this, BCK_ST_HANGDAMAGE, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->field_0x7f4 = 17.0f;

            if (a_this->current.angle.x < 0x1000 && a_this->current.angle.x > -0x1000) {
                if (i_this->field_0x69c.y != 0 || i_this->field_0x69c.x != 0) {
                    cMtx_YrotS(*calc_mtx, i_this->field_0x69c.y);
                    cMtx_XrotM(*calc_mtx, i_this->field_0x69c.x);
                    sp54.x = 0.0f;
                    sp54.y = YREG_F(5) + 10.0f;
                    sp54.z = 0.0f;
                } else {
                    cMtx_YrotS(*calc_mtx, i_this->mHitDirectionY);
                    sp54.x = 0.0f;
                    sp54.y = YREG_F(5) + 35.0f;
                    sp54.z = -30.0f;
                }

                MtxPosition(&sp54, &a_this->speed);
            } else {
                cMtx_YrotS(*calc_mtx, i_this->mHitDirectionY);
                sp54.x = 0.0f;
                sp54.y = 40.0f;
                sp54.z = -30.0f;
                MtxPosition(&sp54, &a_this->speed);
            }

            i_this->field_0x6a2 = 0;
            i_this->field_0x6a4 = 0;
            i_this->mTimers[2] = 10;
            break;

        case DAMAGE_PHASE_HANGDAMAGE:
            if (i_this->mBgc.ChkGroundHit() || i_this->mpModelMorf->getFrame() >= i_this->field_0x7f4) {
                i_this->field_0x7f4 = YREG_F(4) + 2.0f;
                anm_init(i_this, BCK_ST_DEADWAIT, 10.0f, J3DFrameCtrl::EMode_LOOP, i_this->field_0x7f4);
                i_this->mActionPhase = DAMAGE_PHASE_DEADWAIT;
            }
            // fallthrough
        case DAMAGE_PHASE_DEADWAIT:
            a_this->current.angle.y += i_this->field_0x7d4;
            cLib_addCalcAngleS2(&a_this->current.angle.x, -0x8000, 4, 0x700);

            if (i_this->mBgc.ChkGroundHit()) {
                i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
                i_this->mActionPhase = DAMAGE_PHASE_3;
                a_this->speed.x *= 0.3f;
                a_this->speed.z *= 0.3f;
                a_this->speed.y = 30.0f;
                maxStep = 0x1000;
                fopAcM_effSmokeSet1(&i_this->field_0xf6c, &i_this->field_0xf70, &a_this->current.pos, NULL, 1.0f, &a_this->tevStr, 1);
                i_this->mTimers[1] = 60;
            }

            if (i_this->mTimers[2] == 0 && i_this->mBgc.ChkWallHit()) {
                i_this->mTimers[2] = 10;
                a_this->speed.x *= -0.3f;
                a_this->speed.z *= -0.3f;
            }
            break;
        
        case DAMAGE_PHASE_3:
        case DAMAGE_PHASE_4:
        case DAMAGE_PHASE_DEAD:
            a_this->current.angle.y += i_this->field_0x7d4;
            maxStep = 0x800;

            if (i_this->mBgc.ChkGroundHit()) {
                if (i_this->mActionPhase == DAMAGE_PHASE_3) {
                    i_this->mTimers[1] = 60;
                    i_this->mActionPhase = DAMAGE_PHASE_4;
                }

                a_this->speed.x *= 0.5f;
                a_this->speed.z *= 0.5f;
                maxStep = 0x1000;

                if (i_this->mAnm == BCK_ST_DEAD && i_this->mTimers[0] == 0) {
                    u8 i_size;
                    if (small) {
                        i_size = 7;
                    } else {
                        i_size = 16;
                    }

                    fopAcM_createDisappear(a_this, &a_this->current.pos, i_size, 0, 6);
                    fopAcM_delete(a_this);

                    if (i_this->mSwBit != 0xFF) {
                        dComIfGs_onSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this));
                    }
                }
            }

            f32 fVar2 = i_this->mTimers[1] * (TREG_F(14) + 150.0f);
            f32 fVar3 = cM_ssin(i_this->mTimers[1] * 4000);
            s16 target = fVar2 * fVar3 + 32768.0f;
            cLib_addCalcAngleS2(&a_this->current.angle.x, target, 4, maxStep);
            cLib_addCalcAngleS2(&a_this->current.angle.z, (s16)(fVar2 * cM_scos(i_this->mTimers[1] * 3000)), 4, maxStep);

            if (i_this->mActionPhase == DAMAGE_PHASE_4) {
                cLib_addCalc0(&i_this->field_0x7f4, 1.0f, 0.1f);
                i_this->mpModelMorf->setPlaySpeed(i_this->field_0x7f4);

                if (i_this->field_0x7f4 < 0.2f) {
                    i_this->mActionPhase = DAMAGE_PHASE_DEAD;
                    anm_init(i_this, BCK_ST_DEAD, YREG_F(6) + 30.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                    i_this->mTimers[0] = 40;
                }
            }

            if (i_this->mActionPhase >= DAMAGE_PHASE_4) {
                cLib_addCalcAngleS2(&i_this->field_0x7d4, 0, 1, 0x50);
            }
            break;
    }

    cLib_addCalcAngleS2(&i_this->field_0x69c.y, 0, 4, maxStep);
    cLib_addCalcAngleS2(&i_this->field_0x69c.x, 0, 4, maxStep);
    a_this->current.pos += a_this->speed;
    a_this->speed.y -= YREG_F(8) + 5.0f;
    a_this->current.pos.y -= 30.0f;
    a_this->old.pos.y -= 30.0f;
    i_this->mBgc.CrrPos(dComIfG_Bgsp());
    a_this->current.pos.y += 30.0f;
    a_this->old.pos.y += 30.0f;
}

static void e_st_water(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz pos_delta, sp58;

    pos_delta = a_this->home.pos - a_this->current.pos;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mpModelMorf->setPlaySpeed(0.0f);
            i_this->mActionPhase = WATER_PHASE_DEAD;
            i_this->mTimers[0] = 40;
            a_this->speed.y = 0.0f;
            i_this->mDeathFlag = 1;
            anm_init(i_this, BCK_ST_WATERDEAD, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mSound.startCreatureSound(Z2SE_EN_ST_V_WATERDEAD, 0, -1);
            // fallthrough
        case WATER_PHASE_DEAD:
            a_this->current.pos.y += a_this->speed.y;
            a_this->speed.y -= 0.1f;

            if (a_this->speed.y < -2.0f) {
                a_this->speed.y = -2.0f;
            }

            if (i_this->mTimers[0] == 0) {
                fopAcM_createDisappear(a_this, &a_this->current.pos, l_HIO.basic_size * 10.0f, 0, 6);
                fopAcM_delete(a_this);

                if (i_this->mSwBit != 0xFF) {
                    dComIfGs_onSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this));
                }
            }
            break;
    }

    a_this->current.pos.x += a_this->speed.x;
    a_this->current.pos.z += a_this->speed.z;
    i_this->mBgc.CrrPos(dComIfG_Bgsp());
    a_this->speed.x *= 0.9f;
    a_this->speed.z *= 0.9f;

    cXyz pos(a_this->current.pos);
    pos.y = i_this->mGroundCross;

    if (fabsf(pos.y - a_this->current.pos.y) < 70.0f) {
        f32 scale = 2.0f;
        if (small) {
            scale = 1.0f;
        }

        fopAcM_effHamonSet(&i_this->field_0xf7c, &pos, scale, 0.2f);
    }
}

static BOOL water_check(e_st_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    
    if (small) {
        i_this->mGroundCross = 3120.0f;
    } else {
        dBgS_ObjGndChk_Spl gnd_chk_spl;
        Vec pos;
        pos.x = a_this->current.pos.x;
        pos.y = a_this->current.pos.y + 200.0f;
        pos.z = a_this->current.pos.z;
        gnd_chk_spl.SetPos(&pos);
        i_this->mGroundCross = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);
    }

    return i_this->mGroundCross > a_this->current.pos.y + 10.0f + WREG_F(11) ? TRUE : FALSE;
}

static BOOL gr_check(e_st_class* i_this) {
    fopEn_enemy_c* a_this = &i_this->actor;
    dBgS_ObjGndChk gnd_chk;
    Vec pos;
    pos.x = a_this->current.pos.x;
    pos.y = a_this->current.pos.y + 200.0f;
    pos.z = a_this->current.pos.z;
    gnd_chk.SetPos(&pos);

    if (a_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 200.0f) {
        return TRUE;
    }

    return FALSE;
}

static void e_st_g_normal(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz pos, sp58;
    f32 target = 0.0f;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mActionPhase = G_NORMAL_PHASE_WAIT;
            anm_init(i_this, BCK_ST_WAIT, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mTimers[0] = cM_rndF(100.0f) + 50.0f;
            i_this->mAngleFromPlayer = a_this->current.angle.y;
            break;

        case G_NORMAL_PHASE_WAIT:
            if (i_this->mTimers[0] == 0) {
                i_this->mActionPhase = G_NORMAL_PHASE_TURN;
                pos.x = a_this->home.pos.x + cM_rndFX(700.0f);
                f32 rndValue = cM_rndFX(700.0f);
                pos.z = a_this->home.pos.z + rndValue;
                pos.x -= a_this->current.pos.x;
                pos.z -= a_this->current.pos.z;
                i_this->mAngleFromPlayer = cM_atan2s(pos.x, pos.z);                
            }
            break;
        
        case G_NORMAL_PHASE_TURN: {
            s16 angle_delta = i_this->mAngleFromPlayer - a_this->current.angle.y;
            if (angle_delta < 0x800 && angle_delta > -0x800) {
                anm_init(i_this, BCK_ST_MOVE, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mActionPhase = G_NORMAL_PHASE_MOVE;
                i_this->mTimers[0] = cM_rndF(100.0f) + 50.0f;
            } else if (i_this->mAnm != BCK_ST_TURN) {
                anm_init(i_this, BCK_ST_TURN, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;
        }

        case G_NORMAL_PHASE_MOVE:
            target = (VREG_F(0) + 1.7f) * l_HIO.basic_size;

            if (i_this->mTimers[0] == 0) {
                i_this->mActionPhase = PHASE_INIT;
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, target, 1.0f, 1.0f);
    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mAngleFromPlayer, 2, 0x100);

    if (pl_check(i_this, l_HIO.pl_recognize_dist)) {
        i_this->mAction = ACTION_G_FIGHT;
        i_this->mActionPhase = PHASE_INIT;
    }
}

static s8 e_st_g_fight(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp64, sp70;
    int frame = i_this->mpModelMorf->getFrame();
    f32 target = 0.0f;
    s16 maxStep = 0x200;
    s8 isStop = false;
    s8 isPreparingAttack = false;
    s8 rv = false;
    f32 combat_start_dist = l_HIO.combat_start_dist;
    if (small) {
        combat_start_dist *= BREG_F(11) + 0.58f;
    }

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            anm_init(i_this, BCK_ST_MOVE, 6.0f, J3DFrameCtrl::EMode_LOOP, 2.0f);
            i_this->mActionPhase = G_FIGHT_PHASE_MOVE;
            // fallthrough
        case G_FIGHT_PHASE_MOVE:
            maxStep = 0x400;
            i_this->mAngleFromPlayer = fopAcM_searchPlayerAngleY(a_this);
            target = (VREG_F(2) + 3.5f) * l_HIO.basic_size;
            if (pl_check(i_this, combat_start_dist)) {
                i_this->mActionPhase = G_FIGHT_PHASE_WAIT02;
                anm_init(i_this, BCK_ST_WAIT02, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mTimers[0] = cM_rndF(20.0f) + 20.0f;
            }
            break;
        
        case G_FIGHT_PHASE_WAIT02:
            rv = true;
            i_this->mAngleFromPlayer = fopAcM_searchPlayerAngleY(a_this);

            if (i_this->mTimers[0] == 0) {
                anm_init(i_this, BCK_ST_ATTACKA, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_ATTACKA, -1);
                i_this->mActionPhase = G_FIGHT_PHASE_ATTACKA;
            }

            isPreparingAttack = true;
            break;
        
        case G_FIGHT_PHASE_ATTACKA:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_ST_ATTACKB, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_ATTACKB, -1);
                i_this->mActionPhase = G_FIGHT_PHASE_ATTACKB;
                i_this->mTimers[0] = l_HIO.time_before_attack;
            }

            isPreparingAttack = true;
            break;

        case G_FIGHT_PHASE_ATTACKB:
            if (i_this->mTimers[0] == 0) {
                anm_init(i_this, BCK_ST_ATTACKC, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_ATTACKC, -1);
                i_this->mActionPhase = G_FIGHT_PHASE_ATTACKC;
            }

            isPreparingAttack = true;
            break;
            
        case G_FIGHT_PHASE_ATTACKC:
            if (frame >= 0x1A && frame <= 0x22) {
                i_this->field_0xb94 = 1;
            }

            if (i_this->mpModelMorf->isStop()) {
                isStop = true;
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, target, 1.0f, 2.0f);
    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mAngleFromPlayer, 2, maxStep);

    if (isPreparingAttack && i_this->mPlayerDistance > combat_start_dist + 100.0f) {
        i_this->mActionPhase = PHASE_INIT;
    }

    if (!pl_check(i_this, l_HIO.pl_recognize_dist + 100.0f) || isStop) {
        i_this->mAction = ACTION_G_NORMAL;
        i_this->mActionPhase = PHASE_INIT;
        rv = false;
    }

    if (i_this->mAtSph.ChkAtHit()) {
        fopAc_ac_c* actor_p = dCc_GetAc(i_this->mAtSph.GetAtHitObj()->GetAc());
        if (actor_p != NULL && fopAcM_GetName(actor_p) == PROC_ALINK) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER4, 31, cXyz(0.0f, 1.0f, 0.0f));
        }
    }

    return rv;
}

static s8 e_st_g_def(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp48, sp54;

    i_this->field_0xcf4 = 1;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            anm_init(i_this, BCK_ST_DEFENSE, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_DEFENSE, -1);
            i_this->mActionPhase = G_DEF_PHASE_DEFENSE;
            break;
        
        case G_DEF_PHASE_DEFENSE:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_G_FIGHT;
                i_this->mActionPhase = PHASE_INIT;
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, 0.0f, 1.0f, 2.0f);
    return 1;
}

static void e_st_g_damage(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            if ((i_this->mHorizHitDir & 1) != 0) {
                anm_init(i_this, BCK_ST_HITL, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            } else {
                anm_init(i_this, BCK_ST_HITR, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            }

            i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_HIT, -1);
            i_this->mHorizHitDir++;
            i_this->mActionPhase = G_DAMAGE_PHASE_HIT;
            break;
        
        case G_DAMAGE_PHASE_HIT:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_G_FIGHT;
                i_this->mActionPhase = PHASE_INIT;
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, 0.0f, 1.0f, 2.0f);
}

static void e_st_g_s_damage(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mActionPhase = G_S_DAMAGE_PHASE_NODAMAGE;
            anm_init(i_this, BCK_ST_DAMAGE, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_NODAMAGE, -1);
            break;
        
        case G_S_DAMAGE_PHASE_NODAMAGE:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_G_FIGHT;
                i_this->mActionPhase = PHASE_INIT;
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, 0.0f, 1.0f, 2.0f);
}

static void e_st_g_wind(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            i_this->mActionPhase = G_WIND_PHASE_WINDDAMAGE;
            anm_init(i_this, BCK_ST_WINDDAMAGE, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureSound(Z2SE_EN_ST_V_WINDDAMAGE, 0, -1);
            break;
        
        case G_WIND_PHASE_WINDDAMAGE:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_G_FIGHT;
                i_this->mActionPhase = PHASE_INIT;
                i_this->arg0 = 2;
            }
            break;
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);
}

static void e_st_g_chance(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp3c, sp48;

    i_this->field_0xcf4 = 2;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            anm_init(i_this, BCK_ST_DAWNA, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_DAWNA, -1);
            i_this->mActionPhase = G_CHANCE_PHASE_DAWNA;
            a_this->onDownFlg();
            break;

        case G_CHANCE_PHASE_DAWNA:
            if (i_this->mpModelMorf->checkFrame(13.0f)) {
                i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_ST_DAWNB, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_DAWNB, -1);
                i_this->mActionPhase = G_CHANCE_PHASE_DAWNB;
                i_this->mTimers[0] = l_HIO.down_time;
            }
            break;
        
        case G_CHANCE_PHASE_DAWNB:
            if (i_this->mAnm == BCK_ST_DAWNC && i_this->mpModelMorf->isStop()) {
                anm_init(i_this, BCK_ST_DAWNB, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }

            if (i_this->mTimers[0] == 0) {
                anm_init(i_this, BCK_ST_STAND, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_STAND, -1);
                i_this->mActionPhase = G_CHANCE_PHASE_STAND;
            }
            break;

        case G_CHANCE_PHASE_STAND:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = ACTION_G_FIGHT;
                i_this->mActionPhase = PHASE_INIT;
                a_this->offDownFlg();
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, 0.0f, 1.0f, 4.0f);
    a_this->setDownPos(&a_this->current.pos);

    if (a_this->checkCutDownHitFlg()) {
        i_this->mAction = ACTION_G_END;
        i_this->mActionPhase = PHASE_INIT;
        a_this->offCutDownHitFlg();
        a_this->offDownFlg();
        i_this->mDeathFlag = 1;
        daPy_getPlayerActorClass()->onEnemyDead();
    }
}

static s8 e_st_g_end(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp58, sp64;
    s8 rv = 1;
    
    i_this->mDefTimer = 10;
    i_this->mInvulnerabilityTimer = 10;

    switch (i_this->mActionPhase) {
        case PHASE_INIT:
            anm_init(i_this, BCK_ST_DEATH, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_DEATH2, -1);
            i_this->mActionPhase = G_END_PHASE_DEATH2;
            break;
        
        case G_END_PHASE_DEATH2:
            if (i_this->mpModelMorf->checkFrame(36.5f)) {
                i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                fopAcM_createDisappear(a_this, &a_this->current.pos, l_HIO.basic_size * 10.0f, 0, 6);
                fopAcM_delete(a_this);

                if (i_this->mSwBit != 0xFF) {
                    dComIfGs_onSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this));
                }
            }
            break;

        case G_END_PHASE_10:
            anm_init(i_this, BCK_ST_DEATHA, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_DEATHA, -1);
            i_this->mActionPhase = G_END_PHASE_DEATHA;
            break;

        case G_END_PHASE_DEATHA:
            rv = 0;

            if (i_this->mpModelMorf->checkFrame(32.0f)) {
                i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
            }

            if (i_this->mpModelMorf->isStop()) {
                fopAcM_createDisappear(a_this, &a_this->current.pos, l_HIO.basic_size * 10.0f, 0, 6);
                fopAcM_delete(a_this);

                if (i_this->mSwBit != 0xFF) {
                    dComIfGs_onSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this));
                }
            }
            break;
    }

    if (i_this->mBgc.ChkWallHit()) {
        a_this->speedF = 0.0f;
    }

    cLib_addCalc2(&a_this->speedF, 0.0f, 1.0f, 4.0f);
    return rv;
}

static void damage_check_g(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);

    i_this->mStts.Move();

    if (i_this->mInvulnerabilityTimer != 0) {
        return;
    }

    if (i_this->mSph.ChkTgHit()) {
        i_this->mAtInfo.mpCollider = i_this->mSph.GetTgHitObj();

        if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
            i_this->mAction = ACTION_G_WIND;
            i_this->mActionPhase = PHASE_INIT;
            i_this->mInvulnerabilityTimer = 10;
            return;
        }

        cc_at_check(a_this, &i_this->mAtInfo);
        OS_REPORT("E_st DAM %d\n", i_this->mAtInfo.mAttackPower);
        OS_REPORT("E_st HP  %d\n", a_this->health);
        i_this->mInvulnerabilityTimer = 10;

        if (i_this->mAtInfo.mHitType == 0x10) {
            i_this->mAction = ACTION_G_S_DAMAGE;
            i_this->mActionPhase = PHASE_INIT;
            a_this->speedF = KREG_F(15) + -10.0f;
        } else if (a_this->health <= 0) {
            i_this->mAction = ACTION_G_END;
            i_this->mActionPhase = G_END_PHASE_10;
            daPy_getPlayerActorClass()->onEnemyDead();
            a_this->speedF = KREG_F(14) + -40.0f;
            daPy_getPlayerActorClass()->onEnemyDead();
            i_this->mDeathFlag = 1;
        } else {
            if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_JUMP) {
                if (daPy_getPlayerActorClass()->checkCutJumpCancelTurn()) {
                    i_this->mInvulnerabilityTimer = NREG_S(7) + 3;
                }
            }

            if (i_this->mAtInfo.mHitType == 1 && (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_TURN_RIGHT || daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_UNK_9)) {
                i_this->mAction = ACTION_G_CHANCE;
                a_this->speedF = KREG_F(14) + -40.0f;
                i_this->mInvulnerabilityTimer = 30;
            } else if (daPy_getPlayerActorClass()->getCutCount() >= 4) {
                i_this->mAction = ACTION_G_CHANCE;
                a_this->speedF = KREG_F(14) + -40.0f;
                i_this->mInvulnerabilityTimer = 20;
            } else {
                i_this->mAction = ACTION_G_DAMAGE;
                a_this->speedF = KREG_F(18) + -15.0f;
            }

            i_this->mActionPhase = PHASE_INIT;
        }
    }

    if (a_this->health <= 10) {
        a_this->health = 0;
        i_this->mSph.SetTgHitMark(CcG_Tg_UNK_MARK_3);
    }

    if (i_this->mDefTimer == 0 && i_this->mDefSph.ChkTgHit()) {
        cXyz sp24, sp30;
        def_se_set(&i_this->mSound, i_this->mDefSph.GetTgHitObj(), 0x34, NULL);
        dScnPly_c::setPauseTimer(4);
        i_this->mDefTimer = 10;
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        sp24.x = KREG_F(0);
        sp24.y = KREG_F(1) + 100.0f;
        sp24.z = KREG_F(2) + 50.0f;
        MtxPosition(&sp24, &sp30);
        sp30 += a_this->current.pos;
        dComIfGp_setHitMark(2, NULL, &sp30, &a_this->shape_angle, NULL, 0);

        if (i_this->mAnm == BCK_ST_DAWNB) {
            anm_init(i_this, BCK_ST_DAWNC, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_ST_V_DAWNC, -1);
        }

        a_this->speedF = KREG_F(17) + -12.0f;
    }
}

static void ground_angle_set(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dComIfGp_getPlayer(0);
    s16 sVar1 = 0;
    s16 sVar2 = 0;
    cXyz spc8, spd4;
    dBgS_GndChk gnd_chk;
    f32 scale = (VREG_F(11) + 58.0f) * l_HIO.basic_size;

    if (i_this->mBgc.GetGroundH() != -1e9f) {
        Vec spbc;
        spbc.x = a_this->current.pos.x;
        spbc.y = a_this->current.pos.y + 100.0f;
        spbc.z = a_this->current.pos.z + scale;
        gnd_chk.SetPos(&spbc);
        spbc.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

        if (spbc.y != -1e9f) {
            sVar2 = -cM_atan2s(spbc.y - a_this->current.pos.y, spbc.z - a_this->current.pos.z);
            if (sVar2 > 0x2000 || sVar2 < -0x2000) {
                sVar2 = 0;
            }
        }

        spbc.x = a_this->current.pos.x + scale;
        spbc.y = a_this->current.pos.y + 100.0f;
        spbc.z = a_this->current.pos.z;
        gnd_chk.SetPos(&spbc);
        spbc.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

        if (spbc.y != -1e9f) {
            sVar1 = (s16)cM_atan2s(spbc.y - a_this->current.pos.y, spbc.x - a_this->current.pos.x);
            if (sVar1 > 0x2000 || sVar1 < -0x2000) {
                sVar1 = 0;
            }
        }

        cLib_addCalcAngleS2(&i_this->field_0x6a4, sVar2, 4, 0x300);
        cLib_addCalcAngleS2(&i_this->field_0x6a2, sVar1, 4, 0x300);
    }
}

static BOOL foot_control(e_st_class* i_this, st_foot* foot_p) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    BOOL rv = FALSE;
    dBgS_LinChk lin_chk;
    lin_chk.Set(&i_this->field_0xf88, &foot_p->mPos, a_this);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        foot_p->field_0x0 += TREG_F(5) + 500.0f;

        if (foot_p->field_0x0 > 20000.0f) {
            foot_p->field_0x0 = 20000.0f;
        } else {
            rv = TRUE;
        }
    } else {
        cLib_addCalc2(&foot_p->field_0x0, TREG_F(6), 1.0f, TREG_F(7) + 1000.0f);
    }

    foot_p->mAngles[0] = foot_p->field_0x0 * l_HIO.field_0x18;

    if (foot_p->field_0x0 > 0.0f) {
        foot_p->mAngles[1] = foot_p->field_0x0 * l_HIO.field_0x1c;
        foot_p->mAngles[2] = foot_p->field_0x0 * l_HIO.field_0x20;
    } else {
        foot_p->mAngles[2] = 0;
        foot_p->mAngles[1] = 0;
    }

    return rv;
}

static cXyz top_pos_data[4] = {
    cXyz(40.0f, 2.0f, 0.0f),
    cXyz(58.0f, 2.0f, 0.0f),
    cXyz(55.0f, 0.0f, 0.0f),
    cXyz(78.0f, 0.0f, 0.0f),
};

static int base_j[8] = {
    JNT_LLEGA_1,
    JNT_LLEGB_1,
    JNT_LLEGC_1,
    JNT_LLEGD_1,
    JNT_RLEGA_1,
    JNT_RLEGB_1,
    JNT_RLEGC_1,
    JNT_RLEGD_1,
};

static int top_j[8] = {
    JNT_LLEGA_3,
    JNT_LLEGB_3,
    JNT_LLEGC_3,
    JNT_LLEGD_3,
    JNT_RLEGA_3,
    JNT_RLEGB_3,
    JNT_RLEGC_3,
    JNT_RLEGD_3,
};

static void foot_control_main(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz sp38;
    st_foot* foot_p = i_this->mStFeet;
    J3DModel* model = i_this->mpModelMorf->getModel();
    int iVar1 = 0;

    for (int i = 0; i < 8; i++, foot_p++) {
        MTXCopy(model->getAnmMtx(top_j[i]), *calc_mtx);
        MtxPosition(&top_pos_data[i & 3], &foot_p->mPos);
        if (foot_control(i_this, foot_p)) {
            iVar1++;
            if (iVar1 < 100) {
                i_this->mpModelMorf->updateDL();
                i--;
                foot_p--;
            }
        }
    }
}

static void action(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz sp9c, spa8;

    if (i_this->arg0 == 2) {
        damage_check_g(i_this);
    } else {
        damage_check(i_this);
    }

    s8 unk_flag_1 = 1;
    s8 unk_flag_2 = 0;
    s8 unk_flag_3 = 0;
    s8 unk_flag_4 = 0;
    s8 unk_flag_5 = 0;
    s8 unk_flag_6 = 0;
    s8 isLinkSearch = 0;
    a_this->field_0x566 = 0;
    i_this->mSph.SetAtSpl((dCcG_At_Spl)0);

    switch (i_this->mAction) {
        case ACTION_WAIT:
            e_st_wait(i_this);
            unk_flag_2 = 1;
            break;

        case ACTION_MOVE:
            e_st_move(i_this);
            unk_flag_2 = 1;
            break;

        case ACTION_PL_SEARCH:
            e_st_pl_search(i_this);
            a_this->field_0x566 = 1;
            unk_flag_2 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_SHOOT:
            e_st_shoot(i_this);
            a_this->field_0x566 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_HANG:
            i_this->field_0xa58 = 0;
            e_st_hang(i_this);
            i_this->field_0xb94 = 1;
            break;

        case ACTION_HANG_SHOOT:
            i_this->field_0xa58 = 0;
            e_st_hang_shoot(i_this);
            a_this->field_0x566 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_HANG_DROP:
            e_st_hang_drop(i_this);
            unk_flag_4 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_HANG_2:
            i_this->field_0xa58 = 2;
            unk_flag_3 = e_st_hang_2(i_this);
            i_this->mSph.SetAtSpl(dCcG_At_Spl_UNK_1);
            i_this->field_0xb94 = 1;
            unk_flag_4 = 1;
            a_this->field_0x566 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_HANG_2_SHOOT:
            i_this->field_0xa58 = 0;
            e_st_hang_2_shoot(i_this);
            a_this->field_0x566 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_JUMP_ATTACK:
            i_this->mSph.SetAtSpl(dCcG_At_Spl_UNK_C);
            i_this->field_0xa58 = 0;
            e_st_jump_attack(i_this);
            a_this->field_0x566 = 1;
            i_this->field_0xb94 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_S_DAMAGE:
            e_st_s_damage(i_this);
            break;

        case ACTION_DAMAGE:
            e_st_damage(i_this);
            unk_flag_1 = 0;
            unk_flag_4 = 1;
            break;

        case ACTION_WATER:
            e_st_water(i_this);
            break;

        case ACTION_G_NORMAL:
            e_st_g_normal(i_this);
            unk_flag_5 = 1;
            a_this->field_0x566 = 1;
            unk_flag_2 = 1;
            unk_flag_4 = 1;
            break;

        case ACTION_G_FIGHT:
            unk_flag_6 = e_st_g_fight(i_this);
            unk_flag_5 = 1;
            a_this->field_0x566 = 1;
            unk_flag_2 = 1;
            unk_flag_4 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_G_DEF:
            unk_flag_6 = e_st_g_def(i_this);
            unk_flag_5 = 1;
            a_this->field_0x566 = 1;
            unk_flag_2 = 1;
            unk_flag_4 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_G_DAMAGE:
            e_st_g_damage(i_this);
            unk_flag_5 = 1;
            a_this->field_0x566 = 1;
            unk_flag_2 = 1;
            unk_flag_4 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_G_S_DAMAGE:
            e_st_g_s_damage(i_this);
            unk_flag_5 = 1;
            a_this->field_0x566 = 1;
            unk_flag_2 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_G_WIND:
            e_st_g_wind(i_this);
            unk_flag_5 = 1;
            a_this->field_0x566 = 1;
            unk_flag_2 = 1;
            isLinkSearch = 1;
            break;

        case ACTION_G_CHANCE:
            e_st_g_chance(i_this);
            unk_flag_5 = 1;
            unk_flag_2 = 1;
            unk_flag_4 = 1;
            i_this->field_0xa58 = 2;
            break;

        case ACTION_G_END:
            unk_flag_1 = e_st_g_end(i_this);
            unk_flag_5 = 1;
            unk_flag_2 = 1;
            unk_flag_4 = 1;
            i_this->field_0xa58 = 2;
            break;
    }

    if (isLinkSearch) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    if (unk_flag_5) {
        dBgS_LinChk lin_chk;
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        sp9c.z = a_this->speedF;
        sp9c.x = 0.0f;
        sp9c.y = 0.0f;
        MtxPosition(&sp9c, &spa8);
        a_this->speed.x = spa8.x;
        a_this->speed.z = spa8.z;
        a_this->current.pos += a_this->speed * l_HIO.basic_size;
        a_this->speed.y -= 5.0f;

        if (a_this->speed.y < -80.0f) {
            a_this->speed.y = -80.0f;
        }

        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        sp9c.x = 0.0f;
        sp9c.y = 200.0f;
        sp9c.z = hREG_F(0) + -200.0f;
        MtxPosition(&sp9c, &spa8);
        spa8 += a_this->current.pos;
        sp9c = a_this->current.pos;
        sp9c.y += 200.0f;
        lin_chk.Set(&sp9c, &spa8, NULL);

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            sp9c.x = 0.0f;
            sp9c.y = 0.0f;
            sp9c.z = TREG_F(11) + fabsf(a_this->speedF) + 30.0f;
            MtxPosition(&sp9c, &spa8);
            a_this->current.pos += spa8;
        }

        i_this->mAcchCir.SetWall(JREG_F(12) + 60.0f, JREG_F(13) + 120.0f - 80.0f + 15.0f);

        if (i_this->field_0xa58 == 1) {
            cXyz* pcVar1 = i_this->mStts.GetCCMoveP();
            if (pcVar1 != NULL) {
                a_this->current.pos.x += pcVar1->x * 0.5f;
                a_this->current.pos.z += pcVar1->z * 0.5f;
                i_this->mBgPos.x += pcVar1->x * 0.5f;
                i_this->mBgPos.z += pcVar1->z * 0.5f;
            }
        }

        i_this->mBgc.CrrPos(dComIfG_Bgsp());
        ground_angle_set(i_this);

        if (unk_flag_2) {
            cLib_addCalc2(&i_this->field_0x6b0.y, a_this->current.pos.y, 0.5f, 25.0f);
            i_this->field_0x6b0.x = a_this->current.pos.x;
            i_this->field_0x6b0.z = a_this->current.pos.z;
        } else {
            i_this->field_0x6b0 = a_this->current.pos;
        }

        a_this->shape_angle.y = a_this->current.angle.y;

        if (unk_flag_6 && (i_this->mFrameCounter & 0x30) != 0) {
            if (daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_GUARD_ATTACK) {
                if (daPy_getPlayerActorClass()->getCutAtFlg() != 0 && !small) {
                    i_this->mAction = ACTION_G_DEF;
                    i_this->mActionPhase = PHASE_INIT;
                    i_this->field_0xcf4 = 1;
                }
            }
        }

        cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 4, 0x800);
        cLib_addCalcAngleS2(&i_this->field_0x69c.y, 0, 1, 0x800);
        cLib_addCalcAngleS2(&i_this->field_0x69c.x, 0, 1, 0x800);
    } else if (unk_flag_2 && i_this->field_0x69c.x == 0) {
        cLib_addCalc2(&i_this->field_0x6b0.y, a_this->current.pos.y, 0.5f, 25.0f);
        i_this->field_0x6b0.x = a_this->current.pos.x;
        i_this->field_0x6b0.z = a_this->current.pos.z;

        if (gr_check(i_this)) {
            i_this->mAction = ACTION_HANG_DROP;
            i_this->mActionPhase = PHASE_INIT;
        }
    } else {
        i_this->field_0x6b0 = a_this->current.pos;
    }

    if (i_this->arg0 == 0 && i_this->arg2 == 1 && a_this->current.pos.y - player->current.pos.y < 0.0f) {
        unk_flag_1 = 0;
    }

    if (unk_flag_1) {
        fopAcM_OnStatus(a_this, 0);
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    }

    if (unk_flag_4 && water_check(i_this)) {
        i_this->mAction = ACTION_WATER;
        i_this->mActionPhase = PHASE_INIT;
        a_this->current.pos.y = i_this->mGroundCross - (WREG_F(11) + 20.0f);
        f32 scale = 2.5f;
        if (small) {
            scale = 0.8f;
        }

        fopKyM_createWpillar(&a_this->current.pos, scale, 0);
        i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_WATER_S, 0, -1);
    }

    if (unk_flag_3) {
        static u16 p_name[2] = {
            dPa_RM(ID_ZI_S_ST_FALLSAND_A),
            dPa_RM(ID_ZI_S_ST_FALLSAND_B),
        };

        for (int i = 0; i < 2; i++) {
            i_this->mPrtcls[i] = dComIfGp_particle_set(i_this->mPrtcls[i], p_name[i], &i_this->field_0x704, NULL, NULL);
        }
    }

    cXyz i_effSize(1.5f, 1.5f, 1.5f);
    setMidnaBindEffect(a_this, &i_this->mSound, &a_this->eyePos, &i_effSize);
}

static int daE_ST_Execute(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    cXyz pos, sph_center;

    i_this->mPlayerDistance = fopAcM_searchPlayerDistance(a_this);
    f32 scale = l_HIO.basic_size;
    if (small) {
        scale = 1.0f;
    }

    i_this->mFrameCounter++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->mInvulnerabilityTimer != 0) {
        i_this->mInvulnerabilityTimer--;
    }

    if (i_this->mDefTimer != 0) {
        i_this->mDefTimer--;
    }

    i_this->field_0xa58 = 1;
    i_this->field_0xb94 = 0;
    action(i_this);
    J3DModel* model = i_this->mpModelMorf->getModel();

    if (i_this->arg0 == 2) {
        mDoMtx_stack_c::transS(i_this->field_0x6b0.x, i_this->field_0x6b0.y, i_this->field_0x6b0.z);
        mDoMtx_stack_c::XrotM(i_this->field_0x6a4 / 2);
        mDoMtx_stack_c::ZrotM(i_this->field_0x6a2 / 2);
        mDoMtx_stack_c::YrotM(a_this->current.angle.y);
        mDoMtx_stack_c::XrotM(a_this->current.angle.x);
        mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
        mDoMtx_stack_c::scaleM(scale, scale, scale);
        model->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        cXyz offset(0.0f, 0.0f, 0.0f);
        s16 sVar1 = 0;

        if (i_this->field_0x7ec > 0.0f) {
            cMtx_YrotS(*calc_mtx, i_this->field_0x7f2);
            pos.x = 0.0f;
            pos.y = 0.0f;
#if VERSION == VERSION_WII_USA_R0
            pos.z = i_this->field_0x7ec * -cM_ssin(i_this->field_0x7f0 * (TREG_S(4) + 900));
#else
            pos.z = i_this->field_0x7ec * -cM_ssin(i_this->field_0x7f0 * (TREG_F(4) + 900));
#endif
            MtxPosition(&pos, &offset);
            i_this->field_0x7f0++;
            cLib_addCalc0(&i_this->field_0x7ec, 1.0f, 0.1f);
            sVar1 = pos.z * (TREG_F(17) + -5.0f);
        }

        mDoMtx_stack_c::transS(i_this->field_0x6b0.x + offset.x, i_this->field_0x6b0.y + i_this->field_0x71c + offset.y, i_this->field_0x6b0.z + offset.z);

        if (sVar1 != 0) {
            mDoMtx_stack_c::YrotM(i_this->field_0x7f2);
            mDoMtx_stack_c::XrotM(sVar1);
            mDoMtx_stack_c::YrotM(-i_this->field_0x7f2);
        }

        i_this->field_0x71c = 0.0f;
        mDoMtx_stack_c::YrotM(i_this->field_0x69c.y);
        mDoMtx_stack_c::XrotM(i_this->field_0x69c.x);
        mDoMtx_stack_c::XrotM(i_this->field_0x6a4);
        mDoMtx_stack_c::ZrotM(i_this->field_0x6a2);
        mDoMtx_stack_c::YrotM(a_this->current.angle.y);
        mDoMtx_stack_c::XrotM(a_this->current.angle.x);
        mDoMtx_stack_c::ZrotM(a_this->current.angle.z);
        mDoMtx_stack_c::scaleM(scale, scale, scale);
        model->setBaseTRMtx(mDoMtx_stack_c::get());
        MTXCopy(mDoMtx_stack_c::get(), *calc_mtx);

        pos.set(0.0f, ZREG_F(0) + 30.0f, 0.0f);
        MtxPosition(&pos, &i_this->field_0x6bc);
        pos.set(0.0f, ZREG_F(1) + 110.0f, ZREG_F(2) + 100.0f);
        MtxPosition(&pos, &i_this->field_0x6c8);
        pos.set(0.0f, ZREG_F(3) + -50.0f, ZREG_F(4) + 100.0f);
        MtxPosition(&pos, &i_this->field_0x6d4);
        pos.set(ZREG_F(6) + 80.0f, ZREG_F(0) + 30.0f, 0.0f);
        MtxPosition(&pos, &i_this->field_0x6e0[0]);
        pos.x *= -1.0f;
        MtxPosition(&pos, &i_this->field_0x6e0[1]);
        pos.set(0.0f, ZREG_F(0) + 30.0f, ZREG_F(5) + -120.0f);
        MtxPosition(&pos, &i_this->field_0x6e0[2]);
    }

    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));

    if ((i_this->mAnm == BCK_ST_MOVE || i_this->mAnm == BCK_ST_TURN) && (i_this->mpModelMorf->checkFrame(3.0f) || i_this->mpModelMorf->checkFrame(7.0f) ||
        i_this->mpModelMorf->checkFrame(11.0f) || i_this->mpModelMorf->checkFrame(15.0f) || i_this->mpModelMorf->checkFrame(19.0f) ||
        i_this->mpModelMorf->checkFrame(23.0f) || i_this->mpModelMorf->checkFrame(27.0f) || i_this->mpModelMorf->checkFrame(31.0f) ||
        i_this->mpModelMorf->checkFrame(35.0f) || i_this->mpModelMorf->checkFrame(39.0f))) {
        i_this->mSound.startCreatureSound(Z2SE_EN_ST_FOOTNOTE, 0, -1);
    }

    i_this->mpModelMorf->modelCalc();

    if (i_this->arg0 == 2) {
        if (i_this->field_0xf85 > 10) {
            if (!mDoCPd_c::getHoldZ(PAD_2)) {
                foot_control_main(i_this);
            }
        } else {
            i_this->field_0xf85++;
        }
    }

    MTXCopy(model->getAnmMtx(JNT_HEAD), *calc_mtx);
    pos.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&pos, &a_this->eyePos);
    a_this->attention_info.position = a_this->eyePos;
    a_this->attention_info.position.y += KREG_F(8) + 120.0f;

    if (i_this->arg0 == 2) {
        pos.set(BREG_F(0) + 65.0f, BREG_F(1), BREG_F(2));
    } else {
        pos.set(BREG_F(0) + 35.0f, BREG_F(1), BREG_F(2));
    }

    MtxPosition(&pos, &i_this->field_0x704);
    MTXCopy(model->getAnmMtx(JNT_WAIST), *calc_mtx);
    pos.set(JREG_F(1) + 40.0f, JREG_F(2), JREG_F(3) + -20.0f);
    MtxPosition(&pos, &sph_center);

    if (i_this->mInvulnerabilityTimer != 0 || i_this->field_0xcf4 != 0) {
        sph_center.x += 100000.0f;
    }
    i_this->mSph.SetC(sph_center);
    i_this->mSph.SetR((JREG_F(4) + 55.0f) * scale);
    if (i_this->field_0xa58 != 0) {
        i_this->mSph.OnCoSetBit();
    } else {
        i_this->mSph.OffCoSetBit();
    }
    dComIfG_Ccsp()->Set(&i_this->mSph);

    sph_center = i_this->field_0x704;
    if (i_this->field_0xb94 == 0 || i_this->mInvulnerabilityTimer != 0 || i_this->field_0xcf4 != 0) {
        sph_center.x -= 100000.0f;
    }
    i_this->mAtSph.SetC(sph_center);
    i_this->mAtSph.SetR((JREG_F(5) + 30.0f) * scale);
    dComIfG_Ccsp()->Set(&i_this->mAtSph);

    if (i_this->arg0 == 2) {
        pos.set(AREG_F(6), AREG_F(7), AREG_F(8));
        MtxPosition(&pos, &i_this->field_0xf88);
        pos.set(AREG_F(9) + 90.0f, AREG_F(10), AREG_F(11) + -50.0f);
        MtxPosition(&pos, &i_this->field_0x710);
    } else {
        pos.set(BREG_F(6) + 120.0f, BREG_F(7), BREG_F(8) + -3.0f);
        MtxPosition(&pos, &i_this->field_0x710);
    }

    pos.set(BREG_F(16), BREG_F(17), BREG_F(18));
    MtxPosition(&pos, &sph_center);
    if (i_this->field_0xcf4 == 0) {
        sph_center.y -= 20000.0f;
    }
    i_this->mDefSph.SetC(sph_center);
    i_this->mDefSph.SetR((JREG_F(14) + 50.0f) * scale);
    dComIfG_Ccsp()->Set(&i_this->mDefSph);

    i_this->field_0xcf4 = 0;

    if (i_this->field_0x720 == 1) {
        roof_line_calc(i_this);
    } else if (i_this->field_0x720 == 2) {
        roof_line_calc2(i_this);
    }

    if (i_this->field_0x760 != 0) {
        tail_line_calc(i_this);
        cLib_addCalc0(&i_this->field_0x764, 1.0f, 2.0f);

        if (i_this->field_0x764 < 1.0f) {
            i_this->field_0x760 = 0;
        }
    }

    if (i_this->mDeathFlag != 0) {
        cLib_addCalc2(&i_this->mColor, -20.0f, 1.0f, 0.4f);
    }

    if (i_this->arg0 == 2 && a_this->home.pos.y - a_this->current.pos.y > 2000.0f) {
        fopAcM_delete(a_this);
        if (i_this->mSwBit != 0xFF) {
            dComIfGs_onSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this));
        }
    }

    a_this->attention_info.flags |= fopAc_AttnFlag_UNK_0x200000;
    return 1;
}

static int daE_ST_IsDelete(e_st_class* i_this) {
    return 1;
}

static int daE_ST_Delete(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;

    dComIfG_resDelete(&i_this->mPhase, "E_st");
    
    if (i_this->mIsFirstSpawn) {
        hio_set = false;
    }

    if (a_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    e_st_class* i_this = (e_st_class*)a_this;

    int i_index;
    if (small) {
        i_index = BMDR_ST;
    } else {
        i_index = BMDR_ST2;
    }

    i_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_st", i_index), NULL, NULL,
                                               (J3DAnmTransform*)dComIfG_getObjectRes("E_st", BCK_ST_WAIT), 0, 1.0f, 0, -1,
                                               &i_this->mSound, 0x80000, 0x11000084);
    if (i_this->mpModelMorf == NULL || i_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    if (i_this->field_0x720 != 0) {
        if (!i_this->mLineMat1.init(1, 20, 1)) {
            return 0;
        }

        f32* size_p = i_this->mLineMat1.getSize(0);
        for (int i = 0; i < 20; i++, size_p++) {
            *size_p = XREG_S(1) * 0.1f + 2.0f;
        }

        if (!i_this->mLineMat2.init(1, 20, 1)) {
            return 0;
        }

        size_p = i_this->mLineMat2.getSize(0);
        for (int i = 0; i < 20; i++, size_p++) {
            *size_p = XREG_S(1) * 0.1f + 2.0f;
        }
    }

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setUserArea((uintptr_t)a_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    return 1;
}

static s16 ya[6] = {
    -0x4000, 0x4000, 0x0000, -0x8000, 0x0000, 0x0000,
};

static s16 xa[6] = {
    -0x4000, -0x4000, -0x4000, -0x4000, 0x0000, -0x8000,
};

static int kabe_initial_pos_set(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz sp94, spa0;

    a_this->current.pos.y += 30.0f;

    for (int i = 0; i < 6; i++) {
        MtxTrans(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z, 0);
        cMtx_YrotM(*calc_mtx, ya[i]);
        cMtx_XrotM(*calc_mtx, xa[i]);
        sp94.x = 0.0f;
        sp94.y = -200.0f;
        sp94.z = 0.0f;
        MtxPosition(&sp94, &spa0);
        lin_chk.Set(&a_this->current.pos, &spa0, NULL);

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            i_this->mBgPos = a_this->current.pos;
            i_this->field_0x69c.y = ya[i];
            i_this->field_0x69c.x = xa[i];
            bg_pos_get(i_this);
            OS_REPORT("//////////////E_ST ON BG !! %d\n", i);
            return 1;
        }
    }

    return 0;
}

static BOOL roof_initial_pos_set(e_st_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->actor;
    dBgS_LinChk lin_chk;
    cXyz end;
    
    a_this->current.pos.y += 80.0f;
    a_this->home.pos.y = a_this->current.pos.y;
    end = a_this->current.pos;
    end.y += 5000.0f;
    lin_chk.Set(&a_this->current.pos, &end, NULL);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        i_this->field_0x744 = lin_chk.GetCross();
        OS_REPORT("//////////////E_ST ON ROOF !! \n");
        return TRUE;
    }

    return FALSE;
}

static cPhs__Step daE_ST_Create(fopAc_ac_c* a_this) {
    e_st_class* i_this = (e_st_class*)a_this;
    fopAcM_ct(a_this, e_st_class);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->mPhase, "E_st");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_st PARAM %x\n", fopAcM_GetParam(a_this));
        i_this->arg0 = fopAcM_GetParam(a_this) & 0xF;
        if (i_this->arg0 == 0xF) {
            i_this->arg0 = 0;
        }

        i_this->arg1 = (fopAcM_GetParam(a_this) & 0xF0) >> 4;
        if (i_this->arg1 == 0xF) {
            i_this->arg1 = 0;
        }

        i_this->arg2 = fopAcM_GetParam(a_this) >> 8;
        if (i_this->arg2 == 0xFF) {
            i_this->arg2 = 0;
        }

        i_this->arg3 = fopAcM_GetParam(a_this) >> 16;
        if (i_this->arg3 == 0xFF) {
            i_this->arg3 = 0;
        }

        i_this->mSwBit = fopAcM_GetParam(a_this) >> 24;
        if (i_this->mSwBit != 0xFF) {
            if (dComIfGs_isSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this))) {
                return cPhs_ERROR_e;
            }
        }

        if (i_this->arg0 == 0) {
            if (TREG_S(9) == 0) {
                if (!kabe_initial_pos_set(i_this)) {
                    OS_REPORT("//////////////E_ST NON BG !!\n");
                    return cPhs_ERROR_e;
                }

                i_this->mAction = ACTION_WAIT;
            } else {
                i_this->mAction = ACTION_UNK_100;
            }
        } else if (i_this->arg0 == 1) {
            if (!roof_initial_pos_set(i_this)) {
                OS_REPORT("//////////////E_ST NON ROOF !!\n");
                return cPhs_ERROR_e;
            }

            if (i_this->arg1 == 2 || i_this->arg1 == 4) {
                i_this->mAction = ACTION_HANG_2;
                i_this->field_0x7d8 = i_this->arg2 * 10.0f;
            } else {
                i_this->mAction = ACTION_HANG;
                i_this->field_0x7d8 = i_this->arg2 * 10.0f;
            }

            i_this->field_0x720 = 1;
            a_this->current.angle.x = 0x4000;
        } else if (i_this->arg0 == 2) {
            i_this->mAction = ACTION_G_NORMAL;
        }

        small = FALSE;

        if (strcmp(dComIfGp_getStartStageName(), "D_MN05") == 0 && fopAcM_GetRoomNo(a_this) == 1) {
            small = TRUE;
        }

        OS_REPORT("E_st//////////////E_ST SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x2800)) {
            OS_REPORT("//////////////E_ST SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_ST SET 2 !!\n");

        if (!hio_set) {
            i_this->mIsFirstSpawn = true;
            hio_set = true;
            l_HIO.id = -1;
        }
        
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(a_this, i_this->mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(a_this, -200.0f, -4000.0f, -200.0f);
        fopAcM_SetMax(a_this, 200.0f, 4000.0f, 200.0f);

        if (i_this->arg3 == 0) {
            a_this->attention_info.distances[fopAc_attn_BATTLE_e] = 3;
        } else {
            a_this->attention_info.distances[fopAc_attn_BATTLE_e] = 4;
        }

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0xd}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };

        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 30.0f} // mSph
            } // mSphAttr
        };

        static dCcD_SrcCyl line_cyl_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {
                    {0.0f, 0.0f, 0.0f}, // mCenter
                    30.0f, // mRadius
                    2000.0f // mHeight
                } // mLineCyl
            }
        };

        static dCcD_SrcSph def_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_STONE, 0x5, 0x0, 0x0, 0x3}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 100.0f} // mSph
            } // mSphAttr
        };

        i_this->mStts.Init(100, 0, a_this);
        i_this->mSph.Set(cc_sph_src);
        i_this->mSph.SetStts(&i_this->mStts);
        i_this->mAtSph.Set(at_sph_src);
        i_this->mAtSph.SetStts(&i_this->mStts);
        i_this->mDefSph.Set(def_sph_src);
        i_this->mDefSph.SetStts(&i_this->mStts);

        if (i_this->field_0x720 != 0) {
            i_this->mLineCyl.Set(line_cyl_src);
            i_this->mLineCyl.SetStts(&i_this->mStts);
            i_this->mLineCyl.OnTgNoHitMark();
        }

        i_this->mSound.init(&a_this->current.pos, &a_this->eyePos, 3, 1);
        i_this->mSound.setEnemyName("E_st");
        i_this->mAtInfo.mpSound = &i_this->mSound;
        i_this->mAtInfo.mPowerType = 7;
        i_this->mTimers[1] = 100;
        fopAcM_SetParam(a_this, 0);

        i_this->mBgc.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1, &i_this->mAcchCir,
                         fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(60.0f, 60.0f);

        if (small) {
            a_this->health = 0;
            a_this->field_0x560 = 0;
        } else {
            a_this->health = 80;
            a_this->field_0x560 = 80;
        }

        i_this->mFrameCounter = cM_rndF(65535.0f);
        i_this->field_0x724 = 1.0f;
        i_this->field_0x6b0 = a_this->current.pos;
        daE_ST_Execute(i_this);
        daE_ST_Execute(i_this);
    }

    return phase;
}

AUDIO_INSTANCES;

static actor_method_class l_daE_ST_Method = {
    (process_method_func)daE_ST_Create,
    (process_method_func)daE_ST_Delete,
    (process_method_func)daE_ST_Execute,
    (process_method_func)daE_ST_IsDelete,
    (process_method_func)daE_ST_Draw,
};

actor_process_profile_definition g_profile_E_ST = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_ST,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_st_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  129,                    // mPriority
  &l_daE_ST_Method,       // sub_method
  0x10040100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
