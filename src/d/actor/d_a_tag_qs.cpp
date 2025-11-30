//
//  d_a_tag_qs
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_qs.h"
#include "d/actor/d_a_obj_carry.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_a_item_static.h"
#include "d/d_save.h"
#include "d/d_com_inf_game.h"
#include "SSystem/SComponent/c_math.h"

static dCcD_SrcCyl l_cc_cyl_src = {
    {
        {0x0, {{AT_TYPE_CSTATUE_SWING, 0x0, 0x11}, {0x0, 0x0}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x5, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {0.0f, 0.0f, 0.0f}, // mCenter
        50.0f, // mRadius
        50.0f // mHeight
    } // mCyl
};

daTagQs_c::~daTagQs_c() {}

int daTagQs_c::create() {
    fopAcM_ct(this, daTagQs_c);
    f32 typeScale;
    if (getType() == 0) {
        typeScale = 1.0f;
    } else {
        typeScale = 20.0f;
    }
    field_0x568 = scale.x * 50.0f * typeScale;
    field_0x56c = scale.y * 50.0f * typeScale;
    field_0x570 = 0;
    field_0x574 = 0;
    mStts.Init(0xff, 0, this);
    mCyl.Set(l_cc_cyl_src);
    mCyl.SetStts(&mStts);
    return cPhs_COMPLEATE_e;
}

f32 daTagQs_c::getPower() {
    return getType() == 0 ? 4.0f : 5.0f;
}

f32 daTagQs_c::getSafeArea() {
    return getType() == 0 ? 0.8f : 0.5f;
}

f32 daTagQs_c::getPower80() {
    return getType() == 0 ? 6.0f : 10.0f;
}

f32 daTagQs_c::getCenterPower() {
    return getType() == 0 ? 12.0f : 20.0f;
}

f32 daTagQs_c::calcPower(f32 param_1, f32 param_2) {
    f32 dVar12 = 1.0f;
    if (getSwNo() != 0xff) {
        if (fopAcM_isSwitch(this, getSwNo())) {
            dVar12 = field_0x570 / 30.0f;
        } else {
            dVar12 = (30 - field_0x574) / 30.0f;
        }
    }
    if (param_2 > (param_1 * 0.8f)) {
        return dVar12 * getPower();
    }

    return dVar12 *
           ((1.0f - (param_2 / (param_1 * getSafeArea()))) * (getCenterPower() - getPower80()) +
            getPower80());
}

static int hikiyose(cXyz* param_1, cXyz* param_2, cXyz* param_3, f32 param_4) {
    cXyz cStack_44 = *param_1 - *param_2;
    cXyz cStack_50(cStack_44);
    cStack_50.y = 0.0f;
    f32 dVar6 = fabsf(cStack_50.getSquareMag());
    if (dVar6 < 4.0f) {
        param_2->x = param_1->x + cM_rndFX(1.0f);
        param_2->y -= 0.5f;
        param_2->z = param_1->z + cM_rndFX(1.0f);
        param_3->y = param_2->y;
        if (param_2->y < param_1->y - 100.0f) {
            return 2;
        }
        return 1;
    } else if (cStack_44.abs() < param_4) {
        param_2->x = param_1->x;
        param_2->y = param_1->y;
        param_2->z = param_1->z;
    } else {
        cStack_44.normalize();
        cStack_44 *= param_4;
        *param_2 += cStack_44;
    }
    return 0;
}

static void* search(void* param_1, void* param_2) {
    if (param_1 == NULL || !fopAcM_IsActor(param_1)) {
        return NULL;
    }
    fopAc_ac_c* actor1 = (fopAc_ac_c*) param_1;
    daTagQs_c* actor2 = (daTagQs_c*) param_2;
    f32 dVar9 = actor2->current.pos.absXZ(actor1->current.pos);
    if (dVar9 > actor2->field_0x568) {
        return NULL;
    }
    if (actor2->current.pos.y - 110.0f > actor1->current.pos.y ||
        actor2->current.pos.y + actor2->field_0x56c < actor1->current.pos.y)
    {
        return NULL;
    }
    if (actor1->current.pos.y >
        actor2->current.pos.y + (actor2->field_0x56c * dVar9) / (actor2->field_0x568 * 0.9f) + 20.0f)
    {
        return NULL;
    }
    if (fopAcM_GetProfName(param_1) == PROC_ITEM) {
        daItem_c* item = (daItem_c*)param_1;
        item->getItemNo();
        if (item->getItemNo() <= 3) {
            int iVar3 = hikiyose(&actor2->current.pos, &actor1->current.pos, &actor1->old.pos,
                                 actor2->calcPower(actor2->field_0x568, dVar9));
            if (iVar3 == 1) {
                item->startCtrl();
            } else if (iVar3 == 2) {
                fopAcM_delete(actor1);
            }
        }
        return NULL;
    }
    if (fopAcM_GetProfName(param_1) == PROC_Obj_Carry) {
        daObjCarry_c* carry = (daObjCarry_c*)param_1;
        if (carry->getType() == 7 || carry->getType() == 1 || carry->getType() == 5) {
            int iVar3 = hikiyose(&actor2->current.pos, &carry->current.pos, &carry->old.pos, actor2->calcPower(actor2->field_0x568, dVar9));
            if (iVar3 == 1) {
                carry->startCtrl();
            } else if (iVar3 == 2) {
                fopAcM_delete(carry);
            }
        }
        return NULL;
    }
    if (param_1 == daPy_getPlayerActorClass()) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        cXyz local_58 = actor2->current.pos - player->current.pos;
        if (local_58.getSquareMag() >= actor2->getPower()) {
            s16 sVar6 = cM_atan2s(local_58.x, local_58.z);
            if (!player->checkPlayerFly()) {
                player->setOutPower(actor2->calcPower(actor2->field_0x568, dVar9), sVar6, 0);
            }
        }
        return NULL;
    }
    
    return NULL;
}

int daTagQs_c::execute() {
    fopAcM_Search(search, this);
    if (getSwNo() != 0xff) {
        int bossLife;
        if (fopAcM_isSwitch(this, getSwNo())) {
            field_0x570++;
            if (field_0x570 > 30) {
                field_0x570 = 30;
            }
            field_0x574 = 0;
            bossLife = (field_0x570 * 100.0f) / 30.0f;
            if (getType() == 0) {
                Z2GetAudioMgr()->seStartLevel(Z2SE_ENV_QUICKSAND_LOOP, &current.pos, 0,
                                              dComIfGp_getReverb(fopAcM_GetRoomNo(this)), 1.0f,
                                              1.0f, -1.0f, -1.0f, 0);
            }
        } else {
            field_0x574++;
            if (field_0x574 > 30) {
                field_0x574 = 30;
            }
            field_0x570 = 0;
            bossLife = 100.0f - field_0x574 * 100.0f / 30.0f;
        }
        dComIfGs_BossLife_public_Set(bossLife);
    } else {
        if (getType() == 0) {
            Z2GetAudioMgr()->seStartLevel(Z2SE_ENV_QUICKSAND_LOOP, &current.pos, 0,
                                          dComIfGp_getReverb(fopAcM_GetRoomNo(this)), 1.0f, 1.0f,
                                          -1.0f, -1.0f, 0);
        }
    }
    return 1;
}

int daTagQs_c::draw() {
    return 1;
}

static int daTagQs_Draw(daTagQs_c* i_this) {
    return i_this->draw();
}

static int daTagQs_Execute(daTagQs_c* i_this) {
    return i_this->execute();
}

static int daTagQs_IsDelete(daTagQs_c* i_this) {
    return 1;
}

static int daTagQs_Delete(daTagQs_c* i_this) {
    i_this->~daTagQs_c();
    return 1;
}

static int daTagQs_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagQs_c*>(i_this)->create();
}

static actor_method_class l_daTagQs_Method = {
    (process_method_func)daTagQs_Create,     (process_method_func)daTagQs_Delete,
    (process_method_func)daTagQs_Execute, (process_method_func)daTagQs_IsDelete,
    (process_method_func)daTagQs_Draw,
};

extern actor_process_profile_definition g_profile_TAG_QS = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_TAG_QS,
    &g_fpcLf_Method.base,
    sizeof(daTagQs_c),
    0,
    0,
    &g_fopAc_Method.base,
    274,
    &l_daTagQs_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
