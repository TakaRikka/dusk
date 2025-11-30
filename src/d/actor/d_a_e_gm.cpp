/**
 * @file d_a_e_gm.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_gm.h"
#include "f_op/f_op_actor_enemy.h"
#include "d/actor/d_a_cstatue.h"
#include "d/actor/d_a_b_gm.h"
#include "f_op/f_op_camera_mng.h"
#include "d/d_msg_object.h"
#include "Z2AudioLib/Z2Instances.h"

enum E_gm_RES_File_ID {
    /* BCK */
    /* 0x5 */ BCK_GB_APPEAR = 0x5,
    /* 0x6 */ BCK_GB_DAMAGE,
    /* 0x7 */ BCK_GB_DEAD,
    /* 0x8 */ BCK_GB_MOVE,
    /* 0x9 */ BCK_GB_WAIT,

    /* BMDE */
    /* 0xC */ BMDE_GOMA_EGG = 0xC,

    /* BMDV */
    /* 0xF */ BMDV_GB = 0xF,
};

enum E_mg_RES_File_ID {
    /* BCK */
    /* 0x4 */ BCK_MD_DAMAGE = 0x4,
    /* 0x5 */ BCK_MD_DEATH,
    /* 0x6 */ BCK_MD_DEATH_PRESS,
    /* 0x7 */ BCK_MD_JUMP,
    /* 0x8 */ BCK_MD_PIKU,
    /* 0x9 */ BCK_MD_UP,
    /* 0xA */ BCK_MD_WALK,

    /* BMDV */
    /* 0xD */ BMDV_MD = 0xD,
};

enum Joint {
    /* 0x00 */ JNT_WORLD_ROOT,
    /* 0x01 */ JNT_BELLY,
    /* 0x02 */ JNT_HEAD,
    /* 0x03 */ JNT_AGOL,
    /* 0x04 */ JNT_AGOR,
    /* 0x05 */ JNT_LEGL1A,
    /* 0x06 */ JNT_LEGL1B,
    /* 0x07 */ JNT_LEGL1C,
    /* 0x08 */ JNT_LEGL2A,
    /* 0x09 */ JNT_LEGL2B,
    /* 0x0A */ JNT_LEGL2C,
    /* 0x0B */ JNT_LEGR1A,
    /* 0x0C */ JNT_LEGR1B,
    /* 0x0D */ JNT_LEGR1C,
    /* 0x0E */ JNT_LEGR2A,
    /* 0x0F */ JNT_LEGR2B,
    /* 0x10 */ JNT_LEGR2C,
};

enum Type {
    /* 0x0 */ TYPE_0,
    /* 0x1 */ TYPE_1,
    /* 0x2 */ TYPE_NORMAL,
    /* 0x3 */ TYPE_GOMA,
};

class daE_GM_HIO_c {
public:
    daE_GM_HIO_c();
    virtual ~daE_GM_HIO_c() {}

    /* 子ゴーマ - Baby Ghoma */
    /* 0x04 */ s8 id;
    /* 0x08 */ f32 model_size;
    /* 0x0C */ f32 basic_speed;
    /* 0x10 */ f32 max_acceleration;

    /* ダンジョン用子ゴーマ - Dungeon Baby Ghoma */
    /* 0x14 */ f32 escape_speed;
    /* 0x18 */ f32 player_run_pursuit_speed;
    /* 0x1C */ f32 player_walk_pursuit_speed;
    /* 0x20 */ f32 player_search_y_limit;

    /* 0x24 */ s16 min_acceleration_cycle;
};

namespace {
    /* 806D7B34-806D7B38 000038 0004+00 2/3 0/0 0/0 .data l_hitActorID__22@unnamed@d_a_e_gm_cpp@ */
    static fpc_ProcID l_hitActorID = fpcM_ERROR_PROCESS_ID_e;

    /* 806D7B38-806D7B3C 00003C 0004+00 2/4 0/0 0/0 .data l_coreActorID__22@unnamed@d_a_e_gm_cpp@ */
    static fpc_ProcID l_coreActorID = fpcM_ERROR_PROCESS_ID_e;

    /* 806D7B3C-806D7B7C 000040 0040+00 0/1 0/0 0/0 .data cc_sph_src__22@unnamed@d_a_e_gm_cpp@ */
    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xD}, {0xDAFBFDFF, 0x3}, 0x75}},
            {dCcD_SE_HARD_BODY, 0, 0, 0, 0},
            {dCcD_SE_NONE, 0, 0, 0, 2},
            {0x0},
        },
        {
            {{0.0f, 0.0f, 0.0f}, 30.0f},
        }
    };
} // namespace

daE_GM_HIO_c::daE_GM_HIO_c() {
    id = -1;
    model_size = 1.3f;
    basic_speed = 8.0f;
    max_acceleration = 10.0f;
    min_acceleration_cycle = 0x380;
    escape_speed = 20.0f;
    player_run_pursuit_speed = 20.0f;
    player_walk_pursuit_speed = 15.0f;
    player_search_y_limit = 300.0f;
}

void daE_GM_c::setAction(actionFunc action) {
    if (mAction) {
        mMode = -1;
        (this->*mAction)();
    }

    mOldAction = mAction;
    mAction = action;
    mMode = 0;
    (this->*mAction)();
}

static u8 l_initHIO;

static daE_GM_HIO_c l_HIO;

namespace {
    /* 806D7EF8-806D7F04 000088 000C+00 0/3 0/0 0/0 .bss             l_corePos__22@unnamed@d_a_e_gm_cpp@ */
    static cXyz l_corePos(0.0f, 0.0f, 0.0f);

    /* 806D7F10-806D7F18 0000A0 0006+02 0/3 0/0 0/0 .bss             l_coreAngle__22@unnamed@d_a_e_gm_cpp@ */
    static csXyz l_coreAngle;

    /* 806D7F18-806D7F1C 0000A8 0004+00 0/2 0/0 0/0 .bss             l_coreSpeedF__22@unnamed@d_a_e_gm_cpp@ */
    static f32 l_coreSpeedF;
} // namespace

/* 806D7F1C -00001 0004+00 11/14 0/0 0/0 .bss             None */
static u8 lbl_195_bss_AC;

/* 806D7F1D 0001+00 data_806D7F1D None */
static u8 lbl_195_bss_AD;

/* 806D7F1E 0001+00 data_806D7F1E None */
static u8 lbl_195_bss_AE;

/* 806D7F1F 0001+00 data_806D7F1F None */
static u8 lbl_195_bss_AF;

void daE_GM_c::action() {
    if (field_0xa76) {
        if (checkAction(&daE_GM_c::egg_wait) || checkAction(&daE_GM_c::rebound)) {
            mSph.SetAtType(0);
            mSph.SetTgType(0);
            cLib_addCalc2(&mColor, 100.0f, 1.0f, 8.0f);

            if (mColor <= 100.0f && lbl_195_bss_AE == 0) {
                setDeathLightEffect();
                fopAcM_seStart(this, JA_SE_CM_MONS_EXPLODE, 0);
                fopAcM_delete(this);
                lbl_195_bss_AE++;
                return;
            }
        }
    }

    (this->*mAction)();
}

static void* s_obj_sub2(void* i_actor, void* i_data) {
    if (!fpcM_IsCreating(fopAcM_GetID(i_actor)) && fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_CSTATUE &&
        fopAcM_searchActorDistanceXZ((fopAc_ac_c*)i_actor, (fopAc_ac_c*)i_data) < 4000.0f && ((daCstatue_c*)i_actor)->checkBossAtGroundHit()) {
        return i_actor;
    }

    return NULL;
}

void daE_GM_c::egg_wait() {
    if (mMode == 0) {
        cXyz sp3c, sp48;

        mSph.SetAtType(AT_TYPE_0);
        field_0xa69 = 0;
        maxFallSpeed = -100.0f;
        mAcchCir.SetWall(scale.y * 50.0f, scale.x * 50.0f);

        if (checkOldAction(&daE_GM_c::rebound)) {
            speedF = 0.0f;
            speed.y = 0.0f;
            gravity = -5.0f;
        } else {
            speedF = cM_rndFX(10.0f) + 15.0f;
            sp3c.set(0.0f, 0.0f, speedF);
            mDoMtx_stack_c::ZXYrotS(current.angle);
            mDoMtx_stack_c::multVec(&sp3c, &sp48);
            speed.y = sp48.y;
            gravity = -10.0f;
            field_0xa6a = 0;

            if (mType == TYPE_1) {
                field_0xa71 = false;
                current.angle.y = cM_rndFX(32768.0f);
            } else {
                mSound.startCreatureSound(Z2SE_EN_GBA_LAYEGG, 0, -1);
                current.angle.y += (int)cM_rndFX(8192.0f);
            }
        }

        mMode++;
    } else if (mMode == -1) {
        mSph.SetAtType(AT_TYPE_CSTATUE_SWING);
    } else if (mMode == 1) {
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mObjAcch.CrrPos(dComIfG_Bgsp());
        cLib_chaseF(&field_0xa10.y, field_0xa50 + 1.0f, 0.1f);
        cLib_chaseF(&field_0xa10.x, 1.0f, 0.05f);
        cLib_chaseF(&field_0xa10.z, 1.0f, 0.05f);

        if (mObjAcch.ChkGroundHit()) {
            daCstatue_c* statue_p = (daCstatue_c*)fpcM_Search(s_obj_sub2, this);
            if (statue_p != NULL) {
                field_0xa1c.y = cLib_targetAngleY(&statue_p->current.pos, &current.pos);
                speedF = cM_rndFX(5.0f);
                setAction(&daE_GM_c::rebound);
            } else {
                field_0xa10.set(1.2f, 0.6f, 1.2f);
                field_0xa50 = hREG_F(7) + 0.6f;
                field_0xa64 = 4000;
                speedF = 0.0f;
                field_0xa5c = 0x400;

                if (mType == TYPE_0) {
                    mSound.startCreatureSound(Z2SE_EN_GBA_LAND, 0, -1);
                }

                mMode++;
            }
        } else {
            current.angle.x += 0x400;
            current.angle.z += 0x400;
        }
    } else {
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mObjAcch.CrrPos(dComIfG_Bgsp());
        cLib_chaseF(&field_0xa10.y, field_0xa50 + 1.0f, 0.1f);
        cLib_chaseF(&field_0xa10.x, 1.0f, 0.05f);
        cLib_chaseF(&field_0xa10.z, 1.0f, 0.05f);

        if (mObjAcch.ChkGroundHit()) {
            daCstatue_c* statue_p2 = (daCstatue_c*)fpcM_Search(s_obj_sub2, this);
            if (statue_p2 != NULL) {
                field_0xa1c.y = cLib_targetAngleY(&statue_p2->current.pos, &current.pos);
                speedF = cM_rndFX(5.0f);
                setAction(&daE_GM_c::rebound);
                return;
            }
        }
        
        if (mType == TYPE_1) {
            cLib_addCalcAngleS(&field_0xa5c, 0, 8, 0x100, 4);
            current.angle.x += field_0xa5c;

            if (field_0xa71 || hREG_F(29) != 0.0f) {
                field_0xa6a++;
                if (field_0xa6a > field_0xa5e) {
                    field_0xa10.setall(1.0f);
                    current.angle.x = 0;
                    current.angle.z = 0;
                    setAction(&daE_GM_c::appear);
                }
            }
        } else if (!field_0xa76) {
            field_0xa6a++;

            if (field_0xa6a < 8) {
                cLib_addCalcAngleS(&field_0xa5c, 0, 8, 0x100, 4);
                current.angle.x += field_0xa5c;
            } else if (field_0xa6a > 100) {
                current.angle.set(0, cM_rndFX(32768.0f), 0);
                field_0xa10.setall(1.0f);
                setAction(&daE_GM_c::appear);
            }
        }
    }
}

void daE_GM_c::appear() {
    if (mMode == 0) {
        field_0xa69 = 1;
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gm", BCK_GB_APPEAR), 0, 3.0f, 1.0f, 0.0f, -1.0f);
        mAcchCir.SetWall(scale.y * 10.0f, scale.x * 30.0f);
        mSound.startCreatureSound(Z2SE_EN_GBA_FUKA, 0, -1);
        speedF = 0.0f;
        gravity = -5.0f;
        field_0xa44 = 0.0f;
        setEggBreakEffect();
        mMode++;
    } else if (mMode != -1) {
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mObjAcch.CrrPos(dComIfG_Bgsp());

        if (mObjAcch.ChkGroundHit()) {
            daCstatue_c* statue_p = (daCstatue_c*)fpcM_Search(s_obj_sub2, this);
            if (statue_p != NULL) {
                field_0xa1c.y = cLib_targetAngleY(&statue_p->current.pos, &current.pos);
                speedF = cM_rndFX(5.0f);
                setAction(&daE_GM_c::rebound);
            } else if (mpModelMorf->isStop()) {
                if (mType == TYPE_1) {
                    setAction(&daE_GM_c::walk2);
                } else {
                    setAction(&daE_GM_c::walk);
                }
            }
        }
    }
}

static s16 const l_rand[4] = {
    0xF800, 0x0400, 0xFC00, 0x0800,
};

static s16 const l_rand2[4] = {
    0xD000, 0x3800, 0xB800, 0x5000,
};

static f32 const l_rand_base_spd[4] = {
    -2.0f, 0.0f, 0.0f, 2.0f,
};

void daE_GM_c::walk() {
    if (mMode == 0) {
        field_0xa69 = 1;
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gm", BCK_GB_MOVE), 2, 3.0f, 2.0f, 0.0f, -1.0f);
        field_0xa44 = 0.0f;
        mAcchCir.SetWall(scale.y * 10.0f, scale.x * 30.0f);
        field_0xa6b = 0;
        field_0xa6c = 0;
        field_0xa5e = 0;
        mSph.SetAtType(AT_TYPE_CSTATUE_SWING);
        mMode++;
    } else if (mMode != -1) {
        daPy_py_c* player = daPy_getPlayerActorClass();

        if (field_0xa76) {
            setAction(&daE_GM_c::dead_wait);
        } else {
            b_gm_class* actor_p;
            if (fopAcM_SearchByID(fopAcM_GetLinkId(this), (fopAc_ac_c**)&actor_p) != 0 && actor_p != NULL && actor_p->mDemoMode == 0x15) {
                setAction(&daE_GM_c::wait);
            } else {
                speedF = l_HIO.basic_speed + l_HIO.max_acceleration * 0.5f + l_HIO.max_acceleration * 0.5f * cM_ssin(field_0xa5e);
                field_0xa5e += l_HIO.min_acceleration_cycle;
                mpModelMorf->setPlaySpeed(speedF / 6.5f);
                fopAcM_posMoveF(this, mStts.GetCCMoveP());
                mObjAcch.CrrPos(dComIfG_Bgsp());

                daCstatue_c* statue_p = (daCstatue_c*)fpcM_Search(s_obj_sub2, this);
                if (statue_p != NULL) {
                    field_0xa1c.y = cLib_targetAngleY(&statue_p->current.pos, &current.pos);
                    speedF = cM_rndFX(5.0f);
                    setAction(&daE_GM_c::rebound);
                } else {
                    if (field_0xa6c == 0) {
                        if (current.pos.absXZ(player->current.pos) > 400.0f || (player->getDamageWaitTimer() == 0 && field_0xa72 == 0 && lbl_195_bss_AC == 0)) {
                            field_0xa1c.y = l_rand[(mRandIndex & 12) >> 2] + cLib_targetAngleY(&current.pos, &player->current.pos);
                        } else {
                            field_0xa1c.y = l_rand2[(mRandIndex & 12) >> 2] + cLib_targetAngleY(&current.pos, &player->current.pos);
                        }
                    }

                    if (mAcchCir.ChkWallHit()) {
                        field_0xa6c = cM_rndF(30.0f) + 30.0f;
                        field_0xa1c.y = mAcchCir.GetWallAngleY();
                    }

                    cLib_addCalcAngleS(&current.angle.y, field_0xa1c.y, 0x20, 0x400, 0x100);
                    cLib_addCalcAngleS(&current.angle.x, 0, 4, 0x4000, 0x1000);
                    cLib_addCalcAngleS(&current.angle.z, 0, 4, 0x4000, 0x1000);
                    field_0xa6e += (int)(speedF / 6.5f);

                    if (field_0xa6e >= 4) {
                        field_0xa6e = 0;
                        mSound.startCreatureSound(Z2SE_EN_GBA_FOOTNOTE, 0, -1);
                    }
                }
            }
        }
    }
}

void daE_GM_c::create_item() {
    if (mType == TYPE_NORMAL) {
        fopAcM_createItemFromEnemyID(field_0x564, &current.pos, -1, -1, NULL, NULL, NULL, NULL);
    } else {
        f32 fVar1;
        if ((int)dComIfGs_getLife() < 5) {
            fVar1 = 9.0f;
        } else {
            fVar1 = 11.0f;
        }

        switch ((int)cM_rndF(fVar1)) {
            case 0:
                fopAcM_createItem(&current.pos, fpcNm_ITEM_HEART, -1, -1, NULL, NULL, 0);
                break;

            case 1:
            case 2:
                fopAcM_createItem(&current.pos, fpcNm_ITEM_ARROW_1, -1, -1, NULL, NULL, 0);
                break;
        }
    }
}

void daE_GM_c::pechanko_damage() {
    if (mMode == 0) {
        mSph.SetAtType(AT_TYPE_0);
        mSph.SetTgType(0);
        attention_info.flags = 0;

        if (field_0xa69 == 1) {
            mpModelMorf->setPlaySpeed(0.0f);
        }

        field_0xa6b = 60;
        speedF = 0.0f;
        gravity = -5.0f;
        maxFallSpeed = -100.0f;
        field_0xa5c = cM_rndF(10.0f) + 60.0f;
        current.angle.z = 0;
        current.angle.x = 0;
        mSound.startCreatureSound(Z2SE_EN_GBA_V_DEATH, 0, -1);
        field_0xa74 = 0;
        mMode++;
    } else if (mMode != -1) {
        cLib_chaseF(&field_0xa10.y, 0.4f, 0.2f);
        cLib_chaseF(&field_0xa10.x, 1.3f, 0.1f);
        cLib_chaseF(&field_0xa10.z, 1.3f, 0.1f);
        cLib_addCalc2(&mColor, 100.0f, 1.0f, 8.0f);

        if (mObjAcch.ChkGroundHit()) {
            if (field_0xa5c != 0) {
                field_0xa5c--;
            } else if (mColor <= 100.0f && lbl_195_bss_AE == 0) {
                setDeathLightEffect();
                fopAcM_seStart(this, JA_SE_CM_MONS_EXPLODE, 0);
                create_item();
                fopAcM_delete(this);
                lbl_195_bss_AE++;
            }
        } else {
            fopAcM_posMoveF(this, NULL);
            mObjAcch.CrrPos(dComIfG_Bgsp());
        }
    }
}

void daE_GM_c::wait() {
    if (mMode == 0) {
        field_0xa69 = 1;
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gm", BCK_GB_WAIT), 2, 3.0f, 1.0f, 0.0f, -1.0f);
        speedF = 0.0f;
        mMode++;
    } else if (mMode != -1) {
        fopAcM_posMoveF(this, NULL);
        mObjAcch.CrrPos(dComIfG_Bgsp());

        b_gm_class* actor_p;
        if (fopAcM_SearchByID(fopAcM_GetLinkId(this), (fopAc_ac_c**)&actor_p) == 0 || actor_p == NULL) {
            return;
        }
        
        if (actor_p->mDemoMode != 0x15) {
            setAction(&daE_GM_c::walk);
        }
    }
}

void daE_GM_c::egg_damage() {
    if (mMode == 0) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        mSph.SetAtType(AT_TYPE_0);
        mSph.SetTgType(0);
        field_0xa69 = 0;
        speedF = cM_rndFX(10.0f) + 30.0f;
        speed.y = cM_rndFX(10.0f) + 40.0f;
        gravity = -5.0f;
        maxFallSpeed = -100.0f;
        current.angle.y = cLib_targetAngleY(&player->current.pos, &current.pos);
        current.angle.z = 0;
        field_0xa10.set(1.1f, 0.8f, 1.1f);
        attention_info.flags = 0;
        field_0xa50 = hREG_F(7) + 0.8f;
        field_0xa64 = 4000;
        field_0xa5c = cM_rndF(10.0f);
        mSound.startCreatureSound(Z2SE_EN_GBA_V_DEATH, 0, -1);
        field_0xa74--;
        mMode++;
    } else if (mMode == -1) {
        mSph.SetAtType(AT_TYPE_CSTATUE_SWING);
    } else {
        cLib_addCalc2(&mColor, 100.0f, 1.0f, 8.0f);
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mObjAcch.CrrPos(dComIfG_Bgsp());

        if (mObjAcch.ChkGroundHit()) {
            speedF = 0.0f;
            cLib_chaseF(&field_0xa10.y, 0.8f, 0.1f);
            cLib_chaseF(&field_0xa10.x, 1.1f, 0.05f);
            cLib_chaseF(&field_0xa10.z, 1.1f, 0.05f);

            if (field_0xa5c != 0) {
                field_0xa5c--;
            } else if (mColor <= 100.0f && lbl_195_bss_AE == 0) {
                setDeathLightEffect();
                fopAcM_seStart(this, JA_SE_CM_MONS_EXPLODE, 0);
                create_item();
                fopAcM_delete(this);
                lbl_195_bss_AE++;
            }
        } else {
            current.angle.x += 0x400;
            cLib_chaseF(&field_0xa10.y, 1.0f, 0.1f);
            cLib_chaseF(&field_0xa10.x, 1.0f, 0.05f);
            cLib_chaseF(&field_0xa10.z, 1.0f, 0.05f);
        }
    }
}

static u16 const l_eff_id[4] = {
    dPa_RM(ID_ZI_S_DEADEFLIGHT_A), dPa_RM(ID_ZI_S_DEADEFLIGHT_B), dPa_RM(ID_ZI_S_DEADEFLIGHT_C), dPa_RM(ID_ZI_S_DEADEFLIGHT_D),
};

void daE_GM_c::setDeathLightEffect() {
    for (int i = 0; i < 4; i++) {
        dComIfGp_particle_set(l_eff_id[i], &current.pos, &tevStr, NULL, NULL);
    }
}

static u16 l_egg_eff_id[2] = {
    dPa_RM(ID_ZI_S_GBEGG_HARETSU_A), dPa_RM(ID_ZI_S_GBEGG_HARETSU_B),
};

void daE_GM_c::setEggBreakEffect() {
    for (int i = 0; i < 2; i++) {
        dComIfGp_particle_set(l_egg_eff_id[i], &current.pos, &tevStr, NULL, NULL);
    }
}

void daE_GM_c::damage() {
    if (mMode == 0) {
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gm", BCK_GB_DAMAGE), 2, 3.0f, 1.0f, 0.0f, -1.0f);
        attention_info.flags = 0;
        field_0xa44 = 16.0f;
        field_0xa69 = 1;
        speedF = cM_rndFX(5.0f) + 15.0f;
        speed.y = cM_rndFX(10.0f) + 50.0f;
        gravity = -5.0f;
        maxFallSpeed = -100.0f;
        field_0xa5c = 0;
        field_0xa5e = cM_rndF(10.0f);
        field_0xa1c.x = 0x7300;
        field_0xa1c.z = 0;
        mSph.SetTgType(0);
        mSph.SetAtType(AT_TYPE_0);
        mSound.startCreatureSound(Z2SE_EN_GBA_V_DEATH, 0, -1);

        if (mType == TYPE_NORMAL && mSwBit != 0xFF) {
            dComIfGs_onSwitch(mSwBit, fopAcM_GetRoomNo(this));
        }

        field_0xa74--;
        mMode++;
    } else if (mMode != -1) {
        f32 fVar1 = speed.y;
        cLib_addCalc2(&mColor, 100.0f, 1.0f, 8.0f);
        speed.x = speedF * cM_ssin(field_0xa1c.y);
        speed.y += gravity;

        if (speed.y < maxFallSpeed) {
            speed.y = maxFallSpeed;
        }

        speed.z = speedF * cM_scos(field_0xa1c.y);
        fopAcM_posMove(this, mStts.GetCCMoveP());
        mObjAcch.CrrPos(dComIfG_Bgsp());

        if (mType != TYPE_GOMA && field_0xa5c == 0 && fVar1 > 0.0f && speed.y < 0.0f) {
            mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gm", BCK_GB_DEAD), 2, 8.0f, 1.0f, 0.0f, -1.0f);
            field_0xa5c++;
        }

        cLib_addCalcAngleS(&current.angle.x, field_0xa1c.x, 8, 0x1000, 0x400);
        cLib_addCalcAngleS(&current.angle.z, field_0xa1c.z, 8, 0x1000, 0x400);

        if (mObjAcch.ChkGroundHit() && fVar1 <= 0.0f) {
            speed.y = -fVar1 * 0.5f;
            speedF *= 0.5f;

            if (speed.y < 10.0f) {
                speed.y = 0.0f;
                speedF = 0.0f;

                if (field_0xa5e != 0) {
                    field_0xa5e--;
                } else if (mColor <= 100.0f && lbl_195_bss_AE == 0) {
                    setDeathLightEffect();
                    fopAcM_seStart(this, JA_SE_CM_MONS_EXPLODE, 0);
                    create_item();
                    fopAcM_delete(this);
                    lbl_195_bss_AE++;
                }
            }
        }
    }
}

void daE_GM_c::core_damage() {
    if (mMode == 0) {
        bool bVar1 = false;
        field_0xa69 = 2;
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_mg", BCK_MD_DAMAGE), 0, 3.0f, 1.0f, 0.0f, -1.0f);
        field_0xa74--;

        if (current.angle.x != 0 || current.angle.z != 0) {
            bVar1 = true;
            field_0xa1c.z = 0;
            field_0xa1c.x = 0;
        }

        if (field_0xa74 != 0) {
            daPy_py_c* player = daPy_getPlayerActorClass();

            if (bVar1) {
                field_0xa1c.y = cLib_targetAngleY(&player->current.pos, &current.pos);
                speed.z = 0.0f;
                speed.x = 0.0f;
                speed.y = 30.0f;
            } else {
                field_0xa1c.y = current.angle.y;
                speed.setall(0.0f);
            }

            mSound.startCreatureVoice(Z2SE_EN_MD_V_DAMAGE, -1);
            gravity = -5.0f;
            maxFallSpeed = -100.0f;
            mMode = 3;
        } else {
            field_0xa68 = 1;
            lbl_195_bss_AF = 1;

            if (bVar1) {
                speed.z = 0.0f;
                speed.x = 0.0f;
                speed.y = 30.0f;
            } else {
                speed.setall(0.0f);
            }

            gravity = -5.0f;
            maxFallSpeed = -100.0f;
            mSph.SetTgType(0);
            mSph.SetAtType(AT_TYPE_0);
            mMode++;
        }
    } else if (mMode != -1) {
        if (mMode == 1) {
            cLib_addCalcAngleS(&current.angle.x, field_0xa1c.x, 4, 0x2000, 0x400);
            cLib_addCalcAngleS(&current.angle.z, field_0xa1c.z, 4, 0x2000, 0x400);
            speed.y += gravity;

            if (maxFallSpeed > speed.y) {
                speed.y = maxFallSpeed;
            }

            fopAcM_posMove(this, mStts.GetCCMoveP());
            mObjAcch.CrrPos(dComIfG_Bgsp());

            if (mObjAcch.ChkGroundHit() && current.angle.x == 0 && current.angle.z == 0 && mpModelMorf->isStop()) {
                mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_mg", BCK_MD_DEATH), 0, 3.0f, 1.0f, 0.0f, -1.0f);
                mSound.startCreatureVoice(Z2SE_EN_MD_V_DEATH, -1);
                mMode++;
            }
        } else if (mMode == 2) {
            speed.y += gravity;

            if (maxFallSpeed > speed.y) {
                speed.y = maxFallSpeed;
            }

            fopAcM_posMove(this, mStts.GetCCMoveP());
            mObjAcch.CrrPos(dComIfG_Bgsp());

            if (mpModelMorf->getFrame() > 73.0f) {
                cLib_addCalc2(&mColor, 75.0f, 1.0f, 2.0f);
            }

            if (mpModelMorf->isStop()) {
                setAction(&daE_GM_c::core_delwait);
            }
        } else {
            cLib_addCalcAngleS(&current.angle.x, field_0xa1c.x, 4, 0x1000, 0x400);
            cLib_addCalcAngleS(&current.angle.y, field_0xa1c.y, 4, 0x1000, 0x400);
            cLib_addCalcAngleS(&current.angle.z, field_0xa1c.z, 4, 0x1000, 0x400);
            speed.y += gravity;

            if (maxFallSpeed > speed.y) {
                speed.y = maxFallSpeed;
            }

            fopAcM_posMove(this, mStts.GetCCMoveP());
            mObjAcch.CrrPos(dComIfG_Bgsp());

            if (mObjAcch.ChkGroundHit()) {
                speed.z = 0.0f;
                speed.x = 0.0f;
                mpModelMorf->setPlaySpeed(1.0f);
            } else if (mpModelMorf->getFrame() > 12.0f) {
                mpModelMorf->setPlaySpeed(0.0f);
            }

            if (mpModelMorf->isStop()) {
                setAction(&daE_GM_c::core_escape);
            }
        }
    }
}

void daE_GM_c::core_delwait() {
    if (mMode == 0) {
        field_0xa75 = 0;
        fopAcM_seStart(this, JA_SE_CM_MONS_EXPLODE, 0);
        fopAcM_createDisappear(this, &current.pos, 20, 0, 0xFF);
        field_0xa6b = 36;
        mMode++;
    } else if (mMode != -1 && field_0xa6b == 0 && lbl_195_bss_AE == 0) {
        fopAcM_delete(this);
        Z2GetAudioMgr()->seStart(Z2SE_OBJ_KAGERI_MIRROR_GET, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        lbl_195_bss_AE++;
    }
}

void daE_GM_c::core_piku() {
    if (mMode == 0) {
        field_0xa69 = 2;
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_mg", BCK_MD_PIKU), 0, 3.0f, 1.0f, 0.0f, -1.0f);
        mMode++;
    } else if (mMode != -1) {
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mObjAcch.CrrPos(dComIfG_Bgsp());

        if (mObjAcch.ChkGroundHit()) {
            daCstatue_c* statue_p = (daCstatue_c*)fpcM_Search(s_obj_sub2, this);
            if (statue_p != NULL) {
                field_0xa1c.y = cLib_targetAngleY(&statue_p->current.pos, &current.pos);
                speedF = cM_rndFX(5.0f);
                setAction(&daE_GM_c::rebound);
            } else {
                speedF = 0.0f;

                if (mpModelMorf->isStop()) {
                    setAction(&daE_GM_c::core_escape);
                }
            }
        }
    }
}

void daE_GM_c::core_pechanko_damage() {
    if (mMode == 0) {
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_mg", BCK_MD_DEATH_PRESS), 2, 3.0f, 1.0f, 0.0f, -1.0f);
        field_0xa6b = 180;
        mSph.SetTgType(0);
        mSph.SetAtType(AT_TYPE_0);
        field_0xa68 = 2;
        lbl_195_bss_AF = 1;
        speedF = 0.0f;
        gravity = -5.0f;
        maxFallSpeed = -100.0f;
        current.angle.z = 0;
        current.angle.x = 0;
        mSound.startCreatureVoice(Z2SE_EN_MD_V_DEATH_PRESS, -1);
        field_0xa74 = 0;
        mMode++;
    } else if (mMode != -1) {
        cLib_addCalc2(&mColor, 75.0f, 1.0f, 2.0f);

        if (mObjAcch.ChkGroundHit()) {
            if (field_0xa6b == 0 && mColor == 75.0f) {
                setAction(&daE_GM_c::core_delwait);
            }
        } else {
            fopAcM_posMoveF(this, NULL);
            mObjAcch.CrrPos(dComIfG_Bgsp());
        }
    }
}

void daE_GM_c::hook() {
    if (mMode == 0) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        cXyz sp28, sp34;

        if (player->getHookshotTopPos()->y > current.pos.y + 60.0f + hREG_F(0)) {
            sp28 = mSph.GetC() - current.pos;
            sp34 = player->current.pos;
            sp34.y += 80.0f;
            sp34 -= eyePos;

            if (sp34.normalizeRS()) {
                sp28.x += sp34.x * 140.0f;
                sp28.y += sp34.y * 140.0f;
                sp28.z += sp34.z * 140.0f;
            }
        } else {
            sp28 = *player->getHookshotTopPos() - current.pos;
        }

        player->setHookshotCarryOffset(fopAcM_GetID(this), &sp28);
        mSph.SetTgType(0);
        mMode++;
    } else if (mMode == -1) {
        field_0xa72 = 10;
        mSph.SetTgType(0xD8FBFDFF);
    } else if (!fopAcM_CheckStatus(this, fopAcM_STATUS_HOOK_CARRY_NOW)) {
        setAction(&daE_GM_c::core_escape);
    }
}

void daE_GM_c::rebound() {
    if (mMode == 0) {
        daPy_getPlayerActorClass();
        field_0xa44 = 16.0f;
        field_0xa1c.x = 0x7300;

        if (field_0xa69 == 1) {
            mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gm", BCK_GB_DAMAGE), 2, 3.0f, 1.0f, 0.0f, -1.0f);
        } else if (field_0xa69 == 2) {
            field_0xa44 = 60.0f;
            field_0xa1c.x = 0x6000;
            mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_mg", BCK_MD_WALK), 2, 3.0f, 1.0f, 0.0f, -1.0f);
        }

        mSph.SetAtType(AT_TYPE_0);
        speed.y = cM_rndFX(10.0f) + 50.0f;
        gravity = -5.0f;
        maxFallSpeed = -100.0f;
        field_0xa1c.z = 0;
        mMode++;
    } else if (mMode != -1) {
        if (mMode == 1) {
            speed.x = speedF * cM_ssin(field_0xa1c.y);
            speed.y += gravity;

            if (speed.y < maxFallSpeed) {
                speed.y = maxFallSpeed;
            }

            speed.z = speedF * cM_scos(field_0xa1c.y);
            fopAcM_posMove(this, mStts.GetCCMoveP());
            mObjAcch.CrrPos(dComIfG_Bgsp());
            cLib_addCalcAngleS(&current.angle.x, field_0xa1c.x, 8, 0x1000, 0x400);
            current.angle.y += 0x1000;
            cLib_addCalcAngleS(&current.angle.z, field_0xa1c.z, 8, 0x1000, 0x400);

            if (mObjAcch.ChkGroundHit()) {
                daCstatue_c* statue_p = (daCstatue_c*)fpcM_Search(s_obj_sub2, this);
                if (statue_p != NULL) {
                    field_0xa1c.y = cLib_targetAngleY(&statue_p->current.pos, &current.pos);
                    speedF = cM_rndFX(5.0f);
                    setAction(&daE_GM_c::rebound);
                } else {
                    speedF = 0.0f;

                    if (field_0xa69 == 0) {
                        setAction(&daE_GM_c::egg_wait);
                    } else {
                        field_0xa6b = 40;
                        mMode++;
                    }
                }
            }
        } else if (mMode == 2) {
            cLib_addCalcAngleS(&current.angle.x, field_0xa1c.x, 8, 0x1000, 0x400);
            cLib_addCalcAngleS(&current.angle.z, field_0xa1c.z, 8, 0x1000, 0x400);

            daCstatue_c* statue_p2 = (daCstatue_c*)fpcM_Search(s_obj_sub2, this);
            if (statue_p2 != NULL) {
                field_0xa1c.y = cLib_targetAngleY(&statue_p2->current.pos, &current.pos);
                speedF = cM_rndFX(5.0f);
                setAction(&daE_GM_c::rebound);
            } else if (field_0xa6b == 0) {
                field_0xa1c.z = 0;
                field_0xa1c.x = 0;
                mMode++;
            }
        } else {
            cLib_addCalcAngleS(&current.angle.x, field_0xa1c.x, 0x10, 0x2000, 0x400);
            cLib_addCalcAngleS(&current.angle.z, field_0xa1c.z, 0x10, 0x2000, 0x400);

            if (current.angle.x == 0 && current.angle.z == 0) {
                if (mType == TYPE_1) {
                    setAction(&daE_GM_c::walk2);
                } else if (mType == TYPE_GOMA) {
                    setAction(&daE_GM_c::core_escape);
                } else if (mType == TYPE_NORMAL) {
                    setAction(&daE_GM_c::normal_wait);
                } else if (mType == TYPE_0 && field_0xa76) {
                    setAction(&daE_GM_c::dead_wait);
                } else {
                    setAction(&daE_GM_c::walk);
                }
            } else {
                daCstatue_c* statue_p3 = (daCstatue_c*)fpcM_Search(s_obj_sub2, this);
                if (statue_p3 != NULL) {
                    field_0xa1c.y = cLib_targetAngleY(&statue_p3->current.pos, &current.pos);
                    speedF = cM_rndFX(5.0f);
                    setAction(&daE_GM_c::rebound);
                }
            }
        }
    }
}

void daE_GM_c::walk2() {
    if (mMode == 0) {
        field_0xa69 = 1;
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gm", BCK_GB_MOVE), 2, 3.0f, 2.0f, 0.0f, -1.0f);
        gravity = -5.0f;
        field_0xa40 = 0.0f;
        field_0xa44 = 0.0f;
        speedF = l_coreSpeedF + field_0xa40;
        mSph.SetAtType(AT_TYPE_CSTATUE_SWING);
        mMode++;
    } else if (mMode != -1) {
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mObjAcch.CrrPos(dComIfG_Bgsp());

        if (mObjAcch.ChkGroundHit()) {
            daCstatue_c* statue_p = (daCstatue_c*)fpcM_Search(s_obj_sub2, this);
            if (statue_p != NULL) {
                field_0xa1c.y = cLib_targetAngleY(&statue_p->current.pos, &current.pos);
                speedF = cM_rndFX(5.0f);
                setAction(&daE_GM_c::rebound);
                return;
            }

            if (l_coreActorID == fpcM_ERROR_PROCESS_ID_e || (l_coreActorID != fpcM_ERROR_PROCESS_ID_e && lbl_195_bss_AF != 0)) {
                setAction(&daE_GM_c::dead_wait);
                return;
            }
        }

        f32 fVar1 = current.pos.absXZ(l_corePos);
        s16 sVar1;
        s16 sVar2 = cLib_targetAngleY(&current.pos, &l_corePos);

        if (fVar1 < 350.0f) {
            s16 sVar3 = l_coreAngle.y;
            cLib_chaseF(&field_0xa40, 0.0f, 0.1f);
            sVar1 = sVar3;
        } else {
            sVar1 = sVar2;
            cLib_chaseF(&field_0xa40, 4.0f, 0.1);
        }

        cLib_chaseAngleS(&current.angle.y, sVar1, 0x400);
        speedF = l_coreSpeedF + field_0xa40;
        mpModelMorf->setPlaySpeed(speedF / 9.0f);
        field_0xa6e += (int)(speedF / 9.0f);
        
        if (field_0xa6e >= 4) {
            field_0xa6e = 0;
            mSound.startCreatureSound(Z2SE_EN_GBA_FOOTNOTE, 0, -1);
        }
    }
}

void daE_GM_c::dead_wait() {
    if (mMode == 0) {
        field_0xa69 = 1;
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gm", BCK_GB_WAIT), 2, 3.0f, 1.0f, 0.0f, -1.0f);
        speedF = 0.0f;

        if (mType != TYPE_0) {
            field_0xa6b = 60;
        } else {
            field_0xa6b = 0;
        }

        mMode++;
    } else if (mMode != -1) {
        if (mMode == 1) {
            fopAcM_posMoveF(this, mStts.GetCCMoveP());
            mObjAcch.CrrPos(dComIfG_Bgsp());

            if (field_0xa6b == 0) {
                mSph.SetTgType(0);
                mSph.SetAtType(AT_TYPE_0);
                mMode++;
            }
        } else {
            fopAcM_posMoveF(this, mStts.GetCCMoveP());
            mObjAcch.CrrPos(dComIfG_Bgsp());
            cLib_addCalc2(&mColor, 100.0f, 1.0f, 8.0f);

            if (mColor <= 100.0f && lbl_195_bss_AE < 2) {
                setDeathLightEffect();
                fopAcM_seStart(this, JA_SE_CM_MONS_EXPLODE, 0);
                fopAcM_delete(this);
                lbl_195_bss_AE++;
            }
        }
    }
}

static f32 dummy() {
    return 1000.0f;
}

void daE_GM_c::core_wait() {
    if (mMode == 0) {
        daPy_py_c* player = daPy_getPlayerActorClass();

        field_0xa69 = 2;
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_mg", BCK_MD_UP), 0, 3.0f, 0.0f, 0.0f, -1.0f);
        field_0xa44 = 0.0f;
        gravity = -5.0f;
        speed.y = 0.0f;
        speedF = 0.0f;
        field_0xa71 = false;
        current.angle.y = cLib_targetAngleY(&player->current.pos, &current.pos);
        fopAcM_SetCullSize(this, 7);
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x80000);
        mSph.SetTgType(0xD8FBFDFF);
        mMode++;
    } else if (mMode != -1) {
        if (mMode == 1) {
            fopAcM_posMoveF(this, mStts.GetCCMoveP());
            mObjAcch.CrrPos(dComIfG_Bgsp());

            if (field_0xa71 || hREG_F(29) != 0.0f) {
                mpModelMorf->setPlaySpeed(1.0f);
                mMode++;
            }
        } else {
            fopAcM_posMoveF(this, mStts.GetCCMoveP());
            mObjAcch.CrrPos(dComIfG_Bgsp());

            if (mpModelMorf->isStop()) {
                setAction(&daE_GM_c::core_escape);
            }
        }
    }
}

void daE_GM_c::core_escape() {
    if (mMode == 0) {
        field_0xa69 = 2;
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_mg", BCK_MD_WALK), 2, 3.0f, 1.0f, 0.0f, -1.0f);
        field_0xa44 = 0.0f;
        field_0xa40 = 0.0f;
        mSph.SetAtType(AT_TYPE_CSTATUE_SWING);
        mMode++;
    } else if (mMode != -1) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        cXyz sp6c(0.0f, 0.0f, 0.0f);

        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mObjAcch.CrrPos(dComIfG_Bgsp());

        daCstatue_c* statue_p = (daCstatue_c*)fpcM_Search(s_obj_sub2, this);
        if (statue_p != NULL) {
            field_0xa1c.y = cLib_targetAngleY(&statue_p->current.pos, &current.pos);
            speedF = cM_rndFX(5.0f);
            setAction(&daE_GM_c::rebound);
        } else {
            f32 fVar1 = current.pos.absXZ(player->current.pos);
            f32 fVar2 = sp6c.absXZ(current.pos);
            s16 sVar1 = (s16)cLib_targetAngleY(&player->current.pos, &current.pos);
            s16 sVar2 = cLib_targetAngleY(&sp6c, &current.pos);
            s16 sVar3 = fopCamM_GetAngleY(dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0)));

            if (fVar1 < 800.0f) {
                field_0xa6d = 60;
            }

            f32 fVar3;
            if (field_0xa6d != 0) {
                fVar3 = 10.0f;
            } else {
                fVar3 = 0.0f;
            }

            if (field_0xa6d != 0) {
                field_0xa5c = 0x400;
            } else if (field_0xa6b == 0) {
                field_0xa6b = cM_rndF(30.0f) + 30.0f;
                field_0xa5c = cM_rndF(256.0f) + 256.0f;
            }

            if (fVar1 < 2000.0f) {
                if (fVar2 > 2000.0f) {
                    field_0xa1c.y = sVar2 - 0x8000;
                    field_0xa5c = 0x200;
                } else if (abs(sVar3 - sVar1) < 0x6000) {
                    if (sVar3 < sVar1) {
                        field_0xa1c.y = sVar3 + 0x4000;
                    } else {
                        field_0xa1c.y = sVar3 - 0x4000;
                    }
                } else if (field_0xa6c == 0) {
                    field_0xa6c = cM_rndF(30.0f) + 30.0f;
                    field_0xa1c.y = cM_rndFX(32768.0f);
                }
            } else if (fVar2 > 2000.0f) {
                field_0xa1c.y = sVar2 - 0x8000;
                field_0xa5c = 0x200;
            } else if (field_0xa6c == 0) {
                field_0xa6c = cM_rndF(30.0f) + 30.0f;
                field_0xa1c.y = cM_rndFX(32768.0f);
            }

            mpModelMorf->setPlaySpeed(speedF / 18.0f);
            cLib_chaseS(&field_0xa60, field_0xa5c, 0x10);
            cLib_chaseAngleS(&current.angle.y, field_0xa1c.y, field_0xa60);
            cLib_addCalcAngleS(&current.angle.x, 0, 4, 0x4000, 0x1000);
            cLib_addCalcAngleS(&current.angle.z, 0, 4, 0x4000, 0x1000);

            if (player->speedF < 15.0f) {
                cLib_chaseF(&field_0xa40, fVar3, 0.1f);
            } else {
                cLib_chaseF(&field_0xa40, fVar3, 1.0f);
            }

            speedF = (field_0xa40 + 13.0f) + l_HIO.max_acceleration * 0.5f + (l_HIO.max_acceleration * 0.5f * cM_ssin(field_0xa5e));
            field_0xa5e += l_HIO.min_acceleration_cycle;
            field_0xa6e += (int)(speedF / 18.0f);

            if (field_0xa6e >= 5) {
                field_0xa6e = 0;
                mSound.startCreatureSound(Z2SE_EN_GBA_FOOTNOTE, 0, -1);
            }
        }
    }
}

BOOL daE_GM_c::chaseCheck() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (std::abs(current.pos.y - player->current.pos.y) >= l_HIO.player_search_y_limit) {
        return FALSE;
    }

    if (current.pos.absXZ(player->current.pos) > field_0xa54) {
        return FALSE;
    }

    s16 sVar1 = cLib_targetAngleY(&player->current.pos, &current.pos);
    if (abs(sVar1 - player->shape_angle.y) >= 0x4000) {
        return TRUE;
    }

    return FALSE;
}

void daE_GM_c::normal_wait() {
    if (mMode == 0) {
        field_0xa69 = 1;
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gm", BCK_GB_MOVE), 2, 3.0f, 1.0f, 0.0f, -1.0f);
        field_0xa4c = 0.0f;
        gravity = -5.0f;
        maxFallSpeed = -100.0f;
        field_0xa1c.y = current.angle.y;
        field_0xa6e = 0;
        field_0xa5c = 0;
        mMode++;
    } else if (mMode != -1) {
        daPy_py_c* player = daPy_getPlayerActorClass();

        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mObjAcch.CrrPos(dComIfG_Bgsp());

        if (chaseCheck()) {
            setAction(&daE_GM_c::normal_chase);
        } else {
            f32 fVar1 = current.pos.absXZ(player->current.pos);
            f32 fVar2 = std::abs(current.pos.y - player->current.pos.y);
            s16 sVar1 = cLib_targetAngleY(&current.pos, &player->current.pos);

            if (fVar2 < l_HIO.player_search_y_limit && fVar1 < 400.0f) {
                field_0xa4c = l_HIO.escape_speed;
                field_0xa5c = 0x400;
                field_0xa1c.y = sVar1 - 0x8000;
                mSph.SetAtType(AT_TYPE_CSTATUE_SWING);
            } else {
                field_0xa4c = 5.0f;
                field_0xa5c = 0x200;

                if (field_0xa6b == 0) {
                    field_0xa6b = cM_rndF(30.0f) + 30.0f;
                    s16 sVar1 = cLib_targetAngleY(&current.pos, &home.pos);
                    field_0xa1c.y = sVar1 + cM_rndFX(16384.0f);
                }

                mSph.SetAtType(AT_TYPE_0);
            }

            if (mAcchCir.ChkWallHit()) {
                field_0xa5c = 0x200;
                field_0xa1c.y = mAcchCir.GetWallAngleY();
            }

            cLib_chaseF(&speedF, field_0xa4c, 1.0f);
            cLib_addCalcAngleS(&current.angle.y, field_0xa1c.y, 0x10, 0x800, field_0xa5c);
            cLib_addCalcAngleS(&current.angle.x, 0, 4, 0x4000, 0x1000);
            cLib_addCalcAngleS(&current.angle.z, 0, 4, 0x4000, 0x1000);

            f32 fVar3 = 0.0f;
            if (mMode == 1) {
                if (speedF != fVar3) {
                    mpModelMorf->setPlaySpeed(speedF / 10.0f);
                } else if (abs(field_0xa1c.y - current.angle.y) < 0x400) {
                    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gm", BCK_GB_WAIT), 2, 3.0f, 1.0f, 0.0f, -1.0f);
                    mMode = 2;
                } else {
                    mpModelMorf->setPlaySpeed(1.0f);
                }
            } else if (mMode == 2 && (speedF != fVar3 || abs(field_0xa1c.y - current.angle.y) > 0x400)) {
                mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gm", BCK_GB_MOVE), 2, 3.0f, 1.0f, 0.0f, -1.0f);
                mMode = 1;
            }

            field_0xa6e += (int)mpModelMorf->getPlaySpeed();

            if (field_0xa6e >= 4) {
                field_0xa6e = 0;
                mSound.startCreatureSound(Z2SE_EN_GBA_FOOTNOTE, 0, -1);
            }
        }
    }
}

void daE_GM_c::normal_chase() {
    if (mMode == 0) {
        field_0xa69 = 1;
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_gm", BCK_GB_MOVE), 2, 3.0f, 1.0f, 0.0f, -1.0f);
        field_0xa4c = 0.0f;
        gravity = -5.0f;
        maxFallSpeed = -100.0f;
        field_0xa6e = 0;
        field_0xa6d = 0;
        mSph.SetAtType(AT_TYPE_CSTATUE_SWING);
        mMode++;
    } else if (mMode != -1) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mObjAcch.CrrPos(dComIfG_Bgsp());

        if (!chaseCheck()) {
            setAction(&daE_GM_c::normal_wait);
        } else {
            if (field_0xa6d == 0) {
                field_0xa6d = 30;
                home.pos = current.pos;
            }

            s16 sVar1 = cLib_targetAngleY(&current.pos, &player->current.pos);
            if (abs(current.angle.y - sVar1) < 0x2000) {
                if (player->speedF > 19.0f) {
                    field_0xa4c = l_HIO.player_run_pursuit_speed;
                } else {
                    field_0xa4c = l_HIO.player_walk_pursuit_speed;
                }
            } else {
                field_0xa4c = 4.0f;
            }

            if (field_0xa6c == 0) {
                if (player->getDamageWaitTimer() != 0 || field_0xa72 != 0 || lbl_195_bss_AC != 0) {
                    field_0xa1c.y = l_rand2[(mRandIndex & 12) >> 2] + cLib_targetAngleY(&current.pos, &player->current.pos);
                } else {
                    field_0xa1c.y = sVar1;
                }
            }

            if (mAcchCir.ChkWallHit()) {
                field_0xa6c = cM_rndF(30.0f) + 30.0f;
                field_0xa1c.y = mAcchCir.GetWallAngleY();
            }

            cLib_chaseF(&speedF, field_0xa4c, 0.4f);
            cLib_addCalcAngleS(&current.angle.y, field_0xa1c.y, 0x20, 0x400, 0x100);
            cLib_addCalcAngleS(&current.angle.x, 0, 4, 0x4000, 0x1000);
            cLib_addCalcAngleS(&current.angle.z, 0, 4, 0x4000, 0x1000);
            mpModelMorf->setPlaySpeed(speedF / 7.5f);
            field_0xa6e += (int)(speedF / 7.5f);

            if (field_0xa6e >= 4) {
                field_0xa6e = 0;
                mSound.startCreatureSound(Z2SE_EN_GBA_FOOTNOTE, 0, -1);
            }
        }
    }
}

void daE_GM_c::mtx_set() {
    J3DModel* model;

    mDoMtx_stack_c::transS(current.pos);
    if (field_0xa69 == 1) {
        mDoMtx_stack_c::transM(0.0f, field_0xa44 * scale.y, 0.0f);
        mDoMtx_stack_c::ZXYrotM(current.angle);
        mDoMtx_stack_c::transM(0.0f, -field_0xa44 * scale.y, 0.0f);
        model = mpModelMorf->getModel();
        model->setBaseScale(scale);
        model->setBaseTRMtx(mDoMtx_stack_c::get());
        mpModelMorf->modelCalc();
        fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
    } else if (field_0xa69 == 2) {
        mDoMtx_stack_c::transM(0.0f, field_0xa44 * scale.y, 0.0f);
        mDoMtx_stack_c::ZXYrotM(current.angle);
        mDoMtx_stack_c::transM(0.0f, -field_0xa44 * scale.y, 0.0f);
        model = mpModelMorf->getModel();
        model->setBaseScale(scale);
        model->setBaseTRMtx(mDoMtx_stack_c::get());
        mpModelMorf->modelCalc();
        fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
    } else {
        cLib_chaseS(&field_0xa64, field_0xa66, 0x80);
        cLib_chaseF(&field_0xa50, hREG_F(7) + 0.1f, 0.04f);
        field_0xa62 += field_0xa64;
        mDoMtx_stack_c::transM(0.0f, (scale.y + field_0xa50) * 50.0f, 0.0f);
        mDoMtx_stack_c::ZrotM(field_0xa62);
        mDoMtx_stack_c::XrotM(field_0xa62);
        mDoMtx_stack_c::scaleM(scale.x, scale.y + field_0xa50, scale.z);
        mDoMtx_stack_c::XrotM(-field_0xa62);
        mDoMtx_stack_c::ZrotM(-field_0xa62);
        mDoMtx_stack_c::ZXYrotM(current.angle);
        mpEggModel->setBaseTRMtx(mDoMtx_stack_c::get());
        fopAcM_SetMtx(this, mpEggModel->getBaseTRMtx());
    }
}

void daE_GM_c::cc_set() {
    cXyz sp24, sp30, i_effPos, i_effSize;

    if (field_0xa69 == 1) {
        J3DModel* model = mpModelMorf->getModel();
        mDoMtx_stack_c::copy(model->getBaseTRMtx());
        sp24.set(0.0f, scale.y * 20.0f, scale.z * -15.0f);
        mDoMtx_stack_c::multVec(&sp24, &eyePos);
        attention_info.position = eyePos;
        attention_info.position.y += scale.y * 20.0f;

        sp24.set(0.0f, scale.y * 30.0f, scale.z * -15.0f);
        mDoMtx_stack_c::multVec(&sp24, &sp30);
        mSph.SetC(sp30);
        mSph.SetR(scale.y * 30.0f);
        dComIfG_Ccsp()->Set(&mSph);
        i_effPos = eyePos;
        i_effSize.setall(0.8f);
    } else if (field_0xa69 == 2) {
        mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(JNT_HEAD));
        sp24.set(0.0f, scale.y * 60.0f, 0.0f);
        mDoMtx_stack_c::multVec(&sp24, &eyePos);
        attention_info.position = eyePos;
        attention_info.position.y += scale.y * 60.0f;

        sp24.set(scale.x * -100.0f, scale.y * 50.0f, 0.0f);
        mDoMtx_stack_c::multVec(&sp24, &sp30);
        mSph.SetC(sp30);
        mSph.SetR(scale.y * 120.0f);
        dComIfG_Ccsp()->Set(&mSph);
        i_effPos = sp30;
        i_effSize.setall(2.0f);
    } else {
        mDoMtx_stack_c::copy(mpEggModel->getBaseTRMtx());
        sp24.set(0.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::multVec(&sp24, &eyePos);
        attention_info.position = eyePos;
        attention_info.position.y += scale.y * 50.0f;

        sp24.set(0.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::multVec(&sp24, &sp30);
        mSph.SetC(sp30);
        mSph.SetR(scale.y * 50.0f);
        dComIfG_Ccsp()->Set(&mSph);
        i_effPos = eyePos;
        i_effSize.setall(1.0f);
    }

    setMidnaBindEffect(this, &mSound, &i_effPos, &i_effSize);
}

int daE_GM_c::draw() {
    if (field_0xa75 == 0) {
        return 1;
    }

    J3DModel* model;
    J3DModelData* modelData;

    if (field_0xa69 == 1) {
        model = mpModelMorf->getModel();
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);

        modelData = model->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* matNode_p = modelData->getMaterialNodePointer(i);
            matNode_p->getMatColor(0)->r = mColor;
            matNode_p->getMatColor(0)->g = mColor;
            matNode_p->getMatColor(0)->b = mColor;
            matNode_p->getTevKColor(3)->r = mColor;
            matNode_p->getTevKColor(3)->g = mColor;
            matNode_p->getTevKColor(3)->b = mColor;
        }

        fopAcM_setEffectMtx(this, model->getModelData());
        mpModelMorf->entryDL();
        dComIfGd_setSimpleShadow(&current.pos, mObjAcch.GetGroundH(), hREG_F(11) + 30.0f, mObjAcch.m_gnd, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    } else if (field_0xa69 == 2) {
        model = mpModelMorf->getModel();
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);

        modelData = model->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* matNode_p = modelData->getMaterialNodePointer(i);
            matNode_p->getTevColor(0)->r = mColor - 255.0f;
            matNode_p->getTevColor(0)->g = mColor - 255.0f;
            matNode_p->getTevColor(0)->b = mColor - 255.0f;
            matNode_p->getMatColor(0)->r = mColor;
            matNode_p->getMatColor(0)->g = mColor;
            matNode_p->getMatColor(0)->b = mColor;
        }

        fopAcM_setEffectMtx(this, model->getModelData());
        mpModelMorf->entryDL();
        cXyz sp50(current.pos.x, current.pos.y + 100.0f, current.pos.z);
        mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &sp50, 700.0f, 0.0f, current.pos.y,
                                        mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f,
                                        dDlst_shadowControl_c::getSimpleTex());
    } else {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mpEggModel, &tevStr);

        modelData = mpEggModel->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* matNode_p = modelData->getMaterialNodePointer(i);
            matNode_p->getMatColor(0)->r = mColor;
            matNode_p->getMatColor(0)->g = mColor;
            matNode_p->getMatColor(0)->b = mColor;
            matNode_p->getTevKColor(3)->r = mColor;
            matNode_p->getTevKColor(3)->g = mColor;
            matNode_p->getTevKColor(3)->b = mColor;
        }

        fopAcM_setEffectMtx(this, mpEggModel->getModelData());
        mDoExt_modelUpdateDL(mpEggModel);
        dComIfGd_setSimpleShadow(&current.pos, mObjAcch.GetGroundH(), hREG_F(11) + 30.0f, mObjAcch.m_gnd, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static int daE_GM_Draw(daE_GM_c* i_this) {
    return i_this->draw();
}

void daE_GM_c::damage_check() {
    mStts.Move();

    if (field_0xa72 == 0) {
        if (mSph.ChkTgHit()) {
            mAtInfo.mpCollider = mSph.GetTgHitObj();

            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_NORMAL_SWORD) || mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB) || mAtInfo.mpCollider->ChkAtType(AT_TYPE_IRON_BALL)) {
                field_0xa74 = 1;
                health = 1;
            } else if (field_0xa74 > 1) {
                health = 100;
            } else {
                health = 1;
            }

            cc_at_check(this, &mAtInfo);

            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                field_0xa72 = 20;
            } else {
                field_0xa72 = 10;
            }

            if (mAtInfo.mHitStatus != 0) {
                field_0xa72 = 10;
            }

            dScnPly_c::setPauseTimer(0);

            if (mType == TYPE_GOMA) {
                if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_40) || mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                    return;
                }

                if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                    setAction(&daE_GM_c::hook);
                } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT) || mAtInfo.mpCollider->ChkAtType(AT_TYPE_SPINNER)) {
                    setAction(&daE_GM_c::core_piku);
                } else if (mSph.GetTgHitObj()->ChkAtType(AT_TYPE_CSTATUE_BOSS_SWING)) {
                    setAction(&daE_GM_c::core_pechanko_damage);
                } else {
                    setAction(&daE_GM_c::core_damage);
                }
            } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK) || mAtInfo.mpCollider->ChkAtType(AT_TYPE_40) ||
                       mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                field_0xa1c.y = cLib_targetAngleY(&daPy_getPlayerActorClass()->current.pos, &current.pos);
                speedF = cM_rndFX(5.0f) + 15.0f;
                setAction(&daE_GM_c::rebound);
            } else if (mSph.GetTgHitObj()->ChkAtType(AT_TYPE_CSTATUE_BOSS_SWING)) {
                setAction(&daE_GM_c::pechanko_damage);
            } else if (field_0xa69 == 0) {
                setAction(&daE_GM_c::egg_damage);
            } else if (field_0xa69 == 1) {
                if (mSph.GetTgHitObj()->ChkAtType(AT_TYPE_2000000)) {
                    if (mAtInfo.mpActor != NULL) {
                        field_0xa1c.y = cLib_targetAngleY(&mAtInfo.mpActor->current.pos, &current.pos);
                    } else {
                        field_0xa1c.y = mAtInfo.mHitDirection.y;
                    }
                } else {
                    field_0xa1c.y = cLib_targetAngleY(&daPy_getPlayerActorClass()->current.pos, &current.pos);
                }

                setAction(&daE_GM_c::damage);
            }
        } else if (checkAction(&daE_GM_c::walk) && mSph.ChkAtHit() && l_hitActorID == fpcM_ERROR_PROCESS_ID_e) {
            l_hitActorID = fopAcM_GetID(this);
            lbl_195_bss_AC = 10;
            field_0xa72 = 10;
        }
    }
}

int daE_GM_c::execute() {
    if (dMsgObject_isTalkNowCheck()) {
        return 1;
    }

    if (field_0xa6b != 0) {
        field_0xa6b--;
    }

    if (field_0xa6c != 0) {
        field_0xa6c--;
    }

    if (field_0xa6d != 0) {
        field_0xa6d--;
    }

    if (field_0xa72 != 0) {
        field_0xa72--;
    }

    if (l_hitActorID == fopAcM_GetID(this) && lbl_195_bss_AC != 0) {
        lbl_195_bss_AC--;
        if (lbl_195_bss_AC == 0) {
            l_hitActorID = fpcM_ERROR_PROCESS_ID_e;
        }
    }

    damage_check();
    action();

    if (l_coreActorID == fopAcM_GetID(this)) {
        l_corePos = current.pos;
        l_coreAngle = current.angle;
        l_coreSpeedF = speedF;
    }

    shape_angle = current.angle;

    if (field_0xa69 == 0) {
        scale = field_0xa10;
    } else if (field_0xa69 == 2) {
        scale = field_0xa10;
        mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    } else {
        scale.set(field_0xa10.x * l_HIO.model_size, field_0xa10.y * l_HIO.model_size, field_0xa10.z * l_HIO.model_size);
        mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    mtx_set();
    cc_set();

    return 1;
}

static int daE_GM_Execute(daE_GM_c* i_this) {
    return i_this->execute();
}

static int daE_GM_IsDelete(daE_GM_c* i_this) {
    return 1;
}

int daE_GM_c::_delete() {
    fopAcM_GetID(this);

    if (mType == TYPE_GOMA) {
        dComIfG_resDelete(&mPhase, "E_mg");
    } else {
        dComIfG_resDelete(&mPhase, "E_gm");
    }

    lbl_195_bss_AD--;

    if (lbl_195_bss_AE != 0) {
        lbl_195_bss_AE--;
    }

    if (l_hitActorID == fopAcM_GetID(this)) {
        l_hitActorID = fpcM_ERROR_PROCESS_ID_e;
        lbl_195_bss_AC = 0;
    }

    if (l_coreActorID == fopAcM_GetID(this)) {
        l_coreActorID = fpcM_ERROR_PROCESS_ID_e;
    }

    if (l_initHIO != 0) {
        l_initHIO = 0;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (heap != NULL) {
        mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int daE_GM_Delete(daE_GM_c* i_this) {
    return i_this->_delete();
}

int daE_GM_c::CreateHeap() {
    J3DModelData* modelData;

    if (mType == TYPE_GOMA) {
        modelData = (J3DModelData*)dComIfG_getObjectRes("E_mg", BMDV_MD);

        JUT_ASSERT(2683, modelData != NULL);

        mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_mg", 9),
                                           0, 1.0f, 0, -1, &mSound, 0, 0x11000084);
    } else {
        if (mType != TYPE_NORMAL) {
            modelData = (J3DModelData*)dComIfG_getObjectRes("E_gm", BMDE_GOMA_EGG);

            JUT_ASSERT(2701, modelData != NULL);

            mpEggModel = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
            if (mpEggModel == NULL) {
                return 0;
            }
        }

        modelData = (J3DModelData*)dComIfG_getObjectRes("E_gm", BMDV_GB);

        JUT_ASSERT(2714, modelData != NULL);

        mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_gm", BCK_GB_APPEAR),
                                           0, 1.0f, 0, -1, &mSound, 0, 0x11000084);
    }

    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    return static_cast<daE_GM_c*>(a_this)->CreateHeap();
}

static u8 const l_damage_count[4] = {1, 1, 1, 3};

static u16 const l_heapsize[4] = {
    0x3120, 0x3120, 0x21E0, 0x2DE0,
};

cPhs__Step daE_GM_c::create() {
    fopAcM_ct(this, daE_GM_c);

    mType = fopAcM_GetParam(this);
    if (mType == 0xFF) {
        mType = TYPE_0;
    }

    mRandIndex = lbl_195_bss_AD;
    lbl_195_bss_AD++;

    if (mType == TYPE_NORMAL) {
        mSwBit = (fopAcM_GetParam(this) & 0xFF00) >> 8;
        if (mSwBit != 0xFF) {
            if (dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this))) {
                OS_REPORT("E_gm やられ後なので再セットしません\n");
                return cPhs_ERROR_e;
            }
        }

        u8 uVar1 = (fopAcM_GetParam(this) & 0xFF0000) >> 16;
        if (uVar1 == 0xFF) {
            uVar1 = 10;
        }

        field_0xa54 = uVar1 * 100.0f;
    }

    cPhs__Step phase;
    if (mType == TYPE_GOMA) {
        phase = (cPhs__Step)dComIfG_resLoad(&mPhase, "E_mg");
    } else {
        phase = (cPhs__Step)dComIfG_resLoad(&mPhase, "E_gm");
    }

    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("e_gm PARAM %x\n", fopAcM_GetParam(this));
        OS_REPORT("e_gm or e_mg//////////////E_GM SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(this, useHeapInit, l_heapsize[mType])) {
            OS_REPORT("//////////////E_GM SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_GM SET 2 !!\n");

        if (l_initHIO == 0) {
            l_initHIO = 1;
            field_0xa77 = 1;
            l_HIO.id = -1;
        }

        field_0xa10.setall(1.0f);
        mSph.Set(cc_sph_src);
        mSph.SetStts(&mStts);
        mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
        mObjAcch.OnLineCheck();
        mAcchCir.SetWall(scale.y * 30.0f, scale.x * 30.0f);

        if (mType == TYPE_GOMA) {
            mStts.Init(0xFF, 0, this);
            mSound.setEnemyName("E_mg");
        } else {
            mStts.Init(0x32, 0, this);
            mSound.setEnemyName("E_gm");
        }

        mAtInfo.mpSound = &mSound;
        health = 1;
        field_0x560 = 1;
        field_0xa74 = l_damage_count[mType];
        mColor = 255.0f;
        field_0xa76 = false;
        fopAcM_OnStatus(this, 0);
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        if (mType == TYPE_NORMAL) {
            mSound.init(&current.pos, NULL, 2, 1);
            fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x10000);
            fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x4000);
            setAction(&daE_GM_c::normal_wait);
        } else if (mType == TYPE_GOMA) {
            mSound.init(&current.pos, &eyePos, 3, 1);

            if (l_coreActorID == fpcM_ERROR_PROCESS_ID_e) {
                l_coreActorID = fopAcM_GetID(this);
            }

            field_0xa68 = 0;
            attention_info.flags = 0;
            health = 100;
            field_0x560 = 100;
            setAction(&daE_GM_c::core_wait);
        } else {
            mSound.init(&current.pos, NULL, 2, 1);
            fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x10000);
            field_0xa50 = hREG_F(7) + 0.1f;
            field_0xa66 = cM_rndFX(100.0f) + 1000.0f;
            field_0xa64 = field_0xa66;
            setAction(&daE_GM_c::egg_wait);
        }

        field_0xa75 = 1;
        field_0x564 = 44;
        daE_GM_Execute(this);
    }

    return phase;
}

static int daE_GM_Create(daE_GM_c* i_this) {
    return i_this->create();
}

AUDIO_INSTANCES;

static actor_method_class l_daE_GM_Method = {
    (process_method_func)daE_GM_Create,
    (process_method_func)daE_GM_Delete,
    (process_method_func)daE_GM_Execute,
    (process_method_func)daE_GM_IsDelete,
    (process_method_func)daE_GM_Draw,
};

extern actor_process_profile_definition g_profile_E_GM = {
  fpcLy_CURRENT_e,       // mLayerID
  7,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_E_GM,             // mProcName
  &g_fpcLf_Method.base, // sub_method
  sizeof(daE_GM_c),      // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  119,                   // mPriority
  &l_daE_GM_Method,      // sub_method
  0x00044100,            // mStatus
  fopAc_ENEMY_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};
