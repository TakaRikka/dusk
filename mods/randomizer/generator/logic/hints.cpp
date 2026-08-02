#include "hints.hpp"

#include "search.hpp"
#include "world.hpp"
#include "../utility/platform.hpp"
#include "../utility/string.hpp"
#include "../utility/text.hpp"
#include "../utility/random.hpp"
#include "../randomizer.hpp"

#include <ranges>

namespace randomizer::logic::hints {

    static const std::list<std::pair<std::string, std::string>> dungeonColors = {
        {"Forest Temple", "<green>"},
        {"Goron Mines", "<red>"},
        {"Lakebed Temple", "<blue>"},
        {"Arbiters Grounds", "<orange>"},
        {"Snowpeak Ruins", "<light blue>"},
        {"Temple of Time", "<dark green>"},
        {"City in the Sky", "<yellow>"},
        {"Palace of Twilight", "<purple>"},
        {"Hyrule Castle", "<silver>"}
    };

    static const std::map<std::string, std::string> bossColors = {
        {"Diababa", "<green>"},
        {"Fyrus", "<red>"},
        {"Morpheel", "<blue>"},
        {"Stallord", "<orange>"},
        {"Blizzeta", "<light blue>"},
        {"Armogohma", "<dark green>"},
        {"Argorok", "<yellow>"},
        {"Zant", "<purple>"},
        {"Ganondorf", "<silver>"}
    };

    static location::Location* GetHintableLocation(const location::LocationPool& locations) {
        for (const auto location : locations) {
            if (!location->IsHinted()) {
                return location;
            }
        }
        return nullptr;
    }

    static void CalculatePossiblePathLocations(world::WorldPool& worlds) {
        LOG_TO_DEBUG("Calculating Possible Path Locations");
        // First generate the goal location keys and also remove items from non-
        // progress locations since they shouldn't be considered when determining
        // path locations
        std::unordered_map<location::Location*, item::Item*> nonRequiredLocations{};
        for (const auto& world : worlds) {
            // Defeat Ganondorf is always a goal location
            world->AddGoalLocation(world->GetLocation("Defeat Ganondorf"));
            // Required dungeons also have goal locations
            for (const auto& dungeon : world->GetDungeonTable() | std::views::values) {
                if (dungeon->IsRequired()) {
                    world->AddGoalLocation(dungeon->GetGoalLocation());
                }
            }

            // Collect items at non-progress locations to give back later
            for (const auto& location: world->GetAllLocations()) {
                if (!location->IsProgression()) {
                    nonRequiredLocations[location] = location->GetCurrentItem();
                    location->RemoveCurrentItem();
                }
            }
        }

        // Determine path locations for every progression location with a major item by going through
        // and seeing if taking away the item at each location can still access the other locations
        for (const auto& world : worlds) {
            for (const auto& potentialPathLocation : world->GetAllLocations()) {
                auto itemAtLocation = potentialPathLocation->GetCurrentItem();

                // Skip over locations with removed items
                if (itemAtLocation == nullptr) {
                    continue;
                }

                // If this is not a progression location with a major item, skip over it
                if (!potentialPathLocation->IsProgression() || !itemAtLocation->IsMajor()) {
                    continue;
                }

                // Take the item away from the location
                potentialPathLocation->RemoveCurrentItem();

                // Run a search without the item
                auto search = search::Search::Accessible(&worlds);
                search.SearchWorlds();

                // Check to see if we can reach each progression location. For each progression location that we can't reach,
                // add the potential path location as a path location
                for (auto& location : world->GetAllLocations()) {
                    if (!search._visitedLocations.contains(location)) {
                        location->AddPathLocation(potentialPathLocation);
                    }
                }

                // Then give back the location's item
                potentialPathLocation->SetCurrentItem(itemAtLocation);
            }
        }

        // Give back non-progress items
        for (auto& [location, item] : nonRequiredLocations) {
            location->SetCurrentItem(item);
        }
    }

    // Calculates the passed in location's hint importance, as well as recursively calculates the importance of any
    // locations that this one may unlock, but haven't had their own importance calculated yet.
    // By the time we call this function, we're only trying to determine if a location is
    // possibly required or not required.
    //
    // This function returns any location that blocks the passed in location from being not required
    static location::Location* CalculateLocationHintImportance(
        location::Location* location,
        std::unordered_set<location::Location*>& currentlyChecking)
    {
        // If we already know this location's importance, we don't need to calculate it
        if (location->GetImportance() != location::Importance::UNKNOWN) {
            return nullptr;
        }

        // Get all the progressive chain locations for this location's item.
        // Filter out locations that aren't progression, are already in this
        // location's path locations, have an item that we can trivially know
        // is barren, or have already been deemed as not required.
        std::unordered_set<location::Location*> chainLocations{};
        std::unordered_set pathLocations(
            location->GetPathLocations().begin(),
            location->GetPathLocations().end()
        );
        for (const auto& loc : location->GetCurrentItem()->GetChainLocations()) {
            if (loc->IsProgression() &&
                loc->GetCurrentItem()->IsMajor() &&
               !pathLocations.contains(loc) &&
               !loc->GetCurrentItem()->CanBeInBarrenRegion() &&
                loc->GetImportance() != location::Importance::NOT_REQUIRED)
            {
                chainLocations.insert(loc);
            }
        }

        // Remove the current location, as the item at this location obviously can't lead to itself
        chainLocations.erase(location);

        // If there are no chain locations left, then this item is not required
        if (chainLocations.empty()) {
            location->SetImportance(location::Importance::NOT_REQUIRED);
            return nullptr;
        }

        // Get all items from this location's logically required path locations
        item_pool::ItemPool logicallyRequiredItems{};
        for (auto pathLocation : pathLocations) {
            logicallyRequiredItems.push_back(pathLocation->GetCurrentItem());
        }

        // Perform an importance search to see which locations are reachable without using this item in any way
        auto worlds = &location->GetWorld()->GetRandomizer()->GetWorlds();
        auto importanceSearch = search::Search::LocationImportance(worlds, location, logicallyRequiredItems);
        importanceSearch.SearchWorlds();

        // Any locations that we found can be subtracted out of our chain locations because the item at this location
        // does not help reach them in any way.
        std::erase_if(chainLocations, [&](location::Location* loc) {
            return importanceSearch._visitedLocations.contains(loc);
        });

        // If any remaining chain locations are also currently being checked for their hint importance,
        // then we subtract them out. This will allow us to only check locations which aren't potentially
        // dependent on each other's items. If we go through all the other locations first and don't
        // find anything that makes this location possibly required, we'd only be left with locations
        // currently being checked that are dependent on one another. If these location's items are only
        // potentially dependent on each other, then they're both not required.
        std::erase_if(chainLocations, [&](location::Location* loc) {
            return currentlyChecking.contains(loc);
        });

        // Check to see if any remaining chain locations are at least possible required
        for (auto chainLocation : chainLocations) {
            // If any remaining chain location hasn't had its hint importance calculated, then do so now
            if (chainLocation->GetImportance() == location::Importance::UNKNOWN) {
                currentlyChecking.insert(chainLocation);
                CalculateLocationHintImportance(chainLocation, currentlyChecking);
                currentlyChecking.erase(chainLocation);
            }
            // If any remaining chain location is at least possibly required, then this location is also possibly required
            if (chainLocation->GetImportance() > location::Importance::NOT_REQUIRED) {
                location->SetImportance(location::Importance::POSSIBLY_REQUIRED);
                return chainLocation;
            }
        }

        // If all remaining chain locations were not required, then this location is also not required
        location->SetImportance(location::Importance::NOT_REQUIRED);
        return nullptr;
    }

    static void CalculatePossibleBarrenRegions(const world::WorldPool& worlds) {
        LOG_TO_DEBUG("Calculating Barren Regions");
        for (const auto& world : worlds) {
            auto defeatGanondorf = world->GetLocation("Defeat Ganondorf");
            std::unordered_set ganondorfPathLocations(
                defeatGanondorf->GetPathLocations().begin(),
                defeatGanondorf->GetPathLocations().end()
            );

            for (auto& location: world->GetAllLocations()) {
                // If this location is progression, then add its hint regions to
                // the set of potentially barren regions
                if (location->IsProgression()) {
                    for (const auto& locAccess : location->GetAccessList()) {
                        for (const auto& hintRegion : locAccess->GetArea()->GetHintRegions()) {
                            world->GetBarrenRegions()[hintRegion] = {};
                        }
                    }
                }

                // Set importance for locations that we can easily calculate as required or not
                // required right now.

                // Locations which contain barren items, or whose chain locations
                // all contain barren items are not required
                if (!location->GetCurrentItem()->IsMajor() || std::ranges::none_of(
                   location->GetCurrentItem()->GetChainLocations(), [](location::Location* loc) {
                       return loc->GetCurrentItem()->IsMajor();
                   }
                )) {
                    location->SetImportance(location::Importance::NOT_REQUIRED);
                // Locations which are on the path to Ganondorf are always required
                } else if (ganondorfPathLocations.contains(location) || location == defeatGanondorf) {
                    location->SetImportance(location::Importance::REQUIRED);
                }
            }

            // Any location which appears in the playthrough, but doesn't have an importance set
            // yet, is going to be possibly required
            const auto& playthrough = world->GetRandomizer()->GetPlaythroughSpheres();
            for (const auto& sphere : playthrough) {
                for (auto loc : sphere) {
                    if (loc->GetImportance() == location::Importance::UNKNOWN) {
                        loc->SetImportance(location::Importance::POSSIBLY_REQUIRED);
                    }
                }
            }

            // Any remaining locations which don't have a hint importance set yet are either possibly required or not required.
            // These locations' hint importance take much longer to calculate if we have complex hint importance on
            for (const auto& location : world->GetAllLocations()) {
                if (location->GetImportance() == location::Importance::UNKNOWN) {
                    std::unordered_set<location::Location*> currentlyChecking{};
                    auto blockLocation = CalculateLocationHintImportance(location, currentlyChecking);
                    if (blockLocation) {
                        LOG_TO_DEBUG(location->GetName() + ": " + location->GetCurrentItem()->GetName() +
                            " possibly required due to " + blockLocation->GetName() + ": " + blockLocation->GetCurrentItem()->GetName());
                    } else {
                        LOG_TO_DEBUG(location->GetName() + ": " + location->GetCurrentItem()->GetName() + " not required.");
                    }
                }
            }

            // Now loop through all the progression locations again and remove
            // any regions from the barren regions which have any possibly required or
            // required items (unless they can be in a barren region). Otherwise, add
            // the location to the list of locations in the barren region
            for (auto location : world->GetAllLocations()) {
                if (!location->IsProgression()) {
                    continue;
                }

                for (auto& locAccess : location->GetAccessList()) {
                    for (const auto& hintRegion : locAccess->GetArea()->GetHintRegions()) {
                        if (world->GetBarrenRegions().contains(hintRegion)) {
                            if (location->GetImportance() == location::Importance::NOT_REQUIRED ||
                                location->GetCurrentItem()->CanBeInBarrenRegion())
                            {
                                world->GetBarrenRegions()[hintRegion].push_back(location);
                            } else {
                                world->GetBarrenRegions().erase(hintRegion);
                            }
                        }
                    }
                }
            }

            LOG_TO_DEBUG("Barren regions for world " + std::to_string(world->GetID()));
            for (const auto& region : world->GetBarrenRegions() | std::views::keys) {
                LOG_TO_DEBUG("- " + region)
            }
        }
    }

    // Chooses the appropriate plurality for the given text
    static std::string ProcessHintPlurality(const std::string& text, bool plural) {
        auto pluralStartIndex = text.find('|');
        auto pluralEndIndex = text.find('|', pluralStartIndex + 1);

        auto pluralSubstr = text.substr(pluralStartIndex + 1, pluralEndIndex - pluralStartIndex - 1);
        auto pluralSubstrParts = utility::str::Split(pluralSubstr, '/');
        std::string& chosenPluralityStr = plural ? pluralSubstrParts[1] : pluralSubstrParts[0];

        return text.substr(0, pluralStartIndex) + chosenPluralityStr + text.substr(pluralEndIndex + 1);
    }

    static Text GeneratePathHintText(location::Location* location, location::Location* goalLocation) {
        // Collect all the hint regions this location is in
        std::set<std::string> hintRegionNames{};
        for (const auto& locAcc : location->GetAccessList()) {
            for (const auto& region : locAcc->GetArea()->GetHintRegions()) {
                hintRegionNames.insert(region);
            }
        }

        // Get the text object for each region and surround it with the color red
        std::vector<Text> hintRegionText{};
        std::ranges::transform(hintRegionNames, std::back_inserter(hintRegionText), [&](const std::string& region) {
           return addColor(getTextObject(region), Text::RED);
        });

        // Make the regions into a listing
        Text hintRegionListing = makeTextListing(hintRegionText);

        // Get the goal name and apply its color
        const auto& goalName = goalLocation->GetGoalName();
        const auto& goalColor = bossColors.at(goalName);
        Text goalNameText = goalColor + getTextObject(goalName) + "<white>";

        // Construct the full text
        Text fullText = getTextObject("Path Hint");
        fullText.Replace("<regions>", hintRegionListing);
        fullText.Replace("<goal name>", goalNameText);

        // Handle plurality if necessary
        for (auto& langText : fullText.mText) {
            if (!langText.empty() && utility::str::Contains(langText, '|')) {
                langText = ProcessHintPlurality(langText, hintRegionNames.size() > 1);
            }
        }

        return fullText;
    }

    static Text GenerateBarrenHintText(const std::string& region) {
        auto fullText = getTextObject("Barren Hint");
        auto regionText = addColor(getTextObject(region), Text::PURPLE);
        fullText.Replace("<region>", regionText);
        return fullText;
    }

    static Text GenerateItemHintText(location::Location* location) {
        // Collect all the hint regions this location is in
        std::set<std::string> hintRegionNames{};
        for (const auto& locAcc : location->GetAccessList()) {
            for (const auto& region : locAcc->GetArea()->GetHintRegions()) {
                hintRegionNames.insert(region);
            }
        }

        // Get the text object for each region and surround it with the color red
        std::vector<Text> hintRegionText{};
        std::ranges::transform(hintRegionNames, std::back_inserter(hintRegionText), [&](const std::string& region) {
           return addColor(getTextObject(region, Text::PRETTY), Text::RED);
        });

        // Make the regions into a listing
        Text hintRegionListing = makeTextListing(hintRegionText);
        // Get item text with color added
        auto textType = Text::PRETTY;
        Text itemText = addColor(getTextObject(location->GetCurrentItem()->GetName(), textType), Text::GREEN);

        // TODO: Cryptic Text

        Text fullText = getTextObject("Item Hint");
        fullText.Replace("<regions>", hintRegionListing);
        fullText.Replace("<Item Pretty or Cryptic Name>", itemText);

        // Handle plurality if necessary
        for (auto& langText : fullText.mText) {
            if (!langText.empty() && utility::str::Contains(langText, '|')) {
                langText = ProcessHintPlurality(langText, hintRegionNames.size() > 1);
            }
        }

        return fullText;
    }

    static Text GenerateLocationHintText(location::Location* location) {
        // TODO: Cryptic Text
        auto textType = Text::PRETTY;
        const auto& itemText = addColor(getTextObject(location->GetCurrentItem()->GetName(), textType), Text::GREEN);
        Text fullText = getTextObject("Location Hint");
        fullText.Replace("<Item Pretty or Cryptic Name>", itemText);
        fullText.Replace("<Location Name>", addColor(Text{location->GetName()}, Text::RED));

        return fullText;
    }

    HintGenerator::HintGenerator(world::World* world) : _world(world) {}

    void HintGenerator::GeneratePathHints() {

        // Shuffle each pool of path locations so that their orders are random
        std::vector<location::Location*> goalLocationsList{};
        auto defeatGanondorf = this->_world->GetLocation("Defeat Ganondorf");
        for (auto& goalLocation : this->_world->GetGoalLocations()) {
            utility::random::ShufflePool(goalLocation->GetPathLocations());
            // Initially we want to create hints for required dungeon's goal locations
            // and then add Ganondorf in once we have at least 1 hint for each required dungeon
            if (goalLocation != defeatGanondorf) {
                goalLocationsList.push_back(goalLocation);
            }
        }

        bool addedGanondorfPathLocation = false;
        auto numPathHints = this->_world->Setting("Number of Path Hints").GetCurrentOptionAsNumber();
        for (size_t i = 0; i < numPathHints; ++i) {
            // Try to get at least one hint for each required dungeon first
            location::Location* goalLocation{};
            if (i < goalLocationsList.size()) {
                goalLocation = goalLocationsList[i];
            } else {
                // Once we've pulled from all required dungeons, then add ganondorf
                // to the list and choose randomly
                if (i == goalLocationsList.size() && !addedGanondorfPathLocation) {
                    addedGanondorfPathLocation = true;
                    goalLocationsList.push_back(defeatGanondorf);
                }

                if (goalLocationsList.empty()) {
                    LOG_TO_DEBUG("No more possible path hints")
                    break;
                }

                goalLocation = utility::random::RandomElement(goalLocationsList);
            }

            // Collect all valid path locations to hint for this location
            std::vector<location::Location*> validPathLocations{};
            for (auto location : goalLocation->GetPathLocations()) {
                // Don't choose locations which have expected items or that are already hinted
                if (location->HasExpectedItem() || location->IsHinted()) {
                    continue;
                }

                // This is unlikely, but also don't choose any locations which are logically
                // necessary for accessing *every* possible hint sign if we're placing path hints
                // on hint signs.
                if (this->_world->Setting("Path Hints on Hint Signs") != "Off") {
                    // Get a list of all hint signs where we can place path hints to use later
                    auto possiblePathHintSigns = search::GetPossibleHintSigns(location);
                    // Remove the ones which wouldn't be relevant
                    std::erase_if(possiblePathHintSigns, [](location::Location* loc) {
                        const auto& hintSignPlacement = loc->GetWorld()->Setting("Path Hints on Hint Signs");
                        return (hintSignPlacement == "Overworld" && loc->HasCategories("Dungeon")) ||
                               (hintSignPlacement == "Dungeon" && loc->HasCategories("Overworld"));
                    });

                    // If none are left, then don't choose this location
                    if (possiblePathHintSigns.empty()) {
                        continue;
                    }
                }

                validPathLocations.push_back(location);
            }

            auto hintLocation = GetHintableLocation(validPathLocations);
            if (hintLocation == nullptr) {
                LOG_TO_DEBUG("No more path locations to hint for " + goalLocation->GetName())
                utility::container::Erase(goalLocationsList, goalLocation);
                --i;
                continue;
            }

            hintLocation->SetHinted(true);
            LOG_TO_DEBUG("Chose " + hintLocation->GetName() + " as path hint for " + goalLocation->GetName());
            auto hintText = GeneratePathHintText(hintLocation, goalLocation);
            this->_pathHints.emplace_back(hintText, PathHint{goalLocation, hintLocation});
        }
    }

    void HintGenerator::GenerateBarrenHints() {
        std::vector<std::string> barrenPool = {};
        std::vector<double> barrenDistributions = {};
        for (auto& [barrenRegion, barrenLocations] : this->_world->GetBarrenRegions()) {
            barrenPool.push_back(barrenRegion);
            // The probability of a region being chosen for a barren hint is the square root
            // of how many locations are in that region
            barrenDistributions.push_back(sqrt(barrenLocations.size()));
        }

        std::discrete_distribution<size_t> barrenDistribution(barrenDistributions.begin(), barrenDistributions.end());
        auto numBarrenHints = this->_world->Setting("Number of Barren Hints").GetCurrentOptionAsNumber();
        for (size_t i = 0; i < numBarrenHints; ++i) {
            if (barrenPool.empty()) {
                LOG_TO_DEBUG("No more barren regions to hint at.")
                break;
            }

            auto regionIndex = barrenDistribution(utility::random::GetGenerator());
            auto& barrenRegion = barrenPool[regionIndex];
            // Set all locations in the selected barren region as hinted at
            // so we don't hint at them again
            for (auto location : this->_world->GetBarrenRegions()[barrenRegion]) {
                location->SetHinted(true);
            }

            auto hintText = GenerateBarrenHintText(barrenRegion);
            this->_barrenHints.emplace_back(hintText, BarrenHint{barrenRegion});
            LOG_TO_DEBUG("Chose " + barrenRegion + " as hinted barren region");

            // Erase the hinted region from the pool
            barrenPool.erase(barrenPool.begin() + regionIndex);
            barrenDistributions.erase(barrenDistributions.begin() + regionIndex);

            // Reset the distribution
            barrenDistribution = std::discrete_distribution<size_t>(barrenDistributions.begin(), barrenDistributions.end());
        }
    }

    void HintGenerator::GenerateItemHints() {
        location::LocationPool possibleItemHintLocations{};
        for (auto location : this->_world->GetAllLocations()) {
            // If the location is progression...
            // and has a major item...
            // and does not have an expected item...
            // and is not already hinted...
            // then it can be hinted as an item hint
            if (location->IsProgression() &&
                location->GetCurrentItem()->IsMajor() &&
               !location->HasExpectedItem() &&
               !location->IsHinted())
            {
                possibleItemHintLocations.push_back(location);
            }
        }

        // Choose randomly until we've selected the appropriate number of item hints
        utility::random::ShufflePool(possibleItemHintLocations);
        auto numItemHints = this->_world->Setting("Number of Item Hints").GetCurrentOptionAsNumber();
        for (auto i = 0; i < numItemHints; ++i) {
            if (possibleItemHintLocations.empty()) {
                LOG_TO_DEBUG("No more possible item hint locations.")
                break;
            }
            auto hintLocation = possibleItemHintLocations.back();
            possibleItemHintLocations.pop_back();
            hintLocation->SetHinted(true);
            auto hintText = GenerateItemHintText(hintLocation);
            this->_itemHints.emplace_back(hintText, ItemHint{hintLocation});
            LOG_TO_DEBUG("Chose " + hintLocation->GetName() + ": " + hintLocation->GetCurrentItem()->GetName() + " as item hint location")
        }
    }

    void HintGenerator::GenerateRemoteLocationHints() {
        // Return early if we're not prioritizing remote locations for location hints
        if (this->_world->Setting("Prioritize Remote Location Hints") == "Off") {
            return;
        }

        // Gather all the remote locations that are progression and haven't already been hinted
        std::vector<location::Location*> remoteLocations{};
        for (auto location : this->_world->GetAllLocations()) {
            if (location->IsProgression() &&
                location->HasCategories("Remote Location") &&
               !location->IsHinted())
            {
                remoteLocations.push_back(location);
            }
        }

        // Shuffle the pool
        utility::random::ShufflePool(remoteLocations);

        // Get as many remote locations as we can
        auto numLocationHints = this->_world->Setting("Number of Location Hints").GetCurrentOptionAsNumber();
        for (int i = 0; i < numLocationHints; ++i) {
            if (i >= remoteLocations.size()) {
                LOG_TO_DEBUG("All remote locations hinted");
                break;
            }
            auto hintLocation = remoteLocations[i];
            hintLocation->SetHinted(true);
            auto hintText = GenerateLocationHintText(hintLocation);
            this->_locationHints.emplace_back(hintText, LocationHint{hintLocation});
            LOG_TO_DEBUG("Chose " + hintLocation->GetName() + " as remote location hint location")
        }
    }

    void HintGenerator::GenerateLocationHints() {
        // Collect all possible locations to hint
        std::vector<location::Location*> possibleLocationHintLocations{};
        for (auto location : this->_world->GetAllLocations()) {
            if (location->IsProgression() && !location->IsHinted() && !location->HasExpectedItem()) {
                possibleLocationHintLocations.push_back(location);
            }
        }

        // Shuffle the pool
        utility::random::ShufflePool(possibleLocationHintLocations);

        // Get the necessary number of hints we want. If we failed to generate enough hints for any of
        // the other hint types, compensate by adding the difference to the number of location hints
        auto numLocationHints = this->_world->Setting("Number of Location Hints").GetCurrentOptionAsNumber();
        auto expectedNumHints = this->_world->Setting("Number of Path Hints").GetCurrentOptionAsNumber() +
                this->_world->Setting("Number of Barren Hints").GetCurrentOptionAsNumber() +
                this->_world->Setting("Number of Item Hints").GetCurrentOptionAsNumber();
        auto totalNumHints = this->_pathHints.size() + this->_barrenHints.size() +
            this->_itemHints.size() + this->_locationHints.size();
        numLocationHints += expectedNumHints - totalNumHints;
        size_t curNumLocationHints = this->_locationHints.size();

        // Choose location hints until we've filled up the amount we need. Since we previously may
        // have already chosen some remote location hints, start with the number we already have
        for (auto i = curNumLocationHints; i < numLocationHints; ++i) {
            if (possibleLocationHintLocations.empty()) {
                LOG_TO_DEBUG("No more possible location hint locations.")
                break;
            }
            auto hintLocation = possibleLocationHintLocations.back();
            possibleLocationHintLocations.pop_back();
            hintLocation->SetHinted(true);
            auto hintText = GenerateLocationHintText(hintLocation);
            this->_locationHints.emplace_back(hintText, LocationHint{hintLocation});
            LOG_TO_DEBUG("Chose " + hintLocation->GetName() + " as location hint location")
        }
    }

    static void AssignHintSignHints(const location::LocationPool& hintSigns, std::vector<Hint> hints, world::World* world) {
        size_t hintsPerSign = std::ceil(static_cast<double>(hints.size()) / static_cast<double>(hintSigns.size()));
        auto& worlds = world->GetRandomizer()->GetWorlds();
        auto& hintSignHints = world->GetHintSignHints();
        auto hintSignHintsOriginal = hintSignHints;

        // Keep trying to place hints until all have been logically placed at least once
        bool successfullyPlaceHints = false;
        int retryCount = 50;
        while (!successfullyPlaceHints) {
            --retryCount;
            if (retryCount < 0) {
                throw std::runtime_error("Failed to properly place hint sign hints. "
                                         "Try using a different seed for generation");
            }

            // Clear any previous attempt at placing hints
            successfullyPlaceHints = true;
            hintSignHints = hintSignHintsOriginal;

            for (auto& hint : hints) {
                item::Item* itemAtHintLocation{};
                location::Location* hintLocation{};
                if (auto pathHint = std::get_if<PathHint>(&hint.data)) {
                    hintLocation = pathHint->hintedLocation;
                } else if (auto itemHint = std::get_if<ItemHint>(&hint.data)) {
                    hintLocation = itemHint->location;
                }
                else if (auto locationHint = std::get_if<LocationHint>(&hint.data)) {
                    hintLocation = locationHint->location;
                }

                // Remove this item from the world and see which hint signs are available to place hints
                if (hintLocation != nullptr) {
                    itemAtHintLocation = hintLocation->GetCurrentItem();
                    hintLocation->RemoveCurrentItem();
                }

                auto search = search::Search::Accessible(&worlds);
                search.SearchWorlds();
                location::LocationPool availableHintSigns{};
                for (const auto& sign : hintSigns) {
                    if (search._visitedLocations.contains(sign) && hintSignHints[sign].size() < hintsPerSign) {
                        availableHintSigns.emplace_back(sign);
                    }
                }

                if (availableHintSigns.empty()) {
                    LOG_TO_DEBUG("No available hint signs to place hint " + hint.text.mText[Text::ENGLISH]);
                    if (hintLocation != nullptr) {
                        hintLocation->SetCurrentItem(itemAtHintLocation);
                    }
                    successfullyPlaceHints = false;
                    break;
                }

                // Place the hint at the hint sign
                auto hintSign = utility::random::RandomElement(availableHintSigns);
                hintSignHints[hintSign].push_back(hint);

                if (hintLocation != nullptr) {
                    hintLocation->SetCurrentItem(itemAtHintLocation);
                }
            }
        }

        // Once we've placed every hint at least once, duplicate hints
        // and place them randomly until all hint signs have the
        // necessary number of hints. Don't check for logic here since
        // every hint can already be logically accessed at some sign
        auto duplicateHints = hints;
        location::LocationPool availableHintSigns{};
        for (const auto& sign : hintSigns) {
            if (hintSignHints[sign].size() < hintsPerSign) {
                availableHintSigns.emplace_back(sign);
            }
        }
        utility::random::ShufflePool(availableHintSigns);
        while (!availableHintSigns.empty()) {
            // Reset the pool of duplicate hints if we run out.
            // This way we'll duplicate every hint at least n times
            // before potentially duplicating each hint n + 1 times
            if (duplicateHints.empty()) {
                duplicateHints = hints;
            }

            auto hint = duplicateHints.back();
            duplicateHints.pop_back();
            location::Location* chosenSign{};
            for (auto sign : availableHintSigns) {
                // Don't give the same hint to a single sign multiple times
                if (!utility::container::ElementInContainer(hintSignHints[sign], hint)) {
                    chosenSign = sign;
                    break;
                }
            }

            if (chosenSign == nullptr) {
                LOG_TO_DEBUG("Could not find any gossip stones to place hint " + hint.text.mText[Text::ENGLISH] + " Trying a different hint.")
                continue;
            }

            hintSignHints[chosenSign].push_back(hint);
            if (hintSignHints[chosenSign].size() >= hintsPerSign) {
                utility::container::Erase(availableHintSigns, chosenSign);
            }
        }
    }

    void HintGenerator::DistributeHints() {

        // Pools to distribute hints
        std::vector<Hint> overworldSignHints{};
        std::vector<Hint> dungeonSignHints{};
        std::vector<Hint> anySignHints{};

        static constexpr int MIDNA = 0;
        static constexpr int HINT_SIGN = 1;

        // Helper function for distributing hints to their appropriate places
        auto distributeHintTypeToPools = [&](std::vector<Hint>& hints, const std::string& hintType) {
            std::vector<int> hintAssignments{};
            if (this->_world->Setting(hintType + " on Midna") != "Off") {
                hintAssignments.push_back(MIDNA);
            }
            if (this->_world->Setting(hintType + " on Hint Signs") != "Off") {
                hintAssignments.push_back(HINT_SIGN);
            }

            // If the user didn't choose any place to assign these hints, return early
            if (hintAssignments.empty()) {
                return;
            }

            for (size_t i = 0; i < hints.size(); ++i) {
                auto& pathHint = hints[i];
                auto distributeTo = hintAssignments[i % hintAssignments.size()];
                if (distributeTo == MIDNA) {
                    this->_world->GetMidnaHints().push_back(pathHint);
                } else if (distributeTo == HINT_SIGN) {
                    auto& pathHintSigns = this->_world->Setting(hintType + " on Hint Signs");
                    if (pathHintSigns == "Overworld") {
                        overworldSignHints.push_back(pathHint);
                    } else if (pathHintSigns == "Dungeon") {
                        dungeonSignHints.push_back(pathHint);
                    } else if (pathHintSigns == "Any") {
                        anySignHints.push_back(pathHint);
                    }
                }
            }
        };

        distributeHintTypeToPools(this->_pathHints, "Path Hints");
        distributeHintTypeToPools(this->_barrenHints, "Barren Hints");
        distributeHintTypeToPools(this->_itemHints, "Item Hints");
        distributeHintTypeToPools(this->_locationHints, "Location Hints");

        // Get our pools of overworld and dungeon signs
        location::LocationPool overworldSignLocations{};
        location::LocationPool dungeonSignLocations{};
        for (auto hintSign : this->_world->GetHintSignLocations()) {
            if (hintSign->HasCategories("Overworld")) {
                overworldSignLocations.push_back(hintSign);
            }
            if (hintSign->HasCategories("Dungeon")) {
                dungeonSignLocations.push_back(hintSign);
            }
        }

        utility::random::ShufflePool(anySignHints);
        // Distribute our hints which can be on any sign to either overworld or dungeon. Ideally
        // we fill up each pool of hints to match the number of signs there are equally. Only do
        // this if we're already placing some hints on only dungeon, or only overworld hint signs.
        if (!overworldSignHints.empty() || !dungeonSignHints.empty()) {
            for (size_t i = 0; i < anySignHints.size(); ++i) {
                const auto& anySignHint = anySignHints[i];
                size_t hintsPerOverworldSign = overworldSignHints.size() / overworldSignLocations.size();
                size_t hintsPerDungeonSign = dungeonSignHints.size() / dungeonSignLocations.size();

                if (hintsPerOverworldSign < hintsPerDungeonSign) {
                    overworldSignHints.push_back(anySignHint);
                } else if (hintsPerDungeonSign < hintsPerOverworldSign) {
                    dungeonSignHints.push_back(anySignHint);
                } else {
                    if (dungeonSignHints.empty() || (dungeonSignHints.size() % dungeonSignLocations.size() != 0 && i % 2 != 0)) {
                        dungeonSignHints.push_back(anySignHint);
                    } else {
                        overworldSignHints.push_back(anySignHint);
                    }
                }
            }
            anySignHints.clear();
        }

        // Now distribute hints to our hint signs
        // If no hints have to be constrained to either overworld or dungeon signs,
        // then distribute all hints over all signs
        if (overworldSignHints.empty() && dungeonSignHints.empty()) {
            AssignHintSignHints(this->_world->GetHintSignLocations(), anySignHints, this->_world);
        }
        if (!overworldSignHints.empty()) {
            AssignHintSignHints(overworldSignLocations, overworldSignHints, this->_world);
        }
        if (!dungeonSignHints.empty()) {
            AssignHintSignHints(dungeonSignLocations, dungeonSignHints, this->_world);
        }
    }

    void HintGenerator::FinalizeHintSignText() {
        for (const auto& [sign, hints] : this->_world->GetHintSignHints()) {
            auto key = sign->GetName() + " Text";
            auto& signText = this->_world->AddNewText(key);

            // Put on all the hints
            for (const auto& hint : hints) {
                signText += hint.text;
                signText.PadToNextBox();
            }

            // pop off last '\n' so we don't have an extra blank textbox
            for (auto& text : signText.mText) {
                text.pop_back();
            }
        }
    }

    // Tell the player which dungeons are required on the sign in front of Link's House
    static void GenerateRequiredDungeonsHint(world::WorldPool& worlds) {
        for (const auto& world : worlds) {
            auto& requiredDungeonText = world->AddNewText("Links House Sign");
            // Use dungeonColors to loop through in base game dungeon order
            for (const auto& [dungeonName, color] : dungeonColors) {
                // Hyrule Castle is implicitly required
                if (dungeonName == "Hyrule Castle") {
                    continue;
                }
                auto dungeon = world->GetDungeon(dungeonName);
                if (dungeon->IsRequired()) {
                    requiredDungeonText += color + getTextObject(dungeonName) + "\n";
                }
            }

            if (requiredDungeonText.Empty()) {
                requiredDungeonText += getTextObject("No Required Dungeons Text");
            }
        }
    }

    static void doItemTextReplacement(const std::unique_ptr<world::World>& world,
                                      const std::string& locationName,
                                      const std::list<std::string>& textNames,
                                      Text::Color color) {
        auto itemName = world->GetLocation(locationName)->GetCurrentItem()->GetName();
        auto itemStandardName = addColor(getTextObject(itemName), color);
        auto itemPrettyName = addColor(getTextObject(itemName, Text::PRETTY), color);
        for (const auto& textName : textNames) {
            auto& text = world->AddNewText(textName);
            text = getTextObject(textName + " Template");
            text.Replace("<Item Standard Name>", itemStandardName);
            text.Replace("<Item Pretty Name>", itemPrettyName);
            text.Capitalize();
            text.BreakLines();
        }
    }

    static void GenerateItemTextReplacements(world::WorldPool& worlds) {
        for (const auto& world : worlds) {
            doItemTextReplacement(world, "Fishing Hole Bottle", {"Fishing Hole Sign Text"}, Text::GREEN);
            doItemTextReplacement(world, "Charlo Donation Blessing", {"Charlo Donation Ask Text"}, Text::GREEN);
            doItemTextReplacement(world, "Sera Shop Slingshot", {"Slingshot Shop Text",
                "Slingshot Shop Too Expensive Text", "Slingshot Shop Purchase Confirmation Text",
                "Slingshot Shop After Purchase Text"}, Text::ORANGE);

            doItemTextReplacement(world, "Barnes Bomb Bag", {"Barnes Special Offer Text"}, Text::ORANGE);
            doItemTextReplacement(world, "Kakariko Village Malo Mart Wooden Shield", {"Kakariko Malo Mart Wooden Shield Purchase Confirmation Text",
                "Kakariko Malo Mart Wooden Shield Too Expensive Text", "Kakariko Malo Mart Wooden Shield Text"}, Text::ORANGE);

            doItemTextReplacement(world, "Kakariko Village Malo Mart Hylian Shield", {"Kakariko Malo Mart Hylian Shield Purchase Confirmation Text",
                "Kakariko Malo Mart Hylian Shield Too Expensive Text", "Kakariko Malo Mart Hylian Shield After Purchase Text",
                "Kakariko Malo Mart Hylian Shield Text"}, Text::ORANGE);

            doItemTextReplacement(world, "Kakariko Village Malo Mart Red Potion", {"Kakariko Malo Mart Red Potion Too Expensive Text",
                "Kakariko Malo Mart Red Potion Purchase Confirmation Text", "Kakariko Malo Mart Red Potion Text"}, Text::ORANGE);

            doItemTextReplacement(world, "Kakariko Village Malo Mart Hawkeye", {"Kakariko Malo Mart Hawkeye Purchase Confirmation Text",
                "Kakariko Malo Mart Hawkeye Too Expensive Text", "Kakariko Malo Mart Hawkeye After Purchase Text",
                "Kakariko Malo Mart Hawkeye Coming Soon Text", "Kakariko Malo Mart Hawkeye Text"}, Text::ORANGE);

            doItemTextReplacement(world, "Castle Town Malo Mart Magic Armor", {"Chudleys Shop Magic Armor Text",
                "Castle Town Malo Mart Magic Armor After Purchase Text", "Castle Town Malo Mart Magic Armor Text",
                "Castle Town Malo Mart Magic Armor Sold Out Text"}, Text::ORANGE);

            doItemTextReplacement(world, "Coro Bottle", {"Coro Bottle Offer 1 Text",
                "Coro Bottle Offer 2 Text", "Coro Bottle Offer 3 Text", "Coro Bottle Offer 4 Text"}, Text::ORANGE);
        }
    }

    void GenerateMidnaHintsText(world::WorldPool& worlds) {
        for (const auto& world : worlds) {
            auto& midnaHintText = world->AddNewText("Custom Midna Call Hints Text");

            // Put required dungeons on Midna.
            // First loop through to get required number
            int numRequiredDungeons = 0;
            for (const auto& dungeon : world->GetDungeonTable() | std::views::values) {
                if (dungeon->IsRequired()) {
                    ++numRequiredDungeons;
                }
            }

            // Set the text for the number of required dungeons
            if (numRequiredDungeons > 0) {
                midnaHintText += getTextObject("Midna Hints Required Dungeons Intro At Least One Dungeon");
                midnaHintText.Replace("<required dungeon count>", std::to_string(numRequiredDungeons));
                midnaHintText.PadToNextBox();

                // Then loop through again to add the dungeon names.
                // Use dungeonColors to loop through in base game dungeon order
                for (const auto& [dungeonName, color] : dungeonColors) {
                    // Hyrule Castle is implicitly required
                    if (dungeonName == "Hyrule Castle") {
                        continue;
                    }
                    auto dungeon = world->GetDungeon(dungeonName);
                    if (dungeon->IsRequired()) {
                        midnaHintText += color + getTextObject(dungeonName) + "<white>\n";
                    }
                }
            } else {
                midnaHintText += getTextObject("Midna Hints Required Dungeons Intro Zero Dungeons");
            }
            midnaHintText.PadToNextBox();

            // Separate Midna hints based on type
            std::vector<Hint> midnaPathHints{};
            std::vector<Hint> midnaBarrenHints{};
            std::vector<Hint> midnaItemHints{};
            std::vector<Hint> midnaLocationHints{};
            for (auto& hint : world->GetMidnaHints()) {
                if (std::holds_alternative<PathHint>(hint.data)) {
                    midnaPathHints.push_back(hint);
                } else if (std::holds_alternative<BarrenHint>(hint.data)) {
                    midnaBarrenHints.push_back(hint);
                } else if (std::holds_alternative<ItemHint>(hint.data)) {
                    midnaItemHints.push_back(hint);
                } else if (std::holds_alternative<LocationHint>(hint.data)) {
                    midnaLocationHints.push_back(hint);
                }
            }

            // Helper function for adding hint text based on type
            auto addHintsToMidnaText = [&](const std::vector<Hint>& hints, const std::string& type) {
                if (!hints.empty()) {
                    midnaHintText += getTextObject("Midna Hints " + type + " Intro");
                    midnaHintText.PadToNextBox();
                    for (auto& [text, data] : hints) {
                        midnaHintText += text;
                        midnaHintText.PadToNextBox();
                    }
                }
            };

            addHintsToMidnaText(midnaPathHints, "Path Hints");

            // Barren hints are presented slightly differently
            if (!midnaBarrenHints.empty()) {
                midnaHintText += getTextObject("Midna Hints Barren Hints Intro");
                midnaHintText.PadToNextBox();
                for (auto& [text, data] : midnaBarrenHints) {
                    midnaHintText += addColor(getTextObject(std::get<BarrenHint>(data).region), Text::PURPLE) + "\n";
                }
                midnaHintText.PadToNextBox();
            }

            addHintsToMidnaText(midnaItemHints, "Item Hints");
            addHintsToMidnaText(midnaLocationHints, "Location Hints");

            // pop off the last new line so we don't get an extra blank textbox
            for (auto& text : midnaHintText.mText) {
                text.pop_back();
            }
        }
    }

    void GenerateAllHints(world::WorldPool& worlds) {

        utility::platform::Log("Generating Hints...");

        CalculatePossiblePathLocations(worlds);
        CalculatePossibleBarrenRegions(worlds);

        for (const auto& world : worlds) {
            auto hintGenerator = HintGenerator{world.get()};
            // Select barren hints first, as we don't want hinted barren regions to conflict with
            // location hints
            hintGenerator.GenerateBarrenHints();
            // Select remote location hints next as these are the most specific ones.
            hintGenerator.GenerateRemoteLocationHints();
            // Then come path and item hints which hint at specific locations that have good items
            hintGenerator.GeneratePathHints();
            hintGenerator.GenerateItemHints();
            // And finally regular location hints can just be any location
            hintGenerator.GenerateLocationHints();

            hintGenerator.DistributeHints();
            hintGenerator.FinalizeHintSignText();
        }

        GenerateRequiredDungeonsHint(worlds);
        GenerateItemTextReplacements(worlds);
        GenerateMidnaHintsText(worlds);
    }
}