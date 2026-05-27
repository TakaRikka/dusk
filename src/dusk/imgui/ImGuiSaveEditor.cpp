#include "fmt/format.h"
#include "imgui.h"
#include "aurora/gfx.h"

#include "ImGuiConsole.hpp"
#include "ImGuiSaveEditor.hpp"
#include "ImGuiEventFlags.hpp"

#include "d/d_com_inf_game.h"
#include "d/d_item_data.h"
#include "d/d_meter2_info.h"
#include "d/d_save.h"
#include "d/actor/d_a_player.h"
#include "dusk/ui/i18n.hpp"

#include <map>
#include <string_view>

namespace dusk {
    enum ItemType {
        ITEMTYPE_DEFAULT_e,
        ITEMTYPE_EQUIP_e,
    };

    struct itemInfo {
        std::string m_name;
        u8 m_type = ITEMTYPE_DEFAULT_e;
    };

    static std::string tx(std::string_view input) {
        Rml::String translated;
        ui::i18n::translate(translated, Rml::String(input.data(), input.size()));
        return translated;
    }

    static std::string txId(std::string_view input, std::string_view id) {
        std::string label = tx(input);
        label.append(id);
        return label;
    }

    static std::string itemName(int itemNo);

    std::map<int, itemInfo> itemMap = {
        { dItemNo_HEART_e, {"[SAVE_EDITOR_HEART]"} },
        { dItemNo_GREEN_RUPEE_e, {"[SAVE_EDITOR_GREEN_RUPEE]"} },
        { dItemNo_BLUE_RUPEE_e, {"[SAVE_EDITOR_BLUE_RUPEE]"} },
        { dItemNo_YELLOW_RUPEE_e, {"[SAVE_EDITOR_YELLOW_RUPEE]"} },
        { dItemNo_RED_RUPEE_e, {"[SAVE_EDITOR_RED_RUPEE]"} },
        { dItemNo_PURPLE_RUPEE_e, {"[SAVE_EDITOR_PURPLE_RUPEE]"} },
        { dItemNo_ORANGE_RUPEE_e, {"[SAVE_EDITOR_ORANGE_RUPEE]"} },
        { dItemNo_SILVER_RUPEE_e, {"[SAVE_EDITOR_SILVER_RUPEE]"} },
        { dItemNo_S_MAGIC_e, {"[SAVE_EDITOR_SMALL_MAGIC]"} },
        { dItemNo_L_MAGIC_e, {"[SAVE_EDITOR_LARGE_MAGIC]"} },
        { dItemNo_BOMB_5_e, {"[SAVE_EDITOR_BOMBS_5]"} },
        { dItemNo_BOMB_10_e, {"[SAVE_EDITOR_BOMBS_10]"} },
        { dItemNo_BOMB_20_e, {"[SAVE_EDITOR_BOMBS_20]"} },
        { dItemNo_BOMB_30_e, {"[SAVE_EDITOR_BOMBS_30]"} },
        { dItemNo_ARROW_10_e, {"[SAVE_EDITOR_ARROWS_10]"} },
        { dItemNo_ARROW_20_e, {"[SAVE_EDITOR_ARROWS_20]"} },
        { dItemNo_ARROW_30_e, {"[SAVE_EDITOR_ARROWS_30]"} },
        { dItemNo_ARROW_1_e, {"[SAVE_EDITOR_ARROWS_1]"} },
        { dItemNo_PACHINKO_SHOT_e, {"[SAVE_EDITOR_PUMPKIN_SEEDS]"} },
        { dItemNo_NOENTRY_19_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_20_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_21_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_WATER_BOMB_5_e, {"[SAVE_EDITOR_WATER_BOMBS_5]"} },
        { dItemNo_WATER_BOMB_10_e, {"[SAVE_EDITOR_WATER_BOMBS_10]"} },
        { dItemNo_WATER_BOMB_20_e, {"[SAVE_EDITOR_WATER_BOMBS_20]"} },
        { dItemNo_WATER_BOMB_30_e, {"[SAVE_EDITOR_WATER_BOMBS_30]"} },
        { dItemNo_BOMB_INSECT_5_e, {"[SAVE_EDITOR_BOMBLINGS_5]"} },
        { dItemNo_BOMB_INSECT_10_e, {"[SAVE_EDITOR_BOMBLINGS_10]"} },
        { dItemNo_BOMB_INSECT_20_e, {"[SAVE_EDITOR_BOMBLINGS_20]"} },
        { dItemNo_BOMB_INSECT_30_e, {"[SAVE_EDITOR_BOMBLINGS_30]"} },
        { dItemNo_RECOVERY_FAILY_e, {"[SAVE_EDITOR_FAIRY]"} },
        { dItemNo_TRIPLE_HEART_e, {"[SAVE_EDITOR_TRIPLE_HEARTS]"} },
        { dItemNo_SMALL_KEY_e, {"[SAVE_EDITOR_SMALL_KEY]"} },
        { dItemNo_KAKERA_HEART_e, {"[SAVE_EDITOR_PIECE_OF_HEART]"} },
        { dItemNo_UTAWA_HEART_e, {"[SAVE_EDITOR_HEART_CONTAINER]"} },
        { dItemNo_MAP_e, {"[SAVE_EDITOR_DUNGEON_MAP]"} },
        { dItemNo_COMPUS_e, {"[SAVE_EDITOR_COMPASS]"} },
        { dItemNo_DUNGEON_EXIT_e, {"[SAVE_EDITOR_OOCCOO_SR_FIRST_TIME]", ITEMTYPE_EQUIP_e} },
        { dItemNo_BOSS_KEY_e, {"[SAVE_EDITOR_BOSS_KEY]"} },
        { dItemNo_DUNGEON_BACK_e, {"[SAVE_EDITOR_OOCCOO_JR]", ITEMTYPE_EQUIP_e} },
        { dItemNo_SWORD_e, {"[SAVE_EDITOR_ORDON_SWORD]"} },
        { dItemNo_MASTER_SWORD_e, {"[SAVE_EDITOR_MASTER_SWORD]"} },
        { dItemNo_WOOD_SHIELD_e, {"[SAVE_EDITOR_WOODEN_SHIELD]"} },
        { dItemNo_SHIELD_e, {"[SAVE_EDITOR_ORDON_SHIELD]"} },
        { dItemNo_HYLIA_SHIELD_e, {"[SAVE_EDITOR_HYLIAN_SHIELD]"} },
        { dItemNo_TKS_LETTER_e, {"[SAVE_EDITOR_OOCCOO_S_NOTE]", ITEMTYPE_EQUIP_e} },
        { dItemNo_WEAR_CASUAL_e, {"[SAVE_EDITOR_ORDON_CLOTHES]"} },
        { dItemNo_WEAR_KOKIRI_e, {"[SAVE_EDITOR_HERO_S_CLOTHES]"} },
        { dItemNo_ARMOR_e, {"[SAVE_EDITOR_MAGIC_ARMOR]"} },
        { dItemNo_WEAR_ZORA_e, {"[SAVE_EDITOR_ZORA_ARMOR]"} },
        { dItemNo_MAGIC_LV1_e, {"[SAVE_EDITOR_MAGIC_LEVEL_1]"} },
        { dItemNo_DUNGEON_EXIT_2_e, {"[SAVE_EDITOR_OOCCOO_SR]", ITEMTYPE_EQUIP_e} },
        { dItemNo_WALLET_LV1_e, {"[SAVE_EDITOR_WALLET]"} },
        { dItemNo_WALLET_LV2_e, {"[SAVE_EDITOR_BIG_WALLET]"} },
        { dItemNo_WALLET_LV3_e, {"[SAVE_EDITOR_GIANT_WALLET]"} },
        { dItemNo_NOENTRY_55_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_56_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_57_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_58_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_59_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_60_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_ZORAS_JEWEL_e, {"[SAVE_EDITOR_CORAL_EARRING]", ITEMTYPE_EQUIP_e} },
        { dItemNo_HAWK_EYE_e, {"[SAVE_EDITOR_HAWKEYE]", ITEMTYPE_EQUIP_e} },
        { dItemNo_WOOD_STICK_e, {"[SAVE_EDITOR_WOODEN_SWORD]"} },
        { dItemNo_BOOMERANG_e, {"[SAVE_EDITOR_GALE_BOOMERANG]", ITEMTYPE_EQUIP_e} },
        { dItemNo_SPINNER_e, {"[SAVE_EDITOR_SPINNER]", ITEMTYPE_EQUIP_e} },
        { dItemNo_IRONBALL_e, {"[SAVE_EDITOR_BALL_AND_CHAIN]", ITEMTYPE_EQUIP_e} },
        { dItemNo_BOW_e, {"[SAVE_EDITOR_HERO_S_BOW]", ITEMTYPE_EQUIP_e} },
        { dItemNo_HOOKSHOT_e, {"[SAVE_EDITOR_CLAWSHOT]", ITEMTYPE_EQUIP_e} },
        { dItemNo_HVY_BOOTS_e, {"[SAVE_EDITOR_IRON_BOOTS]", ITEMTYPE_EQUIP_e} },
        { dItemNo_COPY_ROD_e, {"[SAVE_EDITOR_DOMINION_ROD]", ITEMTYPE_EQUIP_e} },
        { dItemNo_W_HOOKSHOT_e, {"[SAVE_EDITOR_DOUBLE_CLAWSHOTS]", ITEMTYPE_EQUIP_e} },
        { dItemNo_KANTERA_e, {"[SAVE_EDITOR_LANTERN]", ITEMTYPE_EQUIP_e} },
        { dItemNo_LIGHT_SWORD_e, {"[SAVE_EDITOR_LIGHT_SWORD]"} },
        { dItemNo_FISHING_ROD_1_e, {"[SAVE_EDITOR_FISHING_ROD]", ITEMTYPE_EQUIP_e} },
        { dItemNo_PACHINKO_e, {"[SAVE_EDITOR_SLINGSHOT]", ITEMTYPE_EQUIP_e} },
        { dItemNo_COPY_ROD_2_e, {"[SAVE_EDITOR_DOMINION_ROD_UNCHARGED]"} },
        { dItemNo_NOENTRY_77_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_78_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_BOMB_BAG_LV2_e, {"[SAVE_EDITOR_GIANT_BOMB_BAG]"} },
        { dItemNo_BOMB_BAG_LV1_e, {"[SAVE_EDITOR_EMPTY_BOMB_BAG]", ITEMTYPE_EQUIP_e} },
        { dItemNo_BOMB_IN_BAG_e, {"[SAVE_EDITOR_BOMB_BAG]"} },
        { dItemNo_NOENTRY_82_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_LIGHT_ARROW_e, {"[SAVE_EDITOR_LIGHT_ARROW]"} },
        { dItemNo_ARROW_LV1_e, {"[SAVE_EDITOR_QUIVER]"} },
        { dItemNo_ARROW_LV2_e, {"[SAVE_EDITOR_BIG_QUIVER]"} },
        { dItemNo_ARROW_LV3_e, {"[SAVE_EDITOR_GIANT_QUIVER]"} },
        { dItemNo_NOENTRY_87_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_LURE_ROD_e, {"[SAVE_EDITOR_FISHING_ROD_LURE]"} },
        { dItemNo_BOMB_ARROW_e, {"[SAVE_EDITOR_BOMB_ARROW]"} },
        { dItemNo_HAWK_ARROW_e, {"[SAVE_EDITOR_HAWK_ARROW]"} },
        { dItemNo_BEE_ROD_e, {"[SAVE_EDITOR_FISHING_ROD_BEE_LARVA]", ITEMTYPE_EQUIP_e} },
        { dItemNo_JEWEL_ROD_e, {"[SAVE_EDITOR_FISHING_ROD_EARRING]", ITEMTYPE_EQUIP_e} },
        { dItemNo_WORM_ROD_e, {"[SAVE_EDITOR_FISHING_ROD_WORM]", ITEMTYPE_EQUIP_e} },
        { dItemNo_JEWEL_BEE_ROD_e, {"[SAVE_EDITOR_FISHING_ROD_EARRING_BEE_LARVA]", ITEMTYPE_EQUIP_e} },
        { dItemNo_JEWEL_WORM_ROD_e, {"[SAVE_EDITOR_FISHING_ROD_EARRING_WORM]", ITEMTYPE_EQUIP_e} },
        { dItemNo_EMPTY_BOTTLE_e, {"[SAVE_EDITOR_EMPTY_BOTTLE]", ITEMTYPE_EQUIP_e} },
        { dItemNo_RED_BOTTLE_e, {"[SAVE_EDITOR_RED_POTION]", ITEMTYPE_EQUIP_e} },
        { dItemNo_GREEN_BOTTLE_e, {"[SAVE_EDITOR_GREEN_POTION]", ITEMTYPE_EQUIP_e} },
        { dItemNo_BLUE_BOTTLE_e, {"[SAVE_EDITOR_BLUE_POTION]", ITEMTYPE_EQUIP_e} },
        { dItemNo_MILK_BOTTLE_e, {"[SAVE_EDITOR_MILK_BOTTLE]", ITEMTYPE_EQUIP_e} },
        { dItemNo_HALF_MILK_BOTTLE_e, {"[SAVE_EDITOR_HALF_MILK_BOTTLE]", ITEMTYPE_EQUIP_e} },
        { dItemNo_OIL_BOTTLE_e, {"[SAVE_EDITOR_LANTERN_OIL]", ITEMTYPE_EQUIP_e} },
        { dItemNo_WATER_BOTTLE_e, {"[SAVE_EDITOR_WATER_BOTTLE]", ITEMTYPE_EQUIP_e} },
        { dItemNo_OIL_BOTTLE_2_e, {"[SAVE_EDITOR_LANTERN_OIL_SCOOPED]"} },
        { dItemNo_RED_BOTTLE_2_e, {"[SAVE_EDITOR_RED_POTION_SCOOPED]"} },
        { dItemNo_UGLY_SOUP_e, {"[SAVE_EDITOR_NASTY_SOUP]", ITEMTYPE_EQUIP_e} },
        { dItemNo_HOT_SPRING_e, {"[SAVE_EDITOR_HOTSPRING_WATER]", ITEMTYPE_EQUIP_e} },
        { dItemNo_FAIRY_e, {"[SAVE_EDITOR_FAIRY]", ITEMTYPE_EQUIP_e} },
        { dItemNo_HOT_SPRING_2_e, {"[SAVE_EDITOR_HOTSPRING_WATER_SHOP]"} },
        { dItemNo_OIL2_e, {"[SAVE_EDITOR_LANTERN_REFILL_SCOOPED]"} },
        { dItemNo_OIL_e, {"[SAVE_EDITOR_LANTERN_REFILL_SHOP]"} },
        { dItemNo_NORMAL_BOMB_e, {"[SAVE_EDITOR_BOMBS]", ITEMTYPE_EQUIP_e} },
        { dItemNo_WATER_BOMB_e, {"[SAVE_EDITOR_WATER_BOMBS]", ITEMTYPE_EQUIP_e} },
        { dItemNo_POKE_BOMB_e, {"[SAVE_EDITOR_BOMBLINGS]", ITEMTYPE_EQUIP_e} },
        { dItemNo_FAIRY_DROP_e, {"[SAVE_EDITOR_GREAT_FAIRY_S_TEARS]", ITEMTYPE_EQUIP_e} },
        { dItemNo_WORM_e, {"[SAVE_EDITOR_WORM]", ITEMTYPE_EQUIP_e} },
        { dItemNo_DROP_BOTTLE_e, {"[SAVE_EDITOR_GREAT_FAIRY_TEARS_JOVANI]"} },
        { dItemNo_BEE_CHILD_e, {"[SAVE_EDITOR_BEE_LARVA]", ITEMTYPE_EQUIP_e} },
        { dItemNo_CHUCHU_RARE_e, {"[SAVE_EDITOR_RARE_CHU_JELLY]", ITEMTYPE_EQUIP_e} },
        { dItemNo_CHUCHU_RED_e, {"[SAVE_EDITOR_RED_CHU_JELLY]", ITEMTYPE_EQUIP_e} },
        { dItemNo_CHUCHU_BLUE_e, {"[SAVE_EDITOR_BLUE_CHU_JELLY]", ITEMTYPE_EQUIP_e} },
        { dItemNo_CHUCHU_GREEN_e, {"[SAVE_EDITOR_GREEN_CHU_JELLY]", ITEMTYPE_EQUIP_e} },
        { dItemNo_CHUCHU_YELLOW_e, {"[SAVE_EDITOR_YELLOW_CHU_JELLY]", ITEMTYPE_EQUIP_e} },
        { dItemNo_CHUCHU_PURPLE_e, {"[SAVE_EDITOR_PURPLE_CHU_JELLY]", ITEMTYPE_EQUIP_e} },
        { dItemNo_LV1_SOUP_e, {"[SAVE_EDITOR_SIMPLE_SOUP]", ITEMTYPE_EQUIP_e} },
        { dItemNo_LV2_SOUP_e, {"[SAVE_EDITOR_GOOD_SOUP]", ITEMTYPE_EQUIP_e} },
        { dItemNo_LV3_SOUP_e, {"[SAVE_EDITOR_SUPERB_SOUP]", ITEMTYPE_EQUIP_e} },
        { dItemNo_LETTER_e, {"[SAVE_EDITOR_RENADO_S_LETTER]", ITEMTYPE_EQUIP_e} },
        { dItemNo_BILL_e, {"[SAVE_EDITOR_INVOICE]", ITEMTYPE_EQUIP_e} },
        { dItemNo_WOOD_STATUE_e, {"[SAVE_EDITOR_WOODEN_STATUE]", ITEMTYPE_EQUIP_e} },
        { dItemNo_IRIAS_PENDANT_e, {"[SAVE_EDITOR_ILIA_S_CHARM]", ITEMTYPE_EQUIP_e} },
        { dItemNo_HORSE_FLUTE_e, {"[SAVE_EDITOR_HORSE_CALL]", ITEMTYPE_EQUIP_e} },
        { dItemNo_NOENTRY_133_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_134_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_135_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_136_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_137_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_138_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_139_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_140_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_141_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_142_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_143_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_RAFRELS_MEMO_e, {"[SAVE_EDITOR_AURU_S_MEMO]", ITEMTYPE_EQUIP_e} },
        { dItemNo_ASHS_SCRIBBLING_e, {"[SAVE_EDITOR_ASHEI_S_SKETCH]", ITEMTYPE_EQUIP_e} },
        { dItemNo_NOENTRY_146_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_147_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_148_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_149_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_150_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_151_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_152_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_153_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_154_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_155_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_CHUCHU_YELLOW2_e, {"[SAVE_EDITOR_LANTERN_REFILL_YELLOW_CHU]"} },
        { dItemNo_OIL_BOTTLE3_e, {"[SAVE_EDITOR_LANTERN_OIL_CORO]"} },
        { dItemNo_SHOP_BEE_CHILD_e, {"[SAVE_EDITOR_BEE_LARVE_SHOP]"} },
        { dItemNo_CHUCHU_BLACK_e, {"[SAVE_EDITOR_BLACK_CHU_JELLY]", ITEMTYPE_EQUIP_e} },
        { dItemNo_LIGHT_DROP_e, {"[SAVE_EDITOR_TEAR_OF_LIGHT]"} },
        { dItemNo_DROP_CONTAINER_e, {"[SAVE_EDITOR_VESSEL_OF_LIGHT_FARON]"} },
        { dItemNo_DROP_CONTAINER02_e, {"[SAVE_EDITOR_VESSEL_OF_LIGHT_ELDIN]"} },
        { dItemNo_DROP_CONTAINER03_e, {"[SAVE_EDITOR_VESSEL_OF_LIGHT_LANAYRU]"} },
        { dItemNo_FILLED_CONTAINER_e, {"[SAVE_EDITOR_VESSEL_OF_LIGHT_FILLED]"} },
        { dItemNo_MIRROR_PIECE_2_e, {"[SAVE_EDITOR_MIRROR_SHARD_SNOWPEAK_RUINS]"} },
        { dItemNo_MIRROR_PIECE_3_e, {"[SAVE_EDITOR_MIRROR_SHARD_TEMPLE_OF_TIME]"} },
        { dItemNo_MIRROR_PIECE_4_e, {"[SAVE_EDITOR_MIRROR_SHARD_CITY_IN_THE_SKY]"} },
        { dItemNo_NOENTRY_168_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_169_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_170_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_171_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_172_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_173_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_174_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_175_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_SMELL_YELIA_POUCH_e, {"[SAVE_EDITOR_SCENT_OF_ILIA]"} },
        { dItemNo_SMELL_PUMPKIN_e, {"[SAVE_EDITOR_PUMPKIN_SCENT]"} },
        { dItemNo_SMELL_POH_e, {"[SAVE_EDITOR_POE_SCENT]"} },
        { dItemNo_SMELL_FISH_e, {"[SAVE_EDITOR_REEKFISH_SCENT]"} },
        { dItemNo_SMELL_CHILDREN_e, {"[SAVE_EDITOR_YOUTH_S_SCENT]"} },
        { dItemNo_SMELL_MEDICINE_e, {"[SAVE_EDITOR_MEDICINE_SCENT]"} },
        { dItemNo_NOENTRY_182_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_183_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_184_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_185_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_186_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_187_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_188_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_189_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_190_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_191_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_M_BEETLE_e, {"[SAVE_EDITOR_BEETLE_M]"} },
        { dItemNo_F_BEETLE_e, {"[SAVE_EDITOR_BEETLE_F]"} },
        { dItemNo_M_BUTTERFLY_e, {"[SAVE_EDITOR_BUTTERFLY_M]"} },
        { dItemNo_F_BUTTERFLY_e, {"[SAVE_EDITOR_BUTTERFLY_F]"} },
        { dItemNo_M_STAG_BEETLE_e, {"[SAVE_EDITOR_STAG_BEETLE_M]"} },
        { dItemNo_F_STAG_BEETLE_e, {"[SAVE_EDITOR_STAG_BEETLE_F]"} },
        { dItemNo_M_GRASSHOPPER_e, {"[SAVE_EDITOR_GRASSHOPPER_M]"} },
        { dItemNo_F_GRASSHOPPER_e, {"[SAVE_EDITOR_GRASSHOPPER_F]"} },
        { dItemNo_M_NANAFUSHI_e, {"[SAVE_EDITOR_PHASMID_M]"} },
        { dItemNo_F_NANAFUSHI_e, {"[SAVE_EDITOR_PHASMID_F]"} },
        { dItemNo_M_DANGOMUSHI_e, {"[SAVE_EDITOR_PILL_BUG_M]"} },
        { dItemNo_F_DANGOMUSHI_e, {"[SAVE_EDITOR_PILL_BUG_F]"} },
        { dItemNo_M_MANTIS_e, {"[SAVE_EDITOR_MANTIS_M]"} },
        { dItemNo_F_MANTIS_e, {"[SAVE_EDITOR_MANTIS_F]"} },
        { dItemNo_M_LADYBUG_e, {"[SAVE_EDITOR_LADYBUG_M]"} },
        { dItemNo_F_LADYBUG_e, {"[SAVE_EDITOR_LADYBUG_F]"} },
        { dItemNo_M_SNAIL_e, {"[SAVE_EDITOR_SNAIL_M]"} },
        { dItemNo_F_SNAIL_e, {"[SAVE_EDITOR_SNAIL_F]"} },
        { dItemNo_M_DRAGONFLY_e, {"[SAVE_EDITOR_DRAGONFLY_M]"} },
        { dItemNo_F_DRAGONFLY_e, {"[SAVE_EDITOR_DRAGONFLY_F]"} },
        { dItemNo_M_ANT_e, {"[SAVE_EDITOR_ANT_M]"} },
        { dItemNo_F_ANT_e, {"[SAVE_EDITOR_ANT_F]"} },
        { dItemNo_M_MAYFLY_e, {"[SAVE_EDITOR_MAYFLY_M]"} },
        { dItemNo_F_MAYFLY_e, {"[SAVE_EDITOR_MAYFLY_F]"} },
        { dItemNo_NOENTRY_216_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_217_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_218_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_219_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_220_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_221_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_222_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_223_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_POU_SPIRIT_e, {"[SAVE_EDITOR_POE_SOUL]"} },
        { dItemNo_NOENTRY_225_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_226_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_227_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_228_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_229_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_230_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_231_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_NOENTRY_232_e, {"[SAVE_EDITOR_RESERVED]"} },
        { dItemNo_ANCIENT_DOCUMENT_e, {"[SAVE_EDITOR_ANCIENT_SKY_BOOK]", ITEMTYPE_EQUIP_e} },
        { dItemNo_AIR_LETTER_e, {"[SAVE_EDITOR_ANCIENT_SKY_BOOK_PARTIAL]", ITEMTYPE_EQUIP_e} },
        { dItemNo_ANCIENT_DOCUMENT2_e, {"[SAVE_EDITOR_ANCIENT_SKY_BOOK_FILLED]", ITEMTYPE_EQUIP_e} },
        { dItemNo_LV7_DUNGEON_EXIT_e, {"[SAVE_EDITOR_OOCCOO_SR_CITY_IN_THE_SKY]"} },
        { dItemNo_LINKS_SAVINGS_e, {"[SAVE_EDITOR_PURPLE_RUPEE_LINK_S_SAVINGS]"} },
        { dItemNo_SMALL_KEY2_e, {"[SAVE_EDITOR_SMALL_KEY_NORTH_FARON_GATE]"} },
        { dItemNo_POU_FIRE1_e, {"[SAVE_EDITOR_POE_FIRE_1]"} },
        { dItemNo_POU_FIRE2_e, {"[SAVE_EDITOR_POE_FIRE_2]"} },
        { dItemNo_POU_FIRE3_e, {"[SAVE_EDITOR_POE_FIRE_3]"} },
        { dItemNo_POU_FIRE4_e, {"[SAVE_EDITOR_POE_FIRE_4]"} },
        { dItemNo_BOSSRIDER_KEY_e, {"[SAVE_EDITOR_HYRULE_FIELD_KEYS]"} },
        { dItemNo_TOMATO_PUREE_e, {"[SAVE_EDITOR_ORDON_PUMPKIN]", ITEMTYPE_EQUIP_e} },
        { dItemNo_TASTE_e, {"[SAVE_EDITOR_ORDON_GOAT_CHEESE]", ITEMTYPE_EQUIP_e} },
        { dItemNo_LV5_BOSS_KEY_e, {"[SAVE_EDITOR_BEDROOM_KEY]"} },
        { dItemNo_SURFBOARD_e, {"[SAVE_EDITOR_SURF_LEAF]"} },
        { dItemNo_KANTERA2_e, {"[SAVE_EDITOR_LANTERN_RECLAIMED]"} },
        { dItemNo_L2_KEY_PIECES1_e, {"[SAVE_EDITOR_KEY_SHARD_1]"} },
        { dItemNo_L2_KEY_PIECES2_e, {"[SAVE_EDITOR_KEY_SHARD_2]"} },
        { dItemNo_L2_KEY_PIECES3_e, {"[SAVE_EDITOR_KEY_SHARD_3]"} },
        { dItemNo_KEY_OF_CARAVAN_e, {"[SAVE_EDITOR_BULBLIN_CAMP_KEY]"} },
        { dItemNo_LV2_BOSS_KEY_e, {"[SAVE_EDITOR_GORON_MINES_BOSS_KEY]"} },
        { dItemNo_KEY_OF_FILONE_e, {"[SAVE_EDITOR_SOUTH_FARON_GATE_KEY]"} },
        { dItemNo_NONE_e, {"[SAVE_EDITOR_NONE]"} },
    };

    static std::string itemName(int itemNo) {
        const auto iter = itemMap.find(itemNo);
        return tx(iter != itemMap.end() ? iter->second.m_name : "[SAVE_EDITOR_NONE]");
    }

    static std::string slotItemLabel(int slot, int itemNo) {
        return fmt::format(tx("[SAVE_EDITOR_SLOT_ITEM_FORMAT]"), slot, itemName(itemNo));
    }

    static constexpr int BUG_SPECIES_COUNT = 12;

    static const u8 sBugItemIds[BUG_SPECIES_COUNT * 2] = {
        dItemNo_M_ANT_e,         dItemNo_F_ANT_e,
        dItemNo_M_MAYFLY_e,      dItemNo_F_MAYFLY_e,
        dItemNo_M_BEETLE_e,      dItemNo_F_BEETLE_e,
        dItemNo_M_MANTIS_e,      dItemNo_F_MANTIS_e,
        dItemNo_M_STAG_BEETLE_e, dItemNo_F_STAG_BEETLE_e,
        dItemNo_M_DANGOMUSHI_e,  dItemNo_F_DANGOMUSHI_e,
        dItemNo_M_BUTTERFLY_e,   dItemNo_F_BUTTERFLY_e,
        dItemNo_M_LADYBUG_e,     dItemNo_F_LADYBUG_e,
        dItemNo_M_SNAIL_e,       dItemNo_F_SNAIL_e,
        dItemNo_M_NANAFUSHI_e,   dItemNo_F_NANAFUSHI_e,
        dItemNo_M_GRASSHOPPER_e, dItemNo_F_GRASSHOPPER_e,
        dItemNo_M_DRAGONFLY_e,   dItemNo_F_DRAGONFLY_e,
    };

    static const u16 sBugTurnInFlags[BUG_SPECIES_COUNT * 2] = {
        dSv_event_flag_c::F_0421, dSv_event_flag_c::F_0422,
        dSv_event_flag_c::F_0423, dSv_event_flag_c::F_0424,
        dSv_event_flag_c::F_0401, dSv_event_flag_c::F_0402,
        dSv_event_flag_c::F_0413, dSv_event_flag_c::F_0414,
        dSv_event_flag_c::F_0405, dSv_event_flag_c::F_0406,
        dSv_event_flag_c::F_0411, dSv_event_flag_c::F_0412,
        dSv_event_flag_c::F_0403, dSv_event_flag_c::F_0404,
        dSv_event_flag_c::F_0415, dSv_event_flag_c::F_0416,
        dSv_event_flag_c::F_0417, dSv_event_flag_c::F_0418,
        dSv_event_flag_c::F_0409, dSv_event_flag_c::F_0410,
        dSv_event_flag_c::F_0407, dSv_event_flag_c::F_0408,
        dSv_event_flag_c::F_0419, dSv_event_flag_c::F_0420,
    };

    static const char* sBugSpeciesNames[BUG_SPECIES_COUNT] = {
        "[SAVE_EDITOR_ANT]",      "[SAVE_EDITOR_DAYFLY]",  "[SAVE_EDITOR_BEETLE]",  "[SAVE_EDITOR_MANTIS]",
        "[SAVE_EDITOR_STAG_BEETLE]", "[SAVE_EDITOR_PILL_BUG]", "[SAVE_EDITOR_BUTTERFLY]", "[SAVE_EDITOR_LADYBUG]",
        "[SAVE_EDITOR_SNAIL]",    "[SAVE_EDITOR_PHASMID]", "[SAVE_EDITOR_GRASSHOPPER]", "[SAVE_EDITOR_DRAGONFLY]",
    };

    static constexpr int HIDDEN_SKILL_COUNT = 7;

    static const u16 sHiddenSkillFlags[HIDDEN_SKILL_COUNT] = {
        dSv_event_flag_c::F_0339, dSv_event_flag_c::F_0338,
        dSv_event_flag_c::F_0340, dSv_event_flag_c::F_0341,
        dSv_event_flag_c::F_0342, dSv_event_flag_c::F_0343,
        dSv_event_flag_c::F_0344,
    };

    static const char* sHiddenSkillNames[HIDDEN_SKILL_COUNT] = {
        "[SAVE_EDITOR_ENDING_BLOW]", "[SAVE_EDITOR_SHIELD_ATTACK]", "[SAVE_EDITOR_BACK_SLICE]", "[SAVE_EDITOR_HELM_SPLITTER]",
        "[SAVE_EDITOR_MORTAL_DRAW]", "[SAVE_EDITOR_JUMP_STRIKE]", "[SAVE_EDITOR_GREAT_SPIN]",
    };

    static constexpr int LETTER_COUNT = 16;

    static const char* sLetterSenders[LETTER_COUNT] = {
        "[SAVE_EDITOR_RENADO]",        "[SAVE_EDITOR_OOCCOO_1]", "[SAVE_EDITOR_OOCCOO_2]",       "[SAVE_EDITOR_THE_POSTMAN]",
        "[SAVE_EDITOR_KAKARIKO_GOODS]", "[SAVE_EDITOR_BARNES_1]", "[SAVE_EDITOR_BARNES_2]",      "[SAVE_EDITOR_BARNES_BOMBS]",
        "[SAVE_EDITOR_MALO_MART]",     "[SAVE_EDITOR_TELMA]",    "[SAVE_EDITOR_PURLO]",          "[SAVE_EDITOR_FROM_JR]",
        "[SAVE_EDITOR_PRINCESS_AGITHA]", "[SAVE_EDITOR_LANAYRU_TOURISM]", "[SAVE_EDITOR_SHAD]", "[SAVE_EDITOR_YETA]",
    };

    static constexpr int FISH_COUNT = 6;

    static const struct {
        u8 index;
        const char* name;
    } sFishSpecies[FISH_COUNT] = {
        { 3, "[SAVE_EDITOR_ORDON_CATFISH]" },
        { 5, "[SAVE_EDITOR_GREENGILL]"     },
        { 4, "[SAVE_EDITOR_REEKFISH]"      },
        { 0, "[SAVE_EDITOR_HYRULE_BASS]"   },
        { 2, "[SAVE_EDITOR_HYLIAN_PIKE]"   },
        { 1, "[SAVE_EDITOR_HYLIAN_LOACH]"  },
    };

    static const char* sSwordNames[4] = {
        "[SAVE_EDITOR_ORDON_SWORD]", "[SAVE_EDITOR_MASTER_SWORD]", "[SAVE_EDITOR_WOODEN_SWORD]", "[SAVE_EDITOR_LIGHT_SWORD]",
    };

    static const char* sShieldNames[3] = {
        "[SAVE_EDITOR_WOODEN_SHIELD]", "[SAVE_EDITOR_ORDON_SHIELD]", "[SAVE_EDITOR_HYLIAN_SHIELD]",
    };

    static const char* sFusedShadowNames[3] = {
        "[SAVE_EDITOR_FOREST_TEMPLE]",
        "[SAVE_EDITOR_GORON_MINES]",
        "[SAVE_EDITOR_LAKEBED_TEMPLE]",
    };

    static const struct {
        u8 index;
        const char* name;
    } sMirrorShards[3] = {
        { 1, "[SAVE_EDITOR_SNOWPEAK_RUINS]"  },
        { 2, "[SAVE_EDITOR_TEMPLE_OF_TIME]"  },
        { 3, "[SAVE_EDITOR_CITY_IN_THE_SKY]" },
    };

    static const struct {
        u8 slot;
        u8 item;
    } sDefaultInventory[] = {
        { SLOT_0,  dItemNo_BOOMERANG_e    },
        { SLOT_1,  dItemNo_KANTERA_e      },
        { SLOT_2,  dItemNo_SPINNER_e      },
        { SLOT_3,  dItemNo_HVY_BOOTS_e    },
        { SLOT_4,  dItemNo_BOW_e          },
        { SLOT_5,  dItemNo_HAWK_EYE_e     },
        { SLOT_6,  dItemNo_IRONBALL_e     },
        { SLOT_8,  dItemNo_COPY_ROD_e     },
        { SLOT_9,  dItemNo_HOOKSHOT_e     },
        { SLOT_10, dItemNo_W_HOOKSHOT_e   },
        { SLOT_11, dItemNo_EMPTY_BOTTLE_e },
        { SLOT_12, dItemNo_EMPTY_BOTTLE_e },
        { SLOT_13, dItemNo_EMPTY_BOTTLE_e },
        { SLOT_14, dItemNo_EMPTY_BOTTLE_e },
        { SLOT_15, dItemNo_NORMAL_BOMB_e  },
        { SLOT_16, dItemNo_WATER_BOMB_e   },
        { SLOT_17, dItemNo_POKE_BOMB_e    },
        { SLOT_18, dItemNo_DUNGEON_EXIT_e },
        { SLOT_20, dItemNo_FISHING_ROD_1_e},
        { SLOT_21, dItemNo_HORSE_FLUTE_e  },
        { SLOT_22, dItemNo_ANCIENT_DOCUMENT_e },
        { SLOT_23, dItemNo_PACHINKO_e     },
    };

    ImGuiSaveEditor::ImGuiSaveEditor() {}

    void ImGuiSaveEditor::draw(bool& open) {
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

        // ImGui::SetNextWindowBgAlpha(0.65f);

        if (ImGui::Begin(tx("[SAVE_EDITOR_SAVE_EDITOR]").c_str(), &open, windowFlags)) {
            if (ImGui::BeginTabBar("SaveEditorTabBar", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton)) {
                if (ImGui::BeginTabItem(tx("[SAVE_EDITOR_PLAYER_STATUS]").c_str())) {
                    drawPlayerStatusTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(tx("[SAVE_EDITOR_LOCATION]").c_str())) {
                    drawLocationTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(tx("[SAVE_EDITOR_INVENTORY]").c_str())) {
                    drawInventoryTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(tx("[SAVE_EDITOR_COLLECTION]").c_str())) {
                    drawCollectionTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(tx("[SAVE_EDITOR_FLAGS]").c_str())) {
                    drawFlagsTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(tx("[SAVE_EDITOR_MINIGAME]").c_str())) {
                    drawMinigameTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(tx("[SAVE_EDITOR_CONFIG]").c_str())) {
                    drawConfigTab();
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }

        ImGui::End();
    }

    void InputScalarBE(const char* label, ImGuiDataType dataType, void* pData) {
        switch (dataType) {
        case ImGuiDataType_U16: {
            u16 temp = *(BE(u16)*)pData;
            if (ImGui::InputScalar(label, dataType, &temp)) {
                *(BE(u16)*)pData = temp;
            }
            break;
        }
        case ImGuiDataType_S16: {
            s16 temp = *(BE(s16)*)pData;
            if (ImGui::InputScalar(label, dataType, &temp)) {
                *(BE(s16)*)pData = temp;
            }
            break;
        }
        case ImGuiDataType_U32: {
            u32 temp = *(BE(u32)*)pData;
            if (ImGui::InputScalar(label, dataType, &temp)) {
                *(BE(u32)*)pData = temp;
            }
            break;
        }
        case ImGuiDataType_S32: {
            s32 temp = *(BE(s32)*)pData;
            if (ImGui::InputScalar(label, dataType, &temp)) {
                *(BE(s32)*)pData = temp;
            }
            break;
        }
        case ImGuiDataType_U64: {
            u64 temp = *(BE(u64)*)pData;
            if (ImGui::InputScalar(label, dataType, &temp)) {
                *(BE(u64)*)pData = temp;
            }
            break;
        }
        case ImGuiDataType_S64: {
            s64 temp = *(BE(s64)*)pData;
            if (ImGui::InputScalar(label, dataType, &temp)) {
                *(BE(s64)*)pData = temp;
            }
            break;
        }
        case ImGuiDataType_Float: {
            f32 temp = *(BE(f32)*)pData;
            if (ImGui::InputScalar(label, dataType, &temp)) {
                *(BE(f32)*)pData = temp;
            }
            break;
        }
        }
    }

    void genSelectItemComboBox(const char* label, u8& selectItemData) {
        dSv_player_status_a_c& statusA = dComIfGs_getSaveData()->getPlayer().getPlayerStatusA();
        dSv_player_item_c& item = dComIfGs_getSaveData()->getPlayer().getItem();

        int currentSlotNo = selectItemData;
        std::string defaultLabel =
            currentSlotNo != 0xFF
            ? slotItemLabel(currentSlotNo, item.mItems[currentSlotNo])
            : tx("[SAVE_EDITOR_NONE]");

        // TODO: live update equips
        if (ImGui::BeginCombo(label, defaultLabel.c_str())) {
            if (ImGui::Selectable(tx("[SAVE_EDITOR_NONE]").c_str())) {
                selectItemData = 0xFF;
            }

            for (int i = 0; i < 24; i++) {
                u8 itemNo = item.mItems[i];
                if (ImGui::Selectable(slotItemLabel(i, itemNo).c_str())) {
                    selectItemData = i;
                }
            }
            ImGui::EndCombo();
        }
    }

    void ImGuiSaveEditor::drawPlayerStatusTab() {
        const char* playerName = dComIfGs_getPlayerName();
        ImGui::Text(tx("[SAVE_EDITOR_PLAYER_NAME]").c_str());
        ImGui::SameLine();
        char nameBuffer[8];
        snprintf(nameBuffer, sizeof(nameBuffer), "%s", playerName);
        if (ImGui::InputText("##PlayerNameInput", nameBuffer, 8)) {
            SAFE_STRCPY(dComIfGs_getPlayerName(), nameBuffer);
        }

        const char* horseName = dComIfGs_getHorseName();
        ImGui::Text(tx("[SAVE_EDITOR_HORSE_NAME]").c_str());
        ImGui::SameLine();
        char horseNameBuffer[8];
        snprintf(horseNameBuffer, sizeof(horseNameBuffer), "%s", horseName);
        if (ImGui::InputText("##HorseNameInput", horseNameBuffer, 8)) {
            SAFE_STRCPY(dComIfGs_getHorseName(), horseNameBuffer);
        }

        ImGui::Separator();

        dSv_player_status_a_c& statusA = dComIfGs_getSaveData()->getPlayer().getPlayerStatusA();
        dSv_player_status_b_c& statusB = dComIfGs_getSaveData()->getPlayer().getPlayerStatusB();

        InputScalarBE(tx("[SAVE_EDITOR_MAX_HEALTH]").c_str(), ImGuiDataType_U16, &statusA.mMaxLife);
        InputScalarBE(tx("[SAVE_EDITOR_HEALTH]").c_str(), ImGuiDataType_U16, &statusA.mLife);
        InputScalarBE(tx("[SAVE_EDITOR_RUPEES]").c_str(), ImGuiDataType_U16, &statusA.mRupee);
        InputScalarBE(tx("[SAVE_EDITOR_MAX_OIL]").c_str(), ImGuiDataType_U16, &statusA.mMaxOil);
        InputScalarBE(tx("[SAVE_EDITOR_OIL]").c_str(), ImGuiDataType_U16, &statusA.mOil);

        genSelectItemComboBox(tx("[SAVE_EDITOR_EQUIP_X]").c_str(), statusA.mSelectItem[0]);
        genSelectItemComboBox(tx("[SAVE_EDITOR_EQUIP_Y]").c_str(), statusA.mSelectItem[1]);
        genSelectItemComboBox(tx("[SAVE_EDITOR_COMBO_EQUIP_X]").c_str(), statusA.mMixItem[0]);
        genSelectItemComboBox(tx("[SAVE_EDITOR_COMBO_EQUIP_Y]").c_str(), statusA.mMixItem[1]);


        if (ImGui::BeginCombo(tx("[SAVE_EDITOR_CLOTHES]").c_str(), itemName(statusA.mSelectEquip[0]).c_str())) {
            if (ImGui::Selectable(tx("[SAVE_EDITOR_ORDON_CLOTHES]").c_str())) {
                dMeter2Info_setCloth(dItemNo_WEAR_CASUAL_e, false);
                daPy_getPlayerActorClass()->setClothesChange(0);
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_HERO_S_CLOTHES]").c_str())) {
                dMeter2Info_setCloth(dItemNo_WEAR_KOKIRI_e, false);
                daPy_getPlayerActorClass()->setClothesChange(0);
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_ZORA_ARMOR]").c_str())) {
                dMeter2Info_setCloth(dItemNo_WEAR_ZORA_e, false);
                daPy_getPlayerActorClass()->setClothesChange(0);
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_MAGIC_ARMOR]").c_str())) {
                dMeter2Info_setCloth(dItemNo_ARMOR_e, false);
                daPy_getPlayerActorClass()->setClothesChange(0);
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo(tx("[SAVE_EDITOR_SWORD]").c_str(), itemName(statusA.mSelectEquip[1]).c_str())) {
            if (ImGui::Selectable(tx("[SAVE_EDITOR_NONE]").c_str())) {
                statusA.mSelectEquip[1] = dItemNo_NONE_e;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_WOODEN_SWORD]").c_str())) {
                statusA.mSelectEquip[1] = dItemNo_WOOD_STICK_e;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_ORDON_SWORD]").c_str())) {
                statusA.mSelectEquip[1] = dItemNo_SWORD_e;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_MASTER_SWORD]").c_str())) {
                statusA.mSelectEquip[1] = dItemNo_MASTER_SWORD_e;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_LIGHT_SWORD]").c_str())) {
                statusA.mSelectEquip[1] = dItemNo_LIGHT_SWORD_e;
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo(tx("[SAVE_EDITOR_SHIELD]").c_str(), itemName(statusA.mSelectEquip[2]).c_str())) {
            if (ImGui::Selectable(tx("[SAVE_EDITOR_NONE]").c_str())) {
                statusA.mSelectEquip[2] = dItemNo_NONE_e;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_WOODEN_SHIELD]").c_str())) {
                statusA.mSelectEquip[2] = dItemNo_SHIELD_e;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_ORDON_SHIELD]").c_str())) {
                statusA.mSelectEquip[2] = dItemNo_WOOD_SHIELD_e;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_HYLIAN_SHIELD]").c_str())) {
                statusA.mSelectEquip[2] = dItemNo_HYLIA_SHIELD_e;
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo(tx("[SAVE_EDITOR_SCENT]").c_str(), itemName(statusA.mSelectEquip[3]).c_str())) {
            if (ImGui::Selectable(tx("[SAVE_EDITOR_NONE]").c_str())) {
                statusA.mSelectEquip[3] = dItemNo_NONE_e;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_YOUTH_S_SCENT]").c_str())) {
                statusA.mSelectEquip[3] = dItemNo_SMELL_CHILDREN_e;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_SCENT_OF_ILIA]").c_str())) {
                statusA.mSelectEquip[3] = dItemNo_SMELL_YELIA_POUCH_e;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_POE_SCENT]").c_str())) {
                statusA.mSelectEquip[3] = dItemNo_SMELL_POH_e;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_REEKFISH_SCENT]").c_str())) {
                statusA.mSelectEquip[3] = dItemNo_SMELL_FISH_e;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_MEDICINE_SCENT]").c_str())) {
                statusA.mSelectEquip[3] = dItemNo_SMELL_MEDICINE_e;
            }
            ImGui::EndCombo();
        }

        const char* walletSizeNames[] = {
            "[SAVE_EDITOR_NORMAL]",
            "[SAVE_EDITOR_BIG]",
            "[SAVE_EDITOR_GIANT]",
        };
        int walletSize = statusA.getWalletSize();
        if (ImGui::BeginCombo(tx("[SAVE_EDITOR_WALLET_SIZE]").c_str(), tx(walletSizeNames[walletSize]).c_str())) {
            if (ImGui::Selectable(tx(walletSizeNames[WALLET]).c_str())) {
                statusA.setWalletSize(WALLET);
            }
            if (ImGui::Selectable(tx(walletSizeNames[BIG_WALLET]).c_str())) {
                statusA.setWalletSize(BIG_WALLET);
            }
            if (ImGui::Selectable(tx(walletSizeNames[GIANT_WALLET]).c_str())) {
                statusA.setWalletSize(GIANT_WALLET);
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo(tx("[SAVE_EDITOR_FORM]").c_str(), tx(statusA.mTransformStatus == 0 ? "[SAVE_EDITOR_HUMAN]" : "[SAVE_EDITOR_WOLF]").c_str())) {
            if (ImGui::Selectable(tx("[SAVE_EDITOR_HUMAN]").c_str())) {
                statusA.mTransformStatus = TF_STATUS_HUMAN;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_WOLF]").c_str())) {
                statusA.mTransformStatus = TF_STATUS_WOLF;
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();

        s32 hours = dKy_getdaytime_hour();
        s32 min = dKy_getdaytime_minute();
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 2);
        if (ImGui::InputScalar("##TimeHours", ImGuiDataType_S32, &hours)) {
            hours = std::clamp(hours, 0, 23);
            statusB.setTime((hours * 15.0f) + (min / 60.0f * 15.0f));
        }

        ImGui::SameLine();
        ImGui::Text(":");
        ImGui::SameLine();

        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 2);
        if (ImGui::InputScalar(txId("[SAVE_EDITOR_TIME]", "##TimeMinutes").c_str(), ImGuiDataType_S32, &min)) {
            min = std::clamp(min, 0, 59);
            statusB.setTime((hours * 15.0f) + (min / 60.0f * 15.0f));
        }

        InputScalarBE(tx("[SAVE_EDITOR_DATE]").c_str(), ImGuiDataType_U16, &statusB.mDate);

        int transformLevel = 0;
        for (int i = 0; i < 4; i++) {
            if (statusB.mTransformLevelFlag & (1 << i)) {
                transformLevel++;
            }
        }
        if (ImGui::SliderInt(tx("[SAVE_EDITOR_TRANSFORM_LEVEL]").c_str(), &transformLevel, 0, 4)) {
            u8 newFlags = 0;
            for (int i = 0; i < transformLevel; i++) {
                newFlags |= (1 << i);
            }
            statusB.mTransformLevelFlag = newFlags;
        }

        int darkClearLevel = 0;
        for (int i = 0; i < 4; i++) {
            if (statusB.mDarkClearLevelFlag & (1 << i)) {
                darkClearLevel++;
            }
        }
        if (ImGui::SliderInt(tx("[SAVE_EDITOR_TWILIGHT_CLEAR_LEVEL]").c_str(), &darkClearLevel, 0, 3)) {
            u8 newFlags = 0;
            for (int i = 0; i < darkClearLevel; i++) {
                newFlags |= (1 << i);
            }
            statusB.mDarkClearLevelFlag = newFlags;
        }
    }

    void ImGuiSaveEditor::drawLocationTab() {
        dSv_player_return_place_c& returnPlace = dComIfGs_getSaveData()->getPlayer().getPlayerReturnPlace();
        dSv_horse_place_c& horsePlace = dComIfGs_getSaveData()->getPlayer().getHorsePlace();
        ImGui::Text(tx("[SAVE_EDITOR_SAVE_LOCATION]").c_str());

        ImGui::Text(tx("[SAVE_EDITOR_STAGE]").c_str());
        ImGui::SameLine();
        char nameBuffer[8];
        snprintf(nameBuffer, sizeof(nameBuffer), "%s", returnPlace.mName);
        if (ImGui::InputText("##SaveStageNameInput", nameBuffer, sizeof(nameBuffer))) {
            SAFE_STRCPY(returnPlace.mName, nameBuffer);
        }

        ImGui::Text(tx("[SAVE_EDITOR_ROOM]").c_str());
        ImGui::SameLine();
        int tempRoom = returnPlace.mRoomNo;
        if (ImGui::InputInt("##SaveRoomInput", &tempRoom)) {
            returnPlace.mRoomNo = tempRoom;
        }

        ImGui::Text(tx("[SAVE_EDITOR_SPAWN_ID]").c_str());
        ImGui::SameLine();
        int tempSpawn = returnPlace.mPlayerStatus;
        if (ImGui::InputInt("##SaveSpawnInput", &tempSpawn)) {
            returnPlace.mPlayerStatus = tempSpawn;
        }

        ImGui::Separator();

        ImGui::Text(tx("[SAVE_EDITOR_HORSE_LOCATION]").c_str());

        ImGui::Text(tx("[SAVE_EDITOR_POSITION]").c_str());
        ImGui::SameLine();
        Vec tempPos = horsePlace.mPos;
        if (ImGui::InputFloat3("##HorsePosition", &tempPos.x)) {
            horsePlace.mPos.x = tempPos.x;
            horsePlace.mPos.y = tempPos.y;
            horsePlace.mPos.z = tempPos.z;
        }

        ImGui::Text(tx("[SAVE_EDITOR_ANGLE]").c_str());
        ImGui::SameLine();
        int tempAngle = horsePlace.mAngleY;
        if (ImGui::InputInt("##HorsePosition", &tempAngle)) {
            horsePlace.mAngleY = tempAngle;
        }

        ImGui::Text(tx("[SAVE_EDITOR_STAGE]").c_str());
        ImGui::SameLine();
        char horseStageBuffer[8];
        snprintf(horseStageBuffer, sizeof(horseStageBuffer), "%s", horsePlace.mName);
        if (ImGui::InputText("##HorseStageNameInput", horseStageBuffer, sizeof(horseStageBuffer))) {
            SAFE_STRCPY(horsePlace.mName, horseStageBuffer);
        }

        ImGui::Text(tx("[SAVE_EDITOR_ROOM]").c_str());
        ImGui::SameLine();
        int tempHorseRoom = horsePlace.mRoomNo;
        if (ImGui::InputInt("##HorseRoomInput", &tempHorseRoom)) {
            horsePlace.mRoomNo = tempHorseRoom;
        }

        ImGui::Text(tx("[SAVE_EDITOR_SPAWN_ID]").c_str());
        ImGui::SameLine();
        int tempHorseSpawn = horsePlace.mSpawnId;
        if (ImGui::InputInt("##HorseSpawnInput", &tempHorseSpawn)) {
            horsePlace.mSpawnId = tempHorseSpawn;
        }
    }

    static u8 getSlotDefault(int slot) {
        for (size_t i = 0; i < sizeof(sDefaultInventory) / sizeof(sDefaultInventory[0]); i++) {
            if (sDefaultInventory[i].slot == slot) {
                return sDefaultInventory[i].item;
            }
        }
        return dItemNo_NONE_e;
    }

    void ImGuiSaveEditor::drawInventoryTab() {
        dSv_player_item_c& item = dComIfGs_getSaveData()->getPlayer().getItem();

        if (ImGui::TreeNode(tx("[SAVE_EDITOR_ITEM_WHEEL]").c_str())) {
            if (ImGui::Button(txId("[SAVE_EDITOR_DEFAULT_ALL]", "##inv_default_all").c_str())) {
                for (int slot = 0; slot < 24; slot++) {
                    dComIfGs_setItem(slot, getSlotDefault(slot));
                }
            }
            ImGui::SameLine();
            if (ImGui::Button(txId("[SAVE_EDITOR_CLEAR_ALL]", "##inv_clear_all").c_str())) {
                for (int slot = 0; slot < 24; slot++) {
                    dComIfGs_setItem(slot, dItemNo_NONE_e);
                }
            }

            ImGuiBeginGroupPanel(tx("[SAVE_EDITOR_ITEMS]").c_str(), { 200, 100 });
            for (int slot = 0; slot < 24; slot++) {
                ImGuiStringViewText(slotItemLabel(slot, getSlotDefault(slot)) + ": ");
                ImGui::SameLine(240.0f);
                if (ImGui::BeginCombo(fmt::format("##ItemComboBox{}", slot).c_str(), itemName(item.mItems[slot]).c_str())) {
                    if (ImGui::Selectable(tx("[SAVE_EDITOR_NONE]").c_str())) {
                        dComIfGs_setItem(slot, dItemNo_NONE_e);
                    }

                    for (int i = 0; i < 254; i++) {
                        if (itemMap.find(i)->second.m_type != ITEMTYPE_EQUIP_e) continue;

                        if (ImGui::Selectable(fmt::format("{}##item_{}{}", itemName(i), slot, i).c_str())) {
                            dComIfGs_setItem(slot, itemMap.find(i)->first);
                        }
                    }
                    ImGui::EndCombo();
                }
                ImGui::SameLine();
                if (ImGui::SmallButton(fmt::format("{}##slot_d_{}", tx("[SAVE_EDITOR_DEFAULT]"), slot).c_str())) {
                    dComIfGs_setItem(slot, getSlotDefault(slot));
                }
                ImGui::SameLine();
                if (ImGui::SmallButton(fmt::format("{}##slot_c_{}", tx("[SAVE_EDITOR_CLEAR]"), slot).c_str())) {
                    dComIfGs_setItem(slot, dItemNo_NONE_e);
                }
            }
            ImGuiEndGroupPanel();

            ImGui::TreePop();
        }

        if (ImGui::TreeNode(tx("[SAVE_EDITOR_GET_ITEM_FLAGS]").c_str())) {
            for (int i = 0; i < 254; i++) {
                if (itemMap.find(i)->second.m_name == "[SAVE_EDITOR_RESERVED]") continue;

                bool flag = dComIfGs_isItemFirstBit(i);
                if (ImGui::Checkbox(fmt::format("{}##item_{}", itemName(i), i).c_str(), &flag)) {
                    if (flag)
                        dComIfGs_onItemFirstBit(i);
                    else
                        dComIfGs_offItemFirstBit(i);
                }
            }

            ImGui::TreePop();
        }

        dSv_player_item_record_c& itemRecord = dComIfGs_getSaveData()->getPlayer().getItemRecord();
        dSv_player_item_max_c& itemMax = dComIfGs_getSaveData()->getPlayer().getItemMax();

        ImGuiBeginGroupPanel(tx("[SAVE_EDITOR_ITEM_MAX_CAPACITIES]").c_str(), { 200, 100 });
        ImGui::InputScalar(tx("[SAVE_EDITOR_ARROWS_MAX]").c_str(), ImGuiDataType_U8, &itemMax.mItemMax[0]);
        ImGui::InputScalar(tx("[SAVE_EDITOR_NORMAL_BOMBS_MAX]").c_str(), ImGuiDataType_U8, &itemMax.mItemMax[1]);
        ImGui::InputScalar(tx("[SAVE_EDITOR_WATER_BOMBS_MAX]").c_str(), ImGuiDataType_U8, &itemMax.mItemMax[2]);
        ImGui::InputScalar(tx("[SAVE_EDITOR_BOMBLINGS_MAX]").c_str(), ImGuiDataType_U8, &itemMax.mItemMax[3]);
        ImGuiEndGroupPanel();

        ImGuiBeginGroupPanel(tx("[SAVE_EDITOR_ITEM_AMOUNTS]").c_str(), { 200, 100 });
        ImGui::InputScalar(tx("[SAVE_EDITOR_ARROWS_AMOUNT]").c_str(), ImGuiDataType_U8, &itemRecord.mArrowNum);
        ImGui::InputScalar(tx("[SAVE_EDITOR_SLINGSHOT_AMOUNT]").c_str(), ImGuiDataType_U8, &itemRecord.mPachinkoNum);
        ImGui::InputScalar(tx("[SAVE_EDITOR_BOMB_BAG_1_AMOUNT]").c_str(), ImGuiDataType_U8, &itemRecord.mBombNum[0]);
        ImGui::InputScalar(tx("[SAVE_EDITOR_BOMB_BAG_2_AMOUNT]").c_str(), ImGuiDataType_U8, &itemRecord.mBombNum[1]);
        ImGui::InputScalar(tx("[SAVE_EDITOR_BOMB_BAG_3_AMOUNT]").c_str(), ImGuiDataType_U8, &itemRecord.mBombNum[2]);
        ImGui::InputScalar(tx("[SAVE_EDITOR_BOTTLE_1_AMOUNT]").c_str(), ImGuiDataType_U8, &itemRecord.mBottleNum[0]);
        ImGui::InputScalar(tx("[SAVE_EDITOR_BOTTLE_2_AMOUNT]").c_str(), ImGuiDataType_U8, &itemRecord.mBottleNum[1]);
        ImGui::InputScalar(tx("[SAVE_EDITOR_BOTTLE_3_AMOUNT]").c_str(), ImGuiDataType_U8, &itemRecord.mBottleNum[2]);
        ImGui::InputScalar(tx("[SAVE_EDITOR_BOTTLE_4_AMOUNT]").c_str(), ImGuiDataType_U8, &itemRecord.mBottleNum[3]);
        ImGuiEndGroupPanel();
    }

    static inline void setItemFirstBit(u8 itemNo, bool owned) {
        if (owned) {
            dComIfGs_onItemFirstBit(itemNo);
        } else {
            dComIfGs_offItemFirstBit(itemNo);
        }
    }

    static inline void setEventBit(u16 flag, bool on) {
        if (on) {
            dComIfGs_onEventBit(flag);
        } else {
            dComIfGs_offEventBit(flag);
        }
    }

    static void setLetterGetFlag(int idx, bool received) {
        dSv_letter_info_c& info = dComIfGs_getSaveData()->getPlayer().getLetterInfo();
        if (received) {
            if (dComIfGs_isLetterGetFlag(idx)) return;
            dComIfGs_onLetterGetFlag(idx);
            u8 slot = dMeter2Info_getRecieveLetterNum() - 1;
            if (slot < 64) {
                dComIfGs_setGetNumber(slot, (u8)(idx + 1));
            }
        } else {
            if (!dComIfGs_isLetterGetFlag(idx)) return;
            info.mLetterGetFlags[idx >> 5] &= ~(1u << (idx & 0x1F));
            for (int j = 0; j < 64; j++) {
                if (dComIfGs_getGetNumber(j) == idx + 1) {
                    for (int k = j; k < 63; k++) {
                        dComIfGs_setGetNumber(k, dComIfGs_getGetNumber(k + 1));
                    }
                    dComIfGs_setGetNumber(63, 0);
                    break;
                }
            }
        }
    }

    void ImGuiSaveEditor::drawCollectionTab() {
        if (ImGui::TreeNode(tx("[SAVE_EDITOR_EQUIPMENT]").c_str())) {
            if (ImGui::TreeNode(tx("[SAVE_EDITOR_SWORDS]").c_str())) {
                static u8 sword_list[] = {
                    dItemNo_SWORD_e,
                    dItemNo_MASTER_SWORD_e,
                    dItemNo_WOOD_STICK_e,
                    dItemNo_LIGHT_SWORD_e,
                };

                for (int i = 0; i < 4; i++) {
                    bool got = dComIfGs_isItemFirstBit(sword_list[i]) != 0;
                    if (ImGui::Checkbox(.c_str(), &got)) {
                        if (got) dComIfGs_onItemFirstBit(sword_list[i]);
                        else     dComIfGs_offItemFirstBit(sword_list[i]);
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode(tx("[SAVE_EDITOR_SHIELDS]").c_str())) {
                static u8 shield_list[] = {
                    dItemNo_SHIELD_e,
                    dItemNo_WOOD_SHIELD_e,
                    dItemNo_HYLIA_SHIELD_e,
                };

                for (int i = 0; i < 3; i++) {
                    bool got = dComIfGs_isItemFirstBit(shield_list[i]) != 0;
                    if (ImGui::Checkbox(.c_str(), &got)) {
                        if (got) dComIfGs_onItemFirstBit(shield_list[i]);
                        else     dComIfGs_offItemFirstBit(shield_list[i]);
                    }
                }
                ImGui::TreePop();
            }

            // TODO: the game checks if you're in ranch clothes, and if so won't display any other tunics in the menu
            // find a way to deal with this later
            if (ImGui::TreeNode(tx("[SAVE_EDITOR_TUNICS]").c_str())) {
                bool ordonClothes = dComIfGs_isItemFirstBit(dItemNo_WEAR_CASUAL_e) != 0;
                if (ImGui::Checkbox(txId("[SAVE_EDITOR_ORDON_CLOTHES]", "##tunic_ordon").c_str(), &ordonClothes)) {
                    setItemFirstBit(dItemNo_WEAR_CASUAL_e, ordonClothes);
                }

                bool greenTunic = dComIfGs_isCollectClothes(KOKIRI_CLOTHES_FLAG) != 0;
                if (ImGui::Checkbox(txId("[SAVE_EDITOR_HERO_S_CLOTHES]", "##tunic_green").c_str(), &greenTunic)) {
                    if (greenTunic) dComIfGs_setCollectClothes(KOKIRI_CLOTHES_FLAG);
                    else            dComIfGs_offCollectClothes(KOKIRI_CLOTHES_FLAG);
                }

                bool zoraArmor = dComIfGs_isItemFirstBit(dItemNo_WEAR_ZORA_e) != 0;
                if (ImGui::Checkbox(txId("[SAVE_EDITOR_ZORA_ARMOR]", "##tunic_zora").c_str(), &zoraArmor)) {
                    setItemFirstBit(dItemNo_WEAR_ZORA_e, zoraArmor);
                }

                bool magicArmor = dComIfGs_isItemFirstBit(dItemNo_ARMOR_e) != 0;
                if (ImGui::Checkbox(txId("[SAVE_EDITOR_MAGIC_ARMOR]", "##tunic_magic").c_str(), &magicArmor)) {
                    setItemFirstBit(dItemNo_ARMOR_e, magicArmor);
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(tx("[SAVE_EDITOR_KEY_ITEMS]").c_str())) {
            if (ImGui::TreeNode(tx("[SAVE_EDITOR_FUSED_SHADOWS]").c_str())) {
                for (int i = 0; i < 3; i++) {
                    bool got = dComIfGs_isCollectCrystal((u8)i) != 0;
                    if (ImGui::Checkbox(
                            fmt::format("{}##fs_{}", tx(sFusedShadowNames[i]), i).c_str(), &got)) {
                        if (got) dComIfGs_onCollectCrystal((u8)i);
                        else     dComIfGs_offCollectCrystal((u8)i);
                    }
                }
                ImGui::Spacing();
                if (ImGui::Button(txId("[SAVE_EDITOR_ALL]", "##fs_all").c_str())) {
                    for (int i = 0; i < 3; i++) dComIfGs_onCollectCrystal((u8)i);
                }
                ImGui::SameLine();
                if (ImGui::Button(txId("[SAVE_EDITOR_NONE]", "##fs_clear").c_str())) {
                    for (int i = 0; i < 3; i++) dComIfGs_offCollectCrystal((u8)i);
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode(tx("[SAVE_EDITOR_MIRROR_SHARDS]").c_str())) {
                for (int i = 0; i < 3; i++) {
                    u8 idx  = sMirrorShards[i].index;
                    bool got = dComIfGs_isCollectMirror(idx) != 0;
                    if (ImGui::Checkbox(
                            fmt::format("{}##ms_{}", tx(sMirrorShards[i].name), i).c_str(), &got)) {
                        if (got) dComIfGs_onCollectMirror(idx);
                        else     dComIfGs_offCollectMirror(idx);
                    }
                }
                ImGui::Spacing();
                if (ImGui::Button(txId("[SAVE_EDITOR_ALL]", "##ms_all").c_str())) {
                    for (int i = 0; i < 3; i++) dComIfGs_onCollectMirror(sMirrorShards[i].index);
                }
                ImGui::SameLine();
                if (ImGui::Button(txId("[SAVE_EDITOR_NONE]", "##ms_clear").c_str())) {
                    for (int i = 0; i < 3; i++) dComIfGs_offCollectMirror(sMirrorShards[i].index);
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(tx("[SAVE_EDITOR_HEART_PIECES_POE_SOULS]").c_str())) {
            if (ImGui::TreeNode(tx("[SAVE_EDITOR_POE_SOULS]").c_str())) {
                int poeCount = dComIfGs_getPohSpiritNum();
                ImGui::Text(tx("[SAVE_EDITOR_COLLECTED]").c_str());
                ImGui::SameLine();
                ImGui::SetNextItemWidth(100.0f);
                if (ImGui::InputInt("##poe_count", &poeCount)) {
                    if (poeCount < 0) poeCount = 0;
                    if (poeCount > 60) poeCount = 60;
                    dComIfGs_setPohSpiritNum((u8)poeCount);
                }
                ImGui::SameLine();
                ImGui::TextDisabled("/ 60");
                ImGui::Spacing();
                if (ImGui::Button(txId("[SAVE_EDITOR_ALL_60]", "##poe_all").c_str())) {
                    dComIfGs_setPohSpiritNum(60);
                }
                ImGui::SameLine();
                if (ImGui::Button(txId("[SAVE_EDITOR_CLEAR]", "##poe_clear").c_str())) {
                    dComIfGs_setPohSpiritNum(0);
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode(tx("[SAVE_EDITOR_HEART_PIECES]").c_str())) {
                int maxLife = dComIfGs_getMaxLife();
                int hearts  = maxLife / 5;
                int pieces  = maxLife % 5;
                ImGui::Text(tx("[SAVE_EDITOR_MAX_LIFE]").c_str());
                ImGui::SameLine();
                ImGui::SetNextItemWidth(160.0f);
                if (ImGui::InputInt("##max_life", &maxLife, 1, 5)) {
                    if (maxLife < 15)  maxLife = 15;
                    if (maxLife > 100) maxLife = 100;
                    dComIfGs_setMaxLife((u8)maxLife);
                    u16 maxHealth = (dComIfGs_getMaxLife() / 5) * 4;
                    if (dComIfGs_getLife() > maxHealth) {
                        dComIfGs_setLife(maxHealth);
                    }
                }
                ImGui::SameLine();
                ImGui::TextDisabled(tx("[SAVE_EDITOR_D_HEARTS_D_PIECES]").c_str(), hearts, pieces);
                ImGui::Spacing();
                if (ImGui::Button(txId("[SAVE_EDITOR_3_HEARTS]", "##hp_min").c_str())) {
                    dComIfGs_setMaxLife(15);
                    dComIfGs_setLife(12);
                }
                ImGui::SameLine();
                if (ImGui::Button(txId("[SAVE_EDITOR_20_HEARTS]", "##hp_max").c_str())) {
                    dComIfGs_setMaxLife(100);
                    dComIfGs_setLife(80);
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(tx("[SAVE_EDITOR_GOLDEN_BUGS]").c_str())) {
            if (ImGui::BeginTable("GoldenBugTable", 5,
                                  ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn(tx("[SAVE_EDITOR_SPECIES]").c_str());
                ImGui::TableSetupColumn(tx("[SAVE_EDITOR_MALE]").c_str());
                ImGui::TableSetupColumn(tx("[SAVE_EDITOR_FEMALE]").c_str());
                ImGui::TableSetupColumn(tx("[SAVE_EDITOR_M_TO_AGITHA]").c_str());
                ImGui::TableSetupColumn(tx("[SAVE_EDITOR_F_TO_AGITHA]").c_str());
                ImGui::TableHeadersRow();

                for (int species = 0; species < BUG_SPECIES_COUNT; species++) {
                    int maleIdx   = species * 2;
                    int femaleIdx = species * 2 + 1;
                    u8 maleItem   = sBugItemIds[maleIdx];
                    u8 femaleItem = sBugItemIds[femaleIdx];
                    u16 maleFlag   = sBugTurnInFlags[maleIdx];
                    u16 femaleFlag = sBugTurnInFlags[femaleIdx];

                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGuiStringViewText(tx(sBugSpeciesNames[species]));

                    ImGui::TableSetColumnIndex(1);
                    bool maleOwned = dComIfGs_isItemFirstBit(maleItem) != 0;
                    if (ImGui::Checkbox(fmt::format("##bugM_own_{}", species).c_str(), &maleOwned)) {
                        setItemFirstBit(maleItem, maleOwned);
                    }

                    ImGui::TableSetColumnIndex(2);
                    bool femaleOwned = dComIfGs_isItemFirstBit(femaleItem) != 0;
                    if (ImGui::Checkbox(fmt::format("##bugF_own_{}", species).c_str(), &femaleOwned)) {
                        setItemFirstBit(femaleItem, femaleOwned);
                    }

                    ImGui::TableSetColumnIndex(3);
                    bool maleGiven = dComIfGs_isEventBit(maleFlag) != 0;
                    if (ImGui::Checkbox(fmt::format("##bugM_giv_{}", species).c_str(), &maleGiven)) {
                        setEventBit(maleFlag, maleGiven);
                    }

                    ImGui::TableSetColumnIndex(4);
                    bool femaleGiven = dComIfGs_isEventBit(femaleFlag) != 0;
                    if (ImGui::Checkbox(fmt::format("##bugF_giv_{}", species).c_str(), &femaleGiven)) {
                        setEventBit(femaleFlag, femaleGiven);
                    }
                }

                ImGui::EndTable();
            }

            ImGui::Spacing();
            if (ImGui::Button(txId("[SAVE_EDITOR_COLLECT_ALL]", "##bugs_all").c_str())) {
                for (int i = 0; i < BUG_SPECIES_COUNT * 2; i++) {
                    dComIfGs_onItemFirstBit(sBugItemIds[i]);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button(txId("[SAVE_EDITOR_CLEAR_ALL]", "##bugs_clear").c_str())) {
                for (int i = 0; i < BUG_SPECIES_COUNT * 2; i++) {
                    dComIfGs_offItemFirstBit(sBugItemIds[i]);
                    dComIfGs_offEventBit(sBugTurnInFlags[i]);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button(txId("[SAVE_EDITOR_GIVE_ALL_TO_AGITHA]", "##bugs_giveall").c_str())) {
                for (int i = 0; i < BUG_SPECIES_COUNT * 2; i++) {
                    dComIfGs_onEventBit(sBugTurnInFlags[i]);
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(tx("[SAVE_EDITOR_HIDDEN_SKILLS]").c_str())) {
            for (int i = 0; i < HIDDEN_SKILL_COUNT; i++) {
                bool learned = dComIfGs_isEventBit(sHiddenSkillFlags[i]) != 0;
                if (ImGui::Checkbox(
                        fmt::format("{}##skill_{}", tx(sHiddenSkillNames[i]), i).c_str(), &learned)) {
                    setEventBit(sHiddenSkillFlags[i], learned);
                }
            }
            ImGui::Spacing();
            if (ImGui::Button(txId("[SAVE_EDITOR_LEARN_ALL]", "##skills_all").c_str())) {
                for (int i = 0; i < HIDDEN_SKILL_COUNT; i++) {
                    dComIfGs_onEventBit(sHiddenSkillFlags[i]);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button(txId("[SAVE_EDITOR_FORGET_ALL]", "##skills_clear").c_str())) {
                for (int i = 0; i < HIDDEN_SKILL_COUNT; i++) {
                    dComIfGs_offEventBit(sHiddenSkillFlags[i]);
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(tx("[SAVE_EDITOR_COLLECTION_LOGS]").c_str())) {
            if (ImGui::TreeNode(tx("[SAVE_EDITOR_POSTMAN_LETTERS]").c_str())) {
                for (int i = 0; i < LETTER_COUNT; i++) {
                    bool had = dComIfGs_isLetterGetFlag(i) != 0;
                    if (ImGui::Checkbox(
                            fmt::format("{}##letter_{}", tx(sLetterSenders[i]), i).c_str(), &had)) {
                        setLetterGetFlag(i, had);
                    }
                }
                ImGui::Spacing();
                if (ImGui::Button(txId("[SAVE_EDITOR_RECEIVE_ALL]", "##letters_all").c_str())) {
                    for (int i = 0; i < LETTER_COUNT; i++) setLetterGetFlag(i, true);
                }
                ImGui::SameLine();
                if (ImGui::Button(txId("[SAVE_EDITOR_CLEAR_ALL]", "##letters_clear").c_str())) {
                    for (int i = 0; i < LETTER_COUNT; i++) setLetterGetFlag(i, false);
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode(tx("[SAVE_EDITOR_FISHING_LOG]").c_str())) {
                dSv_fishing_info_c& fish = dComIfGs_getSaveData()->getPlayer().getFishingInfo();
                if (ImGui::BeginTable("FishTable", 3,
                                      ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg)) {
                    ImGui::TableSetupColumn(tx("[SAVE_EDITOR_SPECIES]").c_str());
                    ImGui::TableSetupColumn(tx("[SAVE_EDITOR_CAUGHT]").c_str());
                    ImGui::TableSetupColumn(tx("[SAVE_EDITOR_BIGGEST_CM]").c_str());
                    ImGui::TableHeadersRow();

                    for (int i = 0; i < FISH_COUNT; i++) {
                        u8 idx = sFishSpecies[i].index;
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGuiStringViewText(tx(sFishSpecies[i].name));

                        ImGui::TableSetColumnIndex(1);
                        int count = dComIfGs_getFishNum(idx);
                        ImGui::SetNextItemWidth(100.0f);
                        if (ImGui::InputInt(fmt::format("##fish_c_{}", i).c_str(), &count, 1, 10)) {
                            if (count < 0) count = 0;
                            if (count > 999) count = 999;
                            fish.mFishCount[idx] = (u16)count;
                        }

                        ImGui::TableSetColumnIndex(2);
                        int size = dComIfGs_getFishSize(idx);
                        ImGui::SetNextItemWidth(100.0f);
                        if (ImGui::InputInt(fmt::format("##fish_s_{}", i).c_str(), &size, 1, 10)) {
                            if (size < 0) size = 0;
                            if (size > 255) size = 255;
                            dComIfGs_setFishSize(idx, (u8)size);
                        }
                    }
                    ImGui::EndTable();
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }

    void drawFlagList(const char* id, BE(u32)& flags) {
        u32 tempFlagField = flags;

        for (int i = 31; i >= 0; i--) {
            if ((31 - i) % 8) {
                ImGui::SameLine();
            }

            bool flag = tempFlagField & (1 << i);
            if (ImGui::Checkbox(fmt::format("{0}{1}", id, i).c_str(), &flag)) {
                if (flag)
                    tempFlagField |= (1 << i);
                else
                    tempFlagField &= ~(1 << i);

                flags = tempFlagField;
            }
        }
    }

    static inline void genDungeonItemCheckbox(dSv_memBit_c& membit, const char* label, int flag) {
        bool tempFlag = membit.isDungeonItem(flag);
        if (ImGui::Checkbox(label, &tempFlag)) {
            if (tempFlag)
                membit.onDungeonItem(flag);
            else
                membit.offDungeonItem(flag);
        }
    }
    
    static void genCommonAreaFlags(dSv_memBit_c& membit) {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

        genDungeonItemCheckbox(membit, tx("[SAVE_EDITOR_GOT_MAP]").c_str(), dSv_memBit_c::MAP);
        ImGui::SameLine(230.0f);
        genDungeonItemCheckbox(membit, tx("[SAVE_EDITOR_GOT_COMPASS]").c_str(), dSv_memBit_c::COMPASS);

        genDungeonItemCheckbox(membit, tx("[SAVE_EDITOR_GOT_BOSS_KEY]").c_str(), dSv_memBit_c::BOSS_KEY);
        ImGui::SameLine(230.0f);
        genDungeonItemCheckbox(membit, tx("[SAVE_EDITOR_SAW_BOSS_DEMO]").c_str(), dSv_memBit_c::STAGE_BOSS_DEMO);

        genDungeonItemCheckbox(membit, tx("[SAVE_EDITOR_GOT_HEART_CONTAINER]").c_str(), dSv_memBit_c::STAGE_LIFE);
        ImGui::SameLine(230.0f);
        genDungeonItemCheckbox(membit, tx("[SAVE_EDITOR_DEFEATED_BOSS]").c_str(), dSv_memBit_c::STAGE_BOSS_ENEMY);

        genDungeonItemCheckbox(membit, tx("[SAVE_EDITOR_DEFEATED_MINIBOSS]").c_str(), dSv_memBit_c::STAGE_BOSS_ENEMY_2);
        ImGui::SameLine(230.0f);
        genDungeonItemCheckbox(membit, tx("[SAVE_EDITOR_GOT_OOCCOO]").c_str(), dSv_memBit_c::OOCCOO_NOTE);

        int keyTemp = membit.getKeyNum();
        if (ImGui::SliderInt(tx("[SAVE_EDITOR_KEYS]").c_str(), &keyTemp, 0, 5)) {
            membit.setKeyNum(keyTemp);
        }
    }

    static void genMembitFlags(const char* id, dSv_memBit_c& membit) {
        ImGuiBeginGroupPanel(tx("[SAVE_EDITOR_CHEST]").c_str(), { 100, 100 });
        for (int j = 0; j < 2; j++) {
            drawFlagList(fmt::format("##_tbox{}", j).c_str(), membit.mTbox[j]);
        }
        ImGuiEndGroupPanel();

        ImVec2 post_tbox_cursor = ImGui::GetCursorPos();

        ImGui::SameLine();

        ImGuiBeginGroupPanel(tx("[SAVE_EDITOR_SWITCH]").c_str(), { 100, 100 });
        for (int j = 0; j < 4; j++) {
            drawFlagList(fmt::format("##_switch{}", j).c_str(), membit.mSwitch[j]);
        }
        ImGuiEndGroupPanel();

        ImVec2 post_switch_cursor = ImGui::GetCursorPos();

        ImGui::SetCursorPos(post_tbox_cursor);

        ImGuiBeginGroupPanel(tx("[SAVE_EDITOR_ITEM]").c_str(), { 100, 100 });
        for (int j = 0; j < 1; j++) {
            drawFlagList(fmt::format("##_item{}", j).c_str(), membit.mItem[j]);
        }
        ImGuiEndGroupPanel();
        ImVec2 post_item_custor = ImGui::GetCursorPos();

        ImGui::SetCursorPos({post_item_custor.x, post_switch_cursor.y});
        // genCommonAreaFlags(membit);
    }

    template <typename FlagIter, typename FlagTester>
    requires requires(FlagIter a, FlagTester tester) {
        --a; ++a; a < a; *a;
        a + 1;
        { tester(*a) } -> std::convertible_to<bool>;
    }
    static void sortByFlags(FlagIter begin, FlagIter end, FlagTester&& flagTester) {
        if (begin == end) return;

        auto fullEnd = end;

        // We want to find the location of where we can swap our `On` flags to.
        // We're gonna put the `Off` bits first, and the `On` bits last. 0 < 1
        // We can achieve this by skipping all the `On` bits at the end.

        // backtrack until we find a bit that is off
        while (begin < --end && flagTester(*end)) {
            // move the end pointer back while we find on bits
        }

        // end should now be pointing to a bit that is off
        while (begin < end) {
            // if there's a flag that's on
            if (flagTester(*begin)) {
                // move it to the end
                std::rotate(begin, begin + 1, fullEnd);
                // move back the end of where we're checking
                --end;
                // begin will now point to the next piece of data
                // because we've rotated the data >= begin to the left
            } else {
                // not on, check next flag
                ++begin;
            }
        }
    }

    
    static void genAreaFlagTable(uint8_t areaIndex, dSv_memBit_c& membit) {
        const auto LoadFlag = [&](const EventAreaFlags& flag) -> bool {
            switch (flag.flag.type) {
            case AreaFlagType::Item: {
                return membit.isItem(flag.flag.flagID);
            } break;
            case AreaFlagType::Switch: {
                return membit.isSwitch(flag.flag.flagID);
            } break;
            case AreaFlagType::Tbox: {
                return membit.isTbox(flag.flag.flagID);
            } break;
            }
            return false;
        };

        const auto SetFlag = [&](const AreaFlagInd& flag, bool set) -> void {
            if (set) {
                switch (flag.type) {
                case AreaFlagType::Item: {
                    membit.onItem(flag.flagID);
                } break;
                case AreaFlagType::Switch: {
                    membit.onSwitch(flag.flagID);
                } break;
                case AreaFlagType::Tbox: {
                    membit.onTbox(flag.flagID);
                } break;
                }
            } else {
                switch (flag.type) {
                case AreaFlagType::Item: {
                    membit.offItem(flag.flagID);
                } break;
                case AreaFlagType::Switch: {
                    membit.offSwitch(flag.flagID);
                } break;
                case AreaFlagType::Tbox: {
                    membit.offTbox(flag.flagID);
                } break;
                }
            }
        };

        const auto LoadMultiByteFlag = [&](const AreaFlagMultibit& flag) -> uint8_t {
            BE(u32)* areaFlags = nullptr;
            switch (flag.type) {
            case AreaFlagType::Item: {
                areaFlags = membit.mItem;
            } break;
            case AreaFlagType::Switch: {
                areaFlags = membit.mSwitch;
            } break;
            case AreaFlagType::Tbox: {
                areaFlags = membit.mTbox;
            } break;
            }
            assert(areaFlags != nullptr);
            return (areaFlags[flag.index] & flag.mask) >> flag.shift;
        };

        const auto SetMultiByteFlag = [&](const AreaFlagMultibit& flag, uint8_t val) -> void {
            BE(u32)* areaFlags = nullptr;
            switch (flag.type) {
            case AreaFlagType::Item: {
                areaFlags = membit.mItem;
            } break;
            case AreaFlagType::Switch: {
                areaFlags = membit.mSwitch;
            } break;
            case AreaFlagType::Tbox: {
                areaFlags = membit.mTbox;
            } break;
            }

            areaFlags[flag.index] &= ~flag.mask;
            areaFlags[flag.index] |= (val << flag.shift) & flag.mask;
        };

        auto iter = imguiAreaFlagLookup.find(areaIndex);
        if (iter == imguiAreaFlagLookup.end()) return;

        auto& areaFlags = iter->second;

        static ImGuiTextFilter filter;
        filter.Draw();  // Search bar

        ImVec2 flagTableSize = {700, 400};
        if (ImGui::BeginTable("Area Flags", 3,
                              ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX |
                                  ImGuiTableFlags_Sortable,
                              flagTableSize))
        {
            ImGui::TableSetupScrollFreeze(0, 1);
            constexpr int COLUMN_FLAG = 0, COLUMN_BIT = 1, COLUMN_DESC = 2;
            ImGui::TableSetupColumn(tx("[SAVE_EDITOR_FLAG]").c_str());
            ImGui::TableSetupColumn(tx("[SAVE_EDITOR_BYTE_BIT]").c_str());
            ImGui::TableSetupColumn(tx("[SAVE_EDITOR_DESCRIPTION]").c_str());
            ImGui::TableHeadersRow();

            // if we're sorting by whether the flag is set or not,
            // we want to re-sort whenever a flag updates, which means every frame cuz we don't
            // know when it changes. otherwise only re-sort when the sort is dirty
            if (auto* sort = ImGui::TableGetSortSpecs();
                sort != nullptr && sort->SpecsCount > 0 &&
                (sort->SpecsDirty || sort->Specs[0].ColumnIndex == COLUMN_FLAG))
            {
                const auto column = sort->Specs[0].ColumnIndex;
                const auto direction = sort->Specs[0].SortDirection;

                // if we're sorting by flags, do special sort, regular sort is bad for sorting
                // bools it can swap values that are the same, and that causes constant
                // reordering
                if (column == COLUMN_FLAG) {
                    if (direction == ImGuiSortDirection_Ascending) {
                        sortByFlags(std::begin(areaFlags.bitFlags), std::end(areaFlags.bitFlags),
                                    LoadFlag);
                    } else {
                        sortByFlags(std::rbegin(areaFlags.bitFlags), std::rend(areaFlags.bitFlags),
                                    LoadFlag);
                    }
                } else {
                    const auto cmp = [column](const EventAreaFlags& l,
                                              const EventAreaFlags& r) -> bool {
                        switch (column) {
                        case COLUMN_DESC:
                            return l.description < r.description;
                        case COLUMN_BIT:
                            return l.GetFlagID() < r.GetFlagID();
                        }
                        return false;
                    };

                    if (direction == ImGuiSortDirection_Ascending) {
                        std::sort(std::begin(areaFlags.bitFlags), std::end(areaFlags.bitFlags),
                                  cmp);
                    } else {
                        std::sort(std::rbegin(areaFlags.bitFlags), std::rend(areaFlags.bitFlags),
                                  cmp);
                    }
                }

                sort->SpecsDirty = false;
            }

            for (const auto& e : areaFlags.bitFlags) {
                std::string formattedBitLocation =
                    fmt::format("{0:02X}:{1:02X}", e.byteIndex, e.bitIndex);

                if (!filter.PassFilter(e.description.c_str()) &&
                    !filter.PassFilter(formattedBitLocation.c_str()))
                {
                    continue;
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                bool flag = LoadFlag(e);
                if (ImGui::Checkbox(fmt::format("##_unused_area_flag_{}", e.flag.flagID).c_str(), &flag)) {
                    SetFlag(e.flag, flag);
                }

                ImGui::TableNextColumn();
                ImGui::TextUnformatted(formattedBitLocation.c_str());
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(e.description.c_str());
            }
            ImGui::EndTable();
        }

        for (const auto& multiByteFlag : areaFlags.multibyteFlags) {
            auto flagValue = LoadMultiByteFlag(multiByteFlag.flag);

            const char* currentVal = "[SAVE_EDITOR_UNKNOWN]";

            auto enumValIter = multiByteFlag.enumValues.find(flagValue);
            if (enumValIter != multiByteFlag.enumValues.end()) {
                currentVal = enumValIter->second;
            }

            if (ImGui::BeginCombo(multiByteFlag.name, currentVal)) {
                for (const auto& [val, name] : multiByteFlag.enumValues) {
                    if (ImGui::Selectable(tx(name).c_str())) {
                        SetMultiByteFlag(multiByteFlag.flag, val);
                    }
                }
                ImGui::EndCombo();
            }
        }

        genCommonAreaFlags(membit);
    }

    static void drawCurrentRegionFlags()
    {
        dSv_memBit_c& membit = g_dComIfG_gameInfo.info.mMemory.mBit;
        auto* stageData = dComIfGp_getStageStagInfo();
        if (!stageData)
            return;
        uint8_t stageIndex = dStage_stagInfo_GetSaveTbl(stageData);

        genAreaFlagTable(stageIndex, membit);

        if (ImGui::TreeNode(tx("[SAVE_EDITOR_FLAG_MATRIX]").c_str())) {
            genMembitFlags("##TempSceneFlags", membit);
            ImGui::TreePop();
        }

        stage_stag_info_class* pstag = dComIfGp_getStageStagInfo();
        if (pstag != nullptr) {
            int stageNo = dStage_stagInfo_GetSaveTbl(pstag);
            if (ImGui::Button(txId("[SAVE_EDITOR_SAVE]", "##SaveTempFlags").c_str())) {
                dComIfGs_putSave(stageNo);
            }

            ImGui::SameLine();

            if (ImGui::Button(txId("[SAVE_EDITOR_LOAD]", "##LoadSaveFlags").c_str())) {
                dComIfGs_getSave(stageNo);
            }
        }
    }

    void ImGuiSaveEditor::drawFlagsTab() {
        if (ImGui::TreeNode(tx("[SAVE_EDITOR_CURRENT_REGION_FLAGS]").c_str())) {
            drawCurrentRegionFlags();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(tx("[SAVE_EDITOR_REGION_SAVED_FLAGS]").c_str())) {
            static const std::map<uint8_t, const char*> regionNames = {
                { 0x00, "[SAVE_EDITOR_ORDON]"               },
                { 0x01, "[SAVE_EDITOR_HYRULE_SEWERS]"       },
                { 0x02, "[SAVE_EDITOR_FARON]"               },
                { 0x03, "[SAVE_EDITOR_ELDIN]"               },
                { 0x04, "[SAVE_EDITOR_LANAYRU]"             },
                { 0x06, "[SAVE_EDITOR_HYRULE_FIELD]"        },
                { 0x07, "[SAVE_EDITOR_SACRED_GROVE]"        },
                { 0x08, "[SAVE_EDITOR_SNOWPEAK]"            },
                { 0x09, "[SAVE_EDITOR_CASTLE_TOWN]"         },
                { 0x0A, "[SAVE_EDITOR_GERUDO_DESERT]"       },
                { 0x0B, "[SAVE_EDITOR_FISHING_POND]"        },
                { 0x10, "[SAVE_EDITOR_FOREST_TEMPLE]"       },
                { 0x11, "[SAVE_EDITOR_GORON_MINES]"         },
                { 0x12, "[SAVE_EDITOR_LAKEBED_TEMPLE]"      },
                { 0x13, "[SAVE_EDITOR_ARBITER_S_GROUNDS]"   },
                { 0x14, "[SAVE_EDITOR_SNOWPEAK_RUINS]"      },
                { 0x15, "[SAVE_EDITOR_TEMPLE_OF_TIME]"      },
                { 0x16, "[SAVE_EDITOR_CITY_IN_THE_SKY]"     },
                { 0x17, "[SAVE_EDITOR_PALACE_OF_TWILIGHT]"  },
                { 0x18, "[SAVE_EDITOR_HYRULE_CASTLE]"       },
                { 0x19, "[SAVE_EDITOR_CAVES]"               },
                { 0x1A, "[SAVE_EDITOR_LAKE_HYLIA_LONG_CAVE]"},
                { 0x1B, "[SAVE_EDITOR_GROTTOS]"             }
            };

            if (m_selectedRegion.name == nullptr)
            {
                const auto& firstRegion = *regionNames.find(0);
                m_selectedRegion = { firstRegion.first, firstRegion.second };
            }

            if (ImGui::BeginCombo(tx("[SAVE_EDITOR_REGION]").c_str(), tx(m_selectedRegion.name).c_str())) {
                for (const auto& [id, name] : regionNames) {
                    if (ImGui::Selectable(tx(name).c_str())) {
                        m_selectedRegion = {id, name};
                    }
                }
                ImGui::EndCombo();
            }

            dSv_memBit_c& membit = dComIfGs_getSaveData()->mSave[m_selectedRegion.id].mBit;

            genAreaFlagTable(m_selectedRegion.id, membit);
            
            if (ImGui::TreeNode(tx("[SAVE_EDITOR_FLAG_MATRIX]").c_str())) {
                genMembitFlags("##SaveSceneFlags", membit);

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode(tx("[SAVE_EDITOR_EVENT_FLAGS]").c_str())) {
            dSv_event_c& event = dComIfGs_getSaveData()->mEvent;

            static ImGuiTextFilter filter;
            filter.Draw(); // Search bar

            ImVec2 flagTableSize = {700, 400};
            if (ImGui::BeginTable("Events", 4,
                                  ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_Sortable,
                                  flagTableSize))
            {
                ImGui::TableSetupScrollFreeze(0, 1);
                constexpr int COLUMN_FLAG = 0, COLUMN_NAME = 1, COLUMN_LOC = 2, COLUMN_DESC = 3;
                ImGui::TableSetupColumn(tx("[SAVE_EDITOR_FLAG]").c_str());
                ImGui::TableSetupColumn(tx("[SAVE_EDITOR_NAME]").c_str());
                ImGui::TableSetupColumn(tx("[SAVE_EDITOR_LOCATION]").c_str());
                ImGui::TableSetupColumn(tx("[SAVE_EDITOR_DESCRIPTION]").c_str());
                ImGui::TableHeadersRow();

                // if we're sorting by whether the flag is set or not, 
                // we want to re-sort whenever a flag updates, which means every frame cuz we don't know when it changes.
                // otherwise only re-sort when the sort is dirty
                if (auto* sort = ImGui::TableGetSortSpecs();
                    sort != nullptr && sort->SpecsCount > 0 &&
                    (sort->SpecsDirty || sort->Specs[0].ColumnIndex == COLUMN_FLAG))
                {
                    const auto column = sort->Specs[0].ColumnIndex;
                    const auto direction = sort->Specs[0].SortDirection;

                    // if we're sorting by flags, do special sort, regular sort is bad for sorting bools
                    // it can swap values that are the same, and that causes constant reordering
                    if (column == COLUMN_FLAG) {
                        const auto testEventFunc = [&event](const duskImguiEventFlagEntry& flag) -> bool {
                            return event.isEventBit(flag.flagID);
                        };

                        if (direction == ImGuiSortDirection_Ascending) {
                            sortByFlags(std::begin(duskImguiEventFlags),
                                        std::end(duskImguiEventFlags), testEventFunc);
                        } else {
                            sortByFlags(std::rbegin(duskImguiEventFlags),
                                        std::rend(duskImguiEventFlags), testEventFunc);
                        }
                    } else {
                        const auto cmp = [column](const duskImguiEventFlagEntry& l,
                                                  const duskImguiEventFlagEntry& r) -> bool {
                            switch (column) {
                            case COLUMN_NAME: return l.flagName < r.flagName;
                            case COLUMN_LOC:  return l.location < r.location;
                            case COLUMN_DESC: return l.description < r.description;
                            default:          return false;
                            }
                        };

                        if (direction == ImGuiSortDirection_Ascending) {
                            std::sort(std::begin(duskImguiEventFlags),
                                      std::end(duskImguiEventFlags), cmp);
                        } else {
                            std::sort(std::rbegin(duskImguiEventFlags),
                                      std::rend(duskImguiEventFlags), cmp);
                        }
                    }

                    sort->SpecsDirty = false;
                }
                
                for (const auto& e : duskImguiEventFlags) {
                    if (!filter.PassFilter(e.location.c_str()) &&
                        !filter.PassFilter(e.description.c_str()) &&
                        !filter.PassFilter(e.flagName.c_str()))
                    {
                        continue;
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    bool flag = event.getEventReg(e.flagID);
                    if (ImGui::Checkbox(("##" + e.flagName).c_str(), &flag)) {
                        if (flag) {
                            event.onEventBit(e.flagID);
                        } else {
                            event.offEventBit(e.flagID);
                        }
                    }

                    ImGui::TableNextColumn();
                    ImGuiStringViewText(e.flagName);
                    ImGui::TableNextColumn();
                    ImGuiStringViewText(e.location);
                    ImGui::TableNextColumn();
                    ImGuiStringViewText(e.description);
                }
                ImGui::EndTable();
            }

            // event values that are stored as u8s in the event flags
            for (const auto& e : duskImguiU8Events) {
                int v = event.mEvent[e.byteInd];
                if (ImGui::InputInt(e.description, &v)) {
                    v = std::clamp(v, 0, 0xff);
                    event.mEvent[e.byteInd] = (u8)v;
                }
            }            

            // event values that are stored as u16s in the event flags
            for (const auto& e : duskImguiU16Events) {
                int v = (event.mEvent[e.byteInd] << 8) | event.mEvent[e.byteInd + 1];
                if (ImGui::InputInt(e.description, &v)) {
                    v = std::clamp(v, 0, 0xffff);
                    event.mEvent[e.byteInd] = (u8)(v >> 8);
                    event.mEvent[e.byteInd + 1] = (u8)v;
                }
            }            

            // event values that are stored as swapped u16s in the event flags
            for (const auto& e : duskImguiSwappedU16Events) {
                int v = (event.mEvent[e.byteInd + 1] << 8) | event.mEvent[e.byteInd];
                if (ImGui::InputInt(e.description, &v)) {
                    v = std::clamp(v, 0, 0xffff);
                    event.mEvent[e.byteInd + 1] = (u8)(v >> 8);
                    event.mEvent[e.byteInd] = (u8)v;
                }
            }

            if (ImGui::TreeNode(tx("[SAVE_EDITOR_EVENT_MATRIX]").c_str())) {
                for (int e = 0; e < 255; e++) {
                    ImGui::Text(tx("[SAVE_EDITOR_03D]").c_str(), e);
                    ImGui::SameLine(80.0f);
                    for (int i = 7; i >= 0; i--) {
                        bool flag = event.mEvent[e] & (1 << i);
                        if (ImGui::Checkbox(fmt::format("##event{0}{1}", e, i).c_str(), &flag)) {
                            if (flag)
                                event.mEvent[e] |= (1 << i);
                            else
                                event.mEvent[e] &= ~(1 << i);
                        }
                        ImGui::SameLine();
                    }
                    ImGui::NewLine();
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }

    void ImGuiSaveEditor::drawMinigameTab() {
        dSv_MiniGame_c& minigame = dComIfGs_getSaveData()->getMiniGame();
        InputScalarBE(tx("[SAVE_EDITOR_STAR_GAME_TIME_MILLISECONDS]").c_str(), ImGuiDataType_U32, &minigame.mHookGameTime);
        InputScalarBE(tx("[SAVE_EDITOR_SNOWBOARD_RACE_TIME_MILLISECONDS]").c_str(), ImGuiDataType_U32, &minigame.mRaceGameTime);
        InputScalarBE(tx("[SAVE_EDITOR_FRUIT_POP_FLIGHT_SCORE]").c_str(), ImGuiDataType_U32, &minigame.mBalloonScore);
    }

    void ImGuiSaveEditor::drawConfigTab() {
        dSv_player_config_c& config = dComIfGs_getSaveData()->getPlayer().getConfig();
        ImGui::Checkbox(tx("[SAVE_EDITOR_ENABLE_VIBRATION]").c_str(), (bool*)&config.mVibration);

        static const char* kTargetTypeNames[] = {"[SAVE_EDITOR_HOLD]", "[SAVE_EDITOR_SWITCH]"};
        if (ImGui::BeginCombo(tx("[SAVE_EDITOR_TARGET_TYPE]").c_str(), tx(kTargetTypeNames[config.mAttentionType]).c_str())) {
            if (ImGui::Selectable(tx("[SAVE_EDITOR_HOLD]").c_str())) {
                config.mAttentionType = 0;
            }
            if (ImGui::Selectable(tx("[SAVE_EDITOR_SWITCH]").c_str())) {
                config.mAttentionType = 1;
            }
            ImGui::EndCombo();
        }

        static const char* kSoundModeNames[] = { "[SAVE_EDITOR_MONO]", "[SAVE_EDITOR_STEREO]", "[SAVE_EDITOR_SURROUND]" };
        const char* current = (config.mSoundMode < 3) ? kSoundModeNames[config.mSoundMode]
                                                      : "[SAVE_EDITOR_UNKNOWN]";
        if (ImGui::BeginCombo(tx("[SAVE_EDITOR_SOUND]").c_str(), tx(current).c_str())) {
            for (u8 i = 0; i < 3; i++) {
                bool selected = (config.mSoundMode == i);
                if (ImGui::Selectable(tx(kSoundModeNames[i]).c_str(), selected)) {
                    config.mSoundMode = i;
                }
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
}
