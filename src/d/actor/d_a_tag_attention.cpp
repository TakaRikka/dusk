/**
 * d_a_tag_attention.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_attention.h"

#include "d/actor/d_a_player.h"
#include "d/d_debug_viewer.h"

#if DEBUG
daAttp_HIO_c l_HIO;

void daAttp_HIO_c::genMessage(JORMContext* ctx) {
    static const char* l_type_name[37] = {
        "注目なし - ダミー",       // No focus - dummy
        "注目だけ - 標準",        // Focus only - standard
        "会話     - 標準",      // Conversation - standard
        "戦闘     - 標準",      // Combat - standard
        "戦闘Ｂ   - 標準",       // Combat B - standard
        "会話Ｂ   - 標準",       // Conversation B - standard
        "持上げ   - 標準",       // Lifting - standard
        "持上げ   - タル",       // Lifting - barrel
        "持上げ   - 壷大",       // Lifting - large jar
        "持上げ   - 水壷",       // Lifting - water jar
        "持上げ   - 椅子",       // Lifting - chair
        "持上げ   - 小石",       // Lifting - pebble
        "持上げ   - 岩",        // Lifting - rock
        "馬",                // Horse
        "ドア、宝箱",            // Door, treasure chest
        "持上げボコの棒",          // Lifting stick
        "敵氷",               // Enemy ice
        "引き連れられ石像",         // Dragged stone statue
        "引き連れられ石像",         // Dragged stone statue
        "ジャガ－",             // Jaguar
        "接近会話",             // Close-range conversation
        "長距離戦闘",            // Long-range combat
        "短距離戦闘",            // Short-range combat
        "予備戦闘",             // Reserve combat
        "予備戦闘",             // Reserve combat
        "予備戦闘",             // Reserve combat
        "床レバー用",            // Floor lever
        "狼鎖",               // Wolf chain
        "ＮＰＣナビィ － ハイラル城、呼びかけデモ用（会話）",   // NPC Navii - Hyrule Castle, demo call (conversation)
        "ＮＰＣナビィ － ハイラル城、呼びかけデモ用（注目）",   // NPC Navii - Hyrule Castle, demo call (focus)
        "狼掘る",              // Wolf digging
        "カヌー",              // Canoe
        "覗きタグ",             // Peek tag
        "馬戦闘",              // Horse combat
        "注目点　通常長さ",         // Focus point - normal length
        "注目点　長注目版",         // Focus point - long focus
        "調整用カスタム",          // Adjustment custom
    };
    static const u8 l_type[37] = {
        0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 14, 15, 16,
        17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
        33, 34, 35, 36, 37,
    };
    static const int num = 37;

    // "Focus point"
    ctx->genLabel("注目点", 0, 0, NULL, -1, -1, 0x200, 0x18);
    // "Type switching ON"
    ctx->genCheckBox("タイプ切り替えＯＮ", &enable_type_switching, 1, 0, NULL, -1, -1, 0x200, 0x18);

    // "Type specification"
    ctx->startComboBox("タイプ指定", &type_spec, 0, NULL, -1, -1, 0x100, 0x1a);
    for (int i = 0; i < num; i++) {
        ctx->genComboBoxItem(l_type_name[i], l_type[i]);
    }
    ctx->endComboBox();

    // "Distance display ON"
    ctx->genCheckBox("距離表示ＯＮ", &enable_distance_display, 1, 0, NULL, -1, -1, 0x200, 0x18);

    // "For adjustment"
    ctx->genLabel("調整用", 0, 0, NULL, -1, -1, 0x200, 0x18);
    ctx->genSlider("on", &dist_max, 0.0f, 10000.0f, 0, NULL, -1, -1, 0x200, 0x18);
    ctx->genSlider("off", &dist_max_release, 0.0f, 10000.0f, 0, NULL, -1, -1, 0x200, 0x18);
    ctx->genSlider("angle adjust", &dist_adjust, 0.0f, 10000.0f, 0, NULL, -1, -1, 0x200, 0x18);
    ctx->genSlider("hi", &upper_y, -10000.0f, 10000.0f, 0, NULL, -1, -1, 0x200, 0x18);
    ctx->genSlider("low", &lower_y, -10000.0f, 10000.0f, 0, NULL, -1, -1, 0x200, 0x18);
}
#endif

int daAttp_c::Create() {
    static const u8 l_dist_tbl[] = { 35, 36 };

    u8 arg4 = daAttp_prm::getArg4(this);
    attention_info.distances[fopAc_attn_LOCK_e] = l_dist_tbl[arg4];

#if DEBUG
    field_0x598 = l_dist_tbl[arg4];

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    cMtx_copy(mDoMtx_stack_c::get(), field_0x568);
    fopAcM_SetMtx(this, field_0x568);
#endif

    execute();
    return 1;
}

int daAttp_c::create() {
    fopAcM_ct(this, daAttp_c);
    Create();

#if DEBUG
    // "Key points"
    l_HIO.entryHIO("注目点");
#endif

    return cPhs_COMPLEATE_e;
}

int daAttp_c::execute() {
#if DEBUG
    if (l_HIO.enable_type_switching) {
        attention_info.distances[fopAc_attn_LOCK_e] = l_HIO.type_spec;
    } else {
        attention_info.distances[fopAc_attn_LOCK_e] = field_0x598;
    }

    dist_entry& distTable = dAttention_c::getDistTable(37);
    distTable.mDistMax = l_HIO.dist_max;
    distTable.mDistMaxRelease = l_HIO.dist_max_release;
    distTable.mDistanceAdjust = l_HIO.dist_adjust;
    distTable.mUpperY = l_HIO.upper_y;
    distTable.mLowerY = l_HIO.lower_y;
#endif

    bool bVar1 = true;
    fopAc_ac_c& link = *daPy_getPlayerActorClass();

    cXyz pos = link.current.pos - current.pos;
    mDoMtx_stack_c::YrotS(-shape_angle.y);
    mDoMtx_stack_c::multVec(&pos, &pos);

    switch (daAttp_prm::getArg2(this)) {
    case 0:
        if (pos.z < 0) {
            bVar1 = false;
        }
        break;
    case 1:
        if (pos.y > 0) {
            bVar1 = false;
        }
        break;
    case 2:
        if (pos.y < 0) {
            bVar1 = false;
        }
        break;
    case 3:
        if (pos.y > 0 || pos.z < 0) {
            bVar1 = false;
        }
        break;
    case 4:
        if (pos.y < 0 || pos.z < 0) {
            bVar1 = false;
        }
        break;
    default:
        JUT_ASSERT(432, FALSE);
        break;
    case 7:
        break;
    }

    if (fopAcM_isSwitch(this, daAttp_prm::getSw1(this))) {
        switch (daAttp_prm::getArg1(this)) {
        case 0:
            break;
        case 1:
            if (daPy_py_c::checkNowWolf()) {
                bVar1 = false;
            }
            break;
        case 2:
            if (!daPy_py_c::checkNowWolf()) {
                bVar1 = false;
            }
            break;
        case 3:
            bVar1 = false;
            break;
        default:
            JUT_ASSERT(461, FALSE);
        }
    } else {
        switch (daAttp_prm::getArg0(this)) {
        case 0:
            break;
        case 1:
            if (daPy_py_c::checkNowWolf()) {
                bVar1 = false;
            }
            break;
        case 2:
            if (!daPy_py_c::checkNowWolf()) {
                bVar1 = false;
            }
            break;
        case 3:
            bVar1 = false;
            break;
        default:
            JUT_ASSERT(485, FALSE);
        }

        if (daAttp_prm::getSw2(this) != 0xff) {
            if (fopAcM_isSwitch(this, daAttp_prm::getSw2(this))) {
                if (daAttp_prm::getArg3(this) == 0) {
                    bVar1 = false;
                }
            } else if (daAttp_prm::getArg3(this) == 1) {
                bVar1 = false;
            }
        }
    }

    if (bVar1) {
        attention_info.flags |= fopAc_AttnFlag_LOCK_e;
    } else {
        attention_info.flags &= ~fopAc_AttnFlag_LOCK_e;
    }

    return 1;
}

int daAttp_c::draw() {
#if DEBUG
    if (l_HIO.enable_distance_display) {
        GXColor color1 = {0xff, 0, 0, 0x80};
        GXColor color2 = {0, 0, 0xff, 0x80};
        cXyz sp10(current.pos);
        sp10.y += 5.0f;
        dist_entry& distTable =
            dAttention_c::getDistTable(attention_info.distances[fopAc_attn_LOCK_e]);

        dDbVw_drawCircleXlu(current.pos, distTable.mDistMax, color2, 1, 12);
        dDbVw_drawCircleXlu(current.pos, distTable.mDistMaxRelease, color1, 1, 12);
    }
#endif

    return 1;
}

int daAttp_c::_delete() {
#if DEBUG
    l_HIO.removeHIO();
#endif

    return 1;
}

daAttp_HIO_c::daAttp_HIO_c() {
    type_spec = 0;
    dist_max = dist_max_release = dist_adjust = upper_y = lower_y = 0.0f;
}

static int daAttp_Draw(daAttp_c* i_this) {
    return i_this->draw();
}


static int daAttp_Execute(daAttp_c* i_this) {
    return i_this->execute();
}

static int daAttp_Delete(daAttp_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    return i_this->_delete();
}

static int daAttp_Create(fopAc_ac_c* i_this) {
    daAttp_c* attp = static_cast<daAttp_c*>(i_this);
    fpc_ProcID id = fopAcM_GetID(i_this);
    return attp->create();
}

static actor_method_class l_daAttp_Method = {
    (process_method_func)daAttp_Create,  (process_method_func)daAttp_Delete,
    (process_method_func)daAttp_Execute, (process_method_func)NULL,
    (process_method_func)daAttp_Draw,
};

actor_process_profile_definition g_profile_Tag_Attp = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Tag_Attp,          // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daAttp_c),       // mSize
    0,                      // mSizeOther
    0,                      // mParameter
    &g_fopAc_Method.base,   // sub_method
    440,                    // mPriority
    &l_daAttp_Method,       // sub_method
    0x40100,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_0_e,      // cullType   
};
