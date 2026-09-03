#include "dusk/language.hpp"

#include "dusk/version.hpp"

namespace dusk::language {
namespace {

constexpr GameLanguage kEnglishOnly[] = {GameLanguage::English};
constexpr GameLanguage kJapaneseOnly[] = {GameLanguage::Japanese};
constexpr GameLanguage kPalLanguages[] = {
    GameLanguage::English,
    GameLanguage::German,
    GameLanguage::French,
    GameLanguage::Spanish,
    GameLanguage::Italian,
};
constexpr GameLanguage kWiiUsaLanguages[] = {
    GameLanguage::English,
    GameLanguage::French,
    GameLanguage::Spanish,
};

}  // namespace

std::span<const GameLanguage> available_languages(const iso::DiscInfo& info) noexcept {
    switch (info.region) {
    case iso::Region::Japan:
        return kJapaneseOnly;
    case iso::Region::Europe:
        return kPalLanguages;
    case iso::Region::NorthAmerica:
        if (info.platform == iso::Platform::Wii && info.revision == 2) {
            return kWiiUsaLanguages;
        }
        return kEnglishOnly;
    default:
        return kEnglishOnly;
    }
}

const char* language_name(GameLanguage language) noexcept {
    switch (language) {
    case GameLanguage::English:
        return "English";
    case GameLanguage::German:
        return "German";
    case GameLanguage::French:
        return "French";
    case GameLanguage::Spanish:
        return "Spanish";
    case GameLanguage::Italian:
        return "Italian";
    case GameLanguage::Japanese:
        return "Japanese";
    }
    return "English";
}

const char* msg_folder() noexcept {
    using namespace version;

    switch (getSettings().game.language.getValue()) {
    case GameLanguage::German:
        return "Msgde";
    case GameLanguage::French:
        return "Msgfr";
    case GameLanguage::Spanish:
        return "Msgsp";
    case GameLanguage::Italian:
        return "Msgit";
    case GameLanguage::Japanese:
        return "Msgjp";
    case GameLanguage::English:
    default:
        return isRegionPal() ? "Msguk" : "Msgus";
    }
}

}  // namespace dusk::language
