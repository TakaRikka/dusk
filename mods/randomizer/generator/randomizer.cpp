#include "randomizer.hpp"

#include "logic/entrance_shuffle.hpp"
#include "logic/fill.hpp"
#include "logic/flatten/flatten.hpp"
#include "logic/hints.hpp"
#include "logic/plandomizer.hpp"
#include "logic/search.hpp"
#include "logic/spoiler_log.hpp"
#include "logic/world.hpp"
#include "seedgen/config.hpp"
#include "seedgen/settings.hpp"
#include "utility/time.hpp"

#include <iostream>

#ifndef RANDOMIZER_ONLY
#include "../src/paths.hpp"
#include "../src/randomizer_context.hpp"
#endif

namespace randomizer
{
    logic::world::World* Randomizer::GetWorld(int worldId /*= 1*/) {
        auto worldIndex = worldId - 1;
        if (worldIndex < this->_worlds.size()) {
            return this->_worlds.at(worldIndex).get();
        }

        return nullptr;
    }

    std::optional<std::string> Randomizer::Generate()
    {
        try
        {
            GenerateWorlds();
        }
        catch(const std::exception& e)
        {
            std::cout << "============================================================" << std::endl;
            std::cout << "The following exception occurred: " << e.what() << std::endl;
            return e.what();
        }

        return std::nullopt;
    }

#ifndef RANDOMIZER_ONLY
    void Randomizer::GenerateTrackerWorld() {
        auto contextHash = randomizer_GetContext().mHash;

        if (contextHash.empty()) {
            return;
        }

        std::filesystem::path seedSettings = ::randomizer::paths::GetRandomizerSeedsPath() /
            contextHash / (contextHash + " Anti-Spoiler Log.txt");

        this->_config.LoadFromFile(seedSettings, GetPrefPath());
        this->_config.SetHash(contextHash);

        std::unique_ptr<logic::world::World> world = std::make_unique<logic::world::World>(1, this);
        world->SetSettings(this->_config.GetSettingsList().front());
        // Always use logic when building a tracker world
        world->Setting("Logic Rules").SetCurrentOption("All Locations Reachable");
        world->Build();
        this->_worlds.emplace_back(std::move(world));

        auto trackerWorld = this->_worlds.at(0).get();
        trackerWorld->SetNonProgressLocations();
        trackerWorld->SetTrackerNonProgressLocations();
        trackerWorld->AssignAreaProperties();
        trackerWorld->AssignGoalLocations();

        // Cache exit form times. This *must* run before conducting the flattening search, otherwise
        // the flattening search will pollute the exit timeform cache with a bunch of zeros
        logic::fill::CacheExitTimeForms(this->_worlds);

        // Set raw requirements for each location
        FlattenSearch search = FlattenSearch(trackerWorld);
        search.doSearch();
    }
#endif
    void Randomizer::GenerateWorlds()
    {
        utility::time::ScopedTimer<"Seed generation took ", std::chrono::milliseconds> timer;
        this->_config.LoadFromFile(GetConfigPath(), GetPrefPath());
        // Set permalink now so that resolving random settings doesn't change it
        this->_config.SetPermalink(this->_config.GetPermalink());

        utility::platform::Log(std::string("Seed: ") + this->_config.GetSeed());

        seedgen::config::SeedRNG(this->_config, true, false);
        // Set the hash now before anything else random is decided. This allows us to show the hash for a seed
        // before generating it later
        auto hash = this->_config.GetHash();
        utility::platform::Log(std::string("Hash: ") + hash);

        // Build all worlds
        int worldId = 1;
        for (const auto& settings : this->_config.GetSettingsList())
        {
            std::unique_ptr<logic::world::World> world = std::make_unique<logic::world::World>(worldId++, this);
            world->SetSettings(settings);
            world->ResolveRandomSettings();
            world->ResolveConflictingSettings();
            world->Build();
            this->_worlds.emplace_back(std::move(world));
        }

        // Process Plando Data for all worlds
        if (this->_config.IsUsingPlandomizer())
        {
            try {
                logic::plandomizer::LoadPlandomizerData(this->_worlds, this->_config.GetPlandomizerPath());
            } catch (const std::runtime_error& e) {
                throw std::runtime_error("Plandomizer Error: " + std::string(e.what()));
            }
        }

        // Pre Entrance Shuffle Tasks
        for (auto& world : this->_worlds)
        {
            world->PerformPreEntranceShuffleTasks();
        }

        utility::platform::Log("Shuffling Entrances...");
        for (auto& world : this->_worlds)
        {
            logic::entrance_shuffle::ShuffleWorldEntrances(world.get());
        }

        // Post Entrance Shuffle Tasks
        for (auto& world : this->_worlds)
        {
            world->PerformPostEntranceShuffleTasks();
        }
        logic::fill::CacheExitTimeForms(this->_worlds);

        // Flattens down the requirements for each location and entrance into a single statement.
        // This is used for calculating hint importance.
        utility::platform::Log("Flattening...");
        FlattenSearch search = FlattenSearch(this->_worlds.at(0).get());
        search.doSearch();

        // Set chain locations once the flatten search is done
        for (auto& world : this->_worlds) {
            for (auto& location : world->GetAllLocations()) {
                for (auto& item : location->GetComputedRequirement().getItems(world.get())) {
                    item->AddChainLocation(location);
                }
            }
        }

        utility::platform::Log("Filling Worlds...");
        logic::fill::FillWorlds(this->_worlds);

        // Post Fill Tasks
        for (auto& world : this->_worlds)
        {
            world->PerformPostFillTasks();
        }

        // Generate Playthrough
        logic::search::GeneratePlaythrough(this);

        // Generate Hints
        logic::hints::GenerateAllHints(this->_worlds);

        // Write Logs
        if (this->_config.IsGeneratingSpoilerLog())
        {
            logic::spoiler_log::GenerateSpoilerLog(this);
        }
        logic::spoiler_log::GenerateAntiSpoilerLog(this);
    }

    std::filesystem::path Randomizer::GetSeedOutputPath()
    {
        return this->_baseOutputPath / "seeds" / this->_config.GetHash();
    }
} // namespace randomizer
