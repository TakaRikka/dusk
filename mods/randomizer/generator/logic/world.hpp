#pragma once

#include "area.hpp"
#include "dungeon.hpp"
#include "item.hpp"
#include "item_pool.hpp"
#include "location.hpp"
#include "requirement.hpp"

#include "../seedgen/settings.hpp"
#include "../utility/log.hpp"
#include "../utility/text.hpp"

#include <unordered_map>
#include <map>
#include <vector>
#include <memory>

// Forward Declarations
namespace randomizer
{
    class Randomizer;
}

namespace randomizer::logic::search
{
    class Search;
}

namespace randomizer::logic::world
{
    class World;
    using WorldPool = std::vector<std::unique_ptr<World>>;

    class World
    {
       public:
        World(const int& id, Randomizer* randomizer);

        int GetID() const;
        void SetSettings(const seedgen::settings::Settings& settings);
        const seedgen::settings::Settings& GetSettings() const;
        void SetRandomizer(Randomizer* randomizer);
        Randomizer* GetRandomizer() const;

        /**
         * @brief Resolves all remaining random settings within a specific world
         */
        void ResolveRandomSettings();
        
        /**
         * @brief Resolves settings that conflict with each other. Ideally will only resolve settings that conflict due to
         * having their current option randomly chosen.
         */
        void ResolveConflictingSettings();
        void Build();
        void BuildItemTable();
        void BuildLocationTable();
        void LoadLogicMacros();
        void LoadWorldGraph();
        bool EvaluateSettingCondition(const std::string& condition);

        /**
         * @brief Generate the main item pool and starting item pool for this world.
         */
        void GenerateItemPools();

        /**
         * @brief Decides if a location should be removed depending on settings.
         * @param locationName The name of the location
         * @param originalItemName The name of the original item at the location
         *
         * @return true if the location should be removed. False otherwise
         */
        bool ShouldRemoveLocation(const std::string& locationName, const std::string& originalItemName);

        /**
         * @brief Perform all tasks which must be complete before shuffling entrances.
         */
        void PerformPreEntranceShuffleTasks();
        void PlaceVanillaItems();
        void PlacePlandomizerItems();
        void SetNonProgressLocations();
        void SetTrackerNonProgressLocations();

        /**
         * @brief Perform all tasks which require shuffled entrances to be set, but before running the main item placement
         * algorithm.
         */
        void PerformPostEntranceShuffleTasks();
        void AssignAreaProperties();
        void AssignGoalLocations();

        /**
         * @brief Forbid items from being in certain locations depending on settings
         */
        void SetForbiddenItems();

        /**
         *  @brief STUB: Would choose required dungeons ahead of placing any non-vanilla and non-plandomized items. Not really
         *  required unless we let users choose a specific amount of directly required dungeons
         */
        void DetermineDungeonDependentLocations();

        bool IsNoLogicRequiredDungeon(const std::unique_ptr<dungeon::Dungeon>& dungeon);

        /**
         *  @brief Determines which dungeons are required based on placed items. Sets required dungeons as such in their
         *  properties. If "Unrequired Dungeons Are Barren" is "On", then unrequired dungeons will have all their locations
         *  progression status set to false.
         */
        void DetermineRequiredDungeons();

        /**
         *  @brief Adds junk to the main pool until the number of items in the pool matches the total number of
         * currently empty locations.
         */
        void SanitizeItemPool();
        void SetSearchStartingProperties(search::Search* search) const;
        void PerformPostFillTasks();
        void FinalizeBottleContents();
        void AddPlandomizedLocation(location::Location* location, item::Item* item);
        void AddPlandomizedEntrance(entrance::Entrance* entrance, entrance::Entrance* target);
        std::unordered_map<entrance::Entrance*, entrance::Entrance*> GetPlandomizerEntrances();

        dungeon::Dungeon* GetDungeon(const std::string& name);
        const std::map<std::string, std::unique_ptr<dungeon::Dungeon>>& GetDungeonTable() const;
        item::Item* GetItem(const std::string& name, const bool& ignoreError = false);
        item::Item* GetItem(uint16_t id, const bool& ignoreError = false);
        item::Item* GetShadowCrystal();
        item::Item* GetGameWinningItem() const;
        item_pool::ItemPool& GetItemPool();
        item_pool::ItemPool& GetStartingItemPool();
        location::Location* GetLocation(const std::string& name);
        location::LocationPool GetAllLocations(const bool& includeNonItemLocations = false);
        area::Area* GetArea(const std::string& name, const bool& createIfNotFound = false);
        area::Area* GetRootArea() const;
        const std::map<std::string, std::unique_ptr<area::Area>>& GetAreaTable() const;
        entrance::Entrance* GetEntrance(const std::string& originalName);
        int GetNewEntranceID();
        entrance::EntrancePool GetShuffleableEntrances(const entrance::Type& type,
                                                       bool onlyPrimary = false);
        entrance::EntrancePool GetShuffledEntrances(
            const entrance::Type& type = entrance::Type::ALL,
            bool onlyPrimary = false);
        std::unordered_map<entrance::Entrance*, int>& GetExitTimeFormCache();

        int GetMacroIndex(const std::string& macroName) const;
        const requirement::Requirement& GetMacro(const int& macroIndex);
        int GetEventIndex(const std::string& eventName, bool addIfNone = true);
        std::string GetEventName(const int& eventIndex);

        seedgen::settings::Setting& Setting(const std::string& settingName);
        bool AnyEntranceRandomizerEnabled();

        TextDatabase& GetTextDatabase() { return this->_textDatabase; }
        const std::string& GetText(const std::string& name, Text::Type type = Text::STANDARD, Text::Language language = Text::ENGLISH) {
            if (!this->_textDatabase.at(name).at(type).mText.at(language).empty()) {
                return this->_textDatabase.at(name).at(type).mText.at(language);
            }

            return this->_textDatabase.at(name).at(type).mText.at(Text::ENGLISH);
        }
        // Make a new custom text entry for this world specifically and return a reference to it
        Text& AddNewText(const std::string& name, Text::Type type = Text::STANDARD) {
            return this->_textDatabase[name][type];
        }

       private:
        int _id = -1;
        int _entranceIdCounter = 0;

        seedgen::settings::Settings _settings;
        std::map<std::string, std::unique_ptr<item::Item>> _itemTable = {};
        std::map<int, item::Item*> _itemIdTable = {};
        std::map<std::string, std::unique_ptr<location::Location>> _locationTable = {};
        std::unordered_set<std::string> _intentionallyRemovedLocations = {};
        std::unordered_set<std::string> _registeredLocationCategories = {};
        std::map<std::string, std::unique_ptr<area::Area>> _areaTable = {};
        std::map<std::string, std::unique_ptr<dungeon::Dungeon>> _dungeons = {};
        std::map<int, requirement::Requirement> _macros = {};
        std::unordered_map<std::string, int> _macroIndexes = {};
        std::unordered_map<std::string, int> _eventIndexes = {};
        std::unordered_map<int, std::string> _eventNames = {};
        item_pool::ItemPool _itemPool = {};
        item_pool::ItemPool _startingItemPool = {};
        std::unordered_map<entrance::Entrance*, int> _exitTimeFormCache = {};
        // Custom text for this world specifically
        TextDatabase _textDatabase = {};

        // Plandomizer Data
        std::unordered_map<location::Location*, item::Item*> _plandomizerLocations = {};
        std::unordered_map<entrance::Entrance*, entrance::Entrance*> _plandomizerEntrances = {};

        Randomizer* _randomizer = nullptr;
    };
} // namespace randomizer::logic::world
