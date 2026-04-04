#include "fmt/format.h"
#include "imgui.h"
#include "aurora/gfx.h"

#include "ImGuiConsole.hpp"
#include "ImGuiSaveEditor.hpp"

#include "d/d_com_inf_game.h"
#include "d/d_item_data.h"

#include <map>

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

    ImGuiSaveEditor::ImGuiSaveEditor() {}

    void ImGuiSaveEditor::draw(bool& open) {
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize;

        ImGui::SetNextWindowBgAlpha(0.65f);
        ImGui::SetNextWindowSizeConstraints(ImVec2(600, 700), ImVec2(600, 700));

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
                    //DrawFlagsTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Flags")) {
                    drawFlagsTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Minigame")) {
                    //DrawFlagsTab();
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
            if (ImGui::Selectable("None")) {
                statusA.mSelectEquip[0] = dItemNo_NONE_e;
            }
            if (ImGui::Selectable("Ordon Clothes")) {
                statusA.mSelectEquip[0] = dItemNo_WEAR_CASUAL_e;
            }
            if (ImGui::Selectable("Hero's Clothes")) {
                statusA.mSelectEquip[0] = dItemNo_WEAR_KOKIRI_e;
            }
            if (ImGui::Selectable("Zora Armor")) {
                statusA.mSelectEquip[0] = dItemNo_WEAR_ZORA_e;
            }
            if (ImGui::Selectable("Magic Armor")) {
                statusA.mSelectEquip[0] = dItemNo_ARMOR_e;
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

    void ImGuiSaveEditor::drawInventoryTab() {
        dSv_player_item_c& item = dComIfGs_getSaveData()->getPlayer().getItem();

        ImGuiBeginGroupPanel("Items", { 200, 100 });
        for (int slot = 0; slot < 24; slot++) {
            ImGui::Text("Slot %02d: ", slot);
            ImGui::SameLine();
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
        }
        ImGuiEndGroupPanel();


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

    void genMembitFlags(const char* id, dSv_memBit_c& membit) {
        ImGuiBeginGroupPanel("Chest", { 100, 100 });
        for (int j = 0; j < 2; j++) {
            drawFlagList(fmt::format("##_tbox{}", j).c_str(), membit.mTbox[j]);
        }
        ImGuiEndGroupPanel();

        ImVec2 cursor = ImGui::GetCursorPos();

        ImGui::SameLine();

        ImGuiBeginGroupPanel("Switch", { 100, 100 });
        for (int j = 0; j < 4; j++) {
            drawFlagList(fmt::format("##_switch{}", j).c_str(), membit.mSwitch[j]);
        }
        ImGuiEndGroupPanel();

        ImGui::SetCursorPos(cursor);

        ImGuiBeginGroupPanel("Item", { 100, 100 });
        for (int j = 0; j < 1; j++) {
            drawFlagList(fmt::format("##_item{}", j).c_str(), membit.mItem[j]);
        }
        ImGuiEndGroupPanel();
    }

    void ImGuiSaveEditor::drawFlagsTab() {
        if (ImGui::TreeNode("Current Region Flags")) {
            dSv_memBit_c& membit = g_dComIfG_gameInfo.info.mMemory.mBit;
            genMembitFlags("##TempSceneFlags", membit);

            int stageNo = dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo());
            if (ImGui::Button("Save##SaveTempFlags")) {
                dComIfGs_putSave(stageNo);
            }

            ImGui::SameLine();

            if (ImGui::Button("Load##LoadSaveFlags")) {
                dComIfGs_getSave(stageNo);
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Region Saved Flags")) {
            static std::array<const char*, 27> regionNames = {
                "Ordon",
                "Hyrule Sewers",
                "Faron",
                "Eldin",
                "Lanayru",
                "Unknown",
                "Hyrule Field",
                "Sacred Grove",
                "Snowpeak",
                "Castle Town",
                "Gerudo Desert",
                "Fishing Pond",
                "Reserved",
                "Reserved",
                "Reserved",
                "Reserved",
                "Forest Temple",
                "Goron Mines",
                "Lakebed Temple",
                "Arbiter's Grounds",
                "Snowpeak Ruins",
                "Temple of Time",
                "City in the Sky",
                "Palace of Twilight",
                "Hyrule Castle",
                "Caves",
                "Grottos",
            };

            if (ImGui::BeginCombo("Region", regionNames[m_selectedRegion])) {
                for (int i = 0; i < regionNames.size(); i++) {
                    if (strcmp(regionNames[i], "Reserved") == 0) continue;

                    if (ImGui::Selectable(regionNames[i])) {
                        m_selectedRegion = i;
                    }
                }

                ImGui::EndCombo();
            }

            dSv_memBit_c* membit = &dComIfGs_getSaveData()->mSave[m_selectedRegion].mBit;
            if (membit != nullptr) {
                genMembitFlags("##SaveSceneFlags", *membit);
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Event Flags")) {
            dSv_event_c& event = dComIfGs_getSaveData()->mEvent;
            for (int e = 0; e < 255; e++) {
                ImGui::Text("%03d ", e);
                ImGui::SameLine();
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
    }

    void ImGuiSaveEditor::drawConfigTab() {
        dSv_player_config_c& config = dComIfGs_getSaveData()->getPlayer().getConfig();
        ImGui::Checkbox("Enable Vibration", (bool*)&config.mVibration);
        if (ImGui::BeginCombo("Target Type", "Hold")) {
            ImGui::EndCombo();
        }
    }
}