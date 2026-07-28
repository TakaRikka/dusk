#include "settings.hpp"

#include "../utility/crc32.hpp"
#include "../utility/endian.hpp"
#include "../utility/log.hpp"
#include "../utility/container.hpp"
#include "../utility/file.hpp"
#include "../utility/random.hpp"
#include "../utility/string.hpp"
#include "../utility/yaml.hpp"
#include "../logic/location.hpp"
#include "../logic/entrance_shuffle.hpp"

#include <unordered_map>
#include <algorithm>
#include <memory>

namespace randomizer::seedgen::settings
{

    Type TypeFromStr(const std::string& str)
    {
        std::unordered_map<std::string, Type> types = {{"Standard", Type::STANDARD}, {"Preference", Type::PREFERENCE}};

        if (!types.contains(str))
        {
            return Type::INVALID;
        }

        return types.at(str);
    }

    SettingInfo::SettingInfo(int id,
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
                             bool needInGame):
        _id(id),
        _name(name),
        _type(type),
        _options(options),
        _descriptions(descriptions),
        _defaultOptionIndex(defaultOptionIndex),
        _hasRandomOption(hasRandomOption),
        _randomOptionIndex(randomOptionIndex),
        _randomLow(randomLow),
        _randomHigh(randomHigh),
        _trackerImportant(trackerImportant),
        _needInGame(needInGame)
    {
        // The logic expression of a setting replaces spaces with underscores,
        // and removes apostraphes and parenthesis
        auto logicName = name;
        std::ranges::replace(logicName, ' ', '_');
        utility::str::Erase(logicName, "'", ")", "(");
        this->_logicName = logicName;

        // Same for logic expressions of options for this setting
        for (const auto& option : options)
        {
            auto logicOption = option;
            std::ranges::replace(logicOption, ' ', '_');
            utility::str::Erase(logicOption, "'", ")", "(");
            this->_logicOptions.push_back(logicOption);
        }

        // Note: Assumes an option is never a negative number
        this->_optionsAreNumbers = std::ranges::all_of(options, [](const std::string& option) {
            return !option.empty() && std::ranges::all_of(option, ::isdigit);
        });

        // Set options bitlength
        this->_optionsBitLength = std::bit_width(this->_options.size());
    }

    std::string SettingInfo::GetDefaultOption() const
    {
        return this->_options[this->_defaultOptionIndex];
    }

    int SettingInfo::GetIndexOfOption(const std::string& option) const
    {
        return utility::container::GetIndex(this->_options, option);
    }

    std::string SettingInfo::GetRandomOption() const
    {
        return this->_options.at(this->_randomOptionIndex);
    }

    Setting::Setting(SettingInfo* info, const std::string& option): _info(info)
    {
        this->_currentOptionIndex = info->GetIndexOfOption(option);
    }

    void Setting::SetCurrentOption(int newOptionIndex)
    {
        if (newOptionIndex >= this->GetInfo()->GetOptions().size())
        {
            throw std::runtime_error(std::string("Index ") + std::to_string(newOptionIndex) +
                                     " is out of bounds for setting \"" + this->GetInfo()->GetName() + "\"");
        }
        this->_currentOptionIndex = newOptionIndex;
    }

    void Setting::SetCurrentOption(const std::string& optionName)
    {
        int optionNameIndex = this->GetInfo()->GetIndexOfOption(optionName);
        if (optionNameIndex == -1)
        {
            throw std::runtime_error(std::string("\"") + optionName + "\" is not a valid option for setting \"" +
                                     this->GetInfo()->GetName() + "\"");
        }
        this->SetCurrentOption(optionNameIndex);
    }

    std::string Setting::GetCurrentOption() const
    {
        return this->_info->GetOptions()[this->_currentOptionIndex];
    }

    int Setting::GetCurrentOptionAsNumber() const {
        try {
            return std::stoi(this->GetCurrentOption());
        } catch (const std::invalid_argument&) {
            throw std::runtime_error("Option \"" + GetCurrentOption() + "\" for setting \"" + this->GetInfo()->GetName() +
                                     "\" cannot be turned into a number");
        }
    }

    void Setting::ResolveIfRandom()
    {
        if (this->GetCurrentOptionIndex() == this->GetInfo()->GetRandomOptionIndex())
        {
            this->_isUsingRandomOption = true;
            auto randomOption =
                utility::random::Random(this->GetInfo()->GetRandomLow(), this->GetInfo()->GetRandomHigh());
            this->SetCurrentOption(randomOption);
            LOG_TO_DEBUG("Chose \"" + this->GetInfo()->GetOptions()[randomOption] + " as random option for setting \"" +
                         this->GetInfo()->GetName());
        }
    }

    bool Setting::operator==(const char* optionName) const
    {
        int optionNameIndex = this->GetInfo()->GetIndexOfOption(optionName);
        if (optionNameIndex == -1)
        {
            throw std::runtime_error(std::string("\"") + optionName + "\" is not a valid option for setting \"" +
                                     this->GetInfo()->GetName() + "\"");
        }
        return this->_currentOptionIndex == optionNameIndex;
    }

    bool Setting::operator!=(const char* optionName) const
    {
        return !(*this == optionName);
    }

    bool Setting::operator>=(const char* optionName) const
    {
        int optionNameIndex = this->GetInfo()->GetIndexOfOption(optionName);
        if (optionNameIndex == -1)
        {
            throw std::runtime_error(std::string("\"") + optionName + "\" is not a valid option for setting \"" +
                                     this->GetInfo()->GetName() + "\"");
        }
        return this->_currentOptionIndex >= optionNameIndex;
    }

    bool Setting::operator<=(const char* optionName) const
    {
        int optionNameIndex = this->GetInfo()->GetIndexOfOption(optionName);
        if (optionNameIndex == -1)
        {
            throw std::runtime_error(std::string("\"") + optionName + "\" is not a valid option for setting \"" +
                                     this->GetInfo()->GetName() + "\"");
        }
        return this->_currentOptionIndex <= optionNameIndex;
    }

    bool Setting::operator>(const char* optionName) const
    {
        int optionNameIndex = this->GetInfo()->GetIndexOfOption(optionName);
        if (optionNameIndex == -1)
        {
            throw std::runtime_error(std::string("\"") + optionName + "\" is not a valid option for setting \"" +
                                     this->GetInfo()->GetName() + "\"");
        }
        return this->_currentOptionIndex > optionNameIndex;
    }

    bool Setting::operator<(const char* optionName) const
    {
        int optionNameIndex = this->GetInfo()->GetIndexOfOption(optionName);
        if (optionNameIndex == -1)
        {
            throw std::runtime_error(std::string("\"") + optionName + "\" is not a valid option for setting \"" +
                                     this->GetInfo()->GetName() + "\"");
        }
        return this->_currentOptionIndex < optionNameIndex;
    }

    Settings::Settings() {
        // Load settings info and set defaults
        auto settingInfoMap = GetAllSettingsInfo();
        for (auto& [settingName, settingInfo] : *settingInfoMap) {
            InsertSetting(settingName, Setting(settingInfo.get(), settingInfo->GetDefaultOption()));
        }
    }

    void Settings::InsertSetting(const std::string& settingName, Setting setting)
    {
        this->_map.emplace(settingName, setting);
    }

    void Settings::AddStartingItem(const std::string& itemName, const int& count /*= 1*/)
    {
        if (!this->_startingInventory.contains(itemName))
        {
            this->_startingInventory.emplace(itemName, 0);
        }
        this->_startingInventory.at(itemName) += count;
    }

    void Settings::AddExcludedLocation(const std::string& locationName)
    {
        this->_excludedLocations.insert(locationName);
    }

    void Settings::AddMixedPool(const std::list<std::string>& pool)
    {
        this->_mixedEntrancePools.push_back(pool);
    }

    SettingInfoMap_t* GetAllSettingsInfo()
    {
        static std::unique_ptr<SettingInfoMap_t> settingInfoMap = std::make_unique<SettingInfoMap_t>();

        // If we haven't loaded in our setting info yet, do so now
        if (settingInfoMap->empty())
        {
            settingInfoMap = LoadAllSettingsInfo();
        }

        return settingInfoMap.get();
    }

    std::unique_ptr<SettingInfoMap_t> LoadAllSettingsInfo()
    {
        std::unique_ptr<SettingInfoMap_t> settingInfoMap = std::make_unique<SettingInfoMap_t>();
        auto settingsDataTree = LOAD_EMBED_YAML(RANDO_DATA_PATH "settings_list.yaml");

        // Process all nodes of the yaml file. Each node contains one setting
        int settingIdCounter = 0;
        for (const auto& settingNode : settingsDataTree)
        {
            // Check to make sure all required fields are present
            const auto requiredFields = {"Name", "Default Option", "Options"};
            for (const auto& field : requiredFields)
            {
                if (!settingNode[field])
                {
                    throw std::runtime_error(std::string("Field \"") + field + "\" is missing from settings list node:\n" +
                                             YAML::Dump(settingNode));
                }
            }

            // Required Fields
            const auto& name = settingNode["Name"].as<std::string>();
            const auto& defaultOption = settingNode["Default Option"].as<std::string>();
            std::vector<std::string> options = {};
            std::vector<std::string> descriptions = {};
            for (const auto& optionNodes : settingNode["Options"])
            {
                for (const auto& optionNode : optionNodes)
                {
                    const auto& option = optionNode.first.as<std::string>();
                    const auto& description = optionNode.second.as<std::string>();

                    // If we're specifying a range, then include all numbers in the range
                    if (randomizer::utility::str::Contains(option, "-"))
                    {
                        // Fill in all the options between the lower and upper bounds
                        auto ops = randomizer::utility::str::Split(option, '-');
                        int lowerBound = std::stoi(ops[0]);
                        int upperBound = std::stoi(ops[1]);
                        for (auto i = lowerBound; i <= upperBound; i++)
                        {
                            options.push_back(std::to_string(i));
                            descriptions.push_back(description);
                        }
                    }
                    else
                    {
                        options.push_back(option);
                        descriptions.push_back(description);
                    }
                }
            }

            // Calculate default option index
            auto defaultOptionIndex = utility::container::GetIndex(options, defaultOption);
            if (defaultOptionIndex == -1)
            {
                throw std::runtime_error(std::string("Default Option \"") + defaultOption + "\" is not defined for setting \"" +
                                         name + "\"");
            }

            // Optional fields. If found, use the field value. If not found, use a default
            const auto& type = TypeFromStr(settingNode["Type"] ? settingNode["Type"].as<std::string>() : "Standard");
            if (type == Type::INVALID)
            {
                throw std::runtime_error(std::string("Unknown setting type \"") + settingNode["Type"].as<std::string>() +
                                         "\" for setting \"" + name + "\"");
            }

            const auto& trackerImportant =
                settingNode["Tracker Important"] ? settingNode["Tracker Important"].as<bool>() : false;
            const auto& needInGame =
                settingNode["Need In Game"] ? settingNode["Need In Game"].as<bool>() : false;
            const auto& hasRandomOption =
                settingNode["Autogenerate Random"] ? settingNode["Autogenerate Random"].as<bool>() : true;
            const auto& randomAlias = settingNode["Random Alias"] ? settingNode["Random Alias"].as<std::string>() : "Random";

            int randomLow = 0;
            int randomHigh = options.size() - 1;
            if (settingNode["Random Low"])
            {
                auto randomLowStr = settingNode["Random Low"].as<std::string>();
                randomLow = utility::container::GetIndex(options, randomLowStr);
                if (randomLow == -1)
                {
                    throw std::runtime_error(std::string("Random Low Option \"") + randomLowStr +
                                             "\" is not defined for setting \"" + name + "\"");
                }
            }
            if (settingNode["Random High"])
            {
                auto randomHighStr = settingNode["Random High"].as<std::string>();
                randomHigh = utility::container::GetIndex(options, randomHighStr);
                if (randomHigh == -1)
                {
                    throw std::runtime_error(std::string("Random High Option \"") + randomHighStr +
                                             "\" is not defined for setting \"" + name + "\"");
                }
            }

            // Generate the random option if it's not already there
            if (hasRandomOption && utility::container::GetIndex(options, randomAlias) != -1)
            {
                options.push_back(randomAlias);
                descriptions.push_back("A random option will be chosen");
            }

            int randomOptionIndex = utility::container::GetIndex(options, randomAlias);

            // Insert the data for the setting
            auto info = std::make_unique<SettingInfo>(settingIdCounter++,
                                                      name,
                                                      type,
                                                      options,
                                                      descriptions,
                                                      defaultOptionIndex,
                                                      hasRandomOption,
                                                      randomOptionIndex,
                                                      randomLow,
                                                      randomHigh,
                                                      trackerImportant,
                                                      needInGame);
            settingInfoMap->emplace(name, std::move(info));
        }

        return std::move(settingInfoMap);
    }

    uint32_t GetSettingInfoHash() {
        static uint32_t settingsInfoHash = 0;
        if (settingsInfoHash == 0) {
            auto allSettingInfo = GetAllSettingsInfo();
            for (const auto& [settingName, settingInfo] : *allSettingInfo) {
                settingsInfoHash = utility::crc32(settingName.data(), settingName.length(), settingsInfoHash);
                for (const auto& optionName : settingInfo->GetOptions()) {
                    settingsInfoHash = utility::crc32(optionName.data(), optionName.length(), settingsInfoHash);
                }
            }

            for (const auto& [itemName, maxRef] : logic::item_pool::GetValidStartingInventoryItems()) {
                int maxCount = maxRef;
                settingsInfoHash = utility::crc32(itemName.data(), itemName.length(), settingsInfoHash);
                if constexpr (std::endian::native == std::endian::big) {
                    maxCount = Utility::Endian::byteswap(maxCount);
                }
                settingsInfoHash = utility::crc32(&maxCount, sizeof(maxCount), settingsInfoHash);
            }

            for (const auto& locationName : logic::location::GetAllRandomizerLocationNames()) {
                settingsInfoHash = utility::crc32(locationName.data(), locationName.length(), settingsInfoHash);
            }

            for (const auto& entranceType : logic::entrance_shuffle::GetPossibleMixedPoolTypes()) {
                settingsInfoHash = utility::crc32(entranceType.data(), entranceType.length(), settingsInfoHash);
            }
        }
        return settingsInfoHash;
    }

}; // namespace randomizer::seedgen::settings
