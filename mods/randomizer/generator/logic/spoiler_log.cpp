#include "spoiler_log.hpp"

#include "entrance_shuffle.hpp"
#include "../randomizer.hpp"
#include "../utility/file.hpp"
#include "../utility/platform.hpp"
#include "../utility/string.hpp"
#include "../utility/yaml.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

namespace randomizer::logic::spoiler_log
{
    std::string SpoilerFormatLocation(const location::Location* location, const size_t& longestNameLength)
    {
        const auto numSpaces = longestNameLength - location->GetName().length();
        const std::string spaces(numSpaces, ' ');

        return location->GetName() + ": " + spaces + location->GetCurrentItem()->GetName();
    }

    std::string SpoilerFormatEntrance(const entrance::Entrance* entrance, const size_t& longestNameLength)
    {
        const auto numSpaces = longestNameLength - entrance->GetAlias().length();
        const std::string spaces(numSpaces, ' ');
        const auto replacement = entrance->GetReplaces();

        return entrance->GetAlias() + ": " + spaces + replacement->GetAliasFrom();
    }

    void LogBasicInfo(std::ofstream& log, Randomizer* randomizer)
    {
        // TODO: print mod version instead of dusklight version
        // log << "Dusklight Version: " << DUSK_WC_DESCRIBE << std::endl;
        log << "Seed: " << randomizer->GetConfig().GetSeed() << std::endl;
        log << "Permalink: " << randomizer->GetConfig().GetPermalink() << std::endl;
        log << "Hash: " << randomizer->GetConfig().GetHash() << std::endl;
    }

    void LogSettings(std::ofstream& log, Randomizer* randomizer)
    {
        log << std::endl << "# Settings" << std::endl;
        log << YAML::Dump(randomizer->GetConfig().SettingsToYaml()) << std::endl;
    }

    void GenerateSpoilerLog(Randomizer* randomizer)
    {
        utility::platform::Log("Generating Spoiler Log");

        // Create folders
        if (!utility::file::dirExists(randomizer->GetSeedOutputPath()))
        {
            utility::file::create_directories(randomizer->GetSeedOutputPath());
        }

        auto& config = randomizer->GetConfig();
        auto& worlds = randomizer->GetWorlds();

        std::filesystem::path filepath = randomizer->GetSeedOutputPath() / (config.GetHash() + " Spoiler Log.txt");
        std::ofstream spoilerLog;
        spoilerLog.open(filepath);

        LogBasicInfo(spoilerLog, randomizer);

        // Gather worlds with starting inventories
        std::list<world::World*> worldsWithStartingInventories = {};
        for (const auto& world : worlds)
        {
            if (!world->GetStartingItemPool().empty())
            {
                worldsWithStartingInventories.push_back(world.get());
            }
        }
        // Print starting inventories if there are any
        if (!worldsWithStartingInventories.empty())
        {
            spoilerLog << std::endl << "All Starting Items:" << std::endl;
            for (const auto& world : worldsWithStartingInventories)
            {
                spoilerLog << "    World " << world->GetID() << ":" << std::endl;
                for (const auto& item : world->GetStartingItemPool())
                {
                    spoilerLog << "      - " << item->GetName() << std::endl;
                }
            }
        }

        // Gather worlds with required dungeons
        std::list<world::World*> worldsWithRequiredDungeons = {};
        for (const auto& world : worlds)
        {
            for (const auto& [dungeonName, dungeon] : world->GetDungeonTable()) {
                if (dungeon->IsRequired()) {
                    worldsWithRequiredDungeons.push_back(world.get());
                    break;
                }
            }
        }
        // Print required dungeons if there are any
        if (!worldsWithRequiredDungeons.empty())
        {
            spoilerLog << std::endl << "Required Dungeons:" << std::endl;
            for (const auto& world : worldsWithRequiredDungeons)
            {
                spoilerLog << "    World " << world->GetID() << ":" << std::endl;
                for (const auto& [dungeonName, dungeon] : world->GetDungeonTable()) {
                    if (dungeon->IsRequired()) {
                        spoilerLog << "      - " << dungeonName << std::endl;
                    }
                }
            }
        }

        // Get name lengths for pretty formatting
        size_t longestNameLength = 0;
        for (const auto& sphere : randomizer->GetPlaythroughSpheres())
        {
            for (const auto& location : sphere)
            {
                longestNameLength = std::max(location->GetName().length(), longestNameLength);
            }
        }

        // Print playthrough
        int sphereNum = 0;
        spoilerLog << std::endl << "Playthrough:" << std::endl;
        for (auto& sphere : randomizer->GetPlaythroughSpheres())
        {
            sphereNum += 1;
            spoilerLog << "    Sphere " << sphereNum << ":" << std::endl;
            sphere.sort([](const auto& a, const auto& b) { return a->GetName()[0] < b->GetName()[0]; });
            for (const auto& location : sphere)
            {
                spoilerLog << "        " << SpoilerFormatLocation(location, longestNameLength) << std::endl;
            }
        }

        // Get name lengths for pretty formatting
        longestNameLength = 0;
        for (const auto& sphere : randomizer->GetEntranceSpheres())
        {
            for (const auto& entrance : sphere)
            {
                longestNameLength = std::max(entrance->GetAlias().length(), longestNameLength);
            }
        }

        // Print entrance playthrough
        sphereNum = 0;
        if (longestNameLength != 0)
        {
            spoilerLog << std::endl << "Entrance Playthrough:" << std::endl;
        }
        for (auto& sphere : randomizer->GetEntranceSpheres())
        {
            sphereNum += 1;
            if (sphere.empty())
            {
                continue;
            }
            spoilerLog << "    Sphere " << sphereNum << ":" << std::endl;
            sphere.sort([](auto& e1, auto& e2) { return e1->GetID() < e2->GetID(); });
            for (const auto& entrance : sphere)
            {
                spoilerLog << "        " << SpoilerFormatEntrance(entrance, longestNameLength) << std::endl;
            }
        }

        // Recalculate longest name length for all locations
        longestNameLength = 0;
        for (const auto& world : worlds)
        {
            for (const auto& location : world->GetAllLocations())
            {
                longestNameLength = std::max(location->GetName().length(), longestNameLength);
            }
        }

        // Print All Locations
        spoilerLog << std::endl << "All Locations:" << std::endl;
        for (const auto& world : worlds)
        {
            spoilerLog << "    World " << world->GetID() << ":" << std::endl;
            for (const auto& location : world->GetAllLocations())
            {
                spoilerLog << "        " << SpoilerFormatLocation(location, longestNameLength) << std::endl;
            }
        }

        // Recalculate longest name length for all shuffled entrances
        longestNameLength = 0;
        for (const auto& world : worlds)
        {
            for (const auto& entrance : world->GetShuffledEntrances())
            {
                longestNameLength = std::max(entrance->GetAlias().length(), longestNameLength);
            }
        }
        // Print all randomized entrances
        if (longestNameLength != 0)
        {
            spoilerLog << std::endl << "All Entrances:" << std::endl;
        }
        for (const auto& world : worlds)
        {
            auto entrances = world->GetShuffledEntrances();
            if (!entrances.empty())
            {
                spoilerLog << "    World " << world->GetID() << ":" << std::endl;
                // Create entrance pools to easily separate the entrances by type
                auto entrancePools = entrance_shuffle::CreateEntrancePools(world.get());
                auto mixedPools = world->GetSettings().GetMixedEntrancePools();
                for (auto& [entranceType, entrancePool] : entrancePools)
                {
                    auto typeStr = entrance::TypeToStr(entranceType);
                    // If this is a mixed pool, display the types it mixed
                    if (typeStr.starts_with("Mixed Pool"))
                    {
                        typeStr += " (";
                        auto& pool = mixedPools.front();
                        for (const auto& type : pool)
                        {
                            typeStr += type + " + ";
                        }
                        typeStr.erase(typeStr.end() - 3, typeStr.end()); // Remove the last " + "
                        typeStr += ")";
                        mixedPools.pop_front();
                    }
                    spoilerLog << "        " << typeStr << ":" << std::endl;
                    std::ranges::sort(entrancePool, [](auto& e1, auto& e2) {
                        return e1->GetID() < e2->GetID();
                    });
                    for (const auto& entrance : entrancePool)
                    {
                        // Ignore entrances that are impossible
                        if (entrance->GetRequirement()._type == requirement::Type::IMPOSSIBLE)
                        {
                            continue;
                        }
                        spoilerLog << "            " << SpoilerFormatEntrance(entrance, longestNameLength) << std::endl;
                    }
                }
            }
        }

                // Print Hints
        spoilerLog << std::endl << "Hints:" << std::endl;
        for (const auto& world : worlds) {
            // Midna hints first
            spoilerLog << "    World " << world->GetID() << ":" << std::endl;
            spoilerLog << "        Midna:" << std::endl;
            auto midnaText = world->GetText("Custom Midna Call Hints Text");
            midnaText = utility::str::Replace(midnaText, "\n", "\n            ");
            spoilerLog << "            " << midnaText << std::endl;

            spoilerLog << std::endl;
            // Put sign hints into a structure that will automatically sort them
            const auto& hintSignHints = world->GetHintSignHints();
            std::map<std::string, std::list<std::string>> hints{};
            for (const auto& [hintSign, hintsOnSign] : hintSignHints) {
                for (const auto& hint : hintsOnSign) {
                    // Format the hints so they appear nice in the spoiler log
                    auto hintText = hint.text.mText[Text::ENGLISH];
                    hintText = utility::str::Replace(hintText, "\n", "\n                ");
                    // If this is a path hint, also include the location and item it's referring to
                    if (auto pathHint = std::get_if<hints::PathHint>(&hint.data)) {
                        hintText += " (" + pathHint->hintedLocation->GetName() + ": " + pathHint->hintedLocation->GetCurrentItem()->GetName() + ")";
                    }
                    hints[hintSign->GetName()].push_back(hintText);
                }
            }

            // Then print them
            if (!hints.empty()) {
                spoilerLog << "        Hint Signs:" << std::endl;
                for (const auto& [signName, hintsOnSign] : hints) {
                    spoilerLog << "           " << signName << ":" << std::endl;
                    for (const auto& hint : hintsOnSign) {
                        spoilerLog << "               " << hint << std::endl;
                    }
                }
                spoilerLog << std::endl;
            }
        }

        // Log Settings
        LogSettings(spoilerLog, randomizer);

        spoilerLog.close();

        utility::platform::Log("Wrote spoiler log to " + filepath.string());
    }

    void GenerateAntiSpoilerLog(Randomizer* randomizer)
    {
        // Create logs folder if it doesn't exist
        if (!utility::file::dirExists(randomizer->GetSeedOutputPath()))
        {
            utility::file::create_directories(randomizer->GetSeedOutputPath());
        }

        std::filesystem::path filepath = randomizer->GetSeedOutputPath() / (randomizer->GetConfig().GetHash() + " Anti-Spoiler Log.txt");
        std::ofstream antiSpoilerLog;
        antiSpoilerLog.open(filepath);

        LogBasicInfo(antiSpoilerLog, randomizer);
        LogSettings(antiSpoilerLog, randomizer);
    }
} // namespace randomizer::logic::spoiler_log
