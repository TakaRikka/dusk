#pragma once

#include "entrance.hpp"
#include "requirement.hpp"

#include <memory>
#include <set>
#include <list>

// Forward Declarations
namespace randomizer::logic::location
{
    class Location;
}

namespace randomizer::logic::search
{
    class Search;
}

namespace randomizer::logic::world
{
    class World;
}

namespace randomizer::logic::area
{
    class Area;
    class LocationAccess
    {
       public:
        LocationAccess(location::Location* loc, const requirement::Requirement& req, Area* area);

        location::Location* GetLocation() const;
        const requirement::Requirement& GetRequirement();
        Area* GetArea() const;
        int GetID() const;

       private:
        int _id = -1;
        location::Location* _loc = nullptr;
        requirement::Requirement _req;
        Area* _area = nullptr;
    };

    class EventAccess
    {
       public:
        EventAccess(const requirement::Requirement& req, Area* area, const int& eventIndex);

        const requirement::Requirement& GetRequirement();
        Area* GetArea() const;
        int GetEventIndex() const;
        std::string GetName() const;

       private:
        requirement::Requirement _req;
        Area* _area = nullptr;
        int _eventIndex = -1;
    };

    class Area
    {
       public:
        Area(const std::string& name, world::World* world);

        std::string GetName() const;
        void SetHardAssignedRegion(const std::string& _hardAssignedRegion);
        std::string GetHardAssignRegion() const;
        void SetEvents(std::list<std::unique_ptr<EventAccess>>& events);
        std::list<EventAccess*> GetEvents() const;
        void SetLocations(std::list<std::unique_ptr<LocationAccess>>& locations);
        std::list<LocationAccess*> GetLocations() const;
        void SetExits(std::list<std::unique_ptr<entrance::Entrance>>& exits);
        std::list<entrance::Entrance*> GetExits() const;
        void AddExit(std::unique_ptr<entrance::Entrance>& exit);
        void RemoveExit(entrance::Entrance* exit);
        void AddEntrance(entrance::Entrance* entrance);
        void RemoveEntrance(entrance::Entrance* entrance);
        std::list<entrance::Entrance*> GetEntrances() const;
        world::World* GetWorld() const;
        void SetCanChangeTime(const bool& canChangeTime);
        bool CanChangeTime() const;
        void SetCanTransform(const bool& canTransform);
        bool CanTransform() const;
        void AddHintRegion(const std::string& region);
        std::set<std::string> GetHintRegions();
        void SetTwilightCompletedMacroIndex(const int& macroIndex);
        int GetTwilightCompletedMacroIndex() const;
        bool TwilightCleared(search::Search* search) const;

        /**
         *  @brief Assigns this area's hint regions(s) as well as assigns any locations within the area to a dungeon if the
         * area's hint region is a dungeon
         */
        void AssignHintRegionsAndDungeonLocations();

       private:
        int _id = -1;
        std::string _name = "";
        std::string _hardAssignedRegion = "";
        std::set<std::string> _hintRegions = {};
        std::list<std::unique_ptr<EventAccess>> _events = {};
        std::list<std::unique_ptr<LocationAccess>> _locations = {};
        std::list<std::unique_ptr<entrance::Entrance>> _exits = {};
        std::list<entrance::Entrance*> _entrances = {};
        world::World* _world;
        bool _canChangeTime = false;
        bool _canTransform = false;
        int _twilightCompletedMacroIndex = -1;
    };

} // namespace randomizer::logic::area
