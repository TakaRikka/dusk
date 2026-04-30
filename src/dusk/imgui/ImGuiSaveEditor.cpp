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

#include <map>
#include <bit>

namespace dusk {
    enum ItemType {
        ITEMTYPE_DEFAULT_e,
        ITEMTYPE_EQUIP_e,
    };

    struct itemInfo {
        std::string m_name;
        u8 m_type = ITEMTYPE_DEFAULT_e;
    };

    std::map<int, itemInfo> itemMap = {
        { dItemNo_HEART_e, {"Heart"} },
        { dItemNo_GREEN_RUPEE_e, {"Green Rupee"} },
        { dItemNo_BLUE_RUPEE_e, {"Blue Rupee"} },
        { dItemNo_YELLOW_RUPEE_e, {"Yellow Rupee"} },
        { dItemNo_RED_RUPEE_e, {"Red Rupee"} },
        { dItemNo_PURPLE_RUPEE_e, {"Purple Rupee"} },
        { dItemNo_ORANGE_RUPEE_e, {"Orange Rupee"} },
        { dItemNo_SILVER_RUPEE_e, {"Silver Rupee"} },
        { dItemNo_S_MAGIC_e, {"Small Magic"} },
        { dItemNo_L_MAGIC_e, {"Large Magic"} },
        { dItemNo_BOMB_5_e, {"Bombs (5)"} },
        { dItemNo_BOMB_10_e, {"Bombs (10)"} },
        { dItemNo_BOMB_20_e, {"Bombs (20)"} },
        { dItemNo_BOMB_30_e, {"Bombs (30)"} },
        { dItemNo_ARROW_10_e, {"Arrows (10)"} },
        { dItemNo_ARROW_20_e, {"Arrows (20)"} },
        { dItemNo_ARROW_30_e, {"Arrows (30)"} },
        { dItemNo_ARROW_1_e, {"Arrows (1)"} },
        { dItemNo_PACHINKO_SHOT_e, {"Pumpkin Seeds"} },
        { dItemNo_NOENTRY_19_e, {"Reserved"} },
        { dItemNo_NOENTRY_20_e, {"Reserved"} },
        { dItemNo_NOENTRY_21_e, {"Reserved"} },
        { dItemNo_WATER_BOMB_5_e, {"Water Bombs (5)"} },
        { dItemNo_WATER_BOMB_10_e, {"Water Bombs (10)"} },
        { dItemNo_WATER_BOMB_20_e, {"Water Bombs (20)"} },
        { dItemNo_WATER_BOMB_30_e, {"Water Bombs (30)"} },
        { dItemNo_BOMB_INSECT_5_e, {"Bomblings (5)"} },
        { dItemNo_BOMB_INSECT_10_e, {"Bomblings (10)"} },
        { dItemNo_BOMB_INSECT_20_e, {"Bomblings (20)"} },
        { dItemNo_BOMB_INSECT_30_e, {"Bomblings (30)"} },
        { dItemNo_RECOVERY_FAILY_e, {"Fairy"} },
        { dItemNo_TRIPLE_HEART_e, {"Triple Hearts"} },
        { dItemNo_SMALL_KEY_e, {"Small Key"} },
        { dItemNo_KAKERA_HEART_e, {"Piece of Heart"} },
        { dItemNo_UTAWA_HEART_e, {"Heart Container"} },
        { dItemNo_MAP_e, {"Dungeon Map"} },
        { dItemNo_COMPUS_e, {"Compass"} },
        { dItemNo_DUNGEON_EXIT_e, {"Ooccoo Sr. (First Time)", ITEMTYPE_EQUIP_e} },
        { dItemNo_BOSS_KEY_e, {"Boss Key"} },
        { dItemNo_DUNGEON_BACK_e, {"Ooccoo Jr.", ITEMTYPE_EQUIP_e} },
        { dItemNo_SWORD_e, {"Ordon Sword"} },
        { dItemNo_MASTER_SWORD_e, {"Master Sword"} },
        { dItemNo_WOOD_SHIELD_e, {"Wooden Shield"} },
        { dItemNo_SHIELD_e, {"Ordon Shield"} },
        { dItemNo_HYLIA_SHIELD_e, {"Hylian Shield"} },
        { dItemNo_TKS_LETTER_e, {"Ooccoo's Note", ITEMTYPE_EQUIP_e} },
        { dItemNo_WEAR_CASUAL_e, {"Ordon Clothes"} },
        { dItemNo_WEAR_KOKIRI_e, {"Hero's Clothes"} },
        { dItemNo_ARMOR_e, {"Magic Armor"} },
        { dItemNo_WEAR_ZORA_e, {"Zora Armor"} },
        { dItemNo_MAGIC_LV1_e, {"Magic Level 1"} },
        { dItemNo_DUNGEON_EXIT_2_e, {"Ooccoo Sr.", ITEMTYPE_EQUIP_e} },
        { dItemNo_WALLET_LV1_e, {"Wallet"} },
        { dItemNo_WALLET_LV2_e, {"Big Wallet"} },
        { dItemNo_WALLET_LV3_e, {"Giant Wallet"} },
        { dItemNo_NOENTRY_55_e, {"Reserved"} },
        { dItemNo_NOENTRY_56_e, {"Reserved"} },
        { dItemNo_NOENTRY_57_e, {"Reserved"} },
        { dItemNo_NOENTRY_58_e, {"Reserved"} },
        { dItemNo_NOENTRY_59_e, {"Reserved"} },
        { dItemNo_NOENTRY_60_e, {"Reserved"} },
        { dItemNo_ZORAS_JEWEL_e, {"Coral Earring", ITEMTYPE_EQUIP_e} },
        { dItemNo_HAWK_EYE_e, {"Hawkeye", ITEMTYPE_EQUIP_e} },
        { dItemNo_WOOD_STICK_e, {"Wooden Sword"} },
        { dItemNo_BOOMERANG_e, {"Gale Boomerang", ITEMTYPE_EQUIP_e} },
        { dItemNo_SPINNER_e, {"Spinner", ITEMTYPE_EQUIP_e} },
        { dItemNo_IRONBALL_e, {"Ball and Chain", ITEMTYPE_EQUIP_e} },
        { dItemNo_BOW_e, {"Hero's Bow", ITEMTYPE_EQUIP_e} },
        { dItemNo_HOOKSHOT_e, {"Clawshot", ITEMTYPE_EQUIP_e} },
        { dItemNo_HVY_BOOTS_e, {"Iron Boots", ITEMTYPE_EQUIP_e} },
        { dItemNo_COPY_ROD_e, {"Dominion Rod", ITEMTYPE_EQUIP_e} },
        { dItemNo_W_HOOKSHOT_e, {"Double Clawshots", ITEMTYPE_EQUIP_e} },
        { dItemNo_KANTERA_e, {"Lantern", ITEMTYPE_EQUIP_e} },
        { dItemNo_LIGHT_SWORD_e, {"Light Sword"} },
        { dItemNo_FISHING_ROD_1_e, {"Fishing Rod", ITEMTYPE_EQUIP_e} },
        { dItemNo_PACHINKO_e, {"Slingshot", ITEMTYPE_EQUIP_e} },
        { dItemNo_COPY_ROD_2_e, {"Dominion Rod (Uncharged)"} },
        { dItemNo_NOENTRY_77_e, {"Reserved"} },
        { dItemNo_NOENTRY_78_e, {"Reserved"} },
        { dItemNo_BOMB_BAG_LV2_e, {"Giant Bomb Bag"} },
        { dItemNo_BOMB_BAG_LV1_e, {"Empty Bomb Bag", ITEMTYPE_EQUIP_e} },
        { dItemNo_BOMB_IN_BAG_e, {"Bomb Bag"} },
        { dItemNo_NOENTRY_82_e, {"Reserved"} },
        { dItemNo_LIGHT_ARROW_e, {"Light Arrow"} },
        { dItemNo_ARROW_LV1_e, {"Quiver"} },
        { dItemNo_ARROW_LV2_e, {"Big Quiver"} },
        { dItemNo_ARROW_LV3_e, {"Giant Quiver"} },
        { dItemNo_NOENTRY_87_e, {"Reserved"} },
        { dItemNo_LURE_ROD_e, {"Fishing Rod (Lure)"} },
        { dItemNo_BOMB_ARROW_e, {"Bomb Arrow"} },
        { dItemNo_HAWK_ARROW_e, {"Hawk Arrow"} },
        { dItemNo_BEE_ROD_e, {"Fishing Rod (Bee Larva)", ITEMTYPE_EQUIP_e} },
        { dItemNo_JEWEL_ROD_e, {"Fishing Rod (Earring)", ITEMTYPE_EQUIP_e} },
        { dItemNo_WORM_ROD_e, {"Fishing Rod (Worm)", ITEMTYPE_EQUIP_e} },
        { dItemNo_JEWEL_BEE_ROD_e, {"Fishing Rod (Earring + Bee Larva)", ITEMTYPE_EQUIP_e} },
        { dItemNo_JEWEL_WORM_ROD_e, {"Fishing Rod (Earring + Worm)", ITEMTYPE_EQUIP_e} },
        { dItemNo_EMPTY_BOTTLE_e, {"Empty Bottle", ITEMTYPE_EQUIP_e} },
        { dItemNo_RED_BOTTLE_e, {"Red Potion", ITEMTYPE_EQUIP_e} },
        { dItemNo_GREEN_BOTTLE_e, {"Green Potion", ITEMTYPE_EQUIP_e} },
        { dItemNo_BLUE_BOTTLE_e, {"Blue Potion", ITEMTYPE_EQUIP_e} },
        { dItemNo_MILK_BOTTLE_e, {"Milk Bottle", ITEMTYPE_EQUIP_e} },
        { dItemNo_HALF_MILK_BOTTLE_e, {"Half Milk Bottle", ITEMTYPE_EQUIP_e} },
        { dItemNo_OIL_BOTTLE_e, {"Lantern Oil", ITEMTYPE_EQUIP_e} },
        { dItemNo_WATER_BOTTLE_e, {"Water Bottle", ITEMTYPE_EQUIP_e} },
        { dItemNo_OIL_BOTTLE_2_e, {"Lantern Oil (Scooped)"} },
        { dItemNo_RED_BOTTLE_2_e, {"Red Potion (Scooped)"} },
        { dItemNo_UGLY_SOUP_e, {"Nasty Soup", ITEMTYPE_EQUIP_e} },
        { dItemNo_HOT_SPRING_e, {"Hotspring Water", ITEMTYPE_EQUIP_e} },
        { dItemNo_FAIRY_e, {"Fairy", ITEMTYPE_EQUIP_e} },
        { dItemNo_HOT_SPRING_2_e, {"Hotspring Water (Shop)"} },
        { dItemNo_OIL2_e, {"Lantern Refill (Scooped)"} },
        { dItemNo_OIL_e, {"Lantern Refill (Shop)"} },
        { dItemNo_NORMAL_BOMB_e, {"Bombs", ITEMTYPE_EQUIP_e} },
        { dItemNo_WATER_BOMB_e, {"Water Bombs", ITEMTYPE_EQUIP_e} },
        { dItemNo_POKE_BOMB_e, {"Bomblings", ITEMTYPE_EQUIP_e} },
        { dItemNo_FAIRY_DROP_e, {"Great Fairy's Tears", ITEMTYPE_EQUIP_e} },
        { dItemNo_WORM_e, {"Worm", ITEMTYPE_EQUIP_e} },
        { dItemNo_DROP_BOTTLE_e, {"Great Fairy Tears (Jovani)"} },
        { dItemNo_BEE_CHILD_e, {"Bee Larva", ITEMTYPE_EQUIP_e} },
        { dItemNo_CHUCHU_RARE_e, {"Rare Chu Jelly", ITEMTYPE_EQUIP_e} },
        { dItemNo_CHUCHU_RED_e, {"Red Chu Jelly", ITEMTYPE_EQUIP_e} },
        { dItemNo_CHUCHU_BLUE_e, {"Blue Chu Jelly", ITEMTYPE_EQUIP_e} },
        { dItemNo_CHUCHU_GREEN_e, {"Green Chu Jelly", ITEMTYPE_EQUIP_e} },
        { dItemNo_CHUCHU_YELLOW_e, {"Yellow Chu Jelly", ITEMTYPE_EQUIP_e} },
        { dItemNo_CHUCHU_PURPLE_e, {"Purple Chu Jelly", ITEMTYPE_EQUIP_e} },
        { dItemNo_LV1_SOUP_e, {"Simple Soup", ITEMTYPE_EQUIP_e} },
        { dItemNo_LV2_SOUP_e, {"Good Soup", ITEMTYPE_EQUIP_e} },
        { dItemNo_LV3_SOUP_e, {"Superb Soup", ITEMTYPE_EQUIP_e} },
        { dItemNo_LETTER_e, {"Renado's Letter", ITEMTYPE_EQUIP_e} },
        { dItemNo_BILL_e, {"Invoice", ITEMTYPE_EQUIP_e} },
        { dItemNo_WOOD_STATUE_e, {"Wooden Statue", ITEMTYPE_EQUIP_e} },
        { dItemNo_IRIAS_PENDANT_e, {"Ilia's Charm", ITEMTYPE_EQUIP_e} },
        { dItemNo_HORSE_FLUTE_e, {"Horse Call", ITEMTYPE_EQUIP_e} },
        { dItemNo_NOENTRY_133_e, {"Reserved"} },
        { dItemNo_NOENTRY_134_e, {"Reserved"} },
        { dItemNo_NOENTRY_135_e, {"Reserved"} },
        { dItemNo_NOENTRY_136_e, {"Reserved"} },
        { dItemNo_NOENTRY_137_e, {"Reserved"} },
        { dItemNo_NOENTRY_138_e, {"Reserved"} },
        { dItemNo_NOENTRY_139_e, {"Reserved"} },
        { dItemNo_NOENTRY_140_e, {"Reserved"} },
        { dItemNo_NOENTRY_141_e, {"Reserved"} },
        { dItemNo_NOENTRY_142_e, {"Reserved"} },
        { dItemNo_NOENTRY_143_e, {"Reserved"} },
        { dItemNo_RAFRELS_MEMO_e, {"Auru's Memo", ITEMTYPE_EQUIP_e} },
        { dItemNo_ASHS_SCRIBBLING_e, {"Ashei's Sketch", ITEMTYPE_EQUIP_e} },
        { dItemNo_NOENTRY_146_e, {"Reserved"} },
        { dItemNo_NOENTRY_147_e, {"Reserved"} },
        { dItemNo_NOENTRY_148_e, {"Reserved"} },
        { dItemNo_NOENTRY_149_e, {"Reserved"} },
        { dItemNo_NOENTRY_150_e, {"Reserved"} },
        { dItemNo_NOENTRY_151_e, {"Reserved"} },
        { dItemNo_NOENTRY_152_e, {"Reserved"} },
        { dItemNo_NOENTRY_153_e, {"Reserved"} },
        { dItemNo_NOENTRY_154_e, {"Reserved"} },
        { dItemNo_NOENTRY_155_e, {"Reserved"} },
        { dItemNo_CHUCHU_YELLOW2_e, {"Lantern Refill (Yellow Chu)"} },
        { dItemNo_OIL_BOTTLE3_e, {"Lantern Oil (Coro)"} },
        { dItemNo_SHOP_BEE_CHILD_e, {"Bee Larve (Shop)"} },
        { dItemNo_CHUCHU_BLACK_e, {"Black Chu Jelly", ITEMTYPE_EQUIP_e} },
        { dItemNo_LIGHT_DROP_e, {"Tear of Light"} },
        { dItemNo_DROP_CONTAINER_e, {"Vessel of Light (Faron)"} },
        { dItemNo_DROP_CONTAINER02_e, {"Vessel of Light (Eldin)"} },
        { dItemNo_DROP_CONTAINER03_e, {"Vessel of Light (Lanayru)"} },
        { dItemNo_FILLED_CONTAINER_e, {"Vessel of Light (Filled)"} },
        { dItemNo_MIRROR_PIECE_2_e, {"Mirror Shard (Snowpeak Ruins)"} },
        { dItemNo_MIRROR_PIECE_3_e, {"Mirror Shard (Temple of Time)"} },
        { dItemNo_MIRROR_PIECE_4_e, {"Mirror Shard (City in the Sky)"} },
        { dItemNo_NOENTRY_168_e, {"Reserved"} },
        { dItemNo_NOENTRY_169_e, {"Reserved"} },
        { dItemNo_NOENTRY_170_e, {"Reserved"} },
        { dItemNo_NOENTRY_171_e, {"Reserved"} },
        { dItemNo_NOENTRY_172_e, {"Reserved"} },
        { dItemNo_NOENTRY_173_e, {"Reserved"} },
        { dItemNo_NOENTRY_174_e, {"Reserved"} },
        { dItemNo_NOENTRY_175_e, {"Reserved"} },
        { dItemNo_SMELL_YELIA_POUCH_e, {"Scent of Ilia"} },
        { dItemNo_SMELL_PUMPKIN_e, {"Pumpkin Scent"} },
        { dItemNo_SMELL_POH_e, {"Poe Scent"} },
        { dItemNo_SMELL_FISH_e, {"Reekfish Scent"} },
        { dItemNo_SMELL_CHILDREN_e, {"Youth's Scent"} },
        { dItemNo_SMELL_MEDICINE_e, {"Medicine Scent"} },
        { dItemNo_NOENTRY_182_e, {"Reserved"} },
        { dItemNo_NOENTRY_183_e, {"Reserved"} },
        { dItemNo_NOENTRY_184_e, {"Reserved"} },
        { dItemNo_NOENTRY_185_e, {"Reserved"} },
        { dItemNo_NOENTRY_186_e, {"Reserved"} },
        { dItemNo_NOENTRY_187_e, {"Reserved"} },
        { dItemNo_NOENTRY_188_e, {"Reserved"} },
        { dItemNo_NOENTRY_189_e, {"Reserved"} },
        { dItemNo_NOENTRY_190_e, {"Reserved"} },
        { dItemNo_NOENTRY_191_e, {"Reserved"} },
        { dItemNo_M_BEETLE_e, {"Beetle (M)"} },
        { dItemNo_F_BEETLE_e, {"Beetle (F)"} },
        { dItemNo_M_BUTTERFLY_e, {"Butterfly (M)"} },
        { dItemNo_F_BUTTERFLY_e, {"Butterfly (F)"} },
        { dItemNo_M_STAG_BEETLE_e, {"Stag Beetle (M)"} },
        { dItemNo_F_STAG_BEETLE_e, {"Stag Beetle (F)"} },
        { dItemNo_M_GRASSHOPPER_e, {"Grasshopper (M)"} },
        { dItemNo_F_GRASSHOPPER_e, {"Grasshopper (F)"} },
        { dItemNo_M_NANAFUSHI_e, {"Phasmid (M)"} },
        { dItemNo_F_NANAFUSHI_e, {"Phasmid (F)"} },
        { dItemNo_M_DANGOMUSHI_e, {"Pill Bug (M)"} },
        { dItemNo_F_DANGOMUSHI_e, {"Pill Bug (F)"} },
        { dItemNo_M_MANTIS_e, {"Mantis (M)"} },
        { dItemNo_F_MANTIS_e, {"Mantis (F)"} },
        { dItemNo_M_LADYBUG_e, {"Ladybug (M)"} },
        { dItemNo_F_LADYBUG_e, {"Ladybug (F)"} },
        { dItemNo_M_SNAIL_e, {"Snail (M)"} },
        { dItemNo_F_SNAIL_e, {"Snail (F)"} },
        { dItemNo_M_DRAGONFLY_e, {"Dragonfly (M)"} },
        { dItemNo_F_DRAGONFLY_e, {"Dragonfly (F)"} },
        { dItemNo_M_ANT_e, {"Ant (M)"} },
        { dItemNo_F_ANT_e, {"Ant (F)"} },
        { dItemNo_M_MAYFLY_e, {"Mayfly (M)"} },
        { dItemNo_F_MAYFLY_e, {"Mayfly (F)"} },
        { dItemNo_NOENTRY_216_e, {"Reserved"} },
        { dItemNo_NOENTRY_217_e, {"Reserved"} },
        { dItemNo_NOENTRY_218_e, {"Reserved"} },
        { dItemNo_NOENTRY_219_e, {"Reserved"} },
        { dItemNo_NOENTRY_220_e, {"Reserved"} },
        { dItemNo_NOENTRY_221_e, {"Reserved"} },
        { dItemNo_NOENTRY_222_e, {"Reserved"} },
        { dItemNo_NOENTRY_223_e, {"Reserved"} },
        { dItemNo_POU_SPIRIT_e, {"Poe Soul"} },
        { dItemNo_NOENTRY_225_e, {"Reserved"} },
        { dItemNo_NOENTRY_226_e, {"Reserved"} },
        { dItemNo_NOENTRY_227_e, {"Reserved"} },
        { dItemNo_NOENTRY_228_e, {"Reserved"} },
        { dItemNo_NOENTRY_229_e, {"Reserved"} },
        { dItemNo_NOENTRY_230_e, {"Reserved"} },
        { dItemNo_NOENTRY_231_e, {"Reserved"} },
        { dItemNo_NOENTRY_232_e, {"Reserved"} },
        { dItemNo_ANCIENT_DOCUMENT_e, {"Ancient Sky Book", ITEMTYPE_EQUIP_e} },
        { dItemNo_AIR_LETTER_e, {"Ancient Sky Book (Partial)", ITEMTYPE_EQUIP_e} },
        { dItemNo_ANCIENT_DOCUMENT2_e, {"Ancient Sky Book (Filled)", ITEMTYPE_EQUIP_e} },
        { dItemNo_LV7_DUNGEON_EXIT_e, {"Ooccoo Sr. (City in the Sky)"} },
        { dItemNo_LINKS_SAVINGS_e, {"Purple Rupee (Link's Savings)"} },
        { dItemNo_SMALL_KEY2_e, {"Small Key (North Faron Gate)"} },
        { dItemNo_POU_FIRE1_e, {"Poe Fire 1"} },
        { dItemNo_POU_FIRE2_e, {"Poe Fire 2"} },
        { dItemNo_POU_FIRE3_e, {"Poe Fire 3"} },
        { dItemNo_POU_FIRE4_e, {"Poe Fire 4"} },
        { dItemNo_BOSSRIDER_KEY_e, {"Hyrule Field Keys"} },
        { dItemNo_TOMATO_PUREE_e, {"Ordon Pumpkin", ITEMTYPE_EQUIP_e} },
        { dItemNo_TASTE_e, {"Ordon Goat Cheese", ITEMTYPE_EQUIP_e} },
        { dItemNo_LV5_BOSS_KEY_e, {"Bedroom Key"} },
        { dItemNo_SURFBOARD_e, {"Surf Leaf"} },
        { dItemNo_KANTERA2_e, {"Lantern (Reclaimed)"} },
        { dItemNo_L2_KEY_PIECES1_e, {"Key Shard (1)"} },
        { dItemNo_L2_KEY_PIECES2_e, {"Key Shard (2)"} },
        { dItemNo_L2_KEY_PIECES3_e, {"Key Shard (3)"} },
        { dItemNo_KEY_OF_CARAVAN_e, {"Bulblin Camp Key"} },
        { dItemNo_LV2_BOSS_KEY_e, {"Goron Mines Boss Key"} },
        { dItemNo_KEY_OF_FILONE_e, {"South Faron Gate Key"} },
        { dItemNo_NONE_e, {"None"} },
    };

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
        "Ant",      "Dayfly",  "Beetle",  "Mantis",
        "Stag Beetle", "Pill Bug", "Butterfly", "Ladybug",
        "Snail",    "Phasmid", "Grasshopper", "Dragonfly",
    };

    static constexpr int HIDDEN_SKILL_COUNT = 7;

    static const u16 sHiddenSkillFlags[HIDDEN_SKILL_COUNT] = {
        dSv_event_flag_c::F_0339, dSv_event_flag_c::F_0338,
        dSv_event_flag_c::F_0340, dSv_event_flag_c::F_0341,
        dSv_event_flag_c::F_0342, dSv_event_flag_c::F_0343,
        dSv_event_flag_c::F_0344,
    };

    static const char* sHiddenSkillNames[HIDDEN_SKILL_COUNT] = {
        "Ending Blow", "Shield Attack", "Back Slice", "Helm Splitter",
        "Mortal Draw", "Jump Strike", "Great Spin",
    };

    static constexpr int LETTER_COUNT = 16;

    static const char* sLetterSenders[LETTER_COUNT] = {
        "Renado",        "Ooccoo 1", "Ooccoo 2",       "The Postman",
        "Kakariko Goods", "Barnes 1", "Barnes 2",      "Barnes Bombs",
        "Malo Mart",     "Telma",    "Purlo",          "From Jr.",
        "Princess Agitha", "Lanayru Tourism", "Shad", "Yeta",
    };

    static constexpr int FISH_COUNT = 6;

    static const struct {
        u8 index;
        const char* name;
    } sFishSpecies[FISH_COUNT] = {
        { 3, "Ordon Catfish" },
        { 5, "Greengill"     },
        { 4, "Reekfish"      },
        { 0, "Hyrule Bass"   },
        { 2, "Hylian Pike"   },
        { 1, "Hylian Loach"  },
    };

    static const char* sSwordNames[4] = {
        "Ordon Sword", "Master Sword", "Wooden Sword", "Light Sword",
    };

    static const char* sShieldNames[3] = {
        "Wooden Shield", "Ordon Shield", "Hylian Shield",
    };

    static const char* sFusedShadowNames[3] = {
        "Forest Temple",
        "Goron Mines",
        "Lakebed Temple",
    };

    static const struct {
        u8 index;
        const char* name;
    } sMirrorShards[3] = {
        { 1, "Snowpeak Ruins"  },
        { 2, "Temple of Time"  },
        { 3, "City in the Sky" },
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

        if (ImGui::Begin("Save Editor", &open, windowFlags)) {
            if (ImGui::BeginTabBar("SaveEditorTabBar", ImGuiTabBarFlags_NoCloseWithMiddleMouseButton)) {
                if (ImGui::BeginTabItem("Player Status")) {
                    drawPlayerStatusTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Location")) {
                    drawLocationTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Inventory")) {
                    drawInventoryTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Collection")) {
                    drawCollectionTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Flags")) {
                    drawFlagsTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Minigame")) {
                    drawMinigameTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Config")) {
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
            ? fmt::format("Slot {0} ({1})", currentSlotNo, itemMap.find(item.mItems[currentSlotNo])->second.m_name)
            : "None";

        // TODO: live update equips
        if (ImGui::BeginCombo(label, defaultLabel.c_str())) {
            if (ImGui::Selectable("None")) {
                selectItemData = 0xFF;
            }

            for (int i = 0; i < 24; i++) {
                u8 itemNo = item.mItems[i];
                if (ImGui::Selectable(fmt::format("Slot {0} ({1})", i, itemMap.find(itemNo)->second.m_name).c_str())) {
                    selectItemData = i;
                }
            }
            ImGui::EndCombo();
        }
    }

    void ImGuiSaveEditor::drawPlayerStatusTab() {
        const char* playerName = dComIfGs_getPlayerName();
        ImGui::Text("Player Name: ");
        ImGui::SameLine();
        char nameBuffer[8];
        snprintf(nameBuffer, sizeof(nameBuffer), "%s", playerName);
        if (ImGui::InputText("##PlayerNameInput", nameBuffer, 8)) {
            strcpy(dComIfGs_getPlayerName(), nameBuffer);
        }

        const char* horseName = dComIfGs_getHorseName();
        ImGui::Text("Horse Name:  ");
        ImGui::SameLine();
        char horseNameBuffer[8];
        snprintf(horseNameBuffer, sizeof(horseNameBuffer), "%s", horseName);
        if (ImGui::InputText("##HorseNameInput", horseNameBuffer, 8)) {
            strcpy(dComIfGs_getHorseName(), horseNameBuffer);
        }

        ImGui::Separator();

        dSv_player_status_a_c& statusA = dComIfGs_getSaveData()->getPlayer().getPlayerStatusA();
        dSv_player_status_b_c& statusB = dComIfGs_getSaveData()->getPlayer().getPlayerStatusB();

        InputScalarBE("Max Health", ImGuiDataType_U16, &statusA.mMaxLife);
        InputScalarBE("Health", ImGuiDataType_U16, &statusA.mLife);
        InputScalarBE("Rupees", ImGuiDataType_U16, &statusA.mRupee);
        InputScalarBE("Max Oil", ImGuiDataType_U16, &statusA.mMaxOil);
        InputScalarBE("Oil", ImGuiDataType_U16, &statusA.mOil);

        genSelectItemComboBox("Equip X", statusA.mSelectItem[0]);
        genSelectItemComboBox("Equip Y", statusA.mSelectItem[1]);
        genSelectItemComboBox("Combo Equip X", statusA.mMixItem[0]);
        genSelectItemComboBox("Combo Equip Y", statusA.mMixItem[1]);


        if (ImGui::BeginCombo("Clothes", itemMap.find(statusA.mSelectEquip[0])->second.m_name.c_str())) {
            if (ImGui::Selectable("Ordon Clothes")) {
                dMeter2Info_setCloth(dItemNo_WEAR_CASUAL_e, false);
                daPy_getPlayerActorClass()->setClothesChange(0);
            }
            if (ImGui::Selectable("Hero's Clothes")) {
                dMeter2Info_setCloth(dItemNo_WEAR_KOKIRI_e, false);
                daPy_getPlayerActorClass()->setClothesChange(0);
            }
            if (ImGui::Selectable("Zora Armor")) {
                dMeter2Info_setCloth(dItemNo_WEAR_ZORA_e, false);
                daPy_getPlayerActorClass()->setClothesChange(0);
            }
            if (ImGui::Selectable("Magic Armor")) {
                dMeter2Info_setCloth(dItemNo_ARMOR_e, false);
                daPy_getPlayerActorClass()->setClothesChange(0);
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo("Sword", itemMap.find(statusA.mSelectEquip[1])->second.m_name.c_str())) {
            if (ImGui::Selectable("None")) {
                statusA.mSelectEquip[1] = dItemNo_NONE_e;
            }
            if (ImGui::Selectable("Wooden Sword")) {
                statusA.mSelectEquip[1] = dItemNo_WOOD_STICK_e;
            }
            if (ImGui::Selectable("Ordon Sword")) {
                statusA.mSelectEquip[1] = dItemNo_SWORD_e;
            }
            if (ImGui::Selectable("Master Sword")) {
                statusA.mSelectEquip[1] = dItemNo_MASTER_SWORD_e;
            }
            if (ImGui::Selectable("Light Sword")) {
                statusA.mSelectEquip[1] = dItemNo_LIGHT_SWORD_e;
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo("Shield", itemMap.find(statusA.mSelectEquip[2])->second.m_name.c_str())) {
            if (ImGui::Selectable("None")) {
                statusA.mSelectEquip[2] = dItemNo_NONE_e;
            }
            if (ImGui::Selectable("Wooden Shield")) {
                statusA.mSelectEquip[2] = dItemNo_SHIELD_e;
            }
            if (ImGui::Selectable("Ordon Shield")) {
                statusA.mSelectEquip[2] = dItemNo_WOOD_SHIELD_e;
            }
            if (ImGui::Selectable("Hylian Shield")) {
                statusA.mSelectEquip[2] = dItemNo_HYLIA_SHIELD_e;
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo("Scent", itemMap.find(statusA.mSelectEquip[3])->second.m_name.c_str())) {
            if (ImGui::Selectable("None")) {
                statusA.mSelectEquip[3] = dItemNo_NONE_e;
            }
            if (ImGui::Selectable("Youth's Scent")) {
                statusA.mSelectEquip[3] = dItemNo_SMELL_CHILDREN_e;
            }
            if (ImGui::Selectable("Scent of Ilia")) {
                statusA.mSelectEquip[3] = dItemNo_SMELL_YELIA_POUCH_e;
            }
            if (ImGui::Selectable("Poe Scent")) {
                statusA.mSelectEquip[3] = dItemNo_SMELL_POH_e;
            }
            if (ImGui::Selectable("Reekfish Scent")) {
                statusA.mSelectEquip[3] = dItemNo_SMELL_FISH_e;
            }
            if (ImGui::Selectable("Medicine Scent")) {
                statusA.mSelectEquip[3] = dItemNo_SMELL_MEDICINE_e;
            }
            ImGui::EndCombo();
        }

        const char* walletSizeNames[] = {
            "Normal",
            "Big",
            "Giant",
        };
        int walletSize = statusA.getWalletSize();
        if (ImGui::BeginCombo("Wallet Size", walletSizeNames[walletSize])) {
            if (ImGui::Selectable(walletSizeNames[WALLET])) {
                statusA.setWalletSize(WALLET);
            }
            if (ImGui::Selectable(walletSizeNames[BIG_WALLET])) {
                statusA.setWalletSize(BIG_WALLET);
            }
            if (ImGui::Selectable(walletSizeNames[GIANT_WALLET])) {
                statusA.setWalletSize(GIANT_WALLET);
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginCombo("Form", statusA.mTransformStatus == 0 ? "Human" : "Wolf")) {
            if (ImGui::Selectable("Human")) {
                statusA.mTransformStatus = TF_STATUS_HUMAN;
            }
            if (ImGui::Selectable("Wolf")) {
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
        if (ImGui::InputScalar("Time##TimeMinutes", ImGuiDataType_S32, &min)) {
            min = std::clamp(min, 0, 59);
            statusB.setTime((hours * 15.0f) + (min / 60.0f * 15.0f));
        }

        InputScalarBE("Date", ImGuiDataType_U16, &statusB.mDate);

        int transformLevel = 0;
        for (int i = 0; i < 4; i++) {
            if (statusB.mTransformLevelFlag & (1 << i)) {
                transformLevel++;
            }
        }
        if (ImGui::SliderInt("Transform Level", &transformLevel, 0, 3)) {
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
        if (ImGui::SliderInt("Twilight Clear Level", &darkClearLevel, 0, 3)) {
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
        ImGui::Text("Save Location");

        ImGui::Text("Stage:    ");
        ImGui::SameLine();
        char nameBuffer[8];
        snprintf(nameBuffer, sizeof(nameBuffer), "%s", returnPlace.mName);
        if (ImGui::InputText("##SaveStageNameInput", nameBuffer, 8)) {
            strcpy(returnPlace.mName, nameBuffer);
        }

        ImGui::Text("Room:     ");
        ImGui::SameLine();
        int tempRoom = returnPlace.mRoomNo;
        if (ImGui::InputInt("##SaveRoomInput", &tempRoom)) {
            returnPlace.mRoomNo = tempRoom;
        }

        ImGui::Text("Spawn ID: ");
        ImGui::SameLine();
        int tempSpawn = returnPlace.mPlayerStatus;
        if (ImGui::InputInt("##SaveSpawnInput", &tempSpawn)) {
            returnPlace.mPlayerStatus = tempSpawn;
        }

        ImGui::Separator();

        ImGui::Text("Horse Location");

        ImGui::Text("Position: ");
        ImGui::SameLine();
        Vec tempPos = horsePlace.mPos;
        if (ImGui::InputFloat3("##HorsePosition", &tempPos.x)) {
            horsePlace.mPos.x = tempPos.x;
            horsePlace.mPos.y = tempPos.y;
            horsePlace.mPos.z = tempPos.z;
        }

        ImGui::Text("Angle:    ");
        ImGui::SameLine();
        int tempAngle = horsePlace.mAngleY;
        if (ImGui::InputInt("##HorsePosition", &tempAngle)) {
            horsePlace.mAngleY = tempAngle;
        }

        ImGui::Text("Stage:    ");
        ImGui::SameLine();
        char horseStageBuffer[8];
        snprintf(horseStageBuffer, sizeof(horseStageBuffer), "%s", horsePlace.mName);
        if (ImGui::InputText("##HorseStageNameInput", horseStageBuffer, 8)) {
            strcpy(horsePlace.mName, horseStageBuffer);
        }

        ImGui::Text("Room:     ");
        ImGui::SameLine();
        int tempHorseRoom = horsePlace.mRoomNo;
        if (ImGui::InputInt("##HorseRoomInput", &tempHorseRoom)) {
            horsePlace.mRoomNo = tempHorseRoom;
        }

        ImGui::Text("Spawn ID: ");
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

        if (ImGui::TreeNode("Item Wheel")) {
            if (ImGui::Button("Default All##inv_default_all")) {
                for (int slot = 0; slot < 24; slot++) {
                    dComIfGs_setItem(slot, getSlotDefault(slot));
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Clear All##inv_clear_all")) {
                for (int slot = 0; slot < 24; slot++) {
                    dComIfGs_setItem(slot, dItemNo_NONE_e);
                }
            }

            ImGuiBeginGroupPanel("Items", { 200, 100 });
            for (int slot = 0; slot < 24; slot++) {
                ImGui::Text("Slot %02d (%s): ", slot, itemMap.find(getSlotDefault(slot))->second.m_name.c_str());
                ImGui::SameLine(240.0f);
                if (ImGui::BeginCombo(fmt::format("##ItemComboBox{}", slot).c_str(), itemMap.find(item.mItems[slot])->second.m_name.c_str())) {
                    if (ImGui::Selectable("None")) {
                        dComIfGs_setItem(slot, dItemNo_NONE_e);
                    }

                    for (int i = 0; i < 254; i++) {
                        if (itemMap.find(i)->second.m_type != ITEMTYPE_EQUIP_e) continue;

                        if (ImGui::Selectable(fmt::format("{0}##item_{1}{2}", itemMap.find(i)->second.m_name, slot, i).c_str())) {
                            dComIfGs_setItem(slot, itemMap.find(i)->first);
                        }
                    }
                    ImGui::EndCombo();
                }
                ImGui::SameLine();
                if (ImGui::SmallButton(fmt::format("Default##slot_d_{}", slot).c_str())) {
                    dComIfGs_setItem(slot, getSlotDefault(slot));
                }
                ImGui::SameLine();
                if (ImGui::SmallButton(fmt::format("Clear##slot_c_{}", slot).c_str())) {
                    dComIfGs_setItem(slot, dItemNo_NONE_e);
                }
            }
            ImGuiEndGroupPanel();

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Get Item Flags")) {
            for (int i = 0; i < 254; i++) {
                if (itemMap.find(i)->second.m_name == "Reserved") continue;

                bool flag = dComIfGs_isItemFirstBit(i);
                if (ImGui::Checkbox(fmt::format("{0}##item_{1}", itemMap.find(i)->second.m_name, i).c_str(), &flag)) {
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

        ImGuiBeginGroupPanel("Item Max Capacities", { 200, 100 });
        ImGui::InputScalar("Arrows Max", ImGuiDataType_U8, &itemMax.mItemMax[0]);
        ImGui::InputScalar("Normal Bombs Max", ImGuiDataType_U8, &itemMax.mItemMax[1]);
        ImGui::InputScalar("Water Bombs Max", ImGuiDataType_U8, &itemMax.mItemMax[2]);
        ImGui::InputScalar("Bomblings Max", ImGuiDataType_U8, &itemMax.mItemMax[3]);
        ImGuiEndGroupPanel();

        ImGuiBeginGroupPanel("Item Amounts", { 200, 100 });
        ImGui::InputScalar("Arrows Amount", ImGuiDataType_U8, &itemRecord.mArrowNum);
        ImGui::InputScalar("Slingshot Amount", ImGuiDataType_U8, &itemRecord.mPachinkoNum);
        ImGui::InputScalar("Bomb Bag 1 Amount", ImGuiDataType_U8, &itemRecord.mBombNum[0]);
        ImGui::InputScalar("Bomb Bag 2 Amount", ImGuiDataType_U8, &itemRecord.mBombNum[1]);
        ImGui::InputScalar("Bomb Bag 3 Amount", ImGuiDataType_U8, &itemRecord.mBombNum[2]);
        ImGui::InputScalar("Bottle 1 Amount", ImGuiDataType_U8, &itemRecord.mBottleNum[0]);
        ImGui::InputScalar("Bottle 2 Amount", ImGuiDataType_U8, &itemRecord.mBottleNum[1]);
        ImGui::InputScalar("Bottle 3 Amount", ImGuiDataType_U8, &itemRecord.mBottleNum[2]);
        ImGui::InputScalar("Bottle 4 Amount", ImGuiDataType_U8, &itemRecord.mBottleNum[3]);
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
        if (ImGui::TreeNode("Equipment")) {
            if (ImGui::TreeNode("Swords")) {
                static u8 sword_list[] = {
                    dItemNo_SWORD_e,
                    dItemNo_MASTER_SWORD_e,
                    dItemNo_WOOD_STICK_e,
                    dItemNo_LIGHT_SWORD_e,
                };

                for (int i = 0; i < 4; i++) {
                    bool got = dComIfGs_isItemFirstBit(sword_list[i]) != 0;
                    if (ImGui::Checkbox(fmt::format("{0}##sword_{1}", sSwordNames[i], i).c_str(), &got)) {
                        if (got) dComIfGs_onItemFirstBit(sword_list[i]);
                        else     dComIfGs_offItemFirstBit(sword_list[i]);
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Shields")) {
                static u8 shield_list[] = {
                    dItemNo_SHIELD_e,
                    dItemNo_WOOD_SHIELD_e,
                    dItemNo_HYLIA_SHIELD_e,
                };

                for (int i = 0; i < 3; i++) {
                    bool got = dComIfGs_isItemFirstBit(shield_list[i]) != 0;
                    if (ImGui::Checkbox(fmt::format("{0}##shield_{1}", sShieldNames[i], i).c_str(), &got)) {
                        if (got) dComIfGs_onItemFirstBit(shield_list[i]);
                        else     dComIfGs_offItemFirstBit(shield_list[i]);
                    }
                }
                ImGui::TreePop();
            }

            // TODO: the game checks if you're in ranch clothes, and if so won't display any other tunics in the menu
            // find a way to deal with this later
            if (ImGui::TreeNode("Tunics")) {
                bool ordonClothes = dComIfGs_isItemFirstBit(dItemNo_WEAR_CASUAL_e) != 0;
                if (ImGui::Checkbox("Ordon Clothes##tunic_ordon", &ordonClothes)) {
                    setItemFirstBit(dItemNo_WEAR_CASUAL_e, ordonClothes);
                }

                bool greenTunic = dComIfGs_isCollectClothes(KOKIRI_CLOTHES_FLAG) != 0;
                if (ImGui::Checkbox("Hero's Clothes##tunic_green", &greenTunic)) {
                    if (greenTunic) dComIfGs_setCollectClothes(KOKIRI_CLOTHES_FLAG);
                    else            dComIfGs_offCollectClothes(KOKIRI_CLOTHES_FLAG);
                }

                bool zoraArmor = dComIfGs_isItemFirstBit(dItemNo_WEAR_ZORA_e) != 0;
                if (ImGui::Checkbox("Zora Armor##tunic_zora", &zoraArmor)) {
                    setItemFirstBit(dItemNo_WEAR_ZORA_e, zoraArmor);
                }

                bool magicArmor = dComIfGs_isItemFirstBit(dItemNo_ARMOR_e) != 0;
                if (ImGui::Checkbox("Magic Armor##tunic_magic", &magicArmor)) {
                    setItemFirstBit(dItemNo_ARMOR_e, magicArmor);
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Key Items")) {
            if (ImGui::TreeNode("Fused Shadows")) {
                for (int i = 0; i < 3; i++) {
                    bool got = dComIfGs_isCollectCrystal((u8)i) != 0;
                    if (ImGui::Checkbox(
                            fmt::format("{0}##fs_{1}", sFusedShadowNames[i], i).c_str(), &got)) {
                        if (got) dComIfGs_onCollectCrystal((u8)i);
                        else     dComIfGs_offCollectCrystal((u8)i);
                    }
                }
                ImGui::Spacing();
                if (ImGui::Button("All##fs_all")) {
                    for (int i = 0; i < 3; i++) dComIfGs_onCollectCrystal((u8)i);
                }
                ImGui::SameLine();
                if (ImGui::Button("None##fs_clear")) {
                    for (int i = 0; i < 3; i++) dComIfGs_offCollectCrystal((u8)i);
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Mirror Shards")) {
                for (int i = 0; i < 3; i++) {
                    u8 idx  = sMirrorShards[i].index;
                    bool got = dComIfGs_isCollectMirror(idx) != 0;
                    if (ImGui::Checkbox(
                            fmt::format("{0}##ms_{1}", sMirrorShards[i].name, i).c_str(), &got)) {
                        if (got) dComIfGs_onCollectMirror(idx);
                        else     dComIfGs_offCollectMirror(idx);
                    }
                }
                ImGui::Spacing();
                if (ImGui::Button("All##ms_all")) {
                    for (int i = 0; i < 3; i++) dComIfGs_onCollectMirror(sMirrorShards[i].index);
                }
                ImGui::SameLine();
                if (ImGui::Button("None##ms_clear")) {
                    for (int i = 0; i < 3; i++) dComIfGs_offCollectMirror(sMirrorShards[i].index);
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Heart Pieces & Poe Souls")) {
            if (ImGui::TreeNode("Poe Souls")) {
                int poeCount = dComIfGs_getPohSpiritNum();
                ImGui::Text("Collected:");
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
                if (ImGui::Button("All 60##poe_all")) {
                    dComIfGs_setPohSpiritNum(60);
                }
                ImGui::SameLine();
                if (ImGui::Button("Clear##poe_clear")) {
                    dComIfGs_setPohSpiritNum(0);
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Heart Pieces")) {
                int maxLife = dComIfGs_getMaxLife();
                int hearts  = maxLife / 5;
                int pieces  = maxLife % 5;
                ImGui::Text("Max Life:");
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
                ImGui::TextDisabled("(%d hearts + %d pieces)", hearts, pieces);
                ImGui::Spacing();
                if (ImGui::Button("3 Hearts##hp_min")) {
                    dComIfGs_setMaxLife(15);
                    dComIfGs_setLife(12);
                }
                ImGui::SameLine();
                if (ImGui::Button("20 Hearts##hp_max")) {
                    dComIfGs_setMaxLife(100);
                    dComIfGs_setLife(80);
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Golden Bugs")) {
            if (ImGui::BeginTable("GoldenBugTable", 5,
                                  ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn("Species");
                ImGui::TableSetupColumn("Male");
                ImGui::TableSetupColumn("Female");
                ImGui::TableSetupColumn("M \xe2\x86\x92 Agitha");  // M → Agitha
                ImGui::TableSetupColumn("F \xe2\x86\x92 Agitha");  // F → Agitha
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
                    ImGui::TextUnformatted(sBugSpeciesNames[species]);

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
            if (ImGui::Button("Collect All##bugs_all")) {
                for (int i = 0; i < BUG_SPECIES_COUNT * 2; i++) {
                    dComIfGs_onItemFirstBit(sBugItemIds[i]);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Clear All##bugs_clear")) {
                for (int i = 0; i < BUG_SPECIES_COUNT * 2; i++) {
                    dComIfGs_offItemFirstBit(sBugItemIds[i]);
                    dComIfGs_offEventBit(sBugTurnInFlags[i]);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Give All to Agitha##bugs_giveall")) {
                for (int i = 0; i < BUG_SPECIES_COUNT * 2; i++) {
                    dComIfGs_onEventBit(sBugTurnInFlags[i]);
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Hidden Skills")) {
            for (int i = 0; i < HIDDEN_SKILL_COUNT; i++) {
                bool learned = dComIfGs_isEventBit(sHiddenSkillFlags[i]) != 0;
                if (ImGui::Checkbox(
                        fmt::format("{0}##skill_{1}", sHiddenSkillNames[i], i).c_str(), &learned)) {
                    setEventBit(sHiddenSkillFlags[i], learned);
                }
            }
            ImGui::Spacing();
            if (ImGui::Button("Learn All##skills_all")) {
                for (int i = 0; i < HIDDEN_SKILL_COUNT; i++) {
                    dComIfGs_onEventBit(sHiddenSkillFlags[i]);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Forget All##skills_clear")) {
                for (int i = 0; i < HIDDEN_SKILL_COUNT; i++) {
                    dComIfGs_offEventBit(sHiddenSkillFlags[i]);
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Collection Logs")) {
            if (ImGui::TreeNode("Postman Letters")) {
                for (int i = 0; i < LETTER_COUNT; i++) {
                    bool had = dComIfGs_isLetterGetFlag(i) != 0;
                    if (ImGui::Checkbox(
                            fmt::format("{0}##letter_{1}", sLetterSenders[i], i).c_str(), &had)) {
                        setLetterGetFlag(i, had);
                    }
                }
                ImGui::Spacing();
                if (ImGui::Button("Receive All##letters_all")) {
                    for (int i = 0; i < LETTER_COUNT; i++) setLetterGetFlag(i, true);
                }
                ImGui::SameLine();
                if (ImGui::Button("Clear All##letters_clear")) {
                    for (int i = 0; i < LETTER_COUNT; i++) setLetterGetFlag(i, false);
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Fishing Log")) {
                dSv_fishing_info_c& fish = dComIfGs_getSaveData()->getPlayer().getFishingInfo();
                if (ImGui::BeginTable("FishTable", 3,
                                      ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg)) {
                    ImGui::TableSetupColumn("Species");
                    ImGui::TableSetupColumn("Caught");
                    ImGui::TableSetupColumn("Biggest (cm)");
                    ImGui::TableHeadersRow();

                    for (int i = 0; i < FISH_COUNT; i++) {
                        u8 idx = sFishSpecies[i].index;
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted(sFishSpecies[i].name);

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

        genDungeonItemCheckbox(membit, "Got Map", dSv_memBit_c::MAP);
        ImGui::SameLine(230.0f);
        genDungeonItemCheckbox(membit, "Got Compass", dSv_memBit_c::COMPASS);

        genDungeonItemCheckbox(membit, "Got Boss Key", dSv_memBit_c::BOSS_KEY);
        ImGui::SameLine(230.0f);
        genDungeonItemCheckbox(membit, "Saw Boss Demo", dSv_memBit_c::STAGE_BOSS_DEMO);

        genDungeonItemCheckbox(membit, "Got Heart Container", dSv_memBit_c::STAGE_LIFE);
        ImGui::SameLine(230.0f);
        genDungeonItemCheckbox(membit, "Defeated Boss", dSv_memBit_c::STAGE_BOSS_ENEMY);

        genDungeonItemCheckbox(membit, "Defeated Miniboss", dSv_memBit_c::STAGE_BOSS_ENEMY_2);
        ImGui::SameLine(230.0f);
        genDungeonItemCheckbox(membit, "Got Ooccoo", dSv_memBit_c::OOCCOO_NOTE);

        int keyTemp = membit.getKeyNum();
        if (ImGui::SliderInt("Keys", &keyTemp, 0, 5)) {
            membit.setKeyNum(keyTemp);
        }
    }

    static void genMembitFlags(const char* id, dSv_memBit_c& membit) {
        ImGuiBeginGroupPanel("Chest", { 100, 100 });
        for (int j = 0; j < 2; j++) {
            drawFlagList(fmt::format("##_tbox{}", j).c_str(), membit.mTbox[j]);
        }
        ImGuiEndGroupPanel();

        ImVec2 post_tbox_cursor = ImGui::GetCursorPos();

        ImGui::SameLine();

        ImGuiBeginGroupPanel("Switch", { 100, 100 });
        for (int j = 0; j < 4; j++) {
            drawFlagList(fmt::format("##_switch{}", j).c_str(), membit.mSwitch[j]);
        }
        ImGuiEndGroupPanel();

        ImVec2 post_switch_cursor = ImGui::GetCursorPos();

        ImGui::SetCursorPos(post_tbox_cursor);

        ImGuiBeginGroupPanel("Item", { 100, 100 });
        for (int j = 0; j < 1; j++) {
            drawFlagList(fmt::format("##_item{}", j).c_str(), membit.mItem[j]);
        }
        ImGuiEndGroupPanel();
        genCommonAreaFlags(membit);
    }

    template <typename T>
    concept FlagIter = requires(T t) {
        ++t;
        --t;
        t + 1;
        t < t;
        { t->flagID } -> std::convertible_to<u16>;
    };

    template <typename T>
    concept FlagTester = requires(T t, u16 flagID) {
        { t(flagID) } -> std::convertible_to<bool>;
    };

    static void sortByFlags(FlagIter auto begin, FlagIter auto end, FlagTester auto&& flagTester) {
        if (begin == end) return;

        FlagIter auto fullEnd = end;

        // We want to find the location of where we can swap our `On` flags to.
        // We're gonna put the `Off` bits first, and the `On` bits last. 0 < 1
        // We can achieve this by skipping all the `On` bits at the end.

        // backtrack until we find a bit that is off
        while (begin < --end && flagTester(end->flagID)) {
            // move the end pointer back while we find on bits
        }

        // end should now be pointing to a bit that is off
        while (begin < end) {
            // if there's a flag that's on
            if (flagTester(begin->flagID)) {
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
        constexpr auto makeMask             = [](uint8_t size) -> uint16_t              { return (1 << size) - 1; };
        constexpr auto getByteIndexFromFlag = [](uint16_t f) -> uint8_t                 { return f >> 8; };
        constexpr auto getBitMaskFromFlag   = [](uint16_t f) -> uint8_t                 { return f & 0xff; };
        constexpr auto getValueSize         = [](uint16_t f) -> uint8_t                 { return std::popcount(getBitMaskFromFlag(f)); };
        
        constexpr auto makeEventFlag = [](uint8_t byteIndex, uint8_t bitIndices) -> uint16_t {
            return (byteIndex << 8) | bitIndices;
        };

        constexpr auto eventFlagToAreaFlag = [](uint16_t areaFlag) -> int {
            auto byteInd = getByteIndexFromFlag(areaFlag);
            constexpr size_t areaIndexSize = 5;
            // if we're looking at 0x580, that would be byte 5, and check if 0x80 is set on that byte
            // the event flags are structured differently than area flags
            // B is byte index, b is the flag mask to check
            // event flags are BBBBBBBB bbbbbbbb
            // for area flags, they check bitIndex, not mask, i is index
            // also area uses u32 index, not byte index
            // area flags are BBBiiiii
            // so we need to convert from bit mask to index
            // also our byte index has to become a u32 index
            
            // dividing byte index by sizeof(u32) gets us the u32 index
            // but in big endian, the first byte sequencially is 24 bits we have to skip
            // so we skip 24 bytes for the first byte, 16 for the second, etc
            // essentially (3 - (x % 4)), reversing the modulus, 0=3, 1=2
            auto bitsToSkip = 8 * ((sizeof(u32) - 1) - (byteInd % sizeof(u32)));
            return ((byteInd / sizeof(u32)) << areaIndexSize) | ((std::countr_zero(areaFlag) + bitsToSkip) & makeMask(areaIndexSize));
        };

        constexpr uint8_t validTbox = sizeof(membit.mTbox);
        constexpr uint8_t validSwitch = validTbox + sizeof(membit.mSwitch);
        constexpr uint8_t validItem = validSwitch + sizeof(membit.mItem);
        constexpr uint16_t tboxConvert = 0;
        constexpr uint16_t switchConvert = sizeof(membit.mTbox) << 8;
        constexpr uint16_t itemConvert = switchConvert + (sizeof(membit.mItem) << 8);

        const auto LoadFlag = [&membit](uint16_t flag) -> bool {            
            const auto byteIndex = getByteIndexFromFlag(flag);
            static_assert((0x1580 - switchConvert) == 0xd80);

            if (byteIndex < validTbox) {
                uint16_t newFlag = flag - tboxConvert;
                return membit.isTbox(eventFlagToAreaFlag(newFlag));
            }
            if (byteIndex < validSwitch) {
                uint16_t newFlag = flag - switchConvert;
                return membit.isSwitch(eventFlagToAreaFlag(newFlag));
            }
            if (byteIndex < validItem) {
                uint16_t newFlag = flag - itemConvert;
                return membit.isItem(eventFlagToAreaFlag(newFlag));
            }
            return false;
        };

        const auto SetFlag = [&membit](uint16_t flag, bool set) -> void {
            const auto byteIndex = getByteIndexFromFlag(flag);
            if (set) {
                if (byteIndex < validTbox) {
                    membit.onTbox(eventFlagToAreaFlag(flag - tboxConvert));
                }
                if (byteIndex < validSwitch) {
                    membit.onSwitch(eventFlagToAreaFlag(flag - switchConvert));
                }
                if (byteIndex < validItem) {
                    membit.onItem(eventFlagToAreaFlag(flag - itemConvert));
                }
            } else {
                if (byteIndex < validTbox) {
                    membit.offTbox(eventFlagToAreaFlag(flag - tboxConvert));
                }
                if (byteIndex < validSwitch) {
                    membit.offSwitch(eventFlagToAreaFlag(flag - switchConvert));
                }
                if (byteIndex < validItem) {
                    membit.offItem(eventFlagToAreaFlag(flag - itemConvert));
                }
            }
        };

        const auto LoadMultiByteFlag = [&](uint16_t flag) -> uint8_t {
            const auto bitInds = getBitMaskFromFlag(flag);
            const auto byteIndex = getByteIndexFromFlag(flag);

            const uint16_t startingMask = std::bit_floor(bitInds);
            uint8_t val = 0;
            for (uint16_t mask = startingMask; (bitInds & mask) != 0; mask >>= 1) {
                val <<= 1;
                if (LoadFlag(makeEventFlag(byteIndex, bitInds & mask))) {
                    val |= 1;
                }
            }
            return val;
        };

        const auto SetMultiByteFlag = [&](uint16_t flag, uint8_t val) -> void {
            const auto bitInds = getBitMaskFromFlag(flag);
            const auto byteIndex = getByteIndexFromFlag(flag);

            const uint16_t startingMask = std::bit_floor(bitInds);
            uint16_t valueMask = 1 << (getValueSize(flag) - 1);

            for (uint16_t bitIndexMask = startingMask; (bitInds & bitIndexMask) != 0;
                 bitIndexMask >>= 1, valueMask >>= 1)
            {
                SetFlag(makeEventFlag(byteIndex, bitInds & bitIndexMask), (val & valueMask) != 0);
            }
        };

        const auto LoadSpreadMultiByte = [&](uint16_t high, uint16_t low) -> uint8_t {
            if (low == AREA_FLAG_NONE)
                return LoadMultiByteFlag(high);
            return (LoadMultiByteFlag(high) << getValueSize(low)) | LoadMultiByteFlag(low);
        };

        const auto SetSpreadMultiByte = [&](uint16_t high, uint16_t low, uint8_t value) -> void {
            if (low == AREA_FLAG_NONE)
                return SetMultiByteFlag(high, value);
            const auto lowerSize = getValueSize(low);
            SetMultiByteFlag(high, value >> lowerSize);
            SetMultiByteFlag(low, value & makeMask(lowerSize));
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
            ImGui::TableSetupColumn("Flag");
            ImGui::TableSetupColumn("Byte:Bit");
            ImGui::TableSetupColumn("Description");
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
                            return l.flagID < r.flagID;
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
                bool flag = LoadFlag(e.flagID);
                if (ImGui::Checkbox("##", &flag)) {
                    SetFlag(e.flagID, flag);
                }

                ImGui::TableNextColumn();
                ImGui::TextUnformatted(formattedBitLocation.c_str());
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(e.description.c_str());
            }
            ImGui::EndTable();
        }

        for (const auto& multiByteFlag : areaFlags.multibyteFlags) {
            auto flagValue = LoadSpreadMultiByte(multiByteFlag.flags[0], multiByteFlag.flags[1]); 

            const char* currentVal = "UNKNOWN";

            auto enumValIter = multiByteFlag.enumValues.find(flagValue);
            if (enumValIter != multiByteFlag.enumValues.end()) {
                currentVal = enumValIter->second;
            }

            if (ImGui::BeginCombo(multiByteFlag.name, currentVal)) {
                for (const auto& [val, name] : multiByteFlag.enumValues) {
                    if (ImGui::Selectable(name)) {
                        SetSpreadMultiByte(multiByteFlag.flags[0], multiByteFlag.flags[1], val);
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

        if (ImGui::TreeNode("Flag Matrix")) {
            genMembitFlags("##TempSceneFlags", membit);
            ImGui::TreePop();
        }

        stage_stag_info_class* pstag = dComIfGp_getStageStagInfo();
        if (pstag != nullptr) {
            int stageNo = dStage_stagInfo_GetSaveTbl(pstag);
            if (ImGui::Button("Save##SaveTempFlags")) {
                dComIfGs_putSave(stageNo);
            }

            ImGui::SameLine();

            if (ImGui::Button("Load##LoadSaveFlags")) {
                dComIfGs_getSave(stageNo);
            }
        }
    }

    void ImGuiSaveEditor::drawFlagsTab() {
        if (ImGui::TreeNode("Current Region Flags")) {
            drawCurrentRegionFlags();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Region Saved Flags")) {
            static const std::map<uint8_t, const char*> regionNames = {
                { 0x00, "Ordon"               },
                { 0x01, "Hyrule Sewers"       },
                { 0x02, "Faron"               },
                { 0x03, "Eldin"               },
                { 0x04, "Lanayru"             },
                { 0x06, "Hyrule Field"        },
                { 0x07, "Sacred Grove"        },
                { 0x08, "Snowpeak"            },
                { 0x09, "Castle Town"         },
                { 0x0A, "Gerudo Desert"       },
                { 0x0B, "Fishing Pond"        },
                { 0x10, "Forest Temple"       },
                { 0x11, "Goron Mines"         },
                { 0x12, "Lakebed Temple"      },
                { 0x13, "Arbiter's Grounds"   },
                { 0x14, "Snowpeak Ruins"      },
                { 0x15, "Temple of Time"      },
                { 0x16, "City in the Sky"     },
                { 0x17, "Palace of Twilight"  },
                { 0x18, "Hyrule Castle"       },
                { 0x19, "Caves"               },
                { 0x1A, "Lake Hylia Long Cave"},
                { 0x1B, "Grottos"             }
            };

            if (m_selectedRegion.name == nullptr)
            {
                const auto& firstRegion = *regionNames.find(0);
                m_selectedRegion = { firstRegion.first, firstRegion.second };
            }

            if (ImGui::BeginCombo("Region", m_selectedRegion.name)) {
                for (const auto& [id, name] : regionNames) {
                    if (ImGui::Selectable(name)) {
                        m_selectedRegion = {id, name};
                    }
                }
                ImGui::EndCombo();
            }

            dSv_memBit_c& membit = dComIfGs_getSaveData()->mSave[m_selectedRegion.id].mBit;

            genAreaFlagTable(m_selectedRegion.id, membit);
            
            if (ImGui::TreeNode("Flag Matrix")) {
                genMembitFlags("##SaveSceneFlags", membit);

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Event Flags")) {
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
                ImGui::TableSetupColumn("Flag");
                ImGui::TableSetupColumn("Name");
                ImGui::TableSetupColumn("Location");
                ImGui::TableSetupColumn("Description");
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
                        const auto testEventFunc = [&event](u16 flag) -> bool { return event.isEventBit(flag); };

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

            if (ImGui::TreeNode("Event Matrix")) {
                for (int e = 0; e < 255; e++) {
                    ImGui::Text("%03d ", e);
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
        InputScalarBE("STAR Game Time (Milliseconds)", ImGuiDataType_U32, &minigame.mHookGameTime);
        InputScalarBE("Snowboard Race Time (Milliseconds)", ImGuiDataType_U32, &minigame.mRaceGameTime);
        InputScalarBE("Fruit-Pop-Flight Score", ImGuiDataType_U32, &minigame.mBalloonScore);
    }

    void ImGuiSaveEditor::drawConfigTab() {
        dSv_player_config_c& config = dComIfGs_getSaveData()->getPlayer().getConfig();
        ImGui::Checkbox("Enable Vibration", (bool*)&config.mVibration);

        static const char* kTargetTypeNames[] = {"Hold", "Switch"};
        if (ImGui::BeginCombo("Target Type", kTargetTypeNames[config.mAttentionType])) {
            if (ImGui::Selectable("Hold")) {
                config.mAttentionType = 0;
            }
            if (ImGui::Selectable("Switch")) {
                config.mAttentionType = 1;
            }
            ImGui::EndCombo();
        }

        static const char* kSoundModeNames[] = { "Mono", "Stereo", "Surround" };
        const char* current = (config.mSoundMode < 3) ? kSoundModeNames[config.mSoundMode]
                                                      : "Unknown";
        if (ImGui::BeginCombo("Sound", current)) {
            for (u8 i = 0; i < 3; i++) {
                bool selected = (config.mSoundMode == i);
                if (ImGui::Selectable(kSoundModeNames[i], selected)) {
                    config.mSoundMode = i;
                }
                if (selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
}
