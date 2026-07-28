#include "world.hpp"


#include "search.hpp"
#include "../randomizer.hpp"
#include "../utility/exception.hpp"
#include "../utility/file.hpp"
#include "../utility/general.hpp"
#include "../utility/log.hpp"
#include "../utility/platform.hpp"
#include "../utility/random.hpp"
#include "../utility/string.hpp"
#include "../utility/yaml.hpp"

#include <filesystem>
#include <iostream>
#include <ranges>
#include <unordered_set>

namespace randomizer::logic::world
{
    World::World(const int& id, Randomizer* randomizer) :
        _id(id), _randomizer(randomizer)
    {}

    int World::GetID() const
    {
        return this->_id;
    }
    void World::SetSettings(const seedgen::settings::Settings& settings)
    {
        _settings = settings;
    }
    const seedgen::settings::Settings& World::GetSettings() const
    {
        return this->_settings;
    }
    void World::SetRandomizer(Randomizer* randomizer)
    {
        this->_randomizer = randomizer;
    }
    Randomizer* World::GetRandomizer() const
    {
        return this->_randomizer;
    }

    void World::ResolveRandomSettings()
    {
        for (auto& [name, setting] : this->_settings.GetMap())
        {
            setting.ResolveIfRandom();
        }
    }

    void World::ResolveConflictingSettings()
    {
        // If Bonks Do Damage is On and the Damage Multiplier is OHKO and Eldin or Lanayru Twilight are not cleared, then
        // this creates a logically impossible scenario. We can't guarantee repeatable access to a bottled fairy in twilight
        // unless the player starts with the shadow crystal in their inventory. Turn off Bonks Do Damage in this case.
        bool bonksDoDamage = this->Setting("Bonks Do Damage") == "On";
        bool ohko = this->Setting("Logic Damage Multiplier") == "OHKO";
        bool eldinTwilightNotCleared = this->Setting("Eldin Twilight Cleared") == "Off";
        bool lanayruTwilightNotCleared = this->Setting("Lanayru Twilight Cleared") == "Off";
        if (bonksDoDamage && ohko && (eldinTwilightNotCleared || lanayruTwilightNotCleared))
        {
            this->Setting("Bonks Do Damage").SetCurrentOption("Off");
            LOG_TO_DEBUG("Changing Bonks Do Damage to Off");
        }

        // If we're starting as wolf link, the prologue has to be skipped
        if (this->Setting("Starting Form") == "Wolf" && this->Setting("Skip Prologue") == "Off")
        {
            this->Setting("Skip Prologue").SetCurrentOption("On");
            LOG_TO_DEBUG("Turning off Prologue due to Wolf Start");
        }
    }

    void World::Build()
    {
        utility::platform::Log(std::string("Building World ") + std::to_string(this->GetID()));
        this->BuildItemTable();
        this->BuildLocationTable();
        this->LoadLogicMacros();
        this->LoadWorldGraph();
        // TODO: Verify Hint Data
        this->GenerateItemPools();
    }

    void World::BuildItemTable()
    {
        LOG_TO_DEBUG("Building Item Table for World " + std::to_string(this->GetID()));
        auto itemDataTree = LOAD_EMBED_YAML(RANDO_DATA_PATH "items.yaml");
        // Process all nodes of the yaml file. Each node contains one item
        for (const auto& itemNode : itemDataTree)
        {
            // Check to make sure all required fields are present
            YAMLVerifyFields(itemNode, "Name", "Importance", "Id");

            // Required Fields
            auto id = itemNode["Id"].as<int>();
            auto name = itemNode["Name"].as<std::string>();
            auto importanceStr = itemNode["Importance"].as<std::string>();
            auto importance = item::ImportanceFromStr(importanceStr);
            if (importance == item::Importance::INVALID)
            {
                throw std::runtime_error(std::string("Unknown importance \"") + importanceStr + "\" from item node:\n" +
                                         YAML::Dump(itemNode));
            }

            LOG_TO_DEBUG("Processing new item " + name + "\tid: " + std::to_string(id));

            // Optional fields
            auto gameWinningItem = itemNode["Game Winning Item"].as<bool>(false);
            auto dungeonSmallKey = itemNode["Dungeon Small Key"].as<std::string>("");
            auto dungeonBigKey = itemNode["Dungeon Big Key"].as<std::string>("");
            auto dungeonCompass = itemNode["Dungeon Compass"].as<std::string>("");
            auto dungeonMap = itemNode["Dungeon Map"].as<std::string>("");

            // Make the item and insert it into the item table
            auto item = std::make_unique<item::Item>(id, name, this, importance, gameWinningItem,
                dungeonSmallKey != "", dungeonBigKey != "", dungeonCompass != "", dungeonMap != "");

            this->_itemTable.try_emplace(name, std::move(item));

            // Assign dungeon items to dungeons
            auto curItem = this->GetItem(name);
            if (dungeonSmallKey != "")
            {
                this->GetDungeon(dungeonSmallKey)->SetSmallKey(curItem);
            }
            else if (dungeonBigKey != "")
            {
                this->GetDungeon(dungeonBigKey)->SetBigKey(curItem);
            }
            else if (dungeonCompass != "")
            {
                this->GetDungeon(dungeonCompass)->SetCompass(curItem);
            }
            else if (dungeonMap != "")
            {
                this->GetDungeon(dungeonMap)->SetDungeonMap(curItem);
            }

            // Put item into itemIdTable as well
            this->_itemIdTable.try_emplace(id, curItem);
        }
    }

    void World::BuildLocationTable()
    {
        LOG_TO_DEBUG("Building Location Table for World " + std::to_string(this->GetID()));
        auto locationDataTree = LOAD_EMBED_YAML(RANDO_DATA_PATH "locations.yaml");

        // Process all nodes of the yaml file. Each node contains one location
        int locationIdCounter = 0;
        for (const auto& locationNode : locationDataTree)
        {
            // Check to make sure all required fields are present
            YAMLVerifyFields(locationNode, "Name", "Categories", "Metadata");

            // Required Fields
            auto name = locationNode["Name"].as<std::string>();
            std::unordered_set<std::string> categories = {};
            for (const auto& category : locationNode["Categories"])
            {
                categories.insert(category.as<std::string>());
                // Add the category to the registered location categories for this world.
                // When checking a locations categories, we can check to make sure the category
                // is in here to make sure proper categories are being checked.
                this->_registeredLocationCategories.insert(category.as<std::string>());
            }

            // Optional Fields
            auto originalItemName = locationNode["Original Item"].as<std::string>("Nothing");

            // If this location should be removed based on settings, don't insert it into the location table
            if (ShouldRemoveLocation(name, originalItemName))
            {
                this->_intentionallyRemovedLocations.insert(name);
                continue;
            }

            auto originalItem = this->GetItem(originalItemName);
            auto goalLocation = locationNode["Goal Location"].as<bool>(false);
            auto hintPriority = locationNode["Hint Priority"].as<std::string>("Never");
            auto metadata = locationNode["Metadata"];

            // Add metadata fields to categories as well
            if (metadata.IsMap()) {
                for (const auto& fieldNode : metadata) {
                    const auto& category = fieldNode.first.as<std::string>();
                    categories.insert(category);
                    this->_registeredLocationCategories.insert(category);
                }
            }

            auto location = std::make_unique<location::Location>(locationIdCounter++,
                                                                 name,
                                                                 categories,
                                                                 this,
                                                                 originalItem,
                                                                 goalLocation,
                                                                 hintPriority,
                                                                 metadata);

            LOG_TO_DEBUG("Processing new location " + name + "\tid: " + std::to_string(locationIdCounter - 1) +
                         "\toriginal item: " + originalItemName);

            location->SetRegisteredLocationCategories(&this->_registeredLocationCategories);

            this->_locationTable.emplace(name, std::move(location));
        }
    }

    void World::LoadLogicMacros()
    {
        LOG_TO_DEBUG("Loading Macros for World " + std::to_string(this->GetID()));

        auto macrosDataTree = LOAD_EMBED_YAML(RANDO_DATA_PATH "macros.yaml");

        // Process all nodes of the yaml file. Each node contains one macro
        int macroIdCounter = 0;
        for (const auto& macroNode : macrosDataTree)
        {
            auto macroName = macroNode.first.as<std::string>();
            auto macroReqStr = macroNode.second.as<std::string>();

            // Process the macro
            this->_macros[macroIdCounter] = requirement::ParseRequirementString(macroReqStr, this, true);

            // Store it
            this->_macroIndexes[macroName] = macroIdCounter;
            LOG_TO_DEBUG("\"" + macroName + "\" assigned macro index of " + std::to_string(macroIdCounter));
            macroIdCounter += 1;
        }
    }

    void World::LoadWorldGraph()
    {
        LOG_TO_DEBUG("Loading world graph for World " + std::to_string(this->GetID()));

        std::unordered_set<int> definedEvents = {};
        std::unordered_set<std::string> definedAreas = {};

        auto files = std::to_array({
            GET_EMBED_DATA(RANDO_DATA_PATH "world/Root.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/overworld/Ordona Province.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/overworld/Faron Province.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/overworld/Eldin Province.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/overworld/Lanayru Province.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/overworld/Gerudo Desert.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/overworld/Snowpeak Province.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/dungeons/Forest Temple.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/dungeons/Goron Mines.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/dungeons/Lakebed Temple.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/dungeons/Arbiters Grounds.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/dungeons/Snowpeak Ruins.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/dungeons/Temple of Time.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/dungeons/City in the Sky.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/dungeons/Palace of Twilight.yaml"),
            GET_EMBED_DATA(RANDO_DATA_PATH "world/dungeons/Hyrule Castle.yaml"),
        });

        // Loop through and process all files
        for (const auto& file : files)
        {
            auto worldDataTree = YAML::Load(file);
            for (const auto& areaNode : worldDataTree)
            {
                YAMLVerifyFields(areaNode, "Name");

                // Required Fields
                auto areaName = areaNode["Name"].as<std::string>();

                // Optional Fields
                auto mapSector = areaNode["Map Sector"].as<std::string>("");
                auto region = areaNode["Region"].as<std::string>("");
                auto twilight = areaNode["Twilight"].as<std::string>("");
                auto dungeonStartArea = areaNode["Dungeon Start Area"].as<bool>(false);
                auto canWarp = areaNode["Can Warp"].as<bool>(false);
                auto canChangeTime = areaNode["Can Change Time"].as<bool>(false);
                auto canTransformStr = areaNode["Can Transform"].as<std::string>("Always");

                // Copy our events map so we can add autogenerated events to it
                std::map<std::string, std::string> eventNodes = {};
                if (areaNode["Events"])
                {
                    for (const auto& eventNode : areaNode["Events"])
                    {
                        auto eventName = eventNode.first.as<std::string>();
                        auto eventReqStr = eventNode.second.as<std::string>();
                        eventNodes.emplace(eventName, eventReqStr);
                    }
                }

                // Add an event for accessing this area
                eventNodes.emplace("Can Access " + areaName, "Nothing");

                // If we can warp, add the Can Warp event
                if (canWarp)
                {
                    eventNodes.emplace("Can Warp", "Nothing");
                }

                // If this area unlocks a map sector, add the event for the map sector
                if (mapSector != "")
                {
                    eventNodes.emplace(mapSector + " Map Sector", "Nothing");
                }

                // Create and get the area object now so we can pass it to all the other things
                // which need a pointer to it
                auto area = this->GetArea(areaName, /*createIfNotFound = */ true);
                definedAreas.emplace(areaName);

                // Set if the area can change time
                area->SetCanChangeTime(canChangeTime);

                // If this area is in a dungeon, check and set the dungeon start area
                if (this->_dungeons.contains(region))
                {
                    auto dungeon = this->GetDungeon(region);
                    if (dungeonStartArea)
                    {
                        dungeon->SetStartingArea(area);
                    }
                }

                // Set hint region stuff
                if (region != "")
                {
                    area->SetHardAssignedRegion(region);
                    area->AddHintRegion(region);
                }

                // Set the transform status
                // Check to make sure a valid string is used for Can Transform
                const std::unordered_set<std::string> validTransformStatuses = {"Always", "If Transform Anywhere", "Never"};
                if (!validTransformStatuses.contains(canTransformStr))
                {
                    throw std::runtime_error("Unknown Can Transform Status \"" + canTransformStr + "\" in area \"" + areaName +
                                             "\".");
                }

                auto canTransform = canTransformStr == "Always" ||
                                    (this->Setting("Logic Transform Anywhere") == "On" && canTransformStr == "If Transform Anywhere");

                area->SetCanTransform(canTransform);

                // Set the completed twilight macro index if necessary
                if (twilight != "")
                {
                    auto twilightMacro = "Can Complete " + twilight + " Twilight";
                    auto canCompleteTwilightMacroIndex = this->GetMacroIndex(twilightMacro);

                    if (canCompleteTwilightMacroIndex == -1)
                    {
                        throw std::runtime_error('\"' + twilightMacro +
                                                 "\" is not a macro that exists when trying to set twilight macro for " +
                                                 area->GetName());
                    }

                    // Only bother with this if the setting for clearing this twilight is off
                    if (this->Setting(twilight + " Twilight Cleared") == "Off")
                    {
                        area->SetTwilightCompletedMacroIndex(canCompleteTwilightMacroIndex);
                    }
                }

                // Lists of events, locations, and exits that we pass along to the area object
                std::list<std::unique_ptr<area::EventAccess>> events = {};
                std::list<std::unique_ptr<area::LocationAccess>> locations = {};
                std::list<std::unique_ptr<entrance::Entrance>> exits = {};

                // Process events
                for (const auto& [eventName, eventReqStr] : eventNodes)
                {
                    // Parse the requirement string
                    auto eventReq = requirement::ParseRequirementString(eventReqStr, this);

                    // Create the EventAccess wrapper and put it into the list of events for this area
                    auto eventIndex = this->GetEventIndex(eventName);
                    auto event = std::make_unique<area::EventAccess>(eventReq, area, eventIndex);
                    events.emplace_back(std::move(event));
                    definedEvents.emplace(eventIndex);
                }

                // Process locations
                if (areaNode["Locations"])
                {
                    for (const auto& locationNode : areaNode["Locations"])
                    {
                        // Get location name and requirement string
                        auto locationName = locationNode.first.as<std::string>();
                        auto locationReqStr = locationNode.second.as<std::string>();

                        // Ignore the location if it's been intentionally removed
                        if (this->_intentionallyRemovedLocations.contains(locationName))
                        {
                            continue;
                        }

                        auto location = this->GetLocation(locationName);

                        // If this location is in a twilight section, and is not a twilit insect, add the Not_Twilight macro.
                        // We can't assume repeatable access to non-insect locations in twilights.
                        if (twilight != "" && !location->GetOriginalItem()->GetName().ends_with("Twilight Tear"))
                        {
                            locationReqStr = "Not_Twilight and (" + locationReqStr + ")";
                            LOG_TO_DEBUG("Adding Not_Twilight check to requirement for " + locationName);
                        }

                        // Parse the requirement string
                        auto locationReq = requirement::ParseRequirementString(locationReqStr, this);

                        // Create the LocationAccess wrapper and put it into the list of locations for this area

                        auto locationAccess = std::make_unique<area::LocationAccess>(location, locationReq, area);
                        locations.emplace_back(std::move(locationAccess));

                        // Also add this LocationAccess to the locations list of access points
                        location->AddLocationAccess(locations.back().get());
                    }
                }

                // Process Exits
                if (areaNode["Exits"])
                {
                    for (const auto& exitNode : areaNode["Exits"])
                    {
                        // Get the connected area and requirement string
                        auto connectedAreaName = exitNode.first.as<std::string>();
                        auto entranceReqStr = exitNode.second.as<std::string>();
                        auto connectedArea = this->GetArea(connectedAreaName, /*createIfNotFound = */ true);

                        // Parse the requirement string
                        auto entranceReq = requirement::ParseRequirementString(entranceReqStr, this);

                        // Create the Entrance object and put it into the list of exits for this area
                        auto entrance =
                            std::make_unique<entrance::Entrance>(area, connectedArea, entranceReq, this);
                        exits.emplace_back(std::move(entrance));
                    }
                }

                area->SetEvents(events);
                area->SetLocations(locations);
                area->SetExits(exits);
            }
        }

        // Make sure that all used events are defined
        for (const auto& [eventName, eventIndex] : this->_eventIndexes)
        {
            if (!definedEvents.contains(eventIndex))
            {
                throw std::runtime_error("Event \"" + eventName + "\" is used but never defined.");
            }
        }

        // Make sure all used areas are defined
        for (const auto& [areaName, area] : this->_areaTable)
        {
            if (!definedAreas.contains(areaName))
            {
                throw std::runtime_error("Area \"" + areaName + "\" is used but never defined.");
            }
        }

        // Pass a pointer for each exit to the entrance list for the area it connects to
        for (const auto& [areaName, area] : this->_areaTable)
        {
            for (const auto& exit : area->GetExits())
            {
                exit->GetConnectedArea()->AddEntrance(exit);
            }
        }
    }

    bool World::EvaluateSettingCondition(const std::string& condition)
    {
        auto req = requirement::ParseRequirementString(condition, this, true);
        return requirement::EvaluateSimpleRequirement(req, this);
    }

    void World::GenerateItemPools()
    {
        LOG_TO_DEBUG("Now building item pools");
        item_pool::GenerateItemPool(this);
        item_pool::GenerateStartingItemPool(this);

        LOG_TO_DEBUG("Item Pool for world " + std::to_string(this->GetID()) + ":");
        for (const auto& item : this->_itemPool)
        {
            LOG_TO_DEBUG("- " + item->GetName());
        }
        LOG_TO_DEBUG("Starting Inventory for world " + std::to_string(this->GetID()) + ":");
        for (const auto& item : this->_startingItemPool)
        {
            LOG_TO_DEBUG("- " + item->GetName());
        }
    }

    bool World::ShouldRemoveLocation(const std::string& locationName, const std::string& originalItemName)
    {
        // Twilight Tears
        if (originalItemName == "Faron Twilight Tear" && this->Setting("Faron Twilight Cleared") == "On")
        {
            LOG_TO_DEBUG("Removing " + locationName + " because Faron Twilight is cleared.");
            return true;
        }

        if (originalItemName == "Eldin Twilight Tear" && this->Setting("Eldin Twilight Cleared") == "On")
        {
            LOG_TO_DEBUG("Removing " + locationName + " because Eldin Twilight is cleared.");
            return true;
        }

        if (originalItemName == "Lanayru Twilight Tear" && this->Setting("Lanayru Twilight Cleared") == "On")
        {
            LOG_TO_DEBUG("Removing " + locationName + " because Lanayru Twilight is cleared.");
            return true;
        }

        // Ilia Memory Quest
        const auto& iliaQuest = this->Setting("Ilia Memory Quest");
        if ((iliaQuest >= "Letter" && locationName == "Renados Letter") ||
            (iliaQuest >= "Invoice" && locationName == "Telma Invoice") ||
            (iliaQuest >= "Statue" && locationName == "Wooden Statue") ||
            (iliaQuest >= "Charm" && locationName == "Ilia Charm"))
        {
            LOG_TO_DEBUG("Removing " + locationName + " because Ilia Memory Quest is " + iliaQuest.GetCurrentOption() + ".");
            return true;
        }

        return false;
    }

    void World::PerformPreEntranceShuffleTasks()
    {
        this->PlaceVanillaItems();
        this->SetNonProgressLocations();
        this->SanitizeItemPool();
        this->PlacePlandomizerItems();
    }

    void World::PlaceVanillaItems()
    {
        LOG_TO_DEBUG("Now placing vanilla items");

        for (auto& [locationName, location] : this->_locationTable)
        {
            auto originalItem = location->GetOriginalItem();
            auto originalItemName = originalItem->GetName();

            // Place all vanilla items
            // Vanilla Small Keys
            if ((this->Setting("Small Keys") == "Vanilla" &&
                 (originalItem->IsDungeonSmallKey() ||
                  utility::str::Contains(originalItemName, "Ordon Pumpkin", "Ordon Cheese"))) ||
                // Vanilla Big Keys (only include Hyrule Castle Big Key if it has no requirements)
                (this->Setting("Big Keys") == "Vanilla" && originalItem->IsBigKey() && 
                 (originalItemName != "Hyrule Castle Big Key" || this->Setting("Hyrule Castle Big Key Requirements") == "None")) ||
                // Vanilla Maps and Compasses
                (this->Setting("Maps and Compasses") == "Vanilla" &&
                 (originalItem->IsDungeonMap() || originalItem->IsCompass())) ||
                // Hyrule Castle Big Key
                (originalItemName == "Hyrule Castle Big Key" && this->Setting("Hyrule Castle Big Key Requirements") != "None") ||
                // Vanilla Poe Souls
                (originalItemName == "Poe Soul" &&
                 (this->Setting("Poe Souls") == "Vanilla" ||
                  (this->Setting("Poe Souls") == "Dungeon" && location->HasCategories("Overworld")) ||
                  (this->Setting("Poe Souls") == "Overworld" && location->HasCategories("Dungeon")))) ||
                // Vanilla Golden Bugs
                (this->Setting("Golden Bugs") == "Off" && location->HasCategories("Golden Bug")) ||
                // Sky Characters
                (this->Setting("Sky Characters") == "Off" && location->HasCategories("Sky Character")) ||
                // NPC Gifts
                (this->Setting("Gifts From NPCs") == "Off" && location->HasCategories("Npc")) ||
                // Shop Items
                (this->Setting("Shop Items") == "Off" && location->HasCategories("Shop")) ||
                // Hidden Skills
                (this->Setting("Hidden Skills") == "Off" && location->HasCategories("Golden Wolf")) ||
                // Hidden Rupees
                (this->Setting("Hidden Rupees") == "Off" && location->HasCategories("Rupee - Hidden")) ||
                // Freestanding Rupees
                (this->Setting("Freestanding Rupees") == "Off" && location->HasCategories("Rupee - Freestanding")) ||
                // Warp Portals
                (location->HasCategories("Warp Portal")) ||
                // Some locations which will always be vanilla for the time being
                (utility::str::Contains(locationName,
                                               "Renados Letter",
                                               "Telma Invoice",
                                               "Wooden Statue",
                                               "Ilia Charm",
                                               "Defeat Ganondorf",
                                               "Twilit Insect",
                                               "Twilit Bloat")))
            {
                // Change bottled items to all be empty bottles. It's much easier logically to only have to worry about a single
                // item as a bottle instead of all bottled items as bottles for the search algorithm. Other contents will
                // replace the empty bottles after all items have been placed
                if (originalItem->IsBottle())
                {
                    originalItem = this->GetItem("Empty Bottle");
                }

                // Don't place stamps for now
                if (originalItem->IsStamp())
                {
                    originalItem = this->GetItem("Purple Rupee");
                }

                location->SetCurrentItem(originalItem);
                location->SetKnownVanillaItem(true);
                utility::container::Erase(this->_itemPool, originalItem);
            }
        }
    }

    void World::PlacePlandomizerItems()
    {
        for (auto& [location, item] : this->_plandomizerLocations)
        {
            if (!location->IsEmpty())
            {
                throw std::runtime_error("Cannot plandomize \"" + item->GetName() + "\" at \"" + location->GetName() +
                                         "\" because vanilla item \"" + location->GetCurrentItem()->GetName() +
                                         "\" already exists there.");
            }
            location->SetCurrentItem(item);
            utility::container::Erase(this->_itemPool, item);
        }

        // If no world has entrance randomizer enabled, check to see if our plandomized item placements work
        if (std::ranges::none_of(this->GetRandomizer()->GetWorlds(), [](const auto& world) {
            return world->AnyEntranceRandomizerEnabled();
        })) {
            if (!this->_plandomizerLocations.empty() && Setting("Logic Rules") != "No Logic") {
                auto& worlds = this->GetRandomizer()->GetWorlds();
                auto completeItemPool = item_pool::GetCompleteItemPool(worlds);
                auto verifyLogicError = search::VerifyLogic(&worlds, completeItemPool);
                if (verifyLogicError.has_value())
                {
                    throw std::runtime_error("Plandomizer item placements do not work! Reason:\n" + verifyLogicError.value());
                }
            }
        }
    }

    void World::SetNonProgressLocations()
    {
        LOG_TO_DEBUG("Now setting nonprogress locations for world " + std::to_string(this->GetID()));

        // Any manually excluded locations are nonprogress
        for (const auto& locationName : this->_settings.GetExcludedLocations())
        {
            auto location = this->GetLocation(locationName);
            location->SetProgression(false);
        }

        // Some locations not being randomized can conflict with other settings. When
        // the appropriate location and setting conflict, these locations should have their item
        // removed and be set to nonprogress.
        for (auto& [locationName, location] : this->_locationTable)
        {
            auto originalItem = location->GetOriginalItem();
            auto originalItemName = originalItem->GetName();

            // If an NPC gives a key when not randomized, but keys are keysy (keys shouldn't exist)
            if ((this->Setting("Gifts From NPCs") == "Off" && location->HasCategories("Npc") &&
                 ((this->Setting("Small Keys") == "Keysy" && originalItem->IsDungeonSmallKey()) ||
                  (this->Setting("Big Keys") == "Keysy" && originalItem->IsBigKey()) ||
                  (this->Setting("Maps and Compasses") == "Start With" &&
                   (originalItem->IsDungeonMap() || originalItem->IsCompass())))) ||
                // Sky Characters are not randomized, but City in the Sky doesn't require Sky Book Characters (Sky characters
                // shouldn't exist)
                (this->Setting("Sky Characters") == "Off" && this->Setting("City Does Not Require Filled Skybook") == "On" &&
                 location->HasCategories("Sky Character")) ||
                // We're starting with a shop item, but shop items aren't randomized
                (this->Setting("Shop Items") == "Off" && location->HasCategories("Shop") &&
                 utility::container::ElementInContainer(this->_startingItemPool, originalItem)))
            {
                location->RemoveCurrentItem();
                location->SetKnownVanillaItem(false);
                location->SetProgression(false);
            }
        }
    }

    void World::SetTrackerNonProgressLocations() {
        for (auto& [locationName, location] : this->_locationTable) {
            auto originalItemName = location->GetOriginalItem()->GetName();
            // Poe Souls
            if ((originalItemName == "Poe Soul" &&
             (this->Setting("Poe Souls") == "Vanilla" ||
              (this->Setting("Poe Souls") == "Dungeon" && location->HasCategories("Overworld")) ||
              (this->Setting("Poe Souls") == "Overworld" && location->HasCategories("Dungeon")))) ||
            // Vanilla Golden Bugs
            (this->Setting("Golden Bugs") == "Off" && location->HasCategories("Golden Bug")) ||
            // Sky Characters
            (this->Setting("Sky Characters") == "Off" && location->HasCategories("Sky Character")) ||
            // NPC Gifts
            (this->Setting("Gifts From NPCs") == "Off" && location->HasCategories("Npc")) ||
            // Shop Items
            (this->Setting("Shop Items") == "Off" && location->HasCategories("Shop")) ||
            // Hidden Skills
            (this->Setting("Hidden Skills") == "Off" && location->HasCategories("Golden Wolf")) ||
            // Hidden Rupees
            (this->Setting("Hidden Rupees") == "Off" && location->HasCategories("Rupee - Hidden")) ||
            // Freestanding Rupees
            (this->Setting("Freestanding Rupees") == "Off" && location->HasCategories("Rupee - Freestanding"))) {
                location->SetProgression(false);
            }
        }
    }

    void World::PerformPostEntranceShuffleTasks()
    {
        this->AssignAreaProperties();
        this->AssignGoalLocations();
        this->DetermineDungeonDependentLocations();
        this->SetForbiddenItems();
    }

    void World::AssignAreaProperties()
    {
        for (auto& [areaName, area] : this->_areaTable)
        {
            area->AssignHintRegionsAndDungeonLocations();
        }

        for (auto& [areaName, area] : this->_areaTable)
        {
            // Also assign dungeons their starting entrance
            for (const auto& exit : area->GetExits())
            {
                auto parentRegions = exit->GetParentArea()->GetHintRegions();
                auto connectedRegions = exit->GetConnectedArea()->GetHintRegions();
                for (auto& [dungeonName, dungeon] : this->_dungeons)
                {
                    // If this exit leads into a dungeon and its parent area is not part of the dungeon
                    // then this is the entrance that leads into the dungeon
                    if (connectedRegions.contains(dungeonName) && !parentRegions.contains(dungeonName))
                    {
                        dungeon->AddStartingEntrance(exit);
                    }
                }
            }
        }
    }

    void World::AssignGoalLocations()
    {
        std::unordered_map<std::string, location::LocationPool> dungeonGoalLocations = {};
        for (const auto& [dungeonName, dungeon] : this->_dungeons)
        {
            dungeonGoalLocations[dungeonName] = {};
        }
        // Collect all the possible goal locations for each dungeon
        for (auto& [areaName, area] : this->_areaTable)
        {
            for (const auto& locAcc : area->GetLocations())
            {
                auto location = locAcc->GetLocation();
                if (location->IsGoalLocation())
                {
                    for (const auto& region : area->GetHintRegions())
                    {
                        if (dungeonGoalLocations.contains(region))
                        {
                            dungeonGoalLocations.at(region).push_back(location);
                        }
                    }
                }
            }
        }

        // Set a single goal location for each dungeon
        for (auto& [dungeonName, dungeon] : this->_dungeons)
        {
            auto& possibleGoalLocations = dungeonGoalLocations.at(dungeonName);
            // If a goal location becomes unreachable due to beatable only logic, then it's possible a dungeon may not be
            // assigned a goal location. Dungeons without a goal location cannot be chosen as required dungeons.
            if (!possibleGoalLocations.empty())
            {
                dungeon->SetGoalLocation(utility::random::RandomElement(possibleGoalLocations));
            }
            else
            {
                LOG_TO_DEBUG("No goal location could be chosen for " + dungeonName);
            }
        }
    }

    void World::SetForbiddenItems()
    {
        // Prevent small keys from appearing on bosses if the setting is on
        if (this->Setting("No Small Keys on Bosses") == "On")
        {
            // Gather all boss locations (heart container and dungeon reward checks)
            auto bossLocations = this->GetAllLocations();
            utility::container::FilterAndEraseFromVector(
                bossLocations,
                [](const auto& location)
                { return !utility::str::Contains(location->GetName(), "Heart Container", "Dungeon Reward"); });

            // Gather all small key items
            item_pool::ItemPool smallKeys = {};
            for (const auto& [itemName, item] : this->_itemTable)
            {
                if (item->IsDungeonSmallKey() || utility::general::IsAnyOf(itemName,
                                                                                  "Ordon Pumpkin",
                                                                                  "Ordon Cheese",
                                                                                  "North Faron Woods Gate Key",
                                                                                  "Gerudo Desert Bulblin Camp Key"))
                {
                    smallKeys.push_back(item.get());
                }
            }

            // Set the small keys as forbidden on the boss locations
            for (auto& location : bossLocations)
            {
                for (const auto& smallKey : smallKeys)
                {
                    location->AddForbiddenItem(smallKey);
                }
            }
        }
    }

    void World::DetermineDungeonDependentLocations()
    {
        for (const auto& [dungeonName, dungeon] : this->_dungeons)
        {
            // Hyrule Castle is implicitly required
            if (dungeonName == "Hyrule Castle") {
                continue;
            }

            // Disable the dungeon's starting entrances
            for (auto& entrance : dungeon->GetStartingEntrances())
            {
                entrance->SetDisbled(true);
            }

            // Run an accessibility search to see which locations inherently require accessing this dungeon
            auto completeItemPool = item_pool::GetCompleteItemPool(this->_randomizer->GetWorlds());
            auto search = search::Search::Accessible(&this->_randomizer->GetWorlds(), completeItemPool);
            search.SearchWorlds();
            for (auto& location : this->_locationTable | std::ranges::views::values) {
                // Don't check locations which are part of this dungeon
                if (utility::container::ElementInContainer(dungeon->GetLocations(), location.get())) {
                    continue;
                }

                // If the search does not contain this location, then the location is dependent on accessing this dungeon
                if (!search._visitedLocations.contains(location.get())) {
                    dungeon->AddOutsideDependentLocation(location.get());
                }
            }

            // Re-enable the dungeon's entrances
            for (auto& entrance : dungeon->GetStartingEntrances())
            {
                entrance->SetDisbled(false);
            }
        }
    }

    // For no logic, we're purely going to base whether the dungeon is required on the Hyrule Castle
    // Barrier requirements and Hyrule Castle Big Key chest requirements
    bool World::IsNoLogicRequiredDungeon(const std::unique_ptr<dungeon::Dungeon>& dungeon) {
        auto& barrierRequirements = this->Setting("Hyrule Barrier Requirements");
        auto& bigKeyRequirements = this->Setting("Hyrule Castle Big Key Requirements");
        auto barrierDungeonCount = this->Setting("Hyrule Barrier Dungeons").GetCurrentOptionAsNumber();
        auto bigkeyDungeonCount = this->Setting("Hyrule Castle Big Key Dungeons").GetCurrentOptionAsNumber();

        // If all dungeons are required, then always return true
        if ((barrierRequirements == "Dungeons" && barrierDungeonCount == 8) ||
            (bigKeyRequirements == "Dungeons" && bigkeyDungeonCount == 8))
        {
            return true;
        }

        bool dungeonHasFusedShadow = std::ranges::any_of(dungeon->GetLocations(), [](const auto& location) {
            return location->GetCurrentItem()->GetName() == "Progressive Fused Shadow";
        });
        bool dungeonHasMirrorShard = std::ranges::any_of(dungeon->GetLocations(), [](const auto& location) {
            return location->GetCurrentItem()->GetName() == "Progressive Mirror Shard";
        });

        if (dungeonHasFusedShadow && (barrierRequirements == "Fused Shadows" || bigKeyRequirements == "Fused Shadows")) {
            return true;
        }

        if (dungeonHasMirrorShard && (barrierRequirements == "Mirror Shards" || bigKeyRequirements == "Mirror Shards")) {
            return true;
        }

        if (barrierRequirements == "Vanilla" && (dungeon->GetName() == "Palace of Twilight" ||
            (this->Setting("Palace of Twilight Requirements") == "Vanilla" && dungeon->GetName() == "City in the Sky")))
        {
            return true;
        }

        return false;
    }

    void World::DetermineRequiredDungeons()
    {
        for (const auto& [dungeonName, dungeon] : this->_dungeons)
        {
            // To determine if a dungeon is required, we're going to disable all of its entrances and then check to see
            // that the game is still beatable. If the game is not beatable with the dungeon entrances disabled, then the
            // dungeon is required. For no logic, we determine required dungeons differently since otherwise no dungeon
            // would be required.

            // Hyrule Castle is implicitly required
            if (dungeonName == "Hyrule Castle") {
                continue;
            }

            // Disable the dungeon's starting entrances
            for (auto& entrance : dungeon->GetStartingEntrances())
            {
                entrance->SetDisbled(true);
            }

            // Check if the game is beatable, set dungeon as required if so. If the dungeon is not required and barren
            // unrequired dungeons is on, then set all the locations in the unrequired dungeon as nonprogress.
            auto completeItemPool = item_pool::GetCompleteItemPool(this->_randomizer->GetWorlds());
            if (!search::GameBeatable(&this->_randomizer->GetWorlds(), completeItemPool) ||
                (this->Setting("Logic Rules") == "No Logic" && this->IsNoLogicRequiredDungeon(dungeon)))
            {
                dungeon->SetRequired(true);
            }
            else if (this->Setting("Unrequired Dungeons Are Barren") == "On")
            {
                for (auto& location : dungeon->GetLocations())
                {
                    location->SetProgression(false);
                }
                for (auto& location : dungeon->GetOutsideDependentLocations())
                {
                    location->SetProgression(false);
                }
            }

            // Re-enable the dungeon's entrances
            for (auto& entrance : dungeon->GetStartingEntrances())
            {
                entrance->SetDisbled(false);
            }
        }
    }

    void World::SanitizeItemPool()
    {
        auto junkPool = item_pool::GetInitialJunkPool();

        // Depending on the Trap item Frequency setting, add some amount of ice traps to the pool
        if (this->Setting("Trap Item Frequency") == "Few")
        {
            junkPool.emplace("Foolish Item", 6);
        }
        else if (this->Setting("Trap Item Frequency") == "Many")
        {
            junkPool.emplace("Foolish Item", 27);
        }
        else if (this->Setting("Trap Item Frequency") == "Mayhem")
        {
            junkPool.emplace("Foolish Item", 64);
        }
        else if (this->Setting("Trap Item Frequency") == "Nightmare")
        {
            junkPool.clear();
            junkPool.emplace("Foolish Item", 1);
        }

        // Create an actual item pool from the junk items
        item_pool::ItemPool mainJunkPool = {};
        for (const auto& [itemName, count] : junkPool)
        {
            auto item = this->GetItem(itemName);
            for (auto i = 0; i < count; i++)
            {
                mainJunkPool.push_back(item);
            }
        }

        auto allItemLocations = this->GetAllLocations();
        const auto numEmptyLocations = std::ranges::count_if(allItemLocations, [](const auto& location) {
            return location->IsEmpty();
        });

        // Create a copy of the real pool we just made. When adding junk items we want to add all the items from the junk pool
        // once if possible, then if there's more space left pick randomly from the full pool
        auto mainJunkPoolCopy = mainJunkPool;

        // Add items until the pool's size matches the number of empty locations
        while (this->_itemPool.size() < numEmptyLocations)
        {
            item::Item* randomJunkItem;
            if (!mainJunkPool.empty())
            {
                randomJunkItem = utility::random::PopRandomElement(mainJunkPool);
            }
            else
            {
                randomJunkItem = utility::random::RandomElement(mainJunkPoolCopy);
            }
            this->_itemPool.emplace_back(randomJunkItem);
            LOG_TO_DEBUG("Added junk item \"" + randomJunkItem->GetName() + "\" to item pool for world " +
                         std::to_string(this->GetID()));
        }
    }

    void World::SetSearchStartingProperties(search::Search* search) const
    {
        // Set the root area to have all player forms and times of day (necessary for entrance rando validation)
        const auto root = this->GetRootArea();
        search->_areaFormTime[root] = requirement::FormTime::ALL;
    }

    void World::PerformPostFillTasks()
    {
        this->FinalizeBottleContents();
    }

    void World::FinalizeBottleContents()
    {
        // Replace 3 bottles with other bottle contents we currently use.
        const auto bottleWithGreatFairiesTears = this->GetItem("Bottle with Great Fairies Tears");
        const auto bottleWithHalfMilk = this->GetItem("Bottle with Half Milk");
        const auto bottleWithLanternOil = this->GetItem("Bottle with Lantern Oil");
        const auto emptyBottle = this->GetItem("Empty Bottle");
        item_pool::ItemPool bottlePool = {bottleWithGreatFairiesTears,
                                                        bottleWithHalfMilk,
                                                        bottleWithLanternOil,
                                                        emptyBottle};

        // If npc gifts are vanilla, then set those vanilla bottles appropriately
        if (this->Setting("Gifts From NPCs") == "Off")
        {
            for (auto& [locationName, location] : this->_locationTable)
            {
                auto originalItem = location->GetOriginalItem();
                if (location->HasCategories("Npc") && originalItem->IsBottle())
                {
                    location->SetCurrentItem(originalItem);
                }
            }
        }
        // Otherwise gather all the locations which have a bottle and replace the bottles at those locations instead
        else
        {
            // Gather the bottle locations
            location::LocationPool bottleLocations = {};
            for (auto& [locationName, location] : this->_locationTable)
            {
                auto originalItem = location->GetCurrentItem();
                if (originalItem->IsBottle())
                {
                    bottleLocations.push_back(location.get());
                }
            }

            // Place the new bottle items
            utility::random::ShufflePool(bottleLocations);
            for (auto& bottleLocation : bottleLocations)
            {
                if (!bottlePool.empty()) {
                    bottleLocation->SetCurrentItem(utility::random::PopRandomElement(bottlePool));
                } else {
                    bottleLocation->SetCurrentItem(this->GetItem("Empty Bottle"));
                }
            }
        }
    }

    void World::AddPlandomizedLocation(location::Location* location, item::Item* item)
    {
        if (this->_plandomizerLocations.contains(location))
        {
            throw std::runtime_error("Plandomizer Error: multiple entries for \"" + location->GetName() + "\" in world " +
                                     std::to_string(this->_id));
        }
        this->_plandomizerLocations[location] = item;
    }

    void World::AddPlandomizedEntrance(entrance::Entrance* entrance, entrance::Entrance* target)
    {
        for (const auto& [plandoEntrance, plandoTarget] : this->_plandomizerEntrances)
        {
            if (plandoEntrance == entrance)
            {
                throw std::runtime_error("Plandomizer Error: multiple entries for \"" + entrance->GetOriginalName() +
                                         "\" in world " + std::to_string(this->_id));
            }
            if (plandoTarget == target)
            {
                throw std::runtime_error("Plandomizer Error: multiple entrances target \"" + target->GetOriginalName() +
                                         "\" in world " + std::to_string(this->_id));
            }
        }
        this->_plandomizerEntrances[entrance] = target;
    }

    std::unordered_map<entrance::Entrance*, entrance::Entrance*> World::GetPlandomizerEntrances()
    {
        return this->_plandomizerEntrances;
    }

    dungeon::Dungeon* World::GetDungeon(const std::string& name)
    {
        if (!this->_dungeons.contains(name))
        {
            this->_dungeons.emplace(name, std::make_unique<dungeon::Dungeon>(name, this));
            LOG_TO_DEBUG("Added new dungeon \"" + name + "\" to world " + std::to_string(this->_id));
        }
        return this->_dungeons.at(name).get();
    }

    const std::map<std::string, std::unique_ptr<dungeon::Dungeon>>& World::GetDungeonTable() const
    {
        return this->_dungeons;
    }

    item::Item* World::GetItem(const std::string& name, const bool& ignoreError /*= false*/)
    {
        if (name == "Nothing")
        {
            return item::Nothing.get();
        }

        if (!this->_itemTable.contains(name))
        {
            if (!ignoreError)
            {
                throw std::runtime_error("Unknown item name \"" + name + "\"");
            }
            return nullptr;
        }
        return this->_itemTable.at(name).get();
    }

    item::Item* World::GetItem(uint16_t id, const bool& ignoreError /*= false*/) {
        if (!this->_itemIdTable.contains(id))
        {
            if (!ignoreError)
            {
                throw std::runtime_error("Unknown item id \"" + std::to_string(id) + "\"");
            }
            return item::Nothing.get();
        }
        return this->_itemIdTable.at(id);
    }

    item::Item* World::GetGameWinningItem() const
    {
        return this->_itemTable.at("Game Beatable").get();
    }

    item::Item* World::GetShadowCrystal()
    {
        return this->_itemTable.at("Shadow Crystal").get();
    }

    item_pool::ItemPool& World::GetItemPool()
    {
        return this->_itemPool;
    }

    item_pool::ItemPool& World::GetStartingItemPool()
    {
        return this->_startingItemPool;
    }

    location::Location* World::GetLocation(const std::string& name)
    {
        if (!this->_locationTable.contains(name))
        {
            throw std::runtime_error("Unknown location name \"" + name + "\"");
        }
        return this->_locationTable.at(name).get();
    }

    location::LocationPool World::GetAllLocations(const bool& includeNonItemLocations /* = false */)
    {
        location::LocationPool locationPool = {};
        for (const auto& [locationName, location] : this->_locationTable)
        {
            if (includeNonItemLocations || !location->HasCategories("Non-Item Location"))
            {
                locationPool.emplace_back(location.get());
            }
        }
        return locationPool;
    }

    area::Area* World::GetArea(const std::string& name, const bool& createIfNotFound /* = false */)
    {
        if (!this->_areaTable.contains(name))
        {
            if (createIfNotFound)
            {
                this->_areaTable.emplace(name, std::make_unique<area::Area>(name, this));
            }
            else
            {
                throw std::runtime_error("Unknown area name \"" + name + "\"");
            }
        }
        return this->_areaTable.at(name).get();
    }

    area::Area* World::GetRootArea() const
    {
        return this->_areaTable.at("Root").get();
    }

    const std::map<std::string, std::unique_ptr<area::Area>>& World::GetAreaTable() const
    {
        return this->_areaTable;
    }

    entrance::Entrance* World::GetEntrance(const std::string& originalName)
    {
        auto [parentAreaName, connectedAreaName] = entrance::GetParentAndConnectedAreaNames(originalName);
        auto parentArea = this->GetArea(parentAreaName);
        auto connectedArea = this->GetArea(connectedAreaName);
        for (const auto& exit : parentArea->GetExits())
        {
            if (exit->GetOriginalConnectedArea() == connectedArea)
            {
                return exit;
            }
        }

        throw std::runtime_error("\"" + originalName + "\" is not a known connection");
    }

    int World::GetNewEntranceID()
    {
        return this->_entranceIdCounter++;
    }

    entrance::EntrancePool World::GetShuffleableEntrances(const entrance::Type& type,
                                                          bool onlyPrimary /* = false */)
    {
        entrance::EntrancePool shuffleableEntrances = {};
        for (const auto& [areaName, area] : this->GetAreaTable())
        {
            for (const auto& exit : area->GetExits())
            {
                if ((type == exit->GetType() || type == entrance::Type::ALL) &&
                    (!onlyPrimary || exit->IsPrimary()) && exit->GetType() != entrance::Type::INVALID)
                {
                    shuffleableEntrances.push_back(exit);
                }
            }
        }
        return shuffleableEntrances;
    }

    entrance::EntrancePool World::GetShuffledEntrances(
        const entrance::Type& type /* = entrance::Type::ALL */,
        bool onlyPrimary /* = false */)
    {
        auto entrances = this->GetShuffleableEntrances(type, onlyPrimary);

        // Remove any entrances which aren't shuffled
        utility::container::FilterAndEraseFromVector(entrances, [](const auto& e) { return !e->IsShuffled(); });

        return entrances;
    }

    std::unordered_map<entrance::Entrance*, int>& World::GetExitTimeFormCache()
    {
        return this->_exitTimeFormCache;
    }

    int World::GetMacroIndex(const std::string& macroName) const
    {
        if (this->_macroIndexes.contains(macroName))
        {
            return this->_macroIndexes.at(macroName);
        }
        return -1;
    }

    const requirement::Requirement& World::GetMacro(const int& macroIndex)
    {
        return this->_macros.at(macroIndex);
    }

    int World::GetEventIndex(const std::string& eventName, bool addIfNone /*= true*/)
    {
        // If the event doesn't exist
        if (!this->_eventIndexes.contains(eventName))
        {
            if (addIfNone)
            {
                auto index = this->_randomizer->GetNewEventID();
                this->_eventIndexes.emplace(eventName, index);
                this->_eventNames.emplace(index, eventName);
                LOG_TO_DEBUG("Event \"" + eventName + "\" was assigned eventIndex " + std::to_string(index));
            }
            else
            {
                throw std::runtime_error("Event \"" + eventName + "\" does not exist");
            }
        }

        return this->_eventIndexes.at(eventName);
    }

    std::string World::GetEventName(const int& eventIndex)
    {
        if (!this->_eventNames.contains(eventIndex))
        {
            LOG_TO_ERROR("Invalid Event Index");
        }
        return this->_eventNames.at(eventIndex);
    }

    seedgen::settings::Setting& World::Setting(const std::string& settingName)
    {
        auto& settings = this->_settings;
        // Check to make sure the setting exists
        if (!settings.GetMap().contains(settingName))
        {
            throw std::runtime_error("Setting \"" + settingName + "\" is not a known setting");
        }
        return settings.GetMap().at(settingName);
    }

    bool World::AnyEntranceRandomizerEnabled() {
        return Setting("Randomize Starting Spawn") != "Off" ||
               Setting("Randomize Dungeon Entrances") != "Off" ||
               Setting("Randomize Boss Entrances") != "Off" ||
               Setting("Randomize Grotto Entrances") != "Off" ||
               Setting("Randomize Cave Entrances") != "Off" ||
               Setting("Randomize Interior Entrances") != "Off" ||
               Setting("Randomize Overworld Entrances") != "Off";
    }
} // namespace randomizer::logic::world
