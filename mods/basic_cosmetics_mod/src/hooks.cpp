#include "hooks.hpp"

#include "mod.hpp"
#include "color_utils.hpp"
#include "midna_hair_color.hpp"
#include "texture_utils.hpp"
#include "types.h"

#include "mods/svc/hook.hpp"
#include "mods/svc/log.hpp"

#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_midna.h"
#include "d/d_kankyo.h"
#include "d/d_kantera_icon_meter.h"
#include "d/d_meter2.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"
#include "d/d_pane_class.h"
#include "m_Do/m_Do_dvd_thread.h"

#include <optional>

DEFINE_HOOK_SYMBOL(
    "mDoDvdThd_mountArchive_c::execute", s32(mDoDvdThd_mountArchive_c*), MountArchiveExecute);
void mount_archive_execute_post(ModContext*, void* args, void* retval, void* userdata) {
    auto archive = mods::arg<mDoDvdThd_mountArchive_c*>(args, 0);
    handle_texture_overrides_on_load(archive);
}

// Helper for getting lantern color
std::optional<GXColor> get_lantern_color() {
    auto lanternColorStr = get_str_option(get_cvars().lanternGlowColor, "");
    if (is_valid_hex_color_str(lanternColorStr) || lanternColorStr == "rainbow") {
        if (is_valid_hex_color_str(lanternColorStr)) {
            return hex_color_str_to_gx_color(lanternColorStr);
        }

        // Assume rainbow if not a valid hex str
        auto lanternColor = get_rainbow_rgb(127.5f);
        lanternColor.r /= 2;
        lanternColor.g /= 2;
        lanternColor.b /= 2;
        return lanternColor;
    }

    return std::nullopt;
}

// Override Lantern ambience color
DEFINE_HOOK(&dKy_WolfEyeLight_set, WolfEyeLightSet);
void wolf_eye_light_set_post(ModContext*, void* args, void* retval, void* userdata) {
    auto maybeLanternColor = get_lantern_color();
    if (maybeLanternColor.has_value()) {
        auto lanternColor = maybeLanternColor.value();

        dScnKy_env_light_c* kankyo = dKy_getEnvlight();
        kankyo->field_0x0c18[0].mColor.r = lanternColor.r;
        kankyo->field_0x0c18[0].mColor.g = lanternColor.g;
        kankyo->field_0x0c18[0].mColor.b = lanternColor.b;
    }
}

// Override Lantern Sphere color
DEFINE_HOOK(&daAlink_c::preKandelaarDraw, PreKandelaarDraw);
void pre_kandelaar_draw_post(ModContext*, void* args, void* retval, void* userdata) {
    auto maybeLanternColor = get_lantern_color();
    if (maybeLanternColor.has_value()) {
        auto lanternColor = maybeLanternColor.value();

        J3DMaterial* mat_p = daAlink_getAlinkActorClass()->mpKanteraGlowModel->getModelData()->getMaterialNodePointer(0);

        J3DGXColorS10 color;
        color.r = lanternColor.r;
        color.g = lanternColor.g;
        color.b = lanternColor.b;
        color.a = 255;
        mat_p->setTevColor(1, &color);

        color.r = lanternColor.r;
        color.g = lanternColor.g;
        color.b = lanternColor.b;
        mat_p->setTevColor(2, &color);
    }
}

// Override Main Lantern Meter Color
DEFINE_HOOK(&CPaneMgr::setBlackWhite, CPaneMgrSetBlackWhite);
HookAction cpane_mgr_set_black_white_pre(ModContext*, void* args, void* retval, void* userdata) {
    // Check for magic meter
    auto pane = mods::arg<CPaneMgr*>(args, 0);
    if (dMeter2Info_getMeterClass() == NULL || pane != dMeter2Info_getMeterClass()->getMeterDrawPtr()->mpMagicMeter) {
        return HOOK_CONTINUE;
    }

    // If magic meter, check to see we're setting lantern colors
    auto& black = mods::arg_ref<JUtility::TColor>(args, 1);
    auto& white = mods::arg_ref<JUtility::TColor>(args, 2);
    if (black != JUtility::TColor(255, 255, 140, 255) &&
        white != JUtility::TColor(230, 170, 0, 255)) {
        return HOOK_CONTINUE;
    }

    auto maybeLanternColor = get_lantern_color();
    if (maybeLanternColor.has_value()) {
        auto lanternColor = maybeLanternColor.value();
        black = JUtility::TColor(lanternColor.r, lanternColor.g, lanternColor.b, 255);
        white = JUtility::TColor(lanternColor.r, lanternColor.g, lanternColor.b, 255);
    }

    return HOOK_CONTINUE;
}

// Override Lantern Icon Meter Color
DEFINE_HOOK(&dKantera_icon_c::setNowGauge, KanteraIconSetNowGauge);
void kantera_icon_set_now_gauge_post(ModContext*, void* args, void* retval, void* userdata) {
    auto maybeLanternColor = get_lantern_color();
    if (maybeLanternColor.has_value()) {
        auto lanternColor = maybeLanternColor.value();

        auto kanteraIcon = mods::arg<dKantera_icon_c*>(args, 0);
        kanteraIcon->mpGauge->setBlackWhite(
            JUtility::TColor(lanternColor.r, lanternColor.g, lanternColor.b, 255),
            JUtility::TColor(lanternColor.r, lanternColor.g, lanternColor.b, 255));
    }
}

// Override Light Sword Effect Color
DEFINE_HOOK(&daAlink_c::setLightningSwordEffect, SetLightningSwordEffect);
void set_lightning_sword_effect_post(ModContext*, void* args, void* retval, void* userdata) {
    auto glowColorStr = get_str_option(get_cvars().lightSwordGlowColor, "");
    if (is_valid_hex_color_str(glowColorStr) || glowColorStr == "rainbow") {
        GXColor glowColor{};
        if (glowColorStr == "rainbow") {
            glowColor = get_rainbow_rgb(127.5f);
        } else {
            glowColor = hex_color_str_to_gx_color(glowColorStr);
        }

        auto link = mods::arg<daAlink_c*>(args, 0);
        if (link->mEquipItem == 0x103 && link->checkNoResetFlg3(daPy_py_c::FLG3_UNK_100000)) {
            for (size_t i = 0; i < 3; i++) {
                auto emitter = dComIfGp_particle_getEmitter(link->field_0x327c[i]);
                if (emitter != NULL) {
                    emitter->setGlobalEnvColor(glowColor.r, glowColor.g, glowColor.b);
                    emitter->setGlobalPrmColor(glowColor.r, glowColor.g, glowColor.b);
                }
            }
        }
    }
}

// Heart Icon tags
static constexpr std::array heart_tags = {
    MULTI_CHAR('hear_00'), MULTI_CHAR('hear_01'), MULTI_CHAR('hear_02'), MULTI_CHAR('hear_03'), MULTI_CHAR('hear_04'), MULTI_CHAR('hear_05'), MULTI_CHAR('hear_06'),
    MULTI_CHAR('hear_07'), MULTI_CHAR('hear_08'), MULTI_CHAR('hear_09'), MULTI_CHAR('hear_10'), MULTI_CHAR('hear_11'), MULTI_CHAR('hear_12'), MULTI_CHAR('hear_13'),
    MULTI_CHAR('hear_14'), MULTI_CHAR('hear_15'), MULTI_CHAR('hear_16'), MULTI_CHAR('hear_17'), MULTI_CHAR('hear_18'), MULTI_CHAR('hear_19'), MULTI_CHAR('bigh_00'),
    MULTI_CHAR('bigh_01'), MULTI_CHAR('bigh_02'), MULTI_CHAR('bigh_03'), /*MULTI_CHAR('bigh_n')*/
};

DEFINE_HOOK(&dMeter2Draw_c::init, dMeter2Init);
void d_meter_2_init_post(ModContext*, void* args, void* retval, void* userdata) {
    auto dMeter2Draw = mods::arg<dMeter2Draw_c*>(args, 0);
    auto heartColorStr = get_str_option(get_cvars().heartColor, "");
    if (is_valid_hex_color_str(heartColorStr)) {
        auto heartColor = hex_color_str_to_gx_color(heartColorStr);
        auto screen = dMeter2Draw->getMainScreenPtr();
        for (auto tag : heart_tags) {
            auto element = static_cast<J2DPicture*>(screen->search(tag));
            if (element != nullptr) {
                element->setBlackWhite(heartColor, JUtility::TColor(200, 200, 200, 255));
            }
        }
    }
}

// Override Midna Hair Color
DEFINE_HOOK(&daMidna_c::create, MidnaCreate);
void midna_create_post(ModContext*, void* args, void* retval, void* userdata) {
    auto step = reinterpret_cast<int*>(retval);
    // check for cPhs_Compleate
    if (*step != 4) {
        return;
    }

    auto midna = mods::arg<daMidna_c*>(args, 0);
    midna->field_0x6e0 = *get_midna_hair_normalColor();
    if (dKy_darkworld_check()) {
        midna->field_0x6e8 = *get_midna_hair_normalKColor();
        midna->field_0x6ec = *get_midna_hair_normalKColor2();
    } else {
        midna->field_0x6e8 = *get_midna_hair_lNormalKColor();
        midna->field_0x6ec = *get_midna_hair_lNormalKColor2();
    }
}

// Override Midna Hair Color Part 2
DEFINE_HOOK(&daMidna_c::setBodyPartMatrix, MidnaSetBodyPartMatrix);

static GXColorS10 midnaField0x6e0{};
static GXColor midnaField0x6e8{};
static GXColor midnaField0x6ec{};

// Copy the original values from before the function
HookAction midna_set_body_part_matrix_pre(ModContext*, void* args, void* retval, void* userdata) {
    auto midna = mods::arg<daMidna_c*>(args, 0);

    midnaField0x6e0 = midna->field_0x6e0;
    midnaField0x6e8 = midna->field_0x6e8;
    midnaField0x6ec = midna->field_0x6ec;

    return HOOK_CONTINUE;
}

void midna_set_body_part_matrix_post(ModContext*, void* args, void* retval, void* userdata) {
    auto midna = mods::arg<daMidna_c*>(args, 0);
    if (midna->mpHairhandBmd != NULL) {
        // Restore the original values from before the function ran (this undoes the chase)
        midna->field_0x6e0 = midnaField0x6e0;
        midna->field_0x6e8 = midnaField0x6e8;
        midna->field_0x6ec = midnaField0x6ec;

        // statement copied from inside function to determine colors
        bool bigColors = midna->checkStateFlg0(daMidna_c::FLG0_UNK_10000000) ||
            midna->mBckHeap[2].getIdx() == daMidna_c::m_anmDataTable[daMidna_c::ANM_HAIR].mResID ||
            midna->mBckHeap[2].getIdx() == daMidna_c::m_anmDataTable[daMidna_c::ANM_S_TAKES].mResID ||
            midna->mBckHeap[2].getIdx() == daMidna_c::m_anmDataTable[daMidna_c::ANM_S_WAITS].mResID ||
            midna->mBckHeap[2].getIdx() == daMidna_c::m_anmDataTable[daMidna_c::ANM_S_PACKAWAY].mResID ||
            midna->mBckHeap[2].getIdx() == daMidna_c::m_anmDataTable[daMidna_c::ANM_GRABST].mResID ||
            midna->checkEndResetStateFlg0(daMidna_c::ERFLG0_UNK_40) ||
            dComIfGp_checkPlayerStatus1(0, 0x800000);

        GXColorS10 color{};
        GXColor kcolor1{};
        GXColor kcolor2{};

        // Then set our own colors
        if (bigColors) {
            kcolor1 = *get_midna_hair_bigKColor();
            if (dKy_darkworld_check()) {
                color = *get_midna_hair_bigColor();
                kcolor2 = *get_midna_hair_normalKColor2();
            } else {
                color = *get_midna_hair_lBigColor();
                kcolor2 = *get_midna_hair_lBigKColor2();
            }
        } else {
            color = *get_midna_hair_normalColor();
            if (dKy_darkworld_check()) {
                kcolor1 = *get_midna_hair_normalKColor();
                kcolor2 = *get_midna_hair_normalKColor2();
            } else {
                kcolor1 = *get_midna_hair_lNormalKColor();
                kcolor2 = *get_midna_hair_lNormalKColor2();
            }
        }

        // And reapply the chase
        cLib_chaseS(&midna->field_0x6e0.r, color.r, 10);
        cLib_chaseS(&midna->field_0x6e0.g, color.g, 10);
        cLib_chaseS(&midna->field_0x6e0.b, color.b, 10);
        cLib_chaseUC(&midna->field_0x6e8.r, kcolor1.r, 10);
        cLib_chaseUC(&midna->field_0x6e8.g, kcolor1.g, 10);
        cLib_chaseUC(&midna->field_0x6e8.b, kcolor1.b, 10);
        cLib_chaseUC(&midna->field_0x6ec.r, kcolor2.r, 10);
        cLib_chaseUC(&midna->field_0x6ec.g, kcolor2.g, 10);
        cLib_chaseUC(&midna->field_0x6ec.b, kcolor2.b, 10);
    }
}

// Override Midna Charge Ring Color
DEFINE_HOOK(&daAlink_c::setWolfLockDomeModel, SetWolfLockDomeModel);
void wolf_lock_dome_model_post(ModContext*, void* args, void* retval, void* userdata) {
    auto domeRingColorStr = get_str_option(get_cvars().midnaChargeRingColor, "");
    if (is_valid_hex_color_str(domeRingColorStr)) {
        auto domeRingColor = hex_color_str_to_gx_color(domeRingColorStr);
        const u8 domeWave1RGBA[3] = {domeRingColor.r, domeRingColor.g, domeRingColor.b};
        const u8 domeWave2RGBA[3] = {domeRingColor.r, domeRingColor.g, domeRingColor.b};
        u8** chromaRegisterTable = reinterpret_cast<u8**>(&daAlink_getAlinkActorClass()->field_0x0724->mAnmCRegDataR);

        for (int i = 0; i < 3; i++)
        {
            u8* currentTable = chromaRegisterTable[i];
            const u8 currentWave1Color = domeWave1RGBA[i];
            const u8 currentWave2Color = domeWave2RGBA[i];
            const u8 currentBaseColor = (currentWave1Color + currentWave2Color) / 2;

            currentTable[0x3] = currentBaseColor;  // Set Alpha for the ring base
            currentTable[0x13] = currentWave1Color; // Set Alpha for ring wave 1
            currentTable[0x23] = currentWave2Color; // Set Alpha for ring wave 2
            currentTable[0xB] = currentBaseColor;  // Set Alpha for darkworld ring base
            currentTable[0x1B] = currentWave1Color; // Set Alpha for darkworld ring wave 1
            currentTable[0x2B] = currentWave2Color; // Set Alpha for darkworld ring wave 2
        }
    }
}

ModResult add_all_hooks() {
    auto result = mods::hook::add_post<MountArchiveExecute>(mount_archive_execute_post);
    if (result != MOD_OK) {
        mods::log::debug("failed to add post hook to mountArchive_execute, Result {}", static_cast<int>(result));
        return result;
    }

    result = mods::hook::add_post<WolfEyeLightSet>(wolf_eye_light_set_post);
    if (result != MOD_OK) {
        mods::log::debug("failed to add post hook to dKy_WolfEyeLight_set, Result {}", static_cast<int>(result));
        return result;
    }

    result = mods::hook::add_post<PreKandelaarDraw>(pre_kandelaar_draw_post);
    if (result != MOD_OK) {
        mods::log::debug("failed to add post hook to daAlink_c::preKandelaarDraw, Result {}", static_cast<int>(result));
        return result;
    }

    result = mods::hook::add_post<SetWolfLockDomeModel>(wolf_lock_dome_model_post);
    if (result != MOD_OK) {
        mods::log::debug("failed to add post hook to daAlink_c::setWolfLockDomeModel, Result {}", static_cast<int>(result));
        return result;
    }

    result = mods::hook::add_pre<CPaneMgrSetBlackWhite>(cpane_mgr_set_black_white_pre);
    if (result != MOD_OK) {
        mods::log::debug("failed to add pre hook to CPaneMgr::setBlackWhite, Result {}", static_cast<int>(result));
        return result;
    }

    result = mods::hook::add_post<KanteraIconSetNowGauge>(kantera_icon_set_now_gauge_post);
    if (result != MOD_OK) {
        mods::log::debug("failed to add post hook to dKantera_icon_c::setNowGauge, Result {}", static_cast<int>(result));
        return result;
    }

    result = mods::hook::add_post<SetLightningSwordEffect>(set_lightning_sword_effect_post);
    if (result != MOD_OK) {
        mods::log::debug("failed to add post hook to daAlink_c::setLightningSwordEffect, Result {}", static_cast<int>(result));
        return result;
    }

    result = mods::hook::add_post<MidnaCreate>(midna_create_post);
    if (result != MOD_OK) {
        mods::log::debug("failed to add post hook to daMidna_c::create, Result {}", static_cast<int>(result));
        return result;
    }

    result = mods::hook::add_pre<MidnaSetBodyPartMatrix>(midna_set_body_part_matrix_pre);
    if (result != MOD_OK) {
        mods::log::debug("failed to add pre hook to daMidna_c::setBodyPartMatrix, Result {}", static_cast<int>(result));
        return result;
    }

    result = mods::hook::add_post<MidnaSetBodyPartMatrix>(midna_set_body_part_matrix_post);
    if (result != MOD_OK) {
        mods::log::debug("failed to add post hook to daMidna_c::setBodyPartMatrix, Result {}", static_cast<int>(result));
        return result;
    }

    result = mods::hook::add_post<dMeter2Init>(d_meter_2_init_post);
    if (result != MOD_OK) {
        mods::log::debug("failed to add post hook to dMeter2Draw_c::init, Result {}", static_cast<int>(result));
        return result;
    }

    return MOD_OK;
}