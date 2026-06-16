#include "dusk/name_compat.hpp"

#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "dusk/settings.h"
#include "dusk/version.hpp"

namespace dusk::name_compat {

namespace {

bool shouldUseTpHdChineseNameCompatibility() {
    return tphd_active() && version::isRegionPal() &&
           getSettings().game.enableChineseNameKeyboard.getValue();
}

void getDefaultPlayerName(TEXT_SPAN dst) {
    dMeter2Info_getString(0x382, dst, nullptr);
}

}  // namespace

bool isLegacyTpHdChineseDefaultPlayerName(const char* name) {
    if (!shouldUseTpHdChineseNameCompatibility()) {
        return false;
    }

    const auto* bytes = reinterpret_cast<const unsigned char*>(name);

    // Old PAL default-name bytes render as "üß" with the TPHD Chinese font.
    return bytes[0] == 0xFC && bytes[1] == 0xDF && bytes[2] == 0;
}

void copyPlayerNameForDisplay(TEXT_SPAN dst, const char* name) {
    if (isLegacyTpHdChineseDefaultPlayerName(name)) {
        getDefaultPlayerName(dst);
    } else {
        SafeStringCopy(dst, name);
    }
}

void normalizeLoadedSaveNames() {
    if (!isLegacyTpHdChineseDefaultPlayerName(dComIfGs_getPlayerName())) {
        return;
    }

    char name[32];
    getDefaultPlayerName(name);
    dComIfGs_setPlayerName(name);
}

}  // namespace dusk::name_compat
