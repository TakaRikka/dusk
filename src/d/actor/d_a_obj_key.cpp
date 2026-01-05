/**
 * @file d_a_obj_key.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_key.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_obj_smallkey.h"
#include "d/actor/d_a_e_dn.h"
#include "d/actor/d_a_e_rd.h"
#include "d/actor/d_a_e_rdb.h"
#include "d/actor/d_a_e_oc.h"
#include "d/actor/d_a_e_db.h"
#include "SSystem/SComponent/c_counter.h"

class daObj_Key_HIO_c : public JORReflexible{
public:
    daObj_Key_HIO_c();
    virtual ~daObj_Key_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ s8 id;
    /* 0x8 */ f32 size;
    /* 0xC */ f32 chain_size;
};

daObj_Key_HIO_c::daObj_Key_HIO_c() {
    id = -1;
    size = 1.0f;
    chain_size = 1.5f;
}

static int daObj_Key_Draw(obj_key_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    if (!i_this->hide) {
        g_env_light.settingTevStruct(16, &actor->current.pos, &actor->tevStr);

        for (int i = 0; i < 2; i++) {
            g_env_light.setLightTevColorType_MAJI(i_this->key_s.model[i], &actor->tevStr);
            mDoExt_modelUpdate(i_this->key_s.model[i]);
        }
    }

    return 1;
}

static u8 hio_set;

static daObj_Key_HIO_c l_HIO;

static s16 target_info_count;

static void* s_count_sub(void* i_actor, void* i_other) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_KEY && ((fopAc_ac_c*)i_actor)->field_0x560 != 0) {
        ((fopAc_ac_c*)i_actor)->health = target_info_count;
        target_info_count++;
    }

    return NULL;
}

static void* s_master_sub(void* i_actor, void* i_other) {
    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_DN) ||
        fopAcM_GetName(i_actor) == PROC_E_RD ||
        fopAcM_GetName(i_actor) == PROC_E_RDB ||
        fopAcM_GetName(i_actor) == PROC_E_DB ||
        fopAcM_GetName(i_actor) == PROC_E_HB ||
        fopAcM_GetName(i_actor) == PROC_E_OC ||
        fopAcM_GetName(i_actor) == PROC_ALINK)
    {
        f32 x_dist = ((fopAc_ac_c*)i_actor)->current.pos.x - ((fopAc_ac_c*)i_other)->current.pos.x;
        f32 z_dist = ((fopAc_ac_c*)i_actor)->current.pos.z - ((fopAc_ac_c*)i_other)->current.pos.z;

        if ((SQUARE(x_dist) + SQUARE(z_dist)) < SQUARE(200.0f)) {
            return i_actor;
        }
    }

    return NULL;
}

static void masterkey_check(obj_key_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    daKey_c* pkey = (daKey_c*)fopAcM_SearchByName(PROC_Obj_SmallKey);
    if (pkey != NULL) {
        f32 x_dist = actor->current.pos.x - pkey->current.pos.x;
        f32 z_dist = actor->current.pos.z - pkey->current.pos.z;
        if ((SQUARE(x_dist) + SQUARE(z_dist)) < SQUARE(200.0f)) {
            i_this->key_s.key_actor_id = fopAcM_GetID(pkey);
            i_this->action = 1;
        }
    } else if (i_this->timers[0] == 0) {
        fopAcM_delete(actor);
    }
}

static void action_check(obj_key_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    fopAc_ac_c* pactor = (fopAc_ac_c*)fpcM_Search(s_master_sub, i_this);
    if (pactor != NULL) {
        i_this->action = 3;
        actor->parentActorID = fopAcM_GetID(pactor);
    } else if (i_this->timers[0] == 0) {
        i_this->action = 2;
    }
}

static void chain_control_00(obj_key_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    switch (i_this->field_0x60c) {
    case 0:
        if (i_this->acch.ChkGroundHit()) {
            actor->speed.x = cM_rndFX(5.0f);
            actor->speed.z = cM_rndFX(5.0f);
            actor->speed.y = 20.0f;

            i_this->sound.startSound(JA_SE_CM_MAGBALL_BOUND, 0, -1);
            i_this->field_0x60c++;
        }

        actor->shape_angle.y += 0x200;
        actor->shape_angle.x += 0x900;
        break;
    case 1:
        if (i_this->acch.ChkGroundHit()) {
            actor->speed.z = 0.0f;
            actor->speed.x = 0.0f;

            i_this->sound.startSound(JA_SE_CM_MAGBALL_BOUND, 0, -1);
            i_this->field_0x60c++;
        }
        /* fallthrough */
    case 2:
        cLib_addCalcAngleS2(&actor->shape_angle.x, 0, 1, 0x1000);
        cLib_addCalcAngleS2(&actor->shape_angle.z, 0, 1, 0x1000);
        break;
    }

    cXyz sp48;
    cXyz sp3C;
    dBgS_GndChk gndchk;
    int i;

    i_this->key_s.pos[1] = actor->current.pos;
    i_this->key_s.angle[1] = actor->shape_angle;

    cXyz* ppos = i_this->key_s.pos;
    csXyz* pangle = i_this->key_s.angle;
    f32* pgnd_y = i_this->key_s.ground_y;

    f32 temp_f25 = -20.0f + TREG_F(0);
    f32 sp20 = TREG_F(3);

    sp48.y = 0.0f;
    sp48.x = 0.0f;
    sp48.z = 17.0f + (15.0f + (4.5f + (5.0f + TREG_F(2))));

    for (i = 0; i >= 0; i--, ppos--, pangle--, pgnd_y--) {        
        f32 temp_f30 = ppos[0].x - ppos[1].x;
        f32 temp_f26;
        f32 temp_f29 = ppos[0].z - ppos[1].z;

        f32 var_f28 = ppos[0].y + temp_f25;
        if (var_f28 <= *pgnd_y) {
            var_f28 = *pgnd_y;
        }

        temp_f26 = var_f28 - ppos[1].y;

        s16 spC, spA;
        spA = (s16)cM_atan2s(temp_f30, temp_f29);
        spC = -cM_atan2s(temp_f26, JMAFastSqrt(SQUARE(temp_f30) + SQUARE(temp_f29)));

        cMtx_YrotS(*calc_mtx, spA);
        cMtx_XrotM(*calc_mtx, spC);
        MtxPosition(&sp48, &sp3C);
        ppos[0].x = ppos[1].x + sp3C.x;
        ppos[0].y = ppos[1].y + sp3C.y;
        ppos[0].z = ppos[1].z + sp3C.z;
        pangle->x = spC;
        pangle->y = spA;

        if (((g_Counter.mCounter0 + i) & 15) == 0) {
            Vec sp30;
            sp30 = *ppos;
            sp30.y += 200.0f;
            gndchk.SetPos(&sp30);

            *pgnd_y = TREG_F(6) + (5.0f + dComIfG_Bgsp().GroundCross(&gndchk));
            if (*pgnd_y - ppos->y > 200.0f) {
                *pgnd_y = ppos->y;
            }
        }
    }

    s16 rot_z = 0;
    for (int i = 0; i < 2; i++) {
        MtxTrans(i_this->key_s.pos[i].x, i_this->key_s.pos[i].y, i_this->key_s.pos[i].z, 0);
        cMtx_YrotM(*calc_mtx, i_this->key_s.angle[i].y);
        cMtx_XrotM(*calc_mtx, i_this->key_s.angle[i].x);

        f32 size;
        f32 trans_z;
        if (i == 1) {
            size = l_HIO.size;
            trans_z = 2.0f + (14.0f + TREG_F(4));
            cMtx_ZrotM(*calc_mtx, i_this->key_s.angle[i].z);
        } else {
            size = l_HIO.chain_size;
            trans_z = 5.0f + (5.0f + TREG_F(5));
            cMtx_ZrotM(*calc_mtx, rot_z);
            rot_z += 0x4000;
        }

        MtxScale(size, size, size, 1);
        MtxTrans(0.0f, 0.0f, trans_z, 1);
        cMtx_XrotM(*calc_mtx, -0x4000);
        i_this->key_s.model[i]->setBaseTRMtx(*calc_mtx);
    }

    i_this->hide = FALSE;
}

static void chain_control_01(obj_key_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;

    cXyz spC4;
    cXyz spB8;
    Vec spAC;
    dBgS_GndChk gndchk;
    int i;

    fopAc_ac_c* pparent = fopAcM_SearchByID(actor->parentActorID);
    daKey_c* pkey = (daKey_c*)fopAcM_SearchByID(i_this->key_s.key_actor_id);

    if (pparent == NULL) {
        if (pkey != NULL) {
            spB8 = actor->current.pos;
            spB8.y += 200.0f;

            if (fopAcM_gc_c::gndCheck(&spB8)) {
                spB8.y = 50.0f + fopAcM_gc_c::getGroundY();
            }

            pkey->setPos(spB8);
            pkey->speed.y = 50.0f;
        }

        fopAcM_delete(actor);
        return;
    }

    if (pkey != NULL) {
        cXyz spA0;
        spA0 = actor->current.pos;
        spA0.x += 10000.0f;
        spA0.y += 10000.0f;

        pkey->setPos(spA0);
        pkey->seStartTwinkle(Z2SE_OBJ_KEY_TWINKLE, &actor->current.pos);
        pkey->speed.y = 0.0f;
        pkey->old.pos = actor->current.pos;
    }

    cXyz sp94(i_this->key_s.pos[0]);
    actor->scale.x = l_HIO.size;
    actor->field_0x560 = 0;
    u8 sp8 = 0;

    J3DModel* parent_model;
    if (fopAcM_GetName(pparent) == PROC_E_DN) {
        e_dn_class* dn = (e_dn_class*)pparent;
        parent_model = dn->anm_p->getModel();

        MTXCopy(parent_model->getAnmMtx(1), *calc_mtx);
        spC4.set(5.0f + KREG_F(0), 30.0f + KREG_F(1), 20.0f + KREG_F(2));
        MtxPosition(&spC4, &spB8);
    } else if (fopAcM_GetName(pparent) == PROC_E_RD) {
        e_rd_class* rd = (e_rd_class*)pparent;
        parent_model = rd->mpModelMorf->getModel();

        MTXCopy(parent_model->getAnmMtx(VREG_S(0) + 12), *calc_mtx);
        spC4.set(12.0f + VREG_F(0), -5.0f + VREG_F(1), VREG_F(2));
        MtxPosition(&spC4, &spB8);
        rd->actor.field_0x567 = 1;
    } else if (fopAcM_GetName(pparent) == PROC_E_RDB) {
        e_rdb_class* rdb = (e_rdb_class*)pparent;
        parent_model = rdb->mpModelMorf->getModel();

        if (rdb->field_0x1104 != 0) {
            MTXCopy(parent_model->getAnmMtx(XREG_S(2) + 21), *calc_mtx);
            spC4.set(7.0f + VREG_F(0), -3.0f + VREG_F(1), VREG_F(2));
        } else {
            MTXCopy(parent_model->getAnmMtx(XREG_S(2) + 22), *calc_mtx);
            spC4.set(-5.0f + VREG_F(0), -15.0f + VREG_F(1), -50.0f + VREG_F(2));
        }

        MtxPosition(&spC4, &spB8);

        if (rdb->field_0xfe5 != 0) {
            fopAcM_delete(pkey);
            fopAcM_delete(actor);
        }
    } else if (fopAcM_GetName(pparent) == PROC_E_OC) {
        daE_OC_c* oc = (daE_OC_c*)pparent;
        parent_model = oc->getOcModel();

        MTXCopy(parent_model->getAnmMtx(nREG_S(0) + 15), *calc_mtx);
        spC4.set(5.0f + nREG_F(0), nREG_F(1), nREG_F(2));
        MtxPosition(&spC4, &spB8);
        sp8 = 1;
    } else if (fopAcM_GetName(pparent) == PROC_E_DB || fopAcM_GetName(pparent) == PROC_E_HB) {
        e_db_class* db = (e_db_class*)pparent;

        if (db->action == 20) {
            spB8 = db->field_0x69c[2];
        } else {
            spB8 = db->field_0x69c[TREG_S(3) + 9];
        }
    } else {
        MTXCopy(daPy_getLinkPlayerActorClass()->getModelJointMtx(TREG_S(3) + 2), *calc_mtx);
        spC4.set(-30.0f + KREG_F(0), -15.0f + KREG_F(1), 12.0f + KREG_F(2));
        MtxPosition(&spC4, &spB8);
        actor->scale.x = 1.0f;
        actor->field_0x560 = 1;
        target_info_count = 0;
        fpcM_Search(s_count_sub, i_this);
    }

    i_this->key_s.pos[0] = spB8;

    cXyz* ppos = &i_this->key_s.pos[1];
    csXyz* pangle = &i_this->key_s.angle[1];
    f32* pgnd_y = &i_this->key_s.ground_y[1];

    f32 sp20 = -20.0f + TREG_F(0);

    f32 temp_f27;
    f32 temp_f31, temp_f30;
    temp_f30 = sp94.x - i_this->key_s.pos[0].x;
    temp_f31 = sp94.z - i_this->key_s.pos[0].z;

    f32 temp_f29 = (temp_f30 * temp_f30) + (temp_f31 * temp_f31);

    if (temp_f29 > (20.0f + TREG_F(7))) {
        temp_f29 = (0.4f + TREG_F(9)) * JMAFastSqrt(temp_f29);
        if (i_this->field_0x618 < temp_f29) {
            i_this->field_0x618 = temp_f29;
            if (i_this->field_0x618 > 20.0f) {
                i_this->field_0x618 = 20.0f;
            }

            i_this->field_0x616 = cM_rndFX(200.0f);
            i_this->field_0x614 = (s16)cM_atan2s(temp_f30, temp_f31);
            i_this->field_0x612 = 0;
        }
    }

    cXyz sp88;
    i_this->field_0x612 += (s16)3000;
    i_this->field_0x614 += i_this->field_0x616;
    cMtx_YrotS(*calc_mtx, i_this->field_0x614);

    spC4.x = spC4.y = 0.0f;
    spC4.z = -i_this->field_0x618 * cM_ssin(i_this->field_0x612);
    MtxPosition(&spC4, &sp88);

    cLib_addCalc0(&i_this->field_0x618, 0.1f, 0.2f + TREG_F(10));

    cXyz sp7C(0.0f, 0.0f, 0.0f);
    s16 sp10, spE, spC;

    if (sp8) {
        spC4 = i_this->key_s.pos[0] - pparent->current.pos;

        spC = cM_atan2s(spC4.x, spC4.z);
        spC -= pparent->shape_angle.y;
        if (spC > 0x4000 || spC < -0x4000) {
            cMtx_YrotS(*calc_mtx, ((pparent->shape_angle.y + TREG_S(5)) - 10000));
            spC4.x = 0.0f;
            spC4.y = 0.0f;
            spC4.z = (1.0f + TREG_F(17)) * spC4.abs();
            MtxPosition(&spC4, &sp7C);
        }
    }

    spC4.x = spC4.y = 0.0f;
    spC4.z = 15.0f + (20.0f + (4.5f + (5.0f + TREG_F(2))));

    for (i = 1; i < 3; i++, ppos++, pangle++, pgnd_y++) {
        if (i == 2) {
            sp88.x *= 2.0f;
            sp88.z *= 2.0f;
        }

        temp_f30 = sp7C.x + (sp88.x + (ppos[0].x - ppos[-1].x));
        temp_f31 = sp7C.z + (sp88.z + (ppos[0].z - ppos[-1].z));
        
        f32 var_f26 = ppos->y + sp20;
        if (var_f26 <= *pgnd_y) {
            var_f26 = *pgnd_y;
        }

        temp_f27 = var_f26 - ppos[-1].y;
        
        sp10 = -cM_atan2s(temp_f27, temp_f31);
        spE = (s16)cM_atan2s(temp_f30, JMAFastSqrt(SQUARE(temp_f27) + SQUARE(temp_f31)));
        cMtx_XrotS(*calc_mtx, sp10);
        cMtx_YrotM(*calc_mtx, spE);
        MtxPosition(&spC4, &spB8);
        ppos[0].x = ppos[-1].x + spB8.x;
        ppos[0].y = ppos[-1].y + spB8.y;
        ppos[0].z = ppos[-1].z + spB8.z;
        pangle[-1].x = sp10;
        pangle[-1].y = spE;

        if (((g_Counter.mCounter0 + i) & 15) == 0) {
            spAC = *ppos;
            spAC.y += 200.0f;
            gndchk.SetPos(&spAC);

            *pgnd_y = TREG_F(6) + (10.0f + dComIfG_Bgsp().GroundCross(&gndchk));
            if (*pgnd_y - ppos->y > 200.0f) {
                *pgnd_y = ppos->y;
            }
        }
    }

    s16 spA;
    if (fopAcM_GetName(pparent) == PROC_E_RDB) {
        e_rdb_class* rdb = (e_rdb_class*)pparent;
        if (rdb->field_0x1104 != 0) {
            spA = -(pparent->shape_angle.y + TREG_S(7) + 0x4000);
        } else {
            spA = -(pparent->shape_angle.y + TREG_S(7));
        }
    } else {
        spA = -(pparent->shape_angle.y + TREG_S(7) + 0x4000);
    }

    f32 var_f28;
    f32 var_f25 = 0.0f;

    for (int i = 0; i < 2; i++) {
        MtxTrans(i_this->key_s.pos[i].x, i_this->key_s.pos[i].y, i_this->key_s.pos[i].z, 0);
        cMtx_XrotM(*calc_mtx, i_this->key_s.angle[i].x);
        cMtx_YrotM(*calc_mtx, i_this->key_s.angle[i].y);
        spA += (s16)(TREG_S(8) + 0x4000);

        if (i == 1) {
            var_f28 = actor->scale.x;
            var_f25 = 20.0f + TREG_F(4);
        } else {
            var_f28 = l_HIO.chain_size;
            var_f25 = 3.0f + (5.0f + TREG_F(5));
        }

        cMtx_ZrotM(*calc_mtx, spA);
        MtxScale(var_f28, var_f28, var_f28, 1);

        if (i == 1) {
            cMtx_XrotM(*calc_mtx, (actor->health * (TREG_S(1) + 500)));
        }

        MtxTrans(0.0f, 0.0f, var_f25, 1);
        cMtx_XrotM(*calc_mtx, -0x4000);
        i_this->key_s.model[i]->setBaseTRMtx(*calc_mtx);

        if (i == 1) {
            spC4.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&spC4, &actor->current.pos);
            actor->shape_angle.y = i_this->key_s.angle[1].y;
        }
    }

    i_this->hide = FALSE;
}

static void action(obj_key_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    f32 temp_f31 = -3.0f + TREG_F(12);
    BOOL set_particles = TRUE;

    switch (i_this->action) {
    case 0:
        masterkey_check(i_this);
        set_particles = FALSE;
        break;
    case 1:
        action_check(i_this);
        set_particles = FALSE;
        break;
    case 2:
        chain_control_00(i_this);
        actor->current.pos += actor->speed;
        actor->speed.y -= 5.0f;

        actor->current.pos.y += temp_f31;
        actor->old.pos.y += temp_f31;
        i_this->acch.CrrPos(dComIfG_Bgsp());
        actor->current.pos.y -= temp_f31;
        actor->old.pos.y -= temp_f31;

        if (fopAcM_searchPlayerDistance(actor) < 50.0f) {
            i_this->action = 3;
            actor->parentActorID = fopAcM_GetID(dComIfGp_getPlayer(0));
        }
        break;
    case 3:
        chain_control_01(i_this);
        break;
    }

    if (set_particles) {
        for (int i = 0; i < 2; i++) {
            static u16 key_eno[] = {0x827A, 0x827B};
            i_this->field_0x9ac[i] = dComIfGp_particle_set(i_this->field_0x9ac[i], key_eno[i], &actor->current.pos, NULL, NULL);
        }
    }
}

static int daObj_Key_Execute(obj_key_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    for (int i = 0; i < 2; i++) {
        if (i_this->timers[i] != 0) {
            i_this->timers[i]--;
        }
    }

    action(i_this);

    i_this->sound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    return 1;
}

static int daObj_Key_IsDelete(obj_key_class* i_this) {
    return 1;
}

static int daObj_Key_Delete(obj_key_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fpc_ProcID id = fopAcM_GetID(actor);
    dComIfG_resDelete(&i_this->phase, "Obj_key");

    if (i_this->HIOInit) {
        hio_set = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (actor->heap != NULL) {
        i_this->sound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    obj_key_class* a_this = (obj_key_class*)i_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_key", 5);
    JUT_ASSERT(969, modelData != NULL);

    for (int i = 0; i < 2; i++) {
        if (i == 1) {
            modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_key", 3);
            JUT_ASSERT(975, modelData != NULL);
        }

        a_this->key_s.model[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (a_this->key_s.model[i] == NULL) {
            return 0;
        }

        a_this->key_s.pos[i] = i_this->current.pos;
    }

    a_this->sound.init(&i_this->current.pos, 1);
    return 1;
}

static int daObj_Key_Create(fopAc_ac_c* i_this) {
    obj_key_class* a_this = (obj_key_class*)i_this;
    fopAcM_ct(i_this, obj_key_class);

    int phase_state = dComIfG_resLoad(&a_this->phase, "Obj_key");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_KEY PARAM %x\n", fopAcM_GetParam(i_this));

        if (strcmp(dComIfGp_getStartStageName(), "F_SP118") != 0) {
            if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0 && dComIfGs_isStageMiddleBoss()) {
                return cPhs_ERROR_e;
            }

            int swbit = (fopAcM_GetParam(i_this) & 0xFF000000) >> 0x18;
            if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(i_this))) {
                return cPhs_ERROR_e;
            }
        }

        OS_REPORT("OBJ_KEY//////////////OBJ_KEY SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x1020)) {
            OS_REPORT("//////////////OBJ_KEY SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_KEY SET 2 !!\n");

        if (!hio_set) {
            a_this->HIOInit = TRUE;
            hio_set = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("カギＯＢＪ", &l_HIO);
        }

        fopAcM_SetMtx(i_this, a_this->key_s.model[0]->getBaseTRMtx());
        fopAcM_SetMin(i_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(i_this, 200.0f, 200.0f, 200.0f);

        a_this->acch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->acchcir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->acchcir.SetWall(30.0f, 30.0f);

        a_this->action = 0;
        a_this->timers[0] = 10;
        a_this->key_s.key_actor_id = fpcM_ERROR_PROCESS_ID_e;
        a_this->hide = TRUE;

        daObj_Key_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daObj_Key_Method = {
    (process_method_func)daObj_Key_Create,
    (process_method_func)daObj_Key_Delete,
    (process_method_func)daObj_Key_Execute,
    (process_method_func)daObj_Key_IsDelete,
    (process_method_func)daObj_Key_Draw,
};

actor_process_profile_definition g_profile_OBJ_KEY = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_OBJ_KEY,           // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(obj_key_class),  // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    44,                     // mPriority
    &l_daObj_Key_Method,    // sub_method
    0x00044100,             // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
