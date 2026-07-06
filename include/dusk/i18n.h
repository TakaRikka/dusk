#pragma once

#include <string_view>

/**
 * Lightweight UI localization ("i18n") for Dusklight.
 *
 * Strings are keyed by their English source text (msgid), gettext-style. Wrap a
 * user-facing UI literal in tr(...) and it will be translated into the language
 * selected by the "ui.language" setting, falling back to the English source when
 * no translation exists for the active language.
 *
 *     button->set_text(tr("Play"));
 *
 * The English source doubles as the key, so English builds are unaffected and
 * adding a new string never requires touching existing translation tables.
 */
namespace dusk::i18n {

/**
 * \brief Translate an English source string into the active UI language.
 *
 * @param msgid A null-terminated English source string (used as the lookup key).
 * @return The translation for the active UI language, or \p msgid itself when no
 *         translation exists. The returned pointer is always null-terminated and
 *         has static storage duration.
 */
const char* tr(const char* msgid) noexcept;

}  // namespace dusk::i18n

namespace dusk {
// Bring tr() into the dusk namespace so it resolves unqualified from dusk::ui.
using i18n::tr;
}  // namespace dusk
