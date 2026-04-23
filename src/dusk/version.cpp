#include "dusk/version.hpp"

#include "dusk/logging.h"

namespace dusk::version {

using namespace std::string_view_literals;

static GameVersion gameVersion;

void init() {
    DVDDiskID diskId;
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
    return gameVersion == GameVersion::GcnUsa
        || gameVersion == GameVersion::GcnPal
        || gameVersion == GameVersion::GcnJpn;
}

bool isWii() {
    return gameVersion == GameVersion::WiiUsaRev0
        || gameVersion == GameVersion::WiiUsa
        || gameVersion == GameVersion::WiiPal
        || gameVersion == GameVersion::WiiJpn
        || gameVersion == GameVersion::WiiKor;
}

bool isRegionJpn() {
    return gameVersion == GameVersion::WiiJpn || gameVersion == GameVersion::GcnJpn;
}

bool isRegionPal() {
    return gameVersion == GameVersion::WiiPal || gameVersion == GameVersion::GcnPal;
}

GameVersion getGameVersion() {
    return gameVersion;
}

}  // namespace dusk::version