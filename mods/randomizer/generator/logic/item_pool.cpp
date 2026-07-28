#include "item_pool.hpp"

#include "world.hpp"

#include <map>

namespace randomizer::logic::item_pool
{

    std::map<std::string, int> minimalItemPool = {
        {"Shadow Crystal", 1},
        {"Slingshot", 1},
        {"Lantern", 1},
        {"Gale Boomerang", 1},
        {"Iron Boots", 1},
        {"Bomb Bag", 1},
        {"Spinner", 1},
        {"Ball and Chain", 1},

        {"Progressive Fishing Rod", 2},
        {"Progressive Sword", 4},
        {"Progressive Bow", 1},
        {"Progressive Clawshot", 2},
        {"Progressive Dominion Rod", 2},
        {"Progressive Wallet", 2},
        {"Progressive Sky Book", 7},

        {"Aurus Memo", 1},
        {"Asheis Sketch", 1},
        {"Renados Letter", 1},
        {"Invoice", 1},
        {"Wooden Statue", 1},
        {"Ilias Charm", 1},
        {"Zora Armor", 1},
        {"Hylian Shield", 1},
        {"Ordon Shield", 1},
        {"Empty Bottle", 4},
        {"Progressive Hidden Skill", 1},
        {"Poe Soul", 60},

        {"Progressive Fused Shadow", 3},
        {"Progressive Mirror Shard", 4},

        // Golden Bugs
        {"Male Ant", 1},
        {"Female Ant", 1},
        {"Male Beetle", 1},
        {"Female Beetle", 1},
        {"Male Pill Bug", 1},
        {"Female Pill Bug", 1},
        {"Male Phasmid", 1},
        {"Female Phasmid", 1},
        {"Male Grasshopper", 1},
        {"Female Grasshopper", 1},
        {"Male Stag Beetle", 1},
        {"Female Stag Beetle", 1},
        {"Male Butterfly", 1},
        {"Female Butterfly", 1},
        {"Male Ladybug", 1},
        {"Female Ladybug", 1},
        {"Male Mantis", 1},
        {"Female Mantis", 1},
        {"Male Dragonfly", 1},
        {"Female Dragonfly", 1},
        {"Male Dayfly", 1},
        {"Female Dayfly", 1},
        {"Male Snail", 1},
        {"Female Snail", 1},

        // Keys
        {"Gate Keys", 1},
        {"Gerudo Desert Bulblin Camp Key", 1},
        {"North Faron Woods Gate Key", 1},
        {"Forest Temple Small Key", 4},
        {"Goron Mines Small Key", 3},
        {"Lakebed Temple Small Key", 3},
        {"Arbiters Grounds Small Key", 5},
        {"Snowpeak Ruins Small Key", 4},
        {"Ordon Pumpkin", 1},
        {"Ordon Cheese", 1},
        {"Temple of Time Small Key", 3},
        {"City in the Sky Small Key", 1},
        {"Palace of Twilight Small Key", 7},
        {"Hyrule Castle Small Key", 3},

        // Big Keys
        {"Forest Temple Big Key", 1},
        {"Goron Mines Key Shard", 3},
        {"Lakebed Temple Big Key", 1},
        {"Arbiters Grounds Big Key", 1},
        {"Snowpeak Ruins Bedroom Key", 1},
        {"Temple of Time Big Key", 1},
        {"City in the Sky Big Key", 1},
        {"Palace of Twilight Big Key", 1},
        {"Hyrule Castle Big Key", 1},

        // Maps and Compasses
        {"Forest Temple Compass", 1},
        {"Goron Mines Compass", 1},
        {"Lakebed Temple Compass", 1},
        {"Arbiters Grounds Compass", 1},
        {"Snowpeak Ruins Compass", 1},
        {"Temple of Time Compass", 1},
        {"City in the Sky Compass", 1},
        {"Palace of Twilight Compass", 1},
        {"Hyrule Castle Compass", 1},
        {"Forest Temple Dungeon Map", 1},
        {"Goron Mines Dungeon Map", 1},
        {"Lakebed Temple Dungeon Map", 1},
        {"Arbiters Grounds Dungeon Map", 1},
        {"Snowpeak Ruins Dungeon Map", 1},
        {"Temple of Time Dungeon Map", 1},
        {"City in the Sky Dungeon Map", 1},
        {"Palace of Twilight Dungeon Map", 1},
        {"Hyrule Castle Dungeon Map", 1},

        // Warp Portals
        {"Ordon Spring Portal", 1},
        {"South Faron Portal", 1},
        {"North Faron Portal", 1},
        {"Kakariko Gorge Portal", 1},
        {"Kakariko Village Portal", 1},
        {"Death Mountain Portal", 1},
        {"Bridge of Eldin Portal", 1},
        {"Zoras Domain Portal", 1},
        {"Lake Hylia Portal", 1},
        {"Castle Town Portal", 1},
        {"Upper Zoras River Portal", 1},
        {"Snowpeak Portal", 1},
        {"Gerudo Desert Portal", 1},
        {"Mirror Chamber Portal", 1},

        // Tears
        {"Faron Twilight Tear", 16},
        {"Eldin Twilight Tear", 16},
        {"Lanayru Twilight Tear", 16},

        // Junk we should always have
        {"Purple Rupee Links House", 1},
        {"Green Rupee", 2},
        {"Orange Rupee", 50},
        {"Silver Rupee", 2},
    };

    // This is intended to be added on top of the minimal item pool
    std::map<std::string, int> standardItemPool = {
        {"Bomb Bag", 2},
        {"Progressive Bow", 2},
        {"Progressive Wallet", 1},
        {"Magic Armor", 1},
        {"Hawkeye", 1},
        {"Giant Bomb Bag", 1},
        {"Horse Call", 1},
        // {"Bottle with Half Milk", 1},  // Special bottles replace Empty Bottles after the fill algorithm is done
        // {"Bottle with Lantern Oil", 1},
        // {"Bottle with Great Fairies Tears", 1},
        {"Progressive Hidden Skill", 6},

        {"Heart Container", 8},
        {"Piece of Heart", 45},
    };

    // This is intended to be added on top of the minimal and standard pools
    std::map<std::string, int> plentifulItemPool = {
        {"Shadow Crystal", 1},
        {"Slingshot", 1},
        {"Lantern", 1},
        {"Gale Boomerang", 1},
        {"Iron Boots", 1},
        {"Bomb Bag", 1},
        {"Spinner", 1},
        {"Ball and Chain", 1},

        {"Progressive Fishing Rod", 1},
        {"Progressive Sword", 4},
        {"Progressive Bow", 1},
        {"Progressive Clawshot", 1},
        {"Progressive Dominion Rod", 1},
        {"Progressive Wallet", 1},
        {"Progressive Sky Book", 1},

        {"Aurus Memo", 1},
        {"Asheis Sketch", 1},
        // {"Renados Letter", 1}, Vanilla until flag issues are figured out
        {"Zora Armor", 1},
        {"Magic Armor", 1},
        {"Hylian Shield", 1},
        {"Empty Bottle", 1},
        {"Progressive Hidden Skill", 1},

        // Keys
        {"Gate Keys", 1},
        {"Forest Temple Small Key", 1},
        {"Goron Mines Small Key", 1},
        {"Lakebed Temple Small Key", 1},
        {"Arbiters Grounds Small Key", 1},
        {"Snowpeak Ruins Small Key", 1},
        {"Ordon Pumpkin", 1},
        {"Ordon Cheese", 1},
        {"Temple of Time Small Key", 1},
        {"City in the Sky Small Key", 1},
        {"Palace of Twilight Small Key", 1},
        {"Hyrule Castle Small Key", 1},

        // Big Keys
        {"Forest Temple Big Key", 1},
        {"Goron Mines Key Shard", 1},
        {"Lakebed Temple Big Key", 1},
        {"Arbiters Grounds Big Key", 1},
        {"Snowpeak Ruins Bedroom Key", 1},
        {"Temple of Time Big Key", 1},
        {"City in the Sky Big Key", 1},
        {"Palace of Twilight Big Key", 1},
        {"Hyrule Castle Big Key", 1},
    };

    std::map<std::string, int> initialJunkPool = {
        {"Bombs 5", 8},
        {"Bombs 10", 2},
        {"Bombs 20", 1},
        {"Bombs 30", 1},
        {"Arrows 10", 13},
        {"Arrows 20", 6},
        {"Arrows 30", 2},
        {"Seeds 50", 2},
        {"Water Bombs 5", 3},
        {"Water Bombs 10", 5},
        {"Water Bombs 15", 3},
        {"Bomblings 5", 2},
        {"Bomblings 10", 2},
        {"Blue Rupee", 1},
        {"Yellow Rupee", 6},
        {"Red Rupee", 6},
        {"Purple Rupee", 12},
    };

    void GenerateItemPool(world::World* world)
    {
        auto itemPool = minimalItemPool;

        // Minimal item pool things
        if (world->Setting("Item Scarcity") == "Minimal")
        {
            // If glitched logic, include magic armor
        }

        // Add the vanilla item pool if necessary
        if (world->Setting("Item Scarcity").IsAnyOf("Vanilla", "Plentiful"))
        {
            for (const auto& [itemName, count] : standardItemPool)
            {
                itemPool[itemName] += count;
            }
        }

        // Add the plentiful item pool if necessary
        if (world->Setting("Item Scarcity") == "Plentiful")
        {
            for (const auto& [itemName, count] : plentifulItemPool)
            {
                itemPool[itemName] += count;
            }
        }

        // Remove the Tears for Twilight sections if they're cleared
        if (world->Setting("Faron Twilight Cleared") == "On") {
            itemPool.erase("Faron Twilight Tear");
        }

        if (world->Setting("Eldin Twilight Cleared") == "On") {
            itemPool.erase("Eldin Twilight Tear");
        }

        if (world->Setting("Lanayru Twilight Cleared") == "On") {
            itemPool.erase("Lanayru Twilight Tear");
        }

        // Remove items depending on Ilia Memory Quest setting
        const auto& iliaQuest = world->Setting("Ilia Memory Quest");
        if (iliaQuest > "Letter") {
            itemPool.erase("Renados Letter");
        }
        if (iliaQuest > "Invoice") {
            itemPool.erase("Invoice");
        }
        if (iliaQuest > "Statue") {
            itemPool.erase("Wooden Statue");
        }

        // Remove the North Faron Woods Gate Key if we're skipping prologue
        if (world->Setting("Skip Prologue") == "On")
        {
            itemPool.erase("North Faron Woods Gate Key");
        }

        // Remove the bulblin camp key if we're skipping bulblin camp
        if (world->Setting("Arbiters Does Not Require Bulblin Camp") == "On")
        {
            itemPool.erase("Gerudo Desert Bulblin Camp Key");
        }

        // Remove sky book characters if we're starting with the sky canon open
        if (world->Setting("City Does Not Require Filled Skybook") == "On")
        {
            itemPool.erase("Progressive Sky Book");
        }

        // Remove Small Keys if we're playing without them
        if (world->Setting("Small Keys") == "Keysy")
        {
            std::list<std::string> smallKeys = {
                {"Gate Keys"},
                {"Forest Temple Small Key"},
                {"Goron Mines Small Key"},
                {"Lakebed Temple Small Key"},
                {"Arbiters Grounds Small Key"},
                {"Snowpeak Ruins Small Key"},
                {"Ordon Pumpkin"},
                {"Ordon Cheese"},
                {"Temple of Time Small Key"},
                {"City in the Sky Small Key"},
                {"Palace of Twilight Small Key"},
                {"Hyrule Castle Small Key"},
            };
            for (const auto& key : smallKeys)
            {
                itemPool.erase(key);
            }
        }

        // Remove Big Keys if we're playing without them
        if (world->Setting("Big Keys") == "Keysy")
        {
            std::list<std::string> bigKeys = {
                {"Forest Temple Big Key"},
                {"Goron Mines Key Shard"},
                {"Lakebed Temple Big Key"},
                {"Arbiters Grounds Big Key"},
                {"Snowpeak Ruins Bedroom Key"},
                {"Temple of Time Big Key"},
                {"City in the Sky Big Key"},
                {"Palace of Twilight Big Key"},
            };

            if (world->Setting("Hyrule Castle Big Key Requirements") == "None")
            {
                bigKeys.emplace_back("Hyrule Castle Big Key");
            }

            for (const auto& key : bigKeys)
            {
                itemPool.erase(key);
            }
        }

        // Add items to the world's _itemPool
        auto& worldItemPool = world->GetItemPool();
        for (const auto& [itemName, count] : itemPool)
        {
            auto item = world->GetItem(itemName);
            for (auto i = 0; i < count; i++)
            {
                worldItemPool.push_back(item);
            }
        }
    }

    void GenerateStartingItemPool(world::World* world)
    {
        auto startingItems = world->GetSettings().GetStartingInventory();
        auto& startingItemPool = world->GetStartingItemPool();
        auto& itemPool = world->GetItemPool();

        // Add Maps and Compasses to starting items if we start with them
        if (world->Setting("Maps and Compasses") == "Start With")
        {
            std::list<std::string> mapsAndCompasses = {
                {"Forest Temple Compass"},
                {"Goron Mines Compass"},
                {"Lakebed Temple Compass"},
                {"Arbiters Grounds Compass"},
                {"Snowpeak Ruins Compass"},
                {"Temple of Time Compass"},
                {"City in the Sky Compass"},
                {"Palace of Twilight Compass"},
                {"Hyrule Castle Compass"},
                {"Forest Temple Dungeon Map"},
                {"Goron Mines Dungeon Map"},
                {"Lakebed Temple Dungeon Map"},
                {"Arbiters Grounds Dungeon Map"},
                {"Snowpeak Ruins Dungeon Map"},
                {"Temple of Time Dungeon Map"},
                {"City in the Sky Dungeon Map"},
                {"Palace of Twilight Dungeon Map"},
                {"Hyrule Castle Dungeon Map"},
            };

            for (const auto& itemName : mapsAndCompasses)
            {
                startingItems[itemName] = 1;
            }
        }

        // Handle warp portals
        startingItems["Ordon Spring Portal"] = 1;
        if (world->Setting("Faron Twilight Cleared") == "On")
        {
            startingItems["South Faron Portal"] = 1;
            startingItems["North Faron Portal"] = 1;
        }

        if (world->Setting("Eldin Twilight Cleared") == "On")
        {
            startingItems["Kakariko Gorge Portal"] = 1;
            startingItems["Kakariko Village Portal"] = 1;
            startingItems["Death Mountain Portal"] = 1;
        }

        if (world->Setting("Lanayru Twilight Cleared") == "On")
        {
            startingItems["Zoras Domain Portal"] = 1;
            startingItems["Lake Hylia Portal"] = 1;
            startingItems["Castle Town Portal"] = 1;
        }

        // Automatically give players the Mirror Chamber Portal if Mirror Chamber Access is closed
        // and they aren't both randomizing and decoupling dungeon entrances. Otherwise, there's no
        // way to access the chamber
        if (world->Setting("Mirror Chamber Access") == "Closed" &&
          !(world->Setting("Randomize Dungeon Entrances") == "On" && world->Setting("Decouple Entrances") == "On"))
        {
            startingItems["Mirror Chamber Portal"] = 1;
        }

        // Add each item to the world's _startingItemPool and erase it from the regular _itemPool
        for (const auto& [itemName, count] : startingItems)
        {
            auto item = world->GetItem(itemName);
            for (auto i = 0; i < count; i++)
            {
                startingItemPool.push_back(item);
            }
            utility::container::Erase(itemPool, item, count);
        }
    }

    std::map<std::string, int> GetInitialJunkPool()
    {
        return initialJunkPool;
    }

    ItemPool GetCompleteItemPool(const world::WorldPool& worlds)
    {
        ItemPool completeItemPool = {};
        for (const auto& world : worlds)
        {
            auto& worldItemPool = world->GetItemPool();
            std::ranges::copy(worldItemPool, std::back_inserter(completeItemPool));
        }

        return completeItemPool;
    }

    const std::map<std::string, int>& GetValidStartingInventoryItems() {
        static std::map<std::string, int> validStartingInventoryItems{};
        if (validStartingInventoryItems.empty()) {
            validStartingInventoryItems = minimalItemPool;
            for (const auto& [item, count] : standardItemPool) {
                validStartingInventoryItems[item] += count;
            }

            // Remove junk
            validStartingInventoryItems.erase("Purple Rupee Links House");
            validStartingInventoryItems.erase("Green Rupee");
            validStartingInventoryItems.erase("Orange Rupee");
            validStartingInventoryItems.erase("Silver Rupee");
        }
        return validStartingInventoryItems;
    }
} // namespace randomizer::logic::item_pool
