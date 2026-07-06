#include "dusk/i18n.h"

#include <absl/container/flat_hash_map.h>

#include "dusk/settings.h"

namespace dusk::i18n {
namespace {

using TranslationTable = absl::flat_hash_map<std::string_view, const char*>;

// Italian translations, keyed by English source string.
const TranslationTable& italian() {
    static const TranslationTable kTable = {
        {"Play", "Gioca"},
        {"Select Disc Image", "Seleziona immagine disco"},
        {"Settings", "Impostazioni"},
        {"Quit", "Esci"},
        {"Enable VSync", "Attiva VSync"},
        {"Interface Language", "Lingua interfaccia"},
    };
    return kTable;
}

// Returns the translation table for the given language, or nullptr for English
const TranslationTable* table_for(UiLanguage language) {
    switch (language) {
    case UiLanguage::Italian:
        return &italian();
    case UiLanguage::English:
        break;
    }
    return nullptr;
}

}  // namespace

const char* tr(const char* msgid) noexcept {
    const TranslationTable* table = table_for(getSettings().ui.language.getValue());
    if (table == nullptr) {
        return msgid;
    }
    if (const auto it = table->find(std::string_view{msgid}); it != table->end()) {
        return it->second;
    }
    return msgid;
}

}  // namespace dusk::i18n
