#include "area.hpp"

#include "search.hpp"
#include "world.hpp"
#include "../randomizer.hpp"

#include <algorithm>
#include <unordered_set>

namespace randomizer::logic::area
{

    LocationAccess::LocationAccess(location::Location* loc,
                                   const requirement::Requirement& req,
                                   Area* area):
        _loc(loc), _req(std::move(req)), _area(area)
    {
        this->_id = area->GetWorld()->GetRandomizer()->GetNewLocAccID();
    }

    location::Location* LocationAccess::GetLocation() const
    {
        return this->_loc;
    }
    const requirement::Requirement& LocationAccess::GetRequirement()
    {
        return this->_req;
    }
    Area* LocationAccess::GetArea() const
    {
        return this->_area;
    }
    int LocationAccess::GetID() const
    {
        return this->_id;
    }

    EventAccess::EventAccess(const requirement::Requirement& req, Area* area, const int& eventIndex):
        _req(std::move(req)), _area(area), _eventIndex(eventIndex)
    {
    }

    const requirement::Requirement& EventAccess::GetRequirement()
    {
        return this->_req;
    }
    Area* EventAccess::GetArea() const
    {
        return this->_area;
    }
    int EventAccess::GetEventIndex() const
    {
        return this->_eventIndex;
    }

    std::string EventAccess::GetName() const
    {
        return this->_area->GetWorld()->GetEventName(this->_eventIndex);
    }

    Area::Area(const std::string& name, world::World* world): _name(name), _world(world)
    {
        this->_id = world->GetRandomizer()->GetNewAreaID();
    }

    std::string Area::GetName() const
    {
        return this->_name;
    }
    void Area::SetHardAssignedRegion(const std::string& _hardAssignedRegion)
    {
        this->_hardAssignedRegion = _hardAssignedRegion;
    }
    std::string Area::GetHardAssignRegion() const
    {
        return this->_hardAssignedRegion;
    }
    void Area::SetEvents(std::list<std::unique_ptr<EventAccess>>& events)
    {
        this->_events = std::move(events);
    }

    std::list<EventAccess*> Area::GetEvents() const
    {
        std::list<EventAccess*> events;
        for (const auto& event : this->_events)
        {
            events.emplace_back(event.get());
        }
        return events;
    }

    void Area::SetLocations(std::list<std::unique_ptr<LocationAccess>>& locations)
    {
        this->_locations = std::move(locations);
    }

    std::list<LocationAccess*> Area::GetLocations() const
    {
        std::list<LocationAccess*> locations;
        for (const auto& loc : this->_locations)
        {
            locations.emplace_back(loc.get());
        }
        return locations;
    }

    void Area::SetExits(std::list<std::unique_ptr<entrance::Entrance>>& exits)
    {
        this->_exits = std::move(exits);
    }

    std::list<entrance::Entrance*> Area::GetExits() const
    {
        std::list<entrance::Entrance*> exits;
        for (const auto& exit : this->_exits)
        {
            exits.emplace_back(exit.get());
        }
        return exits;
    }

    void Area::AddExit(std::unique_ptr<entrance::Entrance>& exit)
    {
        this->_exits.push_back(std::move(exit));
    }

    void Area::RemoveExit(entrance::Entrance* exit)
    {
        std::erase_if(this->_exits, [&](const auto& e) { return e.get() == exit; });
    }

    void Area::AddEntrance(entrance::Entrance* entrance)
    {
        this->_entrances.emplace_back(entrance);
    }

    void Area::RemoveEntrance(entrance::Entrance* entrance)
    {
        std::erase(this->_entrances, entrance);
    }

    std::list<entrance::Entrance*> Area::GetEntrances() const
    {
        return this->_entrances;
    }
    world::World* Area::GetWorld() const
    {
        return this->_world;
    }
    void Area::SetCanChangeTime(const bool& canChangeTime)
    {
        this->_canChangeTime = canChangeTime;
    }
    bool Area::CanChangeTime() const
    {
        return this->_canChangeTime;
    }
    void Area::SetCanTransform(const bool& canTransform)
    {
        this->_canTransform = canTransform;
    }
    bool Area::CanTransform() const
    {
        return this->_canTransform;
    }
    void Area::AddHintRegion(const std::string& region)
    {
        this->_hintRegions.emplace(region);
    }
    std::set<std::string> Area::GetHintRegions()
    {
        return this->_hintRegions;
    }
    void Area::SetTwilightCompletedMacroIndex(const int& macroIndex)
    {
        this->_twilightCompletedMacroIndex = macroIndex;
    }
    int Area::GetTwilightCompletedMacroIndex() const
    {
        return this->_twilightCompletedMacroIndex;
    }

    bool Area::TwilightCleared(search::Search* search) const
    {
        return this->_twilightCompletedMacroIndex == -1 || requirement::EvaluateRequirementAtFormTime(
                                                               this->GetWorld()->GetMacro(this->_twilightCompletedMacroIndex),
                                                               search,
                                                               requirement::FormTime::ALL,
                                                               this->GetWorld());
    }

    void Area::AssignHintRegionsAndDungeonLocations()
    {
        std::set<std::string> hintRegions = {};
        std::unordered_set<Area*> alreadyChecked = {};
        std::list<Area*> areaQueue = {this};

        while (!areaQueue.empty())
        {
            auto area = areaQueue.back();
            areaQueue.pop_back();
            alreadyChecked.insert(area);

            // If this area has a hard assigned region, then we won't assign it any other regions
            auto hardAssignedRegion = area->GetHardAssignRegion();
            if (hardAssignedRegion != "")
            {
                // If the region is None, then don't assign it. None is meant to be a blocker that prevents other regions
                // from assigning themselves through this area
                if (hardAssignedRegion != "None")
                {
                    hintRegions.insert(hardAssignedRegion);
                }
                continue;
            }

            // If this area isn't assigned any hint regions, add its entrancs' parent areas to the queue as long as they
            // haven't been checked yet
            for (const auto& entrance : area->GetEntrances())
            {
                if (!alreadyChecked.contains(entrance->GetParentArea()))
                {
                    areaQueue.push_back(entrance->GetParentArea());
                }
            }
        }

        // When determining which regions to assign the area to, overworld regions will take complete priority over dungeon
        // regions. Dungeon regions should only be assigned if dungeons are the only regions listed. So if we have any overworld
        // hint regions, filter out the dungeon ones.
        const auto& dungeons = this->GetWorld()->GetDungeonTable();
        std::set<std::string> dungeonRegions = {};
        std::ranges::copy_if(hintRegions,
                     std::inserter(dungeonRegions, dungeonRegions.begin()),
                     [&](const auto& hintRegion) { return dungeons.contains(hintRegion); });
        // If we have less dungeons than total hint regions, we have at least one overworld hint region
        // So erase all the dungeons in that case.
        if (dungeonRegions.size() < hintRegions.size())
        {
            for (const auto& dungeon : dungeonRegions)
            {
                hintRegions.erase(dungeon);
            }
        }

        // Assign the found hint regions to the area
        for (const auto& region : hintRegions)
        {
            this->AddHintRegion(region);
            LOG_TO_DEBUG("Assigned \"" + region + "\" as hint region to \"" + this->GetName() + "\"");

            // Also assign any locations in this area to the dungeon if there are any dungeon regions
            if (dungeons.contains(region))
            {
                auto locAccs = this->GetLocations();
                auto dungeon = this->GetWorld()->GetDungeon(region);
                for (const auto& locAcc : locAccs)
                {
                    auto location = locAcc->GetLocation();
                    dungeon->AddLocation(location);
                }
            }
        }
    }

} // namespace randomizer::logic::area
