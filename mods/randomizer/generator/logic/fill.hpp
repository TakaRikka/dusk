#pragma once

#include "item.hpp"
#include "world.hpp"

namespace randomizer::logic::fill
{

    void FillWorlds(world::WorldPool& worlds);

    /**
     *  @brief Assumed fill is an algorithm which statistically places items more
     *   evenly across the world compared to forward fill. The idea is that
     *   we first start with all the items, take an item out, search for
     *   available locations (picking up any placed items along the way),
     *   and choose a random location of the available ones to place the item.
     *   Repeat for all items in the itemsToPlacePool.
     *
     *  @param worlds The worlds to fill with items
     *  @param itemsToPlacePool The pool of items which we want to place
     *  @param itemsNotYetPlaced The pool of items which aren't placed yet, but will be later.
     *  This is important for the assumed fill algorithm since we need to assume we have these items.
     *  @param allowedLocations Locations where items in itemsToPlacePool are allowed to be filled.
     *  @param worldToFill A specific world to fill. If -1 (default), then all worlds are considered
     */
    void AssumedFill(world::WorldPool& worlds,
                     item_pool::ItemPool& itemsToPlacePool,
                     const item_pool::ItemPool& itemsNotYetPlaced,
                     location::LocationPool allowedLocations,
                     const int& worldToFill = -1);

    /**
     *  @brief Places items in locations completely randomly without any logic checks.
     *
     *  @param itemsToPlace The pool of items to place
     *  @param allowedLocations The locations where the items can be placed
     */
    void FastFill(item_pool::ItemPool& itemsToPlace, location::LocationPool allowedLocations);

    void PlaceRestrictedItems(std::unique_ptr<world::World>& world, world::WorldPool& worlds);

    /**
     *  @brief If the prologue is not being skipped, place the sword and slingshot early on to prevent possible placement
     *  failures later.
     *
     *  @param world The world to place the prologue items in
     *  @param worlds All the worlds being generated
     */
    void PlacePrologueItems(std::unique_ptr<world::World>& world, world::WorldPool& worlds);

    void PlaceGoalLocationItems(std::unique_ptr<world::World>& world, world::WorldPool& worlds);

    void PlaceOwnDungeonItems(std::unique_ptr<world::World>& world, world::WorldPool& worlds);

    void PlaceAnywhereDungeonRewards(std::unique_ptr<world::World>& world,
                                     world::WorldPool& worlds);

    void PlaceAnyDungeonItems(std::unique_ptr<world::World>& world, world::WorldPool& worlds);

    void PlaceOverworldItems(std::unique_ptr<world::World>& world, world::WorldPool& worlds);

    /**
     *  @brief Cache all the possible timeforms for each exit. This way, the search algorithm doesn't end up testing for
     * timeforms that we know ahead of time wouldn't be possible anyway
     *  @param worlds The worlds to calculate and cache the possible timeforms for
     */
    void CacheExitTimeForms(world::WorldPool& worlds);
} // namespace randomizer::logic::fill
