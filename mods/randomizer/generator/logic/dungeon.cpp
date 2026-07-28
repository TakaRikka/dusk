#include "dungeon.hpp"

#include "area.hpp"
#include "entrance.hpp"
#include "item.hpp"
#include "world.hpp"

#include "../utility/container.hpp"
#include "../utility/log.hpp"

namespace randomizer::logic::dungeon
{
    Dungeon::Dungeon(const std::string& name, world::World* world): _name(name), _world(world) {}

    std::string Dungeon::GetName() const
    {
        return this->_name;
    }

    void Dungeon::SetSmallKey(item::Item* item)
    {
        this->_smallKey = item;
        LOG_TO_DEBUG("Set \"" + item->GetName() + "\" as small key for dungeon " + this->_name);
    }

    item::Item* Dungeon::GetSmallKey() const
    {
        return this->_smallKey;
    }

    void Dungeon::SetBigKey(item::Item* item)
    {
        this->_bigKey = item;
        LOG_TO_DEBUG("Set \"" + item->GetName() + "\" as big key for dungeon " + this->_name);
    }

    item::Item* Dungeon::GetBigKey() const
    {
        return this->_bigKey;
    }

    void Dungeon::SetCompass(item::Item* item)
    {
        this->_compass = item;
        LOG_TO_DEBUG("Set \"" + item->GetName() + "\" as compass for dungeon " + this->_name);
    }

    item::Item* Dungeon::GetCompass() const
    {
        return this->_compass;
    }

    void Dungeon::SetDungeonMap(item::Item* item)
    {
        this->_dungeonMap = item;
        LOG_TO_DEBUG("Set \"" + item->GetName() + "\" as dungeon map for dungeon " + this->_name);
    }

    item::Item* Dungeon::GetDungeonMap() const
    {
        return this->_dungeonMap;
    }

    void Dungeon::SetStartingArea(area::Area* startingArea)
    {
        this->_startingArea = startingArea;
        LOG_TO_DEBUG("Set \"" + startingArea->GetName() + "\" as starting area for dungeon " + this->_name)
    }

    area::Area* Dungeon::GetStartingAreas()
    {
        return this->_startingArea;
    }

    void Dungeon::AddStartingEntrance(entrance::Entrance* startingEntrance)
    {
        this->_startingEntrances.insert(startingEntrance);
        LOG_TO_DEBUG("Added \"" + startingEntrance->GetOriginalName() + "\" as starting entrance for dungeon " + this->_name)
    }

    std::unordered_set<entrance::Entrance*> Dungeon::GetStartingEntrances() const
    {
        return this->_startingEntrances;
    };

    void Dungeon::AddLocation(location::Location* location)
    {
        if (!utility::container::ElementInContainer(this->_locations, location))
        {
            this->_locations.push_back(location);
            LOG_TO_DEBUG(location->GetName() + " has been assigned to dungeon " + this->_name);
        }
    }

    location::LocationPool Dungeon::GetLocations()
    {
        return this->_locations;
    }

    void Dungeon::SetGoalLocation(location::Location* goalLocation)
    {
        this->_goalLocation = goalLocation;
        LOG_TO_DEBUG(goalLocation->GetName() + " has been assigned as goal location to dungeon " + this->_name);
    }

    location::Location* Dungeon::GetGoalLocation()
    {
        return this->_goalLocation;
    }

    void Dungeon::SetRequired(const bool& required)
    {
        this->_required = required;
        LOG_TO_DEBUG(this->_name + " has been set as required.");
    }

    bool Dungeon::IsRequired() const
    {
        return this->_required;
    }

    void Dungeon::AddOutsideDependentLocation(location::Location* location) {
        this->_outsideDependentLocations.push_back(location);
    }

    std::list<location::Location*> Dungeon::GetOutsideDependentLocations() {
        return this->_outsideDependentLocations;
    }

    bool Dungeon::ShouldBeBarren() const
    {
        return !this->_required &&
                this->_world->Setting("Unrequired Dungeons Are Barren") == "On" &&
                this->_name != "Hyrule Castle";
    }

} // namespace randomizer::logic::dungeon
