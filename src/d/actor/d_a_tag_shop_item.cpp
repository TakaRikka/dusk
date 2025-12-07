/**
 * d_a_tag_shop_item.cpp
 * Tag - Shop Item
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_shop_item.h"
#include "d/d_procname.h"

int daTag_ShopItem_c::create() {
    fopAcM_ct(this, daTag_ShopItem_c);
    initialize();
    field_0x572 = false;

    if (strcmp("F_SP109", dComIfGp_getStartStageName()) == 0) {
        // saveBitLabels[64] = Goron Mines completed
        if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[64])) {
            return cPhs_ERROR_e;
        }

        if (dComIfGs_getTime() >= 90.0f && dComIfGs_getTime() < 270.0f) {
            return cPhs_ERROR_e;
        }
    }

    if (getGroupID() == 15) {
        mCreateTimer = 150;

        // "Event Item\n"
        OS_REPORT("イベントアイテム\n");

        if (getSwitchBit1() != 0xFF) {
            if (!dComIfGs_isSaveSwitch(getSwitchBit1())) {
                dComIfGs_onSaveSwitch(getSwitchBit2());
                field_0x572 = true;
                mCreateTimer = 0;
                // "Before Talk\n"
                OS_REPORT("会話前\n");
            } else {
                // "After Talk\n"
                OS_REPORT("会話後\n");
            }
        }
    } else {
        mCreateTimer = 0;

        if (getSwitchBit1() != 0xFF) {
            if (dComIfGs_isSaveSwitch(getSwitchBit1())) {
                mCreateTimer = 150;
                // "Already Sold\n"
                OS_REPORT("もう売れたよ\n");
            }
        }

        if (getSwitchBit2() != 0xFF) {
            if (!dComIfGs_isSaveSwitch(getSwitchBit2())) {
                mCreateTimer = 150;
                // "Not sold yet\n"
                OS_REPORT("まだ売れない\n");
            }
        }
    }

    return cPhs_COMPLEATE_e;
}

int daTag_ShopItem_c::Delete() {
    if (field_0x572 && getGroupID() == 15) {
        if (dComIfGs_isSaveSwitch(getSwitchBit1())) {
            dComIfGs_offSaveSwitch(getSwitchBit2());
        }
    }

    return 1;
}

int daTag_ShopItem_c::Execute() {
    if (mProcessID == fpcM_ERROR_PROCESS_ID_e) {
        if (mCreateTimer == 0) {
            mProcessID =
                fopAcM_create(PROC_ShopItem, (getType() & 0xFF) | (getGroupID() << 0x1C),
                              &current.pos, fopAcM_GetRoomNo(this), &current.angle, NULL, -1);

        } else {
            mCreateTimer--;

            if (getGroupID() == 15) {
                if (field_0x572) {
                    mCreateTimer = 0;
                }
            } else {
                if (getSwitchBit1() != 0xFF && dComIfGs_isSaveSwitch(getSwitchBit1())) {
                } else if (getSwitchBit2() == 0xFF || dComIfGs_isSaveSwitch(getSwitchBit2())) {
                    mCreateTimer = 0;
                }
            }
        }
    } else {
        daItemBase_c* item_actor_p = (daItemBase_c*)fopAcM_SearchByID(mProcessID);

        if (item_actor_p != NULL) {
            if (getGroupID() == 15) {
                if (!field_0x572) {
                    item_actor_p->hide();
                } else {
                    item_actor_p->show();
                }
            } else {
                if (getSwitchBit1() != 0xFF && dComIfGs_isSaveSwitch(getSwitchBit1())) {
                    item_actor_p->hide();
                } else if (getSwitchBit2() != 0xFF && !dComIfGs_isSaveSwitch(getSwitchBit2())) {
                    item_actor_p->hide();
                } else {
                    item_actor_p->show();
                }
            }
        }
    }

    return 1;
}

int daTag_ShopItem_c::Draw() {
    return 1;
}

u8 daTag_ShopItem_c::getType() {
    return fopAcM_GetParam(this) & 0xFF;
}

u8 daTag_ShopItem_c::getGroupID() {
    return fopAcM_GetParam(this) >> 0x1C;
}

u8 daTag_ShopItem_c::getSwitchBit1() {
    return home.angle.z & 0xFF;
}

u8 daTag_ShopItem_c::getSwitchBit2() {
    return home.angle.z >> 8 & 0xFF;
}

void daTag_ShopItem_c::initialize() {
    fopAcM_setCullSizeBox(this, -30.0f, -15.0f, -30.0f, 30.0f, 45.0f, 30.0f);
    attention_info.flags = 0;
    current.angle.set(0, home.angle.y, 0);
    shape_angle = current.angle;
    mProcessID = -1;
}

static int daTag_ShopItem_Create(void* i_this) {
    return static_cast<daTag_ShopItem_c*>(i_this)->create();
}

static int daTag_ShopItem_Delete(void* i_this) {
    return static_cast<daTag_ShopItem_c*>(i_this)->Delete();
}

static int daTag_ShopItem_Execute(void* i_this) {
    return static_cast<daTag_ShopItem_c*>(i_this)->Execute();
}

static int daTag_ShopItem_Draw(void* i_this) {
    return static_cast<daTag_ShopItem_c*>(i_this)->Draw();
}

static int daTag_ShopItem_IsDelete(void* i_this) {
    return 1;
}

daTag_ShopItem_c::~daTag_ShopItem_c() {}

static actor_method_class daTag_ShopItem_MethodTable = {
    (process_method_func)daTag_ShopItem_Create,  (process_method_func)daTag_ShopItem_Delete,
    (process_method_func)daTag_ShopItem_Execute, (process_method_func)daTag_ShopItem_IsDelete,
    (process_method_func)daTag_ShopItem_Draw,
};

extern actor_process_profile_definition g_profile_TAG_SHOPITM = {
    fpcLy_CURRENT_e,              // mLayerID
    7,                            // mListID
    fpcPi_CURRENT_e,              // mListPrio
    PROC_TAG_SHOPITM,             // mProcName
    &g_fpcLf_Method.base,        // sub_method
    sizeof(daTag_ShopItem_c),     // mSize
    0,                            // mSizeOther
    0,                            // mParameters
    &g_fopAc_Method.base,         // sub_method
    75,                           // mPriority
    &daTag_ShopItem_MethodTable,  // sub_method
    0x64100,                      // mStatus
    5,                            // mActorType
    fopAc_CULLBOX_CUSTOM_e,       // cullType
};
