#include "fill.hpp"

#include "item_pool.hpp"
#include "search.hpp"
#include "../utility/random.hpp"
#include "../utility/string.hpp"

#include <iostream>
#include <algorithm>

namespace randomizer::logic::fill
{
    void FillWorlds(world::WorldPool& worlds)
    {
        // Place each world's restricted items first
        for (auto& world : worlds)
        {
            PlaceRestrictedItems(world, worlds);
        }

        item_pool::ItemPool itemPool = {};
        location::LocationPool locationPool = {};

        // Combine all worlds' item pools and location pools
        for (const auto& world : worlds)
        {
            for (const auto& item : world->GetItemPool())
            {
                itemPool.emplace_back(item);
            }
            for (const auto& location : world->GetAllLocations())
            {
                locationPool.emplace_back(location);
            }
        }

        // Place remaining major items in progress locations
        auto majorItems =
            utility::container::FilterAndEraseFromVector(itemPool, [](const auto& item) { return item->IsMajor(); });
        auto progressLocations =
            utility::container::FilterFromVector(locationPool,
                                                        [](const auto& location) { return location->IsProgression(); });
        AssumedFill(worlds, majorItems, itemPool, progressLocations);

        // Place Minor items in progression locations if possible
        auto minorItems =
            utility::container::FilterAndEraseFromVector(itemPool, [](const auto& item) { return item->IsMinor(); });
        FastFill(minorItems, progressLocations);

        // If there are still minor items left, add them back to the main item pool
        for (const auto& minorItem : minorItems)
        {
            itemPool.push_back(minorItem);
        }

        // Then place everything else anywhere
        FastFill(itemPool, locationPool);

        // Verify that all logic is satisfied
        auto verifyLogicError = search::VerifyLogic(&worlds);
        if (verifyLogicError.has_value())
        {
            throw std::runtime_error("Not all logic satisfied! Reason:\n" + verifyLogicError.value());
        }
    }

    void AssumedFill(world::WorldPool& worlds,
                     item_pool::ItemPool& itemsToPlacePool,
                     const item_pool::ItemPool& itemsNotYetPlaced,
                     location::LocationPool allowedLocations,
                     const int& worldToFill /* = -1 */)
    {
        // Assumed Fill may sometimes place items in such a way that accidentally locks out being able to place specific items
        // later on. Allow the algorithm to retry a reasonable amount of times before returning an error.
        int retries = 10;
        bool unsuccessfulPlacement = true;
        while (unsuccessfulPlacement)
        {
            if (retries <= 0)
            {
                std::string errorMsg = "Ran out of retries while attempting to place the following items:\n";
                const auto count = itemsToPlacePool.size() > 5 ? 5 : itemsToPlacePool.size();

                for (int i = 0; i < count; i++)
                {
                    const auto& item = itemsToPlacePool[i];
                    errorMsg += "- " + item->GetName() + "\n";
                }

                if (count < itemsToPlacePool.size())
                {
                    errorMsg += "- (" + std::to_string(itemsToPlacePool.size() - count) + " more)";
                }

                throw std::runtime_error(errorMsg);
            }

            retries -= 1;
            unsuccessfulPlacement = false;

            utility::random::ShufflePool(itemsToPlacePool);
            auto itemsToPlace = itemsToPlacePool;
            location::LocationPool rollbacks = {};

            while (!itemsToPlace.empty())
            {
                // Get a random item to place
                auto itemToPlace = itemsToPlace.back();
                itemsToPlace.pop_back();

                utility::random::ShufflePool(allowedLocations);
                location::Location* spotToFill = nullptr;

                // Assume we have all the items which haven't been played yet, except the one we're about to place
                auto assumedItems = itemsNotYetPlaced;
                assumedItems.insert(assumedItems.end(), itemsToPlace.begin(), itemsToPlace.end());
                auto search = search::Search::Accessible(&worlds, assumedItems, worldToFill);
                search.SearchWorlds();
                // search.DumpWorldGraph();
                // return 1;

                // Loop through the shuffled locations until we find a valid one.
                // If a world is only checking for beatable logic, then we can ignore
                // any access checks and just choose a random location if the world is already beatable
                auto beatableOnlyLogic = itemToPlace->GetWorld()->Setting("Logic Rules") == "Beatable Only";
                auto noLogic = itemToPlace->GetWorld()->Setting("Logic Rules") == "No Logic";
                bool canChooseAnyLocation =
                    noLogic || (search._ownedItems.contains(itemToPlace->GetWorld()->GetGameWinningItem()) && beatableOnlyLogic);

                for (const auto& location : allowedLocations)
                {
                    // Get all reachable LocationAccess spots for this location
                    std::list<area::LocationAccess*> locAccList;
                    for (const auto& locAcc : location->GetAccessList())
                    {
                        if (canChooseAnyLocation || search._visitedAreas.contains(locAcc->GetArea()))
                        {
                            locAccList.push_back(locAcc);
                        }
                    }

                    // If this location is not empty, or has no potentially reachable LocationAccess spot, or is forbidden from
                    // having this item, then we can't place the item here
                    if (!location->IsEmpty() || locAccList.empty() || location->GetForbiddenItems().contains(itemToPlace))
                    {
                        continue;
                    }

                    // If any of the LocationAccess spots evaluate to complete, then we can place an item here
                    if (std::ranges::any_of(locAccList, [&](const auto& la) {
                        return canChooseAnyLocation ||
                               requirement::EvaluateLocationRequirement(&search, la) == requirement::EvalSuccess::COMPLETE;
                    }))
                    {
                        spotToFill = location;
                        break;
                    }
                }

                // If we couldn't find a spot to place this item, undo all item placements within this fill attempt and try
                // again from the top.
                if (spotToFill == nullptr)
                {
                    LOG_TO_DEBUG("No accessible locations to place " + itemToPlace->GetName() + ". Retrying " +
                                 std::to_string(retries) + " more times.");
                    for (auto& location : rollbacks)
                    {
                        itemsToPlace.push_back(location->GetCurrentItem());
                        location->RemoveCurrentItem();
                    }

                    // Also add back the randomly selected item
                    itemsToPlace.push_back(itemToPlace);
                    rollbacks.clear();
                    // Break out of the item placement loop and flag an unsuccessful placement attempt to try again
                    unsuccessfulPlacement = true;
                    break;
                }

                // Place the item at the location
                spotToFill->SetCurrentItem(itemToPlace);
                rollbacks.push_back(spotToFill);
            }
        }
    }

    void FastFill(item_pool::ItemPool& itemsToPlace, location::LocationPool allowedLocations)
    {
        auto emptyLocations =
            utility::container::FilterFromVector(allowedLocations,
                                                        [](const auto& location) { return location->IsEmpty(); });

        if (itemsToPlace.size() > emptyLocations.size())
        {
            std::cout << "WARNING: More items than locations when placing items with fast fill. Items: " << itemsToPlace.size()
                      << " Locations: " << emptyLocations.size() << std::endl;
        }

        utility::random::ShufflePool(emptyLocations);
        for (auto& location : emptyLocations)
        {
            if (itemsToPlace.empty())
            {
                break;
            }
            location->SetCurrentItem(utility::random::PopRandomElement(itemsToPlace));
        }
    }

    void PlaceRestrictedItems(std::unique_ptr<world::World>& world, world::WorldPool& worlds)
    {
        PlaceGoalLocationItems(world, worlds);
        PlaceOwnDungeonItems(world, worlds);
        PlacePrologueItems(world, worlds);
        PlaceAnywhereDungeonRewards(world, worlds);
        PlaceAnyDungeonItems(world, worlds);
        PlaceOverworldItems(world, worlds);
    }

    void PlacePrologueItems(std::unique_ptr<world::World>& world, world::WorldPool& worlds)
    {
        if (world->Setting("Skip Prologue") == "On") {
            return;
        }

        auto locations = world->GetAllLocations();
        // Filter out excluded locations
        utility::container::FilterAndEraseFromVector(locations, [](const auto& location) {
            return !location->IsProgression();
        });
        
        // Filter out the slingshot and progressive swords to place first. The slingshot and first sword have a very limited
        // pool of locations and have to be found in the intro. We also include the lantern, shadow crystal, and progressive
        // fishing rod because those items can lock prologue locations also.
        auto& itemPool = world->GetItemPool();
        auto prologueItems = utility::container::FilterAndEraseFromVector(
            itemPool,
            [](const auto& item)
            {
                return item->GetName() == "Slingshot" || item->GetName() == "Progressive Sword" ||
                       item->GetName() == "Lantern" || item->GetName() == "Progressive Fishing Rod" ||
                       item->GetName() == "North Faron Woods Gate Key" ||
                       item->IsShadowCrystal();
            });
        auto completeItemPool = item_pool::GetCompleteItemPool(worlds);
        AssumedFill(worlds, prologueItems, completeItemPool, locations);
    }

    void PlaceGoalLocationItems(std::unique_ptr<world::World>& world, world::WorldPool& worlds)
    {
        // If dungeon rewards can be anywhere, then return early and place them later
        if (world->Setting("Dungeon Rewards Can Be Anywhere") == "On")
        {
            return;
        }

        auto allLocations = world->GetAllLocations();
        location::LocationPool goalLocations = {};

        // Filter out goal locations
        goalLocations = utility::container::FilterFromVector(allLocations, [](const auto& location) {
            return location->IsGoalLocation() && location->IsEmpty() && location->IsProgression();
        });

        // Filter out goal items
        std::set<std::string> goalItemNames = {"Progressive Mirror Shard", "Progressive Fused Shadow"};

        auto goalItems = utility::container::FilterAndEraseFromVector(
            world->GetItemPool(),
            [&](const auto& item) { return goalItemNames.contains(item->GetName()); });

        // Return an error if there aren't enough goal locations
        if (goalItems.size() > goalLocations.size())
        {
            throw std::runtime_error("Not enough available locations to place dungeon rewards at the end of dungeons.");
        }

        // Place goal items at goal locations
        auto completeItemPool = item_pool::GetCompleteItemPool(worlds);
        AssumedFill(worlds, goalItems, completeItemPool, goalLocations);

        // Determine required dungeons now that we placed goal location items
        world->DetermineRequiredDungeons();
    }

    void PlaceOwnDungeonItems(std::unique_ptr<world::World>& world, world::WorldPool& worlds)
    {
        for (const auto& [dungeonName, dungeon] : world->GetDungeonTable())
        {
            // Filter hint signs out of dungeon locations
            auto dungeonLocations = dungeon->GetLocations();
            utility::container::FilterAndEraseFromVector(dungeonLocations, [](const auto& location) {
                return location->HasCategories("Non-Item Location");
            });

            // Filter out excluded locations if this dungeon is required
            if (dungeon->IsRequired()) {
                utility::container::FilterAndEraseFromVector(dungeonLocations, [](const auto& location) {
                    return !location->IsProgression();
                });
            }

            // Clang doesn't like passing structured binding variables to lambda functions via reference, so we create these
            // temporary variables to serve the purpose
            auto& dungeon_ = dungeon;
            auto& dungeonName_ = dungeonName;

            // Small Keys
            if (world->Setting("Small Keys") == "Own Dungeon")
            {
                auto smallKeys = utility::container::FilterAndEraseFromVector(
                    world->GetItemPool(),
                    [&](const auto& item)
                    {
                        return item == dungeon_->GetSmallKey() ||
                               (dungeonName_ == "Snowpeak Ruins" &&
                                (item->GetName() == "Ordon Pumpkin" || item->GetName() == "Ordon Cheese"));
                    });
                auto completeItemPool = item_pool::GetCompleteItemPool(worlds);
                AssumedFill(worlds, smallKeys, completeItemPool, dungeonLocations);
            }

            // Big Keys
            if (world->Setting("Big Keys") == "Own Dungeon")
            {
                auto bigKeys = utility::container::FilterAndEraseFromVector(world->GetItemPool(),
                                                                                   [&](const auto& item)
                                                                                   { return item == dungeon_->GetBigKey(); });
                auto completeItemPool = item_pool::GetCompleteItemPool(worlds);
                AssumedFill(worlds, bigKeys, completeItemPool, dungeonLocations);
            }

            // Place maps and compasses last with fast fill since they're junk items
            if (world->Setting("Maps and Compasses") == "Own Dungeon")
            {
                auto mapsCompasses = utility::container::FilterAndEraseFromVector(
                    world->GetItemPool(),
                    [&](const auto& item) { return item == dungeon_->GetCompass() || item == dungeon_->GetDungeonMap(); });
                auto completeItemPool = item_pool::GetCompleteItemPool(worlds);
                FastFill(mapsCompasses, dungeonLocations);
            }
        }
    }

    void PlaceAnywhereDungeonRewards(std::unique_ptr<world::World>& world, world::WorldPool& worlds)
    {
        // If dungeon rewards can't be anywhere, then return early as we placed them earlier
        if (world->Setting("Dungeon Rewards Can Be Anywhere") == "Off")
        {
            return;
        }

        auto allLocations = world->GetAllLocations();
        // Filter out any nonprogress locations
        utility::container::FilterAndEraseFromVector(allLocations, [](const auto& location) {
            return !location->IsProgression();
        });

        // Filter out goal items
        std::set<std::string> goalItemNames = {"Progressive Mirror Shard", "Progressive Fused Shadow"};

        auto goalItems = utility::container::FilterAndEraseFromVector(
            world->GetItemPool(),
            [&](const auto& item) { return goalItemNames.contains(item->GetName()); });

        // Place the items
        auto completeItemPool = item_pool::GetCompleteItemPool(worlds);
        AssumedFill(worlds, goalItems, completeItemPool, allLocations);

        // Determine required dungeons now that we placed goal location items
        world->DetermineRequiredDungeons();
    }

    void PlaceAnyDungeonItems(std::unique_ptr<world::World>& world, world::WorldPool& worlds)
    {
        item_pool::ItemPool anyDungeonItems = {};
        location::LocationPool anyDungeonLocations = {};

        // Split the placement of any dungeon items into two pools. Dungeon items from dungeons which should be barren
        // will only be distributed among barren dungeons, where as items from nonbarren dungeons will be distributed
        // among nonbarren dungeons
        std::list<dungeon::Dungeon*> nonBarrenDungeons = {};
        std::list<dungeon::Dungeon*> barrenDungeons = {};
        for (const auto& [dungeonName, dungeon] : world->GetDungeonTable())
        {
            if (dungeon->ShouldBeBarren())
            {
                barrenDungeons.push_back(dungeon.get());
            }
            else
            {
                nonBarrenDungeons.push_back(dungeon.get());
            }
        }

        // Loop through each pool separately
        for (const auto& dungeons : {nonBarrenDungeons, barrenDungeons})
        {
            anyDungeonItems.clear();
            anyDungeonLocations.clear();
            // Gather all the appropriate items and locations for the dungeon in this pool
            for (const auto& dungeon : dungeons)
            {
                // Clang doesn't like passing structured binding variables to lambda functions via reference, so we create these
                // temporary variables to serve the purpose
                auto& dungeon_ = dungeon;
                // Add small keys to the pool if small keys are any dungeon
                if (world->Setting("Small Keys") == "Any Dungeon")
                {
                    auto smallKeys = utility::container::FilterAndEraseFromVector(
                        world->GetItemPool(),
                        [&](const auto& item)
                        {
                            return item == dungeon_->GetSmallKey() ||
                                   (dungeon_->GetName() == "Snowpeak Ruins" &&
                                    (item->GetName() == "Ordon Pumpkin" || item->GetName() == "Ordon Cheese"));
                        });
                    std::ranges::copy(smallKeys, std::back_inserter(anyDungeonItems));
                }

                // Add big keys to the pool if big keys are any dungeon
                if (world->Setting("Big Keys") == "Any Dungeon")
                {
                    auto bigKeys = utility::container::FilterAndEraseFromVector(
                        world->GetItemPool(),
                        [&](const auto& item) { return item == dungeon_->GetBigKey(); });
                    std::ranges::copy(bigKeys, std::back_inserter(anyDungeonItems));
                }

                // Add maps and compasses to the pool if maps and compasses are any dungeon
                if (world->Setting("Maps and Compasses") == "Any Dungeon")
                {
                    auto mapsCompasses = utility::container::FilterAndEraseFromVector(
                        world->GetItemPool(),
                        [&](const auto& item) { return item == dungeon_->GetCompass() || item == dungeon_->GetDungeonMap(); });
                    std::ranges::copy(mapsCompasses, std::back_inserter(anyDungeonItems));
                }

                // Add this dungeon's locations to the anyDungeonLocations pool. If this is a nonbarren dungeon, only include
                // locations which are still progression. If it's a barren dungeon, include all the locations
                auto dungeonLocations = dungeon->GetLocations();
                // Filter out non-item locations (i.e. hint signs)
                utility::container::FilterAndEraseFromVector(dungeonLocations, [](const auto& location) {
                    return location->HasCategories("Non-Item Location");
                });

                std::ranges::copy_if(dungeonLocations,
                             std::back_inserter(anyDungeonLocations),
                             [&](const auto& location) { return dungeon->ShouldBeBarren() || location->IsProgression(); });
            }

            // Place the dungeon items in the appropriate dungeon locations
            auto completeItemPool = item_pool::GetCompleteItemPool(worlds);
            AssumedFill(worlds, anyDungeonItems, completeItemPool, anyDungeonLocations);
        }
    }

    void PlaceOverworldItems(std::unique_ptr<world::World>& world, world::WorldPool& worlds)
    {
        item_pool::ItemPool overworldItems = {};
        location::LocationPool overworldLocations = world->GetAllLocations();
        // Filter out any nonprogress locations
        utility::container::FilterAndEraseFromVector(overworldLocations,
                                                            [](const auto& location) { return !location->IsProgression(); });

        for (const auto& [dungeonName, dungeon] : world->GetDungeonTable())
        {
            // Clang doesn't like passing structured binding variables to lambda functions via reference, so we create these
            // temporary variables to serve the purpose
            auto& dungeon_ = dungeon;
            auto& dungeonName_ = dungeonName;

            // Add small keys to the pool if small keys are overworld
            if (world->Setting("Small Keys") == "Overworld")
            {
                auto smallKeys = utility::container::FilterAndEraseFromVector(
                    world->GetItemPool(),
                    [&](const auto& item)
                    {
                        return item == dungeon_->GetSmallKey() ||
                               (dungeonName_ == "Snowpeak Ruins" &&
                                (item->GetName() == "Ordon Pumpkin" || item->GetName() == "Ordon Cheese"));
                    });
                std::ranges::copy(smallKeys, std::back_inserter(overworldItems));
            }

            // Add big keys to the pool if big keys are overworld
            if (world->Setting("Big Keys") == "Overworld")
            {
                auto bigKeys = utility::container::FilterAndEraseFromVector(world->GetItemPool(),
                                                                                   [&](const auto& item)
                                                                                   { return item == dungeon_->GetBigKey(); });
                std::ranges::copy(bigKeys, std::back_inserter(overworldItems));
            }

            // Add maps and compasses to the pool if maps and compasses are overworld
            if (world->Setting("Maps and Compasses") == "Overworld")
            {
                auto mapsCompasses = utility::container::FilterAndEraseFromVector(
                    world->GetItemPool(),
                    [&](const auto& item) { return item == dungeon_->GetCompass() || item == dungeon_->GetDungeonMap(); });
                std::ranges::copy(mapsCompasses, std::back_inserter(overworldItems));
            }

            // Remove this dungeon's locations from the overworldLocations pool
            overworldLocations = utility::container::FilterFromVector(
                overworldLocations,
                [&](const auto& location)
                { return !utility::container::ElementInContainer(dungeon_->GetLocations(), location); });
        }

        // Place the dungeon items in the overworld locations
        auto completeItemPool = item_pool::GetCompleteItemPool(worlds);
        AssumedFill(worlds, overworldItems, completeItemPool, overworldLocations);
    }

    void CacheExitTimeForms(world::WorldPool& worlds)
    {
        auto completeItemPool = item_pool::GetCompleteItemPool(worlds);
        auto searchWithItems = search::Search::AllLocationsReachable(&worlds, completeItemPool);
        searchWithItems.SearchWorlds();

        for (auto& world : worlds)
        {
            LOG_TO_DEBUG("Caching timeforms for world " + std::to_string(world->GetID()));
            auto& exitTimeFormCache = world->GetExitTimeFormCache();
            exitTimeFormCache.clear();
            for (const auto& [areaName, area] : world->GetAreaTable())
            {
                const auto& areaFormTimes = searchWithItems._areaFormTime[area.get()];
                for (const auto& exit : area->GetExits())
                {
                    auto req = exit->GetRequirement();
                    exitTimeFormCache[exit] = requirement::FormTime::NONE;
                    for (const auto& formTime : requirement::FormTime::ALL_FORM_TIMES)
                    {
                        if (formTime & areaFormTimes &&
                            requirement::EvaluateRequirementAtFormTime(req,
                                                                                     &searchWithItems,
                                                                                     formTime,
                                                                                     world.get()))
                        {
                            exitTimeFormCache[exit] |= formTime;
                        }
                    }
                }
            }
        }
    }
} // namespace randomizer::logic::fill
