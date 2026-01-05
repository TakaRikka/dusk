#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_attack_item.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"

void daTagAtkItem_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
}

static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x1f}, {0x400020, 0x11}, 0x78}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                 // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6},                 // mGObjTg
        {0x0},                                              // mGObjCo
    },                                                      // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            50.0f,               // mRadius
            100.0f               // mHeight
        }                        // mCyl
    }
};

int daTagAtkItem_c::Create() {
    mCcStts.Init(0, 0xFF, this);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mCcStts);
    mCyl.SetR(scale.x * 50.0f);
    mCyl.SetH(scale.y * 100.0f);

    current.pos.y = home.pos.y - scale.y * 100.0f;
    return 1;
}

int daTagAtkItem_c::create() {
    fopAcM_ct(this, daTagAtkItem_c);

    if (!Create()) {
        return cPhs_ERROR_e;
    }

    OS_REPORT("攻撃反応アイテム：<%x>\n", fopAcM_GetParam(this));
    return cPhs_COMPLEATE_e;
}

int daTagAtkItem_c::execute() {
    action();
    setBaseMtx();
    eventUpdate();
    mCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCyl);
    return 1;
}


void daTagAtkItem_c::action() {
    if (checkHit()) {
        if (getEvId() != 0xFF) {
            orderEvent(getEvId(), 0xFF, 1);
        } else {
            createItem();
            fopAcM_delete(this);
        }
    }
}


bool daTagAtkItem_c::eventStart() {
    createItem();
    return true;
}

bool daTagAtkItem_c::eventEnd() {
    fopAcM_delete(this);
    return true;
}

BOOL daTagAtkItem_c::checkHit() {
    if (mCyl.ChkTgHit()) {
        cCcD_Obj* hitobj_p = mCyl.GetTgHitObj();
        if (hitobj_p != NULL &&
            (hitobj_p->ChkAtType(AT_TYPE_IRON_BALL) || hitobj_p->ChkAtType(AT_TYPE_BOMB)))
        {
            // "Attack Reaction Item: Rotate attack hit!\n"
            OS_REPORT("攻撃反応アイテム：回転アタックヒット！\n");
            return true;
        }
    }

    daPy_py_c* player_p = daPy_getPlayerActorClass();

    if ((player_p->checkFrontRollCrash() || player_p->checkWolfAttackReverse()) &&
        player_p->current.pos.absXZ(current.pos) < scale.x * 50.0f &&
        std::fabs(player_p->current.pos.y - current.pos.y) < scale.y * 100.0f)
    {
        // "Attack Reaction Item: Rotate attack hit!\n"
        OS_REPORT("攻撃反応アイテム：回転アタックヒット！\n");
        return true;
    }

    return false;
}

void daTagAtkItem_c::createItem() {
    csXyz angle(0, 0, 0);

    int create_num;
    if (getNum() == 0xFF) {
        create_num = 1;
    } else {
        create_num = getNum();
    }

    int item_bit = getItemBit();
    for (int i = 0; i < create_num; i++) {
        angle.y += (s16)cM_rndFX(0x7FFF);

        fopAcM_createItemFromTable(&home.pos, getItemNo(), item_bit, fopAcM_GetHomeRoomNo(this),
                                   &angle, 0, NULL, NULL, NULL, false);

        if (item_bit != 0xFF) {
            item_bit++;
        }
    }
}

int daTagAtkItem_c::_delete() {
    return 1;
}

static int daTagAtkItem_Execute(daTagAtkItem_c* i_this) {
    return i_this->execute();
}

static int daTagAtkItem_Delete(daTagAtkItem_c* i_this) {
    return i_this->_delete();
}

static int daTagAtkItem_Create(daTagAtkItem_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daTagAtkItem_Method = {
    (process_method_func)daTagAtkItem_Create,
    (process_method_func)daTagAtkItem_Delete,
    (process_method_func)daTagAtkItem_Execute,
    (process_method_func)NULL,
    (process_method_func)NULL,
};

actor_process_profile_definition g_profile_Tag_AttackItem = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_AttackItem,
    &g_fpcLf_Method.base,
    sizeof(daTagAtkItem_c),
    0,
    0,
    &g_fopAc_Method.base,
    585,
    &l_daTagAtkItem_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
