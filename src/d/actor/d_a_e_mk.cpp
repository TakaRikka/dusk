/**
 * @file d_a_e_mk.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_mk.h"
#include "res/Object/E_mk.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_obj_pillar.h"
#include "d/actor/d_a_e_db.h"
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
    virtual ~daE_MK_HIO_c() {}
    daE_MK_HIO_c();

    void genMessage(JORMContext*);

    /* 0x04 */ s8 no;
    /* 0x08 */ f32 size;
    /* 0x0C */ f32 boomerang_ratio;
    /* 0x10 */ f32 pl_throw_dist_max;
    /* 0x14 */ u8 halt_action;
    /* 0x18 */ Vec crown_pos_adjust;
};

daE_MK_HIO_c::daE_MK_HIO_c() {
    no = -1;
    size = 1.3f;
    boomerang_ratio = 1.0f;
    pl_throw_dist_max = 1800.0f;
    halt_action = 0;
    crown_pos_adjust.x = 0.0f;
    crown_pos_adjust.y = 0.0f;
    crown_pos_adjust.z = 0.0f;
}

#if DEBUG
void daE_MK_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("  ブーメラン猿", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("基本サイズ", &size, 0.0f, 5.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("ブーメラン比率", &boomerang_ratio, 0.0f, 2.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("PL投max距離", &pl_throw_dist_max, 0.0f, 4000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genLabel("        ", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genCheckBox("動作停止", &halt_action, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genLabel("        ", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genLabel("王冠位置補正", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("　X", &crown_pos_adjust.x, -50.0f, 50.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("　Y", &crown_pos_adjust.y, -50.0f, 50.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("　Z", &crown_pos_adjust.z, -50.0f, 50.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

static void anm_init(e_mk_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->anmP->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_mk", i_index),
                                i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->anmNo = i_index;
}

static void ok_anm_init(e_mk_class* i_this, int i_index, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->crownAnmP->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_mk", i_index),
                                i_mode, i_morf, i_speed, 0.0f, -1.0f, NULL);
}

static int daE_MK_Draw(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    J3DModel* model_p = i_this->anmP->getModel();
    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(model_p, &actor->tevStr);
    i_this->btpP->entry(model_p->getModelData());
    i_this->anmP->entryDL();

    cXyz pos;
    if (i_this->unkFlag3 != 0) {
        pos.set(actor->current.pos.x, actor->current.pos.y + 50.0f + BREG_F(18), actor->current.pos.z);
        i_this->shadowKey1 = dComIfGd_setShadow(i_this->shadowKey1, 1, model_p, &pos, 
                                                 BREG_F(19) + 1200.0f, 0.0f, actor->current.pos.y, 
                                                 i_this->acch.GetGroundH(), i_this->acch.m_gnd, 
                                                 &i_this->actor.tevStr, 0, 1.0f, 
                                                 dDlst_shadowControl_c::getSimpleTex());
    }

    if (i_this->crownStatus != 0) {
        g_env_light.setLightTevColorType_MAJI(i_this->crownAnmP->getModel(), &actor->tevStr);
        i_this->crownAnmP->entryDL();
        if (i_this->unkFlag1 != 0) {
            pos.set(i_this->crownPos.x, actor->current.pos.y + 50.0f + BREG_F(18), i_this->crownPos.z);
            i_this->shadowKey2 = dComIfGd_setShadow(i_this->shadowKey2, 1, i_this->crownAnmP->getModel(), 
                                                     &pos, BREG_F(19) + 500.0f, 0.0f, actor->current.pos.y, 
                                                     i_this->acch.GetGroundH(), i_this->acch.m_gnd, 
                                                     &i_this->actor.tevStr, 0, 1.0f, 
                                                     dDlst_shadowControl_c::getSimpleTex());
        }
    }

    if (i_this->boomerangStatus != 0) {
        g_env_light.setLightTevColorType_MAJI(i_this->boomerangModelP, &actor->tevStr);
        mDoExt_modelUpdateDL(i_this->boomerangModelP);
    }

    return 1;
}

static void* s_obj_delete(void* i_actor, void* i_data) {
    UNUSED(i_data);
    
    if (fopAcM_IsActor(i_actor)) {
        if (fopAcM_GetName(i_actor) == PROC_E_DB) {
            fopAc_ac_c* actor = (fopAc_ac_c*)i_actor;
            actor->health = 1000;
        } else if (fopAcM_GetName(i_actor) == PROC_E_DB_LEAF) {
            fopAcM_delete((fopAc_ac_c*)i_actor);
        }
    }

    return NULL;
}

static cXyz demo_jump_pos_1(1050.0f, 500.0f, -450.0f);

static cXyz demo_jump_pos_2(1262.0f, 1030.0f, -1665.0f);

static cXyz demo_jump_pos_3(2400.0f, 1115.0f, -1850.0f);

static cXyz STAGE_CENTER_POS;

static s16 STAGE_ANGLE_Y;

u8 hio_set;

static daE_MK_HIO_c l_HIO;

static void* target_info[20];

static int target_info_count;

static void* s_h_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);

    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_Pillar) && target_info_count < 20) {
        target_info[target_info_count] = (daPillar_c*)i_actor;
        target_info_count++;
    }

    return NULL;
}

static daPillar_c* search_hasira(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    daPillar_c* pillar_p;

    target_info_count = 0;
    for (int i = 0; i < 20; i++) {
        target_info[i] = NULL;
    }

    fpcM_Search(s_h_sub, i_this);

    if (i_this->firstHasiraFlag == 0) {
        i_this->firstHasiraFlag++;
        return (daPillar_c*)target_info[TREG_S(7) + 5];
    }

    f32 dist = TREG_F(12) + 1500.0f;
    s16 angle = 0x7000; // 157.5°
    if (target_info_count != 0) {
        cXyz pos_delta;
        daPillar_c* rv = NULL;
        int iVar1 = 0;
        for (int i = 0; i < target_info_count;) {
            pillar_p = (daPillar_c*)target_info[i];
            pos_delta = pillar_p->current.pos - actor->current.pos;
            f32 abs_val = pos_delta.abs();
            if (abs_val > TREG_F(16) + 700.0f && abs_val < dist) {
                s16 angle_delta = actor->current.angle.y - cM_atan2s(pos_delta.x, pos_delta.z);
                if (angle_delta < angle && angle_delta > (s16)-angle) {
                    if (iVar1 == 0 || cM_rndF(1.0f) < 0.3f) {
                        rv = pillar_p;
                    }
                    
                    iVar1++;
                }
            }

            i++;
            if (i == target_info_count) {
                i = 0;
                dist += 100.0f;
                if (dist > 2000.0f) {
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
    UNUSED(i_data);
    
    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_DB)) {
        e_db_class* actor = (e_db_class*)i_actor;
        if (actor->action == 10) {
            if (actor->mode >= 1 && target_info_count < 20) {
                target_info[target_info_count] = i_actor;
                target_info_count++;
            }
        }
    }

    return NULL;
}

static e_db_class* search_db(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;

    target_info_count = 0;
    for (int i = 0; i < 20; i++) {
        target_info[i] = NULL;
    }

    fpcM_Search(s_d_sub, i_this);

    f32 dist = 2000.0f;
    if (target_info_count != 0) {
        cXyz pos_delta;
        e_db_class* actor_p;
        for (int i = 0; i < target_info_count;) {
            actor_p = (e_db_class*)target_info[i];
            pos_delta = actor_p->enemy.current.pos - actor->current.pos;
            f32 abs_val = pos_delta.abs();

            if (cM_rndF(1.0f) < 0.5f && abs_val > 500.0f && abs_val < dist) {
                return actor_p;
            }
            
            i++;
            if (i == target_info_count) {
                i = 0;
                dist += 50.0f;
                if (dist > 5000.0f) {
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
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz work;
    int frame = i_this->anmP->getFrame();

    switch(i_this->mode) {
        case 0:
            i_this->unkCounter1 = cM_rndF(3.0f) + 2.0f;
            i_this->mode++;
            // fallthrough
        case 1:
            i_this->posTargetStep.set(0.0f, 0.0f, 0.0f);
            actor->speed.set(0.0f, 0.0f, 0.0f);
            if (i_this->demoMode == e_mk_class::DEMO_MODE_END) {
                i_this->prevHasira = NULL;
                target_info_count = 0;
                fpcM_Search(s_h_sub, i_this);
                if (i_this->demoHasiraFlag == 0) {
                    work = demo_jump_pos_1;
                    i_this->prevHasira = (daPillar_c*)target_info[8];
                } else if (i_this->demoHasiraFlag == 1) {
                    work = demo_jump_pos_2;
                    i_this->demoSubMode++;
                    i_this->prevHasira = (daPillar_c*)target_info[6];
                } else if (i_this->demoHasiraFlag == 2) {
                    work = demo_jump_pos_3;
                } else {
                    i_this->action = e_mk_class::ACT_E_DEMO;
                    i_this->mode = 100;
                    actor->gravity = 0.0f;
                    actor->scale.x = 1.0f;
                    return;
                }

                i_this->demoHasiraFlag++;
                i_this->posTarget = STAGE_CENTER_POS + work;
                work = i_this->posTarget - actor->current.pos;
                actor->current.angle.y = cM_atan2s(work.x, work.z);
                actor->current.angle.x = -cM_atan2s(work.y, JMAFastSqrt(work.x * work.x + work.z * work.z));
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_JUMP_START_e, TREG_F(1) + 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 3;
                break;
            }
            
            i_this->prevHasira = (daPillar_c*)i_this->hasira;
            i_this->hasira = search_hasira(i_this);

            if (i_this->hasira != NULL) {
                i_this->mode++;
            } else {
                OS_REPORT("E_mk HASIRA NON \n");
                break;
            }
            // fallthrough
        case 2:
            i_this->posTarget = i_this->hasira->current.pos;
            i_this->posTarget.y += TREG_F(8) + 800.0f;
            if (fopAcM_gc_c::gndCheck(&i_this->posTarget)) {
                i_this->posTarget.y = fopAcM_gc_c::getGroundY();
            }

            work = i_this->posTarget - actor->current.pos;
            actor->current.angle.y = cM_atan2s(work.x, work.z);
            actor->current.angle.x = -cM_atan2s(work.y, JMAFastSqrt(work.x * work.x + work.z * work.z));
            anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_JUMP_START_e, TREG_F(1) + 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode++;
            break;

        case 3:
            // At frame 9 of the mk_jump_start animation, Ook's feet begin lifting off the ground
            if (frame == TREG_S(0) + 9) {
                cMtx_YrotS(*calc_mtx, actor->current.angle.y);
                cMtx_XrotM(*calc_mtx, actor->current.angle.x);
                work.x = 0.0f;
                work.y = 0.0f;
                work.z = TREG_F(10) + 70.0f;
                MtxPosition(&work, &actor->speed);
                i_this->unkFlag2 = 1;
                i_this->prevPos = actor->current.pos;
                i_this->prevPosTarget = i_this->posTarget;

                i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_JUMP, -1);
                i_this->sound.startCreatureSound(Z2SE_EN_MK_JUMP_WIND, 0, -1);
                if (i_this->unkFlag5 != 0) {
                    i_this->setSmokeFlag = 2;
                }

                if (i_this->prevHasira != NULL) {
                    work = i_this->prevHasira->current.pos;
                    work.y += 500.0f;
                    work -= actor->current.pos;
                    if (work.abs() < 200.0f) {
                        daPillar_c* pillar = i_this->prevHasira;
                        pillar->setShake(2);
                    }
                }
            }

            if (i_this->anmP->isStop()) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_JUMP_MIDDLE_e, TREG_F(2) + 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode++;
            }
            break;

        case 4:
            work = i_this->posTarget - actor->current.pos;
            if (work.abs() < TREG_F(13) + 300.0f) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_JUMP_END_e, TREG_F(3) + 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode++;
                i_this->posTargetStep = actor->speed;
                actor->speed.set(0.0f, 0.0f, 0.0f);
            }
            break;

        case 5:
            if (i_this->anmP->isStop() != 0) {
                i_this->unkFlag2 = 0;
                if (i_this->unkFlag5 != 0) {
                    i_this->mode = 10;
                    i_this->unkFlag5 = 0;
                    anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_PICKUP_e, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                } else {
                    i_this->unkCounter1--;
                    if (i_this->unkCounter1 == 0) {
                        if (cM_rndF(1.0f) < 0.75f) {
                            i_this->action = e_mk_class::ACT_SHOOT;
                            if ((cM_rndF(1.0f) < 0.5f && i_this->unkCounter3 >= 3) || i_this->distToPl > l_HIO.pl_throw_dist_max) {
                                i_this->mode = 1;
                            } else {
                                i_this->mode = 0;
                                i_this->unkCounter3++;
                            }
                            i_this->unkTimer1 = 0;
                        } else {
                            i_this->action = e_mk_class::ACT_WAIT;
                            i_this->mode = 0;
                        }
                    } else {
                        i_this->mode = 1;
                    }
                }
            }
            break;

        case 10:
            if (frame == 5) {
                i_this->unkFlag4 = 6;
                i_this->boomerangStatus = 1;
            }

            if (i_this->anmP->isStop()) {
                i_this->mode = 1;
            }
    }

    cLib_addCalc2(&actor->current.pos.x, i_this->posTarget.x, 1.0f, fabsf(i_this->posTargetStep.x));
    cLib_addCalc2(&actor->current.pos.z, i_this->posTarget.z, 1.0f, fabsf(i_this->posTargetStep.z));
    cLib_addCalc2(&actor->current.pos.y, i_this->posTarget.y, 1.0f, fabsf(i_this->posTargetStep.y));
}

static void e_mk_wait(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;

    switch (i_this->mode) {
        case 0:
            i_this->timer[0] = cM_rndF(40.0f) + 40.0f;
            if (cM_rndF(1.0f) < 0.5f) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_PROVOCATION_02_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 1;
            } else {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_PROVOCATION_03_e, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 2;
            }
            break;

        case 1:
            if (i_this->timer[0] == 0) {
                i_this->action = e_mk_class::ACT_MOVE;
                i_this->mode = 0;
            }
            break;

        case 2:
            if (i_this->anmP->isStop()) {
                i_this->action = e_mk_class::ACT_MOVE;
                i_this->mode = 0;
            }
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->angleToPl, 2, 0x1000);
    if (i_this->distToPl < TREG_F(19) + 800.0f) {
        i_this->action = e_mk_class::ACT_MOVE;
        i_this->mode = 0;
        OS_REPORT("E_MK PL NEAR \n");
    }
}

static void e_mk_shoot(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    int frame = i_this->anmP->getFrame();
    cXyz work;
    fopAc_ac_c* actor_p = fopAcM_SearchByID(i_this->boomerangId);
    daPillar_c* hasira = i_this->hasira;
    s8 unk_flag = 0;

    switch(i_this->mode) {
        case 0:
            i_this->db = NULL;
            anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_THROW_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 2;
            break;

        case 1:
            i_this->db = search_db(i_this);
            if (i_this->db != NULL) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_THROW_UP_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 10;
            } else {
                OS_REPORT("E_MK DB  NON \n");
                i_this->mode = 0;
            }
            break;

    case 2:
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->angleToPl, 2, 0x1000);

        if (i_this->anmP->checkFrame(15.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_THROW_BOOM, -1);
        }

        if (frame == hREG_S(2) + 17) {
            i_this->unkFlag4 = 7;
        }
        
        if (frame == hREG_S(3) + 27) {
            i_this->unkFlag4 = 1;
        }

        if (frame == hREG_S(4) + 27) {
            i_this->boomerangStatus = 0;
            i_this->mode = 3;
        }
        break;

    case 3:
        if (i_this->anmP->isStop()) {
            i_this->mode = 20;
            if (cM_rndF(1.0f) < 0.333f) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_PROVOCATION_01_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            else {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_WAIT_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
        }
        unk_flag = 1;
        break;

    case 10:
        work = i_this->db->enemy.current.pos - actor->current.pos;
        cLib_addCalcAngleS2(&actor->current.angle.y, cM_atan2s(work.x, work.z), 2, 0x1000);
        if (i_this->anmP->checkFrame(15.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_THROW_BOOM, -1);
        }

        if (frame == hREG_S(2) + 17) {
            i_this->unkFlag4 = 7;
        }

        if (frame == hREG_S(3) + 27) {
            i_this->unkFlag4 = 1;
        }

        if (frame == hREG_S(4) + 27) {
            i_this->boomerangStatus = 0;
            i_this->mode = 3;
        }
        break;

    case 15:
        if (i_this->anmP->isStop()) {
            i_this->action = e_mk_class::ACT_MOVE;
            i_this->mode = 0;
        }
        unk_flag = 3;
        break;

    case 20:
        unk_flag = 2;
    }

    if (unk_flag != 0) {
        if ((unk_flag == 2) && (actor_p != NULL)) {
            cXyz pos_delta = actor_p->current.pos - actor->current.pos;
            cLib_addCalcAngleS2(&actor->current.angle.y, cM_atan2s(pos_delta.x,pos_delta.z), 2, 0x800);
        }

        if (unk_flag <= 2) {
            if (i_this->unkFlag4 == 5) {
                i_this->unkFlag4 = 0;
                i_this->unkTimer1 = 47;
            }

            if (i_this->unkTimer1 == 35) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_GLAD_e, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 20;
            }

            if (i_this->unkTimer1 == 1) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_WAIT_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }

            if (i_this->unkFlag4 == 2) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_CATCH_e, 3.0f, J3DFrameCtrl::EMode_NONE, 0.0f);
                i_this->unkFlag4 = 0;
            }

            if (i_this->unkFlag4 == 3) {
                i_this->unkFlag4 = 0;
                i_this->boomerangStatus = 1;
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_CATCH_e, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 15;

                i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_CATCH_BOOM, -1);
                i_this->sound.startCreatureSound(Z2SE_EN_MK_CATCH_BOOM, 0, -1);

#if VERSION == VERSION_GCN_JPN
                return;
#endif
            }
        }

        if ((hasira != NULL) && hasira->checkRollAttack()) {
            i_this->action = e_mk_class::ACT_YORO;
            i_this->mode = 0;
            if (i_this->boomerangStatus != 0) {
                i_this->timer[0] = 40;
            } else {
                i_this->timer[0] = 250;
            }
        }
    }
}

static s8 lbl_210_bss_130;

static void e_mk_yoro(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;

    switch (i_this->mode) {
        case 0:
            i_this->sound.startCreatureVoiceLevel(Z2SE_EN_MK_V_LOOSE_BALANCE, -1);
            anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_BALANCE_START_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 1;
            break;

        case 1:
            i_this->sound.startCreatureVoiceLevel(Z2SE_EN_MK_V_LOOSE_BALANCE, -1);
            if (i_this->anmP->isStop()) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_BALANCE_e, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 2;
            }
            break;

        case 2:
            i_this->sound.startCreatureVoiceLevel(Z2SE_EN_MK_V_LOOSE_BALANCE, -1);
            if (i_this->timer[0] == 0) {
                i_this->action = e_mk_class::ACT_WAIT;
                i_this->mode = 0;
                i_this->unkFlag4 = 6;
                i_this->boomerangStatus = 1;
            }

    }

    if (i_this->unkFlag4 == 4) {
        i_this->unkFlag4 = 0;
        i_this->action = e_mk_class::ACT_DROP;
        i_this->mode = 0;

        i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_HIT_BOOM, -1);
        i_this->sound.startCollisionSE(Z2SE_HIT_WOOD_WEAPON, 31);

        i_this->invulnerabilityTimer = 5;
        i_this->tubaTimer = 5;
        if (lbl_210_bss_130 == 0) {
            lbl_210_bss_130 = 1;
            i_this->demoMode = e_mk_class::DEMO_MODE_BOHIT;
            i_this->demoSubMode = 1;
        }
    }
}

static void e_mk_drop(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz work;
    int frame = i_this->anmP->getFrame();

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_FALL_e, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 1;
            cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
            work.x = 0.0;
            work.y = KREG_F(8) + 30.0f;
            work.z = KREG_F(9) + -10.0f;
            MtxPosition(&work, &actor->speed);
            i_this->invulnerabilityTimer = 50;
            break;

        case 1:
            if (frame >= TREG_S(3) + 14) {
                i_this->anmP->setPlaySpeed(0.0f);
                i_this->mode = 2;
            }
            break;

        case 2:
            i_this->invulnerabilityTimer = 5;
            if (i_this->acch.ChkGroundHit()) {
                i_this->anmP->setPlaySpeed(1.0f);
                i_this->mode = 3;
                dComIfGp_getVibration().StartShock((int)YREG_S(2) + VIBMODE_S_POWER5, 15, cXyz(0.0f, 1.0f, 0.0f));
                i_this->sound.startCreatureSound(Z2SE_EN_MK_FALL_GROUND, 0, -1);
                actor->speed.x *= TREG_F(9) + 1.0f;
                actor->speed.z *= TREG_F(9) + 1.0f;
                i_this->setSmokeFlag = 1;
                i_this->unkCounter2 = 0;
                Z2GetAudioMgr()->changeSubBgmStatus(1);
                dComIfGs_onOneZoneSwitch(5, -1);
            }
            break;
            
        case 3:
            if (frame == 22 || frame == 32) {
                i_this->setSmokeFlag = 1;
            }

            if (frame >= 45) {
                actor->speed.set(0.0f, 0.0f, 0.0f);
            }

            if (i_this->anmP->isStop()) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_CHANCE_e, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 4;
                i_this->timer[0] = 150;
            }
            break;
        
        case 4:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_STANDUP_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 5;
                i_this->unkCounter2 = 4;
                i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_AWAKE, -1);
            }
            break;

        case 5:
            if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_SPRING_e && ((frame == 9 || (frame == 24)) || (frame == 35))) {
                i_this->setSmokeFlag = 2;
            }

            if (i_this->anmP->isStop()) {
                i_this->action = e_mk_class::ACT_MOVE;
                i_this->mode = 2;
                i_this->posTargetStep.set(0.0f, 0.0f, 0.0f);
                i_this->unkCounter1 = cM_rndF(3.0f) + 2.0f;
                i_this->unkFlag5 = 1;
                if (actor->health > 100) {
                    Z2GetAudioMgr()->changeSubBgmStatus(2);
                } else {
                    Z2GetAudioMgr()->changeSubBgmStatus(3);
                }
            }
    }
}

static void e_mk_damage(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;

    switch (i_this->mode) {
        case 0:
            i_this->unkCounter2++;
            actor->speed.set(0.0f, 0.0f, 0.0f);
            if (i_this->unkCounter2 >= 4) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_SPRING_e, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->action = e_mk_class::ACT_DROP;
                i_this->mode = 5;
                i_this->invulnerabilityTimer = 60;
            } else {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DAMAGE_e, 3.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 1;
                i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_DAMAGE, -1);
            }
            break;

        case 1:
            if (i_this->anmP->isStop()) {
                anm_init(i_this,dRes_INDEX_E_MK_BCK_MK_CHANCE_e, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->action = e_mk_class::ACT_DROP;
                i_this->mode = 4;
            }
            break;
    }
}

static void damage_check(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (i_this->invulnerabilityTimer != 0) {
        return;
    }

    i_this->stts.Move();
    dCcU_AtInfo unused;

    if (i_this->action >= 9) {
        if (i_this->tgSph.ChkTgHit()) {
            i_this->atInfo.mpCollider = i_this->tgSph.GetTgHitObj();
            cc_at_check(actor, &i_this->atInfo);
            if (i_this->atInfo.mpCollider->ChkAtType(AT_TYPE_UNK) != 0) {
                i_this->invulnerabilityTimer = 20;
            } else {
                i_this->invulnerabilityTimer = 10;
            }

            if (actor->health <= 0) {
                fpcM_Search(s_obj_delete, i_this);
                i_this->action = 30;
                i_this->mode = 0;
                i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_DEAD, -1);
                i_this->invulnerabilityTimer = 20000;
                Z2GetAudioMgr()->subBgmStop();

#if PLATFORM_GCN
                dScnPly_c::nextPauseTimer = 0;
#endif
            } else {
                i_this->action = 10;
                i_this->mode = 0;
                if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP &&
                    player->checkCutJumpCancelTurn())
                {
                    i_this->invulnerabilityTimer = 3;
                }
            }
        }
    }

    if (actor->health <= 1) {
        actor->health = 0;
        i_this->tgSph.SetTgHitMark((CcG_Tg_HitMark)3);
    }
}

static void e_mk_s_demo(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    int frame = i_this->anmP->getFrame();
    i_this->invulnerabilityTimer = 5;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_WAIT_e, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mode = 1;
            i_this->timer[0] = (s16)TREG_S(9);
            // fallthrough
        case 1:
            if (i_this->timer[0] == 0) {
                i_this->mode = 100;
                i_this->demoMode = e_mk_class::DEMO_MODE_START;
                i_this->demoSubMode = 1;
            }
            break;

        case 2:
            i_this->sound.startCreatureSound(Z2SE_MK_DARK_BOOM_RESUME, 0, -1);
            anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_THROW_RAISE_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 3;
            break;

        case 3:
            if (i_this->anmP->isStop()) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_THROW_WAIT_e, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 100;
            }
            break;

        case 4:
            anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_THROW_e, 1.0f, J3DFrameCtrl::EMode_NONE ,1.0f);
            i_this->mode = 5;
            i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_THROW_BOOM, -1);
            break;

        case 5:
            if (frame == hREG_S(3) + 8) {
                i_this->unkFlag4 = 1;
                Z2GetAudioMgr()->bgmStreamPlay();
            }

            if (frame == hREG_S(4) + 8) {
                i_this->boomerangStatus = 0;
            }
    
            if (frame == TREG_S(5) + 14) {
                i_this->mode = 6;
                i_this->demoSubMode = 7;
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_WAIT_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;

        case 6:
            if (i_this->unkFlag4 == 2) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_CATCH_e, 3.0f, J3DFrameCtrl::EMode_NONE, 0.0f);
                i_this->unkFlag4 = 0;
            }

            if (i_this->unkFlag4 == 3) {
                i_this->unkFlag4 = 0;
                i_this->boomerangStatus = 1;
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_CATCH_e, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_CATCH_BOOM, -1);
                i_this->sound.startCreatureSound(Z2SE_EN_MK_CATCH_BOOM, 0, -1);
                i_this->mode = 7;
            }
            break;

        case 7:
            if (i_this->anmP->isStop()) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_GLAD_e, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->timer[0] = 30;
                i_this->mode = 8;
            }
            break;

        case 8:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_PROVOCATION_03_e, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 9;
            }
            break;

        case 9:
            if (i_this->anmP->isStop()) {
                i_this->action = e_mk_class::ACT_MOVE;
                i_this->unkCounter1 = cM_rndF(2.0f) + 7.0f;
                i_this->mode = 1;
                i_this->demoSubMode = 11;
                i_this->demoCamCounter = 0;
            }
            break;

        case 100:
            break;
    }
}

static s8 e_mk_e_demo(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    f32 sqrt_val;
    cXyz work, offset;
    int frame = i_this->anmP->getFrame();
    s8 rv = 1;
    s8 unk_flag = 0;

    switch (i_this->mode) {
        case -100:
            anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_CHANCE_e, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mode++;
            break;

        case 0:
            anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_START_SPRING_e, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->mode = 1;
            i_this->demoMode = e_mk_class::DEMO_MODE_END;
            i_this->demoSubMode = 1;
            dComIfGs_onSwitch(108, fopAcM_GetRoomNo(actor));
            break;

        case 1:
            if (i_this->anmP->isStop()) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_SPRING_e, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 2;
                actor->current.angle.y = STAGE_ANGLE_Y + -0x4000;
                cMtx_YrotS(*calc_mtx, actor->current.angle.y);
                work.x = 0.0;
                work.y = 0.0;
                work.z = AREG_F(12) + -1000.0f;
                MtxPosition(&work, &offset);
                actor->current.pos = STAGE_CENTER_POS + offset;
                actor->old.pos = STAGE_CENTER_POS + offset;
                i_this->demoSubMode = 4;
                i_this->demoCamCounter = 0;
                actor->gravity = AREG_F(15) + -10.0f;
            }
            break;

        case 2:
            if (i_this->acch.ChkGroundHit()) {
                actor->speedF = 0.0f;
            }
    
            if (frame == AREG_S(1) + 1 || frame == AREG_S(2) + 18) {
                actor->speed.y = AREG_F(14) + 50.0f;
                actor->speedF = AREG_F(13) + 30.0f;
                i_this->sound.startCreatureSound(Z2SE_EN_MK_JUMP_START, 0, -1);
            }
    
            work = STAGE_CENTER_POS - actor->current.pos;
            sqrt_val = JMAFastSqrt(work.x * work.x + work.z * work.z);
            if (sqrt_val < AREG_F(16) + 200.0f) {
                rv = 2;
                if (sqrt_val <AREG_F(17) + 100.0f) {
                    i_this->mode = 3;
                    anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_HIT_e, 2.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                    actor->speedF = 0.0f;
                    actor->gravity = 0.0f;
                    actor->speed.y = 0.0f;
                    i_this->timer[0] = 40;
                    unk_flag = 60;
                    target_info_count = 0;
                    fpcM_Search(s_h_sub, i_this);
                    daPillar_c* pillar = static_cast<daPillar_c*>(target_info[8]);
                    pillar->setShake(1);
                }
            }
            break;

        case 3:
            rv = 2;
            i_this->field_0x5d4 = 0;
            if (i_this->timer[0] < 12) {
              i_this->btpFrame = i_this->timer[0] << 1;
              if (i_this->btpFrame >= 12.0f) {
                i_this->btpFrame = i_this->btpFrame - 12.0f;
              }
            }
            
            if (i_this->timer[0] == 0) {
              actor->gravity = AREG_F(18) + -0.03f;
              if (actor->speed.y < AREG_F(19) + -1.0f) {
                  anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_FALL_e, WREG_F(2) + 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                  actor->speedF = WREG_F(0);
                  actor->gravity = WREG_F(1) + -2.0f;
                  i_this->mode = 4;
              }
            }
            break;

        case 4:
            if (i_this->acch.ChkGroundHit()) {
                actor->speedF = 0.0f;
                i_this->setSmokeFlag = 2;
                unk_flag = 30;
                i_this->mode = 100;
                i_this->unkFlag1 = 1;
                ok_anm_init(i_this, dRes_INDEX_E_MK_BCK_OK_DEMO_DROP_e, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                cMtx_YrotS(*calc_mtx, actor->current.angle.y);
                work.x = 0.0f;
                work.y = 0.0f;
                work.z = WREG_F(6) + -50.0f;
                MtxPosition(&work, &offset);
                i_this->crownPos += offset;
                i_this->unkRotation = actor->shape_angle;
                i_this->unkRotation.x = 0x4000; // 90°
                i_this->field_0x5fc = WREG_F(8) + 30.0f;
                i_this->field_0x600 = WREG_F(9) + -20.0f;
                i_this->demoSubMode = 5;
                i_this->demoCamCounter = 0;
                i_this->btpFrameFlag = 1;
            }
            break;

        case 5:
            anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_WAKEUP_e, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
            i_this->field_0x5d4 = 60;
            i_this->mode++;
            break;

        case 6:
            if (i_this->anmP->isStop()) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_ESCAPE_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode++;
                i_this->field_0x5d4 = 125;
                i_this->demoSubMode = 8;
                i_this->demoCamCounter = 0;
            }
            break;

        case 7:
            if (frame == 144) {
                i_this->unkFlag5 = 0;
                i_this->unkCounter1 = 100;
                i_this->posTargetStep.set(0.0f, 0.0f, 0.0f);
                actor->speed.set(0.0f, 0.0f, 0.0f);
                i_this->posTarget = STAGE_CENTER_POS;
                i_this->posTarget.y += 500.0f;
                work = i_this->posTarget - actor->current.pos;
                actor->current.angle.y = cM_atan2s(work.x, work.z);
                actor->current.angle.x = -cM_atan2s(work.y, JMAFastSqrt(work.x * work.x + work.z * work.z));
                cMtx_YrotS(*calc_mtx, actor->current.angle.y);
                cMtx_XrotM(*calc_mtx, actor->current.angle.x);
                work.x = 0.0;
                work.y = 0.0;
                work.z = TREG_F(10) + 70.0f;
                MtxPosition(&work, &actor->speed);
                i_this->unkFlag2 = 1;
                i_this->prevPos = actor->current.pos;
                i_this->prevPosTarget = i_this->posTarget;
                i_this->sound.startCreatureSound(Z2SE_EN_MK_JUMP_WIND, 0, -1);
                i_this->action = e_mk_class::ACT_MOVE;
                i_this->mode = 4;
                return 0;
            }
            break;

        case -99:
        case 100:
            break;
    }

    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    work.x = 0.0f;
    work.y = 0.0f;
    work.z = actor->speedF;
    MtxPosition(&work, &offset);
    actor->speed.x = offset.x;
    actor->speed.z = offset.z;
    if (unk_flag != 0) {
      dComIfGp_getVibration().StartShock((int)YREG_S(2) + VIBMODE_S_POWER5, 1, cXyz(0.0f, 1.0f, 0.0f));
      i_this->sound.startCreatureSound(Z2SE_EN_FM_FOOTNOTE, 0, -1);
      i_this->field_0xc98 = unk_flag;
    }
    
    return rv;
}

/* Acotr logic for the cutscene in the Forest Temple when Ook cuts down the bridge with the boomerang */
static void e_mk_r04_demo(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz work, offset;
    int frame = i_this->anmP->getFrame();
    f32 target_speedF = 0.0f;
    i_this->invulnerabilityTimer = 5;

    switch(i_this->mode) {
        case 0:
            anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_WAIT_e, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mode = 1;
            i_this->demoMode = e_mk_class::DEMO_MODE_R04;
            i_this->demoSubMode = 1;
            break;

        case 2:
            anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_RUN_e, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            i_this->mode = 3;
            break;

        case 3:
            target_speedF = KREG_F(8) + 30.0f;
            if (actor->current.pos.z > -600.0f) {
                i_this->mode = 4;
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_WAIT_e, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->timer[0] = 0;
            }
            break;

        case 4:
            if (i_this->timer[0] == 0) {
                i_this->sound.startCreatureSound(Z2SE_MK_DARK_BOOM_RESUME, 0, -1);
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_THROW_RAISE_e, 5.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 5;
            }
            break;

        case 5:
            if (i_this->anmP->isStop()) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_THROW_WAIT_e, 2.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 6;
                i_this->timer[0] = VREG_S(7) + 95;
            }
            break;

        case 6:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_THROW_e, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 7;
                i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_THROW_BOOM, -1);
                i_this->unkFlag4 = 7;
            }
            break;

        case 7:
            if (frame == hREG_S(3) + 8) {
                i_this->unkFlag4 = 1;
            }

            if (frame == hREG_S(4) + 8) {
                i_this->boomerangStatus = 0;
            }

            if (frame == hREG_S(5) + 14) {
                i_this->mode = 8;
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_WAIT_e, 10.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
            }
            break;

        case 8:
            if (i_this->unkFlag4 == 2) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_CATCH_e, 3.0f, J3DFrameCtrl::EMode_NONE, 0.0f);
                i_this->unkFlag4 = 0;
            }

            if (i_this->unkFlag4 == 3) {
                i_this->unkFlag4 = 0;
                i_this->boomerangStatus = 1;
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_CATCH_e, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 9;

                i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_CATCH_BOOM, -1);
                i_this->sound.startCreatureSound(Z2SE_EN_MK_CATCH_BOOM, 0, -1);
            }
            break;

        case 9:
            if (i_this->anmP->isStop()) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_DEMO_GLAD_e, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->timer[0] = 30;
                i_this->mode = 10;
            }
            break;

        case 10:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_PROVOCATION_03_e, 10.0f, J3DFrameCtrl::EMode_NONE, 1.0f);
                i_this->mode = 11;
            }
            break;

        case 11:
            if (i_this->anmP->isStop()) {
                anm_init(i_this, dRes_INDEX_E_MK_BCK_MK_RUN_e, 3.0f, J3DFrameCtrl::EMode_LOOP, 1.0f);
                i_this->mode = 12;
                i_this->timer[0] = 10;
            }
            break;

        case 12:
            if (i_this->timer[0] == 0) {
                target_speedF = KREG_F(8) + 30.0f;
            }

            cLib_addCalcAngleS2(&actor->current.angle.y, actor->home.angle.y + (u16)-0x8000, 2, 0x1000);
            break;
    }

    cLib_addCalc2(&actor->speedF, target_speedF, 1.0f, 5.0f);
    actor->gravity = AREG_F(15) + -10.0f;
    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    work.x = 0.0f;
    work.y = 0.0f;
    work.z = actor->speedF;
    MtxPosition(&work, &offset);
    actor->speed.x = offset.x;
    actor->speed.z = offset.z;
    actor->current.pos += actor->speed;
    actor->speed.y += actor->gravity;
}

static void cam_3d_morf(e_mk_class* i_this, f32 i_scale) {
    cLib_addCalc2(&i_this->camCenter.x, i_this->camCenterTarget.x, i_scale, i_this->distToCamCenterTarget.x * i_this->camStepScale1);
    cLib_addCalc2(&i_this->camCenter.y, i_this->camCenterTarget.y, i_scale, i_this->distToCamCenterTarget.y * i_this->camStepScale1);
    cLib_addCalc2(&i_this->camCenter.z, i_this->camCenterTarget.z, i_scale, i_this->distToCamCenterTarget.z * i_this->camStepScale1);
    cLib_addCalc2(&i_this->camEye.x, i_this->camEyeTarget.x, i_scale, i_this->distToCamEyeTarget.x * i_this->camStepScale1);
    cLib_addCalc2(&i_this->camEye.y, i_this->camEyeTarget.y, i_scale, i_this->distToCamEyeTarget.y * i_this->camStepScale1);
    cLib_addCalc2(&i_this->camEye.z, i_this->camEyeTarget.z, i_scale, i_this->distToCamEyeTarget.z * i_this->camStepScale1);
}

/* Camera logic for cutscene at start of Ook battle */
static void demo_camera_start(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* deku_baba_p[2];
    fopAc_ac_c* boomerang_p;
    boomerang_p = fopAcM_SearchByID(i_this->boomerangId);
    camera_class* camera1 = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera2 = (camera_class*)dComIfGp_getCamera(0);
    cXyz work, offset, offset_2, unused;

    switch (i_this->demoSubMode) {
        case 0:
            break;

        case 1:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera1->mCamera.Stop();
            i_this->demoSubMode = 2;
            i_this->demoCamCounter = 0;
            i_this->demoCamFovy = 50.0f + NREG_F(10);
            camera1->mCamera.SetTrimSize(3);
            i_this->field_0xc90 = 2500.0f + NREG_F(1);

            daPy_getPlayerActorClass()->changeOriginalDemo();
            Z2GetAudioMgr()->bgmStreamPrepare(0x200000D);

            i_this->camEye = camera2->lookat.eye;
            i_this->camCenter = camera2->lookat.center;
            i_this->camEyeTarget.set(-271.0f, 4559.0f, -7241.0f);
            i_this->camCenterTarget.set(-70.0f, 4378.0f, -6233.0f);

            i_this->distToCamEyeTarget.x = fabsf(i_this->camEyeTarget.x - i_this->camEye.x);
            i_this->distToCamEyeTarget.y = fabsf(i_this->camEyeTarget.y - i_this->camEye.y);
            i_this->distToCamEyeTarget.z = fabsf(i_this->camEyeTarget.z - i_this->camEye.z);
            i_this->distToCamCenterTarget.x = fabsf(i_this->camCenterTarget.x - i_this->camCenter.x);
            i_this->distToCamCenterTarget.y = fabsf(i_this->camCenterTarget.y - i_this->camCenter.y);
            i_this->distToCamCenterTarget.z = fabsf(i_this->camCenterTarget.z - i_this->camCenter.z);

            dComIfGp_getEvent()->startCheckSkipEdge(actor);
            // fallthrough
        case 2:
            if (i_this->demoCamCounter == (s16)(8 + VREG_S(0))) {
                daPy_getPlayerActorClass()->changeDemoMode(25, 0, 0, 0);
            }

            if (i_this->demoCamCounter >= (s16)(35 + VREG_S(1))) {
                cam_3d_morf(i_this, 0.1f);
                cLib_addCalc2(&i_this->camStepScale1, 0.03f + VREG_F(1), 1.0f, 0.0005f + VREG_F(2));
            }

            if (i_this->demoCamCounter >= (s16)(160 + VREG_S(3))) {
                i_this->demoCamCounter = 0;
                i_this->demoSubMode = 3;
            }
            break;

        case 3:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            work.x = 0.0f;
            work.y = 150.0f + NREG_F(6);
            work.z = 250.0f + NREG_F(7);
            MtxPosition(&work, &offset);
            i_this->camEye = actor->current.pos + offset;
            i_this->camCenter = actor->current.pos;
            i_this->camCenter.y += 150.0f + NREG_F(8);
            if (i_this->demoCamCounter >= 60) {
                i_this->demoCamCounter = 0;
                i_this->demoSubMode = 4;
                i_this->mode = 2;
            }
            break;

        case 4:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            work.x = 0.0f;
            work.y = 150.0f + NREG_F(9);
            work.z = 700.0f + NREG_F(10);
            MtxPosition(&work, &offset);
            i_this->camEye = actor->current.pos + offset;
            i_this->camCenter = actor->current.pos;
            i_this->camCenter.y += 230.0f + NREG_F(11);

            if (i_this->demoCamCounter == 35) {
                daPy_getPlayerActorClass()->changeDemoMode(12, 0, 1, 0);
            }

            if (i_this->demoCamCounter >= (s16)(95 + XREG_S(0))) {
                i_this->demoCamCounter = 0;
                i_this->demoSubMode = 5;
                i_this->field_0xc9c = 1100.0f;
                i_this->camStepScale1 = 0.0f;
                i_this->field_0xc90 = 2500.0f + NREG_F(12);
                cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
                cMtx_XrotM(*calc_mtx, 1700 + NREG_S(2));
                work.x = 0.0f;
                work.y = 0.0f;
                work.z = i_this->field_0xc90;
                MtxPosition(&work, &offset_2);
                offset_2 += actor->current.pos;
                cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
                work.x = NREG_F(2) - 300.0f;
                work.y = 0.0f;
                work.z = 0.0f;
                MtxPosition(&work, &offset);
                offset_2 += offset;
                work.x = 100.0f + NREG_F(13);
                work.y = 0.0f;
                work.z = 200.0f + NREG_F(14);
                MtxPosition(&work, &offset);
                offset += offset_2;
                offset.y = player->current.pos.y;
                player->setPlayerPosAndAngle(&offset, actor->current.angle.y + (u16)-0x8000, 0);
            }
            break;

        case 5:
            i_this->field_0xc90 = 2500.0f + NREG_F(12) - 50.0f;
            i_this->field_0xc9c = -700.0f + TREG_F(14) + 200.0f + 1000.0f;
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            cMtx_XrotM(*calc_mtx, i_this->field_0xc9c);
            work.x = 0.0f;
            work.y = 0.0f;
            work.z = 3500.0f + NREG_F(0);
            MtxPosition(&work, &i_this->camCenter);
            i_this->camCenter += actor->current.pos;
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            cMtx_XrotM(*calc_mtx, 2000 + NREG_S(2));
            work.x = 0.0f;
            work.y = 0.0f;
            work.z = i_this->field_0xc90;
            MtxPosition(&work, &i_this->camEye);
            i_this->camEye += actor->current.pos;
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            work.x = NREG_F(2) - 300.0f;
            work.y = 0.0f;
            work.z = 0.0f;
            MtxPosition(&work, &offset);
            i_this->camEye += offset;

            if (i_this->demoCamCounter == 5) {
                daPy_getPlayerActorClass()->changeDemoMode(23, 1, 2, 0);
            }

            if (i_this->demoCamCounter >= 50) {
                i_this->demoCamCounter = 0;
                i_this->demoSubMode = 6;
            }
            break;

        case 6:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            work.x = 0.0f;
            work.y = 150.0f + NREG_F(9);
            work.z = 700.0f + NREG_F(10);
            MtxPosition(&work, &offset);
            i_this->camEye = actor->current.pos + offset;
            i_this->camCenter = actor->current.pos;
            i_this->camCenter.y += 230.0f + NREG_F(11);

            if (i_this->demoCamCounter == 2) {
                i_this->unkFlag4 = 7;
            }

            if (i_this->demoCamCounter >= 10) {
                i_this->mode = 4;
                i_this->demoSubMode = 100;
                work.x = 200.0f + JREG_F(14);
                work.y = 0.0f;
                work.z = -150.0f + JREG_F(15);
                MtxPosition(&work, &offset);
                offset += player->current.pos;
                player->setPlayerPosAndAngle(&offset, actor->current.angle.y + (u16)-0x8000, 0);
            }
            break;

        case 7:
            if (boomerang_p == NULL) {
                break;
            }

            i_this->camCenter = boomerang_p->current.pos;
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            work.x = 300.0f + NREG_F(16);
            work.y = -200.0f + NREG_F(17);
            work.z = 1000.0f + NREG_F(18);
            MtxPosition(&work, &offset);
            i_this->camEye = actor->current.pos + offset;
            i_this->demoSubMode = 8;
            i_this->demoCamCounter = 0;
            // fallthrough
        case 8:
            cLib_addCalc2(&i_this->camCenter.x, boomerang_p->current.pos.x, 0.2f, 100.0f);
            cLib_addCalc2(&i_this->camCenter.y, boomerang_p->current.pos.y + i_this->field_0xc8c, 0.2f, 100.0f);
            cLib_addCalc2(&i_this->camCenter.z, boomerang_p->current.pos.z, 0.2f, 100.0f);
            cLib_addCalc2(&i_this->field_0xc8c, -300.0f + JREG_F(7), 1.0f, 5.0f + JREG_F(8));

            i_this->camEye += boomerang_p->speed * (0.45f + TREG_F(19));
            i_this->camEye.y -= 3.0f + JREG_F(3);
            if (i_this->demoCamCounter < 65) {
                break;
            }

            i_this->demoSubMode = 9;
            i_this->demoCamCounter = 0;
            // fallthrough
        case 9:
            deku_baba_p[0] = fopAcM_SearchByID(i_this->dbId1);
            deku_baba_p[1] = fopAcM_SearchByID(i_this->dbId2);
            if (i_this->demoCamCounter == 0) {
                deku_baba_p[0]->current.pos.x = player->current.pos.x - 200.0f;
                deku_baba_p[0]->current.pos.y = player->current.pos.y + 500.0f;
                deku_baba_p[0]->current.pos.z = player->current.pos.z - 1200.0f;
                deku_baba_p[0]->speed.y = 0.0f;
                deku_baba_p[1]->current.pos.x = player->current.pos.x + 200.0f;
                deku_baba_p[1]->current.pos.y = player->current.pos.y + 1000.0f;
                deku_baba_p[1]->current.pos.z = player->current.pos.z - 1000.0f;
                deku_baba_p[1]->speed.y = 0.0f;
                i_this->camCenter.x = deku_baba_p[0]->current.pos.x;
                i_this->camCenter.z = deku_baba_p[0]->current.pos.z;
            }

            i_this->camEye.x = player->current.pos.x + 100.0f + ZREG_F(0);
            i_this->camEye.y = player->current.pos.y + 100.0f + ZREG_F(1);
            i_this->camEye.z = player->current.pos.z - 400.0f + ZREG_F(2);
            i_this->camCenter.y = deku_baba_p[0]->current.pos.y + ZREG_F(3);

            cLib_addCalc2(&i_this->camCenter.x, deku_baba_p[0]->current.pos.x + 100.0f, 0.1f, 50.0f);
            cLib_addCalc2(&i_this->camCenter.z, deku_baba_p[0]->current.pos.z, 0.1f, 50.0f);

            if (i_this->demoCamCounter <= (s16)(60 + AREG_S(0))) {
                boomerang_p->current.pos -= boomerang_p->speed;
            }

            if (i_this->demoCamCounter >= (s16)(80 + AREG_S(0))) {
                i_this->demoSubMode = 10;
                i_this->demoCamCounter = 0;
            }
            break;

        case 10:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            work.x = 0.0f;
            work.y = 150.0f + NREG_F(9);
            work.z = 700.0f + NREG_F(10);
            MtxPosition(&work, &offset);
            i_this->camEye = actor->current.pos + offset;
            i_this->camCenter = actor->current.pos;
            i_this->camCenter.y += 230.0f + NREG_F(11);

            deku_baba_p[0] = fopAcM_SearchByID(i_this->dbId1);
            deku_baba_p[1] = fopAcM_SearchByID(i_this->dbId2);
            deku_baba_p[0]->current.pos.x = player->current.pos.x - 200.0f;
            deku_baba_p[0]->current.pos.z = player->current.pos.z - 1200.0f;
            deku_baba_p[1]->current.pos.x = player->current.pos.x + 200.0f;
            deku_baba_p[1]->current.pos.z = player->current.pos.z - 1000.0f;
            break;

        case 11:
            if (i_this->demoCamCounter >= (s16)(30 + JREG_S(9))) {
                cMtx_YrotS(*calc_mtx, player->shape_angle.y);
                work.x = 0.0f;
                work.y = 200.0f;
                work.z = -400.0f;
                MtxPosition(&work, &i_this->camEye);
                i_this->camEye += player->current.pos;
                i_this->camCenter = player->current.pos;
                i_this->camCenter.y += 100.0f;
                i_this->demoMode = e_mk_class::DEMO_MODE_FINISH;
                fopAcM_OffStatus(actor, 0x4000);
                Z2GetAudioMgr()->subBgmStart(Z2BGM_BOOMERAMG_MONKEY);
            }

            deku_baba_p[0] = fopAcM_SearchByID(i_this->dbId1);
            deku_baba_p[1] = fopAcM_SearchByID(i_this->dbId2);
            deku_baba_p[0]->current.pos.x = player->current.pos.x - 200.0f;
            deku_baba_p[0]->current.pos.z = player->current.pos.z - 1200.0f;
            deku_baba_p[1]->current.pos.x = player->current.pos.x + 200.0f;
            deku_baba_p[1]->current.pos.z = player->current.pos.z - 1000.0f;
            break;

        case 100:
            break;
    }

    if (dComIfGp_getEvent()->checkSkipEdge()) {
        u8 sw_bit = (fopAcM_GetParam(actor) & 0xff0000) >> 16;
        if (sw_bit != 0xff) {
            dComIfGs_onSwitch(sw_bit, fopAcM_GetRoomNo(actor));
        }

        dStage_changeScene(2, 0.0f, 0, fopAcM_GetRoomNo(actor), 0, -1);
    }
}

static void demo_camera_end(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* boomerang_p = (fopAc_ac_c*)fopAcM_SearchByID(i_this->boomerangId);
    camera_class* camera = (camera_class*)dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz work, pos, unused_1, unused_2;

    switch (i_this->demoSubMode) {
        case 1:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xffff, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                break;
            }

            camera->mCamera.Stop();
            i_this->demoSubMode = 2;
            i_this->demoCamCounter = 0;
            i_this->demoCamFovy = NREG_F(10) + 55.0f;
            camera->mCamera.SetTrimSize(3);
            daPy_getPlayerActorClass()->changeOriginalDemo();
            boomerang_p->current.pos.y += 10000.0f;
            Z2GetAudioMgr()->bgmStreamPrepare(0x200000E);
            dComIfGs_onStageMiddleBoss();
            // fallthrough
        case 2:
            cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
            work.x = AREG_F(0);
            work.y = AREG_F(1) + 130.0f;
            work.z = AREG_F(2);
            MtxPosition(&work, &i_this->camCenter);
            i_this->camCenter += actor->current.pos;
            work.x = AREG_F(3) + 400.0f;
            work.y = AREG_F(4) + 100.0f;
            work.z = AREG_F(5) + 200.0f;
            MtxPosition(&work, &i_this->camEye);
            i_this->camEye += actor->current.pos;
            work.x = AREG_F(6);
            work.y = AREG_F(7);
            work.z = AREG_F(8) + -300.0f;
            MtxPosition(&work, &pos);
            pos += actor->current.pos;
            player->setPlayerPosAndAngle(&pos, (s16)actor->shape_angle.y, 0);

            if (i_this->demoCamCounter >= (s16)(AREG_S(0) + 30)) {
                i_this->demoSubMode++;
                i_this->demoCamCounter = 0;
            }
            break;

        case 3:
            cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
            work.x = AREG_F(0) + -300.0f;
            work.y = AREG_F(1);
            work.z = AREG_F(2) + -150.0f;
            MtxPosition(&work, &i_this->camCenter);
            i_this->camCenter += actor->current.pos;
            work.x = AREG_F(3) + 100.0f;
            work.y = AREG_F(4) + 1000.0f;
            work.z = AREG_F(5);
            MtxPosition(&work, &i_this->camEye);
            i_this->camEye += actor->current.pos;
            break;

        case 4:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            work.x = AREG_F(6) + 250.0f;
            work.y = AREG_F(7);
            work.z = AREG_F(8) + -200.0f;
            MtxPosition(&work, &i_this->camCenter);
            i_this->camCenter += STAGE_CENTER_POS;
            work.x = AREG_F(9) + 250.0f;
            work.y = AREG_F(10) + 700.0f;
            work.z = AREG_F(11) + 300.0f;
            MtxPosition(&work, &i_this->camEye);
            i_this->camEye += STAGE_CENTER_POS;
            break;

        case 5:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            work.x = AREG_F(6) + 450.0f;
            work.y = AREG_F(7);
            work.z = AREG_F(8) + -100.0f;
            MtxPosition(&work, &i_this->camCenter);
            i_this->camCenter += STAGE_CENTER_POS;
            work.x = AREG_F(9) + 250.0f;
            work.y = AREG_F(10) + 300.0f;
            work.z = AREG_F(11) + 300.0f;
            MtxPosition(&work, &i_this->camEye);
            i_this->camEye += STAGE_CENTER_POS;
            work.x = AREG_F(9) + 800.0f;
            work.y = AREG_F(10) + 100.0f;
            work.z = AREG_F(11) + 100.0f;
            MtxPosition(&work, &pos);
            pos += STAGE_CENTER_POS;
            i_this->distToCamEyeTarget.x = fabsf(pos.x - i_this->camEye.x) * 0.02f;
            i_this->distToCamEyeTarget.y = fabsf(pos.y - i_this->camEye.y) * 0.02f;
            i_this->distToCamEyeTarget.z = fabsf(pos.z - i_this->camEye.z) * 0.02f;
            i_this->demoSubMode = 6;
            i_this->camStepScale1 = 0;
            work.x = WREG_F(12) + 1400.0f;
            work.y = WREG_F(13);
            work.z = WREG_F(14) + 500.0f;
            MtxPosition(&work, &pos);
            pos += STAGE_CENTER_POS;
            player->setPlayerPosAndAngle(&pos, (s16)actor->shape_angle.y, 0);
            // fallthrough
        case 6:
            cLib_addCalc2(&i_this->camCenter.x, i_this->crownPos.x, 0.1f, i_this->camStepScale1 * 20.0f);
            cLib_addCalc2(&i_this->camCenter.y, i_this->crownPos.y + 30.0f + WREG_F(11), 0.1f, i_this->camStepScale1 * 20.0f);
            cLib_addCalc2(&i_this->camCenter.z, i_this->crownPos.z, 0.1f, i_this->camStepScale1 * 20.0f);
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            work.x = AREG_F(9) + 800.0f;
            work.y = AREG_F(10) + 100.0f;
            work.z = AREG_F(11) + 100.0f;
            MtxPosition(&work, &pos);

            pos += STAGE_CENTER_POS;
            cLib_addCalc2(&i_this->camStepScale1, 1.0f, 1.0f , NREG_F(4) + 0.1f);

            if (i_this->demoCamCounter >= 10) {
                cLib_addCalc2(&i_this->camEye.x, pos.x, 0.1f, i_this->distToCamEyeTarget.x * i_this->camStepScale2);
                cLib_addCalc2(&i_this->camEye.y, pos.y, 0.1f, i_this->distToCamEyeTarget.y * i_this->camStepScale2);
                cLib_addCalc2(&i_this->camEye.z, pos.z, 0.1f, i_this->distToCamEyeTarget.z * i_this->camStepScale2);
                cLib_addCalc2(&i_this->camStepScale2, 1.0f, 1.0f, NREG_F(4) + 0.1f);
            }
            
            if (i_this->demoCamCounter == 40) {
                Z2GetAudioMgr()->seStart(Z2SE_MK_DARK_INSECT_DEATH, &i_this->crownPos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }

            if (i_this->demoCamCounter >= 90) {
                if (i_this->demoCamCounter == 90) {
                    work.x = WREG_F(12) + 1300.0f;
                    work.y = WREG_F(13);
                    work.z = WREG_F(14);
                    MtxPosition(&work, &pos);
                    pos += STAGE_CENTER_POS;
                    player->setPlayerPosAndAngle(&pos, (s16)actor->shape_angle.y, 0);
                    player->changeDemoMode(3, 1, 0, 0);
                    work.x = WREG_F(15) + 170.0f;
                    work.y = 0.0f;
                    work.z = 0.0f;
                    MtxPosition(&work, &pos);
                    pos += i_this->crownPos;
                    player->changeDemoPos0(&pos);
                }

                if (i_this->demoCamCounter == 130) {
                    ok_anm_init(i_this, dRes_INDEX_E_MK_BCK_OK_DEMO_DIE_e, 1.0f,
                                J3DFrameCtrl::EMode_NONE, 1.0f);
                }

                if (i_this->demoCamCounter == 180) {
                    i_this->crownStatus = 0;
                    cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
                    work.x = 0.0f;
                    work.y = 0.0f;
                    work.z = AREG_F(2) + -20.0f;
                    MtxPosition(&work, &pos);
                    pos += i_this->crownPos;
                    fopAcM_createDisappear(actor, &pos, 5, 0, 0xff);
                }

                if (i_this->demoCamCounter == 248) {
                    i_this->demoSubMode++;
                    i_this->demoCamCounter = 0;
                    i_this->mode = 5;
                    cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
                    work.x = AREG_F(9) + 300.0f;
                    work.y = AREG_F(10);
                    work.z = AREG_F(11) + 300.0f;
                    MtxPosition(&work, &pos);
                    pos += STAGE_CENTER_POS;
                    actor->current.pos.x = pos.x;
                    actor->current.pos.z = pos.z;
                    work.x = AREG_F(3) + 50.0f + 80.0f;
                    work.y = AREG_F(4) + 120.0f - 20.0f;
                    work.z = AREG_F(5) + 100.0f;
                    MtxPosition(&work, &i_this->camEye);
                    i_this->camEye += STAGE_CENTER_POS;
                    i_this->camCenter = actor->current.pos;
                    i_this->camCenter.y += AREG_F(12) + 150.0f + 40.0f;
                    work.x = AREG_F(6) + 400.0f;
                    work.y = AREG_F(7);
                    work.z = AREG_F(8) + 700.0f;
                    MtxPosition(&work, &pos);
                    pos += STAGE_CENTER_POS;
                    work.x = STAGE_CENTER_POS.x - pos.x;
                    work.z = STAGE_CENTER_POS.z - pos.z;
                    player->setPlayerPosAndAngle(&pos, cM_atan2s(work.x, work.z), 0);
                    actor->current.angle.y = player->shape_angle.y;
                    Z2GetAudioMgr()->bgmStreamPlay();
                }
            }
            break;

        case 7:
            break;

        case 8:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            work.x = AREG_F(0) + 500.0f;
            work.y = AREG_F(1) + 300.0f;
            work.z = AREG_F(2);
            MtxPosition(&work, &i_this->camCenter);
            i_this->camCenter += STAGE_CENTER_POS;
            work.x = AREG_F(3) + 50.0f;
            work.y = AREG_F(4) + 70.0f;
            work.z = AREG_F(5) + 1100.0f;
            MtxPosition(&work, &i_this->camEye);
            i_this->camEye += STAGE_CENTER_POS;
            break;
        
        case 9:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            work.x = AREG_F(0) + 5500.0f;
            work.y = AREG_F(1) + 2300.0f;
            work.z = AREG_F(2) + -5000.0f;
            MtxPosition(&work, &i_this->camCenter);
            i_this->camCenter += STAGE_CENTER_POS;
            work.x = AREG_F(3) + 450.0f;
            work.y = AREG_F(4) + 370.0f;
            work.z = AREG_F(5) + -100.0f;
            MtxPosition(&work,&i_this->camEye);
            i_this->camEye += STAGE_CENTER_POS;
            if (i_this->demoCamCounter >= 300) {
                i_this->demoSubMode = 10;
                i_this->demoCamCounter = 0;
                work.x = AREG_F(16) + 400.0f;
                work.y = AREG_F(17);
                work.z = AREG_F(18) + 700.0f - 100.0f;
                MtxPosition(&work, &pos);
                pos += STAGE_CENTER_POS;
                work.x = STAGE_CENTER_POS.x - player->current.pos.x;
                work.z = STAGE_CENTER_POS.z - player->current.pos.z;
                player->setPlayerPosAndAngle(&pos, cM_atan2s(work.x, work.z), 0);
            }
            break;

        case 10:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            work.x = AREG_F(0);
            work.y = AREG_F(1) + 300.0f;
            work.z = AREG_F(2) - 100.0f;
            MtxPosition(&work, &i_this->camCenter);
            i_this->camCenter += STAGE_CENTER_POS;
            work.x = AREG_F(0) + 800.0f;
            work.y = AREG_F(1);
            work.z = AREG_F(2) + 600.0f - 100.0f;
            MtxPosition(&work, &pos);
            pos += STAGE_CENTER_POS;
            i_this->distToCamCenterTarget.x = fabsf(pos.x - i_this->camCenter.x) * 0.01f;
            i_this->distToCamCenterTarget.y = fabsf(pos.y - i_this->camCenter.y) * 0.01f;
            i_this->distToCamCenterTarget.z = fabsf(pos.z - i_this->camCenter.z) * 0.01f;
            work.x = AREG_F(3) + 450.0f;
            work.y = AREG_F(4) + 70.0f;
            work.z = AREG_F(5) + 1000.0f - 100.0f;
            MtxPosition(&work, &i_this->camEye);
            i_this->camEye += STAGE_CENTER_POS;
            work.x = AREG_F(3) + -150.0f;
            work.y = AREG_F(4) + 45.0f;
            work.z = AREG_F(5) + 800.0f - 100.0f;
            MtxPosition(&work, &pos);
            pos += STAGE_CENTER_POS;
            i_this->distToCamEyeTarget.x = fabsf(pos.x - i_this->camEye.x) * 0.01f;
            i_this->distToCamEyeTarget.y = fabsf(pos.y - i_this->camEye.y) * 0.01f;
            i_this->distToCamEyeTarget.z = fabsf(pos.z - i_this->camEye.z) * 0.01f;
            i_this->camStepScale1 = 0.0f;
            i_this->demoSubMode = 11;
            // fallthrough
        case 11:
            cMtx_YrotS(*calc_mtx, STAGE_ANGLE_Y);
            work.x = AREG_F(0) + 800.0f;
            work.y = AREG_F(1);
            work.z = AREG_F(2) + 600.0f - 100.0f;
            MtxPosition(&work, &pos);
            pos += STAGE_CENTER_POS;
            cLib_addCalc2(&i_this->camCenter.x, pos.x, 0.1f, i_this->distToCamCenterTarget.x * i_this->camStepScale1);
            cLib_addCalc2(&i_this->camCenter.y, pos.y, 0.1f, i_this->distToCamCenterTarget.y * i_this->camStepScale1);
            cLib_addCalc2(&i_this->camCenter.z, pos.z, 0.1f, i_this->distToCamCenterTarget.z * i_this->camStepScale1);
            work.x = AREG_F(3) + -150.0f;
            work.y = AREG_F(4) + 45.0f;
            work.z = AREG_F(5) + 800.0f - 100.0f;
            MtxPosition(&work, &pos);
            pos += STAGE_CENTER_POS;
            cLib_addCalc2(&i_this->camEye.x, pos.x, 0.1f, i_this->distToCamEyeTarget.x * i_this->camStepScale1);
            cLib_addCalc2(&i_this->camEye.y, pos.y, 0.1f, i_this->distToCamEyeTarget.y * i_this->camStepScale1);
            cLib_addCalc2(&i_this->camEye.z, pos.z, 0.1f, i_this->distToCamEyeTarget.z * i_this->camStepScale1);
            cLib_addCalc2(&i_this->camStepScale1, 0.5f, 1.0f, NREG_F(4) + 0.02f);

            if (i_this->demoCamCounter == 2) {
                fopAc_ac_c* boomerang_p = fopAcM_SearchByID(i_this->boomerangId);
                work.x = AREG_F(6) + 50.0f;
                work.y = AREG_F(7) + 10.0f;
                work.z = AREG_F(8) + 800.0f - 100.0f;
                MtxPosition(&work, &boomerang_p->current.pos);
                boomerang_p->current.pos += STAGE_CENTER_POS;
                boomerang_p->current.angle.y = 0;
                boomerang_p->shape_angle.y = STAGE_ANGLE_Y + YREG_S(8);
            }

            if (i_this->demoCamCounter == 200) {
                player->changeDemoMode(1, 3, 0, 0);
                player->changeDemoParam0(3);
                dComIfGp_getEvent()->setPtT(boomerang_p);
            }

            if (i_this->demoCamCounter == (s16)(KREG_S(9) + 250)) {                
                i_this->demoSubMode = 20;
                i_this->demoCamCounter = 0;
            }
            break;

        case 20:
            work = boomerang_p->current.pos - player->current.pos;
            if (i_this->demoCamCounter == 3) {
                player->changeDemoMode(3, 1, 0, 0);
                cMtx_YrotS(*calc_mtx, cM_atan2s(work.x, work.z));
                work.x = nREG_F(7) + -20.0f;
                work.y = 0.0f;
                work.z = ZREG_F(0) + 200.0f;
                MtxPosition(&work, &pos);
                pos += player->current.pos;
                player->changeDemoPos0(&pos);
                work.z = 10000.0f;
                MtxPosition(&work, &pos);
                actor->current.pos = pos;
            }

            pos = boomerang_p->current.pos - player->eyePos;
            actor->current.pos = player->eyePos + (pos * 20.0f);
            if (i_this->demoCamCounter == 20) {
                boomerang_p->health = 1;
                i_this->demoSubMode = 21;
                i_this->demoCamCounter = 0;
                i_this->camStepScale1 = 0.0f;
            }
            break;

        case 21:
            cLib_addCalc2(&i_this->camCenter.y, boomerang_p->current.pos.y + ZREG_F(4) - 20.0f, 0.2f, i_this->camStepScale1 * 200.0f);
            cLib_addCalc2(&i_this->camCenter.x, boomerang_p->current.pos.x, 0.4f, i_this->camStepScale1 * 200.0f);
            cLib_addCalc2(&i_this->camCenter.z, boomerang_p->current.pos.z, 0.4f, i_this->camStepScale1 * 200.0f);
            cLib_addCalc2(&i_this->camEye.y, player->current.pos.y + 110.0f + ZREG_F(5), 0.1f, i_this->camStepScale1 * 10.0f);
            cLib_addCalc2(&i_this->camStepScale1, 1.0f, 1.0f, 0.02f);
            pos = boomerang_p->current.pos - player->eyePos;
            actor->current.pos = player->eyePos + (pos * 20.0f);
            if (i_this->demoCamCounter >= 70) {
                if (i_this->demoCamCounter == 70) {
                    i_this->msgFlow.init(actor, 102, 0, NULL);
                }

                if (i_this->msgFlow.doFlow(actor, NULL, 0)) {
                    boomerang_p->health = 1;
                }
            }
            break;

        case 22:
            player->changeDemoMode(28, 0, 0, 0);
            i_this->demoSubMode++;
            i_this->demoCamCounter = 0;
            i_this->sound.startCreatureSound(Z2SE_EN_MK_CATCH_BOOM, 0, -1);
            // fallthrough
        case 23:
            cLib_addCalc2(&i_this->camCenter.x, player->current.pos.x, 0.4f, i_this->camStepScale1 * 200.0f);
            cLib_addCalc2(&i_this->camCenter.z, player->current.pos.z, 0.4f, i_this->camStepScale1 * 200.0f);

            if (i_this->demoCamCounter == 20) {
                fopAcM_createItemForMidBoss(&player->current.pos, 64, fopAcM_GetRoomNo(actor), 0, 0, 0, -1);
                i_this->demoMode = e_mk_class::DEMO_MODE_FINISH;
                actor->current.pos.y += 20000.0f;
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
    UNUSED(i_data);

    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KS) && static_cast<npc_ks_class*>(i_actor)->set_id == 0) {
        return i_actor;
    }

    return NULL;
}

static void* s_brg_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);

    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_BRG) {
        static_cast<obj_brg_class*>(i_actor)->field_0xaef4 = VREG_F(11) + 500.0f;
        static_cast<obj_brg_class*>(i_actor)->mType |= (u8)4;
    }

    return NULL;
}

static void* s_brg_sub2(void* i_actor, void* i_data) {
    UNUSED(i_data);

    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_BRG) {
#if PLATFORM_SHIELD
        static_cast<obj_brg_class*>(i_actor)->mType &= (u8)~4u;
#else
        static_cast<obj_brg_class*>(i_actor)->mType &= ~4u;
#endif
    }

    return NULL;
}

/* Camera logic for the cutscene in the Forest Temple when Ook cuts the bridge down with the boomerang */
static void demo_camera_r04(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c *)dComIfGp_getPlayer(0);
    fopAc_ac_c* boomerang_p = fopAcM_SearchByID(i_this->boomerangId);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz work, pos, unused, unused2;

    npc_ks_class* monkey_p = (npc_ks_class*)fpcM_Search(s_ks_sub, i_this);
    if (monkey_p == NULL) {
        return;
    }

    switch (i_this->demoSubMode) {
        case 1:
            if (!monkey_p->actor.eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(&monkey_p->actor, 2, 0xffff, 0);
                monkey_p->actor.eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                break;
            }

            camera->mCamera.Stop();
            i_this->demoSubMode = 2;
            i_this->demoCamCounter = 0;
            i_this->demoCamFovy = NREG_F(10) + 55.0f;
            camera->mCamera.SetTrimSize(3);

            daPy_getPlayerActorClass()->changeOriginalDemo();
            i_this->camCenter.set(-711.0f, 3588.0f, 2730.0f);
            i_this->camEye.set(116.0f, 3400.0f, 4597.0f);
            i_this->camCenterTarget.set(-291.0f, 3256.0f, 2640.0f);
            i_this->camEyeTarget.set(1060.0f, 3795.0f, 4086.0f);

            i_this->distToCamCenterTarget.x = fabsf(i_this->camCenterTarget.x - i_this->camCenter.x) * 0.015f;
            i_this->distToCamCenterTarget.y = fabsf(i_this->camCenterTarget.y - i_this->camCenter.y) * 0.015f;
            i_this->distToCamCenterTarget.z = fabsf(i_this->camCenterTarget.z - i_this->camCenter.z) * 0.015f;
            i_this->distToCamEyeTarget.x = fabsf(i_this->camEyeTarget.x - i_this->camEye.x) * 0.02f;
            i_this->distToCamEyeTarget.y = fabsf(i_this->camEyeTarget.y - i_this->camEye.y) * 0.02f;
            i_this->distToCamEyeTarget.z = fabsf(i_this->camEyeTarget.z - i_this->camEye.z) * 0.02f;

            dComIfGp_getEvent()->startCheckSkipEdge(actor);
            // fallthrough
        case 2:
            if (i_this->demoCamCounter > 60) {
                cLib_addCalc2(&i_this->camCenter.x, i_this->camCenterTarget.x, 0.05f, i_this->distToCamCenterTarget.x * i_this->camStepScale1);
                cLib_addCalc2(&i_this->camCenter.y, i_this->camCenterTarget.y, 0.05f, i_this->distToCamCenterTarget.y * i_this->camStepScale1);
                cLib_addCalc2(&i_this->camCenter.z, i_this->camCenterTarget.z, 0.05f, i_this->distToCamCenterTarget.z * i_this->camStepScale1);
                cLib_addCalc2(&i_this->camEye.x, i_this->camEyeTarget.x, 0.05f, i_this->distToCamEyeTarget.x * i_this->camStepScale1);
                cLib_addCalc2(&i_this->camEye.y, i_this->camEyeTarget.y, 0.05f, i_this->distToCamEyeTarget.y * i_this->camStepScale1);
                cLib_addCalc2(&i_this->camEye.z, i_this->camEyeTarget.z, 0.05f, i_this->distToCamEyeTarget.z * i_this->camStepScale1);
                cLib_addCalc2(&i_this->camStepScale1, 1.0f, 1.0f, NREG_F(4) + 0.01f);

                if (i_this->demoCamCounter == ZREG_S(1) + 258) {
                    monkey_p->mode = 2;
                    pos.set(110.0f, 3300.0f, 4326.0f);
                    player->setPlayerPosAndAngle(&pos, (s16)0xFFFF8000, 0);
                    player->changeDemoMode(1, 1, 0, 0);
                }

                if (i_this->demoCamCounter == ZREG_S(1) + 260) {
                    cMtx_YrotS(*calc_mtx, monkey_p->actor.shape_angle.y);
                    work.x = ZREG_F(0) + 200.0f;
                    work.y = ZREG_F(1) + 300.0f;
                    work.z = ZREG_F(2) + 300.0f;
                    MtxPosition(&work, &i_this->camEye);
                    i_this->camEye += monkey_p->actor.current.pos;
                    i_this->camCenter = monkey_p->actor.current.pos;
                    i_this->camCenter.y += ZREG_F(3) + 130.0f;
                    i_this->demoSubMode = 3;
                    i_this->demoCamCounter = 0;
                }
            }

            actor->current.pos.y = actor->home.pos.y + 10000.0f;
            break;

        case 3:
            actor->current.pos.y = actor->home.pos.y + 10000.0f;
            cLib_addCalc2(&i_this->camCenter.z, monkey_p->actor.current.pos.z, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->camCenter.y, monkey_p->actor.current.pos.y + 130.0f + ZREG_F(3) + (KREG_F(18) + 30.0f) * cM_ssin(i_this->demoCamCounter * 0xA00), 0.2f, 50.0f);

            if (i_this->demoCamCounter != 93) {
                break;
            }

            actor->current.pos.y = actor->home.pos.y;
            cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
            work.x = ZREG_F(4);
            work.y = ZREG_F(5) + -200.0f;
            work.z = ZREG_F(6) + 3350.0f;
            MtxPosition(&work, &i_this->camEye);
            i_this->camEye += actor->current.pos;
            i_this->camCenter = actor->current.pos;
            i_this->camCenter.y += ZREG_F(7) + 220.0f;
            i_this->demoSubMode = 4;
            i_this->demoCamCounter = 0;
            i_this->mode = 2;
            monkey_p->mode = 20;
            break;
        
        case 4:
            cLib_addCalc2(&i_this->camCenter.z, actor->current.pos.z, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->camCenter.y, actor->current.pos.y + 220.0f + ZREG_F(7), 0.2f, 50.0f);

            if (i_this->demoCamCounter != 74) {
                break;
            }

            i_this->demoSubMode = 45;
            i_this->demoCamCounter = 0;

            cMtx_YrotS(*calc_mtx, monkey_p->actor.shape_angle.y);
            work.x = ZREG_F(5) + -300.0f;
            work.y = ZREG_F(6) + 200.0f;
            work.z = ZREG_F(7) + -100.0f;
            MtxPosition(&work, &i_this->camEye);
            i_this->camEye += monkey_p->actor.current.pos;
            i_this->camCenter = monkey_p->actor.current.pos;
            i_this->camCenter.y += ZREG_F(8) + 90.0f;
            break;

        case 45:
            cLib_addCalc2(&i_this->camCenter.z, monkey_p->actor.current.pos.z, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->camCenter.y, monkey_p->actor.current.pos.y + 90.0f + ZREG_F(8) + (KREG_F(18) + 30.0f) * cM_ssin(i_this->demoCamCounter * 0xA00), 0.2f, 50.0f);

            if (i_this->demoCamCounter == 10) {
                monkey_p->actor.field_0x567 = 1;
            }

            if (i_this->demoCamCounter != NREG_S(7) + 55) {
                break;
            }

            i_this->demoSubMode = 46;
            i_this->demoCamCounter = 0;
            // fallthrough
        case 46:
            cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
            work.x = ZREG_F(14);
            work.y = ZREG_F(15) + 100.0f;
            work.z = ZREG_F(16) + 600.0f;
            MtxPosition(&work, &i_this->camEye);
            i_this->camEye += actor->current.pos;
            i_this->camCenter = actor->current.pos;
            i_this->camCenter.y += ZREG_F(17) + 220.0f;

            if (i_this->demoCamCounter < ZREG_S(9) + 45) {
                break;
            }

            if (boomerang_p == NULL) {
                break;
            }

            i_this->demoSubMode = 5;
            i_this->demoCamCounter = 0;
            // fallthrough
        case 5:
            i_this->camCenter = boomerang_p->current.pos;
            i_this->camEye.set(ZREG_F(8) + -477.0f, ZREG_F(9) + 3119.0f, ZREG_F(10) + 1643.0f);

            if (i_this->demoCamCounter == 30) {
                monkey_p->actor.current.pos.z = ZREG_F(15) + 1600.0f;
                monkey_p->actor.current.pos.y = ZREG_F(17) + 3200.0f;
            }

            if (i_this->demoCamCounter != ZREG_S(4) + 69) {
                break;
            }

            i_this->demoSubMode = 6;
            i_this->demoCamCounter = 0;
            break;

        case 6:
            if (i_this->demoCamCounter == ZREG_S(6) + 3) {
                monkey_p->mode = 4;
            }

            if (i_this->demoCamCounter != ZREG_S(5) + 25) {
                break;
            }

            i_this->demoSubMode = 7;
            i_this->demoCamCounter = 0;

            cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
            work.x = ZREG_F(11) + -300.0f;
            work.y = ZREG_F(12) + 100.0f;
            work.z = ZREG_F(13) + 550.0f;
            MtxPosition(&work, &i_this->camEye);
            i_this->camEye += actor->current.pos;
            i_this->camCenter = actor->current.pos;
            i_this->camCenter.y += ZREG_F(14) + 190.0f;
            // fallthrough
        case 7:
            monkey_p->actor.speed.y = 0.0f;
            monkey_p->actor.current.pos.set(0.0f, 2500.0f, 3373.0f);

            if (i_this->demoCamCounter != 250) {
                break;
            }

            i_this->demoSubMode = 8;
            i_this->demoCamCounter = 0;
            monkey_p->mode++;
            i_this->camEye.set(-282.0f, 2534.0f, 3147.0f);
            i_this->camCenter = monkey_p->actor.current.pos;
            i_this->camCenter.y += ZREG_F(19);
            // fallthrough
        case 8:
            cLib_addCalc2(&i_this->camCenter.y, monkey_p->actor.current.pos.y + ZREG_F(19), 0.2f, 50.0f);
            fpcM_Search(s_brg_sub, i_this);

            if (i_this->demoCamCounter == (s16)(XREG_S(0) + 65)) {
                monkey_p->actor.current.pos.set(0.0f, 3310.0f, 3427.0f);
                monkey_p->mode++;
                i_this->demoSubMode = 9;
                i_this->demoCamCounter = 0;
                fpcM_Search(s_brg_sub2, i_this);
                i_this->camCenter.set(4.0f, 3393.0f, 4180.0f);
                i_this->camEye.set(-131.0f, 3452.0f, 4552.0f);
                player->changeDemoMode(1, 1, 0, 0);
            }
            break;

        case 9:
            if (i_this->demoCamCounter != 138) {
                break;
            }

            i_this->camCenter.set(30.0f, 3422.0f, 4520.0f);
            i_this->camEye.set(343.0f, 3422.0f, 4271.0f);
            i_this->demoSubMode = 10;
            i_this->demoCamCounter = 0;
            i_this->prevPlShapeAngle = player->shape_angle.y;
            break;

        case 10: {
            cLib_addCalcAngleS2(&i_this->prevPlShapeAngle, monkey_p->target_angle + (u16)-0x8000, 4,
                                0x800);
            player->setPlayerPosAndAngle(&player->current.pos, i_this->prevPlShapeAngle, 0);

            if (i_this->demoCamCounter != 60) {
                break;
            }

            camera->mCamera.Reset(i_this->camCenter, i_this->camEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            fopAcM_delete(actor);
            u8 sw_bit = (fopAcM_GetParam(actor) & 0xff0000) >> 16;
            if (sw_bit == 0xFF) {
                break;
            }

            dComIfGs_onSwitch(sw_bit, fopAcM_GetRoomNo(actor));
        } break;

        case 0:
        case 100:
        default:
            break;
    }
}

static void demo_camera_bohit(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c *)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz work, unused_1, unused_2, unused_3;

    switch (i_this->demoSubMode) {
        case 1:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xffff, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                break;
            }

            camera->mCamera.Stop();
            i_this->demoSubMode = 2;
            i_this->demoCamCounter = 0;
            i_this->demoCamFovy = NREG_F(10) + 55.0f;
            camera->mCamera.SetTrimSize(3);

            cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
            if (cM_rndF(1.0f) < 0.5f) {
                work.x = AREG_F(0) + 500.0f;
            } else {
                work.x = -(AREG_F(0) + 500.0f);
            }

            work.y = AREG_F(1) + -120.0f;
            work.z = AREG_F(2);
            MtxPosition(&work, &i_this->camEye);
            i_this->camEye += actor->current.pos;
            i_this->camCenter = actor->current.pos;
            // fallthrough
        case 2:
            cLib_addCalc2(&i_this->camCenter.x, actor->current.pos.x, 0.5f, 50.0f);
            cLib_addCalc2(&i_this->camCenter.y, actor->current.pos.y + 100.0f + KREG_F(7), 0.5f, 50.0f);
            cLib_addCalc2(&i_this->camCenter.z, actor->current.pos.z, 0.5f, 50.0f);

            if (i_this->demoCamCounter == 25) {
                i_this->field_0xc98 = 30.0f;
            }

            if (i_this->demoCamCounter == 80) {
                i_this->demoMode = e_mk_class::DEMO_MODE_FINISH;
                i_this->timer[0] = 150;
            }
            break;

        case 0:
        default:
            break;
    }
}

static void demo_camera(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz unused_1, unused_2;

    switch (i_this->demoMode) {
        case e_mk_class::DEMO_MODE_START:
            demo_camera_start(i_this);
            break;
        
        case e_mk_class::DEMO_MODE_END:
            demo_camera_end(i_this);
            break;

        case e_mk_class::DEMO_MODE_R04:
            demo_camera_r04(i_this);
            if (dComIfGp_getEvent()->checkSkipEdge()) {
                npc_ks_class* monkey_p = (npc_ks_class*)fpcM_Search(s_ks_sub, i_this);
                if (monkey_p != NULL) {
                    u8 sw_bit = (fopAcM_GetParam(actor) & 0xFF0000) >> 16;
                    if (sw_bit != 0xFF) {
                        dComIfGs_onSwitch(sw_bit, fopAcM_GetRoomNo(actor));
                    }

                    sw_bit = (fopAcM_GetParam(monkey_p) & 0xFF000000) >> 24;
                    if (sw_bit != 0xFF) {
                        dComIfGs_onSwitch(sw_bit, fopAcM_GetRoomNo(&monkey_p->actor));
                    }

                    dComIfGp_setNextStage("D_MN05", 0, 4, 0);
                }
            }
            break;

        case e_mk_class::DEMO_MODE_BOHIT:
            demo_camera_bohit(i_this);
            break;

        case e_mk_class::DEMO_MODE_FINISH:
            camera->mCamera.Reset(i_this->camCenter, i_this->camEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            i_this->demoMode = e_mk_class::DEMO_MODE_NONE;
            i_this->demoSubMode = 0;
            daPy_getPlayerActorClass()->cancelOriginalDemo();

        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case e_mk_class::DEMO_MODE_NONE:
            break;
    }

    if (i_this->demoMode != e_mk_class::DEMO_MODE_NONE) {
        s16 bank = i_this->field_0xc98 * cM_scos(i_this->field_0x6b4 * 0x2C00) * 7.5f;
        cXyz center = i_this->camCenter;
        cXyz eye = i_this->camEye;

        center.y += (KREG_F(8) + 1.0f) * (i_this->field_0xc98 * cM_ssin(i_this->field_0x6b4 * 0x2A00));
        eye.y += (KREG_F(8) + 1.0f) * (i_this->field_0xc98 * cM_ssin(i_this->field_0x6b4 * 0x2A00));
        center.x +=(KREG_F(8) + 1.0f) *  (i_this->field_0xc98 * cM_ssin(i_this->field_0x6b4 * 0x2800));
        eye.x += (KREG_F(8) + 1.0f) * (i_this->field_0xc98 * cM_ssin(i_this->field_0x6b4 * 0x2800));
        center.z += (KREG_F(8) + 1.0f) * (i_this->field_0xc98 * cM_scos(i_this->field_0x6b4 * 0x2C00));
        eye.z += (KREG_F(8) + 1.0f) * (i_this->field_0xc98 * cM_scos(i_this->field_0x6b4 * 0x2C00));

        camera->mCamera.Set(center, eye, bank, i_this->demoCamFovy);

        cLib_addCalc0(&i_this->field_0xc98, 1.0f, TREG_F(16) + 3.0f);
        i_this->demoCamCounter++;

        if (i_this->demoCamCounter > 10000) {
            i_this->demoCamCounter = 10000;
        }
    }
}

static void action(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c *)dComIfGp_getPlayer(0);
    cXyz work, pos;
    i_this->distToPl = fopAcM_searchPlayerDistanceXZ(actor);
    i_this->angleToPl = fopAcM_searchPlayerAngleY(actor);
    damage_check(i_this);
    s8 boss_room_wait_flag = 1;
    s8 status_flag = 1;
    i_this->unkFlag3 = 0;

    switch (i_this->action) {
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
            i_this->unkFlag3 = 1;
            break;

        case e_mk_class::ACT_DAMAGE:
            e_mk_damage(i_this);
            i_this->unkFlag3 = 1;
            break;

        case e_mk_class::ACT_S_DEMO:
            e_mk_s_demo(i_this);
            boss_room_wait_flag = 0;
            break;

        case e_mk_class::ACT_E_DEMO:
            i_this->unkFlag3 = e_mk_e_demo(i_this);
            boss_room_wait_flag = 0;
            status_flag = 0;
            break;

        case e_mk_class::ACT_R04_DEMO:
            e_mk_r04_demo(i_this);
            i_this->unkFlag3 = 1;
    }

    if (status_flag) {
        fopAcM_OnStatus(actor, 0);
        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 4, 0x2000);

    if (i_this->unkFlag3 != 0) {
        actor->current.pos += actor->speed;
        actor->speed.y += actor->gravity;

        if (i_this->unkFlag3 == 1) {
            i_this->acch.CrrPos(dComIfG_Bgsp());
        }
    } else {
        actor->current.pos += actor->speed;
    }

    if (i_this->setSmokeFlag != 0) {
        if (i_this->setSmokeFlag == 2) {
            J3DModel* model = i_this->anmP->getModel();
            MTXCopy(model->getAnmMtx(MK_JNT_LEG_L_4_e), *calc_mtx);
            work.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&work, &pos);
            fopAcM_effSmokeSet2(&i_this->field_0xc08, &i_this->field_0xc0c, &pos, &actor->shape_angle, TREG_F(17) + 2.5f, &actor->tevStr);

            MTXCopy(model->getAnmMtx(MK_JNT_LEG_R_4_e), *calc_mtx);
            MtxPosition(&work, &pos);
            fopAcM_effSmokeSet2(&i_this->field_0xc10, &i_this->field_0xc14, &pos, &actor->shape_angle, TREG_F(17) + 2.5f, &actor->tevStr);
        } else {
            fopAcM_effSmokeSet1(&i_this->field_0xc08, &i_this->field_0xc0c, &actor->current.pos, &actor->shape_angle, TREG_F(18) + 2.5f, &actor->tevStr, 1);
        }

        i_this->setSmokeFlag = 0;
    }

    if (boss_room_wait_flag) {
        daPy_getPlayerActorClass()->onBossRoomWait();
    }
}

static void anm_se_set(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    
    static u16 bo_eno_1[2] = {
        dPa_RM(ID_ZI_S_MKDEMO_BOOM_WAIT_A),
        dPa_RM(ID_ZI_S_MKDEMO_BOOM_WAIT_B),
    };    
    
    static u16 bo_eno_0[4] = {
        dPa_RM(ID_ZI_S_MKDEMO_BOOM_TAME_A), 
        dPa_RM(ID_ZI_S_MKDEMO_BOOM_TAME_B), 
        dPa_RM(ID_ZI_S_MKDEMO_BOOM_TAME_C), 
        dPa_RM(ID_ZI_S_MKDEMO_BOOM_FLASH_A),
    };



    if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_WAIT_e) {
        if (i_this->anmP->checkFrame(2.0f) || i_this->anmP->checkFrame(13.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_BREATH, -1);
        }
    } else if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_SPRING_e) {
        if (i_this->anmP->checkFrame(3.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_DAMAGE_L, -1);
        }

        if ((i_this->anmP->checkFrame(21.0f) || i_this->anmP->checkFrame(31.0f)) || i_this->anmP->checkFrame(41.0f)) {
            i_this->sound.startCreatureSound(Z2SE_EN_MK_JUMP_END, 0, -1);
        }
    } else if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_PROVOCATION_01_e || i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_PROVOCATION_02_e) {
        if (i_this->anmP->checkFrame(1.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_PROVOKE_A, -1);
        }
    } else if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_PROVOCATION_03_e) {
        if (i_this->anmP->checkFrame(1.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_PROVOKE_B, -1);
        }

        if (i_this->anmP->checkFrame(15.0f) || i_this->anmP->checkFrame(64.0f)) {
            i_this->sound.startCreatureSound(Z2SE_EN_MK_FOOTNOTE, 0, -1);
        }

        if (i_this->anmP->checkFrame(32.0f)) {
            i_this->sound.startCreatureSound(Z2SE_EN_MK_SCRATCH_HIP, 0, -1);
        }
    } else if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_GLAD_e || i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_DEMO_GLAD_e) {
        if (i_this->anmP->checkFrame(1.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_SNEER, -1);
        }
    } else if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_CHANCE_e) {
        if (i_this->anmP->checkFrame(1.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_DOWN, -1);
        }
    } else if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_JUMP_START_e) {
        if (i_this->anmP->checkFrame(8.0f)) {
            i_this->sound.startCreatureSound(Z2SE_EN_MK_JUMP_START, 0, -1);
        }
    } else if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_THROW_UP_e) {
        if (i_this->anmP->checkFrame(20.0f)) {
            i_this->sound.startCreatureSound(Z2SE_EN_MK_JUMP_START, 0, -1);
        }
    } else if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_JUMP_END_e) {
        if (i_this->anmP->checkFrame(3.0f)) {
            i_this->sound.startCreatureSound(Z2SE_EN_MK_JUMP_END, 0, -1);
        }
    } else if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_THROW_UP_e) {
        if (i_this->anmP->checkFrame(38.0f)) {
            i_this->sound.startCreatureSound(Z2SE_EN_MK_JUMP_END, 0, -1);
        }
    } else if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_RUN_e) {
        if (i_this->anmP->checkFrame(1.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_EN_MK_V_BREATH, -1);
        }

        if (i_this->anmP->checkFrame(1.0f) || i_this->anmP->checkFrame(6.0f)) {
            i_this->sound.startCreatureSound(Z2SE_EN_MK_FOOTNOTE, 0, -1);
        }
    }

    if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_DEMO_THROW_e || i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_DEMO_THROW_WAIT_e) {
        for (int i = 0; i < 2; i++) {
            i_this->boEno1Prtcls[i] = dComIfGp_particle_set(i_this->boEno1Prtcls[i], bo_eno_1[i], &actor->current.pos, NULL, 0);

            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->boEno1Prtcls[i]);
            if (emitter != NULL) {
                emitter->setGlobalSRTMatrix(i_this->boomerangModelP->getBaseTRMtx());
            }
        }
    }

    if (i_this->anmNo == dRes_INDEX_E_MK_BCK_MK_DEMO_THROW_WAIT_e) {
        for (int i = 0; i < 4; i++) {
            i_this->boEno0Prtcls[i] = dComIfGp_particle_set(i_this->boEno0Prtcls[i], bo_eno_0[i], &actor->eyePos, NULL, 0);

            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->boEno0Prtcls[i]);
            if (emitter != NULL) {
                if (i == 0) {
                    emitter->setGlobalSRTMatrix(i_this->anmP->getModel()->getAnmMtx(MK_JNT_HAND_R_2_e));
                } else {
                    emitter->setGlobalSRTMatrix(i_this->boomerangModelP->getBaseTRMtx());
                }
            }
        }
    }
}

static int daE_MK_Execute(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    cXyz work, pos, unused;
    i_this->field_0x6b4++;

    for (int i = 0; i < 4; i++) {
        if (i_this->timer[i] != 0) {
            i_this->timer[i]--;
        }
    }

    if (i_this->invulnerabilityTimer != 0) {
        i_this->invulnerabilityTimer--;
    }

    if (i_this->unkTimer1 != 0) {
        i_this->unkTimer1--;
    }

    if (l_HIO.halt_action == 0) {
        action(i_this);
        demo_camera(i_this);
    }

    if (i_this->unkFlag2 != 0) {
        work = i_this->prevPosTarget - i_this->prevPos;
        f32 fVar1 = work.abs();

        work = i_this->prevPosTarget - actor->current.pos;
        f32 fVar2 = work.abs();

        f32 fVar3 = fVar1 * (BREG_F(19) + 0.1f);

        if (fVar3 > BREG_F(18) + 200.0f) {
            fVar3 = BREG_F(18) + 200.0f;
        }

        i_this->PosYTransOffset = fVar3 * cM_ssin((fVar2 / fVar1) * 32768.0f);
    } else {
        i_this->PosYTransOffset = 0;
    }

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y + i_this->PosYTransOffset, actor->current.pos.z);
    mDoMtx_stack_c::YrotM((s16)actor->shape_angle.y);
    mDoMtx_stack_c::XrotM((s16)actor->shape_angle.x);
    mDoMtx_stack_c::scaleM(l_HIO.size, l_HIO.size, l_HIO.size);

    J3DModel* model = i_this->anmP->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->anmP->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));

    if (i_this->field_0x5d4 == 0) {
        i_this->field_0x5d4 = cM_rndF(60.0f) + 30.0f;
    } else {
        i_this->field_0x5d4--;

        if (i_this->field_0x5d4 <= 12) {
            i_this->btpFrame = 11.99f - i_this->field_0x5d4;
        } else {
            i_this->btpFrame = 0.0f;
        }
    }

    if (i_this->btpFrameFlag != 0) {
        i_this->btpP->setFrame(i_this->btpFrame + 12.0f);
    } else {
        i_this->btpP->setFrame(i_this->btpFrame);
    }

    i_this->anmP->modelCalc();
    MTXCopy(model->getAnmMtx(MK_JNT_HEAD_e), *calc_mtx);
    work.set(KREG_F(12), KREG_F(13), KREG_F(14));
    MtxPosition(&work, &actor->eyePos);
    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += BREG_F(7) + 70.0f;

    if (i_this->crownStatus != 0) {
        if (i_this->unkFlag1 == 0) {
            MTXCopy(i_this->anmP->getModel()->getAnmMtx(MK_JNT_CROWN_e), mDoMtx_stack_c::get());
            mDoMtx_stack_c::transM(l_HIO.crown_pos_adjust.x, l_HIO.crown_pos_adjust.y, l_HIO.crown_pos_adjust.z);
            i_this->crownAnmP->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
            mDoMtx_multVecZero(mDoMtx_stack_c::get(), &i_this->crownPos);
        } else {
            cMtx_YrotS(*calc_mtx, i_this->unkRotation.y);
            work.x = 0.0f;
            work.y = i_this->field_0x5fc;
            work.z = i_this->field_0x600;
            MtxPosition(&work, &pos);
            i_this->crownPos += pos;
            i_this->field_0x5fc -= 5.0f;

            if (i_this->crownPos.y <= STAGE_CENTER_POS.y + WREG_F(7) + 5.0f) {
                i_this->crownPos.y = STAGE_CENTER_POS.y + WREG_F(7) + 5.0f;

                if (i_this->field_0x5fc < -30.0f) {
                    i_this->field_0x5fc *= -0.4f;
                    i_this->field_0x604 = NREG_F(0) + 3500.0f;
                } else {
                    i_this->field_0x5fc = 0.0f;
                }

                i_this->field_0x600 *= WREG_F(10) + 0.75f;
            }

            s16 x_rot_offset = i_this->field_0x604 * cM_ssin(i_this->field_0x6b4 * (NREG_S(3) + 3000));
            s16 z_rot = i_this->field_0x604 * cM_ssin(i_this->field_0x6b4 * (NREG_S(4) + 4000));
            cLib_addCalc0(&i_this->field_0x604, 0.1f, NREG_F(1) + 50.0f);
            mDoMtx_stack_c::transS(i_this->crownPos.x, i_this->crownPos.y, i_this->crownPos.z);
            mDoMtx_stack_c::YrotM(i_this->unkRotation.y);
            mDoMtx_stack_c::XrotM(i_this->unkRotation.x + x_rot_offset);
            mDoMtx_stack_c::ZrotM(z_rot);
            mDoMtx_stack_c::transM(WREG_F(3), WREG_F(4) + 15.0f, WREG_F(5) + -10.0f);
            mDoMtx_stack_c::XrotM(-8000); // ~-44°
            mDoMtx_stack_c::ZrotM(-0x4000); // -90°
            i_this->crownAnmP->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
            cLib_addCalcAngleS2(&i_this->unkRotation.x, 0, 2, 0x400);
        }

        i_this->crownAnmP->play(NULL, 0, 0);
        i_this->crownAnmP->modelCalc();
    }
    
    if (i_this->boomerangStatus != 0) {
        MTXCopy(i_this->anmP->getModel()->getAnmMtx(MK_JNT_HAND_R_3_e), mDoMtx_stack_c::get());
        mDoMtx_stack_c::scaleM(l_HIO.boomerang_ratio, l_HIO.boomerang_ratio, l_HIO.boomerang_ratio);
        mDoMtx_stack_c::transM(JREG_F(17) + 15.0f, JREG_F(18) + 70.0f, JREG_F(19) + 20.0f);
        mDoMtx_stack_c::YrotM(JREG_S(0) + -0x652C);
        mDoMtx_stack_c::XrotM(JREG_S(1) + -0x2219);
        mDoMtx_stack_c::ZrotM(JREG_S(2) + 0x38D8);
        i_this->boomerangModelP->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    if (i_this->unkFlag4 == 7) {
        u32 parameters = 0;
        if (i_this->demoSubMode != 0) {
            parameters = 1;
        }

        i_this->boomerangId = fopAcM_createChild(PROC_E_MK_BO, fopAcM_GetID(actor), parameters,
                                                 &actor->current.pos, fopAcM_GetRoomNo(actor), NULL, NULL,
                                                 -1, NULL);
        i_this->unkFlag4 = 0;
    }

    if (i_this->unkFlag4 == 1) {
        e_mk_bo_class* boomerang_p = (e_mk_bo_class*)fopAcM_SearchByID(i_this->boomerangId);
        if (boomerang_p != NULL) {
            MTXCopy(i_this->anmP->getModel()->getAnmMtx(MK_JNT_HAND_R_3_e), *calc_mtx);
            work.set(KREG_F(15), KREG_F(16), KREG_F(17));
            MtxPosition(&work, &pos);
            boomerang_p->enemy.current.pos = pos;
            boomerang_p->field_0x9b4 = 0;
            i_this->unkFlag4 = 0;
        }
    }

    MTXCopy(model->getAnmMtx(MK_JNT_WAIST_e), *calc_mtx);
    work.set(BREG_F(3) + 20.0f, BREG_F(4), BREG_F(5));
    MtxPosition(&work, &pos);

    if (i_this->action >= 9) {
        actor->eyePos = pos;
        actor->attention_info.position = actor->eyePos;
        actor->attention_info.position.y += BREG_F(7) + 40.0f;
    }

    if (i_this->invulnerabilityTimer != 0) {
        pos.y += 100000.0f;
    }

    i_this->tgSph.SetC(pos);
    i_this->tgSph.SetR((BREG_F(6) + 60.0f) * l_HIO.size);
    dComIfG_Ccsp()->Set(&i_this->tgSph);

    work.set(BREG_F(7) + -70.0f, BREG_F(8) + 70.0f, BREG_F(9));
    MtxPosition(&work, &pos);

    if (i_this->invulnerabilityTimer != 0) {
        pos.y += 100000.0f;
    }

    i_this->sph.SetC(pos);
    i_this->sph.SetR((BREG_F(10) + 60.0f) * l_HIO.size);
    dComIfG_Ccsp()->Set(&i_this->sph);

    if (i_this->tubaTimer != 0) {
        i_this->tubaTimer--;
        if (i_this->tubaTimer == 0) {
            dComIfGp_particle_set(ID_ZI_J_TUBA00, &actor->eyePos, &actor->shape_angle, NULL);
        }
    }

    anm_se_set(i_this);
    return 1;
}

static int daE_MK_IsDelete(e_mk_class* i_this) {
    return 1;
}

static int daE_MK_Delete(e_mk_class* i_this) {
    fopEn_enemy_c* actor = (fopEn_enemy_c*)&i_this->actor;
    fpc_ProcID id = fopAcM_GetID(i_this);

    dComIfG_resDelete(&i_this->phase, "E_mk");
    if (i_this->hioInit != 0) {
        hio_set = 0;
        mDoHIO_DELETE_CHILD(l_HIO.no);
    }

    if (actor->heap != NULL) {
        i_this->anmP->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    e_mk_class* i_this = (e_mk_class*)actor;
    i_this->anmP = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_mk", dRes_INDEX_E_MK_BMD_MK_e), NULL, NULL,
                                               (J3DAnmTransform*)dComIfG_getObjectRes("E_mk", dRes_INDEX_E_MK_BCK_MK_WAIT_e), 0, 1.0f,
                                               0, -1, &i_this->sound, 0x80000, 0x11020084);
    if (i_this->anmP == NULL || i_this->anmP->getModel() == NULL) {
        return 0;
    }

    i_this->btpP = new mDoExt_btpAnm();
    if (i_this->btpP == NULL) {
        return 0;
    }
    if (i_this->btpP->init(i_this->anmP->getModel()->getModelData(), (J3DAnmTexPattern*)dComIfG_getObjectRes("E_mk", dRes_INDEX_E_MK_BTP_MK_EYE_e),
                       1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }

    i_this->crownAnmP = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("E_mk", dRes_INDEX_E_MK_BMD_OK_e), NULL, NULL,
                                              (J3DAnmTransform*)dComIfG_getObjectRes("E_mk", dRes_INDEX_E_MK_BCK_OK_DEFAULTPOSE_e), 0, 1.0f,
                                              0, -1, 1, NULL, 0x80000, 0x11000084);
    if (i_this->crownAnmP == NULL || i_this->crownAnmP->getModel() == NULL) {
        return 0;
    }

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_mk", dRes_INDEX_E_MK_BMD_BM_e);
    JUT_ASSERT(4743, modelData != NULL);
    i_this->boomerangModelP = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->boomerangModelP == NULL) {
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

    fopAcM_ct(&mk->actor, e_mk_class);

    int phase = dComIfG_resLoad(&mk->phase, "E_mk");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_MK PARAM %x\n", fopAcM_GetParam(i_actor));
        if (strcmp(dComIfGp_getStartStageName(), "D_MN05B") == 0 && dComIfGs_isStageMiddleBoss()) {
            OS_REPORT("中ボスE_MK やられ後なので再セットしません\n"); // Since it's the middle boss E_MK after being defeated, I won't reset it.
            return cPhs_ERROR_e;
        }

        u8 i_no = (fopAcM_GetParam(i_actor) & 0xFF000000) >> 24;
        if (i_no != 0xFF) {
            if (dComIfGs_isSwitch(i_no, fopAcM_GetRoomNo(i_actor))) {
                OS_REPORT("E_MK やられ後なので再セットしません\n"); // Since it's after the E_MK was done, I won't reset it.
                return cPhs_ERROR_e;
            }
        }

        mk->arg0 = fopAcM_GetParam(i_actor);
        OS_REPORT("E_MK//////////////E_MK SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_actor, useHeapInit, 0x56f0)) {
            OS_REPORT("//////////////E_MK SET NON !!\n");
            return cPhs_ERROR_e;
        }

        lbl_210_bss_130 = 0;
        if (hio_set == 0) {
            mk->hioInit = 1;
            hio_set = 1;
            l_HIO.no = mDoHIO_CREATE_CHILD("ブーメラン猿", &l_HIO);
        }

        i_actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        fopAcM_SetMtx(i_actor, mk->anmP->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_actor, -500.0f, -500.0f, -500.0f);
        fopAcM_SetMax(i_actor, 500.0f, 500.0f, 500.0f);

        mk->acch.Set(fopAcM_GetPosition_p(i_actor), fopAcM_GetOldPosition_p(i_actor), i_actor, 1, &mk->acchcir,
                          fopAcM_GetSpeed_p(i_actor), NULL, NULL);
        mk->acchcir.SetWall(80.0f, 30.0f);
        i_actor->field_0x560 = i_actor->health = 200;

        mk->sound.init(&i_actor->current.pos, &i_actor->eyePos, 3, 1);
        mk->atInfo.mpSound = &mk->sound;

        i_actor->attention_info.distances[2] = 4;

        mk->stts.Init(0xFF, 0, i_actor);
        mk->tgSph.Set(cc_sph_src);
        mk->tgSph.SetStts(&mk->stts);
        mk->sph.Set(cc_sph_src);
        mk->sph.SetStts(&mk->stts);
        mk->sph.OnTgNoHitMark();
        mk->crownStatus = 1;
        mk->boomerangStatus = 1;

        if (fopAcM_GetRoomNo(i_actor) == 4) {
            mk->action = e_mk_class::ACT_R04_DEMO;
            i_actor->current.pos.set(0.0f, 3900.0f, -3000.0f);
            i_actor->current.angle.y = 0;
            i_actor->home = i_actor->current;
        } else {
            STAGE_CENTER_POS = i_actor->home.pos;
            STAGE_CENTER_POS.y -= 500.0f;
            STAGE_ANGLE_Y = i_actor->home.angle.y;

            i_no = (fopAcM_GetParam(i_actor) & 0xff0000) >> 16;
            if (i_no != 0xFF && dComIfGs_isSwitch(i_no, fopAcM_GetRoomNo(i_actor))) {
                dComIfGs_offSwitch(i_no, fopAcM_GetRoomNo(i_actor));
                mk->action = e_mk_class::ACT_WAIT;
                Z2GetAudioMgr()->subBgmStart(Z2BGM_BOOMERAMG_MONKEY);
            } else {
                mk->action = e_mk_class::ACT_S_DEMO;
                u32 i_parameters = fopAcM_GetParam(i_actor) & 0xFF000000 | 0xFFFF01;
                cXyz sp30(-21.0f, 5114.0f, -4941.0f);
                mk->dbId1 = fopAcM_createChild(PROC_E_DB, fopAcM_GetID(i_actor), i_parameters,
                                                        &sp30, fopAcM_GetRoomNo(i_actor), NULL,
                                                        NULL, -1, NULL);

                sp30.set(-10.0f, 5114.0f, -4401.0f);
                mk->dbId2 = fopAcM_createChild(PROC_E_DB, fopAcM_GetID(i_actor), i_parameters,
                                                        &sp30, fopAcM_GetRoomNo(i_actor), NULL,
                                                        NULL, -1, NULL);
                
                fopAcM_OnStatus(i_actor, 0x4000);
            }
        }

        i_actor->gravity = -5.0f;
        i_actor->scale.x = l_HIO.size * l_HIO.boomerang_ratio;
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

actor_process_profile_definition g_profile_E_MK = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_MK,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_mk_class),     // size
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  165,                    // mPriority
  &l_daE_MK_Method,       // sub_method
  0x00040100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
