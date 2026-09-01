#include "config.hpp"

#include "packed_bits.hpp"
#include "seed.hpp"
#include "../utility/base64pp.hpp"
#include "../utility/crc32.hpp"
#include "../utility/log.hpp"
#include "../utility/platform.hpp"
#include "../utility/random.hpp"
#include "../utility/yaml.hpp"
#include "../logic/entrance_shuffle.hpp"

#include <fmt/format.h>
#include <iostream>

// Fields which aren't part of settings_list.yaml
constexpr std::string_view SEED = "Seed";
constexpr std::string_view PLANDOMIZER = "Plandomizer";
constexpr std::string_view PLANDOMIZER_PATH = "Plandomizer Path";
constexpr std::string_view STARTING_INVENTORY = "Starting Inventory";
constexpr std::string_view EXCLUDED_LOCATIONS = "Excluded Locations";
constexpr std::string_view MIXED_ENTRANCE_POOLS = "Mixed Entrance Pools";
constexpr std::string_view GENERATE_SPOILER_LOG = "Generate Spoiler Log";

namespace randomizer::seedgen::config
{
    Config::Config() {
        // Create at least one player's settings
        this->_settingsList.push_front(settings::Settings());
    }

    Config::Config(const fspath& settingsPath, const fspath& preferencesPath) {
        // Create at least one player's settings
        this->_settingsList.push_front(settings::Settings());
        LoadFromFile(settingsPath, preferencesPath);
    }

    void Config::ResetSettingsToDefault() {
        for (auto& settings : this->_settingsList) {
            for (auto& [settingName, setting] : settings.GetMap()) {
                if (setting.GetInfo()->GetType() == settings::Type::STANDARD) {
                    setting.SetCurrentOption(setting.GetInfo()->GetDefaultOption());
                }
            }
            settings.GetModifiableExcludedLocations().clear();
            settings.GetModifiableStartingInventory().clear();
            settings.GetModifiableMixedEntrancePools().clear();
        }
    }

    void Config::ResetPreferencesToDefault() {
        for (auto& settings : this->_settingsList) {
            for (auto& [settingName, setting] : settings.GetMap()) {
                if (setting.GetInfo()->GetType() == settings::Type::PREFERENCE) {
                    setting.SetCurrentOption(setting.GetInfo()->GetDefaultOption());
                }
            }
            this->_plandomizerPath = "";
        }
    }

    void Config::LoadFromFile(const fspath& settingsPath,
                              const fspath& preferencesPath,
                              bool createIfNotFound /*= true*/,
                              bool allowRewrite /*= true*/)
    {
        // Create files for settings/preferences if they don't exist
        if (!std::filesystem::exists(settingsPath))
        {
            if (createIfNotFound)
            {
                WriteSettingsToFile(settingsPath);
            }
            else
            {
                throw std::runtime_error("Could not open settings file at \"" + settingsPath.generic_string() + "\"");
            }
        }

        if (!std::filesystem::exists(preferencesPath))
        {
            if (createIfNotFound)
            {
                WritePreferencesToFile(preferencesPath);
            }
            else
            {
                throw std::runtime_error("Could not open preferences file at \"" + preferencesPath.generic_string() + "\"");
            }
        }

        auto& settings = this->_settingsList.front();
        settings.GetModifiableExcludedLocations().clear();
        settings.GetModifiableMixedEntrancePools().clear();
        settings.GetModifiableStartingInventory().clear();

        // Load settings info
        auto settingInfoMap = settings::GetAllSettingsInfo();

        // Read in settings and preferences. If we have to change anything,
        // rewrite the appropriate file if allowed.
        bool rewriteSettings = false;
        auto settingsTree = LoadYAML(settingsPath);

        // Loop through all setting fields
        for (const auto& settingNode : settingsTree)
        {
            auto settingName = settingNode.first.as<std::string>();
            if (settings::UpdateOldSettingName(settingName)) {
                utility::platform::Log(std::string("Setting \"") + settingName +
                    "\" was updated from \"" + settingNode.first.as<std::string>() + "\"");
                rewriteSettings = true;
            }
            // Insert the setting if it's in the info map
            if (settingInfoMap->contains(settingName))
            {
                auto& settingInfo = settingInfoMap->at(settingName);
                auto settingOption = settingNode.second.as<std::string>();

                // If the option doesn't exist, revert to default and rewrite later if necessary
                if (settingInfo->GetIndexOfOption(settingOption) == -1)
                {
                    utility::platform::Log(std::string("Setting \"") + settingName + "\" has no option \"" +
                                                  settingOption + "\". Reverting to default \"" +
                                                  settingInfo->GetDefaultOption() + "\"");
                    settingOption = settingInfo->GetDefaultOption();
                    rewriteSettings = true;
                }

                settings.GetMap().at(settingName).SetCurrentOption(settingOption);
            }
            // Special handling for starting inventory
            else if (settingName == STARTING_INVENTORY)
            {
                for (const auto& inventoryNode : settingNode.second)
                {
                    const auto& itemName = inventoryNode.first.as<std::string>();
                    const auto& count = inventoryNode.second.as<int>();

                    settings.AddStartingItem(itemName, count);
                }
            }
            // Special Handling for Excluded Locations
            else if (settingName == EXCLUDED_LOCATIONS)
            {
                for (const auto& locationNode : settingNode.second)
                {
                    const auto& locationName = locationNode.as<std::string>();
                    settings.AddExcludedLocation(locationName);
                }
            }
            // Special Handling for Mixed Entrance Pools
            else if (settingName == MIXED_ENTRANCE_POOLS)
            {
                for (const auto& poolNode : settingNode.second)
                {
                    if (!poolNode.IsSequence())
                    {
                        throw std::runtime_error("Mixed Entrance Pools is not a nested sequence of strings");
                    }
                    settings.AddMixedPool(poolNode.as<std::list<std::string>>());
                }
            }
            // Special handling for Seed
            else if (settingName == SEED)
            {
                const auto& seed = settingNode.second.as<std::string>();
                this->_seed = seed;

                // If seed is empty string, generate a new one
                if (this->_seed.empty())
                {
                    this->_seed = seed::GenerateSeed();
                }
            }
            // Special handling for Plandomizer
            else if (settingName == PLANDOMIZER)
            {
                const auto& plandomizer = settingNode.second.as<bool>(false);
                this->_isUsingPlandomizer = plandomizer;
            }
        }

        // Loop through all preference fields
        bool rewritePreferences = false;
        auto preferencesTree = LoadYAML(preferencesPath);
        for (const auto& preferenceNode : preferencesTree)
        {
            auto preferenceName = preferenceNode.first.as<std::string>();
            if (settings::UpdateOldSettingName(preferenceName)) {
                utility::platform::Log(std::string("Preference \"") + preferenceName +
                    "\" was updated from \"" + preferenceNode.first.as<std::string>() + "\"");
                rewritePreferences = true;
            }
            // Insert the preference if it's in the info map
            if (settingInfoMap->contains(preferenceName))
            {
                auto& preferenceInfo = settingInfoMap->at(preferenceName);
                auto preferenceOption = preferenceNode.second.as<std::string>();

                // If the option doesn't exist, revert to default and rewrite later if necessary
                if (preferenceInfo->GetIndexOfOption(preferenceOption) == -1)
                {
                    utility::platform::Log(std::string("Preference \"") + preferenceName + " has no option \"" +
                                                  preferenceOption + "\". Reverting to default \"" +
                                                  preferenceInfo->GetDefaultOption() + "\"");
                    preferenceOption = preferenceInfo->GetDefaultOption();
                    rewritePreferences = true;
                }

                settings.GetMap().at(preferenceName).SetCurrentOption(preferenceOption);
            }
            else if (preferenceName == PLANDOMIZER_PATH)
            {
                const auto& plandomizerPath = preferenceNode.second.as<std::string>();
                this->_plandomizerPath = plandomizerPath;
            }
        }

        // Rewrite the file(s) if any settings or preferences are missing
        for (auto& [settingName, settingInfo] : *settingInfoMap)
        {
            if (!settingsTree[settingName])
            {
                utility::platform::Log(std::string("Added missing setting \"") + settingName + "\"");
                if (settingInfo->GetType() == settings::Type::STANDARD)
                {
                    rewriteSettings = true;
                }
                else if (settingInfo->GetType() == settings::Type::PREFERENCE)
                {
                    rewritePreferences = true;
                }
            }
        }
        if (!settingsTree[SEED])
        {
            this->_seed = seed::GenerateSeed();
            utility::platform::Log("Seed is missing. Generated new seed.");
            rewriteSettings = true;
        }
        if (!settingsTree[PLANDOMIZER] || !settingsTree[GENERATE_SPOILER_LOG] || !settingsTree[STARTING_INVENTORY] ||
            !settingsTree[EXCLUDED_LOCATIONS] || !settingsTree[MIXED_ENTRANCE_POOLS])
        {
            rewriteSettings = true;
        }
        if (!preferencesTree[PLANDOMIZER_PATH])
        {
            rewritePreferences = true;
        }

        // Rewrite files if deemed necessary
        if (allowRewrite && rewriteSettings)
        {
            utility::platform::Log(std::string("Rewriting ") + settingsPath.generic_string());
            this->WriteSettingsToFile(settingsPath);
        }
        if (allowRewrite && rewritePreferences)
        {
            utility::platform::Log(std::string("Rewriting ") + preferencesPath.generic_string());
            this->WritePreferencesToFile(preferencesPath);
        }
    }

    YAML::Node Config::SettingsToYaml()
    {
        YAML::Node out;
        for (auto& settings : this->_settingsList)
        {
            out[SEED] = this->_seed;
            out[PLANDOMIZER] = this->_isUsingPlandomizer;
            out[GENERATE_SPOILER_LOG] = this->_isGeneratingSpoilerLog;

            // Sort settings by id to keep relevant settings close together in the settings file
            std::list<std::string> sortedNames = {};
            for (auto& [settingName, setting] : settings.GetMap())
            {
                sortedNames.push_back(settingName);
            }
            sortedNames.sort(
                [&](const auto& a, const auto& b)
                { return settings.GetMap().at(a).GetInfo()->GetID() < settings.GetMap().at(b).GetInfo()->GetID(); });

            for (const auto& settingName : sortedNames)
            {
                auto& setting = settings.GetMap().at(settingName);
                if (setting.GetInfo()->GetType() == settings::Type::STANDARD)
                {
                    out[settingName] = setting.GetCurrentOption();
                }
            }

            out[STARTING_INVENTORY] = std::map<std::string, int>();
            for (const auto& [itemName, count] : settings.GetStartingInventory())
            {
                out[STARTING_INVENTORY][itemName] = count;
            }

            out[EXCLUDED_LOCATIONS] = std::list<std::string>();
            for (const auto& locationName : settings.GetExcludedLocations())
            {
                out[EXCLUDED_LOCATIONS].push_back(locationName);
            }

            out[MIXED_ENTRANCE_POOLS] = std::list<std::list<std::string>>();
            int i = 0;
            for (const auto& pool : settings.GetMixedEntrancePools())
            {
                out[MIXED_ENTRANCE_POOLS].push_back({});
                for (const auto& type : pool)
                {
                    out[MIXED_ENTRANCE_POOLS][i].push_back(type);
                }
                i += 1;
            }
        }

        return out;
    }

    YAML::Node Config::PreferencesToYaml()
    {
        YAML::Node out;
        for (auto& settings : this->_settingsList)
        {
            out[PLANDOMIZER_PATH] = this->_plandomizerPath.generic_string();
            for (auto& [settingName, setting] : settings.GetMap())
            {
                if (setting.GetInfo()->GetType() == settings::Type::PREFERENCE)
                {
                    out[settingName] = setting.GetCurrentOption();
                }
            }
        }

        return out;
    }

    void Config::WriteSettingsToFile(const fspath& settingsPath)
    {
        std::ofstream outputFile(settingsPath);
        if (outputFile.is_open() == false)
        {
            throw std::runtime_error("Unable to open settings file \"" + settingsPath.generic_string() + "\" for writing.");
        }

        outputFile << this->SettingsToYaml();
        outputFile.close();
    }

    void Config::WritePreferencesToFile(const fspath& preferencesPath)
    {
        std::ofstream outputFile(preferencesPath);
        if (outputFile.is_open() == false)
        {
            throw std::runtime_error("Unable to open preferences file \"" + preferencesPath.generic_string() +
                                     "\" for writing.");
        }

        outputFile << this->PreferencesToYaml();
        outputFile.close();
    }

    void Config::WriteToFile(const fspath& settingsPath, const fspath& preferencesPath) {
        WriteSettingsToFile(settingsPath);
        WritePreferencesToFile(preferencesPath);
    }

    std::string Config::GetHash(bool generateIfEmpty)
    {
        if (this->_hash.empty() && generateIfEmpty)
        {
            this->_hash = seed::GenerateHash();
        }

        return this->_hash;
    }

    std::string Config::GetPermalink() {
        // If a permalink was set, return that instead
        if (!this->_permalink.empty()) {
            return this->_permalink;
        }

        std::string permalink{};

        permalink += RANDOMIZER_VERSION;
        permalink += '\0';
        permalink += std::to_string(settings::GetSettingInfoHash());
        permalink += '\0';
        permalink += this->_seed;
        permalink += '\0';

        // Pack the settings up
        PackedBitsWriter bitsWriter{};
        // Regular Settings
        for (const auto& [settingName, setting] : GetSettings().GetMap()) {
            if (setting.GetInfo()->GetType() != settings::Type::STANDARD) {
                continue;
            }

            auto optionIndex = setting.GetCurrentOptionIndex();
            auto bitLength = setting.GetInfo()->GetOptionsBitLength();
            bitsWriter.write(optionIndex, bitLength);
        }
        // Starting Items
        const auto& startingInventory = GetSettings().GetStartingInventory();
        for (const auto& [itemName, maxCount] : logic::item_pool::GetValidStartingInventoryItems()) {
            int count = 0;
            if (startingInventory.contains(itemName)) {
                count = startingInventory.at(itemName);
            }

            int numBits = std::bit_width(static_cast<uint32_t>(maxCount));
            bitsWriter.write(count, numBits);
        }
        // Excluded Locations
        for (const auto& locationName : logic::location::GetAllRandomizerLocationNames()) {
            if (GetSettings().GetExcludedLocations().contains(locationName)) {
                bitsWriter.write(1, 1);
            } else {
                bitsWriter.write(0, 1);
            }
        }
        // Mixed Entrance Pools
        const auto& mixedEntrancePools = GetSettings().GetMixedEntrancePools();
        const auto& possibleMixedPoolTypes = logic::entrance_shuffle::GetPossibleMixedPoolTypes();
        for (const auto& entranceType : possibleMixedPoolTypes) {
            uint32_t poolIndex = 0;
            uint32_t counter = 0;
            for (const auto& pool : mixedEntrancePools) {
                counter += 1;
                if (utility::container::ElementInContainer(pool, entranceType)) {
                    poolIndex = counter;
                    break;
                }
            }
            bitsWriter.write(poolIndex, std::bit_width(possibleMixedPoolTypes.size()));
        }

        bitsWriter.flush();
        for (auto byte : bitsWriter.bytes) {
            permalink += byte;
        }
        permalink = b64_encode(permalink);

        return permalink;
    }

    std::optional<std::string> Config::LoadFromPermalink(std::string b64permalink) {

        // Strip trailing spaces
        std::erase_if(b64permalink, [](unsigned char ch){ return std::isspace(ch); });

        std::string permalink = b64_decode(b64permalink);
        // Empty string gets returned if there was an error
        if (permalink.empty()) {
            return "Pasted permalink is invalid and could not be decoded. (You likely miscopied it.)";
        }

        // Split the string into 4 parts along the null terminator delimiter
        // 1st part - Version string
        // 2nd part - setting info hash
        // 3rd part - seed string
        // 4th part - packed bits representing settings
        std::vector<std::string> permaParts = {};
        constexpr char delimiter = '\0';
        size_t pos = permalink.find(delimiter);
        while (pos != std::string::npos) {
            if (permaParts.size() != 3) {
                permaParts.push_back(permalink.substr(0, pos));
                permalink.erase(0, pos + 1);
            }
            else {
                permaParts.push_back(permalink);
                break;
            }

            pos = permalink.find(delimiter);
        }

        if (permaParts.size() != 4) {
            return "Pasted permalink does not have the expected number of parts.";
        }

        const auto& permaVersion = permaParts[0];
        const auto& permaSettingsInfoHash = permaParts[1];
        const auto& permaSeed = permaParts[2];
        const auto& permaPackedSettings = permaParts[3];

        if (permaSettingsInfoHash != std::to_string(settings::GetSettingInfoHash())) {
            return fmt::format("Pasted permalink was generated with an incompatible Randomizer version.\n"
                                  "Your version: {}\nPermalink version: {}", RANDOMIZER_VERSION, permaVersion);
        }

        const std::vector<char> bytes(permaPackedSettings.begin(), permaPackedSettings.end());
        PackedBitsReader bitsReader{bytes};
        Config newConfig{};

        for (auto& [settingName, setting] : newConfig.GetSettings().GetMap()) {
            if (setting.GetInfo()->GetType() != settings::Type::STANDARD) {
                continue;
            }

            auto bitLength = setting.GetInfo()->GetOptionsBitLength();
            auto optionIndex = bitsReader.read(bitLength);
            setting.SetCurrentOption(optionIndex);
        }
        // Starting Items
        auto& startingInventory = newConfig.GetSettings().GetModifiableStartingInventory();
        for (const auto& [itemName, maxCount] : logic::item_pool::GetValidStartingInventoryItems()) {
            int count = 0;
            int numBits = std::bit_width(static_cast<uint32_t>(maxCount));
            count = bitsReader.read(numBits);

            if (count > 0) {
                startingInventory[itemName] = count;
            }
        }
        // Excluded Locations
        auto& excludedLocations = newConfig.GetSettings().GetModifiableExcludedLocations();
        for (const auto& locationName : logic::location::GetAllRandomizerLocationNames()) {
            if (bitsReader.read(1) == 1) {
                excludedLocations.insert(locationName);
            }
        }

        // Mixed Entrance Pools
        auto& mixedEntrancePools = newConfig.GetSettings().GetModifiableMixedEntrancePools();
        const auto& possibleMixedPoolTypes = logic::entrance_shuffle::GetPossibleMixedPoolTypes();
        for (const auto& entranceType : possibleMixedPoolTypes) {
            auto poolIndex = bitsReader.read(std::bit_width(possibleMixedPoolTypes.size()));
            if (poolIndex == 0) {
                continue;
            }
            poolIndex -= 1;
            if (poolIndex < possibleMixedPoolTypes.size()) {
                while (poolIndex >= mixedEntrancePools.size()) {
                    mixedEntrancePools.push_back({});
                }
                auto& pool = *std::next(mixedEntrancePools.begin(), poolIndex);
                pool.push_back(entranceType);
            }
        }

        if (!bitsReader.reached_last_byte()) {
            return "Pasted permalink is incorrect length. (You likely miscopied it.)";
        }

        // Once we've gotten all the info, copy it over to this config
        this->SetSeed(permaSeed);
        for (auto& settings : this->_settingsList) {
            for (auto& [settingName, setting] : settings.GetMap()) {
                if (setting.GetInfo()->GetType() == settings::Type::STANDARD) {
                    setting.SetCurrentOption(newConfig.GetSettings().GetMap().at(settingName).GetCurrentOptionIndex());
                }
            }
            settings.GetModifiableExcludedLocations() = newConfig.GetSettings().GetExcludedLocations();
            settings.GetModifiableStartingInventory() = newConfig.GetSettings().GetStartingInventory();
            settings.GetModifiableMixedEntrancePools() = newConfig.GetSettings().GetMixedEntrancePools();
        }

        return std::nullopt;
    }

    int SeedRNG(Config& config,
                bool resolveNonStandardRandom /* = false */,
                bool ignoreInvalidPlandomizer /* = true */)
    {
        // Seed with system time incase we have to choose random preferences during seeding
        auto seed = static_cast<uint32_t>(std::random_device {}());
        utility::random::RandomInit(seed);

        // Seed the rng using a combination of the seed and standard settings
        std::string hashStr = config.GetSeed();
        for (auto& settings : config.GetSettingsList())
        {
            for (auto& [settingName, setting] : settings.GetMap())
            {
                if (setting.GetInfo()->GetType() == settings::Type::STANDARD)
                {
                    hashStr += settingName + setting.GetCurrentOption();
                }
                else if (resolveNonStandardRandom)
                {
                    setting.ResolveIfRandom();
                }
            }

            // Special handling for other settings
            for (const auto& [itemName, count] : settings.GetStartingInventory())
            {
                hashStr += itemName + std::to_string(count);
            }

            for (const auto& locationName : settings.GetExcludedLocations())
            {
                hashStr += locationName;
            }

            for (const auto& pool : settings.GetMixedEntrancePools())
            {
                for (const auto& type : pool)
                {
                    hashStr += type;
                }
            }
        }

        // Change the seed if we're using plandomizer
        if (config.IsUsingPlandomizer())
        {
            std::string plandomizerContents;
            auto retVal = utility::file::GetContents(config.GetPlandomizerPath(), plandomizerContents);
            if (!ignoreInvalidPlandomizer && retVal != 0)
            {
                LOG_TO_ERROR("Could not read plandomizer file at \"" + config.GetPlandomizerPath().generic_string() + "\"");
                return 1;
            }
            hashStr += plandomizerContents;
        }

        // Change the seed if we're generating a spoiler log
        if (config.IsGeneratingSpoilerLog())
        {
            hashStr += "Spoiler Log: True";
        }

        const size_t integerSeed = utility::crc32(hashStr.data(), hashStr.length());
        utility::random::RandomInit(integerSeed);

        return 0;
    }
} // namespace randomizer::seedgen::config
