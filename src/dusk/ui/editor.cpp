#include "editor.hpp"

#include <RmlUi/Core.h>
#include <fmt/format.h>

#include "button.hpp"
#include "number_button.hpp"
#include "pane.hpp"
#include "select_button.hpp"
#include "string_button.hpp"

namespace dusk::ui {
namespace {

bool has_save_data() {
    return dComIfGs_getSaveData() != nullptr;
}

dSv_player_status_a_c* get_player_status() {
    if (!has_save_data()) {
        return nullptr;
    }
    return &dComIfGs_getSaveData()->getPlayer().getPlayerStatusA();
}

Rml::String get_player_name() {
    if (!has_save_data()) {
        return "";
    }
    return dComIfGs_getPlayerName();
}

void set_player_name(Rml::String name) {
    dComIfGs_setPlayerName(name.c_str());
}

Rml::String get_horse_name() {
    if (!has_save_data()) {
        return "";
    }
    return dComIfGs_getHorseName();
}

void set_horse_name(Rml::String name) {
    dComIfGs_setHorseName(name.c_str());
}

enum ItemType {
    ITEMTYPE_DEFAULT_e,
    ITEMTYPE_EQUIP_e,
};

struct itemInfo {
    std::string m_name;
    u8 m_type = ITEMTYPE_DEFAULT_e;
};

std::map<int, itemInfo> itemMap = {
    {dItemNo_HEART_e, {"Heart"}},
    {dItemNo_GREEN_RUPEE_e, {"Green Rupee"}},
    {dItemNo_BLUE_RUPEE_e, {"Blue Rupee"}},
    {dItemNo_YELLOW_RUPEE_e, {"Yellow Rupee"}},
    {dItemNo_RED_RUPEE_e, {"Red Rupee"}},
    {dItemNo_PURPLE_RUPEE_e, {"Purple Rupee"}},
    {dItemNo_ORANGE_RUPEE_e, {"Orange Rupee"}},
    {dItemNo_SILVER_RUPEE_e, {"Silver Rupee"}},
    {dItemNo_S_MAGIC_e, {"Small Magic"}},
    {dItemNo_L_MAGIC_e, {"Large Magic"}},
    {dItemNo_BOMB_5_e, {"Bombs (5)"}},
    {dItemNo_BOMB_10_e, {"Bombs (10)"}},
    {dItemNo_BOMB_20_e, {"Bombs (20)"}},
    {dItemNo_BOMB_30_e, {"Bombs (30)"}},
    {dItemNo_ARROW_10_e, {"Arrows (10)"}},
    {dItemNo_ARROW_20_e, {"Arrows (20)"}},
    {dItemNo_ARROW_30_e, {"Arrows (30)"}},
    {dItemNo_ARROW_1_e, {"Arrows (1)"}},
    {dItemNo_PACHINKO_SHOT_e, {"Pumpkin Seeds"}},
    {dItemNo_NOENTRY_19_e, {"Reserved"}},
    {dItemNo_NOENTRY_20_e, {"Reserved"}},
    {dItemNo_NOENTRY_21_e, {"Reserved"}},
    {dItemNo_WATER_BOMB_5_e, {"Water Bombs (5)"}},
    {dItemNo_WATER_BOMB_10_e, {"Water Bombs (10)"}},
    {dItemNo_WATER_BOMB_20_e, {"Water Bombs (20)"}},
    {dItemNo_WATER_BOMB_30_e, {"Water Bombs (30)"}},
    {dItemNo_BOMB_INSECT_5_e, {"Bomblings (5)"}},
    {dItemNo_BOMB_INSECT_10_e, {"Bomblings (10)"}},
    {dItemNo_BOMB_INSECT_20_e, {"Bomblings (20)"}},
    {dItemNo_BOMB_INSECT_30_e, {"Bomblings (30)"}},
    {dItemNo_RECOVERY_FAILY_e, {"Fairy"}},
    {dItemNo_TRIPLE_HEART_e, {"Triple Hearts"}},
    {dItemNo_SMALL_KEY_e, {"Small Key"}},
    {dItemNo_KAKERA_HEART_e, {"Piece of Heart"}},
    {dItemNo_UTAWA_HEART_e, {"Heart Container"}},
    {dItemNo_MAP_e, {"Dungeon Map"}},
    {dItemNo_COMPUS_e, {"Compass"}},
    {dItemNo_DUNGEON_EXIT_e, {"Ooccoo Sr. (First Time)", ITEMTYPE_EQUIP_e}},
    {dItemNo_BOSS_KEY_e, {"Boss Key"}},
    {dItemNo_DUNGEON_BACK_e, {"Ooccoo Jr.", ITEMTYPE_EQUIP_e}},
    {dItemNo_SWORD_e, {"Ordon Sword"}},
    {dItemNo_MASTER_SWORD_e, {"Master Sword"}},
    {dItemNo_WOOD_SHIELD_e, {"Wooden Shield"}},
    {dItemNo_SHIELD_e, {"Ordon Shield"}},
    {dItemNo_HYLIA_SHIELD_e, {"Hylian Shield"}},
    {dItemNo_TKS_LETTER_e, {"Ooccoo's Note", ITEMTYPE_EQUIP_e}},
    {dItemNo_WEAR_CASUAL_e, {"Ordon Clothes"}},
    {dItemNo_WEAR_KOKIRI_e, {"Hero's Clothes"}},
    {dItemNo_ARMOR_e, {"Magic Armor"}},
    {dItemNo_WEAR_ZORA_e, {"Zora Armor"}},
    {dItemNo_MAGIC_LV1_e, {"Magic Level 1"}},
    {dItemNo_DUNGEON_EXIT_2_e, {"Ooccoo Sr.", ITEMTYPE_EQUIP_e}},
    {dItemNo_WALLET_LV1_e, {"Wallet"}},
    {dItemNo_WALLET_LV2_e, {"Big Wallet"}},
    {dItemNo_WALLET_LV3_e, {"Giant Wallet"}},
    {dItemNo_NOENTRY_55_e, {"Reserved"}},
    {dItemNo_NOENTRY_56_e, {"Reserved"}},
    {dItemNo_NOENTRY_57_e, {"Reserved"}},
    {dItemNo_NOENTRY_58_e, {"Reserved"}},
    {dItemNo_NOENTRY_59_e, {"Reserved"}},
    {dItemNo_NOENTRY_60_e, {"Reserved"}},
    {dItemNo_ZORAS_JEWEL_e, {"Coral Earring", ITEMTYPE_EQUIP_e}},
    {dItemNo_HAWK_EYE_e, {"Hawkeye", ITEMTYPE_EQUIP_e}},
    {dItemNo_WOOD_STICK_e, {"Wooden Sword"}},
    {dItemNo_BOOMERANG_e, {"Gale Boomerang", ITEMTYPE_EQUIP_e}},
    {dItemNo_SPINNER_e, {"Spinner", ITEMTYPE_EQUIP_e}},
    {dItemNo_IRONBALL_e, {"Ball and Chain", ITEMTYPE_EQUIP_e}},
    {dItemNo_BOW_e, {"Hero's Bow", ITEMTYPE_EQUIP_e}},
    {dItemNo_HOOKSHOT_e, {"Clawshot", ITEMTYPE_EQUIP_e}},
    {dItemNo_HVY_BOOTS_e, {"Iron Boots", ITEMTYPE_EQUIP_e}},
    {dItemNo_COPY_ROD_e, {"Dominion Rod", ITEMTYPE_EQUIP_e}},
    {dItemNo_W_HOOKSHOT_e, {"Double Clawshots", ITEMTYPE_EQUIP_e}},
    {dItemNo_KANTERA_e, {"Lantern", ITEMTYPE_EQUIP_e}},
    {dItemNo_LIGHT_SWORD_e, {"Light Sword"}},
    {dItemNo_FISHING_ROD_1_e, {"Fishing Rod", ITEMTYPE_EQUIP_e}},
    {dItemNo_PACHINKO_e, {"Slingshot", ITEMTYPE_EQUIP_e}},
    {dItemNo_COPY_ROD_2_e, {"Dominion Rod (Uncharged)"}},
    {dItemNo_NOENTRY_77_e, {"Reserved"}},
    {dItemNo_NOENTRY_78_e, {"Reserved"}},
    {dItemNo_BOMB_BAG_LV2_e, {"Giant Bomb Bag"}},
    {dItemNo_BOMB_BAG_LV1_e, {"Empty Bomb Bag", ITEMTYPE_EQUIP_e}},
    {dItemNo_BOMB_IN_BAG_e, {"Bomb Bag"}},
    {dItemNo_NOENTRY_82_e, {"Reserved"}},
    {dItemNo_LIGHT_ARROW_e, {"Light Arrow"}},
    {dItemNo_ARROW_LV1_e, {"Quiver"}},
    {dItemNo_ARROW_LV2_e, {"Big Quiver"}},
    {dItemNo_ARROW_LV3_e, {"Giant Quiver"}},
    {dItemNo_NOENTRY_87_e, {"Reserved"}},
    {dItemNo_LURE_ROD_e, {"Fishing Rod (Lure)"}},
    {dItemNo_BOMB_ARROW_e, {"Bomb Arrow"}},
    {dItemNo_HAWK_ARROW_e, {"Hawk Arrow"}},
    {dItemNo_BEE_ROD_e, {"Fishing Rod (Bee Larva)", ITEMTYPE_EQUIP_e}},
    {dItemNo_JEWEL_ROD_e, {"Fishing Rod (Earring)", ITEMTYPE_EQUIP_e}},
    {dItemNo_WORM_ROD_e, {"Fishing Rod (Worm)", ITEMTYPE_EQUIP_e}},
    {dItemNo_JEWEL_BEE_ROD_e, {"Fishing Rod (Earring + Bee Larva)", ITEMTYPE_EQUIP_e}},
    {dItemNo_JEWEL_WORM_ROD_e, {"Fishing Rod (Earring + Worm)", ITEMTYPE_EQUIP_e}},
    {dItemNo_EMPTY_BOTTLE_e, {"Empty Bottle", ITEMTYPE_EQUIP_e}},
    {dItemNo_RED_BOTTLE_e, {"Red Potion", ITEMTYPE_EQUIP_e}},
    {dItemNo_GREEN_BOTTLE_e, {"Green Potion", ITEMTYPE_EQUIP_e}},
    {dItemNo_BLUE_BOTTLE_e, {"Blue Potion", ITEMTYPE_EQUIP_e}},
    {dItemNo_MILK_BOTTLE_e, {"Milk Bottle", ITEMTYPE_EQUIP_e}},
    {dItemNo_HALF_MILK_BOTTLE_e, {"Half Milk Bottle", ITEMTYPE_EQUIP_e}},
    {dItemNo_OIL_BOTTLE_e, {"Lantern Oil", ITEMTYPE_EQUIP_e}},
    {dItemNo_WATER_BOTTLE_e, {"Water Bottle", ITEMTYPE_EQUIP_e}},
    {dItemNo_OIL_BOTTLE_2_e, {"Lantern Oil (Scooped)"}},
    {dItemNo_RED_BOTTLE_2_e, {"Red Potion (Scooped)"}},
    {dItemNo_UGLY_SOUP_e, {"Nasty Soup", ITEMTYPE_EQUIP_e}},
    {dItemNo_HOT_SPRING_e, {"Hotspring Water", ITEMTYPE_EQUIP_e}},
    {dItemNo_FAIRY_e, {"Fairy", ITEMTYPE_EQUIP_e}},
    {dItemNo_HOT_SPRING_2_e, {"Hotspring Water (Shop)"}},
    {dItemNo_OIL2_e, {"Lantern Refill (Scooped)"}},
    {dItemNo_OIL_e, {"Lantern Refill (Shop)"}},
    {dItemNo_NORMAL_BOMB_e, {"Bombs", ITEMTYPE_EQUIP_e}},
    {dItemNo_WATER_BOMB_e, {"Water Bombs", ITEMTYPE_EQUIP_e}},
    {dItemNo_POKE_BOMB_e, {"Bomblings", ITEMTYPE_EQUIP_e}},
    {dItemNo_FAIRY_DROP_e, {"Great Fairy's Tears", ITEMTYPE_EQUIP_e}},
    {dItemNo_WORM_e, {"Worm", ITEMTYPE_EQUIP_e}},
    {dItemNo_DROP_BOTTLE_e, {"Great Fairy Tears (Jovani)"}},
    {dItemNo_BEE_CHILD_e, {"Bee Larva", ITEMTYPE_EQUIP_e}},
    {dItemNo_CHUCHU_RARE_e, {"Rare Chu Jelly", ITEMTYPE_EQUIP_e}},
    {dItemNo_CHUCHU_RED_e, {"Red Chu Jelly", ITEMTYPE_EQUIP_e}},
    {dItemNo_CHUCHU_BLUE_e, {"Blue Chu Jelly", ITEMTYPE_EQUIP_e}},
    {dItemNo_CHUCHU_GREEN_e, {"Green Chu Jelly", ITEMTYPE_EQUIP_e}},
    {dItemNo_CHUCHU_YELLOW_e, {"Yellow Chu Jelly", ITEMTYPE_EQUIP_e}},
    {dItemNo_CHUCHU_PURPLE_e, {"Purple Chu Jelly", ITEMTYPE_EQUIP_e}},
    {dItemNo_LV1_SOUP_e, {"Simple Soup", ITEMTYPE_EQUIP_e}},
    {dItemNo_LV2_SOUP_e, {"Good Soup", ITEMTYPE_EQUIP_e}},
    {dItemNo_LV3_SOUP_e, {"Superb Soup", ITEMTYPE_EQUIP_e}},
    {dItemNo_LETTER_e, {"Renado's Letter", ITEMTYPE_EQUIP_e}},
    {dItemNo_BILL_e, {"Invoice", ITEMTYPE_EQUIP_e}},
    {dItemNo_WOOD_STATUE_e, {"Wooden Statue", ITEMTYPE_EQUIP_e}},
    {dItemNo_IRIAS_PENDANT_e, {"Ilia's Charm", ITEMTYPE_EQUIP_e}},
    {dItemNo_HORSE_FLUTE_e, {"Horse Call", ITEMTYPE_EQUIP_e}},
    {dItemNo_NOENTRY_133_e, {"Reserved"}},
    {dItemNo_NOENTRY_134_e, {"Reserved"}},
    {dItemNo_NOENTRY_135_e, {"Reserved"}},
    {dItemNo_NOENTRY_136_e, {"Reserved"}},
    {dItemNo_NOENTRY_137_e, {"Reserved"}},
    {dItemNo_NOENTRY_138_e, {"Reserved"}},
    {dItemNo_NOENTRY_139_e, {"Reserved"}},
    {dItemNo_NOENTRY_140_e, {"Reserved"}},
    {dItemNo_NOENTRY_141_e, {"Reserved"}},
    {dItemNo_NOENTRY_142_e, {"Reserved"}},
    {dItemNo_NOENTRY_143_e, {"Reserved"}},
    {dItemNo_RAFRELS_MEMO_e, {"Auru's Memo", ITEMTYPE_EQUIP_e}},
    {dItemNo_ASHS_SCRIBBLING_e, {"Ashei's Sketch", ITEMTYPE_EQUIP_e}},
    {dItemNo_NOENTRY_146_e, {"Reserved"}},
    {dItemNo_NOENTRY_147_e, {"Reserved"}},
    {dItemNo_NOENTRY_148_e, {"Reserved"}},
    {dItemNo_NOENTRY_149_e, {"Reserved"}},
    {dItemNo_NOENTRY_150_e, {"Reserved"}},
    {dItemNo_NOENTRY_151_e, {"Reserved"}},
    {dItemNo_NOENTRY_152_e, {"Reserved"}},
    {dItemNo_NOENTRY_153_e, {"Reserved"}},
    {dItemNo_NOENTRY_154_e, {"Reserved"}},
    {dItemNo_NOENTRY_155_e, {"Reserved"}},
    {dItemNo_CHUCHU_YELLOW2_e, {"Lantern Refill (Yellow Chu)"}},
    {dItemNo_OIL_BOTTLE3_e, {"Lantern Oil (Coro)"}},
    {dItemNo_SHOP_BEE_CHILD_e, {"Bee Larve (Shop)"}},
    {dItemNo_CHUCHU_BLACK_e, {"Black Chu Jelly", ITEMTYPE_EQUIP_e}},
    {dItemNo_LIGHT_DROP_e, {"Tear of Light"}},
    {dItemNo_DROP_CONTAINER_e, {"Vessel of Light (Faron)"}},
    {dItemNo_DROP_CONTAINER02_e, {"Vessel of Light (Eldin)"}},
    {dItemNo_DROP_CONTAINER03_e, {"Vessel of Light (Lanayru)"}},
    {dItemNo_FILLED_CONTAINER_e, {"Vessel of Light (Filled)"}},
    {dItemNo_MIRROR_PIECE_2_e, {"Mirror Shard (Snowpeak Ruins)"}},
    {dItemNo_MIRROR_PIECE_3_e, {"Mirror Shard (Temple of Time)"}},
    {dItemNo_MIRROR_PIECE_4_e, {"Mirror Shard (City in the Sky)"}},
    {dItemNo_NOENTRY_168_e, {"Reserved"}},
    {dItemNo_NOENTRY_169_e, {"Reserved"}},
    {dItemNo_NOENTRY_170_e, {"Reserved"}},
    {dItemNo_NOENTRY_171_e, {"Reserved"}},
    {dItemNo_NOENTRY_172_e, {"Reserved"}},
    {dItemNo_NOENTRY_173_e, {"Reserved"}},
    {dItemNo_NOENTRY_174_e, {"Reserved"}},
    {dItemNo_NOENTRY_175_e, {"Reserved"}},
    {dItemNo_SMELL_YELIA_POUCH_e, {"Scent of Ilia"}},
    {dItemNo_SMELL_PUMPKIN_e, {"Pumpkin Scent"}},
    {dItemNo_SMELL_POH_e, {"Poe Scent"}},
    {dItemNo_SMELL_FISH_e, {"Reekfish Scent"}},
    {dItemNo_SMELL_CHILDREN_e, {"Youth's Scent"}},
    {dItemNo_SMELL_MEDICINE_e, {"Medicine Scent"}},
    {dItemNo_NOENTRY_182_e, {"Reserved"}},
    {dItemNo_NOENTRY_183_e, {"Reserved"}},
    {dItemNo_NOENTRY_184_e, {"Reserved"}},
    {dItemNo_NOENTRY_185_e, {"Reserved"}},
    {dItemNo_NOENTRY_186_e, {"Reserved"}},
    {dItemNo_NOENTRY_187_e, {"Reserved"}},
    {dItemNo_NOENTRY_188_e, {"Reserved"}},
    {dItemNo_NOENTRY_189_e, {"Reserved"}},
    {dItemNo_NOENTRY_190_e, {"Reserved"}},
    {dItemNo_NOENTRY_191_e, {"Reserved"}},
    {dItemNo_M_BEETLE_e, {"Beetle (M)"}},
    {dItemNo_F_BEETLE_e, {"Beetle (F)"}},
    {dItemNo_M_BUTTERFLY_e, {"Butterfly (M)"}},
    {dItemNo_F_BUTTERFLY_e, {"Butterfly (F)"}},
    {dItemNo_M_STAG_BEETLE_e, {"Stag Beetle (M)"}},
    {dItemNo_F_STAG_BEETLE_e, {"Stag Beetle (F)"}},
    {dItemNo_M_GRASSHOPPER_e, {"Grasshopper (M)"}},
    {dItemNo_F_GRASSHOPPER_e, {"Grasshopper (F)"}},
    {dItemNo_M_NANAFUSHI_e, {"Phasmid (M)"}},
    {dItemNo_F_NANAFUSHI_e, {"Phasmid (F)"}},
    {dItemNo_M_DANGOMUSHI_e, {"Pill Bug (M)"}},
    {dItemNo_F_DANGOMUSHI_e, {"Pill Bug (F)"}},
    {dItemNo_M_MANTIS_e, {"Mantis (M)"}},
    {dItemNo_F_MANTIS_e, {"Mantis (F)"}},
    {dItemNo_M_LADYBUG_e, {"Ladybug (M)"}},
    {dItemNo_F_LADYBUG_e, {"Ladybug (F)"}},
    {dItemNo_M_SNAIL_e, {"Snail (M)"}},
    {dItemNo_F_SNAIL_e, {"Snail (F)"}},
    {dItemNo_M_DRAGONFLY_e, {"Dragonfly (M)"}},
    {dItemNo_F_DRAGONFLY_e, {"Dragonfly (F)"}},
    {dItemNo_M_ANT_e, {"Ant (M)"}},
    {dItemNo_F_ANT_e, {"Ant (F)"}},
    {dItemNo_M_MAYFLY_e, {"Mayfly (M)"}},
    {dItemNo_F_MAYFLY_e, {"Mayfly (F)"}},
    {dItemNo_NOENTRY_216_e, {"Reserved"}},
    {dItemNo_NOENTRY_217_e, {"Reserved"}},
    {dItemNo_NOENTRY_218_e, {"Reserved"}},
    {dItemNo_NOENTRY_219_e, {"Reserved"}},
    {dItemNo_NOENTRY_220_e, {"Reserved"}},
    {dItemNo_NOENTRY_221_e, {"Reserved"}},
    {dItemNo_NOENTRY_222_e, {"Reserved"}},
    {dItemNo_NOENTRY_223_e, {"Reserved"}},
    {dItemNo_POU_SPIRIT_e, {"Poe Soul"}},
    {dItemNo_NOENTRY_225_e, {"Reserved"}},
    {dItemNo_NOENTRY_226_e, {"Reserved"}},
    {dItemNo_NOENTRY_227_e, {"Reserved"}},
    {dItemNo_NOENTRY_228_e, {"Reserved"}},
    {dItemNo_NOENTRY_229_e, {"Reserved"}},
    {dItemNo_NOENTRY_230_e, {"Reserved"}},
    {dItemNo_NOENTRY_231_e, {"Reserved"}},
    {dItemNo_NOENTRY_232_e, {"Reserved"}},
    {dItemNo_ANCIENT_DOCUMENT_e, {"Ancient Sky Book", ITEMTYPE_EQUIP_e}},
    {dItemNo_AIR_LETTER_e, {"Ancient Sky Book (Partial)", ITEMTYPE_EQUIP_e}},
    {dItemNo_ANCIENT_DOCUMENT2_e, {"Ancient Sky Book (Filled)", ITEMTYPE_EQUIP_e}},
    {dItemNo_LV7_DUNGEON_EXIT_e, {"Ooccoo Sr. (City in the Sky)"}},
    {dItemNo_LINKS_SAVINGS_e, {"Purple Rupee (Link's Savings)"}},
    {dItemNo_SMALL_KEY2_e, {"Small Key (North Faron Gate)"}},
    {dItemNo_POU_FIRE1_e, {"Poe Fire 1"}},
    {dItemNo_POU_FIRE2_e, {"Poe Fire 2"}},
    {dItemNo_POU_FIRE3_e, {"Poe Fire 3"}},
    {dItemNo_POU_FIRE4_e, {"Poe Fire 4"}},
    {dItemNo_BOSSRIDER_KEY_e, {"Hyrule Field Keys"}},
    {dItemNo_TOMATO_PUREE_e, {"Ordon Pumpkin", ITEMTYPE_EQUIP_e}},
    {dItemNo_TASTE_e, {"Ordon Goat Cheese", ITEMTYPE_EQUIP_e}},
    {dItemNo_LV5_BOSS_KEY_e, {"Bedroom Key"}},
    {dItemNo_SURFBOARD_e, {"Surf Leaf"}},
    {dItemNo_KANTERA2_e, {"Lantern (Reclaimed)"}},
    {dItemNo_L2_KEY_PIECES1_e, {"Key Shard (1)"}},
    {dItemNo_L2_KEY_PIECES2_e, {"Key Shard (2)"}},
    {dItemNo_L2_KEY_PIECES3_e, {"Key Shard (3)"}},
    {dItemNo_KEY_OF_CARAVAN_e, {"Bulblin Camp Key"}},
    {dItemNo_LV2_BOSS_KEY_e, {"Goron Mines Boss Key"}},
    {dItemNo_KEY_OF_FILONE_e, {"South Faron Gate Key"}},
    {dItemNo_NONE_e, {"None"}},
};

Rml::String item_label_for_slot(u8 slot) {
    if (slot == 0xFF) {
        return "None";
    }
    auto& item = dComIfGs_getSaveData()->getPlayer().getItem();
    auto itemNo = item.mItems[slot];
    return fmt::format("Slot {0} ({1})", slot, itemMap.find(itemNo)->second.m_name);
}

}  // namespace

EditorWindow::EditorWindow() {
    add_tab("Player Status", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Direction::Vertical);
        auto& rightPane = add_child<Pane>(content, Pane::Direction::Vertical);

        leftPane.add_section("Player");
        leftPane
            .add_child<StringButton>(StringButton::Props{
                .key = "Player Name",
                .getValue = get_player_name,
                .setValue = set_player_name,
                .maxLength = 16,
            })
            .on_hover([&rightPane](Rml::Event&) { rightPane.clear(); });
        leftPane
            .add_child<StringButton>(StringButton::Props{
                .key = "Horse Name",
                .getValue = get_horse_name,
                .setValue = set_horse_name,
                .maxLength = 16,
            })
            .on_hover([&rightPane](Rml::Event&) { rightPane.clear(); });
        ;
        leftPane
            .add_child<NumberButton>(NumberButton::Props{
                .key = "Max Health",
                .getValue = [] { return get_player_status()->getMaxLife(); },
                .setValue = [](int value) { return get_player_status()->setMaxLife(value); },
                .max = UINT16_MAX,  // TODO: actual max
            })
            .on_hover([&rightPane](Rml::Event&) { rightPane.clear(); });
        leftPane
            .add_child<NumberButton>(NumberButton::Props{
                .key = "Health",
                .getValue = [] { return get_player_status()->getLife(); },
                .setValue = [](int value) { return get_player_status()->setLife(value); },
                .max = UINT16_MAX,  // TODO: actual max
            })
            .on_hover([&rightPane](Rml::Event&) { rightPane.clear(); });
        leftPane
            .add_child<NumberButton>(NumberButton::Props{
                .key = "Rupees",
                .getValue = [] { return get_player_status()->getRupee(); },
                .setValue = [](int value) { return get_player_status()->setRupee(value); },
                .max = get_player_status()->getRupeeMax(),
            })
            .on_hover([&rightPane](Rml::Event&) { rightPane.clear(); });
        leftPane
            .add_child<NumberButton>(NumberButton::Props{
                .key = "Max Oil",
                .getValue = [] { return get_player_status()->getMaxOil(); },
                .setValue = [](int value) { return get_player_status()->setMaxOil(value); },
                .max = UINT16_MAX,  // TODO: actual max
            })
            .on_hover([&rightPane](Rml::Event&) { rightPane.clear(); });
        leftPane
            .add_child<NumberButton>(NumberButton::Props{
                .key = "Oil",
                .getValue = [] { return get_player_status()->getOil(); },
                .setValue = [](int value) { return get_player_status()->setOil(value); },
                .max = UINT16_MAX,  // TODO: actual max
            })
            .on_hover([&rightPane](Rml::Event&) { rightPane.clear(); });

        leftPane.add_section("Equipment");
        const auto genSelectItemComboBox = [&leftPane, &rightPane](
                                               const Rml::String& label, u8& selectItemData) {
            leftPane
                .add_select_button({
                    .key = label,
                    .value = item_label_for_slot(selectItemData),
                })
                .on_hover([&rightPane, &selectItemData](Rml::Event&) {
                    rightPane.clear();
                    rightPane.add_button("None", [&selectItemData] { selectItemData = 0xFF; });
                    for (int i = 0; i < 24; i++) {
                        rightPane.add_button(
                            item_label_for_slot(i), [i, &selectItemData] { selectItemData = i; });
                    }
                });
        };
        genSelectItemComboBox("Equip X", get_player_status()->mSelectItem[0]);
        genSelectItemComboBox("Equip Y", get_player_status()->mSelectItem[1]);
        genSelectItemComboBox("Combo Equip X", get_player_status()->mMixItem[0]);
        genSelectItemComboBox("Combo Equip Y", get_player_status()->mMixItem[1]);
    });

    add_tab("Location", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Inventory", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Collection", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Flags", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Minigame", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Config", [this](Rml::Element* content) {
        // TODO
    });
}

}  // namespace dusk::ui
