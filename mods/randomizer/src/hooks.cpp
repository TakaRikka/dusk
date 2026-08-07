#include "hooks.hpp"
#include "session.hpp"
#include "randomizer_context.hpp"
#include "ui/rando_config.hpp"
#include "flags.h"
#include "stages.h"
#include "tools.h"
#include "item_ids.h"

#include <mods/svc/hook.hpp>
#include <mods/svc/log.hpp>

#include "d/actor/d_a_alink.h"
#include "d/d_file_select.h"
#include "d/d_meter2_info.h"
#include "d/d_save.h"
#include "d/d_shop_system.h"

DEFINE_HOOK(&dFile_select_c::selectDataNameMove, dFile_select_c__selectDataNameMove);
DEFINE_HOOK(&dFile_select_c::dataSelect, dFile_select_c__dataSelect);

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

// TODO: item service
/* HookAction hookPreExecItemGet(ModContext*, void* args, void*, void*) {
    if (randomizer_IsActive()) {
        const u8 item = mods::arg<u8>(args, 0);
        item_funcs::exec_item_get(item);
        dusk::mods::item_granted(item, mods::arg<u32>(args, 1), mods::arg<fopAc_ac_c*>(args, 2));
        return HOOK_SKIP_ORIGINAL;
    }
    return HOOK_CONTINUE;
} */

/* HookAction hookPreCheckItemGet(ModContext*, void* args, void* retval, void*) {
    if (randomizer_IsActive()) {
        *static_cast<int*>(retval) = item_funcs::check_item_get(mods::arg<u8>(args, 0), mods::arg<int>(args, 1));
        return HOOK_SKIP_ORIGINAL;
    }
    return HOOK_CONTINUE;
} */

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

    return MOD_OK;
}
}