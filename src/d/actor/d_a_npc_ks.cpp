/**
 * @file d_a_npc_ks.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_ks.h"
#include "d/actor/d_a_obj_kago.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"
#include "d/d_bomb.h"
#include "d/actor/d_a_boomerang.h"
#include "d/actor/d_a_obj_pillar.h"
#include "c/c_damagereaction.h"
#include "d/actor/d_a_e_fs.h"
#include "d/actor/d_a_obj_brg.h"
#include "d/actor/d_a_midna.h"
#include "d/d_meter2_info.h"
#include "d/actor/d_a_e_oc.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"

class daNpc_Ks_HIO_c : public JORReflexible {
public:
    daNpc_Ks_HIO_c();
    virtual ~daNpc_Ks_HIO_c() {};

    void genMessage(JORMContext*);

    /* 0x04 */ s8 no;
    /* 0x08 */ f32 basic_size;
    /* 0x0C */ f32 pull_distance;
    /* 0x10 */ f32 holding_speed_l;
    /* 0x14 */ f32 holding_speed_h;
    /* 0x18 */ f32 demo_speed;
    /* 0x1C */ f32 demo_speed_2;
    /* 0x20 */ f32 guided_movement_speed;
    /* 0x24 */ f32 field_0x24;  // unused
    /* 0x28 */ u8 field_0x28;   // unused
    /* 0x2C */ f32 lantern_swing_interval;
    /* 0x30 */ f32 link_approach_distance;
    /* 0x34 */ f32 lantern_size;
};

struct path {
    int field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
};

daNpc_Ks_HIO_c::daNpc_Ks_HIO_c() {
    no = -1;
    basic_size = 0.8f;
    pull_distance = 200.0f;
    holding_speed_l = 10.0f;
    holding_speed_h = 25.0f;
    demo_speed = 20.0f;
    demo_speed_2 = 25.0f;
    guided_movement_speed = 10.0f;
    link_approach_distance = 300.0f;
    lantern_size = 1.8f;
    field_0x24 = 600.0f;    // unused
    field_0x28 = 1;         // unused
    lantern_swing_interval = 10.0f;
}

#if DEBUG
void daNpc_Ks_HIO_c::genMessage(JORMContext* ctext) {
    // little monkey
    ctext->genLabel("　小猿　", 0x80000001, 0, NULL, -1, -1, 0x200, 0x18);
    ctext->genLabel("       　", 0x80000001, 0, NULL, -1, -1, 0x200, 0x18);
    ctext->genLabel("       　", 0x80000001, 0, NULL, -1, -1, 0x200, 0x18);
    ctext->genLabel("       　", 0x80000001, 0, NULL, -1, -1, 0x200, 0x18);
    // (below is) lantern monkey related
    ctext->genLabel("       ↓　カンテラ猿関連　↓　", 0x80000001, 0, NULL, -1, -1, 0x200, 0x18);
    // latern swing interval
    ctext->genSlider("カンテラ振り間隔", &lantern_swing_interval, 0.0f, 20.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // guided movement speed
    ctext->genSlider("誘導移動速度", &guided_movement_speed, 0.0f, 60.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // link approach distance
    ctext->genSlider("リンク接近距離", &link_approach_distance, 0.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // lantern size
    ctext->genSlider("カンテラサイズ", &lantern_size, 0.0f, 3.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // (above is) lantern monkey related
    ctext->genLabel("       ↑　カンテラ猿関連　↑　", 0x80000001, 0, NULL, -1, -1, 0x200, 0x18);
    ctext->genLabel("       　", 0x80000001, 0, NULL, -1, -1, 0x200, 0x18);
    ctext->genLabel("       　", 0x80000001, 0, NULL, -1, -1, 0x200, 0x18);
    ctext->genLabel("       　", 0x80000001, 0, NULL, -1, -1, 0x200, 0x18);
    ctext->genLabel("       　", 0x80000001, 0, NULL, -1, -1, 0x200, 0x18);
    // basic size
    ctext->genSlider("基本大きさ", &basic_size, 0.0f, 3.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // pull distance
    ctext->genSlider("引連間合い", &pull_distance, 0.0f, 500.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // holding speed L
    ctext->genSlider("引連速Ｌ", &holding_speed_l, 0.0f, 60.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // holding speed H
    ctext->genSlider("引連速Ｈ", &holding_speed_h, 0.0f, 60.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // demo speed
    ctext->genSlider("デモ速", &demo_speed, 0.0f, 60.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // demo speed 2
    ctext->genSlider("デモ速2", &demo_speed_2, 0.0f, 60.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
}
#endif

static void setPlayerPosAndAngle(cXyz* i_pos, s16 i_angle) {
    if (dComIfGp_checkPlayerStatus0(0, 0x100) == 0) {
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(i_pos, i_angle, 0);
    }
}

static BOOL checkDoorDemo() {
    if (daPy_getPlayerActorClass()->eventInfo.checkCommandDoor() && dComIfGp_event_chkEventFlag(4) == 0) {
        return TRUE;
    }

    return FALSE;
}

static BOOL otherBgCheck(fopAc_ac_c* actor, fopAc_ac_c* actor2) {
    dBgS_LinChk p_line;
    cXyz unused_cxyz, pi_start, pi_end;
    pi_end = actor2->current.pos;
    pi_end.y += 100.0f + KREG_F(18);

    pi_start = actor->current.pos;
    pi_start.y += 120.0f + KREG_F(19);

    p_line.Set(&pi_start, &pi_end, actor);

    if (dComIfG_Bgsp().LineCross(&p_line) != 0) {
        return TRUE;
    }

    return FALSE;
}

static void anm_init(npc_ks_class* i_this, int param_2, f32 i_morf, u8 i_attr, f32 i_rate) {
    if (i_this->anm_time != 0 && (param_2 == 51 || param_2 == 53)) {
        if (cM_rndF(1.0f) < 0.5f) {
            param_2 = 11;
        } else {
            param_2 = 56;
        }
    }

    int i_index = param_2;
    if (i_this->field_0xc17 != 0) {
        if (param_2 == 51) {
            param_2 = 52;
        } else if (param_2 == 8) {
            param_2 = 9;
        } else if (param_2 == 28) {
            param_2 = 29;
        } else if (param_2 == 26) {
            param_2 = 27;
        } else if (param_2 == 8) {
            param_2 = 9;
        } else if (param_2 == 7) {
            param_2 = 10;
        } else if (param_2 == 43) {
            param_2 = 40;
        } else if (param_2 == 39) {
            param_2 = 42;
        } else if (param_2 == 39) {
            param_2 = 42;
        } else {
            // bug: developers meant to set equal to 44?
            param_2 == 44;
            dComIfGs_shake_kandelaar();
        }
    }

    i_this->model->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->res_name, param_2), i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->res_id = i_index;
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DJoint* my_joint = i_joint;
        int jnt_no = my_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        npc_ks_class* i_this = (npc_ks_class*)model->getUserArea();
        fopEn_enemy_c* actor = &i_this->actor;

        if (i_this != NULL) {
            if (i_this->field_0x5fc == 1 && jnt_no == 3) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, -i_this->field_0x5fe);
                cMtx_XrotM(*calc_mtx, -(i_this->field_0x5fe / 2));
                cMtx_ZrotM(*calc_mtx, s16(i_this->field_0x600));
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);

            } else if (i_this->field_0x5fc >= 2 && (jnt_no == 3 || jnt_no == 4)) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                if (jnt_no == 3) {
                    cMtx_YrotM(*calc_mtx, -(i_this->field_0x5fe / 2));
                    cMtx_XrotM(*calc_mtx, -(i_this->field_0x5fe / 2));
                } else {
                    cMtx_YrotM(*calc_mtx, -i_this->field_0x5fe);
                }

                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }

            if (i_this->field_0x5fc >= 2 && (jnt_no == 1 || jnt_no == 2)) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                if (jnt_no == 2) {
                    cMtx_YrotM(*calc_mtx, i_this->field_0x602 / 2);
                } else {
                    cMtx_YrotM(*calc_mtx, i_this->field_0x602);
                }

                cMtx_XrotM(*calc_mtx, -i_this->field_0x5fe);
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }

            if (jnt_no == 15) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_ZrotM(*calc_mtx, i_this->field_0x606);
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static int daNpc_Ks_Draw(npc_ks_class* i_this) {
    if (i_this->no_draw != 0) {
        i_this->no_draw = 0;
        return 1;
    }

    fopAc_ac_c* actor = &i_this->actor;
    J3DModel* model = i_this->model->getModel();
    if (i_this->res_name == "Npc_kst") {
        g_env_light.settingTevStruct(4, &actor->current.pos, &actor->tevStr);
    } else {
        g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    }

    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);

    if (i_this->field_0x5e4 != 0) {
        i_this->btp2->entry(model->getModelData());
    } else {
        i_this->btp->entry(model->getModelData());
    }
    i_this->model->entryDL();

    if (i_this->field_0xc17 != 0) {
        g_env_light.setLightTevColorType_MAJI(i_this->stick_model, &actor->tevStr);
        mDoExt_modelUpdateDL(i_this->stick_model);
    }

    if (i_this->bara_model != NULL) {
        g_env_light.setLightTevColorType_MAJI(i_this->bara_model, &actor->tevStr);
        mDoExt_modelUpdateDL(i_this->bara_model);
    }

    cXyz pos;
    pos.set(actor->current.pos.x, actor->current.pos.y + 100.0f, actor->current.pos.z);
    i_this->shadow_key = dComIfGd_setShadow(i_this->shadow_key, 1, model, &pos, 1000.0f, 0.0f, actor->current.pos.y, 
                                            i_this->ObjAcch.GetGroundH(), i_this->ObjAcch.m_gnd, &i_this->actor.tevStr,
                                            0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    if (i_this->field_0xc17 != 0) {
        dComIfGd_addRealShadow(i_this->shadow_key, i_this->stick_model);
    }

    return 1;
}

static void* s_resq_sub(void* i_actor, void* i_data) {
    (void) i_data;
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KS) {
        ((npc_ks_class*)i_actor)->anm_time = cM_rndF(50.0f) + 220.0f;
    }
    return NULL;
};

static cXyz obj_pos;

static npc_ks_class* leader;

static u32 call_pt;

static u8 hio_set;

static daNpc_Ks_HIO_c l_HIO;

// Note that this is .bss in dbg asm, thus it must come here.
static npc_ks_class* saru_p[8] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static void* target_info[10];

static int target_info_count;

static void* s_b_sub(void* i_actor, void* i_data) {
    (void) i_data;
    if (fopAcM_IsActor(i_actor) && dBomb_c::checkBombActor((fopAc_ac_c*)i_actor) && 
        !((dBomb_c*)i_actor)->checkStateExplode() && target_info_count < 10) {
            target_info[target_info_count] = i_actor;
            target_info_count++;
    }
    
    return NULL;
}

static int target_bgc[10];

#if DEBUG

// fakematch to get proper casting
#define NPC_KS_18DEG_ROT (s16) 0x800
#define NPC_KS_36DEG_ROT (s16) 0x1600
#define NPC_KS_45DEG_ROT (s16) 0x2000
#define NPC_KS_90DEG_ROT (s16) 0x4000
#define NPC_KS_180DEG_ROT (s16) 0x8000
#define NPC_KS_NEG_180DEG_ROT (s16) -0x8000

#define NPC_KS_FABSF fabsf

#else

#define NPC_KS_18DEG_ROT 0x800
#define NPC_KS_36DEG_ROT 0x1600
#define NPC_KS_45DEG_ROT 0x2000
#define NPC_KS_90DEG_ROT 0x4000
#define NPC_KS_180DEG_ROT 0x8000
#define NPC_KS_NEG_180DEG_ROT -0x8000

#define NPC_KS_FABSF std::fabsf

#endif

static fopAc_ac_c* search_bomb(npc_ks_class* i_this, int param_2) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* actor_p;
    target_info_count = 0;

    for (int i = 0; i < 10; i++) {
        target_info[i] = NULL;
        target_bgc[i] = 0;
    }

    fpcM_Search(s_b_sub, i_this);
    f32 fVar1 = 50.0f;
    
    if (target_info_count != 0) {
        cXyz sp4c, unused_vec;
        int i = 0;
        while (i < target_info_count) {
            actor_p = (fopAc_ac_c*)target_info[i];
            sp4c.x = actor_p->current.pos.x - actor->eyePos.x;
            sp4c.y = (actor_p->current.pos.y + 50.0f) - actor->eyePos.y;
            sp4c.z = actor_p->current.pos.z - actor->eyePos.z;
            f32 sq_rt = JMAFastSqrt(sp4c.x * sp4c.x + sp4c.z * sp4c.z);
            if (sq_rt < fVar1) {
                if (param_2 == 0) {
                    return actor_p;
                }

                if (target_bgc[i] != 0 || fopAcM_otherBgCheck(actor, actor_p)) {
                    target_bgc[i] = 1;
                } else if (NPC_KS_FABSF(sp4c.y) <= 300.0f) {
                    s16 sVar1 = actor->shape_angle.y - cM_atan2s(sp4c.x, sp4c.z);
                    if (sVar1 < 0) {
                        sVar1 = -1 * sVar1;
                    }

                    if ((u16)sVar1 < 20000) {
                        return actor_p;
                    }
                }
            }

            i++;
            if (i == target_info_count) {
                i = 0;
                fVar1 += 100.0f;
                if (fVar1 > 1500.0f) {
                    return NULL;
                }
            }
        }
    } else {
        return NULL;
    }

    return NULL;
}

static fopAc_ac_c* bomb_view_check(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    return search_bomb(i_this, 1);
}

static fopAc_ac_c* bomb_check(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    return search_bomb(i_this, 0);
}

static void* s_e_sub(void* i_actor, void* i_data) {
    (void) i_data;
    if (fopAcM_IsActor(i_actor) && ((npc_ks_class*)i_actor)->actor.field_0x566 != 0 && target_info_count < 10) {
        target_info[target_info_count] = i_actor;
        target_info_count++;
    }
    return NULL;
}

static fopAc_ac_c* search_enemy(npc_ks_class* i_this, int param_2, f32 param_3) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* enemy_p;
    if (dComIfGp_event_runCheck()) {
        if (fopAcM_getTalkEventPartner(daPy_getLinkPlayerActorClass()) != (fopAc_ac_c *)daPy_py_c::getMidnaActor()) {
            return NULL;
        }
    }

    target_info_count = 0;
    for (int i = 0; i < 10; i++) {
        target_info[i] = NULL;
        target_bgc[i] = 0;
    }

    fpcM_Search(s_e_sub, i_this);
    f32 fVar1 = 50.0f;
    f32 fVar2 = 300.0f;
    // boss key room
    if (fopAcM_GetRoomNo(actor) == 1) {
        fVar2 = 800.0f;
    }

    if (target_info_count != 0) {
        cXyz mae, unused_cxyz;
        int i = 0;
        while (i < target_info_count) {
            enemy_p = (fopAc_ac_c*)target_info[i];
            mae.x = enemy_p->current.pos.x - actor->eyePos.x;
            mae.y = (enemy_p->current.pos.y + 50.0f) - actor->eyePos.y;
            mae.z = enemy_p->current.pos.z - actor->eyePos.z;
            f32 square_root = JMAFastSqrt(mae.x * mae.x + mae.z * mae.z);
            if (square_root < fVar1) {
                if (param_2 == 0) {
                    return enemy_p;
                }

                if (target_bgc[i] != 0 || fopAcM_otherBgCheck(actor, enemy_p)) {
                    target_bgc[i] = 1;
                } else if (NPC_KS_FABSF(mae.y) <= fVar2) {
                    s16 sVar1 = actor->shape_angle.y - cM_atan2s(mae.x, mae.z);
                    if (sVar1 < 0) {
                        sVar1 = -1 * sVar1;
                    }

                    if ((u16)sVar1 < 20000) {
                        return enemy_p;
                    }
                }
            }

            i++;
            if (i == target_info_count) {
                i = 0;
                fVar1 += 50.0f;
                if (fVar1 > param_3) {
                    return NULL;
                }
            }
        }
    } else {
        return NULL;
    }

    return NULL;
}

static fopAc_ac_c* enemy_view_check(npc_ks_class* i_this, f32 param_2) {
    fopAc_ac_c* actor = &i_this->actor;
    return search_enemy(i_this, 1, param_2);
}

static fopAc_ac_c* enemy_check(npc_ks_class* i_this, f32 param_2) {
    fopAc_ac_c* actor = &i_this->actor;
    return search_enemy(i_this, 0, param_2);
}

static void* s_en_sub(void* i_actor, void* i_data) {
    (void) i_data;
    if (fopAcM_IsActor(i_actor) && ((npc_ks_class*)i_actor)->actor.field_0x560 != 0 && target_info_count < 10) {
        target_info[target_info_count] = i_actor;
        target_info_count++;
    }
    return NULL;
}

static fopAc_ac_c* en_search_test(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    target_info_count = 0;
    for (int i = 0; i < 10; i++) {
        target_info[i] = NULL;
    }

    fpcM_Search(s_en_sub, i_this);

    f32 fVar1 = 50.0f;
    if (target_info_count != 0) {
        cXyz mae;
        int i = 0;
        while (i < target_info_count) {
            fopAc_ac_c* actor_p = (fopAc_ac_c*)target_info[i];
            mae = actor_p->current.pos - actor->current.pos;
            f32 abs = mae.abs(); 
            if (abs < fVar1) {
                return actor_p;
            }

            i++;
            if (i == target_info_count) {
                i = 0;
                fVar1 += 50.0f;
                if (fVar1 > 350.0f) {
                    return NULL;
                }
            }
        }
    } else {
        return NULL;
    }

    return NULL;
}

static void* s_ori_sub(void* i_actor, void* i_data) {
    (void) i_data;
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_SO) {
        return i_actor;
    }
    return NULL;
}

static void npc_ks_pole_ori(npc_ks_class* i_this) {
    obj_so_class* cage;
    if (leader != NULL) {
        fopAc_ac_c* actor = &i_this->actor;
        if (i_this->mode == 0) {
            cage = (obj_so_class*)fpcM_Search(s_ori_sub, i_this);
            if (cage != NULL) {
                actor->parentActorID = fopAcM_GetID(cage);
                anm_init(i_this, 20, 5.0f, 2, 1.0f);
                i_this->mode++;
                i_this->mode = 1;
            } else {
                return;
            }
        }

        cage = (obj_so_class*) fopAcM_SearchByID(actor->parentActorID);
        if (cage != NULL) {
            if (cage->actor.health == 0) {
                i_this->action = 201;
                i_this->mode = 10;
                dComIfGs_onTbox(i_this->bitTRB);
            } else {
                if (i_this->mode < 10 && cage->field_0xdae != 0) {
                    i_this->mode = 10;
                    i_this->model->setPlaySpeed(0.0f);
                }

                switch (i_this->mode) {
                    case 1: {
                        if (i_this->model->checkFrame(5.0f) || i_this->model->checkFrame(20.0f)) {
                            cage->field_0xdc8 += 400.0f + KREG_F(9);
                            cage->field_0xdac = 0;
                            i_this->sound.startCreatureSound(Z2SE_OBJ_MONKEYJAIL_CREAK, 0, -1);
                            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_PRISONED, -1);
                        }

                        break;
                    }

                    case 10: {
                        break;
                    }
                }

                actor->current.angle.y = cage->actor.shape_angle.y;
                actor->current.angle.x = cage->actor.shape_angle.x;
                actor->current.pos = cage->actor.current.pos;
                actor->current.pos.y += TREG_F(3);

                if (leader->demo_mode == 0) {
                    if (!dComIfGs_isSwitch(20, fopAcM_GetRoomNo(actor))) {
                        cXyz ato(11497.0f, 3764.0f, 3810.0f);
                        daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
                        ato -= player->current.pos;
                        ato.y = 0.0f;
                        if (ato.abs() < 300.0f) {
                            leader->demo_mode = 110;
                        }
                    }
                }
            }
        }
    }
}

static int npc_ks_ori(npc_ks_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    cXyz mae, ato;
    int rv = 0;
    switch (i_this->mode) {
        case 0:
            anm_init(i_this, 21, 10.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->timer[0] = cM_rndF(200.0f) + 100.0f;
            break;

        case 1:
            if (i_this->model->checkFrame(5.0f) || i_this->model->checkFrame(20.0f)) {
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_PRISONED, -1);
                i_this->sound.startCreatureSound(Z2SE_OBJ_MONKEYJAIL_CREAK, 0, -1);
            }

            if (i_this->timer[0] == 0 && i_this->model->checkFrame(99.0f)) {
                i_this->mode = 2;
                anm_init(i_this, 7, 5.0f, 0, 1.0f);
                i_this->timer[0] = cM_rndF(60.0f) + 40.0f;
            }
            break;

        case 2:
            if (i_this->model->isStop()) {
                if (i_this->timer[0] == 0) {
                    i_this->mode = 0;
                } else {
                    anm_init(i_this, 7, 5.0f, 0, 1.0f);
                }
            }
            break;

        case 10:
            cMtx_YrotS(*calc_mtx, actor->current.angle.y);
            mae.x = 0.0f;
            mae.y = 0.0f;
            mae.z = actor->speedF;
            MtxPosition(&mae, &ato);
            actor->current.pos += ato;

            if (i_this->timer[1] == 1) {
                actor->speedF = 0.0f;
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
            }

            if (i_this->timer[0] == 0) {
                actor->health = 0;
                i_this->action = 100;
                i_this->timer[0] = 20;
                i_this->mode = 11;
                i_this->current_angle.y = i_this->target_angle;
                fopAcM_setStageLayer(actor);
                fpcM_Search(s_resq_sub, i_this);
            }
            break;

        case 11:
            rv = 1;
            if (i_this->timer[1] != 0) {
                actor->speedF = -20.0f + NREG_F(11);
            } else {
                actor->speedF = 0.0f;
            }

            if (i_this->timer[0] == 50) {
                anm_init(i_this, 11, 3.0f, 2, 1.0f);
            }

            if (i_this->timer[0] == 0) {
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
                i_this->timer[0] = 15;
                i_this->mode++;
            }
            break;

        case 12:
            rv = 1;
            if (i_this->timer[0] == 1) {
                anm_init(i_this, 32, 2.0f, 0, 1.0f);
                if (fopAcM_GetRoomNo(actor) == 11) {
                    actor->current.angle.y += NPC_KS_36DEG_ROT;
                }
            }

            if (i_this->res_id == 26) {
                actor->speedF = 20.0f;
            }

            if (i_this->res_id == 32 && i_this->model->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                actor->speedF = 40.0f;
                actor->speed.y = 35.0f;
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                i_this->field_0xbe0 = 1;
                i_this->mode++;
                i_this->timer[0] = 50;
            }
            break;

        case 13:
            rv = 1;
            actor->gravity = -5.0f;
            if (i_this->timer[0] == 0 || !dComIfGp_event_runCheck()) {
                fopAcM_delete(actor);
            }
    }

    if (i_this->mode < 10) {
        int swBit = (fopAcM_GetParam(actor) & 0xFF000000) >> 24;
        if (swBit != 0xFF) {
            if (dComIfGs_isSwitch(swBit, fopAcM_GetRoomNo(actor))) {
                dComIfGs_onTbox(i_this->bitTRB);
                if (i_this->set_id >= 5) {
                    i_this->mode = 11;
                    i_this->timer[0] = 80;
                    anm_init(i_this, 51 ,5.0f, 2, 1.0f);
                    if (fopAcM_GetRoomNo(actor) == 11) {
                        i_this->timer[1] = 10;
                    }
                } else {
                    i_this->mode = 10;
                    actor->speedF = -10.0f + NREG_F(11);
                    i_this->timer[0] = 110 + nREG_S(2);
                    i_this->timer[1] = 15;
                }
            }
        }
    } else {
        i_this->field_0x5fc = 0;
    }

    return rv;
}

static void* shot_bo_sub(void* i_actor, void* i_data) {
    (void) i_data;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_BOOMERANG && 
        dComIfGp_checkPlayerStatus0(0, 0x80000) == 0 && fopAcM_GetParam(i_actor) == 1) {
        cXyz ato(((daBoomerang_c*)i_actor)->current.pos - ((fopAc_ac_c*)i_data)->current.pos);
        if (ato.abs() < 1000.0f) {
            return i_actor;
        }
    }

    return NULL;
}

static path guide_path_04[7] = {
    { 0, -4554.0f, 3893.0f, -2377.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, -3379.0f, 3944.0f, -2314.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, -1887.0f, 3944.0f, -2184.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, -912.0f, 3907.0f, -2243.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 100.0f, 3900.0f, -2536.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 519.0f, 4590.0f, -2889.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { -1, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

static int npc_ks_ori2(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    obj_so_class* cage_p;
    int rv = 1;
    if (i_this->mode == 0) {
        cage_p = (obj_so_class*)fpcM_Search(s_ori_sub, i_this);
        if (cage_p != NULL) {
            actor->parentActorID = fopAcM_GetID(cage_p);
            anm_init(i_this, 20, 5.0f, 2, 1.0f);
            i_this->mode = 1;
            actor->home.angle.y = cage_p->actor.shape_angle.y;
            i_this->field_0xbde = i_this->target_angle - actor->home.angle.y;
        }

        return rv;
    }

    cage_p = (obj_so_class*)fopAcM_SearchByID(actor->parentActorID);
    if (cage_p == NULL) {
        return rv;
    }

    actor->home.angle.y = cage_p->actor.shape_angle.y;

    if (i_this->mode <= 2 && cage_p->field_0x1054 != 0) {
        i_this->mode = 3;
        i_this->timer[0] = 90;
        int swBit = (fopAcM_GetParam(actor) & 0xFF000000) >> 24;
        dComIfGs_onSwitch(swBit, fopAcM_GetRoomNo(actor));
        dComIfGs_onTbox(i_this->bitTRB);

        if (fopAcM_GetRoomNo(actor) == 4) {
            i_this->demo_mode = 70;
        } else if (fopAcM_GetRoomNo(actor) == 19) {
            i_this->demo_mode = 75;
        } else if (fopAcM_GetRoomNo(actor) == 22) {
            i_this->demo_mode = 72;
        }

        return rv;
    }

    cXyz mae;
    s16 sVar1 = i_this->field_0xbde - 1000 + TREG_S(8) + actor->home.angle.y;
    f32 fVar1 = 0.0f;
    switch (i_this->mode) {
        case 1:
            if (i_this->model->checkFrame(46.0f + TREG_F(11))) {
                i_this->field_0xbde = i_this->target_angle - actor->home.angle.y & -0x2000 + TREG_S(7);
            }

            if (i_this->model->checkFrame(5.0f) || i_this->model->checkFrame(20.0f)) {
                cage_p->field_0xdc8 += 100.0f + KREG_F(9);
                cage_p->field_0xdac = 0;
                i_this->sound.startCreatureSound(Z2SE_OBJ_MONKEYJAIL_CREAK, 0, -1);
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_POINT, -1);
            }

            if (enemy_check(i_this, 400.0f) == NULL) {
                if (daPy_getPlayerActorClass()->getCutAtFlg() == 0 && fpcM_Search(shot_bo_sub, i_this) == NULL) break;
            }

            anm_init(i_this, 41, 5.0f, 2, 1.0f);
            i_this->mode = 2;
            i_this->timer[0] = cM_rndF(20.0f) + 40.0f;
            break;

        case 2:
            sVar1 += NPC_KS_NEG_180DEG_ROT;
            if (i_this->timer[0] == 0) {
                if (cage_p->partBreak()) {
                    anm_init(i_this, 22, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 20, 5.0f, 2, 1.0f);
                }

                i_this->mode = 1;
            } else {
                if (enemy_check(i_this, 400.0f) == NULL) {
                    if (daPy_getPlayerActorClass()->getCutAtFlg() == 0) break;
                }

                i_this->timer[0] = cM_rndF(20.0f) + 40.0f;
            }
            break;

        case 3:
            fVar1 = -20.0f;
            if (i_this->timer[0] == 50) {
                anm_init(i_this, 51, 3.0f, 2, 1.0f);
            }

            if (i_this->timer[0] < 50) {
                cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
                if (i_this->timer[0] < 25) {
                    mae.x = 200.0f;
                } else {
                    mae.x = -200.0f;
                }
                mae.y = 50.0f + KREG_F(11);
                mae.z = 50.0f;
                MtxPosition(&mae, &i_this->find_pos);
                i_this->find_pos += actor->current.pos;
                i_this->search_time = 20;
            }

            if (i_this->timer[0] == 0) {
                anm_init(i_this, 30, 5.0f, 2, 1.0f);
                i_this->mode = 4;
                i_this->timer[0] = 60;
                i_this->search_time = 0;
            }
            break;

        case 4:
            fVar1 = -20.0f;
            i_this->field_0x5fc = 0;
            if (i_this->timer[0] == 0) {
                if (fopAcM_GetRoomNo(actor) == 22) {
                    i_this->action = 116;
                    i_this->mode = 0;
                    i_this->field_0xaec = 1;
                } else if (fopAcM_GetRoomNo(actor) == 4) {
                    i_this->mode = 10;
                } else {
                    i_this->mode = 5;
                }
            }
            break;

        case 5:
            i_this->field_0x5fc = 0;
            fVar1 = -20.0f;
            anm_init(i_this, 32, 2.0f, 0, 1.0f);
            i_this->mode++;
            break;

        case 6:
            i_this->field_0x5fc = 0;
            fVar1 = -20.0f;
            sVar1 += NPC_KS_45DEG_ROT;
            if (i_this->model->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                actor->speedF = 40.0f;
                actor->speed.y = 35.0f;
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                i_this->field_0xbe0 = 1;
                i_this->mode++;
            }
            break;

        case 7:
            i_this->field_0x5fc = 0;
            sVar1 += NPC_KS_90DEG_ROT;
            actor->gravity = -5.0f;
            break;

        case 10:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mode = 11;
            i_this->path_no = 0;
            actor->speedF = 0.0f;
            // fallthrough
        case 11:
            if (guide_path_04[i_this->path_no].field_0x0 != 0) {
                i_this->guide_path.x = guide_path_04[i_this->path_no].field_0x4;
                i_this->guide_path.y = guide_path_04[i_this->path_no].field_0x8;
                i_this->guide_path.z = guide_path_04[i_this->path_no].field_0xc;
                i_this->field_0x910 = actor->current.pos;
                i_this->field_0x91c = i_this->guide_path;
                i_this->mode = 12;
                anm_init(i_this, 32, 2.0f, 0, 1.0f);
                actor->speedF = 0.0f;
            } else {
                i_this->guide_path.x = guide_path_04[i_this->path_no].field_0x4;
                i_this->guide_path.z = guide_path_04[i_this->path_no].field_0xc;
                i_this->mode = 15;
                actor->speedF = l_HIO.demo_speed_2;
                if (i_this->res_id != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 12:
            rv = 2;
            if (i_this->res_id == 32 && i_this->model->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                actor->speedF = 40.0f + TREG_F(9);
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                i_this->field_0xbe0 = 1;
            }

            mae = i_this->guide_path - actor->current.pos;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            i_this->current_angle.x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));
            if (mae.abs() < actor->speedF * 1.2f) {
                actor->current.pos = i_this->guide_path;
                i_this->mode = 11;
                i_this->field_0xbe0 = 1;
                i_this->path_no++;
                if (guide_path_04[i_this->path_no].field_0x0 < 0) {
                    fopAcM_delete(actor);
                }
            }
            break;

        case 15:
            mae.x = i_this->guide_path.x - actor->current.pos.x;
            mae.z = i_this->guide_path.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < (actor->speedF * 1.2f)) {
                i_this->path_no++;
                i_this->mode = 11;
            }
    }

    if (i_this->mode >= 10) {
        i_this->field_0x5fc = 0;
        sVar1 = i_this->current_angle.y;
        if (actor->speedF > 25.0f) {
            i_this->field_0x90c = 1;
        }
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, sVar1, 2, 0x2000);
    
    if (i_this->mode < 7) {
        actor->current.angle.x = cage_p->actor.shape_angle.x;
        actor->current.pos = cage_p->actor.current.pos;
        actor->current.pos.y += fVar1;
    }

    return rv;
}

static void npc_ks_home(npc_ks_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    cXyz mae, ato;
    switch (i_this->mode) {
        case 0:
            if (cM_rndF(1.0f) < 0.5f) {
                anm_init(i_this, 30, 3.0f, 0, 1.0f);
            } else {
                anm_init(i_this, 5, 3.0f, 0, 1.0f);
            }

            i_this->mode = 1;
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_WAIT, -1);
            break;

        case 1:
            if (i_this->model->isStop()) {
                i_this->mode = 0;
            }
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->target_angle, 2, 0x800);
    if (checkDoorDemo() != 0) {
        int room_no = fopAcM_GetRoomNo(actor);
        int stay_no = dStage_roomControl_c::getNextStayNo();
        if (room_no == 0 && stay_no == 4) {
            i_this->action = 100;
            i_this->mode = 0;
            i_this->path_no = 0;
            i_this->field_0xaec = 1;
            if (fopAcM_CheckCondition(actor, 4) != 0) {
                camera_class* camera = dComIfGp_getCamera(0);
                mae.x = camera->lookat.eye.x - camera->lookat.center.x;
                mae.z = camera->lookat.eye.z - camera->lookat.center.z;
                cMtx_YrotS(*calc_mtx, cM_atan2s(mae.x, mae.z));
                if ((i_this->set_id & 1) != 0) {
                    mae.x = 100.0f;
                } else {
                    mae.x = -100.0f;
                }
                mae.y = -50.0f;
                mae.z = 200.0f;
                MtxPosition(&mae, &ato);
                actor->current.pos = camera->lookat.eye + ato;
                actor->old = actor->current;
            }
        }
    }
}

static void* s_sw_sub(void* i_actor, void* i_data) {
    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_SW)) {
        if (fopAcM_GetRoomNo((fopAc_ac_c*)i_data) == 0) {
            if (((npc_ks_class*)i_data)->set_id == ((obj_sw_class*)i_actor)->field_0x570) {
                return i_actor;
            }
        } else {
            return i_actor;
        }
    }
    return NULL;
}

static void* s_ha_sub(void* i_actor, void* i_data) {
    (void) i_data;
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_Pillar) {
        return i_actor;
    }
    return NULL;
}

static void* s_01_sub(void* i_actor, void* i_data) {
    (void) i_data;
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KS && ((npc_ks_class*)i_actor)->action == 4) {
        return i_actor;
    }
    return NULL;
}

static path move_path_02[5] = {
    { 0, 10910.0f, 3748.0f, 4630.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 0, 11002.0f, 3748.0f, 4085.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 0, 11474.0f, 3748.0f, 3709.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 0, 12007.0f, 3698.0f, 3553.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 12103.0f, 3148.0f, 4760.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

static int npc_ks_demo_02(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* unused_p = dComIfGp_getPlayer(0);
    cXyz mae, ato;
    f32 speed = 0.0f;
    int rv = 1;
    int iVar1 = 0;

    obj_so_class* cage_p = (obj_so_class*)fopAcM_SearchByName(PROC_OBJ_SO);
    if (cage_p == NULL) {
        return 0;
    }
    
    if (cage_p->field_0xdae != 0 && i_this->mode < 17) {
        i_this->action = 201;
        i_this->mode = 0;
        return 0;
    }
    
    daPillar_c* hasira_p = (daPillar_c*)fpcM_Search(s_ha_sub, i_this);
    if (hasira_p == NULL) {
        return 0;
    }
    fopAc_ac_c* hasira_actor = hasira_p;

    switch (i_this->mode) {
        int _;
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->path_no = 0;
            // fallthrough
        case 1:
            i_this->guide_path.x = move_path_02[i_this->path_no].field_0x4;
            i_this->guide_path.y = move_path_02[i_this->path_no].field_0x8;
            i_this->guide_path.z = move_path_02[i_this->path_no].field_0xc;
            i_this->mode = 2;
            // fallthrough
        case 2:
            speed = l_HIO.demo_speed;
            mae = i_this->guide_path - actor->current.pos;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 2, 0x1000);
            mae.y = 0.0f;
            if (mae.abs() <= actor->speedF * 2.0f) {
                i_this->path_no++;
                if (i_this->path_no < 5) {
                    i_this->mode = 1;
                } else {
                    i_this->mode = 10;
                    anm_init(i_this, 25, 5.0f, 2, 1.0f);
                    i_this->timer[0] = 35;
                }
            }
            break;

        case 10:
            cLib_addCalcAngleS2(&actor->current.angle.y, cage_p->actor.shape_angle.y + 0x8000, 2, 0x1000);
            if (i_this->timer[0] == 0) {
                i_this->mode = 11;
                anm_init(i_this, 26, 5.0f, 2, 1.0f);
            }
            break;

        case 11:
            rv = 0;
            speed = l_HIO.demo_speed;
            cMtx_YrotS(*calc_mtx, cage_p->actor.shape_angle.y);
            mae.x = 0.0f;
            mae.y = 0.0f;
            mae.z = (90.0f + NREG_F(1)) * cage_p->actor.scale.x;
            MtxPosition(&mae, &ato);
            ato += cage_p->actor.current.pos;
            cLib_addCalc2(&actor->current.pos.x, ato.x, 1.0f, actor->speedF);
            cLib_addCalc2(&actor->current.pos.z, ato.z, 1.0f, actor->speedF);
            mae.x = ato.x - actor->current.pos.x;
            mae.z = ato.z - actor->current.pos.z;
            mae.y = JMAFastSqrt(mae.x * mae.x + mae.z * mae.z);
            if (mae.y < 55.0f && i_this->res_id != 12) {
                anm_init(i_this, 12, 3.0f, 2, 1.0f);
            }

            if (mae.y < 1.0f + NREG_F(0)) {
                i_this->mode = 12;
                actor->current.angle.y = cage_p->actor.shape_angle.y + 0x8000;
            }
            break;

        case 12:
            rv = 0;
            iVar1 = 1;
            speed = l_HIO.demo_speed;
            cMtx_YrotS(*calc_mtx, cage_p->actor.shape_angle.y);
            mae.x = 0.0f;
            mae.y = 0.0f;
            mae.z = 90.0f + NREG_F(1);
            MtxPosition(&mae, &ato);
            ato += cage_p->actor.current.pos;
            cLib_addCalc2(&actor->current.pos.x, ato.x, 1.0f, actor->speedF);
            cLib_addCalc2(&actor->current.pos.y, ato.y, 1.0f, actor->speedF);
            cLib_addCalc2(&actor->current.pos.z, ato.z, 1.0f, actor->speedF);
            mae = ato - actor->current.pos;
            if (mae.abs() < 1.0f + NREG_F(0)) {
                i_this->mode = 13;
                anm_init(i_this, 23, 2.0f, 2, 1.0f);
                i_this->timer[0] = 60;
            }
            break;

        case 13:
            rv = 0;
            iVar1 = 1;
            cMtx_YrotS(*calc_mtx, cage_p->actor.shape_angle.y);
            mae.x = 0.0f;
            mae.y = 0.0f;
            mae.z = (90.0f + NREG_F(1)) * cage_p->actor.scale.x;
            MtxPosition(&mae, &ato);
            ato += cage_p->actor.current.pos;
            cLib_addCalc2(&actor->current.pos.x, ato.x, 1.0f, 10.0f);
            cLib_addCalc2(&actor->current.pos.z, ato.z, 1.0f, 10.0f);
            
            if (i_this->model->checkFrame(10.0f) || i_this->model->checkFrame(20.0f)) {
                cage_p->field_0xdc8 += 200.0f;
                i_this->sound.startCreatureSound(Z2SE_OBJ_MONKEYJAIL_CREAK, 0, -1);
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_POINT, -1);
            }

            if (i_this->timer[0] == 0) {
                i_this->mode = 14;
                anm_init(i_this, 15, 2.0f, 2, 1.0f);
            }
            break;

        case 14:
            rv = 0;
            iVar1 = 1;
            cMtx_YrotS(*calc_mtx, cage_p->actor.shape_angle.y);
            mae.x = 0.0f;
            mae.y = 0.0f;
            mae.z = 90.0f + NREG_F(1);
            MtxPosition(&mae, &ato);
            ato += cage_p->actor.current.pos;
            cLib_addCalc2(&actor->current.pos.x, ato.x, 1.0f, 10.0f);
            cLib_addCalc2(&actor->current.pos.z, ato.z, 1.0f, 10.0f);
            cLib_addCalc2(&actor->current.pos.y, i_this->ObjAcch.GetGroundH(), 1.0f, l_HIO.demo_speed);
            
            if (NPC_KS_FABSF(actor->current.pos.y - i_this->ObjAcch.GetGroundH()) < 1.0f) {
                anm_init(i_this, 26, 5.0f, 2, 1.0f);
                actor->speedF = 0.0f;
                i_this->timer[0] = 20;
                i_this->mode = 15;
            }
            break;

        case 15:
            speed = l_HIO.demo_speed;
            cLib_addCalcAngleS2(&actor->current.angle.y, s16(cage_p->actor.shape_angle.y), 2, 0x2000);
            if (i_this->timer[0] == 0) {
                i_this->mode = 16;
                anm_init(i_this, 25, 5.0f, 2, 1.0f);
            }
            break;

        case 16: {
            i_this->field_0xaec = 2;
            i_this->field_0x5fc = 1;
            if (i_this->timer[0] == 0) {
                if (i_this->res_id != 25) {
                    anm_init(i_this, 25, 5.0f, 2, 1.0f);
                    i_this->timer[0] = 25;
                } else {
                    anm_init(i_this, 50, 5.0f, 2, 1.0f);
                    i_this->timer[0] = cM_rndF(30.0f) + 30.0f;
                }
            }

            cLib_addCalcAngleS2(&actor->current.angle.y, cage_p->actor.shape_angle.y + 0x8000, 2, 0x1000);
            if (dComIfGp_checkPlayerStatus0(0, 0x40) != 0 || dComIfGp_checkPlayerStatus0(0, 0x2000) != 0) {
                i_this->timer[2] = 20;
            }

            if (i_this->timer[2] == 0 && cage_p->field_0xdc4 == 0) {
                fopAcM_OnStatus(actor, 0);
                cLib_onBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_TALKCHECK_e | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
                actor->eventInfo.onCondition(dEvtCnd_CANTALK_e);
                actor->attention_info.distances[fopAc_attn_TALK_e] = 2;
                actor->attention_info.distances[fopAc_attn_SPEAK_e] = 2;
            }

            dAttention_c* attention_p = dComIfGp_getAttention();
            if (actor == attention_p->LockonTarget(0) && i_this->timer[2] == 0 
                && mDoCPd_c::getTrigA(PAD_1) != 0) {
                i_this->mode = 17;
                anm_init(i_this, 25, 2.0f, 0, 1.0f);
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                actor->speedF = 0.0f;
                leader->demo_mode = 60;
            }
            break;
        }

        case 17:
            rv = 0;
            if (i_this->res_id == 25) {
                if (i_this->model->isStop()) {
                    anm_init(i_this, 49, 3.0f, 0, 1.0f);
                }
            } else {
                speed = 30.0f;
            }

            if (i_this->res_id == 49 && i_this->model->isStop()) {
                anm_init(i_this, 38, 1.0f, 2, 1.0f);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_ROLL, 0, -1);
            }

            mae = hasira_actor->current.pos - actor->current.pos;
            actor->current.angle.y = cM_atan2s(mae.x, mae.z);
            cMtx_YrotS(*calc_mtx, actor->current.angle.y);
            mae.x = 0.0f;
            mae.y = 0.0f;
            mae.z = actor->speedF;
            MtxPosition(&mae, &ato);
            actor->current.pos += ato;
            mae.x = hasira_actor->current.pos.x - actor->current.pos.x;
            mae.z = hasira_actor->current.pos.z - actor->current.pos.z;
            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < 90.0f + NREG_F(7)) {
                hasira_p->setShake(daPillar_c::SHAKE_STRONG);
                anm_init(i_this, 6, 5.0f, 0, 1.0f);
                i_this->mode = 18;
                actor->speedF = -20.0f + NREG_F(4);
                actor->speed.y = 30.0f + NREG_F(5);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_ATTACK_POLL, 0, -1);
                if (cage_p != NULL) {
                    cage_p->field_0xdc8 = 1500.0f + NREG_F(8);
                    i_this->sound.startCreatureSound(Z2SE_OBJ_MONKEYJAIL_SHAKE, 0, -1);
                }
            }
            break;

        case 18:
            speed = -20.0f + NREG_F(4);
            cLib_addCalcAngleS2(&actor->current.angle.x, ZREG_S(7) - 0x3830, 1, 0x600);
            if (actor->speed.y <= 0.0f && i_this->ObjAcch.ChkGroundHit()) {
                i_this->mode = 19;
                actor->speed.y = 20.0f + NREG_F(6);
                actor->current.angle.x = ZREG_S(7) - 0x3830;
                i_this->timer[0] = 40;
                i_this->timer[1] = 10;
                fopAcM_effSmokeSet1(&i_this->field_0x858, &i_this->field_0x85c, &actor->eyePos, NULL, 1.7f + TREG_F(18), &actor->tevStr, 1);
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_CRUSHED, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_FALL_GROUND, 0, -1);
                anm_init(i_this, 13, 2.0f, 2, 1.0f);
            }
            break;

        case 19:
            if (i_this->timer[1] != 0) {
                speed = actor->speedF = -15.0f + NREG_F(14);
            } else {
                speed = actor->speedF = 0.0f;
            }

            if (i_this->timer[0] == 0) {
                anm_init(i_this, 35, 3.0f, 0, 1.0f);
                i_this->mode = 20;
            }
            break;

        case 20:
            cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);
            if (i_this->model->isStop()) {
                i_this->mode = 16;
                anm_init(i_this, 25, 5.0f, 2, 1.0f);
                if (leader->demo_mode != 0) {
                    leader->demo_mode = 100;
                }
                i_this->timer[2] = 30;
            }
            break;
    }

    cLib_addCalc2(&actor->speedF, speed, 1.0f, 4.0f);
    if (checkDoorDemo() != 0) {
        i_this->action = 100;
        i_this->mode = 0;
    } else if (iVar1 != 0 && cage_p->actor.shape_angle.x > 0x200) {
        anm_init(i_this, 6, 5.0f, 0, 1.0f);
        i_this->mode = 18;
        actor->speedF = -8.0f + NREG_F(4);
        actor->speed.y = 10.0f + NREG_F(5);
        i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_PRISONED, -1);
    }

    return rv;
}

static void* s_dn_sub(void* i_actor, void* i_data) {
    (void) i_data;
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_OC) {
        return i_actor;
    }
    return NULL;
}

static int npc_ks_demo_022(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz unused_cxyz_0, unused_cxyz_1;
    f32 speed = 0.0f;     // doesn't change
    BOOL rt = TRUE;     // doesn't change
    daE_OC_c* bokoblin_p = (daE_OC_c*)fpcM_Search(s_dn_sub, i_this);
    int frame = i_this->model->getFrame();

    switch(i_this->mode) {
        case 0:
            anm_init(i_this, 51, 5.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->timer[0] = 100;
            // fallthrough
        case 1:
            i_this->field_0x5fc = 1;
            if (i_this->timer[0] == 0) {
                i_this->mode = 20;
                anm_init(i_this, 30, 3.0f, 2, 1.0f);
                i_this->timer[0] = 90;
                fpcM_Search(s_resq_sub, i_this);
            }
            break;

        case 10:
            anm_init(i_this, 13, 2.0f, 2, 1.0f);
            i_this->timer[0] = 30;
            i_this->mode = 11;
            actor->current.angle.x = ZREG_S(7) - 0x3830;
            // fallthrough
        case 11:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, 35, 3.0f, 0, 1.0f);
                i_this->mode = 12;
                mDoAud_seStart(12, 0, 0, 0);
                Z2GetAudioMgr()->muteSceneBgm(120, 0.0f);
                Z2GetAudioMgr()->setBattleBgmOff(true);
            }
            break;

        case 12:
            cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);
            if (i_this->model->isStop()) {
                i_this->mode = 20;
                anm_init(i_this, 30, 3.0f, 2, 1.0f);
                i_this->timer[0] = 90;
                fpcM_Search(s_resq_sub, i_this);
            }
            break;

        case 20:
            if (i_this->timer[0] == 0) {
                i_this->mode = 21;
            }
            break;

        case 21:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mode = 22;
            i_this->timer[0] = cM_rndF(80.0f) + 100.0f;
            i_this->field_0xaec = 1;
            break;

        case 22:
            if (((i_this->res_id == 39 || i_this->res_id == 43 && frame < 7) ||
                (frame > 40 && frame < 48) || frame > 65)) {
                i_this->field_0x5e0 = 4;
            }

            if ((i_this->timer[0] == 0) && (i_this->res_id == 39)) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->res_id == 43) {
                if (i_this->model->isStop()) {
                    i_this->mode = 21;
                }
            }

            if (bokoblin_p == NULL) {
                i_this->timer[0] = cM_rndF(10.0f) + 30.0f;
                i_this->mode = 23;
                int swBit = (fopAcM_GetParam(actor) & 0xFF000000) >> 24;
                dComIfGs_onSwitch(swBit, fopAcM_GetRoomNo(actor));
            }
            break;

        case 23:
            if (i_this->timer[0] == 0) {
                if (i_this->set_id == 0) {
                    anm_init(i_this, 56, 3.0f, 2, 1.0f);
                }
                else {
                    anm_init(i_this, 11, 3.0f, 2, 1.0f);
                }
                i_this->timer[0] = cM_rndF(25.0f) + 70.0f;
                i_this->mode = 24;
            }
            break;

        case 24:
            if (i_this->timer[0] == 0) {
                i_this->action = 114;
                i_this->mode = 0;
                i_this->timer[2] = 60;
            }
    }
    
    cLib_addCalc2(&actor->speedF, speed, 1.0f, 4.0f);
    return rt;
}

static void npc_ks_demo_04(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz unused_cxyz_0, unused_cxyz_1;
    f32 speed = 0.0f;
    f32 step_speed = 10.0f;
    switch(i_this->mode) {
        case 0:
            anm_init(i_this, 51, 5.0f, 2, 1.0f);
            i_this->mode = 1;
            break;

        case 2:
            actor->current.pos.set(0.0f, 3300.0f, 4393.0f);
            actor->current.angle.y = -0x8000;
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mode = 3;
            break;

        case 3:
            speed = l_HIO.demo_speed;
            if (actor->field_0x567 != 0) {
                actor->field_0x567 = 0;
                anm_init(i_this, 45, 3.0f, 0, 1.0f);
            }

            if (i_this->res_id == 45) {
                if (i_this->model->checkFrame(20.0f)) {
                    mDoAud_seStart(0x5001a, 0, 0, 0);
                }

                speed = 0.0f;
                step_speed = 4.0f;
                if (i_this->model->getFrame() >= 37.0f + AREG_F(17)) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }

            break;

        case 4:
            i_this->mode = 5;
            anm_init(i_this, 33, 5.0f, 0, 1.0f);
            mDoAud_seStart(0x5001a, 0, 0, 0);
            actor->speed.y = 0.0f;
            break;

        case 5:
            speed = l_HIO.demo_speed;
            actor->gravity = -2.0f + KREG_F(7);
            break;

        case 6:
            anm_init(i_this, 12, 3.0f, 2, 1.0f);
            i_this->mode = 7;
            // fallthrough
        case 7:
            actor->gravity = 0.0f;
            actor->speed.y = 10.0f;
            if ((i_this->count & 31) == 0) {
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_WAIT, -1);
            }
            break;

        case 8:
            anm_init(i_this, 26, 5.0f, 2, 1.0f);
            i_this->timer[0] = 35;
            i_this->mode = 9;
            break;

        case 9:
            speed = l_HIO.demo_speed;
            step_speed = 4.0f;
            if (i_this->timer[0] == 0) {
                anm_init(i_this, 37, 3.0f, 0, 1.0f);
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_RELEIEF, -1);
                i_this->field_0x5e4 = 1;
                i_this->mode = 10;
            }
            break;

        case 10:
            i_this->field_0x5fc = 1;
            if (i_this->model->isStop()) {
                i_this->field_0x5e4 = 0;
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
                i_this->mode = 11;
                i_this->timer[0] = 25 + KREG_S(7);
            }
            break;

        case 11:
            speed = l_HIO.demo_speed;
            step_speed = 4.0f;
            if (i_this->timer[0] == 0) {
                anm_init(i_this, 7, 5.0f, 2, 1.0f);
                i_this->timer[0] = 120;
                i_this->mode = 12;
            }
            break;

        case 12:
            cLib_addCalcAngleS2(&actor->current.angle.y, i_this->target_angle, 2, 0x1000);
            if (i_this->timer[0] == 0) {
                i_this->action = 100;
                i_this->mode = 0;
            }
            break;

        case 20:
            anm_init(i_this, 51, 5.0f, 2, 1.0f);
            i_this->mode = 21;
            // fallthrough
        case 21:
            if (actor->field_0x567 != 0) {
                actor->field_0x567 = 0;
                i_this->mode = 22;
                anm_init(i_this, 45, 3.0f, 0, 1.0f);
            }
            break;

        case 22:
            if (i_this->model->isStop()) {
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
            }

            if (i_this->res_id == 26) {
                cLib_addCalcAngleS2(&actor->current.angle.y, 0, 2, 0x1000);
                if (actor->current.angle.y < 0x1000 && actor->current.angle.y > -0x1000) {
                    speed = l_HIO.demo_speed;
                    step_speed = 10.0f;
                }
            }
    }

    cLib_addCalc2(&actor->speedF, speed, 1.0f, step_speed);
}

static void npc_ks_to_hang(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz mae, ato;
    cXyz sp3c(i_this->field_0xbc8);
    
    switch (i_this->mode) {
        case 0:
            i_this->guide_path = sp3c;
            i_this->field_0x910 = actor->current.pos;
            i_this->field_0x91c = i_this->guide_path;
            anm_init(i_this, 32, 2.0f, 0, 1.0f);
            actor->speedF = 0.0f;
            i_this->mode++;
            break;

        case 1:
            mae = i_this->guide_path - actor->current.pos;
            if (i_this->res_id == 32) {
                actor->speedF = 0.0f;
                if (i_this->model->isStop()) {
                    anm_init(i_this, 33, 1.0f, 0, 1.0f);
                    actor->speedF = 40.0f;
                    i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                    i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                    i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                    i_this->field_0xbe0 = 1;
                }
            }

            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            i_this->current_angle.x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));
            cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 1, 0x2000);
            if (mae.abs() <= actor->speedF * 1.1f) {
                i_this->mode = 10;
                obj_sw_class* sw_p = i_this->child_no;
                if (sw_p != NULL) {
                    if (sw_p->field_0x91c == 1) {
                        i_this->field_0x630 = sw_p->field_0xd8c / 2;
                    } else {
                        i_this->field_0x630 = ((i_this->set_id + 1) * (sw_p->field_0xd8c / (sw_p->field_0x91c + 1)) - 1);
                    }

                    actor->parentActorID = fopAcM_GetID(sw_p);

                    actor->current.pos = i_this->guide_path;
                    i_this->field_0x910 = actor->current.pos;
                    anm_init(i_this, 32, 2.0f, 0, 1.0f);
                    i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
                }
            }
            break;
            
        case 2:
            if (i_this->model->isStop()) {
                i_this->mode = 0;
            }
            break;

        case 10:
            obj_sw_class* sw_p = (obj_sw_class*)fopAcM_SearchByID(actor->parentActorID);
            i_this->guide_path = sw_p->field_0x920[i_this->field_0x630];
            i_this->field_0x91c = i_this->guide_path;
            mae = i_this->guide_path - actor->current.pos;
            if (i_this->res_id == 32) {
                actor->speedF = 0.0f;
                if (i_this->model->isStop()) {
                    anm_init(i_this, 33, 1.0f, 0, 1.0f);
                    actor->speedF = 30.0f;
                    i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                    i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                    i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                    i_this->field_0xbe0 = 1;
                }
            }

            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            i_this->current_angle.x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));
            if (mae.abs() <= actor->speedF* 1.1f) {
                actor->current.pos = i_this->guide_path;
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
                if (i_this->child_no->field_0x91c == 1) {
                    i_this->action = 11;
                    i_this->mode = 0;
                } else {
                    i_this->action = 21;
                    i_this->mode = 0;
                    i_this->timer[2] = 15;
                }
            }
    }

    if (actor->speedF> 25.0f) {
        i_this->field_0x90c= 1;
    }
}

static void* s_next_do_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KS) {
        npc_ks_class* mon_data = (npc_ks_class*) i_data;
        npc_ks_class* monkey_actor = (npc_ks_class*) i_actor;
        if (monkey_actor->field_0x5b5 != 0 && mon_data->order == (monkey_actor->order - 1 & 0xFF)) {
            monkey_actor->mode++;
            return i_actor;
        }
    }

    return NULL;
}

static void* s_next_get_sub(void* actor, void* i_data) {
    if (fopAcM_IsActor(actor) && fopAcM_GetName(actor) == PROC_NPC_KS) {
        npc_ks_class* mon_data = (npc_ks_class*) i_data;
        npc_ks_class* monkey_actor = (npc_ks_class*) actor;
        if (monkey_actor->field_0x5b5 != 0 &&
        monkey_actor->order != mon_data->order && monkey_actor->field_0x620 != 0) {
            monkey_actor->field_0x620 = 0;
            anm_init(monkey_actor, 24, 1.0f, 2, 1.0f);
            mon_data->field_0x620 = 1;
            mon_data->field_0x624 = mon_data->field_0x614 - monkey_actor->field_0x614;
            mon_data->field_0x624 *= (0.1f + BREG_F(5));
            mon_data->field_0x634 = 10.0f + BREG_F(10);
            return actor;
        }
    }

    return NULL;
}

static void order_set(int param_1) {
    if (saru_p[0]->dis < saru_p[param_1 - 1]->dis) {
        for (int i = 0; i < param_1; i++) {
            saru_p[i]->order = i;
        }
    } else {
        for (int i = 0; i < param_1; i++) {
            saru_p[i]->order = (param_1 - 1) - i;
        }
    }
}

static void all_carry_finish(int param_1) {
    for (int i = 0; i < param_1; i++) {
        saru_p[i]->action = 10;
        saru_p[i]->mode = 0;
    }
}

static void hang_end_check(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz mae, ato;
    if (i_this->monkey_room_no > 3 || !dComIfGs_isStageMiddleBoss()) {
        if ((fopAcM_GetRoomNo(actor) != 4 || !(player->current.pos.z < 2500.0f)) && checkDoorDemo()) {
            i_this->action = 100;
            i_this->mode = 0;
            i_this->path_no = 0;
            i_this->field_0xaec = 1;
            actor->current.angle.x = 0;
            if (fopAcM_CheckCondition(actor, 4) != 0) {
                camera_class* camera = dComIfGp_getCamera(0);
                if (checkDoorDemo()) {
                    cMtx_YrotS(*calc_mtx, player->shape_angle.y + 0x8000);
                } else {
                    mae.x = camera->lookat.eye.x - camera->lookat.center.x;
                    mae.z = camera->lookat.eye.z - camera->lookat.center.z;
                    cMtx_YrotS(*calc_mtx, cM_atan2s(mae.x, mae.z));
                }

                if ((i_this->set_id & 1) != 0) {
                    mae.x = 100.0f;
                } else {
                    mae.x = -100.0f;
                }
                mae.y = -50.0f;
                mae.z = 100.0f;
                MtxPosition(&mae, &ato);
                actor->current.pos = camera->lookat.eye + ato;
                actor->old = actor->current;
            }
        }
    }
}

static void npc_ks_hang(npc_ks_class* i_this) {
    if (leader == NULL) {
        return;
    }

    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* actor = &i_this->actor;
    if (fopAcM_GetRoomNo(actor) == 4) {
        for (int i = 0; i < 4; i++) {
            if (saru_p[i] == NULL) {
                return;
            }
        }

        if (player->current.pos.y < 2800.0f) {
            cDmr_SkipInfo = 4;
        }
    }

    fopAc_ac_c* actor_p;
#if VERSION == VERSION_SHIELD_DEBUG
    // FIXME: fakematch?
    actor_p = fopAcM_SearchByID(actor->parentActorID);
    if (actor_p == NULL)
#else
    if ((actor_p = fopAcM_SearchByID(actor->parentActorID)) == NULL)
#endif
    {
        actor_p = (fopAc_ac_c*) fpcM_Search(s_sw_sub, i_this);
        if (actor_p != NULL) {
            actor->parentActorID = fopAcM_GetID(actor_p);
            i_this->child_no = (obj_sw_class*) actor_p;
            i_this->field_0x630 = (i_this->set_id + 1) * (i_this->child_no->field_0xd8c / (i_this->child_no->field_0x91c + 1)) - 1;
        } else {
            return;
        }
    }
    obj_sw_class* sw_p = (obj_sw_class*) actor_p;
    sw_p->field_0x900 += 5.0f;

    cXyz mae, ato;
    int iVar1 = 0;

    cLib_addCalcAngleS2(&actor->current.angle.y, actor->home.angle.y + 0x4000, 2, 0x800);
    static s16 start_pya = 0;
    if (!daPy_getPlayerActorClass()->checkAutoJumpStart()) {
        start_pya = i_this->target_angle;
    }

    s16 sVar1;
    switch (i_this->mode) {
        case 0:
            i_this->timer[0] = 0;
            i_this->mode++;
            i_this->field_0x604 = 0;
            // fallthrough
        case 1:
            if (i_this->set_id == 0 && leader->demo_mode == 0) {
                order_set(sw_p->field_0x91c);
            }

            if (i_this->order == 0 && i_this->res_id != 16 && i_this->dis < 550.0f + YREG_F(9)) {
                anm_init(i_this, 16, 5.0f, 2, 1.0f);
                i_this->timer[0] = cM_rndF(50.0f) + 200.0f;
            }

            if (i_this->timer[0] == 0) {
                i_this->timer[0] = cM_rndF(30.0f) + 50.0f;
                if (cM_rndF(1.0f) < 0.3f) {
                    anm_init(i_this, 17, 5.0f, 2, 1.0f);
                } else if (cM_rndF(1.0f) < 0.3f) {
                    anm_init(i_this, 18, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 19, 5.0f, 2, 1.0f);
                }
            }

            leader->field_0x92c = 0;
            if (i_this->order == 0 && !dComIfGp_event_runCheck() && i_this->hang_time == 0 && i_this->dis < 150.0f + YREG_F(0)) {
                leader->demo_mode = 1;
                i_this->field_0x620 = 1;
                i_this->field_0x610 = 0;
                obj_pos = player->current.pos;
                cMtx_YrotS(*calc_mtx, actor->home.angle.y);
                mae.x = 0.0f;
                mae.y = -150.0f + WREG_F(0);
                mae.z = -400.0f + WREG_F(1);
                MtxPosition(&mae, &obj_pos);
                obj_pos += actor->current.pos;
                i_this->field_0x624 = obj_pos - i_this->field_0x614;
                i_this->field_0x624 *= (0.075f + WREG_F(2));
                i_this->mode = 4;
                i_this->timer[0] = 17 + WREG_S(8);
                i_this->timer[1] = 25 + WREG_S(9);
                i_this->field_0x5fa = 0x4000;
                i_this->field_0x60c = 8000.0f;
                i_this->field_0x604 = 0;
                anm_init(i_this, 24, 3.0f, 2, 1.0f);
            }

            sVar1 = start_pya - sw_p->actor.current.angle.y;
            if (sVar1 < 0x4000 && sVar1 > -0x4000) {
                actor->home.angle.y = sw_p->actor.current.angle.y + 0x8000;
            } else {
                actor->home.angle.y = sw_p->actor.current.angle.y;
            }
            break;

        case 2:
            i_this->mode++;
            i_this->timer[0] = 10 + BREG_S(6);
            // fallthrough
        case 3:
            i_this->field_0x5fa = 0x4000;
            i_this->field_0x60c = 8000.0f;
            if (i_this->timer[0] == s16(8 + BREG_S(7))) {
                fpcM_Search(s_next_get_sub, i_this);
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_THROW, -1);
            }

            if (i_this->timer[0] == s16(2 + ZREG_S(6))) {
                i_this->timer[2] = 15 + ZREG_S(7);
            }

            if (i_this->timer[0] == 0) {
                i_this->mode++;
                i_this->timer[0] = 10 + BREG_S(8);
                i_this->timer[1] = 18 + BREG_S(4);
            }
            break;

        case 4:
            if (i_this->timer[0] > s16(8 + BREG_S(5))) {
                i_this->field_0x5fa = 0x4000;
                i_this->field_0x60c = 8000.0f;
            }

            if (i_this->timer[0] == s16(8 + BREG_S(5))) {
                anm_init(i_this, 48, 2.0f, 2, 1.0f);
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_CATCH, -1);
                i_this->sound.startCreatureSound(Z2SE_FN_ROPE_CREAK, 0, -1);
                sw_p->field_0x8fc = 20.0f + XREG_F(0);
                sw_p->field_0xd10[i_this->field_0x630] = 10;
            }

            if (mDoCPd_c::getTrigA(0) != 0) {
                if (i_this->timer[1] <= 9 && i_this->field_0x604 < 0 && i_this->field_0x602 < 0) {
                    leader->field_0x92c = 1;
                    fpcM_Search(s_next_do_sub, i_this);
                    if (i_this->order == sw_p->field_0x91c -1) {
                        i_this->mode = 5;
                    } else {
                        i_this->mode = 10;
                        anm_init(i_this, 24, 1.0f, 2, 1.0f);
                    }
                } else {
                    leader->demo_mode = 100;
                    i_this->field_0x620 = 0;
                    iVar1 = 1;
                    i_this->hang_time = 20;
                }
            }
            break;

        case 5:
            if (i_this->field_0x602 >= 0 || i_this->field_0x604 >= 0) break;

            i_this->field_0x620 = 0;
            i_this->timer[0] = 15 + BREG_S(3);
            i_this->mode = 6;
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_THROW, -1);
            player->changeDemoMode(24, 0, 0, 0);
            anm_init(i_this, 24, 1.0f, 2, 1.0f);
            // fallthrough
        case 6:
            if (i_this->timer[0] == 8) {
                leader->demo_mode = 3;
                leader->demo_camera_no = 0;
            }

            if (i_this->timer[0] == 1) {
                iVar1 = 1;
            }
            break;

        case 10:
            break;
    }

    actor->current.pos = sw_p->field_0x920[i_this->field_0x630];
    s16 sVar2 = i_this->field_0x602;
    cLib_addCalcAngleS2(&i_this->field_0x602, i_this->field_0x60c * cM_ssin(i_this->field_0x5fa), 4, 0x1000);
    i_this->field_0x604 = i_this->field_0x602 - sVar2;
    i_this->field_0x5fa += (s16) 0x800;
    actor->current.angle.z = -(i_this->field_0x602 / 4);

    if (i_this->mode != 4) {
        cLib_addCalc0(&i_this->field_0x60c, 0.5f, 100.0f + TREG_F(3));
    }

    hang_end_check(i_this);
    if (iVar1 != 0) {
        i_this->field_0x620 = 0;
        i_this->mode = 0;
        all_carry_finish(sw_p->field_0x91c);
    }
}

static void npc_ks_hang_s(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* base_sw_p;
#if VERSION == VERSION_SHIELD_DEBUG
    // FIXME: fakematch?
    base_sw_p = fopAcM_SearchByID(actor->parentActorID);
    if (base_sw_p == NULL)
#else
    if ((base_sw_p = fopAcM_SearchByID(actor->parentActorID)) == NULL)
#endif
    {
        base_sw_p = (fopAc_ac_c*) fpcM_Search(s_sw_sub, i_this);
        if (base_sw_p != NULL) {
            actor->parentActorID = fopAcM_GetID(base_sw_p);
            i_this->child_no = (obj_sw_class*) base_sw_p;
        } else {
            return;
        }
    }

    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    obj_sw_class* sw_p = (obj_sw_class*) base_sw_p;
    sw_p->field_0x900 += 5.0f;
    cXyz mae, ato;
    cLib_addCalcAngleS2(&actor->current.angle.y, actor->home.angle.y + 0x4000, 2, 0x800);

    s16 sVar1;
    switch (i_this->mode) {
        case 0:
            int asdf;
            i_this->mode++;
            i_this->sound.startCreatureSound(Z2SE_FN_ROPE_CREAK, 0, -1);
            sw_p->field_0x8fc = 20.0f + XREG_F(0);
            sw_p->field_0xd10[i_this->field_0x630] = 10;
            i_this->field_0x92c = 0;
            break;

        case 1:
            if (i_this->res_id != 16 && i_this->dis < 550.0f + YREG_F(9)) {
                anm_init(i_this, 16, 5.0f, 2, 1.0f);
                i_this->timer[0] = cM_rndF(50.0f) + 200.0f;
            }

            if (i_this->timer[0] == 0) {
                i_this->timer[0] = cM_rndF(30.0f) + 50.0f;
                if (cM_rndF(1.0f) < 0.3f) {
                    anm_init(i_this, 17, 5.0f, 2, 1.0f);
                } else if (cM_rndF(1.0f) < 0.3f) {
                    anm_init(i_this, 18, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 19, 5.0f, 2, 1.0f);
                }
            }

            if (i_this->hang_time == 0 && !dComIfGp_event_runCheck() && i_this->dis < 300.0f + YREG_F(10)) {
                leader->demo_mode = 1;
                i_this->field_0x620 = 1;
                i_this->field_0x610 = 0;
                cMtx_YrotS(*calc_mtx, actor->home.angle.y);
                mae.x = 0.0f;
                mae.y = -150.0f + WREG_F(0);
                mae.z = -400.0f + WREG_F(1);
                MtxPosition(&mae, &obj_pos);
                obj_pos += actor->current.pos;
                i_this->field_0x624 = obj_pos - i_this->field_0x614;
                i_this->field_0x624 *= (0.075f + WREG_F(2));

                i_this->mode = 2;
                i_this->timer[0] = 17 + WREG_S(8);
                i_this->timer[1] = 25 + WREG_S(9);
                i_this->field_0x5fa = 0x4000;
                i_this->field_0x60c = 8000.0f;
                anm_init(i_this, 24, 3.0f, 2, 1.0f);
            }

            sVar1 = i_this->target_angle - sw_p->actor.current.angle.y;
            if (sVar1 < 0x4000 && sVar1 > -0x4000) {
                actor->home.angle.y = sw_p->actor.current.angle.y + 0x8000;
            } else {
                actor->home.angle.y = sw_p->actor.current.angle.y;
            }
            break;

        case 2:
            if (i_this->timer[0] > s16(8 + BREG_S(5))) {
                i_this->field_0x5fa = 0x4000;
                i_this->field_0x60c = 8000.0f;
            }

            if (i_this->timer[0] == s16(8 + BREG_S(5))) {
                anm_init(i_this, 48, 2.0f, 2, 1.0f);
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_CATCH, -1);
                i_this->sound.startCreatureSound(Z2SE_FN_ROPE_CREAK, 0, -1);
                sw_p->field_0x8fc = 20.0f + XREG_F(0);
                sw_p->field_0xd10[i_this->field_0x630] = 10;
            }

            if (mDoCPd_c::getTrigA(0) != 0) {
                if (i_this->field_0x604 < 0 && i_this->field_0x602 < 0) {
                    i_this->field_0x92c = 1;
                    i_this->field_0x620 = 0;
                    i_this->timer[0] = 15 + BREG_S(3);
                    i_this->mode = 5;
                    i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_THROW, -1);
                    player->changeDemoMode(24, 0, 0, 0);
                    anm_init(i_this, 24, 1.0f, 2, 1.0f);
                    leader->demo_mode = 3;
                } else {
                    leader->demo_mode = 100;
                    i_this->field_0x620 = 0;
                    i_this->timer[0] = 15 + BREG_S(3);
                    i_this->mode = 5;
                    i_this->hang_time = 20;
                }
            }
            break;

        case 5:
            if (i_this->timer[0] == 1) {
                leader->demo_mode = 100;
                i_this->field_0x620 = 0;
                i_this->mode = 0;
            }
    }

    actor->current.pos = sw_p->field_0x920[i_this->field_0x630];
    s16 sVar2 = i_this->field_0x602;
    cLib_addCalcAngleS2(&i_this->field_0x602, i_this->field_0x60c * cM_ssin(i_this->field_0x5fa), 4, 0x1000);
    i_this->field_0x604 = i_this->field_0x602 - sVar2;
    i_this->field_0x5fa += NPC_KS_18DEG_ROT;
    actor->current.angle.z = -(i_this->field_0x602 / 4);

    if (i_this->field_0x620 != 2) {
        cLib_addCalc0(&i_this->field_0x60c, 0.5f, 100.0f + TREG_F(3));
    }

    hang_end_check(i_this);
}

static void npc_ks_e_hang(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* base_sw_p = fopAcM_SearchByID(actor->parentActorID);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    obj_sw_class* sw_p = (obj_sw_class*) base_sw_p;

    sw_p->field_0x900 += 5.0f;
    cXyz unused_cxyz;
    int unused_sp10 = 0;
    cLib_addCalcAngleS2(&actor->current.angle.y, actor->home.angle.y + 0x4000, 2, 0x800);

    switch (i_this->mode) {
        case 0:
            i_this->mode = 20;
            i_this->sound.startCreatureSound(Z2SE_FN_ROPE_CREAK, 0, -1);
            sw_p->field_0x8fc = 20.0f + XREG_F(0);
            sw_p->field_0xd10[i_this->field_0x630] = 10;
            actor->home.angle.y = sw_p->actor.current.angle.y;
            i_this->field_0x5fa = -0x4000;
            i_this->field_0x60c = 4000.0f;

            if (i_this->set_id == 0) {
                anm_init(i_this, 24, 3.0f, 2, 1.0f);
                i_this->timer[0] = 10000;
                i_this->timer[1] = 15;
            }
            break;

        case 2:
            anm_init(i_this, 48, 2.0f, 2, 1.0f);
            i_this->sound.startCreatureSound(Z2SE_FN_ROPE_CREAK, 0, -1);
            sw_p->field_0x8fc = 20.0f + XREG_F(0);
            sw_p->field_0xd10[i_this->field_0x630] = 10;
            i_this->field_0x5fa = -0x8000;
            i_this->field_0x60c = 8000.0f;
            i_this->mode++;
            break;

        case 3:
            if (i_this->field_0x5fa == s16(YREG_S(7) - 0x3800)) {
                actor->health = 10;
                i_this->mode = 20;
                i_this->timer[0] = 0;
            }
            break;

        case 20:
            if (i_this->timer[0] == 0) {
                if (cM_rndF(1.0f) < 0.3f) {
                    anm_init(i_this, 17, 10.0f, 2, 1.0f);
                } else if (cM_rndF(1.0f) < 0.3f) {
                    anm_init(i_this, 18, 10.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 19, 10.0f, 2, 1.0f);
                }

                i_this->timer[0] = 10000;
                i_this->timer[1] = 35;
            }

            if (i_this->timer[1] == 1) {
                anm_init(i_this, 24, 7.0f, 2, 1.0f);
            } else if (i_this->timer[1] == 0) {
                i_this->field_0x5fa = 0x4000;
                i_this->field_0x60c = 8000.0f;
            }
    }

    actor->current.pos = sw_p->field_0x920[i_this->field_0x630];
    cLib_addCalcAngleS2(&i_this->field_0x602, i_this->field_0x60c * cM_ssin(i_this->field_0x5fa), 4, 0x1000);
    i_this->field_0x5fa += NPC_KS_18DEG_ROT;
    actor->current.angle.z = -(i_this->field_0x602 / 4);
    cLib_addCalc0(&i_this->field_0x60c, 0.5f, 100.0f + TREG_F(3));
}

static int all_move_check(int param_1, int param_2) {
    int iVar1 = 0;
    while (iVar1 < param_2) {
        // the following forces dbg stack reg to use r31:
        int unused;
        if (saru_p[iVar1] == NULL) return 0;

        if (param_1 != saru_p[iVar1]->set_id && (saru_p[iVar1]->action != 21 || saru_p[iVar1]->mode != 20)) {
            return 0;
        }

        iVar1++;
    }

    return 1;
}

static int go_jump_check(int param_1) {
    int reg_r30 = param_1 - 1;
    while (reg_r30 >= 0) {
        // the following forces dbg stack reg to use r31:
        int unused;
        if (saru_p[reg_r30]->action != 21 || saru_p[reg_r30]->timer[2] != 0) {
            return 0;
        }

        reg_r30--;
    }

    return 1;
}

static int npc_ks_e_jump(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz mae, unused_cxyz;
    obj_sw_class* sw_p2 = i_this->child_no;
    npc_ks_class* ks_p = (npc_ks_class*)i_this->field_0x930;
    int rv = 2;
    switch(i_this->mode) {
        case 0:
        i_this->field_0xaec = 1;
        if (i_this->dis > l_HIO.pull_distance) {
            anm_init(i_this, 28, 5.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->timer[0] = 20;
        } else {
            anm_init(i_this, 51, 5.0f, 2, 1.0f);
            i_this->mode = 5;
            actor->speedF = 0.0f;
        }

        if (fopAcM_GetRoomNo(actor) == 4) {
            actor->current.pos.y = 3300.0f;
            if (actor->current.pos.x > 180.0f) {
                actor->current.pos.x = 180.0f;
            } else if (actor->current.pos.x < -180.0f) {
                actor->current.pos.x = -180.0f;
            }
        }
        break;

    case 1:
        actor->speedF = l_HIO.holding_speed_h;
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->target_angle, 1, 0x2000);
        rv = 1;
        if (i_this->timer[0] == 0 || i_this->dis < l_HIO.pull_distance - 20.0f) {
            anm_init(i_this, 51, 5.0f, 2, 1.0f);
            i_this->mode = 5;
            actor->speedF = 0.0f;
        }
        break;

    case 5:
        i_this->field_0x5fd = 1;
        mae = ks_p->field_0x614 - actor->current.pos;
        i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 1, 0x2000);

        if (go_jump_check(i_this->set_id) != 0) {
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->timer[0] = 10;
            i_this->mode = 6;
            i_this->field_0xaec = 0;
        }
        break;

    case 6:
        i_this->field_0x5fd = 1;
        actor->speedF = 20.0f;
        rv = 1;
        if (i_this->timer[0] == 0) {
            anm_init(i_this, 32, 2.0f, 0, 1.0f);
            i_this->mode = 7;
        }
        break;

    case 7:
        // bVar1 = true;
        i_this->field_0x5fd = 1;
        if (i_this->res_id == 32) {
            actor->speedF = 0.0f;
            if (i_this->model->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                actor->speedF = 40.0f;
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);

                i_this->field_0x910 = actor->current.pos;
                i_this->field_0xbe0 = 1;
            }
        }

        i_this->guide_path = ks_p->field_0x614;
        i_this->field_0x91c = i_this->guide_path;
        mae = i_this->guide_path - actor->current.pos;
        i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
        i_this->current_angle.x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 1, 0x2000);

        if (actor->speedF >= 40.0f) {
            i_this->field_0x90c = 1;
            cLib_addCalc2(&i_this->field_0x928, -183.0f + XREG_F(11), 1.0f, 15.0f + XREG_F(12));
            cLib_addCalcAngleS2(&actor->current.angle.x, 0x1800, 4, 0x1800);
        }

        if (mae.abs() <= actor->speedF * 1.2f) {
            actor->speedF = 0.0f;
            i_this->mode = 10;
            anm_init(i_this, 48, 2.0f, 2, 1.0f);
            ks_p->mode = 2;
        }
        break;

    case 10:
        actor->home.angle.y = sw_p2->actor.current.angle.y;
        actor->current.angle.y = actor->home.angle.y;
        actor->current.pos = ks_p->field_0x614;
        actor->current.angle.x = (s16)((int)ks_p->field_0x602 << 1);
        if (ks_p->actor.health != 10) break;
        ks_p->actor.health = 0;
        i_this->field_0x910 = actor->current.pos;
        i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_THROW, -1);

        if (fopAcM_GetRoomNo(actor) == 2) {
            actor->speedF = 25.0f + NREG_F(16);
        } else {
            actor->speedF = 40.0f + NREG_F(15);
        }

        if (ks_p->set_id == i_this->set_id - 1) {
            i_this->mode = 11;
        } else {
            i_this->mode = 20;
            i_this->field_0x930 = saru_p[ks_p->set_id + 1];
        }

        anm_init(i_this, 33, 1.0f, 0, 1.0f);
        i_this->field_0xbe0 = 1;
        // fallthrough
    case 11:
        i_this->field_0x90c = 1;
        i_this->field_0x630 = (i_this->set_id + 1) * (sw_p2->field_0xd8c / (sw_p2->field_0x91c + 1)) - 1;
        i_this->guide_path = sw_p2->field_0x920[i_this->field_0x630];

        actor->parentActorID = fopAcM_GetID(sw_p2);
        i_this->field_0x91c = i_this->guide_path;
        mae = i_this->guide_path - actor->current.pos;
        i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
        i_this->current_angle.x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));

        if (mae.abs() <= actor->speedF * 1.2f) {
            actor->speedF = 0.0f;
            i_this->mode = 12;
            anm_init(i_this, 24, 2.0f, 2, 1.0f);
            i_this->sound.startCreatureSound(Z2SE_FN_ROPE_CREAK, 0, -1);
            sw_p2->field_0x8fc = 20.0f + XREG_F(0);
            sw_p2->field_0xd10[i_this->field_0x630] = 10;
        }
        // fallthrough
    case 12:
        cLib_addCalc0(&i_this->field_0x928, 1.0f, 15.0f + NREG_F(17));
        cLib_addCalcAngleS2(&actor->current.angle.x, -0x8000, 1, 0x700 + NREG_S(7));
        if (i_this->mode == 12 && i_this->field_0x928 == 0.0f) {
            if (all_move_check(i_this->set_id, sw_p2->field_0x91c) != 0) {
                all_carry_finish(sw_p2->field_0x91c);
            } else {
                i_this->action = 21;
                i_this->mode = 0;
                i_this->timer[2] = 15;
            }
        }
        break;

    case 20:
        i_this->field_0x90c = 1;
        cLib_addCalcAngleS2(&actor->current.angle.x, 0x1800, 4, 0x1800);
        i_this->guide_path = ks_p->field_0x614;
        i_this->field_0x91c = i_this->guide_path;
        mae = i_this->guide_path - actor->current.pos;
        i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
        i_this->current_angle.x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));

        if (mae.abs() <= actor->speedF * 1.2f) {
            actor->speedF = 0.0f;
            i_this->mode = 10;
            anm_init(i_this, 48, 2.0f, 2, 1.0f);
            ks_p->mode = 2;
        }
    }

    if (i_this->mode != 11 && i_this->mode != 12) {
        i_this->field_0x608 = (s16)((int)actor->current.angle.x << 1);
    }

    return rv;
}

static npc_ks_class* get_move_saru(npc_ks_class* i_this, int param_2) {
    fopAc_ac_c* actor = &i_this->actor;
    if (fopAcM_GetRoomNo(actor) == 0) {
        for (int i = 0; i < 2; i++) {
            if (saru_p[i] != NULL && (saru_p[i]->action == 20 || saru_p[i]->action == 22) && i_this->field_0xb40 == saru_p[i]->child_no->field_0x570) {
                return saru_p[i];
            }
        }
    } else {
        for (int i = 0; i < param_2; i++) {
            if (i_this->field_0xb40 == saru_p[i]->child_no->field_0x570 && ((saru_p[i]->action == 20 || saru_p[i]->action == 22) || saru_p[i]->timer[2] != 0)) {
                return saru_p[i];
            }
        }
    }

    return NULL;
}

static int all_hang_check(npc_ks_class* i_this, int param_2) {
    fopAc_ac_c* actor = &i_this->actor;
    if (fopAcM_GetRoomNo(actor) == 0) {
        for (int i = 0; i < 2; i++) {
            if (saru_p[i] != NULL && saru_p[i]->field_0x5b5 != 0 && i_this->field_0xb40 == saru_p[i]->child_no->field_0x570) {
                return 1;
            }
        }

        return 0;
    }

    for (int i = 0; i < param_2; i++) {
        if (saru_p[i]->field_0x5b5 == 0) {
            return 0;
        }
    }

    return 1;
}

static void cam_3d_morf(npc_ks_class* i_this, f32 param_2) {
    cLib_addCalc2(&i_this->demo_eye.x, i_this->field_0xb6c.x, param_2, i_this->field_0xb84.x * i_this->field_0xbc4);
    cLib_addCalc2(&i_this->demo_eye.y, i_this->field_0xb6c.y, param_2, i_this->field_0xb84.y * i_this->field_0xbc4);
    cLib_addCalc2(&i_this->demo_eye.z, i_this->field_0xb6c.z, param_2, i_this->field_0xb84.z * i_this->field_0xbc4);
    cLib_addCalc2(&i_this->demo_way.x, i_this->field_0xb54.x, param_2, i_this->field_0xb78.x * i_this->field_0xbc4);
    cLib_addCalc2(&i_this->demo_way.y, i_this->field_0xb54.y, param_2, i_this->field_0xb78.y * i_this->field_0xbc4);
    cLib_addCalc2(&i_this->demo_way.z, i_this->field_0xb54.z, param_2, i_this->field_0xb78.z * i_this->field_0xbc4);
}

static void* s_fs_sub(void* i_actor, void* i_data) {
    (void) i_data;
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_FS) {
        ((e_fs_class*)i_actor)->mAction = 10;
        ((e_fs_class*)i_actor)->mMode = 0;
    }
    return NULL;
}

static void* s_fsdown_sub(void* i_actor, void* i_data) {
    (void) i_data;
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_FS) {
        ((e_fs_class*)i_actor)->mTimer[0] = (fopAcM_GetID(i_actor) & 0x3) << 3;
        ((e_fs_class*)i_actor)->mMode++;
    }
    return NULL;
}

static void demo_camera(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* unused_cam_p = dComIfGp_getCamera(0);
    obj_sw_class* sw_p = i_this->child_no;
    fopAc_ac_c* base_sw_p = &sw_p->actor;
    cXyz mae, ato;
    f32 fVar1, fVar2;
    npc_ks_class* npc_ks_p;
    obj_brg_class* brg_p;
    daMidna_c* midna_p;

    switch (i_this->demo_mode) {
        case 0:
            break;

        case 1:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->demo_mode = 2;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->demo_camera_no = 0;
            player->changeOriginalDemo();
            player->changeDemoMode(19, 0, 0, 0);
            player->changeDemoParam0(0);
            i_this->demo_eye = player->current.pos;
            i_this->field_0xba8.z = 500.0f + AREG_F(0);
            i_this->field_0xba8.x = 500.0f + AREG_F(1);
            i_this->field_0xba8.y = 0.0f + AREG_F(2);
            // fallthrough
        case 2:
            i_this->target_speed = 55.0f;
            cLib_addCalc2(&i_this->demo_eye.x, player->current.pos.x, 0.2f, 50.0f + AREG_F(16));
            cLib_addCalc2(&i_this->demo_eye.y, base_sw_p->current.pos.y + AREG_F(4) - 250.0f, 0.2f, 50.0f + AREG_F(16));
            cLib_addCalc2(&i_this->demo_eye.z, player->current.pos.z, 0.2f, 50.0f + AREG_F(16));
            if (fopAcM_GetRoomNo(actor) == 2) {
                cMtx_YrotS(*calc_mtx, base_sw_p->current.angle.y);
            } else {
                cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            }

            mae.x = i_this->field_0xba8.x;
            mae.y = i_this->field_0xba8.y;
            mae.z = i_this->field_0xba8.z;
            MtxPosition(&mae,&ato);

            ato.x = ato.x + player->current.pos.x;
            ato.y = ato.y + (base_sw_p->current.pos.y + AREG_F(5) - 250.0f);
            ato.z = ato.z + player->current.pos.z;
            if (i_this->demo_camera_no == 0) {
                i_this->demo_way = ato;
            } else {
                cLib_addCalc2(&i_this->demo_way.x, ato.x, 0.1f, 200.0f);
                cLib_addCalc2(&i_this->demo_way.y, ato.y, 0.1f, 200.0f);
                cLib_addCalc2(&i_this->demo_way.z, ato.z, 0.1f, 200.0f);
            }

            if (i_this->field_0x92c != 0) {
                i_this->field_0xba8.z += AREG_F(3) - 10.0f;
            }
            break;

        case 3:
            cLib_addCalc2(&i_this->demo_eye.x, player->current.pos.x, 0.4f, 100.0f);
            cLib_addCalc2(&i_this->demo_eye.y, player->current.pos.y + BREG_F(19) + 70.0f, 0.4f, 100.0f );
            cLib_addCalc2(&i_this->demo_eye.z, player->current.pos.z, 0.4f, 100.0f);
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            fVar1 = 50.0f;
            if (fopAcM_GetRoomNo(actor) == 4 && i_this->field_0xbdb == 0) {
                mae.x = 100.0f + BREG_F(10);
                mae.y = 100.0f + BREG_F(11);
                mae.z = 300.0f + BREG_F(12);

                if (i_this->demo_camera_no == 10 + NREG_S(6)) {
                    player->changeDemoMode(40, 0, 0, 0);
                }

                if (i_this->demo_camera_no == 28 + NREG_S(7)) {
                    player->changeDemoMode(41, 0, 0, 0);
                }

                if (i_this->demo_camera_no == 40 + NREG_S(8)) {
                    i_this->demo_mode = 100;
                    i_this->field_0xbdb = 1;
                }

                fVar1 = 200.0f;
            } else {
                mae.x = 0.0f;
                mae.y = 0.0f;
                mae.z = -400.0f;

                if (i_this->demo_camera_no == 8) {
                    i_this->demo_mode = 100;
                }
            }

            MtxPosition(&mae, &ato);
            ato.x = ato.x + player->current.pos.x;
            ato.y = ato.y + player->current.pos.y;
            ato.z = ato.z + player->current.pos.z;
            cLib_addCalc2(&i_this->demo_way.x, ato.x, 0.2f, fVar1);
            cLib_addCalc2(&i_this->demo_way.y, ato.y, 0.2f, fVar1);
            cLib_addCalc2(&i_this->demo_way.z, ato.z, 0.2f, fVar1);
            break;

        case 10:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->demo_mode = 11;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->demo_camera_no = 0;
            i_this->target_speed = 55.0f;
            i_this->field_0xba8.z = 500.0f + AREG_F(0);
            i_this->field_0xba8.x = 500.0f + AREG_F(1);
            i_this->field_0xba8.y = AREG_F(2);
            i_this->field_0xbc0 = NULL;
            // fallthrough
        case 11:
            ato = i_this->field_0xbc8;
            ato.y = player->current.pos.y;
            setPlayerPosAndAngle(&ato, player->shape_angle.y);
            npc_ks_p = i_this->field_0xbc0;
            i_this->field_0xbc0 = get_move_saru(i_this,sw_p->field_0x91c);;
            i_this->field_0xb6c.y = base_sw_p->current.pos.y + AREG_F(4) - 200.0f;
            if (i_this->field_0xbc0 != NULL) {
                base_sw_p = (fopAc_ac_c*) i_this->field_0xbc0->child_no;
                cMtx_YrotS(*calc_mtx, i_this->field_0xbd4);
                mae.x = i_this->field_0xba8.x;
                mae.y = i_this->field_0xba8.y;
                mae.z = i_this->field_0xba8.z;
                MtxPosition(&mae, &i_this->field_0xb54);
                i_this->field_0xb54.x += i_this->field_0xbc0->actor.current.pos.x;
                i_this->field_0xb54.y += base_sw_p->current.pos.y + AREG_F(5) - 250.0f;
                i_this->field_0xb54.z += i_this->field_0xbc0->actor.current.pos.z;
                i_this->field_0xb6c.x = i_this->field_0xbc0->actor.current.pos.x;
                i_this->field_0xb6c.y = base_sw_p->current.pos.y + AREG_F(4) - 200.0f;

                if (i_this->field_0xbc0->action == 20 && i_this->field_0xb6c.y < i_this->field_0xbc0->actor.current.pos.y + AREG_F(8)) {
                    i_this->field_0xb6c.y = i_this->field_0xbc0->actor.current.pos.y + AREG_F(8);
                }

                i_this->field_0xb6c.z = i_this->field_0xbc0->actor.current.pos.z;
                if (npc_ks_p != i_this->field_0xbc0) {
                    i_this->demo_way = i_this->field_0xb54;
                    i_this->demo_eye = i_this->field_0xb6c;
                }
                i_this->field_0xb84.set(200.0f, 200.0f, 200.0f);
                i_this->field_0xbc4 = 1.0f;
            } else {
                if (all_hang_check(i_this, sw_p->field_0x91c) != 0) {
                    i_this->demo_mode = 12;
                    if (fopAcM_GetRoomNo(actor) == 0) {
                        i_this->demo_camera_no = 45;
                    } else if (sw_p->field_0x91c >= 3) {
                        i_this->field_0xb6c.x = saru_p[sw_p->field_0x91c - 2]->actor.current.pos.x;
                        i_this->field_0xb6c.y = saru_p[sw_p->field_0x91c - 2]->actor.current.pos.y;
                        i_this->field_0xb6c.y = i_this->field_0xb6c.y - 150.0f;
                        i_this->field_0xb6c.z = saru_p[sw_p->field_0x91c - 2]->actor.current.pos.z;
                        
                        i_this->field_0xb84.x = NPC_KS_FABSF(i_this->field_0xb6c.x - i_this->demo_eye.x);
                        i_this->field_0xb84.y = NPC_KS_FABSF(i_this->field_0xb6c.y - i_this->demo_eye.y);
                        i_this->field_0xb84.z = NPC_KS_FABSF(i_this->field_0xb6c.z - i_this->demo_eye.z);
                        i_this->field_0xbc4 = 0.0f;
                        i_this->demo_camera_no = 5;
                    } else {
                        i_this->demo_camera_no = 45 + AREG_S(4);
                    }
                }
            }
            // fallthrough
        case 12:
            cLib_addCalc2(&i_this->demo_way.x, i_this->field_0xb54.x, 0.2f, i_this->field_0xbc4 * 200.0f);
            cLib_addCalc2(&i_this->demo_way.y, i_this->field_0xb54.y, 0.2f, i_this->field_0xbc4 * 200.0f);
            cLib_addCalc2(&i_this->demo_way.z, i_this->field_0xb54.z, 0.2f, i_this->field_0xbc4 * 200.0f);
            cLib_addCalc2(&i_this->demo_eye.x, i_this->field_0xb6c.x, 0.2f, i_this->field_0xb84.x * i_this->field_0xbc4);
            cLib_addCalc2(&i_this->demo_eye.y, i_this->field_0xb6c.y, 0.2f, i_this->field_0xb84.y * i_this->field_0xbc4);
            cLib_addCalc2(&i_this->demo_eye.z, i_this->field_0xb6c.z, 0.2f, i_this->field_0xb84.z * i_this->field_0xbc4);
            if (i_this->demo_mode == 12) {
                cLib_addCalc2(&i_this->target_speed, 40.0f, 0.2f, i_this->field_0xbc4);
                cLib_addCalc2(&i_this->field_0xbc4, 0.1f, 1.0f, 0.005f);
                if (85 < i_this->demo_camera_no) {
                    i_this->demo_mode = 100;
                    mae.x = saru_p[i_this->field_0xb40]->actor.current.pos.x - i_this->field_0xbc8.x;
                    mae.z = saru_p[i_this->field_0xb40]->actor.current.pos.z - i_this->field_0xbc8.z;
                    s16 angle = cM_atan2s(mae.x, mae.z);
                    cMtx_YrotS(*calc_mtx, angle);
                    mae.x = 0.0f;
                    mae.y = 0.0f;
                    if (fopAcM_GetRoomNo(actor) == 4) {
                        mae.z = -250.0f + JREG_F(0);
                    } else {
                        mae.z = -150.0f + JREG_F(0);
                    }
                    MtxPosition(&mae,&ato);
                    ato.x = ato.x + i_this->field_0xbc8.x;
                    ato.z = ato.z + i_this->field_0xbc8.z;

                    if (fopAcM_GetRoomNo(actor)  == 2) {
                        ato.y = 3764.0f;
                    } else {
                        ato.y = player->current.pos.y;
                    }
                    setPlayerPosAndAngle(&ato, angle);
                    mae.x = 0.0f;
                    mae.y = 100.0f + JREG_F(1);
                    mae.z = -250.0f + JREG_F(2);
                    MtxPosition(&mae, &i_this->demo_way);
                    i_this->demo_way += player->current.pos;
                    i_this->demo_eye = player->current.pos;
                    i_this->demo_eye.y += 120.0f;
                    i_this->target_speed = 55.0f;
                }
            }
            break;

        case 60:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->demo_mode = 61;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->demo_camera_no = 0;
            cMtx_YrotS(*calc_mtx, s16(actor->shape_angle.y));
            mae.x = 300.0f + WREG_F(11);
            mae.y = 100.0f + WREG_F(12);
            mae.z = WREG_F(13);
            MtxPosition(&mae, &i_this->demo_way);
            i_this->demo_way += actor->current.pos;

            mae.x = i_this->demo_way.x - player->current.pos.x;
            mae.z = i_this->demo_way.z - player->current.pos.z;
            if (mae.x * mae.x + mae.z * mae.z < 10000.0f) {
                OS_REPORT("////////KS PL REVISED \n");
                i_this->demo_way.x += + 200.0f + JREG_F(7);
                i_this->demo_way.z += JREG_F(8);
            }

            i_this->demo_eye = actor->eyePos;
            i_this->demo_eye.y += WREG_F(10);
            player->changeOriginalDemo();
            player->changeDemoMode(1, 1, 0, 0);
            // fallthrough
        case 61:
            ato = player->old.pos;
            ato.y = 3150.0f;
            setPlayerPosAndAngle(&ato, player->shape_angle.y);
            i_this->target_speed = 65.0f + WREG_F(8);
            cLib_addCalc2(&i_this->demo_eye.x, actor->eyePos.x, 0.1f + WREG_F(9), 50.0f);
            cLib_addCalc2(&i_this->demo_eye.y, actor->eyePos.y + WREG_F(10), 0.1f + WREG_F(9), 50.0f);
            cLib_addCalc2(&i_this->demo_eye.z, actor->eyePos.z, 0.1f + WREG_F(9), 50.0f);
            break;

        case 70:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->demo_mode = 71;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->demo_camera_no = 0;
            i_this->target_speed = 55.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(1, 1, 0, 0);
            // fallthrough
        case 71:
            if (i_this->demo_camera_no == 0) {
                i_this->demo_eye = actor->current.pos;
                i_this->demo_eye.y = actor->current.pos.y + 120.0f + VREG_F(6);
                i_this->demo_way.set(-4786.0f, 3982.0f, -1843.0f);
            } else if (i_this->demo_camera_no == 210 + WREG_S(0)) {
                i_this->demo_way.set(-3232.0f, 4137.0f, -1425.0f);
            } else if (i_this->demo_camera_no == 240 + WREG_S(1)) {
                i_this->demo_way.set(-2459.0f, 4199.0f, -2752.0f);
            } else if (i_this->demo_camera_no == 290 + WREG_S(2)) {
                i_this->demo_way.set(103.0f, 4025.0f, -1930.0f);
            }
            cLib_addCalc2(&i_this->demo_eye.x, actor->current.pos.x, 0.2f, 60.0f);
            cLib_addCalc2(&i_this->demo_eye.y, actor->current.pos.y + 120.0f + VREG_F(6), 0.1f, 30.0f);
            cLib_addCalc2(&i_this->demo_eye.z, actor->current.pos.z, 0.2f, 60.0f);
            
            if (i_this->demo_camera_no == 358) {
                i_this->demo_mode = 99;
            }
            break;

        case 72:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->demo_mode = 73;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->demo_camera_no = 0;
            i_this->target_speed = 55.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(1, 1, 0, 0);
            // fallthrough
        case 73:
            if (i_this->demo_camera_no == 0) {
                i_this->demo_eye = actor->current.pos;
                i_this->demo_eye.y = actor->current.pos.y + 120.0f + VREG_F(6);
                i_this->demo_way.set(-112.0f, 2420.0f, 13414.0f);
            }

            ato.set(145.0f, 2354.0f, 13179.0f);
            setPlayerPosAndAngle(&ato, -0x691B);
            cLib_addCalc2(&i_this->demo_eye.x, actor->current.pos.x, 0.1f, 20.0f);
            cLib_addCalc2(&i_this->demo_eye.y, actor->current.pos.y + 120.0f + VREG_F(6), 0.1f, 10.0f);
            cLib_addCalc2(&i_this->demo_eye.z, actor->current.pos.z, 0.12f, 20.0f);
            
            if (i_this->demo_camera_no == 280) {
                i_this->demo_mode = 100;
            }
            break;

        case 75:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }
            
            i_this->demo_mode = 76;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->demo_camera_no = 0;
            i_this->target_speed = 55.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(1,1,0,0);
            // fallthrough
        case 76:
            setPlayerPosAndAngle(&player->current.pos, i_this->target_angle + 0x8000);
            if (i_this->demo_camera_no == 0) {
                cMtx_YrotS(*calc_mtx,actor->shape_angle.y);
                mae.x = 0.0f;
                mae.y = 100.0f + JREG_F(1);
                mae.z = 400.0f + JREG_F(2);
                MtxPosition(&mae, &i_this->demo_way);
                i_this->demo_way += actor->current.pos;
                i_this->demo_eye = actor->current.pos;
                i_this->demo_eye.y = actor->current.pos.y + 120.0f + VREG_F(6);
            }
            cLib_addCalc2(&i_this->demo_eye.x, actor->current.pos.x, 0.2f, 60.0f);
            cLib_addCalc2(&i_this->demo_eye.y, actor->current.pos.y + 120.0f + VREG_F(6), 0.1f, 30.0f);
            cLib_addCalc2(&i_this->demo_eye.z, actor->current.pos.z, 0.2f, 60.0f);
            if (i_this->demo_camera_no == 180) {
                i_this->demo_mode = 99;
            }
            break;

        case 77:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->demo_mode = 78;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->demo_camera_no = 0;
            i_this->target_speed = 55.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(1, 1, 0, 0);
            i_this->target_speed = 65.0f;
            i_this->demo_eye.set(8486.0f, 4143.0f, -10114.0f);
            i_this->demo_way.set(8364.0f, 4325.0f, -9756.0f);
            // fallthrough
        case 78:
            cLib_addCalc2(&i_this->target_speed, 55.0f, 0.05f, 0.2f);
            if (i_this->demo_camera_no != 80) break;
            i_this->demo_mode = 79;
            i_this->demo_camera_no = 0;
            // fallthrough
        case 79:
            if (i_this->demo_camera_no == 0) {
                i_this->demo_eye = actor->current.pos;
                i_this->demo_eye.y = actor->current.pos.y + 120.0f + VREG_F(6);
                i_this->demo_way.set(6899.0f, 3867.0f, -8870.0f);
            } else if (i_this->demo_camera_no == 60) {
                i_this->demo_way.set(8222.0f, 3896.0f, -9579.0f);
            }
            cLib_addCalc2(&i_this->demo_eye.x, actor->current.pos.x, 0.2f, 60.0f);
            cLib_addCalc2(&i_this->demo_eye.y, actor->current.pos.y + 120.0f + VREG_F(6), 0.1f, 30.0f);
            cLib_addCalc2(&i_this->demo_eye.z, actor->current.pos.z, 0.2f, 60.0f);
            if (i_this->demo_camera_no >= 130) {
                cLib_addCalc2(&i_this->target_speed, 40.0f, 0.05f, 0.3f);
            }

            if (i_this->demo_camera_no == 200) {
                i_this->demo_mode = 100;
                dComIfGs_onSwitch(83, fopAcM_GetRoomNo(actor));
            }
            break;
        
        case 80:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->demo_mode = 81;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->demo_camera_no = 0;
            i_this->target_speed = 55.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(1, 1, 0, 0);
            // fallthrough
        case 81:
            if (i_this->demo_camera_no == 0) {
                saru_p[3]->actor.current.pos.x = -500.0f;
                saru_p[3]->actor.current.pos.y = 3681.0f;
                saru_p[3]->actor.current.pos.z = 7485.0f;
            }

            if (i_this->demo_camera_no == 30) {
                saru_p[0]->actor.current.pos.x = player->current.pos.x;
                saru_p[0]->actor.current.pos.y = player->current.pos.y + 500.0f;
                saru_p[0]->actor.current.pos.z = player->current.pos.z - 100.0f;
            }

            if (i_this->demo_camera_no == 60) {
                saru_p[1]->actor.current.pos.x = player->current.pos.x + 200.0f;
                saru_p[1]->actor.current.pos.y = player->current.pos.y + 500.0f;
                saru_p[1]->actor.current.pos.z = player->current.pos.z;
            }

            if (i_this->demo_camera_no == 90) {
                saru_p[2]->actor.current.pos.x = i_this->demo_way.x - 250.0f;
                saru_p[2]->actor.current.pos.y = i_this->demo_way.y + 200.0f;
                saru_p[2]->actor.current.pos.z = i_this->demo_way.z - 100.0f;
            }

            if (i_this->demo_camera_no < 80) {
                i_this->demo_eye.set(-333.0f,3316.0f,7149.0f);
                i_this->demo_way.set(-531.0f,3295.0f,7381.0f);
            } else {
                i_this->demo_eye.set(162.0f,3354.0f,6881.0f);
                i_this->demo_way.set(263.0f,3374.0f,7168.0f);
            }
            
            if (i_this->demo_camera_no == 170) {
                i_this->demo_mode = 100;
                i_this->demo_eye.set(-333.0f,3316.0f,7149.0f);
                i_this->demo_way.set(-531.0f,3295.0f,7381.0f);
            }
            break;

        case 110:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->demo_mode = 111;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->demo_camera_no = 0;
            i_this->target_speed = 59.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(3, 0, 0, 0);
            ato.set(12899.0f, 3767.0f, 3364.0f);
            player->changeDemoPos0(&ato);
            
            i_this->demo_eye.set(11821.0f, 3889.0f, 4033.0f);
            i_this->demo_way.set(11710.0f, 3936.0f, 4253.0f);
            i_this->field_0xb6c.set(12772.0f, 3940.0f, 3565.0f);
            i_this->field_0xb54.set(13003.0f, 4022.0f, 3613.0f);

            i_this->field_0xb78.x = NPC_KS_FABSF(i_this->field_0xb54.x - i_this->demo_way.x);
            i_this->field_0xb78.y = NPC_KS_FABSF(i_this->field_0xb54.y - i_this->demo_way.y);
            i_this->field_0xb78.z = NPC_KS_FABSF(i_this->field_0xb54.z - i_this->demo_way.z);
            i_this->field_0xb84.x = NPC_KS_FABSF(i_this->field_0xb6c.x - i_this->demo_eye.x);
            i_this->field_0xb84.y = NPC_KS_FABSF(i_this->field_0xb6c.y - i_this->demo_eye.y);
            i_this->field_0xb84.z = NPC_KS_FABSF(i_this->field_0xb6c.z - i_this->demo_eye.z);
            i_this->field_0xbc4 = 0.0f;
            // fallthrough
        case 111:
        case 112:
            if (i_this->demo_camera_no < 10) {
                ato.set(11497.0f, 3764.0f, 3810.0f);
                setPlayerPosAndAngle(&ato, 0x4D5A);
            }

            if (i_this->demo_camera_no > 5) {
                cam_3d_morf(i_this, 0.1f + BREG_F(17));
                f32 tgt = 0.04f + BREG_F(17);
                cLib_addCalc2(&i_this->field_0xbc4, tgt, 1.0f, 0.001f + BREG_F(19));
            }

            if (i_this->field_0xbdc == 0 && player->current.pos.x >= 12626.0f) {
                i_this->field_0xbdc = 1;
                JUTReport(300, 220, " PL IN %d");
                brg_p = (obj_brg_class *)fopAcM_SearchByName(44);
                if (brg_p != NULL) {
                    brg_p->field_0xaf1c = 4;
                    brg_p->field_0xaed0 = 6;
                    dComIfGp_particle_set(0x833F, &brg_p->mBr[6].field_0x0bc, &brg_p->mBr[6].field_0x0d4, NULL);
                }
            }

            if (i_this->field_0xbdc == 1 &&  player->current.pos.x >= 12714.0f) {
                i_this->field_0xbdc = 2;
                i_this->demo_mode = 112;
                daPy_getPlayerActorClass()->changeDemoMode(25, 0, 0, 0);
                i_this->demo_camera_no = 20;
            }

            if (i_this->demo_mode == 112 && i_this->demo_camera_no >= 53 + BREG_S(9)) {
                i_this->demo_mode = 100;
                cMtx_YrotS(*calc_mtx, player->shape_angle.y);
                mae.x = 0.0f;
                mae.y = 100.0f + JREG_F(1);
                mae.z = -250.0f + JREG_F(2);
                MtxPosition(&mae, &i_this->demo_way);
                i_this->demo_way += player->current.pos;
                i_this->demo_eye = player->current.pos;
                i_this->demo_eye.y += 120.0f;
            }
            break;

        case 200:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->demo_mode = 201;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->demo_camera_no = 0;
            i_this->target_speed = 55.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(2, 0, 0, 0);
            ato.set(-23451.0f,262.0f,-15980.0f);
            player->changeDemoPos0(&ato);
            i_this->demo_eye.set(-22325.0f, 275.0f, -15119.0f);
            i_this->demo_way.set(-22672.0f, 311.0f, -15080.0f);
            i_this->field_0xb6c.set(-22343.0f, 273.0f, -15017.0f);
            i_this->field_0xb78.set(0.0f, 0.0f, 0.0f);
            i_this->field_0xb84.x = NPC_KS_FABSF(i_this->field_0xb6c.x - i_this->demo_eye.x);
            i_this->field_0xb84.y = NPC_KS_FABSF(i_this->field_0xb6c.y - i_this->demo_eye.y);
            i_this->field_0xb84.z = NPC_KS_FABSF(i_this->field_0xb6c.z - i_this->demo_eye.z);
            i_this->field_0xbc4 = 0.0f;
            /* dSv_event_flag_c::F_0224 - Faron Woods - Flag for lantern guide monkey cutscene */
            dComIfGs_onEventBit(u16(dSv_event_flag_c::saveBitLabels[0xE0]));
            // fallthrough
        case 201:
            if (i_this->demo_camera_no < 30) {
                ato.set(-22177.0f, 213.0f, -14705.0f);
                setPlayerPosAndAngle(&ato, -0x5FFD);
                player->speedF = 0.0f;
            }

            cam_3d_morf(i_this, 0.05f + BREG_F(17));
            cLib_addCalc2(&i_this->field_0xbc4, 0.02f + BREG_F(18), 1.0f, 0.0001f + BREG_F(19));

            if (i_this->demo_camera_no == 110 + AREG_S(5)) {
                i_this->mode = 2;
                i_this->field_0xbb4 = 500.0f;
            }

            if (i_this->demo_camera_no == 132 + AREG_S(6)) {
                i_this->field_0xc17 = 2;

                mDoAud_seStart(0x5001A, NULL, 0, 0);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                dComIfGs_setItem(1, 0xFF);
                dMeter2Info_setOilGaugeBackUp(dComIfGs_getOil());
                dComIfGp_getVibration().StartShock(2, 31, cXyz(0.0f, 1.0f, 0.0f));
            }

            if (i_this->demo_camera_no == 139 + AREG_S(6)) {
                player->changeDemoMode(1, 1, 0, 0);
            }

            if (i_this->demo_camera_no == 144) {
                cMtx_YrotS(*calc_mtx, player->shape_angle.y);
                mae.x = 0.0f;
                mae.y = AREG_F(1);
                mae.z = 100.0f + AREG_F(2);
                MtxPosition(&mae, &ato);
                actor->current.pos = player->current.pos + ato;
                actor->current.angle.y = s16(player->shape_angle.y);
                i_this->current_angle.y = player->shape_angle.y;
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
                i_this->mode = 230;
                i_this->timer[0] = 65 + JREG_S(7);
            }

            if (i_this->demo_camera_no == 145) {
                i_this->demo_mode = 202;
                i_this->demo_camera_no = 0;
                i_this->demo_eye.set(-22613.0f, 350.0f, -15097.0f);
                i_this->demo_way.set(-22275.0f, 350.0f, -15092.0f);
            }
            break;

        case 202:
            if (i_this->demo_camera_no == 30) {
                i_this->demo_eye.set(-23371.0f, 380.0f, -15901.0f);
                i_this->demo_way.set(-23163.0f, 474.0f, -15653.0f);
                player->changeDemoMode(1, 1, 0, 0);
            }

            if (i_this->demo_camera_no >= 120) {
                cLib_addCalc2(&i_this->target_speed, 80.0f, 0.05f, 0.2f + YREG_F(9));
            }

            if (i_this->demo_camera_no == 270) {
                i_this->demo_mode = 203;
                i_this->demo_camera_no = 0;
            }
            break;

        case 203:
            i_this->demo_eye.set(-22650.0f, 334.0f, -15147.0f);
            i_this->demo_way.set(-22277.0f, 355.0f, -15077.0f);
            i_this->target_speed = 55.0f;
            if (i_this->demo_camera_no == 10) {
                midna_p = daPy_py_c::getMidnaActor();
                midna_p->changeOriginalDemo();
                midna_p->changeDemoMode(2);
            }

            if (i_this->demo_camera_no >= 100) {
                i_this->demo_camera_no = 100;
            }

            if (i_this->demo_camera_no >= 70) {
                if (i_this->demo_camera_no == 70) {
                    i_this->msg_flow.init(actor, 2015, 0, NULL);
                }
                if (i_this->msg_flow.doFlow(actor, NULL, 0) != 0) {
                    i_this->demo_mode = 204;
                    daMidna_c* midna_p = daPy_py_c::getMidnaActor();
                    midna_p->changeDemoMode(5);
                }
            }
            break;

        case 204: {
            daMidna_c* midna_p = daPy_py_c::getMidnaActor();
            if (midna_p->checkShadowReturnEnd() != 0) {
                i_this->demo_mode = 100;
            }
            break;
        }

        case 300:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->demo_mode = 301;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->demo_camera_no = 0;
            i_this->target_speed = 60.0f;
            player->changeOriginalDemo();
            i_this->demo_eye.set(-30827.0f, 411.0f, -17200.0f);
            i_this->demo_way.set(-30601.0f, 388.0f, -17465.0f);
            // fallthrough
        case 301:
            ato.set(-30496.0f, 300.0f, -17449.0f);
            setPlayerPosAndAngle(&ato, -0x18B4);
            if (i_this->demo_camera_no == 60) {
                i_this->demo_mode = 302;
                i_this->demo_camera_no = 0;
                i_this->demo_eye.set(-30844.0f, 298.0f, -17007.0f);
                i_this->demo_way.set(-30653.0f, 357.0f, -17289.0f);
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
                i_this->field_0xc17 = 3;
                mDoAud_seStart(0x600AC, &i_this->field_0xc04, 0, 0);
            }
            break;

        case 302:
            if (i_this->demo_camera_no == 30) {
                dComIfGp_setItemOilCount(-int(dComIfGs_getMaxOil()));
            }

            if (i_this->demo_camera_no == 80) {
                i_this->demo_mode = 303;
                i_this->demo_camera_no = 0;
                i_this->demo_eye.set(-30887.0f, 429.0f, -17181.0f);
                i_this->demo_way.set(-30870.0f, 471.0f, -17527.0f);
            }
            break;

        case 303:
            if (i_this->demo_camera_no == 10) {
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
                i_this->mode = 301;
                actor->speedF = l_HIO.demo_speed_2;
            }

            if (i_this->demo_camera_no == 0x50) {
                i_this->demo_mode = 100;
                i_this->mode = 302;
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
                /* dSv_event_flag_c::F_0225 - Faron Woods - Lanter guide monkey doesn't come out a second time */
                dComIfGs_onEventBit(u16(dSv_event_flag_c::saveBitLabels[0xE1]));
                dComIfGs_offSwitch(61, fopAcM_GetRoomNo(actor));
            }
            break;

        case 350:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->demo_mode = 351;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->demo_camera_no = 0;
            i_this->target_speed = 55.0f;
            player->changeOriginalDemo();
            i_this->demo_eye = actor->current.pos;
            i_this->demo_way.set(-37509.0f, 805.0f, -22416.0f);
            // fallthrough
        case 351:
            mae.set(-36523.0f, 335.0f, -20500.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&mae, 0x8000, 0);
            mae = actor->current.pos;
            cLib_addCalc2(&i_this->demo_eye.x, mae.x, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->demo_eye.y, mae.y + 100.0f + nREG_F(0), 0.2f, 50.0f);
            cLib_addCalc2(&i_this->demo_eye.z, mae.z, 0.2f, 50.0f);

            if (i_this->demo_camera_no == 105) {
                i_this->demo_way.set(-37817.0f, 800.0f, -21442.0f);
            }

            if (i_this->demo_camera_no == 178) {
                fpcM_Search(s_fsdown_sub, i_this);
            }

            if (i_this->demo_camera_no == 180) {
                i_this->demo_eye.set(-36666.0f, 600.0f, -22213.0f);
                i_this->demo_way.set(-36489.0f, 399.0f, -20932.0f);
                i_this->field_0xb6c.set(-36574.0f, 421.0f, -21554.0f);
                i_this->field_0xb54.set(-36397.0f, 374.0f, -20263.0f);

                i_this->field_0xb78.x = NPC_KS_FABSF(i_this->field_0xb54.x - i_this->demo_way.x);
                i_this->field_0xb78.y = NPC_KS_FABSF(i_this->field_0xb54.y - i_this->demo_way.y);
                i_this->field_0xb78.z = NPC_KS_FABSF(i_this->field_0xb54.z - i_this->demo_way.z);
                i_this->field_0xb84.x = NPC_KS_FABSF(i_this->field_0xb6c.x - i_this->demo_eye.x);
                i_this->field_0xb84.y = NPC_KS_FABSF(i_this->field_0xb6c.y - i_this->demo_eye.y);
                i_this->field_0xb84.z = NPC_KS_FABSF(i_this->field_0xb6c.z - i_this->demo_eye.z);
                i_this->field_0xbc4 = 0.0f;
                i_this->demo_mode = 352;
                i_this->demo_camera_no = 0;
                i_this->demo_eye.y = 1500.0f + nREG_F(1);
            }
            break;

        case 352:
            if (i_this->demo_camera_no == 40) {
                i_this->mode = 40;
            }

            if (i_this->demo_camera_no == 110) {
                fpcM_Search(s_fsdown_sub, i_this);
            }

            if (i_this->demo_camera_no > 160) {
                cam_3d_morf(i_this, 0.1f + BREG_F(17));
                cLib_addCalc2(&i_this->field_0xbc4, 0.2f + BREG_F(18), 1.0f, 0.005f + BREG_F(19));
            } else if (i_this->demo_camera_no > 25 + nREG_S(0)) {
                cLib_addCalc2(&i_this->demo_eye.y, 600.0f, 0.5f, 100.0f + nREG_F(2));
            }

            if (i_this->demo_camera_no == 230) {
                i_this->demo_mode = 100;
                fpcM_Search(s_fsdown_sub, i_this);
                /* dSv_event_flag_c::F_0544 - Faron Woods - Watched cutscene of monkey girl running away after being attacked by puppet */
                dComIfGs_onEventBit(u16(dSv_event_flag_c::saveBitLabels[0x220]));
            }
            break;

        case 360:
            if (!actor->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
                actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->demo_mode = 361;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->demo_camera_no = 0;
            i_this->target_speed = 55.0f;
            player->changeOriginalDemo();

            i_this->demo_eye.set(-36581.0f, 412.0f, -21418.0f);
            i_this->demo_way.set(-36509.0f, 412.0f, -21127.0f);
            i_this->field_0xb6c.set(-38113.0f, 1234.0f, -22897.0f);
            i_this->field_0xb54.set(-37882.0f, 1182.0f, -22713.0f);

            i_this->field_0xb78.x = NPC_KS_FABSF(i_this->field_0xb54.x - i_this->demo_way.x);
            i_this->field_0xb78.y = NPC_KS_FABSF(i_this->field_0xb54.y - i_this->demo_way.y);
            i_this->field_0xb78.z = NPC_KS_FABSF(i_this->field_0xb54.z - i_this->demo_way.z);
            i_this->field_0xb84.x = NPC_KS_FABSF(i_this->field_0xb6c.x - i_this->demo_eye.x);
            i_this->field_0xb84.y = NPC_KS_FABSF(i_this->field_0xb6c.y - i_this->demo_eye.y);
            i_this->field_0xb84.z = NPC_KS_FABSF(i_this->field_0xb6c.z - i_this->demo_eye.z);
            i_this->field_0xbc4 = 0.0f;
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mode = 41;
            i_this->timer[0] = 30;
            // fallthrough
        case 361:
            mae.set(-36540.0f, 335.0f, -20870.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&mae, -32768, 0);
            if (i_this->demo_camera_no >= 105) {
                if (i_this->demo_camera_no == 105) {
                    i_this->msg_flow.init(actor, 116, 0, NULL);
                }

                if (i_this->msg_flow.getNowMsgNo() == 0x1421) {
                    i_this->demo_mode = 362;
                    i_this->demo_camera_no = 0;
                }
                i_this->msg_flow.doFlow(actor, NULL, 0);
            }
            break;

        case 362:
            if (i_this->demo_camera_no == 15) {
                i_this->timer[2] = 30;
            }
            if (i_this->demo_camera_no >= 30) {
                cam_3d_morf(i_this, 0.2f + BREG_F(17));
                cLib_addCalc2(&i_this->field_0xbc4, 0.3f + BREG_F(18), 1.0f, 0.01f + BREG_F(19));
            }

            if (i_this->msg_flow.getNowMsgNo() == 0x1423) {
                i_this->demo_mode = 363;
                i_this->demo_camera_no = 0;
                i_this->field_0xb6c.set(-36467.0f, 426.0f, -20914.0f);
                i_this->field_0xb54.set(-36244.0f, 409.0f, -20714.0f);

                i_this->field_0xb78.x = NPC_KS_FABSF(i_this->field_0xb54.x - i_this->demo_way.x);
                i_this->field_0xb78.y = NPC_KS_FABSF(i_this->field_0xb54.y - i_this->demo_way.y);
                i_this->field_0xb78.z = NPC_KS_FABSF(i_this->field_0xb54.z - i_this->demo_way.z);
                i_this->field_0xb84.x = NPC_KS_FABSF(i_this->field_0xb6c.x - i_this->demo_eye.x);
                i_this->field_0xb84.y = NPC_KS_FABSF(i_this->field_0xb6c.y - i_this->demo_eye.y);
                i_this->field_0xb84.z = NPC_KS_FABSF(i_this->field_0xb6c.z - i_this->demo_eye.z);
                i_this->field_0xbc4 = 0.0f;
            }
            i_this->msg_flow.doFlow(actor, NULL, 0);
            break;

        case 363:
            cam_3d_morf(i_this, 0.1f + BREG_F(17));
            cLib_addCalc2(&i_this->field_0xbc4, 0.3f + BREG_F(18), 1.0f, 0.01f + BREG_F(19));
            if (i_this->msg_flow.doFlow(actor, NULL, 0) != 0) {
                i_this->demo_mode = 100;
            }
            break;

    }

    if (i_this->demo_mode == 99 || i_this->demo_mode == 98) {
        if (i_this->demo_mode == 99) {
            fopAcM_delete(actor);
        }

        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        mae.x = 0.0f;
        mae.y = 100.0f + JREG_F(1);
        mae.z = -250.0f + JREG_F(2);
        MtxPosition(&mae, &i_this->demo_way);
        i_this->demo_way += player->current.pos;
        i_this->demo_eye = player->current.pos;
        i_this->demo_eye.y += 120.0f;
        i_this->demo_mode = 100;
    }

    if (i_this->demo_mode == 100) {
        camera->mCamera.Reset(i_this->demo_eye, i_this->demo_way, i_this->target_speed, 0);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->demo_mode = 0;
    }

    if (i_this->demo_mode != 0) {
        camera->mCamera.Set(i_this->demo_eye, i_this->demo_way, i_this->target_speed, 0);
        i_this->demo_camera_no++;
    }
}

static int saru_count_check(npc_ks_class* i_this) {
    i_this->child_no = (obj_sw_class*)fpcM_Search(s_sw_sub, i_this);
    if (i_this->child_no == NULL) {
        return 0;
    } else {
        for (int i = 0; i < i_this->child_no->field_0x91c; i++) {
            if (saru_p[i] == NULL) {
                return 0;
            }
        }

        return 1;
    }
}

static void action_check(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz mae;

    switch (fopAcM_GetRoomNo(actor)) {
        case 0:
            if (!dComIfGp_event_runCheck() && i_this->set_id == 0) {
                if (saru_p[1] == NULL) {
                    if (dComIfGs_isSwitch(22, fopAcM_GetRoomNo(actor))) {
                        mae.x = actor->current.pos.x - 5.0f;
                        mae.z = actor->current.pos.z - 5274.0f;
                        if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < 500.0f) {
                            i_this->action = 110;
                            i_this->mode = 0;
                            i_this->field_0xaec = 1;
                            return;
                        }
                    } else {
                        dBgS_GndChk gnd_chk;
                        mae.set(0.0f, 3250.0f, 6500.0f);
                        gnd_chk.SetPos(&mae);
                        if (dComIfG_Bgsp().GroundCross(&gnd_chk) > 3000.0f) {
                            i_this->action = 112;
                            i_this->mode = 0;
                            if (actor->current.pos.y < 3000.0f) {
                                actor->current.pos = player->current.pos;
                                actor->old.pos = actor->current.pos;
                            }

                            i_this->field_0xaec = 1;
                            return;
                        }
                    }
                } else if (saru_p[1] != NULL && saru_p[2] != NULL && saru_p[3] != NULL) {
                    if (player->current.pos.y < 3000.0f) {
                        dComIfGs_onSwitch(82, fopAcM_GetRoomNo(actor));
                    }

                    if (!dComIfGs_isSwitch(82, fopAcM_GetRoomNo(actor))) {
                        mae.x = player->current.pos.x - -500.0f;
                        mae.z = player->current.pos.z - 7211.0f;
                        if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < 200.0f) {
                            dComIfGs_onSwitch(82, fopAcM_GetRoomNo(actor));
                            for (int i = 0; i < 4; i++) {
                                saru_p[i]->action = 111;
                                saru_p[i]->mode = 0;
                                saru_p[i]->field_0xaec = 1;
                            }
                            i_this->demo_mode = 80;
                        }
                    }
                }
            }

            if (!dComIfGs_isSwitch(82, fopAcM_GetRoomNo(actor)) && saru_count_check(i_this) != 0) {
                if (dComIfGs_isSwitch(22, fopAcM_GetRoomNo(actor))) {
                    for (int i = 0; i < 2; i++) {
                        mae.x = player->current.pos.x - i_this->child_no->field_0x904[i].x;
                        mae.y = player->current.pos.y - (i_this->child_no->field_0x904[i].y - 450.0f);
                        mae.z = player->current.pos.z - i_this->child_no->field_0x904[i].z;
                        if (mae.abs() < 300.0f + YREG_F(11)) {
                            i_this->field_0xbd9 = 0;
                            i_this->action = 20;
                            i_this->mode = 0;
                            i_this->field_0xaec = 0;
                            i_this->field_0xbc8 = i_this->child_no->field_0x904[i];
                            mae = i_this->child_no->field_0x904[1 - i] - i_this->field_0xbc8;
                            leader->field_0xbd4 = cM_atan2s(mae.x, mae.z);
                            leader->demo_mode = 10;
                            leader->field_0xb40 = i_this->child_no->field_0x570;

                            if (i_this->set_id != 0) {
                                leader->child_no = i_this->child_no;
                                leader->field_0xbc8 = i_this->field_0xbc8;
                            }
                        }
                    }
                }
            }
            break;

        case 1:
            if (!dComIfGp_event_runCheck() && i_this->set_id == 0 && saru_p[1] == NULL && actor->field_0x567 == 0) {
                mae.x = actor->current.pos.x - 5334.0f;
                mae.z = actor->current.pos.z - 7609.0f;
                if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < 700.0f) {
                    i_this->action = 113;
                    i_this->mode = 0;
                    i_this->field_0xaec = 1;
                    actor->field_0x567 = 1;
                }
            }
            break;

        case 2:
        case 4:
            if (saru_count_check(i_this) != 0) {
                int iVar1 = 1;
                if (fopAcM_GetRoomNo(actor) == 2) {
                    for (int i = 0; i < i_this->child_no->field_0x91c; i++) {
                        mae = player->current.pos - saru_p[i]->actor.current.pos;
                        if (mae.abs() > 400.0f) {
                            iVar1 = 0;
                        }
                    }
                }

                if (iVar1 != 0) {
                    mae.x = player->current.pos.x - i_this->child_no->field_0x904[0].x;
                    mae.y = player->current.pos.y - (i_this->child_no->field_0x904[0].y - 450.0f);
                    mae.z = player->current.pos.z - i_this->child_no->field_0x904[0].z;
                    if (mae.abs() < 300.0f) {
                        i_this->field_0xbc8 = i_this->child_no->field_0x904[0];
                        if (i_this->set_id == 0) {
                            i_this->field_0xbd9 = 0;
                            i_this->action = 20;
                            i_this->mode = 0;
                            i_this->field_0xaec = 0;
                            leader->demo_mode = 10;
                            leader->field_0xb40 = 0;
                            leader->field_0xbc8 = i_this->field_0xbc8;
                            mae = i_this->child_no->field_0x904[1] - i_this->field_0xbc8;
                            leader->field_0xbd4 = cM_atan2s(mae.x, mae.z);
                        } else {
                            i_this->action = 22;
                            i_this->mode = 0;
                            i_this->field_0x930 = leader;
                        }
                    }
                }
            } else if (!checkDoorDemo() && fopAcM_GetRoomNo(actor) == 4 && i_this->set_id == 0) {
                if (!dComIfGs_isSwitch(22, fopAcM_GetRoomNo(actor))) {
                    i_this->action = 203;
                    i_this->mode = 0;
                }
            }
            break;

        case 9:
            if (!dComIfGp_event_runCheck() && i_this->set_id == 4 && player->current.pos.z < -3900.0f) {
                if (!dComIfGs_isSwitch(81, fopAcM_GetRoomNo(actor))) {
                    dBgS_GndChk gnd_chk;
                    mae.set(7275.0f, 3373.0f, -4790.0f);
                    gnd_chk.SetPos(&mae);
                    if (dComIfG_Bgsp().GroundCross(&gnd_chk) > 3000.0f) {
                        i_this->action = 115;
                        i_this->mode = 0;
                        dComIfGs_onSwitch(81, fopAcM_GetRoomNo(actor));
                        i_this->field_0xaec = 1;
                    }
                }
            }
            break;

        case 12:
            if (!dComIfGp_event_runCheck() && i_this->set_id == 4) {
                if (!dComIfGs_isSwitch(83, fopAcM_GetRoomNo(actor))) {
                    i_this->action = 204;
                    i_this->mode = 0;
                    fopAcM_setRoomLayer(actor, 12);
                    i_this->demo_mode = 77;
                    i_this->field_0xaec = 1;
                }
            }
    }
}

static BOOL water_check(npc_ks_class* i_this, cXyz param_2, f32 water_lvl) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz pos;
    dBgS_GndChk gnd_chk;
    dBgS_LinChk lin_chk;
    pos.x = param_2.x;
    pos.y = param_2.y + 3000.0f;
    pos.z = param_2.z;
    lin_chk.Set(&actor->current.pos, &pos, actor);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        pos.y = lin_chk.GetCross().y - 10.0f;;
    }

    dBgS_ObjGndChk_Spl gnd_chk_spl;
    gnd_chk_spl.SetPos(&pos);
    i_this->waterY = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);
    pos.y = i_this->waterY + 60.0f;

    gnd_chk.SetPos(&pos);
    f32 gnd_cross = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if ((i_this->waterY - gnd_cross) > water_lvl) {
        return TRUE;
    }

    return FALSE;
}

static f32 yuka_jump_x = 80.0f;

static int npc_ks_option(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);
    fopAc_ac_c* actor_p;
    cXyz mae, ato;
    f32 target_speed = 0.0f;
    f32 step_speed = 5.0f;
    int unused_0x58 = 0;
    (void) unused_0x58;
    u32 soundID = Z2SE_SY_DUMMY;
    int rt = 1;
    int iVar2 = 1;
    int iVar1 = 1;
    s16 sVar1 = 0x800;
    int frame = i_this->model->getFrame();
    f32 fVar2;
    f32 fVar1 = 0.0f;
    if (fopAcM_GetRoomNo(actor) == 7 || fopAcM_GetRoomNo(actor) == 8) {
        fVar1 = 10000.0f;
    }

    if (daPy_getPlayerActorClass()->checkAutoJumpStart() && i_this->field_0xbd8 < 3) {
        i_this->field_0xbd8++;
    }

    if (i_this->mode >= 30 && i_this->mode < 40) {
        actor_p = bomb_check(i_this);
        if (actor_p == NULL) {
            i_this->mode = 0;
        } else {
            actor->health = 0;
            mae.x = actor_p->current.pos.x - actor->current.pos.x;
            mae.z = actor_p->current.pos.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            fVar2 = JMAFastSqrt(mae.x * mae.x + mae.z * mae.z);
        }
    } else if (i_this->mode >= 40 && i_this->mode < 50) {
        actor_p = enemy_check(i_this, fVar1 + 700.0f);
        if (actor_p == NULL) {
            i_this->mode = 0;
        } else {
            actor->health = 0;
            if (i_this->mode == 40) {
                mae.x = actor_p->current.pos.x - actor->current.pos.x;
                mae.z = actor_p->current.pos.z - actor->current.pos.z;
                i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
                fVar2 = JMAFastSqrt(mae.x * mae.x + mae.z * mae.z);
            }
        }
    }

    f32 dbg_f30 = l_HIO.pull_distance;
    if (checkDoorDemo()) {
        dbg_f30 -= 70.0f;
    }

    switch (i_this->mode) {
        case 0:
            i_this->mode = 1;
            anm_init(i_this, 51, 5.0f, 2, 1.0f);
            i_this->timer[0] = cM_rndF(100.0f) + 100.0f;
            fopAcM_setStageLayer(actor);
            actor->health = 0;
            // fallthrough
        case 1:
            if (i_this->timer[0] == 0) {
                if (cM_rndF(1.0f) < 0.5f) {
                    if (i_this->res_id != 51) {
                        anm_init(i_this, 51, 5.0f, 2, 1.0f);
                        i_this->timer[0] = cM_rndF(100.0f) + 80.0f;
                        soundID = Z2SE_KOSARU_V_WAIT;
                    }
                } else if (i_this->res_id != 53) {
                    anm_init(i_this, 53, 8.0f, 2, 1.0f);
                    i_this->timer[0] = cM_rndF(30.0f) + 40.0f;
                    soundID = Z2SE_KOSARU_V_WAIT;
                }

            }

            if (i_this->timer[1] == 0 && i_this->dis > (dbg_f30 + 20.0f)) {
                anm_init(i_this, 28, 5.0f, 2, 1.0f);
                i_this->mode = 2;
                soundID = Z2SE_KOSARU_V_JUMP;
            }
            break;

        case 2:
            target_speed = l_HIO.holding_speed_l;
            if (i_this->dis < (dbg_f30 - 20.0f)) {
                i_this->mode = 0;
            } else if (i_this->dis > dbg_f30 + 200.0f) {
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
                i_this->mode = 3;
                soundID = Z2SE_KOSARU_V_WALK;
            }

            if (i_this->ObjAcch.ChkWallHit() && i_this->ObjAcch.ChkGroundHit() && !otherBgCheck(actor, player)) {
                actor->speed.y = 30.0f;
            }
            break;
            
        case 3:
            target_speed = l_HIO.holding_speed_h;
            if (i_this->model->checkFrame(1.0f)) {
                soundID = Z2SE_KOSARU_V_WALK;
            }

            if (i_this->dis < (dbg_f30 + 130.0f)) {
                anm_init(i_this, 28, 3.0f, 2, 1.0f);
                i_this->mode = 2;
            }

            if (i_this->ObjAcch.ChkWallHit() && i_this->ObjAcch.ChkGroundHit() && !otherBgCheck(actor, player)) {
                actor->speed.y = 30.0f;
            }
            break;

        case 5:
            i_this->current_angle.y = i_this->target_angle;
            if (frame >= 11) {
                i_this->mode = 0;
            }
            break;

        case 7:
            anm_init(i_this, 47, 3.0f, 2, i_this->field_0x5d4);
            i_this->mode = 8;
            // fallthrough
        case 8:
            if (i_this->model->checkFrame(1.0f)) {
                i_this->sound.startCreatureSound(Z2SE_KOSARU_SWIM, 0, -1);
            }

            iVar1 = 0;
            actor->gravity = 0.0f;
            cLib_addCalc0(&actor->speed.y, 1.0f, 1.0f);
            cLib_addCalc2(&actor->current.pos.y, i_this->waterY, 0.1f, 10.0f);

            if (i_this->dis > (dbg_f30 + 50.0f)) {
                target_speed = 5.0f + JREG_F(8);
            } else {
                target_speed = 0.0f;
            }

            cLib_addCalc2(&i_this->field_0x5d4, target_speed * 0.1f + 0.5f, 1.0f, 0.1f);
            i_this->model->setPlaySpeed(i_this->field_0x5d4);

            step_speed = 0.1f;
            sVar1 = 0x100;
            i_this->current_angle.y = i_this->target_angle;

            if (i_this->timer[0] == 0) {
                i_this->timer[0] = cM_rndF(30.0f) + 20.0f;
                soundID = Z2SE_KOSARU_V_WAIT;
            }
            break;

        case 10:
            i_this->timer[0] = cM_rndF(30.0f) + 20.0f;
            i_this->mode = 11;
            i_this->current_angle.y = i_this->target_angle;
            // fallthrough
        case 11:
            i_this->field_0x5fc = 1;
            if (i_this->timer[0] == 0) {
                if (cM_rndF(1.0f) < 0.5f) {
                    anm_init(i_this, 30, 3.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, 5, 3.0f, 0, 1.0f);
                }

                i_this->mode = 12;
            }
            break;

        case 12:
            i_this->field_0x5fc = 1;
            if (i_this->model->isStop()) {
                if (cM_rndF(1.0f) < 0.5f) {
                    anm_init(i_this, 30, 3.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, 5, 3.0f, 0, 1.0f);
                }

                i_this->mode = 13;
            }
            break;

        case 13:
            if (i_this->model->isStop()) {
                i_this->mode = 0;
            }
            break;

        case 20:
            iVar2 = 0;
            if (i_this->timer[0] == 0) {
                i_this->action = 200;
                i_this->mode = 0;
                i_this->field_0xaec = 0;
                return rt;
            }
            break;

        case 30:
            target_speed = l_HIO.holding_speed_h;
            i_this->current_angle.y += NPC_KS_180DEG_ROT;
            if (fVar2 > 400.0f) {
                i_this->mode = 31;
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
            }
            break;

        case 31:
            i_this->current_angle.y = i_this->target_angle;
            i_this->search_id = fopAcM_GetID(actor_p);
            i_this->field_0x5fc = 1;
            if (fVar2 < 350.0f) {
                i_this->mode = 30;
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mode = 41;
            i_this->timer[0] = cM_rndF(80.0f) + 100.0f;
            // ditto.
            i_this->current_angle.y += NPC_KS_180DEG_ROT;
            break;

        case 41:
            sVar1 = 0x1000;
            if ((i_this->res_id == 39 || (i_this->res_id == 43 && frame < 7)) || ((frame > 40 && frame < 48) || frame > 65)) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->timer[0] == 0 && i_this->res_id == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->res_id == 43) {
                if (i_this->model->isStop()) {
                    i_this->mode = 40;
                }
            }
            break;

        case 50:
            rt = 2;
            actor->speedF = 0.0f;
            if (i_this->timer[0] == 0) {
                fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);
                cMtx_YrotS(*calc_mtx, (s16) player->shape_angle.y);
                mae.y = 50.0f;
                mae.z = 0.0f;

                int i = 0;
                for (; i < 2; i++) {
                    dBgS_GndChk gnd_chk;
                    mae.x = yuka_jump_x;
                    MtxPosition(&mae, &ato);
                    ato += player->current.pos;
                    gnd_chk.SetPos(&ato);
                    ato.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
                    yuka_jump_x *= -1.0f;
                    if (NPC_KS_FABSF(ato.y - player->current.pos.y) < 20.0f) {
                        i_this->mode = 51;
                        i_this->guide_path = ato;
                        i_this->field_0x910 = actor->current.pos;
                        i_this->field_0x91c = i_this->guide_path;
                        anm_init(i_this, 32, 2.0f, 0, 1.0f);
                        break;
                    }
                }
                if (i == 2) {
                    i_this->mode = 0;
                }
            }

            i_this->current_angle.y = i_this->target_angle;
            target_speed = 4096.0f;
            break;

        case 51:
            rt = 2;
            if (i_this->res_id == 32 && i_this->model->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                actor->speedF = 40.0f + TREG_F(9);
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->field_0xbe0 = 1;
            }

            mae = i_this->guide_path - actor->current.pos;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            i_this->current_angle.x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));
            i_this->current_angle.y = i_this->current_angle.y;
            sVar1 = 0x2000;

            if (actor->speedF > 25.0f) {
                i_this->field_0x90c = 1;
            }

            if (mae.abs() <= actor->speedF * 1.2f) {
                i_this->mode = 0;
                actor->speedF *= 0.5f;
                i_this->field_0xbe0 = 1;
                return rt;
            }

            target_speed = actor->speedF;
    }

    cLib_addCalc2(&actor->speedF, target_speed, 1.0f, step_speed);
    if (i_this->mode < 7) {
        i_this->field_0x5fc = 1;
        if (target_speed > 1.0f) {
            i_this->current_angle.y = i_this->target_angle;
        } else {
            s16 range = actor->current.angle.y - i_this->target_angle;
            if ((range > 0x3000 || range < -0x3000) && i_this->mode < 5) {
                anm_init(i_this, 28, 3.0f, 0, 1.0f);
                i_this->mode = 5;
                soundID = Z2SE_KOSARU_V_WALK;
            }
        }

        if (actor->health != 0) {
            actor->health = 0;
            i_this->mode = 10;
            return rt;
        }

        if (!checkDoorDemo()) {
            if ((actor_p = (fopAc_ac_c*)fpcM_Search(s_01_sub, i_this)) != NULL) {
                mae = actor_p->current.pos - actor->current.pos;
                i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
                i_this->mode = 20;
                i_this->timer[0] = 60;
                return rt;
            }
        }

        if (i_this->timer[3] == 0) {
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->search_id = fopAcM_GetID(en_search_test(i_this));
            } else {
                i_this->search_id = -1;
            }

            i_this->timer[3] = cM_rndF(30.0f) + 30.0f;
        }

        if ((i_this->count & 15) == 0 && bomb_view_check(i_this) != NULL) {
            i_this->mode = 30;
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            return rt;
        }

        if ((i_this->count + 2 & 15) == 0 && enemy_view_check(i_this, fVar1 + 600.0f) != NULL) {
            i_this->mode = 40;
            return rt;
        }
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 2, sVar1);
    cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);

    fopAc_ac_c* player3 = (fopAc_ac_c*) dComIfGp_getPlayer(0);
    if (iVar2 != 0) {
        if (NPC_KS_FABSF(player3->current.pos.y - actor->current.pos.y) > 3000.0f ||
            (fopAcM_CheckCondition(actor, 4) != 0 && fopAcM_otherBgCheck(actor, dComIfGp_getPlayer(0)))) {
            if (iVar1 != 0 && player3->speedF > 2.0f) {
                camera_class* camera = (camera_class*) dComIfGp_getCamera(0);
                mae.x = camera->lookat.eye.x - camera->lookat.center.x;
                mae.z = camera->lookat.eye.z - camera->lookat.center.z;
                cMtx_YrotS(*calc_mtx, cM_atan2s(mae.x, mae.z));
                mae.x = 0.0f;
                mae.y = -50.0f;
                mae.z = 100.0f;
                MtxPosition(&mae, &ato);
                ato += camera->lookat.eye;
                
                dBgS_GndChk gnd_chk;
                gnd_chk.SetPos(&ato);
                if (NPC_KS_FABSF(ato.y - dComIfG_Bgsp().GroundCross(&gnd_chk)) < 500.0f) {
                    actor->current.pos = ato;
                    actor->old = actor->current;
                    OS_REPORT("////////KS OP RESET \n");
                } else {
                    OS_REPORT("////////KS OP NO FLOOR・NONRESET \n");
                    soundID = Z2SE_SY_DUMMY;
                }
            } else {
                soundID = Z2SE_SY_DUMMY;
            }
        }
    }

    if (soundID != Z2SE_SY_DUMMY) {
        i_this->sound.startCreatureVoice(soundID, -1);
    }

    if (i_this->anm_time != 0 && (i_this->res_id == 26 || i_this->res_id == 28)) {
        anm_init(i_this, 55, 3.0f, 2, 1.0f);
    } else if (i_this->anm_time == 0 && i_this->res_id == 55) {
        anm_init(i_this, 26, 3.0f, 2, 1.0f);
    }

    if (iVar1 != 0) {
        if (i_this->field_0xbdd != 0 && actor->current.pos.y < i_this->waterY) {
            if (actor->speed.y < -10.0f) {
                actor->speed.y = -10.0f;
            }

            i_this->mode = 7;
            actor->speedF = 0.0f;
            i_this->field_0x5d4 = 0.0f;
            cXyz water_pos = actor->current.pos;
            water_pos.y = i_this->waterY;

            f32 sc_load_val = 1.0f;
            static cXyz sc(sc_load_val, sc_load_val, sc_load_val);
            static u16 w_eff_id[4] = {
                0x01B8, 0x01B9, 0x01BA, 0x01BB
            };

            for (int i = 0; i < 4; i++) {
                i_this->w_eff[i] = dComIfGp_particle_set(i_this->w_eff[i], w_eff_id[i], &water_pos, &actor->tevStr, NULL,
                                                                &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
            }

            i_this->sound.startCreatureSound(Z2SE_AL_INTO_WATER, 0, -1);
            return rt;
        }

        if (i_this->mode < 50 && i_this->field_0xbd8 != 0) {
            i_this->field_0xbd8--;
            i_this->mode = 50;
            i_this->timer[0] = 20 + YREG_S(3);
        }
    } else {
        f32 sc;
        if (actor->speedF >= 3.0f) {
            sc = 0.2f + KREG_F(1);
        } else {
            sc = 0.05f + KREG_F(0);
        }

        cXyz water_pos = actor->current.pos;
        water_pos.y = i_this->waterY;
        fopAcM_effHamonSet(&i_this->hammonPrtcl, &water_pos, 1.5f + KREG_F(2), sc);
        if (i_this->field_0xbdd == 0) {
            i_this->mode = 0;
        }
    }

    action_check(i_this);
    return rt;
}

static void npc_ks_awaydoor(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->timer[0] = 30;
            break;

        case 1:
            if (i_this->timer[0] == 0) {
                fopAcM_delete(actor);
            }
    }

    cXyz mae, ato;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cMtx_YrotS(*calc_mtx, player->shape_angle.y);
    mae.x = 0.0f;
    mae.y = 0.0f;
    mae.z = 150.0f;
    MtxPosition(&mae, &ato);
    ato += player->current.pos;
    mae.x = actor->current.pos.x - ato.x;
    mae.z = actor->current.pos.z - ato.z;
    cLib_addCalcAngleS2(&actor->current.angle.y, cM_atan2s(mae.x, mae.z), 1, 0x2000);
    cLib_addCalc2(&actor->speedF, l_HIO.holding_speed_h, 1.0f, 4.0f);
}

static path guide_path_00[10] = {
    { 0, 54.0f, 3300.0f, 5328.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 39.0f, 3300.0f, 5592.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 15.0f, 3276.0f, 5849.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 8.0f, 3234.0f, 6132.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, -1.0f, 3198.0f, 6372.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 5.0f, 3162.0f, 6620.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 46.0f, 3150.0f, 6877.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 159.0f, 3150.0f, 7102.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 292.0f, 3150.0f, 7219.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { -1, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

static path guide_path_00_2[7] =  {
    { 0, 0.0f, 3150.0f, 6775.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 0, 9.0f, 3195.0f, 6449.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 0, 1.0f, 3255.0f, 6044.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 0, 1.0f, 3300.0f, 5612.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, -2.0f, 3300.0f, 5108.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { -1, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { -1, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

static path guide_path_00_3[6] = {
    { 0, -81.0f, 3150.0f, 6492.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 87.0f, 3200.0f, 6191.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 121.0f, 3250.0f, 5857.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 160.0f, 3300.0f, 5622.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 48.0f, 3300.0f, 4906.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { -1, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

static path guide_path_01[12] = {
    { 0, 5262.0f, 3213.0f, 7710.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 5968.0f, 3250.0f, 7167.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 6250.0f, 3320.0f, 6656.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 6321.0f, 3420.0f, 6432.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 6506.0f, 3420.0f, 5962.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 6930.0f, 3423.0f, 5477.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 7029.0f, 3356.0f, 4814.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 7847.0f, 3373.0f, 4538.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 8410.0f, 3477.0f, 4530.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 9200.0f, 3453.0f, 4839.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 10240.0f, 3751.0f, 4888.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { -1, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

static int npc_ks_guide_00(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz mae, ato;
    int rv = 1;
    int frame = i_this->model->getFrame();

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->path_no = 0;
            actor->speedF = 0.0f;
            // fallthrough
        case 1:
            if (i_this->dis > 700.0f || guide_path_00[i_this->path_no].field_0x0 < 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
                actor->speedF = 0.0f;
            } else {
                i_this->guide_path.x = guide_path_00[i_this->path_no].field_0x4;
                i_this->guide_path.z = guide_path_00[i_this->path_no].field_0xc;
                i_this->mode = 5;
                actor->speedF = l_HIO.demo_speed_2;
                if (i_this->res_id != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 5:
            mae.x = i_this->guide_path.x - actor->current.pos.x;
            mae.z = i_this->guide_path.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < actor->speedF * 1.2f) {
                i_this->path_no++;
                i_this->mode = 1;
            }
            break;

        case 10:
            if (i_this->res_id == 8) {
                i_this->current_angle.y = i_this->target_angle + 0x8000;
            } else {
                i_this->current_angle.y = i_this->target_angle;
            }

            if (i_this->dis < 550.0f && guide_path_00[i_this->path_no].field_0x0 >= 0) {
                i_this->mode = 1;
            } else {
                if (i_this->dis < 150.0f && i_this->res_id != 51) {
                    anm_init(i_this, 51, 5.0f, 2, 1.0f);
                }

                if (i_this->res_id == 51) {
                    i_this->field_0x5fc = 1;
                }

                if (guide_path_00[i_this->path_no].field_0x0 < 0 && saru_count_check(i_this) != 0) {
                    mae.x = player->current.pos.x - i_this->child_no->field_0x904[0].x;
                    mae.y = player->current.pos.y - (i_this->child_no->field_0x904[0].y - 450.0f);
                    mae.z = player->current.pos.z - i_this->child_no->field_0x904[0].z;

                    if (mae.abs() < 300.0f) {
                        i_this->field_0xbd9 = 1;
                        i_this->action = 20;
                        i_this->mode = 0;
                        i_this->field_0xaec = 0;
                        i_this->field_0xbc8 = i_this->child_no->field_0x904[0];
                        mae = i_this->child_no->field_0x904[1] - i_this->field_0xbc8;
                        leader->field_0xbd4 = cM_atan2s(mae.x, mae.z);
                        leader->demo_mode = 10;
                        leader->field_0xb40 = i_this->child_no->field_0x570;
                        if (i_this->set_id != 0) {
                            leader->child_no = i_this->child_no;
                            leader->field_0xbc8 = i_this->field_0xbc8;
                        }
                    }
                }
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mode = 41;
            i_this->timer[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if ((i_this->res_id == 39 || (i_this->res_id == 43 && frame < 7) || (frame > 40 && frame < 48)) || frame > 65) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->timer[0] == 0 && i_this->res_id == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->res_id == 43) {
                if (i_this->model->isStop()) {
                    i_this->mode = 40;
                }
            }
            break;

        case 42:
            if (i_this->timer[0] == 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
            }
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 1, 0x1000);
    s16 sVar1 = actor->current.angle.y - i_this->current_angle.y;
    if (sVar1 > 0x1000 || sVar1 < -0x1000) {
        rv = 0;
    }

    cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->action = 100;
        i_this->mode = 0;
    }

    if (i_this->mode >= 40 && i_this->mode <= 41) {
        fopAc_ac_c* enemy_p = enemy_check(i_this, 800.0f);
        if (enemy_p == NULL) {
            i_this->mode = 42;
            i_this->timer[0] = 30;
        } else {
            mae.x = enemy_p->current.pos.x - actor->current.pos.x;
            mae.z = enemy_p->current.pos.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z) + 0x8000;
        }
    } else {
        if (enemy_view_check(i_this, 700.0f) != NULL) {
            i_this->mode = 40;
            actor->speedF = 0.0f;
        }
    }

    return rv;
}

static int npc_ks_guide_00_2(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz mae, ato;
    int rv = 1;
    int frame = i_this->model->getFrame();

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->path_no = 0;
            actor->speedF = 0.0f;
            // fallthrough
        case 1:
            if (i_this->dis > 900.0f || guide_path_00_2[i_this->path_no].field_0x0 < 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
                actor->speedF = 0.0f;
            } else {
                i_this->guide_path.x = guide_path_00_2[i_this->path_no].field_0x4;
                i_this->guide_path.z = guide_path_00_2[i_this->path_no].field_0xc;
                i_this->mode = 5;
                actor->speedF = l_HIO.demo_speed_2;
                if (i_this->res_id != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 5:
            mae.x = i_this->guide_path.x - actor->current.pos.x;
            mae.z = i_this->guide_path.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < actor->speedF * 1.2f) {
                if (i_this->set_id == 0 || i_this->set_id == 1) {
                    i_this->path_no++;
                }

                i_this->path_no++;
                i_this->mode = 1;
            }
            break;

        case 10:
            if (i_this->res_id == 7) {
                i_this->current_angle.y = i_this->target_angle;
            } else {
                i_this->current_angle.y = i_this->target_angle + 0x8000;
            }

            if (i_this->dis < 750.0f && guide_path_00_2[i_this->path_no].field_0x0 >= 0) {
                i_this->mode = 1;
            }
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 1, 0x1000);

    s16 sVar1 = actor->current.angle.y - i_this->current_angle.y;
    if (sVar1 > 0x1000 || sVar1 < -0x1000) {
        rv = 0;
    }

    cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->action = 100;
        i_this->mode = 0;
    }

    return rv;
}

static int npc_ks_guide_00_3(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz mae, ato;
    int rv = 1;
    int frame = i_this->model->getFrame();

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->path_no = 0;
            actor->speedF = 0.0f;
            // fallthrough
        case 1:
            if (i_this->dis > 500.0f || guide_path_00_3[i_this->path_no].field_0x0 < 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
                actor->speedF = 0.0f;
            } else {
                i_this->guide_path.x = guide_path_00_3[i_this->path_no].field_0x4;
                i_this->guide_path.z = guide_path_00_3[i_this->path_no].field_0xc;
                i_this->mode = 5;
                actor->speedF = l_HIO.demo_speed_2;
                if (i_this->res_id != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 5:
            mae.x = i_this->guide_path.x - actor->current.pos.x;
            mae.z = i_this->guide_path.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < actor->speedF * 1.2f) {
                i_this->path_no++;
                i_this->mode = 1;
            }
            break;

        case 10:
            if (i_this->res_id == 8) {
                i_this->current_angle.y = i_this->target_angle + 0x8000;
            } else {
                i_this->current_angle.y = i_this->target_angle;
            }

            if (i_this->dis < 400.0f && guide_path_00_3[i_this->path_no].field_0x0 >= 0) {
                i_this->mode = 1;
            } else {
                if (i_this->dis < 150.0f && i_this->res_id != 51) {
                    anm_init(i_this, 51, 5.0f, 2, 1.0f);
                }

                if (i_this->res_id == 51) {
                    i_this->field_0x5fc = 1;
                }
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mode = 41;
            i_this->timer[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if (i_this->res_id == 39 || (i_this->res_id == 43 && frame < 7) || ((frame > 40 && frame < 48) || frame > 65)) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->timer[0] == 0 && i_this->res_id == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->res_id == 43) {
                if (i_this->model->isStop()) {
                    i_this->mode = 40;
                }
            }
            break;

        case 42:
            if (i_this->timer[0] == 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
            }
    }

    cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->action = 100;
        i_this->mode = 0;
    }

    s16 sVar1 = actor->current.angle.y - i_this->current_angle.y;
    if (sVar1 > 0x1000 || sVar1 < -0x1000) {
        rv = 0;
    }

    if (i_this->mode >= 40 && i_this->mode <= 41) {
        fopAc_ac_c* enemy_p = enemy_check(i_this, 700.0f);
        if (enemy_p == NULL) {
            i_this->mode = 42;
            i_this->timer[0] = 30;
        } else {
            mae.x = enemy_p->current.pos.x - actor->current.pos.x;
            mae.z = enemy_p->current.pos.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z) + 0x8000;
        }
    } else if (enemy_view_check(i_this, 600.0f) != NULL) {
        i_this->mode = 40;
        actor->speedF = 0.0f;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 1, 0x1000);
    return rv;
}

static int npc_ks_guide_01(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz mae, ato;
    int rv = 1;
    int frame = i_this->model->getFrame();

    f32 fVar1 = 0.0f;
    if (fopAcM_GetRoomNo(actor) == 1 && i_this->path_no >= 5) {
        fVar1 = 300.0f;
    }

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->path_no = 0;
            actor->speedF = 0.0f;
            // fallthrough
        case 1:
        case 101:
            if (i_this->mode == 1 && (i_this->dis > 900.0f || guide_path_01[i_this->path_no].field_0x0 < 0)) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
                actor->speedF = 0.0f;
            } else if (guide_path_01[i_this->path_no].field_0x0 != 0) {
                i_this->guide_path.x = guide_path_01[i_this->path_no].field_0x4;
                i_this->guide_path.y = guide_path_01[i_this->path_no].field_0x8;
                i_this->guide_path.z = guide_path_01[i_this->path_no].field_0xc;
                i_this->field_0x910 = actor->current.pos;
                i_this->field_0x91c = i_this->guide_path;
                i_this->mode = 2;
                anm_init(i_this, 32, 2.0f, 0, 1.0f);
                actor->speedF = 0.0f;
            } else {
                i_this->guide_path.x = guide_path_01[i_this->path_no].field_0x4;
                i_this->guide_path.z = guide_path_01[i_this->path_no].field_0xc;
                i_this->mode = 5;
                actor->speedF = l_HIO.demo_speed_2;
                if (i_this->res_id != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 2:
            rv = 2;
            if (i_this->res_id == 32 && i_this->model->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                actor->speedF = 40.0f;
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                i_this->field_0xbe0 = 1;
            }

            mae = i_this->guide_path - actor->current.pos;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            i_this->current_angle.x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));
            if (mae.abs() < actor->speedF * 1.2f) {
                actor->current.pos = i_this->guide_path;
                i_this->mode = 1;
                i_this->field_0xbe0 = 1;
                i_this->path_no++;
            }
            break;

        case 5:
            mae.x = i_this->guide_path.x - actor->current.pos.x;
            mae.z = i_this->guide_path.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < actor->speedF * 1.2f) {
                i_this->path_no++;
                i_this->mode = 1;
            }
            break;

        case 10:
            if (i_this->res_id == 8) {
                i_this->current_angle.y = i_this->target_angle + 0x8000;
            } else {
                i_this->current_angle.y = i_this->target_angle;
            }

            if (guide_path_01[i_this->path_no].field_0x0 >= 0) {
                if (i_this->dis < 600.0f) {
                    i_this->mode = 1;
                }

                fopAc_ac_c* player = dComIfGp_getPlayer(0);
                mae.x = player->current.pos.x - 9200.0f;
                mae.z = player->current.pos.z - 4839.0f;
                ato.x = actor->current.pos.x - 9200.0f;
                ato.z = actor->current.pos.z - 4839.0f;

                if ((mae.x * mae.x + mae.z * mae.z) < (ato.x * ato.x + ato.z * ato.z)) {
                    i_this->mode = 101;
                }
            } else if (i_this->dis < 150.0f) {
                i_this->action = 100;
                i_this->mode = 0;
                return 1;
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mode = 41;
            i_this->timer[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if (i_this->res_id == 39 || (i_this->res_id == 43 && frame < 7) || ((frame > 40 && frame < 48) ) || frame > 65) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->timer[0] == 0 && i_this->res_id == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->res_id == 43 && i_this->model->isStop()) {
                i_this->mode = 40;
            }
            break;

        case 42:
            if (i_this->timer[0] == 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
            }
            break;
    }

    cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->action = 100;
        i_this->mode = 0;
    }

    if (actor->speedF > 30.0f) {
        i_this->field_0x90c = 1;
    } else {
        s16 sVar1 = actor->current.angle.y - i_this->current_angle.y;
        if (sVar1 > 0x1000 || sVar1 < -0x1000) {
            rv = 0;
        }
    }

    if (i_this->mode >= 40 && i_this->mode <= 41) {
        fopAc_ac_c* enemy_p = enemy_check(i_this, fVar1 + 700.0f);
        if (enemy_p == NULL) {
            i_this->mode = 42;
            i_this->timer[0] = 30;
        } else {
            mae.x = enemy_p->current.pos.x - actor->current.pos.x;

            mae.z = enemy_p->current.pos.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z) + 0x8000;
        }
    } else if (enemy_view_check(i_this, fVar1 + 600.0f) != NULL) {
        i_this->mode = 40;
        actor->speedF = 0.0f;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 1, 0x1000);

    return rv;
}

static path guide_path_02[21] = {
    { 0, 12307.0f, 3152.0f, 5237.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 12929.0f, 3152.0f, 5512.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 13490.0f, 3202.0f, 5456.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 13885.0f, 3352.0f, 5122.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 14053.0f, 3402.0f, 4796.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 14020.0f, 3494.0f, 4337.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 13709.0f, 3579.0f, 3878.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 13473.0f, 3695.0f, 3649.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 13080.0f, 3752.0f, 3455.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 12889.0f, 3764.0f, 3265.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { -1, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 12748.0f, 3152.0f, 4865.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 13097.0f, 3152.0f, 5282.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 13586.0f, 3202.0f, 5575.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 13927.0f, 3302.0f, 5421.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 14210.0f, 3402.0f, 4899.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 14112.0f, 3494.0f, 4217.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 13665.0f, 3656.0f, 3593.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 13341.0f, 3735.0f, 3363.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 13019.0f, 3752.0f, 3436.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { -1, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

static int npc_ks_guide_02(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz mae, ato;
    int rv = 1;
    f32 fVar1;
    int frame_int = i_this->model->getFrame();
    if (i_this->set_id == 0) {
        fVar1 = 1000.0f + YREG_F(5);
    } else {
        fVar1 = 700.0f + YREG_F(6);
    }

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mode = 1;
            if (i_this->set_id == 0) {
                i_this->path_no = 0;
            } else {
                i_this->path_no = 11;
            }

            actor->speedF = 0.0f;
            // fallthrough
        case 1:
            if (i_this->timer[2] == 0 && (((player->current.pos.y - actor->current.pos.y) < 10.0f && i_this->dis > fVar1) || 
                guide_path_02[i_this->path_no].field_0x0 < 0)) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
                actor->speedF = 0.0f;
            } else {
                i_this->guide_path.x = guide_path_02[i_this->path_no].field_0x4;
                i_this->guide_path.z = guide_path_02[i_this->path_no].field_0xc;
                i_this->mode = 5;
                actor->speedF = l_HIO.demo_speed_2;
                if (i_this->res_id != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 5:
            if (i_this->ObjAcch.ChkWallHit() != 0) {
                actor->speed.y = 20.0f;
            }

            mae.x = i_this->guide_path.x - actor->current.pos.x;
            mae.z = i_this->guide_path.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < actor->speedF * 1.2f) {
                i_this->path_no++;
                i_this->mode = 1;
            }
            break;

        case 10:
            if (i_this->res_id == 7) {
                i_this->current_angle.y = i_this->target_angle;
            } else {
                i_this->current_angle.y = i_this->target_angle + 0x8000;
            }

            if (((player->current.pos.y - actor->current.pos.y) > 10.0f || i_this->dis < (fVar1 - 150.0f)) && guide_path_02[i_this->path_no].field_0x0 >= 0) {
                i_this->mode = 1;
            }

            if (guide_path_02[i_this->path_no].field_0x0 < 0 && i_this->set_id == 0 && saru_count_check(i_this) != 0) {
                fopAc_ac_c* player_2 = (fopAc_ac_c*) dComIfGp_getPlayer(0);
                int iVar1 = 0;
                for (int i = 0; i < i_this->child_no->field_0x91c; i++) {
                    mae = player_2->current.pos - saru_p[i]->actor.current.pos;
                    if (mae.abs() > 400.0f) {
                        iVar1 = 1;
                    }
                }

                mae.x = player->current.pos.x - i_this->child_no->field_0x904[0].x;
                mae.y = player->current.pos.y - (i_this->child_no->field_0x904[0].y - 450.0f);
                mae.z = player->current.pos.z - i_this->child_no->field_0x904[0].z;

                if (iVar1 == 0 && mae.abs() < 300.0f) {
                    i_this->field_0xbd9 = 1;
                    i_this->field_0xbc8 = i_this->child_no->field_0x904[0];
                    i_this->action = 20;
                    i_this->mode = 0;
                    i_this->field_0xaec = 0;
                    leader->demo_mode = 10;
                    leader->field_0xb40 = 0;
                    leader->field_0xbc8 = i_this->field_0xbc8;

                    mae = i_this->child_no->field_0x904[1] - i_this->field_0xbc8;
                    leader->field_0xbd4 = cM_atan2s(mae.x, mae.z);
                    saru_count_check(saru_p[1]);
                    saru_p[1]->action = 22;
                    saru_p[1]->mode = 0;
                    saru_p[1]->field_0x930 = leader;
                }
            }
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 1, 0x1000);

    s16 sVar1 = actor->current.angle.y - i_this->current_angle.y;
    if (sVar1 > 0x1000 || sVar1 < -0x1000) {
        rv = 0;
    }

    cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->action = 100;
        i_this->mode = 0;
    }

    return rv;
}

static path guide_path_22[6] = {
    { 0, 0.0f, 2354.0f, 12941.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 0.0f, 3000.0f, 12879.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0.0f, 3150.0f, 12556.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0.0f, 3150.0f, 12164.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0.0f, 3150.0f, 11695.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { -1, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

static int npc_ks_guide_22(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    i_this->dis = fopAcM_searchPlayerDistance(actor);
    cXyz mae, ato;
    int rv = 1;
    int frame = i_this->model->getFrame();

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->path_no = 0;
            actor->speedF = 0.0f;
            // fallthrough
        case 1:
            if (i_this->dis > 500.0f || guide_path_22[i_this->path_no].field_0x0 < 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
                actor->speedF = 0.0f;
            } else if (guide_path_22[i_this->path_no].field_0x0 != 0) {
                anm_init(i_this, 12, 3.0f, 2, 1.0f);
                i_this->guide_path.x = guide_path_22[i_this->path_no].field_0x4;
                i_this->guide_path.y = guide_path_22[i_this->path_no].field_0x8;
                i_this->guide_path.z = guide_path_22[i_this->path_no].field_0xc;
                i_this->mode = 2;
                actor->speedF = 0.0f;
            } else {
                i_this->guide_path.x = guide_path_22[i_this->path_no].field_0x4;
                i_this->guide_path.z = guide_path_22[i_this->path_no].field_0xc;
                i_this->mode = 5;
                actor->speedF = l_HIO.demo_speed_2;
                
                if (i_this->res_id != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 2:
            rv = 0;
            
            mae.x = i_this->guide_path.x - actor->current.pos.x;
            mae.z = i_this->guide_path.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            cLib_addCalc2(&actor->current.pos.y, i_this->guide_path.y, 1.0f, 15.0f);

            if (actor->current.pos.y >= (i_this->guide_path.y - 110.0f) && i_this->res_id != 26) {
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
            }

            if (actor->current.pos.y >= (i_this->guide_path.y - 50.0f)) {
                cLib_addCalc2(&actor->current.pos.x, i_this->guide_path.x, 1.0f, 20.0f);
                cLib_addCalc2(&actor->current.pos.z, i_this->guide_path.z, 1.0f, 20.0f);
            }

            if (actor->current.pos.y >= i_this->guide_path.y) {
                actor->current.pos = i_this->guide_path;
                i_this->path_no++;
                i_this->mode = 1;
            }
            break;

        case 5:
            mae.x = i_this->guide_path.x - actor->current.pos.x;
            mae.z = i_this->guide_path.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);

            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < actor->speedF * 1.2f) {
                i_this->path_no++;
                i_this->mode = 1;
            }
            break;

        case 10:
            if (i_this->res_id == 8) {
                i_this->current_angle.y = i_this->target_angle + 0x8000;
            } else {
                i_this->current_angle.y = i_this->target_angle;
            }

            if (i_this->dis < 300.0f && guide_path_22[i_this->path_no].field_0x0 >= 0) {
                i_this->mode = 1;
            }

            if (i_this->dis < 150.0f && i_this->res_id != 51) {
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
            }

            if (i_this->res_id == 51) {
                i_this->field_0x5fc = 1;
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mode = 41;
            i_this->timer[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if ((i_this->res_id == 39 || (i_this->res_id == 43 && frame < 7) || (frame > 40 && frame < 48)) || frame > 65) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->timer[0] == 0 && i_this->res_id == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->res_id == 43 && i_this->model->isStop()) {
                i_this->mode = 40;
            }
            break;

        case 42:
            if (i_this->timer[0] == 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
            }
    }

    cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->action = 100;
        i_this->mode = 0;
    }

    if (actor->speedF > 30.0f) {
        i_this->field_0x90c = 1;
    } else {
        s16 sVar1 = actor->current.angle.y - i_this->current_angle.y;
        if (sVar1 > 0x1000 || sVar1 < -0x1000) {
            rv = 0;
        }
    }

    if (i_this->mode >= 40 && i_this->mode <= 41) {
        fopAc_ac_c* enemy_p = enemy_check(i_this, 700.0f);
        if (enemy_p == NULL) {
            i_this->mode = 42;
            i_this->timer[0] = 30;
        } else {
            mae.x = enemy_p->current.pos.x - actor->current.pos.x;
            mae.z = enemy_p->current.pos.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z) + 0x8000;
        }
    } else if (enemy_view_check(i_this, 600.0f) != NULL) {
        i_this->mode = 40;
        actor->speedF = 0.0f;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 1, 0x1000);
    
    return rv;
}

static path guide_path_09[9] = {
    { 0, 7395.0f, 3273.0f, -4866.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 7404.0f, 3273.0f, -5527.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 7404.0f, 3225.0f, -6251.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 7415.0f, 3225.0f, -6578.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 7386.0f, 3407.0f, -6691.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 7335.0f, 3521.0f, -7005.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 7229.0f, 3600.0f, -7728.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 7319.0f, 3602.0f, -8395.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { -1, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

static int npc_ks_guide_09(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz mae, ato;
    int rv = 1;
    int frame = i_this->model->getFrame();
    f32 fVar1 = 0.0f;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->path_no = 0;
            actor->speedF = 0.0f;
            // fallthrough
        case 1:
            if (i_this->dis > 900.0f || guide_path_09[i_this->path_no].field_0x0 < 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
                actor->speedF = 0.0f;
            } else if (guide_path_09[i_this->path_no].field_0x0 != 0) {
                i_this->guide_path.x = guide_path_09[i_this->path_no].field_0x4;
                i_this->guide_path.y = guide_path_09[i_this->path_no].field_0x8;
                i_this->guide_path.z = guide_path_09[i_this->path_no].field_0xc;
                i_this->field_0x910 = actor->current.pos;
                i_this->field_0x91c = i_this->guide_path;
                i_this->mode = 2;
                anm_init(i_this, 32, 2.0f, 0, 1.0f);
                actor->speedF = 0.0f;
            } else {
                i_this->guide_path.x = guide_path_09[i_this->path_no].field_0x4;
                i_this->guide_path.z = guide_path_09[i_this->path_no].field_0xc;
                i_this->mode = 5;
                actor->speedF = l_HIO.demo_speed_2;
                
                if (i_this->res_id != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 2:
            rv = 2;
            if (i_this->res_id == 32 && i_this->model->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                actor->speedF = 40.0f;
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
            }

            mae = i_this->guide_path - actor->current.pos;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            i_this->current_angle.x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));

            if (mae.abs() < actor->speedF * 1.2f) {
                actor->current.pos = i_this->guide_path;
                i_this->mode = 1;
                i_this->path_no++;
            }
            break;

        case 5:
            mae.x = i_this->guide_path.x - actor->current.pos.x;
            mae.z = i_this->guide_path.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);

            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < actor->speedF * 1.2f) {
                i_this->path_no++;
                i_this->mode = 1;
            }
            break;

        case 10:
            if (i_this->res_id == 8) {
                i_this->current_angle.y = i_this->target_angle + 0x8000;
            } else {
                i_this->current_angle.y = i_this->target_angle;
            }

            if (guide_path_09[i_this->path_no].field_0x0 >= 0) {
                if (i_this->dis < 600.0f) {
                    i_this->mode = 1;
                }
            } else {
                if (i_this->dis < 150.0f && i_this->res_id != 51) {
                    anm_init(i_this, 51, 5.0f, 2, 1.0f);
                }
                
                if (i_this->res_id == 51) {
                    i_this->field_0x5fc = 1;
                }
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mode = 41;
            i_this->timer[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if ((i_this->res_id == 39 || (i_this->res_id == 43 && frame < 7) || (frame > 40 && frame < 48)) || frame > 65) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->timer[0] == 0 && i_this->res_id == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->res_id == 43 && i_this->model->isStop()) {
                i_this->mode = 40;
            }
            break;

        case 42:
            if (i_this->timer[0] == 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
            } 
    }

        cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->action = 100;
        i_this->mode = 0;
    }

    if (actor->speedF > 30.0f) {
        i_this->field_0x90c = 1;
    } else {
        s16 sVar1 = actor->current.angle.y - i_this->current_angle.y;
        if (sVar1 > 0x1000 || sVar1 < -0x1000) {
            rv = 0;
        }
    }

    if (i_this->mode >= 40 && i_this->mode <= 41) {
        fopAc_ac_c* enemy_p = enemy_check(i_this, 700.0f);
        if (enemy_p == NULL) {
            i_this->mode = 42;
            i_this->timer[0] = 30;
        } else {
            mae.x = enemy_p->current.pos.x - actor->current.pos.x;
            mae.z = enemy_p->current.pos.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z) + 0x8000;
        }
    } else if (enemy_view_check(i_this, 600.0f) != NULL) {
        i_this->mode = 40;
        actor->speedF = 0.0f;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 1, 0x1000);
    
    return rv;
}

static path guide_path_12[5] = {
    { 0, 7370.0f, 3600.0f, -9473.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 7500.0f, 3698.0f, -9867.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 8130.0f, 3378.0f, -10770.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 8750.0f, 3675.0f, -10744.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { -1, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

static int npc_ks_demo_12(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz mae, ato;
    int rv = 1;
    int frame = i_this->model->getFrame();

    switch (i_this->mode) {
        case 0:
            if (i_this->demo_mode >= 79) {
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
                i_this->mode = 1;
                i_this->path_no = 0;
            }

            actor->speedF = 0.0f;
            break;

        case 1:
            if (guide_path_12[i_this->path_no].field_0x0 != 0) {
                i_this->guide_path.x = guide_path_12[i_this->path_no].field_0x4;
                i_this->guide_path.y = guide_path_12[i_this->path_no].field_0x8;
                i_this->guide_path.z = guide_path_12[i_this->path_no].field_0xc;
                i_this->field_0x910 = actor->current.pos;
                i_this->field_0x91c = i_this->guide_path;
                i_this->mode = 2;
                anm_init(i_this, 32, 2.0f, 0, 1.0f);
                actor->speedF = 0.0f;
            } else {
                i_this->guide_path.x = guide_path_12[i_this->path_no].field_0x4;
                i_this->guide_path.z = guide_path_12[i_this->path_no].field_0xc;
                i_this->mode = 5;
                actor->speedF = l_HIO.demo_speed_2;

                if (i_this->res_id != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 2:
            rv = 2;
            if (i_this->res_id == 32 && i_this->model->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                actor->speedF = 40.0f;
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                i_this->field_0xbe0 = 1;
            }

            mae = i_this->guide_path - actor->current.pos;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            i_this->current_angle.x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));

            if (mae.abs() < actor->speedF * 1.2f) {
                actor->current.pos = i_this->guide_path;
                i_this->mode = 1;
                i_this->field_0xbe0 = 1;
                i_this->path_no++;
            }
            break;

        case 5:
            mae.x = i_this->guide_path.x - actor->current.pos.x;
            mae.z = i_this->guide_path.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);

            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < actor->speedF * 1.2f) {
                i_this->path_no++;
                i_this->mode = 1;
            }
    }

    cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);

    if (actor->speedF > 30.0f) {
        i_this->field_0x90c = 1;
    } else {
        s16 sVar1 = actor->current.angle.y - i_this->current_angle.y;
        if (sVar1 > 0x1000 || sVar1 < -0x1000) {
            rv = 0;
        }
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 1, 0x1000);

    if (guide_path_12[i_this->path_no].field_0x0 < 0) {
        i_this->action = 8;
        actor->speedF = 0.0f;
    }

    return rv;
}

static path guide_path_0409[5] = {
    { 0, 6939.0f, 3462.0f, -230.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 7077.0f, 3518.0f, -866.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 7120.0f, 3591.0f, -1596.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 7292.0f, 3613.0f, -2636.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { -1, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

static int npc_ks_guide_0409(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz mae;
    int rv = 1;
    int frame = i_this->model->getFrame();

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mode = 1;
            i_this->path_no = 0;
            actor->speedF = 0.0f;
            // fallthrough
        case 1:
            if (player->current.pos.z > actor->current.pos.z && (i_this->dis > 700.0f || guide_path_0409[i_this->path_no].field_0x0 < 0)) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
                actor->speedF = 0.0f;
            } else {
                i_this->guide_path.x = guide_path_0409[i_this->path_no].field_0x4;
                i_this->guide_path.z = guide_path_0409[i_this->path_no].field_0xc;
                i_this->mode = 5;
                actor->speedF = l_HIO.demo_speed_2;

                if (i_this->res_id != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 5:
            mae.x = i_this->guide_path.x - actor->current.pos.x;
            mae.z = i_this->guide_path.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);

            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < actor->speedF * 1.2f) {
                i_this->path_no++;
                i_this->mode = 1;
            }
            break;

        case 10:
            if (i_this->res_id == 8) {
                i_this->current_angle.y = i_this->target_angle + 0x8000;
            } else {
                i_this->current_angle.y = i_this->target_angle;
            }

            if ((i_this->dis < 550.0f || player->current.pos.z < actor->current.pos.z)
                && guide_path_0409[i_this->path_no].field_0x0 >= 0) {
                i_this->mode = 1;
            } else {
                if (i_this->dis < 150.0f && i_this->res_id != 51) {
                    anm_init(i_this, 51, 5.0f, 2, 1.0f);
                }

                if (i_this->res_id == 51) {
                    i_this->field_0x5fc = 1;
                }
            }
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 1, 0x1000);

    s16 sVar1 = actor->current.angle.y - i_this->current_angle.y;
    if (sVar1 > 0x1000 || sVar1 < -0x1000) {
        rv = 0;
    }

    cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        f32 fVar1 = 7280.0f - player->current.pos.x;
        f32 fVar2 = -2709.0f - player->current.pos.z;
        if (JMAFastSqrt(fVar1 * fVar1 + fVar2 * fVar2) < 500.0f) {
            i_this->action = 100;
            i_this->mode = 0;
        }
    }

    return rv;
}

static int path_search(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    int path_index = 0;
    while (path_index < 255) {
        dPath* path_p = dPath_GetRoomPath(path_index, fopAcM_GetRoomNo(actor));
        if (path_p != NULL) {
            cXyz sp28 = actor->current.pos - path_p->m_points->m_position;
            if (sp28.abs() < 200.0f) {
                OS_REPORT("  サルがが%d番のレール発見しました。\n", path_index);
                i_this->field_0xc18 = path_p;
                return 1;
            }
        }

        path_index++;
    }

    i_this->field_0xc18 = 0;
    return 0;
}

static int npc_ks_mori(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz mae, ato;
    int rv = 1;
    int frame = i_this->model->getFrame();
    f32 fVar2 = 1.25f + TREG_F(17);
    f32 fVar1 = l_HIO.link_approach_distance;

    switch (i_this->mode) {
        case 0:
                /* dSv_event_flag_c::F_0225 - Faron Woods - Lanter guide monkey doesn't come out a second time */
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[225])) {
                     /* dSv_event_flag_c::F_0226 - Faron Woods - Get lantern back from monkey */
                if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[226])) {
                    i_this->mode = 302;
                    i_this->field_0xc17 = 3;
                    dComIfGp_setItemOilCount(-(dComIfGs_getMaxOil() & 0xFFFF));
                }
            } else {
                i_this->field_0xaec = 1;
                    /* dSv_event_flag_c::F_0224 - Faron Woods - Flag for lantern guide monkey cutscene */
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[224])) {
                    i_this->mode = 3;
                    call_pt = 0;
                    i_this->field_0xc17 = 2;
                    actor->current.angle.y = player->shape_angle.y + 0x8000;
                    i_this->current_angle.y = actor->current.angle.y;
                } else {
                    actor->current.pos.y = actor->home.pos.y + 10000.0f;
                    actor->speed.y = 0.0f;

                    if (!daPy_getPlayerActorClass()->checkKandelaarSwing(1)) {
                        if (!daPy_getPlayerActorClass()->getKandelaarFlamePos()) break;
                        if (!daPy_getPlayerActorClass()->checkUseKandelaar(0)) break;
                    }

                    mae.set(-27426.0f, player->current.pos.y, -20000.0f);
                    mae -= player->current.pos;
                    if (mae.abs() < 7500.0f) {
                        i_this->mode = 1;
                        i_this->timer[0] = 10;
                    }
                }
            }
            break;

        case 1:
            actor->current.pos.y = actor->home.pos.y + 10000.0f;
            actor->speed.y = 0.0f;
            if (i_this->timer[0] != 1) break;

            if (daPy_getPlayerActorClass()->checkKandelaarSwing(1) || (daPy_getPlayerActorClass()->getKandelaarFlamePos() != NULL && 
                daPy_getPlayerActorClass()->checkUseKandelaar(0))) {
                i_this->demo_mode = 200;
                anm_init(i_this, 44, 0.0f, 2, 1.0f);
            } else {
                i_this->mode = 0;
            }
            break;

        case 2:
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            mae.x = i_this->field_0xbb4;
            mae.y = 0.0f;
            mae.z = 100.0f;
            MtxPosition(&mae, &ato);
            actor->current.pos = player->current.pos + ato;
            actor->current.angle.y = player->shape_angle.y - 0x4000;
            i_this->current_angle.y = actor->current.angle.y;
            cLib_addCalc2(&i_this->field_0xbb4, -500.0f, 1.0f, 20.0f);
            break;

        case 230:
            actor->speedF = 20.0f;
            if (i_this->timer[0] == 0) {
                i_this->mode = 231;
                actor->speedF = 0.0f;
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
                i_this->timer[0] = 30;
            }
            break;

        case 231:
            if (i_this->timer[0] == 0) {
                anm_init(i_this, 44, 3.0f, 2, 1.0f);
                i_this->timer[0] = 110;
                i_this->mode = 232;
            }
            break;

        case 232:
            if (i_this->timer[0] == 40) {
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
            }

            if (i_this->timer[0] == 0) {
                i_this->mode = 3;
                call_pt = 1;
            }
            break;

        case 3:
            if (path_search(i_this) != 0) {
                anm_init(i_this, 28, 6.0f, 2, 1.0f);
                i_this->mode = 4;
                i_this->path_no = 1;
                actor->speedF = 0.0f;
                i_this->timer[3] = l_HIO.lantern_swing_interval * 30.0f;
            }
            break;

        case 4:
            if (i_this->path_no >= i_this->field_0xc18->m_num) {
                anm_init(i_this, 7, 5.0f, 2, 1.0f);
                i_this->mode = 11;
                actor->speedF = 0.0f;
            } else {
                if (i_this->dis > (fVar1 + 150.0f)) {
                    if ((call_pt & 1) != 0) {
                        anm_init(i_this, 8, 5.0f, 2, 1.0f);
                    } else {
                        anm_init(i_this, 7, 5.0f, 2, 1.0f);
                    }

                    call_pt++;
                    i_this->mode = 10;
                    actor->speedF = 0.0f;
                } else {
                    dPnt* pnt_p = i_this->field_0xc18->m_points;
                    pnt_p = &pnt_p[i_this->path_no];
                    i_this->guide_path.x = pnt_p->m_position.x;
                    i_this->guide_path.y = pnt_p->m_position.y;
                    i_this->guide_path.z = pnt_p->m_position.z;
                    i_this->mode = 5;
                    actor->speedF = l_HIO.guided_movement_speed * fVar2;
                    
                    if (i_this->res_id != 28) {
                        anm_init(i_this, 28, 6.0f, 2, 1.0f);
                    }
                }
            }
            break;

        case 5:
            mae.x = i_this->guide_path.x - actor->current.pos.x;
            mae.z = i_this->guide_path.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            
            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < 100.0f) {
                i_this->path_no++;
                i_this->mode = 4;
            }
            break;

        case 10:
            if (i_this->res_id == 7) {
                i_this->current_angle.y = i_this->target_angle;
            } else {
                i_this->current_angle.y = i_this->target_angle + 0x8000;
            }

            if (i_this->dis < fVar1) {
                i_this->mode = 4;
            }
            break;

        case 11:
            if (i_this->res_id == 7) {
                i_this->current_angle.y = i_this->target_angle;
            }

            mae.x = -30900.0f - player->current.pos.x;
            mae.z = -15756.0f - player->current.pos.z;
            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < 1300.0f || i_this->dis < fVar1) {
                i_this->demo_mode = 300;
                i_this->mode = 300;
            }
            break;

        case 300:
            i_this->current_angle.y = i_this->target_angle;
            break;

        case 301:
            mae.x = -31872.0f - actor->current.pos.x;
            mae.z = -15490.0f - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            break;

        case 302:
            actor->current.pos.y = actor->home.pos.y + 10000.0f;
            actor->speed.y = 0.0f;
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mode = 41;
            i_this->timer[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if ((i_this->res_id == 39 || (i_this->res_id == 43 && frame < 7) || (frame > 40 && frame < 48)) || frame > 65) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->timer[0] == 0 && i_this->res_id == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->res_id == 43 && i_this->model->isStop()) {
                i_this->mode = 40;
            }
            break;

        case 50:
            anm_init(i_this, 46, 3.0f, 0, 1.0f);
            i_this->mode = 51;
            break;

        case 51:
            if (i_this->model->isStop()) {
                i_this->mode = 40;
            }
            break;

        case 53:
            if (i_this->timer[0] == 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mode = 10;
                i_this->timer[3] = 0;
            }
            break;

        case 100:
            anm_init(i_this, 44, 3.0f, 2, 1.0f);
            i_this->mode = 101;
            i_this->timer[0] = 80;
            break;

        case 101:
            if (i_this->timer[0] == 0) {
                i_this->mode = 4;
                i_this->timer[3] = l_HIO.lantern_swing_interval * 30.0f;
            }
            break;
    }

    if (i_this->mode != 11 && i_this->mode > 3 && i_this->mode < 40 && (i_this->timer[3] == 0 || dComIfGs_shake_kandelaar_check() == 1)) {
        i_this->mode = 100;
        actor->speedF = 0.0f;
    }

    if (i_this->mode >= 40 && i_this->mode <= 52) {
        fopAc_ac_c* enemy_p = enemy_check(i_this, 800.0f);
        if (enemy_p == NULL) {
            i_this->mode = 53;
            i_this->timer[0] = 30;
        }
    } else {
        fopAc_ac_c* enemy_p = enemy_view_check(i_this, 400.0f);
        if (enemy_p != NULL) {
            i_this->mode = 50;
            actor->speedF = 0.0f;
            mae.x = enemy_p->current.pos.x - actor->current.pos.x;
            mae.z = enemy_p->current.pos.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z) + 0x8000;
        }
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 2, 0x1000);

    s16 sVar1 = actor->current.angle.y - i_this->current_angle.y;
    if (sVar1 > 0x1000 || sVar1 < -0x1000) {
        rv = 0;
    }

    return rv;
}

static path guide_path_fs[7] = {
    { 1, -38786.0f, 1140.0f, -23321.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 1, -38195.0f, 750.0f, -22650.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 0, -37953.0f, 694.0f, -22015.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 0, -37606.0f, 545.0f, -21748.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, -37158.0f, 306.0f, -21628.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { 0, -36548.0f, 348.0f, -21429.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
    { -1, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

static int npc_ks_fsdemo(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);
    cXyz mae, ato;
    int rv = 1;
    int frame = i_this->model->getFrame();
    int iVar1 = 0;
    s16 sVar1 = 0x1000;

    i_this->field_0x5fc = 0;
    i_this->field_0xaec = 1;

    switch (i_this->mode) {
        case 0:
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x220])) {
                i_this->mode = 40;
            } else {
                fpcM_Search(s_fs_sub, i_this);
                actor->current.pos.set(-39906.0f, 21962.0f, -24243.0f);
                rv = 0;

                if (player->current.pos.z < -21000.0f) {
                    i_this->mode = 1;
                    actor->current.pos.set(-39906.0f, 1962.0f, -24243.0f);
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                    i_this->path_no = 0;
                    actor->speedF = 0.0f;
                    i_this->demo_mode = 350;
                    mDoAud_seStart(Z2SE_KOSARU_V_PRISONED, 0, 0, 0);
                }
            }
            break;

        case 1:
            if (guide_path_fs[i_this->path_no].field_0x0 != 0) {
                i_this->guide_path.x = guide_path_fs[i_this->path_no].field_0x4;
                i_this->guide_path.y = guide_path_fs[i_this->path_no].field_0x8;
                i_this->guide_path.z = guide_path_fs[i_this->path_no].field_0xc;
                i_this->field_0x910 = actor->current.pos;
                i_this->field_0x91c = i_this->guide_path;
                i_this->mode = 2;
                anm_init(i_this, 32, 2.0f, 0, 1.0f);
                actor->speedF = 0.0f;
            } else {
                i_this->guide_path.x = guide_path_fs[i_this->path_no].field_0x4;
                i_this->guide_path.z = guide_path_fs[i_this->path_no].field_0xc;
                i_this->mode = 5;
                actor->speedF = l_HIO.demo_speed_2;

                if (i_this->res_id != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 2:
            rv = 2;
            if (i_this->res_id == 32 && i_this->model->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                actor->speedF = 40.0f + TREG_F(9);
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                i_this->field_0xbe0 = 1;
            }

            mae = i_this->guide_path - actor->current.pos;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);
            i_this->current_angle.x = -cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));

            if (mae.abs() < actor->speedF * 1.2f) {
                actor->current.pos = i_this->guide_path;

                if (i_this->path_no == 0) {
                    anm_init(i_this, 45, 3.0f, 0, 1.0f);
                    i_this->mode = 3;
                    i_this->current_angle.y += 0x8000;
                    actor->speedF = 0.0f;
                } else {
                    i_this->mode = 1;
                }

                i_this->field_0xbe0 = 1;
                i_this->path_no++;
            }
            break;

        case 3:
            if (i_this->model->isStop()) {
                i_this->current_angle.y += 0x8000;
                i_this->mode = 1;
            }
            break;

        case 5:
            mae.x = i_this->guide_path.x - actor->current.pos.x;
            mae.z = i_this->guide_path.z - actor->current.pos.z;
            i_this->current_angle.y = cM_atan2s(mae.x, mae.z);

            if (JMAFastSqrt(mae.x * mae.x + mae.z * mae.z) < actor->speedF * 1.2f) {
                i_this->path_no++;
                i_this->mode = 1;

                if (guide_path_fs[i_this->path_no].field_0x0 < 0) {
                    i_this->mode = 10;
                    actor->speedF = 0.0f;
                    anm_init(i_this, 51, 5.0f, 2, 1.0f);
                }
            }
            break;

        case 10:
            actor->speedF = 0.0f;
            break;

        case 20:
            i_this->mode = 21;
            anm_init(i_this, 51, 10.0f, 2, 1.0f);
            // fallthrough
        case 21: {
            i_this->current_angle.y = i_this->target_angle;
            sVar1 = 0;
            i_this->field_0x5fc = 1;

            if (i_this->timer[2] != 0) {
                i_this->search_time = 10;
                i_this->find_pos.set(-37799.0f, 815.0f, -22323.0f);
                i_this->current_angle.y -= (s16) 0x3000;
                sVar1 = 0x800;
            }

            iVar1 = 1;
            s16 range = actor->current.angle.y - i_this->target_angle;
            if ((range > 0x3000 || range < -0x3000) && i_this->mode < 22) {
                anm_init(i_this, 28, 3.0f, 0, 1.0f);
                i_this->mode = 22;
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_WALK, -1);
            }

            break;
        }

        case 22:
            sVar1 = 0x800;
            i_this->current_angle.y = i_this->target_angle;

            if (frame >= 11) {
                i_this->mode = 20;
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mode = 41;
            i_this->timer[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if ((i_this->res_id == 39 || (i_this->res_id == 43 && frame < 7) || (frame > 40 && frame < 48)) || frame > 65) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->timer[0] == 0 && i_this->res_id == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->res_id == 43) {
                if (i_this->demo_mode >= 360 && i_this->model->checkFrame(57.0f)) {
                    i_this->mode = 20;
                } else if (i_this->model->isStop()) {
                    i_this->mode = 40;
                }
            }
            break;

        case 42:
            if (i_this->timer[1] == 1) {
                i_this->demo_mode = 360;
            }
    }

    if ((i_this->mode == 40 || i_this->mode == 41) && fopAcM_SearchByName(PROC_E_FS) == NULL && i_this->demo_mode == 0) {
        i_this->mode = 42;
        i_this->timer[1] = 30;
    }

    if (actor->speedF > 25.0f) {
        i_this->field_0x90c = 1;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->current_angle.y, 2, sVar1);

    if (iVar1 != 0) {
        fopAcM_OnStatus(actor, 0);
        cLib_onBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
        actor->eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (dComIfGp_event_runCheck()) {
            if (actor->eventInfo.checkCommandTalk()) {
                if (i_this->field_0xaee == 0) {
                    i_this->msg_flow.init(actor, 0x74, 0, NULL);
                    i_this->field_0xaee = 1;
                }

                if (i_this->msg_flow.doFlow(actor, NULL, 0)) {
                    dComIfGp_event_reset();
                    i_this->field_0xaee = 0;
                }
            }
        } else {
            i_this->field_0xaee = 0;
        }
    } else {
        fopAcM_OffStatus(actor, 0);
        cLib_offBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
        i_this->field_0xaee = 0;
    }

    return rv;
}

static void npc_ks_kago(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    i_this->field_0x5fc = 0;

    switch (i_this->mode) {
        case 0:
            anm_init(i_this, 34, 3.0f, 2, 1.0f);
            i_this->mode = 1;
            // fallthrough
        case 1:
            break;
    }

    cLib_addCalcAngleS2(&actor->current.angle.y, i_this->target_angle, 2, 0x2000);
    i_this->field_0xaec = 0;

        /* dSv_event_flag_c::F_0577 - Ordon Village - 2nd Day - Retrieved basket from monkey (hit hawk) */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x241])) {
        fopAcM_delete(actor);
    }
}

static void anm_se_set(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    if (i_this->res_id == 17) {
        if (i_this->model->checkFrame(1.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_WAIT, -1);
        }

        if (i_this->model->checkFrame(5.0f) || i_this->model->checkFrame(16.0f)) {
            i_this->sound.startCreatureSound(Z2SE_KOSARU_HAND_CLAP, 0, -1);
        }
    } else if (i_this->res_id == 55) {
        if (i_this->model->checkFrame(9.0f)) {
            i_this->sound.startCreatureSound(Z2SE_KOSARU_HAND_CLAP, 0, -1);
        }
    } else if (i_this->res_id == 11) {
        if (i_this->model->checkFrame(10.0f)) {
            i_this->sound.startCreatureSound(Z2SE_KOSARU_HAND_CLAP, 0, -1);
        }
    } else if (i_this->res_id == 56) {
        if (i_this->model->checkFrame(1.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_DELIGHT, -1);
        }

        if (i_this->model->checkFrame(24.0f) || i_this->model->checkFrame(11.0f)) {
            i_this->sound.startCreatureSound(Z2SE_KOSARU_HAND_CLAP, 0, -1);
        }
    } else if (i_this->res_id == 12) {
        if (i_this->model->checkFrame(3.0f) || i_this->model->checkFrame(10.0f)) {
            if (fopAcM_GetRoomNo(actor) == 22) {
                i_this->sound.startCreatureSound(Z2SE_KOSARU_FT_TSUTA, 0, -1);
            } else {
                i_this->sound.startCreatureSound(Z2SE_KOSARU_CLIMB, 0, -1);
            }
        }
    } else if (i_this->res_id == 15) {
        if (i_this->model->checkFrame(3.0f) || i_this->model->checkFrame(10.0f)) {
            i_this->sound.startCreatureSound(Z2SE_KOSARU_CLIMB, 0, -1);
        }
    } else if (i_this->res_id == 25) {
        if (i_this->model->checkFrame(19.0f) || i_this->model->checkFrame(36.0f) || i_this->model->checkFrame(54.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_POINT, -1);
        }
    } else if (i_this->res_id == 26) {
        if (i_this->model->checkFrame(1.0f) && i_this->field_0xc17 == 2) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_WALK, -1);
        }

        if (i_this->model->checkFrame(2.0f)) {
            if (i_this->action == 203 && i_this->mode <= 3) {
                i_this->sound.startCreatureSound(Z2SE_KOSARU_FT_BRIDGE, 0, -1);
            } else {
                i_this->sound.startCreatureSound(Z2SE_KOSARU_FOOTNOTE, 0, -1);

                if (i_this->field_0xc17 == 2) {
                    mDoAud_seStart(Z2SE_KOSARU_KANTERA_SWING, &i_this->field_0xc04, 0, 0);
                }
            }
        }
    } else if (i_this->res_id == 28) {
        if (i_this->model->checkFrame(1.0f) && i_this->field_0xc17 == 2) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_WALK, -1);
        }

        if (i_this->model->checkFrame(2.0f)) {
            i_this->sound.startCreatureSound(Z2SE_KOSARU_FOOTNOTE, 0, -1);
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_WALK, -1);

            if (i_this->field_0xc17 == 2) {
                mDoAud_seStart(Z2SE_KOSARU_KANTERA_SWING, &i_this->field_0xc04, 0, 0);
            }
        }
    } else if (i_this->res_id == 43) {
        if (i_this->model->checkFrame(5.0f) || i_this->model->checkFrame(47.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_SCARED, -1);
        }
    } else if (i_this->res_id == 39) {
        if (i_this->model->checkFrame(1.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_SCARED, -1);
        }
    } else if (i_this->res_id == 30) {
        if (i_this->model->checkFrame(4.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
            i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
        } else if (i_this->model->checkFrame(22.0f)) {
            i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
        }
    } else if (i_this->res_id == 5) {
        if (i_this->model->checkFrame(4.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
            i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
        } else if (i_this->model->checkFrame(22.0f)) {
            i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
        }
    } else if (i_this->res_id == 7) {
        if (i_this->model->checkFrame(8.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
            i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
        } else if (i_this->model->checkFrame(17.0f)) {
            i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);

            if (i_this->field_0xc17 == 2) {
                mDoAud_seStart(Z2SE_KOSARU_KANTERA_ADD, &i_this->field_0xc04, 0, 0);
            }
        }
    } else if (i_this->res_id == 8) {
        if (i_this->model->checkFrame(1.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_CALL, -1);
        }
    } else if (i_this->res_id == 44) {
        if (i_this->model->checkFrame(9.0f) || i_this->model->checkFrame(19.0f)) {
            mDoAud_seStart(Z2SE_KOSARU_KANTERA_SWING, &i_this->field_0xc04, 0, 0);
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_SWING_KANTERA, -1);
        }
    } else if (i_this->res_id == 35) {
        if (i_this->model->checkFrame(40.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_SLAPPED_HIP, -1);
        }
    } else if (i_this->res_id == 16) {
        if (i_this->model->checkFrame(1.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_CALL, -1);
        }
    } else if (i_this->res_id == 46) {
        if (i_this->model->checkFrame(4.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP_SURPRISE, -1);
        } else if (i_this->model->checkFrame(18.0f)) {
            i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
        } else if ((i_this->model->checkFrame(25.0f) || i_this->model->checkFrame(38.0f)) || i_this->model->checkFrame(51.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_POINT, -1);
            mDoAud_seStart(Z2SE_KOSARU_KANTERA_SWING, &i_this->field_0xc04, 0, 0);
        }
    } else if (i_this->res_id == 34) {
        if (i_this->model->checkFrame(1.0f)) {
            f32 fVar1 = i_this->dis * 0.0215f;
            if (fVar1 > 127.0f) {
                fVar1 = 127.0f;
            }

            i_this->sound.startCreatureSound(Z2SE_KOSARU_V_KAGO_WAIT, fVar1, -1);
        } else if (i_this->model->checkFrame(16.0f)) {
            i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
        }
    } else if (i_this->res_id == 45) {
        if (i_this->model->checkFrame(4.0f)) {
            i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP_SURPRISE, -1);
        } else if (i_this->model->checkFrame(16.0f)) {
            i_this->sound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
        }
    }
}

static void action(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz mae, ato;

    fopAcM_OffStatus(actor, 0);
    cLib_offBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_TALKCHECK_e | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);

    if (i_this->field_0x5b5 != 0) {
        mae = player->eyePos - i_this->field_0x614;
        i_this->dis = mae.abs();
        cLib_addCalcAngleS2(&actor->current.angle.x, -0x8000, 1, 0x1000);
    } else {
        i_this->dis = fopAcM_searchPlayerDistanceXZ(actor);
    }

    i_this->target_angle = fopAcM_searchPlayerAngleY(actor);
    i_this->field_0x5fc = 0;
    i_this->field_0xaed = 0;
    i_this->field_0x5b5 = 0;

    int iVar1 = 1;
    int iVar2 = 1;
    int int_0x2c = 1;
    int iVar3 = 0;

    actor->gravity = -5.0f;

    switch (i_this->action) {
        case 4:
            actor->gravity = 0.0f;
            npc_ks_pole_ori(i_this);
            i_this->field_0x5fc = 1;
            iVar1 = 0;
            break;

        case 5:
            i_this->field_0x5fc = 1;
            iVar2 = npc_ks_ori(i_this);
            if (iVar2 == 0) {
                iVar1 = 0;
            }
            break;

        case 6:
            actor->gravity = 0.0f;
            i_this->field_0x5fc = 1;
            iVar2 = npc_ks_ori2(i_this);
            break;

        case 8:
            npc_ks_home(i_this);
            i_this->field_0x5fc = 1;
            break;

        case 10:
            actor->gravity = 0.0f;
            npc_ks_hang(i_this);
            i_this->field_0x5fc = 2;
            i_this->field_0x5b5 = 1;
            iVar1 = 0;
            iVar2 = 0;
            break;

        case 11:
            actor->gravity = 0.0f;
            npc_ks_hang_s(i_this);
            i_this->field_0x5fc = 2;
            i_this->field_0x5b5 = 1;
            iVar1 = 0;
            iVar2 = 0;
            break;

        case 20:
            actor->gravity = 0.0f;
            npc_ks_to_hang(i_this);
            iVar2 = 2;
            break;

        case 21:
            actor->gravity = 0.0f;
            npc_ks_e_hang(i_this);
            i_this->field_0x5fc = 3;
            i_this->field_0x5b5 = 1;
            iVar1 = 0;
            iVar2 = 0;
            break;

        case 22:
            iVar2 = npc_ks_e_jump(i_this);
            break;

        case 100:
            actor->gravity = -5.0f;
            iVar3 = 1;
            if (checkDoorDemo()) {
                if (i_this->mode >= 7) {
                    i_this->mode = 0;
                }

                cLib_addCalc2(&actor->current.pos.y, player->current.pos.y, 1.0f, 100.0f);

                if (i_this->dis > 400.0f) {
                    cMtx_YrotS(*calc_mtx, i_this->target_angle);
                    mae.x = 0.0f;
                    mae.y = 0.0f;
                    mae.z = -300.0f;
                    MtxPosition(&mae, &ato);
                    actor->current.pos.x = player->current.pos.x + ato.x;
                    actor->current.pos.z = player->current.pos.z + ato.z;
                }

                iVar1 = 0;
                actor->speed.y = 0.0f;
                int room_no = fopAcM_GetRoomNo(actor);
#if VERSION == VERSION_SHIELD_DEBUG
                int stay_no = dStage_roomControl_c::getNextStayNo();
#else
                // FIXME: fakematch. dbg asm implies that return value of getNextStayNo() must be int.
                // However, changing its return from s8 to int causes regalloc issue here.
                // Thus, use mNextStayNo directly here.
                int stay_no = dStage_roomControl_c::mNextStayNo;
#endif
                if (room_no != stay_no) {
                    if (   (room_no == 0 && stay_no == 3)
                        || (room_no == 3 && stay_no == 4)
                        || (dComIfGs_isSwitch(0x52, fopAcM_GetRoomNo(actor)) && room_no == 4 && stay_no == 0)
                        || ((i_this->set_id == 2 || i_this->set_id == 3) && room_no == 0 && stay_no == 15)
                        || (room_no == 1 && stay_no == 4)
                        || (room_no == 0 && stay_no == 1 && i_this->set_id >= 2)
                        || (room_no == 9 && stay_no != 12)
                    ) {
                        i_this->action = 101;
                        i_this->mode = 0;
                        break;
                    }
                }
            } else {
                i_this->field_0xaec = 1;
            }

            iVar2 = npc_ks_option(i_this);
            fopAcM_SetRoomNo(actor, fopAcM_GetRoomNo(player));
            actor->tevStr.room_no = fopAcM_GetRoomNo(actor);
            break;

        case 101:
            actor->gravity = -5.0f;
            npc_ks_awaydoor(i_this);
            break;

        case 110:
            actor->gravity = -5.0f;
            iVar2 = npc_ks_guide_00(i_this);
            iVar3 = 1;
            break;

        case 111:
            actor->gravity = -5.0f;
            iVar2 = npc_ks_guide_00_2(i_this);
            iVar3 = 1;
            break;

        case 112:
            actor->gravity = -5.0f;
            iVar2 = npc_ks_guide_00_3(i_this);
            iVar3 = 1;
            break;

        case 113:
            actor->gravity = -5.0f;
            iVar2 = npc_ks_guide_01(i_this);
            iVar3 = 1;
            break;

        case 114:
            actor->gravity = -5.0f;
            iVar2 = npc_ks_guide_02(i_this);
            iVar3 = 1;
            break;

        case 115:
            actor->gravity = -5.0f;
            iVar2 = npc_ks_guide_09(i_this);
            iVar3 = 1;
            break;

        case 116:
            actor->gravity = -5.0f;
            iVar2 = npc_ks_guide_22(i_this);
            iVar3 = 1;
            break;

        case 117:
            actor->gravity = -5.0f;
            iVar2 = npc_ks_guide_0409(i_this);
            iVar3 = 1;
            break;

        case 200:
            iVar1 = npc_ks_demo_02(i_this);
            if (iVar1 == 0) {
                iVar2 = 0;
            }
            break;

        case 201:
            iVar1 = npc_ks_demo_022(i_this);
            if (iVar1 == 0) {
                iVar2 = 0;
            }
            break;

        case 203:
            npc_ks_demo_04(i_this);
            break;

        case 204:
            iVar2 = npc_ks_demo_12(i_this);
            break;

        case 300:
            iVar2 = npc_ks_mori(i_this);
            break;

        case 301:
            iVar2 = npc_ks_fsdemo(i_this);
            break;

        case 302:
            npc_ks_kago(i_this);
            break;
    }

    i_this->field_0xbdd = 0;

    if (iVar3 != 0) {
        if (water_check(i_this, actor->current.pos, 60.0f + TREG_F(19))) {
            if (actor->current.pos.y <= i_this->waterY) {
                i_this->field_0xbdd = 1;
                if (i_this->action != 100) {
                    i_this->action = 100;
                    i_this->mode = 0;
                }
            }
        }
    }

    if (int_0x2c && i_this->dis < 500.0f + KREG_F(15) && NPC_KS_FABSF(actor->current.pos.y - player->current.pos.y) < 1000.0f) {
        daPy_py_c::setLookPos(&actor->eyePos);
    }

    if (i_this->field_0xaed == 1 && i_this->dis < 300.0f) {
        i_this->action = 1000;
        i_this->mode = 0;
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&actor->shape_angle.x, actor->current.angle.x, 2, 0x2000);
    cLib_addCalcAngleS2(&actor->shape_angle.z, actor->current.angle.z, 2, 0x2000);

    if (iVar2 == 1) {
        f32 fVar1 = 1.0f;

        if (i_this->res_id == 47 && fopAcM_CheckCondition(actor, 4) != 0) {
            fVar1 = 10.0f + KREG_F(13);
        }

        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = actor->speedF * fVar1;
        MtxPosition(&mae, &ato);

        actor->speed.x = ato.x;
        actor->speed.z = ato.z;
        actor->current.pos += actor->speed;
        actor->speed.y += actor->gravity;

        if (actor->speed.y < -90.0f) {
            actor->speed.y = -90.0f;
        }
    } else if (iVar2 == 2) {
        cMtx_YrotS(*calc_mtx, i_this->current_angle.y);
        cMtx_XrotM(*calc_mtx, i_this->current_angle.x);
        mae.x = 0.0f;
        mae.y = 0.0f;
        mae.z = actor->speedF;
        MtxPosition(&mae, &actor->speed);
        actor->current.pos += actor->speed;
        iVar1 = 0;
    }

    if (i_this->field_0xaec == 1) {
        cXyz* move_p = i_this->stts.GetCCMoveP();
        if (move_p != NULL) {
            actor->current.pos.x += move_p->x * 0.5f;
            actor->current.pos.y += move_p->y * 0.5f;
            actor->current.pos.z += move_p->z * 0.5f;
        }
    }

    if (iVar1 != 0) {
        if (actor->shape_angle.x < -1000) {
            f32 fVar2 = (30.0f + ZREG_F(3)) * cM_ssin(actor->shape_angle.x);
            actor->current.pos.y += fVar2;
            actor->old.pos.y += fVar2;

            i_this->ObjAcch.CrrPos(dComIfG_Bgsp());

            actor->current.pos.y -= fVar2;
            actor->old.pos.y -= fVar2;
        } else {
            i_this->ObjAcch.CrrPos(dComIfG_Bgsp());
        }
    }

    s16 sVar1 = 0;
    s16 sVar2 = 0;

    if (i_this->field_0x5fc != 0) {
        if (i_this->search_time == 0) {
            if ((i_this->count & 7U) == 0 && fopAcM_otoCheck(actor, 2000.0f)) {
                SND_INFLUENCE* sound = dKy_Sound_get();
                i_this->find_pos = sound->position;
                i_this->search_time = cM_rndF(20.0f) + 30.0f;
            }

            fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);
            if (i_this->search_id != -1) {
                fopAc_ac_c* actor_p = fopAcM_SearchByID(i_this->search_id);
                if (actor_p != NULL) {
                    mae = actor_p->eyePos - actor->current.pos;
                } else {
                    mae = player->eyePos - actor->current.pos;
                }
            } else {
                mae = player->eyePos - actor->current.pos;
                mae.y += TREG_F(1) - 70.0f;
            }
        } else {
            i_this->search_time--;
            if (i_this->search_time < 40) {
                mae = i_this->find_pos - actor->current.pos;
                mae.y += 100.0f;
            } else {
                mae = player->eyePos - actor->current.pos;
                mae.y += TREG_F(1) -70.0f;
            }
        }

        sVar1 = cM_atan2s(mae.x, mae.z) - actor->shape_angle.y;
        sVar1 *= -1;
        sVar2 = actor->shape_angle.x + cM_atan2s(mae.y, JMAFastSqrt(mae.x * mae.x + mae.z * mae.z));

        if (sVar1 > 8000) {
            sVar1 = 8000;
        } else if (sVar1 < -8000) {
            sVar1 = -8000;
        }

        if (sVar2 > 16000) {
            sVar2 = 16000;
        } else if (sVar2 < -8000) {
            sVar2 = -8000;
        }

        if (i_this->field_0x5fc >= 2) {
            sVar1 /= 2;
        }
    }

    if (i_this->field_0x5fd) {
        sVar2 = 10000;
        i_this->field_0x5fd = 0;
        i_this->field_0x5fc = 1;
    }

    cLib_addCalcAngleS2(&i_this->field_0x5fe, sVar1, 4, 0x1000);
    cLib_addCalcAngleS2(&i_this->field_0x600, sVar2, 4, 0x1000);

    if (i_this->field_0x5e0 == 0) {
        i_this->field_0x5e0 = cM_rndF(60.0f) + 30.0f;
    } else {
        i_this->field_0x5e0--;
        if (i_this->field_0x5e0 <= 7) {
            i_this->field_0x5e2 = 7 - i_this->field_0x5e0;
        } else {
            i_this->field_0x5e2 = 0;
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0x606, i_this->field_0x608, 2, 5000);
    i_this->field_0x608 = 0;
    if (i_this->field_0xbe0 != 0) {
        fopAcM_effSmokeSet1(&i_this->field_0x858, &i_this->field_0x85c, &actor->current.pos, NULL,
                            0.75f + TREG_F(18), &actor->tevStr, 1);
        i_this->field_0xbe0 = 0;
    }
}

static void* s_kago_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_KAGO) {
        if (((daObj_Kago_c*)i_actor)->getType() == 0) {
            return i_actor;
        }
    }

    return NULL;
}

static void kantera_sub(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    MTXCopy(i_this->model->getModel()->getAnmMtx(14), mDoMtx_stack_c::get());
    i_this->stick_model->setBaseTRMtx(mDoMtx_stack_c::get());

    if (i_this->field_0xc17 == 2) {
        mDoMtx_stack_c::transM(44.0f + KREG_F(7), 144.5f + KREG_F(8), 11.0f + KREG_F(9));
        mDoMtx_multVecZero(mDoMtx_stack_c::get(), &i_this->field_0xc04);
        f32 model_size = l_HIO.lantern_size;
        mDoMtx_stack_c::scaleM(model_size, model_size, model_size);
        daPy_getPlayerActorClass()->setKandelaarMtx(mDoMtx_stack_c::get(), 1, 1);
    } else if (i_this->field_0xc17 == 3) {
        daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
        if (fopAcM_checkCarryNow(actor) != 0 && player->getGrabUpStart()) {
            fopAcM_cancelCarryNow(actor);
            cLib_offBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_CARRY_e);
            i_this->field_0xc17 = 0;
            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[0xE2]);
        } else {
            i_this->field_0xc04.x = -30715.0f + AREG_F(8);
            i_this->field_0xc04.z = -17200.0f + AREG_F(9);
            i_this->field_0xc04.y += i_this->field_0xc10;
            i_this->field_0xc10 -= 5.0f;

            if (i_this->field_0xc04.y <= 338.0f + AREG_F(10)) {
                i_this->field_0xc04.y = 338.0f + AREG_F(10);
                if (i_this->field_0xc16 == 0) {
                    i_this->field_0xc10 = 20.0f + NREG_F(7);
                    i_this->field_0xc16 = 1;
                    i_this->field_0xc14 = 12;
                } else {
                    i_this->field_0xc10 = 0.0f;
                }
            }

            mDoMtx_stack_c::transS(i_this->field_0xc04.x, i_this->field_0xc04.y, i_this->field_0xc04.z);
            s16 res = cM_ssin(i_this->field_0xc14 * 13000) * i_this->field_0xc14 * 500.0f + 14500.0f;
            if (i_this->field_0xc14 != 0) {
                i_this->field_0xc14--;
            }

            mDoMtx_stack_c::ZrotM(res);
            mDoMtx_stack_c::YrotM(5000);
            daPy_getPlayerActorClass()->setKandelaarMtx(mDoMtx_stack_c::get(), 0, 0);
            fopAcM_OnCarryType(actor, fopAcM_CARRY_ITEM);
            cLib_onBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_CARRY_e);
            actor->attention_info.distances[fopAc_attn_CARRY_e] = 6;
            actor->attention_info.position.x = i_this->field_0xc04.x;
            actor->attention_info.position.y = i_this->field_0xc04.y + 30.0f;
            actor->attention_info.position.z = i_this->field_0xc04.z;
            actor->eyePos = actor->attention_info.position;
        }
    }
}

static int c_start;

static int daNpc_Ks_Execute(npc_ks_class* i_this) {
    if (i_this->action == 300) {
        if (c_start == 0 && i_this->demo_mode == 0 && dComIfGp_event_runCheck()) {
            kantera_sub(i_this);
            return 1;
        }
    } else {
        if (cDmrNowMidnaTalk()) {
            return 1;
        }
    }

    fopAc_ac_c* actor = &i_this->actor;
    cXyz mae, ato;

    if (i_this->set_id == 2) {
        if (fopAcM_GetRoomNo(actor) == 0) {
            dComIfGs_onSwitch(0x4F, fopAcM_GetRoomNo(actor));
        }
    } else if (i_this->set_id == 3 && fopAcM_GetRoomNo(actor) == 0) {
        dComIfGs_onSwitch(0x50, fopAcM_GetRoomNo(actor));
    }

    i_this->count++;

    for (int i = 0; i < 4; i++) {
        if (i_this->timer[i] != 0) {
            i_this->timer[i]--;
        }
    }

    if (i_this->field_0x5f8 != 0) {
        i_this->field_0x5f8--;
    }

    if (i_this->anm_time != 0) {
        i_this->anm_time--;
    }

    if (i_this->hang_time != 0) {
        i_this->hang_time--;
    }

    action(i_this);

    if (i_this->field_0x90c != 0) {
        mae = i_this->field_0x91c - i_this->field_0x910;
        f32 fVar1 = mae.abs();
        mae = i_this->field_0x91c - actor->current.pos;
        f32 fVar2 = mae.abs();
        f32 fVar3 = fVar1 * 0.2f;
        if (fVar3 > 200.0f) {
            fVar3 = 200.0f;
        }
        i_this->field_0x908 = fVar3 * cM_ssin((fVar2 / fVar1) * 32768.0f);
        i_this->field_0x90c = 0;
    } else {
        cLib_addCalc0(&i_this->field_0x908, 1.0f, 5.0f);
    }

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y + i_this->field_0x908, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::XrotM(actor->shape_angle.x);
    mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, i_this->field_0x928, 0.0f);
    mDoMtx_stack_c::scaleM(l_HIO.basic_size, l_HIO.basic_size, l_HIO.basic_size);

    J3DModel* model = i_this->model->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->model->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    anm_se_set(i_this);

    if (i_this->field_0x5e4 != 0) {
        i_this->btp2->play();
    } else {
        i_this->btp->setFrame(i_this->field_0x5e2);
    }

    i_this->model->modelCalc();

    if (i_this->res_id == 34) {
        daObj_Kago_c* kago = (daObj_Kago_c*)fpcM_Search(s_kago_sub, i_this);
        if (kago != NULL) {
            MTXCopy(i_this->model->getModel()->getAnmMtx(14), mDoMtx_stack_c::get());
            mDoMtx_stack_c::YrotM(-2000);
            mDoMtx_stack_c::XrotM(-0x3638);
            mDoMtx_stack_c::ZrotM(-0x2E68);
            mDoMtx_stack_c::transM(-60.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::YrotM(0);
            mDoMtx_stack_c::XrotM(0);
            mDoMtx_stack_c::ZrotM(0);
            
            kago->setMtx(mDoMtx_stack_c::get());
            if (fopAcM_checkHawkCarryNow(kago) != 0) {
                anm_init(i_this, 39, 5.0f, 2, 1.0f);
                i_this->sound.startCreatureVoice(Z2SE_KOSARU_V_JUMP_SURPRISE, -1);
            }
        }
    }

    if (i_this->bara_model != NULL) {
        i_this->bara_model->setBaseTRMtx(i_this->model->getModel()->getAnmMtx(4));
    }

    kantera_sub(i_this);

    MTXCopy(model->getAnmMtx(4), *calc_mtx);
    mae.set(0.0f, 0.0f, 0.0f);
    if (i_this->field_0xc17 != 3) {
        MtxPosition(&mae, &actor->eyePos);
        actor->attention_info.position = actor->eyePos;
        actor->attention_info.position.y += 30.0f;
    }

    MTXCopy(model->getAnmMtx(10), *calc_mtx);
    mae.set(30.0f, 0.0f, 0.0f);
    MtxPosition(&mae, &i_this->field_0x614);

    MTXCopy(model->getAnmMtx(14), *calc_mtx);
    mae.set(30.0f, -0.0f, 0.0f);
    MtxPosition(&mae, &ato);
    i_this->field_0x614 += (ato - i_this->field_0x614) * 0.5f;

    if (i_this->field_0x620 != 0) {
        if (i_this->field_0x620 == 1) {
            cLib_addCalc2(&obj_pos.x, i_this->field_0x614.x, 1.0f, NPC_KS_FABSF(i_this->field_0x624.x) + 4.0f);
            cLib_addCalc2(&obj_pos.y, i_this->field_0x614.y, 1.0f, NPC_KS_FABSF(i_this->field_0x624.y) + 4.0f);
            cLib_addCalc2(&obj_pos.z, i_this->field_0x614.z, 1.0f, NPC_KS_FABSF(i_this->field_0x624.z) + 4.0f);
            mae.x = obj_pos.x - i_this->field_0x614.x;
            mae.z = obj_pos.z - i_this->field_0x614.z;
            if ((mae.x * mae.x + mae.z * mae.z) <= 15.0f) {
                i_this->field_0x620 = 2;
                i_this->field_0x621 = 10;
                leader->field_0x92c = 0;
                dComIfGp_getVibration().StartShock(2, 31, cXyz(0.0f, 1.0f, 0.0f));
            }
        } else if (i_this->field_0x620 == 2) {
            obj_pos = i_this->field_0x614;
        } else {
            obj_pos += i_this->field_0x624;
        }

        i_this->field_0x638 += i_this->field_0x634;
        i_this->field_0x634 += -3.0f;

        if (i_this->field_0x638 < 0.0f) {
            i_this->field_0x638 = 0.0f;
        }

        daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
        cXyz ato;
        ato.x = obj_pos.x;
        ato.z = obj_pos.z;
        ato.y = obj_pos.y + i_this->field_0x638;
        setPlayerPosAndAngle(&ato, actor->home.angle.y);

        if (i_this->field_0x620 == 2) {
            if (i_this->field_0x604 < 0 && i_this->field_0x602 < 0) {
                dComIfGp_event_offHindFlag(2);
                dComIfGp_setDoStatusForce(1, 2);
            }

            player->changeDemoParam0(1);
            cLib_addCalcAngleS2(&i_this->field_0x610, i_this->field_0x602 << 1, 1, 4000);
        } else {
            player->changeDemoParam0(0);
            cLib_addCalcAngleS2(&i_this->field_0x610, 10000, 4, 4000);
        }

        if (i_this->field_0x620 != 0) {
            player->changeDemoParam1(i_this->field_0x610);
        }
    }

    if (i_this->field_0x621 != 0) {
        i_this->field_0x621--;
    }

    ato = actor->current.pos;

    if (i_this->field_0xaec == 0) {
        i_this->cc_cyl.OffCoSetBit();
    } else {
        i_this->cc_cyl.OnCoSetBit();
    }

    i_this->cc_cyl.SetC(ato);
    i_this->cc_cyl.SetR(l_HIO.basic_size * 50.0f);
    i_this->cc_cyl.SetH(l_HIO.basic_size * 130.0f);
    dComIfG_Ccsp()->Set(&i_this->cc_cyl);

    if (i_this->set_id == 0) {
        demo_camera(leader);
    } else if (i_this->set_id >= 4) {
        demo_camera(i_this);
    }

    return 1;
}

static int daNpc_Ks_IsDelete(npc_ks_class* i_this) {
    return 1;
}

static int daNpc_Ks_Delete(npc_ks_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAcM_RegisterDeleteID(i_this, "Npc_Ks");
    dComIfG_resDelete(&i_this->mPhase, i_this->res_name);

    if (i_this->hio_init != 0) {
        hio_set = 0;
        mDoHIO_DELETE_CHILD(l_HIO.no);
    }

    if (actor->heap != NULL) {
        i_this->model->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    npc_ks_class* i_this = (npc_ks_class*)actor;

    i_this->model = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes(i_this->res_name, npc_ks_class::BMDR_SARU), NULL, NULL, 
                                               (J3DAnmTransform*)dComIfG_getObjectRes(i_this->res_name, npc_ks_class::BCK_SARU_WAIT_A), 0, 1.0f, 0, -1, 
                                               &i_this->sound, 0x80000, 0x11020084);
    if (i_this->model == NULL || i_this->model->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = i_this->model->getModel();
    model->setUserArea((uintptr_t)i_this);
    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    i_this->btp = new mDoExt_btpAnm();
    if (i_this->btp == NULL) {
        return 0;
    }
    if (i_this->btp->init(i_this->model->getModel()->getModelData(), 
                            (J3DAnmTexPattern*)dComIfG_getObjectRes(i_this->res_name, npc_ks_class::BTP_SARU), 1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }

    i_this->btp2 = new mDoExt_btpAnm();
    if (i_this->btp2 == NULL) {
        return 0;
    }
    if (i_this->btp2->init(i_this->model->getModel()->getModelData(), 
                            (J3DAnmTexPattern*)dComIfG_getObjectRes(i_this->res_name, npc_ks_class::BTP_SARU_RELIEF), 1, 2, 1.0f, 0, -1) == 0) {
        return 0;
    }

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->res_name, npc_ks_class::BMDR_SARU_STICK);
    JUT_ASSERT(10232, modelData != NULL);
    i_this->stick_model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->stick_model == NULL) {
        return 0;
    }

    if (i_this->monkey_room_no == 0 || i_this->monkey_room_no == 10 || i_this->monkey_room_no == 20 || i_this->monkey_room_no == 21 || i_this->monkey_room_no == 22) {
        modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->res_name, npc_ks_class::BMDR_SARU_BARA);
        JUT_ASSERT(10249, modelData != NULL);
        i_this->bara_model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->bara_model == NULL) {
            return 0;
        }
    }

    return 1;
}

static BOOL start_check(npc_ks_class* i_this) {
    static f32 dx[3] = {
        7268.0f, 5166.0f, 9233.0f,
    };

    static f32 dy[3] = {
        3600.0f, 3300.0f, 3300.0f,
    };

    static f32 dz[3] = {
        -3389.0f, -5327.0f, -5323.0f,
    };

    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    BOOL rv = TRUE;
    f32 fVar1;
    f32 fVar2;

    if (i_this->monkey_room_no <= 3 && fopAcM_GetRoomNo(actor) == 4) {
        if (dComIfGs_isStageMiddleBoss()) {
            return FALSE;
        }

        fVar1 = -player->current.pos.x;
        fVar2 = 4620.0f - player->current.pos.z;
        if (JMAFastSqrt(fVar1 * fVar1 + fVar2 * fVar2) > 1000.0f) {
            return FALSE;
        }

        if (cDmr_SkipInfo != 0) {
            cDmr_SkipInfo--;
            actor->current.angle.y = actor->home.angle.y + 0x4000;
            actor->shape_angle.y = actor->current.angle.y;
            actor->current.angle.x = -0x8000;
            actor->shape_angle.x = -0x8000;
            i_this->field_0x630 = i_this->monkey_room_no * 6 + 6;
            i_this->action = 10;
            i_this->hang_time = 30;
            return rv;
        }
    }

    int bitSw = (fopAcM_GetParam(actor) & 0xFF000000) >> 24;
    int iVar2 = 0;

    switch (i_this->monkey_room_no) {
        case 0:
            if (fopAcM_GetRoomNo(actor) == 22) {
                if (bitSw != 0xFF) {
                    if (!dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                        i_this->action = 6;
                        break;
                    }
                }
            } else if (fopAcM_GetRoomNo(actor) == 12) {
                i_this->action = 8;
                break;
            }

            if (dComIfGs_isStageMiddleBoss()) {
                if (fopAcM_GetRoomNo(actor) == 0) {
                    actor->current.angle.y = actor->home.angle.y + 0x4000;
                    actor->shape_angle.y = actor->current.angle.y;
                    actor->current.angle.x = -0x8000;
                    actor->shape_angle.x = -0x8000;
                    i_this->field_0x630 = 3;
                    i_this->action = 11;
                    return rv;
                }

                if (fopAcM_GetRoomNo(actor) == 2) {
                    actor->current.angle.y = actor->home.angle.y + 0x4000;
                    actor->shape_angle.y = actor->current.angle.y;
                    actor->current.angle.x = -0x8000;
                    actor->shape_angle.x = -0x8000;
                    i_this->action = 10;
                    i_this->hang_time = 30;
                    return rv;
                }

                rv = FALSE;
            } else {
                if (fopAcM_GetRoomNo(actor) == 0) {
                    if (!dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                        rv = FALSE;
                    }
                }
            }

            i_this->action = 100;
            break;

        case 1:
            if (fopAcM_GetRoomNo(actor) == 12) {
                i_this->action = 8;
            } else {
                if (dComIfGs_isStageMiddleBoss()) {
                    if (fopAcM_GetRoomNo(actor) == 0) {
                        actor->current.angle.y = actor->home.angle.y + 0x4000;
                        actor->shape_angle.y = actor->current.angle.y;
                        actor->current.angle.x = -0x8000;
                        actor->shape_angle.x = -0x8000;
                        i_this->field_0x630 = 3;
                        i_this->action = 11;
                        return rv;
                    }

                    if (fopAcM_GetRoomNo(actor) == 2) {
                        actor->current.angle.y = actor->home.angle.y + 0x4000;
                        actor->shape_angle.y = actor->current.angle.y;
                        actor->current.angle.x = -0x8000;
                        actor->shape_angle.x = -0x8000;
                        i_this->action = 10;
                        i_this->hang_time = 30;
                        return rv;
                    }

                    rv = FALSE;
                } else {
                    if (fopAcM_GetRoomNo(actor) == 2) {
                        if (dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                            i_this->action = 100;
                        } else {
                            i_this->action = 4;
                        }
                    } else {
                        if (!dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                            rv = FALSE;
                        } else {
                            i_this->action = 100;
                        }
                    }
                }
            }
            break;

        case 2:
            if (fopAcM_GetRoomNo(actor) == 12) {
                i_this->action = 8;
            } else {
                if (dComIfGs_isStageMiddleBoss()) {
                    rv = FALSE;
                }

                if (fopAcM_GetRoomNo(actor) == 7) {
                    if (dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                        rv = FALSE;
                    } else {
                        i_this->action = 5;
                    }
                } else {
                    if (!dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                        rv = FALSE;
                    } else {
                        if (fopAcM_GetRoomNo(actor) == 0) {
                            i_this->action = 8;
                        } else {
                            if (fopAcM_GetRoomNo(actor) == 3 || fopAcM_GetRoomNo(actor) == 5 || fopAcM_GetRoomNo(actor) == 7) {
                                if (dComIfGs_isSwitch(79, fopAcM_GetRoomNo(actor))) {
                                    rv = FALSE;
                                    break;
                                }
                            }

                            i_this->action = 100;
                        }
                    }
                }
            }
            break;

        case 3:
            if (fopAcM_GetRoomNo(actor) == 12) {
                i_this->action = 8;
            } else {
                if (dComIfGs_isStageMiddleBoss()) {
                    rv = FALSE;
                }

                if (fopAcM_GetRoomNo(actor) == 5) {
                    if (dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                        rv = FALSE;
                    } else {
                        i_this->action = 5;
                    }
                } else {
                    if (!dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                        rv = FALSE;
                    } else {
                        if (fopAcM_GetRoomNo(actor) == 0) {
                            i_this->action = 8;
                        } else {
                            if (fopAcM_GetRoomNo(actor) == 3 || fopAcM_GetRoomNo(actor) == 5 || fopAcM_GetRoomNo(actor) == 7) {
                                if (dComIfGs_isSwitch(80, fopAcM_GetRoomNo(actor))) {
                                    rv = FALSE;
                                    break;
                                }
                            }

                            i_this->action = 100;
                        }
                    }
                }
            }
            break;

        case 4:
            if (fopAcM_GetRoomNo(actor) == 12) {
                if (!dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                    rv = FALSE;
                } else {
                    i_this->action = 8;
                }
            } else {
                if (dComIfGs_isSwitch(83, fopAcM_GetRoomNo(actor))) {
                    rv = FALSE;
                } else {
                    if (fopAcM_GetRoomNo(actor) == 4) {
                        if (dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                            i_this->action = 117;
                            i_this->mode = 0;
                            actor->current.pos.set(6854.0f, 3458.0f, 440.0f);
                            actor->current.angle.y = 0;
                            actor->old = actor->current;
                        } else {
                            i_this->action = 6;
                        }
                    } else {
                        if (fopAcM_GetRoomNo(actor) == 9) {
                            if (!dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                                return FALSE;
                            }

                            i_this->action = 100;

                            for (int i = 0; i < 3; i++) {
                                f32 pos_x = dx[i] - player->current.pos.x;
                                f32 pos_y = dz[i] - player->current.pos.z;
                                if (JMAFastSqrt(pos_x * pos_x + pos_y * pos_y) < 700.0f) {
                                    actor->current.pos.set(dx[i], dy[i], dz[i]);
                                    actor->old = actor->current;
                                    OS_REPORT("////////KS R09 SET %d\n", i);
                                    return rv;
                                }
                            }
                        }
                    }
                }
            }
            break;

        case 5:
            if (fopAcM_GetRoomNo(actor) == 12) {
                if (!dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                    rv = FALSE;
                } else {
                    i_this->action = 8;
                }
            } else {
                if (fopAcM_GetRoomNo(actor) == 10) {
                    if (dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                        rv = FALSE;
                    } else {
                        i_this->action = 5;
                    }
                }
            }
            break;

        case 6:
            if (fopAcM_GetRoomNo(actor) == 12) {
                if (!dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                    rv = FALSE;
                } else {
                    i_this->action = 8;
                }
            } else {
                if (fopAcM_GetRoomNo(actor) == 19) {
                    if (dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                        rv = FALSE;
                    } else {
                        i_this->action = 6;
                    }
                }
            }
            break;

        case 7:
            if (fopAcM_GetRoomNo(actor) == 12) {
                if (!dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                    rv = FALSE;
                } else {
                    i_this->action = 8;
                }
            } else {
                if (fopAcM_GetRoomNo(actor) == 11) {
                    if (dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
                        rv = FALSE;
                    } else {
                        i_this->action = 5;
                    }
                }
            }
            break;

        case 10:
            i_this->action = 100;
            break;

        case 20:
            fopAcM_OnStatus(actor, 0x20000);
            i_this->action = 300;
            i_this->field_0xc17 = 1;
            iVar2 = 1;
            break;

        case 21:
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x20A])) {
                rv = FALSE;
            } else {
                i_this->action = 301;
            }
            break;

        case 22:
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x241])) {
                rv = FALSE;
            } else {
                i_this->action = 302;
            }
            break;

        default:
            i_this->action = 100;
            i_this->monkey_room_no &= (u8) 0xF;
            i_this->set_id = i_this->monkey_room_no;
            break;
    }

    if (i_this->action == 100) {
        fopAcM_setStageLayer(actor);
        if (fopAcM_GetRoomNo(actor) == 1) {
            f32 x_pos = 6836.0f - player->current.pos.x;
            f32 y_pos = 3585.0f - player->current.pos.z;
            if (JMAFastSqrt(x_pos * x_pos + y_pos * y_pos) < 500.0f) {
                actor->home.pos.set(7025.0f, 3355.0f, 4500.0f);
                actor->home.angle.y = -0x7343;
                actor->old = actor->current = actor->home;
            }

            i_this->timer[1] = cM_rndF(20.0f) + 80.0f;
        } else {
            i_this->timer[1] = cM_rndF(20.0f) + 40.0f;
        }
    }

    if (iVar2 != 0) {
        if (dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(actor))) {
            rv = FALSE;
        }
    }

    return rv;
}

static void* s_check_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KS) {
        if (fopAcM_GetID(i_actor) != fopAcM_GetID(i_data) && ((npc_ks_class*)i_actor)->set_id == ((npc_ks_class*)i_data)->set_id) {
            return i_actor;
        }
    }

    return NULL;
}

static int daNpc_Ks_Create(fopAc_ac_c* actor) {
    static dCcD_SrcCyl cc_cyl_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xd8fafd3f, 0x3}, 0x79}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {
                {0.0f, 0.0f, 0.0f}, // mCenter
                30.0f, // mRadius
                20.0f // mHeight
            } // mCyl
        }
    };

    npc_ks_class* i_this = (npc_ks_class*)actor;
    fopAcM_ct(&i_this->actor, npc_ks_class);

    stage_stag_info_class* stag_info = dComIfGp_getStage()->getStagInfo(); // unused
    if (dKy_darkworld_check()) {
        i_this->res_name = "Npc_kst";
    } else {
        i_this->res_name = "Npc_ks";
    }

    int phase = dComIfG_resLoad(&i_this->mPhase, i_this->res_name);
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("NPC_KS PARAM %x\n", fopAcM_GetParam(actor));
        i_this->monkey_room_no = fopAcM_GetParam(actor);

        u8 arg0 = (fopAcM_GetParam(actor) & 0xFF00) >> 8;

        if (i_this->monkey_room_no == 0xFF) {
            i_this->monkey_room_no = 0;
        }

        if (i_this->monkey_room_no == 20
            /* dSv_event_flag_c::F_0225 - Faron Woods - Lanter guide monkey doesn't come out a second time */
            && dComIfGs_isEventBit(u16(dSv_event_flag_c::saveBitLabels[0xE1]))
            /* dSv_event_flag_c::F_0226 - Faron Woods - Get lantern back from monkey */
            && dComIfGs_isEventBit(u16(dSv_event_flag_c::saveBitLabels[0xE2]))) {
            return cPhs_ERROR_e;
        }

        if (arg0 == 0xFF) {
            arg0 = 0;
        }

        if (fopAcM_GetRoomNo(actor) == 4 && i_this->monkey_room_no == 4) {
            actor->current.pos.set(-5370.0f, 3950.0f, -2330.0f);
        }

        if (strcmp(dComIfGp_getStartStageName(), "D_MN05") == 0 && dComIfGs_isStageBossEnemy() != 0) {
            return cPhs_ERROR_e;
        }

        i_this->set_id = i_this->monkey_room_no & 15;
        if (fpcM_Search(s_check_sub, i_this) != NULL) {
            return cPhs_ERROR_e;
        }
            
        OS_REPORT("////////SET ID  %d\n", i_this->set_id);

        if (i_this->set_id == 0) {
            leader = i_this;
            OS_REPORT("////////0 \n");
        } else {
            OS_REPORT("////////1 \n");
        }

        i_this->field_0xaf0 = actor->current.angle.x;
        actor->current.angle.x = actor->shape_angle.x = 0;
        i_this->bitTRB = actor->current.angle.z & 63;
        actor->current.angle.z = actor->shape_angle.z = 0;
        OS_REPORT("NPC_KS bitTRB %x\n", i_this->bitTRB);
        obj_pos.set(0.0f, 0.0f, 0.0f);

        OS_REPORT("NPC_KS//////////////NPC_KS SET 1 !!\n");
        if (!fopAcM_entrySolidHeap(actor, useHeapInit, 0x38A0)) {
            OS_REPORT("//////////////NPC_KS SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////NPC_KS SET 2 !!\n");
        if (hio_set == 0) {
            i_this->hio_init = 1;
            hio_set = 1;
            l_HIO.no = mDoHIO_CREATE_CHILD("コザル", &l_HIO); // Kozaru
        }

        fopAcM_SetMtx(actor, i_this->model->getModel()->getBaseTRMtx());
        fopAcM_SetMin(actor, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(actor, 200.0f, 200.0f, 200.0f);
        i_this->ObjAcch.Set(fopAcM_GetPosition_p(actor), fopAcM_GetOldPosition_p(actor), actor, 1,
                                &i_this->AcchCir, fopAcM_GetSpeed_p(actor), NULL, NULL);
        i_this->AcchCir.SetWall(50.0f, 50.0f);
        i_this->count = cM_rndF(65536.0f);
        saru_p[i_this->set_id] = i_this;

        if (arg0 != 0 && i_this->monkey_room_no < 8) {
            actor->current.angle.y = actor->home.angle.y + 0x4000;
            actor->shape_angle.y = actor->current.angle.y;
            actor->current.angle.x = -0x8000;
            actor->shape_angle.x = -0x8000;
            i_this->field_0x630 = (fopAcM_GetParam(actor) & 0xFF0000) >> 16;

            i_this->child_no = (obj_sw_class*)fopAcM_SearchByID(actor->parentActorID);
            if (i_this->child_no != NULL && i_this->child_no->field_0x91c == 1) {
                i_this->action = 11;
            } else {
                i_this->action = 10;
            }

            i_this->hang_time = 30;
        } else {
            if (!start_check(i_this)) {
                saru_p[i_this->set_id] = NULL;
                return cPhs_ERROR_e;
            }
        }

        i_this->sound.init(&actor->current.pos, &actor->eyePos, 2, 1);
        i_this->stts.Init(100, 0, actor);
        i_this->cc_cyl.Set(cc_cyl_src);
        i_this->cc_cyl.SetStts(&i_this->stts);
        i_this->cc_cyl.OnTgNoHitMark();

        OS_REPORT("//////////////NPC_KS SET 3 !!\n");
        c_start = 1;
        daNpc_Ks_Execute(i_this);
        c_start = 0;
        OS_REPORT("//////////////NPC_KS SET 4 !!\n");
    }

    return phase;
}

AUDIO_INSTANCES;

static actor_method_class l_daNpc_Ks_Method = {
    (process_method_func)daNpc_Ks_Create,
    (process_method_func)daNpc_Ks_Delete,
    (process_method_func)daNpc_Ks_Execute,
    (process_method_func)daNpc_Ks_IsDelete,
    (process_method_func)daNpc_Ks_Draw,
};

actor_process_profile_definition g_profile_NPC_KS = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_KS,            // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(npc_ks_class),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  701,                    // mPriority
  &l_daNpc_Ks_Method,     // sub_method
  0x00044100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
