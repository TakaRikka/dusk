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

    /* 0x04 */ s8 field_0x4;
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
    field_0x4 = -1;
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

static BOOL otherBgCheck(fopAc_ac_c* p_actor, fopAc_ac_c* p_actor2) {
    dBgS_LinChk p_line;
    cXyz unused_cxyz, pi_start, pi_end;
    pi_end = p_actor2->current.pos;
    pi_end.y += 100.0f + KREG_F(18);

    pi_start = p_actor->current.pos;
    pi_start.y += 120.0f + KREG_F(19);

    p_line.Set(&pi_start, &pi_end, p_actor);

    if (dComIfG_Bgsp().LineCross(&p_line) != 0) {
        return TRUE;
    }

    return FALSE;
}

static void anm_init(npc_ks_class* i_this, int param_2, f32 i_morf, u8 i_attr, f32 i_rate) {
    if (i_this->field_0x94c != 0 && (param_2 == 51 || param_2 == 53)) {
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

    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, param_2), i_attr, i_morf, i_rate, 0.0f, -1.0f);
    i_this->field_0x5d0 = i_index;
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DJoint* my_joint = i_joint;
        int jointNo = my_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        npc_ks_class* npc_ks_p = (npc_ks_class*)model->getUserArea();
        fopEn_enemy_c* base_ks_p = &npc_ks_p->actor;

        if (npc_ks_p != NULL) {
            if (npc_ks_p->field_0x5fc == 1 && jointNo == 3) {
                MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
                cMtx_YrotM(*calc_mtx, -npc_ks_p->field_0x5fe);
                cMtx_XrotM(*calc_mtx, -(npc_ks_p->field_0x5fe / 2));
                cMtx_ZrotM(*calc_mtx, s16(npc_ks_p->field_0x600));
                model->setAnmMtx(jointNo, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);

            } else if (npc_ks_p->field_0x5fc >= 2 && (jointNo == 3 || jointNo == 4)) {
                MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
                if (jointNo == 3) {
                    cMtx_YrotM(*calc_mtx, -(npc_ks_p->field_0x5fe / 2));
                    cMtx_XrotM(*calc_mtx, -(npc_ks_p->field_0x5fe / 2));
                } else {
                    cMtx_YrotM(*calc_mtx, -npc_ks_p->field_0x5fe);
                }

                model->setAnmMtx(jointNo, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }

            if (npc_ks_p->field_0x5fc >= 2 && (jointNo == 1 || jointNo == 2)) {
                MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
                if (jointNo == 2) {
                    cMtx_YrotM(*calc_mtx, npc_ks_p->field_0x602 / 2);
                } else {
                    cMtx_YrotM(*calc_mtx, npc_ks_p->field_0x602);
                }

                cMtx_XrotM(*calc_mtx, -npc_ks_p->field_0x5fe);
                model->setAnmMtx(jointNo, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }

            if (jointNo == 15) {
                MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
                cMtx_ZrotM(*calc_mtx, npc_ks_p->field_0x606);
                model->setAnmMtx(jointNo, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static int daNpc_Ks_Draw(npc_ks_class* npc_ks) {
    if (npc_ks->field_0xbd7 != 0) {
        npc_ks->field_0xbd7 = 0;
        return 1;
    }

    fopAc_ac_c* a_this = &npc_ks->actor;
    J3DModel* model = npc_ks->mpModelMorf->getModel();
    if (npc_ks->mResName == "Npc_kst") {
        g_env_light.settingTevStruct(4, &a_this->current.pos, &a_this->tevStr);
    } else {
        g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    }

    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);

    if (npc_ks->field_0x5e4 != 0) {
        npc_ks->mBtp2->entry(model->getModelData());
    } else {
        npc_ks->mBtp1->entry(model->getModelData());
    }
    npc_ks->mpModelMorf->entryDL();

    if (npc_ks->field_0xc17 != 0) {
        g_env_light.setLightTevColorType_MAJI(npc_ks->mpStickModel, &a_this->tevStr);
        mDoExt_modelUpdateDL(npc_ks->mpStickModel);
    }

    if (npc_ks->mpRoseModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(npc_ks->mpRoseModel, &a_this->tevStr);
        mDoExt_modelUpdateDL(npc_ks->mpRoseModel);
    }

    cXyz sp28;
    sp28.set(a_this->current.pos.x, a_this->current.pos.y + 100.0f, a_this->current.pos.z);
    npc_ks->mShadowKey = dComIfGd_setShadow(npc_ks->mShadowKey, 1, model, &sp28, 1000.0f, 0.0f, a_this->current.pos.y, 
                                            npc_ks->mObjAcch.GetGroundH(), npc_ks->mObjAcch.m_gnd, &npc_ks->actor.tevStr,
                                            0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    if (npc_ks->field_0xc17 != 0) {
        dComIfGd_addRealShadow(npc_ks->mShadowKey, npc_ks->mpStickModel);
    }

    return 1;
}

static void* s_resq_sub(void* i_actor, void* i_data) {
    (void) i_data;
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KS) {
        ((npc_ks_class*)i_actor)->field_0x94c = cM_rndF(50.0f) + 220.0f;
    }
    return NULL;
};

static cXyz obj_pos;

static npc_ks_class* leader;

static u32 call_pt;

static u8 l_HIOInit;

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
    fopAc_ac_c* a_this = &i_this->actor;
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
            sp4c.x = actor_p->current.pos.x - a_this->eyePos.x;
            sp4c.y = (actor_p->current.pos.y + 50.0f) - a_this->eyePos.y;
            sp4c.z = actor_p->current.pos.z - a_this->eyePos.z;
            f32 sq_rt = JMAFastSqrt(sp4c.x * sp4c.x + sp4c.z * sp4c.z);
            if (sq_rt < fVar1) {
                if (param_2 == 0) {
                    return actor_p;
                }

                if (target_bgc[i] != 0 || fopAcM_otherBgCheck(a_this, actor_p)) {
                    target_bgc[i] = 1;
                } else if (NPC_KS_FABSF(sp4c.y) <= 300.0f) {
                    s16 sVar1 = a_this->shape_angle.y - cM_atan2s(sp4c.x, sp4c.z);
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
    fopAc_ac_c* a_this = &i_this->actor;
    return search_bomb(i_this, 1);
}

static fopAc_ac_c* bomb_check(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
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
    fopAc_ac_c* a_this = &i_this->actor;
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
    if (fopAcM_GetRoomNo(a_this) == 1) {
        fVar2 = 800.0f;
    }

    if (target_info_count != 0) {
        cXyz sp68, unused_cxyz;
        int i = 0;
        while (i < target_info_count) {
            enemy_p = (fopAc_ac_c*)target_info[i];
            sp68.x = enemy_p->current.pos.x - a_this->eyePos.x;
            sp68.y = (enemy_p->current.pos.y + 50.0f) - a_this->eyePos.y;
            sp68.z = enemy_p->current.pos.z - a_this->eyePos.z;
            f32 square_root = JMAFastSqrt(sp68.x * sp68.x + sp68.z * sp68.z);
            if (square_root < fVar1) {
                if (param_2 == 0) {
                    return enemy_p;
                }

                if (target_bgc[i] != 0 || fopAcM_otherBgCheck(a_this, enemy_p)) {
                    target_bgc[i] = 1;
                } else if (NPC_KS_FABSF(sp68.y) <= fVar2) {
                    s16 sVar1 = a_this->shape_angle.y - cM_atan2s(sp68.x, sp68.z);
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
    fopAc_ac_c* a_this = &i_this->actor;
    return search_enemy(i_this, 1, param_2);
}

static fopAc_ac_c* enemy_check(npc_ks_class* i_this, f32 param_2) {
    fopAc_ac_c* a_this = &i_this->actor;
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
    fopAc_ac_c* a_this = &i_this->actor;
    target_info_count = 0;
    for (int i = 0; i < 10; i++) {
        target_info[i] = NULL;
    }

    fpcM_Search(s_en_sub, i_this);

    f32 fVar1 = 50.0f;
    if (target_info_count != 0) {
        cXyz sp4c;
        int i = 0;
        while (i < target_info_count) {
            fopAc_ac_c* actor_p = (fopAc_ac_c*)target_info[i];
            sp4c = actor_p->current.pos - a_this->current.pos;
            f32 abs = sp4c.abs(); 
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
    obj_so_class* monkeycage_p;
    if (leader != NULL) {
        fopAc_ac_c* a_this = &i_this->actor;
        if (i_this->mMode == 0) {
            monkeycage_p = (obj_so_class*)fpcM_Search(s_ori_sub, i_this);
            if (monkeycage_p != NULL) {
                a_this->parentActorID = fopAcM_GetID(monkeycage_p);
                anm_init(i_this, 20, 5.0f, 2, 1.0f);
                i_this->mMode++;
                i_this->mMode = 1;
            } else {
                return;
            }
        }

        monkeycage_p = (obj_so_class*) fopAcM_SearchByID(a_this->parentActorID);
        if (monkeycage_p != NULL) {
            if (monkeycage_p->actor.health == 0) {
                i_this->mActionID = 201;
                i_this->mMode = 10;
                dComIfGs_onTbox(i_this->bitTRB);
            } else {
                if (i_this->mMode < 10 && monkeycage_p->field_0xdae != 0) {
                    i_this->mMode = 10;
                    i_this->mpModelMorf->setPlaySpeed(0.0f);
                }

                switch (i_this->mMode) {
                    case 1: {
                        if (i_this->mpModelMorf->checkFrame(5.0f) || i_this->mpModelMorf->checkFrame(20.0f)) {
                            monkeycage_p->field_0xdc8 += 400.0f + KREG_F(9);
                            monkeycage_p->field_0xdac = 0;
                            i_this->mSound.startCreatureSound(Z2SE_OBJ_MONKEYJAIL_CREAK, 0, -1);
                            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_PRISONED, -1);
                        }

                        break;
                    }

                    case 10: {
                        break;
                    }
                }

                a_this->current.angle.y = monkeycage_p->actor.shape_angle.y;
                a_this->current.angle.x = monkeycage_p->actor.shape_angle.x;
                a_this->current.pos = monkeycage_p->actor.current.pos;
                a_this->current.pos.y += TREG_F(3);

                if (leader->field_0xb42 == 0) {
                    if (!dComIfGs_isSwitch(20, fopAcM_GetRoomNo(a_this))) {
                        cXyz sp2c(11497.0f, 3764.0f, 3810.0f);
                        daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
                        sp2c -= player->current.pos;
                        sp2c.y = 0.0f;
                        if (sp2c.abs() < 300.0f) {
                            leader->field_0xb42 = 110;
                        }
                    }
                }
            }
        }
    }
}

static int npc_ks_ori(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp30, sp3c;
    int rv = 0;
    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 21, 10.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->mTimers[0] = cM_rndF(200.0f) + 100.0f;
            break;

        case 1:
            if (i_this->mpModelMorf->checkFrame(5.0f) || i_this->mpModelMorf->checkFrame(20.0f)) {
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_PRISONED, -1);
                i_this->mSound.startCreatureSound(Z2SE_OBJ_MONKEYJAIL_CREAK, 0, -1);
            }

            if (i_this->mTimers[0] == 0 && i_this->mpModelMorf->checkFrame(99.0f)) {
                i_this->mMode = 2;
                anm_init(i_this, 7, 5.0f, 0, 1.0f);
                i_this->mTimers[0] = cM_rndF(60.0f) + 40.0f;
            }
            break;

        case 2:
            if (i_this->mpModelMorf->isStop()) {
                if (i_this->mTimers[0] == 0) {
                    i_this->mMode = 0;
                } else {
                    anm_init(i_this, 7, 5.0f, 0, 1.0f);
                }
            }
            break;

        case 10:
            cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
            sp30.x = 0.0f;
            sp30.y = 0.0f;
            sp30.z = a_this->speedF;
            MtxPosition(&sp30, &sp3c);
            a_this->current.pos += sp3c;

            if (i_this->mTimers[1] == 1) {
                a_this->speedF = 0.0f;
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
            }

            if (i_this->mTimers[0] == 0) {
                a_this->health = 0;
                i_this->mActionID = 100;
                i_this->mTimers[0] = 20;
                i_this->mMode = 11;
                i_this->field_0x8fc.y = i_this->field_0x5c8;
                fopAcM_setStageLayer(a_this);
                fpcM_Search(s_resq_sub, i_this);
            }
            break;

        case 11:
            rv = 1;
            if (i_this->mTimers[1] != 0) {
                a_this->speedF = -20.0f + NREG_F(11);
            } else {
                a_this->speedF = 0.0f;
            }

            if (i_this->mTimers[0] == 50) {
                anm_init(i_this, 11, 3.0f, 2, 1.0f);
            }

            if (i_this->mTimers[0] == 0) {
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
                i_this->mTimers[0] = 15;
                i_this->mMode++;
            }
            break;

        case 12:
            rv = 1;
            if (i_this->mTimers[0] == 1) {
                anm_init(i_this, 32, 2.0f, 0, 1.0f);
                if (fopAcM_GetRoomNo(a_this) == 11) {
                    a_this->current.angle.y += NPC_KS_36DEG_ROT;
                }
            }

            if (i_this->field_0x5d0 == 26) {
                a_this->speedF = 20.0f;
            }

            if (i_this->field_0x5d0 == 32 && i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                a_this->speedF = 40.0f;
                a_this->speed.y = 35.0f;
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                i_this->field_0xbe0 = 1;
                i_this->mMode++;
                i_this->mTimers[0] = 50;
            }
            break;

        case 13:
            rv = 1;
            a_this->gravity = -5.0f;
            if (i_this->mTimers[0] == 0 || !dComIfGp_event_runCheck()) {
                fopAcM_delete(a_this);
            }
    }

    if (i_this->mMode < 10) {
        int swBit = (fopAcM_GetParam(a_this) & 0xFF000000) >> 24;
        if (swBit != 0xFF) {
            if (dComIfGs_isSwitch(swBit, fopAcM_GetRoomNo(a_this))) {
                dComIfGs_onTbox(i_this->bitTRB);
                if (i_this->field_0x5b6 >= 5) {
                    i_this->mMode = 11;
                    i_this->mTimers[0] = 80;
                    anm_init(i_this, 51 ,5.0f, 2, 1.0f);
                    if (fopAcM_GetRoomNo(a_this) == 11) {
                        i_this->mTimers[1] = 10;
                    }
                } else {
                    i_this->mMode = 10;
                    a_this->speedF = -10.0f + NREG_F(11);
                    i_this->mTimers[0] = 110 + nREG_S(2);
                    i_this->mTimers[1] = 15;
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
        cXyz sp28(((daBoomerang_c*)i_actor)->current.pos - ((fopAc_ac_c*)i_data)->current.pos);
        if (sp28.abs() < 1000.0f) {
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
    fopAc_ac_c* a_this = &i_this->actor;
    obj_so_class* cage_p;
    int rv = 1;
    if (i_this->mMode == 0) {
        cage_p = (obj_so_class*)fpcM_Search(s_ori_sub, i_this);
        if (cage_p != NULL) {
            a_this->parentActorID = fopAcM_GetID(cage_p);
            anm_init(i_this, 20, 5.0f, 2, 1.0f);
            i_this->mMode = 1;
            a_this->home.angle.y = cage_p->actor.shape_angle.y;
            i_this->field_0xbde = i_this->field_0x5c8 - a_this->home.angle.y;
        }

        return rv;
    }

    cage_p = (obj_so_class*)fopAcM_SearchByID(a_this->parentActorID);
    if (cage_p == NULL) {
        return rv;
    }

    a_this->home.angle.y = cage_p->actor.shape_angle.y;

    if (i_this->mMode <= 2 && cage_p->field_0x1054 != 0) {
        i_this->mMode = 3;
        i_this->mTimers[0] = 90;
        int swBit = (fopAcM_GetParam(a_this) & 0xFF000000) >> 24;
        dComIfGs_onSwitch(swBit, fopAcM_GetRoomNo(a_this));
        dComIfGs_onTbox(i_this->bitTRB);

        if (fopAcM_GetRoomNo(a_this) == 4) {
            i_this->field_0xb42 = 70;
        } else if (fopAcM_GetRoomNo(a_this) == 19) {
            i_this->field_0xb42 = 75;
        } else if (fopAcM_GetRoomNo(a_this) == 22) {
            i_this->field_0xb42 = 72;
        }

        return rv;
    }

    cXyz sp40;
    s16 sVar1 = i_this->field_0xbde - 1000 + TREG_S(8) + a_this->home.angle.y;
    f32 fVar1 = 0.0f;
    switch (i_this->mMode) {
        case 1:
            if (i_this->mpModelMorf->checkFrame(46.0f + TREG_F(11))) {
                i_this->field_0xbde = i_this->field_0x5c8 - a_this->home.angle.y & -0x2000 + TREG_S(7);
            }

            if (i_this->mpModelMorf->checkFrame(5.0f) || i_this->mpModelMorf->checkFrame(20.0f)) {
                cage_p->field_0xdc8 += 100.0f + KREG_F(9);
                cage_p->field_0xdac = 0;
                i_this->mSound.startCreatureSound(Z2SE_OBJ_MONKEYJAIL_CREAK, 0, -1);
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_POINT, -1);
            }

            if (enemy_check(i_this, 400.0f) == NULL) {
                if (daPy_getPlayerActorClass()->getCutAtFlg() == 0 && fpcM_Search(shot_bo_sub, i_this) == NULL) break;
            }

            anm_init(i_this, 41, 5.0f, 2, 1.0f);
            i_this->mMode = 2;
            i_this->mTimers[0] = cM_rndF(20.0f) + 40.0f;
            break;

        case 2:
            sVar1 += NPC_KS_NEG_180DEG_ROT;
            if (i_this->mTimers[0] == 0) {
                if (cage_p->partBreak()) {
                    anm_init(i_this, 22, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 20, 5.0f, 2, 1.0f);
                }

                i_this->mMode = 1;
            } else {
                if (enemy_check(i_this, 400.0f) == NULL) {
                    if (daPy_getPlayerActorClass()->getCutAtFlg() == 0) break;
                }

                i_this->mTimers[0] = cM_rndF(20.0f) + 40.0f;
            }
            break;

        case 3:
            fVar1 = -20.0f;
            if (i_this->mTimers[0] == 50) {
                anm_init(i_this, 51, 3.0f, 2, 1.0f);
            }

            if (i_this->mTimers[0] < 50) {
                cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
                if (i_this->mTimers[0] < 25) {
                    sp40.x = 200.0f;
                } else {
                    sp40.x = -200.0f;
                }
                sp40.y = 50.0f + KREG_F(11);
                sp40.z = 50.0f;
                MtxPosition(&sp40, &i_this->field_0x940);
                i_this->field_0x940 += a_this->current.pos;
                i_this->field_0x93c = 20;
            }

            if (i_this->mTimers[0] == 0) {
                anm_init(i_this, 30, 5.0f, 2, 1.0f);
                i_this->mMode = 4;
                i_this->mTimers[0] = 60;
                i_this->field_0x93c = 0;
            }
            break;

        case 4:
            fVar1 = -20.0f;
            i_this->field_0x5fc = 0;
            if (i_this->mTimers[0] == 0) {
                if (fopAcM_GetRoomNo(a_this) == 22) {
                    i_this->mActionID = 116;
                    i_this->mMode = 0;
                    i_this->field_0xaec = 1;
                } else if (fopAcM_GetRoomNo(a_this) == 4) {
                    i_this->mMode = 10;
                } else {
                    i_this->mMode = 5;
                }
            }
            break;

        case 5:
            i_this->field_0x5fc = 0;
            fVar1 = -20.0f;
            anm_init(i_this, 32, 2.0f, 0, 1.0f);
            i_this->mMode++;
            break;

        case 6:
            i_this->field_0x5fc = 0;
            fVar1 = -20.0f;
            sVar1 += NPC_KS_45DEG_ROT;
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                a_this->speedF = 40.0f;
                a_this->speed.y = 35.0f;
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                i_this->field_0xbe0 = 1;
                i_this->mMode++;
            }
            break;

        case 7:
            i_this->field_0x5fc = 0;
            sVar1 += NPC_KS_90DEG_ROT;
            a_this->gravity = -5.0f;
            break;

        case 10:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mMode = 11;
            i_this->field_0x904 = 0;
            a_this->speedF = 0.0f;
            // fallthrough
        case 11:
            if (guide_path_04[i_this->field_0x904].field_0x0 != 0) {
                i_this->field_0x8f0.x = guide_path_04[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.y = guide_path_04[i_this->field_0x904].field_0x8;
                i_this->field_0x8f0.z = guide_path_04[i_this->field_0x904].field_0xc;
                i_this->field_0x910 = a_this->current.pos;
                i_this->field_0x91c = i_this->field_0x8f0;
                i_this->mMode = 12;
                anm_init(i_this, 32, 2.0f, 0, 1.0f);
                a_this->speedF = 0.0f;
            } else {
                i_this->field_0x8f0.x = guide_path_04[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.z = guide_path_04[i_this->field_0x904].field_0xc;
                i_this->mMode = 15;
                a_this->speedF = l_HIO.demo_speed_2;
                if (i_this->field_0x5d0 != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 12:
            rv = 2;
            if (i_this->field_0x5d0 == 32 && i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                a_this->speedF = 40.0f + TREG_F(9);
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                i_this->field_0xbe0 = 1;
            }

            sp40 = i_this->field_0x8f0 - a_this->current.pos;
            i_this->field_0x8fc.y = cM_atan2s(sp40.x, sp40.z);
            i_this->field_0x8fc.x = -cM_atan2s(sp40.y, JMAFastSqrt(sp40.x * sp40.x + sp40.z * sp40.z));
            if (sp40.abs() < a_this->speedF * 1.2f) {
                a_this->current.pos = i_this->field_0x8f0;
                i_this->mMode = 11;
                i_this->field_0xbe0 = 1;
                i_this->field_0x904++;
                if (guide_path_04[i_this->field_0x904].field_0x0 < 0) {
                    fopAcM_delete(a_this);
                }
            }
            break;

        case 15:
            sp40.x = i_this->field_0x8f0.x - a_this->current.pos.x;
            sp40.z = i_this->field_0x8f0.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp40.x, sp40.z);
            if (JMAFastSqrt(sp40.x * sp40.x + sp40.z * sp40.z) < (a_this->speedF * 1.2f)) {
                i_this->field_0x904++;
                i_this->mMode = 11;
            }
    }

    if (i_this->mMode >= 10) {
        i_this->field_0x5fc = 0;
        sVar1 = i_this->field_0x8fc.y;
        if (a_this->speedF > 25.0f) {
            i_this->field_0x90c = 1;
        }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, sVar1, 2, 0x2000);
    
    if (i_this->mMode < 7) {
        a_this->current.angle.x = cage_p->actor.shape_angle.x;
        a_this->current.pos = cage_p->actor.current.pos;
        a_this->current.pos.y += fVar1;
    }

    return rv;
}

static void npc_ks_home(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp2c, sp38;
    switch (i_this->mMode) {
        case 0:
            if (cM_rndF(1.0f) < 0.5f) {
                anm_init(i_this, 30, 3.0f, 0, 1.0f);
            } else {
                anm_init(i_this, 5, 3.0f, 0, 1.0f);
            }

            i_this->mMode = 1;
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_WAIT, -1);
            break;

        case 1:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mMode = 0;
            }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5c8, 2, 0x800);
    if (checkDoorDemo() != 0) {
        int roomNo = fopAcM_GetRoomNo(a_this);
        int nextStayNo = dStage_roomControl_c::getNextStayNo();
        if (roomNo == 0 && nextStayNo == 4) {
            i_this->mActionID = 100;
            i_this->mMode = 0;
            i_this->field_0x904 = 0;
            i_this->field_0xaec = 1;
            if (fopAcM_CheckCondition(a_this, 4) != 0) {
                camera_class* camera = dComIfGp_getCamera(0);
                sp2c.x = camera->lookat.eye.x - camera->lookat.center.x;
                sp2c.z = camera->lookat.eye.z - camera->lookat.center.z;
                cMtx_YrotS(*calc_mtx, cM_atan2s(sp2c.x, sp2c.z));
                if ((i_this->field_0x5b6 & 1) != 0) {
                    sp2c.x = 100.0f;
                } else {
                    sp2c.x = -100.0f;
                }
                sp2c.y = -50.0f;
                sp2c.z = 200.0f;
                MtxPosition(&sp2c, &sp38);
                a_this->current.pos = camera->lookat.eye + sp38;
                a_this->old = a_this->current;
            }
        }
    }
}

static void* s_sw_sub(void* i_actor, void* i_data) {
    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_SW)) {
        if (fopAcM_GetRoomNo((fopAc_ac_c*)i_data) == 0) {
            if (((npc_ks_class*)i_data)->field_0x5b6 == ((obj_sw_class*)i_actor)->field_0x570) {
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
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KS && ((npc_ks_class*)i_actor)->mActionID == 4) {
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
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* unused_p = dComIfGp_getPlayer(0);
    cXyz sp3c, sp48;
    f32 fVar1 = 0.0f;
    int rv = 1;
    int iVar1 = 0;

    obj_so_class* cage_p = (obj_so_class*)fopAcM_SearchByName(PROC_OBJ_SO);
    if (cage_p == NULL) {
        return 0;
    }
    
    if (cage_p->field_0xdae != 0 && i_this->mMode < 17) {
        i_this->mActionID = 201;
        i_this->mMode = 0;
        return 0;
    }
    
    daPillar_c* pillar_p = (daPillar_c*)fpcM_Search(s_ha_sub, i_this);
    if (pillar_p == NULL) {
        return 0;
    }
    fopAc_ac_c* a_pill_p = pillar_p;

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->field_0x904 = 0;
            // fallthrough
        case 1:
            i_this->field_0x8f0.x = move_path_02[i_this->field_0x904].field_0x4;
            i_this->field_0x8f0.y = move_path_02[i_this->field_0x904].field_0x8;
            i_this->field_0x8f0.z = move_path_02[i_this->field_0x904].field_0xc;
            i_this->mMode = 2;
            // fallthrough
        case 2:
            fVar1 = l_HIO.demo_speed;
            sp3c = i_this->field_0x8f0 - a_this->current.pos;
            i_this->field_0x8fc.y = cM_atan2s(sp3c.x, sp3c.z);
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 2, 0x1000);
            sp3c.y = 0.0f;
            if (sp3c.abs() <= a_this->speedF * 2.0f) {
                i_this->field_0x904++;
                if (i_this->field_0x904 < 5) {
                    i_this->mMode = 1;
                } else {
                    i_this->mMode = 10;
                    anm_init(i_this, 25, 5.0f, 2, 1.0f);
                    i_this->mTimers[0] = 35;
                }
            }
            break;

        case 10:
            cLib_addCalcAngleS2(&a_this->current.angle.y, cage_p->actor.shape_angle.y + 0x8000, 2, 0x1000);
            if (i_this->mTimers[0] == 0) {
                i_this->mMode = 11;
                anm_init(i_this, 26, 5.0f, 2, 1.0f);
            }
            break;

        case 11:
            rv = 0;
            fVar1 = l_HIO.demo_speed;
            cMtx_YrotS(*calc_mtx, cage_p->actor.shape_angle.y);
            sp3c.x = 0.0f;
            sp3c.y = 0.0f;
            sp3c.z = (90.0f + NREG_F(1)) * cage_p->actor.scale.x;
            MtxPosition(&sp3c, &sp48);
            sp48 += cage_p->actor.current.pos;
            cLib_addCalc2(&a_this->current.pos.x, sp48.x, 1.0f, a_this->speedF);
            cLib_addCalc2(&a_this->current.pos.z, sp48.z, 1.0f, a_this->speedF);
            sp3c.x = sp48.x - a_this->current.pos.x;
            sp3c.z = sp48.z - a_this->current.pos.z;
            sp3c.y = JMAFastSqrt(sp3c.x * sp3c.x + sp3c.z * sp3c.z);
            if (sp3c.y < 55.0f && i_this->field_0x5d0 != 12) {
                anm_init(i_this, 12, 3.0f, 2, 1.0f);
            }

            if (sp3c.y < 1.0f + NREG_F(0)) {
                i_this->mMode = 12;
                a_this->current.angle.y = cage_p->actor.shape_angle.y + 0x8000;
            }
            break;

        case 12:
            rv = 0;
            iVar1 = 1;
            fVar1 = l_HIO.demo_speed;
            cMtx_YrotS(*calc_mtx, cage_p->actor.shape_angle.y);
            sp3c.x = 0.0f;
            sp3c.y = 0.0f;
            sp3c.z = 90.0f + NREG_F(1);
            MtxPosition(&sp3c, &sp48);
            sp48 += cage_p->actor.current.pos;
            cLib_addCalc2(&a_this->current.pos.x, sp48.x, 1.0f, a_this->speedF);
            cLib_addCalc2(&a_this->current.pos.y, sp48.y, 1.0f, a_this->speedF);
            cLib_addCalc2(&a_this->current.pos.z, sp48.z, 1.0f, a_this->speedF);
            sp3c = sp48 - a_this->current.pos;
            if (sp3c.abs() < 1.0f + NREG_F(0)) {
                i_this->mMode = 13;
                anm_init(i_this, 23, 2.0f, 2, 1.0f);
                i_this->mTimers[0] = 60;
            }
            break;

        case 13:
            rv = 0;
            iVar1 = 1;
            cMtx_YrotS(*calc_mtx, cage_p->actor.shape_angle.y);
            sp3c.x = 0.0f;
            sp3c.y = 0.0f;
            sp3c.z = (90.0f + NREG_F(1)) * cage_p->actor.scale.x;
            MtxPosition(&sp3c, &sp48);
            sp48 += cage_p->actor.current.pos;
            cLib_addCalc2(&a_this->current.pos.x, sp48.x, 1.0f, 10.0f);
            cLib_addCalc2(&a_this->current.pos.z, sp48.z, 1.0f, 10.0f);
            
            if (i_this->mpModelMorf->checkFrame(10.0f) || i_this->mpModelMorf->checkFrame(20.0f)) {
                cage_p->field_0xdc8 += 200.0f;
                i_this->mSound.startCreatureSound(Z2SE_OBJ_MONKEYJAIL_CREAK, 0, -1);
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_POINT, -1);
            }

            if (i_this->mTimers[0] == 0) {
                i_this->mMode = 14;
                anm_init(i_this, 15, 2.0f, 2, 1.0f);
            }
            break;

        case 14:
            rv = 0;
            iVar1 = 1;
            cMtx_YrotS(*calc_mtx, cage_p->actor.shape_angle.y);
            sp3c.x = 0.0f;
            sp3c.y = 0.0f;
            sp3c.z = 90.0f + NREG_F(1);
            MtxPosition(&sp3c, &sp48);
            sp48 += cage_p->actor.current.pos;
            cLib_addCalc2(&a_this->current.pos.x, sp48.x, 1.0f, 10.0f);
            cLib_addCalc2(&a_this->current.pos.z, sp48.z, 1.0f, 10.0f);
            cLib_addCalc2(&a_this->current.pos.y, i_this->mObjAcch.GetGroundH(), 1.0f, l_HIO.demo_speed);
            
            if (NPC_KS_FABSF(a_this->current.pos.y - i_this->mObjAcch.GetGroundH()) < 1.0f) {
                anm_init(i_this, 26, 5.0f, 2, 1.0f);
                a_this->speedF = 0.0f;
                i_this->mTimers[0] = 20;
                i_this->mMode = 15;
            }
            break;

        case 15:
            fVar1 = l_HIO.demo_speed;
            cLib_addCalcAngleS2(&a_this->current.angle.y, s16(cage_p->actor.shape_angle.y), 2, 0x2000);
            if (i_this->mTimers[0] == 0) {
                i_this->mMode = 16;
                anm_init(i_this, 25, 5.0f, 2, 1.0f);
            }
            break;

        case 16: {
            i_this->field_0xaec = 2;
            i_this->field_0x5fc = 1;
            if (i_this->mTimers[0] == 0) {
                if (i_this->field_0x5d0 != 25) {
                    anm_init(i_this, 25, 5.0f, 2, 1.0f);
                    i_this->mTimers[0] = 25;
                } else {
                    anm_init(i_this, 50, 5.0f, 2, 1.0f);
                    i_this->mTimers[0] = cM_rndF(30.0f) + 30.0f;
                }
            }

            cLib_addCalcAngleS2(&a_this->current.angle.y, cage_p->actor.shape_angle.y + 0x8000, 2, 0x1000);
            if (dComIfGp_checkPlayerStatus0(0, 0x40) != 0 || dComIfGp_checkPlayerStatus0(0, 0x2000) != 0) {
                i_this->mTimers[2] = 20;
            }

            if (i_this->mTimers[2] == 0 && cage_p->field_0xdc4 == 0) {
                fopAcM_OnStatus(a_this, 0);
                cLib_onBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_TALKCHECK_e | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
                a_this->eventInfo.onCondition(dEvtCnd_CANTALK_e);
                a_this->attention_info.distances[fopAc_attn_TALK_e] = 2;
                a_this->attention_info.distances[fopAc_attn_SPEAK_e] = 2;
            }

            dAttention_c* attention_p = dComIfGp_getAttention();
            if (a_this == attention_p->LockonTarget(0) && i_this->mTimers[2] == 0 
                && mDoCPd_c::getTrigA(PAD_1) != 0) {
                i_this->mMode = 17;
                anm_init(i_this, 25, 2.0f, 0, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                a_this->speedF = 0.0f;
                leader->field_0xb42 = 60;
            }
            break;
        }

        case 17:
            rv = 0;
            if (i_this->field_0x5d0 == 25) {
                if (i_this->mpModelMorf->isStop()) {
                    anm_init(i_this, 49, 3.0f, 0, 1.0f);
                }
            } else {
                fVar1 = 30.0f;
            }

            if (i_this->field_0x5d0 == 49 && i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 38, 1.0f, 2, 1.0f);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_ROLL, 0, -1);
            }

            sp3c = a_pill_p->current.pos - a_this->current.pos;
            a_this->current.angle.y = cM_atan2s(sp3c.x, sp3c.z);
            cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
            sp3c.x = 0.0f;
            sp3c.y = 0.0f;
            sp3c.z = a_this->speedF;
            MtxPosition(&sp3c, &sp48);
            a_this->current.pos += sp48;
            sp3c.x = a_pill_p->current.pos.x - a_this->current.pos.x;
            sp3c.z = a_pill_p->current.pos.z - a_this->current.pos.z;
            if (JMAFastSqrt(sp3c.x * sp3c.x + sp3c.z * sp3c.z) < 90.0f + NREG_F(7)) {
                pillar_p->setShake(daPillar_c::SHAKE_STRONG);
                anm_init(i_this, 6, 5.0f, 0, 1.0f);
                i_this->mMode = 18;
                a_this->speedF = -20.0f + NREG_F(4);
                a_this->speed.y = 30.0f + NREG_F(5);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_ATTACK_POLL, 0, -1);
                if (cage_p != NULL) {
                    cage_p->field_0xdc8 = 1500.0f + NREG_F(8);
                    i_this->mSound.startCreatureSound(Z2SE_OBJ_MONKEYJAIL_SHAKE, 0, -1);
                }
            }
            break;

        case 18:
            fVar1 = -20.0f + NREG_F(4);
            cLib_addCalcAngleS2(&a_this->current.angle.x, ZREG_S(7) - 0x3830, 1, 0x600);
            if (a_this->speed.y <= 0.0f && i_this->mObjAcch.ChkGroundHit()) {
                i_this->mMode = 19;
                a_this->speed.y = 20.0f + NREG_F(6);
                a_this->current.angle.x = ZREG_S(7) - 0x3830;
                i_this->mTimers[0] = 40;
                i_this->mTimers[1] = 10;
                fopAcM_effSmokeSet1(&i_this->field_0x858, &i_this->field_0x85c, &a_this->eyePos, NULL, 1.7f + TREG_F(18), &a_this->tevStr, 1);
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_CRUSHED, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_FALL_GROUND, 0, -1);
                anm_init(i_this, 13, 2.0f, 2, 1.0f);
            }
            break;

        case 19:
            if (i_this->mTimers[1] != 0) {
                fVar1 = a_this->speedF = -15.0f + NREG_F(14);
            } else {
                fVar1 = a_this->speedF = 0.0f;
            }

            if (i_this->mTimers[0] == 0) {
                anm_init(i_this, 35, 3.0f, 0, 1.0f);
                i_this->mMode = 20;
            }
            break;

        case 20:
            cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x800);
            if (i_this->mpModelMorf->isStop()) {
                i_this->mMode = 16;
                anm_init(i_this, 25, 5.0f, 2, 1.0f);
                if (leader->field_0xb42 != 0) {
                    leader->field_0xb42 = 100;
                }
                i_this->mTimers[2] = 30;
            }
            break;
    }

    cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, 4.0f);
    if (checkDoorDemo() != 0) {
        i_this->mActionID = 100;
        i_this->mMode = 0;
    } else if (iVar1 != 0 && cage_p->actor.shape_angle.x > 0x200) {
        anm_init(i_this, 6, 5.0f, 0, 1.0f);
        i_this->mMode = 18;
        a_this->speedF = -8.0f + NREG_F(4);
        a_this->speed.y = 10.0f + NREG_F(5);
        i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_PRISONED, -1);
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
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz unused_cxyz_0, unused_cxyz_1;
    f32 reg_f31 = 0.0f;     // doesn't change
    BOOL retval = TRUE;     // doesn't change
    daE_OC_c* bokoblin_p = (daE_OC_c*)fpcM_Search(s_dn_sub, i_this);
    int frame = i_this->mpModelMorf->getFrame();

    switch(i_this->mMode) {
        case 0:
            anm_init(i_this, 51, 5.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->mTimers[0] = 100;
            // fallthrough
        case 1:
            i_this->field_0x5fc = 1;
            if (i_this->mTimers[0] == 0) {
                i_this->mMode = 20;
                anm_init(i_this, 30, 3.0f, 2, 1.0f);
                i_this->mTimers[0] = 90;
                fpcM_Search(s_resq_sub, i_this);
            }
            break;

        case 10:
            anm_init(i_this, 13, 2.0f, 2, 1.0f);
            i_this->mTimers[0] = 30;
            i_this->mMode = 11;
            a_this->current.angle.x = ZREG_S(7) - 0x3830;
            // fallthrough
        case 11:
            if (i_this->mTimers[0] == 0) {
                anm_init(i_this, 35, 3.0f, 0, 1.0f);
                i_this->mMode = 12;
                mDoAud_seStart(12, 0, 0, 0);
                Z2GetAudioMgr()->muteSceneBgm(120, 0.0f);
                Z2GetAudioMgr()->setBattleBgmOff(true);
            }
            break;

        case 12:
            cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x800);
            if (i_this->mpModelMorf->isStop()) {
                i_this->mMode = 20;
                anm_init(i_this, 30, 3.0f, 2, 1.0f);
                i_this->mTimers[0] = 90;
                fpcM_Search(s_resq_sub, i_this);
            }
            break;

        case 20:
            if (i_this->mTimers[0] == 0) {
                i_this->mMode = 21;
            }
            break;

        case 21:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mMode = 22;
            i_this->mTimers[0] = cM_rndF(80.0f) + 100.0f;
            i_this->field_0xaec = 1;
            break;

        case 22:
            if (((i_this->field_0x5d0 == 39 || i_this->field_0x5d0 == 43 && frame < 7) ||
                (frame > 40 && frame < 48) || frame > 65)) {
                i_this->field_0x5e0 = 4;
            }

            if ((i_this->mTimers[0] == 0) && (i_this->field_0x5d0 == 39)) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->field_0x5d0 == 43) {
                if (i_this->mpModelMorf->isStop()) {
                    i_this->mMode = 21;
                }
            }

            if (bokoblin_p == NULL) {
                i_this->mTimers[0] = cM_rndF(10.0f) + 30.0f;
                i_this->mMode = 23;
                int swBit = (fopAcM_GetParam(a_this) & 0xFF000000) >> 24;
                dComIfGs_onSwitch(swBit, fopAcM_GetRoomNo(a_this));
            }
            break;

        case 23:
            if (i_this->mTimers[0] == 0) {
                if (i_this->field_0x5b6 == 0) {
                    anm_init(i_this, 56, 3.0f, 2, 1.0f);
                }
                else {
                    anm_init(i_this, 11, 3.0f, 2, 1.0f);
                }
                i_this->mTimers[0] = cM_rndF(25.0f) + 70.0f;
                i_this->mMode = 24;
            }
            break;

        case 24:
            if (i_this->mTimers[0] == 0) {
                i_this->mActionID = 114;
                i_this->mMode = 0;
                i_this->mTimers[2] = 60;
            }
    }
    
    cLib_addCalc2(&a_this->speedF, reg_f31, 1.0f, 4.0f);
    return retval;
}

static void npc_ks_demo_04(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz unused_cxyz_0, unused_cxyz_1;
    f32 fVar1 = 0.0f;
    f32 fVar2 = 10.0f;
    switch(i_this->mMode) {
        case 0:
            anm_init(i_this, 51, 5.0f, 2, 1.0f);
            i_this->mMode = 1;
            break;

        case 2:
            a_this->current.pos.set(0.0f, 3300.0f, 4393.0f);
            a_this->current.angle.y = -0x8000;
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mMode = 3;
            break;

        case 3:
            fVar1 = l_HIO.demo_speed;
            if (a_this->field_0x567 != 0) {
                a_this->field_0x567 = 0;
                anm_init(i_this, 45, 3.0f, 0, 1.0f);
            }

            if (i_this->field_0x5d0 == 45) {
                if (i_this->mpModelMorf->checkFrame(20.0f)) {
                    mDoAud_seStart(0x5001a, 0, 0, 0);
                }

                fVar1 = 0.0f;
                fVar2 = 4.0f;
                if (i_this->mpModelMorf->getFrame() >= 37.0f + AREG_F(17)) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }

            break;

        case 4:
            i_this->mMode = 5;
            anm_init(i_this, 33, 5.0f, 0, 1.0f);
            mDoAud_seStart(0x5001a, 0, 0, 0);
            a_this->speed.y = 0.0f;
            break;

        case 5:
            fVar1 = l_HIO.demo_speed;
            a_this->gravity = -2.0f + KREG_F(7);
            break;

        case 6:
            anm_init(i_this, 12, 3.0f, 2, 1.0f);
            i_this->mMode = 7;
            // fallthrough
        case 7:
            a_this->gravity = 0.0f;
            a_this->speed.y = 10.0f;
            if ((i_this->field_0x5e6 & 31) == 0) {
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_WAIT, -1);
            }
            break;

        case 8:
            anm_init(i_this, 26, 5.0f, 2, 1.0f);
            i_this->mTimers[0] = 35;
            i_this->mMode = 9;
            break;

        case 9:
            fVar1 = l_HIO.demo_speed;
            fVar2 = 4.0f;
            if (i_this->mTimers[0] == 0) {
                anm_init(i_this, 37, 3.0f, 0, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_RELEIEF, -1);
                i_this->field_0x5e4 = 1;
                i_this->mMode = 10;
            }
            break;

        case 10:
            i_this->field_0x5fc = 1;
            if (i_this->mpModelMorf->isStop()) {
                i_this->field_0x5e4 = 0;
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
                i_this->mMode = 11;
                i_this->mTimers[0] = 25 + KREG_S(7);
            }
            break;

        case 11:
            fVar1 = l_HIO.demo_speed;
            fVar2 = 4.0f;
            if (i_this->mTimers[0] == 0) {
                anm_init(i_this, 7, 5.0f, 2, 1.0f);
                i_this->mTimers[0] = 120;
                i_this->mMode = 12;
            }
            break;

        case 12:
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5c8, 2, 0x1000);
            if (i_this->mTimers[0] == 0) {
                i_this->mActionID = 100;
                i_this->mMode = 0;
            }
            break;

        case 20:
            anm_init(i_this, 51, 5.0f, 2, 1.0f);
            i_this->mMode = 21;
            // fallthrough
        case 21:
            if (a_this->field_0x567 != 0) {
                a_this->field_0x567 = 0;
                i_this->mMode = 22;
                anm_init(i_this, 45, 3.0f, 0, 1.0f);
            }
            break;

        case 22:
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
            }

            if (i_this->field_0x5d0 == 26) {
                cLib_addCalcAngleS2(&a_this->current.angle.y, 0, 2, 0x1000);
                if (a_this->current.angle.y < 0x1000 && a_this->current.angle.y > -0x1000) {
                    fVar1 = l_HIO.demo_speed;
                    fVar2 = 10.0f;
                }
            }
    }

    cLib_addCalc2(&a_this->speedF, fVar1, 1.0f, fVar2);
}

static void npc_ks_to_hang(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp24, sp30;
    cXyz sp3c(i_this->field_0xbc8);
    
    switch (i_this->mMode) {
        case 0:
            i_this->field_0x8f0 = sp3c;
            i_this->field_0x910 = a_this->current.pos;
            i_this->field_0x91c = i_this->field_0x8f0;
            anm_init(i_this, 32, 2.0f, 0, 1.0f);
            a_this->speedF = 0.0f;
            i_this->mMode++;
            break;

        case 1:
            sp24 = i_this->field_0x8f0 - a_this->current.pos;
            if (i_this->field_0x5d0 == 32) {
                a_this->speedF = 0.0f;
                if (i_this->mpModelMorf->isStop()) {
                    anm_init(i_this, 33, 1.0f, 0, 1.0f);
                    a_this->speedF = 40.0f;
                    i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                    i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                    i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                    i_this->field_0xbe0 = 1;
                }
            }

            i_this->field_0x8fc.y = cM_atan2s(sp24.x, sp24.z);
            i_this->field_0x8fc.x = -cM_atan2s(sp24.y, JMAFastSqrt(sp24.x * sp24.x + sp24.z * sp24.z));
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 1, 0x2000);
            if (sp24.abs() <= a_this->speedF * 1.1f) {
                i_this->mMode = 10;
                obj_sw_class* sw_p = i_this->field_0x934;
                if (sw_p != NULL) {
                    if (sw_p->field_0x91c == 1) {
                        i_this->field_0x630 = sw_p->field_0xd8c / 2;
                    } else {
                        i_this->field_0x630 = ((i_this->field_0x5b6 + 1) * (sw_p->field_0xd8c / (sw_p->field_0x91c + 1)) - 1);
                    }

                    a_this->parentActorID = fopAcM_GetID(sw_p);

                    a_this->current.pos = i_this->field_0x8f0;
                    i_this->field_0x910 = a_this->current.pos;
                    anm_init(i_this, 32, 2.0f, 0, 1.0f);
                    i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
                }
            }
            break;
            
        case 2:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mMode = 0;
            }
            break;

        case 10:
            obj_sw_class* sw_p = (obj_sw_class*)fopAcM_SearchByID(a_this->parentActorID);
            i_this->field_0x8f0 = sw_p->field_0x920[i_this->field_0x630];
            i_this->field_0x91c = i_this->field_0x8f0;
            sp24 = i_this->field_0x8f0 - a_this->current.pos;
            if (i_this->field_0x5d0 == 32) {
                a_this->speedF = 0.0f;
                if (i_this->mpModelMorf->isStop()) {
                    anm_init(i_this, 33, 1.0f, 0, 1.0f);
                    a_this->speedF = 30.0f;
                    i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                    i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                    i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                    i_this->field_0xbe0 = 1;
                }
            }

            i_this->field_0x8fc.y = cM_atan2s(sp24.x, sp24.z);
            i_this->field_0x8fc.x = -cM_atan2s(sp24.y, JMAFastSqrt(sp24.x * sp24.x + sp24.z * sp24.z));
            if (sp24.abs() <= a_this->speedF* 1.1f) {
                a_this->current.pos = i_this->field_0x8f0;
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
                if (i_this->field_0x934->field_0x91c == 1) {
                    i_this->mActionID = 11;
                    i_this->mMode = 0;
                } else {
                    i_this->mActionID = 21;
                    i_this->mMode = 0;
                    i_this->mTimers[2] = 15;
                }
            }
    }

    if (a_this->speedF> 25.0f) {
        i_this->field_0x90c= 1;
    }
}

static void* s_next_do_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KS) {
        npc_ks_class* mon_data = (npc_ks_class*) i_data;
        npc_ks_class* monkey_actor = (npc_ks_class*) i_actor;
        if (monkey_actor->field_0x5b5 != 0 && mon_data->field_0x5b8 == (monkey_actor->field_0x5b8 - 1 & 0xFF)) {
            monkey_actor->mMode++;
            return i_actor;
        }
    }

    return NULL;
}

static void* s_next_get_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KS) {
        npc_ks_class* mon_data = (npc_ks_class*) i_data;
        npc_ks_class* monkey_actor = (npc_ks_class*) i_actor;
        if (monkey_actor->field_0x5b5 != 0 &&
        monkey_actor->field_0x5b8 != mon_data->field_0x5b8 && monkey_actor->field_0x620 != 0) {
            monkey_actor->field_0x620 = 0;
            anm_init(monkey_actor, 24, 1.0f, 2, 1.0f);
            mon_data->field_0x620 = 1;
            mon_data->field_0x624 = mon_data->field_0x614 - monkey_actor->field_0x614;
            mon_data->field_0x624 *= (0.1f + BREG_F(5));
            mon_data->field_0x634 = 10.0f + BREG_F(10);
            return i_actor;
        }
    }

    return NULL;
}

static void order_set(int param_1) {
    if (saru_p[0]->field_0x5c4 < saru_p[param_1 - 1]->field_0x5c4) {
        for (int i = 0; i < param_1; i++) {
            saru_p[i]->field_0x5b8 = i;
        }
    } else {
        for (int i = 0; i < param_1; i++) {
            saru_p[i]->field_0x5b8 = (param_1 - 1) - i;
        }
    }
}

static void all_carry_finish(int param_1) {
    for (int i = 0; i < param_1; i++) {
        saru_p[i]->mActionID = 10;
        saru_p[i]->mMode = 0;
    }
}

static void hang_end_check(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp30, sp3c;
    if (i_this->mRoomMonkeyID > 3 || !dComIfGs_isStageMiddleBoss()) {
        if ((fopAcM_GetRoomNo(a_this) != 4 || !(player->current.pos.z < 2500.0f)) && checkDoorDemo()) {
            i_this->mActionID = 100;
            i_this->mMode = 0;
            i_this->field_0x904 = 0;
            i_this->field_0xaec = 1;
            a_this->current.angle.x = 0;
            if (fopAcM_CheckCondition(a_this, 4) != 0) {
                camera_class* camera = dComIfGp_getCamera(0);
                if (checkDoorDemo()) {
                    cMtx_YrotS(*calc_mtx, player->shape_angle.y + 0x8000);
                } else {
                    sp30.x = camera->lookat.eye.x - camera->lookat.center.x;
                    sp30.z = camera->lookat.eye.z - camera->lookat.center.z;
                    cMtx_YrotS(*calc_mtx, cM_atan2s(sp30.x, sp30.z));
                }

                if ((i_this->field_0x5b6 & 1) != 0) {
                    sp30.x = 100.0f;
                } else {
                    sp30.x = -100.0f;
                }
                sp30.y = -50.0f;
                sp30.z = 100.0f;
                MtxPosition(&sp30, &sp3c);
                a_this->current.pos = camera->lookat.eye + sp3c;
                a_this->old = a_this->current;
            }
        }
    }
}

static void npc_ks_hang(npc_ks_class* i_this) {
    if (leader == NULL) {
        return;
    }

    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* a_this = &i_this->actor;
    if (fopAcM_GetRoomNo(a_this) == 4) {
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
    actor_p = fopAcM_SearchByID(a_this->parentActorID);
    if (actor_p == NULL)
#else
    if ((actor_p = fopAcM_SearchByID(a_this->parentActorID)) == NULL)
#endif
    {
        actor_p = (fopAc_ac_c*) fpcM_Search(s_sw_sub, i_this);
        if (actor_p != NULL) {
            a_this->parentActorID = fopAcM_GetID(actor_p);
            i_this->field_0x934 = (obj_sw_class*) actor_p;
            i_this->field_0x630 = (i_this->field_0x5b6 + 1) * (i_this->field_0x934->field_0xd8c / (i_this->field_0x934->field_0x91c + 1)) - 1;
        } else {
            return;
        }
    }
    obj_sw_class* sw_p = (obj_sw_class*) actor_p;
    sw_p->field_0x900 += 5.0f;

    cXyz sp2c, sp38;
    int iVar1 = 0;

    cLib_addCalcAngleS2(&a_this->current.angle.y, a_this->home.angle.y + 0x4000, 2, 0x800);
    static s16 start_pya = 0;
    if (!daPy_getPlayerActorClass()->checkAutoJumpStart()) {
        start_pya = i_this->field_0x5c8;
    }

    s16 sVar1;
    switch (i_this->mMode) {
        case 0:
            i_this->mTimers[0] = 0;
            i_this->mMode++;
            i_this->field_0x604 = 0;
            // fallthrough
        case 1:
            if (i_this->field_0x5b6 == 0 && leader->field_0xb42 == 0) {
                order_set(sw_p->field_0x91c);
            }

            if (i_this->field_0x5b8 == 0 && i_this->field_0x5d0 != 16 && i_this->field_0x5c4 < 550.0f + YREG_F(9)) {
                anm_init(i_this, 16, 5.0f, 2, 1.0f);
                i_this->mTimers[0] = cM_rndF(50.0f) + 200.0f;
            }

            if (i_this->mTimers[0] == 0) {
                i_this->mTimers[0] = cM_rndF(30.0f) + 50.0f;
                if (cM_rndF(1.0f) < 0.3f) {
                    anm_init(i_this, 17, 5.0f, 2, 1.0f);
                } else if (cM_rndF(1.0f) < 0.3f) {
                    anm_init(i_this, 18, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 19, 5.0f, 2, 1.0f);
                }
            }

            leader->field_0x92c = 0;
            if (i_this->field_0x5b8 == 0 && !dComIfGp_event_runCheck() && i_this->field_0x94e == 0 && i_this->field_0x5c4 < 150.0f + YREG_F(0)) {
                leader->field_0xb42 = 1;
                i_this->field_0x620 = 1;
                i_this->field_0x610 = 0;
                obj_pos = player->current.pos;
                cMtx_YrotS(*calc_mtx, a_this->home.angle.y);
                sp2c.x = 0.0f;
                sp2c.y = -150.0f + WREG_F(0);
                sp2c.z = -400.0f + WREG_F(1);
                MtxPosition(&sp2c, &obj_pos);
                obj_pos += a_this->current.pos;
                i_this->field_0x624 = obj_pos - i_this->field_0x614;
                i_this->field_0x624 *= (0.075f + WREG_F(2));
                i_this->mMode = 4;
                i_this->mTimers[0] = 17 + WREG_S(8);
                i_this->mTimers[1] = 25 + WREG_S(9);
                i_this->field_0x5fa = 0x4000;
                i_this->field_0x60c = 8000.0f;
                i_this->field_0x604 = 0;
                anm_init(i_this, 24, 3.0f, 2, 1.0f);
            }

            sVar1 = start_pya - sw_p->actor.current.angle.y;
            if (sVar1 < 0x4000 && sVar1 > -0x4000) {
                a_this->home.angle.y = sw_p->actor.current.angle.y + 0x8000;
            } else {
                a_this->home.angle.y = sw_p->actor.current.angle.y;
            }
            break;

        case 2:
            i_this->mMode++;
            i_this->mTimers[0] = 10 + BREG_S(6);
            // fallthrough
        case 3:
            i_this->field_0x5fa = 0x4000;
            i_this->field_0x60c = 8000.0f;
            if (i_this->mTimers[0] == s16(8 + BREG_S(7))) {
                fpcM_Search(s_next_get_sub, i_this);
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_THROW, -1);
            }

            if (i_this->mTimers[0] == s16(2 + ZREG_S(6))) {
                i_this->mTimers[2] = 15 + ZREG_S(7);
            }

            if (i_this->mTimers[0] == 0) {
                i_this->mMode++;
                i_this->mTimers[0] = 10 + BREG_S(8);
                i_this->mTimers[1] = 18 + BREG_S(4);
            }
            break;

        case 4:
            if (i_this->mTimers[0] > s16(8 + BREG_S(5))) {
                i_this->field_0x5fa = 0x4000;
                i_this->field_0x60c = 8000.0f;
            }

            if (i_this->mTimers[0] == s16(8 + BREG_S(5))) {
                anm_init(i_this, 48, 2.0f, 2, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_CATCH, -1);
                i_this->mSound.startCreatureSound(Z2SE_FN_ROPE_CREAK, 0, -1);
                sw_p->field_0x8fc = 20.0f + XREG_F(0);
                sw_p->field_0xd10[i_this->field_0x630] = 10;
            }

            if (mDoCPd_c::getTrigA(0) != 0) {
                if (i_this->mTimers[1] <= 9 && i_this->field_0x604 < 0 && i_this->field_0x602 < 0) {
                    leader->field_0x92c = 1;
                    fpcM_Search(s_next_do_sub, i_this);
                    if (i_this->field_0x5b8 == sw_p->field_0x91c -1) {
                        i_this->mMode = 5;
                    } else {
                        i_this->mMode = 10;
                        anm_init(i_this, 24, 1.0f, 2, 1.0f);
                    }
                } else {
                    leader->field_0xb42 = 100;
                    i_this->field_0x620 = 0;
                    iVar1 = 1;
                    i_this->field_0x94e = 20;
                }
            }
            break;

        case 5:
            if (i_this->field_0x602 >= 0 || i_this->field_0x604 >= 0) break;

            i_this->field_0x620 = 0;
            i_this->mTimers[0] = 15 + BREG_S(3);
            i_this->mMode = 6;
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_THROW, -1);
            player->changeDemoMode(24, 0, 0, 0);
            anm_init(i_this, 24, 1.0f, 2, 1.0f);
            // fallthrough
        case 6:
            if (i_this->mTimers[0] == 8) {
                leader->field_0xb42 = 3;
                leader->field_0xb44 = 0;
            }

            if (i_this->mTimers[0] == 1) {
                iVar1 = 1;
            }
            break;

        case 10:
            break;
    }

    a_this->current.pos = sw_p->field_0x920[i_this->field_0x630];
    s16 sVar2 = i_this->field_0x602;
    cLib_addCalcAngleS2(&i_this->field_0x602, i_this->field_0x60c * cM_ssin(i_this->field_0x5fa), 4, 0x1000);
    i_this->field_0x604 = i_this->field_0x602 - sVar2;
    i_this->field_0x5fa += (s16) 0x800;
    a_this->current.angle.z = -(i_this->field_0x602 / 4);

    if (i_this->mMode != 4) {
        cLib_addCalc0(&i_this->field_0x60c, 0.5f, 100.0f + TREG_F(3));
    }

    hang_end_check(i_this);
    if (iVar1 != 0) {
        i_this->field_0x620 = 0;
        i_this->mMode = 0;
        all_carry_finish(sw_p->field_0x91c);
    }
}

static void npc_ks_hang_s(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* base_sw_p;
#if VERSION == VERSION_SHIELD_DEBUG
    // FIXME: fakematch?
    base_sw_p = fopAcM_SearchByID(a_this->parentActorID);
    if (base_sw_p == NULL)
#else
    if ((base_sw_p = fopAcM_SearchByID(a_this->parentActorID)) == NULL)
#endif
    {
        base_sw_p = (fopAc_ac_c*) fpcM_Search(s_sw_sub, i_this);
        if (base_sw_p != NULL) {
            a_this->parentActorID = fopAcM_GetID(base_sw_p);
            i_this->field_0x934 = (obj_sw_class*) base_sw_p;
        } else {
            return;
        }
    }

    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    obj_sw_class* sw_p = (obj_sw_class*) base_sw_p;
    sw_p->field_0x900 += 5.0f;
    cXyz sp34, sp40;
    cLib_addCalcAngleS2(&a_this->current.angle.y, a_this->home.angle.y + 0x4000, 2, 0x800);

    s16 sVar1;
    switch (i_this->mMode) {
        case 0:
            int asdf;
            i_this->mMode++;
            i_this->mSound.startCreatureSound(Z2SE_FN_ROPE_CREAK, 0, -1);
            sw_p->field_0x8fc = 20.0f + XREG_F(0);
            sw_p->field_0xd10[i_this->field_0x630] = 10;
            i_this->field_0x92c = 0;
            break;

        case 1:
            if (i_this->field_0x5d0 != 16 && i_this->field_0x5c4 < 550.0f + YREG_F(9)) {
                anm_init(i_this, 16, 5.0f, 2, 1.0f);
                i_this->mTimers[0] = cM_rndF(50.0f) + 200.0f;
            }

            if (i_this->mTimers[0] == 0) {
                i_this->mTimers[0] = cM_rndF(30.0f) + 50.0f;
                if (cM_rndF(1.0f) < 0.3f) {
                    anm_init(i_this, 17, 5.0f, 2, 1.0f);
                } else if (cM_rndF(1.0f) < 0.3f) {
                    anm_init(i_this, 18, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 19, 5.0f, 2, 1.0f);
                }
            }

            if (i_this->field_0x94e == 0 && !dComIfGp_event_runCheck() && i_this->field_0x5c4 < 300.0f + YREG_F(10)) {
                leader->field_0xb42 = 1;
                i_this->field_0x620 = 1;
                i_this->field_0x610 = 0;
                cMtx_YrotS(*calc_mtx, a_this->home.angle.y);
                sp34.x = 0.0f;
                sp34.y = -150.0f + WREG_F(0);
                sp34.z = -400.0f + WREG_F(1);
                MtxPosition(&sp34, &obj_pos);
                obj_pos += a_this->current.pos;
                i_this->field_0x624 = obj_pos - i_this->field_0x614;
                i_this->field_0x624 *= (0.075f + WREG_F(2));

                i_this->mMode = 2;
                i_this->mTimers[0] = 17 + WREG_S(8);
                i_this->mTimers[1] = 25 + WREG_S(9);
                i_this->field_0x5fa = 0x4000;
                i_this->field_0x60c = 8000.0f;
                anm_init(i_this, 24, 3.0f, 2, 1.0f);
            }

            sVar1 = i_this->field_0x5c8 - sw_p->actor.current.angle.y;
            if (sVar1 < 0x4000 && sVar1 > -0x4000) {
                a_this->home.angle.y = sw_p->actor.current.angle.y + 0x8000;
            } else {
                a_this->home.angle.y = sw_p->actor.current.angle.y;
            }
            break;

        case 2:
            if (i_this->mTimers[0] > s16(8 + BREG_S(5))) {
                i_this->field_0x5fa = 0x4000;
                i_this->field_0x60c = 8000.0f;
            }

            if (i_this->mTimers[0] == s16(8 + BREG_S(5))) {
                anm_init(i_this, 48, 2.0f, 2, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_CATCH, -1);
                i_this->mSound.startCreatureSound(Z2SE_FN_ROPE_CREAK, 0, -1);
                sw_p->field_0x8fc = 20.0f + XREG_F(0);
                sw_p->field_0xd10[i_this->field_0x630] = 10;
            }

            if (mDoCPd_c::getTrigA(0) != 0) {
                if (i_this->field_0x604 < 0 && i_this->field_0x602 < 0) {
                    i_this->field_0x92c = 1;
                    i_this->field_0x620 = 0;
                    i_this->mTimers[0] = 15 + BREG_S(3);
                    i_this->mMode = 5;
                    i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_THROW, -1);
                    player->changeDemoMode(24, 0, 0, 0);
                    anm_init(i_this, 24, 1.0f, 2, 1.0f);
                    leader->field_0xb42 = 3;
                } else {
                    leader->field_0xb42 = 100;
                    i_this->field_0x620 = 0;
                    i_this->mTimers[0] = 15 + BREG_S(3);
                    i_this->mMode = 5;
                    i_this->field_0x94e = 20;
                }
            }
            break;

        case 5:
            if (i_this->mTimers[0] == 1) {
                leader->field_0xb42 = 100;
                i_this->field_0x620 = 0;
                i_this->mMode = 0;
            }
    }

    a_this->current.pos = sw_p->field_0x920[i_this->field_0x630];
    s16 sVar2 = i_this->field_0x602;
    cLib_addCalcAngleS2(&i_this->field_0x602, i_this->field_0x60c * cM_ssin(i_this->field_0x5fa), 4, 0x1000);
    i_this->field_0x604 = i_this->field_0x602 - sVar2;
    i_this->field_0x5fa += NPC_KS_18DEG_ROT;
    a_this->current.angle.z = -(i_this->field_0x602 / 4);

    if (i_this->field_0x620 != 2) {
        cLib_addCalc0(&i_this->field_0x60c, 0.5f, 100.0f + TREG_F(3));
    }

    hang_end_check(i_this);
}

static void npc_ks_e_hang(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* base_sw_p = fopAcM_SearchByID(a_this->parentActorID);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    obj_sw_class* sw_p = (obj_sw_class*) base_sw_p;

    sw_p->field_0x900 += 5.0f;
    cXyz unused_cxyz;
    int unused_sp10 = 0;
    cLib_addCalcAngleS2(&a_this->current.angle.y, a_this->home.angle.y + 0x4000, 2, 0x800);

    switch (i_this->mMode) {
        case 0:
            i_this->mMode = 20;
            i_this->mSound.startCreatureSound(Z2SE_FN_ROPE_CREAK, 0, -1);
            sw_p->field_0x8fc = 20.0f + XREG_F(0);
            sw_p->field_0xd10[i_this->field_0x630] = 10;
            a_this->home.angle.y = sw_p->actor.current.angle.y;
            i_this->field_0x5fa = -0x4000;
            i_this->field_0x60c = 4000.0f;

            if (i_this->field_0x5b6 == 0) {
                anm_init(i_this, 24, 3.0f, 2, 1.0f);
                i_this->mTimers[0] = 10000;
                i_this->mTimers[1] = 15;
            }
            break;

        case 2:
            anm_init(i_this, 48, 2.0f, 2, 1.0f);
            i_this->mSound.startCreatureSound(Z2SE_FN_ROPE_CREAK, 0, -1);
            sw_p->field_0x8fc = 20.0f + XREG_F(0);
            sw_p->field_0xd10[i_this->field_0x630] = 10;
            i_this->field_0x5fa = -0x8000;
            i_this->field_0x60c = 8000.0f;
            i_this->mMode++;
            break;

        case 3:
            if (i_this->field_0x5fa == s16(YREG_S(7) - 0x3800)) {
                a_this->health = 10;
                i_this->mMode = 20;
                i_this->mTimers[0] = 0;
            }
            break;

        case 20:
            if (i_this->mTimers[0] == 0) {
                if (cM_rndF(1.0f) < 0.3f) {
                    anm_init(i_this, 17, 10.0f, 2, 1.0f);
                } else if (cM_rndF(1.0f) < 0.3f) {
                    anm_init(i_this, 18, 10.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 19, 10.0f, 2, 1.0f);
                }

                i_this->mTimers[0] = 10000;
                i_this->mTimers[1] = 35;
            }

            if (i_this->mTimers[1] == 1) {
                anm_init(i_this, 24, 7.0f, 2, 1.0f);
            } else if (i_this->mTimers[1] == 0) {
                i_this->field_0x5fa = 0x4000;
                i_this->field_0x60c = 8000.0f;
            }
    }

    a_this->current.pos = sw_p->field_0x920[i_this->field_0x630];
    cLib_addCalcAngleS2(&i_this->field_0x602, i_this->field_0x60c * cM_ssin(i_this->field_0x5fa), 4, 0x1000);
    i_this->field_0x5fa += NPC_KS_18DEG_ROT;
    a_this->current.angle.z = -(i_this->field_0x602 / 4);
    cLib_addCalc0(&i_this->field_0x60c, 0.5f, 100.0f + TREG_F(3));
}

static int all_move_check(int param_1, int param_2) {
    int iVar1 = 0;
    while (iVar1 < param_2) {
        // the following forces dbg stack reg to use r31:
        int unused;
        if (saru_p[iVar1] == NULL) return 0;

        if (param_1 != saru_p[iVar1]->field_0x5b6 && (saru_p[iVar1]->mActionID != 21 || saru_p[iVar1]->mMode != 20)) {
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
        if (saru_p[reg_r30]->mActionID != 21 || saru_p[reg_r30]->mTimers[2] != 0) {
            return 0;
        }

        reg_r30--;
    }

    return 1;
}

static int npc_ks_e_jump(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp28, unused_cxyz;
    obj_sw_class* sw_p2 = i_this->field_0x934;
    npc_ks_class* ks_p = (npc_ks_class*)i_this->field_0x930;
    int rv = 2;
    switch(i_this->mMode) {
        case 0:
        i_this->field_0xaec = 1;
        if (i_this->field_0x5c4 > l_HIO.pull_distance) {
            anm_init(i_this, 28, 5.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->mTimers[0] = 20;
        } else {
            anm_init(i_this, 51, 5.0f, 2, 1.0f);
            i_this->mMode = 5;
            a_this->speedF = 0.0f;
        }

        if (fopAcM_GetRoomNo(a_this) == 4) {
            a_this->current.pos.y = 3300.0f;
            if (a_this->current.pos.x > 180.0f) {
                a_this->current.pos.x = 180.0f;
            } else if (a_this->current.pos.x < -180.0f) {
                a_this->current.pos.x = -180.0f;
            }
        }
        break;

    case 1:
        a_this->speedF = l_HIO.holding_speed_h;
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5c8, 1, 0x2000);
        rv = 1;
        if (i_this->mTimers[0] == 0 || i_this->field_0x5c4 < l_HIO.pull_distance - 20.0f) {
            anm_init(i_this, 51, 5.0f, 2, 1.0f);
            i_this->mMode = 5;
            a_this->speedF = 0.0f;
        }
        break;

    case 5:
        i_this->field_0x5fd = 1;
        sp28 = ks_p->field_0x614 - a_this->current.pos;
        i_this->field_0x8fc.y = cM_atan2s(sp28.x, sp28.z);
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 1, 0x2000);

        if (go_jump_check(i_this->field_0x5b6) != 0) {
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mTimers[0] = 10;
            i_this->mMode = 6;
            i_this->field_0xaec = 0;
        }
        break;

    case 6:
        i_this->field_0x5fd = 1;
        a_this->speedF = 20.0f;
        rv = 1;
        if (i_this->mTimers[0] == 0) {
            anm_init(i_this, 32, 2.0f, 0, 1.0f);
            i_this->mMode = 7;
        }
        break;

    case 7:
        // bVar1 = true;
        i_this->field_0x5fd = 1;
        if (i_this->field_0x5d0 == 32) {
            a_this->speedF = 0.0f;
            if (i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                a_this->speedF = 40.0f;
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);

                i_this->field_0x910 = a_this->current.pos;
                i_this->field_0xbe0 = 1;
            }
        }

        i_this->field_0x8f0 = ks_p->field_0x614;
        i_this->field_0x91c = i_this->field_0x8f0;
        sp28 = i_this->field_0x8f0 - a_this->current.pos;
        i_this->field_0x8fc.y = cM_atan2s(sp28.x, sp28.z);
        i_this->field_0x8fc.x = -cM_atan2s(sp28.y, JMAFastSqrt(sp28.x * sp28.x + sp28.z * sp28.z));
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 1, 0x2000);

        if (a_this->speedF >= 40.0f) {
            i_this->field_0x90c = 1;
            cLib_addCalc2(&i_this->field_0x928, -183.0f + XREG_F(11), 1.0f, 15.0f + XREG_F(12));
            cLib_addCalcAngleS2(&a_this->current.angle.x, 0x1800, 4, 0x1800);
        }

        if (sp28.abs() <= a_this->speedF * 1.2f) {
            a_this->speedF = 0.0f;
            i_this->mMode = 10;
            anm_init(i_this, 48, 2.0f, 2, 1.0f);
            ks_p->mMode = 2;
        }
        break;

    case 10:
        a_this->home.angle.y = sw_p2->actor.current.angle.y;
        a_this->current.angle.y = a_this->home.angle.y;
        a_this->current.pos = ks_p->field_0x614;
        a_this->current.angle.x = (s16)((int)ks_p->field_0x602 << 1);
        if (ks_p->actor.health != 10) break;
        ks_p->actor.health = 0;
        i_this->field_0x910 = a_this->current.pos;
        i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_THROW, -1);

        if (fopAcM_GetRoomNo(a_this) == 2) {
            a_this->speedF = 25.0f + NREG_F(16);
        } else {
            a_this->speedF = 40.0f + NREG_F(15);
        }

        if (ks_p->field_0x5b6 == i_this->field_0x5b6 - 1) {
            i_this->mMode = 11;
        } else {
            i_this->mMode = 20;
            i_this->field_0x930 = saru_p[ks_p->field_0x5b6 + 1];
        }

        anm_init(i_this, 33, 1.0f, 0, 1.0f);
        i_this->field_0xbe0 = 1;
        // fallthrough
    case 11:
        i_this->field_0x90c = 1;
        i_this->field_0x630 = (i_this->field_0x5b6 + 1) * (sw_p2->field_0xd8c / (sw_p2->field_0x91c + 1)) - 1;
        i_this->field_0x8f0 = sw_p2->field_0x920[i_this->field_0x630];

        a_this->parentActorID = fopAcM_GetID(sw_p2);
        i_this->field_0x91c = i_this->field_0x8f0;
        sp28 = i_this->field_0x8f0 - a_this->current.pos;
        i_this->field_0x8fc.y = cM_atan2s(sp28.x, sp28.z);
        i_this->field_0x8fc.x = -cM_atan2s(sp28.y, JMAFastSqrt(sp28.x * sp28.x + sp28.z * sp28.z));

        if (sp28.abs() <= a_this->speedF * 1.2f) {
            a_this->speedF = 0.0f;
            i_this->mMode = 12;
            anm_init(i_this, 24, 2.0f, 2, 1.0f);
            i_this->mSound.startCreatureSound(Z2SE_FN_ROPE_CREAK, 0, -1);
            sw_p2->field_0x8fc = 20.0f + XREG_F(0);
            sw_p2->field_0xd10[i_this->field_0x630] = 10;
        }
        // fallthrough
    case 12:
        cLib_addCalc0(&i_this->field_0x928, 1.0f, 15.0f + NREG_F(17));
        cLib_addCalcAngleS2(&a_this->current.angle.x, -0x8000, 1, 0x700 + NREG_S(7));
        if (i_this->mMode == 12 && i_this->field_0x928 == 0.0f) {
            if (all_move_check(i_this->field_0x5b6, sw_p2->field_0x91c) != 0) {
                all_carry_finish(sw_p2->field_0x91c);
            } else {
                i_this->mActionID = 21;
                i_this->mMode = 0;
                i_this->mTimers[2] = 15;
            }
        }
        break;

    case 20:
        i_this->field_0x90c = 1;
        cLib_addCalcAngleS2(&a_this->current.angle.x, 0x1800, 4, 0x1800);
        i_this->field_0x8f0 = ks_p->field_0x614;
        i_this->field_0x91c = i_this->field_0x8f0;
        sp28 = i_this->field_0x8f0 - a_this->current.pos;
        i_this->field_0x8fc.y = cM_atan2s(sp28.x, sp28.z);
        i_this->field_0x8fc.x = -cM_atan2s(sp28.y, JMAFastSqrt(sp28.x * sp28.x + sp28.z * sp28.z));

        if (sp28.abs() <= a_this->speedF * 1.2f) {
            a_this->speedF = 0.0f;
            i_this->mMode = 10;
            anm_init(i_this, 48, 2.0f, 2, 1.0f);
            ks_p->mMode = 2;
        }
    }

    if (i_this->mMode != 11 && i_this->mMode != 12) {
        i_this->field_0x608 = (s16)((int)a_this->current.angle.x << 1);
    }

    return rv;
}

static npc_ks_class* get_move_saru(npc_ks_class* i_this, int param_2) {
    fopAc_ac_c* a_this = &i_this->actor;
    if (fopAcM_GetRoomNo(a_this) == 0) {
        for (int i = 0; i < 2; i++) {
            if (saru_p[i] != NULL && (saru_p[i]->mActionID == 20 || saru_p[i]->mActionID == 22) && i_this->field_0xb40 == saru_p[i]->field_0x934->field_0x570) {
                return saru_p[i];
            }
        }
    } else {
        for (int i = 0; i < param_2; i++) {
            if (i_this->field_0xb40 == saru_p[i]->field_0x934->field_0x570 && ((saru_p[i]->mActionID == 20 || saru_p[i]->mActionID == 22) || saru_p[i]->mTimers[2] != 0)) {
                return saru_p[i];
            }
        }
    }

    return NULL;
}

static int all_hang_check(npc_ks_class* i_this, int param_2) {
    fopAc_ac_c* a_this = &i_this->actor;
    if (fopAcM_GetRoomNo(a_this) == 0) {
        for (int i = 0; i < 2; i++) {
            if (saru_p[i] != NULL && saru_p[i]->field_0x5b5 != 0 && i_this->field_0xb40 == saru_p[i]->field_0x934->field_0x570) {
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
    cLib_addCalc2(&i_this->field_0xb60.x, i_this->field_0xb6c.x, param_2, i_this->field_0xb84.x * i_this->field_0xbc4);
    cLib_addCalc2(&i_this->field_0xb60.y, i_this->field_0xb6c.y, param_2, i_this->field_0xb84.y * i_this->field_0xbc4);
    cLib_addCalc2(&i_this->field_0xb60.z, i_this->field_0xb6c.z, param_2, i_this->field_0xb84.z * i_this->field_0xbc4);
    cLib_addCalc2(&i_this->field_0xb48.x, i_this->field_0xb54.x, param_2, i_this->field_0xb78.x * i_this->field_0xbc4);
    cLib_addCalc2(&i_this->field_0xb48.y, i_this->field_0xb54.y, param_2, i_this->field_0xb78.y * i_this->field_0xbc4);
    cLib_addCalc2(&i_this->field_0xb48.z, i_this->field_0xb54.z, param_2, i_this->field_0xb78.z * i_this->field_0xbc4);
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
    fopAc_ac_c* a_this = &i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* unused_cam_p = dComIfGp_getCamera(0);
    obj_sw_class* sw_p = i_this->field_0x934;
    fopAc_ac_c* base_cage_p = &sw_p->actor;
    cXyz sp44, sp50;
    f32 fVar1, fVar2;
    npc_ks_class* npc_ks_p;
    obj_brg_class* brg_p;
    daMidna_c* midna_p;

    switch (i_this->field_0xb42) {
        case 0:
            break;

        case 1:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->field_0xb42 = 2;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0xb44 = 0;
            player->changeOriginalDemo();
            player->changeDemoMode(19, 0, 0, 0);
            player->changeDemoParam0(0);
            i_this->field_0xb60 = player->current.pos;
            i_this->field_0xba8.z = 500.0f + AREG_F(0);
            i_this->field_0xba8.x = 500.0f + AREG_F(1);
            i_this->field_0xba8.y = 0.0f + AREG_F(2);
            // fallthrough
        case 2:
            i_this->field_0xbb8 = 55.0f;
            cLib_addCalc2(&i_this->field_0xb60.x, player->current.pos.x, 0.2f, 50.0f + AREG_F(16));
            cLib_addCalc2(&i_this->field_0xb60.y, base_cage_p->current.pos.y + AREG_F(4) - 250.0f, 0.2f, 50.0f + AREG_F(16));
            cLib_addCalc2(&i_this->field_0xb60.z, player->current.pos.z, 0.2f, 50.0f + AREG_F(16));
            if (fopAcM_GetRoomNo(a_this) == 2) {
                cMtx_YrotS(*calc_mtx, base_cage_p->current.angle.y);
            } else {
                cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            }

            sp44.x = i_this->field_0xba8.x;
            sp44.y = i_this->field_0xba8.y;
            sp44.z = i_this->field_0xba8.z;
            MtxPosition(&sp44,&sp50);

            sp50.x = sp50.x + player->current.pos.x;
            sp50.y = sp50.y + (base_cage_p->current.pos.y + AREG_F(5) - 250.0f);
            sp50.z = sp50.z + player->current.pos.z;
            if (i_this->field_0xb44 == 0) {
                i_this->field_0xb48 = sp50;
            } else {
                cLib_addCalc2(&i_this->field_0xb48.x, sp50.x, 0.1f, 200.0f);
                cLib_addCalc2(&i_this->field_0xb48.y, sp50.y, 0.1f, 200.0f);
                cLib_addCalc2(&i_this->field_0xb48.z, sp50.z, 0.1f, 200.0f);
            }

            if (i_this->field_0x92c != 0) {
                i_this->field_0xba8.z += AREG_F(3) - 10.0f;
            }
            break;

        case 3:
            cLib_addCalc2(&i_this->field_0xb60.x, player->current.pos.x, 0.4f, 100.0f);
            cLib_addCalc2(&i_this->field_0xb60.y, player->current.pos.y + BREG_F(19) + 70.0f, 0.4f, 100.0f );
            cLib_addCalc2(&i_this->field_0xb60.z, player->current.pos.z, 0.4f, 100.0f);
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            fVar1 = 50.0f;
            if (fopAcM_GetRoomNo(a_this) == 4 && i_this->field_0xbdb == 0) {
                sp44.x = 100.0f + BREG_F(10);
                sp44.y = 100.0f + BREG_F(11);
                sp44.z = 300.0f + BREG_F(12);

                if (i_this->field_0xb44 == 10 + NREG_S(6)) {
                    player->changeDemoMode(40, 0, 0, 0);
                }

                if (i_this->field_0xb44 == 28 + NREG_S(7)) {
                    player->changeDemoMode(41, 0, 0, 0);
                }

                if (i_this->field_0xb44 == 40 + NREG_S(8)) {
                    i_this->field_0xb42 = 100;
                    i_this->field_0xbdb = 1;
                }

                fVar1 = 200.0f;
            } else {
                sp44.x = 0.0f;
                sp44.y = 0.0f;
                sp44.z = -400.0f;

                if (i_this->field_0xb44 == 8) {
                    i_this->field_0xb42 = 100;
                }
            }

            MtxPosition(&sp44, &sp50);
            sp50.x = sp50.x + player->current.pos.x;
            sp50.y = sp50.y + player->current.pos.y;
            sp50.z = sp50.z + player->current.pos.z;
            cLib_addCalc2(&i_this->field_0xb48.x, sp50.x, 0.2f, fVar1);
            cLib_addCalc2(&i_this->field_0xb48.y, sp50.y, 0.2f, fVar1);
            cLib_addCalc2(&i_this->field_0xb48.z, sp50.z, 0.2f, fVar1);
            break;

        case 10:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->field_0xb42 = 11;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0xb44 = 0;
            i_this->field_0xbb8 = 55.0f;
            i_this->field_0xba8.z = 500.0f + AREG_F(0);
            i_this->field_0xba8.x = 500.0f + AREG_F(1);
            i_this->field_0xba8.y = AREG_F(2);
            i_this->field_0xbc0 = NULL;
            // fallthrough
        case 11:
            sp50 = i_this->field_0xbc8;
            sp50.y = player->current.pos.y;
            setPlayerPosAndAngle(&sp50, player->shape_angle.y);
            npc_ks_p = i_this->field_0xbc0;
            i_this->field_0xbc0 = get_move_saru(i_this,sw_p->field_0x91c);;
            i_this->field_0xb6c.y = base_cage_p->current.pos.y + AREG_F(4) - 200.0f;
            if (i_this->field_0xbc0 != NULL) {
                base_cage_p = (fopAc_ac_c*) i_this->field_0xbc0->field_0x934;
                cMtx_YrotS(*calc_mtx, i_this->field_0xbd4);
                sp44.x = i_this->field_0xba8.x;
                sp44.y = i_this->field_0xba8.y;
                sp44.z = i_this->field_0xba8.z;
                MtxPosition(&sp44, &i_this->field_0xb54);
                i_this->field_0xb54.x += i_this->field_0xbc0->actor.current.pos.x;
                i_this->field_0xb54.y += base_cage_p->current.pos.y + AREG_F(5) - 250.0f;
                i_this->field_0xb54.z += i_this->field_0xbc0->actor.current.pos.z;
                i_this->field_0xb6c.x = i_this->field_0xbc0->actor.current.pos.x;
                i_this->field_0xb6c.y = base_cage_p->current.pos.y + AREG_F(4) - 200.0f;

                if (i_this->field_0xbc0->mActionID == 20 && i_this->field_0xb6c.y < i_this->field_0xbc0->actor.current.pos.y + AREG_F(8)) {
                    i_this->field_0xb6c.y = i_this->field_0xbc0->actor.current.pos.y + AREG_F(8);
                }

                i_this->field_0xb6c.z = i_this->field_0xbc0->actor.current.pos.z;
                if (npc_ks_p != i_this->field_0xbc0) {
                    i_this->field_0xb48 = i_this->field_0xb54;
                    i_this->field_0xb60 = i_this->field_0xb6c;
                }
                i_this->field_0xb84.set(200.0f, 200.0f, 200.0f);
                i_this->field_0xbc4 = 1.0f;
            } else {
                if (all_hang_check(i_this, sw_p->field_0x91c) != 0) {
                    i_this->field_0xb42 = 12;
                    if (fopAcM_GetRoomNo(a_this) == 0) {
                        i_this->field_0xb44 = 45;
                    } else if (sw_p->field_0x91c >= 3) {
                        i_this->field_0xb6c.x = saru_p[sw_p->field_0x91c - 2]->actor.current.pos.x;
                        i_this->field_0xb6c.y = saru_p[sw_p->field_0x91c - 2]->actor.current.pos.y;
                        i_this->field_0xb6c.y = i_this->field_0xb6c.y - 150.0f;
                        i_this->field_0xb6c.z = saru_p[sw_p->field_0x91c - 2]->actor.current.pos.z;
                        
                        i_this->field_0xb84.x = NPC_KS_FABSF(i_this->field_0xb6c.x - i_this->field_0xb60.x);
                        i_this->field_0xb84.y = NPC_KS_FABSF(i_this->field_0xb6c.y - i_this->field_0xb60.y);
                        i_this->field_0xb84.z = NPC_KS_FABSF(i_this->field_0xb6c.z - i_this->field_0xb60.z);
                        i_this->field_0xbc4 = 0.0f;
                        i_this->field_0xb44 = 5;
                    } else {
                        i_this->field_0xb44 = 45 + AREG_S(4);
                    }
                }
            }
            // fallthrough
        case 12:
            cLib_addCalc2(&i_this->field_0xb48.x, i_this->field_0xb54.x, 0.2f, i_this->field_0xbc4 * 200.0f);
            cLib_addCalc2(&i_this->field_0xb48.y, i_this->field_0xb54.y, 0.2f, i_this->field_0xbc4 * 200.0f);
            cLib_addCalc2(&i_this->field_0xb48.z, i_this->field_0xb54.z, 0.2f, i_this->field_0xbc4 * 200.0f);
            cLib_addCalc2(&i_this->field_0xb60.x, i_this->field_0xb6c.x, 0.2f, i_this->field_0xb84.x * i_this->field_0xbc4);
            cLib_addCalc2(&i_this->field_0xb60.y, i_this->field_0xb6c.y, 0.2f, i_this->field_0xb84.y * i_this->field_0xbc4);
            cLib_addCalc2(&i_this->field_0xb60.z, i_this->field_0xb6c.z, 0.2f, i_this->field_0xb84.z * i_this->field_0xbc4);
            if (i_this->field_0xb42 == 12) {
                cLib_addCalc2(&i_this->field_0xbb8, 40.0f, 0.2f, i_this->field_0xbc4);
                cLib_addCalc2(&i_this->field_0xbc4, 0.1f, 1.0f, 0.005f);
                if (85 < i_this->field_0xb44) {
                    i_this->field_0xb42 = 100;
                    sp44.x = saru_p[i_this->field_0xb40]->actor.current.pos.x - i_this->field_0xbc8.x;
                    sp44.z = saru_p[i_this->field_0xb40]->actor.current.pos.z - i_this->field_0xbc8.z;
                    s16 angle = cM_atan2s(sp44.x, sp44.z);
                    cMtx_YrotS(*calc_mtx, angle);
                    sp44.x = 0.0f;
                    sp44.y = 0.0f;
                    if (fopAcM_GetRoomNo(a_this) == 4) {
                        sp44.z = -250.0f + JREG_F(0);
                    } else {
                        sp44.z = -150.0f + JREG_F(0);
                    }
                    MtxPosition(&sp44,&sp50);
                    sp50.x = sp50.x + i_this->field_0xbc8.x;
                    sp50.z = sp50.z + i_this->field_0xbc8.z;

                    if (fopAcM_GetRoomNo(a_this)  == 2) {
                        sp50.y = 3764.0f;
                    } else {
                        sp50.y = player->current.pos.y;
                    }
                    setPlayerPosAndAngle(&sp50, angle);
                    sp44.x = 0.0f;
                    sp44.y = 100.0f + JREG_F(1);
                    sp44.z = -250.0f + JREG_F(2);
                    MtxPosition(&sp44, &i_this->field_0xb48);
                    i_this->field_0xb48 += player->current.pos;
                    i_this->field_0xb60 = player->current.pos;
                    i_this->field_0xb60.y += 120.0f;
                    i_this->field_0xbb8 = 55.0f;
                }
            }
            break;

        case 60:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->field_0xb42 = 61;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0xb44 = 0;
            cMtx_YrotS(*calc_mtx, s16(a_this->shape_angle.y));
            sp44.x = 300.0f + WREG_F(11);
            sp44.y = 100.0f + WREG_F(12);
            sp44.z = WREG_F(13);
            MtxPosition(&sp44, &i_this->field_0xb48);
            i_this->field_0xb48 += a_this->current.pos;

            sp44.x = i_this->field_0xb48.x - player->current.pos.x;
            sp44.z = i_this->field_0xb48.z - player->current.pos.z;
            if (sp44.x * sp44.x + sp44.z * sp44.z < 10000.0f) {
                OS_REPORT("////////KS PL REVISED \n");
                i_this->field_0xb48.x += + 200.0f + JREG_F(7);
                i_this->field_0xb48.z += JREG_F(8);
            }

            i_this->field_0xb60 = a_this->eyePos;
            i_this->field_0xb60.y += WREG_F(10);
            player->changeOriginalDemo();
            player->changeDemoMode(1, 1, 0, 0);
            // fallthrough
        case 61:
            sp50 = player->old.pos;
            sp50.y = 3150.0f;
            setPlayerPosAndAngle(&sp50, player->shape_angle.y);
            i_this->field_0xbb8 = 65.0f + WREG_F(8);
            cLib_addCalc2(&i_this->field_0xb60.x, a_this->eyePos.x, 0.1f + WREG_F(9), 50.0f);
            cLib_addCalc2(&i_this->field_0xb60.y, a_this->eyePos.y + WREG_F(10), 0.1f + WREG_F(9), 50.0f);
            cLib_addCalc2(&i_this->field_0xb60.z, a_this->eyePos.z, 0.1f + WREG_F(9), 50.0f);
            break;

        case 70:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->field_0xb42 = 71;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0xb44 = 0;
            i_this->field_0xbb8 = 55.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(1, 1, 0, 0);
            // fallthrough
        case 71:
            if (i_this->field_0xb44 == 0) {
                i_this->field_0xb60 = a_this->current.pos;
                i_this->field_0xb60.y = a_this->current.pos.y + 120.0f + VREG_F(6);
                i_this->field_0xb48.set(-4786.0f, 3982.0f, -1843.0f);
            } else if (i_this->field_0xb44 == 210 + WREG_S(0)) {
                i_this->field_0xb48.set(-3232.0f, 4137.0f, -1425.0f);
            } else if (i_this->field_0xb44 == 240 + WREG_S(1)) {
                i_this->field_0xb48.set(-2459.0f, 4199.0f, -2752.0f);
            } else if (i_this->field_0xb44 == 290 + WREG_S(2)) {
                i_this->field_0xb48.set(103.0f, 4025.0f, -1930.0f);
            }
            cLib_addCalc2(&i_this->field_0xb60.x, a_this->current.pos.x, 0.2f, 60.0f);
            cLib_addCalc2(&i_this->field_0xb60.y, a_this->current.pos.y + 120.0f + VREG_F(6), 0.1f, 30.0f);
            cLib_addCalc2(&i_this->field_0xb60.z, a_this->current.pos.z, 0.2f, 60.0f);
            
            if (i_this->field_0xb44 == 358) {
                i_this->field_0xb42 = 99;
            }
            break;

        case 72:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->field_0xb42 = 73;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0xb44 = 0;
            i_this->field_0xbb8 = 55.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(1, 1, 0, 0);
            // fallthrough
        case 73:
            if (i_this->field_0xb44 == 0) {
                i_this->field_0xb60 = a_this->current.pos;
                i_this->field_0xb60.y = a_this->current.pos.y + 120.0f + VREG_F(6);
                i_this->field_0xb48.set(-112.0f, 2420.0f, 13414.0f);
            }

            sp50.set(145.0f, 2354.0f, 13179.0f);
            setPlayerPosAndAngle(&sp50, -0x691B);
            cLib_addCalc2(&i_this->field_0xb60.x, a_this->current.pos.x, 0.1f, 20.0f);
            cLib_addCalc2(&i_this->field_0xb60.y, a_this->current.pos.y + 120.0f + VREG_F(6), 0.1f, 10.0f);
            cLib_addCalc2(&i_this->field_0xb60.z, a_this->current.pos.z, 0.12f, 20.0f);
            
            if (i_this->field_0xb44 == 280) {
                i_this->field_0xb42 = 100;
            }
            break;

        case 75:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }
            
            i_this->field_0xb42 = 76;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0xb44 = 0;
            i_this->field_0xbb8 = 55.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(1,1,0,0);
            // fallthrough
        case 76:
            setPlayerPosAndAngle(&player->current.pos, i_this->field_0x5c8 + 0x8000);
            if (i_this->field_0xb44 == 0) {
                cMtx_YrotS(*calc_mtx,a_this->shape_angle.y);
                sp44.x = 0.0f;
                sp44.y = 100.0f + JREG_F(1);
                sp44.z = 400.0f + JREG_F(2);
                MtxPosition(&sp44, &i_this->field_0xb48);
                i_this->field_0xb48 += a_this->current.pos;
                i_this->field_0xb60 = a_this->current.pos;
                i_this->field_0xb60.y = a_this->current.pos.y + 120.0f + VREG_F(6);
            }
            cLib_addCalc2(&i_this->field_0xb60.x, a_this->current.pos.x, 0.2f, 60.0f);
            cLib_addCalc2(&i_this->field_0xb60.y, a_this->current.pos.y + 120.0f + VREG_F(6), 0.1f, 30.0f);
            cLib_addCalc2(&i_this->field_0xb60.z, a_this->current.pos.z, 0.2f, 60.0f);
            if (i_this->field_0xb44 == 180) {
                i_this->field_0xb42 = 99;
            }
            break;

        case 77:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->field_0xb42 = 78;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0xb44 = 0;
            i_this->field_0xbb8 = 55.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(1, 1, 0, 0);
            i_this->field_0xbb8 = 65.0f;
            i_this->field_0xb60.set(8486.0f, 4143.0f, -10114.0f);
            i_this->field_0xb48.set(8364.0f, 4325.0f, -9756.0f);
            // fallthrough
        case 78:
            cLib_addCalc2(&i_this->field_0xbb8, 55.0f, 0.05f, 0.2f);
            if (i_this->field_0xb44 != 80) break;
            i_this->field_0xb42 = 79;
            i_this->field_0xb44 = 0;
            // fallthrough
        case 79:
            if (i_this->field_0xb44 == 0) {
                i_this->field_0xb60 = a_this->current.pos;
                i_this->field_0xb60.y = a_this->current.pos.y + 120.0f + VREG_F(6);
                i_this->field_0xb48.set(6899.0f, 3867.0f, -8870.0f);
            } else if (i_this->field_0xb44 == 60) {
                i_this->field_0xb48.set(8222.0f, 3896.0f, -9579.0f);
            }
            cLib_addCalc2(&i_this->field_0xb60.x, a_this->current.pos.x, 0.2f, 60.0f);
            cLib_addCalc2(&i_this->field_0xb60.y, a_this->current.pos.y + 120.0f + VREG_F(6), 0.1f, 30.0f);
            cLib_addCalc2(&i_this->field_0xb60.z, a_this->current.pos.z, 0.2f, 60.0f);
            if (i_this->field_0xb44 >= 130) {
                cLib_addCalc2(&i_this->field_0xbb8, 40.0f, 0.05f, 0.3f);
            }

            if (i_this->field_0xb44 == 200) {
                i_this->field_0xb42 = 100;
                dComIfGs_onSwitch(83, fopAcM_GetRoomNo(a_this));
            }
            break;
        
        case 80:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->field_0xb42 = 81;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0xb44 = 0;
            i_this->field_0xbb8 = 55.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(1, 1, 0, 0);
            // fallthrough
        case 81:
            if (i_this->field_0xb44 == 0) {
                saru_p[3]->actor.current.pos.x = -500.0f;
                saru_p[3]->actor.current.pos.y = 3681.0f;
                saru_p[3]->actor.current.pos.z = 7485.0f;
            }

            if (i_this->field_0xb44 == 30) {
                saru_p[0]->actor.current.pos.x = player->current.pos.x;
                saru_p[0]->actor.current.pos.y = player->current.pos.y + 500.0f;
                saru_p[0]->actor.current.pos.z = player->current.pos.z - 100.0f;
            }

            if (i_this->field_0xb44 == 60) {
                saru_p[1]->actor.current.pos.x = player->current.pos.x + 200.0f;
                saru_p[1]->actor.current.pos.y = player->current.pos.y + 500.0f;
                saru_p[1]->actor.current.pos.z = player->current.pos.z;
            }

            if (i_this->field_0xb44 == 90) {
                saru_p[2]->actor.current.pos.x = i_this->field_0xb48.x - 250.0f;
                saru_p[2]->actor.current.pos.y = i_this->field_0xb48.y + 200.0f;
                saru_p[2]->actor.current.pos.z = i_this->field_0xb48.z - 100.0f;
            }

            if (i_this->field_0xb44 < 80) {
                i_this->field_0xb60.set(-333.0f,3316.0f,7149.0f);
                i_this->field_0xb48.set(-531.0f,3295.0f,7381.0f);
            } else {
                i_this->field_0xb60.set(162.0f,3354.0f,6881.0f);
                i_this->field_0xb48.set(263.0f,3374.0f,7168.0f);
            }
            
            if (i_this->field_0xb44 == 170) {
                i_this->field_0xb42 = 100;
                i_this->field_0xb60.set(-333.0f,3316.0f,7149.0f);
                i_this->field_0xb48.set(-531.0f,3295.0f,7381.0f);
            }
            break;

        case 110:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->field_0xb42 = 111;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0xb44 = 0;
            i_this->field_0xbb8 = 59.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(3, 0, 0, 0);
            sp50.set(12899.0f, 3767.0f, 3364.0f);
            player->changeDemoPos0(&sp50);
            
            i_this->field_0xb60.set(11821.0f, 3889.0f, 4033.0f);
            i_this->field_0xb48.set(11710.0f, 3936.0f, 4253.0f);
            i_this->field_0xb6c.set(12772.0f, 3940.0f, 3565.0f);
            i_this->field_0xb54.set(13003.0f, 4022.0f, 3613.0f);

            i_this->field_0xb78.x = NPC_KS_FABSF(i_this->field_0xb54.x - i_this->field_0xb48.x);
            i_this->field_0xb78.y = NPC_KS_FABSF(i_this->field_0xb54.y - i_this->field_0xb48.y);
            i_this->field_0xb78.z = NPC_KS_FABSF(i_this->field_0xb54.z - i_this->field_0xb48.z);
            i_this->field_0xb84.x = NPC_KS_FABSF(i_this->field_0xb6c.x - i_this->field_0xb60.x);
            i_this->field_0xb84.y = NPC_KS_FABSF(i_this->field_0xb6c.y - i_this->field_0xb60.y);
            i_this->field_0xb84.z = NPC_KS_FABSF(i_this->field_0xb6c.z - i_this->field_0xb60.z);
            i_this->field_0xbc4 = 0.0f;
            // fallthrough
        case 111:
        case 112:
            if (i_this->field_0xb44 < 10) {
                sp50.set(11497.0f, 3764.0f, 3810.0f);
                setPlayerPosAndAngle(&sp50, 0x4D5A);
            }

            if (i_this->field_0xb44 > 5) {
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
                i_this->field_0xb42 = 112;
                daPy_getPlayerActorClass()->changeDemoMode(25, 0, 0, 0);
                i_this->field_0xb44 = 20;
            }

            if (i_this->field_0xb42 == 112 && i_this->field_0xb44 >= 53 + BREG_S(9)) {
                i_this->field_0xb42 = 100;
                cMtx_YrotS(*calc_mtx, player->shape_angle.y);
                sp44.x = 0.0f;
                sp44.y = 100.0f + JREG_F(1);
                sp44.z = -250.0f + JREG_F(2);
                MtxPosition(&sp44, &i_this->field_0xb48);
                i_this->field_0xb48 += player->current.pos;
                i_this->field_0xb60 = player->current.pos;
                i_this->field_0xb60.y += 120.0f;
            }
            break;

        case 200:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->field_0xb42 = 201;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0xb44 = 0;
            i_this->field_0xbb8 = 55.0f;
            player->changeOriginalDemo();
            player->changeDemoMode(2, 0, 0, 0);
            sp50.set(-23451.0f,262.0f,-15980.0f);
            player->changeDemoPos0(&sp50);
            i_this->field_0xb60.set(-22325.0f, 275.0f, -15119.0f);
            i_this->field_0xb48.set(-22672.0f, 311.0f, -15080.0f);
            i_this->field_0xb6c.set(-22343.0f, 273.0f, -15017.0f);
            i_this->field_0xb78.set(0.0f, 0.0f, 0.0f);
            i_this->field_0xb84.x = NPC_KS_FABSF(i_this->field_0xb6c.x - i_this->field_0xb60.x);
            i_this->field_0xb84.y = NPC_KS_FABSF(i_this->field_0xb6c.y - i_this->field_0xb60.y);
            i_this->field_0xb84.z = NPC_KS_FABSF(i_this->field_0xb6c.z - i_this->field_0xb60.z);
            i_this->field_0xbc4 = 0.0f;
            /* dSv_event_flag_c::F_0224 - Faron Woods - Flag for lantern guide monkey cutscene */
            dComIfGs_onEventBit(u16(dSv_event_flag_c::saveBitLabels[0xE0]));
            // fallthrough
        case 201:
            if (i_this->field_0xb44 < 30) {
                sp50.set(-22177.0f, 213.0f, -14705.0f);
                setPlayerPosAndAngle(&sp50, -0x5FFD);
                player->speedF = 0.0f;
            }

            cam_3d_morf(i_this, 0.05f + BREG_F(17));
            cLib_addCalc2(&i_this->field_0xbc4, 0.02f + BREG_F(18), 1.0f, 0.0001f + BREG_F(19));

            if (i_this->field_0xb44 == 110 + AREG_S(5)) {
                i_this->mMode = 2;
                i_this->field_0xbb4 = 500.0f;
            }

            if (i_this->field_0xb44 == 132 + AREG_S(6)) {
                i_this->field_0xc17 = 2;

                mDoAud_seStart(0x5001A, NULL, 0, 0);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                dComIfGs_setItem(1, 0xFF);
                dMeter2Info_setOilGaugeBackUp(dComIfGs_getOil());
                dComIfGp_getVibration().StartShock(2, 31, cXyz(0.0f, 1.0f, 0.0f));
            }

            if (i_this->field_0xb44 == 139 + AREG_S(6)) {
                player->changeDemoMode(1, 1, 0, 0);
            }

            if (i_this->field_0xb44 == 144) {
                cMtx_YrotS(*calc_mtx, player->shape_angle.y);
                sp44.x = 0.0f;
                sp44.y = AREG_F(1);
                sp44.z = 100.0f + AREG_F(2);
                MtxPosition(&sp44, &sp50);
                a_this->current.pos = player->current.pos + sp50;
                a_this->current.angle.y = s16(player->shape_angle.y);
                i_this->field_0x8fc.y = player->shape_angle.y;
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
                i_this->mMode = 230;
                i_this->mTimers[0] = 65 + JREG_S(7);
            }

            if (i_this->field_0xb44 == 145) {
                i_this->field_0xb42 = 202;
                i_this->field_0xb44 = 0;
                i_this->field_0xb60.set(-22613.0f, 350.0f, -15097.0f);
                i_this->field_0xb48.set(-22275.0f, 350.0f, -15092.0f);
            }
            break;

        case 202:
            if (i_this->field_0xb44 == 30) {
                i_this->field_0xb60.set(-23371.0f, 380.0f, -15901.0f);
                i_this->field_0xb48.set(-23163.0f, 474.0f, -15653.0f);
                player->changeDemoMode(1, 1, 0, 0);
            }

            if (i_this->field_0xb44 >= 120) {
                cLib_addCalc2(&i_this->field_0xbb8, 80.0f, 0.05f, 0.2f + YREG_F(9));
            }

            if (i_this->field_0xb44 == 270) {
                i_this->field_0xb42 = 203;
                i_this->field_0xb44 = 0;
            }
            break;

        case 203:
            i_this->field_0xb60.set(-22650.0f, 334.0f, -15147.0f);
            i_this->field_0xb48.set(-22277.0f, 355.0f, -15077.0f);
            i_this->field_0xbb8 = 55.0f;
            if (i_this->field_0xb44 == 10) {
                midna_p = daPy_py_c::getMidnaActor();
                midna_p->changeOriginalDemo();
                midna_p->changeDemoMode(2);
            }

            if (i_this->field_0xb44 >= 100) {
                i_this->field_0xb44 = 100;
            }

            if (i_this->field_0xb44 >= 70) {
                if (i_this->field_0xb44 == 70) {
                    i_this->mMsgFlow.init(a_this, 2015, 0, NULL);
                }
                if (i_this->mMsgFlow.doFlow(a_this, NULL, 0) != 0) {
                    i_this->field_0xb42 = 204;
                    daMidna_c* midna_p = daPy_py_c::getMidnaActor();
                    midna_p->changeDemoMode(5);
                }
            }
            break;

        case 204: {
            daMidna_c* midna_p = daPy_py_c::getMidnaActor();
            if (midna_p->checkShadowReturnEnd() != 0) {
                i_this->field_0xb42 = 100;
            }
            break;
        }

        case 300:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->field_0xb42 = 301;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0xb44 = 0;
            i_this->field_0xbb8 = 60.0f;
            player->changeOriginalDemo();
            i_this->field_0xb60.set(-30827.0f, 411.0f, -17200.0f);
            i_this->field_0xb48.set(-30601.0f, 388.0f, -17465.0f);
            // fallthrough
        case 301:
            sp50.set(-30496.0f, 300.0f, -17449.0f);
            setPlayerPosAndAngle(&sp50, -0x18B4);
            if (i_this->field_0xb44 == 60) {
                i_this->field_0xb42 = 302;
                i_this->field_0xb44 = 0;
                i_this->field_0xb60.set(-30844.0f, 298.0f, -17007.0f);
                i_this->field_0xb48.set(-30653.0f, 357.0f, -17289.0f);
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
                i_this->field_0xc17 = 3;
                mDoAud_seStart(0x600AC, &i_this->field_0xc04, 0, 0);
            }
            break;

        case 302:
            if (i_this->field_0xb44 == 30) {
                dComIfGp_setItemOilCount(-int(dComIfGs_getMaxOil()));
            }

            if (i_this->field_0xb44 == 80) {
                i_this->field_0xb42 = 303;
                i_this->field_0xb44 = 0;
                i_this->field_0xb60.set(-30887.0f, 429.0f, -17181.0f);
                i_this->field_0xb48.set(-30870.0f, 471.0f, -17527.0f);
            }
            break;

        case 303:
            if (i_this->field_0xb44 == 10) {
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
                i_this->mMode = 301;
                a_this->speedF = l_HIO.demo_speed_2;
            }

            if (i_this->field_0xb44 == 0x50) {
                i_this->field_0xb42 = 100;
                i_this->mMode = 302;
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
                /* dSv_event_flag_c::F_0225 - Faron Woods - Lanter guide monkey doesn't come out a second time */
                dComIfGs_onEventBit(u16(dSv_event_flag_c::saveBitLabels[0xE1]));
                dComIfGs_offSwitch(61, fopAcM_GetRoomNo(a_this));
            }
            break;

        case 350:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->field_0xb42 = 351;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0xb44 = 0;
            i_this->field_0xbb8 = 55.0f;
            player->changeOriginalDemo();
            i_this->field_0xb60 = a_this->current.pos;
            i_this->field_0xb48.set(-37509.0f, 805.0f, -22416.0f);
            // fallthrough
        case 351:
            sp44.set(-36523.0f, 335.0f, -20500.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp44, 0xffff8000, 0);
            sp44 = a_this->current.pos;
            cLib_addCalc2(&i_this->field_0xb60.x, sp44.x, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->field_0xb60.y, sp44.y + 100.0f + nREG_F(0), 0.2f, 50.0f);
            cLib_addCalc2(&i_this->field_0xb60.z, sp44.z, 0.2f, 50.0f);

            if (i_this->field_0xb44 == 105) {
                i_this->field_0xb48.set(-37817.0f, 800.0f, -21442.0f);
            }

            if (i_this->field_0xb44 == 178) {
                fpcM_Search(s_fsdown_sub, i_this);
            }

            if (i_this->field_0xb44 == 180) {
                i_this->field_0xb60.set(-36666.0f, 600.0f, -22213.0f);
                i_this->field_0xb48.set(-36489.0f, 399.0f, -20932.0f);
                i_this->field_0xb6c.set(-36574.0f, 421.0f, -21554.0f);
                i_this->field_0xb54.set(-36397.0f, 374.0f, -20263.0f);

                i_this->field_0xb78.x = NPC_KS_FABSF(i_this->field_0xb54.x - i_this->field_0xb48.x);
                i_this->field_0xb78.y = NPC_KS_FABSF(i_this->field_0xb54.y - i_this->field_0xb48.y);
                i_this->field_0xb78.z = NPC_KS_FABSF(i_this->field_0xb54.z - i_this->field_0xb48.z);
                i_this->field_0xb84.x = NPC_KS_FABSF(i_this->field_0xb6c.x - i_this->field_0xb60.x);
                i_this->field_0xb84.y = NPC_KS_FABSF(i_this->field_0xb6c.y - i_this->field_0xb60.y);
                i_this->field_0xb84.z = NPC_KS_FABSF(i_this->field_0xb6c.z - i_this->field_0xb60.z);
                i_this->field_0xbc4 = 0.0f;
                i_this->field_0xb42 = 352;
                i_this->field_0xb44 = 0;
                i_this->field_0xb60.y = 1500.0f + nREG_F(1);
            }
            break;

        case 352:
            if (i_this->field_0xb44 == 40) {
                i_this->mMode = 40;
            }

            if (i_this->field_0xb44 == 110) {
                fpcM_Search(s_fsdown_sub, i_this);
            }

            if (i_this->field_0xb44 > 160) {
                cam_3d_morf(i_this, 0.1f + BREG_F(17));
                cLib_addCalc2(&i_this->field_0xbc4, 0.2f + BREG_F(18), 1.0f, 0.005f + BREG_F(19));
            } else if (i_this->field_0xb44 > 25 + nREG_S(0)) {
                cLib_addCalc2(&i_this->field_0xb60.y, 600.0f, 0.5f, 100.0f + nREG_F(2));
            }

            if (i_this->field_0xb44 == 230) {
                i_this->field_0xb42 = 100;
                fpcM_Search(s_fsdown_sub, i_this);
                /* dSv_event_flag_c::F_0544 - Faron Woods - Watched cutscene of monkey girl running away after being attacked by puppet */
                dComIfGs_onEventBit(u16(dSv_event_flag_c::saveBitLabels[0x220]));
            }
            break;

        case 360:
            if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
                a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            i_this->field_0xb42 = 361;
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            i_this->field_0xb44 = 0;
            i_this->field_0xbb8 = 55.0f;
            player->changeOriginalDemo();

            i_this->field_0xb60.set(-36581.0f, 412.0f, -21418.0f);
            i_this->field_0xb48.set(-36509.0f, 412.0f, -21127.0f);
            i_this->field_0xb6c.set(-38113.0f, 1234.0f, -22897.0f);
            i_this->field_0xb54.set(-37882.0f, 1182.0f, -22713.0f);

            i_this->field_0xb78.x = NPC_KS_FABSF(i_this->field_0xb54.x - i_this->field_0xb48.x);
            i_this->field_0xb78.y = NPC_KS_FABSF(i_this->field_0xb54.y - i_this->field_0xb48.y);
            i_this->field_0xb78.z = NPC_KS_FABSF(i_this->field_0xb54.z - i_this->field_0xb48.z);
            i_this->field_0xb84.x = NPC_KS_FABSF(i_this->field_0xb6c.x - i_this->field_0xb60.x);
            i_this->field_0xb84.y = NPC_KS_FABSF(i_this->field_0xb6c.y - i_this->field_0xb60.y);
            i_this->field_0xb84.z = NPC_KS_FABSF(i_this->field_0xb6c.z - i_this->field_0xb60.z);
            i_this->field_0xbc4 = 0.0f;
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mMode = 41;
            i_this->mTimers[0] = 30;
            // fallthrough
        case 361:
            sp44.set(-36540.0f, 335.0f, -20870.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp44, -32768, 0);
            if (i_this->field_0xb44 >= 105) {
                if (i_this->field_0xb44 == 105) {
                    i_this->mMsgFlow.init(a_this, 116, 0, NULL);
                }

                if (i_this->mMsgFlow.getNowMsgNo() == 0x1421) {
                    i_this->field_0xb42 = 362;
                    i_this->field_0xb44 = 0;
                }
                i_this->mMsgFlow.doFlow(a_this, NULL, 0);
            }
            break;

        case 362:
            if (i_this->field_0xb44 == 15) {
                i_this->mTimers[2] = 30;
            }
            if (i_this->field_0xb44 >= 30) {
                cam_3d_morf(i_this, 0.2f + BREG_F(17));
                cLib_addCalc2(&i_this->field_0xbc4, 0.3f + BREG_F(18), 1.0f, 0.01f + BREG_F(19));
            }

            if (i_this->mMsgFlow.getNowMsgNo() == 0x1423) {
                i_this->field_0xb42 = 363;
                i_this->field_0xb44 = 0;
                i_this->field_0xb6c.set(-36467.0f, 426.0f, -20914.0f);
                i_this->field_0xb54.set(-36244.0f, 409.0f, -20714.0f);

                i_this->field_0xb78.x = NPC_KS_FABSF(i_this->field_0xb54.x - i_this->field_0xb48.x);
                i_this->field_0xb78.y = NPC_KS_FABSF(i_this->field_0xb54.y - i_this->field_0xb48.y);
                i_this->field_0xb78.z = NPC_KS_FABSF(i_this->field_0xb54.z - i_this->field_0xb48.z);
                i_this->field_0xb84.x = NPC_KS_FABSF(i_this->field_0xb6c.x - i_this->field_0xb60.x);
                i_this->field_0xb84.y = NPC_KS_FABSF(i_this->field_0xb6c.y - i_this->field_0xb60.y);
                i_this->field_0xb84.z = NPC_KS_FABSF(i_this->field_0xb6c.z - i_this->field_0xb60.z);
                i_this->field_0xbc4 = 0.0f;
            }
            i_this->mMsgFlow.doFlow(a_this, NULL, 0);
            break;

        case 363:
            cam_3d_morf(i_this, 0.1f + BREG_F(17));
            cLib_addCalc2(&i_this->field_0xbc4, 0.3f + BREG_F(18), 1.0f, 0.01f + BREG_F(19));
            if (i_this->mMsgFlow.doFlow(a_this, NULL, 0) != 0) {
                i_this->field_0xb42 = 100;
            }
            break;

    }

    if (i_this->field_0xb42 == 99 || i_this->field_0xb42 == 98) {
        if (i_this->field_0xb42 == 99) {
            fopAcM_delete(a_this);
        }

        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        sp44.x = 0.0f;
        sp44.y = 100.0f + JREG_F(1);
        sp44.z = -250.0f + JREG_F(2);
        MtxPosition(&sp44, &i_this->field_0xb48);
        i_this->field_0xb48 += player->current.pos;
        i_this->field_0xb60 = player->current.pos;
        i_this->field_0xb60.y += 120.0f;
        i_this->field_0xb42 = 100;
    }

    if (i_this->field_0xb42 == 100) {
        camera->mCamera.Reset(i_this->field_0xb60, i_this->field_0xb48, i_this->field_0xbb8, 0);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->field_0xb42 = 0;
    }

    if (i_this->field_0xb42 != 0) {
        camera->mCamera.Set(i_this->field_0xb60, i_this->field_0xb48, i_this->field_0xbb8, 0);
        i_this->field_0xb44++;
    }
}

static int saru_count_check(npc_ks_class* i_this) {
    i_this->field_0x934 = (obj_sw_class*)fpcM_Search(s_sw_sub, i_this);
    if (i_this->field_0x934 == NULL) {
        return 0;
    } else {
        for (int i = 0; i < i_this->field_0x934->field_0x91c; i++) {
            if (saru_p[i] == NULL) {
                return 0;
            }
        }

        return 1;
    }
}

static void action_check(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz spd4;

    switch (fopAcM_GetRoomNo(a_this)) {
        case 0:
            if (!dComIfGp_event_runCheck() && i_this->field_0x5b6 == 0) {
                if (saru_p[1] == NULL) {
                    if (dComIfGs_isSwitch(22, fopAcM_GetRoomNo(a_this))) {
                        spd4.x = a_this->current.pos.x - 5.0f;
                        spd4.z = a_this->current.pos.z - 5274.0f;
                        if (JMAFastSqrt(spd4.x * spd4.x + spd4.z * spd4.z) < 500.0f) {
                            i_this->mActionID = 110;
                            i_this->mMode = 0;
                            i_this->field_0xaec = 1;
                            return;
                        }
                    } else {
                        dBgS_GndChk dStack_74;
                        spd4.set(0.0f, 3250.0f, 6500.0f);
                        dStack_74.SetPos(&spd4);
                        if (dComIfG_Bgsp().GroundCross(&dStack_74) > 3000.0f) {
                            i_this->mActionID = 112;
                            i_this->mMode = 0;
                            if (a_this->current.pos.y < 3000.0f) {
                                a_this->current.pos = player->current.pos;
                                a_this->old.pos = a_this->current.pos;
                            }

                            i_this->field_0xaec = 1;
                            return;
                        }
                    }
                } else if (saru_p[1] != NULL && saru_p[2] != NULL && saru_p[3] != NULL) {
                    if (player->current.pos.y < 3000.0f) {
                        dComIfGs_onSwitch(82, fopAcM_GetRoomNo(a_this));
                    }

                    if (!dComIfGs_isSwitch(82, fopAcM_GetRoomNo(a_this))) {
                        spd4.x = player->current.pos.x - -500.0f;
                        spd4.z = player->current.pos.z - 7211.0f;
                        if (JMAFastSqrt(spd4.x * spd4.x + spd4.z * spd4.z) < 200.0f) {
                            dComIfGs_onSwitch(82, fopAcM_GetRoomNo(a_this));
                            for (int i = 0; i < 4; i++) {
                                saru_p[i]->mActionID = 111;
                                saru_p[i]->mMode = 0;
                                saru_p[i]->field_0xaec = 1;
                            }
                            i_this->field_0xb42 = 80;
                        }
                    }
                }
            }

            if (!dComIfGs_isSwitch(82, fopAcM_GetRoomNo(a_this)) && saru_count_check(i_this) != 0) {
                if (dComIfGs_isSwitch(22, fopAcM_GetRoomNo(a_this))) {
                    for (int i = 0; i < 2; i++) {
                        spd4.x = player->current.pos.x - i_this->field_0x934->field_0x904[i].x;
                        spd4.y = player->current.pos.y - (i_this->field_0x934->field_0x904[i].y - 450.0f);
                        spd4.z = player->current.pos.z - i_this->field_0x934->field_0x904[i].z;
                        if (spd4.abs() < 300.0f + YREG_F(11)) {
                            i_this->field_0xbd9 = 0;
                            i_this->mActionID = 20;
                            i_this->mMode = 0;
                            i_this->field_0xaec = 0;
                            i_this->field_0xbc8 = i_this->field_0x934->field_0x904[i];
                            spd4 = i_this->field_0x934->field_0x904[1 - i] - i_this->field_0xbc8;
                            leader->field_0xbd4 = cM_atan2s(spd4.x, spd4.z);
                            leader->field_0xb42 = 10;
                            leader->field_0xb40 = i_this->field_0x934->field_0x570;

                            if (i_this->field_0x5b6 != 0) {
                                leader->field_0x934 = i_this->field_0x934;
                                leader->field_0xbc8 = i_this->field_0xbc8;
                            }
                        }
                    }
                }
            }
            break;

        case 1:
            if (!dComIfGp_event_runCheck() && i_this->field_0x5b6 == 0 && saru_p[1] == NULL && a_this->field_0x567 == 0) {
                spd4.x = a_this->current.pos.x - 5334.0f;
                spd4.z = a_this->current.pos.z - 7609.0f;
                if (JMAFastSqrt(spd4.x * spd4.x + spd4.z * spd4.z) < 700.0f) {
                    i_this->mActionID = 113;
                    i_this->mMode = 0;
                    i_this->field_0xaec = 1;
                    a_this->field_0x567 = 1;
                }
            }
            break;

        case 2:
        case 4:
            if (saru_count_check(i_this) != 0) {
                int iVar1 = 1;
                if (fopAcM_GetRoomNo(a_this) == 2) {
                    for (int i = 0; i < i_this->field_0x934->field_0x91c; i++) {
                        spd4 = player->current.pos - saru_p[i]->actor.current.pos;
                        if (spd4.abs() > 400.0f) {
                            iVar1 = 0;
                        }
                    }
                }

                if (iVar1 != 0) {
                    spd4.x = player->current.pos.x - i_this->field_0x934->field_0x904[0].x;
                    spd4.y = player->current.pos.y - (i_this->field_0x934->field_0x904[0].y - 450.0f);
                    spd4.z = player->current.pos.z - i_this->field_0x934->field_0x904[0].z;
                    if (spd4.abs() < 300.0f) {
                        i_this->field_0xbc8 = i_this->field_0x934->field_0x904[0];
                        if (i_this->field_0x5b6 == 0) {
                            i_this->field_0xbd9 = 0;
                            i_this->mActionID = 20;
                            i_this->mMode = 0;
                            i_this->field_0xaec = 0;
                            leader->field_0xb42 = 10;
                            leader->field_0xb40 = 0;
                            leader->field_0xbc8 = i_this->field_0xbc8;
                            spd4 = i_this->field_0x934->field_0x904[1] - i_this->field_0xbc8;
                            leader->field_0xbd4 = cM_atan2s(spd4.x, spd4.z);
                        } else {
                            i_this->mActionID = 22;
                            i_this->mMode = 0;
                            i_this->field_0x930 = leader;
                        }
                    }
                }
            } else if (!checkDoorDemo() && fopAcM_GetRoomNo(a_this) == 4 && i_this->field_0x5b6 == 0) {
                if (!dComIfGs_isSwitch(22, fopAcM_GetRoomNo(a_this))) {
                    i_this->mActionID = 203;
                    i_this->mMode = 0;
                }
            }
            break;

        case 9:
            if (!dComIfGp_event_runCheck() && i_this->field_0x5b6 == 4 && player->current.pos.z < -3900.0f) {
                if (!dComIfGs_isSwitch(81, fopAcM_GetRoomNo(a_this))) {
                    dBgS_GndChk dStack_c8;
                    spd4.set(7275.0f, 3373.0f, -4790.0f);
                    dStack_c8.SetPos(&spd4);
                    if (dComIfG_Bgsp().GroundCross(&dStack_c8) > 3000.0f) {
                        i_this->mActionID = 115;
                        i_this->mMode = 0;
                        dComIfGs_onSwitch(81, fopAcM_GetRoomNo(a_this));
                        i_this->field_0xaec = 1;
                    }
                }
            }
            break;

        case 12:
            if (!dComIfGp_event_runCheck() && i_this->field_0x5b6 == 4) {
                if (!dComIfGs_isSwitch(83, fopAcM_GetRoomNo(a_this))) {
                    i_this->mActionID = 204;
                    i_this->mMode = 0;
                    fopAcM_setRoomLayer(a_this, 12);
                    i_this->field_0xb42 = 77;
                    i_this->field_0xaec = 1;
                }
            }
    }
}

static BOOL water_check(npc_ks_class* i_this, cXyz param_2, f32 param_3) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp154;
    dBgS_GndChk dStack_80;
    dBgS_LinChk dStack_f0;
    sp154.x = param_2.x;
    sp154.y = param_2.y + 3000.0f;
    sp154.z = param_2.z;
    dStack_f0.Set(&a_this->current.pos, &sp154, a_this);
    if (dComIfG_Bgsp().LineCross(&dStack_f0)) {
        sp154.y = dStack_f0.GetCross().y - 10.0f;;
    }

    dBgS_ObjGndChk_Spl cStack_148;
    cStack_148.SetPos(&sp154);
    i_this->field_0x63c = dComIfG_Bgsp().GroundCross(&cStack_148);
    sp154.y = i_this->field_0x63c + 60.0f;

    dStack_80.SetPos(&sp154);
    f32 gnd_cross = dComIfG_Bgsp().GroundCross(&dStack_80);
    if ((i_this->field_0x63c - gnd_cross) > param_3) {
        return TRUE;
    }

    return FALSE;
}

static f32 yuka_jump_x = 80.0f;

static int npc_ks_option(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);
    fopAc_ac_c* actor_p;
    cXyz sp140, sp14c;
    f32 fVar3 = 0.0f;
    f32 fVar4 = 5.0f;
    int unused_0x58 = 0;
    (void) unused_0x58;
    u32 i_soundID = Z2SE_SY_DUMMY;
    int retval = 1;
    int iVar2 = 1;
    int iVar1 = 1;
    s16 sVar1 = 0x800;
    int frame = i_this->mpModelMorf->getFrame();
    f32 fVar2;
    f32 fVar1 = 0.0f;
    if (fopAcM_GetRoomNo(a_this) == 7 || fopAcM_GetRoomNo(a_this) == 8) {
        fVar1 = 10000.0f;
    }

    if (daPy_getPlayerActorClass()->checkAutoJumpStart() && i_this->field_0xbd8 < 3) {
        i_this->field_0xbd8++;
    }

    if (i_this->mMode >= 30 && i_this->mMode < 40) {
        actor_p = bomb_check(i_this);
        if (actor_p == NULL) {
            i_this->mMode = 0;
        } else {
            a_this->health = 0;
            sp140.x = actor_p->current.pos.x - a_this->current.pos.x;
            sp140.z = actor_p->current.pos.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp140.x, sp140.z);
            fVar2 = JMAFastSqrt(sp140.x * sp140.x + sp140.z * sp140.z);
        }
    } else if (i_this->mMode >= 40 && i_this->mMode < 50) {
        actor_p = enemy_check(i_this, fVar1 + 700.0f);
        if (actor_p == NULL) {
            i_this->mMode = 0;
        } else {
            a_this->health = 0;
            if (i_this->mMode == 40) {
                sp140.x = actor_p->current.pos.x - a_this->current.pos.x;
                sp140.z = actor_p->current.pos.z - a_this->current.pos.z;
                i_this->field_0x8fc.y = cM_atan2s(sp140.x, sp140.z);
                fVar2 = JMAFastSqrt(sp140.x * sp140.x + sp140.z * sp140.z);
            }
        }
    }

    f32 dbg_f30 = l_HIO.pull_distance;
    if (checkDoorDemo()) {
        dbg_f30 -= 70.0f;
    }

    switch (i_this->mMode) {
        case 0:
            i_this->mMode = 1;
            anm_init(i_this, 51, 5.0f, 2, 1.0f);
            i_this->mTimers[0] = cM_rndF(100.0f) + 100.0f;
            fopAcM_setStageLayer(a_this);
            a_this->health = 0;
            // fallthrough
        case 1:
            if (i_this->mTimers[0] == 0) {
                if (cM_rndF(1.0f) < 0.5f) {
                    if (i_this->field_0x5d0 != 51) {
                        anm_init(i_this, 51, 5.0f, 2, 1.0f);
                        i_this->mTimers[0] = cM_rndF(100.0f) + 80.0f;
                        i_soundID = Z2SE_KOSARU_V_WAIT;
                    }
                } else if (i_this->field_0x5d0 != 53) {
                    anm_init(i_this, 53, 8.0f, 2, 1.0f);
                    i_this->mTimers[0] = cM_rndF(30.0f) + 40.0f;
                    i_soundID = Z2SE_KOSARU_V_WAIT;
                }

            }

            if (i_this->mTimers[1] == 0 && i_this->field_0x5c4 > (dbg_f30 + 20.0f)) {
                anm_init(i_this, 28, 5.0f, 2, 1.0f);
                i_this->mMode = 2;
                i_soundID = Z2SE_KOSARU_V_JUMP;
            }
            break;

        case 2:
            fVar3 = l_HIO.holding_speed_l;
            if (i_this->field_0x5c4 < (dbg_f30 - 20.0f)) {
                i_this->mMode = 0;
            } else if (i_this->field_0x5c4 > dbg_f30 + 200.0f) {
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
                i_this->mMode = 3;
                i_soundID = Z2SE_KOSARU_V_WALK;
            }

            if (i_this->mObjAcch.ChkWallHit() && i_this->mObjAcch.ChkGroundHit() && !otherBgCheck(a_this, player)) {
                a_this->speed.y = 30.0f;
            }
            break;
            
        case 3:
            fVar3 = l_HIO.holding_speed_h;
            if (i_this->mpModelMorf->checkFrame(1.0f)) {
                i_soundID = Z2SE_KOSARU_V_WALK;
            }

            if (i_this->field_0x5c4 < (dbg_f30 + 130.0f)) {
                anm_init(i_this, 28, 3.0f, 2, 1.0f);
                i_this->mMode = 2;
            }

            if (i_this->mObjAcch.ChkWallHit() && i_this->mObjAcch.ChkGroundHit() && !otherBgCheck(a_this, player)) {
                a_this->speed.y = 30.0f;
            }
            break;

        case 5:
            i_this->field_0x8fc.y = i_this->field_0x5c8;
            if (frame >= 11) {
                i_this->mMode = 0;
            }
            break;

        case 7:
            anm_init(i_this, 47, 3.0f, 2, i_this->field_0x5d4);
            i_this->mMode = 8;
            // fallthrough
        case 8:
            if (i_this->mpModelMorf->checkFrame(1.0f)) {
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_SWIM, 0, -1);
            }

            iVar1 = 0;
            a_this->gravity = 0.0f;
            cLib_addCalc0(&a_this->speed.y, 1.0f, 1.0f);
            cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x63c, 0.1f, 10.0f);

            if (i_this->field_0x5c4 > (dbg_f30 + 50.0f)) {
                fVar3 = 5.0f + JREG_F(8);
            } else {
                fVar3 = 0.0f;
            }

            cLib_addCalc2(&i_this->field_0x5d4, fVar3 * 0.1f + 0.5f, 1.0f, 0.1f);
            i_this->mpModelMorf->setPlaySpeed(i_this->field_0x5d4);

            fVar4 = 0.1f;
            sVar1 = 0x100;
            i_this->field_0x8fc.y = i_this->field_0x5c8;

            if (i_this->mTimers[0] == 0) {
                i_this->mTimers[0] = cM_rndF(30.0f) + 20.0f;
                i_soundID = Z2SE_KOSARU_V_WAIT;
            }
            break;

        case 10:
            i_this->mTimers[0] = cM_rndF(30.0f) + 20.0f;
            i_this->mMode = 11;
            i_this->field_0x8fc.y = i_this->field_0x5c8;
            // fallthrough
        case 11:
            i_this->field_0x5fc = 1;
            if (i_this->mTimers[0] == 0) {
                if (cM_rndF(1.0f) < 0.5f) {
                    anm_init(i_this, 30, 3.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, 5, 3.0f, 0, 1.0f);
                }

                i_this->mMode = 12;
            }
            break;

        case 12:
            i_this->field_0x5fc = 1;
            if (i_this->mpModelMorf->isStop()) {
                if (cM_rndF(1.0f) < 0.5f) {
                    anm_init(i_this, 30, 3.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, 5, 3.0f, 0, 1.0f);
                }

                i_this->mMode = 13;
            }
            break;

        case 13:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mMode = 0;
            }
            break;

        case 20:
            iVar2 = 0;
            if (i_this->mTimers[0] == 0) {
                i_this->mActionID = 200;
                i_this->mMode = 0;
                i_this->field_0xaec = 0;
                return retval;
            }
            break;

        case 30:
            fVar3 = l_HIO.holding_speed_h;
            i_this->field_0x8fc.y += NPC_KS_180DEG_ROT;
            if (fVar2 > 400.0f) {
                i_this->mMode = 31;
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
            }
            break;

        case 31:
            i_this->field_0x8fc.y = i_this->field_0x5c8;
            i_this->field_0x938 = fopAcM_GetID(actor_p);
            i_this->field_0x5fc = 1;
            if (fVar2 < 350.0f) {
                i_this->mMode = 30;
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mMode = 41;
            i_this->mTimers[0] = cM_rndF(80.0f) + 100.0f;
            // ditto.
            i_this->field_0x8fc.y += NPC_KS_180DEG_ROT;
            break;

        case 41:
            sVar1 = 0x1000;
            if ((i_this->field_0x5d0 == 39 || (i_this->field_0x5d0 == 43 && frame < 7)) || ((frame > 40 && frame < 48) || frame > 65)) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->mTimers[0] == 0 && i_this->field_0x5d0 == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->field_0x5d0 == 43) {
                if (i_this->mpModelMorf->isStop()) {
                    i_this->mMode = 40;
                }
            }
            break;

        case 50:
            retval = 2;
            a_this->speedF = 0.0f;
            if (i_this->mTimers[0] == 0) {
                fopAc_ac_c* player_p2 = (fopAc_ac_c*) dComIfGp_getPlayer(0);
                cMtx_YrotS(*calc_mtx, (s16) player_p2->shape_angle.y);
                sp140.y = 50.0f;
                sp140.z = 0.0f;

                int i = 0;
                for (; i < 2; i++) {
                    dBgS_GndChk dStack_e0;
                    sp140.x = yuka_jump_x;
                    MtxPosition(&sp140, &sp14c);
                    sp14c += player_p2->current.pos;
                    dStack_e0.SetPos(&sp14c);
                    sp14c.y = dComIfG_Bgsp().GroundCross(&dStack_e0);
                    yuka_jump_x *= -1.0f;
                    if (NPC_KS_FABSF(sp14c.y - player_p2->current.pos.y) < 20.0f) {
                        i_this->mMode = 51;
                        i_this->field_0x8f0 = sp14c;
                        i_this->field_0x910 = a_this->current.pos;
                        i_this->field_0x91c = i_this->field_0x8f0;
                        anm_init(i_this, 32, 2.0f, 0, 1.0f);
                        break;
                    }
                }
                if (i == 2) {
                    i_this->mMode = 0;
                }
            }

            i_this->field_0x8fc.y = i_this->field_0x5c8;
            fVar3 = 4096.0f;
            break;

        case 51:
            retval = 2;
            if (i_this->field_0x5d0 == 32 && i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                a_this->speedF = 40.0f + TREG_F(9);
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->field_0xbe0 = 1;
            }

            sp140 = i_this->field_0x8f0 - a_this->current.pos;
            i_this->field_0x8fc.y = cM_atan2s(sp140.x, sp140.z);
            i_this->field_0x8fc.x = -cM_atan2s(sp140.y, JMAFastSqrt(sp140.x * sp140.x + sp140.z * sp140.z));
            i_this->field_0x8fc.y = i_this->field_0x8fc.y;
            sVar1 = 0x2000;

            if (a_this->speedF > 25.0f) {
                i_this->field_0x90c = 1;
            }

            if (sp140.abs() <= a_this->speedF * 1.2f) {
                i_this->mMode = 0;
                a_this->speedF *= 0.5f;
                i_this->field_0xbe0 = 1;
                return retval;
            }

            fVar3 = a_this->speedF;
    }

    cLib_addCalc2(&a_this->speedF, fVar3, 1.0f, fVar4);
    if (i_this->mMode < 7) {
        i_this->field_0x5fc = 1;
        if (fVar3 > 1.0f) {
            i_this->field_0x8fc.y = i_this->field_0x5c8;
        } else {
            s16 sVar2 = a_this->current.angle.y - i_this->field_0x5c8;
            if ((sVar2 > 0x3000 || sVar2 < -0x3000) && i_this->mMode < 5) {
                anm_init(i_this, 28, 3.0f, 0, 1.0f);
                i_this->mMode = 5;
                i_soundID = Z2SE_KOSARU_V_WALK;
            }
        }

        if (a_this->health != 0) {
            a_this->health = 0;
            i_this->mMode = 10;
            return retval;
        }

        if (!checkDoorDemo()) {
            if ((actor_p = (fopAc_ac_c*)fpcM_Search(s_01_sub, i_this)) != NULL) {
                sp140 = actor_p->current.pos - a_this->current.pos;
                i_this->field_0x8fc.y = cM_atan2s(sp140.x, sp140.z);
                i_this->mMode = 20;
                i_this->mTimers[0] = 60;
                return retval;
            }
        }

        if (i_this->mTimers[3] == 0) {
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0x938 = fopAcM_GetID(en_search_test(i_this));
            } else {
                i_this->field_0x938 = -1;
            }

            i_this->mTimers[3] = cM_rndF(30.0f) + 30.0f;
        }

        if ((i_this->field_0x5e6 & 15) == 0 && bomb_view_check(i_this) != NULL) {
            i_this->mMode = 30;
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            return retval;
        }

        if ((i_this->field_0x5e6 + 2 & 15) == 0 && enemy_view_check(i_this, fVar1 + 600.0f) != NULL) {
            i_this->mMode = 40;
            return retval;
        }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 2, sVar1);
    cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x800);

    fopAc_ac_c* player_p3 = (fopAc_ac_c*) dComIfGp_getPlayer(0);
    if (iVar2 != 0) {
        if (NPC_KS_FABSF(player_p3->current.pos.y - a_this->current.pos.y) > 3000.0f ||
            (fopAcM_CheckCondition(a_this, 4) != 0 && fopAcM_otherBgCheck(a_this, dComIfGp_getPlayer(0)))) {
            if (iVar1 != 0 && player_p3->speedF > 2.0f) {
                camera_class* camera = (camera_class*) dComIfGp_getCamera(0);
                sp140.x = camera->lookat.eye.x - camera->lookat.center.x;
                sp140.z = camera->lookat.eye.z - camera->lookat.center.z;
                cMtx_YrotS(*calc_mtx, cM_atan2s(sp140.x, sp140.z));
                sp140.x = 0.0f;
                sp140.y = -50.0f;
                sp140.z = 100.0f;
                MtxPosition(&sp140, &sp14c);
                sp14c += camera->lookat.eye;
                
                dBgS_GndChk dStack_134;
                dStack_134.SetPos(&sp14c);
                if (NPC_KS_FABSF(sp14c.y - dComIfG_Bgsp().GroundCross(&dStack_134)) < 500.0f) {
                    a_this->current.pos = sp14c;
                    a_this->old = a_this->current;
                    OS_REPORT("////////KS OP RESET \n");
                } else {
                    OS_REPORT("////////KS OP NO FLOOR・NONRESET \n");
                    i_soundID = Z2SE_SY_DUMMY;
                }
            } else {
                i_soundID = Z2SE_SY_DUMMY;
            }
        }
    }

    if (i_soundID != Z2SE_SY_DUMMY) {
        i_this->mSound.startCreatureVoice(i_soundID, -1);
    }

    if (i_this->field_0x94c != 0 && (i_this->field_0x5d0 == 26 || i_this->field_0x5d0 == 28)) {
        anm_init(i_this, 55, 3.0f, 2, 1.0f);
    } else if (i_this->field_0x94c == 0 && i_this->field_0x5d0 == 55) {
        anm_init(i_this, 26, 3.0f, 2, 1.0f);
    }

    if (iVar1 != 0) {
        if (i_this->field_0xbdd != 0 && a_this->current.pos.y < i_this->field_0x63c) {
            if (a_this->speed.y < -10.0f) {
                a_this->speed.y = -10.0f;
            }

            i_this->mMode = 7;
            a_this->speedF = 0.0f;
            i_this->field_0x5d4 = 0.0f;
            cXyz sp158 = a_this->current.pos;
            sp158.y = i_this->field_0x63c;

            f32 sc_load_val = 1.0f;
            static cXyz sc(sc_load_val, sc_load_val, sc_load_val);
            static u16 w_eff_id[4] = {
                0x01B8, 0x01B9, 0x01BA, 0x01BB
            };

            for (int i = 0; i < 4; i++) {
                i_this->field_0xbe4[i] = dComIfGp_particle_set(i_this->field_0xbe4[i], w_eff_id[i], &sp158, &a_this->tevStr, NULL,
                                                                &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
            }

            i_this->mSound.startCreatureSound(Z2SE_AL_INTO_WATER, 0, -1);
            return retval;
        }

        if (i_this->mMode < 50 && i_this->field_0xbd8 != 0) {
            i_this->field_0xbd8--;
            i_this->mMode = 50;
            i_this->mTimers[0] = 20 + YREG_S(3);
        }
    } else {
        f32 dbg_f25;
        if (a_this->speedF >= 3.0f) {
            dbg_f25 = 0.2f + KREG_F(1);
        } else {
            dbg_f25 = 0.05f + KREG_F(0);
        }

        cXyz sp164 = a_this->current.pos;
        sp164.y = i_this->field_0x63c;
        fopAcM_effHamonSet(&i_this->field_0xbf4, &sp164, 1.5f + KREG_F(2), dbg_f25);
        if (i_this->field_0xbdd == 0) {
            i_this->mMode = 0;
        }
    }

    action_check(i_this);
    return retval;
}

static void npc_ks_awaydoor(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->mTimers[0] = 30;
            break;

        case 1:
            if (i_this->mTimers[0] == 0) {
                fopAcM_delete(a_this);
            }
    }

    cXyz sp1c, sp28;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cMtx_YrotS(*calc_mtx, player->shape_angle.y);
    sp1c.x = 0.0f;
    sp1c.y = 0.0f;
    sp1c.z = 150.0f;
    MtxPosition(&sp1c, &sp28);
    sp28 += player->current.pos;
    sp1c.x = a_this->current.pos.x - sp28.x;
    sp1c.z = a_this->current.pos.z - sp28.z;
    cLib_addCalcAngleS2(&a_this->current.angle.y, cM_atan2s(sp1c.x, sp1c.z), 1, 0x2000);
    cLib_addCalc2(&a_this->speedF, l_HIO.holding_speed_h, 1.0f, 4.0f);
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
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp2c, sp38;
    int rv = 1;
    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->field_0x904 = 0;
            a_this->speedF = 0.0f;
            // fallthrough
        case 1:
            if (i_this->field_0x5c4 > 700.0f || guide_path_00[i_this->field_0x904].field_0x0 < 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
                a_this->speedF = 0.0f;
            } else {
                i_this->field_0x8f0.x = guide_path_00[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.z = guide_path_00[i_this->field_0x904].field_0xc;
                i_this->mMode = 5;
                a_this->speedF = l_HIO.demo_speed_2;
                if (i_this->field_0x5d0 != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 5:
            sp2c.x = i_this->field_0x8f0.x - a_this->current.pos.x;
            sp2c.z = i_this->field_0x8f0.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp2c.x, sp2c.z);
            if (JMAFastSqrt(sp2c.x * sp2c.x + sp2c.z * sp2c.z) < a_this->speedF * 1.2f) {
                i_this->field_0x904++;
                i_this->mMode = 1;
            }
            break;

        case 10:
            if (i_this->field_0x5d0 == 8) {
                i_this->field_0x8fc.y = i_this->field_0x5c8 + 0x8000;
            } else {
                i_this->field_0x8fc.y = i_this->field_0x5c8;
            }

            if (i_this->field_0x5c4 < 550.0f && guide_path_00[i_this->field_0x904].field_0x0 >= 0) {
                i_this->mMode = 1;
            } else {
                if (i_this->field_0x5c4 < 150.0f && i_this->field_0x5d0 != 51) {
                    anm_init(i_this, 51, 5.0f, 2, 1.0f);
                }

                if (i_this->field_0x5d0 == 51) {
                    i_this->field_0x5fc = 1;
                }

                if (guide_path_00[i_this->field_0x904].field_0x0 < 0 && saru_count_check(i_this) != 0) {
                    sp2c.x = player->current.pos.x - i_this->field_0x934->field_0x904[0].x;
                    sp2c.y = player->current.pos.y - (i_this->field_0x934->field_0x904[0].y - 450.0f);
                    sp2c.z = player->current.pos.z - i_this->field_0x934->field_0x904[0].z;

                    if (sp2c.abs() < 300.0f) {
                        i_this->field_0xbd9 = 1;
                        i_this->mActionID = 20;
                        i_this->mMode = 0;
                        i_this->field_0xaec = 0;
                        i_this->field_0xbc8 = i_this->field_0x934->field_0x904[0];
                        sp2c = i_this->field_0x934->field_0x904[1] - i_this->field_0xbc8;
                        leader->field_0xbd4 = cM_atan2s(sp2c.x, sp2c.z);
                        leader->field_0xb42 = 10;
                        leader->field_0xb40 = i_this->field_0x934->field_0x570;
                        if (i_this->field_0x5b6 != 0) {
                            leader->field_0x934 = i_this->field_0x934;
                            leader->field_0xbc8 = i_this->field_0xbc8;
                        }
                    }
                }
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mMode = 41;
            i_this->mTimers[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if ((i_this->field_0x5d0 == 39 || (i_this->field_0x5d0 == 43 && frame < 7) || (frame > 40 && frame < 48)) || frame > 65) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->mTimers[0] == 0 && i_this->field_0x5d0 == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->field_0x5d0 == 43) {
                if (i_this->mpModelMorf->isStop()) {
                    i_this->mMode = 40;
                }
            }
            break;

        case 42:
            if (i_this->mTimers[0] == 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
            }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 1, 0x1000);
    s16 sVar1 = a_this->current.angle.y - i_this->field_0x8fc.y;
    if (sVar1 > 0x1000 || sVar1 < -0x1000) {
        rv = 0;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->mActionID = 100;
        i_this->mMode = 0;
    }

    if (i_this->mMode >= 40 && i_this->mMode <= 41) {
        fopAc_ac_c* enemy_p = enemy_check(i_this, 800.0f);
        if (enemy_p == NULL) {
            i_this->mMode = 42;
            i_this->mTimers[0] = 30;
        } else {
            sp2c.x = enemy_p->current.pos.x - a_this->current.pos.x;
            sp2c.z = enemy_p->current.pos.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp2c.x, sp2c.z) + 0x8000;
        }
    } else {
        if (enemy_view_check(i_this, 700.0f) != NULL) {
            i_this->mMode = 40;
            a_this->speedF = 0.0f;
        }
    }

    return rv;
}

static int npc_ks_guide_00_2(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp34, sp40;
    int rv = 1;
    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->field_0x904 = 0;
            a_this->speedF = 0.0f;
            // fallthrough
        case 1:
            if (i_this->field_0x5c4 > 900.0f || guide_path_00_2[i_this->field_0x904].field_0x0 < 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
                a_this->speedF = 0.0f;
            } else {
                i_this->field_0x8f0.x = guide_path_00_2[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.z = guide_path_00_2[i_this->field_0x904].field_0xc;
                i_this->mMode = 5;
                a_this->speedF = l_HIO.demo_speed_2;
                if (i_this->field_0x5d0 != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 5:
            sp34.x = i_this->field_0x8f0.x - a_this->current.pos.x;
            sp34.z = i_this->field_0x8f0.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp34.x, sp34.z);
            if (JMAFastSqrt(sp34.x * sp34.x + sp34.z * sp34.z) < a_this->speedF * 1.2f) {
                if (i_this->field_0x5b6 == 0 || i_this->field_0x5b6 == 1) {
                    i_this->field_0x904++;
                }

                i_this->field_0x904++;
                i_this->mMode = 1;
            }
            break;

        case 10:
            if (i_this->field_0x5d0 == 7) {
                i_this->field_0x8fc.y = i_this->field_0x5c8;
            } else {
                i_this->field_0x8fc.y = i_this->field_0x5c8 + 0x8000;
            }

            if (i_this->field_0x5c4 < 750.0f && guide_path_00_2[i_this->field_0x904].field_0x0 >= 0) {
                i_this->mMode = 1;
            }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 1, 0x1000);

    s16 sVar1 = a_this->current.angle.y - i_this->field_0x8fc.y;
    if (sVar1 > 0x1000 || sVar1 < -0x1000) {
        rv = 0;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->mActionID = 100;
        i_this->mMode = 0;
    }

    return rv;
}

static int npc_ks_guide_00_3(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp2c, sp38;
    int rv = 1;
    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->field_0x904 = 0;
            a_this->speedF = 0.0f;
            // fallthrough
        case 1:
            if (i_this->field_0x5c4 > 500.0f || guide_path_00_3[i_this->field_0x904].field_0x0 < 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
                a_this->speedF = 0.0f;
            } else {
                i_this->field_0x8f0.x = guide_path_00_3[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.z = guide_path_00_3[i_this->field_0x904].field_0xc;
                i_this->mMode = 5;
                a_this->speedF = l_HIO.demo_speed_2;
                if (i_this->field_0x5d0 != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 5:
            sp2c.x = i_this->field_0x8f0.x - a_this->current.pos.x;
            sp2c.z = i_this->field_0x8f0.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp2c.x, sp2c.z);
            if (JMAFastSqrt(sp2c.x * sp2c.x + sp2c.z * sp2c.z) < a_this->speedF * 1.2f) {
                i_this->field_0x904++;
                i_this->mMode = 1;
            }
            break;

        case 10:
            if (i_this->field_0x5d0 == 8) {
                i_this->field_0x8fc.y = i_this->field_0x5c8 + 0x8000;
            } else {
                i_this->field_0x8fc.y = i_this->field_0x5c8;
            }

            if (i_this->field_0x5c4 < 400.0f && guide_path_00_3[i_this->field_0x904].field_0x0 >= 0) {
                i_this->mMode = 1;
            } else {
                if (i_this->field_0x5c4 < 150.0f && i_this->field_0x5d0 != 51) {
                    anm_init(i_this, 51, 5.0f, 2, 1.0f);
                }

                if (i_this->field_0x5d0 == 51) {
                    i_this->field_0x5fc = 1;
                }
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mMode = 41;
            i_this->mTimers[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if (i_this->field_0x5d0 == 39 || (i_this->field_0x5d0 == 43 && frame < 7) || ((frame > 40 && frame < 48) || frame > 65)) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->mTimers[0] == 0 && i_this->field_0x5d0 == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->field_0x5d0 == 43) {
                if (i_this->mpModelMorf->isStop()) {
                    i_this->mMode = 40;
                }
            }
            break;

        case 42:
            if (i_this->mTimers[0] == 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
            }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->mActionID = 100;
        i_this->mMode = 0;
    }

    s16 sVar1 = a_this->current.angle.y - i_this->field_0x8fc.y;
    if (sVar1 > 0x1000 || sVar1 < -0x1000) {
        rv = 0;
    }

    if (i_this->mMode >= 40 && i_this->mMode <= 41) {
        fopAc_ac_c* enemy_p = enemy_check(i_this, 700.0f);
        if (enemy_p == NULL) {
            i_this->mMode = 42;
            i_this->mTimers[0] = 30;
        } else {
            sp2c.x = enemy_p->current.pos.x - a_this->current.pos.x;
            sp2c.z = enemy_p->current.pos.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp2c.x, sp2c.z) + 0x8000;
        }
    } else if (enemy_view_check(i_this, 600.0f) != NULL) {
        i_this->mMode = 40;
        a_this->speedF = 0.0f;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 1, 0x1000);
    return rv;
}

static int npc_ks_guide_01(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp3c, sp48;
    int rv = 1;
    int frame = i_this->mpModelMorf->getFrame();

    f32 fVar1 = 0.0f;
    if (fopAcM_GetRoomNo(a_this) == 1 && i_this->field_0x904 >= 5) {
        fVar1 = 300.0f;
    }

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->field_0x904 = 0;
            a_this->speedF = 0.0f;
            // fallthrough
        case 1:
        case 101:
            if (i_this->mMode == 1 && (i_this->field_0x5c4 > 900.0f || guide_path_01[i_this->field_0x904].field_0x0 < 0)) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
                a_this->speedF = 0.0f;
            } else if (guide_path_01[i_this->field_0x904].field_0x0 != 0) {
                i_this->field_0x8f0.x = guide_path_01[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.y = guide_path_01[i_this->field_0x904].field_0x8;
                i_this->field_0x8f0.z = guide_path_01[i_this->field_0x904].field_0xc;
                i_this->field_0x910 = a_this->current.pos;
                i_this->field_0x91c = i_this->field_0x8f0;
                i_this->mMode = 2;
                anm_init(i_this, 32, 2.0f, 0, 1.0f);
                a_this->speedF = 0.0f;
            } else {
                i_this->field_0x8f0.x = guide_path_01[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.z = guide_path_01[i_this->field_0x904].field_0xc;
                i_this->mMode = 5;
                a_this->speedF = l_HIO.demo_speed_2;
                if (i_this->field_0x5d0 != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 2:
            rv = 2;
            if (i_this->field_0x5d0 == 32 && i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                a_this->speedF = 40.0f;
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                i_this->field_0xbe0 = 1;
            }

            sp3c = i_this->field_0x8f0 - a_this->current.pos;
            i_this->field_0x8fc.y = cM_atan2s(sp3c.x, sp3c.z);
            i_this->field_0x8fc.x = -cM_atan2s(sp3c.y, JMAFastSqrt(sp3c.x * sp3c.x + sp3c.z * sp3c.z));
            if (sp3c.abs() < a_this->speedF * 1.2f) {
                a_this->current.pos = i_this->field_0x8f0;
                i_this->mMode = 1;
                i_this->field_0xbe0 = 1;
                i_this->field_0x904++;
            }
            break;

        case 5:
            sp3c.x = i_this->field_0x8f0.x - a_this->current.pos.x;
            sp3c.z = i_this->field_0x8f0.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp3c.x, sp3c.z);
            if (JMAFastSqrt(sp3c.x * sp3c.x + sp3c.z * sp3c.z) < a_this->speedF * 1.2f) {
                i_this->field_0x904++;
                i_this->mMode = 1;
            }
            break;

        case 10:
            if (i_this->field_0x5d0 == 8) {
                i_this->field_0x8fc.y = i_this->field_0x5c8 + 0x8000;
            } else {
                i_this->field_0x8fc.y = i_this->field_0x5c8;
            }

            if (guide_path_01[i_this->field_0x904].field_0x0 >= 0) {
                if (i_this->field_0x5c4 < 600.0f) {
                    i_this->mMode = 1;
                }

                fopAc_ac_c* player = dComIfGp_getPlayer(0);
                sp3c.x = player->current.pos.x - 9200.0f;
                sp3c.z = player->current.pos.z - 4839.0f;
                sp48.x = a_this->current.pos.x - 9200.0f;
                sp48.z = a_this->current.pos.z - 4839.0f;

                if ((sp3c.x * sp3c.x + sp3c.z * sp3c.z) < (sp48.x * sp48.x + sp48.z * sp48.z)) {
                    i_this->mMode = 101;
                }
            } else if (i_this->field_0x5c4 < 150.0f) {
                i_this->mActionID = 100;
                i_this->mMode = 0;
                return 1;
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mMode = 41;
            i_this->mTimers[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if (i_this->field_0x5d0 == 39 || (i_this->field_0x5d0 == 43 && frame < 7) || ((frame > 40 && frame < 48) ) || frame > 65) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->mTimers[0] == 0 && i_this->field_0x5d0 == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->field_0x5d0 == 43 && i_this->mpModelMorf->isStop()) {
                i_this->mMode = 40;
            }
            break;

        case 42:
            if (i_this->mTimers[0] == 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
            }
            break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->mActionID = 100;
        i_this->mMode = 0;
    }

    if (a_this->speedF > 30.0f) {
        i_this->field_0x90c = 1;
    } else {
        s16 sVar1 = a_this->current.angle.y - i_this->field_0x8fc.y;
        if (sVar1 > 0x1000 || sVar1 < -0x1000) {
            rv = 0;
        }
    }

    if (i_this->mMode >= 40 && i_this->mMode <= 41) {
        fopAc_ac_c* enemy_p = enemy_check(i_this, fVar1 + 700.0f);
        if (enemy_p == NULL) {
            i_this->mMode = 42;
            i_this->mTimers[0] = 30;
        } else {
            sp3c.x = enemy_p->current.pos.x - a_this->current.pos.x;

            sp3c.z = enemy_p->current.pos.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp3c.x, sp3c.z) + 0x8000;
        }
    } else if (enemy_view_check(i_this, fVar1 + 600.0f) != NULL) {
        i_this->mMode = 40;
        a_this->speedF = 0.0f;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 1, 0x1000);

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
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp3c, sp48;
    int rv = 1;
    f32 fVar1;
    int frame_int = i_this->mpModelMorf->getFrame();
    if (i_this->field_0x5b6 == 0) {
        fVar1 = 1000.0f + YREG_F(5);
    } else {
        fVar1 = 700.0f + YREG_F(6);
    }

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mMode = 1;
            if (i_this->field_0x5b6 == 0) {
                i_this->field_0x904 = 0;
            } else {
                i_this->field_0x904 = 11;
            }

            a_this->speedF = 0.0f;
            // fallthrough
        case 1:
            if (i_this->mTimers[2] == 0 && (((player->current.pos.y - a_this->current.pos.y) < 10.0f && i_this->field_0x5c4 > fVar1) || 
                guide_path_02[i_this->field_0x904].field_0x0 < 0)) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
                a_this->speedF = 0.0f;
            } else {
                i_this->field_0x8f0.x = guide_path_02[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.z = guide_path_02[i_this->field_0x904].field_0xc;
                i_this->mMode = 5;
                a_this->speedF = l_HIO.demo_speed_2;
                if (i_this->field_0x5d0 != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 5:
            if (i_this->mObjAcch.ChkWallHit() != 0) {
                a_this->speed.y = 20.0f;
            }

            sp3c.x = i_this->field_0x8f0.x - a_this->current.pos.x;
            sp3c.z = i_this->field_0x8f0.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp3c.x, sp3c.z);
            if (JMAFastSqrt(sp3c.x * sp3c.x + sp3c.z * sp3c.z) < a_this->speedF * 1.2f) {
                i_this->field_0x904++;
                i_this->mMode = 1;
            }
            break;

        case 10:
            if (i_this->field_0x5d0 == 7) {
                i_this->field_0x8fc.y = i_this->field_0x5c8;
            } else {
                i_this->field_0x8fc.y = i_this->field_0x5c8 + 0x8000;
            }

            if (((player->current.pos.y - a_this->current.pos.y) > 10.0f || i_this->field_0x5c4 < (fVar1 - 150.0f)) && guide_path_02[i_this->field_0x904].field_0x0 >= 0) {
                i_this->mMode = 1;
            }

            if (guide_path_02[i_this->field_0x904].field_0x0 < 0 && i_this->field_0x5b6 == 0 && saru_count_check(i_this) != 0) {
                fopAc_ac_c* player_2 = (fopAc_ac_c*) dComIfGp_getPlayer(0);
                int iVar1 = 0;
                for (int i = 0; i < i_this->field_0x934->field_0x91c; i++) {
                    sp3c = player_2->current.pos - saru_p[i]->actor.current.pos;
                    if (sp3c.abs() > 400.0f) {
                        iVar1 = 1;
                    }
                }

                sp3c.x = player->current.pos.x - i_this->field_0x934->field_0x904[0].x;
                sp3c.y = player->current.pos.y - (i_this->field_0x934->field_0x904[0].y - 450.0f);
                sp3c.z = player->current.pos.z - i_this->field_0x934->field_0x904[0].z;

                if (iVar1 == 0 && sp3c.abs() < 300.0f) {
                    i_this->field_0xbd9 = 1;
                    i_this->field_0xbc8 = i_this->field_0x934->field_0x904[0];
                    i_this->mActionID = 20;
                    i_this->mMode = 0;
                    i_this->field_0xaec = 0;
                    leader->field_0xb42 = 10;
                    leader->field_0xb40 = 0;
                    leader->field_0xbc8 = i_this->field_0xbc8;

                    sp3c = i_this->field_0x934->field_0x904[1] - i_this->field_0xbc8;
                    leader->field_0xbd4 = cM_atan2s(sp3c.x, sp3c.z);
                    saru_count_check(saru_p[1]);
                    saru_p[1]->mActionID = 22;
                    saru_p[1]->mMode = 0;
                    saru_p[1]->field_0x930 = leader;
                }
            }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 1, 0x1000);

    s16 sVar1 = a_this->current.angle.y - i_this->field_0x8fc.y;
    if (sVar1 > 0x1000 || sVar1 < -0x1000) {
        rv = 0;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->mActionID = 100;
        i_this->mMode = 0;
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
    fopAc_ac_c* a_this = &i_this->actor;
    i_this->field_0x5c4 = fopAcM_searchPlayerDistance(a_this);
    cXyz sp28, sp34;
    int rv = 1;
    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->field_0x904 = 0;
            a_this->speedF = 0.0f;
            // fallthrough
        case 1:
            if (i_this->field_0x5c4 > 500.0f || guide_path_22[i_this->field_0x904].field_0x0 < 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
                a_this->speedF = 0.0f;
            } else if (guide_path_22[i_this->field_0x904].field_0x0 != 0) {
                anm_init(i_this, 12, 3.0f, 2, 1.0f);
                i_this->field_0x8f0.x = guide_path_22[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.y = guide_path_22[i_this->field_0x904].field_0x8;
                i_this->field_0x8f0.z = guide_path_22[i_this->field_0x904].field_0xc;
                i_this->mMode = 2;
                a_this->speedF = 0.0f;
            } else {
                i_this->field_0x8f0.x = guide_path_22[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.z = guide_path_22[i_this->field_0x904].field_0xc;
                i_this->mMode = 5;
                a_this->speedF = l_HIO.demo_speed_2;
                
                if (i_this->field_0x5d0 != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 2:
            rv = 0;
            
            sp28.x = i_this->field_0x8f0.x - a_this->current.pos.x;
            sp28.z = i_this->field_0x8f0.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp28.x, sp28.z);
            cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x8f0.y, 1.0f, 15.0f);

            if (a_this->current.pos.y >= (i_this->field_0x8f0.y - 110.0f) && i_this->field_0x5d0 != 26) {
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
            }

            if (a_this->current.pos.y >= (i_this->field_0x8f0.y - 50.0f)) {
                cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x8f0.x, 1.0f, 20.0f);
                cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x8f0.z, 1.0f, 20.0f);
            }

            if (a_this->current.pos.y >= i_this->field_0x8f0.y) {
                a_this->current.pos = i_this->field_0x8f0;
                i_this->field_0x904++;
                i_this->mMode = 1;
            }
            break;

        case 5:
            sp28.x = i_this->field_0x8f0.x - a_this->current.pos.x;
            sp28.z = i_this->field_0x8f0.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp28.x, sp28.z);

            if (JMAFastSqrt(sp28.x * sp28.x + sp28.z * sp28.z) < a_this->speedF * 1.2f) {
                i_this->field_0x904++;
                i_this->mMode = 1;
            }
            break;

        case 10:
            if (i_this->field_0x5d0 == 8) {
                i_this->field_0x8fc.y = i_this->field_0x5c8 + 0x8000;
            } else {
                i_this->field_0x8fc.y = i_this->field_0x5c8;
            }

            if (i_this->field_0x5c4 < 300.0f && guide_path_22[i_this->field_0x904].field_0x0 >= 0) {
                i_this->mMode = 1;
            }

            if (i_this->field_0x5c4 < 150.0f && i_this->field_0x5d0 != 51) {
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
            }

            if (i_this->field_0x5d0 == 51) {
                i_this->field_0x5fc = 1;
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mMode = 41;
            i_this->mTimers[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if ((i_this->field_0x5d0 == 39 || (i_this->field_0x5d0 == 43 && frame < 7) || (frame > 40 && frame < 48)) || frame > 65) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->mTimers[0] == 0 && i_this->field_0x5d0 == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->field_0x5d0 == 43 && i_this->mpModelMorf->isStop()) {
                i_this->mMode = 40;
            }
            break;

        case 42:
            if (i_this->mTimers[0] == 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
            }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->mActionID = 100;
        i_this->mMode = 0;
    }

    if (a_this->speedF > 30.0f) {
        i_this->field_0x90c = 1;
    } else {
        s16 sVar1 = a_this->current.angle.y - i_this->field_0x8fc.y;
        if (sVar1 > 0x1000 || sVar1 < -0x1000) {
            rv = 0;
        }
    }

    if (i_this->mMode >= 40 && i_this->mMode <= 41) {
        fopAc_ac_c* enemy_p = enemy_check(i_this, 700.0f);
        if (enemy_p == NULL) {
            i_this->mMode = 42;
            i_this->mTimers[0] = 30;
        } else {
            sp28.x = enemy_p->current.pos.x - a_this->current.pos.x;
            sp28.z = enemy_p->current.pos.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp28.x, sp28.z) + 0x8000;
        }
    } else if (enemy_view_check(i_this, 600.0f) != NULL) {
        i_this->mMode = 40;
        a_this->speedF = 0.0f;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 1, 0x1000);
    
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
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp40, sp4c;
    int rv = 1;
    int frame = i_this->mpModelMorf->getFrame();
    f32 fVar1 = 0.0f;

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->field_0x904 = 0;
            a_this->speedF = 0.0f;
            // fallthrough
        case 1:
            if (i_this->field_0x5c4 > 900.0f || guide_path_09[i_this->field_0x904].field_0x0 < 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
                a_this->speedF = 0.0f;
            } else if (guide_path_09[i_this->field_0x904].field_0x0 != 0) {
                i_this->field_0x8f0.x = guide_path_09[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.y = guide_path_09[i_this->field_0x904].field_0x8;
                i_this->field_0x8f0.z = guide_path_09[i_this->field_0x904].field_0xc;
                i_this->field_0x910 = a_this->current.pos;
                i_this->field_0x91c = i_this->field_0x8f0;
                i_this->mMode = 2;
                anm_init(i_this, 32, 2.0f, 0, 1.0f);
                a_this->speedF = 0.0f;
            } else {
                i_this->field_0x8f0.x = guide_path_09[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.z = guide_path_09[i_this->field_0x904].field_0xc;
                i_this->mMode = 5;
                a_this->speedF = l_HIO.demo_speed_2;
                
                if (i_this->field_0x5d0 != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 2:
            rv = 2;
            if (i_this->field_0x5d0 == 32 && i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                a_this->speedF = 40.0f;
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
            }

            sp40 = i_this->field_0x8f0 - a_this->current.pos;
            i_this->field_0x8fc.y = cM_atan2s(sp40.x, sp40.z);
            i_this->field_0x8fc.x = -cM_atan2s(sp40.y, JMAFastSqrt(sp40.x * sp40.x + sp40.z * sp40.z));

            if (sp40.abs() < a_this->speedF * 1.2f) {
                a_this->current.pos = i_this->field_0x8f0;
                i_this->mMode = 1;
                i_this->field_0x904++;
            }
            break;

        case 5:
            sp40.x = i_this->field_0x8f0.x - a_this->current.pos.x;
            sp40.z = i_this->field_0x8f0.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp40.x, sp40.z);

            if (JMAFastSqrt(sp40.x * sp40.x + sp40.z * sp40.z) < a_this->speedF * 1.2f) {
                i_this->field_0x904++;
                i_this->mMode = 1;
            }
            break;

        case 10:
            if (i_this->field_0x5d0 == 8) {
                i_this->field_0x8fc.y = i_this->field_0x5c8 + 0x8000;
            } else {
                i_this->field_0x8fc.y = i_this->field_0x5c8;
            }

            if (guide_path_09[i_this->field_0x904].field_0x0 >= 0) {
                if (i_this->field_0x5c4 < 600.0f) {
                    i_this->mMode = 1;
                }
            } else {
                if (i_this->field_0x5c4 < 150.0f && i_this->field_0x5d0 != 51) {
                    anm_init(i_this, 51, 5.0f, 2, 1.0f);
                }
                
                if (i_this->field_0x5d0 == 51) {
                    i_this->field_0x5fc = 1;
                }
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mMode = 41;
            i_this->mTimers[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if ((i_this->field_0x5d0 == 39 || (i_this->field_0x5d0 == 43 && frame < 7) || (frame > 40 && frame < 48)) || frame > 65) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->mTimers[0] == 0 && i_this->field_0x5d0 == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->field_0x5d0 == 43 && i_this->mpModelMorf->isStop()) {
                i_this->mMode = 40;
            }
            break;

        case 42:
            if (i_this->mTimers[0] == 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
            } 
    }

        cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        i_this->mActionID = 100;
        i_this->mMode = 0;
    }

    if (a_this->speedF > 30.0f) {
        i_this->field_0x90c = 1;
    } else {
        s16 sVar1 = a_this->current.angle.y - i_this->field_0x8fc.y;
        if (sVar1 > 0x1000 || sVar1 < -0x1000) {
            rv = 0;
        }
    }

    if (i_this->mMode >= 40 && i_this->mMode <= 41) {
        fopAc_ac_c* enemy_p = enemy_check(i_this, 700.0f);
        if (enemy_p == NULL) {
            i_this->mMode = 42;
            i_this->mTimers[0] = 30;
        } else {
            sp40.x = enemy_p->current.pos.x - a_this->current.pos.x;
            sp40.z = enemy_p->current.pos.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp40.x, sp40.z) + 0x8000;
        }
    } else if (enemy_view_check(i_this, 600.0f) != NULL) {
        i_this->mMode = 40;
        a_this->speedF = 0.0f;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 1, 0x1000);
    
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
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp48, sp54;
    int rv = 1;
    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
        case 0:
            if (i_this->field_0xb42 >= 79) {
                anm_init(i_this, 26, 3.0f, 2, 1.0f);
                i_this->mMode = 1;
                i_this->field_0x904 = 0;
            }

            a_this->speedF = 0.0f;
            break;

        case 1:
            if (guide_path_12[i_this->field_0x904].field_0x0 != 0) {
                i_this->field_0x8f0.x = guide_path_12[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.y = guide_path_12[i_this->field_0x904].field_0x8;
                i_this->field_0x8f0.z = guide_path_12[i_this->field_0x904].field_0xc;
                i_this->field_0x910 = a_this->current.pos;
                i_this->field_0x91c = i_this->field_0x8f0;
                i_this->mMode = 2;
                anm_init(i_this, 32, 2.0f, 0, 1.0f);
                a_this->speedF = 0.0f;
            } else {
                i_this->field_0x8f0.x = guide_path_12[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.z = guide_path_12[i_this->field_0x904].field_0xc;
                i_this->mMode = 5;
                a_this->speedF = l_HIO.demo_speed_2;

                if (i_this->field_0x5d0 != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 2:
            rv = 2;
            if (i_this->field_0x5d0 == 32 && i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                a_this->speedF = 40.0f;
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                i_this->field_0xbe0 = 1;
            }

            sp48 = i_this->field_0x8f0 - a_this->current.pos;
            i_this->field_0x8fc.y = cM_atan2s(sp48.x, sp48.z);
            i_this->field_0x8fc.x = -cM_atan2s(sp48.y, JMAFastSqrt(sp48.x * sp48.x + sp48.z * sp48.z));

            if (sp48.abs() < a_this->speedF * 1.2f) {
                a_this->current.pos = i_this->field_0x8f0;
                i_this->mMode = 1;
                i_this->field_0xbe0 = 1;
                i_this->field_0x904++;
            }
            break;

        case 5:
            sp48.x = i_this->field_0x8f0.x - a_this->current.pos.x;
            sp48.z = i_this->field_0x8f0.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp48.x, sp48.z);

            if (JMAFastSqrt(sp48.x * sp48.x + sp48.z * sp48.z) < a_this->speedF * 1.2f) {
                i_this->field_0x904++;
                i_this->mMode = 1;
            }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x800);

    if (a_this->speedF > 30.0f) {
        i_this->field_0x90c = 1;
    } else {
        s16 sVar1 = a_this->current.angle.y - i_this->field_0x8fc.y;
        if (sVar1 > 0x1000 || sVar1 < -0x1000) {
            rv = 0;
        }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 1, 0x1000);

    if (guide_path_12[i_this->field_0x904].field_0x0 < 0) {
        i_this->mActionID = 8;
        a_this->speedF = 0.0f;
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
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp50;
    int rv = 1;
    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 26, 3.0f, 2, 1.0f);
            i_this->mMode = 1;
            i_this->field_0x904 = 0;
            a_this->speedF = 0.0f;
            // fallthrough
        case 1:
            if (player->current.pos.z > a_this->current.pos.z && (i_this->field_0x5c4 > 700.0f || guide_path_0409[i_this->field_0x904].field_0x0 < 0)) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
                a_this->speedF = 0.0f;
            } else {
                i_this->field_0x8f0.x = guide_path_0409[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.z = guide_path_0409[i_this->field_0x904].field_0xc;
                i_this->mMode = 5;
                a_this->speedF = l_HIO.demo_speed_2;

                if (i_this->field_0x5d0 != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 5:
            sp50.x = i_this->field_0x8f0.x - a_this->current.pos.x;
            sp50.z = i_this->field_0x8f0.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp50.x, sp50.z);

            if (JMAFastSqrt(sp50.x * sp50.x + sp50.z * sp50.z) < a_this->speedF * 1.2f) {
                i_this->field_0x904++;
                i_this->mMode = 1;
            }
            break;

        case 10:
            if (i_this->field_0x5d0 == 8) {
                i_this->field_0x8fc.y = i_this->field_0x5c8 + 0x8000;
            } else {
                i_this->field_0x8fc.y = i_this->field_0x5c8;
            }

            if ((i_this->field_0x5c4 < 550.0f || player->current.pos.z < a_this->current.pos.z)
                && guide_path_0409[i_this->field_0x904].field_0x0 >= 0) {
                i_this->mMode = 1;
            } else {
                if (i_this->field_0x5c4 < 150.0f && i_this->field_0x5d0 != 51) {
                    anm_init(i_this, 51, 5.0f, 2, 1.0f);
                }

                if (i_this->field_0x5d0 == 51) {
                    i_this->field_0x5fc = 1;
                }
            }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 1, 0x1000);

    s16 sVar1 = a_this->current.angle.y - i_this->field_0x8fc.y;
    if (sVar1 > 0x1000 || sVar1 < -0x1000) {
        rv = 0;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.x, 0, 1, 0x800);

    if (checkDoorDemo()) {
        f32 fVar1 = 7280.0f - player->current.pos.x;
        f32 fVar2 = -2709.0f - player->current.pos.z;
        if (JMAFastSqrt(fVar1 * fVar1 + fVar2 * fVar2) < 500.0f) {
            i_this->mActionID = 100;
            i_this->mMode = 0;
        }
    }

    return rv;
}

static int path_search(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    int path_index = 0;
    while (path_index < 255) {
        dPath* path_p = dPath_GetRoomPath(path_index, fopAcM_GetRoomNo(a_this));
        if (path_p != NULL) {
            cXyz sp28 = a_this->current.pos - path_p->m_points->m_position;
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
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp44, sp50;
    int rv = 1;
    int frame = i_this->mpModelMorf->getFrame();
    f32 fVar2 = 1.25f + TREG_F(17);
    f32 fVar1 = l_HIO.link_approach_distance;

    switch (i_this->mMode) {
        case 0:
                /* dSv_event_flag_c::F_0225 - Faron Woods - Lanter guide monkey doesn't come out a second time */
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[225])) {
                     /* dSv_event_flag_c::F_0226 - Faron Woods - Get lantern back from monkey */
                if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[226])) {
                    i_this->mMode = 302;
                    i_this->field_0xc17 = 3;
                    dComIfGp_setItemOilCount(-(dComIfGs_getMaxOil() & 0xFFFF));
                }
            } else {
                i_this->field_0xaec = 1;
                    /* dSv_event_flag_c::F_0224 - Faron Woods - Flag for lantern guide monkey cutscene */
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[224])) {
                    i_this->mMode = 3;
                    call_pt = 0;
                    i_this->field_0xc17 = 2;
                    a_this->current.angle.y = player->shape_angle.y + 0x8000;
                    i_this->field_0x8fc.y = a_this->current.angle.y;
                } else {
                    a_this->current.pos.y = a_this->home.pos.y + 10000.0f;
                    a_this->speed.y = 0.0f;

                    if (!daPy_getPlayerActorClass()->checkKandelaarSwing(1)) {
                        if (!daPy_getPlayerActorClass()->getKandelaarFlamePos()) break;
                        if (!daPy_getPlayerActorClass()->checkUseKandelaar(0)) break;
                    }

                    sp44.set(-27426.0f, player->current.pos.y, -20000.0f);
                    sp44 -= player->current.pos;
                    if (sp44.abs() < 7500.0f) {
                        i_this->mMode = 1;
                        i_this->mTimers[0] = 10;
                    }
                }
            }
            break;

        case 1:
            a_this->current.pos.y = a_this->home.pos.y + 10000.0f;
            a_this->speed.y = 0.0f;
            if (i_this->mTimers[0] != 1) break;

            if (daPy_getPlayerActorClass()->checkKandelaarSwing(1) || (daPy_getPlayerActorClass()->getKandelaarFlamePos() != NULL && 
                daPy_getPlayerActorClass()->checkUseKandelaar(0))) {
                i_this->field_0xb42 = 200;
                anm_init(i_this, 44, 0.0f, 2, 1.0f);
            } else {
                i_this->mMode = 0;
            }
            break;

        case 2:
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            sp44.x = i_this->field_0xbb4;
            sp44.y = 0.0f;
            sp44.z = 100.0f;
            MtxPosition(&sp44, &sp50);
            a_this->current.pos = player->current.pos + sp50;
            a_this->current.angle.y = player->shape_angle.y - 0x4000;
            i_this->field_0x8fc.y = a_this->current.angle.y;
            cLib_addCalc2(&i_this->field_0xbb4, -500.0f, 1.0f, 20.0f);
            break;

        case 230:
            a_this->speedF = 20.0f;
            if (i_this->mTimers[0] == 0) {
                i_this->mMode = 231;
                a_this->speedF = 0.0f;
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
                i_this->mTimers[0] = 30;
            }
            break;

        case 231:
            if (i_this->mTimers[0] == 0) {
                anm_init(i_this, 44, 3.0f, 2, 1.0f);
                i_this->mTimers[0] = 110;
                i_this->mMode = 232;
            }
            break;

        case 232:
            if (i_this->mTimers[0] == 40) {
                anm_init(i_this, 51, 5.0f, 2, 1.0f);
            }

            if (i_this->mTimers[0] == 0) {
                i_this->mMode = 3;
                call_pt = 1;
            }
            break;

        case 3:
            if (path_search(i_this) != 0) {
                anm_init(i_this, 28, 6.0f, 2, 1.0f);
                i_this->mMode = 4;
                i_this->field_0x904 = 1;
                a_this->speedF = 0.0f;
                i_this->mTimers[3] = l_HIO.lantern_swing_interval * 30.0f;
            }
            break;

        case 4:
            if (i_this->field_0x904 >= i_this->field_0xc18->m_num) {
                anm_init(i_this, 7, 5.0f, 2, 1.0f);
                i_this->mMode = 11;
                a_this->speedF = 0.0f;
            } else {
                if (i_this->field_0x5c4 > (fVar1 + 150.0f)) {
                    if ((call_pt & 1) != 0) {
                        anm_init(i_this, 8, 5.0f, 2, 1.0f);
                    } else {
                        anm_init(i_this, 7, 5.0f, 2, 1.0f);
                    }

                    call_pt++;
                    i_this->mMode = 10;
                    a_this->speedF = 0.0f;
                } else {
                    dPnt* pnt_p = i_this->field_0xc18->m_points;
                    pnt_p = &pnt_p[i_this->field_0x904];
                    i_this->field_0x8f0.x = pnt_p->m_position.x;
                    i_this->field_0x8f0.y = pnt_p->m_position.y;
                    i_this->field_0x8f0.z = pnt_p->m_position.z;
                    i_this->mMode = 5;
                    a_this->speedF = l_HIO.guided_movement_speed * fVar2;
                    
                    if (i_this->field_0x5d0 != 28) {
                        anm_init(i_this, 28, 6.0f, 2, 1.0f);
                    }
                }
            }
            break;

        case 5:
            sp44.x = i_this->field_0x8f0.x - a_this->current.pos.x;
            sp44.z = i_this->field_0x8f0.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp44.x, sp44.z);
            
            if (JMAFastSqrt(sp44.x * sp44.x + sp44.z * sp44.z) < 100.0f) {
                i_this->field_0x904++;
                i_this->mMode = 4;
            }
            break;

        case 10:
            if (i_this->field_0x5d0 == 7) {
                i_this->field_0x8fc.y = i_this->field_0x5c8;
            } else {
                i_this->field_0x8fc.y = i_this->field_0x5c8 + 0x8000;
            }

            if (i_this->field_0x5c4 < fVar1) {
                i_this->mMode = 4;
            }
            break;

        case 11:
            if (i_this->field_0x5d0 == 7) {
                i_this->field_0x8fc.y = i_this->field_0x5c8;
            }

            sp44.x = -30900.0f - player->current.pos.x;
            sp44.z = -15756.0f - player->current.pos.z;
            if (JMAFastSqrt(sp44.x * sp44.x + sp44.z * sp44.z) < 1300.0f || i_this->field_0x5c4 < fVar1) {
                i_this->field_0xb42 = 300;
                i_this->mMode = 300;
            }
            break;

        case 300:
            i_this->field_0x8fc.y = i_this->field_0x5c8;
            break;

        case 301:
            sp44.x = -31872.0f - a_this->current.pos.x;
            sp44.z = -15490.0f - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp44.x, sp44.z);
            break;

        case 302:
            a_this->current.pos.y = a_this->home.pos.y + 10000.0f;
            a_this->speed.y = 0.0f;
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mMode = 41;
            i_this->mTimers[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if ((i_this->field_0x5d0 == 39 || (i_this->field_0x5d0 == 43 && frame < 7) || (frame > 40 && frame < 48)) || frame > 65) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->mTimers[0] == 0 && i_this->field_0x5d0 == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->field_0x5d0 == 43 && i_this->mpModelMorf->isStop()) {
                i_this->mMode = 40;
            }
            break;

        case 50:
            anm_init(i_this, 46, 3.0f, 0, 1.0f);
            i_this->mMode = 51;
            break;

        case 51:
            if (i_this->mpModelMorf->isStop()) {
                i_this->mMode = 40;
            }
            break;

        case 53:
            if (i_this->mTimers[0] == 0) {
                if ((call_pt & 1) != 0) {
                    anm_init(i_this, 8, 5.0f, 2, 1.0f);
                } else {
                    anm_init(i_this, 7, 5.0f, 2, 1.0f);
                }

                call_pt++;
                i_this->mMode = 10;
                i_this->mTimers[3] = 0;
            }
            break;

        case 100:
            anm_init(i_this, 44, 3.0f, 2, 1.0f);
            i_this->mMode = 101;
            i_this->mTimers[0] = 80;
            break;

        case 101:
            if (i_this->mTimers[0] == 0) {
                i_this->mMode = 4;
                i_this->mTimers[3] = l_HIO.lantern_swing_interval * 30.0f;
            }
            break;
    }

    if (i_this->mMode != 11 && i_this->mMode > 3 && i_this->mMode < 40 && (i_this->mTimers[3] == 0 || dComIfGs_shake_kandelaar_check() == 1)) {
        i_this->mMode = 100;
        a_this->speedF = 0.0f;
    }

    if (i_this->mMode >= 40 && i_this->mMode <= 52) {
        fopAc_ac_c* enemy_p = enemy_check(i_this, 800.0f);
        if (enemy_p == NULL) {
            i_this->mMode = 53;
            i_this->mTimers[0] = 30;
        }
    } else {
        fopAc_ac_c* enemy_p = enemy_view_check(i_this, 400.0f);
        if (enemy_p != NULL) {
            i_this->mMode = 50;
            a_this->speedF = 0.0f;
            sp44.x = enemy_p->current.pos.x - a_this->current.pos.x;
            sp44.z = enemy_p->current.pos.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp44.x, sp44.z) + 0x8000;
        }
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 2, 0x1000);

    s16 sVar1 = a_this->current.angle.y - i_this->field_0x8fc.y;
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
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);
    cXyz sp2c, sp38;
    int rv = 1;
    int frame = i_this->mpModelMorf->getFrame();
    int iVar1 = 0;
    s16 sVar1 = 0x1000;
    s16 sVar2;

    i_this->field_0x5fc = 0;
    i_this->field_0xaec = 1;

    switch (i_this->mMode) {
        case 0:
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x220] & 0xFFFF)) {
                i_this->mMode = 40;
            } else {
                fpcM_Search(s_fs_sub, i_this);
                a_this->current.pos.set(-39906.0f, 21962.0f, -24243.0f);
                rv = 0;

                if (player->current.pos.z < -21000.0f) {
                    i_this->mMode = 1;
                    a_this->current.pos.set(-39906.0f, 1962.0f, -24243.0f);
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                    i_this->field_0x904 = 0;
                    a_this->speedF = 0.0f;
                    i_this->field_0xb42 = 350;
                    mDoAud_seStart(Z2SE_KOSARU_V_PRISONED, 0, 0, 0);
                }
            }
            break;

        case 1:
            if (guide_path_fs[i_this->field_0x904].field_0x0 != 0) {
                i_this->field_0x8f0.x = guide_path_fs[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.y = guide_path_fs[i_this->field_0x904].field_0x8;
                i_this->field_0x8f0.z = guide_path_fs[i_this->field_0x904].field_0xc;
                i_this->field_0x910 = a_this->current.pos;
                i_this->field_0x91c = i_this->field_0x8f0;
                i_this->mMode = 2;
                anm_init(i_this, 32, 2.0f, 0, 1.0f);
                a_this->speedF = 0.0f;
            } else {
                i_this->field_0x8f0.x = guide_path_fs[i_this->field_0x904].field_0x4;
                i_this->field_0x8f0.z = guide_path_fs[i_this->field_0x904].field_0xc;
                i_this->mMode = 5;
                a_this->speedF = l_HIO.demo_speed_2;

                if (i_this->field_0x5d0 != 26) {
                    anm_init(i_this, 26, 3.0f, 2, 1.0f);
                }
            }
            break;

        case 2:
            rv = 2;
            if (i_this->field_0x5d0 == 32 && i_this->mpModelMorf->isStop()) {
                anm_init(i_this, 33, 1.0f, 0, 1.0f);
                a_this->speedF = 40.0f + TREG_F(9);
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                i_this->field_0xbe0 = 1;
            }

            sp2c = i_this->field_0x8f0 - a_this->current.pos;
            i_this->field_0x8fc.y = cM_atan2s(sp2c.x, sp2c.z);
            i_this->field_0x8fc.x = -cM_atan2s(sp2c.y, JMAFastSqrt(sp2c.x * sp2c.x + sp2c.z * sp2c.z));

            if (sp2c.abs() < a_this->speedF * 1.2f) {
                a_this->current.pos = i_this->field_0x8f0;

                if (i_this->field_0x904 == 0) {
                    anm_init(i_this, 45, 3.0f, 0, 1.0f);
                    i_this->mMode = 3;
                    i_this->field_0x8fc.y += 0x8000;
                    a_this->speedF = 0.0f;
                } else {
                    i_this->mMode = 1;
                }

                i_this->field_0xbe0 = 1;
                i_this->field_0x904++;
            }
            break;

        case 3:
            if (i_this->mpModelMorf->isStop()) {
                i_this->field_0x8fc.y += 0x8000;
                i_this->mMode = 1;
            }
            break;

        case 5:
            sp2c.x = i_this->field_0x8f0.x - a_this->current.pos.x;
            sp2c.z = i_this->field_0x8f0.z - a_this->current.pos.z;
            i_this->field_0x8fc.y = cM_atan2s(sp2c.x, sp2c.z);

            if (JMAFastSqrt(sp2c.x * sp2c.x + sp2c.z * sp2c.z) < a_this->speedF * 1.2f) {
                i_this->field_0x904++;
                i_this->mMode = 1;

                if (guide_path_fs[i_this->field_0x904].field_0x0 < 0) {
                    i_this->mMode = 10;
                    a_this->speedF = 0.0f;
                    anm_init(i_this, 51, 5.0f, 2, 1.0f);
                }
            }
            break;

        case 10:
            a_this->speedF = 0.0f;
            break;

        case 20:
            i_this->mMode = 21;
            anm_init(i_this, 51, 10.0f, 2, 1.0f);
            // fallthrough
        case 21: {
            i_this->field_0x8fc.y = i_this->field_0x5c8;
            sVar1 = 0;
            i_this->field_0x5fc = 1;

            if (i_this->mTimers[2] != 0) {
                i_this->field_0x93c = 10;
                i_this->field_0x940.set(-37799.0f, 815.0f, -22323.0f);
                i_this->field_0x8fc.y -= (s16) 0x3000;
                sVar1 = 0x800;
            }

            iVar1 = 1;
            sVar2 = a_this->current.angle.y - i_this->field_0x5c8;
            if ((sVar2 > 0x3000 || sVar2 < -0x3000) && i_this->mMode < 22) {
                anm_init(i_this, 28, 3.0f, 0, 1.0f);
                i_this->mMode = 22;
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_WALK, -1);
            }

            break;
        }

        case 22:
            sVar1 = 0x800;
            i_this->field_0x8fc.y = i_this->field_0x5c8;

            if (frame >= 11) {
                i_this->mMode = 20;
            }
            break;

        case 40:
            anm_init(i_this, 39, 5.0f, 2, 1.0f);
            i_this->mMode = 41;
            i_this->mTimers[0] = cM_rndF(80.0f) + 100.0f;
            break;

        case 41:
            if ((i_this->field_0x5d0 == 39 || (i_this->field_0x5d0 == 43 && frame < 7) || (frame > 40 && frame < 48)) || frame > 65) {
                i_this->field_0x5e0 = 4;
            }

            if (i_this->mTimers[0] == 0 && i_this->field_0x5d0 == 39) {
                anm_init(i_this, 43, 5.0f, 0, 1.0f);
            }

            if (i_this->field_0x5d0 == 43) {
                if (i_this->field_0xb42 >= 360 && i_this->mpModelMorf->checkFrame(57.0f)) {
                    i_this->mMode = 20;
                } else if (i_this->mpModelMorf->isStop()) {
                    i_this->mMode = 40;
                }
            }
            break;

        case 42:
            if (i_this->mTimers[1] == 1) {
                i_this->field_0xb42 = 360;
            }
    }

    if ((i_this->mMode == 40 || i_this->mMode == 41) && fopAcM_SearchByName(PROC_E_FS) == NULL && i_this->field_0xb42 == 0) {
        i_this->mMode = 42;
        i_this->mTimers[1] = 30;
    }

    if (a_this->speedF > 25.0f) {
        i_this->field_0x90c = 1;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x8fc.y, 2, sVar1);

    if (iVar1 != 0) {
        fopAcM_OnStatus(a_this, 0);
        cLib_onBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
        a_this->eventInfo.onCondition(dEvtCnd_CANTALK_e);
        // TODO: gameInfo fake match to force reuse of pointer
        dComIfG_play_c* play = &g_dComIfG_gameInfo.play;
        if (play->getEvent().runCheck()) {
            if (a_this->eventInfo.checkCommandTalk()) {
                if (i_this->field_0xaee == 0) {
                    i_this->mMsgFlow.init(a_this, 0x74, 0, NULL);
                    i_this->field_0xaee = 1;
                }

                if (i_this->mMsgFlow.doFlow(a_this, NULL, 0)) {
                    play->getEvent().reset();
                    i_this->field_0xaee = 0;
                }
            }
        } else {
            i_this->field_0xaee = 0;
        }
    } else {
        fopAcM_OffStatus(a_this, 0);
        cLib_offBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
        i_this->field_0xaee = 0;
    }

    return rv;
}

static void npc_ks_kago(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    i_this->field_0x5fc = 0;

    switch (i_this->mMode) {
        case 0:
            anm_init(i_this, 34, 3.0f, 2, 1.0f);
            i_this->mMode = 1;
            // fallthrough
        case 1:
            break;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5c8, 2, 0x2000);
    i_this->field_0xaec = 0;

        /* dSv_event_flag_c::F_0577 - Ordon Village - 2nd Day - Retrieved basket from monkey (hit hawk) */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x241])) {
        fopAcM_delete(a_this);
    }
}

static void anm_se_set(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    if (i_this->field_0x5d0 == 17) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_WAIT, -1);
        }

        if (i_this->mpModelMorf->checkFrame(5.0f) || i_this->mpModelMorf->checkFrame(16.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_HAND_CLAP, 0, -1);
        }
    } else if (i_this->field_0x5d0 == 55) {
        if (i_this->mpModelMorf->checkFrame(9.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_HAND_CLAP, 0, -1);
        }
    } else if (i_this->field_0x5d0 == 11) {
        if (i_this->mpModelMorf->checkFrame(10.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_HAND_CLAP, 0, -1);
        }
    } else if (i_this->field_0x5d0 == 56) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_DELIGHT, -1);
        }

        if (i_this->mpModelMorf->checkFrame(24.0f) || i_this->mpModelMorf->checkFrame(11.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_HAND_CLAP, 0, -1);
        }
    } else if (i_this->field_0x5d0 == 12) {
        if (i_this->mpModelMorf->checkFrame(3.0f) || i_this->mpModelMorf->checkFrame(10.0f)) {
            if (fopAcM_GetRoomNo(a_this) == 22) {
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_FT_TSUTA, 0, -1);
            } else {
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_CLIMB, 0, -1);
            }
        }
    } else if (i_this->field_0x5d0 == 15) {
        if (i_this->mpModelMorf->checkFrame(3.0f) || i_this->mpModelMorf->checkFrame(10.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_CLIMB, 0, -1);
        }
    } else if (i_this->field_0x5d0 == 25) {
        if (i_this->mpModelMorf->checkFrame(19.0f) || i_this->mpModelMorf->checkFrame(36.0f) || i_this->mpModelMorf->checkFrame(54.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_POINT, -1);
        }
    } else if (i_this->field_0x5d0 == 26) {
        if (i_this->mpModelMorf->checkFrame(1.0f) && i_this->field_0xc17 == 2) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_WALK, -1);
        }

        if (i_this->mpModelMorf->checkFrame(2.0f)) {
            if (i_this->mActionID == 203 && i_this->mMode <= 3) {
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_FT_BRIDGE, 0, -1);
            } else {
                i_this->mSound.startCreatureSound(Z2SE_KOSARU_FOOTNOTE, 0, -1);

                if (i_this->field_0xc17 == 2) {
                    mDoAud_seStart(Z2SE_KOSARU_KANTERA_SWING, &i_this->field_0xc04, 0, 0);
                }
            }
        }
    } else if (i_this->field_0x5d0 == 28) {
        if (i_this->mpModelMorf->checkFrame(1.0f) && i_this->field_0xc17 == 2) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_WALK, -1);
        }

        if (i_this->mpModelMorf->checkFrame(2.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_FOOTNOTE, 0, -1);
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_WALK, -1);

            if (i_this->field_0xc17 == 2) {
                mDoAud_seStart(Z2SE_KOSARU_KANTERA_SWING, &i_this->field_0xc04, 0, 0);
            }
        }
    } else if (i_this->field_0x5d0 == 43) {
        if (i_this->mpModelMorf->checkFrame(5.0f) || i_this->mpModelMorf->checkFrame(47.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_SCARED, -1);
        }
    } else if (i_this->field_0x5d0 == 39) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_SCARED, -1);
        }
    } else if (i_this->field_0x5d0 == 30) {
        if (i_this->mpModelMorf->checkFrame(4.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(22.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
        }
    } else if (i_this->field_0x5d0 == 5) {
        if (i_this->mpModelMorf->checkFrame(4.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(22.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
        }
    } else if (i_this->field_0x5d0 == 7) {
        if (i_this->mpModelMorf->checkFrame(8.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP, -1);
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_START, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(17.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);

            if (i_this->field_0xc17 == 2) {
                mDoAud_seStart(Z2SE_KOSARU_KANTERA_ADD, &i_this->field_0xc04, 0, 0);
            }
        }
    } else if (i_this->field_0x5d0 == 8) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_CALL, -1);
        }
    } else if (i_this->field_0x5d0 == 44) {
        if (i_this->mpModelMorf->checkFrame(9.0f) || i_this->mpModelMorf->checkFrame(19.0f)) {
            mDoAud_seStart(Z2SE_KOSARU_KANTERA_SWING, &i_this->field_0xc04, 0, 0);
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_SWING_KANTERA, -1);
        }
    } else if (i_this->field_0x5d0 == 35) {
        if (i_this->mpModelMorf->checkFrame(40.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_SLAPPED_HIP, -1);
        }
    } else if (i_this->field_0x5d0 == 16) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_CALL, -1);
        }
    } else if (i_this->field_0x5d0 == 46) {
        if (i_this->mpModelMorf->checkFrame(4.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP_SURPRISE, -1);
        } else if (i_this->mpModelMorf->checkFrame(18.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
        } else if ((i_this->mpModelMorf->checkFrame(25.0f) || i_this->mpModelMorf->checkFrame(38.0f)) || i_this->mpModelMorf->checkFrame(51.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_POINT, -1);
            mDoAud_seStart(Z2SE_KOSARU_KANTERA_SWING, &i_this->field_0xc04, 0, 0);
        }
    } else if (i_this->field_0x5d0 == 34) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            f32 fVar1 = i_this->field_0x5c4 * 0.0215f;
            if (fVar1 > 127.0f) {
                fVar1 = 127.0f;
            }

            i_this->mSound.startCreatureSound(Z2SE_KOSARU_V_KAGO_WAIT, fVar1, -1);
        } else if (i_this->mpModelMorf->checkFrame(16.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
        }
    } else if (i_this->field_0x5d0 == 45) {
        if (i_this->mpModelMorf->checkFrame(4.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP_SURPRISE, -1);
        } else if (i_this->mpModelMorf->checkFrame(16.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_KOSARU_JUMP_END, 0, -1);
        }
    }
}

static void action(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp44, sp50;

    fopAcM_OffStatus(a_this, 0);
    cLib_offBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_TALKCHECK_e | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);

    if (i_this->field_0x5b5 != 0) {
        sp44 = player->eyePos - i_this->field_0x614;
        i_this->field_0x5c4 = sp44.abs();
        cLib_addCalcAngleS2(&a_this->current.angle.x, -0x8000, 1, 0x1000);
    } else {
        i_this->field_0x5c4 = fopAcM_searchPlayerDistanceXZ(a_this);
    }

    i_this->field_0x5c8 = fopAcM_searchPlayerAngleY(a_this);
    i_this->field_0x5fc = 0;
    i_this->field_0xaed = 0;
    i_this->field_0x5b5 = 0;

    int iVar1 = 1;
    int iVar2 = 1;
    int int_0x2c = 1;
    int iVar3 = 0;

    a_this->gravity = -5.0f;

    switch (i_this->mActionID) {
        case 4:
            a_this->gravity = 0.0f;
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
            a_this->gravity = 0.0f;
            i_this->field_0x5fc = 1;
            iVar2 = npc_ks_ori2(i_this);
            break;

        case 8:
            npc_ks_home(i_this);
            i_this->field_0x5fc = 1;
            break;

        case 10:
            a_this->gravity = 0.0f;
            npc_ks_hang(i_this);
            i_this->field_0x5fc = 2;
            i_this->field_0x5b5 = 1;
            iVar1 = 0;
            iVar2 = 0;
            break;

        case 11:
            a_this->gravity = 0.0f;
            npc_ks_hang_s(i_this);
            i_this->field_0x5fc = 2;
            i_this->field_0x5b5 = 1;
            iVar1 = 0;
            iVar2 = 0;
            break;

        case 20:
            a_this->gravity = 0.0f;
            npc_ks_to_hang(i_this);
            iVar2 = 2;
            break;

        case 21:
            a_this->gravity = 0.0f;
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
            a_this->gravity = -5.0f;
            iVar3 = 1;
            if (checkDoorDemo()) {
                if (i_this->mMode >= 7) {
                    i_this->mMode = 0;
                }

                cLib_addCalc2(&a_this->current.pos.y, player->current.pos.y, 1.0f, 100.0f);

                if (i_this->field_0x5c4 > 400.0f) {
                    cMtx_YrotS(*calc_mtx, i_this->field_0x5c8);
                    sp44.x = 0.0f;
                    sp44.y = 0.0f;
                    sp44.z = -300.0f;
                    MtxPosition(&sp44, &sp50);
                    a_this->current.pos.x = player->current.pos.x + sp50.x;
                    a_this->current.pos.z = player->current.pos.z + sp50.z;
                }

                iVar1 = 0;
                a_this->speed.y = 0.0f;
                int roomNo = fopAcM_GetRoomNo(a_this);
#if VERSION == VERSION_SHIELD_DEBUG
                int nextStayNo = dStage_roomControl_c::getNextStayNo();
#else
                // FIXME: fakematch. dbg asm implies that return value of getNextStayNo() must be int.
                // However, changing its return from s8 to int causes regalloc issue here.
                // Thus, use mNextStayNo directly here.
                int nextStayNo = dStage_roomControl_c::mNextStayNo;
#endif
                if (roomNo != nextStayNo) {
                    if (   (roomNo == 0 && nextStayNo == 3)
                        || (roomNo == 3 && nextStayNo == 4)
                        || (dComIfGs_isSwitch(0x52, fopAcM_GetRoomNo(a_this)) && roomNo == 4 && nextStayNo == 0)
                        || ((i_this->field_0x5b6 == 2 || i_this->field_0x5b6 == 3) && roomNo == 0 && nextStayNo == 15)
                        || (roomNo == 1 && nextStayNo == 4)
                        || (roomNo == 0 && nextStayNo == 1 && i_this->field_0x5b6 >= 2)
                        || (roomNo == 9 && nextStayNo != 12)
                    ) {
                        i_this->mActionID = 101;
                        i_this->mMode = 0;
                        break;
                    }
                }
            } else {
                i_this->field_0xaec = 1;
            }

            iVar2 = npc_ks_option(i_this);
            fopAcM_SetRoomNo(a_this, fopAcM_GetRoomNo(player));
            a_this->tevStr.room_no = fopAcM_GetRoomNo(a_this);
            break;

        case 101:
            a_this->gravity = -5.0f;
            npc_ks_awaydoor(i_this);
            break;

        case 110:
            a_this->gravity = -5.0f;
            iVar2 = npc_ks_guide_00(i_this);
            iVar3 = 1;
            break;

        case 111:
            a_this->gravity = -5.0f;
            iVar2 = npc_ks_guide_00_2(i_this);
            iVar3 = 1;
            break;

        case 112:
            a_this->gravity = -5.0f;
            iVar2 = npc_ks_guide_00_3(i_this);
            iVar3 = 1;
            break;

        case 113:
            a_this->gravity = -5.0f;
            iVar2 = npc_ks_guide_01(i_this);
            iVar3 = 1;
            break;

        case 114:
            a_this->gravity = -5.0f;
            iVar2 = npc_ks_guide_02(i_this);
            iVar3 = 1;
            break;

        case 115:
            a_this->gravity = -5.0f;
            iVar2 = npc_ks_guide_09(i_this);
            iVar3 = 1;
            break;

        case 116:
            a_this->gravity = -5.0f;
            iVar2 = npc_ks_guide_22(i_this);
            iVar3 = 1;
            break;

        case 117:
            a_this->gravity = -5.0f;
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
        if (water_check(i_this, a_this->current.pos, 60.0f + TREG_F(19))) {
            if (a_this->current.pos.y <= i_this->field_0x63c) {
                i_this->field_0xbdd = 1;
                if (i_this->mActionID != 100) {
                    i_this->mActionID = 100;
                    i_this->mMode = 0;
                }
            }
        }
    }

    if (int_0x2c && i_this->field_0x5c4 < 500.0f + KREG_F(15) && NPC_KS_FABSF(a_this->current.pos.y - player->current.pos.y) < 1000.0f) {
        daPy_py_c::setLookPos(&a_this->eyePos);
    }

    if (i_this->field_0xaed == 1 && i_this->field_0x5c4 < 300.0f) {
        i_this->mActionID = 1000;
        i_this->mMode = 0;
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 2, 0x2000);
    cLib_addCalcAngleS2(&a_this->shape_angle.z, a_this->current.angle.z, 2, 0x2000);

    if (iVar2 == 1) {
        f32 fVar1 = 1.0f;

        if (i_this->field_0x5d0 == 47 && fopAcM_CheckCondition(a_this, 4) != 0) {
            fVar1 = 10.0f + KREG_F(13);
        }

        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        sp44.x = 0.0f;
        sp44.y = 0.0f;
        sp44.z = a_this->speedF * fVar1;
        MtxPosition(&sp44, &sp50);

        a_this->speed.x = sp50.x;
        a_this->speed.z = sp50.z;
        a_this->current.pos += a_this->speed;
        a_this->speed.y += a_this->gravity;

        if (a_this->speed.y < -90.0f) {
            a_this->speed.y = -90.0f;
        }
    } else if (iVar2 == 2) {
        cMtx_YrotS(*calc_mtx, i_this->field_0x8fc.y);
        cMtx_XrotM(*calc_mtx, i_this->field_0x8fc.x);
        sp44.x = 0.0f;
        sp44.y = 0.0f;
        sp44.z = a_this->speedF;
        MtxPosition(&sp44, &a_this->speed);
        a_this->current.pos += a_this->speed;
        iVar1 = 0;
    }

    if (i_this->field_0xaec == 1) {
        cXyz* move_p = i_this->mStts.GetCCMoveP();
        if (move_p != NULL) {
            a_this->current.pos.x += move_p->x * 0.5f;
            a_this->current.pos.y += move_p->y * 0.5f;
            a_this->current.pos.z += move_p->z * 0.5f;
        }
    }

    if (iVar1 != 0) {
        if (a_this->shape_angle.x < -1000) {
            f32 fVar2 = (30.0f + ZREG_F(3)) * cM_ssin(a_this->shape_angle.x);
            a_this->current.pos.y += fVar2;
            a_this->old.pos.y += fVar2;

            i_this->mObjAcch.CrrPos(dComIfG_Bgsp());

            a_this->current.pos.y -= fVar2;
            a_this->old.pos.y -= fVar2;
        } else {
            i_this->mObjAcch.CrrPos(dComIfG_Bgsp());
        }
    }

    s16 sVar1 = 0;
    s16 sVar2 = 0;

    if (i_this->field_0x5fc != 0) {
        if (i_this->field_0x93c == 0) {
            if ((i_this->field_0x5e6 & 7U) == 0 && fopAcM_otoCheck(a_this, 2000.0f)) {
                SND_INFLUENCE* sound = dKy_Sound_get();
                i_this->field_0x940 = sound->position;
                i_this->field_0x93c = cM_rndF(20.0f) + 30.0f;
            }

            fopAc_ac_c* player_o_p = (fopAc_ac_c*) dComIfGp_getPlayer(0);
            if (i_this->field_0x938 != -1) {
                fopAc_ac_c* actor_p = fopAcM_SearchByID(i_this->field_0x938);
                if (actor_p != NULL) {
                    sp44 = actor_p->eyePos - a_this->current.pos;
                } else {
                    sp44 = player_o_p->eyePos - a_this->current.pos;
                }
            } else {
                sp44 = player_o_p->eyePos - a_this->current.pos;
                sp44.y += TREG_F(1) - 70.0f;
            }
        } else {
            i_this->field_0x93c--;
            if (i_this->field_0x93c < 40) {
                sp44 = i_this->field_0x940 - a_this->current.pos;
                sp44.y += 100.0f;
            } else {
                sp44 = player->eyePos - a_this->current.pos;
                sp44.y += TREG_F(1) -70.0f;
            }
        }

        sVar1 = cM_atan2s(sp44.x, sp44.z) - a_this->shape_angle.y;
        sVar1 *= -1;
        sVar2 = a_this->shape_angle.x + cM_atan2s(sp44.y, JMAFastSqrt(sp44.x * sp44.x + sp44.z * sp44.z));

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
        fopAcM_effSmokeSet1(&i_this->field_0x858, &i_this->field_0x85c, &a_this->current.pos, NULL,
                            0.75f + TREG_F(18), &a_this->tevStr, 1);
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
    fopAc_ac_c* a_this = &i_this->actor;
    MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(14), mDoMtx_stack_c::get());
    i_this->mpStickModel->setBaseTRMtx(mDoMtx_stack_c::get());

    if (i_this->field_0xc17 == 2) {
        mDoMtx_stack_c::transM(44.0f + KREG_F(7), 144.5f + KREG_F(8), 11.0f + KREG_F(9));
        mDoMtx_multVecZero(mDoMtx_stack_c::get(), &i_this->field_0xc04);
        f32 dbg_f31 = l_HIO.lantern_size;
        mDoMtx_stack_c::scaleM(dbg_f31, dbg_f31, dbg_f31);
        daPy_getPlayerActorClass()->setKandelaarMtx(mDoMtx_stack_c::get(), 1, 1);
    } else if (i_this->field_0xc17 == 3) {
        daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
        if (fopAcM_checkCarryNow(a_this) != 0 && player->getGrabUpStart()) {
            fopAcM_cancelCarryNow(a_this);
            cLib_offBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
            i_this->field_0xc17 = 0;
            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[0xE2] & 0xFFFF);
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
            fopAcM_OnCarryType(a_this, fopAcM_CARRY_ITEM);
            cLib_onBit<u32>(a_this->attention_info.flags, fopAc_AttnFlag_CARRY_e);
            a_this->attention_info.distances[fopAc_attn_CARRY_e] = 6;
            a_this->attention_info.position.x = i_this->field_0xc04.x;
            a_this->attention_info.position.y = i_this->field_0xc04.y + 30.0f;
            a_this->attention_info.position.z = i_this->field_0xc04.z;
            a_this->eyePos = a_this->attention_info.position;
        }
    }
}

static int c_start;

static int daNpc_Ks_Execute(npc_ks_class* i_this) {
    if (i_this->mActionID == 300) {
        if (c_start == 0 && i_this->field_0xb42 == 0 && dComIfGp_event_runCheck()) {
            kantera_sub(i_this);
            return 1;
        }
    } else {
        if (cDmrNowMidnaTalk()) {
            return 1;
        }
    }

    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp60, sp6c;

    if (i_this->field_0x5b6 == 2) {
        if (fopAcM_GetRoomNo(a_this) == 0) {
            dComIfGs_onSwitch(0x4F, fopAcM_GetRoomNo(a_this));
        }
    } else if (i_this->field_0x5b6 == 3 && fopAcM_GetRoomNo(a_this) == 0) {
        dComIfGs_onSwitch(0x50, fopAcM_GetRoomNo(a_this));
    }

    i_this->field_0x5e6++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->field_0x5f8 != 0) {
        i_this->field_0x5f8--;
    }

    if (i_this->field_0x94c != 0) {
        i_this->field_0x94c--;
    }

    if (i_this->field_0x94e != 0) {
        i_this->field_0x94e--;
    }

    action(i_this);

    if (i_this->field_0x90c != 0) {
        sp60 = i_this->field_0x91c - i_this->field_0x910;
        f32 fVar1 = sp60.abs();
        sp60 = i_this->field_0x91c - a_this->current.pos;
        f32 fVar2 = sp60.abs();
        f32 fVar3 = fVar1 * 0.2f;
        if (fVar3 > 200.0f) {
            fVar3 = 200.0f;
        }
        i_this->field_0x908 = fVar3 * cM_ssin((fVar2 / fVar1) * 32768.0f);
        i_this->field_0x90c = 0;
    } else {
        cLib_addCalc0(&i_this->field_0x908, 1.0f, 5.0f);
    }

    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y + i_this->field_0x908, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(a_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, i_this->field_0x928, 0.0f);
    mDoMtx_stack_c::scaleM(l_HIO.basic_size, l_HIO.basic_size, l_HIO.basic_size);

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
    anm_se_set(i_this);

    if (i_this->field_0x5e4 != 0) {
        i_this->mBtp2->play();
    } else {
        i_this->mBtp1->setFrame(i_this->field_0x5e2);
    }

    i_this->mpModelMorf->modelCalc();

    if (i_this->field_0x5d0 == 34) {
        daObj_Kago_c* basket_p = (daObj_Kago_c*)fpcM_Search(s_kago_sub, i_this);
        if (basket_p != NULL) {
            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(14), mDoMtx_stack_c::get());
            mDoMtx_stack_c::YrotM(-2000);
            mDoMtx_stack_c::XrotM(-0x3638);
            mDoMtx_stack_c::ZrotM(-0x2E68);
            mDoMtx_stack_c::transM(-60.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::YrotM(0);
            mDoMtx_stack_c::XrotM(0);
            mDoMtx_stack_c::ZrotM(0);
            
            basket_p->setMtx(mDoMtx_stack_c::get());
            if (fopAcM_checkHawkCarryNow(basket_p) != 0) {
                anm_init(i_this, 39, 5.0f, 2, 1.0f);
                i_this->mSound.startCreatureVoice(Z2SE_KOSARU_V_JUMP_SURPRISE, -1);
            }
        }
    }

    if (i_this->mpRoseModel != NULL) {
        i_this->mpRoseModel->setBaseTRMtx(i_this->mpModelMorf->getModel()->getAnmMtx(4));
    }

    kantera_sub(i_this);

    MTXCopy(model->getAnmMtx(4), *calc_mtx);
    sp60.set(0.0f, 0.0f, 0.0f);
    if (i_this->field_0xc17 != 3) {
        MtxPosition(&sp60, &a_this->eyePos);
        a_this->attention_info.position = a_this->eyePos;
        a_this->attention_info.position.y += 30.0f;
    }

    MTXCopy(model->getAnmMtx(10), *calc_mtx);
    sp60.set(30.0f, 0.0f, 0.0f);
    MtxPosition(&sp60, &i_this->field_0x614);

    MTXCopy(model->getAnmMtx(14), *calc_mtx);
    sp60.set(30.0f, -0.0f, 0.0f);
    MtxPosition(&sp60, &sp6c);
    i_this->field_0x614 += (sp6c - i_this->field_0x614) * 0.5f;

    if (i_this->field_0x620 != 0) {
        if (i_this->field_0x620 == 1) {
            cLib_addCalc2(&obj_pos.x, i_this->field_0x614.x, 1.0f, NPC_KS_FABSF(i_this->field_0x624.x) + 4.0f);
            cLib_addCalc2(&obj_pos.y, i_this->field_0x614.y, 1.0f, NPC_KS_FABSF(i_this->field_0x624.y) + 4.0f);
            cLib_addCalc2(&obj_pos.z, i_this->field_0x614.z, 1.0f, NPC_KS_FABSF(i_this->field_0x624.z) + 4.0f);
            sp60.x = obj_pos.x - i_this->field_0x614.x;
            sp60.z = obj_pos.z - i_this->field_0x614.z;
            if ((sp60.x * sp60.x + sp60.z * sp60.z) <= 15.0f) {
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
        cXyz sp78;
        sp78.x = obj_pos.x;
        sp78.z = obj_pos.z;
        sp78.y = obj_pos.y + i_this->field_0x638;
        setPlayerPosAndAngle(&sp78, a_this->home.angle.y);

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

    sp6c = a_this->current.pos;

    if (i_this->field_0xaec == 0) {
        i_this->field_0x98c.OffCoSetBit();
    } else {
        i_this->field_0x98c.OnCoSetBit();
    }

    i_this->field_0x98c.SetC(sp6c);
    i_this->field_0x98c.SetR(l_HIO.basic_size * 50.0f);
    i_this->field_0x98c.SetH(l_HIO.basic_size * 130.0f);
    dComIfG_Ccsp()->Set(&i_this->field_0x98c);

    if (i_this->field_0x5b6 == 0) {
        demo_camera(leader);
    } else if (i_this->field_0x5b6 >= 4) {
        demo_camera(i_this);
    }

    return 1;
}

static int daNpc_Ks_IsDelete(npc_ks_class* i_this) {
    return 1;
}

static int daNpc_Ks_Delete(npc_ks_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAcM_RegisterDeleteID(i_this, "Npc_Ks");
    dComIfG_resDelete(&i_this->mPhase, i_this->mResName);

    if (i_this->field_0xc1c != 0) {
        l_HIOInit = 0;
        mDoHIO_DELETE_CHILD(l_HIO.field_0x4);
    }

    if (a_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    npc_ks_class* i_this = (npc_ks_class*)a_this;

    i_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes(i_this->mResName, npc_ks_class::BMDR_SARU), NULL, NULL, 
                                               (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, npc_ks_class::BCK_SARU_WAIT_A), 0, 1.0f, 0, -1, 
                                               &i_this->mSound, 0x80000, 0x11020084);
    if (i_this->mpModelMorf == NULL || i_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setUserArea((uintptr_t)i_this);
    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    i_this->mBtp1 = new mDoExt_btpAnm();
    if (i_this->mBtp1 == NULL) {
        return 0;
    }
    if (i_this->mBtp1->init(i_this->mpModelMorf->getModel()->getModelData(), 
                            (J3DAnmTexPattern*)dComIfG_getObjectRes(i_this->mResName, npc_ks_class::BTP_SARU), 1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }

    i_this->mBtp2 = new mDoExt_btpAnm();
    if (i_this->mBtp2 == NULL) {
        return 0;
    }
    if (i_this->mBtp2->init(i_this->mpModelMorf->getModel()->getModelData(), 
                            (J3DAnmTexPattern*)dComIfG_getObjectRes(i_this->mResName, npc_ks_class::BTP_SARU_RELIEF), 1, 2, 1.0f, 0, -1) == 0) {
        return 0;
    }

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, npc_ks_class::BMDR_SARU_STICK);
    JUT_ASSERT(10232, modelData != NULL);
    i_this->mpStickModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->mpStickModel == NULL) {
        return 0;
    }

    if (i_this->mRoomMonkeyID == 0 || i_this->mRoomMonkeyID == 10 || i_this->mRoomMonkeyID == 20 || i_this->mRoomMonkeyID == 21 || i_this->mRoomMonkeyID == 22) {
        modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, npc_ks_class::BMDR_SARU_BARA);
        JUT_ASSERT(10249, modelData != NULL);
        i_this->mpRoseModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->mpRoseModel == NULL) {
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

    fopAc_ac_c* a_this = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    BOOL rv = TRUE;
    f32 fVar1;
    f32 fVar2;

    if (i_this->mRoomMonkeyID <= 3 && fopAcM_GetRoomNo(a_this) == 4) {
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
            a_this->current.angle.y = a_this->home.angle.y + 0x4000;
            a_this->shape_angle.y = a_this->current.angle.y;
            a_this->current.angle.x = -0x8000;
            a_this->shape_angle.x = -0x8000;
            i_this->field_0x630 = i_this->mRoomMonkeyID * 6 + 6;
            i_this->mActionID = 10;
            i_this->field_0x94e = 30;
            return rv;
        }
    }

    int iVar1 = (fopAcM_GetParam(a_this) & 0xFF000000) >> 24;
    int iVar2 = 0;

    switch (i_this->mRoomMonkeyID) {
        case 0:
            if (fopAcM_GetRoomNo(a_this) == 22) {
                if (iVar1 != 0xFF) {
                    if (!dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                        i_this->mActionID = 6;
                        break;
                    }
                }
            } else if (fopAcM_GetRoomNo(a_this) == 12) {
                i_this->mActionID = 8;
                break;
            }

            if (dComIfGs_isStageMiddleBoss()) {
                if (fopAcM_GetRoomNo(a_this) == 0) {
                    a_this->current.angle.y = a_this->home.angle.y + 0x4000;
                    a_this->shape_angle.y = a_this->current.angle.y;
                    a_this->current.angle.x = -0x8000;
                    a_this->shape_angle.x = -0x8000;
                    i_this->field_0x630 = 3;
                    i_this->mActionID = 11;
                    return rv;
                }

                if (fopAcM_GetRoomNo(a_this) == 2) {
                    a_this->current.angle.y = a_this->home.angle.y + 0x4000;
                    a_this->shape_angle.y = a_this->current.angle.y;
                    a_this->current.angle.x = -0x8000;
                    a_this->shape_angle.x = -0x8000;
                    i_this->mActionID = 10;
                    i_this->field_0x94e = 30;
                    return rv;
                }

                rv = FALSE;
            } else {
                if (fopAcM_GetRoomNo(a_this) == 0) {
                    if (!dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                        rv = FALSE;
                    }
                }
            }

            i_this->mActionID = 100;
            break;

        case 1:
            if (fopAcM_GetRoomNo(a_this) == 12) {
                i_this->mActionID = 8;
            } else {
                if (dComIfGs_isStageMiddleBoss()) {
                    if (fopAcM_GetRoomNo(a_this) == 0) {
                        a_this->current.angle.y = a_this->home.angle.y + 0x4000;
                        a_this->shape_angle.y = a_this->current.angle.y;
                        a_this->current.angle.x = -0x8000;
                        a_this->shape_angle.x = -0x8000;
                        i_this->field_0x630 = 3;
                        i_this->mActionID = 11;
                        return rv;
                    }

                    if (fopAcM_GetRoomNo(a_this) == 2) {
                        a_this->current.angle.y = a_this->home.angle.y + 0x4000;
                        a_this->shape_angle.y = a_this->current.angle.y;
                        a_this->current.angle.x = -0x8000;
                        a_this->shape_angle.x = -0x8000;
                        i_this->mActionID = 10;
                        i_this->field_0x94e = 30;
                        return rv;
                    }

                    rv = FALSE;
                } else {
                    if (fopAcM_GetRoomNo(a_this) == 2) {
                        if (dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                            i_this->mActionID = 100;
                        } else {
                            i_this->mActionID = 4;
                        }
                    } else {
                        if (!dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                            rv = FALSE;
                        } else {
                            i_this->mActionID = 100;
                        }
                    }
                }
            }
            break;

        case 2:
            if (fopAcM_GetRoomNo(a_this) == 12) {
                i_this->mActionID = 8;
            } else {
                if (dComIfGs_isStageMiddleBoss()) {
                    rv = FALSE;
                }

                if (fopAcM_GetRoomNo(a_this) == 7) {
                    if (dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                        rv = FALSE;
                    } else {
                        i_this->mActionID = 5;
                    }
                } else {
                    if (!dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                        rv = FALSE;
                    } else {
                        if (fopAcM_GetRoomNo(a_this) == 0) {
                            i_this->mActionID = 8;
                        } else {
                            if (fopAcM_GetRoomNo(a_this) == 3 || fopAcM_GetRoomNo(a_this) == 5 || fopAcM_GetRoomNo(a_this) == 7) {
                                if (dComIfGs_isSwitch(79, fopAcM_GetRoomNo(a_this))) {
                                    rv = FALSE;
                                    break;
                                }
                            }

                            i_this->mActionID = 100;
                        }
                    }
                }
            }
            break;

        case 3:
            if (fopAcM_GetRoomNo(a_this) == 12) {
                i_this->mActionID = 8;
            } else {
                if (dComIfGs_isStageMiddleBoss()) {
                    rv = FALSE;
                }

                if (fopAcM_GetRoomNo(a_this) == 5) {
                    if (dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                        rv = FALSE;
                    } else {
                        i_this->mActionID = 5;
                    }
                } else {
                    if (!dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                        rv = FALSE;
                    } else {
                        if (fopAcM_GetRoomNo(a_this) == 0) {
                            i_this->mActionID = 8;
                        } else {
                            if (fopAcM_GetRoomNo(a_this) == 3 || fopAcM_GetRoomNo(a_this) == 5 || fopAcM_GetRoomNo(a_this) == 7) {
                                if (dComIfGs_isSwitch(80, fopAcM_GetRoomNo(a_this))) {
                                    rv = FALSE;
                                    break;
                                }
                            }

                            i_this->mActionID = 100;
                        }
                    }
                }
            }
            break;

        case 4:
            if (fopAcM_GetRoomNo(a_this) == 12) {
                if (!dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                    rv = FALSE;
                } else {
                    i_this->mActionID = 8;
                }
            } else {
                if (dComIfGs_isSwitch(83, fopAcM_GetRoomNo(a_this))) {
                    rv = FALSE;
                } else {
                    if (fopAcM_GetRoomNo(a_this) == 4) {
                        if (dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                            i_this->mActionID = 117;
                            i_this->mMode = 0;
                            a_this->current.pos.set(6854.0f, 3458.0f, 440.0f);
                            a_this->current.angle.y = 0;
                            a_this->old = a_this->current;
                        } else {
                            i_this->mActionID = 6;
                        }
                    } else {
                        if (fopAcM_GetRoomNo(a_this) == 9) {
                            if (!dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                                return FALSE;
                            }

                            i_this->mActionID = 100;

                            for (int i = 0; i < 3; i++) {
                                f32 pos_x = dx[i] - player->current.pos.x;
                                f32 pos_y = dz[i] - player->current.pos.z;
                                if (JMAFastSqrt(pos_x * pos_x + pos_y * pos_y) < 700.0f) {
                                    a_this->current.pos.set(dx[i], dy[i], dz[i]);
                                    a_this->old = a_this->current;
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
            if (fopAcM_GetRoomNo(a_this) == 12) {
                if (!dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                    rv = FALSE;
                } else {
                    i_this->mActionID = 8;
                }
            } else {
                if (fopAcM_GetRoomNo(a_this) == 10) {
                    if (dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                        rv = FALSE;
                    } else {
                        i_this->mActionID = 5;
                    }
                }
            }
            break;

        case 6:
            if (fopAcM_GetRoomNo(a_this) == 12) {
                if (!dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                    rv = FALSE;
                } else {
                    i_this->mActionID = 8;
                }
            } else {
                if (fopAcM_GetRoomNo(a_this) == 19) {
                    if (dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                        rv = FALSE;
                    } else {
                        i_this->mActionID = 6;
                    }
                }
            }
            break;

        case 7:
            if (fopAcM_GetRoomNo(a_this) == 12) {
                if (!dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                    rv = FALSE;
                } else {
                    i_this->mActionID = 8;
                }
            } else {
                if (fopAcM_GetRoomNo(a_this) == 11) {
                    if (dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
                        rv = FALSE;
                    } else {
                        i_this->mActionID = 5;
                    }
                }
            }
            break;

        case 10:
            i_this->mActionID = 100;
            break;

        case 20:
            fopAcM_OnStatus(a_this, 0x20000);
            i_this->mActionID = 300;
            i_this->field_0xc17 = 1;
            iVar2 = 1;
            break;

        case 21:
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x20A] & 0xFFFF)) {
                rv = FALSE;
            } else {
                i_this->mActionID = 301;
            }
            break;

        case 22:
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x241] & 0xFFFF)) {
                rv = FALSE;
            } else {
                i_this->mActionID = 302;
            }
            break;

        default:
            i_this->mActionID = 100;
            i_this->mRoomMonkeyID &= (u8) 0xF;
            i_this->field_0x5b6 = i_this->mRoomMonkeyID;
            break;
    }

    if (i_this->mActionID == 100) {
        fopAcM_setStageLayer(a_this);
        if (fopAcM_GetRoomNo(a_this) == 1) {
            f32 x_pos = 6836.0f - player->current.pos.x;
            f32 y_pos = 3585.0f - player->current.pos.z;
            if (JMAFastSqrt(x_pos * x_pos + y_pos * y_pos) < 500.0f) {
                a_this->home.pos.set(7025.0f, 3355.0f, 4500.0f);
                a_this->home.angle.y = -0x7343;
                a_this->old = a_this->current = a_this->home;
            }

            i_this->mTimers[1] = cM_rndF(20.0f) + 80.0f;
        } else {
            i_this->mTimers[1] = cM_rndF(20.0f) + 40.0f;
        }
    }

    if (iVar2 != 0) {
        if (dComIfGs_isSwitch(iVar1, fopAcM_GetRoomNo(a_this))) {
            rv = FALSE;
        }
    }

    return rv;
}

static void* s_check_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KS) {
        if (fopAcM_GetID(i_actor) != fopAcM_GetID(i_data) && ((npc_ks_class*)i_actor)->field_0x5b6 == ((npc_ks_class*)i_data)->field_0x5b6) {
            return i_actor;
        }
    }

    return NULL;
}

static int daNpc_Ks_Create(fopAc_ac_c* a_this) {
    static dCcD_SrcCyl cc_cyl_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xd8fafd3f, 0x3}, 0x79}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            30.0f, // mRadius
            20.0f // mHeight
        } // mCyl
    };

    npc_ks_class* i_this = (npc_ks_class*)a_this;
    fopAcM_ct(&i_this->actor, npc_ks_class);

    stage_stag_info_class* stag_info = dComIfGp_getStage()->getStagInfo(); // unused
    if (dKy_darkworld_check()) {
        i_this->mResName = "Npc_kst";
    } else {
        i_this->mResName = "Npc_ks";
    }

    int phase = dComIfG_resLoad(&i_this->mPhase, i_this->mResName);
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("NPC_KS PARAM %x\n", fopAcM_GetParam(a_this));
        i_this->mRoomMonkeyID = fopAcM_GetParam(a_this);

        u8 uVar1 = (fopAcM_GetParam(a_this) & 0xFF00) >> 8;

        if (i_this->mRoomMonkeyID == 0xFF) {
            i_this->mRoomMonkeyID = 0;
        }

        if (i_this->mRoomMonkeyID == 20
            /* dSv_event_flag_c::F_0225 - Faron Woods - Lanter guide monkey doesn't come out a second time */
            && dComIfGs_isEventBit(u16(dSv_event_flag_c::saveBitLabels[0xE1]))
            /* dSv_event_flag_c::F_0226 - Faron Woods - Get lantern back from monkey */
            && dComIfGs_isEventBit(u16(dSv_event_flag_c::saveBitLabels[0xE2]))) {
            return cPhs_ERROR_e;
        }

        if (uVar1 == 0xFF) {
            uVar1 = 0;
        }

        if (fopAcM_GetRoomNo(a_this) == 4 && i_this->mRoomMonkeyID == 4) {
            a_this->current.pos.set(-5370.0f, 3950.0f, -2330.0f);
        }

        if (strcmp(dComIfGp_getStartStageName(), "D_MN05") == 0 && dComIfGs_isStageBossEnemy() != 0) {
            return cPhs_ERROR_e;
        }

        i_this->field_0x5b6 = i_this->mRoomMonkeyID & 15;
        if (fpcM_Search(s_check_sub, i_this) != NULL) {
            return cPhs_ERROR_e;
        }
            
        OS_REPORT("////////SET ID  %d\n", i_this->field_0x5b6);

        if (i_this->field_0x5b6 == 0) {
            leader = i_this;
            OS_REPORT("////////0 \n");
        } else {
            OS_REPORT("////////1 \n");
        }

        i_this->field_0xaf0 = a_this->current.angle.x;
        a_this->current.angle.x = a_this->shape_angle.x = 0;
        i_this->bitTRB = a_this->current.angle.z & 63;
        a_this->current.angle.z = a_this->shape_angle.z = 0;
        OS_REPORT("NPC_KS bitTRB %x\n", i_this->bitTRB);
        obj_pos.set(0.0f, 0.0f, 0.0f);

        OS_REPORT("NPC_KS//////////////NPC_KS SET 1 !!\n");
        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x38A0)) {
            OS_REPORT("//////////////NPC_KS SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////NPC_KS SET 2 !!\n");
        if (l_HIOInit == 0) {
            i_this->field_0xc1c = 1;
            l_HIOInit = 1;
            l_HIO.field_0x4 = mDoHIO_CREATE_CHILD("コザル", &l_HIO); // Kozaru
        }

        fopAcM_SetMtx(a_this, i_this->mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(a_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(a_this, 200.0f, 200.0f, 200.0f);
        i_this->mObjAcch.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1,
                                &i_this->mAcchCir, fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(50.0f, 50.0f);
        i_this->field_0x5e6 = cM_rndF(65536.0f);
        saru_p[i_this->field_0x5b6] = i_this;

        if (uVar1 != 0 && i_this->mRoomMonkeyID < 8) {
            a_this->current.angle.y = a_this->home.angle.y + 0x4000;
            a_this->shape_angle.y = a_this->current.angle.y;
            a_this->current.angle.x = -0x8000;
            a_this->shape_angle.x = -0x8000;
            i_this->field_0x630 = (fopAcM_GetParam(a_this) & 0xFF0000) >> 16;

            i_this->field_0x934 = (obj_sw_class*)fopAcM_SearchByID(a_this->parentActorID);
            if (i_this->field_0x934 != NULL && i_this->field_0x934->field_0x91c == 1) {
                i_this->mActionID = 11;
            } else {
                i_this->mActionID = 10;
            }

            i_this->field_0x94e = 30;
        } else {
            if (!start_check(i_this)) {
                saru_p[i_this->field_0x5b6] = NULL;
                return cPhs_ERROR_e;
            }
        }

        i_this->mSound.init(&a_this->current.pos, &a_this->eyePos, 2, 1);
        i_this->mStts.Init(100, 0, a_this);
        i_this->field_0x98c.Set(cc_cyl_src);
        i_this->field_0x98c.SetStts(&i_this->mStts);
        i_this->field_0x98c.OnTgNoHitMark();

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

extern actor_process_profile_definition g_profile_NPC_KS = {
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
