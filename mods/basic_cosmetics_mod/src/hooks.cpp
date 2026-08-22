#include "hooks.hpp"

#include "color_utils.hpp"
#include "midna_hair_color.hpp"
#include "mod.hpp"
#include "types.h"

#include "mods/svc/hook.hpp"
#include "mods/svc/log.hpp"

#include "SSystem/SComponent/c_phase.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_midna.h"
#include "d/d_a_item_static.h"
#include "d/d_bright_check.h"
#include "d/d_file_sel_info.h"
#include "d/d_file_select.h"
#include "d/d_kankyo.h"
#include "d/d_kantera_icon_meter.h"
#include "d/d_menu_collect.h"
#include "d/d_menu_dmap.h"
#include "d/d_menu_fishing.h"
#include "d/d_menu_fmap2D.h"
#include "d/d_menu_insect.h"
#include "d/d_menu_letter.h"
#include "d/d_menu_option.h"
#include "d/d_menu_ring.h"
#include "d/d_menu_save.h"
#include "d/d_menu_skill.h"
#include "d/d_meter2.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_button.h"
#include "d/d_meter_hakusha.h"
#include "d/d_msg_object.h"
#include "d/d_msg_out_font.h"
#include "d/d_msg_scrn_base.h"
#include "d/d_pane_class.h"

// Lantern ambience color
DEFINE_HOOK(&dKy_WolfEyeLight_set, WolfEyeLightSet);
void wolf_eye_light_set_post(ModContext*, void*, void*, void*) {
    auto maybeLanternColor = get_config_var_color(get_cvars().lanternGlowColor, true);
    if (maybeLanternColor.has_value()) {
        auto lanternColor = maybeLanternColor.value();

        dScnKy_env_light_c* kankyo = dKy_getEnvlight();
        kankyo->field_0x0c18[0].mColor.r = lanternColor.r;
        kankyo->field_0x0c18[0].mColor.g = lanternColor.g;
        kankyo->field_0x0c18[0].mColor.b = lanternColor.b;
    }
}

// Lantern Sphere color
DEFINE_HOOK(&daAlink_c::preKandelaarDraw, PreKandelaarDraw);
void pre_kandelaar_draw_post(ModContext*, void*, void*, void*) {
    auto maybeLanternColor = get_config_var_color(get_cvars().lanternGlowColor, true);
    if (maybeLanternColor.has_value()) {
        auto lanternColor = maybeLanternColor.value();

        J3DMaterial* mat_p = daAlink_getAlinkActorClass()
                                 ->mpKanteraGlowModel->getModelData()
                                 ->getMaterialNodePointer(0);

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

// Main Lantern Meter Color
DEFINE_HOOK(&CPaneMgr::setBlackWhite, CPaneMgrSetBlackWhite);
HookAction cpane_mgr_set_black_white_pre(ModContext*, void* args, void*, void*) {
    // Check for magic meter
    auto pane = mods::arg<CPaneMgr*>(args, 0);
    if (dMeter2Info_getMeterClass() == NULL ||
        pane != dMeter2Info_getMeterClass()->getMeterDrawPtr()->mpMagicMeter)
    {
        return HOOK_CONTINUE;
    }

    // If magic meter, check to see we're setting lantern colors
    auto& black = mods::arg_ref<JUtility::TColor>(args, 1);
    auto& white = mods::arg_ref<JUtility::TColor>(args, 2);
    if (black != JUtility::TColor(255, 255, 140, 255) &&
        white != JUtility::TColor(230, 170, 0, 255))
    {
        return HOOK_CONTINUE;
    }

    auto maybeLanternColor = get_config_var_color(get_cvars().lanternGlowColor, true);
    if (maybeLanternColor.has_value()) {
        auto lanternColor = maybeLanternColor.value();
        black = JUtility::TColor(lanternColor.r, lanternColor.g, lanternColor.b, 255);
        white = JUtility::TColor(lanternColor.r, lanternColor.g, lanternColor.b, 255);
    }

    return HOOK_CONTINUE;
}

// Lantern Icon Meter Color
DEFINE_HOOK(&dKantera_icon_c::setNowGauge, KanteraIconSetNowGauge);
void kantera_icon_set_now_gauge_post(ModContext*, void* args, void*, void*) {
    auto maybeLanternColor = get_config_var_color(get_cvars().lanternGlowColor, true);
    if (maybeLanternColor.has_value()) {
        auto lanternColor = maybeLanternColor.value();

        auto kanteraIcon = mods::arg<dKantera_icon_c*>(args, 0);
        kanteraIcon->mpGauge->setBlackWhite(
            JUtility::TColor(lanternColor.r, lanternColor.g, lanternColor.b, 255),
            JUtility::TColor(lanternColor.r, lanternColor.g, lanternColor.b, 255));
    }
}

// Light Sword Effect Color
DEFINE_HOOK(&daAlink_c::setLightningSwordEffect, SetLightningSwordEffect);
void set_lightning_sword_effect_post(ModContext*, void* args, void*, void*) {
    auto maybeGlowColor = get_config_var_color(get_cvars().lightSwordGlowColor, true);
    if (maybeGlowColor.has_value()) {
        auto glowColor = maybeGlowColor.value();

        auto link = mods::arg<daAlink_c*>(args, 0);
        // Check copied from inside the hooked function
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

void recolor_ui_button(ConfigVarHandle option, u64 tag, J2DScreen* screen) {
    auto buttonColorStr = get_str_option(option, "");
    if (is_valid_hex_color_str(buttonColorStr)) {
        auto color = hex_color_str_to_gx_color(buttonColorStr);
        auto element = static_cast<J2DPicture*>(screen->search(tag));
        if (element != nullptr) {
            element->setBlackWhite(
                JUtility::TColor(0, 0, 0, 0), JUtility::TColor(color.r, color.g, color.b, 0xFF));
        }
    }
}

// Main gameplay UI. Top left hearts and top right buttons
DEFINE_HOOK(&dMeter2Draw_c::init, dMeter2Init);
void d_meter_2_init_post(ModContext*, void* args, void*, void*) {
    auto dMeter2Draw = mods::arg<dMeter2Draw_c*>(args, 0);
    auto screen = dMeter2Draw->getMainScreenPtr();

    // Heart tags on main UI
    static constexpr std::array kHeartTags = {MULTI_CHAR('hear_00'), MULTI_CHAR('hear_01'),
        MULTI_CHAR('hear_02'), MULTI_CHAR('hear_03'), MULTI_CHAR('hear_04'), MULTI_CHAR('hear_05'),
        MULTI_CHAR('hear_06'), MULTI_CHAR('hear_07'), MULTI_CHAR('hear_08'), MULTI_CHAR('hear_09'),
        MULTI_CHAR('hear_10'), MULTI_CHAR('hear_11'), MULTI_CHAR('hear_12'), MULTI_CHAR('hear_13'),
        MULTI_CHAR('hear_14'), MULTI_CHAR('hear_15'), MULTI_CHAR('hear_16'), MULTI_CHAR('hear_17'),
        MULTI_CHAR('hear_18'), MULTI_CHAR('hear_19'), MULTI_CHAR('bigh_00'), MULTI_CHAR('bigh_01'),
        MULTI_CHAR('bigh_02'), MULTI_CHAR('bigh_03')};

    auto maybeHeartColor = get_config_var_color(get_cvars().heartColor);
    if (maybeHeartColor.has_value()) {
        auto heartColor = maybeHeartColor.value();
        for (auto tag : kHeartTags) {
            auto element = static_cast<J2DPicture*>(screen->search(tag));
            if (element != nullptr) {
                element->setBlackWhite(heartColor, JUtility::TColor(200, 200, 200, 255));
            }
        }
    }

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('a_btn'), screen);
    recolor_ui_button(get_cvars().bButtonColor, MULTI_CHAR('b_btn'), screen);
    recolor_ui_button(get_cvars().xButtonColor, MULTI_CHAR('x_btn'), screen);
    recolor_ui_button(get_cvars().yButtonColor, MULTI_CHAR('y_btn'), screen);
    recolor_ui_button(get_cvars().zButtonColor, MULTI_CHAR('zbtn'), screen);
}

// Hearts on the file select screen
DEFINE_HOOK(&dFile_info_c::setHeartCnt, FileInfoSetHeartCount);
void file_info_set_heart_count_post(ModContext*, void* args, void*, void*) {
    auto fileInfo = mods::arg<dFile_info_c*>(args, 0);

    // Heart tags on file select
    static constexpr std::array kFileSelectHeartTags{
        MULTI_CHAR('hear_20'),
        MULTI_CHAR('hear_21'),
        MULTI_CHAR('hear_22'),
        MULTI_CHAR('hear_23'),
        MULTI_CHAR('hear_24'),
        MULTI_CHAR('hear_25'),
        MULTI_CHAR('hear_26'),
        MULTI_CHAR('hear_27'),
        MULTI_CHAR('hear_28'),
        MULTI_CHAR('hear_29'),
        MULTI_CHAR('hear_30'),
        MULTI_CHAR('hear_31'),
        MULTI_CHAR('hear_32'),
        MULTI_CHAR('hear_33'),
        MULTI_CHAR('hear_34'),
        MULTI_CHAR('hear_35'),
        MULTI_CHAR('hear_36'),
        MULTI_CHAR('hear_37'),
        MULTI_CHAR('hear_38'),
        MULTI_CHAR('hear_39'),
    };

    auto maybeHeartColor = get_config_var_color(get_cvars().heartColor);
    if (maybeHeartColor.has_value()) {
        auto heartColor = maybeHeartColor.value();
        for (auto tag : kFileSelectHeartTags) {
            auto element = static_cast<J2DPicture*>(fileInfo->mFileInfo.Scr->search(tag));
            if (element != nullptr) {
                element->setBlackWhite(heartColor, JUtility::TColor(200, 200, 200, 255));
            }
        }
    }
}

// Buttons that appear contextually at the bottom of the screen during gameplay
// (Are X, Y ever used there?)
DEFINE_HOOK(&dMeterButton_c::screenInitButton, ScreenInitButton);
void screen_init_button_post(ModContext*, void* args, void*, void*) {
    auto meterButton = mods::arg<dMeterButton_c*>(args, 0);
    auto screen = meterButton->mpButtonScreen;

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('a_btn1'), screen);
    recolor_ui_button(get_cvars().bButtonColor, MULTI_CHAR('b_btn'), screen);
    recolor_ui_button(get_cvars().xButtonColor, MULTI_CHAR('x_btn'), screen);
    recolor_ui_button(get_cvars().yButtonColor, MULTI_CHAR('y_btn'), screen);
    recolor_ui_button(get_cvars().zButtonColor, MULTI_CHAR('zbtn'), screen);
}

// A and B buttons when saving a file
DEFINE_HOOK(&dMenu_save_c::screenSet, MenuSaveScreenSet);
void menu_save_screen_set_post(ModContext*, void* args, void*, void*) {
    auto menuSave = mods::arg<dMenu_save_c*>(args, 0);
    auto screen = menuSave->mSaveSel.Scr;

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('wabtn'), screen);
    recolor_ui_button(get_cvars().bButtonColor, MULTI_CHAR('wbbtn'), screen);
}

// A and B buttons when selecting a file
DEFINE_HOOK(&dFile_select_c::screenSet, FileSelectScreenSet);
void file_select_screen_set_post(ModContext*, void* args, void*, void*) {
    auto fileSelect = mods::arg<dFile_select_c*>(args, 0);
    auto screen = fileSelect->fileSel.Scr;

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('wabtn'), screen);
    recolor_ui_button(get_cvars().bButtonColor, MULTI_CHAR('wbbtn'), screen);
}

// A button on brightness check screen
DEFINE_HOOK(&dBrightCheck_c::screenSet, BrightCheckScreenSet);
void bright_check_screen_set_post(ModContext*, void* args, void*, void*) {
    auto brightCheck = mods::arg<dBrightCheck_c*>(args, 0);
    auto screen = brightCheck->mBrightCheck.Scr;

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('a_btn1'), screen);
}

// X and Y buttons on the item wheel screen
DEFINE_HOOK(&dMenu_Ring_c::_create, MenuRingCreate);
void menu_ring_create_post(ModContext*, void* args, void*, void*) {
    auto menuRing = mods::arg<dMenu_Ring_c*>(args, 0);
    auto screen = menuRing->mpScreen;

    recolor_ui_button(get_cvars().xButtonColor, MULTI_CHAR('xbtn'), screen);
    recolor_ui_button(get_cvars().yButtonColor, MULTI_CHAR('ybtn'), screen);
}

// A and B buttons on the main pause screen
DEFINE_HOOK(&dMenu_Collect2D_c::_create, MenuCollect2DCreate);
void menu_collect_2D_create_post(ModContext*, void* args, void*, void*) {
    auto menuCollect2D = mods::arg<dMenu_Collect2D_c*>(args, 0);
    auto screen = menuCollect2D->mpScreenIcon;

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('a_btn'), screen);
    recolor_ui_button(get_cvars().bButtonColor, MULTI_CHAR('b_btn'), screen);
}

// A and B buttons on the fishing journal screen
DEFINE_HOOK(&dMenu_Fishing_c::screenSetDoIcon, MenuFishingScreenSetDoIcon);
void menu_fishing_screen_set_do_icon_post(ModContext*, void* args, void*, void*) {
    auto menuFishing = mods::arg<dMenu_Fishing_c*>(args, 0);
    auto screen = menuFishing->mpIconScreen;

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('a_btn'), screen);
    recolor_ui_button(get_cvars().bButtonColor, MULTI_CHAR('b_btn'), screen);
}

// A and B buttons on the insect collection screen
DEFINE_HOOK(&dMenu_Insect_c::screenSetDoIcon, MenuInsectScreenSetDoIcon);
void menu_insect_screen_set_do_icon_post(ModContext*, void* args, void*, void*) {
    auto menuInsect = mods::arg<dMenu_Insect_c*>(args, 0);
    auto screen = menuInsect->mpIconScreen;

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('a_btn'), screen);
    recolor_ui_button(get_cvars().bButtonColor, MULTI_CHAR('b_btn'), screen);
}

// A and B buttons on the letters screen
DEFINE_HOOK(&dMenu_Letter_c::screenSetDoIcon, MenuLetterScreenSetDoIcon);
void menu_letter_screen_set_do_icon_post(ModContext*, void* args, void*, void*) {
    auto menuLetter = mods::arg<dMenu_Letter_c*>(args, 0);
    auto screen = menuLetter->mpIconScreen;

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('a_btn'), screen);
    recolor_ui_button(get_cvars().bButtonColor, MULTI_CHAR('b_btn'), screen);
}

// A, B, and Z buttons on option screen
DEFINE_HOOK(&dMenu_Option_c::_create, MenuOptionCreate);
void menu_option_create_post(ModContext*, void* args, void*, void*) {
    auto menuOption = mods::arg<dMenu_Option_c*>(args, 0);
    auto screen = menuOption->mpScreenIcon;
    auto backScreen = menuOption->mpBackScreen;
    auto tvScreen = menuOption->mpTVScreen;

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('a_btn'), screen);
    recolor_ui_button(get_cvars().bButtonColor, MULTI_CHAR('b_btn'), screen);
    recolor_ui_button(get_cvars().zButtonColor, MULTI_CHAR('g_zbtn'), backScreen);

    // A Button on option's brightness check screen
    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('a_btn1'), tvScreen);
}

// A and B buttons on the hidden skills screen
DEFINE_HOOK(&dMenu_Skill_c::screenSetDoIcon, MenuSkillScreenSetDoIcon);
void menu_skill_screen_set_do_icon_post(ModContext*, void* args, void*, void*) {
    auto menuSkill = mods::arg<dMenu_Skill_c*>(args, 0);
    auto screen = menuSkill->mpIconScreen;

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('a_btn'), screen);
    recolor_ui_button(get_cvars().bButtonColor, MULTI_CHAR('b_btn'), screen);
}

// A, B, and Z buttons on the field map screen
DEFINE_HOOK(&dMenu_Fmap_c::_create, MenuFMapCreate);
void menu_fmap_create_post(ModContext*, void* args, void*, void*) {
    auto menuFMap = mods::arg<dMenu_Fmap_c*>(args, 0);
    auto screen = menuFMap->mpDraw2DTop->mpTitleScreen;

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('a_btn'), screen);
    recolor_ui_button(get_cvars().bButtonColor, MULTI_CHAR('b_btn1'), screen);
    recolor_ui_button(get_cvars().zButtonColor, MULTI_CHAR('zbtn'), screen);
}

// A and B buttons on the dungeon map screen
DEFINE_HOOK_SYMBOL("dMenu_DmapBg_c::buttonIconScreenInit", void(dMenu_DmapBg_c*), MenuDMapButtonIconScreenInit);
void menu_dmap_button_icon_screen_init_post(ModContext*, void* args, void*, void*) {
    auto menuDMap = mods::arg<dMenu_DmapBg_c*>(args, 0);
    auto screen = menuDMap->mButtonScreen;

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('a_btn'), screen);
    recolor_ui_button(get_cvars().bButtonColor, MULTI_CHAR('b_btn'), screen);
}

// A button on the howling screen
DEFINE_HOOK(&dMsgObject_c::talkStartInit, MsgObjectTalkStartInit);
void msg_object_talk_start_init_post(ModContext*, void* args, void*, void*) {
    auto msgObject = mods::arg<dMsgObject_c*>(args, 0);

    // If textbox kind is howling
    if (msgObject->mFukiKind == 17) {
        auto screen = msgObject->mpScrnDraw->mpScreen;

        recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('abtn'), screen);
    }
}

// A button when on Epona
DEFINE_HOOK(&dMeterHakusha_c::_create, MeterHakushaCreate);
void meter_hakusha_create_post(ModContext*, void* args, void*, void*) {
    auto meterHakusha = mods::arg<dMeterHakusha_c*>(args, 0);
    auto screen = meterHakusha->mpButtonScreen;

    recolor_ui_button(get_cvars().aButtonColor, MULTI_CHAR('a_btn1'), screen);
}

// A, B, X, and Y button icons in text
DEFINE_HOOK(&COutFont_c::createPane, OutFontCreatePane);
void out_font_create_pane_post(ModContext*, void* args, void*, void*) {
    auto outFont = mods::arg<COutFont_c*>(args, 0);
    auto paneArr = outFont->mpPane;

    auto maybeAButtonColor = get_config_var_color(get_cvars().aButtonColor);
    if (maybeAButtonColor.has_value()) {
        auto aButtonColor = maybeAButtonColor.value();
        paneArr[0]->setBlackWhite(JUtility::TColor(255, 255, 255, 0),
            JUtility::TColor(aButtonColor.r, aButtonColor.g, aButtonColor.b, 255));
    }

    auto maybeBButtonColor = get_config_var_color(get_cvars().bButtonColor);
    if (maybeBButtonColor.has_value()) {
        auto bButtonColor = maybeBButtonColor.value();
        paneArr[1]->setBlackWhite(JUtility::TColor(255, 255, 255, 0),
            JUtility::TColor(bButtonColor.r, bButtonColor.g, bButtonColor.b, 255));
    }

    // Condition copied from hooked function
    auto xyBlack = JUtility::TColor(255, 255, 255, 0);
    if (outFont->field_0x242 == 1) {
        xyBlack = JUtility::TColor(0, 0, 0, 0);
    }

    auto maybeXButtonColor = get_config_var_color(get_cvars().xButtonColor);
    if (maybeXButtonColor.has_value()) {
        auto xButtonColor = maybeXButtonColor.value();
        paneArr[5]->setBlackWhite(
            xyBlack, JUtility::TColor(xButtonColor.r, xButtonColor.g, xButtonColor.b, 255));
    }

    auto maybeYButtonColor = get_config_var_color(get_cvars().yButtonColor);
    if (maybeYButtonColor.has_value()) {
        auto yButtonColor = maybeYButtonColor.value();
        paneArr[6]->setBlackWhite(
            xyBlack, JUtility::TColor(yButtonColor.r, yButtonColor.g, yButtonColor.b, 255));
    }
}

// Heart icon in text
DEFINE_HOOK(&COutFontSet_c::drawFont, OutFontSetDrawFont);
void out_font_set_draw_font_post(ModContext*, void* args, void*, void*) {
    auto outFontSet = mods::arg<COutFontSet_c*>(args, 0);

    auto maybeHeartColor = get_config_var_color(get_cvars().heartColor);
    if (maybeHeartColor.has_value()) {
        auto heartColor = maybeHeartColor.value();
        u32 heartColor_u32 = heartColor.r << 24 | heartColor.g << 16 | heartColor.b << 8 | 0xFF;
        if (outFontSet->getType() == 0x1B) {  // Heart icon type
            outFontSet->mColor = heartColor_u32;
        }
    }
}

// Heart Drop/Piece of Heart/Heart Container model color
DEFINE_HOOK(&daItemBase_c::CreateItemHeap, ItemBaseCreateItemHeap);
void item_base_create_item_heap_post(ModContext*, void* args, void*, void*) {
    auto itemBase = mods::arg<daItemBase_c*>(args, 0);
    if (itemBase == NULL) {
        return;
    }
    auto itemNo = itemBase->m_itemNo;
    if (itemNo == dItemNo_HEART_e || itemNo == dItemNo_UTAWA_HEART_e ||
        itemNo == dItemNo_KAKERA_HEART_e)
    {
        auto maybeHeartColor = get_config_var_color(get_cvars().heartColor);
        if (maybeHeartColor.has_value()) {
            auto heartColor = maybeHeartColor.value();
            auto heartColorS10 = GXColorS10(heartColor.r, heartColor.g, heartColor.b, heartColor.a);

            // Edit inner heart material color for Piece of Heart / Heart Container
            if (itemNo == dItemNo_UTAWA_HEART_e || itemNo == dItemNo_KAKERA_HEART_e) {
                *itemBase->mpModel->getModelData()->getMaterialNodePointer(3)->getTevKColor(1) =
                    heartColor;
                *itemBase->mpModel->getModelData()->getMaterialNodePointer(3)->getTevColor(1) =
                    heartColorS10;
            }

            const u8 heartColorRGB[3] = {heartColor.r, heartColor.g, heartColor.b};
            u8** cRegTable =
                reinterpret_cast<u8**>(&itemBase->mpBrkAnm->getBrkAnm()->mAnmCRegDataR);
            u8** kRegTable =
                reinterpret_cast<u8**>(&itemBase->mpBrkAnm->getBrkAnm()->mAnmKRegDataR);

            for (int i = 0; i < 3; i++) {
                u8* cReg = cRegTable[i];
                u8* kReg = kRegTable[i];

                auto curColor = heartColorRGB[i];

                // Set heart drop inner heart color
                cReg[0x3] = curColor;
                cReg[0xB] = curColor;

                // Set heart drop outer heart color
                kReg[0x3] = curColor;
                kReg[0xB] = curColor;

                if (itemNo == dItemNo_KAKERA_HEART_e) {
                    cReg[0x13] = curColor;
                    cReg[0x1B] = curColor;
                    kReg[0x13] = curColor;
                    kReg[0x1B] = curColor;
                }
                if (itemNo == dItemNo_UTAWA_HEART_e) {
                    cReg[0x13] = curColor;
                    kReg[0x13] = curColor;
                    kReg[0x1B] = curColor;
                    kReg[0x23] = curColor;
                    kReg[0x2B] = curColor;
                }
            }
        }
    }
}

// Midna Hair Color
DEFINE_HOOK(&daMidna_c::create, MidnaCreate);
void midna_create_post(ModContext*, void* args, void* retval, void*) {
    auto step = reinterpret_cast<int*>(retval);
    // Don't set colors if midna isn't done loading
    if (*step != cPhs_COMPLEATE_e) {
        return;
    }

    auto midna = mods::arg<daMidna_c*>(args, 0);
    midna->field_0x6e0 = g_currentMidnaHairColors.normalColor;
    if (dKy_darkworld_check()) {
        midna->field_0x6e8 = g_currentMidnaHairColors.normalKColor;
        midna->field_0x6ec = g_currentMidnaHairColors.normalKColor2;
    } else {
        midna->field_0x6e8 = g_currentMidnaHairColors.lNormalKColor;
        midna->field_0x6ec = g_currentMidnaHairColors.lNormalKColor2;
    }
}

// Override Midna Hair Color Part 2
DEFINE_HOOK(&daMidna_c::setBodyPartMatrix, MidnaSetBodyPartMatrix);

static GXColorS10 midnaField0x6e0{};
static GXColor midnaField0x6e8{};
static GXColor midnaField0x6ec{};

// Copy the original values before setBodyPartMatrix runs
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
        // Restore the original values from before setBodyPartMatrix ran (this undoes the chase)
        midna->field_0x6e0 = midnaField0x6e0;
        midna->field_0x6e8 = midnaField0x6e8;
        midna->field_0x6ec = midnaField0x6ec;

        // Statement copied from inside function to determine colors
        bool bigColors =
            midna->checkStateFlg0(daMidna_c::FLG0_UNK_10000000) ||
            midna->mBckHeap[2].getIdx() == daMidna_c::m_anmDataTable[daMidna_c::ANM_HAIR].mResID ||
            midna->mBckHeap[2].getIdx() ==
                daMidna_c::m_anmDataTable[daMidna_c::ANM_S_TAKES].mResID ||
            midna->mBckHeap[2].getIdx() ==
                daMidna_c::m_anmDataTable[daMidna_c::ANM_S_WAITS].mResID ||
            midna->mBckHeap[2].getIdx() ==
                daMidna_c::m_anmDataTable[daMidna_c::ANM_S_PACKAWAY].mResID ||
            midna->mBckHeap[2].getIdx() ==
                daMidna_c::m_anmDataTable[daMidna_c::ANM_GRABST].mResID ||
            midna->checkEndResetStateFlg0(daMidna_c::ERFLG0_UNK_40) ||
            dComIfGp_checkPlayerStatus1(0, 0x800000);

        GXColorS10 color{};
        GXColor kcolor1{};
        GXColor kcolor2{};

        // Set our own colors
        if (bigColors) {
            kcolor1 = g_currentMidnaHairColors.bigKColor;
            if (dKy_darkworld_check()) {
                color = g_currentMidnaHairColors.bigColor;
                kcolor2 = g_currentMidnaHairColors.normalKColor2;
            } else {
                color = g_currentMidnaHairColors.lBigColor;
                kcolor2 = g_currentMidnaHairColors.lBigKColor2;
            }
        } else {
            color = g_currentMidnaHairColors.normalColor;
            if (dKy_darkworld_check()) {
                kcolor1 = g_currentMidnaHairColors.normalKColor;
                kcolor2 = g_currentMidnaHairColors.normalKColor2;
            } else {
                kcolor1 = g_currentMidnaHairColors.lNormalKColor;
                kcolor2 = g_currentMidnaHairColors.lNormalKColor2;
            }
        }

        // Reapply the chase that happens in the function
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

// Midna Charge Ring Color
DEFINE_HOOK(&daAlink_c::setWolfLockDomeModel, SetWolfLockDomeModel);
void wolf_lock_dome_model_post(ModContext*, void*, void*, void*) {
    auto domeRingColorStr = get_str_option(get_cvars().midnaChargeRingColor, "");
    if (is_valid_hex_color_str(domeRingColorStr)) {
        auto domeRingColor = hex_color_str_to_gx_color(domeRingColorStr);
        const u8 domeWave1RGBA[3] = {domeRingColor.r, domeRingColor.g, domeRingColor.b};
        const u8 domeWave2RGBA[3] = {domeRingColor.r, domeRingColor.g, domeRingColor.b};
        u8** chromaRegisterTable =
            reinterpret_cast<u8**>(&daAlink_getAlinkActorClass()->field_0x0724->mAnmCRegDataR);

        for (int i = 0; i < 3; i++) {
            u8* currentTable = chromaRegisterTable[i];
            const u8 currentWave1Color = domeWave1RGBA[i];
            const u8 currentWave2Color = domeWave2RGBA[i];
            const u8 currentBaseColor = (currentWave1Color + currentWave2Color) / 2;

            currentTable[0x3] = currentBaseColor;    // Set Alpha for the ring base
            currentTable[0x13] = currentWave1Color;  // Set Alpha for ring wave 1
            currentTable[0x23] = currentWave2Color;  // Set Alpha for ring wave 2
            currentTable[0xB] = currentBaseColor;    // Set Alpha for darkworld ring base
            currentTable[0x1B] = currentWave1Color;  // Set Alpha for darkworld ring wave 1
            currentTable[0x2B] = currentWave2Color;  // Set Alpha for darkworld ring wave 2
        }
    }
}

#define ADD_POST_HOOK(defined_hook, function, original)                                            \
    result = mods::hook::add_post<defined_hook>(function);                                         \
    if (result != MOD_OK) {                                                                        \
        mods::log::debug(                                                                          \
            "failed to add post hook to" #original ", Result {}", static_cast<int>(result));       \
        return result;                                                                             \
    }

#define ADD_PRE_HOOK(defined_hook, function, original)                                             \
    result = mods::hook::add_pre<defined_hook>(function);                                          \
    if (result != MOD_OK) {                                                                        \
        mods::log::debug(                                                                          \
            "failed to add pre hook to" #original ", Result {}", static_cast<int>(result));        \
        return result;                                                                             \
    }

ModResult add_all_hooks() {
    ModResult result{};

    // Hooks for lantern glow
    ADD_POST_HOOK(WolfEyeLightSet, wolf_eye_light_set_post, dKy_WolfEyeLight_set)
    ADD_POST_HOOK(PreKandelaarDraw, pre_kandelaar_draw_post, daAlink_c::preKandelaarDraw)

    // Hooks for lantern meter color
    ADD_PRE_HOOK(CPaneMgrSetBlackWhite, cpane_mgr_set_black_white_pre, CPaneMgr::setBlackWhite)
    ADD_POST_HOOK(
        KanteraIconSetNowGauge, kantera_icon_set_now_gauge_post, dKantera_icon_c::setNowGauge)

    // Hook for midna charge ring
    ADD_POST_HOOK(SetWolfLockDomeModel, wolf_lock_dome_model_post, daAlink_c::setWolfLockDomeModel)

    // Hook for light sword glow
    ADD_POST_HOOK(SetLightningSwordEffect, set_lightning_sword_effect_post,
        daAlink_c::setLightningSwordEffect)

    // Hooks for Midna Hair Color
    ADD_POST_HOOK(MidnaCreate, midna_create_post, daMidna_c::create)
    ADD_PRE_HOOK(
        MidnaSetBodyPartMatrix, midna_set_body_part_matrix_pre, daMidna_c::setBodyPartMatrix)
    ADD_POST_HOOK(
        MidnaSetBodyPartMatrix, midna_set_body_part_matrix_post, daMidna_c::setBodyPartMatrix)

    // Hooks for UI colors
    ADD_POST_HOOK(dMeter2Init, d_meter_2_init_post, dMeter2Draw_c::init)
    ADD_POST_HOOK(FileInfoSetHeartCount, file_info_set_heart_count_post, dFile_info_c::setHeartCnt)
    ADD_POST_HOOK(ScreenInitButton, screen_init_button_post, dMeterButton_c::screenInitButton)
    ADD_POST_HOOK(MenuSaveScreenSet, menu_save_screen_set_post, dMenu_save_c::screenSet)
    ADD_POST_HOOK(FileSelectScreenSet, file_select_screen_set_post, dFile_select_c::screenSet)
    ADD_POST_HOOK(BrightCheckScreenSet, bright_check_screen_set_post, dBrightCheck_c::screenSet)
    ADD_POST_HOOK(MenuRingCreate, menu_ring_create_post, dMenu_Ring_c::_create)
    ADD_POST_HOOK(MenuCollect2DCreate, menu_collect_2D_create_post, dMenu_Collect2D_c::_create)
    ADD_POST_HOOK(MenuFishingScreenSetDoIcon, menu_fishing_screen_set_do_icon_post,
        dMenu_Fishing_c::screenSetDoIcon)
    ADD_POST_HOOK(MenuInsectScreenSetDoIcon, menu_insect_screen_set_do_icon_post,
        dMenu_Insect_c::screenSetDoIcon)
    ADD_POST_HOOK(MenuLetterScreenSetDoIcon, menu_letter_screen_set_do_icon_post,
        dMenu_Letter_c::screenSetDoIcon)
    ADD_POST_HOOK(MenuOptionCreate, menu_option_create_post, dMenu_Option_c::_create)
    ADD_POST_HOOK(MenuSkillScreenSetDoIcon, menu_skill_screen_set_do_icon_post,
        dMenu_Skill_c::screenSetDoIcon)
    ADD_POST_HOOK(OutFontCreatePane, out_font_create_pane_post, COutFont_c::createPane)
    ADD_POST_HOOK(OutFontSetDrawFont, out_font_set_draw_font_post, COutFontSet_c::drawFont)
    ADD_POST_HOOK(MenuFMapCreate, menu_fmap_create_post, dMenu_Fmap_c::_create)
    ADD_POST_HOOK(MenuDMapButtonIconScreenInit, menu_dmap_button_icon_screen_init_post,
        dMenu_DmapBg_c::buttonIconScreenInit)
    ADD_POST_HOOK(
        MsgObjectTalkStartInit, msg_object_talk_start_init_post, dMsgObject_c::talkStartInit)
    ADD_POST_HOOK(MeterHakushaCreate, meter_hakusha_create_post, dMeterHakusha_c::_create)

    // Heart Model Color
    ADD_POST_HOOK(
        ItemBaseCreateItemHeap, item_base_create_item_heap_post, daItemBase_c::CreateItemHeap)

    return MOD_OK;
}

#define UNINSTALL_HOOK(defined_hook) mods::hook::uninstall<defined_hook>(svc_hook);

ModResult remove_all_hooks() {
    UNINSTALL_HOOK(WolfEyeLightSet)
    UNINSTALL_HOOK(PreKandelaarDraw)
    UNINSTALL_HOOK(CPaneMgrSetBlackWhite)
    UNINSTALL_HOOK(KanteraIconSetNowGauge)
    UNINSTALL_HOOK(SetWolfLockDomeModel)
    UNINSTALL_HOOK(SetLightningSwordEffect)
    UNINSTALL_HOOK(MidnaCreate)
    UNINSTALL_HOOK(MidnaSetBodyPartMatrix)
    UNINSTALL_HOOK(MidnaSetBodyPartMatrix)
    UNINSTALL_HOOK(dMeter2Init)
    UNINSTALL_HOOK(FileInfoSetHeartCount)
    UNINSTALL_HOOK(ScreenInitButton)
    UNINSTALL_HOOK(MenuSaveScreenSet)
    UNINSTALL_HOOK(FileSelectScreenSet)
    UNINSTALL_HOOK(BrightCheckScreenSet)
    UNINSTALL_HOOK(MenuRingCreate)
    UNINSTALL_HOOK(MenuCollect2DCreate)
    UNINSTALL_HOOK(MenuFishingScreenSetDoIcon)
    UNINSTALL_HOOK(MenuInsectScreenSetDoIcon)
    UNINSTALL_HOOK(MenuLetterScreenSetDoIcon)
    UNINSTALL_HOOK(MenuOptionCreate)
    UNINSTALL_HOOK(MenuSkillScreenSetDoIcon)
    UNINSTALL_HOOK(OutFontCreatePane)
    UNINSTALL_HOOK(OutFontSetDrawFont)
    UNINSTALL_HOOK(MenuFMapCreate)
    UNINSTALL_HOOK(MenuDMapButtonIconScreenInit)
    UNINSTALL_HOOK(MsgObjectTalkStartInit)
    UNINSTALL_HOOK(MeterHakushaCreate)
    UNINSTALL_HOOK(ItemBaseCreateItemHeap)
    return MOD_OK;
}
