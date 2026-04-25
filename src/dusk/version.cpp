#include "dusk/version.hpp"

#include "dusk/logging.h"

namespace dusk::version {

using namespace std::string_view_literals;

static bool versionInitialized;
static GameVersion gameVersion;
static DVDDiskID diskId;

void init() {
    versionInitialized = true;

    if (!DVDLowReadDiskID(&diskId, nullptr)) {
        DuskLog.fatal("DVDLowReadDiskID failed to return instantly.");
    }

    std::string_view company(diskId.company, sizeof(diskId.company));
    std::string_view game(diskId.gameName, sizeof(diskId.gameName));

    if (company != "01"sv) {
        DuskLog.fatal("Wrong company ID in disc: {}", company);
    }

    if (game == "GZ2E"sv) {
        gameVersion = GameVersion::GcnUsa;
    } else if (game == "GZ2P") {
        gameVersion = GameVersion::GcnPal;
    } else {
        // TODO: Handle remaining valid versions.
        DuskLog.fatal("Unknown/unsupported game version in disc: {}", game);
    }

    DuskLog.info("Loaded game disc is {}{}", game, company);
}

bool isGcn() {
    return getGameVersion() == GameVersion::GcnUsa
        || getGameVersion() == GameVersion::GcnPal
        || getGameVersion() == GameVersion::GcnJpn;
}

bool isWii() {
    return getGameVersion() == GameVersion::WiiUsaRev0
        || getGameVersion() == GameVersion::WiiUsa
        || getGameVersion() == GameVersion::WiiPal
        || getGameVersion() == GameVersion::WiiJpn
        || getGameVersion() == GameVersion::WiiKor;
}

bool isPalOrAtLeastWiiR2() {
    return getGameVersion() == GameVersion::GcnPal || getGameVersion() >= GameVersion::WiiUsa;
}

bool isRegionJpn() {
    return getGameVersion() == GameVersion::WiiJpn || getGameVersion() == GameVersion::GcnJpn;
}

bool isRegionPal() {
    return getGameVersion() == GameVersion::WiiPal || getGameVersion() == GameVersion::GcnPal;
}

bool isRegionUsa() {
    return getGameVersion() == GameVersion::WiiUsa || getGameVersion() == GameVersion::WiiUsaRev0
        || getGameVersion() == GameVersion::GcnUsa;
}

GameVersion getGameVersion() {
    if (!versionInitialized) {
        abort();
    }

    return gameVersion;
}

const DVDDiskID& getDiskID() {
    return diskId;
}

}  // namespace dusk::version