/**
 * d_a_shop_item.cpp
 * Shop Item Actor
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_shop_item.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_lib.h"

const char* daShopItem_c::getShopArcname() {
    switch (m_itemNo) {
    case fpcNm_ITEM_NONE:
        if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0) {
            mShopItemID = SHOP_ITEMNO_ARMOR_SOLD;
        } else {
            mShopItemID = SHOP_ITEMNO_SOLD;
        }
        break;
    case fpcNm_ITEM_OIL_BOTTLE:
        mShopItemID = SHOP_ITEMNO_OIL;
        break;
    case fpcNm_ITEM_MILK_BOTTLE:
        mShopItemID = SHOP_ITEMNO_MILK;
        break;
    case fpcNm_ITEM_HALF_MILK_BOTTLE:
        mShopItemID = SHOP_ITEMNO_HALF_MILK;
        break;
    case fpcNm_ITEM_RED_BOTTLE:
        mShopItemID = SHOP_ITEMNO_RED;
        break;
    case fpcNm_ITEM_GREEN_BOTTLE:
        mShopItemID = SHOP_ITEMNO_GREEN;
        break;
    case fpcNm_ITEM_BLUE_BOTTLE:
        mShopItemID = SHOP_ITEMNO_BLUE;
        break;
    case fpcNm_ITEM_DROP_BOTTLE:
        mShopItemID = SHOP_ITEMNO_DROP;
        break;
    case fpcNm_ITEM_ARROW_10:
    case fpcNm_ITEM_ARROW_20:
    case fpcNm_ITEM_ARROW_30:
        mShopItemID = SHOP_ITEMNO_ARROW;
        break;
    case fpcNm_ITEM_SHIELD:
        mShopItemID = SHOP_ITEMNO_SHIELD_B;
        break;
    case fpcNm_ITEM_HYLIA_SHIELD:
        mShopItemID = SHOP_ITEMNO_SHIELD_A;
        break;
    case fpcNm_ITEM_NORMAL_BOMB:
    case fpcNm_ITEM_BOMB_5:
    case fpcNm_ITEM_BOMB_10:
    case fpcNm_ITEM_BOMB_20:
    case fpcNm_ITEM_BOMB_30:
        mShopItemID = SHOP_ITEMNO_BOMB;
        break;
    case fpcNm_ITEM_WATER_BOMB:
        mShopItemID = SHOP_ITEMNO_WATER_BOMB;
        break;
    case fpcNm_ITEM_POKE_BOMB:
        mShopItemID = SHOP_ITEMNO_POKE_BOMB;
        break;
    case fpcNm_ITEM_BOMB_BAG_LV1:
    case fpcNm_ITEM_BOMB_IN_BAG:
        mShopItemID = SHOP_ITEMNO_BOMB_BAG;
        break;
    case fpcNm_ITEM_BOMB_BAG_LV2:
        mShopItemID = SHOP_ITEMNO_BOMB_BAG_LV2;
        break;
    case fpcNm_ITEM_BEE_CHILD:
        mShopItemID = SHOP_ITEMNO_BEE_CHILD;
        break;
    case fpcNm_ITEM_PACHINKO:
        mShopItemID = SHOP_ITEMNO_PACHINKO;
        break;
    case fpcNm_ITEM_HAWK_EYE:
        mShopItemID = SHOP_ITEMNO_HAWKEYE;
        break;
    case fpcNm_ITEM_ARMOR:
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
    mpModel->setUserArea(NULL);
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
    if (m_itemNo != fpcNm_ITEM_NONE) {
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
        m_itemNo = fpcNm_ITEM_NONE;
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
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_ShopItem,
    &g_fpcLf_Method.base,
    sizeof(daShopItem_c),
    0,
    0,
    &g_fopAc_Method.base,
    242,
    &daShopItemMethodTable,
    0x60100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
