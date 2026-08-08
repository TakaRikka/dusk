#include "session.hpp"

#include <mods/svc/log.hpp>

#include "randomizer_context.hpp"
#include "flags.h"
#include "item_ids.h"
#include "tools.h"

#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_meter2_info.h"

namespace randomizer::session {
ServiceManager svc_mng;

ModResult initialize(const ServiceManager& services) {
    svc_mng = services;

    return MOD_OK;
}

void setupRandomizerFile() {
    // Setup file based on randomizer data
    auto& randoData = randomizer_GetContext();
    randoData.mCreatingSave = true;

    // Set starting flags
    // Event Flags
    for (const auto& flag : randoData.mStartEventFlags) {
        dComIfGs_onEventBit(flag);
    }
    // Region Flags
    for (const auto& [region, flags] : randoData.mStartRegionFlags) {
        for (const auto& flag : flags) {
            onRegionFlag(region, flag);
        }
    }

    // Map bits (fills in overworld on map)
    setRegionBit(randoData.mMapBits);

    // Other flags based on starting flags
    if (dComIfGs_isEventBit(CLEARED_FARON_TWILIGHT))
    {
        dComIfGs_onDarkClearLV(0);
        dComIfGs_setLightDropNum(0, 0x10);
        execItemGet(dItemNo_Randomizer_DROP_CONTAINER_e);
        execItemGet(dItemNo_Randomizer_WEAR_KOKIRI_e);
    }

    if (dComIfGs_isEventBit(CLEARED_ELDIN_TWILIGHT))
    {
        dComIfGs_onDarkClearLV(1);
        dComIfGs_setLightDropNum(1, 0x10);
        execItemGet(dItemNo_Randomizer_DROP_CONTAINER02_e);
    }

    if (dComIfGs_isEventBit(CLEARED_LANAYRU_TWILIGHT))
    {
        dComIfGs_onDarkClearLV(2);
        dComIfGs_setLightDropNum(2, 0x10);
        execItemGet(dItemNo_Randomizer_DROP_CONTAINER03_e);
    }

    if (randoData.mSettings[RandomizerContext::SKIP_MINOR_CUTSCENES] == RandomizerContext::ON)
    {
        // Add letter data in this order to more or less reflect an order they can be obtained in game
        static const int letterOrder[] = {3, 2, 4, 7, 5, 6, 13, 12, 10, 9, 8, 15, 0, 14, 11};
        int letterNum = 0;
        for (int i : letterOrder) {
            if (dMenu_Letter::getLetterName(i) != 0) {
                dComIfGs_onLetterGetFlag(i);
                dComIfGs_setGetNumber(letterNum++, i + 1);
            }
        }
        setAllLetterRead();
    }

    // If MDH and the twilights are pre-completed
    if (dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_COMPLETED))
    {
        if ((dComIfGs_getSaveData()->getPlayer().getPlayerStatusB().mDarkClearLevelFlag & 0x7) == 0x7)
        {
            dComIfGs_onDarkClearLV(3);
            dComIfGs_onTransformLV(3); // Puts Midna on players back
        }
    }

    // Set starting inventory
    for (const auto& itemId: randoData.mStartingInventory) {
        execItemGet(itemId);
    }

    g_randomizerState = RandomizerState();
    mods::log::debug("Created Rando Save");
    randoData.mCreatingSave = false;
}

}