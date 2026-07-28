#pragma once

#include "location.hpp"

#include <unordered_set>

// Forward declarations
namespace randomizer::logic::item
{
    class Item;
}

namespace randomizer::logic::area
{
    class Area;
}

namespace randomizer::logic::entrance
{
    class Entrance;
}
namespace randomizer::logic::world
{
    class World;
}

namespace randomizer::logic::dungeon
{
    /**
     *  @brief Holds dungeon specific data
     */
    class Dungeon
    {
       public:
        Dungeon(const std::string& name, world::World* world);

        std::string GetName() const;
        void SetSmallKey(item::Item* item);
        item::Item* GetSmallKey() const;
        void SetBigKey(item::Item* item);
        item::Item* GetBigKey() const;
        void SetCompass(item::Item* item);
        item::Item* GetCompass() const;
        void SetDungeonMap(item::Item* item);
        item::Item* GetDungeonMap() const;
        void SetStartingArea(area::Area* startingArea);
        area::Area* GetStartingAreas();
        void AddStartingEntrance(entrance::Entrance* startingEntrance);
        std::unordered_set<entrance::Entrance*> GetStartingEntrances() const;
        void AddLocation(location::Location* location);
        location::LocationPool GetLocations();
        void SetGoalLocation(location::Location* goalLocation);
        location::Location* GetGoalLocation();
        void SetRequired(const bool& required);
        bool IsRequired() const;
        void AddOutsideDependentLocation(location::Location* location);
        std::list<location::Location*> GetOutsideDependentLocations();

        /**
         *  @brief Returns whether or not the dungeon should be barren given the current settings and placement of dungeon
         * rewards and/or plandomized items
         */
        bool ShouldBeBarren() const;

       private:
        std::string _name = "";
        world::World* _world;
        item::Item* _smallKey;
        item::Item* _bigKey;
        item::Item* _compass;
        item::Item* _dungeonMap;
        area::Area* _startingArea;
        std::unordered_set<entrance::Entrance*> _startingEntrances;
        location::Location* _goalLocation;
        location::LocationPool _locations = {};
        // Locations which depend on beating this dungeon
        std::list<location::Location*> _outsideDependentLocations = {};
        bool _required = false;
    };
} // namespace randomizer::logic::dungeon
