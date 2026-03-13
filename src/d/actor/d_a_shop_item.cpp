/**
 * d_a_shop_item.cpp
 * Shop Item Actor
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_shop_item.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_lib.h"
#include <cstring>

const char* daShopItem_c::getShopArcname() {
    switch (m_itemNo) {
    case dItemNo_NONE_e:
        if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0) {
            mShopItemID = SHOP_ITEMNO_ARMOR_SOLD;
        } else {
            mShopItemID = SHOP_ITEMNO_SOLD;
        }
        break;
    case dItemNo_OIL_BOTTLE_e:
        mShopItemID = SHOP_ITEMNO_OIL;
        break;
    case dItemNo_MILK_BOTTLE_e:
        mShopItemID = SHOP_ITEMNO_MILK;
        break;
    case dItemNo_HALF_MILK_BOTTLE_e:
        mShopItemID = SHOP_ITEMNO_HALF_MILK;
        break;
    case dItemNo_RED_BOTTLE_e:
        mShopItemID = SHOP_ITEMNO_RED;
        break;
    case dItemNo_GREEN_BOTTLE_e:
        mShopItemID = SHOP_ITEMNO_GREEN;
        break;
    case dItemNo_BLUE_BOTTLE_e:
        mShopItemID = SHOP_ITEMNO_BLUE;
        break;
    case dItemNo_DROP_BOTTLE_e:
        mShopItemID = SHOP_ITEMNO_DROP;
        break;
    case dItemNo_ARROW_10_e:
    case dItemNo_ARROW_20_e:
    case dItemNo_ARROW_30_e:
        mShopItemID = SHOP_ITEMNO_ARROW;
        break;
    case dItemNo_SHIELD_e:
        mShopItemID = SHOP_ITEMNO_SHIELD_B;
        break;
    case dItemNo_HYLIA_SHIELD_e:
        mShopItemID = SHOP_ITEMNO_SHIELD_A;
        break;
    case dItemNo_NORMAL_BOMB_e:
    case dItemNo_BOMB_5_e:
    case dItemNo_BOMB_10_e:
    case dItemNo_BOMB_20_e:
    case dItemNo_BOMB_30_e:
        mShopItemID = SHOP_ITEMNO_BOMB;
        break;
    case dItemNo_WATER_BOMB_e:
        mShopItemID = SHOP_ITEMNO_WATER_BOMB;
        break;
    case dItemNo_POKE_BOMB_e:
        mShopItemID = SHOP_ITEMNO_POKE_BOMB;
        break;
    case dItemNo_BOMB_BAG_LV1_e:
    case dItemNo_BOMB_IN_BAG_e:
        mShopItemID = SHOP_ITEMNO_BOMB_BAG;
        break;
    case dItemNo_BOMB_BAG_LV2_e:
        mShopItemID = SHOP_ITEMNO_BOMB_BAG_LV2;
        break;
    case dItemNo_BEE_CHILD_e:
        mShopItemID = SHOP_ITEMNO_BEE_CHILD;
        break;
    case dItemNo_PACHINKO_e:
        mShopItemID = SHOP_ITEMNO_PACHINKO;
        break;
    case dItemNo_HAWK_EYE_e:
        mShopItemID = SHOP_ITEMNO_HAWKEYE;
        break;
    case dItemNo_ARMOR_e:
        mShopItemID = SHOP_ITEMNO_ARMOR;
        break;
    default:
        return NULL;
    }

    return mData[mShopItemID].get_arcName();
}

const f32 daShopItem_c::m_cullfar_max = 5000.0f;

u16 daShopItem_c::getHeapSize() {
    static const u16 HeapSizeTbl[] = {
        0x0810, 0x0810, 0x0810, 0x0810, 0x0810, 0x0810, 0x0810, 0x0810,
        0x0EB0, 0x0810, 0x0EB0, 0x3990, 0x3990, 0x8000, 0x0810, 0x1540,
        0x0810, 0x0810, 0x0810, 0x43A0, 0x2280, 0x0810, 0x0810,
    };

    u8 a_ShopItemID = getShopItemID();
    OS_REPORT("ShopItemID [%u]\n", a_ShopItemID);
    ASSERT(a_ShopItemID < SHOP_ITEMNO_MAX);

    return HeapSizeTbl[a_ShopItemID];
}

void daShopItem_c::CreateInit() {
    fopAcM_SetMtx(this, mMtx);
    fopAcM_setCullSizeBox(this, -100.0f, 0.0f, -100.0f, 100.0f, 200.0f, 100.0f);

    if (mDoLib_clipper::getFar() > 1.0f) {
        fopAcM_setCullSizeFar(this, 5000.0f / mDoLib_clipper::getFar());
    }

    show();

    if (strcmp("R_SP109", dComIfGp_getStartStageName()) == 0 && dComIfGp_getStartStageRoomNo() == 1)
    {
        scale.set(mData[mShopItemID].get_scale() * 0.8f, mData[mShopItemID].get_scale() * 0.8f,
                  mData[mShopItemID].get_scale() * 0.8f);
    } else {
        scale.set(mData[mShopItemID].get_scale(), mData[mShopItemID].get_scale(),
                  mData[mShopItemID].get_scale());
    }

    home.pos = current.pos;
    set_mtx();
    mpModel->setUserArea(0);
}

void daShopItem_c::set_mtx() {
    mpModel->setBaseScale(scale);

    if (daShopItem_prm::getGroup(this) == 15) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    } else {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y + mData[mShopItemID].get_offsetY(),
                               current.pos.z);
    }

    MTXCopy(mDoMtx_stack_c::get(), mMtx);
    mDoMtx_stack_c::YrotM(mAngleY);

    if (daShopItem_prm::getGroup(this) == 15) {
        mDoMtx_stack_c::ZXYrotM(-11300, 32700, 7300);
    } else {
        mDoMtx_stack_c::ZXYrotM(mAngleX + mData[mShopItemID].get_angleX(),
                                mData[mShopItemID].get_angleY(), mData[mShopItemID].get_angleZ());
    }

    mDoMtx_stack_c::ZXYrotM(current.angle.x, current.angle.y, current.angle.z);

    if (daShopItem_prm::getGroup(this) == 15) {
        mDoMtx_stack_c::XrotM(0);
    } else {
        mDoMtx_stack_c::XrotM(mData[mShopItemID].get_angleOffsetX());
    }

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

bool daShopItem_c::_execute() {
    animPlay(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    set_mtx();
    return true;
}

bool daShopItem_c::_draw() {
    if (!chkDraw()) {
        return true;
    }

    DrawBase();
    return true;
}

void daShopItem_c::setListStart() {}

void daShopItem_c::setShadow() {
    if (m_itemNo != dItemNo_NONE_e) {
        daItemBase_c::setShadow();
    } else {
        mShadowKey = dComIfGd_setShadow(mShadowKey, 1, mpModel, &current.pos, 80.0f, 0.0f,
                                        current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0,
                                        1.0f, dDlst_shadowControl_c::getSimpleTex());
    }
}

BOOL daShopItem_c::chkFlag(int i_flag) {
    return mData[mShopItemID].get_flag() & i_flag;
}

s8 daShopItem_c::getTevFrm() {
    return mData[mShopItemID].get_tevfrm();
}

s8 daShopItem_c::getBtpFrm() {
    return mData[mShopItemID].get_btpfrm();
}

u8 daShopItem_c::getShadowSize() {
    return mData[mShopItemID].get_shadowSize();
}

u8 daShopItem_c::getCollisionH() {
    return mData[mShopItemID].get_collisionH();
}

u8 daShopItem_c::getCollisionR() {
    return mData[mShopItemID].get_collisionR();
}

int daShopItem_c::_create() {
    fopAcM_ct(this, daShopItem_c);

    mAngleX = 0;
    mAngleY = 0;
    m_itemNo = daShopItem_prm::getItemNo(this);

    if (getShopArcname() == NULL) {
        // "Display model archive name doesn't exist![%d]\n"
        OS_REPORT("陳列用モデルのアーカイブ名がありません！[%d]\n", m_itemNo);
        m_itemNo = dItemNo_NONE_e;
    }

    int phase_state = dComIfG_resLoad(&mPhase, getShopArcname());
    switch (phase_state) {
    case cPhs_COMPLEATE_e:
        if (phase_state == cPhs_COMPLEATE_e) {
            if (!fopAcM_entrySolidHeap(this, CheckShopItemCreateHeap, getHeapSize())) {
                return cPhs_ERROR_e;
            }

            // "Shop Item[%d]Memory:%d used\n"
            OS_REPORT("ショップアイテム[%d]メモリ:%x 使用\n", m_itemNo, heap->getHeapSize());
            CreateInit();
        }
        break;
    }

    return phase_state;
}

static int daShopItem_Create(void* i_this) {
    return static_cast<daShopItem_c*>(i_this)->_create();
}

int daShopItem_c::_delete() {
    DeleteBase(getShopArcname());
    return 1;
}

static int daShopItem_Delete(void* i_this) {
    return static_cast<daShopItem_c*>(i_this)->_delete();
}

static int daShopItem_Draw(void* i_this) {
    return static_cast<daShopItem_c*>(i_this)->_draw();
}

static int daShopItem_Execute(void* i_this) {
    return static_cast<daShopItem_c*>(i_this)->_execute();
}

static int daShopItem_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daShopItemMethodTable = {
    (process_method_func)daShopItem_Create,  (process_method_func)daShopItem_Delete,
    (process_method_func)daShopItem_Execute, (process_method_func)daShopItem_IsDelete,
    (process_method_func)daShopItem_Draw,
};

actor_process_profile_definition g_profile_ShopItem = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_ShopItem_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daShopItem_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_ShopItem_e,
    /* Actor SubMtd */ &daShopItemMethodTable,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_NOPAUSE_e | fopAcStts_CULL_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
