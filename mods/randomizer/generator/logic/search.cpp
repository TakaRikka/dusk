#include "search.hpp"

#include "world.hpp"
#include "../randomizer.hpp"
#include "../utility/general.hpp"
#include "../utility/platform.hpp"

#include <fstream>
#include <iostream>

namespace randomizer::logic::search
{
    Search::Search(): _searchMode(SearchMode::NO_SEARCH) {

    }

    Search::Search(const SearchMode& searchMode,
                   world::WorldPool* worlds,
                   const item_pool::ItemPool& items /* = {} */,
                   const int& worldToSearch /* = -1 */,
                   bool startingInventory /*= true */):
        _searchMode(searchMode), _worlds(worlds), _startingInventory(startingInventory)
    {
        // Set the items we should already own
        this->_ownedItems.insert(items.begin(), items.end());

        // Add starting inventory items for each world
        if (this->_startingInventory) {
            for (const auto& world : *(this->_worlds))
            {
                if (worldToSearch == -1 || world->GetID() == worldToSearch)
                {
                    const auto& startingInventory = world->GetStartingItemPool();
                    this->_ownedItems.insert(startingInventory.begin(), startingInventory.end());
                }
            }
        }

        // Set search starting properties and add each world's root exits to _exitsToTry
        for (const auto& world : *(this->_worlds))
        {
            if (worldToSearch == -1 || world->GetID() == worldToSearch)
            {
                auto root = world->GetRootArea();
                this->_visitedAreas.emplace(root);
                world->SetSearchStartingProperties(this);
                for (const auto& exit : root->GetExits())
                { // Don't add target exits if we're doing a sphere zero search
                    if (!exit->IsDisabled() && (this->_searchMode != SearchMode::SPHERE_ZERO || !exit->IsTarget()))
                    {
                        this->_exitsToTry.emplace_back(exit);
                    }
                }
            }
        }
    }

    void Search::SearchWorlds()
    {
        if (this->_searchMode == SearchMode::NO_SEARCH) {
            return;
        }
        // Get all locations which fit criteria to test on each iteration
        std::list<area::LocationAccess*> itemLocations = {};
        for (const auto& world : *(this->_worlds))
        {
            for (const auto& [areaName, area] : world->GetAreaTable())
            {
                for (const auto& locAccess : area->GetLocations())
                {
                    // Only add locations that aren't empty, unless we're searching with one of the modes below
                    if (!locAccess->GetLocation()->IsEmpty() ||
                        utility::general::IsAnyOf(this->_searchMode,
                                                         SearchMode::ACCESSIBLE_LOCATIONS,
                                                         SearchMode::ALL_LOCATIONS_REACHABLE,
                                                         SearchMode::SPHERE_ZERO,
                                                         SearchMode::TRACKER_SPHERES))
                    {
                        itemLocations.emplace_back(locAccess);
                    }
                }
            }
        }

        // Main Searching Loop
        // Keep iterating while new things are being found, but if the search is beatable and we're either generating the
        // playthrough or checking for beatability, exit early.
        this->_newThingsFound = true;
        while (
            this->_newThingsFound &&
            !(this->_isBeatable &&
              utility::general::IsAnyOf(this->_searchMode, SearchMode::GENERATE_PLAYTHROUGH, SearchMode::GAME_BEATABLE)))
        {
            // Keep track of making logical progress. We want to keep iterating as long as we're finding new things on each
            // iteration
            this->_newThingsFound = false;

            // Add an empty sphere if we're generating the playthrough or tracker spheres
            if (utility::general::IsAnyOf(this->_searchMode,
                                                 SearchMode::GENERATE_PLAYTHROUGH,
                                                 SearchMode::TRACKER_SPHERES))
            {
                this->_playthroughSpheres.push_back({});
                this->_entranceSpheres.push_back({});
            }

            // Process Events and Exits at least once. If we're calculating spheres, then keep repeating these until nothing
            // new is found.
            do
            {
                this->_newThingsFound = false;
                this->ProcessEvents();
                this->ProcessExits();
            } while (this->_newThingsFound && utility::general::IsAnyOf(this->_searchMode,
                                                                               SearchMode::GENERATE_PLAYTHROUGH,
                                                                               SearchMode::TRACKER_SPHERES));

            this->ProcessLocations(itemLocations);
            this->_sphereNum += 1;
        }
    }

    void Search::ProcessEvents()
    {
        for (const auto& event : this->_eventsToTry)
        {
            // Ignore the event if we've already found it, or we're not searching its world at the moment
            if (this->_ownedEvents.contains(event->GetEventIndex()) ||
                (this->_worldToSearch != -1 && event->GetArea()->GetWorld()->GetID() != this->_worldToSearch))
            {
                continue;
            }

            if (requirement::EvaluateEventRequirement(this, event) ==
                requirement::EvalSuccess::COMPLETE)
            {
                this->_newThingsFound = true;
                this->_ownedEvents.insert(event->GetEventIndex());
            }
        }
    }

    void Search::ProcessExits()
    {
        for (const auto& exit : this->_exitsToTry)
        {
            // Ignore the exit if we've already completed it, or we're not searching its world at the moment
            if (this->_successfulExits.contains(exit) ||
                (this->_worldToSearch != -1 && this->_worldToSearch != exit->GetWorld()->GetID()))
            {
                continue;
            }

            // If the exit is successful
            auto evalSuccess = requirement::EvaluateExitRequirement(this, exit);
            if (utility::general::IsAnyOf(evalSuccess,
                                                      requirement::EvalSuccess::COMPLETE,
                                                      requirement::EvalSuccess::PARTIAL))
            {
                this->AddExitToEntranceSpheres(exit);
                if (evalSuccess == requirement::EvalSuccess::COMPLETE)
                {
                    this->_successfulExits.insert(exit);
                }
                this->_newThingsFound = true;

                // If this exit's connected region hasn't been explored yet, then explore it
                if (!this->_visitedAreas.contains(exit->GetConnectedArea()))
                {
                    this->_visitedAreas.insert(exit->GetConnectedArea());
                    this->Explore(exit->GetConnectedArea());
                }
            }
        }
    }

    void Search::ProcessLocations(std::list<area::LocationAccess*>& itemLocations)
    {
        std::list<location::Location*> accessibleThisIteration = {};
        // Loop through all possible item locations for this search
        for (const auto& locAccess : itemLocations)
        {
            auto location = locAccess->GetLocation();
            auto world = location->GetWorld();

            // If we've already visited this location, or have *not* visited this area, or aren't searching this world,
            // then ignore the location this time
            if (this->_visitedLocations.contains(location) || !this->_visitedAreas.contains(locAccess->GetArea()) ||
                (this->_worldToSearch != -1 && world->GetID() != this->_worldToSearch))
            {
                continue;
            }

            // If the location's requirement is met
            if (requirement::EvaluateLocationRequirement(this, locAccess) ==
                requirement::EvalSuccess::COMPLETE)
            {
                this->_visitedLocations.insert(location);
                this->_newThingsFound = true;
                // If we're calculating spheres, then process this location later for accurate sphere calculation. Otherwise
                // process it now for slightly faster searching
                if (utility::general::IsAnyOf(this->_searchMode,
                                                     SearchMode::GENERATE_PLAYTHROUGH,
                                                     SearchMode::TRACKER_SPHERES))
                {
                    accessibleThisIteration.push_back(location);
                }
                else
                {
                    this->ProcessLocation(location);
                }
            }
        }

        for (const auto& location : accessibleThisIteration)
        {
            this->ProcessLocation(location);
            if (this->_isBeatable)
            {
                return;
            }
        }
    }

    void Search::ProcessLocation(location::Location* location)
    {
        // Don't return if we aren't collecting items
        if (!this->_collectItems)
        {
            return;
        }

        // Add the tracked item if we're doing tracker sphere tracking
        if (this->_searchMode == SearchMode::TRACKER_SPHERES)
        {
            this->_ownedItems.insert(location->GetTrackedItem());
        }
        // Otherwise add the current item as usual
        else
        {
            this->_ownedItems.insert(location->GetCurrentItem());
        }

        // If we just added the shadow crystal, expand timeforms for all areas we've visited so far
        if (location->GetCurrentItem()->IsShadowCrystal())
        {
            for (auto& area : this->_visitedAreas)
            {
                if (area->GetWorld()->GetID() == location->GetWorld()->GetID())
                {
                    this->ExpandFormTimes(area);
                }
            }
        }

        // If we're generating spheres and the location has a major item, add the location to the last sphere
        if (this->_searchMode == SearchMode::TRACKER_SPHERES ||
            (this->_searchMode == SearchMode::GENERATE_PLAYTHROUGH && location->GetCurrentItem()->IsMajor()))
        {
            this->_playthroughSpheres.back().push_back(location);
        }

        // If we're generating the playthrough or just checking for beatability, then we can stop searching early if we've
        // found all world's game winning items
        if (utility::general::IsAnyOf(this->_searchMode, SearchMode::GENERATE_PLAYTHROUGH, SearchMode::GAME_BEATABLE) &&
            location->GetCurrentItem()->IsGameWinningItem())
        {
            if (std::ranges::count_if(this->_ownedItems, [](const auto& item) {
                return item->IsGameWinningItem();
            }) == this->_worlds->size())
            {
                if (this->_searchMode == SearchMode::GENERATE_PLAYTHROUGH)
                {
                    auto& lastSphere = this->_playthroughSpheres.back();
                    std::erase_if(lastSphere,[](const auto& loc) { return !loc->GetCurrentItem()->IsGameWinningItem(); });
                }
                this->_isBeatable = true;
            }
        }
    }

    void Search::Explore(area::Area* area)
    {
        for (const auto& event : area->GetEvents())
        {
            this->_eventsToTry.push_back(event);
        }

        for (const auto& exit : area->GetExits())
        {
            auto evalSuccess = requirement::EvaluateExitRequirement(this, exit);
            switch (evalSuccess)
            {
                case requirement::EvalSuccess::COMPLETE:
                    this->_successfulExits.insert(exit);
                    this->AddExitToEntranceSpheres(exit);
                    if (!this->_visitedAreas.contains(exit->GetConnectedArea()))
                    {
                        this->_visitedAreas.insert(exit->GetConnectedArea());
                        this->Explore(exit->GetConnectedArea());
                    }
                case requirement::EvalSuccess::PARTIAL:
                    this->_exitsToTry.push_back(exit);
                    this->AddExitToEntranceSpheres(exit);
                    if (!this->_visitedAreas.contains(exit->GetConnectedArea()))
                    {
                        this->_visitedAreas.insert(exit->GetConnectedArea());
                        this->Explore(exit->GetConnectedArea());
                    }
                case requirement::EvalSuccess::NONE:
                    [[fallthrough]];
                case requirement::EvalSuccess::DISCONNECTED:
                    this->_exitsToTry.push_back(exit);
            }
        }
    }

    void Search::ExpandFormTimes(area::Area* area)
    {
        using namespace requirement;

        auto& areaFormTime = this->_areaFormTime[area];
        auto twilightCleared = area->TwilightCleared(this);

        auto shadowCrystal = area->GetWorld()->GetShadowCrystal();
        // Check if we can add additional form times to the area
        if (area->CanChangeTime() && area->CanTransform() && this->_ownedItems.contains(shadowCrystal) && twilightCleared)
        {
            // LOG_TO_DEBUG("Spread All to " + area->GetName());
            areaFormTime |= FormTime::ALL;
        }
        // This might look backwards at first glance, but spreading formtime by the form spreads both day and night for the form
        else if (area->CanChangeTime() && twilightCleared)
        {
            if (areaFormTime & FormTime::WOLF)
            {
                // LOG_TO_DEBUG("Spread Day/Night to " + area->GetName());
                areaFormTime |= FormTime::WOLF;
            }
            else if (areaFormTime & FormTime::HUMAN)
            {
                // LOG_TO_DEBUG("Spread Day/Night to " + area->GetName());
                areaFormTime |= FormTime::HUMAN;
            }
        }
        // Same as above except with spreading time spreads the form
        else if (area->CanTransform() && this->_ownedItems.contains(shadowCrystal) && twilightCleared)
        {
            if (areaFormTime & FormTime::NIGHT)
            {
                // LOG_TO_DEBUG("Spread Human/Wolf to " + area->GetName());
                areaFormTime |= FormTime::NIGHT;
            }

            if (areaFormTime & FormTime::DAY)
            {
                // LOG_TO_DEBUG("Spread Human/Wolf to " + area->GetName());
                areaFormTime |= FormTime::DAY;
            }
        }
    }

    void Search::AddExitToEntranceSpheres(entrance::Entrance* exit)
    {
        if (utility::general::IsAnyOf(this->_searchMode,
                                             SearchMode::GENERATE_PLAYTHROUGH,
                                             SearchMode::TRACKER_SPHERES) &&
            exit->IsShuffled())
        {
            if (!this->_playthroughEntrances.contains(exit))
            {
                this->_entranceSpheres.back().push_back(exit);
                this->_playthroughEntrances.insert(exit);
                if (!exit->IsDecoupled() && exit->GetReplaces()->GetReverse())
                {
                    this->_playthroughEntrances.insert(exit->GetReplaces()->GetReverse());
                }
            }
        }
    }

    bool Search::HasAccessibleDisconnectedExit()
    {
        for (const auto& exit : this->_exitsToTry)
        {
            if (exit->GetConnectedArea() == nullptr && 
                requirement::EvaluateDisconnectedExitRequiremrnt(this, exit) != requirement::EvalSuccess::NONE)
            {
                return true;
            }
        }
        return false;
    }

    void Search::RemoveEmptySpheres()
    {
        // Get rid of any empty spheres in both the item playthrough and entrance playthrough
        // based only on if the item playthrough has empty spheres. Both the playthroughs
        // will have the same number of spheres, so we only need to conditionally
        // check one of them.
        auto itemItr = this->_playthroughSpheres.begin();
        auto entranceItr = this->_entranceSpheres.begin();
        while (itemItr != this->_playthroughSpheres.end())
        {
            if (itemItr->empty() && entranceItr->empty())
            {
                itemItr = this->_playthroughSpheres.erase(itemItr);
                entranceItr = this->_entranceSpheres.erase(entranceItr);
            }
            else
            {
                ++itemItr; // Only incremement if we don't erase
                ++entranceItr;
            }
        }
    }

    void Search::DumpWorldGraph(const int& worldNum /* = 0 */)
    {
        auto& world = this->_worlds->at(worldNum);
        std::cout << "Now dumping search graph for world " << worldNum << std::endl;
        std::ofstream worldGraph;
        std::string filepath = "World.gv";
        worldGraph.open(filepath);
        worldGraph << "digraph {\n\tcenter=true;\n";
        for (const auto& [areaName, area] : world->GetAreaTable())
        {
            auto color = this->_visitedAreas.contains(area.get()) ? "black" : "red";
            std::string formTimeStr = ":<br/>";
            auto& areaFormTime = this->_areaFormTime[area.get()];
            if (areaFormTime & requirement::FormTime::HUMAN)
            {
                formTimeStr += " Human";
            }
            if (areaFormTime & requirement::FormTime::WOLF)
            {
                formTimeStr += " Wolf";
            }
            if (areaFormTime & requirement::FormTime::DAY)
            {
                formTimeStr += " Day";
            }
            if (areaFormTime & requirement::FormTime::NIGHT)
            {
                formTimeStr += " Night";
            }
            if (areaFormTime & requirement::FormTime::TWILIGHT)
            {
                formTimeStr += " Twilight";
            }

            worldGraph << "\t\"" << areaName << "\"[label=<" << areaName << formTimeStr << "> shape=\"plain\" fontcolor=\""
                       << color << "\"];\n";

            // Make edge connections defined by events
            for (const auto& event : area->GetEvents())
            {
                color = this->_ownedEvents.contains(event->GetEventIndex()) ? "blue" : "red";
                auto eventName = world->GetEventName(event->GetEventIndex());
                worldGraph << "\t\"" << eventName << "\"[label=<" << eventName << "> shape=\"plain\" fontcolor=\"" << color
                           << "\"];";
                worldGraph << "\t\"" << areaName << "\" -> \"" << eventName << "\"[dir=forward color=\"" << color << "\"]";
            }

            // Make edge connections defined by exits
            for (const auto& exit : area->GetExits())
            {
                if (exit->GetConnectedArea())
                {
                    color = this->_successfulExits.contains(exit) ? "black" : "red";
                    worldGraph << "\t\"" << areaName << "\" -> \"" << exit->GetConnectedArea()->GetName()
                               << "\"[dir=forward color=\"" << color << "\"]";
                }
            }

            // Make edge connections between areas and their locations
            for (const auto& locAccess : area->GetLocations())
            {
                auto location = locAccess->GetLocation();
                color = this->_visitedLocations.contains(location) ? "black" : "red";
                worldGraph << "\t\"" << location->GetName() << "\"[label=<" << location->GetName() << ":<br/>"
                           << location->GetCurrentItem()->GetName() << "> shape=\"plain\" fontcolor=\"" << color << "\"];";
                worldGraph << "\t\"" << areaName << "\" -> \"" << location->GetName() << "\"[dir=forward color=\"" << color
                           << "\"]";
            }
        }

        worldGraph << "}";
        worldGraph.close();
    }

    std::optional<std::string> VerifyLogic(world::WorldPool* worlds,
                                           const item_pool::ItemPool& items /* = {} */)
    {
        // Run an all locations reachable search
        auto search = Search::AllLocationsReachable(worlds, items);
        search.SearchWorlds();

        for (const auto& world : *worlds)
        {
            // If all locations should be reachable, make sure they're all reachable
            if (world->Setting("Logic Rules") == "All Locations Reachable")
            {
                auto numlocationsReached =
                    std::ranges::count_if(search._visitedLocations, [&](const auto& location) {
                        return location->GetWorld() == world.get();
                    });
                auto allLocations = world->GetAllLocations(/*includeNonItemLocations = */ true);

                if (numlocationsReached != allLocations.size())
                {
                    std::string errorMsg = "Not all locations reachable! Missing locations:\n";
                    // Gather all the missing locations
                    std::vector<location::Location*> unreachedLocations = {};
                    for (const auto& location : allLocations)
                    {
                        if (!search._visitedLocations.contains(location))
                        {
                            unreachedLocations.push_back(location);
                        }
                    }
                    // Only print the first 5 so we don't clog the error message
                    for (auto i = 0; i < unreachedLocations.size(); i++)
                    {
                        errorMsg += "- " + unreachedLocations[i]->GetName() + "\n";
                        if (i == 4 && i != unreachedLocations.size())
                        {
                            errorMsg += "(" + std::to_string(unreachedLocations.size() - i) + " more)";
                            break;
                        }
                    }
                    return errorMsg;
                }
            }
        }

        return std::nullopt;
    }

    void GeneratePlaythrough(Randomizer* randomizer)
    {
        auto& worlds = randomizer->GetWorlds();
        LOG_TO_DEBUG("Generating Playthrough");
        // Generate Initial Playthrough
        auto playthroughSearch = Search::Playthrough(&worlds);
        playthroughSearch.SearchWorlds();

        auto& playthroughSpheres = playthroughSearch._playthroughSpheres;

        // Keep track of all locations we temporaily take items away from so we can give them back after playthrough calculation
        std::unordered_map<location::Location*, item::Item*> tempEmptyLocations = {};
        // Keep track of all the locations that appear in the playthrough
        std::unordered_set<location::Location*> playthroughLocationsSet = {};
        for (const auto& sphere : playthroughSpheres)
        {
            for (const auto& location : sphere)
            {
                playthroughLocationsSet.insert(location);
            }
        }

        // Remove all items from locations that are not part of the playthrough set
        for (const auto& world : worlds)
        {
            for (const auto& location : world->GetAllLocations())
            {
                if (!playthroughLocationsSet.contains(location))
                {
                    tempEmptyLocations[location] = location->GetCurrentItem();
                    location->RemoveCurrentItem();
                }
            }
        }

        utility::platform::Log("Paring down playthrough");
        // Pare down the playthrough in reverse order so we're paring it down from highest to lowest sphere.
        // This way, lower sphere items will be prioritized for the playthrough
        playthroughSpheres.reverse();
        for (const auto& sphere : playthroughSpheres)
        {
            for (auto& location : sphere)
            {
                auto itemAtLocation = location->GetCurrentItem();
                location->RemoveCurrentItem();

                // If the game is beatable, temporarily take this item away and erase the location from the playthrough
                // locations
                if (GameBeatable(&worlds))
                {
                    tempEmptyLocations[location] = itemAtLocation;
                    playthroughLocationsSet.erase(location);
                }
                else
                {
                    location->SetCurrentItem(itemAtLocation);
                }
            }
        }

        // Generate a new playthrough search incase some spheres were flattened by the previous generation having access
        // to extra items
        auto newSearch = Search::Playthrough(&worlds);
        newSearch.SearchWorlds();

        // Now do the same process for entrances to pare down the entrance playthrough
        auto& entranceSpheres = newSearch._entranceSpheres;
        std::unordered_map<entrance::Entrance*, area::Area*> nonRequiredEntrances = {};

        for (auto& sphere : entranceSpheres)
        {
            // Make a copy to avoid iterator invalidation
            auto sphereCopy = sphere;
            for (const auto& entrance : sphereCopy)
            {
                auto connectedArea = entrance->Disconnect();
                if (GameBeatable(&worlds))
                {
                    // If the game is still beatable then this entrance is not required
                    sphere.remove(entrance);
                    nonRequiredEntrances[entrance] = connectedArea;
                }
                else
                {
                    // If the entrance is required, reconnect it
                    entrance->Connect(connectedArea);
                }
            }
        }

        // Reconnect all non-required entrances
        for (auto& [entrance, connectedArea] : nonRequiredEntrances)
        {
            entrance->Connect(connectedArea);
        }

        // Give items back their locations
        for (auto& [location, item] : tempEmptyLocations)
        {
            location->SetCurrentItem(item);
        }

        // Erase all locations not in the playthrough locations set
        for (auto& sphere : newSearch._playthroughSpheres)
        {
            sphere.remove_if([&](const auto& location) {
                return !playthroughLocationsSet.contains(location);
            });
        }

        // Remove any empty spheres
        newSearch.RemoveEmptySpheres();

        randomizer->GetPlaythroughSpheres() = newSearch._playthroughSpheres;
        randomizer->GetEntranceSpheres() = newSearch._entranceSpheres;
    }

    bool GameBeatable(world::WorldPool* worlds, const item_pool::ItemPool& items /* = {} */)
    {
        auto search = Search::Beatable(worlds, items);
        search.SearchWorlds();
        return search._isBeatable;
    }

} // namespace randomizer::logic::search
