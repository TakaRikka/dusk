#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>

// Foward Declarations
namespace randomizer::logic::world
{
    class World;
    using WorldPool = std::vector<std::unique_ptr<World>>;
} // namespace randomizer::logic::world

namespace randomizer::logic::item
{
    class Item;
}

namespace randomizer::logic::item_pool
{
    using ItemPool = std::vector<item::Item*>;

    /**
     *  @brief Generates and sets the item pool of randomized items for a single world.
     *
     *  @param world The world to generate the item pool for
     */
    void GenerateItemPool(world::World* world);

    /**
     *  @brief Generates and sets the starting item pool for a single world. Starting items will be
     *         subtracted from the world's regular item pool, so be sure to call GenerateItemPool first
     *
     *  @param world The world to generate the starting item pool for
     */
    void GenerateStartingItemPool(world::World* world);

    std::map<std::string, int> GetInitialJunkPool();

    ItemPool GetCompleteItemPool(const world::WorldPool& worlds);

    const std::map<std::string, int>& GetValidStartingInventoryItems();
} // namespace randomizer::logic::item_pool
