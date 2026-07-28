#pragma once

#include <string>

#include "settings.hpp"
#include "../utility/path.hpp"

// forward declaration
namespace YAML
{
    class Node;
}

namespace randomizer::seedgen::config
{

    class Config
    {
       public:
        Config();
        Config(const fspath& settingsPath, const fspath& preferencesPath);

        fspath GetPlandomizerPath() const { return this->_plandomizerPath; }
        void SetSeed(const std::string& newSeed) { this->_seed = newSeed; }
        std::string GetSeed() const { return this->_seed; }
        auto& GetSettingsList() { return this->_settingsList; }
        auto& GetSettings() { return this->_settingsList.front();}
        bool IsUsingPlandomizer() const { return this->_isUsingPlandomizer; }
        bool IsGeneratingSpoilerLog() const { return this->_isGeneratingSpoilerLog; }
        void ResetSettingsToDefault();
        void ResetPreferencesToDefault();

        void LoadFromFile(const fspath& settingsPath,
                          const fspath& preferencesPath,
                          bool createIfNotFound = true,
                          bool allowRewrite = true);
        YAML::Node SettingsToYaml();
        YAML::Node PreferencesToYaml();
        void WriteSettingsToFile(const fspath& filePath);
        void WritePreferencesToFile(const fspath& preferencesPath);
        void WriteToFile(const fspath& filePath, const fspath& preferencesPath);

        std::optional<std::string> LoadFromPermalink(std::string b64permalink);
        std::string GetPermalink();
        void SetPermalink(const std::string& newPermalink) { this->_permalink = newPermalink; }

        /**
         *  @brief Returns the hash for the config.
         *  @param generateIfEmpty Generates a new hash if the current hash is empty
         *
         *  @return The hash as a string
         */
        std::string GetHash(bool generateIfEmpty = true);
        void SetHash(const std::string& newHash) { this->_hash = newHash; }

       private:
        fspath _plandomizerPath;

        std::string _seed;
        std::string _hash;
        std::string _permalink;
        std::list<settings::Settings> _settingsList;
        bool _isUsingPlandomizer = false;
        bool _isGeneratingSpoilerLog = true;
    };

    int SeedRNG(Config& config, bool resolveNonStandardRandom = false, bool ignoreInvalidPlandomizer = true);
} // namespace randomizer::seedgen::config
