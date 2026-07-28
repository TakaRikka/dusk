#pragma once

#include "logic/world.hpp"

#include <optional>

namespace randomizer
{
    class Randomizer
    {
    public:
        Randomizer() = delete;
        Randomizer(const std::filesystem::path& baseOutputPath) : _baseOutputPath(baseOutputPath) {}

        /**
         *  @brief Generates a complete randomizer seed
         *
         *  @return a std::optional<std::string> containing a message if there was an error.
         */
        std::optional<std::string> Generate();
        void GenerateWorlds();
        void GenerateTrackerWorld();

        auto& GetConfig() { return this->_config; }
        auto& GetWorlds() { return this->_worlds; }
        /**
         * @param worldId
         * @return The world with the specified Id. If no Id is specified, the first world will
         * be returned. If a world with the Id does not exist, a nullptr will be returned.
         */
        logic::world::World* GetWorld(int worldId = 1);

        int GetNewEventID() { return ++(this->_eventIdCounter); }
        int GetNewAreaID() { return ++(this->_areaIdCounter); }
        int GetNewLocAccID() { return ++(this->_locAccIdCounter); }

        auto& GetPlaythroughSpheres() { return this->_playthroughSpheres; }
        auto& GetEntranceSpheres() { return this->_entranceSpheres; }

        std::filesystem::path GetSeedOutputPath();
        std::filesystem::path GetBaseOutputPath() const { return this->_baseOutputPath; };
        void SetBaseOutputPath(const std::filesystem::path& path) { this->_baseOutputPath = path; };

        std::filesystem::path GetConfigPath() const { return this->GetBaseOutputPath() / "settings.yaml"; }
        std::filesystem::path GetPrefPath() const { return this->GetBaseOutputPath() / "preferences.yaml"; }
    private:
        seedgen::config::Config _config{};
        logic::world::WorldPool _worlds{};

        int _eventIdCounter{};
        int _areaIdCounter{};
        int _locAccIdCounter{};

        // Playthrough data
        std::list<std::list<logic::location::Location*>> _playthroughSpheres{};
        std::list<std::list<logic::entrance::Entrance*>> _entranceSpheres{};

        std::filesystem::path _baseOutputPath{};
    };
} // namespace randomizer
