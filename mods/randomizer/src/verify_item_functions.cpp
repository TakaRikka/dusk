#include "verify_item_functions.h"

#include <array>

#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_item_data.h"
#include "item_ids.h"
#include "utilities.h"

bool haveItem(u32 item) {
    return checkItemGet((u8)item, 1);
}

template<typename T, size_t N>
u32 getProgressiveItem(const std::array<T, N>& progressiveItemsList) {
    u32 listLength = N;
    for (int i = 0; i < listLength; i++)
    {
        const u32 item = progressiveItemsList[i];
        if (!haveItem(item))
        {
            return item;
        }
    }

    // All previous obtained, so return last upgrade
    return progressiveItemsList[listLength - 1];
}

u32 getProgressiveSword() {
    static constexpr std::array progressiveItemsList = {
        dItemNo_Randomizer_WOOD_STICK_e,
        dItemNo_Randomizer_SWORD_e,
        dItemNo_Randomizer_MASTER_SWORD_e,
        dItemNo_Randomizer_LIGHT_SWORD_e,
    };

    return getProgressiveItem(progressiveItemsList);
};

u32 getProgressiveBow() {
    static constexpr std::array progressiveItemsList = {
        dItemNo_Randomizer_BOW_e,
        dItemNo_Randomizer_ARROW_LV2_e,
        dItemNo_Randomizer_ARROW_LV3_e,
    };

    return getProgressiveItem(progressiveItemsList);
};

u32 getProgressiveSkill() {
    static constexpr std::array progressiveItemsList = {
        dItemNo_Randomizer_ENDING_BLOW_e,
        dItemNo_Randomizer_SHIELD_ATTACK_e,
        dItemNo_Randomizer_BACK_SLICE_e,
        dItemNo_Randomizer_HELM_SPLITTER_e,
        dItemNo_Randomizer_MORTAL_DRAW_e,
        dItemNo_Randomizer_JUMP_STRIKE_e,
        dItemNo_Randomizer_GREAT_SPIN_e,
    };

    return getProgressiveItem(progressiveItemsList);
};

u32 getProgressiveSkybook() {
    if (!haveItem(dItemNo_Randomizer_ANCIENT_DOCUMENT2_e))
    {
        if (haveItem(dItemNo_Randomizer_ANCIENT_DOCUMENT_e))
        {
            if (getAncientDocumentNum() != 5)
            {
                return dItemNo_Randomizer_AIR_LETTER_e;
            }
        }
        else
        {
            return dItemNo_Randomizer_ANCIENT_DOCUMENT_e;
        }
    }

    // All previous obtained, so return last upgrade
    return dItemNo_Randomizer_ANCIENT_DOCUMENT2_e;
};

u32 getProgressiveKeyShard() {
    static constexpr std::array progressiveItemsList = {
        dItemNo_Randomizer_L2_KEY_PIECES1_e,
        dItemNo_Randomizer_L2_KEY_PIECES2_e,
        dItemNo_Randomizer_LV2_BOSS_KEY_e,
    };

    return getProgressiveItem(progressiveItemsList);
};

u32 getProgressiveMirrorShard() {
    static constexpr std::array progressiveItemsList = {
        dItemNo_Randomizer_MIRROR_PIECE_1_e,
        dItemNo_Randomizer_MIRROR_PIECE_2_e,
        dItemNo_Randomizer_MIRROR_PIECE_3_e,
        dItemNo_Randomizer_MIRROR_PIECE_4_e,
    };

    return getProgressiveItem(progressiveItemsList);
};

u32 getProgressiveFusedShadow() {
    static constexpr std::array progressiveItemsList = {
        dItemNo_Randomizer_FUSED_SHADOW_1_e,
        dItemNo_Randomizer_FUSED_SHADOW_2_e,
        dItemNo_Randomizer_FUSED_SHADOW_3_e,
    };

    return getProgressiveItem(progressiveItemsList);
};

u8 getWarashibeItemCount() {
    static constexpr u8 itemsList[] = {
        dItemNo_Randomizer_LETTER_e,
        dItemNo_Randomizer_BILL_e,
        dItemNo_Randomizer_WOOD_STATUE_e,
        dItemNo_Randomizer_IRIAS_PENDANT_e,
        dItemNo_Randomizer_HORSE_FLUTE_e
    };
    u8 count = 0;

    u32 listLength = sizeof(itemsList) / sizeof(itemsList[0]);
    for (int i = 0; i < listLength; i++)
    {
        const u32 item = itemsList[i];
        if (haveItem(item))
        {
            count++;
        }
    }
    return count;
};

u32 verifyProgressiveItem(u32 item)
{
    switch (item)
    {
        case dItemNo_Randomizer_WOOD_STICK_e:
        case dItemNo_Randomizer_SWORD_e:
        case dItemNo_Randomizer_MASTER_SWORD_e:
        case dItemNo_Randomizer_LIGHT_SWORD_e:
        {
            item = getProgressiveSword();
            break;
        }
        case dItemNo_Randomizer_BOW_e:
        case dItemNo_Randomizer_ARROW_LV2_e:
        case dItemNo_Randomizer_ARROW_LV3_e:
        {
            item = getProgressiveBow();
            break;
        }
        case dItemNo_WALLET_LV2_e:
        case dItemNo_WALLET_LV3_e:
        {
            if (haveItem(dItemNo_WALLET_LV2_e))
            {
                item = dItemNo_WALLET_LV3_e;
            }
            else
            {
                item = dItemNo_WALLET_LV2_e;
            }
            break;
        }
        case dItemNo_Randomizer_ENDING_BLOW_e:
        case dItemNo_Randomizer_SHIELD_ATTACK_e:
        case dItemNo_Randomizer_BACK_SLICE_e:
        case dItemNo_Randomizer_HELM_SPLITTER_e:
        case dItemNo_Randomizer_MORTAL_DRAW_e:
        case dItemNo_Randomizer_JUMP_STRIKE_e:
        case dItemNo_Randomizer_GREAT_SPIN_e:
        {
            item = getProgressiveSkill();
            break;
        }
        case dItemNo_Randomizer_HOOKSHOT_e:
        case dItemNo_Randomizer_W_HOOKSHOT_e:
        {
            // If we have either clawshot, we want to return the double no matter what.
            // We check for both in this case because the game unsets the clawshot flag once the double has been obtained.
            if (haveItem(dItemNo_Randomizer_HOOKSHOT_e) || haveItem(dItemNo_Randomizer_W_HOOKSHOT_e))
            {
                item = dItemNo_Randomizer_W_HOOKSHOT_e;
            }
            else
            {
                item = dItemNo_Randomizer_HOOKSHOT_e;
            }
            break;
        }
        case dItemNo_Randomizer_ANCIENT_DOCUMENT_e:
        case dItemNo_Randomizer_AIR_LETTER_e:
        case dItemNo_Randomizer_ANCIENT_DOCUMENT2_e:
        {
            item = getProgressiveSkybook();
            break;
        }
        case dItemNo_Randomizer_L2_KEY_PIECES1_e:
        case dItemNo_Randomizer_L2_KEY_PIECES2_e:
        case dItemNo_Randomizer_LV2_BOSS_KEY_e:
        {
            item = getProgressiveKeyShard();
            break;
        }
        case dItemNo_Randomizer_COPY_ROD_e:
        case dItemNo_Randomizer_COPY_ROD_2_e:
        {
            if (!haveItem(dItemNo_Randomizer_COPY_ROD_e))
            {
                item = dItemNo_Randomizer_COPY_ROD_e;
            }
            else
            {
                item = dItemNo_Randomizer_COPY_ROD_2_e;
            }
            break;
        }
        case dItemNo_Randomizer_FISHING_ROD_1_e:
        case dItemNo_Randomizer_ZORAS_JEWEL_e:
        {
            if (haveItem(dItemNo_Randomizer_FISHING_ROD_1_e))
            {
                item = dItemNo_Randomizer_ZORAS_JEWEL_e;
            }
            else
            {
                item = dItemNo_Randomizer_FISHING_ROD_1_e;
            }
            break;
        }
        case dItemNo_Randomizer_MIRROR_PIECE_1_e:
        case dItemNo_Randomizer_MIRROR_PIECE_2_e:
        case dItemNo_Randomizer_MIRROR_PIECE_3_e:
        case dItemNo_Randomizer_MIRROR_PIECE_4_e:
        {
            item = getProgressiveMirrorShard();
            break;
        }
        case dItemNo_Randomizer_FUSED_SHADOW_1_e:
        case dItemNo_Randomizer_FUSED_SHADOW_2_e:
        case dItemNo_Randomizer_FUSED_SHADOW_3_e:
        {
            item = getProgressiveFusedShadow();
            break;
        }
        case dItemNo_Randomizer_ARROW_10_e:
        case dItemNo_Randomizer_ARROW_20_e:
        case dItemNo_Randomizer_ARROW_30_e:
        {
            if (!haveItem(dItemNo_Randomizer_BOW_e))
            {
                item = dItemNo_Randomizer_BLUE_RUPEE_e;
            }
            break;
        }
        case dItemNo_Randomizer_BOMB_5_e:
        case dItemNo_Randomizer_BOMB_10_e:
        case dItemNo_Randomizer_BOMB_20_e:
        case dItemNo_Randomizer_BOMB_30_e:
        case dItemNo_Randomizer_WATER_BOMB_5_e:
        case dItemNo_Randomizer_WATER_BOMB_10_e:
        case dItemNo_Randomizer_WATER_BOMB_20_e:
        case dItemNo_Randomizer_WATER_BOMB_30_e:
        case dItemNo_Randomizer_BOMB_INSECT_5_e:
        case dItemNo_Randomizer_BOMB_INSECT_10_e:
        case dItemNo_Randomizer_BOMB_INSECT_20_e:
        case dItemNo_Randomizer_BOMB_INSECT_30_e:
        {
            if (!haveItem(dItemNo_Randomizer_BOMB_BAG_LV1_e))
            {
                item = dItemNo_Randomizer_BLUE_RUPEE_e;
            }
            break;
        }
        case dItemNo_Randomizer_PACHINKO_SHOT_e:
        {
            if (!haveItem(dItemNo_Randomizer_PACHINKO_e))
            {
                item = dItemNo_Randomizer_BLUE_RUPEE_e;
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return item;
}