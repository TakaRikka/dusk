#pragma once

#include "../utility/container.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <list>
#include <set>
#include <memory>
#include <stdexcept>

namespace randomizer::seedgen::settings
{
    class SettingInfo;
    using SettingInfoMap_t = std::map<std::string, std::unique_ptr<SettingInfo>>;

    /**
     *  @brief Enum for different types of settings.
     *
     *   Standard settings will affect the rng used for seed generation.
     *   Preference settings will not affect the rng used for seed generation.
     */
    enum Type
    {
        INVALID = 0,
        STANDARD,
        PREFERENCE,
    };

    /**
     *  @brief Takes a string representation of a Type and returns the
     *  associated enum value.
     *
     *  @param str The string representation of a Type.
     *  @return The associated enum value for the passed in type.
     */
    Type TypeFromStr(const std::string& str);

    /**
     *  @brief SettingInfo holds static info about a setting.
     *
     *  Data should only ever be set once when reading from settings.yaml
     *  and creating appropriate info entries.
     *
     */
    class SettingInfo
    {
       public:
        SettingInfo(int id,
                    const std::string& name,
                    Type type,
                    const std::vector<std::string>& options,
                    const std::vector<std::string>& descriptions,
                    int defaultOptionIndex,
                    bool hasRandomOption,
                    int randomOptionIndex,
                    int randomLow,
                    int randomHigh,
                    bool trackerImportant,
                    bool needInGame);

        int GetID() const { return this->_id; }

        /**
         *  @brief Returns the setting's name as displayed in a UI.
         */
        std::string GetName() const { return this->_name; }

        /**
         *  @brief Returns the type of the setting.
         */
        Type GetType() const { return this->_type; }

        /**
         *  @brief Returns a vector of strings of the setting's available options.
         */
        const std::vector<std::string>& GetOptions() const { return this->_options; }

        /**
         *  @brief Returns a vector of strings of the setting's options' descriptions.
         */
        const std::vector<std::string>& GetDescriptions() const { return this->_descriptions; }

        /**
         *  @brief Returns the index of the default option in the options vector for the setting.
         */
        int GetDefaultOptionIndex() const { return this->_defaultOptionIndex; }

        /**
         *  @brief Returns the string representation of the default option for the setting.
         */
        std::string GetDefaultOption() const;
        int GetIndexOfOption(const std::string& option) const;
        bool HasRandomOption() const { return this->_hasRandomOption; }
        int GetRandomOptionIndex() const { return this->_randomOptionIndex; }
        std::string GetRandomOption() const;
        int GetRandomLow() const { return this->_randomLow; }
        int GetRandomHigh() const { return this->_randomHigh; }
        bool TrackerImportant() const { return this->_trackerImportant; }
        bool NeedInGame() const { return this->_needInGame; }
        bool OptionsAreNumbers() const { return this->_optionsAreNumbers; }
        int GetOptionsBitLength() const {return this->_optionsBitLength; }

       private:
        int _id = -1;
        std::string _name = "";
        Type _type = INVALID;
        std::vector<std::string> _options = {};
        std::vector<std::string> _descriptions = {};
        int _defaultOptionIndex = 0;
        bool _hasRandomOption = true;
        int _randomOptionIndex = 0;     // The index of this setting's random option
        int _randomLow = 0;             // Lower bound when choosing a random option
        int _randomHigh = 0;            // Upper bound when choosing a random option
        bool _trackerImportant = false; // Whether or not this setting can affect trackers
        bool _needInGame = false;       // Whether or not we need to read this setting during gameplay
        bool _optionsAreNumbers = false;// Whether this setting's options are all numbers
        int _optionsBitLength = 0;

        // Variables that hold the setting's name and options when being checked
        // in a logical requirement string.
        std::string _logicName = "";
        std::vector<std::string> _logicOptions = {};
    };

    /**
     *  @brief Setting holds the data for a single setting in a specific world.
     *
     *  A setting's current option index can change depending on certain
     *  circumstances (i.e. the user changes it, or it conflicts with another setting)
     */
    class Setting
    {
       public:
        Setting(SettingInfo* info, const std::string& option);

        void SetCurrentOption(const std::string& newOption);
        void SetCurrentOption(int newOptionIndex);
        std::string GetCurrentOption() const;
        int GetCurrentOptionAsNumber() const;
        int GetCurrentOptionIndex() const { return this->_currentOptionIndex; }
        bool IsUsingRandomOption() const { return this->_isUsingRandomOption; }
        SettingInfo* GetInfo() const { return this->_info; }
        const std::string& GetCustomOption() const { return this->_customOption; }
        void ResolveIfRandom();

        bool operator==(const char* optionName) const;
        bool operator!=(const char* optionName) const;
        bool operator>=(const char* optionName) const;
        bool operator<=(const char* optionName) const;
        bool operator>(const char* optionName) const;
        bool operator<(const char* optionName) const;

        template<class... Types>
        bool IsAnyOf(Types... optionNames)
        {
            // Check to make sure all listed options exist
            for (const auto& optionName : {optionNames...})
            {
                if (!utility::container::ElementInContainer(this->GetInfo()->GetOptions(), optionName))
                {
                    throw std::runtime_error("\"" + std::string(optionName) + "\" is not a known option for setting \"" +
                                             this->GetInfo()->GetName() + "\"");
                }
            }

            // Check if any of the options are the current one
            for (const auto& optionName : {optionNames...})
            {
                if (optionName == this->GetCurrentOption())
                {
                    return true;
                }
            }
            return false;
        }

       private:
        int _currentOptionIndex = -1;
        bool _isUsingRandomOption = false;
        SettingInfo* _info = nullptr;
        std::string _customOption = ""; // For things like hex color strings
    };

    /**
     *  @brief Settings holds all of the settings for a specific world.
     *
     */
    class Settings
    {
       public:
        Settings();

        void InsertSetting(const std::string& settingName, Setting setting);
        void AddStartingItem(const std::string& itemName, const int& count = 1);
        void AddExcludedLocation(const std::string& locationName);
        void AddMixedPool(const std::list<std::string>& pool);
        std::map<std::string, Setting>& GetMap() { return this->_map; }
        const std::map<std::string, int>& GetStartingInventory() const { return this->_startingInventory; }
        const std::set<std::string>& GetExcludedLocations() const { return this->_excludedLocations; }
        const std::list<std::list<std::string>>& GetMixedEntrancePools() const { return this->_mixedEntrancePools; }
        std::map<std::string, int>& GetModifiableStartingInventory() { return this->_startingInventory; }
        std::set<std::string>& GetModifiableExcludedLocations() { return this->_excludedLocations; }
        std::list<std::list<std::string>>& GetModifiableMixedEntrancePools() { return this->_mixedEntrancePools; }

       private:
        std::map<std::string, Setting> _map = {};
        std::map<std::string, int> _startingInventory = {};
        std::set<std::string> _excludedLocations = {};
        std::list<std::list<std::string>> _mixedEntrancePools = {};
    };

    /**
     *  @brief Gets the map of each setting name to its info
     *  @return pointer to the setting info map
     */
    SettingInfoMap_t* GetAllSettingsInfo();

    /**
     *  @brief Reads settings_list.yaml and loads in all setting data
     */
    std::unique_ptr<SettingInfoMap_t> LoadAllSettingsInfo();

    /**
     * @brief Generates a hash based on the current settings info. This can be used
     * to determine if settings between different permalinks are valid
     * @return the hash for the current settings info
     */
    uint32_t GetSettingInfoHash();

}; // namespace randomizer::seedgen::settings
