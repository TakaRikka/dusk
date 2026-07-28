#include "hints.hpp"

#include "../utility/text.hpp"
#include "world.hpp"

#include <ranges>

namespace randomizer::logic::hints {

    static const std::list<std::pair<std::string, std::string>> dungeonColors = {
        {"Forest Temple", "<green>"},
        {"Goron Mines", "<red>"},
        {"Lakebed Temple", "<blue>"},
        {"Arbiters Grounds", "<orange>"},
        {"Snowpeak Ruins", "<light blue>"},
        {"Temple of Time", "<dark green>"},
        {"City in the Sky", "<yellow>"},
        {"Palace of Twilight", "<purple>"},
        // {"Hyrule Castle", "<silver>"}
    };

    // Tell the player which dungeons are required on the sign in front of Link's House
    static void GenerateRequiredDungeonsHint(world::WorldPool& worlds) {
        for (const auto& world : worlds) {
            auto& requiredDungeonText = world->AddNewText("Links House Sign");
            // Use dungeonColors to loop through in base game dungeon order
            for (const auto& [dungeonName, color] : dungeonColors) {
                auto dungeon = world->GetDungeon(dungeonName);
                if (dungeon->IsRequired()) {
                    requiredDungeonText += color + getTextObject(dungeonName) + "\n";
                }
            }

            if (requiredDungeonText.Empty()) {
                requiredDungeonText += getTextObject("No Required Dungeons Text");
            }
        }
    }

    static void doItemTextReplacement(const std::unique_ptr<world::World>& world,
                                      const std::string& locationName,
                                      const std::list<std::string>& textNames,
                                      Text::Color color) {
        auto itemName = world->GetLocation(locationName)->GetCurrentItem()->GetName();
        auto itemStandardName = addColor(getTextObject(itemName), color, 1, true);
        auto itemPrettyName = addColor(getTextObject(itemName, Text::PRETTY), color);
        for (const auto& textName : textNames) {
            auto& text = world->AddNewText(textName);
            text = getTextObject(textName + " Template");
            text.Replace("<Item Standard Name>", itemStandardName);
            text.Replace("<Item Pretty Name>", itemPrettyName);
            text.Capitalize();
            text.BreakLines();
        }
    }

    static void GenerateItemTextReplacements(world::WorldPool& worlds) {
        for (const auto& world : worlds) {
            doItemTextReplacement(world, "Fishing Hole Bottle", {"Fishing Hole Sign Text"}, Text::GREEN);
            doItemTextReplacement(world, "Charlo Donation Blessing", {"Charlo Donation Ask Text"}, Text::GREEN);
            doItemTextReplacement(world, "Sera Shop Slingshot", {"Slingshot Shop Text",
                "Slingshot Shop Too Expensive Text", "Slingshot Shop Purchase Confirmation Text",
                "Slingshot Shop After Purchase Text"}, Text::ORANGE);

            doItemTextReplacement(world, "Barnes Bomb Bag", {"Barnes Special Offer Text"}, Text::ORANGE);
            doItemTextReplacement(world, "Kakariko Village Malo Mart Wooden Shield", {"Kakariko Malo Mart Wooden Shield Purchase Confirmation Text",
                "Kakariko Malo Mart Wooden Shield Too Expensive Text", "Kakariko Malo Mart Wooden Shield Text"}, Text::ORANGE);

            doItemTextReplacement(world, "Kakariko Village Malo Mart Hylian Shield", {"Kakariko Malo Mart Hylian Shield Purchase Confirmation Text",
                "Kakariko Malo Mart Hylian Shield Too Expensive Text", "Kakariko Malo Mart Hylian Shield After Purchase Text",
                "Kakariko Malo Mart Hylian Shield Text"}, Text::ORANGE);

            doItemTextReplacement(world, "Kakariko Village Malo Mart Red Potion", {"Kakariko Malo Mart Red Potion Too Expensive Text",
                "Kakariko Malo Mart Red Potion Purchase Confirmation Text", "Kakariko Malo Mart Red Potion Text"}, Text::ORANGE);

            doItemTextReplacement(world, "Kakariko Village Malo Mart Hawkeye", {"Kakariko Malo Mart Hawkeye Purchase Confirmation Text",
                "Kakariko Malo Mart Hawkeye Too Expensive Text", "Kakariko Malo Mart Hawkeye After Purchase Text",
                "Kakariko Malo Mart Hawkeye Coming Soon Text", "Kakariko Malo Mart Hawkeye Text"}, Text::ORANGE);

            doItemTextReplacement(world, "Castle Town Malo Mart Magic Armor", {"Chudleys Shop Magic Armor Text",
                "Castle Town Malo Mart Magic Armor After Purchase Text", "Castle Town Malo Mart Magic Armor Text",
                "Castle Town Malo Mart Magic Armor Sold Out Text"}, Text::ORANGE);

            doItemTextReplacement(world, "Coro Bottle", {"Coro Bottle Offer 1 Text",
                "Coro Bottle Offer 2 Text", "Coro Bottle Offer 3 Text", "Coro Bottle Offer 4 Text"}, Text::ORANGE);
        }
    }

    void GenerateMidnaHintsText(world::WorldPool& worlds) {
        for (const auto& world : worlds) {
            auto& midnaHintText = world->AddNewText("Custom Midna Call Hints Text");

            // Put required dungeons on Midna.
            // First loop through to get required number
            int numRequiredDungeons = 0;
            for (const auto& dungeon : world->GetDungeonTable() | std::views::values) {
                if (dungeon->IsRequired()) {
                    ++numRequiredDungeons;
                }
            }

            // Set the text for the number of required dungeons
            if (numRequiredDungeons > 0) {
                midnaHintText += getTextObject("Midna Hints Required Dungeons Intro At Least One Dungeon");
                midnaHintText.Replace("<required dungeon count>", std::to_string(numRequiredDungeons));
                // Add newlines to begin listing dungeons on the next textbox
                midnaHintText += "\n\n\n\n";

                // Then loop through again to add the dungeon names.
                // Use dungeonColors to loop through in base game dungeon order
                int displayedRequiredDungeons = 0;
                for (const auto& [dungeonName, color] : dungeonColors) {
                    auto dungeon = world->GetDungeon(dungeonName);
                    if (dungeon->IsRequired()) {
                        ++displayedRequiredDungeons;
                        midnaHintText += color + getTextObject(dungeonName) + "<white>";
                        // Add newline after every dungeon except the last one
                        if (displayedRequiredDungeons < numRequiredDungeons) {
                            midnaHintText += "\n";
                        }
                    }
                }
            } else {
                midnaHintText += getTextObject("Midna Hints Required Dungeons Intro Zero Dungeons");
            }

            midnaHintText.BreakLines();
        }
    }

    void GenerateAllHints(world::WorldPool& worlds) {
        GenerateRequiredDungeonsHint(worlds);
        GenerateItemTextReplacements(worlds);
        GenerateMidnaHintsText(worlds);
    }
}
