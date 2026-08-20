#include "hooks.hpp"
#include "session.hpp"
#include "randomizer_context.hpp"
#include "ui/rando_config.hpp"
#include "flags.h"
#include "stages.h"
#include "tools.h"
#include "item.hpp"
#include "item_ids.h"
#include "verify_item_functions.h"

#include <mods/svc/hook.hpp>
#include <mods/svc/log.hpp>

#include "c/c_damagereaction.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_b_bq.h"
#include "d/actor/d_a_demo_item.h"
#include "d/actor/d_a_door_shutter.h"
#include "d/actor/d_a_e_md.h"
#include "d/actor/d_a_e_mk.h"
#include "d/actor/d_a_kytag08.h"
#include "d/actor/d_a_mg_rod.h"
#include "d/actor/d_a_npc4.h"
#include "d/actor/d_a_npc_bans.h"
#include "d/actor/d_a_npc_fairy.h"
#include "d/actor/d_a_npc_shad.h"
#include "d/actor/d_a_npc_yelia.h"
#include "d/actor/d_a_npc_ykm.h"
#include "d/actor/d_a_npc_ykw.h"
#include "d/actor/d_a_npc_zrc.h"
#include "d/actor/d_a_npc_zrz.h"
#include "d/actor/d_a_obj_bosswarp.h"
#include "d/actor/d_a_obj_master_sword.h"
#include "d/actor/d_a_obj_swBallC.h"
#include "d/actor/d_a_obj_zra_rock.h"
#include "d/actor/d_a_tag_kmsg.h"
#include "d/actor/d_a_tbox2.h"
#include "d/actor/d_a_obj_item.h"
#include "d/actor/d_a_obj_life_container.h"
#include "d/d_door_param2.h"
#include "d/d_event.h"
#include "d/d_file_sel_info.h"
#include "d/d_file_select.h"
#include "d/d_gameover.h"
#include "d/d_menu_item_explain.h"
#include "d/d_menu_ring.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "d/d_s_name.h"
#include "d/d_s_play.h"
#include "d/d_save.h"
#include "d/d_shop_system.h"
#include "d/d_item.h"
#include "f_op/f_op_overlap_mng.h"
#include "m_Do/m_Do_Reset.h"

DEFINE_HOOK(&dFile_select_c::selectDataNameMove, dFile_select_c__selectDataNameMove);
DEFINE_HOOK(&dFile_select_c::dataSelect, dFile_select_c__dataSelect);

DEFINE_HOOK(&dFile_info_c::setSaveData, dFile_info_c__setSaveData);

DEFINE_HOOK(&dSv_event_c::isEventBit, dSv_event_c__isEventBit);
DEFINE_HOOK(&dSv_event_c::onEventBit, dSv_event_c__onEventBit);

DEFINE_HOOK(&dSv_memBit_c::isSwitch, dSv_memBit_c__isSwitch);
DEFINE_HOOK(&dSv_memBit_c::onSwitch, dSv_memBit_c__onSwitch);
DEFINE_HOOK(&dSv_memBit_c::onDungeonItem, dSv_memBit_c__onDungeonItem);
DEFINE_HOOK(&dSv_memBit_c::offDungeonItem, dSv_memBit_c__offDungeonItem);
DEFINE_HOOK(&dSv_memBit_c::isDungeonItem, dSv_memBit_c__isDungeonItem);

DEFINE_HOOK(&dSv_player_status_b_c::isDarkClearLV, dSv_player_status_b_c__isDarkClearLV);

DEFINE_HOOK(&dSv_player_item_c::checkEmptyBottle, dSv_player_item_c__checkEmptyBottle);
DEFINE_HOOK(&dSv_player_item_c::setLineUpItem, dSv_player_item_c__setLineUpItem);

DEFINE_HOOK(&dSv_info_c::onSwitch, dSv_info_c__onSwitch);

#ifdef _MSVC_LANG
#define setNextStage_sig "?dComIfGp_setNextStage@@YAXPEBDFCCMIHCFHH@Z"
#else
#define setNextStage_sig "_Z21dComIfGp_setNextStagePKcsaafjiasii"
#endif
DEFINE_HOOK_SYMBOL(setNextStage_sig, void(char const *, s16, s8, s8, f32, u32, int, s8, s16, int, int), setNextStage);

DEFINE_HOOK_SYMBOL("daObj_Gb_Create", int(fopAc_ac_c*), ObjGb_Create);

DEFINE_HOOK(&dMeter2Info_readItemTexture, readItemTexture);

DEFINE_HOOK(&dShopSystem_c::seq_decide_yes, dShopSystem_c__seq_decide_yes);

DEFINE_HOOK(&CheckFieldItemCreateHeap, dItemData_CheckFieldItemCreateHeap);

DEFINE_HOOK(&dEvt_control_c::talkEnd, dEvt_control_c__talkEnd);

DEFINE_HOOK(&dComIfG_play_c::getLayerNo_common_common, dComIfG_play_c__getLayerNo_common_common);
DEFINE_HOOK(&dComIfGs_onStageSwitch, onStageSwitch);

extern void getItemFunc(u8);
DEFINE_HOOK(&getItemFunc, dItem_getItemFunc);

extern int checkItemGet(u8 i_itemNo, int i_default);
DEFINE_HOOK(&checkItemGet, dItem_checkItemGet);

DEFINE_HOOK(&daAlink_c::decideDoStatus, daAlink_c__decideDoStatus);
DEFINE_HOOK_SYMBOL("daAlink_searchBouDoor", void*(fopAc_ac_c*, void*), searchBouDoor);
DEFINE_HOOK(&daAlink_c::checkGroundSpecialMode, daAlink_c__checkGroundSpecialMode);
DEFINE_HOOK(&daAlink_c::setGetItemFace, daAlink_c__setGetItemFace);
DEFINE_HOOK(&daAlink_c::setGetSubBgm, daAlink_c__setGetSubBgm);
DEFINE_HOOK(&daAlink_c::procCoGetItem, daAlink_c__procCoGetItem);
DEFINE_HOOK(&daAlink_c::procCoWarpInit, daAlink_c__procCoWarpInit);

DEFINE_HOOK_SYMBOL("b_bq_end", void(b_bq_class*), bq_end);

DEFINE_HOOK(&daDoor20_c::checkOpenMsgDoor, daDoor20_c__checkOpenMsgDoor);

DEFINE_HOOK_SYMBOL("demo_camera_end", void(e_mk_class*), e_mk_demo_camera_end);

DEFINE_HOOK(&dStage_changeScene4Event, changeScene4Event);
DEFINE_HOOK_SYMBOL("dStage_playerInit", int(dStage_dt_c*, void*, int, void*), stage_playerInit);

DEFINE_HOOK_SYMBOL("daKytag08_Execute", int(kytag08_class*), Kytag08_Execute);

DEFINE_HOOK(&daNpcT_chkEvtBit, NpcT_chkEvtBit);
DEFINE_HOOK(&daNpcF_chkEvtBit, NpcF_chkEvtBit);
DEFINE_HOOK(&daNpcF_c::orderEvent, daNpcF_c__orderEvent);

DEFINE_HOOK(&daNpc_Bans_c::isDelete, daNpc_Bans_c__isDelete);

DEFINE_HOOK(&daNpc_Fairy_c::AppearDemoCall, daNpc_Fairy_c__AppearDemoCall);

DEFINE_HOOK(&daNpcShad_c::Create, daNpcShad_c__Create);
DEFINE_HOOK(&daNpcShad_c::wait_type1, daNpcShad_c__wait_type1);

DEFINE_HOOK(&daNpc_Yelia_c::cutTakeWoodStatue, daNpc_Yelia_c__cutTakeWoodStatue);
DEFINE_HOOK(&dSv_player_item_c::setWarashibeItem, dSv_player_item_c__setWarashibeItem);

DEFINE_HOOK(&daNpc_ykM_c::isDelete, daNpc_ykM_c__isDelete);
DEFINE_HOOK(&daNpc_ykW_c::isDelete, daNpc_ykW_c__isDelete);

DEFINE_HOOK_SYMBOL("daE_MD_Create", int(fopAc_ac_c*), daE_MD_c__create);

DEFINE_HOOK(&fopAcM_orderMapToolEvent, orderMapToolEvent);
DEFINE_HOOK_SYMBOL("daObjMasterSword_Execute", int(daObjMasterSword_c*), daObjMasterSword_c__execute);

DEFINE_HOOK(&dScnName_c::changeGameScene, dScnName_c__changeGameScene);

DEFINE_HOOK(&daNpc_zrZ_c::isDelete, daNpc_zrZ_c__isDelete);

DEFINE_HOOK(&daTbox2_c::Create, daTbox2_c__Create);
DEFINE_HOOK(&daTbox2_c::setGetDemoItem, daTbox2_c__setGetDemoItem);

DEFINE_HOOK(&dGameover_c::_create, dGameover_c___create);

DEFINE_HOOK(&daObjSwBallC_c::Create, daObjSwBallC_c__Create);
DEFINE_HOOK(&daObjSwBallC_c::actionWait, daObjSwBallC_c__actionWait);

DEFINE_HOOK_SYMBOL("daDitem_Execute", int(daDitem_c*), daDitem_c__execute);

DEFINE_HOOK(&daObjBossWarp_c::demoProc, daObjBossWarp_c__demoProc);

DEFINE_HOOK_SYMBOL("lure_heart", void(dmg_rod_class*), mgRod_lure_heart);
DEFINE_HOOK_SYMBOL("uki_catch", void(dmg_rod_class*), mgRod_uki_catch);

#ifdef _MSVC_LANG
#define setEmptyBottle_noarg_sig "?setEmptyBottle@dSv_player_item_c@@QEAAXXZ"
#else
#define setEmptyBottle_noarg_sig "_ZN17dSv_player_item_c14setEmptyBottleEv"
#endif
DEFINE_HOOK_SYMBOL(setEmptyBottle_noarg_sig, void(dSv_player_item_c*), dSv_player_item_c__setEmptyBottle);

DEFINE_HOOK(&daNpc_zrC_c::isDelete, daNpc_zrC_c__isDelete);

DEFINE_HOOK(&daObjZraRock_c::create, daObjZraRock_c__create);

#ifdef _MSVC_LANG
#define phase_1_dScnPly_sig "?phase_1@@YAHPAUdScnPly_c@@@Z" // sig is wrong?
#else
#define phase_1_dScnPly_sig "_Z7phase_1P9dScnPly_c"
#endif
// DEFINE_HOOK_SYMBOL(phase_1_dScnPly_sig, int(dScnPly_c*), phase_1__dScnPly_c);

DEFINE_HOOK(&dMenu_Ring_c::textScaleHIO, dMenu_Ring_c__textScaleHIO);

#ifdef _MSVC_LANG
#define dMenu_Ring_c__destructor_sig "??1dMenu_Ring_c@@UEAA@XZ"
#else
#define dMenu_Ring_c__destructor_sig "_ZN12dMenu_Ring_cD1Ev"
#endif
DEFINE_HOOK_SYMBOL(dMenu_Ring_c__destructor_sig, void(dMenu_Ring_c*), dMenu_Ring_c__destructor);

DEFINE_HOOK(&dMenu_Ring_c::setActiveCursor, dMenu_Ring_c__setActiveCursor);
DEFINE_HOOK(&dMenu_Ring_c::getItemMaxNum, dMenu_Ring_c__getItemMaxNum);
DEFINE_HOOK(&dMenu_Ring_c::getItemNum, dMenu_Ring_c__getItemNum);

DEFINE_HOOK(&daItem_c::CreateInit, daItem_c__CreateInit);
DEFINE_HOOK(&daItem_c::itemActionForBoomerang, daItem_c__itemActionForBoomerang);
DEFINE_HOOK(&daItem_c::itemGetNextExecute, daItem_c__itemGetNextExecute);
DEFINE_HOOK(&daItem_c::itemGet, daItem_c__itemGet);

DEFINE_HOOK(&daObjLife_c::setEffect, daObjLife_c__setEffect);
DEFINE_HOOK(&daObjLife_c::create, daObjLife_c__create);
DEFINE_HOOK(&daObjLife_c::actionGetDemo, daObjLife_c__actionGetDemo);
DEFINE_HOOK(&daObjLife_c::calcScale, daObjLife_c__calcScale);

DEFINE_HOOK_SYMBOL("dComIfGs_getCollectSmell", u8(), getCollectSmell);

DEFINE_HOOK(&dEvt_control_c::skipper, dEvt_control_c__skipper);

namespace randomizer::ui {
dialogSelectModeState g_dialogSelectModeState = SelectReady;
}

namespace randomizer::hooks {
namespace {
HookAction hookPreDataSelect(ModContext*, void* args, void* retval, void* userdata) {
    ui::g_dialogSelectModeState = ui::SelectReady;
    ui::g_file_select_window_ctx.is_proceed = false;
    return HOOK_CONTINUE;
}

HookAction hookPreSelectDataNameMove(ModContext*, void* args, void* retval, void* userdata) {
    dFile_select_c* i_this = mods::arg<dFile_select_c*>(args, 0);

    // if coming from "start randomizer" button, let transition occur as normal
    if (ui::g_file_select_window_ctx.is_proceed) {
        return HOOK_CONTINUE;
    }

    bool isHeaderTxtChange = i_this->headerTxtChangeAnm();
    bool isFileRecScale = i_this->fileRecScaleAnm2();
    bool isModoruTxtDisp = i_this->modoruTxtDispAnm();

    if (ui::g_dialogSelectModeState == ui::SelectReady && isHeaderTxtChange == true && isFileRecScale == true && isModoruTxtDisp == true) {
        ui::g_dialogSelectModeState = ui::SelectWait;

        ModResult rt = ui::buildFileSelectGateMenu(i_this);
        if (rt != MOD_OK) {
            mods::log::error("Failed to build menu");
            return HOOK_CONTINUE;
        }
    }

    return HOOK_SKIP_ORIGINAL;
}

void hookPostSetSaveData(ModContext* ctx, void* args, void* retval, void* userdata) {
    dFile_info_c* i_this = mods::arg<dFile_info_c*>(args, 0);
    u8 i_dataNo = mods::arg<u8>(args, 3);

    if (*static_cast<int*>(retval) == 0) {
        char hash[64];
        size_t size = sizeof(hash) - 1;

        ModResult rt = session::svc_mng.save->peek_blob(ctx, i_dataNo, "seed_hash", hash, &size);
        if (rt != MOD_OK || size == 0) {
            // leave file text vanilla if seed hash isn't found
            mods::log::debug("no seed_hash found for file {}", i_dataNo);
            return;
        }

        hash[size] = 0;
        const std::string curFileSeedHash = hash;
        if (!curFileSeedHash.empty()) {
            const auto setHBinding = [](J2DTextBox* tbox, J2DTextBoxHBinding bind) {
                tbox->mFlags &= 0b0011;
                tbox->mFlags |= ((bind & 3) << 2);
            };

            // Overwrite "Save time" text with "Randomizer"
            auto saveTimeText = (J2DTextBox*)i_this->mFileInfo.Scr->search(MULTI_CHAR('f_s_t_02'));
            SafeStringCopy(saveTimeText->getStringPtr(), "Randomizer");
            setHBinding(saveTimeText, J2DTextBoxHBinding::HBIND_LEFT);

            // Overwrite the "Total play time" text with the seed hash
            auto playTimeText = (J2DTextBox*)i_this->mFileInfo.Scr->search(MULTI_CHAR('f_p_t_02'));
            SafeStringCopy(playTimeText->getStringPtr(), curFileSeedHash.c_str());

            // Give the text double the space on the menu incase the seed hash is long
            setHBinding(playTimeText, J2DTextBoxHBinding::HBIND_LEFT);
            playTimeText->resize(playTimeText->getWidth() * 2, playTimeText->getHeight());
        }
    }
}

HookAction hookPreIsEventBit(ModContext*, void* args, void* retval, void*) {
    const u16 i_no = mods::arg<u16>(args, 1);
    auto& out = *static_cast<BOOL*>(retval);

    switch (i_no) {
    case BO_TALKED_TO_YOU_AFTER_OPENING_IRON_BOOTS_CHEST: {
        if (daAlink_c::checkStageName(allStages[Ordon_Village_Interiors])) {
            out = dComIfGs_isEventBit(HEARD_BO_TEXT_AFTER_SUMO_FIGHT) ? TRUE : FALSE;
            return HOOK_SKIP_ORIGINAL;
        }
        break;
    }
    case GAVE_ILIA_HER_CHARM:    // Gave Ilia the charm
    case CITY_OOCCOO_CS_WATCHED: // CiTS Intro CS watched
    {
        if (daAlink_c::checkStageName(allStages[Hidden_Village])) {
            if (!dComIfGs_isEventBit(GOT_ILIAS_CHARM)) {
                // If we haven't gotten the item from Impaz then we need to return false or it
                // will break her dialogue.
                out = FALSE;
                return HOOK_SKIP_ORIGINAL;
            }
        }
        break;
    }
    case GORON_MINES_CLEARED: {
        if (daAlink_c::checkStageName(allStages[Goron_Mines]) ||
            daAlink_c::checkStageName(allStages[Death_Mountain_Interiors])) {
            out = FALSE; // The gorons will not act properly if the flag is set.
            return HOOK_SKIP_ORIGINAL;
        }
        break;
    }
    case ZORA_ESCORT_CLEARED: {
        if (daAlink_c::checkStageName(allStages[Castle_Town])) {
            // If the flag isn't set the player will be thrown into escort when they open the door
            out = TRUE;
            return HOOK_SKIP_ORIGINAL;
        }
        if (playerIsInRoomStage(0, allStages[Kakariko_Village_Interiors])) {
            out = TRUE; // Return true to prevent Renado/Ilia crash after ToT
            return HOOK_SKIP_ORIGINAL;
        }
        break;
    }
    case CITY_IN_THE_SKY_CLEARED: // Would like to find where this is checked and patch it there.
    {
        if (!dComIfGs_isEventBit(FIXED_THE_MIRROR_OF_TWILIGHT)) {
            if (randomizer_GetContext().mSettings[RandomizerContext::PALACE_OF_TWILIGHT_REQUIREMENTS] !=
                RandomizerContext::VANILLA) {
                out = FALSE;
                return HOOK_SKIP_ORIGINAL;
            }
        }
        break;
    }
    case HOWLED_AT_SNOWPEAK_STONE: {
        if (daAlink_c::checkStageName(allStages[Snowpeak])) {
            // return false so the player can howl at the stone multiple times to remove map glitch
            out = FALSE;
            return HOOK_SKIP_ORIGINAL;
        }
        break;
    }
    case WATCHED_CUTSCENE_AFTER_GOATS_2: {
        if (playerIsInRoomStage(1, allStages[Ordon_Village_Interiors])) {
            // false -> Sera gives the milk item once they help the cat;
            // true -> the shop is always usable even if the cat is not returned.
            out = dComIfGs_isEventBit(SERAS_CAT_RETURNED_TO_SHOP) ? FALSE : TRUE;
            return HOOK_SKIP_ORIGINAL;
        }
        break;
    }
    case FIXED_THE_MIRROR_OF_TWILIGHT: {
        if (daAlink_c::checkStageName(allStages[Palace_of_Twilight])) {
            out = TRUE; // If the flag is not set, the player cannot leave PoT from the inside.
            return HOOK_SKIP_ORIGINAL;
        }
        break;
    }
    default:
        break;
    }

    return HOOK_CONTINUE;
}

HookAction hookPreOnEventBit(ModContext*, void* args, void*, void*) {
    const u16 i_no = mods:: arg<u16>(args, 1);

    switch (i_no) {
    // Wolf <-> Human crash patches/bug fixes: some cutscenes/events either crash or act
    // weird if Link is in the wrong form and the game no longer auto-transforms once the
    // Shadow Crystal has been obtained.
    case ENTERED_ORDON_SPRING_DAY_3:
        if (dComIfGs_isEventBit(TRANSFORMING_UNLOCKED)) {
            dComIfGs_setTransformStatus(0);
        }
        break;

    case WATCHED_CUTSCENE_AFTER_BEING_CAPTURED_IN_FARON_TWILIGHT:
        if (dComIfGs_isEventBit(TRANSFORMING_UNLOCKED)) {
            dComIfGs_setTransformStatus(1);
        }
        break;

    case MIDNAS_DESPERATE_HOUR_COMPLETED:
        dComIfGs_onDarkClearLV(3);
        break;

    case CLEARED_FARON_TWILIGHT:
        // If we've already cleared Eldin Twilight, Lanayru Twilight, and MDH
        if (dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_COMPLETED)) {
            if (dComIfGs_isDarkClearLV(2) && dComIfGs_isDarkClearLV(3)) {
                // Set the flag for the last transformed twilight; also puts Midna on the
                // player's back
                dComIfGs_onTransformLV(3);
                dComIfGs_onDarkClearLV(3);
            }
        }
        break;

    case CLEARED_ELDIN_TWILIGHT:
        dComIfGs_onEventBit(MAP_WARPING_UNLOCKED); // in glitched logic, you can skip the gorge bridge
        if (dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_COMPLETED)) {
            if (dComIfGs_isDarkClearLV(1) && dComIfGs_isDarkClearLV(3)) {
                dComIfGs_onTransformLV(3);
                dComIfGs_onDarkClearLV(3);
            }
        }
        // Set flag for the bridge between Castle Town and Eldin field if skip bridge
        // donation is on and both Eldin and Lanayru twilight are cleared
        if (dComIfGs_isEventBit(CLEARED_LANAYRU_TWILIGHT) &&
            randomizer_GetContext().mSettings[RandomizerContext::SKIP_BRIDGE_DONATION] ==
                RandomizerContext::ON)
        {
            dComIfGs_onEventBit(BRIDGE_REPAIR_FUNDRAISING_COMPLETED);
            dComIfGs_onStageSwitch(6, 0x1B); // Bridge exists
        }
        break;

    case CLEARED_LANAYRU_TWILIGHT:
        if (dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_COMPLETED)) {
            if (dComIfGs_isDarkClearLV(1) && dComIfGs_isDarkClearLV(2)) {
                dComIfGs_onTransformLV(3);
                dComIfGs_onDarkClearLV(3);
            }
        }
        if (dComIfGs_isEventBit(CLEARED_ELDIN_TWILIGHT) &&
            randomizer_GetContext().mSettings[RandomizerContext::SKIP_BRIDGE_DONATION] ==
                RandomizerContext::ON)
        {
            dComIfGs_onEventBit(BRIDGE_REPAIR_FUNDRAISING_COMPLETED);
            dComIfGs_onStageSwitch(6, 0x1B); // Bridge exists
        }
        break;

    case REMOVE_SWORD_SHIELD_FROM_WOLF_BACK:
        if (!dComIfGs_isEventBit(CLEARED_FARON_TWILIGHT)) {
            dComIfGs_onTransformLV(0); // Set the last transformed twilight to include Faron
        }
        break;

    case GAVE_TELMA_RENADOS_LETTER:
        offWarashibeItem(dItemNo_Randomizer_LETTER_e);
        break;

    default:
        break;
    }
    return HOOK_CONTINUE;
}

HookAction hookPreMembitIsSwitch(ModContext*, void* args, void* retval, void*) {
    if (getStageID() == Hidden_Village_Interiors) {
        if (mods::arg<int>(args, 1) == 0x61) { // Is Impaz in her house
            *static_cast<BOOL*>(retval) = TRUE;
            return HOOK_SKIP_ORIGINAL;
        }
    }
    return HOOK_CONTINUE;
}

// kinda hacky check to see if this membit object is the temp memory area in save info
inline bool isTempMemBit(dSv_memBit_c* i_this) {
    return i_this == &dComIfGs_getSaveInfo()->getMemory().getBit();
}

HookAction hookPreMembitOnSwitch(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<dSv_memBit_c*>(args, 0);
    const int i_no = mods::arg<int>(args, 1);

    if (isTempMemBit(i_this)) {
        if (getStageID() == Arbiters_Grounds) {
            // Poe flame CS trigger
            if (i_no == 0x26) {
                i_this->offSwitch(0x45); // Open the Poe gate
                return HOOK_SKIP_ORIGINAL;
            }
        } else if (getStageID() == Lake_Hylia) {
            // Lanayru Twilight End CS trigger
            if (i_no == 0xD) {
                if (dComIfGs_isEventBit(TRANSFORMING_UNLOCKED)) {
                    // Set player to Human as the game will not do so if Shadow Crystal has
                    // been obtained.
                    dComIfGs_setTransformStatus(0);
                }
            }
        } else if (getStageID() == Kakariko_Village) {
            // Hawkeye is for sale
            if (i_no == 0x3E) {
                i_this->offSwitch(0xB); // Remove the coming soon sign so the hawkeye can be bought
            }
        } else if (getStageID() == Hyrule_Field) {
            // Destroyed North Eldin rocks barrier
            if (i_no == 0x11) {
                // Unlock Eldin Province on the map. Done manually rather than via
                // `onRegionBit`, which would see the rocks unbroken and skip the region.
                dComIfGs_getSaveData()->getPlayer().getPlayerFieldLastStayInfo().mRegion |= 0x08;
            }
        }
    }
    return HOOK_CONTINUE;
}

HookAction hookPreOnDungeonItem(ModContext*, void* args, void*, void*) {
    int i_no = mods::arg<int>(args, 1);

    // Don't use the stage life collection flag for rando
    if (i_no == dSv_memBit_c::STAGE_LIFE) {
        return HOOK_SKIP_ORIGINAL;
    }
    // Don't turn Ooccoo into the note when defeating a boss
    else if (dComIfGs_isStageBossEnemy() && i_no == dSv_memBit_c::OOCCOO_NOTE) {
        return HOOK_SKIP_ORIGINAL;
    }
    return HOOK_CONTINUE;
}

HookAction hookPreOffDungeonItem(ModContext*, void* args, void*, void*) {
    if (mods::arg<int>(args, 1) == dSv_memBit_c::STAGE_LIFE) {
        return HOOK_SKIP_ORIGINAL;
    }
    return HOOK_CONTINUE;
}

HookAction hookPreIsDungeonItem(ModContext*, void* args, void* retval, void*) {
    const int i_no = mods::arg<int>(args, 1);
    auto& out = *static_cast<BOOL*>(retval);

    switch (i_no) {
    case dSv_memBit_c::STAGE_LIFE:
        out = FALSE;
        return HOOK_SKIP_ORIGINAL;
    case dSv_memBit_c::STAGE_BOSS_ENEMY: {
        // If we are in a dungeon or fighting a midboss, we don't want the boss being
        // defeated to affect the gameplay.
        std::string stageName = dComIfGp_getStartStageName();
        if (stageName.starts_with("D_MN")) {
            out = FALSE;
            return HOOK_SKIP_ORIGINAL;
        }
        break;
    }
    case dSv_memBit_c::STAGE_BOSS_ENEMY_2: {
        // If we are in the early rooms of FT, we don't want Ook being defeated to affect
        // gameplay
        if (daAlink_c::checkStageName("D_MN05") && dComIfGp_roomControl_getStayNo() < 4) {
            out = FALSE;
            return HOOK_SKIP_ORIGINAL;
        }
        break;
    }
    default:
        break;
    }
    return HOOK_CONTINUE;
}

HookAction hookPreIsDarkClearLV(ModContext*, void* args, void* retval, void*) {
    if (mods::arg<int>(args, 1) == 0 &&
        playerIsInRoomStage(1, allStages[Ordon_Village_Interiors]))
    {
        // Return false so Sera will give us the bottle if we have rescued the cat.
        *static_cast<BOOL*>(retval) = FALSE;
        return HOOK_SKIP_ORIGINAL;
    }
    return HOOK_CONTINUE;
}

HookAction hookPreCheckEmptyBottle(ModContext*, void*, void* retval, void*) {
    if (getStageID() == Cave_of_Ordeals) {
        // Return 1 to allow the player to collect the floor 50 reward, as this makes the
        // game think the player has an empty bottle.
        *static_cast<u8*>(retval) = 1;
        return HOOK_SKIP_ORIGINAL;
    }
    return HOOK_CONTINUE;
}

void hookPostSetLineUpItem(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<dSv_player_item_c*>(args, 0);
    if (i_this->mItems[7] == dItemNo_NONE_e) {
        return;
    }

    // append slot 7 after the vanilla lineup, unless already present
    int slot_idx = 0;
    for (; slot_idx < 24; slot_idx++) {
        const u8 lineup = i_this->mItemSlots[slot_idx];
        if (lineup == 7) {
            return;
        }
        if (lineup == 0xFF) {
            break;
        }
    }

    if (slot_idx < 24) {
        i_this->mItemSlots[slot_idx] = 7;
    }
}

HookAction hookPreSaveInfoOnSwitch(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<dSv_info_c*>(args, 0);
    const int i_no = mods::arg<int>(args, 1);
    const int room_no = mods::arg<int>(args, 2);

    // Set custom flag for the Temple of Time pedestal strike
    if (getStageID() == Sacred_Grove && i_no == 0xEE) {
        i_this->onSwitch(0x63, room_no);
    }

    // We check to see if the flag being set is for the UZR portal as a safety precaution.
    if (daAlink_c::checkStageName("F_SP126") && i_no == 0x15 &&
        dComIfGs_getTransformStatus() == TF_STATUS_WOLF)
    {
        // Set the flag to make Iza 1 available and set the memory bit for having talked to her
        // after opening the portal as human.
        dComIfGs_onEventBit(0xB02);
        i_this->onSwitch(0x37, room_no);

        // Note for the above stuff. This works for now. Eventually would like to adjust this to
        // a FLW patch since I think we could accomplish similar results by having the conversation
        // continue as normal regardless of form, but I haven't looked into it that much.
    }
    return HOOK_CONTINUE;
}

bool hookLureHeart_isSkipGetItem = false;
HookAction hookPreLureHeart(ModContext*, void* args, void*, void*) {
    hookLureHeart_isSkipGetItem = true;
    return HOOK_CONTINUE;
}

void hookPostLureHeart(ModContext*, void*, void*, void*) {
    hookLureHeart_isSkipGetItem = false;
}

HookAction hookPreGetItemFunc(ModContext*, void* args, void*, void*) {
    const u8 item = mods::arg<u8>(args, 0);

    // coming from lure_heart, don't give item here. let FLW message handle it
    if (hookLureHeart_isSkipGetItem && item == dItemNo_KAKERA_HEART_e) {
        return HOOK_SKIP_ORIGINAL;
    }

    item::exec_item_get(item);
    return HOOK_SKIP_ORIGINAL;
}

 HookAction hookPreCheckItemGet(ModContext*, void* args, void* retval, void*) {
    *static_cast<int*>(retval) = item::check_item_get(mods::arg<u8>(args, 0), mods::arg<int>(args, 1));
    return HOOK_SKIP_ORIGINAL;
}

HookAction hookPreSetNextStage(ModContext*, void* args, void*, void*) {
    randomizer_checkAndOverrideEntranceData(
        mods::arg_ref<char const*>(args, 0),
        mods::arg_ref<s8>(args, 2),
        mods::arg_ref<s16>(args, 1),
        mods::arg_ref<s8>(args, 3),
        mods::arg_ref<u32>(args, 5)
    );
    return HOOK_CONTINUE;
}

HookAction hookPreObjGbCreate(ModContext*, void* args, void* retval, void*) {
    if (getStageID() == StageIDs::Mirror_Chamber && !randomizer_mirrorChamberWallShouldExist()) {
        *static_cast<int*>(retval) = cPhs_ERROR_e;
        return HOOK_SKIP_ORIGINAL;
    }
    return HOOK_CONTINUE;
}

void hookPostReadItemTexture(ModContext*, void* args, void*, void*) {
    const u8 item_no = mods::arg<u8>(args, 1);
    void* tex_buf1 = mods::arg<void*>(args, 2);
    if (tex_buf1 == nullptr || item_no != dItemNo_Randomizer_MAGIC_LV1_e) {
        return;
    }

    ResourceBuffer bti = RESOURCE_BUFFER_INIT;
    if (session::svc_mng.resource->load(session::svc_mng.mod_ctx, "shadow_crystal.bti", &bti) == MOD_OK) {
        std::memcpy(tex_buf1, bti.data, bti.size < 0xC00 ? bti.size : 0xC00);
        session::svc_mng.resource->free(session::svc_mng.mod_ctx, &bti);
    }
}

HookAction hookPreShopSeqDecideYes(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<dShopSystem_c*>(args, 0);
    int item_no = 0;

    if (i_this->mFlow.getEventId(&item_no) == 1 && playerIsInRoomStage(3, "R_SP109")) {
        const u16 key = static_cast<u16>((getStageID() << 8) | (item_no & 0xFF));
        if (randomizer_GetContext().mShopOverrides.contains(key)) {
            i_this->setSoldOutFlag();
        }
    }
    return HOOK_CONTINUE;
}

HookAction hookPreCheckFieldItemCreateHeap(ModContext*, void* args, void* retval, void*) {
    auto* i_this = mods::arg<fopAc_ac_c*>(args, 0);

    switch (static_cast<daItemBase_c*>(i_this)->getItemNo()) {
    case dItemNo_Randomizer_EMPTY_BOTTLE_e:
    case dItemNo_Randomizer_HALF_MILK_BOTTLE_e:
    case dItemNo_Randomizer_OIL_BOTTLE3_e:
    case dItemNo_Randomizer_DROP_BOTTLE_e:
    case dItemNo_Randomizer_LINKS_SAVINGS_e:
    case dItemNo_Randomizer_POU_SPIRIT_e:
        *static_cast<int*>(retval) = CheckItemCreateHeap(i_this);
        return HOOK_SKIP_ORIGINAL;
    default:
        return HOOK_CONTINUE;
    }
}

void hookPostTalkEnd(ModContext*, void*, void*, void*) {
    if (g_randomizerState.getHasPendingToDChange()) {
        g_randomizerState.setHasPendingToDChange(false);
        g_randomizerState.handleTimeOfDayChange();
    }
}

HookAction hookPreGetLayerNo(ModContext*, void* args, void* retval, void*) {
    auto i_stageName = mods::arg<const char*>(args, 0);
    auto i_roomNo = mods::arg<int>(args, 1);
    auto& layer = mods::arg_ref<int>(args, 2);

    if (strcmp(dComIfGp_getStartStageName(), "S_MV000") == 0 ||
          (strcmp(dComIfGp_getStartStageName(), "F_SP102") == 0 && layer == 10)) {
        return HOOK_CONTINUE;
    }

    int stageID = getStageID(i_stageName);
    bool condition = false;
    bool darkIsClear = false;

    if (layer < 0) {
        layer = -1;

        // Stage is in a Twilight state
        if (dKy_darkworld_stage_check(i_stageName, i_roomNo) == TRUE) {
            layer = 14;
        }

        if (layer < 13) {
            switch(stageID) {
            case Snowpeak_Ruins: {
                if (dComIfGs_isEventBit(SNOWPEAK_RUINS_CLEARED)) {
                    layer = 3;
                }
                break;
            }
            case Snowpeak: {
                if (dComIfGs_isEventBit(SNOWPEAK_RUINS_CLEARED) && (i_roomNo != 0)) {
                    layer = 3;
                }
                break;
            }
            case Faron_Woods:
            case Faron_Woods_Interiors: {
                if ((i_roomNo == 5) || (i_roomNo == 6)) { // North Faron or Mist Area
                    condition = dComIfGs_isEventBit(ORDON_DAY_2_OVER); // Talo Saved
                    if (condition) {
                        layer = 3;
                    } else {
                        layer = 1;
                    }
                }
                else {
                    condition = dComIfGs_isEventBit(ORDON_DAY_2_OVER); // Talo Saved
                    if (condition) {
                        condition = dComIfGs_isEventBit(FOREST_TEMPLE_CLEARED); // Forest Temple Completed

                        if (condition) {
                            layer = 5;
                        }
                    } else {
                        layer = 1;
                    }
                }
                break;
            }

            case Kakariko_Village:
            {
                condition = dComIfGs_isEventBit(WATCHED_CUTSCENE_AFTER_GORON_MINES); // Cutscene after GM Watched
                if (condition == false) {
                    condition = dComIfGs_isEventBit(GORON_MINES_CLEARED); // Goron Mines Completed
                    if (condition == false) {
                        layer = 2;

                        // If it is night, the layer is different.
                        dComIfG_get_timelayer(&layer);
                    }
                    else {
                        layer = 12;
                    }
                }
                else {
                    layer = 2;
                    dComIfG_get_timelayer(&layer);
                }

                break;
            }
            case Kakariko_Graveyard:
            {
                condition = dComIfGs_isEventBit(GOT_ZORA_ARMOR_FROM_RUTELA); // Got Zora Armor from Rutela
                if (condition == false) {
                    condition = dComIfGs_isEventBit(ZORA_ESCORT_CLEARED); // Zora Escort Cleared

                    if (condition == false) {
                        layer = 2;

                        // If it is night, the layer is different.
                        dComIfG_get_timelayer(&layer);
                    }
                    else {
                        layer = 4;
                    }
                }
                else {
                    layer = 2;
                    dComIfG_get_timelayer(&layer);
                }
                break;
            }

            case Kakariko_Graveyard_Interiors: {
                if (((i_roomNo == 1 &&
                        (condition = dComIfGs_isEventBit(LAKEBED_TEMPLE_CLEARED),
                        condition != false)))) // Lakebed Completed
                {
                    layer = 4;
                    dComIfG_get_timelayer(&layer);
                }
                else {
                    layer = 2;
                    dComIfG_get_timelayer(&layer);
                }
                break;
            }

            case Kakariko_Village_Interiors: {
                if (i_roomNo == 1) { // Lakebed Completed
                    layer = 4;
                    dComIfG_get_timelayer(&layer);
                }
                else if (i_roomNo == 3) {
                    layer = 2;
                }
                else {
                    layer = 2;
                    dComIfG_get_timelayer(&layer);
                }
                break;
            }

            case Death_Mountain: {
                condition =
                    dComIfGs_isEventBit(GORON_MINES_CLEARED); // Goron Mines Completed

                if (condition) {
                    layer = 2;
                }
                break;
            }

            case Death_Mountain_Interiors: {
                layer = 0;
                break;
            }

            case Lake_Hylia: {
                if (i_roomNo == 1) { // Lanayru Spring

                    condition = dComIfGs_isEventBit(LAKEBED_TEMPLE_CLEARED); // Lakebed Temple has been completed
                    if (condition) {
                        condition = dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_STARTED); // MDH has been started
                        if (condition == false) {
                            layer = 9;
                        }
                        else {
                            layer = 2;
                        }
                    }
                }
                else {
                    condition = dComIfGs_isEventBit(SKY_CANNON_REPAIRED); // Sky Cannon Repaired
                    if (condition == false) {
                        condition = dComIfGs_isEventBit(WARPED_SKY_CANNON_TO_LAKE_HYLIA); // Sky Cannon Warped to Lake Hylia

                        if (condition == false) {
                            layer = 2;
                        }
                        else {
                            layer = 1;
                        }
                    }
                    else {
                        layer = 3;
                    }
                }
                break;
            }

            case Castle_Town_Interiors:
            {
                if (condition = dComIfGs_isEventBit(LAKEBED_TEMPLE_CLEARED),condition) { // Lakebed Temple Completed
                    layer = 2;
                    if (condition = dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_COMPLETED),condition) { // MDH Completed
                        layer = 0;
                    }
                }
                if (i_roomNo == 5) { // Telma's Bar
                    layer = 4;
                }
                break;
            }

            case Castle_Town:  {
                condition = dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_COMPLETED); // MDH Completed
                if (condition == false) {
                    condition = dComIfGs_isEventBit(LAKEBED_TEMPLE_CLEARED); // Lakebed Temple Completed
                    if (condition == false) {
                        if ((i_roomNo == 3) &&
                            (condition = dComIfGs_isEventBit(ZORA_ESCORT_CLEARED),condition != false)) { // Zora Escort Cleared
                            layer = 1;
                            }
                        else if (i_roomNo == 4) {
                            layer = 1;
                        }
                    }
                    else {
                        layer = 2;
                    }
                }
                else {
                    if (((i_roomNo == 4) || (i_roomNo == 3)) || (i_roomNo == 1)) {
                        layer = 1;
                    }
                    else {
                        layer = 0;
                    }
                }

                if (i_roomNo == 0) {
                    if (dComIfGs_getStartPoint() == 0xF) {
                        layer = 5;
                    }
                }
                break;
            }

            case Zoras_Domain: {
                layer = 0;
                break;
            }

            case Upper_Zoras_River: {
                condition = dComIfGs_isEventBit(IZA_1_MINIGAME_UNLOCKED); // Iza 1 Unlocked
                if (condition != false)
                {
                    layer = 1;
                }
                break;
            }

            case Gerudo_Desert: {
                layer = 8;

                condition = dComIfGs_isEventBit(VISITED_DESERT_FOR_THE_FIRST_TIME); // Have been to desert
                if (condition != false) {
                    layer = 0;
                }
                break;
            }

            case Zoras_River: {
                condition = dComIfGs_isEventBit(IZA_1_MINIGAME_DONE); // Iza 1 Minigame Completed

                if (condition == false) {
                    condition = dComIfGs_isEventBit(STARTED_IZA_1_MINIGAME); // Iza 1 Minigame Started
                    if (condition != false) {
                        layer = 2;
                    }
                }
                else {
                    layer = 1;
                }
                break;
            }

            case Ordon_Village: {
                if (i_roomNo == 0) {
                    if (!dKy_daynight_check()) {
                        layer = 0;
                    }
                    else {
                        layer = 5;
                    }
                }

                else {
                    if (i_roomNo == 1) {
                        condition =
                            dComIfGs_isEventBit(ORDON_DAY_1_FINISHED); // Ordon Day 1 done

                        if (condition) {
                            condition = dComIfGs_isEventBit(ORDON_DAY_2_OVER); // Talo Saved
                            if (condition) {
                                layer = 2;
                            }
                            else {
                                layer = 4;
                            }
                        }
                        else {
                            layer = 3;
                        }
                    }
                }
                break;
            }

            case Ordon_Village_Interiors:
            {
                /* not used in randomizer anymore. keeping for documentation sake
                if ( i_roomNo == 1 )     // Sera's Shop
                {
                    condition = dComIfGs_isEventBit(
                        BOUGHT_SLINGSHOT_FROM_SERA );     // Bought slinghot from Sera

                    if ( condition )
                    {
                        layer = 2;
                    }
                }*/
                if (i_roomNo == 2) { // Jaggle's House

                    darkIsClear = dComIfGs_isDarkClearLV(0);
                    if (darkIsClear == false) {
                        condition = dComIfGs_isEventBit(FINISHED_SEWERS); // First Trip to Sewers done
                        if (condition != false) {
                            layer = 1;
                        }
                    }
                    else {
                        layer = 1;
                    }
                }
                /* not used in randomizer anymore. keeping for documentation sake
                else
                {
                    if ( i_roomNo == 5 )     // Rusl's House
                    {
                        darkIsClear = libtp::tp::d_save::isDarkClearLV( playerStatusBPtr, 0 );
                        if ( darkIsClear != false )
                        {
                            layer = 2;
                        }
                    }
                }*/

                break;
            }

            case Ordon_Spring: {
                condition = dComIfGs_isEventBit(ORDON_DAY_2_OVER); // Talo saved
                if (condition) {
                    condition =
                        dComIfGs_isEventBit(FINISHED_SEWERS); // First trip to Sewers done

                    if (condition) {
                        darkIsClear = dComIfGs_isDarkClearLV(0);
                        if (darkIsClear != false) {
                            layer = 2;
                        }
                        else {
                            layer = 4;
                        }
                    }
                    else {
                        layer = 0;
                    }
                }
                else {
                    condition = dComIfGs_isEventBit(TALO_CHASES_MONKEY); // Sword training done on Ordon Day 2
                    if (condition) {
                        layer = 3;
                    }
                    else {
                        layer = 1;
                    }
                }

                break;
            }

            case Ordon_Ranch: {
                condition = dComIfGs_isEventBit(ORDON_DAY_1_FINISHED); // Day 1 done
                if (condition) {
                    condition = dComIfGs_isEventBit(ORDON_DAY_2_OVER); // Talo Saved
                    if (condition) {
                        condition = dComIfGs_isEventBit(WATCHED_CUTSCENE_AFTER_GOATS_2); // Saw CS after Goats 2 done

                        if (condition) {
                            layer = 2;
                            dComIfG_get_timelayer(&layer);
                        }
                        else {
                            layer = 9;
                        }
                    }
                    else {
                        layer = 2;
                    }
                }
                else {
                    layer = 12;
                }
                break;
            }

            case Hyrule_Field: {
                // First 3 twilights are cleared
                if ((dComIfGs_getSaveData()->getPlayer().getPlayerStatusB().mDarkClearLevelFlag & 0x7) == 0x7) {
                    if (dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_COMPLETED)) {
                        layer = 6;
                    }
                    else if (dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_STARTED)) {
                        layer = 4;
                    }
                    else {
                        layer = 0;
                    }
                }
                else {
                    layer = 0;
                }
                break;
            }

            case Outside_Castle_Town: {
                if (i_roomNo == 8) {
                    condition = dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_COMPLETED); // MDH Completed
                    if (condition == false) {
                        condition = dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_STARTED); // MDH State Activated
                        if (condition != false) {
                            layer = 4;
                        }
                    }
                    else {
                        layer = 6;
                    }
                }
                else {
                    if (i_roomNo == 0x10) {
                        condition = dComIfGs_isEventBit(GOT_WOOD_STATUE); // Wooden Statue Gotten
                        if (condition == false) {
                            condition = dComIfGs_isEventBit(TALKED_TO_LOUISE_ABOUT_THE_STOLEN_STATUE); // Talked to Louise after Medicine Scent
                            if (condition == false) {
                                condition = dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_COMPLETED); // MDH Completed
                                if (condition == false) {
                                    condition = dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_STARTED); // MDH State Activated
                                    if (condition != false) {
                                        layer = 4;
                                    }
                                    else {
                                        layer = 6;
                                    }
                                }
                                else {
                                    layer = 6;
                                }
                            }
                            else {
                                layer = 1;
                            }
                        }
                        else {
                            layer = 6;
                        }
                    }
                    else {
                        if (i_roomNo == 0x11) {
                            condition = dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_COMPLETED); // MDH Completed
                            if (condition == false) {
                                condition = dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_STARTED); // MDH State Activated
                                if (condition != false) {
                                    layer = 4;
                                }
                            }
                            else {
                                layer = 0;
                            }
                        }
                    }
                }
                break;
            }

            case Hidden_Village: {
                condition = dComIfGs_isEventBit(GAVE_ILIA_THE_WOOD_STATUE); // Ilia shown the wooden statue
                if (condition != false) {
                    condition = dComIfGs_isEventBit(GOT_ILIAS_CHARM); // Ilia shown Ilia's Charm
                    if (condition != false) {
                        layer = 1;
                    }
                }
                else {
                    layer = 1;
                }

                break;
            }

            case Castle_Town_Shops: {
                if (i_roomNo == 5) {
                    layer = 0;
                    condition = dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_STARTED);
                    if (condition) {
                        layer = 1;
                        condition = dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_COMPLETED);
                        if (condition) {
                            layer = 0;
                        }
                    }
                }
                else {
                    condition = dComIfGs_isEventBit(MALO_MART_CASTLE_TOWN_BRANCH_IS_OPEN); // CT Shop is Malo Mart

                    if (condition != false) {
                        layer = 1;
                    }
                }
                break;
            }

            case Sacred_Grove: {
                layer = 2;
                break;
            }

            case Bulblin_Camp: {
                condition = dComIfGs_isEventBit(ESCAPED_BURNING_TENT_IN_BULBLIN_CAMP); // Escaped Burning Tent in Bulblin Camp
                if (condition) {
                    if (i_roomNo == 3) // Other states for this room are very similar, but do not have the boar
                        // in the dzx.
                    { // Setting state 1 solves for any potential softlocks regarding the boar in that area.
                        layer = 1;
                    }
                    else {
                        layer = 3;
                    }
                }
                break;
            }

            case Faron_Woods_Cave: {
                condition = dComIfGs_isEventBit(ORDON_DAY_2_OVER); // Talo saved
                if (condition != false) {
                    layer = 1;
                }
                break;
            }

            case Hyrule_Castle_Sewers: {
                condition = dComIfGs_isEventBit(FINISHED_SEWERS); // Sewers Finished
                if (condition) {
                    layer = 13;
                }
                else {
                    layer = 14;
                }
                break;
            }

            case Hyrule_Castle: {
                if (((i_roomNo != 0xb) && (i_roomNo != 0xd)) && (i_roomNo != 0xe)) {
                    layer = 1;
                }
                break;
            }

            case Fishing_Pond:
            case Fishing_Pond_Interiors: {
                switch (g_env_light.fishing_hole_season) {
                case 1:
                    layer = 0;
                    break;
                case 2:
                    layer = 1;
                    break;
                case 3:
                    layer = 2;
                    break;
                case 4:
                    layer = 3;
                    break;
                }
                break;
            }
            default: {
                break;
            }
            }
        }
    }

    return HOOK_CONTINUE;
}

HookAction hookPreOnStageSwitch(ModContext*, void* args, void* retval, void*) {
    const int i_stageNo = mods::arg<int>(args, 0);
    const int i_no = mods::arg<int>(args, 1);

    // Avoid trying to get the save table if stag info is NULL
    if (dComIfGp_getStageStagInfo() == NULL) {
        dComIfGs_onSaveSwitch(i_stageNo, i_no);
        return HOOK_SKIP_ORIGINAL;
    }

    return HOOK_CONTINUE;
}

HookAction hookPreDecideDoStatus(ModContext*, void* args, void* retval, void*) {
    daAlink_c* i_this = mods::arg<daAlink_c*>(args, 0);
    bool set_status = false;

    if (i_this->mAttList != NULL) {
        s16 actor_name = fopAcM_GetName(i_this->field_0x27f4);
        if (actor_name == fpcNm_Tag_Lv6Gate_e ||
            (actor_name == fpcNm_TAG_KMSG_e && static_cast<daTag_KMsg_c*>(i_this->field_0x27f4)->getType() == 3))
        {
            // Separate check for striking sword into the pedestal for randomizer
            if (!i_this->checkEquipAnime() && randomizer_checkTempleOfTimeRequirement()) {
                i_this->setDoStatus(BUTTON_STATUS_STRIKE);
                set_status = true;
            }
        }
    }

    if (set_status) {
        i_this->decideCommonDoStatus();
        return HOOK_SKIP_ORIGINAL;
    }

    return HOOK_CONTINUE;
}

HookAction hookPreSearchBouDoor(ModContext*, void* args, void* retval, void*) {
    // In randomizer, we don't want Bo preventing us from entering his house on Day 2
    if (daAlink_c::checkStageName("F_SP103"))
    {
        *static_cast<void**>(retval) = nullptr;
        return HOOK_SKIP_ORIGINAL;
    }

    return HOOK_CONTINUE;
}

HookAction hookPreCheckGroundSpecialMode(ModContext*, void* args, void* retval, void*) {
    daAlink_c* i_this = mods::arg<daAlink_c*>(args, 0);

    if (i_this->mLinkAcch.ChkGroundHit()
        && !i_this->checkModeFlg(daAlink_c::MODE_PLAYER_FLY)
        && !i_this->checkMagneBootsOn()
        && i_this->checkEndResetFlg0(daAlink_c::ERFLG0_FORCE_WOLF_CHANGE))
    {
        u8 stage = getStageID();
        // In rando, don't transform in twilight fog unless we have shadow crystal
        if (!dComIfGs_isEventBit(TRANSFORMING_UNLOCKED) &&
            (stage == Palace_of_Twilight || stage == Phantom_Zant_1 || stage == Phantom_Zant_2))
        {
            *static_cast<BOOL*>(retval) = FALSE;
            return HOOK_SKIP_ORIGINAL;
        }
        *static_cast<BOOL*>(retval) = i_this->procCoMetamorphoseInit();
        return HOOK_SKIP_ORIGINAL;
    }

    return HOOK_CONTINUE;
}

void hookPostSetGetItemFace(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<daAlink_c*>(args, 0);
    const u16 i_itemNo = mods::arg<u16>(args, 1);

    switch (i_itemNo) {
    case dItemNo_Randomizer_WOOD_STICK_e:
    case dItemNo_Randomizer_SWORD_e:
    case dItemNo_Randomizer_SHIELD_e:
    case dItemNo_Randomizer_MASTER_SWORD_e:
    case dItemNo_Randomizer_LIGHT_SWORD_e:
    case dItemNo_Randomizer_MAGIC_LV1_e:
        i_this->setFaceBasicBck(dRes_ID_ALANM_BCK_FI_e);
        break;
    case dItemNo_Randomizer_FOOLISH_ITEM_e:
        i_this->setFaceBasicBck(dRes_ID_ALANM_BCK_FJ_e);
        break;
    }
}

HookAction hookPreSetGetSubBgm(ModContext*, void* args, void*, void*) {
    enum {
        SETYPE_HEART,
        SETYPE_ITEM_GET,
        SETYPE_ITEM_GET_MINI,
        SETYPE_ITEM_GET_ME,
        SETYPE_ITEM_GET_INSECT,
        SETYPE_ITEM_GET_SMELL,
        SETYPE_ITEM_GET_POU,
        SETYPE_ITEM_GET_ME_S,
        SETYPE_NONE,
    };

    static constexpr u8 getSeTypeRandomizer[255] = {
        /* dItemNo_Randomizer_HEART_e             */ SETYPE_NONE,
        /* dItemNo_Randomizer_GREEN_RUPEE_e       */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_BLUE_RUPEE_e        */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_YELLOW_RUPEE_e      */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_RED_RUPEE_e         */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_PURPLE_RUPEE_e      */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_ORANGE_RUPEE_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_SILVER_RUPEE_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_S_MAGIC_e           */ SETYPE_NONE,
        /* dItemNo_Randomizer_L_MAGIC_e           */ SETYPE_NONE,
        /* dItemNo_Randomizer_BOMB_5_e            */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_BOMB_10_e           */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_BOMB_20_e           */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_BOMB_30_e           */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_ARROW_10_e          */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_ARROW_20_e          */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_ARROW_30_e          */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_ARROW_1_e           */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_PACHINKO_SHOT_e     */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_FOOLISH_ITEM_e      */ SETYPE_NONE,
        /* dItemNo_Randomizer_NOENTRY_20_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_21_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_WATER_BOMB_5_e      */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_WATER_BOMB_10_e     */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_WATER_BOMB_20_e     */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_WATER_BOMB_30_e     */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_BOMB_INSECT_5_e     */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_BOMB_INSECT_10_e    */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_BOMB_INSECT_20_e    */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_BOMB_INSECT_30_e    */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_RECOVERY_FAILY_e    */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_TRIPLE_HEART_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_SMALL_KEY_e         */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_KAKERA_HEART_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_UTAWA_HEART_e       */ SETYPE_HEART,
        /* dItemNo_Randomizer_MAP_e               */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_COMPUS_e            */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_DUNGEON_EXIT_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_BOSS_KEY_e          */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_DUNGEON_BACK_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_SWORD_e             */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_MASTER_SWORD_e      */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_WOOD_SHIELD_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_SHIELD_e            */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_HYLIA_SHIELD_e      */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_TKS_LETTER_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_WEAR_CASUAL_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_WEAR_KOKIRI_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_ARMOR_e             */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_WEAR_ZORA_e         */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_MAGIC_LV1_e         */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_DUNGEON_EXIT_2_e    */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_WALLET_LV1_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_WALLET_LV2_e        */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_WALLET_LV3_e        */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_55_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_56_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_57_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_58_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_59_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_60_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_ZORAS_JEWEL_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_HAWK_EYE_e          */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_WOOD_STICK_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_BOOMERANG_e         */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_SPINNER_e           */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_IRONBALL_e          */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_BOW_e               */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_HOOKSHOT_e          */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_HVY_BOOTS_e         */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_COPY_ROD_e          */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_W_HOOKSHOT_e        */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_KANTERA_e           */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_LIGHT_SWORD_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_FISHING_ROD_1_e     */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_PACHINKO_e          */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_COPY_ROD_2_e        */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_77_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_78_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_BOMB_BAG_LV2_e      */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_BOMB_BAG_LV1_e      */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_BOMB_IN_BAG_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_82_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_LIGHT_ARROW_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_ARROW_LV1_e         */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_ARROW_LV2_e         */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_ARROW_LV3_e         */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_87_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_LURE_ROD_e          */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_BOMB_ARROW_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_HAWK_ARROW_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_BEE_ROD_e           */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_JEWEL_ROD_e         */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_WORM_ROD_e          */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_JEWEL_BEE_ROD_e     */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_JEWEL_WORM_ROD_e    */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_EMPTY_BOTTLE_e      */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_RED_BOTTLE_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_GREEN_BOTTLE_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_BLUE_BOTTLE_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_MILK_BOTTLE_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_HALF_MILK_BOTTLE_e  */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_OIL_BOTTLE_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_WATER_BOTTLE_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_OIL_BOTTLE_2_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_RED_BOTTLE_2_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_UGLY_SOUP_e         */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_HOT_SPRING_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_FAIRY_e             */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_HOT_SPRING_2_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_OIL2_e              */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_OIL_e               */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NORMAL_BOMB_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_WATER_BOMB_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_POKE_BOMB_e         */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_FAIRY_DROP_e        */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_WORM_e              */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_DROP_BOTTLE_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_BEE_CHILD_e         */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_CHUCHU_RARE_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_CHUCHU_RED_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_CHUCHU_BLUE_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_CHUCHU_GREEN_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_CHUCHU_YELLOW_e     */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_CHUCHU_PURPLE_e     */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_LV1_SOUP_e          */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_LV2_SOUP_e          */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_LV3_SOUP_e          */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_LETTER_e            */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_BILL_e              */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_WOOD_STATUE_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_IRIAS_PENDANT_e     */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_HORSE_FLUTE_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_133_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_134_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_135_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_136_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_137_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_138_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_139_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_140_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_141_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_142_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_143_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_RAFRELS_MEMO_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_ASHS_SCRIBBLING_e   */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_146_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_147_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_148_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_149_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_150_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_151_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_152_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_153_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_154_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_155_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_CHUCHU_YELLOW2_e    */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_OIL_BOTTLE3_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_SHOP_BEE_CHILD_e    */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_CHUCHU_BLACK_e      */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_LIGHT_DROP_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_DROP_CONTAINER_e    */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_DROP_CONTAINER02_e  */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_DROP_CONTAINER03_e  */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_FILLED_CONTAINER_e  */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_MIRROR_PIECE_2_e    */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_MIRROR_PIECE_3_e    */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_MIRROR_PIECE_4_e    */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_168_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_169_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_170_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_171_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_172_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_173_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_174_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_175_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_SMELL_YELIA_POUCH_e */ SETYPE_ITEM_GET_SMELL,
        /* dItemNo_Randomizer_SMELL_PUMPKIN_e     */ SETYPE_ITEM_GET_SMELL,
        /* dItemNo_Randomizer_SMELL_POH_e         */ SETYPE_ITEM_GET_SMELL,
        /* dItemNo_Randomizer_SMELL_FISH_e        */ SETYPE_ITEM_GET_SMELL,
        /* dItemNo_Randomizer_SMELL_CHILDREN_e    */ SETYPE_ITEM_GET_SMELL,
        /* dItemNo_Randomizer_SMELL_MEDICINE_e    */ SETYPE_ITEM_GET_SMELL,
        /* dItemNo_Randomizer_NOENTRY_182_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_183_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_184_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_185_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_186_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_187_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_188_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_189_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_190_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_NOENTRY_191_e       */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_M_BEETLE_e          */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_F_BEETLE_e          */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_M_BUTTERFLY_e       */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_F_BUTTERFLY_e       */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_M_STAG_BEETLE_e     */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_F_STAG_BEETLE_e     */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_M_GRASSHOPPER_e     */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_F_GRASSHOPPER_e     */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_M_NANAFUSHI_e       */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_F_NANAFUSHI_e       */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_M_DANGOMUSHI_e      */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_F_DANGOMUSHI_e      */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_M_MANTIS_e          */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_F_MANTIS_e          */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_M_LADYBUG_e         */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_F_LADYBUG_e         */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_M_SNAIL_e           */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_F_SNAIL_e           */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_M_DRAGONFLY_e       */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_F_DRAGONFLY_e       */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_M_ANT_e             */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_F_ANT_e             */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_M_MAYFLY_e          */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_F_MAYFLY_e          */ SETYPE_ITEM_GET_INSECT,
        /* dItemNo_Randomizer_NOENTRY_216_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_217_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_218_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_219_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_220_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_221_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_222_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_223_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_POU_SPIRIT_e        */ SETYPE_ITEM_GET_POU,
        /* dItemNo_Randomizer_NOENTRY_225_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_226_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_227_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_228_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_229_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_230_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_231_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_NOENTRY_232_e       */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_ANCIENT_DOCUMENT_e  */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_AIR_LETTER_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_ANCIENT_DOCUMENT2_e */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_LV7_DUNGEON_EXIT_e  */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_LINKS_SAVINGS_e     */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_SMALL_KEY2_e        */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_POU_FIRE1_e         */ SETYPE_NONE,
        /* dItemNo_Randomizer_POU_FIRE2_e         */ SETYPE_NONE,
        /* dItemNo_Randomizer_POU_FIRE3_e         */ SETYPE_NONE,
        /* dItemNo_Randomizer_POU_FIRE4_e         */ SETYPE_NONE,
        /* dItemNo_Randomizer_BOSSRIDER_KEY_e     */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_TOMATO_PUREE_e      */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_TASTE_e             */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_LV5_BOSS_KEY_e      */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_SURFBOARD_e         */ SETYPE_NONE,
        /* dItemNo_Randomizer_KANTERA2_e          */ SETYPE_ITEM_GET_ME,
        /* dItemNo_Randomizer_L2_KEY_PIECES1_e    */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_L2_KEY_PIECES2_e    */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_L2_KEY_PIECES3_e    */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_KEY_OF_CARAVAN_e    */ SETYPE_ITEM_GET_MINI,
        /* dItemNo_Randomizer_LV2_BOSS_KEY_e      */ SETYPE_ITEM_GET,
        /* dItemNo_Randomizer_KEY_OF_FILONE_e     */ SETYPE_ITEM_GET_MINI,
    };

    static constexpr u32 bgmLabel[8] = {
        Z2BGM_HEART_GET,       Z2BGM_ITEM_GET,       Z2BGM_ITEM_GET_MINI, Z2BGM_ITEM_GET_ME,
        Z2BGM_ITEM_GET_INSECT, Z2BGM_ITEM_GET_SMELL, Z2BGM_ITEM_GET_POU,  Z2BGM_ITEM_GET_ME_S,
    };

    auto* i_this = mods::arg<daAlink_c*>(args, 0);
    const int i_itemNo = mods::arg<int>(args, 1);
    u32 se_type = getSeTypeRandomizer[i_itemNo];

    if (se_type == SETYPE_ITEM_GET_ME && i_this->mProcVar4.field_0x3010 == 0) {
        se_type = SETYPE_ITEM_GET_ME_S;
    }

    if (se_type != SETYPE_NONE || i_itemNo == dItemNo_Randomizer_FOOLISH_ITEM_e) {
        mDoAud_subBgmStart(bgmLabel[se_type]);
        dComIfGp_setMesgBgmOn();
    }

    return HOOK_SKIP_ORIGINAL;
}

HookAction hookPreProcCoGetItem(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<daAlink_c*>(args, 0);
    if (i_this->field_0x32cc != 0 || i_this->mProcVar2.field_0x300c != dItemNo_Randomizer_POU_SPIRIT_e) {
        return HOOK_CONTINUE;
    }

    auto* item_partner_p = static_cast<daItemBase_c*>(fopAcM_getItemEventPartner(i_this));
    if (item_partner_p == nullptr || fpcM_IsCreating(fpcM_GetID(item_partner_p))) {
        return HOOK_CONTINUE;
    }

    // Don't show special text in rando
    if (dComIfGs_getPohSpiritNum() == 20) {
        i_this->field_0x32cc = i_this->mProcVar2.field_0x300c + 0x65;
    }

    return HOOK_CONTINUE;
}

u8 hookProcCoWarpInit_prevSlot18 = dItemNo_NONE_e;
bool hookProcCoWarpInit_restoreSlot18 = false;
HookAction hookPreProcCoWarpInit(ModContext*, void* args, void*, void*) {
    hookProcCoWarpInit_restoreSlot18 = false;

    auto* i_this = mods::arg<daAlink_c*>(args, 0);
    const int param_0 = mods::arg<int>(args, 1);
    const int param_1 = mods::arg<int>(args, 2);

    // copy necessary functionality from og function to set up patch state
    if (param_0 != 0 || i_this->checkWolf()) {
        return HOOK_CONTINUE;
    }

    const BOOL isSideWarp = param_1 == 0 &&
        ((daAlink_c::checkStageName("F_SP125") && fopAcM_GetRoomNo(i_this) == 4) ||
         (daAlink_c::checkStageName("D_MN08") && fopAcM_GetRoomNo(i_this) == 0));
    if (isSideWarp || !i_this->checkBossRoom() || fopAcM_GetRoomNo(i_this) != 50) {
        return HOOK_CONTINUE;
    }

    char stageName[32];
    SAFE_STRCPY(stageName, dComIfGp_getStartStageName());

    for (int i = 0; i < 32; i++) {
        if ((s64)stageName[i] == 0) {
            stageName[i - 1] = 0;
            break;
        }
    }

    if (!checkItemGet(dItemNo_DUNGEON_EXIT_e, 1) &&
        !(checkItemGet(dItemNo_DUNGEON_BACK_e, 1) &&
          strcmp(stageName, dComIfGs_getWarpStageName()) == 0))
    {
        return HOOK_CONTINUE;
    }

    // In rando, we only want to clear the Ooccoo slot if Ooccoo is in it.
    // so continue with normal function if Ooccoo is in slot.
    u8 ooccooSlot = dComIfGs_getItem(SLOT_18, false);
    if (ooccooSlot == dItemNo_Randomizer_DUNGEON_EXIT_e ||
        ooccooSlot == dItemNo_Randomizer_DUNGEON_EXIT_2_e ||
        ooccooSlot == dItemNo_Randomizer_LV7_DUNGEON_EXIT_e)
    {
        return HOOK_CONTINUE;
    }

    hookProcCoWarpInit_prevSlot18 = ooccooSlot;
    hookProcCoWarpInit_restoreSlot18 = true;
    return HOOK_CONTINUE;
}

void hookPostProcCoWarpInit(ModContext*, void*, void*, void*) {
    if (hookProcCoWarpInit_restoreSlot18) {
        dComIfGs_setItem(SLOT_18, hookProcCoWarpInit_prevSlot18);
        hookProcCoWarpInit_restoreSlot18 = false;
    }
}

void hookPostBqEnd(ModContext*, void* args, void* retval, void*) {
    // If the player is wolf, they will softlock after the defeat cutscene is completed.
    checkTransformFromWolf();
}

HookAction hookPreCheckOpenMsgDoor(ModContext*, void* args, void* retval, void*) {
    daDoor20_c* i_this = mods::arg<daDoor20_c*>(args, 0);
    int* param_1 = mods::arg<int*>(args, 1);

    if (!door_param2_c::isMsgDoor(i_this)) {
        *static_cast<int*>(retval) = 1;
        return HOOK_SKIP_ORIGINAL;
    }

    int msgNo = door_param2_c::getMsgNo(i_this);
    if (msgNo == 0xffff) {
        *param_1 = 0;
        *static_cast<int*>(retval) = 1;
        return HOOK_SKIP_ORIGINAL;
    }

    i_this->field_0x624.init(NULL, msgNo, 0, NULL);
    int rv = 1;
    // If we are in SPR, we don't want Yeta's msg flow to prevent us from opening the door if we haven't talked to her.
    if (!daAlink_c::checkStageName("D_MN11")) {
        rv = i_this->field_0x624.checkOpenDoor(i_this, param_1);
    }
    dMsgObject_endFlowGroup();
    *static_cast<int*>(retval) = rv;
    return HOOK_SKIP_ORIGINAL;
}

void hookPostEmkDemoCameraEnd(ModContext*, void* args, void* retval, void*) {
    e_mk_class* i_this = mods::arg<e_mk_class*>(args, 0);

    switch (i_this->demoSubMode) {
    case 6:
        if (i_this->demoCamCounter == 180) {
            // If the player is wolf, they will void and lose the boomerang check.
            checkTransformFromWolf();
        }
        break;
    }
}

void hookPostChangeScene4Event(ModContext*, void* args, void* retval, void*) {
    int i_exitId = mods::arg<int>(args, 0);
    s8 room_no = mods::arg<s8>(args, 1);

    stage_scls_info_dummy_class* scls;
    if (room_no == -1) {
        scls = dComIfGp_getStageSclsInfo();
    } else {
        dStage_roomDt_c* room = dComIfGp_roomControl_getStatusRoomDt(room_no);
        scls = room->getSclsInfo();
    }

    if (scls == NULL) {
        return;
    }

    stage_scls_info_class* scls_info = &scls->m_entries[i_exitId];

    // If randomizer is active and we're loading the first spawn, set our starting time of day
    if (std::strcmp(scls_info->mStage, "F_SP103") == 0
        && scls_info->mRoom == 1
        && scls_info->mStart == 1)
    {
        dKy_set_nexttime(15.0f * randomizer_GetContext().mStartHour);
        g_randomizerState.mUpdateTracker = true;
    }
}

HookAction hookPreStagePlayerInit(ModContext*, void* args, void* retval, void*) {
    void* i_data = mods::arg<void*>(args, 1);
    int num = mods::arg<int>(args, 2);

    stage_actor_class* player = (stage_actor_class*)((int*)i_data + 1);
    stage_actor_data_class* player_data = player->m_entries;

    // Modify entrance types in certain situations to avoid crashes
    for (size_t i = 0; i < num; ++i) {
        u8& entranceType = reinterpret_cast<u8*>(&player_data[i].base.parameters)[2];
        switch (entranceType) {
        // Only replace the entrance type if it is a door.
        case 0x80:
        case 0xA0:
        case 0xB0:
        {
            if (dComIfGs_getTransformStatus() == TF_STATUS_WOLF) {
                // Change the entrance type to play the animation of walking out of the
                // loading zone instead of entering through the door.
                entranceType = 0x50;
            }
            break;
        }

        // Water swimming entrance.
        // If we have this, but there isn't any water to spawn in, the game hangs
        case 0xD0:
        {
            // If there's no water, change to non-swimming entrance
            if (getStageID() == Lake_Hylia && !dComIfGs_isEventBit(WARPED_METEOR_TO_ZORAS_DOMAIN)) {
                entranceType = 0x50;
            }
            break;
        }
        default:
            break;
        }
    }

    return HOOK_CONTINUE;
}

void hookPostKytag08Execute(ModContext*, void* args, void* retval, void*) {
    kytag08_class* i_this = mods::arg<kytag08_class*>(args, 0);

    if (i_this->mSizeTimer < 100 || dComIfGs_BossLife_public_Get() == 1) {
        dComIfGs_BossLife_public_Set(0);
        i_this->mTargetAvoidPos = i_this->current.pos;
        i_this->mSizeTimer = 180;
        mDoAud_startFogWipeTrigger(&i_this->current.pos);
    }
}

HookAction hookPreNpcTChkEvtBit(ModContext*, void* args, void* retval, void*) {
    u32 i_no = mods::arg<u32>(args, 0);

    switch (i_no) {
    case 0x153: // Checking if the player has Ending Blow
        if (getStageID() == Hidden_Skill) {
            *static_cast<BOOL*>(retval) = TRUE;
            return HOOK_SKIP_ORIGINAL;
        }
        break;
    case 0x40: // Checking if the player has completed Goron Mines
        if (getStageID() == Kakariko_Village_Interiors) {
            // Return true so Barnes will sell bombs no matter what
            *static_cast<BOOL*>(retval) = TRUE;
            return HOOK_SKIP_ORIGINAL;
        }
        break;
    }

    return HOOK_CONTINUE;
}

HookAction hookPreNpcFChkEvtBit(ModContext*, void* args, void* retval, void*) {
    u32 i_no = mods::arg<u32>(args, 0);

    // shad handling
    if (std::strcmp(dComIfGp_getStartStageName(), "R_SP209") == 0) {
        switch (i_no) {
        case 0x10B:
            // spawn even if vanilla city requirements aren't met
            *static_cast<BOOL*>(retval) = TRUE;
            return HOOK_SKIP_ORIGINAL;
        case 0x12E:
        case 0x12F:
            // skip vanilla cannon spawn failure and move checks
            *static_cast<BOOL*>(retval) = FALSE;
            return HOOK_SKIP_ORIGINAL;
        case 0x311:
            // despawn after custom flag is set
            *static_cast<BOOL*>(retval) = dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x333]);
            return HOOK_SKIP_ORIGINAL;
        }
    }

    switch (i_no) {
    case 0x169: // Checking if Raised Mirror in Mirror Chamber
        // Only let Auru despawn in randomizer if we already collected his item
        if (getStageID() == Lake_Hylia) {
            *static_cast<BOOL*>(retval) = dComIfGs_isEventBit(GOT_AURUS_MEMO);
            return HOOK_SKIP_ORIGINAL;
        }
        break;
    }

    return HOOK_CONTINUE;
}

HookAction hookPreNpcBansIsDelete(ModContext*, void* args, void* retval, void*) {
    daNpc_Bans_c* i_this = mods::arg<daNpc_Bans_c*>(args, 0);

    switch (i_this->mType) {
    case 3: // MAKING_BOMBS
        *static_cast<BOOL*>(retval) = TRUE;
        return HOOK_SKIP_ORIGINAL;
    case 4: // SHOP
        *static_cast<BOOL*>(retval) = FALSE;
        return HOOK_SKIP_ORIGINAL;
    }

    return HOOK_CONTINUE;
}

HookAction hookPreNpcFOrderEvent(ModContext*, void* args, void* retval, void*) {
    daNpcF_c* i_this = mods::arg<daNpcF_c*>(args, 0);
    int& i_forceSpeak = mods::arg_ref<int>(args, 1);
    u16 i_priority = mods::arg<u16>(args, 4);

    // kinda hacky way to check for the state where Bo is trying to talk after getting Iron Boots
    if (i_this->eventInfo.getArchiveName() == nullptr) {
        return HOOK_CONTINUE;
    }
    const std::string arcName = i_this->eventInfo.getArchiveName();
    if (arcName == "Bou4" && i_priority == 40) {
        i_forceSpeak = FALSE;
    }

    return HOOK_CONTINUE;
}

void hookPostFairyAppearDemoCall(ModContext*, void* args, void* retval, void*) {
    daNpc_Fairy_c* i_this = mods::arg<daNpc_Fairy_c*>(args, 0);

    // randomizer overrides EVT_APPEAR_50F_04 set to always be EVT_APPEAR_50F_01
    if (i_this->field_0xff4 == 12) {
        i_this->field_0xff4 = 9;
    }
}

void hookPostYkMIsDelete(ModContext*, void* args, void* retval, void*) {
    daNpc_ykM_c* i_this = mods::arg<daNpc_ykM_c*>(args, 0);

    if (i_this->mType == daNpc_ykM_c::TYPE_COOK) {
        // We don't want cooking Yeto to leave the dungeon, even if the BK is obtained.
        *static_cast<BOOL*>(retval) = FALSE;
    }
}

void hookPostYkWIsDelete(ModContext*, void* args, void* retval, void*) {
    daNpc_ykW_c* i_this = mods::arg<daNpc_ykW_c*>(args, 0);

    if (i_this->field_0xf80 == 1) {
        // We don't want Yeta to leave the dungeon, even if the BK is obtained.
        *static_cast<BOOL*>(retval) = FALSE;
    }
}

void hookPostNpcZrzIsDelete(ModContext*, void* args, void* retval, void*) {
    if (dComIfGs_isEventBit(GOT_ZORA_ARMOR_FROM_RUTELA)) {
        return;
    }

    auto* i_this = mods::arg<daNpc_zrZ_c*>(args, 0);
    if (i_this->mDemoMode != 3) {
        return;
    }

    const int roomNo = fopAcM_GetRoomNo(i_this);
    if (dComIfGs_isSwitch(i_this->mSwitch1, roomNo) && dComIfGs_isSwitch(i_this->mSwitch2, roomNo)) {
        *static_cast<BOOL*>(retval) = FALSE;
    }
}

u8 hookNpcShadWaitType1_patchItemNo = dItemNo_NONE_e;
bool hookNpcShadWaitType1_isPatchItemNo = false;
HookAction hookPreNpcShadWaitType1(ModContext*, void* args, void* retval, void*) {
    dEvt_control_c* event = dComIfGp_getEvent();
    hookNpcShadWaitType1_isPatchItemNo = false;

    if (event->mPreItemNo >= dItemNo_Randomizer_ANCIENT_DOCUMENT_e) {
        // backup original item no so we can restore it in post hook
        hookNpcShadWaitType1_patchItemNo = event->mPreItemNo;

        // force item no so that the dominion rod check is forced regardless
        // of which skybook you have
        event->mPreItemNo = dItemNo_Randomizer_ANCIENT_DOCUMENT_e;

        hookNpcShadWaitType1_isPatchItemNo = true;
    }

    return HOOK_CONTINUE;
}

void hookPostNpcShadWaitType1(ModContext*, void* args, void* retval, void*) {
    if (hookNpcShadWaitType1_isPatchItemNo) {
        dComIfGp_getEvent()->mPreItemNo = hookNpcShadWaitType1_patchItemNo;
        hookNpcShadWaitType1_isPatchItemNo = false;
    }
}

// kinda ugly way to handle the wood statue. using a flag here to tell when its the right time
// to be overriding the setWarashibeItem call with offWarashibeItem instead. maybe look into
// a better solution later
bool hookYeliaTakeWoodStatue_isOffWarashibeItem = false;
HookAction hookPreNpcYeliaCutTakeWoodStatue(ModContext*, void* args, void*, void*) {
    hookYeliaTakeWoodStatue_isOffWarashibeItem = false;

    auto* i_this = mods::arg<daNpc_Yelia_c*>(args, 0);
    const int i_staffId = mods::arg<int>(args, 1);
    const int yeliaStaffId = dComIfGp_evmng_getMyStaffId("Yelia", i_this, -1);
    if (i_staffId != yeliaStaffId || !dComIfGp_getEventManager().getIsAddvance(i_staffId)) {
        return HOOK_CONTINUE;
    }

    int* prm = dComIfGp_evmng_getMyIntegerP(i_staffId, "prm");
    if (prm != nullptr && *prm == 99) {
        hookYeliaTakeWoodStatue_isOffWarashibeItem = true;
    }

    return HOOK_CONTINUE;
}

void hookPostNpcYeliaCutTakeWoodStatue(ModContext*, void*, void*, void*) {
    hookYeliaTakeWoodStatue_isOffWarashibeItem = false;
}

HookAction hookPreSetWarashibeItem(ModContext*, void* args, void*, void*) {
    if (!hookYeliaTakeWoodStatue_isOffWarashibeItem) {
        return HOOK_CONTINUE;
    }

    const u8 itemNo = mods::arg<u8>(args, 1);
    if (itemNo == dItemNo_NONE_e) {
        offWarashibeItem(dItemNo_Randomizer_WOOD_STATUE_e);
        return HOOK_SKIP_ORIGINAL;
    }

    return HOOK_CONTINUE;
}

// kinda hacky, force the SkipInfo check to always fail so that the armor always gets created,
// then restore the original SkipInfo afterward. needed to make the ball and chain check respawn
// if the player leaves the room without grabbing it
daE_MD_c* hookEMdCreate_skipActor = nullptr;
u8 hookEMdCreate_prevSkipInfo = 0;
HookAction hookPreEMdCreate(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<daE_MD_c*>(args, 0);
    if (cDmr_SkipInfo == 0 || i_this->current.pos.z <= -1500.0f) {
        return HOOK_CONTINUE;
    }

    hookEMdCreate_skipActor = i_this;
    hookEMdCreate_prevSkipInfo = cDmr_SkipInfo;
    cDmr_SkipInfo = 0;
    return HOOK_CONTINUE;
}

void hookPostEMdCreate(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<daE_MD_c*>(args, 0);
    if (hookEMdCreate_skipActor != i_this) {
        return;
    }

    cDmr_SkipInfo = hookEMdCreate_prevSkipInfo;
    hookEMdCreate_skipActor = nullptr;
    hookEMdCreate_prevSkipInfo = 0;
}

HookAction hookPreOrderMapToolEvent(ModContext*, void* args, void*, void*) {
    auto eventId = mods::arg<u8>(args, 1);

    // skip starting the master sword cutscene so we can handle the checks manually
    if (daAlink_c::checkStageName("F_SP117") && eventId == 2) {
        // Set the necessary flags to de-spawn the MS and set the save file event flag.
        dComIfGs_onTmpBit(0x820);
        dComIfGs_onEventBit(0x2120);
        return HOOK_SKIP_ORIGINAL;
    }

    return HOOK_CONTINUE;
}

HookAction hookPreMasterSwordExecute(ModContext*, void* args, void* retval, void*) {
    auto* i_this = mods::arg<daObjMasterSword_c*>(args, 0);

    (i_this->*i_this->mActionFunc[1])();
    dComIfG_Ccsp()->Set(&i_this->mCyl);

    i_this->mBtk.play();
    i_this->mBrk.play();

    if (dComIfGs_isTmpBit(dSv_event_tmp_flag_c::tempBitLabels[73])) {
        // Don't automatically give the master sword in randomizer
        fopAcM_delete(i_this);
    }

    *static_cast<int*>(retval) = 1;
    return HOOK_SKIP_ORIGINAL;
}

void hookPost_dScnName_c__changeGameScene(ModContext* ctx, void* args, void* retval, void* userdata) {
    if (!mDoRst::isReset() && !fopOvlpM_IsPeek()) {
        randomizer::session::registerStartingLocation();
    }
}

void hookPostTbox2Create(ModContext*, void* args, void* retval, void*) {
    if (*static_cast<int*>(retval) != 1) {
        return;
    }

    auto* i_this = mods::arg<daTbox2_c*>(args, 0);
    u8 tboxId = fopAcM_GetParamBit(i_this, 16, 8);
    if (tboxId == 0xFF || !dComIfGs_isTbox(tboxId)) {
        return;
    }
    // If the flag for this box is set, open it

    // Set the action for not allowing the player to open it
    i_this->init_actionWait();

    // Set the animation frame to open
    i_this->mpBck->setFrame(i_this->mpBck->getEndFrame());

    // Set collision to open
    if (i_this->mpBgW != NULL) {
        dComIfG_Bgsp().Release(i_this->mpBgW);
    }
    if (i_this->mBoxBgW != NULL) {
        dComIfG_Bgsp().Regist(i_this->mBoxBgW, i_this);
    }
}

HookAction hookPreTbox2SetGetDemoItem(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<daTbox2_c*>(args, 0);
    if (i_this->mReturnRupee) {
        return HOOK_CONTINUE;
    }

    u8 tboxId = fopAcM_GetParamBit(i_this, 16, 8);
    if (tboxId != 0xFF) {
        dComIfGs_onTbox(tboxId);
    }

    return HOOK_CONTINUE;
}

u8 hookGameoverCreate_prevSlot18 = dItemNo_NONE_e;
bool hookGameoverCreate_restoreSlot18 = false;
HookAction hookPreGameoverCreate(ModContext*, void*, void*, void*) {
    if (dMeter2Info_getGameOverType() == 1 && strcmp(dComIfGp_getLastPlayStageName(), "D_MN10A") == 0) {
        // In rando, we only want to clear the Ooccoo slot if Ooccoo is in it.
        // so continue with normal function if Ooccoo is in slot.
        u8 ooccooSlot = dComIfGs_getItem(SLOT_18, false);
        if (ooccooSlot == dItemNo_Randomizer_DUNGEON_EXIT_e ||
            ooccooSlot == dItemNo_Randomizer_DUNGEON_EXIT_2_e)
        {
            return HOOK_CONTINUE;
        }

        hookGameoverCreate_prevSlot18 = ooccooSlot;
        hookGameoverCreate_restoreSlot18 = true;
    }

    return HOOK_CONTINUE;
}

void hookPostGameoverCreate(ModContext*, void*, void*, void*) {
    if (hookGameoverCreate_restoreSlot18) {
        dComIfGs_setItem(SLOT_18, hookGameoverCreate_prevSlot18);
        hookGameoverCreate_restoreSlot18 = false;
    }
}

inline void createPalaceSolsRewardItem(daObjSwBallC_c* i_this) {
    cXyz scale{1.0f, 1.0f, 1.0f};
    cXyz position{250.0f, -200.0f, 11000.0f};
    initCreatePlayerItem(
        dItemNo_Randomizer_WOOD_STICK_e,
        0x81,
        &position,
        fopAcM_GetRoomNo(i_this),
        &i_this->shape_angle,
        &scale);
}

void hookPostSwBallCreate(ModContext*, void* args, void* retval, void*) {
    auto* i_this = mods::arg<daObjSwBallC_c*>(args, 0);
    if (*static_cast<int*>(retval) != 1) {
        return;
    }

    if (fopAcM_isSwitch(i_this, 0x3d) && fopAcM_isSwitch(i_this, 0x3e)) {
        createPalaceSolsRewardItem(i_this);
    }
}

HookAction hookPreSwBallActionWait(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<daObjSwBallC_c*>(args, 0);
    if (!fopAcM_isSwitch(i_this, 0x3d) ||
        !fopAcM_isSwitch(i_this, 0x3e))
    {
        return HOOK_CONTINUE;
    }

    // Don't play the cutscene in rando, just spawn in the item for
    // Palace of Twilight Collect Both Sols

    dComIfGs_onTbox(10);
    dComIfGs_onTbox(11);

    i_this->calcLightBallScale();
    i_this->field_0x574->setPlaySpeed(1.0f);
    if (i_this->field_0x574->play() != 0 && !fopAcM_isSwitch(i_this, 0x3f)) {
        fopAcM_onSwitch(i_this, 0x3f);
        fopAcM_onSwitch(i_this, 0x27);
        createPalaceSolsRewardItem(i_this);
    }

    return HOOK_SKIP_ORIGINAL;
}

void hookPostDitemExecute(ModContext*, void* args, void* retval, void*) {
    auto* i_this = mods::arg<daDitem_c*>(args, 0);

    // Certain items use field models that are too big to fit in link's hands so we want to scale them down to fit.
    switch (i_this->m_itemNo) {
    case dItemNo_Randomizer_MIRROR_PIECE_1_e:
    case dItemNo_Randomizer_MIRROR_PIECE_2_e:
    case dItemNo_Randomizer_MIRROR_PIECE_3_e:
    case dItemNo_Randomizer_MIRROR_PIECE_4_e:
    {
        i_this->scale.x = 0.05f;
        break;
    }
    case dItemNo_Randomizer_MASTER_SWORD_e:
    case dItemNo_Randomizer_LIGHT_SWORD_e:
    {
        i_this->scale.x = 0.001f;
        break;
    }
    }
}

// pretty ugly way of handling this, but oh well. basically, we reconstruct the actionTable
// and use it to check that we're in the correct action before proceeding. then store info
// about what level we're on, and use it in the post-hook to undo the flag that's normally set.
daObjBossWarp_c* hookBossWarpDemoProc_patchActor = nullptr;
int hookBossWarpDemoProc_nowLevel = -1;
HookAction hookPreBossWarpDemoProc(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<daObjBossWarp_c*>(args, 0);

    hookBossWarpDemoProc_patchActor = nullptr;
    hookBossWarpDemoProc_nowLevel = -1;

    static const char* const actionTable[15] = {
        "WAIT",
        "APPEAR",
        "DISAPPEAR",
        "SCENE_CHG",
        "STONE_FALL",
        "STONE_MIDNA",
        "WALK_TARGET1",
        "APPEAR_END",
        "STONE_DELETE",
        "STONE_PUTAWAY",
        "WCHECK",
        "SETPOS",
        "SCALING",
        "STONE_SCALE",
        "HEART_MOVE",
    };

    bool isRewardAction =
        dComIfGp_evmng_getIsAddvance(i_this->mStaffId)
        && dComIfGp_evmng_getMyActIdx(i_this->mStaffId, actionTable, 15, 0, 0) == 4;
    if (!isRewardAction) {
        return HOOK_CONTINUE;
    }

    // this was a static function in the original TU, so reconstructing it for use here
    auto getNowLevel = []() {
        static const char* const stages[9] = {
            "D_MN05A",
            "D_MN04A",
            "D_MN01A",
            "D_MN10A",
            "D_MN11A",
            "D_MN06A",
            "D_MN07A",
            "D_MN08A",
            "D_MN01A",
        };

        for (int i = 0; i < 9; i++) {
            if (std::strcmp(dComIfGp_getStartStageName(), stages[i]) == 0) {
                return i;
            }
        }

        return -1;
    };

    hookBossWarpDemoProc_patchActor = i_this;
    hookBossWarpDemoProc_nowLevel = getNowLevel();
    return HOOK_CONTINUE;
}

void hookPostBossWarpDemoProc(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<daObjBossWarp_c*>(args, 0);
    if (hookBossWarpDemoProc_patchActor != i_this) {
        return;
    }

    int level = hookBossWarpDemoProc_nowLevel;
    hookBossWarpDemoProc_patchActor = nullptr;
    hookBossWarpDemoProc_nowLevel = -1;

    // undo the flag that was set in the original function
    switch (level) {
    case 0:
        dComIfGs_offCollectCrystal(0);
        break;
    case 1:
        dComIfGs_offCollectCrystal(1);
        break;
    case 2:
        dComIfGs_offCollectCrystal(2);
        break;
    case 4:
        dComIfGs_offCollectMirror(1);
        break;
    case 5:
        dComIfGs_offCollectMirror(2);
        break;
    case 6:
        dComIfGs_offCollectMirror(3);
        break;
    }
}

bool hookUkiCatch_isSkipSetBottle = false;
HookAction hookPreUkiCatch(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<dmg_rod_class*>(args, 0);
    hookUkiCatch_isSkipSetBottle = false;

    fopAc_ac_c* mgfish_a = fopAcM_SearchByID(i_this->mg_fish_id);
    mg_fish_class* mgfish = (mg_fish_class*)mgfish_a;
    if (mgfish == nullptr) {
        return HOOK_CONTINUE;
    }

    // replicating the bottle catch check here to remove rng check by overriding whatever catch
    // you have if you meet the bottle catch conditions
    if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[468]) &&
        strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0)
    {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cXyz bin_pos(6800.0f, 30.0f, -270.0f);
        bin_pos -= player->current.pos;
        if (bin_pos.abs() < 2500.0f) {
            s16 angle = player->shape_angle.y - cM_atan2s(bin_pos.x, bin_pos.z);
            if (angle < 0x4000 && angle > -0x4000) {
                mgfish->mCaughtType = MG_CATCH_BIN;
            }
        }
    }

    // if catching a bottle, set a flag to skip giving an empty bottle so that the FLW patch
    // can handle the check
    if (mgfish->mCaughtType == MG_CATCH_BIN) {
        hookUkiCatch_isSkipSetBottle = true;
    }

    return HOOK_CONTINUE;
}

void hookPostUkiCatch(ModContext*, void*, void*, void*) {
    hookUkiCatch_isSkipSetBottle = false;
}

HookAction hookPreSetEmptyBottle(ModContext*, void*, void*, void*) {
    // coming from uki_catch, skip giving bottle. let FLW patch handle it
    if (hookUkiCatch_isSkipSetBottle) {
        return HOOK_SKIP_ORIGINAL;
    }

    return HOOK_CONTINUE;
}

HookAction hookPreNpcZrCIsDelete(ModContext*, void* args, void* retval, void*) {
    auto* i_this = mods::arg<daNpc_zrC_c*>(args, 0);
    if (i_this->mType == 4
        || i_this->mType == 0
        || i_this->mType == 1
        || (i_this->mType == 2 && daNpcF_chkEvtBit(0x108))
        || i_this->mType == 3
        )
    {
        *static_cast<BOOL*>(retval) = FALSE;
        return HOOK_SKIP_ORIGINAL;
    }

    return HOOK_CONTINUE;
}

void hookPostObjZraRockCreate(ModContext*, void* args, void* retval, void*) {
    auto* i_this = mods::arg<daObjZraRock_c*>(args, 0);
    if (*static_cast<int*>(retval) != cPhs_ERROR_e) {
        return;
    }

    // if returned cPhs_ERROR_e, check if it's because the switch was set.
    if (dComIfGs_isSwitch((fopAcM_GetParam(i_this) >> 8) & 0xff, fopAcM_GetRoomNo(i_this))) {
        // Don't delete the rock when we're following rutela
        if (!dComIfGs_isEventBit(GOT_ZORA_ARMOR_FROM_RUTELA) && dComIfGs_isEventBit(ZORA_ESCORT_CLEARED)) {
            *static_cast<int*>(retval) = cPhs_COMPLEATE_e;
        }
    }
}

bool g_horseCallFirstBit = false;
u8 g_slot21Item = 0;
HookAction hookPreScnPlyPhase1(ModContext*, void*, void*, void*) {
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP104") && dComIfGp_getStartStageRoomNo() == 1 &&
        dComIfGp_getStartStagePoint() == 23 && dComIfGp_getStartStageLayer() == 12)
    {
        // Save item info for rando to restore after function runs
        g_horseCallFirstBit = dComIfGs_isItemFirstBit(dItemNo_HORSE_FLUTE_e);
        g_slot21Item = dComIfGs_getItem(SLOT_21, false);
    }

    return HOOK_CONTINUE;
}

void hookPostScnPlyPhase1(ModContext*, void*, void*, void*) {
    if (!strcmp(dComIfGp_getStartStageName(), "F_SP104") && dComIfGp_getStartStageRoomNo() == 1 &&
        dComIfGp_getStartStagePoint() == 23 && dComIfGp_getStartStageLayer() == 12)
    {
        // Restore item info
        if (!g_horseCallFirstBit) {
            dComIfGs_offItemFirstBit(dItemNo_HORSE_FLUTE_e);
        }
        dComIfGs_setItem(SLOT_21, g_slot21Item);
    }
}

J2DPicture* g_dpadIcon{};
void hookPostMenuRingTextScaleHIO(ModContext*, void* args, void*, void*) {
    auto menuRing = mods::arg<dMenu_Ring_c*>(args, 0);
    if (menuRing->mItemSlots[menuRing->mCurrentSlot] == 0x15) {
        // Draw d-pad icon to indicate switching between Ilia quest items
        if (getWarashibeItemCount() >= 2) {
            if (g_dpadIcon == nullptr) {
                g_dpadIcon = JKR_NEW J2DPicture((ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "font_51.bti"));
            }
            g_dpadIcon->setAlpha(menuRing->mAlphaRate * 255.0);
            g_dpadIcon->draw(menuRing->mCenterPosX + 330.f, menuRing->mCenterPosY + 194.f, 30.f, 30.f, false, false, false);
        }
    }
}

void hookPostMenuRingDestructor(ModContext*, void*, void*, void*) {
    if (g_dpadIcon != nullptr) {
        JKR_DELETE(g_dpadIcon);
        g_dpadIcon = nullptr;
    }
}

void hookPostMenuRingSetActiveCursor(ModContext*, void* args, void*, void*) {
    auto menuRing = mods::arg<dMenu_Ring_c*>(args, 0);

    // Copy if else chain from inside the function but without any of the effects
    u8 item = dComIfGs_getItem(menuRing->mItemSlots[menuRing->mCurrentSlot], false);
    if (menuRing->mStatus == dMenu_Ring_c::STATUS_WAIT &&
        menuRing->mOldStatus != dMenu_Ring_c::STATUS_EXPLAIN_FORCE &&
        menuRing->mOldStatus != dMenu_Ring_c::STATUS_EXPLAIN &&
        menuRing->mpItemExplain->getStatus() == 0)
    {
        if (mDoCPd_c::getTrigR(PAD_1) && !menuRing->mPlayerIsWolf && item != dItemNo_NONE_e) {

        } else if (mDoCPd_c::getTrigX(PAD_1) && !menuRing->mPlayerIsWolf && item != dItemNo_NONE_e) {

        } else if (mDoCPd_c::getTrigY(PAD_1) && !menuRing->mPlayerIsWolf && item != dItemNo_NONE_e) {

        } else if (mDoCPd_c::getTrigX(PAD_1) || mDoCPd_c::getTrigY(PAD_1)) {

        }
        // And add our conditional onto the end
        else if (menuRing->mItemSlots[menuRing->mCurrentSlot] == 0x15) {
            // Allow switching quest items if dpad right is pressed
            if (mDoCPd_c::getTrigRight(PAD_1)) {
                setNextWarashibeItem();
                // Update slot image
                for (int i = 0; i < menuRing->mTotalItemTexToAlloc; i++) {
                    if (menuRing->mItemSlots[i] == 0x15) {
                        u8 questItem = dComIfGs_getItem(menuRing->mItemSlots[i], false);

                        s32 i_textureNum =
                            dMeter2Info_readItemTexture(questItem, menuRing->mpItemBuf[i][0], NULL, menuRing->mpItemBuf[i][1], NULL,
                                                        menuRing->mpItemBuf[i][2], NULL, NULL, NULL, -1);
                        for (int k = 0; k < i_textureNum; k++) {
                            // Delete old texture so we aren't leaking memory
                            if (menuRing->mpItemTex[i][k] != NULL) {
                                JKR_DELETE(menuRing->mpItemTex[i][k]);
                            }

                            menuRing->mpItemTex[i][k] = JKR_NEW J2DPicture(menuRing->mpItemBuf[i][k]);
                            menuRing->mpItemTex[i][k]->setBasePosition(J2DBasePosition_4);
                        }
                        dMeter2Info_setItemColor(questItem, menuRing->mpItemTex[i][0], menuRing->mpItemTex[i][1], menuRing->mpItemTex[i][2], NULL);
                        u8 texScale = dItem_data::getTexScale(questItem);
                        f32 fVar1 = (texScale / 100.0f);
                        f32 fVar2 = (menuRing->mpItemBuf[i][0]->width / 48.0f);
                        fVar1 = fVar2 * fVar1;
                        menuRing->mItemSlotParam1[i] = fVar1;
                        menuRing->mItemSlotParam2[i] = (menuRing->mpItemBuf[i][0]->height / 48.0f * (texScale / 100.0f));
                    }
                }
            }
        }
    }
}

void hookPostMenuRingGetItemMaxNum(ModContext*, void* args, void* retval, void*) {
    auto i_slotNo = mods::arg<u8>(args, 1);
    auto ret = static_cast<u8*>(retval);

    u8 item = dComIfGs_getItem(i_slotNo, false);
    switch (item) {
    case dItemNo_Randomizer_ANCIENT_DOCUMENT_e:
    case dItemNo_Randomizer_AIR_LETTER_e:
    case dItemNo_Randomizer_ANCIENT_DOCUMENT2_e:
        *ret = 6;
        break;
    default:
        break;
    }
}

void hookPostMenuRingGetItemNum(ModContext*, void* args, void* retval, void*) {
    auto i_slotNo = mods::arg<u8>(args, 1);
    auto ret = static_cast<u8*>(retval);

    u8 item = dComIfGs_getItem(i_slotNo, false);
    switch (item) {
    case dItemNo_Randomizer_ANCIENT_DOCUMENT_e:
    case dItemNo_Randomizer_AIR_LETTER_e:
    case dItemNo_Randomizer_ANCIENT_DOCUMENT2_e:
        *ret = getAncientDocumentNum();
        break;
    default:
        break;
    }
}

void hookPostItemCreateInit(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<daItem_c*>(args, 0);

    switch (i_this->m_itemNo) {
    case dItemNo_Randomizer_KAKERA_HEART_e:
    case dItemNo_Randomizer_UTAWA_HEART_e:
    case dItemNo_Randomizer_ARROW_1_e:
    case dItemNo_Randomizer_ARROW_10_e:
    case dItemNo_Randomizer_ARROW_20_e:
    case dItemNo_Randomizer_ARROW_30_e:
    case dItemNo_Randomizer_GREEN_RUPEE_e:
    case dItemNo_Randomizer_BLUE_RUPEE_e:
    case dItemNo_Randomizer_YELLOW_RUPEE_e:
    case dItemNo_Randomizer_RED_RUPEE_e:
    case dItemNo_Randomizer_PURPLE_RUPEE_e:
    case dItemNo_Randomizer_ORANGE_RUPEE_e:
    case dItemNo_Randomizer_SILVER_RUPEE_e:
    case dItemNo_Randomizer_HEART_e:
        i_this->mItemScale.setall(1.0f);
        break;
    case dItemNo_Randomizer_BOW_e:
        i_this->mItemScale.setall(1.5f);
        break;
    case dItemNo_Randomizer_MASTER_SWORD_e:
    case dItemNo_Randomizer_LIGHT_SWORD_e:
    case dItemNo_Randomizer_MIRROR_PIECE_1_e:
    case dItemNo_Randomizer_MIRROR_PIECE_2_e:
    case dItemNo_Randomizer_MIRROR_PIECE_3_e:
    case dItemNo_Randomizer_MIRROR_PIECE_4_e:
        i_this->mItemScale.setall(0.7f);
        break;
    default:
        i_this->mItemScale.setall(2.0f);
        break;
    }
}

HookAction hookPreItemActionForBoomerang(ModContext*, void* args, void* retval, void*) {
    *static_cast<int*>(retval) = 1;
    return HOOK_SKIP_ORIGINAL;
}

HookAction hookPreItemItemGetNextExecute(ModContext*, void* args, void* retval, void*) {
    auto* i_this = mods::arg<daItem_c*>(args, 0);

    if (!i_this->checkFlag(daItem_c::FLAG_DELETE_ITEM_e) && !i_this->checkFlag(daItem_c::FLAG_INIT_GET_ITEM_e)) {
        i_this->setFlag(daItem_c::FLAG_INIT_GET_ITEM_e);
        BOOL haveItem = false;

        switch (i_this->m_itemNo) {
        case dItemNo_HEART_e:
        case dItemNo_GREEN_RUPEE_e:
        case dItemNo_ARROW_10_e:
        case dItemNo_ARROW_20_e:
        case dItemNo_ARROW_30_e:
        case dItemNo_ARROW_1_e:
            i_this->procInitSimpleGetDemo();
            i_this->itemGet();
            break;
        case dItemNo_BLUE_RUPEE_e:
        case dItemNo_YELLOW_RUPEE_e:
        case dItemNo_RED_RUPEE_e:
        case dItemNo_PURPLE_RUPEE_e:
        case dItemNo_ORANGE_RUPEE_e:
        case dItemNo_SILVER_RUPEE_e:
        case dItemNo_PACHINKO_SHOT_e:
            if (daPy_getPlayerActorClass()->checkCanoeRide() ||
                daPy_getPlayerActorClass()->checkHorseRide() ||
                daPy_getPlayerActorClass()->checkBoardRide()) // Check snowboarding for rando
            {
                if (checkItemGet(i_this->m_itemNo, 1)) {
                    haveItem = true;
                }
                i_this->procInitSimpleGetDemo();
                i_this->itemGet();

                if (!haveItem) {
                    dComIfGs_offItemFirstBit(i_this->m_itemNo);
                }
            } else if (!checkItemGet(i_this->m_itemNo, 1)) {
                i_this->procInitGetDemoEvent();
            } else {
                i_this->procInitSimpleGetDemo();
                i_this->itemGet();
            }
            break;
        default:
            if (i_this->mItemOverridden) {
                i_this->procInitGetDemoEvent();
                break;
            }
        }

        fopAcM_onItem(i_this, i_this->mItemBitNo);
        i_this->mCcCyl.SetTgType(0);
        i_this->mCcCyl.OffCoSPrmBit(1);
        i_this->mCcCyl.ClrTgHit();
        i_this->mCcCyl.ClrCoHit();
    }

    return HOOK_SKIP_ORIGINAL;
}

HookAction hookPreItemItemGet(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<daItem_c*>(args, 0);

    // we can safely skip the original here as long as it's not a progressive item
    switch (i_this->m_itemNo) {
    // Play sound for heart pieces and containers as well
    case dItemNo_UTAWA_HEART_e:
    case dItemNo_KAKERA_HEART_e:
        mDoAud_seStart(Z2SE_HEART_PIECE_GET, NULL, 0, 0);
        execItemGet(i_this->m_itemNo, i_this->mItemGiveTag, i_this);
        return HOOK_SKIP_ORIGINAL;
    case dItemNo_BOOMERANG_e:
        mDoAud_seStart(Z2SE_CONSUMP_ITEM_GET, NULL, 0, 0);
        execItemGet(i_this->m_itemNo, i_this->mItemGiveTag, i_this);
        return HOOK_SKIP_ORIGINAL;
    }

    return HOOK_CONTINUE;
}

HookAction hookPreObjLifeSetEffect(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<daObjLife_c*>(args, 0);

    // In randomizer, we don't want rupees or poe souls to sparkle. They are bright enough.
    switch (i_this->m_itemNo) {
    case dItemNo_Randomizer_GREEN_RUPEE_e:
    case dItemNo_Randomizer_BLUE_RUPEE_e:
    case dItemNo_Randomizer_RED_RUPEE_e:
    case dItemNo_Randomizer_YELLOW_RUPEE_e:
    case dItemNo_Randomizer_LINKS_SAVINGS_e:
    case dItemNo_Randomizer_PURPLE_RUPEE_e:
    case dItemNo_Randomizer_ORANGE_RUPEE_e:
    case dItemNo_Randomizer_SILVER_RUPEE_e:
    case dItemNo_Randomizer_POU_SPIRIT_e:
        return HOOK_SKIP_ORIGINAL;
    }

    return HOOK_CONTINUE;
}

HookAction hookPreObjLifeCreate(ModContext*, void* args, void* retval, void*) {
    auto* i_this = mods::arg<daObjLife_c*>(args, 0);
    // it's safe to do this here since the actor init condition will be set, preventing the
    // original ct call from running again
    if (!fopAcM_CheckCondition(i_this, fopAcCnd_INIT_e)) {
        fopAcM_ct_placement(i_this, daObjLife_c);
        fopAcM_OnCondition(i_this, fopAcCnd_INIT_e);
    }

    if (!i_this->mIsPrmsInit) {
        u32 params = fopAcM_GetParam(i_this);
        u8 itemId = params & 0xFF;
        u8 roomNo = fopAcM_GetRoomNo(i_this);

        if (itemId == dItemNo_Randomizer_ENDING_BLOW_e) {
            auto goldenWolfFlags = getCurrentGoldenWolfFlags(roomNo);
            // Don't spawn this item if we haven't howled at the howling stone, or if we've already
            // obtained the item
            if ((goldenWolfFlags.howledAtStoneFlag != 0xFFFF && !dComIfGs_isEventBit(goldenWolfFlags.howledAtStoneFlag)) ||
                dComIfGs_isEventBit(goldenWolfFlags.obtainedItemFlag))
            {
                *static_cast<int*>(retval) = cPhs_ERROR_e;
                return HOOK_SKIP_ORIGINAL;
            }

            // Store the map marker flag and obtained item flags to turn off/on later if
            // the player collects the item
            i_this->home.angle.z = goldenWolfFlags.mapMarkerFlag;
            i_this->home.angle.x = static_cast<s16>(goldenWolfFlags.obtainedItemFlag);
        }

        // Also adjust the height of the object depending on the item
        switch (itemId) {
        case dItemNo_Randomizer_MASTER_SWORD_e:
        case dItemNo_Randomizer_LIGHT_SWORD_e:
        case dItemNo_Randomizer_WOOD_SHIELD_e:
        case dItemNo_Randomizer_HYLIA_SHIELD_e:
        case dItemNo_Randomizer_SHIELD_e:
        case dItemNo_Randomizer_SPINNER_e:
        {
            i_this->current.pos.y += 30.f;
            break;
        }
        case dItemNo_Randomizer_WOOD_STICK_e:
        {
            i_this->current.pos.y += 60.f;
            break;
        }
        case dItemNo_Randomizer_SWORD_e:
        case dItemNo_Randomizer_MIRROR_PIECE_1_e:
        case dItemNo_Randomizer_MIRROR_PIECE_2_e:
        case dItemNo_Randomizer_MIRROR_PIECE_3_e:
        case dItemNo_Randomizer_MIRROR_PIECE_4_e:
        case dItemNo_Randomizer_FUSED_SHADOW_1_e:
        case dItemNo_Randomizer_FUSED_SHADOW_2_e:
        case dItemNo_Randomizer_FUSED_SHADOW_3_e:
        case dItemNo_Randomizer_COPY_ROD_e:
        case dItemNo_Randomizer_COPY_ROD_2_e:
        {
            i_this->current.pos.y += 50.f;
            break;
        }

        case dItemNo_Randomizer_BOW_e:
        {
            i_this->current.pos.y += 55.f;
            break;
        }
        case dItemNo_Randomizer_BOOMERANG_e:
        case dItemNo_Randomizer_FISHING_ROD_1_e:
        case dItemNo_Randomizer_ARROW_LV2_e:
        case dItemNo_Randomizer_ARROW_LV3_e:
        {
            i_this->current.pos.y += 40.f;
            break;
        }
        case dItemNo_Randomizer_FOREST_SMALL_KEY_e:
        case dItemNo_Randomizer_MINES_SMALL_KEY_e:
        case dItemNo_Randomizer_LAKEBED_SMALL_KEY_e:
        case dItemNo_Randomizer_ARBITERS_SMALL_KEY_e:
        case dItemNo_Randomizer_SNOWPEAK_SMALL_KEY_e:
        case dItemNo_Randomizer_TEMPLE_OF_TIME_SMALL_KEY_e:
        case dItemNo_Randomizer_CITY_SMALL_KEY_e:
        case dItemNo_Randomizer_PALACE_SMALL_KEY_e:
        case dItemNo_Randomizer_HYRULE_SMALL_KEY_e:
        case dItemNo_Randomizer_FOREST_BOSS_KEY_e:
        case dItemNo_Randomizer_LAKEBED_BOSS_KEY_e:
        case dItemNo_Randomizer_ARBITERS_BOSS_KEY_e:
        case dItemNo_Randomizer_TEMPLE_OF_TIME_BOSS_KEY_e:
        case dItemNo_Randomizer_CITY_BOSS_KEY_e:
        case dItemNo_Randomizer_PALACE_BOSS_KEY_e:
        case dItemNo_Randomizer_HYRULE_BOSS_KEY_e:
        case dItemNo_Randomizer_SMALL_KEY2_e:
        case dItemNo_Randomizer_LV5_BOSS_KEY_e:
        case dItemNo_Randomizer_CAMP_SMALL_KEY_e:
        case dItemNo_Randomizer_BOSSRIDER_KEY_e:
        case dItemNo_Randomizer_PACHINKO_e:
        case dItemNo_Randomizer_BOMB_BAG_LV2_e:
        case dItemNo_Randomizer_BOMB_BAG_LV1_e:
        case dItemNo_Randomizer_BOMB_IN_BAG_e:
        case dItemNo_Randomizer_NORMAL_BOMB_e:
        case dItemNo_Randomizer_POU_SPIRIT_e:
        {
            i_this->current.pos.y += 20.f;
            break;
        }
        case dItemNo_Randomizer_ARMOR_e:
        {
            i_this->current.pos.y += 25.f;
            break;
        }
        default:
            break;
        }
    }

    return HOOK_CONTINUE;
}

void hookPostObjLifeActionGetDemo(ModContext*, void* args, void*, void*) {
    auto* i_this = mods::arg<daObjLife_c*>(args, 0);

    // In randomizer, turn off the map marker flag for this golden wolf replacement item
    // if we're collecting it. We store the map marker flag in unused home.angle.z
    // Also set the flag for having collected this golden wolf item, stored in home.angle.x
    if (static_cast<u16>(i_this->home.angle.z) != 0xFFFF) {
        dComIfGs_offSwitch(static_cast<u16>(i_this->home.angle.z), fopAcM_GetRoomNo(i_this));
    }
    if (static_cast<u16>(i_this->home.angle.x) != 0xFFFF) {
        dComIfGs_onEventBit(static_cast<u16>(i_this->home.angle.x));
    }
}

HookAction hookPreObjLifeCalcScale(ModContext*, void* args, void* retval, void*) {
    auto* i_this = mods::arg<daObjLife_c*>(args, 0);

    // Change scale for certain items
    f32 newScale = 1.0f;
    switch (i_this->m_itemNo) {
    case dItemNo_Randomizer_KAKERA_HEART_e:
    case dItemNo_Randomizer_UTAWA_HEART_e:
    case dItemNo_Randomizer_ARROW_10_e:
    case dItemNo_Randomizer_ARROW_20_e:
    case dItemNo_Randomizer_ARROW_30_e:
        newScale = 1.0f;
        break;
    case dItemNo_Randomizer_BOW_e:
        newScale = 1.5f;
        break;
    case dItemNo_Randomizer_MASTER_SWORD_e:
    case dItemNo_Randomizer_LIGHT_SWORD_e:
    case dItemNo_Randomizer_MIRROR_PIECE_1_e:
    case dItemNo_Randomizer_MIRROR_PIECE_2_e:
    case dItemNo_Randomizer_MIRROR_PIECE_3_e:
    case dItemNo_Randomizer_MIRROR_PIECE_4_e:
        newScale = 0.7f;
        break;
    default:
        newScale = 2.0f;
    }

    cLib_chaseF(&i_this->field_0x954, 1.0f, 0.2f);
    if (i_this->field_0x954 == newScale) {
        cLib_chaseF(&i_this->field_0x94c, 0.0f, 0.05f);
        i_this->field_0x950 = i_this->field_0x94c * cM_ssin(i_this->field_0x95e * 3000);

        if (i_this->field_0x95e < 1000) {
            i_this->field_0x95e++;
        }
    } else {
        i_this->field_0x950 = 0.0f;
    }

    i_this->scale.setall(i_this->field_0x950 + i_this->field_0x954);
    return HOOK_SKIP_ORIGINAL;
}

void hookPostGetCollectSmell(ModContext*, void*, void* retval, void*) {
    auto currentSmell = static_cast<u8*>(retval);
    if (getStageID() == Snowpeak && dComIfGs_isEventBit(GOT_REEKFISH_SCENT)) {
        *currentSmell = dItemNo_Randomizer_SMELL_FISH_e;
    }
}

// Can't think of a better way to do this for now other than replacing the whole function,
// but we can try to revisit later
void hookReplaceEvtControlSkipper(ModContext*, void* args, void* retval, void*) {
    auto evtControl = mods::arg<dEvt_control_c*>(args, 0);

    bool doSkip = false;
    bool canSkip = false;

    evtControl->offFlag2(8);

    if (evtControl->mEventStatus == 1) {
        if (evtControl->mSkipFunc != NULL) {
            canSkip = true;
        }

        bool is_trig_skipbtn = mDoCPd_c::getTrigStart(PAD_1);
        // Automatically skip cutscenes in rando if Skip Major Cutscenes is on
        if (is_trig_skipbtn || (canSkip &&
             randomizer_GetContext().mSettings[RandomizerContext::SKIP_MAJOR_CUTSCENES] == RandomizerContext::ON)) {
            if (evtControl->mSkipTimer > 0) {
                evtControl->mSkipTimer = -1;

                if (canSkip && evtControl->mIsSkipFade) {
                    mDoGph_gInf_c::fadeOut(0.1f);
                }
            } else if (evtControl->mSkipTimer == 0) {
                evtControl->mSkipTimer = 1;
            }
        }

        if (evtControl->mSkipTimer > 0) {
            if (canSkip) {
                dComIfGp_setSButtonStatusForce(0x43, 1);
            } else {
                dComIfGp_setSButtonStatusForce(0x4D, 1);
            }

            if (evtControl->mSkipTimer++ > 45) {
                evtControl->mSkipTimer = 0;
            }
        } else if (evtControl->mSkipTimer != 0) {
            if (canSkip && evtControl->mIsSkipFade) {
                if (evtControl->mSkipTimer-- < -20) {
                    doSkip = true;
                    evtControl->mSkipTimer = 0;
                }
            } else {
                if (canSkip) {
                    doSkip = true;
                }
                evtControl->mSkipTimer = 0;
            }
        }

        if (doSkip) {
            dMsgObject_onKillMessageFlag();

            fopAc_ac_c* skipActor = evtControl->convPId(evtControl->mSkipActorId);
            if (skipActor == NULL) {
                OS_REPORT("\x1b[31m%06d: event: Skip ordered actor DEAD!! (%d) \n\x1b[m", g_Counter.mCounter0, mSkipActorId);
                skipActor = dComIfGp_getPlayer(0);
            }

            int skipRet = evtControl->mSkipFunc(skipActor, evtControl->mSkipParameter);
            evtControl->onFlag2(8);

            if (skipRet != 0) {
                evtControl->mSkipFunc = NULL;

                if (skipRet == 2) {
                    evtControl->onFlag2(1);
                } else {
                    evtControl->onFlag2(2);
                }
            }
        }
    }

    *static_cast<bool*>(retval) = doSkip;
}


}

ModResult initialize() {
#define ADD_HOOK_PRE(originalFn, hookFn)                             \
    if (mods::hook::add_pre<originalFn>(hookFn) != MOD_OK) {         \
        mods::log::error("Failed to add pre-hook for " #originalFn); \
        return MOD_ERROR;                                            \
    }

#define ADD_HOOK_POST(originalFn, hookFn)                             \
    if (mods::hook::add_post<originalFn>(hookFn) != MOD_OK) {         \
        mods::log::error("Failed to add post-hook for " #originalFn); \
        return MOD_ERROR;                                             \
    }

#define ADD_HOOK_REPLACE(originalFn, hookFn)                              \
    if (mods::hook::replace<originalFn>(hookFn) != MOD_OK) {             \
        mods::log::error("Failed to add replace-hook for " #originalFn); \
        return MOD_ERROR;                                                \
    }

    ADD_HOOK_PRE(dFile_select_c__selectDataNameMove, hookPreSelectDataNameMove);
    ADD_HOOK_PRE(dFile_select_c__dataSelect, hookPreDataSelect);

    ADD_HOOK_POST(dFile_info_c__setSaveData, hookPostSetSaveData);

    ADD_HOOK_PRE(dSv_event_c__isEventBit, hookPreIsEventBit);
    ADD_HOOK_PRE(dSv_event_c__onEventBit, hookPreOnEventBit);

    ADD_HOOK_PRE(dSv_memBit_c__isSwitch, hookPreMembitIsSwitch);
    ADD_HOOK_PRE(dSv_memBit_c__onSwitch, hookPreMembitOnSwitch);
    ADD_HOOK_PRE(dSv_memBit_c__onDungeonItem, hookPreOnDungeonItem);
    ADD_HOOK_PRE(dSv_memBit_c__offDungeonItem, hookPreOffDungeonItem);
    ADD_HOOK_PRE(dSv_memBit_c__isDungeonItem, hookPreIsDungeonItem);

    ADD_HOOK_PRE(dSv_player_status_b_c__isDarkClearLV, hookPreIsDarkClearLV);

    ADD_HOOK_PRE(dSv_player_item_c__checkEmptyBottle, hookPreCheckEmptyBottle);
    ADD_HOOK_POST(dSv_player_item_c__setLineUpItem, hookPostSetLineUpItem);

    ADD_HOOK_PRE(dSv_info_c__onSwitch, hookPreSaveInfoOnSwitch);

    ADD_HOOK_PRE(setNextStage, hookPreSetNextStage);

    ADD_HOOK_PRE(ObjGb_Create, hookPreObjGbCreate);

    ADD_HOOK_POST(readItemTexture, hookPostReadItemTexture);

    ADD_HOOK_PRE(dShopSystem_c__seq_decide_yes, hookPreShopSeqDecideYes);

    ADD_HOOK_PRE(dItemData_CheckFieldItemCreateHeap, hookPreCheckFieldItemCreateHeap);

    ADD_HOOK_POST(dEvt_control_c__talkEnd, hookPostTalkEnd);

    ADD_HOOK_PRE(dComIfG_play_c__getLayerNo_common_common, hookPreGetLayerNo);

    ADD_HOOK_PRE(dItem_getItemFunc, hookPreGetItemFunc);
    ADD_HOOK_PRE(dItem_checkItemGet, hookPreCheckItemGet);

    ADD_HOOK_PRE(onStageSwitch, hookPreOnStageSwitch);

    ADD_HOOK_PRE(daAlink_c__decideDoStatus, hookPreDecideDoStatus);
    ADD_HOOK_PRE(searchBouDoor, hookPreSearchBouDoor);
    ADD_HOOK_PRE(daAlink_c__checkGroundSpecialMode, hookPreCheckGroundSpecialMode);
    ADD_HOOK_POST(daAlink_c__setGetItemFace, hookPostSetGetItemFace);
    ADD_HOOK_PRE(daAlink_c__setGetSubBgm, hookPreSetGetSubBgm);
    ADD_HOOK_PRE(daAlink_c__procCoGetItem, hookPreProcCoGetItem);
    ADD_HOOK_PRE(daAlink_c__procCoWarpInit, hookPreProcCoWarpInit);
    ADD_HOOK_POST(daAlink_c__procCoWarpInit, hookPostProcCoWarpInit);

    ADD_HOOK_POST(bq_end, hookPostBqEnd);

    ADD_HOOK_PRE(daDoor20_c__checkOpenMsgDoor, hookPreCheckOpenMsgDoor);

    ADD_HOOK_POST(e_mk_demo_camera_end, hookPostEmkDemoCameraEnd);

    ADD_HOOK_POST(changeScene4Event, hookPostChangeScene4Event);
    ADD_HOOK_PRE(stage_playerInit, hookPreStagePlayerInit);

    ADD_HOOK_POST(Kytag08_Execute, hookPostKytag08Execute);

    ADD_HOOK_PRE(NpcT_chkEvtBit, hookPreNpcTChkEvtBit);
    ADD_HOOK_PRE(NpcF_chkEvtBit, hookPreNpcFChkEvtBit);
    ADD_HOOK_PRE(daNpcF_c__orderEvent, hookPreNpcFOrderEvent);

    ADD_HOOK_PRE(daNpc_Bans_c__isDelete, hookPreNpcBansIsDelete);

    ADD_HOOK_POST(daNpc_Fairy_c__AppearDemoCall, hookPostFairyAppearDemoCall);

    ADD_HOOK_PRE(daNpcShad_c__wait_type1, hookPreNpcShadWaitType1);
    ADD_HOOK_POST(daNpcShad_c__wait_type1, hookPostNpcShadWaitType1);

    ADD_HOOK_PRE(daNpc_Yelia_c__cutTakeWoodStatue, hookPreNpcYeliaCutTakeWoodStatue);
    ADD_HOOK_POST(daNpc_Yelia_c__cutTakeWoodStatue, hookPostNpcYeliaCutTakeWoodStatue);
    ADD_HOOK_PRE(dSv_player_item_c__setWarashibeItem, hookPreSetWarashibeItem);

    ADD_HOOK_POST(daNpc_ykM_c__isDelete, hookPostYkMIsDelete);
    ADD_HOOK_POST(daNpc_ykW_c__isDelete, hookPostYkWIsDelete);

    ADD_HOOK_PRE(daE_MD_c__create, hookPreEMdCreate);
    ADD_HOOK_POST(daE_MD_c__create, hookPostEMdCreate);

    ADD_HOOK_PRE(orderMapToolEvent, hookPreOrderMapToolEvent);
    ADD_HOOK_PRE(daObjMasterSword_c__execute, hookPreMasterSwordExecute);

    ADD_HOOK_POST(dScnName_c__changeGameScene, hookPost_dScnName_c__changeGameScene);

    ADD_HOOK_POST(daNpc_zrZ_c__isDelete, hookPostNpcZrzIsDelete);

    ADD_HOOK_POST(daTbox2_c__Create, hookPostTbox2Create);
    ADD_HOOK_PRE(daTbox2_c__setGetDemoItem, hookPreTbox2SetGetDemoItem);

    ADD_HOOK_PRE(dGameover_c___create, hookPreGameoverCreate);
    ADD_HOOK_POST(dGameover_c___create, hookPostGameoverCreate);

    ADD_HOOK_POST(daObjSwBallC_c__Create, hookPostSwBallCreate);
    ADD_HOOK_PRE(daObjSwBallC_c__actionWait, hookPreSwBallActionWait);

    ADD_HOOK_POST(daDitem_c__execute, hookPostDitemExecute);

    ADD_HOOK_PRE(daObjBossWarp_c__demoProc, hookPreBossWarpDemoProc);
    ADD_HOOK_POST(daObjBossWarp_c__demoProc, hookPostBossWarpDemoProc);

    ADD_HOOK_PRE(mgRod_lure_heart, hookPreLureHeart);
    ADD_HOOK_POST(mgRod_lure_heart, hookPostLureHeart);

    ADD_HOOK_PRE(mgRod_uki_catch, hookPreUkiCatch);
    ADD_HOOK_POST(mgRod_uki_catch, hookPostUkiCatch);

    ADD_HOOK_PRE(dSv_player_item_c__setEmptyBottle, hookPreSetEmptyBottle);

    ADD_HOOK_PRE(daNpc_zrC_c__isDelete, hookPreNpcZrCIsDelete);

    ADD_HOOK_POST(daObjZraRock_c__create, hookPostObjZraRockCreate);

    // ADD_HOOK_PRE(phase_1__dScnPly_c, hookPreScnPlyPhase1);
    // ADD_HOOK_POST(phase_1__dScnPly_c, hookPostScnPlyPhase1);

    ADD_HOOK_POST(dMenu_Ring_c__textScaleHIO, hookPostMenuRingTextScaleHIO);
    ADD_HOOK_POST(dMenu_Ring_c__destructor, hookPostMenuRingDestructor);
    ADD_HOOK_POST(dMenu_Ring_c__setActiveCursor, hookPostMenuRingSetActiveCursor);
    ADD_HOOK_POST(dMenu_Ring_c__getItemMaxNum, hookPostMenuRingGetItemMaxNum);
    ADD_HOOK_POST(dMenu_Ring_c__getItemNum, hookPostMenuRingGetItemNum);

    ADD_HOOK_POST(daItem_c__CreateInit, hookPostItemCreateInit);
    ADD_HOOK_PRE(daItem_c__itemActionForBoomerang, hookPreItemActionForBoomerang)
    ADD_HOOK_PRE(daItem_c__itemGetNextExecute, hookPreItemItemGetNextExecute);
    ADD_HOOK_PRE(daItem_c__itemGet, hookPreItemItemGet);

    ADD_HOOK_PRE(daObjLife_c__setEffect, hookPreObjLifeSetEffect);
    ADD_HOOK_PRE(daObjLife_c__create, hookPreObjLifeCreate);
    ADD_HOOK_POST(daObjLife_c__actionGetDemo, hookPostObjLifeActionGetDemo);
    ADD_HOOK_PRE(daObjLife_c__calcScale, hookPreObjLifeCalcScale);

    ADD_HOOK_POST(getCollectSmell, hookPostGetCollectSmell);

    ADD_HOOK_REPLACE(dEvt_control_c__skipper, hookReplaceEvtControlSkipper);

    return MOD_OK;
}

ModResult uninstall() {
    auto svc_hook = session::svc_mng.hook;

    mods::hook::uninstall<dFile_select_c__selectDataNameMove>(svc_hook);
    mods::hook::uninstall<dFile_select_c__dataSelect>(svc_hook);

    mods::hook::uninstall<dFile_info_c__setSaveData>(svc_hook);

    mods::hook::uninstall<dSv_event_c__isEventBit>(svc_hook);
    mods::hook::uninstall<dSv_event_c__onEventBit>(svc_hook);

    mods::hook::uninstall<dSv_memBit_c__isSwitch>(svc_hook);
    mods::hook::uninstall<dSv_memBit_c__onSwitch>(svc_hook);
    mods::hook::uninstall<dSv_memBit_c__onDungeonItem>(svc_hook);
    mods::hook::uninstall<dSv_memBit_c__offDungeonItem>(svc_hook);
    mods::hook::uninstall<dSv_memBit_c__isDungeonItem>(svc_hook);

    mods::hook::uninstall<dSv_player_status_b_c__isDarkClearLV>(svc_hook);

    mods::hook::uninstall<dSv_player_item_c__checkEmptyBottle>(svc_hook);
    mods::hook::uninstall<dSv_player_item_c__setLineUpItem>(svc_hook);

    mods::hook::uninstall<dSv_info_c__onSwitch>(svc_hook);
    mods::hook::uninstall<setNextStage>();

    mods::hook::uninstall<ObjGb_Create>(svc_hook);

    mods::hook::uninstall<readItemTexture>(svc_hook);

    mods::hook::uninstall<dShopSystem_c__seq_decide_yes>(svc_hook);

    mods::hook::uninstall<dItemData_CheckFieldItemCreateHeap>(svc_hook);

    mods::hook::uninstall<dEvt_control_c__talkEnd>(svc_hook);

    mods::hook::uninstall<dComIfG_play_c__getLayerNo_common_common>(svc_hook);

    mods::hook::uninstall<dItem_getItemFunc>(svc_hook);
    mods::hook::uninstall<dItem_checkItemGet>(svc_hook);

    mods::hook::uninstall<onStageSwitch>(svc_hook);

    mods::hook::uninstall<daAlink_c__decideDoStatus>(svc_hook);
    mods::hook::uninstall<searchBouDoor>(svc_hook);
    mods::hook::uninstall<daAlink_c__checkGroundSpecialMode>(svc_hook);
    mods::hook::uninstall<daAlink_c__setGetItemFace>(svc_hook);
    mods::hook::uninstall<daAlink_c__setGetSubBgm>(svc_hook);
    mods::hook::uninstall<daAlink_c__procCoGetItem>(svc_hook);
    mods::hook::uninstall<daAlink_c__procCoWarpInit>(svc_hook);

    mods::hook::uninstall<bq_end>(svc_hook);

    mods::hook::uninstall<daDoor20_c__checkOpenMsgDoor>(svc_hook);

    mods::hook::uninstall<e_mk_demo_camera_end>(svc_hook);

    mods::hook::uninstall<changeScene4Event>(svc_hook);
    mods::hook::uninstall<stage_playerInit>(svc_hook);

    mods::hook::uninstall<Kytag08_Execute>(svc_hook);

    mods::hook::uninstall<NpcT_chkEvtBit>(svc_hook);
    mods::hook::uninstall<NpcF_chkEvtBit>(svc_hook);
    mods::hook::uninstall<daNpcF_c__orderEvent>(svc_hook);

    mods::hook::uninstall<daNpc_Bans_c__isDelete>(svc_hook);

    mods::hook::uninstall<daNpc_Fairy_c__AppearDemoCall>(svc_hook);

    mods::hook::uninstall<daNpcShad_c__Create>(svc_hook);
    mods::hook::uninstall<daNpcShad_c__wait_type1>(svc_hook);

    mods::hook::uninstall<daNpc_Yelia_c__cutTakeWoodStatue>(svc_hook);
    mods::hook::uninstall<dSv_player_item_c__setWarashibeItem>(svc_hook);

    mods::hook::uninstall<daNpc_ykM_c__isDelete>(svc_hook);
    mods::hook::uninstall<daNpc_ykW_c__isDelete>(svc_hook);

    mods::hook::uninstall<daE_MD_c__create>(svc_hook);

    mods::hook::uninstall<orderMapToolEvent>(svc_hook);
    mods::hook::uninstall<daObjMasterSword_c__execute>(svc_hook);

    mods::hook::uninstall<dScnName_c__changeGameScene>();

    mods::hook::uninstall<daNpc_zrZ_c__isDelete>(svc_hook);

    mods::hook::uninstall<daTbox2_c__Create>(svc_hook);
    mods::hook::uninstall<daTbox2_c__setGetDemoItem>(svc_hook);

    mods::hook::uninstall<dGameover_c___create>(svc_hook);

    mods::hook::uninstall<daObjSwBallC_c__Create>(svc_hook);
    mods::hook::uninstall<daObjSwBallC_c__actionWait>(svc_hook);

    mods::hook::uninstall<daDitem_c__execute>(svc_hook);

    mods::hook::uninstall<daObjBossWarp_c__demoProc>(svc_hook);

    mods::hook::uninstall<mgRod_lure_heart>(svc_hook);
    mods::hook::uninstall<mgRod_uki_catch>(svc_hook);
    mods::hook::uninstall<dSv_player_item_c__setEmptyBottle>(svc_hook);

    mods::hook::uninstall<daNpc_zrC_c__isDelete>(svc_hook);

    mods::hook::uninstall<daObjZraRock_c__create>(svc_hook);

    //mods::hook::uninstall<phase_1__dScnPly_c>();

    mods::hook::uninstall<dMenu_Ring_c__textScaleHIO>(svc_hook);
    mods::hook::uninstall<dMenu_Ring_c__destructor>(svc_hook);
    mods::hook::uninstall<dMenu_Ring_c__setActiveCursor>(svc_hook);
    mods::hook::uninstall<dMenu_Ring_c__getItemMaxNum>(svc_hook);
    mods::hook::uninstall<dMenu_Ring_c__getItemNum>(svc_hook);

    mods::hook::uninstall<daItem_c__CreateInit>(svc_hook);
    mods::hook::uninstall<daItem_c__itemActionForBoomerang>(svc_hook);
    mods::hook::uninstall<daItem_c__itemGetNextExecute>(svc_hook);
    mods::hook::uninstall<daItem_c__itemGet>(svc_hook);

    mods::hook::uninstall<daObjLife_c__setEffect>(svc_hook);
    mods::hook::uninstall<daObjLife_c__create>(svc_hook);
    mods::hook::uninstall<daObjLife_c__actionGetDemo>(svc_hook);
    mods::hook::uninstall<daObjLife_c__calcScale>(svc_hook);

    mods::hook::uninstall<getCollectSmell>(svc_hook);

    mods::hook::uninstall<dEvt_control_c__skipper>(svc_hook);

    return MOD_OK;
}
}
