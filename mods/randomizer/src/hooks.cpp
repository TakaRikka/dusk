#include "hooks.hpp"
#include "session.hpp"
#include "randomizer_context.hpp"
#include "ui/rando_config.hpp"
#include "flags.h"
#include "stages.h"
#include "tools.h"
#include "item.hpp"
#include "item_ids.h"

#include <mods/svc/hook.hpp>
#include <mods/svc/log.hpp>

#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_b_bq.h"
#include "d/actor/d_a_door_shutter.h"
#include "d/actor/d_a_e_mk.h"
#include "d/actor/d_a_kytag08.h"
#include "d/actor/d_a_npc4.h"
#include "d/actor/d_a_npc_bans.h"
#include "d/actor/d_a_tag_kmsg.h"
#include "d/actor/d_a_npc_fairy.h"
#include "d/actor/d_a_npc_ykm.h"
#include "d/actor/d_a_npc_ykw.h"
#include "d/d_door_param2.h"
#include "d/d_file_sel_info.h"
#include "d/d_file_select.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "d/d_save.h"
#include "d/d_shop_system.h"

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

/*DEFINE_HOOK_SYMBOL("__Z21dComIfGp_setNextStagePKcsaafjiasii",
    void(char const*, s16, s8, s8, f32, u32, int, s8, s16, int, int), setNextStage);*/

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

DEFINE_HOOK(&daNpc_ykM_c::isDelete, daNpc_ykM_c__isDelete);
DEFINE_HOOK(&daNpc_ykW_c::isDelete, daNpc_ykW_c__isDelete);

namespace randomizer::ui {
dialogSelectModeState g_dialogSelectModeState = SelectReady;
}

namespace randomizer::hooks {
namespace {
UiDialogHandle playModeDialog{0};
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

    // if selected vanilla mode, let transition occur as normal
    if (ui::g_dialogSelectModeState == ui::SelectVanilla) {
        return HOOK_CONTINUE;
    }

    if (ui::g_dialogSelectModeState == ui::SelectReady && isHeaderTxtChange == true && isFileRecScale == true && isModoruTxtDisp == true) {
        ui::g_dialogSelectModeState = ui::SelectWait;

        auto buildDialog = [i_this]() {
            UiDialogDesc desc = UI_DIALOG_DESC_INIT;
            desc.title = "Play Type";
            desc.body_rml = "What mode would you like to play?";
            desc.icon = "question-mark";
            desc.variant = UI_DIALOG_NORMAL;

            UiDialogAction actions[2];
            actions[0] = {
                .label = "Vanilla",
                .on_pressed = [](ModContext* ctx, UiDialogHandle dialogHandle, void*) {
                    mDoAud_seStartMenu(Z2SE_SY_CURSOR_OK);
                    randomizer_GetContext() = RandomizerContext();
                    ui::g_dialogSelectModeState = ui::SelectVanilla;
                    session::svc_mng.ui->dialog_close(ctx, dialogHandle);
                },
                .user_data = nullptr,
                .keep_open = false,
            };
            actions[1] = {
                .label = "Randomizer",
                .on_pressed = [](ModContext* ctx, UiDialogHandle dialogHandle, void* userdata) {
                    mDoAud_seStartMenu(Z2SE_SY_CURSOR_OK);
                    ui::g_dialogSelectModeState = ui::SelectRandomizer;
                    ui::buildFileSelectGateMenu(static_cast<dFile_select_c*>(userdata));
                    session::svc_mng.ui->dialog_close(ctx, dialogHandle);
                },
                .user_data = i_this,
                .keep_open = false,
            };
            desc.actions = actions;
            desc.action_count = 2;

            if (session::svc_mng.ui->dialog_push(session::svc_mng.mod_ctx, &desc, &playModeDialog) != MOD_OK) {
                mods::log::error("Failed to push dialog");
                return MOD_ERROR;
            }

            return MOD_OK;
        };

        if (buildDialog() != MOD_OK) {
            mods::log::error("Failed to build dialog");
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
    // Allow rando to use all item slots. Checks the loaded hash rather than
    // randomizer_IsActive() because this runs on file select.
    if (randomizer_GetContext().mHash.empty()) {
        return;
    }

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
    // Set custom flag for the Temple of Time pedestal strike
    if (getStageID() == Sacred_Grove && mods::arg<int>(args, 1) == 0xEE) {
        mods::arg<dSv_info_c*>(args, 0)->onSwitch(0x63, mods::arg<int>(args, 2));
    }
    return HOOK_CONTINUE;
}

HookAction hookPreGetItemFunc(ModContext*, void* args, void*, void*) {
    const u8 item = mods::arg<u8>(args, 0);
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
        mods::arg_ref<s8>(args, 3)
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
    if (std::strcmp(scls_info->mStage, "F_SP103")
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

    //ADD_HOOK_PRE(setNextStage, hookPreSetNextStage);

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

    ADD_HOOK_POST(daNpc_ykM_c__isDelete, hookPostYkMIsDelete);
    ADD_HOOK_POST(daNpc_ykW_c__isDelete, hookPostYkWIsDelete);

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

    mods::hook::uninstall<daNpc_ykM_c__isDelete>(svc_hook);
    mods::hook::uninstall<daNpc_ykW_c__isDelete>(svc_hook);

    return MOD_OK;
}
}