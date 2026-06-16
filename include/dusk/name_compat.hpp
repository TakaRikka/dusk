#pragma once

#include "dusk/string.hpp"

namespace dusk::name_compat {

bool isLegacyTpHdChineseDefaultPlayerName(const char* name);
void copyPlayerNameForDisplay(TEXT_SPAN dst, const char* name);
void normalizeLoadedSaveNames();

}  // namespace dusk::name_compat
