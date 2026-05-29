#include "dusk/i18n.hpp"
#include "dusk/config.hpp"
#include <aurora/lib/logging.hpp>

#include <unordered_map>
#include <fstream>
#include <array>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace dusk {
namespace {
    constexpr std::array<const char*, 9> kLanguageNames = {
        "English", "German", "French", "Spanish", "Italian", "Dutch", "Japanese", "Korean", "Chinese",
    };

    std::unordered_map<std::string, std::string> translationMap;
    int g_LastLanguageIdx = -1;

    aurora::Module I18nLog{"dusk::i18n"};
}

void load_translations() {
    int currentLang = static_cast<int>(getSettings().game.language.getValue());
    
    if (currentLang == g_LastLanguageIdx) return;
    
    translationMap.clear();
    g_LastLanguageIdx = currentLang;

    if (currentLang >= kLanguageNames.size()) return;
    std::string currentLangName = kLanguageNames[currentLang];

    if (currentLangName == "English") return;

    std::string fileName = "";
    
    if (currentLangName == "Spanish")   fileName = "lang_ES.json";
    else if (currentLangName == "German")    fileName = "lang_DE.json";
    else if (currentLangName == "French")    fileName = "lang_FR.json";
    else if (currentLangName == "Italian")   fileName = "lang_IT.json";
    else if (currentLangName == "Dutch")     fileName = "lang_NL.json";
    else if (currentLangName == "Japanese")  fileName = "lang_JA.json";
    else if (currentLangName == "Korean")    fileName = "lang_KO.json";
    else if (currentLangName == "Chinese")   fileName = "lang_ZH.json";

    if (!fileName.empty()) {
        std::filesystem::path filePath = std::filesystem::current_path() / "res" / "Languages" / fileName;
        std::ifstream file(filePath);
        
        if (file.is_open()) {
            try {
                nlohmann::json j;
                file >> j; 
                for (auto& [key, value] : j.items()) {
                    if (value.is_string()) {
                        translationMap[key] = value.get<std::string>();
                    } else {
                        I18nLog.warn("Translation key '{}' is not a string, skipping.", key);
                    }
                }
                I18nLog.info("Loaded translation file: {}", filePath.string());
            } catch (const std::exception& e) {
                I18nLog.error("Failed to parse translation file {}: {}", filePath.string(), e.what());
            }
            file.close();
        } else {
            I18nLog.error("Translation file not found: {}", filePath.string());
        }
    }
}

Rml::String tr(const std::string& text) {
    auto it = translationMap.find(text);
    if (it != translationMap.end()) {
        return it->second;
    }
    return text;
}

} // namespace dusk
