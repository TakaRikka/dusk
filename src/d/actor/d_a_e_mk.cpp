/**
 * @file d_a_e_mk.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_mk.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_obj_pillar.h"
#include "d/actor/d_a_e_db.h"
#include "d/actor/d_a_e_db_leaf.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"
#include "d/actor/d_a_npc_ks.h"
#include "d/actor/d_a_obj_brg.h"
#include "d/actor/d_a_e_mk_bo.h"
#include "d/d_camera.h"
#include "f_op/f_op_camera_mng.h"
#include "Z2AudioLib/Z2Instances.h"

class daE_MK_HIO_c : public JORReflexible {
public:
    daE_MK_HIO_c();
    virtual ~daE_MK_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mSize;
    /* 0x0C */ f32 mBoomerangRatio;
    /* 0x10 */ f32 mPlayerThrowDistMax;
    /* 0x14 */ u8 mHaltAction;
    /* 0x18 */ Vec mCrownPosAdjust;
};

daE_MK_HIO_c::daE_MK_HIO_c() {
    field_0x4 = -1;
    mSize = 1.3f;
    mBoomerangRatio = 1.0f;
    mPlayerThrowDistMax = 1800.0f;
    mHaltAction = 0;
    mCrownPosAdjust.x = 0.0f;
    mCrownPosAdjust.y = 0.0f;
    mCrownPosAdjust.z = 0.0f;
}

static void anm_init(e_mk_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_mk", i_index),
                                i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->mAnm = i_index;
}

static void ok_anm_init(e_mk_class* i_this, int i_index, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->mpCrownModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_mk", i_index),
                                i_mode, i_morf, i_speed, 0.0f, -1.0f, NULL);
}

static int daE_MK_Draw(e_mk_class* i_this) {
    J3DModel* model_p = i_this->mpModelMorf->getModel();
    g_env_light.settingTevStruct(0, &i_this->enemy.current.pos, &i_this->enemy.tevStr);
    g_env_light.setLightTevColorType_MAJI(model_p, &i_this->enemy.tevStr);
    J3DModelData* i_modelData = model_p->getModelData();
    i_this->mBtp->entry(i_modelData);
    i_this->mpModelMorf->entryDL();

    cXyz sp38;
    if (i_this->field_0x6f8 != 0) {
        sp38.set(i_this->enemy.current.pos.x, i_this->enemy.current.pos.y + 50.0f, i_this->enemy.current.pos.z);
        i_this->mShadowKey1 = dComIfGd_setShadow(i_this->mShadowKey1, 1, model_p, &sp38, 
                                                 1200.0f, 0.0f, i_this->enemy.current.pos.y, 
                                                 i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd, 
                                                 &i_this->enemy.tevStr, 0, 1.0f, 
                                                 dDlst_shadowControl_c::getSimpleTex());
    }

    if (i_this->field_0x60c != 0) {
        model_p = i_this->mpCrownModelMorf->getModel();
        g_env_light.setLightTevColorType_MAJI(model_p, &i_this->enemy.tevStr);
        i_this->mpCrownModelMorf->entryDL();
        if (i_this->field_0x5e0 != 0) {
            sp38.set(i_this->field_0x5e4.x, i_this->enemy.current.pos.y + 50.0f, i_this->field_0x5e4.z);
            i_this->mShadowKey2 = dComIfGd_setShadow(i_this->mShadowKey2, 1, i_this->mpCrownModelMorf->getModel(), 
                                                     &sp38, 500.0f, 0.0f, i_this->enemy.current.pos.y, 
                                                     i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd, 
                                                     &i_this->enemy.tevStr, 0, 1.0f, 
                                                     dDlst_shadowControl_c::getSimpleTex());
        }
    }

    if (i_this->field_0x60d != 0) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpBoomerangModel, &i_this->enemy.tevStr);
        mDoExt_modelUpdateDL(i_this->mpBoomerangModel);
    }

    return 1;
}

static void* s_obj_delete(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor)) {
        if (fopAcM_GetName(i_actor) == PROC_E_DB) {
            static_cast<e_db_class*>(i_actor)->enemy.health = 1000;
        } else if (fopAcM_GetName(i_actor) == PROC_E_DB_LEAF) {
            fopAcM_delete(static_cast<e_db_leaf_class*>(i_actor));
        }
    }

    return NULL;
}

static cXyz demo_jump_pos_1(1050.0f, 500.0f, -450.0f);

static cXyz demo_jump_pos_2(1262.0f, 1030.0f, -1665.0f);

static cXyz demo_jump_pos_3(2400.0f, 1115.0f, -1850.0f);

static cXyz STAGE_CENTER_POS;

/* 8071CB08 0002+00 data_8071CB08 STAGE_ANGLE_Y */
static s16 STAGE_ANGLE_Y;

/* 8071CB0A 0001+00 .bss */
u8 lbl_210_bss_AA;

static daE_MK_HIO_c l_HIO;

static void* target_info[20];

static int target_info_count;

static void* s_h_sub(void* i_actor, void* i_data) {
    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_Pillar) && target_info_count < 20) {
        target_info[target_info_count] = (daPillar_c*)i_actor;
        target_info_count++;
    }

    return NULL;
}

static daPillar_c* search_hasira(e_mk_class* i_this) {
    target_info_count = 0;
    for (int i = 0; i < 20; i++) {
        target_info[i] = NULL;
    }

    fpcM_Search(s_h_sub, i_this);

    if (i_this->field_0x71d == 0) {
        i_this->field_0x71d++;
        return (daPillar_c*)target_info[5];
    }

    f32 fVar1 = 1500.0f;
    if (target_info_count != 0) {
        cXyz sp44;
        daPillar_c* pPillar;
        daPillar_c* rv = NULL;
        int iVar1 = 0;
        int i = 0;
        while (i < target_info_count) {
            pPillar = (daPillar_c*)target_info[i];
            sp44 = pPillar->current.pos - i_this->enemy.current.pos;
            f32 fVar2 = sp44.abs();
            if (fVar2 > 700.0f && fVar2 < fVar1) {
                s16 sVar1 = i_this->enemy.current.angle.y - cM_atan2s(sp44.x, sp44.z);
                if (sVar1 < 0x7000 && sVar1 > -0x7000) {
                    if (iVar1 == 0 || cM_rndF(1.0f) < 0.3f) {
                        rv = pPillar;
                    }
                    iVar1++;
                }
            }

            i++;
            if (i == target_info_count) {
                i = 0;
                fVar1 += 100.0f;
                if (fVar1 > 2000.0f) {
                    return NULL;
                }
                if (rv != NULL) {
                    return rv;
                }
            }
        }
    } else {
        return NULL;
    }

    return NULL;
}

static void* s_d_sub(void* i_actor, void* i_data) {
    if (((fopAc_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_DB) && static_cast<e_db_class*>(i_actor)->action == 10)
          && (static_cast<e_db_class*>(i_actor)->mode >= 1 && target_info_count < 20)) {
        target_info[target_info_count] = i_actor;
        target_info_count++;
    }

    return NULL;
}

static e_db_class* search_db(e_mk_class* i_this) {
    target_info_count = 0;
    for (int i = 0; i < 20; i++) {
        target_info[i] = NULL;
    }

    fpcM_Search(s_d_sub, i_this);

    f32 fVar1 = 2000.0f;
    if (target_info_count != 0) {
        cXyz sp44;
        e_db_class* pDekuBaba;
        int i = 0;
        while (i < target_info_count) {
            pDekuBaba = (e_db_class*)target_info[i];
            sp44 = pDekuBaba->enemy.current.pos - i_this->enemy.current.pos;

            f32 fVar2 = sp44.abs();
            if ((cM_rndF(1.0f) < 0.5f && fVar2 > 500.0f) && fVar2 < fVar1) {
                return pDekuBaba;
            }
            
            i++;
            if (i == target_info_count) {
                i = 0;
                fVar1 += 50.0f;
                if (fVar1 > 5000.0f) {
                    return NULL;
                }
            }
        }
    } else {
        return NULL;
    }

    return NULL;
}

static void e_mk_move(e_mk_class* i_this) {
    cXyz sp28;
    int frame = i_this->mpModelMorf->getFrame();
    switch(i_this->mMode) {
        case 0:
            i_this->field_0x6fa = cM_rndF(3.0f) + 2.0f;
            i_this->mMode++;
            // fallthrough
        case 1:
            i_this->field_0x6dc.set(0.0f, 0.0f, 0.0f);
            i_this->enemy.speed.set(0.0f, 0.0f, 0.0f);
            if (i_this->field_0xc30 == 2) {
                i_this->mPillar = NULL;
                target_info_count = 0;
                fpcM_Search(s_h_sub, i_this);
                if (i_this->field_0x71e == 0) {
                    sp28 = demo_jump_pos_1;
                    i_this->mPillar = (daPillar_c*)target_info[8];
                } else if (i_this->field_0x71e == 1) {
                    sp28 = demo_jump_pos_2;
                    i_this->mDemoMode++;
                    i_this->mPillar = (daPillar_c*)target_info[6];
                } else if (i_this->field_0x71e == 2) {
                    sp28 = demo_jump_pos_3;
                } else {
                    i_this->mAction = e_mk_class::ACT_E_DEMO;
                    i_this->mMode = 100;
                    i_this->enemy.gravity = 0.0f;
                    i_this->enemy.scale.x = 1.0f;
                    return;
                }

                i_this->field_0x71e++;
                i_this->field_0x5b8 = STAGE_CENTER_POS + sp28;
                sp28 = i_this->field_0x5b8 - i_this->enemy.current.pos;
                i_this->enemy.current.angle.y = cM_atan2s(sp28.x, sp28.z);
                i_this->enemy.current.angle.x = -cM_atan2s(sp28.y, JMAFastSqrt(sp28.x * sp28.x + sp28.z * sp28.z));
                anm_init(i_this, 29, 5.0f, 0, 1.0f);
                i_this->mMode = 3;
            } else {
                i_this->mPillar = (daPillar_c*)i_this->mHasira;
                i_this->mHasira = search_hasira(i_this);
                #ifdef DEBUG
                if (i_this->mHasira == NULL) {
                    OSReport("E_mk HASIRA NON \n");
                }
                #endif
                if (i_this->mHasira  != NULL) {
                    i_this->mMode++;
                // fallthrough
            case 2:
                    i_this->field_0x5b8 = i_this->mHasira->current.pos;
                    i_this->field_0x5b8.y += 800.0f;
                    if (fopAcM_gc_c::gndCheck(&i_this->field_0x5b8)) {
                        i_this->field_0x5b8.y = fopAcM_gc_c::getGroundY();
                    }

                    sp28 = i_this->field_0x5b8 - i_this->enemy.current.pos;
                    i_this->enemy.current.angle.y = cM_atan2s(sp28.x, sp28.z);
                    i_this->enemy.current.angle.x = -cM_atan2s(sp28.y, JMAFastSqrt(sp28.x * sp28.x + sp28.z * sp28.z));
                    anm_init(i_this, 29, 5.0f, 0, 1.0f);
                    i_this->mMode++;
                }
            }
            break;

    case 3:
        if (frame == 9) {
            cMtx_YrotS(*calc_mtx, i_this->enemy.current.angle.y);
            cMtx_XrotM(*calc_mtx, i_this->enemy.current.angle.x);
            sp28.x = 0.0f;
            sp28.y = 0.0f;
            sp28.z = 70.0f;
            MtxPosition(&sp28, &i_this->enemy.speed);
            i_this->field_0x6be = 1;
            i_this->field_0x6c0 = i_this->enemy.current.pos;
            i_this->field_0x6cc = i_this->field_0x5b8;

            i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_JUMP, -1);
            i_this->mSound.startCreatureSound(Z2SE_EN_MK_JUMP_WIND, 0, -1);
            if (i_this->field_0x718 != 0) {
                i_this->field_0x71b = 2;
            }

            if (i_this->mPillar != NULL) {
                sp28 = i_this->mPillar->current.pos;
                sp28.y += 500.0f;
                sp28 -= i_this->enemy.current.pos;
                if (sp28.abs() < 200.0f) {
                    i_this->mPillar->setShake(2);
                }
            }
        }

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, 28, 5.0f, 0, 1.0f);
            i_this->mMode++;
        }
        break;

    case 4:
        sp28 = i_this->field_0x5b8 - i_this->enemy.current.pos;
        if (sp28.abs() < 300.0f) {
            anm_init(i_this, 26, 2.0f, 0, 1.0f);
            i_this->mMode++;
            i_this->field_0x6dc = i_this->enemy.speed;
            i_this->enemy.speed.set(0.0f, 0.0f, 0.0f);
        }
        break;

    case 5:
        if (i_this->mpModelMorf->isStop() != 0) {
            i_this->field_0x6be = 0;
            if (i_this->field_0x718 != 0) {
                i_this->mMode = 10;
                i_this->field_0x718 = 0;
                anm_init(i_this, 30, 3.0f, 0, 1.0f);
            } else {
                i_this->field_0x6fa--;
                if (i_this->field_0x6fa == 0) {
                    if (cM_rndF(1.0f) < 0.75f) {
                        i_this->mAction = e_mk_class::ACT_SHOOT;
                        if ((cM_rndF(1.0f) < 0.5f && i_this->field_0x71c >= 3) || i_this->mDistToPlayer > l_HIO.mPlayerThrowDistMax) {
                            i_this->mMode = 1;
                        } else {
                            i_this->mMode = 0;
                            i_this->field_0x71c++;
                        }
                        i_this->field_0x71a = 0;
                    } else {
                        i_this->mAction = e_mk_class::ACT_WAIT;
                        i_this->mMode = 0;
                    }
                } else {
                    i_this->mMode = 1;
                }
            }
        }
        break;

    case 10:
        if (frame == 5) {
            i_this->field_0x707 = 6;
            i_this->field_0x60d = 1;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mMode = 1;
        }
    }


    cLib_addCalc2(&i_this->enemy.current.pos.x, i_this->field_0x5b8.x, 1.0f, fabsf(i_this->field_0x6dc.x));
    cLib_addCalc2(&i_this->enemy.current.pos.z, i_this->field_0x5b8.z, 1.0f, fabsf(i_this->field_0x6dc.z));
    cLib_addCalc2(&i_this->enemy.current.pos.y, i_this->field_0x5b8.y, 1.0f, fabsf(i_this->field_0x6dc.y));
}

static void e_mk_wait(e_mk_class* i_this) {
    switch (i_this->mMode) {
        case 0:
            i_this->field_0x6fc[0] = cM_rndF(40.0f) + 40.0f;
            if (cM_rndF(1.0f) < 0.5f) {
                anm_init(i_this, 32, 10.0f, 2, 1.0f);
                i_this->mMode = 1;
            } else {
                anm_init(i_this, 33, 10.0f, 0, 1.0f);
                i_this->mMode = 2;
            }
            break;

        case 1:
            if (i_this->field_0x6fc[0] == 0) {
                i_this->mAction = e_mk_class::ACT_MOVE;
                i_this->mMode = 0;
            }
            break;

        case 2:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = e_mk_class::ACT_MOVE;
                i_this->mMode = 0;
            }
    }

    cLib_addCalcAngleS2(&i_this->enemy.current.angle.y, i_this->mAngleToPlayer, 2, 0x1000);
    if (i_this->mDistToPlayer < 800.0f) {
        i_this->mAction = e_mk_class::ACT_MOVE;
        i_this->mMode = 0;
    }
}

static void e_mk_shoot(e_mk_class* i_this) {
    cXyz sp48;
    int frame = i_this->mpModelMorf->getFrame();
    fopAc_ac_c* actor = fopAcM_SearchByID(i_this->field_0x708);
    daPillar_c* pHasira = i_this->mHasira;
    s8 bVar5 = 0;
    switch(i_this->mMode) {
        case 0:
            i_this->field_0x70c = NULL;
            anm_init(i_this, 38, 5.0f, 0, 1.0f);
            i_this->mMode = 2;
            break;

        case 1:
            i_this->field_0x70c = search_db(i_this);
            if (i_this->field_0x70c != NULL) {
                anm_init(i_this, 39, 5.0f, 0, 1.0f);
                i_this->mMode = 10;
            } else {
                i_this->mMode = 0;
            }
            break;

    case 2:
        cLib_addCalcAngleS2(&i_this->enemy.current.angle.y, i_this->mAngleToPlayer, 2, 0x1000);

        if (i_this->mpModelMorf->checkFrame(15.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_THROW_BOOM, -1);
        }

        if (frame == 17) {
            i_this->field_0x707 = 7;
        }
        
        if (frame == 27) {
            i_this->field_0x707 = 1;
        }

        if (frame == 27) {
            i_this->field_0x60d = 0;
            i_this->mMode = 3;
        }
        break;

    case 3:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mMode = 20;
            if (cM_rndF(1.0f) < 0.333f) {
                anm_init(i_this, 31, 10.0f, 2, 1.0f);
            }
            else {
                anm_init(i_this, 40, 10.0f, 2, 1.0f);
            }
        }
        bVar5 = 1;
        break;

    case 10:
        sp48 = i_this->field_0x70c->enemy.current.pos - i_this->enemy.current.pos;
        cLib_addCalcAngleS2(&i_this->enemy.current.angle.y, cM_atan2s(sp48.x, sp48.z), 2, 0x1000);
        if (i_this->mpModelMorf->checkFrame(15.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_THROW_BOOM, -1);
        }

        if (frame == 17) {
            i_this->field_0x707 = 7;
        }

        if (frame == 27) {
            i_this->field_0x707 = 1;
        }

        if (frame == 27) {
            i_this->field_0x60d = 0;
            i_this->mMode = 3;
        }
        break;

    case 15:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = e_mk_class::ACT_MOVE;
            i_this->mMode = 0;
        }
        bVar5 = 3;
        break;

    case 20:
        bVar5 = 2;
    }

    if (bVar5 != 0) {
        if ((bVar5 == 2) && (actor != NULL)) {
            cXyz sp54 = actor->current.pos - i_this->enemy.current.pos;
            cLib_addCalcAngleS2(&i_this->enemy.current.angle.y, cM_atan2s(sp54.x,sp54.z), 2, 0x800);
        }
        if (bVar5 <= 2) {
            if (i_this->field_0x707 == 5) {
                i_this->field_0x707 = 0;
                i_this->field_0x71a = 47;
            }

            if (i_this->field_0x71a == 35) {
                anm_init(i_this, 25, 5.0f, 2, 1.0f);
                i_this->mMode = 20;
            }

            if (i_this->field_0x71a == 1) {
                anm_init(i_this, 40, 10.0f, 2, 1.0f);
            }

            if (i_this->field_0x707 == 2) {
                anm_init(i_this, 9, 3.0f, 0, 0.0f);
                i_this->field_0x707 = 0;
            }

            if (i_this->field_0x707 == 3) {
                i_this->field_0x707 = 0;
                i_this->field_0x60d = 1;
                anm_init(i_this, 9, 1.0f, 0, 1.0f);
                i_this->mMode = 15;

                i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_CATCH_BOOM, -1);
                i_this->mSound.startCreatureSound(Z2SE_EN_MK_CATCH_BOOM, 0, -1);

#if VERSION == VERSION_GCN_JPN
                return;
#endif
            }
        }

        if ((pHasira != NULL) && pHasira->checkRollAttack()) {
            i_this->mAction = e_mk_class::ACT_YORO;
            i_this->mMode = 0;
            if (i_this->field_0x60d != 0) {
                i_this->field_0x6fc[0] = 40;
            }
            else {
                i_this->field_0x6fc[0] = 250;
            }
        }
    }
}

static s8 lbl_210_bss_130;

static void e_mk_yoro(e_mk_class* i_this) {
    switch (i_this->mMode) {
        case 0:
            i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_MK_V_LOOSE_BALANCE, -1);
            anm_init(i_this, 8, 5.0f, 0, 1.0f);
            i_this->mMode = 1;
            break;

        case 1:
            i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_MK_V_LOOSE_BALANCE, -1);
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 7, 5.0f, 2, 1.0f);
                i_this->mMode = 2;
            }
            break;

        case 2:
            i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_MK_V_LOOSE_BALANCE, -1);
            if (i_this->field_0x6fc[0] == 0) {
                i_this->mAction = e_mk_class::ACT_WAIT;
                i_this->mMode = 0;
                i_this->field_0x707 = 6;
                i_this->field_0x60d = 1;
            }

    }

    if (i_this->field_0x707 == 4) {
        i_this->field_0x707 = 0;
        i_this->mAction = e_mk_class::ACT_DROP;
        i_this->mMode = 0;

        i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_HIT_BOOM, -1);
        i_this->mSound.startCollisionSE(Z2SE_HIT_WOOD_WEAPON, 31);

        i_this->field_0x704 = 5;
        i_this->field_0x706 = 5;
        if (lbl_210_bss_130 == 0) {
            lbl_210_bss_130 = 1;
            i_this->field_0xc30 = 4;
            i_this->mDemoMode = 1;
        }
    }
}

static void e_mk_drop(e_mk_class* i_this) {
    cXyz sp24;
    int frame = i_this->mpModelMorf->getFrame();
    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 23, 3.0f, 0, 1.0f);
            i_this->mMode = 1;
            cMtx_YrotS(*calc_mtx, i_this->enemy.shape_angle.y);
            sp24.x = 0.0;
            sp24.y = 30.0;
            sp24.z = -10.0;
            MtxPosition(&sp24, &i_this->enemy.speed);
            i_this->field_0x704 = 50;
            break;

        case 1:
            if (frame >= 14) {
                i_this->mpModelMorf->setPlaySpeed(0.0f);
                i_this->mMode = 2;
            }
            break;

        case 2:
            i_this->field_0x704 = 5;
            if (i_this->mAcch.ChkGroundHit()) {
                i_this->mpModelMorf->setPlaySpeed(1.0f);
                i_this->mMode = 3;
                dComIfGp_getVibration().StartShock(5, 15, cXyz(0.0f, 1.0f, 0.0f));
                i_this->mSound.startCreatureSound(Z2SE_EN_MK_FALL_GROUND, 0, -1);
                i_this->field_0x71b = 1;
                i_this->field_0x719 = 0;
                Z2GetAudioMgr()->changeSubBgmStatus(1);
                dComIfGs_onOneZoneSwitch(5, -1);
            }
            break;
            
        case 3:
            if (frame == 22 || frame == 32) {
                i_this->field_0x71b = 1;
            }

            if (frame >= 45) {
                i_this->enemy.speed.set(0.0f, 0.0f, 0.0f);
            }

            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 10, 5.0f, 2, 1.0f);
                i_this->mMode = 4;
                i_this->field_0x6fc[0] = 150;
            }
            break;
        
        case 4:
            if (i_this->field_0x6fc[0] == 0) {
                anm_init(i_this, 36, 5.0f, 0, 1.0f);
                i_this->mMode = 5;
                i_this->field_0x719 = 4;
                i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_AWAKE, -1);
            }
            break;

        case 5:
            if (i_this->mAnm == 35 && ((frame == 9 || (frame == 24)) || (frame == 35))) {
                i_this->field_0x71b = 2;
            }

            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = e_mk_class::ACT_MOVE;
                i_this->mMode = 2;
                i_this->field_0x6dc.set(0.0f, 0.0f, 0.0f);
                i_this->field_0x6fa = cM_rndF(3.0f) + 2.0f;
                i_this->field_0x718 = 1;
                if (i_this->enemy.health > 100) {
                    Z2GetAudioMgr()->changeSubBgmStatus(2);
                } else {
                    Z2GetAudioMgr()->changeSubBgmStatus(3);
                }
            }
    }
}

static void e_mk_damage(e_mk_class* i_this) {
    switch (i_this->mMode) {
        case 0:
            i_this->field_0x719++;
            i_this->enemy.speed.set(0.0f, 0.0f, 0.0f);
            if (i_this->field_0x719 >= 4) {
                anm_init(i_this, 35, 3.0f, 0, 1.0f);
                i_this->mAction = e_mk_class::ACT_DROP;
                i_this->mMode = 5;
                i_this->field_0x704 = 60;
            } else {
                anm_init(i_this, 12, 3.0f, 0, 1.0f);
                i_this->mMode = 1;
                i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_DAMAGE, -1);
            }
            break;

        case 1:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this,10, 5.0f, 2, 1.0f);
                i_this->mAction = e_mk_class::ACT_DROP;
                i_this->mMode = 4;
            }
            break;
    }
}

static void damage_check(e_mk_class* i_this) {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    if (i_this->field_0x704 == 0) {
        i_this->field_0x95c.Move();
        if (i_this->mAction >= 9) {
            if (i_this->field_0xad0.ChkTgHit()) {
                i_this->mAtInfo.mpCollider = i_this->field_0xad0.GetTgHitObj();
                cc_at_check(&i_this->enemy, &i_this->mAtInfo);
                if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK) != 0) {
                    i_this->field_0x704 = 20;
                } else {
                    i_this->field_0x704 = 10;
                }

                if (i_this->enemy.health <= 0) {
                    fpcM_Search(s_obj_delete, i_this);
                    i_this->mAction = 30;
                    i_this->mMode = 0;
                    i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_DEAD, -1);
                    i_this->field_0x704 = 20000;
                    Z2GetAudioMgr()->subBgmStop();
                    dScnPly_c::nextPauseTimer = 0;
                } else {
                    i_this->mAction = 10;
                    i_this->mMode = 0;
                    if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP && player->checkCutJumpCancelTurn()) {
                        i_this->field_0x704 = 3;
                    }
                }
            }
        }

        if (i_this->enemy.health <= 1) {
            i_this->enemy.health = 0;
            i_this->field_0xad0.SetTgHitMark((CcG_Tg_HitMark )3);
        }
    }
}

static void e_mk_s_demo(e_mk_class* i_this) {
    int frame = i_this->mpModelMorf->getFrame();
    i_this->field_0x704 = 5;
    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 40, 1.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->field_0x6fc[0] = 0;
            // fallthrough
        case 1:
            if (i_this->field_0x6fc[0] == 0) {
                i_this->mMode = 100;
                i_this->field_0xc30 = 1;
                i_this->mDemoMode = 1;
            }
            break;

        case 2:
            i_this->mSound.startCreatureSound(Z2SE_MK_DARK_BOOM_RESUME, 0, -1);
            anm_init(i_this, 20, 5.0f, 0, 1.0f);
            i_this->mMode = 3;
            break;

        case 3:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 21, 2.0f, 2, 1.0f);
                i_this->mMode = 100;
            }
            break;

        case 4:
            anm_init(i_this, 19, 1.0f, 0 ,1.0f);
            i_this->mMode = 5;
            i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_THROW_BOOM, -1);
            break;

        case 5:
            if (frame == 8) {
                i_this->field_0x707 = 1;
                Z2GetAudioMgr()->bgmStreamPlay();
            }

            if (frame == 8) {
                i_this->field_0x60d = 0;
            }
    
            if (frame == 14) {
                i_this->mMode = 6;
                i_this->mDemoMode = 7;
                anm_init(i_this, 40, 10.0f, 2, 1.0f);
            }
            break;

        case 6:
            if (i_this->field_0x707 == 2) {
                anm_init(i_this, 9, 3.0f, 0, 0.0f);
                i_this->field_0x707 = 0;
            }

            if (i_this->field_0x707 == 3) {
                i_this->field_0x707 = 0;
                i_this->field_0x60d = 1;
                anm_init(i_this, 9, 1.0f, 0, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_CATCH_BOOM, -1);
                i_this->mSound.startCreatureSound(Z2SE_EN_MK_CATCH_BOOM, 0, -1);
                i_this->mMode = 7;
            }
            break;

        case 7:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 15, 5.0f, 2, 1.0f);
                i_this->field_0x6fc[0] = 30;
                i_this->mMode = 8;
            }
            break;

        case 8:
            if (i_this->field_0x6fc[0] == 0) {
                anm_init(i_this, 33, 10.0f, 0, 1.0f);
                i_this->mMode = 9;
            }
            break;

        case 9:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mAction = e_mk_class::ACT_MOVE;
                i_this->field_0x6fa = cM_rndF(2.0f) + 7.0f;
                i_this->mMode = 1;
                i_this->mDemoMode = 11;
                i_this->mDemoCamTimer = 0;
            }
            break;

        case 100:
            return;
    }
}

static int e_mk_e_demo(e_mk_class* i_this) {
    f32 fVar1;
    cXyz sp38, sp44;
    int frame = i_this->mpModelMorf->getFrame();
    int rv = 1;
    s8 cVar6 = 0;

    switch (i_this->mMode) {
        case -100:
            anm_init(i_this, 10, 5.0f, 2, 1.0f);
            i_this->mMode++;
            break;

        case 0:
            anm_init(i_this, 18, 1.0f, 0, 1.0f);
            i_this->mMode = 1;
            i_this->field_0xc30 = 2;
            i_this->mDemoMode = 1;
            dComIfGs_onSwitch(108, fopAcM_GetRoomNo(&i_this->enemy));
            break;

        case 1:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 17, 1.0f, 2, 1.0f);
                i_this->mMode = 2;
                i_this->enemy.current.angle.y = STAGE_ANGLE_Y + -0x4000;
                cMtx_YrotS(*calc_mtx,i_this->enemy.current.angle.y);
                sp38.x = 0.0;
                sp38.y = 0.0;
                sp38.z = -1000.0;
                MtxPosition(&sp38, &sp44);
                i_this->enemy.current.pos = STAGE_CENTER_POS + sp44;
                i_this->enemy.old.pos = STAGE_CENTER_POS + sp44;
                i_this->mDemoMode = 4;
                i_this->mDemoCamTimer = 0;
                i_this->enemy.gravity = -10.0f;
            }
            break;

        case 2:
            if (i_this->mAcch.ChkGroundHit()) {
                i_this->enemy.speedF = 0.0f;
            }
    
            if (frame == 1 || frame == 18) {
                i_this->enemy.speed.y = 50.0f;
                i_this->enemy.speedF = 30.0f;
                i_this->mSound.startCreatureSound(Z2SE_EN_MK_JUMP_START, 0, -1);
            }
    
            sp38 = STAGE_CENTER_POS - i_this->enemy.current.pos;
            fVar1 = JMAFastSqrt(sp38.x * sp38.x + sp38.z * sp38.z);
            if (fVar1 < 200.0f) {
                rv = 2;
                if (fVar1 < 100.0f) {
                    i_this->mMode = 3;
                    anm_init(i_this, 16, 2.0f, 0, 1.0f);
                    i_this->enemy.speedF = 0.0f;
                    i_this->enemy.gravity = 0.0f;
                    i_this->enemy.speed.y = 0.0f;
                    i_this->field_0x6fc[0] = 40;
                    cVar6 = 60;
                    target_info_count = 0;
                    fpcM_Search(s_h_sub, i_this);
                    static_cast<daPillar_c*>(target_info[8])->setShake(1);
                }
            }
            break;

        case 3:
            rv = 2;
            i_this->field_0x5d4 = 0;
            if (i_this->field_0x6fc[0] < 12) {
              i_this->field_0x5d8 = i_this->field_0x6fc[0] << 1;
              if (i_this->field_0x5d8 >= 12.0f) {
                i_this->field_0x5d8 = i_this->field_0x5d8 - 12.0f;
              }
            }
            
            if (i_this->field_0x6fc[0] == 0) {
              i_this->enemy.gravity = -0.03f;
              if (i_this->enemy.speed.y < -1.0f) {
                  anm_init(i_this, 14, 10.0f, 0, 1.0f);
                  i_this->enemy.speedF = 0.0f;
                  i_this->enemy.gravity = -2.0f;
                  i_this->mMode = 4;
              }
            }
            break;

        case 4:
            if (i_this->mAcch.ChkGroundHit()) {
                i_this->enemy.speedF = 0.0f;
                i_this->field_0x71b = 2;
                cVar6 = 30;
                i_this->mMode = 100;
                i_this->field_0x5e0 = 1;
                ok_anm_init(i_this, 43, 1.0f, 2, 1.0f);
                cMtx_YrotS(*calc_mtx, i_this->enemy.current.angle.y);
                sp38.x = 0.0f;
                sp38.y = 0.0f;
                sp38.z = -50.0f;
                MtxPosition(&sp38, &sp44);
                i_this->field_0x5e4 += sp44;
                i_this->field_0x5f0 = i_this->enemy.shape_angle;
                i_this->field_0x5f0.x = 0x4000;
                i_this->field_0x5fc = 30.0f;
                i_this->field_0x600 = -20.0f;
                i_this->mDemoMode = 5;
                i_this->mDemoCamTimer = 0;
                i_this->field_0x60e = 1;
            }
            break;

        case 5:
            anm_init(i_this, 22, 1.0f, 0, 1.0f);
            i_this->field_0x5d4 = 60;
            i_this->mMode++;
            break;

        case 6:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 13, 5.0f, 0, 1.0f);
                i_this->mMode++;
                i_this->field_0x5d4 = 125;
                i_this->mDemoMode = 8;
                i_this->mDemoCamTimer = 0;
            }
            break;

        case 7:
            if (frame == 144) {
                i_this->field_0x718 = 0;
                i_this->field_0x6fa = 100;
                i_this->field_0x6dc.set(0.0f, 0.0f, 0.0f);
                i_this->enemy.speed.set(0.0f, 0.0f, 0.0f);
                i_this->field_0x5b8 = STAGE_CENTER_POS;
                i_this->field_0x5b8.y += 500.0f;
                sp38 = i_this->field_0x5b8 - i_this->enemy.current.pos;
                i_this->enemy.current.angle.y = cM_atan2s(sp38.x, sp38.z);
                i_this->enemy.current.angle.x = -cM_atan2s(sp38.y, JMAFastSqrt(sp38.x * sp38.x + sp38.z * sp38.z));
                cMtx_YrotS(*calc_mtx, i_this->enemy.current.angle.y);
                cMtx_XrotM(*calc_mtx, i_this->enemy.current.angle.x);
                sp38.x = 0.0;
                sp38.y = 0.0;
                sp38.z = 70.0;
                MtxPosition(&sp38, &i_this->enemy.speed);
                i_this->field_0x6be = 1;
                i_this->field_0x6c0 = i_this->enemy.current.pos;
                i_this->field_0x6cc = i_this->field_0x5b8;
                i_this->mSound.startCreatureSound(Z2SE_EN_MK_JUMP_WIND, 0, -1);
                i_this->mAction = e_mk_class::ACT_MOVE;
                i_this->mMode = 4;
                return 0;
            }
            break;

        case -99:
        case 100:
            break;
    }

    cMtx_YrotS(*calc_mtx, i_this->enemy.current.angle.y);
    sp38.x = 0.0f;
    sp38.y = 0.0f;
    sp38.z = i_this->enemy.speedF;
    MtxPosition(&sp38, &sp44);
    i_this->enemy.speed.x = sp44.x;
    i_this->enemy.speed.z = sp44.z;
    if (cVar6 != 0) {
      dComIfGp_getVibration().StartShock(5, 1, cXyz(0.0f, 1.0f, 0.0f));
      i_this->mSound.startCreatureSound(Z2SE_EN_FM_FOOTNOTE, 0, -1);
      i_this->field_0xc98 = cVar6;
    }
    
    return rv;
}

static void e_mk_r04_demo(e_mk_class* i_this) {
    int frame = i_this->mpModelMorf->getFrame();
    f32 fVar2 = 0.0f;
    i_this->field_0x704 = 5;
    switch(i_this->mMode) {
        case 0:
            anm_init(i_this, 40, 1.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->field_0xc30 = 3;
            i_this->mDemoMode = 1;
            break;

        case 2:
            anm_init(i_this, 34, 3.0f, 2, 1.0f);
            i_this->mMode = 3;
            break;

        case 3:
            fVar2 = 30.0f;
            if (i_this->enemy.current.pos.z > -600.0f) {
                i_this->mMode = 4;
                anm_init(i_this, 40, 5.0f, 2, 1.0f);
                i_this->field_0x6fc[0] = 0;
            }
            break;

        case 4:
            if (i_this->field_0x6fc[0] == 0) {
                i_this->mSound.startCreatureSound(Z2SE_MK_DARK_BOOM_RESUME, 0, -1);
                anm_init(i_this, 20, 5.0f, 0, 1.0f);
                i_this->mMode = 5;
            }
            break;

        case 5:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 21, 2.0f, 2, 1.0f);
                i_this->mMode = 6;
                i_this->field_0x6fc[0] = 95;
            }
            break;

        case 6:
            if (i_this->field_0x6fc[0] == 0) {
                anm_init(i_this, 19, 1.0f, 0, 1.0f);
                i_this->mMode = 7;
                i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_THROW_BOOM, -1);
                i_this->field_0x707 = 7;
            }
            break;

        case 7:
            if (frame == 8) {
                i_this->field_0x707 = 1;
            }

            if (frame == 8) {
                i_this->field_0x60d = 0;
            }

            if (frame == 14) {
                i_this->mMode = 8;
                anm_init(i_this, 40, 10.0f, 2, 1.0f);
            }
            break;

        case 8:
            if (i_this->field_0x707 == 2) {
                anm_init(i_this, 9, 3.0f, 0, 0.0f);
                i_this->field_0x707 = 0;
            }

            if (i_this->field_0x707 == 3) {
                i_this->field_0x707 = 0;
                i_this->field_0x60d = 1;
                anm_init(i_this, 9, 1.0f, 0, 1.0f);
                i_this->mMode = 9;

                i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_CATCH_BOOM, -1);
                i_this->mSound.startCreatureSound(Z2SE_EN_MK_CATCH_BOOM, 0, -1);
            }
            break;

        case 9:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 15, 5.0f, 2, 1.0f);
                i_this->field_0x6fc[0] = 30;
                i_this->mMode = 10;
            }
            break;

        case 10:
            if (i_this->field_0x6fc[0] == 0) {
                anm_init(i_this, 33, 10.0f, 0, 1.0f);
                i_this->mMode = 11;
            }
            break;

        case 11:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 34, 3.0f, 2, 1.0f);
                i_this->mMode = 12;
                i_this->field_0x6fc[0] = 10;
            }
            break;

        case 12:
            if (i_this->field_0x6fc[0] == 0) {
                fVar2 = 30.0f;
            }

            cLib_addCalcAngleS2(&i_this->enemy.current.angle.y, i_this->enemy.home.angle.y + (u16)-0x8000, 2, 0x1000);
            break;
    }

    cLib_addCalc2(&i_this->enemy.speedF, fVar2, 1.0f, 5.0f);
    i_this->enemy.gravity = -10.0f;
    cMtx_YrotS(*calc_mtx, i_this->enemy.current.angle.y);
    cXyz sp2c, sp38;
    sp2c.x = 0.0f;
    sp2c.y = 0.0f;
    sp2c.z = i_this->enemy.speedF;
    MtxPosition(&sp2c, &sp38);
    i_this->enemy.speed.x = sp38.x;
    i_this->enemy.speed.z = sp38.z;
    i_this->enemy.current.pos += i_this->enemy.speed;
    i_this->enemy.speed.y += i_this->enemy.gravity;
}

static void cam_3d_morf(e_mk_class* i_this, f32 param_2) {
    cLib_addCalc2(&i_this->mCamCenter.x, i_this->field_0xc5c.x, param_2, i_this->field_0xc74.x * i_this->field_0xc84);
    cLib_addCalc2(&i_this->mCamCenter.y, i_this->field_0xc5c.y, param_2, i_this->field_0xc74.y * i_this->field_0xc84);
    cLib_addCalc2(&i_this->mCamCenter.z, i_this->field_0xc5c.z, param_2, i_this->field_0xc74.z * i_this->field_0xc84);
    cLib_addCalc2(&i_this->mCamEye.x, i_this->field_0xc50.x, param_2, i_this->field_0xc68.x * i_this->field_0xc84);
    cLib_addCalc2(&i_this->mCamEye.y, i_this->field_0xc50.y, param_2, i_this->field_0xc68.y * i_this->field_0xc84);
    cLib_addCalc2(&i_this->mCamEye.z, i_this->field_0xc50.z, param_2, i_this->field_0xc68.z * i_this->field_0xc84);
}

static void demo_camera_start(e_mk_class* i_this) {
    fopAc_ac_c* a_this = &i_this->enemy;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* actor_sp1c; // sp1c
    fopAc_ac_c* actor_sp18; // sp18
    fopAc_ac_c* actor_sp14; // sp14
    actor_sp14 = fopAcM_SearchByID(i_this->field_0x708);
    camera_class* camera1 = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera2 = (camera_class*)dComIfGp_getCamera(0);
    cXyz sp34, sp40, sp4c, sp58;

    switch (i_this->mDemoMode) {
        case 0:
            break;
        case 1:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera1->mCamera.Stop();
            i_this->mDemoMode = 2;
            i_this->mDemoCamTimer = 0;
            i_this->mDemoCamFovy = 50.0f + NREG_F(10);
            camera1->mCamera.SetTrimSize(3);
            i_this->field_0xc90 = 2500.0f + NREG_F(11);

            daPy_getPlayerActorClass()->changeOriginalDemo();
            Z2GetAudioMgr()->bgmStreamPrepare(0x200000D);

            i_this->mCamEye = camera2->lookat.eye;
            i_this->mCamCenter = camera2->lookat.center;
            i_this->field_0xc50.set(-271.0f, 4559.0f, -7241.0f);
            i_this->field_0xc5c.set(-70.0f, 4378.0f, -6233.0f);

            i_this->field_0xc68.x = fabsf(i_this->field_0xc50.x - i_this->mCamEye.x);
            i_this->field_0xc68.y = fabsf(i_this->field_0xc50.y - i_this->mCamEye.y);
            i_this->field_0xc68.z = fabsf(i_this->field_0xc50.z - i_this->mCamEye.z);
            i_this->field_0xc74.x = fabsf(i_this->field_0xc5c.x - i_this->mCamCenter.x);
            i_this->field_0xc74.y = fabsf(i_this->field_0xc5c.y - i_this->mCamCenter.y);
            i_this->field_0xc74.z = fabsf(i_this->field_0xc5c.z - i_this->mCamCenter.z);

            dComIfGp_getEvent().startCheckSkipEdge(a_this);
            // fallthrough
        case 2:
            if (i_this->mDemoCamTimer == (s16)(8 + VREG_S(0))) {
                ((daPy_py_c*)daPy_getPlayerActorClass())->changeDemoMode(25, 0, 0, 0);
            }

            if (i_this->mDemoCamTimer >= (s16)(35 + VREG_S(1))) {
                cam_3d_morf(i_this, 0.1f);
                cLib_addCalc2(&i_this->field_0xc84, 0.03f + VREG_F(1), 1.0f, 0.0005f + VREG_F(2));
            }

            if (i_this->mDemoCamTimer >= (s16)(160 + VREG_S(2))) {
                i_this->mDemoCamTimer = 0;
                i_this->mDemoMode = 3;
            }
            break;
        case 3:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp34.x = 0.0f;
            sp34.y = 150.0f + NREG_F(6);
            sp34.z = 250.0f + NREG_F(7);
            MtxPosition(&sp34, &sp40);
            i_this->mCamEye = a_this->current.pos + sp40;
            i_this->mCamCenter = a_this->current.pos;
            i_this->mCamCenter.y += 150.0f + NREG_F(8);
            if (i_this->mDemoCamTimer >= 60) {
                i_this->mDemoCamTimer = 0;
                i_this->mDemoMode = 4;
                i_this->mMode = 2;
            }
            break;

            case 4:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp34.x = 0.0f;
            sp34.y = 150.0f + NREG_F(9);
            sp34.z = 700.0f + NREG_F(10);
            MtxPosition(&sp34, &sp40);
            i_this->mCamEye = a_this->current.pos + sp40;
            i_this->mCamCenter = a_this->current.pos;
            i_this->mCamCenter.y += 230.0f + NREG_F(11);

            if (i_this->mDemoCamTimer == 35) {
                daPy_getPlayerActorClass()->changeDemoMode(12, 0, 1, 0);
            }

            if (i_this->mDemoCamTimer >= (s16)(95 + XREG_S(0))) {
                i_this->mDemoCamTimer = 0;
                i_this->mDemoMode = 5;
                i_this->field_0xc9c = 1100.0f;
                i_this->field_0xc84 = 0.0f;
                i_this->field_0xc90 = 2500.0f + NREG_F(12);
                cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
                cMtx_XrotM(*calc_mtx, 1700 + NREG_S(2));
                sp34.x = 0.0f;
                sp34.y = 0.0f;
                sp34.z = i_this->field_0xc90;
                MtxPosition(&sp34, &sp4c);
                sp4c += a_this->current.pos;
                cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
                sp34.x = NREG_F(2) - 300.0f;
                sp34.y = 0.0f;
                sp34.z = 0.0f;
                MtxPosition(&sp34, &sp40);
                sp4c += sp40;
                sp34.x = 100.0f + NREG_F(13);
                sp34.y = 0.0f;
                sp34.z = 200.0f + NREG_F(14);
                MtxPosition(&sp34, &sp40);
                sp40 += sp4c;
                sp40.y = player->current.pos.y;
                player->setPlayerPosAndAngle(&sp40, a_this->current.angle.y + (u16)-0x8000, 0);
            }
            break;
        case 5:
            i_this->field_0xc90 = 2500.0f + NREG_F(12) - 50.0f;
            i_this->field_0xc9c = -700.0f + TREG_F(14) + 200.0f + 1000.0f;
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            cMtx_XrotM(*calc_mtx, i_this->field_0xc9c);
            sp34.x = 0.0f;
            sp34.y = 0.0f;
            sp34.z = 3500.0f + NREG_F(0);
            MtxPosition(&sp34, &i_this->mCamCenter);
            i_this->mCamCenter += a_this->current.pos;
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            cMtx_XrotM(*calc_mtx, 2000 + NREG_S(2));
            sp34.x = 0.0f;
            sp34.y = 0.0f;
            sp34.z = i_this->field_0xc90;
            MtxPosition(&sp34, &i_this->mCamEye);
            i_this->mCamEye += a_this->current.pos;
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp34.x = NREG_F(2) - 300.0f;
            sp34.y = 0.0f;
            sp34.z = 0.0f;
            MtxPosition(&sp34, &sp40);
            i_this->mCamEye += sp40;

            if (i_this->mDemoCamTimer == 5) {
                daPy_getPlayerActorClass()->changeDemoMode(23, 1, 2, 0);
            }

            if (i_this->mDemoCamTimer >= 50) {
                i_this->mDemoCamTimer = 0;
                i_this->mDemoMode = 6;
            }
            break;
        case 6:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp34.x = 0.0f;
            sp34.y = 150.0f + NREG_F(9);
            sp34.z = 700.0f + NREG_F(10);
            MtxPosition(&sp34, &sp40);
            i_this->mCamEye = a_this->current.pos + sp40;
            i_this->mCamCenter = a_this->current.pos;
            i_this->mCamCenter.y += 230.0f + NREG_F(11);

            if (i_this->mDemoCamTimer == 2) {
                i_this->field_0x707 = 7;
            }

            if (i_this->mDemoCamTimer >= 10) {
                i_this->mMode = 4;
                i_this->mDemoMode = 100;
                sp34.x = 200.0f + JREG_F(14);
                sp34.y = 0.0f;
                sp34.z = -150.0f + JREG_F(15);
                MtxPosition(&sp34, &sp40);
                sp40 += player->current.pos;
                player->setPlayerPosAndAngle(&sp40, a_this->current.angle.y + (u16)-0x8000, 0);
            }
            break;
        case 7:
            if (actor_sp14 == NULL) break;

            i_this->mCamCenter = actor_sp14->current.pos;
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp34.x = 300.0f + NREG_F(16);
            sp34.y = -200.0f + NREG_F(17);
            sp34.z = 1000.0f + NREG_F(18);
            MtxPosition(&sp34, &sp40);
            i_this->mCamEye = a_this->current.pos + sp40;
            i_this->mDemoMode = 8;
            i_this->mDemoCamTimer = 0;
            // fallthrough
        case 8:
            cLib_addCalc2(&i_this->mCamCenter.x, actor_sp14->current.pos.x, 0.2f, 100.0f);
            cLib_addCalc2(&i_this->mCamCenter.y, actor_sp14->current.pos.y + i_this->field_0xc8c, 0.2f, 100.0f);
            cLib_addCalc2(&i_this->mCamCenter.z, actor_sp14->current.pos.z, 0.2f, 100.0f);
            cLib_addCalc2(&i_this->field_0xc8c, -300.0f + JREG_F(7), 1.0f, 5.0f + JREG_F(8));

            i_this->mCamEye += actor_sp14->speed * (0.45f + TREG_F(19));
            i_this->mCamEye.y -= 3.0f + JREG_F(3);
            if (i_this->mDemoCamTimer < 65) {
                break;
            }

            i_this->mDemoMode = 9;
            i_this->mDemoCamTimer = 0;
            // fallthrough
        case 9:
            actor_sp18 = fopAcM_SearchByID(i_this->mBabaChildID);
            actor_sp1c = fopAcM_SearchByID(i_this->mBabaChildID2);
            if (i_this->mDemoCamTimer == 0) {
                actor_sp18->current.pos.x = player->current.pos.x - 200.0f;
                actor_sp18->current.pos.y = player->current.pos.y + 500.0f;
                actor_sp18->current.pos.z = player->current.pos.z - 1200.0f;
                actor_sp18->speed.y = 0.0f;
                actor_sp1c->current.pos.x = player->current.pos.x + 200.0f;
                actor_sp1c->current.pos.y = player->current.pos.y + 1000.0f;
                actor_sp1c->current.pos.z = player->current.pos.z - 1000.0f;
                actor_sp1c->speed.y = 0.0f;
                i_this->mCamCenter.x = actor_sp18->current.pos.x;
                i_this->mCamCenter.z = actor_sp18->current.pos.z;
            }

            i_this->mCamEye.x = player->current.pos.x + 100.0f + ZREG_F(0);
            i_this->mCamEye.y = player->current.pos.y + 100.0f + ZREG_F(1);
            i_this->mCamEye.z = player->current.pos.z - 400.0f + ZREG_F(2);
            i_this->mCamCenter.y = actor_sp18->current.pos.y + ZREG_F(3);

            cLib_addCalc2(&i_this->mCamCenter.x, actor_sp18->current.pos.x + 100.0f, 0.1f, 50.0f);
            cLib_addCalc2(&i_this->mCamCenter.z, actor_sp18->current.pos.z, 0.1f, 50.0f);

            if (i_this->mDemoCamTimer <= (s16)(60 + AREG_S(0))) {
                actor_sp14->current.pos -= actor_sp14->speed;
            }

            if (i_this->mDemoCamTimer >= (s16)(80 + AREG_S(0))) {
                i_this->mDemoMode = 10;
                i_this->mDemoCamTimer = 0;
            }
            break;
        case 10:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp34.x = 0.0f;
            sp34.y = 150.0f + NREG_F(9);
            sp34.z = 700.0f + NREG_F(10);
            MtxPosition(&sp34, &sp40);
            i_this->mCamEye = a_this->current.pos + sp40;
            i_this->mCamCenter = a_this->current.pos;
            i_this->mCamCenter.y += 230.0f + NREG_F(11);

            actor_sp18 = fopAcM_SearchByID(i_this->mBabaChildID);
            actor_sp1c = fopAcM_SearchByID(i_this->mBabaChildID2);
            actor_sp18->current.pos.x = player->current.pos.x - 200.0f;
            actor_sp18->current.pos.z = player->current.pos.z - 1200.0f;
            actor_sp1c->current.pos.x = player->current.pos.x + 200.0f;
            actor_sp1c->current.pos.z = player->current.pos.z - 1000.0f;
            break;
        case 11:
            if (i_this->mDemoCamTimer >= (s16)(30 + JREG_S(9))) {
                cMtx_YrotS(*calc_mtx, player->shape_angle.y);
                sp34.x = 0.0f;
                sp34.y = 200.0f;
                sp34.z = -400.0f;
                MtxPosition(&sp34, &i_this->mCamEye);
                i_this->mCamEye += player->current.pos;
                i_this->mCamCenter = player->current.pos;
                i_this->mCamCenter.y += 100.0f;
                i_this->field_0xc30 = 10;
                fopAcM_OffStatus(a_this, 0x4000);
                Z2GetAudioMgr()->subBgmStart(Z2BGM_BOOMERAMG_MONKEY);
            }

            actor_sp18 = fopAcM_SearchByID(i_this->mBabaChildID);
            actor_sp1c = fopAcM_SearchByID(i_this->mBabaChildID2);
            actor_sp18->current.pos.x = player->current.pos.x - 200.0f;
            actor_sp18->current.pos.z = player->current.pos.z - 1200.0f;
            actor_sp1c->current.pos.x = player->current.pos.x + 200.0f;
            actor_sp1c->current.pos.z = player->current.pos.z - 1000.0f;
            break;
        case 100:
            break;
    }

    if (dComIfGp_getEvent().checkSkipEdge()) {
        u8 i_no = (fopAcM_GetParam(i_this) & 0xff0000) >> 16;
        if (i_no != 0xff) {
            dComIfGs_onSwitch(i_no, fopAcM_GetRoomNo(a_this));
        }

        dStage_changeScene(2, 0.0f, 0, fopAcM_GetRoomNo(a_this), 0, -1);
    }
}

static void demo_camera_end(e_mk_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* actor = (fopAc_ac_c*)fopAcM_SearchByID(i_this->field_0x708);
    camera_class* camera = (camera_class*)dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp2c, i_pos, sp44, sp50;

    switch (i_this->mDemoMode) {
        case 1:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->mDemoMode = 2;
            i_this->mDemoCamTimer = 0;
            i_this->mDemoCamFovy = 55.0f;
            camera->mCamera.SetTrimSize(3);
            daPy_getPlayerActorClass()->changeOriginalDemo();
            actor->current.pos.y += 10000.0f;
            Z2GetAudioMgr()->bgmStreamPrepare(0x200000E);
            dComIfGs_onStageMiddleBoss();
            // fallthrough
        case 2:
            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
            sp2c.x = 0.0f;
            sp2c.y = 130.0f;
            sp2c.z = 0.0f;
            MtxPosition(&sp2c, &i_this->mCamCenter);
            i_this->mCamCenter += a_this->current.pos;
            sp2c.x = 400.0f;
            sp2c.y = 100.0f;
            sp2c.z = 200.0f;
            MtxPosition(&sp2c, &i_this->mCamEye);
            i_this->mCamEye += a_this->current.pos;
            sp2c.x = 0.0f;
            sp2c.y = 0.0f;
            sp2c.z = -300.0f;
            MtxPosition(&sp2c, &i_pos);
            i_pos += a_this->current.pos;
            player->setPlayerPosAndAngle(&i_pos, a_this->shape_angle.y, 0);

            if (i_this->mDemoCamTimer < 30) {
                return;
            }

            i_this->mDemoMode++;
            i_this->mDemoCamTimer = 0;
            return;

        case 3:
            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
            sp2c.x = -300.0f;
            sp2c.y = 0.0f;
            sp2c.z = -150.0f;
            MtxPosition(&sp2c, &i_this->mCamCenter);
            i_this->mCamCenter += a_this->current.pos;
            sp2c.x = 100.0f;
            sp2c.y = 1000.0f;
            sp2c.z = 0.0f;
            MtxPosition(&sp2c, &i_this->mCamEye);
            i_this->mCamEye += a_this->current.pos;
            return;

        case 4:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp2c.x = 250.0f;
            sp2c.y = 0.0f;
            sp2c.z = -200.0f;
            MtxPosition(&sp2c, &i_this->mCamCenter);
            i_this->mCamCenter += STAGE_CENTER_POS;
            sp2c.x = 250.0f;
            sp2c.y = 700.0f;
            sp2c.z = 300.0f;
            MtxPosition(&sp2c, &i_this->mCamEye);
            i_this->mCamEye += STAGE_CENTER_POS;
            return;

        case 5:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp2c.x = 450.0f;
            sp2c.y = 0.0f;
            sp2c.z = -100.0f;
            MtxPosition(&sp2c, &i_this->mCamCenter);
            i_this->mCamCenter += STAGE_CENTER_POS;
            sp2c.x = 250.0f;
            sp2c.y = 300.0f;
            sp2c.z = 300.0f;
            MtxPosition(&sp2c, &i_this->mCamEye);
            i_this->mCamEye += STAGE_CENTER_POS;
            sp2c.x = 800.0f;
            sp2c.y = 100.0f;
            sp2c.z = 100.0f;
            MtxPosition(&sp2c, &i_pos);
            i_pos += STAGE_CENTER_POS;
            i_this->field_0xc68.x = fabsf(i_pos.x - i_this->mCamEye.x) * 0.02f;
            i_this->field_0xc68.y = fabsf(i_pos.y - i_this->mCamEye.y) * 0.02f;
            i_this->field_0xc68.z = fabsf(i_pos.z - i_this->mCamEye.z) * 0.02f;
            i_this->mDemoMode = 6;
            i_this->field_0xc84 = 0;
            sp2c.x = 1400.0f;
            sp2c.y = 0.0f;
            sp2c.z = 500.0f;
            MtxPosition(&sp2c, &i_pos);
            i_pos += STAGE_CENTER_POS;
            player->setPlayerPosAndAngle(&i_pos, a_this->shape_angle.y, 0);
            // fallthrough
        case 6:
            cLib_addCalc2(&i_this->mCamCenter.x, i_this->field_0x5e4.x, 0.1f, i_this->field_0xc84 * 20.0f);
            cLib_addCalc2(&i_this->mCamCenter.y, i_this->field_0x5e4.y + 30.0f, 0.1f, i_this->field_0xc84 * 20.0f);
            cLib_addCalc2(&i_this->mCamCenter.z, i_this->field_0x5e4.z, 0.1f, i_this->field_0xc84 * 20.0f);
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp2c.x = 800.0f;
            sp2c.y = 100.0f;
            sp2c.z = 100.0f;
            MtxPosition(&sp2c, &i_pos);

            i_pos += STAGE_CENTER_POS;
            cLib_addCalc2(&i_this->field_0xc84, 1.0f, 1.0f , 0.1f);

            if (i_this->mDemoCamTimer >= 10) {
                cLib_addCalc2(&i_this->mCamEye.x, i_pos.x, 0.1f, i_this->field_0xc68.x * i_this->field_0xc88);
                cLib_addCalc2(&i_this->mCamEye.y, i_pos.y, 0.1f, i_this->field_0xc68.y * i_this->field_0xc88);
                cLib_addCalc2(&i_this->mCamEye.z, i_pos.z, 0.1f, i_this->field_0xc68.z * i_this->field_0xc88);
                cLib_addCalc2(&i_this->field_0xc88, 1.0f, 1.0f, 0.1f);
            }
            
            if (i_this->mDemoCamTimer == 40) {
                Z2GetAudioMgr()->seStart(Z2SE_MK_DARK_INSECT_DEATH, &i_this->field_0x5e4, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
            
            if (i_this->mDemoCamTimer < 90) {
                return;
            }

            if (i_this->mDemoCamTimer == 90) {
                sp2c.x = 1300.0f;
                sp2c.y = 0.0f;
                sp2c.z = 0.0f;
                MtxPosition(&sp2c, &i_pos);
                i_pos += STAGE_CENTER_POS;
                player->setPlayerPosAndAngle(&i_pos, a_this->shape_angle.y, 0);
                player->changeDemoMode(3, 1, 0, 0);
                sp2c.x = 170.0f;
                sp2c.y = 0.0f;
                sp2c.z = 0.0f;
                MtxPosition(&sp2c, &i_pos);
                i_pos += i_this->field_0x5e4;
                player->changeDemoPos0(&i_pos);
            }

            if (i_this->mDemoCamTimer == 130) {
                ok_anm_init(i_this, 42, 1.0f, 0, 1.0f);
            }

            if (i_this->mDemoCamTimer == 180) {
                i_this->field_0x60c = 0;
                cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
                sp2c.x = 0.0f;
                sp2c.y = 0.0f;
                sp2c.z = -20.0f;
                MtxPosition(&sp2c, &i_pos);
                i_pos += i_this->field_0x5e4;
                fopAcM_createDisappear(a_this, &i_pos, 5, 0, 0xff);
            }

            if (i_this->mDemoCamTimer != 248) {
                return;
            }

            i_this->mDemoMode++;
            i_this->mDemoCamTimer = 0;
            i_this->mMode = 5;
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp2c.x = 300.0f;
            sp2c.y = 0.0f;
            sp2c.z = 300.0f;
            MtxPosition(&sp2c, &i_pos);
            i_pos += STAGE_CENTER_POS;
            a_this->current.pos.x = i_pos.x;
            a_this->current.pos.z = i_pos.z;
            sp2c.x = 130.0f;
            sp2c.y = 100.0f;
            sp2c.z = 100.0f;
            MtxPosition(&sp2c, &i_this->mCamEye);
            i_this->mCamEye += STAGE_CENTER_POS;
            i_this->mCamCenter = a_this->current.pos;
            i_this->mCamCenter.y += 190.0f;
            sp2c.x = 400.0f;
            sp2c.y = 0.0f;
            sp2c.z = 700.0f;
            MtxPosition(&sp2c, &i_pos);
            i_pos += STAGE_CENTER_POS;
            sp2c.x = STAGE_CENTER_POS.x - i_pos.x;
            sp2c.z = STAGE_CENTER_POS.z - i_pos.z;
            player->setPlayerPosAndAngle(&i_pos, cM_atan2s(sp2c.x, sp2c.z), 0);
            a_this->current.angle.y = player->shape_angle.y;
            Z2GetAudioMgr()->bgmStreamPlay();
            // fallthrough
        case 7:
            return;

        case 8:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp2c.x = 500.0f;
            sp2c.y = 300.0f;
            sp2c.z = 0.0f;
            MtxPosition(&sp2c, &i_this->mCamCenter);
            i_this->mCamCenter += STAGE_CENTER_POS;
            sp2c.x = 50.0f;
            sp2c.y = 70.0f;
            sp2c.z = 1100.0f;
            MtxPosition(&sp2c, &i_this->mCamEye);
            i_this->mCamEye += STAGE_CENTER_POS;
            return;
        
        case 9:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp2c.x = 5500.0f;
            sp2c.y = 2300.0f;
            sp2c.z = -5000.0f;
            MtxPosition(&sp2c, &i_this->mCamCenter);
            i_this->mCamCenter += STAGE_CENTER_POS;
            sp2c.x = 450.0f;
            sp2c.y = 370.0f;
            sp2c.z = -100.0f;
            MtxPosition(&sp2c,&i_this->mCamEye);
            i_this->mCamEye += STAGE_CENTER_POS;
            if (i_this->mDemoCamTimer < 300) return;
            i_this->mDemoMode = 10;
            i_this->mDemoCamTimer = 0;
            sp2c.x = 400.0f;
            sp2c.y = 0.0f;
            sp2c.z = 600.0f;
            MtxPosition(&sp2c, &i_pos);
            i_pos += STAGE_CENTER_POS;
            sp2c.x = STAGE_CENTER_POS.x - player->current.pos.x;
            sp2c.z = STAGE_CENTER_POS.z - player->current.pos.z;
            player->setPlayerPosAndAngle(&i_pos, cM_atan2s(sp2c.x, sp2c.z), 0);
            return;

        case 10:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp2c.x = 0.0f;
            sp2c.y = 300.0f;
            sp2c.z = -100.0f;
            MtxPosition(&sp2c, &i_this->mCamCenter);
            i_this->mCamCenter += STAGE_CENTER_POS;
            sp2c.x = 800.0f;
            sp2c.y = 0.0f;;
            sp2c.z = 500.0f;
            MtxPosition(&sp2c ,&i_pos);
            i_pos += STAGE_CENTER_POS;
            i_this->field_0xc74.x = fabsf(i_pos.x - i_this->mCamCenter.x) * 0.01f;
            i_this->field_0xc74.y = fabsf(i_pos.y - i_this->mCamCenter.y) * 0.01f;
            i_this->field_0xc74.z = fabsf(i_pos.z - i_this->mCamCenter.z) * 0.01f;
            sp2c.x = 450.0f;
            sp2c.y = 70.0f;
            sp2c.z = 900.0f;
            MtxPosition(&sp2c, &i_this->mCamEye);
            i_this->mCamEye += STAGE_CENTER_POS;
            sp2c.x = -150.0f;
            sp2c.y = 45.0f;
            sp2c.z = 700.0f;
            MtxPosition(&sp2c, &i_pos);
            i_pos += STAGE_CENTER_POS;
            i_this->field_0xc68.x = fabsf(i_pos.x - i_this->mCamEye.x) * 0.01f;
            i_this->field_0xc68.y = fabsf(i_pos.y - i_this->mCamEye.y) * 0.01f;
            i_this->field_0xc68.z = fabsf(i_pos.z - i_this->mCamEye.z) * 0.01f;
            i_this->field_0xc84 = 0.0f;
            i_this->mDemoMode = 11;
            // fallthrough
        case 11:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            sp2c.x = 800.0f;
            sp2c.y = 0.0f;
            sp2c.z = 500.0f;
            MtxPosition(&sp2c, &i_pos);
            i_pos += STAGE_CENTER_POS;
            cLib_addCalc2(&i_this->mCamCenter.x, i_pos.x, 0.1f, i_this->field_0xc74.x * i_this->field_0xc84);
            cLib_addCalc2(&i_this->mCamCenter.y, i_pos.y, 0.1f, i_this->field_0xc74.y * i_this->field_0xc84);
            cLib_addCalc2(&i_this->mCamCenter.z, i_pos.z, 0.1f, i_this->field_0xc74.z * i_this->field_0xc84);
            sp2c.x = -150.0f;
            sp2c.y = 45.0f;
            sp2c.z = 700.0f;
            MtxPosition(&sp2c, &i_pos);
            i_pos += STAGE_CENTER_POS;
            cLib_addCalc2(&i_this->mCamEye.x, i_pos.x, 0.1f, i_this->field_0xc68.x * i_this->field_0xc84);
            cLib_addCalc2(&i_this->mCamEye.y, i_pos.y, 0.1f, i_this->field_0xc68.y * i_this->field_0xc84);
            cLib_addCalc2(&i_this->mCamEye.z, i_pos.z, 0.1f, i_this->field_0xc68.z * i_this->field_0xc84);
            cLib_addCalc2(&i_this->field_0xc84, 0.5f, 1.0f, 0.02f);

            if (i_this->mDemoCamTimer == 2) {
                fopAc_ac_c* local_b0 = fopAcM_SearchByID(i_this->field_0x708);
                sp2c.x = 50.0f;
                sp2c.y = 10.0f;
                sp2c.z = 700.0f;
                MtxPosition(&sp2c, &local_b0->current.pos);
                local_b0->current.pos += STAGE_CENTER_POS;
                local_b0->current.angle.y = 0;
                local_b0->shape_angle.y = STAGE_ANGLE_Y;
            }

            if (i_this->mDemoCamTimer == 200) {
                player->changeDemoMode(1, 3, 0, 0);
                player->changeDemoParam0(3);
                dComIfGp_getEvent().setPtT(actor);
            }

            if (i_this->mDemoCamTimer != 250) {
                return;
            }

            i_this->mDemoMode = 20;
            i_this->mDemoCamTimer = 0;
            return;

        case 20:
            sp2c = actor->current.pos - player->current.pos;
            if (i_this->mDemoCamTimer == 3) {
                player->changeDemoMode(3, 1, 0, 0);
                cMtx_YrotS(*calc_mtx, cM_atan2s(sp2c.x, sp2c.z));
                sp2c.x = -20.0f;
                sp2c.y = 0.0f;
                sp2c.z = 200.0f;
                MtxPosition(&sp2c, &i_pos);
                i_pos += player->current.pos;
                player->changeDemoPos0(&i_pos);
                sp2c.z = 10000.0f;
                MtxPosition(&sp2c, &i_pos);
                a_this->current.pos = i_pos;
            }

            i_pos = actor->current.pos - player->eyePos;
            a_this->current.pos = player->eyePos + (i_pos * 20.0f);
            if (i_this->mDemoCamTimer != 20) {
                return;
            }

            actor->health = 1;
            i_this->mDemoMode = 21;
            i_this->mDemoCamTimer = 0;
            i_this->field_0xc84 = 0.0f;
            return;

        case 21:
            cLib_addCalc2(&i_this->mCamCenter.y, actor->current.pos.y - 20.0f, 0.2f, i_this->field_0xc84 * 200.0f);
            cLib_addCalc2(&i_this->mCamCenter.x, actor->current.pos.x, 0.4f, i_this->field_0xc84 * 200.0f);
            cLib_addCalc2(&i_this->mCamCenter.z, actor->current.pos.z, 0.4f, i_this->field_0xc84 * 200.0f);
            cLib_addCalc2(&i_this->mCamEye.y, player->current.pos.y + 110.0f, 0.1f, i_this->field_0xc84 * 10.0f);
            cLib_addCalc2(&i_this->field_0xc84, 1.0f, 1.0f, 0.02f);
            i_pos = actor->current.pos - player->eyePos;
            a_this->current.pos = player->eyePos + (i_pos * 20.0f);
            if (i_this->mDemoCamTimer < 70) {
                return;
            }

            if (i_this->mDemoCamTimer == 70) {
                i_this->mMsgFlow.init(a_this, 102, 0, NULL);
            }

            if (i_this->mMsgFlow.doFlow(a_this, NULL, 0) == 0) {
                return;
            }

            actor->health = 1;
            return;

        case 22:
            player->changeDemoMode(28, 0, 0, 0);
            i_this->mDemoMode++;
            i_this->mDemoCamTimer = 0;
            i_this->mSound.startCreatureSound(Z2SE_EN_MK_CATCH_BOOM, 0, -1);
            // fallthrough
        case 23:
            cLib_addCalc2(&i_this->mCamCenter.x, player->current.pos.x, 0.4f, i_this->field_0xc84 * 200.0f);
            cLib_addCalc2(&i_this->mCamCenter.z, player->current.pos.z, 0.4f, i_this->field_0xc84 * 200.0f);

            if (i_this->mDemoCamTimer == 20) {
                fopAcM_createItemForMidBoss(&player->current.pos, 64, fopAcM_GetRoomNo(a_this), 0, 0, 0, -1);
                i_this->field_0xc30 = 10;
                a_this->current.pos.y += 20000.0f;
                target_info_count = 0;
                fpcM_Search(s_h_sub, i_this);
                fopAcM_delete(static_cast<daPillar_c*>(target_info[0]));
                fopAcM_delete(static_cast<daPillar_c*>(target_info[7]));
            }
            break;

        case 0:
        case 100:
            break;
    }
}

static void* s_ks_sub(void* i_actor, void* i_data) {
    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KS) && static_cast<npc_ks_class*>(i_actor)->field_0x5b6 == 0) {
        return i_actor;
    }

    return NULL;
}

static void* s_brg_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_BRG) {
        static_cast<obj_brg_class*>(i_actor)->field_0xaef4 = 500.0f;
        static_cast<obj_brg_class*>(i_actor)->mType |= 4;
    }

    return NULL;
}

static void* s_brg_sub2(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_BRG) {
        static_cast<obj_brg_class*>(i_actor)->mType &= ~0x4;
    }

    return NULL;
}

static void demo_camera_r04(e_mk_class* i_this) {
    u8 i_no;
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c *)dComIfGp_getPlayer(0);
    fopAc_ac_c* actor = fopAcM_SearchByID(i_this->field_0x708);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp2c, sp38;

    npc_ks_class* i_actor = (npc_ks_class*)fpcM_Search(s_ks_sub, i_this);
    if (i_actor == NULL) {
        return;
    }

    switch (i_this->mDemoMode) {
        case 1:
            if (!i_actor->actor.eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(&i_actor->actor, 2, 0xffff, 0);
                i_actor->actor.eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->mDemoMode = 2;
            i_this->mDemoCamTimer = 0;
            i_this->mDemoCamFovy = 55.0f;
            camera->mCamera.SetTrimSize(3);

            daPy_getPlayerActorClass()->changeOriginalDemo();
            i_this->mCamCenter.set(-711.0f, 3588.0f, 2730.0f);
            i_this->mCamEye.set(116.0f, 3400.0f, 4597.0f);
            i_this->field_0xc5c.set(-291.0f, 3256.0f, 2640.0f);
            i_this->field_0xc50.set(1060.0f, 3795.0f, 4086.0f);

            i_this->field_0xc74.x = fabsf(i_this->field_0xc5c.x - i_this->mCamCenter.x) * 0.015f;
            i_this->field_0xc74.y = fabsf(i_this->field_0xc5c.y - i_this->mCamCenter.y) * 0.015f;
            i_this->field_0xc74.z = fabsf(i_this->field_0xc5c.z - i_this->mCamCenter.z) * 0.015f;
            i_this->field_0xc68.x = fabsf(i_this->field_0xc50.x - i_this->mCamEye.x) * 0.02f;
            i_this->field_0xc68.y = fabsf(i_this->field_0xc50.y - i_this->mCamEye.y) * 0.02f;
            i_this->field_0xc68.z = fabsf(i_this->field_0xc50.z - i_this->mCamEye.z) * 0.02f;

            dComIfGp_getEvent().startCheckSkipEdge(i_this);
            // fallthrough
        case 2:
            if (i_this->mDemoCamTimer > 60) {
                cLib_addCalc2(&i_this->mCamCenter.x, i_this->field_0xc5c.x, 0.05f, i_this->field_0xc74.x * i_this->field_0xc84);
                cLib_addCalc2(&i_this->mCamCenter.y, i_this->field_0xc5c.y, 0.05f, i_this->field_0xc74.y * i_this->field_0xc84);
                cLib_addCalc2(&i_this->mCamCenter.z, i_this->field_0xc5c.z, 0.05f, i_this->field_0xc74.z * i_this->field_0xc84);
                cLib_addCalc2(&i_this->mCamEye.x, i_this->field_0xc50.x, 0.05f, i_this->field_0xc68.x * i_this->field_0xc84);
                cLib_addCalc2(&i_this->mCamEye.y, i_this->field_0xc50.y, 0.05f, i_this->field_0xc68.y * i_this->field_0xc84);
                cLib_addCalc2(&i_this->mCamEye.z, i_this->field_0xc50.z, 0.05f, i_this->field_0xc68.z * i_this->field_0xc84);
                cLib_addCalc2(&i_this->field_0xc84, 1.0f, 1.0f, 0.01f);

                if (i_this->mDemoCamTimer == 258) {
                    i_actor->mMode = 2;
                    sp38.set(110.0f, 3300.0f, 4326.0f);
                    player->setPlayerPosAndAngle(&sp38, (s16)0xffff8000, 0);
                    player->changeDemoMode(1, 1, 0, 0);
                }

                if (i_this->mDemoCamTimer == 260) {
                    cMtx_YrotS(*calc_mtx, i_actor->actor.shape_angle.y);
                    sp2c.x = 200.0f;
                    sp2c.y = 300.0f;
                    sp2c.z = 300.0f;
                    MtxPosition(&sp2c, &i_this->mCamEye);
                    i_this->mCamEye += i_actor->actor.current.pos;
                    i_this->mCamCenter = i_actor->actor.current.pos;
                    i_this->mCamCenter.y += 130.0f;
                    i_this->mDemoMode = 3;
                    i_this->mDemoCamTimer = 0;
                }
            }

            a_this->current.pos.y = a_this->home.pos.y + 10000.0f;
            return;

        case 3:
            a_this->current.pos.y = a_this->home.pos.y + 10000.0f;
            cLib_addCalc2(&i_this->mCamCenter.z, i_actor->actor.current.pos.z, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->mCamCenter.y, i_actor->actor.current.pos.y + 130.0f + cM_ssin(i_this->mDemoCamTimer * 0xA00) * 30.0f, 0.2f, 50.0f);

            if (i_this->mDemoCamTimer != 93) {
                return;
            }

            a_this->current.pos.y = a_this->home.pos.y;
            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
            sp2c.x = 0.0f;
            sp2c.y = -200.0f;
            sp2c.z = 3350.0f;
            MtxPosition(&sp2c, &i_this->mCamEye);
            i_this->mCamEye += a_this->current.pos;
            i_this->mCamCenter = a_this->current.pos;
            i_this->mCamCenter.y += 220.0f;
            i_this->mDemoMode = 4;
            i_this->mDemoCamTimer = 0;
            i_this->mMode = 2;
            i_actor->mMode = 20;
            return;
        
        case 4:
            cLib_addCalc2(&i_this->mCamCenter.z, a_this->current.pos.z, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->mCamCenter.y, a_this->current.pos.y + 220.0f, 0.2f, 50.0f);

            if (i_this->mDemoCamTimer != 74) {
                return;
            }

            i_this->mDemoMode = 45;
            i_this->mDemoCamTimer = 0;

            cMtx_YrotS(*calc_mtx, i_actor->actor.shape_angle.y);
            sp2c.x = -300.0f;
            sp2c.y = 200.0f;
            sp2c.z = -100.0f;
            MtxPosition(&sp2c, &i_this->mCamEye);
            i_this->mCamEye += i_actor->actor.current.pos;
            i_this->mCamCenter = i_actor->actor.current.pos;
            i_this->mCamCenter.y += 90.0f;
            return;

        case 45:
            cLib_addCalc2(&i_this->mCamCenter.z, i_actor->actor.current.pos.z, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->mCamCenter.y, i_actor->actor.current.pos.y + 90.0f + 30.0f * cM_ssin(i_this->mDemoCamTimer * 0xA00), 0.2f, 50.0f);

            if (i_this->mDemoCamTimer == 10) {
                i_actor->actor.field_0x567 = 1;
            }

            if (i_this->mDemoCamTimer != 55) {
                return;
            }

            i_this->mDemoMode = 46;
            i_this->mDemoCamTimer = 0;
            // fallthrough
        case 46:
            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
            sp2c.x = 0.0f;
            sp2c.y = 100.0f;
            sp2c.z = 600.0f;
            MtxPosition(&sp2c, &i_this->mCamEye);
            i_this->mCamEye += a_this->current.pos;
            i_this->mCamCenter = a_this->current.pos;
            i_this->mCamCenter.y += 220.0f;

            if (i_this->mDemoCamTimer < 45) {
                return;
            }

            if (actor == NULL) {
                return;
            }

            i_this->mDemoMode = 5;
            i_this->mDemoCamTimer = 0;
            // fallthrough
        case 5:
            i_this->mCamCenter = actor->current.pos;
            i_this->mCamEye.set(-477.0f, 3119.0f, 1643.0f);

            if (i_this->mDemoCamTimer == 30) {
                i_actor->actor.current.pos.z = 1600.0f;
                i_actor->actor.current.pos.y = 3200.0f;
            }

            if (i_this->mDemoCamTimer != 69) {
                return;
            }

            i_this->mDemoMode = 6;
            i_this->mDemoCamTimer = 0;
            return;

        case 6:
            if (i_this->mDemoCamTimer == 3) {
                i_actor->mMode = 4;
            }

            if (i_this->mDemoCamTimer != 25) {
                return;
            }

            i_this->mDemoMode = 7;
            i_this->mDemoCamTimer = 0;

            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
            sp2c.x = -300.0f;
            sp2c.y = 100.0f;
            sp2c.z = 550.0f;
            MtxPosition(&sp2c, &i_this->mCamEye);
            i_this->mCamEye += a_this->current.pos;
            i_this->mCamCenter = a_this->current.pos;
            i_this->mCamCenter.y += 190.0f;
            // fallthrough
        case 7:
            i_actor->actor.speed.y = 0.0f;
            i_actor->actor.current.pos.set(0.0f, 2500.0f, 3373.0f);

            if (i_this->mDemoCamTimer != 250) {
                return;
            }

            i_this->mDemoMode = 8;
            i_this->mDemoCamTimer = 0;
            i_actor->mMode++;
            i_this->mCamEye.set(-282.0f, 2534.0f, 3147.0f);
            i_this->mCamCenter = i_actor->actor.current.pos;
            // fallthrough
        case 8:
            cLib_addCalc2(&i_this->mCamCenter.y, i_actor->actor.current.pos.y, 0.2f, 50.0f);
            fpcM_Search(s_brg_sub, i_this);

            if (i_this->mDemoCamTimer == 65) {
                i_actor->actor.current.pos.set(0.0f, 3310.0f, 3427.0f);
                i_actor->mMode++;
                i_this->mDemoMode = 9;
                i_this->mDemoCamTimer = 0;
                fpcM_Search(s_brg_sub2, i_this);
                i_this->mCamCenter.set(4.0f, 3393.0f, 4180.0f);
                i_this->mCamEye.set(-131.0f, 3452.0f, 4552.0f);
                player->changeDemoMode(1, 1, 0, 0);
            }
            return;

        case 9:
            if (i_this->mDemoCamTimer != 138) {
                return;
            }

            i_this->mCamCenter.set(30.0f, 3422.0f, 4520.0f);
            i_this->mCamEye.set(343.0f, 3422.0f, 4271.0f);
            i_this->mDemoMode = 10;
            i_this->mDemoCamTimer = 0;
            i_this->field_0xca0 = player->shape_angle.y;
            return;

        case 10:
            cLib_addCalcAngleS2(&i_this->field_0xca0, i_actor->field_0x5c8 + (u16)-0x8000, 4, 0x800);
            player->setPlayerPosAndAngle(&player->current.pos, i_this->field_0xca0, 0);

            if (i_this->mDemoCamTimer != 60) {
                return;
            }

            camera->mCamera.Reset(i_this->mCamCenter, i_this->mCamEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            fopAcM_delete(a_this);
            i_no = fopAcM_GetParam(i_this) >> 16;
            if (i_no == 0xFF) {
                return;
            }

            dComIfGs_onSwitch(i_no, fopAcM_GetRoomNo(a_this));
            return;

        case 0:
        case 100:
        default:
            return;
    }
}

static void demo_camera_bohit(e_mk_class* i_this) {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp20;

    switch (i_this->mDemoMode) {
        case 1:
            if (!i_this->enemy.eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(&i_this->enemy, 2, 0xffff, 0);
                i_this->enemy.eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            i_this->mDemoMode = 2;
            i_this->mDemoCamTimer = 0;
            i_this->mDemoCamFovy = 55.0f;
            camera->mCamera.SetTrimSize(3);

            cMtx_YrotS(*calc_mtx, i_this->enemy.shape_angle.y);
            if (cM_rndF(1.0f) < 0.5f) {
                sp20.x = 500.0f;
            } else {
                sp20.x = -500.0f;
            }

            sp20.y = -120.0f;
            sp20.z = 0.0f;
            MtxPosition(&sp20, &i_this->mCamEye);
            i_this->mCamEye += i_this->enemy.current.pos;
            i_this->mCamCenter = i_this->enemy.current.pos;
            // fallthrough
        case 2:
            cLib_addCalc2(&i_this->mCamCenter.x, i_this->enemy.current.pos.x, 0.5f, 50.0f);
            cLib_addCalc2(&i_this->mCamCenter.y, i_this->enemy.current.pos.y + 100.0f, 0.5f, 50.0f);
            cLib_addCalc2(&i_this->mCamCenter.z, i_this->enemy.current.pos.z, 0.5f, 50.0f);

            if (i_this->mDemoCamTimer == 25) {
                i_this->field_0xc98 = 30.0f;
            }

            if (i_this->mDemoCamTimer == 80) {
                i_this->field_0xc30 = 10;
                i_this->field_0x6fc[0] = 150;
            }
            return;

        case 0:
        default:
            return;
    }
}

static void demo_camera(e_mk_class* i_this) {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp34, sp40;

    switch (i_this->field_0xc30) {
        case 1:
            demo_camera_start(i_this);
            break;
        
        case 2:
            demo_camera_end(i_this);
            break;

        case 3:
            demo_camera_r04(i_this);
            if (dComIfGp_getEvent().checkSkipEdge()) {
                npc_ks_class* i_actor = (npc_ks_class*)fpcM_Search(s_ks_sub, i_this);
                if (i_actor != NULL) {
                    u8 i_no = fopAcM_GetParam(i_this) >> 16;
                    if (i_no != 0xFF) {
                        dComIfGs_onSwitch(i_no, fopAcM_GetRoomNo(&i_this->enemy));
                    }

                    i_no = fopAcM_GetParam(i_actor) >> 24;
                    if (i_no != 0xFF) {
                        dComIfGs_onSwitch(i_no, fopAcM_GetRoomNo(&i_actor->actor));
                    }

                    dComIfGp_setNextStage("D_MN05", 0, 4, 0);
                }
            }
            break;

        case 4:
            demo_camera_bohit(i_this);
            break;

        case 10:
            camera->mCamera.Reset(i_this->mCamCenter, i_this->mCamEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            i_this->field_0xc30 = 0;
            i_this->mDemoMode = 0;
            daPy_getPlayerActorClass()->cancelOriginalDemo();

        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 0:
            break;
    }

    if (i_this->field_0xc30 != 0) {
        s16 sVar1 = i_this->field_0xc98 * cM_scos(i_this->field_0x6b4 * 0x2C00) * 7.5f;
        cXyz i_center = i_this->mCamCenter;
        cXyz i_eye = i_this->mCamEye;

        i_center.y += i_this->field_0xc98 * cM_ssin(i_this->field_0x6b4 * 0x2A00);
        i_eye.y += i_this->field_0xc98 * cM_ssin(i_this->field_0x6b4 * 0x2A00);
        i_center.x += i_this->field_0xc98 * cM_ssin(i_this->field_0x6b4 * 0x2800);
        i_eye.x += i_this->field_0xc98 * cM_ssin(i_this->field_0x6b4 * 0x2800);
        i_center.z += i_this->field_0xc98 * cM_scos(i_this->field_0x6b4 * 0x2C00);
        i_eye.z += i_this->field_0xc98 * cM_scos(i_this->field_0x6b4 * 0x2C00);

        camera->mCamera.Set(i_center, i_eye, sVar1, i_this->mDemoCamFovy);

        cLib_addCalc0(&i_this->field_0xc98, 1.0f, 3.0f);
        i_this->mDemoCamTimer++;

        if (i_this->mDemoCamTimer > 10000) {
            i_this->mDemoCamTimer = 10000;
        }
    }
}

static void action(e_mk_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp28, sp34;
    i_this->mDistToPlayer = fopAcM_searchPlayerDistanceXZ(a_this);
    i_this->mAngleToPlayer = fopAcM_searchPlayerAngleY(a_this);
    damage_check(i_this);
    s8 sVar1 = 1;
    s8 sVar2 = 1;
    i_this->field_0x6f8 = 0;

    switch (i_this->mAction) {
        case e_mk_class::ACT_WAIT:
            e_mk_wait(i_this);
            break;
        
        case e_mk_class::ACT_MOVE:
            e_mk_move(i_this);
            break;

        case e_mk_class::ACT_SHOOT:
            e_mk_shoot(i_this);
            break;

        case e_mk_class::ACT_YORO:
            e_mk_yoro(i_this);
            break;

        case e_mk_class::ACT_DROP:
            e_mk_drop(i_this);
            i_this->field_0x6f8 = 1;
            break;

        case e_mk_class::ACT_DAMAGE:
            e_mk_damage(i_this);
            i_this->field_0x6f8 = 1;
            break;

        case e_mk_class::ACT_S_DEMO:
            e_mk_s_demo(i_this);
            sVar1 = 0;
            break;

        case e_mk_class::ACT_E_DEMO:
            i_this->field_0x6f8 = e_mk_e_demo(i_this);
            sVar1 = 0;
            sVar2 = 0;
            break;

        case e_mk_class::ACT_R04_DEMO:
            e_mk_r04_demo(i_this);
            i_this->field_0x6f8 = 1;
    }

    if (sVar2) {
        fopAcM_OnStatus(a_this, 0);
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 4, 0x2000);

    if (i_this->field_0x6f8 != 0) {
        a_this->current.pos += a_this->speed;
        a_this->speed.y += a_this->gravity;

        if (i_this->field_0x6f8 == 1) {
            i_this->mAcch.CrrPos(dComIfG_Bgsp());
        }
    } else {
        a_this->current.pos += a_this->speed;
    }

    if (i_this->field_0x71b != 0) {
        if (i_this->field_0x71b == 2) {
            J3DModel* model = i_this->mpModelMorf->getModel();
            PSMTXCopy(model->getAnmMtx(29), *calc_mtx);
            sp28.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&sp28, &sp34);
            fopAcM_effSmokeSet1(&i_this->field_0xc08, &i_this->field_0xc0c, &sp34, &a_this->shape_angle, 2.5f, &a_this->tevStr, 0);

            PSMTXCopy(model->getAnmMtx(33), *calc_mtx);
            MtxPosition(&sp28, &sp34);
            fopAcM_effSmokeSet1(&i_this->field_0xc10, &i_this->field_0xc14, &sp34, &a_this->shape_angle, 2.5f, &a_this->tevStr, 0);
        } else {
            fopAcM_effSmokeSet1(&i_this->field_0xc08, &i_this->field_0xc0c, &a_this->current.pos, &a_this->shape_angle, 2.5f, &a_this->tevStr, 1);
        }

        i_this->field_0x71b = 0;
    }

    if (sVar1) {
        daPy_getPlayerActorClass()->onBossRoomWait();
    }
}

static void anm_se_set(e_mk_class* i_this) {
    static u16 bo_eno_1[2] = {
        0x8214,
        0x8215,
    };    
    
    static u16 bo_eno_0[4] = {
        0x8211, 
        0x8212, 
        0x8213, 
        0x820C,
    };



    if (i_this->mAnm == 40) {
        if (i_this->mpModelMorf->checkFrame(2.0f) || i_this->mpModelMorf->checkFrame(13.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_BREATH, -1);
        }
    } else if (i_this->mAnm == 35) {
        if (i_this->mpModelMorf->checkFrame(3.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_DAMAGE_L, -1);
        }

        if ((i_this->mpModelMorf->checkFrame(21.0f) || i_this->mpModelMorf->checkFrame(31.0f)) || i_this->mpModelMorf->checkFrame(41.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_MK_JUMP_END, 0, -1);
        }
    } else if (i_this->mAnm == 31 || i_this->mAnm == 32) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_PROVOKE_A, -1);
        }
    } else if (i_this->mAnm == 33) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_PROVOKE_B, -1);
        }

        if (i_this->mpModelMorf->checkFrame(15.0f) || i_this->mpModelMorf->checkFrame(64.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_MK_FOOTNOTE, 0, -1);
        }

        if (i_this->mpModelMorf->checkFrame(32.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_MK_SCRATCH_HIP, 0, -1);
        }
    } else if (i_this->mAnm == 25 || i_this->mAnm == 15) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_SNEER, -1);
        }
    } else if (i_this->mAnm == 10) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_DOWN, -1);
        }
    } else if (i_this->mAnm == 29) {
        if (i_this->mpModelMorf->checkFrame(8.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_MK_JUMP_START, 0, -1);
        }
    } else if (i_this->mAnm == 39) {
        if (i_this->mpModelMorf->checkFrame(20.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_MK_JUMP_START, 0, -1);
        }
    } else if (i_this->mAnm == 26) {
        if (i_this->mpModelMorf->checkFrame(3.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_MK_JUMP_END, 0, -1);
        }
    } else if (i_this->mAnm == 39) {
        if (i_this->mpModelMorf->checkFrame(38.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_MK_JUMP_END, 0, -1);
        }
    } else if (i_this->mAnm == 34) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_MK_V_BREATH, -1);
        }

        if (i_this->mpModelMorf->checkFrame(1.0f) || i_this->mpModelMorf->checkFrame(6.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_MK_FOOTNOTE, 0, -1);
        }
    }

    if (i_this->mAnm == 19 || i_this->mAnm == 21) {
        for (int i = 0; i < 2; i++) {
            i_this->field_0xc18[i] = dComIfGp_particle_set(i_this->field_0xc18[i], bo_eno_1[i], &i_this->enemy.current.pos, NULL, 0);

            JPABaseEmitter* pEmitter1 = dComIfGp_particle_getEmitter(i_this->field_0xc18[i]);
            if (pEmitter1 != NULL) {
                pEmitter1->setGlobalSRTMatrix(i_this->mpBoomerangModel->getBaseTRMtx());
            }
        }
    }

    if (i_this->mAnm == 21) {
        for (int i = 0; i < 4; i++) {
            i_this->field_0xc20[i] = dComIfGp_particle_set(i_this->field_0xc20[i], bo_eno_0[i], &i_this->enemy.eyePos, NULL, 0);

            JPABaseEmitter* pEmitter2 = dComIfGp_particle_getEmitter(i_this->field_0xc20[i]);
            if (pEmitter2 != NULL) {
                if (i == 0) {
                    pEmitter2->setGlobalSRTMatrix(i_this->mpModelMorf->getModel()->getAnmMtx(22));
                } else {
                    pEmitter2->setGlobalSRTMatrix(i_this->mpBoomerangModel->getBaseTRMtx());
                }
            }
        }
    }
}

static int daE_MK_Execute(e_mk_class* i_this) {
    cXyz sp6c, sp78, sp84;
    i_this->field_0x6b4++;

    for (int i = 0; i < 4; i++) {
        if (i_this->field_0x6fc[i] != 0) {
            i_this->field_0x6fc[i]--;
        }
    }

    if (i_this->field_0x704 != 0) {
        i_this->field_0x704--;
    }

    if (i_this->field_0x71a != 0) {
        i_this->field_0x71a--;
    }

    if (l_HIO.mHaltAction == 0) {
        action(i_this);
        demo_camera(i_this);
    }

    if (i_this->field_0x6be != 0) {
        sp6c = i_this->field_0x6cc - i_this->field_0x6c0;
        f32 fVar1 = sp6c.abs();

        sp6c = i_this->field_0x6cc - i_this->enemy.current.pos;
        f32 fVar2 = sp6c.abs();

        f32 fVar3 = fVar1 * 0.1f;

        if (fVar3 > 200.0f) {
            fVar3 = 200.0f;
        }

        int iVar1 = (fVar2 / fVar1) * 32768.0f;
        i_this->field_0x6d8 = fVar3 * cM_ssin(iVar1);
    } else {
        i_this->field_0x6d8 = 0;
    }

    mDoMtx_stack_c::transS(i_this->enemy.current.pos.x, i_this->enemy.current.pos.y + i_this->field_0x6d8, i_this->enemy.current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->enemy.shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->enemy.shape_angle.x);
    mDoMtx_stack_c::scaleM(l_HIO.mSize, l_HIO.mSize, l_HIO.mSize);

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(&i_this->enemy)));

    if (i_this->field_0x5d4 == 0) {
        i_this->field_0x5d4 = cM_rndF(60.0f) + 30.0f;
    } else {
        i_this->field_0x5d4--;

        if (i_this->field_0x5d4 <= 12) {
            i_this->field_0x5d8 = 11.99f - i_this->field_0x5d4;
        } else {
            i_this->field_0x5d8 = 0.0f;
        }
    }

    if (i_this->field_0x60e != 0) {
        i_this->mBtp->setFrame(i_this->field_0x5d8 + 12.0f);
    } else {
        i_this->mBtp->setFrame(i_this->field_0x5d8);
    }

    i_this->mpModelMorf->modelCalc();
    PSMTXCopy(model->getAnmMtx(7), *calc_mtx);
    sp6c.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp6c, &i_this->enemy.eyePos);
    i_this->enemy.attention_info.position = i_this->enemy.eyePos;
    i_this->enemy.attention_info.position.y += 70.0f;

    if (i_this->field_0x60c != 0) {
        if (i_this->field_0x5e0 == 0) {
            PSMTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(10), mDoMtx_stack_c::get());
            mDoMtx_stack_c::transM(l_HIO.mCrownPosAdjust.x, l_HIO.mCrownPosAdjust.y, l_HIO.mCrownPosAdjust.z);
            i_this->mpCrownModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
            mDoMtx_multVecZero(mDoMtx_stack_c::get(), &i_this->field_0x5e4);
        } else {
            cMtx_YrotS(*calc_mtx, i_this->field_0x5f0.y);
            sp6c.x = 0.0f;
            sp6c.y = i_this->field_0x5fc;
            sp6c.z = i_this->field_0x600;
            MtxPosition(&sp6c, &sp78);
            i_this->field_0x5e4 += sp78;
            i_this->field_0x5fc -= 5.0f;

            if (i_this->field_0x5e4.y <= STAGE_CENTER_POS.y + 5.0f) {
                i_this->field_0x5e4.y = STAGE_CENTER_POS.y + 5.0f;

                if (i_this->field_0x5fc < -30.0f) {
                    i_this->field_0x5fc *= -0.4f;
                    i_this->field_0x604 = 3500.0f;
                } else {
                    i_this->field_0x5fc = 0.0f;
                }

                i_this->field_0x600 *= 0.75f;
            }

            s16 sVar2 = i_this->field_0x604 * cM_ssin(i_this->field_0x6b4 * 3000);
            s16 sVar3 = i_this->field_0x604 * cM_ssin(i_this->field_0x6b4 * 4000);
            cLib_addCalc0(&i_this->field_0x604, 0.1f, 50.0f);
            mDoMtx_stack_c::transS(i_this->field_0x5e4.x, i_this->field_0x5e4.y, i_this->field_0x5e4.z);
            mDoMtx_stack_c::YrotM(i_this->field_0x5f0.y);
            mDoMtx_stack_c::XrotM(i_this->field_0x5f0.x + sVar2);
            mDoMtx_stack_c::ZrotM(sVar3);
            mDoMtx_stack_c::transM(0.0f, 15.0f, -10.0f);
            mDoMtx_stack_c::XrotM(-8000);
            mDoMtx_stack_c::ZrotM(-0x4000);
            i_this->mpCrownModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
            cLib_addCalcAngleS2(&i_this->field_0x5f0.x, 0, 2, 0x400);
        }

        i_this->mpCrownModelMorf->play(NULL, 0, 0);
        i_this->mpCrownModelMorf->modelCalc();
    }
    
    if (i_this->field_0x60d != 0) {
        PSMTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(23), mDoMtx_stack_c::get());
        mDoMtx_stack_c::scaleM(l_HIO.mBoomerangRatio, l_HIO.mBoomerangRatio, l_HIO.mBoomerangRatio);
        mDoMtx_stack_c::transM(15.0f, 70.0f, 20.0f);
        mDoMtx_stack_c::YrotM(-0x652C);
        mDoMtx_stack_c::XrotM(-0x2219);
        mDoMtx_stack_c::ZrotM(0x38D8);
        i_this->mpBoomerangModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    if (i_this->field_0x707 == 7) {
        u32 i_parameters = 0;
        if (i_this->mDemoMode != 0) {
            i_parameters = 1;
        }

        i_this->field_0x708 = fopAcM_createChild(PROC_E_MK_BO, fopAcM_GetID(i_this), i_parameters,
                                                 &i_this->enemy.current.pos, fopAcM_GetRoomNo(&i_this->enemy), NULL, NULL,
                                                 -1, NULL);
        i_this->field_0x707 = 0;
    }

    if (i_this->field_0x707 == 1) {
        e_mk_bo_class* i_actor = (e_mk_bo_class*)fopAcM_SearchByID(i_this->field_0x708);
        if (i_actor != NULL) {
            PSMTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(23), *calc_mtx);
            sp6c.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&sp6c, &sp78);
            i_actor->enemy.current.pos = sp78;
            i_actor->field_0x9b4 = 0;
            i_this->field_0x707 = 0;
        }
    }

    PSMTXCopy(model->getAnmMtx(24), *calc_mtx);
    sp6c.set(20.0f, 0.0f, 0.0f);
    MtxPosition(&sp6c, &sp78);

    if (i_this->mAction >= 9) {
        i_this->enemy.eyePos = sp78;
        i_this->enemy.attention_info.position = i_this->enemy.eyePos;
        i_this->enemy.attention_info.position.y += 40.0f;
    }

    if (i_this->field_0x704 != 0) {
        sp78.y += 100000.0f;
    }

    i_this->field_0xad0.SetC(sp78);
    i_this->field_0xad0.SetR(l_HIO.mSize * 60.0f);
    dComIfG_Ccsp()->Set(&i_this->field_0xad0);

    sp6c.set(-70.0f, 70.0f, 0.0f);
    MtxPosition(&sp6c, &sp78);

    if (i_this->field_0x704 != 0) {
        sp78.y += 100000.0f;
    }

    i_this->field_0x998.SetC(sp78);
    i_this->field_0x998.SetR(l_HIO.mSize * 60.0f);
    dComIfG_Ccsp()->Set(&i_this->field_0x998);

    if (i_this->field_0x706 != 0) {
        i_this->field_0x706--;
        if (i_this->field_0x706 == 0) {
            dComIfGp_particle_set(0x15E, &i_this->enemy.eyePos, &i_this->enemy.shape_angle, NULL);
        }
    }

    anm_se_set(i_this);
    return 1;
}

static int daE_MK_IsDelete(e_mk_class* i_this) {
    return 1;
}

static int daE_MK_Delete(e_mk_class* i_this) {
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, "E_mk");
    if (i_this->field_0xcf5 != 0) {
        lbl_210_bss_AA = 0;
    }

    if (i_this->enemy.heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    e_mk_class* i_this = (e_mk_class*)actor;
    i_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_mk", 48), NULL, NULL,
                                               (J3DAnmTransform*)dComIfG_getObjectRes("E_mk", 40), 0, 1.0f,
                                               0, -1, &i_this->mSound, 0x80000, 0x11020084);
    if (i_this->mpModelMorf == NULL || i_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    i_this->mBtp = new mDoExt_btpAnm();
    if (i_this->mBtp == NULL) {
        return 0;
    }
    if (i_this->mBtp->init(i_this->mpModelMorf->getModel()->getModelData(), (J3DAnmTexPattern*)dComIfG_getObjectRes("E_mk", 55),
                       1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }

    i_this->mpCrownModelMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("E_mk", 49), NULL, NULL,
                                              (J3DAnmTransform*)dComIfG_getObjectRes("E_mk", 41), 0, 1.0f,
                                              0, -1, 1, NULL, 0x80000, 0x11000084);
    if (i_this->mpCrownModelMorf == NULL || i_this->mpCrownModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModelData* i_modelData = (J3DModelData*)dComIfG_getObjectRes("E_mk", 46);
    JUT_ASSERT(4743, i_modelData != NULL);
    i_this->mpBoomerangModel = mDoExt_J3DModel__create(i_modelData, 0x80000, 0x11000084);
    if (i_this->mpBoomerangModel == NULL) {
        return 0;
    }

    return 1;
}

static int daE_MK_Create(fopAc_ac_c* i_actor) {
    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x43}, 0x75}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 60.0f} // mSph
        } // mSphAttr
    };

    e_mk_class* mk = (e_mk_class*)i_actor;

    fopAcM_ct(&mk->enemy, e_mk_class);

    int phase = dComIfG_resLoad(&mk->mPhase, "E_mk");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_MK PARAM %x\n", fopAcM_GetParam(i_actor));
        if (strcmp(dComIfGp_getStartStageName(), "D_MN05B") == 0 && dComIfGs_isStageMiddleBoss()) {
            OS_REPORT("中ボスE_MK やられ後なので再セットしません\n"); // Since it's the middle boss E_MK after being defeated, I won't reset it.
            return cPhs_ERROR_e;
        }

        u8 i_no = (fopAcM_GetParam(i_actor) & 0xff000000) >> 24;
        if (i_no != 0xFF) {
            if (dComIfGs_isSwitch(i_no, fopAcM_GetRoomNo(i_actor))) {
                OS_REPORT("E_MK やられ後なので再セットしません\n"); // Since it's after the E_MK was done, I won't reset it.
                return cPhs_ERROR_e;
            }
        }

        mk->mParam = fopAcM_GetParam(i_actor);
        OS_REPORT("E_MK//////////////E_MK SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_actor, useHeapInit, 0x56f0)) {
            OS_REPORT("//////////////E_MK SET NON !!\n");
            return cPhs_ERROR_e;
        }

        lbl_210_bss_130 = 0;
        if (lbl_210_bss_AA == 0) {
            mk->field_0xcf5 = 1;
            lbl_210_bss_AA = 1;
            l_HIO.field_0x4 = mDoHIO_CREATE_CHILD("ブーメラン猿", &l_HIO);
        }

        i_actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        fopAcM_SetMtx(i_actor, mk->mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_actor, -500.0f, -500.0f, -500.0f);
        fopAcM_SetMax(i_actor, 500.0f, 500.0f, 500.0f);

        mk->mAcch.Set(fopAcM_GetPosition_p(i_actor), fopAcM_GetOldPosition_p(i_actor), &mk->enemy, 1, &mk->mAcchCir,
                          fopAcM_GetSpeed_p(i_actor), NULL, NULL);
        mk->mAcchCir.SetWall(80.0f, 30.0f);
        i_actor->field_0x560 = i_actor->health = 200;

        mk->mSound.init(&i_actor->current.pos, &i_actor->eyePos, 3, 1);
        mk->mAtInfo.mpSound = &mk->mSound;

        i_actor->attention_info.distances[2] = 4;

        mk->field_0x95c.Init(0xFF, 0, i_actor);
        mk->field_0xad0.Set(cc_sph_src);
        mk->field_0xad0.SetStts(&mk->field_0x95c);
        mk->field_0x998.Set(cc_sph_src);
        mk->field_0x998.SetStts(&mk->field_0x95c);
        mk->field_0x998.OnTgNoHitMark();
        mk->field_0x60c = 1;
        mk->field_0x60d = 1;

        if (fopAcM_GetRoomNo(i_actor) == 4) {
            mk->mAction = e_mk_class::ACT_R04_DEMO;
            i_actor->current.pos.set(0.0f, 3900.0f, -3000.0f);
            i_actor->current.angle.y = 0;
            i_actor->home = i_actor->current;
        } else {
            STAGE_CENTER_POS = i_actor->home.pos;
            STAGE_CENTER_POS.y -= 500.0f;
            STAGE_ANGLE_Y = i_actor->home.angle.y;

            u8 i_no = (fopAcM_GetParam(i_actor) & 0xff0000) >> 16;
            if (i_no != 0xFF && dComIfGs_isSwitch(i_no, fopAcM_GetRoomNo(i_actor))) {
                dComIfGs_offSwitch(i_no, fopAcM_GetRoomNo(i_actor));
                mk->mAction = e_mk_class::ACT_WAIT;
                Z2GetAudioMgr()->subBgmStart(Z2BGM_BOOMERAMG_MONKEY);
            } else {
                mk->mAction = e_mk_class::ACT_S_DEMO;
                u32 i_parameters = fopAcM_GetParam(i_actor) & 0xFF000000 | 0xFFFF01;
                cXyz sp30(-21.0f, 5114.0f, -4941.0f);
                mk->mBabaChildID = fopAcM_createChild(PROC_E_DB, fopAcM_GetID(i_actor), i_parameters,
                                                        &sp30, fopAcM_GetRoomNo(i_actor), NULL,
                                                        NULL, -1, NULL);

                sp30.set(-10.0f, 5114.0f, -4401.0f);
                mk->mBabaChildID2 = fopAcM_createChild(PROC_E_DB, fopAcM_GetID(i_actor), i_parameters,
                                                        &sp30, fopAcM_GetRoomNo(i_actor), NULL,
                                                        NULL, -1, NULL);
                
                fopAcM_OnStatus(i_actor, 0x4000);
            }
        }

        i_actor->gravity = -5.0f;
        i_actor->scale.x = l_HIO.mSize * l_HIO.mBoomerangRatio;
        daE_MK_Execute(mk);
    }

    return phase;
}

AUDIO_INSTANCES

static actor_method_class l_daE_MK_Method = {
    (process_method_func)daE_MK_Create,
    (process_method_func)daE_MK_Delete,
    (process_method_func)daE_MK_Execute,
    (process_method_func)daE_MK_IsDelete,
    (process_method_func)daE_MK_Draw,
};

extern actor_process_profile_definition g_profile_E_MK = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_MK,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_mk_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  165,                    // mPriority
  &l_daE_MK_Method,       // sub_method
  0x00040100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
