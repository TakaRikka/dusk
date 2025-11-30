/**
 * d_a_tag_lv5soup.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_lv5soup.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"

int daTag_Lv5Soup_c::create() {
    fopAcM_ct(this, daTag_Lv5Soup_c);
    fopAcM_setCullSizeBox(this, -30.0f, -15.0f, -30.0f, 30.0f, 45.0f, 30.0f);
    attention_info.flags = 0;
    return cPhs_COMPLEATE_e;
}

int daTag_Lv5Soup_c::Delete() {
    return 1;
}

int daTag_Lv5Soup_c::Execute() {
    if (isAreaCheck()) {
        // Yeto took pumpkin and pumpkin not in soup
        if (dComIfGs_isEventBit(0x0020) && !dComIfGs_isEventBit(0x0002)) {
            // Pumpkin in soup
            dComIfGs_onEventBit(0x0002);
        }

        // Yeto took cheese and cheese not in soup
        if (dComIfGs_isEventBit(0x0010) && !dComIfGs_isEventBit(0x0001)) {
            // Cheese in soup
            dComIfGs_onEventBit(0x0001);
        }
    }
    return 1;
}

int daTag_Lv5Soup_c::Draw() {
    return 1;
}

bool daTag_Lv5Soup_c::isAreaCheck() {
    cXyz dist = daPy_getPlayerActorClass()->current.pos - current.pos;
    f32 boundX = scale.x * 100.0f;
    f32 boundY = scale.y * 100.0f;

    if (dist.absXZ() < boundX && -boundY < dist.y && dist.y < boundY) {
        return true;
    } else {
        return false;
    }
}

static int daTag_Lv5Soup_Create(void* i_this) {
    return static_cast<daTag_Lv5Soup_c*>(i_this)->create();
}

static int daTag_Lv5Soup_Delete(void* i_this) {
    return static_cast<daTag_Lv5Soup_c*>(i_this)->Delete();
}

static int daTag_Lv5Soup_Execute(void* i_this) {
    return static_cast<daTag_Lv5Soup_c*>(i_this)->Execute();
}

static int daTag_Lv5Soup_Draw(void* i_this) {
    return static_cast<daTag_Lv5Soup_c*>(i_this)->Draw();
}

static int daTag_Lv5Soup_IsDelete(void* i_this) {
    return 1;
}

daTag_Lv5Soup_c::~daTag_Lv5Soup_c() {}

static actor_method_class daTag_Lv5Soup_MethodTable = {
    (process_method_func)daTag_Lv5Soup_Create,  (process_method_func)daTag_Lv5Soup_Delete,
    (process_method_func)daTag_Lv5Soup_Execute, (process_method_func)daTag_Lv5Soup_IsDelete,
    (process_method_func)daTag_Lv5Soup_Draw,
};

extern actor_process_profile_definition g_profile_TAG_LV5SOUP = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    0x122,
    &g_fpcLf_Method.base,
    sizeof(daTag_Lv5Soup_c),
    0,
    0,
    &g_fopAc_Method.base,
    70,
    &daTag_Lv5Soup_MethodTable,
    0x64100,
    5,
    fopAc_CULLBOX_CUSTOM_e,
};
