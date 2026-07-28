#pragma once

#include "item_pool.hpp"
#include "../utility/log.hpp"

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <list>
#include <memory>
#include <optional>

// Forward Declarations (we have a lot here)
namespace randomizer
{
    class Randomizer;
}

namespace randomizer::logic::world
{
    class World;
    using WorldPool = std::vector<std::unique_ptr<World>>;
} // namespace randomizer::logic::world

namespace randomizer::logic::item
{
    class Item;
}

namespace randomizer::logic::location
{
    class Location;
}

namespace randomizer::logic::area
{
    class EventAccess;
    class LocationAccess;
    class Area;
} // namespace randomizer::logic::area

namespace randomizer::logic::entrance
{
    class Entrance;
}

namespace randomizer::logic::search
{
    enum class SearchMode
    {
        NO_SEARCH,
        ACCESSIBLE_LOCATIONS,
        GAME_BEATABLE,
        ALL_LOCATIONS_REACHABLE,
        GENERATE_PLAYTHROUGH,
        SPHERE_ZERO,
        TRACKER_SPHERES
    };

    class Search
    {
       public:
        Search();
        Search(const SearchMode& searchMode,
               world::WorldPool* worlds,
               const item_pool::ItemPool& items = {},
               const int& worldToSearch = -1,
               bool startingInventory = true);

        static auto Accessible(world::WorldPool* worlds,
                               const item_pool::ItemPool& items = {},
                               const int& worldToSearch = -1)
        {
            return Search(SearchMode::ACCESSIBLE_LOCATIONS, worlds, items, worldToSearch);
        }

        static auto AccessibleNoStartingInventory(world::WorldPool* worlds,
                       const item_pool::ItemPool& items = {},
                       const int& worldToSearch = -1)
        {
            return Search(SearchMode::ACCESSIBLE_LOCATIONS, worlds, items, worldToSearch, false);
        }

        static auto AllLocationsReachable(world::WorldPool* worlds,
                                          const item_pool::ItemPool& items = {},
                                          const int& worldToSearch = -1)
        {
            return Search(SearchMode::ALL_LOCATIONS_REACHABLE, worlds, items, worldToSearch);
        }

        static auto Playthrough(world::WorldPool* worlds,
                                const item_pool::ItemPool& items = {},
                                const int& worldToSearch = -1)
        {
            return Search(SearchMode::GENERATE_PLAYTHROUGH, worlds, items, worldToSearch);
        }

        static auto Beatable(world::WorldPool* worlds,
                             const item_pool::ItemPool& items = {},
                             const int& worldToSearch = -1)
        {
            return Search(SearchMode::GAME_BEATABLE, worlds, items, worldToSearch);
        }

        static auto SphereZero(world::WorldPool* worlds,
                             const item_pool::ItemPool& items = {},
                             const int& worldToSearch = -1)
        {
            return Search(SearchMode::SPHERE_ZERO, worlds, items, worldToSearch);
        }

        void SearchWorlds();

        /**
         *  @brief Loop through and see if there are any events that are now accessible. Add them to the ownedEvents list if
         * they are.
         *
         */
        void ProcessEvents();
        void ProcessExits();
        void ProcessLocations(std::list<area::LocationAccess*>& itemLocations);
        void ProcessLocation(location::Location* location);
        void Explore(area::Area* area);
        void ExpandFormTimes(area::Area* area);

        void AddExitToEntranceSpheres(entrance::Entrance*);
        bool HasAccessibleDisconnectedExit();
        void RemoveEmptySpheres();

        /**
         *  @brief Will dump a file which can be turned into a visual graph using graphviz
         *  https://graphviz.org/download/
         *  Use this command to generate the graph: "dot -Tsvg <filename> -o world.svg"
         *  Then, open world.svg in a browser and CTRL + F to find the area of interest
         */
        void DumpWorldGraph(const int& world = 0);

        SearchMode _searchMode;
        world::WorldPool* _worlds;
        int _worldToSearch = -1;

        // Search variables
        int _sphereNum = 0;
        bool _newThingsFound = true;
        bool _isBeatable = false;
        bool _collectItems = true;
        bool _startingInventory = true;
        std::unordered_set<int> _ownedEvents;
        std::unordered_multiset<item::Item*> _ownedItems;

        std::list<area::EventAccess*> _eventsToTry;
        std::list<entrance::Entrance*> _exitsToTry;
        std::unordered_set<location::Location*> _visitedLocations;
        std::unordered_set<area::Area*> _visitedAreas;
        std::unordered_set<entrance::Entrance*> _successfulExits;
        std::unordered_set<entrance::Entrance*> _playthroughEntrances;

        std::list<std::list<location::Location*>> _playthroughSpheres;
        std::list<std::list<entrance::Entrance*>> _entranceSpheres;

        std::unordered_map<area::Area*, int> _areaFormTime;
    };

    /**
     * @brief Verifies that necessary logic for all worlds is satisfied.
     * 
     * @param worlds The worlds to verify logic for
     * @param items The pool of items that haven't been placed yet
     * 
     * @return An optional value that holds a string explaining why the logic was not satisfied if validation failed
     */
    std::optional<std::string> VerifyLogic(world::WorldPool* worlds,
                                           const item_pool::ItemPool& items = {});
    void GeneratePlaythrough(randomizer::Randomizer* randomizer);
    bool GameBeatable(world::WorldPool* worlds, const item_pool::ItemPool& items = {});
} // namespace randomizer::logic::search
