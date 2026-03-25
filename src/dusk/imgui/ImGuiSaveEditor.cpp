#include "fmt/format.h"
#include "imgui.h"
#include "aurora/gfx.h"

#include "ImGuiConsole.hpp"
#include "ImGuiSaveEditor.hpp"

#include "d/d_com_inf_game.h"
#include "d/d_item_data.h"

#include <map>

namespace dusk {
    std::map<int, std::string> itemMap = {
        {dItemNo_HEART_e, "Heart"},
        {dItemNo_GREEN_RUPEE_e, "Green Rupee"},
        {dItemNo_BLUE_RUPEE_e, "Blue Rupee"},
        {dItemNo_YELLOW_RUPEE_e, "Yellow Rupee"},
        {dItemNo_RED_RUPEE_e, "Red Rupee"},
        {dItemNo_PURPLE_RUPEE_e, "Purple Rupee"},
        {dItemNo_ORANGE_RUPEE_e, "Orange Rupee"},
        {dItemNo_SILVER_RUPEE_e, "Silver Rupee"},
        {dItemNo_S_MAGIC_e, "Small Magic"},
        {dItemNo_L_MAGIC_e, "Large Magic"},
        {dItemNo_BOMB_5_e, "Bombs (5)"},
        {dItemNo_BOMB_10_e, "Bombs (10)"},
        {dItemNo_BOMB_20_e, "Bombs (20)"},
        {dItemNo_BOMB_30_e, "Bombs (30)"},
        {dItemNo_ARROW_10_e, "Arrows (10)"},
        {dItemNo_ARROW_20_e, "Arrows (20)"},
        {dItemNo_ARROW_30_e, "Arrows (30)"},
        {dItemNo_ARROW_1_e, "Arrows (1)"},
        {dItemNo_PACHINKO_SHOT_e, "Pumpkin Seeds"},
        {dItemNo_NOENTRY_19_e, "Unknown"},
        {dItemNo_NOENTRY_20_e, "Unknown"},
        {dItemNo_NOENTRY_21_e, "Unknown"},
        {dItemNo_WATER_BOMB_5_e, "Water Bombs (5)"},
        {dItemNo_WATER_BOMB_10_e, "Water Bombs (10)"},
        {dItemNo_WATER_BOMB_20_e, "Water Bombs (20)"},
        {dItemNo_WATER_BOMB_30_e, "Water Bombs (30)"},
        {dItemNo_BOMB_INSECT_5_e, "Bomblings (5)"},
        {dItemNo_BOMB_INSECT_10_e, "Bomblings (10)"},
        {dItemNo_BOMB_INSECT_20_e, "Bomblings (20)"},
        {dItemNo_BOMB_INSECT_30_e, "Bomblings (30)"},
        {dItemNo_RECOVERY_FAILY_e, "Fairy"},
        {dItemNo_TRIPLE_HEART_e, "Triple Hearts"},
        {dItemNo_SMALL_KEY_e, "Small Key"},
        {dItemNo_KAKERA_HEART_e, "Piece of Heart"},
        {dItemNo_UTAWA_HEART_e, "Heart Container"},
        {dItemNo_MAP_e, "Map"},
        {dItemNo_COMPUS_e, "Compass"},
        {dItemNo_DUNGEON_EXIT_e, "Ooccoo Sr."},
        {dItemNo_BOSS_KEY_e, "Boss Key"},
        {dItemNo_DUNGEON_BACK_e, "Ooccoo Jr."},
        {dItemNo_SWORD_e, "Ordon Sword"},
        {dItemNo_MASTER_SWORD_e, "Master Sword"},
        {dItemNo_WOOD_SHIELD_e, "Wooden Shield"},
        {dItemNo_SHIELD_e, "Ordon Shield"},
        {dItemNo_HYLIA_SHIELD_e, "Hylian Shield"},
        {dItemNo_TKS_LETTER_e, "Ooccoo's Letter"},
        {dItemNo_WEAR_CASUAL_e, "Ordon Clothes"},
        {dItemNo_WEAR_KOKIRI_e, "Hero's Clothes"},
        {dItemNo_ARMOR_e, "Magic Armor"},
        {dItemNo_WEAR_ZORA_e, "Zora Armor"},
        {dItemNo_MAGIC_LV1_e, "Magic Level 1"},
        {dItemNo_DUNGEON_EXIT_2_e, "Ooccoo Sr."},
        {dItemNo_WALLET_LV1_e, "Wallet"},
        {dItemNo_WALLET_LV2_e, "Big Wallet"},
        {dItemNo_WALLET_LV3_e, "Giant Wallet"},
        {dItemNo_NOENTRY_55_e, "Unknown"},
        {dItemNo_NOENTRY_56_e, "Unknown"},
        {dItemNo_NOENTRY_57_e, "Unknown"},
        {dItemNo_NOENTRY_58_e, "Unknown"},
        {dItemNo_NOENTRY_59_e, "Unknown"},
        {dItemNo_NOENTRY_60_e, "Unknown"},
        {dItemNo_ZORAS_JEWEL_e, "Coral Earring"},
        {dItemNo_HAWK_EYE_e, "Hawkeye"},
        {dItemNo_WOOD_STICK_e, "Wooden Sword"},
        {dItemNo_BOOMERANG_e, "Gale Boomerang"},
        {dItemNo_SPINNER_e, "Spinner"},
        {dItemNo_IRONBALL_e, "Ball and Chain"},
        {dItemNo_BOW_e, "Hero's Bow"},
        {dItemNo_HOOKSHOT_e, "Clawshot"},
        {dItemNo_HVY_BOOTS_e, "Iron Boots"},
        {dItemNo_COPY_ROD_e, "Dominion Rod"},
        {dItemNo_W_HOOKSHOT_e, "Double Clawshots"},
        {dItemNo_KANTERA_e, "Lantern"},
        {dItemNo_LIGHT_SWORD_e, "Light Sword"},
        {dItemNo_FISHING_ROD_1_e, "Fishing Rod"},
        {dItemNo_PACHINKO_e, "Slingshot"},
        {dItemNo_COPY_ROD_2_e, "Dominion Rod"},
        {dItemNo_NOENTRY_77_e, "Unknown"},
        {dItemNo_NOENTRY_78_e, "Unknown"},
        {dItemNo_BOMB_BAG_LV2_e, "Giant Bomb Bag"},
        {dItemNo_BOMB_BAG_LV1_e, "Bomb Bag"},
        {dItemNo_BOMB_IN_BAG_e, "Bomb Bag"},
        {dItemNo_NOENTRY_82_e, "Unknown"},
        {dItemNo_LIGHT_ARROW_e, "Light Arrow"},
        {dItemNo_ARROW_LV1_e, "Quiver"},
        {dItemNo_ARROW_LV2_e, "Big Quiver"},
        {dItemNo_ARROW_LV3_e, "Giant Quiver"},
        {dItemNo_NOENTRY_87_e, "Unknown"},
        {dItemNo_LURE_ROD_e, "Fishing Rod (Lure)"},
        {dItemNo_BOMB_ARROW_e, "Bomb Arrow"},
        {dItemNo_HAWK_ARROW_e, "Hawk Arrow"},
        {dItemNo_BEE_ROD_e, "Fishing Rod (Bee Larva)"},
        {dItemNo_JEWEL_ROD_e, "Fishing Rod (Earring)"},
        {dItemNo_WORM_ROD_e, "Fishing Rod (Worm)"},
        {dItemNo_JEWEL_BEE_ROD_e, "Fishing Rod (Earring + Bee Larva)"},
        {dItemNo_JEWEL_WORM_ROD_e, "Fishing Rod (Earring + Worm)"},
        {dItemNo_EMPTY_BOTTLE_e, "Empty Bottle"},
        {dItemNo_RED_BOTTLE_e, "Red Potion"},
        {dItemNo_GREEN_BOTTLE_e, "Green Potion"},
        {dItemNo_BLUE_BOTTLE_e, "Blue Potion"},
        {dItemNo_MILK_BOTTLE_e, "Milk Bottle"},
        {dItemNo_HALF_MILK_BOTTLE_e, "Half Milk Bottle"},
        {dItemNo_OIL_BOTTLE_e, "Lantern Oil"},
        {dItemNo_WATER_BOTTLE_e, "Water Bottle"},
        {dItemNo_OIL_BOTTLE_2_e, "Oil Bottle"},
        {dItemNo_RED_BOTTLE_2_e, "Red Potion"},
        {dItemNo_UGLY_SOUP_e, "Nasty Soup"},
        {dItemNo_HOT_SPRING_e, "Hotspring Water"},
        {dItemNo_FAIRY_e, "Fairy"},
        {dItemNo_HOT_SPRING_2_e, "Hotspring Water"},
        {dItemNo_OIL2_e, "Lantern Oil"},
        {dItemNo_OIL_e, "Lantern Oil"},
        {dItemNo_NORMAL_BOMB_e, "Bombs"},
        {dItemNo_WATER_BOMB_e, "Water Bombs"},
        {dItemNo_POKE_BOMB_e, "Bomblings"},
        {dItemNo_FAIRY_DROP_e, "Great Fairy's Tears"},
        {dItemNo_WORM_e, "Worm"},
        {dItemNo_DROP_BOTTLE_e, ""},
        {dItemNo_BEE_CHILD_e, ""},
        {dItemNo_CHUCHU_RARE_e, "Rare Chu Jelly"},
        {dItemNo_CHUCHU_RED_e, "Red Chu Jelly"},
        {dItemNo_CHUCHU_BLUE_e, "Blue Chu Jelly"},
        {dItemNo_CHUCHU_GREEN_e, "Green Chu Jelly"},
        {dItemNo_CHUCHU_YELLOW_e, "Yellow Chu Jelly"},
        {dItemNo_CHUCHU_PURPLE_e, "Purple Chu Jelly"},
        {dItemNo_LV1_SOUP_e, "Simple Soup"},
        {dItemNo_LV2_SOUP_e, "Good Soup"},
        {dItemNo_LV3_SOUP_e, "Superb Soup"},
        {dItemNo_LETTER_e, "Renado's Letter"},
        {dItemNo_BILL_e, "Invoice"},
        {dItemNo_WOOD_STATUE_e, "Wooden Statue"},
        {dItemNo_IRIAS_PENDANT_e, "Ilia's Charm"},
        {dItemNo_HORSE_FLUTE_e, "Horse Call"},
        {dItemNo_NOENTRY_133_e, "Unknown"},
        {dItemNo_NOENTRY_134_e, "Unknown"},
        {dItemNo_NOENTRY_135_e, "Unknown"},
        {dItemNo_NOENTRY_136_e, "Unknown"},
        {dItemNo_NOENTRY_137_e, "Unknown"},
        {dItemNo_NOENTRY_138_e, "Unknown"},
        {dItemNo_NOENTRY_139_e, "Unknown"},
        {dItemNo_NOENTRY_140_e, "Unknown"},
        {dItemNo_NOENTRY_141_e, "Unknown"},
        {dItemNo_NOENTRY_142_e, "Unknown"},
        {dItemNo_NOENTRY_143_e, "Unknown"},
        {dItemNo_RAFRELS_MEMO_e, "Auru's Memo"},
        {dItemNo_ASHS_SCRIBBLING_e, "Ashei's Sketch"},
        {dItemNo_NOENTRY_146_e, "Unknown"},
        {dItemNo_NOENTRY_147_e, "Unknown"},
        {dItemNo_NOENTRY_148_e, "Unknown"},
        {dItemNo_NOENTRY_149_e, "Unknown"},
        {dItemNo_NOENTRY_150_e, "Unknown"},
        {dItemNo_NOENTRY_151_e, "Unknown"},
        {dItemNo_NOENTRY_152_e, "Unknown"},
        {dItemNo_NOENTRY_153_e, "Unknown"},
        {dItemNo_NOENTRY_154_e, "Unknown"},
        {dItemNo_NOENTRY_155_e, "Unknown"},
        {dItemNo_CHUCHU_YELLOW2_e, "Yellow Chu Jelly"},
        {dItemNo_OIL_BOTTLE3_e, "Lantern Oil"},
        {dItemNo_SHOP_BEE_CHILD_e, ""},
        {dItemNo_CHUCHU_BLACK_e, "Black Chu Jelly"},
        {dItemNo_LIGHT_DROP_e, "Tear of Light"},
        {dItemNo_DROP_CONTAINER_e, "Vessel of Light (Faron)"},
        {dItemNo_DROP_CONTAINER02_e, "Vessel of Light (Eldin)"},
        {dItemNo_DROP_CONTAINER03_e, "Vessel of Light (Lanayru)"},
        {dItemNo_FILLED_CONTAINER_e, "Vessel of Light (Filled)"},
        {dItemNo_MIRROR_PIECE_2_e, "Mirror Shard (Snowpeak Ruins)"},
        {dItemNo_MIRROR_PIECE_3_e, "Mirror Shard (Temple of Time)"},
        {dItemNo_MIRROR_PIECE_4_e, "Mirror Shard (City in the Sky)"},
        {dItemNo_NOENTRY_168_e, "Unknown"},
        {dItemNo_NOENTRY_169_e, "Unknown"},
        {dItemNo_NOENTRY_170_e, "Unknown"},
        {dItemNo_NOENTRY_171_e, "Unknown"},
        {dItemNo_NOENTRY_172_e, "Unknown"},
        {dItemNo_NOENTRY_173_e, "Unknown"},
        {dItemNo_NOENTRY_174_e, "Unknown"},
        {dItemNo_NOENTRY_175_e, "Unknown"},
        {dItemNo_SMELL_YELIA_POUCH_e, "Scent of Ilia"},
        {dItemNo_SMELL_PUMPKIN_e, "Pumpkin Scent"},
        {dItemNo_SMELL_POH_e, "Poe Scent"},
        {dItemNo_SMELL_FISH_e, "Reekfish Scent"},
        {dItemNo_SMELL_CHILDREN_e, "Youth's Scent"},
        {dItemNo_SMELL_MEDICINE_e, "Medicine Scent"},
        {dItemNo_NOENTRY_182_e, "Unknown"},
        {dItemNo_NOENTRY_183_e, "Unknown"},
        {dItemNo_NOENTRY_184_e, "Unknown"},
        {dItemNo_NOENTRY_185_e, "Unknown"},
        {dItemNo_NOENTRY_186_e, "Unknown"},
        {dItemNo_NOENTRY_187_e, "Unknown"},
        {dItemNo_NOENTRY_188_e, "Unknown"},
        {dItemNo_NOENTRY_189_e, "Unknown"},
        {dItemNo_NOENTRY_190_e, "Unknown"},
        {dItemNo_NOENTRY_191_e, "Unknown"},
        {dItemNo_M_BEETLE_e, "Beetle ♂"},
        {dItemNo_F_BEETLE_e, "Beetle ♀"},
        {dItemNo_M_BUTTERFLY_e, "Butterfly ♂"},
        {dItemNo_F_BUTTERFLY_e, "Butterfly ♀"},
        {dItemNo_M_STAG_BEETLE_e, "Stag Beetle ♂"},
        {dItemNo_F_STAG_BEETLE_e, "Stag Beetle ♀"},
        {dItemNo_M_GRASSHOPPER_e, "Grasshopper ♂"},
        {dItemNo_F_GRASSHOPPER_e, "Grasshopper ♀"},
        {dItemNo_M_NANAFUSHI_e, "Phasmid ♂"},
        {dItemNo_F_NANAFUSHI_e, "Phasmid ♀"},
        {dItemNo_M_DANGOMUSHI_e, "Pill Bug ♂"},
        {dItemNo_F_DANGOMUSHI_e, "Pill Bug ♀"},
        {dItemNo_M_MANTIS_e, "Mantis ♂"},
        {dItemNo_F_MANTIS_e, "Mantis ♀"},
        {dItemNo_M_LADYBUG_e, "Ladybug ♂"},
        {dItemNo_F_LADYBUG_e, "Ladybug ♀"},
        {dItemNo_M_SNAIL_e, "Snail ♂"},
        {dItemNo_F_SNAIL_e, "Snail ♀"},
        {dItemNo_M_DRAGONFLY_e, "Dragonfly ♂"},
        {dItemNo_F_DRAGONFLY_e, "Dragonfly ♀"},
        {dItemNo_M_ANT_e, "Ant ♂"},
        {dItemNo_F_ANT_e, "Ant ♀"},
        {dItemNo_M_MAYFLY_e, "Mayfly ♂"},
        {dItemNo_F_MAYFLY_e, "Mayfly ♀"},
        {dItemNo_NOENTRY_216_e, "Unknown"},
        {dItemNo_NOENTRY_217_e, "Unknown"},
        {dItemNo_NOENTRY_218_e, "Unknown"},
        {dItemNo_NOENTRY_219_e, "Unknown"},
        {dItemNo_NOENTRY_220_e, "Unknown"},
        {dItemNo_NOENTRY_221_e, "Unknown"},
        {dItemNo_NOENTRY_222_e, "Unknown"},
        {dItemNo_NOENTRY_223_e, "Unknown"},
        {dItemNo_POU_SPIRIT_e, ""},
        {dItemNo_NOENTRY_225_e, "Unknown"},
        {dItemNo_NOENTRY_226_e, "Unknown"},
        {dItemNo_NOENTRY_227_e, "Unknown"},
        {dItemNo_NOENTRY_228_e, "Unknown"},
        {dItemNo_NOENTRY_229_e, "Unknown"},
        {dItemNo_NOENTRY_230_e, "Unknown"},
        {dItemNo_NOENTRY_231_e, "Unknown"},
        {dItemNo_NOENTRY_232_e, "Unknown"},
        {dItemNo_ANCIENT_DOCUMENT_e, "Ancient Sky Book"},
        {dItemNo_AIR_LETTER_e, "Sky Character"},
        {dItemNo_ANCIENT_DOCUMENT2_e, "Ancient Sky Book (Filled)"},
        {dItemNo_LV7_DUNGEON_EXIT_e, "Ooccoo Sr. (City in the Sky)"},
        {dItemNo_LINKS_SAVINGS_e, "Link's Savings"},
        {dItemNo_SMALL_KEY2_e, ""},
        {dItemNo_POU_FIRE1_e, ""},
        {dItemNo_POU_FIRE2_e, ""},
        {dItemNo_POU_FIRE3_e, ""},
        {dItemNo_POU_FIRE4_e, ""},
        {dItemNo_BOSSRIDER_KEY_e, ""},
        {dItemNo_TOMATO_PUREE_e, ""},
        {dItemNo_TASTE_e, ""},
        {dItemNo_LV5_BOSS_KEY_e, "Bedroom Key"},
        {dItemNo_SURFBOARD_e, ""},
        {dItemNo_KANTERA2_e, ""},
        {dItemNo_L2_KEY_PIECES1_e, "Key Shard (1)"},
        {dItemNo_L2_KEY_PIECES2_e, "Key Shard (2)"},
        {dItemNo_L2_KEY_PIECES3_e, "Key Shard (3)"},
        {dItemNo_KEY_OF_CARAVAN_e, "Bulblin Camp Key"},
        {dItemNo_LV2_BOSS_KEY_e, "Key Shard (Completed)"},
        {dItemNo_KEY_OF_FILONE_e, "Faron Gate Key"},
        {dItemNo_NONE_e, "None"},
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
                    //DrawInventoryTab();
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
            ? fmt::format("Slot {0} ({1})", currentSlotNo, itemMap.find(item.mItems[currentSlotNo])->second)
            : "None";

        // TODO: live update equips
        if (ImGui::BeginCombo(label, defaultLabel.c_str())) {
            if (ImGui::Selectable("None")) {
                selectItemData = 0xFF;
            }

            for (int i = 0; i < 24; i++) {
                u8 itemNo = item.mItems[i];
                if (ImGui::Selectable(fmt::format("Slot {0} ({1})", i, itemMap.find(itemNo)->second).c_str())) {
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


        if (ImGui::BeginCombo("Clothes", itemMap.find(statusA.mSelectEquip[0])->second.c_str())) {
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

        if (ImGui::BeginCombo("Sword", itemMap.find(statusA.mSelectEquip[1])->second.c_str())) {
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

        if (ImGui::BeginCombo("Shield", itemMap.find(statusA.mSelectEquip[2])->second.c_str())) {
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

        if (ImGui::BeginCombo("Scent", itemMap.find(statusA.mSelectEquip[3])->second.c_str())) {
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
                for (int i = 8; i >= 0; i--) {
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